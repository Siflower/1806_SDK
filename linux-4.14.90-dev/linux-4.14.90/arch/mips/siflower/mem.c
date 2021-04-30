/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * PROM library functions for acquiring/using memory descriptors given to
 * us from the YAMON.
 *
 * Copyright (C) 1999,2000,2012  MIPS Technologies, Inc.
 * All rights reserved.
 * Authors: Carsten Langgaard <carstenl@mips.com>
 *          Steven J. Hill <sjhill@mips.com>
 */
#include <linux/init.h>
#include <linux/bootmem.h>
#include <linux/string.h>

#include <asm/prom.h>
#include <asm/fw/fw.h>

#define FW_MAX_MEMBLOCKS    32

enum fw_memtypes {
    fw_dontuse,
    fw_code,
    fw_free,
};

typedef struct {
    unsigned long base; /*  Within KSEG0 */
    unsigned int size;  /*  bytes */
    enum fw_memtypes type;  /*  fw_memtypes */
} fw_memblock_t;

static fw_memblock_t mdesc[FW_MAX_MEMBLOCKS];

static char __initdata DEFAULT_BOOT_ARG[COMMAND_LINE_SIZE] = "mem=256M";

/* determined physical memory size, not overridden by command line args	 */
unsigned long physical_memsize = 0L;

#if 0
//parser COMMAND_LINE_SIZE,replace mem= with memsize=
static void arcs_mem_str_replace(char *cmdline)
{
	char *ptr = NULL;
	int len,remain_start;
	char memsize[] = "memsize=";
	static char tmpline[COMMAND_LINE_SIZE] __initdata;

	ptr = strstr(cmdline, "mem=");
	if(ptr){
		//replace mem size
		strcpy(tmpline, cmdline);
		memset(cmdline,0,strlen(cmdline));
		len = ptr - cmdline;
		remain_start = (ptr + 4) - cmdline;
		if(len > 0) memcpy(cmdline,tmpline,len);
		memcpy(cmdline + len, memsize, strlen(memsize));
		len += strlen(memsize);
		memcpy(cmdline + len, tmpline + remain_start, strlen(tmpline) - remain_start);
	}
}
#endif

fw_memblock_t * __init fw_getmdesc(int eva)
{
	char *memsize_str, *ptr;
	unsigned long memsize = 0, ememsize = 0;
	static char cmdline[COMMAND_LINE_SIZE] __initdata;
	int tmp;

	/* otherwise look in the environment */

	memsize_str = fw_getenv("memsize");
	if (memsize_str) {
		tmp = kstrtoul(memsize_str, 0, &memsize);
		if (tmp)
			pr_warn("Failed to read the 'memsize' env variable.\n");
	}
	if (!memsize && !ememsize) {
		pr_warn("memsize not set in YAMON, set to default (256MB)\n");
		physical_memsize = 0x10000000;
	} else {
		if (memsize > (256 << 20)) { /* memsize should be capped to 256M */
			pr_warn("Unsupported memsize value (0x%lx) detected! "
				"Using 0x10000000 (256M) instead\n",
				memsize);
			memsize = 256 << 20;
		}
		/* If ememsize is set, then set physical_memsize to that */
		physical_memsize = ememsize ? : memsize;
	}

#ifdef CONFIG_CPU_BIG_ENDIAN
	/* SOC-it swaps, or perhaps doesn't swap, when DMA'ing the last
	   word of physical memory */
	physical_memsize -= PAGE_SIZE;
#endif

	//set boot arg to default value for fpga env build
	if(!arcs_cmdline[0])
		strlcpy(arcs_cmdline, DEFAULT_BOOT_ARG, COMMAND_LINE_SIZE);

	/* Check the command line for a memsize directive that overrides
	   the physical/default amount */
	strcpy(cmdline, arcs_cmdline);
	memsize = physical_memsize;
	ptr = strstr(cmdline, "mem=");
	if(ptr){
		memsize = memparse(ptr + 4, NULL);
	}

	/* Last 64K for HIGHMEM arithmetics */
	if (memsize > 0x7fff0000)
		memsize = 0x7fff0000;

	memset(mdesc, 0, sizeof(mdesc));
#ifdef CONFIG_CORE1_MEM_RES
	mdesc[0].type = fw_free;
	mdesc[0].base = PHYS_OFFSET;
	mdesc[0].size = CONFIG_FIRMWARE_LOAD_BASE - CPHYSADDR(mdesc[0].base);

	mdesc[1].type = fw_dontuse;
	mdesc[1].base = mdesc[0].base + mdesc[0].size;
#ifdef CONFIG_MEMORY_OPTIMIZE
	mdesc[1].size = CONFIG_FIRMWARE_SIZE * 1024;
#else
    // Just a workaround to make sure lmac can work even on platform with low memory
    mdesc[1].size = 2048 * 1024;
#endif

	mdesc[2].type = fw_free;
	mdesc[2].base = mdesc[1].base + mdesc[1].size;
	mdesc[2].size = memsize - CPHYSADDR(mdesc[2].base);
#else
	mdesc[0].type = fw_free;
	mdesc[0].base = PHYS_OFFSET;
	mdesc[0].size = memsize - CPHYSADDR(mdesc[0].base);
#endif

#if 0
    //4.1.4 don't need this
	//workaround here for memblock information will be override by user mem if mem= is parsered in setup.c
	//more details please see @usermem of arch/mips/kernel/setup.c
	arcs_mem_str_replace(arcs_cmdline);
#endif
	printk("arcs_cmdline=%s\n",arcs_cmdline);

	return &mdesc[0];
}

static int __init fw_memtype_classify(unsigned int type)
{
	switch (type) {
	case fw_free:
		return BOOT_MEM_RAM;
	case fw_code:
		return BOOT_MEM_ROM_DATA;
	default:
		return BOOT_MEM_RESERVED;
	}
}

void __init fw_meminit(void)
{
	fw_memblock_t *p = NULL;

    memset(mdesc, 0 , sizeof(mdesc));
	/* SF16A18 don't support eva. */
    p = fw_getmdesc(0);
	while (p && p->size) {
		long type;
		unsigned long base, size;

		type = fw_memtype_classify(p->type);
		base = p->base;
		size = p->size;

        //printk("add_memory_region---base=%x size=%x type=%ld\n",(unsigned int)base, (unsigned int)size, type);
		add_memory_region(base, size, type);
		p++;
	}
}

void __init prom_free_prom_memory(void)
{
	unsigned long addr;
	int i;

	for (i = 0; i < boot_mem_map.nr_map; i++) {
		if (boot_mem_map.map[i].type != BOOT_MEM_ROM_DATA)
			continue;

		addr = boot_mem_map.map[i].addr;
		free_init_pages("YAMON memory",
				addr, addr + boot_mem_map.map[i].size);
	}
}
