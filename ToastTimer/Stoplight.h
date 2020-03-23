/***************************************************
  This is a library for our I2C LED Backpacks

****************************************************/

#include <Adafruit_NeoPixel.h>

class Stoplight {

    const int PIN = 2;
    const int LENGTH = 8;

    // Parameter 1 = number of pixels in strip
    // Parameter 2 = Arduino pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(LENGTH, PIN, NEO_GRB + NEO_KHZ800);

  public:
    enum Stoplight_e {SL_OFF, SL_RED, SL_YELLOW, SL_GREEN};

    void begin() {
      // NeoPixels
      strip.begin();
      strip.setBrightness(50);
      strip.show(); // Initialize all pixels to 'off'
    }

    void set(Stoplight_e state) {
      strip.fill();

      switch (state) {
        case SL_RED:
          strip.fill(strip.Color(255, 0, 0), 0, 7);
          break;

        case SL_YELLOW:
          strip.fill(strip.Color(255, 255, 0), 0, 7);
          break;

        case SL_GREEN:
          strip.fill(strip.Color(0, 255, 0), 0, 7);
          break;
      }
      strip.show();
    }

};
