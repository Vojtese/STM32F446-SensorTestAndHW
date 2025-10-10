/*
 * iap.h
 *
 *  Created on: 31. 3. 2022
 *      Author: slabv
 */

#ifndef INC_IAP_H_
#define INC_IAP_H_

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



/* Declarations  */
void flash_jump_to_app(uint32_t ADDRESS_TO_JUMP);


#endif /* INC_IAP_H_ */
