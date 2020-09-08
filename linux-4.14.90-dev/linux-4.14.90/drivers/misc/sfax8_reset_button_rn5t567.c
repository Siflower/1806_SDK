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

#define PMU_ONOFF_TIME		0x10
#define ONOFF_TIME_V		0x3d
#define ONOFF_TIME_MASK		0xff

#define PMU_PORTION_INT		0x12
#define INT_PORTION_V		0x11
#define INT_PORTION_MASK	0x7f

#define PMU_INT_TYPE		0x9c
#define INT_TYPE_V			0x1
#define INT_TYPE_MASK		0x1

#define PMU_INT				0x9d
#define INT_V				0x1
#define INT_MASK			0x13

#define PMU_ONOFF_MODE		0x60
#define ONOFF_MODE_V		0x04
#define ONOFF_MODE_MASK		0x04

#define PMU_ONOFF_INT		0x13
#define ONOFF_INT_V			0x0
#define ONOFF_INT_MASK_PWON	0x01
#define ONOFF_INT_MASK_PWOF	0x10


#define LED_ON				0
#define LED_OFF				1
static int gpio;

struct sfax8_data *data;

/* Rn5t567 will generate 2 interrupts: press and release */
int button_mask = -1;

static interrupt_type sfax8_rn5t567_recovery_clear_interrupt(struct sfax8 *sfax8)
{
	int ret = 0, value = 0;

	ret = regmap_read(sfax8->regmap, PMU_ONOFF_INT, &value);
	if (ret) {
		printk(KERN_DEBUG "Read reg fail!\n");
		goto out;
	}

	if (value & ONOFF_INT_MASK_PWON) {
		ret = regmap_write(sfax8->regmap, PMU_ONOFF_INT, ONOFF_INT_V);
		if (ret) {
			printk(KERN_DEBUG "Clear reset button irq error\n");
			goto out;
		}
		button_mask += 1;
		if (button_mask % 2)
			goto out;
		else
			return SHORT_PRESS_INT;
	} else if (value & ONOFF_INT_MASK_PWOF) {
		ret = regmap_write(sfax8->regmap, PMU_ONOFF_INT, ONOFF_INT_V);
		if (ret) {
			printk(KERN_DEBUG "Clear reset button irq error\n");
			goto out;
		} else {
			return LONG_PRESS_INT;
		}
	}
out:
	return UNKNOW_INT;

}

static int sfax8_recovery_reg_init(struct sfax8 *sfax8)
{
	int ret = 0;
	ret |= regmap_update_bits(sfax8->regmap, PMU_ONOFF_TIME, ONOFF_TIME_MASK, ONOFF_TIME_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_INT, INT_MASK, INT_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_INT_TYPE, INT_TYPE_MASK, INT_TYPE_V);
	ret |= regmap_update_bits(sfax8->regmap, PMU_PORTION_INT, INT_PORTION_MASK, INT_PORTION_V);
	if (ret)
		printk(KERN_DEBUG "Configure pmu reset button, error %d\n", ret);
	return ret;
}

static int sfax8_rn5t567_recovery_init(struct sfax8_data *u_data, struct sfax8 *sfax8)
{
		if(sfax8_recovery_reg_init(sfax8))
			return -EINVAL;
		else{
			u_data->irq = data->irq;
			u_data->dev = data->dev;
			return 0;
		}
};

/*
* if the parameter is true, the led will be set on,
* else set off
*/
static int sfax8_rn5t567_recovery_led_on(bool on)
{
	int ret;
	if(gpio < 0){
		printk(KERN_ERR "%s: bad gpio number.\n", __func__);
		return -EINVAL;
	}
	if(gpio != 0){
		ret = gpio_direction_output(gpio, LED_OFF);
		if (ret){
			printk(KERN_ERR "Set led gpio output fail!\n");
			return -EINVAL;
		}
		if(on)
			gpio_set_value(gpio, LED_ON);
		else
			gpio_set_value(gpio, LED_OFF);
	}else{
		u16 eth_led = 0;
		for(eth_led = 55 ; eth_led <= 59 ; eth_led++){
			ret = gpio_direction_output(eth_led, LED_OFF);
			if (ret){
				printk(KERN_ERR "Set led gpio output fail!\n");
				return -EINVAL;
			}
			if(on)
				gpio_set_value(eth_led, LED_ON);
			else
				gpio_set_value(eth_led, LED_OFF);
		}
	}
	return 0;
}
/*
* if the parameter is true, mask the recovery interrupt; else
* not mask the interrupt.
*/
static int sfax8_rn5t567_recovery_interrupt_mask(bool mask, struct sfax8 *sfax8)
{

	int ret = 0;
	//if we want to clear the reset_button interrupt mask,
	//do clear interupt status before
	if(!mask)
		sfax8_rn5t567_recovery_clear_interrupt(sfax8);
	ret = regmap_update_bits(sfax8->regmap, PMU_INT, INT_MASK, !mask);
	return ret;

}

int sfax8_rn5t567_rb_probe(struct platform_device *pdev)
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
		printk( "Alloc sfax8 recovery data error\n");
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

	led_gpio = of_get_named_gpio(pdev->dev.of_node, "led-gpio", 0);
	if (!gpio_is_valid(led_gpio)){
		//printk( "set eth led!\n");
		led_gpio = 0;
	}
	gpio = led_gpio;

	if(led_gpio != 0){
		ret = devm_gpio_request(&pdev->dev, led_gpio, "reset led gpio");
		if (ret){
			printk(KERN_ERR "Request reset led gpio fail! %d\n", ret);
			goto err;
		}
	}else{
		u16 eth_led = 0;
		for(eth_led = 55 ; eth_led <= 59 ; eth_led++){
			ret = devm_gpio_request(&pdev->dev, eth_led, "reset led gpio");
			if (ret){
				printk(KERN_ERR "Request reset led gpio %d fail! %d\n", (int)eth_led,ret);
				goto err;
			}
		}
	}

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

	ops->init = sfax8_rn5t567_recovery_init;
	ops->clear_interrupt = sfax8_rn5t567_recovery_clear_interrupt;
	ops->set_led_onoff = sfax8_rn5t567_recovery_led_on;
	ops->set_interrupt_mask = sfax8_rn5t567_recovery_interrupt_mask;
	dev_set_drvdata(&pdev->dev, ops);

err:
	return ret;

}
EXPORT_SYMBOL(sfax8_rn5t567_rb_probe);
