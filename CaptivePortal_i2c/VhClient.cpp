/*
	VibHub Client
    
    Handles network communication with server
*/

#include "VhClient.h"

#include "Configuration.h"
#include "Config.h"
#include "Vhled.h"
#include "VhPwm.h"


VhClient::VhClient(void) :
    _socket(),
    _connected(false),
    _running(false)
{
    
}


void VhClient::setup(){
    // Initialize motors
    // VhMotor::begin();
	motors.push_back(VhMotor(CHANNEL_MOTOR_0));
	motors.push_back(VhMotor(CHANNEL_MOTOR_1));
	motors.push_back(VhMotor(CHANNEL_MOTOR_2));
	motors.push_back(VhMotor(CHANNEL_MOTOR_3));
    
    // Attach event handlers
    // For simplicity, events are always attached regardless
    _socket.on("connect", std::bind(&VhClient::event_connect, this, _1, _2));
    _socket.on("disconnect", std::bind(&VhClient::event_disconnect, this, _1, _2));
    _socket.on("vib", std::bind(&VhClient::event_vib, this, _1, _2));
    _socket.on("p", std::bind(&VhClient::event_p, this, _1, _2));
}


void VhClient::connect(){
    Serial.println("VhClient::connect");
    _running = true; // Start loop
    _socket.begin(vhConf.server, vhConf.port);
}

void VhClient::disconnect(){
    Serial.println("VhClient::disconnect");
    _connected = false; // we don't need the disconnect event
    _running = false; // Don't run loop (will cause reconnect)
    _socket.disconnect();
}

void VhClient::event_connect(const char * payload, size_t length){
    Serial.println("VhClient::event_connect");
    _connected = true;
    _socket.emit("id", ("\"" + (String)vhConf.deviceid + "\"").c_str());
    vhled.setState(STATE_RUNNING);
    pwm.enable();
}

void VhClient::event_disconnect(const char * payload, size_t length){
    if (_connected){ // Fire only once
        Serial.println("VhClient::event_disconnect");
        vhled.setState(STATE_SOCKET_ERR);
        _connected = false;
        pwm.disable();
    }
}


void VhClient::event_vib(const char * payload, size_t length){
    Serial.printf("VhClient::event_vib: %s\n", payload);

    DynamicJsonDocument jsonBuffer(4096);
    DeserializationError error = deserializeJson(jsonBuffer, payload);

    if( error ){
        Serial.println("Unable to read vib event");
        return;
    }

    Serial.println("Vib received with");
    serializeJson(jsonBuffer, Serial);
    Serial.println();

    DynamicJsonDocument js(2048);
    JsonArray arr = js.to<JsonArray>();

    if( !jsonBuffer.is<JsonArray>() ){
        JsonObject obj = jsonBuffer.as<JsonObject>();
        arr.add(obj);
    }
    else{
        for( byte i=0; i<jsonBuffer.size(); ++i )
            arr.add(jsonBuffer[i]);
    }

    // Cycle through all programs
    for( byte i=0; i<arr.size(); ++i ){

        JsonObject j = arr[i];

        //Serial.printf("Program %i\n", i);
        //serializeJson(j, Serial);
        //Serial.println();

        bool mo[4] = {true, true, true, true};
        
        if( j.containsKey("port") ){

            int port = j["port"];
            if( port > 0 ){

                for( int i = 0; i<4; ++i )
                    mo[i] = port&(1<<i);

            }

        }

        int repeats = 0;
        if( j.containsKey("repeats") )
            repeats = j["repeats"];

        for( byte n=0; n<4; ++n ){

            if( mo[n] )
                motors[n].loadProgram(j["stages"], repeats);

        }

    }
    


}


void VhClient::event_p(const char * payload, size_t length){
    unsigned long int data = strtoul(payload, 0, 16);
    uint8_t vibArray[4];
    vibArray[0] = (int)((data & 0xFF000000) >> 24 );
    vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
    vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
    vibArray[3] = (int)((data & 0X000000FF));
    Serial.printf("VhClient::event_p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);
    
    int i;
    for( i = 0; i < 4; ++i ){
        motors[i].stopProgram();  // Stop any running program when this is received
        motors[i].setPWM(vibArray[i]);
    }
}


void VhClient::loop() {

    if (_running){
        _socket.loop();
        
        for( int i=0; i<motors.size(); ++i )
            motors[i].update();
    }

}


VhClient client = VhClient();
