/**
* spi-nand-cmd.c
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
#include "spi-nand.h"
#include "errno.h"
#include "external.h"
#include "spi.h"

#ifdef SPI_NAND

struct spi_nand_cmd_cfg {
	u8		opcode;
	u8		addr_bytes;
	u8		addr_bits;
	u8		dummy_bytes;
	u8		data_bits;
};


static struct spi_nand_cmd_cfg spi_nand_cmd_cfg_table[] = {
/*opcode	addr_bytes	addr_bits	dummy_bytes	data_nbits*/
	{SPINAND_CMD_GET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_SET_FEATURE,		1,	1,	0,	1},
	{SPINAND_CMD_PAGE_READ,			3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_RDM,	3,	1,	0,	0},
	{SPINAND_CMD_READ_PAGE_CACHE_LAST,	0,	0,	0,	0},
	{SPINAND_CMD_READ_FROM_CACHE,		2,	1,	1,	1},
	{SPINAND_CMD_READ_FROM_CACHE_FAST,	2,	1,	1,	1},
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

static struct spi_nand_cmd_cfg *spi_nand_lookup_cmd_cfg_table(u8 opcode,
				struct spi_nand_cmd_cfg *table)
{
	struct spi_nand_cmd_cfg *index = table;

	for (; index->opcode != SPINAND_CMD_END; index++) {
		if (index->opcode == opcode)
			return index;
	}

	printf("Invalid spi nand opcode %x\n", opcode);

	return NULL;
}

bool spi_nand_cmd_supported(struct spi_nand_chip *chip, u8 opcode)
{
	struct spi_nand_cmd_cfg *table = spi_nand_cmd_cfg_table;

	for (; table->opcode != SPINAND_CMD_END; table++) {
		if (table->opcode == opcode)
			return true;
	}
	return false;
}

static int spi_nand_xfer(struct spi_slave *slave, unsigned int bitlen, u8 bits,
			const void *dout, void *din, unsigned long flags)
{
	// if (bits == 4)
	// 	flags |= SPI_XFER_QUAD;
	// else if (bits == 2)
	// 	flags |= SPI_XFER_DUAL;

	return SPI_xfer(bitlen, dout, din, flags);
}

/*
 * spi_nand_issue_cmd - to process a command to send to the SPI-NAND
 * @spi: spi device structure
 * @cmd: command structure
 *
 *    Set up the command buffer to send to the SPI controller.
 *    The command buffer has to initialized to 0.
 */
int spi_nand_issue_cmd(struct spi_nand_chip *chip, struct spi_nand_cmd *cmd)
{
	struct spi_nand_cmd_cfg *cmd_cfg = NULL;
	struct spi_slave *spi = chip->spi;
	int flags = SPI_XFER_BEGIN;
	u8 buf[SPINAND_MAX_ADDR_LEN];

	cmd_cfg = spi_nand_lookup_cmd_cfg_table(cmd->cmd, spi_nand_cmd_cfg_table);

	if (!cmd_cfg)
		return -EINVAL;

	if (cmd->n_tx == 0 && cmd->n_rx == 0 && cmd_cfg->addr_bytes == 0 && cmd_cfg->dummy_bytes == 0)
		flags |= SPI_XFER_END;

	spi_nand_xfer(spi, 8, 1, &cmd->cmd, NULL, flags);
	if (cmd_cfg->addr_bytes || cmd_cfg->dummy_bytes) {
		flags = 0;
		if (cmd->n_tx == 0 && cmd->n_rx == 0)
			flags |= SPI_XFER_END;
		if (cmd_cfg->addr_bytes > cmd->n_addr) {
			memcpy(buf, cmd->addr, cmd->n_addr);
			memset(cmd->addr, 0, cmd->n_addr);
			memcpy(cmd->addr + cmd_cfg->addr_bytes - cmd->n_addr, buf, cmd->n_addr);
		}
		spi_nand_xfer(spi, (cmd_cfg->addr_bytes + cmd_cfg->dummy_bytes) * 8, cmd_cfg->addr_bits,
				cmd->addr, NULL, flags);
	}

	if (cmd->n_tx) {
		spi_nand_xfer(spi, cmd->n_tx * 8, cmd_cfg->data_bits, cmd->tx_buf, NULL, SPI_XFER_END);
	} else if (cmd->n_rx) {
		spi_nand_xfer(spi, cmd->n_rx * 8, cmd_cfg->data_bits, NULL, cmd->rx_buf, SPI_XFER_END);
	}

	return 0;
}
#endif /* SPI_NAND */
