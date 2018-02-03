/*

Firmware configuration stuff

anything board specific or shared between classes

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



// Defaults
	// Brightness of RGB LED (0-255)
	#define DEFAULT_LED_PWM 50
    #define LED_INVERT true
    
    #define SERVER_DOMAIN  "vibhub.io"
    #define SERVER_PORT    80

    #define DEBOUNCE_MS 50			// Prevents button bounce
    

// PWM Channel reservation
	#define CHANNEL_RED 0
	#define CHANNEL_GREEN 1
	#define CHANNEL_BLUE 2

	#define CHANNEL_MOTOR_0 3
	#define CHANNEL_MOTOR_1 4
	#define CHANNEL_MOTOR_2 5
	#define CHANNEL_MOTOR_3 6

