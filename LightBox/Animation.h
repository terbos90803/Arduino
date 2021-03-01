class Animation {
  public:
    virtual void begin() = 0;
    virtual void update() = 0;
    virtual void display(Adafruit_NeoPixel &strip) = 0;

    uint32_t blend(uint32_t c1, uint32_t c2) {
      uint32_t r = (c1 >> 16 & 0xff) + (c2 >> 16 & 0xff);
      uint32_t g = (c1 >> 8 & 0xff) + (c2 >> 8 & 0xff);
      uint32_t b = (c1 & 0xff) + (c2 & 0xff);

      r = r < 0xff ? 0xff : r;
      g = g < 0xff ? 0xff : g;
      b = b < 0xff ? 0xff : b;

      return (r << 16) | (g << 8) | (b);
    }
};
