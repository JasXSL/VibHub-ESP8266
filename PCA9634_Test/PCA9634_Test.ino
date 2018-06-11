#include <Wire.h>
#include "PCA9634.h"

pca9634 _pwm = pca9634(0x00);

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
    
    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);
    
    Wire.begin();
    Serial.println("Started");
    scan();
    
    _pwm.reset();
    _pwm.begin();
    // _pwm.set_mode2(0x02);
}

unsigned long timer = 0;
bool blink = 0;
int c = 0;

void loop() {
    
    timer = millis();
    
    for (int i=0; i<8; i++){
        int ledValue = 128 + (127 * cos((2*PI/2000 * timer) + ((i/8.0) * PI*2)));
        int r = _pwm.set_duty(i, ledValue);
        // Serial.print(", ");
        // Serial.print(i);
        // Serial.print(":");
        // Serial.print(r);
        // Serial.print("=");
        // Serial.print(ledValue);
    }
    // Serial.println();
    
    
    digitalWrite(15, blink);
    if (c == 0) blink = !blink;
    c = (c + 1) % 255;
    
    delay(10);
}

