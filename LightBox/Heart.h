// Heart animation
#include "Raster.h"

class Heart : public Raster
{
    const float step = M_PI/360;
    const uint32_t color = 0xff9afe;
    int frame = 0;

  public:
    virtual void begin() override {
      Serial.println("heart.begin()");

      for (float t = -M_PI; t <= M_PI; t += step) {
        float ct = cosf(t);
        float st = sinf(t);
        
        float x1 = ct;
        float y1 = -st;
        float r = 20 * (st*sqrtf(abs(ct)) / (st + 7.0f/5.0f) - 2.0f*st + 2.0f);
        for (float ri = 0; ri <= r; ri += 1.0) {
          float x = x1 * ri + 50;
          float y = y1 * ri + 20;
          getPixelSafe(x, y) = color;
        }
      }
    }

    virtual void update() override {
      frame = (frame + 1) % 100;
      if (frame == 0)
        m.reset();
      m.scale(1.01);
    }

} heart;
