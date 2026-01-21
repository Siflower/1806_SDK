#include "sf_hnat.h"
#ifdef  CONFIG_SFAX8_HNAT_TEST_TOOL
#include "sf_hnat_test_tool.h"
#endif
#include "sf_hnat_common.h"
#include "sf_hnat_debug.h"
#include "sf_pcap_parse.h"
#include <linux/inet.h>
#include <linux/delay.h>

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
#include "sf_test_thread_poll.h"

#define TEST_PKT_LEN 1512
#define TEST_SET_MAX 100


struct sf_test_tool_set  pg_test_sets[TEST_SET_MAX];

struct sf_frag_pkt_set test_frag_set = {
	.frag_no = 2,
	.ip_id_no = 3,
	.is_frag = 0,
	.is_continus = 0,
};

#endif

extern  unsigned int napt_hash_full_count;
extern	unsigned int dip_hash_full_count;
extern	unsigned int add_fail_count;
extern	unsigned int update_flow_count;
extern	unsigned int crc_clean_flow_count;
extern	unsigned int sf_hnat_udp_check;
extern 	unsigned int sf_hnat_enable;
void sf_hnat_hw_dump_count(void){
	const struct kernel_symbol *sym;
	void (*dump_count)(void) = NULL;

	preempt_disable();

	sym = find_symbol("sf_flow_dump_count", NULL, NULL, true, true);
	if (sym) {
		dump_count = (void *)sym->value;
	}
	preempt_enable();

	if(dump_count)
	  dump_count();
}
extern unsigned char g_print;
#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
;

#ifdef CONFIG_DEBUG_FS
int sf_hnat_debug_open(struct inode *inode, struct file *file) {
	file->private_data = inode->i_private;
	return 0;
}

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL

void print_rx_data(u64 total_pkt_num, u32 tick, u64 total_data_len, u32 pre_fix){
	u32 band_width = Divided_64(total_data_len * 8, 1024);
	band_width = band_width *  CONFIG_HZ / tick ;
	if ( tick == CONFIG_HZ )
	  printk("[recv] %ds - %ds  band_width %uKbps process pkt %llu  data %llu\n",
				  pre_fix, pre_fix + tick/CONFIG_HZ, band_width,total_pkt_num,total_data_len);
	else
	  printk("[recv] %ds - %ds  band_width %uKbps process pkt %llu  data %llu\n",
				 pre_fix, pre_fix + tick/CONFIG_HZ + 1, band_width,total_pkt_num,total_data_len);

}


void sf_gmac_rx_timer_handler(unsigned long data){
	struct sf_test_tool_priv *ptest_priv = (struct sf_test_tool_priv *)data;
	struct sf_rx_test_unit *prx_unit = &ptest_priv->rx_test_unit;
	struct timer_list* pop_timer = &prx_unit->rx_timer;

	u32 exp_delta = 0;
	u64 target_jiffies = 0, now_jiffies = 0;
	u32 pkt_recv = 0;
	u32 data_recv = 0;

	prx_unit->rx_last_timer_jiffies += CONFIG_HZ;
	target_jiffies = prx_unit->rx_target_jiffies;
	pkt_recv = prx_unit->rx_pkt_count - prx_unit->rx_last_pkt_count;
	data_recv = prx_unit->rx_total_data - prx_unit->rx_last_data_count;

	prx_unit->rx_last_pkt_count = prx_unit->rx_pkt_count;
	prx_unit->rx_last_data_count = prx_unit->rx_total_data;

	print_rx_data(pkt_recv, CONFIG_HZ, data_recv, prx_unit->rx_timer_index++);

	now_jiffies = get_jiffies_64();
	exp_delta = (prx_unit->rx_last_timer_jiffies + CONFIG_HZ  - now_jiffies);

	// printk("here line %d\n", __LINE__);
	if(now_jiffies >= target_jiffies || prx_unit->should_end == 1  ){

		ptest_priv->g_start_test_rx = 0;
		print_rx_data(prx_unit->rx_pkt_count , prx_unit->end_rx_jiffies - prx_unit->start_rx_jiffies,prx_unit->rx_total_data,  0);
		del_timer(pop_timer);
	}
	else
	  mod_timer(pop_timer, jiffies + exp_delta);

	return;
}

void  sf_gmac_test_set_pkt(u8 set_index ,char* file_name, u8 is_wan_lan , u8 is_jumbo){
	int ret = 0;
	struct sf_test_tool_set * ptest_set = &pg_test_sets[set_index];


	struct sf_cap_file_data * pcap_data = vmalloc(sizeof(struct sf_cap_file_data));
	if(!pcap_data){
		printk("malloc cap data  fail\n");
		return ;
	}

	ret = sf_parse_cap_file(file_name, is_jumbo, pcap_data);
	if(ret < 0){
		printk("parse cap file fail \n");
		goto error_parse;
	}

	ptest_set->pkt_is_wan_lan = is_wan_lan;
	ptest_set->proto = pcap_data->is_udp;

	if(pcap_data->is_pppoe)
	  ptest_set->ppp_sid = pcap_data->tpppoe_hdr.hdr.sid;

	if(is_wan_lan){
		memcpy(ptest_set->dest_mac,pcap_data->teth_hdr.h_source,ETH_ALEN);
		memcpy(ptest_set->router_dest_mac,pcap_data->teth_hdr.h_dest,ETH_ALEN);
		ptest_set->dip = pcap_data->tip_hdr.saddr;
		ptest_set->router_ip = pcap_data->tip_hdr.daddr;

		if( pcap_data->is_udp){
			ptest_set->dport = ntohs(pcap_data->tudp_hdr.source);
			ptest_set->router_port = ntohs(pcap_data->tudp_hdr.dest);
		}
		else{
			ptest_set->dport = ntohs(pcap_data->ttcp_hdr.source);
			ptest_set->router_port = ntohs(pcap_data->ttcp_hdr.dest);
		}


		if(pcap_data->is_vlan)
		  ptest_set->dest_vlan = ntohs(pcap_data->tvlan_hdr.h_vlan_TCI & VLAN_VID_MASK);
	}
	else{
		memcpy(ptest_set->src_mac,pcap_data->teth_hdr.h_source,ETH_ALEN);
		memcpy(ptest_set->router_src_mac,pcap_data->teth_hdr.h_dest,ETH_ALEN);
		ptest_set->sip = pcap_data->tip_hdr.saddr;
		ptest_set->dip = pcap_data->tip_hdr.daddr;

		if( pcap_data->is_udp){
			ptest_set->sport = ntohs(pcap_data->tudp_hdr.source);
			ptest_set->dport = ntohs(pcap_data->tudp_hdr.dest);
		}
		else{
			ptest_set->sport = ntohs(pcap_data->ttcp_hdr.source);
			ptest_set->dport = ntohs(pcap_data->ttcp_hdr.dest);
		}

		if(pcap_data->is_vlan)
		  ptest_set->src_vlan = ntohs(pcap_data->tvlan_hdr.h_vlan_TCI & VLAN_VID_MASK);
	}

	ptest_set->valid = 1;
	ptest_set->pcap_file = pcap_data;
	return;
error_parse:
	vfree(pcap_data);
	return;
}


// TYPE 0 src TYPE 1 dest  TYPE 3 router
void sf_gmac_test_set_manual(u8 type, u8 set_index ,char* ip,   char* port , char* mac1, char* mac2 ){
	unsigned char* set_mac1 = NULL;
	unsigned char* set_mac2 = NULL;
	int ret = 0;
	switch (type) {
		// src
		case 0:
			pg_test_sets[set_index].sip =  in_aton(ip);
			ret = kstrtou16(port, 0 , &pg_test_sets[set_index].sport);
			set_mac1 = pg_test_sets[set_index].src_mac;
			break;
		//dest
		case 1:
			pg_test_sets[set_index].dip =  in_aton(ip);
			ret = kstrtou16(port, 0 , &pg_test_sets[set_index].dport);
			set_mac1 = pg_test_sets[set_index].dest_mac;
			break;
		// router
		case 2:
			pg_test_sets[set_index].router_ip = in_aton(ip);
			ret = kstrtou16(port, 0 , &pg_test_sets[set_index].router_port);
			set_mac1 = pg_test_sets[set_index].router_src_mac;
			set_mac2 = pg_test_sets[set_index].router_dest_mac;

			break;
		default:
			break;
	}

	if(set_mac1 != NULL){
			sscanf(mac1,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
					&set_mac1[0],&set_mac1[1],&set_mac1[2],
					&set_mac1[3],&set_mac1[4],&set_mac1[5]);
	}

	if(set_mac2 != NULL){
			sscanf(mac2,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
					&set_mac2[0],&set_mac2[1],&set_mac2[2],
					&set_mac2[3],&set_mac2[4],&set_mac2[5]);
	}

	pg_test_sets[set_index].valid = 1;
	return;
}

int sf_test_tool_init_rx_timer(struct sf_test_tool_priv * ptest_priv){
	struct timer_list* pop_timer = NULL;
	u32 exp_delta = 0;

	struct sf_rx_test_unit * prx_unit = &ptest_priv->rx_test_unit;
	pop_timer = &prx_unit->rx_timer;
	exp_delta = (prx_unit->start_rx_jiffies + CONFIG_HZ - get_jiffies_64());
	prx_unit->rx_target_jiffies = prx_unit->start_rx_jiffies + prx_unit->time * CONFIG_HZ;
	prx_unit->rx_last_timer_jiffies = prx_unit->start_rx_jiffies;
	prx_unit->rx_last_pkt_count = 0;
	prx_unit->rx_timer_index = 0;

	init_timer(pop_timer);
	pop_timer->data     = (unsigned long)ptest_priv;
	pop_timer->function = sf_gmac_rx_timer_handler;
	pop_timer->expires  = jiffies +  exp_delta;
	printk("ticks %d \n", exp_delta);
	add_timer(pop_timer);
	return 0;
}

// 0 set start 1 set end 1 set pkt count
void sf_test_tool_set_rx(struct platform_device *pdev, unsigned char type, unsigned int pkt_length){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	struct sf_test_tool_priv *ptest_priv = phnat_priv->ptest_priv;
	if(type == 0 ){
		if(ptest_priv->rx_test_unit.start_rx_jiffies == 0){
			ptest_priv->rx_test_unit.start_rx_jiffies = get_jiffies_64();
			sf_test_tool_init_rx_timer(ptest_priv);
		}
	}else if(type == 2){
		ptest_priv->rx_test_unit.rx_pkt_count++;
		ptest_priv->rx_test_unit.rx_total_data += pkt_length;
	}else if (type == 1){
		ptest_priv->rx_test_unit.end_rx_jiffies = get_jiffies_64();
	}
	return;
}

void sf_test_tool_init(struct platform_device *pdev, struct device *driver_dev, void * driver_priv){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	struct sf_test_tool_priv* ptest_priv = phnat_priv->ptest_priv;
	ptest_priv->hnat_priv = phnat_priv;
	ptest_priv->frag_set = &test_frag_set;
	ptest_priv->g_start_test_tx = 0;
	ptest_priv->g_start_test_rx = 0;
	ptest_priv->g_start_test = 0;
	ptest_priv->last_use_index = 0;
	ptest_priv->driver_dev = driver_dev;
	ptest_priv->driver_priv = driver_priv;
	ptest_priv->rest_space = 1;
	memset(pg_test_sets, 0 , sizeof(struct sf_test_tool_set) * TEST_SET_MAX);
	ptest_priv->ptest_sets = pg_test_sets;
	sf_thread_pool_init(ptest_priv);
	atomic_set(&ptest_priv->is_tx_pause, 0);
	return;
}

void sf_test_tool_deinit(struct platform_device *pdev){
	struct sf_hnat_priv* phnat_priv = platform_get_drvdata(pdev);
	struct sf_test_tool_priv* ptest_priv = phnat_priv->ptest_priv;
	u8 i = 0;
	sf_thread_pool_deinit(ptest_priv);
	for(; i < TEST_SET_MAX; i++ ){
		if(pg_test_sets[i].pcap_file != NULL){
			vfree(pg_test_sets[i].pcap_file);
			pg_test_sets[i].pcap_file = NULL;
		}
	}
	return;
}
#endif

ssize_t sf_hnat_debug_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos) {
	int buf_count = 0, ret = 0;
	char buf[512] = {0};
	size_t read;

	struct sf_hnat_priv *phnat_priv = (struct sf_hnat_priv *)file->private_data;

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	struct sf_test_tool_priv * ptest_priv = phnat_priv->ptest_priv;
	u8  use_index = ptest_priv->last_use_index;
	ret = sprintf(buf, "hnat test tool config index :%d\n", use_index );
	buf_count += ret;
	ret = sprintf(buf+buf_count, " cap file 0x%p valid %d\n", pg_test_sets[use_index].pcap_file , pg_test_sets[use_index].valid);
	buf_count += ret;

	ret = sprintf(buf+buf_count, " src ip %d.%d.%d.%d, port: %d ,mac  %pM \n",
				NIPQUAD(pg_test_sets[use_index].sip),  pg_test_sets[use_index].sport, pg_test_sets[use_index].src_mac);
	buf_count += ret;

	ret = sprintf(buf+buf_count, " dest ip %d.%d.%d.%d, port: %d ,mac  %pM \n",
				NIPQUAD(pg_test_sets[use_index].dip), pg_test_sets[use_index].dport, pg_test_sets[use_index].dest_mac);
	buf_count += ret;

	ret = sprintf(buf+buf_count, " router ip %d.%d.%d.%d, port: %d ,smac  %pM , dmac %pM\n",
				NIPQUAD(pg_test_sets[use_index].router_ip), pg_test_sets[use_index].router_port, pg_test_sets[use_index].router_src_mac, pg_test_sets[use_index].router_dest_mac);

	buf_count += ret;

	ret = sprintf(buf+buf_count, " svlan:%d, dvlan:%d pppoe_sid 0x%02x proto %d\n",
				 pg_test_sets[use_index].src_vlan, pg_test_sets[use_index].dest_vlan, pg_test_sets[use_index].ppp_sid, pg_test_sets[use_index].proto);

	buf_count += ret;
#else
	ret = sprintf(buf, "hnat basic debug %08x\n", (unsigned int)phnat_priv->base );
#endif


	read = simple_read_from_buffer(user_buf, count, ppos, buf, buf_count);
	return read;
}

 u64 Divided_64(__u64 a, __u64 b) {
	__u64 result = a;

	if(b == 0) {
		return 0;
	}
	do_div(result, b);
	return (u64)result;
}



static void print_help(void){
#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	printk(" echo start > /sys/kernel/debug/hnat_debug \n");
	printk(" echo end > /sys/kernel/debug/hnat_debug \n");
	printk(" intro: would set vaild flag , defalut index 0\n");
	printk(" echo src		[ip] [port] [mac] <cfg_index>\n");
	printk(" echo dest		[ip] [port] [mac] <cfg_index>\n");
	printk(" echo router	[ip] [port] [src_mac] [dest_mac] <cfg_index>\n");
	printk(" echo port		[cfg_index] [src_port]  <router_port> <dest_port>\n");

	// printk(" default vlan is null\n");
	printk(" echo vlan		[src_vlan] [dest_vlan] <cfg_index>\n");

	// printk(" default proto is tcp 0 ppp_sid is null\n");
	printk(" echo other		[proto] [ppp_sid] <cfg_index>\n");

	printk(" demo :echo src	 192.168.6.123 62518 10:16:88:11:22:33 5 >  /sys/kernel/debug/hnat_debug\n");
	printk(" demo :echo dest	 192.168.5.193 5001  10:16:88:33:22:11 5 >  /sys/kernel/debug/hnat_debug\n");
	printk(" demo :echo router 192.168.20.5 6001  10:16:88:11:11:11 10:16:88:33:33:33 5>  /sys/kernel/debug/hnat_debug\n");
	printk(" demo :echo port   5 62518  5001  6001 >  /sys/kernel/debug/hnat_debug\n");
	printk(" demo :echo vlan	 4  3 5 >  /sys/kernel/debug/hnat_debug\n");
	// because the cfg data all save as lan(src) - wan(dest)
	// so need to specify the transmission direction
	// default lan-wan  udp
	// find the file in /etc and update the cfg_index rule
	printk(" intro:setup test set with pkt, use cfg_index without \n other args, clear test set  pkt && valid flag\n");
	printk(" echo pkt [cfg_index] <file_name>  <is_wan_lan> <is_jumbo>\n");
	printk(" echo use [cfg_index]\n");
	printk(" intro:test would use USE cmd cfg_index, default us example pkt,\n if test set valid replace with test set, use cap pkt if exist\n");
	printk(" intro:work_mode 0 tx 1 rx  time unit seconds band width Mbps stop rx if time is 0");
	printk(" echo test [word_mode] [time] [band_width] <is_wan_lan> <is_udp> <mtu> \n");
	printk(" demo: echo test  0 10 20  >  /sys/kernel/debug/hnat_debug\n");
	printk(" demo: echo test  0 10 20 1 1 >  /sys/kernel/debug/hnat_debug\n");

	printk(" intro: /*for recv pkt*/\n");
	printk(" echo test 1 [time] >  /sys/kernel/debug/hnat_debug\n");
	printk(" intro: /*for send special pkt*/\n");
	printk(" echo test 2 [time] [band_width] [pkt_index] >  /sys/kernel/debug/hnat_debug\n");
	printk("set hwnat entry with test set\n");
	printk(" echo rule [cfg_index]\n");
	// stop tx test with key
	printk(" echo stop [key]  >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo rule_full >  /sys/kernel/debug/gmac_debug //rule full entry\n");
	printk(" echo test_full [time] [band_width] <mtu>// send full entry pkt \n");
	printk(" demo: echo test_full 10 10 >  /sys/kernel/debug/hnat_debug\n");
	//when set frag continue, frag no should be 2, ip id no should be 3
	printk(" echo frag_set [frag_no] [ip_id_no] <is_continue> >  /sys/kernel/debug/hnat_debug\n");
	printk(" demo: echo frag_set 2 3 >  /sys/kernel/debug/hnat_debug\n");
#endif

	printk(" echo natmode <mode> >  /sys/kernel/debug/gmac_debug\n");
	printk(" intro: 0-BASIC_MODE, 1-SYMMETRIC_MODE, 2-FULL_CONE_MODE, 3-RESTRICT_CONE_MODE, 4-PORT_RESTRICT_CONE_MODE\n");
	printk(" echo rd_speed >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo readl [addr] [number] >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo writel [addr] [data] >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo tabread [tab_no] [depth] >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo tabwrite [tab_no] [depth] [data(5)] >  /sys/kernel/debug/hnat_debug\n");
	printk(" intro: dump all entry of table and crc table, 1 for napt, 0 for arp\n");
	printk(" echo dump <napt/arp> >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo stat  >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo log mode  >  /sys/kernel/debug/hnat_debug\n");
	printk(" intro: mode:0 for disbable hnat log, mode:1 for enable hnat log\n");
	printk(" echo addifname [is_wan] [index] [ifname] >  /sys/kernel/debug/hnat_debug\n");
	printk(" intro: add interface to monitor ip change. is_wan 0 for lan 1 for wan\n");
	printk(" demo: echo addifname 0 1 br-lan2 > /sys/kernel/debug/hnat_debug\n");
	printk(" echo addlan [index] [addr] [prefix_len] <ifname> >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo addwan [index] [addr] [prefix_len] <ifname> >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo dellan [index] <if_remove_name> >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo delwan [index] <if_remove_name> >  /sys/kernel/debug/hnat_debug\n");
	printk(" echo getlan > /sys/kernel/debug/hnat_debug\n");
	printk(" echo getwan > /sys/kernel/debug/hnat_debug\n");
	printk(" echo enable 0/1 > /sys/kernel/debug/hnat_debug\n");

	return;

}

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
int sf_hnat_debug_write_test_tool(struct sf_hnat_priv *phnat_priv, char str[][18], unsigned char index_arg){

	u8 work_mode = 0;
	u16 key = 0;
	u32  rx_time = 0;
	u8 set_index = 0;
	u8 entry_type = 0;
	u8 select_pkt_index = 0;
	int ret = 0;
	unsigned char dnat_to_host = 0;
	int i = 0;
	struct sf_test_tool_para * ptool_para = NULL;
	u16 tmp_sport= 0, tmp_dport= 0, tmp_rport= 0;
	struct sf_hashkey key_tmp;
	struct sf_hashkey *sf_key = &key_tmp;
	struct sf_test_tool_priv * ptest_priv = phnat_priv->ptest_priv;
	struct sf_rx_test_unit * prx_unit = &ptest_priv->rx_test_unit;
	u8 is_wan_lan = 0, is_jumbo = 0, is_udp = 0;
	unsigned  int napt_index;

	if(strncmp(str[0], "start", 5) == 0){
		ptest_priv->g_start_test = 1;
		printk(" enter test mode\n");
	}
	else if(strncmp(str[0], "end", 3) == 0){
		ptest_priv->g_start_test = 0;
		printk(" exit test mode\n");
	}
	else if(strncmp(str[0], "stop", 3) == 0){
		printk(" stop key test thread\n");
		ret = kstrtou16(str[1], 0 , &key);
		sf_thread_pool_end_test(ptest_priv,key );
	}

	else if(strncmp(str[0], "port", 4) == 0){
		ret = kstrtou8(str[1], 0 , &set_index);
		ret = kstrtou16(str[2], 0 , &pg_test_sets[set_index].sport);
		if(index_arg >= 3){
			ret = kstrtou16(str[3], 0 , &pg_test_sets[set_index].router_port);
		}
		if(index_arg == 4){
			ret = kstrtou16(str[4], 0 , &pg_test_sets[set_index].dport);
		}
	}
	else if(strncmp(str[0], "src", 3) == 0){
		if(index_arg == 4){
			ret = kstrtou8(str[4], 0 , &set_index);
		}
		sf_gmac_test_set_manual(0, set_index, str[1], str[2], str[3], NULL);
	}
	else if(strncmp(str[0], "dest", 4) == 0){
		if(index_arg == 4){
			ret = kstrtou8(str[4], 0 , &set_index);
		}
		sf_gmac_test_set_manual(1, set_index, str[1], str[2], str[3], NULL);
	}
	else if(strncmp(str[0], "router",6) == 0){
		if(index_arg == 5){
			ret = kstrtou8(str[5], 0 , &set_index);
		}
		sf_gmac_test_set_manual(2, set_index, str[1], str[2], str[3], str[4]);
	}
	else if(strncmp(str[0], "vlan",4) == 0){
		if(index_arg == 3){
			ret = kstrtou8(str[3], 0 , &set_index);
		}
		ret = kstrtou16(str[1], 0 , &pg_test_sets[set_index].src_vlan);
		ret = kstrtou16(str[2], 0 , &pg_test_sets[set_index].dest_vlan);
	}
	else if(strncmp(str[0], "other",5) == 0){
		if(index_arg == 3){
			ret = kstrtou8(str[3], 0 , &set_index);
		}
		ret = kstrtou8(str[1], 0 , &pg_test_sets[set_index].proto);
		ret = kstrtou16(str[2], 0 , &pg_test_sets[set_index].ppp_sid);
	}
	else if(strncmp(str[0], "test_full",9) == 0){
		printk("test_full start\n");
		ptest_priv->g_test_tx_pause_count = 0;
		ptool_para = vmalloc(sizeof(struct sf_test_tool_para));
		if(!ptool_para){
			printk("malloc tool para fail \n");
			return 0;
		}
		memset(ptool_para,0, sizeof(struct sf_test_tool_para));
		ret = kstrtou32(str[1], 0 , &ptool_para->time);
		ret = kstrtou16(str[2], 0 , &ptool_para->band_width);
		if(index_arg >=  3  ){
			ret = kstrtou32(str[3], 0 , &ptool_para->mtu);
		}
		sf_thread_test_send_full(ptest_priv, ptool_para);
	}
	else if(strncmp(str[0], "frag_set",5) == 0){
		ret = kstrtou32(str[1], 0 , &test_frag_set.frag_no);
		ret = kstrtou32(str[2], 0 , &test_frag_set.ip_id_no);
		if(index_arg >=  3  ){
			ret = kstrtou8(str[3], 0 , &test_frag_set.is_continus);
		}
		printk("frag number = %d, ip id number = %d, is continue = %d\n",
				test_frag_set.frag_no, test_frag_set.ip_id_no, test_frag_set.is_continus);
	}
	else if(strncmp(str[0], "pkt", 3) == 0){
		ret = kstrtou8(str[1], 0 , &set_index);
		if(index_arg == 1){
			// remove config
			if(pg_test_sets[set_index].pcap_file != NULL){
				vfree(pg_test_sets[set_index].pcap_file);
				pg_test_sets[set_index].pcap_file = NULL;
			}
			pg_test_sets[set_index].valid = 0;
		}
		if(index_arg >= 3){
			if(index_arg == 3)
			  ret = kstrtou8(str[3], 0 , &is_wan_lan);

			if(index_arg >= 4)
			  ret = kstrtou8(str[4], 0 , &is_jumbo);

			sf_gmac_test_set_pkt(set_index,str[2],is_wan_lan,is_jumbo);
		}
	}
	else if(strncmp(str[0], "use",3) == 0){
		ret = kstrtou8(str[1], 0 , &ptest_priv->last_use_index);
		printk("now use index %d\n", ptest_priv->last_use_index);
	}
	else if(strncmp(str[0], "test",4) == 0){

		ret = kstrtou8(str[1], 0 , &work_mode);

		if(work_mode == 0){
			ptest_priv->g_test_tx_pause_count = 0;
			ptool_para = vmalloc(sizeof(struct sf_test_tool_para));
			if(!ptool_para){
				printk("malloc tool para fail \n");
				return 0;
			}
			memset(ptool_para,0, sizeof(struct sf_test_tool_para));
			ret = kstrtou32(str[2], 0 , &ptool_para->time);
			ret = kstrtou16(str[3], 0 , &ptool_para->band_width);
			//remove is_wan_lan because now we use pkt file directly
			if(index_arg >=  4  ){
				ret = kstrtou8(str[4], 0 , &is_wan_lan );
			}

			if(index_arg >=  5  ){
				ret = kstrtou8(str[5], 0 , &is_udp );
			}
			if(index_arg >=  6  ){
				ret = kstrtou32(str[6], 0 , &ptool_para->mtu);
			}

			if(index_arg >=  7  ){
				ret = kstrtou8(str[7], 0 , &test_frag_set.is_frag);
				printk("pkt is frag %d\n", test_frag_set.is_frag);
				printk("frag number = %d, ip id number = %d\n",
							test_frag_set.frag_no, test_frag_set.ip_id_no);
			}
			sf_thread_pool_start_test(ptest_priv, ptest_priv->last_use_index,is_wan_lan, is_udp, ptool_para);

		}else if (work_mode == 1){
			//TODO: change mtu here

			ret = kstrtou32(str[2], 0, &rx_time);
			if(ptest_priv->g_start_test_rx ==  1) {
				if (rx_time != 0){
					printk("rx testing\n");
					return 0;
				}
				else{
					prx_unit->should_end = 1;
					return 0;
				}
			}
			writel(0x1, ptest_priv->hnat_priv->base + 0x4);
			memset(prx_unit , 0 , sizeof(struct sf_rx_test_unit));
			prx_unit->time = rx_time ;
			ptest_priv->g_start_test_rx = 1;

			printk("start rx \n ");
			// start rx
		}else if (work_mode == 2){
			ptest_priv->g_test_tx_pause_count = 0;
			ptool_para = vmalloc(sizeof(struct sf_test_tool_para));
			if(!ptool_para){
				printk("malloc tool para fail \n");
				return 0;
			}
			memset(ptool_para,0, sizeof(struct sf_test_tool_para));
			ret = kstrtou32(str[2], 0 , &ptool_para->time);
			ret = kstrtou16(str[3], 0 , &ptool_para->band_width);
			ret = kstrtou8(str[4], 0 , &select_pkt_index);

			sf_thread_test_select_pkt(ptest_priv, ptool_para, select_pkt_index);

		}else
		  return -1;
	}
	else if(strncmp(str[0], "rule_full",9) == 0){
		// ret = kstrtou8(str[1], 0 , &ptest_priv->last_use_index);
		 set_index = 0;
		 printk("rule full start \n");
		 for(i = 0;i < 1024;i++){
			 memset(sf_key,sizeof(struct sf_hashkey), 0);
			 tmp_sport = htons(pg_test_sets[set_index].sport);
			 tmp_dport = htons(pg_test_sets[set_index].dport);
			 tmp_rport = htons(pg_test_sets[set_index].router_port);

			 memcpy(&sf_key->sip,(void*) &pg_test_sets[set_index].sip,  4);
			 memcpy(&sf_key->sport,(void*) &tmp_sport,  2);
			 memcpy(&sf_key->dip,(void*) &pg_test_sets[set_index].dip,  4);
			 memcpy(&sf_key->dport, (void*) &tmp_dport, 2);
			 memcpy(&sf_key->router_pub_ip, (void*) &pg_test_sets[set_index].router_ip, 4);
			 memcpy(&sf_key->router_port, (void*) &tmp_rport, 2);

			 memcpy(sf_key->src_mac, pg_test_sets[set_index].src_mac,6);
			 memcpy(sf_key->dest_mac, pg_test_sets[set_index].dest_mac,6);
			 memcpy(sf_key->router_src_mac, pg_test_sets[set_index].router_src_mac,6);
			 memcpy(sf_key->router_dest_mac,pg_test_sets[set_index].router_dest_mac,6);
			 sf_key->src_vlan = pg_test_sets[set_index].src_vlan;
			 sf_key->dest_vlan = pg_test_sets[set_index].dest_vlan;
			 sf_key->ppp_sid = pg_test_sets[set_index].ppp_sid;
			 sf_key->proto = pg_test_sets[set_index].proto;
			 sf_key->valid = 1;
			 if(sf_key->ppp_sid != 0)
				 sf_key->cur_pppoe_en = 1;
			 else
				 sf_key->cur_pppoe_en = 0;

			 sf_key->dnat_to_host = 0;
			 printk("the send to host %d\n", sf_key->dnat_to_host);
			 printk("the entry type %d\n", entry_type);

			 sf_hnat_add_entry(phnat_priv,sf_key,&napt_index);
			 pg_test_sets[set_index].sport += 1;
			 pg_test_sets[set_index].dport += 1;
			 pg_test_sets[set_index].router_port += 1;
			 if((i > 1) && (i % 2 == 1) ){
				 pg_test_sets[set_index].sip = htonl(ntohl(pg_test_sets[set_index].sip) + 1);
			 }
			 if(i % 16 == 1){
				 memcpy(pg_test_sets[set_index].dest_mac,(void*) &pg_test_sets[set_index].dip, 4);
				 memcpy(pg_test_sets[set_index].src_mac,(void*) &pg_test_sets[set_index].sip, 4);
			 }
		}
		 printk("rule full end\n");
	}
	else if(strncmp(str[0], "rule",4) == 0){
		// ret = kstrtou8(str[1], 0 , &ptest_priv->last_use_index);
		 ret = kstrtou8(str[1], 0 , &set_index);
		if(index_arg == 2){
			ret = kstrtou8(str[2], 0 , &entry_type);
		}
		if(index_arg == 3){
			ret = kstrtou8(str[3], 0 , &dnat_to_host);
		}
		memset(sf_key,sizeof(struct sf_hashkey), 0);
		tmp_sport = htons(pg_test_sets[set_index].sport);
		tmp_dport = htons(pg_test_sets[set_index].dport);
		tmp_rport = htons(pg_test_sets[set_index].router_port);

		memcpy(&sf_key->sip,(void*) &pg_test_sets[set_index].sip,  4);
		memcpy(&sf_key->sport,(void*) &tmp_sport,  2);
		memcpy(&sf_key->dip,(void*) &pg_test_sets[set_index].dip,  4);
		memcpy(&sf_key->dport, (void*) &tmp_dport, 2);
		memcpy(&sf_key->router_pub_ip, (void*) &pg_test_sets[set_index].router_ip, 4);
		memcpy(&sf_key->router_port, (void*) &tmp_rport, 2);

		memcpy(&sf_key->src_mac, pg_test_sets[set_index].src_mac,6);
		memcpy(&sf_key->dest_mac, pg_test_sets[set_index].dest_mac,6);
		memcpy(&sf_key->router_src_mac, pg_test_sets[set_index].router_src_mac,6);
		memcpy(&sf_key->router_dest_mac,pg_test_sets[set_index].router_dest_mac,6);
		sf_key->src_vlan = pg_test_sets[set_index].src_vlan;
		sf_key->dest_vlan = pg_test_sets[set_index].dest_vlan;
		sf_key->ppp_sid = pg_test_sets[set_index].ppp_sid;
		sf_key->proto = pg_test_sets[set_index].proto;
		sf_key->valid = 1;
		if(sf_key->ppp_sid != 0)
			sf_key->cur_pppoe_en = 1;
		else
			sf_key->cur_pppoe_en = 0;

		if(dnat_to_host)
			sf_key->dnat_to_host = 1;
		else
			sf_key->dnat_to_host = 0;
		printk("the send to host %d\n", sf_key->dnat_to_host);
		printk("the entry type %d\n", entry_type);

		if(entry_type)
			sf_hnat_delete_entry(phnat_priv,sf_key);
		else
			sf_hnat_add_entry(phnat_priv,sf_key,&napt_index);
	}
	else
	  return -1;

	return 0;
}
#endif

long sf_hnat_debug_ioctl(struct file *file, unsigned int a, unsigned long b) {
	// struct sf_hnat_priv *phnat_priv = (struct sf_hnat_priv *)file->private_data;
	// printk("[hnat info]Now wan is up, vldclean\n");
	// sf_hnat_napt_vld_clean(phnat_priv, 0);
	return 0;
}

ssize_t sf_hnat_debug_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos) {
	struct sf_hnat_priv *phnat_priv = (struct sf_hnat_priv *)file->private_data;
	unsigned char ret = 0, last_i = 0, index_arg = 0;
	char str[8][18] = {'\0'};
	char buf[128] = {0};
	int i = 0;
	unsigned int value = 0;
	u8 nat_mode = 0;
	u8 dump_type = 0;
	u16 dump_index = 0;

	unsigned char mode;
	unsigned int prefix_len;
	unsigned char lan_index;
	unsigned char wan_index;
	unsigned char is_wan;
	unsigned char index;
	unsigned char remove_name = 0;
	int tab_no,depth,data[7];
	unsigned int addr,reg_data;
	int num=0;
	size_t len = min_t(size_t, count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
	  return -EFAULT;

	for(;i < len;i++){
		if(buf[i] == ' '){
			memcpy(str[index_arg], buf + last_i, i - last_i);
			// printk("arg[%d] %s ",index_arg,str[index_arg]);
			last_i=(i + 1);
			index_arg++;
		}
	}
	memcpy(str[index_arg], buf + last_i, count - last_i);

	if(index_arg > 7)
	  goto err_parsing;
	// printk("arg[%d]  %s \n",index_arg, str[index_arg]);
	if(strncmp(str[0], "help",4) == 0){
		print_help();
		return count;
	}
	if(strncmp(str[0], "udp_check",9) == 0){
		ret = kstrtou32(str[1], 0, &value);
		sf_hnat_udp_check = value;
		return count;
	}
	if(strncmp(str[0], "enable",6) == 0){
		ret = kstrtou32(str[1], 0, &value);
		sf_hnat_enable = value;
		if (value == 0) {
			sf_hnat_napt_vld_clean();
			printk("disable sf_hnat successfully\n");
		}
		else
			printk("enable sf_hnat successfully\n");
		return count;
	}
	if(strncmp(str[0], "rd_speed ",8) == 0){
		sf_hnat_speed_read(phnat_priv);
		return count;
	}
	if(strncmp(str[0], "tabread",4) == 0){

		ret = kstrtou32(str[1], 0 , &tab_no);
		ret = kstrtou32(str[2], 0 , &depth);
		sf_hnat_table_rd(phnat_priv, tab_no,depth,data);
		printk("read table %d, depth %d:\n", tab_no, depth);
		for(i = 0;i < 7;i++){
			printk("data%d = 0x%x\n", i, data[i]);
		}
		return count;
	}
	if(strncmp(str[0], "tabwrite",4) == 0){

		ret = kstrtou32(str[1], 0 , &tab_no);
		ret = kstrtou32(str[2], 0 , &depth);
		for(i = 0;i < 5;i++){
			ret = kstrtou32(str[i+3], 0 , &data[i]);
		}
		sf_hnat_table_wr(phnat_priv, tab_no, depth, data);
		return count;
	}
	if(strncmp(str[0], "readl",5) == 0){

		ret = kstrtou32(str[1], 0 , &addr);
		ret = kstrtou32(str[2], 0 , &num);

		printk("readl:\n");
		for(;num > 0;num-=4){
			printk("addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x  addr 0x%x:data 0x%08x", addr, readl((volatile void *)addr), addr+4, readl((volatile void *)addr+4), addr+8, readl((volatile void *)addr+8), addr+12, readl((volatile void *)addr+12));
			addr += 16;
		}
		return count;
	}
	if(strncmp(str[0], "writel",6) == 0){

		ret = kstrtou32(str[1], 0 , &addr);
		ret = kstrtou32(str[2], 0 , &reg_data);

		writel(reg_data, (volatile void *)addr);
		printk("write addr 0x%x: data 0x%x\n", addr, reg_data);
		return count;
	}
	else if(strncmp(str[0], "dump",4) == 0){
		ret = kstrtou8(str[1], 0 , &dump_type);
		ret = kstrtou16(str[2], 0 , &dump_index);
		printk("#current napt num %d tcp %d udp %d\n", phnat_priv->curr_napt_num, phnat_priv->curr_napt_tcp_num, phnat_priv->curr_napt_udp_num);
		printk("# hash full  %d dip hash full   %d add fail %d update_flow %d  crc_clean %d\n", napt_hash_full_count,dip_hash_full_count, add_fail_count,update_flow_count, crc_clean_flow_count);
		sf_hnat_hw_dump_count();
		if(dump_type == 1){
			for(i = 0;i < 1024;i++){
				sf_dump_napt_table_content_by_index(phnat_priv,i);
			}
		}
		else if (dump_type == 2){
			for(i = 0;i < 512;i++){
				sf_dump_dip_table_content_by_index(phnat_priv,i);
			}
		}
		else if (dump_type == 3){
			sf_dump_napt_table_content_by_index(phnat_priv,dump_index);
		}
		else if (dump_type == 4){
			sf_hnat_dump_flow(phnat_priv);
		}
		else if (dump_type == 5){
			for(i = 0;i < 512;i++){
				 sf_hnat_dump_dip_by_index(phnat_priv, i);
				if(dump_index == 1){
					sf_hnat_dump_dip_hash_by_index(i);
				}
			}
		}
		else if (dump_type == 0){
			for(i = 0;i < 1024;i++){
				sf_hnat_dump_napt_key_by_index(i);
				if(dump_index == 1){
					sf_hnat_dump_napt_hash_by_index(i);
				}
			}
		}
	}

	else if(strncmp(str[0], "natmode",7) == 0){
		ret = kstrtou8(str[1], 0 , &nat_mode);
		if(nat_mode == 0){
			sf_hnat_change_nat_mode(phnat_priv,BASIC_MODE);
			printk("nat_mode = BASIC_MODE\n");
		}else if(nat_mode == 1){
			sf_hnat_change_nat_mode(phnat_priv,SYMMETRIC_MODE);
			printk("nat_mode = SYMMETRIC_MODE\n");
		}else if(nat_mode == 2){
			sf_hnat_change_nat_mode(phnat_priv,FULL_CONE_MODE);
			printk("nat_mode = FULL_CONE_MODE\n");
		}else if(nat_mode == 3){
			sf_hnat_change_nat_mode(phnat_priv,RESTRICT_CONE_MODE);
			printk("nat_mode = RESTRICT_CONE_MODE\n");
		}else if(nat_mode == 4){
			sf_hnat_change_nat_mode(phnat_priv,PORT_RESTRICT_CONE_MODE);
			printk("nat_mode = PORT_RESTRICT_CONE_MODE\n");
		}

	}
	else if(strncmp(str[0], "stat",4) == 0){

		sf_hnat_read_count_registers(phnat_priv);
	}
	else if(strncmp(str[0], "log",3) == 0){
		ret = kstrtou8(str[1], 0 , &mode);
		if(mode){
			g_print = mode;
		}else{
			g_print = 0;
		}
	}
	else if(strncmp(str[0], "addifname",9) == 0){
		ret = kstrtou8(str[1], 0 , &is_wan);
		ret = kstrtou8(str[2], 0 , &index);
		if(index < 8 && index >= 0) {
			if(is_wan){
				strncpy(phnat_priv->wan_subnet[index].ifname, str[3], strlen(str[3]));
				phnat_priv->wan_subnet[index].ifname[strlen(str[3]) - 1] = '\0';
				printk("[hnat info] ifname %s, index %d  is wan %d \n", phnat_priv->wan_subnet[index].ifname, index,is_wan);
			}
			else{
				strncpy(phnat_priv->lan_subnet[index].ifname, str[3], strlen(str[3]));
				phnat_priv->lan_subnet[index].ifname[strlen(str[3]) - 1] = '\0';
				printk("[hnat info] ifname %s, index %d  is wan %d\n", phnat_priv->lan_subnet[index].ifname, index,is_wan);
			}
		}else
		  printk("[hnat info]  index error %d \n", index);
	}
	else if(strncmp(str[0], "addwan",6) == 0){
		ret = kstrtou8(str[1], 0 , &wan_index);
		ret = kstrtou32(str[2], 0 , &addr);
		ret = kstrtou32(str[3], 0 , &prefix_len);
		if(wan_index < 8 && wan_index >= 0) {
			phnat_priv->wan_subnet[wan_index].ipaddr = addr;
			phnat_priv->wan_subnet[wan_index].prefix_len = prefix_len;
			phnat_priv->wan_subnet[wan_index].valid = 1;
			if(index_arg == 4){
			  strncpy(phnat_priv->wan_subnet[wan_index].ifname, str[4],  strlen(str[4]));
			  phnat_priv->wan_subnet[wan_index].ifname[strlen(str[4]) - 1] = '\0';
			  printk("[hnat info]get wan name %s,ip %08x pf %d  \n", phnat_priv->wan_subnet[wan_index].ifname, addr, prefix_len);
			}
		}
	}
	else if(strncmp(str[0], "delwan",6) == 0){
		ret = kstrtou8(str[1], 0 , &wan_index);
		if(index_arg == 2)
		  ret = kstrtou8(str[2], 0 , &remove_name);
		if(wan_index < 8 && wan_index >= 0) {
			if(remove_name)
			  phnat_priv->wan_subnet[wan_index].ifname[0] = '\0';
			sf_hnat_del_wan_subnet(phnat_priv, wan_index);
		}
	}
	else if(strncmp(str[0], "addlan",6) == 0){
		ret = kstrtou8(str[1], 0 , &lan_index);
		ret = kstrtou32(str[2], 0 , &addr);
		ret = kstrtou32(str[3], 0 , &prefix_len);
		if(lan_index < 8 && lan_index >= 0) {
			phnat_priv->lan_subnet[lan_index].ipaddr = addr;
			phnat_priv->lan_subnet[lan_index].prefix_len = prefix_len;

			if(index_arg == 4){
			  strncpy(phnat_priv->lan_subnet[lan_index].ifname, str[4],  strlen(str[4]));
			  phnat_priv->lan_subnet[lan_index].ifname[strlen(str[4]) - 1] = '\0';
			  printk("[hnat info]get wan name %s,ip %08x pf %d  \n", phnat_priv->lan_subnet[wan_index].ifname, addr, prefix_len);
			}
			sf_hnat_update_lan_subnet(phnat_priv, lan_index);
		}
	}
	else if(strncmp(str[0], "dellan",6) == 0){
		ret = kstrtou8(str[1], 0 , &lan_index);
		if(index_arg == 2)
		  ret = kstrtou8(str[2], 0 , &remove_name);
		if(lan_index < 8 && lan_index >= 0) {
			if(remove_name)
			  phnat_priv->lan_subnet[lan_index].ifname[0] = '\0';
			sf_hnat_del_lan_subnet(phnat_priv, lan_index);
		}
	}
	else if(strncmp(str[0], "vldclean",8) == 0){
		printk("[hnat info] hnat rule flush\n");
		sf_hnat_napt_vld_clean();
	}
 //==== for test: get all lan ip and mask / get wanmask / get ppphd_entry status
	else if(strncmp(str[0], "getlan",6) == 0){
		for(i = 0; i < 8; i++){
			if(phnat_priv->lan_subnet[i].valid)
			  printk("[hnat info] lan index %d; ip %08x; prefix_len %d ifname %s\n", i, phnat_priv->lan_subnet[i].ipaddr, phnat_priv->lan_subnet[i].prefix_len,phnat_priv->lan_subnet[i].ifname);
		}
	}
	else if(strncmp(str[0], "getwan",6) == 0){
		for(i = 0; i < 8; i++){
			if(phnat_priv->wan_subnet[i].valid)
			  printk("[hnat info] wan index %d; ip %08x; prefix_len %d ifname %s  vld %d\n", i, phnat_priv->wan_subnet[i].ipaddr, phnat_priv->wan_subnet[i].prefix_len, phnat_priv->wan_subnet[i].ifname, phnat_priv->wan_subnet[i].valid);
		}
	}
	else if(strncmp(str[0], "ppphddump",9) == 0){
		ret = sf_hnat_dump_ppphd(phnat_priv);
	}
//==== test end

	else{
#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
		if(sf_hnat_debug_write_test_tool(phnat_priv, str, index_arg) == 0){
			return count;
		}
#endif
	  goto err_parsing;
	}

	return count;
err_parsing:
	printk("parsing Error %s nat debug,lease check your input!, echo help get notice\n", str[0]);
	return count;
}


#endif
