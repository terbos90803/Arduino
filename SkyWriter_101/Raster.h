#if !defined(RASTER_H)
#define RASTER_H

#include "Display.h"

// Raster is encoded.
// Run Length Encoding with a 2-byte entry


class Raster : public Display
{
    Adafruit_DotStar & strip;
    const int width;
    const int height;
    const uint8_t *image;
    const uint16_t *colStart;
    const uint32_t *colormap;
    const int lastPixel;

  public:
    Raster(Adafruit_DotStar & _strip, int _width, int _height, const uint8_t *_image, const uint16_t *_colStart, const uint32_t *_colormap)
      : strip(_strip), width(_width), height(_height), image(_image), colStart(_colStart), colormap(_colormap), lastPixel(strip.numPixels() - 1)
    {}

    int getWidth() const
    {
      return width;
    }

    virtual unsigned long getColumnTime() const
    {
      return 1800;
    }

    void displayColumn(int nCol) const
    {
      if (nCol >= 0 && nCol < width)
      {
        int ix = pgm_read_word(colStart + nCol);
        int nRuns = pgm_read_byte(image + ix++);
        uint16_t pix = lastPixel;
        for (int r = 0; r < nRuns; ++r)
        {
          int runlength = pgm_read_byte(image + ix++);
          uint32_t c = pgm_read_dword(colormap + pgm_read_byte(image + ix++));
          for (int p = 0; p < runlength; ++p, --pix) {
            strip.setPixelColor(pix, c);
          }
        }
      }
      else
      {
        strip.clear();
      }
      strip.show();                     // Refresh strip
    }
};

#endif
