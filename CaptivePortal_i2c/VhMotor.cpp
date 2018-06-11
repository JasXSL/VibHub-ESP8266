#include "Arduino.h"
#include "VhMotor.h"
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "VhPwm.h"
#include "RandObject.h"


VhMotor::VhMotor( int channel ) :
    _channel(channel),
    _duty(-1)
{
    // Force initial state
    setPWM(0);
}

// Plays cached program
void VhMotor::playProgram(){

	timeline = TweenDuino::Timeline();
	int size = _active_program.size();
	int lastpwm = _duty;
	Serial.printf("PlayProgram, received %i stages\n", size);
	long free = ESP.getFreeHeap();
	Serial.printf("Free memory: %i\n", free);

	if(!size){
		stopProgram();
		return;
	}

	int totalDuration = 0;
	for( VhProgramStage chunk : _active_program ){
		int r;
		int intens = chunk.intensity.getValue(lastpwm);
		int lastIntensity = lastpwm;
		int dur = chunk.duration.getValue();
		int rep = chunk.repeats.getValue();
		if( dur < 1 )
			dur = 1;
		if( rep < 0 )
			rep = 0;
		++rep;
		totalDuration += dur;
		for( r = 0; r < rep; ++r ){

			// Snapback repeats
			if( !chunk.yoyo && r ) 
				timeline.addTo(_duty, (float)lastpwm, 1);

			int v = intens;
			if( chunk.yoyo && r%2 == 1 )
				v = lastpwm;
			
			//Serial.printf("Added playback from %i to %i over %i ms | yFlip %i\n", lastIntensity, v, dur, yoyo && r%2 == 1);	
			lastIntensity = v;
			timeline.addTo(_duty, (float)v, dur, chunk.ease, chunk.easeType);

		}

		lastpwm = lastIntensity;

	}

	Serial.printf("Program built, total duration: %i!\n", totalDuration);
	program_running = true;
	timeline.begin(millis());

}

void VhMotor::loadProgram( JsonArray &stages, int repeats = 0 ){

	Serial.println();
	Serial.printf("Loading new program on channel %i with %i stages.\n", _channel, stages.size());
	_repeats = repeats;
	std::vector<VhProgramStage>().swap(_active_program);
	for( auto stage : stages ){
		_active_program.push_back(VhProgramStage(stage.as<JsonObject>()));
	}
	playProgram();

}

void VhMotor::stopProgram(){
	program_running = false;
}

void VhMotor::update(){

	if(!program_running)
		return;

	uint32_t time = millis();
	timeline.update(time);

	if( timeline.isComplete() ){
		// Handle repeats
		if(_repeats == -1 || _repeats > 0){
			//timeline.restartFrom(time);
			Serial.println("Program completed, looping");
			playProgram();
			if( _repeats > 0 )
				--_repeats;
		}
		else
			stopProgram();
	}

	//Serial.printf("Setting program duty: %f\n", _duty);
	setPWM(_duty);

}

void VhMotor::setPWM( int duty, bool fast_decay, bool forward ){
    if (_duty != duty){
        _duty = duty;
        pwm.setMotor(_channel, duty, fast_decay, forward);
    }
}

