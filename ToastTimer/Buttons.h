class Buttons {
    const int grayPin = 5;
    const int greenPin = 6;
    const int redPin = 9;

    bool grayPressed = false;
    bool greenPressed = false;
    bool redPressed = false;

  public:
    void begin() {
      pinMode(grayPin, INPUT_PULLUP);
      pinMode(greenPin, INPUT_PULLUP);
      pinMode(redPin, INPUT_PULLUP);
    }

    bool getGray() {
      return digitalRead(grayPin) == LOW;
    }

    bool getGreen() {
      return digitalRead(greenPin) == LOW;
    }

    bool getRed() {
      return digitalRead(redPin) == LOW;
    }

    bool getGrayPressed() {
      return getPressed(getGray(), grayPressed);
    }

    bool getGreenPressed() {
      return getPressed(getGreen(), greenPressed);
    }

    bool getRedPressed() {
      return getPressed(getRed(), redPressed);
    }

  private:
    bool getPressed(bool raw, bool &pressed) {
      bool newPress = raw && !pressed;
      pressed = raw;
      return newPress;
    }
};
