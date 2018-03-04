/*
 GyroFade
 
 Use the gyro input to alter the brightness of an LED.
 
 This example code is in the public domain.
 
 */
int brightness = 0;    // how bright the LED is

void setup()  { 
  // declare pin 3 to be an output:
  pinMode(3, OUTPUT);
} 

void loop()  {
  brightness = analogRead(A6) / 4;
  
  // set the brightness of pin 3:
  analogWrite(3, brightness);    
}
