class LavaLamp : public Animation
{
  public:
    virtual void begin() {

    }

    virtual void update() {

    }

    virtual void display(Adafruit_NeoPixel &strip) {
      for (int i = 0; i < strip.numPixels(); ++i) {
        uint32_t color = 0;
        
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }
};
