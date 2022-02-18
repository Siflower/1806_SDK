/*
 * SPI Driver for SFA18
 *
 * Copyright (C) 2016 Qi Zhang <qi.zhang@siflower.com.cn>
 *
 * Inspired form linux kernel driver and atmel uboot driver
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <spi.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <fdtdec.h>
#include "sfax8_spi.h"
#include <asm/gpio.h>
#include <mach/pad.h>


DECLARE_GLOBAL_DATA_PTR;

struct sfax8_spi_platdata {
	unsigned long base;
	unsigned int clock;
};

struct sfax8_spi_priv {
	struct sfax8_spi_registers *spi_reg;
	unsigned int source_clock;
	struct gpio_desc	cs0_gpio;
	struct gpio_desc    cs1_gpio;
};

static void _spi_cs_activate(struct sfax8_spi_priv *priv)
{
	struct sfax8_spi_registers *reg = priv->spi_reg;

	setbits_le32(&reg->sspcr1, SSPCR1_SSE);

	if (!dm_gpio_is_valid(&priv->cs0_gpio))
		return;

	dm_gpio_set_value(&priv->cs0_gpio, 1);
}

static void _spi_cs_deactivate(struct sfax8_spi_priv *priv)
{
	struct sfax8_spi_registers *reg = priv->spi_reg;

	clrbits_le32(&reg->sspcr1, SSPCR1_SSE);
	if (!dm_gpio_is_valid(&priv->cs0_gpio))
		return;

	dm_gpio_set_value(&priv->cs0_gpio, 0);
}

static int _spi_xfer(struct sfax8_spi_priv *priv, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	unsigned int len_tx;
	unsigned int len_rx;
	unsigned int len;
	u32 status, tx_stat;
	const u8 *txp = dout;
	u8 *rxp = din;
	u8 value;
	struct sfax8_spi_registers *reg = priv->spi_reg;

	debug("spi_xfer: slave :%u dout %p din %p bitlen %u\n", (uint)reg,
			(uint *)dout, (uint *)din, bitlen);

	if (bitlen == 0)
		/* Finish any previously submitted transfers */
		goto out;

	if (bitlen % 8) {
		/* Errors always terminate an ongoing transfer */
		flags |= SPI_XFER_END;
		goto out;
	}

	len = bitlen / 8;

	if (flags & SPI_XFER_BEGIN) {
		/* Empty RX FIFO */
		while ((readl(&reg->sspsr) & SSPSR_RNE))
			readl(&reg->sspdr);

		_spi_cs_activate(priv);
	}

	for (len_tx = 0, len_rx = 0; len_rx < len;) {
		tx_stat = readl(&reg->sspris);
		status = readl(&reg->sspsr);

		if ((len_tx < len) && (tx_stat & SSPRIS_TX)) {
			if (txp)
				value = *txp++;
			else
				value = 0xff;

			writel(value, &reg->sspdr);
			len_tx++;
		}

		if (status & SSPSR_RNE) {
			value = readl(&reg->sspdr);

			if (rxp)
				*rxp++ = value;
			len_rx++;
		}
	}

out:
	if (flags & SPI_XFER_END) {
		/*
		 * Wait until the transfer is completely done before
		 * we deactivate CS.
		 */
		do {
			status = readl(&reg->sspsr);
		} while (status & SSPSR_BSY);

		_spi_cs_deactivate(priv);
	}

	return 0;
}

#ifndef CONFIG_DM_SPI

#else	/* Config_DM_SPI */

static int sfax8_spi_xfer(struct udevice *dev, unsigned int bitlen,
			  const void *dout, void *din, unsigned long flags)
{
	struct udevice *bus = dev_get_parent(dev);
	struct sfax8_spi_priv *priv = dev_get_priv(bus);

	return _spi_xfer(priv, bitlen, dout, din, flags);
}

#ifdef CONFIG_SFAX8_SPI_DYNAMIC_SETUP

static int _spi_set_speed(struct sfax8_spi_registers *reg, unsigned int rate,
		unsigned int sspclock)
{
	unsigned int cpsr, scr;
	unsigned int reg_scr = readl(&reg->sspcr0);

	/* Clear scr bits */
	reg_scr &= (~0xff) << SSPCR0_SCR_SHIFT;

	/* Calculate divisors so that we can get speed according the
	 * following formula:
	 *	rate = spi_clock_rate / (cpsr * (1 + scr))
	 *
	 * cpsr must be even number and starts from 2, scr can be any number
	 * between 0 and 255.
	 */
	for (cpsr = 2; cpsr <= 254; cpsr += 2) {
		for (scr = 0; scr <= 255; scr++) {
			if ((sspclock / (cpsr * (scr + 1))) <= rate) {
				writel(reg_scr | (scr << SSPCR0_SCR_SHIFT),
						&reg->sspcr0);
				writel(cpsr, &reg->sspcpsr);
				debug("scr = %d, cpsr = %d\n", scr, cpsr);
				return 0;
			}
		}
	}

	return -1;
}

static int _spi_set_wordlen(struct sfax8_spi_registers *reg,
					unsigned int wordlen)
{
	debug("spi set wordlen %u\n", wordlen);
	if ((wordlen < 4) || (wordlen > 16))
		return -1;

	writel((readl(&reg->sspcr0) & (~0xf)) | (wordlen - 1), &reg->sspcr0);
	return 0;
}

static int sfax8_spi_set_speed(struct udevice *bus, unsigned int hz)
{
	struct sfax8_spi_priv *priv = dev_get_priv(bus);
	struct sfax8_spi_registers *reg = priv->spi_reg;
	unsigned int sspclock = priv->source_clock;

	if (hz > sspclock / 2)
		hz = priv->source_clock / 2;

	if (hz < sspclock / (244 * 246)) {
		debug("spi speed too slow\n");
		return -1;
	}

	return _spi_set_speed(reg, hz, sspclock);
}

static int sfax8_spi_set_mode(struct udevice *bus, unsigned int mode)
{
	struct sfax8_spi_priv *priv = dev_get_priv(bus);
	struct sfax8_spi_registers *reg = priv->spi_reg;

	/* set control register 0 */
	if(mode & SPI_CPHA)
		setbits_le32(&reg->sspcr0, SSPCR0_SPH);
	else
		clrbits_le32(&reg->sspcr0, SSPCR0_SPH);

	if (mode & SPI_CPOL)
		setbits_le32(&reg->sspcr0, SSPCR0_SPO);
	else
		clrbits_le32(&reg->sspcr0, SSPCR0_SPO);

	/* set control register 1 */
	writel(0, &reg->sspcr1);
	writel(0, &reg->sspcr1);

	return 0;
}

static int sfax8_spi_claim_bus(struct udevice *dev)
{
	struct spi_slave *slave = dev_get_parent_priv(dev);
	struct udevice *bus = dev_get_parent(dev);
	struct sfax8_spi_priv *priv = dev_get_priv(bus);

	_spi_set_wordlen(priv->spi_reg, slave->wordlen);

	return 0;
}

static int sfax8_spi_release_bus(struct udevice *dev)
{
	/* Nothing to do here */
	return 0;
}
#endif /* CONFIG_SFAX8_SPI_DYNAMIC_SETUP */

static int sfax8_spi_probe(struct udevice *bus)
{
	struct sfax8_spi_priv *priv = dev_get_priv(bus);
	struct udevice *pinctrl;
	fdt_addr_t addr;
	int ret;

	addr = dev_get_addr(bus);
	if (addr == FDT_ADDR_T_NONE)
		return -EINVAL;
	priv->spi_reg = (struct sfax8_spi_registers *)addr;

	priv->source_clock = fdtdec_get_int(gd->fdt_blob, bus->of_offset, "clock", 1);

	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret)
		debug("%s: Cannot find pinctrl device\n", __func__);

	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_SPI0);
	if (ret)
		debug("%s: Failed to set up spi0 pinctrl!\n", __func__);

	/* SFAX8 SPI controller can automatically drive /CS during transfer
	 * depending on fifo fill-level. /CS will stay asserted as long as
	 * TX fifo is non-empty, else will be deasserted confirming completion
	 * of the ongoing transfer. To avoid this sort of error we will drive
	 * /CS manually by toggling cs-gpio pins.
	 */
	ret = gpio_request_by_name_nodev(gd->fdt_blob, bus->of_offset,
			"cs0-gpios", 0, &priv->cs0_gpio, GPIOD_IS_OUT);

	if (ret) {
		printf("sfax8-spi: error, cs0-gpios not found\n");
		return ret;
	}

#ifndef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
	writel(0x207, &priv->spi_reg->sspcr0);
	writel(0, &priv->spi_reg->sspcr1);
	writel(0x2, &priv->spi_reg->sspcpsr);
#endif

	return 0;
}

static const struct dm_spi_ops sfax8_spi_ops = {
	.xfer = sfax8_spi_xfer,
#ifdef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
	.claim_bus = sfax8_spi_claim_bus,
	.release_bus = sfax8_spi_release_bus,
	.set_speed = sfax8_spi_set_speed,
	.set_mode = sfax8_spi_set_mode,
#endif
	/*
	 * cs_info is not needed, since we require all chip selects to be
	 * in the device tree explicitly
	 */
};

static const struct udevice_id sfax8_spi_ids[] = {
	{ .compatible = "siflower,armada-a18-spi" },
	{ .compatible = "siflower,armada-a28-spi" },
	{ }
};

U_BOOT_DRIVER(sfax8_spi) = {
	.name = "sfax8_spi",
	.id = UCLASS_SPI,
	.of_match = sfax8_spi_ids,
	.ops = &sfax8_spi_ops,
	.priv_auto_alloc_size = sizeof(struct sfax8_spi_priv),
	.probe = sfax8_spi_probe,
};
#endif
