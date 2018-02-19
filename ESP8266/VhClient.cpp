/*
	VibHub Client
    
    Handles network communication with server
*/

#include "VhClient.h"

#include "Configuration.h"
#include "Config.h"
#include "Vhled.h"


VhClient::VhClient(void) :
    _socket(),
    _connected(false),
    _running(false)
{
    
}


void VhClient::setup(){
    // Initialize motors
	motors.push_back(Motor(PIN_MOTOR_0, CHANNEL_MOTOR_0));
	motors.push_back(Motor(PIN_MOTOR_1, CHANNEL_MOTOR_1));
	motors.push_back(Motor(PIN_MOTOR_2, CHANNEL_MOTOR_2));
	motors.push_back(Motor(PIN_MOTOR_3, CHANNEL_MOTOR_3));
    
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
}

void VhClient::event_disconnect(const char * payload, size_t length){
    if (_connected){ // Fire only once
        Serial.println("VhClient::event_disconnect");
        vhled.setState(STATE_SOCKET_ERR);
        _connected = false;
    }
}


void VhClient::event_vib(const char * payload, size_t length){
    Serial.printf("VhClient::event_vib: %s\n", payload);

    DynamicJsonBuffer jsonBuffer;
    JsonVariant variant = jsonBuffer.parse(payload);

    
    variant.printTo(Serial);

    if( variant.success() ){

        // Convert to JSON array
        int i;
        DynamicJsonBuffer buf;
        JsonArray& js = buf.createArray();
        if( !variant.is<JsonArray>() ){


            JsonObject& obj = variant;
            js.add(obj);

        }
        else{
            
            JsonArray &arr = variant;
            for( i=0; i<arr.size(); ++i )
                js.add(arr[i]);

        }

        // Cycle through all programs
        for( i=0; i<js.size(); ++i ){

            JsonObject& j = js[i];

            bool mo[4] = {true, true, true, true};
            
            if( j.containsKey("port") ){

                int port = atoi(j["port"]);
                if( port != -1 ){

                    mo[0] = mo[1] = mo[2] = mo[3] =  false;
                    mo[port] = true;

                }

            }

            int repeats = 0;
            if( j.containsKey("repeats") )
                repeats = atoi(j["repeats"]);

            int i;
            for( i=0; i<4; ++i ){
                
                //Todo: Add type checking?
                if( mo[i] )
                    motors[i].loadProgram(j["stages"], repeats);

            }

        }
        

    }
    else
        Serial.println("failed to load json config");

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
    for( i = 0; i < 4; ++i )
        motors[i].setPWM(vibArray[i]);

}


void VhClient::loop() {
    if (_running){
        _socket.loop();
    }
}


VhClient client = VhClient();
