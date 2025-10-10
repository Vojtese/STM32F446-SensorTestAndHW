/*
 * iap.c
 *
 *  Created on: 31. 3. 2022
 *      Author: slabv
 */

/* Includes --------------------------
 * ----------------------------------------*/
#include "iap.h"

/* Declarations */
typedef void (*fnc_ptr)(void);

/* Definitions */

/**
  * @brief	Function, that runs an application saved in a flash memory
  * @param	address to jump to
  * @retval	None
  */
void flash_jump_to_app(uint32_t ADDRESS_TO_JUMP)
{
	/* Function, that points to user app */
	if (((*(__IO uint32_t*)ADDRESS_TO_JUMP) & 0x2FFE0000 ) == 0x20020000)
	{
		fnc_ptr jump_to_app;
		jump_to_app = (fnc_ptr)(*(__IO uint32_t*) (ADDRESS_TO_JUMP + 4u));
		//HAL_UART_DeInit(&huart2);


		HAL_DeInit();

		/* Redirecting the main stack pointer */
		__set_MSP(*(__IO uint32_t*)ADDRESS_TO_JUMP);
		jump_to_app();
	}
}

