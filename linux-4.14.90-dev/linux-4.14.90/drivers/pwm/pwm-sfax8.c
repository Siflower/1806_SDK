/*
 * Copyright (C) 2016 Shanghai Siflower Communication Technology Co., Ltd.
 *
 * PWM driver for Siflower SoCs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 */

#include <linux/bitops.h>
#include <linux/clk.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <sf16a18.h>

/* For struct sfax8_timer_variant and sfax8_pwm_lock. */
#include <clocksource/sfax8_pwm.h>

#define REG_TCFG0			0x00
#define REG_TCFG1			0x04
#define REG_TCON			0x08

#define REG_TCNTB(chan)			(0x0c + ((chan) * 0xc))
#define REG_TCMPB(chan)			(0x10 + ((chan) * 0xc))

#define TCFG0_PRESCALER_MASK		0xff
#define TCFG0_PRESCALER1_SHIFT		8

#define TCFG0_PRES_MASK			0xf
#define TCFG1_MUX_MASK			0xf
#define TCFG0_SHIFT(chan)		(8 * (chan))
#define TCFG1_SHIFT(chan)		(4 * (chan))

/*
 * Each channel occupies 4 bits in TCON register, but there is a gap of 4
 * bits (one channel) after channel 0, so channels have different numbering
 * when accessing TCON register. See to_tcon_channel() function.
 *
 * In addition, the location of autoreload bit for channel 4 (TCON channel 5)
 * in its set of bits is 2 as opposed to 3 for other channels.
 */
#define TCON_START(chan)		BIT(4 * (chan) + 0)
#define TCON_MANUALUPDATE(chan)		BIT(4 * (chan) + 1)
#define TCON_INVERT(chan)		BIT(4 * (chan) + 2)
#define _TCON_AUTORELOAD(chan)		BIT(4 * (chan) + 3)
#define _TCON_AUTORELOAD4(chan)		BIT(4 * (chan) + 2)
#define TCON_AUTORELOAD(chan)		\
	((chan < 5) ? _TCON_AUTORELOAD(chan) : _TCON_AUTORELOAD4(chan))

/**
 * struct sfax8_pwm_channel - private data of PWM channel
 * @period_ns:	current period in nanoseconds programmed to the hardware
 * @duty_ns:	current duty time in nanoseconds programmed to the hardware
 * @tin_ns:	time of one timer tick in nanoseconds with current timer rate
 */
struct sfax8_pwm_channel {
	u32 period_ns;
	u32 duty_ns;
	u32 tin_ns;
};

/**
 * struct sfax8_pwm_chip - private data of PWM chip
 * @chip:		generic PWM chip
 * @variant:		local copy of hardware variant data
 * @inverter_mask:	inverter status for all channels - one bit per channel
 * @base:		base address of mapped PWM registers
 * @base_clk:		base clock used to drive the timers
 * @tclk0:		external clock 0 (can be ERR_PTR if not present)
 * @tclk1:		external clock 1 (can be ERR_PTR if not present)
 */
struct sfax8_pwm_chip {
	struct pwm_chip chip;
	struct sfax8_pwm_variant variant;
	u8 inverter_mask;

	void __iomem *base;
	struct clk *base_clk;
	struct clk *tclk0;
	struct clk *tclk1;
};

#ifndef CONFIG_CLKSRC_SFAX8_PWM
/*
 * PWM block is shared between pwm-sfax8 and sfax8_pwm_timer drivers
 * and some registers need access synchronization. If both drivers are
 * compiled in, the spinlock is defined in the clocksource driver,
 * otherwise following definition is used.
 *
 * Currently we do not need any more complex synchronization method
 * because all the supported SoCs contain only one instance of the PWM
 * IP. Should this change, both drivers will need to be modified to
 * properly synchronize accesses to particular instances.
 */
static DEFINE_SPINLOCK(sfax8_pwm_lock);
#endif

static inline
struct sfax8_pwm_chip *to_sfax8_pwm_chip(struct pwm_chip *chip)
{
	return container_of(chip, struct sfax8_pwm_chip, chip);
}

static inline unsigned int to_tcon_channel(unsigned int channel)
{
	/* TCON register has a gap of 4 bits (1 channel) after channel 0 */
	return (channel == 0) ? 0 : (channel + 1);
}

static void pwm_sfax8_clk_gate(int on)
{
	u32 val;

	val = get_module_clk_gate(SF_PWM_SOFT_RESET, 0);
	if (on)
		val |= 0x1;
	else
		val &= ~0x1;
	set_module_clk_gate(SF_PWM_SOFT_RESET, val, 0);
}

static void pwm_sfax8_set_divisor(struct sfax8_pwm_chip *pwm,
				    unsigned int channel, u8 divisor)
{
	u8 shift = TCFG1_SHIFT(channel);
	unsigned long flags;
	u32 reg;
	u8 bits;

	bits = 8 + (fls(divisor) - 1) - pwm->variant.div_base;

	spin_lock_irqsave(&sfax8_pwm_lock, flags);

	reg = readl(pwm->base + REG_TCFG1);
	reg &= ~(TCFG1_MUX_MASK << shift);
	reg |= bits << shift;
	writel(reg, pwm->base + REG_TCFG1);

	spin_unlock_irqrestore(&sfax8_pwm_lock, flags);

}

static void pwm_sfax8_set_prescaler(struct sfax8_pwm_chip *pwm, unsigned int channel, u8 prescaler)
{
	u8 shift = TCFG0_SHIFT(channel);
	unsigned long flags;
	u32 reg;

	spin_lock_irqsave(&sfax8_pwm_lock, flags);

	reg = readl(pwm->base + REG_TCFG0);
	reg &= ~(TCFG0_PRES_MASK << shift);
	reg |= prescaler << shift;
	writel(reg, pwm->base + REG_TCFG0);

	spin_unlock_irqrestore(&sfax8_pwm_lock, flags);
}

static int pwm_sfax8_is_tdiv(struct sfax8_pwm_chip *chip, unsigned int chan)
{
	struct sfax8_pwm_variant *variant = &chip->variant;
	u32 reg;

	reg = readl(chip->base + REG_TCFG1);
	reg >>= TCFG1_SHIFT(chan);
	reg &= TCFG1_MUX_MASK;

	return (BIT(reg) & variant->tclk_mask) == 0;
}

static unsigned long pwm_sfax8_get_tin_rate(struct sfax8_pwm_chip *chip,
					      unsigned int chan)
{
	unsigned long rate;
	u32 reg;

	rate = clk_get_rate(chip->base_clk);

	reg = readl(chip->base + REG_TCFG0);
	if (chan >= 2)
		reg >>= TCFG0_PRESCALER1_SHIFT;
	reg &= TCFG0_PRESCALER_MASK;

	return rate / (reg + 1);
}

static unsigned long pwm_sfax8_calc_tin(struct sfax8_pwm_chip *chip,
					  unsigned int chan, unsigned long freq)
{
	unsigned long rate;
	struct clk *clk;
	u8 div;
	u8 pres = 1;

	if (!pwm_sfax8_is_tdiv(chip, chan)) {
		clk = (chan < 2) ? chip->tclk0 : chip->tclk1;
		if (!IS_ERR(clk)) {
			rate = clk_get_rate(clk);
			if (rate)
				return rate;
		}

		dev_warn(chip->chip.dev,
			"tclk of PWM %d is inoperational, using tdiv\n", chan);
	}

	rate = pwm_sfax8_get_tin_rate(chip, chan);
	dev_dbg(chip->chip.dev, "tin parent at %lu\n", rate);

	/*
	 * Compare minimum PWM frequency that can be achieved with possible
	 * divider settings and choose the lowest divisor that can generate
	 * frequencies lower than requested.
	 */
	if (rate < freq)
	{
		dev_err(chip->chip.dev, "can't set so high a frequency like %lu\n", freq);
		return -EINVAL;
	}

	for (div = 1; div < 5; div++)
	{
		if ((rate >> div) < freq)
		{
			div--;
			break;
		}
	}


	if (div == 5)
		div = 4;
	if (div == 0)
		div = 5;

	div--;

	pwm_sfax8_set_divisor(chip, chan, BIT(div));

	div++;

	if(div == 1 || div == 2 || div == 3 || div == 4)
	    rate = rate >> div;

	while((rate / pres) > (freq << 16) && pres < 256)
		pres++;

	pres--;

	pwm_sfax8_set_prescaler(chip, chan, pres);

	return rate / (pres + 1);
}

static int pwm_sfax8_request(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct sfax8_pwm_chip *our_chip = to_sfax8_pwm_chip(chip);
	struct sfax8_pwm_channel *our_chan;

	if (!(our_chip->variant.output_mask & BIT(pwm->hwpwm))) {
		dev_warn(chip->dev,
			"tried to request PWM channel %d without output\n",
			pwm->hwpwm);
		return -EINVAL;
	}

	our_chan = devm_kzalloc(chip->dev, sizeof(*our_chan), GFP_KERNEL);
	if (!our_chan)
		return -ENOMEM;

	pwm_set_chip_data(pwm, our_chan);

	return 0;
}

static void pwm_sfax8_free(struct pwm_chip *chip, struct pwm_device *pwm)
{
	devm_kfree(chip->dev, pwm_get_chip_data(pwm));
	pwm_set_chip_data(pwm, NULL);
}

static int pwm_sfax8_enable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct sfax8_pwm_chip *our_chip = to_sfax8_pwm_chip(chip);
	unsigned int tcon_chan = to_tcon_channel(pwm->hwpwm);
	unsigned long flags;
	u32 tcon;

	pwm_sfax8_clk_gate(1);

	spin_lock_irqsave(&sfax8_pwm_lock, flags);

	tcon = readl(our_chip->base + REG_TCON);

	tcon &= ~TCON_START(tcon_chan);
	tcon |= TCON_MANUALUPDATE(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	tcon &= ~TCON_MANUALUPDATE(tcon_chan);
	tcon |= TCON_START(tcon_chan) | TCON_AUTORELOAD(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	spin_unlock_irqrestore(&sfax8_pwm_lock, flags);

	return 0;
}

static void pwm_sfax8_disable(struct pwm_chip *chip, struct pwm_device *pwm)
{
	struct sfax8_pwm_chip *our_chip = to_sfax8_pwm_chip(chip);
	unsigned int tcon_chan = to_tcon_channel(pwm->hwpwm);
	unsigned long flags;
	u32 tcon;

	spin_lock_irqsave(&sfax8_pwm_lock, flags);

	tcon = readl(our_chip->base + REG_TCON);
	tcon &= ~TCON_AUTORELOAD(tcon_chan);
	writel(tcon, our_chip->base + REG_TCON);

	pwm_sfax8_clk_gate(0);

	spin_unlock_irqrestore(&sfax8_pwm_lock, flags);
}

static int pwm_sfax8_config(struct pwm_chip *chip, struct pwm_device *pwm,
			      int duty_ns, int period_ns)
{
	struct sfax8_pwm_chip *our_chip = to_sfax8_pwm_chip(chip);
	struct sfax8_pwm_channel *chan = pwm_get_chip_data(pwm);
	u32 tin_ns = chan->tin_ns, tcnt, tcmp;

	/*
	 * We currently avoid using 64bit arithmetic by using the
	 * fact that anything faster than 1Hz is easily representable
	 * by 32bits.
	 */
	if (period_ns > NSEC_PER_SEC)
		return -ERANGE;

	if (period_ns == chan->period_ns && duty_ns == chan->duty_ns)
		return 0;

	tcnt = readl(our_chip->base + REG_TCNTB(pwm->hwpwm));

	/* We need tick count for calculation, not last tick. */
	++tcnt;

	/* Check to see if we are changing the clock rate of the PWM. */
	if (chan->period_ns != period_ns) {
		unsigned long tin_rate;
		u32 period;

		period = NSEC_PER_SEC / period_ns;

		dev_dbg(our_chip->chip.dev, "duty_ns=%d, period_ns=%d (%u)\n",
						duty_ns, period_ns, period);

		tin_rate = pwm_sfax8_calc_tin(our_chip, pwm->hwpwm, period);

		dev_dbg(our_chip->chip.dev, "tin_rate=%lu\n", tin_rate);

		tin_ns = NSEC_PER_SEC / tin_rate;
		tcnt = period_ns / tin_ns;
	}

	/* Period is too short. */
	if (tcnt <= 1)
		return -ERANGE;

	/* Note that counters count down. */
	tcmp = duty_ns / tin_ns;

	/* 0% duty is not available */
	if (!tcmp)
		++tcmp;

	tcmp = tcnt - tcmp;

	/* Decrement to get tick numbers, instead of tick counts. */
	--tcnt;
	/* -1UL will give 100% duty. */
	--tcmp;

	dev_dbg(our_chip->chip.dev,
				"tin_ns=%u, tcmp=%u/%u\n", tin_ns, tcmp, tcnt);

	/* Update PWM registers. */
	writel(tcnt, our_chip->base + REG_TCNTB(pwm->hwpwm));
	writel(tcmp, our_chip->base + REG_TCMPB(pwm->hwpwm));

	chan->period_ns = period_ns;
	chan->tin_ns = tin_ns;
	chan->duty_ns = duty_ns;

	return 0;
}

static void pwm_sfax8_set_invert(struct sfax8_pwm_chip *chip,
				   unsigned int channel, bool invert)
{
	unsigned int tcon_chan = to_tcon_channel(channel);
	unsigned long flags;
	u32 tcon;

	spin_lock_irqsave(&sfax8_pwm_lock, flags);

	tcon = readl(chip->base + REG_TCON);

	if (invert) {
		chip->inverter_mask |= BIT(channel);
		tcon |= TCON_INVERT(tcon_chan);
	} else {
		chip->inverter_mask &= ~BIT(channel);
		tcon &= ~TCON_INVERT(tcon_chan);
	}

	writel(tcon, chip->base + REG_TCON);

	spin_unlock_irqrestore(&sfax8_pwm_lock, flags);
}

static int pwm_sfax8_set_polarity(struct pwm_chip *chip,
				    struct pwm_device *pwm,
				    enum pwm_polarity polarity)
{
	struct sfax8_pwm_chip *our_chip = to_sfax8_pwm_chip(chip);
	bool invert = (polarity == PWM_POLARITY_NORMAL);

	/* Inverted means normal in the hardware. */
	pwm_sfax8_set_invert(our_chip, pwm->hwpwm, invert);

	return 0;
}

static const struct pwm_ops pwm_sfax8_ops = {
	.request	= pwm_sfax8_request,
	.free		= pwm_sfax8_free,
	.enable		= pwm_sfax8_enable,
	.disable	= pwm_sfax8_disable,
	.config		= pwm_sfax8_config,
	.set_polarity	= pwm_sfax8_set_polarity,
	.owner		= THIS_MODULE,
};

#ifdef CONFIG_OF
static const struct sfax8_pwm_variant sfax8_variant = {
	.bits		= 32,
	.div_base	= 0,
	.has_tint_cstat	= false,
	.tclk_mask	= 0,
};

static const struct of_device_id sfax8_pwm_matches[] = {
	{ .compatible = "siflower,sfax8-pwm-output", .data = &sfax8_variant },
	{},
};

static int pwm_sfax8_parse_dt(struct sfax8_pwm_chip *chip)
{
	struct device_node *np = chip->chip.dev->of_node;
	const struct of_device_id *match;
	struct property *prop;
	const __be32 *cur;
	u32 val;

	match = of_match_node(sfax8_pwm_matches, np);
	if (!match)
		return -ENODEV;

	memcpy(&chip->variant, match->data, sizeof(chip->variant));

	of_property_for_each_u32(np, "pwm-outputs", prop, cur, val) {
		if (val >= SFAX8_PWM_NUM) {
			dev_err(chip->chip.dev,
				"%s: invalid channel index in pwm-outputs property\n",
								__func__);
			continue;
		}
		chip->variant.output_mask |= BIT(val);
	}

	return 0;
}
#else
static int pwm_sfax8_parse_dt(struct sfax8_pwm_chip *chip)
{
	return -ENODEV;
}
#endif

static int pwm_sfax8_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct sfax8_pwm_chip *chip;
	struct resource *res;
	unsigned int chan;
	int ret;

	if(release_reset(SF_PWM_SOFT_RESET))
		return -EFAULT;

	chip = devm_kzalloc(&pdev->dev, sizeof(*chip), GFP_KERNEL);
	if (chip == NULL)
		return -ENOMEM;

	chip->chip.dev = &pdev->dev;
	chip->chip.ops = &pwm_sfax8_ops;
	chip->chip.base = -1;
	chip->chip.npwm = SFAX8_PWM_NUM;
	chip->inverter_mask = BIT(SFAX8_PWM_NUM) - 1;

	if (IS_ENABLED(CONFIG_OF) && pdev->dev.of_node) {
		ret = pwm_sfax8_parse_dt(chip);
		if (ret)
			return ret;

		chip->chip.of_xlate = of_pwm_xlate_with_flags;
		chip->chip.of_pwm_n_cells = 3;
	} else {
		if (!pdev->dev.platform_data) {
			dev_err(&pdev->dev, "no platform data specified\n");
			return -EINVAL;
		}

		memcpy(&chip->variant, pdev->dev.platform_data,
							sizeof(chip->variant));
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	chip->base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(chip->base))
		return PTR_ERR(chip->base);

	chip->base_clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(chip->base_clk)) {
		dev_err(dev, "failed to get timer base clk\n");
		return PTR_ERR(chip->base_clk);
	}

	ret = clk_prepare_enable(chip->base_clk);
	if (ret < 0) {
		dev_err(dev, "failed to enable base clock\n");
		return ret;
	}

	for (chan = 0; chan < SFAX8_PWM_NUM; ++chan)
		if (chip->variant.output_mask & BIT(chan))
			pwm_sfax8_set_invert(chip, chan, true);

	/* Following clocks are optional. */
	chip->tclk0 = devm_clk_get(&pdev->dev, "pwm-tclk0");
	chip->tclk1 = devm_clk_get(&pdev->dev, "pwm-tclk1");

	platform_set_drvdata(pdev, chip);

	ret = pwmchip_add(&chip->chip);
	if (ret < 0) {
		dev_err(dev, "failed to register PWM chip\n");
		clk_disable_unprepare(chip->base_clk);
		return ret;
	}

	dev_dbg(dev, "base_clk at %lu, tclk0 at %lu, tclk1 at %lu\n",
		clk_get_rate(chip->base_clk),
		!IS_ERR(chip->tclk0) ? clk_get_rate(chip->tclk0) : 0,
		!IS_ERR(chip->tclk1) ? clk_get_rate(chip->tclk1) : 0);

	return 0;
}

static int pwm_sfax8_remove(struct platform_device *pdev)
{
	struct sfax8_pwm_chip *chip = platform_get_drvdata(pdev);
	int ret;

	ret = pwmchip_remove(&chip->chip);
	if (ret < 0)
		return ret;

	clk_disable_unprepare(chip->base_clk);

	if(hold_reset(SF_PWM_SOFT_RESET))
		return -EFAULT;

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int pwm_sfax8_suspend(struct device *dev)
{
	struct sfax8_pwm_chip *chip = dev_get_drvdata(dev);
	unsigned int i;

	/*
	 * No one preserves these values during suspend so reset them.
	 * Otherwise driver leaves PWM unconfigured if same values are
	 * passed to pwm_config() next time.
	 */
	for (i = 0; i < SFAX8_PWM_NUM; ++i) {
		struct pwm_device *pwm = &chip->chip.pwms[i];
		struct sfax8_pwm_channel *chan = pwm_get_chip_data(pwm);

		if (!chan)
			continue;

		chan->period_ns = 0;
		chan->duty_ns = 0;
	}

	return 0;
}

static int pwm_sfax8_resume(struct device *dev)
{
	struct sfax8_pwm_chip *chip = dev_get_drvdata(dev);
	unsigned int chan;

	/*
	 * Inverter setting must be preserved across suspend/resume
	 * as nobody really seems to configure it more than once.
	 */
	for (chan = 0; chan < SFAX8_PWM_NUM; ++chan) {
		if (chip->variant.output_mask & BIT(chan))
			pwm_sfax8_set_invert(chip, chan,
					chip->inverter_mask & BIT(chan));
	}

	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(pwm_sfax8_pm_ops, pwm_sfax8_suspend,
			 pwm_sfax8_resume);

static struct platform_driver pwm_sfax8_driver = {
	.driver		= {
		.name	= "sfax8-pwm-output",
		.owner	= THIS_MODULE,
		.pm	= &pwm_sfax8_pm_ops,
		.of_match_table = of_match_ptr(sfax8_pwm_matches),
	},
	.probe		= pwm_sfax8_probe,
	.remove		= pwm_sfax8_remove,
};
module_platform_driver(pwm_sfax8_driver);

MODULE_DESCRIPTION("SFAx8 PWM driver");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Qi Zhang <qi.zhang@siflower.com.cn>");
MODULE_ALIAS("platform:sfax8-pwm");
