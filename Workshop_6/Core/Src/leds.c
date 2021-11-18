#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "leds.h"

//HAL_I2C_Master_Transmit(&hi2c1, (I2C_ADDRESS << 1), &regAddress, 1,  I2C_TIMEOUT);
//HAL_I2C_Master_Receive(&hi2c1, (I2C_ADDRESS << 1), &regData, 1,  I2C_TIMEOUT);

uint8_t DEFAULT_SETUP = 0x21;
uint8_t TRANSMIT[4] = {0};
uint8_t TX_RST[5] = {LED0, 0,0,0,0};
uint8_t REG_DATA = 0;
extern char DONE[13];

void DRIVER_SETUP(void){																								//Load base configuration to external driver
	HAL_I2C_Mem_Write (I2C_HANDLER, DRIVER_ADDR, MODE1, 1, (uint8_t *)&DEFAULT_SETUP, 1, 1000);
}

void LED_RST(uint8_t reg){
	TX_RST[0] = reg;
	HAL_I2C_Master_Transmit(&hi2c1, DRIVER_ADDR, TX_RST, 5, 1000);
}

void LED_WRITE(uint8_t led_num, uint16_t percent){																		//Writes a new value in pwm control registers of driver
	(led_num == 16) ? (led_num = ALL_LED) : (led_num = 6+(led_num*4));
	uint16_t duty = 4095*percent/100;
	TRANSMIT[3] = (uint8_t)(duty >> 8);
	TRANSMIT[2] = (uint8_t)duty;
	LED_RST(led_num);
	HAL_I2C_Mem_Write_IT(I2C_HANDLER, DRIVER_ADDR, led_num, 1, (uint8_t *)TRANSMIT, 4);
}

void LED_PRESCALLER_SET(uint8_t presc_val){																				//Writes new value to prescaler register of driver
	SLEEP_MODE(ON);
	TRANSMIT[2] = presc_val;
	HAL_I2C_Mem_Write(I2C_HANDLER, DRIVER_ADDR, PRESC_ADDR, 1, (uint8_t *)&TRANSMIT[2], 1, 1000);
	SLEEP_MODE(OFF);
	HAL_UART_Transmit(&huart3, (uint8_t *)DONE, 13, 1000);
}

void I2C_READER(uint8_t reg_addr){																						//Reader for I2C device register
	HAL_I2C_Mem_Read(&hi2c1, DRIVER_ADDR, reg_addr, 1, (uint8_t *)&REG_DATA , 1, 1000);
}


void SLEEP_MODE(uint8_t state){																							//Disable and enable sleep mode
	I2C_READER(MODE1);
	REG_DATA &= ~(1 << 4);
	REG_DATA |= (state << 4);
	TRANSMIT[2] = REG_DATA;
	HAL_I2C_Mem_Write(I2C_HANDLER, DRIVER_ADDR, MODE1, 1, (uint8_t *)&TRANSMIT[2], 1, 1000);
	HAL_UART_Transmit(&huart3, (uint8_t *)DONE, 13, 1000);

}
