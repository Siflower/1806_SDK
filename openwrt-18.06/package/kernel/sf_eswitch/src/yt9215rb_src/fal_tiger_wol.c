#include "fal_tiger_wol.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"

yt_ret_t  fal_tiger_wol_ctrl_set(yt_unit_t unit, yt_enable_t enable)
{
    wol_ctrl_t wol_ctrl;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);
    HAL_FIELD_SET(WOL_CTRLm, WOL_CTRL_WOL_ENf, &wol_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_wol_ctrl_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    wol_ctrl_t wol_ctrl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);
    HAL_FIELD_GET(WOL_CTRLm, WOL_CTRL_WOL_ENf, &wol_ctrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_wol_ethertype_set(yt_unit_t unit, yt_tpid_t eth_type)
{
    wol_ctrl_t wol_ctrl;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);
    HAL_FIELD_SET(WOL_CTRLm, WOL_CTRL_WOL_ETH_TYPEf, &wol_ctrl, eth_type);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_wol_ethertype_get(yt_unit_t unit, yt_tpid_t *pEth_type)
{
    wol_ctrl_t wol_ctrl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t eth_type;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, WOL_CTRLm, 0, sizeof(wol_ctrl_t), &wol_ctrl), ret);
    HAL_FIELD_GET(WOL_CTRLm, WOL_CTRL_WOL_ETH_TYPEf, &wol_ctrl, &eth_type);
    *pEth_type = eth_type;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_wol_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_wol_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_wol_port_macAddr_set(yt_unit_t unit, yt_port_t port, yt_mac_addr_t macAddr)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(macAddr);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_wol_port_macAddr_get(yt_unit_t unit, yt_port_t port, yt_mac_addr_t *pMacAddr)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pMacAddr);

    return CMM_ERR_NOT_SUPPORT;
}

