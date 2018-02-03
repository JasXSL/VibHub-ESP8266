/*
Library requirements:
https://github.com/timum-viw/socket.io-client
Install Adafruit Motor Shield V2 from library manager

ESP32 library requirements:
Comment out SocketIoClient.cpp#L41, hexdump() is not available on ESP32
https://github.com/tzapu/WiFiManager/tree/esp32 (In high flux as of 2018-01-28, hopefully will be RC soon)
https://github.com/bertmelis/Ticker-esp32
https://github.com/bbx10/arduinoWebSockets/tree/esp32
https://github.com/bbx10/Hash_tng
https://github.com/bbx10/webserver_tng

*/

// PIN RESERVATIONS

	// RGB LED
	#define PIN_LED_RED 13
	#define PIN_LED_GREEN 12
	#define PIN_LED_BLUE 14

	// MOTORS
	#define PIN_MOTOR_0 25
	#define PIN_MOTOR_1 33
	#define PIN_MOTOR_2 32
	#define PIN_MOTOR_3 35

	// CONF BUTTON
	#define CONF_PIN 36

// PWM Channel reservation
	#define CHANNEL_RED 0
	#define CHANNEL_GREEN 1
	#define CHANNEL_BLUE 2

	#define CHANNEL_MOTOR_0 3
	#define CHANNEL_MOTOR_1 4
	#define CHANNEL_MOTOR_2 5
	#define CHANNEL_MOTOR_3 6


// Defaults
	// Brightness of RGB LED (0-255)
	#define DEFAULT_LED_PWM 20


// Libraries
	#include "Config.h"
	#include <WebServer.h>      //https://github.com/bbx10/webserver_tng
	#include <ESP32Ticker.h>    //https://github.com/bertmelis/Ticker-esp32
	#include <DNSServer.h>
	#include <WiFiManager.h>    //https://github.com/tzapu/WiFiManager
	#include "driver/ledc.h"
	#include <SocketIoClient.h>
	#include "Motor.h"
	#include "VhPwm.h"
	#include <Arduino.h>


// move these later
#define getssid() "VibHub_"+String((uint16_t)(ESP.getEfuseMac()>>32))


// Globals
	std::vector<Motor> motors;		// Vector containing motors to be PWMed
	std::vector<VhPwm> leds;		// Vector containing LEDs

	bool confSaveSettings;			// Whether to save settings on disconnect from captive or not

	Ticker ledTicker;				// Ticker for LED
	bool ledTickerHigh;				// Whether LED ticker is high
	
	int programState = 0;			// State the program is currently in
		#define STATE_BOOT 0			// Program is booting	<0,0,128>
		#define STATE_PORTAL 1			// Program is in captive portal mode	<0,0,255> blinking
		#define STATE_WIFI_ERR 2		// Unable to connect to wifi	<255,0,0>
		#define STATE_SOCKET_ERR 3		// Unable to connect to socket server 	<128,128,0>
		#define STATE_RUNNING 4			// Connected and listening	<0,255,0>

	SocketIoClient webSocket;		// SocketIO client


// Program state changes
	void setState( int state = STATE_BOOT ){

		int rgb[3] = {0};

		ledTicker.detach();

		if( state == 1 ){
			
			rgb[2] = 255;
			ledTicker.attach(0.5, ledTickerCallback);

		}
		else if( state == 2 )
			rgb[0] = 255;
		else if( state == 3 )
			rgb[0] = rgb[1] = 128;
		else if( state == 4 )
			rgb[1] = 255;
		else
			rgb[2] = 50;


		int i;
		for( i = 0; i < 3; ++i )
			leds[i].setPWM(round(rgb[i]/255.0*DEFAULT_LED_PWM));
		
	}


// Socket.io events

	void event_connect(const char * payload, size_t length) {
		Serial.println("got connect");
		webSocket.emit("id", ("\"" + (String)vhConf.deviceid + "\"").c_str());
		setState(STATE_RUNNING);
	}

	void event_disconnect(const char * payload, size_t length) {
		Serial.println("got disconnect");
		setState(STATE_SOCKET_ERR);
	}

	void event_vib(const char * payload, size_t length) {
		Serial.printf("got vib: %s\n", payload);
	}

	// Received a quick set of all 4 motors PWM values
	void event_p( const char * payload, size_t length ){

		unsigned long int data = strtoul(payload, 0, 16);
		uint8_t vibArray[4];
		vibArray[0] = (int)((data & 0xFF000000) >> 24 );
		vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
		vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
		vibArray[3] = (int)((data & 0X000000FF));
		Serial.printf("got p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);

		int i;
		for( i = 0; i < 4; ++i )
			motors[i].setPWM(vibArray[i]);


	}


// Wifi events
	//gets called when WiFiManager enters configuration mode
	void configModeCallback( WiFiManager *myWiFiManager ){

		// Set portal LED state
		setState(STATE_PORTAL);

		Serial.println("Entered config mode");
		Serial.println(WiFi.softAPIP());
		//if you used auto generated SSID, print it
		Serial.println(myWiFiManager->getConfigPortalSSID());

	}

	// Config mode exited
	void configModeSaveCallback(){
		
		confSaveSettings = true;

	}

	// Initializes wifi and/or captive portal
	void initWifi( bool force ){
	
		WiFiManager wifiManager;
		// Setup custom CSS
		//wifiManager.setCustomHeadElement("<style>body{background-color:#000;color:#FFF;}</style>");

		//set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
		wifiManager.setAPCallback(configModeCallback);
		wifiManager.setSaveConfigCallback(configModeSaveCallback);

		// Custom settings
		WiFiManagerParameter devId("Device ID", "Your device ID", vhConf.deviceid, 64);
		WiFiManagerParameter serverHost("Server Host", "vibhub.io", vhConf.server, 64);
		char port[6];
		itoa(vhConf.port, port, 10);
		WiFiManagerParameter serverPort("Server Port", "80", port, 6);

		wifiManager.addParameter(&devId);
		wifiManager.addParameter(&serverHost);
		wifiManager.addParameter(&serverPort);
		
		String ssid = getssid();
		if( force ){

			if( !wifiManager.startConfigPortal(ssid.c_str()) ){

				setState( STATE_WIFI_ERR );
				Serial.println("failed to connect and hit timeout");
				//reset and try again, or maybe put it to deep sleep
				ESP.restart();
				delay(1000);
			}

		}
		else if( !wifiManager.autoConnect(ssid.c_str()) ){

			setState( STATE_WIFI_ERR );
			Serial.println("failed to connect and hit timeout");
			//reset and try again, or maybe put it to deep sleep
			ESP.restart();
			delay(1000);

		}

		if( confSaveSettings ){

			Serial.println("Configuration change detected, saving and rebootski");

			strcpy(vhConf.deviceid, devId.getValue());
			strcpy(vhConf.server, serverHost.getValue());
			char p[5];
			strcpy(p, serverPort.getValue());
			vhConf.port = atoi(p);
			
			vhConf.save();

			ESP.restart();
			delay(1000);

		}
		else
			Serial.println("No device ID change detected");

	}



// Ticker callbacks
	// Flash blue on and off
	void ledTickerCallback(){

		ledTickerHigh = !ledTickerHigh;
		leds[2].setPWM(round((int)ledTickerHigh*DEFAULT_LED_PWM));

	}


// MAIN PROGRAM HERE

	void setup() {
		
		Serial.begin(115200);
		delay(500);
		Serial.println("\nStarting...");

		//set wifireset pin as input
		pinMode(CONF_PIN, INPUT);

		int r = digitalRead(CONF_PIN);
		Serial.println(r);
		if( r == HIGH ){

			Serial.println("Resetting to factory default");
			WiFiManager wifiManager;
			wifiManager.resetSettings();

		}

		// Begin configuration and tell it if it should reset or not
		vhConf.begin( r == HIGH );

		leds.push_back(VhPwm(PIN_LED_RED, CHANNEL_RED));
		leds.push_back(VhPwm(PIN_LED_GREEN, CHANNEL_GREEN));
		leds.push_back(VhPwm(PIN_LED_BLUE, CHANNEL_BLUE));

		// Initialize motors
		motors.push_back(Motor(PIN_MOTOR_0, CHANNEL_MOTOR_0));
		motors.push_back(Motor(PIN_MOTOR_1, CHANNEL_MOTOR_1));
		motors.push_back(Motor(PIN_MOTOR_2, CHANNEL_MOTOR_2));
		motors.push_back(Motor(PIN_MOTOR_3, CHANNEL_MOTOR_3));
		
		// Set LED state
		setState(STATE_BOOT);

		// Initialize wifi
		initWifi(false);

		//if you get here you have connected to the WiFi
		Serial.println("Connected");

		// Set socket loading state
		setState(STATE_SOCKET_ERR);
		
		//Do socket.io stuff
		webSocket.on("connect", event_connect);
		webSocket.on("disconnect", event_disconnect);
		webSocket.on("vib", event_vib);
		webSocket.on("p", event_p);
		
		webSocket.begin(vhConf.server, vhConf.port);
	}

	void loop() {
		webSocket.loop();

		// Check the button
		int r = digitalRead(CONF_PIN);
		// Enter config mode
		if( r == HIGH )
			initWifi(true);
		


	}
