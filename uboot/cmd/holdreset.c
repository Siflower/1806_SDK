/*
 * Copyright (C) 2017 SiFlower.
 *
 * Nevermore Wang <nevermore.wang@siflower.com.cn>
 *
 * This command is used to hold all module resets in SFAX8.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <errno.h>
#include <image.h>
#include <malloc.h>
#include <mapmem.h>
#include <asm/byteorder.h>
#include <linux/compiler.h>
#include <linux/ctype.h>
#include <linux/err.h>
#include <u-boot/zlib.h>
#include <asm/io.h>
#include <mach/sfax8.h>

#define CLK_EN_OFFSET		0x04
#define BOE_OFFSET		0x0C
#ifdef SF19A28
#define MEMCTL_OFFSET		0x14
#endif
#define IRAM_SOFT_RESET1_OFF	0x1c
#define IRAM_SOFT_CLK_EN1_OFF	0x20
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct reset{
	unsigned int soft_reset;
	unsigned int sysm_reset;
};

static struct reset resets[] =
{
	{
		.soft_reset = NPU_SOFT_RESET,
		.sysm_reset = NPU_SYSM_RESET,
	},
	{
		.soft_reset = EMAC_SOFT_RESET,
		.sysm_reset = EMAC_SYSM_RESETN,
	},
#ifndef CONFIG_TARGET_SFA18_MPW0
	{
		.soft_reset = GDU_SOFT_RESET,
		.sysm_reset = GDU_SYSM_RESET,
	},
	{
		.soft_reset = CRYPTO_SOFT_RESET,
		.sysm_reset = CRYPTO_SYSM_RESET,
	},
#endif
	{
		.soft_reset = WIFI_1_SOFT_RESET,
		.sysm_reset = WIFI_1_SYSM_RESET,
	},
	{
		.soft_reset = WIFI_2_SOFT_RESET,
		.sysm_reset = WIFI_2_SYSM_RESET,
	},
	{
		.soft_reset = DMA_SOFT_RESET,
		.sysm_reset = DMA_SYSM_RESET,
	},
	{
		.soft_reset = SCDMA_SOFT_RESET,
		.sysm_reset = SCDMA_SYSM_RESET,
	},
	{
		.soft_reset = USB_SOFT_RESET,
		.sysm_reset = USB_SYSM_RESET,
	},
	{
		.soft_reset = EMMC_SOFT_RESET,
		.sysm_reset = EMMC_SYSM_RESET,
	},
	{
		.soft_reset = SDIO_SOFT_RESET,
		.sysm_reset = SDIO_SYSM_RESET,
	},
	{
		.soft_reset = CATIP_SOFT_RESET,
		.sysm_reset = CATIP_SYSM_RESET,
	},
	{
		.soft_reset = IIS_SOFT_RESET,
		.sysm_reset = IIS_SYSM_RESET,
	},
	{
		.soft_reset = IIC_SOFT_RESET,
		.sysm_reset = IIC_SYSM_RESET,
	},
	{
		.soft_reset = SPI_SOFT_RESET,
		.sysm_reset = SPI_SYSM_RESET,
	},
	{
		.soft_reset = UART_SOFT_RESET,
		.sysm_reset = UART_SYSM_RESET,
	},
	{
		.soft_reset = PCM_SOFT_RESET,
		.sysm_reset = PCM_SYSM_RESET,
	},
	{
		.soft_reset = PWM_SOFT_RESET,
		.sysm_reset = PWM_SYSM_RESET,
	},
	{
		.soft_reset = TIMER_SOFT_RESET,
		.sysm_reset = TIMER_SYSM_RESET,
	},
	{
		.soft_reset = WDT_SOFT_RESET,
		.sysm_reset = WDT_SYSM_RESET,
	},
	{
		.soft_reset = SPDIF_SOFT_RESET,
		.sysm_reset = SPDIF_SYSM_RESET,
	},
	{
		.soft_reset = IRAM_SOFT_RESET,
		.sysm_reset = IRAM_SYSM_RESET,
	},
	{
		.soft_reset = IROM_SOFT_RESET,
		.sysm_reset = IROM_SYSM_RESET,
	},
};
int npuphy_disable(void)
{
	writel(0, (void*)OMINI_PHY_ENABLE);
	return 0;
}
EXPORT_SYMBOL(npuphy_disable);

int usbphy_sleep(void)
{
	writel(0, (void*)USB_SLEEPM0);
	return 0;
}
EXPORT_SYMBOL(usbphy_sleep);

unsigned int sf_get_sysm_offset(unsigned int offset)
{
	int i = 0;
	for( ; i < ARRAY_SIZE(resets); i++)
		if(resets[i].soft_reset == offset)
			return resets[i].sysm_reset;
	return 0;
}


int hold_reset(unsigned int addr)
{
	unsigned int ret;
	ret = sf_get_sysm_offset(addr);
	if(ret){
		writel(0x0, (void*)(ret));
		writel(0x1, (void*)(ret));
	}else
		return -EINVAL;

	writel(0xFF, (void*)addr);
	writel(0x0, (void*)(addr + CLK_EN_OFFSET));
	if(addr == IRAM_SOFT_RESET){
		writel(0xFF, (void*)addr + IRAM_SOFT_RESET1_OFF);
		writel(0x0, (void*)(addr + IRAM_SOFT_CLK_EN1_OFF));
	}
	writel(0x0, (void*)(addr + BOE_OFFSET));
#ifdef SF19A28
	switch (addr) {
		case NPU_SOFT_RESET:
		case EMAC_SOFT_RESET:
		case CATIP_SOFT_RESET:
		case WIFI_1_SOFT_RESET:
		case WIFI_2_SOFT_RESET:
		case USB_SOFT_RESET:
			writel(BIT(1), (void*)(addr + MEMCTL_OFFSET));
			break;
		default:
			break;
	}
#endif


	return 0;
}

void hold_all_module_reset(void)
{
	//iram
	hold_reset(IRAM_SOFT_RESET);
	//irom
	hold_reset(IROM_SOFT_RESET);
	/* apb */
	//i2s
	hold_reset(IIS_SOFT_RESET);
	//i2c
	hold_reset(IIC_SOFT_RESET);
	//spi
	hold_reset(SPI_SOFT_RESET);
	//uart in use
	//pcm
	hold_reset(PCM_SOFT_RESET);
	//pwm
	hold_reset(PWM_SOFT_RESET);
	//timer
	hold_reset(TIMER_SOFT_RESET);
	//watchdog
	hold_reset(WDT_SOFT_RESET);
	//spdif
	hold_reset(SPDIF_SOFT_RESET);

	/* GDMA */
	hold_reset(DMA_SOFT_RESET);

	/* NPU */
	hold_reset(NPU_SOFT_RESET);

	/* WIFI */
	hold_reset(CATIP_SOFT_RESET);
	hold_reset(WIFI_2_SOFT_RESET);
	hold_reset(WIFI_1_SOFT_RESET);
	hold_reset(SCDMA_SOFT_RESET);

	/* USB */
	hold_reset(USB_SOFT_RESET);

	/* GMAC */
	hold_reset(EMAC_SOFT_RESET);

	/* EMMC */
	hold_reset(EMMC_SOFT_RESET);

	/* SDIO */
	hold_reset(SDIO_SOFT_RESET);

	/* GDU */
	hold_reset(GDU_SOFT_RESET);

	/* CRYPTO */
	hold_reset(CRYPTO_SOFT_RESET);

#ifndef CONFIG_TARGET_SFA18_MPW0
	//disable the main clk output debug enable
	writel(0x0, (void*)CATIP_MAIN_CLK_DEBUG_EN);
#endif

	printf("All resets are held!\n");

}
EXPORT_SYMBOL(hold_all_module_reset);

int do_rhreset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	hold_all_module_reset();
	/* Power down usb/npu phy */
	usbphy_sleep();
	npuphy_disable();
	return 0;
}

U_BOOT_CMD(
	rhreset, CONFIG_SYS_MAXARGS, 1, do_rhreset,
	"Rehold all module resets to save power.",
	""
);
