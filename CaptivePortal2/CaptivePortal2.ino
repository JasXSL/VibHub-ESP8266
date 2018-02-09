/*
Library requirements:


ESP32 library requirements:
Comment out SocketIoClient.cpp#L41, hexdump() is not available on ESP32
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
	#include "VhButton.h"
	#include "VhWifi.h"
	#include "VhClient.h"
	#include "Motor.h"


void setup() {
    // Set LED state
    vhled.setup();
    vhled.setState(STATE_BOOT);
    
    
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    delay(500);
    Serial.println("\nStarting...");
	
    
    button.setup();
    client.setup();
    
    
    // Reset config and wifi if config button is held on boot
    bool reset = false;
    if( button.isPressed() ){
        Serial.println("Resetting everything");
        reset = true;
    }
    
    vhConf.load(reset);
    
    vhWifi.connect(reset, reset);
    Serial.println("Connected");
    
    // Set socket loading state
    vhled.setState(STATE_SOCKET_ERR);
    
    // motorCtrl.begin();
    
    //Connect to server
    client.connect();
}

void loop() {
    client.loop();
    button.loop();
}
