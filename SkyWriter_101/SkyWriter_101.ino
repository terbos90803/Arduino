/*
   SkyWriter 101

   Driver for the SkyWriter message stick
   Targets Arduino 101 with a 72 element DotStar strip

   author: Terry Wells
   date: 2018-02-25

*/

#include <Adafruit_DotStar.h>
#include <CurieIMU.h>
#include <CurieBLE.h>
#include "FrameBuffer.h"
#include "MomentumLogo.h"
#include "Displayable.h"

const int NUMPIXELS = 72; // Number of LEDs in strip

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno and 101 = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BGR);

// Message service
BLEService msgService("411acab3-23af-4a05-9bb7-35eaec0a7bb2"); // create service
BLEUnsignedCharCharacteristic msgSelector("411acab3-23af-4a05-9bb7-35eaec0a7bb2", BLERead | BLEWrite);
BLEDescriptor msgSelDesc("411acab3-23af-4a05-9bb7-35eaec0a7bb2", "Selector");
BLEUnsignedCharCharacteristic msgNumPresets("4f504ffd-2db6-4b53-96cd-804988d27cda", BLERead);
BLEDescriptor msgNumPresetsDesc("4f504ffd-2db6-4b53-96cd-804988d27cda", "Number of Presets");
// Custom message
BLECharacteristic customMsg("7f4f6376-e0fa-45a3-ae89-47f162c30518", BLERead | BLEWrite, 20);
BLEDescriptor customDesc("7f4f6376-e0fa-45a3-ae89-47f162c30518", "Custom");
BLECharacteristic customFG("8e818756-e307-40b9-8217-f279bea0af1e", BLERead | BLEWrite, 20);
BLECharacteristic customBG("4c56472a-cd05-4aec-b9c5-9afa90c37ec9", BLERead | BLEWrite, 20);

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

char g_message[32] = "Custom";
char g_msgfg[32] = "77777777777777777777";
char g_msgbg[32] = "00000000000000000000";

FrameBuffer framebuffer(strip, colormap);
TextMessage customTextMsg(framebuffer, g_message, g_msgfg, g_msgbg);
RASTER_MomentumLogo;

TextMessage preset0(framebuffer,
                    "4999  Momentum  4999",
                    "88887799999999778888",
                    "00000000000000000000");
TextMessage preset1(framebuffer,
                    "Go Go  Mo Mo",
                    "888888999999",
                    "000000000000");
Image preset2(MomentumLogoRaster);

Displayable * presets[] = { &preset0, &preset1, &preset2 };
const byte nPresets = sizeof(presets) / sizeof(presets[0]);
const Display *display = 0;
int rotatePreset = 0;

void BLEsetup()
{
  BLE.begin();

  // set the local name peripheral advertises.  Up to 8 chars.
  BLE.setLocalName("SkyWrite");
  BLE.setDeviceName("SkyWriter 101");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(msgService);

  // add service and characteristic
  msgSelector.addDescriptor(msgSelDesc);
  msgService.addCharacteristic(msgSelector);
  msgNumPresets.addDescriptor(msgNumPresetsDesc);
  msgService.addCharacteristic(msgNumPresets);
  customMsg.addDescriptor(customDesc);
  msgService.addCharacteristic(customMsg);
  msgService.addCharacteristic(customFG);
  msgService.addCharacteristic(customBG);
  BLE.addService(msgService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  msgSelector.setEventHandler(BLEWritten, msgCharacteristicWritten);
  // set an initial value for the characteristic
  msgSelector.setValue(0);
  msgNumPresets.setValue(nPresets);

  // assign event handlers for characteristic
  customMsg.setEventHandler(BLEWritten, customMsgWritten);
  customMsg.setValue(g_message); // set an initial value for the characteristic
  customFG.setEventHandler(BLEWritten, customFGWritten);
  customFG.setValue(g_msgfg); // set an initial value for the characteristic
  customBG.setEventHandler(BLEWritten, customBGWritten);
  customBG.setValue(g_msgbg); // set an initial value for the characteristic

  // advertise the service
  BLE.advertise();
  Serial.println(("Bluetooth device active, waiting for connections..."));
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

  BLEsetup();

  display = presets[0]->selectDisplay();
}

void loop() {
  static int fwdStartPixel = 0;
  static int revStartPixel = 0;
  static unsigned int fwdCount = 0;
  static unsigned int revCount = 0;

  float gx, gy, gz; //scaled Gyro values
  unsigned long startTime = micros();

  // read gyro measurements from device, scaled to the configured range
  CurieIMU.readGyroScaled(gx, gy, gz);
  const int gyro = -gz;

  int dispWidth = display->getWidth();
  int dispMidPixel = dispWidth / 2;

  if (gyro > 100)
  {
    // Swinging forward

    if (revCount > 0)
    {
      // Analyze last reverse swing
      int midPixel = revCount / 2;
      revStartPixel = midPixel - dispMidPixel;
    }
    revCount = 0;


    int pixelIndex = fwdCount - fwdStartPixel; // Calculate the pixel index into the message

    display->displayColumn(pixelIndex);
    ++fwdCount;
  }
  else if (gyro < -100)
  {
    // Swinging backwards

    if (fwdCount > 0)
    {
      // Analyze last forward swing
      int midPixel = fwdCount / 2;
      fwdStartPixel = midPixel - dispMidPixel;
    }
    fwdCount = 0;


    int pixelIndex = dispWidth - (revCount - revStartPixel); // Calculate the pixel index into the message

    display->displayColumn(pixelIndex);
    ++revCount;
  }
  else
  {
    // Not swinging (at least not very fast)
    display->displayColumn(-1);
  }

  // poll peripheral
  BLE.poll();

  unsigned long minWait = display->getColumnTime();
  while (micros() - startTime < minWait)
    ; // wait for at least a minimum usecs

  // Rotate the display presets every 10 seconds
  // This will be the default behavior on boot
  // Changing any setting via the BTLE connection will disable the rotation
  if (rotatePreset >= 0) {
    int newPreset = millis() / 10000 % nPresets;
    if (newPreset != rotatePreset) {
      rotatePreset = newPreset;
      display = presets[rotatePreset]->selectDisplay();
    }
  }
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void msgCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  byte selection = msgSelector.value();
  Serial.print("New message: ");
  Serial.println(selection);
  if (selection < nPresets)
    display = presets[selection]->selectDisplay();
  rotatePreset = -1;
}

void customMsgWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customMsg.valueLength();
  strncpy(g_message, (const char *)customMsg.value(), len);
  g_message[len] = 0;
  display = customTextMsg.selectDisplay();
  rotatePreset = -1;
}

void customFGWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customFG.valueLength();
  strncpy(g_msgfg, (const char *)customFG.value(), len);
  g_msgfg[len] = 0;
  display = customTextMsg.selectDisplay();
  rotatePreset = -1;
}

void customBGWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customBG.valueLength();
  strncpy(g_msgbg, (const char *)customBG.value(), len);
  g_msgbg[len] = 0;
  display = customTextMsg.selectDisplay();
  rotatePreset = -1;
}
