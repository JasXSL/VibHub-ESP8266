
/*
  VibHub motor data
*/
#ifndef VhPwm_h
#define VhPwm_h

#include "Arduino.h"

class VhPwm{

  public:
    VhPwm() {}
    void begin();
    void enable();
    void disable();
    void setPin(uint8_t pin, uint16_t duty, bool invert=false);
};

extern VhPwm pwm;

#endif

