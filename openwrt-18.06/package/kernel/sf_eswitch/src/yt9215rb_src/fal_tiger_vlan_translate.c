#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_cmm.h"
#include "fal_tiger_vlan_translate.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t fal_tiger_vlan_trans_untagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable)
{
    vlan_trans_untag_vid_mode_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t tmpMode = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    
    osal_memset(&entry, sizeof(vlan_trans_untag_vid_mode_ctrl_t), 0, sizeof(vlan_trans_untag_vid_mode_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_TRANS_UNTAG_VID_MODE_CTRLm, 0,sizeof(vlan_trans_untag_vid_mode_ctrl_t), &entry), ret);
    HAL_FIELD_GET(VLAN_TRANS_UNTAG_VID_MODE_CTRLm, VLAN_TRANS_UNTAG_VID_MODE_CTRL_MODEf, &entry, &tmpMode);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        if(enable)
        {
            SET_BIT(tmpMode, macid << 1);
        }
        else
        {
            CLEAR_BIT(tmpMode, macid << 1);
        }
    }
    else if(YT_VLAN_TYPE_SVLAN == type)
    {
        if(enable)
        {
            SET_BIT(tmpMode, (macid << 1) + 1);
        }
        else
        {
            CLEAR_BIT(tmpMode, (macid << 1) + 1);
        }
    }
    HAL_FIELD_SET(VLAN_TRANS_UNTAG_VID_MODE_CTRLm, VLAN_TRANS_UNTAG_VID_MODE_CTRL_MODEf, &entry, tmpMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_TRANS_UNTAG_VID_MODE_CTRLm, 0, sizeof(vlan_trans_untag_vid_mode_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_trans_untagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable)
{
    vlan_trans_untag_vid_mode_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t tmpMode = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(vlan_trans_untag_vid_mode_ctrl_t), 0, sizeof(vlan_trans_untag_vid_mode_ctrl_t));

    *pEnable = YT_DISABLE;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_TRANS_UNTAG_VID_MODE_CTRLm, 0,sizeof(vlan_trans_untag_vid_mode_ctrl_t), &entry), ret);
    HAL_FIELD_GET(VLAN_TRANS_UNTAG_VID_MODE_CTRLm, VLAN_TRANS_UNTAG_VID_MODE_CTRL_MODEf, &entry, &tmpMode);
    if ((YT_VLAN_TYPE_CVLAN == type && (IS_BIT_SET(tmpMode, macid << 1))) ||
        (YT_VLAN_TYPE_SVLAN == type && (IS_BIT_SET(tmpMode, (macid << 1) + 1))))
    {
        *pEnable = YT_ENABLE;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t  port,  yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(type);   
    CMM_UNUSED_PARAM(port);    
    CMM_UNUSED_PARAM(enable);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_igr_trans_prio_tagPvidIgnore_get(yt_unit_t unit, yt_vlan_type_t  type,  yt_port_t  port,  yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(type);   
    CMM_UNUSED_PARAM(port);    
    CMM_UNUSED_PARAM(pEnable);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_igr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t missAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(type);   
    CMM_UNUSED_PARAM(port);    
    CMM_UNUSED_PARAM(missAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_igr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_act_type_t *pMissAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(type);   
    CMM_UNUSED_PARAM(port);    
    CMM_UNUSED_PARAM(pMissAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange)
{
    vlan_range_profilen_t vlan_range_profilen;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange0Min), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange0Max), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange1Min), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange1Max), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange2Min), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange2Max), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange3Min), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pVlanRange->vidRange3Max), CMM_ERR_EXCEED_RANGE);

    osal_memset(&vlan_range_profilen, sizeof(vlan_range_profilen_t), 0, sizeof(vlan_range_profilen_t));

    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID0f, &vlan_range_profilen, pVlanRange->vidRange0Min);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID0f, &vlan_range_profilen, pVlanRange->vidRange0Max);
    
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID1_0f, &vlan_range_profilen, (pVlanRange->vidRange1Min & 0xFF));
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID1_1f, &vlan_range_profilen, (pVlanRange->vidRange1Min >> 8));
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID1f, &vlan_range_profilen, pVlanRange->vidRange1Max);
    
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID2f, &vlan_range_profilen, pVlanRange->vidRange2Min);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_0f, &vlan_range_profilen, pVlanRange->vidRange2Max & 0xF);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_1f, &vlan_range_profilen, pVlanRange->vidRange2Max >> 4);
    
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID3f, &vlan_range_profilen, pVlanRange->vidRange3Min);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID3f, &vlan_range_profilen, pVlanRange->vidRange3Max);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_RANGE_PROFILENm, profileId, sizeof(vlan_range_profilen), &vlan_range_profilen), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profile_id, yt_vlan_range_group_t *pVlan_range)
{
    vlan_range_profilen_t vlan_range_profilen;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t tmp_range1 = 0;
    uint16_t tmp_range2 = 0;

    osal_memset(&vlan_range_profilen, sizeof(vlan_range_profilen_t), 0, sizeof(vlan_range_profilen_t));

    CMM_PARAM_CHK(0 == profile_id, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(CAL_MAX_VLAN_RANGE_PROFILE_NUM(unit) <= profile_id, CMM_ERR_EXCEED_RANGE);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_RANGE_PROFILENm, profile_id, sizeof(vlan_range_profilen_t), &vlan_range_profilen), ret);
    
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID0f, &vlan_range_profilen, &pVlan_range->vidRange0Min);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID0f, &vlan_range_profilen, &pVlan_range->vidRange0Max);
    
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID1_0f, &vlan_range_profilen, &tmp_range1);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID1_1f, &vlan_range_profilen, &tmp_range2);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID1f, &vlan_range_profilen, &pVlan_range->vidRange1Max);
    pVlan_range->vidRange1Min = (tmp_range2 << 8) | tmp_range1;

    tmp_range2 = tmp_range1 = 0;
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID2f, &vlan_range_profilen, &pVlan_range->vidRange2Min);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_0f, &vlan_range_profilen, &tmp_range1);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_1f, &vlan_range_profilen, &tmp_range2);
    pVlan_range->vidRange2Max = (tmp_range2 << 4) | tmp_range1;
    
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MIN_VID3f, &vlan_range_profilen, &pVlan_range->vidRange3Min);
    HAL_FIELD_GET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID3f, &vlan_range_profilen, &pVlan_range->vidRange3Max);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profile_id)
{
    vlan_range_profilen_t vlan_range_profilen;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&vlan_range_profilen, sizeof(vlan_range_profilen_t), 0, sizeof(vlan_range_profilen_t));

    CMM_PARAM_CHK(0 == profile_id, CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(CAL_MAX_VLAN_RANGE_PROFILE_NUM(unit) <= profile_id, CMM_ERR_EXCEED_RANGE);

    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID0f, &vlan_range_profilen, 0);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID1f, &vlan_range_profilen, 0);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_0f, &vlan_range_profilen, 0);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID2_1f, &vlan_range_profilen, 0);
    HAL_FIELD_SET(VLAN_RANGE_PROFILENm, VLAN_RANGE_PROFILEN_MAX_VID3f, &vlan_range_profilen, 0);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_RANGE_PROFILENm, profile_id, sizeof(vlan_range_profilen), &vlan_range_profilen), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_port_rangeProfileSel_set(yt_unit_t unit, yt_port_t  port, yt_profile_id_t profile_id)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_PARAM_CHK(CAL_MAX_VLAN_RANGE_PROFILE_NUM(unit) <= profile_id, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);
    HAL_FIELD_SET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_VLAN_RANGE_PROFILE_IDf, &tmpCtrl, profile_id);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_port_rangeProfileSel_get(yt_unit_t unit, yt_port_t  port, yt_profile_id_t *pProfile_id)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);
    HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_VLAN_RANGE_PROFILE_IDf, &tmpCtrl, pProfile_id);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_mode_set(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t rangeMode)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);
    HAL_FIELD_SET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_VLAN_RANGE_MODEf, &tmpCtrl, rangeMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_trans_mode_get(yt_unit_t unit, yt_port_t  port, yt_vlan_range_trans_mode_t *pRangeMode)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    
    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);
    HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_VLAN_RANGE_MODEf, &tmpCtrl, pRangeMode);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_igr_trans_table_add(yt_unit_t unit, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pTableIdx)
{
    vlan_xlate_tbl_t entry;
    vlan_xlate_action_tbl_t entryAction;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_port_mask_t  portmask;
    yt_trans_tbl_id_t entryIdx = 0;
    uint32_t valid = 0;
    uint32_t tmpPortmask = 0;

    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pRuleTbl->svidOrVidRangeId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pRuleTbl->cvidOrVidRangeId), CMM_ERR_EXCEED_RANGE);

    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_CLEAR_MEMBER_PORT(portmask);

    osal_memset(&entry, sizeof(vlan_xlate_tbl_t), 0, sizeof(vlan_xlate_tbl_t));
    osal_memset(&entryAction, sizeof(vlan_xlate_action_tbl_t), 0, sizeof(vlan_xlate_action_tbl_t));

    for(entryIdx = 0; entryIdx < CAL_VLAN_XLATE_ENTRY_NUM(unit); entryIdx++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_XLATE_TBLm, entryIdx, sizeof(vlan_xlate_tbl_t), &entry), ret);
        HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_VALIDf, &entry, &valid);
        HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SRC_PORT_MASKf, &entry, &tmpPortmask);
        if (!valid && !tmpPortmask)
        {
            break;
        }
    }
    CMM_PARAM_CHK(CAL_VLAN_XLATE_ENTRY_NUM(unit) == entryIdx, CMM_ERR_TABLE_FULL);

    portmask.portbits[0] = pRuleTbl->srcPortMaskNeedCare.portbits[0];
    CAL_YTPLIST_TO_MLIST(unit, portmask, macmask);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_VALIDf, &entry, YT_ENABLE);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_0f, &entry, (pRuleTbl->svidOrVidRangeId&0x1));
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_1f, &entry, (pRuleTbl->svidOrVidRangeId>>1));
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_INCLf, &entry, pRuleTbl->svidNeedCare);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_STAG_FMTf, &entry, pRuleTbl->stagFormat);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_STAG_FMT_INCLf, &entry, pRuleTbl->stagFormatNeedCare);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CVIDf, &entry, pRuleTbl->cvidOrVidRangeId);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CVID_INCLf, &entry, pRuleTbl->cvidNeedCare);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CTAG_FMTf, &entry, pRuleTbl->ctagFormat);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CTAG_FMT_INCLf, &entry, pRuleTbl->ctagFormatNeedCare);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SRC_PORT_MASKf, &entry, macmask.portbits[0]);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_XLATE_TBLm, entryIdx, sizeof(vlan_xlate_tbl_t), &entry), ret);

    HAL_FIELD_SET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_SVID_CMDf, &entryAction, pAction->svidAction);
    HAL_FIELD_SET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_SVID_0f, &entryAction, pAction->assignSvid& 0x3F);
    HAL_FIELD_SET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_SVID_1f, &entryAction, pAction->assignSvid >> 6);
    HAL_FIELD_SET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_CVID_CMDf, &entryAction, pAction->cvidAction);
    HAL_FIELD_SET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_CVIDf, &entryAction, pAction->assignCvid);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_XLATE_ACTION_TBLm, entryIdx, sizeof(vlan_xlate_action_tbl_t), &entryAction), ret);

    *pTableIdx = entryIdx;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_igr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_vlan_trans_tbl_t *pRuleTbl,  const yt_vlan_trans_action_tbl_t *pAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(tableIdx);   
    CMM_UNUSED_PARAM(pRuleTbl);    
    CMM_UNUSED_PARAM(pAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_igr_trans_table_get(yt_unit_t unit,  yt_trans_tbl_id_t tableIdx, yt_vlan_trans_tbl_t *pRuleTbl,  yt_vlan_trans_action_tbl_t *pAction)
{
    vlan_xlate_tbl_t entry;
    vlan_xlate_action_tbl_t entryAction;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t portmask;
    yt_port_mask_t macmask;
    uint32_t tmpPortmask = 0;
    uint32_t valid = 0;
    uint16_t svid0 = 0;
    uint16_t svid1 = 0;

    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_CLEAR_MEMBER_PORT(portmask);

    osal_memset(&entry, sizeof(vlan_xlate_tbl_t), 0, sizeof(vlan_xlate_tbl_t));
    osal_memset(&entryAction, sizeof(vlan_xlate_action_tbl_t), 0, sizeof(vlan_xlate_action_tbl_t));
    
    CMM_PARAM_CHK(CAL_VLAN_XLATE_ENTRY_NUM(unit) <= tableIdx, CMM_ERR_EXCEED_RANGE);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_XLATE_TBLm, tableIdx, sizeof(vlan_xlate_tbl_t), &entry), ret);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_VALIDf, &entry, &valid);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SRC_PORT_MASKf, &entry, &tmpPortmask);
    if (!valid && !tmpPortmask)
    {
        osal_memset(pRuleTbl, sizeof(yt_vlan_trans_tbl_t), 0, sizeof(yt_vlan_trans_tbl_t));
        osal_memset(pAction, sizeof(yt_vlan_trans_action_tbl_t), 0, sizeof(yt_vlan_trans_action_tbl_t));
        return CMM_ERR_OK;
    }
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_INCLf, &entry, &pRuleTbl->svidNeedCare);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_0f, &entry, &svid0);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SVID_1f, &entry, &svid1);
    pRuleTbl->svidOrVidRangeId = svid0 | (svid1 << 1);
    
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_STAG_FMT_INCLf, &entry, &pRuleTbl->stagFormatNeedCare);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_STAG_FMTf, &entry, &pRuleTbl->stagFormat);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CVID_INCLf, &entry, &pRuleTbl->cvidNeedCare);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CVIDf, &entry, &pRuleTbl->cvidOrVidRangeId);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CTAG_FMT_INCLf, &entry, &pRuleTbl->ctagFormatNeedCare);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_CTAG_FMTf, &entry, &pRuleTbl->ctagFormat);
    HAL_FIELD_GET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SRC_PORT_MASKf, &entry, macmask.portbits);    
    CAL_MLIST_TO_YTPLIST(unit, macmask, portmask);
    pRuleTbl->srcPortMaskNeedCare.portbits[0]= portmask.portbits[0];

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_XLATE_ACTION_TBLm, tableIdx, sizeof(vlan_xlate_action_tbl_t), &entryAction), ret);
    HAL_FIELD_GET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_SVID_CMDf, &entryAction, &pAction->svidAction);
    HAL_FIELD_GET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_SVID_0f, &entryAction, &svid0);
    HAL_FIELD_GET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_SVID_1f, &entryAction, &svid1);
    pAction->assignSvid= svid0 | (svid1<< 6);
    HAL_FIELD_GET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_CVID_CMDf, &entryAction, &pAction->cvidAction);
    HAL_FIELD_GET(VLAN_XLATE_ACTION_TBLm, VLAN_XLATE_ACTION_TBL_NEW_CVIDf, &entryAction, &pAction->assignCvid);

    return CMM_ERR_OK;
}


yt_ret_t  fal_tiger_vlan_igr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t tableIdx)
{
    vlan_xlate_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&entry, sizeof(vlan_xlate_tbl_t), 0, sizeof(vlan_xlate_tbl_t));

    CMM_PARAM_CHK(CAL_VLAN_XLATE_ENTRY_NUM(unit) <= tableIdx, CMM_ERR_EXCEED_RANGE);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, VLAN_XLATE_TBLm, tableIdx, sizeof(vlan_xlate_tbl_t), &entry), ret);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_VALIDf, &entry, YT_DISABLE);
    HAL_FIELD_SET(VLAN_XLATE_TBLm, VLAN_XLATE_TBL_SRC_PORT_MASKf, &entry, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, VLAN_XLATE_TBLm, tableIdx, sizeof(vlan_xlate_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_egr_trans_lookup_miss_ctrl_set(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t missAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(port);   
    CMM_UNUSED_PARAM(type);    
    CMM_UNUSED_PARAM(missAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_egr_trans_lookup_miss_ctrl_get(yt_unit_t unit, yt_port_t  port, yt_vlan_type_t  type,  yt_vlan_egrtrans_lookup_miss_act_t *pMissAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(port);   
    CMM_UNUSED_PARAM(type);    
    CMM_UNUSED_PARAM(pMissAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_add(yt_unit_t unit, yt_profile_id_t profileId, const yt_vlan_range_group_t *pVlanRange)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(profileId);   
    CMM_UNUSED_PARAM(pVlanRange);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_get(yt_unit_t unit, yt_profile_id_t profileId, yt_vlan_range_group_t *pVlanRange)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(profileId);   
    CMM_UNUSED_PARAM(pVlanRange);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_egr_trans_rangeProfile_del(yt_unit_t unit, yt_profile_id_t profileId)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(profileId);   

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_vlan_egr_trans_table_add(yt_unit_t unit, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction, yt_trans_tbl_id_t *pEntry_id)
{
    egr_vlan_trans_rule_ctrln_t entry;
    egr_vlan_trans_rule_ctrl1n_t entry1;
    egr_vlan_trans_data_ctrln_t entryData;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_port_mask_t  portmask;
    yt_trans_tbl_id_t entryIdx = 0;
    uint32_t valid = 0;
    uint32_t tmpPortmask = 0;

    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pRuleTbl->vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pRuleTbl->vidRangeMin), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pRuleTbl->vidRangeMax), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((pRuleTbl->vidRangeMax < pRuleTbl->vidRangeMin), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_RANGE_TRANS_MODE_CVLAN > pRuleTbl->vidRangeMode || YT_VLAN_RANGE_TRANS_MODE_SVLAN < pRuleTbl->vidRangeMode), CMM_ERR_EXCEED_RANGE);

    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_CLEAR_MEMBER_PORT(portmask);

    osal_memset(&entry, sizeof(egr_vlan_trans_rule_ctrln_t), 0, sizeof(egr_vlan_trans_rule_ctrln_t));
    osal_memset(&entry1, sizeof(egr_vlan_trans_rule_ctrl1n_t), 0, sizeof(egr_vlan_trans_rule_ctrl1n_t));
    osal_memset(&entryData, sizeof(egr_vlan_trans_data_ctrln_t), 0, sizeof(egr_vlan_trans_data_ctrln_t));

    for(entryIdx = 0; entryIdx < CAL_VLAN_EGR_XLATE_TBL_NUM(unit); entryIdx++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TRANS_RULE_CTRLNm, entryIdx, sizeof(egr_vlan_trans_rule_ctrln_t), &entry), ret);
        HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VALIDf, &entry, &valid);
        HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PORT_MASKf, &entry, &tmpPortmask);
        if (valid == 0 && !tmpPortmask)
        {
            break;
        }
    }
    CMM_PARAM_CHK(CAL_VLAN_EGR_XLATE_TBL_NUM(unit) == entryIdx, CMM_ERR_TABLE_FULL);

    portmask.portbits[0] = pRuleTbl->dstPortMaskNeedCare.portbits[0];
    CAL_YTPLIST_TO_MLIST(unit, portmask, macmask);

    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VALIDf, &entry, YT_ENABLE);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_CVID_INCLf, &entry, pRuleTbl->cvidNeedCare);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_SVID_INCLf, &entry, pRuleTbl->svidNeedCare);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_MVR_INCLf, &entry, pRuleTbl->mvrNeedCare);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PORT_MASKf, &entry, macmask.portbits[0]);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VID0f, &entry, pRuleTbl->vid);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PKT_CTAG_INCLf, &entry, pRuleTbl->originalCtagFormatNeedCare);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PKT_STAG_INCLf, &entry, pRuleTbl->originalStagFormatNeedCare);

    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID_RANGE_TYPEf, &entry1, pRuleTbl->vidRangeMode);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID1f, &entry1, pRuleTbl->vidRangeMin);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID2f, &entry1, pRuleTbl->vidRangeMax);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_VLAN_TRANS_RULE_CTRLNm, entryIdx, sizeof(egr_vlan_trans_rule_ctrln_t), &entry), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_VLAN_TRANS_RULE_CTRL1Nm, entryIdx, sizeof(egr_vlan_trans_rule_ctrl1n_t), &entry1), ret);
    
    HAL_FIELD_SET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_SVID_ENf, &entryData, pAction->svidEnable);
    HAL_FIELD_SET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_CVID_ENf, &entryData, pAction->cvidEnable);
    HAL_FIELD_SET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_SVIDf, &entryData, pAction->assignSvid);
    HAL_FIELD_SET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_CVIDf, &entryData, pAction->assignCvid);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_VLAN_TRANS_DATA_CTRLNm, entryIdx, sizeof(egr_vlan_trans_data_ctrln_t), &entryData), ret);

    *pEntry_id = entryIdx;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_egr_trans_table_add_by_index(yt_unit_t unit, yt_trans_tbl_id_t tableIdx, const yt_egr_vlan_trans_tbl_t  *pRuleTbl, const yt_egr_vlan_trans_action_tbl_t *pAction)
{
    CMM_UNUSED_PARAM(unit);    
    CMM_UNUSED_PARAM(tableIdx);   
    CMM_UNUSED_PARAM(pRuleTbl);    
    CMM_UNUSED_PARAM(pAction);    

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_vlan_egr_trans_table_get(yt_unit_t unit, yt_trans_tbl_id_t entryIdx, yt_egr_vlan_trans_tbl_t  *pRuleTbl, yt_egr_vlan_trans_action_tbl_t *pAction)
{
    egr_vlan_trans_rule_ctrln_t entry;
    egr_vlan_trans_rule_ctrl1n_t entry1;
    egr_vlan_trans_data_ctrln_t entryData;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t portmask;
    yt_port_mask_t  macmask;
    uint32_t valid = 0;
    uint32_t tmpPortmask = 0;
    
    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_CLEAR_MEMBER_PORT(portmask);

    osal_memset(&entry, sizeof(egr_vlan_trans_rule_ctrln_t), 0, sizeof(egr_vlan_trans_rule_ctrln_t));
    osal_memset(&entry1, sizeof(egr_vlan_trans_rule_ctrl1n_t), 0, sizeof(egr_vlan_trans_rule_ctrl1n_t));
    osal_memset(&entryData, sizeof(egr_vlan_trans_data_ctrln_t), 0, sizeof(egr_vlan_trans_data_ctrln_t));

    CMM_PARAM_CHK(CAL_VLAN_EGR_XLATE_TBL_NUM(unit) <= entryIdx, CMM_ERR_EXCEED_RANGE);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TRANS_RULE_CTRLNm, entryIdx, sizeof(egr_vlan_trans_rule_ctrln_t), &entry), ret);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VALIDf, &entry, &valid);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PORT_MASKf, &entry, &tmpPortmask);
    if (valid == 0 && !tmpPortmask)
    {
        osal_memset(pRuleTbl, sizeof(yt_egr_vlan_trans_tbl_t), 0, sizeof(yt_egr_vlan_trans_tbl_t));
        osal_memset(pAction, sizeof(yt_egr_vlan_trans_action_tbl_t), 0, sizeof(yt_egr_vlan_trans_action_tbl_t));
        return CMM_ERR_OK;
    }
    
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_SVID_INCLf, &entry, &pRuleTbl->svidNeedCare);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_CVID_INCLf, &entry, &pRuleTbl->cvidNeedCare);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_MVR_INCLf, &entry, &pRuleTbl->mvrNeedCare);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PORT_MASKf, &entry, macmask.portbits);
    CAL_MLIST_TO_YTPLIST(unit, macmask, portmask);
    pRuleTbl->dstPortMaskNeedCare.portbits[0] = portmask.portbits[0];
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TRANS_RULE_CTRL1Nm, entryIdx, sizeof(egr_vlan_trans_rule_ctrl1n_t), &entry1), ret);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID_RANGE_TYPEf, &entry1, &pRuleTbl->vidRangeMode);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID1f, &entry1, &pRuleTbl->vidRangeMin);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRL1Nm, EGR_VLAN_TRANS_RULE_CTRL1N_VID2f, &entry1, &pRuleTbl->vidRangeMax);    
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VID0f, &entry, &pRuleTbl->vid);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PKT_CTAG_INCLf, &entry, &pRuleTbl->originalCtagFormatNeedCare);
    HAL_FIELD_GET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PKT_STAG_INCLf, &entry, &pRuleTbl->originalStagFormatNeedCare);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TRANS_DATA_CTRLNm, entryIdx, sizeof(egr_vlan_trans_data_ctrln_t), &entryData), ret);
    HAL_FIELD_GET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_SVID_ENf, &entryData, &pAction->svidEnable);
    HAL_FIELD_GET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_CVID_ENf, &entryData, &pAction->cvidEnable);
    HAL_FIELD_GET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_SVIDf, &entryData, &pAction->assignSvid);
    HAL_FIELD_GET(EGR_VLAN_TRANS_DATA_CTRLNm, EGR_VLAN_TRANS_DATA_CTRLN_CVIDf, &entryData, &pAction->assignCvid);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_egr_trans_table_del(yt_unit_t unit, yt_trans_tbl_id_t entryIdx)
{
    egr_vlan_trans_rule_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&entry, sizeof(egr_vlan_trans_rule_ctrln_t), 0, sizeof(egr_vlan_trans_rule_ctrln_t));

    CMM_PARAM_CHK(CAL_VLAN_EGR_XLATE_TBL_NUM(unit) <= entryIdx, CMM_ERR_EXCEED_RANGE);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TRANS_RULE_CTRLNm, entryIdx, sizeof(egr_vlan_trans_rule_ctrln_t), &entry), ret);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_VALIDf, &entry, YT_DISABLE);
    HAL_FIELD_SET(EGR_VLAN_TRANS_RULE_CTRLNm, EGR_VLAN_TRANS_RULE_CTRLN_PORT_MASKf, &entry, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_VLAN_TRANS_RULE_CTRLNm, entryIdx, sizeof(egr_vlan_trans_rule_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}
