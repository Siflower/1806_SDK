
#include "fal_tiger_interrupt.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#ifdef PORT_INCLUDED
#include "fal_tiger_port.h"
#endif

yt_ret_t fal_tiger_int_polarity_set(yt_unit_t unit, yt_int_polarity_t type)
{
    global_ctrl1_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_INTERRUP_POLARITYf, &entry, type);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);

    return CMM_ERR_OK;
}


yt_ret_t fal_tiger_int_polarity_get(yt_unit_t unit, yt_int_polarity_t *pType)
{
    global_ctrl1_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t polarity;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &entry), ret);
    HAL_FIELD_GET(GLOBAL_CTRL1m, GLOBAL_CTRL1_INTERRUP_POLARITYf, &entry, &polarity);
    *pType = polarity ? INT_POLAR_HIGH : INT_POLAR_LOW;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_control_set(yt_unit_t unit, yt_int_type_t type, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    intr_mask_t intr_mask;
    uint32_t mask;
    uint32_t fieldId;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);
    mask = (YT_ENABLE == enable)?1:0;
    fieldId = INTR_MASK_HW_EXCEPTIONf-type;
    HAL_FIELD_SET(INTR_MASKm, fieldId, &intr_mask, mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_control_get(yt_unit_t unit, yt_int_type_t type, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t intr_mask;
    uint32_t mask;
    uint32_t fieldId;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);
    fieldId = INTR_MASK_HW_EXCEPTIONf-type;
    HAL_FIELD_GET(INTR_MASKm, fieldId, &intr_mask, &mask);
    *pEnable = mask ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_status_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    cmm_err_t ret = CMM_ERR_OK;
    intr_status_t intr_status;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INTR_STATUSm, 0, sizeof(yt_intr_status_t), &intr_status), ret);
    *pIntStatus = (intr_status.entry_data[0] & 0x1FFFFFFF);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_typeStatus_get(yt_unit_t unit, yt_int_type_t type, yt_intr_status_t intStatus, yt_bool_t *pActive)
{
    intr_status_t intr_status;
    uint32_t mask;
    uint32_t fieldId;

    CMM_UNUSED_PARAM(unit);
    intr_status.entry_data[0] = intStatus;
    fieldId = INTR_MASK_HW_EXCEPTIONf-type;
    HAL_FIELD_GET(INTR_MASKm, fieldId, &intr_status, &mask);
    *pActive = (mask==1) ? TRUE : FALSE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_port_int_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    intr_mask_t intr_mask;
    uint32_t mask;
    yt_macid_t macId;
    uint32_t fieldId;

    macId = CAL_YTP_TO_MAC(unit,port);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);
    mask = (YT_ENABLE == enable)?1:0;
    fieldId = INTR_MASK_PHY_INT0f - macId;
    HAL_FIELD_SET(INTR_MASKm, fieldId, &intr_mask, mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_port_int_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    intr_mask_t intr_mask;
    uint32_t mask;
    yt_macid_t macId;
    uint32_t fieldId;

    macId = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INTR_MASKm, 0, sizeof(intr_mask_t), &intr_mask), ret);
    fieldId = INTR_MASK_PHY_INT0f - macId;
    HAL_FIELD_GET(INTR_MASKm, fieldId, &intr_mask, &mask);
    *pEnable = mask ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pIntStatus);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_int_portIntStatus_get(yt_unit_t unit, yt_port_t port, yt_intr_status_t intStatus, yt_bool_t *pActive)
{
    intr_status_t intr_status;
    yt_macid_t macId;
    uint32_t mask;
    uint32_t fieldId;
#ifdef PORT_INCLUDED
    uint16_t tmpData;
#endif

    macId = CAL_YTP_TO_MAC(unit,port);

    intr_status.entry_data[0] = intStatus;
    fieldId = INTR_MASK_PHY_INT0f - macId;
    HAL_FIELD_GET(INTR_MASKm, fieldId, &intr_status, &mask);
    *pActive = mask ? YT_ENABLE : YT_DISABLE;

#ifdef PORT_INCLUDED
    /* TODO: clean phy status asynchronously */
    /* read and clean phy interrupt status */
    fal_tiger_port_phy_interruptStatus_get(unit, port, &tmpData);
#endif

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_int_wol_signalOutputType_set(yt_unit_t unit, yt_int_wol_outputType_t outputType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(outputType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_int_wol_signalOutputType_get(yt_unit_t unit, yt_int_wol_outputType_t *pOutputType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pOutputType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_int_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}


yt_ret_t fal_tiger_int_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_int_wol_allPortIntStatus_get(yt_unit_t unit, yt_intr_status_t *pIntStatus)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pIntStatus);

    return CMM_ERR_NOT_SUPPORT;
}

