#include "Arduino.h"
#include "Motor.h"


#include <Wire.h>
// #include <Adafruit_MotorShield.h>

// Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Adafruit_DCMotor *Motors [4] = {
    // AFMS.getMotor(1),
    // AFMS.getMotor(2),
    // AFMS.getMotor(3),
    // AFMS.getMotor(4)
// };

Motor::Motor(){
    
}

void Motor::begin( uint16_t freq, TwoWire *theWire ){
    // AFMS.begin( freq, theWire);
}

// Sets channels 1-4 at intensity between 0 and 255
void Motor::setIntensity( uint8_t channel, uint8_t intensity ){

	// Motors[channel]->setSpeed(intensity);

}

void Motor::run( uint8_t channel, uint8_t cmd ){
    
    // Motors[channel]->run(cmd);
    
}

void Motor::setAll( uint8_t *intensities ){
    
    // for(int i=0; i<4; i++) {
        // Motors[i]->setSpeed(intensities[i]);
    // }
    
}

void Motor::runAll( uint8_t cmd ){
    
    // for(int i=0; i<4; i++) {
        // Motors[i]->run(cmd);
    // }
    
}

Motor motorCtrl = Motor();
