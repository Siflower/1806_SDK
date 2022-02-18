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

#include <types.h>
#include <stddef.h> /* for size_t */
#include <errorno.h>
#include "types.h"
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
#define CFG_OTP_PROTECT	0xc0
#define CFG_SNOR_ENABLE	0x82

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
 * @oob_size:	[INTERN] the size of page oob size
 * @block_shift:	[INTERN] number of address bits in a eraseblock
 * @page_shift:		[INTERN] number of address bits in a page (column
 *			address bits).
 * @pagemask:		[INTERN] page number mask = number of (pages / chip) - 1
 * @options:		[BOARDSPECIFIC] various chip options. They can partly
 *			be set to inform nand_scan about special functionality.
 * @ecc_strength:	[INTERN] ECC correctability
 * @refresh_threshold:	[INTERN] Bitflip threshold to return -EUCLEAN
 * @ecclayout:		[BOARDSPECIFIC] ECC layout control structure
 *			See the defines for further explanation.
 * @onfi_params:	[INTERN] holds the ONFI page parameter
 */
struct spi_nand_chip {
	char		*name;
	struct spi_slave	*spi;

	u8		mfr_id;
	u8		dev_id;
	u8		read_cache_op;
	u8		write_cache_op;
	u8		write_cache_rdm_op;

	u8		*oobbuf;
	u32		size;
	u32		block_size;
	u16		page_size;
	u16		oob_size;
	u8		lun_shift;
	u8		block_shift;
	u8		page_shift;
	u16		page_mask;
	u32		options;
	u32		ecc_strength;
	u8		refresh_threshold;
	u8		lun;
	struct nand_ecclayout *ecclayout;
	// struct spi_nand_onfi_params	 onfi_params;
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
	u32		ecc_strength;
	u32		options;
};

#define SPINAND_MAX_ADDR_LEN		4

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
	OTP_PROTECT_MODE,
	SNOR_READ_ENABLE_MODE,
};

#define SPI_NAND_INFO(nm, mid, did, pagesz, oobsz, pg_per_blk,\
	blk_per_lun, lun_per_chip, ecc_stren, opts)		\
	{ .name = (nm), .mfr_id = (mid), .dev_id = (did),\
	.page_size = (pagesz), .oob_size = (oobsz),\
	.pages_per_blk = (pg_per_blk), .blks_per_lun = (blk_per_lun),\
	.luns_per_chip = (lun_per_chip),			\
	.ecc_strength = (ecc_stren), .options = (opts) }

/*SPI NAND chip options*/
#define SPINAND_NEED_PLANE_SELECT	(1 << 0)
#define SPINAND_NEED_DIE_SELECT	(1 << 1)

#define SPINAND_MFR_MICRON		0x2C
#define SPINAND_MAX_ID_LEN		2

#ifdef DEBUG
#define spi_nand_debug(fmt,args...)	printf(fmt ,##args)
#else
#define spi_nand_debug(fmt,args...)
#endif
#define spi_nand_error(fmt,args...)	printf(fmt ,##args)
#define spi_nand_info(fmt,args...)	printf(fmt ,##args)

/*free oob descrption*/
struct nand_oobfree {
	u32 offset;
	u32 length;
};

/*ECC layout description*/
struct nand_ecclayout {
	u32 eccbytes;
	u32 eccpos[680];
	u32 oobavail;
	struct nand_oobfree oobfree[32];
};

/*oob operation mode*/
enum {
	MTD_OPS_PLACE_OOB = 0,
	MTD_OPS_AUTO_OOB = 1,
	MTD_OPS_RAW = 2,
};

/**
 * struct mtd_oob_ops - oob operation operands
 * @mode:	operation mode
 * @len:	number of data bytes to write/read
 * @retlen:	number of data bytes written/read
 * @ooblen:	number of oob bytes to write/read
 * @oobretlen:	number of oob bytes written/read
 * @ooboffs:	offset of oob data in the oob area (only relevant when
 *		mode = MTD_OPS_PLACE_OOB or MTD_OPS_RAW)
 * @datbuf:	data buffer - if NULL only oob data are read/written
 * @oobbuf:	oob data buffer
 *
 * Note, it is allowed to read more than one OOB area at one go, but not write.
 * The interface assumes that the OOB write requests program only one page's
 * OOB area.
 */
struct mtd_oob_ops {
	unsigned int	mode;
	size_t		len;
	size_t		retlen;
	size_t		ooblen;
	size_t		oobretlen;
	uint32_t	ooboffs;
	uint8_t		*datbuf;
	uint8_t		*oobbuf;
};

#ifdef SPI_NAND
int spi_nand_write(struct spi_nand_chip *chip, loff_t to, size_t len,
	size_t *retlen, const u_char *buf);
int spi_nand_write_oob(struct spi_nand_chip *chip, loff_t to,
			  struct mtd_oob_ops *ops);
int spi_nand_read(struct spi_nand_chip *chip, loff_t from, size_t len,
	size_t *retlen, u_char *buf);
int spi_nand_read_oob(struct spi_nand_chip *chip, loff_t from,
			struct mtd_oob_ops *ops);
int spi_nand_erase(struct spi_nand_chip *chip, u64 addr, u64 len);
int spi_nand_init(struct spi_nand_chip *chip);
int spi_nand_internal_data_move(struct spi_nand_chip *chip, u32 page_src,
		u32 page_dst, u32 offset, const u_char *buf, size_t len);
int spi_nand_lock_tight(struct spi_nand_chip *chip);
int spi_nand_block_markbad(struct spi_nand_chip *chip, loff_t offs);
int spi_nand_block_isbad(struct spi_nand_chip *chip, loff_t offs);
int spi_nand_lock_block(struct spi_nand_chip *chip, u8 lock);
void spi_nand_release(struct spi_nand_chip *chip);
#endif /* SPI_NAND */

int SPI_NAND_read(u32 offset, u8 *data, u8 chnum, u32 len);
int SPI_NAND_write(u8 chnum, u32 page_size, u32 offset, const u8 *buf, u32 len);
int SPI_NAND_Erase(u32 offset, u32 len);
void SPI_NAND_set_soft_ecc(int enable);
int inline SPI_NAND_get_soft_ecc(void);

#endif
