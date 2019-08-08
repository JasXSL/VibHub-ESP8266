/*
	VibHub Button
*/
#ifndef VhButton_h
#define VhButton_h

#include <Arduino.h>
#include <Ticker.h>

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
