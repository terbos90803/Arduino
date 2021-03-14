// Implement an animation with a raster-based source
#pragma once

#include "Matrix.h"

class Raster : public Animation
{
    static constexpr int width1 = width - 1;
    static constexpr int height1 = height - 1;
    
  protected:
    uint32_t raster[width][height] = {0};
    Matrix m;

    uint32_t & getPixelSafe(float xf, float yf)
    {
      int x, y;

      if (xf < 0.0f)
        x = 0;
      else if (xf > width1)
        x = width1;
      else
        x = round(xf);

      if (yf < 0.0f)
        y = 0;
      else if (yf > height1)
        y = height1;
      else
        y = round(yf);

      return raster[x][y];
    }

  private:
    // Display the transformed raster
    virtual uint32_t getColor(const Pixel &p) override {
      Pixel pt = m.transform(p);
      return bilerpColor(pt.x, pt.y);
    }

    // interpolate a single byte in the color
    uint32_t interpolate(uint32_t c1, uint32_t c2, float b, int shift)
    {
      float v1 = (c1 >> shift) & 0xff;
      float v2 = (c2 >> shift) & 0xff;
      float v = v1 * (1.0 - b) + v2 * (b);
      return (static_cast<uint32_t>(v) & 0xff) << shift;
    }

    // interpolate an RGB color
    // b must be in the range [0.0, 1.0]
    uint32_t lerpColor(uint32_t c1, uint32_t c2, float b)
    {
      return interpolate(c1, c2, b, 16) | interpolate(c1, c2, b, 8) | interpolate(c1, c2, b, 0);
    }

    // interpolate an RGB color in 2D space
    uint32_t bilerpColor(float x, float y)
    {
      if (x == floor(x) && y == floor(y)) {
        return getPixelSafe(x, y);
      } else {
        uint32_t ul = getPixelSafe(floor(x), floor(y));
        uint32_t ur = getPixelSafe(ceil(x), floor(y));
        uint32_t ll = getPixelSafe(floor(x), ceil(y));
        uint32_t lr = getPixelSafe(ceil(x), ceil(y));

        uint32_t high = lerpColor(ul, ur, x - floor(x));
        uint32_t low = lerpColor(ll, lr, x - floor(x));
        return lerpColor(high, low, y - floor(y));
      }
    }


};
