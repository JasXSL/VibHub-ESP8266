#include "Arduino.h"
#include "Motor.h"
#include "driver/ledc.h"

#define LEDC_FREQ 12000
#define LEDC_RES 8


Motor::Motor( int pin, int channel ){

  pinMode(pin, OUTPUT); 

  ledcSetup(channel, LEDC_FREQ, LEDC_RES);
  ledcAttachPin(pin, channel);

  _channel = channel;
  _duty = 0;

}

void Motor::setPWM( int duty ){

  _duty = duty;
  ledcWrite(_channel, _duty);

}




