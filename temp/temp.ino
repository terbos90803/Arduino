// SD card attached to SPI bus as follows:
// ** MOSI - pin 11
// ** MISO - pin 12
// ** CLK - pin 13
// ** CS - pin 8
//RTC Clock
// SDA -Pin A4
// SCL -Pin A5

//LCD16*2-Arduino Nano
//     RS-Pin7
//     E -Pin6
//    R/W-GND
//    DB7-Pin2
//    DB6-pin3
//    DB5-pin4
//    DB4-pin5

#include <LiquidCrystal.h>
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 rtc;
#include <SD.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);  

const int chipSelect = 8;
int Temp = 0;   // temprature
void setup()   {
lcd.begin(16, 2);

  //Serial.begin(9600);
  //Serial.print("Initializing SD card...");

lcd.print("Initializing SD card..."); 
delay(1000);
lcd.clear();
 pinMode(10, OUTPUT); // required for SD lib
 digitalWrite(10,HIGH);
// see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
  
    //  Serial.println("Card failed, or not present");
   
    lcd.print("Card Failed or");
    lcd.setCursor(0, 1);
    lcd.print("Not present");
    // don't do anything more:
    delay(3000);
    return;
  }
  
  //Serial.println("card initialized.");
  
  lcd.print("Card Initialized.");
delay(2000);
lcd.clear();

 Wire.begin();
 rtc.begin();
 if (! rtc.isrunning()) {
    lcd.clear();
    lcd.print("RTC NOT Running");
    //Serial.println("RTC is NOT running!");
// following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(__DATE__, __TIME__));  //useed when only to set date and time in firt setting of RTC
  }
  
  // rtc.adjust(DateTime(__DATE__, __TIME__)); use it when you need to correct the time  
 delay(2000);
}
void loop() {
  lcd.setCursor(13, 0);
  Temp = (5.0 * analogRead(A0) * 100.0) / 1024;
DateTime now = rtc.now();
    // print to the serial port too:
    //Serial.print(Temp);
    //Serial.print(",");
    //Serial.print(now.year(), DEC);
    //Serial.print('/');
    //Serial.print(now.month(), DEC);
    //Serial.print('/');
    //Serial.print(now.day(), DEC);
    //Serial.print(' ');
    //Serial.print(now.hour(), DEC);
    //Serial.print(':');
    //Serial.print(now.minute(), DEC);
    //Serial.print(':');
    //Serial.print(now.second(), DEC);
    //Serial.println();


lcd.setCursor(12, 0);
   lcd.print(Temp);
   lcd.print((char)223);     // degrees character
   lcd.print("C");
lcd.setCursor(1, 0);
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);
lcd.setCursor(3, 1);
  if (now.hour() < 10) lcd.print("0");    //  add zero
  lcd.print(now.hour(), DEC);             // Print hour
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");  // add  zero
  lcd.print(now.minute(), DEC);           // Print  minute
//  lcd.print(":");
//  if (now.second() < 10) lcd.print("0");  // add zero
//  lcd.print(now.second(), DEC);           // Print second


int s=now.second();
if(s==0){    //get the log toSD every 60 sec ( 1Min )
LogToSD();  //Log to SD using commands under void LogToSD()
}  
delay(1000); 
}



void LogToSD(){
DateTime now = rtc.now();
// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
   File dataFile = SD.open("datalog.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(Temp);
    dataFile.print(",");
    dataFile.print(now.year(), DEC);
    dataFile.print('/');
    dataFile.print(now.month(), DEC);
    dataFile.print('/');
    dataFile.print(now.day(), DEC);
    dataFile.print(' ');
    dataFile.print(",");
    dataFile.print(now.hour(), DEC);
    dataFile.print(':');
    dataFile.print(now.minute(), DEC);
    dataFile.print(':');
    dataFile.print(now.second(), DEC);
    dataFile.println();
    dataFile.close();
    } 
  

  
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening datalog.txt");
    lcd.clear();
    lcd.print("File Error");
    delay(2000);
      }
 }
