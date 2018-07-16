/*
Library for controlling 2 DRV8833 drivers via a PCA9635

http://www.ti.com/lit/ds/symlink/drv8833c.pdf
*/

#ifndef DRV8833_H_
#define DRV8833_H_



#include <Arduino.h>
#include "PCA9634.h"


class drv8833
{
    public:
        
        // address is the i2c bus adderss of the pca9634 chip
        drv8833(uint8_t address, uint8_t enable_pin);
        
        void begin(void);

        void enable();
        void disable();
        
        void setPin( uint8_t pin, uint8_t duty );
		
        void setMotor( uint8_t motor, uint8_t duty, bool fast_decay = true, bool forward = true );

    private:
        uint8_t eepin;
        pca9634 pwmdriver;
};

#endif