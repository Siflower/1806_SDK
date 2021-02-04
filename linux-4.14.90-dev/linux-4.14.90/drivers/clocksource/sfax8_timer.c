/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/sfax8_timer.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/clk.h>
#include <linux/sched_clock.h>
#include <linux/cpu.h>
#include <sf16a18.h>

#define CONFIG_CLKSRC_SFAX8_TIMER

#define SF_TIMER_MIN_PERIOD			4
#define APBT_MIN_DELTA_USEC			200

#define SF_TIMER_LOAD_COUNT		0x00
#define SF_TIMER_CURRENT_VALUE		0x04
#define SF_TIMER_CONTROL			0x08
#define SF_TIMER_EOI				0x0c
#define SF_TIMER_INT_STATUS		0x10


#define SF_TIMER_CONTROL_ENABLE		(1 << 0)
/* 1: periodic, 0:free running. */
#define SF_TIMER_CONTROL_MODE_PERIODIC	(1 << 1)
#define SF_TIMER_CONTROL_INT		(1 << 2)

DEFINE_PER_CPU(struct sfax8_clock_event_device, sf_clockevent);
struct device_node *sf_timer;

static inline struct sfax8_clock_event_device *
ced_to_sfax8_ced(struct clock_event_device *evt)
{
	return container_of(evt, struct sfax8_clock_event_device, ced);
}

static unsigned long apbt_readl(struct sfax8_timer *timer, unsigned long offs)
{
	return readl(timer->base + offs);
}

static void apbt_writel(struct sfax8_timer *timer, unsigned long val,
		 unsigned long offs)
{
	writel(val, timer->base + offs);
}

static void apbt_disable_int(struct sfax8_timer *timer)
{
	unsigned long ctrl = apbt_readl(timer, SF_TIMER_CONTROL);

	ctrl |= SF_TIMER_CONTROL_INT;
	apbt_writel(timer, ctrl, SF_TIMER_CONTROL);
}

/**
 * sfax8_clockevent_pause() - stop the clock_event_device from running
 *
 * @sf_ced:	The APB clock to stop generating events.
 */
void sfax8_clockevent_pause(struct sfax8_clock_event_device *sf_ced)
{
	disable_irq(sf_ced->timer.irq);
	apbt_disable_int(&sf_ced->timer);
}

static void apbt_eoi(struct sfax8_timer *timer)
{
	apbt_readl(timer, SF_TIMER_EOI);
}

static irqreturn_t sfax8_clockevent_irq(int irq, void *data)
{
	struct clock_event_device *evt = data;
	struct sfax8_clock_event_device *sf_ced = ced_to_sfax8_ced(evt);

	if (!evt->event_handler) {
		pr_info("Spurious APBT timer interrupt %d", irq);
		return IRQ_NONE;
	}

	if (sf_ced->eoi)
		sf_ced->eoi(&sf_ced->timer);

	evt->event_handler(evt);
	return IRQ_HANDLED;
}

static void apbt_enable_int(struct sfax8_timer *timer)
{
	unsigned long ctrl = apbt_readl(timer, SF_TIMER_CONTROL);
	/* clear pending intr */
	apbt_readl(timer, SF_TIMER_EOI);
	ctrl &= ~SF_TIMER_CONTROL_INT;
	apbt_writel(timer, ctrl, SF_TIMER_CONTROL);
}

static int apbt_next_event(unsigned long delta,
			   struct clock_event_device *evt)
{
	unsigned long ctrl;
	struct sfax8_clock_event_device *sf_ced = ced_to_sfax8_ced(evt);

	/* Disable timer */
	ctrl = apbt_readl(&sf_ced->timer, SF_TIMER_CONTROL);
	ctrl &= ~SF_TIMER_CONTROL_ENABLE;
	apbt_writel(&sf_ced->timer, ctrl, SF_TIMER_CONTROL);
	/* write new count */
	apbt_writel(&sf_ced->timer, delta, SF_TIMER_LOAD_COUNT);
	ctrl |= SF_TIMER_CONTROL_ENABLE;
	apbt_writel(&sf_ced->timer, ctrl, SF_TIMER_CONTROL);

	return 0;
}

/**
 * sfax8_clockevent_init() - use an APB timer as a clock_event_device
 *
 * @cpu:	The CPU the events will be targeted at.
 * @name:	The name used for the timer and the IRQ for it.
 * @rating:	The rating to give the timer.
 * @base:	I/O base for the timer registers.
 * @irq:	The interrupt number to use for the timer.
 * @freq:	The frequency that the timer counts at.
 *
 * This creates a clock_event_device for using with the generic clock layer
 * but does not start and register it.  This should be done with
 * sfax8_clockevent_register() as the next step.  If this is the first time
 * it has been called for a timer then the IRQ will be requested, if not it
 * just be enabled to allow CPU hotplug to avoid repeatedly requesting and
 * releasing the IRQ.
 */
void sfax8_clockevent_init(int cpu, const char *name, unsigned rating,
		       void __iomem *base, int irq, unsigned long freq)
{
	int err;
	struct sfax8_clock_event_device *sf_ced =
						&per_cpu(sf_clockevent, cpu);

	if (!sf_ced)
		return;
	sf_ced->timer.base = base;
	sf_ced->timer.irq = irq;
	sf_ced->timer.freq = freq;

	clockevents_calc_mult_shift(&sf_ced->ced, freq, SF_TIMER_MIN_PERIOD);
	sf_ced->ced.max_delta_ns = clockevent_delta2ns(0x7fffffff,
						       &sf_ced->ced);
	sf_ced->ced.min_delta_ns = clockevent_delta2ns(800, &sf_ced->ced);
	sf_ced->ced.cpumask = cpumask_of(cpu);
	sf_ced->ced.features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT;
//	sf_ced->ced.set_mode = apbt_set_mode;
	sf_ced->ced.set_next_event = apbt_next_event;
	sf_ced->ced.irq = sf_ced->timer.irq;
	sf_ced->ced.rating = rating;
	sf_ced->ced.name = name;

	sf_ced->irqaction.name		= sf_ced->ced.name;
	sf_ced->irqaction.handler	= sfax8_clockevent_irq;
	sf_ced->irqaction.dev_id	= &sf_ced->ced;
	sf_ced->irqaction.irq		= irq;
	sf_ced->irqaction.flags		= IRQF_NOBALANCING | IRQF_TIMER;

	sf_ced->eoi = apbt_eoi;
	err = setup_irq(irq, &sf_ced->irqaction);
	if (err) {
		pr_err("failed to request timer irq\n");
		kfree(sf_ced);
		return;
	}

	sfax8_clockevent_register(sf_ced);

	return;
}

/**
 * sfax8_clockevent_resume() - resume a clock that has been paused.
 *
 * @sf_ced:	The APB clock to resume.
 */
void sfax8_clockevent_resume(struct sfax8_clock_event_device *sf_ced)
{
	enable_irq(sf_ced->timer.irq);
}

/**
 * sfax8_clockevent_stop() - stop the clock_event_device and release the IRQ.
 *
 * @sf_ced:	The APB clock to stop generating the events.
 */
void sfax8_clockevent_stop(struct sfax8_clock_event_device *sf_ced)
{
	free_irq(sf_ced->timer.irq, &sf_ced->ced);
}

/**
 * sfax8_clockevent_register() - register the clock with the generic layer
 *
 * @sf_ced:	The APB clock to register as a clock_event_device.
 */
void sfax8_clockevent_register(struct sfax8_clock_event_device *sf_ced)
{
	apbt_writel(&sf_ced->timer, 0, SF_TIMER_CONTROL);
	clockevents_register_device(&sf_ced->ced);
	apbt_enable_int(&sf_ced->timer);
}

static void __init timer_get_base_and_rate(struct device_node *np,
				    void __iomem **base, u32 *rate)
{
	struct clk *timer_clk;
	struct clk *pclk;
	unsigned int cpu = smp_processor_id();

	*base = of_iomap(np, cpu);

	if (!*base)
		panic("Unable to map regs for %s", np->name);

	/*
	 * Not all implementations use a periphal clock, so don't panic
	 * if it's not present
	 */
	pclk = of_clk_get_by_name(np, "pclk");
	if (!IS_ERR(pclk))
		if (clk_prepare_enable(pclk))
			pr_warn("pclk for %s is present, but could not be activated\n",
				np->name);

	timer_clk = of_clk_get_by_name(np, "timer");
	if (IS_ERR(timer_clk))
		goto try_clock_freq;

	if (!clk_prepare_enable(timer_clk)) {
		*rate = clk_get_rate(timer_clk);
		return;
	}

try_clock_freq:
	if (of_property_read_u32(np, "clock-freq", rate) &&
	    of_property_read_u32(np, "clock-frequency", rate))
		panic("No clock nor clock-frequency property for %s", np->name);
}

#ifdef CONFIG_CLKSRC_SFAX8_TIMER
static inline struct sfax8_clocksource *
clocksource_to_sfax8_clocksource(struct clocksource *cs)
{
	return container_of(cs, struct sfax8_clocksource, cs);
}

/**
 * sfax8_clocksource_start() - start the clocksource counting.
 *
 * @sf_cs:	The clocksource to start.
 *
 * This is used to start the clocksource before registration and can be used
 * to enable calibration of timers.
 */
void sfax8_clocksource_start(struct sfax8_clocksource *sf_cs)
{
	/*
	 * start count down from 0xffff_ffff. this is done by toggling the
	 * enable bit then load initial load count to ~0.
	 */
	unsigned long ctrl = apbt_readl(&sf_cs->timer, SF_TIMER_CONTROL);

	ctrl &= ~SF_TIMER_CONTROL_ENABLE;
	apbt_writel(&sf_cs->timer, ctrl, SF_TIMER_CONTROL);
	apbt_writel(&sf_cs->timer, ~0, SF_TIMER_LOAD_COUNT);
	/* enable, mask interrupt */
	ctrl &= ~SF_TIMER_CONTROL_MODE_PERIODIC;
	ctrl |= (SF_TIMER_CONTROL_ENABLE | SF_TIMER_CONTROL_INT);
	apbt_writel(&sf_cs->timer, ctrl, SF_TIMER_CONTROL);
	/* read it once to get cached counter value initialized */
	sfax8_clocksource_read(sf_cs);
}

static cycle_t __apbt_read_clocksource(struct clocksource *cs)
{
	unsigned long current_count;
	struct sfax8_clocksource *sf_cs =
		clocksource_to_sfax8_clocksource(cs);

	current_count = apbt_readl(&sf_cs->timer, SF_TIMER_CURRENT_VALUE);

	return (cycle_t)~current_count;
}

static void apbt_restart_clocksource(struct clocksource *cs)
{
	struct sfax8_clocksource *sf_cs =
		clocksource_to_sfax8_clocksource(cs);

	sfax8_clocksource_start(sf_cs);
}

/**
 * sfax8_clocksource_init() - use an APB timer as a clocksource.
 *
 * @rating:	The rating to give the clocksource.
 * @name:	The name for the clocksource.
 * @base:	The I/O base for the timer registers.
 * @freq:	The frequency that the timer counts at.
 *
 * This creates a clocksource using an APB timer but does not yet register it
 * with the clocksource system.  This should be done with
 * sfax8_clocksource_register() as the next step.
 */
struct sfax8_clocksource *
sfax8_clocksource_init(unsigned rating, const char *name, void __iomem *base,
			unsigned long freq)
{
	struct sfax8_clocksource *sf_cs = kzalloc(sizeof(*sf_cs), GFP_KERNEL);

	if (!sf_cs)
		return NULL;

	sf_cs->timer.base = base;
	sf_cs->timer.freq = freq;
	sf_cs->cs.name = name;
	sf_cs->cs.rating = rating;
	sf_cs->cs.read = __apbt_read_clocksource;
	sf_cs->cs.mask = CLOCKSOURCE_MASK(32);
	sf_cs->cs.flags = CLOCK_SOURCE_IS_CONTINUOUS;
	sf_cs->cs.resume = apbt_restart_clocksource;

	return sf_cs;
}

/**
 * sfax8_clocksource_register() - register the APB clocksource.
 *
 * @sf_cs:	The clocksource to register.
 */
void sfax8_clocksource_register(struct sfax8_clocksource *sf_cs)
{
	clocksource_register_hz(&sf_cs->cs, sf_cs->timer.freq);
}

/**
 * sfax8_clocksource_read() - read the current value of a clocksource.
 *
 * @sf_cs:	The clocksource to read.
 */
cycle_t sfax8_clocksource_read(struct sfax8_clocksource *sf_cs)
{
	return (cycle_t)~apbt_readl(&sf_cs->timer, SF_TIMER_CURRENT_VALUE);
}

static void __iomem *sched_io_base;
static u32 sched_rate;

static u64 notrace read_sched_clock(void)
{
	return ~readl(sched_io_base);
}

static void __init init_sched_clock(void)
{

	sched_clock_register(read_sched_clock, 32, sched_rate);
}

static void __init add_clocksource(struct device_node *source_timer)
{
	void __iomem *iobase;
	struct sfax8_clocksource *cs;
	u32 rate;

	timer_get_base_and_rate(source_timer, &iobase, &rate);
#ifdef CONFIG_SF16A18_MPW0
	//FIXME: switch clocksource to timer 2.
	iobase = of_iomap(source_timer, 2);
#endif
	cs = sfax8_clocksource_init(300, source_timer->name, iobase, rate);
	if (!cs)
		panic("Unable to initialise clocksource device");

	sfax8_clocksource_start(cs);
	sfax8_clocksource_register(cs);

	/*
	 * Fallback to use the clocksource as sched_clock if no separate
	 * timer is found. sched_io_base then points to the current_value
	 * register of the clocksource timer.
	 */
	sched_io_base = iobase + 0x04;
	sched_rate = rate;
	init_sched_clock();
}
#endif /* CONFIG_CLKSRC_SFAX8_TIMER */

#ifdef CONFIG_SF16A18_MPW0
static void add_clockevent(void)
{
	void __iomem *iobase;
	unsigned int cpu = smp_processor_id();
	u32 irq, rate;

	irq = irq_of_parse_and_map(sf_timer, cpu);
	if (irq == 0)
		panic("No IRQ for clock event timer\n");

	timer_get_base_and_rate(sf_timer, &iobase, &rate);

	sfax8_clockevent_init(cpu, sf_timer->name, 350, iobase, irq,
			rate);

	if(irq_force_affinity(irq, cpumask_of(cpu)) != 0)
		panic("Failed to set timer clockevent irq affinity!\n");
}

static int sf_timer_notify(struct notifier_block *self, unsigned long action,
				void *hcpu)
{
	switch (action & ~CPU_TASKS_FROZEN) {
	case CPU_STARTING:
		add_clockevent();
		break;
	case CPU_DYING:
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block sf_timer_nb = {
	.notifier_call = sf_timer_notify,
};
#endif

static int __init sfax8_timer_init(struct device_node *timer)
{
	if(release_reset(SF_TIMER_SOFT_RESET))
		return -1;

	sf_timer = timer;

#ifdef CONFIG_CLKSRC_SFAX8_TIMER
	add_clocksource(sf_timer);
#endif
#ifdef CONFIG_SF16A18_MPW0
	if(register_cpu_notifier(&sf_timer_nb))
		panic("Unable to initialise clockevent device");
	add_clockevent();
#endif
	return 0;
}
CLOCKSOURCE_OF_DECLARE(sfax8_timer, "siflower,sfax8-timer", sfax8_timer_init);
