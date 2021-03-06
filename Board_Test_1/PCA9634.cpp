/*
Based off https://github.com/HydroSense/HydroSense_PCA9634

Significantly expanded by Kadah, 2018

See datasheet for more info on everything
https://www.nxp.com/docs/en/data-sheet/PCA9634.pdf
*/
 
 
#include "PCA9634.h"

//#define DEBUG_DUTY

pca9634::pca9634(uint8_t address){
    _address = address;
}

void pca9634::begin( uint8_t mode1 /*= 0*/, uint8_t mode2 /*= 0x16*/ ){
    write_reg( PCA9634_REG_MODE1, mode1 );
    write_reg( PCA9634_REG_MODE2, mode2 );
    write_reg( PCA9634_REG_LEDOUT0, PCA9634_ALL_LED_TO_PWM );
    write_reg( PCA9634_REG_LEDOUT1, PCA9634_ALL_LED_TO_PWM );
    // initalize all leds to default PWM control
    int i;
    for (i=0; i < 8; i++){
        set_duty(i, 0);
    }
}

uint8_t pca9634::set_mode1(uint8_t mode1){
    return write_reg( PCA9634_REG_MODE1, mode1 );
}

uint8_t pca9634::set_mode2(uint8_t mode2){
    return write_reg (PCA9634_REG_MODE2, mode2);
}

uint8_t pca9634::set_duty(uint8_t led, uint8_t duty_cycle){
    
    #ifdef DEBUG_DUTY
        Serial.print("pca9634::set_duty - pin: ");
        Serial.print(led);
        Serial.print(" duty_cycle: ");
        Serial.println(duty_cycle);
    #endif
    
    // limit led to a 3-bit unsigned value.
    return write_reg( PCA9634_REG_PWM0 + ( (uint8_t) (led & 0x7) ), duty_cycle );
}


void pca9634::reset(){
    Wire.beginTransmission( PCA9634_SWRST_ADDR );
    Wire.write( PCA9634_SWRST_B1 );
    Wire.write( PCA9634_SWRST_B2 );
    Wire.endTransmission( );
    delayMicroseconds(60);
}

uint8_t pca9634::write_reg(uint8_t reg, uint8_t value){
    Wire.beginTransmission( _address );
    Wire.write( reg );
    Wire.write( value );
    return Wire.endTransmission( );
}
