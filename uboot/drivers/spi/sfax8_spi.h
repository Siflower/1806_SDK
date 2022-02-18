/*
 * (C) Copyright 2016
 * Siflower Communication Technology <www.siflower.com>
 * Written-by: Qi Zhang <qi.zhang@siflower.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __SFA18_SPI_H__
#define __SFA18_SPI_H__

/* SPI Registers */
struct sfax8_spi_registers {
	u32 sspcr0;		/* 0x000 */
	u32 sspcr1;		/* 0x004 */
	u32 sspdr;		/* 0x008 */
	u32 sspsr;		/* 0x00c */
	u32 sspcpsr;	/* 0x010 */
	u32 sspimsc;	/* 0x014 */
	u32 sspris;		/* 0x018 */
	u32 sspmis;		/* 0x01c */
	u32 sspicr;		/* 0x020 */
	u32 sspdmacr;	/* 0x024 */
	u32 sspfifolev;	/* 0x028 */
};

#define SSPCR0			0x0000
#define SSPCR0_MODE_SHIFT	6
#define SSPCR0_SCR_SHIFT	8
#define SSPCR0_SPH		BIT(7)
#define SSPCR0_SPO		BIT(6)
#define SSPCR0_FRF_SHIFT	4
#define SSPCR0_DSS_8BIT		7

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

#define SSPIMSC			0x0014
#define SSPIMSC_ROR		BIT(0)
#define SSPIMSC_RT		BIT(1)
#define SSPIMSC_RX		BIT(2)
#define SSPIMSC_TX		BIT(3)

#define SSPRIS			0x0018
#define SSPRIS_ROR		BIT(0)
#define SSPRIS_RT		BIT(1)
#define SSPRIS_RX		BIT(2)
#define SSPRIS_TX		BIT(3)

#define SSPMIS			0x001C
#define SSPMIS_ROR		BIT(0)
#define SSPMIS_RT		BIT(1)
#define SSPMIS_RX		BIT(2)
#define SSPMIS_TX		BIT(3)

#define SSPICR			0x0020
#define SSPICR_ROR		BIT(0)
#define SSPICR_RT		BIT(1)

#define SSPDMACR		0x0024
#define SSPDMACR_TXEN	BIT(1)
#define SSPDMACR_RXEN	BIT(0)

#define SSPFIFOLEV		0x0028
#define SSPFIFO_TX_SHIFT	0
#define SSPFIFO_RX_SHIFT	8

#define	SPI0_CS0		5
/* timeout in milliseconds */
#define SPI_TIMEOUT		5
/* maximum depth of RX/TX FIFO */
#define SPI_FIFO_SIZE		64
/* default wordlen */
#define SPI_DEFAULT_WORDLEN	8

#endif /* __SFA18_SPI_H__ */

