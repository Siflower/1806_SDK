#include <common.h>
#include <mach/pcba_test.h>
#include <dm/uclass.h>
#include <spi_flash.h>

#ifdef CONFIG_MD5
#include <u-boot/md5.h>
#endif

const struct test_module test_list[] = {
#ifdef CONFIG_SFAX8_NPU_TEST
	{ "eth", npu_test }, /* NPU */
#endif
#ifdef CONFIG_SFAX8_GMAC_TEST
	{ "gmac", gmac_test }, /* GMAC */
#endif
#ifdef CONFIG_SFAX8_WIFI_TEST
	{ "wifi", wifi_test }, /* WIFI */
#endif
#ifdef CONFIG_SFAX8_GPIO_TEST
	{ "gpio", gpio_test }, /* GPIO */
#endif
#ifdef CONFIG_SFAX8_BUTTON_TEST
	{ "button", button_test }, /* GPIO */
#endif
#ifdef CONFIG_SFAX8_SD_TEST
	{ "sd card", sd_test }, /* SDIO */
#endif
#ifdef CONFIG_SFAX8_USB_TEST
	{ "usb", usb_test }, /* USB */
#endif
#ifdef CONFIG_SFAX8_AUDIO_TEST
	{ "codec", codec_test }, /* PCM/IIS */
#endif
#ifdef CONFIG_SFAX8_PMU_TEST
	{ "pmu", pmu_test }, /* IIC */
#endif
#ifdef CONFIG_SFAX8_GDU_TEST
	{ "lcd", lcd_test }, /* GDU */
#endif
#ifdef CONFIG_SFAX8_SHARE_MEM_TEST
	{ "sha_mem", share_memory_test }, /* share memory */
#endif
	{ "spi", spi_test }, /* SPI */
	{ "ddr", ddr_test } /* DDR */
};
const int test_list_sz = ARRAY_SIZE(test_list);

int sf_flash_size = 0;

/*
 * Commands analyzation.
 */
int sf_get_chipid(unsigned char *buf, unsigned int *len)
{
	long long chipid;

	chipid = read_chipid();
	if (chipid == -ETIMEDOUT)
		return CMD_TIMEOUT;
	if (chipid < 0)
		return CMD_FAIL;
	memcpy(buf, &chipid, sizeof(chipid));
	*len = *len + sizeof(chipid);

	return CMD_SUCCESS;
}

int sf_get_chipinfo(unsigned char *buf, unsigned int *len)
{
	struct chipinfo *c = (struct chipinfo *)buf;

	c->ddr_size = CONFIG_SYS_MEM_SIZE;
	c->flash_size = sf_flash_size;
	*len = *len + sizeof(*c);

	return CMD_SUCCESS;
}

int sf_get_testlist(unsigned char *buf, unsigned int *len)
{
	int i = 0;

	if (!test_list_sz) {
		printf("Error: empty test list!\n");
		return CMD_FAIL;
	}

	for (i = 0; i < test_list_sz; i++) {
		// set 0xff so PC code can use strstr() func easily.
		memset(buf + *len, 0xff, CASE_NAMELEN);
		memcpy(buf + *len, test_list[i].name, strlen(test_list[i].name));
		*len += CASE_NAMELEN;
	}

	return CMD_SUCCESS;
}

/*
 * | --4-- | --4-- |
 * | start | lenth |
 */
int sf_dev_erase(void *buf)
{
	struct udevice *udev;
	unsigned int offset;
	int ret, len;

	if (!buf)
		return CMD_FAIL;

	offset = *(unsigned int *)buf;
	len = *((int *)buf + 1);
	len -= (len % DEV_ERASE_SZ) ? (len % DEV_ERASE_SZ) : DEV_ERASE_SZ;
	len += DEV_ERASE_SZ;
	printf("offset 0x%x, len %d\n", offset, len);
	if (offset < SF_FACTORY_OFFSET ||
			offset > (SF_FACTORY_OFFSET + SF_FACTORY_SIZE))
		return ERROR_CMD;

	ret = uclass_get_device(UCLASS_SPI_FLASH, 0, &udev);
	if (ret != 0)
		return CMD_FAIL;

	ret = spi_flash_erase_dm(udev, offset, len);
	if (ret == -ETIMEDOUT)
		return CMD_TIMEOUT;

	return  ret == 0 ? CMD_SUCCESS : CMD_FAIL;
}

/*
 * | --4-- | --4-- |
 * | start | lenth |
 */
int sf_dev_read(void *buf, void *send_buf, unsigned int *l)
{
	struct udevice *udev;
	unsigned int offset, len;
	int ret;

	if (!buf)
		return CMD_FAIL;

	offset = *(unsigned int *)buf;
	len = *((int *)buf + 1);
	if (len > SF_MAX_BUF_SIZE - sizeof(struct packet_header))
		len = SF_MAX_BUF_SIZE - sizeof(struct packet_header);
	*l = len;
	printf("offset 0x%x, len %d\n", offset, len);
	if (offset == 0)
		return ERROR_CMD;

	ret = uclass_get_device(UCLASS_SPI_FLASH, 0, &udev);
	if (ret != 0) {
		return CMD_FAIL;
	}

	ret = spi_flash_read_dm(udev, offset, len, send_buf);
	if (ret == -ETIMEDOUT)
		return CMD_TIMEOUT;

	return  ret == 0 ? CMD_SUCCESS : CMD_FAIL;
}

/*
 * | --4-- | --4-- | ---- |
 * | start | lenth | data |
 */
int sf_dev_write(void *buf)
{
	struct udevice *udev;
	unsigned int offset;
	int write_len, ret;
	char *data;

	if (!buf)
		return CMD_FAIL;

	offset = *(unsigned int *)buf;
	write_len = *((int *)buf + 1);
	data = (char *)buf + 4 + 4;
	/*
	 * The max data lenth for one command is 256 bytes(8 bit).
	 */
	if (write_len > 256)
		write_len = 256;

	if (offset < SF_FACTORY_OFFSET ||
			offset > (SF_FACTORY_OFFSET + SF_FACTORY_SIZE))
		return ERROR_CMD;

	ret = uclass_get_device(UCLASS_SPI_FLASH, 0, &udev);
	if (ret != 0)
		return CMD_FAIL;

	ret = spi_flash_write_dm(udev, offset, write_len, data);
	if (ret == -ETIMEDOUT)
		return CMD_TIMEOUT;

	return ret  == 0 ? CMD_SUCCESS : CMD_FAIL;
}

int sf_probe(void)
{
	int ret = 0;
#ifdef CONFIG_SPI_BOOT
	ret = run_command_list("sf probe 0 33000000", -1, 0);
	printf("sf probe 0 33000000\n");
#elif defined CONFIG_SPI_NAND_BOOT /* CONFIG_SPI_BOOT */
	ret = run_command_list("spi_nand 0 33000000", -1, 0);
	printf("spi_nand 0 33000000\n");
#else  /* CONFIG_SPI_NAND_BOOT */
	ret = run_command_list("mmc dev 1", -1, 0);
	printf("mmc dev 1\n");
#endif /* CONFIG_SPI_BOOT */
	return  ret == 0 ? CMD_SUCCESS : CMD_FAIL;
}

int sf_read_to_ddr(void *buf)
{
	int ret = 0;
	int ddr_addr = 0, read_size = 0, flash_addr = 0;
	char cmd[64];

	memset(cmd, 0, sizeof(cmd));
	if (!buf)
		return CMD_FAIL;

	ddr_addr = *(unsigned int *)buf;
	read_size = *((int *)buf + 1);
	flash_addr = *((int *)buf + 2);

#ifdef CONFIG_SPI_BOOT
	sprintf(cmd,"sf read %#x %#x %#x", ddr_addr, read_size, flash_addr);
#elif defined CONFIG_SPI_NAND_BOOT /* CONFIG_SPI_BOOT */
	sprintf(cmd,"spi_nand %#x %#x %#x", ddr_addr, read_size, flash_addr);
#else  /* CONFIG_SPI_NAND_BOOT */
	sprintf(cmd,"mmc read %#x %#x %#x", ddr_addr, read_size, flash_addr);
#endif /* CONFIG_SPI_BOOT */
	printf(cmd);
	printf("\n");
	ret = run_command_list(cmd, -1, 0);

	return  ret == 0 ? CMD_SUCCESS : CMD_FAIL;
}

int sf_check_md5(void *buf)
{
	uint32_t addr = 0, len = 0;
	unsigned char cal_md5[16], buf_md5[16];

	memset(cal_md5, 0, sizeof(cal_md5));
	memset(buf_md5, 0, sizeof(buf_md5));
	if (!buf)
		return CMD_FAIL;

	addr = *(unsigned int *)buf;
	len = *((int *)buf + 1);
	memcpy(buf_md5, (unsigned char *)buf + 8, sizeof(buf_md5));
#ifdef CONFIG_MD5
	md5((unsigned char *)addr, len, cal_md5);
#else
	// TODO: we have to use another way to check it.
	return CMD_SUCCESS;
#endif
	printf("md5 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x, addr %#x, len %d\n", buf_md5[0], buf_md5[1],
			buf_md5[2], buf_md5[3], buf_md5[4], buf_md5[5], buf_md5[6], buf_md5[7], buf_md5[8], buf_md5[9], buf_md5[10], buf_md5[11], buf_md5[12], buf_md5[13], buf_md5[14], buf_md5[15], addr, len);
	printf("cal_md5 front 4 bytes 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", cal_md5[0], cal_md5[1],
			cal_md5[2], cal_md5[3], cal_md5[4], cal_md5[5], cal_md5[6], cal_md5[7], cal_md5[8], cal_md5[9], cal_md5[10], cal_md5[11], cal_md5[12], cal_md5[13], cal_md5[14], cal_md5[15]);

	return  memcmp(cal_md5, buf_md5, sizeof(cal_md5)) ? CMD_FAIL : CMD_SUCCESS;
}

long long __weak read_chipid(void)
{
	int timeout = 0, fail = 0;

	if (timeout == 1)
		return -ETIMEDOUT;
	if (fail == 1)
		return -1;

	return 0x1234;
}

int __weak npu_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak gmac_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak wifi_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak gpio_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak button_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak sd_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak usb_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak codec_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak pmu_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak lcd_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}
int __weak share_memory_test(void *d)
{
	printf("weak: %s\n", __func__);
	return CMD_FAIL;
}

/*
 * SPI & DDR always returns success.
 */
int spi_test(void *d)
{
	return CMD_SUCCESS;
}

int ddr_test(void *d)
{
	return CMD_SUCCESS;
}
