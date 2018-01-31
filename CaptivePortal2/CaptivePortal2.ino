/*
Library requirements:
https://github.com/timum-viw/socket.io-client
Install Adafruit Motor Shield V2 from library manager

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

// Default Config:

#define SERVER_DOMAIN  "vibhub.io"
#define SERVER_PORT    80
// #define SERVER_DOMAIN  "192.168.0.104"
// #define SERVER_PORT    6969


// IO Config

//Don't use the onboard flash button on gpio0 as this can put the chip different bootmode by accident
//This could be used for other functions after setup as part of a single button interface
#if defined(ESP8266)
#define WIFIRESET_PIN 14
#elif defined(ESP32)
//ESP32 DevKit
#define WIFIRESET_PIN 36
#endif

//for LED status
//Set to which ever pins the LEDs are connected to, BUILTIN_LED is incorrect for ESP8266 and should 2 not 16
#if defined(ESP8266)
  #define RED_LED 5
  #define GREEN_LED 4
  #define BLUE_LED 2
#elif defined(ESP32)
  //Sparkfun's ESP32 Thing BUILTIN_LED is on GPIO5 and is driven high
  //ESP32 devboard's BUILTIN_LED might be on GPIO25
  // #define RED_LED 5
  // #define GREEN_LED 4
  // #define BLUE_LED 2
  //ESP32 DevKit
  #define RED_LED 13
  #define GREEN_LED 12
  #define BLUE_LED 14
#endif

// PWM Channels
#define CHANNEL_RED 0
#define CHANNEL_GREEN 1
#define CHANNEL_BLUE 2
#define LED_PWM 150


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

#include "./Motor.h"

// ledC
#define LEDC_FREQ 12000
#define LEDC_RES 8

bool indicator = false;

Ticker ticker;
SocketIoClient webSocket;
Motor motorCtrl;


void event_connect(const char * payload, size_t length) {
  Serial.println("got connect");
  webSocket.emit("id", "\"ESP8266\"");
}

void event_disconnect(const char * payload, size_t length) {
  Serial.println("got disconnect");
}

void event_vib(const char * payload, size_t length) {
  Serial.printf("got vib: %s\n", payload);
}

void event_p(const char * payload, size_t length) {
  unsigned long int data = strtoul(payload, 0, 16);
  uint8_t vibArray[4];
  vibArray[0] = (int)((data & 0xFF000000) >> 24 );
  vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
  vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
  vibArray[3] = (int)((data & 0X000000FF));
  Serial.printf("got p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);
  motorCtrl.setAll(vibArray);
  motorCtrl.runAll(FORWARD);
}


void tick() {
    //toggle LED indicator
    indicator = (int)(!indicator);
    ledcWrite(CHANNEL_BLUE, !(indicator*LED_PWM));
    ledcWrite(CHANNEL_RED, indicator*LED_PWM);
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
    //Serial.setDebugOutput(true);
    
    delay(500);
    Serial.println("\nStarting...");

    //set wifireset pin as input
    pinMode(WIFIRESET_PIN, INPUT);
    
    //set led pin as output
    ledcSetup(CHANNEL_RED, LEDC_FREQ, LEDC_RES);
    ledcSetup(CHANNEL_GREEN, LEDC_FREQ, LEDC_RES);
    ledcSetup(CHANNEL_BLUE, LEDC_FREQ, LEDC_RES);

    // Attach pins
    ledcAttachPin(RED_LED, CHANNEL_RED);
    ledcAttachPin(GREEN_LED, CHANNEL_GREEN);
    ledcAttachPin(BLUE_LED, CHANNEL_BLUE);

    ledcWrite(CHANNEL_RED, LED_PWM);
    
    // start ticker with 0.5 because we start in AP mode and try to connect
    ticker.attach(0.6, tick);
    
    
    
    motorCtrl.begin();
    
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
     String ssid = "VibHub_" + String(ESP.getChipId());
    #else
      uint64_t chipid = ESP.getEfuseMac();
      String ssid = "VibHub_" + String((uint16_t)(chipid>>32));
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
    ledcWrite(CHANNEL_RED, LED_PWM);
    ledcWrite(CHANNEL_GREEN, 0);
    ledcWrite(CHANNEL_BLUE, LED_PWM);
    
    //Do socket.io stuff
    webSocket.on("connect", event_connect);
    webSocket.on("disconnect", event_disconnect);
    webSocket.on("vib", event_vib);
    webSocket.on("p", event_p);
    
    webSocket.begin(SERVER_DOMAIN, SERVER_PORT);
}

void loop() {
    webSocket.loop();
}
