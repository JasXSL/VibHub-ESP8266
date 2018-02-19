#include "Vhled.h"
#include "Configuration.h"
#include <Arduino.h>
#include <vector>
#if defined(ESP8266)
#include <Ticker.h>
#elif defined(ESP32)
#include <ESP32Ticker.h>
#endif

Ticker ledTicker;               // Ticker for LED

Vhled::Vhled() :
    programState(-1)
{}

#if defined(ESP8266)

#define RED     0x1
#define GREEN   0x2
#define BLUE    0x4

#define YELLOW  0x3
#define PURPLE  0x5
#define CYAN    0x6

#define OFF     0x0
#define WHITE   0x7


#if defined(LED_INVERT)
    #define setLedPin(pin, on) digitalWrite(pin, !(on));
#else
    #define setLedPin(pin, on) digitalWrite(pin, on);
#endif

void setLed(int color){
    setLedPin(PIN_LED_RED,  color&RED);
    setLedPin(PIN_LED_GREEN,color&GREEN);
    setLedPin(PIN_LED_BLUE, color&BLUE);
}


// Flash led on and off
bool ledTickerHigh;				// Whether LED ticker is high
void flashingTick(int color){
    ledTickerHigh = !ledTickerHigh;
    setLed(color*ledTickerHigh);
}


void Vhled::setup(){
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    
    setLed(OFF);
}

void Vhled::setState( int state ){
    if (programState == state) return;
    programState = state;
    
    Serial.print("setState: ");
    Serial.print(state);
    
    ledTicker.detach();
    
    switch(state) {
        case STATE_INIT :
            setLed(BLUE);
            Serial.println(" - STATE_INIT blue");
            break;
        case STATE_PORTAL :
            setLed(BLUE);
            ledTickerHigh = true;
            ledTicker.attach_ms(15, flashingTick, BLUE);
            Serial.println(" - STATE_PORTAL blue blink");
            break;
        case STATE_WIFI_ERR :
            setLed(RED);
            ledTickerHigh = true;
            ledTicker.attach_ms(500, flashingTick, RED);
            Serial.println(" - STATE_WIFI_ERR red blink");
            break;
        case STATE_SOCKET_ERR :
            setLed(YELLOW);
            Serial.println(" - STATE_SOCKET_ERR half green");
            break;
        case STATE_RUNNING :
            setLed(GREEN);
            Serial.println(" - STATE_RUNNING green");
            break;
        default : // STATE_BOOT
            setLed(CYAN);
            Serial.println(" - STATE_BOOT half blue");
    }
}

#elif defined(ESP32)

#define RED 0
#define GREEN 1
#define BLUE 2


std::vector<VhPwm> leds;		// Vector containing LEDs


// Flash led on and off
bool ledTickerHigh;				// Whether LED ticker is high
void flashingTick(int led){
    ledTickerHigh = !ledTickerHigh;
    #if defined(LED_INVERT)
    leds[led].setPWM(255-round((int)ledTickerHigh*DEFAULT_LED_PWM));
    #else
    leds[led].setPWM(round((int)ledTickerHigh*DEFAULT_LED_PWM));
    #endif
}

// Fade led on and off
int pulseBrightness = 255;
int pulseAmount = -5;
void pulseTick(int led){
    #if defined(LED_INVERT)
    leds[led].setPWM(255-round(pulseBrightness/255.0*DEFAULT_LED_PWM));
    #else
    leds[led].setPWM(round(pulseBrightness/255.0*DEFAULT_LED_PWM));
    #endif
    pulseBrightness = pulseBrightness + pulseAmount;
    if (pulseBrightness <= 0 || pulseBrightness >= 255) {
        pulseAmount = -pulseAmount;
    }
}


void Vhled::setup(){
    leds.push_back(VhPwm(PIN_LED_RED, CHANNEL_RED));
    leds.push_back(VhPwm(PIN_LED_GREEN, CHANNEL_GREEN));
    leds.push_back(VhPwm(PIN_LED_BLUE, CHANNEL_BLUE));
}

void Vhled::setState( int state ){
    if (programState == state) return;
    programState = state;
    
    Serial.print("setState: ");
    Serial.println(state);
    
    ledTicker.detach();
    
    int rgb[3] = {0,0,0};
    switch(state) {
        case STATE_INIT :
            rgb[BLUE] = 255;
            break;
        case STATE_PORTAL :
            rgb[BLUE] = 255;
            pulseBrightness = 255;
            ledTicker.attach_ms(15, pulseTick, BLUE);
            break;
        case STATE_WIFI_ERR :
            rgb[RED] = 255;
            ledTicker.attach_ms(500, flashingTick, RED);
            break;
        case STATE_SOCKET_ERR :
            rgb[RED] = rgb[GREEN] = 128;
            break;
        case STATE_RUNNING :
            rgb[GREEN] = 255;
            break;
        default : // STATE_BOOT
            rgb[BLUE] = 50;
    }
    
    for( int i = 0; i < 3; ++i ){
        #if defined(LED_INVERT)
        leds[i].setPWM(255-round(rgb[i]/255.0*DEFAULT_LED_PWM));
        #else
        leds[i].setPWM(round(rgb[i]/255.0*DEFAULT_LED_PWM));
        #endif
    }
}

#endif

Vhled vhled = Vhled();
