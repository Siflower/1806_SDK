/*
 * SIFLOWER sfax8 USB PHY driver
 *
 * Copyright (C) 2015 Google, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <sf16a18.h>

#ifdef CONFIG_SF16A18_MPW0
#define USB_SLEEPM0		0xC844
#define USB_DEBUG_EN_OFFSET	0xC88C
#else
#define USB_SLEEPM0		0xC044
#define USB_DEBUG_EN_OFFSET	0xC08C
#endif

struct sfax8_usb_phy {
	struct device *dev;
	struct clk *phy_clk;
	struct clk *bus_clk;
};

static int sfax8_usb_phy_power_on(struct phy *phy)
{
	struct device_node *node;
	struct regmap *regmap_base;
	struct sfax8_usb_phy *p_phy = phy_get_drvdata(phy);
	int ret;

	printk(KERN_DEBUG "%s:\n", __func__);

	node = of_parse_phandle(phy->dev.of_node, "usbgrf", 0);
	regmap_base = syscon_node_to_regmap(node);

	if(regmap_base) {
		regmap_write(regmap_base, USB_DEBUG_EN_OFFSET, 1);
		regmap_write(regmap_base, USB_SLEEPM0, 1);
	} else {
		dev_err(p_phy->dev, "Failed to get grf regmap base\n");
		return -EINVAL;
	}

	ret = clk_prepare_enable(p_phy->phy_clk);
	if (ret < 0)
		dev_err(p_phy->dev, "Failed to enable PHY clock: %d\n", ret);

	return ret;

}

static int sfax8_usb_phy_power_off(struct phy *phy)
{
	struct sfax8_usb_phy *p_phy = phy_get_drvdata(phy);

	clk_disable_unprepare(p_phy->phy_clk);

	return 0;
}

static const struct phy_ops sfax8_usb_phy_ops = {
	.power_on = sfax8_usb_phy_power_on,
	.power_off = sfax8_usb_phy_power_off,
	.owner = THIS_MODULE,
};

static int sfax8_usb_phy_probe(struct platform_device *pdev)
{
	struct sfax8_usb_phy *p_phy;
	struct phy_provider *provider;
	struct phy *phy;
	int ret;

	if(release_reset(SF_USB_SOFT_RESET))
		return -EFAULT;

	set_module_clk_gate(SF_USB_SOFT_RESET, 0xD, 0);

	p_phy = devm_kzalloc(&pdev->dev, sizeof(*p_phy), GFP_KERNEL);
	if (!p_phy)
		return -ENOMEM;
	p_phy->dev = &pdev->dev;
	platform_set_drvdata(pdev, p_phy);

	p_phy->phy_clk = devm_clk_get(p_phy->dev, "usb_phy_clk");
	if (IS_ERR(p_phy->phy_clk)) {
		dev_err(p_phy->dev, "Failed to get usb_phy clock: %ld\n",
			PTR_ERR(p_phy->phy_clk));
		return PTR_ERR(p_phy->phy_clk);
	}

	p_phy->bus_clk = devm_clk_get(p_phy->dev, "bus3_clk");
	if (IS_ERR(p_phy->bus_clk)) {
		dev_err(p_phy->dev, "Failed to get usb bus clock: %ld\n",
			PTR_ERR(p_phy->bus_clk));
		return PTR_ERR(p_phy->bus_clk);
	}

	ret = clk_prepare_enable(p_phy->bus_clk);
	if(ret < 0) {
		dev_err(p_phy->dev, "Failed to enable usb bus clock: %d\n", ret);
		return ret;
	}

	phy = devm_phy_create(p_phy->dev, NULL, &sfax8_usb_phy_ops);
	if (IS_ERR(phy)) {
		dev_err(p_phy->dev, "Failed to create PHY: %ld\n",
			PTR_ERR(phy));
		return PTR_ERR(phy);
	}
	phy_set_drvdata(phy, p_phy);

	provider = devm_of_phy_provider_register(p_phy->dev,
						 of_phy_simple_xlate);
	if (IS_ERR(provider)) {
		dev_err(p_phy->dev, "Failed to register PHY provider: %ld\n",
			PTR_ERR(provider));
		return PTR_ERR(provider);
	}

	return 0;
}

static int sfax8_usb_phy_remove(struct platform_device *pdev)
{
	struct sfax8_usb_phy *p_phy;

	p_phy = (struct sfax8_usb_phy *)platform_get_drvdata(pdev);

	if(hold_reset(SF_USB_SOFT_RESET))
		return -EFAULT;

	set_module_clk_gate(SF_USB_SOFT_RESET, 0x0, 0);

	clk_disable_unprepare(p_phy->bus_clk);

	return 0;
}

static const struct of_device_id sfax8_usb_phy_of_match[] = {
	{ .compatible = "siflower,sfax8-usb-phy", },
	{ },
};
MODULE_DEVICE_TABLE(of, sfax8_usb_phy_of_match);

static struct platform_driver sfax8_usb_phy_driver = {
	.probe		= sfax8_usb_phy_probe,
	.remove 	= sfax8_usb_phy_remove,
	.driver		= {
		.name	= "sfax8-usb-phy",
		.of_match_table = sfax8_usb_phy_of_match,
	},
};
module_platform_driver(sfax8_usb_phy_driver);

MODULE_AUTHOR("Nevermore");
MODULE_DESCRIPTION("SIFLOWER sfax8 USB2.0 PHY driver");
MODULE_LICENSE("GPL");
