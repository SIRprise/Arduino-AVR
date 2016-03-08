#ifndef SoftPWM_h
#define SoftPWM_h

#include "Arduino.h"

namespace SoftPWM {
    void init(int pin);
    void start();
    void stop();
    void setDuty(unsigned char val);

    extern int _pin;
}

#endif
