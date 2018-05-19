
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
    void setMotor(uint8_t motor, uint8_t duty, bool fast_decay, bool forward);
};

extern VhPwm pwm;

#endif

