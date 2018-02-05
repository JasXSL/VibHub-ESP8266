/*
	VibHub config and wifi
*/
#ifndef VhConfig_h
#define VhConfig_h

#include "Configuration.h"


class Config{

	public:
		Config(void);
        
        void load( bool reset=false );
        void save();
        void reset();
		void generateDeviceID();
		
        
        char server[64];
        unsigned int port;
        char deviceid[64];
        
	private:
        void gen_random( char *s, const int len );
        
};

extern Config vhConf;

#endif //VhConfig_h