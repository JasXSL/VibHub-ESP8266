/*
Based off https://github.com/HydroSense/HydroSense_PCA9634
*/

#ifndef PCA9634_H_
#define PCA9634_H_

#define PCA9634_REG_MODE1		(byte)0x00		// Mode register 1
#define PCA9634_REG_MODE2		(byte)0x01		// Mode register 2
#define PCA9634_REG_PWM0		(byte)0x02		// brightness control LED0
#define PCA9634_REG_PWM1		(byte)0x03		// brightness control LED1
#define PCA9634_REG_PWM2		(byte)0x04		// brightness control LED2
#define PCA9634_REG_PWM3		(byte)0x05		// brightness control LED3
#define PCA9634_REG_PWM4		(byte)0x06		// brightness control LED4
#define PCA9634_REG_PWM5		(byte)0x07		// brightness control LED5
#define PCA9634_REG_PWM6		(byte)0x08		// brightness control LED6
#define PCA9634_REG_PWM7		(byte)0x09		// brightness control LED7
#define PCA9634_REG_GRPPWM		(byte)0x0A		// group duty cycle
#define PCA9634_REG_GRPFREQ		(byte)0x0B		// group frequency
#define PCA9634_REG_LEDOUT0		(byte)0x0C		// LED output state 0
#define PCA9634_REG_LEDOUT1		(byte)0x0D		// LED output state 1
#define PCA9634_REG_SUBADR1		(byte)0x0E		// I2C-bus subaddress 1
#define PCA9634_REG_SUBADR2		(byte)0x0F		// I2C-bus subaddress 2
#define PCA9634_REG_SUBADR3		(byte)0x10		// I2C-bus subaddress 3
#define PCA9634_REG_ALLCALLADR	(byte)0x11		// LED All I2C-bus address

#define PCA9634_SWRST_ADDR		(byte)0x06		// Software reset address
#define PCA9634_SWRST_B1		(byte)0xA5		// Software reset byte 1
#define PCA9634_SWRST_B2		(byte)0x5A		// Software reset byte 2

#define PCA9634_ALL_LED_TO_OFF				(byte)0x00
#define PCA9634_ALL_LED_TO_ON				(byte)0x55
#define PCA9634_ALL_LED_TO_PWM				(byte)0xAA
#define PCA9634_ALL_LED_TO_GRPPWM			(byte)0xFF

#define ADDRESS_HI							(byte)0x12
#define ADDRESS_LO							(byte)0x34

#define PCA9634_ERROR_INVALID_LED           -1

#include <Arduino.h>
#include <Wire.h>


class pca9634
{
    public:
        
        // address is the i2c bus adderss of the pca9634 chip
        pca9634(uint8_t address);

        // sets up pca9634 and turns all leds off
        void begin(void);

        // these 3 functions do writes directly to pca9634 registers
        // read that manual if you want to use them.
        uint8_t set_mode2(uint8_t mode2);
        uint8_t set_mode1(uint8_t mode1);
        uint8_t set_duty(uint8_t led, uint8_t duty_cycle);
        
        void reset(); // SWRST Call

    private:
        uint8_t _address;
        uint8_t write_reg(uint8_t reg, uint8_t value);     
};

#endif
