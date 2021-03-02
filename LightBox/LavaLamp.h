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
        const uint32_t blobColor = 0xff0000;
        const uint32_t bkgColor = 0x000000;

        float blobx;
        float blobxv;
        float bloby = height;
        float blobyv = 0;
        float blobTemp = 50;

      public:
        Blob(int x, int r) : blobRadius(r), blobRadius2(r * r), kT(5.0 / blobRadius2) {
          blobx = x;
          blobxv = (float)x / (20 * width);
        }

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

        uint32_t getPixel(int x, int y)
        {
          // calc the distance from the pixel to the blob center
          float dx = x - blobx;
          float dy = y - bloby;
          float dsq = dx * dx + dy * dy; // work in squared space to avoid a sqrt.

          // calc the distance from the pixel to the blob
          return dsq <= blobRadius2 ? blobColor : bkgColor;
        }
    };
    Blob b0{5, 5};
    Blob b1{25, 12};
    Blob b2{50, 10};
    Blob b3{75, 15};
    Blob b4{90, 8};
    Blob * blobs[5] = {&b0, &b1, &b2, &b3, &b4};

  public:
    virtual void begin() {
      Serial.println("lavalamp.begin()");
    }

    virtual void update() {
      for (Blob * blob : blobs) {
        blob->move();
      }
    }

    virtual uint32_t getColor(int x, int y) {
      uint32_t color = 0;
      for (Blob * blob : blobs) {
        color = blend(color, blob->getPixel(x, y));
      }
      return color;
    }

} lavalamp;
