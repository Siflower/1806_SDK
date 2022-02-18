/*
 * Copyright (C) 2016 Neil Zhang <neil@siflower.com.cn>
 *
 * SPDX-License-Identifier:	GPL-2.0
 */


/*
 -----------------------------------------------
 |  SPL |  UBOOT | ENV | FACTORY | KERNEL & FS |
 -----------------------------------------------
 | 128K |  384K  | 64K |   64K   |  FREE SPACE |
 -----------------------------------------------
 */


#ifndef _SFA18_CONFIG_H
#define _SFA18_CONFIG_H

/*
 * System configuration
 */
#define CONFIG_SFA18
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_DISPLAY_BOARDINFO

#if defined CONFIG_SFA18_PCBA_TEST || !defined CONFIG_SFA18_UBOOT_LITE
#define CONFIG_MD5
#endif

#ifndef CONFIG_SFA18_UBOOT_LITE
#define CONFIG_AUTO_COMPLETE
#define CONFIG_CMDLINE_EDITING
/*
 *  MMC/SD IP block
 */
#if !defined(CONFIG_SOC_SFA28_MPW0) && !defined(CONFIG_SOC_SFA28_FULLMASK)
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_DWMMC
#endif
#define CONFIG_BOUNCE_BUFFER
/* RAW SD card / eMMC locations. */
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR 256
#define CONFIG_SYS_LONGHELP		/* verbose help, undef to save memory */
#else
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#endif /* CONFIG_SFA18_UBOOT_LITE */

#define CONFIG_MEMSIZE_IN_BYTES

#ifndef CONFIG_SYS_UBOOT_START
#define CONFIG_SYS_UBOOT_START	CONFIG_SYS_TEXT_BASE
#endif

/*
 * CPU Configuration
 */
#define CONFIG_SYS_MHZ			20	/* arbitrary value */
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_MHZ * 1000000)

/* gmac support */
#ifdef CONFIG_SFA18_GMAC
#define CONFIG_MII
#define PHY_ANEG_TIMEOUT        8000    /* PHY needs a longer aneg time */

#define CONFIG_PHY_REALTEK
#define CONFIG_PHY_INTEL_XWAY

#endif
/*
 * Memory map
 */
#define CONFIG_SYS_MALLOC_SIMPLE
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE

#ifdef CONFIG_64BIT
# define CONFIG_SYS_SDRAM_BASE		0xffffffff80000000
#else
# define CONFIG_SYS_SDRAM_BASE		0x80000000
#endif

#define CONFIG_SYS_INIT_SP_OFFSET	0x400000

#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x01000000)
#define CONFIG_SYS_MEMTEST_START	(CONFIG_SYS_SDRAM_BASE + 0x00100000)
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x00800000)

#define CONFIG_SYS_MALLOC_LEN		(1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)
#define CONFIG_SYS_BOOTM_LEN		(64 * 1024 * 1024)

#define CONFIG_SYS_CBSIZE		    256
#define CONFIG_SYS_PBSIZE		    (CONFIG_SYS_CBSIZE + \
					                sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS		    16


#define CONFIG_SPL_TEXT_BASE        0xbc010000 /*test + rodata + data*/
#define CONFIG_SPL_MAX_SIZE         0x12000
#define CONFIG_SPL_BSS_START_ADDR   0xbc022000 /*bss*/
#define CONFIG_SPL_BSS_MAX_SIZE     0x8000
#define CONFIG_SPL_STACK            0xbc02f000 /*stack bottom,grow up!!*/

#ifdef CONFIG_SPI_NAND
#define CONFIG_SYS_SPI_U_BOOT_OFFS	(128*1024) /*  128KB  */
#else
#ifndef CONFIG_SFA18_UBOOT_LITE
#define CONFIG_SYS_SPI_U_BOOT_OFFS	(128*1024) /*  128KB  */
#else
#define CONFIG_SYS_SPI_U_BOOT_OFFS	(32*1024) /*  32KB  */
#endif
#endif
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_MALLOC_F_ADDR		(CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE)

#ifdef CONFIG_TARGET_SFA18_MPW0
#define SFA18_CRYSTAL_6M
#endif


/*
 * Serial driver
 */
#define CONFIG_BAUDRATE			115200


/*
 * Flash configuration
 */
#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SF_DEFAULT_SPEED	33000000
#define CONFIG_SF_DEFAULT_MODE 0


/*
 * Watchdog
 */
#define CONFIG_HW_WATCHDOG
#define CONFIG_SFAX8_WATCHDOG
#define CONFIG_SF_WDT_CLOCK_KHZ		50000
#define CONFIG_HW_WATCHDOG_TIMEOUT_MS 10000



/*
 * Environment
 */
#ifdef CONFIG_SPI_BOOT
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE (4 * 1024)
// #elif defined CONFIG_SPI_NAND_BOOT
// #define CONFIG_ENV_IS_IN_SPI_NAND
// #define CONFIG_ENV_SECT_SIZE (4 * 1024)
#elif defined CONFIG_SPI_NAND_BOOT
#define CONFIG_ENV_IS_IN_SPI_NAND
#define CONFIG_ENV_SECT_SIZE (128 * 1024)
#else
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV 1
#endif

#define CONFIG_ENV_SIZE			0x10000 /* 64KB */
#define CONFIG_ENV_OFFSET (512 * 1024) /* MUST be aligned to an erase sector boundary */

/*
 * Commands
 */
/* Now CONFIG_SFA18_UBOOT_LITE can handle all difference. If we have more
 * different partition in the furture, we can use CONFIG_SFA18_FLASH_SIZE_MB. */
#define SFAX8_SF_READ_DDR_ADDR 0x81000000
#ifdef CONFIG_SPI_NAND
#define SFAX8_SF_READ_FLASH_ADDR 0xc0000
#define SFAX8_SF_READ_SIZE 0x300000
#else //CONFIG_SPI_NAND
#ifdef CONFIG_SFA18_UBOOT_LITE
#define SFAX8_SF_READ_FLASH_ADDR 0x32000
#define SFAX8_SF_READ_SIZE 0x300000
#else /* CONFIG_SFA18_UBOOT_LITE */
#define SFAX8_SF_READ_FLASH_ADDR 0xa0000
#define SFAX8_SF_READ_SIZE 0x300000
#endif /* CONFIG_SFA18_UBOOT_LITE */
#endif //!CONFIG_SPI_NAND

#ifdef CONFIG_SFA18_PCBA_TEST
/* only do pcba test here, don't boot up! */
#define CONFIG_BOOTCOMMAND \
	"sf_pcba_test;"
#else /* CONFIG_SFA18_PCBA_TEST */
#ifdef CONFIG_SPI_BOOT
#define CONFIG_BOOTCOMMAND                     \
	"sf probe 0 33000000;"                 \
	"sf read " __stringify(SFAX8_SF_READ_DDR_ADDR) " " __stringify(SFAX8_SF_READ_FLASH_ADDR) " " __stringify(SFAX8_SF_READ_SIZE) ";" \
	"bootm"
#elif defined CONFIG_SPI_NAND_BOOT /* CONFIG_SPI_BOOT */
#define CONFIG_BOOTCOMMAND                           \
	"spi_nand probe 0 33000000;"                 \
	"spi_nand read " __stringify(SFAX8_SF_READ_DDR_ADDR) " " __stringify(SFAX8_SF_READ_FLASH_ADDR) " " __stringify(SFAX8_SF_READ_SIZE) ";" \
	"bootm"
#else  /* CONFIG_SPI_NAND_BOOT */
#define CONFIG_BOOTCOMMAND                   \
	"mmc dev 1;"                         \
	"mmc read 0x81000000 0x500 0x8000; " \
	"bootm"
#endif /* CONFIG_SPI_BOOT */
#endif /* CONFIG_SFA18_PCBA_TEST */

#define CONFIG_LZMA
#define CONFIG_CMD_LZMADEC

/*
 * Spl device support
 */
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
#define CONFIG_SPL_TIMER_SUPPORT
#define CONFIG_SPL_WATCHDOG_SUPPORT
#define CONFIG_SPL_DMA_SUPPORT
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE


/*
 * Uboot boot device
 */
#ifdef CONFIG_SPI_BOOT
/*spl spi flash  boot*/
#define CONFIG_SPL_SPI_SUPPORT
#define CONFIG_SPL_SPI_FLASH_SUPPORT
#define CONFIG_SPL_SPI_LOAD
#elif defined CONFIG_SPI_NAND_BOOT
/* define SPL_SPI_SUPPORT here to compile httpd correctly */
#define CONFIG_SPL_SPI_SUPPORT
#else
/*spl emm/sd boot*/
#define CONFIG_SPL_MMC_SUPPORT
#endif


/*
 * Web Failsafe configuration added by neil.zhang 20150522
 */

#define WEBFAILSAFE_UPLOAD_RAM_ADDRESS                  0x81000000
/* U-Boot partition size and offset*/

/*cause we use spl + uboot for upgrade, so that upgrade uboot from spl*/
#define WEBFAILSAFE_UPLOAD_UBOOT_ADDRESS                0


#ifdef CONFIG_SPI_NAND
#define WEBFAILSAFE_UPLOAD_UBOOT_SIZE_IN_BYTES          ( 128 * 1024 * 5 )
#else
#define WEBFAILSAFE_UPLOAD_UBOOT_SIZE_IN_BYTES          ( 128 * 1024 * 4 )
#endif
/*Firmware partition offset */
#ifdef CONFIG_SPI_NAND
#define WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS               0xc0000 //CFG_KERN_ADDR
#else
#ifndef CONFIG_SFA18_UBOOT_LITE
#define WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS               0xa0000 //CFG_KERN_ADDR
#else
#define WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS               0x32000 //CFG_KERN_ADDR
#endif
#endif

#ifdef CONFIG_SPI_NAND
#define WEBFAILSAFE_FACTORY_ADDRESS						(0xc0000 - 0x20000)
#else
#ifndef CONFIG_SFA18_UBOOT_LITE
#define WEBFAILSAFE_FACTORY_ADDRESS						(0xa0000 - 0x10000)
#else
#define WEBFAILSAFE_FACTORY_ADDRESS						(0x32000 - 0x1000)
#endif
#endif

#ifndef CONFIG_SFA18_UBOOT_LITE
/*ART partition size and offset*/
#define WEBFAILSAFE_UPLOAD_ART_ADDRESS                  0//CFG_FACTORY_ADDR
#define WEBFAILSAFE_UPLOAD_ART_SIZE_IN_BYTES            ( 16 * 1024 * 1024 )
#endif
/*max. firmware size <= (FLASH_SIZE -  WEBFAILSAFE_UPLOAD_LIMITED_AREA_IN_BYTES)*/
/*progress state info*/
#define WEBFAILSAFE_PROGRESS_START                      0
#define WEBFAILSAFE_PROGRESS_TIMEOUT                    1
#define WEBFAILSAFE_PROGRESS_UPLOAD_READY               2
#define WEBFAILSAFE_PROGRESS_UPGRADE_READY              3
#define WEBFAILSAFE_PROGRESS_UPGRADE_FAILED             4
/*update type*/
#define WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE               0
#define WEBFAILSAFE_UPGRADE_TYPE_UBOOT                  1
#ifndef CONFIG_SFA18_UBOOT_LITE
#define WEBFAILSAFE_UPGRADE_TYPE_ART                    2
#endif
#define WEBFAILSAFE_UPGRADE_TYPE_TEST                   3

#ifdef CONFIG_CMD_BTN_HTTPD
#define CONFIG_PREBOOT  "btn_httpd_detect 192.168.4.1"
#endif

#endif /* _SFA18_CONFIG_H */
