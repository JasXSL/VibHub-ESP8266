#include <Arduino.h>
#include "VhPwm.h"
#include "driver/ledc.h"

#define LEDC_FREQ 12000
#define LEDC_RES 8


VhPwm::VhPwm( int pin, int channel ){

  pinMode(pin, OUTPUT); 

  ledcSetup(channel, LEDC_FREQ, LEDC_RES);
  ledcAttachPin(pin, channel);

  _channel = channel;
  _duty = 0;

}

void VhPwm::setPWM( int duty ){

  _duty = duty;
  ledcWrite(_channel, round(_duty));

}




