/*
 * (C) Copyright 2013 - 2014 Xilinx, Inc
 *
 * Michal Simek <michal.simek@xilinx.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <image.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/u-boot.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <fdtdec.h>
#include <mach/pad.h>


DECLARE_GLOBAL_DATA_PTR;

bool boot_linux;

u32 spl_boot_device(void)
{
	struct udevice *pinctrl;
	int ret;

	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
	    debug("%s: Cannot find pinctrl device\n", __func__);
	    goto err;
	}

#if defined(CONFIG_SPL_SPI_FLASH_SUPPORT)
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_SPI0);
	if (ret) {
	    debug("%s: Failed to set up spi0 pinctrl!\n", __func__);
	    goto err;
	}

	return BOOT_DEVICE_SPI;
#elif defined(CONFIG_SPL_MMC_SUPPORT)
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_EMMC);
	if (ret) {
	    debug("%s: Failed to set up mmc0 pinctrl!\n", __func__);
	    goto err;
	}

	return BOOT_DEVICE_MMC1;//BOOT_DEVICE_MMC2

#endif

err:
	hang();
	return -ENODEV;
}

void board_boot_order(u32 *spl_boot_list)
{
	struct udevice *pinctrl;
	int ret;

	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
	    debug("%s: Cannot find pinctrl device\n", __func__);
	    goto err;
	}

#if defined(CONFIG_SPL_SPI_FLASH_SUPPORT)
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_SPI0);
	if (ret) {
	    debug("%s: Failed to set up spi0 pinctrl!\n", __func__);
	    goto err;
	}
	spl_boot_list[0] = BOOT_DEVICE_SPI;
#elif defined(CONFIG_SPL_MMC_SUPPORT)
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_EMMC);
	if (ret) {
	    debug("%s: Failed to set up mmc0 pinctrl!\n", __func__);
	    goto err;
	}

	spl_boot_list[0] = BOOT_DEVICE_MMC1;
	spl_boot_list[1] = BOOT_DEVICE_MMC2;
	
#endif
	return ;
err:
	hang();
}

#if defined(CONFIG_SPL_MMC_SUPPORT)
u32 spl_boot_mode(void)
{
	return MMCSD_MODE_RAW;
}
#endif


/* Board initialization after bss clearance */
void spl_board_init(void)
{
	/* enable console uart printing */
	preloader_console_init();
}
#ifdef CONFIG_SPL_OS_BOOT
void __noreturn jump_to_image_linux(void *arg)
{
	debug("Entering kernel arg pointer: 0x%p\n", arg);
	typedef void (*image_entry_arg_t)(char *, ulong, ulong)
		__attribute__ ((noreturn));
	image_entry_arg_t image_entry =
		(image_entry_arg_t)spl_image.entry_point;

	image_entry(NULL, 0, (ulong)arg);
}
#endif /* CONFIG_SPL_OS_BOOT */

__weak int spl_start_uboot(void)
{
#ifdef CONFIG_SPL_OS_BOOT
	if (boot_linux)
		return 0;
#endif

	return 1;
}
