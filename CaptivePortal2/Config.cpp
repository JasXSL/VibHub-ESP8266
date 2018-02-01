
#include "Config.h"

#include "Arduino.h"
#include <FS.h>
#include <SPIFFS.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

//gets called when WiFiManager enters configuration mode
// void configModeCallback (WiFiManager *myWiFiManager) {
    // Serial.println("Entered config mode");
    // Serial.println(WiFi.softAPIP());
    // //if you used auto generated SSID, print it
    // Serial.println(myWiFiManager->getConfigPortalSSID());
    // //entered config mode, make led toggle faster
    // // ticker.attach(0.2, tick);
// }


ConfigClass::ConfigClass(){
    
}


bool ConfigClass::begin( bool reset ){
    
    //Set default device ID
    #if defined(ESP8266)
        itoa(ESP.getChipId(), deviceid, 16);
    #else
        itoa(ESP.getEfuseMac(), deviceid, 16);
    #endif
    
    
    //clean FS, for testing
    // SPIFFS.format();

    //read configuration from FS json
    Serial.println("mounting FS...");
    
    #if defined(ESP8266)
    if (SPIFFS.begin()) {
    #elif defined(ESP32)
    if (SPIFFS.begin(true)) {
    #endif
        Serial.println("mounted file system");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success()) {
                    Serial.println("\nparsed json");

                    strcpy(server, json["server"]);
                    strcpy(port, json["port"]);
                    strcpy(deviceid, json["deviceid"]);
                } else {
                    Serial.println("failed to load json config");
                }
            }
        }
    } else {
        Serial.println("failed to mount FS");
    }
    //end read
    
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    
    
    // The extra parameters to be configured
    WiFiManagerParameter custom_server("server", "VibHud server", server, 40);
    WiFiManagerParameter custom_port("port", "VibHud port", port, 6);
    WiFiManagerParameter custom_deviceid("deviceid", "DeviceID", deviceid, 24);
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
    // wifiManager.setAPCallback(configModeCallback);
    
    //set minimu quality of signal so it ignores AP's under that quality
    //defaults to 8%
    wifiManager.setMinimumSignalQuality();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setConfigPortalTimeout(120);

    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point with the specified name
    //and goes into a blocking loop awaiting configuration
    //default password is password
    String ssid = "VibHub_" + String(deviceid);
    if (!wifiManager.autoConnect(ssid.c_str(), "password")) {
        Serial.println("failed to connect and hit timeout");
        return false;
    }
    
    Serial.println("connected");

    //read updated parameters
    strcpy(server, custom_server.getValue());
    strcpy(port, custom_port.getValue());
    strcpy(deviceid, custom_deviceid.getValue());

    //save the custom parameters to FS
    if (shouldSaveConfig) {
        Serial.println("saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        json["server"] = server;
        json["port"] = port;
        json["deviceid"] = deviceid;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile) {
            Serial.println("failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        configFile.close();
        //end save
    }

    Serial.println("local ip");
    Serial.println(WiFi.localIP());
    
    return true;
}

void ConfigClass::reset(){
    Serial.println("Resetting config");
    SPIFFS.format();
}


ConfigClass Config = ConfigClass();
