/*
  Fan Temp/Speed controller
  Reads an analog input on pin 3, converts it to temperature.  Assumes an LM34 device is attached.
  
 */

int buzzerPin = 2;
int fanPowerPin = 9;
int tempSensorPin = A3;
float minTemp = 70.0;
float maxTemp = 90.0;

int pwmTop = 500; // Choose PWM period to be above audible frequency
int pwmStep = pwmTop / 10;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  // TCCR1B
  // Bit                   7      6      5      4      3      2      1       0
  // Bit Name            COM1A1 COM1A0 COM1B1 COM1B0 -----  -----  WGM11   WGM10
  // Initial Value         0      0      0      0      0      0      0       0
  // changed to            1      0      1      0      0      0      1       0
  TCCR1A = B10100010;

  // TCCR1B
  // Bit                7      6      5        4        3        2        1        0
  // Bit Name         ICNC1  ICES1  -----    WGM13    WGM12     CS12     CS11     CS10
  // Initial Value      0      0      0        0        0        0        0        0
  // changed to         0      0      0        1        0        0        0        1  
  // CS12,CS11,CS10 = (0,0,1) System clock, no division
  TCCR1B = B00010001;
  
  ICR1 = pwmTop;
}

void setFanSpeed(int s)
{
  analogWrite(fanPowerPin, s);
  Serial.println(s);
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop()
{
  for (int i = 0; i < pwmTop; i=i+pwmStep)
    setFanSpeed(i);
  for (int i = pwmTop; i > 0; i=i-pwmStep)
    setFanSpeed(i);
}
