/*
 * =====================================================================================
 *
 *       Filename:  ate_server.c
 *
 *    Description:  1.make board as socket server to communcate with PC GUI.
 *					2.The APP of ATE test tool
 *
 *        Version:  1.0
 *        Created:  02/01/2016 02:38:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  star , star.jiang@siflower.com.cn
 *        Company:  Siflower
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
//#include <linux/in.h>
#include <fcntl.h>
#include <string.h>
#include <linux/wireless.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
//#define IFNAME "wlan0"
char ifname[16] = {"wlan0"};

char beacon[192] = {
	0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x16, 0x88, 0x08, 0x07, 0xf9,
	0x00, 0x16, 0x88, 0x08, 0x07, 0xf9, 0x20, 0x5e, 0x5f, 0x20, 0x31, 0x3b, 0x00, 0x00, 0x00, 0x00,
	0x64, 0x00, 0x01, 0x00, 0x00, 0x06, 0x6f, 0x70, 0x65, 0x6e, 0x35, 0x67, 0x01, 0x08, 0x8c, 0x12,
	0x98, 0x24, 0xb0, 0x48, 0x60, 0x6c, 0x03, 0x01, 0x9d, 0x07, 0x06, 0x4e, 0x6f, 0x20, 0x24, 0x0d,
	0x14, 0x05, 0x04, 0x00, 0x01, 0x00, 0x00, 0x2d, 0x1a, 0xec, 0x01, 0x17, 0xff, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3d, 0x16, 0x9d, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbf, 0x0c, 0xa0, 0x01, 0xc0,
	0x31, 0xfa, 0xff, 0x0c, 0x03, 0xfa, 0xff, 0x0c, 0x03, 0xc0, 0x05, 0x00, 0x00, 0x00, 0xf5, 0xff,
	0x7f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xdd, 0x18, 0x00, 0x50, 0xf2, 0x02,
	0x01, 0x01, 0x00, 0x00, 0x03, 0xa4, 0x00, 0x00, 0x27, 0xa4, 0x00, 0x00, 0x42, 0x43, 0x5e, 0x00,
	0x62, 0x32, 0x2f, 0x00, 0xdd, 0x07, 0x00, 0x0c, 0x43, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define IS_24G 1
#define ETH_P_SFCFG 0x1688
#define SFCFG_MAGIC_NO      0x18181688
#define USE_ONE_CALI_TABLE 0

/* calibration size */
#define CALI_TABLE_2G_SIZE 364
#define CALI_TABLE_5G_SIZE 1325


/*  command id */
#define SFCFG_CMD_ATE_START				 0x0000
#define SFCFG_CMD_ATE_STOP				 0x0001
#define SFCFG_CMD_ATE_TX_START			 0x0002
#define SFCFG_CMD_ATE_TX_STOP			 0x0003
#define SFCFG_CMD_ATE_RX_START			 0x0004
#define SFCFG_CMD_ATE_RX_STOP			 0x0005
#define SFCFG_CMD_ATE_TX_FRAME_START	 0x0006
#define SFCFG_CMD_ATE_RX_FRAME_START	 0x0007
#define SFCFG_CMD_ATE_MACBYPASS_TX_START 0x0008
#define SFCFG_CMD_ATE_MACBYPASS_TX_STOP  0x0009
#define SFCFG_CMD_ATE_TX_TEST_TONE_START 0x000a
#define SFCFG_CMD_ATE_TX_TEST_TONE_STOP  0x000b
#define SFCFG_CMD_ATE_MACBYPASS_RX_START 0x000c
#define SFCFG_CMD_ATE_MACBYPASS_RX_STOP  0x000d

#define SFCFG_CMD_ATE_GET_POWER 0X0110
#define SFCFG_CMD_ATE_SET_BANDWIDTH		0x0100
#define SFCFG_CMD_ATE_SET_CHANNEL		0x0101
#define SFCFG_CMD_ATE_SET_PHY_MODE		0x0102
#define SFCFG_CMD_ATE_SET_RATE			0x0103
#define SFCFG_CMD_ATE_SET_PREAMBLE		0x0104
#define SFCFG_CMD_ATE_SET_GI			0x0105
#define SFCFG_CMD_ATE_SET_TX_POWER		0x0106
#define SFCFG_CMD_ATE_GET_INFO			0x0107
#define SFCFG_CMD_ATE_SAVE_TO_FLASH		0x0108
#define SFCFG_CMD_ATE_SET_CENTER_FREQ1	0x0109
#define SFCFG_CMD_ATE_READ_FROM_FLASH 0X10b
#define SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH 0X10c


#define SFCFG_CMD_ATE_WHOLE_FRAME			0x1000
#define SFCFG_CMD_ATE_TX_COUNT				0x1001
#define SFCFG_CMD_ATE_PAYLOAD_LENGTH		0x1002
#define SFCFG_CMD_ATE_TX_FC					0x1003
#define SFCFG_CMD_ATE_TX_DUR				0x1004
#define SFCFG_CMD_ATE_TX_BSSID				0x1005
#define SFCFG_CMD_ATE_TX_DA					0x1006
#define SFCFG_CMD_ATE_TX_SA					0x1007
#define SFCFG_CMD_ATE_TX_SEQC				0x1008
#define SFCFG_CMD_ATE_PAYLOAD				0x1009
#define SFCFG_CMD_ATE_TX_FRAME_BW			0x100a
#define SFCFG_CMD_ATE_MACBYPASS_INTERVAL    0x100b
#define SFCFG_CMD_ATE_XO_VALUE  0X100C
#define SFCFG_CMD_ATE_AET_NUM   0X100D

#define SFCFG_CMD_ATE_READ_REG       0x10000
#define SFCFG_CMD_ATE_WRITE_REG      0x10001
#define SFCFG_CMD_ATE_BULK_READ_REG  0x10002
#define SFCFG_CMD_ATE_BULK_WRITE_REG 0x10003

#define SFCFG_PRIV_IOCTL_ATE	  (SIOCIWFIRSTPRIV + 0x08)

#define true  1
#define false 0

typedef unsigned char   bool;
typedef int    int32_t;
typedef unsigned char   uint8_t;
typedef unsigned int   uint32_t;
typedef unsigned short  uint16_t;

enum register_mode {
	MAC_REG,
	PHY_REG,
	RF_REG,
};

enum Cali_table_part {
	LB_ANT1,
	HB_ANT1,
	LB_ANT2,
	HB_ANT2,
	CALI_TABLE_PART_NUM,
};

bool g_tx_frame_flag = false;
bool g_rx_frame_flag = false;
bool g_stop_tx_frame_flag = false;
bool g_stop_rx_frame_flag = false;
bool g_clock_flag = false;
bool g_start_tx_frame_by_macbypass_flag = false;
bool g_stop_tx_frame_by_macbypass_flag = false;
bool g_start_rx_by_macbypass_flag = false;
bool g_stop_rx_by_macbypass_flag = false;
bool g_tx_test_tone_flag = false;
bool g_stop_tx_test_tone_flag = false;

/*tx test machine send  pkts number*/
float rx_pkt_all = 0;

unsigned int g_tx_frame_num  = 1;
unsigned int g_tx_chan_freq  = 2412;
unsigned int g_tx_band_width = 0;
unsigned int g_tx_mode       = 0;
unsigned int g_tx_rate_idx   = 0;
unsigned int g_tx_use_sgi    = 0;
unsigned int g_tx_power    = 0;

static int32_t ioctl_socket = -1;
#if 0
static  uint8_t *sa = "16:88:aa:bb:cc:ee";
static  uint8_t *da = "16:88:aa:bb:cc:ee";
static  uint8_t *bssid = "16:88:aa:bb:cc:ee";
#endif
struct tx_frame_param {
	int frame_num;
};

enum ieee80211_band {
	IEEE80211_BAND_2GHZ =0,
	IEEE80211_BAND_5GHZ,
	IEEE80211_BAND_60GHZ,

	/* keep last */
	IEEE80211_NUM_BANDS
};


enum nl80211_chan_width {
	NL80211_CHAN_WIDTH_20_NOHT,
	NL80211_CHAN_WIDTH_20,
	NL80211_CHAN_WIDTH_40,
	NL80211_CHAN_WIDTH_80,
	NL80211_CHAN_WIDTH_80P80,
	NL80211_CHAN_WIDTH_160,
};


enum format_mode {
	NON_HT_MODE = 0,
	NON_HT_DUP_MODE,
	HT_MM_MODE,
	HT_GF_MODE,
	VHT_MODE,
};



struct rwnx_ioctl_cfg {
    uint32_t magic_no;
    uint32_t command_type;
    uint32_t command_id;
    uint16_t length;
    uint16_t sequence;
    uint32_t status;
    uint8_t data[2048];
}__attribute__((packed));


struct  rwnx_ioctl_cfg *server_cfg = NULL;

struct rwnx_ate_channel_params{
	enum ieee80211_band chan_band;
	unsigned int chan_freq;
	enum nl80211_chan_width chan_width;
};


struct rwnx_ate_rate_params{
	enum format_mode mode;
	unsigned int rate_idx;
	bool use_short_preamble; //only for 2.4G CCK mode rate_idx 1~3
	bool use_short_gi;       //for HT VHT mode but not HT_GF_MODE
};

struct rwnx_ate_packet_params{
	unsigned int frame_num;
	unsigned int frame_len;
	unsigned int tx_power;
};

struct rwnx_ate_address_params{
	unsigned char bssid[6];
	unsigned char da[6];
	unsigned char sa[6];
	unsigned char addr4[6];

};

struct sfcfghdr {
	uint32_t magic_no;
	uint32_t comand_type;
	uint32_t comand_id;
	uint16_t length;
	uint16_t sequence;
	uint32_t status;
	char data[4096];
}__attribute__((packed));



//back to user space  /sync to user space parameter
struct rwnx_ioctl_ate_dump_info {
	int bandwidth;
	int frame_bandwidth;
	int band;
	int freq;
	int freq1;
	int rate;
	int mode;
	int gi;
	int pre;
	int power;
	int len;
	int rssi;
	int reg_status;
	uint32_t xof1;
	uint32_t xof2;
	uint32_t count;
	uint32_t tx_cont;
	uint32_t tx_successful;
	uint32_t tx_retry;
	uint32_t rec_rx_count;
	uint32_t fcs_err;
	uint32_t per;
	uint32_t phy_err;
	uint32_t fcs_ok;
	uint32_t fcs_ok_for_macaddr;
	uint32_t fcs_group;
	uint32_t reg_val;
	uint8_t  bssid[12];
	uint8_t  da[12];
	uint8_t  sa[12];
}__attribute__((packed));


struct rwnx_ioctl_ate_dump_info *user = NULL;
uint32_t per = 0;
char per_check[5] = "fail";
uint32_t tmp1 = 0;
uint32_t tmp2 = 0;
uint32_t tmp3 = 0;
uint32_t tmp4 = 0;
uint32_t tmp5 = 0;
uint32_t tmp_f = 0;
uint32_t tmp_p = 0;
uint32_t tmp_r = 0;
uint32_t tmp_m = 0;
uint32_t tmp_g = 0;
uint32_t num_fcsok=0;
/* create the socket fd to wext ioctl*/
static int32_t get_ioctl_socket(void)
{
	/*  Prepare socket */
	if (ioctl_socket == -1)
	{
		ioctl_socket = socket(AF_INET, SOCK_DGRAM, 0);
		fcntl(ioctl_socket, F_SETFD, fcntl(ioctl_socket, F_GETFD) | FD_CLOEXEC);
	}

	return ioctl_socket;
}

/* close the ioctl socket fd*/
void close_ioctl_socket(void)
{
	if (ioctl_socket > -1)
		close(ioctl_socket);
	ioctl_socket = -1;
}


/* do the ioctl*/
int32_t do_ioctl(int32_t cmd, struct iwreq *wrq)
{
	int32_t s = get_ioctl_socket();
	strncpy(wrq->ifr_name, ifname, IFNAMSIZ);
	if(s < 0)
		return s;
	return ioctl(s, cmd, wrq);
}


/*
 *Show ATE parameters
 *SFCFG_CMD_ATE_ATESHOW
 *desc:
 *   .header_type    = IW_HEADER_TYPE_ATESHOW,
 *   .flags      = IW_DESCR_FLAG_DUMP,
 * */


int32_t get_ate_info()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	struct rwnx_ioctl_ate_dump_info *usdrv;

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
	tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
	tmp.length      = 10;
	tmp.sequence    = 1;

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
		printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
		return -1;
	}
    usdrv = (struct rwnx_ioctl_ate_dump_info *)malloc(sizeof(struct rwnx_ioctl_ate_dump_info));
    if(!usdrv){
        printf("oom!\n");
        return -2;
    }

	memset(usdrv,0,sizeof(struct rwnx_ioctl_ate_dump_info));
	memcpy(usdrv, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));

	printf("bandwidth = %d\n",usdrv->bandwidth);
	printf("frame_bandwidth = %d\n",usdrv->frame_bandwidth);
	printf("band = %d\n",usdrv->band);
	printf("freq = %d\n",usdrv->freq);
	printf("freq = %d\n",usdrv->freq1);
	printf("rate = %d\n",usdrv->rate);
	printf("mode = %d\n",usdrv->mode);
	printf("GI = %d\n",usdrv->gi);
	printf("preamble = %d\n",usdrv->pre);
	printf("power = %d\n",usdrv->power);
	printf("length = %d\n",usdrv->len);
	printf("rssi = %d\n",usdrv->rssi);
	printf("reg_status = %d\n",usdrv->reg_status);
	printf("xof1 = %d\n",usdrv->xof1);
	printf("xof2 = %d\n",usdrv->xof2);
	printf("total count = %d\n",usdrv->count);
	printf("tx_cont = %d\n",usdrv->tx_cont);
	printf("tx_successful = %d\n",usdrv->tx_successful);
	printf("tx_retry = %d\n",usdrv->tx_retry);
	printf("rec_rx_count = %d\n",usdrv->rec_rx_count);
	printf("fcs_err = %d\n",usdrv->fcs_err);
	printf("per = %d\n",usdrv->per);
	printf("phy_err = %d\n",usdrv->phy_err);
	printf("reg_val = %d\n",usdrv->reg_val);
	printf("bssid = 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x\n",usdrv->bssid[0],usdrv->bssid[1],usdrv->bssid[2],usdrv->bssid[3],usdrv->bssid[4],usdrv->bssid[5]);
	printf("da = 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x\n",usdrv->da[0],usdrv->da[1],usdrv->da[2],usdrv->da[3],usdrv->da[4],usdrv->da[5]);
	printf("sa = 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x : 0x%02x\n",usdrv->sa[0],usdrv->sa[1],usdrv->sa[2],usdrv->sa[3],usdrv->sa[4],usdrv->sa[5]);
    free(usdrv);
	return 0;
}

int32_t read_iw_reg(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	struct rwnx_ioctl_ate_dump_info user_reg;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate read reg
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_READ_REG;
	tmp.comand_id   = SFCFG_CMD_ATE_READ_REG;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata,20);
	printf("tmp.data= %s---------\n",tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_READ_REG failed\n");
		return -1;
	}
	//get the results
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
	tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
	tmp.length      = 10;
	tmp.sequence    = 1;
	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
		printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
		return -1;
	}
	memset(&user_reg,0,sizeof(struct rwnx_ioctl_ate_dump_info));
	memcpy(&user_reg, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));
	printf("register value = 0x%x\n",user_reg.reg_val);
	return 0;
}

int32_t write_iw_reg(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	struct rwnx_ioctl_ate_dump_info user_reg;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate read reg
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_WRITE_REG;
	tmp.comand_id   = SFCFG_CMD_ATE_WRITE_REG;
	tmp.length      = 21;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata,tmp.length);
	printf("tmp.data= %s---------\n",tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_WRITE_REG failed\n");
		return -1;
	}
	//get the results
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
	tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
	tmp.length      = 10;
	tmp.sequence    = 1;
	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
		printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
		return -1;
	}
	memset(&user_reg,0,sizeof(struct rwnx_ioctl_ate_dump_info));
	memcpy(&user_reg, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));
	if(user_reg.reg_status == 0){
		printf("write register %x done!\n",user_reg.reg_val);
	}
	else{
		printf("write register fail!\n");
	}
	return 0;
}

int32_t read_iw_bulk_reg(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	struct rwnx_ioctl_ate_dump_info user_reg;
	int num = 0;
	int type = 0;
	int space = 0;
	int i = 0;
	char addrs[1000] = {0};
	char tmp_addr[9] = {0};
	if(tmpdata[0]=='0'){
		type = MAC_REG;
		space = 8;}
	else if(tmpdata[0]=='1'){
		type = PHY_REG;
		space = 8;}
	else{type = 2;
		space = 4;
	}
	num = atoi(tmpdata+2);
	memcpy(addrs,tmpdata+4,sizeof(addrs));
	while(i<num){
		memcpy(tmp_addr,tmpdata+4+i*(space+1),space);
		memset(&wrq, 0, sizeof(struct iwreq));
		memset(&tmp, 0, sizeof(struct sfcfghdr));

		// cmd ate read reg
		tmp.magic_no = SFCFG_MAGIC_NO;
		tmp.comand_type = SFCFG_CMD_ATE_READ_REG;
		tmp.comand_id   = SFCFG_CMD_ATE_READ_REG;
		tmp.length      = 8;
		tmp.sequence    = 1;

		memcpy(tmp.data,tmp_addr,tmp.length);

		wrq.u.data.pointer = (caddr_t)&tmp;
		wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
			printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_READ_BULK_REG failed\n");
			return -1;
		}
		//get the results
		memset(&wrq, 0, sizeof(struct iwreq));
		memset(&tmp, 0, sizeof(struct sfcfghdr));

		tmp.magic_no = SFCFG_MAGIC_NO;
		tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
		tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
		tmp.length      = 10;
		tmp.sequence    = 1;
		wrq.u.data.pointer = (caddr_t)&tmp;
		wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
			printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
			return -1;
		}
		memset(&user_reg,0,sizeof(struct rwnx_ioctl_ate_dump_info));
		memcpy(&user_reg, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));
		printf("register:0x%s value = 0x%x\n",tmp_addr,user_reg.reg_val);
		i++;
	}
	return 0;
}


int32_t write_iw_bulk_reg(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	struct rwnx_ioctl_ate_dump_info user_reg;
	int num = 0;
	int type = 0;
	int space = 0;
	int i = 0;
	char addrs[1000] = {0};
	char tmp_addr_val[18] = {0};
	char tmp_addr[9] = {0};
	if(tmpdata[0]=='0'){
		type = MAC_REG;
		space = 17;}
	else if(tmpdata[0]=='1'){
		type = PHY_REG;
		space = 17;}
	else{type = RF_REG;
		space = 9;
	}
	num = atoi(tmpdata+2);
	memcpy(addrs,tmpdata+4,sizeof(addrs));
	while(i<num){
		memcpy(tmp_addr_val,tmpdata+4+i*(space+1),space);
		memcpy(tmp_addr,tmp_addr_val,(space-1)/2);
		memset(&wrq, 0, sizeof(struct iwreq));
		memset(&tmp, 0, sizeof(struct sfcfghdr));

		// cmd ate write reg
		tmp.magic_no = SFCFG_MAGIC_NO;
		tmp.comand_type = SFCFG_CMD_ATE_WRITE_REG;
		tmp.comand_id   = SFCFG_CMD_ATE_WRITE_REG;
		tmp.length      = space;
		tmp.sequence    = 1;

		memcpy(tmp.data,tmp_addr_val,tmp.length);

		wrq.u.data.pointer = (caddr_t)&tmp;
		wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
			printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_WRITE_BULK_REG failed\n");
			return -1;
		}
		//get the results
		memset(&wrq, 0, sizeof(struct iwreq));
		memset(&tmp, 0, sizeof(struct sfcfghdr));

		tmp.magic_no = SFCFG_MAGIC_NO;
		tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
		tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
		tmp.length      = 10;
		tmp.sequence    = 1;
		wrq.u.data.pointer = (caddr_t)&tmp;
		wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
			printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
			return -1;
		}
		memset(&user_reg,0,sizeof(struct rwnx_ioctl_ate_dump_info));
		memcpy(&user_reg, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));
		if(user_reg.reg_status == 0){
			printf("write register:0x%s done!\n",tmp_addr);
		}
		else{
			printf("write register:0x%s fail!\n",tmp_addr);
		}
		i++;
	}
	return 0;
}

int32_t get_factory_power(char *channel,char *bw,char *mode,char *mcs,char *gain_mode,char *band){
	struct iwreq wrq;
	struct sfcfghdr tmp;
	int power = 0;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	//cmd get factory power;
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_GET_POWER;
	tmp.comand_id   = SFCFG_CMD_ATE_GET_POWER;
	tmp.length      = 20;
	tmp.sequence    = 1;

	sprintf(tmp.data,"%s %s %s %s %s %s",channel,bw,mode,mcs,gain_mode,band);
	printf("tmp.data= %s--------------\n",tmp.data);
	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
	  printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_GET_POWER failed\n");
		return -1;
	 }
	 memcpy(&power, wrq.u.data.pointer, 4);

	 return power;
}
int32_t set_phy_bandwidth(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate bandwidth
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_BANDWIDTH;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_BANDWIDTH;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata,10);
	printf("tmp.data= %s---------\n",tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_BANDWIDTH failed\n");
		return -1;
	}

	return 0;
}


int32_t set_phy_chan_freq(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_CHANNEL;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_CHANNEL;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data,tmpdata,10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_CHANNEL failed\n");
		return -1;
	}
	return 0;

}


int32_t set_phy_center_freq1(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_CENTER_FREQ1;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_CENTER_FREQ1;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data,tmpdata,10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_CENTER_FREQ1 failed\n");
		return -1;
	}
	return 0;

}

int32_t set_phy_mode(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_PHY_MODE;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_PHY_MODE;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_PHY_MODE failed\n");
		return -1;
	}

	return 0;

}


int32_t set_phy_rate_idx(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_RATE;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_RATE;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_RATE failed\n");
		return -1;
	}
	return 0;

}


int32_t set_phy_use_sgi(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_GI;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_GI;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_GI failed\n");
		return -1;
	}
	return 0;

}


int32_t set_phy_preamble(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_PREAMBLE;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_PREAMBLE;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_PREAMBLE failed\n");
		return -1;
	}
	return 0;

}


int32_t set_phy_power(char *tmpdata,char *channel,char *bw,char *mode,char *rate,char *band)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	int power;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SET_TX_POWER;
	tmp.comand_id   = SFCFG_CMD_ATE_SET_TX_POWER;
	tmp.length      = 20;
	tmp.sequence    = 1;
	if (!strcmp(tmpdata,"factory")){
		power = get_factory_power(channel,bw,mode,rate,"3","band");
		printf("power is %d band is %s\r\n",power,band);
		snprintf(tmp.data,4,"%d",power);
	}
	else{
		memcpy(tmp.data,tmpdata, 20);
	}

	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_TX_POWER failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_bandwidth(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate bandwidth
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_FRAME_BW;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_FRAME_BW;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata,10);
	printf("tmp.data= %s---------\n",tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_FRAME_BW failed\n");
		return -1;
	}
	return 0;
}


int32_t set_pkg_frame_length(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_PAYLOAD_LENGTH;
	tmp.comand_id   = SFCFG_CMD_ATE_PAYLOAD_LENGTH;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_PAYLOAD_LENGTH failed\n");
		return -1;
	}
	return 0;

}

int32_t set_pkg_frame_num(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate num
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_COUNT;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_COUNT;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data,tmpdata,10);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_TX_COUNT failed\n");
		return -1;
	}
	return 0;

}

int32_t set_pkg_fc(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_FC;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_FC;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_TX_FC failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_dur(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_DUR;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_DUR;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_TX_DUR failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_bssid(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_BSSID;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_BSSID;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_BSSID failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_da(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_DA;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_DA;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_DA failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_sa(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_SA;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_SA;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_SA failed\n");
		return -1;
	}
	return 0;

}


int32_t set_pkg_seqc(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_SEQC;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_SEQC;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 20);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_PHY_MODE failed\n");
		return -1;
	}
	return 0;
}


int32_t set_pkg_whole_frame(char beacon[], int length_array)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	int i = 0;
	//fill beacon mac header
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	//cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_WHOLE_FRAME;
	tmp.comand_id   = SFCFG_CMD_ATE_WHOLE_FRAME;
	tmp.length      = length_array;
	tmp.sequence    = 1;

	printf("%s: length_array = %d\n",__func__,length_array);
	memcpy(tmp.data, beacon, length_array);

	for(i = 0; i <= length_array; i++)
	{
		printf("%s: tmp.data[%d] = 0x%02x\n",__func__,i,tmp.data[i]);
	}

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_PHY_MODE failed\n");
		return -1;
	}
	return 0;
}


int32_t set_pkg_payload(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//int i = 0;
	//fill the frame payload
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	//cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_PAYLOAD;
	tmp.comand_id   = SFCFG_CMD_ATE_PAYLOAD;
	tmp.length      = sizeof(tmpdata);
	tmp.sequence    = 1;

	memcpy(tmp.data, tmpdata, tmp.length);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SET_PAYLOAD failed\n");
		return -1;
	}
	return 0;
}

int32_t send_tx_test_tone_start(char *data)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	int data_length;
	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_TEST_TONE_START;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_TEST_TONE_START;
	tmp.length      = 10;
	tmp.sequence    = 1;

	data_length=sizeof(data);
	strncpy(tmp.data, data, data_length);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_TEST_TONE_START failed\n");
		return -1;
	}
	return 0;
}

int32_t send_tx_test_tone_stop()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_TX_TEST_TONE_STOP;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_TEST_TONE_STOP;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_TEST_TONE_STOP failed\n");
		return -1;
	}
	return 0;
}

int32_t send_ate_start()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//struct tx_frame_param *tx_frame_param = NULL;
	// struct rwnx_ate_config_params *ate_config_params = NULL;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_START;
	tmp.comand_id   = SFCFG_CMD_ATE_START;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_START failed\n");
		return -1;
	}
	return 0;
}

int32_t send_ate_stop()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//struct tx_frame_param *tx_frame_param = NULL;
	// struct rwnx_ate_config_params *ate_config_params = NULL;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_STOP;
	tmp.comand_id   = SFCFG_CMD_ATE_STOP;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_STOP failed\n");
		return -1;
	}
	return 0;
}


int32_t send_tx_start()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//struct tx_frame_param *tx_frame_param = NULL;
	// struct rwnx_ate_config_params *ate_config_params = NULL;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_TX_START;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_START;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_START failed\n");
		return -1;
	}
	return 0;
}


int32_t send_tx_start_by_macbypass()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_MACBYPASS_TX_START;
	tmp.comand_id   = SFCFG_CMD_ATE_MACBYPASS_TX_START;
	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_MACBYPASS_TX_START failed\n");
		return -1;
	}
	return 0;
}


int32_t send_rx_start()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//struct rwnx_ioctl_ate_dump_info user;
	tmp1 = 0;
	tmp2 = 0;
	tmp3 = 0;
	per = 100;
	tmp_f = 0;
	tmp_p = 0;
	tmp_r = 0;

	printf("%s>>\n", __func__);

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	//first call SFCFG_CMD_ATE_START
	tmp.comand_type = SFCFG_CMD_ATE_RX_START;
	tmp.comand_id   = SFCFG_CMD_ATE_RX_START;
//	tmp.comand_type = SFCFG_CMD_ATE_MACBYPASS_RX_START;
//	tmp.comand_id   = SFCFG_CMD_ATE_MACBYPASS_RX_START;

	tmp.length      = 10;
	tmp.sequence    = 1;

	strncpy(tmp.data, "true", 10);
	wrq.u.data.pointer = (caddr_t)&tmp;

	//12 is the sizeof sfcfghdr except data field, 10 is the actual data
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_RX_START failed\n");
		return -1;
	}
#if 0
	//read results from driver
	printf("show PER, RSSI!\n");
	user = (struct rwnx_ioctl_ate_dump_info *)malloc(sizeof(struct rwnx_ioctl_ate_dump_info));
	g_clock_flag = 1;

	while(g_clock_flag){
		memset(&wrq, 0, sizeof(struct iwreq));
		memset(&tmp, 0, sizeof(struct sfcfghdr));

		tmp.magic_no = SFCFG_MAGIC_NO;
		tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
		tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
		tmp.length      = 10;
		tmp.sequence    = 1;

		wrq.u.data.pointer = (caddr_t)&tmp;
		wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
			printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
			return -1;
		}
		memset(user, 0, sizeof(struct rwnx_ioctl_ate_dump_info));
		memcpy(user, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));

		tmp_f = user->fcs_err - tmp1;
		tmp_p = user->phy_err - tmp2;
		tmp_r = user->rec_rx_count - tmp3;
		if(tmp_r)
			per = ((tmp_f+tmp_p)*100/tmp_r);
		else
			per = 100;
		tmp1 = user->fcs_err;
		tmp2 = user->phy_err;
		tmp3 = user->rec_rx_count;
		printf("receive data = %d,rssi = %d,fcs error = %d,phy error = %d,per(every 3 seconds) = %d, per(from the beginning) = %d\n",
				user->rec_rx_count, user->rssi, user->fcs_err, user->phy_err, per, user->per);
		sleep(1);
	}
	free(user);
#endif
	if(user)
		free(user);
	user = (struct rwnx_ioctl_ate_dump_info *)malloc(sizeof(struct rwnx_ioctl_ate_dump_info));
	if(!user)
	{
		printf("oom!\n");
		return -1;

	}
	return 0;
}

int32_t get_rx_info(uint8_t flag)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//read results from driver
	printf("show PER, RSSI!\n");
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_GET_INFO;
	tmp.comand_id   = SFCFG_CMD_ATE_GET_INFO;
	tmp.length      = 10;
	tmp.sequence    = 1;

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
		printf("SFCFG_CMD_ATE_GET_INFO ioctl failed\n");
		return -1;
	}

	memset(user, 0, sizeof(struct rwnx_ioctl_ate_dump_info));
	memcpy(user, wrq.u.data.pointer, sizeof(struct rwnx_ioctl_ate_dump_info));

	tmp_f = user->fcs_err - tmp1;
	tmp_p = user->phy_err - tmp2;
	tmp_r = user->rec_rx_count - tmp3;
	tmp_m = user->fcs_ok_for_macaddr - tmp4;
	tmp_g = user->fcs_group - tmp5;
	num_fcsok = user->fcs_ok;

	tmp1 = user->fcs_err;
	tmp2 = user->phy_err;
	tmp3 = user->rec_rx_count;
	tmp4 = user->fcs_ok_for_macaddr;
	tmp5 = user->fcs_group;

	printf("receive data = %d,fcs ok  = %d,fcs error = %d,phy error = %d,fcs_ok_for_macaddr = %d,fcs_group = %d\n",user->rec_rx_count, user->fcs_ok, user->fcs_err, user->phy_err, user->fcs_ok_for_macaddr, user->fcs_group);

	return 0;
}


int32_t send_tx_stop()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no    = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_TX_STOP;
	tmp.comand_id   = SFCFG_CMD_ATE_TX_STOP;
	tmp.length      = 10;
	tmp.sequence    = 1;
	strncpy(tmp.data, "false", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = tmp.length + 16;//12 is the sizeof sfcfghdr except data field, 10 is the actual data

	//call SFCFG_CMD_ATE_TX_STOP
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_TX_STOP failed\n");
		return -1;
	}
	return 0;
}

int32_t send_tx_stop_by_macbypass()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no    = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_MACBYPASS_TX_STOP;
	tmp.comand_id   = SFCFG_CMD_ATE_MACBYPASS_TX_STOP;
	tmp.length      = 10;
	tmp.sequence    = 1;
	strncpy(tmp.data, "false", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = tmp.length + 16;//12 is the sizeof sfcfghdr except data field, 10 is the actual data

	//call SFCFG_CMD_ATE_TX_STOP
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_MACBYPASS_TX_STOP failed\n");
		return -1;
	}
	return 0;
}

int32_t send_rx_stop()
{
	struct iwreq wrq;
	struct sfcfghdr tmp;

	g_clock_flag = false;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	tmp.magic_no    = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_RX_STOP;
	tmp.comand_id   = SFCFG_CMD_ATE_RX_STOP;
//	tmp.comand_type = SFCFG_CMD_ATE_MACBYPASS_RX_STOP;
//	tmp.comand_id   = SFCFG_CMD_ATE_MACBYPASS_RX_STOP;
	tmp.length      = 10;
	tmp.sequence    = 1;
	strncpy(tmp.data, "false", 10);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = tmp.length + 16;//12 is the sizeof sfcfghdr except data field, 10 is the actual data

	//call SFCFG_CMD_ATE_RX_STOP
	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_RX_FRAME_START failed\n");
		return -1;
	}
	if(user)
		free(user);
	user = NULL;
	return 0;
}

int32_t set_xo_value(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_XO_VALUE;
	tmp.comand_id   = SFCFG_CMD_ATE_XO_VALUE;
	tmp.length      = 10;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, 10);
	printf("XO_VALUE = %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_XO_VALUE failed\n");
		return -1;
	}

	return 0;
}

int32_t save_XO_config_to_flash(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	unsigned int num = 0;
	char number[10] = "0";
	num = atoi(tmpdata);
    if (num > 15)
    {
	    sprintf(number,"0x%x%x",num,num);
    	tmpdata = number;
    }
    else
    {
	    sprintf(number,"0x0%x0%x",num,num);
    	tmpdata = number;
    }

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SAVE_TO_FLASH;
	tmp.comand_id   = SFCFG_CMD_ATE_SAVE_TO_FLASH;
	tmp.length      = 20;
	tmp.sequence    = 1;

	memcpy(tmp.data,tmpdata, tmp.length);
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_TO_FLASH failed\n");
		return -1;
	}
	return 0;
}

int32_t save_power_table_to_factory(uint32_t length,uint32_t offset,char *buffer){
	struct iwreq  wrq;
	struct sfcfghdr tmp;
	uint32_t offset_tmp = offset;
	int i = 0;

	memset(&wrq,0,sizeof(struct iwreq));
	memset(&tmp,0,sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
	tmp.comand_id = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
#if USE_ONE_CALI_TABLE
	tmp.length      = 2044;
#else
	tmp.length		= 4092;
#endif
	tmp.sequence = 1;

	sprintf(tmp.data,"%4d",offset_tmp + 2048 + 4);
	memcpy(&tmp.data[4],buffer,length);

	printf("cali_table[0] :%u \n",(unsigned char)buffer[0]);
	printf("cali_table[1689] :%u \n",(unsigned char)buffer[1688]);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	 if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
	        printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_TO_FLASH failed\n");
		return -1;
	 }
	return 0;
}

int32_t save_data_to_flash(uint32_t offset,char *buffer){
	struct iwreq  wrq;
	struct sfcfghdr tmp;
	uint32_t num = 0;
//	uint32_t sizeof_buffer = 0;
	//int i = 0;
	uint32_t data_length = 0;
	memset(&wrq,0,sizeof(struct iwreq));
	memset(&tmp,0,sizeof(struct sfcfghdr));

	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
	tmp.comand_id = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
	tmp.length      = 2048;
	tmp.sequence = 1;

	num = sizeof(offset);
	if (offset <2048 || offset >4096){
	     printf("the data is write out of range");
	     return -1;
	}
	data_length = strlen(buffer);
	memcpy(&tmp.data[0],&offset,sizeof(offset));
	memcpy(&tmp.data[num],buffer,data_length);
	tmp.length = data_length + sizeof(offset);
	printf("data length is %d\n--------",data_length);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	 if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
	        printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_TO_FLASH failed\n");
		return -1;
	 }
	return 0;
}

int32_t read_gain_table_from_mtd(char *buffer){
	struct iwreq wrq;
	struct sfcfghdr tmp;
	//uint32_t offset = 0;
	//int i ;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	//read gian table from mtd
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_READ_FROM_FLASH;
	tmp.comand_id   = SFCFG_CMD_ATE_READ_FROM_FLASH;
	tmp.length      = 20;
	tmp.sequence    = 1;


	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE,&wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_READ_FROM_FLASH failed\n");
		return -1;
	}
    memcpy(buffer,&tmp.data[0],2048);
	printf ("copy end!!!!\n");
	return 0;
}



int32_t save_power_config_to_flash(uint16_t tmpdata, uint16_t channel, uint16_t bw, uint16_t mode,uint16_t rate){
	struct iwreq wrq;
	struct sfcfghdr tmp;
	uint32_t offset = 0;

	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));

	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SAVE_TO_FLASH;
	tmp.comand_id   = SFCFG_CMD_ATE_SAVE_TO_FLASH;
	tmp.length      = 20;
	tmp.sequence    = 1;


	if (channel < 2485){
		switch(channel){
			case 2412:offset = (((channel - 2412) / 5)*28);break;
			case 2417:offset = (((channel - 2412) / 5)*28);break;
			case 2422:offset = (((channel - 2412) / 5)*28);break;
			case 2427:offset = (((channel - 2412) / 5)*28);break;
			case 2432:offset = (((channel - 2412) / 5)*28);break;
			case 2437:offset = (((channel - 2412) / 5)*28);break;
			case 2442:offset = (((channel - 2412) / 5)*28);break;
			case 2447:offset = (((channel - 2412) / 5)*28);break;
			case 2452:offset = (((channel - 2412) / 5)*28);break;
			case 2457:offset = (((channel - 2412) / 5)*28);break;
			case 2462:offset = (((channel - 2412) / 5)*28);break;
			case 2467:offset = (((channel - 2412) / 5)*28);break;
			case 2472:offset = (((channel - 2412) / 5)*28);break;
			default:return -1;
		}
		if (mode == 0){
			offset += rate;
		}
		else if (mode == 2){
			switch(bw)
			{
				case 1: offset += 12 + rate;break;
				case 2: offset += 20 + rate;break;
				default: return -1;
			}
		}
		if (mode != 0&& mode != 2)
		      return -1;
	}
	else{
		switch(channel){
			case 5180:offset = 364 + 53*0;break;
			case 5200:offset = 364 + 53*1;break;
			case 5220:offset = 364 + 53*2;break;
			case 5240:offset = 364 + 53*3;break;
			case 5260:offset = 364 + 53*4;break;
			case 5280:offset = 364 + 53*5;break;
			case 5300:offset = 364 + 53*6;break;
			case 5320:offset = 364 + 53*7;break;
			case 5500:offset = 364 + 53*8;break;
			case 5520:offset = 364 + 53*9;break;
			case 5540:offset = 364 + 53*10;break;
			case 5560:offset = 364 + 53*11;break;
			case 5580:offset = 364 + 53*12;break;
			case 5600:offset = 364 + 53*13;break;
			case 5620:offset = 364 + 53*14;break;
			case 5640:offset = 364 + 53*15;break;
			case 5660:offset = 364 + 53*16;break;
			case 5680:offset = 364 + 53*17;break;
			case 5700:offset = 364 + 53*18;break;
			case 5720:offset = 364 + 53*19;break;
			case 5745:offset = 364 + 53*20;break;
			case 5765:offset = 364 + 53*21;break;
			case 5785:offset = 364 + 53*22;break;
			case 5805:offset = 364 + 53*23;break;
			default: return -1;
			}
		if (mode == 0)
		      offset += rate;
		if (mode == 2){
			switch(bw){
				case 1:offset += 8  + rate;break;
				case 2:offset += 16 + rate;break;
				default: return -1;
			}
		}
		if (mode ==4){
			switch(bw){
				case 1:offset += 24 + rate;break;
				case 2:offset += 33 + rate;break;
				case 3:offset += 43 + rate;break;
				default: return -1;
			}
		}
		if (mode != 0 && mode != 2 && mode != 4 )
		      return -1;
	}
	printf("offset:%d\n",offset);
	sprintf(tmp.data,"1%02hu0%4d",tmpdata,offset + 2048 + 4);//factory form 2048 begin and 4 bytes form 2048 to 2052 is save xo;
	printf("tmp.data= %s---------\n", tmp.data);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + tmp.length;
		if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		 printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_TO_FLASH failed\n");
		  return -1;
		}
		return 0;
	}

int32_t save_version_to_factory(char *version)
{
    struct iwreq wrq;
    struct sfcfghdr tmp;
    memset(&wrq, 0, sizeof(struct iwreq));
    memset(&tmp, 0, sizeof(struct sfcfghdr));

    // cmd ate band
    tmp.magic_no = SFCFG_MAGIC_NO;
    tmp.comand_type = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
    tmp.comand_id   = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
    tmp.length      = 2;
    tmp.sequence    = 1;


    sprintf(tmp.data,"%4d",2048);
    memcpy(&tmp.data[4],version,2);
    wrq.u.data.pointer = (caddr_t)&tmp;
    wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + 10;

    printf("tmp.data------version----%s\n",tmp.data);

    if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
         printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_TO_FLASH failed\n");
          return -1;
        }

    return 0;
}

int32_t save_temp_to_flash(char *tmpdata)
{
	struct iwreq wrq;
	struct sfcfghdr tmp;
	int offset = 174;
	int length = 2;
	memset(&wrq, 0, sizeof(struct iwreq));
	memset(&tmp, 0, sizeof(struct sfcfghdr));
	// cmd ate band
	tmp.magic_no = SFCFG_MAGIC_NO;
	tmp.comand_type = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
	tmp.comand_id   = SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH;
	tmp.length      = 2;
	tmp.sequence    = 1;

	sprintf(tmp.data,"%d",offset);
	memcpy(&tmp.data[4],tmpdata,length);
	printf("tmpdata= %d---------\n", tmpdata[0]);

	wrq.u.data.pointer = (caddr_t)&tmp;
	wrq.u.data.length = sizeof(struct sfcfghdr) - sizeof(((struct sfcfghdr *)0)->data) + 10;

	if(do_ioctl(SFCFG_PRIV_IOCTL_ATE, &wrq) < 0){
		printf("SFCFG_PRIV_IOCTL_ATE ioctl SFCFG_CMD_ATE_SAVE_DATA_TO_FLASH failed\n");
		return -1;
	}
	return 0;
}

static void usage()
{
	printf("Usage:\n"
			"  ate_cmd wlan0/1 set/read/write "
			"ATEXXX = XXX \\\n"
			"\n"
		  );
	printf(" ate_cmd wlan0 set ATE = ATESTART, ATE start\n");
	printf(" ate_cmd wlan0 set ATE = ATETXSTART, Tx frame start (-t)\n");
	printf(" ate_cmd wlan0 set ATE = ATETXSTOP, Stop current TX action (-s)\n");
	printf(" ate_cmd wlan0 set ATE = ATERXSTART, RX frame start (-r)\n");
	printf(" ate_cmd wlan0 set ATE = ATERXSTOP, Stop current RX action (-k)\n");
	printf(" ate_cmd wlan0 set ATE = ATESHOW, Show Setting info\n");
	printf(" ate_cmd wlan0 set ATEPHYBW = X,(0~3) Set ATE PHY Bandwidth (-w)\n");
	printf(" ate_cmd wlan0 set ATETXBW = X,(0~3) Set ATE Tx Frame Bandwidth (-u)\n");
	printf(" ate_cmd wlan0 set ATECHAN = X, Set ATE Channel(primmary freq), decimal(-f)\n");
	printf(" ate_cmd wlan0 set ATECENTERFREQ1 = X, Set ATE center freq1, decimal(-c)\n");
	printf(" ate_cmd wlan0 set ATETXNUM = X, Set ATE Tx Frame Number(-n)\n");
	printf(" ate_cmd wlan0 set ATELEN = X, Set ATE Frame Length(-l)\n");
	printf(" ate_cmd wlan0 set ATETXMODE = X,(0~4) Set ATE Tx Mode(-m)\n");
	printf(" ate_cmd wlan0 set ATETXMCS = X, Set ATE Tx MCS type(-i)\n");
	printf(" ate_cmd wlan0 set ATETXGI = X,(0~1) Set ATE Tx Guard Interval(-g)\n");
	printf(" ate_cmd wlan0 set ATETXPOW = XX,(0~31) Set ATE Tx power(-p)\n");
	printf(" ate_cmd wlan0 set ATETXPREAM = X,(0~1) Set ATE Tx pream\n");
	printf(" ate_cmd wlan0 set ATETXDA = XX:XX:XX:XX:XX:XX, Set da\n");
	printf(" ate_cmd wlan0 set ATETXSA = XX:XX:XX:XX:XX:XX, Set sa\n");
	printf(" ate_cmd wlan0 set ATETXBSSID = XX:XX:XX:XX:XX:XX, Set bssid\n");
	printf(" ate_cmd wlan0 read ATEREG = X, Read the Value of One Register, X must be 0xYYYYYYYY or 0xYYYY\n");
	printf(" ate_cmd wlan0 write ATEREG = X(addr)_X(val), Write the Value of One Rergister\n");
	//printf(" ate_cmd wlan0 read ATEBULKREG = type_num_X1_X1_..., Read Many Registers\n");
	//printf(" ate_cmd wlan0 write ATEBULKREG = type_num_addr1_val1_X2_X2..., Write Many Registers\n");
	printf(" ate_cmd wlan0 set ATE = ATEMACHDR -f 0x0008 -t 0x0000 -b 00:aa:bb:cc:dd:ee -d 16:88:aa:bb:cc:dd -s 16:88:aa:bb:cc:dd -q 0x5e20, Set the frame mac header\n");
	printf(" ate_cmd wlan1 fastconfig -n 0 -l 1024 -f 5500 -c 5530 -w 3 -u 3 -m 4 -i 9 -g 0 -p 28 -t, Transmit frames by fast setting on HB\n");
	printf(" ate_cmd wlan1 fastconfig -l 1024 -f 5500 -c 5530 -w 3 -u 3 -m 4 -i 9 -g 0 -p 28 -y, Transmit frames by macbypass on HB\n");
	printf(" ate_cmd wlan1 fastconfig -q, stop tx by macbypass on HB!!\n");
	printf(" ate_cmd wlan1 fastconfig -f 5500 -c 5500 -w 3 -u 3 -p 28 -o, Transmit test tone on HB\n");
	printf(" ate_cmd wlan1 fastconfig -x, Stop test tone on HB\n");
	printf(" ate_cmd wlan1 fastconfig -n 0 -l 1024 -f 5500 -c 5500 -w 1 -u 1 -m 2 -i 7 -g 1 -p 28 -a 16, Transmit aggregated frames by fast setting on HB, 16 is the max agg buffer size\n");
	printf("ate_cmd wlan1 fastconfig -f 5500 -c 5500 -w 3 -u 3 -m 4 -i 9 -g 0 -p factory -y\n");//power param is factorya
	printf("ate_cmd save data to flash postion value1value2value3........\n");//save data to flash ;
	printf(" if you use (ate_init lb/hb) to active ate tool, only use wlan0\n");
	printf(" if you use (ate_init) to active ate tool, use wlan0 for 2.4G test and wlan1 for 5G test\n");

}

int32_t ate_main(char *buffer)
{
	char value[6] = "0";
	uint16_t value2;
    	uint16_t value3;
	uint16_t value4;
	uint16_t value5;
	uint16_t value6;
	uint32_t offset;

	printf(" enter into ate cmd main-------------\n");

    if(strlen(buffer) < 8){
        usage();
        return -1;
    }

	send_ate_start();

	if(!strncasecmp(buffer,"ate_cmd save xo ",15)){
		sscanf(buffer,"ate_cmd save xo %s", value);
		if(save_XO_config_to_flash(value))
			return -3;
		goto DONE;
	}
	if(!strncasecmp(buffer,"ate_cmd save ver ",16)){
		sscanf(buffer,"ate_cmd save ver %s", value);
		if(save_version_to_factory(value))
			return -3;
		goto DONE;
	}
	if(!strncasecmp(buffer,"ate_cmd save pow ",16)){
		sscanf(buffer,"ate_cmd save pow %hd %hd %hd %hd %hd",&value6, &value2, &value3, &value4, &value5);
		if(save_power_config_to_flash(value6,value2,value3,value4,value5))
			return -3;
		goto DONE;
	}
	if(!strncasecmp(buffer,"ate_cmd save data to flash ",sizeof("ate_cmd save data to flash ")-1)){
	       sscanf(buffer,"ate_cmd save data to flash %d",&offset);
	       buffer=buffer + sizeof("ate_cmd save data to flash ")+sizeof(offset);
	       printf("buffer is=  %d = %c\n",*buffer,*buffer);
	       if(save_data_to_flash(offset,buffer))
				return -1;
		     goto DONE;
	}
	if(!strncasecmp(buffer,"ate_cmd fastconfig ",18)){
	//**************************************
		char num[] = "10000";
		char length[]= "1024";
		char pri_freq[] = "1111";
		char cen_freq[] = "1111";
		char rf_bw[] = "0";
		char frame_bw[] = "0";
		char mode[] = "0";
		char mcs[] = "0";
		char sgi[] = "0";
		char power[] = "25";
		char op[] = "s";
		char arg[10] = "0";
		char addr_val[] = "0x1301_0x0000";
		//char* addr_value = NULL;
		char data[4] = "xo";
		char band[] = "\0";
		//int tmp_power= 0;
		if(!strncasecmp(buffer,"ate_cmd fastconfig z ",20)){
			sscanf(buffer,"ate_cmd fastconfig %s %s",op,addr_val);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig o ",20)){
			sscanf(buffer,"ate_cmd fastconfig %s f:%s c:%s p:%s %s",op,pri_freq,cen_freq,power,data);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig single r ",27)){
				sscanf(buffer,"ate_cmd fastconfig single %s f:%s c:%s w:%s",op,pri_freq,cen_freq,rf_bw);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig per r ",24)){
				sscanf(buffer,"ate_cmd fastconfig per %s f:%s c:%s w:%s N:%f ",op,pri_freq,cen_freq,rf_bw,&rx_pkt_all);
				printf("rx send all pkt is: %f \n",rx_pkt_all);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig r ",20)){
				sscanf(buffer,"ate_cmd fastconfig %s f:%s c:%s w:%s",op,pri_freq,cen_freq,rf_bw);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig y ",20)){
			sscanf(buffer,"ate_cmd fastconfig %s l:%s f:%s c:%s w:%s u:%s m:%s i:%s g:%s B:%s p:%s",
				op,length,pri_freq,cen_freq,rf_bw,frame_bw,mode,mcs,sgi,band,power);
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n",op ,length, pri_freq, cen_freq);
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig e ",20)){
			sscanf(buffer,"ate_cmd fastconfig %s %s",op,addr_val);
		}
		else{
			sscanf(buffer,"ate_cmd fastconfig %s n:%s l:%s f:%s c:%s w:%s u:%s m:%s i:%s g:%s p:%s",
				op,num,length,pri_freq,cen_freq,rf_bw,frame_bw,mode,mcs,sgi,power);
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n",op ,num, length, pri_freq);
		}
		if(pri_freq[0] == '5'){
			strcpy(ifname, "wlan1");
		}
		if(pri_freq[0] == '2'){
			strcpy(ifname, "wlan0");
		}
		//set_pkg_da(da);
		//set_pkg_sa(sa);
		//set_pkg_bssid(bssid);
		switch (op[0])
		{
			case 't':
				g_tx_frame_flag = true;
				printf("%s -t param g_tx_frame_flag=%d\n", __func__, g_tx_frame_flag);
				break;
			case 'r':
				g_rx_frame_flag = true;
				printf("%s -r param g_rx_frame_flag=%d\n", __func__, g_rx_frame_flag);
				break;
			case 's':
				g_stop_tx_frame_flag = true;
				printf("%s -s param g_stop_tx_frame_flag=%d\n", __func__, g_stop_tx_frame_flag);
				goto NEXT;
			case 'k':
				g_stop_rx_frame_flag = true;
				printf("%s -k param g_stop_rx_frame_flag=%d\n", __func__, g_stop_rx_frame_flag);
				goto NEXT;
			case 'y':
				g_start_tx_frame_by_macbypass_flag = true;
				printf("%s -y param g_start_tx_frame_by_macbypass_flag=%d\n", __func__, g_start_tx_frame_by_macbypass_flag);
				break;
			case 'e':
				if(set_xo_value(addr_val))
					return -2;
				goto NEXT;
			case 'q':
				g_stop_tx_frame_by_macbypass_flag = true;
				printf("%s -y param g_stop_tx_frame_by_macbypass_flag=%d\n", __func__, g_stop_tx_frame_by_macbypass_flag);
				goto NEXT;
			case 'o':
				g_tx_test_tone_flag = true;
				printf("%s -o param g_tx_test_tone_flag=%d\n", __func__, g_tx_test_tone_flag);
				break;
			case 'x':
				g_stop_tx_test_tone_flag = true;
				printf("%s -x param g_stop_tx_test_tone_flag=%d\n", __func__, g_stop_tx_test_tone_flag);
				goto NEXT;
			case 'v':
				if(read_iw_reg(arg))
					return -2;
				goto NEXT;
			case 'z':
				if(write_iw_reg(addr_val))
					return -2;
				goto NEXT;
			case 'a':
				if(read_iw_bulk_reg(arg))
					return -2;
				goto NEXT;
			case 'b':
				if(write_iw_bulk_reg(arg))
					return -2;
				goto NEXT;
			case 'm':
				g_start_tx_frame_by_macbypass_flag = true;
				printf("%s -y param g_start_tx_frame_by_macbypass_flag=%d\n", __func__, g_start_tx_frame_by_macbypass_flag);
				break;
			case '?':
			case ':':
				usage();
				break;
		}
		if(set_pkg_frame_num(num))
			return -2;
		if(set_pkg_frame_length(length))
			return -2;
		if(set_phy_chan_freq(pri_freq))
			return -2;
		if(set_phy_center_freq1(cen_freq))
			return -2;
		if(set_phy_bandwidth(rf_bw))
			return -2;
		if(set_pkg_bandwidth(frame_bw))
			return -2;
		if(set_phy_mode(mode))
			return -2;
		if(set_phy_rate_idx(mcs))
			return -2;
		if(set_phy_use_sgi(sgi))
			return -2;
		if(set_phy_power(power,pri_freq,rf_bw,mode,mcs,band))
			return -2;
NEXT:
		if (g_tx_frame_flag) {
			if(send_tx_start())
				return -2;
			g_tx_frame_flag = false;
			goto DONE;
		}

		if (g_rx_frame_flag) {
			if(send_rx_start())
				return -2;
			g_rx_frame_flag = false;
			goto DONE;
		}

		if (g_stop_tx_frame_flag) {
			if(send_tx_stop())
				return -2;
			g_stop_tx_frame_flag = false;
			goto DONE;
		}

		if (g_stop_rx_frame_flag) {
			if(send_rx_stop())
				return -2;
			g_stop_rx_frame_flag = false;
			goto DONE;
		}

		if (g_start_tx_frame_by_macbypass_flag) {
			if(send_tx_start_by_macbypass())
				return -2;
			g_start_tx_frame_by_macbypass_flag = false;
			goto DONE;
		}

		if (g_stop_tx_frame_by_macbypass_flag) {
			if(send_tx_stop_by_macbypass())
				return -2;
			g_stop_tx_frame_by_macbypass_flag = false;
			goto DONE;
		}

		if (g_tx_test_tone_flag) {
			if(send_tx_test_tone_start(data))
				return -2;
			g_tx_test_tone_flag = false;
			goto DONE;
		}

		if (g_stop_tx_test_tone_flag) {
			if(send_tx_test_tone_stop())
				return -2;
			g_stop_tx_test_tone_flag = false;
			goto DONE;
		}
	}
DONE:
	close_ioctl_socket();
	return 1;
}

int main()
{
	int sfp,nfp;
	int i;
	struct sockaddr_in s_add,c_add;
	socklen_t sin_size;
	int optval = 1;
	int ret = 0;
	int cali_table_write_flg = 0;
	enum Cali_table_part cali_table_part = LB_ANT1;
	float per_n = 0;
	unsigned short portnum=1234;//Port NO
	char bufsend[1024]={0};
	//char bufend_calitable[2048] = {0};
	char buffer[1024]={0};
	char tmp[1024]="ate_cmd fastconfig t n:10000 l:1024 f:2412 c:2412 w:2 u:2 m:2 i:7 g:0 p:28";
	char tmp1[]="stop socket";
#if	USE_ONE_CALI_TABLE
	unsigned char bufsend_calitable[2048] = {0};
	unsigned char buffer2[2048]={0};
#else
	unsigned char bufsend_calitable[3382] = {0};
	unsigned char buffer2[3382]={0};
	int error_code = 0;
	int recv_len = 0;
	int buffer2_offset = 0;
#endif

	//int k_return = 0;
	//1.create socket, AF_INET is IPv4 address;SOCL_STREAM is the type of transmit
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	//sfp = socket(AF_PACKET, SOCK_STREAM, 0);
	if(-1 == sfp)
	{
		printf("socket fail ! \r\n");
		return -1;
	}
	// RM#1003287 continuously use the command ate_server lead to bind fail
	setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	printf("**socket ok !\r\n");
	//2.Bind
	bzero(&s_add,sizeof(struct sockaddr_in));
	s_add.sin_family=AF_INET;//IPv4
	//s_add.sin_addr.s_addr=htonl(INADDR_ANY);//all zero
	s_add.sin_addr.s_addr=inet_addr("192.168.4.1");
	s_add.sin_port=htons(portnum);
	//socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, 1);
	if(-1 == bind(sfp,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
	{
		printf("bind fail !\r\n");
		printf("1.Please make sure that br-lan is 192.168.4.1!\n");
		return -1;
	}
	printf("**bind ok !\r\n");
	//3.listen
	if(-1 == listen(sfp,10))
	{
		printf("listen fail !\r\n");
		return -1;
	}
	printf("**listen ok\r\n");
	//4.accept
while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		/* accept*/
		nfp = accept(sfp, (struct sockaddr *)(&c_add), &sin_size);//back a new socket and client link
		if(-1 == nfp)
		{
			printf("accept fail !\r\n");
			//return -1;
			goto NEXT;
		}
		printf("**accept ok!\r\n**Server start get connect from %#x : %#x\r\n",ntohl(c_add.sin_addr.s_addr),ntohs(c_add.sin_port));

		memset(bufsend,0,1024);
		strncpy(bufsend,"connect ok!",11);
		if(-1 == write(nfp,bufsend,strlen(bufsend))){
			printf("send cmd fail!\r\n");
			goto NEXT;
		}

		//5.
while(1){
BEGAIN:
		printf("waiting for command...\n");
		memset(buffer,0,1024);
		//recv(nfp,buffer,1024,0);
		if(cali_table_write_flg == 1)
		{
#if USE_ONE_CALI_TABLE
			recv(nfp,buffer2,2048,0);
			memset(bufsend_calitable,0,2048);
			memcpy(bufsend_calitable,buffer2,2048);
#if 1 //debug
			printf("recv cali table is : \n");
			for(i = 0;i < 1689;i++)
			{
				//TODO need write xo & version
				if(i <= 27 )
				{
					printf("%u ",bufsend_calitable[i]);
						if((i%27==0) && (i!=0))
							printf("\n");

				}else if(i > 27 && i < 364)
				{
					printf("%u ",bufsend_calitable[i]);
                        if((i-27)%28==0)
                            printf("\n");
				}
				else {
					printf("%u ",bufsend_calitable[i]);
						if((i-363)%53==0)
							printf("\n");
				}
			}
#endif
			save_power_table_to_factory(2044,0,bufsend_calitable);
			cali_table_write_flg = 0;
			system("sfwifi_ate temp_disable lb");
			goto BEGAIN;
#else
			printf("waiting table from tool...\n");
			// receive calitable in 4 times (LB_ANT1 HB_ANT1 LB_ANT2 HB_ANT2)
			while (cali_table_part != CALI_TABLE_PART_NUM)
			{
				recv_len = 0;
				buffer2_offset = 0;
				switch (cali_table_part)
				{
					case LB_ANT1:
						buffer2_offset = recv_len;
						recv_len += recv(nfp, buffer2 + buffer2_offset, CALI_TABLE_2G_SIZE + 1, 0);
						printf("*****receive len = %d*****\n",recv_len);
						strncpy(bufsend,"recv lb1 table ok!",18);
						write(nfp,bufsend,strlen(bufsend));
						//enter next part
						cali_table_part++;
						break;
					case HB_ANT1:
						buffer2_offset = CALI_TABLE_2G_SIZE + recv_len;
						recv_len += recv(nfp, buffer2 + buffer2_offset, CALI_TABLE_5G_SIZE + 1, 0);
						printf("*****receive len = %d*****\n",recv_len);
						strncpy(bufsend,"recv hb1 table ok!",18);
						write(nfp,bufsend,strlen(bufsend));
						cali_table_part++;
						break;
					case LB_ANT2:
						buffer2_offset = CALI_TABLE_5G_SIZE + CALI_TABLE_2G_SIZE + recv_len;
						recv_len += recv(nfp, buffer2 + buffer2_offset, CALI_TABLE_2G_SIZE + 1, 0);
						printf("*****receive len = %d*****\n",recv_len);
						strncpy(bufsend,"recv lb2 table ok!",18);
						write(nfp,bufsend,strlen(bufsend));
						cali_table_part++;
						break;
					case HB_ANT2:
						buffer2_offset = CALI_TABLE_5G_SIZE + CALI_TABLE_2G_SIZE*2 + recv_len;
						recv_len += recv(nfp, buffer2 + buffer2_offset, CALI_TABLE_5G_SIZE + 1, 0);
						printf("*****receive len = %d*****\n",recv_len);
						strncpy(bufsend,"recv hb2 table ok!",18);
						write(nfp,bufsend,strlen(bufsend));
						cali_table_part++;
						break;
				}

			}
			printf("get table from tool over\n");

			memset(bufsend_calitable,0,3378);
			memcpy(bufsend_calitable,buffer2,3378);
#if 1 //debug
			printf("recv cali table is : \n");
			for(i = 0;i < 3378;i++)
			{
				//TODO need write xo & version
				if(i <= 27 )
				{
					printf("%u ",bufsend_calitable[i]);
						if((i%27==0) && (i!=0))
							printf("\n");

				}else if(i > 27 && i < 364)
				{
					printf("%u ",bufsend_calitable[i]);
                        if((i-27)%28==0)
                            printf("\n");
				}
				else if(i >= 364 && i < 1689){
					printf("%u ",bufsend_calitable[i]);
						if((i-363)%53==0)
							printf("\n");
				}
				else if(i >= 1689 && i <= 1716 )
				{
					printf("%u ",bufsend_calitable[i]);
						if(((i-1689)%27==0) && (i!=1689))
							printf("\n");

				}else if(i > 1716 && i < 2053)
				{
					printf("%u ",bufsend_calitable[i]);
                        if((i-1716)%28==0)
                            printf("\n");
				}
				else {
					printf("%u ",bufsend_calitable[i]);
						if((i-2052)%53==0)
							printf("\n");
				}
			}
#endif
			save_power_table_to_factory(4092,0,bufsend_calitable);
			cali_table_write_flg = 0;
			cali_table_part = LB_ANT1;
			system("sfwifi_ate temp_disable lb");
			goto BEGAIN;
		}
#endif
		else
		{
			recv(nfp,buffer,1024,0);
		}
		// buffer[1024]='\0';
		printf("Message from client:%s\r\n",buffer);
		if(!strncasecmp(buffer,"ate_init lb1",12)){
			printf("******ate tool init lb1******\n");
			if(system("ate_init lb1") == -1)
				ret = -4;
			if(system("sfwifi_ate temp_disable lb1") == -1)
				ret = -4;
			ret = 2;
		}
		else if(!strncasecmp(buffer,"ate_init lb2",12)){
			printf("******ate tool init lb2******\n");
			if(system("ate_init lb2") == -1)
				ret = -4;
			if(system("sfwifi_ate temp_disable lb2") == -1)
				ret = -4;
			ret = 2;
		}
		else if(!strncasecmp(buffer,"ate_init hb1",12)){
			printf("******ate tool init hb1******\n");
			if(system("ate_init hb1") == -1)
				ret = -4;
			if(system("sfwifi_ate temp_disable hb1") == -1)
				ret = -4;
			ret = 2;
		}
		else if(!strncasecmp(buffer,"ate_init hb2",12)){
			printf("******ate tool init hb2******\n");
			if(system("ate_init hb2") == -1)
				ret = -4;
			if(system("sfwifi_ate temp_disable hb2") == -1)
				ret = -4;
			ret = 2;
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig get",22)){
					memset(bufsend,0,1024);
					get_rx_info(1);
					snprintf(bufsend,1023,"per receive data = %d,fcs ok  = %d,fcs error = %d,phy error = %d,fcs_ok_for_macaddr = %d,fcs_group = %d\n",user->rec_rx_count, user->fcs_ok, user->fcs_err, user->phy_err ,user->fcs_ok_for_macaddr ,user->fcs_group);
					if(rx_pkt_all != 0)
					{
						per_n = 1 - (user->fcs_ok / rx_pkt_all);
						if(user->rec_rx_count >= rx_pkt_all)
						{
							printf("rx test per is %.2f \n",per_n);
							snprintf(bufsend,1023,"PER = %.2f \n",per_n);
							ret = 2;
							//goto NEXT;
						}
					}
					if(-1 == write(nfp,bufsend,strlen(bufsend))){
						printf("send cmd fail!\r\n");
						goto NEXT;
					}
					goto BEGAIN;
		}
		else if(!strncasecmp(buffer,"ate_cmd fastconfig single r",27)){
					ate_main(buffer);
					memset(bufsend,0,1024);
					if(!strncasecmp(buffer,"ate_cmd fastconfig single r",27))
						get_rx_info(0);
					snprintf(bufsend,1023,"receive data=%d,rssi=%d,PER(/2sec)=%d,PER(all)=%d\n",user->rec_rx_count, user->rssi, per, user->per);
					if(-1 == write(nfp,bufsend,strlen(bufsend))){
						printf("send cmd fail!\r\n");
						goto NEXT;
					}
					goto BEGAIN;
		}

		else if (!strncasecmp(buffer,"ate_cmd get cali_table ",20)){
			memset(bufsend_calitable,0,2048);
			read_gain_table_from_mtd(bufsend_calitable);
			int pos = write(nfp,bufsend_calitable,2048);
			int size = 2048;
			int n_pos = size - pos;
			printf("pos is :%d n_pos is :%dsize is %d\r\n",pos,n_pos,size);
			while(n_pos >0){
			pos = write(nfp,&bufsend_calitable[pos],n_pos);
			n_pos = n_pos - pos;
			printf("pos is :%dn_pos is%d\r\n",pos,n_pos);

			if (pos == -1){
			printf("in socket ,write fail!!!!!");
			goto NEXT;
			}
			else if (n_pos  == 0){
				goto BEGAIN;
			}
			}
		}
		else if (!strncasecmp(buffer,"ate_cmd write cali_table ",22)){
			/*
			 * flg value means:
			 * 1: recv write table cmd change to 1
			 * 0: not recv write table cmd, default 0
			 * */
			printf("*****receive ate_cmd write cali_table*****\n");
			cali_table_write_flg = 1;
			cali_table_part = LB_ANT1;
			goto BEGAIN;
		}
		else if (!strncasecmp(buffer,"ate_cmd get_current_temp",24)){
			FILE *file;
			char buffer[16];

			file = fopen("/sys/kernel/debug/aetnensis/temperature", "r");
			if (file == NULL) {
				perror("Unable to open file");
			}

			if (fgets(buffer, sizeof(buffer), file) != NULL) {
				snprintf(bufsend,1023,"get temp is: %s\n",buffer);
				write(nfp,bufsend,strlen(bufsend));
				fclose(file);
				goto BEGAIN;
			} else {
				perror("Unable to read file");
			}
			ret = 0;
			fclose(file);
		}
		else if (!strncasecmp(buffer,"ate_cmd save temp ",18)){
			char temp[2] = "0";
			if (sscanf(buffer + 18, "%d", &temp[0]) == 1) {
				printf("temp value: %d\n", temp[0]);
				if(temp[0] < 0)
				{
					printf("----temperature is %d degrees below zero!!----\n",temp[0]);
					temp[0] = ~temp[0] + 1;
					//if use temp below zero ,set the flag 1,default 0;
					temp[1] = 1;
				}
				if(!save_temp_to_flash(temp))
					printf("----save temperature ok!----\n");
			} else {
				printf("Failed to extract a valid integer.\n");
			}
		}
		else if(!strncasecmp(buffer,tmp,7)){
			printf("ate cmd receive!\n");
			ret = ate_main(buffer);
		}
		else if(!strncasecmp(buffer,tmp1,11)){
			printf("Socket stop!\n");
			ret = -1;
		}
		else{
			ret = 3;
		}

		memset(bufsend,0,1024);
		switch(ret){
			case -4: strncpy(bufsend,"ate init fail!",15);
					 break;
			case -3: strncpy(bufsend,"ate test tool save config fail!",33);
					 break;
			case -2: strncpy(bufsend,"ate test process fail!",20);
					 break;
			case -1: strncpy(bufsend,"socket server stop!",20);
					 break;
			case  1: strncpy(bufsend,"ate cmd success!",16);
					 break;
			case  2: strncpy(bufsend,"ate init success!",18);
					 break;
			case  3: strncpy(bufsend,"Error CMD!",11);
					 break;
			default: strncpy(bufsend,"do nothing!",11);
					 break;
		}
		if(-1 == write(nfp,bufsend,strlen(bufsend)))
		{
			printf("send cmd fail!\n");
			goto NEXT;
		}
		printf("send cmd success!\n");
		if(ret == -1)
			goto DONE;
		}
NEXT:
	printf("waiting...\n");
	close(nfp);
	printf("no client, waiting...\n");
	sleep(1);
	}
DONE:
	close(nfp);
	close(sfp);
	return 0;
}
