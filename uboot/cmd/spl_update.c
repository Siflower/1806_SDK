/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <mach/pad.h>

#define MMC_BLK_SIZE 512

static int do_update_spl(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int updatesize = 0;
	char buf[128];

	if(argc < 2){
		//128k
		updatesize = 0x20000;
	}else{
		if(!strcmp(argv[1],"all")){
			//4M
			updatesize = 0x400000;
		}else if(!strcmp(argv[1],"uboot")){
			//512k
			updatesize = 0x80000;
		}else{
			char *memsuffix = NULL;
			int memshift = 0;
			updatesize = (int)simple_strtol(argv[1], &memsuffix, 0);
			if(!memsuffix){
				printf("suffix error\n");
				return -1;
			}
			switch (*memsuffix) {
				case 'M':
				case 'm':
					memshift = 20; /*  megabytes */
					break;
				case 'K':
				case 'k':
					memshift = 10; /*  kilobytes */
					break;
				case 'B':
				case 'b':
					memshift = 0; /*  bytes*/
					break;
				default:{
					printf("unknown suffix\n");
					return -1;
				}
			}
			updatesize = ((int) updatesize << memshift);
		}
	}

	printf("update spl from sd\n");

	if(run_command("mmc dev 1", 0))
	{
		printf("mmc switch dev error!\n");
		goto err;
	}

	sprintf(buf,"mmc read 0x81000000 0x0 0x%lx",(unsigned long)updatesize/MMC_BLK_SIZE);
	if(run_command(buf, 0))
	{
		printf("mmc read error!\n");
		goto err;
	}

	run_command("sf probe 0 33000000",0);
	sprintf(buf,"sf erase 0x0 0x%lx",(unsigned long)updatesize);
	run_command(buf,0);
	sprintf(buf,"sf write 0x81000000 0x0 0x%lx",(unsigned long)updatesize);
	run_command(buf,0);
	printf("operation success\n");
	return 0;

err:
	printf("operation fail\n");

	return 0;
}

U_BOOT_CMD(
		spld,	CONFIG_SYS_MAXARGS,	1,	do_update_spl,
	"update spl from device",
	"use like: spld sd"
);
