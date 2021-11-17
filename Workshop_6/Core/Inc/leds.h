/*
 * leds.h
 *
 *  Created on: Nov 15, 2021
 *      Author: DIO
 */
/*
 * Library for PCA9685 LED driver
 * For use it, type /led in uart console and follow instructions
 * For change pwm frequency for LED, write 0 duty cycle previously, it's mandatory
 */



#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#endif /* INC_LEDS_H_ */

#define LED0 		6
#define LED1 		10
#define LED2 		14
#define LED3 		18
#define LED4 		22
#define LED5 		26
#define LED6 		30
#define LED7 		34
#define LED8 		38
#define LED9 		42
#define LED10 		46
#define LED11 		50
#define LED12 		54
#define LED13 		58
#define LED14 		62
#define LED15 		66
#define ALL_LED 	250
#define PRESC_ADDR 	254
#define MODE1 		0x0
#define MODE2 		0x01

#define ON 1
#define OFF 0

#define DRIVER_ADDR 0x80
#define I2C_HANDLER &hi2c1

void LED_WRITE(uint8_t led_num, uint16_t percent);
void LED_PRESCALLER_SET(uint8_t presc_val);
void DRIVER_SETUP(void);
void I2C_READER(uint8_t reg_addr);
void SLEEP_MODE(uint8_t state);


