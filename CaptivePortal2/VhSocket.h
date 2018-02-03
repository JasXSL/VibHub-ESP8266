/*
	VibHub Socket.io
*/
#ifndef VhSocket_h
#define VhSocket_h

#include <Arduino.h>
#include <SocketIoClient.h>
#include <functional>

using namespace std::placeholders;


class VhSocket : public SocketIoClient {

	public:
        VhSocket(void): SocketIoClient(){}
        
        void connect();
        
	private:
        void event_connect(const char * payload, size_t length);
        void event_disconnect(const char * payload, size_t length);
};

#endif //VhSocket_h