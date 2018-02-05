#include "Arduino.h"
#include "Motor.h"

#include <TweenDuino.h> // https://github.com/stickywes/TweenDuino
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

void Motor::loadProgram( JsonArray &stages, int repeats ){

	Serial.println();
	Serial.print("Program loaded on:");
	Serial.print(_channel);

	int size = stages.size();
	int i;
	for( i=0; i<size; ++i ){

		Serial.print("\nStage: ");
		Serial.print(i);

		JsonObject& s = stages[i];
		int intensity = 0;
		int duration = 0;
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

	}

}



