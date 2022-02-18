/*
 * Driver for Broadcom SFAX8 GPIO unit (pinctrl + GPIO)
 *
 * Copyright (C) 2012 Chris Boot, Simon Arlott, Stephen Warren
 *
 * This driver is inspired by:
 * pinctrl-nomadik.c, please see original file for copyright information
 * pinctrl-tegra.c, please see original file for copyright information
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
 */

#include <linux/bitmap.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/irqdesc.h>
#include <linux/irqdomain.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pinctrl/machine.h>
#include <linux/pinctrl/pinconf.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/pinmux.h>
#include <linux/pinctrl/pinconf-generic.h>
#include <linux/platform_device.h>
#include <linux/printk.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/regmap.h>
#include <linux/clk.h>
#include <linux/mfd/syscon.h>

#include "core.h"
#include "pinconf.h"
#include "pinctrl-sfax8.h"

#ifdef CLIB
#include <stdio.h>
#include <string.h>
#endif

#define MODULE_NAME "pinctrl-sfax8"
#define SFAX8_NUM_GPIOS (PAD_INDEX_MAX + 1)
#define SFAX8_NUM_BANKS 4
#define SFAX8_GPIOS_PER_BANK 16

#define SFAX8_PIN_BITMAP_SZ \
	DIV_ROUND_UP(SFAX8_NUM_GPIOS, sizeof(unsigned long) * 8)

/* GPIO register offsets */
#if 1
#define GPFSEL0		0x0	/* Function Select */
#define GPSET0		0x1c	/* Pin Output Set */
#define GPCLR0		0x28	/* Pin Output Clear */
#define GPLEV0		0x34	/* Pin Level */
#define GPEDS0		0x40	/* Pin Event Detect Status */
#define GPREN0		0x4c	/* Pin Rising Edge Detect Enable */
#define GPFEN0		0x58	/* Pin Falling Edge Detect Enable */
#define GPHEN0		0x64	/* Pin High Detect Enable */
#define GPLEN0		0x70	/* Pin Low Detect Enable */
#define GPAREN0		0x7c	/* Pin Async Rising Edge Detect */
#define GPAFEN0		0x88	/* Pin Async Falling Edge Detect */
#define GPPUD		0x94	/* Pin Pull-up/down Enable */
#define GPPUDCLK0	0x98	/* Pin Pull-up/down Enable Clock */
#endif
#define FSEL_REG(p)		(GPFSEL0 + (((p) / 10) * 4))
#define FSEL_SHIFT(p)		(((p) % 10) * 3)
#define GPIO_REG_OFFSET(p)	((p) / 32)
#define GPIO_REG_SHIFT(p)	((p) % 32)

enum sfax8_pinconf_param {
	/* argument: sfax8_pinconf_pull */
	SFAX8_PINCONF_PARAM_PULL,
};

enum sfax8_pinconf_pull {
	SFAX8_PINCONFIG_PULL_NONE,
	SFAX8_PINCONFIG_PULL_DOWN,
	SFAX8_PINCONFIG_PULL_UP,
};

#define SFAX8_PINCONF_PACK(_param_, _arg_) ((_param_) << 16 | (_arg_))
#define SFAX8_PINCONF_UNPACK_PARAM(_conf_) ((_conf_) >> 16)
#define SFAX8_PINCONF_UNPACK_ARG(_conf_) ((_conf_) & 0xffff)

#define IOMUX_GPIO_ONLY		BIT(0)
#define IOMUX_WIDTH_4BIT	BIT(1)
#define IOMUX_SOURCE_PMU	BIT(2)
#define IOMUX_UNROUTED		BIT(3)

#define PIN_BANK(id, pins, label)			\
	{						\
		.bank_num	= id,			\
		.nr_pins	= pins,			\
		.name		= label,		\
		.iomux		= {			\
			{ .offset = -1 },		\
			{ .offset = -1 },		\
			{ .offset = -1 },		\
			{ .offset = -1 },		\
		},					\
	}

struct sfax8_gpio_irqdata {
	struct sfax8_pinctrl *pc;
	int bank;
};

enum siflower_pinctrl_type {
	sfax8,
};

struct siflower_iomux {
	int				type;
	int				offset;
};

struct siflower_pin_bank {
	void __iomem			*reg_base;
	struct regmap			*regmap_pull;
	struct clk			*clk;
	int				irq;
	u32				pin_base;
	u8				nr_pins;
	char				*name;
	u8				bank_num;
	struct siflower_iomux		iomux[4];
	bool				valid;
	struct device_node		*of_node;
	struct sfax8_pinctrl		*drvdata;
	struct irq_domain		*domain;
	struct gpio_chip		gpio_chip;
	struct pinctrl_gpio_range	grange;
	spinlock_t			slock;
	u32				toggle_edge_mode;
};

struct siflower_pin_config {
	unsigned int		func;
	unsigned long		*configs;
	unsigned int		nconfigs;
};

struct siflower_pin_ctrl {
	struct siflower_pin_bank	*pin_banks;
	u32				nr_banks;
	u32				nr_pins;
	char				*label;
	enum siflower_pinctrl_type	type;
	int				grf_mux_offset;
	int				pmu_mux_offset;
	void	(*pull_calc_reg)(struct siflower_pin_bank *bank,
				    int pin_num, struct regmap **regmap,
				    int *reg, u8 *bit);
};

struct siflower_pin_group {
	const char			*name;
	unsigned int			npins;
	unsigned int			*pins;
	struct siflower_pin_config	*data;
};

struct siflower_pmx_func {
	const char		*name;
	const char		**groups;
	u8			ngroups;
};

struct sfax8_pinctrl {
	struct device *dev;
	void __iomem *base;
	int irq[SFAX8_NUM_BANKS];

	/* note: locking assumes each bank will have its own unsigned long */
	unsigned long enabled_irq_map[SFAX8_NUM_BANKS];
	unsigned int irq_type[SFAX8_NUM_GPIOS];

	struct pinctrl_dev *pctl_dev;
	struct irq_domain *irq_domain;
	struct gpio_chip gpio_chip;
	struct pinctrl_gpio_range gpio_range;

	struct sfax8_gpio_irqdata irq_data[SFAX8_NUM_BANKS];
	spinlock_t irq_lock[SFAX8_NUM_BANKS];

	struct regmap			*regmap_base;
	int				reg_size;
	struct regmap			*regmap_pull;
	struct regmap			*regmap_pmu;
	struct siflower_pin_ctrl	*ctrl;
	struct pinctrl_desc		pctl;
	struct siflower_pin_group	*groups;
	unsigned int			ngroups;
	struct siflower_pmx_func	*functions;
	unsigned int			nfunctions;
};

static struct lock_class_key gpio_lock_class;

/* pins are just named GPIO0..GPIO62 */
#define SFAX8_GPIO_PIN(a) PINCTRL_PIN(a, "gpio" #a)

#ifdef CONFIG_DEBUG_FS
static const char * const irq_type_names[] = {
	[IRQ_TYPE_NONE] = "none",
	[IRQ_TYPE_EDGE_RISING] = "edge-rising",
	[IRQ_TYPE_EDGE_FALLING] = "edge-falling",
	[IRQ_TYPE_EDGE_BOTH] = "edge-both",
	[IRQ_TYPE_LEVEL_HIGH] = "level-high",
	[IRQ_TYPE_LEVEL_LOW] = "level-low",
};
#endif

static inline struct siflower_pin_bank *gc_to_pin_bank(struct gpio_chip *chip)
{
	return container_of(chip, struct siflower_pin_bank, gpio_chip);
}

static inline u32 sfax8_gpio_rd(struct sfax8_pinctrl *pc, unsigned reg)
{
	return readl(pc->base + reg);
}

static inline void sfax8_gpio_wr(struct sfax8_pinctrl *pc, unsigned reg,
		u32 val)
{
	writel(val, pc->base + reg);
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin funciton.                                                       *
*                                                                                                 *
***************************************************************************************************/
#ifdef CONFIG_SOC_SF19A28
static int sfax8_pad_set_func(struct sfax8_pinctrl *info, u32 index, pad_func func)
{
	int tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	switch (func)
	{
		// write 0,0,0 to fmux_sel_reg mode_bit1_reg mode_bit0_reg
		case FUNC0:
		case FUNC1:
		case FUNC2:
		case FUNC3:
			regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
			tmp &= ~(0x7 << 0);
			tmp |= (func << 0) | (0x1 << FUNC_SW_SEL_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG1(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp &= ~(0x1 << SW_OEN_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp |=(0x1 << SW_IE_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);
			break;

		case GPIO_INPUT:
			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp |=(0x1 << SW_OEN_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp |=(0x1 << SW_IE_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
			tmp |=(0x1 << FMUX_SEL_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG1(index), tmp);

			sfax8_gpio_wr(info, GPIO_DIR(index), 1);
			break;

		case GPIO_OUTPUT:
			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp &= ~(0x1 << SW_OEN_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG0(index), &tmp);
			tmp &=~ (0x1 << SW_IE_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG0(index), tmp);

			regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
			tmp |=(0x1 << FMUX_SEL_REG);
			regmap_write(info->regmap_base, PAD_INDEX_REG1(index), tmp);

			sfax8_gpio_wr(info, GPIO_DIR(index), 0);
			break;

		default:
			printk("sf_pad_set_func error! index:%d func:%d \n", index, func);
			return -EINVAL;
			break;
	}

	return 0;
}

static u32 sfax8_pad_get_value(struct sfax8_pinctrl *info, u32 index)
{
	unsigned int tmp;
	int level = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
	if (tmp & (1 << FMUX_SEL_REG))
	{
		tmp = sfax8_gpio_rd(info, GPIO_DIR(index));
		if (tmp)
			level = sfax8_gpio_rd(info, GPIO_RDAT(index));
		else
			level = sfax8_gpio_rd(info, GPIO_WDAT(index));
	}

	return level;
}
#else
struct sfax8_pin_message {
	u32		mod_cnt;
	u32		mod_num;
	u32		fun_cnt;
	u32		fun_num;
};

static void sfax8_get_pin_message(struct sfax8_pin_message *pin_msg, u32 index)
{
	pin_msg->fun_cnt = PAD_FUCN_SEL(index / 8);
	pin_msg->fun_num = index % 8;
	pin_msg->mod_cnt = PAD_MODE_SEL(index / PAD_PER_GROUP_PINS);
	pin_msg->mod_num = index % PAD_PER_GROUP_PINS;
}

static int sfax8_pad_set_func(struct sfax8_pinctrl *info, u32 index, pad_func func)
{
	int tmp;
	struct sfax8_pin_message pin_msg;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	sfax8_get_pin_message(&pin_msg, index);

	switch (func)
	{
		case FUNC0:
		case FUNC1:
		case FUNC2:
		case FUNC3:
		regmap_read(info->regmap_base, pin_msg.fun_cnt, &tmp);
		tmp &= ~(1 << pin_msg.fun_num);
		regmap_write(info->regmap_base, pin_msg.fun_cnt, tmp);

		regmap_read(info->regmap_base, pin_msg.mod_cnt, &tmp);
		tmp &= ~(0x3 << (pin_msg.mod_num * 2));
		tmp |= (func << (pin_msg.mod_num * 2));
		regmap_write(info->regmap_base, pin_msg.mod_cnt, tmp);
		break;

		case GPIO_INPUT:
		case GPIO_OUTPUT:
		regmap_read(info->regmap_base, pin_msg.fun_cnt, &tmp);
		tmp |= 1 << pin_msg.fun_num;
		regmap_write(info->regmap_base, pin_msg.fun_cnt, tmp);

		tmp = (func == GPIO_INPUT) ? 1 : 0;
		sfax8_gpio_wr(info, GPIO_DIR(index), tmp);
		break;

		default:
		printk("sfax8_pad_set_func error! index:%d func:%d \n", index, func);
		return -EINVAL;
		break;
	}

	return 0;
}

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to get pin output level.                                                   *
*                                                                                                 *
***************************************************************************************************/
static u32 sfax8_pad_get_value(struct sfax8_pinctrl *info, u32 index)
{
	int tmp;
	int cnt = 0;
	int num = 0;
	int level = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	cnt = index / 8;
	num = index % 8;

	regmap_read(info->regmap_base, PAD_FUCN_SEL(cnt), &tmp);
	if (tmp & (1 << num))
	{
		tmp = sfax8_gpio_rd(info, GPIO_DIR(index));
		if (tmp)
			level = sfax8_gpio_rd(info, GPIO_RDAT(index));
		else
			level = sfax8_gpio_rd(info, GPIO_WDAT(index));
	}

	return level;
}

#endif

#ifdef CONFIG_DEBUG_FS
/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to get pin function.                                                       *
*                                                                                                 *
***************************************************************************************************/
#ifdef CONFIG_SOC_SF19A28
static pad_func sfax8_pad_get_func(struct sfax8_pinctrl *info, u32 index)
{
	unsigned int tmp;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
	if (tmp & (0x1 << FMUX_SEL_REG)) {
		tmp = sfax8_gpio_rd(info, GPIO_DIR(index));
		if (tmp)
			return GPIO_OUTPUT;
		else
			return GPIO_INPUT;
	} else {
		regmap_read(info->regmap_base, PAD_INDEX_REG1(index), &tmp);
		tmp = (tmp & (0x3 << MODE_BIT0_REG) ) >> MODE_BIT0_REG;
		switch(tmp) {
		case 0:
			return FUNC0;
			break;
		case 1:
			return FUNC1;
			break;
		default:
			printk("sf_pad_get_func error! index:%d\n", index);
			return FUNC0;
			break;
		}

	}

}
#else
static pad_func sfax8_pad_get_func(struct sfax8_pinctrl *info, u32 index)
{
	int tmp;
	struct sfax8_pin_message pin_msg;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	sfax8_get_pin_message(&pin_msg, index);

	regmap_read(info->regmap_base, pin_msg.fun_cnt, &tmp);
	if (tmp & (1 << pin_msg.fun_num))
	{
		tmp = sfax8_gpio_rd(info, GPIO_DIR(index));
		if (tmp)
			return GPIO_OUTPUT;
		else
			return GPIO_INPUT;
	}
	else
	{
		regmap_read(info->regmap_base, pin_msg.mod_cnt, &tmp);
		switch((tmp & (0x11 <<pin_msg.mod_num)) >> pin_msg.mod_num)
		{
			case 0:
			return FUNC0;
			break;

			case 1:
			return FUNC1;
			break;

			case 2:
			return FUNC2;
			break;

			case 3:
			return FUNC3;
			break;

			default:
			printk("sfax8_pad_get_func error! index:%d func:%d \n", index, tmp);
			return FUNC0;
			break;
		}

	}
}
#endif /* CONFIG_SOC_SF19A28 */

static inline enum sfax8_fsel sfax8_pinctrl_fsel_get(
		struct sfax8_pinctrl *pc, unsigned pin)
{
	u32 val = sfax8_pad_get_func(pc, pin);
	enum sfax8_fsel status = val;

	dev_dbg(pc->dev, "get %08x (%u => %s)\n", val, pin,
			sfax8_functions[status]);

	return status;
}
#endif /* CONFIG_DEBUG_FS */

#ifdef CONFIG_SFAX8_PINCONF
/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin pull type.                                                      *
*                                                                                                 *
***************************************************************************************************/
static int sfax8_pad_set_pull(struct regmap *regmap_base, u32 index, pad_pull pull)
{
	int tmp_pu = 0,tmp_pd = 0;
	int cnt = 0;
	int num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	cnt = index / 8;
	num = index % 8;

	regmap_read(regmap_base, PAD_IO_PULLUP(cnt), &tmp_pu);
	regmap_read(regmap_base, PAD_IO_PULLDOWN(cnt), &tmp_pd);

	switch (pull)
	{
		case NOPULL:
		tmp_pu &= ~(1 << num);
		tmp_pd &= ~(1 << num);
		break;

		case PULLUP:
		tmp_pd &= ~(1 << num);
		tmp_pu |= (1 << num);
		break;

		case PULLDOWN:
		tmp_pu &= ~(1 << num);
		tmp_pd |= (1 << num);
		break;

		default:
		return -EINVAL;
		break;
	}

	regmap_write(regmap_base, PAD_IO_PULLUP(cnt), tmp_pu);
	regmap_write(regmap_base, PAD_IO_PULLDOWN(cnt), tmp_pd);

	return 0;
}


/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to get pin pull type.                                                      *
*                                                                                                 *
***************************************************************************************************/
static pad_pull sfax8_pad_get_pull(struct regmap *regmap_base, u32 index)
{
	int tmpu = 0,tmpd = 0;
	int cnt = 0;
	int num = 0;

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	cnt = index / 8;
	num = index % 8;

	regmap_read(regmap_base, PAD_IO_PULLUP(cnt), &tmpu);
	tmpu = tmpu & (1 << num);

	regmap_read(regmap_base, PAD_IO_PULLDOWN(cnt), &tmpd);
	tmpd = tmpd & (1 << num);

	if((tmpu > 0) && (tmpd == 0))
		return PULLUP;
	else if((tmpu == 0) && (tmpd > 0))
		return PULLDOWN;
	else if((tmpu == 0) && (tmpd == 0))
		return NOPULL;
	else
	{
		printk("sf pad_%d pull status error!!\n",index);
		return -EINVAL;
	}
}
#endif

/**************************************************************************************************
*                                                                                                 *
* -Description:                                                                                   *
*    This part is used to set pin output level.                                                   *
*                                                                                                 *
***************************************************************************************************/
static int sfax8_pad_set_value(struct sfax8_pinctrl *info, u32 index, pad_output_level level)
{

	if(index > PAD_INDEX_MAX)
		return -EINVAL;

	sfax8_gpio_wr(info, GPIO_WDAT(index), level);

	return 0;
}

static int sfax8_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	return pinctrl_request_gpio(chip->base + offset);
}

static void sfax8_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	pinctrl_free_gpio(chip->base + offset);
}

static int sfax8_gpio_direction_input(struct gpio_chip *chip, unsigned offset)
{
	return pinctrl_gpio_direction_input(chip->base + offset);
}

static int sfax8_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct sfax8_pinctrl *pc = dev_get_drvdata(chip->parent);

	return sfax8_pad_get_value(pc, offset);
}

static int sfax8_gpio_direction_output(struct gpio_chip *chip,
		unsigned offset, int value)
{
	struct sfax8_pinctrl *pc = dev_get_drvdata(chip->parent);

	pinctrl_gpio_direction_output(chip->base + offset);

	return sfax8_pad_set_value(pc, offset, value);
}


static void sfax8_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct sfax8_pinctrl *pc = dev_get_drvdata(chip->parent);

	sfax8_pad_set_value(pc, offset, value);
}

static int sfax8_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct sfax8_pinctrl *pc = dev_get_drvdata(chip->parent);

	return irq_linear_revmap(pc->irq_domain, offset);
}

static struct gpio_chip sfax8_gpio_chip = {
	.label = MODULE_NAME,
	.owner = THIS_MODULE,
	.request = sfax8_gpio_request,
	.free = sfax8_gpio_free,
	.direction_input = sfax8_gpio_direction_input,
	.direction_output = sfax8_gpio_direction_output,
	.get = sfax8_gpio_get,
	.set = sfax8_gpio_set,
	.to_irq = sfax8_gpio_to_irq,
	.base = -1,
	.ngpio = SFAX8_NUM_GPIOS,
	.can_sleep = false,
};

static irqreturn_t sfax8_gpio_irq_handler(int irq, void *dev_id)
{
	struct sfax8_gpio_irqdata *irqdata = dev_id;
	struct sfax8_pinctrl *pc = irqdata->pc;
	int bank = irqdata->bank;
	unsigned long events;
	unsigned offset;
	unsigned gpio;
	unsigned int type, pending;

	if(!pc)
		return IRQ_NONE;

	if(pc->irq[bank] != irq) {
		printk("irq number error!\n");
		return IRQ_NONE;
	}

	events = pc->enabled_irq_map[bank];
	for_each_set_bit(offset, &events, 32) {
		gpio = (SFAX8_GPIOS_PER_BANK * bank) + offset;
		type = pc->irq_type[gpio];

		pending = sfax8_gpio_rd(pc, GPIO_INTPEND(gpio));

		/* ack edge triggered IRQs immediately */
		if (!(type & IRQ_TYPE_LEVEL_MASK))
			sfax8_gpio_wr(pc, GPIO_INTPEND(gpio), 0);

		if (pending)
			generic_handle_irq(irq_linear_revmap(pc->irq_domain, gpio));

		/* ack level triggered IRQ after handling them */
		if (type & IRQ_TYPE_LEVEL_MASK)
			sfax8_gpio_wr(pc, GPIO_INTPEND(gpio), 0);
	}

	return IRQ_HANDLED;
}

/* fast path for IRQ handler */
static void sfax8_gpio_irq_enable(struct irq_data *data)
{
	struct sfax8_pinctrl *pc = irq_data_get_irq_chip_data(data);
	unsigned gpio = irqd_to_hwirq(data);
	unsigned offset = gpio%SFAX8_GPIOS_PER_BANK;
	unsigned bank = gpio/SFAX8_GPIOS_PER_BANK;
	unsigned long flags;

	spin_lock_irqsave(&pc->irq_lock[bank], flags);
	set_bit(offset, &pc->enabled_irq_map[bank]);
	sfax8_gpio_wr(pc, GPIO_INTGMSK(gpio), 0);
	sfax8_gpio_wr(pc, GPIO_INTMSK(gpio), 0);
	spin_unlock_irqrestore(&pc->irq_lock[bank], flags);
}

static void sfax8_gpio_irq_disable(struct irq_data *data)
{
	struct sfax8_pinctrl *pc = irq_data_get_irq_chip_data(data);
	unsigned gpio = irqd_to_hwirq(data);
	unsigned offset = gpio%SFAX8_GPIOS_PER_BANK;
	unsigned bank = gpio/SFAX8_GPIOS_PER_BANK;
	unsigned long flags;

	spin_lock_irqsave(&pc->irq_lock[bank], flags);
	clear_bit(offset, &pc->enabled_irq_map[bank]);
	sfax8_gpio_wr(pc, GPIO_INTMSK(gpio), 1);
	sfax8_gpio_wr(pc, GPIO_INTGMSK(gpio), 1);
	spin_unlock_irqrestore(&pc->irq_lock[bank], flags);
}

static int sfax8_gpio_irq_set_type(struct irq_data *data, unsigned int type)
{
	struct sfax8_pinctrl *pc = irq_data_get_irq_chip_data(data);
	unsigned gpio = irqd_to_hwirq(data);

	switch (type) {
		case IRQF_TRIGGER_NONE:
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 0);
			break;
		case IRQF_TRIGGER_RISING:
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 2);
			break;
		case IRQF_TRIGGER_FALLING:
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 4);
			break;
		case IRQF_TRIGGER_HIGH:
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 1);
			break;
		case IRQF_TRIGGER_LOW:
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 0);
			break;

		case (IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING):
			sfax8_gpio_wr(pc, GPIO_INTTYPE(gpio), 6);
			break;
		default:
			pr_warn("IRQ trigger type not valid!\n");
	}
	pc->irq_type[gpio] = type;

	return 0;
}

static struct irq_chip sfax8_gpio_irq_chip = {
	.name = MODULE_NAME,
	.irq_enable = sfax8_gpio_irq_enable,
	.irq_disable = sfax8_gpio_irq_disable,
	.irq_set_type = sfax8_gpio_irq_set_type,
};

static int sfax8_pctl_get_groups_count(struct pinctrl_dev *pctldev)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	return info->ngroups;
}

static const char *sfax8_pctl_get_group_name(struct pinctrl_dev *pctldev,
		unsigned selector)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	return info->groups[selector].name;
}

static int sfax8_pctl_get_group_pins(struct pinctrl_dev *pctldev,
		unsigned selector,
		const unsigned **pins,
		unsigned *num_pins)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	if(selector >= info->ngroups)
		return -EINVAL;

	*pins = info->groups[selector].pins;
	*num_pins = info->groups[selector].npins;

	return 0;
}

static void sfax8_pctl_pin_dbg_show(struct pinctrl_dev *pctldev,
		struct seq_file *s,
		unsigned offset)
{
#ifdef CONFIG_DEBUG_FS
	struct sfax8_pinctrl *pc = pinctrl_dev_get_drvdata(pctldev);
	enum sfax8_fsel fsel = sfax8_pinctrl_fsel_get(pc, offset);
	const char *fname = sfax8_functions[fsel];
	int value = sfax8_pad_get_value(pc, offset);
	int irq = irq_find_mapping(pc->irq_domain, offset);

	seq_printf(s, "function %s in %s; irq %d (%s)",
		fname, value ? "hi" : "lo",
		irq, irq_type_names[pc->irq_type[offset]]);
#endif
}

static void sfax8_pctl_dt_free_map(struct pinctrl_dev *pctldev,
		struct pinctrl_map *maps, unsigned num_maps)
{
	/*
	 * We must free maps here because when
	 * alloc mem for maps, the dev is set to
	 * pctldev->dev, which won't be removed while
	 * create_pinctrl <=> pinctrl_free.So if not,
	 * there will be a memory leak.
	 */
	devm_kfree(pctldev->dev, maps);
	return;
}

static const inline struct siflower_pin_group *pinctrl_name_to_group(
					const struct sfax8_pinctrl *info,
					const char *name)
{
	int i;

	for (i = 0; i < info->ngroups; i++) {
		if (!strcmp(info->groups[i].name, name))
			return &info->groups[i];
	}

	return NULL;
}

static int sfax8_pctl_dt_node_to_map(struct pinctrl_dev *pctldev,
		struct device_node *np,
		struct pinctrl_map **map, unsigned *num_maps)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);
	const struct siflower_pin_group *grp;
	struct pinctrl_map *new_map;
	struct device_node *parent;
	int map_num = 1;
	int i;

	/*
	 * first find the group of this node and check if we need to create
	 * config maps for pins
	 */
	grp = pinctrl_name_to_group(info, np->name);
	if (!grp) {
		dev_err(info->dev, "unable to find group for node %s\n",
			np->name);
		return -EINVAL;
	}

	map_num += grp->npins;
	new_map = devm_kzalloc(pctldev->dev, sizeof(*new_map) * map_num,
								GFP_KERNEL);
	if (!new_map)
		return -ENOMEM;

	*map = new_map;
	*num_maps = map_num;

	/* create mux map */
	parent = of_get_parent(np);
	if (!parent) {
		devm_kfree(pctldev->dev, new_map);
		return -EINVAL;
	}
	new_map[0].type = PIN_MAP_TYPE_MUX_GROUP;
	new_map[0].data.mux.function = parent->name;
	new_map[0].data.mux.group = np->name;
	of_node_put(parent);
	/* create config map */
	new_map++;
	for (i = 0; i < grp->npins; i++) {
		new_map[i].type = PIN_MAP_TYPE_CONFIGS_PIN;
		new_map[i].data.configs.group_or_pin =
				pin_get_name(pctldev, grp->pins[i]);
		new_map[i].data.configs.configs = grp->data[i].configs;
		new_map[i].data.configs.num_configs = grp->data[i].nconfigs;
	}

	dev_dbg(pctldev->dev, "maps: function %s group %s num %d\n",
		(*map)->data.mux.function, (*map)->data.mux.group, map_num);

	return 0;
}

static const struct pinctrl_ops sfax8_pctl_ops = {
	.get_groups_count = sfax8_pctl_get_groups_count,
	.get_group_name = sfax8_pctl_get_group_name,
	.get_group_pins = sfax8_pctl_get_group_pins,
	.pin_dbg_show = sfax8_pctl_pin_dbg_show,
	.dt_node_to_map = sfax8_pctl_dt_node_to_map,
	.dt_free_map = sfax8_pctl_dt_free_map,
};

static int sfax8_pmx_get_functions_count(struct pinctrl_dev *pctldev)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	return info->nfunctions;
}

static const char *sfax8_pmx_get_function_name(struct pinctrl_dev *pctldev,
		unsigned selector)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	return info->functions[selector].name;
}

static int sfax8_pmx_get_function_groups(struct pinctrl_dev *pctldev,
		unsigned selector,
		const char * const **groups,
		unsigned * const num_groups)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	*groups = info->functions[selector].groups;
	*num_groups = info->functions[selector].ngroups;

	return 0;
}

static int sfax8_pmx_set(struct pinctrl_dev *pctldev,
		unsigned selector,
		unsigned group)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);
	const unsigned int *pins = info->groups[group].pins;
	const struct siflower_pin_config *data = info->groups[group].data;
	int cnt, ret = 0;

	dev_dbg(info->dev, "enable function %s group %s\n",
		info->functions[selector].name, info->groups[group].name);

	/*
	 * for each pin in the pin group selected, program the correspoding pin
	 * pin function number in the config register.
	 */
	for (cnt = 0; cnt < info->groups[group].npins; cnt++) {
		sfax8_pad_set_func(info, pins[cnt], data[cnt].func);
		if (ret)
			break;
	}

	return 0;
}

static void sfax8_pmx_gpio_disable_free(struct pinctrl_dev *pctldev,
		struct pinctrl_gpio_range *range,
		unsigned offset)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);

	/* disable by setting to GPIO_IN */
	sfax8_pad_set_func(info, offset, SFAX8_FSEL_GPIO_IN);
}

static int sfax8_pmx_gpio_set_direction(struct pinctrl_dev *pctldev,
		struct pinctrl_gpio_range *range,
		unsigned offset,
		bool input)
{
	struct sfax8_pinctrl *info = pinctrl_dev_get_drvdata(pctldev);
	enum sfax8_fsel fsel = input ?
		SFAX8_FSEL_GPIO_IN : SFAX8_FSEL_GPIO_OUT;

	sfax8_pad_set_func(info, offset, fsel);

	return 0;
}

static const struct pinmux_ops sfax8_pmx_ops = {
	.get_functions_count = sfax8_pmx_get_functions_count,
	.get_function_name = sfax8_pmx_get_function_name,
	.get_function_groups = sfax8_pmx_get_function_groups,
	.set_mux = sfax8_pmx_set,
	.gpio_disable_free = sfax8_pmx_gpio_disable_free,
	.gpio_set_direction = sfax8_pmx_gpio_set_direction,
};

static void siflower_calc_pull_reg_and_bit(struct siflower_pin_bank *bank,
				    int pin_num, struct regmap **regmap,
				    int *reg, u8 *bit)
{
	struct sfax8_pinctrl *info = bank->drvdata;

	/* The first 24 pins of the first bank are located in PMU */
	if (bank->bank_num == 0) {
		*regmap = info->regmap_pmu;
		*reg = 0x64;//RK3188_PULL_PMU_OFFSET;

		*reg += ((pin_num / 8) * 4);
		*bit = pin_num % 8;
		*bit *= 2;
	} else {
		*regmap = info->regmap_base;
		*reg = 0x164;

		/* correct the offset, as we're starting with the 2nd bank */
		*reg -= 0x10;
		*reg += bank->bank_num * 16;
		*reg += ((pin_num / 8) * 4);

		*bit = (pin_num % 8);
		*bit *= 2;
	}
}

static int sfax8_pinconf_get(struct pinctrl_dev *pctldev,
			unsigned pin, unsigned long *config)
{
	/* No way to read back config in HW */
	return -ENOTSUPP;
}

#ifdef CONFIG_SFAX8_PINCONF
static struct siflower_pin_bank *pin_to_bank(struct sfax8_pinctrl *info,
								unsigned pin)
{
	struct siflower_pin_bank *b = info->ctrl->pin_banks;

	while (pin >= (b->pin_base + b->nr_pins))
		b++;

	return b;
}

static int siflower_set_pull(struct siflower_pin_bank *bank,
					int pin_num, int pull, struct regmap *regmap_base)
{
	struct sfax8_pinctrl *info = bank->drvdata;
	struct siflower_pin_ctrl *ctrl = info->ctrl;
	int ret;
	unsigned long flags;
	u8 bit;
	u32 data, rmask;

	dev_dbg(info->dev, "setting pull of GPIO%d-%d to %d\n",
		 bank->bank_num, pin_num, pull);

	switch (ctrl->type) {
	case sfax8:
		spin_lock_irqsave(&bank->slock, flags);

		/* enable the write to the equivalent lower bits */
		data = 1 << (bit + 16);
		rmask = data | (data >> 16);

		switch (pull) {
		case PIN_CONFIG_BIAS_DISABLE:
			ret = sfax8_pad_set_pull(regmap_base,pin_num,NOPULL);
			break;
		case PIN_CONFIG_BIAS_PULL_UP:
			ret = sfax8_pad_set_pull(regmap_base,pin_num,PULLUP);
			break;
		case PIN_CONFIG_BIAS_PULL_DOWN:
			ret = sfax8_pad_set_pull(regmap_base,pin_num,PULLDOWN);
			break;
		case PIN_CONFIG_BIAS_PULL_PIN_DEFAULT:
			ret = 0;
			break;
		default:
			spin_unlock_irqrestore(&bank->slock, flags);
			dev_err(info->dev, "unsupported pull setting %d\n",
				pull);
			return -EINVAL;
		}

		spin_unlock_irqrestore(&bank->slock, flags);
		break;
	default:
		dev_err(info->dev, "unsupported pinctrl type\n");
		return -EINVAL;
	}

	return ret;
}

static int sfax8_pinconf_set(struct pinctrl_dev *pctldev,
			unsigned pin, unsigned long *configs,
			unsigned num_configs)
{

	struct sfax8_pinctrl *pc = pinctrl_dev_get_drvdata(pctldev);
	struct siflower_pin_bank *bank = pin_to_bank(pc,pin);
	enum pin_config_param param;
	u16 arg;
	int i;
	int rc;

	for (i = 0; i < num_configs; i++) {
		param = pinconf_to_config_param(configs[i]);
		arg = pinconf_to_config_argument(configs[i]);

		switch (param) {
		case PIN_CONFIG_BIAS_DISABLE:
			rc = siflower_set_pull(bank, pin - bank->pin_base,
				param, pc->regmap_base);
			if (rc)
				return rc;
			break;
		case PIN_CONFIG_BIAS_PULL_UP:
		case PIN_CONFIG_BIAS_PULL_DOWN:
		case PIN_CONFIG_BIAS_PULL_PIN_DEFAULT:
		case PIN_CONFIG_BIAS_BUS_HOLD:
			//if (!siflower_pinconf_pull_valid(param))
			//	return -ENOTSUPP;

			if (!arg)
				return -EINVAL;

			rc = siflower_set_pull(bank, pin - bank->pin_base,
				param, pc->regmap_base);
			if (rc)
				return rc;
			break;
		default:
			return -ENOTSUPP;
			break;
		}
	} /* for each config */

	return 0;
}

#else
static int sfax8_pinconf_set(struct pinctrl_dev *pctldev,
			unsigned pin, unsigned long *configs,
			unsigned num_configs)
{
	return 0;
}

#endif

static struct siflower_pin_bank *bank_num_to_bank(
					struct sfax8_pinctrl *info,
					unsigned num)
{
	struct siflower_pin_bank *b = info->ctrl->pin_banks;
	int i;

	for (i = 0; i < info->ctrl->nr_banks; i++, b++) {
		if (b->bank_num == num)
			return b;
	}

	return ERR_PTR(-EINVAL);
}

static const struct of_device_id sfax8_gpio_match[] = {
	{ .compatible = "siflower,sfax8-gpio" },
	{},
};

static void siflower_pinctrl_child_count(struct sfax8_pinctrl *info,
						struct device_node *np)
{
	struct device_node *child;

	for_each_child_of_node(np, child) {
		if (of_match_node(sfax8_gpio_match, child))
			continue;

		info->nfunctions++;
		info->ngroups += of_get_child_count(child);
	}
}

static int siflower_pinctrl_parse_groups(struct device_node *np,
					      struct siflower_pin_group *grp,
					      struct sfax8_pinctrl *info,
					      u32 index)
{
	struct siflower_pin_bank *bank;
	int size;
	const __be32 *list;
	int num;
	int i, j;
	int ret;

	dev_dbg(info->dev, "group(%d): %s\n", index, np->name);

	/* Initialise group */
	grp->name = np->name;

	/*
	 * the binding format is sfax8,pins = <bank pin mux CONFIG>,
	 * do sanity check and calculate pins number
	 */
	list = of_get_property(np, "sfax8,pins", &size);
	/* we do not check return since it's safe node passed down */
	size /= sizeof(*list);

	if (!size || size % 4) {
		dev_err(info->dev, "wrong pins number or pins and configs should be by 4\n");
		return -EINVAL;
	}

	grp->npins = size / 4;

	grp->pins = devm_kzalloc(info->dev, grp->npins * sizeof(unsigned int),
						GFP_KERNEL);
	grp->data = devm_kzalloc(info->dev, grp->npins *
					  sizeof(struct siflower_pin_config),
					GFP_KERNEL);
	if (!grp->pins || !grp->data)
		return -ENOMEM;

	for (i = 0, j = 0; i < size; i += 4, j++) {
		const __be32 *phandle;
		struct device_node *np_config;

		num = be32_to_cpu(*list++);
		bank = bank_num_to_bank(info, num);
		if (IS_ERR(bank))
			return PTR_ERR(bank);

		grp->pins[j] = bank->pin_base + be32_to_cpu(*list++);
		grp->data[j].func = be32_to_cpu(*list++);

		phandle = list++;
		if (!phandle)
			return -EINVAL;

		np_config = of_find_node_by_phandle(be32_to_cpup(phandle));
		ret = pinconf_generic_parse_dt_config(np_config, info->pctl_dev,
				&grp->data[j].configs, &grp->data[j].nconfigs);
		if (ret)
			return ret;
	}

	return 0;
}


static int siflower_pinctrl_parse_functions(struct device_node *np,
						struct sfax8_pinctrl *info,
						u32 index)
{
	struct device_node *child;
	struct siflower_pmx_func *func;
	struct siflower_pin_group *grp;
	int ret;
	static u32 grp_index;
	u32 i = 0;

	dev_dbg(info->dev, "parse function(%d): %s\n", index, np->name);

	func = &info->functions[index];

	/* Initialise function */
	func->name = np->name;
	func->ngroups = of_get_child_count(np);
	if (func->ngroups <= 0)
		return 0;

	func->groups = devm_kzalloc(info->dev,
			func->ngroups * sizeof(char *), GFP_KERNEL);
	if (!func->groups)
		return -ENOMEM;

	for_each_child_of_node(np, child) {
		func->groups[i] = child->name;
		grp = &info->groups[grp_index++];
		ret = siflower_pinctrl_parse_groups(child, grp, info, i++);
		if (ret)
			return ret;
	}

	return 0;
}

static int siflower_pinctrl_parse_dt(struct platform_device *pdev,
					      struct sfax8_pinctrl *info)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct device_node *child;
	int ret;
	int i;

	siflower_pinctrl_child_count(info, np);

	dev_dbg(&pdev->dev, "nfunctions = %d\n", info->nfunctions);
	dev_dbg(&pdev->dev, "ngroups = %d\n", info->ngroups);

	info->functions = devm_kzalloc(dev, info->nfunctions *
					      sizeof(struct siflower_pmx_func),
					      GFP_KERNEL);
	if (!info->functions) {
		dev_err(dev, "failed to allocate memory for function list\n");
		return -EINVAL;
	}

	info->groups = devm_kzalloc(dev, info->ngroups *
					    sizeof(struct siflower_pin_group),
					    GFP_KERNEL);
	if (!info->groups) {
		dev_err(dev, "failed allocate memory for ping group list\n");
		return -EINVAL;
	}

	i = 0;

	for_each_child_of_node(np, child) {
		if (of_match_node(sfax8_gpio_match, child))
			continue;

		ret = siflower_pinctrl_parse_functions(child, info, i++);
		if (ret) {
			dev_err(&pdev->dev, "failed to parse function\n");
			return ret;
		}
	}

	return 0;
}

static int siflower_get_bank_data(struct siflower_pin_bank *bank,
				  struct sfax8_pinctrl *info)
{
	struct resource res;

	if (of_address_to_resource(bank->of_node, 0, &res)) {
		dev_err(info->dev, "cannot find IO resource for bank\n");
		return -ENOENT;
	}

	bank->reg_base = devm_ioremap_resource(info->dev, &res);
	if (IS_ERR(bank->reg_base))
		return PTR_ERR(bank->reg_base);

	bank->irq = irq_of_parse_and_map(bank->of_node, 0);

	bank->clk = of_clk_get(bank->of_node, 0);
	if (IS_ERR(bank->clk))
		return PTR_ERR(bank->clk);

	return clk_prepare_enable(bank->clk);
}


static struct of_device_id sfax8_pinctrl_match[];

static struct siflower_pin_ctrl *siflower_pinctrl_get_soc_data(
						struct sfax8_pinctrl *d,
						struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct device_node *node = pdev->dev.of_node;
	struct device_node *np;
	struct siflower_pin_ctrl *ctrl;
	struct siflower_pin_bank *bank;
	int grf_offs = 0, pmu_offs = 0, i, j;

	match = of_match_node(sfax8_pinctrl_match, node);
	ctrl = (struct siflower_pin_ctrl *)match->data;

	for_each_child_of_node(node, np) {
		if (!of_find_property(np, "gpio-controller", NULL))
			continue;

		bank = ctrl->pin_banks;
		for (i = 0; i < ctrl->nr_banks; ++i, ++bank) {
			if (!strcmp(bank->name, np->name)) {
				bank->of_node = np;

				if (!siflower_get_bank_data(bank, d))
					bank->valid = true;

				break;
			}
		}
	}

	bank = ctrl->pin_banks;
	for (i = 0; i < ctrl->nr_banks; ++i, ++bank) {
		int bank_pins = 0;

		spin_lock_init(&bank->slock);
		bank->drvdata = d;
		bank->pin_base = ctrl->nr_pins;
		ctrl->nr_pins += bank->nr_pins;

		/* calculate iomux offsets */
		for (j = 0; j < 4; j++) {
			struct siflower_iomux *iom = &bank->iomux[j];
			int inc;

			if (bank_pins >= bank->nr_pins)
				break;

			/* preset offset value, set new start value */
			if (iom->offset >= 0) {
				if (iom->type & IOMUX_SOURCE_PMU)
					pmu_offs = iom->offset;
				else
					grf_offs = iom->offset;
			} else {
			/* set current offset */
				iom->offset = (iom->type & IOMUX_SOURCE_PMU) ?
							pmu_offs : grf_offs;
			}

			dev_dbg(d->dev, "bank %d, iomux %d has offset 0x%x\n",
				 i, j, iom->offset);

			/*
			 * Increase offset according to iomux width.
			 * 4bit iomux'es are spread over two registers.
			 */
			inc = (iom->type & IOMUX_WIDTH_4BIT) ? 8 : 4;
			if (iom->type & IOMUX_SOURCE_PMU)
				pmu_offs += inc;
			else
				grf_offs += inc;

			bank_pins += 8;
		}
	}

	return ctrl;
}



static const struct pinconf_ops sfax8_pinconf_ops = {
	.pin_config_get = sfax8_pinconf_get,
	.pin_config_set = sfax8_pinconf_set,
};

static struct pinctrl_desc sfax8_pinctrl_desc = {
	.name = MODULE_NAME,
	//.pins = sfax8_gpio_pins,
	//.npins = ARRAY_SIZE(sfax8_gpio_pins),
	.pctlops = &sfax8_pctl_ops,
	.pmxops = &sfax8_pmx_ops,
	.confops = &sfax8_pinconf_ops,
	.owner = THIS_MODULE,
};

static struct pinctrl_gpio_range sfax8_pinctrl_gpio_range = {
	.name = MODULE_NAME,
	.npins = SFAX8_NUM_GPIOS,
};

static int sfax8_pinctrl_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np_gpio, *np = dev->of_node, *node;
	struct sfax8_pinctrl *pc;
	struct siflower_pin_ctrl *ctrl;
	//struct siflower_pin_bank *bank = pc->ctrl->pin_banks;
	struct pinctrl_pin_desc *pdesc;
	struct siflower_pin_bank *pin_bank;
	int err, i, bank, k, pin;

	pc = devm_kzalloc(dev, sizeof(*pc), GFP_KERNEL);
	if (!pc){
		dev_err(dev, "the pc is null!\n");
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, pc);
	pc->dev = dev;

	ctrl = siflower_pinctrl_get_soc_data(pc, pdev);
	if (!ctrl) {
		dev_err(dev, "driver data not available\n");
		return -EINVAL;
	}
	pc->ctrl = ctrl;

	pc->base = pc->ctrl->pin_banks->reg_base;

	node = of_parse_phandle(np, "pad-base", 0);
	if (node) {
		pc->regmap_base = syscon_node_to_regmap(node);
		if (IS_ERR(pc->regmap_base))
			return PTR_ERR(pc->regmap_base);
	}

	regcache_cache_bypass(pc->regmap_base, true);

	pc->gpio_chip = sfax8_gpio_chip;
	pc->gpio_chip.parent = dev;
	pc->gpio_chip.of_node = pc->ctrl->pin_banks->of_node;//np;
	pc->gpio_chip.base = pc->ctrl->pin_banks->pin_base;
	pc->gpio_chip.ngpio = pc->ctrl->pin_banks->nr_pins;
	pc->gpio_chip.label = pc->ctrl->pin_banks->name;

	pc->irq_domain = irq_domain_add_linear(np, SFAX8_NUM_GPIOS,
			&irq_domain_simple_ops, NULL);
	if (!pc->irq_domain) {
		dev_err(dev, "could not create IRQ domain\n");
		return -ENOMEM;
	}

	for (i = 0; i < SFAX8_NUM_GPIOS; i++) {
		int irq = irq_create_mapping(pc->irq_domain, i);
		irq_set_lockdep_class(irq, &gpio_lock_class);
		irq_set_chip_and_handler(irq, &sfax8_gpio_irq_chip,
				handle_simple_irq);
		irq_set_chip_data(irq, pc);
	}

	np_gpio = of_find_compatible_node(np, NULL, "siflower,sfax8-gpio");
	if(!np_gpio)
		dev_err(dev, "Can't get gpio configuration in dts.\n");

	for (i = 0; i < SFAX8_NUM_BANKS; i++) {
		int len;
		char *name;

		pc->irq[i] = of_irq_get(np_gpio, i);
		pc->irq_data[i].pc = pc;
		pc->irq_data[i].bank = i;
		spin_lock_init(&pc->irq_lock[i]);

		len = strlen(dev_name(pc->dev)) + 16;
		name = devm_kzalloc(pc->dev, len, GFP_KERNEL);
		if (!name)
			return -ENOMEM;
		snprintf(name, len, "%s:bank%d", dev_name(pc->dev), i);

		err = devm_request_irq(dev, pc->irq[i],
			sfax8_gpio_irq_handler, IRQF_SHARED,
			name, &pc->irq_data[i]);
		if (err) {
			dev_err(dev, "unable to request IRQ %d\n", pc->irq[i]);
			return err;
		}
	}

	err = gpiochip_add(&pc->gpio_chip);
	if (err) {
		dev_err(dev, "could not add GPIO chip\n");
		return err;
	}

	//add here for a test! Mark
	pdesc = devm_kzalloc(&pdev->dev, sizeof(*pdesc) *
			pc->ctrl->nr_pins, GFP_KERNEL);

	sfax8_pinctrl_desc.pins = pdesc;
	sfax8_pinctrl_desc.npins = pc->ctrl->nr_pins;

	for (bank = 0 , k = 0; bank < pc->ctrl->nr_banks; bank++) {
		pin_bank = &pc->ctrl->pin_banks[bank];
		for (pin = 0; pin < pin_bank->nr_pins; pin++, k++) {
			pdesc->number = k;
			pdesc->name = kasprintf(GFP_KERNEL, "%s-%d",
						pin_bank->name, pin);
			pdesc++;
		}
	}

	pc->pctl_dev = pinctrl_register(&sfax8_pinctrl_desc, dev, pc);
	if (!pc->pctl_dev) {
		gpiochip_remove(&pc->gpio_chip);
		return -EINVAL;
	}

	pc->gpio_range = sfax8_pinctrl_gpio_range;
	pc->gpio_range.base = pc->gpio_chip.base;
	pc->gpio_range.gc = &pc->gpio_chip;
	pinctrl_add_gpio_range(pc->pctl_dev, &pc->gpio_range);

	for (bank = 0; bank < pc->ctrl->nr_banks; ++bank) {
		pin_bank = &pc->ctrl->pin_banks[bank];
		pin_bank->grange.name = pin_bank->name;
		pin_bank->grange.id = bank;
		pin_bank->grange.pin_base = pin_bank->pin_base;
		pin_bank->grange.base = pin_bank->gpio_chip.base;
		pin_bank->grange.npins = pin_bank->gpio_chip.ngpio;
		pin_bank->grange.gc = &pin_bank->gpio_chip;
		pinctrl_add_gpio_range(pc->pctl_dev, &pin_bank->grange);
	}

	siflower_pinctrl_parse_dt(pdev, pc);

	return 0;
}

static int sfax8_pinctrl_remove(struct platform_device *pdev)
{
	struct sfax8_pinctrl *pc = platform_get_drvdata(pdev);

	pinctrl_unregister(pc->pctl_dev);
	gpiochip_remove(&pc->gpio_chip);

	return 0;
}

static struct siflower_pin_bank sfax8_pin_banks[] = {
	PIN_BANK(0, SFAX8_NUM_GPIOS, "gpio"),
};

static struct siflower_pin_ctrl sfax8_pin_ctrl = {
		.pin_banks		= sfax8_pin_banks,
		.nr_banks		= ARRAY_SIZE(sfax8_pin_banks),
		.label			= "SFAX8-GPIO",
		.type			= 0,
		.pull_calc_reg		= siflower_calc_pull_reg_and_bit,
};

static struct of_device_id sfax8_pinctrl_match[] = {
	{ .compatible = "siflower,sfax8-pinctrl",
		.data = (void *)&sfax8_pin_ctrl },
	{}
};
MODULE_DEVICE_TABLE(of, sfax8_pinctrl_match);

static struct platform_driver sfax8_pinctrl_driver = {
	.probe = sfax8_pinctrl_probe,
	.remove = sfax8_pinctrl_remove,
	.driver = {
		.name = MODULE_NAME,
		.owner = THIS_MODULE,
		.of_match_table = sfax8_pinctrl_match,
	},
};
//module_platform_driver(sfax8_pinctrl_driver);
static int __init sfax8_pinctrl_drv_register(void)
{
	return platform_driver_register(&sfax8_pinctrl_driver);
}
postcore_initcall(sfax8_pinctrl_drv_register);

MODULE_AUTHOR("Mark.cai");
MODULE_DESCRIPTION("SFAX8 Pin control driver");
MODULE_LICENSE("GPL");
