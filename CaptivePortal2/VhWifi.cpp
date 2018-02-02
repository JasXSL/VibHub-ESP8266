#include "VhWifi.h"
#include "Configuration.h"
#include "Config.h"
#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Vhled.h"



#define getssid() "VibHub_"+String((uint16_t)(ESP.getEfuseMac()>>32))

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
    // Set portal LED state
    vhled.setState(STATE_PORTAL);
    
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
}


VhWifi::VhWifi(){
    
}


void VhWifi::connect( bool force, bool reset ){
    WiFiManager wifiManager;
    
    // The extra parameters to be configured
    WiFiManagerParameter custom_server("server", "VibHud server", config.server, 40);
    WiFiManagerParameter custom_port("port", "VibHud port", config.port, 6);
    WiFiManagerParameter custom_deviceid("deviceid", "DeviceID", config.deviceid, 24);
    wifiManager.addParameter(&custom_server);
    wifiManager.addParameter(&custom_port);
    wifiManager.addParameter(&custom_deviceid);
    
    if (reset){
        Serial.println("Wifi settings reset");
        wifiManager.resetSettings();
    }
    
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

    
    String ssid = getssid();
    if( force ){
        if( !wifiManager.startConfigPortal(ssid.c_str()) ){
            vhled.setState( STATE_WIFI_ERR );
            Serial.println("failed to connect and hit timeout");
            //reset and try again, or maybe put it to deep sleep
            ESP.restart();
            delay(1000);
        }
    }
    else if( !wifiManager.autoConnect(ssid.c_str()) ){
        vhled.setState( STATE_WIFI_ERR );
        Serial.println("failed to connect and hit timeout");
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(1000);
    }
    
    if (shouldSaveConfig) {
        //read updated parameters
        strcpy(config.server, custom_server.getValue());
        strcpy(config.port, custom_port.getValue());
        strcpy(config.deviceid, custom_deviceid.getValue());
        config.save();
    }
    else
        Serial.println("No device ID change detected");
    
    Serial.print("local ip: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("connected");
}



VhWifi vhWifi = VhWifi();
