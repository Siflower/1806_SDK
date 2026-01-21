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

#define YT_RMA_BYPASS_PORT_ISO_BIT                       (1U)
#define YT_RMA_BYPASS_VLAN_FILTER_BIT                (2U)
#define YT_RMA_BYPASS_UNKNOWN_MC_BIT             (4U)
#define YT_RMA_BYPASS_STP_BIT                                   (8U)
#define YT_RMA_BYPASS_STORM_BIT                            (16U)
#define YT_RMA_BYPASS_FLOW_METER_BIT               (32U)
#define YT_RMA_BYPASS_PORT_METER_BIT                (64U)
#define YT_RMA_BYPASS_ALL                                            (127U)

typedef enum yt_rma_da_e
{
    YT_RMA_DA_BRG_GROUP = 0,
    YT_RMA_DA_FD_PAUSE_01,
    YT_RMA_DA_SLOW_PROT_02,
    YT_RMA_DA_1X_PAE_03,
    YT_RMA_DA_UNDEF_04,
    YT_RMA_DA_UNDEF_05,
    YT_RMA_DA_UNDEF_06,
    YT_RMA_DA_UNDEF_07,
    YT_RMA_DA_PROV_BRG_GROUP_08,
    YT_RMA_DA_UNDEF_09,
    YT_RMA_DA_UNDEF_0A,
    YT_RMA_DA_UNDEF_0B,
    YT_RMA_DA_UNDEF_0C,
    YT_RMA_DA_PROV_BRG_GVRP_0D,
    YT_RMA_DA_LLDP_0E,
    YT_RMA_DA_UNDEF_0F,
    YT_RMA_DA_MANAGEMENT_10,
    YT_RMA_DA_LSG_11,
    YT_RMA_DA_LDG_12,
    YT_RMA_DA_UNDEF_13,
    YT_RMA_DA_UNDEF_14,
    YT_RMA_DA_UNDEF_15,
    YT_RMA_DA_UNDEF_16,
    YT_RMA_DA_UNDEF_17,
    YT_RMA_DA_UNDEF_18,
    YT_RMA_DA_UNDEF_19,
    YT_RMA_DA_UNDEF_1A,
    YT_RMA_DA_UNDEF_1B,
    YT_RMA_DA_UNDEF_1C,
    YT_RMA_DA_UNDEF_1D,
    YT_RMA_DA_UNDEF_1E,
    YT_RMA_DA_UNDEF_1F,
    YT_RMA_DA_MRP_20,
    YT_RMA_DA_MRP_21,
    YT_RMA_DA_MRP_22,
    YT_RMA_DA_MRP_23,
    YT_RMA_DA_MRP_24,
    YT_RMA_DA_MRP_25,
    YT_RMA_DA_MRP_26,
    YT_RMA_DA_MRP_27,
    YT_RMA_DA_MRP_28,
    YT_RMA_DA_MRP_29,
    YT_RMA_DA_MRP_2A,
    YT_RMA_DA_MRP_2B,
    YT_RMA_DA_MRP_2C,
    YT_RMA_DA_MRP_2D,
    YT_RMA_DA_MRP_2E,
    YT_RMA_DA_MRP_2F,
    YT_RMA_DA_END
}yt_rma_da_t;

typedef enum yt_rma_cpu_code_e
{
    YT_RMA_CPU_CODE_RESV0 = 25,
    YT_RMA_CPU_CODE_RESV1,
    YT_RMA_CPU_CODE_RESV2,
    YT_RMA_CPU_CODE_RESV3,
    YT_RMA_CPU_CODE_RESV4,
    YT_RMA_CPU_CODE_RESV5,
    YT_RMA_CPU_CODE_RESV6,
    YT_RMA_CPU_CODE_RESV7,
    YT_RMA_CPU_CODE_RESV8,
    YT_RMA_CPU_CODE_RESV9
}yt_rma_cpu_code_t;

typedef struct yt_rma_bypass_s
{
    yt_enable_t bypass_port_isolation;
    yt_enable_t bypass_vlan_filter;
    yt_enable_t bypass_unknown_mc;
    yt_enable_t bypass_stp;
    yt_enable_t bypass_storm;
    yt_enable_t bypass_flow_meter;
    yt_enable_t bypass_port_meter;
    uint8_t setBitMask;
}yt_rma_bypass_t;

/**
 * @internal      yt_rma_action_set
 * @endinternal
 *
 * @brief         set action of specific reserved multicast address
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[in]     action              -rma packet action,refer to yt_act_type_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
*/

extern yt_ret_t  yt_rma_action_set(yt_unit_t unit, yt_rma_da_t  da, yt_act_type_t action);


/**
 * @internal      yt_rma_action_get
 * @endinternal
 *
 * @brief         get action of specific reserved multicast address type
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[out]    paction             -rma packet cpu code,refer to yt_rma_cpu_code_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_action_get(yt_unit_t unit, yt_rma_da_t  da, yt_act_type_t *pAction);


/**
 * @internal      yt_rma_cpu_code_set
 * @endinternal
 *
 * @brief         set cpu code of specific reserved multicast address
 * @note          APPLICABLE DEVICES  -Tiger,Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[in]     cpuCode              -rma packet action,refer to yt_act_type_t
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
*/

extern yt_ret_t  yt_rma_cpu_code_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_cpu_code_t cpuCode);


/**
 * @internal      yt_rma_cpu_code_get
 * @endinternal
 *
 * @brief         get cpu code of specific reserved multicast address type
 * @note          APPLICABLE DEVICES             -Tiger,Shark, Whale
 * @param[in]     unit                                              -unit id
 * @param[in]     da                                                 -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[out]    pCpuCode                                       -rma packet action,refer to yt_fwd_type_t
 * @retval        CMM_ERR_OK                            -on success
 * @retval        CMM_ERR_FAIL                         -on fail
 */
extern yt_ret_t  yt_rma_cpu_code_get(yt_unit_t unit, yt_rma_da_t  da, yt_rma_cpu_code_t *pCpuCode);

/**
 * @internal      yt_rma_bypass_set
 * @endinternal
 *
 * @brief         set specific bypass state of specific reserved multicast address
 * @note          APPLICABLE DEVICES         -Tiger,Shark, Whale
 * @param[in]     unit                                          -unit id
 * @param[in]     da                                             -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[in]     rmaBypass                              -enable or disable
 * @retval        CMM_ERR_OK                       -on success
 * @retval        CMM_ERR_FAIL                    -on fail
 */
extern yt_ret_t yt_rma_bypass_set(yt_unit_t unit, yt_rma_da_t  da, yt_rma_bypass_t rmaBypass);


/**
 * @internal      yt_rma_bypass_get
 * @endinternal
 *
 * @brief         get specific bypass state of specific reserved multicast address
 * @note          APPLICABLE DEVICES         -Tiger,Shark, Whale
 * @param[in]     unit                                          -unit id
 * @param[in]     da                                             -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[in]     pRmaBypass                          -enable or disable
 * @retval        CMM_ERR_OK                       -on success
 * @retval        CMM_ERR_FAIL                    -on fail
 */
extern yt_ret_t yt_rma_bypass_get(yt_unit_t unit, yt_rma_da_t  da,yt_rma_bypass_t *pRmaBypass);


/**
 * @internal      yt_rma_sa_learn_dis_set
 * @endinternal
 *
 * @brief         set sa learning for specific RMA da type
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[in]     enable                  -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_sa_learn_dis_set(yt_unit_t unit, yt_rma_da_t da, yt_enable_t enable);


/**
 * @internal      yt_rma_sa_learn_dis_get
 * @endinternal
 *
 * @brief         get the setting of rma sa learn
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     da               -the last byte of rma mac address,refer to yt_rma_da_t
 * @param[out]    pEnable                 -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_rma_sa_learn_dis_get(yt_unit_t unit, yt_rma_da_t da, yt_enable_t *pEnable);

#endif

