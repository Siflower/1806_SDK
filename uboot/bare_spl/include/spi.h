/**
  ******************************************************************************
  * @file  sf16w18_spi.h
  * @author  Qi 
  * @version  V4.0.0
  * @date  2016/09/08
  * @brief  This file contains all the functions prototypes for the SPI firmware
  *         library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SF16W18_SPI_H
#define __SF16W18_SPI_H

/* Includes ------------------------------------------------------------------*/
#include <sf_mmap.h>
#include <types.h>

#define BIT(n)			(1<<n)

#define SPI_FIFO_DEPTH	64
#define SPI_WAIT_TOUT	5000

#define SPI0_BASE		(SIFLOWER_SPI_BASE)
#define SPI1_BASE		(SIFLOWER_SPI_BASE + SIFLOWER_SPI_OFFSET)
#define SPI2_BASE		(SIFLOWER_SPI_BASE + 2*SIFLOWER_SPI_OFFSET)
#define SPI0_CS0		5
#define SPI2_CS0		8
#ifdef SF19A28
#define SPI_BASE		SPI2_BASE
#define SPI_CS			SPI2_CS0
#else
#define SPI_BASE		SPI0_BASE
#define SPI_CS			SPI0_CS0
#endif

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

/* SPI flag */
#define SPI_XFER_BEGIN		BIT(0)	/* Assert CS before transfer */
#define SPI_XFER_END		BIT(1)	/* Deassert CS after transfer */

#define MIN(a,b)		((a)<(b)?(a):(b))

/* SPI Registers */
struct sfa18_spi_registers {
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

void SPI_Init();
int SPI_xfer(unsigned int bitlen, const void *dout, void *din, unsigned long flags);

#endif /*__SF16W18_SPI_H */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
