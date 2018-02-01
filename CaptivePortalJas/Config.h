/*
	VibHub motor data
*/
#ifndef Config_h
#define Config_h

class Config{

	public:
		Config();								
		void begin();			// Loads the config
		void save();
		void generateDeviceID();

		char server[64] = "vibhub.io";
		int port    = 80;
		char deviceid[24];

	private:
		void gen_random( char *s, const int len );


};

#endif