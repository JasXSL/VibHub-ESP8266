/*
	VibHub Button
*/
#ifndef VhButton_h
#define VhButton_h

#include <Arduino.h>
#if defined(ESP8266)
#include <Ticker.h>
#elif defined(ESP32)
#include <ESP32Ticker.h>
#endif
#include "Configuration.h"


class VhButton {
    public:
        VhButton(void);
        
        void setup();
        // void interrupt();
        bool loop( bool isReconfiguring=false );
        
        bool isPressed() {return digitalRead(CONF_PIN) == BUTTON_DOWN;}
        
    private:
        
};

extern VhButton button;

#endif //VhButton_h
