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
#include "GLCDFont.h"
//#include "Tahoma19x21.h"
#include "ComicSans21x25.h"

struct Message {
  const char message[32];
  const char fgcolors[32];
  const char bgcolors[32];
} const g_messages[] = {
  {
    "4999  Momentum  4999",
    "88887799999999778888",
    "00000000000000000000"
  },
  {
    "4999  Momentum  4999",
    "88887700000000778888",
    "00000099999999000000"
  },
  {
    "Go Go  Mo Mo",
    "888888999999",
    "000000000000"
  }
};
const byte nMessages = sizeof(g_messages) / sizeof(g_messages[0]);
char g_message[32] = "Custom";
char g_msgfg[32] = "77777777777777777777";
char g_msgbg[32] = "00000000000000000000";

const int NUMPIXELS = 72; // Number of LEDs in strip

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno and 101 = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BGR);


//BLEPeripheral blePeripheral; // create peripheral instance
// Selector service
BLEService msgService("411acab3-23af-4a05-9bb7-35eaec0a7bb2"); // create service
BLEUnsignedCharCharacteristic msgSelector("411acab3-23af-4a05-9bb7-35eaec0a7bb2", BLERead | BLEWrite);
BLEDescriptor msgDesc("411acab3-23af-4a05-9bb7-35eaec0a7bb2", "Selector");
// Custom service
BLEService customService("7f4f6376-e0fa-45a3-ae89-47f162c30518"); // create service
BLECharacteristic customMsg("7f4f6376-e0fa-45a3-ae89-47f162c30518", BLERead | BLEWrite, 20);
BLECharacteristic customFG("8e818756-e307-40b9-8217-f279bea0af1e", BLERead | BLEWrite, 20);
BLECharacteristic customBG("4c56472a-cd05-4aec-b9c5-9afa90c37ec9", BLERead | BLEWrite, 20);
BLEDescriptor customDesc("7f4f6376-e0fa-45a3-ae89-47f162c30518", "Custom");

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

void renderMessage(byte msgSelection)
{
  const char *message = NULL, *fgcolors = NULL, *bgcolors = NULL;
  if (msgSelection < nMessages)
  {
    message = g_messages[msgSelection].message;
    fgcolors = g_messages[msgSelection].fgcolors;
    bgcolors = g_messages[msgSelection].bgcolors;
  }
  else if (*g_message) {
    message = g_message;
    fgcolors = g_msgfg;
    bgcolors = g_msgbg;
  }
  msgLength = strlen(message);
  //msgHeight = tahoma.getHeight();
  //msgWidth = tahoma.render(message, fgcolors, bgcolors, framebuffer);
  msgHeight = comicsans.getHeight();
  msgWidth = comicsans.render(message, fgcolors, bgcolors, framebuffer);
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

void BLEsetup()
{
  BLE.begin();
  
  // set the local name peripheral advertises.  Up to 8 chars.
  BLE.setLocalName("SkyWrite");
  BLE.setDeviceName("SkyWriter 101");
  // set the UUID for the service this peripheral advertises
  BLE.setAdvertisedService(msgService);
  BLE.setAdvertisedService(customService);

  // add service and characteristic
  msgSelector.addDescriptor(msgDesc);
  msgService.addCharacteristic(msgSelector);
  BLE.addService(msgService);
  customMsg.addDescriptor(customDesc);
  customService.addCharacteristic(customMsg);
  customService.addCharacteristic(customFG);
  customService.addCharacteristic(customBG);
  BLE.addService(customService);
  
  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  msgSelector.setEventHandler(BLEWritten, msgCharacteristicWritten);
  // set an initial value for the characteristic
  msgSelector.setValue(0);

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

  // Render bitmap
  renderMessage(0);
}

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

  // poll peripheral
  BLE.poll();
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
  renderMessage(selection);
}

void customMsgWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customMsg.valueLength();
  strncpy(g_message, (const char *)customMsg.value(), len);
  g_message[len] = 0;
  renderMessage(-1);
}

void customFGWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customFG.valueLength();
  strncpy(g_msgfg, (const char *)customFG.value(), len);
  g_msgfg[len] = 0;
  renderMessage(-1);
}

void customBGWritten(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update message
  Serial.print("Characteristic event, written: ");

  int len = customBG.valueLength();
  strncpy(g_msgbg, (const char *)customBG.value(), len);
  g_msgbg[len] = 0;
  renderMessage(-1);
}

