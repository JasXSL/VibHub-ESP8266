/*

*/


#include <Arduino.h>
#include "DRV8833.h"


drv8833::drv8833(uint8_t address, uint8_t enable_pin) :
    pwmdriver(address),
    eepin(enable_pin)
{}

void drv8833::begin(){
	pinMode(eepin, OUTPUT);
    disable();;
    
    pwmdriver.reset();
    pwmdriver.begin(PCA9634_MODE1_ALLCALLS, (PCA9634_MODE2_INVRT|PCA9634_MODE2_OUTDRV|PCA9634_MODE2_OUTNE0));
}


void drv8833::enable(){
    digitalWrite(eepin, HIGH);
}

void drv8833::disable(){
    digitalWrite(eepin, LOW);
}

void drv8833::setPin( uint8_t pin, uint8_t duty ){
	pwmdriver.set_duty(pin, duty);
}


void drv8833::setMotor( uint8_t motor, uint8_t duty, bool fast_decay, bool forward ){
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

