#include <Wire.h>
#include "DRV8833.h"


drv8833 motors(0x00, 15);


void scan()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

//    Serial.print("Scanning: ");
//    Serial.println(address,HEX);

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}


void setup() {
    Serial.begin(115200);
    
    Wire.begin();
    Serial.println("Started");
    scan();
    
    
    motors.begin();
    
    motors.enable();
}

unsigned long timer = 0;

void loop() {
    
    timer = millis();
    
    motors.setMotor(0, (128 + (127 * cos((2*PI/2000 * timer) + ((0/4.0) * PI*2)))), true, true);
    motors.setMotor(1, (128 + (127 * cos((2*PI/2000 * timer) + ((1/4.0) * PI*2)))), false, true);
    motors.setMotor(2, (128 + (127 * cos((2*PI/2000 * timer) + ((2/4.0) * PI*2)))), true, false);
    motors.setMotor(3, (128 + (127 * cos((2*PI/2000 * timer) + ((3/4.0) * PI*2)))), false, false);
    
    delay(10);
}

