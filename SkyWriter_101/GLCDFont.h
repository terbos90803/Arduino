#if !defined(GLCD_FONT_H)
#define GLCD_FONT_H

/*
   GLCDFont class to render GLCD fonts
*/

#include "FrameBuffer.h"

class GLCDFont
{
    const uint8_t * const font PROGMEM;
    const int width;
    const int height;
    const int firstChar;
    const int lastChar;
    const int bpcol; // bytes per column
    const int bpglyph; // bytes per glyph

    const uint8_t * getGlyphBase(byte c)
    {
      if (c < firstChar || c > lastChar)
        return NULL;
      return font + (c - firstChar) * bpglyph;
    }

    void renderGlyph(int &x, char c, byte fgcolor, byte bgcolor, FrameBuffer &output)
    {
      Column kerning(height, bgcolor);

      const uint8_t *glyph(getGlyphBase(c));
      int cols(pgm_read_byte(glyph));
      for (int col = 0; col < cols; ++col)
      {
        const uint8_t *fCol(glyph + 1 + col * bpcol); // base of glyph, skip length byte, index by column
        ColumnBuilder column;
        uint8_t b = 0;
        for (int y = 0; y < height; ++y)
        {
          if (y % 8 == 0)
            b = pgm_read_byte(fCol + y / 8);
          column.addPixel(b & 1 ? fgcolor : bgcolor);
          b >>= 1;
        }
        output.setColumn(x++, column.build());
      }

      // Add 1 pixel of kerning between characters
      output.setColumn(x++, kerning);
    }

  public:
    GLCDFont(const uint8_t * const _font, int _width, int _height, int _firstChar, int _lastChar)
      : font(_font), width(_width), height(_height), firstChar(_firstChar), lastChar(_lastChar),
        bpcol((height + 7) / 8), bpglyph(1 + bpcol * width)
    {}

    /// \return height of font in pixels
    int getHeight() const {
      return height;
    }

    /// \return width of render in pixels without actually rendering anything
    int getWidth(const char *string)
    {
      int pixels = 0;
      for (; string && *string; ++string)
      {
        pixels += pgm_read_byte(getGlyphBase(*string)) + 1; // add 1 for kerning
      }
      return pixels;
    }

    /// \return width of render in pixels
    int render(const char *string, byte fgcolor, byte bgcolor, FrameBuffer &output)
    {
      int x = 0;
      for (; string && *string; ++string)
      {
        renderGlyph(x, *string, fgcolor, bgcolor, output);
      }
      return x;
    }

    /// \return width of render in pixels
    int render(const char *string, const char *fgcolors, const char *bgcolors, FrameBuffer &output)
    {
      int x = 0;
      for (; string && *string; ++string, ++fgcolors, ++bgcolors)
      {
        byte fgcolor = *fgcolors - '0';
        byte bgcolor = *bgcolors - '0';
        renderGlyph(x, *string, fgcolor, bgcolor, output);
      }
      return x;
    }

};
#endif
