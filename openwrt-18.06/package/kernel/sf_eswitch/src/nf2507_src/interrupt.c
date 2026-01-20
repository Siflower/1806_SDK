/**  @file
  *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#include "l2sw_switch.h"
#include "interrupt.h"
#include "sal_string.h"
#include "l2sw_asicdrv_interrupt.h"

static l2sw_api_ret_t _l2sw_int_polarity_set(l2sw_uint8 units, l2sw_int_polarity_t type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(type >= INT_POLAR_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicInterruptPolarity(units, type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_polarity_get(l2sw_uint8 units, l2sw_int_polarity_t *pType)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pType)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicInterruptPolarity(units, pType)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_control_set(l2sw_uint8 units, l2sw_int_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 mask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= INT_TYPE_END)
        return L2SW_ERR_INPUT;

    if (type == INT_TYPE_RESERVED)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_getAsicInterruptMask(units, &mask)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_ENABLED == enable)
        mask = mask | (1<<type);
    else if (L2SW_DISABLED == enable)
        mask = mask & ~(1<<type);
    else
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicInterruptMask(units, mask)) != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_control_get(l2sw_uint8 units, l2sw_int_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 mask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicInterruptMask(units, &mask)) != L2SW_ERR_OK)
        return retVal;

    if (0 == (mask&(1<<type)))
        *pEnable=L2SW_DISABLED;
    else
        *pEnable=L2SW_ENABLED;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_status_set(l2sw_uint8 units, l2sw_int_status_t *pStatusMask)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pStatusMask)
        return L2SW_ERR_NULL_POINTER;

    if(pStatusMask->value[0] & (0x0001 << INT_TYPE_RESERVED))
        return L2SW_ERR_INPUT;

    if(pStatusMask->value[0] >= (0x0001 << INT_TYPE_END))
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicInterruptStatus(units, (l2sw_uint32)pStatusMask->value[0]))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_status_get(l2sw_uint8 units, l2sw_int_status_t* pStatusMask)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32          ims_mask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pStatusMask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicInterruptStatus(units, &ims_mask)) != L2SW_ERR_OK)
        return retVal;

    pStatusMask->value[0] = (ims_mask & 0x00000FFF);
    return L2SW_ERR_OK;
}

#define ADV_NOT_SUPPORT (0xFFFF)
static l2sw_api_ret_t _l2sw_int_Advidx_get(l2sw_uint8 units, l2sw_int_advType_t adv_type, l2sw_uint32 *pAsic_idx)
{
    l2sw_uint32 asic_idx[ADV_END] =
    {
        INTRST_L2_LEARN,
        INTRST_SPEED_CHANGE,
        INTRST_SPECIAL_CONGESTION,
        INTRST_PORT_LINKDOWN,
        INTRST_PORT_LINKUP,
        ADV_NOT_SUPPORT,
        INTRST_RLDP_LOOPED,
        INTRST_RLDP_RELEASED,
    };

    if(adv_type >= ADV_END)
        return L2SW_ERR_INPUT;

    if(asic_idx[adv_type] == ADV_NOT_SUPPORT)
        return L2SW_ERR_CHIP_NOT_SUPPORTED;

    *pAsic_idx = asic_idx[adv_type];
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_int_advanceInfo_get(l2sw_uint8 units, l2sw_int_advType_t adv_type, l2sw_int_info_t *pInfo)
{
    l2sw_api_ret_t   retVal = 0;
    l2sw_uint32      data = 0;
    l2sw_uint32      intAdvType = 0;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(adv_type >= ADV_END)
        return L2SW_ERR_INPUT;

    if(NULL == pInfo)
        return L2SW_ERR_NULL_POINTER;

    if(adv_type != ADV_METER_EXCEED_MASK)
    {
        if((retVal = _l2sw_int_Advidx_get(units, adv_type, &intAdvType)) != L2SW_ERR_OK)
            return retVal;
    }

    switch(adv_type)
    {
        case ADV_L2_LEARN_PORT_MASK:
            /* Get physical portmask */
            if((retVal = l2sw_getAsicInterruptRelatedStatus(units, intAdvType, &data)) != L2SW_ERR_OK)
                return retVal;

            /* Clear Advanced Info */
            if((retVal = l2sw_setAsicInterruptRelatedStatus(units, intAdvType, 0xFFFF)) != L2SW_ERR_OK)
                return retVal;

            /* Translate to logical portmask */
            if((retVal = l2sw_switch_portmask_P2L_get(units, data, &(pInfo->portMask))) != L2SW_ERR_OK)
                return retVal;

            /* Get system learn */
            if((retVal = l2sw_getAsicInterruptRelatedStatus(units, INTRST_SYS_LEARN, &data)) != L2SW_ERR_OK)
                return retVal;

            /* Clear system learn */
            if((retVal = l2sw_setAsicInterruptRelatedStatus(units, INTRST_SYS_LEARN, 0x0001)) != L2SW_ERR_OK)
                return retVal;

            pInfo->systemLearnOver = data;
            break;
        case ADV_SPEED_CHANGE_PORT_MASK:
        case ADV_SPECIAL_CONGESTION_PORT_MASK:
        case ADV_PORT_LINKDOWN_PORT_MASK:
        case ADV_PORT_LINKUP_PORT_MASK:
        case ADV_RLDP_LOOPED:
        case ADV_RLDP_RELEASED:
            /* Get physical portmask */
            if((retVal = l2sw_getAsicInterruptRelatedStatus(units, intAdvType, &data)) != L2SW_ERR_OK)
                return retVal;

            /* Clear Advanced Info */
            if((retVal = l2sw_setAsicInterruptRelatedStatus(units, intAdvType, 0xFFFF)) != L2SW_ERR_OK)
                return retVal;

            /* Translate to logical portmask */
            if((retVal = l2sw_switch_portmask_P2L_get(units, data, &(pInfo->portMask))) != L2SW_ERR_OK)
                return retVal;

            break;
        case ADV_METER_EXCEED_MASK:
            /* Get Meter Mask */
            if((retVal = l2sw_getAsicInterruptRelatedStatus(units, INTRST_METER0_15, &data)) != L2SW_ERR_OK)
                return retVal;

            /* Clear Advanced Info */
            if((retVal = l2sw_setAsicInterruptRelatedStatus(units, INTRST_METER0_15, 0xFFFF)) != L2SW_ERR_OK)
                return retVal;

            pInfo->meterMask = data & 0xFFFF;

            /* Get Meter Mask */
            if((retVal = l2sw_getAsicInterruptRelatedStatus(units, INTRST_METER16_31, &data)) != L2SW_ERR_OK)
                return retVal;

            /* Clear Advanced Info */
            if((retVal = l2sw_setAsicInterruptRelatedStatus(units, INTRST_METER16_31, 0xFFFF)) != L2SW_ERR_OK)
                return retVal;

            pInfo->meterMask = pInfo->meterMask | ((data << 16) & 0xFFFF0000);

            break;
        default:
            return L2SW_ERR_INPUT;
    }

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_int_polarity_set
 * Description:
 *      Set interrupt polarity configuration.
 * Input:
 *      type - Interruptpolarity type.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can set interrupt polarity configuration.
 */
l2sw_api_ret_t l2sw_int_polarity_set(l2sw_uint8 units, l2sw_int_polarity_t type)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_polarity_set(units, type);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_polarity_get
 * Description:
 *      Get interrupt polarity configuration.
 * Input:
 *      None
 * Output:
 *      pType - Interruptpolarity type.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      The API can get interrupt polarity configuration.
 */
l2sw_api_ret_t l2sw_int_polarity_get(l2sw_uint8 units, l2sw_int_polarity_t *pType)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_polarity_get(units, pType);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_control_set
 * Description:
 *      Set interrupt trigger status configuration.
 * Input:
 *      type - Interrupt type.
 *      enable - Interrupt status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      The API can set interrupt status configuration.
 *      The interrupt trigger status is shown in the following:
 *      - INT_TYPE_LINK_STATUS
 *      - INT_TYPE_METER_EXCEED
 *      - INT_TYPE_LEARN_LIMIT
 *      - INT_TYPE_LINK_SPEED
 *      - INT_TYPE_CONGEST
 *      - INT_TYPE_GREEN_FEATURE
 *      - INT_TYPE_LOOP_DETECT
 *      - INT_TYPE_8051,
 *      - INT_TYPE_CABLE_DIAG,
 *      - INT_TYPE_ACL,
 *      - INT_TYPE_SLIENT
 */
l2sw_api_ret_t l2sw_int_control_set(l2sw_uint8 units, l2sw_int_type_t type, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_control_set(units, type, enable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_control_get
 * Description:
 *      Get interrupt trigger status configuration.
 * Input:
 *      type - Interrupt type.
 * Output:
 *      pEnable - Interrupt status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get interrupt status configuration.
 *      The interrupt trigger status is shown in the following:
 *      - INT_TYPE_LINK_STATUS
 *      - INT_TYPE_METER_EXCEED
 *      - INT_TYPE_LEARN_LIMIT
 *      - INT_TYPE_LINK_SPEED
 *      - INT_TYPE_CONGEST
 *      - INT_TYPE_GREEN_FEATURE
 *      - INT_TYPE_LOOP_DETECT
 *      - INT_TYPE_8051,
 *      - INT_TYPE_CABLE_DIAG,
 *      - INT_TYPE_ACL,
 *      - INT_TYPE_UPS,
 *      - INT_TYPE_SLIENT
 */
l2sw_api_ret_t l2sw_int_control_get(l2sw_uint8 units, l2sw_int_type_t type, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_control_get(units, type, pEnable);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_status_set
 * Description:
 *      Set interrupt trigger status to clean.
 * Input:
 *      None
 * Output:
 *      pStatusMask - Interrupt status bit mask.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT - Invalid input parameters.
 * Note:
 *      The API can clean interrupt trigger status when interrupt happened.
 *      The interrupt trigger status is shown in the following:
 *      - INT_TYPE_LINK_STATUS    (value[0] (Bit0))
 *      - INT_TYPE_METER_EXCEED   (value[0] (Bit1))
 *      - INT_TYPE_LEARN_LIMIT    (value[0] (Bit2))
 *      - INT_TYPE_LINK_SPEED     (value[0] (Bit3))
 *      - INT_TYPE_CONGEST        (value[0] (Bit4))
 *      - INT_TYPE_GREEN_FEATURE  (value[0] (Bit5))
 *      - INT_TYPE_LOOP_DETECT    (value[0] (Bit6))
 *      - INT_TYPE_8051           (value[0] (Bit7))
 *      - INT_TYPE_CABLE_DIAG     (value[0] (Bit8))
 *      - INT_TYPE_ACL            (value[0] (Bit9))
 *      - INT_TYPE_SLIENT         (value[0] (Bit11))
 *      The status will be cleared after execute this API.
 */
l2sw_api_ret_t l2sw_int_status_set(l2sw_uint8 units, l2sw_int_status_t *pStatusMask)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_status_set(units, pStatusMask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_status_get
 * Description:
 *      Get interrupt trigger status.
 * Input:
 *      None
 * Output:
 *      pStatusMask - Interrupt status bit mask.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get interrupt trigger status when interrupt happened.
 *      The interrupt trigger status is shown in the following:
 *      - INT_TYPE_LINK_STATUS    (value[0] (Bit0))
 *      - INT_TYPE_METER_EXCEED   (value[0] (Bit1))
 *      - INT_TYPE_LEARN_LIMIT    (value[0] (Bit2))
 *      - INT_TYPE_LINK_SPEED     (value[0] (Bit3))
 *      - INT_TYPE_CONGEST        (value[0] (Bit4))
 *      - INT_TYPE_GREEN_FEATURE  (value[0] (Bit5))
 *      - INT_TYPE_LOOP_DETECT    (value[0] (Bit6))
 *      - INT_TYPE_8051           (value[0] (Bit7))
 *      - INT_TYPE_CABLE_DIAG     (value[0] (Bit8))
 *      - INT_TYPE_ACL            (value[0] (Bit9))
 *      - INT_TYPE_SLIENT         (value[0] (Bit11))
 *
 */
l2sw_api_ret_t l2sw_int_status_get(l2sw_uint8 units, l2sw_int_status_t* pStatusMask)
{
    l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_status_get(units, pStatusMask);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_int_advanceInfo_get
 * Description:
 *      Get interrupt advanced information.
 * Input:
 *      adv_type - Advanced interrupt type.
 * Output:
 *      info - Information per type.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can get advanced information when interrupt happened.
 *      The status will be cleared after execute this API.
 */
l2sw_api_ret_t l2sw_int_advanceInfo_get(l2sw_uint8 units, l2sw_int_advType_t adv_type, l2sw_int_info_t *pInfo)
{
	l2sw_api_ret_t retVal;
    
    L2SW_API_LOCK(units);
    retVal = _l2sw_int_advanceInfo_get(units, adv_type, pInfo);    
    L2SW_API_UNLOCK(units);

    return retVal;
}

