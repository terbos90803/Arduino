class Animation {
    int frame = 0;

  public:
    virtual void begin() = 0;
    virtual void update() = 0;

    void display(Adafruit_NeoPixel &strip) {
      for (int i = 0; i < strip.numPixels(); ++i) {
        int x = pixels[i].x;
        int y = pixels[i].y;
        uint32_t color = getColor(x, y);
        strip.setPixelColor(i, strip.gamma32(color));
      }
      strip.show();
    }

  protected:
#if 0
    uint32_t blend(uint32_t c1, uint32_t c2) {
      uint32_t r = (c1 >> 16 & 0xff) + (c2 >> 16 & 0xff);
      uint32_t g = (c1 >> 8 & 0xff) + (c2 >> 8 & 0xff);
      uint32_t b = (c1 & 0xff) + (c2 & 0xff);

      r = r > 0xff ? 0xff : r;
      g = g > 0xff ? 0xff : g;
      b = b > 0xff ? 0xff : b;

      return (r << 16) | (g << 8) | (b);
    }
#else
    uint32_t blend(uint32_t c1, uint32_t c2) {
      uint32_t r = (c1 & 0xff0000) + (c2 & 0xff0000);
      uint32_t g = (c1 & 0x00ff00) + (c2 & 0x00ff00);
      uint32_t b = (c1 & 0x0000ff) + (c2 & 0x0000ff);

      r = r > 0xff0000 ? 0xff0000 : r;
      g = g > 0x00ff00 ? 0x00ff00 : g;
      b = b > 0x0000ff ? 0x0000ff : b;

      return r | g | b;
    }
#endif

  private:
    virtual uint32_t getColor(int x, int y) = 0;
};
