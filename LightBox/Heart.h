// Heart animation
#include "Raster.h"

class Heart : public Raster
{
    const float step = M_PI/360;
    const uint32_t color = 0xff9afe;
    Matrix r1, r2;
    float scale=1.0, grow_rate=0.1, grow = grow_rate;
    float rot=0.0, spin=0.0; //0.1;

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
          float y = y1 * ri + 17;
          getPixelSafe(x, y) = color;
        }
      }

      r1.translate(-50,-50); // Translate the raster to be centered on the origin
      r2.translate(50,50);   // Translate the scaled raster back to the display center
    }

    virtual void update() override {
      if (scale < 1.0)
        grow = grow_rate;
      else if (scale > 4.0)
        grow = -grow_rate;
      scale += grow;

      rot += spin;
      
      m = r1;
      m.scale(scale);
      m.rotate(rot);
      m.rmult(r2);
    }

} heart;
