/*
 * (C) Copyright 2018
 * Mark Cai, <mark.cai@siflower.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * SFAX8 UART's
 * ------------------------------------
 *
 *
 */

#ifndef __ASSEMBLY__
/*
 * We can use a combined structure for SFAX8, because they overlap
 * only in common registers.
 */
struct sfax8_regs {
	u32	dr;		/* 0x00 Data register */
	u32	ecr;		/* 0x04 Error clear register (Write) */
	u32 reserved1;	/* 0x08 reserved register*/
	u32 reserved2;	/* 0x0C reserved register*/
	u32 reserved3;	/* 0x10 reserved register*/
	u32 reserved4;	/* 0x14 reserved register*/
	u32	fr;		/* 0x18 Flag register (Read only) */
	u32	sfax8_rlcr;	/* 0x1c Receive line control register */
	u32	ilpr;		/* 0x20 IrDA low-power counter register */
	u32	sfax8_ibrd;	/* 0x24 Integer baud rate register */
	u32	sfax8_fbrd;	/* 0x28 Fractional baud rate register */
	u32	sfax8_lcrh;	/* 0x2C Line control register */
	u32	sfax8_cr;	/* 0x30 Control register */
};
#endif

#define UART_SFAX8_RSR_OE               0x08
#define UART_SFAX8_RSR_BE               0x04
#define UART_SFAX8_RSR_PE               0x02
#define UART_SFAX8_RSR_FE               0x01

#define UART_SFAX8_FR_TXFE              0x80
#define UART_SFAX8_FR_RXFF              0x40
#define UART_SFAX8_FR_TXFF              0x20
#define UART_SFAX8_FR_RXFE              0x10
#define UART_SFAX8_FR_BUSY              0x08
#define UART_SFAX8_FR_TMSK              (UART_SFAX8_FR_TXFF + UART_SFAX8_FR_BUSY)

/*
 *  SFAX8 definitions
 *
 */
#define UART_SFAX8_LCRH_SPS             (1 << 7)
#define UART_SFAX8_LCRH_WLEN_8          (3 << 5)
#define UART_SFAX8_LCRH_WLEN_7          (2 << 5)
#define UART_SFAX8_LCRH_WLEN_6          (1 << 5)
#define UART_SFAX8_LCRH_WLEN_5          (0 << 5)
#define UART_SFAX8_LCRH_FEN             (1 << 4)
#define UART_SFAX8_LCRH_STP2            (1 << 3)
#define UART_SFAX8_LCRH_EPS             (1 << 2)
#define UART_SFAX8_LCRH_PEN             (1 << 1)
#define UART_SFAX8_LCRH_BRK             (1 << 0)

#define UART_SFAX8_CR_CTSEN             (1 << 15)
#define UART_SFAX8_CR_RTSEN             (1 << 14)
#define UART_SFAX8_CR_OUT2              (1 << 13)
#define UART_SFAX8_CR_OUT1              (1 << 12)
#define UART_SFAX8_CR_RTS               (1 << 11)
#define UART_SFAX8_CR_DTR               (1 << 10)
#define UART_SFAX8_CR_RXE               (1 << 9)
#define UART_SFAX8_CR_TXE               (1 << 8)
#define UART_SFAX8_CR_LPE               (1 << 7)
#define UART_SFAX8_CR_IIRLP             (1 << 2)
#define UART_SFAX8_CR_SIREN             (1 << 1)
#define UART_SFAX8_CR_UARTEN            (1 << 0)

#define UART_SFAX8_IMSC_OEIM            (1 << 10)
#define UART_SFAX8_IMSC_BEIM            (1 << 9)
#define UART_SFAX8_IMSC_PEIM            (1 << 8)
#define UART_SFAX8_IMSC_FEIM            (1 << 7)
#define UART_SFAX8_IMSC_RTIM            (1 << 6)
#define UART_SFAX8_IMSC_TXIM            (1 << 5)
#define UART_SFAX8_IMSC_RXIM            (1 << 4)
#define UART_SFAX8_IMSC_DSRMIM          (1 << 3)
#define UART_SFAX8_IMSC_DCDMIM          (1 << 2)
#define UART_SFAX8_IMSC_CTSMIM          (1 << 1)
#define UART_SFAX8_IMSC_RIMIM           (1 << 0)

