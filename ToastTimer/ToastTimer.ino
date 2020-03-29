/***************************************************
  ToastTimer

  A Toastmaster speech timer

****************************************************/

#include "Battery.h"
#include "Buttons.h"
#include "NumericDisplay.h"
#include "Stoplight.h"
#include "Speech.h"
#include "Timer.h"

Buttons buttons;
NumericDisplay display;
Stoplight stoplight;
Timer timer;

Speech tableTopic(1, 2);
Speech evaluation(2, 3);
Speech icebreaker(4, 6);
Speech prepared(5, 7);
Speech *speeches[] = {&tableTopic, &evaluation, &icebreaker, &prepared};
const int numSpeeches = sizeof(speeches) / sizeof(Speech *);
int spIx = 0;

enum { M_SELECT, M_RUNNING, M_STOP } mode = M_SELECT;

void setup() {
  Serial.begin(9600);
  Serial.println("ToastTimer v1.0");

  display.begin();
  stoplight.begin();
}

void loop() {

  switch (mode) {
    case M_SELECT:
      // SELECT MODE:
      // - Display selected speech length, no stoplight
      // - Gray button rotates through preset speeches
      // - Green button resets and starts timer
      // - Red button checks battery

      speeches[spIx]->display(display);
      stoplight.set(Stoplight::SL_OFF);

      if (buttons.getRed()) {
        float batt = getBatteryVoltage();
        display.print(batt);
        display.writeDisplay();
      } else {
        if (buttons.getGrayPressed()) {
          spIx = (spIx + 1) % numSpeeches;
        }
        if (buttons.getGreenPressed()) {
          mode = M_RUNNING;
          timer.reset();
          timer.start();
        }
      }
      break;

    case M_RUNNING:
      // RUNNING MODE:
      // - Display elapsed time and stoplight status
      // - Gray button does nothing
      // - Green button does nothing
      // - Red button stops timer

      timer.display(display, true);
      speeches[spIx]->checkTime(timer.getSeconds(), stoplight);
      if (buttons.getRedPressed()) {
        mode = M_STOP;
        timer.stop();
      }
      break;

    case M_STOP:
      // STOP MODE:
      // - Display elapsed time, no stoplight
      // - Gray button returns to Select mode
      // - Green button resumes timer
      // - Red button does nothing

      timer.display(display, false);
      stoplight.set(Stoplight::SL_OFF);
      if (buttons.getGrayPressed()) {
        mode = M_SELECT;
      } else if (buttons.getGreenPressed()) {
        mode = M_RUNNING;
        timer.start();
      }
      break;

    default:
      // Shouldn't ever get here.  If we do, jump back to select mode.
      mode = M_SELECT;
      break;
  }

  delay(50);
}
