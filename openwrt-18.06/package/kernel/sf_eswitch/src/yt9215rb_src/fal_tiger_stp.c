/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_stp.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t fal_tiger_stp_state_set(yt_unit_t unit, yt_stp_id_t stp_id, yt_port_t port, yt_stp_state_t state)
{
    l2_stp_staten_t l2_stp_staten;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    int32_t stpState;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stp_id, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_STP_STATENm, stp_id, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);
    HAL_FIELD_GET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, &stpState);
    stpState &= ~(0x3 <<(macid << 1));
    stpState |= (state <<(macid << 1));
    HAL_FIELD_SET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, stpState);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_STP_STATENm, stp_id, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stp_state_get(yt_unit_t unit, yt_stp_id_t stp_id, yt_port_t port, yt_stp_state_t *pState)
{
    l2_stp_staten_t l2_stp_staten;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    int32_t stpState;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stp_id, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_STP_STATENm, stp_id, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);
    HAL_FIELD_GET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, &stpState);
    *pState = 0x3 & (stpState >> (macid << 1));

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stp_id)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stp_id, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_STP_IDf, &entry, stp_id);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStp_id)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t stp_id;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_STP_IDf, &entry, &stp_id);
    *pStp_id = stp_id;

    return CMM_ERR_OK;
}
