/*
 * sens.h
 *
 *  Created on: 16. 5. 2022
 *      Author: slabv
 */

#ifndef INC_SENS_H_
#define INC_SENS_H_

#include "stm32f4xx_hal.h"
/*externs*/
extern TIM_HandleTypeDef htim6;
extern UART_HandleTypeDef huart1;
extern  ADC_HandleTypeDef hadc1;
extern  ADC_HandleTypeDef hadc2;

/* DS18B20 */

void ultrasound(void);

uint8_t buff[4];
uint16_t val;


#endif /* INC_SENS_H_ */
