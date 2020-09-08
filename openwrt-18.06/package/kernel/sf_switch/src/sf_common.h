#ifndef SF_COMMON_H
#define SF_COMMON_H

#include <linux/io.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/clk.h>
#include <asm/clock.h>
#include <linux/atomic.h>
#include <linux/etherdevice.h>
#ifdef CONFIG_SFAX8_SWITCH_DPS
#include <linux/workqueue.h>
#include <linux/sfax8_netlink.h>
#endif
#include <linux/phy.h>
#include <linux/interrupt.h>

#define NUM_TX_DESCR             128
#define NUM_RX_DESCR             128

#define FWD_PORT_LIST_MASK           0x3f
#define SF_MAX_PORTS                 6
#define SF_HOST_PORT                 5

#define VLANID_MASK                  0x1FFF
#define DEFAULT_VLANID               1

#ifdef SF_SPEED_BANLANCE
#define BMU_LIMIT_COUNT 350
#endif

#define MIN_FRAME_SIZE 46
#define MAX_FRAME_SIZE 9000

/* We have 66(NET_IP_ALIGN + NET_SKB_PAD) bytes headroom before, and NPU
 * hardware only needs 16 bytes, so it's enough for NPU driver.
 * But mac80211 needs 112(extra_tx_headroom + IEEE80211_ENCRYPT_HEADROOM) bytes
 * headroom, so mac80211 will resize the skb and copy the buffer of the skb,
 * which is unused cost.
 * So now we expand the head, and mac80211 will not copy the buffer.
 * */
#define EXTER_HEADROOM 64

//#define CONFIG_SFAX8_SWITCH_FPGA
// #ifdef CONFIG_SFAX8_SWITCH_FPGA
// 	#define SUPPORT_2_PORTS
// #endif

// #ifdef SUPPORT_2_PORTS
// #define SF_MAC_PORTS                 2
// #else
// #define SF_MAC_PORTS                 5
// #endif

#define SF_MAC_PORTS                 5
#define SF_MAC_PORTS_LIST                0x1f
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
#define SF_POWERSAVE_PORTS              4
#define SF_POWERSAVE_PORTS_MASK            0xf
#endif

#define BD_CTRL_BD_BUFLEN_MASK (0xffff)
#define BD_CTRL_CBD_INT_EN     (1 << 16)
#define BD_CTRL_PKT_INT_EN     (1 << 17)
#define BD_CTRL_LIFM           (1 << 18)
#define BD_CTRL_LAST_BD        (1 << 19)
#define BD_CTRL_DIR            (1 << 20)
#define BD_CTRL_PKT_XFER       (1 << 24)
#define BD_CTRL_DESC_EN        (1 << 31)
#define BD_CTRL_PARSE_DISABLE    (1 << 25)
#define BD_CTRL_BRFETCH_DISABLE  (1 << 26)
#define BD_CTRL_RTFETCH_DISABLE  (1 << 27)

#define CSR_TX_DMA_EN             (1 << 0)
#define CSR_TX_BDP_POLL_CNTR_EN   (1 << 1)
#define CSR_TX_BDP_CH_START_WSTB  (1 << 2)
#define CSR_RX_DMA_EN             (1 << 0)
#define CSR_RX_BDP_POLL_CNTR_EN   (1 << 1)
#define CSR_RX_BDP_CH_START_WSTB  (1 << 2)


/* Punt reason codes */
#define SF_PUNT_L2_SPL          (1 << 0)
#define SF_PUNT_SA_MISS         (1 << 1)
#define SF_PUNT_SA_RELEARN      (1 << 2)
#define SF_PUNT_SA_IS_ACTIVE    (1 << 3)
#define SF_PUNT_SNOOP_UPPER     (1 << 4)
#define SF_PUNT_REQ             (1 << 5)
#define SF_PUNT_MGMT            (1 << 6)
#define SF_PUNT_IGMP            (1 << 7)
#define SF_PUNT_FLOOD           (1 << 8)
#define SF_PUNT_PARSE           (1 << 9)


#define  SF_SHORT_SWAP(in_usValue) (((0xFF) & ((in_usValue) >> 8)) | ((0xFF00) & ((in_usValue) << 8)))
#define  SF_INT_SWAP(in_usValue) (((0xFF) & ((in_usValue) >> 24)) | ((0xFF00) & ((in_usValue) >> 8)) | ((0xFF0000) & ((in_usValue) << 8)) | ((0xFF000000) & ((in_usValue) << 24)))


#define	 MAC_CFG_LOOPBACK_MODE			(1 << 12)
#define  STP_CTRL_ADDR1_REG_VAL			0x00000040
#define  STP_CTRL_ADDR2_REG_VAL			0x00000080

#define  NO_MGMT						0x0
#define  TMU_QUE_TAIL_DROP				0x1
#define  TMU_QUE_WRED					0x2

#define  TMU_EACH_PHY_OFFFSET			0x1000
#define  TMU_EACH_SCH_OFFSET			0x300
#define  SHPR_EN_BIT					0x00000001

#define  TOTAL_CONTEXT_REGS			    0x08
#define  WRITE_CMD						0x01
#define  START_CMD						0x02
#define  DONE_BIT						0x04
#define  SHED_ADDRESS_MASK				0x300
#define  SHPR_ADDRESS_MASK				0x100
#define  DEV_ADDRESS_MASK				0x1000

#define  SCH_SLOT_ZERO					0x0
#define  SCH_SLOT_ONE					0x1
#define  SCH_SLOT_TWO					0x2
#define  SCH_SLOT_THREE					0x3
#define  SCH_SLOT_FOUR					0x4
#define  SCH_SLOT_FIVE					0x5
#define  SCH_SLOT_SIX					0x6
#define  SCH_SLOT_SEVEN					0x7
#define  CURQ_QMAX_QMIN_CFG				0x4
#define  CURQ_HW_PROB_CFG_TBL0			0x5
#define  CURQ_HW_PROB_CFG_TBL1			0x6
#define  REG_LEN						0x4

#define  TMU_MAX_SCHED_NUM				0x02
#define  TMU_MAX_SHPR_NUM				0x04
#define  TMU_MAX_DEV_NUM				0x06
#define  TMU_MAX_QUE_NO					0x08

#define  SCH_ALGO_PQ					0
#define  SCH_ALGO_DWRR					2
#define  SCH_ALGO_RR					3
#define  SCH_ALGO_WRR					4

#define TC_DSCP							0
#define TC_PCP							1
#define TC_PID							2
#define TC_MACDA						3

#define TMU_HW_EN					    0x0000000F
#define CUTTHRU_TYPE_START_POS			16
#define GLOBAL_MGMT_PORT_START_POS		16

typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef unsigned short ushort;

typedef struct _tmu_schd_params_t_ {
	uint tlsd_schd_id;    /*!< Scheduler number (1 to 2) depends on device number */
	uint tlsd_dev_id;     /*!< Device number ( 1 to 5) */
	uint tlsd_algo;       /*!< Scheduler algorithm type 1-RR, 2-WRR(packet based only),3-DWRR and 0-PQ */
	uint tlsd_position;   /*!< Scheduler position */
	uint tlsd_weight;     /*!< Scheduler weight */
	uint tlsd_rate_units; /*!< Rate units 1 for bits/sec and 2 for packets/sec */
} tmu_schd_params_t;

typedef struct _tmu_shpr_params_t_ {
	uint tlsp_shpr_updates;   /*!< Shaper updates field */
	uint tlsp_shpr_id;    /*!< Shaper number to connect to scheduler depends on device number.*/
	uint tlsp_dev_id;     /*!< Device number ( 1 to 4) */
	uint tlsp_clk_frequency;  /*!< Clock frequency used to do weights calculation a given rate (Mhz) */
	uint tlsp_rate;       /*!< Data rate for a given shaper packets per second or bits per second (Mbps). */
	uint tlsp_rate_units; /*!< Rate units 1 for bits/sec and 2 for packets/sec */
	uint tlsp_max_credit; /*!< Maximum value of the credit that can accumulated per shaper. */
	uint tlsp_position;   /*!< Shaper position to connect. */
	uint tlsp_min_credit; /*!< Minimum value of the credit per shaper */
	uint tlsp_mode;       /*!< mode 0 for normal mode and 1 for credit mode */
} tmu_shpr_params_t;

typedef struct _tmu_classq_params_t_ {
	uint tlcq_classq_updates; /*!< Raise flags for updated parameters */
	uint tlcq_dev_id;     /*!< Device number ( 1 to 4) */
	uint tlcq_classq_id;  /*!< Queue number (1 to 8), value depends on device number */
	uint tlcq_schd_id;    /*!< Scheduler number to which this classq is going to add and need to enable this scheduler bit in TDQ scheduler enable register. */
	uint tlcq_schd_slot;  /*!< Each scheduler contains 8 queues (1-low priority and 8-high priority),assign current input classq in one of this priority queue. */
	uint tlcq_max_len;    /*!< Quelength */
	uint tlcq_weight;     /*!< Queue weight for weighted round robin algorithm */
	uint tlcq_qmgmt;      /*!< This field decides queue management for drop mechanism tail drop or WRED. */
	uint tlcq_min_len;        /*!< If que management is WRED then this field is treated as Qmin */
	uint tlcq_prob_cfg0;      /*!< If queue management is WRED,configure probability value cfg0(max value 255) */
	uint tlcq_prob_cfg1;
	uint tlcq_prob_cfg2;
	uint tlcq_prob_cfg3;
	uint tlcq_prob_cfg4;
	uint tlcq_prob_cfg5;
	uint tlcq_prob_cfg6;
	uint tlcq_prob_cfg7;
} tmu_classq_params_t;


enum aft_e {

	ANY_TAGGING,       /*!< allow both tagged and untagged frames */
	TAGGED_ONLY,       /*!< allow only tagged frames */
	UNTAGGED_ONLY,     /*!< allow only untagged frames */
};

/*!
 *  * STP blocking state
 *   */
enum block_state_e {

	FORWARDING,        /*!< ok to learn SA and forward traffic */
	BLOCKED,           /*!< do not learn SA; do not forward frames */
	LEARN_ONLY,        /*!< ok to learn SA, but do not forward frames */
};
struct port_s {

	ushort               index;     /*!< index number identifying this port */
	ushort               shutdown;  /*!< shutdown/enable state, 1=shutdown */
	ushort               tpid;      /*!< 0, 0x8100, 0x88a8, 0x9100, 0x9200 */
	enum aft_e           aft;       /*!< acceptable frame type */
	ushort               fallback_bd_id; /*!< fallback bridge domain (PVID) */
	enum block_state_e   block_state;    /*!< block/forward state */

	/* QOS Related Fields Per Port */
	ushort                 trusted;          /* Indicates the port is trusted */
	ushort/*tcsel_table_s*/        tcsel_table;      /* TC Select Table */
	ushort/*pcp2tc_table_s*/       pcp2tc_table;     /* PCP2TC Table */
	ushort		  def_cfi;          /* Default CFI Value */
	ushort/*tc_s*/                 def_pri;          /* This is the default PRI Field.
													  * The PCP bits added when a tag
													  ** is added */
	ushort/*tc_s*/                 def_tc;           /* This is the default TC Field.
													  * The TC bits are used for COS
													  ** mapping. Ideally both def_pri
													  ** but for customer requirement */
	int/*tc2cos_table_s*/       tc2cos_table;     /* TC2COS Table */
	ushort vid_prefix; /* Take UNTAG Information from Bridge Table */
	ushort csr_untag_from_btable; /* Take UNTAG Information from Bridge Table */
	ushort                   pindex;	    /* Actual Physical Index (PHY NO).
											 * Will be used to remap local
											 * port to Act PHY This is useful
											 * to distinguish multiple logical
											 * representations in Port-7. Also
											 * helps send multiple different pkts
											 * to same PHY VLAN/STPID to P-7 */
};

enum fwd_action_e {

	ACT_FORWARD = 0,  /*!< Normal forward, use forward_list of MAC entry */
	ACT_FLOOD,        /*!< Flood to all ports in bridge domain, use forward list of BD entry */
	ACT_PUNT,         /*!< Punt packet to PE/host */
	ACT_DISCARD,      /*!< Discard this frame */
	ACT_OVERRIDE,
	ACT_FWD_MASK,
	ACT_COS_DISCARD
};

struct bd_entry_s {

	ushort forward_list;          /*!< forward_list has a bit flag corresponding
								   * to each port
								   *                                     */

	ushort untag_list;            /*!< Indicates whether to untag the packets
								   * at egress
								   *                                     */

	ushort mstp_id;               /*!< Index for MSTP FSM Number. Used to clear all
								   * entries of an MSTP index
								   *                                     */
	/*
	 ** forwarding actions for various situations
	 ** Note:  ACT_DO_BD_ACT is not a valid setting for these
	 ** Note2: ACT_FORWARD is not really valid as a miss action, but sliently
	 ** becomes ACT_FLOOD
	 **/
	enum fwd_action_e ucast_hit_action;   /*!< action to take when unicast DA found (default is ACT_FORWARD) */
	enum fwd_action_e mcast_hit_action;   /*!< action to take when multicast DA found (default is ACT_FORWARD) */
	enum fwd_action_e ucast_miss_action;  /*!< action to take when unicast DA not found (default is ACT_FLOOD) */
	enum fwd_action_e mcast_miss_action;  /*!< action to take when multicast DA not found (default is ACT_FLOOD) */
	uint tc;
};

struct bd {
	volatile uint bd_ctrl;
	uint bd_status;
	uint bd_bufptr;
	uint bd_next;
};
typedef struct packet_header
{
	uchar phyno;
	uchar *data;
	uchar *pLayer2Hdr;
	uchar *pLayer3Hdr;
	uchar *pLayer4Hdr;
	uint  vlanid;
	uint  punt_reason;
	uint  data_len;
	uint  avb_proto;
}pktHdrs_t;

/* VLan header */
typedef struct vlanEthhdr_s {
	uchar h_dest[ETH_ALEN];     /* destination eth addr */
	uchar h_source[ETH_ALEN];   /* source ether addr */
	ushort h_vlan_proto;        /* Should always be 0x8100 */
	ushort h_vlan_TCI;          /* Encapsulates priority and VLAN ID */
	ushort h_vlan_encap_proto;  /* pkt type ID field (or len) */
}vlanEthhdr_t;

struct sf_priv_stats{
	unsigned long tx_irq;
	unsigned long rx_irq;
	unsigned long all_irq;
	unsigned long tx_stop_queue;
	unsigned long tx_wake_queue;
};

struct sf_priv {

	struct net_device *netdev;
	struct device     *device;
	struct dentry     *npu_debug;
	struct dentry     *npu_counts;
	struct clk        *eth_bus_clk; /* bus1_clk */
	struct clk        *eth_ref_clk; /* supply for ephy */
	struct clk        *eth_byp_ref_clk; /* supply for mac gmii_refclk,not used */
	struct clk        *eth_tsu_clk; /* supply for timestamp unit */
#if defined(CONFIG_SF16A18_MPW1) || defined(CONFIG_SF16A18_FULL_MASK)
	struct clk        *npu_pe_clk;	/* NPU processor clk */
#endif
	//TODO: spinlock  need to enhance
	// spinlock_t lock;                     /* Spin lock flag */
	spinlock_t rx_lock;                     /* Spin lock flag */
	spinlock_t switch_lock;                  /* Spin lock flag */
        atomic_t bmu_irq_count;
	int iram_addr;
	int iram_len;
	struct bd *tx_bd_tbl;                /*   64-bit aligned */
	struct bd *rx_bd_tbl;                /*   64 bit aligned */
	dma_addr_t tx_bd_tbl_pa;
	dma_addr_t rx_bd_tbl_pa;

	uint count;                          /*enhence switch thread*/
	uint rxd_index;
	uint txd_next_index;
	uint txd_free_index;

	struct sk_buff	*p_tx_skb[NUM_TX_DESCR];
	struct sk_buff	*p_rx_skb[NUM_RX_DESCR];

	struct sf_switch_data_t *sf_switch_data;
	int						port_list;
#ifdef CONFIG_SFAX8_SWITCH_VLAN
	struct sfax8_switch_t *sfax8_switch;
#else
	int						phy_irq_base[SF_MAC_PORTS];
	struct bd_entry_s		fallback_bd_entry;
	struct port_s			port[SF_MAX_PORTS];
#endif

	struct tasklet_struct	task[SF_MAC_PORTS];
	struct task_struct		*bmu_kthread;
	struct napi_struct napi_tx;
	struct napi_struct napi_rx;
	struct phy_device phy_dev[SF_MAC_PORTS];
	struct net_device_stats netstats;
	struct sf_priv_stats	sfstats;
	uint   msg_enable;
	uint   shutdown_list;
#ifdef CONFIG_SFAX8_SWITCH_DPS
	struct work_struct dpswork;
	uint updown_port;
	uint port_status;
	sf_nlfamily dps_family;
#endif
    int bmu_irq;
};


#endif /* ifndef SF_COMMON_H */
