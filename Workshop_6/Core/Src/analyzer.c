#include "main.h"
#include "stm32f4xx_it.h"
#include "stdio.h"
#include "string.h"
#include "analyzer.h"
#include "leds.h"

extern UART_HandleTypeDef huart3;
extern char RX[20];
extern char NEW_LINE;

uint8_t MATCHES = 0;
uint8_t VALUES[2] = {0};
uint8_t work = 0;
uint8_t selector = 0;
char UNkNOWN_COM[19] = {"Unknown command!\r\n"};
char MESG[170] = {"Welcome to LED control interface\r\nTo set PWM duty cycle press 1\r\nTo set PRESCALLER value press 2\r\nTo operate sleep mode press 3\r\nUse +- and Enter for setup\r\n"};
char MODE = 0;
char RX_ADJ = 0;
char MASK1[3] = {"led"};
char TX[50] = {0};

void MESSAGE_ANALYZER(void){																			//Compare input buffer with example if finds '/'
	if(RX[0] == '/'){
		for(uint8_t pos = 0; pos < 3; pos++){
			if(RX[1+pos] == MASK1[pos]){
				MATCHES++;
			}
		}
		if(MATCHES == 3){
			MATCHES = 0;
			HID();
		} else{
			HAL_UART_Transmit(&huart3, (uint8_t *)UNkNOWN_COM, strlen(UNkNOWN_COM), 1000);				//If there is no matches print unknown command mesage
		}
	} else{																								//If there is no '/' print unknown command mesage
		HAL_UART_Transmit(&huart3, (uint8_t *)UNkNOWN_COM, strlen(UNkNOWN_COM), 1000);
	}
}

void HID(void){																							//Mode selector
	HAL_UART_AbortReceive_IT(&huart3);
	HAL_UART_Transmit(&huart3, (uint8_t *)MESG, strlen(MESG), 1000);
	HAL_UART_Receive(&huart3, (uint8_t *)&MODE, 1, 1000);
	if(MODE == '1'){
		MODE_1();
	}else if(MODE == '2'){
		MODE_2();
	}else if (MODE == '3'){
		MODE_3();
	} else {

	}
}

void MODE_1(void){																						//Handler for mode 1
	work = 1;
	while(work == 1){
		sprintf(TX, "Set LED %d duty cycle to %d \r", VALUES[0], VALUES[1]);
		HAL_UART_Transmit(&huart3, (uint8_t *)TX, strlen(TX), 1000);
		HAL_UART_Receive(&huart3, (uint8_t *)&RX_ADJ, 1, 1000);
		if(RX_ADJ == '\r'){
			(selector != 1) ? (selector++) : (selector = 0, LED_WRITE(VALUES[0], VALUES[1]),VALUES[0] = 0, VALUES[1] = 0, work = 0);
		} else if(RX_ADJ == '-'){
			(VALUES[selector] != 0) ? (VALUES[selector]--) : (0);
		} else if(RX_ADJ == '+'){
			(selector == 0) ? ((VALUES[selector] < 16) ? (VALUES[selector]++) : (0)) : ((VALUES[selector] < 100) ? (VALUES[selector]++) : (0));
		}
	}
}

void MODE_2(void){																						//Handler for mode 2
	work = 1;
	while(work == 1){
		sprintf(TX, "Set LED PRESCALLER value to %d \r", VALUES[0]);
		HAL_UART_Transmit(&huart3, (uint8_t *)TX, strlen(TX), 1000);
		HAL_UART_Receive(&huart3, (uint8_t *)&RX_ADJ, 1, 1000);
		if(RX_ADJ == '\r'){
			LED_PRESCALLER_SET(VALUES[0]);
			VALUES[0] = 0;
			work = 0;
		} else if(RX_ADJ == '-'){
			(VALUES[0] != 0) ? (VALUES[0]--) : (0);
		} else if(RX_ADJ == '+'){
			(VALUES[0] < 254) ? (VALUES[0]++) : (0);
		}
	}
}

void MODE_3(void){																						//Handler for mode 3
	work = 1;
	while(work == 1){
		sprintf(TX, "Set LED sleep mode to %d\r", VALUES[0]);
		HAL_UART_Transmit(&huart3, (uint8_t *)TX, strlen(TX), 1000);
		HAL_UART_Receive(&huart3, (uint8_t *)&RX_ADJ, 1, 1000);
		if(RX_ADJ == '\r'){
			SLEEP_MODE(VALUES[0]);
			work = 0;
		} else if(RX_ADJ == '-' || RX_ADJ == '+'){
			VALUES[0] ^= 1 ;
		}
	}
}

