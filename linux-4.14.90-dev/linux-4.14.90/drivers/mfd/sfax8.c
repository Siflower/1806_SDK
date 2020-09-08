/*
 * Core driver for ams SF16ax8 PMICs
 *
 * Copyright (C) 2013 AMS AG
 * Copyright (c) 2013, NVIDIA Corporation. All rights reserved.
 * Copyright (C) 2016 SIFLOWER
 *
 * Author: Xijun Guo  <xijun.guo@siflower.com.cn>
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
 */

#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mfd/sfax8.h>
#include <linux/of.h>
#include <linux/slab.h>

/*
* 0x30 : the address of pmu ip6103.
* 0x36 : the address of pmu rn5t567.
*/
static struct sfax8_pmu_info pmu_infos[] = {
	{
		.addr = 0x30,
		.name = "IP6103",
		.cell = sfax8_ip6103_devs,
		.cell_len = ARRAY_SIZE(sfax8_ip6103_devs),
		.config = &sfax8_ip6103_regmap_config,
	},
	{
		.addr= 0x36,
		.name = "RN5T567",
		.cell = sfax8_rn5t567_devs,
		.cell_len = ARRAY_SIZE(sfax8_rn5t567_devs),
		.config = &sfax8_rn5t567_regmap_config,
	},
};

static int sfax8_i2c_of_probe(struct i2c_client *i2c,
			struct sfax8 *sfax8)
{
	struct device_node *np = i2c->dev.of_node;
	//struct irq_data *irq_data;

	if (!np) {
		dev_err(&i2c->dev, "Device Tree not found\n");
		return -EINVAL;
	}

/*	irq_data = irq_get_irq_data(i2c->irq);
	if (!irq_data) {
		dev_err(&i2c->dev, "Invalid IRQ: %d\n", i2c->irq);
		return -EINVAL;
	}

	sfax8->en_intern_int_pullup = of_property_read_bool(np,
					"ams,enable-internal-int-pullup");
	sfax8->en_intern_i2c_pullup = of_property_read_bool(np,
					"ams,enable-internal-i2c-pullup");
	sfax8->irq_flags = irqd_get_trigger_type(irq_data);
	dev_dbg(&i2c->dev, "IRQ flags are 0x%08lx\n", sfax8->irq_flags);*/
	return 0;
}

static int sfax8_i2c_probe(struct i2c_client *i2c,
			const struct i2c_device_id *id)
{
	struct sfax8 *sfax8;
	struct i2c_msg *msgs;
	int ret, i;
	int pmu_reg, pmu_addr = 0;
	pmu_type type;

	sfax8 = devm_kzalloc(&i2c->dev, sizeof(struct sfax8), GFP_KERNEL);
	if (!sfax8)
		return -ENOMEM;

	msgs = (struct i2c_msg *)kmalloc(2 * sizeof(struct i2c_msg), GFP_KERNEL);
	if(!msgs){
		printk("Malloc i2c msg failed.\n");
		ret = -ENOMEM;
		goto err_nomem1;
	}

	pmu_reg = PMU_REG;
	msgs[0].buf = (unsigned char *)&pmu_reg;
	msgs[0].len = msgs[1].len = 1;
	msgs[0].flags = 0;
	msgs[1].flags = 1;
	msgs[1].buf = (unsigned char *)kmalloc(sizeof(char), GFP_KERNEL);
	if(!msgs[1].buf){
		ret = -ENOMEM;
		goto err_nomem2;
	}

	for(i = 0; i < ARRAY_SIZE(pmu_infos); i++){
		msgs[0].addr = msgs[1].addr = pmu_infos[i].addr;
		ret = i2c_transfer(i2c->adapter, msgs, 2);
		if(ret < 0)
			continue;
		else{
			pmu_addr = pmu_infos[i].addr;
			i2c->addr = pmu_addr;
			type = i;
			break;
		}
	}
	kfree(msgs[1].buf);
	kfree(msgs);
	if(pmu_addr == 0){
		printk("No PMU detected!\n");
		return -ENXIO;
	}else{
		printk("PMU %s has detected!\n", pmu_infos[type].name);
	}

	sfax8->dev = &i2c->dev;
	sfax8->chip_irq = i2c->irq;
	sfax8->type = type;
	i2c_set_clientdata(i2c, sfax8);

	ret = sfax8_i2c_of_probe(i2c, sfax8);
	if (ret < 0)
		return ret;

	dev_dbg(sfax8->dev, "start regmap init\n");

	sfax8->regmap = devm_regmap_init_i2c(i2c, pmu_infos[type].config);
	if (IS_ERR(sfax8->regmap)) {
		ret = PTR_ERR(sfax8->regmap);
		dev_err(&i2c->dev, "regmap init failed: %d\n", ret);
		return ret;
	}
/*
	ret = sfax8_check_device_id(sfax8);
	if (ret < 0)
		return ret;

	irq_flags = sfax8->irq_flags | IRQF_ONESHOT;
	ret = regmap_add_irq_chip(sfax8->regmap, sfax8->chip_irq,
			irq_flags, -1, &sfax8_irq_chip,
			&sfax8->irq_data);
	if (ret < 0) {
		dev_err(sfax8->dev, "Failed to add regmap irq: %d\n", ret);
		return ret;
	}

	ret = sfax8_configure_pullups(sfax8);
	if (ret < 0)
		goto scrub;
*/
	dev_dbg(sfax8->dev, "start add devices\n");
	ret = mfd_add_devices(&i2c->dev, -1, pmu_infos[type].cell,
			pmu_infos[type].cell_len, NULL, 0,NULL);
//			regmap_irq_get_domain(sfax8->irq_data));
	if (ret) {
		dev_err(sfax8->dev, "Failed to add MFD devices: %d\n", ret);
		goto scrub;
	}

	dev_dbg(sfax8->dev, "SF16ax8 core driver initialized successfully\n");
	return 0;

err_nomem1:
	kfree(sfax8);
err_nomem2:
	kfree(msgs);
scrub:
//	regmap_del_irq_chip(sfax8->chip_irq, sfax8->irq_data);
	return ret;
}

static int sfax8_i2c_remove(struct i2c_client *i2c)
{
	struct sfax8 *sfax8 = i2c_get_clientdata(i2c);

	mfd_remove_devices(sfax8->dev);
//	regmap_del_irq_chip(sfax8->chip_irq, sfax8->irq_data);
	return 0;
}

static const struct of_device_id sfax8_of_match[] = {
	{ .compatible = "siflower, sfax8-pmu", },
	{},
};
MODULE_DEVICE_TABLE(of, sfax8_of_match);

static const struct i2c_device_id sfax8_i2c_id[] = {
	{ "sfax8", 0 },
	{},
};
MODULE_DEVICE_TABLE(i2c, sfax8_i2c_id);

static struct i2c_driver sfax8_i2c_driver = {
	.driver = {
		.name = "sfax8-pmu",
		.owner = THIS_MODULE,
		.of_match_table = sfax8_of_match,
	},
	.probe = sfax8_i2c_probe,
	.remove = sfax8_i2c_remove,
	.id_table = sfax8_i2c_id,
};

module_i2c_driver(sfax8_i2c_driver);

MODULE_DESCRIPTION("I2C support for SF16ax8 PMU");
MODULE_AUTHOR("Xijun Guo <xijun.guo@siflower.com.cn>");
MODULE_LICENSE("GPL");
