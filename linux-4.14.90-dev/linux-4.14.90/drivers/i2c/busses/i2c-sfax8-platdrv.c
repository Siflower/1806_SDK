/*
 * Siflower I2C adapter driver (master only).
 *
 * Based on the TI DAVINCI I2C adapter driver.
 *
 *
 * ----------------------------------------------------------------------------
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
 * ----------------------------------------------------------------------------
 *
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/dmi.h>
#include <linux/i2c.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/acpi.h>
#include "i2c-sfax8-core.h"
#include <linux/uaccess.h>

extern void set_i2c_clk_gate(u32 base, bool on);
#ifdef CONFIG_DEBUG_FS
#include <linux/string.h>
#include <linux/debugfs.h>
static int sfax8_i2c_debug_show(struct seq_file *m, void *v)
{
	seq_printf(m,"i2c debug interface\n");
	return 0;
}

static int sfax8_i2c_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, sfax8_i2c_debug_show, NULL);
}

static ssize_t sfax8_i2c_debug_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	int ret = 0;
	char *buff;

	if(*ppos > 0)
		return 0;

	buff = kmalloc(sizeof(char) * count, GFP_KERNEL);
	if(!buff)
		return -ENOMEM;

	ret = snprintf(buff, count, "Usage: echo i2c_ctl_num r(read)|w(write) addr reg_addr [value(if write)] > i2c_debug\n");
	if(copy_to_user(buf, buff, ret))
		ret = -EFAULT;

	*ppos += ret;
	kfree(buff);
	return ret;
}

static ssize_t sfax8_i2c_debug_write(struct file *file, const char __user *user_buf,
				size_t ncount, loff_t *ppos)
{
		char tmp[128];
		char s_data[5][8] = { 0 };
		unsigned char ctl, rw, addr, reg, value;
		size_t buf_size;
		char sp = ' ';
		int ret, count = 0;
		struct i2c_adapter *adapter;
		struct i2c_msg *msgs;
		struct i2c_msg i2c_msg[2];
		char buff[2];
		int o_index = 0, n_index = 0;

		buf_size = 128 <= ncount ? 128: ncount;

		if(copy_from_user((char *)tmp, user_buf, buf_size)){
			ret = -EFAULT;
			goto err;
		}

		if( *(tmp + buf_size - 1) == '\n')
			*(tmp + buf_size - 1) = '\0';

		while(strchr(tmp + n_index, sp)){
			//continues space
			n_index = buf_size - strlen(strchr(tmp + o_index, sp));
			if(n_index == o_index){
				n_index++;
				o_index++;
				continue;
			}

			if(count > 4){
				ret = -EINVAL;
				goto err;
			}

			if(n_index - o_index > 8){
				ret = -EINVAL;
				goto err;
			}
			strncpy(s_data[count], tmp + o_index, n_index - o_index - 1);
			o_index = n_index;
			count++;
		}
		if(n_index != buf_size - 1){
			if(count > 4 ){
				ret = -EINVAL;
				return ret;
			}else{
				strncpy(s_data[count], tmp + n_index, buf_size - n_index);
				count++;
			}

		}

		if(count < 3 || count > 5){
			ret = -EINVAL;
			goto err;
		}

		ctl = (unsigned char)simple_strtoull(s_data[0], NULL, 0);
		if(ctl > 3){
			ret = -EINVAL;
			goto err;
		}
		rw = 2;
		if((!strcmp(s_data[1], "r")) || (!strcmp(s_data[1], "read")) )
			rw = 1;
		if((!strcmp(s_data[1], "w")) || (!strcmp(s_data[1], "write")) )
			rw = 0;

		if(rw == 2){
			pr_err("Usage error, rw parameter is invalid.\n");
			ret = -EINVAL;
			goto err;
		}
		//get addr
		addr = (unsigned char)simple_strtoull(s_data[2], NULL, 0);

		//get reg
		reg = (unsigned char)simple_strtoull(s_data[3], NULL, 0);


		adapter = i2c_get_adapter(ctl);
		if(!adapter){
			pr_debug("Can't get i2c adapter!\n");
			ret = -EBADF;
			goto err;
		}

		msgs = i2c_msg;

		if(!rw){
			//get value
			value = (unsigned char)simple_strtoull(s_data[4], NULL, 0);
			msgs[0].buf = buff;
			msgs[0].buf[0] = reg;
			msgs[0].buf[1] = value;
			msgs[0].len = 2;
			msgs[0].addr = addr;
			msgs[0].flags = 0;
		}else {
			msgs[0].addr = msgs[1].addr = addr;
			msgs[0].len = msgs[1].len = 1;
			msgs[0].flags = 0;
			msgs[0].buf = &reg;
			msgs[1].flags = 1;
			msgs[1].buf = buff;
		}

		ret = i2c_transfer(adapter, msgs, 1 + rw);

		if(ret > 0){
			if(rw)
				printk("read value:0x%x\n", msgs[1].buf[0]);
			ret = ncount;
		}else{
			printk("Transmit error\n");
		}

err:
		return ret;
}

static struct file_operations i2c_ops = {
	.owner = THIS_MODULE,
	.open  = sfax8_i2c_debug_open,
	.read  = sfax8_i2c_debug_read,
	.write  = sfax8_i2c_debug_write,
	.llseek  = default_llseek,
	.release = single_release,
};
#endif
static struct i2c_algorithm i2c_sf_algo = {
	.master_xfer	= i2c_sf_xfer,
	.functionality	= i2c_sf_func,
};
static u32 i2c_sf_get_clk_rate_khz(struct sf_i2c_dev *dev)
{
	return clk_get_rate(dev->clk)/1000;
}

static int sf_i2c_probe(struct platform_device *pdev)
{
	struct sf_i2c_dev *dev;
	struct i2c_adapter *adap;
	struct resource *mem;
	struct sf_i2c_platform_data *pdata;
	int irq, r;
	u32 clk_freq = 0;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "no irq resource?\n");
		return irq; /* -ENXIO */
	}
	dev = devm_kzalloc(&pdev->dev, sizeof(struct sf_i2c_dev), GFP_KERNEL);
	if (!dev)
		return -ENOMEM;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	dev->base = devm_ioremap_resource(&pdev->dev, mem);
	if (IS_ERR(dev->base))
		return PTR_ERR(dev->base);

	init_completion(&dev->cmd_complete);
	mutex_init(&dev->lock);
	dev->dev = &pdev->dev;
	dev->irq = irq;
	platform_set_drvdata(pdev, dev);

	/* fast mode by default because of legacy reasons */
	clk_freq = 400000;

	if (pdev->dev.of_node) {
		of_property_read_u32(pdev->dev.of_node, "clock-frequency",
				     &clk_freq);

		/* Only standard mode at 100kHz and fast mode at 400kHz
		 * are supported.
		 */
		if (clk_freq != 100000 && clk_freq != 400000 && clk_freq != 3400000) {
			dev_err(&pdev->dev, "Only 100kHz, 400khz and 3.4 mhz supported");
			return -EINVAL;
		}
	} else {
		pdata = dev_get_platdata(&pdev->dev);
		if (pdata)
			clk_freq = pdata->i2c_scl_freq;
	}

	dev->functionality =
		I2C_FUNC_I2C |
		I2C_FUNC_10BIT_ADDR |
		I2C_FUNC_SMBUS_BYTE |
		I2C_FUNC_SMBUS_BYTE_DATA |
		I2C_FUNC_SMBUS_WORD_DATA |
		I2C_FUNC_SMBUS_I2C_BLOCK;
	if (clk_freq == 100000)
		dev->master_cfg =  SF_IC_CON_MASTER | SF_IC_CON_SLAVE_DISABLE |
			SF_IC_CON_RESTART_EN | SF_IC_CON_SPEED_STD;
	else if(clk_freq == 400000)
		dev->master_cfg =  SF_IC_CON_MASTER | SF_IC_CON_SLAVE_DISABLE |
			SF_IC_CON_RESTART_EN | SF_IC_CON_SPEED_FAST;
	else if(clk_freq == 3400000)
		dev->master_cfg =  SF_IC_CON_MASTER | SF_IC_CON_SLAVE_DISABLE |
			SF_IC_CON_RESTART_EN | SF_IC_CON_SPEED_HIGH;

	printk(KERN_DEBUG "i2c clk freq is %d\n", clk_freq);
	dev->clk = devm_clk_get(&pdev->dev, NULL);
	dev->get_clk_rate_khz = i2c_sf_get_clk_rate_khz;
	if (IS_ERR(dev->clk))
		return PTR_ERR(dev->clk);
	clk_prepare_enable(dev->clk);
	if (!dev->tx_fifo_depth) {
		dev->tx_fifo_depth = 16;
		dev->rx_fifo_depth = 16;
		dev->adapter.nr = pdev->id;
	}
	set_i2c_clk_gate((u32)dev->base, true);
	r = i2c_sf_init(dev);
	set_i2c_clk_gate((u32)dev->base, false);
	if (r)
		return r;

	i2c_sf_disable_int(dev);
	r = devm_request_irq(&pdev->dev, dev->irq, i2c_sf_isr, IRQF_SHARED,
			pdev->name, dev);
	if (r) {
		dev_err(&pdev->dev, "failure requesting irq %i\n", dev->irq);
		return r;
	}
	adap = &dev->adapter;
	i2c_set_adapdata(adap, dev);
	adap->owner = THIS_MODULE;
	adap->class = I2C_CLASS_DEPRECATED;
	strlcpy(adap->name, "sfax8-i2c-adapter",
			sizeof(adap->name));
	adap->algo = &i2c_sf_algo;
	adap->dev.parent = &pdev->dev;
	adap->dev.of_node = pdev->dev.of_node;
	r = i2c_add_numbered_adapter(adap);
	if (r) {
		dev_err(&pdev->dev, "failure adding adapter\n");
		return r;
	}
	pm_runtime_set_autosuspend_delay(&pdev->dev, 1000);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_set_active(&pdev->dev);
	pm_runtime_enable(&pdev->dev);
	#ifdef CONFIG_DEBUG_FS
	debugfs_create_file("i2c_debug", 0777, NULL, NULL, &i2c_ops);
	#endif

	return 0;
}

static int sf_i2c_remove(struct platform_device *pdev)
{
	struct sf_i2c_dev *dev = platform_get_drvdata(pdev);

	pm_runtime_get_sync(&pdev->dev);

	i2c_del_adapter(&dev->adapter);

	i2c_sf_disable(dev);

	pm_runtime_put(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id sf_i2c_of_match[] = {
	{ .compatible = "siflower,sfax8-i2c", },
	{},
};
MODULE_DEVICE_TABLE(of, sf_i2c_of_match);
#endif

#ifdef CONFIG_PM
static int sf_i2c_suspend(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sf_i2c_dev *i_dev = platform_get_drvdata(pdev);

	i2c_sf_disable(i_dev);
	clk_disable_unprepare(i_dev->clk);

	return 0;
}

static int sf_i2c_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct sf_i2c_dev *i_dev = platform_get_drvdata(pdev);

	clk_prepare_enable(i_dev->clk);
	i2c_sf_init(i_dev);

	return 0;
}
#endif

static UNIVERSAL_DEV_PM_OPS(sf_i2c_dev_pm_ops, sf_i2c_suspend,
			    sf_i2c_resume, NULL);

/* work with hotplug and coldplug */
MODULE_ALIAS("platform:i2c_siflower");

static struct platform_driver sf_i2c_driver = {
	.probe = sf_i2c_probe,
	.remove = sf_i2c_remove,
	.driver		= {
		.name	= "i2c_siflower",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(sf_i2c_of_match),
		/*.acpi_match_table = ACPI_PTR(sf_i2c_acpi_match),*/
		.pm	= &sf_i2c_dev_pm_ops,
	},
};

static int __init sf_i2c_init_driver(void)
{
	if(release_reset(SF_IIC_SOFT_RESET))
		return -EFAULT;
	return platform_driver_register(&sf_i2c_driver);
}
subsys_initcall(sf_i2c_init_driver);

static void __exit sf_i2c_exit_driver(void)
{
	platform_driver_unregister(&sf_i2c_driver);
	if(hold_reset(SF_IIC_SOFT_RESET))
		return ;
}
module_exit(sf_i2c_exit_driver);

MODULE_DESCRIPTION("Siflower SFAX8 I2C bus adapter");
MODULE_LICENSE("GPL");
