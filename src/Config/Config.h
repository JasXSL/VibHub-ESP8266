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

	public:
		String ssid;
		String password;
		String device_id;
		String host;
		int port;

};

#endif