#include <Arduino.h>
#include "VhPwm.h"
// #include "driver/ledc.h"
#include "Configuration.h"


VhPwm::VhPwm( int pin, int channel ) :
    // _channel(channel),
    _channel(pin),
    _duty(0)
{
    
    // ledcSetup(channel, LEDC_FREQ, LEDC_RES);
    // ledcAttachPin(pin, channel);
    
    pinMode(pin,OUTPUT);
    
}

void VhPwm::setPWM( int duty ){

    _duty = duty;
    // ledcWrite(_channel, round(_duty));
    analogWrite(_channel, round(_duty));

}




