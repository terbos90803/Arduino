#if !defined(FRAMEBUFFER_H)
#define FRAMEBUFFER_H

#include "Display.h"

// Framebuffer is encoded.
// Run Length Encoding with a 2-byte entry


struct Run {
  byte length;
  byte color;

  Run() : length(0), color(0)
  {}
  Run(byte _length, byte _color) : length(_length), color(_color)
  {}
};

struct Column {
  static const byte RUNS_PER_COL = 10; // max number of runs per column

  byte nRuns;
  Run runs[RUNS_PER_COL];

  // Empty Column
  Column() : nRuns(0)
  {}

  //Solid color Column
  Column(int height, byte color)
    : nRuns(1)
  {
    runs[0] = Run(height, color);
  }
};

class ColumnBuilder {
    Column col;
    Run run;

    void pushRun()
    {
      if (run.length > 0 && col.nRuns < Column::RUNS_PER_COL)
      {
        col.runs[col.nRuns++] = run;
      }
      run.length = 0;
    }

  public:
    void addPixel(byte color)
    {
      if (color != run.color)
      {
        pushRun();
        run.color = color;
      }
      ++run.length;
    }

    Column & build()
    {
      pushRun();
      return col;
    }
};

class FrameBuffer : public Display
{
    static const int FB_WIDTH = 300; // max number of columns

    Column columns[FB_WIDTH];
    Adafruit_DotStar & strip;
    const uint32_t *colormap;
    const int lastPixel;
    int width;

  public:
    FrameBuffer(Adafruit_DotStar & _strip, const uint32_t *_colormap)
      : strip(_strip), colormap(_colormap), lastPixel(strip.numPixels() - 1)
    {}

    int getMaxWidth() const
    {
      return FB_WIDTH;
    }

    void setWidth(int newWidth)
    {
      width = newWidth;
    }
    
    virtual int getWidth() const
    {
      return width;
    }

    void setColumn(int nCol, const Column &col)
    {
      if (nCol >= 0 && nCol < FB_WIDTH)
        columns[nCol] = col;
    }

    void displayColumn(int nCol) const
    {
      if (nCol >= 0 && nCol < FB_WIDTH)
      {
        const Column &col = columns[nCol];
        uint16_t pix = lastPixel;
        for (int r = 0; r < col.nRuns; ++r)
        {
          const Run &run = col.runs[r];
          int runlength = run.length;
          uint32_t c = colormap[run.color];
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
