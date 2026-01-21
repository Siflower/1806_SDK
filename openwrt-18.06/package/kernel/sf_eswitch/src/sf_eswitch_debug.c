/*
* Description
*
* Copyright (C) 2016-2019 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include "sf_eswitch.h"
#include "sf_eswitch_debug.h"
#include "intel7084_src/src/gsw_sw_init.h"
#include "realtek8367c_src/rtk_switch.h"
#include "realtek8367c_src/vlan.h"
#include "realtek8367c_src/stat.h"
#include "realtek8367c_src/rtl8367c_asicdrv_vlan.h"
#include "realtek8367c_src/l2.h"
#include "yt9215rb_src/yt_stat.h"
#include "yt9215rb_src/yt_port.h"
#include "an8855_src/sf_an8855_ops.h"
#include "air.h"

extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);
extern int jl5106_get_mib_counters_and_print(unsigned int port);
extern int jl5106_mib_port_clear_all(unsigned int port);
extern struct vlan_entry vlan_entries;


long sf_eswitch_debug_ioctl (struct file * fil, unsigned int a, unsigned long b){

	intel7084_mc_ioctl (a, b);
	return 0;
}

int sf_eswitch_debug_open(struct inode *inode, struct file *file)
{
	struct sfax8_debug_info *debug = NULL;

	debug = kzalloc(sizeof(struct sfax8_debug_info), GFP_KERNEL);
	if (!debug)
		return -ENOMEM;

	debug->i_private = inode->i_private;
	file->private_data = debug;

	return 0;
}

int sf_eswitch_debug_release(struct inode *inode, struct file *file)
{
	struct sfax8_debug_info *debug = file->private_data;

	if (!debug)
		return 0;

	file->private_data = NULL;
	kfree(debug);
	return 0;
}

ssize_t sf_eswitch_debug_read(struct file *file, char __user *user_buf,
		size_t count, loff_t *ppos)
{
	struct sfax8_debug_info *debug = file->private_data;
	struct sf_eswitch_priv *pesw_priv = debug->i_private;
	struct switch_port_link link;
	int i, max_port = 0, ret = 0;
	char buf[256] = {0};
	size_t read;

	if (pesw_priv->model == RTK8367C)
		max_port = RTK_SWITCH_PORT_NUM;
	else if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082)
		max_port = INTEL_SWITCH_PORT_NUM;
	else if (pesw_priv->model == NF2507)
		max_port = 5;
	else if (pesw_priv->model == YT9215RB || pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC)
		max_port = YT9215RB_PHY_PORT_NUM;
	else if (pesw_priv->model == AN8855)
		max_port = 4;
	else if (pesw_priv->model == JL5106)
		max_port = 4;

	ret = sprintf(buf, "check phy link status\n");
	for(i = 0; i < max_port; i++){
		if (!check_port_in_portlist(pesw_priv, i))
			continue;
		pesw_priv->pesw_api->ops->get_port_link(&pesw_priv->swdev, i, &link);
		ret += sprintf(buf+ret, "phy%d link %d speed %d duplex %d\n",
				i, link.link, link.speed, link.duplex);
	}

	// add switch type here
	ret += sprintf(buf+ret, "switch_type %d\n", pesw_priv->model);
	read = simple_read_from_buffer(user_buf, count, ppos, buf, ret);
	return read;
}

static void print_help(void)
{
	printk(" Attention: all example should append with '>  /sys/kernel/debug/esw_debug'\n");
	printk(" read/write switch reg, no value for read, witch value for write\n");
	printk(" example: echo rwReg		[addr] [value] ,for {realtek,nf,yt,intel} switch\n");
	printk(" example: echo rwReg		[addr] [shift] [size] [value] ,for {realtek,nf,yt,intel} switch\n");
	printk(" read/write switch phy reg, no value for read, witch value for write, support both intel/realtek switch\n");
	printk(" example: echo rwPHYReg		[port] [addr] [value]\n");
	printk(" read/write switch mmd reg, no value for read, witch value for write, only support intel switch now\n");
	printk(" example: echo rwMMDReg		[port] [addr] [value]\n");
	printk(" set switch port egress mode, 0 for org, 1 for keep, only support {realtek,nf,yt,intel} switch now\n");
	printk(" example: echo rwPvid		[port] [pvid]\n");
	printk(" get switch port pvid, no pvid value for read, witch pvid for write\n");
	printk(" example: echo rwVlanPorts		[vlan id] [member_list] [untag_list]\n");
	printk(" set switch vlan ports, no member_list and untag_list for read, witch member_list and untag_list for write\n");
	printk(" example: echo setPortEgressMode	[port] [mode]\n");
	printk(" dump switch port tx/rx count, only support {realtek,yt,intel} switch now\n");
	printk(" example: echo dumpSwitchCount		[port] \n");
	printk(" clear switch port tx/rx count, only support {realtek,yt,intel} switch now\n");
	printk(" example: echo clearSwitchCount		[port] \n");
	printk(" enable software multicast function, only support intel switch now\n");
	printk(" example: echo enableMulticastFunc \n");
	printk(" port join/leave mc_ip group, only support intel switch now\n");
	printk(" example: echo setMulticastEntry	[port] [type] [mc_ip]\n");
	printk(" dump multicast entries, only support intel switch now\n");
	printk(" example: echo dumpMulticastEntry \n");
}

ssize_t sf_eswitch_debug_write(struct file *file, const char __user *user_buf,
		size_t count, loff_t *ppos)
{
	struct sfax8_debug_info *debug = file->private_data;
	struct sf_eswitch_priv *pesw_priv = debug->i_private;
	unsigned int i = 0, ret = 0, last_i = 0, index_arg = 0;
	char str[5][20] = {'\0'};
	char buf[128] = {0};
	rtk_uint32 address = 0;
	rtk_l2_ucastAddr_t l2_data;
	struct switch_dev *swdev=&pesw_priv->swdev;
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int port_list = 0;


	size_t len = min_t(size_t, count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;

	for(; i < len; i++){
		if (buf[i] == ' '){
			memcpy(str[index_arg], buf + last_i, i - last_i);
			last_i = (i + 1);
			index_arg++;
		}
	}
	memcpy(str[index_arg], buf + last_i, count - last_i);

	if (index_arg > 5)
		goto err_parsing;

	if (strncmp(str[0], "help", 4) == 0){
		print_help();
		return count;
	} else if (strncmp(str[0], "dumpMac", 7) == 0){
		unsigned int port = 0;
		ret = kstrtou32(str[1], 0, &port);
		while(1){
			if((rtk_l2_addr_next_get(READMETHOD_NEXT_L2UCSPA, port, &address, &l2_data))!=RT_ERR_OK)
				break;
			printk("%pM\n", l2_data.mac.octet);
			address++;
		}
	} else if (strncmp(str[0], "renetlink", 9) == 0){
		unsigned int port = 0;
		ret = kstrtou32(str[1], 0, &port);
#ifdef CONFIG_SFAX8_GENL
		notify_link_event(pesw_priv, port, 1, "eth0", NULL, 0, false);
#endif
	} else if (strncmp(str[0], "led", 3) == 0){
		unsigned int led_mode = 0;
		ret = kstrtou32(str[1], 0, &led_mode);
		if(led_mode == 0 || led_mode == 1){
			if(led_mode == 0)
				led_mode = 2;
			else
				led_mode = 0;
		}
		else{
			printk("0:led all off; 1:led normal");
			return count;
		}
		pesw_priv->pesw_api->led_init(led_mode);
	} else if (strncmp(str[0], "rwReg", 4) == 0){
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			unsigned int addr = 0, shift = 0, size = 0, value = 0;

			ret = kstrtou32(str[1], 0, &addr);
			ret = kstrtou32(str[2], 0, &shift);
			ret = kstrtou32(str[3], 0, &size);
			if (str[4][0] != '\0'){
				ret = kstrtou32(str[4], 0, &value);
				pesw_priv->pesw_api->setAsicReg(addr, value);
				printk("intel write addr:0x%08x value:0x%08x\n", addr, value);
			} else {
				pesw_priv->pesw_api->getAsicReg(addr, (rtk_uint32 *)&value);
				printk("intel read addr:0x%08x value:0x%08x\n", addr, value);
			}
		} else {
			unsigned int addr = 0, value = 0;
			ret = kstrtou32(str[1], 0, &addr);
			if (str[2][0] != '\0'){
				ret = kstrtou32(str[2], 0, &value);
				pesw_priv->pesw_api->setAsicReg(addr, value);
				printk("{nf|rtk|yt} write addr:0x%08x value:0x%08x\n", addr, value);
			} else {
				if (pesw_priv->model == YT9215RB || pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC)
					pesw_priv->pesw_api->getAsicReg(addr, (u32 *)&value);
				else
					pesw_priv->pesw_api->getAsicReg(addr, (rtk_uint32 *)&value);
				printk("{nf|rtk|yt} read addr:0x%08x value:0x%08x\n", addr, value);
			}
		}
	} else if (strncmp(str[0], "rwPHYReg", 8) == 0){
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			unsigned int port=0, addr=0, value=0;
			GSW_MDIO_data_t md;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &addr);
			md.nAddressDev = port;
			md.nAddressReg = addr;
			if (str[3][0] != '\0'){
				ret = kstrtou32(str[3], 0, &value);
				md.nData = value;
				intel7084_phy_wr(&md);
				printk("intel write phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, value);
			} else {
				intel7084_phy_rd( &md);
				printk("intel read phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, md.nData);
			}
		} else {
			unsigned int port=0, addr=0, value=0;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &addr);
			if (str[3][0] != '\0'){
				ret = kstrtou32(str[3], 0, &value);
				pesw_priv->pesw_api->setAsicPHYReg(port, addr, (uint)value);
				printk("{nf|rtk|yt} write phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, value);
			} else {
				pesw_priv->pesw_api->getAsicPHYReg(port, addr, (uint *)&value);
				printk("{nf|rtk|yt} read phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, value);
			}
		}
	} else if (strncmp(str[0], "rwMMDReg", 8) == 0){
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			GSW_MMD_data_t md;
			unsigned int port=0, addr=0, value=0;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &addr);
			md.nAddressDev = port;
			md.nAddressReg = addr;
			if (str[3][0] != '\0'){
				ret = kstrtou32(str[3], 0, &value);
				md.nData = value;
				intel7084_mmd_wr(&md);
				printk("intel mmd write phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, value);
			} else {
				intel7084_mmd_rd(&md);
				printk("intel mmd read phy:%d addr:0x%08x val:0x%08x\n",
						port, addr, md.nData);
			}
		} else if (pesw_priv->model == AN8855) {
			unsigned int port=0, devad=0, reg=0, value=0;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &devad);
			ret = kstrtou32(str[3], 0, &reg);
			if (str[4][0] != '\0') {
				ret = kstrtou32(str[4], 0, &value);
				SF_MDIO_LOCK();
				__switch_phy_c45_write(0, port, devad, reg, value);
				SF_MDIO_UNLOCK();
				printk("mmd write phy:%d devad:0x%x reg:0x%08x val:0x%08x\n",
						port, devad, reg, value);
			} else {
				SF_MDIO_LOCK();
				__switch_phy_c45_read(0, port, devad, reg, &value);
				SF_MDIO_UNLOCK();
				printk("mmd read phy:%d devad:0x%x reg:0x%08x val:0x%08x\n",
						port, devad, reg, value);
			}
		}
	} else if (strncmp(str[0], "rwPvid", 6) == 0){
		unsigned int port=0, pvid=0;
		ret = kstrtou32(str[1], 0, &port);
		ret = kstrtou32(str[2], 0, &pvid);
		if (str[2][0] != '\0'){
			pesw_priv->pesw_api->ops->set_port_pvid(&pesw_priv->swdev, port, pvid);
			printk("set port%d pvid:%d\n", port, pvid);
		} else{
			pesw_priv->pesw_api->ops->get_port_pvid(&pesw_priv->swdev, port, &pvid);
			printk("get port%d pvid:%d\n", port, pvid);
		}
	} else if (strncmp(str[0], "rwVlanPorts", 11) == 0){
		struct switch_val val;
		struct switch_port p[5] = {0};
		unsigned int vid=0, mbr_list = 0, untag_list = 0, index = 0;
		ret = kstrtou32(str[1], 0, &vid);
		if (str[3][0] != '\0'){
			ret = kstrtou32(str[2], 0, &mbr_list);
			ret = kstrtou32(str[3], 0, &untag_list);
			for (i = 0; i < 7; i++) {
				if (mbr_list & (1 << i))
				{
					if (i == 6) {
						if ( pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC)
							p[index].id = YT9215S_NUM_CPU_PORTS;
						else
							p[index].id = EXT_PORT0;
					} else{
						p[index].id = i;
					}
					if (!(untag_list & (1 << i)))
						p[index].flags = BIT(SWITCH_PORT_FLAG_TAGGED);
					index++;
				}
			}

			val.port_vlan = vid;
			val.len = index;
			val.value.ports = p;
			pesw_priv->pesw_api->ops->set_vlan_ports(&pesw_priv->swdev, &val);
			printk("set vid:%d member_list:0x%x untag_list:0x%x\n", vid, mbr_list, untag_list);
		} else{
			val.port_vlan = vid;
			val.value.ports = p;
			pesw_priv->pesw_api->ops->get_vlan_ports(&pesw_priv->swdev, &val);
			for (i = 0; i < val.len; i++) {
				if ((pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC) &&
						val.value.ports[i].id == YT9215S_NUM_CPU_PORTS) {
					mbr_list |= (1 << 6);
				} else if (val.value.ports[i].id == EXT_PORT0) {
					mbr_list |= (1 << 6);
				} else
					mbr_list |= (1 << val.value.ports[i].id);

				if (!(val.value.ports[i].flags & BIT(SWITCH_PORT_FLAG_TAGGED)))
				{
					if (val.value.ports[i].id == EXT_PORT0) {
						untag_list |= (1 << 6);
					} else if ((pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC) &&
							val.value.ports[i].id == YT9215S_NUM_CPU_PORTS) {
						untag_list |= (1 << 6);
					} else
						untag_list |= (1 << val.value.ports[i].id);
				}
			}
			printk("get vid:%d member_list:0x%x untag_list:0x%x\n", vid, mbr_list, untag_list);
		}
	} else if (strncmp(str[0], "setPortEgressMode", 17) == 0){
		if (pesw_priv->model == RTK8367C) {
			unsigned int port=0, mode=0;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &mode);
			rtl8367c_setAsicVlanEgressTagMode(port, mode);
			printk("set port%d egress tag mode:%d\n", port, mode);
		}
	} else if (strncmp(str[0], "dumpSwitchCount", 15) == 0){
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			unsigned int port=0;
			GSW_RMON_Port_cnt_t parm;
			ret = kstrtou32(str[1], 0, &port);
			parm.nPortId = port;
			intel7084_count_rd(&parm);
			printk("===== get switch port:%u counter =====\n", port);
			printk("nRxGoodPkts:     %-20u nTxGoodPkts:     %-20u\n", parm.nRxGoodPkts, parm.nTxGoodPkts);
			printk("nRxUnicastPkts:  %-20u nRxBroadcastPkts:%-20u nRxMulticastPkts:%-20u\n",
					parm.nRxUnicastPkts, parm.nRxBroadcastPkts, parm.nRxMulticastPkts);
			printk("nTxUnicastPkts:  %-20u nTxBroadcastPkts:%-20u nTxMulticastPkts:%-20u\n",
					parm.nTxUnicastPkts, parm.nTxBroadcastPkts, parm.nTxMulticastPkts);
			printk("nRxGoodPausePkts:%-20u nTxPauseCount:%-20u\n",
					parm.nRxGoodPausePkts, parm.nTxPauseCount);
			printk("nTxDroppedPkts:  %-20u nRxDroppedPkts:%-20u\n",
					parm.nTxDroppedPkts, parm.nRxDroppedPkts);
			printk("nRxUdrSzErrPkts: %-20u nRxOversizeErrPkts:%-20u\n",
					parm.nRxUnderSizeErrorPkts, parm.nRxOversizeErrorPkts);
			printk("nRxFCSErrorPkts: %-20u nTxAcmDroppedPkts:%-20u\n",
					parm.nRxFCSErrorPkts, parm.nTxAcmDroppedPkts);
		} else if (pesw_priv->model == RTK8367C) {
			unsigned int port=0;
			rtk_stat_port_cntr_t parm;
			ret = kstrtou32(str[1], 0, &port);
			rtk_stat_port_getAll(port, &parm);
			printk("===== get switch port:%u counter =====\n", port);
			printk("ifInUcastPkts:          %-20u ifOutUcastPkts:    %-20u\n",
					parm.ifInUcastPkts, parm.ifOutUcastPkts);
			printk("ifInBroadcastPkts:      %-20u ifOutBroadcastPkts:%-20u\n",
					parm.ifInBroadcastPkts, parm.ifOutBrocastPkts);
			printk("ifInMulticastPkts:      %-20u ifOutMulticastPkts:%-20u\n",
					parm.ifInMulticastPkts, parm.ifOutMulticastPkts);
			printk("dot3StatsFCSErrors:     %-20u dot3StatsSymbolErrors:%-20u\n",
					parm.dot3StatsFCSErrors, parm.dot3StatsSymbolErrors);
			printk("inMldChecksumError:     %-20u inIgmpChecksumError:%-20u\n",
					parm.inMldChecksumError, parm.inIgmpChecksumError);
			printk("inReportSuppressionDrop:%-20u inLeaveSuppressionDrop:%-20u\n",
					parm.inReportSuppressionDrop, parm.inLeaveSuppressionDrop);
			printk("etherStatsDropEvents:   %-20u etherStatsPkts1024toMaxOctets:%-20u\n",
					parm.etherStatsDropEvents, parm.etherStatsPkts1024toMaxOctets);
			printk("dot3InPauseFrames:      %-20u dot3OutPauseFrames:%-20u\n",
					parm.dot3InPauseFrames, parm.dot3OutPauseFrames);
			printk("etherStatsUndersizePkts:%-20u etherStatsOversizePkts:%-20u\n",
					parm.etherStatsUndersizePkts, parm.etherStatsOversizePkts);
		} else if (pesw_priv->model == YT9215RB || pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC) {
			unsigned int port=0;
			yt_stat_mib_port_cnt_t parm;

			ret = kstrtou32(str[1], 0, &port);
			yt_stat_mib_port_get(0, port, &parm);

			printk("===== get switch port:%u counter =====\n", port);
			printk("ifInUcastPkts:          %-20u ifOutUcastPkts:    %-20u\n",
					parm.RX_UNICAST, parm.TX_UNICAST);
			printk("ifInBroadcastPkts:      %-20u ifOutBroadcastPkts:%-20u\n",
					parm.RX_BROADCAST, parm.TX_BROADCAST);
			printk("ifInMulticastPkts:      %-20u ifOutMulticastPkts:%-20u\n",
					parm.RX_MULTICAST, parm.TX_MULTICAST);
			printk("rx_fcs_err:             %-20u rx_alignment_err:%-20u\n",
					parm.RX_FCS_ERR, parm.RX_ALIGNMENT_ERR);
			printk("dot3InPauseFrames:      %-20u dot3OutPauseFrames:%-20u\n",
					parm.RX_PAUSE, parm.TX_PAUSE);
			printk("etherStatsUndersizePkts:%-20u etherStatsOversizePkts:%-20u\n",
					parm.RX_UNDERSIZE, parm.TX_UNDERSIZE);
			printk("rx_discard_err:         %-20u rx_fragment_crc_err:%-20u\n",
					parm.RX_DISCARD, parm.RX_FRAGMENT);
		} else if (pesw_priv->model == AN8855) {
			unsigned int port=0;
			AIR_MIB_CNT_RX_T rx_mib;
			AIR_MIB_CNT_TX_T tx_mib;

			ret = kstrtou32(str[1], 0, &port);
			air_mib_get(0, port, &rx_mib, &tx_mib);
			printk("===== get switch port:%u counter =====\n", port);
			printk("TUPC:      %-20u RUPC:%-20u\n",
					tx_mib.TUPC, rx_mib.RUPC);
			printk("TBPC:      %-20u RBPC:%-20u\n",
					tx_mib.TBPC, rx_mib.RBPC);
			printk("TMPC:      %-20u RMPC:%-20u\n",
					tx_mib.TMPC, rx_mib.RMPC);
			printk("TCRC:      %-20u RCEPC:%-20u\n",
					tx_mib.TCRC, rx_mib.RCEPC);
			printk("TPPC:      %-20u RPPC:%-20u\n",
					tx_mib.TPPC, rx_mib.RPPC);
			printk("TOC:       %-20llu ROC:%-20llu\n",
					tx_mib.TOC, rx_mib.ROC);
			printk("TDPC:      %-20u RDPC:%-20u\n",
					tx_mib.TDPC, rx_mib.RDPC);
			printk("TCEC:      %-20u RFPC:%-20u\n",
					tx_mib.TCEC, rx_mib.RFPC);
			printk("TSCEC:      %-20u RAEPC:%-20u\n",
					tx_mib.TSCEC, rx_mib.RAEPC);
			printk("TMCEC:      %-20u RUSPC:%-20u\n",
					tx_mib.TMCEC, rx_mib.RUSPC);
			printk("TDEC:      %-20u RFEPC:%-20u\n",
					tx_mib.TDEC, rx_mib.RFEPC);
			printk("TLCEC:      %-20u ROSPC:%-20u\n",
					tx_mib.TLCEC, rx_mib.ROSPC);
			printk("TXCEC:      %-20u RJEPC:%-20u\n",
					tx_mib.TXCEC, rx_mib.RJEPC);
			printk("TL64PC:      %-20u RL64PC:%-20u\n",
					tx_mib.TL64PC, rx_mib.RL64PC);
			printk("TL65PC:      %-20u RL65PC:%-20u\n",
					tx_mib.TL65PC, rx_mib.RL65PC);
			printk("TL128PC:      %-20u RL128PC:%-20u\n",
					tx_mib.TL128PC, rx_mib.RL128PC);
			printk("TL256PC:      %-20u RL256PC:%-20u\n",
					tx_mib.TL256PC, rx_mib.RL256PC);
			printk("TL512PC:      %-20u RL512PC:%-20u\n",
					tx_mib.TL512PC, rx_mib.RL512PC);
			printk("TL1024PC:      %-20u RL1024PC:%-20u\n",
					tx_mib.TL1024PC, rx_mib.RL1024PC);
			printk("TL1519PC:      %-20u RL1519PC:%-20u\n",
					tx_mib.TL1519PC, rx_mib.RL1519PC);
			printk("TODPC:      %-20u RCDPC:%-20u\n",
					tx_mib.TODPC, rx_mib.RCDPC);
			printk("TOC2:      %-20llu RIDPC:%-20u\n",
					tx_mib.TOC2, rx_mib.RIDPC);
			printk("TOC2:      %-20llu RADPC:%-20u\n",
					tx_mib.TOC2, rx_mib.RADPC);
			printk("TOC2:      %-20llu FCDPC:%-20u\n",
					tx_mib.TOC2, rx_mib.FCDPC);
			printk("TOC2:      %-20llu WRDPC:%-20u\n",
					tx_mib.TOC2, rx_mib.WRDPC);
			printk("TOC2:      %-20llu MRDPC:%-20u\n",
					tx_mib.TOC2, rx_mib.MRDPC);
			printk("TOC2:      %-20llu SFSPC:%-20u\n",
					tx_mib.TOC2, rx_mib.SFSPC);
			printk("TOC2:      %-20llu SFTPC:%-20u\n",
					tx_mib.TOC2, rx_mib.SFTPC);
			printk("TOC2:      %-20llu RXC_DPC:%-20u\n",
					tx_mib.TOC2, rx_mib.RXC_DPC);
			printk("TOC2:      %-20llu ROC2:%-20llu\n",
					tx_mib.TOC2, rx_mib.ROC2);
		} else if (pesw_priv->model == JL5106) {
			unsigned int port=0;
			int ret;

			ret = kstrtou32(str[1], 0, &port);
			jl5106_get_mib_counters_and_print(port);
		}

	} else if (strncmp(str[0], "clearSwitchCount", 16) == 0){
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			unsigned int port=0, mode=0;
			GSW_RMON_clear_t mp;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou32(str[2], 0, &mode);
			mp.nRmonId = port;
			mp.eRmonType = mode;
			intel7084_count_clear(&mp);
			printk("clear switch counter type:%d port:%d\n", mode, port);
		} else if (pesw_priv->model == RTK8367C) {
			unsigned int port=0;
			ret = kstrtou32(str[1], 0, &port);
			rtk_stat_port_reset(port);
			printk("clear switch counter port:%d\n", port);
		} else if (pesw_priv->model == YT9215RB || pesw_priv->model == YT9215S || pesw_priv->model == YT9215SC) {
			unsigned int port=0;
			ret = kstrtou32(str[1], 0, &port);
			yt_stat_mib_clear(0, port);
			printk("clear switch counter port:%d\n", port);
		} else if (pesw_priv->model == AN8855) {
			unsigned int port=0;
			ret = kstrtou32(str[1], 0, &port);
			air_mib_clear_by_port(0, port);
			printk("clear switch counter port:%d\n", port);
		} else if (pesw_priv->model == JL5106) {
			unsigned int port=0;
			ret = kstrtou32(str[1], 0, &port);
			jl5106_mib_port_clear_all(port);
			printk("clear switch counter port:%d\n", port);
		}

	} else if (strncmp(str[0], "enableMulticastFunc", 19) == 0){
		//enable software multicast function
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082)
			intel7084_multicast_set();
	} else if (strncmp(str[0], "setMulticastEntry", 17) == 0){
		//port join/leave mc_ip group
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			u32 port,mc_ip;
			u8 type;
			ret = kstrtou32(str[1], 0, &port);
			ret = kstrtou8(str[2], 0, &type);
			ret = kstrtou32(str[3], 0, &mc_ip);
			intel7084_multicast_entry_set(port, type, mc_ip);
		}
	} else if (strncmp(str[0], "dumpMulticastEntry", 18) == 0){
		//read multicast entries
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082)
			intel7084_multicast_entry_get();
	} else if (strncmp(str[0], "enableBridgeRedirect", sizeof(str[0])) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err = intel7084_bridge_redirect_ip(str[1]);

			if (err < 0)
				return err;
		} else {
			return -EOPNOTSUPP;
		}
	} else if (strncmp(str[0], "disableBridgeRedirect", sizeof(str[0])) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err = intel7084_bridge_redirect_disable();

			if (err < 0)
				return err;
		} else {
			return -EOPNOTSUPP;
		}
	} else if (strncmp(str[0], "setdelay", 8) == 0) {
			unsigned int tx_delay=0, rx_delay=0, enable=1;
			ret = kstrtou32(str[1], 0, &tx_delay);
			ret = kstrtou32(str[2], 0, &rx_delay);
			ret = kstrtou32(str[3], 0, &enable);
			SF_MDIO_LOCK();
			int err = yt_port_extif_rgmii_delay_set(0, 6, rx_delay, tx_delay, enable);
			SF_MDIO_UNLOCK();
			if (err < 0) {
				printk("error set delay tx:%d rx:%d\n", tx_delay, rx_delay);
				return err;
			}
	} else if (strncmp(str[0], "getdelay", 8) == 0) {
			u8 tx_delay, rx_delay;
			yt_enable_t ret;
			SF_MDIO_LOCK();
			int err = yt_port_extif_rgmii_delay_get(0, 6, &rx_delay, &tx_delay, &ret);
			SF_MDIO_UNLOCK();
			if (err < 0) {
				printk("error get delay \n");
				return err;
			}
			if (ret)
				printk("get delay tx:%d rx:%d\n", tx_delay, rx_delay);
	}
#ifdef CONFIG_SFAX8_ESWITCH_REDIRECT
	else if (strncmp(str[0], "disableUploadDHCP", 17) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err = intel7084_bridge_redirect_dhcp_disabled();

			if (err < 0)
				return err;
		} else if (pesw_priv->model == AN8855) {
			air_an8855_redirect_dhcp_disabled();
		} else {
			return -EOPNOTSUPP;
		}
	} else if (strncmp(str[0], "enableUploadDHCP", 16) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err = intel7084_bridge_redirect_dhcp();

			if (err < 0)
				return err;
		} else if (pesw_priv->model == AN8855) {
			air_an8855_redirect_dhcp();
		} else {
			return -EOPNOTSUPP;
		}
	}
	else if (strncmp(str[0], "disableUploadARP", 16) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err =intel7084_bridge_redirect_arp_disabled();

			if (err < 0)
				return err;
		} else {
			return -EOPNOTSUPP;
		}
	} else if (strncmp(str[0], "enableUploadARP", 15) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err =intel7084_bridge_redirect_arp();

			if (err < 0)
				return err;
		}else {
			return -EOPNOTSUPP;
		}
	}else if (strncmp(str[0], "enableUploadDNS", 15) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err =intel7084_bridge_redirect_dns();

			if (err < 0)
				return err;
		} else if (pesw_priv->model == AN8855) {
			air_an8855_redirect_dns();
		}else {
			return -EOPNOTSUPP;
		}
	} else if (strncmp(str[0], "disableUploadDNS", 16) == 0) {
		if (pesw_priv->model == INTEL7084 || pesw_priv->model == INTEL7082) {
			int err =intel7084_bridge_redirect_dns_disabled();

			if (err < 0)
				return err;
		} else if (pesw_priv->model == AN8855) {
			air_an8855_redirect_dns_disabled();
		}else {
			return -EOPNOTSUPP;
		}
	}
#endif
	else if (strncmp(str[0], "applyVlan", 9) == 0){
		swdev->ops->apply_config(swdev);
	}
	else if (strncmp(str[0], "vlan_entries", 12) == 0){
		printk("Dumping VLAN configurations:\n");
		mutex_lock(&swdev->sw_mutex);
		list_for_each(pos, &(vlan_entries.entry_list)) {
			entry = list_entry(pos, struct vlan_entry, entry_list);
			printk("VLAN %d: member=0x%x, untag=0x%x\n", entry->vid, entry->member, entry->untag);
			port_list |= entry->member;
		}
		mutex_unlock(&swdev->sw_mutex);
		printk("Final Port List: 0x%x\n", port_list);
	}
	else {
		printk("command not support!!!\n");
	}

	return count;

err_parsing:
	printk("parsing Error,please check your input!\n");
	return count;
}
