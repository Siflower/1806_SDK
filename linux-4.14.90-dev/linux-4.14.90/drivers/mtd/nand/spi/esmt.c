// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2018 Shanghai Siflower Communication Technology Co., Ltd.
 *
 * Authors:
 *	Qi Zhang <qi.zhang@siflower.com.cn>
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_ESMT		0xc8

#define ESMT_STATUS_ECC_MASK		GENMASK(5, 4)

static struct nand_ecclayout nand_oob_64 = {
	.eccbytes = 24,
	.eccpos = {
		   8, 9, 10, 11, 12, 13, 14, 15,
		   24, 25, 26, 27, 28, 29, 30, 31,
		   40, 41, 42, 43, 44, 45, 46, 47,
		   56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 38,
	.oobfree = {
		{.offset = 2,
		 .length = 2},
		{.offset = 18,
		 .length = 2},
		{.offset = 34,
		 .length = 2},
		{.offset = 50,
		 .length = 2}, }
};

static SPINAND_OP_VARIANTS(read_cache_variants,
		SPINAND_PAGE_READ_FROM_CACHE_QUADIO_OP(0, 2, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_DUALIO_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
		SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants,
		SPINAND_PROG_LOAD_X4(true, 0, NULL, 0),
		SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
		SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
		SPINAND_PROG_LOAD(false, 0, NULL, 0));

static int mt29f2g01abagd_ecc_get_status(struct spinand_device *spinand,
					 u8 status)
{
	switch (status & ESMT_STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case STATUS_ECC_HAS_BITFLIPS:
		return 1;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info esmt_spinand_table[] = {
	SPINAND_INFO("F50L1G41LB", 0x01,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(1, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(0,
				     mt29f2g01abagd_ecc_get_status)),
};

static int esmt_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	/*
	 * ESMT SPI NAND read ID need a dummy byte,
	 * so the first byte in raw_id is dummy.
	 */
	if (id[1] != SPINAND_MFR_ESMT)
		return 0;

	ret = spinand_match_and_init(spinand, esmt_spinand_table,
				     ARRAY_SIZE(esmt_spinand_table), id[2]);
	if (ret)
		return ret;

	return 1;
}

static int esmt_spinand_init(struct spinand_device *spinand)
{
	struct mtd_info *mtd = spinand_to_mtd(spinand);
	int i;

	mtd->ecclayout = &nand_oob_64;

	/*
	 * The number of bytes available for a client to place data into
	 * the out of band area.
	 */
	mtd->ecclayout->oobavail = 0;
	for (i = 0; mtd->ecclayout->oobfree[i].length
			&& i < ARRAY_SIZE(mtd->ecclayout->oobfree); i++)
		mtd->ecclayout->oobavail += mtd->ecclayout->oobfree[i].length;
	mtd->oobavail = mtd->ecclayout->oobavail;

	return 0;
}

static const struct spinand_manufacturer_ops esmt_spinand_manuf_ops = {
	.detect = esmt_spinand_detect,
	.init = esmt_spinand_init,
};

const struct spinand_manufacturer esmt_spinand_manufacturer = {
	.id = SPINAND_MFR_ESMT,
	.name = "ESMT",
	.ops = &esmt_spinand_manuf_ops,
};
