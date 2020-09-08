/*
 * sfax8_rn5t567 definitions
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


#include <linux/mfd/core.h>
#include <linux/regmap.h>
#define SFAX8_RN5T567_LSIVER    0x00
#define SFAX8_RN5T567_OTPVER    0x01
#define SFAX8_RN5T567_IODAC     0x02
#define SFAX8_RN5T567_VINDAC    0x03
#define SFAX8_RN5T567_OUT32KEN  0x05
#define SFAX8_RN5T567_CPUCNT    0x06
#define SFAX8_RN5T567_PSWR      0x07
#define SFAX8_RN5T567_PONHIS    0x09
#define SFAX8_RN5T567_POFFHIS   0x0A
#define SFAX8_RN5T567_WATCHDOG  0x0B
#define SFAX8_RN5T567_WATCHDOGCNT       0x0C
#define SFAX8_RN5T567_PWRFUNC   0x0D
#define SFAX8_RN5T567_SLPCNT    0x0E
#define SFAX8_RN5T567_REPCNT    0x0F
#define SFAX8_RN5T567_PWRONTIMSET       0x10
#define SFAX8_RN5T567_NOETIMSET 0x11
#define SFAX8_RN5T567_PWRIREN   0x12
#define SFAX8_RN5T567_PWRIRQ    0x13
#define SFAX8_RN5T567_PWRMON    0x14
#define SFAX8_RN5T567_PWRIRSEL  0x15

#define SFAX8_RN5T567_DC1_SLOT  0x16
#define SFAX8_RN5T567_DC2_SLOT  0x17
#define SFAX8_RN5T567_DC3_SLOT  0x18
#define SFAX8_RN5T567_DC4_SLOT  0x19

#define SFAX8_RN5T567_LDO1_SLOT  0x1B
#define SFAX8_RN5T567_LD02_SLOT  0x1C
#define SFAX8_RN5T567_LDO3_SLOT  0x1D
#define SFAX8_RN5T567_LDO4_SLOT  0x1E
#define SFAX8_RN5T567_LDO5_SLOT  0x1F

#define SFAX8_RN5T567_DC1CTL    0x2C
#define SFAX8_RN5T567_DC1CTL2   0x2D
#define SFAX8_RN5T567_DC2CTL    0x2E
#define SFAX8_RN5T567_DC2CTL2   0x2F
#define SFAX8_RN5T567_DC3CTL    0x30
#define SFAX8_RN5T567_DC3CTL2   0x31
#define SFAX8_RN5T567_DC4CTL    0x32
#define SFAX8_RN5T567_DC4CTL2   0x33
#define SFAX8_RN5T567_DC1DAC    0x36
#define SFAX8_RN5T567_DC2DAC    0x37
#define SFAX8_RN5T567_DC3DAC    0x38
#define SFAX8_RN5T567_DC4DAC    0x39
#define SFAX8_RN5T567_DC1DAC_SLP        0x3B
#define SFAX8_RN5T567_DC2DAC_SLP        0x3C
#define SFAX8_RN5T567_DC3DAC_SLP        0x3D
#define SFAX8_RN5T567_DC4DAC_SLP        0x3E
#define SFAX8_RN5T567_DCIREN    0x40
#define SFAX8_RN5T567_DCIRQ     0x41
#define SFAX8_RN5T567_DCIRMON   0x42
#define SFAX8_RN5T567_LDOEN1    0x44
#define SFAX8_RN5T567_LDOEN2    0x45
#define SFAX8_RN5T567_LDODIS    0x46
#define SFAX8_RN5T567_LDO1DAC   0x4C
#define SFAX8_RN5T567_LDO2DAC   0x4D
#define SFAX8_RN5T567_LDO3DAC   0x4E
#define SFAX8_RN5T567_LDO4DAC   0x4F
#define SFAX8_RN5T567_LDO5DAC   0x50
#define SFAX8_RN5T567_LDORTCDAC 0x56
#define SFAX8_RN5T567_LDORTC2DAC        0x57
#define SFAX8_RN5T567_LDO1DAC_SLP       0x58
#define SFAX8_RN5T567_LDO2DAC_SLP       0x59
#define SFAX8_RN5T567_LDO3DAC_SLP       0x5A
#define SFAX8_RN5T567_LDO4DAC_SLP       0x5B
#define SFAX8_RN5T567_LDO5DAC_SLP       0x5C
#define SFAX8_RN5T567_IOSEL     0x90
#define SFAX8_RN5T567_IOOUT     0x91
#define SFAX8_RN5T567_GPEDGE1   0x92
#define SFAX8_RN5T567_EN_GPIR   0x94
#define SFAX8_RN5T567_IR_GPR    0x95
#define SFAX8_RN5T567_IR_GPF    0x96
#define SFAX8_RN5T567_MON_IOIN  0x97
#define SFAX8_RN5T567_GPLED_FUNC        0x98
#define SFAX8_RN5T567_INTPOL    0x9C
#define SFAX8_RN5T567_INTEN     0x9D
#define SFAX8_RN5T567_INTMON    0x9E
#define SFAX8_RN5T567_PREVINDAC 0xB0
#define SFAX8_RN5T567_OVTEMP    0xBC
#define SFAX8_RN5T567_MAX_REGISTER		0xBC
#define SFAX8_RN5T567_IRC_MODE_MASK				0x3
#define SFAX8_RN5T567_VREG_MASK_2_0				0x07
#define SFAX8_RN5T567_VREG_MASK_6_1				0x7e
#define SFAX8_RN5T567_VREG_MASK_7_0				0xff
#define SFAX8_RN5T567_DCDC_VOLTAGES				0xe9
#define SFAX8_RN5T567_DCDC3_VOLTAGES				0x68
#define SFAX8_RN5T567_LDO_VOLTAGES				0x69
#define SFAX8_RN5T567_LDO3_VOLTAGES				0x75
#define SFAX8_RN5T567_SVCC_VOLTAGES				0x7
#define SFAX8_RN5T567_IRQ_RTC_ALARM               5
#define ONE_BIT_SHIFT(n)					(0x1 << n)
#define TWO_BITS_SHIFT(n)					(0x3 << n)

static const struct mfd_cell sfax8_rn5t567_devs[] = {
	{
		.name = "sfax8-regulator",
	},
	{
		.name = "power-management",
	},
	{
		.name = "reset-button"
	},
};
static const struct regmap_range sfax8_rn5t567_readable_ranges[] = {
	regmap_reg_range(SFAX8_RN5T567_LSIVER, SFAX8_RN5T567_LDO5_SLOT),
	regmap_reg_range(SFAX8_RN5T567_DC1CTL, SFAX8_RN5T567_DC4DAC),
	regmap_reg_range(SFAX8_RN5T567_DC1DAC_SLP, SFAX8_RN5T567_DC4DAC_SLP),
	regmap_reg_range(SFAX8_RN5T567_DCIREN, SFAX8_RN5T567_LDODIS),
	regmap_reg_range(SFAX8_RN5T567_LDO1DAC, SFAX8_RN5T567_LDO5DAC),
	regmap_reg_range(SFAX8_RN5T567_LDORTCDAC, SFAX8_RN5T567_LDO5DAC_SLP),
	regmap_reg_range(SFAX8_RN5T567_IOSEL, SFAX8_RN5T567_GPLED_FUNC),
	regmap_reg_range(SFAX8_RN5T567_INTPOL, SFAX8_RN5T567_INTMON),
	regmap_reg_range(SFAX8_RN5T567_PREVINDAC, SFAX8_RN5T567_OVTEMP),
};
static const struct regmap_access_table sfax8_rn5t567_readable_table = {
	.yes_ranges = sfax8_rn5t567_readable_ranges,
	.n_yes_ranges = ARRAY_SIZE(sfax8_rn5t567_readable_ranges),
};

static const struct regmap_range sfax8_rn5t567_writable_ranges[] = {
	regmap_reg_range(SFAX8_RN5T567_LSIVER, SFAX8_RN5T567_LDO5_SLOT),
	regmap_reg_range(SFAX8_RN5T567_DC1CTL, SFAX8_RN5T567_DC4DAC),
	regmap_reg_range(SFAX8_RN5T567_DC1DAC_SLP, SFAX8_RN5T567_DC4DAC_SLP),
	regmap_reg_range(SFAX8_RN5T567_DCIREN, SFAX8_RN5T567_LDODIS),
	regmap_reg_range(SFAX8_RN5T567_LDO1DAC, SFAX8_RN5T567_LDO5DAC),
	regmap_reg_range(SFAX8_RN5T567_LDORTCDAC, SFAX8_RN5T567_LDO5DAC_SLP),
	regmap_reg_range(SFAX8_RN5T567_IOSEL, SFAX8_RN5T567_GPLED_FUNC),
	regmap_reg_range(SFAX8_RN5T567_INTPOL, SFAX8_RN5T567_INTMON),
	regmap_reg_range(SFAX8_RN5T567_PREVINDAC, SFAX8_RN5T567_OVTEMP),
};
static const struct regmap_access_table sfax8_rn5t567_writable_table = {
	.yes_ranges = sfax8_rn5t567_writable_ranges,
	.n_yes_ranges = ARRAY_SIZE(sfax8_rn5t567_writable_ranges),
};

static const struct regmap_range sfax8_rn5t567_cacheable_ranges[] = {
	regmap_reg_range(SFAX8_RN5T567_LDO1DAC_SLP, SFAX8_RN5T567_LDO5DAC_SLP),
	regmap_reg_range(SFAX8_RN5T567_DC1DAC_SLP, SFAX8_RN5T567_DC4DAC_SLP),

};
static const struct regmap_access_table sfax8_rn5t567_volatile_table = {
	.no_ranges = sfax8_rn5t567_cacheable_ranges,
	.n_no_ranges = ARRAY_SIZE(sfax8_rn5t567_cacheable_ranges),
};

static const struct regmap_config sfax8_rn5t567_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = SFAX8_RN5T567_MAX_REGISTER,
	.cache_type = REGCACHE_RBTREE,
	.rd_table = &sfax8_rn5t567_readable_table,
	.wr_table = &sfax8_rn5t567_writable_table,
	.volatile_table = &sfax8_rn5t567_volatile_table,
};
