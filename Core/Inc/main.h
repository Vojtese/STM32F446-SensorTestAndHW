/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_gpio.h"

#include "stm32f4xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC2_IN10_HDL_Pin GPIO_PIN_0
#define ADC2_IN10_HDL_GPIO_Port GPIOC
#define ADC2_IN11_DIFF_Pin GPIO_PIN_1
#define ADC2_IN11_DIFF_GPIO_Port GPIOC
#define ADC1_IN12_Is_Pin GPIO_PIN_2
#define ADC1_IN12_Is_GPIO_Port GPIOC
#define ADC1_IN13_Us_Pin GPIO_PIN_3
#define ADC1_IN13_Us_GPIO_Port GPIOC
#define DAC_OUT1_HDL_Pin GPIO_PIN_4
#define DAC_OUT1_HDL_GPIO_Port GPIOA
#define DAC_OUT2_DIFF_Pin GPIO_PIN_5
#define DAC_OUT2_DIFF_GPIO_Port GPIOA
#define GPI_1_Pin GPIO_PIN_6
#define GPI_1_GPIO_Port GPIOA
#define GPI_2_Pin GPIO_PIN_7
#define GPI_2_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_10
#define LED_GPIO_Port GPIOB
#define P1_INPUT_Pin GPIO_PIN_14
#define P1_INPUT_GPIO_Port GPIOB
#define P2_INPUT_Pin GPIO_PIN_15
#define P2_INPUT_GPIO_Port GPIOB
#define P3_INPUT_Pin GPIO_PIN_6
#define P3_INPUT_GPIO_Port GPIOC
#define DS18B20_Pin GPIO_PIN_7
#define DS18B20_GPIO_Port GPIOC
#define P4_INPUT_Pin GPIO_PIN_8
#define P4_INPUT_GPIO_Port GPIOC
#define TTL2_Pin GPIO_PIN_9
#define TTL2_GPIO_Port GPIOC
#define TTL1_Pin GPIO_PIN_8
#define TTL1_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
