/*
 * Copyright (C) 2013 Altera Corporation <www.altera.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <watchdog.h>
#include <asm/io.h>
#include <asm/utils.h>
#include <mach/sfax8.h>

#define SF_WDT_CR	0x00
#define SF_WDT_TORR	0x04
#define SF_WDT_CRR	0x0C

#define SF_WDT_CR_EN_OFFSET	0x00
#define SF_WDT_CR_RMOD_OFFSET	0x01
#define SF_WDT_CR_RMOD_VAL	0x00
#define SF_WDT_CRR_RESTART_VAL	0x76


#if defined(CONFIG_HW_WATCHDOG)
/*
 * Set the watchdog time interval.
 * Counter is 32 bit.
 */
static int sfax8_wdt_settimeout(unsigned int timeout)
{
	signed int i;

	/* calculate the timeout range value */
	i = (log_2_n_round_up(timeout * CONFIG_SF_WDT_CLOCK_KHZ)) - 16;
	if (i > 15)
		i = 15;
	if (i < 0)
		i = 0;

	writel(i, (void *)(SIFLOWER_WDT_BASE + SF_WDT_TORR));
	return 0;
}

static void sfax8_wdt_enable(void)
{
	writel(((SF_WDT_CR_RMOD_VAL << SF_WDT_CR_RMOD_OFFSET) |
	      (0x1 << SF_WDT_CR_EN_OFFSET)),
	      (void *)(SIFLOWER_WDT_BASE + SF_WDT_CR));
}

static unsigned int sfax8_wdt_is_enabled(void)
{
	unsigned long val;
	val = readl((void *)(SIFLOWER_WDT_BASE + SF_WDT_CR));
	return val & 0x1;
}

void hw_watchdog_reset(void)
{
	if (sfax8_wdt_is_enabled())
		/* restart the watchdog counter */
		writel(SF_WDT_CRR_RESTART_VAL,
		      (void *)(SIFLOWER_WDT_BASE + SF_WDT_CRR));
}

void hw_watchdog_init(void)
{
	/* reset to disable the watchdog */
	hw_watchdog_reset();
	/* set timer in miliseconds */
	sfax8_wdt_settimeout(CONFIG_HW_WATCHDOG_TIMEOUT_MS);
	/* enable the watchdog */
	sfax8_wdt_enable();
	/* reset the watchdog */
	hw_watchdog_reset();
}
#endif
