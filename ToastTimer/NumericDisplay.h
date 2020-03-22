/***************************************************
  This is a library for the Adafruit I2C LED Backpacks

****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

class NumericDisplay {

    Adafruit_7segment matrix = Adafruit_7segment();

  public:
    NumericDisplay() {
      // 7-seg matrix
      matrix.begin(0x70);
    }

    void print(float val) {
      matrix.print(val);
      matrix.writeDisplay();
    }

    void print(int val) {
      matrix.println(val);
      matrix.writeDisplay();
    }

#if 0
    // print with print/println
    for (uint16_t counter = 0; counter < 9999; counter++) {
      matrix.println(counter);
      matrix.writeDisplay();
      delay(10);
    }

    // method #2 - draw each digit
    uint16_t blinkcounter = 0;
    boolean drawDots = false;
    for (uint16_t counter = 0; counter < 9999; counter ++) {
      matrix.writeDigitNum(0, (counter / 1000), drawDots);
      matrix.writeDigitNum(1, (counter / 100) % 10, drawDots);
      matrix.drawColon(drawDots);
      matrix.writeDigitNum(3, (counter / 10) % 10, drawDots);
      matrix.writeDigitNum(4, counter % 10, drawDots);

      blinkcounter += 50;
      if (blinkcounter < 500) {
        drawDots = false;
      } else if (blinkcounter < 1000) {
        drawDots = true;
      } else {
        blinkcounter = 0;
      }
      matrix.writeDisplay();
      delay(10);
    }
#endif
};