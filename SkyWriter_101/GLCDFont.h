/*
   GLCDFont class to render GLCD fonts
*/

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
      for (int i = 0; string && *string; ++i, ++string)
      {
        pixels += pgm_read_byte_near(getGlyphBase(*string));
      }
      return pixels;
    }

    /// \return width of render in pixels
    int render(const char *string, uint32_t color, uint32_t *output)
    {
      int x(0);
      for (int i(0); output && string && *string; ++i, ++string)
      {
        const uint8_t *glyph(getGlyphBase(*string));
        int cols(pgm_read_byte_near(glyph));
        for (int col(0); col < cols; ++col, ++x)
        {
          const uint8_t *fCol(glyph + 1 + col*bpcol);
          uint32_t *oCol(output + x*height);
          uint8_t b(0);
          for (int y(0); y < height; ++y)
          {
            if (y%8 == 0)
              b = pgm_read_byte_near(fCol + y/8);
            oCol[y] = b&1 ? color : 0;
            b >>= 1;
          }
        }
      }
      return x;
    }
};

