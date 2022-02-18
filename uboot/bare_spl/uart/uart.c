/**
  ******************************************************************************
  * @file  sf16w18_uart.c
  * @author  Mark.cai
  * @version  V1.0
  * @date  03/28/2016
  * @brief  This file provides all the UART firmware functions.
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

/* ---------------------Includes ---------------------------------------------*/

#include <types.h>
#include <io.h>
#include <sf_mmap.h>
#include <errorno.h>
#include <irq_handle.h>
#include <pad.h>
#include "uart.h"

#ifdef CLIB
#include <stdio.h>
#endif

/* ----------------------macro variable--------------------------------------- */
#define COUNT_MAX        200

#define ODD_PARITY        1
#define EVEN_PARITY       2
#define MASK_PARITY       3
#define NULL_PARITY       4
#define NO_PARITY         5

#define DL_CTRL_NO        1


/* ----------------------extern funciton--------------------------------------- */

/* ----------------------local funciton--------------------------------------- */


/* !
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *                                                                                                 *
 *     Read a single byte from the uart port. Returns 1 on success, 0                              *
 *     otherwise. When the function is succesfull, the character read is                           *
 *     written into its argument c.                                                                *
 *                                                                                                 *
 ***************************************************************************************************
 */

char uart_getc(u32 channel)
{
    u32 count = 0;

	while(count < COUNT_MAX*400)//8 seconds
    {
        if(!(readl(UART0_FR+channel*UART_BASE_INTERVAL) & UART_FR_RXFE))		/*to check the receive FIFO empty */
			break;
		count++;
		m_delay(1);
    }

	if(count > COUNT_MAX*400)
		return 0;

    return readl(UART0_DR+channel*UART_BASE_INTERVAL)&0xff;

}


/* !
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *                                                                                                 *
 *     Read a single byte from the uart port. Returns 0 fail, otherwise success.                   *
 *      When the function is succesfull, the character read is                                     *
 *     written into its argument c.                                                                *
 *                                                                                                 *
 ***************************************************************************************************
 */

char uart_getc_mdelay(u32 channel,u32 mdelay)
{
	int i = 0;
	u32 retry_timer = mdelay*100;

	if(retry_timer > 0)
	{
		for(i=0;i < retry_timer;i++)
		{
			if(!(readl(UART0_FR+channel*UART_BASE_INTERVAL) & UART_FR_RXFE))		/*to check the receive FIFO empty */
			{
				return readl(UART0_DR+channel*UART_BASE_INTERVAL)&0xff;
			}
#ifdef SIMULATION
			m_delay(1);
#else
			u_delay(10);
#endif
		}
	}
	else
	{
		if(!(readl(UART0_FR+channel*UART_BASE_INTERVAL) & UART_FR_RXFE))		/*to check the receive FIFO empty */
			return readl(UART0_DR+channel*UART_BASE_INTERVAL)&0xff;
	}

	return 0;
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *  Output a single byte to the uart port.                                                         *
 *                                                                                                 *
 ***************************************************************************************************
 */

static void uart_putc(u32 channel,const char c)
{
    writel(c, UART0_DR+channel*UART_BASE_INTERVAL);

}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to choose the baudrate wo want to use.                                     *
 *        1. get the baudrate wo want ;                                                            *
 *        2. create the baudrate wo choose;                                                        *
 *        3. check the register ;                                                                  *
 *                                                                                                 *
 ***************************************************************************************************
 */

static int uart_set_baud(u32 channel,u32 baudrate)
{
	u32 clk = UART_CLK;
    u16 ibr, fbr;

	if(channel)
    	printf("uart channel:%d baudrate:%d \n",channel,baudrate);

    /*create the baudrate */

    //clk = module_get_clock(1);											/*get the uart clock*/
    ibr = (clk/(16*baudrate)) & 0xffff;                                 /*calculate the baudrate divisor and put the integer part into ibr*/
    fbr = (((clk - ibr*(16*baudrate)) *64)/(16*baudrate)) & 0x3f;       /*put the fractional part into fbr*/

    writel(ibr, UART0_IBRD+channel*UART_BASE_INTERVAL);					/*put ibr into Uart Integer Baud Rate Register*/
    writel(fbr, UART0_FBRD+channel*UART_BASE_INTERVAL);

    return 0 ;
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to choose the data length wo want to use.                                  *
 *        1. choose the data length wo want ;                                                      *
 *        2. set the data length ;                                                                 *
 *                                                                                                 *
 ***************************************************************************************************
 */

static int uart_set_data_len(u32 channel,u32 data_len)
{
    u8 reg = 0;

	reg = readl(UART0_LCRH+channel*UART_BASE_INTERVAL);
	if(channel)
		printf("uart channel:%d data_len:%d \n",channel,data_len);

	if(data_len == 5)
		{
		reg &= ~((1 << 5)|(1 << 6));							    /*data length = 5 */
		writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);
		}
    else if(data_len == 6)
		{
		reg |= (1 << 5);
		reg &= ~(1 << 6);
		writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);		/*data length = 6 */
		}
    else if(data_len == 7)
		{
		reg &= ~(1 << 5);
		reg |= (1 << 6);
		writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);       /*data length = 7 */
		}
    else if(data_len ==8)
		{
		reg |= ((1 << 5)|(1 << 6));
		writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);       /*data length = 8 */
		}
    else
		return -1;

	return 0 ;
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to choose the parity mode wo want to test .                                *
 *        1. choose the parity kind wo want ;                                                      *
 *        2. set the parity mode ;                                                                 *
 *                                                                                                 *
 ***************************************************************************************************
 */

static int uart_set_parity(u32 channel,u32 parity)
{
    u8 reg = 0;

	reg = readl(UART0_LCRH+channel*UART_BASE_INTERVAL);

	if(channel)
		printf("uart channel:%d parity:%d\n",channel,parity);

    switch(parity){
    	case ODD_PARITY:
			reg |= (1 << 1);
			reg &= ~(1 << 2);
			writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);							/*odd parity */
		break;

	    case EVEN_PARITY:
			reg |= ((1 << 1)|(1 << 2));
			writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);                           /*even parity */
		break;

	    case MASK_PARITY:
	        reg |= ((1 << 1)|(1 << 7));
	        reg &= ~(1 << 2);
			writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);							/*parity = 1 */
	    break;

		case NULL_PARITY:
		    reg |= ((1 << 1)|(1 << 2)|(1 << 7));
		    writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);                           /*parity = 0 */
		break;

	    case NO_PARITY:
	    reg &= ~(1 << 1);
			writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);							/*NO parity */
		break;

		default:
			return -1;
		break;
    }

	return 0;
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to config uart module .                                                    *
 *                                                                                                 *
 ***************************************************************************************************
 */
static void uart_config(u32 channel,u32 baudrate,u32 data_len,u32 parity,u32 stop)
{
	u8 reg = 0;

	uart_set_baud(channel,baudrate);			/*create the baudrate you choose */
    uart_set_data_len(channel,data_len);		/*choose the data length you want */
    uart_set_parity(channel,parity);			/*choose the parity mode you want */

    reg = readl(UART0_LCRH+channel*UART_BASE_INTERVAL);
    if(stop==1)
        reg &= ~(1 << 3);
    else
        reg |= (1 << 3);
	reg |= (1 << 4);
	writel( reg , UART0_LCRH+channel*UART_BASE_INTERVAL);	/*choose one stop bit and enable the FIFO */

}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *    This part is used to reset the UART and check all the registers.                             *
 *        1. reset UART ;                                                                          *
 *        2. read all the registers to check the default setting;                                  *
 *                                                                                                 *
 ***************************************************************************************************
 */

static int uart_init_enable(u32 channel)
{
	u16 ttl = 0;

	ttl = readl(UART0_CR+channel*UART_BASE_INTERVAL);
	ttl |= 0x301;
    writel( ttl , UART0_CR+channel*UART_BASE_INTERVAL);

    return 0 ;
}
/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *  dump UART all registers;                                                                       *
 *                                                                                                 *
 ***************************************************************************************************
 */
void uart_dump_all_reg(void)
{
	#if 0
	int index = 0;
	for(index = 0;index < 3;index++){
		/*read all the register to check the default setting */
	    readl(UART0_DR+channel*UART_BASE_INTERVAL);
	    readl(UART0_RSR+channel*UART_BASE_INTERVAL);
	    readl(UART0_ECR+channel*UART_BASE_INTERVAL);
	    readl(UART0_FR+channel*UART_BASE_INTERVAL);
	    readl(UART0_ILPR+channel*UART_BASE_INTERVAL);
	    readl(UART0_IBRD+channel*UART_BASE_INTERVAL);
	    readl(UART0_FBRD+channel*UART_BASE_INTERVAL);
	    readl(UART0_LCRH+channel*UART_BASE_INTERVAL);
	    readl(UART0_CR+channel*UART_BASE_INTERVAL);
	    readl(UART0_IFLS+channel*UART_BASE_INTERVAL);
	    readl(UART0_IMSC+channel*UART_BASE_INTERVAL);
	    readl(UART0_RIS+channel*UART_BASE_INTERVAL);
	    readl(UART0_MIS+channel*UART_BASE_INTERVAL);
	    readl(UART0_ICR+channel*UART_BASE_INTERVAL);
	    readl(UART0_DMACR+channel*UART_BASE_INTERVAL);
	}
	#endif
}

/*!
 ***************************************************************************************************
 * -Description:                                                                                   *
 *  init the UART* ;                                                                               *
 *  config the baudrate:115200,data length:8,parity mode:NONE,stop bit:1,FIFO level: half          *
 *  channel:uart0 for debug                                                                        *
 ***************************************************************************************************
 */
int uart_init_debug_channel(int channel)
{
	uart_init_enable(channel);
	uart_config(channel, 115200, 8, 5, 1);

	return 0;
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *  tx debug message by uart0 ;                                                                    *
 *  buf: assic I/O stream                                                                          *
 *  len: message len                                                                               *
 ***************************************************************************************************
 */
void uart_tx_debug_msg(char * buf,unsigned int len)
{
    u32 index = 0;
    int i;
	u32 count = 0;

    while (index < len)
    {
        if(len-index > UART_FIFO_DEPTH)
        {
            count = 0;

			for(i = 0;i < UART_FIFO_DEPTH;i++)
            {
                uart_putc(UART_DEBUG_CHANNEL,buf[index]);
                index++;
            }
            while(!(readl(UART0_FR+UART_DEBUG_CHANNEL*UART_BASE_INTERVAL) & (1 << 7)))
			{
				if(count > COUNT_MAX*2)
					break;
				count++;
				u_delay(10);
			}

        }
        else
        {
            count = 0;

			for(i=0;i<(len-index);i++)
            {
                uart_putc(UART_DEBUG_CHANNEL,buf[index]);
                index++;
            }
            while(!(readl(UART0_FR+UART_DEBUG_CHANNEL*UART_BASE_INTERVAL) & (1 << 7)))
			{
				if(count > COUNT_MAX*2)
					break;
				count++;
				u_delay(10);
			}
        }
    }
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *  rx debug message by uart0 ;                                                                    *
 *  buf: assic I/O stream                                                                          *
 *  len: message len                                                                               *
 ***************************************************************************************************
 */
void uart_rx_debug_msg(char * buf,unsigned int len)
{
	int index = 0;

	while(index < len)
	{
		buf[index] = uart_getc(UART_DEBUG_CHANNEL);
		index++;
	}
}

/*!
 ***************************************************************************************************
 * -Description:                                                                                   *
 *  init the UART* ;                                                                               *
 *  config the channel,baudrate,data length,parity mode,stop bit                                   *
 *                                                                                                 *
 ***************************************************************************************************
 */

void uart_init(u8 channel,u32 baudrate,u8 datalength,u8 parity,u8 stop)
{

    uart_init_enable(channel);                                  /*reset the UART*/
    uart_config(channel,baudrate,datalength,parity,stop);
}

/*!
 ***************************************************************************************************
 * -Description:                                                                                   *
 *  tx debug message by uart ;                                                                     *
 *  buf: assic I/O stream                                                                          *
 *  len: message len                                                                               *
 ***************************************************************************************************
 */
void uart_tx_msg(u8 channel,char * buf,unsigned int len)
{
    u32 index = 0;
    int i;
	u32 count = 0;

    while (index < len)
    {
        if(len-index > UART_FIFO_DEPTH)
        {
            count = 0;
			for(i = 0;i < UART_FIFO_DEPTH;i++)
            {
                uart_putc(channel,buf[index]);
                index++;
            }
            while(!(readl(UART0_FR+channel*UART_BASE_INTERVAL) & (1 << 7)))
			{
				if(count > COUNT_MAX)
					break;
				count++;
				u_delay(10);
			}

        }
        else
        {
            count = 0;
			for(i=0;i<(len-index);i++)
            {
                uart_putc(channel,buf[index]);
                index++;
            }
            while(!(readl(UART0_FR+channel*UART_BASE_INTERVAL) & (1 << 7)))
			{
				if(count > COUNT_MAX)
					break;
				count++;
				u_delay(10);
			}
        }
    }
}

/*!
 ***************************************************************************************************
 *                                                                                                 *
 * -Description:                                                                                   *
 *  rx debug message by uart ;                                                                     *
 *  buf: assic I/O stream                                                                          *
 *  len: message len                                                                               *
 ***************************************************************************************************
 */
void uart_rx_msg(u8 channel,char * buf,unsigned int len)
{
	int index = 0;

	while(index < len)
	{
		buf[index] = uart_getc(channel);
		index++;
	}
}
