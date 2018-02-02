/*
	VibHub Led
*/
#ifndef Vhled_h
#define Vhled_h

#include "VhPwm.h"

//TODO: Color enum class

//TODO: enum class
#define STATE_BOOT 0			// Program is booting	<0,0,128>
#define STATE_PORTAL 1			// Program is in captive portal mode	<0,0,255> blinking
#define STATE_WIFI_ERR 2		// Unable to connect to wifi	<255,0,0>
#define STATE_SOCKET_ERR 3		// Unable to connect to socket server 	<128,128,0>
#define STATE_RUNNING 4			// Connected and listening	<0,255,0>

class Vhled{

	public:
		Vhled(void);
        
        void setup();
		void setState( int state = STATE_BOOT );
		
        
	private:
        int programState;			// State the program is currently in
        
};





extern Vhled vhled;

#endif //Vhled_h