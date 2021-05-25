class Raindrops : public Animation
{
    static constexpr int numDrops = 3;
    static constexpr float rate = 1.5;
    static constexpr float ring_width = 5.0;
    static constexpr int percent_new_ring = 3;

    class Drop
    {
        float radius;
        Pixel center;
        float r1, r2, r3, r4;
        uint16_t hue;

      public:
        bool active {false};

        void start() {
          center.x = random(10, 90);
          center.y = random(10, 90);
          radius = 0;
          hue = random(120, 240) * 65536 / 360; // 120=green, 240=blue
          active = true;
        }

        void grow() {
          if (active) {
            radius += rate;
            if (radius > 150)
              active = false;

            // pre-calc two radii in square space
            float r = radius;
            r1 = r * r;
            r = max(r - ring_width, 0.0);
            r2 = r * r;
            r = max(r - ring_width, 0.0);
            r = max(r - ring_width, 0.0);
            r3 = r * r;
            r = max(r - ring_width, 0.0);
            r4 = r * r;
          }
        }

        uint32_t getColor(const Pixel &p) {
          if (!active)
            return 0;

          // calc the distance from the pixel to the ring center
          float dx = p.x - center.x;
          float dy = p.y - center.y;
          float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.

          // calc the distance from the pixel to the ring
          float b = 0.0;
          if (r3 < dsq && dsq < r2)
            b = 255.0;
          else if (r4 < dsq && dsq < r1)
            b = 128.0;

          return strip.ColorHSV(hue, 255, b); // 43690 = 240*65536/360 = blue
        }
    } drops[numDrops];

  public:
    virtual void begin() override {
      Serial.println("raindrops.begin()");
    }

    virtual void update() override {
      if (random(100) < percent_new_ring) {
        for (Drop &d : drops)
          if (!d.active) {
            d.start();
            break;
          }
      }
      for (Drop &d : drops) {
        d.grow();
      }
    }

    virtual uint32_t getColor(const Pixel &p) override {
      uint32_t c = 0;
      for (Drop &d : drops) {
        if (d.active)
          c = blend(c, d.getColor(p));
      }
      return c;
    }

} raindrops;
