/*
 * flash.h
 *
 *  Created on: 9. 4. 2022
 *      Author: slabv
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

/* Includes */
#include "stm32f4xx_hal.h"

/* FLASH STRUCTURE */

/* 0-7 sectors of 16,64,128kB per sectors */
/* 512/0x200 - quarter page */
/* 512/0x400 - half page */
/* 512/0x800 - one page */

////////////////////////////////////	-- 0x0800 0000	-- Sect 0
//                                //
//       	Bootloader			  //
//                                //	-- 0x0800 3FFF  -- end of Sect 0
////////////////////////////////////	-- 0x0800 4000	-- start of Sect 1
//                                //
//       	Config:  			  //
//       	iapStruct			  //
//                                //	-- 0x0800 FFFF	-- Sect 3 end
////////////////////////////////////	-- 0x0801 0000  -- Sect 4 start
//                                //
//       	APP 1    			  //
//                                //	-- 0x0803 FFFF	-- Sect 5 end
////////////////////////////////////	-- 0x0804 0000  -- Sect 6 start
//                                //
//       	APP 2                 //
//                                //	-- 0x0807 FFFF	-- Sect 7 end
////////////////////////////////////	-- 0x0807 8000


/* Constant definitions */

/* Application START addresses */
#define APP_1_START_FLASH_ADDRESS ((uint32_t)0x08010000u)
#define APP_2_START_FLASH_ADDRESS ((uint32_t)0x08040000u)
/* Application END addresses */
#define APP_1_END_FLASH_ADDRESS	((uint32_t)0x0803FFFFu)
#define APP_2_END_FLASH_ADDRESS	((uint32_t)0x0807FFFFu)
#define FLASH_APP_END_ADDRESS   ((uint32_t)0x0807FFFFu)	/**< Leave a little extra space at the end. */
/* Number of pages, where the program seats */
#define APP_1_FLASH_PAGE_NUMBER      (uint32_t)((APP_1_FLASH_ADDRESS - 0x08000000) >> 12)
#define APP_2_FLASH_PAGE_NUMBER      (uint32_t)((APP_2_FLASH_ADDRESS - 0x08000000) >> 12)
/* define the user application size */
#define USER_FLASH_SIZE   	(FLASH_APP_END_ADDRESS - FLASH_APP_START_ADDRESS + 1)
#define PAGE_SIZE		((uint32_t)0x800)


/* Typedef structures/enums etc... */

/* Status report for the functions. */
typedef enum {
  FLASH_OK              = 0x00u, /**< The action was successful. */
  FLASH_ERROR_SIZE      = 0x01u, /**< The binary is too big. */
  FLASH_ERROR_WRITE     = 0x02u, /**< Writing failed. */
  FLASH_ERROR_READBACK  = 0x04u, /**< Writing was successful, but the content of the memory is wrong. */
  FLASH_ERROR           = 0xFFu  /**< Generic error. */
} flash_status;
/* iapStruct */
typedef struct{
	uint32_t BOOT_VALIDITY;				/*< 0/1 - valid or invalid app to boot */
	uint32_t ADDRESS_APP_1_START;		/*< Start address of the App 1 */
	uint32_t ADDRESS_APP_2_START;		/*< Start address of the App 2 */
	uint32_t ADDRESS_APP_1_END;			/*< End address of the App 1 */
	uint32_t ADDRESS_APP_2_END;			/*< End address of the App 2 */
	uint32_t ADDRESS_TO_JUMP;			/*< What app address to jump*/
	uint32_t ADDRESS_APP_END;			/*< What app address to jump*/
	struct state{
		uint16_t APP_1;					/*< Test/error state of the App 1 */
		uint16_t APP_2;					/*< Test/error state of the App 2 */
	}error, tested;
	struct version{
		uint16_t MAJOR;					/*< Major version of the APP 1/2 or bootloader */
		uint16_t MINOR;					/*< Minor version of the APP 1/2 or bootloader */
	}app_1_version, app_2_version, bl_version;
}IAPSTRUCT;

/* Declarations*/
flash_status flash_erase(uint32_t address, uint32_t APP_FLASH_END_ADDRESS);
flash_status flash_write(uint32_t address, uint32_t *data, uint32_t numofwords);
void flash_read (uint32_t address, uint32_t *data, uint32_t numofwords);

#endif /* INC_FLASH_H_ */
