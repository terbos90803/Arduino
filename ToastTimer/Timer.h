// Basic timer

class Timer {
    unsigned long elapsed = 0;
    unsigned long lastTime = 0;
    bool running = false;

  public:

    void reset() {
      elapsed = 0;
      running = false;
    }

    void start() {
      lastTime = millis();
      running = true;
    }

    void stop() {
      running = false;
    }

    int getSeconds() {
      if (running) {
        unsigned long now = millis();
        elapsed += now - lastTime;
        lastTime = now;
      }
      return elapsed / 1000;
    }

    void display(NumericDisplay &display, bool blinkColon) {
      int seconds = getSeconds();
      bool blink = (elapsed / 500) % 2;
      bool showColon = blinkColon ? blink : true;
      display.formatTime(seconds, showColon);
    }
};
