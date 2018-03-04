/*
  Fan Temp/Speed controller
  Reads an analog input on pin 3, converts it to temperature.  Assumes an LM34 device is attached.
  
  Closet fan board is an Arduino Pro Mini (5V, 16MHz) w/ ATmega328
 */

int buzzerPin = 2;
int tachPin = 3;
int fanPowerPin = 9;
int tempSensorPin = A3;
float minTemp = 70.0;
float maxTemp = 90.0;
float alarmTemp = 100.0;
int lowFanThreshold = 10;

int flash = LOW;
volatile int tachCount = 0;

// interrupt handler for the tach
void tachPulse()
{
  ++tachCount;
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // init and disable buzzer  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  
  // attach interrupt handler to fan pulse
  pinMode(tachPin, INPUT_PULLUP);
  attachInterrupt(1, tachPulse, RISING);
}

// the loop routine runs over and over again forever:
void loop()
{
  // read the input on the temp sensor:
  int sensorValue = analogRead(tempSensorPin);
  // Convert the analog reading (which goes from 0 - 1023) to a temperature (0 - 500F):
  float temp = map(sensorValue, 0, 1023, 0, 500);  
//  float temp = sensorValue * (500.0 / 1023.0);
  // print out the temp
  Serial.println(temp);
  
  // get the current fan speed (tach count since last loop)
  int fanSpeed = tachCount;
  tachCount = 0;
  Serial.println(fanSpeed);

  // collect any alarms
  int alarm = LOW;
  
  // If the temp exceeds 100F, flash the alarm
  if(temp > alarmTemp)
    alarm = flash;
  
  // If fan speed drops below threshold, solid alarm
  if(fanSpeed < lowFanThreshold)
    alarm = HIGH;
    
  // Set final composite alarm signal  
  digitalWrite(buzzerPin, alarm);
  
  flash = !flash;
  delay(1000);
}
