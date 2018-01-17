
/*
	VibHub motor data
*/
#ifndef Motor_h
#define Motor_h

class Motor{

	public:
		Motor( int pin, int channel );
		void setIntensity( int intensity );
		
	private:
		int _channel;
		int _duty;
		void setPWM( int duty );


};

#endif