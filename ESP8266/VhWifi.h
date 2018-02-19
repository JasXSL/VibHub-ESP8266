/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h

#include "WiFiManager.h"

class VhWifi{
	public:
		VhWifi(void) {}
        
		void connect( bool force=false, bool resetSetting=false );
		void clearSettings();
        
	private:
        WiFiManager* _wifiManager;
};





extern VhWifi vhWifi;

#endif //VhWifi_h