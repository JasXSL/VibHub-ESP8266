#include "Arduino.h"
#include "Motor.h"
#include "driver/ledc.h"

#define PWM_FREQ 5000
#define PWM_RES 8



Motor::Motor( int pin, int channel ){

	pinMode(pin, OUTPUT);	

	ledcSetup(channel, PWM_FREQ, PWM_RES);
  	ledcAttachPin(pin, channel);

	_channel = channel;
	_duty = 0;

}

// Sets intensity between 0 and 100
void Motor::setIntensity( int intensity ){

	int duty = round((float)intensity/100.0*255);
	Motor::setPWM( duty );

}

void Motor::setPWM( int duty ){

	_duty = duty;
	ledcWrite(_channel, _duty);

}



