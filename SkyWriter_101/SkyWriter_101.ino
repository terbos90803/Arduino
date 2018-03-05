/*
   SkyWriter 101

   Driver for the SkyWriter message stick
   Targets Arduino 101 with a 72 element DotStar strip

   author: Terry Wells
   date: 2018-02-25

*/

#include <Adafruit_DotStar.h>
#include <SPI.h>
#include "CurieIMU.h"
#include "FrameBuffer.h"
#include "GLCDFont.h"
#include "Tahoma19x21.h"

const char defaultmessage[] = {
  "4999  Momentum  4999"
};
const char fgcolors[] = {
  "88887799999999778888"
};
const char bgcolors[] = {
  "00000000000000000000"
};

const int NUMPIXELS = 72; // Number of LEDs in strip

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno and 101 = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BGR);

enum COLORS {
  BLACK = 0,
  RED   = 1,
  GREEN = 2,
  BLUE  = 3,
  CYAN  = 4,
  MAGENTA = 5,
  YELLOW = 6,
  WHITE = 7,
  MO_BLUE = 8,
  MO_PURPLE = 9
};

const uint32_t colormap[] = {
  0x000000, //0 - BLACK
  0xFF0000, //1 - RED
  0x00FF00, //2 - GREEN
  0x0000FF, //3 - BLUE
  0x00FFFF, //4 - CYAN
  0xFF00FF, //5 - MAGENTA
  0xFFFF00, //6 - YELLOW
  0xFFFFFF, //7 - WHITE
  0x06CEFF, //8 - MOMENTUM BLUE
  0x9F01FF, //9 - MOMENTUM PURPLE
};

int msgLength = 0;
int msgWidth = 0;
int msgHeight = 0;
int msgMidPixel = 0;

FrameBuffer framebuffer(strip, colormap);

void renderMessage(const char *message, byte fgcolor, byte bgcolor)
{
  msgLength = strlen(message);
  msgHeight = tahoma.getHeight();
  msgWidth = tahoma.render(message, fgcolors, bgcolors, framebuffer);
  if (msgWidth > framebuffer.getWidth()) {
    Serial.println("ERROR: Message is larger than framebuffer");
  }
  Serial.print("Message chars:");
  Serial.print(msgLength);
  Serial.print("  msgHeight:");
  Serial.print(msgHeight);
  Serial.print("  msgWidth:");
  Serial.println(msgWidth);
  msgMidPixel = msgWidth / 2;
}

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP

  Serial.begin(9600); // initialize Serial communication
  //  while (!Serial);    // wait for the serial port to open.  Prevents sketch from running if not connected to serial monitor.

  // initialize device
  Serial.println("Initializing IMU device...");
  CurieIMU.begin();
  // Set the gyro range in degrees/sec.
  // Valid values are: 125, 250, 500, 1000, 2000
  CurieIMU.setGyroRange(1000);

  // Render bitmap
  renderMessage(defaultmessage, RED, BLACK);
}

#if 0
void selectColor(int c)
{
  color = c;
}

void chooseColor()
{
  unsigned long time = millis() / 4000;
  switch (time % 3)
  {
    case 0:
      selectColor(RED);
      break;

    case 1:
      selectColor(GREEN);
      break;

    case 2:
      selectColor(BLUE);
      break;
  }
}
#endif

void loop() {
  static int fwdStartPixel = 0;
  static int revStartPixel = 0;
  static unsigned int fwdCount = 0;
  static unsigned int revCount = 0;

  float gx, gy, gz; //scaled Gyro values

  // read gyro measurements from device, scaled to the configured range
  CurieIMU.readGyroScaled(gx, gy, gz);
  const int gyro = -gz;

  if (gyro > 100)
  {
    // Swinging forward

    if (revCount > 0)
    {
      // Analyze last reverse swing
      int midPixel = revCount / 2;
      revStartPixel = midPixel - msgMidPixel;
    }
    revCount = 0;


    int pixelIndex = fwdCount - fwdStartPixel; // Calculate the pixel index into the message

    if (pixelIndex >= 0 && pixelIndex < msgWidth)
    {
      framebuffer.displayColumn(pixelIndex);
    }
    else
    {
      framebuffer.displayColumn(-1);
    }
    ++fwdCount;
  }
  else if (gyro < -100)
  {
    // Swinging backwards

    if (fwdCount > 0)
    {
      // Analyze last forward swing
      int midPixel = fwdCount / 2;
      fwdStartPixel = midPixel - msgMidPixel;
    }
    fwdCount = 0;


    int pixelIndex = msgWidth - (revCount - revStartPixel); // Calculate the pixel index into the message

    if (pixelIndex >= 0 && pixelIndex < msgWidth)
    {
      framebuffer.displayColumn(pixelIndex);
    }
    else
    {
      framebuffer.displayColumn(-1);
    }
    ++revCount;

  }
  else
  {
    // Not swinging (at least not very fast)

    framebuffer.displayColumn(-1);
  }
}
