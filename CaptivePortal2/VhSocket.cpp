#include "VhSocket.h"
#include "Configuration.h"
#include "Config.h"
#include "Motor.h"
#include "Vhled.h"
#include <Arduino.h>
#include <SocketIoClient.h>     //https://github.com/timum-viw/socket.io-client


SocketIoClient webSocket;



// Socket.io events

void event_connect(const char * payload, size_t length) {
    Serial.println("got connect");
    webSocket.emit("id", ("\"" + (String)config.deviceid + "\"").c_str());
    vhled.setState(STATE_RUNNING);
}

void event_disconnect(const char * payload, size_t length) {
    Serial.println("got disconnect");
    vhled.setState(STATE_SOCKET_ERR);
}

void event_vib(const char * payload, size_t length) {
    Serial.printf("got vib: %s\n", payload);
}

void event_p(const char * payload, size_t length) {
    unsigned long int data = strtoul(payload, 0, 16);
    uint8_t vibArray[4];
    vibArray[0] = (int)((data & 0xFF000000) >> 24 );
    vibArray[1] = (int)((data & 0x00FF0000) >> 16 );
    vibArray[2] = (int)((data & 0x0000FF00) >> 8 );
    vibArray[3] = (int)((data & 0X000000FF));
    Serial.printf("got p - v0: %u, v1: %u, v2: %u, v3: %u\n", vibArray[0], vibArray[1], vibArray[2], vibArray[3]);
    motorCtrl.setAll(vibArray);
    motorCtrl.runAll(FORWARD);
}


VhSocket::VhSocket(){
}


void VhSocket::connect(){
    //Do socket.io stuff
    webSocket.on("connect", event_connect);
    webSocket.on("disconnect", event_disconnect);
    webSocket.on("vib", event_vib);
    webSocket.on("p", event_p);
    
    //TODO: atoi here is a kludge, fix properly
    webSocket.begin(config.server, atoi(config.port));
}


void VhSocket::loop(){
    webSocket.loop();
}


VhSocket vhSocket = VhSocket();
