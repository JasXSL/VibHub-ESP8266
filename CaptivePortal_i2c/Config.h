/*
	VibHub config
*/
#ifndef VhConfig_h
#define VhConfig_h

class VhConfig{

	public:
		VhConfig(void);
        
        void load( bool reset=false );
        void save();
        void reset();
		
        
        char server[64];
        unsigned int port;
        char deviceid[64];
        
	private:
        void gen_random( char *s, const int len );
        
};

extern VhConfig vhConf;

#endif //VhConfig_h