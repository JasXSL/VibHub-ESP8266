
/*
	VibHub motor data
*/
#ifndef VhMotor_h
#define VhMotor_h
#include "VhPwm.h"
#include <vector>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "TweenDuino.h" // https://github.com/stickywes/TweenDuino
#include "VhProgramStage.h"
// Motor class extending PWM class
class VhMotor: public VhPwm{

	public:
		VhMotor( int channel );
        
		void loadProgram( JsonArray &stages, int repeats );
		void update();
        void setPWM( int duty, bool fast_decay = false, bool forward = false );
		void stopProgram();

	private:
		TweenDuino::Timeline timeline;
		int _repeats;
		bool program_running = false;
		std::vector<VhProgramStage> _active_program;
		void playProgram();					// Plays the loaded program
		
    protected:
        int _channel;
        float _duty;
};

#endif
