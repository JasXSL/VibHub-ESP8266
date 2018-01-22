#include "FS.h"
#ifdef defined(ESP32)
#include "SPIFFS.h"
#endif
#define ARDUINOJSON_ENABLE_ARDUINO_STRING 1
#include <ArduinoJson.h>
#include "Config.h"

#define ConfigPATH "/config.txt"



// Loads the config
Config::Config(){}


void Config::begin(){
    
    strlcpy(host, "vibhub.dangly.parts", sizeof(host));
    port = 80;
    
    char * path = ConfigPATH;
    
    #if defined(ESP8266)
        if (!SPIFFS.begin()) {
            Serial.println("Failed to mount file system");
            return;
        }
        File configFile = SPIFFS.open(path, "r");
    #elif defined(ESP32)
        if(!SPIFFS.begin(true)){
            Serial.println("Failed to mount file system");
            return;
        }
        FS fs = SPIFFS;
        File configFile = fs.open(path);
    #endif
    
    if (!configFile) {
        Serial.println("Config file does not exist yet, creating.");
        save();
        return;
    }
    
    Serial.printf("Reading file: %s\n", path);
    
    size_t size = configFile.size();
    Serial.println("Config file size: " + String(size));
    
    
    
    if (size > 1024) {
        Serial.println("Config file size is too large");
        return;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);
    configFile.close();

    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());

    if (!json.success()) {
        Serial.println("Failed to parse config file, resetting config");
        save();
        return;
    }
    
    Serial.println("Config file contents:");
    json.printTo(Serial);
    Serial.println("");

    if (json["confver"] == 1) {
        strlcpy(device_id, json["device_id"] | device_id, sizeof(device_id));
        strlcpy(host, json["host"] | host, sizeof(host));
        port = json["port"];
    }
    else {
        Serial.printf("Unknown config file version: %s\n", json["confver"]);
        save();
        return;
    }
    
    Serial.println("DeviceID: "+(String)device_id);
    Serial.println("Host: "+(String)host);
    Serial.println("Port: "+(String)port);
    
}

void Config::save(){
    
    char * path = ConfigPATH;
    #if defined(ESP8266)
        if (!SPIFFS.begin()) {
            Serial.println("Failed to mount file system");
            return;
        }
        File configFile = SPIFFS.open(path, "w");
    #elif defined(ESP32)
        if(!SPIFFS.begin(true)){
            Serial.println("Failed to mount file system");
            return;
        }
        FS fs = SPIFFS;
        File configFile = fs.open(path, FILE_WRITE);
    #endif
    
    if (!configFile) {
      Serial.println("Config file creation failed");
    }
    
    Serial.printf("Writing config file: %s\n", path);
    
    
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["confver"] = 1;
    json["device_id"] = device_id;
    json["host"] = host;
    json["port"] = port;
    
    json.printTo(configFile);
    configFile.close();
    
}


void Config::format(){
    //TODO: format may be overkill.
    
    Serial.println("Formating file system");
    #if defined(ESP8266)
      if (SPIFFS.begin()) SPIFFS.format();
      else Serial.println("Failed to mount file system");
    #elif defined(ESP32)
        if(SPIFFS.begin(true)) SPIFFS.format();
        else Serial.println("Failed to mount file system");
    #endif
    
}
