#ifndef ESP32
	#define ESP32
#endif
#include "Connection.h"
#include <WiFi.h>
#include "../../VibHub-ESP32.h"
//#include "../Config/Config.h"

// external

Connection::Connection(){}

// Tries to connect to wifi, returns true if it succeeds
bool Connection::wifiConnect(){

  int len = conf.ssid.length();
  char ssid[len];
  conf.ssid.toCharArray(ssid, len);

  len = conf.password.length();
  char password[len];
  conf.password.toCharArray(password, len);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while( WiFi.status() != WL_CONNECTED ){
    delay(500);
    Serial.print(".");
    if( ++attempts == 10 ){
      Serial.println("Unable to connect for some reason");
      return false;
    }
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
  
}

// Returns whether we are connected to wifi
bool Connection::wifiConnected(){
  return WiFi.status() == WL_CONNECTED;
}

// Connects or reconnects to socket
void Connection::socketConnect( bool reconnect ){

  if( !wifiConnected() )
    return;

  int len = conf.host + 1;
  char host[len];
  conf.host.toCharArray(host, len);
  
  Serial.println("Starting socket.io:");
  Serial.println("Host: "+conf.host+" Port: "+(String)conf.port);

  // Reconnect
  if( reconnect ){

    client.reconnect(host, conf.port);
    delay(1000);
    return;
    
  }

  // New connection
  if( !client.connect(host, conf.port) ){
    Serial.println("connection failed");
    return;
  }

  if( client.connected() ){
    client.send("id", conf.device_id);
  	Serial.println("Connection established, sending ID");
  }
  

}




void Connection::ini( Config co ){

	conf = co;
	bool con = wifiConnect();

	

	if( con )
		socketConnect(false);

}


void Connection::loop(){

  /*
	if( !client.connected() )
    	socketConnect(true);
  */
  
	if( client.monitor() ){

		//Serial.println(RID);
		//Serial.println(Rcontent);
		onSocketMessage( client.RID, client.Rcontent );		

	}

}
