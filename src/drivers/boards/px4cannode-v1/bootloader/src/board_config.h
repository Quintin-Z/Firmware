/****************************************************************************
 *
 *   Copyright (c) 2012-2015 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/*
 * @file board_config.h
 *
 * PX4CANNODEv1 for the bootloader internal definitions
 * This file is related to the parrent folder version but defines 
 * differnet usages of the hardware for bootloading
 */

#pragma once


/************************************************************************************
 * Included Files
 ************************************************************************************/

#include "../../board_config.h"
#include "protocol.h"
#include <nuttx/compiler.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
//todo:wrap OPT_x in in ifdefs for command line definitions
#define OPT_TBOOT_MS            2000
#define OPT_NODE_STATUS_RATE_MS 800
#define OPT_NODE_INFO_RATE_MS   200
#define OPT_BL_NUMBER_TIMERS    6

#define OPT_WAIT_FOR_GETNODEINFO                    0
#define OPT_WAIT_FOR_GETNODEINFO_JUMPER_GPIO        1
#define OPT_WAIT_FOR_GETNODEINFO_JUMPER_GPIO_INVERT 1

#define OPT_ENABLE_WD           1

#define OPT_RESTART_TIMEOUT_MS  20000u

/* Reserved for the Booloader */
#define OPT_BOOTLOADER_SIZE_IN_K            (1024*8)

/* Reserved for the application out of the total
 * system flash minus the BOOTLOADER_SIZE_IN_K
 */
#define OPT_APPLICATION_RESERVER_IN_K            0

#define OPT_APPLICATION_IMAGE_OFFSET    OPT_BOOTLOADER_SIZE_IN_K
#define OPT_APPLICATION_IMAGE_LENGTH    (FLASH_SIZE-(OPT_BOOTLOADER_SIZE_IN_K+OPT_APPLICATION_RESERVER_IN_K))


#define FLASH_BASE              STM32_FLASH_BASE
#define FLASH_NUMBER_PAGES      STM32_FLASH_NPAGES
#define FLASH_PAGE_SIZE         STM32_FLASH_PAGESIZE
#define FLASH_SIZE              (FLASH_NUMBER_PAGES*FLASH_PAGE_SIZE)

#define APPLICATION_LOAD_ADDRESS (FLASH_BASE + OPT_APPLICATION_IMAGE_OFFSET)
#define APPLICATION_SIZE (FLASH_SIZE-OPT_APPLICATION_IMAGE_OFFSET)
#define APPLICATION_LAST_32BIT_ADDRRESS ((uint32_t *)((APPLICATION_LOAD_ADDRESS+APPLICATION_SIZE)-sizeof(uint32_t)))
#define APPLICATION_LAST_64BIT_ADDRRESS ((uint64_t *)((APPLICATION_LOAD_ADDRESS+APPLICATION_SIZE)-sizeof(uint64_t)))



/* Bootloader Option*****************************************************************
 *
 *   GPIO      Function                                     MPU        Board
 *                                                          Pin #      Name
 * -- ----- --------------------------------             ----------------------------
 *  *  PC[09] PC9/TIM3_CH4                                     40       BOOT0
 */
#define GPIO_GETNODEINFO_JUMPER (BUTTON_BOOT0n&~(GPIO_EXTI))


/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

typedef enum {

  reset,
  autobaud_start,
  autobaud_end,
  allocation_start,
  allocation_end,
  fw_update_start,
  fw_update_erase_fail,
  fw_update_invalid_response,
  fw_update_timeout,
  fw_update_invalid_crc,
  jump_to_app,
} uiindication_t;
/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/************************************************************************************
 * Name: stm32_boarddeinitialize
 *
 * Description:
 *   This function is called by the bootloader code priore to booting
 *   the application. Is should place the HW into an benign initialized state.
 *
 ************************************************************************************/

void stm32_boarddeinitialize(void);

/****************************************************************************
 * Name: board_get_product_name
 *
 * Description:
 *   Called to retrive the product name. The retuned alue is a assumed
 *   to be written to a pascal style string that will be length prefixed
 *   and not null terminated
 *
 * Input Parameters:
 *    product_name - A pointer to a buffer to write the name.
 *    maxlen       - The imum number of chatater that can be written
 *
 * Returned Value:
 *   The length of charaacters written to the buffer.
 *
 ****************************************************************************/

uint8_t board_get_product_name(uint8_t * product_name, size_t maxlen);

/****************************************************************************
 * Name: board_get_hardware_version
 *
 * Description:
 *   Called to retrive the hardware version information.
 *
 * Input Parameters:
 *    hw_version - A pointer to a uavcan_hardwareversion_t.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void board_get_hardware_version(uavcan_hardwareversion_t * hw_version);

/****************************************************************************
 * Name: board_indicate
 *
 * Description:
 *   Provides User feedback to indicate the state of the bootloader
 *   on board specific  hardware.
 *
 * Input Parameters:
 *    indication - A member of the uiindication_t
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void board_indicate(uiindication_t indication);


#endif /* __ASSEMBLY__ */