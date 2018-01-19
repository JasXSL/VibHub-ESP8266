#include "FS.h"
#include "SPIFFS.h"
#include "Config.h"

#define ConfigPATH "/config.txt"


// Loads the config
Config::Config(){}


void Config::begin(){


    host = "vibhub.dangly.parts";
    port = 80;
    
    
	if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
	char * path = ConfigPATH;
    FS fs = SPIFFS;
    
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("No config found on device");
        return;
    }

    Serial.print("Read from file: ");
    int i = 0;
    
    String row = "";
    while(file.available()){

        char c = char(file.read());
    
        if( c == '\n' ){

          if( i == 0 )
            ssid = row;
          else if( i == 1 )
            password = row;
          else if( i == 2 )
            device_id = row;
          else if( i == 3 )
            host = row;
          else if( i == 4 )
            port = row.toInt();

          row = "";          
          ++i;
        }
        else
          row+=(String)c;
      
    }
    
    Serial.println("SSID: "+(String)ssid);
    Serial.println("Password: "+(String)password);
    Serial.println("DeviceID: "+(String)device_id);
    Serial.println("Host: "+(String)host);
    Serial.println("Port: "+(String)port);
    

}

void Config::save(){

	char * path = ConfigPATH;
    FS fs = SPIFFS;
    
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    file.flush();
    
    file.println(ssid);
    file.println(password);
    file.println(device_id);
    file.println("");

}


