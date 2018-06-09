#include <Arduino.h>
#include "VhPwm.h"
#include "Configuration.h"
#include <Wire.h>
#include "PCA9634.h"

pca9634 pwmdriver(PWMI2CADDRESS);

void VhPwm::begin()
{
    pinMode(PIN_ENABLE, OUTPUT);
    disable();
    
    Wire.begin();
    Wire.setClock(I2CCLOCK);
    
    pwmdriver.begin();
}

void VhPwm::enable(){
    digitalWrite(PIN_ENABLE, HIGH);
}

void VhPwm::disable(){  
    digitalWrite(PIN_ENABLE, LOW);
}

void VhPwm::setMotor( uint8_t motor, uint8_t duty, bool fast_decay, bool forward ){
    // double motor index to give us the pwm pin offset
    // Double are for if you want to reverse to make it "choppy"
    motor = motor*2;
    
    if (forward) {
        if (fast_decay) {
            pwmdriver.set_duty(motor, duty);
            pwmdriver.set_duty(motor+1, 0);
        }
        else { // slow decay
            pwmdriver.set_duty(motor, 255);
            pwmdriver.set_duty(motor+1, 255-duty);
        }
    }
    else { // reverse
        if (fast_decay) {
            pwmdriver.set_duty(motor, 0);
            pwmdriver.set_duty(motor+1, duty);
        }
        else { // slow decay
           pwmdriver.set_duty(motor, 255-duty);
            pwmdriver.set_duty(motor+1, 255); 
        }
    }
}

VhPwm pwm = VhPwm();
