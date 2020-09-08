/*
 * Synopsys DesignWare I2C adapter driver (master only).
 *
 * Based on the TI DAVINCI I2C adapter driver.
 *
 * Copyright (C) 2006 Texas Instruments.
 * Copyright (C) 2007 MontaVista Software Inc.
 * Copyright (C) 2009 Provigent Ltd.
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

#include <sf16a18.h>
#define SF_IC_CON_MASTER		0x1
#define SF_IC_CON_SPEED_STD		0x2
#define SF_IC_CON_SPEED_FAST		0x4
#define SF_IC_CON_SPEED_HIGH		0x6
#define SF_IC_CON_10BITADDR_MASTER	0x10
#define SF_IC_CON_RESTART_EN		0x20
#define SF_IC_CON_SLAVE_DISABLE		0x40


/**
 * struct sf_i2c_dev - private i2c-designware data
 * @dev: driver model device node
 * @base: IO registers pointer
 * @cmd_complete: tx completion indicator
 * @lock: protect this struct and IO registers
 * @clk: input reference clock
 * @cmd_err: run time hasfare error code
 * @msgs: points to an array of messages currently being transfered
 * @msgs_num: the number of elements in msgs
 * @msg_write_idx: the element index of the current tx message in the msgs
 *	array
 * @tx_buf_len: the length of the current tx buffer
 * @tx_buf: the current tx buffer
 * @msg_read_idx: the element index of the current rx message in the msgs
 *	array
 * @rx_buf_len: the length of the current rx buffer
 * @rx_buf: the current rx buffer
 * @msg_err: error status of the current transfer
 * @status: i2c master status, one of STATUS_*
 * @abort_source: copy of the TX_ABRT_SOURCE register
 * @irq: interrupt number for the i2c master
 * @adapter: i2c subsystem adapter node
 * @tx_fifo_depth: depth of the hardware tx fifo
 * @rx_fifo_depth: depth of the hardware rx fifo
 * @rx_outstanding: current master-rx elements in tx fifo
 * @ss_hcnt: standard speed HCNT value
 * @ss_lcnt: standard speed LCNT value
 * @fs_hcnt: fast speed HCNT value
 * @fs_lcnt: fast speed LCNT value
 *
 * HCNT and LCNT parameters can be used if the platform knows more accurate
 * values than the one computed based only on the input clock frequency.
 * Leave them to be %0 if not used.
 */
struct sf_i2c_dev {
	struct device		*dev;
	void __iomem		*base;
	struct completion	cmd_complete;
	struct mutex		lock;
	struct clk		*clk;
	u32			(*get_clk_rate_khz) (struct sf_i2c_dev *dev);
	struct sf_pci_controller *controller;
	int			cmd_err;
	struct i2c_msg		*msgs;
	int			msgs_num;
	int			msg_write_idx;
	u32			tx_buf_len;
	u8			*tx_buf;
	int			msg_read_idx;
	u32			rx_buf_len;
	u8			*rx_buf;
	int			msg_err;
	unsigned int		status;
	u32			abort_source;
	int			irq;
//	u32			accessor_flags;
	struct i2c_adapter	adapter;
	u32			functionality;
	u16			master_cfg;
	u16		tx_fifo_depth;
	u16		rx_fifo_depth;
	int			rx_outstanding;
/*	u32			sda_hold_time;
	u32			sda_falling_time;
	u32			scl_falling_time;
*/
	u16			ss_hcnt;
	u16			ss_lcnt;
	u16			fs_hcnt;
	u16			fs_lcnt;
};

#define ACCESS_SWAP		0x00000001
#define ACCESS_16BIT		0x00000002

struct sf_i2c_platform_data {
	unsigned int i2c_scl_freq;
};

extern u32 sf_readl(struct sf_i2c_dev *dev, int offset);
extern void sf_writel(struct sf_i2c_dev *dev, u32 b, int offset);
extern int i2c_sf_init(struct sf_i2c_dev *dev);
extern int i2c_sf_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[],
		int num);
extern u32 i2c_sf_func(struct i2c_adapter *adap);
extern irqreturn_t i2c_sf_isr(int this_irq, void *dev_id);
extern void i2c_sf_enable(struct sf_i2c_dev *dev);
extern u16 i2c_sf_is_enabled(struct sf_i2c_dev *dev);
extern void i2c_sf_disable(struct sf_i2c_dev *dev);
extern void i2c_sf_clear_int(struct sf_i2c_dev *dev);
extern void i2c_sf_disable_int(struct sf_i2c_dev *dev);
extern u32 i2c_sf_read_comp_param(struct sf_i2c_dev *dev);
