/*
 *	A18 not use IR device on IP6103, and this driver is not full testing,
 *	add this driver just for future to use.
 *
 */

#include <linux/err.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/regmap.h>
#include <linux/input.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <misc/sfax8.h>

#define PMU_ONOFF_TIME		0x02
#define ONOFF_TIME_V		0x83
#define ONOFF_TIME_MASK		0xc3
#define PMU_INT				0x85
#define INT_V				0x11
#define INT_MASK			0x13
#define PMU_ONOFF_MODE		0x60
#define ONOFF_MODE_V		0x06
#define ONOFF_MODE_MASK		0x06
#define PMU_ONOFF_INT		0x62
#define ONOFF_LONG_INT_V	0x04
#define ONOFF_SHORT_INT_V	0x02
#define ONOFF_INT_MASK		0x06
#define PMU_RESET_EN		0x01
#define RESET_EN_MASK		0x80
#define RESET_DISABLE		0x00
#define LED_ON				0
#define LED_OFF				1


static struct sfax8_data *data;
static int gpio;

/*
* this function get interrupt type and clear the interrupt flag, there are two interrupt
* types: long press interrupt and short press interrupt
* @return LONG_PRESS_INT
*		  SHORT_PRESS_INT
*		  UNKNOW_INT get interrupt type failed.
*/
static interrupt_type sfax8_ip6103_recovery_clear_interrupt(struct sfax8 *sfax8)
{
	int ret = 0, value = 0;
	interrupt_type type = UNKNOW_INT;
	int tmp = 0;
	ret = regmap_read(sfax8->regmap, PMU_ONOFF_INT, &value);
	if(ret){
		printk(KERN_ERR "Read reg fail!\n");
		return UNKNOW_INT;
	}

	if( value & ONOFF_INT_MASK){
		if(value & ONOFF_LONG_INT_V){
			type = LONG_PRESS_INT;
			tmp = ONOFF_LONG_INT_V;
		}

		if(value & ONOFF_SHORT_INT_V){
			if(type != LONG_PRESS_INT)
				type = SHORT_PRESS_INT;
			tmp |= ONOFF_SHORT_INT_V;
		}
		ret = regmap_write(sfax8->regmap, PMU_ONOFF_INT, tmp);
		if (ret){
			printk(KERN_ERR "Clear reset button interrupt error\n");
			return UNKNOW_INT;
		}else{
			return type;
		}
	}
	return UNKNOW_INT;
}

static int sfax8_recovery_reg_init(struct sfax8 * sfax8)
{
	int ret = 0;
	ret |= regmap_update_bits(sfax8->regmap, PMU_ONOFF_TIME, ONOFF_TIME_MASK, ONOFF_TIME_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_INT, INT_MASK, INT_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_ONOFF_MODE, ONOFF_MODE_MASK, ONOFF_MODE_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_RESET_EN, RESET_EN_MASK, RESET_DISABLE);

	if (ret){
		printk(KERN_ERR "Configure pmu reset button, error %d\n", ret);
	}
	return ret;
}

static int sfax8_ip6103_recovery_init(struct sfax8_data *u_data, struct sfax8 *sfax8)
{
		if(sfax8_recovery_reg_init(sfax8))
			return -EINVAL;
		else{
			u_data->irq = data->irq;
			u_data->dev = data->dev;
			return 0;
		}
};

static int sfax8_ip6103_set_interrupt_mask(bool mask, struct sfax8 * sfax8)
{
	int ret = 0;
	char tmp;
	if(mask)
		tmp = 0;
	else{
		tmp = ONOFF_MODE_V;
		//if we want to clear the reset_button interrupt mask,
		// do clear interupt status before
		sfax8_ip6103_recovery_clear_interrupt(sfax8);
	}
	ret = regmap_update_bits(sfax8->regmap, PMU_ONOFF_MODE, ONOFF_MODE_MASK, tmp);
	return ret;

}

int sfax8_ip6103_rb_probe(struct platform_device *pdev)
{
	struct device_node *np;
	int ret = 0;
	int irq;
	u16 rb_gpio, led_gpio;
	struct sfax8 *parent_data;
	struct rb_ops *ops;

	printk(KERN_DEBUG "Start reset button probe!\n");

	data = devm_kzalloc(&pdev->dev, sizeof(struct sfax8_data), GFP_KERNEL);
	if (!data){
		printk(KERN_ERR "Alloc sfax8 recovery data error\n");
		ret = -ENOMEM;
		goto err;
	}
	data->dev = &pdev->dev;


	ops = devm_kzalloc(&pdev->dev, sizeof(struct rb_ops), GFP_KERNEL);
	if(!ops){
		ret = -ENOMEM;
		goto err;
	}

	np = of_get_child_by_name(pdev->dev.parent->of_node, "reset-button");
	pdev->dev.of_node = np;

	rb_gpio = of_get_named_gpio(pdev->dev.of_node, "rb-gpio", 0);
	if (!gpio_is_valid(rb_gpio)){
		printk( "Get reset gpio fail!\n");
		ret = -1;
		goto err;
	}
	ret = devm_gpio_request(&pdev->dev, rb_gpio, "reset button gpio");
	if (ret){
		printk("Request gpio fail! %d\n", ret);
		goto err;
	}

	ret = gpio_direction_input(rb_gpio);
	if (ret){
		printk("Set gpio input fail!\n");
		goto err;
	}

	irq = gpio_to_irq(rb_gpio);
	if (irq <= 0){
		printk("GPIO to irq fail!\n");
		ret = -1;
		data->irq = -1;
		goto err;
	}
	data->irq = irq;

	parent_data = dev_get_drvdata(pdev->dev.parent);

	regcache_cache_bypass(parent_data->regmap, true);

	ops->init = sfax8_ip6103_recovery_init;
	ops->clear_interrupt = sfax8_ip6103_recovery_clear_interrupt;
	ops->set_interrupt_mask = sfax8_ip6103_set_interrupt_mask;
	dev_set_drvdata(&pdev->dev, ops);


err:
	return ret;

}
EXPORT_SYMBOL(sfax8_ip6103_rb_probe);
