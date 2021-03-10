// Heart animation

class Heart : public Raster
{
    int frame = 0;

  public:
    virtual void begin() override {
      Serial.println("heart.begin()");
    }

    virtual void update() override {
    }

} heart;
