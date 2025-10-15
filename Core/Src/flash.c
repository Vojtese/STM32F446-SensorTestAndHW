/*
 * flash.c
 *
 *  Created on: 9. 4. 2022
 *      Author: slabv
 */

/* Includes */
#include "flash.h"

/* For F4 processors with sector memroy a FLASH_erase(sectorNB, VoltageRange)  is to use with unlock and lock fcn */
/**
 * @brief   This function erases the memory.
 * @param   address: First address to be erased (the last is the end of the flash).
 * @param	APP_FLESH_END_ADDRESS: is the end address fo the flash area to be erased
 * @retval  status: Report about the success of the erasing.
 */
flash_status flash_erase(uint32_t address, uint32_t APP_FLASH_END_ADDRESS)
{
  HAL_FLASH_Unlock();

  flash_status status = FLASH_ERROR;
  FLASH_EraseInitTypeDef erase_init;
  uint32_t error = 0u;

  /*erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init.PageAddress = address;*/
  /* Calculate the number of pages from "address" and the end of flash. */
  /*erase_init.NbPages = (APP_FLASH_END_ADDRESS - address) / PAGE_SIZE;*/
  /* Do the actual erasing. */
  if (HAL_OK == HAL_FLASHEx_Erase(&erase_init, &error))
  {
    status = FLASH_OK;
  }

  HAL_FLASH_Lock();

  return status;
}

/**
 * @brief   This function flashes the memory.
 * @param   address: First address to be written to.
 * @param   *data:   Array of the data that we want to write.
 * @param   *numofwords: Size of the array.
 * @retval  status: Report about the success of the writing.
 */
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t numofwords)
{
  flash_status status = FLASH_OK;

  HAL_FLASH_Unlock();

  /* Loop through the array. */
  for (uint32_t i = 0u; (i < numofwords) && (FLASH_OK == status); i++)
  {
    /* If we reached the end of the memory, then report an error and don't do anything else.*/
    if ((uint32_t)FLASH <= address)
    {
      status |= FLASH_ERROR_SIZE;
    }
    else
    {
      /* The actual flashing. If there is an error, then report it. */
      if (HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data[i]))
      {
        status |= FLASH_ERROR_WRITE;
      }
      /* Read back the content of the memory. If it is wrong, then report an error. */
      if (((data[i])) != (*(volatile uint32_t*)address))
      {
        status |= FLASH_ERROR_READBACK;
      }

      /* Shift the address by a word. */
      address += 4u;
    }
  }

  HAL_FLASH_Lock();

  return status;
}
/**
  *  @brief   Function that reads Flash data.
  *  @param   address: where to start the reading the data
  *  @param   *data:   Array of the data where we want to read
  *  @param   numofwords: number of words of the desired array
  *  @retval  None
  */
void flash_read (uint32_t address, uint32_t *data, uint32_t numofwords)
{
	while (1)
	{

		*data = *(__IO uint32_t *)address;
		address += 4;
		data++;
		if (!(numofwords--)) break;
	}
}
