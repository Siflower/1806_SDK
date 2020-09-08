/*************************************************************************
	> File Name: sf_debug.h
	> Author: Qin xia
	> Mail: qin.xia@siflower.com.cn
	> Created Time: 2017年01月19日 星期四 09时46分38秒
 ************************************************************************/

#include <linux/netdevice.h>
#include <linux/kernel.h>
#ifdef CONFIG_SFAX8_SWITCH_API
#include "sf_api.h"
#endif
#include "sf_eth.h"
#include "sf_debug.h"
#include "sf_mdio.h"
#ifdef CONFIG_SFAX8_SWITCH_VLAN
#include "sf_switch.h"
#endif
#include "sf_eth_addr.h"

int g_user_port = 0;
unsigned long g_user_speed = 0;
extern struct sf_priv * g_sf_priv;
extern int eth_reset_count;
extern int tx_q_stop_count;
extern int skb_al_fail_count;

extern int sf_eth_reset(struct sf_priv *priv);
#ifdef SF_SPEED_BANLANCE
extern void gpi_ing_shaper_init(int limit);
#endif
int g_eth_init_debug = 1;
int g_switch_init_debug = 1;
int g_eth_poll_debug = 0;
int g_eth_tx_debug = 0;
int g_eth_rx_debug = 0;
int g_eth_irq_debug = 0;
int g_switch_debug = 0;
int g_eth_reset_debug = 0;
int g_i_tx_wake_time= 5;

void  print_msg(int mode, struct net_device *netdev, const char* fmt, ...)
{
	char *module_info = NULL;
	char msg[1000] = {0};
	va_list args;

	switch (mode) {
		case ETH_INIT_DEBUG:
			module_info = "INIT_INFO: ";
			if(!g_eth_init_debug)
			  return;
			break;

		case ETH_POLL_DEBUG:
			module_info = "ETH_POLL_INFO: ";
			if(!g_eth_poll_debug)
			  return;
			break;

		case ETH_TX_DEBUG:
			module_info = "ETH_TX_INFO: ";
			if(!g_eth_tx_debug)
			  return;
			break;

		case ETH_RX_DEBUG:
			module_info = "ETH_RX_INFO: ";
			if(!g_eth_rx_debug)
			  return;
			break;

		case ETH_IRQ_DEBUG:
			module_info = "ETH_IRQ_INFO: ";
			if(!g_eth_irq_debug)
			  return;

		case SWITCH_INIT_DEBUG:
			module_info = "SWITCH_INT_INFO: ";
			if(!g_switch_init_debug)
			  return;
			break;

		case SWITCH_DEBUG:
			module_info = "SWITCH_INFO: ";
			if(!g_switch_debug)
			  return;
			break;

		case ETH_RESET_DEBUG:
			module_info = "ETH_RESET_INFO: ";
			if(!g_eth_reset_debug)
			  return;
			break;

		default:
			return;
	}

	va_start(args, fmt);
	vsprintf(msg, fmt, args);
	netdev_info(netdev,"%s%s",module_info,msg);
	va_end(args);

}

#ifdef CONFIG_DEBUG_FS
int sfax8_npu_debug_show(struct seq_file *m, void *v)
{
	seq_printf(m,"npu debug interface\n");
	return 0;
}

int sfax8_npu_debug_open(struct inode *inode, struct file *file)
{
	return single_open(file, sfax8_npu_debug_show, NULL);
}

int sfax8_npu_counts_show(struct seq_file *m, void *v)
{
	seq_printf(m,"npu counts interface\n");
	return 0;
}
int sfax8_npu_counts_open(struct inode *inode, struct file *file)
{
	return single_open(file, sfax8_npu_counts_show, NULL);
}

#ifdef CONFIG_SFAX8_SWITCH_API
static int help =0;
ssize_t sfax8_npu_debug_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	int enable = 0, ret = 0;

	if(*ppos > 0)
	  return 0;

	if(!help){
		ret = sprintf(buf, "%s\n", "\nHelp: Special function for NPU \
					\nIn the sys/kernel/debug/ directories you can use 'echo xx > npu_debug' to choose which function to load \
					\nlist for function: \
					\n00 -- set global snoop special mac \
					\n01 -- init global cutthru etype \
					\n02 -- enable dos filter \
					\n03 -- set dscp rule \
					\n04 -- set global management ether type \
					\n05 -- set port enable and set port vlan attribute \
					\n06 -- set global snoop ethertype \
					\n07 -- set port untag \
					\n08 -- set valid prefix \
					\n09 -- set tmu various configure \
					\n0a -- set GPI ingress shaper configure \
					\n0b -- set GPI flowcontrol \
					\n0c -- set mac addr filter"
					);
		*ppos += ret;
		return ret;
	}
	else{
		enable = help - 1;
	}
	switch (enable) {
		case SET_SPL_MAC:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 00 00 01 3960c2000000 fffffffffff0 > write \
						\n00 -- @enable this function \
						\n00 -- @index decide which mac addr register to choose, 0 for reg1, 1 for reg2 \
						\n01 -- @ENABLE bit for snoop control register, 1 for enable, 0 for shutdown, always set to 1 \
						\n3960c2000000 -- @mac mac addr \
						\nfffffffffff0 -- @mask mask value \
						\ncomments:you can call this function twice at most"
						);
			break;

		case INIT_GLB_CUTTHRU:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 01 99 > write \
						\n01 -- @enable this function \
						\n99 -- @cutthru_type cutthru type \
						\ncomments:port always set to 0x3f"
						);
			break;

		case SET_DOS_FIL:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 02 00 01 > write \
						\n02 -- @enable this function \
						\n00 -- @item decide dos filter type \
						\n01 -- @value value set to register \
						\nsuch as item 00 -- DOS_ENABLE -- value --01 \
						\n        item 01 -- DOS_FILTER_ENABLE -- value --00 -- IPV4_SIP_EQ_DIP_EN \
						\n                                                01 -- IPV4_ICMP_FRAG_EN \
						\n                                                02 -- IPV4_ICMP_PING_TOO_BIG_EN \
						\n                                                03 -- IPV4_UDP_SP_EQ_DP_EN \
						\n                                                04 -- IPV4_TCP_FLAG_CHK \
						\n                                                05 -- IPV4_TCP_SP_EQ_DP \
						\n                                                06 -- IPV4_TCP_FRAG_OFF1 \
						\n                                                07 -- IPV4_TCP_SYN_ERR \
						\n                                                08 -- IPV4_TCP_XMAS \
						\n                                                09 -- IPV4_TCP_NULL \
						\n        item 02 -- DOS_COMB_FILTER_ENABLE -- value -- 00 -- COMB_FILTER_1 \
						\n                                                      01 -- COMB_FILTER_2 \
						\n                                                      02 -- COMB_FILTER_3 \
						\n                                                      03 -- COMB_FILTER_4 \
						\n        item 03 -- DOS_COMB_FILTER_VALUE -- value \
						\n        item 04 -- DOS_ICMPV4_MAX_LEN -- value \
						\ncomments:the value of IPV6 is omitted in DOS_FILTER_ENABLE,and you can call this function many times"
						);
			break;

		case SET_DSCP_RULE:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 03 00 > write \
						\n03 -- @enable this function \
						\n00 -- @port port number"
						);
			break;

		case SET_GLB_MGMT_ETYE:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 04 00 8100 > write \
						\n04   -- @enable this function \
						\n00   -- @port port number \
						\n8100 -- @mgmt_etype global management ether type"
						);
			break;

		case SET_PORT_CTL:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 05 00 01 02 > write \
						\n05 -- @enable this function \
						\n00 -- @port port number \
						\n01 -- @ENABLE 0 for shutdown, 1 for enable \
						\n02 -- @attr set port vlan attribute \
						\n      00 -- ANY_TAGGING \
						\n      01 -- TAGGED_ONLY \
						\n      02 -- UNTAGGED_ONLY"
						);
			break;

		case SET_SNOOP_ETYE:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
						\nExample:echo 06 08 01 8623 > write \
						\n06 -- @enable this function \
						\n08 -- @idx decide snoop ethertype module \
						\n      04 -- SNOOP_IGMP \
						\n      05 -- SNOOP_MLDV2 \
						\n      08 -- SNOOP_ETYPE0 \
						\n      09 -- SNOOP_ETYPE1 \
						\n      0a -- SNOOP_ETYPE2 \
						\n      0b -- SNOOP_ETYPE3 \
						\n      0c -- SNOOP_ETYPE_ARP \
					\n      0d -- SNOOP_ETYPE_RARP \
					\n      0e -- SNOOP_HOPOPTS \
					\n01 -- @ENABLE 0 for shutdown, 1 for enable, always set to 1 \
					\n8623 -- @snoop_etype decide snoop ethertype for SNOOP_ETYPE0/1/2/3,it is NULL for others"
					);
			break;

		case SET_PORT_UNTAG:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
					\nExample:echo 07 05 > write \
					\n07 -- @enable this function \
					\n05 -- @port port number"
					);
			break;

		case SET_VID_PFIX:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
					\nExample:echo 08 00 > write \
					\n08 -- @enable this function \
					\n00 -- @port port number"
					);
			break;

		case SET_TMU_CFG:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
                    \nExample:echo 09 04 03fe 01 3ff000 00 64 01 02 64 3c 0a 14 1e 28 32 3c 46 50 3f > write \
					\n09 -- @enable this function\n \
					\n***for schduler set*** \
					\n04 -- @tsch.tlsd_algo \
					\n      00 -- Priority queue \
					\n      02 -- DWRR \
					\n      03 -- Round Robin \
					\n      04 -- WRR \
					\n03fe -- @tsch.tlsd_weight \
					\n01 -- @tsch.tlsd_rate_units; 0 - data rate 1- packet rate\n \
					\n***for shaper set*** \
					\n3ff000 -- @tshp.tlsp_max_credit Maximum value of the credit that can accumulated per shaper \
					\n00 -- @tshp.tlsp_rate_units 0 - data rate 1- packet rate \
					\n64 -- @tshp.tlsp_rate Data rate for a given shaper packets per second or bits per second (Mbps) \
					\n01 -- @tshp.tlsp_mode mode 0 for normal mode and 1 for credit mode\n \
					\n***for class queue set*** \
					\n02 -- @tque.tlcq_qmgmt 0 for NO_MGMT,1 for TMU_QUE_TAIL_DROP and 2 for TMU_QUE_WRED \
					\n64 -- @tque.tlcq_max_len \
					\n3c -- @tque.tlcq_min_len \
					\n0a -- @tque.tlcq_prob_cfg0 If queue management is WRED,configure probability value cfg0 (max value 255) \
					\n14 -- @tque.tlcq_prob_cfg1 as before \
					\n1e -- @tque.tlcq_prob_cfg2 as before \
					\n28 -- @tque.tlcq_prob_cfg3 as before \
					\n32 -- @tque.tlcq_prob_cfg4 as before \
					\n3c -- @tque.tlcq_prob_cfg5 as before \
					\n46 -- @tque.tlcq_prob_cfg6 as before \
					\n50 -- @tque.tlcq_prob_cfg7 as before \
					\n3f -- @port_list \
					\ncomment: If queue management is TMU_QUE_TAIL_DROP,the last will not be configured"
					);
			break;

		case SET_GPI_INGRS_CFG:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
					\nExample:echo 0a 03ff00 00 64 00 09 > write \
					\n0a -- @enable this function \
					\n03ff00 -- @tshp.tlsp_max_credit Maximum value of the credit that can accumulated per shaper \
					\n00 -- @tshp.tlsp_rate_units 0 - data rate 1- packet rate \
					\n64 -- @tshp.tlsp_rate Data rate for a given shaper packets per second or bits per second (Mbps) \
					\n00 -- @dev_no device number \
					\n09 -- @effect Bits [1:0] value 00 Shaper0 is for port level data rate, value 01 Shaper0 is for BCAST Packets, value 10 Shaper0 is for MCAST Packets\
					\n              Bits [3:2] value 00 Shaper1 is for port level data rate, value 01 Shaper1 is for BCAST Packets, value 10 Shaper1 is for MCAST Packets"
					);
			break;

		case SET_GPI_FLW_CTL:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
					\nExample:echo 0b 00 0a 14 32 64>write \
					\n0b -- @enable this function \
					\n00 -- @dev_no device number \
					\n0a -- @bmu_low_watermark \
					\n14 -- @bmu_high_watermark \
					\n32 -- @phy_low_watermark \
					\n64 -- @phy_high_watermark"
					);
			break;

		case SET_MAC_ADDR_FIL:
			ret = sprintf(buf, "%s\n", "\nUsage:you can use 'echo xx > npu_debug' to pass parameters to this function \
					\nExample:echo 0c 3960c2000000 > write \
					\n0c -- @enable this function \
					\n3960c2000000 -- @mac mac addr"
					);
			break;

		default:
			ret = sprintf(buf, "%s\n", "input error");
	}

	*ppos += ret;
	return ret;
}

/*calling the write node will execute following function.
 * @buf a pointer to the buf which we input,we transfer the buf to arg[20] (2 bits of buf corresponding to 1 bit of arg[20]).
 * The arg[20] contain arguments we need;
 * arg[0] == enable; decide which module to load;
 * @count length of buf
 *
 * */
ssize_t sfax8_npu_debug_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	int i = 0, last_i = 0, index_arg = 0, ret = 0, enable = 0;
	char buf[256] = {0};
	char tmp_str[13] = {'\0'};
	unsigned long long arg[20] = {0};
	tmu_schd_params_t tsch;
	tmu_shpr_params_t tshp;
	tmu_classq_params_t tque;

	size_t len = min_t(size_t, count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
	  return -EFAULT;

	for(i = 0; i < len; i++){
		if(buf[i] == ' '){
			memcpy(tmp_str, buf + last_i, i - last_i);
			ret = kstrtoull(tmp_str, 16, &arg[index_arg]);
			if(ret == -EINVAL)
			  goto err_parsing;
			else if(ret == -ERANGE)
			  goto err_overflow;

			memset(tmp_str, '\0', sizeof(tmp_str));
			last_i = i + 1;
			index_arg++;
		}
	}

	ret = kstrtoull(buf + last_i, 16, &arg[index_arg]);
	if(ret == -EINVAL)
	  goto err_parsing;
	else if(ret == -ERANGE)
	  goto err_overflow;

	enable = arg[0];

	if(len == 3){
		help = enable + 1;
		printk("you can use 'cat npu_debug'  again to get more information about how to use this function\n");
		return count;
	}

	/*Switch to select function module*/
	switch (enable) {
		case SET_SPL_MAC:
			/*punt special mac packet to host*/
			set_global_snoop_special_mac(arg[1], (arg+2), (arg+3), arg[4]);
			break;

		case INIT_GLB_CUTTHRU:
			/*init global cutthru list
			 *indicates etype/tpid will be rewritten on some egress ports */
			set_cutthru(0x3f, arg[1]);
			break;

		case SET_DOS_FIL:
			/*enable dos filter*/
			set_dos_value(arg[1], arg[2]);
			break;

		case SET_DSCP_RULE:
			// pcp0 ==> map bit 0:2 ==> tc 0
			set_pcp2tc_map(arg[1],0xFAC688);
			//port 0 => 0 port 6 ==6
			set_pid2tc_map(0xFAC688);
			set_tc_sel_one_to_one(arg[1], TC_MACDA);
			break;

		case SET_GLB_MGMT_ETYE:
			/*set global management ether type*/
			set_global_mgmt_port(arg[1], arg[2]);
			break;

		case SET_PORT_CTL:
			/*set port enable && set port vlan attribute*/
			set_port_enable(arg[1], arg[2]);
			set_port_vlan_attr(arg[1], arg[3]);
			break;

		case SET_SNOOP_ETYE:
			//set to reg 1
			set_global_snoop_ethertype(arg[1],(arg+2),arg[3]);
			break;

		case SET_PORT_UNTAG:
			set_port_untag(arg[1]);
			break;

		case SET_VID_PFIX:
			set_port_vid_prefix(arg[1]);
			break;

		case SET_TMU_CFG:
			//for schduler set
			tsch.tlsd_algo = arg[1];
			tsch.tlsd_weight = arg[2];
			tsch.tlsd_rate_units = arg[3];
			// for shaper set
			tshp.tlsp_max_credit = arg[4];
			tshp.tlsp_rate_units = arg[5];
			tshp.tlsp_clk_frequency = 250; //mhz
			tshp.tlsp_rate = arg[6] * 1000; /* kbps */
			tshp.tlsp_mode = arg[7]; /*0x1-credit based mode */
			//for class queue set
			tque.tlcq_qmgmt	= arg[8];
			tque.tlcq_max_len = arg[9];
			tque.tlcq_min_len = arg[10];
			if(tque.tlcq_qmgmt == TMU_QUE_WRED) {
				tque.tlcq_prob_cfg0 = arg[11];
				tque.tlcq_prob_cfg1 = arg[12];
				tque.tlcq_prob_cfg2 = arg[13];
				tque.tlcq_prob_cfg3 = arg[14];
				tque.tlcq_prob_cfg4 = arg[15];
				tque.tlcq_prob_cfg5 = arg[16];
				tque.tlcq_prob_cfg6 = arg[17];
				tque.tlcq_prob_cfg7 = arg[18];
			}
			else {
				tque.tlcq_qmgmt = TMU_QUE_TAIL_DROP;
			}

			set_tmu_various_cfg(&tsch, &tshp, &tque, arg[19]);
			break;

		case SET_GPI_INGRS_CFG:
			tshp.tlsp_max_credit = arg[1];
			tshp.tlsp_rate_units = arg[2];
			tshp.tlsp_clk_frequency = 250; //mhz
			tshp.tlsp_rate = arg[3] * 1000; /* kbps */
			npu_gpi_ingress_config(&tshp,arg[4],arg[5]);
			break;

		case SET_GPI_FLW_CTL:
			set_bmu_watermark(arg[2], arg[3]);
			init_gpi_fw_control(arg[1], arg[4], arg[5]);
			break;

		case SET_MAC_ADDR_FIL:
			sf_writel(arg[1], REG_EMAC_FILTER(0)); //set_SA_DA_model
			gmac_set_mac_addr1(arg+2);
			break;

		default:
			break;
	}
	printk("from user,length=%d,content=%s\n", len, buf);
	return count;

err_parsing:
	printk("parsing Error,please check your input!\n");
err_overflow:
	printk("convert overflow,please check your input!\n");

	return count;
}
#else

ssize_t sfax8_npu_debug_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	int i, port_status, ret = 0;
	char buf[128] = {0};
	size_t read;

	ret = sprintf(buf, "check phy link status\n");
	for(i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		port_status = check_phy_linkup(i, g_sf_priv, 1);
		ret += sprintf(buf+ret, "phy%d    status %d\n", i, port_status);
	}

	read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
	return read;
}

ssize_t sfax8_npu_debug_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long addr = 0, value = 0, i = 0, j = 0, ret = 0, last_i = 0, index_arg = 0;
	struct bd *temp_bd;
	char str[5][11] = {'\0'};
	char buf[128] = {0};

	size_t len = min_t(size_t, count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
	  return -EFAULT;

	for(;i < len;i++){
		if(buf[i] == ' '){
			memcpy(str[index_arg], buf + last_i, i - last_i);
			last_i=(i + 1);
			index_arg++;
		}
	}
	memcpy(str[index_arg], buf + last_i, count - last_i);
	// printk("str1 %s str2 %s str3 %s str4 %s\n",str[0],str[1],str[2],str[3]);
	ret = kstrtoul(str[0], 0, &addr);
	if(ret == -EINVAL)
	  goto err_parsing;
	else if(ret == -ERANGE)
	  goto err_overflow;

	switch (addr)
	{
		case 0:
			g_eth_tx_debug = 1;
			break;

		case 1:
			g_eth_rx_debug = 1;
			break;

		case 2:
			g_eth_irq_debug = 1;
			break;

		case 3:
			g_switch_debug = 1;
			break;

		case 4:
			g_eth_poll_debug = 1;
			break;

		case 5:
			g_eth_tx_debug = 0;
			g_eth_rx_debug = 0;
			g_eth_irq_debug = 0;
			g_switch_debug = 0;
			g_eth_poll_debug = 0;
			g_eth_reset_debug = 0;
			break;

		case 6:
			if(netif_queue_stopped(g_sf_priv->netdev)){
				printk("queue stopped\n");
				netif_wake_queue(g_sf_priv->netdev);
			}
			break;

		case 7:
			{
				unsigned long print_detail = 0;
				ret = kstrtoul(str[1], 0, &print_detail);
				for (j = 0; j < 7; j++)
				  printk("dump buffer occupide by master%ld,read address 0x%lx, value 0x%x\n",j, (0x240060 + 4*j), sf_readl(0x240060 + 4*j));

				printk("tx_idx %d  tx_free_idx %d rx_idx %d\n", g_sf_priv->txd_next_index, g_sf_priv->txd_free_index,g_sf_priv->rxd_index);
				temp_bd= g_sf_priv->tx_bd_tbl + g_sf_priv->txd_next_index;
				printk("tx_bd ctrl 0x%x \n",temp_bd->bd_ctrl);
				temp_bd= g_sf_priv->rx_bd_tbl + g_sf_priv->rxd_index;
				printk("rx_bd ctrl 0x%x \n",temp_bd->bd_ctrl);

				if(print_detail == 1){
					for (j = 0; j < NUM_TX_DESCR; j++){
						temp_bd= g_sf_priv->tx_bd_tbl + j;
						printk("idx %ld txbd_address %p  tx_bd ctrl 0x%x  skb %p\n",j,temp_bd,temp_bd->bd_ctrl,g_sf_priv->p_tx_skb[j]);
					}
				}
			}
			break;

		case 8:
			{
				unsigned long phy_num=0, phy_addr=0, phy_value=0;
				ret = kstrtoul(str[1], 0, &phy_num);
				ret = kstrtoul(str[2], 0, &phy_addr);
				if(str[3][0] != '\0'){
					ret = kstrtoul(str[3], 0, &phy_value);
					if (str[4][0] != '\0'){
						ret = kstrtoul(str[4], 0, &g_user_speed);
						g_user_port = phy_num;
						printk("reauto to set phy%ld advertised speed 0x%lx\n", phy_num, g_user_speed);
						sf_phy_re_auto(g_sf_priv, phy_num, 1);
						break;
					}

					write_phy_reg(phy_num, phy_addr , phy_value , g_sf_priv->phy_dev, 1);
					printk("write phy %ld address %ld write value 0x%lx\n", phy_num, phy_addr, phy_value);
				}else
				  printk("dump phy %ld address %ld read value 0x%x\n", phy_num, phy_addr, read_phy_reg(phy_num, phy_addr , g_sf_priv->phy_dev, 1));
			}
			break;

		case 9:
			ret = kstrtoul(str[1], 0, &value);
			if(ret == -EINVAL)
			  goto err_parsing;
			else if(ret == -ERANGE)
			  goto err_overflow;
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
			sf_phy_energy_detect_disable(value, g_sf_priv->phy_dev);
			printk("disable phy%ld energy detect mdoe\n",value);
#else
			g_i_tx_wake_time = value;
			printk("set tx waketime %ld ms\n",value);
#endif
			break;

		case 0xa:
			{
				unsigned long start_reset = 0;
				ret = kstrtoul(str[1], 0, &start_reset);
				if(start_reset == 1){
					sf_eth_reset(g_sf_priv);
				}else
				  g_eth_reset_debug = 1;
				printk("reset done\n");
			}
			break;

		case 0xb:
			printk("tx_irq is %ld  tx packets %ld  all irq %ld tx stop %ld tx wake %ld\n", g_sf_priv->sfstats.tx_irq, g_sf_priv->netstats.tx_packets, g_sf_priv->sfstats.all_irq,g_sf_priv->sfstats.tx_stop_queue, g_sf_priv->sfstats.tx_wake_queue);
			printk("rx_irq is %ld  rx packets %ld \n", g_sf_priv->sfstats.rx_irq, g_sf_priv->netstats.rx_packets);
			break;

#ifdef SF_SPEED_BANLANCE
		case 0xc:
			{
				ret = kstrtoul(str[1], 0, &value);
				gpi_ing_shaper_init(value);
				printk("set gpi ingress shaper rate %ld Mbps\n", value);
			}
			break;
#endif
		case 0xd:
			{
                                unsigned long onoff_switch = 0, i = 0;
                                ret = kstrtoul(str[1], 0, &onoff_switch);
                                ret = kstrtoul(str[2], 0, &value);
                                if (index_arg < 2){
                                        printk("input error! need at least 2 argument：\
                                                        \n arg1: onoff_switch, 1 means set led on time, 0 means set led off time \
                                                        \n arg2: led on/off time, max is 0xffff, default on time 0xff, off time 0x1ff\n");
                                        break;
                                }

                                if (onoff_switch == 1){
                                        printk("set port_list: 0x%x led on time: 0x%lx, max is 0xffff\n", (g_sf_priv->port_list & SF_MAC_PORTS_LIST), value);
                                        for(i = 0; i < SF_MAC_PORTS; i++){
                                                if (!(SF_MAC_PORTS_LIST & (1 << i)))
                                                        continue;
                                                if (g_sf_priv->port_list & (1 << i)){
                                                        writeb(value & 0xff, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 8);
                                                        writeb(value >> 8, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 12);
                                                }
                                        }
                                }else{
                                        printk("set port_list: 0x%x led off time: 0x%lx, max is 0xffff\n", (g_sf_priv->port_list & SF_MAC_PORTS_LIST), value);
                                        for(i = 0; i < SF_MAC_PORTS; i++){
                                                if (!(SF_MAC_PORTS_LIST & (1 << i)))
                                                        continue;
                                                if (g_sf_priv->port_list & (1 << i)){
                                                        writeb(value & 0xff, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i);
                                                        writeb(value >> 8, (void *)EMAC1_LED_OFF_COUNT_LOW + 0x10 * i + 4);
                                                }
                                        }
                                }
			}
			break;

		default:
			printk("error args\n");
			break;
	}
	return count;

err_parsing:
	printk("parsing Error,please check your input!\n");
	return count;

err_overflow:
	printk("convert overflow,please check your input!\n");
	return count;
}

ssize_t sfax8_npu_counts_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
	int ret = 0;
	char buf[128] = {0};
	size_t read;

	ret = sprintf(buf, "check phy link status\n");
    ret += sprintf(buf+ret, "eth_reset_count:%d    tx_q_stop_count:%d   skb_al_fail_count:%d\n", eth_reset_count, tx_q_stop_count, skb_al_fail_count);

	read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
	return read;
}
ssize_t sfax8_npu_counts_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long addr = 0, i = 0, ret = 0, last_i = 0, index_arg = 0;
	char str[5][11] = {'\0'};
	char buf[128] = {0};

	size_t len = min_t(size_t, count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
	  return -EFAULT;

	for(;i < len;i++){
		if(buf[i] == ' '){
			memcpy(str[index_arg], buf + last_i, i - last_i);
			last_i=(i + 1);
			index_arg++;
		}
	}
	memcpy(str[index_arg], buf + last_i, count - last_i);
	// printk("str1 %s str2 %s str3 %s str4 %s\n",str[0],str[1],str[2],str[3]);
	ret = kstrtoul(str[0], 0, &addr);
	if(ret == -EINVAL)
	  goto err_parsing;
	else if(ret == -ERANGE)
	  goto err_overflow;

	switch (addr)
	{
		case 0:
        {
            printk("eth_reset_count is:%d\n",eth_reset_count);
            printk("tx_q_stop_count is:%d\n",tx_q_stop_count);
            printk("skb_al_fail_count is:%d\n",skb_al_fail_count);
        }
			break;

		default:
			printk("error args\n");
			break;
	}
	return count;

err_parsing:
	printk("parsing Error,please check your input!\n");
	return count;

err_overflow:
	printk("convert overflow,please check your input!\n");
	return count;
}

#endif
#endif
