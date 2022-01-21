/*
 * Copyright 2016 Shanghai Siflower Communication Technology Co., Ltd.
 * http://www.siflower.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Based on dw_wdt.c
 *
 * This file implements a driver for the Siflower watchdog device
 * in the many subsystems. The watchdog has 16 different timeout periods
 * and these are a function of the input clock frequency.
 *
 * The Siflower watchdog can be stopped once it has been started so we
 * implement a stop function. The watchdog core will continue to send
 * heartbeat requests after the watchdog device has been closed.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <sf19a28.h>

static DEFINE_PER_CPU(struct timer_list, sfax8_wdt_timer);
atomic_t g_last_jiffies[4];

#define WDOG_CONTROL_REG_OFFSET		    0x00
#define WDOG_CONTROL_REG_WDT_EN_MASK	    0x01
#define WDOG_TIMEOUT_RANGE_REG_OFFSET	    0x04
#define WDOG_TIMEOUT_RANGE_TOPINIT_SHIFT    4
#define WDOG_CURRENT_COUNT_REG_OFFSET	    0x08
#define WDOG_COUNTER_RESTART_REG_OFFSET     0x0c
#define WDOG_COUNTER_RESTART_KICK_VALUE	    0x76

#define WDOG_SYS_EVENT		(0x19E02050)
#define WDOG_SYS_EVENT_MASK	(0x80)
#define WDOG_DEV_OPEN		(1)       /* Opened via /dev/watchdog ? */

/* The maximum TOP (timeout period) value that can be set in the watchdog. */
#define SFAX8_WDT_MAX_TOP		15

#define SFAX8_WDT_DEFAULT_SECONDS	20

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
		 "(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

struct sfax8_wdt {
	void __iomem		*regs;
	void __iomem		*sys_event;
	struct clk		*clk;
	unsigned long		rate;
	struct notifier_block	restart_handler;
	struct watchdog_device	wdd;
};

#define to_sfax8_wdt(wdd)	container_of(wdd, struct sfax8_wdt, wdd)

/*
* enable or disable sys event mask for watchdog, only
* enable the sys enevt mask, the watchdog would work well.
* @onoff : if true enable the mask, else disable the mask.
*/
static int sfax8_wdt_sys_mask(struct sfax8_wdt *sfax8_wdt, bool onoff)
{
	u32 tmp = readl(sfax8_wdt->sys_event);
	if(onoff)
		tmp = tmp & (~WDOG_SYS_EVENT_MASK);
	else
		tmp = tmp | WDOG_SYS_EVENT_MASK;

	writel(tmp, sfax8_wdt->sys_event);

	return 0;
}

static inline int sfax8_wdt_is_enabled(struct sfax8_wdt *sfax8_wdt)
{
	return readl(sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET) &
		WDOG_CONTROL_REG_WDT_EN_MASK;
}

static inline int sfax8_wdt_top_in_seconds(struct sfax8_wdt *sfax8_wdt, unsigned top)
{
	/*
	 * There are 16 possible timeout values in 0..15 where the number of
	 * cycles is 2 ^ (16 + i) and the watchdog counts down.
	 */
	return (1U << (16 + top)) / sfax8_wdt->rate;
}

static void sfax8_wdt_clk_gate(int on)
{
	u32 val;

	val = get_module_clk_gate(SF_WDT_SOFT_RESET, 0);
	if (on)
		val |= 0x1;
	else
		val &= ~0x1;
	set_module_clk_gate(SF_WDT_SOFT_RESET, val, 0);


}

static void sfax8_wdt_ping_cpu(void * iwdd){
	struct watchdog_device *wdd = (struct watchdog_device*) iwdd;
	struct timer_list *t = this_cpu_ptr(&sfax8_wdt_timer);
	int cpu = smp_processor_id();

	unsigned char feed =1;
	unsigned char cpu_index = 0;
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);

	atomic_set(&(g_last_jiffies[cpu]),jiffies);

	if (cpu == 0) {
		for (cpu_index = 0; cpu_index < 4; cpu_index++) {
			if (jiffies - atomic_read(&(g_last_jiffies[cpu_index])) > (wdd->timeout*HZ*2/3)) {
				feed = 0;
				printk("[wdt_error] cpu:%d not feed wdt\n", cpu_index);
			}
		}
		if (feed) {
			writel(WDOG_COUNTER_RESTART_KICK_VALUE, sfax8_wdt->regs +
			       WDOG_COUNTER_RESTART_REG_OFFSET);
		}
	}

	mod_timer(t, jiffies + (wdd->timeout * HZ - HZ/2 )/ 2);

	// printk("[wdt_info]  cpu%d set jiffies %08x now %08x\n", cpu,atomic_read(&(g_last_jiffies[cpu])), jiffies);
}

static int sfax8_wdt_ping(struct watchdog_device *wdd)
{
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);
	unsigned char  feed = 1;
	unsigned char  cpu_index = 0;

	/*
	 *  Modify timer here or we won't know when to ping after close
	 *  /dev/watchdog
	 */
	on_each_cpu(sfax8_wdt_ping_cpu,wdd,1);

	for(cpu_index = 0;cpu_index < 4;cpu_index++){
		if((jiffies - atomic_read(&(g_last_jiffies[cpu_index]))) > (wdd->timeout * HZ)) {
			feed = 0;
			printk("[wdt_error]  cpu%d not feed wdt\n",cpu_index);
		}
	}
	if(feed){
		writel(WDOG_COUNTER_RESTART_KICK_VALUE, sfax8_wdt->regs +
				  WDOG_COUNTER_RESTART_REG_OFFSET);
	  // printk("[wdt_info]  feed here\n");
	}

	// printk("[wdt_info]  start timer\n");
	return 0;
}

static void sfax8_wdt_timer_ping(unsigned long arg)
{
	struct watchdog_device *wdd = (struct watchdog_device *)arg;

	/*
	 * If watchdog is not opened via /dev/watchdog, ping it
	 * every wdog->timeout / 2 seconds to prevent reboot
	 */
	if (test_bit(WDOG_ACTIVE, &wdd->status) &&
	    (!test_bit(WDOG_DEV_OPEN, &wdd->status)))
		sfax8_wdt_ping_cpu(wdd);
}

static int sfax8_wdt_set_timeout(struct watchdog_device *wdd, unsigned int top_s)
{
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);
	int i, top_val = SFAX8_WDT_MAX_TOP;

	/*
	 * Iterate over the timeout values until we find the closest match. We
	 * always look for >=.
	 */
	for (i = 0; i <= SFAX8_WDT_MAX_TOP; ++i)
		if (sfax8_wdt_top_in_seconds(sfax8_wdt, i) >= top_s) {
			top_val = i;
			break;
		}

	/*
	 * Set the new value in the watchdog.  Some versions of sfax8_wdt
	 * have have TOPINIT in the TIMEOUT_RANGE register (as per
	 * CP_WDT_DUAL_TOP in WDT_COMP_PARAMS_1).  On those we
	 * effectively get a pat of the watchdog right here.
	 */
	writel(top_val | top_val << WDOG_TIMEOUT_RANGE_TOPINIT_SHIFT,
	       sfax8_wdt->regs + WDOG_TIMEOUT_RANGE_REG_OFFSET);

	wdd->timeout = sfax8_wdt_top_in_seconds(sfax8_wdt, top_val);

	sfax8_wdt_ping(wdd);


	return 0;
}

static int sfax8_wdt_start(struct watchdog_device *wdd)
{
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);

	sfax8_wdt_clk_gate(1);

	atomic_set(&(g_last_jiffies[0]),jiffies);
	atomic_set(&(g_last_jiffies[1]),jiffies);
	atomic_set(&(g_last_jiffies[2]),jiffies);
	atomic_set(&(g_last_jiffies[3]),jiffies);

	sfax8_wdt_set_timeout(wdd, wdd->timeout);

	writel(WDOG_CONTROL_REG_WDT_EN_MASK,
	       sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET);

	sfax8_wdt_sys_mask(sfax8_wdt, true);
	return 0;
}

static int sfax8_wdt_stop(struct watchdog_device *wdd)
{
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);
	u32 val = readl(sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET);

	writel(val & (~WDOG_CONTROL_REG_WDT_EN_MASK),
			sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET);
	sfax8_wdt_sys_mask(sfax8_wdt, false);

	sfax8_wdt_clk_gate(0);

	return 0;
}

static int sfax8_wdt_restart_handle(struct notifier_block *this,
				 unsigned long mode, void *cmd)
{
	struct sfax8_wdt *sfax8_wdt;
	u32 val;

	sfax8_wdt = container_of(this, struct sfax8_wdt, restart_handler);

	writel(0, sfax8_wdt->regs + WDOG_TIMEOUT_RANGE_REG_OFFSET);
	val = readl(sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET);
	if (val & WDOG_CONTROL_REG_WDT_EN_MASK)
		writel(WDOG_COUNTER_RESTART_KICK_VALUE,
		       sfax8_wdt->regs + WDOG_COUNTER_RESTART_REG_OFFSET);
	else
		writel(WDOG_CONTROL_REG_WDT_EN_MASK,
		       sfax8_wdt->regs + WDOG_CONTROL_REG_OFFSET);

	/* wait for reset to assert... */
	mdelay(500);

	return NOTIFY_DONE;
}

static unsigned int sfax8_wdt_get_timeleft(struct watchdog_device *wdd)
{
	struct sfax8_wdt *sfax8_wdt = to_sfax8_wdt(wdd);

	return readl(sfax8_wdt->regs + WDOG_CURRENT_COUNT_REG_OFFSET) /
		sfax8_wdt->rate;
}

static const struct watchdog_info sfax8_wdt_ident = {
	.options	= WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT |
			  WDIOF_MAGICCLOSE,
	.identity	= "Siflower Watchdog",
};

static const struct watchdog_ops sfax8_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= sfax8_wdt_start,
	.stop		= sfax8_wdt_stop,
	.ping		= sfax8_wdt_ping,
	.set_timeout	= sfax8_wdt_set_timeout,
	.get_timeleft	= sfax8_wdt_get_timeleft,
};

#ifdef CONFIG_PM_SLEEP
static int sfax8_wdt_suspend(struct device *dev)
{
	struct sfax8_wdt *sfax8_wdt = dev_get_drvdata(dev);

	clk_disable_unprepare(sfax8_wdt->clk);

	return 0;
}

static int sfax8_wdt_resume(struct device *dev)
{
	struct sfax8_wdt *sfax8_wdt = dev_get_drvdata(dev);
	int err = clk_prepare_enable(sfax8_wdt->clk);

	if (err)
		return err;

	sfax8_wdt_ping(&sfax8_wdt->wdd);

	return 0;
}
#endif /* CONFIG_PM_SLEEP */

static SIMPLE_DEV_PM_OPS(sfax8_wdt_pm_ops, sfax8_wdt_suspend, sfax8_wdt_resume);


static void sfax8_init_timer(void *wdd){
	struct timer_list *t = this_cpu_ptr(&sfax8_wdt_timer);
	setup_timer(t, sfax8_wdt_timer_ping, (unsigned long)wdd);
}

static void sfax8_deinit_timer(void *wdd){
	struct timer_list *t = this_cpu_ptr(&sfax8_wdt_timer);
	del_timer_sync(t);
}

static int sfax8_wdt_drv_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct watchdog_device *wdd;
	struct sfax8_wdt *sfax8_wdt;
	struct resource *mem;
	int ret;

	if(release_reset(SF_WDT_SOFT_RESET))
		return -EFAULT;

	sfax8_wdt = devm_kzalloc(dev, sizeof(*sfax8_wdt), GFP_KERNEL);
	if (!sfax8_wdt)
		return -ENOMEM;

	mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	sfax8_wdt->regs = devm_ioremap_resource(dev, mem);
	if (IS_ERR(sfax8_wdt->regs))
		return PTR_ERR(sfax8_wdt->regs);

	sfax8_wdt->sys_event = ioremap(WDOG_SYS_EVENT, sizeof(WDOG_SYS_EVENT));

	if (IS_ERR(sfax8_wdt->sys_event))
		return PTR_ERR(sfax8_wdt->sys_event);

	sfax8_wdt->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(sfax8_wdt->clk))
		return PTR_ERR(sfax8_wdt->clk);

	ret = clk_prepare_enable(sfax8_wdt->clk);
	if (ret)
		return ret;

	sfax8_wdt->rate = clk_get_rate(sfax8_wdt->clk);
	if (sfax8_wdt->rate == 0) {
		ret = -EINVAL;
		goto out_disable_clk;
	}

	wdd = &sfax8_wdt->wdd;
	wdd->info = &sfax8_wdt_ident;
	wdd->ops = &sfax8_wdt_ops;
	wdd->min_timeout = 1;
	wdd->max_timeout =
		sfax8_wdt_top_in_seconds(sfax8_wdt, SFAX8_WDT_MAX_TOP) * 1000;
	wdd->parent = dev;

	watchdog_set_drvdata(wdd, sfax8_wdt);
	watchdog_set_nowayout(wdd, nowayout);

	wdd->timeout = SFAX8_WDT_DEFAULT_SECONDS;
	watchdog_init_timeout(wdd, 0, dev);

	platform_set_drvdata(pdev, sfax8_wdt);

	on_each_cpu(sfax8_init_timer, wdd, 1);
	/* Always start watchdog */
	sfax8_wdt_start(wdd);
	set_bit(WDOG_ACTIVE, &wdd->status);

	ret = watchdog_register_device(wdd);
	if (ret)
		goto out_disable_clk;

	sfax8_wdt->restart_handler.notifier_call = sfax8_wdt_restart_handle;
	sfax8_wdt->restart_handler.priority = 128;
	ret = register_restart_handler(&sfax8_wdt->restart_handler);
	if (ret)
		pr_warn("cannot register restart handler\n");

	return 0;

out_disable_clk:
	clk_disable_unprepare(sfax8_wdt->clk);
	return ret;
}

static int sfax8_wdt_drv_remove(struct platform_device *pdev)
{
	struct sfax8_wdt *sfax8_wdt = platform_get_drvdata(pdev);

	unregister_restart_handler(&sfax8_wdt->restart_handler);
	watchdog_unregister_device(&sfax8_wdt->wdd);
	sfax8_wdt_stop(&sfax8_wdt->wdd);
	iounmap(sfax8_wdt->sys_event);
	clk_disable_unprepare(sfax8_wdt->clk);
	on_each_cpu(sfax8_deinit_timer, NULL, 1);

	if(hold_reset(SF_WDT_SOFT_RESET))
		return -EFAULT;

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id sfax8_wdt_of_match[] = {
	{ .compatible = "siflower,sfax8-wdt", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, sfax8_wdt_of_match);
#endif

static struct platform_driver sfax8_wdt_driver = {
	.probe		= sfax8_wdt_drv_probe,
	.remove		= sfax8_wdt_drv_remove,
	.driver		= {
		.name	= "sfax8_wdt",
		.of_match_table = of_match_ptr(sfax8_wdt_of_match),
		.pm	= &sfax8_wdt_pm_ops,
	},
};

module_platform_driver(sfax8_wdt_driver);

MODULE_AUTHOR("Qi Zhang");
MODULE_DESCRIPTION("Siflower Watchdog Driver");
MODULE_LICENSE("GPL");
