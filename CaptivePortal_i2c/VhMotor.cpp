#include "Arduino.h"
#include "VhMotor.h"
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson



VhMotor::VhMotor( int channel ) :
    _channel(channel),
    _duty(0)
{
    // Force initial state
    setPWM(0);
}

void VhMotor::loadProgram( JsonArray stages, int repeats = 0 ){

	#ifdef DEBUG
		Serial.println();
		Serial.printf("Loading new program with #%i stages.\n", stages.size());
	#endif
	
	program.reset(repeats);
	for( auto stage : stages )
		program.addStageFromJson(stage);
	
	program.start();

}

void VhMotor::stopProgram(){
	program.completed = true;	// Makes sure we don't run loop on the program
	program.reset(0);			// Frees up some memory
}

void VhMotor::update(){

	if( !program.loop() )
		return;

	_duty = floor(program.value);
	if( _duty < 0 )
		_duty = 0;
	else if( _duty > 255 )
		_duty = 255;

	setPWM(_duty);

}

// fast_decay and forward are false by default
void VhMotor::setPWM( int duty, bool fast_decay, bool forward ){
	//Serial.printf("Setting duty: %i \n", duty);
	_duty = duty;
    pwm.setMotor(_channel, duty, fast_decay, forward);
}

