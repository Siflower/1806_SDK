#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/mtd/spinand.h>

#define SPINAND_MFR_FUDANMICRO		0xA1

#define FUDANMICRO_STATUS_ECC_MASK		GENMASK(5, 4)
#define FUDANMICRO_CFG_BUF_READ		BIT(3)

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

static int fm25s01a_ecc_get_status(struct spinand_device *spinand,
					 u8 status)
{
	switch (status & FUDANMICRO_STATUS_ECC_MASK) {
	case STATUS_ECC_NO_BITFLIPS:
		return 0;

	case STATUS_ECC_HAS_BITFLIPS:
		printk(KERN_ERR "##########1111######status :%#x\n", status);
		return 1;

	case STATUS_ECC_UNCOR_ERROR:
		printk(KERN_ERR "#########2222#######status :%#x\n", status);
		return -EBADMSG;

	default:
		break;
	}

	printk(KERN_ERR "########3333########status :%#x\n", status);
	return -EINVAL;
}
static int fm25s01a_ooblayout_ecc(struct mtd_info *mtd, int section,
				  struct mtd_oob_region *region)
{
	if (section > 3)
		return -ERANGE;

	region->offset = (16 * section) + 8;
	region->length = 8;

	return 0;
}

static int fm25s01a_ooblayout_free(struct mtd_info *mtd, int section,
				   struct mtd_oob_region *region)
{
	if (section > 3)
		return -ERANGE;

	region->offset = (16 * section) + 2;
	region->length = 2;

	return 0;
}

static const struct mtd_ooblayout_ops fm25s01a_ooblayout = {
	.ecc = fm25s01a_ooblayout_ecc,
	.free = fm25s01a_ooblayout_free,
};


static const struct spinand_info fudanmicro_spinand_table[] = {

	SPINAND_INFO("FM25S01A", 0xE4,
		     NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		     NAND_ECCREQ(1, 512),
		     SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
					      &write_cache_variants,
					      &update_cache_variants),
		     0,
		     SPINAND_ECCINFO(&fm25s01a_ooblayout, fm25s01a_ecc_get_status)),
		     //SPINAND_ECCINFO(0, fm25s01a_ecc_get_status)),
};

/**
 * fudanmicro_spinand_detect - initialize device related part in spinand_device
 * struct if it is a fudanmicro device.
 * @spinand: SPI NAND device structure
 */
static int fudanmicro_spinand_detect(struct spinand_device *spinand)
{
	u8 *id = spinand->id.data;
	int ret;

	/*
	 * Fudanmicro SPI NAND read ID need a dummy byte,
	 * so the first byte in raw_id is dummy.
	 */
	if (id[1] != SPINAND_MFR_FUDANMICRO)
		return 0;

	ret = spinand_match_and_init(spinand, fudanmicro_spinand_table,
				     ARRAY_SIZE(fudanmicro_spinand_table), id[2]);
	if (ret)
		return ret;

	return 1;
}

static int fudanmicro_spinand_init(struct spinand_device *spinand)
{
	struct nand_device *nand = spinand_to_nand(spinand);
	unsigned int i;

	/*
	 * Make sure all dies are in buffer read mode and not continuous read
	 * mode.
	 */
	for (i = 0; i < nand->memorg.ntargets; i++) {
		spinand_select_target(spinand, i);
		spinand_upd_cfg(spinand, FUDANMICRO_CFG_BUF_READ,
				FUDANMICRO_CFG_BUF_READ);
	}

	return 0;
}

static const struct spinand_manufacturer_ops fudanmicro_spinand_manuf_ops = {
	.detect = fudanmicro_spinand_detect,
	.init = fudanmicro_spinand_init,
};

const struct spinand_manufacturer fudanmicro_spinand_manufacturer = {
	.id = SPINAND_MFR_FUDANMICRO,
	.name = "Fudanmicro",
	.ops = &fudanmicro_spinand_manuf_ops,
};
