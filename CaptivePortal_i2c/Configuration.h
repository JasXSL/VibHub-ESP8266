/*

Firmware configuration stuff

anything board specific or shared between classes

*/
#ifndef VhConfiguration_h
#define VhConfiguration_h

// Software version
	#define VH_VERSION "0.1.0"

// PWM Settings
    // #define PWMFREQ 1600
    // #define PWMI2CADDRESS 0x40
    #define PWMI2CADDRESS 0x00
    #define I2CCLOCK 400000


// PIN RESERVATIONS
    
    #if defined(ESP8266)
        // PWM Driver
        // SDA pin: GPIO 4
        // SCL pin: GPIO 5
        #define PIN_ENABLE 15
        
        // RGB LED pins
    	#define PIN_LED_RED 2
    	#define PIN_LED_GREEN 12
    	#define PIN_LED_BLUE 13
    
    	// // MOTORS
    	// #define PIN_MOTOR_0 4
    	// #define PIN_MOTOR_1 5
    	// #define PIN_MOTOR_2 13
    	// #define PIN_MOTOR_3 12
    
    	// CONF BUTTON
    	#define CONF_PIN 14
        
    #elif defined(ESP32)
    	// // RGB LED
    	// #define PIN_LED_RED 13
    	// #define PIN_LED_GREEN 12
    	// #define PIN_LED_BLUE 14
    
    	// // MOTORS
    	// #define PIN_MOTOR_0 26
    	// #define PIN_MOTOR_1 25
    	// #define PIN_MOTOR_2 33
    	// #define PIN_MOTOR_3 32
    
    	// // CONF BUTTON
    	// #define CONF_PIN 36
    #endif
    



// Defaults
    
    // TWEENDUINO
	#define TIMELINE_SIZE 64		// Max stages of the timeline

	// Brightness of RGB LED (0-255)
	#define DEFAULT_LED_PWM 90
    // Define LED_INVERT true for common anode LED
    #define LED_INVERT true
    
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

	#define CHANNEL_MOTOR_0 0
	#define CHANNEL_MOTOR_1 1
	#define CHANNEL_MOTOR_2 2
	#define CHANNEL_MOTOR_3 3
    
// PMW Config
    #define LEDC_FREQ 5000
    #define LEDC_RES 8


#endif //VhConfiguration_h
