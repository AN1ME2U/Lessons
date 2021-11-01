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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */
extern TIM_HandleTypeDef htim4;
/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t mode = 4;
uint8_t steps_num = 1;
uint8_t duty = 0;
const uint16_t ZERO = 0x000;																		//Reset value of the register
const uint16_t freq = 500;																			//APB timer clock freq/100000
const uint16_t step = 5;																			//PWM step in KHz
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

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
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	if(EXTI->PR & EXTI_PR_PR6){
		(duty != 100)?(duty += 5) : (duty = 0);																	//Increase duty cycle
		TIM4->CCR1 &= ZERO;																						//Reset CCR register
		TIM4->CCR2 &= ZERO;
		TIM4->CCR3 &= ZERO;
		TIM4->CCR4 &= ZERO;
		TIM4->CCR1 |= duty;																						//Write duty cycle value to the CCR register
		TIM4->CCR2 |= duty;
		TIM4->CCR3 |= duty;
		TIM4->CCR4 |= duty;
	}
	if(EXTI->PR & EXTI_PR_PR8){
		(duty != 0)?(duty -= 5) : (duty = 0);																	//Decrease duty cycle
		TIM4->CCR1 &= ZERO;
		TIM4->CCR2 &= ZERO;																						//Reset CCR register
		TIM4->CCR3 &= ZERO;
		TIM4->CCR4 &= ZERO;
		TIM4->CCR1 |= duty;																						//Write duty cycle value to the CCR register
		TIM4->CCR2 |= duty;
		TIM4->CCR3 |= duty;
		TIM4->CCR4 |= duty;
	}
	if(EXTI->PR & EXTI_PR_PR9){
		(steps_num != 1)?(steps_num--) : (steps_num = 1);														//Decrease number of steps value. Minimum value is 1
		uint16_t presc = (freq/(step*steps_num));																//Calculate timer prescaler value
			TIM4->PSC &= ZERO;																					//Reset PRC register
			TIM4->PSC |= presc-1;																				//Write prescaler value to the PRC register



	}
  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */
	if(EXTI->PR & EXTI_PR_PR11){
		steps_num++;																							//Increase number of steps value
		uint16_t presc = (freq/(step*steps_num));																//Calculate timer prescaler value
		TIM4->PSC &= ZERO;																						//Reset PRC register
		TIM4->PSC |= presc-1;																					//Write prescaler value to the PRC register

	}
	if(EXTI->PR & EXTI_PR_PR15){
		(mode == 4)?(mode = 0) : (mode++);																		//Select next mode in circle
		switch (mode){
			case 0:
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);														//Disable PWM on channel 4
				break;
			case 1:
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);														//Start PWM on channel 1
				break;
			case 2:
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);														//Disable PWM on channel 1
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);														//Start PWM on channel 2
				break;
			case 3:
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);														//Disable PWM on channel 2
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);														//Start PWM on channel 3
				break;
			case 4:
				HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);														//Disable PWM on channel 3
				HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);														//Start PWM on channel 4
				break;
		}
	}
  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
