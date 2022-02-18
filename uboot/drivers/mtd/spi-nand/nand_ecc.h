/*
 *  Copyright (C) 2000-2010 Steven J. Hill <sjhill@realitydiluted.com>
 *			    David Woodhouse <dwmw2@infradead.org>
 *			    Thomas Gleixner <tglx@linutronix.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file is the header for the ECC algorithm.
 */

#ifndef __MTD_NAND_ECC_H__
#define __MTD_NAND_ECC_H__

#define ECC_SIZE 0x100
#define ECC_MASK 0xff
#define COLUMN_TO_ECC_OFFSET(column) ((column) / ECC_SIZE * 3 + 4)

/*
 * Calculate 3 byte ECC code for eccsize byte block
 */
void nand_calculate_ecc(const u_char *dat, unsigned int eccsize,
				u_char *ecc_code);

/*
 * Detect and correct a 1 bit error for eccsize byte block
 */
int nand_correct_data(u_char *dat, u_char *read_ecc, u_char *calc_ecc,
			unsigned int eccsize);

#endif /* __MTD_NAND_ECC_H__ */
