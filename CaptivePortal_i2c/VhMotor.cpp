#include "Arduino.h"
#include "VhMotor.h"
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "VhPwm.h"
#include "RandObject.h"


VhMotor::VhMotor( int channel ) :
    _channel(channel),
    _duty(0)
{
}

// Plays cached program
void VhMotor::playProgram(){
	JsonArray &stages = _active_program;
	timeline = TweenDuino::Timeline();

	int size = stages.size();
	int i;
	int lastpwm = _duty;
	Serial.printf("PlayProgram, received %i stages\n", size);
	long free = ESP.getFreeHeap();
	Serial.printf("Free memory: %i\n", free);
	if(!size){
		stopProgram();
		return;
	}

	int totalDuration = 0;
	for( i=0; i<size; ++i ){

		Serial.printf("\nStage #%i\n", i);
		JsonObject& s = stages[i];
		RandObject intensity = RandObject();
		RandObject duration = RandObject();
		RandObject repeats = RandObject();
		char easing[48] = "Linear.In";
		bool yoyo = false;
		
		TweenDuino::Tween::Ease ease = TweenDuino::Tween::LINEAR;
		TweenDuino::Tween::EaseType easeType = TweenDuino::Tween::IN;
		
		if( s.containsKey("i") )
			intensity.load(s["i"]);
		if( s.containsKey("d") )
			duration.load(s["d"]);
		if( s.containsKey("e") )
			strcpy(easing, s["e"]);
		if( s.containsKey("r") )
			repeats.load(s["r"]);
		if( s.containsKey("y") )
			yoyo = s["y"] ? true : false;

		char *token = strtok(easing, ".");
		std::vector<char*> tokens;
		while( token ){

			tokens.push_back(token);
			token = strtok(NULL,".");

		}

		if( tokens.size() >= 2 ){

			Serial.printf("Easing: %s.%s\n", tokens[0], tokens[1]);
			// Translate into a const
			if( strcmp(tokens[0], "Quadratic") == 0 )
				ease = TweenDuino::Tween::QUAD;
			else if( strcmp(tokens[0], "Cubic") == 0 )
				ease = TweenDuino::Tween::CUBIC;
			else if( strcmp(tokens[0], "Quartic") == 0 )
				ease = TweenDuino::Tween::QUART;
			else if( strcmp(tokens[0], "Quintic") == 0 )
				ease = TweenDuino::Tween::QUINT;
			else if( strcmp(tokens[0], "Sinusoidal") == 0 )
				ease = TweenDuino::Tween::SINE;
			else if( strcmp(tokens[0], "Exponential") == 0 )
				ease = TweenDuino::Tween::EXPONENTIAL;
			else if( strcmp(tokens[0], "Circular") == 0 )
				ease = TweenDuino::Tween::CIRCULAR;
			else if( strcmp(tokens[0], "Elastic") == 0 )
				ease = TweenDuino::Tween::ELASTIC;
			else if( strcmp(tokens[0], "Back") == 0 )
				ease = TweenDuino::Tween::BACK;
			else if( strcmp(tokens[0], "Bounce") == 0 )
				ease = TweenDuino::Tween::BOUNCE;
			else{
				Serial.printf("Unknown easing function: %s\n", tokens[0]);
			}
			
			if( strcmp(tokens[1], "In") == 0 )
				easeType = TweenDuino::Tween::IN;
			else if( strcmp(tokens[1], "Out") == 0 )
				easeType = TweenDuino::Tween::OUT;
			else if( strcmp(tokens[1], "InOut") == 0 )
				easeType = TweenDuino::Tween::INOUT;
			

		}
		

		/*
		Serial.print("\nIntensity: ");
		Serial.print(intensity);
		Serial.print(" | Duration: ");
		Serial.print(duration);
		Serial.print(" | Easing: ");
		Serial.print(easing);
		Serial.print(" | Repeats: ");
		Serial.print(rep);
		Serial.print(" | Yoyo: ");
		Serial.print(yoyo);
		
		Serial.println();
		*/

		
		
		int r;
		int intens = intensity.getValue(lastpwm);
		int lastIntensity = lastpwm;
		int dur = duration.getValue();
		int rep = repeats.getValue();
		if( dur < 1 )
			dur = 1;
		if( rep < 0 )
			rep = 0;
		++rep;
		totalDuration += dur;
		for( r = 0; r < rep; ++r ){

			// Snapback repeats
			if( !yoyo && r ) 
				timeline.addTo(_duty, (float)lastpwm, 1);

			int v = intens;
			if( yoyo && r%2 == 1 )
				v = lastpwm;
			
			//Serial.printf("Added playback from %i to %i over %i ms | yFlip %i\n", lastIntensity, v, dur, yoyo && r%2 == 1);	
			lastIntensity = v;
			timeline.addTo(_duty, (float)v, dur, ease, easeType);

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
	_active_program = stages;
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

