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
#include <linux/gpio_keys.h>

#define DRV_NAME "reset-button"
#define DRV_VERSION "0.0.1"

#define FAT 1
#define FIT 0

static struct workqueue_struct *led_workqueue;

struct sfax8_rb {
	int irq;
	struct input_dev *idev;
	struct work_struct work;
	struct delayed_work	detect;
};

struct sfax8_rb *rb;

//used to judge the mode now is FAT or FIT
static int sfax8_mode_judge(void)
{
	int ret = 0;

#if defined(CONFIG_DT_SF16A18_FULLMASK_86V) || defined(CONFIG_DT_SF16A18_FULLMASK_86V_C2)
	ret = sfax8_mode_value();
#endif

	return ret?FAT:FIT;
}

#ifdef CONFIG_SF_KERNEL_LITE
/* Call /etc/rc.button/xxx. */
static void sfax8_btn_ops(struct sfax8_rb *sfrb, int btn)
{
	input_report_key(sfrb->idev, btn, 1);
	input_sync(sfrb->idev);

	input_report_key(sfrb->idev, btn, 0);
	input_sync(sfrb->idev);

	return;
}
#endif

//to check if ac and ap is connected
static int sfax8_apac_connect(void)
{
	//TBD
	return 0;
}

#ifdef CONFIG_SF_KERNEL_LITE
//reboot and get lastest config from AC.
static void sfax8_update_reboot(struct sfax8_rb *sfrb)
{
	sfax8_btn_ops(sfrb, KEY_RESTART);
	return;
}
#endif

//openwrt request that recover is time more than 5s between button press and release
static void sfax8_recover_reboot(struct sfax8_rb *sfrb)
{
	printk("Recover after 7s!\n");
	input_report_key(sfrb->idev, KEY_RESTART,1);
	input_sync(sfrb->idev);

	msleep(6 * 1000);

	input_report_key(sfrb->idev, KEY_RESTART,0);
	input_sync(sfrb->idev);
}

#ifdef CONFIG_SF_NETWORK_WPS
//wps
static void sfax8_wps_connect(struct sfax8_rb *sfrb)
{
	input_set_capability(sfrb->idev, EV_KEY, KEY_WPS_BUTTON);
	printk("Linking your device through wps!\n");
	input_report_key(sfrb->idev, KEY_WPS_BUTTON, 1);
	input_sync(sfrb->idev);
	input_report_key(sfrb->idev, KEY_WPS_BUTTON, 0);
	input_sync(sfrb->idev);
	input_set_capability(sfrb->idev, EV_KEY, KEY_RESTART);
	return;
}
#endif

static void sfax8_rb_work(struct work_struct *work){
	struct sfax8_rb *sfrb = container_of(work, struct sfax8_rb, work);

	u32 ret = 0;

	ret = sfax8_recovery_clear_interrupt();
	switch (ret)
	{
		case SHORT_PRESS_INT:
#if defined(CONFIG_DT_SF16A18_FULLMASK_86V) || defined(CONFIG_DT_SF16A18_FULLMASK_86V_C2)
			sfax8_update_reboot(sfrb);
#else
#ifdef CONFIG_SF_NETWORK_WPS
			sfax8_wps_connect(sfrb);
#endif
#endif
			break;
		case LONG_PRESS_INT:
			sfax8_recover_reboot(sfrb);
			break;
		default :
			break;
	}
	enable_irq(sfrb->irq);
	return;
}

static irqreturn_t sfax8_rb_irq_handler(int irq, void *_sfax8_rb)
{
	struct sfax8_rb *sfrb = _sfax8_rb;

	disable_irq_nosync(sfrb->irq);

	schedule_work(&sfrb->work);

	return IRQ_HANDLED;
}

static int led_schedule_delayed_work(struct delayed_work *work,
				     unsigned long delay)
{
	return queue_delayed_work(led_workqueue, work, delay);
}

void led_flicker_rescan(struct work_struct *work)
{
	unsigned long delay = 200;
	struct sfax8_rb *sfrb =
		container_of(work, struct sfax8_rb, detect.work);

	if(sfax8_mode_judge() == FIT && sfax8_apac_connect())
	{
		//sfax8_led_flicker(1);
		goto out;
	}

 out:
	led_schedule_delayed_work(&sfrb->detect, delay);
}

static int __init sfax8_rb_init(void)
{
	struct input_dev *input_dev;
	struct sfax8_rb *sfrb;
	struct sfax8_data data;
	int ret = 0;

	led_workqueue = alloc_ordered_workqueue("kledd", 0);
	if (!led_workqueue)
		return -ENOMEM;

	ret = sfax8_recovery_init(&data);
	if (ret){
		pr_err("Configure pmu reset button, error %d\n", ret);
		goto err;
	}
	input_dev = devm_input_allocate_device(data.dev);
	if (!input_dev){
		pr_err("Input allocate device error\n");
		ret = -ENOMEM;
		goto err;
	}

	sfrb = devm_kzalloc(data.dev, sizeof(struct sfax8_rb), GFP_KERNEL);
	if (!sfrb){
		pr_err("Alloc sfrb error\n");
		ret = -ENOMEM;
		goto err;
	}
	rb = sfrb;

	input_dev->name = "sfax8";
	input_dev->phys = "sfax8/input1";
	input_dev->id.bustype = BUS_I2C;
	input_dev->dev.parent = data.dev;
	input_set_capability(input_dev, EV_KEY, KEY_RESTART);
	input_set_capability(input_dev, EV_KEY, BTN_0);
	input_set_capability(input_dev, EV_KEY, BTN_1);

	sfrb->idev = input_dev;
	sfrb->irq = data.irq;
	INIT_WORK(&sfrb->work, (void *)(void *)sfax8_rb_work);

	ret = request_irq(sfrb->irq,sfax8_rb_irq_handler,IRQF_TRIGGER_HIGH, "reset button", rb);
	if (ret){
		pr_err("Request irq error %d\n", ret);
		goto err;
	}

	ret = input_register_device(input_dev);
	if(ret){
		pr_err("Register input device sfax8 error %d\n", ret);
		goto err;
	}

	//INIT_DELAYED_WORK(&sfrb->detect, led_flicker_rescan);
	//led_schedule_delayed_work(&sfrb->detect, 1000);

err:
	return ret;

}

static void __exit sfax8_rb_exit(void){
	free_irq(rb->irq, rb);
	input_unregister_device(rb->idev);
	//destroy_workqueue(led_workqueue);
}

module_init(sfax8_rb_init);
module_exit(sfax8_rb_exit);

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=sfax8_reset_button";

MODULE_DESCRIPTION(DRV_NAME);
MODULE_VERSION(DRV_VERSION);
MODULE_AUTHOR("Xijun Guo <xijun.guo@siflower.org>");
MODULE_LICENSE("GPL");
