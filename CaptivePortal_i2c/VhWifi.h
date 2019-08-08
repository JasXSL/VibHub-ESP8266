/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h


#include "Configuration.h"
#include "WiFiManager.h"



class VhWifi{
	public:
		VhWifi(void) {}
        
		void connect( bool force=false, bool resetSetting=false );
		void clearSettings();

	private:
        WiFiManager* _wifiManager;

		// Any non-constant data needed to be loaded should go in here
		String getCustomJSPre();		// Non constant data that should go above the constant data
		String getCustomJSPost();		// == || == below the constant data
		
};



extern VhWifi vhWifi;

#endif //VhWifi_h