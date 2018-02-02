/*
	VibHub config and wifi
*/
#ifndef VhConfig_h
#define VhConfig_h



class Config{

	public:
		Config(void);
        
        void load();
        void save();
        void reset();
		
        
        char server[40] = "vibhub.io";
        char port[6]    = "80";
        char deviceid[24];
        
	private:
        
        
};

extern Config config;

#endif //VhConfig_h