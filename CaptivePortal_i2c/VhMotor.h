
/*
	VibHub motor data
*/
#ifndef VhMotor_h
#define VhMotor_h
#include "VhPwm.h"
#include <vector>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino

class VhMotor: public VhPwm{

	public:
		VhMotor( int pin, int channel );
		void loadProgram( JsonArray &stages, int repeats );
		void update();
        
        void setPWM( int duty );

	private:
		TweenDuino::Timeline timeline;
		int _repeats;
		
    protected:
        int _channel;
        float _duty;
};

#endif
