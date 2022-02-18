/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <mach/sfax8.h>
#include <asm/io.h>

enum {
	DISABLE,
	ENABLE
}readable;

static int do_read_from_efuse(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int  datasize = 0;
	unsigned int start = 0;
	char data[193] = {0};
	char cmd[256] = {0};
	int i = 0;
	switch(argc){
	case 1:
		datasize=192;
		break;
	case 2:
		datasize=simple_strtoull(argv[1], NULL, 0);
		break;
	case 3:
		datasize=simple_strtoull(argv[2], NULL, 0);
		start=simple_strtoull(argv[1], NULL, 0);
		break;
	default:
		break;
	}

	if((argc==3 && start == 0) || datasize == 0){
		printf("Invalid arguments\n");
		return -1;
	}
	writel(ENABLE, (void *)EFUSE_READ_CTL);
	for(i = 0; i < datasize; i++){
		data[start + i]  = readb(EFUSE_DATA_BASE + start + i);

	}
	data[start + i] ='\0' ;
	writel(DISABLE, (void *)EFUSE_READ_CTL);
	sprintf(cmd, "setenv bootargs efusedata=%s\n", data + start);
	printf("cmd is %s\n", cmd);
	run_command(cmd, 0);

	return 0;
}

U_BOOT_CMD(
		r_efuse,	CONFIG_SYS_MAXARGS,	1,	do_read_from_efuse,
	"read data form efuse",
	"use like: r_efuse databytes or r_efuse datastart databytes"
);
