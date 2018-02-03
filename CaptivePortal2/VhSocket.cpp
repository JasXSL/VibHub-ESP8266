#include "VhSocket.h"
#include "Configuration.h"
#include "Config.h"
#include "Motor.h"
#include "Vhled.h"
#include <Arduino.h>
#include <SocketIoClient.h>

// #undef bind

void VhSocket::connect(){
    //Handle basic socket.io events
    SocketIoClient::on("connect", std::bind(&VhSocket::event_connect, this, _1, _2));
    SocketIoClient::on("disconnect", std::bind(&VhSocket::event_disconnect, this, _1, _2));
    
    //TODO: atoi here is a kludge, fix properly
    begin(config.server, config.port);
}


void VhSocket::event_connect(const char * payload, size_t length) {
    Serial.println("got connect");
    SocketIoClient::emit("id", ("\"" + (String)config.deviceid + "\"").c_str());
    vhled.setState(STATE_RUNNING);
}

void VhSocket::event_disconnect(const char * payload, size_t length) {
    Serial.println("got disconnect");
    vhled.setState(STATE_SOCKET_ERR);
}

