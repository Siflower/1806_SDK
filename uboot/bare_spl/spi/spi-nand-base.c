/**
* spi-nand-base.c
*
* Copyright (c) 2009-2015 Micron Technology, Inc.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#include <io.h>
#include <types.h>
#include "spi.h"
#include "spi-nand.h"
#include "nand_ecc.h"
#include "errno.h"
#include "external.h"

#ifdef SPI_NAND

extern int spi_nand_issue_cmd(struct spi_nand_chip *chip, struct spi_nand_cmd *cmd);
static int spi_nand_do_write_oob(struct spi_nand_chip *chip, loff_t to,
			     struct mtd_oob_ops *ops);

/* OOB layout */
static struct nand_ecclayout micron_ecc_layout_64 = {
	.eccbytes = 32,
	.eccpos = {
		8, 9, 10, 11, 12, 13, 14, 15,
		24, 25, 26, 27, 28, 29, 30, 21,
		40, 41, 42, 43, 44, 45, 46, 47,
		56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 30,
	.oobfree = {
		{.offset = 2,
		 .length = 6},
		{.offset = 16,
		 .length = 8},
		{.offset = 32,
		 .length = 8},
		{.offset = 48,
		 .length = 8}, }
};

static u8 oob_buffer[64];
static struct spi_nand_chip sfax8_spi_nand;
static int soft_ecc = 0;
static int nand_init = 0;

#define ops_to_ecc_off(ops) (soft_ecc || (ops->mode == MTD_OPS_RAW))
#define use_soft_ecc()	(soft_ecc)

/**
 * spi_nand_read_reg - send command 0Fh to read register
 * @chip: SPI-NAND device structure
 * @reg; register to read
 * @buf: buffer to store value
 */
static int spi_nand_read_reg(struct spi_nand_chip *chip,
			uint8_t reg, uint8_t *buf)
{
	struct spi_nand_cmd cmd;
	int ret;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_GET_FEATURE;
	cmd.n_addr = 1;
	cmd.addr[0] = reg;
	cmd.n_rx = 1;
	cmd.rx_buf = buf;

	ret = spi_nand_issue_cmd(chip, &cmd);
	if (ret < 0)
		spi_nand_error("err: %d read register %d\n", ret, reg);

	return ret;
}

/**
 * spi_nand_write_reg - send command 1Fh to write register
 * @chip: SPI-NAND device structure
 * @reg; register to write
 * @buf: buffer stored value
 */
static int spi_nand_write_reg(struct spi_nand_chip *chip,
			uint8_t reg, uint8_t *buf)
{
	struct spi_nand_cmd cmd;
	int ret;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_SET_FEATURE;
	cmd.n_addr = 1;
	cmd.addr[0] = reg;
	cmd.n_tx = 1,
	cmd.tx_buf = buf,

	ret = spi_nand_issue_cmd(chip, &cmd);
	if (ret < 0)
		spi_nand_error("err: %d write register %d\n", ret, reg);

	return ret;
}

/**
 * spi_nand_read_status - get status register value
 * @chip: SPI-NAND device structure
 * @status: buffer to store value
 * Description:
 *   After read, write, or erase, the Nand device is expected to set the
 *   busy status.
 *   This function is to allow reading the status of the command: read,
 *   write, and erase.
 *   Once the status turns to be ready, the other status bits also are
 *   valid status bits.
 */
static int spi_nand_read_status(struct spi_nand_chip *chip, uint8_t *status)
{
	return spi_nand_read_reg(chip, REG_STATUS, status);
}

/**
 * spi_nand_get_cfg - get configuration register value
 * @chip: SPI-NAND device structure
 * @cfg: buffer to store value
 * Description:
 *   Configuration register includes OTP config, Lock Tight enable/disable
 *   and Internal ECC enable/disable.
 */
static int spi_nand_get_cfg(struct spi_nand_chip *chip, u8 *cfg)
{
	return spi_nand_read_reg(chip, REG_CFG, cfg);
}

/**
 * spi_nand_set_cfg - set value to configuration register
 * @chip: SPI-NAND device structure
 * @cfg: buffer stored value
 * Description:
 *   Configuration register includes OTP config, Lock Tight enable/disable
 *   and Internal ECC enable/disable.
 */
static int spi_nand_set_cfg(struct spi_nand_chip *chip, u8 *cfg)
{
	return spi_nand_write_reg(chip, REG_CFG, cfg);
}

/**
 * spi_nand_set_ds - set value to die select register
 * @chip: SPI-NAND device structure
 * @cfg: buffer stored value
 * Description:
 *   Configuration register includes OTP config, Lock Tight enable/disable
 *   and Internal ECC enable/disable.
 */
static int spi_nand_set_ds(struct spi_nand_chip *chip, u8 *ds)
{
	return spi_nand_write_reg(chip, REG_DIE_SELECT, ds);
}

/**
 * spi_nand_lun_select - send die select command if needed
 * @chip: SPI-NAND device structure
 * @lun: lun need to access
 */
static int spi_nand_lun_select(struct spi_nand_chip *chip, u8 lun)
{
	u8 ds = 0;
	int ret = 0;

	if (chip->lun != lun) {
		ds = (lun == 1) ? DIE_SELECT_DS1 : DIE_SELECT_DS0;
		ret = spi_nand_set_ds(chip, &ds);
		chip->lun = lun;
	}

	return ret;
}

/**
 * spi_nand_enable_ecc - enable internal ECC
 * @chip: SPI-NAND device structure
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spi_nand_enable_ecc(struct spi_nand_chip *chip)
{
	u8 cfg = 0;

	spi_nand_get_cfg(chip, &cfg);
	if ((cfg & CFG_ECC_MASK) == CFG_ECC_ENABLE)
		return 0;
	cfg |= CFG_ECC_ENABLE;
	return spi_nand_set_cfg(chip, &cfg);
}

/**
 * spi_nand_disable_ecc - disable internal ECC
 * @chip: SPI-NAND device structure
 * Description:
 *   There is one bit( bit 0x10 ) to set or to clear the internal ECC.
 *   Enable chip internal ECC, set the bit to 1
 *   Disable chip internal ECC, clear the bit to 0
 */
static int spi_nand_disable_ecc(struct spi_nand_chip *chip)
{
	u8 cfg = 0;

	spi_nand_get_cfg(chip, &cfg);
	if ((cfg & CFG_ECC_MASK) == CFG_ECC_ENABLE) {
		cfg &= ~CFG_ECC_ENABLE;
		return spi_nand_set_cfg(chip, &cfg);
	}
	return 0;
}

/**
 * spi_nand_ecc_status - decode status regisger to get ecc info
 * @status: status register value to decode
 * @corrected: bitflip count that ecc corrected
 * @ecc_error: uncorrected bitflip happen or not
 */
static void spi_nand_ecc_status(unsigned int status,
					unsigned int *corrected,
					unsigned int *ecc_error)
{
	unsigned int ecc_status = status & SPI_NAND_MT29F_ECC_MASK;

	*ecc_error = (ecc_status == SPI_NAND_MT29F_ECC_UNCORR);
	switch (ecc_status) {
	case SPI_NAND_MT29F_ECC_0_BIT:
		*corrected = 0;
		break;
	case SPI_NAND_MT29F_ECC_1_3_BIT:
		*corrected = 3;
		break;
	case SPI_NAND_MT29F_ECC_4_6_BIT:
		*corrected = 6;
		break;
	case SPI_NAND_MT29F_ECC_7_8_BIT:
		*corrected = 8;
		break;
	}
}


/**
 * spi_nand_write_enable - send command 06h to enable write or erase the
 * Nand cells
 * @chip: SPI-NAND device structure
 * Description:
 *   Before write and erase the Nand cells, the write enable has to be set.
 *   After the write or erase, the write enable bit is automatically
 *   cleared (status register bit 2)
 *   Set the bit 2 of the status register has the same effect
 */
static int spi_nand_write_enable(struct spi_nand_chip *chip)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_WR_ENABLE;

	return spi_nand_issue_cmd(chip, &cmd);
}

/**
 * spi_nand_read_page_to_cache - send command 13h to read data from Nand to cache
 * @chip: SPI-NAND device structure
 * @page_addr: page to read
 */
static int spi_nand_read_page_to_cache(struct spi_nand_chip *chip,
					u32 page_addr)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_PAGE_READ;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)page_addr;

	return spi_nand_issue_cmd(chip, &cmd);
}

/**
 * spi_nand_read_from_cache - read data out from cache register
 * @chip: SPI-NAND device structure
 * @page_addr: page to read
 * @column: the location to read from the cache
 * @len: number of bytes to read
 * @rbuf: buffer held @len bytes
 * Description:
 *   Command can be 03h, 0Bh, 3Bh, 6Bh, BBh, EBh
 *   The read can specify 1 to (page size + spare size) bytes of data read at
 *   the corresponding locations.
 *   No tRd delay.
 */
static int spi_nand_read_from_cache(struct spi_nand_chip *chip, u32 page_addr,
		u32 column, size_t len, u8 *rbuf)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = chip->read_cache_op;
	cmd.n_addr = 2;
	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		cmd.addr[0] |= (u8)(((page_addr >>
			(chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)column;
	cmd.n_rx = len;
	cmd.rx_buf = rbuf;

	return spi_nand_issue_cmd(chip, &cmd);
}

/**
 * spi_nand_program_data_to_cache - write data to cache register
 * @chip: SPI-NAND device structure
 * @page_addr: page to write
 * @column: the location to write to the cache
 * @len: number of bytes to write
 * @wrbuf: buffer held @len bytes
 * @clr_cache: clear cache register or not
 * Description:
 *   Command can be 02h, 32h, 84h, 34h
 *   02h and 32h will clear the cache with 0xff value first
 *   Since it is writing the data to cache, there is no tPROG time.
 */
static int spi_nand_program_data_to_cache(struct spi_nand_chip *chip,
		u32 page_addr, u32 column, size_t len, const u8 *wbuf, bool clr_cache)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	if (clr_cache)
		cmd.cmd = chip->write_cache_op;
	else
		cmd.cmd = chip->write_cache_rdm_op;
	cmd.n_addr = 2;
	cmd.addr[0] = (u8)(column >> 8);
	if (chip->options & SPINAND_NEED_PLANE_SELECT)
		cmd.addr[0] |= (u8)(((page_addr >>
			(chip->block_shift - chip->page_shift)) & 0x1) << 4);
	cmd.addr[1] = (u8)column;
	cmd.n_tx = len;
	cmd.tx_buf = wbuf;

	return spi_nand_issue_cmd(chip, &cmd);
}


/**
 * spi_nand_program_execute - send command 10h to write a page from
 * cache to the Nand array
 * @chip: SPI-NAND device structure
 * @page_addr: the physical page location to write the page.
 * Description:
 *   Need to wait for tPROG time to finish the transaction.
 */
static int spi_nand_program_execute(struct spi_nand_chip *chip, u32 page_addr)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_PROG_EXC;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)page_addr;

	return spi_nand_issue_cmd(chip, &cmd);
}


/**
 * spi_nand_erase_block_erase - send command D8h to erase a block
 * @chip: SPI-NAND device structure
 * @page_addr: the page to erase.
 * Description:
 *   Need to wait for tERS.
 */
static int spi_nand_erase_block(struct spi_nand_chip *chip,
					u32 page_addr)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_BLK_ERASE;
	cmd.n_addr = 3;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)page_addr;

	return spi_nand_issue_cmd(chip, &cmd);
}
#if 0
/**
 * spi_nand_read_page_cache_random - send command 30h for data read
 * @chip: SPI-NAND device structure
 * @page_addr: the page to read to data register.
 * Description:
 *   Transfer data from data register to cache register and kick off the other
 *   page data transferring from array to data register.
 */
static int spi_nand_read_page_cache_random(struct spi_nand_chip *chip,
					u32 page_addr)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_READ_PAGE_CACHE_RDM;
	cmd.addr[0] = (u8)(page_addr >> 16);
	cmd.addr[1] = (u8)(page_addr >> 8);
	cmd.addr[2] = (u8)page_addr;
	cmd.n_addr = 3;

	return spi_nand_issue_cmd(chip, &cmd);
}

/**
 * spi_nand_read_page_cache_last - send command 3Fh to end
 * READ PAGE CACHE RANDOM(30h) sequence
 * @chip: SPI-NAND device structure
 * Description:
 *   End the READ PAGE CACHE RANDOM sequence and copies a page from
 *   the data register to the cache register.
 */
static int spi_nand_read_page_cache_last(struct spi_nand_chip *chip)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_READ_PAGE_CACHE_LAST;

	return spi_nand_issue_cmd(chip, &cmd);
}
#endif
/**
 * spi_nand_wait - wait until the command is done
 * @chip: SPI-NAND device structure
 * @s: buffer to store status register(can be NULL)
 */
static int spi_nand_wait(struct spi_nand_chip *chip, u8 *s)
{
	/* set timeout to 1 second */
	int timeout = 100000;
	int time = 0;
	u8 status;

	unsigned long ret = -ETIMEDOUT;

	while (time < timeout) {
		spi_nand_read_status(chip, &status);
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			ret = 0;
			goto out;
		}
		u_delay(10);
		time++;
	}
out:
	if (s)
		*s = status;

	return ret;
}
#if 0
/**
 * spi_nand_wait_crbusy - wait until CRBSY is clear
 * @chip: SPI-NAND device structure
 * Description:
 *   Used in READ PAGE CACHE RANDOM(30h) sequence, CRBSY bit clear
 *   means data is transferd from data register to cache register.
 */
static int spi_nand_wait_crbusy(struct spi_nand_chip *chip)
{
	u8 status;
	/* set timeout to 1 second */
	int timeout = 100000;
	int time = 0;
	unsigned long ret = -ETIMEDOUT;

	while (time < timeout) {
		spi_nand_read_status(chip, &status);
		if ((status & STATUS_CRBSY_MASK) == STATUS_READY) {
			ret = 0;
			goto out;
		}
		u_delay(10);
		time++;
	}
out:
	return ret;
}
#endif
/**
 * spi_nand_read_id - send 9Fh command to get ID
 * @chip: SPI-NAND device structure
 * @buf: buffer to store id
 */
static int spi_nand_read_id(struct spi_nand_chip *chip, u8 *buf)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_READ_ID;
	cmd.n_rx = 2;
	cmd.rx_buf = buf;

	return spi_nand_issue_cmd(chip, &cmd);
}

/**
 * spi_nand_reset - send command FFh to reset chip.
 * @chip: SPI-NAND device structure
 */
static int spi_nand_reset(struct spi_nand_chip *chip)
{
	struct spi_nand_cmd cmd;

	memset(&cmd, 0, sizeof(struct spi_nand_cmd));
	cmd.cmd = SPINAND_CMD_RESET;

	if (spi_nand_issue_cmd(chip, &cmd) < 0)
		spi_nand_error("spi_nand reset failed!\n");

	/* elapse 2ms before issuing any other command */
	u_delay(2000);

	return 0;
}

/**
 * spi_nand_lock_block - [Interface] write block lock register to
 * lock/unlock device
 * @spi: spi device structure
 * @lock: value to set to block lock register
 * Description:
 *   After power up, all the Nand blocks are locked.  This function allows
 *   one to unlock the blocks, and so it can be written or erased.
 *
 * Register	Bit Address
 *   |     7     |   6     |    5   |    4    |    3    |   2    |    1     |     0        |
 *   |---------------------------------------------------------|
 *   | BRWD2 |  BP3  |  BP2  |  BP1  |  BP0  |   TB   |   WP   | Reversed |
 *
 * Block Lock Register Block Protection Bits
 *
 *	TB	BP3	BP2	BP1	BP0	Protected Portion
 *	0	0	0	0	0	All unlocked
 *	0	0	0	0	1	Upper 1/1024 locked
 *	0	0	0	1	0	Upper 1/512 locked
 *	0	0	0	1	1	Upper 1/256 locked
 *	0	0	1	0	0	Upper 1/128 locked
 *	0	0	1	0	1	Upper 1/64 locked
 *	0	0	1	1	0	Upper 1/32 locked
 *	0	0	1	1	1	Upper 1/16 locked
 *	0	1	0	0	0	Upper 1/8 locked
 *	0	1	0	0	1	Upper 1/4 locked
 *	0	1	0	1	0	Upper 1/2 locked
 *	0	1	0	1	1	All Locked
 *	0	1	1	0	0	All Locked
 *	0	1	1	0	1	All Locked
 *	0	1	1	1	0	All Locked
 *	0	1	1	1	1	All Locked
 *	1	0	0	0	0	All unlocked
 *	1	0	0	0	1	Lower 1/1024 locked
 *	1	0	0	1	0	Lower 1/512 locked
 *	1	0	0	1	1	Lower 1/256 locked
 *	1	0	1	0	0	Lower 1/128 locked
 *	1	0	1	0	1	Lower 1/64 locked
 *	1	0	1	1	0	Lower 1/32 locked
 *	1	0	1	1	1	Lower 1/16 locked
 *	1	1	0	0	0	Lower 1/8 locked
 *	1	1	0	0	1	Lower 1/4 locked
 *	1	1	0	1	0	Lower 1/2 locked
 *	1	1	0	1	1	All Locked
 *	1	1	1	0	0	All Locked
 *	1	1	1	0	1	All Locked
 *	1	1	1	1	0	All Locked
 *	1	1	1	1	1	All Locked

 */
int spi_nand_lock_block(struct spi_nand_chip *chip, u8 lock)
{
	return spi_nand_write_reg(chip, REG_BLOCK_LOCK, &lock);
}

/**
 * spi_nand_change_mode - switch chip to OTP/OTP protect/Normal mode
 * @chip: SPI-NAND device structure
 * @mode: mode to enter
 */
static int spi_nand_change_mode(struct spi_nand_chip *chip, u8 mode)
{
	u8 cfg;

	spi_nand_get_cfg(chip, &cfg);
	switch (mode) {
	case OTP_MODE:
		cfg = (cfg & ~CFG_OTP_MASK) | CFG_OTP_ENTER;
		break;
	case OTP_PROTECT_MODE:
		cfg = (cfg & ~CFG_OTP_MASK) | CFG_OTP_PROTECT;
		break;
	case SNOR_READ_ENABLE_MODE:
		cfg = (cfg & ~CFG_OTP_MASK) | CFG_SNOR_ENABLE;
		break;
	case NORMAL_MODE:
		cfg = (cfg & ~CFG_OTP_MASK) | CFG_OTP_EXIT;
		break;
	}
	spi_nand_set_cfg(chip, &cfg);

	return 0;
}

static int spi_nand_read_column_ecc_from_cache(struct spi_nand_chip *chip, u32 page_addr, u32 column, u_char *buf)
{
	int ecc_offset = chip->page_size + COLUMN_TO_ECC_OFFSET(column);
	return spi_nand_read_from_cache(chip, page_addr, ecc_offset, 3, buf);
}

/**
 * spi_nand_do_read_page - read page from flash to buffer
 * @chip: spi nand chip structure
 * @page_addr: page address/raw address
 * @column: column address
 * @ecc_off: without ecc or not
 * @corrected: how many bit error corrected
 * @buf: data buffer
 * @len: data length to read
 * Description:
 *   Return -EBADMSG when internal ecc can not correct bitflips.
 *   The command sequence to transfer data from NAND array to output is
 *   follows:
 *      13h (PAGE READ to cache register)
 *      0Fh (GET FEATURES command to read the status)
 *      0Bh/03h/3Bh/6Bh (Read from Cache Xn); or BBh/EBh (Read From
 *      Cache Dual/Quad IO)
 */
static int spi_nand_do_read_page(struct spi_nand_chip *chip, u32 page_addr,
				u32 column, bool ecc_off, unsigned int *corrected, u_char *buf, size_t len)
{
	u8 tmp_buf[ECC_SIZE], *pbuffer;
	u8 calc_ecc[3];
	u8 read_ecc[3];
	int ret;
	unsigned int ecc_error;
	u8 status;
	int ecc_column, size, retlen = len;

	// printf("read page %d, column %d, len %ld\n", page_addr, column, len);
	spi_nand_read_page_to_cache(chip, page_addr);
	ret = spi_nand_wait(chip, &status);
	if (ret < 0) {
		spi_nand_error("error %d waiting page 0x%x to cache\n",
			ret, page_addr);
		return ret;
	}
	if (!ecc_off) {
		spi_nand_ecc_status(status, corrected, &ecc_error);
		if (ecc_error) {
			spi_nand_error("internal ECC error reading page 0x%x\n",
				page_addr);
			ret = -EBADMSG;
		}
	}

	if ((!use_soft_ecc()) || (column + len > chip->page_size)) {
		spi_nand_read_from_cache(chip, page_addr, column, len, buf);
		return ret;
	}

	while(retlen) {
		if (column & ECC_MASK || retlen < ECC_SIZE) {
			ecc_column = column & (~ECC_MASK);
			pbuffer = tmp_buf;
		} else {
			ecc_column = column;
			pbuffer = buf;
		}
		spi_nand_read_from_cache(chip, page_addr, ecc_column, ECC_SIZE, pbuffer);
		spi_nand_read_column_ecc_from_cache(chip, page_addr, column, read_ecc);
		spi_nand_debug("ecc:%x %x %x read of page %d, column %d\n", read_ecc[0], read_ecc[1], read_ecc[2],
				page_addr, column);
		nand_calculate_ecc(pbuffer, ECC_SIZE, calc_ecc);
		spi_nand_debug("calc_ecc:%x %x %x\n", calc_ecc[0], calc_ecc[1], calc_ecc[2]);
		if ((ret = nand_correct_data(pbuffer, read_ecc, calc_ecc, ECC_SIZE)) < 0)
			return ret;
		if (column & ECC_MASK || retlen < ECC_SIZE) {
			size = min(retlen, ECC_SIZE - (column & ECC_MASK));
			memcpy(buf, &tmp_buf[(column & ECC_MASK)], size);
			retlen -= size;
			column += size;
			buf += size;
		} else {
			retlen -= ECC_SIZE;
			column += ECC_SIZE;
			buf += ECC_SIZE;
		}
	}

	return 0;
}

static int spi_nand_write_ecc(struct spi_nand_chip *chip, u32 page_addr,
				u32 column, const u_char *buf, size_t len)
{
	u_char *calc_ecc;
	u_char tmp[ECC_SIZE];
	size_t ecc_offset =  0, total_ecc_size;
	const u_char *buf_pointer = buf;
	struct mtd_oob_ops ops = {0};

	if (!use_soft_ecc())
		return 0;

	total_ecc_size = DIV_ROUND_UP(len, ECC_SIZE) * 3;
	calc_ecc = malloc(total_ecc_size);

	while (len) {
		if (len < ECC_SIZE) {
			memcpy(tmp, buf_pointer, len);
			memset(&tmp[len], 0xff, ECC_SIZE - len);
			buf_pointer = tmp;
		}
		nand_calculate_ecc(buf_pointer, ECC_SIZE, &calc_ecc[ecc_offset]);
		spi_nand_debug("ecc:%x %x %x write to page %d, oob offset %ld\n", calc_ecc[ecc_offset],
		       calc_ecc[ecc_offset + 1], calc_ecc[ecc_offset + 2], page_addr, COLUMN_TO_ECC_OFFSET(column) + ecc_offset);
		ecc_offset += 3;
		buf_pointer += ECC_SIZE;
		len -= min(len, (size_t)ECC_SIZE);
	}

	ops.mode = MTD_OPS_PLACE_OOB;
	ops.oobbuf = calc_ecc;
	ops.ooblen = total_ecc_size;
	ops.ooboffs = COLUMN_TO_ECC_OFFSET(column);
	spi_nand_do_write_oob(chip, page_addr << chip->page_shift, &ops);
	free(calc_ecc);
	return 0;
}

/**
 * spi_nand_do_write_page - write data from buffer to flash
 * @chip: spi nand chip structure
 * @page_addr: page address/raw address
 * @column: column address
 * @buf: data buffer
 * @len: data length to write
 * @clr_cache: clear cache register with 0xFF or not
 * Description:
 *   Page program sequence is as follows:
 *       06h (WRITE ENABLE)
 *       02h/32h/84h/34h (PROGRAM LOAD (RAMDOM_DATA) Xn)
 *       10h (PROGRAM EXECUTE)
 *       0Fh (GET FEATURE command to read the status)
 *   PROGRAM LOAD Xn instruction will reset the cache resigter with 0xFF,
 *   while PROGRAM LOAD RANDOM DATA Xn instruction will only update the
 *   data bytes that are specified by the command input sequence and the rest
 *   of data in the cache buffer will remain unchanged.
 */
static int spi_nand_do_write_page(struct spi_nand_chip *chip, u32 page_addr,
				u32 column, const u_char *buf, size_t len, bool clr_cache)
{
	u8 status;
	bool p_fail = false;
	int ret = 0;

	if (column % ECC_SIZE) {
		spi_nand_info("%s: write address not aligned to %d\n", __func__, ECC_SIZE);
		return -EINVAL;
	}
	spi_nand_write_enable(chip);
	spi_nand_program_data_to_cache(chip, page_addr,
					column, len, buf, clr_cache);
	spi_nand_program_execute(chip, page_addr);
	ret = spi_nand_wait(chip, &status);
	if (ret < 0) {
		spi_nand_error("error %d reading page 0x%x from cache\n",
			ret, page_addr);
		return ret;
	}
	if ((status & STATUS_P_FAIL_MASK) == STATUS_P_FAIL) {
		spi_nand_error("program page 0x%x failed\n", page_addr);
		p_fail = true;
	}
	if (p_fail)
		ret = -EIO;

	return ret;
}

/**
 * spi_nand_transfer_oob - transfer oob to client buffer
 * @chip: SPI-NAND device structure
 * @oob: oob destination address
 * @ops: oob ops structure
 * @len: size of oob to transfer
 */
static void spi_nand_transfer_oob(struct spi_nand_chip *chip, u8 *oob,
				  struct mtd_oob_ops *ops, size_t len)
{
#if 0
	switch (ops->mode) {

	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_RAW:
		memcpy(oob, chip->oobbuf + ops->ooboffs, len);
		return;
	default: {
		struct nand_oobfree *free = chip->ecclayout->oobfree;
		uint32_t boffs = 0, roffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Read request not from offset 0? */
			if (roffs) {
				if (roffs >= free->length) {
					roffs -= free->length;
					continue;
				}
				boffs = free->offset + roffs;
				bytes = min(len, (free->length - roffs));
				roffs = 0;
			} else {
				bytes = min(len, free->length);
				boffs = free->offset;
			}
			memcpy(oob, chip->oobbuf + boffs, bytes);
			oob += bytes;
		}
		return;
	}
	}
#else
	memcpy(oob, chip->oobbuf + ops->ooboffs, len);
	return;
#endif
}

/**
 * spi_nand_fill_oob - transfer client buffer to oob
 * @chip: SPI-NAND device structure
 * @oob: oob data buffer
 * @len: oob data write length
 * @ops: oob ops structure
 */
static void spi_nand_fill_oob(struct spi_nand_chip *chip, uint8_t *oob,
				size_t len, struct mtd_oob_ops *ops)
{
	memset(chip->oobbuf, 0xff, chip->oob_size);
#if 0
	switch (ops->mode) {

	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_RAW:
		memcpy(chip->oobbuf + ops->ooboffs, oob, len);
		return;

	default: {
		struct nand_oobfree *free = chip->ecclayout->oobfree;
		uint32_t boffs = 0, woffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Write request not from offset 0? */
			if (woffs) {
				if (woffs >= free->length) {
					woffs -= free->length;
					continue;
				}
				boffs = free->offset + woffs;
				bytes = min(len, (free->length - woffs));
				woffs = 0;
			} else {
				bytes = min(len, free->length);
				boffs = free->offset;
			}
			memcpy(chip->oobbuf + boffs, oob, bytes);
			oob += bytes;
		}
		return;
	}
	}
#else
	memcpy(chip->oobbuf + ops->ooboffs, oob, len);
	return;
#endif
}

/**
 * spi_nand_read_pages - read data from flash to buffer
 * @chip: spi nand chip structure
 * @from: offset to read from
 * @ops: oob operations description structure
 * Description:
 *   Normal read function, read one page to buffer before issue
 *   another. Return -EUCLEAN when bitflip is over threshold.
 *   Return -EBADMSG when internal ecc can not correct bitflips.
 */
static int spi_nand_read_pages(struct spi_nand_chip *chip,
			loff_t from, struct mtd_oob_ops *ops)
{
	int page_addr, page_offset, size;
	int ret;
	unsigned int corrected = 0;
	unsigned int max_bitflip = 0;
	int readlen = ops->len;
	int oobreadlen = ops->ooblen;
	bool ecc_off = ops_to_ecc_off(ops);
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ? chip->ecclayout->oobavail : chip->oob_size;
	unsigned int failed = 0;
	int lun_num;

	spi_nand_debug("%s: from = 0x%012lx, len = %i\n",
			 __func__, from, readlen);

	page_addr = from >> chip->page_shift;
	page_offset = from & chip->page_mask;
	lun_num = from >> chip->lun_shift;
	ops->retlen = 0;
	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	while (1) {
		size = min(readlen, chip->page_size - page_offset);
		ret = spi_nand_do_read_page(chip, page_addr, page_offset,
					ecc_off, &corrected, ops->datbuf + ops->retlen, size);
		if (ret == -EBADMSG)
			failed++;
		else if (ret) {
			spi_nand_error("error %d reading page 0x%x\n",
				ret, page_addr);
			goto out;
		}
		max_bitflip = max(corrected, max_bitflip);

		ops->retlen += size;
		readlen -= size;
		page_offset = 0;

		if (ops->oobbuf) {
			size = min(oobreadlen, ooblen);
			spi_nand_read_from_cache(chip, page_addr,
				chip->page_size, chip->oob_size, chip->oobbuf);
			spi_nand_transfer_oob(chip,
				ops->oobbuf + ops->oobretlen, ops, size);
			ops->oobretlen += size;
			oobreadlen -= size;
		}
		if (!readlen)
			break;

		page_addr++;
		/* Check, if we cross lun boundary */
		if (!(page_addr & ((1 << (chip->lun_shift - chip->page_shift)) - 1))
			&& (chip->options & SPINAND_NEED_DIE_SELECT)) {
			lun_num++;
			spi_nand_lun_select(chip, lun_num);
		}
	}
out:
	if (max_bitflip >= chip->refresh_threshold)
		ret = -EUCLEAN;
	if (failed)
		ret = -EBADMSG;

	return ret;
}

static inline bool is_read_page_fast_benefit(struct spi_nand_chip *chip,
			loff_t from, size_t len)
{
	return false;
}

/**
 * spi_nand_do_read_ops - read data from flash to buffer
 * @chip: spi nand device structure
 * @from: offset to read from
 * @ops: oob ops structure
 * Description:
 *   Disable internal ECC before reading when MTD_OPS_RAW set.
 */
static int spi_nand_do_read_ops(struct spi_nand_chip *chip, loff_t from,
			  struct mtd_oob_ops *ops)
{
	int ret;
	int oobreadlen = ops->ooblen;
	bool ecc_off = ops_to_ecc_off(ops);;
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ?
		chip->ecclayout->oobavail : chip->oob_size;

	spi_nand_debug("%s: from = 0x%012lx, len = %i\n",
			 __func__, from, (int)ops->len);
	/* Do not allow reads past end of device */
	if (from >= chip->size) {
		spi_nand_error("%s: attempt to read beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	/* for oob */
	if (oobreadlen > 0) {
		if (ops->ooboffs >= ooblen) {
			spi_nand_error("%s: attempt to start read outside oob\n",
					__func__);
			return -EINVAL;
		}

		if (ops->ooboffs + oobreadlen >
		((chip->size >> chip->page_shift) - (from >> chip->page_shift))
		* ooblen) {
			spi_nand_error("%s: attempt to read beyond end of device\n",
					__func__);
			return -EINVAL;
		}
		ooblen -= ops->ooboffs;
		ops->oobretlen = 0;
	}

	if (ecc_off)
		spi_nand_disable_ecc(chip);

	ret = spi_nand_read_pages(chip, from, ops);

	if (ecc_off)
		spi_nand_enable_ecc(chip);

	return ret;
}

/**
 * spi_nand_do_write_ops - write data from buffer to flash
 * @chip: spi nand device structure
 * @to: offset to write to
 * @ops: oob operations description structure
 * Description:
 *   Disable internal ECC before writing when MTD_OPS_RAW set.
 */
static int spi_nand_do_write_ops(struct spi_nand_chip *chip, loff_t to,
			 struct mtd_oob_ops *ops)
{
	int page_addr, page_offset, size;
	int writelen = ops->len;
	int oobwritelen = ops->ooblen;
	int ret = 0;
	int ooblen = chip->oob_size;
	bool ecc_off = ops_to_ecc_off(ops);
	bool clr_cache = true;
	int lun_num;

	spi_nand_debug("%s: to = 0x%012lx, len = %i\n",
			 __func__, to, writelen);
	/* Do not allow reads past end of device */
	if (to >= chip->size) {
		spi_nand_error("%s: attempt to write beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	page_addr = to >> chip->page_shift;
	page_offset = to & chip->page_mask;
	lun_num = to >> chip->lun_shift;
	ops->retlen = 0;

	/* for oob */
	if (oobwritelen > 0) {
		if (ops->ooboffs >= ooblen) {
			spi_nand_error("%s: attempt to start write outside oob\n",
					__func__);
			return -EINVAL;
		}
		if (ops->ooboffs + oobwritelen >
		((chip->size >> chip->page_shift) - (to >> chip->page_shift))
			* ooblen) {
			spi_nand_error("%s: attempt to write beyond end of device\n",
					__func__);
			return -EINVAL;
		}
		ooblen -= ops->ooboffs;
		ops->oobretlen = 0;
	}
	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	if (ecc_off)
		spi_nand_disable_ecc(chip);

	while (1) {
		if (ops->oobbuf) {
			size = min(oobwritelen, ooblen);

			spi_nand_fill_oob(chip, ops->oobbuf + ops->oobretlen,
					size, ops);
			ret = spi_nand_program_data_to_cache(chip, page_addr,
			chip->page_size, chip->oob_size, chip->oobbuf, true);
			if (ret) {
				spi_nand_error("error %d store page oob to cache 0x%x\n",
					ret, page_addr);
				goto out;
			}
			clr_cache = false;
			ops->oobretlen += size;
			oobwritelen -= size;
		}
		size = min(writelen, chip->page_size - page_offset);
		ret = spi_nand_do_write_page(chip, page_addr, page_offset,
				ops->datbuf + ops->retlen, size, clr_cache);
		if (ret) {
			spi_nand_error("error %d writing page 0x%x\n",
				ret, page_addr);
			goto out;
		}
		if (page_offset < chip->page_size)
			spi_nand_write_ecc(chip, page_addr, page_offset, ops->datbuf + ops->retlen, size);
		ops->retlen += size;
		writelen -= size;
		page_offset = 0;
		if (!writelen)
			break;
		page_addr++;
		/* Check, if we cross lun boundary */
		if (!(page_addr & ((1 << (chip->lun_shift - chip->page_shift)) - 1))
			&& (chip->options & SPINAND_NEED_DIE_SELECT)) {
			lun_num++;
			spi_nand_lun_select(chip, lun_num);
		}
	}
out:
	if (ecc_off)
		spi_nand_enable_ecc(chip);

	return ret;
}

/**
 * spi_nand_read - [Interface] SPI-NAND read
 * @chip: spi nand device structure
 * @from: offset to read from
 * @len: number of bytes to read
 * @retlen: pointer to variable to store the number of read bytes
 * @buf: the databuffer to put data
 */
int spi_nand_read(struct spi_nand_chip *chip, loff_t from, size_t len,
	size_t *retlen, u8 *buf)
{
	struct mtd_oob_ops ops;
	int ret;

	memset(&ops, 0, sizeof(ops));
	ops.len = len;
	ops.datbuf = buf;
	ops.mode = MTD_OPS_PLACE_OOB;
	ret = spi_nand_do_read_ops(chip, from, &ops);

	*retlen = ops.retlen;

	return ret;
}

/**
 * spi_nand_write - [Interface] SPI-NAND write
 * @chip: spi nand device structure
 * @to: offset to write to
 * @len: number of bytes to write
 * @retlen: pointer to variable to store the number of written bytes
 * @buf: the data to write
 */
int spi_nand_write(struct spi_nand_chip *chip, loff_t to, size_t len,
	size_t *retlen, const u8 *buf)
{
	struct mtd_oob_ops ops;
	int ret;

	if (to % ECC_SIZE) {
		spi_nand_info("write address not aligned to %d\n", ECC_SIZE);
		return -EINVAL;
	}

	memset(&ops, 0, sizeof(ops));
	ops.len = len;
	ops.datbuf = (uint8_t *)buf;
	ops.mode = MTD_OPS_PLACE_OOB;
	ret =  spi_nand_do_write_ops(chip, to, &ops);

	*retlen = ops.retlen;

	return ret;
}

/**
 * spi_nand_do_read_oob - read out-of-band
 * @chip: spi nand device structure
 * @from: offset to read from
 * @ops: oob operations description structure
 * Description:
 *   Disable internal ECC before reading when MTD_OPS_RAW set.
 */
static int spi_nand_do_read_oob(struct spi_nand_chip *chip, loff_t from,
			  struct mtd_oob_ops *ops)
{
	int page_addr;
	unsigned int corrected = 0;
	int readlen = ops->ooblen;
	int max_len = (ops->mode == MTD_OPS_AUTO_OOB) ?
		chip->ecclayout->oobavail : chip->oob_size;
	int ooboffs = ops->ooboffs;
	bool ecc_off = ops_to_ecc_off(ops);
	unsigned int failed = 0;
	int ret = 0;
	int lun_num;

	spi_nand_debug("%s: from = 0x%012lx, len = %i\n",
			 __func__, from, readlen);
	if (ooboffs >= max_len) {
		spi_nand_error("%s: attempt to read outside oob\n",
				__func__);
		return -EINVAL;
	}
	if (from >= chip->size ||
		ooboffs + readlen > ((chip->size >> chip->page_shift) -
							(from >> chip->page_shift)) * max_len) {
		spi_nand_error("%s: attempt to read beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	/* Shift to get page */
	page_addr = (from >> chip->page_shift);
	lun_num = from >> chip->lun_shift;
	max_len -= ooboffs;
	ops->oobretlen = 0;
	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	if (ecc_off)
		spi_nand_disable_ecc(chip);

	while (1) {
		/*read data from chip*/
		ret = spi_nand_do_read_page(chip, page_addr, chip->page_size,
				ecc_off, &corrected, chip->oobbuf, chip->oob_size);
		if (ret == -EBADMSG)
			failed++;
		else if (ret) {
			spi_nand_error("error %d reading page 0x%x\n",
				ret, page_addr);
			goto out;
		}

		max_len = min(max_len, readlen);
		spi_nand_transfer_oob(chip, ops->oobbuf + ops->oobretlen,
					ops, max_len);

		readlen -= max_len;
		ops->oobretlen += max_len;
		if (!readlen)
			break;

		page_addr++;
		/* Check, if we cross lun boundary */
		if (!(page_addr & ((1 << (chip->lun_shift - chip->page_shift)) - 1))
			&& (chip->options & SPINAND_NEED_DIE_SELECT)) {
			lun_num++;
			spi_nand_lun_select(chip, lun_num);
		}
	}
out:
	if (ecc_off)
		spi_nand_enable_ecc(chip);
	if (failed)
		ret = -EBADMSG;

	return ret;
}

/**
 * spi_nand_do_write_oob - write out-of-band
 * @chip: spi nand device structure
 * @to: offset to write to
 * @ops: oob operation description structure
 * Description:
 *   Disable internal ECC before writing when MTD_OPS_RAW set.
 */
static int spi_nand_do_write_oob(struct spi_nand_chip *chip, loff_t to,
			     struct mtd_oob_ops *ops)
{
	int ret = 0;
	int page_addr, max_len = (ops->mode == MTD_OPS_AUTO_OOB) ?
		chip->ecclayout->oobavail : chip->oob_size;
	int ooboffs = ops->ooboffs;
	bool ecc_off = ops_to_ecc_off(ops);
	int writelen = ops->ooblen;
	int lun_num;

	spi_nand_debug("%s: to = 0x%012lx, len = %i\n",
			 __func__, to, writelen);

	/* Do not allow write past end of page */
	if (ooboffs > max_len) {
		spi_nand_error("%s: attempt to write outside oob\n",
				__func__);
		return -EINVAL;
	}
	if (to >= chip->size ||
		ooboffs + writelen > ((chip->size >> chip->page_shift) -
							(to >> chip->page_shift)) * max_len) {
		spi_nand_error("%s: attempt to write beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	/* Shift to get page */
	page_addr = to >> chip->page_shift;
	lun_num = to >> chip->lun_shift;
	max_len -= ooboffs;
	ops->oobretlen = 0;

	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);
	if (ecc_off)
		spi_nand_disable_ecc(chip);

	while (1) {
		max_len = min(max_len, writelen);
		spi_nand_fill_oob(chip, ops->oobbuf + ops->oobretlen, max_len, ops);

		ret = spi_nand_do_write_page(chip, page_addr, chip->page_size,
					chip->oobbuf, chip->oob_size, true);
		if (ret) {
			spi_nand_error("error %d writing page 0x%x\n",
				ret, page_addr);
			goto out;
		}

		writelen -= max_len;
		ops->oobretlen += max_len;
		if (!writelen)
			break;

		page_addr++;
		/* Check, if we cross lun boundary */
		if (!(page_addr & ((1 << (chip->lun_shift - chip->page_shift)) - 1))
			&& (chip->options & SPINAND_NEED_DIE_SELECT)) {
			lun_num++;
			spi_nand_lun_select(chip, lun_num);
		}
	}
out:
	if (ecc_off)
		spi_nand_enable_ecc(chip);

	return ret;
}

/**
 * spi_nand_read_oob - [Interface] SPI-NAND read data and/or out-of-band
 * @chip: spi nand device structure
 * @from: offset to read from
 * @ops: oob operation description structure
 */
int spi_nand_read_oob(struct spi_nand_chip *chip, loff_t from,
			struct mtd_oob_ops *ops)
{
	int ret = -ENOTSUP;

	ops->retlen = 0;

	/* Do not allow reads past end of device */
	if (ops->datbuf && (from + ops->len) > chip->size) {
		spi_nand_error("%s: attempt to read beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	switch (ops->mode) {
	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_AUTO_OOB:
	case MTD_OPS_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf)
		ret = spi_nand_do_read_oob(chip, from, ops);
	else
		ret = spi_nand_do_read_ops(chip, from, ops);

out:
	return ret;
}

/**
 * spi_nand_write_oob - [Interface] SPI-NAND write data and/or out-of-band
 * @chip: spi nand device structure
 * @to: offset to write to
 * @ops: oob operation description structure
 */
int spi_nand_write_oob(struct spi_nand_chip *chip, loff_t to,
			  struct mtd_oob_ops *ops)
{
	int ret = -ENOTSUP;

	ops->retlen = 0;

	/* Do not allow writes past end of device */
	if (ops->datbuf && (to + ops->len) > chip->size) {
		spi_nand_error("%s: attempt to write beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	switch (ops->mode) {
	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_AUTO_OOB:
	case MTD_OPS_RAW:
		break;

	default:
		goto out;
	}

	if (!ops->datbuf)
		ret = spi_nand_do_write_oob(chip, to, ops);
	else
		ret = spi_nand_do_write_ops(chip, to, ops);

out:
	return ret;
}

/**
 * spi_nand_block_isbad - [Interface] check block is bad or not via read
 * bad block mark(the first two byte in oob area of the first page in the block)
 * @chip: spi nand device structure
 * @offs: offset from device start
 * Description:
 *   For a block, read the first page's first two byte of oob data, if data is all
 *   0xFF, the block is a good block, otherwise it a bad block.
 */
int spi_nand_block_isbad(struct spi_nand_chip *chip, loff_t offs)
{
	struct mtd_oob_ops ops = {0};
	u32 block_addr;
	u8 bad[2] = {0, 0};
	u8 ret = 0;

	block_addr = offs >> chip->block_shift;
	ops.mode = MTD_OPS_PLACE_OOB;
	ops.ooblen = 2;
	ops.oobbuf = bad;

	spi_nand_do_read_oob(chip, block_addr << chip->block_shift, &ops);
	if (bad[0] != 0xFF || bad[1] != 0xFF)
		ret =  1;

	return ret;
}

/**
 * spi_nand_block_markbad - [Interface] Mark a block as bad block
 * @chip: spi nand device structure
 * @offs: offset from device start
 * Description:
 *   For a block, bad block mark is the first page's first two byte of oob data,
 *   Write 0x0 to the area if we want to mark bad block.
 *   It is unnecessary to write the mark if the block has already marked as bad
 *   block.
 */
int spi_nand_block_markbad(struct spi_nand_chip *chip, loff_t offs)
{
	int ret;
	u32 block_addr;
	struct mtd_oob_ops ops = {0};
	u8 buf[2] = {0, 0};

	ret = spi_nand_block_isbad(chip, offs);
	if (ret) {
		if (ret > 0)
			return 0;
		return ret;
	}
	block_addr = offs >> chip->block_shift;
	spi_nand_erase(chip, block_addr << chip->block_shift, chip->block_size);
	ops.mode = MTD_OPS_PLACE_OOB;
	ops.ooblen = 2;
	ops.oobbuf = buf;

	ret = spi_nand_do_write_oob(chip, block_addr << chip->block_shift, &ops);

	return ret;
}

/**
 * spi_nand_erase - [Interface] erase block(s)
 * @chip: spi nand device structure
 * @addr: address that erase start with, should be blocksize aligned
 * @len: length that want to be erased, should be blocksize aligned
 * Description:
 *   Erase one ore more blocks
 *   The command sequence for the BLOCK ERASE operation is as follows:
 *       06h (WRITE ENBALE command)
 *       D8h (BLOCK ERASE command)
 *       0Fh (GET FEATURES command to read the status register)
 */
int spi_nand_erase(struct spi_nand_chip *chip, uint64_t addr, uint64_t len)
{
	int page_addr, pages_per_block;
	u8 status;
	int ret = 0;
	int lun_num;

	spi_nand_debug("%s: address = 0x%012lx, len = %llu\n",
			 __func__, addr, len);
	/* check address align on block boundary */
	if (addr & (chip->block_size - 1)) {
		spi_nand_error("%s: Unaligned address\n", __func__);
		return -EINVAL;
	}

	if (len & (chip->block_size - 1)) {
		spi_nand_error("%s: Length not block aligned\n", __func__);
		return -EINVAL;
	}

	/* Do not allow erase past end of device */
	if ((len + addr) > chip->size) {
		spi_nand_error("%s: Erase past end of device\n", __func__);
		return -EINVAL;
	}

	pages_per_block = 1 << (chip->block_shift - chip->page_shift);
	page_addr = addr >> chip->page_shift;
	lun_num = addr >> chip->lun_shift;

	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	while (len) {
		/* Check if we have a bad block, we do not erase bad blocks! */
		if (spi_nand_block_isbad(chip, ((loff_t) page_addr) <<
					chip->page_shift)) {
			spi_nand_error("%s: attempt to erase a bad block at 0x%012lx\n",
			__func__, ((loff_t) page_addr) << chip->page_shift);
			goto erase_exit;
		}
		spi_nand_write_enable(chip);
		spi_nand_erase_block(chip, page_addr);
		ret = spi_nand_wait(chip, &status);
		if (ret < 0) {
			spi_nand_error("block erase command wait failed\n");
			goto erase_exit;
		}
		if ((status & STATUS_E_FAIL_MASK) == STATUS_E_FAIL) {
			spi_nand_error("erase block 0x%012lx failed\n",
				((loff_t) page_addr) << chip->page_shift);
			ret = -EIO;
			goto erase_exit;
		}

		/* Increment page address and decrement length */
		len -= (1ULL << chip->block_shift);
		page_addr += pages_per_block;
		/* Check, if we cross lun boundary */
		if (len && !(page_addr &
			((1 << (chip->lun_shift - chip->page_shift)) - 1))
			&& (chip->options & SPINAND_NEED_DIE_SELECT)) {
			lun_num++;
			spi_nand_lun_select(chip, lun_num);
		}
	}

erase_exit:
	return ret;
}

/**
 * spi_nand_enable_snor_read_protocol - [Interface] enable SPI NOR read
 * protocol mode to implement 03h/0Bh command as SPI NOR.
 * @chip: spi nand device structure
 * Description:
 *   Once this solution is enabled, 03h command is working under <=20Mhz@3.3V,
 *   <=5MHz@1.8V; 0Bh command is working under 133Mhz@3.3v, 83Mhz@1.8V.
 *
 *   The command sequence is as follows:
 *       Set Feature command 1Fh with feature address B0h and data value 82h
 *              (access SPI NOR Read Protocol enable mode)
 *       WRITE Enable (06h)
 *       PROGRAM execute command (10h) with block/page address all '0'
 *       Get Feature command 1Fh with status register address C0h to check the
 *              device is ready or busy (OIP bit)
 */
int spi_nand_enable_snor_read_protocol(struct spi_nand_chip *chip)
{
	int ret = 0;
	u8 status;

	spi_nand_change_mode(chip, SNOR_READ_ENABLE_MODE);
	spi_nand_write_enable(chip);
	spi_nand_program_execute(chip, 0);
	ret = spi_nand_wait(chip, &status);
	if (ret || ((status & STATUS_P_FAIL_MASK) == STATUS_P_FAIL))
		spi_nand_error("%s failed.\n", __func__);
	spi_nand_change_mode(chip, NORMAL_MODE);

	return ret;
}

/**
 * spi_nand_read_otp_page - [Interface] read OTP page
 * @chip: spi nand device structure
 * @page_addr: page address/raw address to read
 * @column: column address
 * @buf: buffer to store data
 * @len: length to read
 */
int spi_nand_read_otp_page(struct spi_nand_chip *chip, u32 page_addr,
				u32 column, u_char *buf, size_t len)
{
	unsigned int corrected;

	spi_nand_change_mode(chip, OTP_MODE);
	spi_nand_do_read_page(chip, page_addr, column, true, &corrected, buf, len);
	spi_nand_change_mode(chip, NORMAL_MODE);

	return 0;
}

/**
 * spi_nand_write_otp_page - [Interface] write OTP page
 * @chip: spi nand device structure
 * @page_addr: page address/raw address to write
 * @column: column address
 * @buf: buffer held data
 * @len: length to write
 * @ecc_off: disable internal ECC when writing or not
 */
int spi_nand_write_otp_page(struct spi_nand_chip *chip, u32 page_addr,
				u32 column, u_char *buf, size_t len, bool ecc_off)
{
	if (ecc_off)
		spi_nand_disable_ecc(chip);
	spi_nand_change_mode(chip, OTP_MODE);
	spi_nand_do_write_page(chip, page_addr, column, buf, len, true);
	spi_nand_change_mode(chip, NORMAL_MODE);
	if (ecc_off)
		spi_nand_enable_ecc(chip);

	return 0;
}

/**
 * spi_nand_protect_otp_page - [Interface] protect OTP page
 * @chip: spi nand device structure
 * Description:
 *   Protecting the OTP area prevents further programming.
 */
int spi_nand_protect_otp_page(struct spi_nand_chip *chip)
{
	int ret;

	spi_nand_change_mode(chip, OTP_PROTECT_MODE);
	spi_nand_write_enable(chip);
	spi_nand_program_execute(chip, 0);
	ret = spi_nand_wait(chip, NULL);
	if (ret)
		spi_nand_debug("%s failed.\n", __func__);
	spi_nand_change_mode(chip, NORMAL_MODE);

	return 0;
}

static u8 ilog2(unsigned int v)
{
	u8 l = 0;
	while ((1UL << l) < v)
		l++;
	return l;
}

/**
 * spi_nand_scan_id_table - scan chip info in id table
 * @chip: SPI-NAND device structure
 * @id: point to manufacture id and device id
 * Description:
 *   If found in id table, config chip with table information.
 */
static bool spi_nand_scan_id_table(struct spi_nand_chip *chip, u8 *id)
{
	int page_size = 2048;
	int pages_per_block = 64;
	int blks_per_lun = 1024;

	chip->name = "common spi nand";
	chip->size = page_size * pages_per_block * blks_per_lun;
	chip->block_size = page_size * pages_per_block;
	chip->page_size = page_size;
	chip->oob_size = 64;
	chip->lun_shift = ilog2(chip->block_size * blks_per_lun);
	chip->ecc_strength = 64;
	chip->options = 0;

	return true;
}

/**
 * spi_nand_set_rd_wr_op - Chose the best read write command
 * @chip: SPI-NAND device structure
 * Description:
 *   Chose the fastest r/w command according to spi controller's ability.
 * Note:
 *   If 03h/0Bh follows SPI NAND protocol, there is no difference,
 *   while if follows SPI NOR protocol, 03h command is working under
 *   <=20Mhz@3.3V,<=5MHz@1.8V; 0Bh command is working under
 *   133Mhz@3.3v, 83Mhz@1.8V.
 */
static void spi_nand_set_rd_wr_op(struct spi_nand_chip *chip)
{
	chip->read_cache_op = SPINAND_CMD_READ_FROM_CACHE_FAST;
	chip->write_cache_op = SPINAND_CMD_PROG_LOAD;
	chip->write_cache_rdm_op = SPINAND_CMD_PROG_LOAD_RDM_DATA;
}

/*
 * spi_nand_lock_tight - [Interface] enable device lock tight
 * @chip: spi nand device structure
 * Description:
 *   Lock tight prevents blocks protection state from being modified
 *   through software method. Once enable, Only another power cycle
 *   could disable lock tight mode.
 */
int spi_nand_lock_tight(struct spi_nand_chip *chip)
{
	u8 cfg;

	spi_nand_get_cfg(chip, &cfg);
	cfg |= CFG_LOT_ENABLE;
	return spi_nand_set_cfg(chip, &cfg);
}

/**
 * spi_nand_init - [Interface] Init SPI-NAND device driver
 * @spi: spi device structure
 * @chip_ptr: pointer point to spi nand device structure pointer
 */
int spi_nand_init(struct spi_nand_chip *chip)
{
	u8 id[SPINAND_MAX_ID_LEN] = {0};

	if (nand_init)
		return 0;

	memset(chip, 0, sizeof(struct spi_nand_chip));

	spi_nand_set_rd_wr_op(chip);
	spi_nand_reset(chip);
	spi_nand_read_id(chip, id);
	spi_nand_info("SPI-NAND got mfr_id: %x, dev_id: %x\n", id[0], id[1]);

	spi_nand_scan_id_table(chip, id);

	chip->mfr_id = id[0];
	chip->dev_id = id[1];
	chip->block_shift = ilog2(chip->block_size);
	chip->page_shift = ilog2(chip->page_size);
	chip->page_mask = chip->page_size - 1;
	chip->lun = 0;
	chip->oobbuf = oob_buffer;
	chip->ecclayout = &micron_ecc_layout_64;
	chip->refresh_threshold = (chip->ecc_strength * 3 + 3) / 4;

	spi_nand_lock_block(chip, BL_ALL_UNLOCKED);
	spi_nand_disable_ecc(chip);

	nand_init = 1;

	return 0;
}

void inline SPI_NAND_set_soft_ecc(int enable) {
	soft_ecc = enable;
}

int inline SPI_NAND_get_soft_ecc(void) {
	return soft_ecc;
}

int spi_nand_cmd_read_ops(struct spi_nand_chip *chip, u32 offset,
		size_t len, void *data)
{
	size_t retlen;
	size_t leftlen = len;
	size_t readlen;
	size_t block_len, block_off;
	loff_t block_start;
	u32 readoffset;
	int ret = 0;
	bool end;

	while (leftlen > 0) {
		if (offset >= chip->size)
			return -1;

		readoffset = offset;
		readlen = 0;
		end = false;
		while ((readlen < leftlen) && !end) {
			block_start = offset & ~(loff_t)(chip->block_size - 1);
			block_off = offset & (chip->block_size - 1);
			block_len = chip->block_size - block_off;

			if (!spi_nand_block_isbad(chip, block_start))
				readlen += block_len;
			else
				end = true;
			offset += block_len;
		}
		if (readlen) {
			readlen = min(readlen, leftlen);
			ret = spi_nand_read(chip, readoffset, readlen, &retlen, data + (len - leftlen));
			if (ret || readlen != retlen)
				return -1;
			leftlen -= readlen;
		}
	}

	return ret;
}

int SPI_NAND_read(u32 offset, u8 *data, u8 chnum, u32 len) {
	int err;

	SPI_Init();

	spi_nand_init(&sfax8_spi_nand);

	err = spi_nand_cmd_read_ops(&sfax8_spi_nand, offset, len, data);

	return err;
}

int SPI_NAND_Erase(u32 offset, u32 len) {
	SPI_Init();

	spi_nand_init(&sfax8_spi_nand);

	return spi_nand_erase(&sfax8_spi_nand, offset, len);
}

int SPI_NAND_write(u8 chnum, u32 page_size, u32 offset, const u8 *buf, u32 len) {
	size_t retlen;
	int err;

	SPI_Init();

	spi_nand_init(&sfax8_spi_nand);

	err = spi_nand_write(&sfax8_spi_nand, offset, len, &retlen, buf);
	if (err)
		return err;
	if (retlen != len)
		return -EINVAL;

	return 0;
}

#else /* SPI_NAND */

int SPI_NAND_read(u32 offset, u8 *data, u8 chnum, u32 len) { return -1; }
int SPI_NAND_write(u8 chnum, u32 page_size, u32 offset, const u8 *buf, u32 len) { return -1; }
int SPI_NAND_Erase(u32 offset, u32 len) { return -1; }
void SPI_NAND_set_soft_ecc(int enable) {}
int inline SPI_NAND_get_soft_ecc(void) { return 0; }

#endif /* SPI_NAND */
