/*
Library requirements:
https://github.com/timum-viw/socket.io-client

ESP8266 library requirements:
https://github.com/Links2004/arduinoWebSockets/
https://github.com/tzapu/WiFiManager/

ESP32 library requirements:
Comment out SocketIoClient.cpp#L41, hexdump() is not available on ESP32
https://github.com/tzapu/WiFiManager/tree/esp32 (In high flux as of 2018-01-28, hopefully will be RC soon)
https://github.com/bertmelis/Ticker-esp32
https://github.com/bbx10/arduinoWebSockets/tree/esp32
https://github.com/bbx10/Hash_tng
https://github.com/bbx10/webserver_tng


*/
#include <Arduino.h>

#if defined(ESP8266)
#include <ESP8266WebServer.h>
#include <Ticker.h>
#elif defined(ESP32)
#include <WebServer.h>      //https://github.com/bbx10/webserver_tng
#include <ESP32Ticker.h>    //https://github.com/bertmelis/Ticker-esp32
#endif

#include <DNSServer.h>
#include <WiFiManager.h>    //https://github.com/tzapu/WiFiManager

#include <SocketIoClient.h>

#define USE_SERIAL Serial

//Don't use the onboard flash button on gpio0 as this can put the chip different bootmode by accident
//This could be used for other functions after setup as part of a single button interface
#define WIFIRESET_PIN 14

//for LED status
//Set to which ever pins the LEDs are connected to, BUILTIN_LED is incorrect for ESP8266 and should 2 not 16
#if defined(ESP8266)
  #define RED_LED 5
  #define GREEN_LED 4
  #define BLUE_LED 2
#elif defined(ESP32)
  //Sparkfun's ESP32 Thing BUILTIN_LED is on GPIO5 and is driven high
  //ESP32 devboard's BUILTIN_LED might be on GPIO25
  #define RED_LED 5
  #define GREEN_LED 4
  #define BLUE_LED 2
#endif

Ticker ticker;

SocketIoClient webSocket;


void event_connect(const char * payload, size_t length) {
  USE_SERIAL.println("got connect");
  webSocket.emit("id", "\"ESP8266\"");
}

void event_disconnect(const char * payload, size_t length) {
  USE_SERIAL.println("got disconnect");
}

void event_vib(const char * payload, size_t length) {
  USE_SERIAL.printf("got vib: %s\n", payload);
}

void event_p(const char * payload, size_t length) {
  unsigned long int data = strtoul(payload, 0, 16);
  unsigned char vibArray[4];
  vibArray[0] = (int)((data & 0xFF000000) >> 24 );
  vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
  vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
  vibArray[3] = (int)((data & 0X000000FF));
  USE_SERIAL.printf("got p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);
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
    Serial.begin(115200);
    //USE_SERIAL.setDebugOutput(true);
    
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
    
    
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset settings - for testing
    //wifiManager.resetSettings();

    //Reset everything when button is held down while turing on
    if(digitalRead(WIFIRESET_PIN) == LOW) {
        Serial.println("Resetting wifi settings");
        wifiManager.resetSettings();
    }


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
    
    //Do socket.io stuff
    webSocket.on("connect", event_connect);
    webSocket.on("disconnect", event_disconnect);
    webSocket.on("vib", event_vib);
    webSocket.on("p", event_p);
    
    webSocket.begin("vibhub.io", 80);
    // webSocket.begin("192.168.0.104", 6969);
}

void loop() {
    webSocket.loop();
}
