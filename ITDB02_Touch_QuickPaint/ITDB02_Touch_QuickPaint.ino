// UTouch_QuickPaint (C)2013-2014 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a quick demo of how to use the library.
//
// This program requires the UTFT library and a display
// module with at least 320x240 pixels resolution.
//
// It is assumed that the display module is connected to an
// appropriate shield or that you know how to change the pin
// numbers in the setup.
//

#include <UTFT.h>
#include <ITDB02_Touch.h>

// Initialize display
// ------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : <display model>,19,18,17,16
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT    myGLCD(ITDB32S, 38, 39, 40, 41);

// Initialize touchscreen
// ----------------------
// Set the pins to the correct ones for your development board
// -----------------------------------------------------------
// Standard Arduino Uno/2009 Shield            : 15,10,14, 9, 8
// Standard Arduino Mega/Due shield            :  6, 5, 4, 3, 2
// CTE TFT LCD/SD Shield for Arduino Due       :  6, 5, 4, 3, 2
// Teensy 3.x TFT Test Board                   : 26,31,27,28,29
// ElecHouse TFT LCD/SD Shield for Arduino Due : 25,26,27,29,30
//
ITDB02_Touch  myTouch( 6, 5, 4, 3, 2);

// Declare which fonts we will be using
extern uint8_t BigFont[];

int color = 0;
word colorlist[] = {VGA_WHITE, VGA_BLACK, VGA_RED, VGA_BLUE, VGA_GREEN, VGA_FUCHSIA, VGA_YELLOW, VGA_AQUA};
int  bsize = 4;

void drawColorMarkerAndBrushSize(int col)
{
  myGLCD.setColor(VGA_BLACK);
  myGLCD.fillRect(25, 0, 31, 239);
  myGLCD.fillRect(myGLCD.getDisplayXSize() - 31, 161, myGLCD.getDisplayXSize() - 1, 191);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawPixel(25, (col * 30) + 15);
  for (int i = 1; i < 7; i++)
    myGLCD.drawLine(25 + i, ((col * 30) + 15) - i, 25 + i, ((col * 30) + 15) + i);

  if (color == 1)
    myGLCD.setColor(VGA_WHITE);
  else
    myGLCD.setColor(colorlist[col]);
  if (bsize == 1)
    myGLCD.drawPixel(myGLCD.getDisplayXSize() - 15, 177);
  else
    myGLCD.fillCircle(myGLCD.getDisplayXSize() - 15, 177, bsize);

  myGLCD.setColor(colorlist[col]);
}

void setup()
{
  Serial.begin(9600);
  
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);

  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);

  myGLCD.setColor(VGA_WHITE);
  myGLCD.drawLine(32, 0, 32, myGLCD.getDisplayYSize() - 1);
  myGLCD.drawLine(myGLCD.getDisplayXSize() - 32, 0, myGLCD.getDisplayXSize() - 32, myGLCD.getDisplayYSize() - 1);
  myGLCD.print("C", myGLCD.getDisplayXSize() - 24, 8);
  myGLCD.print("L", myGLCD.getDisplayXSize() - 24, 24);
  myGLCD.print("E", myGLCD.getDisplayXSize() - 24, 40);
  myGLCD.print("A", myGLCD.getDisplayXSize() - 24, 56);
  myGLCD.print("R", myGLCD.getDisplayXSize() - 24, 72);
  myGLCD.print("+", myGLCD.getDisplayXSize() - 24, 136);
  myGLCD.print("-", myGLCD.getDisplayXSize() - 24, 200);
  myGLCD.fillRect(myGLCD.getDisplayXSize() - 32, 96, myGLCD.getDisplayXSize() - 1, 128);
  myGLCD.drawLine(myGLCD.getDisplayXSize() - 32, 160, myGLCD.getDisplayXSize() - 1, 160);
  myGLCD.drawLine(myGLCD.getDisplayXSize() - 32, 192, myGLCD.getDisplayXSize() - 1, 192);
  myGLCD.drawLine(myGLCD.getDisplayXSize() - 32, 224, myGLCD.getDisplayXSize() - 1, 224);
  for (int i = 0; i < 8; i++)
  {
    myGLCD.setColor(colorlist[i]);
    myGLCD.fillRect(0, (i * 30), 24, (((i + 1) * 30) - 1));
  }
  drawColorMarkerAndBrushSize(color);
}

long getMedian(long a, long b, long c)
{
  if (a < b)
  {
    if (c < a)
      return a;
    if (c < b)
      return c;
    return b;
  }
  else
  {
    if (c < b)
      return b;
    if (c < a)
      return c;
    return a;
  }
}

// Median filter picks the middle of the last three samples.
// This is very useful for cleaning up noisy inputs.
long filterTouch(long *arr, long s)
{
  arr[0] = arr[1];
  arr[1] = arr[2];
  arr[2] = s;
  return getMedian(arr[0], arr[1], arr[2]);
}

enum Touch { t_none, t_paint, t_color, t_clear, t_plus, t_mid, t_minus };

void loop()
{
  long x, y;
  long xs[3] = {0, 0, 0};
  long ys[3] = {0, 0, 0};
  Touch lastTouch = t_none;

  while (myTouch.dataAvailable() == true)
  {
    myTouch.read();
    x = filterTouch(xs, myTouch.getX());
    y = filterTouch(ys, myTouch.getY());
    if ((x > 0) and (y > 0))
    {
      // Normal case: paint area
      if (x > (31 + bsize) and (x < myGLCD.getDisplayXSize() - (31 + bsize)))
      {
        lastTouch = t_paint;
        if (bsize == 1)
          myGLCD.drawPixel(x, y);
        else
          myGLCD.fillCircle(x, y, bsize);
      }
      else
      {
        // Change color
        if (x < (30 + bsize))
        {
          lastTouch = t_color;
          if (y < 240)
          {
            color = y / 30;
            drawColorMarkerAndBrushSize(color);
          }
        }
        else
        {
          // CLEAR
          if (y < 96)
          {
            lastTouch = t_clear;
            myGLCD.setColor(VGA_BLACK);
            myGLCD.fillRect(33, 0, myGLCD.getDisplayXSize() - 33, myGLCD.getDisplayYSize() - 1);
            myGLCD.setColor(colorlist[color]);
          }
          // "+" size
          if ((y > 128) && (y < 160) && lastTouch != t_plus)
          {
            lastTouch = t_plus;
            if (bsize < 7)
            {
              bsize++;
              drawColorMarkerAndBrushSize(color);
            }
          }
          // mid size
          if ((y > 160) && (y < 192))
          {
            lastTouch = t_mid;
            bsize = 4;
            drawColorMarkerAndBrushSize(color);
          }
          // "-" size
          if ((y > 192) && (y < 224) && lastTouch != t_minus)
          {
            lastTouch = t_minus;
            if (bsize > 1)
            {
              bsize--;
              drawColorMarkerAndBrushSize(color);
            }
          }
        }
      }
    }
    else
      lastTouch = t_none;
  }
}

