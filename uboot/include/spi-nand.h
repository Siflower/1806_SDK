/**
* spi-nand.h
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
#ifndef _SPI_NAND_H
#define _SPI_NAND_H

#include <linux/types.h>
#include <linux/mtd/mtd.h>
#define MFR_ID_MICRON		0x2C
#define MFR_ID_ICMAX		0x9B
#define MFR_ID_ESMT		0xC8
#define MFR_ID_WINBOND		0xEF
#define MFR_ID_MACRONIX		0xC2
#define MFR_ID_TOSHIBA		0x98
#define MFR_ID_GIGADEVICE	0xC8

/*
 * Standard SPI-NAND flash commands
 */
#define SPINAND_CMD_RESET			0xff
#define SPINAND_CMD_GET_FEATURE			0x0f
#define SPINAND_CMD_SET_FEATURE			0x1f
#define SPINAND_CMD_PAGE_READ			0x13
#define SPINAND_CMD_READ_PAGE_CACHE_RDM		0x30
#define SPINAND_CMD_READ_PAGE_CACHE_LAST	0x3f
#define SPINAND_CMD_READ_FROM_CACHE		0x03
#define SPINAND_CMD_READ_FROM_CACHE_FAST	0x0b
#define SPINAND_CMD_READ_FROM_CACHE_X2		0x3b
#define SPINAND_CMD_READ_FROM_CACHE_DUAL_IO	0xbb
#define SPINAND_CMD_READ_FROM_CACHE_X4		0x6b
#define SPINAND_CMD_READ_FROM_CACHE_QUAD_IO	0xeb
#define SPINAND_CMD_BLK_ERASE			0xd8
#define SPINAND_CMD_PROG_EXC			0x10
#define SPINAND_CMD_PROG_LOAD			0x02
#define SPINAND_CMD_PROG_LOAD_RDM_DATA		0x84
#define SPINAND_CMD_PROG_LOAD_X4		0x32
#define SPINAND_CMD_PROG_LOAD_RDM_DATA_X4	0x34
#define SPINAND_CMD_READ_ID			0x9f
#define SPINAND_CMD_WR_DISABLE			0x04
#define SPINAND_CMD_WR_ENABLE			0x06
#define SPINAND_CMD_INTER_ECC_READ		0x7c
#define SPINAND_CMD_END				0x0


/* feature registers */
#define REG_BLOCK_LOCK		0xa0
#define REG_CFG			0xb0
#define REG_STATUS		0xc0
#define REG_DIE_SELECT		0xd0

/* status */
#define STATUS_OIP_MASK		0x01
#define STATUS_CRBSY_MASK		0x80
#define STATUS_READY		(0 << 0)
#define STATUS_BUSY		(1 << 0)

#define STATUS_E_FAIL_MASK	0x04
#define STATUS_E_FAIL		(1 << 2)

#define STATUS_P_FAIL_MASK	0x08
#define STATUS_P_FAIL		(1 << 3)


/*Configuration register defines*/
#define CFG_ECC_MASK		0X10
#define CFG_ECC_ENABLE		0x10
#define CFG_LOT_MASK		0x20
#define CFG_LOT_ENABLE		0x20
#define CFG_OTP_MASK		0xc2
#define CFG_OTP_ENTER		0x40
#define CFG_OTP_EXIT		0x00

/* block lock */
#define BL_ALL_LOCKED		0x7c
#define BL_U_1_1024_LOCKED		0x08
#define BL_U_1_512_LOCKED		0x10
#define BL_U_1_256_LOCKED		0x18
#define BL_U_1_128_LOCKED		0x20
#define BL_U_1_64_LOCKED		0x28
#define BL_U_1_32_LOCKED		0x30
#define BL_U_1_16_LOCKED		0x38
#define BL_U_1_8_LOCKED		0x40
#define BL_U_1_4_LOCKED		0x48
#define BL_U_1_2_LOCKED		0x50
#define BL_L_1_1024_LOCKED		0x0c
#define BL_L_1_512_LOCKED		0x14
#define BL_L_1_256_LOCKED		0x1c
#define BL_L_1_128_LOCKED		0x24
#define BL_L_1_64_LOCKED		0x2c
#define BL_L_1_32_LOCKED		0x34
#define BL_L_1_16_LOCKED		0x3c
#define BL_L_1_8_LOCKED		0x44
#define BL_L_1_4_LOCKED		0x4c
#define BL_L_1_2_LOCKED		0x54
#define BL_ALL_UNLOCKED		0X00

/* die select */
#define DIE_SELECT_MASK		0x40
#define DIE_SELECT_DS0		0x00
#define DIE_SELECT_DS1		0x40

#define SPI_NAND_MT29F_ECC_MASK		0x70
#define SPI_NAND_MT29F_ECC_0_BIT		0x00
#define SPI_NAND_MT29F_ECC_1_3_BIT	0x10
#define SPI_NAND_MT29F_ECC_4_6_BIT	0x30
#define SPI_NAND_MT29F_ECC_7_8_BIT	0x50
#define SPI_NAND_MT29F_ECC_UNCORR	0x20

#define STATUS_ECC_MASK		GENMASK(5, 4)
#define STATUS_ECC_NO_BITFLIPS	(0 << 4)
#define STATUS_ECC_HAS_BITFLIPS	(1 << 4)
#define STATUS_ECC_UNCOR_ERROR	(2 << 4)

struct micron_onfi_specific {
	u8		two_plane_page_read;		/*166*/
	u8		reserved0[8];			/*167-174*/
	u8		otp_mode;				/*175*/
	u8		otp_page_start;		/*176*/
	u8		otp_data_protect_addr;	/*177*/
	u8		otp_page_number;			/*178*/
	u8		otp_feature_addr;			/*179*/
	u8		reserved1[68];			/*180-247*/
	u8		ecc_ability;			/*248*/
	u8		die_selection;			/*249*/
	u8		reserved2[3];			/*250-252*/
	u8		parameter_page_version;	/*253*/
}__attribute__((packed));

struct spi_nand_cmd_cfg {
	u8		opcode;
	u8		addr_bytes;
	u8		addr_bits;
	u8		dummy_bytes;
	u8		data_bits;
};

union spi_nand_vendor_specific {
	u8		vendor_specific[88];
	struct micron_onfi_specific micron_sepcific;
};

struct spi_nand_onfi_params {
	/* rev info and features block */
	/* 'O' 'N' 'F' 'I'  */
	u8		sig[4];				/*0-3*/
	__le16		revision;			/*4-5*/
	__le16		features;			/*6-7*/
	__le16		opt_cmd;			/*8-9*/
	u8		reserved0[22];			/*10-31*/

	/* manufacturer information block */
	char		manufacturer[12];		/*32-43*/
	char		model[20];			/*44-63*/
	u8		mfr_id;				/*64*/
	__le16		date_code;			/*65-66*/
	u8		reserved1[13];			/*67-79*/

	/* memory organization block */
	__le32		byte_per_page;			/*80-83*/
	__le16		spare_bytes_per_page;		/*84*85*/
	__le32		data_bytes_per_ppage;		/*86-89*/
	__le16		spare_bytes_per_ppage;		/*90-91*/
	__le32		pages_per_block;		/*92-95*/
	__le32		blocks_per_lun;			/*96-99*/
	u8		lun_count;			/*100*/
	u8		addr_cycles;			/*101*/
	u8		bits_per_cell;			/*102*/
	__le16		bb_per_lun;			/*103-104*/
	__le16		block_endurance;		/*105-106*/
	u8		guaranteed_good_blocks;		/*107*/
	__le16		guaranteed_block_endurance;	/*108-109*/
	u8		programs_per_page;		/*110*/
	u8		ppage_attr;			/*111*/
	u8		ecc_bits;			/*112*/
	u8		interleaved_bits;		/*113*/
	u8		interleaved_ops;		/*114*/
	u8		reserved2[13];			/*115-127*/

	/* electrical parameter block */
	u8		io_pin_capacitance_max;		/*128*/
	__le16		timing_mode;			/*129-130*/
	__le16		program_cache_timing_mode;	/*131-132*/
	__le16		t_prog;				/*133-134*/
	__le16		t_bers;				/*135-136*/
	__le16		t_r;				/*137-138*/
	__le16		t_ccs;				/*139-140*/
	u8		reserved3[23];			/*141-163*/

	/* vendor */
	__le16		vendor_specific_revision;	/*164-165*/
	union spi_nand_vendor_specific vendor;		/*166-253*/

	__le16		crc;				/*254-255*/
} __attribute__((packed));

#define ONFI_CRC_BASE	0x4F4E

/*
 * @ecc_strength:	[INTERN] ECC correctability
 * @ecclayout:		[BOARDSPECIFIC] ECC layout control structure
 * @get_ecc_status:	The function to get ecc status
 * */

struct spi_nand_chip;

struct nand_ecc_info {
	u32	ecc_strength;
	const struct nand_ecclayout	*ecc_layout;
	int(*get_ecc_status)(struct spi_nand_chip *chip,
			     unsigned int status);
};

/**
 * struct spi_nand_chip - SPI-NAND Private Flash Chip Data
 * @name:		name of the chip
 * @spi:		[INTERN] point to spi device structure
 * @mfr_id:		[BOARDSPECIFIC] manufacture id
 * @dev_id:		[BOARDSPECIFIC] device id
 * @read_cache_op:	[REPLACEABLE] Opcode of read from cache
 * @write_cache_op:	[REPLACEABLE] Opcode of program load
 * @write_cache_rdm_op:	[REPLACEABLE] Opcode of program load random
 * @oobbuf:		[INTERN] buffer for read/write oob
 * @size:		[INTERN] the size of chip
 * @block_size:		[INTERN] the size of eraseblock
 * @page_size:		[INTERN] the size of page
 * @oob_size:		[INTERN] the size of page oob size
 * @block_shift:	[INTERN] number of address bits in a eraseblock
 * @page_shift:		[INTERN] number of address bits in a page (column
 *			address bits).
 * @pagemask:		[INTERN] page number mask = number of (pages / chip) - 1
 * @options:		[BOARDSPECIFIC] various chip options. They can partly
 *			be set to inform nand_scan about special functionality.
 * @refresh_threshold:	[INTERN] Bitflip threshold to return -EUCLEAN
 *			See the defines for further explanation.
 * @ecc_info		struct nand_ecc_info
 * @onfi_params:	[INTERN] holds the ONFI page parameter
 */
struct spi_nand_chip {
	char		*name;
	struct udevice *dev;
	struct spi_slave	*spi;

	u8		mfr_id;
	u8		dev_id;
	u8		read_cache_op;
	u8		write_cache_op;
	u8		write_cache_rdm_op;

	u8		*oobbuf;
	u64		size;
	u32		block_size;
	u16		page_size;
	u16		oob_size;
	u8		lun_shift;
	u8		block_shift;
	u8		page_shift;
	u16		page_mask;
	u32		options;
	u8		refresh_threshold;
	u8		lun;
	struct nand_ecc_info ecc_info;
	struct spi_nand_onfi_params	 onfi_params;
	struct spi_nand_cmd_cfg *cfg_table;
};

struct spi_nand_flash {
	char		*name;
	u8		mfr_id;
	u8		dev_id;
	u32		page_size;
	u32		oob_size;
	u32		pages_per_blk;
	u32		blks_per_lun;
	u32		luns_per_chip;
	u32		options;
	struct nand_ecc_info	ecc_info;
};

#define SPINAND_MAX_ADDR_LEN		4
#define MIN_TRY_COUNT		3

struct spi_nand_cmd {
	u8		cmd;
	u8		n_addr;		/* Number of address */
	u8		addr[SPINAND_MAX_ADDR_LEN];	/* Reg Offset */
	u32		n_tx;		/* Number of tx bytes */
	const u8	*tx_buf;	/* Tx buf */
	u32		n_rx;		/* Number of rx bytes */
	u8		*rx_buf;	/* Rx buf */
};

enum {
	NORMAL_MODE,
	OTP_MODE,
};

#define SPI_NAND_ECCINFO(__ecc_strength, __ecc_layout,		\
			__get_ecc_status)			\
	.ecc_info = {						\
		.ecc_strength		= (__ecc_strength),	\
		.ecc_layout		= (__ecc_layout),	\
		.get_ecc_status 	= (__get_ecc_status),	\
	}

#define SPI_NAND_INFO(nm, mid, did, pagesz, oobsz, pg_per_blk,\
	blk_per_lun, lun_per_chip, opts, ...)		\
	{ .name = (nm), .mfr_id = (mid), .dev_id = (did),\
	.page_size = (pagesz), .oob_size = (oobsz),\
	.pages_per_blk = (pg_per_blk), .blks_per_lun = (blk_per_lun),\
	.luns_per_chip = (lun_per_chip),			\
	.options = (opts), __VA_ARGS__}

/*SPI NAND chip options*/
#define SPINAND_NEED_PLANE_SELECT	(1 << 0)
#define SPINAND_NEED_DIE_SELECT		(1 << 1)
#define SPINAND_NO_ECC 			(1 << 31)

#define SPINAND_MFR_MICRON		0x2C
#define SPINAND_MAX_ID_LEN		2

#ifdef DEBUG
#define spi_nand_debug(fmt,args...)	printf(fmt ,##args)
#else
#define spi_nand_debug(fmt,args...)
#endif
#define spi_nand_error(fmt,args...)	pr_err(fmt ,##args)
#define spi_nand_info(fmt,args...)	pr_info(fmt ,##args)

int spi_nand_bind_cfg_table(struct spi_nand_chip *chip);
bool spi_nand_cmd_supported(struct spi_nand_chip *chip, u8 opcode);
int spi_nand_block_markbad(struct spi_nand_chip *chip, loff_t offs);
int spi_nand_block_isbad(struct spi_nand_chip *chip, loff_t offs);
int spi_nand_probe_slave(struct spi_slave *spi, struct spi_nand_chip **chip);
int spi_nand_cmd_read_ops(struct spi_nand_chip *chip, u32 offset,
		size_t len, void *data);
int spi_nand_cmd_write_ops(struct spi_nand_chip *chip, u32 offset,
		size_t len, const void *buf);
int spi_nand_cmd_erase_ops(struct spi_nand_chip *chip, u32 offset, size_t len, int spread);
struct spi_nand_chip *spi_nand_flash_probe(unsigned int busnum, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode);
int spi_nand_cmd_reset_ops(struct spi_nand_chip *chip);
void spi_nand_flash_free(struct spi_nand_chip *chip);
void spi_nand_release(struct spi_nand_chip *chip);
int spi_nand_probe_bus_cs(unsigned int busnum, unsigned int cs,
			   unsigned int max_hz, unsigned int spi_mode,
			   struct udevice **devp);
int spi_nand_cmd_read_pages(struct spi_nand_chip *chip, u32 from, u_char *buf, size_t len);
int spi_nand_read_reg(struct spi_nand_chip *chip,
			uint8_t reg, uint8_t *buf);
int spi_nand_write_reg(struct spi_nand_chip *chip,
			uint8_t reg, uint8_t *buf);
int spi_nand_cmd_soft_ecc(bool enable);
int spi_nand_cmd_write_oob(struct spi_nand_chip *chip, u32 to, u_char *buf, size_t len);
#endif
