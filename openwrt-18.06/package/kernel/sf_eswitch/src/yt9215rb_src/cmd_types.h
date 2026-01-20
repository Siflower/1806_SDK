#ifndef _CMD_TYPES_H
#define _CMD_TYPES_H

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
#include "yt_l2.h"
#include "yt_stat.h"
#include "yt_port.h"
#if defined (BOUNDSCHECK_FOR_FUNCS)
#include "securec.h"
#endif
#include "yt_mirror.h"
#include "yt_qos.h"


#define CMD_DOS_TYPE_UDP_SPORT_EQ_DPORT     BIT(0U)
#define CMD_DOS_TYPE_TCP_SPORT_EQ_DPORT     BIT(1U)
#define CMD_DOS_TYPE_TCP_HEADER_PARTIAL      BIT(2U)
#define CMD_DOS_TYPE_TCP_FRAG_NOT_FIRST       BIT(3U)
#define CMD_DOS_TYPE_ICMP_FRAG                             BIT(4U)
#define CMD_DOS_TYPE_LARGE_IPV4_PAYLOAD      BIT(5U)
#define CMD_DOS_TYPE_LARGE_IPV6_PAYLOAD      BIT(6U)
#define CMD_DOS_TYPE_LARGE_IPV4_ICMP               BIT(7U)
#define CMD_DOS_TYPE_LARGE_IPV6_ICMP               BIT(8U)
#define CMD_DOS_TYPE_LAND                                         BIT(9U)
#define CMD_DOS_TYPE_DA_ZERO                                  BIT(10U)
#define CMD_DOS_TYPE_SA_MC                                       BIT(11U)
#define CMD_DOS_TYPE_SA_BC                                        BIT(12U)
#define CMD_DOS_TYPE_SA_ZERO                                  BIT(13U)
#define CMD_DOS_TYPE_SA_EQ_DA                                BIT(14U)
#define CMD_DOS_TYPE_TCP_XMAS_SCAN                   BIT(15U)
#define CMD_DOS_TYPE_TCP_NULL_SCAN                    BIT(16U)
#define CMD_DOS_TYPE_TCP_SYN_PORTLESS1024     BIT(17U)
#define CMD_DOS_TYPE_TCP_SYNFIN_SCAN                 BIT(18U)
#define CMD_DOS_TYPE_TCP_SYNRST_SCAN                BIT(19U)
#define CMD_DOS_TYPE_TCP_ALLFLAGS_SCAN          BIT(20U)
#define CMD_DOS_TYPE_CFI_MISMATCH                       BIT(21U)

typedef enum cmd_mib_clear_mode_e
{
    CLEAR_BY_PORT,
    CLEAR_BY_ALL,
}cmd_mib_clear_mode_t;

typedef enum cmd_fdb_add_ucast_mode_e
{
    IS_PORT,
    IS_LAG,
}cmd_fdb_add_ucast_mode_t;

typedef enum cmd_port_oper_mode_e
{
    MAC_OPER,
    PHY_OPER,
}cmd_port_oper_mode_t;

typedef enum cmd_phy_reg_type
{
    MII,
    EXT
}cmd_phy_reg_type_t;

enum cmd_phy_type_e
{
    INT_PHY,
    EXT_PHY,
};
typedef enum cmd_phy_type_e cmd_phy_type_t;

#define CMD_YT_ACTION_INIT    (1)
#define CMD_YT_ACTION_EXIT    (2)

typedef struct cmd_mib_get_params_s
{
    yt_port_t port;
    yt_stat_mib_port_cnt_t mib_port_cnt;
}cmd_mib_get_params_t;

typedef struct cmd_mib_clear_params_s
{
    yt_port_t port;
    cmd_mib_clear_mode_t mib_clear_mode;
}cmd_mib_clear_params_t;

typedef struct cmd_mirror_params_s
{
    yt_mirror_entry_t mirrorEntry;
    yt_mirror_group_t grpId;
}cmd_mirror_params_t;

typedef struct cmd_port_extifmode_params_s
{
    yt_port_t port;
    yt_extif_mode_t extif_mode;
}cmd_port_extifmode_params_t;

typedef struct cmd_port_eee_timer_params_s
{
    yt_port_t port;
    uint32_t wakeupTimer;
    uint32_t sleepTimer;
}cmd_port_eee_timer_params_t;

typedef struct cmd_port_rgmii_delay_s
{
    yt_port_t port;
    uint8_t rxDelay;
    uint8_t txDelay;
    yt_enable_t txcState;
}cmd_port_rgmii_delay_t;

typedef struct cmd_get_port_extifmode_params_s
{
    yt_port_t port;
    yt_extif_mode_t extif_mode;
}cmd_get_port_extifmode_params_t;

typedef struct cmd_get_port_fc_params_s
{
    yt_port_t port;
    yt_enable_t fcAutoNeg;
    yt_enable_t fcRxState;
    yt_enable_t fcTxState;
}cmd_get_port_fc_params_t;

typedef struct cmd_get_port_force_params_s
{
    yt_port_speed_duplex_t forceConfig;
    yt_enable_t macAutoNeg;
    yt_enable_t fcAutoNeg;
}cmd_get_port_force_params_t;

typedef struct cmd_get_lag_en_params_s
{
    yt_port_t  port;
    yt_enable_t lagState;
    uint8_t lagId;
}cmd_get_lag_en_params_t;

typedef struct cmd_get_port_mac_en_params_s
{
    yt_port_t port;
    yt_enable_t rxMacState;
    yt_enable_t txMacState;
}cmd_get_port_mac_en_params_t;

typedef struct cmd_set_port_autoneg_params_s
{
    yt_port_t port;
    yt_enable_t state;
}cmd_set_port_autoneg_params_t;

typedef struct cmd_set_port_jumbo_params_s
{
    yt_port_t port;
    yt_enable_t state;
}cmd_set_port_jumbo_params_t;

typedef struct cmd_set_port_jumbo_size_params_s
{
    yt_port_t port;
    uint32_t size;
}cmd_set_port_jumbo_size_params_t;

typedef struct cmd_get_port_autoneg_params_s
{
    yt_port_t port;
    yt_enable_t state;
}cmd_get_port_autoneg_params_t;

typedef struct cmd_get_port_state_params_s
{
    yt_port_t port;
    yt_enable_t state;
    cmd_port_oper_mode_t operMode;
    yt_phy_chip_mode_t phyMode;
}cmd_get_port_state_params_t;

typedef struct cmd_set_port_state_params_s
{
    yt_port_t port;
    yt_enable_t state;
    cmd_port_oper_mode_t operMode;
    yt_phy_chip_mode_t phyMode;
}cmd_set_port_state_params_t;

typedef struct cmd_set_port_fc_state_params_s
{
    yt_port_t port;
     yt_port_an_ability_t port_ability;
}cmd_set_port_fc_state_params_t;

typedef struct cmd_set_port_force_ctrl_params_s
{
    yt_port_t port;
    yt_port_speed_duplex_t speed_duplex;
}cmd_set_port_force_ctrl_params_t;

typedef struct cmd_isolation_info_s {
    yt_iso_vlan_info_t vlanInfo;
    uint32_t valid;
}cmd_isolation_info_t;

typedef struct cmd_qos_que_map_param_s
{
    yt_port_t port;
    yt_queue_type_t qtype;
    yt_qos_qmap_t qmap;
}cmd_qos_que_map_param_t;

typedef struct cmd_port_status_s
{
    yt_port_t port;
    yt_port_link_status_t portStatus;
}cmd_port_status_t;

typedef struct cmd_port_status_all_s
{
    yt_port_t port;
    yt_port_linkStatus_all_t portStatusAll;
}cmd_port_status_all_t;

typedef struct cmd_port_state_s
{
    yt_port_t port;
    yt_port_cableDiag_t status;
    yt_phy_chip_mode_t phyMode;
}cmd_port_state_t;

typedef struct cmd_port_combo_mode_s
{
    yt_port_t port;
    yt_combo_mode_t mode;
}cmd_port_combo_mode_t;

typedef struct cmd_port_chip_mode_s
{
    yt_port_t port;
    yt_phy_chip_mode_t mode;
}cmd_port_chip_mode_t;

typedef struct cmd_port_forceCtrl_s
{
    yt_port_t port;
    cmd_get_port_force_params_t forceCtrl;
}cmd_port_forceCtrl_t;

typedef struct cmd_port_gap_s
{
    yt_port_t port;
    uint8_t size;
}cmd_port_gap_t;

typedef struct cmd_phy_crossover_s
{
    yt_port_t port;
    yt_utp_crossover_mode_t crossoverMode;
}cmd_phy_crossover_t;

typedef enum cmd_qos_pri_type_e
{
    PRI_TYPE_CPRI,
    PRI_TYPE_SPRI,
}cmd_qos_pri_type_t;

typedef struct cmd_get_port_csd_info_s
{
    yt_port_t port;
    yt_port_cableDiag_t csdInfo;
}cmd_get_port_csd_info_t;

typedef struct cmd_qos_pri_map_param_s
{
    cmd_qos_pri_type_t type;
    yt_qos_pmap_tci_t pri;
    yt_pri_t int_pri;
}cmd_qos_pri_map_param_t;

#define CMD_QOS_DWRR_FLAG_MODE      BIT(0)
#define CMD_QOS_DWRR_FLAG_WEIGHT      BIT(1)
typedef struct cmd_qos_dwrr_param_s
{
    uint32_t flags;
    yt_qid_t qinfo;
    yt_rate_mode_t mode;
    yt_queue_weight_t weight;
}cmd_qos_dwrr_param_t;

typedef struct cmd_qos_remark_pri_s
{
    cmd_qos_pri_type_t type;
    yt_qos_remark_info_t srcInfo;
    yt_qos_pmap_tci_t dstInfo;
}cmd_qos_remark_pri_t;

typedef enum cmd_fdb_flush_e
{
    FLUSH_PORT,
    FLUSH_LAG,
    FLUSH_VLAN,
    FLUSH_PORT_VLAN,
    FLUSH_LAG_VLAN,
    FLUSH_ALL,
}cmd_fdb_flush_t;

typedef enum cmd_fdb_count_e
{
    COUNT_PORT,
    COUNT_LAG,
    COUNT_ALL,
}cmd_fdb_count_t;

typedef enum cmd_fdb_learnlimit_e
{
    LEARNLIMIT_PORT,
    LEARNLIMIT_LAG,
    LEARNLIMIT_VLANGROUP,
    LEARNLIMIT_SYSTERM,
}cmd_fdb_learnlimit_t;

typedef enum cmd_mcast_flush_e
{
    MCAST_FLUSH_VLAN,
    MCAST_FLUSH_ALL,
}cmd_mcast_flush_t;

typedef enum cmd_dot1x_flow_type_e
{
    CMD_DOT1X_MC = 0,
    CMD_DOT1X_BC,
    CMD_DOT1X_TYPE_END
} cmd_dot1x_flow_type_t;

typedef enum cmd_dot1x_flow_direction_e
{
    CMD_DOT1X_RX = 0,
    CMD_DOT1X_TX,
    CMD_DOT1X_FLOW_DIR_END
} cmd_dot1x_flow_direction_t;

typedef enum cmd_igmp_mld_learnlimit_e
{
    PORT_LEARNLIMIT,
    LAG_LEARNLIMIT,
    SYSTERM_LEARNLIMIT,
}cmd_igmp_mld_learnlimit_t;

#endif //CMD_TYPES_H

