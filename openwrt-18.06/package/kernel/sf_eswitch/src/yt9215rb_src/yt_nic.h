/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_nic.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_NIC_H
#define __YT_NIC_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

typedef enum yt_cpuport_mode_e {
    CPUPORT_MODE_INTERNAL,
    CPUPORT_MODE_EXTERNAL,
}yt_cpuport_mode_t;

#if defined(SWITCH_SERIES_TIGER)
typedef enum yt_cpu_copy_reason_e
{
    CPU_CODE_LLDP_EEE = 11,
    CPU_CODE_LLDP,
    CPU_CODE_EAPOL,
    CPU_CODE_ARP,
    CPU_CODE_ND,
    CPU_CODE_BPDU,
    CPU_CODE_ERP = 17,
    CPU_CODE_OAM = 20,
    CPU_CODE_LOOP_PDU,
    CPU_CODE_PENDING_FDB,
    CPU_CODE_ACL,
    CPU_CODE_IGMP,
    CPU_CODE_UNKNOWN_UCAST,
    CPU_CODE_UNKNOWN_MCAST,
    CPU_CODE_LEARN_MODE_AUTOANDCOPY,
    CPU_CODE_DA_TRAP,
    CPU_CODE_RMA = 0x3f
}yt_cpu_copy_reason_t;
#endif

typedef enum yt_cputag_mode_e {
    CPUTAG_MODE_8B,
    CPUTAG_MODE_4B_PORT,    /* mainly include src port and dst port */
    CPUTAG_MODE_4B_VID,     /* mainly include forward vlan id */
}yt_cputag_mode_t;

/**
 * @internal      yt_nic_init
 * @endinternal
 *
 * @brief         init nic module
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_init(yt_unit_t unit);



/**
 * @internal      yt_nic_cpuport_mode_set
 * @endinternal
 *
 * @brief         Select internal or external cpu port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
 */
extern yt_ret_t  yt_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode);


/**
 * @internal      yt_nic_cpuport_mode_get
 * @endinternal
 *
 * @brief         Get cpu port mode setting
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 * @retval        CMM_ERR_INPUT        -input parameter error
 */
extern yt_ret_t  yt_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pMode);


/**
 * @internal      yt_nic_ext_cpuport_en_set
 * @endinternal
 *
 * @brief         enable or disable extend cpu port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      yt_nic_ext_cpuport_en_get
 * @endinternal
 *
 * @brief         get the state of extend cpu port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable);


/**
 * @internal      yt_nic_ext_cpuport_port_set
 * @endinternal
 *
 * @brief         set the extend cpu port num
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port);


/**
 * @internal      yt_nic_ext_cpuport_port_get
 * @endinternal
 *
 * @brief         get the extend cpu port num
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pPort               -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pPort);


/**
 * @internal      yt_nic_cpuport_tagtpid_set
 * @endinternal
 *
 * @brief         set vlan tag tpid for cpu port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -tag tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid);


/**
 * @internal      yt_nic_cpuport_tagtpid_get
 * @endinternal
 *
 * @brief         get vlan tag tpid for cpu port
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pTpid               -tag tpid value
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *pTpid);

/**
 * @internal      yt_nic_ext_cputag_en_set
 * @endinternal
 *
 * @brief         enable or disable extend cpu tag
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);

/**
 * @internal      yt_nic_ext_cputag_en_get
 * @endinternal
 *
 * @brief         get the state of extend cpu tag
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);

/**
 * @internal      yt_nic_int_cputag_en_set
 * @endinternal
 *
 * @brief         enable or disable internal cpu tag
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_int_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);

/**
 * @internal      yt_nic_int_cputag_en_get
 * @endinternal
 *
 * @brief         get the state of internal cpu tag
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_int_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);

/**
 * @internal      yt_nic_cpu_pkt_keepAll_en_set
 * @endinternal
 *
 * @brief         enable/disable external/internal cpu port tx packets keep all(do not modify pkt content exclude cpu tag)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     extEn               -external cpu port pkt keep all enable/disable
 * @param[in]     intEn               -internal cpu port pkt keep all enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern  yt_ret_t  yt_nic_cpu_pkt_keepAll_en_set(yt_unit_t unit, yt_enable_t extEn, yt_enable_t intEn);

/**
 * @internal      yt_nic_cpu_pkt_keepAll_en_get
 * @endinternal
 *
 * @brief         get state of external/internal cpu port tx packets keep all(do not modify pkt content exclude cpu tag)
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]     pExtEN             -external cpu port pkt keep all enable/disable
 * @param[out]     pIntEN             -internal cpu port pkt keep all enable/disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern  yt_ret_t  yt_nic_cpu_pkt_keepAll_en_get(yt_unit_t unit, yt_enable_t *pExtEN, yt_enable_t *pIntEN);

/**
 * @internal      yt_nic_cputag_mode_set
 * @endinternal
 *
 * @brief         Set cpu tag mode
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cputag_mode_set(yt_unit_t unit, yt_cputag_mode_t mode);

/**
 * @internal      yt_nic_cputag_mode_get
 * @endinternal
 *
 * @brief         Get cpu tag mode
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_nic_cputag_mode_get(yt_unit_t unit, yt_cputag_mode_t *pMode);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
