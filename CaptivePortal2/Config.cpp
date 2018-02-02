#include "Config.h"
#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson


Config::Config(){
    
}


void Config::load(){
    
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
    
    if (SPIFFS.begin(true)) {
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
}


    
void Config::save(){
    //save the custom parameters to FS
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

void Config::reset(){
    Serial.println("Resetting config");
    SPIFFS.format();
}


Config config = Config();
