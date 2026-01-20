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

/**
 * @internal      fal_tiger_stp_state_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[in]     state               -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_state_set(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t state)
{
    l2_stp_staten_t l2_stp_staten;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    int32_t stpState = 0;;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stpId, CMM_ERR_INPUT);
    //Compatible with shark stp state structures
    if(state == STP_STATE_DISABLE)
    {
        state = 0;
    }
    else if(state == STP_STATE_FORWARD)
    {
        state = 3;
    }
    else
    {
        /*nothing to do */
    }
    osal_memset(&l2_stp_staten, sizeof(l2_stp_staten_t), 0, sizeof(l2_stp_staten_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_STP_STATENm, stpId, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);
    HAL_FIELD_GET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, &stpState);
    stpState &= ~(0x3<<(macid << 1));
    stpState |= (state<<(macid << 1));
    HAL_FIELD_SET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, stpState);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_STP_STATENm, stpId, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_stp_state_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     stpId               -stp instance id
 * @param[in]     port                -port num
 * @param[out]    pState              -stp state, STP_STATE_LEARN, STP_STATE_DISCARD, STP_STATE_FORWARD
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_state_get(yt_unit_t unit, yt_stp_id_t stpId, yt_port_t port, yt_stp_state_t *pState)
{
    l2_stp_staten_t l2_stp_staten;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);
    int32_t stpState = 0;

    CMM_PARAM_CHK(pState == NULL, CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stpId, CMM_ERR_INPUT);
    osal_memset(&l2_stp_staten, sizeof(l2_stp_staten_t), 0, sizeof(l2_stp_staten_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_STP_STATENm, stpId, sizeof(l2_stp_staten_t), &l2_stp_staten), ret);
    HAL_FIELD_GET(L2_STP_STATENm, L2_STP_STATEN_STATEf, &l2_stp_staten, &stpState);
    *pState = 0x3 & (stpState>>(macid << 1));
    //Compatible with shark stp state structures
    if(*pState == STP_STATE_DISABLE)
    {
        *pState = 0;
    }
    else if(*pState == STP_STATE_FORWARD)
    {
        *pState = 3;
    }
    else
    {
        /*nothing to do*/
    }

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_stp_instance_map_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_instance_map_set(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t stpId)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stpId, CMM_ERR_INPUT);
    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_STP_IDf, &entry, stpId);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_stp_instance_map_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     vid                 -vlan id
 * @param[out]    pStpId              -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_instance_map_get(yt_unit_t unit, yt_vlan_t vid, yt_stp_id_t *pStpId)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t stpId = 0;

    CMM_PARAM_CHK(pStpId == NULL, CMM_ERR_NULL_POINT);
    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_STP_IDf, &entry, &stpId);
    *pStpId = stpId;
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_stp_instance_map_vlanlist_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     pVlanlist           -vlan list
 * @param[in]     stpId               -stp instance id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_instance_map_vlanlist_set(yt_unit_t unit, yt_vlan_mask_t *pVlanlist, yt_stp_id_t stpId)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_vlan_tbl_t entry;
    yt_vlan_t vid = 0;
    uint8_t u32Pos = 0;
    uint8_t uBitPos = 0;

    CMM_PARAM_CHK(CAL_MAX_VALUE_MSTI(unit) < stpId, CMM_ERR_INPUT);
    for (u32Pos = 0; u32Pos < YT_VLANS_WORD_NUM; u32Pos++)
    {
        if (pVlanlist->vlanbits[u32Pos] != 0)
        {
            for (uBitPos = 0; uBitPos < YT_VLANS_WORD_SIZE; uBitPos++)
            {
                if (((1<<uBitPos) & pVlanlist->vlanbits[u32Pos]) != 0)
                {
                    vid = u32Pos*YT_VLANS_WORD_SIZE + uBitPos;
                    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));
                    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
                    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_STP_IDf, &entry, stpId);
                    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
                }
            }
        }
    }

    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_stp_bpdu_act_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     action              -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_bpdu_act_set(yt_unit_t unit, yt_port_t port,
    yt_stp_bpdu_type_t bpduType, yt_act_type_t action)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(bpduType);
    CMM_UNUSED_PARAM(action);
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_stp_bpdu_act_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[out]    pAction             -Fwd/Drop/Copy/Trap
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_bpdu_act_get(yt_unit_t unit, yt_port_t port,
    yt_stp_bpdu_type_t bpduType, yt_act_type_t *pAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(bpduType);
    CMM_UNUSED_PARAM(pAction);
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_stp_bpdu_egr_filter_set
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[in]     portMask            -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_bpdu_egr_filter_set(yt_unit_t unit, yt_stp_bpdu_type_t bpduType,
    yt_port_mask_t portMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(bpduType);
    CMM_UNUSED_PARAM(portMask);
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_stp_bpdu_egr_filter_get
 * @endinternal
 *
 * @brief         Description
 * @note          APPLICABLE DEVICES  -Tiger
 * @param[in]     unit                -unit id
 * @param[in]     bpduType            -Customer bpdu or Provider bpdu type
 * @param[out]    pPortMask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_stp_bpdu_egr_filter_get(yt_unit_t unit, yt_stp_bpdu_type_t bpduType,
    yt_port_mask_t *pPortMask)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(bpduType);
    CMM_UNUSED_PARAM(pPortMask);
    return CMM_ERR_NOT_SUPPORT;
}

