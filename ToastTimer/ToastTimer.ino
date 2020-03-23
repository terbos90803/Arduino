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

int count = 0;

void loop() {

#if 1

  switch (mode) {
    case M_SELECT:
      // SELECT MODE:
      // - Display selected speech length, no stoplight
      // - Gray button rotates through preset speeches
      // - Green button resets and starts timer
      // - Red button checks battery

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
        speeches[spIx]->display(display);
        stoplight.set(Stoplight::SL_OFF);
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

#elif 1

  if (buttons.getGray()) {
    stoplight.set(Stoplight::SL_YELLOW);
  } else if (buttons.getGreen()) {
    stoplight.set(Stoplight::SL_GREEN);
  } else if (buttons.getRed()) {
    stoplight.set(Stoplight::SL_RED);
  } else {
    stoplight.set(Stoplight::SL_OFF);
  }

  unsigned long time = millis();
  int seconds = time / 1000;
  bool showColon = (time / 500) % 2;
  display.formatTime(seconds, showColon);

#else
  float batt = getBatteryVoltage();
  Serial.println(batt);
  display.print(batt);
  display.writeDisplay();


  switch (count++ % 4) {
    case 0:
      stoplight.set(Stoplight::SL_OFF);
      break;
    case 1:
      stoplight.set(Stoplight::SL_RED);
      break;
    case 2:
      stoplight.set(Stoplight::SL_YELLOW);
      break;
    case 3:
      stoplight.set(Stoplight::SL_GREEN);
      break;
  }
#endif

  delay(50);
}
