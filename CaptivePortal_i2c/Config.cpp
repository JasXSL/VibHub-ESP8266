#include "Config.h"
#include <Arduino.h>
#include <FS.h>
#if defined(ESP32)
#include <SPIFFS.h>
#endif
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include "Configuration.h"

#define ConfigPATH "/config.json"


VhConfig::VhConfig(void) :
    // server(DEFAULT_SERVER),
    port(DEFAULT_PORT)
{
    strcpy(server, DEFAULT_SERVER);
}


void VhConfig::load( bool reset ){
    Serial.println("VhConfig::load");
    
    #if defined(ESP8266)
    if(!SPIFFS.begin()){
    #elif defined(ESP32)
    if(!SPIFFS.begin(true)){
    #endif
        Serial.println("SPIFFS Mount Failed. Device may be damaged");
        return;
    }
    
    Serial.println("mounted file system");
    
    // Check if config exists
    if( SPIFFS.exists(ConfigPATH) ){

        // Reset button held
        if( reset )
            SPIFFS.remove(ConfigPATH);
        
        // No reset, load the file
        else{

            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open(ConfigPATH, "r");
            if( configFile ){

                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);
                DynamicJsonBuffer jsonBuffer;
                JsonObject& json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);

                if( json.success() ){

                    Serial.println("\nparsed json");

                    strcpy(server, json["server"]);
                    char p[5];
                    strcpy(p, json["port"]);
                    port = atoi(p);
                    strcpy(deviceid, json["deviceid"]);
                    
                }
                else
                    Serial.println("failed to load json config");

            }
            
        }

    }
    
    Serial.print("DeviceID: ");
	Serial.println(deviceid);
    Serial.print("Server: ");
	Serial.println(server);
    Serial.print("Port: ");
	Serial.println(port);

	if( deviceid[0] == '\0' || port == 0 || port > 65535 || server[0] == '\0' ){

		if( deviceid[0] == '\0' ){

			Serial.println("No device ID found, randomizing one");
			gen_random(deviceid, 16);

		}

		if( port == 0 || port > 65535 ){
			
			Serial.println("Invalid port, resetting to default");
			port = DEFAULT_PORT;

		}

		if( server[0] == '\0' ){

			Serial.println("Invalid server, resetting to factory default");
			strcpy(server, DEFAULT_SERVER);

		}

		save();
	}
    

}

void VhConfig::gen_random( char *s, const int len ){
    
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for( int i = 0; i < len; ++i )
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    
    s[len] = 0;

}

void VhConfig::save(){

    Serial.println("VhConfig::save");
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();
	json["server"] = server;
	json["port"] = port;
	json["deviceid"] = deviceid;

	File configFile = SPIFFS.open(ConfigPATH, "w");
	if( !configFile )
		Serial.println("failed to open config file for writing");
	
	json.printTo(Serial);
	json.printTo(configFile);
	configFile.close();
    
}


void VhConfig::reset(){
    Serial.println("VhConfig::reset");
    
    #if defined(ESP8266)
    if(!SPIFFS.begin()){
    #elif defined(ESP32)
    if(!SPIFFS.begin(true)){
    #endif
        if( SPIFFS.exists(ConfigPATH) ){
            SPIFFS.remove(ConfigPATH);
        }
    }
}




VhConfig vhConf = VhConfig();
