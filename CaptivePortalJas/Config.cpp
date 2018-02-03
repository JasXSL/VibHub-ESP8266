#include "Config.h"
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#define ConfigPATH "/config.json"

// Dummy
Config::Config(){}


void Config::begin( bool reset ){

	if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed. Device may be damaged");
        return;
    }
    
	// Mount file system
	if( SPIFFS.begin(true) ){

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

    } 
	else
        Serial.println("failed to mount FS");
    
    Serial.println("DeviceID:");
	Serial.println(deviceid);
    Serial.println("Server:");
	Serial.println(server);
    Serial.println("Port:");
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

void Config::gen_random( char *s, const int len ){
    
	static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for( int i = 0; i < len; ++i )
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    
    s[len] = 0;

}

void Config::save(){

	Serial.println("saving config");
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


Config vhConf = Config();
