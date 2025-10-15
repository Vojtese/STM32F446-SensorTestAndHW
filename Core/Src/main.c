/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "iap.h"
#include "flash.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*Defines*/
/**
 * @brief           Calculate length of statically allocated array
 */
#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))
#define STRUCT_ADDRERSS 0x08004800u


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

DAC_HandleTypeDef hdac;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* variables for iap*/
/**
  * @brief           USART RX buffer for DMA to transfer every received byte
  * @note            Contains raw data that are about to be processed by different events
  */
uint8_t usart_rx_dma_buffer[245];
uint32_t flashBuffer[60];
uint32_t flashBufferCRC[60];
uint8_t crcRes;
uint8_t cnt1 = 0;
uint8_t cnt2 = 0;
uint8_t cnt3 = 0;
uint8_t ind = 0;
uint32_t addrCNT = 0;
uint32_t addrCNTEND = 0;
/*DS18B20 vars*/
uint8_t Presence = 0;
uint8_t Temp_byte1;
uint8_t Temp_byte2;
uint16_t TEMP;
extern uint16_t val;
extern uint8_t buff[4];
uint32_t Is;
uint32_t Us;
uint32_t hdl;
uint32_t diff;

uint32_t vADC1[2];
uint32_t vADC2[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_DAC_Init(void);
static void MX_UART4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

void usart_rx_check(void);
void usart_process_data(const void* data, size_t len);
uint8_t gencrc(uint8_t crc, uint8_t Size, uint8_t *Buffer);
void DS18B20_acq(void);
void DS18B20_Write (uint8_t data);
void delay (uint32_t us);
uint8_t DS18B20_Start (void);
uint8_t read (void);
void gpio_set_input(void);
void gpio_set_output(void);
extern void ultrasound(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	/*IAP*/
	IAPSTRUCT structREAD;
	uint32_t *iapStructREAD_p = (uint32_t *)&structREAD;
	uint32_t numofwords = 12;
	uint8_t vals[8];


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  /*DS18B20 for delay in us*/


  /* IAP */
  flash_read((uint32_t)STRUCT_ADDRERSS, iapStructREAD_p, numofwords);
  addrCNT = structREAD.ADDRESS_APP_2_START;
  addrCNTEND = structREAD.ADDRESS_APP_2_END;

  /* ADC1-2 */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)vADC1, 2);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)vADC2, 2);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 2000);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 1000);


  HAL_TIM_Base_Start(&htim6);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_ADC_Stop_DMA(&hadc1);
	  HAL_ADC_Stop_DMA(&hadc2);
	  //DS18B20_acq();
	  Presence = DS18B20_Start ();
	  HAL_Delay (1);
	  DS18B20_Write (0xCC);  // skip ROM
	  DS18B20_Write (0x44);  // convert t
	  HAL_Delay (800);

	  Presence = DS18B20_Start ();
	  HAL_Delay(1);
	  DS18B20_Write (0xCC);  // skip ROM
	  DS18B20_Write (0xBE);  // Read Scratch-pad

	  Temp_byte1 = read();
	  Temp_byte2 = read();
	  TEMP = (Temp_byte2<<8) | Temp_byte1;


	  HAL_UART_Receive(&huart1, buff, 4, 100u);
	  ultrasound();

	  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)vADC1, 2);
	  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)vADC2, 2);

	  Is = vADC1[0];
	  Us = vADC1[1];
	  hdl = vADC2[0];
	  diff = vADC2[1];




	  vals[0] = HAL_GPIO_ReadPin(GPIOB, P1_INPUT_Pin);
	  vals[1] = HAL_GPIO_ReadPin(GPIOB, P2_INPUT_Pin);
	  vals[2] = HAL_GPIO_ReadPin(GPIOC, P3_INPUT_Pin);
	  vals[3] = HAL_GPIO_ReadPin(GPIOC, P4_INPUT_Pin);
	  vals[4] = HAL_GPIO_ReadPin(GPIOA, TTL1_Pin);
	  vals[5] = HAL_GPIO_ReadPin(GPIOC, TTL2_Pin);
	  vals[6] = HAL_GPIO_ReadPin(GPIOA, GPI_1_Pin);
	  vals[7] = HAL_GPIO_ReadPin(GPIOA, GPI_2_Pin);


	  printf("Temp (%d.%d)	Us:%ld	Is:%ld	HDL:%ld	DIFF:%ld	Dist:%d mm\n\rP1:%d	P2:%d	P3:%d	P4:%d	TTL1:%d	TTL2:%d	GPI1:%d	GPI2:%d\n\r",
			  TEMP/16, TEMP%16, Us, Is, hdl, diff, val, vals[0],vals[1],vals[2],vals[3],vals[4],vals[5],vals[6],vals[7]);
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  HAL_Delay(1000u);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */
  /*LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_12);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_12, LL_ADC_SAMPLINGTIME_15CYCLES);
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_13);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_13, LL_ADC_SAMPLINGTIME_3CYCLES);*/
  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ENABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 2;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */
  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 72-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 0xffff-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**UART4 GPIO Configuration
  PA0-WKUP   ------> UART4_TX
  PA1   ------> UART4_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* UART4 DMA Init */

  /* UART4_RX Init */
  LL_DMA_SetChannelSelection(DMA1, LL_DMA_STREAM_2, LL_DMA_CHANNEL_4);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_2, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_STREAM_2, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_2, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_2);

  /* UART4 interrupt Init */
  NVIC_SetPriority(UART4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(UART4_IRQn);

  /* USER CODE BEGIN UART4_Init 1 */
  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_STREAM_2, LL_USART_DMA_GetRegAddr(UART4));
  LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)usart_rx_dma_buffer);
  LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, ARRAY_LEN(usart_rx_dma_buffer));

  /* Enable HT & TC interrupts for RX */
  LL_DMA_EnableIT_HT(DMA1, LL_DMA_STREAM_2);
  LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
  /* USER CODE END UART4_Init 1 */
  USART_InitStruct.BaudRate = 38400;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(UART4, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(UART4);
  LL_USART_Enable(UART4);
  /* USER CODE BEGIN UART4_Init 2 */
  LL_USART_EnableDMAReq_RX(UART4);
  LL_USART_EnableIT_IDLE(UART4);

  /* Enable USART and DMA */
  LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA2);

  /* DMA interrupt init */
  /* DMA1_Stream2_IRQn interrupt configuration */
  NVIC_SetPriority(DMA1_Stream2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(DMA1_Stream2_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GPI_1_Pin GPI_2_Pin TTL1_Pin */
  GPIO_InitStruct.Pin = GPI_1_Pin|GPI_2_Pin|TTL1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : P1_INPUT_Pin P2_INPUT_Pin */
  GPIO_InitStruct.Pin = P1_INPUT_Pin|P2_INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : P3_INPUT_Pin P4_INPUT_Pin TTL2_Pin */
  GPIO_InitStruct.Pin = P3_INPUT_Pin|P4_INPUT_Pin|TTL2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DS18B20_Pin */
  GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
  *	@brief	Check the new data coming in uart
  *
  *
  *	@param	None
  *	@retval None
  *
  */
void usart_rx_check(void)
{
	/* Set up previous position  as stayic variable -> Default linker sets this with value 0
	 * It serves as latest start position
	 */
	static size_t prev_pos;
	size_t pos;

	/* calculation of current position in buffer and look for new data coming */
	pos = ARRAY_LEN(usart_rx_dma_buffer) - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_2);
	if (pos != prev_pos) {				/* checks change in received data */
		if (pos>prev_pos){				/* Current position is further than previous one */
			/*
			             * Processing is done in "linear" mode.
			             *
			             * Application processing is fast with single data block,
			             * length is simply calculated by subtracting pointers
			             *
			             * [   0   ]
			             * [   1   ] <- old_pos |------------------------------------|
			             * [   2   ]            |                                    |
			             * [   3   ]            | Single block (len = pos - old_pos) |
			             * [   4   ]            |                                    |
			             * [   5   ]            |------------------------------------|
			             * [   6   ] <- pos
			             * [   7   ]
			             * [ N - 1 ]
			             */
			            usart_process_data(&usart_rx_dma_buffer[prev_pos], pos - prev_pos);
		} else {
            /*
             * Processing is done in "overflow" mode..
             *
             * Application must process data twice,
             * since there are 2 linear memory blocks to handle
             *
             * [   0   ]            |---------------------------------|
             * [   1   ]            | Second block (len = pos)        |
             * [   2   ]            |---------------------------------|
             * [   3   ] <- pos
             * [   4   ] <- old_pos |---------------------------------|
             * [   5   ]            |                                 |
             * [   6   ]            | First block (len = N - old_pos) |
             * [   7   ]            |                                 |
             * [ N - 1 ]            |---------------------------------|
             */
            usart_process_data(&usart_rx_dma_buffer[prev_pos], ARRAY_LEN(usart_rx_dma_buffer) - prev_pos);
            if (pos > 0) {
                usart_process_data(&usart_rx_dma_buffer[0], pos);
            }
		}
		prev_pos = pos;						/* Save current position as old for next transfers */
	}
}
/**
  *	@brief	Processing data from uart
  *	@note	Processing directly or copy to bigger buffer
  *	@param	*data: data to process
  *	@param	len: length in units of bytes
  *	@retval	None
  */
void usart_process_data(const void *data, size_t len) {
	const uint8_t* d = data;
	cnt1++;
	/*
	 * This function is called on DMA TC or HT events, and on UART IDLE (if enabled) event.
	 *
	 * For the sake of this example, function does a loop-back data over UART in polling mode.
	 * Check ringbuff RX-based example for implementation with TX & RX DMA transfer.
	 */
	if (cnt1 > 4)
	{

		if(cnt3 == usart_rx_dma_buffer[3] && cnt3 != 0)
		{
			if ((cnt2 + 1) % 4 == 0 && cnt2 != 0)
			{
				flashBufferCRC[ind] =
						((usart_rx_dma_buffer[cnt1 - 4] & 0xFF) <<0) |
						((usart_rx_dma_buffer[cnt1 - 4 + 1] & 0xFF) << 8) |
						((usart_rx_dma_buffer[cnt1 - 4 + 2] & 0xFF) << 16) |
						((long)(usart_rx_dma_buffer[cnt1 - 4 + 3] & 0xFF) << 24);
				ind++;
			}
			cnt2++;

			if (usart_rx_dma_buffer[0] == 0)		/* Header is 0x00 */
			{
				for (; len > 0; --len, ++d) {
					LL_USART_TransmitData8(UART4, 1);
					while (!LL_USART_IsActiveFlag_TXE(UART4)) {}
				}
				while (!LL_USART_IsActiveFlag_TC(UART4)) {}
			}
			if ((usart_rx_dma_buffer[1]+4 < cnt1) && (cnt1 < usart_rx_dma_buffer[1]+6))
			{
				crcRes = gencrc(0x00,usart_rx_dma_buffer[1],(uint8_t *)flashBufferCRC);
				/* CRC here */
 				if(crcRes == usart_rx_dma_buffer[cnt1-1])
				{
					flash_write(addrCNT, flashBufferCRC, (uint32_t)((usart_rx_dma_buffer[1])/4));
					memset(usart_rx_dma_buffer, 0, sizeof(usart_rx_dma_buffer));
					addrCNT += (uint32_t)((usart_rx_dma_buffer[1])/4);
					//memset(flashBuffer, 0, sizeof(flashBuffer));
					//memset(flashBufferCRC, 0, sizeof(flashBufferCRC));
					cnt1 = 0;
					cnt2 = 0;
					cnt3++;
					ind = 0;

					IAPSTRUCT structREAD;
					uint32_t *iapStructREAD_p = (uint32_t *)&structREAD;
					uint32_t numofwords = 12;
					uint32_t initBL;
					uint32_t *p_BL = &initBL;
					flash_read((uint32_t)STRUCT_ADDRERSS, iapStructREAD_p, numofwords);
					structREAD.ADDRESS_TO_JUMP = structREAD.ADDRESS_APP_1_START;
					numofwords = 12;
					HAL_FLASH_Unlock();
					FLASH_Erase_Sector(1, FLASH_VOLTAGE_RANGE_3);
					HAL_FLASH_Lock();
					flash_write((uint32_t)STRUCT_ADDRERSS, iapStructREAD_p, numofwords);
					initBL = 1;
					flash_write((uint32_t)0x08004000u, p_BL, 1);
				}
 				NVIC_SystemReset();
			}

		}
		else{
			if ((cnt2 + 1) % 4 == 0 && cnt2 != 0)
			{
				flashBufferCRC[ind] =
						((usart_rx_dma_buffer[cnt1 - 4] & 0xFF) <<0) |
						((usart_rx_dma_buffer[cnt1 - 4 + 1] & 0xFF) << 8) |
						((usart_rx_dma_buffer[cnt1 - 4 + 2] & 0xFF) << 16) |
						((long)(usart_rx_dma_buffer[cnt1 - 4 + 3] & 0xFF) << 24);
				ind++;
			}
			cnt2++;

			if (usart_rx_dma_buffer[0] == 0)		/* Header is 0x00 */
			{
				for (; len > 0; --len, ++d) {
					LL_USART_TransmitData8(UART4, 1);
					while (!LL_USART_IsActiveFlag_TXE(UART4)) {}
				}
				while (!LL_USART_IsActiveFlag_TC(UART4)) {}
			}
			if ((244< cnt1) && (cnt1 <246))
			{
				crcRes = gencrc(0x00,sizeof(flashBufferCRC),(uint8_t *)flashBufferCRC);
				if(crcRes == usart_rx_dma_buffer[cnt1-1])
				{
					if (cnt3 == 0)
					{
						HAL_FLASH_Unlock();
						FLASH_Erase_Sector(4, FLASH_VOLTAGE_RANGE_3);
						FLASH_Erase_Sector(5, FLASH_VOLTAGE_RANGE_3);
						HAL_FLASH_Lock();
					}
					flash_write(addrCNT, flashBufferCRC, (uint32_t)((usart_rx_dma_buffer[1])/4));
					memset(usart_rx_dma_buffer, 0, sizeof(usart_rx_dma_buffer));
					addrCNT += sizeof(flashBuffer);
					memset(flashBuffer, 0, sizeof(flashBuffer));
					cnt1 = 0;
					cnt2 = 0;
					cnt3++;
					ind = 0;

				}
			}

		}

	}
	else{
		if (usart_rx_dma_buffer[0] == 0)		/* Header is 0x00 */
		{
			for (; len > 0; --len, ++d) {
				LL_USART_TransmitData8(UART4, 1);
				while (!LL_USART_IsActiveFlag_TXE(UART4)) {}
			}
			while (!LL_USART_IsActiveFlag_TC(UART4)) {}
		}
	}


}
/**
  *	@brief	CRC 8-bit for packet integrity
  *	@param	*data: of 0-59B
  *	@param	length:	length of the data received
  *	@retval	CRC number
  */
uint8_t gencrc(uint8_t crc, uint8_t Size, uint8_t *Buffer)
{
  static const uint8_t CrcTable[] = { // 0x07 Polynomial Table, 8-bit,
		  0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,
		  0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,
		  0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,
		  0x5f,0x01,0xe3,0xbd,0x3e,0x60,0x82,0xdc,
		  0x23,0x7d,0x9f,0xc1,0x42,0x1c,0xfe,0xa0,
		  0xe1,0xbf,0x5d,0x03,0x80,0xde,0x3c,0x62,
		  0xbe,0xe0,0x02,0x5c,0xdf,0x81,0x63,0x3d,
		  0x7c,0x22,0xc0,0x9e,0x1d,0x43,0xa1,0xff,
		  0x46,0x18,0xfa,0xa4,0x27,0x79,0x9b,0xc5,
		  0x84,0xda,0x38,0x66,0xe5,0xbb,0x59,0x07,
		  0xdb,0x85,0x67,0x39,0xba,0xe4,0x06,0x58,
		  0x19,0x47,0xa5,0xfb,0x78,0x26,0xc4,0x9a,
		  0x65,0x3b,0xd9,0x87,0x04,0x5a,0xb8,0xe6,
		  0xa7,0xf9,0x1b,0x45,0xc6,0x98,0x7a,0x24,
		  0xf8,0xa6,0x44,0x1a,0x99,0xc7,0x25,0x7b,
		  0x3a,0x64,0x86,0xd8,0x5b,0x05,0xe7,0xb9,
		  0x8c,0xd2,0x30,0x6e,0xed,0xb3,0x51,0x0f,
		  0x4e,0x10,0xf2,0xac,0x2f,0x71,0x93,0xcd,
		  0x11,0x4f,0xad,0xf3,0x70,0x2e,0xcc,0x92,
		  0xd3,0x8d,0x6f,0x31,0xb2,0xec,0x0e,0x50,
		  0xaf,0xf1,0x13,0x4d,0xce,0x90,0x72,0x2c,
		  0x6d,0x33,0xd1,0x8f,0x0c,0x52,0xb0,0xee,
		  0x32,0x6c,0x8e,0xd0,0x53,0x0d,0xef,0xb1,
		  0xf0,0xae,0x4c,0x12,0x91,0xcf,0x2d,0x73,
		  0xca,0x94,0x76,0x28,0xab,0xf5,0x17,0x49,
		  0x08,0x56,0xb4,0xea,0x69,0x37,0xd5,0x8b,
		  0x57,0x09,0xeb,0xb5,0x36,0x68,0x8a,0xd4,
		  0x95,0xcb,0x29,0x77,0xf4,0xaa,0x48,0x16,
		  0xe9,0xb7,0x55,0x0b,0x88,0xd6,0x34,0x6a,
		  0x2b,0x75,0x97,0xc9,0x4a,0x14,0xf6,0xa8,
		  0x74,0x2a,0xc8,0x96,0x15,0x4b,0xa9,0xf7,
		  0xb6,0xe8,0x0a,0x54,0xd7,0x89,0x6b,0x35  };

  while(Size--)
  {
    crc =   *Buffer++ ^ crc; // Apply Byte

    crc = CrcTable[crc & 0xFF]; // One round of 8-bits
  }

  return(crc);
}
/**
  *	@brief DS18B20 ACQ fcn
  */

void DS18B20_acq(void)
{
	Presence = DS18B20_Start ();
	HAL_Delay (1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0x44);  // convert t
	HAL_Delay (800);

	Presence = DS18B20_Start ();
	HAL_Delay(1);
	DS18B20_Write (0xCC);  // skip ROM
	DS18B20_Write (0xBE);  // Read Scratch-pad

	Temp_byte1 = read();
	Temp_byte2 = read();
	TEMP = (Temp_byte2<<8) | Temp_byte1;



}
void delay (uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim6,0);
    while ((__HAL_TIM_GET_COUNTER(&htim6))<us);
}

void DS18B20_Write (uint8_t data)
{
	gpio_set_output();

	for (int i=0; i<8; i++)
	{

		if ((data & (1<<i))!=0)  // if the bit is high
		{
			// write 1

			gpio_set_output();
			HAL_GPIO_WritePin (GPIOC, DS18B20_Pin, 0);  // pull the pin LOW
			delay (1);  // wait for 1 us

			gpio_set_input();
			delay (60);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			gpio_set_output();
			HAL_GPIO_WritePin (GPIOC, DS18B20_Pin, 0);  // pull the pin LOWi
			delay (60);  // wait for 60 us

			gpio_set_input();
		}
	}
}
uint8_t DS18B20_Start (void)
{
	uint8_t Response = 0;
	gpio_set_output();
	HAL_GPIO_WritePin (GPIOC, DS18B20_Pin, 0);  // pull the pin low
	delay (480);   // delay according to datasheet

	gpio_set_input();
	delay (80);    // delay according to datasheet

	if (!(HAL_GPIO_ReadPin (GPIOC, DS18B20_Pin))) Response = 1;    // if the pin is low i.e the presence pulse is detected
	else Response = -1;

	delay (400); // 480 us delay totally.

	return Response;
}
uint8_t read (void)
{
	uint8_t value=0;
	gpio_set_input ();

	for (int i=0;i<8;i++)
	{
		gpio_set_output ();   // set as output

		HAL_GPIO_WritePin (GPIOC, DS18B20_Pin, 0);  // pull the data pin LOW
		delay (2);  // wait for 2 us

		gpio_set_input ();  // set as input
		if (HAL_GPIO_ReadPin (GPIOC, DS18B20_Pin))  // if the pin is HIGH
		{
			value |= 1<<i;  // read = 1
		}
		delay (60);  // wait for 60 us
	}
	return value;
}
void gpio_set_input(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
void gpio_set_output(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}
/**
  * @brief Print the characters to UART (printf).
  * @retval int
  */
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the UART3 and Loop until the end of transmission */
  LL_USART_TransmitData8(UART4, (uint8_t)ch);
  HAL_Delay(2);
  return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
