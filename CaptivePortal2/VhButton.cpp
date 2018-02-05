/*
	VibHub Button
    
    Handles single button interface
*/

#include <functional>
#include "VhButton.h"
#include "Vhled.h"
#include "Config.h"
#include "VhWifi.h"
#include "VhClient.h"

unsigned long _heldTime;
int _lastButtonState;
bool _doReconfigure;
bool _isReconfiguring;
bool _buttonHeld;

VhButton::VhButton(void){
    _heldTime = 0;
    _lastButtonState = BUTTON_UP;
    _doReconfigure = false;
    _isReconfiguring = false;
    _buttonHeld = false;
}


void VhButton::setup(){
    //set wifireset pin as input
    pinMode(CONF_PIN, INPUT);
}

bool VhButton::loop( bool isReconfiguring ) {
    
    if (_doReconfigure){
        _doReconfigure = false;
        Serial.println("VhButton: doReconfigure");
        
        // Disconnect from server
        client.disconnect();
        
        // Start wifi portal
        vhWifi.connect(true, false);
        
        Serial.println("VhButton: Finished manual reconfig");
        // _isReconfiguring = false;
        
        return false;
    }
    
    int buttonState = digitalRead(CONF_PIN);
    
    // Pressed
    if (buttonState == BUTTON_DOWN && _lastButtonState == BUTTON_UP){
        _heldTime = millis();
    }
    // Released
    else if (buttonState == BUTTON_UP && _lastButtonState == BUTTON_DOWN){
        _buttonHeld = false;
        if ((millis() - _heldTime) > long(HOLD_TIME)){
            Serial.println("VhButton:  Button Held >3s");
            if (!isReconfiguring){
                Serial.println("VhButton: Going in to manual reconfig");
                // Run reconfig next tick
                // Doing it here will cause crash
                _doReconfigure = true;
                // _isReconfiguring = true;
            }
            else {
                Serial.println("VhButton: Aborting manual reconfig to do reset");
                vhled.setState(STATE_WIFI_ERR);
                
                Serial.println("VhButton: vhConf.reset");
                vhConf.reset();

                Serial.println("VhButton: vhWifi.clearSettings");
                vhWifi.clearSettings();
                
                //vhWifi will trigger the reboot
                return true;
            }
        }
        else if ((millis() - _heldTime) > long(DEBOUNCE)){
            Serial.println("VhButton: Button Pressed");
            if (isReconfiguring){
                // _isReconfiguring = false;
                Serial.println("VhButton: Stopping reconfig");
                
                // Give feedback
                vhled.setState(STATE_INIT);
                
                //vhWifi trigger the reboot
                return true;
            }
        }
    }
    // Give visual indication that button has been held long enough
    else if (!_buttonHeld && buttonState == BUTTON_DOWN && _lastButtonState == BUTTON_DOWN){
        if ((millis() - _heldTime) > long(HOLD_TIME)){
            Serial.println("VhButton: Button is being held");
            vhled.setState(STATE_INIT);
            _buttonHeld = true; // Fire only once
        }
    }
    
    _lastButtonState = buttonState;
    
    return false;
}


VhButton button = VhButton();
