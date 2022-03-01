/*
 *	Copyright 1994, 1995, 2000 Neil Russell.
 *	(See License)
 *	Copyright 2000, 2001 DENX Software Engineering, Wolfgang Denk, wd@denx.de
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/byteorder.h>
#include "httpd.h"
#include <serial.h>

#include "../httpd/uipopt.h"
#include "../httpd/uip.h"
#include "../httpd/uip_arp.h"

// extern flash_info_t flash_info[];

static int arptimer = 0;

struct in_addr net_httpd_ip;

// start http daemon
void HttpdStart(void){
	struct uip_eth_addr eaddr;
	unsigned short int ip[2];
	ulong tmp_ip_addr = ntohl(net_ip.s_addr);
	printf( "HTTP server is starting at IP: %ld.%ld.%ld.%ld\n", ( tmp_ip_addr & 0xff000000  ) >> 24, ( tmp_ip_addr & 0x00ff0000  ) >> 16, ( tmp_ip_addr & 0x0000ff00  ) >> 8, ( tmp_ip_addr & 0x000000ff));
	eaddr.addr[0] = net_ethaddr[0];
	eaddr.addr[1] = net_ethaddr[1];
	eaddr.addr[2] = net_ethaddr[2];
	eaddr.addr[3] = net_ethaddr[3];
	eaddr.addr[4] = net_ethaddr[4];
	eaddr.addr[5] = net_ethaddr[5];
	// set MAC address
	uip_setethaddr(eaddr);

	uip_init();
	httpd_init();

	ip[0] =  htons((tmp_ip_addr & 0xFFFF0000) >> 16);
	ip[1] = htons(tmp_ip_addr & 0x0000FFFF);

	uip_sethostaddr(ip);

	printf("done set host addr 0x%x 0x%x\n", uip_hostaddr[0], uip_hostaddr[1]);
	// set network mask (255.255.255.0 -> local network)
	ip[0] = htons((0xFFFFFF00 & 0xFFFF0000) >> 16);
	ip[1] = htons(0xFFFFFF00 & 0x0000FFFF);

	net_netmask.s_addr = 0x00FFFFFF;
	uip_setnetmask(ip);

	// should we also set default router ip address?
	//uip_setdraddr();
	do_http_progress(WEBFAILSAFE_PROGRESS_START);
	webfailsafe_is_running = 1;
}

void HttpdStop(void){
	webfailsafe_is_running = 0;
	webfailsafe_ready_for_upgrade = 0;
	webfailsafe_upgrade_type = WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE;
	do_http_progress(WEBFAILSAFE_PROGRESS_UPGRADE_FAILED);
}
void HttpdDone(void){
	webfailsafe_is_running = 0;
	webfailsafe_ready_for_upgrade = 0;
	webfailsafe_upgrade_type = WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE;
	do_http_progress(WEBFAILSAFE_PROGRESS_UPGRADE_READY);
}

#ifdef CONFIG_MD5
#include <u-boot/md5.h>

void printChecksumMd5(int address,unsigned int size)
{
	void *buf = (void*)(address);
	int i = 0;
	u8 output[16];
	md5_wd(buf, size, output, CHUNKSZ_MD5);
	printf("md5 for %08x ... %08x ==> ", address, address + size);
	for (i = 0; i < 16; i++)
		printf("%02x", output[i] & 0xFF);
	printf("\n");
}
#else
void printChecksumMd5(int address,unsigned int size)
{
}
#endif

#ifdef CONFIG_SPL_SPI_SUPPORT

#ifdef CONFIG_SPI_NAND_BOOT
#define ERASE_SECTOR_SIZE 0x20000 //128k
static void assemble_command(char * buf, const char *img_type, const ulong size, unsigned long from, unsigned long to)
{
	printf("\n\n****************************\n*     %s UPGRADING     *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n", img_type);
	sprintf(buf, "spi_nand probe 0 33000000;spi_nand update 0x%lx 0x%lx 0x%lx;", from, to, size);
}
#else  // CONFIG_SPI_NAND_BOOT

#ifndef CONFIG_SFA18_UBOOT_LITE
#define ERASE_SECTOR_SIZE 0x10000 //64k
#else
#define ERASE_SECTOR_SIZE 0x1000 //4k
#endif

// from: the address that the image is in the memmory
// to: the offset in the flash that the image is going to be written at
static void assemble_command(char * buf, const char *img_type, const ulong size, unsigned long from, unsigned long to)
{


	printf("\n\n****************************\n*     %s UPGRADING     *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n", img_type);
	sprintf(buf, "sf probe 0 33000000;sf erase 0x%lx 0x%lx 0x%lx;sf write 0x%lx 0x%lx 0x%lx;",
		to, size, (unsigned long)ERASE_SECTOR_SIZE,
		from, to, size);
}
#endif // CONFIG_SPI_NAND_BOOT

int do_http_upgrade(const ulong size, const int upgrade_type){
	char buf[96];
	ulong erasesize = 0;
	ulong erasesector = ERASE_SECTOR_SIZE;
	//printf checksum if defined
	printChecksumMd5(WEBFAILSAFE_UPLOAD_RAM_ADDRESS,size);
	erasesize = size;
	if(erasesize % erasesector){
		//aligned with erase sector size
		erasesize = erasesize + (erasesector - (erasesize % erasesector));
	}
	if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_UBOOT){
		assemble_command(buf, "U-BOOT", erasesize, WEBFAILSAFE_UPLOAD_RAM_ADDRESS, WEBFAILSAFE_UPLOAD_UBOOT_ADDRESS);

	} else if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE){
		assemble_command(buf, "FIRMWARE", erasesize, WEBFAILSAFE_UPLOAD_RAM_ADDRESS, WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS);
	} else if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_TEST){
#if 0
		char image_type[32];
		int i = 0;
		printf(">> Please entry the type of image\n");
		printf(">> [1]:linux        [2]:spl_uboot\n");
		printf(">> [3]:pcba         [4]:uboot\n");
		printf(">> [5]:spl          [6]:mpw\n");
		printf(">> [7]:mpw_header   [8]:elf\n");
		printf("entry: ");
		serial_setbrg();
		for (;;) {
			if (i >= 31)
				break;
			image_type[i] = getc();
			printf("%c", image_type[i]);
			if (image_type[i] == '\r')
				break;
			i++;

		}
		image_type[i] = '\0';

		printf("\n\n****************************\n*     JUMP TO TEST IMAGE     *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n");
		sprintf(buf, "boot_test 0x%x 0x%lx %s", WEBFAILSAFE_UPLOAD_RAM_ADDRESS, erasesize, image_type);
#endif
		printf("\n\n****************************\n*     JUMP TO TEST IMAGE     *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n");
		sprintf(buf, "boot_test 0x%x 0x%lx %s", WEBFAILSAFE_UPLOAD_RAM_ADDRESS, erasesize, uip_httpd_file_name);
	}
#ifndef CONFIG_SFA18_UBOOT_LITE
	else if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_ART){
		// TODO: add option to change ART partition offset,
		// for those who want to use OFW on router with replaced/bigger FLASH
		assemble_command(buf, "ART", erasesize, WEBFAILSAFE_UPLOAD_RAM_ADDRESS, WEBFAILSAFE_UPLOAD_ART_ADDRESS);
	}
#endif
	else {
		return(-1);
	}

	printf("Executing: %s\n\n", buf);
#if 0
	//compare written data
	{
		int ret = run_command(buf, 0);
		sprintf(buf,"sf read 0x82000000 0x%lx 0x%lx;",WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS,erasesize);
		ret = run_command(buf, 0);
		//compare data
		ret = memcmp((void*)(0x82000000),(void *)(0x81000000),erasesize);
		printf("memcmp 0x82000000 with 0x810000000 ret = %d\n",ret);
		return ret;
	}
#else
	return(run_command(buf, 0));
#endif
	return(-1);
}
#endif
#ifdef CONFIG_SPL_MMC_SUPPORT

#define MMC_BLK_SIZE 512

int do_http_upgrade(const ulong size, const int upgrade_type){
	char buf[96];
	ulong eraseblk = 0;
#if PRINT_CHECKSUM
	printChecksumMd5(WEBFAILSAFE_UPLOAD_RAM_ADDRESS,size);
#endif
	eraseblk = size/MMC_BLK_SIZE + ((size % MMC_BLK_SIZE) ? 1 : 0);
	if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_UBOOT){
		printf("\n\n****************************\n*     U-BOOT UPGRADING     *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n");
		sprintf(buf,
				"mmc dev 1;mmc erase 0x%lx 0x%lx;mmc write 0x%lx 0x%lx 0x%lx;",
				(unsigned long int)WEBFAILSAFE_UPLOAD_UBOOT_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk,
				(unsigned long int)WEBFAILSAFE_UPLOAD_RAM_ADDRESS,
				(unsigned long int)WEBFAILSAFE_UPLOAD_UBOOT_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk);

	} else if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_FIRMWARE){

		printf("\n\n****************************\n*    FIRMWARE UPGRADING    *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n");
		sprintf(buf,
				"mmc dev 1;mmc erase 0x%lx 0x%lx;mmc write 0x%lx 0x%lx 0x%lx;",
				(unsigned long int)WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk,
				(unsigned long int)WEBFAILSAFE_UPLOAD_RAM_ADDRESS,
				(unsigned long int)WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk);

	} else if(upgrade_type == WEBFAILSAFE_UPGRADE_TYPE_ART){

		// TODO: add option to change ART partition offset,
		// for those who want to use OFW on router with replaced/bigger FLASH
		printf("\n\n****************************\n*      ART  UPGRADING      *\n* DO NOT POWER OFF DEVICE! *\n****************************\n\n");
		sprintf(buf,
				"mmc dev 1;mmc erase 0x%lx 0x%lx;mmc write 0x%lx 0x%lx 0x%lx;",
				(unsigned long int)WEBFAILSAFE_UPLOAD_ART_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk,
				(unsigned long int)WEBFAILSAFE_UPLOAD_RAM_ADDRESS,
				(unsigned long int)WEBFAILSAFE_UPLOAD_ART_ADDRESS/MMC_BLK_SIZE,
				(unsigned long int)eraseblk);
	} else {
		return(-1);
	}

	printf("Executing: %s\n\n", buf);
	return(run_command(buf, 0));

	return(-1);
}
#endif
// info about current progress of failsafe mode
int do_http_progress(const int state){
	//unsigned char i = 0;

	/* toggle LED's here */
	switch(state){
		case WEBFAILSAFE_PROGRESS_START:

			// // blink LED fast 10 times
			// for(i = 0; i < 10; ++i){
			// 	all_led_on();
			// 	milisecdelay(25);
			// 	all_led_off();
			// 	milisecdelay(25);
			// }

			printf("HTTP server is ready!\n\n");
			break;

		case WEBFAILSAFE_PROGRESS_TIMEOUT:
			//printf("Waiting for request...\n");
			break;

		case WEBFAILSAFE_PROGRESS_UPLOAD_READY:
			printf("HTTP upload is done! Upgrading...\n");
			break;

		case WEBFAILSAFE_PROGRESS_UPGRADE_READY:
			printf("HTTP ugrade is done! Rebooting...\n\n");
			break;

		case WEBFAILSAFE_PROGRESS_UPGRADE_FAILED:
			printf("## Error: HTTP ugrade failed!\n\n");

			// // blink LED fast for 4 sec
			// for(i = 0; i < 80; ++i){
			// 	all_led_on();
			// 	milisecdelay(25);
			// 	all_led_off();
			// 	milisecdelay(25);
			// }

			// wait 1 sec
			// milisecdelay(1000);

			break;
	}

	return(0);
}

void NetSendHttpd(void){

	volatile uchar *tmpbuf = net_tx_packet;
	int i;

	for(i = 0; i < 40 + UIP_LLH_LEN; i++){

		tmpbuf[i] = uip_buf[i];
	}

	for(; i < uip_len; i++){

		tmpbuf[i] = uip_appdata[i - 40 - UIP_LLH_LEN];
	}

	eth_send(net_tx_packet, uip_len);
}

void NetReceiveHttpd(volatile uchar * inpkt, int len){

	if (!strncmp((const char *)inpkt + 12, "8100", 4)) {
		/* remove vlan tag if exist */
		memcpy(uip_buf, (const void *)inpkt, 12);
		memcpy(uip_buf + 12, (const void *)(inpkt + 16), len - 16);
		uip_len = len - 4;
	} else {
		memcpy(uip_buf, (const void *)inpkt, len);
		uip_len = len;
	}
	struct uip_eth_hdr * tmp = (struct uip_eth_hdr *)&uip_buf[0];

	if(tmp->type == htons(UIP_ETHTYPE_IP)){

		uip_arp_ipin();
		uip_input();

		if(uip_len > 0){

			uip_arp_out();
			NetSendHttpd();
		}
	} else if(tmp->type == htons(UIP_ETHTYPE_ARP)){

		uip_arp_arpin();

		if(uip_len > 0){

			NetSendHttpd();
		}
	}
}

void HttpdHandler(void){
	int i;

	for(i = 0; i < UIP_CONNS; i++){
		uip_periodic(i);

		if(uip_len > 0){
			uip_arp_out();
			NetSendHttpd();
		}
	}

	if(++arptimer == 20){
		uip_arp_timer();
		arptimer = 0;
	}
}
