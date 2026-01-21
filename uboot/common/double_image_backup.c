#include <linux/err.h>
#include <common.h>
#include <double_image_backup.h>

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
#include <spi.h>
#include <environment.h>
#include <dm.h>
#include <linux/types.h>
#include <spi-nand.h>
#include <asm-generic/errno.h>
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
#include <linux/err.h>
#include <common.h>
#include <dm/uclass.h>
#include <spi_flash.h>
#endif

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
static struct spi_nand_chip *env_flash;

extern void spi_read(struct spi_nand_chip *flash, loff_t from, size_t len, u_char *buf);
extern void spi_write(struct spi_nand_chip *flash, loff_t to, size_t len, const u_char *buf);
extern void spi_erase(struct spi_nand_chip *flash, loff_t off, size_t len);
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
static struct udevice *udev;
#endif

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
static void sf_spinand_probe(void)
{
	int ret;
	struct udevice *udev;
	struct spi_nand_chip *chip;

	ret = uclass_get_device(UCLASS_SPI_NAND, 0, &udev);
	if (ret == 0) {
		chip = dev_get_uclass_priv(udev);
		if (chip) {
			env_flash = chip;
		}
	}
}
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
static struct udevice *get_sf_udev(void)
{
	int ret;
	ret = uclass_get_device(UCLASS_SPI_FLASH, 0, &udev);
	return (ret != 0) ? NULL : udev;
}

static void spi_read(loff_t from, size_t len, u_char *buf)
{
	spi_flash_read_dm(udev, from, len, buf);
}

static void spi_write(loff_t to, size_t len, const u_char *buf)
{
	spi_flash_write_dm(udev, to, len, buf);
}

static void spi_erase(loff_t off, size_t len)
{
	spi_flash_erase_dm(udev, off, len);
}
#endif

static unsigned int get_flag_on_flash(void)
{
	unsigned int flash_flag = 0;
	unsigned char factory[FACTORY_SIZE];

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
	spi_read(env_flash, FACTORY_OFFSET, FACTORY_SIZE, factory);
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
	spi_read(FACTORY_OFFSET, FACTORY_SIZE, factory);
#endif
	flash_flag = *((unsigned int*)(factory + FACTORY_BACKUP_FLAG_OFFSET));

	return flash_flag;
}

static void set_flag_on_flash(unsigned int value)
{
	unsigned char factory[FACTORY_SIZE];

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
	spi_read(env_flash, FACTORY_OFFSET, FACTORY_SIZE, factory);
	spi_erase(env_flash, FACTORY_OFFSET, FACTORY_SIZE);
	*((unsigned int*)(factory + FACTORY_BACKUP_FLAG_OFFSET)) = value;
	spi_write(env_flash, FACTORY_OFFSET, FACTORY_SIZE, factory);
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
	spi_read(FACTORY_OFFSET, FACTORY_SIZE, factory);
	spi_erase(FACTORY_OFFSET, FACTORY_SIZE);
	*((unsigned int*)(factory + FACTORY_BACKUP_FLAG_OFFSET)) = value;
	spi_write(FACTORY_OFFSET, FACTORY_SIZE, factory);
#endif
}

static u32 get_flag_on_ram(void)
{
	u32 value = *((volatile u32*)BAKUP_FLAG_ON_RAM_ADDRESS);
	return value;
}

static void set_flag_on_ram(u32 value)
{
	*((volatile u32*)BAKUP_FLAG_ON_RAM_ADDRESS) = value;
}

int need_boot_backup_img(void)
{
	u32 flash_flag, ram_flag;
#if defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
	sf_spinand_probe();
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP)
	get_sf_udev();
#endif

	flash_flag = get_flag_on_flash();
	ram_flag = get_flag_on_ram();

	if (ram_flag == LAST_BOOTUP_FAILED) {
		if (flash_flag == MASTER_IMAGE) {
			set_flag_on_flash(BACKUP_IMAGE);
			return BOOT_BACKUP;
		} else if (flash_flag == BACKUP_IMAGE) {
			set_flag_on_flash(MASTER_IMAGE);
			return BOOT_MASTER;
		} else {
			printf("boot backup image fail\n");
			return MASTER_IMAGE;
		}
	} else if (ram_flag == LINUX_REBOOT) {
		if (flash_flag == MASTER_IMAGE)
			return BOOT_MASTER;
		else
			return BOOT_BACKUP;
	} else {
		//if first boot ,we set master img as default
		return BOOT_MASTER;
	}
}

void staring_up_image(void)
{
	set_flag_on_ram(BOOTUP_IMAGE);
}
