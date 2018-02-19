#include <Arduino.h>
#include "VhPwm.h"
#include "Configuration.h"


#if defined(ESP8266)

VhPwm::VhPwm( int pin, int channel ) :
    _channel(pin),
    _duty(0)
{
    pinMode(pin,OUTPUT);
}

void VhPwm::setPWM( int duty ){

    _duty = duty;
    analogWrite(_channel, round(_duty));

}


#elif defined(ESP32)

#include "driver/ledc.h"

VhPwm::VhPwm( int pin, int channel ) :
    _channel(channel),
    _duty(0)
{
    
    ledcSetup(channel, LEDC_FREQ, LEDC_RES);
    ledcAttachPin(pin, channel);
    
}

void VhPwm::setPWM( int duty ){

    _duty = duty;
    ledcWrite(_channel, round(_duty));

}

#endif






