#ifndef SF_SWITCH_HASH_TABLE_H
#define SF_SWITCH_HASH_TABLE_H

#include "sf_eth.h"
#include "sf_switch.h"
#include "sf_eth_addr.h"
#define  REQ_MAC_BASE_OFFSET   REG_CLASS_DAMACHASH_HOST_CMD_REG
#define REQ_VLAN_BASE_OFFSET   REG_CLASS_DAVLANHASH_HOST_CMD_REG
/* VLAN Mac Hash table */
#define  REQ_VLAN_CMD_REG          (REQ_VLAN_BASE_OFFSET + 0x0)
#define  REQ_VLAN_MAC1_ADDR_REG    (REQ_VLAN_BASE_OFFSET + 0x4)
#define  REQ_VLAN_MAC2_ADDR_REG    (REQ_VLAN_BASE_OFFSET + 0x8)
#define  REQ_VLAN_MAC3_ADDR_REG    (REQ_VLAN_BASE_OFFSET + 0xc)
#define  REQ_VLAN_MAC4_ADDR_REG    (REQ_VLAN_BASE_OFFSET + 0x10)
#define  REQ_VLAN_MAC5_ADDR_REG    (REQ_VLAN_BASE_OFFSET + 0x14)
#define  REQ_VLAN_ENTRY_REG        (REQ_VLAN_BASE_OFFSET + 0x18)
#define  REQ_VLAN_STATUS_REG       (REQ_VLAN_BASE_OFFSET + 0x1c)
#define  REQ_VLAN_DIRECT_REG       (REQ_VLAN_BASE_OFFSET + 0x20)

#define  FREE_VLAN_LIST_ENTRIES    REG_CLASS_DAVLANHASH_FREELIST_ENTRIES
#define  FREE_VLAN_LIST_HEAD_PTR   REG_CLASS_DAVLANHASH_FREELIST_HEAD_PTR
#define  FREE_VLAN_LIST_TAIL_PTR   REG_CLASS_DAVLANHASH_FREELIST_TAIL_PTR

/* Mac Hash table */
#define  FREE_LIST_ENTRIES    REG_CLASS_DAMACHASH_FREELIST_ENTRIES
#define  FREE_LIST_HEAD_PTR   REG_CLASS_DAMACHASH_FREELIST_HEAD_PTR
#define  FREE_LIST_TAIL_PTR   REG_CLASS_DAMACHASH_FREELIST_TAIL_PTR

/* Mac Hash table */
#define  REQ_CMD_REG          (REQ_MAC_BASE_OFFSET + 0x0)
#define  REQ_MAC1_ADDR_REG    (REQ_MAC_BASE_OFFSET + 0x4)
#define  REQ_MAC2_ADDR_REG    (REQ_MAC_BASE_OFFSET + 0x8)
#define  REQ_MAC3_ADDR_REG    (REQ_MAC_BASE_OFFSET + 0xc)
#define  REQ_MAC4_ADDR_REG    (REQ_MAC_BASE_OFFSET + 0x10)
#define  REQ_MAC5_ADDR_REG    (REQ_MAC_BASE_OFFSET + 0x14)
#define  REQ_ENTRY_REG        (REQ_MAC_BASE_OFFSET + 0x18)
#define  REQ_STATUS_REG       (REQ_MAC_BASE_OFFSET + 0x1c)
#define  REQ_DIRECT_REG       (REQ_MAC_BASE_OFFSET + 0x20)

/* HW MAC TABLE CMDS */
#define CMD_INIT		1
#define CMD_ADD			2
#define CMD_DEL			3
#define CMD_UPDATE		4
#define CMD_SEARCH		5
#define CMD_MEM_READ	6
#define CMD_MEM_WRITE	7
#define CMD_FLUSH		8

#define CLR_STATUS_REG				0xFFFFFFFF
#define TWO_FIELD_HASH_ENTRIES		0x200
#define MAC_HASH_ENTRIES			0x200
#define TWO_FIELD_COLL_ENTRIES		0x200
#define MAC_COLL_ENTRIES			0x200
#define TWO_FIELD_INIT_HEAD_PTR		0x200
#define MAC_INIT_HEAD_PTR			0x200
#define TWO_FIELD_INIT_TAIL_PTR		0x3ff
#define MAC_INIT_TAIL_PTR			0x3ff

#define VLAN_INIT_HEAD_PTR       0x40
#define VLAN_INIT_TAIL_PTR       0x7f
#define VLAN_FREE_LIST_ENTRIES   0x40

#define  VLAN_HASH_ENTRIES        0x40
#define  VLAN_COLL_HASH_ENTRIES   0x40

/* 2-filed mac table valid defines */
#define BIT_SET_MAC_2F_MACTBL     0x00000100
#define BIT_SET_VLAN_2F_MACTBL    0x00000200


#define BIT_SET_VLAN_1F_VLANTBL     0x00000100

/* VLAN (BD) Hash table action entry map:*
 *  *=======================================*
 *   * [07:00]         - forward port list
 *   * [15:08]         - untag list
 *   * [18:16]         - ucast_hit_action
 *   * [21:19]         - mcast_hit_action
 *   * [24:22]         - ucast_miss_action
 *   * [27:25]         - mcast_miss_action
 *   * [30:28]         - mstp
 *   * [31:31]         - Not used yet
 *   *=======================================*/
#define BRENTRY_FWD_PORT_LIST_START_POS        0
#define BRENTRY_UNTAG_LIST_START_POS           8
#define BRENTRY_UCAST_HIT_ACT_START_POS        16
#define BRENTRY_MCAST_HIT_ACT_START_POS        19
#define BRENTRY_UCAST_MISS_ACT_START_POS       22
#define BRENTRY_MCAST_MISS_ACT_START_POS       25
#define BRENTRY_MSTP_HIT_ACT_START_POS         28
#define BRENTRY_RESERVED_BIT_MASK_START_POS    31

#define BRENTRY_FWD_PORT_LIST_MASK   FWD_PORT_LIST_MASK /* 00 - 07 */
#define BRENTRY_UNTAG_LIST_MASK      0x0000FF00      /* 08 - 15 */
#define BRENTRY_UCAST_HIT_ACT_MASK   0x00070000      /* 16 - 18 */
#define BRENTRY_MCAST_HIT_ACT_MASK   0x00380000      /* 19 - 21 */
#define BRENTRY_UCAST_MISS_ACT_MASK  0x01c00000      /* 22 - 24 */
#define BRENTRY_MCAST_MISS_ACT_MASK  0x0E000000      /* 25 - 27 */
#define BRENTRY_MSTP_HIT_ACT_MASK    0x70000000      /* 28 - 30 */
#define BRENTRY_RESERVED_BIT_MASK    0x80000000      /* 31 -    */
#define BRENTRY_ACTION_ENTRY_MASK    0x7FFFFFFF      /* 00 - 30 */


/*  MAC 2F HASH TABLE MACROs */
/*  MAC Hash table action entry map
 *   *  * [07:00]           - forward port list              *
 *   *  * [10:8]            - TC                             *
 *   *  * [13:11]           - fwd_action (ACTION FIELD)      *
 *   *  * [25:12]           - Not used yet                   *
 *   *  * [26]              - cut through                    *
 *   *  * [28:27]           - reserved for feature block     *
 *   *  * [29]              - fresh                          *
 *   *  * [30]              - static                         */

#define MAC_2F_ENTRY_FWD_PORT_LIST_MASK         FWD_PORT_LIST_MASK
#define MAC_2F_ENTRY_FWD_ACT_MASK                0x00003800
#define MAC_2F_ENTRY_CUT_THROUGH_MASK            0x04000000
#define MAC_2F_ENTRY_FRESH_MASK                  0x20000000
#define MAC_2F_ENTRY_STATIC_MASK                 0x40000000
#define MAC_2F_ENTRY_TC_MASK                     0x700

#define MAC_2F_ENTRY_FWD_PORT_LIST_START_POS     0
#define MAC_2F_ENTRY_FWD_ACT_START_POS           11
#define MAC_2F_ENTRY_CUT_THROUGH_START_POS       26
#define MAC_2F_ENTRY_FRESH_BIT_POS               29
#define MAC_2F_ENTRY_STATIC_BIT_POS              30
#define MAC_2F_ENTRY_TC_START_POS                8

/*  !MAC 2F HASH TABLE MACROs */
#define STATUS_CMD_DONE              0x00000001
#define STATUS_ENTRY_NOT_FOUND       0x00000002
#define STATUS_INIT_DONE             0x00000004
#define STATUS_ENTRY_ADDED           0x00000008
#define STATUS_ENTRY_MATCH           0x00000010
#define CMD_ENTRY_PORTS_START_POS    16
#define CMD_ENTRY_FIELD_VALID_MASK   0x00001F00

//TPID: Tag Protocol Identifier
#define VLAN_TAGGED_FRAME        0x8100
//#define PROVIDER_BRIDGING_FRAME  0x88a8
//#define QinQ_9100_FRAME          0x9100
//#define QinQ_9200_FRAME          0x9200
//
#define STPID0                                0x5500    /*  Not Used by Anyone in IEEE page */
#define STPID1                                0x5501    /*  http://standards.ieee.org/develop/regauth/ethertype/eth.txt */
#define STPID2                                0x5502
#define STPID1_START_POS                      16
#define MAC_TABLE  1
#define VLAN_TABLE 2

// add for optimize ageing
#define AGEING_THRES 500
#define FAST_AGEING_THRES  1020

int vlan_hash_table_search(uint vlanid);
int vlan_hash_table_insert(uint field_entries, uint portNo, uint action_entry);
int mac_hash_table_update(uint field_entries[5], uint brentry, uint field_valids);
int mac_hash_table_add(uint field_entries[5], uint iport, uint brentry, uint field_valids);
int mac_hash_table_search(uint field_entries[5], uint field_valids);
int  hash_table_config_done(int type);
int add_vlan_entry_args(int vlanid, int port, int port_list, int untag_list);

void mac_hash_table_init(void);
void vlan_hash_table_init(void);
int vlan_hash_table_flush(void);

void host_ageing(void);
int sf_process_packet(uchar *data, uint data_len, struct rx_header *rxhdr,struct sf_priv *priv);
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
int vlan_hash_table_refresh(struct switch_dev *dev);
#endif
#ifdef CONFIG_SFAX8_SWITCH_VLAN
int sf_switch_init_broadcast_mac(int vlanid, int port_list);
#endif
#endif /* ifndef SF_SWITCH_HASH_TABLE_H */
