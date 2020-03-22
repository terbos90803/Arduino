class Buttons {
    const int gray = 5;
    const int green = 6;
    const int red = 9;

  public:
    Buttons() {
      pinMode(gray, INPUT_PULLUP);
      pinMode(green, INPUT_PULLUP);
      pinMode(red, INPUT_PULLUP);
    }

    bool getGray() {
      return digitalRead(gray) == LOW;
    }

    bool getGreen() {
      return digitalRead(green) == LOW;
    }

    bool getRed() {
      return digitalRead(red) == LOW;
    }
};
