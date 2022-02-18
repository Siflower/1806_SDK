/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <mach/sfax8.h>
#include <mach/pcba_test.h>
#include <asm/io.h>
#include <malloc.h>
#include <errno.h>
#include "audio.h"


//IIS Soft Reset Register
#define I2S_RESET		(IIS_SOFT_RESET)

//IIS Base Address
#define I2S_BASE		(0xB8000000)
//IIS Rigister Map
#define IER				(0x00)	/*Enable Register*/
#define IRER			(0x04)	/*Receiver Block Enable Register*/
#define ITER			(0x08)	/*Transmitter Block Enable Register*/
#define CER             (0x0C)	/*Clock Enable Register*/
#define CCR				(0x10)	/*Clock Configuration Register*/
#define RXFFR           (0x14)	/*Receiver Block FIFO Register*/
#define TXFFR           (0x18)	/*Transmitter Block FIFO Register*/
#define LRBR            (0x20)	/*Left Receive Buffer*/
#define LTHR0           (0x20)	/*Left Transmit Holding Register 0*/
#define RRBR            (0x24)	/*Right Receive Buffer*/
#define RTHR0           (0x24)	/*Right Transmit Holding Register 0*/
#define RER             (0x28)	/*Receive Enable Register*/
#define TER0            (0x2C)	/*Transmit Enable Register 0*/
#define RCR             (0x30)	/*Receive Configuration Register*/
#define TCR0            (0x34)	/*Transmit Configuration Register 0*/
#define ISR0            (0x38)	/*Interrupt Status Register 0*/
#define IMR0            (0x3C)	/*Interrupt Mask Register 0*/
#define ROR             (0x40)	/*Receive Overrun Register*/
#define TOR0            (0x44)	/*Transmit Overrun Register 0*/
#define RFCR            (0x48)	/*Receive FIFO Configuration Register*/
#define TFCR0           (0x4C)	/*Transimit FIFO Configuration Register 0*/
#define RFF             (0x50)	/*Receive FIFO Flush*/
#define TFF0            (0x54)	/*Transmit FIFO Flush 0*/
#define LTHR1           (0x60)	/*Left Transmit Holding Register 1*/
#define RTHR1           (0x64)	/*Right Transmit Holding Register 1*/
#define TER1            (0x6C)	/*Transmit Enable Register 1*/
#define TCR1            (0x74)	/*Transmit Configuration Register 1*/
#define ISR1            (0x78)	/*Interrupt Status Register 1*/
#define IMR1            (0x7C)	/*Interrupt Mask Register 1*/
#define TOR1            (0x84)	/*Transmit Overrun Register 1*/
#define TFCR1           (0x8C)	/*Transmit FIFO Configuration Register 1*/
#define TFF1            (0x94)	/*Transmit FIFO Flush 1*/
#define LTHR2           (0xA0)	/*Left Transmit Holding Register 2*/
#define RTHR2           (0xA4)	/*Right Transmit Holding Register 2*/
#define TER2            (0xAC)	/*Transmit Enable Register 2*/
#define TCR2            (0xB4)	/*Transmit Configuration Register 2*/
#define ISR2            (0xB8)	/*Interrupt Status Register 2*/
#define IMR2            (0xBC)	/*Interrupt Mask Register 2*/
#define TOR2            (0xC4)	/*Transmit Overrun Register 2*/
#define TFCR2           (0xCC)	/*Transmit FIFO Configuration Register 2*/
#define TFF2            (0xD4)	/*Transmit FIFO Flush 2*/
#define LTHR3           (0xE0)	/*Left Transmit Holding Register 3*/
#define RTHR3           (0xE4)	/*Right Transmit Holding Register 3*/
#define TER3            (0xEC)	/*Transmit Enable Register 3*/
#define TCR3            (0xF4)	/*Transmit Configuration Register 3*/
#define ISR3            (0xF8)	/*Interrupt Status Register 3*/
#define IMR3            (0xFC)	/*Interrupt Mask Register 3*/
#define TOR3            (0x104)	/*Transmit Overrun Register 3*/
#define TFCR3           (0x10C)	/*Transmit FIFO Configuration Register 3*/
#define TFF3            (0x114)	/*Transmit FIFO Flush 3*/
#define RXDMA           (0x1C0)	/*Receiver Block DMA Register*/
#define RRXDMA          (0x1C4)	/*Reset Receiver Block DMA Register*/
#define TXDMA           (0x1C8)	/*Transmitter Block DMA Register*/
#define RTXDMA          (0x1CC)	/*Reset Transmitter Block DMA Register*/
#define IMCR            (0x3000) /*Mode Control Register */
#define ICDR            (0x3004) /*CDCLK Divisor Register*/
#define ISDR            (0x3008) /*SCLK Divisor Register*/



#define WAV_HEADER		(0x4E)	//wav header size
#define WAV_SIZE		(1024 * 20)// the size of each wav file
#define WAV_NUM			(0x3)//how many wav files


/*
* extern codec init function
*/
extern codec_init(int volume);
/*
*get wav data from flash and store the data in this addr.
*@offset: the offset of flash
*@len:	the length of read data
*@addr: the data we store in.
*
*/
int get_wav_data_from_flash(int offset, int len, void *addr)
{
	char probe[] = "sf probe 0 33000000;";
	char cmd[64] = { 0 };
	sprintf(cmd,"%s sf read 0x%x 0x%x 0x%x", probe, (int)addr,
						 offset, len);
	run_command(cmd, 0);
	return 0;

}

/*
* do i2s soft reset
*/
int i2s_reset(void)
{
	writew(0xFF,(void *)I2S_RESET);
	writew(0x00,(void *)I2S_RESET);
	return 0;
}

/*
*init i2s
*/
int i2s_init(void)
{
	u32 tmp = 0;
	i2s_reset();

	writel(0x0, (void *)(I2S_BASE + IER)); // disable IIS

	tmp = (0x0 << 11) | (0x1 << 2) | (0x1 << 1);
	writel(tmp, (void *)(I2S_BASE + IMCR));	// set iis0 in master mode

	//set SCLKDIV, iis clk is 108Mhz, fs is 8khz, channes is 2
	//16 bits, 108 * 1000000 / ( 8 * 1000 * 16 * 2)
	writel(108 * 1000 / ( 8 * 16 * 2) - 1, (void *)(I2S_BASE + ISDR));
	writel(0x8, (void *)(I2S_BASE + ICDR)); // set cdclk to 12MHZ
	writel(0x0, (void *)(I2S_BASE + CCR)); // set wss
	writel(0x2, (void *)(I2S_BASE + TCR0)); // set tx resolution to 16 bits
	writel(0x1, (void *)(I2S_BASE + TXFFR)); // clear tx fifo
	writel(0x23, (void *)(I2S_BASE + IMR0)); //mask all interrupts except tx empty interrupt

	//close all tx and tx channels, and then open tx chan 0
	writel(0x0, (void *)(I2S_BASE + RER));
	writel(0x0, (void *)(I2S_BASE + TER0));
	writel(0x0, (void *)(I2S_BASE + TER1));
	writel(0x0, (void *)(I2S_BASE + TER2));
	writel(0x0, (void *)(I2S_BASE + TER3));
	writel(0x1, (void *)(I2S_BASE + ITER));
	writel(0x1, (void *)(I2S_BASE + TER0));

	writel(0xA, (void *)(I2S_BASE + TFCR0)); // set tx chan0 interrupt trigger to 0xa
	writel(0x1, (void *)(I2S_BASE + IER)); // enable IIS
	writel(0x1, (void *)(I2S_BASE + CER)); // enable IIS clock

	return 0;
}

/*
*wait for tx empty
*@return 0: tx is empty, -1 : wait for tx empty time out
*/
int wait_tx_empty(void)
{
	int i = 0;
	for( ; i < 100; i++){
		if(readl((void *)(I2S_BASE + ISR0)) & 0x10)
			break;
		else
			udelay(50);
	}
	if( i == 100)
		return -1;
	return 0;
}

/*
* play wav data
* @addr:
* @len: len = wav header length + raw wav data length, we just need raw wav data
* 		the len should keep align with 4bytes.
* @return 0; ok; -1 : some error.
*/
int play(void *addr, int len)
{
	int header = WAV_HEADER;
	int i;
	for(i = 0; i < len - header; i = i + 4){
		if(!wait_tx_empty()){
			writel(readw((void *)(addr + header + i)), (void *)(I2S_BASE + LTHR0));
			writel(readw((void *)(addr + header + i + 2)), (void *)(I2S_BASE + RTHR0));
		}else
			break;
	}
	if( i >= len - header)
		return 0;
	else
		return -1;
}
/*
* codec test
* @d: data point, first 32-bits is wav number,
	  the second 32-bits is volume.
*/
int codec_test(void *d)
{

	int ret, len;
	u8 random = 0, volume = 0;
	len = WAV_SIZE;

	random = *((int *)d);
	volume = *((int *)d + 1);
	printf("random is %d : volume is %d\n", random, volume);

	if(random > 3 || volume > 192)
		return ERROR_CMD;

	ret = codec_init(volume);
	if(ret)
		goto err;

	printf("Codec init ok!\n");
	ret = i2s_init();
	if(ret)
		goto err;
	printf("I2s init ok!\n");

	printf("Play start!\n");
	ret = play(wav_data[random], len);
	if(ret)
		goto err;
	printf("Play end!\n");
	return CMD_SUCCESS;

err:
	printf("I2C: time out\n");

	return CMD_TIMEOUT;
}
