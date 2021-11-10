/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM4_CCR1 0x40000834U
#define TIM4_CCR2 0x40000838U
#define TIM4_CCR4 0x40000840U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint32_t ADC_RES[3] = {};																													//Array to store adc values after every measure
uint32_t *ADRESATS[3] = {(uint32_t *)TIM4_CCR2, (uint32_t *)TIM4_CCR1 ,(uint32_t *)TIM4_CCR4};												//"Address book" for CCR registers of TIM4
uint8_t extreme_conditions = 0;																												//Number of extreme conditions
uint8_t extreme_conditions_back = 0;																										//Copy of extreme conditions value for compare
const uint8_t SENSATIVITY_LEVEL_HIGH = 90;																									//High level of PWM duty
const uint8_t SENSATIVITY_LEVEL_ADC = 50;																									//ADC low sensativity level
const uint16_t ONE_HZ = 499;
const uint16_t TWO_DOT_FIVE_HZ = 399;
const uint16_t FIVE_HZ = 199;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void leds(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	if(TIM3->SR & TIM_SR_UIF){
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
	}
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
void TIM4_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	if(TIM4->SR & TIM_SR_UIF){

		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&ADC_RES, 3);																		//New measure on every tim update event
	}
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
	if(DMA2->LISR & DMA_LISR_TCIF0){
		leds();																													//Calling leds function after end of measuring
	}
  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void leds(void){
	uint16_t PWM_DUTY = 0;																										//Local value for pwm duty
	//TIM4->CCR2 &= RESET;																										//Reseting CCR registers
	//TIM4->CCR4 &= RESET;
	//TIM4->CCR1 &= RESET;
	ADC_RES[2] = 4096 - ADC_RES[2];																								//Inverting external sensor value
	for(uint8_t RANK = 0; RANK < 3; RANK++){
		(ADC_RES[RANK] < SENSATIVITY_LEVEL_ADC)?(PWM_DUTY = 1) : (PWM_DUTY = ADC_RES[RANK]/40);									//If ADC value less then senastivity lever there if no effect
		(PWM_DUTY > SENSATIVITY_LEVEL_HIGH)? (PWM_DUTY = 100, extreme_conditions++) : (0);										//PWM duty max after passing high sensativity level
		*ADRESATS[RANK] = PWM_DUTY-1;																							//Load PWM value to CCR registers by address
	}

	if(extreme_conditions != extreme_conditions_back){																			//If new num of extreme conditions same as past? there is no effect
		TIM3->ARR = RESET;																										//Reset ARR register of TIM3
		(extreme_conditions == 0)?(TIM3->CR1 |= TIM_CR1_UDIS) : (TIM3->CR1 &= ~TIM_CR1_UDIS);										//If at least one extreme conditon allow TIM3 update interrupt generation
		switch(extreme_conditions){
		case 0:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);																//If 0, dissable LED
			break;
		case 1:
			TIM3->ARR = ONE_HZ;																									//If 1, LED frequency sets to 1 Hz
			break;
		case 2:
			TIM3->ARR = TWO_DOT_FIVE_HZ;																						//If 2, LED frequency sets to 2.5 Hz
			break;
		case 3:
			TIM3->ARR = FIVE_HZ;																								//If 3, LED frequency sets to 5 Hz
			break;
		default:
			break;
		}
		extreme_conditions_back = extreme_conditions;																			//Copy num of extreme conditions for compare in next cycle
	}

	extreme_conditions = RESET;
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
