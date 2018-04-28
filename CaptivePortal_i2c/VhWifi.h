/*
	VibHub wifi
*/
#ifndef VhWifi_h
#define VhWifi_h



//TODO: Currently the ESP32 branch of WifiManager doesn't work correctly on the ESP8266
// #if defined(ESP8266)
// #include "WiFiManager8266.h"
// #elif defined(ESP32)
#include "WiFiManager.h"
// #endif

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