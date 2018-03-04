/*
 GyroFade
 
 Use the gyro input to alter the brightness of an LED.
 
 This example code is in the public domain.
 
 */
int brightness = 0;    // how bright the LED is

void setup()  { 
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
} 

void loop()  {
  int gyro = analogRead(A6); // range 0-1023, center ~512
  
  if (gyro > 700)
  {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(5, LOW);
  }
  else if (gyro < 300)
  {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(5, LOW);
  }
  else
  {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(5, HIGH);
  }
}
