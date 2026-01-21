/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_nic.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __FAL_TIGER_NIC_H__
#define __FAL_TIGER_NIC_H__


#include "fal_cmm.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @internal      fal_tiger_nic_init
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_init(yt_unit_t unit);


/**
 * @internal      fal_tiger_nic_cpuport_mode_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     mode                -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_mode_set(yt_unit_t unit, yt_cpuport_mode_t mode);


/**
 * @internal      fal_tiger_nic_cpuport_mode_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pmode               -internal or external cpu port
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_mode_get(yt_unit_t unit, yt_cpuport_mode_t *pmode);


/**
 * @internal      fal_tiger_nic_ext_cpuport_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      fal_tiger_nic_ext_cpuport_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_en_get(yt_unit_t unit,  yt_enable_t *pEnable);


/**
 * @internal      fal_tiger_nic_ext_cpuport_port_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_port_set(yt_unit_t unit,  yt_port_t port);


/**
 * @internal      fal_tiger_nic_ext_cpuport_port_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pport               -port num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cpuport_port_get(yt_unit_t unit,  yt_port_t *pport);


/**
 * @internal      fal_tiger_nic_cpuport_tagtpid_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     tpid                -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_tagtpid_set(yt_unit_t unit, uint16_t tpid);


/**
 * @internal      fal_tiger_nic_cpuport_tagtpid_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    ptpid               -x
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_cpuport_tagtpid_get(yt_unit_t unit, uint16_t *ptpid);


/**
 * @internal      fal_tiger_nic_ext_cputag_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_ext_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);


/**
 * @internal      fal_tiger_nic_ext_cputag_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_nic_ext_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);

/**
 * @internal      fal_tiger_nic_int_cputag_en_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t fal_tiger_nic_int_cputag_en_set(yt_unit_t unit,  yt_enable_t enable);

/**
 * @internal      fal_tiger_nic_int_cputag_en_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  fal_tiger_nic_int_cputag_en_get(yt_unit_t unit,  yt_enable_t *pEnable);


extern yt_ret_t  fal_tiger_nic_cpu_pkt_keepAll_en_set(yt_unit_t unit, yt_enable_t extEn, yt_enable_t intEn);
extern yt_ret_t  fal_tiger_nic_cpu_pkt_keepAll_en_get(yt_unit_t unit, yt_enable_t *pExtEN, yt_enable_t *pIntEN);


extern yt_ret_t  fal_tiger_nic_cputag_mode_set(yt_unit_t unit, yt_cputag_mode_t mode);
extern yt_ret_t  fal_tiger_nic_cputag_mode_get(yt_unit_t unit, yt_cputag_mode_t *pMode);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
