/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <dm.h>
#include <spi.h>
#include <spi-nand.h>
#include <asm/io.h>

#include <dm/device-internal.h>

#ifndef CONFIG_SF_DEFAULT_SPEED
# define CONFIG_SF_DEFAULT_SPEED	1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
# define CONFIG_SF_DEFAULT_MODE		SPI_MODE_3
#endif
#ifndef CONFIG_SF_DEFAULT_CS
# define CONFIG_SF_DEFAULT_CS		0
#endif
#ifndef CONFIG_SF_DEFAULT_BUS
# define CONFIG_SF_DEFAULT_BUS		0
#endif

static struct spi_nand_chip *chip;
/*
 * This function computes the length argument for the erase command.
 * The length on which the command is to operate can be given in two forms:
 * 1. <cmd> offset len  - operate on <'offset',  'len')
 * 2. <cmd> offset +len - operate on <'offset',  'round_up(len)')
 * If the second form is used and the length doesn't fall on the
 * sector boundary, than it will be adjusted to the next sector boundary.
 * If it isn't in the flash, the function will fail (return -1).
 * Input:
 *    arg: length specification (i.e. both command arguments)
 * Output:
 *    len: computed length for operation
 * Return:
 *    1: success
 *   -1: failure (bad format, bad address).
 */
static int sf_parse_len_arg(char *arg, ulong *len)
{
	char *ep;
	char round_up_len; /* indicates if the "+length" form used */
	ulong len_arg;

	round_up_len = 0;
	if (*arg == '+') {
		round_up_len = 1;
		++arg;
	}

	len_arg = simple_strtoul(arg, &ep, 16);
	if (ep == arg || *ep != '\0')
		return -1;

	if (round_up_len && chip->block_size > 0)
		*len = ROUND(len_arg, chip->block_size);
	else
		*len = len_arg;

	return 1;
}

#ifdef CONFIG_DM_SPI_FLASH
struct spi_nand_chip *spi_nand_flash_probe(unsigned int busnum, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct udevice *new, *bus_dev;
	int ret;

	/* Remove the old device, otherwise probe will just be a nop */
	ret = spi_find_bus_and_cs(busnum, cs, &bus_dev, &new);
	if (!ret) {
		device_remove(new);
		device_unbind(new);
	}
	ret = spi_nand_probe_bus_cs(busnum, cs, max_hz, spi_mode, &new);
	if (ret) {
		printf("Failed to initialize SPI flash at %u:%u (error %d)\n",
		       busnum, cs, ret);
		return NULL;
	}

	return dev_get_uclass_priv(new);
}

void spi_nand_flash_free(struct spi_nand_chip *chip)
{
	spi_free_slave(chip->spi);
	// spi_nand_release(chip);
}
#else
struct spi_nand_chip *spi_nand_flash_probe(unsigned int busnum, unsigned int cs,
		unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *bus;
	struct spi_nand_chip *chip = NULL;
	int ret;

	bus = spi_setup_slave(busnum, cs, max_hz, spi_mode);
	ret = spi_nand_probe_slave(bus, &chip);
	if (ret) {
		printf("spi_nand_init failed\n");
		return NULL;
	}
	return chip;
}

void spi_nand_flash_free(struct spi_nand_chip *chip)
{
	spi_free_slave(chip->spi);
	spi_nand_release(chip);
}
#endif

static int do_spi_nand_flash_probe(int argc, char * const argv[])
{
	unsigned int bus = CONFIG_SF_DEFAULT_BUS;
	unsigned int cs = CONFIG_SF_DEFAULT_CS;
	unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
	unsigned int mode = CONFIG_SF_DEFAULT_MODE;
	char *endp;
	struct spi_nand_chip *new;

	if (argc >= 2) {
		cs = simple_strtoul(argv[1], &endp, 0);
		if (*argv[1] == 0 || (*endp != 0 && *endp != ':'))
			return -1;
		if (*endp == ':') {
			if (endp[1] == 0)
				return -1;

			bus = cs;
			cs = simple_strtoul(endp + 1, &endp, 0);
			if (*endp != 0)
				return -1;
		}
	}

	if (argc >= 3) {
		speed = simple_strtoul(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			return -1;
	}
	if (argc >= 4) {
		mode = simple_strtoul(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			return -1;
	}

	new = spi_nand_flash_probe(bus, cs, speed, mode);
	if (!new) {
		printf("Failed to initialize SPI NAND flash at %u:%u\n", bus, cs);
		return 1;
	}

	if (chip)
		spi_nand_flash_free(chip);
	chip = new;

	return 0;
}

/**
 * Update an area of SPI flash by erasing and writing any blocks which need
 * to change. Existing blocks with the correct data are left unchanged.
 *
 * @param flash		flash context pointer
 * @param offset	flash offset to write
 * @param len		number of bytes to write
 * @param buf		buffer to write from
 * @return 0 if ok, 1 on error
 */
static int spi_nand_flash_update(struct spi_nand_chip *chip, u32 offset,
		size_t len, const char *buf)
{
	int ret = 0;

	ret = spi_nand_cmd_erase_ops(chip, offset, len, true);
	if (ret) {
		printf("SPI-NAND: %zu bytes @ %#x Erased: ERROR\n", (size_t)len, (u32)offset);
		return ret;
	}
	ret = spi_nand_cmd_write_ops(chip, offset, len, buf);
	if (ret) {
		printf("SPI-NAND: %zu bytes @ %#x Written: ERROR\n", (size_t)len, (u32)offset);
	}
	return ret;
}

static int do_spi_nand_flash_read_write(int argc, char * const argv[])
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf;
	char *endp;
	int ret = 1;

	if (argc < 4)
		return -1;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	/* Consistency checking */
	if (offset + len > chip->size) {
		printf("ERROR: attempting %s past flash size (%#llx)\n",
		       argv[0], chip->size);
		return 1;
	}

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (strcmp(argv[0], "update") == 0) {
		ret = spi_nand_flash_update(chip, offset, len, buf);
		printf("SPI-NAND: %zu bytes @ %#x Updated: %s\n", (size_t)len, (u32)offset,
		       ret ? "ERROR" : "OK");
	} else if (strncmp(argv[0], "read", 4) == 0 ||
			strncmp(argv[0], "write", 5) == 0) {
		int read;

		read = strncmp(argv[0], "read", 4) == 0;
		if (read)
			ret = spi_nand_cmd_read_ops(chip, offset, len, buf);
		else
			ret = spi_nand_cmd_write_ops(chip, offset, len, buf);

		printf("SPI-NAND: %zu bytes @ %#x %s: %s(%d)\n", (size_t)len, (u32)offset,
		       read ? "Read" : "Written", ret ? "ERROR" : "OK", ret);
	}

	unmap_physmem(buf, len);

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_flash_erase(int argc, char * const argv[], int spread)
{
	unsigned long offset;
	unsigned long len;
	char *endp;
	int ret;

	if (argc < 3)
		return -1;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	ret = sf_parse_len_arg(argv[2], &len);
	if (ret != 1)
		return -1;

	/* Consistency checking */
	if (offset + len > chip->size) {
		printf("ERROR: attempting %s past flash size (%#llx)\n",
		       argv[0], chip->size);
		return 1;
	}

	ret = spi_nand_cmd_erase_ops(chip, offset, len, spread);
	printf("SPI-NAND: %zu bytes @ %#x Erased: %s\n", (size_t)len, (u32)offset,
	       ret ? "ERROR" : "OK");

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_read_write_reg(int argc, char *const argv[])
{
	u32 reg;
	u8 cfg;
	char *endp;
	int ret = 1;
	u8 data = 0;

	if (argc < 2)
		return -1;

	reg = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	if (strncmp(argv[0], "readreg", 7) == 0) {
		ret = spi_nand_read_reg(chip, reg, &data);
		printf("Read %#x %s: %#x\n", reg, ret ? "ERROR" : "OK", data);
	} else if (strncmp(argv[0], "writereg", 7) == 0) {
		if (argc < 3)
			return -1;
		cfg = simple_strtoul(argv[2], &endp, 16);
		if (*argv[2] == 0 || *endp != 0)
			return -1;
		ret = spi_nand_write_reg(chip, reg, &cfg);
		printf("Write %#x %s: %#x\n", reg, ret ? "ERROR" : "OK", cfg);
		ret = spi_nand_read_reg(chip, reg, &data);
		printf("Read %#x %s: %#x\n", reg, ret ? "ERROR" : "OK", data);
	}

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_cmd_page_read(int argc, char *const argv[])
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf;
	char *endp;
	int ret;

	if (argc < 4)
		return -1;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	/* Consistency checking */
	if (offset + len > chip->size) {
		printf("ERROR: attempting %s past flash size (%#llx)\n",
		       argv[0], chip->size);
		return 1;
	}

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	ret = spi_nand_cmd_read_pages(chip, offset, buf, len);
	printf("SPI-NAND: %zu bytes @ %#x Read: %s\n", (size_t)len, (u32)offset,
	       ret ? "ERROR" : "OK");

	unmap_physmem(buf, len);

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_write_oob(int argc, char *const argv[])
{
	unsigned long addr;
	unsigned long to;
	unsigned long len;
	void *buf;
	char *endp;
	int ret;

	if (argc < 4)
		return -1;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	to = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	/* Consistency checking */
	if (to + len > chip->size) {
		printf("ERROR: attempting %s past flash size (%#llx)\n",
		       argv[0], chip->size);
		return 1;
	}

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	ret = spi_nand_cmd_write_oob(chip, to, buf, len);
	printf("SPI-NAND: %zu bytes @ %#x Write: %s\n", (size_t)len, (u32)to,
	       ret ? "ERROR" : "OK");

	unmap_physmem(buf, len);

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_set_soft_ecc(int argc, char *const argv[])
{
	unsigned long enable;
	char *endp;
	int ret = 1;

	if (argc < 2)
		return -1;

	enable = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	if (enable)
		enable = true;

	ret = spi_nand_cmd_soft_ecc(enable);
	printf("SOFT-ECC %s %s\n", enable ? "Enable" : "Disable", ret ? "ERROR" : "OK");
	return ret;
}

static int do_spi_nand_flash_show_bad(void)
{
	loff_t offset;

	for (offset = 0; offset < chip->size; offset += chip->block_size) {
		if(spi_nand_block_isbad(chip, offset))
			printf("Bad block at 0x%#llx\n", offset);
	}

	return 0;
}

static int do_spi_nand_flash_mark_bad(int argc, char * const argv[])
{
	int ret;
	loff_t offset;
	char *endp;

	if (argc < 2)
		return -1;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	ret = spi_nand_block_markbad(chip, offset);
	if (!ret) {
		printf("SPI-NAND: 0x%#llx marked as bad block\n", offset);
	}

	return ret == 0 ? 0 : 1;
}

static int do_spi_nand_flash(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];
	--argc;
	++argv;

	if (strcmp(cmd, "probe") == 0) {
		ret = do_spi_nand_flash_probe(argc, argv);
		goto done;
	}

	/* The remaining commands require a selected device */
	if (!chip) {
		puts("No SPI flash selected. Please run `spi_nand probe'\n");
		return 1;
	}

	if (strcmp(cmd, "read") == 0 || strcmp(cmd, "write") == 0 ||
	    strcmp(cmd, "update") == 0)
		ret = do_spi_nand_flash_read_write(argc, argv);
	else if (strcmp(cmd, "erase") == 0)
		ret = do_spi_nand_flash_erase(argc, argv, false);
	else if (strcmp(cmd, "erase.spread") == 0)
		ret = do_spi_nand_flash_erase(argc, argv, true);
	else if (strcmp(cmd, "erase.force") == 0)
		ret = do_spi_nand_flash_erase(argc, argv, 2);
	else if (strcmp(cmd, "bad") == 0)
		ret = do_spi_nand_flash_show_bad();
	else if (strcmp(cmd, "markbad") == 0)
		ret = do_spi_nand_flash_mark_bad(argc, argv);
	else if (strcmp(cmd, "reset") == 0)
		ret = spi_nand_cmd_reset_ops(chip);
	else if (strcmp(cmd, "pageread") == 0)
		ret = do_spi_nand_cmd_page_read(argc, argv);
	else if (strcmp(cmd, "writeoob") == 0)
		ret = do_spi_nand_write_oob(argc, argv);
	else if (strcmp(cmd, "readreg") == 0 || strcmp(cmd, "writereg") == 0)
		ret = do_spi_nand_read_write_reg(argc, argv);
	else if (strcmp(cmd, "softecc") == 0)
		ret = do_spi_nand_set_soft_ecc(argc, argv);
	else
		ret = -1;

done:
	if (ret != -1)
		return ret;

usage:
	return CMD_RET_USAGE;
}


U_BOOT_CMD(
	spi_nand,	5,	1,	do_spi_nand_flash,
	"SPI NAND flash sub-system",
	"probe [[bus:]cs] [hz] [mode]	- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"spi_nand read addr offset len	- read `len' bytes starting at\n"
	"				  `offset' to memory at `addr', skipping bad blocks.\n"
	"spi_nand write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset', skipping bad blocks.\n"
	"spi_nand erase[.spread] offset [+]len		- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n"
	"				  With '.spread', erase enough for given file size, otherwise,\n"
	"				  'size' includes skipped bad blocks.\n"
	"				  With '.force', erase bad block marks.\n"
	"spi_nand update addr offset len	- erase and write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"spi_nand bad - show bad blocks\n"
	"spi_nand markbad offset - mark block include `offset' as bad block\n"
);
