/*
 SkyWriter
 
 Driver code for the SkyWriter light message stick.
 
 */

const int BLACK = 0;
const int GREEN = 1;
const int RED = 2;
const int BLUE = 3;

void selectColor(int color)
{
  int i;
  for (i = 0; i < 4; ++i)
  {
    digitalWrite(10 + i, LOW);
  }
  if (color > 0)
    digitalWrite(9 + color, HIGH);
}

void setData(byte data)
{
  int i;
  data = ~data; // invert data to match output logic.
  
  for (i = 0; i < 8; ++i)
  {
    digitalWrite(2 + i, data & 1);
    data = data >> 1;
  }
}

void setup()  {
  // Enable all color drive pins and set the initial color to black (all off)
  pinMode(10, OUTPUT); // green
  pinMode(11, OUTPUT); // red
  pinMode(12, OUTPUT); // blue
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
} 


const byte image[] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B10111010,
  B11111110,
  B11100000,
  B11101000,
  B11111000,
  B11111110,
  B11101010,
  B01101110,
  B00100000,
  B00010000,
  B00001000,
  B00000000,
  B00000000
};

const int length = sizeof(image) / sizeof(byte);
  
void loop()  {
  static unsigned int count = 0;
  
  int gyro = analogRead(A0); // range 0-1023, center ~512
  
  selectColor((count / length) % 4);
  setData(image[count % length]);
  digitalWrite(13, count & 1);

  ++count;
  delay(1);
}
