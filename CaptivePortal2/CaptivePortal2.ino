/*
Library requirements:
https://github.com/timum-viw/socket.io-client
// Install Adafruit Motor Shield V2 from library manager

// ESP8266 library requirements:
// https://github.com/Links2004/arduinoWebSockets/
// https://github.com/tzapu/WiFiManager/

ESP32 library requirements:
Comment out SocketIoClient.cpp#L41, hexdump() is not available on ESP32
https://github.com/tzapu/WiFiManager/tree/esp32 (In high flux as of 2018-01-28, hopefully will be RC soon)
https://github.com/bertmelis/Ticker-esp32
https://github.com/bbx10/arduinoWebSockets/tree/esp32
https://github.com/bbx10/Hash_tng
https://github.com/bbx10/webserver_tng

*/



// Libraries
    #include <Arduino.h>
    #include "Configuration.h"
	#include "Config.h"
	#include "VhPwm.h"
	#include "Vhled.h"
	#include "VhWifi.h"
	#include "VhSocket.h"
	#include "Motor.h"


// Globals
    // Socket.io client
    VhSocket socket;
    
// Local state vars
    int lastButtonState = BUTTON_UP;
    long buttonDownTime;
    bool buttonHeld = false;



// Socket.io events

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

    
    

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    
    delay(500);
    Serial.println("\nStarting...");
	
    
    // Set LED state
    vhled.setup();
    vhled.setState(STATE_BOOT);
    
    
    
    bool reset = false;

    //set wifireset pin as input
    pinMode(CONF_PIN, INPUT);
    
    
    // Reset config and wifi if config button is held on boot
    if( digitalRead(CONF_PIN) == BUTTON_DOWN ){
        Serial.println("Resetting everything");
        reset = true;
    }
    
    config.load(reset);
    
    
    vhWifi.connect(reset, reset);
    //if you get here you have connected to the WiFi
    Serial.println("Connected");
    
    // Set socket loading state
    vhled.setState(STATE_SOCKET_ERR);
    
    motorCtrl.begin();
    
    //TODO: Possibly connect directly to motor controller
    socket.on("vib", event_vib);
    socket.on("p", event_p);
    
    socket.connect();
}

void loop() {
    socket.loop();
    
    // Check the button
    int buttonState = digitalRead(CONF_PIN);
    
    if (buttonState == BUTTON_DOWN){
        // Pressed
        if (lastButtonState == BUTTON_UP){
            buttonDownTime = millis();
        }
        // Held
        else if (!buttonHeld && (millis() - buttonDownTime) > long(HOLD_TIME)){
            Serial.println("== Button is being held");
            // Give visual indication that button has been held long enough
            vhled.setState(STATE_INIT);
            buttonHeld = true;
        }
    }
    // Released
    else if (buttonState == BUTTON_UP && lastButtonState == BUTTON_DOWN){
        if (buttonHeld){
            Serial.println("== Button Held >5s");
            buttonHeld = false;
            //TODO
            vhled.setState(STATE_PORTAL);
            //vhWifi.connect(true, false);
        }
        else if ((millis() - buttonDownTime) > long(DEBOUNCE)){
            Serial.println("== Button Pressed");
            //TODO: cancel config mode
            vhled.setState(STATE_RUNNING);
        }
    }
    lastButtonState = buttonState;
}
