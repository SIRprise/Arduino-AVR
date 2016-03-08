#ifndef DebouncedInput_h
#define DebouncedInput_h

#include "Arduino.h"

class DebouncedInput
{
  public:
    DebouncedInput();
    void init(int* pin, int nbOfPins); //pullups will be set
    int getButton(); // 0 if no button pressed, returns pin number of button pressed
    int getButtonBusy(); //blocking
    unsigned long metrGetButtonBusyTime(); //time of pressing any of the buttons (blocking)
  private:
    int _nbOfPins;
    int* _pin;
};

#endif
