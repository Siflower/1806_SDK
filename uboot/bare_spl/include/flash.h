/**
  ******************************************************************************
  * @file  sf16w18_flash.h
  * @author  yang
  * @version  V3.0.0
  * @date  03/22/2016
  * @brief  This file contains all the functions prototypes for the FLASH
  *         firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Siflower Inc SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2026 Siflower Inc</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF16W18_FLASH_H
#define __SF16W18_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "spi.h"




/* Flash command field */
#define FLASH_Block_Erase_32KB				0x52
#define FLASH_Block_Erase_64KB				0xD8
#define FLASH_Sector_Erase					0x20
#define FLASH_Chip_Erase					0xC7
#define FLASH_Write_En						0x06
#define FLASH_Write_Dis						0x04
#define FLASH_Reset							0x99
#define FLASH_Reset_En						0x66
#define FLASH_Read_SR						0x05
#define FLASH_Write_SR						0x01
#define FLASH_Read							0x03
#define FLASH_Fast_Read						0x0B
#define FLASH_Read_ID						0x9f
#define FLASH_Page_Program					0x02
#define FLASH_Write_ExtnAddr				0xc5
#define FLASH_Read_ExtnAddr					0xc8

/* Flash status field */
#define FLASH_Write_Enable					(1 << 1)
#define FLASH_BSY							(1 << 0)

#define SPI_FLASH_3B_ADDR_LEN	3
#define SPI_FLASH_CMD_LEN		(1 + SPI_FLASH_3B_ADDR_LEN)
#define SPI_FLASH_16MB_BOUN		0x1000000
#define SPI_FLASH_DUMMY_BYTE	1

#define FLASH_RW_CMD_SIZE                   5

#define FLASH_PAGE_SIZE						256
#define FLASH_SECTOR_4K						(4 * 1024)
#define FLASH_SECTOR_32K					(32 * 1024)
#define FLASH_SECTOR_64K					(64 * 1024)
#define SPI_FLASH_PROG_TIMEOUT			(2 * 100000)
#define SPI_FLASH_PAGE_ERASE_TIMEOUT	(5 * 1000000)
#define SPI_FLASH_SECTOR_ERASE_TIMEOUT	(10 * 1000000)

extern int SPI_FLASH_Read(u8 chnum, u32 addr, u8* buf, u32 len);
extern u32 SPI_FLASH_GetDeviceID(u8 chnum);
extern int SPI_FLASH_Erase(u32 addr, u32 earse_size, u32 blk_size);
extern int SPI_FLASH_Write(u8 chnum, u32 page_size, u32 addr, const u8 *buf, u32 len);

#endif /* __SF16W18_FLASH_H */


/******************* (C) COPYRIGHT 2016 Siflower Inc. *****END OF FILE****/
