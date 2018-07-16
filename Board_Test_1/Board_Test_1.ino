#include <Wire.h>
#include "PCA9634.h"
#include "DRV8833.h"
#include "ESP8266_info.h"
#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#define PIN_ENABLE 15

drv8833 driver(PCA9634_ALL_CALL_ADDR, PIN_ENABLE);


void flash_info() {
	uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();

    Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
    Serial.printf("Flash real size: %u\n\n", realSize);

    Serial.printf("Flash ide  size: %u\n", ideSize);
    Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
    Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

    if(ideSize != realSize) {
        Serial.println("Flash Chip configuration wrong!\n");
    } else {
        Serial.println("Flash Chip configuration ok.\n");
    }
}

void i2c_scan() {
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


// Blinks enable pin at 10hz for 2 seconds
void blink_enable(){
	bool blink = false;
	for (int j=0; j<20; j++){
		digitalWrite(PIN_ENABLE, blink);
		blink = !blink;
		delay(100);
	}
}

//
#define LEDTESTDELAY 150
void pwmn_wave() {
	unsigned long timer = 0;
	int i; int j; int k;
	int ledValue;
	// Turn all off
	for (i=0; i<8; i++){
		driver.setPin(i, 0);
	}
	
	// Sequence through each led channel one at a time
	Serial.println("LED Seq");
	for (j=0; j<2; j++){
		for (i=0; i<8; i++){
			driver.setPin(i, 255);
			delay(LEDTESTDELAY);
			driver.setPin(i, 0);
		}
		for (i=6; i>0; i--){ // Missing the first and last is intentional
			driver.setPin(i, 255);
			delay(LEDTESTDELAY);
			driver.setPin(i, 0);
		}
		yield();
	}
	// End sequence by manaully doing first
	driver.setPin(0, 255);
	delay(LEDTESTDELAY);
	driver.setPin(0, 0);
	
	// Fade on each channel then off
	Serial.println("LED Fade");
	for (k=0; k<2; k++){
		for (i=0; i<8; i++){
			for (j=0; j<256; j++){
				yield();
				delay(1);
				driver.setPin(i, j);
			}
			yield();
		}
		for (i=0; i<8; i++){
			for (j=255; j>=0; j--){
				yield();
				delay(1);
				driver.setPin(i, j);
			}
		}
	}
	
	// Waves all
	Serial.println("LED Wave");
    for (j=0; j<500; j++){
		timer = millis();
		for (i=0; i<8; i++){
			ledValue = 128 + (127 * cos((2*PI/2000 * timer) + ((i/8.0) * PI*2)));
			driver.setPin(i, ledValue);
		}
		delay(10);
		yield();
	}
}

//
void _motor_wave(bool fast_decay, bool forward){
	unsigned long timer = 0;
	for (int j=0; j<300; j++){
		timer = millis();
		
		driver.setMotor(0, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		driver.setMotor(1, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		driver.setMotor(2, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		driver.setMotor(3, (128 + (127 * cos((2*PI/2000 * timer) + PI*2))), fast_decay, forward);
		
		yield();
		delay(20);
	}
}
void motor_wave() {
	Serial.println("Fast Decay, Forward");
	_motor_wave(true, true);
	Serial.println("Fast Decay, Reverse");
	_motor_wave(true, false);
	Serial.println("Slow Decay, Forward");
	_motor_wave(false, true);
	Serial.println("Slow Decay, Reverse");
	_motor_wave(false, false);
}

void motor_reset_channels(){
	driver.setMotor(0, 0, true, true);
	driver.setMotor(1, 0, true, true);
	driver.setMotor(2, 0, true, true);
	driver.setMotor(3, 0, true, true);
	// for (int i=0; i<8; i++){
		// driver.setPin(i, 0);
	// }
}

#define CHANNELTESTDELAY 300
void motor_test_channel(uint8_t ch){
	
	Serial.print("Testing motor channel: ");
	Serial.println(ch);
	driver.enable();
	
	Serial.println("Fast Decay, Off");
	driver.setMotor(ch, 0, true, true);
	delay(CHANNELTESTDELAY);
	
	Serial.println("Fast Decay, Forward, On");
	driver.setMotor(ch, 255, true, true);
	delay(CHANNELTESTDELAY);
	
	Serial.println("Fast Decay, Reverse, On");
	driver.setMotor(ch, 255, true, false);
	delay(CHANNELTESTDELAY);
	
	
	Serial.println("Slow Decay, Off");
	driver.setMotor(ch, 0, false, true);
	delay(CHANNELTESTDELAY);
	
	Serial.println("Slow Decay, Forward, On");
	driver.setMotor(ch, 255, false, true);
	delay(CHANNELTESTDELAY);
	
	Serial.println("Slow Decay, Reverse, On");
	driver.setMotor(ch, 255, false, false);
	delay(CHANNELTESTDELAY);
	
	
	// Turn off
	driver.setMotor(ch, 0, true, true);
	driver.disable();
	delay(CHANNELTESTDELAY);
}


void setup() {
    Serial.begin(115200);
    
    Wire.begin();
    Serial.println("Started");
    
	
	flash_info();
	
	i2c_scan();
    
    driver.begin();
	
	motor_reset_channels();
	blink_enable();
	driver.disable();
	
}

unsigned int loops = 0;

void loop() {
	
	Serial.print("\nLoop : ");
	Serial.println(loops);
	loops++;
	
	Serial.println("PWM Wave");
	driver.disable();
	pwmn_wave();
	
	motor_reset_channels();
	
	Serial.println("Motor Channel Test");
	motor_test_channel(0);
	motor_test_channel(1);
	motor_test_channel(2);
	motor_test_channel(3);
	
	Serial.println("Motor Wave");
	driver.enable();
	motor_wave();
}

