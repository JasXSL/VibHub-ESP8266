#ifndef ESP32
  #define ESP32
#endif
#include <SocketIOClient.h>
#include <WiFi.h>
#include "src/Motor/Motor.h"


int stage = 0;
Motor m( 25, 0 );    // Setup a motor on pin 25 and channel 0

#define SSID "" // SSID here
#define PASSWORD "" // Password here
#define DEVICE_ID "ESP32"

// SIO
SocketIOClient client;
char host[] = "vibhub.dangly.parts";     // 192.168.0.195
int port = 80;
extern String RID;
extern String Rname;
extern String Rcontent;

void onConnect(){
    client.send("id", DEVICE_ID);
    Serial.println("Connection established, sending ID");
}

void updateMotor( int port, int intensity ){

  //Serial.println("Updating motor on port "+(String)port+" "+(String)intensity );
  m.setIntensity(intensity);
  
}

void setup() {

    // Initialize
    // 115200 is baud of the board
    Serial.begin(115200);
    delay(500);

    Serial.println("INI");
    
    m.setIntensity(100);

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
      onConnect();


}

void loop() {
  // put your main code here, to run repeatedly:

  if( !client.connected() ){

    client.reconnect(host, port);
    if( client.connected() )
      onConnect();
    delay(1000);
    
  }

  
  if (client.monitor()){
    
      //Serial.println(RID);
      //Serial.println(Rcontent);

      if( RID == "pw" ){
        
        //Serial.println("Got content: "+(String)Rcontent);
        char r[Rcontent.length()+1];
        Rcontent.toCharArray(r, Rcontent.length()+1);
        
        char *ptr = strtok(r, "|");
        while( ptr != NULL ){

          char *sub = strtok(ptr, ",");
          int params[2];
          int i = 0;
          while( sub != NULL ){

             String s(sub);
             int n = s.toInt();
             params[i++] = s.toInt();

             sub = strtok(NULL, ",");
          }

          updateMotor(params[0], params[1]);
          ptr = strtok(NULL, "|");
          
        }

      }
      
  }

}

