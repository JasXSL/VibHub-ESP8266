/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h



//TODO: Currently the ESP32 branch of WifiManager doesn't work correctly on the ESP8266
// #if defined(ESP8266)
// #include "WiFiManager8266.h"
// #elif defined(ESP32)
#include "Configuration.h"
#include "WiFiManager.h"
// #endif


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