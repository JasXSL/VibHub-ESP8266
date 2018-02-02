/*
	VibHub motor data
*/
#ifndef VibHud_Motor_h
#define VibHud_Motor_h

#include <Wire.h>
// #include <Adafruit_MotorShield.h>

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

class Motor{

	public:
		Motor(void);
        
		void begin(uint16_t freq = 1600, TwoWire *theWire = NULL);
        
		void setIntensity( uint8_t channel, uint8_t intensity );
        void run( uint8_t channel, uint8_t cmd );
		
        void setAll( uint8_t *intensities );
        void runAll( uint8_t cmd );
        
	private:
		


};

extern Motor motorCtrl;

#endif //VibHud_Motor_h