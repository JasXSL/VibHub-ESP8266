
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h

#include "VhPwm.h"
class Motor: public VhPwm{

	public:
		Motor( int pin, int channel ): VhPwm(pin, channel){}
		void loadProgram();

};

#endif
