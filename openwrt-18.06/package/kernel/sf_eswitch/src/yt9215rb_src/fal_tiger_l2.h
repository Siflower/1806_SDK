/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_l2.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_L2_H
#define __FAL_TIGER_L2_H

#include "yt_types.h"
#include "yt_l2.h"
#include "yt_multicast.h"
#include "yt_vlan.h"
//will add macro later

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FDB_BUCKET_TYPE_NUM     2

#define FDB_STATUS_INVALID      0
#define FDB_STATUS_MIN_TIME     1
#define FDB_STATUS_MAX_TIME     5
#define FDB_STATUS_PENDING      6
#define FDB_STATUS_STATIC       7
#define FDB_STATUS_MOVE_AGING_MAX_TIME 3
#define FDB_BUSY_CHECK_NUMBER   1000

typedef enum yt_l2_learn_algorithm_e
{
    YT_L2_LEARN_ALGO_CRC,
    YT_L2_LEARN_ALGO_XOR,
} yt_l2_learn_algorithm_t;

typedef enum yt_l2_unknown_pkt_action_e {
    L2_UNKNOWN_ACTION_FWD = 0,
    L2_UNKNOWN_ACTION_TRAP,
    L2_UNKNOWN_ACTION_DROP,
    L2_UNKNOWN_ACTION_COPY,
    L2_UNKNOWN_ACTION_END
}yt_l2_unknown_pkt_action_t;

typedef struct yt_l2_learn_algo_cfg_e
{
    yt_l2_learn_algorithm_t cfg[FDB_BUCKET_TYPE_NUM];
} yt_l2_learn_algo_cfg_t;

typedef struct yt_l2_fdb_info_s {
    uint8_t     status;
    uint8_t     dmacIntPriEn;
    uint16_t    newVid;
    uint8_t     intPri;
    uint8_t     smacIntPriEn;
    uint8_t     copyToCpu;
    uint8_t     dmacDrop;
    uint16_t    dstPortMask;
    uint8_t     smacDrop;
}yt_l2_fdb_info_t;

typedef enum fal_tiger_l2_fdb_op_cmd_e {
    FAL_TIGER_FDB_OP_CMD_ADD,
    FAL_TIGER_FDB_OP_CMD_DEL,
    FAL_TIGER_FDB_OP_CMD_GET_ONE,
    FAL_TIGER_FDB_OP_CMD_GET_NEXT,
    FAL_TIGER_FDB_OP_CMD_FLUSH,
}fal_tiger_l2_fdb_op_cmd_t;

typedef enum fal_tiger_l2_fdb_get_next_type_e {
    FAL_TIGER_FDB_GET_NEXT_UCAST_ONE_BY_PORT,
    FAL_TIGER_FDB_GET_NEXT_UCAST_ONE_BY_FID,
    FAL_TIGER_FDB_GET_NEXT_UCAST_ONE,
    FAL_TIGER_FDB_GET_NEXT_MCAST_ONE,
}fal_tiger_l2_fdb_get_next_type_t;

typedef enum yt_l2_tbl_op_flush_mode_e {
    L2_FDB_FLUSH_MODE_UFDB_ALL,    
    L2_FDB_FLUSH_MODE_UFDB_PORT,    
    L2_FDB_FLUSH_MODE_UFDB_PORT_FID,    
    L2_FDB_FLUSH_MODE_UFDB_FID,    
    L2_FDB_FLUSH_MODE_MFDB_ALL,
    L2_FDB_FLUSH_MODE_MFDB_PORT,   
    L2_FDB_FLUSH_MODE_MFDB_PORT_FID,    
    L2_FDB_FLUSH_MODE_MFDB_FID,
}yt_l2_tbl_op_flush_mode_t;

typedef struct yt_l2_tbl_flush_ctrl_s {
    yt_l2_tbl_op_flush_mode_t mode;
    uint8_t flushStaticEn;
    yt_vlan_t fid;
    yt_port_mask_t portMask;
}yt_l2_tbl_flush_ctrl_t;

typedef struct yt_l2_fdb_op_result_s {
    uint16_t    entryIdx;
    uint8_t     opResult;
    uint8_t     overwrite;
    uint8_t     lookupFail;
    uint8_t     opDone;
} yt_l2_fdb_op_result_t;

typedef enum yt_l2_fdb_op_mode_e {
    L2_FDB_OP_MODE_HASH,
    L2_FDB_OP_MODE_INDEX,
}_yt_l2_fdb_op_mode_t;

typedef struct yt_l2_fdb_op_mode_s {
    _yt_l2_fdb_op_mode_t l2FdbOpMode;
    uint16_t    entryIdx;
}yt_l2_fdb_op_mode_t;

/**
 * @internal      fal_tiger_l2_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_l2_fdb_mcast_addr_add
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_mcast_addr_add(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_l2_fdb_ucast_addr_add
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[in]     port                -port num
 * @param[in]     isLag               -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_ucast_addr_add(yt_unit_t unit, const l2_ucastMacAddr_info_t *pUcastMac);


/**
 * @internal      fal_tiger_l2_fdb_ucast_addr_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_ucast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr);


/**
 * @internal      fal_tiger_l2_fdb_mcast_addr_del
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_mcast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr);


/**
 * @internal      fal_tiger_l2_fdb_linkdownFlush_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_linkdownFlush_en_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_linkdownFlush_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_linkdownFlush_en_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_fdb_all_ucast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_all_ucast_flush(yt_unit_t unit);


/**
 * @internal      fal_tiger_l2_fdb_port_ucast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_port_ucast_flush(yt_unit_t unit, yt_port_t port, yt_bool_t isLag);


/**
 * @internal      fal_tiger_l2_fdb_vlan_ucast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_vlan_ucast_flush(yt_unit_t unit, yt_vlan_t vid);


/**
 * @internal      fal_tiger_l2_fdb_all_mcast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_all_mcast_flush(yt_unit_t unit);


/**
 * @internal      fal_tiger_l2_fdb_port_mcast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_port_mcast_flush(yt_unit_t unit, yt_port_t port);


/**
 * @internal      fal_tiger_l2_fdb_vlan_mcast_flush
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_vlan_mcast_flush(yt_unit_t unit, yt_vlan_t vid);


/**
 * @internal      fal_tiger_l2_fdb_type_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[out]    ptype               -FDB type,dynamic or static
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_type_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_l2_fdb_type_t *ptype);


/**
 * @internal      fal_tiger_l2_port_learnlimit_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_port_learnlimit_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_port_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     maxcnt              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_cnt_set(yt_unit_t unit, yt_port_t port, uint32_t maxcnt);


/**
 * @internal      fal_tiger_l2_port_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pmaxcnt             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_cnt_get(yt_unit_t unit, yt_port_t port, uint32_t *pmaxcnt);


/**
 * @internal      fal_tiger_l2_port_learnlimit_exceed_drop_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);


/**
 * @internal      fal_tiger_l2_port_learnlimit_exceed_drop_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_port_learnlimit_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);


/**
 * @internal      fal_tiger_l2_system_learnlimit_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_system_learnlimit_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_system_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     maxcnt              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_cnt_set(yt_unit_t unit, uint32_t maxcnt);


/**
 * @internal      fal_tiger_l2_system_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pmaxcnt             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_cnt_get(yt_unit_t unit, uint32_t *pmaxcnt);


/**
 * @internal      fal_tiger_l2_system_learnlimit_exceed_drop_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_act_set(yt_unit_t unit, yt_act_type_t actType);


/**
 * @internal      fal_tiger_l2_system_learnlimit_exceed_drop_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_system_learnlimit_act_get(yt_unit_t unit, yt_act_type_t *pActType);


/**
 * @internal      fal_tiger_l2_fdb_drop_sa_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_drop_sa_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_drop_sa_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_drop_sa_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_fdb_drop_da_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_drop_da_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_drop_da_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_drop_da_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_fdb_copy2cpu_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_copy2cpu_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_copy2cpu_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -yt_types.h
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_copy2cpu_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_filter_mcast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_l2_filter_mcast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      fal_tiger_l2_filter_bcast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_bcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_l2_filter_bcast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_bcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      fal_tiger_l2_filter_unknown_ucast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_unknown_ucast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_l2_filter_unknown_ucast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_unknown_ucast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);

/**
 * @internal      fal_tiger_l2_filter_unknown_mcast_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_unknown_mcast_set(yt_unit_t unit, yt_port_mask_t port_mask);


/**
 * @internal      fal_tiger_l2_filter_unknown_mcast_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -yt_types.h
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_filter_unknown_mcast_get(yt_unit_t unit, yt_port_mask_t *pport_mask);


/**
 * @internal      fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern uint32_t  fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set(yt_unit_t unit, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_port_uc_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pcnt                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_port_uc_cnt_get(yt_unit_t unit, yt_port_t port, uint32 *pcnt);


/**
 * @internal      fal_tiger_l2_fdb_lag_uc_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[out]    pcnt                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_lag_uc_cnt_get(yt_unit_t unit, uint8_t groupid, uint32_t *pcnt);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_en_set(yt_unit_t unit, uint8_t groupid,  yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_en_get(yt_unit_t unit, uint8_t groupid,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_cnt_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[in]     maxcnt              -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_cnt_set(yt_unit_t unit, uint8_t groupid,  uint32_t maxcnt);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[out]    pmaxcnt             -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_cnt_get(yt_unit_t unit, uint8_t groupid,  uint32_t *pmaxcnt);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_exceed_drop_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_act_set(yt_unit_t unit, uint8_t groupId, yt_act_type_t actType);


/**
 * @internal      fal_tiger_l2_lag_learnlimit_exceed_drop_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     groupid             -x
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_lag_learnlimit_act_get(yt_unit_t unit, uint8_t groupId, yt_act_type_t *pActType);


/**
 * @internal      fal_tiger_l2_fdb_uc_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pcnt                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_uc_cnt_get(yt_unit_t unit,  uint32 *pcnt);


/**
 * @internal      fal_tiger_l2_fdb_mc_cnt_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    pcnt                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_mc_cnt_get(yt_unit_t unit,  uint32 *pcnt);


/**
 * @internal      fal_tiger_l2_fdb_aging_port_en_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -yt port
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_aging_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      fal_tiger_l2_fdb_aging_port_en_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_aging_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_l2_fdb_aging_time_set
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[in]     sec                 -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_aging_time_set(yt_unit_t unit,  uint32_t sec);


/**
 * @internal      fal_tiger_l2_fdb_aging_time_get
 * @endinternal
 *
 * @brief         Description
 * @param[in]     unit                -unit id
 * @param[out]    psec                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_l2_fdb_aging_time_get(yt_unit_t unit,  uint32_t *psec);


/**
 * @internal      fal_tiger_l2_fdb_uc_withindex_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index               -x
 * @param[out]    pUcastMac           -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_fdb_uc_withindex_get(yt_unit_t unit, uint16_t index, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      fal_tiger_l2_fdb_uc_withMacAndVid_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     mac_addr            -mac address
 * @param[out]    pUcastMac           -ucast mac info,refer to l2_ucastMacAddr_info_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -no valid mac on this index
 */
extern yt_ret_t  fal_tiger_l2_fdb_uc_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      fal_tiger_l2_fdb_uc_withindex_getnext
 * @endinternal
 *
 * @brief         get next vaild ucast info by current index
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     index                 -current index
 * @param[out]    pNext_index            -the index of next vaild entry
 * @param[out]    pUcastMac           -ucast mac info,refer to l2_ucastMacAddr_info_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_ENTRY_NOT_FOUND -no valid mac entry on this index next
 */
extern yt_ret_t  fal_tiger_l2_fdb_uc_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNext_index, l2_ucastMacAddr_info_t *pUcastMac);

/**
 * @internal      fal_tiger_l2_port_learn_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port               -port id
 * @param[in]     enable            -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_port_learn_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      fal_tiger_l2_fdb_uc_withindex_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port               -port id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_l2_port_learn_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

extern yt_ret_t  fal_tiger_l2_fdb_vlan_port_ucast_flush(yt_unit_t unit, yt_vlan_t vid, yt_port_t port, yt_bool_t isLag);
extern yt_ret_t  fal_tiger_l2_port_learn_mode_set(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t mode);
extern yt_ret_t  fal_tiger_l2_port_learn_mode_get(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t *pMode);
extern yt_ret_t  fal_tiger_l2_port_suspend_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);
extern yt_ret_t  fal_tiger_l2_port_suspend_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);
extern yt_ret_t  fal_tiger_l2_new_sa_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType);
extern yt_ret_t  fal_tiger_l2_new_sa_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType);
extern yt_ret_t  fal_tiger_l2_unknown_ucast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action);
extern yt_ret_t  fal_tiger_l2_unknown_ucast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction);
extern yt_ret_t  fal_tiger_l2_unknown_mcast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action);
extern yt_ret_t  fal_tiger_l2_unknown_mcast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction);
extern yt_ret_t  fal_tiger_l2_fdb_add_notify_register(yt_unit_t unit, l2_fdb_add_notify func, yt_l2_fdb_notify_type_t type);
extern yt_ret_t  fal_tiger_l2_fdb_del_notify_register(yt_unit_t unit, l2_fdb_del_notify func, yt_l2_fdb_notify_type_t type);
extern yt_ret_t  fal_tiger_l2_fdb_uc_get_first_from_db(yt_unit_t unit, l2_ucastMacAddr_info_t *pUcastMac);
extern yt_ret_t  fal_tiger_l2_fdb_uc_get_next_from_db(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac);
extern yt_ret_t fal_tiger_l2_mcast_addr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pMcastMac);
extern yt_ret_t fal_tiger_l2_mcast_addr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_mcast_mac_data_t *pMcastMac);
extern yt_ret_t  fal_tiger_l2_mcast_addr_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNext_index, yt_mcast_mac_data_t *pMcastMac);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
