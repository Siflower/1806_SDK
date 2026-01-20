#ifndef _YT_TYPES_H
#define _YT_TYPES_H

/*
 * Include Files
 */
#if defined (__LINUX__)
#include <stdint.h>
#include <unistd.h>
#endif

#if !defined (__KERNEL__)
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#if !defined (MEM_ALLOC_BY_CUSTOM_FUNC)
#include <malloc.h>
#endif
#endif

#if defined (BOUNDSCHECK_FOR_FUNCS)
#include "securec.h"
#endif

/*
 * Symbol Definition
 */

/*
 * Macro Definition
 */ 
#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define LINE_LEN        128

#define MAC_ADDR_LEN                    6
#define IP4_ADDR_LEN                    4
#define IP6_ADDR_LEN                    16

#define YT_MAX_PORT                     32 /* max user port num */
#define YT_MAX_UNIT                     3

#if defined(SWITCH_SERIES_SHARK)
#define YT_MAX_PORT_PER_UNIT            29
#else
#define YT_MAX_PORT_PER_UNIT            11
#endif

#define YT_MAX_SERDES_PER_UNIT          6
#define YT_MAX_PHY_PER_UNIT             6
#define YT_PORTS_WORD_SIZE              32
#define YT_PORTS_WORD_NUM               ((YT_MAX_PORT +  31)/YT_PORTS_WORD_SIZE)
#define YT_VLAN_MAX_SIZE                4096
#define YT_VLANS_WORD_SIZE              32
#define YT_VLANS_WORD_NUM               ((YT_VLAN_MAX_SIZE +  31)/YT_VLANS_WORD_SIZE)
#define YT_VLANS_BIT_SIZE(max)          ((max +  31)/YT_VLANS_WORD_SIZE)

/*
 * Data Type Declaration
 */
#if !defined (__LINUX__)
typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef short                   int16_t;
#ifndef _UINT32_T_DECLARED
typedef unsigned int            uint32_t;
#endif

#ifndef _INT32_T_DECLARED
typedef int                     int32_t;
#endif
#endif

typedef unsigned long long      uint64;

typedef _Bool                   yt_bool_t;
typedef uint16_t                yt_vlan_t;
typedef uint16_t                yt_tpid_t;
typedef uint8_t                 yt_fid_entry_idx_t;
typedef uint16_t                yt_fid_t;
typedef uint8_t                 yt_mac_vlan_table_idx_t;
typedef uint8_t                 yt_ip_subnet_vlan_table_idx_t;
typedef uint16_t                yt_trans_tbl_id_t;
typedef uint32_t                yt_port_t;
typedef uint32_t                yt_macid_t;
typedef uint32_t                yt_ret_t;
typedef uint8_t                 yt_unit_t;
typedef uint8_t                 yt_tpidprofile_id_mask_t;
typedef uint8_t                 yt_stp_id_t;
typedef uint8_t                 yt_local_id_t;
typedef uint8_t                 yt_profile_id_t;
typedef uint8_t                 yt_meterid_t;
typedef uint8_t                 yt_pri_t;
typedef uint8_t                 yt_dscp_t;
typedef uint32_t                yt_rate_t;
typedef uint8_t                 yt_acl_bin_id_t;
typedef uint8_t                 yt_acl_phase_id_t;
typedef uint8_t                 yt_acl_hw_entry_id_t;
typedef uint8_t                 yt_acl_instance_id_t;
typedef uint8_t                 yt_acl_pattern_id_t;
typedef uint32_t                yt_cpu_code_t;
typedef uint32_t                yt_queue_pri_t;
typedef uint32_t                yt_queue_weight_t;
typedef uint16_t                yt_trans_tbl_id_t;
typedef uint32_t                yt_intr_status_t;
typedef uint8_t                 yt_mirror_group_t;
typedef uint32_t                yt_acl_entry_id_t;
typedef uint32_t                uint32;
typedef unsigned char           uint8;

typedef enum yt_fwd_type_e {
    FWD_TYPE_FWD,
    FWD_TYPE_DROP,
    FWD_TYPE_COPY,
    FWD_TYPE_TRAP,
}yt_fwd_type_t;

typedef enum  yt_l2_type_e
{
    L2_NONE,
    L2_ETHV2,
    L2_ETHSAP,
    L2_ETHSNAP,
#ifdef SWITCH_SERIES_SHARK
    L2_ETH_LOOP,
    L2_ETH_WOL,
#endif
    L2_ETHMAX
}yt_l2_type_t;

typedef struct yt_mac_addr_s
{
    uint8_t addr[MAC_ADDR_LEN];
} yt_mac_addr_t;

typedef uint32_t  yt_ip_addr_t;
typedef struct
{
	uint8_t	addr[IP6_ADDR_LEN];
}yt_ipv6_addr_t;

typedef struct yt_port_mask_s
{
    uint32_t portbits[YT_PORTS_WORD_NUM];
} yt_port_mask_t;

typedef enum yt_queue_type_e
{
    UNICAST_QUEUE,
    MULTICAST_QUEUE
}yt_queue_type_t;

typedef struct yt_qid_s
{
    uint8_t    port;
    yt_queue_type_t    qtype;
    uint8_t    qid;
}yt_qid_t;

typedef enum yt_rate_mode_e
{
    YT_RATE_MODE_BPS,
    YT_RATE_MODE_PPS,
}yt_rate_mode_t;

/* port medium type */
typedef enum yt_port_medium_e
{
    PORT_MEDI_COPPER	= 0,      /* port's PHY media is copper */
    PORT_MEDI_FIBER,       /* port's PHY media is fiber */
    PORT_MEDI_COMBO_FIBER	,   /* port's PHY media is combo and fiber first */
    PORT_MEDI_COMBO_COPPER,   /* port's PHY media is combo and copper first */
    PORT_MEDI_SERDES,      /* port media is serdes */
    PORT_MEDI_END
}yt_port_medium_t;

typedef enum yt_port_speed_e
{
    PORT_SPEED_10M = 0,
    PORT_SPEED_100M,
    PORT_SPEED_1000M,
    PORT_SPEED_2500M,
    PORT_SPEED_5G,
    PORT_SPEED_10G,
    PORT_SPEED_END
}yt_port_speed_t;

typedef enum yt_port_duplex_e
{
    PORT_DUPLEX_HALF = 0,
    PORT_DUPLEX_FULL
}yt_port_duplex_t;

typedef enum yt_port_speed_duplex_e
{
    PORT_SPEED_DUP_10HALF = 0,
    PORT_SPEED_DUP_10FULL,
    PORT_SPEED_DUP_100HALF,
    PORT_SPEED_DUP_100FULL,
    PORT_SPEED_DUP_1000FULL,
    PORT_SPEED_DUP_2500FULL,
    PORT_SPEED_DUP_5GFULL,
    PORT_SPEED_DUP_10GFULL,
    PORT_SPEED_DUP_END
}yt_port_speed_duplex_t;

typedef struct yt_port_an_ability_s
{
    yt_bool_t   half_10_en;
    yt_bool_t   full_10_en;
    yt_bool_t   half_100_en;
    yt_bool_t   full_100_en;
    yt_bool_t   full_1000_en;
    yt_bool_t   full_2500_en;
    yt_bool_t   fc_en;//flow control
    yt_bool_t   asyFC_en;//asymmetric flow control
}yt_port_an_ability_t;

typedef enum yt_port_link_status_e
{
    PORT_LINK_DOWN = 0,
    PORT_LINK_UP
}yt_port_link_status_t;

typedef enum yt_phy_type_e
{
    PHY_INTERNAL,
    PHY_EXTERNAL,
    PHY_TYPE_MAX,
}yt_phy_type_t;

#define YT_PHY_NONE                      (0)
#define YT_PHY_INT                       BIT(PHY_INTERNAL)
#define YT_PHY_EXT                       BIT(PHY_EXTERNAL)
#define YT_PHY_INTEXT                    (YT_PHY_INT | YT_PHY_EXT)

typedef enum yt_act_type_e {
    ACT_TYPE_FWD,
    ACT_TYPE_DROP,
    ACT_TYPE_COPY,
    ACT_TYPE_TRAP,
}yt_act_type_t;

typedef enum yt_enable_e {
    YT_DISABLE = 0,
    YT_ENABLE = 1,
}yt_enable_t;

typedef enum yt_extif_mode_e
{
    YT_EXTIF_MODE_MII = 0,
    YT_EXTIF_MODE_REMII,
    YT_EXTIF_MODE_RMII_MAC,
    YT_EXTIF_MODE_RMII_PHY,
    YT_EXTIF_MODE_RGMII,
    YT_EXTIF_MODE_XMII_DISABLE,//disable external port  mode
    YT_EXTIF_MODE_SG_MAC,
    YT_EXTIF_MODE_SG_PHY,
    YT_EXTIF_MODE_FIB_1000,
    YT_EXTIF_MODE_FIB_100,
    YT_EXTIF_MODE_BX2500,
    YT_EXTIF_MODE_SGFIB_AS,
    YT_EXTIF_MODE_QSGMII,
    YT_EXTIF_MODE_10GQXGMII,
    YT_EXTIF_MODE_USXGMII_10G,
}yt_extif_mode_t;

typedef struct yt_port_linkStatus_all_s
{
    yt_port_link_status_t link_status;
    yt_port_speed_t link_speed;
    yt_port_duplex_t    link_duplex;
    yt_bool_t   rx_fc_en;
    yt_bool_t   tx_fc_en;
}yt_port_linkStatus_all_t;

typedef struct yt_iso_vlan_info_s
{
    yt_vlan_t min_vid;
    yt_vlan_t max_vid;
    yt_port_mask_t iso_msk;
}yt_iso_vlan_info_t;

typedef struct yt_vlan_mask_s
{
    uint32_t vlanbits[YT_VLANS_WORD_NUM];
}yt_vlan_mask_t;

typedef enum yt_phy_chip_mode_e
{
    YT_PHY_CHIP_MODE_COPPER = 0,
    YT_PHY_CHIP_MODE_FIBER,
    YT_PHY_CHIP_MODE_COMBO,
    YT_PHY_CHIP_MODE_AUTO_BX2500_SGMII,
    YT_PHY_CHIP_MODE_FORCE_BX2500,
    YT_PHY_CHIP_MODE_FIBER_FORCE_BX2500,
    YT_PHY_CHIP_MODE_RESERVED
}yt_phy_chip_mode_t;

typedef enum yt_combo_mode_e
{
    COMBO_MODE_COPPER_FIRST = 0,
    COMBO_MODE_FIBER_FIRST
}yt_combo_mode_t;

typedef enum yt_port_cable_status_e
{
    PORT_CABLE_STATUS_OK,
    PORT_CABLE_STATUS_UNKNOWN,
    PORT_CABLE_STATUS_SHORT,
    PORT_CABLE_STATUS_OPEN,
    PORT_CABLE_STATUS_OPEN_AND_SHORT,
    PORT_CABLE_STATUS_END
} yt_port_cable_status_t;

typedef struct yt_port_cableDiag_s
{
yt_bool_t pair_valid[4]; /* pair_status valid */
yt_port_cable_status_t pair_status[4];
uint16_t pair_length[4]; /* pair length in cent meters */
#ifdef MULTI_CHANNEL_PHY
uint16_t multi_pair_length[3][4];
#endif
} yt_port_cableDiag_t;

typedef enum yt_utp_template_testmode_e
{
    YT_UTP_TEMPLATE_TMODE_10M_10MSINE = 0, /*10MHz sine wave*/
    YT_UTP_TEMPLATE_TMODE_10M_PRANDOM, /*prseudo random*/
    YT_UTP_TEMPLATE_TMODE_10M_LINKPULSE, /*normal link pulse only*/
    YT_UTP_TEMPLATE_TMODE_10M_5MSINE, /*5MHz sine wave*/
    YT_UTP_TEMPLATE_TMODE_10M_NORMAL, /*normal mode*/
    YT_UTP_TEMPLATE_TMODE_100M_T1, /*waveform test*/
    YT_UTP_TEMPLATE_TMODE_100M_T2, /*jitter test on master*/
    YT_UTP_TEMPLATE_TMODE_100M_T3, /*jitter test on slave*/
    YT_UTP_TEMPLATE_TMODE_100M_T4, /*distortion test*/
    YT_UTP_TEMPLATE_TMODE_100M_MDI,
    YT_UTP_TEMPLATE_TMODE_100M_MDIX = 10,
    YT_UTP_TEMPLATE_TMODE_1000M_T1, /*waveform test*/
    YT_UTP_TEMPLATE_TMODE_1000M_T2, /*jitter test on master*/
    YT_UTP_TEMPLATE_TMODE_1000M_T3, /*jitter test on slave*/
    YT_UTP_TEMPLATE_TMODE_1000M_T4 = 14, /*distortion test*/
    YT_UTP_TEMPLATE_TMODE_SDS2500M,
    YT_UTP_TEMPLATE_TMODE_SDS1000M,
    YT_UTP_TEMPLATE_TMODE_2500M_T1, /*Test Mode 1, Transmit waveform test */
    YT_UTP_TEMPLATE_TMODE_2500M_T2, /* Test Mode 1, Transmit waveform test */
    YT_UTP_TEMPLATE_TMODE_2500M_T3, /* Test Mode 3, Transmit Jitter test (slave mode) */
    YT_UTP_TEMPLATE_TMODE_2500M_T4_DUALTONE_1, /* Test Mode 4, Dual tone 1 */
    YT_UTP_TEMPLATE_TMODE_2500M_T4_DUALTONE_2, /* Test Mode 4, Dual tone 2 */
    YT_UTP_TEMPLATE_TMODE_2500M_T4_DUALTONE_3, /* Test Mode 4, Dual tone 3 */
    YT_UTP_TEMPLATE_TMODE_2500M_T4_DUALTONE_4, /* Test Mode 4, Dual tone 4 */
    YT_UTP_TEMPLATE_TMODE_2500M_T4_DUALTONE_5, /* Test Mode 4, Dual tone 5 */
    YT_UTP_TEMPLATE_TMODE_2500M_T5, /* Test Mode 5, PSD mask and power level test  */
    YT_UTP_TEMPLATE_TMODE_2500M_T6, /* Test Mode 6, Transmitter droop test */
    YT_UTP_TEMPLATE_TMODE_2500M_T7, /* Test Mode 7, Pseudo-random test mode */
    YT_UTP_TEMPLATE_TMODE_NONE        /* Disable test mode */
}yt_utp_template_testmode_t;

typedef enum yt_phy_loopback_mode_e
{
    YT_PHY_LOOPBACK_MODE_INTERNAL,
    YT_PHY_LOOPBACK_MODE_EXTERNAL,
    YT_PHY_LOOPBACK_MODE_REMOTE,
    YT_PHY_LOOPBACK_MODE_DISABLE
}yt_phy_loopback_mode_t;

typedef struct yt_utp_snr_s
{
    uint16_t snr[4];
}yt_utp_snr_t;

typedef enum yt_utp_crossover_mode_e
{
    YT_UTP_CROSSOVER_MODE_MDI = 0,
    YT_UTP_CROSSOVER_MODE_MDIX,
    YT_UTP_CROSSOVER_MODE_RESV,
    YT_UTP_CROSSOVER_MODE_AUTO
}yt_utp_crossover_mode_t;

typedef enum yt_utp_crossover_status_e
{
    YT_UTP_CROSSOVER_STATUS_MDI = 0,
    YT_UTP_CROSSOVER_STATUS_MDIX = 1
}yt_utp_crossover_status_t;

typedef enum cmm_err_e
{
    CMM_ERR_OK                   = 0,
    CMM_ERR_FAIL                 = 1,
    CMM_ERR_NULL_POINT  = 2,
    CMM_ERR_NOT_SUPPORT,
    CMM_ERR_NOT_INIT,
    CMM_ERR_INPUT = 5,
    CMM_ERR_REG_TABLE_NUM,
    CMM_ERR_REG_TABLE_OP,
    CMM_ERR_TABLE_FULL,
    CMM_ERR_ENTRY_NOT_FOUND,
    CMM_ERR_REG_TABLE_IDX = 10,
    CMM_ERR_SAMEENTRY_EXIST,
    CMM_ERR_ENTRY_FULL,
    CMM_ERR_FDB_OP_BUSY,
    CMM_ERR_PORT,
    CMM_ERR_PORTLIST = 15,
    CMM_ERR_BUSYING_TIME,
    CMM_ERR_EXCEED_RANGE,
    CMM_ERR_PORT_BEEN_IN_LAG,
    CMM_ERR_TOO_LESS_INFO,
    CMM_ERR_ACL_NOT_INIT,
    CMM_ERR_ACL_INSTANCE_NOT_FOUND,
    CMM_ERR_ACL_INSTANCE_FULL,
    CMM_ERR_ACL_KEY_SELECT_FAIL,
    CMM_ERR_ACL_ENTRY_NOT_FOUND,
    CMM_ERR_ACL_PATTERN_NOT_FOUND,
    CMM_ERR_ACL_BIN_NOT_ENOUGH,
    CMM_ERR_ACL_INVALID_KEY_ON_INSTANCE,
    CMM_ERR_ACL_OP_BUSY,
    CMM_ERR_ACL_OP_PROHIBIT,
    CMM_ERR_FORBIDDEN,
    CMM_ERR_ENTRY_OVERLAPPING,
    CMM_ERR_ENTRY_INVALID,
    CMM_ERR_MAX,
} cmm_err_t;

typedef enum yt_vlan_type_e
{
    YT_VLAN_TYPE_CVLAN,
    YT_VLAN_TYPE_SVLAN,
}yt_vlan_type_t;

#endif //YT_TYPES_H

