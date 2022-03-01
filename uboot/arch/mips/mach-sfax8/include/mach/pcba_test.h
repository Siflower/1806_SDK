#ifndef _SF_PCBATEST_H_
#define _SF_PCBATEST_H_

#include <linux/list.h>

//#define NM_DEBUG

#define SF_PCBA_TEST_MAGIC	0x38316173
#define TEST_TIMEOUT	3000 /* ms */
#define CASE_NAMELEN	8
#define DEV_ERASE_SZ	4096
/* Tx/Rx buffer is limited to 512 bytes */
#define SF_MAX_BUF_SIZE	512

#define SF_FACTORY_SIZE 4096
#ifdef CONFIG_SFA18_UBOOT_LITE
#define SF_FACTORY_OFFSET 0x31000
#else
#define SF_FACTORY_OFFSET 0x90000
#endif

struct connect_interface {
	struct list_head list;
	int (*connect)(void *);
	int (*recv)(unsigned char *buf, int buf_len); /* Return data lenth received */
	int (*send)(unsigned char *buf, int len);
	int type;
	int priority;
	void *data;
};

enum interface_type {
	SF_NPU,
	SF_GMAC,
	SF_USB,
};

enum cmd_t {
	CMD_SYNC,
	GET_CHIPID,
	GET_TESTLIST,
	DEV_ERASE,
	DEV_READ,
	DEV_WRITE,
	RUN_TEST,
	CMD_MULT,
	GET_CHIPINFO,
	TEST_FINISH,
	SF_PROBE,
	SF_READ,
	CHECK_MD5,
};

struct cmd_queue {
	struct list_head list;
	int val;
	void *data;
};

enum result_header {
	CMD_FAIL,
	CMD_SUCCESS,
	/* TODO: used when data lenth longer than packet size. */
	CMD_CONTINUE,
	CMD_TIMEOUT,
	ERROR_CMD,
	// value 50-150 is used for wifi temp test result
};

/*
 * packet format:
 * | ------------20 bytes-------------------- | ---- |
 * | --4-- | -4- | -4- | --4-- | -2- | --2--- | ---- |
 * | magic | num | crc | lenth | cmd | result | data |
 */
struct packet_header {
	unsigned int magic;
	unsigned int num;
	unsigned int crc;
	unsigned int lenth; /* total lenth */
	unsigned short cmd;
	unsigned short result;
};

/*
 * When cmd_mult is set in packet header, the
 * *data* is seperated into several cmds like this:
 * | -2- | --1-- | ---1--- | ---- |
 * | cmd | lenth | is_last | data |
 */
struct cmd_header {
	unsigned short cmd;
	unsigned char lenth; /* total lenth, less than 256 bytes */
	unsigned char is_last;
};

struct test_module {
	char name[CASE_NAMELEN];
	int (*func)(void *);
};

struct chipinfo {
	unsigned int ddr_size;
	unsigned int flash_size;
};

extern int sf_pcba_test(void);
extern int register_interface(struct connect_interface *);
extern int remove_interface(int type);
extern void *malloc_al(int size);
extern void free_al(void *paligned);
extern int sf_random(int);
extern int sf_atoi(int *a, int len, char *s);
extern int sf_get_chipid(unsigned char *buf, unsigned int *len);
extern int sf_get_chipinfo(unsigned char *buf, unsigned int *len);
extern int sf_get_testlist(unsigned char *buf, unsigned int *len);
extern int sf_dev_erase(void *buf);
extern int sf_dev_read(void *buf, void *send_buf, unsigned int *l);
extern int sf_dev_write(void *buf);
extern int sf_probe(void);
extern int sf_read_to_ddr(void *buf);
extern int sf_check_md5(void *buf);

/* cmds */
extern long long read_chipid(void);
extern int npu_test(void *);
extern int gmac_test(void *);
extern int wifi_test(void *);
extern int gpio_test(void *);
extern int button_test(void *);
extern int sd_test(void *);
extern int usb_test(void *);
extern int codec_test(void *);
extern int pmu_test(void *);
extern int lcd_test(void *);
extern int share_memory_test(void *);
extern int spi_test(void *);
extern int ddr_test(void *);

extern const struct test_module test_list[];
extern const int test_list_sz;
extern int sf_flash_size;

#endif /* _SF_PCBATEST_H_ */
