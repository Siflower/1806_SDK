#ifndef DOUBLE_IMAGE_BACKUP_
#define DOUBLE_IMAGE_BACKUP_

#include <spi.h>

#if defined(CONFIG_DOUBLE_IMAGE_BACKUP)
#define FACTORY_OFFSET 0x90000
#define FACTORY_SIZE 0x10000
#elif defined(CONFIG_DOUBLE_IMAGE_BACKUP_NAND)
#define FACTORY_OFFSET 0xa0000
#define FACTORY_SIZE 0x20000
#endif

#define FACTORY_BACKUP_FLAG_OFFSET 280
#define BAKUP_FLAG_ON_RAM_ADDRESS 0xbc00fffc
/* ram flag */
#define LAST_BOOTUP_FAILED 0xAAAAAAAA
#define LINUX_REBOOT 0xFFFFFFFF
#define BOOTUP_IMAGE 0xAAAAAAAA
/* flash flag */
#define MASTER_IMAGE 0xFFFFFFFF
#define BACKUP_IMAGE 0xEEEEEEEE
/* return flag */
#define BOOT_MASTER 0
#define BOOT_BACKUP 1

#ifndef CONFIG_ENV_SPI_BUS
#define CONFIG_ENV_SPI_BUS	0
#endif
#ifndef CONFIG_ENV_SPI_CS
#define CONFIG_ENV_SPI_CS	0
#endif
#ifndef CONFIG_ENV_SPI_MAX_HZ
#define CONFIG_ENV_SPI_MAX_HZ	1000000
#endif
#ifndef CONFIG_ENV_SPI_MODE
#define CONFIG_ENV_SPI_MODE	SPI_MODE_3
#endif

void staring_up_image(void);
int need_boot_backup_img(void);

#endif