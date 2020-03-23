// Class to manage the speech timer

class Speech {

    int greenTime = 0;
    int yellowTime = 0;
    int redTime = 0;

  public:

    Speech(int greenMinute, int redMinute) {
      greenTime = greenMinute * 60;
      redTime = redMinute * 60;
      yellowTime = (greenTime + redTime) / 2;
    }

    void setTimes(int green, int yellow, int red) {
      greenTime = green;
      yellowTime = yellow;
      redTime = red;
    }

    void display(NumericDisplay &disp) {
      disp.formatRange(greenTime / 60, redTime / 60);
    }

    void checkTime(int seconds, Stoplight &stoplight) {
      if (seconds >= redTime)
        stoplight.set(Stoplight::SL_RED);
      else if (seconds >= yellowTime)
        stoplight.set(Stoplight::SL_YELLOW);
      else if (seconds >= greenTime)
        stoplight.set(Stoplight::SL_GREEN);
      else
        stoplight.set(Stoplight::SL_OFF);
    }
};
