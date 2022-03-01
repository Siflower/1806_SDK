/*
 * SiFLower SFAX8 PCBA test.
 */
#include <common.h>
#include <mach/pcba_test.h>
#include <dm/uclass.h>
#include <spi_flash.h>
#include <asm/gpio.h>
#include <stdlib.h>
#include "net.h"

static LIST_HEAD(con_if_head);
static LIST_HEAD(cmd_queue_head);

struct connect_interface *gl_con_if;
struct cmd_queue *front, *rear;
/* Buffer cache aligned? */
unsigned char recv_buf[SF_MAX_BUF_SIZE] __aligned(32);
unsigned char ack_buf[SF_MAX_BUF_SIZE] __aligned(32);
unsigned int cmd_num = 0;

extern void sf_low_power(void);

static struct connect_interface *get_if_by_type(int type)
{
	struct connect_interface *cur;
	struct list_head *head;

	list_for_each(head, &con_if_head) {
		cur = list_entry(head, struct connect_interface, list);
		if (cur->type == type)
			return cur;
	}

	return NULL;
}

static struct connect_interface *get_maxprior_if(void)
{
	struct connect_interface *cur, *ret = NULL;
	struct list_head *head;
	int max = -1;

	list_for_each(head, &con_if_head) {
		cur = list_entry(head, struct connect_interface, list);
		if (cur->priority > max) {
			max = cur->priority;
			ret = cur;
		}
	}

	return ret;
}

/*
 * 1.Get priority from config.
 * 2.con_if->type should be unique.
 * 3.con_if will be freed soon.
 */
int register_interface(struct connect_interface *con_if)
{
	struct connect_interface *new;

	new = get_if_by_type(con_if->type);
	if (!new) {
		new = (struct connect_interface *)malloc(sizeof(*new));
		if (!new)
			return -ENOMEM;
		new->type = con_if->type;
		new->priority = con_if->priority;
		new->connect = con_if->connect;
		new->recv = con_if->recv;
		new->send = con_if->send;
		new->data = con_if->data;
		list_add(&new->list, &con_if_head);
		return 0;
	}
	printf("Remove is needed before reregister\n");

	return -EEXIST;
}

int remove_interface(int type)
{
	struct connect_interface *old;

	old = get_if_by_type(type);
	if (!old)
		return 0;

	list_del(&old->list);
	free(old);

	return 0;
}

static int cmd_queue_init(void)
{
	front = (struct cmd_queue *)malloc(sizeof(*front));
	if (!front)
		return -ENOMEM;
	list_add(&front->list, &cmd_queue_head);

	rear = front;

	return 0;
}

static int get_dev_info(void)
{
#ifdef CONFIG_SPI_FLASH
	struct udevice *udev;
	struct spi_flash *flash;

	if (uclass_get_device(UCLASS_SPI_FLASH, 0, &udev) != 0)
		return -ENODEV;
	flash = dev_get_uclass_priv(udev);
	if (!flash)
		return -ENODEV;
	printf("total flash size is %d\n", flash->size);
	sf_flash_size = flash->size;
#else
	sf_flash_size = 0;
#endif
	return 0;
}

static int cmd_enqueue(int val, void *data)
{
	struct cmd_queue *q = (struct cmd_queue *)malloc(sizeof(*q));

	if (!q) {
		printf("malloc failed at %s\n", __func__);
		return -ENOMEM;
	}
	q->val = val;
	q->data = data;
	list_add(&q->list, &rear->list);
	rear = q;

	return 0;
}

static int cmd_queue_is_empty(void)
{
	return front == rear;
}

static int cmd_dequeue(int *val, void **data)
{
	struct cmd_queue *q;

	if (cmd_queue_is_empty())
		return -EPERM;

	q = list_entry((&front->list)->next, struct cmd_queue, list);
	*val = q->val;
	*data = q->data;
	/* If the entry is the last one, we need to reset rear. */
	if (q == rear)
		rear = front;
	list_del(&q->list);
	free(q);

	return 0;
}

/*
 * Returns 1 on success.
 */
static int check_crc(int crc, unsigned char *buf, int len)
{
	int result = crc32(0, buf, len);
	return result == crc;
}

static int app_connect(void)
{
	gl_con_if = get_maxprior_if();
	if (!gl_con_if) {
		printf("No connect interface register!\n");
		return -ENODEV;
	}

	return gl_con_if->connect(gl_con_if->data);
}

/*
 * Use cmd queue! Implemented by list.
 */
static int recv_cmd(void **data)
{
	struct packet_header *hdr;
	int val, crc, len = 0;

	while (cmd_dequeue(&val, data) != 0) {
		len = gl_con_if->recv(recv_buf, SF_MAX_BUF_SIZE);
		hdr = (struct packet_header *)recv_buf;
		if ((hdr->magic != SF_PCBA_TEST_MAGIC) || (len < sizeof(*hdr)))
			continue;

		if (hdr->lenth != len) {
			printf("Warning: received %d vs header->len %d\n",
								len, hdr->lenth);
		}
		cmd_num = hdr->num;
		crc = hdr->crc;
		hdr->crc = 0;
		if (!check_crc(crc, recv_buf, len)) {
			printf("receive a pkt with wrong crc\n");
			continue;
		}
		if (len > SF_MAX_BUF_SIZE)
			hdr->cmd = -1; /* ERROR_CMD */
		if (hdr->cmd == CMD_MULT) {
			struct cmd_header *chdr;

			chdr = (struct cmd_header *)(recv_buf + sizeof(*hdr));
			while (!chdr->is_last) {
				cmd_enqueue(chdr->cmd,
					(unsigned char *)chdr + sizeof(*chdr));
				chdr = (struct cmd_header *)
					((unsigned char *)chdr + chdr->lenth);
			}
			/* The last one. */
			cmd_enqueue(chdr->cmd,
					(unsigned char *)chdr + sizeof(*chdr));

		} else {
			cmd_enqueue(hdr->cmd, recv_buf + sizeof(*hdr));
		}
	}

	return val;
}

#ifdef NM_DEBUG
static int func1(void *d)
{
	printf("this is test function 1!\n");
	return 0;
}

static int func2(void *d)
{
	printf("this is test function 2!\n");
	return 0;
}

static int func3(void *d)
{
	printf("this is test function 3!\n");
	return 0;
}

static int test_recv(unsigned char *b, int buf_len)
{
	struct packet_header *h;
	int len;

	h = (struct packet_header *)malloc(sizeof(*h));
	h->magic = SF_PCBA_TEST_MAGIC;
	h->lenth = sizeof(*h) + 8;
	//h->cmd = CMD_SYNC;
	//h->cmd = GET_CHIPID;
	//h->cmd = GET_TESTLIST;
	{
#if 0
		const char *p = "led";
		h->cmd = RUN_TEST;
		memcpy(b + sizeof(*h), p, strlen(p));
		b[sizeof(*h) + CASE_NAMELEN] = sf_random(2);
#endif
	}

	{
#if 0
		h->cmd = DEV_ERASE;
		int *erase = (int *)(b + sizeof(*h));
		erase[0] = 0x100000;
		erase[1] = 4 * 1024;
#endif
	}
	{
#if 0
		h->cmd = DEV_WRITE;
		int *test_write = (int *)(b + sizeof(*h));
		test_write[0] = 0x100000;
		test_write[1] = 4 * 1024;
		test_write[2] = 0x12345678;
#endif
	}
	{
#if 1
		struct cmd_header *ch;
		h->cmd = CMD_MULT;
		ch = (struct cmd_header *)(b + sizeof(*h));
		/* 1 */
		ch->cmd = CMD_SYNC;
		ch->lenth = sizeof(*ch);
		ch->is_last = 0;
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 2 */
		ch->cmd = GET_CHIPID;
		ch->lenth = sizeof(*ch);
		ch->is_last = 0;
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 3 */
		ch->cmd = GET_TESTLIST;
		ch->lenth = sizeof(*ch);
		ch->is_last = 0;
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 4 */
		ch->cmd = DEV_ERASE;
		ch->lenth = sizeof(*ch) + 8;
		ch->is_last = 0;
		{
			int *erase = (int *)((char*)ch + sizeof(*ch));
			erase[0] = sf_flash_size - 4096;
			erase[1] = 4 * 1024;
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 5 */
		ch->cmd = DEV_WRITE;
		ch->lenth = sizeof(*ch) + 12;
		ch->is_last = 0;
		{
			int *test_write = (int *)((char*)ch + sizeof(*ch));
			test_write[0] = sf_flash_size - 4096;
			test_write[1] = 4 * 1024;
			test_write[2] = SF_PCBA_TEST_MAGIC;
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 6 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "wifi";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 7 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "eth";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 8 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "sd card";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 9 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "usb";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 10 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "pmu";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 11 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + 4 + CASE_NAMELEN;
		ch->is_last = 0;
		{
			int *random_val = (int *)((char*)ch + sizeof(*ch) + CASE_NAMELEN);
			const char *p = "led";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
			random_val[0] = sf_random(2);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 12 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "button";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
			/*
			 * GPIO button test need another gpio to pull up
			 * the gpio to be tested.
			 */
			gpio_request(30, "sf_button2");
			gpio_direction_output(30, 1);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 13 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN + 8;
		ch->is_last = 0;
		{
			int *data;
			const char *p = "codec";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
			data = (int *)((char *)ch + sizeof(*ch) + CASE_NAMELEN);
			data[0] = sf_random(3);
			data[1] = sf_random(193);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 14 */
		ch->cmd = RUN_TEST;
		ch->lenth = sizeof(*ch) + CASE_NAMELEN;
		ch->is_last = 0;
		{
			const char *p = "lcd";
			memcpy((char*)ch + sizeof(*ch), p, CASE_NAMELEN);
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 15 */
		ch->cmd = DEV_READ;
		ch->lenth = sizeof(*ch) + 8;
		ch->is_last = 0;
		{
			int *erase = (int *)((char*)ch + sizeof(*ch));
			erase[0] = sf_flash_size - 4096;
			erase[1] = 4 * 1024;
		}
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
		/* 16 */
		ch->cmd = GET_CHIPINFO;
		ch->lenth = sizeof(*ch);
		ch->is_last = 1;
		ch = (struct cmd_header *)((char *)ch + ch->lenth - sizeof(*ch));
		ch++;
#endif
	}
	h->crc = 0;
	h->crc = crc32(0, b, h->lenth);
	memcpy(b, h, sizeof(*h));

	len = h->lenth;
	free(h);
	return len;
}

static int test_send(unsigned char *b, int len)
{
	struct packet_header *h = (struct packet_header *)b;
	char *p = (char *)b;
	printf("magic is 0x%x\n", h->magic);
	printf("crc is 0x%x\n", h->crc);
	printf("len is %d vs %d\n", h->lenth, len);
	printf("result: %s\n", h->result == CMD_SUCCESS?"success":"fail");

	len -= sizeof(*h);
	p += sizeof(*h);
	while (len > 0) {
		printf("data is %s\n", p);
		len -= CASE_NAMELEN;
		p += CASE_NAMELEN;
		//len -= strlen(p) == 0 ? 1000 : strlen(p);
		//p += strlen(p);
	}

	return 0;
}

static int sf_test(void)
{
	struct connect_interface test1 = { {}, func1, test_recv, test_send, 0, 1, NULL };
	struct connect_interface test2 = { {}, func2, test_recv, test_send, 1, 0, NULL };
	struct connect_interface test3 = { {}, func3, test_recv, test_send, 1, 2, NULL };

	register_interface(&test1);
	register_interface(&test2);
	register_interface(&test3);
	remove_interface(1);
	register_interface(&test3);

	return 0;
}
#endif

/*
 * 1.connect
 * 2.receive commands
 * 3.run commands
 * 4.report test result
 */
int sf_pcba_test(void)
{
	struct packet_header *hdr;
	int i, cmd, rst, j;
	unsigned char *send_buf;
	void *data;
	int *int_buf;
	int test_finished = 0;

#ifdef NM_DEBUG
	sf_test();
#endif

#ifdef CONFIG_SFA18_ETH
	sf_low_power();
#endif

	for (j = 0; j < 20; j++) {
		ack_buf[j] = 0xff;
	}
	int_buf = (int *)ack_buf;
	*int_buf = SF_PCBA_TEST_MAGIC;
	/* zero out server ether in case the server ip has changed */
	memset(net_server_ethaddr, 0, 6);
	send_buf = malloc_al(SF_MAX_BUF_SIZE);
	if (!send_buf)
		goto error_send_buf;
	hdr = (struct packet_header *)send_buf;

	if (cmd_queue_init())
		goto error_queue_init;

	if (get_dev_info())
		goto error_queue_init;

	if (app_connect())
		goto error_connect;

	//learn arp first
	gl_con_if->send(ack_buf + 4, 64);

	for (;;) {
		rst = CMD_FAIL;
		cmd = recv_cmd(&data);
		hdr->lenth = 0;
		switch (cmd) {
		case CMD_SYNC:
			printf("sync cmd!\n");
			rst = CMD_SUCCESS;
			break;
		case GET_CHIPID:
			printf("get chipid cmd!\n");
			rst = sf_get_chipid(
					send_buf + sizeof(*hdr), &hdr->lenth);
			break;
		case GET_CHIPINFO:
			printf("get chip info cmd!\n");
			rst = sf_get_chipinfo(
					send_buf + sizeof(*hdr), &hdr->lenth);
			break;
		case GET_TESTLIST:
			printf("get testlist cmd!\n");
			rst = sf_get_testlist(
					send_buf + sizeof(*hdr), &hdr->lenth);
			break;
		case DEV_ERASE:
			printf("erase cmd!\n");
			rst = sf_dev_erase(data);
			break;
		case DEV_READ:
			printf("dev read cmd!\n");
			rst = sf_dev_read(data, send_buf + sizeof(*hdr),
					&hdr->lenth);
			break;
		case DEV_WRITE:
			printf("dev write cmd!\n");
			rst = sf_dev_write(data);
			break;
		case RUN_TEST:
			for (i = 0; i < test_list_sz; i++) {
				if (strcmp(data, test_list[i].name) == 0) {
					rst = test_list[i].func((char *)data +
								CASE_NAMELEN);
					break;
				}
			}
			if (i == test_list_sz) {
				printf("RUN_TEST: test %s not supported\n",
						(char *)data);
				rst = ERROR_CMD;
			}
			break;
		case SF_PROBE:
			rst = sf_probe();
			break;
		case SF_READ:
			rst = sf_read_to_ddr(data);
			break;
		case CHECK_MD5:
			rst = sf_check_md5(data);
			break;
		case TEST_FINISH:
			printf("test finish cmd!\n");
			test_finished = 1;
			rst = CMD_SUCCESS;
			break;
		/* CMD_MULT is handled in recv_cmd. */
		case CMD_MULT:
		default:
			printf("cmd not supported now!\n");
			rst = ERROR_CMD;
			break;
		}
		hdr->lenth += sizeof(*hdr);
		hdr->magic = SF_PCBA_TEST_MAGIC;
		hdr->num = cmd_num;
		hdr->crc = 0;
		hdr->crc = crc32(0, send_buf, hdr->lenth);
		hdr->cmd = cmd;
		hdr->result = rst;
		if (gl_con_if->send(send_buf, hdr->lenth) < 0)
			printf("failed to report result.\n");
		if (test_finished)
			break;
	}

error_connect:
	free(front);
error_queue_init:
	free_al(send_buf);
error_send_buf:
	printf("pcba test end.\n");
	return 0;
}
