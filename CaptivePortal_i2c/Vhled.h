/*
    VibHub Led
*/
#ifndef Vhled_h
#define Vhled_h

#include "Arduino.h"

//TODO: Color enum class

//TODO: enum class
#define STATE_BOOT       0  // Program is booting   <0,0,128>
#define STATE_INIT       1  // Program will go in to captive portal mode    <0,0,255>
#define STATE_PORTAL     2  // Program is in captive portal mode    <0,0,255> blinking
#define STATE_WIFI_ERR   3  // Unable to connect to wifi    <255,0,0>
#define STATE_SOCKET_ERR 4  // Unable to connect to socket server   <128,128,0>
#define STATE_RUNNING    5  // Connected and listening  <0,255,0>

class Vhled{

    public:
        Vhled(void);
        
        void setup();
        void setState( int state = STATE_BOOT );
        void test();
        
    private:
        int programState;           // State the program is currently in
        
};


extern Vhled vhled;

#endif //Vhled_h