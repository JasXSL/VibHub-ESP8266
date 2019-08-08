#include "Config.h"
#include <Arduino.h>
#include <FS.h>
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
    
    if(!SPIFFS.begin()){
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
                String content = "";
                while(configFile.available())
                    content += char(configFile.read());
                Serial.println(content);

                DynamicJsonDocument jsonBuffer(1024);
                DeserializationError error = deserializeJson(jsonBuffer, content.c_str());


                if( !error ){

                    Serial.println("Parsed json:");
                    serializeJson(jsonBuffer, Serial);
                    Serial.println("");

                    strcpy(server, jsonBuffer["server"]);
                    port = jsonBuffer["port"];
                    strcpy(deviceid, jsonBuffer["deviceid"]);
                    
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
			gen_random(deviceid, 12);

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
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for( int i = 0; i < len; ++i )
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    
    s[len] = 0;

}

void VhConfig::save(){

    Serial.println("VhConfig::save");
    DynamicJsonDocument jsonBuffer(1024);
	JsonObject json = jsonBuffer.to<JsonObject>();
	json["server"] = server;
	json["port"] = port;
	json["deviceid"] = deviceid;

	File configFile = SPIFFS.open(ConfigPATH, "w");
	if( !configFile )
		Serial.println("failed to open config file for writing");
	
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
    
}


void VhConfig::reset(){
    Serial.println("VhConfig::reset");
    
    if(!SPIFFS.begin()){
        if( SPIFFS.exists(ConfigPATH) ){
            SPIFFS.remove(ConfigPATH);
        }
    }
}




VhConfig vhConf = VhConfig();
