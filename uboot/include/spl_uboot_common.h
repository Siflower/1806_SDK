#ifndef __SPL_UBOOT_COMMON_H__
#define __SPL_UBOOT_COMMON_H__
#ifdef CONFIG_CMD_BOOT_TEST
#define BOOT_TEST_MAGIC_HIGH	0x5245574f /*OWER*/
#define BOOT_TEST_MAGIC_LOW	0x4c464953 /*SIFL*/
#define BOOT_TEST_HEADER_POS	0xbc00ff80

#define BOOT_TEST_SPL_LOAD_ADDRESS	0xbc004000
struct spl_uboot_common_header {
	unsigned int address;
	unsigned int magic[2];
	/*maxsize:0x80 = 0xbc010000-0xbc00ff80*/
};

/*
 * 16 bytes spl image header:
 * | -------------4-byte-image-size----------- |
 * | ---------------4-byte-magic-------------- |
 * | -----3-byte-flags------ | 1-byte-checksum |
 * | ----------4-byte-rom-patch-offset-------- |
 */
typedef struct spl_image_header {
	u32 ih_size;
	u32 ih_magic;
	struct {
		unsigned ih_checksum:8;
		unsigned support_patch:1; /* bit 8 */
		unsigned soft_ecc:2; /* use 2 bits as soft ecc flag*/
		unsigned reserved:21; /* bit 11~31 */
	} flags;
	u32 dev_offset;
} spl_image_header_t;

#endif /*CONFIG_CMD_BOOT_TEST*/
#endif /*__SPL_UBOOT_COMMON_H__*/
