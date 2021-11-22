#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "spi_driver.h"

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart3;

// Used SPI Flash commands
const uint8_t FLASH_READ = 0x03;							//Read flash from address
const uint8_t FLASH_READ_STATUS = 0x05;						//Read status register
const uint8_t FLASH_EWSR = 0x50;							//Enable write status register
const uint8_t FLASH_ERASE_ALL = 0x60;						//Erase all flash memory
const uint8_t FLASH_WRITE_BYTE = 0x02;						//Write single byte
const uint8_t FLASH_WRITE_STATUS_REGISTER = 0x01;			//Write to status register
const uint8_t FLASH_WRITE_ENABLE = 0x06;					//Write enable
//Used SPI Flash commands

// Unused SPI Flash commands
const uint8_t FLASH_READ_INFO = 0xAB;						//Read manufacturer info
const uint8_t FLASH_READ_JEDEC = 0x9F;						//Read manufacturer info, but in HEDEC specification
const uint8_t FLASH_ERASE_4K = 0x20;						//Erase 4Kbyte block
const uint8_t FLASH_WRITE_AAI = 0xAD;						//Write with address auto increase mode
const uint8_t FLASH_WRITE_DISABLE = 0x04;					//Write disable
//Unused SPI Flash commands

//UART messages
const char WAITING_MSG = '.';
const char WELCOME_MSG[] = {"\r\nHELLO! WHAT would you like to do?\r\nFor read Flash memory press 1(Note: Read only first 100 bytes of each 4Kbyte block. Empty lines will be ignored)\r\nFor erase all data press 2\r\nFor write data from MCU memory press 3\r\n"};
const char WRONG_COMMAND_MSG[] = {"\n\rWrong command, please try again\a\n\r"};
const char TIMEOUT_ERROR_MSG[] = {"\n\rError occurred!!! Message receive timeout!! \a\n\r"};
const char ERASE_COMPLETE_MSG[] = {"\nFLASH ERASE COMPLETE\r\n"};
const char ERASE_IN_PROGRESS_MSG[] = {"\nFLASH ERASE IS IN PROGRESS\r\n"};
const char WRITE_IN_PROGRESS_MSG[] = {"\nSPI FLASH WRITE IS IN PROGRESS\r\n"};
const char WRITE_COMPLETE_MSG[] = {"\n\rSPI FLASH WRITE COMPLETE\r\n"};
//UART messages

//SPI Flash messages
const char FROM[] = {"From: Zvirenko Mykyta,nikitaandronov342@gmail.com\r\n"};
const char MENTOR[] = {"Mentor: Kostiuk Vitalii,vitalii.kostiuk@globallogic.com\r\n"};
const char DATE[] = {"21.11.2021"};
const char CAPSULE_MSG_1[] = {"Factorio is the best game in da world"};
const char CAPSULE_MSG_2[] = {"Is \"One Piece\" still on go?"};
const char CAPSULE_MSG_3[] = {"Ты любишь КАБАЧКИ, Джотаро?"};
const char CAPSULE_MSG_4[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⣀⡀⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_5[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⣴⣿⣿⠿⣫⣥⣄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_6[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⠄⢀⠄⠄⠄⠾⢿⢟⣵⣾⣿⡿⠃⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_7[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⣰⡿⣀⣤⣴⣾⣿⡇⠙⠛⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_8[] = {"⠄⠄⠄⠄⠄⠄⣠⣾⣿⣿⣿⣿⣿⣿⣿⠁⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_9[] = {"⠄⠄⠄⠄⠄⣴⣿⣿⠿⠛⠉⢩⣿⣿⡇⠄⠄⠄⠄⠄⠄⠄⠄⣀⣀⡀⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_10[] = {"⠄⠄⠄⠄⠈⠛⠉⠄⠄⠄⠄⢸⣿⣿⡇⠄⠄⠄⠄⠄⠄⢀⣼⡿⣫⣾⠆⠄⠄⠄\r\n"};
const char CAPSULE_MSG_11[] = {"⠄⠄⠄⠄⢀⣶⣶⣶⣶⣶⣶⣿⣿⣿⠇⠄⠄⠄⣠⣎⣠⣴⣶⠎⠛⠁⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_12[] = {"⠄⠄⠄⠄⣾⣿⣿⣿⣿⠿⠿⠟⠛⠋⠄⠄⢀⣼⣿⠿⠛⣿⡟⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_13[] = {"⠄⠄⠄⠄⠛⠉⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠘⠉⠄⠄⢸⣿⡇⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_14[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⣼⣿⣿⣿⡿⠿⠃⠄⠄⠄⠄⠄⠄⠄\r\n"};
const char CAPSULE_MSG_15[] = {"⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠋⠉⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄\r\n"};
//SPI Flash messages

//SPI Flash messages sizes
const uint8_t SIZES_FOR_TO_WRITE_LIST[] = {		strlen(FROM),
												strlen(MENTOR),
												strlen(DATE),
												strlen(CAPSULE_MSG_1),
												strlen(CAPSULE_MSG_2),
												strlen(CAPSULE_MSG_3),
												strlen(CAPSULE_MSG_4),
												strlen(CAPSULE_MSG_5),
												strlen(CAPSULE_MSG_6),
												strlen(CAPSULE_MSG_7),
												strlen(CAPSULE_MSG_8),
												strlen(CAPSULE_MSG_9),
												strlen(CAPSULE_MSG_10),
												strlen(CAPSULE_MSG_11),
												strlen(CAPSULE_MSG_12),
												strlen(CAPSULE_MSG_13),
												strlen(CAPSULE_MSG_14),
												strlen(CAPSULE_MSG_15)};
//SPI Flash messages sizes

//SPI Flash write sequence list
const char *TO_WRITE_LIST[] = {					FROM,
												MENTOR,
												DATE,
												CAPSULE_MSG_1,
												CAPSULE_MSG_2,
												CAPSULE_MSG_3,
												CAPSULE_MSG_4,
												CAPSULE_MSG_5,
												CAPSULE_MSG_6,
												CAPSULE_MSG_7,
												CAPSULE_MSG_8,
												CAPSULE_MSG_9,
												CAPSULE_MSG_10,
												CAPSULE_MSG_11,
												CAPSULE_MSG_12,
												CAPSULE_MSG_13,
												CAPSULE_MSG_14,
												CAPSULE_MSG_15,};
//SPI Flash write sequence list

//Local variables and arrays
uint8_t address_tx[3] = {0};							//Array for byte or block address
char USR_ENTER = 0;										//Value entered by user
char RECEIVED_DATA[100] = {0};							//Received data from SPI Flash
uint8_t RX_LEN = sizeof(RECEIVED_DATA);					//Length of messages
//Local variables and arrays

void HOME_SCREEN(void){																									//Main user interact function
	while(1){
		SPI_DISABLE_WP();
		HAL_UART_Transmit(&huart3, (uint8_t *)WELCOME_MSG, strlen(WELCOME_MSG), 1000);
		HAL_UART_Receive(&huart3, (uint8_t *)&USR_ENTER, 1, 100000);
		switch (USR_ENTER){
			case 0:
				HAL_UART_Transmit(&huart3, (uint8_t *)TIMEOUT_ERROR_MSG, strlen(TIMEOUT_ERROR_MSG), 1000);				//Send error message if occurred timeout
				break;
			case '1':
				SPI_READ();
				break;
			case '2':
				SPI_ERASE();
				break;
			case '3':
				SPI_WRITE();
				break;
			default:
				HAL_UART_Transmit(&huart3, (uint8_t *)WRONG_COMMAND_MSG, strlen(WRONG_COMMAND_MSG), 1000);				//Send error message if symbol is unknown
				break;
		}
		USR_ENTER = 0;
	}
}



void SPI_WREN(void){																									//Write enable
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_WRITE_ENABLE, 1, 1000 );
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(10);
}

/* 	Temporary unused
void SPI_WRDI(void){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_WRITE_DISABLE, 1, 1000 );
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
}
	Temporary unused */

void SPI_DISABLE_WP(void){																								//Disable flash write protection
	SPI_WREN();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_EWSR, 1, 1000 );															//Sending enable write status register command
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_WRITE_STATUS_REGISTER, 1, 1000 );										//Sending write status register command
	HAL_SPI_Transmit(&hspi1, 0, 1, 1000 );																				//Sending 0 to disable all write protection
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
}


void SPI_READ(void){																									//Read data from flash
	for(uint16_t block = 0; block < 524; block++){
		ADDRESS_MAKER(block, 0);
		uint8_t GARBAGE_CNTR = 0;
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_READ, 1, 1000 );														//Sending Flash read command
		HAL_SPI_Transmit(&hspi1, (uint8_t *) address_tx, 3, 1000 );														//Sending read address
		HAL_SPI_Receive(&hspi1, (uint8_t *) RECEIVED_DATA, RX_LEN, 1000 );												//Receive data from flash
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		for(uint8_t litheral = 0; litheral < RX_LEN; litheral++){
			if(RECEIVED_DATA[litheral] == 0xFF || RECEIVED_DATA[litheral] == '\n' || RECEIVED_DATA[litheral] == '\n'){	//Clear garbage from received message
				RECEIVED_DATA[litheral] = 0;
				GARBAGE_CNTR++;
			}
		}
		if(GARBAGE_CNTR != 100){
			RECEIVED_DATA[RX_LEN - 1] = '\r';
			RECEIVED_DATA[RX_LEN - 2] = '\n';
		}
		GARBAGE_CNTR = 0;
		HAL_UART_Transmit(&huart3, (uint8_t *)RECEIVED_DATA, RX_LEN, 1000);
	}


}

void SPI_ERASE(void){																									//SPI Flash eraser
	SPI_WREN();
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_ERASE_ALL, 1, 1000 );													//Start erase
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart3, (uint8_t *)ERASE_IN_PROGRESS_MSG, strlen(ERASE_IN_PROGRESS_MSG), 1000);					//Sending erase start message
	uint8_t work = 1;
	while (work == 1){																									//While erase in progress
		uint8_t flash_status_register = 0;
		HAL_Delay(50);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_READ_STATUS, 1, 1000 );												//Send read status register command
		HAL_SPI_Receive(&hspi1, (uint8_t *)&flash_status_register, 1, 1000 );											//Receive status register data
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		if((flash_status_register & 0b00000001) == 0){																	//If busy bit equals 0
			HAL_UART_Transmit(&huart3, (uint8_t *)ERASE_COMPLETE_MSG, strlen(ERASE_COMPLETE_MSG), 1000);				//Send erase complete message
			work = 0;																									//End cycle
		} else{
			HAL_UART_Transmit(&huart3, (uint8_t *)&WAITING_MSG, 1, 1000);												//Sent . to UART
		}
	}

}

void SPI_WRITE(void){																									//SPI Flash writer
	HAL_UART_Transmit(&huart3, (uint8_t *)WRITE_IN_PROGRESS_MSG, strlen(WRITE_IN_PROGRESS_MSG), 1000);					//Sending write in progress message
	for(uint8_t line = 1; line < sizeof(SIZES_FOR_TO_WRITE_LIST)+1; line++){											//For every message in write sequence; Start from second 4Kbyte block
		for(uint8_t byte = 0; byte < SIZES_FOR_TO_WRITE_LIST[line-1]; byte++){											//For every byte of sending data
			SPI_WREN();
			ADDRESS_MAKER(line, byte);																					//Generate address
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
			HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_WRITE_BYTE, 1, 1000 );											//Sending write command
			HAL_SPI_Transmit(&hspi1, (uint8_t *) address_tx, 3, 1000 );													//Sending write address
			char a = *(*(&TO_WRITE_LIST[line - 1])+byte);																//Data to send
			HAL_SPI_Transmit(&hspi1, (uint8_t *)&a, 1, 1000 );
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_Delay(30);
		}
		HAL_UART_Transmit(&huart3, (uint8_t *)&WAITING_MSG, 1, 1000);													//After every sended message send one ".

	}
	HAL_UART_Transmit(&huart3, (uint8_t *)WRITE_COMPLETE_MSG, strlen(WRITE_COMPLETE_MSG), 1000);
}

void ADDRESS_MAKER(uint16_t block, uint16_t byte){
	uint32_t address = (block*4000) + byte;
	address_tx[0] = address >> 16;
	address_tx[1] =	address >> 8,
	address_tx[2] =	address;
}

/*Code for tests with one byte of data
void SPI_READE_BYTE(void){
		ADDRESS_MAKER(1, 0);
		uint8_t TX[4];
		TX[0] = FLASH_READ;
		TX[1] = address_tx[0];
		TX[2] = address_tx[1];
		TX[3] = address_tx[2];
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
		//HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_READ, 1, 1000 );
		HAL_SPI_Transmit(&hspi1, (uint8_t *) TX, 4, 1000 );
		HAL_SPI_Receive(&hspi1, (uint8_t *) received_data, 6, 1000 );
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart3, (uint8_t *)received_data, 6, 1000);


}

void SPI_ERASE_BYTE(void){
	SPI_WREN();
	ADDRESS_MAKER(1, 0);
	uint8_t TX[4];
	TX[0] = FLASH_ERASE_4K;
	TX[1] = address_tx[0];
	TX[2] = address_tx[1];
	TX[3] = address_tx[2];
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	//HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_ERASE_4K, 1, 1000 );
	HAL_SPI_Transmit(&hspi1, (uint8_t *) TX, 4, 1000 );
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	//SPI_WRDI();

	HAL_Delay(500);

}

void SPI_WRITE_BYTE(void){
	SPI_WREN();
	ADDRESS_MAKER(1, 5);
	uint8_t TX[5];
	TX[0] = FLASH_WRITE_BYTE;
	TX[1] = address_tx[0];
	TX[2] = address_tx[1];
	TX[3] = address_tx[2];
	TX[4] = 0b01010101;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

	//HAL_SPI_Transmit(&hspi1, (uint8_t *)&FLASH_WRITE_BYTE, 1, 1000 );
	HAL_SPI_Transmit(&hspi1, (uint8_t *) TX, 5, 1000 );
	//HAL_SPI_Transmit(&hspi1, (uint8_t *)&litheral, 1, 1000 );
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_Delay(30);
	//SPI_WRDI();
}
Code for test with one byte of data*/


