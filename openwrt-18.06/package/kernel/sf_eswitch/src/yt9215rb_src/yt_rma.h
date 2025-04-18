/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#ifndef __YT_RMA_H__
#define __YT_RMA_H__

/*
 * Include Files
 */
#include "yt_cmm.h"

typedef enum yt_rma_da_e
{
    RMA_DA_BRG_GROUP = 0,
    RMA_DA_FD_PAUSE_01,
    RMA_DA_SLOW_PROT_02,
    RMA_DA_1X_PAE_03,
    RMA_DA_UNDEF_04,
    RMA_DA_UNDEF_05,
    RMA_DA_UNDEF_06,
    RMA_DA_UNDEF_07,
    RMA_DA_PROV_BRG_GROUP_08,
    RMA_DA_UNDEF_09,
    RMA_DA_UNDEF_0A,
    RMA_DA_UNDEF_0B,
    RMA_DA_UNDEF_0C,
    RMA_DA_PROV_BRG_GVRP_0D,
    RMA_DA_LLDP_0E,
    RMA_DA_UNDEF_0F,
    RMA_DA_MANAGEMENT_10,
    RMA_DA_LSG_11,
    RMA_DA_LDG_12,
    RMA_DA_UNDEF_13,
    RMA_DA_UNDEF_14,
    RMA_DA_UNDEF_15,
    RMA_DA_UNDEF_16,
    RMA_DA_UNDEF_17,
    RMA_DA_UNDEF_18,
    RMA_DA_UNDEF_19,
    RMA_DA_UNDEF_1A,
    RMA_DA_UNDEF_1B,
    RMA_DA_UNDEF_1C,
    RMA_DA_UNDEF_1D,
    RMA_DA_UNDEF_1E,
    RMA_DA_UNDEF_1F,
    RMA_DA_MRP_20,
    RMA_DA_MRP_21,
    RMA_DA_MRP_22,
    RMA_DA_MRP_23,
    RMA_DA_MRP_24,
    RMA_DA_MRP_25,
    RMA_DA_MRP_26,
    RMA_DA_MRP_27,
    RMA_DA_MRP_28,
    RMA_DA_MRP_29,
    RMA_DA_MRP_2A,
    RMA_DA_MRP_2B,
    RMA_DA_MRP_2C,
    RMA_DA_MRP_2D,
    RMA_DA_MRP_2E,
    RMA_DA_MRP_2F,
    RMA_DA_END
}yt_rma_da_t;

typedef enum yt_rma_action_s {
    RMA_ACTION_FWD,
    RMA_ACTION_TRAP,
    RMA_ACTION_COPY,
    RMA_ACTION_DROP,
    RMA_ACTION_END
} yt_rma_action_t;

/**
 * @internal      yt_rma_action_set
 * @endinternal
 *
 * @brief         set action of specific reserved multicast address
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     action              -rma packet action,refer to yt_rma_action_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
 */
extern yt_ret_t  yt_rma_action_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_action_t action);


/**
 * @internal      yt_rma_action_get
 * @endinternal
 *
 * @brief         get action of specific reserved multicast address
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    paction             -rma packet action,refer to yt_rma_action_
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_action_get(yt_unit_t unit, yt_rma_da_t  da, yt_rma_action_t *paction);


/**
 * @internal      yt_rma_bypass_port_isolation_set
 * @endinternal
 *
 * @brief         set rma passthrough port isolation or not
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     enable                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rma_bypass_port_isolation_set(yt_unit_t unit, yt_rma_da_t  da, yt_enable_t enable);


/**
 * @internal      yt_rma_bypass_port_isolation_get
 * @endinternal
 *
 * @brief         get the setting of rma passthrough port isolation
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    pEnable                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_rma_bypass_port_isolation_get(yt_unit_t unit, yt_rma_da_t  da, yt_enable_t *pEnable);


/**
 * @internal      yt_rma_bypass_vlan_filter_set
 * @endinternal
 *
 * @brief         set rma passthrough vlan igress filter or not
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[in]     enable                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_bypass_vlan_filter_set(yt_unit_t unit, yt_rma_da_t da, yt_enable_t enable);


/**
 * @internal      yt_rma_bypass_vlan_filter_get
 * @endinternal
 *
 * @brief         get the setting of rma passthrough vlan igress filter
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_action_t
 * @param[out]    pEnable                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_bypass_vlan_filter_get(yt_unit_t unit, yt_rma_da_t da, yt_enable_t *pEnable);

#endif
