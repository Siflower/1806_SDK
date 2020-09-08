/*
 * sfax8 definitions
 *
 * Copyright (C) 2013 ams
 * Copyright (c) 2013, NVIDIA Corporation. All rights reserved.
 *
 * Author: Florian Lobmaier <florian.lobmaier@ams.com>
 * Author: Laxman Dewangan <ldewangan@nvidia.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include <linux/regmap.h>
#include <linux/mfd/sfax8_rn5t567.h>
#include <linux/mfd/sfax8_ip6103.h>
#include <linux/mfd/core.h>

/*
* We will get a value from the special reg in different pmu, and we should make
* sure that the reading operation from this reg don't change reg value.
*
*/
#define PMU_REG	0x01

struct sfax8_pmu_info{
	unsigned addr;
	char name[20];
	const struct mfd_cell *cell;
	unsigned int cell_len;
	const struct regmap_config *config;
};


typedef enum pmu_type_t{
	PMU_IP6103 = 0,
	PMU_RN5T567,
}pmu_type;

struct sfax8 {
	struct device *dev;
	struct regmap *regmap;
	pmu_type type;
	int chip_irq;
	unsigned long irq_flags;
	bool en_intern_int_pullup;
	bool en_intern_i2c_pullup;
	struct regmap_irq_chip_data *irq_data;
};

static inline int sfax8_read(struct sfax8 *sfax8, u32 reg, u32 *dest)
{
	return regmap_read(sfax8->regmap, reg, dest);
}

static inline int sfax8_write(struct sfax8 *sfax8, u32 reg, u32 value)
{
	return regmap_write(sfax8->regmap, reg, value);
}

static inline int sfax8_block_read(struct sfax8 *sfax8, u32 reg,
		int count, u8 *buf)
{
	return regmap_bulk_read(sfax8->regmap, reg, buf, count);
}

static inline int sfax8_block_write(struct sfax8 *sfax8, u32 reg,
		int count, u8 *data)
{
	return regmap_bulk_write(sfax8->regmap, reg, data, count);
}

static inline int sfax8_update_bits(struct sfax8 *sfax8, u32 reg,
		u32 mask, u8 val)
{
	return regmap_update_bits(sfax8->regmap, reg, mask, val);
}
/*
static inline int sfax8_irq_get_virq(struct sfax8 *sfax8, int irq)
{
	return regmap_irq_get_virq(sfax8->irq_data, irq);
}
*/
