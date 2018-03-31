#if !defined(DISPLAY_H_)
#define DISPLAY_H_

class Display {
  public:
    virtual int getWidth() const = 0;
    virtual unsigned long getColumnTime() const = 0;
    virtual void displayColumn(int col) const = 0;
};

#endif // DISPLAY_H_
