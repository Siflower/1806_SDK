#include <stdio.h>
#include <stdlib.h>

/* Irom supports 48KB spl at most */
#define SPL_SIZE 0xc000 /* 48KB */

struct image_header {
	unsigned int ih_size;
	unsigned int ih_magic;
	struct {
		unsigned ih_checksum : 8;
		unsigned support_patch : 1; /* bit 8 */
		unsigned soft_ecc : 2;      /* use 2 bits as soft ecc flag*/
		unsigned reserved : 21;     /* bit 11~31 */
	} flags;
	unsigned int dev_offset;
};

struct __attribute__((__packed__)) irom_patch {
	unsigned int magic;
	unsigned short type;
	unsigned short is_last;
	unsigned int val;
	unsigned int addr;
	unsigned int gmac_rw;
	unsigned int delay;
	unsigned int reserv;
};

int main(int arg ,char *arv[])
{
	int j, enable = 0, patch[7];
	FILE *in ,*out0, *patchfile;
	unsigned char buf[1024 * 128 + 16] = {0};
	struct image_header header = {0};
	unsigned int size, dev_addr = SPL_SIZE;
	unsigned char checksum;
	struct irom_patch p;
	char *soft_ecc;

	if (!arv[1]) {
		printf("!!输入源文件名作为参数!!\n");
		return 0;
	}
	in = fopen(arv[1], "rb");
	if (in == NULL) {
		printf("Error:can't open file %s!\n", arv[1]);
		return 0;
	}

	if (arv[2]) {
		patchfile = fopen(arv[2], "rb");
		if (patchfile)
			size = fscanf(patchfile, "%d", &enable);
	}
	//printf("enable is %d\n", enable);

	out0 = fopen("irom_spl.img", "wb+");

	size = fread(buf, sizeof(unsigned char), 1024 * 128 + 16, in);
	printf("** bin size = %d **\n", size);
	for (j = 0; j < size; j++) {
		checksum += buf[j];
	}
	header.ih_size = size;
	header.ih_magic = 0x38316173;
	header.flags.ih_checksum = (unsigned int)checksum;
	printf("** checksum = 0x%x **\n", checksum);

	soft_ecc = getenv("SOFT_ECC");
	if (soft_ecc && soft_ecc[0] != '\0' && soft_ecc[0] != '0') {
		header.flags.soft_ecc = 3;
		printf("Using soft ecc!!!\n");
	} else {
		header.flags.soft_ecc = 0;
		printf("No soft ecc\n");
	}

	header.flags.support_patch = enable;
	if (enable) {
		fscanf(patchfile, "%x", &dev_addr);
		header.dev_offset = dev_addr;
		printf("irom patch enabled with addr 0x%x\n", header.dev_offset);
	} else {
		header.dev_offset = 0;
		printf("irom patch not enabled.\n");
	}

	fwrite(&header, sizeof(unsigned char), sizeof(header), out0);
	/* FIXME: check size & SPL_SIZE */
	fwrite(buf, sizeof(unsigned char), dev_addr - sizeof(header), out0);

	if (enable) {
		fgets(buf, sizeof(buf), patchfile);
		while (fscanf(patchfile, "%x %x %x %x %x %x %x", &patch[0], &patch[1], &patch[2], &patch[3],
					&patch[4], &patch[5], &patch[6]) > 0) {
			p.magic = 0xdeadbeef;
			p.type = patch[0];
			p.is_last = patch[1];
			p.val = patch[2];
			p.addr = patch[3];
			p.gmac_rw = patch[4];
			p.delay = patch[5];
			p.reserv = patch[6];
			//printf("buf is %x, %x, %x, %x\n", p.type, p.is_last, p.val, p.addr);
			fwrite(&p,sizeof(unsigned char),sizeof(p),out0);
			fgets(buf, sizeof(buf), patchfile);
		}
	}

	fclose(out0);
	fclose(in);
	if (arv[2])
		fclose(patchfile);
	return 0;
}
