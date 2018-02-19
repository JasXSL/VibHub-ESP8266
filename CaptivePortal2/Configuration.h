/*

Firmware configuration stuff

anything board specific or shared between classes

*/
#ifndef VhConfiguration_h
#define VhConfiguration_h

// PIN RESERVATIONS
    
    #if defined(ESP8266)
        // RGB LED
    	#define PIN_LED_RED 15
    	#define PIN_LED_GREEN 2
    	#define PIN_LED_BLUE 0
    
    	// MOTORS
    	#define PIN_MOTOR_0 4
    	#define PIN_MOTOR_1 5
    	#define PIN_MOTOR_2 12
    	#define PIN_MOTOR_3 13
    
    	// CONF BUTTON
    	#define CONF_PIN 14
    #elif defined(ESP32)
    	// RGB LED
    	#define PIN_LED_RED 13
    	#define PIN_LED_GREEN 12
    	#define PIN_LED_BLUE 14
    
    	// MOTORS
    	#define PIN_MOTOR_0 26
    	#define PIN_MOTOR_1 25
    	#define PIN_MOTOR_2 33
    	#define PIN_MOTOR_3 32
    
    	// CONF BUTTON
    	#define CONF_PIN 36
    #endif
    



// Defaults
    
    // TWEENDUINO
	#define TIMELINE_SIZE 64		// Max stages of the timeline

	// Brightness of RGB LED (0-255)
	#define DEFAULT_LED_PWM 90
    // Define LED_INVERT for common anode LED
    #define LED_INVERT
    
    #define DEFAULT_SERVER  "vibhub.io"
    #define DEFAULT_PORT    80
    
    #define WEBSOCKET_RECONNECT_TIME    1000

    #define DEBOUNCE    30          // Prevents button bounce
    #define HOLD_TIME   3000        // ms hold period
    #define BUTTON_UP   HIGH
    #define BUTTON_DOWN LOW

// PWM Channel reservation
	#define CHANNEL_RED 0
	#define CHANNEL_GREEN 1
	#define CHANNEL_BLUE 2

	#define CHANNEL_MOTOR_0 3
	#define CHANNEL_MOTOR_1 4
	#define CHANNEL_MOTOR_2 5
	#define CHANNEL_MOTOR_3 6
    
// PMW Config
    #define LEDC_FREQ 5000
    #define LEDC_RES 8


#endif //VhConfiguration_h
