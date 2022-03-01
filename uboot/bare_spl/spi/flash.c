/**
  ******************************************************************************
  * @file  sf16w18_flash.c
  * @author  yang
  * @version  V3.0.0
  * @date  03/22/2016
  * @brief  This file provides all the FLASH firmware functions.
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
  * <h2><center>&copy; COPYRIGHT 2016 Siflower Inc</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
#include <io.h>
#include <types.h>
#include <errorno.h>
#include "spi.h"
#ifdef TIMER
#include <timer/timer.h>
#endif
//#include <pad.h>

#ifdef CLIB
#include <stdio.h>
#endif

static void spi_flash_addr(u32 addr, u8 *cmd)
{
	/* cmd[0] is actual command */
	cmd[1] = addr >> 16;
	cmd[2] = addr >> 8;
	cmd[3] = addr >> 0;
}

static int spi_flash_read_write(const u8 *cmd, u32 cmd_len,
				const u8 *data_out, u8 *data_in, u32 data_len)
{
	unsigned long flags = SPI_XFER_BEGIN;
	int ret;

	if (data_len == 0)
		flags |= SPI_XFER_END;

	ret = SPI_xfer(cmd_len * 8, cmd, NULL, flags);
	if (ret) {
		printf("SF: Failed to send command (%u bytes): %d\n", cmd_len, ret);
	} else if (data_len != 0) {
		ret = SPI_xfer(data_len * 8, data_out, data_in, SPI_XFER_END);
		if (ret)
			printf("SF: Failed to transfer %u bytes of data: %d\n",
			      data_len, ret);
	}

	return ret;
}

static int spi_flash_cmd_read(const u8 *cmd,u32 cmd_len, void *data,
		u32 data_len)
{
	return spi_flash_read_write(cmd, cmd_len, NULL, data, data_len);
}

static int spi_flash_cmd_write(const u8 *cmd, u32 cmd_len,
		const void *data, u32 data_len)
{
	return spi_flash_read_write(cmd, cmd_len, data, NULL, data_len);
}

static int spi_flash_cmd(u8 cmd, void *response, u32 len)
{
	return spi_flash_cmd_read(&cmd, 1, response, len);
}

static int spi_flash_read_common(const u8 *cmd, u32 cmd_len, void *data, u32 data_len)
{
	return spi_flash_cmd_read(cmd, cmd_len, data, data_len);
}

static int spi_flash_ready()
{
	u8 sr;
	u8 cmd = FLASH_Read_SR;

	spi_flash_read_common(&cmd, 1, &sr, 1);

	return !(sr & FLASH_BSY);
}

static int spi_flash_cmd_wait_ready(unsigned long timeout)
{
	int ret;
	unsigned long time = 0;

	while (time < timeout / 10) {
		ret = spi_flash_ready();
		if (ret)
			return 0;
		u_delay(10);
		time++;
	}

	printf("SF: Timeout!\n");

	return -EFAULT;
}

static int spi_flash_write_common(const u8 *cmd, u32 cmd_len,
		const void *buf, u32 buf_len)
{
	unsigned long timeout = SPI_FLASH_PROG_TIMEOUT;
	int ret;

	if (buf == NULL)
		timeout = SPI_FLASH_PAGE_ERASE_TIMEOUT;

	spi_flash_cmd(FLASH_Write_En, NULL, 0);

	spi_flash_cmd_write(cmd, cmd_len, buf, buf_len);

	ret = spi_flash_cmd_wait_ready(timeout);
	if (ret < 0) {
		printf("SF: write %s timed out\n",
		      timeout == SPI_FLASH_PROG_TIMEOUT ?
			"program" : "page erase");
		return ret;
	}

	return ret;
}

static int spi_flash_write_bar(u32 bank_sel)
{
	u8 cmd;

	cmd = FLASH_Write_ExtnAddr;
	return spi_flash_write_common(&cmd, 1, &bank_sel, 1);
}

int SPI_FLASH_Read(u8 chnum, u32 offset, u8 *data, u32 len)
{
	const u8 cmdsz = SPI_FLASH_CMD_LEN + SPI_FLASH_DUMMY_BYTE;
	u8 cmd[SPI_FLASH_CMD_LEN + SPI_FLASH_DUMMY_BYTE]={0};
	u32 remain_len, read_len, read_addr;
	int bank_sel = offset / SPI_FLASH_16MB_BOUN;
	int bank_curr = 0;
	int ret = -1;

	SPI_Init();

	cmd[0] = FLASH_Fast_Read;

	if ((ret = spi_flash_write_bar(bank_sel)) < 0)
		return ret;
	bank_curr = bank_sel;

	while (len) {
		read_addr = offset;

		bank_sel = read_addr / SPI_FLASH_16MB_BOUN;
		if (bank_sel != bank_curr) {
			if ((ret = spi_flash_write_bar(bank_sel)) < 0)
				return ret;
			bank_curr = bank_sel;
		}

		remain_len = (SPI_FLASH_16MB_BOUN  * (bank_sel + 1)) - offset;
		if (len < remain_len)
			read_len = len;
		else
			read_len = remain_len;

		spi_flash_addr(read_addr, cmd);

		ret = spi_flash_read_common(cmd, cmdsz, data, read_len);
		if (ret < 0) {
			printf("SF: read failed\n");
			break;
		}

		offset += read_len;
		len -= read_len;
		data += read_len;
	}

	return ret;
}

int SPI_FLASH_Write(u8 chnum, u32 page_size, u32 offset, const u8 *buf, u32 len)
{
	unsigned long byte_addr;
	u32 write_addr;
	u32 chunk_len, actual;
	u8 cmd[SPI_FLASH_CMD_LEN];
	int bank_sel = offset / SPI_FLASH_16MB_BOUN;
	int bank_curr = 0;
	int ret = -1;

	SPI_Init();

	//page_size = FLASH_PAGE_SIZE;	/*flash->page_size*/
/*
	if (flash->flash_is_locked) {
		if (flash->flash_is_locked(flash, offset, len) > 0) {
			printf("offset 0x%x is protected and cannot be written\n",
			       offset);
			return -EINVAL;
		}
	}
*/
	/*cmd[0] = flash->write_cmd;*/
	cmd[0] = FLASH_Page_Program;
	if ((ret = spi_flash_write_bar(bank_sel)) < 0)
		return ret;
	bank_curr = bank_sel;

	for (actual = 0; actual < len; actual += chunk_len) {
		write_addr = offset;

		bank_sel = write_addr / SPI_FLASH_16MB_BOUN;
		if (bank_sel != bank_curr) {
			if ((ret = spi_flash_write_bar(bank_sel)) < 0)
				return ret;
			bank_curr = bank_sel;
		}

		byte_addr = offset % page_size;
		chunk_len = MIN(len - actual, (u32)(page_size - byte_addr));

		spi_flash_addr(write_addr, cmd);

		ret = spi_flash_write_common(cmd, sizeof(cmd), buf + actual, chunk_len);
		if (ret < 0) {
			printf("SF: write failed\n");
			break;
		}

		offset += chunk_len;
	}

	return ret;
}

u32 SPI_FLASH_GetDeviceID(u8 chnum)
{
	u32 jedec;
	u8 idcode[5] = {0};

	SPI_Init();

	spi_flash_cmd(FLASH_Read_ID, idcode, sizeof(idcode));

	jedec = (idcode[0] << 16) | (idcode[1] << 8) | idcode[2];
	return jedec;
}

int SPI_FLASH_Erase(u32 offset, u32 len, u32 blk_size)
{
	u32 erase_size, erase_addr;
	u8 cmd[SPI_FLASH_CMD_LEN];
	int ret = -1;
	int bank_sel = offset / SPI_FLASH_16MB_BOUN;
	int bank_curr = 0;

	SPI_Init();

	erase_size = blk_size;
	if (offset % erase_size) {
		printf("SF: Erase offset/length not multiple of erase size\n");
		return -1;
	}
	if (len % erase_size)
		len = (len / erase_size + 1) * erase_size;
/*
	if (flash->flash_is_locked) {
		if (flash->flash_is_locked(flash, offset, len) > 0) {
			printf("offset 0x%x is protected and cannot be erased\n",
			       offset);
			return -EINVAL;
		}
	}
*/
	switch(erase_size){
		case FLASH_SECTOR_64K:
			cmd[0] = FLASH_Block_Erase_64KB;
			break;
		case FLASH_SECTOR_32K:
			cmd[0] = FLASH_Block_Erase_32KB;
			break;
		case FLASH_SECTOR_4K:
			cmd[0] = FLASH_Sector_Erase;
			break;
		default:
			printf("Error:input a wrong flash sector size!\n");
			return -EINVAL;
	}

	if ((ret = spi_flash_write_bar(bank_sel)) < 0)
		return ret;
	bank_curr = bank_sel;

	while (len) {
		erase_addr = offset;

		bank_sel = erase_addr / SPI_FLASH_16MB_BOUN;
		if(bank_sel != bank_curr) {
			if ((ret = spi_flash_write_bar(bank_sel)) < 0)
				return ret;
			bank_curr=bank_sel;
		}

		spi_flash_addr(erase_addr, cmd);

		ret = spi_flash_write_common(cmd, sizeof(cmd), NULL, 0);
		if (ret < 0) {
			printf("SF: erase failed\n");
			break;
		}

		offset += erase_size;
		len -= erase_size;
	}

	return ret;
}


#if 0
int spi_flash_scan(struct spi_flash *flash)
{
	struct spi_slave *spi = flash->spi;
	const struct spi_flash_params *params;
	u16 jedec, ext_jedec;
	u8 cmd, idcode[5];
	int ret;
	static u8 spi_read_cmds_array[] = {
		CMD_READ_ARRAY_SLOW,
		CMD_READ_ARRAY_FAST,
		CMD_READ_DUAL_OUTPUT_FAST,
		CMD_READ_QUAD_OUTPUT_FAST,
		CMD_READ_DUAL_IO_FAST,
		CMD_READ_QUAD_IO_FAST };

	/* Read the ID codes */
	ret = spi_flash_cmd(spi, CMD_READ_ID, idcode, sizeof(idcode));
	if (ret) {
		printf("SF: Failed to get idcodes\n");
		return ret;
	}

	jedec = idcode[1] << 8 | idcode[2];
	ext_jedec = idcode[3] << 8 | idcode[4];

	/* Validate params from spi_flash_params table */
	params = spi_flash_params_table;
	for (; params->name != NULL; params++) {
		if ((params->jedec >> 16) == idcode[0]) {
			if ((params->jedec & 0xFFFF) == jedec) {
				if (params->ext_jedec == 0)
					break;
				else if (params->ext_jedec == ext_jedec)
					break;
			}
		}
	}

	if (!params->name) {
		printf("SF: Unsupported flash IDs: ");
		printf("manuf %02x, jedec %04x, ext_jedec %04x\n",
		       idcode[0], jedec, ext_jedec);
		return -EPROTONOSUPPORT;
	}

	/* Flash powers up read-only, so clear BP# bits */
	if (idcode[0] == SPI_FLASH_CFI_MFR_ATMEL ||
	    idcode[0] == SPI_FLASH_CFI_MFR_MACRONIX ||
	    idcode[0] == SPI_FLASH_CFI_MFR_SST)
		write_sr(flash, 0);

	/* Assign spi data */
	flash->name = params->name;
	flash->memory_map = spi->memory_map;
	flash->dual_flash = spi->option;

	/* Assign spi flash flags */
	if (params->flags & SST_WR)
		flash->flags |= SNOR_F_SST_WR;

	/* Assign spi_flash ops */
#ifndef CONFIG_DM_SPI_FLASH
	flash->write = spi_flash_cmd_write_ops;
	flash->erase = spi_flash_cmd_erase_ops;
	flash->read = spi_flash_cmd_read_ops;
#endif

	/* lock hooks are flash specific - assign them based on idcode0 */
	switch (idcode[0]) {
#if defined(CONFIG_SPI_FLASH_STMICRO) || defined(CONFIG_SPI_FLASH_SST)
	case SPI_FLASH_CFI_MFR_STMICRO:
	case SPI_FLASH_CFI_MFR_SST:
		flash->flash_lock = stm_lock;
		flash->flash_unlock = stm_unlock;
		flash->flash_is_locked = stm_is_locked;
#endif
		break;
	default:
		printf("SF: Lock ops not supported for %02x flash\n", idcode[0]);
	}

	/* Compute the flash size */
	flash->shift = (flash->dual_flash & SF_DUAL_PARALLEL_FLASH) ? 1 : 0;
	/*
	 * The Spansion S25FL032P and S25FL064P have 256b pages, yet use the
	 * 0x4d00 Extended JEDEC code. The rest of the Spansion flashes with
	 * the 0x4d00 Extended JEDEC code have 512b pages. All of the others
	 * have 256b pages.
	 */
	if (ext_jedec == 0x4d00) {
		if ((jedec == 0x0215) || (jedec == 0x216) || (jedec == 0x220))
			flash->page_size = 256;
		else
			flash->page_size = 512;
	} else {
		flash->page_size = 256;
	}
	flash->page_size <<= flash->shift;
	flash->sector_size = params->sector_size << flash->shift;
	flash->size = flash->sector_size * params->nr_sectors << flash->shift;

	/* Compute erase sector and command */
	if (params->flags & SECT_4K) {
		flash->erase_cmd = CMD_ERASE_4K;
		flash->erase_size = 4096 << flash->shift;
	} else if (params->flags & SECT_32K) {
		flash->erase_cmd = CMD_ERASE_32K;
		flash->erase_size = 32768 << flash->shift;
	} else {
		flash->erase_cmd = CMD_ERASE_64K;
		flash->erase_size = flash->sector_size;
	}

	/* Now erase size becomes valid sector size */
	flash->sector_size = flash->erase_size;

	/* Look for the fastest read cmd */
	cmd = fls(params->e_rd_cmd & spi->mode_rx);
	if (cmd) {
		cmd = spi_read_cmds_array[cmd - 1];
		flash->read_cmd = cmd;
	} else {
		/* Go for default supported read cmd */
		flash->read_cmd = CMD_READ_ARRAY_FAST;
	}

	/* Not require to look for fastest only two write cmds yet */
	if (params->flags & WR_QPP && spi->mode & SPI_TX_QUAD)
		flash->write_cmd = CMD_QUAD_PAGE_PROGRAM;
	else
		/* Go for default supported write cmd */
		flash->write_cmd = CMD_PAGE_PROGRAM;

	/* Set the quad enable bit - only for quad commands */
	if ((flash->read_cmd == CMD_READ_QUAD_OUTPUT_FAST) ||
	    (flash->read_cmd == CMD_READ_QUAD_IO_FAST) ||
	    (flash->write_cmd == CMD_QUAD_PAGE_PROGRAM)) {
		ret = set_quad_mode(flash, idcode[0]);
		if (ret) {
			printf("SF: Fail to set QEB for %02x\n", idcode[0]);
			return -EINVAL;
		}
	}

	/* Read dummy_byte: dummy byte is determined based on the
	 * dummy cycles of a particular command.
	 * Fast commands - dummy_byte = dummy_cycles/8
	 * I/O commands- dummy_byte = (dummy_cycles * no.of lines)/8
	 * For I/O commands except cmd[0] everything goes on no.of lines
	 * based on particular command but incase of fast commands except
	 * data all go on single line irrespective of command.
	 */
	switch (flash->read_cmd) {
	case CMD_READ_QUAD_IO_FAST:
		flash->dummy_byte = 2;
		break;
	case CMD_READ_ARRAY_SLOW:
		flash->dummy_byte = 0;
		break;
	default:
		flash->dummy_byte = 1;
	}

#ifdef CONFIG_SPI_FLASH_STMICRO
	if (params->flags & E_FSR)
		flash->flags |= SNOR_F_USE_FSR;
#endif

	/* Configure the BAR - discover bank cmds and read current bank */
#ifdef CONFIG_SPI_FLASH_BAR
	ret = spi_flash_read_bar(flash, idcode[0]);
	if (ret < 0)
		return ret;
#endif

	return ret;
}
#endif
