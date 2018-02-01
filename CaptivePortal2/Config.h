/*
	VibHub config and wifi
*/
#ifndef VibHub_Config_h
#define VibHub_Config_h



class ConfigClass{

	public:
		ConfigClass(void);
        
		bool begin( bool reset=false );
        void reset();
		
        
        char server[40] = "vibhub.io";
        char port[6]    = "80";
        char deviceid[24];
        
	private:
        
        
};

extern ConfigClass Config;

#endif //VibHub_Config_h