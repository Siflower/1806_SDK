/*
 * Copyright (c) 2015 Microchip Technology Inc
 * Purna Chandra Mandal <purna.mandal@microchip.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <mach/pad.h>


struct sfax8_gpio_priv {
	u32 pad_base;
	u32 gpio_base;
	char name[2];
};


DECLARE_GLOBAL_DATA_PTR;



static int sf_gpio_get_value(struct udevice *dev, unsigned offset)
{
	struct sfax8_gpio_priv *priv = dev_get_priv(dev);

	u8 tmp;
	int level = 0;

	if(offset > PAD_INDEX_MAX)
		return -EINVAL;

	tmp = readb((const void *)GPIO_DIR(priv->gpio_base, offset));
	if (tmp)
		level = readb((const void *)GPIO_RDAT(priv->gpio_base, offset));
	else
		level = readb((const void *)GPIO_WDAT(priv->gpio_base, offset));

	return level;
}

static int sf_gpio_set_value(struct udevice *dev, unsigned offset, int value)
{
	struct sfax8_gpio_priv *priv = dev_get_priv(dev);

	if(offset > PAD_INDEX_MAX)
		return -EINVAL;

	writeb(value, (volatile void*)GPIO_WDAT(priv->gpio_base, offset));

	return 0;
}
static int sf_gpio_direction(struct udevice *dev,unsigned offset, u32 io)
{
	u8 tmp;
	struct sfax8_gpio_priv *priv = dev_get_priv(dev);

	if(offset > PAD_INDEX_MAX)
		return -EINVAL;
#if defined(CONFIG_SOC_SFA28_MPW0) || defined(CONFIG_SOC_SFA28_FULLMASK)
	tmp = readb((void *)PAD_IO_REG1(priv->pad_base, offset));
	if( io == GPIOF_INPUT )
		tmp |= (SW_IE | SW_OEN);
	else if( io == GPIOF_OUTPUT )
		tmp &= ~(SW_IE | SW_OEN);
	else {
		printf("sf_gpio_direction error! offset:%d io:%d \n", offset, io);
		return -EINVAL;
	}
	writeb(tmp, (void *)PAD_IO_REG1(priv->pad_base, offset));

	tmp = readb((void *)PAD_IO_REG2(priv->pad_base, offset));
	tmp |= (FUNC_SW_SEL | FMUX_SEL);
	writeb(tmp, (void *)PAD_IO_REG2(priv->pad_base, offset));
#else
	int fun_cnt = 0;
	int fun_num = 0;
	u32 reg = 0;

	fun_cnt = offset / PAD_FUN_SEL_REG_WID;
	fun_num = offset % PAD_FUN_SEL_REG_WID;

#ifndef CONFIG_TARGET_SFA18_MPW0
	reg = FUNC_SW_SEL(priv->pad_base, fun_cnt);

	//select function/pad mode
	tmp = readb((const void *)reg);
	tmp |= 1 << fun_num;
	writeb(tmp, (volatile void *)reg);
#endif
	reg = PAD_FUCN_SEL(priv->pad_base, fun_cnt);

	//select gpio mode
	tmp = readb((const void *)reg);
	tmp |= 1 << fun_num;
	writeb(tmp, (volatile void *)reg);
#endif
	if(io == GPIOF_INPUT)
	{
		writeb(1, (volatile void *)(GPIO_DIR(priv->gpio_base, offset)));
	}
	else if(io == GPIOF_OUTPUT)
	{
		writeb(0, (volatile void *)(GPIO_DIR(priv->gpio_base, offset)));
	}
	else
	{
		printf("sf_gpio_direction error! offset:%d io:%d \n", offset, io);
		return -EINVAL;
	}

	return 0;
}

static int sf_gpio_direction_input(struct udevice *dev, unsigned offset)
{
	return sf_gpio_direction(dev, offset, GPIOF_INPUT);
}

static int sf_gpio_direction_output(struct udevice *dev,
				       unsigned offset, int value)
{
	sf_gpio_direction(dev, offset, GPIOF_OUTPUT);
	sf_gpio_set_value(dev, offset, value);
	return 0;
}

static const struct dm_gpio_ops gpio_sfax8_ops = {
	.direction_input	= sf_gpio_direction_input,
	.direction_output	= sf_gpio_direction_output,
	.get_value		= sf_gpio_get_value,
	.set_value		= sf_gpio_set_value,
};

static int sfax8_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv = dev_get_uclass_priv(dev);
	struct sfax8_gpio_priv *priv = dev_get_priv(dev);
	int ret = 0;
	struct fdt_resource res;
	void *fdt = (void *)gd->fdt_blob;
	int node = dev->of_offset;
	fdt_addr_t node_addr, node_base;

	node_addr = dev_get_addr(dev);
	if (node_addr == FDT_ADDR_T_NONE)
		    return -EINVAL;

	priv->gpio_base = node_addr; 

	ret = fdt_get_named_resource(fdt, node, "reg", "reg-names",
				     "gpio_base", &res);
	if (ret < 0) {
		printf("gpio: resource \"gpio_base\" not found\n");
		return ret;
	}
	node_base =priv->gpio_base - res.start;

	ret = fdt_get_named_resource(fdt, node, "reg", "reg-names",
				     "pad_base", &res);
	if (ret < 0) {
		printf("gpio: resource \"pad_base\" not found\n");
		return ret;
	}
	priv->pad_base = res.start + node_base;

	uc_priv->gpio_count = PAD_INDEX_MAX;
	priv->name[0] = 'G';
	uc_priv->bank_name = priv->name;

	return 0;
}

static const struct udevice_id sfax8_gpio_ids[] = {
	{ .compatible = "siflower,sfax8-gpio" },
	{ }
};

U_BOOT_DRIVER(gpio_sfax8) = {
	.name		= "gpio_sfax8",
	.id		= UCLASS_GPIO,
	.of_match	= sfax8_gpio_ids,
	.ops		= &gpio_sfax8_ops,
	.probe		= sfax8_gpio_probe,
	.priv_auto_alloc_size	= sizeof(struct sfax8_gpio_priv),
};
