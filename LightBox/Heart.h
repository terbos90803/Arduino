// Heart animation
#include "Raster.h"

class Heart : public Raster
{
    const float step = M_PI/180;
    const uint32_t color = 0xff9afe;
    int frame = 0;

  public:
    virtual void begin() override {
      Serial.println("heart.begin()");

      for (float t = -M_PI; t <= M_PI; t += step) {
        float x1 = cos(t);
        float y1 = sin(t);
        float r = 30 * (sin(t)*sqrt(abs(cos(t))) / (sin(t) + 7/5) - 2*sin(t) + 2);
        for (float ri = 0; ri <= r; ri += 1.0) {
          float x = x1 * ri + 50;
          float y = y1 * ri + 50;
          getPixelSafe(x, y) = color;
        }
      }
    }

    virtual void update() override {
    }

} heart;
