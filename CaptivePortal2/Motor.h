
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h
#include "VhPwm.h"
#include <vector>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino

class Motor: public VhPwm{

	public:
		Motor( int pin, int channel ): VhPwm(pin, channel){}
		void loadProgram( JsonArray &stages, int repeats );
		void update();

	private:
		TweenDuino::Timeline timeline;
		int _repeats;
		

};

#endif
