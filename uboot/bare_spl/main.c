/*
 *	Siflower SF16A18 bootloader.
 *	File Name          : main.c
 *	Author             : Nevermore
 *	Date First Issued  : 01/04/2018
 */
#include <io.h>
#include <stdio.h>
#include <common/stdiol.h>
#include <sys_manager.h>
#include <clk.h>
#include <pad.h>
#include <errorno.h>
#include <ddr_init.h>
#include <uart.h>
#include <spi.h>
#include <flash.h>
#include <spi-nand.h>
#include <mmc.h>
#include "../include/spl_uboot_common.h"
#ifdef POE
#include <ip808.h>
#endif

#ifdef SECURITY_BOOT
#include <security_boot.h>
#endif

#define PCBA_TEST

#ifdef SPI_NAND
#define SYS_SPI_U_BOOT_OFFS	(128 * 1024)
#define SYS_FACTORY_OFFS	(SYS_SPI_U_BOOT_OFFS + 4 * 128 * 1024)
#define SYS_SPI_PCBA_OFFS	(16 * 1024 * 1024 - 5 * 128 * 1024)
#else
#ifdef CONFIG_SFA18_UBOOT_LITE
#define SYS_SPI_U_BOOT_OFFS	(32 * 1024)
#define SYS_FACTORY_OFFS	(SYS_SPI_U_BOOT_OFFS + 164 * 1024)
#define SYS_SPI_PCBA_OFFS	(4 * 1024 * 1024 - 256 * 1024)
#else /* CONFIG_SFA18_UBOOT_LITE */
#define SYS_SPI_U_BOOT_OFFS	(128 * 1024)
#define SYS_FACTORY_OFFS	(SYS_SPI_U_BOOT_OFFS + 384 * 1024 + 64 * 1024)
#if (SFA18_FLASH_SIZE_MB == 8)
#define SYS_SPI_PCBA_OFFS	(8 * 1024 * 1024 - 512 * 1024)
#elif (SFA18_FLASH_SIZE_MB == 32)
#define SYS_SPI_PCBA_OFFS	(32 * 1024 * 1024 - 512 * 1024)
#else
#define SYS_SPI_PCBA_OFFS	(16 * 1024 * 1024 - 512 * 1024)
#endif
#endif /* CONFIG_SFA18_UBOOT_LITE */
#endif

#define DL_SPI_CTRL_NO          0
#define DL_SDIO_BLKSIZ	        (0x200)
#define DL_EMMC_BLKSIZ	        (0x200)

#define IH_OS_U_BOOT            17
#define IH_COMP_NONE		0	/*  No	 Compression Used	*/
#define IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define IH_NMLEN		32


typedef struct image_header {
	__be32		ih_magic;	/* Image Header Magic Number	*/
	__be32		ih_hcrc;	/* Image Header CRC Checksum	*/
	__be32		ih_time;	/* Image Creation Timestamp	*/
	__be32		ih_size;	/* Image Data Size		*/
	__be32		ih_load;	/* Data	 Load  Address		*/
	__be32		ih_ep;		/* Entry Point Address		*/
	__be32		ih_dcrc;	/* Image Data CRC Checksum	*/
	u8		ih_os;		/* Operating System		*/
	u8		ih_arch;	/* CPU architecture		*/
	u8		ih_type;	/* Image Type			*/
	u8		ih_comp;	/* Compression Type		*/
	u8		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t;

typedef enum boot_dev_t
{
	BOOT_SPI_FLASH,
	BOOT_SPI_NAND,
	BOOT_EFLASH,
	BOOT_EMMC,
	BOOT_SD
} boot_dev;

enum led_cmd {
	LED_ON,
	LED_OFF,
	LED_TOGGLE,
	LED_BLINK
};

static __inline__ u32 ___arch__swab32(__u32 x)
{
	__asm__(
	"	wsbh	%0, %1			\n"
	"	rotr	%0, %0, 16		\n"
	: "=r" (x)
	: "r" (x));

	return x;
}
#define __arch__swab32(x)	___arch__swab32(x)

static int flash_read(int bldev, unsigned int off, unsigned char *buf, u32 sz)
{
	switch (bldev) {
	case BOOT_SPI_FLASH:
	case BOOT_EFLASH:
		return SPI_FLASH_Read(DL_SPI_CTRL_NO, off, buf, sz);
	case BOOT_SPI_NAND:
		return SPI_NAND_read(off, buf, DL_SPI_CTRL_NO, sz);
#ifdef MMC
	case BOOT_EMMC:
		mmc_data_read(off, buf, DL_EMMC_BLKSIZ, sz);
		break;
	case BOOT_SD:
		mmc_data_read(off, buf, DL_SDIO_BLKSIZ, sz);
		break;
#endif
	default:
		return -EINVAL;
	}

	return 0;
}

#ifdef SECURITY_BOOT
/*TODO read rsa pubkey from efuse
* but now there is no data in efuse.
* The key data stored in flash temporarily.
*/
static int read_pubkey(unsigned char *key)
{

	return 0;
}
#endif

#ifdef CONFIG_CMD_BOOT_TEST
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memset(void *s,int ch,size_t n);
#endif
/*
 * Return the entry point of uboot.
 * Retrun 0 on failure.
 */
static int load_image(int bl_dev, int boot)
{
#ifdef CONFIG_CMD_BOOT_TEST
	char boot_from_ram = 0;
	struct spl_uboot_common_header *such = NULL;
#endif
	image_header_t header;
	unsigned int size, load, magic, offset;
	unsigned char factory[32];
	int err;

	if ((err = flash_read(bl_dev, SYS_FACTORY_OFFS, factory, 32)))
		return err;
	if (factory[23] == 'P' && factory[24] == 'C' && factory[25] == 'B' &&
			factory[26] == 'T')
		offset = SYS_SPI_PCBA_OFFS;
	else
		offset = SYS_SPI_U_BOOT_OFFS;

#ifdef CONFIG_CMD_BOOT_TEST
	boot_from_ram = 0;
	such = (struct spl_uboot_common_header*)(BOOT_TEST_HEADER_POS);
	if (such->magic[0] == BOOT_TEST_MAGIC_LOW &&
	    such->magic[1] == BOOT_TEST_MAGIC_HIGH) {
		printf("find boot from ram miagic\n");
		boot_from_ram = 1;
	}
#endif
#ifdef CONFIG_CMD_BOOT_TEST
	if (boot_from_ram)
		header = *((image_header_t *)(such->address));
	else
#endif
		flash_read(bl_dev, offset, (unsigned char *)&header, sizeof(header));

	magic = __arch__swab32(header.ih_magic);
	if (magic != IH_MAGIC)
		return boot ? 0 : -EFAULT;

	if (header.ih_os != IH_OS_U_BOOT)
		return 0;

	if (!boot)
		return 0;

	/* TODO: check header & data crc */
	size = __arch__swab32(header.ih_size);
	load = __arch__swab32(header.ih_load);

	/* If uboot name longer than IH_NMLEN bytes, cut down it. */
	header.ih_name[IH_NMLEN - 1] = '\0';
	printf("U-image: %s, size is %d\n", header.ih_name, size);

#ifdef CONFIG_CMD_BOOT_TEST
	/*
	 * clean the magic of boot from ram
	 *
	 * */
	such->magic[0] = 0;
	such->magic[1] = 0;

	if (boot_from_ram)
		memcpy((void*)load, (void *)(such->address + sizeof(header)), size);
	else
#endif
		flash_read(bl_dev, offset + sizeof(header), (unsigned char *)load, size);
#ifdef SECURITY_BOOT
	unsigned char key[RSA_KEY_LEN] = { 0 };
	unsigned  int start, end;
	/* TODO: check the sign of the image. */
	/*FIXME: enable timer to calculate how much time the process of
	* verifing image will spend. It should be removed lately.
	*/

	//disable timer
	writel(0, (void *)0xB8600008);
	writel(0xFFFFFFFF, (void *)0xB8600000);
	//enable timer in free-running mode
	writel(0x5, (void *)0xB8600008);
	start = readl((void *)0xB8600004);

	read_pubkey(key);

	rsa_verify_image((void *)load, size, (void *)key);

	end = readl((void *)0xB8600004);
	printf("The process of verifing img cast %d us\n", (start > end ? (start - end) : ((0xFFFFFFFF - end) + start)) / 198);
#endif
	return __arch__swab32(header.ih_ep);
}

static boot_dev get_boot_dev(void)
{
	u32 devid = 0;

#ifndef SKIP_SPI_FLASH
#ifdef SF19A28
	sf_module_set_pad_func(SF_SPI2);
#else
	sf_module_set_pad_func(SF_SPI0);
#endif
	devid = SPI_FLASH_GetDeviceID(0);
#endif

	if ((devid != 0xffffff) && (devid != 0)) {
		if (load_image(BOOT_SPI_FLASH, 0)) {
			printf("Boot from spi-nand\n");
			return BOOT_SPI_NAND;
		} else {
			printf("Boot from spi-flash\n");
			return BOOT_SPI_FLASH;
		}
#ifdef MMC
	} else {
		devid = (u32)mmc_initialization(1);

		if(devid == 0x1234) {
			printf("Boot from sd\n");
			return BOOT_SD;
		} else {
			sf_module_set_pad_func(SF_EMMC);
			devid = (u32)mmc_initialization(0);

			if (devid == 0x4321) {
				printf("Boot from emmc\n");
				return BOOT_EMMC;
			} else {
				printf("Error: No booting device!!!\n");
			}
		}
#endif
	}

	return -ENODEV;
}

static int preloader_console_init(void)
{
#ifdef SF19A28
	int reg = readl(PAD_INDEX_REG0(10));
	reg |= (1 << 3);
	reg &= ~(1 << 4);
	writel(reg, PAD_INDEX_REG0(10));
#endif
	return uart_init_debug_channel(UART_DEBUG_CHANNEL);
}

static unsigned int bl_load_image(void)
{
	unsigned int addr = 0;
	int bl_dev = BOOT_SPI_FLASH;

	bl_dev = get_boot_dev();
	if (bl_dev < 0)
		goto err;

	addr = load_image(bl_dev, 1);
	if (addr == 0)
		goto err;

	return addr;
err:
	printf("%s failed\n", __func__);
	return 0;
}

#ifndef SF19A28
static int do_led(int id, int cmd)
{
	int i, blink_time = 10;

	sf_pad_set_func(id, GPIO_OUTPUT);

	switch (cmd) {
	case LED_ON:
		sf_pad_set_value(id, 0);
		break;
	case LED_OFF:
		sf_pad_set_value(id, 1);
		break;
	case LED_TOGGLE:
		sf_pad_set_value(id, !sf_pad_get_value(id));
		break;
	case LED_BLINK:
		for (i = 0; i < blink_time; i++) {
			do_led(id, LED_TOGGLE);
			m_delay(50);
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
#endif

static void bl_prepare_for_boot(void)
{
	/* Nothing to do! */
	return;
}

static void jump_to_image(unsigned int addr)
{
	((void (*)())addr)();

	return;
}

void hang(void)
{
	puts("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

/*
 *	Main function.
 *	the cpu_num is get from start.S,and the
 *	setup should only be done by core0 vpe0.
 */
int main(int cpu_num, char **arv)
{
	unsigned int addr;
#ifdef SF19A28_FULLMASK
	int i;
#endif

	if (cpu_num == 0) {
		/* clock gating, sys-manager init */
		clk_gating_init();

#ifndef MPW0
		clk_update();
#endif
		sys_init();

		/* reset JTAG iomux */
		sf_module_set_pad_func(SF_JTAG);

		/* set RGMII iomux */
		sf_module_set_pad_func(SF_RGMII);

		/* disable clk_out */
#ifndef SF19A28_FULLMASK
		sf_pad_set_func(48, GPIO_INPUT);
#else
		sf_pad_set_func(46, GPIO_INPUT);
		/* set USB iomux */
		sf_module_set_pad_func(SF_USB);

		/* set RF iomux */
		sf_module_set_pad_func(SF_RF);
		for(i=30; i < 46;i++)
			sf_pad_set_ds(i,0x7);
#endif

		preloader_console_init();


        /*don't flash gpio 36 case 36 is an function pad for A28*/
#ifndef SF19A28
		/* set led on */
		do_led(
#ifdef LED_ID
                LED_ID,
#else
                36,
#endif
                LED_ON);
#endif

		printf("SiFlower SFAX8 Bootloader (%s - %s)\n",
							   __DATE__, __TIME__);

		ddr_init();
#ifdef POE
		sfax8_ip808_init();
#endif
		/* load uboot */
		addr = bl_load_image();
		if (addr == 0)
			hang();

		printf("loaded - jumping to U-Boot 0x%x...\n", addr);
		bl_prepare_for_boot();

		jump_to_image(addr);
	}

	return 0;
}
