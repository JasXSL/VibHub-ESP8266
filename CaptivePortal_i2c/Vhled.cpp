#include "Vhled.h"
#include "Configuration.h"
#include <Arduino.h>
#include <Ticker.h>


Ticker ledTicker;               // Ticker for LED

Vhled::Vhled() :
    programState(-1)
{}

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

uint8_t  testColor = 0;
void testTick(int color){
    setLed(OFF);
    
    switch(testColor) {
        case 0 : setLed(RED); break;
        case 1 : setLed(GREEN); break;
        case 2 : setLed(BLUE); break;
        case 3 : setLed(YELLOW); break;
        case 4 : setLed(PURPLE); break;
        case 5 : setLed(CYAN); break;
        case 6 : setLed(OFF); break;
        case 7 : setLed(WHITE); break;
        default : setLed(GREEN);
    }
    
    testColor++;
    if (testColor >= 9){
        ledTicker.detach();
    }
}

void Vhled::test(){
    testColor = 0;
    ledTicker.attach(2, testTick, 0);
}


Vhled vhled = Vhled();
