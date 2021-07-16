class Stars : public Animation
{
    static constexpr int numStars = 5;
    static constexpr float source_radius = 10.0;
    static constexpr float accel = 0.07;
    static constexpr float radius = 12.0;
    static constexpr float rsq = radius * radius;

    class Star
    {
        float x, y;
        float vx, vy;
        float ax, ay;
        uint16_t hue;

      public:
        bool active {false};

        void start() {
          float cx = width / 2.0;
          float cy = height / 2.0;

          float r = random(source_radius);
          float a = random(2*M_PI);

          ax = accel * cos(a);
          ay = accel * sin(a);

          vx = vy = 0;

          x = cx + r * cos(a);
          y = cy + r * sin(a);
          
          hue = random(0, 360) * 65536 / 360; // 120=green, 240=blue
          active = true;
        }

        void move() {
          if (active) {
            x += vx;
            y += vy;
            vx += ax;
            vy += ay;
            if (x < -radius || y < -radius || x > width + radius || y > height + radius)
              active = false;
          }
        }

        uint32_t getColor(const Pixel &p) {
          if (!active)
            return 0;

          // calc the distance from the pixel to the ring center
          float dx = p.x - x;
          float dy = p.y - y;
          float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.

          // calc the distance from the pixel to the ring
          float b = 255.0 * (rsq - dsq) / rsq;
          if (b < 0.0)
            b = 0.0;

          return strip.ColorHSV(hue, 100, b);
        }
    } stars[numStars];

  public:
    virtual void begin() override {
      Serial.println("stars.begin()");
    }

    virtual void update() override {
      int makeNew = 1;

      for (Star &s : stars) {
        if (makeNew > 0 && !s.active) {
          s.start();
          --makeNew;
        }
        s.move();
      }
    }

    virtual uint32_t getColor(const Pixel &p) override {
      uint32_t c = 0;
      for (Star &s : stars) {
        if (s.active)
          c = blend(c, s.getColor(p));
      }
      return c;
    }

} stars;
