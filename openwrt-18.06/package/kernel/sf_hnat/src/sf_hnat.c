#include "sf_hnat.h"
#ifdef  CONFIG_SFAX8_HNAT_TEST_TOOL
#include "sf_hnat_test_tool.h"
#endif
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/crc16.h>
#include <net/vxlan.h>
#include <linux/vmalloc.h>
#include <linux/netfilter.h>
#include <net/netfilter/nf_flow_table.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_pppox.h>
#include <linux/ppp_defs.h>
#include <linux/if_vlan.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <linux/list.h>
#include <linux/sched.h>

#include "sf_hnat_common.h"
#include "sf_hnat_reg.h"
#ifdef CONFIG_DEBUG_FS
#include "sf_hnat_debug.h"
#endif

#include <linux/if_addr.h>

#define NAPT_HASH2_INDEX_OFFSET 2
#define NAPT_HASH3_INDEX_OFFSET 6
#define DIP_HASH2_INDEX_OFFSET	4
#define DEFAULT_WAN_MASKLEN 24
#define DEFAULT_LAN_MASKLEN 24
#define PPPOE_EN_MASK       0x00000002

spinlock_t hnat_tb_lock;
spinlock_t hnat_csr_lock;
spinlock_t hnat_napt_lock;
spinlock_t hnat_br_lock;


#define HASH_DEBUG			0x1
#define NAPT_DEBUG			0x2
#define WDEV_DEBUG			0x4
#define SUBNET_DEBUG		0x8
#define DIP_HASH_DEBUG		0x10
#define MONITOR_IP_DEBUG	0x20
#define ARP_DEBUG			0x40
#define COUNT_DEBUG			0x80

struct sf_hashkey sf_hnat_napt_key[NAPT_TABLE_MAX];
unsigned char g_print = 0;
unsigned int add_fail_count = 0;
unsigned int update_flow_count = 0;
unsigned int crc_clean_flow_count = 0;
//table value set reg num

// extern void sf_offload_hw_clean_flow(struct flow_offload *flow);
struct sf_hnat_priv* g_phnat_priv = NULL;

struct net_bridge *g_br = NULL;
struct net_device *g_br_dev = NULL;
int g_pppoe_flag = 0;

static unsigned char hnat_table_value_reg_num[32] =
{
	0, 1, 0, 0, 1, 0, 4, 1,
	1, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 2, 1, 0,
	1, 0, 0, 0, 0, 0, 0, 0
};


static unsigned short hnat_table_entry_num[32] =
{	512, 256, 32, 512, 256, 32, 1024, 32,
	16,	 0,   0,  0,   0,   0,  0,    0,
	256, 64,  512,16,  128, 8,  8,    128,
	128, 0,   0,   0,  0,   0,  0,    0
};

static unsigned int hnat_table_last_value_reg_mask[32] =
{
	0x003fffff, 0x00000fff, 0x003fffff, 0x003fffff,
	0x00000fff, 0x003fffff, 0x00000001, 0x00000000,
	0x00000fff, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000,

	0x000000ff, 0x000000ff, 0x00003fff, 0x00000000,
	0x0007ffff, 0x00000000, 0x0000ffff, 0x00000fff,
	0x00000000,	0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000
};


sf_napt_crc_info sf_hnat_napt_crc_info[NAPT_TABLE_MAX];
//the napt key and napt crc info is one-to-one

static sf_dip_entry sf_hnat_dip_entry[DIP_TABLE_MAX];
static sf_dip_crc_info sf_hnat_dip_crc_info[DIP_TABLE_MAX];
//the dip key and dip crc info is one-to-one

static sf_dmac_entry sf_hnat_dmac_entry[DMAC_TABLE_MAX];
static sf_rmac_entry sf_hnat_rmac_entry[ROUTER_MAC_TABLE_MAX];
static sf_ppphd_entry sf_hnat_ppphd_entry[PPPHD_TABLE_MAX];//here we only need session id
static sf_rt_pub_net_entry sf_hnat_rt_pub_net_entry[RT_PUB_NET_TABLE_MAX];
static sf_vlan_entry sf_hnat_vlan_entry[VLAN_TABLE_MAX];

unsigned int napt_full_calculate = 0;
unsigned int napt_hash_full_count = 0;
unsigned int dip_hash_full_count = 0;

void print_flow_offload_hw_path(struct flow_offload_hw_path *path);
void print_flow_offload(struct flow_offload *flow);

// #define INAT_NAPT_HASH1_WID     0
/*
 *  * byte_arry_to_int
 *   * convert unsigned char arry to int
 *    */
unsigned int hnat_readl(struct sf_hnat_priv* phnat_priv, unsigned int address){
	unsigned int value = 0;
	if(phnat_priv->base == 0){
		return 0;
	}
	value = readl(phnat_priv->base + address);
//	  printk(" ret 0x%x, address 0x%x",value, (unsigned int)phnat_priv->base + address );
		//printk("readl(0x%x);\n",(unsigned int)phnat_priv->base + address);
	return value;
}
void hnat_writel(struct sf_hnat_priv* phnat_priv, unsigned int data, unsigned int  address){
//	  printk(" data 0x%x, address 0x%x",data, (unsigned int)phnat_priv->base + address );
		//printk("writel(0x%x, 0x%x);\n",data, (unsigned int)phnat_priv->base + address );
	if(phnat_priv->base == 0){
		return;
	}
	writel(data, phnat_priv->base + address );
}

static void sf_hnat_clean_flow(struct flow_offload *flow)
{
	flow->timeout = (u32)jiffies + NF_FLOW_TIMEOUT;
	set_bit(NF_FLOW_HW_DYING, &flow->flags);
	set_bit(NF_FLOW_HW_DEAD, &flow->flags);
}

/*
 *  * byte_arry_to_int
 *   * convert unsigned char arry to int
 *    */

static char  sf_hnat_search_wifi_ndev_index(struct sf_hnat_priv *phnat_priv, struct net_device * ndev){
	unsigned char i = 0;
	for(; i < 8 ; i++){
		if(phnat_priv->pwifi_ndev[i] == ndev){
			if(g_print & WDEV_DEBUG )
			  printk("[hnat notice]find wifi ndev  exist, index %d ndev %p\n", i, ndev);
			return i;
		}
	}
	return -1;
}

int sf_hnat_update_lan_subnet(struct sf_hnat_priv *phnat_priv, unsigned char index){
	unsigned int netmask_value;
	unsigned int data;
	unsigned int set_register = HWNAT_REG16_CSR + 0x04 * index;
	unsigned int ipaddr;
	unsigned char prefix_len;

	if(!phnat_priv)
	  return 0;

	ipaddr = phnat_priv->lan_subnet[index].ipaddr;
	prefix_len = phnat_priv->lan_subnet[index].prefix_len;


	spin_lock(&hnat_csr_lock);

	hnat_writel(phnat_priv, ipaddr, set_register);
	if(index < 4){
		set_register = HWNAT_REG30_CSR;
	} else{
		set_register = HWNAT_REG31_CSR;
	}
	netmask_value = hnat_readl(phnat_priv, set_register) & ~(0xff << (index * 8));
	netmask_value |= (((32-prefix_len) | 0x20) << (index * 8));
	hnat_writel(phnat_priv, netmask_value, set_register);
	data = hnat_readl(phnat_priv, HWNAT_REG2_CSR);
	data = data & 0xfffffffe;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);
	data = data | 0x01;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);

	spin_unlock(&hnat_csr_lock);

	phnat_priv->lan_subnet[index].valid = 1;
	//TODO  clean relate napt entry here
	return 0;
}

int sf_hnat_del_lan_subnet(struct sf_hnat_priv *phnat_priv, unsigned int index){
	unsigned int netmask_value;
	unsigned int data;
	unsigned int set_register = HWNAT_REG16_CSR + 0x04 * index;
	unsigned  short  i = 0;

	if(!phnat_priv)
	  return 0 ;

	phnat_priv->lan_subnet[index].valid = 0;
	phnat_priv->lan_subnet[index].ipaddr = 0;
	phnat_priv->lan_subnet[index].prefix_len = 0;

	// clean lan

	for(;i < NAPT_TABLE_MAX; i++){
		if (sf_hnat_napt_key[i].valid == 1 ){
			if (sf_hnat_napt_key[i].lan_subnet_index == index ) {
				sf_hnat_delete_entry_by_index(phnat_priv, i);
			}
		}
	}
	spin_lock(&hnat_csr_lock);
	hnat_writel(phnat_priv, 0x0, set_register);
	if(index < 4){
		set_register = HWNAT_REG30_CSR;
	} else{
		set_register = HWNAT_REG31_CSR;
	}
	netmask_value = hnat_readl(phnat_priv, set_register) & ~(0xff << (index * 8));
	hnat_writel(phnat_priv, netmask_value, set_register);
	data = hnat_readl(phnat_priv, HWNAT_REG2_CSR);
	data = data & 0xfffffffe;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);
	data = data | 0x01;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);
	spin_unlock(&hnat_csr_lock);

	return 0;
}

int sf_hnat_del_wan_subnet(struct sf_hnat_priv *phnat_priv, unsigned int index){

	unsigned  short i = 0;

	if(!phnat_priv)
	  return 0;
	// clean wan
	phnat_priv->wan_subnet[index].valid = 0;
	phnat_priv->wan_subnet[index].ipaddr = 0;
	phnat_priv->wan_subnet[index].prefix_len = 0;
	for(;i < NAPT_TABLE_MAX; i++){
		if (sf_hnat_napt_key[i].valid == 1 ){
			if (sf_hnat_napt_key[i].wan_subnet_index == index ) {
				sf_hnat_delete_entry_by_index(phnat_priv, i);
			}
		}
	}

	return 0;
}

void sf_hnat_dump_napt_key_hash_debug(struct sf_hashkey * sf_key){
	u16 sport, dport;           /* Source and destination ports */
	u32 saddr, daddr;
	// printk("%s==============================start", __FUNCTION__);
	saddr = ntohl(sf_key->sip);
	daddr = ntohl(sf_key->dip);
	sport = ntohs(sf_key->sport);
	dport = ntohs(sf_key->dport);

	printk("src:[%pI4h:%d] -> dest:[%pI4h:%d]\n", &saddr, sport, &daddr, dport);

	saddr = ntohl(sf_key->router_pub_ip);
	sport = ntohs(sf_key->router_port);

	printk("router:[%pI4h:%d] proto %d\n", &saddr, sport, sf_key->proto);
}


void sf_hnat_dump_napt_hash(sf_napt_crc_info *sf_napt_info, unsigned char is_inat){
	unsigned char ptr_index = 0;
	unsigned short *key = NULL;
	if(is_inat){
		ptr_index = sf_napt_info->inat_ptr_index;
		key = sf_napt_info->inat_crc_key;
	}
	else{
		ptr_index = sf_napt_info->enat_ptr_index;
		key = sf_napt_info->enat_crc_key;
	}

	if(ptr_index < NAPT_HASH2_INDEX_OFFSET){
		if(is_inat)
		  printk("[hnat dump] hash INAT tbl 1: index %d, ptr %d\n", key[0], ptr_index);
		else
		  printk("[hnat dump] hash ENAT tbl 1: index %d, ptr %d\n", key[0], ptr_index);
	}//del pointer in hash1
	if(ptr_index >= NAPT_HASH2_INDEX_OFFSET && ptr_index < NAPT_HASH3_INDEX_OFFSET){
		if(is_inat)
		  printk("[hnat dump] hash INAT tbl 2: index %d, ptr %d\n",  key[1], ptr_index - NAPT_HASH2_INDEX_OFFSET);
		else
		  printk("[hnat dump] hash ENAT tbl 2: index %d, ptr %d\n",  key[1], ptr_index - NAPT_HASH2_INDEX_OFFSET);
	}//del pointer in hash2
	if(ptr_index  >= NAPT_HASH3_INDEX_OFFSET){
		if(is_inat)
		  printk("[hnat dump] hash INAT tbl 3: index %d, pit %d\n",  key[2], ptr_index - NAPT_HASH3_INDEX_OFFSET);
		else
		  printk("[hnat dump] hash ENAT tbl 3: index %d, ptr %d\n",  key[2], ptr_index - NAPT_HASH3_INDEX_OFFSET);
	}//del pointer in hash3
}

void sf_hnat_dump_napt_hash_by_index(unsigned short napt_index){
	struct sf_hashkey *sf_key;
	sf_key = &sf_hnat_napt_key[napt_index];
	if(sf_key->valid == 1){
		sf_hnat_dump_napt_hash(&sf_hnat_napt_crc_info[napt_index], 0);
		sf_hnat_dump_napt_hash(&sf_hnat_napt_crc_info[napt_index], 1);
	}
	return;
}

void sf_hnat_dump_napt_key_by_index(unsigned short napt_index){
	struct sf_hashkey *sf_key;
	sf_key = &sf_hnat_napt_key[napt_index];
	if(sf_key->valid == 1){
		printk("####napt key, napt_index %d\n", napt_index);
		sf_hnat_dump_napt_key(sf_key);
	}
	return;
}

void sf_hnat_dump_napt_key(struct sf_hashkey * sf_key){


	u16 sport, dport;           /* Source and destination ports */
	u32 saddr, daddr;
	// printk("%s===========start flow 0x%p", __FUNCTION__,sf_key->pkey_flow);
	saddr = ntohl(sf_key->sip);
	daddr = ntohl(sf_key->dip);
	sport = ntohs(sf_key->sport);
	dport = ntohs(sf_key->dport);

	printk("src:[%pI4h:%d] -> dest:[%pI4h:%d]\n", &saddr, sport, &daddr, dport);

	printk("smac %pM vlanid %d \n router src  mac %pM\n",sf_key->src_mac, sf_key->src_vlan, sf_key->router_src_mac) ;

	printk("dmac %pM  vlanid %d \n router dest mac %pM\n",sf_key->dest_mac, sf_key->dest_vlan, sf_key->router_dest_mac) ;

	saddr = ntohl(sf_key->router_pub_ip);
	sport = ntohs(sf_key->router_port);

	printk("router:[%pI4h:%d]\n", &saddr, sport);

	printk("pppoe sid  0x%x  proto %d cur_ppoe_en %d\n",sf_key->ppp_sid,sf_key->proto, sf_key->cur_pppoe_en) ;

	printk("valid = %d src_vlan_index %d dest_vlan_index %d\n", sf_key->valid, sf_key->src_vlan_index, sf_key->dest_vlan_index);
	printk("rt_pub_net_index = %d src_dip_index %d dest_dip_index %d\n", sf_key->rt_pub_net_index, sf_key->src_dip_index, sf_key->dest_dip_index);
	printk("ppphd_index = %d dnat_to_host %d is_dip_rt_ip_same_subnet %d\n", sf_key->ppphd_index, sf_key->dnat_to_host, sf_key->is_dip_rt_ip_same_subnet);
	printk("lan_index = %d wan_index %d\n", sf_key->lan_subnet_index, sf_key->wan_subnet_index);
	printk("%s===========end", __FUNCTION__);
}


int check_table_no_dep_wid(int tab_no,int index, int *entry_data) {
	unsigned char value_reg_num = hnat_table_value_reg_num[tab_no];
	unsigned short value_entry_num = hnat_table_entry_num[tab_no];
	unsigned int last_value_reg_mask = hnat_table_last_value_reg_mask[tab_no];
	//8~16 is reserved now
	if(tab_no > 8 && tab_no < 16)
		return -1;
	if(entry_data){
		entry_data[value_reg_num] = entry_data[value_reg_num] & last_value_reg_mask;
		return value_reg_num;
	}
	else{
		if(index >= 0 && index < value_entry_num)
		  return value_entry_num;
	}
	return -1;
}

int sf_hnat_wait_reg_rw_not_busy(struct sf_hnat_priv *phnat_priv) {
	int value;
	unsigned int i = 0;
	value = hnat_readl(phnat_priv, HWNAT_REG1_TB_STATUS);
	while(value & HWNAT_REG1_TB_STATUS_BUSY ){
		value = hnat_readl(phnat_priv, HWNAT_REG1_TB_STATUS);
		if(i > 1000){
			printk("%s timeout\n", __FUNCTION__);
			break;
		}
		i++;
	}
	return value;
}

int sf_hnat_table_wr(struct sf_hnat_priv * phnat_priv, int tab_no,int depth,int *data) {
	int value;
	int ret = check_table_no_dep_wid(tab_no,depth,NULL);
	if(ret < 0)
	  return -1;

	  // printk("[hnat notice] %s ==== tab_no 0x%x, index 0x%x\n",__FUNCTION__ , tab_no,depth);

	spin_lock_bh(&hnat_tb_lock);
	sf_hnat_wait_reg_rw_not_busy(phnat_priv);

	value = (tab_no << HWNAT_REG3_TB_ADD_NO_OFF) | depth;
	hnat_writel(phnat_priv, value,HWNAT_REG3_TB_ADDRESS);

	value = data[0];
	hnat_writel(phnat_priv, value,HWNAT_REG5_TB_WRDATA0);
	value = data[1];
	hnat_writel(phnat_priv, value,HWNAT_REG6_TB_WRDATA1);
	value = data[2];
	hnat_writel(phnat_priv, value,HWNAT_REG7_TB_WRDATA2);
	value = data[3];
	hnat_writel(phnat_priv, value,HWNAT_REG8_TB_WRDATA3);
	value = data[4];
	hnat_writel(phnat_priv, value,HWNAT_REG9_TB_WRDATA4);

	value = HWNAT_REG4_TB_OP_CODE_WR;
	hnat_writel(phnat_priv, value,HWNAT_REG4_TB_OP_CODE);

	sf_hnat_wait_reg_rw_not_busy(phnat_priv);
	  // printk("[hnat notice] %s ==== ================end\n",__FUNCTION__ );
	spin_unlock_bh(&hnat_tb_lock);
	return 0;
}


int sf_hnat_table_rd(struct sf_hnat_priv* phnat_priv, int tab_no,int depth,int *data) {
	int value;
	int width;
	int ret = check_table_no_dep_wid(tab_no,depth,NULL);
	if(ret < 0)
	  return -1;

	spin_lock_bh(&hnat_tb_lock);

		 // printk("[hnat notice] %s ==== tab_no 0x%x, index 0x%x\n",__FUNCTION__ , tab_no,depth);
	sf_hnat_wait_reg_rw_not_busy(phnat_priv);
	value = (tab_no << HWNAT_REG3_TB_ADD_NO_OFF) | depth;
	hnat_writel(phnat_priv, value,HWNAT_REG3_TB_ADDRESS);

	value = HWNAT_REG4_TB_OP_CODE_RD;
	hnat_writel(phnat_priv, value,HWNAT_REG4_TB_OP_CODE);
	sf_hnat_wait_reg_rw_not_busy(phnat_priv);

	value = hnat_readl(phnat_priv, HWNAT_REG10_TB_RDDATA0);
	data[0] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG11_TB_RDDATA1);
	data[1] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG12_TB_RDDATA2);
	data[2] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG13_TB_RDDATA3);
	data[3] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG14_TB_RDDATA4);
	data[4] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG15_TB_RDDATA5);
	data[5] = value;
	value = hnat_readl(phnat_priv, HWNAT_REG16_TB_RDDATA6);
	data[6] = value;

	width = check_table_no_dep_wid(tab_no,0,data);
		// printk("[hnat notice] %s ====  read width 0x%x\n",__FUNCTION__ , width);
	spin_unlock_bh(&hnat_tb_lock);

	return width;
}


//pkt_data:start of sip
//ret 2 means pppoe + tcp
unsigned char sf_hnat_table_fast_search(struct sf_hnat_priv *phnat_priv, unsigned char is_inat, unsigned char *pkt_data, int proto_type) {
	unsigned int value = 0;
	unsigned int hnat_info = 0;
	// hnat_writel(phnat_priv, 0xfff0011, HWNAT_REG2_TB_CONFIG);
	// printk("start search\n");
	if(phnat_priv->base == 0){
		return 0;
	}

	spin_lock(&hnat_tb_lock);
	sf_hnat_wait_reg_rw_not_busy(phnat_priv);

	hnat_writel(phnat_priv, *(unsigned int *)(pkt_data + 4),HWNAT_REG5_TB_WRDATA0);

	hnat_writel(phnat_priv,  (*(unsigned short *)pkt_data) << 16  | *(unsigned short *)(pkt_data + 10) , HWNAT_REG6_TB_WRDATA1);

	hnat_writel(phnat_priv,  *(unsigned short *)(pkt_data + 2)  | *(unsigned short *)(pkt_data + 8) << 16 ,HWNAT_REG7_TB_WRDATA2);

	hnat_writel(phnat_priv, proto_type, HWNAT_REG8_TB_WRDATA3);

	if (is_inat == 0)
	  hnat_writel(phnat_priv, 0x2,HWNAT_REG4_TB_OP_CODE);
	else
	  hnat_writel(phnat_priv, 0x102,HWNAT_REG4_TB_OP_CODE);

	value = sf_hnat_wait_reg_rw_not_busy(phnat_priv);

	hnat_info = hnat_readl(phnat_priv, HWNAT_REG15_TB_RDDATA5);
	// hnat_writel(phnat_priv, 0xfff0001, HWNAT_REG2_TB_CONFIG);

	spin_unlock(&hnat_tb_lock);


	if(value & 0x2){
		if ((hnat_info & PPPOE_EN_MASK) && (proto_type == 0))
		  return 2;// tcp pppoe napt hit
		else
		  return 1;//napt hit
	}
	else
		return 0;//napt no hit
}

int sf_hnat_speed_read(struct sf_hnat_priv *phnat_priv) {
	unsigned int rx_min_byte,rx_max_byte,tx_min_byte,tx_max_byte;
	unsigned int rx_min_pkt,rx_max_pkt,tx_min_pkt,tx_max_pkt;
	unsigned int i = 0;
	u64 rx_total_byte,tx_total_byte,rx_total_pkt,tx_total_pkt,x,y,div;

	hnat_writel(phnat_priv, 0x7cff,HWNAT_REG52_CSR);
	hnat_writel(phnat_priv, 0x270f,HWNAT_REG53_CSR);
	hnat_writel(phnat_priv, 0x1,HWNAT_REG51_CSR);

	while(hnat_readl(phnat_priv, HWNAT_REG54_CSR) & 0x1){
		if(i > 1000){
			printk("%s timeout\n", __FUNCTION__);
			return 0;
		}
		i++;
		msleep(10);
	}

	rx_min_pkt = hnat_readl(phnat_priv, HWNAT_REG55_CSR);
	rx_max_pkt = hnat_readl(phnat_priv, HWNAT_REG56_CSR);
	rx_total_pkt = hnat_readl(phnat_priv, HWNAT_REG58_CSR);
	rx_total_pkt = rx_total_pkt << 32 | hnat_readl(phnat_priv, HWNAT_REG57_CSR);

	rx_min_byte = hnat_readl(phnat_priv, HWNAT_REG59_CSR);
	rx_max_byte = hnat_readl(phnat_priv, HWNAT_REG60_CSR);
	rx_total_byte = hnat_readl(phnat_priv, HWNAT_REG62_CSR);
	rx_total_byte = rx_total_byte << 32 | hnat_readl(phnat_priv, HWNAT_REG61_CSR);

	tx_min_pkt = hnat_readl(phnat_priv, HWNAT_REG70_CSR);
	tx_max_pkt = hnat_readl(phnat_priv, HWNAT_REG71_CSR);
	tx_total_pkt = hnat_readl(phnat_priv, HWNAT_REG73_CSR);
	tx_total_pkt = tx_total_pkt << 32 | hnat_readl(phnat_priv, HWNAT_REG72_CSR);

	tx_min_byte = hnat_readl(phnat_priv, HWNAT_REG74_CSR);
	tx_max_byte = hnat_readl(phnat_priv, HWNAT_REG75_CSR);
	tx_total_byte = hnat_readl(phnat_priv, HWNAT_REG77_CSR);
	tx_total_byte = tx_total_byte << 32 | hnat_readl(phnat_priv, HWNAT_REG76_CSR);

	printk("rx min speed %uMbps, L1 speed %uMbps\n", (rx_min_byte*8*1000)/(32000*8),
			(rx_min_byte+rx_min_pkt*24)*8*1000/32000/8);
	printk("rx max speed %uMbps, L1 speed %uMbps\n", (rx_max_byte*8*1000)/(32000*8),
			(rx_max_byte+rx_max_pkt*24)*8*1000/32000/8);
	x = (rx_total_byte*8*1000);
	y = ((rx_total_byte+rx_total_pkt*24)*8*1000);
	div = (u64)32000*8*10000;
	do_div(x,div);
	do_div(y,div);
	printk("rx average speed %lldMbps, L1 speed %lldMbps\n",x,y);

	printk("tx min speed %dMbps, L1 speed %uMbps\n", (tx_min_byte*8*1000)/(32000*8),
			(tx_min_byte+tx_min_pkt*24)*8*1000/32000/8);
	printk("tx max speed %dMbps, L1 speed %uMbps\n", (tx_max_byte*8*1000)/(32000*8),
			(tx_min_byte+tx_min_pkt*24)*8*1000/32000/8);
	x = (tx_total_byte*8*1000);
	y = ((tx_total_byte+tx_total_pkt*24)*8*1000);
	do_div(x,div);
	do_div(y,div);
	printk("tx average speed %lldMbps, L1 speed %lldMbps\n",x,y);
	return 0;
}

int sf_hnat_table_clean(struct sf_hnat_priv * phnat_priv) {
	int data[HWNAT_TB_WRDATA_NO];
	int table_no;
	int table_dep;
	int depth;
	memset(data,0, sizeof(data));
	for (table_no = 0;table_no < HNAT_TB_NO;table_no ++){
		table_dep = check_table_no_dep_wid(table_no,0,NULL);//get depth
		if(table_dep > 0){
			for (depth = 0;depth < table_dep;depth ++){
				sf_hnat_table_wr(phnat_priv, table_no,depth,data);
			}
		}
	}
	return 0;
}


//print the data read from table_reg
void print_read_data(int *data){
	int i = 0;
	unsigned char * tmp ;
	for(;i < 7;i++){
		tmp = (unsigned char *)(data + i);
		printk("data%d = 0x%02x %02x %02x %02x\n",i, *tmp, *(tmp+1), *(tmp+2),*(tmp+3));
	}
}

void sf_hnat_print_the_table_content(struct sf_hnat_priv *phnat_priv, int tab_no,int depth){
	int data[7] = {0};
	printk(">>>>>>>>read table no = %d:\n", tab_no);
	sf_hnat_table_rd(phnat_priv, tab_no, depth, data);
	print_read_data(data);
	printk(">>>>>>>>read table no = %d end\n", tab_no);
}

int sf_dump_dip_table_content_by_index(struct sf_hnat_priv *phnat_priv, int index){
	sf_dip_entry *p_dip_entry;
	sf_dip_crc_info *p_dip_info;
	unsigned short *disf_key;

	p_dip_info = &sf_hnat_dip_crc_info[index];
	p_dip_entry = &sf_hnat_dip_entry[index];
	disf_key = p_dip_info->dip_crc_key;

	if(p_dip_entry->valid == 0)
	  return 0;

	sf_hnat_print_the_table_content(phnat_priv, DIP, index);
	printk("dip crc table\n");
	sf_hnat_print_the_table_content(phnat_priv, DIP_HASH1, disf_key[0]);
	sf_hnat_print_the_table_content(phnat_priv, DIP_HASH2, disf_key[1]);

	printk("print dmac table\n");
	sf_hnat_print_the_table_content(phnat_priv, DMAC, p_dip_entry->dmac_index);

	printk("print vlan table\n");
	sf_hnat_print_the_table_content(phnat_priv, VLAN_ID, p_dip_entry->vlan_index);

	return 0;
}
int sf_dump_napt_table_content_by_index(struct sf_hnat_priv *phnat_priv, int napt_index){
	sf_napt_crc_info *p_napt_info;
	struct sf_hashkey *sf_key;
	int table_no = INAT_NAPT_HASH1;
	unsigned short *inat_key;
	unsigned short *enat_key;

	sf_key = &sf_hnat_napt_key[napt_index];
	p_napt_info = &sf_hnat_napt_crc_info[napt_index];
	inat_key = p_napt_info->inat_crc_key;
	enat_key = p_napt_info->enat_crc_key;

	if(sf_key->valid == 0)
	  return 0;

	printk("####napt key, napt_index %d\n", napt_index);
	sf_hnat_dump_napt_key(sf_key);

	printk("####napt table\n");
	sf_hnat_print_the_table_content(phnat_priv, NAPT, napt_index);
#if 1
	printk("napt crc table\n");
	for(;table_no < NAPT; table_no++){
		if(table_no < ENAT_NAPT_HASH1)
		  sf_hnat_print_the_table_content(phnat_priv, table_no, inat_key[table_no]);
		else
		  sf_hnat_print_the_table_content(phnat_priv, table_no, enat_key[table_no-3]);
	}
#endif

	printk("####rt pub net  table\n");
	sf_hnat_print_the_table_content(phnat_priv, RT_PUB_NET, sf_key->rt_pub_net_index);

	printk("####src ip dip  table\n");
	sf_dump_dip_table_content_by_index(phnat_priv, sf_key->src_dip_index);
	printk("####dest ip dip  table\n");
	sf_dump_dip_table_content_by_index(phnat_priv, sf_key->dest_dip_index);

	if(sf_key->cur_pppoe_en){
		printk("####ppphd   table\n");
		sf_hnat_print_the_table_content(phnat_priv, PPPOE_HD, sf_key->ppphd_index);
	}

	return 0;
}


/**4100  GMAC收到的总报文数量
 * 4170  判断需要做snat的报文数量
 * 4174  判断需要做dnat的报文数量
 * 4124  查中转发到GMAC的报文
 * 4114  上送host的报文
 * 412c	 host下发的报文
 * */
void sf_hnat_read_count_registers(struct sf_hnat_priv *phnat_priv){
	unsigned char i,offset;
	unsigned int	 addr, data, cnt1, cnt2;
	unsigned long long counter1,counter2;
	unsigned int tmp_reg[20] = {0};
	unsigned int tmp_count[40] = {0};


	addr = HWNAT_REG0_CSR;
	for(i = 0; i < 20;i++){
		tmp_reg[i] = hnat_readl(phnat_priv, addr);
		addr += 4;
	}

	data = hnat_readl(phnat_priv, HWNAT_REG2_CSR);
	data = data & 0xffffffef;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);
	data = data | 0x10;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);

	addr = HWNAT_RX_ENTER_SOF_CNT_L;
	for(i = 0; i < 40;i++){
		tmp_count[i] = hnat_readl(phnat_priv, addr);
		addr +=4;
	}

	printk("CSR REGISTERS START:\n");
	addr = HWNAT_REG0_CSR;
	for(i = 0; i < 5;i++){
		offset = i*4;
		printk("addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x", addr, tmp_reg[offset], (addr+4), tmp_reg[offset+1], (addr+8), tmp_reg[offset+2], (addr+12), tmp_reg[offset + 3]);
		addr += 16;
	}
	printk("CSR REGISTERS END:\n");
	// read clean

	printk("COUNTER REGISTERS START:\n");
	addr = HWNAT_RX_ENTER_SOF_CNT_L;
	for(i = 0; i < 10;i++){
		offset = i*4;
		printk("addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x", addr, tmp_count[offset], (addr+4), tmp_count[offset+1], (addr+8), tmp_count[offset+2], (addr+12), tmp_count[offset + 3]);
		addr += 16;
	}

	printk("COUNTER REGISTERS END:\n");
	offset = 0;
	counter1 = tmp_count[offset+1];
	counter1 = counter1 << 32 | tmp_count[offset];
	printk(" GMAC reciv all %lld pkts \n", counter1);

	offset = ((HWNAT_RX_ENAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L))/4;
	printk(" rx %u pkts need to snat\n", tmp_count[offset]);

	offset = ((HWNAT_RX_INAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L))/4;
	printk(" rx %u pkts need to dnat\n", tmp_count[offset]);

	offset = ((HWNAT_TX_ENAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	printk(" tx %u pkts need to snat\n", tmp_count[offset]);
	offset = ((HWNAT_RX2TX_DATA_FRAME_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;

	printk(" %u pkts are hit to GMAC\n", tmp_count[offset]);

	offset = ((HWNAT_TX_SOF_FRAME_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	printk(" host send %d pkts to hnat\n",tmp_count[offset]);

	printk("===============RX=============\n");

	offset = 0;
	counter1 = tmp_count[offset+1];
	counter1 = counter1 << 32 | tmp_count[offset];

	offset = ((HWNAT_RX_ENTER_EOF_CNT_L) - (HWNAT_RX_ENTER_SOF_CNT_L))/4;
	counter2 = tmp_count[offset+1];
	counter2 = counter2 << 32 | tmp_count[offset];
	printk("rx_enter_sof_cnt        %-20llu, rx_enter_eof_cnt         %-20llu\n",counter1,counter2);

	offset = ((HWNAT_RX_2HOST_FRAME_SOF_CNT_L) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	counter1 = tmp_count[offset+1];
	counter1 = counter1 << 32 | tmp_count[offset];

	offset = ((HWNAT_RX_2HOST_FRAME_EOF_CNT_L) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	counter2 = tmp_count[offset+1];
	counter2 = counter2 << 32 | tmp_count[offset];
	printk("rx_2host_frame_sof_cnt  %-20llu, rx_2host_frame_eof_cnt   %-20llu\n",counter1,counter2);

	offset = ((HWNAT_RX_ENTER_DROP_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX2TX_DATA_FRAME_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx_enter_drop_cnt       %-20u, rx2tx_data_frame_cnt     %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RX_ENAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX_INAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx_enat_cnt             %-20u, rx_inat_cnt              %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RX2TX2HOST_CNT ) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX2TX_DROP_CNT ) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx2tx2host_cnt          %-20u, rx2tx_drop_cnt           %-20u\n",cnt1,cnt2);

	printk("===============TX=============\n");

	offset = ((HWNAT_TX_SOF_FRAME_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_TX_EOF_FRAME_CNT )- (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("tx_sof_frame_cnt        %-20u, tx_eof_frame_cnt         %-20u\n",cnt1,cnt2);


	offset = ((HWNAT_TX_EXIT_SOF_CNT_L) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	counter1 = tmp_count[offset+1];
	counter1 = counter1 << 32 | tmp_count[offset];

	offset = ((HWNAT_TX_EXIT_EOF_CNT_L) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	counter2 = tmp_count[offset+1];
	counter2 = counter2 << 32 | tmp_count[offset];
	printk("tx_exit_sof_cnt         %-20llu, tx_exit_eof_cnt          %-20llu\n",counter1,counter2);


	offset = ((HWNAT_TX_RECEIVE_RX_FRAME_CNT )- (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_TX_NOHITS_FRAME_CNT ) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("tx_receive_rx_frame_cnt %-20u, tx_nohits_frame_cnt      %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RCV_STATUS_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_TX_STATUS_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("Hnat_rcv_status_cnt     %-20u, Hnat_tx_status_cnt       %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RCV_TXACK_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_GEN_RXACK_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("Hnat_rcv_txack_cnt      %-20u, Hnat_gen_rxack_cnt       %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_2MTL_ACK_CNT )- (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_MTL_2HNAT_RDYN_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("Hnat2mtl_ack_cnt        %-20u, Mtl_2hnat_rdyn_cnt       %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_TX_REC_RX_TXTRANS_DROP_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_TX_ENAT_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("Timeout_eof_cnt         %-20u, Tx_enat_cnt              %-20u\n",cnt1,cnt2);

	printk("=============ERROR============\n");

	offset = ((HWNAT_PKT_ERR_CNT_SYNC) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX_TOTAL_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx2tx_errpkt_cnt        %-20u, rx_total_err_cnt         %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RX_GMII_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX_CRC_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx_gmii_err_cnt         %-20u, rx_crc_err_cnt           %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RX_LENGTH_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	offset = ((HWNAT_RX_IPHDR_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt2 = tmp_count[offset];
	printk("rx_length_err_cnt       %-20u, rx_iphdr_err_cnt         %-20u\n",cnt1,cnt2);

	offset = ((HWNAT_RX_PAYLOAD_ERR_CNT) - (HWNAT_RX_ENTER_SOF_CNT_L)) /4;
	cnt1 = tmp_count[offset];
	printk("rx_payload_err_cnt      %-20u\n",cnt1);
}

void sf_hnat_csr_init(struct sf_hnat_priv *phnat_priv){
	int data = 0, i = 0;

	hnat_writel(phnat_priv, 0x0040004f | phnat_priv->hnat_mode, HWNAT_REG1_CSR);
	hnat_readl(phnat_priv, HWNAT_REG1_CSR);

	// ndev stop will trigger hnat reset, so restore lan subnet setting here
	for(i=0; i<8; i++){
		if(phnat_priv->lan_subnet[i].valid )
		{
			sf_hnat_update_lan_subnet(phnat_priv, i);
		}
	}
#if defined(CONFIG_DT_SF19A28_FULLMASK_PHY)
	hnat_writel(phnat_priv, 0x32, HWNAT_REG4_CSR);
#elif defined(CONFIG_DT_SF19A28_FULLMASK_REP_PHY)
	hnat_writel(phnat_priv, 0xa, HWNAT_REG4_CSR);
#else
	hnat_writel(phnat_priv, 0x64, HWNAT_REG4_CSR);
#endif

	// vlan replace for not vlan tag situation
	if (phnat_priv->vlan_replace)
	  hnat_writel(phnat_priv, 0x700000, HWNAT_REG3_CSR);
	else
	  hnat_writel(phnat_priv, 0x400000, HWNAT_REG3_CSR);

	//enable all tables,bit 0 is enat_arp_force_dip
	data = HWNAT_REG2_TB_CONFIG_ENTB;

	if(phnat_priv->snat_arp_force_dip)
	  data = data | HWNAT_REG2_TB_CONFIG_USE_DIP;

	if(phnat_priv->use_big_endian)
	  data = data | HWNAT_REG2_TB_CONFIG_EN_BIG_ENDIAN;

	data = data | HWNAT_REG2_TB_CONFIG_CRC3_SEL | HWNAT_REG2_TB_CONFIG_READ_CLR;

	hnat_writel(phnat_priv, data, HWNAT_REG2_TB_CONFIG);//auto judge
	hnat_writel(phnat_priv, 0x1500, HWNAT_REG19_REG_TB_RFC_TIMER);
	hnat_writel(phnat_priv, 0x1500, HWNAT_REG21_REG_TB_TFC_TIMER);
	hnat_writel(phnat_priv,0x3, HWNAT_REG18_REG_TB_FC_CFG);
	hnat_writel(phnat_priv,0x0, HWNAT_REG18_REG_TB_FC_CFG);
#if defined(CONFIG_DT_SF19A28_FULLMASK_PHY)
	hnat_writel(phnat_priv, 0x2040204, HWNAT_REG17_REG_BUF_THRESH);
#elif defined(CONFIG_DT_SF19A28_FULLMASK_REP_PHY)
	hnat_writel(phnat_priv, 0x1090109, HWNAT_REG17_REG_BUF_THRESH);
#else
	hnat_writel(phnat_priv, 0x20f020f, HWNAT_REG17_REG_BUF_THRESH);
#endif
	hnat_writel(phnat_priv, 0x165a0bc0, HWNAT_REG33_CSR);
	hnat_writel(phnat_priv, 0x5f5e10, HWNAT_REG34_CSR);
	hnat_writel(phnat_priv, 0xffffffff, HWNAT_REG35_CSR);
	hnat_writel(phnat_priv, 0x47868c0, HWNAT_REG36_CSR);
}

//get length of 0 bit in netmask
unsigned char get_netmask_len(unsigned int netmask) {
	unsigned char i, masklen;
	for(i = 0; i < 32; i++){
		if((netmask >> i) & 0x1){
			masklen = i;
			break;
		}
		masklen = 32;		//if not break, masklen = 32
	}
	return masklen;
}

char sf_hnat_is_lan_ip(struct sf_hnat_priv* phnat_priv, unsigned int client_ip){
	unsigned char masklen;
	unsigned char i = 0;

	for (;i < 8; i++) {
		if(phnat_priv->lan_subnet[i].valid) {
			masklen = 32 - phnat_priv->lan_subnet[i].prefix_len;
			if((ntohl(client_ip) >> masklen) == (phnat_priv->lan_subnet[i].ipaddr >> masklen))
			  return i;
		}
	}

	return -1;
}//default mask 255.255.255.0

char sf_hnat_is_wan_ip(struct sf_hnat_priv* phnat_priv, unsigned int  rt_pub_ip){

	unsigned char i = 0;

	for (;i < 8; i++) {
		if(phnat_priv->wan_subnet[i].valid) {
			if(rt_pub_ip == htonl(phnat_priv->wan_subnet[i].ipaddr))
			  return i;
		}
	}

	return -1;
}//default mask 255.255.255.0


int sf_hnat_judge_dip_rt_ip(struct sf_hnat_priv* phnat_priv, unsigned short napt_index){
	struct sf_hashkey* sf_key = &sf_hnat_napt_key[napt_index];
	unsigned char masklen = 32 - DEFAULT_WAN_MASKLEN;
	unsigned char i = 0;

	for(i=0; i<8; i++){
		if(phnat_priv->wan_subnet[i].valid){
			if(sf_key->router_pub_ip == htonl(phnat_priv->wan_subnet[i].ipaddr)){
				masklen = 32 - phnat_priv->wan_subnet[i].prefix_len;//need lenth of 0 bit here
				if(g_print & ARP_DEBUG)
				  printk("[hnat debug] get masklen for judge %d \n", masklen);
			}

		}
	}
	if((ntohl(sf_key->dip) >> masklen)  ==  (ntohl(sf_key->router_pub_ip) >> masklen))
	  sf_key->is_dip_rt_ip_same_subnet = 1;//in the same segment
	else
	  sf_key->is_dip_rt_ip_same_subnet = 0;//in the different segment

	if(g_print & ARP_DEBUG)
	  printk("[hnat debug] dip mask  0x%x  rtpubip mask 0x%x dip %08x rt ip %08x  same %d\n", ntohl(sf_key->dip) >> masklen, ntohl(sf_key->router_pub_ip) >> masklen,sf_key->dip, sf_key->router_pub_ip,sf_key->is_dip_rt_ip_same_subnet );

	return 0;
}//default mask 255.255.255.0

int sf_hnat_napt_vld_cfg(struct sf_hnat_priv* phnat_priv, unsigned short napt_index,char enable){
	int data[7] = {0};
	int depth = napt_index / 32;
	sf_hnat_table_rd(phnat_priv, NAPT_VLD, depth, data);
	if(enable)
	  data[0] = data[0] | (1 << (napt_index - depth*32));
	else
	  data[0] = data[0] & ~(1 << (napt_index - depth*32));

	sf_hnat_table_wr(phnat_priv, NAPT_VLD, depth, data);
	return 0;
}

int sf_hnat_dip_vld_cfg(struct sf_hnat_priv* phnat_priv, unsigned short dip_index,char enable){
	int data[7] = {0};
	int depth = dip_index / 32;
	sf_hnat_table_rd(phnat_priv, DIP_VLD, depth, data);
	if(enable)
	  data[0] = data[0] | (1 << (dip_index - depth*32));
	else
	  data[0] = data[0] & ~(1 << (dip_index - depth*32));

	sf_hnat_table_wr(phnat_priv, DIP_VLD, depth, data);
	return 0;
}

//vlan index is 7bit,rcc for arp_hash key calculate
int rcc_dip(unsigned char *data){
	int i = 3;
	for(; i >= 0;i--){
		data[i + 1] = data[i + 1] & 0x7f;
		data[i + 1] = data[i + 1] | ((data[i] & 0x1) << 7);
		data[i] = data[i] >> 1;
	}

	return 0;
}

unsigned short sf_hnat_crc16(unsigned char *buf, unsigned char len , unsigned short * crc_key) {
	u16 crc_value = 0;
	u16 crc2_value = 0;
	unsigned char * buf_op = buf;
	unsigned char op_len = len;
	int i;
	while(op_len > 0) {
		crc_value   = crc_value  ^ (*buf_op++ << 8);
		for (i = 0; i < 8; i++)
		{
			if (crc_value  & 0x8000)
			  crc_value  = (crc_value  << 1) ^ 0x1021;
			else
			  crc_value  = crc_value  << 1;
		}
		op_len--;
	}

	op_len = len;
	buf_op = buf;
	while(op_len > 0) {
		crc2_value   = crc2_value  ^ (*buf_op++ << 8);
		for (i = 0; i < 8; i++)
		{
			if (crc2_value  & 0x8000)
			  crc2_value  = (crc2_value  << 1) ^ 0x8005;
			else
			  crc2_value  = crc2_value  << 1;
		}
		op_len--;
	}

	crc_key[0] = (crc_value & 0xff80) >> 7;
	crc_key[1] =  crc_value & 0xff;
	crc_key[2] = (crc2_value & 0x1f0) >> 4;
	return crc_value;
}

int sf_napt_key_crc_get(struct sf_hnat_priv * phnat_priv, struct sf_hashkey *key,   unsigned  short * crc_key , unsigned char is_inat) {
	unsigned char crc_data[13] = {0};
	unsigned short crc_value;
	if (phnat_priv->hnat_mode == BASIC_MODE) {
		if (is_inat == 0) {
			memcpy(crc_data + 3, (unsigned char *)&key->sip, 4);
		} else {
			memcpy(crc_data + 9, (unsigned char *)&key->router_pub_ip, 4);
		}
	}else if (phnat_priv->hnat_mode == SYMMETRIC_MODE){
		crc_data[0] = key->proto;
		if (is_inat == 0) {
			memcpy(crc_data + 1, (unsigned char *)&key->sport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->sip, 4);
			memcpy(crc_data + 7, (unsigned char *)&key->dport, 2);
			memcpy(crc_data + 9, (unsigned char *)&key->dip, 4);
		} else {
			memcpy(crc_data + 1, (unsigned char *)&key->dport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->dip, 4);
			memcpy(crc_data + 7, (unsigned char *)&key->router_port, 2);
			memcpy(crc_data + 9, (unsigned char *)&key->router_pub_ip, 4);
		}
	}else if (phnat_priv->hnat_mode == FULL_CONE_MODE){
		if (is_inat == 0) {
			crc_data[0] = key->proto;
			memcpy(crc_data + 1, (unsigned char *)&key->sport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->sip, 4);
		} else {
			crc_data[0] = key->proto;
			memcpy(crc_data + 7, (unsigned char *)&key->router_port, 2);
			memcpy(crc_data + 9, (unsigned char *)&key->router_pub_ip, 4);
		}

	}else if (phnat_priv->hnat_mode == RESTRICT_CONE_MODE){
		if (is_inat == 0) {
			crc_data[0] = key->proto;
			memcpy(crc_data + 1, (unsigned char *)&key->sport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->sip, 4);
		} else {
			crc_data[0] = key->proto;
			memcpy(crc_data + 3, (unsigned char *)&key->dip, 4);
			memcpy(crc_data + 7, (unsigned char *)&key->router_port, 2);
			memcpy(crc_data + 9, (unsigned char *)&key->router_pub_ip, 4);
		}
	}else if(phnat_priv->hnat_mode == PORT_RESTRICT_CONE_MODE){
		if (is_inat == 0) {
			crc_data[0] = key->proto;
			memcpy(crc_data + 1, (unsigned char *)&key->sport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->sip, 4);
		} else {
			crc_data[0] = key->proto;
			memcpy(crc_data + 1, (unsigned char *)&key->dport, 2);
			memcpy(crc_data + 3, (unsigned char *)&key->dip, 4);
			memcpy(crc_data + 7, (unsigned char *)&key->router_port, 2);
			memcpy(crc_data + 9, (unsigned char *)&key->router_pub_ip, 4);
		}
	}else{
		return -1;
	}
	crc_value = sf_hnat_crc16(crc_data, 13, crc_key);
	return crc_value;
}



void sf_hnat_del_router_mac_entry(struct sf_hnat_priv * phnat_priv, unsigned char rmac_index) {
	int data[5] = {0};
	sf_hnat_table_wr(phnat_priv, ROUTER_MAC, rmac_index, data);
	return;
}
void sf_hnat_del_router_mac(struct sf_hnat_priv * phnat_priv, unsigned char rmac_index) {
	if(sf_hnat_rmac_entry[rmac_index].ref_count > 0)
	  sf_hnat_rmac_entry[rmac_index].ref_count--;

	if(sf_hnat_rmac_entry[rmac_index].ref_count == 0){
		sf_hnat_rmac_entry[rmac_index].valid = 0;
		phnat_priv->curr_rmac_num--;
		sf_hnat_del_router_mac_entry(phnat_priv, rmac_index);
	}
	return ;
}

//-1 not found
char sf_hnat_search_router_mac(struct sf_hnat_priv * phnat_priv, unsigned char* mac , unsigned char is_add){
	unsigned char i = 0;
	for(;i < ROUTER_MAC_TABLE_MAX; i++){
		if (sf_hnat_rmac_entry[i].valid == 1 ){
			if (memcmp(sf_hnat_rmac_entry[i].rmac, mac, 6) == 0 ) {
				if(is_add == 0){
					sf_hnat_del_router_mac(phnat_priv, i);
				}else if(is_add == 1){
					sf_hnat_rmac_entry[i].ref_count++;
				}
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s %pM search success index %d\n", __FUNCTION__,mac,i);
				return i;
			}
		}
	}
	return -1;
}

void sf_hnat_add_router_mac_entry(struct sf_hnat_priv * phnat_priv, unsigned char rmac_index) {
	int data[5] = {0};

	data[0] = *(unsigned int *)sf_hnat_rmac_entry[rmac_index].rmac; //bit 0~47 SA
	data[1] = *(unsigned short *)(sf_hnat_rmac_entry[rmac_index].rmac + 4);
	sf_hnat_table_wr(phnat_priv, ROUTER_MAC, rmac_index, data);
}
// -1 add fail
char sf_hnat_add_router_mac(struct sf_hnat_priv * phnat_priv, unsigned char * mac) {
	unsigned char i = 0;
	unsigned char rmac_index = 0;
	char ret = sf_hnat_search_router_mac(phnat_priv, mac, 1);
	if(ret < 0 ){
		for(; i < ROUTER_MAC_TABLE_MAX; i++) {
			if(sf_hnat_rmac_entry[i].valid == 0)
				break;
		}
		if(i == ROUTER_MAC_TABLE_MAX){
			printk("[hnat error]cannot found a slot in router mac table\n");
			return -1;
		}else{
			rmac_index = i;
		}
	}
	else
	  return ret;
	memcpy(sf_hnat_rmac_entry[rmac_index].rmac, mac, 6);
	sf_hnat_add_router_mac_entry(phnat_priv, rmac_index);
	sf_hnat_rmac_entry[rmac_index].valid = 1;
	sf_hnat_rmac_entry[rmac_index].ref_count = 1;
	phnat_priv->curr_rmac_num++;
	return rmac_index;
}

void sf_hnat_del_dmac_entry(struct sf_hnat_priv * phnat_priv, unsigned char dmac_index) {
	int data[5] = {0};
	sf_hnat_table_wr(phnat_priv, DMAC, dmac_index, data);
	return;

}
void sf_hnat_add_dmac_entry(struct sf_hnat_priv * phnat_priv, unsigned char dmac_index) {

	int data[5] = {0};
	data[0] = *(unsigned int *)sf_hnat_dmac_entry[dmac_index].dmac; //3it 0~47 DA
	data[1] = (*(unsigned short*)(sf_hnat_dmac_entry[dmac_index].dmac + 4)) | (sf_hnat_dmac_entry[dmac_index].router_mac_index << 16); //bit 48-50 router_mac index

	sf_hnat_table_wr(phnat_priv, DMAC, dmac_index, data);
}

//TODO what hanppened if full
void sf_hnat_del_dmac(struct sf_hnat_priv * phnat_priv, unsigned char dmac_index) {

	if(sf_hnat_dmac_entry[dmac_index].ref_count > 0)
	  sf_hnat_dmac_entry[dmac_index].ref_count--;

	if(sf_hnat_dmac_entry[dmac_index].ref_count == 0){
		sf_hnat_del_router_mac(phnat_priv, sf_hnat_dmac_entry[dmac_index].router_mac_index);
		sf_hnat_del_dmac_entry(phnat_priv, dmac_index);
		sf_hnat_dmac_entry[dmac_index].valid = 0;
		phnat_priv->curr_dmac_num--;
	}

	return ;
}


short sf_hnat_search_dmac(struct sf_hnat_priv * phnat_priv, unsigned char* mac, unsigned char is_add){

	unsigned char i = 0;
	for(;i < DMAC_TABLE_MAX; i++)	{
		if (sf_hnat_dmac_entry[i].valid == 1 ){
			if ((memcmp(sf_hnat_dmac_entry[i].dmac, mac, 6) == 0 ) ){
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s dmac: %pM index %d\n", __FUNCTION__, mac, i);
				if(is_add == 1){
					sf_hnat_dmac_entry[i].ref_count++;
				}else if(is_add == 0){
					sf_hnat_del_dmac(phnat_priv, i);
				}

				return i;
			}
		}
	}
	return -1;
}

//TODO what hanppened if full
short sf_hnat_add_dmac(struct sf_hnat_priv * phnat_priv, unsigned char* op_mac, unsigned char* router_mac) {
	char router_mac_index = -1;
	unsigned char i = 0;
	unsigned char dmac_index;
	short ret = sf_hnat_search_dmac(phnat_priv, op_mac, 1);
	if(ret < 0){
		for(; i < DMAC_TABLE_MAX; i++) {
			if(sf_hnat_dmac_entry[i].valid == 0)
				break;
		}
		if(i == DMAC_TABLE_MAX){
			printk("[hnat error]cannot found a slot in dmac mac table\n");
			return -1;
		}else{
			dmac_index = i;
		}
	}
	else
	  return ret;

	memcpy(sf_hnat_dmac_entry[dmac_index].dmac, op_mac, 6);
	router_mac_index = sf_hnat_search_router_mac(phnat_priv, router_mac, 1);
	if(router_mac_index < 0) {
		router_mac_index = sf_hnat_add_router_mac(phnat_priv, router_mac);
	}
	sf_hnat_dmac_entry[dmac_index].router_mac_index = router_mac_index;
	sf_hnat_add_dmac_entry(phnat_priv, dmac_index);

	sf_hnat_dmac_entry[dmac_index].valid = 1;
	sf_hnat_dmac_entry[dmac_index].ref_count = 1;
	phnat_priv->curr_dmac_num++;

	return dmac_index;
}

int sf_hnat_dump_dip_by_index(struct sf_hnat_priv * phnat_priv, unsigned short index){
	u32 ipaddr;
	u16 dmac_index = sf_hnat_dip_entry[index].dmac_index;
	ipaddr = ntohl(sf_hnat_dip_entry[index].dip);

	if(sf_hnat_dip_entry[index].valid == 1){
		printk("[hnat dump]index %d ip:[%pI4h] vlan index %d\n", index,&ipaddr, sf_hnat_dip_entry[index].vlan_index);
		printk("[hnat dump]dmac index %d mac %pM dip ref_count %d\n", dmac_index, sf_hnat_dmac_entry[dmac_index].dmac, sf_hnat_dip_entry[index].ref_count);
	}

	return 0;
}

int sf_hnat_dump_dip_hash_by_index(unsigned short index){
	unsigned char ptr_index = sf_hnat_dip_crc_info[index].dip_ptr_index;
	if(sf_hnat_dip_entry[index].valid == 1){
		if(ptr_index < DIP_HASH2_INDEX_OFFSET){
			printk("[hnat dump]tbl1  index %d ptr %d",sf_hnat_dip_crc_info[index].dip_crc_key[0],ptr_index);
		}else{
			ptr_index -= DIP_HASH2_INDEX_OFFSET;
			printk("[hnat dump]tbl2  index %d ptr %d",sf_hnat_dip_crc_info[index].dip_crc_key[1],ptr_index);
		}
	}

	return 0;
}


int sf_hnat_dip_print_all(struct sf_hnat_priv * phnat_priv, sf_dip_crc_info *sf_dip_info){
	int dip_index[8] = {0};
	int data[7] = {0};
	int i;
	int index = 0;

	u32 ipaddr;
	ipaddr = ntohl(sf_hnat_dip_entry[index].dip);

	sf_hnat_table_rd(phnat_priv, DIP_HASH1, sf_dip_info->dip_crc_key[0], data);
	dip_index[0] = data[0] & 0x1ff;
	dip_index[1] = (data[0] & (0x1ff << 9)) >> 9;
	dip_index[2] = (data[0] & (0x1ff << 18)) >> 18;
	dip_index[3] = (data[1] & 0xf) << 5 | (data[0] & (0x1f << 27)) >> 27;
	sf_hnat_table_rd(phnat_priv, DIP_HASH2, sf_dip_info->dip_crc_key[1], data);
	dip_index[4] = data[0] & 0x1ff;
	dip_index[5] = (data[0] & (0x1ff << 9)) >> 9;
	dip_index[6] = (data[0] & (0x1ff << 18)) >> 18;
	dip_index[7] = (data[1] & 0xf) << 5 | (data[0] & (0x1f << 27)) >> 27;

	printk("[hnat debug]////////////the conflict dip entries are:\n");
	for(i = 0; i < 8; i++){
		printk("[hnat debug]///////////the conflict num %d:\n", i);
		index = dip_index[i];
		printk("[hnat debug]dip:[%pI4h]\n", &ipaddr);
		printk("[hnat debug]vlan index %d\n", sf_hnat_dip_entry[index].vlan_index);
	}

	printk("[hnat debug]<<<<<<<<<<<<%s end   >>>>>>>>>>>>>>>\n", __FUNCTION__);
	return 0;
}

int sf_hnat_dip_add_crc_hash_tbl2(struct sf_hnat_priv *phnat_priv, unsigned short dip_ptr_index, unsigned short dip_index){
	int data[7] = {0};
	int valid = 0;
	int i = 0;

	int tab_no = DIP_HASH2;
	sf_hnat_table_rd(phnat_priv, tab_no, dip_ptr_index, data);
	valid = data[1] & 0xf0;

	for(; i < 4; i++  ){
		if(!(valid & (1 << (i + 4))))
		  break;
	}

	if(i < 4){
		if(i < 3){
			data[0] = data[0] & ~(0x1ff << (i*9));//clear the pointer first

			data[0] = data[0] | (dip_index << (i*9));
			data[1] = data[1] | (1 << (i + 4));
			sf_hnat_table_wr(phnat_priv, tab_no, dip_ptr_index, data);
		}
		if(i == 3){
			data[0] = data[0] & ~(0x1f << (i*9));//clear the pointer first
			data[1] = data[1] & ~0xf;

			data[0] = data[0] | ((dip_index & 0x1f) << (i*9));
			data[1] = data[1] | (1 << (i + 4)) | ((dip_index & 0x1e0) >> 5);
			sf_hnat_table_wr(phnat_priv, tab_no, dip_ptr_index, data);
		}
	}else
	  return -1;

	return i;
}

int sf_hnat_dip_add_crc_hash_tbl1(struct sf_hnat_priv *phnat_priv,  unsigned short dip_ptr_index, unsigned short dip_index){
	int data[7] = {0};
	int valid = 0;
	int i = 0;
	int tab_no = DIP_HASH1;

	sf_hnat_table_rd(phnat_priv, tab_no, dip_ptr_index, data);
	valid = data[1] & 0xf0;

	for(; i < 4; i++){
		//RM#9392 skip  ptr2
		if(i == 2)
		  continue;
		if(!(valid & (1 << (i + 4))))
		  break;
	}

	if(i < 4){
		if(i < 2){
			data[0] = data[0] & ~(0x1ff << (i*9));//clear the pointer first

			data[0] = data[0] | (dip_index << (i*9));
			data[1] = data[1] | (1 << (i + 4));
			sf_hnat_table_wr(phnat_priv, tab_no, dip_ptr_index, data);
		}
		else if(i == 3){
			data[0] = data[0] & ~(0x1f << (i*9));//clear the pointer first
			data[1] = data[1] & ~0xf;

			data[0] = data[0] | ((dip_index & 0x1f) << (i*9));
			data[1] = data[1] | (1 << (i + 4)) | ((dip_index & 0x1e0) >> 5);
			sf_hnat_table_wr(phnat_priv, tab_no, dip_ptr_index, data);
		}
		else{
			printk("[hnat error] DIP HASH1 err ptr %d set here\n", i);
			return -1;
		}
	}else
	  return -1;

	return i;
}

int sf_hnat_dip_del_crc_hash_entry(struct sf_hnat_priv *phnat_priv, sf_dip_crc_info *sf_dip_info){
	unsigned char index = sf_dip_info->dip_ptr_index;
	int data[7] = {0};
	if(index < DIP_HASH2_INDEX_OFFSET){
		sf_hnat_table_rd(phnat_priv, DIP_HASH1, sf_dip_info->dip_crc_key[0], data);
		if(data[1] & (1 << (index + 4)))
		  data[1] = data[1] & ~(1 << (index + 4));
		else
		  printk("[hnat error]dip tb1 hash idx %d ptr %d delete is not valid\n", sf_dip_info->dip_crc_key[0],index);

		sf_hnat_table_wr(phnat_priv, DIP_HASH1, sf_dip_info->dip_crc_key[0], data);
		if(g_print & DIP_HASH_DEBUG)
		  printk("[hnat notice]dip tb1 hash idx %d ptr %d delete\n", sf_dip_info->dip_crc_key[0],index);
	}//dip hash1 is valid, so we delete dip hash1
	else{
		sf_hnat_table_rd(phnat_priv, DIP_HASH2, sf_dip_info->dip_crc_key[1], data);

		if(data[1] & (1 << (index + 4 - DIP_HASH2_INDEX_OFFSET)))
		  data[1] = data[1] & ~(1 << (index + 4 - DIP_HASH2_INDEX_OFFSET));
		else
		  printk("[hnat error]dip tb2 hash idx %d ptr %d delete is not valid\n", sf_dip_info->dip_crc_key[1],index);

		sf_hnat_table_wr(phnat_priv, DIP_HASH2, sf_dip_info->dip_crc_key[1], data);
		if(g_print & DIP_HASH_DEBUG)
		  printk("[hnat notice]dip tb2 hash idx %d ptr %d delete\n", sf_dip_info->dip_crc_key[1],index);
	}//dip hash1 is invalid, so we delete dip hash2
	return 0;
}

int sf_hnat_add_dip_crc_hash_entry(struct sf_hnat_priv *phnat_priv, unsigned short dip_index, unsigned short crc_value){
	unsigned short *crc_key = sf_hnat_dip_crc_info[dip_index].dip_crc_key;
	int ret = -1;
	sf_dip_crc_info *sf_dip_info;
	sf_dip_info = &sf_hnat_dip_crc_info[dip_index];

	crc_key[0] = crc_value & 0xff;
	crc_key[1] = (crc_value & 0xfc00) >> 10;

	ret = sf_hnat_dip_add_crc_hash_tbl1(phnat_priv, crc_key[0], dip_index);//ret >= 0,config sucess,ret is index(0~3)
	sf_dip_info->dip_ptr_index = ret;
	if(ret >= 0){
		if(g_print & DIP_HASH_DEBUG)
		  printk("[hnat notice]dip tb1 hash idx %d ptr %d dip  0x%08x add success\n", crc_key[0],ret, sf_hnat_dip_entry[dip_index].dip);
		return ret;
	}

	ret = sf_hnat_dip_add_crc_hash_tbl2(phnat_priv, crc_key[1], dip_index);
	sf_dip_info->dip_ptr_index = ret + DIP_HASH2_INDEX_OFFSET;
	if(ret >= 0){
		if(g_print & DIP_HASH_DEBUG)
		  printk("[hnat notice]dip tb2 hash idx %d ptr %d dip 0x%08x add success\n", crc_key[1],ret, sf_hnat_dip_entry[dip_index].dip);
		return ret;
	}

	return -1;
}

void sf_hnat_del_dip_entry(struct sf_hnat_priv * phnat_priv, int dip_index){
	int data[5] = {0};
	sf_hnat_table_wr(phnat_priv, DIP, dip_index , data);
	return ;
}

void sf_hnat_del_dip(struct sf_hnat_priv * phnat_priv, int dip_index){
	if(sf_hnat_dip_entry[dip_index].ref_count > 0)
	  sf_hnat_dip_entry[dip_index].ref_count--;

	if(sf_hnat_dip_entry[dip_index].ref_count == 0){
		sf_hnat_dip_vld_cfg(phnat_priv, dip_index, 0);
		sf_hnat_del_dmac(phnat_priv, sf_hnat_dip_entry[dip_index].dmac_index);
		sf_hnat_del_dip_entry(phnat_priv, dip_index);
		sf_hnat_dip_del_crc_hash_entry(phnat_priv, &sf_hnat_dip_crc_info[dip_index]);
		sf_hnat_dip_entry[dip_index].valid = 0;
		phnat_priv->curr_dip_num--;
	}
}

//search type 1 add , 2 delete 0 just search
int sf_hnat_search_dip(struct sf_hnat_priv *phnat_priv, unsigned int ip, unsigned char vlan_index, unsigned char is_add) {
	unsigned short i = 0;
	for(;i < DIP_TABLE_MAX; i++){
		if (sf_hnat_dip_entry[i].valid == 1 ){
			if((sf_hnat_dip_entry[i].dip == ip) &&
						(sf_hnat_dip_entry[i].vlan_index == vlan_index)) {
				if(is_add == 0){
					sf_hnat_del_dip(phnat_priv, i);
				}
				else if(is_add == 1)
				  sf_hnat_dip_entry[i].ref_count++;

				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d, ref_count %d\n", __FUNCTION__,i, sf_hnat_dip_entry[i].ref_count);
				return i;
			}
		}
	}
	return -1;
}

void sf_hnat_add_dip_entry(struct sf_hnat_priv * phnat_priv, unsigned short dip_index){
	int data[5] = {0};
	memset(data,0,sizeof(data));
	// init dip table, only init index table
	data[0] = sf_hnat_dip_entry[dip_index].dip;
	data[1] = (sf_hnat_dip_entry[dip_index].vlan_index << 7)| sf_hnat_dip_entry[dip_index].dmac_index; //bit 0~31 dip, 32~38 dmac index, 39~45 vlan index
	sf_hnat_table_wr(phnat_priv, DIP, dip_index, data);
	return;
}


//sip/vlan is phnat_private ip/vlan, dip/vlan is public ip/vlan
short sf_hnat_add_dip(struct sf_hnat_priv * phnat_priv, struct sf_hashkey *sf_key,  unsigned char is_sip) {
	unsigned short crc_value;
	unsigned char crc_data[5] = {0};
	unsigned short crc_key[3] = {0};
	unsigned char * op_mac = NULL;
	unsigned char * router_mac = NULL;
	unsigned char vlan_index;
	unsigned short dip_index = -1;
	short dmac_index = -1;
	short ret = -1;
	unsigned short i = 0;
	unsigned int op_ip = 0;
	unsigned char op_vlan_index = 0;

	if(is_sip){
	  op_ip = sf_key->sip;
	  op_vlan_index = sf_key->src_vlan_index;
	}
	else{
		if(phnat_priv->snat_arp_force_dip == 0 && sf_key->is_dip_rt_ip_same_subnet == 0){//when use auto judge, dip and rt_ip in different segment
			op_ip = sf_key->router_pub_ip;
			op_vlan_index = sf_key->dest_vlan_index;
		}
		else{
			op_ip = sf_key->dip;
			op_vlan_index = sf_key->dest_vlan_index;
		}
	}

	ret = sf_hnat_search_dip(phnat_priv, op_ip, op_vlan_index, 1);
	if(ret >= 0){
		if(is_sip == 1 ){
			op_mac = sf_key->src_mac;
			router_mac = sf_key->router_src_mac;
		}
		else{
			op_mac = sf_key->dest_mac;
			router_mac = sf_key->router_dest_mac;
		}
		// check dip-dmac
		dmac_index = sf_hnat_dip_entry[ret].dmac_index;

		if(g_print & ARP_DEBUG)
		  printk("[hnat log] got ip 0x%08x mac %pM opmac %pM \n", op_ip, sf_hnat_dmac_entry[dmac_index].dmac, op_mac);

		if ((memcmp(sf_hnat_dmac_entry[dmac_index].dmac, op_mac, 6) != 0 )){
			printk("[hnat info]  arp change ip 0x%08x vlan %d mac change mac %pM new mac %pM\n",op_ip,sf_hnat_vlan_entry[op_vlan_index].vlan_id,  sf_hnat_dmac_entry[dmac_index].dmac,  op_mac);
			sf_hnat_del_dmac(phnat_priv, dmac_index);
			dmac_index = sf_hnat_add_dmac(phnat_priv, op_mac, router_mac);
			if(dmac_index < 0){
				sf_hnat_del_dip(phnat_priv,ret);
				printk("[hnat error] dip arp change fail add new  mac fail ret  %d\n",dmac_index);
				sf_hnat_dump_dip_by_index(phnat_priv, ret);
				return  -1;
			}

			sf_hnat_dip_entry[ret].dmac_index = dmac_index;
			//update to hw
			sf_hnat_add_dip_entry(phnat_priv, ret);
		}
		// TODO: check dmac-router mac maybe  multi  same mac  same ip on iff lan  is it really happen?
		return ret;
	}
	//search for valid dip index
	for(; i < DIP_TABLE_MAX; i++) {
		if(sf_hnat_dip_entry[i].valid == 0)
		  break;
	}
	if(i == DIP_TABLE_MAX){
		printk("[hnat error]cannot found a slot in DIP table\n");
		return -1;
	}else{
		dip_index = i;
	}

	if(is_sip == 1 ){
		sf_hnat_dip_entry[dip_index].dip = sf_key->sip;
		op_mac = sf_key->src_mac;
		vlan_index = sf_key->src_vlan_index;
		router_mac = sf_key->router_src_mac;
	}
	else{
		if(phnat_priv->snat_arp_force_dip == 0 && sf_key->is_dip_rt_ip_same_subnet == 0)//when use auto judge, dip and rt_ip in different segment
		  sf_hnat_dip_entry[dip_index].dip = sf_key->router_pub_ip;
		else
		  sf_hnat_dip_entry[dip_index].dip =  sf_key->dip;

		op_mac = sf_key->dest_mac;
		vlan_index = sf_key->dest_vlan_index;
		router_mac = sf_key->router_dest_mac;
	}

	sf_hnat_dip_entry[dip_index].vlan_index = vlan_index;

	// TODO: what to do if fail
	dmac_index = sf_hnat_add_dmac(phnat_priv, op_mac, router_mac);
	if(dmac_index < 0){
		goto err_add_dmac;
	}
	sf_hnat_dip_entry[dip_index].dmac_index = dmac_index;

	memcpy(crc_data, (unsigned char *)&sf_hnat_dip_entry[dip_index].dip, 4);
	memcpy(crc_data + 4, &vlan_index, 1);
	rcc_dip(crc_data);

	crc_value = sf_hnat_crc16(crc_data, 5, crc_key);

	ret = sf_hnat_add_dip_crc_hash_entry(phnat_priv, dip_index, crc_value);
	if(ret < 0){
		//TODO: what to do if fail
		u32 ipaddr;
		ipaddr = ntohl(sf_hnat_dip_entry[dip_index].dip);

		dip_hash_full_count++;
		if(g_print & DIP_HASH_DEBUG)
		  sf_hnat_dip_print_all(phnat_priv,&sf_hnat_dip_crc_info[dip_index]);
		if(g_print & DIP_HASH_DEBUG)
		  printk("[hnat error] add dip hash fail index = %d full count %d ip %pI4h:\n", dip_index, dip_hash_full_count, &ipaddr);
		goto err_add_dip_crc;
	}

	//	printk("calc dip crckey1 0x%x key 2 0x%x dip index %d\n", crc_key[0], crc_key[1], dip_index);
	// init dip valid entry, only init entry 0
	sf_hnat_add_dip_entry(phnat_priv, dip_index);

	sf_hnat_dip_vld_cfg(phnat_priv, dip_index, 1);

	sf_hnat_dip_entry[dip_index].valid = 1;
	sf_hnat_dip_entry[dip_index].ref_count = 1;
	phnat_priv->curr_dip_num++;
	//	printk("%s is_sip %d mac: %pM vlan_index %d router_mac %pM  dmac index %d curr_dip_num %d\n", __FUNCTION__,is_sip, op_mac, vlan_index,router_mac, dmac_index , phnat_priv->curr_dip_num);
	return dip_index;

err_add_dip_crc:
	sf_hnat_del_dmac(phnat_priv, dmac_index);
err_add_dmac:
	sf_hnat_dip_entry[dip_index].vlan_index = 0;

	return -1;
}

int sf_hnat_del_ppphd_entry(struct sf_hnat_priv *phnat_priv, unsigned char ppphd_index){

	int data[5] = {0};
	sf_hnat_table_wr(phnat_priv, PPPOE_HD, ppphd_index , data);
	return 0;
}

int sf_hnat_add_ppphd_entry(struct sf_hnat_priv *phnat_priv, unsigned char ppphd_index){
	int data[5] = {0};

	data[0] = 0x00000021;
	data[1] = 0x11000000 | sf_hnat_ppphd_entry[ppphd_index].ppphd_sid;
	sf_hnat_table_wr(phnat_priv, PPPOE_HD, ppphd_index , data);
	return 0;
}

void sf_hnat_del_ppphd(struct sf_hnat_priv *phnat_priv, unsigned char ppphd_index){

	if(sf_hnat_ppphd_entry[ppphd_index].ref_count > 0)
	  sf_hnat_ppphd_entry[ppphd_index].ref_count--;

	if((sf_hnat_ppphd_entry[ppphd_index].ref_count == 0) && (sf_hnat_ppphd_entry[ppphd_index].valid == 1)){
		sf_hnat_del_ppphd_entry(phnat_priv, ppphd_index);
		sf_hnat_ppphd_entry[ppphd_index].valid = 0;
		if(phnat_priv->curr_ppphd_num)
		  phnat_priv->curr_ppphd_num--;
	}
	return ;
}

int sf_hnat_search_ppphd(struct sf_hnat_priv *phnat_priv,unsigned short napt_index, unsigned char is_add){
	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];
	unsigned short i = 0;
	for(;i < PPPHD_TABLE_MAX; i++){
		if (sf_hnat_ppphd_entry[i].valid == 1){
			if (sf_hnat_ppphd_entry[i].ppphd_sid == sf_key->ppp_sid ){
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d\n", __FUNCTION__,i);

				if(is_add == 0){
					sf_hnat_del_ppphd(phnat_priv, i);
				}
				else if(is_add == 1)
				  sf_hnat_ppphd_entry[i].ref_count++;

				return i;
			}
		}
	}
	return -1;
}

char sf_hnat_add_ppphd(struct sf_hnat_priv *phnat_priv, unsigned short napt_index){

	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];
	unsigned char i = 0;
	char ret = -1;
	unsigned char ppphd_index;
	ret = sf_hnat_search_ppphd(phnat_priv,napt_index, 1);
	if(ret < 0){
		for(; i < PPPHD_TABLE_MAX; i++) {
			if(sf_hnat_ppphd_entry[i].valid == 0)
			  break;
		}
		if(i == PPPHD_TABLE_MAX){
			printk("[hnat error]cannot found a slot in ppphd table\n");
			return -1;
		}else{
			ppphd_index = i;
		}
	}
	else
	  return ret;

	sf_hnat_ppphd_entry[ppphd_index].ppphd_sid = sf_key->ppp_sid;
	sf_hnat_ppphd_entry[ppphd_index].valid = 1;
	sf_hnat_ppphd_entry[ppphd_index].ref_count = 1;

	sf_hnat_add_ppphd_entry(phnat_priv,ppphd_index);
	phnat_priv->curr_ppphd_num++;

	return ppphd_index;
}

int sf_hnat_dump_ppphd(struct sf_hnat_priv *phnat_priv){
	unsigned char i;
	for(i = 0; i < PPPHD_TABLE_MAX; i++){
		printk("[hnat info] index %d: sid %d,ref_count %d, valid %d, 0x%p\n", i, sf_hnat_ppphd_entry[i].ppphd_sid, sf_hnat_ppphd_entry[i].ref_count, sf_hnat_ppphd_entry[i].valid, &sf_hnat_ppphd_entry[i]);
	}
	printk("\n[hnat info] curr_ppphd_num = %d\n", phnat_priv->curr_ppphd_num);
	return 0;
}

int sf_hnat_add_rt_pub_net_entry(struct sf_hnat_priv *phnat_priv, unsigned char rt_pub_net_index){
	int data[5] = {0};

	data[0] = sf_hnat_rt_pub_net_entry[rt_pub_net_index].router_pub_ip;
	data[1] = sf_hnat_rt_pub_net_entry[rt_pub_net_index].rt_ip_mask | (sf_hnat_rt_pub_net_entry[rt_pub_net_index].pub_vlan_index << 5);//set default rt_ip_mask 255.255.255.0 = 24
	sf_hnat_table_wr(phnat_priv, RT_PUB_NET, rt_pub_net_index, data);
	return 0;
}

int sf_hnat_del_rt_pub_net_entry(struct sf_hnat_priv *phnat_priv, unsigned char rt_pub_net_index){
	int data[5] = {0};

	sf_hnat_table_wr(phnat_priv, RT_PUB_NET, rt_pub_net_index, data);
	return 0;
}

void sf_hnat_del_rt_pub_net(struct sf_hnat_priv* phnat_priv, unsigned char rt_pub_net_index){

	if(sf_hnat_rt_pub_net_entry[rt_pub_net_index].ref_count > 0)
	  sf_hnat_rt_pub_net_entry[rt_pub_net_index].ref_count--;

	if(sf_hnat_rt_pub_net_entry[rt_pub_net_index].ref_count == 0){
		sf_hnat_rt_pub_net_entry[rt_pub_net_index].valid = 0;
		phnat_priv->curr_rt_pub_net_num--;
		sf_hnat_del_rt_pub_net_entry(phnat_priv, rt_pub_net_index);
		if(g_print & NAPT_DEBUG)
		  printk("[hnat notice]%s del success index %d\n", __FUNCTION__,rt_pub_net_index);
	}
	return ;
}

int sf_hnat_search_rt_pub_net(struct sf_hnat_priv* phnat_priv, unsigned char pub_vlan_index, unsigned short napt_index, unsigned char is_add){
	unsigned short i = 0;
	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];;
	for(;i < RT_PUB_NET_TABLE_MAX; i++){
		if(sf_hnat_rt_pub_net_entry[i].valid == 1){
			if ((sf_hnat_rt_pub_net_entry[i].pub_vlan_index == pub_vlan_index)
						&& (sf_hnat_rt_pub_net_entry[i].router_pub_ip ==  sf_key->router_pub_ip)){
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d\n", __FUNCTION__,i);
				if(is_add == 0)
				  sf_hnat_del_rt_pub_net(phnat_priv, i);
				else if (is_add == 1)
				  sf_hnat_rt_pub_net_entry[i].ref_count++;
				return i;
			}
		}
	}
	return -1;
}

char sf_hnat_add_rt_pub_net(struct sf_hnat_priv* phnat_priv, unsigned char pub_vlan_index, unsigned short napt_index ){
	unsigned int i = 0;
	unsigned char rt_pub_net_index = 0;
	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];
	char ret = sf_hnat_search_rt_pub_net(phnat_priv, pub_vlan_index, napt_index, 1);

	if(ret < 0){
		for(; i < RT_PUB_NET_TABLE_MAX; i++) {
			if(sf_hnat_rt_pub_net_entry[i].valid == 0)
			  break;
		}

		if(i == RT_PUB_NET_TABLE_MAX){
			printk("[hnat error]cannot found a slot in ppphd table\n");
			return -1;
		}else{
			rt_pub_net_index = i;
		}
	}else
	  return ret;

	if(phnat_priv->wan_subnet[sf_key->wan_subnet_index].prefix_len >= 32) {
		sf_hnat_rt_pub_net_entry[rt_pub_net_index].rt_ip_mask = 31;
	} else {
		sf_hnat_rt_pub_net_entry[rt_pub_net_index].rt_ip_mask = phnat_priv->wan_subnet[sf_key->wan_subnet_index].prefix_len;
	}

	sf_hnat_rt_pub_net_entry[rt_pub_net_index].pub_vlan_index = pub_vlan_index;
	sf_hnat_rt_pub_net_entry[rt_pub_net_index].router_pub_ip = sf_key->router_pub_ip;
	sf_hnat_add_rt_pub_net_entry(phnat_priv, rt_pub_net_index);

	sf_hnat_rt_pub_net_entry[rt_pub_net_index].valid = 1;
	sf_hnat_rt_pub_net_entry[rt_pub_net_index].ref_count = 1;
	phnat_priv->curr_rt_pub_net_num++;

	return rt_pub_net_index;
}


int sf_hnat_add_vlan_entry(struct sf_hnat_priv* phnat_priv, unsigned char vlan_index){
	int data[5] = {0};

	data[0] = sf_hnat_vlan_entry[vlan_index].vlan_id;
	sf_hnat_table_wr(phnat_priv, VLAN_ID, vlan_index , data);
	return 0;
}

int sf_hnat_del_vlan_entry(struct sf_hnat_priv* phnat_priv, unsigned char vlan_index){
	int data[5] = {0};

	sf_hnat_table_wr(phnat_priv, VLAN_ID, vlan_index , data);
	return 0;
}

void sf_hnat_del_vlan(struct sf_hnat_priv* phnat_priv, unsigned char vlan_index){

	if(sf_hnat_vlan_entry[vlan_index].ref_count > 0)
	  sf_hnat_vlan_entry[vlan_index].ref_count--;

	if(sf_hnat_vlan_entry[vlan_index].ref_count == 0){
		sf_hnat_del_vlan_entry(phnat_priv, vlan_index);

		if(g_print & NAPT_DEBUG)
		  printk("[hnat notice]%s del success index %d vlan_id %d\n", __FUNCTION__,vlan_index,sf_hnat_vlan_entry[vlan_index].vlan_id );

		sf_hnat_vlan_entry[vlan_index].valid = 0;
		phnat_priv->curr_vlan_id_num--;
	}
	return ;
}

int sf_hnat_search_vlan(struct sf_hnat_priv* phnat_priv, unsigned short vlan_id, unsigned char is_add){
	unsigned short i = 0;
	for(;i < VLAN_TABLE_MAX; i++){
		if(sf_hnat_vlan_entry[i].valid == 1){
			if (sf_hnat_vlan_entry[i].vlan_id == vlan_id){

				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d is_add %d vlan_id %d\n", __FUNCTION__,i, is_add, vlan_id);

				if(is_add == 1)
				  sf_hnat_vlan_entry[i].ref_count++;
				else if (is_add == 0){
					sf_hnat_del_vlan(phnat_priv, i);
				}
				return i;
			}
		}
	}
	return -1;
}
// TODO: if full
short sf_hnat_add_vlan(struct sf_hnat_priv* phnat_priv, unsigned short vlan_id){
	short ret = -1;
	unsigned short vlan_index;
	unsigned char i = 0;

	if(vlan_id == 0)
	  return 0x7f;
	//vlan_index = 0x7f,if there is no vlan

	ret = sf_hnat_search_vlan(phnat_priv, vlan_id, 1);
	if(ret < 0){
		for(; i < VLAN_TABLE_MAX; i++) {
			if(sf_hnat_vlan_entry[i].valid == 0)
			  break;
		}
		if(i == VLAN_TABLE_MAX){
			printk("[hnat error]cannot found a slot in VLAN table\n");
			return -1;
		}else{
			vlan_index = i;
		}
	}else
	  return ret;

	sf_hnat_vlan_entry[vlan_index].vlan_id = vlan_id;
	sf_hnat_add_vlan_entry(phnat_priv,  vlan_index);
	sf_hnat_vlan_entry[vlan_index].valid = 1;
	sf_hnat_vlan_entry[vlan_index].ref_count = 1;
	phnat_priv->curr_vlan_id_num++;
	return vlan_index;
}

//the hash2 is different from hash 1/3
int sf_hnat_napt_add_crc_hash_table_2(struct sf_hnat_priv *phnat_priv, int tab_no, unsigned short hash_index, unsigned short napt_index){
	int data[7] = {0};
	int valid = 0;
	int i = 0;

	sf_hnat_table_rd(phnat_priv, tab_no, hash_index, data);
	valid = data[1] & 0xf00;

	for(; i < 4; i++  ){
		if(!(valid & (1 << (i + 8))))
		  break;
	}

	if(i < 4){

		// printk("[hnat debug]HASH add hash index %d valid %d table %d\n ", hash_index, i, tab_no);
		if(i < 3){
			data[0] = data[0] & ~(0x3ff << (i*10));//clear the pointer first in case multi use conflict

			data[0] = data[0] | (napt_index << (i*10)) ;
			data[1] = data[1] | (1 << (i + 8)) ;
			sf_hnat_table_wr(phnat_priv, tab_no, hash_index, data);
		}
		if(i == 3){
			data[0] = data[0] & ~(0x3 << (i*10));//clear the pointer first in case multi use conflict
			data[1] = data[1] & ~(0xff);

			data[0] = data[0] | ((napt_index & 0x3) << (i*10)) ;
			data[1] = data[1] | (1 << (i + 8)) | ((napt_index & 0x3fc) >> 2);
			sf_hnat_table_wr(phnat_priv, tab_no, hash_index, data);
		}
	}else
	  return -1;

	return i;
}

int sf_hnat_napt_add_crc_hash_table_13(struct sf_hnat_priv * phnat_priv, int tab_no, unsigned short hash_index, unsigned short napt_index){
	int data[7] = {0};
	int valid = 0;
	int i = 0;

	sf_hnat_table_rd(phnat_priv, tab_no, hash_index, data);
	valid = data[0] & 0x300000;

	for(; i < 2; i++  ){
		if(!(valid & (1 << (i + 20))))
		  break;
	}

	if(i < 2){
		data[0] = data[0] & ~(0x3ff << (i*10)); //clear the pointer first in case multi use conflict

		data[0] = data[0] | (napt_index << (i*10)) | (1 << (i + 20));

		// printk("[hnat debug]HASH hash idx %d valid idx %d tbl %d\n ", hash_index, i, tab_no);

		sf_hnat_table_wr(phnat_priv, tab_no, hash_index, data);
	}else
	  return -1;

	return i;
}

int sf_hnat_napt_common_add_crc_hash_entry(struct sf_hnat_priv *phnat_priv,sf_napt_crc_info *sf_napt_info, unsigned short napt_index, struct sf_hashkey *sf_key,  unsigned char is_inat){
	unsigned short* crc_key = NULL;
	int tab_no_start = 0;
	int ret = -1;

	if(is_inat == 0){
		crc_key = sf_napt_info->enat_crc_key;
		tab_no_start = ENAT_NAPT_HASH1;
	}
	if(is_inat){
		crc_key = sf_napt_info->inat_crc_key;
		tab_no_start = INAT_NAPT_HASH1;
	}

	sf_napt_key_crc_get(phnat_priv, sf_key, crc_key, is_inat);

	ret = sf_hnat_napt_add_crc_hash_table_13(phnat_priv, tab_no_start, crc_key[0], napt_index);//hash1
	if(ret >= 0){
		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d add tbl 1 index %d hash ptr %d\n",is_inat, crc_key[0], ret);
		return ret;//enat/inat add hash pointer sucess
	}

	ret = sf_hnat_napt_add_crc_hash_table_2(phnat_priv, (tab_no_start + 1), crc_key[1], napt_index);//hash2, it's bigger than 1/3, config first
	if(ret >= 0){
		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d add tbl 2 index %d hash ptr %d\n",is_inat, crc_key[1], ret);
		return (ret + NAPT_HASH2_INDEX_OFFSET );//enat/inat add hash pointer sucess
	}

	ret = sf_hnat_napt_add_crc_hash_table_13(phnat_priv, (tab_no_start + 2), crc_key[2], napt_index);//hash3
	if(ret >= 0){
		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d add tbl 3 index %d hash ptr %d\n",is_inat, crc_key[2], ret);
		return (ret + NAPT_HASH3_INDEX_OFFSET );//enat/inat add hash pointer sucess
	}

	return ret;
}

int sf_hnat_napt_common_del_crc_hash_entry(struct sf_hnat_priv *phnat_priv, unsigned short *crc_key, unsigned char index, unsigned char is_inat){
	int tab_no_start = 0;
	int tab_no = 0;
	int data[7] = {0};

	if(is_inat){
		tab_no_start = INAT_NAPT_HASH1;
	}
	if(is_inat == 0){
		tab_no_start = ENAT_NAPT_HASH1;
	}
	if(index < NAPT_HASH2_INDEX_OFFSET ){
		tab_no = tab_no_start;
		sf_hnat_table_rd(phnat_priv, tab_no, crc_key[0], data);
		if((data[0] & (1 << (20 + index)))  == 0){
			printk("[hnat error] is_inat %d crc tbl 1 del index %d ptr %d not valid\n",is_inat, crc_key[0], index);
			return 0;
		}
		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d del tbl 1 index %d hash ptr %d\n",is_inat, crc_key[0], index);

		// printk("[hnat debug]HASH1 del hash index %d not valid %d table %d\n ", crc_key[0], index, tab_no);
		data[0] = data[0] & ~(1 << (20 + index)) ;
		sf_hnat_table_wr(phnat_priv, tab_no, crc_key[0], data);
	}//del pointer in hash1
	if(index >=NAPT_HASH2_INDEX_OFFSET  && index < NAPT_HASH3_INDEX_OFFSET ){
		tab_no = tab_no_start + 1;
		sf_hnat_table_rd(phnat_priv, tab_no, crc_key[1], data);

		if((data[1] & (1 << (8 + index - NAPT_HASH2_INDEX_OFFSET)))  == 0 ){
			printk("[hnat error] is_inat %d crc tbl 2 del index %d ptr %d not valid\n",is_inat,  crc_key[1], index - NAPT_HASH2_INDEX_OFFSET);
			return 0;
		}
		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d del tbl 2 index %d hash ptr %d\n",is_inat, crc_key[1], index - NAPT_HASH2_INDEX_OFFSET);

		data[1] = data[1] & ~(1 << (8 + index - NAPT_HASH2_INDEX_OFFSET)) ;
		// printk("[hnat debug]HASH2 del hash index %d not valid %d  table %d\n ", crc_key[1], index, tab_no);
		sf_hnat_table_wr(phnat_priv, tab_no, crc_key[1], data);
	}//del pointer in hash2
	if(index >=  NAPT_HASH3_INDEX_OFFSET ){
		tab_no = tab_no_start + 2;
		sf_hnat_table_rd(phnat_priv, tab_no, crc_key[2], data);
		if((data[0] & (1 << (20 + index - NAPT_HASH3_INDEX_OFFSET )))  == 0){
			printk("[hnat error]is_inat %dcrc tbl 3 del  index %d ptr %d not valid\n",is_inat, crc_key[2], index- NAPT_HASH3_INDEX_OFFSET);
			return 0;
		}

		if(g_print & HASH_DEBUG)
		  printk("[hnat info] is_inat %d del tbl 3 index %d hash ptr %d\n",is_inat, crc_key[2], index - NAPT_HASH3_INDEX_OFFSET);

		data[0] = data[0] & ~(1 << (20 + index - NAPT_HASH3_INDEX_OFFSET )) ;
		// printk("[hnat debug]HASH2 del hash index %d not valid %d  table %d\n ", crc_key[1], index, tab_no);
		sf_hnat_table_wr(phnat_priv, tab_no, crc_key[2], data);
	}//del pointer in hash3
	return 0;
}

int sf_hnat_napt_del_crc_hash_entry(struct sf_hnat_priv *phnat_priv, sf_napt_crc_info *sf_napt_info) {
	sf_hnat_napt_common_del_crc_hash_entry(phnat_priv, sf_napt_info->inat_crc_key, sf_napt_info->inat_ptr_index, 1);
	sf_hnat_napt_common_del_crc_hash_entry(phnat_priv, sf_napt_info->enat_crc_key, sf_napt_info->enat_ptr_index, 0);
	return 0;
}

int sf_hnat_napt_add_crc_hash_entry(struct sf_hnat_priv *phnat_priv, sf_napt_crc_info *sf_napt_info, unsigned short napt_index, struct sf_hashkey *sf_key ) {
	int ret = 0;
	int value = 0;

	sf_napt_info->conflict_flag = 0;
	sf_napt_info->inat_ptr_index = 0;
	sf_napt_info->enat_ptr_index = 0;

	ret = sf_hnat_napt_common_add_crc_hash_entry(phnat_priv, sf_napt_info, napt_index, sf_key, 1);//ret = 0,config sucess
	if(ret < 0){
		sf_napt_info->conflict_flag |= 0x1;
		value = -1;
	} else
	  sf_napt_info->inat_ptr_index = ret;

	ret = sf_hnat_napt_common_add_crc_hash_entry(phnat_priv, sf_napt_info, napt_index, sf_key, 0);//ret = 0,config sucess, we need both enat and inat for entry
	if(ret < 0){
		sf_napt_info->conflict_flag |= 0x2;
		value = -1;
	}else
	  sf_napt_info->enat_ptr_index = ret;


	if(value < 0){
		if(g_print & HASH_DEBUG)
		  printk("[hnat notice] remove crc entry for add fail 0x%x napt %d", sf_napt_info->conflict_flag, napt_index);

		if(!(sf_napt_info->conflict_flag & 0x1))
		  sf_hnat_napt_common_del_crc_hash_entry(phnat_priv, sf_napt_info->inat_crc_key, sf_napt_info->inat_ptr_index, 1);

		if(!(sf_napt_info->conflict_flag & 0x2))
		  sf_hnat_napt_common_del_crc_hash_entry(phnat_priv, sf_napt_info->enat_crc_key, sf_napt_info->enat_ptr_index, 0);
	}

	return  value;
}

void sf_hnat_napt_get_crc_napt_entry(struct sf_hnat_priv *phnat_priv, sf_napt_crc_info *sf_napt_info, unsigned int *napt_index, unsigned int * napt_index_valid, unsigned char is_inat){

	int data[7] = {0};
	int table_no_start = 0;
	unsigned short *crc_key;

	if(is_inat){
		table_no_start = INAT_NAPT_HASH1;
		crc_key = sf_napt_info->inat_crc_key;
	}else{
		table_no_start = ENAT_NAPT_HASH1;
		crc_key = sf_napt_info->enat_crc_key;
	}
	sf_hnat_table_rd(phnat_priv, table_no_start, crc_key[0], data);
	napt_index[0] = data[0] & 0x3ff;
	napt_index_valid[0] = data[0] & (1 << 20);
	napt_index[1] = (data[0] & (0x3ff << 10)) >> 10;
	napt_index_valid[1] = data[0] & (1 << 21);
	sf_hnat_table_rd(phnat_priv, table_no_start+1, crc_key[1], data);
	napt_index[2] = data[0] & 0x3ff;
	napt_index_valid[2] = data[1] & (1 << 8);
	napt_index[3] = (data[0] & (0x3ff << 10)) >> 10;
	napt_index_valid[3] = data[1] & (1 << 9);
	napt_index[4] = (data[0] & (0x3ff << 20)) >> 20;
	napt_index_valid[4] = data[1] & (1 << 10);
	napt_index[5] = (data[1] & 0xff) << 2 |  (data[0] & 0xc0000000) >> 30;
	napt_index_valid[5] = data[1] & (1 << 11);
	sf_hnat_table_rd(phnat_priv, table_no_start+2, crc_key[2], data);
	napt_index[6] = data[0] & 0x3ff;
	napt_index_valid[6] = data[0] & (1 << 20);
	napt_index[7] = (data[0] & (0x3ff << 10)) >> 10;
	napt_index_valid[7] = data[0] & (1 << 21);

	return;
}

// TODO: ageing when crc full  choose form 8 entry
int sf_hnat_napt_crc_hash_aging(struct sf_hnat_priv *phnat_priv, sf_napt_crc_info *sf_napt_info, unsigned char is_inat){
	unsigned int napt_index[8] = {0};
	unsigned int napt_index_valid[8] = {0};
	unsigned char i;
	unsigned int  index = 0;

	sf_hnat_napt_get_crc_napt_entry(phnat_priv, sf_napt_info, napt_index,napt_index_valid, is_inat);
	// aging 2 entry to software offload
	for(i = 0; i < 2; i++){
		index = napt_index[i];
		if(napt_index_valid[i]){
			if(sf_hnat_napt_key[index].valid){
				// clean flow
				sf_hnat_delete_entry_by_index_unlock(phnat_priv, index);
				sf_hnat_clean_flow(sf_hnat_napt_key[index].pkey_flow);
				memset(&sf_hnat_napt_key[index], 0, sizeof(struct sf_hashkey));
			}else{
				if(is_inat){
					printk("[hnat error] aging inat tb1 index %d ptr %d valid but napt %d is invalid\n", sf_napt_info->inat_crc_key[0], i, napt_index[i]);
				}
				else{
					printk("[hnat error] aging enat tb1 index %d ptr %d valid but napt %d is invalid\n", sf_napt_info->enat_crc_key[0], i, napt_index[i]);
				}
			}
		}
	}
	return 0;
}
int sf_hnat_napt_print_crc(struct sf_hnat_priv *phnat_priv, sf_napt_crc_info *sf_napt_info, unsigned char is_inat){
	unsigned int napt_index[8] = {0};
	unsigned int napt_index_valid[8] = {0};
	unsigned short *crc_key;
	unsigned short *entry_crc_key[8];
	unsigned int entry_napt_index[8] = {0};
	unsigned char i;
	unsigned int index = 0;
	if(is_inat)
	  crc_key = sf_napt_info->inat_crc_key;
	else
	  crc_key = sf_napt_info->enat_crc_key;

	sf_hnat_napt_get_crc_napt_entry(phnat_priv, sf_napt_info, napt_index,napt_index_valid,is_inat);

	for(i = 0; i < 8; i++){
		index = napt_index[i];
		if(is_inat){
			entry_crc_key[i] = sf_hnat_napt_crc_info[index].inat_crc_key;
			entry_napt_index[i] = sf_hnat_napt_crc_info[index].inat_ptr_index;
		}else {
			entry_crc_key[i] = sf_hnat_napt_crc_info[index].enat_crc_key;
			entry_napt_index[i] = sf_hnat_napt_crc_info[index].enat_ptr_index;
		}
	}

	printk("[hnat debug]###### the conflict napt is_inat %d  ###########\n", is_inat);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[0]]);
	printk("TB1 crc index %d ptr 0 soft [napt %d, vld 0x%x crc %d ptr index %d]\n",crc_key[0], napt_index[0], napt_index_valid[0], entry_crc_key[0][0], entry_napt_index[0]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[1]]);
	printk("TB1 crc index %d ptr 1 soft [napt %d, vld 0x%x crc %d ptr index %d]\n", crc_key[0], napt_index[1],  napt_index_valid[1], entry_crc_key[1][0], entry_napt_index[1]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[2]]);
	printk("TB2 crc index %d ptr 0 soft [napt %d, vld 0x%x crc %d ptr index %d]\n",crc_key[1], napt_index[2],  napt_index_valid[2], entry_crc_key[2][1], entry_napt_index[2]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[3]]);
	printk("TB2 crc index %d ptr 1 soft [napt %d, vld 0x%x crc %d ptr index %d]\n", crc_key[1], napt_index[3],  napt_index_valid[3], entry_crc_key[3][1], entry_napt_index[3]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[4]]);
	printk("TB2 crc index %d ptr 2 soft [napt %d, vld 0x%x crc %d ptr index %d]\n",crc_key[1], napt_index[4],  napt_index_valid[4], entry_crc_key[4][1], entry_napt_index[4]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[5]]);
	printk("TB2 crc index %d ptr 3 soft [napt %d, vld 0x%x crc %d ptr index %d]\n", crc_key[1], napt_index[5],  napt_index_valid[5], entry_crc_key[5][1], entry_napt_index[5]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[6]]);
	printk("TB3 crc index %d ptr 0 soft [napt %d, vld 0x%x crc %d ptr index %d]\n",crc_key[2], napt_index[6],  napt_index_valid[6], entry_crc_key[6][2], entry_napt_index[6]);

	sf_hnat_dump_napt_key_hash_debug(&sf_hnat_napt_key[napt_index[7]]);
	printk("TB3 crc index %d ptr 1 soft [napt %d, vld 0x%x crc %d ptr index %d]\n", crc_key[2], napt_index[7],  napt_index_valid[7], entry_crc_key[7][2], entry_napt_index[7]);

	printk("[hnat debug]<<<<<<<<<<<<%s end   >>>>>>>>>>>>>>>\n", __FUNCTION__);
	return 0;
}

void sf_hnat_del_napt_entry( struct sf_hnat_priv* phnat_priv, unsigned short napt_index) {
	int data[5] = {0};
	sf_hnat_table_wr(phnat_priv, NAPT, napt_index , data);
}

void sf_hnat_add_napt_entry( struct sf_hnat_priv* phnat_priv, unsigned short napt_index) {
	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];
	int data[5] = {0};
	data[0] = sf_key->dip;
	data[1] = ((sf_key->sip & 0xFFFF) << 16) | sf_key->dport;
	data[2] = (sf_key->sport << 16) | ((sf_key->sip >> 16 ) & 0xFFFF);
	if(sf_key->cur_pppoe_en){
		data[3] = (sf_key->ppphd_index & 0x3) << 30 | (sf_key->cur_pppoe_en << 29) | (sf_key->proto << 27) | (sf_key->src_vlan_index << 20) | (sf_key->router_port << 4) | sf_key->rt_pub_net_index | (sf_key->dnat_to_host << 28);
		data[4] = (sf_key->ppphd_index & 0x4) >> 2;
	}else{
		data[3] = (sf_key->proto << 27) | (sf_key->src_vlan_index << 20) | ((sf_key->router_port) << 4) | sf_key->rt_pub_net_index | (sf_key->dnat_to_host << 28);
	}

	sf_hnat_table_wr(phnat_priv, NAPT, napt_index , data);
	return;
}


void sf_hnat_del_napt(struct sf_hnat_priv *phnat_priv, unsigned short napt_index) {

	struct sf_hashkey *sf_key = &sf_hnat_napt_key[napt_index];

	sf_key->valid = 0;
	sf_hnat_napt_vld_cfg(phnat_priv, napt_index, 0);



	sf_hnat_napt_del_crc_hash_entry(phnat_priv, &sf_hnat_napt_crc_info[napt_index]);

	sf_hnat_del_vlan(phnat_priv, sf_key->src_vlan_index);
	sf_hnat_del_vlan(phnat_priv, sf_key->dest_vlan_index);
	sf_hnat_del_rt_pub_net(phnat_priv, sf_key->rt_pub_net_index);

	if(sf_key->cur_pppoe_en){
		sf_hnat_del_ppphd(phnat_priv, sf_key->ppphd_index);
	}
	// for hash debug
	sf_hnat_del_napt_entry(phnat_priv, napt_index);

	if (g_print & (HASH_DEBUG|NAPT_DEBUG)){
		printk("del napt  %d valid %d\n", napt_index, sf_key->valid);
	}

	return ;
}

int sf_hnat_search_napt(struct sf_hashkey *sf_key) {
	unsigned short i = 0;
	for(;i < NAPT_TABLE_MAX; i++){
		if (sf_hnat_napt_key[i].valid == 1 ){
			if (memcmp(&sf_hnat_napt_key[i], sf_key, 49) == 0 ) {
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d\n", __FUNCTION__,i);
				return i;
			}
		}
	}
	return -1;
}

int sf_hnat_search_napt_by_pflow(struct flow_offload *pkey_flow) {
	unsigned short i = 0;
	for(;i < NAPT_TABLE_MAX; i++){
		if (sf_hnat_napt_key[i].valid == 1 ){
			if( pkey_flow == sf_hnat_napt_key[i].pkey_flow) {
				if(g_print & NAPT_DEBUG)
				  printk("[hnat notice]%s search success index %d\n", __FUNCTION__,i);
				return i;
			}
		}
	}
	return -1;
}

// sip is lan dip is wan
int sf_hnat_add_napt( struct sf_hnat_priv* phnat_priv, struct sf_hashkey *sf_src_key) {
	int ret = -1;
	unsigned short napt_index;
	short src_vlan_index = 0;//private vlan
	short dest_vlan_index = 0;//public vlan
	char rt_pub_net_index = 0;
	char ppphd_index = 0;
	unsigned short i = 0;
	struct sf_hashkey *sf_key;
	sf_napt_crc_info *sf_napt_info;

	//search for valid napt index
	for(; i < NAPT_TABLE_MAX; i++) {
		if(sf_hnat_napt_key[i].valid == 0)
		  break;
	}
	if(i == NAPT_TABLE_MAX){
		napt_full_calculate++;
		//printk("[hnat error] can not find a slot in napt \n");
		return -1;
	}else{
		napt_index = i;
	}

	sf_key = &sf_hnat_napt_key[napt_index];
	memcpy(sf_key, sf_src_key, sizeof(struct sf_hashkey));

	src_vlan_index = sf_hnat_add_vlan(phnat_priv, sf_key->src_vlan);
	if(src_vlan_index < 0 ){
		printk("[hnat error] src vlan_add fail %d\n", sf_key->src_vlan);
		goto err_src_vlan;
	}

	sf_key->src_vlan_index = src_vlan_index;

	dest_vlan_index = sf_hnat_add_vlan(phnat_priv, sf_key->dest_vlan);
	if(dest_vlan_index < 0 ){
		printk("[hnat error]dest vlan_add fail %d\n",  sf_key->dest_vlan);
		goto err_dest_vlan;

	}
	sf_key->dest_vlan_index = dest_vlan_index;

	rt_pub_net_index = sf_hnat_add_rt_pub_net(phnat_priv, dest_vlan_index, napt_index);
	if(rt_pub_net_index  < 0 ){
		u32 ipaddr;
		ipaddr = ntohl(sf_key->router_pub_ip);
		printk("[hnat error]rt pub net add fail %pI4h \n",  &ipaddr);
		goto err_rt_pub;
	}

	sf_key->rt_pub_net_index = rt_pub_net_index;
	// init napt table, only init index table

	/* bit 0~3 rt_pub_net_index, bit 4~19 router public port, bit 20~26 phnat_private vlan index, bit 27 proto tcp/udp, bit 28 INAT to host, bit 29 PPPOE enable,bit 30\31\data[4]bit0 PPPOE index */


	if(sf_key->cur_pppoe_en){
		ppphd_index = sf_hnat_add_ppphd(phnat_priv, napt_index);
		if(ppphd_index < 0 ){
			printk("[hnat error]ppphd add fail %d\n",  sf_key->dest_vlan);
			goto err_ppphd;
		}
		if(g_print & NAPT_DEBUG)
		  printk("[hnat notice]ppphd index %d\n", ppphd_index);
		sf_key->ppphd_index = ppphd_index;
	}

	sf_napt_info = &sf_hnat_napt_crc_info[napt_index];  //record the hash information for delete
	ret = sf_hnat_napt_add_crc_hash_entry(phnat_priv, sf_napt_info, napt_index, sf_key );//0 sucess , -1 fail
	if(ret < 0){
		napt_hash_full_count++;
		if(g_print & HASH_DEBUG){
			printk("[hnat notice]add crc fail napt num %d napt index = %d full count %d\n", phnat_priv->curr_napt_num, napt_index, napt_hash_full_count);
			sf_hnat_dump_napt_key_hash_debug(sf_key);
		}
		goto err_crc_hash;

		if(sf_napt_info->conflict_flag & 0x1){

			if(g_print & HASH_DEBUG)
			  sf_hnat_napt_print_crc(phnat_priv, sf_napt_info, 1);

			sf_hnat_napt_crc_hash_aging(phnat_priv, sf_napt_info, 1);
		}
		if(sf_napt_info->conflict_flag & 0x2){
			if(g_print & HASH_DEBUG)
			  sf_hnat_napt_print_crc(phnat_priv, sf_napt_info, 0);

			sf_hnat_napt_crc_hash_aging(phnat_priv, sf_napt_info, 0);
		}

		ret = sf_hnat_napt_add_crc_hash_entry(phnat_priv, sf_napt_info, napt_index, sf_key );//0 sucess , -1 fail
		if(ret < 0){
			printk("[hnat error]  add crc hash fail after ageing napt%d\n", napt_index);
		}
	}
	// for hash debug

	sf_hnat_add_napt_entry(phnat_priv, napt_index);

	sf_hnat_napt_vld_cfg(phnat_priv, napt_index, 1);
	sf_key->valid = 1;


	if (g_print & (HASH_DEBUG|NAPT_DEBUG)){
		printk("add napt %d valid %d\n", napt_index, sf_key->valid);
	}

	return napt_index;

err_crc_hash:
err_ppphd:
	sf_hnat_del_rt_pub_net(phnat_priv, rt_pub_net_index);

err_rt_pub:
	sf_hnat_del_vlan(phnat_priv, dest_vlan_index);

err_dest_vlan:
	sf_hnat_del_vlan(phnat_priv, src_vlan_index);

err_src_vlan:
	memset(sf_key, 0, sizeof(struct sf_hashkey));

	return -1;
}

char sf_hnat_delete_entry_by_index_unlock(struct sf_hnat_priv* phnat_priv, unsigned int napt_index){

	unsigned char proto = 0;
	if(napt_index >= NAPT_TABLE_MAX){
		printk("[hnat error] error napt index %d line %d\n",  napt_index, __LINE__);
		return -1;//no such entry
	}

	if(!sf_hnat_napt_key[napt_index].valid){
		printk("[hnat error] unlock del napt index  %d valid %d\n",  napt_index, sf_hnat_napt_key[napt_index].valid);
		return -1;
	}

	if(g_print & (NAPT_DEBUG|HASH_DEBUG))
	  printk("[hnat log]  del napt index  %d\n",  napt_index);

	proto = sf_hnat_napt_key[napt_index].proto;
	sf_hnat_del_napt(phnat_priv,  napt_index);
	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].src_dip_index);
	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].dest_dip_index);

	if(phnat_priv->curr_napt_num == 0)
	  printk("[hnat error] tcp num  is  0\n");
	else
	  phnat_priv->curr_napt_num--;

	if(proto == 0){
		if(phnat_priv->curr_napt_tcp_num == 0)
		  printk("[hnat error] tcp num  is  0\n");
		else
		  phnat_priv->curr_napt_tcp_num--;
	}
	else{
		if(phnat_priv->curr_napt_udp_num == 0)
		  printk("[hnat error] udp num  is  0\n");
		else
		  phnat_priv->curr_napt_udp_num--;
	}

	return 0;
}
int sf_hnat_delete_entry_by_index(struct sf_hnat_priv* phnat_priv, unsigned int napt_index){

	unsigned char proto = 0;
	if(napt_index >= NAPT_TABLE_MAX){
		printk("[hnat error] error napt index  %d\n",  napt_index);
		return -1;//no such entry
	}

	spin_lock_bh(&hnat_napt_lock);

	if(!sf_hnat_napt_key[napt_index].valid){
		//printk("[hnat error] error napt index  %d valid %d\n", napt_index, sf_hnat_napt_key[napt_index].valid);
		spin_unlock_bh(&hnat_napt_lock);
		return -1;
	}

	proto = sf_hnat_napt_key[napt_index].proto;
	sf_hnat_del_napt(phnat_priv,  napt_index);
	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].src_dip_index);
	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].dest_dip_index);
	sf_hnat_clean_flow(sf_hnat_napt_key[napt_index].pkey_flow);
	memset(&sf_hnat_napt_key[napt_index], 0, sizeof(struct sf_hashkey));

	if(phnat_priv->curr_napt_num == 0)
	  printk("[hnat error] tcp num  is  0\n");
	else
	  phnat_priv->curr_napt_num--;

	if(proto == 0){
		if(phnat_priv->curr_napt_tcp_num == 0)
		  printk("[hnat error] tcp num  is  0\n");
		else
		  phnat_priv->curr_napt_tcp_num--;
	}
	else{
		if(phnat_priv->curr_napt_udp_num == 0)
		  printk("[hnat error] udp num  is  0\n");
		else
		  phnat_priv->curr_napt_udp_num--;
	}

	spin_unlock_bh(&hnat_napt_lock);


	return napt_index;
}

int sf_hnat_delete_entry(struct sf_hnat_priv* phnat_priv, struct sf_hashkey *sf_key){
	int napt_index;
	napt_index = sf_hnat_search_napt(sf_key);
	if(napt_index < 0){
		sf_hnat_dump_napt_key(sf_key);
		printk("[hnat error] search fail delete fail\n");
		return -1;//no such entry
	}

	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].src_dip_index);
	sf_hnat_del_dip(phnat_priv, sf_hnat_napt_key[napt_index].dest_dip_index);

	sf_hnat_del_napt(phnat_priv,  napt_index);

	return 0;
}
void sf_hnat_flush_all(struct sf_hnat_priv* phnat_priv){
	spin_lock_bh(&hnat_napt_lock);

	sf_hnat_table_clean(phnat_priv);
	phnat_priv->curr_napt_num = 0;
	phnat_priv->curr_napt_tcp_num = 0;
	phnat_priv->curr_napt_udp_num =0;

	phnat_priv->curr_dip_num = 0;
	phnat_priv->curr_dmac_num = 0;
	phnat_priv->curr_rmac_num = 0;
	phnat_priv->curr_ppphd_num = 0;
	phnat_priv->curr_rt_pub_net_num = 0;
	phnat_priv->curr_vlan_id_num = 0;

	memset(sf_hnat_napt_key,		0,  NAPT_TABLE_MAX*sizeof(struct sf_hashkey) );
	memset(sf_hnat_dip_entry,		0,  DIP_TABLE_MAX*sizeof(sf_dip_entry) );
	memset(sf_hnat_dmac_entry,		0,  DMAC_TABLE_MAX*sizeof(sf_dmac_entry) );
	memset(sf_hnat_rmac_entry,		0,  ROUTER_MAC_TABLE_MAX*sizeof(sf_rmac_entry) );
	memset(sf_hnat_ppphd_entry,		0,  PPPHD_TABLE_MAX*sizeof(sf_ppphd_entry) );
	memset(sf_hnat_rt_pub_net_entry,0,  RT_PUB_NET_TABLE_MAX*sizeof(sf_rt_pub_net_entry) );
	memset(sf_hnat_vlan_entry,		0,  VLAN_TABLE_MAX*sizeof(sf_vlan_entry) );
	memset(sf_hnat_napt_crc_info,	0,  NAPT_TABLE_MAX *sizeof(sf_napt_crc_info) );
	memset(sf_hnat_dip_crc_info,	0,  DIP_TABLE_MAX *sizeof(sf_dip_crc_info) );

	spin_unlock_bh(&hnat_napt_lock);

	return ;
}

int sf_hnat_statistic_count_read(struct platform_device *pdev, unsigned char *mac)
{
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	int data[7] = {0};
	u32 i = 0, count = 0;

	for (i = 0; i < DMAC_TABLE_MAX; i++){
		if (sf_hnat_dmac_entry[i].valid != 1)
			continue;

		if (memcmp(sf_hnat_dmac_entry[i].dmac, mac, 6) == 0 ){
			sf_hnat_table_rd(phnat_priv, STATICS, i, data);
			count = data[0];
		}
	}
	return count;
}

// void sf_hnat_start_udp_aging(struct sf_hnat_priv* phnat_priv){
// 	unsigned short i = 0;
// 	for(;i < NAPT_TABLE_MAX; i++){
// 		if((sf_hnat_napt_key[i].valid == 1 ) &&( sf_hnat_napt_key[i].proto == 1 )){
// 			// if(check_pkey_flow_safe(sf_hnat_napt_key[i].pkey_flow, sf_hnat_napt_key[i].keep_flag)){
// 			if(sf_hnat_napt_key[i].pkey_flow){
// 				if(sf_hnat_napt_key[i].pkey_flow->flags & FLOW_OFFLOAD_KEEP){
// 					sf_hnat_napt_key[i].pkey_flow->flags &= ~(FLOW_OFFLOAD_KEEP);
// 					// printk("[hnat debug] ageing  flow  0x%p flags 0x%x flags 0x%p napt_index %d\n", sf_hnat_napt_key[i].pkey_flow,sf_hnat_napt_key[i].pkey_flow->flags, &sf_hnat_napt_key[i].pkey_flow->flags, i);
// 				}

// 			}
// 			// cc
// 		}
// 	}
// }
void sf_hnat_dump_flow(struct sf_hnat_priv * phnat_priv){
	unsigned short depth;
	for (depth = 0; depth < NAPT_TABLE_MAX; depth++){
		if(sf_hnat_napt_key[depth].valid ==  1){
			printk("[hnat debug]dump flow 0x%p flags 0x%lx flags 0x%p napt_index %d\n", sf_hnat_napt_key[depth].pkey_flow,sf_hnat_napt_key[depth].pkey_flow->flags, &sf_hnat_napt_key[depth].pkey_flow->flags, depth);
		}
	}
	return;
}
// fast clean all hnat hw entry  would cause flow offload status error
void _sf_hnat_napt_vld_clean(struct sf_hnat_priv * phnat_priv) {
	int data[HWNAT_TB_WRDATA_NO] = {0};
	unsigned short depth;

	// if(!is_safe)
	//   spin_lock_bh(&napt_offload_lock);

	//clean all
	if(phnat_priv->curr_napt_num > 0){

		phnat_priv->curr_napt_num = 0;
		phnat_priv->curr_napt_tcp_num = 0;
		phnat_priv->curr_napt_udp_num = 0;
		for (depth = 0; depth < 32; depth++){
			sf_hnat_table_wr(phnat_priv, NAPT_VLD, depth, data);
		}
		for (depth = 0; depth < NAPT_TABLE_MAX; depth++){
			if(sf_hnat_napt_key[depth].valid ==  1){
				sf_hnat_napt_key[depth].valid = 0;

				sf_hnat_clean_flow(sf_hnat_napt_key[depth].pkey_flow);

				// printk("[hnat debug]clean flow 0x%p flags 0x%x flags 0x%p napt_index %d\n", sf_hnat_napt_key[depth].pkey_flow,sf_hnat_napt_key[depth].pkey_flow->flags, &sf_hnat_napt_key[depth].pkey_flow->flags, depth);

				sf_hnat_napt_key[depth].pkey_flow = NULL;
				// sf_hnat_napt_key[depth].keep_flag = 0;

				if (g_print & NAPT_DEBUG){
					printk("vldclean delete napt  %d\n", depth);
				}
				sf_hnat_napt_del_crc_hash_entry(phnat_priv, &sf_hnat_napt_crc_info[depth]);
			}
		}
		if(phnat_priv->curr_rmac_num > 0){
			phnat_priv->curr_rmac_num = 0;
			for (depth = 0; depth < ROUTER_MAC_TABLE_MAX; depth++){
				sf_hnat_rmac_entry[depth].valid = 0;
			}
		}

		if(phnat_priv->curr_dmac_num > 0){
			phnat_priv->curr_dmac_num = 0;
			for (depth = 0; depth < DMAC_TABLE_MAX; depth++){
				sf_hnat_dmac_entry[depth].valid =  0;
			}
		}
		for (depth = 0; depth < 16; depth++){
			sf_hnat_table_wr(phnat_priv, DIP_VLD, depth, data);
		}

		if(phnat_priv->curr_dip_num > 0){
			phnat_priv->curr_dip_num = 0;
			for (depth = 0; depth < DIP_TABLE_MAX; depth++){
				if(sf_hnat_dip_entry[depth].valid == 1){
					sf_hnat_dip_entry[depth].valid = 0;
					sf_hnat_dip_del_crc_hash_entry(phnat_priv, &sf_hnat_dip_crc_info[depth]);
				}
			}
		}
		if(phnat_priv->curr_ppphd_num > 0){
			phnat_priv->curr_ppphd_num = 0;
			for (depth = 0; depth < PPPHD_TABLE_MAX; depth++){
				sf_hnat_ppphd_entry[depth].valid = 0;
			}
		}
		if(phnat_priv->curr_rt_pub_net_num > 0){
			phnat_priv->curr_rt_pub_net_num = 0;
			for (depth = 0; depth < RT_PUB_NET_TABLE_MAX ; depth++){
				sf_hnat_rt_pub_net_entry[depth].valid = 0;
			}
		}
		if(phnat_priv->curr_vlan_id_num > 0){
			phnat_priv->curr_vlan_id_num = 0;
			for (depth = 0; depth < VLAN_TABLE_MAX; depth++){
				sf_hnat_vlan_entry[depth].valid = 0;
			}
		}
	}
	// if(!is_safe)
	  // spin_unlock_bh(&napt_offload_lock);
	return;
}

void sf_hnat_napt_vld_clean(void)
{
	if (g_phnat_priv)
		_sf_hnat_napt_vld_clean(g_phnat_priv);
}
EXPORT_SYMBOL(sf_hnat_napt_vld_clean);
// #define  CONFIG_HNAT_FLUSH_FULL
// unsigned short flush_interval = 600; // 10min
// unsigned long last_flush_time = 0;
char sf_hnat_add_entry(struct sf_hnat_priv* phnat_priv, struct sf_hashkey *sf_key, unsigned int* napt_index) {
	short ret = -1;
	short src_dip_index = -1;
	short dest_dip_index = -1;

	spin_lock_bh(&hnat_napt_lock);
	ret = sf_hnat_search_napt(sf_key);
	if(ret >= 0){
		//if same napt with 2 flow ptr  set one free here
		if(sf_hnat_napt_key[ret].pkey_flow != sf_key->pkey_flow ){
			sf_hnat_clean_flow(sf_hnat_napt_key[ret].pkey_flow);
			sf_hnat_napt_key[ret].pkey_flow = sf_key->pkey_flow;
			if(g_print & COUNT_DEBUG)
			  printk("[hnat info] search napt success index %d update flow 0x%p to 0x%p\n",ret,sf_hnat_napt_key[ret].pkey_flow , sf_key->pkey_flow );
			update_flow_count++;
		}
		*napt_index = ret;
		spin_unlock_bh(&hnat_napt_lock);
		return 0;
	}

	ret = sf_hnat_add_napt(phnat_priv,  sf_key);
	if( ret < 0){
		// printk("[hnat error] error add ptr add napt fail\n");
		goto err_add_napt_fail;
	}
	*napt_index = ret;

	if(phnat_priv->snat_arp_force_dip == 0)//when use auto judge, we have to judge if dip and rt_ip are in the same network segment
	  sf_hnat_judge_dip_rt_ip(phnat_priv, *napt_index);

	src_dip_index =  sf_hnat_add_dip(phnat_priv, &sf_hnat_napt_key[*napt_index] , 1);
	if( src_dip_index < 0 ){
		printk("[hnat error]error add src ip fail\n");
		sf_hnat_dump_napt_key(sf_key);
		goto err_add_src_dip;
	}

	sf_hnat_napt_key[*napt_index].src_dip_index = src_dip_index;
	dest_dip_index = sf_hnat_add_dip(phnat_priv, &sf_hnat_napt_key[*napt_index], 0);
	if(dest_dip_index  < 0 ){
		printk("[hnat error]error add dest ip fail\n");
		sf_hnat_dump_napt_key(sf_key);
		goto err_add_dest_dip;
	}

	sf_hnat_napt_key[*napt_index].dest_dip_index = dest_dip_index;

	phnat_priv->curr_napt_num++;

	if(sf_key->proto == 0)
	  phnat_priv->curr_napt_tcp_num++;
	else
	  phnat_priv->curr_napt_udp_num++;

	if (phnat_priv->curr_napt_udp_num >= FLOWOFFLOAD_HW_UDP_THRES) {
		if ((jiffies - phnat_priv->last_age_time) > (60*HZ)) {
			nf_flow_need_udp_aging = true;
			phnat_priv->last_age_time = jiffies;
		}
	} else {
		nf_flow_need_udp_aging = false;
	}

	spin_unlock_bh(&hnat_napt_lock);

	return 1;
err_add_dest_dip:
	sf_hnat_del_dip(phnat_priv, src_dip_index);
err_add_src_dip:
	sf_hnat_del_napt(phnat_priv, *napt_index);
	sf_hnat_napt_key[*napt_index].pkey_flow = NULL;
err_add_napt_fail:
	spin_unlock_bh(&hnat_napt_lock);
	return -1;
}

int sf_hnat_change_nat_mode(struct sf_hnat_priv* phnat_priv, unsigned int hnat_mode){
	phnat_priv->hnat_mode = hnat_mode;
	hnat_writel(phnat_priv, 0x0040004f | phnat_priv->hnat_mode, HWNAT_REG1_CSR);
	return 0;
}

int sf_hnat_is_error(struct sf_hnat_priv *phnat_priv)
{
	u32 data = 0;

	data = hnat_readl(phnat_priv, HWNAT_REG2_CSR);
	data = data & 0xffffffef;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);
	data = data | 0x10;
	hnat_writel(phnat_priv, data,HWNAT_REG2_CSR);

	data = hnat_readl(phnat_priv, HWNAT_RX_ENTER_DROP_CNT);
	if (data > 0)
		return 1;

	return 0;
}

irqreturn_t sf_hnat_nohit_irq_handle(struct sf_hnat_priv* phnat_priv,int irq, void *dev_id){
	unsigned int fifo_data[16];
	int data;
	int i = 0;
	hnat_writel(phnat_priv, 0, HWNAT_REG5_CSR);
	printk("[hnat notice]%s mask irq\n",__FUNCTION__);

	do{
		data = hnat_readl(phnat_priv, HWNAT_REG48_CSR);
		fifo_data[i] = data;
		i++;

		data = hnat_readl(phnat_priv, HWNAT_REG49_CSR);
		fifo_data[i] = data;
		i++;
	}while(!(hnat_readl(phnat_priv, HWNAT_REG50_CSR) & 0x8));

	for(i = 0;i < 8;i++){
		printk("[hnat notice]data %d = 0x%x\n", i, fifo_data[i]);
	}

	hnat_writel(phnat_priv, 0x100, HWNAT_REG5_CSR);
	printk("[hnat notice]%s unmask irq\n",__FUNCTION__);

	return IRQ_HANDLED;
}

// copy  ip  port info for hnat search on frag ip pkt
unsigned char  frag_hdr[12];
#define HNAT_PADDING_LEN    60
#define IP_MF		0x2000		/* Flag: "More Fragments"	*/
#define IP_OFFSET	0x1FFF		/* "Fragment Offset" part	*/
// 1 is hit 0 is not hit
// ATTENTION: maybe need to push vlantag when muilt - phy-wan
unsigned char sf_hnat_search(struct platform_device *pdev, struct sk_buff *skb){
	unsigned char ret = 0;
	struct ethhdr * peth_hdr;
	struct iphdr* pip_hdr;
	struct udphdr* pudp_hdr;
	struct tcphdr* ptcp_hdr;
	unsigned char  l4_proto = 0;
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	u16 sport, dport;           /* Source and destination ports */
	u32 saddr, daddr;
	peth_hdr = (struct ethhdr *) (skb->head + skb->mac_header);

	if(peth_hdr->h_proto == htons(ETH_P_IP)){
		pip_hdr = (struct iphdr *) ((unsigned char*)peth_hdr  + sizeof(struct ethhdr));
		if(sf_hnat_is_lan_ip(phnat_priv, pip_hdr->daddr) >= 0){
			return 0;
		}else{
			l4_proto = pip_hdr->protocol;
			// check udp
			if(l4_proto == IPPROTO_UDP){
				pudp_hdr = (struct udphdr *)((unsigned char*)pip_hdr + sizeof(struct iphdr));
				// pkt is frag
				if ((pip_hdr->frag_off & htons(IP_MF | IP_OFFSET)) != 0) {
					//TODO: support multi  stream
					if((pip_hdr->frag_off & htons(IP_OFFSET)) == 0){
						memcpy(frag_hdr,peth_hdr + 26, 12);
					}
					ret = sf_hnat_table_fast_search(phnat_priv, 0, frag_hdr, 1);//ENAT, udp
				}
				else
				ret = sf_hnat_table_fast_search(phnat_priv, 0, (unsigned char*)peth_hdr + 26, 1);//ENAT, udp
				if(g_print & WDEV_DEBUG )
					printk("[hnat_debug]%s %d ret %d wifi search hit: %pI4 %pM -> %pI4 %pM from %s\n",
							__FUNCTION__,__LINE__, ret, &saddr, peth_hdr->h_source, &daddr,  peth_hdr->h_dest, skb->dev->name);
				// printk(">>>>search ret = %d\n",ret );
			}else if(l4_proto == IPPROTO_TCP){
				ptcp_hdr = (struct tcphdr *)((unsigned char*)pip_hdr + sizeof(struct iphdr));
				if(!(ptcp_hdr->fin || ptcp_hdr->syn || ptcp_hdr->rst)){
					ret = sf_hnat_table_fast_search(phnat_priv, 0, (unsigned char*)peth_hdr + 26, 0);//ENAT, tcp
					//RM#9076 fix pppoe  tcp ack delay
					if(ret == 2){
						if(skb->len < HNAT_PADDING_LEN)
						  skb_put(skb,(HNAT_PADDING_LEN - skb->len));
					}
				}
			}
		}
	}
	if(ret){
		if(g_print & WDEV_DEBUG ){
			saddr = ntohl(pip_hdr->saddr);
			daddr = ntohl(pip_hdr->daddr);

			if(l4_proto == IPPROTO_UDP){
				sport = ntohs(pudp_hdr->source);
				dport = ntohs(pudp_hdr->dest);
			}
			else{
				sport = ntohs(ptcp_hdr->source);
				dport = ntohs(ptcp_hdr->dest);
			}

			if ((pip_hdr->frag_off & htons(IP_MF | IP_OFFSET)) != 0) {
				printk("[hnat info] got  frag pkt and hit id %x offset %d\n", pip_hdr->id, (pip_hdr->frag_off & htons(IP_OFFSET)) << 3);
			}
			printk("[hnat_debug]%s %d wifi search hit: %pI4h:%d -> %pI4h:%d from %s\n", __FUNCTION__,__LINE__,&saddr, sport, &daddr, dport, skb->dev->name);
		}
		skb->cb[SF_HNAT_FLAG] = SF_CB_HNAT_FORWARD;
		*((unsigned int*)(skb->cb+SF_HNAT_DEV_OFFSET)) = (unsigned int)phnat_priv->driver_ndev;
		//printk("wifi p:%p cb:%02x cb1 %02x cb2 %02x cb3 %02x\n", phnat_priv->driver_ndev, *(skb->cb ), *(skb->cb + 1), *(skb->cb + 2), *(skb->cb + 3));
		if (g_print & WDEV_DEBUG)
			printk("%s %d dev %s\n", __func__, __LINE__, phnat_priv->driver_ndev->name);
	}
	return ret;
}

int sf_hnat_print_debug(enum flow_offload_type type, struct flow_offload *flow, struct flow_offload_hw_path *src, struct flow_offload_hw_path *dest) {
	printk("hnat offload %s type=%d flow 0x%p\n", (type == FLOW_OFFLOAD_ADD) ? "ADD":"DEL", type,flow );
	print_flow_offload(flow);
	printk("src=========\n");
	print_flow_offload_hw_path(src);
	printk("hnat offload %s type=%d \n", (type == FLOW_OFFLOAD_ADD) ? "ADD":"DEL", type );
	print_flow_offload_hw_path(dest);
	return 0;
}

#define DNS_PORT 53
static char   sf_hnat_check_ip_info(struct sf_hnat_priv* phnat_priv, struct flow_offload *flow, struct sf_hashkey *sf_key){

	u16 dport;           /* Source and destination ports */
	u32 saddr;
	char inlan;
	char inwan;

	if(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto == IPPROTO_TCP ){
		sf_key->proto = 0;
	}else if( flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto == IPPROTO_UDP){
		sf_key->proto = 1;
	} else{
		printk("[hnat error]%s: error l4 proto 0x%x flow %p\n", __FUNCTION__ ,flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto, flow);
		return -1;
	}

	if (test_bit(NF_FLOW_SNAT, &flow->flags)) {
		sf_key->sip = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.src_v4.s_addr;
		sf_key->sport = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.src_port;

		sf_key->dip = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_v4.s_addr;
		sf_key->dport = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_port;

		sf_key->router_pub_ip = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_v4.s_addr;
		sf_key->router_port = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_port;
	}
	else if (test_bit(NF_FLOW_DNAT, &flow->flags)) {
		sf_key->sip = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.src_v4.s_addr;
		sf_key->sport =flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.src_port;

		sf_key->dip = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_v4.s_addr;
		sf_key->dport = flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_port;

		sf_key->router_pub_ip = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_v4.s_addr;
		sf_key->router_port = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_port;

	}else{
		printk("[hnat error]%s: error flags 0x%lx flow %p\n", __FUNCTION__ ,flow->flags, flow);
		return -1;
	}
	// check dns
	dport = ntohs(sf_key->dport);
	if(dport == DNS_PORT){
		return -2;
	}

	saddr = ntohl(sf_key->sip);
	if(saddr == 0){
		return -1;
	}

	inlan = sf_hnat_is_lan_ip(phnat_priv, sf_key->sip);
	if( inlan >= 0){
		sf_key->lan_subnet_index = inlan;
		if(g_print & SUBNET_DEBUG)
		  printk("[hnat log] get lansubnet %d prefix %d ip %08x\n",  inlan,  phnat_priv->lan_subnet[sf_key->lan_subnet_index].prefix_len, sf_key->sip);
	}else{
		printk("[hnat error]SUBNET error,  sip %pI4h bypass flow %p\n",&saddr, flow);
		return -1;
	}

	inwan = sf_hnat_is_wan_ip(phnat_priv, sf_key->router_pub_ip);
	if( inwan >= 0){
		sf_key->wan_subnet_index = inwan;
		if(g_print & SUBNET_DEBUG)
		  printk("[hnat log] get wansubnet %d prefix %d ip%08x\n",  inwan,  phnat_priv->wan_subnet[sf_key->wan_subnet_index].prefix_len, sf_key->router_pub_ip);
	}else{
		saddr = ntohl(sf_key->router_pub_ip);
		printk("[hnat error]Wan Ip error,  rt pub sip %pI4h not find flow %p\n",&saddr,flow);
		return -1;
	}

	return 0;
}

void sf_hnat_monitor_deladdr (const unsigned int ipaddr, const char *ifname, const unsigned char pf_len)
{
	unsigned char i = 0;
	if(!g_phnat_priv)
	  return;

	if(g_print & MONITOR_IP_DEBUG){
			printk("[hnat debug] monitor del ip %08x name %s  pf %d\n", ipaddr,  ifname,  pf_len);
	}

	for(i=0; i<8; i++){
		if ((strlen(g_phnat_priv->lan_subnet[i].ifname) > 0)
					&& (!strncmp(ifname, g_phnat_priv->lan_subnet[i].ifname,
							strlen(g_phnat_priv->lan_subnet[i].ifname)))){
			if(g_phnat_priv->lan_subnet[i].valid){
				if((g_phnat_priv->lan_subnet[i].ipaddr != ipaddr) ||  \
							g_phnat_priv->lan_subnet[i].prefix_len !=  pf_len)
				{
					printk("[hnat debug] not same  lan ip %08x  %d %s\n", ipaddr,  pf_len,  ifname );
				}
				printk("[hnat info]del lan ip %08x pf %d  index %d\n", g_phnat_priv->lan_subnet[i].ipaddr,  g_phnat_priv->lan_subnet[i].prefix_len, i );
				sf_hnat_del_lan_subnet(g_phnat_priv,  i);
				spin_lock(&hnat_br_lock);
				g_br = NULL;
				g_br_dev = NULL;
				spin_unlock(&hnat_br_lock);
				return;
			}else {
				printk("[hnat debug]not get valid lan ip %08x  %d %s\n", ipaddr,  pf_len,  ifname );
			}
		}
	}

	for(i=0; i<8; i++){
		if ((strlen(g_phnat_priv->wan_subnet[i].ifname) > 0)
					&& (!strncmp(ifname, g_phnat_priv->wan_subnet[i].ifname,
							strlen(g_phnat_priv->wan_subnet[i].ifname)))){
			if(g_phnat_priv->wan_subnet[i].valid){
				if((g_phnat_priv->wan_subnet[i].ipaddr != ipaddr) ||  \
							(g_phnat_priv->wan_subnet[i].prefix_len != pf_len)){
					printk("[hnat debug] not same  wan ip %08x  %d %s\n", ipaddr,  pf_len,  ifname );
				}

				printk("[hnat info]del wan ip %08x pf %d  index %d\n", g_phnat_priv->wan_subnet[i].ipaddr,  g_phnat_priv->wan_subnet[i].prefix_len, i );
				sf_hnat_del_wan_subnet(g_phnat_priv,  i);
				g_pppoe_flag = 0;
				return;
			}else {
				printk("[hnat debug]not get valid wan ip %08x  %d %s  idx %d valid %d \n", ipaddr,  pf_len,  ifname ,i, g_phnat_priv->wan_subnet[i].valid);
			}
		}
	}
}

EXPORT_SYMBOL(sf_hnat_monitor_deladdr);

void sf_hnat_monitor_newaddr (const unsigned int ipaddr, const char *ifname, const unsigned char pf_len)
{
	unsigned char i = 0;
	if(!g_phnat_priv || !ipaddr)
	  return;
	if(g_print & MONITOR_IP_DEBUG){
			printk("[hnat debug] monitor new ip %08x name %s  pf %d\n", ipaddr,  ifname,  pf_len);
	}
	for(i=0; i<8; i++){
		if ((strlen(g_phnat_priv->lan_subnet[i].ifname) > 0)
					&& (!strncmp(ifname, g_phnat_priv->lan_subnet[i].ifname,
							strlen(g_phnat_priv->lan_subnet[i].ifname)))){
			if(g_phnat_priv->lan_subnet[i].valid ){
				if((g_phnat_priv->lan_subnet[i].ipaddr == ipaddr) &&  \
							(g_phnat_priv->lan_subnet[i].prefix_len == pf_len)){
					printk("[hnat warning] lan ip already here dev %s ip %08x\n", ifname, ipaddr);
					return;
				}
				else {
					sf_hnat_del_lan_subnet(g_phnat_priv,  i);
				}
			}

			g_phnat_priv->lan_subnet[i].ipaddr = ipaddr;
			g_phnat_priv->lan_subnet[i].prefix_len = pf_len;
			sf_hnat_update_lan_subnet(g_phnat_priv,  i);
			printk("[hnat info]add lan ip %08x pf %d  index %d name %s\n", ipaddr,  pf_len, i,ifname);
			spin_lock(&hnat_br_lock);
			g_br_dev = dev_get_by_name_rcu(&init_net, "br-lan");
			if (g_br_dev) {
				g_br = netdev_priv(g_br_dev);
			}
			spin_unlock(&hnat_br_lock);
			return ;
		}
	}

	for(i=0; i<8; i++){
		if ((strlen(g_phnat_priv->wan_subnet[i].ifname) > 0)
					&& (!strncmp(ifname, g_phnat_priv->wan_subnet[i].ifname,
							strlen(g_phnat_priv->wan_subnet[i].ifname)))){
			if(g_phnat_priv->wan_subnet[i].valid ){
				if((g_phnat_priv->wan_subnet[i].ipaddr == ipaddr) &&  \
							(g_phnat_priv->wan_subnet[i].prefix_len == pf_len)){
					printk("[hnat warning] wan ip already here  dev %s pf %08x\n", ifname, pf_len);
					return;
				}
				else {
					sf_hnat_del_wan_subnet(g_phnat_priv,  i);
				}
			}
			g_phnat_priv->wan_subnet[i].ipaddr  =ipaddr;
			g_phnat_priv->wan_subnet[i].prefix_len = pf_len;
			g_phnat_priv->wan_subnet[i].valid = 1;
			printk("[hnat info]add wan ip %08x pf %d  index %d vld %d name %s\n", ipaddr,  pf_len, i, g_phnat_priv->wan_subnet[i].valid, ifname );
			if(!strcmp(ifname, "pppoe-wan"))
				g_pppoe_flag = 1;
			else
				g_pppoe_flag = 0;
			return ;
		}
	}
}
EXPORT_SYMBOL(sf_hnat_monitor_newaddr);

int sf_hnat_ndo_flow_offload(struct platform_device *pdev,
			enum flow_offload_type type,
			struct flow_offload *flow,
			struct flow_offload_hw_path *src,
			struct flow_offload_hw_path *dest) {

	struct sf_hashkey  key_tmp;
	struct sf_hashkey *sf_key = &key_tmp;
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	int napt_index;
	char wifi_index = -1;
	int ret;

	smp_rmb();
	if (type == FLOW_OFFLOAD_DEL){
		if(flow->hnat_idx < 0){
			printk("[hnat error] del flow %p index %d\n",flow, flow->hnat_idx);
			return -1;
		}

		ret = sf_hnat_delete_entry_by_index(phnat_priv, (unsigned int)flow->hnat_idx);

		if (ret < 0)
			goto unlock_fail;

		if(g_print & COUNT_DEBUG)
			printk("[hnat info] del flow %p index %d\n",flow, flow->hnat_idx);

		goto unlock_success;
	}

	memset(sf_key, 0,  sizeof(struct sf_hashkey));

	ret = sf_hnat_check_ip_info(phnat_priv, flow, sf_key);
	if(ret < 0){
		if((ret == -1) && (NAPT_DEBUG & g_print ))
		  sf_hnat_print_debug(type, flow, src, dest);

		goto unlock_fail;
	}

	if(g_print & NAPT_DEBUG)
	  sf_hnat_print(type, flow, src, dest);
	// printk("sizeof  %d\n", sizeof(struct sf_hashkey));
	// printk("ppp_sid %d\n", sf_key->ppp_sid);
	if(test_bit(NF_FLOW_SNAT, &flow->flags)){
		if((dest->flags & FLOW_OFFLOAD_PATH_PPPOE)){
			// printk("enable pppoe here\n");
			sf_key->cur_pppoe_en = 1;
		}
		else{
			sf_key->cur_pppoe_en = 0;
		}

		memcpy(sf_key->src_mac, src->eth_dest,6);
		sf_key->src_vlan= src->vlan_id;
		memcpy(sf_key->router_src_mac, src->eth_src, 6);

		memcpy(sf_key->dest_mac, dest->eth_dest,6);
		sf_key->dest_vlan = dest->vlan_id;
		memcpy(sf_key->router_dest_mac, dest->eth_src, 6);

		if(src->dev == dest->dev)
		  sf_key->dnat_to_host = 0;//switch lan to switch wan, both eth0
		else{
			// wifi entry
			wifi_index = sf_hnat_search_wifi_ndev_index(phnat_priv,src->dev);
			if(wifi_index < 0){
				printk("[hnat error] search wifi index fail \n");
				goto unlock_fail;
			}

			// RM#9076 diff wifi pppoe flow and dhcp flow witch vlan
			if (sf_key->cur_pppoe_en == 1)
				sf_key->src_vlan = phnat_priv->wifi_base + WIFI_PPPOE_OFFSET + wifi_index;
			else
				sf_key->src_vlan = phnat_priv->wifi_base + wifi_index;
			sf_key->dnat_to_host = 1;//wifi lan to wan, wlan0-eth0
		}

		if(sf_key->cur_pppoe_en)
		  sf_key->ppp_sid = dest->pppoe_sid;
	}
	else if(test_bit(NF_FLOW_DNAT, &flow->flags)) {
		if((src->flags & FLOW_OFFLOAD_PATH_PPPOE)){
			// printk("enable pppoe here\n");
			sf_key->cur_pppoe_en = 1;
		}
		else{
			sf_key->cur_pppoe_en = 0;
		}
		//IPPROTO_TCP IPPROTO_UDP

		memcpy(sf_key->src_mac, dest->eth_dest,6);
		memcpy(sf_key->router_src_mac, dest->eth_src, 6);

		memcpy(sf_key->dest_mac, src->eth_dest,6);
		memcpy(sf_key->router_dest_mac, src->eth_src, 6);

		sf_key->dest_vlan = src->vlan_id;
		sf_key->src_vlan= dest->vlan_id;
		if(src->dev == dest->dev)
		  sf_key->dnat_to_host = 0;//switch lan to switch wan, both eth0
		else{
			// wifi entry
			wifi_index = sf_hnat_search_wifi_ndev_index(phnat_priv,dest->dev);
			if(wifi_index < 0){
				printk("[hnat error] search wifi index fail \n");
				goto unlock_fail;
			}
			if (sf_key->cur_pppoe_en == 1)
				sf_key->src_vlan = phnat_priv->wifi_base + WIFI_PPPOE_OFFSET + wifi_index;
			else
				sf_key->src_vlan = phnat_priv->wifi_base + wifi_index;
			sf_key->dnat_to_host = 1;//wifi lan to wan, wlan0-eth0
		}

		if(sf_key->cur_pppoe_en)
		  sf_key->ppp_sid = src->pppoe_sid;
	}else{
		printk("[hnat error]not support \n");
	}
	// add for aging napt entrys
	sf_key->pkey_flow = flow;
	// this flag indicate pkey flow is not free in netfilter
	// sf_key->keep_flag = 1;
	// printk("ppp_sid %d\n", sf_key->ppp_sid);
	// ret == 0  no need to  inc  ct count in  flow offload
	if(type == FLOW_OFFLOAD_ADD){
		if (phnat_priv->curr_napt_num >= FLOWOFFLOAD_HW_MAX) {
			if ((jiffies - phnat_priv->last_flush_time) > (300*HZ)) {
				nf_flow_need_all_aging = true;
				phnat_priv->last_flush_time = jiffies;
			}
			goto unlock_fail;
		} else {
			nf_flow_need_all_aging = false;
		}
		 ret = sf_hnat_add_entry(phnat_priv, sf_key , &napt_index);
		if(ret >= 0){
			// printk("[hnat debug] add entry 0x%p flags 0x%x flags 0x%p napt_index %d\n", flow,flow->flags, &flow->flags, napt_index);
			flow->hnat_idx = napt_index;
			smp_wmb();

			if(g_print & COUNT_DEBUG)
			  printk("[hnat info] add flow %p index %d\n",flow, napt_index );

			return ret;
		}else{
			//printk("[hnat error] add entry fail 0x%p\n", flow);
			add_fail_count++;
			goto unlock_fail;
		}
	}

unlock_success:
	return 1;
unlock_fail:
	return -1;
}

#ifdef CONFIG_DEBUG_FS
static struct file_operations hnat_debug_ops = {
	.owner = THIS_MODULE,
	.open  = sf_hnat_debug_open,
	.read  = sf_hnat_debug_read,
	.write  = sf_hnat_debug_write,
	.llseek  = default_llseek,
	.unlocked_ioctl = sf_hnat_debug_ioctl
};
#endif


int sf_hnat_deinit(struct platform_device *pdev){

	struct sf_hnat_priv *phnat_priv = platform_get_drvdata(pdev);

	cancel_work_sync(&nf_flow_offload_hw_work);

	// reset hnat here
	hnat_writel(phnat_priv, 0x0, HWNAT_REG1_CSR);

	phnat_priv->driver_ndev = NULL;
	phnat_priv->base = 0;
	return 0;
}

// use software to add vlan tag
int sf_hnat_xmit_prepare(struct platform_device *pdev, struct sk_buff *skb){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	// skb_vlan_push(skb,0x0081,0x1);
	skb->cb[SF_HNAT_FLAG] = SF_CB_HNAT_FORWARD;
	skb->dev = phnat_priv->driver_ndev;

	return 0;
}
// use software to add vlan tag
int sf_hnat_xmit(struct platform_device *pdev, struct sk_buff *skb){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	// skb_vlan_push(skb,0x0081,0x1);
	// skb->cb[SF_HNAT_FLAG] = SF_CB_HNAT_FORWARD;
	skb->dev = phnat_priv->driver_ndev;
	dev_queue_xmit(skb);
	return 0;
}
int sf_hnat_init(struct platform_device *pdev, void __iomem *reg_base, struct net_device *pdriver_ndev){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	phnat_priv->base = reg_base;

	if(pdriver_ndev != NULL)
	  phnat_priv->driver_ndev = pdriver_ndev;

#ifdef  CONFIG_SF19A28_FULLMASK
	sf_hnat_flush_all(phnat_priv);
	// printk("[hnat notice]%s hnat_mode = 0x%x vlan_replace = 0x%x\n", __FUNCTION__, phnat_priv->hnat_mode, phnat_priv->vlan_replace);
	sf_hnat_csr_init(phnat_priv);//priv->hnat_mode, phnat_priv->vlan_replace);

#endif

	return 0;
}

void print_flow_offload_hw_path(struct flow_offload_hw_path *path) {
	// flags means
	// #define FLOW_OFFLOAD_PATH_ETHERNET¦ BIT(0)
	// #define FLOW_OFFLOAD_PATH_VLAN¦ ¦   BIT(1)
	// #define FLOW_OFFLOAD_PATH_PPPOE¦¦   BIT(2)
	printk("==================hw path flags=0x%x vlan_proto=0x%x vlan_id=0x%x pppoe_sid=0x%x devname=%s\n",
				path->flags, path->vlan_proto, path->vlan_id, path->pppoe_sid,path->dev->name);
	printk(" src_mac=%pM dst_mac=%pM  \n", path->eth_src, path->eth_dest);
}

/*
 * print flow offload
 * typical 2 tcp directions like below
 * ##############################SNAT########################################
 * tcp connection from LAN(192.168.4.107:55423) > WAN(192.168.12.161:5001)
 * gmac_ndo_flow_offload flow=83065700 type=0 flow->flags=1 timeout=379af
 *  ORIGINAL:
 *      src[192.168.4.107:55423] dst[192.168.12.161:5001]
 *  REPLY:
 *      src[192.168.12.161:5001] dst[192.168.12.149:55423]
 & ##############################DNAT########################################
 * tcp connection from WAN(192.168.12.161:44166) > LAN(192.168.4.107:5555) (DMZ)
 * gmac_ndo_flow_offload flow=8329fa00 type=0 flow->flags=2 timeout=76d93
 *  ORIGINAL:
 *      src[192.168.12.161:44166] dst[192.168.12.149:5555]
 *  REPLY:
 *      src[192.168.4.107:5555] dst[192.168.12.161:44166]
 * */

void print_flow_offload(struct flow_offload *flow) {
	u16 sport, dport;           /* Source and destination ports */
	u32 saddr, daddr;
	saddr = ntohl(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.src_v4.s_addr);
	daddr = ntohl(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_v4.s_addr);
	sport = ntohs(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.src_port);
	dport = ntohs(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.dst_port);

	printk("!!!!!!!!!!!!!!!!!!!!!flow_offload flags=0x%lx timeout=0x%x l4 proto= 0x%x l3proto=0x%x \n",flow->flags, flow->timeout, flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto, flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l3proto);

	printk("ORIGINAL: src:[%pI4h:%d] -> dest:[%pI4h:%d]\n", &saddr, sport, &daddr, dport);
	saddr = ntohl(flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.src_v4.s_addr);
	daddr = ntohl(flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_v4.s_addr);
	sport = ntohs(flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.src_port);
	dport = ntohs(flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple.dst_port);

	printk("REPLY: src:[%pI4h:%d] -> dest:[%pI4h:%d]\n", &saddr, sport, &daddr, dport);
}
//l4proto IPPROTO_TCP
//l3 proto	PF_INET
int sf_hnat_print(enum flow_offload_type type, struct flow_offload *flow, struct flow_offload_hw_path *src, struct flow_offload_hw_path *dest) {
		printk("hnat offload %s type=%d \n", (type == FLOW_OFFLOAD_ADD) ? "ADD":"DEL", type );
		print_flow_offload(flow);
		printk("src=========\n");
		print_flow_offload_hw_path(src);
		printk("dest=========\n");
		print_flow_offload_hw_path(dest);
	return 0;
}


static struct net_device * sf_hnat_find_wifi_ndev_by_vlan(struct sf_hnat_priv *phnat_priv, unsigned short vlan_id){
	unsigned char index = vlan_id - phnat_priv->wifi_base;
	if (index >= WIFI_PPPOE_OFFSET)
		return phnat_priv->pwifi_ndev[index-WIFI_PPPOE_OFFSET];

	return phnat_priv->pwifi_ndev[index];
}

int sf_is_hnat_to_wifi_pkt(struct sf_hnat_priv *phnat_priv, struct sk_buff *skb){
	const struct ethhdr *peth_hdr = (struct ethhdr *)skb->data;
	const unsigned char *smac = peth_hdr->h_source;
	struct iphdr* pip_hdr;
	int i = 0;
	if(peth_hdr->h_proto == htons(ETH_P_IP)) {
		pip_hdr = (struct iphdr *) ((unsigned char*)peth_hdr + sizeof(struct ethhdr));
		if(sf_hnat_is_lan_ip(phnat_priv, pip_hdr->saddr) > 0)
			return 0;
		if (sf_hnat_is_lan_ip(phnat_priv, pip_hdr->daddr) < 0)
			return 0;
		spin_lock(&hnat_br_lock);
		if ((g_br_dev) && (ether_addr_equal(g_br_dev->dev_addr, smac))) {
			if (g_print & WDEV_DEBUG)
				printk("[hnat debug] %s %d smac %pM dmac %pM sip %pI4 dip %pI4\n",
					__FUNCTION__, __LINE__, smac, peth_hdr->h_dest, &pip_hdr->saddr, &pip_hdr->daddr);
			i=1;
		}
		spin_unlock(&hnat_br_lock);
	}
	return i;
}

static char  sf_hnat_get_wifi_ndev_index(struct sf_hnat_priv *phnat_priv, struct net_device * ndev){
	unsigned char i = 0;
	char ret = -1;
	for(; i < 8 ; i++){
		if(phnat_priv->pwifi_ndev[i] == ndev){
			// printk("[hnat notice]find wifi ndev  exist\n");
			return i;
		}
		if(phnat_priv->pwifi_ndev[i] == NULL){
			ret = i;
		}
	}
	return ret;
}

extern struct net_device *sf_hnat_find_wifi_ndev_by_dmac(struct sk_buff *skb, struct net_bridge *br);

int sf_hnat_wifi_xmit_prepare(struct platform_device *pdev, struct sk_buff *skb, unsigned short vlan_id, int  frame_len){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	struct net_device * wifi_ndev = NULL;
	if (vlan_id == 0) {
		if (g_br)
			wifi_ndev = sf_hnat_find_wifi_ndev_by_dmac(skb, g_br);
	} else
		wifi_ndev = sf_hnat_find_wifi_ndev_by_vlan(phnat_priv,vlan_id);

	if (vlan_id >= (phnat_priv->wifi_base + WIFI_PPPOE_OFFSET)){
		skb_put(skb, frame_len- 8);
	} else if ((vlan_id == 0) && (wifi_ndev) && (g_pppoe_flag == 1)) {
		skb_put(skb, frame_len - 8);
		if(g_print & WDEV_DEBUG)
			printk("pppoe\n");
	} else
	  skb_put(skb, frame_len);

	// would change skb->dev here
	skb->protocol = eth_type_trans(skb, phnat_priv->driver_ndev);

	if(wifi_ndev){
		//skb->dev = wifi_ndev;
		skb->cb[SF_HNAT_FLAG] = SF_CB_HNAT_FORWARD;

		if(g_print & WDEV_DEBUG )
		  printk("[hnat debug] %s %d find wifi ndev %p, vlan %d  len %d\n",__FUNCTION__,__LINE__, wifi_ndev, vlan_id, frame_len);

		*((unsigned int*)(skb->cb+SF_HNAT_DEV_OFFSET)) = (unsigned int)wifi_ndev;
	}
	else{
		if(g_print & WDEV_DEBUG)
			printk("error find not wifi device\n");
		return -1;
	}
	return 0;
}

int sf_hnat_add_wifi_ndev(struct platform_device *pdev, struct net_device *ndev){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	char index = sf_hnat_get_wifi_ndev_index(phnat_priv, ndev);
	if(index < 0){
		printk("[hnat error]error add wifi ndev\n");
		return -1;
	}
	else{
		phnat_priv->pwifi_ndev[(unsigned char)index] = ndev;
		printk("[hnat notice]add wifi dev index %d ndev%p\n", (unsigned int)index, ndev);
	}
	return 0;
}

int sf_hnat_del_wifi_ndev(struct platform_device *pdev, struct net_device *ndev){

	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	char index = sf_hnat_search_wifi_ndev_index(phnat_priv, ndev);
	if(index < 0){
		printk("[hnat error]error del wifi ndev\n");
		return -1;
	}
	else{
		phnat_priv->pwifi_ndev[(unsigned char)index] = NULL;
	}
	return 0;
}


static int sf_hnat_probe(struct platform_device *pdev) {

	struct sf_hnat_priv *phnat_priv;
	unsigned char i = 0;

	phnat_priv = kzalloc(sizeof(struct sf_hnat_priv), GFP_KERNEL);
	if (!phnat_priv) {
		dev_err(&pdev->dev, "no memory for gswitch data\n");
		return -ENOMEM;
	}
	memset(phnat_priv, 0, sizeof(struct sf_hnat_priv));
	g_phnat_priv  = phnat_priv;
	platform_set_drvdata(pdev, phnat_priv);
	phnat_priv->hnat_mode = SYMMETRIC_MODE;
	phnat_priv->vlan_replace = 0;
	phnat_priv->dev = &pdev->dev;
	phnat_priv->snat_arp_force_dip = 0;
	phnat_priv->use_big_endian = 1;// for table write
	phnat_priv->wifi_base = 4000;
	phnat_priv->last_age_time = jiffies;
	phnat_priv->last_flush_time = jiffies;
	phnat_priv->init = sf_hnat_init;
	phnat_priv->deinit = sf_hnat_deinit;
	phnat_priv->is_hnat_error  = sf_hnat_is_error;
	phnat_priv->ndo_flow_offload = sf_hnat_ndo_flow_offload;
	phnat_priv->search = sf_hnat_search;
	phnat_priv->sts_cnt = sf_hnat_statistic_count_read;
	phnat_priv->print = sf_hnat_print;
	phnat_priv->is_hnat_to_wifi_pkt = sf_is_hnat_to_wifi_pkt;
	phnat_priv->wifi_xmit_prepare = sf_hnat_wifi_xmit_prepare;
	phnat_priv->add_wifi_ndev = sf_hnat_add_wifi_ndev;
	phnat_priv->del_wifi_ndev = sf_hnat_del_wifi_ndev;

// ATTENTION  set interface name here

	phnat_priv->lan_subnet[0].ipaddr  = 0xc0a80400;
	phnat_priv->lan_subnet[0].prefix_len = DEFAULT_LAN_MASKLEN;

	for(i=0; i<8; i++){
		phnat_priv->lan_subnet[i].ifname[0] = '\0';
		phnat_priv->wan_subnet[i].ifname[0] = '\0';
	}

	// ATTENTION   change here for your  device name
	sprintf(phnat_priv->lan_subnet[0].ifname, "%s", "br-lan");
	sprintf(phnat_priv->lan_subnet[1].ifname, "%s", "br-guest");
#ifdef CONFIG_DT_SF19A28_FULLMASK_PHY
	sprintf(phnat_priv->wan_subnet[0].ifname, "%s", "eth0");
#endif
	sprintf(phnat_priv->wan_subnet[0].ifname, "%s", "eth0.2");
	sprintf(phnat_priv->wan_subnet[1].ifname, "%s", "pppoe-wan");

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	phnat_priv->ptest_priv = kzalloc(sizeof(struct sf_test_tool_priv), GFP_KERNEL);;
	phnat_priv->ptest_priv->tool_init = sf_test_tool_init;
	phnat_priv->ptest_priv->tool_deinit = sf_test_tool_deinit;
	phnat_priv->ptest_priv->tool_set_rx = sf_test_tool_set_rx;
#endif
#ifdef CONFIG_DEBUG_FS
	phnat_priv->hnat_debug = debugfs_create_file("hnat_debug", 0777, NULL,(void*)phnat_priv , &hnat_debug_ops);
#endif

    spin_lock_init(&hnat_tb_lock);
    spin_lock_init(&hnat_csr_lock);
    spin_lock_init(&hnat_napt_lock);
	spin_lock_init(&hnat_br_lock);
    // spin_lock_init(&napt_offload_lock);
	printk("[hnat notice]hnat probe success\n");
	return 0;
}

static int sf_hnat_remove(struct platform_device *pdev) {

	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
#ifdef CONFIG_DEBUG_FS
	debugfs_remove(phnat_priv->hnat_debug);
#endif

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	kfree(phnat_priv->ptest_priv);
#endif
	platform_set_drvdata(pdev, NULL);
	g_phnat_priv  = NULL;
	kfree(phnat_priv);
	printk("[hnat notice]hnat remove success\n");
	return 0;
}

static struct platform_driver sf_hnat_driver = {
	.driver = {
		.name = "sf_hnat",
		.owner = THIS_MODULE,
	},
	.probe = sf_hnat_probe,
	.remove = sf_hnat_remove,
};

static int __init sf_hnat_module_init(void) {
	return platform_driver_register(&sf_hnat_driver);
}
module_init(sf_hnat_module_init);

static void __exit sf_hnat_module_exit(void) {
	platform_driver_unregister(&sf_hnat_driver);
}
module_exit(sf_hnat_module_exit);


MODULE_AUTHOR("phoenixshen, <phoenix.shen@siflower.com.cn>");
MODULE_DESCRIPTION("Siflower HNAT driver");
MODULE_LICENSE("GPL v2");
