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
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <common.h>
#include <dm.h>
#include <malloc.h>
#include <linux/types.h>
#include <linux/mtd/mtd.h>
#include <spi-nand.h>
#include <asm-generic/errno.h>
#include <spi.h>
#include "nand_ecc.h"

extern int spi_nand_issue_cmd(struct spi_nand_chip *chip, struct spi_nand_cmd *cmd);
static int spi_nand_erase(struct spi_nand_chip *chip, uint64_t addr, uint64_t len, bool check_bad);
static int spi_nand_do_write_oob(struct spi_nand_chip *chip, loff_t to,
			     struct mtd_oob_ops *ops);
int spi_nand_read_reg(struct spi_nand_chip *chip,
		      uint8_t reg, uint8_t *buf);
static bool soft_ecc = false;

static struct spi_nand_cmd_cfg generic_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_RDM,	3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_LAST,	0,	0,	0,	0},
#ifdef CONFIG_SPI_NAND_USE_SPI_NOR_RD_PROT
	{SPINAND_CMD_READ_FROM_CACHE,		3,	1,	0,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	3,	1,	1,	1},
#else
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
#endif
	{SPINAND_CMD_READ_FROM_CACHE_X2,	2,	1,	1,	2},
	{SPINAND_CMD_READ_FROM_CACHE_DUAL_IO,	2,	2,	1,	2},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_READ_FROM_CACHE_QUAD_IO,	2,	4,	2,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg micron_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_RDM,	3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_LAST,	0,	0,	0,	0},
#ifdef CONFIG_SPI_NAND_USE_SPI_NOR_RD_PROT
	{SPINAND_CMD_READ_FROM_CACHE,		3,	1,	0,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	3,	1,	1,	1},
#else
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
#endif
	{SPINAND_CMD_READ_FROM_CACHE_X2,	2,	1,	1,	2},
	{SPINAND_CMD_READ_FROM_CACHE_DUAL_IO,	2,	2,	1,	2},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_READ_FROM_CACHE_QUAD_IO,	2,	4,	2,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg icmax_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg esmt_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg winbond_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg fudanmicro_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

static struct spi_nand_cmd_cfg macronix_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_INTER_ECC_READ,		0,	0,	1,	0},
	{SPINAND_CMD_END},
};
static struct spi_nand_cmd_cfg toshiba_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};
static struct spi_nand_cmd_cfg gigadevice_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_X4,	2,	1,	1,	4},
	{SPINAND_CMD_BLK_ERASE,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_EXC,			3,	1,	0,	0},
	{SPINAND_CMD_PROG_LOAD,			2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA,	2,	1,	0,	1},
	{SPINAND_CMD_PROG_LOAD_X4,		2,	1,	0,	4},
	{SPINAND_CMD_PROG_LOAD_RDM_DATA_X4,	2,	1,	0,	4},
	{SPINAND_CMD_WR_ENABLE,			0,	0,	0,	0},
	{SPINAND_CMD_WR_DISABLE,		0,	0,	0,	0},
	{SPINAND_CMD_READ_ID,			0,	0,	1,	1},
	{SPINAND_CMD_RESET,			0,	0,	0,	0},
	{SPINAND_CMD_END},
};

/* OOB layout */
static const struct nand_ecclayout micron_ecc_layout_64 = {
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

static const struct nand_ecclayout micron_ecc_layout_128 = {
	.eccbytes = 64,
	.eccpos = {
		64, 65, 66, 67, 68, 69, 70, 71,
		72, 73, 74, 75, 76, 77, 78, 79,
		80, 81, 82, 83, 84, 85, 86, 87,
		88, 89, 90, 91, 92, 93, 94, 95,
		96, 97, 98, 99, 100, 101, 102, 103,
		104, 105, 106, 107, 108, 109, 110, 111,
		112, 113, 114, 115, 116, 117, 118, 119,
		120, 121, 122, 123, 124, 125, 126, 127},
	.oobavail = 62,
	.oobfree = {
		{.offset = 2,
		 .length = 62}, }
};

/**
 * spi_nand_get_ecc_status - decode status regisger to get ecc info
 * Note that this is a general interface, some types of nand flash
 * should be implemented separately according to the datashe
 * @status: status register value to decode
 * return 0 when no bitflips happend,
 * Negative numbers represent uncorrectable errors
 * ohers represent correctable errors and bitflips.
 */
static int spi_nand_get_ecc_status(struct spi_nand_chip *chip,
				   unsigned int status)
{
	switch (status & STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;
		break;
	case STATUS_ECC_HAS_BITFLIPS:
		return 1;
		break;
	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;
		break;
	default:
		break;
	}

	return -EINVAL;
}

static int mx35lf1ge4ab_get_ecc_status(struct spi_nand_chip *chip,
				       unsigned int status)
{
	struct spi_nand_cmd cmd;
	unsigned char inter_ecc;
	int ret;


	switch (status & STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;
		break;
	case STATUS_ECC_HAS_BITFLIPS:
		memset(&cmd, 0, sizeof(struct spi_nand_cmd));
		cmd.cmd = SPINAND_CMD_INTER_ECC_READ;
		cmd.n_rx = 1;
		cmd.rx_buf = &inter_ecc;

		ret = spi_nand_issue_cmd(chip, &cmd);
		if (ret < 0) {
			printf("Read internal ecc status register error\n");
			return 1;
		}

		return (inter_ecc & 0xf);
		break;
	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;
		break;
	default:
		break;
	}

	return -EINVAL;

}

static int gd5f1gq5uexxg_get_ecc_status(struct spi_nand_chip *chip,
				       unsigned int status)
{
	unsigned char inter_ecc;
	int ret;

	switch (status & STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;
		break;
	case STATUS_ECC_HAS_BITFLIPS:
		ret = spi_nand_read_reg(chip, 0xF0, &inter_ecc);
		if (ret < 0) {
			printf("Read internal ecc status register error\n");
			return 1;
		}

		return ((inter_ecc >> 4) & 0x3) + 1;
		break;
	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;
		break;
	default:
		break;
	}

	return -EINVAL;
}

static int tc58cvg0s3hxaix_get_ecc_status(struct spi_nand_chip *chip,
				       unsigned int status)
{
	unsigned char inter_ecc;
	int ret;

	switch (status & STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;
		break;
	case STATUS_ECC_HAS_BITFLIPS:
		ret = spi_nand_read_reg(chip, 0x10, &inter_ecc);
		if (ret < 0) {
			printf("Read internal ecc status register error\n");
			return 1;
		}

		return ((inter_ecc >> 4) & 0xf);
		break;
	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;
		break;
	default:
		break;
	}

	return -EINVAL;
}

static struct spi_nand_flash spi_nand_table[] = {
	#ifdef CONFIG_SPI_NAND_MICRON
	SPI_NAND_INFO("MT29F2G01AAAED", MFR_ID_MICRON, 0x22, 2048, 64, 64, 2048,
			1, SPINAND_NEED_PLANE_SELECT,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	SPI_NAND_INFO("MT29F4G01AAADD", MFR_ID_MICRON, 0x32, 2048, 64, 64, 4096,
			1, SPINAND_NEED_PLANE_SELECT,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	SPI_NAND_INFO("MT29F4G01ABAGD", MFR_ID_MICRON, 0x36, 2048, 128, 64, 2048,
			2, SPINAND_NEED_PLANE_SELECT | SPINAND_NEED_DIE_SELECT,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_128, NULL)),
	SPI_NAND_INFO("MT29F2G01ABAGD", MFR_ID_MICRON, 0x24, 2048, 128, 64, 2048,
			1, SPINAND_NEED_PLANE_SELECT,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_128, NULL)),
	#endif
	#ifdef CONFIG_SPI_NAND_ICMAX
	SPI_NAND_INFO("IMS1G083ZAA1", MFR_ID_ICMAX, 0x12, 2048, 64, 64, 1024,
			1, SPINAND_NO_ECC,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	#endif
	#ifdef CONFIG_SPI_NAND_ESMT
	SPI_NAND_INFO("F50L1G41LB", MFR_ID_ESMT, 0x01, 2048, 64, 64, 1024,
			1, 0,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	#endif
	#ifdef CONFIG_SPI_NAND_WINBOND
	SPI_NAND_INFO("W25N01GV", 0xEF, MFR_ID_WINBOND, 2048, 64, 64, 1024,
			1, 0,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	SPI_NAND_INFO("W25N02KV", 0xEF, 0xaa, 2048, 128, 64, 1024,
			1, 0,
			SPI_NAND_ECCINFO(1,&micron_ecc_layout_64, NULL)),
	#endif
	#ifdef CONFIG_SPI_NAND_MACRONIX
	SPI_NAND_INFO("MX35LF1GE4AB",   MFR_ID_MACRONIX, 0x12, 2048, 64, 64, 1024,
			1, 0,
			SPI_NAND_ECCINFO(4,&micron_ecc_layout_64,
					 mx35lf1ge4ab_get_ecc_status)),

	#endif
	#ifdef CONFIG_SPI_NAND_TOSHIBA
	SPI_NAND_INFO("TC58CVG0S3HxAIx", MFR_ID_TOSHIBA, 0xc2, 2048, 64, 64, 1024,
			1,  0,
			SPI_NAND_ECCINFO(8,&micron_ecc_layout_64,
					 tc58cvg0s3hxaix_get_ecc_status)),
	#endif
	#ifdef CONFIG_SPI_NAND_FUDANMICRO
	SPI_NAND_INFO("FM25S01A", MFR_ID_FUDANMICRO, 0xa1, 2048, 64, 64, 1024,
			1,  0,
			SPI_NAND_ECCINFO(8,&micron_ecc_layout_64,
					 fn25s01a_get_ecc_status)),

	#endif
	#ifdef CONFIG_SPI_NAND_GIGADEVICE
	SPI_NAND_INFO("GD5F1GQ5UExxG", MFR_ID_GIGADEVICE, 0x51, 2048, 64, 64, 1024,
			1, 0,
			SPI_NAND_ECCINFO(4,&micron_ecc_layout_64,
					 gd5f1gq5uexxg_get_ecc_status)),
	#endif
	{.name = NULL},
};

/**
 * spi_nand_read_reg - send command 0Fh to read register
 * @chip: SPI-NAND device structure
 * @reg; register to read
 * @buf: buffer to store value
 */
int spi_nand_read_reg(struct spi_nand_chip *chip,
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
int spi_nand_write_reg(struct spi_nand_chip *chip,
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

	if (chip->options & SPINAND_NO_ECC)
		return 0;

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

	if (chip->options & SPINAND_NO_ECC)
		return 0;

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
static void spi_nand_ecc_status(struct spi_nand_chip *chip,
				unsigned int status,
				unsigned int *corrected,
				unsigned int *ecc_error)
{
	int ret;

	*ecc_error = 0;
	if (chip->ecc_info.get_ecc_status) {
		ret = chip->ecc_info.get_ecc_status(chip, status);
	} else {
		ret = spi_nand_get_ecc_status(chip, status);
	}

	*corrected = ret;
	if (ret < 0) {
		*ecc_error = 1;
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

/**
 * spi_nand_wait - wait until the command is done
 * @chip: SPI-NAND device structure
 * @s: buffer to store status register(can be NULL)
 */
static int spi_nand_wait(struct spi_nand_chip *chip, u8 *s)
{
	unsigned long long start = get_timer(0);;
	u8 status;
	/* set timeout to 1 second */
	int timeout = start + CONFIG_SYS_HZ;
	unsigned long ret = -ETIMEDOUT;
	int count = 0;

	while (get_timer(start) < timeout || count < MIN_TRY_COUNT) {
		spi_nand_read_status(chip, &status);
		if ((status & STATUS_OIP_MASK) == STATUS_READY) {
			ret = 0;
			goto out;
		}
		count++;
	}
out:
	if (s)
		*s = status;

	return ret;
}

/**
 * spi_nand_wait_crbusy - wait until CRBSY is clear
 * @chip: SPI-NAND device structure
 * Description:
 *   Used in READ PAGE CACHE RANDOM(30h) sequence, CRBSY bit clear
 *   means data is transferd from data register to cache register.
 */
static int spi_nand_wait_crbusy(struct spi_nand_chip *chip)
{
	unsigned long long start = get_timer(0);;
	u8 status;
	/* set timeout to 1 second */
	int timeout = start + CONFIG_SYS_HZ;
	unsigned long ret = -ETIMEDOUT;
	int count = 0;

	while (get_timer(start) < timeout || count < MIN_TRY_COUNT) {
		spi_nand_read_status(chip, &status);
		if ((status & STATUS_CRBSY_MASK) == STATUS_READY) {
			ret = 0;
			goto out;
		}
		count++;
	}
out:
	return ret;
}

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
	udelay(2000);

	return 0;
}

/**
 * spi_nand_lock_block - write block lock register to
 * lock/unlock device
 * @spi: spi device structure
 * @lock: value to set to block lock register
 * Description:
 *   After power up, all the Nand blocks are locked.  This function allows
 *   one to unlock the blocks, and so it can be written or erased.
 */
static int spi_nand_lock_block(struct spi_nand_chip *chip, u8 lock)
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
	spi_nand_debug("read ecc from %d\n", ecc_offset);
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
	int ecc_column, size;
	u32 retlen = len;

	spi_nand_debug("%s: page %d, column %d, len %d\n", __func__, page_addr, column, len);
	spi_nand_read_page_to_cache(chip, page_addr);
	ret = spi_nand_wait(chip, &status);
	if (ret < 0) {
		spi_nand_error("error %d waiting page 0x%x to cache\n",
			ret, page_addr);
		return ret;
	}
	if (!(chip->options & SPINAND_NO_ECC) && !ecc_off) {
		spi_nand_ecc_status(chip, status, corrected, &ecc_error);
		if (ecc_error) {
			spi_nand_error("internal ECC error reading page 0x%x\n",
				page_addr);
			ret = -EBADMSG;
		}
	}

	if (!soft_ecc || (column + len > chip->page_size)) {
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

	if (!soft_ecc)
		return 0;

	total_ecc_size = DIV_ROUND_UP(len, ECC_SIZE) * 3;
	calc_ecc = calloc(1, total_ecc_size);

	while (len) {
		if (len < ECC_SIZE) {
			memcpy(tmp, buf_pointer, len);
			memset(&tmp[len], 0xff, ECC_SIZE - len);
			buf_pointer = tmp;
		}
		nand_calculate_ecc(buf_pointer, ECC_SIZE, &calc_ecc[ecc_offset]);
		spi_nand_debug("ecc:%x %x %x write to page %d, oob offset %d\n",
			       calc_ecc[ecc_offset], calc_ecc[ecc_offset + 1], calc_ecc[ecc_offset + 2],
			       page_addr, ecc_offset);
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

	spi_nand_debug("%s: page %d, column %d, len %d\n", __func__, page_addr, column, len);
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
	switch (ops->mode) {

	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_RAW:
		memcpy(oob, chip->oobbuf + ops->ooboffs, len);
		return;

	case MTD_OPS_AUTO_OOB: {
		const struct nand_oobfree *free = chip->ecc_info.ecc_layout->oobfree;
		uint32_t boffs = 0, roffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Read request not from offset 0? */
			if (unlikely(roffs)) {
				if (roffs >= free->length) {
					roffs -= free->length;
					continue;
				}
				boffs = free->offset + roffs;
				bytes = min_t(size_t, len,
					      (free->length - roffs));
				roffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(oob, chip->oobbuf + boffs, bytes);
			oob += bytes;
		}
		return;
	}
	default:
		BUG();
	}
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

	switch (ops->mode) {

	case MTD_OPS_PLACE_OOB:
	case MTD_OPS_RAW:
		memcpy(chip->oobbuf + ops->ooboffs, oob, len);
		return;

	case MTD_OPS_AUTO_OOB: {
		const struct nand_oobfree *free = chip->ecc_info.ecc_layout->oobfree;
		uint32_t boffs = 0, woffs = ops->ooboffs;
		size_t bytes = 0;

		for (; free->length && len; free++, len -= bytes) {
			/* Write request not from offset 0? */
			if (unlikely(woffs)) {
				if (woffs >= free->length) {
					woffs -= free->length;
					continue;
				}
				boffs = free->offset + woffs;
				bytes = min_t(size_t, len,
					      (free->length - woffs));
				woffs = 0;
			} else {
				bytes = min_t(size_t, len, free->length);
				boffs = free->offset;
			}
			memcpy(chip->oobbuf + boffs, oob, bytes);
			oob += bytes;
		}
		return;
	}
	default:
		BUG();
	}
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
	bool ecc_off = ops->mode == MTD_OPS_RAW;
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ? chip->ecc_info.ecc_layout->oobavail : chip->oob_size;
	unsigned int failed = 0;
	int lun_num;

	spi_nand_debug("%s: from = 0x%012llx, len = %i\n",
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

		if (unlikely(ops->oobbuf)) {
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
	if (max_bitflip > chip->refresh_threshold)
		ret = -EUCLEAN;
	if (failed)
		ret = -EBADMSG;

	return ret;
}

/**
 * spi_nand_read_pages_fast - read data from flash to buffer
 * @chip: spi nand chip structure
 * @from: offset to read from
 * @ops: oob operations description structure
 * Description:
 *   Advanced read function, use READ PAGE CACHE RANDOM to
 *   speed up read. Return -EUCLEAN when bitflip is over threshold.
 *   Return -EBADMSG when internal ecc can not correct bitflips.
 */
static int spi_nand_read_pages_fast(struct spi_nand_chip *chip,
			loff_t from, struct mtd_oob_ops *ops)
{
	int page_addr, page_offset, size;
	int ret;
	unsigned int corrected = 0;
	unsigned int max_bitflip = 0;
	int readlen = ops->len;
	int oobreadlen = ops->ooblen;
	bool ecc_off = ops->mode == MTD_OPS_RAW, cross_lun = false;
	bool read_ramdon_issued = false;
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ? chip->ecc_info.ecc_layout->oobavail : chip->oob_size;
	u8 status;
	unsigned int ecc_error;
	unsigned int failed = 0;
	int lun_num;

	page_addr = from >> chip->page_shift;
	page_offset = from & chip->page_mask;
	ops->retlen = 0;
	lun_num = from >> chip->lun_shift;
again:
	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	spi_nand_read_page_to_cache(chip, page_addr);
	ret = spi_nand_wait(chip, &status);
	if (ret < 0) {
		spi_nand_error("error %d waiting page 0x%x to cache\n",
			ret, page_addr);
		return ret;
	}
	while ((page_offset + readlen > chip->page_size) && !cross_lun) {
		if (!(chip->options & SPINAND_NEED_DIE_SELECT) ||
		(page_addr + 1) & ((1 << (chip->lun_shift - chip->page_shift)) - 1)) {
			read_ramdon_issued = true;
			spi_nand_read_page_cache_random(chip, page_addr + 1);
			ret = spi_nand_wait(chip, &status);
			if (ret < 0) {
				spi_nand_error("error %d waiting page 0x%x to data resigter\n",
					ret, page_addr + 1);
				return ret;
			}
		} else {
			cross_lun = true;
			break;
		}
		if (!ecc_off) {
			spi_nand_ecc_status(chip, status, &corrected, &ecc_error);
			if (ecc_error) {
				spi_nand_error("internal ECC error reading page 0x%x\n",
					page_addr);
				failed++;
			}
		}
		max_bitflip = max(corrected, max_bitflip);
		size = min(readlen, chip->page_size - page_offset);
		spi_nand_read_from_cache(chip, page_addr, page_offset, size,
				ops->datbuf + ops->retlen);
		page_offset = 0;
		ops->retlen += size;
		readlen -= size;
		if (unlikely(ops->oobbuf)) {
			size = min(oobreadlen, ooblen);
			spi_nand_read_from_cache(chip, page_addr,
				chip->page_size, chip->oob_size, chip->oobbuf);
			spi_nand_transfer_oob(chip,
				ops->oobbuf + ops->oobretlen, ops, size);
			ops->oobretlen += size;
			oobreadlen -= size;
		}
		if (!cross_lun) {
			ret = spi_nand_wait_crbusy(chip);
			if (ret < 0) {
				spi_nand_error("error %d waiting page 0x%x to cache\n",
					ret, page_addr + 1);
				return ret;
			}
		}
		page_addr++;
	}
	if (read_ramdon_issued) {
		read_ramdon_issued = false;
		spi_nand_read_page_cache_last(chip);
		/*
		* Already check ecc status in loop, no need to check again
		*/
		ret = spi_nand_wait(chip, &status);
		if (ret < 0) {
			spi_nand_error("error %d waiting page 0x%x to cache\n",
				ret, page_addr);
			return ret;
		}
	}
	if (!ecc_off) {
		spi_nand_ecc_status(chip, status, &corrected, &ecc_error);
		if (ecc_error) {
			spi_nand_error("internal ECC error reading page 0x%x\n",
				page_addr);
			failed++;
		}
	}
	max_bitflip = max(corrected, max_bitflip);
	size = min(readlen, chip->page_size - page_offset);
	spi_nand_read_from_cache(chip, page_addr, page_offset, size,
			ops->datbuf + ops->retlen);
	ops->retlen += size;
	readlen -= size;
	if (unlikely(ops->oobbuf)) {
		size = min(oobreadlen, ooblen);
		spi_nand_read_from_cache(chip, page_addr,
			chip->page_size, chip->oob_size, chip->oobbuf);
		spi_nand_transfer_oob(chip,
			ops->oobbuf + ops->oobretlen, ops, size);
		ops->oobretlen += size;
		oobreadlen -= size;
	}
	if (cross_lun) {
		cross_lun = false;
		page_addr++;
		page_offset = 0;
		lun_num++;
		goto again;
	}
	if (max_bitflip > chip->refresh_threshold)
		ret = -EUCLEAN;
	if (failed)
		ret = -EBADMSG;

	return ret;
}

static inline bool is_read_page_fast_benefit(struct spi_nand_chip *chip,
			loff_t from, size_t len)
{
	if (!spi_nand_cmd_supported(chip, SPINAND_CMD_READ_PAGE_CACHE_RDM) ||
	    !spi_nand_cmd_supported(chip, SPINAND_CMD_READ_PAGE_CACHE_LAST))
		return false;

	if (len < chip->page_size << 2)
		return false;
	if (from >> chip->lun_shift == (from + len) >> chip->lun_shift)
		return true;
	if (((1 << chip->lun_shift) - from) >= (chip->page_size << 2) ||
		(from + len - (1 << chip->lun_shift)) >= (chip->page_size << 2))
		return true;
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
	bool ecc_off = ops->mode == MTD_OPS_RAW;
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ?
		chip->ecc_info.ecc_layout->oobavail : chip->oob_size;

	spi_nand_debug("%s: from = 0x%012llx, len = %i\n",
			 __func__, from, ops->len);
	/* Do not allow reads past end of device */
	if (unlikely(from >= chip->size)) {
		spi_nand_error("%s: attempt to read beyond end of device\n",
				__func__);
		return -EINVAL;
	}

	/* for oob */
	if (oobreadlen > 0) {
		if (unlikely(ops->ooboffs >= ooblen)) {
			spi_nand_error("%s: attempt to start read outside oob\n",
					__func__);
			return -EINVAL;
		}

		if (unlikely(ops->ooboffs + oobreadlen >
		((chip->size >> chip->page_shift) - (from >> chip->page_shift))
		* ooblen)) {
			spi_nand_error("%s: attempt to read beyond end of device\n",
					__func__);
			return -EINVAL;
		}
		ooblen -= ops->ooboffs;
		ops->oobretlen = 0;
	}

	if (ecc_off)
		spi_nand_disable_ecc(chip);

	if (is_read_page_fast_benefit(chip, from, ops->len))
		ret = spi_nand_read_pages_fast(chip, from, ops);
	else
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
	int ooblen = (ops->mode == MTD_OPS_AUTO_OOB) ?
		chip->ecc_info.ecc_layout->oobavail : chip->oob_size;
	bool ecc_off = ops->mode == MTD_OPS_RAW;
	bool clr_cache = true;
	int lun_num;


	spi_nand_debug("%s: to = 0x%012llx, len = %i\n",
			 __func__, to, writelen);
	/* Do not allow reads past end of device */
	if (unlikely(to >= chip->size)) {
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
		if (unlikely(ops->ooboffs >= ooblen)) {
			spi_nand_error("%s: attempt to start write outside oob\n",
					__func__);
			return -EINVAL;
		}
		if (unlikely(ops->ooboffs + oobwritelen >
		((chip->size >> chip->page_shift) - (to >> chip->page_shift))
			* ooblen)) {
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
		if (unlikely(ops->oobbuf)) {
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
static int spi_nand_read(struct spi_nand_chip *chip, loff_t from, size_t len,
	size_t *retlen, u8 *buf)
{
	struct mtd_oob_ops ops;
	int ret;

	memset(&ops, 0, sizeof(ops));
	ops.len = len;
	ops.datbuf = buf;
	if (chip->options & SPINAND_NO_ECC)
		ops.mode = MTD_OPS_RAW;
	else
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
static int spi_nand_write(struct spi_nand_chip *chip, loff_t to, size_t len,
	size_t *retlen, const u8 *buf)
{
	struct mtd_oob_ops ops;
	int ret;

	memset(&ops, 0, sizeof(ops));
	ops.len = len;
	ops.datbuf = (uint8_t *)buf;
	if (chip->options & SPINAND_NO_ECC)
		ops.mode = MTD_OPS_RAW;
	else
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
		chip->ecc_info.ecc_layout->oobavail : chip->oob_size;
	int ooboffs = ops->ooboffs;
	bool ecc_off = ops->mode == MTD_OPS_RAW;
	unsigned int failed = 0;
	int ret = 0;
	int lun_num;

	spi_nand_debug("%s: from = 0x%012llx, len = %i\n",
			 __func__, from, readlen);
	if (unlikely(ooboffs >= max_len)) {
		spi_nand_error("%s: attempt to read outside oob\n",
				__func__);
		return -EINVAL;
	}
	if (unlikely(from >= chip->size ||
		ooboffs + readlen > ((chip->size >> chip->page_shift) -
							(from >> chip->page_shift)) * max_len)) {
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
		chip->ecc_info.ecc_layout->oobavail : chip->oob_size;
	int ooboffs = ops->ooboffs;
	bool ecc_off = ops->mode == MTD_OPS_RAW;
	int writelen = ops->ooblen;
	int lun_num;

	spi_nand_debug("%s: to = 0x%012llx, len = %i\n",
			 __func__, to, writelen);

	/* Do not allow write past end of page */
	if (unlikely(ooboffs > max_len)) {
		spi_nand_error("%s: attempt to write outside oob\n",
				__func__);
		return -EINVAL;
	}
	if (unlikely(to >= chip->size ||
		ooboffs + writelen > ((chip->size >> chip->page_shift) -
							(to >> chip->page_shift)) * max_len)) {
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

#if 0
/**
 * spi_nand_read_oob - [Interface] SPI-NAND read data and/or out-of-band
 * @chip: spi nand device structure
 * @from: offset to read from
 * @ops: oob operation description structure
 */
static int spi_nand_read_oob(struct spi_nand_chip *chip, loff_t from,
			struct mtd_oob_ops *ops)
{
	int ret = -ENOTSUPP;

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
static int spi_nand_write_oob(struct spi_nand_chip *chip, loff_t to,
			  struct mtd_oob_ops *ops)
{
	int ret = -ENOTSUPP;

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
#endif

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
	spi_nand_erase(chip, block_addr << chip->block_shift, chip->block_size, false);
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
static int spi_nand_erase(struct spi_nand_chip *chip, uint64_t addr, uint64_t len, bool check_bad)
{
	int page_addr, pages_per_block;
	u8 status;
	int ret = 0;
	int lun_num;

	spi_nand_debug("%s: address = 0x%012llx, len = %llu\n",
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
		if (check_bad && spi_nand_block_isbad(chip, ((loff_t) page_addr) <<
					chip->page_shift)) {
			spi_nand_error("%s: attempt to erase a bad block at 0x%012llx\n",
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
			spi_nand_error("erase block 0x%012llx failed\n",
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
	struct spi_nand_flash *type = spi_nand_table;

	for (; type->name; type++) {
		if (id[0] == type->mfr_id && id[1] == type->dev_id) {
			chip->name = type->name;
			chip->size = type->page_size * type->pages_per_blk
				* type->blks_per_lun * type->luns_per_chip;
			chip->block_size = type->page_size
					* type->pages_per_blk;
			chip->page_size = type->page_size;
			chip->oob_size = type->oob_size;
			chip->lun_shift = ilog2(chip->block_size * type->blks_per_lun);
			chip->options = type->options;
			chip->ecc_info = type->ecc_info;

			return true;
		}
	}

	return false;
}

static u16 onfi_crc16(u16 crc, u8 const *p, size_t len)
{
	int i;

	while (len--) {
		crc ^= *p++ << 8;
		for (i = 0; i < 8; i++)
			crc = (crc << 1) ^ ((crc & 0x8000) ? 0x8005 : 0);
	}

	return crc;
}

/* Sanitize ONFI strings so we can safely print them */
static void sanitize_string(char *s, size_t len)
{
	int i = len - 1;
	int j = 0;

	/* Null terminate */
	s[i--] = 0;

	/* Remove unnecessary space */
	while (i >= 0 && (s[i] <= ' ' || s[i] > 127)) {
		s[i--] = 0;
	}
	/* Remove non printable chars */
	for (j = 0; j <= i; j++) {
		if (s[j] < ' ' || s[j] > 127)
			s[j] = '?';
	}
}

/**
 * spi_nand_detect_onfi - config chip with parameter page
 * @chip: SPI-NAND device structure
 * Description:
 *   This function is called when we can not get info from id table.
 */
static bool spi_nand_detect_onfi(struct spi_nand_chip *chip)
{
	struct spi_nand_onfi_params *p;
	u8 *buffer;
	int read_cache_op;
	bool ret = true;
	int i;

	buffer = malloc(256 * 3);
	spi_nand_change_mode(chip, OTP_MODE);
	spi_nand_read_page_to_cache(chip, 0x01);
	spi_nand_wait(chip, NULL);
	/*
	* read parameter page can only ues 1-1-1 mode
	*/
	read_cache_op = chip->read_cache_op;
	chip->read_cache_op = SPINAND_CMD_READ_FROM_CACHE;
	spi_nand_read_from_cache(chip, 0x01, 0, 256 * 3, buffer);
	chip->read_cache_op = read_cache_op;
	spi_nand_change_mode(chip, NORMAL_MODE);

	p = (struct spi_nand_onfi_params *)buffer;
	for (i = 0; i < 3; i++, p++) {
		if (p->sig[0] != 'O' || p->sig[1] != 'N' ||
				p->sig[2] != 'F' || p->sig[3] != 'I')
			continue;
		if (onfi_crc16(ONFI_CRC_BASE, (uint8_t *)p, 254) ==
				le16_to_cpu(p->crc))
			break;
	}
	if (i == 3) {
		spi_nand_error("Could not find valid ONFI parameter page; aborting\n");
		ret = false;
		goto out;
	}

	memcpy(&chip->onfi_params, p, sizeof(*p));

	p = &chip->onfi_params;

	sanitize_string(p->manufacturer, sizeof(p->manufacturer));
	sanitize_string(p->model, sizeof(p->model));

	chip->name = p->model;
	chip->size = le32_to_cpu(p->byte_per_page) *
			le32_to_cpu(p->pages_per_block) *
			le32_to_cpu(p->blocks_per_lun) * p->lun_count;
	chip->block_size = le32_to_cpu(p->byte_per_page) *
			le32_to_cpu(p->pages_per_block);
	chip->page_size = le32_to_cpu(p->byte_per_page);
	chip->oob_size = le16_to_cpu(p->spare_bytes_per_page);
	chip->lun_shift = ilog2(chip->block_size * le32_to_cpu(p->blocks_per_lun));
	if (p->vendor.micron_sepcific.two_plane_page_read)
		chip->options |= SPINAND_NEED_PLANE_SELECT;
	if (p->vendor.micron_sepcific.die_selection)
		chip->options |= SPINAND_NEED_DIE_SELECT;
	chip->ecc_info.ecc_strength = p->vendor.micron_sepcific.ecc_ability;

out:
	free(buffer);
	return ret;
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
	struct spi_slave *spi = chip->spi;

	if (spi->mode_rx & SPI_RX_QUAD)
		chip->read_cache_op = SPINAND_CMD_READ_FROM_CACHE_QUAD_IO;
	else if (spi->mode_rx & SPI_RX_DUAL)
		chip->read_cache_op = SPINAND_CMD_READ_FROM_CACHE_DUAL_IO;
	else
		chip->read_cache_op = SPINAND_CMD_READ_FROM_CACHE_FAST;

	if (spi->mode & SPI_TX_QUAD) {
		chip->write_cache_op = SPINAND_CMD_PROG_LOAD_X4;
		chip->write_cache_rdm_op = SPINAND_CMD_PROG_LOAD_RDM_DATA_X4;
	} else {
		chip->write_cache_op = SPINAND_CMD_PROG_LOAD;
		chip->write_cache_rdm_op = SPINAND_CMD_PROG_LOAD_RDM_DATA;
	}
}

int spi_nand_bind_cfg_table(struct spi_nand_chip *chip)
{
	if (chip->mfr_id == MFR_ID_MICRON) {
		chip->cfg_table = micron_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_ICMAX) {
		chip->cfg_table = icmax_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_ESMT) {
		chip->cfg_table = esmt_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_WINBOND) {
		chip->cfg_table = winbond_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_MACRONIX) {
		chip->cfg_table = macronix_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_TOSHIBA) {
		chip->cfg_table = toshiba_cmd_cfg_table;
	} else if (chip->mfr_id == 0xa1) {
		chip->cfg_table = fudanmicro_cmd_cfg_table;
	} else if (chip->mfr_id == MFR_ID_GIGADEVICE) {
		chip->cfg_table = gigadevice_cmd_cfg_table;
	} else {
		printf("Warning:Not specified [%x:%x] cmd config table. \
			      Use generic cmd config table.\n", chip->mfr_id, chip->dev_id);
		chip->cfg_table = generic_cmd_cfg_table;
	}
	return 0;
}


/**
 * spi_nand_init - [Interface] Init SPI-NAND device driver
 * @spi: spi device structure
 * @chip_ptr: pointer point to spi nand device structure pointer
 */
static int spi_nand_init(struct spi_slave *spi, struct spi_nand_chip **chip_ptr)
{
	u8 id[SPINAND_MAX_ID_LEN] = {0};
	struct spi_nand_chip *chip = NULL;

	if (!*chip_ptr) {
		chip = malloc(sizeof(struct spi_nand_chip));
		if (!chip)
			return -ENOMEM;
		memset(chip, 0, sizeof(struct spi_nand_chip));
	} else {
		chip = *chip_ptr;
	}

	chip->spi = spi;

	/*
	 * Use generic cmd config table until read flash id
	 * */
	chip->cfg_table = generic_cmd_cfg_table;

	spi_nand_set_rd_wr_op(chip);
	spi_nand_reset(chip);
	spi_nand_read_id(chip, id);

	if (spi_nand_scan_id_table(chip, id))
		goto ident_done;
	spi_nand_info("SPI-NAND type mfr_id: %x, dev_id: %x is not in id table.\n",
				id[0], id[1]);

	if (spi_nand_detect_onfi(chip))
		goto ident_done;

	return -ENODEV;

ident_done:
	spi_nand_info("SPI-NAND: %s is found.\n", chip->name);

	chip->mfr_id = id[0];
	chip->dev_id = id[1];
	chip->block_shift = ilog2(chip->block_size);
	chip->page_shift = ilog2(chip->page_size);
	chip->page_mask = chip->page_size - 1;
	chip->lun = 0;

	chip->oobbuf = malloc(chip->oob_size);
	if (!chip->oobbuf)
		return -ENOMEM;

	chip->refresh_threshold = (chip->ecc_info.ecc_strength * 3 + 3) / 4;

	spi_nand_bind_cfg_table(chip);

	spi_nand_lock_block(chip, BL_ALL_UNLOCKED);
	spi_nand_enable_ecc(chip);
	if (!*chip_ptr)
		*chip_ptr = chip;

	return 0;
}

int spi_nand_cmd_write_ops(struct spi_nand_chip *chip, u32 offset,
		size_t len, const void *buf)
{
	size_t retlen;
	size_t leftlen = len;
	size_t writelen;
	size_t block_len, block_off;
	loff_t block_start;
	u32 writeoffset;
	int ret = 0;
	bool end;

	while (leftlen > 0) {
		if (offset >= chip->size)
			return -1;

		writeoffset = offset;
		writelen = 0;
		end = false;
		while ((writelen < leftlen) && !end) {
			block_start = offset & ~(loff_t)(chip->block_size - 1);
			block_off = offset & (chip->block_size - 1);
			block_len = chip->block_size - block_off;

			if (!spi_nand_block_isbad(chip, block_start))
				writelen += block_len;
			else
				end = true;
			offset += block_len;
		}
		if (writelen) {
			writelen = min(writelen, leftlen);
			ret = spi_nand_write(chip, writeoffset, writelen, &retlen, buf + (len - leftlen));
			if (ret || writelen != retlen)
				return -1;
			leftlen -= writelen;
		}
	}

	return ret;
}

int spi_nand_cmd_erase_ops(struct spi_nand_chip *chip, u32 offset, size_t len, int spread)
{
	size_t leftlen = len;
	size_t eraselen;
	u32 eraseoffset;
	u64 endaddr = offset + len;
	int ret = 0;
	bool end;

	if (offset & (chip->block_size - 1)) {
		spi_nand_error("%s: Unaligned address\n", __func__);
		return -EINVAL;
	}

	if (len & (chip->block_size - 1)) {
		spi_nand_error("%s: Length not block aligned\n", __func__);
		return -EINVAL;
	}

	if (endaddr > chip->size) {
		spi_nand_error("%s: Erase past end of device\n", __func__);
		return -EINVAL;
	}

	while (leftlen > 0) {
		if (offset >= chip->size)
			return -1;

		eraseoffset = offset;
		eraselen = 0;
		end = false;
		while ((eraselen < leftlen) && !end) {
			if ((spread == 2) || (!spi_nand_block_isbad(chip, offset)))
				eraselen += chip->block_size;
			else
				end = true;
			offset += chip->block_size;
		}
		if (eraselen) {
			ret = spi_nand_erase(chip, eraseoffset, eraselen, false);
			if (ret)
				return -1;
		}
		if (spread)
			leftlen -= eraselen;
		else
			leftlen = endaddr - offset;
	}

	return ret;
}

int spi_nand_cmd_reset_ops(struct spi_nand_chip *chip)
{
	size_t leftlen = chip->size;
	int page_addr, pages_per_block;
	int ret = 0;
	u8 status;

	pages_per_block = 1 << (chip->block_shift - chip->page_shift);
	page_addr = 0;

	while (leftlen) {
		spi_nand_write_enable(chip);
		spi_nand_erase_block(chip, page_addr);
		ret = spi_nand_wait(chip, &status);
		if (ret < 0) {
			spi_nand_error("block erase command wait failed\n");
			goto erase_exit;
		}
		if ((status & STATUS_E_FAIL_MASK) == STATUS_E_FAIL) {
			spi_nand_error("erase block 0x%012llx failed\n",
				       ((loff_t)page_addr) << chip->page_shift);
			ret = -EIO;
			goto erase_exit;
		}

		/* Increment page address and decrement length */
		leftlen -= (1ULL << chip->block_shift);
		page_addr += pages_per_block;
	}

erase_exit:
	return ret;
}

int spi_nand_cmd_read_pages(struct spi_nand_chip *chip, u32 from, u_char *buf, size_t len)
{
	int page_addr, page_offset;
	int ret;
	u32 correct;
	bool ecc_off = true;
	unsigned int failed = 0;
	int lun_num;

	spi_nand_debug("%s: from = 0x%x, len = %i\n",
			 __func__, from, len);

	page_addr = from >> chip->page_shift;
	page_offset = from & chip->page_mask;

	if (page_offset + len > chip->page_size + chip->oob_size)
		len = chip->page_size + chip->oob_size - page_offset;

	lun_num = from >> chip->lun_shift;
	if (chip->options & SPINAND_NEED_DIE_SELECT)
		spi_nand_lun_select(chip, lun_num);

	ret = spi_nand_do_read_page(chip, page_addr, page_offset, ecc_off, &correct, buf, len);
	if (ret == -EBADMSG)
		failed++;
	else if (ret) {
		spi_nand_error("error %d reading page 0x%x\n",
			ret, page_addr);
		goto out;
	}

out:
	if (failed)
		ret = -EBADMSG;

	return ret;
}

int spi_nand_cmd_write_oob(struct spi_nand_chip *chip, u32 to, u_char *buf, size_t len)
{
	struct mtd_oob_ops ops = {0};

	ops.mode = MTD_OPS_PLACE_OOB;
	ops.ooblen = len;
	ops.oobbuf = buf;
	ops.ooboffs = to & chip->page_mask;

	return spi_nand_do_write_oob(chip, to, &ops);
}

int spi_nand_cmd_soft_ecc(bool enable)
{
	soft_ecc = enable;
	return 0;
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

int spi_nand_probe_slave(struct spi_slave *spi, struct spi_nand_chip **chip)
{
	int ret;

	/* Setup spi_slave */
	if (!spi) {
		printf("SPI-NAND: Failed to set up slave\n");
		return -ENODEV;
	}

	/* Claim spi bus */
	ret = spi_claim_bus(spi);
	if (ret) {
		printf("SPI-NAND: Failed to claim SPI bus: %d\n", ret);
		return ret;
	}

	ret = spi_nand_init(spi, chip);
	if (ret) {
		printf("SPI NAND init failed\n");
		goto err_init;
	}

err_init:
	/* Release spi bus */
	spi_release_bus(spi);

	return ret;
}

/**
 * spi_nand_release - [Interface] Release function
 * @chip: spi nand device structure
 */
void spi_nand_release(struct spi_nand_chip *chip)
{
	free(chip->oobbuf);
	free(chip);

	return;
}

#ifdef CONFIG_DM_SPI_FLASH
static int spi_nand_flash_do_probe(struct udevice *dev)
{
	struct spi_slave *slave = dev_get_parent_priv(dev);
	struct spi_nand_chip *chip;

	chip = dev_get_uclass_priv(dev);
	chip->dev = dev;
	chip->spi = slave;
	spi_nand_debug("%s: slave=%p\n", __func__, slave);
	return spi_nand_probe_slave(slave, &chip);
}

int spi_nand_probe_bus_cs(unsigned int busnum, unsigned int cs,
			   unsigned int max_hz, unsigned int spi_mode,
			   struct udevice **devp)
{
	struct spi_slave *slave;
	struct udevice *bus;
	char *str;
	int ret;

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_USE_TINY_PRINTF)
	str = "spi_nand";
#else
	char name[30];

	snprintf(name, sizeof(name), "spi_nand@%d:%d", busnum, cs);
	str = strdup(name);
#endif
	ret = spi_get_bus_and_cs(busnum, cs, max_hz, spi_mode,
				  "spi_nand_flash", str, &bus, &slave);
	if (ret)
		return ret;

	*devp = slave->dev;
	return 0;
}

static const struct udevice_id spi_nand_flash_ids[] = {
	{ .compatible = "spi-nand" },
	{ }
};

UCLASS_DRIVER(spi_nand_flash) = {
	.id		= UCLASS_SPI_NAND,
	.name		= "spi_nand",
	.per_device_auto_alloc_size = sizeof(struct spi_nand_chip),
};

U_BOOT_DRIVER(spi_nand_flash) = {
	.name		= "spi_nand_flash",
	.id		= UCLASS_SPI_NAND,
	.of_match	= spi_nand_flash_ids,
	.probe		= spi_nand_flash_do_probe,
	.priv_auto_alloc_size = sizeof(struct spi_nand_chip),
};
#else
int spi_nand_probe_bus_cs(unsigned int busnum, unsigned int cs,
			   unsigned int max_hz, unsigned int spi_mode,
			   struct udevice **devp)
{
	return -1;
}
#endif // CONFIG_DM_SPI_FLASH
