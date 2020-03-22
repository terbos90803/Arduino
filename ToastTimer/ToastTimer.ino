/***************************************************
  ToastTimer
  
  A Toastmaster speech timer

****************************************************/

#include "Battery.h"
#include "Buttons.h"
#include "NumericDisplay.h"
#include "Stoplight.h"

Buttons buttons;
NumericDisplay display;
Stoplight stoplight;

void setup() {
}

int count = 0;

void loop() {

  display.print(getBatteryVoltage());
  delay(500);

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

}
