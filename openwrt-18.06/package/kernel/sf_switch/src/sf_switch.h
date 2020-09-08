#ifndef SF_SWITCH_H
#define SF_SWITCH_H
#include <linux/list.h>
#include <linux/switch.h>
#include <linux/kthread.h>
#define ETH_PROTO_SIZE               2
#define ETH_SMAC_DMAC_SIZE           12

#define STATIC_FLAG_SET           0x40000000
#define FRESH_FLAG_SET            0x00000000
#define FRESH_FLAG_MASK           0x20000000

#define HASH_CNT_PER_ITERATION    0x200
/* Below macros are used when we serach with memread */
#define HASH_ENTRY_VLAN_SET_FLAG  0x02000000  /* MAC3: VLAN FIELDVALID BIT */
#define HASH_ENTRY_VALID_FLAG     0x08000000  /* MAC4: ENTRY VALID BIT */
#define HASH_ENTRY_COLL_FLAG      0x04000000  /* MAC4: HASH_ENTRY_COLL_FLAG*/
#define HASH_ENTRY_FRESH_FLAG     0x04000000  /* */
#define HASH_ENTRY_STATIC_FLAG    0x08000000  /* */

#define  DOS_ALL_ENABLE_MASK         0x1
#define  DOS_CONTROL2_VALUE_MASK     0x3FF03FF
#define  DOS_ACTION_MASK             0x7
#define  DOS_COMB_CONTROL_MASK       0xF
#define  DOS_COMB_VALUE_MASK         0xFFFFFFFF
#define  DOS_IPV4_MAX_LEN_MASK       0x1FFFF

#define  DOS_ACTION_START_POS        0x1
#define  DOS_COMB_CONTROL_START_POS  0x4

enum snoop_etype {
	SNOOP_IGMP = 4,  /*!< Normal forward, use forward_list of MAC entry */
	SNOOP_MLDV2,        /*!< Flood to all ports in bridge domain, use forward list of BD entry */
	SNOOP_ETYPE0 = 8,         /*!< Punt packet to PE/host */
	SNOOP_ETYPE1,      /*!< Discard this frame */
	SNOOP_ETYPE2,
	SNOOP_ETYPE3,
	SNOOP_ETYPE_ARP,
	SNOOP_ETYPE_RARP,
	SNOOP_HOPOPTS,
};

enum dos_value {
	DOS_ENABLE, //all dos enable
	DOS_FILTER_ENABLE, //CLASS_DOS_CONTRL2 value,
	//DOS_ACTION, //is not used in IP
	DOS_COMB_FILTER_ENABLE,
	DOS_COMB_FILTER_VALUE,
	DOS_ICMPV4_MAX_LEN,
};

enum DOS_FILTER_ITEM{
	IPV4_SIP_EQ_DIP_EN,
	IPV4_ICMP_FRAG_EN,
	IPV4_ICMP_PING_TOO_BIG_EN,
	IPV4_UDP_SP_EQ_DP_EN,
	IPV4_TCP_FLAG_CHK,
	IPV4_TCP_SP_EQ_DP,
	IPV4_TCP_FRAG_OFF1,
	IPV4_TCP_SYN_ERR,
	IPV4_TCP_XMAS,
	IPV4_TCP_NULL,
	IPV6_SIP_EQ_DIP_EN,
	IPV6_ICMP_FRAG_EN,
	IPV6_ICMP_PING_TOO_BIG_EN,
	IPV6_UDP_SP_EQ_DP_EN,
	IPV6_TCP_FLAG_CHK,
	IPV6_TCP_SP_EQ_DP,
	IPV6_TCP_FRAG_OFF1,
	IPV6_TCP_SYN_ERR,
	IPV6_TCP_XMAS,
	IPV6_TCP_NULL,
};

enum DOS_COMB_ITEM {
	COMB_FILTER_1,
	COMB_FILTER_2,
	COMB_FILTER_3,
	COMB_FILTER_4,
};

enum NPU_FUC_MODL {
	SET_SPL_MAC,
	INIT_GLB_CUTTHRU,
	SET_DOS_FIL,
	SET_DSCP_RULE,
	SET_GLB_MGMT_ETYE,
	SET_PORT_CTL,
	SET_SNOOP_ETYE,
	SET_PORT_UNTAG,
	SET_VID_PFIX,
	SET_TMU_CFG,
	SET_GPI_INGRS_CFG,
	SET_GPI_FLW_CTL,
	SET_MAC_ADDR_FIL,
	SET_MAC_CHKSUM,
	SET_MAC_L3L4_FIL,
	SEND_PUSE_FRM,
	ENABLE_SRC_RPL,
	SET_MAC_VLAN_FIL,
};

struct sfax8_vlan_entry {
	u16	vid;
	u8	member;
	struct list_head entry_list;
};

struct sfax8_switch_t {
	struct net_device			*netdev;
	void __iomem				*base;
	int							irq_base[SF_MAC_PORTS];
	struct switch_dev			swdev;
	struct sfax8_mapping		*map;

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	struct task_struct			*switch_kthread;
#endif

	char						buf[64];
	uchar						max_port_num;
	uchar						host_port;
	uchar						fwd_port_list;
	struct bd_entry_s			fallback_bd_entry;
	struct port_s				port[SF_MAX_PORTS];
	struct sfax8_vlan_entry		vlan_entries;
};

struct sf_switch_data_t {
	void (*set_mac)( unsigned char *mac);
	int (*switch_open)(struct sf_priv *priv);
	int (*switch_stop)(struct sf_priv *priv);
	int (*switch_probe)(struct sf_priv *priv);
	int (*switch_remove)(struct sf_priv *priv);
	int (*switch_reset)(struct sf_priv *priv);
	int (*has_carrier)(struct sf_priv *priv, unsigned int port_list, int lock);
	void (*adjust_link)(struct sf_priv *priv, int port, int lock);
};

extern struct sf_switch_data_t sf_switch_data;
#endif /* ifndef SF_SWITCH_H */
