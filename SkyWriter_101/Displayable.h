#if !defined(DISPLAYABLE_H_)
#define DISPLAYABLE_H_

#include "Display.h"
#include "FrameBuffer.h"
#include "Raster.h"
#include "GLCDFont.h"
//#include "Tahoma19x21.h"
#include "ComicSans21x25.h"

class Displayable
{
  public:
    Displayable()
    {}

    virtual const Display * selectDisplay() const = 0;
};

class TextMessage : public Displayable
{
    FrameBuffer & fb;
    const char * const msg;
    const char * const fg;
    const char * const bg;

  public:
    TextMessage(FrameBuffer & _fb, const char * _msg, const char * _fg, const char * _bg)
      : fb(_fb), msg(_msg), fg(_fg), bg(_bg)
    {}

    virtual const Display * selectDisplay() const
    {
      int msgLength = strlen(msg);
      //int msgHeight = tahoma.getHeight();
      //int msgWidth = tahoma.render(msg, fg, bg, fb);
      int msgHeight = comicsans.getHeight();
      int msgWidth = comicsans.render(msg, fg, bg, fb);
      if (msgWidth > fb.getMaxWidth()) {
        Serial.println("ERROR: Message is larger than framebuffer");
      }
      Serial.print("Message chars:");
      Serial.print(msgLength);
      Serial.print("  msgHeight:");
      Serial.print(msgHeight);
      Serial.print("  msgWidth:");
      Serial.println(msgWidth);

      return &fb;
    }
};


class Image : public Displayable
{
    const Raster & raster;

  public:
    Image(Raster & _raster) : raster(_raster)
    {}

    virtual const Display * selectDisplay() const
    {
      return &raster;
    }
};

#endif // DISPLAYABLE_H_
