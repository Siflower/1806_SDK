/*
 * (C) Copyright 2000
 * Rob Taylor, Flying Pig Systems. robt@flyingpig.com.
 *
 * (C) Copyright 2018
 * 
 * Mark Cai, <mark.cai@siflower.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/* Simple U-Boot driver for the SFAX8 UARTs */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <watchdog.h>
#include <asm/io.h>
#include <serial.h>
#include <dm/platform_data/serial_sfax8.h>
#include <linux/compiler.h>
#include "serial_sfax8_internal.h"
#include <fdtdec.h>

DECLARE_GLOBAL_DATA_PTR;


static int sfax8_putc(struct sfax8_regs *regs, char c)
{
	/* Wait until there is space in the FIFO */
	if (readl(&regs->fr) & UART_SFAX8_FR_TXFF)
		return -EAGAIN;

	/* Send the character */
	writel(c, &regs->dr);

	return 0;
}

static int sfax8_getc(struct sfax8_regs *regs)
{
	unsigned int data;

	/* Wait until there is data in the FIFO */
	if (readl(&regs->fr) & UART_SFAX8_FR_RXFE)
		return -EAGAIN;

	data = readl(&regs->dr);

	/* Check for an error flag */
	if (data & 0xFFFFFF00) {
		/* Clear the error */
		writel(0xFFFFFFFF, &regs->ecr);
		return -1;
	}

	return (int) data;
}

static int sfax8_tstc(struct sfax8_regs *regs)
{
	WATCHDOG_RESET();
	return !(readl(&regs->fr) & UART_SFAX8_FR_RXFE);
}

static int sfax8_generic_serial_init(struct sfax8_regs *regs)
{
	/* disable everything */
	writel(0, &regs->sfax8_cr);

	return 0;
}

static int sfax8_set_line_control(struct sfax8_regs *regs)
{
	unsigned int lcr;
	/*
	 * Internal update of baud rate register require line
	 * control register write
	 */
	lcr = UART_SFAX8_LCRH_WLEN_8 | UART_SFAX8_LCRH_FEN;
	writel(lcr, &regs->sfax8_lcrh);
	return 0;
}

static int sfax8_generic_setbrg(struct sfax8_regs *regs, int clock, int baudrate)
{
	unsigned int temp;
	unsigned int divider;
	unsigned int remainder;
	unsigned int fraction;

	/*
	* Set baud rate
	*
	* IBRD = UART_CLK / (16 * BAUD_RATE)
	* FBRD = RND((64 * MOD(UART_CLK,(16 * BAUD_RATE)))
	*		/ (16 * BAUD_RATE))
	*/
	temp = 16 * baudrate;
	divider = clock / temp;
	remainder = clock % temp;
	temp = (8 * remainder) / baudrate;
	fraction = (temp >> 1) + (temp & 1);

	writel(divider, &regs->sfax8_ibrd);
	writel(fraction, &regs->sfax8_fbrd);

	sfax8_set_line_control(regs);
	/* Finally, enable the UART */
	writel(UART_SFAX8_CR_UARTEN | UART_SFAX8_CR_TXE |
	       UART_SFAX8_CR_RXE | UART_SFAX8_CR_RTS, &regs->sfax8_cr);

	return 0;
}


#ifdef CONFIG_DM_SERIAL

struct sfax8_priv {
	struct sfax8_regs *regs;
	enum sfax8_type type;
};

static int sfax8_serial_setbrg(struct udevice *dev, int baudrate)
{
	struct sfax8_serial_platdata *plat = dev_get_platdata(dev);
	struct sfax8_priv *priv = dev_get_priv(dev);

	if (!plat->skip_init) {
		sfax8_generic_setbrg(priv->regs, plat->clock,
				     baudrate);
	}

	return 0;
}

static int sfax8_serial_probe(struct udevice *dev)
{
	struct sfax8_serial_platdata *plat = dev_get_platdata(dev);
	struct sfax8_priv *priv = dev_get_priv(dev);

	priv->regs = (struct sfax8_regs *)plat->base;
	if (!plat->skip_init)
		return sfax8_generic_serial_init(priv->regs);
	else
		return 0;
}

static int sfax8_serial_getc(struct udevice *dev)
{
	struct sfax8_priv *priv = dev_get_priv(dev);

	return sfax8_getc(priv->regs);
}

static int sfax8_serial_putc(struct udevice *dev, const char ch)
{
	struct sfax8_priv *priv = dev_get_priv(dev);

	return sfax8_putc(priv->regs, ch);
}

static int sfax8_serial_pending(struct udevice *dev, bool input)
{
	struct sfax8_priv *priv = dev_get_priv(dev);
	unsigned int fr = readl(&priv->regs->fr);

	if (input)
		return sfax8_tstc(priv->regs);
	else
		return fr & UART_SFAX8_FR_TXFF ? 0 : 1;
}

static const struct dm_serial_ops sfax8_serial_ops = {
	.putc = sfax8_serial_putc,
	.pending = sfax8_serial_pending,
	.getc = sfax8_serial_getc,
	.setbrg = sfax8_serial_setbrg,
};

#if CONFIG_IS_ENABLED(OF_CONTROL)
static const struct udevice_id sfax8_serial_id[] ={
	{.compatible = "mips,sfax8", .data = TYPE_SFAX8},
	{}
};

static int sfax8_serial_ofdata_to_platdata(struct udevice *dev)
{
	struct sfax8_serial_platdata *plat = dev_get_platdata(dev);
	fdt_addr_t addr;

	addr = dev_get_addr(dev);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;

	plat->base = addr;
	plat->clock = fdtdec_get_int(gd->fdt_blob, dev->of_offset, "clock", 1);
	plat->type = dev_get_driver_data(dev);
	#if defined(CONFIG_SFA18)
	plat->skip_init = fdtdec_get_int(gd->fdt_blob, dev->of_offset, "skip_init", 0);
	#endif
	return 0;
}
#endif

U_BOOT_DRIVER(serial_sfax8) = {
	.name	= "serial_sfax8",
	.id	= UCLASS_SERIAL,
	.of_match = of_match_ptr(sfax8_serial_id),
	.ofdata_to_platdata = of_match_ptr(sfax8_serial_ofdata_to_platdata),
	.platdata_auto_alloc_size = sizeof(struct sfax8_serial_platdata),
	.probe = sfax8_serial_probe,
	.ops	= &sfax8_serial_ops,
	.flags = DM_FLAG_PRE_RELOC,
	.priv_auto_alloc_size = sizeof(struct sfax8_priv),
};

#endif

