/*

*/


#include "DRV8833.h"


drv8833::drv8833(uint8_t address, uint8_t enable_pin) :
    pwm(address),
    eepin(enable_pin)
{}

void drv8833::begin(){
    
    pinMode(eepin, OUTPUT);
    disable();
    
    pwm.begin();
    pwm.set_mode2(0x04);
    
}


void drv8833::enable(){
    digitalWrite(eepin, HIGH);
}

void drv8833::disable(){
    digitalWrite(eepin, LOW);
}


void drv8833::setMotor( uint8_t motor, uint8_t duty, bool fast_decay, bool forward ){
    
    // double motor index to give us the pwm pin offset
    motor = motor*2;
    
    if (forward) {
        if (fast_decay) {
            pwm.set_duty(motor, duty);
            pwm.set_duty(motor+1, 0);
        }
        else { // slow decay
            pwm.set_duty(motor, 1);
            pwm.set_duty(motor+1, duty);
        }
    }
    else { // reverse
        if (fast_decay) {
            pwm.set_duty(motor, 0);
            pwm.set_duty(motor+1, duty);
        }
        else { // slow decay
           pwm.set_duty(motor, duty);
            pwm.set_duty(motor+1, 1); 
        }
    }
}
