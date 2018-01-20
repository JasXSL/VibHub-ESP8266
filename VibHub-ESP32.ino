#include "src/Config/Config.h"
#include "src/Motor/Motor.h"
#include "src/Connection/Connection.h"
#include "src/Gatt/Gatt.h"

Config conf;
Connection con;
Gatt gatt;

Motor motors[1] = {
  Motor(25, 0)
};


// Update a motor
/*
void updateMotor( int port, int intensity ) {

  //Serial.println("Updating motor on port "+(String)port+" "+(String)intensity );
  motors[0].setIntensity(intensity);

}
*/

// Shared in the header file
void onSocketMessage( String type, String message ){

  if( type == "pw" ){

      char r[message.length()+1];
      message.toCharArray(r, message.length()+1);

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

        Serial.println("Update motors here");
        //updateMotor(params[0], params[1]);
        ptr = strtok(NULL, "|");

      }

    }
  
}




void setup() {

  // Initialize
  // 115200 is baud of the board
  Serial.begin(115200);
  delay(500);

  Serial.println("Setting up config");
  conf.begin();

  // initialize wifi and sockets
  con.ini(conf);

  // Initialize gatt
  gatt.ini();
  Serial.println("Gatt enabled");

}

void loop() {
  // put your main code here, to run repeatedly:

  con.loop();


}

