/*
 * (C) Copyright 2015 Google, Inc
 *
 * SPDX-License-Identifier:	GPL-2.0
 */

#ifndef _ASM_ARCH_PERIPH_H
#define _ASM_ARCH_PERIPH_H

/*
 * The peripherals supported by the hardware. This is used to specify clocks
 * and pinctrl settings. Some SoCs will not support all of these, but it
 * provides a common reference for common drivers to use.
 */
enum periph_id {
	PERIPH_ID_SPI0,
	PERIPH_ID_EMMC,
	PERIPH_ID_SDIO,
	PERIPH_ID_UART0,
	PERIPH_ID_I2C0,
	PERIPH_ID_I2C1,
	PERIPH_ID_I2C2,
	PERIPH_ID_SPI1,
	PERIPH_ID_SPI2,
	PERIPH_ID_I2S0,
	PERIPH_ID_PCM0,
	PERIPH_ID_UART1,
	PERIPH_ID_I2S1,
	PERIPH_ID_PCM1,
	PERIPH_ID_UART2,
	PERIPH_ID_SPDIF,
	PERIPH_ID_UART3,
	PERIPH_ID_PWM0,
	PERIPH_ID_PWM1,
	PERIPH_ID_RTC,
	PERIPH_ID_ETH_LED,
	PERIPH_ID_JTAG,
	PERIPH_ID_USB,
	PERIPH_ID_DDR,
	PERIPH_ID_RGMII,
	PERIPH_ID_RMII,
	PERIPH_ID_GDU
};

//#define PAD_BASE            (SIFLOWER_SYSCTL_BASE+0x30000)
#if defined(CONFIG_SOC_SFA28_MPW0) || defined(CONFIG_SOC_SFA28_FULLMASK)
#define FUNC_SW_SEL (1 << 3)
#define FMUX_SEL	(1 << 2)
#define MODE_BIT1	(1 << 1)
#define MODE_BIT0	(1 << 0)

#define SW_OEN		(1 << 7)
#define SW_ST		(1 << 6)
#define SW_IE		(1 << 5)
#define SW_PD		(1 << 4)
#define SW_PU		(1 << 3)
#define SW_DS2		(1 << 2)
#define SW_DS1		(1 << 1)
#define SW_DS0		(1 << 0)
#endif

#define PAD_FUCN_SEL(pbase, n)        (pbase + (n < 8 ? (0xFF60 + (n)*(0x4)) : (0xFB60 + (n - 8)*(0x4))))
#define PAD_MODE_SEL(pbase, n)        (pbase + (n < 16 ? (0xFC00 + (n)*(0x4)) : (0xF800 + (n - 16) * (0x4))))

#define PAD_IO_REG1(pbase, n)		(pbase + 0xfc00 + 0x8*n)
#define PAD_IO_REG2(pbase, n)		(pbase + 0xfc00 + 0x8*n + 0x4)

#define PAD_REG_OFFSET             (4)//4 bytes,only low 8bit valid
#define PAD_FUN_SEL_REG_WID        (8)//1 bit for one pin, total 8 bit 
#define PAD_PER_GROUP_PINS         (4)//2 bit for one pin
#define PAD_INDEX_INVALID          0xFF
#define PAD_IRQ_PER_GROUP          16

#define PAD_IO_PULLUP(pbase, n)	(pbase + (n < 8 ? (0xFDE0 + (n)*(0x4)) : (0xF9E0 + (n - 8)*(0x4))))
#define PAD_IO_PULLDOWN(pbase, n)	(pbase + (n < 8 ? (0xFE40 + (n)*(0x4)) : (0xFA40 + (n - 8)*(0x4))))

#define GPIO_RDAT(gbase, n)		    (gbase + (n)*(0x40) + 0x0)
#define GPIO_WDAT(gbase, n)		    (gbase + (n)*(0x40) + 0x4)
#define GPIO_DIR(gbase, n)          (gbase + (n)*(0x40) + 0x8)

#define GPIO_INTMSK(gbase, n)		(gbase + (n)*(0x40) + 0xC)
#define GPIO_INTGMSK(gbase, n)		(gbase + (n)*(0x40) + 0x10)
#define GPIO_INTPEND(gbase, n)		(gbase + (n)*(0x40) + 0x14)
#define GPIO_INTTYPE(gbase, n)		(gbase + (n)*(0x40) + 0x18)
#define GPIO_FILTER(gbase, n)		(gbase + (n)*(0x40) + 0x1C)
#define GPIO_CLKDIV(gbase, n)		(gbase + (n)*(0x40) + 0x20)
#define GPIO_INTPENDGLB(gbase, n)	(gbase + (n)*4 + 0x4000)



#ifdef CONFIG_TARGET_SFA18_MPW0
#define PAD_INDEX_MAX        62
#elif defined(CONFIG_SOC_SFA28_MPW0) || defined(CONFIG_SOC_SFA28_FULLMASK)
#define PAD_INDEX_MAX        48
#else
#define FUNC_SW_SEL(pbase, n)		(pbase + (n < 8 ? (0xFC40 + (n)*(0x4)) : 0xF840))
#define FUNC_SW_OEN(pbase, n)		(pbase + (n < 8 ? (0xFC80 + (n)*(0x4)) : 0xF880))
#define FUNC_SW_IE(pbase, n)		(pbase + (n < 8 ? (0xFEA0 + (n)*(0x4)) : (0xFAA0 + (n - 8)*(0x4))))
#define PAD_INDEX_MAX        70
#endif

#endif
