/***************************************************
  ToastTimer

  A Toastmaster speech timer

****************************************************/

#include "Battery.h"
#include "Buttons.h"
#include "NumericDisplay.h"
#include "Stoplight.h"

Buttons buttons;
NumericDisplay display = NumericDisplay();
Stoplight stoplight;

void setup() {
  Serial.begin(9600);
  Serial.println("ToastTimer");

  display.begin();
  stoplight.begin();
}

int count = 0;

void loop() {

#if 0

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
