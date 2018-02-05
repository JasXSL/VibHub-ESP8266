#include "Arduino.h"
#include "Motor.h"

#include <TweenDuino.h> // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

void Motor::loadProgram( JsonArray &stages, int repeats ){

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
		Serial.print(i);

		JsonObject& s = stages[i];
		int intensity = 0;
		int duration = 1;
		char easing[48] = "Linear.None";
		int rep = 0;
		bool yoyo = false;

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
			if( !yoyo ) 
				timeline.addTo(_duty, (float)lastpwm, 1);

			int v = intensity;
			if( yflip )
				v = lastpwm;

			timeline.addTo(_duty, (float)v, duration);
			Serial.print("\nAdded playback: ");
			Serial.print("yFlip ");
			Serial.print(yflip);
			Serial.print(" | to ");
			Serial.print(v);
			Serial.print(" | Over ");
			Serial.print(duration);
			
			if( yoyo )
				yflip = !yflip;

		}
		

		lastpwm = intensity;

	}

	timeline.begin(millis());

}

void Motor::update(){

	uint32_t time = millis();
	timeline.update(time);
	if( timeline.isComplete() ){

		if( _repeats == -1 || _repeats > 0 ){
			
			timeline.restartFrom(time);
			if( _repeats > 0 )
				--_repeats;

		}

		

	}

	setPWM(_duty);

}



