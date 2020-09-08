/*
 * Driver for Siflower Ax8 SPI controller.
 *
 * Copyright (C) 2016 Shanghai Siflower Communication Technology Co., Ltd.
 *
 * This driver is based on Cirrus Logic EP93xx SPI driver(spi-ep93xx.c).
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/io.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/module.h>

#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/scatterlist.h>
#include <linux/spi/spi.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>
#include <sf16a18.h>

#include <linux/dma-mapping.h>
#include <linux/random.h>
#include <linux/sched/task_stack.h>

#if 0
static int __i = 0;
#define RANDOM_FALSE()                                                         \
	({                                                                     \
		u8 __rand;                                                     \
		bool __time_out;                                               \
		get_random_bytes(&__rand, sizeof(__rand));                     \
		__i += __rand & 0xf;                                           \
		if (__i > 9461) {                                              \
			__time_out = true;                                     \
			__i = 0;                                               \
		} else                                                         \
			__time_out = false;                                    \
		__time_out;                                                    \
	})
#else
#define RANDOM_FALSE() false
#endif

#define SSPCR0			0x0000
#define SSPCR0_SPO		BIT(6)
#define SSPCR0_SPH		BIT(7)
#define SSPCR0_MODE_SHIFT	6
#define SSPCR0_SCR_SHIFT	8

#define SSPCR1			0x0004
#define SSPCR1_LBM		BIT(0)
#define SSPCR1_SSE		BIT(1)
#define SSPCR1_MS		BIT(2)
#define SSPCR1_SOD		BIT(3)

#define SSPDR			0x0008

#define SSPSR			0x000c
#define SSPSR_TFE		BIT(0)
#define SSPSR_TNF		BIT(1)
#define SSPSR_RNE		BIT(2)
#define SSPSR_RFF		BIT(3)
#define SSPSR_BSY		BIT(4)
#define SSPCPSR			0x0010

#define SSPIMSCR		0x0014
#define SSPIMSCR_RORIM	BIT(0)
#define SSPIMSCR_RXIM	BIT(2)
#define SSPIMSCR_TXIM	BIT(3)

#define SSPRIS			0x0018

#define SSPMIS			0x001c
#define SSPMIS_RXMIS	BIT(2)
#define SSPMIS_TXMIS	BIT(3)
#define SSPMIS_RORMIS	BIT(0)

#define SSPICR			0x0020
#define RORIC			BIT(0)
#define RTIC			BIT(1)

#define SSPDMACR		0x0024
#define RXDMAE			BIT(0)
#define TXDMAE			BIT(1)

#define SSPFIFO			0x0028

#define SSP_FSSOUT_NTXDOE_SEL	0x24844
#define SSP0_CSN_SEL		0x24840

/* The maximum  bytes that a dma BD can transfer.*/
#define MAX_DMA_BD_BYTES (PAGE_SIZE)
/* timeout in milliseconds */
#define SPI_TIMEOUT	40
/* maximum depth of RX/TX FIFO */
#define SPI_FIFO_SIZE	64

#ifdef CONFIG_SPI_CCM3310
#define CONFIG_SFAX8_SPI_DYNAMIC_SETUP
#endif

#ifndef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
#define DEFAULT_SCR 2
#define DEFAULT_CPSR 2
#define DEFAULT_WORD_LEN 8
#define MODE_0 0
#endif

#ifdef CONFIG_DEBUG_FS
/**
 * struct spi_statistics - statistics for spi transfers
 * @lock:          lock protecting this structure
 *
 * @bytes:         number of bytes transferred to/from device
 */
struct sfax8_spi_statistics {
	spinlock_t		lock; /* lock for the whole structure */

	unsigned long long	bytes;
};
#endif

/**
 * struct sfax8_spi - SFAx8 SPI controller structure
 * @statistics: statistics for the controller
 * @dev: pointer to struct device
 * @master: pointer to spi_master
 * @clk: clock for the controller
 * @sys_base: system manager base address
 * @regs_base: pointer to ioremap()'d registers
 * @sspdr_phys: physical address of the SSPDR register
 * @wait: wait until given transfer is completed
 * @current_xfer: transfer that is currently processed (or %NULL if none)
 * @tx: current byte in transfer to transmit
 * @rx: current byte in transfer to receive
 * @fifo_level: how full is FIFO (%0..%SPI_FIFO_SIZE - %1). Receiving one
 *              frame decreases this level and sending one frame increases it.
 * @dma_rx_ch: RX DMA channel
 * @dma_tx_ch: TX DMA channel
 */
struct sfax8_spi {
#ifdef CONFIG_DEBUG_FS
	struct sfax8_spi_statistics	statistics;
#endif
	struct device			*dev;
	struct spi_master		*master;
	struct clk			*clk;
	struct regmap			*sys_base;
	void __iomem			*regs_base;
	unsigned long			sspdr_phys;
	struct completion		wait;
	struct spi_transfer		*current_xfer;
	size_t				tx;
	size_t				rx;
	size_t				fifo_level;
	struct dma_chan			*dma_rx_ch;
	struct dma_chan			*dma_tx_ch;
	int				irq_num;
	unsigned			use_dma;
};

#ifdef CONFIG_DEBUG_FS
static ssize_t bytes_show(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct sfax8_spi *sfspi = dev_get_drvdata(dev);
	struct sfax8_spi_statistics *stat = &sfspi->statistics;
	unsigned long flags;
	ssize_t len;

	spin_lock_irqsave(&stat->lock, flags);
	len = sprintf(buf, "%llu\n", stat->bytes);
	spin_unlock_irqrestore(&stat->lock, flags);
	return len;
}

static struct device_attribute device_attrs[] = {
	__ATTR_RO(bytes),
};

static inline void sfax8_add_transfer_stats(struct sfax8_spi *sfspi,
				       struct spi_transfer *xfer)
{
	struct sfax8_spi_statistics *stat = &sfspi->statistics;
	unsigned long flags;

	spin_lock_irqsave(&stat->lock, flags);
	stat->bytes += xfer->len;
	spin_unlock_irqrestore(&stat->lock, flags);
}

static int sfax8_init_statistics(struct sfax8_spi *sfspi)
{
	struct sfax8_spi_statistics *stat = &sfspi->statistics;
	struct device *dev = sfspi->dev;
	int i, error = 0;

	dev_set_drvdata(dev, sfspi);
	spin_lock_init(&stat->lock);

	for (i = 0; i < ARRAY_SIZE(device_attrs); i++) {
		error = device_create_file(dev, &device_attrs[i]);

		if (error)
			break;
	}

	if (error) {
		while (--i >= 0)
			device_remove_file(dev, &device_attrs[i]);
	}

	return 0;
}
#else
static void sfax8_add_transfer_stats(struct sfax8_spi *sfspi,
				     struct spi_transfer *xfer)
{}
static int sfax8_init_statistics(struct sfax8_spi *sfspi) { return 0; }
#endif

/* converts bits per word to CR0.DSS value */
#define bits_per_word_to_dss(bpw)	((bpw) - 1)

static void sfax8_spi_write_u8(const struct sfax8_spi *sfspi,
				u16 reg, u8 value)
{
	writeb(value, sfspi->regs_base + reg);
}

static u8 sfax8_spi_read_u8(const struct sfax8_spi *spi, u16 reg)
{
	return readb(spi->regs_base + reg);
}

static void sfax8_spi_write_u16(const struct sfax8_spi *sfspi,
				 u16 reg, u16 value)
{
	writew(value, sfspi->regs_base + reg);
}

static u16 sfax8_spi_read_u16(const struct sfax8_spi *spi, u16 reg)
{
	return readw(spi->regs_base + reg);
}

static void print_regs(u32 begin, u32 end)
{
	u32 base = 0xB1800000;
	u32 i;

	for (i = begin; i <= end; i += 0x10)
		printk("%#x: %#x  %#x  %#x  %#x\n", base + i,
		    readl((void *)(base + i)),
		    readl((void *)(base + i + 4)),
		    readl((void *)(base + i + 8)),
		    readl((void *)(base + i + 0xc)));
}
static void sfax8_spi_dump_regs(const struct sfax8_spi *sfspi)
{
	dev_info(sfspi->dev, "CR0=%#x\n", sfax8_spi_read_u16(sfspi, SSPCR0));
	dev_info(sfspi->dev, "CR1=%#x\n", sfax8_spi_read_u16(sfspi, SSPCR1));
	dev_info(sfspi->dev, "SR=%#x\n", sfax8_spi_read_u16(sfspi, SSPSR));
	dev_info(sfspi->dev, "IMSCR=%#x\n", sfax8_spi_read_u16(sfspi, SSPIMSCR));
	dev_info(sfspi->dev, "RIS=%#x\n", sfax8_spi_read_u16(sfspi, SSPRIS));
	dev_info(sfspi->dev, "MIS=%#x\n", sfax8_spi_read_u16(sfspi, SSPMIS));
	dev_info(sfspi->dev, "DMACR=%#x\n", sfax8_spi_read_u16(sfspi, SSPDMACR));
	if (sfspi->use_dma) {
		print_regs(0, 0x38);
		print_regs(0x40, 0x5c);
		print_regs(0x100, 0x13c);
	}
}

static int sfax8_spi_enable(const struct sfax8_spi *sfspi)
{
	u8 regval;
	/*
	 * Science the clock can't be disabled, there is
	 * no need to enable the clock, too.
	 */
/*	int err;
 *
 *	err = clk_enable(sfspi->clk);
 *	if (err)
 *		return err;
 */

	regval = sfax8_spi_read_u8(sfspi, SSPCR1);
	regval |= SSPCR1_SSE;
	sfax8_spi_write_u8(sfspi, SSPCR1, regval);

	return 0;
}

static void sfax8_spi_disable(const struct sfax8_spi *sfspi)
{
	u8 regval;

	regval = sfax8_spi_read_u8(sfspi, SSPCR1);
	regval &= ~SSPCR1_SSE;
	sfax8_spi_write_u8(sfspi, SSPCR1, regval);
	/*
	 * Can't disable clock now, or the pbus's clock will be
	 * disabled!!!(March 7th, 2017)
	 */
	/*clk_disable(sfspi->clk);*/
}

static void sfax8_spi_enable_interrupts(const struct sfax8_spi *sfspi)
{
	u8 regval;

	regval = sfax8_spi_read_u8(sfspi, SSPIMSCR);
	regval |= (SSPIMSCR_RORIM | SSPIMSCR_TXIM | SSPIMSCR_RXIM);
	sfax8_spi_write_u8(sfspi, SSPIMSCR, regval);
}

static void sfax8_spi_disable_interrupts(const struct sfax8_spi *sfspi)
{
	u8 regval;

	regval = sfax8_spi_read_u8(sfspi, SSPIMSCR);
	regval &= ~(SSPIMSCR_RORIM | SSPIMSCR_TXIM | SSPIMSCR_RXIM);
	sfax8_spi_write_u8(sfspi, SSPIMSCR, regval);
}

#ifdef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
/**
 * sfax8_spi_calc_divisors() - calculates SPI clock divisors
 * @sfspi: sfax8 SPI controller struct
 * @rate: desired SPI output clock rate
 * @div_cpsr: pointer to return the cpsr (pre-scaler) divider
 * @div_scr: pointer to return the scr divider
 */
static int sfax8_spi_calc_divisors(const struct sfax8_spi *sfspi,
				    u32 rate, u8 *div_cpsr, u8 *div_scr)
{
	struct spi_master *master = sfspi->master;
	unsigned long spi_clk_rate = clk_get_rate(sfspi->clk);
	int cpsr, scr;

	/*
	 * Make sure that max value is between values supported by the
	 * controller.
	 */
	rate = clamp(rate, master->min_speed_hz, master->max_speed_hz);

	/*
	 * Calculate divisors so that we can get speed according the
	 * following formula:
	 *	rate = spi_clock_rate / (cpsr * (1 + scr))
	 *
	 * cpsr must be even number and starts from 2, scr can be any number
	 * between 0 and 255.
	 */
	for (cpsr = 2; cpsr <= 254; cpsr += 2) {
		for (scr = 0; scr <= 255; scr++) {
			if ((spi_clk_rate / (cpsr * (scr + 1))) <= rate) {
				*div_scr = (u8)scr;
				*div_cpsr = (u8)cpsr;
				return 0;
			}
		}
	}

	return -EINVAL;
}

/**
 * sfax8_spi_reg_setup() - configures hardware registers
 * @sfspi: sfax8 SPI controller struct
 * @spi: spi device struct
 * @speed_hz: transfer speed
 * @bits_per_word: transfer bits_per_word
 */
static int sfax8_spi_reg_setup(const struct sfax8_spi *sfspi,
				 const struct spi_device *spi,
				 u32 speed_hz, u8 bits_per_word)
{
	u8 dss = bits_per_word_to_dss(bits_per_word);
	u8 div_cpsr = 0;
	u8 div_scr = 0;
	u16 cr0 = 0;
	u16 cr1 = 0;

	int err;

	err = sfax8_spi_calc_divisors(sfspi, speed_hz, &div_cpsr, &div_scr);
	if (err)
		return err;

	cr0 = div_scr << SSPCR0_SCR_SHIFT;
	cr0 |= dss;
	if (spi->mode & SPI_CPHA)
		cr0 |= SSPCR0_SPH;
	if (spi->mode & SPI_CPOL)
		cr0 |= SSPCR0_SPO;

	sfax8_spi_write_u8(sfspi, SSPCPSR, div_cpsr);
	sfax8_spi_write_u16(sfspi, SSPCR0, cr0);

	if (spi->mode & SPI_LOOP) {
		cr1 |= SSPCR1_LBM;
		regmap_write(sfspi->sys_base, SSP_FSSOUT_NTXDOE_SEL, 0xf);
	} else
		regmap_write(sfspi->sys_base, SSP_FSSOUT_NTXDOE_SEL, 0x0);

	sfax8_spi_write_u8(sfspi, SSPCR1, cr1);

	return 0;
}
#else
static int sfax8_spi_reg_setup(const struct sfax8_spi *sfspi,
			       const struct spi_device *spi, u32 speed_hz,
			       u8 bits_per_word)
{
	return 0;
}

static void sfax8_spi_default_setup(struct sfax8_spi *sfspi)
{
	u16 value;

	value = (DEFAULT_SCR << SSPCR0_SCR_SHIFT) |
		bits_per_word_to_dss(DEFAULT_WORD_LEN) |
		(MODE_0 << SSPCR0_MODE_SHIFT);
	sfax8_spi_write_u16(sfspi, SSPCR0, value);
	sfax8_spi_write_u8(sfspi, SSPCR1, 0);
	sfax8_spi_write_u8(sfspi, SSPCPSR, DEFAULT_CPSR);
}
#endif

/**
 * sfax8_spi_setup() - setup an SPI device
 * @spi: SPI device to setup
 *
 * This function sets up SPI device mode, speed etc. Can be called multiple
 * times for a single device. Returns %0 in case of success, negative error in
 * case of failure. When this function returns success, the device is
 * deselected.
 */
static int sfax8_spi_setup(struct spi_device *spi)
{
	struct sfax8_spi *sfspi = spi_master_get_devdata(spi->master);
	int ret;
	int value = (spi->mode & SPI_CS_HIGH) ? 0 : 1;

	dev_dbg(sfspi->dev, "initial setup for %s\n",
			spi->modalias);

	ret = gpio_direction_output(spi->cs_gpio, value);
	if (ret) {
		dev_err(&spi->dev, "Failed to set gpio %d's direction\n",
			spi->cs_gpio);
		return ret;
	}

	sfax8_spi_reg_setup(sfspi, spi, spi->max_speed_hz, spi->bits_per_word);

	return 0;
}

static void sfax8_spi_cleanup(struct spi_device *spi)
{
	if (gpio_is_valid(spi->cs_gpio))
		gpio_free(spi->cs_gpio);
}

/*
 * JFFS2 filesystem may give a buffer address which is in the stack, and not DMA
 * safe. This means the transfer may destory the stack data, thus we have to do
 * a cache sync before we can transfer the message.
 */
static int sfax8_spi_prepare_message(struct spi_master *master,
				  struct spi_message *msg)
{
#if 0
	struct device *dev = &master->dev;
	struct spi_transfer *xfer;

	list_for_each_entry(xfer, &msg->transfers, transfer_list) {
		if (master->can_dma && !master->can_dma(master, msg->spi, xfer))
			continue;

		if (xfer->rx_buf != NULL)
			dma_cache_sync(dev, xfer->rx_buf, xfer->len,
				       DMA_BIDIRECTIONAL);
	}
#endif
	return 0;
}

static int sfax8_spi_unprepare_message(struct spi_master *master,
				  struct spi_message *msg)
{
	struct sfax8_spi *sfspi = spi_master_get_devdata(master);
	if (msg->status == -ETIMEDOUT) {
		dev_err(&sfspi->master->dev, "transfer timeout(len = %d)\n",
			sfspi->current_xfer->len);
		// sfax8_spi_dump_regs(sfspi);
		// BUG(); /* This is used for debug purpose only. */
		if (sfspi->use_dma) {
			dmaengine_terminate_all(sfspi->dma_tx_ch);
			dmaengine_terminate_all(sfspi->dma_rx_ch);
		}
	}

	sfax8_spi_disable(sfspi);

	sfspi->current_xfer = NULL;

	return 0;
}

static int sfax8_enable_clk_gate(struct spi_master *master)
{
	u16 gate_shift = master->bus_num * 2;
	u32 clk_gate;

	clk_gate = get_module_clk_gate(SF_SPI_SOFT_RESET, false);
	clk_gate |= 0x3 << gate_shift;
	set_module_clk_gate(SF_SPI_SOFT_RESET, clk_gate, false);

	return 0;
}

static int sfax8_disable_clk_gate(struct spi_master *master)
{
	u16 gate_shift = master->bus_num * 2;
	u32 clk_gate;

	clk_gate = get_module_clk_gate(SF_SPI_SOFT_RESET, false);
	clk_gate &= ~(0x3 << gate_shift);
	set_module_clk_gate(SF_SPI_SOFT_RESET, clk_gate, false);

	return 0;
}

static int sfax8_reset_hardware(struct spi_master *master,
				struct spi_device *spi)
{
	struct sfax8_spi *sfspi = spi_master_get_devdata(master);
	struct spi_transfer *t = sfspi->current_xfer;

	dev_warn(&master->dev, "sfax8 spi reset hardware!!! len=%d\n",
		 t->len);
	sfax8_spi_dump_regs(sfspi);
#ifdef CONFIG_DMADEVICES
	if (sfspi->dma_tx_ch)
		dmaengine_terminate_all(sfspi->dma_tx_ch);
	if (sfspi->dma_rx_ch)
		dmaengine_terminate_all(sfspi->dma_rx_ch);
#endif
	hold_reset(SF_SPI_SOFT_RESET);
	release_reset(SF_SPI_SOFT_RESET);

#ifndef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
	sfax8_spi_default_setup(sfspi);
#else
	sfax8_spi_setup(spi);
#endif
	return 0;
}

static void sfax8_handle_err(struct spi_master *master, struct spi_message *msg)
{
	if (msg->status == -ETIMEDOUT) {
		sfax8_reset_hardware(master, msg->spi);
	}
}

static void sfax8_do_write(struct sfax8_spi *sfspi, struct spi_transfer *t)
{
	if (t->bits_per_word > 8) {
		u16 tx_val = 0;

		if (t->tx_buf)
			tx_val = ((u16 *)t->tx_buf)[sfspi->tx];
		sfax8_spi_write_u16(sfspi, SSPDR, tx_val);
		sfspi->tx += sizeof(tx_val);
	} else {
		u8 tx_val = 0;

		if (t->tx_buf)
			tx_val = ((u8 *)t->tx_buf)[sfspi->tx];
		sfax8_spi_write_u8(sfspi, SSPDR, tx_val);
		sfspi->tx += sizeof(tx_val);
	}
}

static void sfax8_do_read(struct sfax8_spi *sfspi, struct spi_transfer *t)
{
	if (t->bits_per_word > 8) {
		u16 rx_val;

		rx_val = sfax8_spi_read_u16(sfspi, SSPDR);
		if (t->rx_buf)
			((u16 *)t->rx_buf)[sfspi->rx] = rx_val;
		sfspi->rx += sizeof(rx_val);
	} else {
		u8 rx_val;

		rx_val = sfax8_spi_read_u8(sfspi, SSPDR);
		if (t->rx_buf)
			((u8 *)t->rx_buf)[sfspi->rx] = rx_val;
		sfspi->rx += sizeof(rx_val);
	}
}

/**
 * sfax8_spi_read_write() - perform next RX/TX transfer
 * @sfspi: sfax8 SPI controller struct
 *
 * This function transfers next bytes (or half-words) to/from RX/TX FIFOs. If
 * called several times, the whole transfer will be completed. Returns
 * %-EINPROGRESS when current transfer was not yet completed otherwise %0.
 *
 * When this function is finished, RX FIFO should be empty and TX FIFO should be
 * full.
 */
static int sfax8_spi_read_write(struct sfax8_spi *sfspi)
{
	struct spi_transfer *t = sfspi->current_xfer;

	/* read as long as RX FIFO has frames in it */
	while ((sfax8_spi_read_u8(sfspi, SSPSR) & SSPSR_RNE)) {
		sfax8_do_read(sfspi, t);
		sfspi->fifo_level--;
	}

	/* write as long as TX FIFO has room */
	while (sfspi->fifo_level < SPI_FIFO_SIZE && sfspi->tx < t->len) {
		sfax8_do_write(sfspi, t);
		sfspi->fifo_level++;
	}

	if (sfspi->rx == t->len)
		return 0;

	return -EINPROGRESS;
}

static int sfax8_spi_pio_transfer(struct sfax8_spi *sfspi)
{
	/*
	 * Now everything is set up for the current transfer. We prime the TX
	 * FIFO, enable interrupts, and wait for the transfer to complete.
	 */
	if (sfax8_spi_read_write(sfspi))
		sfax8_spi_enable_interrupts(sfspi);

	return sfspi->current_xfer->len;
}

#ifdef CONFIG_DMADEVICES
static void sfax8_spi_enable_dma(struct sfax8_spi *sfspi)
{
	u8 dmacr = 0;

	if (sfspi->use_dma) {
		dmacr |= TXDMAE;
		dmacr |= RXDMAE;
	}
	sfax8_spi_write_u8(sfspi, SSPDMACR, dmacr);
}
/**
 * sfax8_spi_dma_prepare() - prepares a DMA transfer
 * @sfspi: sfax8 SPI controller struct
 * @dir: DMA transfer direction
 *
 * Function configures the DMA, maps the buffer and prepares the DMA
 * descriptor. Returns a valid DMA descriptor in case of success and ERR_PTR
 * in case of failure.
 */
static struct dma_async_tx_descriptor *
sfax8_spi_dma_prepare(struct sfax8_spi *sfspi, struct dma_chan *chan,
		      struct sg_table *sgt, enum dma_transfer_direction dir)
{
	struct spi_transfer *t = sfspi->current_xfer;
	struct dma_async_tx_descriptor *txd;
	enum dma_slave_buswidth buswidth;
	struct dma_slave_config conf;
	int ret;

	if (t->bits_per_word > 8)
		buswidth = DMA_SLAVE_BUSWIDTH_2_BYTES;
	else
		buswidth = DMA_SLAVE_BUSWIDTH_1_BYTE;

	memset(&conf, 0, sizeof(conf));
	conf.direction = dir;

	if (dir == DMA_DEV_TO_MEM) {
		conf.src_addr = sfspi->sspdr_phys;
		conf.src_addr_width = buswidth;
	} else {
		conf.dst_addr = sfspi->sspdr_phys;
		conf.dst_addr_width = buswidth;
	}

	ret = dmaengine_slave_config(chan, &conf);
	if (ret) {
		dev_dbg(sfspi->dev, "dma %s config fail!\n",
			dma_chan_name(chan));
		return ERR_PTR(ret);
	}

	txd = dmaengine_prep_slave_sg(chan, sgt->sgl, sgt->nents, dir,
				      DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!txd) {
		dev_dbg(sfspi->dev, "dma %s prep fail!\n", dma_chan_name(chan));
		return ERR_PTR(-ENOMEM);
	}
	return txd;
}

static void sfax8_spi_dma_callback(void *callback_param)
{
       struct sfax8_spi *sfspi = callback_param;

       sfax8_spi_disable(sfspi);
       spi_finalize_current_transfer(sfspi->master);
}

static int sfax8_spi_dma_transfer(struct sfax8_spi *sfspi)
{
	struct spi_transfer *t = sfspi->current_xfer;
	struct dma_async_tx_descriptor *rxd, *txd;
	dma_cookie_t cookie;
	int ret;

	if ((!t->rx_sg.sgl) || (!t->tx_sg.sgl)) {
		ret = -EAGAIN;
		goto no_dma;
	}

	rxd = sfax8_spi_dma_prepare(sfspi, sfspi->dma_rx_ch,
			&t->rx_sg, DMA_DEV_TO_MEM);
	if (IS_ERR(rxd)) {
		dev_err(sfspi->dev, "DMA RX failed: %ld\n", PTR_ERR(rxd));
		ret = -EAGAIN;
		goto no_dma_rx;
	}

	txd = sfax8_spi_dma_prepare(sfspi, sfspi->dma_tx_ch,
			&t->tx_sg, DMA_MEM_TO_DEV);
	if (IS_ERR(txd)) {
		dev_err(sfspi->dev, "DMA TX failed: %ld\n", PTR_ERR(txd));
		ret = -EAGAIN;
		goto no_dma_tx;
	}

	/* We are ready when RX is done */
	rxd->callback = sfax8_spi_dma_callback;
	rxd->callback_param = sfspi;

	/* Now submit both descriptors and wait while they finish */
	cookie = dmaengine_submit(rxd);
	if (dma_submit_error(cookie)) {
		ret = cookie;
		goto no_dma_rx;
	}
	cookie = dmaengine_submit(txd);
	if (dma_submit_error(cookie)) {
		ret = cookie;
		goto no_dma_tx;
	}

	dma_async_issue_pending(sfspi->dma_rx_ch);
	dma_async_issue_pending(sfspi->dma_tx_ch);

	if (RANDOM_FALSE())
		return -ETIMEDOUT;

	return t->len;

no_dma_tx:
	if (rxd)
		dmaengine_terminate_all(sfspi->dma_rx_ch);
no_dma_rx:
no_dma:
	if (ret == -EAGAIN) {
		pr_warn_once("%s %s: DMA not available, falling back to PIO\n",
			     dev_driver_string(&sfspi->master->dev),
			     dev_name(&sfspi->master->dev));
	}
	return ret;
}
#endif

/**
 * sfax8_spi_process_transfer() - processes one SPI transfer
 * @sfspi: sfax8 SPI controller struct
 * @t: transfer to process
 *
 * This function processes one SPI transfer given in @t. Function waits until
 * transfer is complete (may sleep) and updates @msg->status based on whether
 * transfer was successfully processed or not.
 */
static int sfax8_spi_process_transfer(struct spi_master *master,
					struct spi_device *spi,
					struct spi_transfer *t)
{
	struct sfax8_spi *sfspi = spi_master_get_devdata(master);
	unsigned long timeout;
	int err;

	sfspi->current_xfer = t;

	sfax8_add_transfer_stats(sfspi, t);

	err = sfax8_spi_reg_setup(sfspi, spi, t->speed_hz, t->bits_per_word);
	if (err) {
		dev_err(sfspi->dev,
			"failed to setup chip for transfer\n");
		return err;
	}

#ifdef CONFIG_DMADEVICES
	if (master->can_dma && master->can_dma(master, spi, t))
		sfspi->use_dma = 1;
	else
		sfspi->use_dma = 0;

	/* DMACR should be set before spi_enable */
	sfax8_spi_enable_dma(sfspi);
#endif

	err = sfax8_spi_enable(sfspi);
	if (err) {
		dev_err(sfspi->dev, "failed to enable SPI controller\n");
		return err;
	}

	/* Interrupt status can only be cleared when spi is enabled */
	sfax8_spi_write_u8(sfspi, SSPICR, RORIC | RTIC);

	/*
	 * Just to be sure: flush any data from RX FIFO.
	 */
	timeout = jiffies + msecs_to_jiffies(SPI_TIMEOUT);
	while (sfax8_spi_read_u16(sfspi, SSPSR) & SSPSR_RNE) {
		if (time_after(jiffies, timeout)) {
			dev_warn(sfspi->dev,
					"timeout while flushing RX FIFO\n");
			return -ETIMEDOUT;
		}
		sfax8_spi_read_u16(sfspi, SSPDR);
	}

	sfspi->rx = 0;
	sfspi->tx = 0;

	/*
	 * We explicitly handle FIFO level. This way we don't have to check TX
	 * FIFO status using %SSPSR_TNF bit which may cause RX FIFO overruns.
	 */
	sfspi->fifo_level = 0;

#ifdef CONFIG_DMADEVICES
	/*
	 * If DMA transfer failed, we try to use pio transfer.
	 */
	if (sfspi->use_dma) {
		err = sfax8_spi_dma_transfer(sfspi);

		if (err != -EAGAIN)
			return err;
	}
#endif

	err = sfax8_spi_pio_transfer(sfspi);
	return err;
}

static irqreturn_t sfax8_spi_interrupt(int irq, void *dev_id)
{
	struct sfax8_spi *sfspi = dev_id;
	u8 irq_status = sfax8_spi_read_u8(sfspi, SSPMIS);

	/*
	 * If we got ROR (receive overrun) interrupt we know that something is
	 * wrong. Just abort the transfer.
	 */
	if (unlikely(irq_status & SSPMIS_RORMIS)) {
		/* clear the overrun interrupt */
		sfax8_spi_write_u8(sfspi, SSPICR, SSPIMSCR_RORIM);
		dev_warn(sfspi->dev,
			 "receive overrun, aborting the transfer\n");
	} else {
		/*
		 * Interrupt is either RX (RIS) or TX (TIS). For both cases we
		 * simply execute next data transfer.
		 */
		if (sfax8_spi_read_write(sfspi)) {
			/*
			 * In normal case, there still is some processing left
			 * for current transfer. Let's wait for the next
			 * interrupt then.
			 */
			return IRQ_HANDLED;
		}
	}

	/*
	 * Current transfer is finished, either with error or with success. In
	 * any case we disable interrupts and notify the worker to handle
	 * any post-processing of the message.
	 */
	sfax8_spi_disable_interrupts(sfspi);
	sfax8_spi_disable(sfspi);
	spi_finalize_current_transfer(sfspi->master);
	return IRQ_HANDLED;
}

#ifdef CONFIG_DMADEVICES
static bool sfax8_spi_can_dma(struct spi_master *master, struct spi_device *spi,
			 struct spi_transfer *xfer)
{
	/*
	 * JFFS2 filesystem may give a buffer address which is on the stack, this
	 * is not DMA safe. This means this transfer may destory the stack data,
	 * don't use DMA in this case.
	 */
	if (object_is_on_stack(xfer->rx_buf))
		return false;
	/*
	 * There is no point of setting up DMA for the transfers which will
	 * fit into the FIFO and can be transferred with a single interrupt.
	 * So in these cases we will be using PIO and don't bother for DMA.
	 */
	return xfer->len > SPI_FIFO_SIZE;
}

static int sfax8_dma_autoprobe(struct sfax8_spi *sfspi)
{
	struct device *dev = &sfspi->master->dev;
	struct spi_master *master = sfspi->master;
	int ret;

	/* automatically configure DMA channels from device tree */
	sfspi->dma_rx_ch = dma_request_slave_channel(dev, "rx");
	if (!sfspi->dma_rx_ch) {
		ret = -ENODEV;
		dev_dbg(sfspi->dev, "DMA request rx channel fail\n");
		goto err_no_rxchan;
	}
	dev_dbg(sfspi->dev, "DMA channel RX %s\n",
		dma_chan_name(sfspi->dma_rx_ch));

	sfspi->dma_tx_ch = dma_request_slave_channel(dev, "tx");
	if (!sfspi->dma_tx_ch) {
		ret = -ENODEV;
		dev_dbg(sfspi->dev, "DMA request tx channel fail\n");
		goto err_no_txchan;
	}
	dev_dbg(sfspi->dev, "DMA channel TX %s\n",
		dma_chan_name(sfspi->dma_tx_ch));

	if (sfspi->dma_rx_ch && sfspi->dma_tx_ch) {
		master->can_dma = sfax8_spi_can_dma;
		master->dma_tx = sfspi->dma_tx_ch;
		master->dma_rx = sfspi->dma_rx_ch;
		master->max_dma_len = MAX_DMA_BD_BYTES;
	}

	dev_info(sfspi->dev, "DMA available");
	return 0;

err_no_txchan:
	dma_release_channel(sfspi->dma_rx_ch);
	sfspi->dma_rx_ch = NULL;
err_no_rxchan:
	return ret;
}

static void sfax8_spi_release_dma(struct sfax8_spi *sfspi)
{
	if (sfspi->dma_rx_ch)
		dma_release_channel(sfspi->dma_rx_ch);
	if (sfspi->dma_tx_ch)
		dma_release_channel(sfspi->dma_tx_ch);
}
#else
static int sfax8_dma_autoprobe(struct sfax8_spi *sfspi) { return 0; }

static void sfax8_spi_release_dma(struct sfax8_spi *sfspi) { return; }
#endif

static int sfax8_spi_probe(struct platform_device *pdev)
{
	struct spi_master *master;
	struct sfax8_spi *sfspi;
	struct device_node *nc, *grf_node;
	struct resource *res;
	int value;
	int irq;
	int error = 0;
	int i, j;
	int cs;

	dev_info(&pdev->dev, "%s...\n", __func__);

	if (release_reset(SF_SPI_SOFT_RESET))
		return -EFAULT;

	nc = pdev->dev.of_node;
	if (!nc)
		return -ENODEV;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "failed to get irq resources\n");
		return -EBUSY;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "unable to get iomem resource\n");
		return -ENODEV;
	}

	master = spi_alloc_master(&pdev->dev, sizeof(*sfspi));
	if (!master)
		return -ENOMEM;

	if (of_property_read_u32(nc, "num-cs", &value)) {
		dev_err(&pdev->dev, "%s has no valid 'num-cs' property\n",
				nc->full_name);
		goto fail_release_master;
	}
	master->num_chipselect = value;
	master->dev.of_node = pdev->dev.of_node;

	master->setup = sfax8_spi_setup;
	master->cleanup = sfax8_spi_cleanup;
	master->transfer_one = sfax8_spi_process_transfer;
	master->prepare_message = sfax8_spi_prepare_message;
	master->unprepare_message = sfax8_spi_unprepare_message;
	master->handle_err = sfax8_handle_err;
	//master->prepare_transfer_hardware = sfax8_enable_clk_gate;
	//master->unprepare_transfer_hardware = sfax8_disable_clk_gate;
	master->bus_num = pdev->id;
	master->mode_bits = SPI_MODE_3 | SPI_LOOP;
	master->flags = SPI_MASTER_MUST_RX | SPI_MASTER_MUST_TX;
	master->bits_per_word_mask = SPI_BPW_RANGE_MASK(4, 16);

	platform_set_drvdata(pdev, master);

	sfspi = spi_master_get_devdata(master);
	sfspi->master = master;

	sfspi->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(sfspi->clk)) {
		dev_err(&pdev->dev, "unable to get spi clock\n");
		error = PTR_ERR(&sfspi->clk);
		goto fail_release_master;
	}

	grf_node = of_parse_phandle(nc, "sfgrf", 0);
	sfspi->sys_base = syscon_node_to_regmap(grf_node);
	if (IS_ERR(sfspi->sys_base)) {
		dev_err(&pdev->dev, "Unable to get system control base\n");
		error = PTR_ERR(sfspi->sys_base);
		goto fail_release_master;
	}

	init_completion(&sfspi->wait);

	/*
	 * Calculate maximum and minimum supported clock rates
	 * for the controller.
	 */
	master->max_speed_hz = clk_get_rate(sfspi->clk) / 2;
	master->min_speed_hz = clk_get_rate(sfspi->clk) / (254 * 256);

	sfspi->dev = &pdev->dev;
	sfspi->sspdr_phys = res->start + SSPDR;

	sfspi->regs_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(sfspi->regs_base)) {
		error = PTR_ERR(sfspi->regs_base);
		goto fail_release_master;
	}

	error = devm_request_irq(&pdev->dev, irq, sfax8_spi_interrupt,
				0, "sfax8-spi", sfspi);
	if (error) {
		dev_err(&pdev->dev, "failed to request irq\n");
		goto fail_release_master;
	}
	sfspi->irq_num = irq;

	if (of_find_property(nc, "use-dma", NULL) && sfax8_dma_autoprobe(sfspi))
		dev_warn(&pdev->dev, "DMA setup failed. Falling back to PIO\n");

	/* make sure that the hardware is disabled */
	sfax8_spi_write_u8(sfspi, SSPCR1, 0);

	for (i = 0; i < master->num_chipselect; i++) {
		cs = of_get_named_gpio(nc, "cs-gpios", i);

		if (!gpio_is_valid(cs)) {
			error = -EINVAL;
			dev_err(&pdev->dev, "cs_gpio(%d) is not valid\n", cs);
			goto fail_free_gpio;
		}

		error = gpio_request((cs), NULL);
		if (error) {
			dev_err(&pdev->dev, "Failed to request gpio %d\n", cs);
			goto fail_free_gpio;
		}
	}

	/* add statistics */
	if (sfax8_init_statistics(sfspi))
		dev_err(&pdev->dev, "failed to creat sfax8 spi attributes\n");

#ifndef CONFIG_SFAX8_SPI_DYNAMIC_SETUP
	sfax8_spi_default_setup(sfspi);

	regmap_write(sfspi->sys_base, SSP_FSSOUT_NTXDOE_SEL, 0x0);
#endif
	error = devm_spi_register_master(&pdev->dev, master);
	if (error) {
		dev_err(&pdev->dev, "failed to register SPI master\n");
		goto fail_free_gpio;
	}

	sfax8_enable_clk_gate(master);
	dev_info(&pdev->dev, "SFAx8 SPI Controller at 0x%08lx irq %d\n",
		 (unsigned long)res->start, irq);

	return 0;

fail_free_gpio:
	for (j = 0; j < i; j++) {
		cs = of_get_named_gpio(nc, "cs-gpios", i);
		gpio_free(cs);
	}
	sfax8_spi_release_dma(sfspi);
fail_release_master:
	spi_master_put(master);

	return error;
}

static int sfax8_spi_remove(struct platform_device *pdev)
{
	struct spi_master *master = platform_get_drvdata(pdev);
	struct sfax8_spi *sfspi = spi_master_get_devdata(master);
	int i = 0, cs;

	for (i = 0; i < master->num_chipselect; i++) {
		cs = of_get_named_gpio(pdev->dev.of_node, "cs-gpios", i);
		gpio_free(cs);
	}

	sfax8_spi_release_dma(sfspi);
	spi_master_put(master);
	sfax8_disable_clk_gate(master);

	if (hold_reset(SF_SPI_SOFT_RESET))
		return -EFAULT;

	return 0;
}

static const struct of_device_id sfspi_of_match[] = {
	{ .compatible = "siflower,sfax8-spi", },
	{},
};
MODULE_DEVICE_TABLE(of, sfspi_of_match);

static struct platform_driver sfax8_spi_driver = {
	.driver		= {
		.name	= "sfax8-spi",
		.of_match_table = sfspi_of_match,
	},
	.probe		= sfax8_spi_probe,
	.remove		= sfax8_spi_remove,
};
module_platform_driver(sfax8_spi_driver);

MODULE_DESCRIPTION("SFAx8 SPI Controller driver");
MODULE_AUTHOR("Qi Zhang <qi.zhang@siflower.com.cn>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sfax8-spi");
