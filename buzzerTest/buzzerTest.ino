/*
  Fan Temp/Speed controller
  Reads an analog input on pin 3, converts it to temperature.  Assumes an LM34 device is attached.
  
 */

int buzzerPin = 2;
int fanPowerPin = 9;
int tempSensorPin = A3;
float minTemp = 70.0;
float maxTemp = 90.0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

// the loop routine runs over and over again forever:
void loop()
{
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
  delay(2000);
}
