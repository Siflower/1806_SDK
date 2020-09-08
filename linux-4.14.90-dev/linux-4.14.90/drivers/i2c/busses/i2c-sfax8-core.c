/*
 * Siflower I2C adapter driver (master only).
 *
 * Based on the Synopsys DesignWare I2C adapter driver.
 *
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * ----------------------------------------------------------------------------
 *
 */
#include <linux/export.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/pm_runtime.h>
#include <linux/delay.h>
#include <linux/module.h>
#include "i2c-sfax8-core.h"
#include <linux/uaccess.h>

/*
 * Registers offset
 */
#define SF_IC_CON		0x0
#define SF_IC_TAR		0x4
#define SF_IC_DATA_CMD		0x10
#define SF_IC_SS_SCL_HCNT	0x14
#define SF_IC_SS_SCL_LCNT	0x18
#define SF_IC_FS_SCL_HCNT	0x1c
#define SF_IC_FS_SCL_LCNT	0x20
#define SF_IC_HS_SCL_HCNT	0x24
#define SF_IC_HS_SCL_LCNT	0x28
#define SF_IC_INTR_STAT		0x2c
#define SF_IC_INTR_MASK		0x30
#define SF_IC_RAW_INTR_STAT	0x34
#define SF_IC_RX_TL		0x38
#define SF_IC_TX_TL		0x3c
#define SF_IC_CLR_INTR		0x40
#define SF_IC_CLR_RX_UNDER	0x44
#define SF_IC_CLR_RX_OVER	0x48
#define SF_IC_CLR_TX_OVER	0x4c
#define SF_IC_CLR_RD_REQ	0x50
#define SF_IC_CLR_TX_ABRT	0x54
#define SF_IC_CLR_RX_DONE	0x58
#define SF_IC_CLR_ACTIVITY	0x5c
#define SF_IC_CLR_STOP_DET	0x60
#define SF_IC_CLR_START_DET	0x64
#define SF_IC_CLR_GEN_CALL	0x68
#define SF_IC_ENABLE		0x6c
#define SF_IC_STATUS		0x70
#define SF_IC_TXFLR		0x74
#define SF_IC_RXFLR		0x78
#define SF_IC_SDA_HOLD		0x7c
#define SF_IC_TX_ABRT_SOURCE	0x80
#define SF_IC_ENABLE_STATUS	0x9c
#define SF_IC_CFG    0xa0
/*
#define SF_IC_COMP_PARAM_1	0xf4
#define SF_IC_COMP_VERSION	0xf8
#define SF_IC_SDA_HOLD_MIN_VERS	0x3131312A
#define SF_IC_COMP_TYPE		0xfc
#define SF_IC_COMP_TYPE_VALUE	0x44570140
*/
#define SF_IC_INTR_RX_UNDER	0x001
#define SF_IC_INTR_RX_OVER	0x002
#define SF_IC_INTR_RX_FULL	0x004
#define SF_IC_INTR_TX_OVER	0x008
#define SF_IC_INTR_TX_EMPTY	0x010
#define SF_IC_INTR_RD_REQ	0x020
#define SF_IC_INTR_TX_ABRT	0x040
#define SF_IC_INTR_RX_DONE	0x080
#define SF_IC_INTR_ACTIVITY	0x100
#define SF_IC_INTR_STOP_DET	0x200
#define SF_IC_INTR_START_DET	0x400
#define SF_IC_INTR_GEN_CALL	0x800

#define SF_IC_INTR_DEFAULT_MASK		(SF_IC_INTR_RX_FULL | \
					 SF_IC_INTR_TX_EMPTY | \
					 SF_IC_INTR_TX_ABRT | \
					 SF_IC_INTR_STOP_DET)

#define SF_IC_STATUS_ACTIVITY	0x1

#define SF_IC_ERR_TX_ABRT	0x1

#define SF_IC_TAR_10BITADDR_MASTER BIT(12)

/*
 * status codes
 */
#define STATUS_IDLE			0x0
#define STATUS_WRITE_IN_PROGRESS	0x1
#define STATUS_READ_IN_PROGRESS		0x2

#define TIMEOUT			20 /* ms */

/*
 * hardware abort codes from the SF_IC_TX_ABRT_SOURCE register
 *
 * only expected abort codes are listed here
 * refer to the datasheet for the full list
 */
#define ABRT_7B_ADDR_NOACK	0
#define ABRT_10ADDR1_NOACK	1
#define ABRT_10ADDR2_NOACK	2
#define ABRT_TXDATA_NOACK	3
#define ABRT_GCALL_NOACK	4
#define ABRT_GCALL_READ		5
#define ABRT_SBYTE_ACKDET	7
#define ABRT_SBYTE_NORSTRT	9
#define ABRT_10B_RD_NORSTRT	10
#define ABRT_MASTER_DIS		11
#define ARB_LOST		12

#define SF_IC_TX_ABRT_7B_ADDR_NOACK	(1UL << ABRT_7B_ADDR_NOACK)
#define SF_IC_TX_ABRT_10ADDR1_NOACK	(1UL << ABRT_10ADDR1_NOACK)
#define SF_IC_TX_ABRT_10ADDR2_NOACK	(1UL << ABRT_10ADDR2_NOACK)
#define SF_IC_TX_ABRT_TXDATA_NOACK	(1UL << ABRT_TXDATA_NOACK)
#define SF_IC_TX_ABRT_GCALL_NOACK	(1UL << ABRT_GCALL_NOACK)
#define SF_IC_TX_ABRT_GCALL_READ	(1UL << ABRT_GCALL_READ)
#define SF_IC_TX_ABRT_SBYTE_ACKDET	(1UL << ABRT_SBYTE_ACKDET)
#define SF_IC_TX_ABRT_SBYTE_NORSTRT	(1UL << ABRT_SBYTE_NORSTRT)
#define SF_IC_TX_ABRT_10B_RD_NORSTRT	(1UL << ABRT_10B_RD_NORSTRT)
#define SF_IC_TX_ABRT_MASTER_DIS	(1UL << ABRT_MASTER_DIS)
#define SF_IC_TX_ARB_LOST		(1UL << ARB_LOST)

#define SF_IC_TX_ABRT_NOACK		(SF_IC_TX_ABRT_7B_ADDR_NOACK | \
					 SF_IC_TX_ABRT_10ADDR1_NOACK | \
					 SF_IC_TX_ABRT_10ADDR2_NOACK | \
					 SF_IC_TX_ABRT_TXDATA_NOACK | \
					 SF_IC_TX_ABRT_GCALL_NOACK)

static char *abort_sources[] = {
	[ABRT_7B_ADDR_NOACK] =
		"slave address not acknowledged (7bit mode)",
	[ABRT_10ADDR1_NOACK] =
		"first address byte not acknowledged (10bit mode)",
	[ABRT_10ADDR2_NOACK] =
		"second address byte not acknowledged (10bit mode)",
	[ABRT_TXDATA_NOACK] =
		"data not acknowledged",
	[ABRT_GCALL_NOACK] =
		"no acknowledgement for a general call",
	[ABRT_GCALL_READ] =
		"read after general call",
	[ABRT_SBYTE_ACKDET] =
		"start byte acknowledged",
	[ABRT_SBYTE_NORSTRT] =
		"trying to send start byte when restart is disabled",
	[ABRT_10B_RD_NORSTRT] =
		"trying to read when restart is disabled (10bit mode)",
	[ABRT_MASTER_DIS] =
		"trying to use disabled adapter",
	[ARB_LOST] =
		"lost arbitration",
};
/*
* we can recognize i2c  by the base address.
*  when we do not use the i2c, we can the i2c clk gate.
*/
void set_i2c_clk_gate(u32 base, bool on)
{
	int num, tmp;
	num = (base & 0x3000) >> 12;
	tmp = get_module_clk_gate(SF_IIC_SOFT_RESET, false);
	if(on)
		tmp = tmp | (1 << num);
	else
		tmp = tmp & (~(1 << num));
	set_module_clk_gate(SF_IIC_SOFT_RESET, tmp, false);
}
EXPORT_SYMBOL(set_i2c_clk_gate);

u16 sf_readw(struct sf_i2c_dev *dev, int offset)
{
	return readw(dev->base + offset);
}

void sf_writew(struct sf_i2c_dev *dev, u16 b, int offset)
{
	writew(b, dev->base + offset);
}

static void __i2c_sf_enable(struct sf_i2c_dev *dev, bool enable)
{
	int timeout = 100;
	do {
		sf_writew(dev, enable, SF_IC_ENABLE);
		if ((sf_readw(dev, SF_IC_ENABLE_STATUS) & 1) == enable)
			return;
		/*
		 * Wait 10 times the signaling period of the highest I2C
		 * transfer supported by the driver (for 400KHz this is
		 * 25us) as described in the Siflower I2C databook.
		 */
		usleep_range(25, 250);
	} while (timeout--);

	dev_warn(dev->dev, "timeout in %sabling adapter\n",
		 enable ? "en" : "dis");
}

/**
 * i2c_sf_init() - initialize the siflower i2c master hardware
 * @dev: device private data
 *
 * This functions configures and enables the I2C master.
 * This function is called during I2C init function, and in case of timeout at
 * run time.
 */
int i2c_sf_init(struct sf_i2c_dev *dev)
{
	u32 input_clock_khz;
	u16 hcnt, lcnt;
	//u32 sda_falling_time, scl_falling_time;

	input_clock_khz = dev->get_clk_rate_khz(dev);
	/* Disable the adapter */
	__i2c_sf_enable(dev, false);
	/* set standard and fast speed deviders for high/low periods */

	/* Standard-mode */
	hcnt =(u16) ((input_clock_khz / 100 / 2) + 1 ) - 8;
	lcnt =(u16)(input_clock_khz / 100 / 2) + 1 ;
	/* Allow platforms to specify the ideal HCNT and LCNT values */
	if (dev->ss_hcnt && dev->ss_lcnt) {
		hcnt = dev->ss_hcnt;
		lcnt = dev->ss_lcnt;
	}
	sf_writew(dev, hcnt, SF_IC_SS_SCL_HCNT);
	sf_writew(dev, lcnt, SF_IC_SS_SCL_LCNT);
	dev_dbg(dev->dev, "Standard-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);

	/* Fast-mode */
	hcnt = (u16)((input_clock_khz / 400 / 2) + 1 ) - 8;
	lcnt = (u16)(input_clock_khz / 400 / 2) + 1 ;

	if (dev->fs_hcnt && dev->fs_lcnt) {
		hcnt = dev->fs_hcnt;
		lcnt = dev->fs_lcnt;
	}
	sf_writew(dev, hcnt, SF_IC_FS_SCL_HCNT);
	sf_writew(dev, lcnt, SF_IC_FS_SCL_LCNT);
	dev_dbg(dev->dev, "Fast-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);

	/* High-mode */
	hcnt = (u16)((input_clock_khz / 3400 / 2) + 1 ) - 8;
	lcnt = (u16)(input_clock_khz / 3400 / 2) + 1 ;

	if (dev->fs_hcnt && dev->fs_lcnt) {
		hcnt = dev->fs_hcnt;
		lcnt = dev->fs_lcnt;
	}
	sf_writew(dev, hcnt, SF_IC_HS_SCL_HCNT);
	sf_writew(dev, lcnt, SF_IC_HS_SCL_LCNT);
	dev_dbg(dev->dev, "Fast-mode HCNT:LCNT = %d:%d\n", hcnt, lcnt);

	/* Configure Tx/Rx FIFO threshold levels */
	sf_writew(dev, dev->tx_fifo_depth / 2, SF_IC_TX_TL);
	sf_writew(dev, 0, SF_IC_RX_TL);
	/* configure the i2c master */
	sf_writew(dev, dev->master_cfg , SF_IC_CON);


	/*ignore no ack in high speed*/
	if((dev->master_cfg & SF_IC_CON_SPEED_HIGH) == SF_IC_CON_SPEED_HIGH)
		sf_writew(dev, 0x100 | sf_readw(dev, SF_IC_CFG), SF_IC_CFG);

	return 0;
}
EXPORT_SYMBOL_GPL(i2c_sf_init);

/*
 * Waiting for bus not busy
 */
static int i2c_sf_wait_bus_not_busy(struct sf_i2c_dev *dev)
{
	int timeout = TIMEOUT;
	while (sf_readw(dev, SF_IC_STATUS) & SF_IC_STATUS_ACTIVITY) {
		if (timeout <= 0) {
			dev_warn(dev->dev, "timeout waiting for bus ready\n");
			return -ETIMEDOUT;
		}
		timeout--;
		usleep_range(1000, 1100);
	}
	return 0;
}

static void i2c_sf_xfer_init(struct sf_i2c_dev *dev)
{
	struct i2c_msg *msgs = dev->msgs;
	u32 ic_con, ic_tar = 0;

	/* Disable the adapter */
	__i2c_sf_enable(dev, false);

	/* if the slave address is ten bit address, enable 10BITADDR */
	ic_con = sf_readw(dev, SF_IC_CON);
	if (msgs[dev->msg_write_idx].flags & I2C_M_TEN) {
		ic_con |= SF_IC_CON_10BITADDR_MASTER;
		/*
		 * If I2C_DYNAMIC_TAR_UPDATE is set, the 10-bit addressing
		 * mode has to be enabled via bit 12 of IC_TAR register.
		 * We set it always as I2C_DYNAMIC_TAR_UPDATE can't be
		 * detected from registers.
		 */
		ic_tar = SF_IC_TAR_10BITADDR_MASTER;
	} else {
		ic_con &= ~SF_IC_CON_10BITADDR_MASTER;
	}

	sf_writew(dev, ic_con, SF_IC_CON);

	/*
	 * Set the slave (target) address and enable 10-bit addressing mode
	 * if applicable.
	 */
	sf_writew(dev, msgs[dev->msg_write_idx].addr | ic_tar, SF_IC_TAR);

	/* enforce disabled interrupts (due to HW issues) */
	i2c_sf_disable_int(dev);
	/* Enable the adapter */
	__i2c_sf_enable(dev, true);


	/* Clear and enable interrupts */
	i2c_sf_clear_int(dev);
	sf_writew(dev, SF_IC_INTR_DEFAULT_MASK, SF_IC_INTR_MASK);
}

/*
 * Initiate (and continue) low level master read/write transaction.
 * This function is only called from i2c_sf_isr, and pumping i2c_msg
 * messages into the tx buffer.  Even if the size of i2c_msg data is
 * longer than the size of the tx buffer, it handles everything.
 */
static void
i2c_sf_xfer_msg(struct sf_i2c_dev *dev)
{
	struct i2c_msg *msgs = dev->msgs;
	u32 intr_mask;
	u16 tx_limit, rx_limit;
	u32 addr = msgs[dev->msg_write_idx].addr;
	u32 buf_len = dev->tx_buf_len;
	u8 *buf = dev->tx_buf;
	bool need_restart = false;

	intr_mask = SF_IC_INTR_DEFAULT_MASK;

	for (; dev->msg_write_idx < dev->msgs_num; dev->msg_write_idx++) {
		/*
		 * if target address has changed, we need to
		 * reprogram the target address in the i2c
		 * adapter when we are done with this transfer
		 */
		if (msgs[dev->msg_write_idx].addr != addr) {
			dev_err(dev->dev,
				"%s: invalid target address\n", __func__);
			dev->msg_err = -EINVAL;
			break;
		}

		if (msgs[dev->msg_write_idx].len == 0) {
			dev_err(dev->dev,
				"%s: invalid message length\n", __func__);
			dev->msg_err = -EINVAL;
			break;
		}

		if (!(dev->status & STATUS_WRITE_IN_PROGRESS)) {
			/* new i2c_msg */
			buf = msgs[dev->msg_write_idx].buf;
			buf_len = msgs[dev->msg_write_idx].len;

			/* If both IC_EMPTYFIFO_HOLD_MASTER_EN and
			 * IC_RESTART_EN are set, we must manually
			 * set restart bit between messages.
			 */
			if ((dev->master_cfg & SF_IC_CON_RESTART_EN) &&
					(dev->msg_write_idx > 0))
				need_restart = true;
		}

		tx_limit = dev->tx_fifo_depth - sf_readw(dev, SF_IC_TXFLR);
		rx_limit = dev->rx_fifo_depth - sf_readw(dev, SF_IC_RXFLR);
		while (buf_len > 0 && tx_limit > 0 && rx_limit > 0) {
			u16 cmd = 0;

			/*
			 * If IC_EMPTYFIFO_HOLD_MASTER_EN is set we must
			 * manually set the stop bit. However, it cannot be
			 * detected from the registers so we set it always
			 * when writing/reading the last byte.
			 */
			if (dev->msg_write_idx == dev->msgs_num - 1 &&
			    buf_len == 1)
				cmd |= BIT(9);

			if (need_restart) {
				cmd |= BIT(10);
				need_restart = false;
			}

			if (msgs[dev->msg_write_idx].flags & I2C_M_RD) {

				/* avoid rx buffer overrun */
				if (rx_limit - dev->rx_outstanding <= 0)
					break;

				sf_writew(dev, cmd | 0x100, SF_IC_DATA_CMD);
				rx_limit--;
				dev->rx_outstanding++;
			} else
				sf_writew(dev, cmd | *buf++, SF_IC_DATA_CMD);
			tx_limit--; buf_len--;
		}
		dev->tx_buf = buf;
		dev->tx_buf_len = buf_len;

		if (buf_len > 0) {
			/* more bytes to be written */
			dev->status |= STATUS_WRITE_IN_PROGRESS;
			break;
		} else
			dev->status &= ~STATUS_WRITE_IN_PROGRESS;
	}

	/*
	 * If i2c_msg index search is completed, we don't need TX_EMPTY
	 * interrupt any more.
	 */
	if (dev->msg_write_idx == dev->msgs_num)
		intr_mask &= ~SF_IC_INTR_TX_EMPTY;

	if (dev->msg_err)
		intr_mask = 0;
	sf_writew(dev, intr_mask,  SF_IC_INTR_MASK);
}

static void
i2c_sf_read(struct sf_i2c_dev *dev)
{
	struct i2c_msg *msgs = dev->msgs;
	u16 rx_valid;

	for (; dev->msg_read_idx < dev->msgs_num; dev->msg_read_idx++) {
		u32 len;
		u8 *buf;

		if (!(msgs[dev->msg_read_idx].flags & I2C_M_RD))
			continue;

		if (!(dev->status & STATUS_READ_IN_PROGRESS)) {
			len = msgs[dev->msg_read_idx].len;
			buf = msgs[dev->msg_read_idx].buf;
		} else {
			len = dev->rx_buf_len;
			buf = dev->rx_buf;
		}

		rx_valid = sf_readw(dev, SF_IC_RXFLR);

		for (; len > 0 && rx_valid > 0; len--, rx_valid--) {
			*buf++ = sf_readw(dev, SF_IC_DATA_CMD);
			dev->rx_outstanding--;
		}

		if (len > 0) {
			dev->status |= STATUS_READ_IN_PROGRESS;
			dev->rx_buf_len = len;
			dev->rx_buf = buf;
			return;
		} else
			dev->status &= ~STATUS_READ_IN_PROGRESS;
	}
}

static int i2c_sf_handle_tx_abort(struct sf_i2c_dev *dev)
{
	unsigned long abort_source = dev->abort_source;
	int i;

	if (abort_source & SF_IC_TX_ABRT_NOACK) {
		for_each_set_bit(i, &abort_source, ARRAY_SIZE(abort_sources))
			dev_dbg(dev->dev,
				"%s: %s\n", __func__, abort_sources[i]);
		return -EREMOTEIO;
	}

	for_each_set_bit(i, &abort_source, ARRAY_SIZE(abort_sources))
		dev_err(dev->dev, "%s: %s\n", __func__, abort_sources[i]);

	if (abort_source & SF_IC_TX_ARB_LOST)
		return -EAGAIN;
	else if (abort_source & SF_IC_TX_ABRT_GCALL_READ)
		return -EINVAL; /* wrong msgs[] data */
	else
		return -EIO;
}

/*
 * Prepare controller for a transaction and call i2c_sf_xfer_msg
 */
int
i2c_sf_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	struct sf_i2c_dev *dev = i2c_get_adapdata(adap);
	int ret;
	int time_out, speed, i, data_len;

	dev_dbg(dev->dev, "%s: msgs: %d\n", __func__, num);

	if(msgs == NULL){
		dev_dbg(dev->dev, "%s: invalid msgs parameter!", __func__);
		return -EINVAL;
	}

	if(num < 0){
		dev_dbg(dev->dev, "%s: invalid num parameter!", __func__);
		return -EINVAL;
	}
	/*if adapter timeout is defined ,using it ,else time_out increases with transmitted data dynamically*/
	/*adapter timeout is initiaized as HZ*/
	if(adap->timeout != HZ)
		time_out = adap->timeout;
	else{

		if((dev->master_cfg & SF_IC_CON_SPEED_HIGH) == SF_IC_CON_SPEED_HIGH)
			speed = 3400000;
		else if((dev->master_cfg & SF_IC_CON_SPEED_FAST)== SF_IC_CON_SPEED_FAST)
			speed = 400000;
		else if((dev->master_cfg & SF_IC_CON_SPEED_STD) == SF_IC_CON_SPEED_STD)
			speed = 100000;

		else {
			dev_dbg(dev->dev, "%s:unsupport transfer speed",__func__);
			goto done;
		}
		data_len = 0;
		for(i = 0; i < num; i++){
			data_len += msgs[i].len;
		}
		time_out = HZ > (data_len * 20 * 100 / speed)?
			HZ : (data_len * 20 * 100 / speed);
	}

	mutex_lock(&dev->lock);
	pm_runtime_get_sync(dev->dev);

	reinit_completion(&dev->cmd_complete);
	dev->msgs = msgs;
	dev->msgs_num = num;
	dev->cmd_err = 0;
	dev->msg_write_idx = 0;
	dev->msg_read_idx = 0;
	dev->msg_err = 0;
	dev->status = STATUS_IDLE;
	dev->abort_source = 0;
	dev->rx_outstanding = 0;
	set_i2c_clk_gate((u32)dev->base, true);
	ret = i2c_sf_wait_bus_not_busy(dev);
	if (ret < 0)
		goto done;


	/* start the transfers */
	i2c_sf_xfer_init(dev);

	/* wait for tx to complete */
	ret = wait_for_completion_timeout(&dev->cmd_complete, time_out);
	if (ret == 0) {
		dev_err(dev->dev, "controller timed out\n");
		/* i2c_sf_init implicitly disables the adapter */
		i2c_sf_init(dev);
		ret = -ETIMEDOUT;
		goto done;
	}

	/*
	 * We must disable the adapter before unlocking the &dev->lock mutex
	 * below. Otherwise the hardware might continue generating interrupts
	 * which in turn causes a race condition with the following transfer.
	 * Needs some more investigation if the additional interrupts are
	 * a hardware bug or this driver doesn't handle them correctly yet.
	 */
	__i2c_sf_enable(dev, false);

	if (dev->msg_err) {
		ret = dev->msg_err;
		goto done;
	}

	/* no error */
	if (likely(!dev->cmd_err)) {
		ret = num;
		goto done;
	}

	/* We have an error */
	if (dev->cmd_err == SF_IC_ERR_TX_ABRT) {
		ret = i2c_sf_handle_tx_abort(dev);
		goto done;
	}
	ret = -EIO;

done:
	pm_runtime_mark_last_busy(dev->dev);
	pm_runtime_put_autosuspend(dev->dev);
	set_i2c_clk_gate((u32)dev->base, false);	
	mutex_unlock(&dev->lock);

	return ret;
}
EXPORT_SYMBOL_GPL(i2c_sf_xfer);

u32 i2c_sf_func(struct i2c_adapter *adap)
{
	struct sf_i2c_dev *dev = i2c_get_adapdata(adap);
	return dev->functionality;
}
EXPORT_SYMBOL_GPL(i2c_sf_func);

static u32 i2c_sf_read_clear_intrbits(struct sf_i2c_dev *dev)
{
	u16 stat;

	/*
	 * The IC_INTR_STAT register just indicates "enabled" interrupts.
	 * Ths unmasked raw version of interrupt status bits are available
	 * in the IC_RAW_INTR_STAT register.
	 *
	 * That is,
	 *   stat = sf_readw(IC_INTR_STAT);
	 * equals to,
	 *   stat = sf_readw(IC_RAW_INTR_STAT) & sf_readl(IC_INTR_MASK);
	 *
	 * The raw version might be useful for debugging purposes.
	 */
	stat = sf_readw(dev, SF_IC_INTR_STAT);

	/*
	 * Do not use the IC_CLR_INTR register to clear interrupts, or
	 * you'll miss some interrupts, triggered during the period from
	 * sf_readw(IC_INTR_STAT) to sf_readl(IC_CLR_INTR).
	 *
	 * Instead, use the separately-prepared IC_CLR_* registers.
	 */
	if (stat & SF_IC_INTR_RX_UNDER)
		sf_readw(dev, SF_IC_CLR_RX_UNDER);
	if (stat & SF_IC_INTR_RX_OVER)
		sf_readw(dev, SF_IC_CLR_RX_OVER);
	if (stat & SF_IC_INTR_TX_OVER)
		sf_readw(dev, SF_IC_CLR_TX_OVER);
	if (stat & SF_IC_INTR_RD_REQ)
		sf_readw(dev, SF_IC_CLR_RD_REQ);
	if (stat & SF_IC_INTR_TX_ABRT) {
		/*
		 * The IC_TX_ABRT_SOURCE register is cleared whenever
		 * the IC_CLR_TX_ABRT is read.  Preserve it beforehand.
		 */
		dev->abort_source = sf_readw(dev, SF_IC_TX_ABRT_SOURCE);
		sf_readw(dev, SF_IC_CLR_TX_ABRT);
	}
	if (stat & SF_IC_INTR_RX_DONE)
		sf_readw(dev, SF_IC_CLR_RX_DONE);
	if (stat & SF_IC_INTR_ACTIVITY)
		sf_readw(dev, SF_IC_CLR_ACTIVITY);
	if (stat & SF_IC_INTR_STOP_DET)
		sf_readw(dev, SF_IC_CLR_STOP_DET);
	if (stat & SF_IC_INTR_START_DET)
		sf_readw(dev, SF_IC_CLR_START_DET);
	if (stat & SF_IC_INTR_GEN_CALL)
		sf_readw(dev, SF_IC_CLR_GEN_CALL);

	return stat;
}

/*
 * Interrupt service routine. This gets called whenever an I2C interrupt
 * occurs.
 */
irqreturn_t i2c_sf_isr(int this_irq, void *dev_id)
{
	struct sf_i2c_dev *dev = dev_id;
	u32 stat, enabled;
	enabled = sf_readw(dev, SF_IC_ENABLE);
	stat = sf_readw(dev, SF_IC_RAW_INTR_STAT);
	dev_dbg(dev->dev, "%s:  %s enabled= 0x%x stat=0x%x\n", __func__,
		dev->adapter.name, enabled, stat);
	if (!enabled || !(stat & ~SF_IC_INTR_ACTIVITY))
		return IRQ_NONE;

	stat = i2c_sf_read_clear_intrbits(dev);

	if (stat & SF_IC_INTR_TX_ABRT) {
		dev->cmd_err |= SF_IC_ERR_TX_ABRT;
		dev->status = STATUS_IDLE;

		/*
		 * Anytime TX_ABRT is set, the contents of the tx/rx
		 * buffers are flushed.  Make sure to skip them.
		 */
		sf_writew(dev, 0, SF_IC_INTR_MASK);
		goto tx_aborted;
	}

	if (stat & SF_IC_INTR_RX_FULL)
		i2c_sf_read(dev);

	if (stat & SF_IC_INTR_TX_EMPTY)
		i2c_sf_xfer_msg(dev);

	/*
	 * No need to modify or disable the interrupt mask here.
	 * i2c_sf_xfer_msg() will take care of it according to
	 * the current transmit status.
	 */

tx_aborted:
	if ((stat & (SF_IC_INTR_TX_ABRT | SF_IC_INTR_STOP_DET)) || dev->msg_err)
		complete(&dev->cmd_complete);

	return IRQ_HANDLED;
}
EXPORT_SYMBOL_GPL(i2c_sf_isr);

void i2c_sf_enable(struct sf_i2c_dev *dev)
{
       /* Enable the adapter */
	__i2c_sf_enable(dev, true);
}
EXPORT_SYMBOL_GPL(i2c_sf_enable);

u16 i2c_sf_is_enabled(struct sf_i2c_dev *dev)
{
	return sf_readw(dev, SF_IC_ENABLE);
}
EXPORT_SYMBOL_GPL(i2c_sf_is_enabled);

void i2c_sf_disable(struct sf_i2c_dev *dev)
{
	/* Disable controller */
	__i2c_sf_enable(dev, false);

	/* Disable all interupts */
	sf_writew(dev, 0, SF_IC_INTR_MASK);
	sf_readw(dev, SF_IC_CLR_INTR);
}
EXPORT_SYMBOL_GPL(i2c_sf_disable);

void i2c_sf_clear_int(struct sf_i2c_dev *dev)
{
	sf_readw(dev, SF_IC_CLR_INTR);
}
EXPORT_SYMBOL_GPL(i2c_sf_clear_int);

void i2c_sf_disable_int(struct sf_i2c_dev *dev)
{
	sf_writew(dev, 0, SF_IC_INTR_MASK);
}
EXPORT_SYMBOL_GPL(i2c_sf_disable_int);
/*
u32 i2c_sf_read_comp_param(struct sf_i2c_dev *dev)
{
	return sf_readw(dev, SF_IC_COMP_PARAM_1);
}
EXPORT_SYMBOL_GPL(i2c_sf_read_comp_param);
*/
MODULE_DESCRIPTION("Siflower I2C bus adapter core");
MODULE_LICENSE("GPL");
