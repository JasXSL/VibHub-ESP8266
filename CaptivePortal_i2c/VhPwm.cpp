#include <Arduino.h>
#include "VhPwm.h"
#include "Configuration.h"

#include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

#include "PCA9634.h"

// Adafruit_PWMServoDriver _pwm = Adafruit_PWMServoDriver(PWMI2CADDRESS);
pca9634 _pwm = pca9634(PWMI2CADDRESS);

void VhPwm::begin()
{
    pinMode(PIN_ENABLE, OUTPUT);
    digitalWrite(PIN_ENABLE, LOW);
    
    // pinMode(PIN_OE, OUTPUT);
    // // digitalWrite(PIN_OE, HIGH); //disable chip
    // digitalWrite(PIN_OE, LOW); //enable chip
    // _pwm.begin();
    // _pwm.reset();
    // _pwm.setPWMFreq(PWMFREQ);
    Wire.setClock(I2CCLOCK);
    
    _pwm.begin();
    _pwm.set_mode2(0x02);
}

void VhPwm::enable(){
    digitalWrite(PIN_ENABLE, HIGH);
}

void VhPwm::disable(){
    digitalWrite(PIN_ENABLE, LOW);
}

void VhPwm::setPin( uint8_t pin, uint16_t duty, bool invert ){
    Serial.printf("setPin: %d = %d\n", pin, duty);
    // _pwm.setPin(pin, duty, invert);
    _pwm.set_duty(pin, invert ? (255 - duty) : duty);
}


VhPwm pwm = VhPwm();
