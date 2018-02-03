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
    
    if (reset){
        Serial.println("Wifi settings reset");
        wifiManager.resetSettings();
    }
    
    
    // The extra parameters to be configured
    WiFiManagerParameter devId("deviceid", "Device ID", config.deviceid, 64);
    WiFiManagerParameter serverHost("server", "Server Host", config.server, 64);
    char port[6];
    itoa(config.port, port, 10);
    WiFiManagerParameter serverPort("port", "Server Port", port, 6);
    
    wifiManager.addParameter(&devId);
    wifiManager.addParameter(&serverHost);
    wifiManager.addParameter(&serverPort);
    
    
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
        
        Serial.println("Configuration change detected, saving and rebootski");
        
        //read updated parameters
        strcpy(config.deviceid, devId.getValue());
        strcpy(config.server, serverHost.getValue());
        char p[5];
        strcpy(p, serverPort.getValue());
        config.port = atoi(p);
        
        config.save();

        ESP.restart();
        delay(1000);
    }
    else
        Serial.println("No device ID change detected");
    
    // Serial.print("local ip: ");
    // Serial.println(WiFi.localIP());
    
    // Serial.println("connected");
}



VhWifi vhWifi = VhWifi();
