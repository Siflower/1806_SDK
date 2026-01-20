/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_interrupt.h
*
* @brief x
*
********************************************************************************
*/
#ifndef __YT_INTERRUPT_H__
#define __YT_INTERRUPT_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "yt_cmm.h"

typedef enum yt_int_polarity_e
{
    INT_POLAR_LOW = 0,
    INT_POLAR_HIGH,
    INT_POLAR_END
} yt_int_polarity_t;

#ifdef SWITCH_SERIES_TIGER
typedef enum yt_int_type_e
{
    INT_TYPE_HW_EXCEPTION = 0,
    INT_TYPE_EEPROM_LOAD_DONE = 11,
    INT_TYPE_LOOP_DETECT,
    INT_TYPE_ACL_ACT = 14,
    INT_TYPE_FDB_LEARN_FULL = 16,
    INT_TYPE_FDB_DELETE,
    INT_TYPE_FDB_LEARN_MOVE,
    INT_TYPE_FDB_LEARN_CREATE,
    INT_TYPE_IGMP_DELETE,
    INT_TYPE_IGMP_LEARN_FULL,
    INT_TYPE_HW_INI_DONE,
    INT_TYPE_SWITCH_WOL_INT = 24,
    INT_TYPE_OVER_TEMP = 28,
    INT_TYPE_END
}yt_int_type_t;
#endif

#ifdef SWITCH_SERIES_SHARK
typedef enum yt_int_type_e
{
    INT_TYPE_MAC_PHY_SYNC = 0,
    INT_TYPE_HW_EXCEPTION,
    INT_TYPE_EEPROM_LOAD_DONE,
    INT_TYPE_LOOP_DETECT,
    INT_TYPE_FDB_LEARN_FULL = 6,
    INT_TYPE_FDB_DELETE,
    INT_TYPE_FDB_LEARN_MOVE,
    INT_TYPE_FDB_LEARN_CREATE,
    INT_TYPE_HW_INI_DONE,
    INT_TYPE_SWITCH_WOL_INT = 12,
    INT_TYPE_OVER_TEMP = 17,
    INT_TYPE_ACL_HIT0,
    INT_TYPE_ACL_HIT1,
    INT_TYPE_ACL_HIT2,
    INT_TYPE_ACL_HIT3,
    INT_TYPE_FDB_OVER_LIMIT,
    INT_TYPE_END
}yt_int_type_t;
#endif

#ifdef SWITCH_SERIES_WHALE
typedef enum yt_int_type_e
{
    INT_TYPE_MAC_PHY_SYNC = 0,
    INT_TYPE_HW_EXCEPTION,
    INT_TYPE_EEPROM_LOAD_DONE,
    INT_TYPE_LOOP_DETECT,
    INT_TYPE_FDB_LEARN_FULL = 6,
    INT_TYPE_FDB_DELETE,
    INT_TYPE_FDB_LEARN_MOVE,
    INT_TYPE_FDB_LEARN_CREATE,
    INT_TYPE_HW_INI_DONE,
    INT_TYPE_SWITCH_WOL_INT = 12,
    INT_TYPE_OVER_TEMP = 17,
    INT_TYPE_FDB_OVER_LIMIT = 22,
    INT_TYPE_DYING_GASP,
    INT_TYPE_MC_ROUTER_PORT_FULL,
    INT_TYPE_IGMP_MLD_LEARN_FULL = 25,
    INT_TYPE_END
}yt_int_type_t;
#endif

typedef enum yt_int_wol_outputType_e
{
    YT_INT_OUTPUT_TYPE_HIGH_LEVEL,
    YT_INT_OUTPUT_TYPE_LOW_LEVEL,
    YT_INT_OUTPUT_TYPE_HIGH_PULSE,
    YT_INT_OUTPUT_TYPE_LOW_PULSE,
    YT_INT_OUTPUT_TYPE_MAX
} yt_int_wol_outputType_t;

/**
 * @internal      yt_int_polarity_set
 * @endinternal
 *
 * @brief         Set interrupt polarity type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -high level low level
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_polarity_set(yt_unit_t unit, yt_int_polarity_t type);


/**
 * @internal      yt_int_polarity_get
 * @endinternal
 *
 * @brief         Get interrupt polarity type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pType               -high level low level
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_polarity_get(yt_unit_t unit, yt_int_polarity_t *pType);


/**
 * @internal      yt_int_control_set
 * @endinternal
 *
 * @brief         Set interrupt mask depend on interrupt type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -interrupt type
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_control_set(yt_unit_t unit, yt_int_type_t type, yt_enable_t enable);


/**
 * @internal      yt_int_control_get
 * @endinternal
 *
 * @brief         Get interrupt mask state depend on interrupt type
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -interrupt type
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_control_get(yt_unit_t unit, yt_int_type_t type, yt_enable_t *pEnable);


/**
 * @internal      yt_int_status_get
 * @endinternal
 *
 * @brief         Get all interrupt state.Note:all interrupt state including ports for Tiger.all interrupt state excluding ports for Shark.
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pIntStatus          -interrupt status bitmask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_status_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);


/**
 * @internal      yt_int_typeStatus_get
 * @endinternal
 *
 * @brief         Get specific type of interrupt state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     type                -interrupt type
 * @param[in]     intStatus           -all interrupt state
 * @param[out]    pActive             -spcific interrupt state active or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_typeStatus_get(yt_unit_t unit, yt_int_type_t type, yt_intr_status_t intStatus, yt_bool_t *pActive);


/**
 * @internal      yt_int_port_int_set
 * @endinternal
 *
 * @brief         Set port interrupt mask
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_port_int_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);


/**
 * @internal      yt_int_port_int_get
 * @endinternal
 *
 * @brief         Get port interrupt mask state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_port_int_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);


/**
 * @internal      yt_int_allPortIntStatus_get
 * @endinternal
 *
 * @brief         Get all ports interrupt state
 * @note          APPLICABLE DEVICES  -Shark, Whale
 * @param[in]     unit                -unit id
 * @param[out]    pIntStatus          -interrupt state
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);


/**
 * @internal      yt_int_portIntStatus_get
 * @endinternal
 *
 * @brief         Get specific port interrupt state from all intr state
 * @note          APPLICABLE DEVICES  -Tiger, Shark, Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     intStatus           -all port interrupt state
 * @param[out]    pActive             -specific port interrupt state active or not
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_portIntStatus_get(yt_unit_t unit, yt_port_t port, yt_intr_status_t intStatus, yt_bool_t *pActive);

/**
 * @internal      yt_int_wol_signalOutputType_set
 * @endinternal
 *
 * @brief         Set WOL pin signal output type
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     outputType          -signal output type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_wol_signalOutputType_set(yt_unit_t unit, yt_int_wol_outputType_t outputType);

/**
 * @internal      yt_int_wol_signalOutputType_get
 * @endinternal
 *
 * @brief         Get WOL pin signal output type
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pOutputType         -signal output type
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_wol_signalOutputType_get(yt_unit_t unit, yt_int_wol_outputType_t *pOutputType);

/**
 * @internal      yt_int_wol_port_enable_set
 * @endinternal
 *
 * @brief         Set port WOL interrupt mask
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable);

/**
 * @internal      yt_int_wol_port_enable_get
 * @endinternal
 *
 * @brief         Get port WOL interrupt mask state
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable);

/**
 * @internal      yt_int_wol_allPortIntStatus_get
 * @endinternal
 *
 * @brief         Get all ports WOL interrupt state
 * @note          APPLICABLE DEVICES  -Whale
 * @param[in]     unit                -unit id
 * @param[out]    pIntStatus          -interrupt state
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t yt_int_wol_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
