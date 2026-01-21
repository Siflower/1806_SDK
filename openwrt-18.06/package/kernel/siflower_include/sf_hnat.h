#ifndef _SF_HNAT_H_
#define _SF_HNAT_H_

#include <linux/etherdevice.h>
#include <linux/types.h>
#include <linux/if.h>
#include <linux/platform_device.h>

#define SF_HNAT_DEV_OFFSET             0 // skb->dev pointer offset
#define SF_HNAT_FLAG                   47 // skb hnat flag
#define SF_CB_HNAT_FORWARD             22 // skb nedd hnat forward
#define WIFI_PPPOE_OFFSET              8 // pppoe mode wifi ndev index offset


struct sf_hnat_if_subnet {
	unsigned int ipaddr;
	unsigned char prefix_len;
	unsigned char  valid;
	char ifname[IFNAMSIZ];
};

struct sf_hnat_priv {
	void __iomem *base;//base addr

	unsigned int hnat_mode;
	char snat_arp_force_dip;//register hnat_table_config bit 0(1:use dip,0:auto judge)
	char use_big_endian;
	char vlan_replace;

	unsigned short curr_napt_num;
	unsigned short curr_napt_tcp_num;
	unsigned short curr_napt_udp_num;
	unsigned short curr_dip_num;
	unsigned char curr_dmac_num;
	unsigned char curr_rmac_num;
	unsigned char curr_ppphd_num;
	unsigned char curr_rt_pub_net_num;
	unsigned char curr_vlan_id_num;
	unsigned short wifi_base;
	unsigned long last_age_time;
	unsigned long last_flush_time;

	struct sf_hnat_if_subnet lan_subnet[8];
	struct sf_hnat_if_subnet wan_subnet[8];
	struct net_device * pwifi_ndev[8];
	struct device *dev;
#ifdef CONFIG_DEBUG_FS
	struct dentry  *hnat_debug;
#endif


	struct net_device *driver_ndev;

	int (*init)(struct platform_device *pdev, void __iomem *reg_base, struct net_device *pdriver_ndev);
	//deinit hnat
	int (*deinit)(struct platform_device *pdev);
	//pass the param from ndo_flow_offload to hnat, config entries
	int (*ndo_flow_offload)(struct platform_device *pdev,
			enum flow_offload_type type,
			struct flow_offload *flow,
			struct flow_offload_hw_path *src,
			struct flow_offload_hw_path *dest);
	//search if the packet is hit in hnat module
	unsigned char (*search)(struct platform_device *pdev, struct sk_buff *skb);
	int (*sts_cnt)(struct platform_device *pdev, unsigned char *mac);
	int (*kick_device)(struct platform_device *pdev, unsigned char *mac);
	int (*print)(enum flow_offload_type type, struct flow_offload *flow, struct flow_offload_hw_path *src, struct flow_offload_hw_path *dest);

	int (*wifi_xmit_prepare)(struct platform_device *pdev, struct sk_buff *skb, unsigned short vlan_id, int  frame_len);
	int (*add_wifi_ndev)(struct platform_device *pdev, struct net_device *ndev);
	int (*del_wifi_ndev)(struct platform_device *pdev, struct net_device *ndev);
	int (*is_hnat_error)(struct sf_hnat_priv *phnat_priv);
	int (*is_hnat_to_wifi_pkt)(struct sf_hnat_priv *phnat_priv, struct sk_buff *skb);

#ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	/*test tool function depends on gmac,
	 * should select gmac tool as well*/
	struct sf_test_tool_priv * ptest_priv;
#else
	// for build without testtool
	void * ptest_priv;
#endif
};


#endif /*_SF_HNAT_H_*/
