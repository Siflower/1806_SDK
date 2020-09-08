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

/* SFAX8_IP6103 registers */
#include <linux/regmap.h>
#include <linux/mfd/core.h>
#define SFAX8_IP6103_WAKE0_REG				0x00
#define SFAX8_IP6103_WAKE1_REG				0x01
#define SFAX8_IP6103_SLEEP_REG				0x02
#define SFAX8_IP6103_PROTECT_REG				0x03
#define SFAX8_IP6103_STATUE0_REG				0x04
#define SFAX8_IP6103_STATUE1_REG				0x05
#define SFAX8_IP6103_STATUE2_REG				0x06
#define SFAX8_IP6103_PWR0_REG				0x07
#define SFAX8_IP6103_PWR1_REG				0x08
#define SFAX8_IP6103_PWR2_REG				0x11
#define SFAX8_IP6103_IRC_CONF0_REG			0x13
#define SFAX8_IP6103_IRC_CONF1_REG			0x14
#define SFAX8_IP6103_IRC_ICCC_REG			0x15
#define SFAX8_IP6103_IRC_IWKDC_REG			0x16
#define SFAX8_IP6103_IRC_IKDC0_REG			0x17
#define SFAX8_IP6103_IRC_IKDC1_REG			0x18
#define SFAX8_IP6103_IRC_IRCC0_REG			0x19
#define SFAX8_IP6103_IRC_IRCC1_REG			0x1a
#define SFAX8_IP6103_PWR_OFF_FLAG_REG		0x1b
#define SFAX8_IP6103_DCDC_CTL_REG			0x20
#define SFAX8_IP6103_DCDC0_CTL0_REG			0x21
#define SFAX8_IP6103_DCDC0_CTL1_REG			0x22
#define SFAX8_IP6103_DCDC1_CTL0_REG				0x28
#define SFAX8_IP6103_DCDC1_CTL1_REG				0x29
#define SFAX8_IP6103_DCDC2_CTL0_REG				0x2F
#define SFAX8_IP6103_DCDC2_CTL1_REG				0x30
#define SFAX8_IP6103_DCDC3_CTL0_REG				0x36
#define SFAX8_IP6103_DCDC3_CTL1_REG				0x37
#define SFAX8_IP6103_SW_LDO_CTL0_REG				0x40
#define SFAX8_IP6103_SW_LDO_CTL1_REG				0x41
#define SFAX8_IP6103_LDO0_REG					0x42
#define SFAX8_IP6103_LDO1_REG					0x44
#define SFAX8_IP6103_LDO2_REG					0x46
#define SFAX8_IP6103_LDO4_REG					0x4A
#define SFAX8_IP6103_LDO5_REG					0x4C
#define SFAX8_IP6103_LDO6_REG					0x4E
#define SFAX8_IP6103_SVCC_REG					0x51
#define SFAX8_IP6103_INT0_REG					0x60
#define SFAX8_IP6103_INT1_REG					0x61
#define SFAX8_IP6103_INT2_REG					0x62
#define SFAX8_IP6103_INT3_REG					0x63
#define SFAX8_IP6103_RST_CTRL_REG	0x70
#define SFAX8_IP6103_ALARM_SEC_REG	0x71
#define SFAX8_IP6103_ALARM_MIN_REG	0x72
#define SFAX8_IP6103_ALARM_HOUR_REG	0x73
#define SFAX8_IP6103_ALARM_DAY_REG	0x74
#define SFAX8_IP6103_ALARM_MON_REG	0x75
#define SFAX8_IP6103_ALARM_YEAR_REG	0x76

#define SFAX8_IP6103_WATCH_SEC_REG	0x77
#define SFAX8_IP6103_WATCH_MIN_REG	0x78
#define SFAX8_IP6103_WATCH_HOUR_REG	0x79
#define SFAX8_IP6103_WATCH_DAY_REG	0x7A
#define SFAX8_IP6103_WATCH_MON_REG	0x7B
#define SFAX8_IP6103_WATCH_YEAR_REG	0x7C

#define SFAX8_IP6103_MFP_LDO65_REG				0x80
#define SFAX8_IP6103_MFP_LDO4_REG				0x81
#define SFAX8_IP6103_MFP_GPIO2_REG				0x82
#define SFAX8_IP6103_MFP_GPIO10_REG				0x83
#define SFAX8_IP6103_MFP_GPIO_PUPD_REG			0x84
#define SFAX8_IP6103_MFP_IRQ_REG					0x85
#define SFAX8_IP6103_MFP_RESET_EN_REG			0x86
#define SFAX8_IP6103_MFP_GPIO74_PUPD_REG			0x87
#define SFAX8_IP6103_MAX_REGISTER				0x90

#define SFAX8_IP6103_IRC_MODE_MASK				0x3
#define SFAX8_IP6103_VREG_MASK_2_0				0x07
#define SFAX8_IP6103_VREG_MASK_6_0				0x7f
#define SFAX8_IP6103_VREG_MASK_6_1				0x7e
#define SFAX8_IP6103_DCDC_VOLTAGES				0x7f
#define SFAX8_IP6103_DCDC3_VOLTAGES				0x68
#define SFAX8_IP6103_LDO_VOLTAGES				0x6c
#define SFAX8_IP6103_SVCC_VOLTAGES				0x7
#define SFAX8_IP6103_IRQ_RTC_ALARM               5

static const struct resource sfax8_ip6103_rtc_resource[] = {
	{
		.name = "sfax8-rtc-alarm",
		.start = SFAX8_IP6103_IRQ_RTC_ALARM,
		.end = SFAX8_IP6103_IRQ_RTC_ALARM,
		.flags = IORESOURCE_IRQ,
	},
};
static const struct mfd_cell sfax8_ip6103_devs[] = {
	{
		.name = "sfax8-regulator",
	},
	{
		.name = "power-management",
	},
	{
		.name = "sfax8-misc",
	},
	{
		.name = "reset-button"
	},
	{
		.name = "sfax8-rtc",
		.num_resources = ARRAY_SIZE(sfax8_ip6103_rtc_resource),
		.resources = sfax8_ip6103_rtc_resource,
	},
};
static const struct regmap_range sfax8_ip6103_readable_ranges[] = {
	regmap_reg_range(SFAX8_IP6103_WAKE0_REG, SFAX8_IP6103_PWR1_REG),
	regmap_reg_range(SFAX8_IP6103_PWR2_REG, SFAX8_IP6103_PWR2_REG),
	regmap_reg_range(SFAX8_IP6103_IRC_CONF0_REG, SFAX8_IP6103_PWR_OFF_FLAG_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC_CTL_REG, SFAX8_IP6103_DCDC0_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC1_CTL0_REG, SFAX8_IP6103_DCDC1_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC2_CTL0_REG, SFAX8_IP6103_DCDC2_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC3_CTL0_REG, SFAX8_IP6103_DCDC3_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_SW_LDO_CTL0_REG, SFAX8_IP6103_LDO0_REG),
	regmap_reg_range(SFAX8_IP6103_LDO1_REG, SFAX8_IP6103_LDO1_REG),
	regmap_reg_range(SFAX8_IP6103_LDO2_REG, SFAX8_IP6103_LDO2_REG),
	regmap_reg_range(SFAX8_IP6103_LDO4_REG, SFAX8_IP6103_LDO4_REG),
	regmap_reg_range(SFAX8_IP6103_LDO5_REG, SFAX8_IP6103_LDO5_REG),
	regmap_reg_range(SFAX8_IP6103_LDO6_REG, SFAX8_IP6103_LDO6_REG),
	regmap_reg_range(SFAX8_IP6103_SVCC_REG, SFAX8_IP6103_SVCC_REG),
	regmap_reg_range(SFAX8_IP6103_INT0_REG, SFAX8_IP6103_INT3_REG),
	regmap_reg_range(SFAX8_IP6103_MFP_LDO65_REG, SFAX8_IP6103_MFP_GPIO74_PUPD_REG),
	regmap_reg_range(SFAX8_IP6103_ALARM_SEC_REG, SFAX8_IP6103_WATCH_YEAR_REG),
};

static const struct regmap_access_table sfax8_ip6103_readable_table = {
	.yes_ranges = sfax8_ip6103_readable_ranges,
	.n_yes_ranges = ARRAY_SIZE(sfax8_ip6103_readable_ranges),
};

static const struct regmap_range sfax8_ip6103_writable_ranges[] = {
	regmap_reg_range(SFAX8_IP6103_WAKE0_REG, SFAX8_IP6103_STATUE0_REG),
	regmap_reg_range(SFAX8_IP6103_PWR0_REG, SFAX8_IP6103_PWR1_REG),
	regmap_reg_range(SFAX8_IP6103_PWR2_REG, SFAX8_IP6103_PWR2_REG),
	regmap_reg_range(SFAX8_IP6103_IRC_CONF0_REG, SFAX8_IP6103_IRC_IWKDC_REG),
	regmap_reg_range(SFAX8_IP6103_PWR_OFF_FLAG_REG, SFAX8_IP6103_PWR_OFF_FLAG_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC_CTL_REG, SFAX8_IP6103_DCDC0_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC1_CTL0_REG, SFAX8_IP6103_DCDC1_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC2_CTL0_REG, SFAX8_IP6103_DCDC2_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC3_CTL0_REG, SFAX8_IP6103_DCDC3_CTL1_REG),
	regmap_reg_range(SFAX8_IP6103_SW_LDO_CTL0_REG, SFAX8_IP6103_LDO0_REG),
	regmap_reg_range(SFAX8_IP6103_LDO1_REG, SFAX8_IP6103_LDO1_REG),
	regmap_reg_range(SFAX8_IP6103_LDO2_REG, SFAX8_IP6103_LDO2_REG),
	regmap_reg_range(SFAX8_IP6103_LDO4_REG, SFAX8_IP6103_LDO4_REG),
	regmap_reg_range(SFAX8_IP6103_LDO5_REG, SFAX8_IP6103_LDO5_REG),
	regmap_reg_range(SFAX8_IP6103_LDO6_REG, SFAX8_IP6103_LDO6_REG),
	regmap_reg_range(SFAX8_IP6103_SVCC_REG, SFAX8_IP6103_SVCC_REG),
	regmap_reg_range(SFAX8_IP6103_INT0_REG, SFAX8_IP6103_INT3_REG),
	regmap_reg_range(SFAX8_IP6103_MFP_LDO65_REG, SFAX8_IP6103_MFP_GPIO74_PUPD_REG),
	regmap_reg_range(SFAX8_IP6103_ALARM_SEC_REG, SFAX8_IP6103_WATCH_YEAR_REG),
};

static const struct regmap_access_table sfax8_ip6103_writable_table = {
	.yes_ranges = sfax8_ip6103_writable_ranges,
	.n_yes_ranges = ARRAY_SIZE(sfax8_ip6103_writable_ranges),
};

static const struct regmap_range sfax8_ip6103_cacheable_ranges[] = {
	regmap_reg_range(SFAX8_IP6103_WAKE1_REG, SFAX8_IP6103_SLEEP_REG),
	regmap_reg_range(SFAX8_IP6103_PWR0_REG, SFAX8_IP6103_PWR2_REG),
	regmap_reg_range(SFAX8_IP6103_DCDC_CTL_REG, SFAX8_IP6103_INT1_REG),
	regmap_reg_range(SFAX8_IP6103_MFP_LDO65_REG, SFAX8_IP6103_MFP_GPIO74_PUPD_REG),
};

static const struct regmap_access_table sfax8_ip6103_volatile_table = {
	.no_ranges = sfax8_ip6103_cacheable_ranges,
	.n_no_ranges = ARRAY_SIZE(sfax8_ip6103_cacheable_ranges),
};

static const struct regmap_config sfax8_ip6103_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = SFAX8_IP6103_MAX_REGISTER,
	.cache_type = REGCACHE_RBTREE,
	.rd_table = &sfax8_ip6103_readable_table,
	.wr_table = &sfax8_ip6103_writable_table,
	.volatile_table = &sfax8_ip6103_volatile_table,
};
