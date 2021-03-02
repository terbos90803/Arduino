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
        Serial.print("animation.display()=");
        Serial.print(i);
        Serial.print(", ");
        Serial.println(color);
      }
      strip.show();
      Serial.println(++frame);
    }

  protected:
    uint32_t blend(uint32_t c1, uint32_t c2) {
      uint32_t r = (c1 >> 16 & 0xff) + (c2 >> 16 & 0xff);
      uint32_t g = (c1 >> 8 & 0xff) + (c2 >> 8 & 0xff);
      uint32_t b = (c1 & 0xff) + (c2 & 0xff);

      r = r < 0xff ? 0xff : r;
      g = g < 0xff ? 0xff : g;
      b = b < 0xff ? 0xff : b;

      return (r << 16) | (g << 8) | (b);
    }

  private:
    virtual uint32_t getColor(int x, int y) = 0;
};
