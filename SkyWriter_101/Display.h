#if !defined(DISPLAY_H_)
#define DISPLAY_H_

class Display {
  public:
    virtual int getWidth() const;
    virtual void displayColumn(int col) const;
};

#endif // DISPLAY_H_
