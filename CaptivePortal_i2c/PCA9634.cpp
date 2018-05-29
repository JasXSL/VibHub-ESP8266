/*
Based off https://github.com/HydroSense/HydroSense_PCA9634
*/
 
 
#include "PCA9634.h"

// #define DEBUG_DUTY

pca9634::pca9634(uint8_t address){
    _address = address;
}

void pca9634::begin(){
    // initalize all leds to default PWM control
    int i;
    write_reg( PCA9634_REG_MODE1, 0 );
    write_reg( PCA9634_REG_MODE2, 0x16 ); // INVRT | OUTDRV | OUTNE1
    write_reg( PCA9634_REG_LEDOUT0, PCA9634_ALL_LED_TO_PWM );
    write_reg( PCA9634_REG_LEDOUT1, PCA9634_ALL_LED_TO_PWM );
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
    return write_reg( PCA9634_REG_PWM0 + ( (uint8_t) (led & 0x7) ),
                     duty_cycle );
}


uint8_t pca9634::write_reg(uint8_t reg, uint8_t value){
    Wire.beginTransmission( _address );
    Wire.write( reg );
    Wire.write( value );
    return Wire.endTransmission( );
}
