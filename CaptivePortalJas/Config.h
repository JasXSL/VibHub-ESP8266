/*
	VibHub motor data
*/
#ifndef Config_h
#define Config_h

#define DEFAULT_SERVER "vibhub.io"
#define DEFAULT_PORT 80

class Config{

	public:
		Config();								
		void begin( bool reset );			// Loads the config
		void save();
		void generateDeviceID();

		char server[64] = DEFAULT_SERVER;
		int port    = DEFAULT_PORT;
		char deviceid[64];

	private:
		void gen_random( char *s, const int len );


};

extern Config vhConf;

#endif