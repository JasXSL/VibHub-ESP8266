#if defined(ESP8266)
#include <ESP8266WiFi.h>   //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>          //https://github.com/esp8266/Arduino
#endif

//needed for library
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>   //https://github.com/tzapu/WiFiManager

#include "src/Config/Config.h"
#include "src/Connection/Connection.h"

Config conf;
Connection con;

//Don't use the onboard flash button on gpio0 as this can put the chip different bootmode by accident
//This could be used for other functions after setup as part of a single button interface
#define WIFIRESET_PIN 14

//for LED status
//Set to which ever pins the LEDs are connected to, BUILTIN_LED is incorrect for ESP8266 and should 2 not 16
#define RED_LED 5
#define GREEN_LED 4
#define BLUE_LED 2
#include <Ticker.h>
Ticker ticker;



// Shared in the header file
void onSocketMessage( String type, String message ){

  Serial.println("Got message - type: '" + type + "' message: '" + message + "'");
  
  if( type == "pw" ){

      char r[message.length()+1];
      message.toCharArray(r, message.length()+1);

      char *ptr = strtok(r, "|");
      while( ptr != NULL ){

        char *sub = strtok(ptr, ",");
        int params[2];
        int i = 0;
        while( sub != NULL ){

          String s(sub);
          int n = s.toInt();
          params[i++] = s.toInt();

          sub = strtok(NULL, ",");
        }

        Serial.println("Update motors here");
        //updateMotor(params[0], params[1]);
        ptr = strtok(NULL, "|");

      }

    }
  
}



void tick() {
    //toggle LED indicator
    int state = digitalRead(BLUE_LED);
    digitalWrite(BLUE_LED, !state);
    digitalWrite(RED_LED, state);
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    //if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    //entered config mode, make led toggle faster
    ticker.attach(0.2, tick);
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    delay(500);
    Serial.println("\nStarting...");

    //set wifireset pin as input
    pinMode(WIFIRESET_PIN, INPUT);
    
    //set led pin as output
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    digitalWrite(GREEN_LED, HIGH);
    // start ticker with 0.5 because we start in AP mode and try to connect
    ticker.attach(0.6, tick);

    
    Serial.println("Setting up config");
    
    #if defined(ESP8266)
      itoa(ESP.getChipId(), conf.device_id, sizeof(conf.device_id));
    #elif defined(ESP32)
      uint64_t chipid = ESP.getEfuseMac();
      itoa((uint16_t)(chipid>>32), conf.device_id, sizeof(conf.device_id));
    #endif
    
    
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset settings - for testing
    //wifiManager.resetSettings();

    //Reset everything when button is held down while turing on
    if(digitalRead(WIFIRESET_PIN) == LOW) {
        Serial.println("Resetting wifi settings");
        wifiManager.resetSettings();
        
        conf.format();
    }

    conf.begin();

    //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
    wifiManager.setAPCallback(configModeCallback);
    
    //fetches ssid and pass and tries to connect
    //if it does not connect it starts an access point
    //and goes into a blocking loop awaiting configuration
    #if defined(ESP8266)
     String ssid = "VibHud_" + String(ESP.getChipId());
    #else
      uint64_t chipid = ESP.getEfuseMac();
      String ssid = "VibHud_" + String((uint16_t)(chipid>>32));
    #endif

  if (!wifiManager.autoConnect(ssid.c_str())) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(1000);
  }

    
    //if you get here you have connected to the WiFi
    Serial.println("Connected");

    ticker.detach();
    //keep LED on
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    
    // initialize wifi and sockets
    con.ini(conf);

    //TODO: this should be somewhere better, here for testing
    //Save config
    conf.save();
}

void loop() {
    con.loop();
}
