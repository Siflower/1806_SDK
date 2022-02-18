/**
  ******************************************************************************
  * @file  sf16w18_spi.c
  * @author Qi
  * @version  V4.0.0
  * @date  09/10/2016
  * @brief  This file provides all the SPI firmware functions.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Siflower Inc SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 Siflower Inc</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include <io.h>
#include <types.h>
#include <clk.h>
#include <errorno.h>
#include <sys_manager.h>
#include <pad.h>
#ifdef TIMER
#include <timer/timer.h>
#endif
#include "flash.h"
#ifdef SPI_DMA
#include <gdma.h>
#endif

#ifdef CLIB
#include <stdio.h>
#endif

volatile int _spi_radio = 20;
#define SPI_FLASH_BITRATE           (SF_PBUS_CLK/20)

/******************************************************************************
 *	Description:This function set SPI bit rate.								  *
 *	$chnum:choose channel number 0 or 1.									  *
 *	$BitRate:																  *
 *	Return:none.															  *
 ******************************************************************************/
static void SPI_SetBitRate(struct sfa18_spi_registers *reg, u32 BitRate)
{
	u32 reg_cr0;
	u32 scr;

	/* Clear SPI CR0 Bit 8:15 and CPR Bit 0:7 */
	reg_cr0 = readl(&reg->sspcr0);
	reg_cr0 &= ~(0xff << 8);

	scr = SPI_CLOCK / (BitRate * 2) - 1;
	writel(2, &reg->sspcpsr);
	writel(reg_cr0 | (scr << 8), &reg->sspcr0);

	return;
}

/******************************************************************************
 *	Description:This function set SPI tx and rx fifo interrupt level.		  *
 *	$chnum:choose channel number 0 or 1.									  *
 *	$Txlevel:the level make interrupt trigger.								  *
 *	Return:none.															  *
 ******************************************************************************/
static void SPI_SetFifoLevel(struct sfa18_spi_registers *reg, u8 Txlevel, u8 Rxlevel)
{
	writel((Rxlevel << 8) | Txlevel, &reg->sspfifolev);

	return;
}

/******************************************************************************
 *	Description:This function enable SPI operation.							  *
 *	$chnum:choose channel number 0 or 1.									  *
 *	Return:none.															  *
 ******************************************************************************/
void SPI_Enable(struct sfa18_spi_registers *reg)
{
	u32 reg_cr1;

	reg_cr1 = readl(&reg->sspcr1);
	writel(reg_cr1 | (1 << 1), &reg->sspcr1);

	return;
}

/******************************************************************************
 *	Description:This function disable SPI operation.						  *
 *	$chnum:choose channel number 0 or 1.									  *
 *	Return:none.															  *
 ******************************************************************************/
void SPI_Disable(struct sfa18_spi_registers *reg)
{
	u32 reg_cr1;

	reg_cr1 = readl(&reg->sspcr1);
	writel(reg_cr1 & (~(1 << 1)),&reg->sspcr1);

	return;
}

/******************************************************************************
 *	Description:This function init SPI.										  *
 *	$chnum:choose channel number 0 or 1.									  *
 *	$Mode:																	  *
 *	$BitRate:																  *
 *	Return:none.															  *
 ******************************************************************************/
void SPI_Init()			
{
	struct sfa18_spi_registers *reg = (struct sfa18_spi_registers *)SPI_BASE;
	writel(0x7, &reg->sspcr0);
	writel(0x0, &reg->sspcr1);
	writel(0x0, &reg->sspcr1);
	SPI_SetBitRate(reg, SPI_FLASH_BITRATE);
	SPI_SetFifoLevel(reg, SPI_FIFO_DEPTH / 2, SPI_FIFO_DEPTH / 2);

	return;
}

/**
 * Activate a SPI chipselect.
 */
void SPI_cs_activate(void)
{
	SPI_Enable((struct sfa18_spi_registers *)SPI_BASE);
	sf_pad_set_func(SPI_CS, GPIO_OUTPUT);
	sf_pad_set_value(SPI_CS, LOW_LEVEL);
}


/**
 * Deactivate a SPI chipselect.
 */
void SPI_cs_deactivate(void)
{
	sf_pad_set_value(SPI_CS, HIGH_LEVEL);
}

/**
 * SPI transfer
 *
 * This writes "bitlen" bits out the SPI MOSI port and simultaneously clocks
 * "bitlen" bits in the SPI MISO port.  That's just the way SPI works.
 *
 * The source of the outgoing bits is the "dout" parameter and the
 * destination of the input bits is the "din" parameter.  Note that "dout"
 * and "din" can point to the same memory location, in which case the
 * input data overwrites the output data (since both are buffered by
 * temporary variables, this is OK).
 *
 * spi_xfer() interface:
 * @slave:	The SPI slave which will be sending/receiving the data.
 * @bitlen:	How many bits to write and read.
 * @dout:	Pointer to a string of bits to send out.  The bits are
 *		held in a byte array and are sent MSB first.
 * @din:	Pointer to a string of bits that will be filled in.
 * @flags:	A bitwise combination of SPI_XFER_* flags.
 *
 * Returns: 0 on success, not 0 on failure
 */
int SPI_xfer(unsigned int bitlen, const void *dout, void *din, unsigned long flags)
{
	unsigned int len_tx = 0;
	unsigned int len_rx = 0;
	unsigned int len = 0;
	u32 loop_num = 0, wait_busy = 0;
	u32 tx_stat = 0, status = 0;
	const u8 *txp = dout;
	u8 *rxp = din;
	u8 value = 0;
	struct sfa18_spi_registers *reg = (struct sfa18_spi_registers *)SPI_BASE;

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

		SPI_cs_activate();
	}

	for (len_tx = 0, len_rx = 0, loop_num = 0; len_rx < len; loop_num++) {
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

		if (!((tx_stat & SSPRIS_TX) || (status & SSPSR_RNE)))
			loop_num++;
		if (loop_num > 1000 * len) {
			printf("spi_xfer time out!!!\n");
			SPI_cs_deactivate();
			return -EFAULT;
		}
	}

out:
	if (flags & SPI_XFER_END) {
		/*
		 * Wait until the transfer is completely done before
		 * we deactivate CS.
		 */
		wait_busy = 0;
		status = readl(&reg->sspsr);
		while (status & SSPSR_BSY) {
			wait_busy++;
			u_delay(1);
			if (wait_busy > SPI_WAIT_TOUT) {
				printf("Error:rx fifo is empty when receive data at spi_rx_data!\n");
				return -EFAULT;
			}
			status = readl(&reg->sspsr);
		}

		SPI_cs_deactivate();
	}

	return 0;
}

/******************* (C) COPYRIGHT 2016 Siflower Inc *****END OF FILE****/
