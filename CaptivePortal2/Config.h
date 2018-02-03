/*
	VibHub config and wifi
*/
#ifndef VhConfig_h
#define VhConfig_h

#include "Configuration.h"


class Config{

	public:
		Config(void) {}
        
        void load( bool reset=false );
        void save();
        void reset();
		void generateDeviceID();
		
        
        char server[64]     = DEFAULT_SERVER;
        unsigned int port   = DEFAULT_PORT;
        char deviceid[64];
        
	private:
        void gen_random( char *s, const int len );
        
};

extern Config config;

#endif //VhConfig_h