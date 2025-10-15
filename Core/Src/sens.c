/*
 * sens.c
 *
 *  Created on: 16. 5. 2022
 *      Author: slabv
 */


#include "sens.h"



void ultrasound(void)
{

	for (int i = 0; i<4;i++)
	{
	  if (buff[i] == 0xff)
	  {
		  switch(i){
		  case 0:
			  if(buff[i+1] == 0xff){
				  break;
			  }
			  val = ((uint16_t)buff[i+1] << 8) | buff[i+2];
			  break;
		  case 1:
			  if(buff[i+1] == 0xff){
				  break;
			  }
			  else if(buff[i-1] == 0xff){
				  break;
			  }
			  val = ((uint16_t)buff[i+1] << 8) | buff[i+2];
			  break;
		  case 2:
			  if(buff[i+1] == 0xff){
				  break;
			  }
			  else if(buff[i-1] == 0xff){
				  break;
			  }
			  val = ((uint16_t)buff[i+1] << 8) | buff[0];
			  break;
		  case 3:
			  if(buff[i+1] == 0xff){
				  break;
			  }
			  else if(buff[i-1] == 0xff){
				  break;
			  }
			  val = ((uint16_t)buff[0] << 8) | buff[i+1];
			  break;
		  default:
			  break;

		  }
	  }
	}
}

void ADC_Select_CH12 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_12;
	  sConfig.Rank = 2;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH13 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_13;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
void ADC_Select_CH10 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_10;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void ADC_Select_CH11 (void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
	  sConfig.Channel = ADC_CHANNEL_11;
	  sConfig.Rank = 2;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
}
