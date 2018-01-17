#ifndef ESP32
  #define ESP32
#endif
#include <SocketIOClient.h>
#include <WiFi.h>
#include "src/Motor/Motor.h"

int stage = 0;
Motor m( 25, 0 );    // Setup a motor on pin 25 and channel 0

#define SSID "DogeSuchWOW"
#define PASSWORD "doge is love doge is life"


// SIO
SocketIOClient client;
char host[] = "http://vibhub.dangly.parts";
int port = 80;
extern String RID;
extern String Rname;
extern String Rcontent;


void setup() {

    // Initialize
    // 115200 is baud of the board
    Serial.begin(115200);
    delay(500);

    Serial.println("INI");
    
    m.setPWM(255);

	  Serial.print("Connecting to ");
	  Serial.println(SSID);

	  WiFi.begin(SSID, PASSWORD);

  	while( WiFi.status() != WL_CONNECTED ){
  		  delay(500);
  		  Serial.print(".");
  	}

    

  	Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
  	Serial.println("Starting socket.io");

    Serial.println("Connecting to "+(String)host+" "+(String)port);
    if( !client.connect(host, port) ){
      Serial.println("connection failed");
      return;
    }
    if( client.connected() )
    {
      client.send("connection", "message", "Connected !!!!");
    }


}

void loop() {
  // put your main code here, to run repeatedly:

  /*
  if (client.monitor()){
    
      //Serial.println(RID);
      //Serial.println(Rcontent);
      
  }
*/
}

