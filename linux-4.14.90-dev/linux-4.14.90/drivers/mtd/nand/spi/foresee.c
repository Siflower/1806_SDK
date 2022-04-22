/*
	 * Copyright (c) 2021 NO
	 * Date: 2021.09.26
	 * Note: reference to maximum driver, see also macronix.c
	 * Author: Yuanhui He <list_view_321@163.com>
*/


#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/mtd/spinand.h>


#define MFR_ID_FORESEE 0xCD
#define FORESEE_STATUS_ECC_MASK		GENMASK(5, 4)
#define FORESEE_STATUS_ECC_NO_BITFLIPS (0<<4)
#define FORESEE_STATUS_ECC_1B_BITFLIPS (1<<4)
#define FORESEE_STATUS_ECC_1B_MORE_BITFLIPS (2<<4)


static SPINAND_OP_VARIANTS(read_cache_variants,
		/* addr ndummy buf len */
				SPINAND_PAGE_READ_FROM_CACHE_X4_OP(0, 1, NULL, 0),
				SPINAND_PAGE_READ_FROM_CACHE_X2_OP(0, 1, NULL, 0),
				SPINAND_PAGE_READ_FROM_CACHE_OP(true, 0, 1, NULL, 0),
				SPINAND_PAGE_READ_FROM_CACHE_OP(false, 0, 1, NULL, 0));

static SPINAND_OP_VARIANTS(write_cache_variants, /* ops nopps */
				SPINAND_PROG_LOAD_X4(true, 0, NULL, 0), /* reset addr buf len */
				SPINAND_PROG_LOAD(true, 0, NULL, 0));

static SPINAND_OP_VARIANTS(update_cache_variants,
				SPINAND_PROG_LOAD_X4(false, 0, NULL, 0),
				SPINAND_PROG_LOAD(false, 0, NULL, 0));

//Todo
static int f35sqa001g_ooblayout_ecc(struct mtd_info *mtd, int section,
								      struct mtd_oob_region *region)
{
	   return -ERANGE; /* Todo: ecc obblayout is needed */
}

//Todo
static int f35sqa001g_ooblayout_free(struct mtd_info *mtd, int section,
								       struct mtd_oob_region *region)
{
		if (section > 3)
				return -ERANGE; /* four ECC section in one page only, return ERANGE if exceeds */

			return -ERANGE; /* Todo: free obblayout is needed */

}

static const struct mtd_ooblayout_ops f35sqa001g_ooblayout = {
	   .ecc = f35sqa001g_ooblayout_ecc,
	   .free = f35sqa001g_ooblayout_free,
};

static int f35sqa001g_ecc_get_status(struct spinand_device *spinand,
								       u8 status)
{
	   struct nand_device *nand = spinand_to_nand(spinand);
	   u8 eccsr;

	   switch (status & FORESEE_STATUS_ECC_MASK) {
	   case STATUS_ECC_NO_BITFLIPS:
			   return 0;
		
	   case STATUS_ECC_UNCOR_ERROR:
		 /* One more ECC bitflips */
			   return -EBADMSG;			 

		case FORESEE_STATUS_ECC_1B_BITFLIPS:
		 /* One ECC bitflips */
			   return 1;			 

		default:
			    break;

	 	}

		return -EINVAL;
}

static const struct spinand_info foresee_spinand_table[] = {
	    SPINAND_INFO("F35SQA001G", 0x71,
	    			 NAND_MEMORG(1, 2048, 64, 64, 1024, 1, 1, 1),
		 		     NAND_ECCREQ(4, 512), /* 4x16Bytes to 4x512Bytes in one page with 2K size*/
		             SPINAND_INFO_OP_VARIANTS(&read_cache_variants,
				   				      	      &write_cache_variants,
					                          &update_cache_variants),
				     SPINAND_HAS_QE_BIT,
		             SPINAND_ECCINFO(&f35sqa001g_ooblayout,
								     f35sqa001g_ecc_get_status)),
};

static int foresee_spinand_detect(struct spinand_device *spinand)
{
	   u8 *id = spinand->id.data;
	   int ret;			 
		/*
		 * FORESEE SPI NAND read ID needs a dummy byte, so the first byte in
		 * raw_id is garbage, drop it.
	    */

		if (id[1] != MFR_ID_FORESEE)
			return 0;
		
		/*
		 * Device ID is the one next to mfrid in raw_id
		 */

		 ret = spinand_match_and_init(spinand, foresee_spinand_table,
									  ARRAY_SIZE(foresee_spinand_table),
									  id[2]);

		 if (ret)
		 		return ret;

		 return 1;

}

static const struct spinand_manufacturer_ops foresee_spinand_manuf_ops = {
	   .detect = foresee_spinand_detect,
};

const struct spinand_manufacturer foresee_spinand_manufacturer = {
		.id = MFR_ID_FORESEE,
		.name = "Foresee",
		.ops = &foresee_spinand_manuf_ops,
};
