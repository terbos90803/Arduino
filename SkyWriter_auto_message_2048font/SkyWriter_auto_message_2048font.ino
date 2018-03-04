/*
 SkyWriter
 
 Driver code for the SkyWriter light message stick.

 Target device is Nano
 
 */
#include <avr/pgmspace.h>
#include "skyfont.h"

const byte message[] = {
  "4999  Momentum  4999"
};

const int msgLength = sizeof(message) / sizeof(byte) - 1;
const int msgPixels = msgLength * 8;
const int msgMidPixel = msgPixels / 2;
byte pixels[msgPixels];

const int BLACK = 0;
const int GREEN = 10;
const int RED = 11;
const int BLUE = 12;

void selectColor(int color)
{
  int i;
  for (i = GREEN; i <= BLUE; ++i)
  {
    digitalWrite(i, LOW);
  }
  if (color > BLACK)
    digitalWrite(color, HIGH);
}

void setData(byte data)
{
  int i;
  data = ~data; // invert data to match output logic.

  for (i = 0; i < 8; ++i)
  {
    // data rows start at pin 2
    digitalWrite(2 + i, data & 1);
    data >>= 1;
  }
}

int gyroCenter = 512;

void setup()  {
  // Enable all color drive pins and set the initial color to black (all off)
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  selectColor(BLACK);

  // enable all row drive pins and set the initial data to 0 (all off)
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  setData(0);

  // enable pin 13 (yellow light) for user feedback
  pinMode(13, OUTPUT);

  // wait for gyro settle time and capture center point
  // digitalWrite(13, HIGH);
  // delay(1000);
  // digitalWrite(13, LOW);
  // gyroCenter = analogRead(A0); // range 0-1023, center ~512
  gyroCenter = 512;

  // Render bitmap
  for (int i = 0; i < msgPixels; ++i)
  {
      int charIndex = message[i / 8] * 8;
      int sliceIndex = i % 8;
      pixels[i] = pgm_read_byte_near(font + charIndex + sliceIndex);
  }
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


void loop()  {
  static int fwdStartPixel = 0;
  static int revStartPixel = 0;
  static unsigned int fwdCount = 0;
  static unsigned int revCount = 0;

  int gyro = gyroCenter - analogRead(A0); // gyro range roughly: [-512, 512], zero is not rotating

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

    if (pixelIndex >= 0 && pixelIndex < msgPixels)
    {
      chooseColor();
      setData(pixels[pixelIndex]);
    }
    else
    {
      selectColor(BLACK);
      setData(0);
    }
    digitalWrite(13, HIGH); // Turn on yellow light
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


    int pixelIndex = msgPixels - (revCount - revStartPixel); // Calculate the pixel index into the message

    if (pixelIndex >= 0 && pixelIndex < msgPixels)
    {
      chooseColor();
      setData(pixels[pixelIndex]);
    }
    else
    {
      selectColor(BLACK);
      setData(0);
    }
    ++revCount;

  }
  else
  {
    // Not swinging (at least not very fast)

    selectColor(BLACK);
    setData(0);
    digitalWrite(13, LOW); // Turn off yellow light
  }

  delayMicroseconds(1500);
}

