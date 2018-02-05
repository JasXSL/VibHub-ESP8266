/*
	VibHub Client
*/
#ifndef VhClient_h
#define VhClient_h

#include <Arduino.h>
#include "VhSocketIoClient.h"
#include <functional>

using namespace std::placeholders;


class VhClient {
    public:
        VhClient(void);
        
        void connect();
        void disconnect();
        
        bool connected() {return _connected;}
        
        void loop();
        
    private:
        SocketIoClient _socket;
        
        void event_connect(const char * payload, size_t length);
        void event_disconnect(const char * payload, size_t length);
        void event_vib(const char * payload, size_t length);
        void event_p(const char * payload, size_t length);
        
        bool _connected;
        bool _running;
};

extern VhClient client;

#endif //VhClient_h
