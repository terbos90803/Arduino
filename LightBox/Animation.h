class Animation {
    static constexpr uint8_t max_byte = 0xff;
    static constexpr uint32_t max_power = numPixels * 3 * max_byte;
    static constexpr uint32_t power_limit = 25000;
    static constexpr float max_brightness = 1.0f;
    static constexpr float min_brightness = (float)power_limit / max_power;
    static constexpr float scale = (min_brightness - max_brightness) / (max_power - power_limit);

    virtual uint32_t getColor(const Pixel &p) = 0;

    inline uint32_t rgb_power(uint32_t color) {
      return ((color >> 16) & 0xff) + ((color >> 8) & 0xff) + (color & 0xff);
    }

    inline uint8_t calcBrightness(uint32_t power) {
      if (power <= power_limit)
        return max_byte;
        
      // Calculate the strip brightness in order to limit the max power
      int b = ((power - power_limit) * scale + max_brightness) * max_byte;
      if (b < 0)
        return 0;
      else if (b > max_byte)
        return max_byte;
      return b;
    }
    
  public:
    virtual void begin() = 0;
    virtual void update() = 0;

    void display(Adafruit_NeoPixel &strip) {
      uint32_t power = 0;
      for (int i = 0; i < strip.numPixels(); ++i) {
        uint32_t color = strip.gamma32(getColor(pixels[i]));
        power += rgb_power(color);
        strip.setPixelColor(i, color);
      }
      auto b = calcBrightness(power);
      strip.setBrightness(b);
      strip.show();
    }

  protected:
    uint32_t blend(uint32_t c1, uint32_t c2) {
      uint32_t r = (c1 & 0xff0000) + (c2 & 0xff0000);
      uint32_t g = (c1 & 0x00ff00) + (c2 & 0x00ff00);
      uint32_t b = (c1 & 0x0000ff) + (c2 & 0x0000ff);

      r = r > 0xff0000 ? 0xff0000 : r;
      g = g > 0x00ff00 ? 0x00ff00 : g;
      b = b > 0x0000ff ? 0x0000ff : b;

      return r | g | b;
    }
};
