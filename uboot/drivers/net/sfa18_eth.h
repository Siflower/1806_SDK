#ifndef SFA18_ETH_H
#define SFA18_ETH_H

#include <common.h>
#include <malloc.h>
#include <net.h>
#include <config.h>
#include <usb_ether.h>
#include <phy.h>
#include <mach/sfax8.h>
#include "sfa18_mac.h"

// #define NO_CACHE
#define NPU_ADDR_BASE            0xB0000000

#define NUM_TX_DESCR            4
#define NUM_RX_DESCR            16

#define FWD_PORT_LIST_MASK           0x3f
#define SF_MAX_PORTS                 6
#define SF_HOST_PORT                 5

#define VLANID_MASK                  0x1FFF
#define DEFAULT_VLANID               1

#define MIN_FRAME_SIZE 46
#define MAX_FRAME_SIZE 2048
#define MAX_BUF_ALLOCSIZE 2048

#define SF_MAC_PORTS                 5
#define BUF_ALIGN                    32

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
#define HIF_INT        (1 << 0)
#define HIF_RXBD_INT   (1 << 1)
#define HIF_RXPKT_INT  (1 << 2)
#define HIF_TXBD_INT   (1 << 3)
#define HIF_TXPKT_INT  (1 << 4)

#define MAC_TABLE  1
#define VLAN_TABLE 2

#define HIF_INT_EN  (1 << 0)
#define HIF_RXBD_INT_EN   (1 << 1)
#define HIF_RXPKT_INT_EN  (1 << 2)
#define HIF_TXBD_INT_EN   (1 << 3)
#define HIF_TXPKT_INT_EN  (1 << 4)

#define MAC_CFG_TX_EN (1 << 3)
#define MAC_CFG_RX_EN (1 << 2)
/* VLAN Mac Hash table */
#define REQ_VLAN_BASE_ADDR   REG_CLASS_DAVLANHASH_HOST_CMD_REG
#define  REQ_VLAN_CMD_REG          (REQ_VLAN_BASE_ADDR + 0x0)
#define  REQ_VLAN_MAC1_ADDR_REG    (REQ_VLAN_BASE_ADDR + 0x4)
#define  REQ_VLAN_MAC2_ADDR_REG    (REQ_VLAN_BASE_ADDR + 0x8)
#define  REQ_VLAN_MAC3_ADDR_REG    (REQ_VLAN_BASE_ADDR + 0xc)
#define  REQ_VLAN_MAC4_ADDR_REG    (REQ_VLAN_BASE_ADDR + 0x10)
#define  REQ_VLAN_MAC5_ADDR_REG    (REQ_VLAN_BASE_ADDR + 0x14)
#define  REQ_VLAN_ENTRY_REG        (REQ_VLAN_BASE_ADDR + 0x18)
#define  REQ_VLAN_STATUS_REG       (REQ_VLAN_BASE_ADDR + 0x1c)
#define  REQ_VLAN_DIRECT_REG       (REQ_VLAN_BASE_ADDR + 0x20)

#define  FREE_VLAN_LIST_ENTRIES    REG_CLASS_DAVLANHASH_FREELIST_ENTRIES
#define  FREE_VLAN_LIST_HEAD_PTR   REG_CLASS_DAVLANHASH_FREELIST_HEAD_PTR
#define  FREE_VLAN_LIST_TAIL_PTR   REG_CLASS_DAVLANHASH_FREELIST_TAIL_PTR

/* HW MAC TABLE CMDS */
#define CMD_INIT    1
#define CMD_ADD     2
#define CMD_DEL     3
#define CMD_UPDATE  4
#define CMD_SEARCH  5
#define CMD_MEM_READ    6
#define CMD_MEM_WRITE   7
#define CMD_FLUSH   8

#define CLR_STATUS_REG         0xFFFFFFFF

/* Vlan group table is common for all hash tables */
#define VLAN_INIT_HEAD_PTR       0x40
#define VLAN_INIT_TAIL_PTR       0x7f
#define VLAN_FREE_LIST_ENTRIES   0x40
#define  VLAN_HASH_ENTRIES        0x40
#define  VLAN_COLL_HASH_ENTRIES   0x40

/* Mac Hash table */
#define  FREE_LIST_ENTRIES    REG_CLASS_DAMACHASH_FREELIST_ENTRIES
#define  FREE_LIST_HEAD_PTR   REG_CLASS_DAMACHASH_FREELIST_HEAD_PTR
#define  FREE_LIST_TAIL_PTR   REG_CLASS_DAMACHASH_FREELIST_TAIL_PTR

/* Mac Hash table */
#define  REQ_MAC_BASE_ADDR   REG_CLASS_DAMACHASH_HOST_CMD_REG
#define  REQ_CMD_REG          (REQ_MAC_BASE_ADDR + 0x0)
#define  REQ_MAC1_ADDR_REG    (REQ_MAC_BASE_ADDR + 0x4)
#define  REQ_MAC2_ADDR_REG    (REQ_MAC_BASE_ADDR + 0x8)
#define  REQ_MAC3_ADDR_REG    (REQ_MAC_BASE_ADDR + 0xc)
#define  REQ_MAC4_ADDR_REG    (REQ_MAC_BASE_ADDR + 0x10)
#define  REQ_MAC5_ADDR_REG    (REQ_MAC_BASE_ADDR + 0x14)
#define  REQ_ENTRY_REG        (REQ_MAC_BASE_ADDR + 0x18)
#define  REQ_STATUS_REG       (REQ_MAC_BASE_ADDR + 0x1c)
#define  REQ_DIRECT_REG       (REQ_MAC_BASE_ADDR + 0x20)

/* HW MAC TABLE CMDS */
#define CMD_INIT    1
#define CMD_ADD     2
#define CMD_DEL     3
#define CMD_UPDATE  4
#define CMD_SEARCH  5
#define CMD_MEM_READ    6
#define CMD_MEM_WRITE   7
#define CMD_FLUSH   8

#define CLR_STATUS_REG         0xFFFFFFFF
#define TWO_FIELD_HASH_ENTRIES     0x200
#define MAC_HASH_ENTRIES           0x200
#define TWO_FIELD_COLL_ENTRIES     0x200
#define MAC_COLL_ENTRIES           0x200
#define TWO_FIELD_INIT_HEAD_PTR    0x200
#define MAC_INIT_HEAD_PTR    0x200
#define TWO_FIELD_INIT_TAIL_PTR    0x3ff
#define MAC_INIT_TAIL_PTR    0x3ff

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

typedef unsigned int   uint;
typedef unsigned char  uchar;
typedef unsigned short ushort;

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
								   * */

	ushort untag_list;            /*!< Indicates whether to untag the packets
								   * at egress
								   * */

	ushort mstp_id;               /*!< Index for MSTP FSM Number. Used to clear all
								   * entries of an MSTP index
								   * */
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

struct sf_switch_s {
	uchar max_port_num;
	uchar host_port;
	uchar fwd_port_list;
	struct bd_entry_s fallback_bd_entry;
	struct port_s port[SF_MAX_PORTS];
};

/*
** struct bd must be padded to cache_line size(32bytes).
** When malloc memory for struct bd, the memory point shoule be aligned
** with 32 bytes, so the BUF_ALIGN is changed to 32.
**
** why:
**		We will malloc a continuous memory space for many bds.
**		If we do not pad struct bd to 32 bytes, the size of struct bd is 16 bytes.
**		In that condition, we assume there existed adjacent struct bd, we called
**		them BD.A and BD.B, BD.A and BD.B may be stored in one cache line due to
**		the size of cache_line is 32bytes. We flushed the BD.A cache but the whole
**		cache line would be fushed, in this time, the BD.B stored in the cache line
**		would be flush too but we didn't expect BD.B to beflushed, in other word,
**		the operation of flushing BD.A is out of our expection, and the operation
**		will have a unpredictable impact on the running program.
*/
struct bd {
	uint bd_ctrl;
	uint bd_status;
	uint bd_bufptr;
	uint bd_next;
}__attribute__(( aligned(32) ));

struct sf_priv {

	struct eth_device *dev;

	char *tx_bd_tbl;
	char *rx_bd_tbl;
	struct bd *tx_bd_tbl_align;          /*   64-bit aligned */
	struct bd *rx_bd_tbl_align;          /*   64 bit aligned */

	char *tx_buf;
    char *rx_buf;
	char *tx_buf_align;          /*   64-bit aligned */
	char *rx_buf_align;          /*   64 bit aligned */

	uint rxd_index;
	uint txd_index;

	struct sf_switch_s sf_switch;
	struct phy_device phy_dev[SF_MAC_PORTS];
#ifdef CONFIG_SFA18_PCBA_TEST
	uchar txport;
	uchar rxport;
	uchar test_result;
	uchar pcba_npu_test;
#endif
};

/********************sys ctl address ************************/
#define SIFLOWER_NPU_HIF_IRQ			16
#define SIFLOWER_NPU_BMU_IRQ			24


/********************Npu address ************************/
#define NPU_ADDR_BASE                     0xB0000000

/***********************BMU CSR*************************/
#define BMU_BASE_OFFSET                          0x240000
#define REG_BMU_CTRL                             BMU_BASE_OFFSET  + 0x04
#define REG_BMU_UCAST_CONFIG                     BMU_BASE_OFFSET  + 0x08
#define REG_BMU_UCAST_BASEADDR                   BMU_BASE_OFFSET  + 0x0c
#define REG_BMU_BUF_SIZE                         BMU_BASE_OFFSET  + 0x10
#define REG_BMU_BUF_CNT							 BMU_BASE_OFFSET  + 0x14
#define REG_BMU_REM_BUF_CNT						 BMU_BASE_OFFSET  + 0x48
#define REG_BMU_THRES                            BMU_BASE_OFFSET  + 0x18
#define REG_BMU_INT_SRC                          BMU_BASE_OFFSET  + 0x20
#define REG_BMU_INT_ENABLE                       BMU_BASE_OFFSET  + 0x24

/***********************EMAC CSR*************************/
#define EMAC0_BASE_OFFSET                        0x200000
#define EMAC1_BASE_OFFSET							0x220000
#define EMAC2_BASE_OFFSET							0x680000
#define EMAC3_BASE_OFFSET							0x690000
#define EMAC4_BASE_OFFSET							0x6a0000
#define REG_EMAC_CFG(X)                          EMAC##X##_BASE_OFFSET + 0x0000
#define REG_EMAC_FILTER(X)                       EMAC##X##_BASE_OFFSET + 0x0004
#define REG_EMAC_GMII_ADDRESS(X)                 EMAC##X##_BASE_OFFSET + 0x0010
#define REG_EMAC_GMII_DATA(X)                    EMAC##X##_BASE_OFFSET + 0x0014
#define REG_EMAC_FLOWCTL(X)                      EMAC##X##_BASE_OFFSET + 0x0018
#define REG_EMAC_ADDRESS0_16_H(X)                EMAC##X##_BASE_OFFSET + 0x0040
#define REG_EMAC_ADDRESS0_32_L(X)                EMAC##X##_BASE_OFFSET + 0x0044
#define REG_EMAC_ADDRESS1_16_H(X)                EMAC##X##_BASE_OFFSET + 0x0048
#define REG_EMAC_ADDRESS1_32_L(X)                EMAC##X##_BASE_OFFSET + 0x004c

#define REG_EMAC_CONTROL(X)                      EMAC##X##_BASE_OFFSET + 0xB80
#define REG_EMAC_TIMESTAMP_CONTROL(X)            EMAC##X##_BASE_OFFSET + 0x700
#define REG_EMAC_FLOW_CONTROL(X)                 EMAC##X##_BASE_OFFSET + 0x0018
#define REG_MAC_ADDRESS0_HIGH(X)                 EMAC##X##_BASE_OFFSET + 0x0040
#define REG_MAC_ADDRESS0_LOW(X)                  EMAC##X##_BASE_OFFSET  + 0x0044

//emac_control_reg to control emac signals
#define REG_EMAC_CFG_GLOBAL(X)					 EMAC##X##_BASE_OFFSET + 0x0B80

#define CONFIG_EMAC_SECTION(X) \
    { REG_EMAC_CFG_GLOBAL(X),		0},\
    { REG_EMAC_ADDRESS0_16_H(X),	0x80000716}, \
    { REG_EMAC_ADDRESS0_32_L(X),	0x20881640}, \
    { REG_EMAC_CFG(X),				0x0210c880}, \
    { REG_EMAC_FILTER(X),			0x80000040}, \
    { REG_EMAC_FLOWCTL(X),			0xFFFF0006},



/***********************Classifier CSR*************************/
#define CLASS_BASE_OFFSET							0x320000

#define REG_CLASS_TX_CTRL                        CLASS_BASE_OFFSET  + 0x04
#define REG_CLASS_HDR_SIZE                       CLASS_BASE_OFFSET  + 0x14
#define REG_CLASS_TM_INQ_ADDR					 CLASS_BASE_OFFSET  + 0x114
#define REG_CLASS_BMU_BUF_FREE                   CLASS_BASE_OFFSET  + 0x24c
#define REG_CLASS_USE_TMU_INQ                    CLASS_BASE_OFFSET    + 0x250
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR1_LSB      CLASS_BASE_OFFSET  + 0x368
#define REG_CLASS_SNOOP_SPL_MCAST_ADDR1_MSB      CLASS_BASE_OFFSET  + 0x36c
#define REG_CLASS_SNOOP_SPL_MCAST_MASK1_LSB      CLASS_BASE_OFFSET  + 0x378
#define REG_CLASS_SNOOP_SPL_MCAST_MASK1_MSB      CLASS_BASE_OFFSET  + 0x37C
#define REG_CLASS_GLOBAL_CFG                     CLASS_BASE_OFFSET  + 0x4ac


#define REG_CLASS_DAMACHASH_HOST_CMD_REG         CLASS_BASE_OFFSET  + 0x390
#define REG_CLASS_DAVLANHASH_HOST_CMD_REG        CLASS_BASE_OFFSET  + 0x3e4
#define REG_CLASS_DAMACHASH_FREELIST_ENTRIES     CLASS_BASE_OFFSET  + 0x3d8
#define REG_CLASS_DAMACHASH_FREELIST_HEAD_PTR    CLASS_BASE_OFFSET  + 0x3dc
#define REG_CLASS_DAMACHASH_FREELIST_TAIL_PTR    CLASS_BASE_OFFSET  + 0x3e0
#define REG_CLASS_DAVLANHASH_FREELIST_ENTRIES    CLASS_BASE_OFFSET  + 0x42c
#define REG_CLASS_DAVLANHASH_FREELIST_HEAD_PTR   CLASS_BASE_OFFSET  + 0x430
#define REG_CLASS_DAVLANHASH_FREELIST_TAIL_PTR   CLASS_BASE_OFFSET  + 0x434

#define REG_CLASS_HW_PORT0_STRUC1      (CLASS_BASE_OFFSET + 0x0000046c)
#define REG_CLASS_HW_PORT0_STRUC2      (CLASS_BASE_OFFSET + 0x00000470)
#define REG_CLASS_HW_PORT1_STRUC1      (CLASS_BASE_OFFSET + 0x00000474)
#define REG_CLASS_HW_PORT1_STRUC2      (CLASS_BASE_OFFSET + 0x00000478)
#define REG_CLASS_HW_PORT2_STRUC1      (CLASS_BASE_OFFSET + 0x0000047c)
#define REG_CLASS_HW_PORT2_STRUC2      (CLASS_BASE_OFFSET + 0x00000480)
#define REG_CLASS_HW_PORT3_STRUC1      (CLASS_BASE_OFFSET + 0x00000484)
#define REG_CLASS_HW_PORT3_STRUC2      (CLASS_BASE_OFFSET + 0x00000488)
#define REG_CLASS_HW_PORT4_STRUC1      (CLASS_BASE_OFFSET + 0x0000048c)
#define REG_CLASS_HW_PORT4_STRUC2      (CLASS_BASE_OFFSET + 0x00000490)
#define REG_CLASS_HW_PORT5_STRUC1      (CLASS_BASE_OFFSET + 0x00000494)
#define REG_CLASS_HW_PORT5_STRUC2      (CLASS_BASE_OFFSET + 0x00000498)
#define REG_CLASS_HW_PORT6_STRUC1      (CLASS_BASE_OFFSET + 0x0000049c)
#define REG_CLASS_HW_PORT6_STRUC2      (CLASS_BASE_OFFSET + 0x000004a0)
#define REG_CLASS_HW_PORT7_STRUC1      (CLASS_BASE_OFFSET + 0x000004a4)
#define REG_CLASS_HW_PORT7_STRUC2      (CLASS_BASE_OFFSET + 0x000004a8)

/***********************EGPI CSR*************************/
#define EGPI0_BASE_OFFSET							0x210000
#define EGPI1_BASE_OFFSET							0x230000
#define EGPI2_BASE_OFFSET							0x6B0000
#define EGPI3_BASE_OFFSET							0x6C0000
#define EGPI4_BASE_OFFSET							0x620000

#define REG_GPI_INQ_PKTPTR0						0x210030
#define REG_GPI_INQ_PKTPTR1						0x230030
#define REG_GPI_INQ_PKTPTR2						0x6b0030
#define REG_GPI_INQ_PKTPTR3						0x6c0030
#define REG_GPI_INQ_PKTPTR4						0x620030
#define REG_GPI_INQ_PKTPTR5						0x340030

#define  REG_GPI_CTRL(X)                        EGPI##X##_BASE_OFFSET + 0x04
#define  REG_GPI_RX_CONFIG(X)                   EGPI##X##_BASE_OFFSET + 0x08
#define  REG_GPI_HDR_SIZE(X)                    EGPI##X##_BASE_OFFSET + 0x0c
#define  REG_GPI_BUF_SIZE(X)                    EGPI##X##_BASE_OFFSET + 0x10
#define  REG_GPI_LMEM_ALLOC_ADDR(X)             EGPI##X##_BASE_OFFSET + 0x14
#define  REG_GPI_LMEM_FREE_ADDR(X)              EGPI##X##_BASE_OFFSET + 0x18
#define  REG_GPI_CLASS_ADDR(X)                  EGPI##X##_BASE_OFFSET + 0x24
#define  REG_GPI_LMEM_SEC_BUF_DATA_ADDR(X)    EGPI##X##_BASE_OFFSET + 0x60
#define  REG_GPI_TMLF_TX(X)                     EGPI##X##_BASE_OFFSET + 0x4c
#define  REG_GPI_DTX_ASEQ(X)                    EGPI##X##_BASE_OFFSET + 0x50

#define CONFIG_EGPI_SECTION(X) \
    { REG_GPI_RX_CONFIG(X),     0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_GPI_HDR_SIZE(X),      0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_GPI_BUF_SIZE(X),      0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_GPI_LMEM_ALLOC_ADDR(X),  0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_GPI_LMEM_FREE_ADDR(X),   0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_GPI_CLASS_ADDR(X),       0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_GPI_LMEM_SEC_BUF_DATA_ADDR(X), 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_GPI_TMLF_TX(X),       0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_GPI_DTX_ASEQ(X),      0x00000050},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/


/***********************ETGPI CSR*************************/
#ifdef CONFIG_NET_SFA18_PTP
#define ETGPI0_BASE_OFFSET							0x630000
#define ETGPI1_BASE_OFFSET							0x640000
#define ETGPI2_BASE_OFFSET							0x650000
#define ETGPI3_BASE_OFFSET							0x660000
#define ETGPI4_BASE_OFFSET							0x670000

#define  REG_TGPI_RX_CONFIG(X)                   ETGPI##X##_BASE_OFFSET + 0x08
#define  REG_TGPI_HDR_SIZE(X)                    ETGPI##X##_BASE_OFFSET + 0x0c
#define  REG_TGPI_BUF_SIZE(X)                    ETGPI##X##_BASE_OFFSET + 0x10
#define  REG_TGPI_LMEM_ALLOC_ADDR(X)             ETGPI##X##_BASE_OFFSET + 0x14
#define  REG_TGPI_LMEM_FREE_ADDR(X)              ETGPI##X##_BASE_OFFSET + 0x18
#define  REG_TGPI_CLASS_ADDR(X)                  ETGPI##X##_BASE_OFFSET + 0x24
#define  REG_TGPI_LMEM_SEC_BUF_DATA_ADDR(X)    ETGPI##X##_BASE_OFFSET + 0x60
#define  REG_TGPI_TMLF_TX(X)                     ETGPI##X##_BASE_OFFSET + 0x4c
#define  REG_TGPI_DTX_ASEQ(X)                    ETGPI##X##_BASE_OFFSET + 0x50

#define CONFIG_ETGPI_SECTION(X) \
    { REG_TGPI_RX_CONFIG(X),     0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_TGPI_HDR_SIZE(X),      0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_TGPI_BUF_SIZE(X),      0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_TGPI_LMEM_ALLOC_ADDR(X),  0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_TGPI_LMEM_FREE_ADDR(X),   0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_TGPI_CLASS_ADDR(X),       0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_TGPI_LMEM_SEC_BUF_DATA_ADDR(X), 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_TGPI_TMLF_TX(X),       0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_TGPI_DTX_ASEQ(X),      0x00000050},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/
#endif

/***********************HGPI CSR*************************/
#define HGPI_BASE_OFFSET							0x340000

#define REG_HGPI_CTRL                            HGPI_BASE_OFFSET + 0x04
#define REG_HGPI_RX_CONFIG                       HGPI_BASE_OFFSET + 0x08
#define REG_HGPI_HDR_SIZE                        HGPI_BASE_OFFSET + 0x0c
#define REG_HGPI_BUF_SIZE                        HGPI_BASE_OFFSET + 0x10
#define REG_HGPI_LMEM_ALLOC_ADDR				HGPI_BASE_OFFSET + 0x14
#define REG_HGPI_LMEM_FREE_ADDR					HGPI_BASE_OFFSET + 0x18
#define REG_HGPI_CLASS_ADDR						HGPI_BASE_OFFSET + 0x24
#define REG_HGPI_INQ_PKTPTR                      HGPI_BASE_OFFSET + 0x30
#define REG_HGPI_LMEM_SEC_BUF_DATA_ADDR			HGPI_BASE_OFFSET + 0x60
#define REG_HGPI_TMLF_TX                         HGPI_BASE_OFFSET + 0x4c
#define REG_HGPI_DTX_ASEQ                        HGPI_BASE_OFFSET + 0x50

#define CONFIG_HGPI_SECTION() \
    { REG_HGPI_RX_CONFIG,     0x02000001}, /*bit 0 – LMEM buffer enable ;bit 31:16 – Retry count for LMEM buffers*/\
    { REG_HGPI_HDR_SIZE,      0x00000030}, /*bit 7:0 – LMEM first buffer header size value*/\
    { REG_HGPI_BUF_SIZE,      0x00000080}, /*bit 15:0 – LMEM buffer size value as 128 bytes*/\
    { REG_HGPI_LMEM_ALLOC_ADDR,  0x00240030}, /*Address of BMU1, where buffer should be allocated.*/\
    { REG_HGPI_LMEM_FREE_ADDR,   0x00240034}, /*Address of BMU1, where buffer should be freed*/\
    { REG_HGPI_CLASS_ADDR,       0x00320010}, /*Program address of the Class HW register where packet from peripherals are sent to*/\
    { REG_HGPI_LMEM_SEC_BUF_DATA_ADDR, 0x00000010}, /*bit 15:0 - LMEM header size from second buffer onwards for each buffer in chain*/\
    { REG_HGPI_TMLF_TX,       0x000000bc},/*bit 7:0 - Threshold number of TMLF words - 64bit size, to be in the TMLF FIFO before transmission starts*/\
    { REG_HGPI_DTX_ASEQ,      0x00000040},/*Initial number of bytes read from received pointer in LMEM, to check for action fields*/

/***********************TMU CSR*************************/
#define TMU_BASE_OFFSET							0x310000
#define REG_TMU_PHY0_INQ_ADDR					TMU_BASE_OFFSET  + 0x200
#define REG_TMU_PHY1_INQ_ADDR					TMU_BASE_OFFSET  + 0x204
#define REG_TMU_PHY2_INQ_ADDR					TMU_BASE_OFFSET  + 0x208
#define REG_TMU_PHY3_INQ_ADDR					TMU_BASE_OFFSET  + 0x20c
#define REG_TMU_PHY4_INQ_ADDR					TMU_BASE_OFFSET  + 0x210
#define REG_TMU_PHY5_INQ_ADDR					TMU_BASE_OFFSET  + 0x214
#define REG_TMU_CNTX_ACCESS_CTRL                 TMU_BASE_OFFSET  + 0x280
#define REG_TMU_PHY0_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x260
#define REG_TMU_PHY1_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x264
#define REG_TMU_PHY2_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x268
#define REG_TMU_PHY3_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x26c
#define REG_TMU_PHY4_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x270
#define REG_TMU_PHY5_TDQ_CTRL                    TMU_BASE_OFFSET  + 0x274
#define REG_TMU_BMU_INQ_ADDR					TMU_BASE_OFFSET  + 0x100

#define REG_TMU_PHY_QUEUE_SEL_ADDR               TMU_BASE_OFFSET + 0x34
#define REG_TMU_CURQ_PTR_ADDR                    TMU_BASE_OFFSET + 0x38
#define REG_TMU_CURQ_PKT_CNT_ADDR                TMU_BASE_OFFSET + 0x3c
#define REG_TMU_CURQ_DROP_CNT_ADDR               TMU_BASE_OFFSET + 0x40
#define REG_TMU_CURQ_TRANS_CNT_ADDR              TMU_BASE_OFFSET + 0x44
#define REG_TMU_CURQ_QSTAT_ADDR                  TMU_BASE_OFFSET + 0x48
#define REG_TMU_HW_PROB_CFG_TBL0_ADDR            TMU_BASE_OFFSET + 0x4c
#define REG_TMU_HW_PROB_CFG_TBL1_ADDR            TMU_BASE_OFFSET + 0x50
#define REG_TMU_CURQ_DEBUG_ADDR                  TMU_BASE_OFFSET + 0x54


/***********************HIF CSR*************************/
#define HIF_BASE_OFFSET							0x330000

#define REG_HIF_TX_CTRL                          HIF_BASE_OFFSET  + 0x04
#define REG_HIF_TX_BDP_ADDR                      HIF_BASE_OFFSET  + 0x10
#define REG_HIF_TX_ALLOC                         HIF_BASE_OFFSET  + 0x0c
#define REG_HIF_RX_CTRL                          HIF_BASE_OFFSET  + 0x20
#define REG_HIF_RX_BDP_ADDR						 HIF_BASE_OFFSET  + 0x24
#define REG_HIF_INT_SRC                          HIF_BASE_OFFSET  + 0x34
#define REG_HIF_INT_EN                           HIF_BASE_OFFSET  + 0x38
#define REG_HIF_POLL_CTRL                        HIF_BASE_OFFSET  + 0x3c
#define REG_HIF_RX_ALLOC                         HIF_BASE_OFFSET  + 0x44

/***********************WSP CSR*************************/
#define WSP_GLOBAL_BASE_OFFSET                     0x600000
#define REG_WSP_SYS_GENERIC_CONTROL              WSP_GLOBAL_BASE_OFFSET + 0x20

typedef struct hwinit_data {
   unsigned int offset;
   unsigned int data;
} hwinit_data_t;



#define PORT_TPID_START_POS                  0
#define PORT_FALLBACK_BDID_START_POS         16
#define PORT_SHUTDOWN_START_POS              0
#define PORT_AFT_START_POS                   4
#define PORT_BLOCKSTATE_START_POS            8
#define PORT_DEF_CFI_START_POS               12
#define PORT_DEF_PRI_START_POS               13
#define PORT_DEF_TC_START_POS                16
#define PORT_TRUSTED_START_POS               19
#define PORT_VID_PREFIX_START_POS            20
#define PORT_UNTAG_FROM_BTABLE_START_POS     21

#define PORT_TPID_MASK                       0x0000ffff
#define PORT_FALLBACK_BDID_MASK              0x1fff0000
#define PORT_SHUTDOWN_MASK                   0x00000001
#define PORT_AFT_MASK                        0x000000f0
#define PORT_BLOCKSTATE_MASK                 0x00000f00
#define PORT_DEF_CFI_MASK                    0x00001000
#define PORT_DEF_PRI_MASK                    0x0000e000
#define PORT_DEF_TC_MASK                     0x00070000
#define PORT_TRUSTED_MASK                    0x00080000
#define PORT_VID_PREFIX_MASK                 0x00100000
#define PORT_UNTAG_FROM_BTABLE_MASK          0x00200000

#define DEF_PORT_FALLBACK_BD_ID    0x1        /* iports fallback bd id */
struct tx_header {

	uchar rsvd0;
	uchar queue;
	uchar txport_map;
	uchar ctrl;
	ushort ref_num;
	ushort rsvd1;
	uint launch_time_nsec;
	uint launch_time_sec;
};

struct rx_header {
	ushort punt_reason;
	uchar rxport_num;
	uchar ctrl;
	uint rsvd;
	uint rx_timestamp_nsec;
	uint rx_timestamp_sec;
};
#define SF_TX_PKT_INJECT_EN     (1 << 0)
int sf_readl(uint offset);
void sf_writel(uint value,uint offset);
#ifdef CONFIG_SFA18_PCBA_TEST
#ifdef CONFIG_TARGET_SFA18_86V
#define PCBA_PKT_SIZE 64
#else
#define PCBA_PKT_SIZE 60
#endif
int sf_eth_send(struct eth_device *dev, void *packet, int length);
int sf_eth_recv(struct eth_device *dev);
int sf_eth_init(struct eth_device *dev, bd_t *bt);
#endif

#endif
