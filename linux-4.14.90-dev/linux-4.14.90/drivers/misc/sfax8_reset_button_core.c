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
#include <misc/sfax8.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/uaccess.h>

static struct platform_device *rb_pdev = NULL;
extern int sfax8_ip6103_rb_probe(struct platform_device *pdev);
extern int sfax8_rn5t567_rb_probe(struct platform_device *pdev);
#define NODE_NAME	"reset_button_mask"

static int (*rb_probe[])(struct platform_device *) = {
	sfax8_ip6103_rb_probe,
	sfax8_rn5t567_rb_probe,
};

int sfax8_recovery_init(struct sfax8_data *data)
{
	struct rb_ops *ops;
	struct sfax8 *sfax8;
	if (!rb_pdev)
		return -EINVAL;
	ops = dev_get_drvdata(&rb_pdev->dev);
	sfax8 = dev_get_drvdata(rb_pdev->dev.parent);
	if(!ops){
		pr_err("1111111111111111111\n");
		return -EINVAL;
	}
	if(ops->init){
		return ops->init(data, sfax8);
		pr_err("ops-init:%d \n",ops->init(data,sfax8));
	}
	else{
		pr_err("2222222222222222222\n");
		return -EINVAL;
	}
}
EXPORT_SYMBOL(sfax8_recovery_init);

interrupt_type sfax8_recovery_clear_interrupt(void)
{
	struct rb_ops *ops;
	struct sfax8 *sfax8;
	if (!rb_pdev)
		return -EINVAL;
	ops = dev_get_drvdata(&rb_pdev->dev);
	sfax8 = dev_get_drvdata(rb_pdev->dev.parent);
	if(!ops)
		return UNKNOW_INT;
	if(ops->clear_interrupt)
		return ops->clear_interrupt(sfax8);
	else
		return UNKNOW_INT;

}
EXPORT_SYMBOL(sfax8_recovery_clear_interrupt);

int sfax8_recovery_led_on(bool onoff)
{
	struct rb_ops *ops;
	if (!rb_pdev)
		return -EINVAL;
	ops = dev_get_drvdata(&rb_pdev->dev);
	if(!ops)
		return -EINVAL;
	if(ops->set_led_onoff)
		return ops->set_led_onoff(onoff);
	else
		return -EINVAL;

}
EXPORT_SYMBOL(sfax8_recovery_led_on);

/*
* mask the recovery interrupt or not.
* if the parameter is true, mask the recovery interrupt; else
* not mask the interrupt.
*/
static int sfax8_recovery_interrupt_mask(bool mask)
{
	struct rb_ops *ops = dev_get_drvdata(&rb_pdev->dev);
	struct sfax8 *sfax8 = dev_get_drvdata(rb_pdev->dev.parent);
	if(!ops)
		return -EINVAL;
	if(ops->set_interrupt_mask)
		return ops->set_interrupt_mask(mask, sfax8);
	else
		return -EINVAL;
}

static int reset_button_show(struct seq_file *m, void *v)
{
	return 0;
}

static int reset_button_open(struct inode *inode, struct file *file)
{
	return single_open(file, reset_button_show, PDE_DATA(inode));
}

static ssize_t reset_button_read(struct file *file, char __user *buffer,
							size_t count, loff_t *f_ops)
{
	char useage[] = "echo 1 | 0 to this node to mask thre reset-button interrupt or not.\n";
	int ret;
	if( *f_ops > 0)
		return 0;
	ret = sizeof(useage);
	if(copy_to_user(buffer, useage, ret))
		ret = -EFAULT;
	else
		*f_ops += ret;
	return ret;
}

static ssize_t reset_button_write(struct file *file, const char __user *buffer,
							size_t count, loff_t *f_ops)
{
	unsigned int tmp;
	bool mask;
	sscanf(buffer, "%u", &tmp);
	if(tmp)
		mask = true;
	else
		mask = false;
	sfax8_recovery_interrupt_mask(mask);
	return count;
}

static struct file_operations reset_button_ops = {
	.owner		= THIS_MODULE,
	.open		= reset_button_open,
	.read		= reset_button_read,
	.write		= reset_button_write,
	.release	= single_release,
	.llseek		= seq_lseek,
};

static int sfax8_rb_probe(struct platform_device *pdev)
{

	struct sfax8 *sfax8;
	struct proc_dir_entry *file;
	int ret;

	rb_pdev = pdev;
	sfax8 = dev_get_drvdata(pdev->dev.parent);
	ret = rb_probe[sfax8->type](pdev);
	if(ret){
		pr_err("Reset button probe failed.\n");
		return ret;
	}

	file = proc_create_data(NODE_NAME, 0644, NULL, &reset_button_ops, NULL);
	if(!file){
		pr_err("Creat %s node failed\n", NODE_NAME);
		return -ENOMEM;
	}

	return 0;
}

static int sfax8_rb_exit(struct platform_device *pdev){
		remove_proc_entry(NODE_NAME, NULL);
		return 0;
}

static struct platform_driver sfax8_rb_driver = {
		.driver			= {
				.name	= "reset-button",
				.owner	= THIS_MODULE,
		},
		.probe			= sfax8_rb_probe,
		.remove			= sfax8_rb_exit,
};

module_platform_driver(sfax8_rb_driver);

MODULE_AUTHOR("Chang Li <chang.li@siflower.com.cn>");
MODULE_LICENSE("GPL");
