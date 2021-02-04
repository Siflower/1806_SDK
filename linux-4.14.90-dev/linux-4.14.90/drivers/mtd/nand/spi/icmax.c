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

#define SPINAND_MFR_ICMAX		0x9b

#define ICMAX_STATUS_ECC_MASK		GENMASK(7, 4)
#define ICMAX_STATUS_ECC_NO_BITFLIPS	(0 << 4)
#define ICMAX_STATUS_ECC_1TO3_BITFLIPS	(1 << 4)
#define ICMAX_STATUS_ECC_4TO6_BITFLIPS	(3 << 4)
#define ICMAX_STATUS_ECC_7TO8_BITFLIPS	(5 << 4)

static struct nand_ecclayout nand_oob_64 = {
	.eccbytes = 24,
	.eccpos = {
		   4, 5, 6, 7, 8, 9, 10, 11,
		   12, 13, 14, 15, 16, 17, 18, 19,
		   20, 21, 22, 23, 24, 25, 26, 27},
	.oobavail = 38,
	.oobfree = {
		{.offset = 2,
		 .length = 2},
		{.offset = 28,
		 .length = 36}, }
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
	switch (status & ICMAX_STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case STATUS_ECC_UNCOR_ERROR:
		return -EBADMSG;

	case ICMAX_STATUS_ECC_1TO3_BITFLIPS:
		return 3;

	case ICMAX_STATUS_ECC_4TO6_BITFLIPS:
		return 6;

	case ICMAX_STATUS_ECC_7TO8_BITFLIPS:
		return 8;

	default:
		break;
	}

	return -EINVAL;
}

static const struct spinand_info icmax_spinand_table[] = {
	SPINAND_INFO("IMS1G083ZAA1", 0x12,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(8, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(0,
				     mt29f2g01abagd_ecc_get_status)),
};

static int icmax_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	/*
	 * Icmax SPI NAND read ID need a dummy byte,
	 * so the first byte in raw_id is dummy.
	 */
	if (id[1] != SPINAND_MFR_ICMAX)
		return 0;

	ret = spinand_match_and_init(spinand, icmax_spinand_table,
				     ARRAY_SIZE(icmax_spinand_table), id[2]);
	if (ret)
		return ret;

	return 1;
}

static int icmax_spinand_init(struct spinand_device *spinand)
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

static const struct spinand_manufacturer_ops icmax_spinand_manuf_ops = {
	.detect = icmax_spinand_detect,
	.init = icmax_spinand_init,
};

const struct spinand_manufacturer icmax_spinand_manufacturer = {
	.id = SPINAND_MFR_ICMAX,
	.name = "Icmax",
	.ops = &icmax_spinand_manuf_ops,
};
