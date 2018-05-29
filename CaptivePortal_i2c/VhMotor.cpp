#include "Arduino.h"
#include "VhMotor.h"
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "VhPwm.h"



VhMotor::VhMotor( int channel ) :
    _channel(channel),
    _duty(0)
{}

void VhMotor::loadProgram( JsonArray &stages, int repeats ){

	Serial.println();
	Serial.print("Program loaded on:");
	Serial.print(_channel);

	_repeats = repeats;
	timeline = TweenDuino::Timeline();

	

	int size = stages.size();
	int i;
	int lastpwm = _duty;
	for( i=0; i<size; ++i ){

		Serial.print("\nStage: ");
		Serial.println(i);

		JsonObject& s = stages[i];
		int intensity = 0;
		int duration = 1;
		char easing[48] = "Linear.In";
		int rep = 0;
		bool yoyo = false;

		
		TweenDuino::Tween::Ease ease = TweenDuino::Tween::LINEAR;
		TweenDuino::Tween::EaseType easeType = TweenDuino::Tween::IN;
		
		if( s.containsKey("i") )
			intensity = s["i"];
		if( s.containsKey("d") )
			duration = s["d"];
		if( s.containsKey("e") )
			strcpy(easing, s["e"]);
		if( s.containsKey("r") )
			rep = s["r"];
		if( s.containsKey("y") )
			yoyo = s["y"] ? true : false;
		
		if( duration < 1 )
			duration = 1;
		if( rep < 0 )
			rep = 0;

		Serial.printf("Intensity: %i\n", intensity);
		
		char *token = strtok(easing, ".");

		std::vector<char*> tokens;
		while( token ){

			tokens.push_back(token);
			token = strtok(NULL,".");

		}

		if( tokens.size() >= 2 ){

			Serial.print("Easing: ");
			Serial.print(tokens[0]);
			Serial.print(" >> ");
			Serial.print(tokens[1]);

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
				Serial.print("\nUnknown easing function: ");
				Serial.print(tokens[0]);
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

		++rep;
		int r;
		bool yflip = false;
		for( r = 0; r < rep; ++r ){

			
			// Snapback repeats
			if( !yoyo && r ) 
				timeline.addTo(_duty, (float)lastpwm, 1);

			int v = intensity;
			if( yflip )
				v = lastpwm;
			
			timeline.addTo(_duty, (float)v, duration, ease, easeType);

			Serial.print("\nAdded playback: ");
			Serial.printf(" From %i to %i over %i ms | yFlip %b\n", lastpwm, v, duration, yflip);	
			if( yoyo )
				yflip = !yflip;

		}
		

		lastpwm = intensity;

	}

	program_running = true;
	timeline.begin(millis());

}

void VhMotor::stopProgram(){
	program_running = false;
}

void VhMotor::update(){

	if(!program_running)
		return;

	uint32_t time = millis();
	timeline.update(time);
	//Serial.println(_duty);
	if( timeline.isComplete() ){
		// Handle repeats
		if(_repeats == -1 || _repeats > 0){
			timeline.restartFrom(time);
			if( _repeats > 0 )
				--_repeats;
		}
		else
			stopProgram();
	}

	//Serial.printf("Setting program duty: %i\n", _duty);
	setPWM(_duty);

}

void VhMotor::setPWM( int duty, bool fast_decay, bool forward ){
    if (_duty != duty){
        _duty = duty;
        pwm.setMotor(_channel, duty, fast_decay, forward);
    }
}

