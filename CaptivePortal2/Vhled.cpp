#include "Vhled.h"
#include "Configuration.h"
#include <Arduino.h>
#include <vector>
#include <ESP32Ticker.h>


#define RED 0
#define GREEN 1
#define BLUE 2


std::vector<VhPwm> leds;		// Vector containing LEDs


Ticker ledTicker;               // Ticker for LED
bool ledTickerHigh;				// Whether LED ticker is high


// Flash blue on and off
void ledTickerCallback(){
    ledTickerHigh = !ledTickerHigh;
    leds[BLUE].setPWM(round((int)ledTickerHigh*DEFAULT_LED_PWM));
}


Vhled::Vhled() :
    programState(-1)
{}


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
            ledTicker.attach(0.5, ledTickerCallback);
            break;
        case STATE_WIFI_ERR :
            rgb[RED] = 255;
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
        leds[i].setPWM(round(255-(rgb[i]/255.0*DEFAULT_LED_PWM)));
        #else
        leds[i].setPWM(round(rgb[i]/255.0*DEFAULT_LED_PWM));
        #endif
    }
}

Vhled vhled = Vhled();
