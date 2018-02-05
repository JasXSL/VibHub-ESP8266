
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h

#include "VhPwm.h"
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

class Motor: public VhPwm{

	public:
		Motor( int pin, int channel ): VhPwm(pin, channel){}
		void loadProgram( JsonArray &stages, int repeats );

};

#endif
