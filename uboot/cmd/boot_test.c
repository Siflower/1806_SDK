#include <common.h>
#include <command.h>
#include <linux/string.h>
#include <errno.h>
#include <spl_uboot_common.h>
#include <image.h>

#ifdef CONFIG_CMD_BOOT_TEST

#define BOOT_TEST_CMD_SIZE	64
char boot_test_cmd[BOOT_TEST_CMD_SIZE];

enum boot_image_type {
	LINUX_OS,
	UBOOT_FIRMWARE,
	SPL_UBOOT_FIRMWARE,
	SPL_IMAGE,
	MPW_IMAGE,
	MPW_WITH_HEADER_IMAGE,
	ELF_IMAGE,
	NOT_SUPPORT_IMAGE_TYPE
};
struct boot_image_type_match {
	char *type_name;
	enum boot_image_type type;
};

struct boot_image_type_match sf_image_type_tables[] = {
	{"linux",	LINUX_OS},
	{"uboot",	UBOOT_FIRMWARE},
	{"pcba",	UBOOT_FIRMWARE},
	{"spl_uboot",	SPL_UBOOT_FIRMWARE},
	{"spl",		SPL_IMAGE},
	{"mpw_header",	MPW_WITH_HEADER_IMAGE},
	{"mpw",		MPW_IMAGE},
	{"elf",		ELF_IMAGE},

	/*
	 * the end is NULL, don't changer it
	 * */
	{},
};

/*
 * The function was boot image with header
 * @addr:	Location of the image in memory
 * */
static int do_boot_image_with_header(unsigned long addr)
{
	debug("###%s >> image addr: 0x%8lx\n", __func__, addr);
	memset(boot_test_cmd, 0, BOOT_TEST_CMD_SIZE);
	sprintf(boot_test_cmd, "bootm 0x%lx", addr);
	debug("###%s >> command:%s\n", __func__, boot_test_cmd);
	return (run_command(boot_test_cmd, 0));
}

/*
 * The function was boot image without header
 * @addr:	Location of the image in memory
 * */
static int do_boot_image_without_header(unsigned long addr)
{
	debug("###%s >> image addr: 0x%8lx\n", __func__, addr);
	memset(boot_test_cmd, 0, BOOT_TEST_CMD_SIZE);
	sprintf(boot_test_cmd, "go 0x%lx", addr);
	debug("###%s >> command:%s\n", __func__, boot_test_cmd);
	return (run_command(boot_test_cmd, 0));
}

/*
 * The function was boot elf image
 * @addr:	Location of the image in memory
 * */
static int do_boot_elf(unsigned long addr)
{
	debug("###%s >> image addr: 0x%8lx\n", __func__, addr);
	memset(boot_test_cmd, 0, BOOT_TEST_CMD_SIZE);
	sprintf(boot_test_cmd, "bootelf 0x%lx", addr);
	debug("###%s >> command:%s\n", __func__, boot_test_cmd);
	return (run_command(boot_test_cmd, 0));
}

int boot_spl_uboot_firmware(unsigned int addr)
{
	struct spl_image_header *spl_ih = NULL;

	struct spl_uboot_common_header *such = NULL;
	/*
	 * copy spl image to iram, and skip 16bytes
	 *
	 * */
	spl_ih = (struct spl_image_header*)addr;
	debug("### %s >> spl header info:\n", __func__);
	debug("ih_size:0x%x\n", spl_ih->ih_size);
	memcpy((void*)(BOOT_TEST_SPL_LOAD_ADDRESS - sizeof(spl_image_header_t)),
	       (void*)addr, spl_ih->ih_size);

	/*
	 * set boot test image flags
	 *
	 * */
	such = (struct spl_uboot_common_header*)(BOOT_TEST_HEADER_POS);
	such->address = addr +  CONFIG_CMD_BOOT_TEST_SPL_SIZE_KB * 1024;
	such->magic[0] = BOOT_TEST_MAGIC_LOW;
	such->magic[1] = BOOT_TEST_MAGIC_HIGH;

	return do_boot_image_without_header(BOOT_TEST_SPL_LOAD_ADDRESS);
}

/*
 * The function was get image type
 *
 * */

static int get_image_type(const char *image_type)
{
	struct boot_image_type_match *type;

	type = sf_image_type_tables;
	while (type && type->type_name) {
		debug("###%s >> tables[%s] match [%s]\n",
		      __func__, type->type_name, image_type);

		if (!strcmp(image_type, type->type_name)) {
			return type->type;
		}
		type++;
	}
	printf("###ERR:image type: %s is not support!\n", image_type);
	return NOT_SUPPORT_IMAGE_TYPE;
}

static int __do_boot_siflower_test_image(unsigned long addr,
					 unsigned long size,
					 int type)
{
	int ret;
	switch (type) {
	case LINUX_OS:
	case UBOOT_FIRMWARE:
		ret = do_boot_image_with_header(addr);
		break;
	case SPL_UBOOT_FIRMWARE:
	case SPL_IMAGE:
	case MPW_WITH_HEADER_IMAGE:
	case MPW_IMAGE:
		ret = boot_spl_uboot_firmware(addr);
		break;
	case ELF_IMAGE:
		ret = do_boot_elf(addr);
		break;
	default:
		printf("###ERR:%s Only a fool would walk here\n", __func__);
		ret = -EINVAL;
	}

	return ret;
}

static int do_boot_siflower_test_image(cmd_tbl_t *cmdtp,
				       int flag,
				       int argc,
				       char * const argv[])
{
	unsigned long addr, size;
	const char *image_type = NULL;
	int type;
	if (argc < 4)
		return CMD_RET_USAGE;

	addr = simple_strtoul(argv[1], NULL, 16);
	size = simple_strtoul(argv[2], NULL, 16);
	image_type = argv[3];

	debug("###%s >> image addr:0x%8lx size:0x%lx type is %s\n",
	      __func__, addr, size, image_type);

	type = get_image_type(image_type);
	if (NOT_SUPPORT_IMAGE_TYPE == type) {
		return CMD_RET_FAILURE;
	}

	if (__do_boot_siflower_test_image(addr, size, type) < 0) {
		printf("###ERR:%s >> boot siflower test image fail.\n", __func__);
		return CMD_RET_FAILURE;
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	boot_test,	4,	1,	do_boot_siflower_test_image,
	"boot siflower test image",
	"boot_test addr size image_type		- imgae_type:\n"
	"					  [linux | uboot | pcba |\n"
	"					  spl_uboot | spl | mpw |\n"
	"					  mpw_with_header | elf]\n"
);


#endif /*CONFIG_CMD_BOOT_TEST*/
