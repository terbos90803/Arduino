class Raindrops : public Animation
{
    int frame = 0;

  public:
    virtual void begin() {
      Serial.println("raindrops.begin()");
    }

    virtual void update() {
      ++frame;
    }

    virtual uint32_t getColor(int x, int y) {
      // calc the distance from the pixel to the ring center
      float dx = x - 50;
      float dy = y - 50;
      float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.  Also gives a nice fade effect.
      //float d = sqrt(dsq);

      // calc the size of the ring based on the frame number (gives animation)
      float rad = (frame % 75);
      float radsq = rad * rad;

      // calc the distance from the pixel to the ring
      float diff = abs(dsq - radsq); // difference in radii squared
      //float diff = abs(d - rad); // difference in radii
      float val = 255 - diff/5; // The ring is full brightness.  brightness falls off further from the ring.
      float b = val > 0 ? val : 0; // clip to avoid negative values

      return strip.ColorHSV(43690, 255, b); // 43690 = 240*65536/360 = blue
    }

} raindrops;
