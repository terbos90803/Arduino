#include <cmath>

class LavaLamp : public Animation
{
    class Blob {
        constexpr static float tempTop = 0;
        constexpr static float tempBottom = 100;
        constexpr static float kBuoy = 0.005;
        constexpr static float kA = 0.01;

        const float blobRadius;
        const float blobRadius2;
        const float kT; // = 0.05;
        const uint16_t bkgColor = 0;

        uint16_t blobHue;
        float blobx;
        float blobxv;
        float bloby = height;
        float blobyv = 0;
        float blobTemp = 50;

      public:
        Blob(uint16_t hue, int x, int r) : 
          blobHue(hue), 
          blobx(x),
          blobxv((float)x / (20 * width)),
          blobRadius(r), 
          blobRadius2(r * r), kT(5.0 / blobRadius2)
        {}

        void move()
        {
          // float blob
          float ytemp = bloby; // Need to interpolate for anything other than height=100
          float tempDiff = ytemp - blobTemp; // positive=blob is colder than surround
          float buoyancy = kBuoy * (tempDiff + 50 - bloby); // negative=accelerate up
          float blobya = kA * buoyancy / (1 + blobyv * blobyv);
          blobTemp += kT * tempDiff;
          blobyv += blobya;
          bloby += blobyv;
          blobx += blobxv * abs(blobyv);

          if (bloby < 0) {
            bloby = 0;
            blobyv = 0;
          }
          else if (bloby > height) {
            bloby = height;
            blobyv = 0;
          }

          if (blobx < 0 || blobx > width) {
            blobxv = -blobxv;
          }
        }

        uint32_t getPixel(const Pixel &p)
        {
          // calc the distance from the pixel to the blob center
          float dx = p.x - blobx;
          float dy = p.y - bloby;
          float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.

          // calc the distance from the pixel to the blob
          int v = 255 * (blobRadius2 - dsq) / blobRadius2;
          if (v < 0)
            v = 0;
          return strip.ColorHSV(blobHue, 255, v);
        }
    };
    Blob b0{0*65536/5, 5, 10};
    Blob b1{1*65536/5, 25, 17};
    Blob b2{2*65536/5, 50, 12};
    Blob b3{3*65536/5, 75, 20};
    Blob b4{4*65536/5, 90, 15};
    Blob * blobs[5] = {&b0, &b1, &b2, &b3, &b4};

  public:
    virtual void begin() override {
      Serial.println("lavalamp.begin()");
    }

    virtual void update() override {
      for (Blob * blob : blobs) {
        blob->move();
      }
    }

    virtual uint32_t getColor(const Pixel &p) override {
      uint32_t color = 0;
      for (Blob * blob : blobs) {
        color = blend(color, blob->getPixel(p));
      }
      return color;
    }

} lavalamp;
