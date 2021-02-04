/*
 * (C) Copyright 2009 Intel Corporation
 * Author: Jacob Pan (jacob.jun.pan@intel.com)
 *
 * Shared with ARM platforms, Jamie Iles, Picochip 2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Support for the Synopsys DesignWare APB Timers.
 */
#ifndef __SFAX8_TIMER_H__
#define __SFAX8_TIMER_H__

#include <linux/clockchips.h>
#include <linux/clocksource.h>
#include <linux/interrupt.h>

typedef u64 cycle_t;
#define APBTMRS_REG_SIZE       0x14

struct sfax8_timer {
	void __iomem				*base;
	unsigned long				freq;
	int					irq;
};

struct sfax8_clock_event_device {
	struct clock_event_device		ced;
	struct sfax8_timer			timer;
	struct irqaction			irqaction;
	void					(*eoi)(struct sfax8_timer *);
};

struct sfax8_clocksource {
	struct sfax8_timer			timer;
	struct clocksource			cs;
};

void sfax8_clockevent_register(struct sfax8_clock_event_device *sf_ced);
void sfax8_clockevent_pause(struct sfax8_clock_event_device *sf_ced);
void sfax8_clockevent_resume(struct sfax8_clock_event_device *sf_ced);
void sfax8_clockevent_stop(struct sfax8_clock_event_device *sf_ced);

void sfax8_clockevent_init(int cpu, const char *name, unsigned rating,
		       void __iomem *base, int irq, unsigned long freq);
struct sfax8_clocksource *
sfax8_clocksource_init(unsigned rating, const char *name, void __iomem *base,
			unsigned long freq);
void sfax8_clocksource_register(struct sfax8_clocksource *sf_cs);
void sfax8_clocksource_start(struct sfax8_clocksource *sf_cs);
cycle_t sfax8_clocksource_read(struct sfax8_clocksource *sf_cs);

#endif /* __SFAX8_TIMER_H__ */
