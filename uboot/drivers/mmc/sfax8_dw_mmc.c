/*
 * Copyright (c) 2013 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <dwmmc.h>
#include <errno.h>
#include <pwrseq.h>
#include <syscon.h>
//#include <asm/gpio.h>
//#include <asm/arch/clock.h>
#include <mach/pad.h>
#include <linux/err.h>

DECLARE_GLOBAL_DATA_PTR;

struct sfax8_mmc_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

struct sfax8_dwmmc_priv {
	struct clk clk;
	struct dwmci_host host;
	uint freq;
};

static uint sfax8_dwmmc_get_mmc_clk(struct dwmci_host *host, uint freq)
{
	#if 0
	struct udevice *dev = host->priv;
	struct sfax8_dwmmc_priv *priv = dev_get_priv(dev);
	int ret;

	ret = clk_set_rate(&priv->clk, freq);
	if (ret < 0) {
		debug("%s: err=%d\n", __func__, ret);
		return ret;
	}

	return freq;
	#else
	struct udevice *dev = host->priv;
	struct sfax8_dwmmc_priv *priv = dev_get_priv(dev);
	return priv->freq;
	#endif
}

static int sfax8_dwmmc_ofdata_to_platdata(struct udevice *dev)
{
	struct sfax8_dwmmc_priv *priv = dev_get_priv(dev);
	struct dwmci_host *host = &priv->host;

	host->name = dev->name;
	host->ioaddr = (void *)dev_get_addr(dev);
	host->buswidth = fdtdec_get_int(gd->fdt_blob, dev->of_offset,
					"bus-width", 4);
	priv->freq = fdtdec_get_int(gd->fdt_blob, dev->of_offset, "freq", 1);
	host->get_mmc_clk = sfax8_dwmmc_get_mmc_clk;
	host->priv = dev;

	/* use non-removeable as sdcard and emmc as judgement */
	if (fdtdec_get_bool(gd->fdt_blob, dev->of_offset, "non-removable"))
		host->dev_index = 0;
	else
		host->dev_index = 1;

	return 0;
}

static int sfax8_dwmmc_probe(struct udevice *dev)
{
#ifdef CONFIG_BLK
	struct sfax8_mmc_plat *plat = dev_get_platdata(dev);
#endif
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct sfax8_dwmmc_priv *priv = dev_get_priv(dev);
	struct dwmci_host *host = &priv->host;
	struct udevice *pwr_dev __maybe_unused;
	u32 minmax[2];
	//int ret;
	int fifo_depth;
#ifdef CONFIG_TARGET_SFA18_MPW0
	int read_delayline,write_delayline;
#endif

	//ret = clk_get_by_index(dev, 0, &priv->clk);
	//if (ret < 0)
	//	return ret;

	if (fdtdec_get_int_array(gd->fdt_blob, dev->of_offset,
				 "clock-freq-min-max", minmax, 2))
		return -EINVAL;

	fifo_depth = fdtdec_get_int(gd->fdt_blob, dev->of_offset,
				    "fifo-depth", 0);
	if (fifo_depth < 0)
		return -EINVAL;

#ifdef CONFIG_TARGET_SFA18_MPW0
	read_delayline = fdtdec_get_int(gd->fdt_blob, dev->of_offset,
				    "read_delayline", 0);

	if((read_delayline < 0) || (read_delayline > 0xFF))
		read_delayline = 0;

	write_delayline = fdtdec_get_int(gd->fdt_blob, dev->of_offset,
				    "write_delayline", 0);

	if((write_delayline < 0) || (write_delayline > 0xFF))
		write_delayline = 0;

	dwmci_writel(host, DWMCI_GPIO, (read_delayline << 8) | (write_delayline << 16));
#endif

	host->fifoth_val = MSIZE(0x2) |
		RX_WMARK(fifo_depth / 2 - 1) | TX_WMARK(fifo_depth / 2);

	if (fdtdec_get_bool(gd->fdt_blob, dev->of_offset, "fifo-mode"))
		host->fifo_mode = true;

#ifdef CONFIG_PWRSEQ
	/* Enable power if needed */
	ret = uclass_get_device_by_phandle(UCLASS_PWRSEQ, dev, "mmc-pwrseq",
					   &pwr_dev);
	if (!ret) {
		ret = pwrseq_set_power(pwr_dev, true);
		if (ret)
			return ret;
	}
#endif
#ifdef CONFIG_BLK
	dwmci_setup_cfg(&plat->cfg, dev->name, host->buswidth, host->caps,
			minmax[1], minmax[0]);
	host->mmc = &plat->mmc;
#else
	ret = add_dwmci(host, minmax[1], minmax[0]);
	if (ret)
		return ret;

#endif
	host->mmc->priv = &priv->host;
	host->mmc->dev = dev;
	upriv->mmc = host->mmc;

	return 0;
}

static int sfax8_dwmmc_bind(struct udevice *dev)
{
#ifdef CONFIG_BLK
	struct sfax8_mmc_plat *plat = dev_get_platdata(dev);
	int ret;

	ret = dwmci_bind(dev, &plat->mmc, &plat->cfg);
	if (ret)
		return ret;
#endif

	return 0;
}

static const struct udevice_id sfax8_mmc_ids[] = {
	{ .compatible = "siflower,sfax8-mmc" },
	{ }
};

U_BOOT_DRIVER(sfax8_mmc) = {
	.name		= "sfax8_mmc",
	.id		= UCLASS_MMC,
	.of_match	= sfax8_mmc_ids,
	.ofdata_to_platdata = sfax8_dwmmc_ofdata_to_platdata,
	.bind		= sfax8_dwmmc_bind,
	.probe		= sfax8_dwmmc_probe,
	.priv_auto_alloc_size = sizeof(struct sfax8_dwmmc_priv),
	.platdata_auto_alloc_size = sizeof(struct sfax8_mmc_plat),
};


#ifdef CONFIG_PWRSEQ
static int sfax8_dwmmc_pwrseq_set_power(struct udevice *dev, bool enable)
{
	struct gpio_desc reset;
	int ret;

	ret = gpio_request_by_name(dev, "reset-gpios", 0, &reset, GPIOD_IS_OUT);
	if (ret)
		return ret;
	dm_gpio_set_value(&reset, 1);
	udelay(1);
	dm_gpio_set_value(&reset, 0);
	udelay(200);

	return 0;
}

static const struct pwrseq_ops siflower_dwmmc_pwrseq_ops = {
	.set_power	= sfax8_dwmmc_pwrseq_set_power,
};

static const struct udevice_id sfax8_dwmmc_pwrseq_ids[] = {
	{ .compatible = "mmc-pwrseq-emmc" },
	{ }
};

U_BOOT_DRIVER(sfax8_dwmmc_pwrseq_drv) = {
	.name		= "mmc_pwrseq_emmc",
	.id		= UCLASS_PWRSEQ,
	.of_match	= sfax8_dwmmc_pwrseq_ids,
	.ops		= &sfax8_dwmmc_pwrseq_ops,
};
#endif
