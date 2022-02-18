/*
 * (C) Copyright 2009
 * Vipin Kumar, ST Micoelectronics, vipin.kumar@st.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <i2c.h>
#include <pci.h>
#include <asm/io.h>
#include "sfax8_i2c.h"

struct sf_scl_sda_cfg {
	u32 ss_hcnt;
	u32 fs_hcnt;
	u32 ss_lcnt;
	u32 fs_lcnt;
	u32 sda_hold;
};


struct sf_i2c {
	struct i2c_regs *regs;
	struct sf_scl_sda_cfg *scl_sda_cfg;
};

#ifdef CONFIG_SYS_I2C_DW_ENABLE_STATUS_UNSUPPORTED
static void sf_i2c_enable(struct i2c_regs *i2c_base, bool enable)
{
	u32 ena = enable ? IC_ENABLE_0B : 0;

	writel(ena, &i2c_base->ic_enable);
}
#else
static void sf_i2c_enable(struct i2c_regs *i2c_base, bool enable)
{
	u32 ena = enable ? IC_ENABLE_0B : 0;
	int timeout = 100;

	do {
		writel(ena, &i2c_base->ic_enable);
		if ((readl(&i2c_base->ic_enable_status) & IC_ENABLE_0B) == ena)
			return;

		/*
		 * Wait 10 times the signaling period of the highest I2C
		 * transfer supported by the driver (for 400KHz this is
		 * 25us) as described in the DesignWare I2C databook.
		 */
		udelay(25);
	} while (timeout--);

	printf("timeout in %sabling I2C adapter\n", enable ? "en" : "dis");
}
#endif

/*
 * i2c_set_bus_speed - Set the i2c speed
 * @speed:	required i2c speed
 *
 * Set the i2c speed.
 */
static unsigned int __sf_i2c_set_bus_speed(struct i2c_regs *i2c_base,
					   struct sf_scl_sda_cfg *scl_sda_cfg,
					   unsigned int speed)
{
	unsigned int cntl;
	unsigned int hcnt, lcnt;
	int i2c_spd;

	if (speed >= I2C_HIGH_SPEED)
		i2c_spd = IC_SPEED_MODE_HIGH;
	else if (speed >= I2C_FAST_SPEED)
		i2c_spd = IC_SPEED_MODE_FAST;
	else
		i2c_spd = IC_SPEED_MODE_STANDARD;

	/* to set speed cltr must be disabled */
	sf_i2c_enable(i2c_base, false);

	cntl = (readl(&i2c_base->ic_con) & (~IC_CON_SPD_MSK));

	switch (i2c_spd) {
	case IC_SPEED_MODE_HIGH:
		cntl |= IC_CON_SPD_SS;
		if (scl_sda_cfg) {
			hcnt = scl_sda_cfg->fs_hcnt;
			lcnt = scl_sda_cfg->fs_lcnt;
		} else {
			hcnt = (IC_CLK * MIN_HS_SCL_HIGHTIME) / NANO_TO_MICRO;
			lcnt = (IC_CLK * MIN_HS_SCL_LOWTIME) / NANO_TO_MICRO;
		}
		writel(hcnt, &i2c_base->ic_hs_scl_hcnt);
		writel(lcnt, &i2c_base->ic_hs_scl_lcnt);
		break;

	case IC_SPEED_MODE_STANDARD:
		cntl |= IC_CON_SPD_SS;
		if (scl_sda_cfg) {
			hcnt = scl_sda_cfg->ss_hcnt;
			lcnt = scl_sda_cfg->ss_lcnt;
		} else {
			hcnt = (IC_CLK * MIN_SS_SCL_HIGHTIME) / NANO_TO_MICRO;
			lcnt = (IC_CLK * MIN_SS_SCL_LOWTIME) / NANO_TO_MICRO;
		}
		writel(hcnt, &i2c_base->ic_ss_scl_hcnt);
		writel(lcnt, &i2c_base->ic_ss_scl_lcnt);
		break;

	case IC_SPEED_MODE_FAST:
	default:
		cntl |= IC_CON_SPD_FS;
		if (scl_sda_cfg) {
			hcnt = scl_sda_cfg->fs_hcnt;
			lcnt = scl_sda_cfg->fs_lcnt;
		} else {
			hcnt = (IC_CLK * MIN_FS_SCL_HIGHTIME) / NANO_TO_MICRO;
			lcnt = (IC_CLK * MIN_FS_SCL_LOWTIME) / NANO_TO_MICRO;
		}
		writel(hcnt, &i2c_base->ic_fs_scl_hcnt);
		writel(lcnt, &i2c_base->ic_fs_scl_lcnt);
		break;
	}

	writel(cntl, &i2c_base->ic_con);

	/* Configure SDA Hold Time if required */
	if (scl_sda_cfg)
		writel(scl_sda_cfg->sda_hold, &i2c_base->ic_sda_config);

	/* Enable back i2c now speed set */
	sf_i2c_enable(i2c_base, true);

	return 0;
}

/*
 * i2c_setaddress - Sets the target slave address
 * @i2c_addr:	target i2c address
 *
 * Sets the target slave address.
 */
static void i2c_setaddress(struct i2c_regs *i2c_base, unsigned int i2c_addr)
{
	/* Disable i2c */
	sf_i2c_enable(i2c_base, false);

	writel(i2c_addr, &i2c_base->ic_tar);

	/* Enable i2c */
	sf_i2c_enable(i2c_base, true);
}

/*
 * i2c_flush_rxfifo - Flushes the i2c RX FIFO
 *
 * Flushes the i2c RX FIFO
 */
static void i2c_flush_rxfifo(struct i2c_regs *i2c_base)
{
	while (readl(&i2c_base->ic_status) & IC_STATUS_RFNE)
		readl(&i2c_base->ic_cmd_data);
}

/*
 * i2c_wait_for_bb - Waits for bus busy
 *
 * Waits for bus busy
 */
static int i2c_wait_for_bb(struct i2c_regs *i2c_base)
{
	unsigned long start_time_bb = get_timer(0);

	while ((readl(&i2c_base->ic_status) & IC_STATUS_MA) ||
	       !(readl(&i2c_base->ic_status) & IC_STATUS_TFE)) {

		/* Evaluate timeout */
		if (get_timer(start_time_bb) > (unsigned long)(I2C_BYTE_TO_BB))
			return 1;
	}

	return 0;
}

static int i2c_xfer_init(struct i2c_regs *i2c_base, uchar chip, uint addr,
			 int alen)
{
	if (i2c_wait_for_bb(i2c_base))
		return 1;

	i2c_setaddress(i2c_base, chip);
	while (alen) {
		alen--;
		/* high byte address going out first */
		writel((addr >> (alen * 8)) & 0xff,
		       &i2c_base->ic_cmd_data);
	}
	return 0;
}

static int i2c_xfer_finish(struct i2c_regs *i2c_base)
{
	ulong start_stop_det = get_timer(0);

	while (1) {
		if ((readl(&i2c_base->ic_raw_intr_stat) & IC_STOP_DET)) {
			readl(&i2c_base->ic_clr_stop_det);
			break;
		} else if (get_timer(start_stop_det) > I2C_STOPDET_TO) {
			break;
		}
	}

	if (i2c_wait_for_bb(i2c_base)) {
		printf("Timed out waiting for bus\n");
		return 1;
	}

	i2c_flush_rxfifo(i2c_base);

	return 0;
}

/*
 * i2c_read - Read from i2c memory
 * @chip:	target i2c address
 * @addr:	address to read from
 * @alen:
 * @buffer:	buffer for read data
 * @len:	no of bytes to be read
 *
 * Read from i2c memory.
 */
static int __sf_i2c_read(struct i2c_regs *i2c_base, u8 dev, uint addr,
			 int alen, u8 *buffer, int len)
{
	unsigned long start_time_rx;

#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	dev |= ((addr >> (alen * 8)) & CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
	addr &= ~(CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW << (alen * 8));

	debug("%s: fix addr_overflow: dev %02x addr %02x\n", __func__, dev,
	      addr);
#endif

	if (i2c_xfer_init(i2c_base, dev, addr, alen))
		return 1;

	start_time_rx = get_timer(0);
	while (len) {
		writel(IC_CMD, &i2c_base->ic_cmd_data);

		if (readl(&i2c_base->ic_status) & IC_STATUS_RFNE) {
			*buffer++ = (uchar)readl(&i2c_base->ic_cmd_data);
			len--;
			start_time_rx = get_timer(0);

		} else if (get_timer(start_time_rx) > I2C_BYTE_TO) {
				return 1;
		}
	}

	return i2c_xfer_finish(i2c_base);
}

/*
 * i2c_write - Write to i2c memory
 * @chip:	target i2c address
 * @addr:	address to read from
 * @alen:
 * @buffer:	buffer for read data
 * @len:	no of bytes to be read
 *
 * Write to i2c memory.
 */
static int __sf_i2c_write(struct i2c_regs *i2c_base, u8 dev, uint addr,
			  int alen, u8 *buffer, int len)
{
	int nb = len;
	unsigned long start_time_tx;

#ifdef CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	dev |= ((addr >> (alen * 8)) & CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW);
	addr &= ~(CONFIG_SYS_I2C_EEPROM_ADDR_OVERFLOW << (alen * 8));

	debug("%s: fix addr_overflow: dev %02x addr %02x\n", __func__, dev,
	      addr);
#endif

	if (i2c_xfer_init(i2c_base, dev, addr, alen))
		return 1;

	start_time_tx = get_timer(0);
	while (len) {
		if (readl(&i2c_base->ic_status) & IC_STATUS_TFNF) {
			if (--len == 0) {
				writel(*buffer, &i2c_base->ic_cmd_data);
			} else {
				writel(*buffer, &i2c_base->ic_cmd_data);
			}
			buffer++;
			start_time_tx = get_timer(0);

		} else if (get_timer(start_time_tx) > (nb * I2C_BYTE_TO)) {
				printf("Timed out. i2c write Failed\n");
				return 1;
		}
	}

	return i2c_xfer_finish(i2c_base);
}

/*
 * __sf_i2c_init - Init function
 * @speed:	required i2c speed
 * @slaveaddr:	slave address for the device
 *
 * Initialization function.
 */
static void __sf_i2c_init(struct i2c_regs *i2c_base, int speed, int slaveaddr)
{
	/* Disable i2c */
	sf_i2c_enable(i2c_base, false);
	int mode;
	mode = slaveaddr;
	if(1 == mode)
		writel((IC_CON_RE | IC_CON_SD | IC_CON_SPD_FS | IC_CON_MM), &i2c_base->ic_con);
	else
		writel((IC_CON_SD | IC_CON_SPD_FS | IC_CON_SL), &i2c_base->ic_con);
	writel(IC_RX_TL, &i2c_base->ic_rx_tl);
	writel(IC_TX_TL, &i2c_base->ic_tx_tl);
	writel(IC_STOP_DET, &i2c_base->ic_intr_mask);
	/* Enable i2c */
	sf_i2c_enable(i2c_base, true);
}

/* The DM I2C functions */

static int siflower_i2c_xfer(struct udevice *bus, struct i2c_msg *msg,
			       int nmsgs)
{
	struct sf_i2c *i2c = dev_get_priv(bus);
	int ret;

	debug("i2c_xfer: %d messages\n", nmsgs);
	for (; nmsgs > 0; nmsgs--, msg++) {
		debug("i2c_xfer: chip=0x%x, len=0x%x\n", msg->addr, msg->len);
		if (msg->flags & I2C_M_RD) {
			ret = __sf_i2c_read(i2c->regs, msg->addr, 0, 0,
					    msg->buf, msg->len);
		} else {
			ret = __sf_i2c_write(i2c->regs, msg->addr, 0, 0,
					     msg->buf, msg->len);
		}
		if (ret) {
			debug("i2c_write: error sending\n");
			return -EREMOTEIO;
		}
	}

	return 0;
}

static int siflower_i2c_set_bus_speed(struct udevice *bus, unsigned int speed)
{
	struct sf_i2c *i2c = dev_get_priv(bus);

	return __sf_i2c_set_bus_speed(i2c->regs, i2c->scl_sda_cfg, speed);
}

static int siflower_i2c_probe_chip(struct udevice *bus, uint chip_addr,
				     uint chip_flags)
{
	struct sf_i2c *i2c = dev_get_priv(bus);
	struct i2c_regs *i2c_base = i2c->regs;
	u32 tmp;
	int ret;

	/* Try to read the first location of the chip */
	ret = __sf_i2c_read(i2c_base, chip_addr, 0, 1, (uchar *)&tmp, 1);
	if (ret)
		__sf_i2c_init(i2c_base, 0, 0);

	return ret;
}

static int siflower_i2c_probe(struct udevice *bus)
{
	struct sf_i2c *priv = dev_get_priv(bus);

	if (device_is_on_pci_bus(bus)) {
#ifdef CONFIG_DM_PCI
		/* Save base address from PCI BAR */
		priv->regs = (struct i2c_regs *)
			dm_pci_map_bar(bus, PCI_BASE_ADDRESS_0, PCI_REGION_MEM);
#endif
	} else {
		priv->regs = (struct i2c_regs *)dev_get_addr_ptr(bus);
	}

	__sf_i2c_init(priv->regs, 0, 1);

	return 0;
}

static int siflower_i2c_bind(struct udevice *dev)
{
	static int num_cards;
	char name[20];

	/* Create a unique device name for PCI type devices */
	if (device_is_on_pci_bus(dev)) {
		/*
		 * ToDo:
		 * Setting req_seq in the driver is probably not recommended.
		 * But without a DT alias the number is not configured. And
		 * using this driver is impossible for PCIe I2C devices.
		 * This can be removed, once a better (correct) way for this
		 * is found and implemented.
		 */
		dev->req_seq = num_cards;
		sprintf(name, "i2c_siflower#%u", num_cards++);
		device_set_name(dev, name);
	}

	return 0;
}

static const struct dm_i2c_ops siflower_i2c_ops = {
	.xfer		= siflower_i2c_xfer,
	.probe_chip	= siflower_i2c_probe_chip,
	.set_bus_speed	= siflower_i2c_set_bus_speed,
};

static const struct udevice_id siflower_i2c_ids[] = {
	{ .compatible = "siflower,sfax8-i2c" },
	{ }
};

U_BOOT_DRIVER(i2c_siflower) = {
	.name	= "i2c_siflower",
	.id	= UCLASS_I2C,
	.of_match = siflower_i2c_ids,
	.bind	= siflower_i2c_bind,
	.probe	= siflower_i2c_probe,
	.priv_auto_alloc_size = sizeof(struct sf_i2c),
	.ops	= &siflower_i2c_ops,
};
