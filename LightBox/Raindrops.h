class Raindrops : public Animation
{
    static constexpr int numDrops = 5;

    class Drop
    {
        static constexpr float rate = 1.0;
        float radius;
        Pixel center;

      public:
        bool active {false};

        void start() {
          center.x = random(10, 90);
          center.y = random(10, 90);
          radius = 0;
          active = true;
        }

        void grow() {
          if (active) {
            radius += rate;
            if (radius > 200)
              active = false;
          }
        }

        uint32_t getColor(const Pixel &p) {
          if (!active)
            return 0;

          // calc the distance from the pixel to the ring center
          float dx = p.x - center.x;
          float dy = p.y - center.y;
          float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.  Also gives a nice fade effect.
          //float d = sqrt(dsq);

          // calc the size of the ring based on the size (gives animation)
          float radsq = radius * radius;

          // calc the distance from the pixel to the ring
          float diff = abs(dsq - radsq); // difference in radii squared
          //float diff = abs(d - radius); // difference in radii
          float val = 255 - diff / 5; // The ring is full brightness.  brightness falls off further from the ring.
          float b = val > 0 ? val : 0; // clip to avoid negative values

          return strip.ColorHSV(43690, 255, b); // 43690 = 240*65536/360 = blue
        }
    } drops[numDrops];

    int nextDrop = 0;

  public:
    virtual void begin() override {
      Serial.println("raindrops.begin()");
    }

    virtual void update() override {
      if (random(100) < 2) {
        drops[nextDrop].start();
        nextDrop = (nextDrop + 1) % numDrops;
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
