/*
Library requirements:
ArduinoJson from library manager
Adafruit PWM Server Driver Library from library manager (required for the PCA9685)

ESP8266:
arduinoWebSockets from library manager

ESP32 library requirements:
https://github.com/bertmelis/Ticker-esp32
https://github.com/bbx10/arduinoWebSockets/tree/esp32
https://github.com/bbx10/Hash_tng
https://github.com/bbx10/webserver_tng
DNS patch or ESP32 platform from: https://github.com/tzapu/WiFiManager/issues/513

*/

// Libraries
    #include <Arduino.h>
    #include "Configuration.h"
	#include "Config.h"
	#include "VhPwm.h"
    #include "Vhled.h"
	#include "VhButton.h"
	#include "VhWifi.h"
	#include "VhClient.h"
	#include "VhMotor.h"
    
    #include <ESP8266mDNS.h>
    #include <ArduinoOTA.h>

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(500);
    Serial.println("\nStarting...");
    
    pwm.begin();
    randomSeed(analogRead(A0));          // Picks a random number
    Serial.printf("Analog read %f \n", analogRead(A0));
    
    // Set LED state
    vhled.setup();
    vhled.setState(STATE_BOOT);
    
    button.setup();
    client.setup();
    
    
    // Reset config and wifi if config button is held on boot
    bool reset = false;
    if( button.isPressed() ){
        Serial.println("Resetting everything");
        reset = true;
    }
    
    vhConf.load(reset);
    Serial.println("Connecting to wifi");
    vhWifi.connect(reset, reset);
    Serial.println("Connected");
    
    // Set socket loading state
    vhled.setState(STATE_SOCKET_ERR);
    
    // motorCtrl.begin();
    
    //Connect to server
    client.connect();
    
    
    //TODO: dedicated programming mode
    
    Serial.println("OTA: Setting up");
    ArduinoOTA.setPort(8266);
    ArduinoOTA.setHostname("vh8266");
    ArduinoOTA.setPassword("vhota");
    
    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("OTA: Ready");
}

void loop() {
    client.loop();
    button.loop();
    ArduinoOTA.handle();
    
    #ifdef ESP8266
        yield();
    #endif
}
