/***************************************************
  This is a library for the Adafruit I2C LED Backpacks

****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class NumericDisplay : public Adafruit_7segment {

    const uint8_t DASH = 0x40;

  public:
    void begin() {
      // 7-seg matrix
      Adafruit_7segment::begin(0x70);
    }

    void formatTime(int rawSeconds, bool showColon) {
      int minutes = rawSeconds / 60;
      int seconds = rawSeconds % 60;

      clear();
      if (minutes > 9) {
        writeDigitNum(0, minutes / 10);
      }
      if (minutes > 0) {
        writeDigitNum(1, minutes % 10);
      }
      drawColon(showColon);
      writeDigitNum(3, seconds / 10);
      writeDigitNum(4, seconds % 10);
      writeDisplay();
    }

    void formatRange(int low, int high) {
      clear();
      writeDigitNum(1, low);
      writeDigitRaw(3, DASH);
      writeDigitNum(4, high);
      writeDisplay();
    }

};
