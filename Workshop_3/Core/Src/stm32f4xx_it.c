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

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t mode = 4;										//Текущий режим работы
uint32_t steps = 1;										//Кол-во шагов по 5КГц.
uint16_t duty = 0;										//Показатель заполненности шима. Таймер считает до 100 и сбрасывается, соответственно меняя значение регистра capture compare можно добиться изменения заполнения по процентам
uint16_t presc = 0;										//Значение, загружаемое в регистр prescale таймера 4
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
	if(EXTI->PR & EXTI_PR_PR6){														//Проверка, сработала ли кнопка 4
		if(duty != 100){
			duty += 5;																//Увеличение времени заполнения шима.
		}
		TIM4->CCR1 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR2 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR3 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR4 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR1 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR2 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR3 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR4 |= ((duty) << 0);												//Запись в регистр времени шима
	}
	if(EXTI->PR & EXTI_PR_PR8){														//Проверка, сработала ли кнопка 5
		if(duty != 0){
			duty -= 5;																//Уменьшение времени заполнения шима
		}
		TIM4->CCR1 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR2 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR3 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR4 &= ~(0xFFFF);													//Сброс регистра перед записью
		TIM4->CCR1 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR2 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR3 |= ((duty) << 0);												//Запись в регистр времени шима
		TIM4->CCR4 |= ((duty) << 0);												//Запись в регистр времени шима
	}
	if(EXTI->PR & EXTI_PR_PR9){														//Проверка, сработала ли кнопка 3
		if(steps > 1){
			steps--;
		} else{
			steps = 1;
		}
		presc = (500000/(5000*steps));												//Считаем prescaller
			TIM4->PSC &= ~(0xFFFF);													//Сброс регистра перед записью
			TIM4->PSC |= presc-1;													//Запись в регистр prescale таймера 4



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
	if(EXTI->PR & EXTI_PR_PR11){													//Проверка, сработала ли кнопка 1
		steps++;
		presc = (500000/(5000*steps));												//Считаем prescaller
		TIM4->PSC &= ~(0xFFFF);														//Сброс регистра перед записью
		TIM4->PSC |= presc-1;														//Запись в регистр prescale таймера 4

	}
	if(EXTI->PR & EXTI_PR_PR15){													//Проверка, сработала ли кнопка 2
		mode++;
		mode = (mode == 5)?0:mode;
		switch (mode){
			case 0:
				TIM4->CCER &= ~(0b0001000100010001);								//Отключаем PWM на всех каналах
				TIM4->CCER |= (1 << 12);											//Включаетм PWM на канале 4
				break;
			case 1:
				TIM4->CCER &= ~(0b0001000100010001);								//Отключаем PWM на всех каналах
				TIM4->CCER |= (1 << 8);												//Включаетм PWM на канале 3
				break;
			case 2:
				TIM4->CCER &= ~(0b0001000100010001);								//Отключаем PWM на всех каналах
				TIM4->CCER |= (1 << 4);												//Включаетм PWM на канале 2
				break;
			case 3:
				TIM4->CCER &= ~(0b0001000100010001);								//Отключаем PWM на всех каналах
				TIM4->CCER |= (1 << 0);												//Включаетм PWM на канале 1
				break;
			case 4:
				TIM4->CCER &= ~(0b0001000100010001);								//Отключаем PWM на всех каналах
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
