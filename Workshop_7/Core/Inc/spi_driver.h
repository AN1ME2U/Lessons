/*
 * driver.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: DIO
 */

#ifndef INC_SPI_DRIVER_H_
#define INC_SPI_DRIVER_H_

void HOME_SCREEN(void);
void SPI_DISABLE_WP(void);
void SPI_WREN(void);
void SPI_READ(void);
void SPI_ERASE(void);
void SPI_WRITE(void);
void ADDRESS_MAKER(uint16_t line, uint16_t byte);

/* 	Temporary unused
void SPI_READ_BYTE(void);
void SPI_ERASE_BYTE(void);
void SPI_WRITE_BYTE(void);
void SPI_WRDI(void);
	Temporary unused */


#endif /* INC_SPI_DRIVER_H_ */
