#include "VhWifi.h"
#include "Configuration.h"
#include "Config.h"
#include <Arduino.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Vhled.h"
#include <qrcode.h>

#if defined(ESP8266)
#define getssid() "VibHub_"+String((uint16_t)ESP.getChipId())
#elif defined(ESP32)
#define getssid() "VibHub_"+String((uint16_t)(ESP.getEfuseMac()>>32))
#endif


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("VhWifi: Should save config");
  shouldSaveConfig = true;
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
    // Set portal LED state
    vhled.setState(STATE_PORTAL);
    
    Serial.println("VhWifi: Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
}


void VhWifi::connect( bool force, bool reset ){
    WiFiManager wifiManager;
    _wifiManager = &wifiManager;
    
    if (reset){
        clearSettings();
    }
    
    // The extra parameters to be configured
    //WiFiManagerParameter devId("deviceid", "Device ID", vhConf.deviceid, 64);
    WiFiManagerParameter serverHost("server", "Server Host", vhConf.server, 64);
    char port[6];
    itoa(vhConf.port, port, 10);
    WiFiManagerParameter serverPort("port", "Server Port", port, 6);
    
    String ct = "Device ID: ";
    ct += vhConf.deviceid;
    WiFiManagerParameter custom_text(ct.c_str());



    //wifiManager.addParameter(&devId);
    wifiManager.addParameter(&serverHost);
    wifiManager.addParameter(&serverPort);
    wifiManager.addParameter(&custom_text);
    
    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);
    
    //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(configModeCallback);
    
    //set minimu quality of signal so it ignores AP's under that quality
    //defaults to 8%
    wifiManager.setMinimumSignalQuality();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setConfigPortalTimeout(120);

    Serial.printf("Free memory: %i \n", free);
    
    String ssid = getssid();
    if( force ){
        if( !wifiManager.startConfigPortal(ssid.c_str()) ){
            vhled.setState( STATE_WIFI_ERR );
            Serial.println("VhWifi: Failed to connect and hit timeout");
            //reset and try again, or maybe put it to deep sleep
            ESP.restart();
            delay(1000);
        }
    }
    else if( !wifiManager.autoConnect(ssid.c_str()) ){
        vhled.setState( STATE_WIFI_ERR );
        Serial.println("VhWifi: Failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(1000);
    }
    
    
    if (shouldSaveConfig) {
        
        Serial.println("VhWifi: Configuration change detected, saving and rebootski");
        
        //read updated parameters
        //strcpy(vhConf.deviceid, devId.getValue());
        strcpy(vhConf.server, serverHost.getValue());
        char p[5];
        strcpy(p, serverPort.getValue());
        vhConf.port = atoi(p);
        
        vhConf.save();

        ESP.restart();
        delay(1000);
    }
    else
        Serial.println("VhWifi: No device ID change detected");
    
    
    
    Serial.print("VhWifi: local ip: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("VhWifi: connected");
}

void VhWifi::clearSettings(){
    Serial.println("VhWifi::clearSettings(");
    if (_wifiManager){
        _wifiManager->resetSettings();
    }
}


String VhWifi::getCustomJSPre(){
    String out = "const QR_SIZE = 25;";

    out+= "const QR_DATA ='";
    QRCode qrcode;
    uint8_t qrcodeData[qrcode_getBufferSize(2)];
    String text = "vibhub:";
    text+= vhConf.deviceid;
    qrcode_initText(&qrcode, qrcodeData, 2, ECC_MEDIUM, text.c_str());
    for( uint8 y = 0; y < qrcode.size; y++ ){
		for( uint8 x = 0; x < qrcode.size; x++ )
            out+= qrcode_getModule(&qrcode, x, y) ? "1" : "0";
	}
    out += "';";
    return out;
}



VhWifi vhWifi = VhWifi();
