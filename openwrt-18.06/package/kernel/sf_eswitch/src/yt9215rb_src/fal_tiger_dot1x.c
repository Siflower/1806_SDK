/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_dot1x.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

static dot1x_data_t dot1x_data[YT_MAX_UNIT];

#define DOT1X_PORT_AUTH_MASK(unit)              dot1x_data[unit].dot1x_portbase_auth_mask
#define DOT1X_PORT_DIR_MASK(unit)               dot1x_data[unit].dot1x_portbase_dir_mask
#define DOT1X_MAC_BASE_MASK(unit)               dot1x_data[unit].dot1x_macbase_state_mask

yt_ret_t fal_tiger_dot1x_init(yt_unit_t unit)
{
    yt_port_t port;
    yt_macid_t macid;

    /* lock init */
    DOT1X_LOCK_INIT();
    for (port = 0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); ++port) {
        macid = CAL_YTP_TO_MAC(unit,port);
        CLEAR_BIT(DOT1X_PORT_AUTH_MASK(unit), macid);
        CLEAR_BIT(DOT1X_PORT_DIR_MASK(unit), macid);
        CLEAR_BIT(DOT1X_MAC_BASE_MASK(unit), macid);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    l2_dot1x_ctrl1_t  entry;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t dot1x_en;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL1m, 0, sizeof(l2_dot1x_ctrl1_t), &entry), ret); 
    HAL_FIELD_GET(L2_DOT1X_CTRL1m, L2_DOT1X_CTRL1_DOT1X_ENf, &entry, &dot1x_en);
    if (enable == YT_ENABLE)
    {
        SET_BIT(dot1x_en, macid);
    }
    else
    {
        CLEAR_BIT(dot1x_en, macid);
    }
    HAL_FIELD_SET(L2_DOT1X_CTRL1m, L2_DOT1X_CTRL1_DOT1X_ENf, &entry, dot1x_en);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL1m, 0, sizeof(l2_dot1x_ctrl1_t), &entry), ret);

    if(enable == YT_ENABLE)
    {
        /* enable rx auth by default */
        fal_tiger_dot1x_portBasedDirection_set(unit, port, YT_DOT1X_AUTH_DIR_IN);
        fal_tiger_dot1x_portBasedAuthStatus_set(unit, port, YT_DOT1X_AUTH_STATUS_UNAUTH);
    }
    else if(enable == YT_DISABLE)
    {
        /* permit all by default */
        fal_tiger_dot1x_portBasedAuthStatus_set(unit, port, YT_DOT1X_AUTH_STATUS_AUTH);
        fal_tiger_dot1x_portBasedDirection_set(unit, port, YT_DOT1X_AUTH_DIR_IN);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    l2_dot1x_ctrl1_t  entry;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t dot1x_en;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL1m, 0, sizeof(l2_dot1x_ctrl1_t), &entry), ret);
    HAL_FIELD_GET(L2_DOT1X_CTRL1m, L2_DOT1X_CTRL1_DOT1X_ENf, &entry, &dot1x_en);
    if (IS_BIT_SET(dot1x_en, macid))
    {
        *pEnable = YT_ENABLE;
    }
    else
    {
        *pEnable = YT_DISABLE;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedAuthStatus_set(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t port_auth)
{
    l2_dot1x_ctrl2_t  entry;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t rx_port_mask;
    uint32_t tx_port_mask;
    yt_dot1x_direction_t authDir;

    fal_tiger_dot1x_portBasedDirection_get(unit, port, &authDir);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret);
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_RX_PERMIT_PORT_MASKf, &entry, &rx_port_mask);
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_TX_PERMIT_PORT_MASKf, &entry, &tx_port_mask);
    if (YT_DOT1X_AUTH_STATUS_AUTH == port_auth)
    {
        SET_BIT(rx_port_mask, macid);
        SET_BIT(tx_port_mask, macid);
    }
    else if(YT_DOT1X_AUTH_STATUS_UNAUTH == port_auth)
    {
        CLEAR_BIT(rx_port_mask, macid);
        if(authDir == YT_DOT1X_AUTH_DIR_BOTH)
        {
            CLEAR_BIT(tx_port_mask, macid);
        }
    }
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_RX_PERMIT_PORT_MASKf, &entry, rx_port_mask);
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_TX_PERMIT_PORT_MASKf, &entry, tx_port_mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret);

    DOT1X_LOCK();
    if (YT_DOT1X_AUTH_STATUS_AUTH == port_auth)
    {
        SET_BIT(DOT1X_PORT_AUTH_MASK(unit), macid);
    }
    else
    {
        CLEAR_BIT(DOT1X_PORT_AUTH_MASK(unit), macid);
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedAuthStatus_get(yt_unit_t unit, yt_port_t port, yt_dot1x_auth_status_t *pPort_auth)
{
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    DOT1X_LOCK();
    if (IS_BIT_SET(DOT1X_PORT_AUTH_MASK(unit), macid))
    {
        *pPort_auth = YT_DOT1X_AUTH_STATUS_AUTH;
    }
    else
    {
        *pPort_auth = YT_DOT1X_AUTH_STATUS_UNAUTH;
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedDirection_set(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t port_direction)
{
    l2_dot1x_ctrl2_t  entry;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t rx_port_mask;
    uint32_t tx_port_mask;
    yt_dot1x_auth_status_t authStat = YT_DOT1X_AUTH_STATUS_UNAUTH;

    DOT1X_LOCK();
    if (IS_BIT_SET(DOT1X_PORT_AUTH_MASK(unit), macid))
    {
        authStat = YT_DOT1X_AUTH_STATUS_AUTH;
    }
    DOT1X_UNLOCK();

    /* if already authed,no need clear auth status */
    if(authStat == YT_DOT1X_AUTH_STATUS_UNAUTH)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret);
        HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_RX_PERMIT_PORT_MASKf, &entry, &rx_port_mask);
        HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_TX_PERMIT_PORT_MASKf, &entry, &tx_port_mask);
        if (YT_DOT1X_AUTH_DIR_BOTH == port_direction)
        {
            CLEAR_BIT(rx_port_mask, macid);
            CLEAR_BIT(tx_port_mask, macid);
        }
        else
        {
            CLEAR_BIT(rx_port_mask, macid);
            SET_BIT(tx_port_mask, macid);
        }
        HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_RX_PERMIT_PORT_MASKf, &entry, rx_port_mask);
        HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_TX_PERMIT_PORT_MASKf, &entry, tx_port_mask);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret);
    }

    DOT1X_LOCK();
    if (YT_DOT1X_AUTH_DIR_IN == port_direction)
    {
        SET_BIT(DOT1X_PORT_DIR_MASK(unit), macid);
    }
    else
    {
        CLEAR_BIT(DOT1X_PORT_DIR_MASK(unit), macid);
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_portBasedDirection_get(yt_unit_t unit, yt_port_t port, yt_dot1x_direction_t *pPort_direction)
{
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    DOT1X_LOCK();
    if (IS_BIT_SET(DOT1X_PORT_DIR_MASK(unit), macid))
    {
        *pPort_direction = YT_DOT1X_AUTH_DIR_IN;
    }
    else
    {
        *pPort_direction = YT_DOT1X_AUTH_DIR_BOTH;
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_macBasedEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;

#if 0
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2_learn_per_port_ctrl;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    if (YT_ENABLE == enable)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);
        /* trap to cpu and learn suspend fdb*/
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2_learn_per_port_ctrl, YT_DISABLE);
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_MODEf, &l2_learn_per_port_ctrl, YT_L2_LEARN_MODE_CPU_CONTROL);
        /* control suspend fdb trap to cpu */
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_SUSPEND_DROP_ENf, &l2_learn_per_port_ctrl, YT_DISABLE);
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_SUSPEND_COPY_ENf, &l2_learn_per_port_ctrl, YT_ENABLE);
    }
    else
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);
        /* trap to cpu and learn suspend fdb*/
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2_learn_per_port_ctrl, YT_DISABLE);
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_MODEf, &l2_learn_per_port_ctrl, YT_L2_LEARN_MODE_AUTO);
        /* control suspend fdb trap to cpu */
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_SUSPEND_DROP_ENf, &l2_learn_per_port_ctrl, YT_DISABLE);
        HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_SUSPEND_COPY_ENf, &l2_learn_per_port_ctrl, YT_DISABLE);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2_learn_per_port_ctrl), &l2_learn_per_port_ctrl), ret);

    DOT1X_LOCK();
    if (YT_ENABLE == enable)
    {
        SET_BIT(DOT1X_MAC_BASE_MASK(unit), macid);
    }
    else
    {
        CLEAR_BIT(DOT1X_MAC_BASE_MASK(unit), macid);
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
#endif
}

yt_ret_t fal_tiger_dot1x_macBasedEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;

#if 0
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    DOT1X_LOCK();
    if (IS_BIT_SET(DOT1X_MAC_BASE_MASK(unit), macid))
    {
        *pEnable = YT_ENABLE;
    }
    else
    {
        *pEnable = YT_DISABLE;
    }
    DOT1X_UNLOCK();

    return CMM_ERR_OK;
#endif
}

yt_ret_t fal_tiger_dot1x_guest_vlan_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_BYPASS_1X_ACCESS_CONTROLf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_guest_vlan_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_BYPASS_1X_ACCESS_CONTROLf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_tx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_BC_TX_BYPASSf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_tx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_BC_TX_BYPASSf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_tx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_MC_TX_BYPASSf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_tx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_MC_TX_BYPASSf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_rx_bypass_bc_set(yt_unit_t unit, yt_enable_t enable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_BC_RX_BYPASSf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_rx_bypass_bc_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_BC_RX_BYPASSf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_rx_bypass_mc_set(yt_unit_t unit, yt_enable_t enable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_SET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_MC_RX_BYPASSf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_rx_bypass_mc_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    l2_dot1x_ctrl2_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_DOT1X_CTRL2m, 0, sizeof(l2_dot1x_ctrl2_t), &entry), ret); 
    HAL_FIELD_GET(L2_DOT1X_CTRL2m, L2_DOT1X_CTRL2_MC_RX_BYPASSf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_dot1x_eapol_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(actType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_dot1x_eapol_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t* pActType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pActType);

    return CMM_ERR_NOT_SUPPORT;
}

