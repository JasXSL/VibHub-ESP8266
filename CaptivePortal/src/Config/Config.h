/*
	VibHub motor data
*/
#ifndef Config_h
#define Config_h

#include "Arduino.h"

class Config{

	public:
		Config();								
		void begin();			// Loads the config
		void save();
        void format();

	public:
        char device_id[64];
        char host[64];
		int port;

};

#endif