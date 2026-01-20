#include <common.h>
#include <command.h>
#include <net.h>
#include <stdlib.h>
#include <mapmem.h>
#include <u-boot/md5.h>
#include <configs/sfa28.h>
#include <status_led.h>

#define MULTICAST_PORT       1758
#define MTFTP_BITMAPSIZE     0x1000
#define MAX_PACKET_DATA_SIZE 1470

unsigned char t_md5sum[16];
unsigned char r_md5sum[16];
unsigned int t_count = 0;
unsigned int r_count = 0;
unsigned int image_size = 0;
unsigned short idx = 0;
unsigned char *image_data = NULL;
unsigned int *tftp_mcast_bitmap = NULL;

static void receive_header(unsigned char *pkt)
{
	int i;

	memcpy(t_md5sum, pkt, sizeof(t_md5sum));
	memcpy(&image_size, pkt + 16, sizeof(image_size));

	r_count = 0;
	t_count = image_size / MAX_PACKET_DATA_SIZE;
	if (image_size % MAX_PACKET_DATA_SIZE != 0) {
		t_count += 1;
	}

	image_data = (unsigned char *)(CONFIG_SYS_SDRAM_BASE + CONFIG_SYS_BOOTM_LEN - 0x01000000);
	memset(image_data, 0, image_size);

	printf("image size: %d\npackets count: %d\nimage address: %pK\n",
		image_size, t_count, image_data);

	printf("md5sum: ");
	for (i = 0; i < 16; i++) {
		printf("%02x", t_md5sum[i]);
	}
	printf("\n");

	__led_set(STATUS_LED_BIT, STATUS_LED_OFF);
	__led_set(STATUS_LED_BIT1, STATUS_LED_ON);
	ext2_set_bit(0, tftp_mcast_bitmap);
}

static void receive_image(unsigned char *pkt, unsigned int len)
{
	memcpy(image_data + (idx - 1) * MAX_PACKET_DATA_SIZE, pkt, len);
	printf("#");

	r_count++;
	if (r_count % 64 == 0)
		printf("\n");

	if (r_count % 320 == 0)
		__led_toggle(STATUS_LED_BIT1);

	ext2_set_bit(idx, tftp_mcast_bitmap);
}

static void upgrade_image(void)
{
	char buf[96];
	void *ptr;
	unsigned long flash_kernel_addr = WEBFAILSAFE_UPLOAD_KERNEL_ADDRESS;
	unsigned long image_load_addr = CONFIG_SYS_LOAD_ADDR;

	__led_set(STATUS_LED_BIT1, STATUS_LED_ON);

	ptr = map_sysmem(image_load_addr, image_size);
	memcpy(ptr, image_data, image_size);

	printf("start upgrade image\n");

	sprintf(buf, "sf probe; sf update %#lx %#lx %#x;", \
		image_load_addr, flash_kernel_addr, image_size);
	printf("%s\n", buf);

	run_command(buf, 0);
	unmap_sysmem(ptr);

	__led_set(STATUS_LED_BIT1, STATUS_LED_OFF);
	__led_set(STATUS_LED_BIT, STATUS_LED_ON);
	printf("upgrade image done\n");
}

static void udp_handler(unsigned char *pkt, unsigned int dest, struct in_addr sip, unsigned int src, unsigned int len)
{
	if (dest != MULTICAST_PORT || len < 2)
		return;

	__be16 *s = (__be16 *)pkt;
	idx = ntohs(*s++);
	pkt = (unsigned char *)s;

	if (test_bit(idx, tftp_mcast_bitmap))
		return;

	if (idx == 0) {
		receive_header(pkt);
	} else if (test_bit(0, tftp_mcast_bitmap) && idx <= t_count) {
		receive_image(pkt, len - 2);

		if (r_count < t_count)
			return;

		memset(r_md5sum, 0, sizeof(r_md5sum));
		md5(image_data, image_size, r_md5sum);

		if (memcmp(t_md5sum, r_md5sum, 16) == 0) {
			printf("\nmd5 check success\n");
			upgrade_image();
		} else {
			printf("\nmd5 check failed\n");
			memset(tftp_mcast_bitmap, 0, MTFTP_BITMAPSIZE);
			memset(image_data, 0, image_size);
			r_count = 0;
		}
	}
}

void Mulupgrade(void)
{
	tftp_mcast_bitmap = (unsigned int *)malloc(MTFTP_BITMAPSIZE);
	if (!tftp_mcast_bitmap) {
		printf("No bitmap, no multicast. Sorry.\n");
		return;
	}

	memset(tftp_mcast_bitmap, 0, MTFTP_BITMAPSIZE);
	net_set_udp_handler(udp_handler);
}
