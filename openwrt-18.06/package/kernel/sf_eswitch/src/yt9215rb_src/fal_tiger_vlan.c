/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_cmm.h"
#include "fal_tiger_vlan.h"
#include "hal_mem.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_mem.h"


static uint32_t fal_tiger_vlan_port_egrTagKeep_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_enable_t enable);
static uint32_t fal_tiger_vlan_port_egrTagKeep_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_enable_t *pEnable);

yt_ret_t  fal_tiger_vlan_init(yt_unit_t unit)
{
    yt_tpid_profiles_t tpids;
    yt_port_t port = 0;

    osal_memset(&tpids, sizeof(yt_tpid_profiles_t), 0, sizeof(yt_tpid_profiles_t));

    fal_tiger_vlan_port_set(unit, 1, YT_LOG_ALL_PORT_MASK(unit), YT_LOG_ALL_PORT_MASK(unit));

    tpids.tpid[0] = 0x8100;
    tpids.tpid[1] = 0x88a8;
    fal_tiger_vlan_egrTpid_set(unit, tpids);
    for(port = 0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        fal_tiger_vlan_port_egrTpidIdx_set(unit, YT_VLAN_TYPE_CVLAN, port, 0);
        fal_tiger_vlan_port_egrTpidIdx_set(unit, YT_VLAN_TYPE_SVLAN, port, 1);
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_set(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  member_portmask, yt_port_mask_t  untag_portmask)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_port_mask_t  utagmacmask;

    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));

    CAL_YTPLIST_TO_MLIST(unit, member_portmask, macmask);
    CAL_YTPLIST_TO_MLIST(unit, untag_portmask, utagmacmask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_PORT_MEMBER_BITMAPf, &entry, macmask.portbits[0]);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_UNTAG_MEMBER_BITMAPf, &entry, utagmacmask.portbits[0]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_get(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  *pMember_portmask, yt_port_mask_t  *pUntag_portmask)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t macMask;
    yt_port_mask_t utagmacmask;
	
    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));
	
    CMM_CLEAR_MEMBER_PORT(macMask);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_PORT_MEMBER_BITMAPf, &entry, macMask.portbits);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_UNTAG_MEMBER_BITMAPf, &entry, utagmacmask.portbits);

    CAL_MLIST_TO_YTPLIST(unit, macMask, (*pMember_portmask));
    CAL_MLIST_TO_YTPLIST(unit, utagmacmask, (*pUntag_portmask));

    return 0;
}

yt_ret_t  fal_tiger_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t enable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
	
    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_SVL_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t *pEnable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_SVL_ENf, &entry, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t fid)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_FID_0f, &entry, (fid & 0x1FF));
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_FID_1f, &entry, (fid >>9));
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_fid_get(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t *pFid)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_fid_t fid0 = 0;
    yt_fid_t fid1 = 0;

    osal_memset(&entry, sizeof(l2_vlan_tbl_t), 0, sizeof(l2_vlan_tbl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_FID_0f, &entry, &fid0);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_FID_1f, &entry, &fid1);
    *pFid = fid0 + (fid1 << 9);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_ingrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpid)
{
    tpid_profile0_t profile;
    uint8_t i = 0;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&profile, sizeof(tpid_profile0_t), 0, sizeof(tpid_profile0_t));

    for(i = 0; CAL_VLAN_TPID_PROFILE_NUM/2 > i; i++)
    {
        HAL_FIELD_SET(TPID_PROFILE0m, TPID_PROFILE0_TPIDf, &profile, tpid.tpid[i]);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, TPID_PROFILE0m, i, sizeof(profile), &profile), ret);

    }

    for(i = 2; CAL_VLAN_TPID_PROFILE_NUM > i; i++)
    {
        HAL_FIELD_SET(TPID_PROFILE1m, TPID_PROFILE1_TPIDf, &profile, tpid.tpid[i]);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, TPID_PROFILE1m, i-2, sizeof(profile), &profile), ret);

    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_ingrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpid)
{
    tpid_profile0_t profile;
    uint8_t i = 0;
    cmm_err_t ret = CMM_ERR_OK;

    osal_memset(&profile, sizeof(tpid_profile0_t), 0, sizeof(tpid_profile0_t));

    for(i = 0; CAL_VLAN_TPID_PROFILE_NUM/2 > i; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, TPID_PROFILE0m, i, sizeof(profile), &profile), ret);
        HAL_FIELD_GET(TPID_PROFILE0m, TPID_PROFILE0_TPIDf, &profile, &(pTpid->tpid[i]));
    }

    for(i = 2; CAL_VLAN_TPID_PROFILE_NUM > i; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, TPID_PROFILE1m, i-2, sizeof(profile), &profile), ret);
        HAL_FIELD_GET(TPID_PROFILE1m, TPID_PROFILE1_TPIDf, &profile, &(pTpid->tpid[i]));
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrTpidMask_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t tpidIdxMask)
{
    parser_port_ctrln_t parserPortCtrln;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&parserPortCtrln, sizeof(parser_port_ctrln_t), 0, sizeof(parser_port_ctrln_t));

    if(type > YT_VLAN_TYPE_SVLAN ||
        tpidIdxMask >= (1<<CAL_VLAN_TPID_PROFILE_NUM))
    {
            return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PARSER_PORT_CTRLNm, macid, sizeof(parser_port_ctrln_t), &parserPortCtrln), ret);

    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_STAG_TPID_MASKf, &parserPortCtrln, (tpidIdxMask & 0xF));
    }
    else if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_CTAG_TPID_MASKf, &parserPortCtrln, (tpidIdxMask & 0xF));
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PARSER_PORT_CTRLNm, macid, sizeof(parserPortCtrln), &parserPortCtrln), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrTpidMask_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask)
{
    parser_port_ctrln_t parserPortCtrln;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&parserPortCtrln, sizeof(parser_port_ctrln_t), 0, sizeof(parser_port_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PARSER_PORT_CTRLNm, macid, sizeof(parser_port_ctrln_t), &parserPortCtrln), ret);

    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_STAG_TPID_MASKf, &parserPortCtrln, pTpidIdxMask);
    }
    else if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_CTAG_TPID_MASKf, &parserPortCtrln, pTpidIdxMask);
    }


    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrDefaultVlan_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t vid)
{
    port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(port_vlan_ctrln_t), 0, sizeof(port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, vid);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, vid);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrDefaultVlan_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pVid)
{
    port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(port_vlan_ctrln_t), 0, sizeof(port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, pVid);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, pVid);
    }

    return CMM_ERR_OK;
}

yt_ret_t   fal_tiger_vlan_port_igrDefPri_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t pri)
{
    port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SPRIOf, &entry, pri);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_CPRIOf, &entry, pri);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t   fal_tiger_vlan_port_igrDefPri_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t *pPri)
{
    port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SPRIOf, &entry, pPri);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_CPRIOf, &entry, pPri);
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enabled)
{
    l2_vlan_ingress_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(l2_vlan_ingress_filter_en_t), 0, sizeof(l2_vlan_ingress_filter_en_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_INGRESS_FILTER_ENm, 0,sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_FILTER_ENf, &entry, &macmask);
    if(enabled)
    {
        SET_BIT(macmask, macid);
    }
    else
    {
        CLEAR_BIT(macmask, macid);
    }
    HAL_FIELD_SET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_FILTER_ENf, &entry, macmask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_INGRESS_FILTER_ENm, 0, sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrFilter_enable_get(yt_unit_t unit, yt_port_t  port, yt_enable_t *pEnabled)
{
    l2_vlan_ingress_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(l2_vlan_ingress_filter_en_t), 0, sizeof(l2_vlan_ingress_filter_en_t));

    *pEnabled = YT_DISABLE;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_INGRESS_FILTER_ENm, 0,sizeof(l2_vlan_ingress_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_INGRESS_FILTER_ENm, L2_VLAN_INGRESS_FILTER_EN_FILTER_ENf, &entry, &macmask);

    *pEnabled = IS_BIT_SET(macmask, macid) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_ingrTransparent_set(yt_unit_t unit, yt_port_t  port, yt_port_mask_t port_mask)
{
    l2_port_vlan_transparent_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    yt_port_mask_t  macmask;
    CAL_YTPLIST_TO_MLIST(unit, port_mask, macmask);

    osal_memset(&entry, sizeof(l2_port_vlan_transparent_ctrl_t), 0, sizeof(l2_port_vlan_transparent_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_VLAN_TRANSPARENT_CTRLm, macid, sizeof(l2_port_vlan_transparent_ctrl_t), &entry), ret);
    HAL_FIELD_SET(L2_PORT_VLAN_TRANSPARENT_CTRLm, L2_PORT_VLAN_TRANSPARENT_CTRL_PORTMASKf, &entry, macmask.portbits[0]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_PORT_VLAN_TRANSPARENT_CTRLm, macid, sizeof(l2_port_vlan_transparent_ctrl_t), &entry), ret);
 
    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_vlan_ingrTransparent_get(yt_unit_t unit, yt_port_t  port, yt_port_mask_t *pPort_mask)
{
    l2_port_vlan_transparent_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    yt_port_mask_t macMask;

    CMM_CLEAR_MEMBER_PORT(macMask);
    osal_memset(&entry, sizeof(l2_port_vlan_transparent_ctrl_t), 0, sizeof(l2_port_vlan_transparent_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_VLAN_TRANSPARENT_CTRLm, macid, sizeof(l2_port_vlan_transparent_ctrl_t), &entry), ret);
    HAL_FIELD_GET(L2_PORT_VLAN_TRANSPARENT_CTRLm, L2_PORT_VLAN_TRANSPARENT_CTRL_PORTMASKf, &entry, macMask.portbits);
    CAL_MLIST_TO_YTPLIST(unit, macMask, (*pPort_mask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t tagAFT)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(port_vlan_ctrl1n_t), &tmpCtrl), ret);

    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_STAG_AFTf, &tmpCtrl, tagAFT);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_CTAG_AFTf, &tmpCtrl, tagAFT);
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(tmpCtrl), &tmpCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_aft_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_vlan_aft_t *pTagAFT)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, sizeof(port_vlan_ctrl1n_t), 0, sizeof(port_vlan_ctrl1n_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(port_vlan_ctrl1n_t), &tmpCtrl), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_STAG_AFTf, &tmpCtrl, pTagAFT);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_CTAG_AFTf, &tmpCtrl, pTagAFT);
    }

    return CMM_ERR_OK;
}

/*
 * YT_VLAN_TAG_MODE_KEEP_ALL and YT_VLAN_TAG_MODE_KEEP_TAGGED_MODE map to internal 4
 * YT_VLAN_TAG_MODE_ENTRY_BASED maps to internal mode 5
 */
yt_ret_t fal_tiger_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tagVal = tagMode;
    yt_enable_t tagKeepTagOnly = YT_DISABLE;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

	if(tagMode == YT_VLAN_TAG_MODE_KEEP_TAGGED_MODE || tagMode == YT_VLAN_TAG_MODE_ENTRY_BASED)
	{
		tagVal = tagMode - 1;
	}

	/* keep all or keep tag only*/
	if(tagMode == YT_VLAN_TAG_MODE_KEEP_TAGGED_MODE || tagMode == YT_VLAN_TAG_MODE_KEEP_ALL)
	{
		tagKeepTagOnly = tagMode==YT_VLAN_TAG_MODE_KEEP_ALL ? YT_DISABLE : YT_ENABLE;
		CMM_ERR_CHK(fal_tiger_vlan_port_egrTagKeep_set(unit, type, port, tagKeepTagOnly), ret);
	}

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_MODEf, &entry, tagVal);
    }
    else if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_MODEf, &entry, tagVal);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_VLAN_CTRLNm, macid, sizeof(entry), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTagMode_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t *pTagMode)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tagMode = 0;
    yt_enable_t tagKeepTagOnly = YT_DISABLE;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_MODEf, &entry, &tagMode);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_MODEf, &entry, &tagMode);
    }

	*pTagMode = tagMode;

	if(tagMode == YT_VLAN_TAG_MODE_ENTRY_BASED - 1)
	{
		*pTagMode = YT_VLAN_TAG_MODE_ENTRY_BASED;
		return CMM_ERR_OK;
	}

	/* keep all or keep tag only*/
	if(tagMode == YT_VLAN_TAG_MODE_KEEP_ALL)
	{
		CMM_ERR_CHK(fal_tiger_vlan_port_egrTagKeep_get(unit, type, port, &tagKeepTagOnly), ret);
		*pTagMode = tagKeepTagOnly == YT_ENABLE ? YT_VLAN_TAG_MODE_KEEP_TAGGED_MODE : YT_VLAN_TAG_MODE_KEEP_ALL;
	}

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrDefaultVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t default_vid)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, default_vid);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, default_vid);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_VLAN_CTRLNm, macid, sizeof(entry), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrDefaultVid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pDefault_vid)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, pDefault_vid);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, pDefault_vid);
    }

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_vlan_port_egrTagKeep_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_enable_t enable)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_KEEP_MODEf, &entry, enable);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_KEEP_MODEf, &entry, enable);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_VLAN_CTRLNm, macid, sizeof(entry), &entry), ret);

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_vlan_port_egrTagKeep_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_enable_t *pEnable)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_port_vlan_ctrln_t), 0, sizeof(egr_port_vlan_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid, sizeof(entry), &entry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_KEEP_MODEf, &entry, pEnable);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_KEEP_MODEf, &entry, pEnable);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids)
{
    egr_tpid_profile_t tpidEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t i = 0;

    osal_memset(&tpidEntry, sizeof(egr_tpid_profile_t), 0, sizeof(egr_tpid_profile_t));

    for(i=0; i< CAL_VLAN_TPID_PROFILE_NUM; i++)
    {
        HAL_FIELD_SET(EGR_TPID_PROFILEm, EGR_TPID_PROFILE_TPIDf, &tpidEntry, tpids.tpid[i]);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_TPID_PROFILEm, i, sizeof(tpidEntry), &tpidEntry), ret);
    }

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids)
{
    egr_tpid_profile_t tpidEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t i = 0;

    osal_memset(&tpidEntry, sizeof(egr_tpid_profile_t), 0, sizeof(egr_tpid_profile_t));

    for(i=0; i< CAL_VLAN_TPID_PROFILE_NUM; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_TPID_PROFILEm, i, sizeof(tpidEntry), &tpidEntry), ret);
        HAL_FIELD_GET(EGR_TPID_PROFILEm, EGR_TPID_PROFILE_TPIDf, &tpidEntry, &(pTpids->tpid[i]));
    }

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTpidIdx_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx)
{
    egr_port_ctrln_t portEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&portEntry, sizeof(egr_port_ctrln_t), 0, sizeof(egr_port_ctrln_t));

    if(type > YT_VLAN_TYPE_SVLAN || tpidIdx >= CAL_VLAN_TPID_PROFILE_NUM)
    {
        return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macid, sizeof(portEntry), &portEntry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CTAG_TPID_SELf, &portEntry, tpidIdx);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_STAG_TPID_SELf, &portEntry, tpidIdx);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_CTRLNm, macid, sizeof(portEntry), &portEntry), ret);

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTpidIdx_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx)
{
    egr_port_ctrln_t portEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&portEntry, sizeof(egr_port_ctrln_t), 0, sizeof(egr_port_ctrln_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macid ,sizeof(portEntry), &portEntry), ret);
    if(YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CTAG_TPID_SELf, &portEntry, pTpidIdx);
    }
    else  if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_STAG_TPID_SELf, &portEntry, pTpidIdx);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t port_mask)
{
    egr_vlan_tag_transparent_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_vlan_tag_transparent_ctrl_t), 0, sizeof(egr_vlan_tag_transparent_ctrl_t));

    CAL_YTPLIST_TO_MLIST(unit, port_mask, macmask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TAG_TRANSPARENT_CTRLm, macid, sizeof(egr_vlan_tag_transparent_ctrl_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_TRANSPARENT_ENf, &entry, enable);
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits[0]);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_CTAG_TRANSPARENT_ENf, &entry, enable);
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_CTAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits[0]);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_VLAN_TAG_TRANSPARENT_CTRLm, macid, sizeof(egr_vlan_tag_transparent_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTransparent_get(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t *pEnable, yt_port_mask_t *pPort_mask)
{
    egr_vlan_tag_transparent_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(egr_vlan_tag_transparent_ctrl_t), 0, sizeof(egr_vlan_tag_transparent_ctrl_t));

    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TAG_TRANSPARENT_CTRLm, macid, sizeof(egr_vlan_tag_transparent_ctrl_t), &entry), ret);
    if(YT_VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_TRANSPARENT_ENf, &entry, pEnable);
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits);
    }
    else if (YT_VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_CTAG_TRANSPARENT_ENf, &entry, pEnable);
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_CTAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits);
    }
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pPort_mask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_egrFilter_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enabled)
{
    l2_egr_vlan_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(l2_egr_vlan_filter_en_t), 0, sizeof(l2_egr_vlan_filter_en_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_EGR_VLAN_FILTER_ENm, 0,sizeof(l2_egr_vlan_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_EGR_VLAN_FILTER_ENm, L2_EGR_VLAN_FILTER_EN_FILTER_ENf, &entry, &macmask);
    if(enabled)
    {
        SET_BIT(macmask, macid);
    }
    else
    {
        CLEAR_BIT(macmask, macid);
    }
    HAL_FIELD_SET(L2_EGR_VLAN_FILTER_ENm, L2_EGR_VLAN_FILTER_EN_FILTER_ENf, &entry, macmask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_EGR_VLAN_FILTER_ENm, 0, sizeof(l2_egr_vlan_filter_en_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_egrFilter_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnabled)
{
    l2_egr_vlan_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(l2_egr_vlan_filter_en_t), 0, sizeof(l2_egr_vlan_filter_en_t));

    *pEnabled = YT_DISABLE;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_EGR_VLAN_FILTER_ENm, 0,sizeof(l2_egr_vlan_filter_en_t), &entry), ret);
    HAL_FIELD_GET(L2_EGR_VLAN_FILTER_ENm, L2_EGR_VLAN_FILTER_EN_FILTER_ENf, &entry, &macmask);
    if (IS_BIT_SET(macmask, macid))
    {
        *pEnabled = YT_ENABLE;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_port_vidTypeSel_set(yt_unit_t unit, yt_port_t port, yt_vlan_type_t mode)
{
    look_up_vlan_sel_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(look_up_vlan_sel_t), 0, sizeof(look_up_vlan_sel_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOK_UP_VLAN_SELm, 0, sizeof(look_up_vlan_sel_t), &entry), ret);
    HAL_FIELD_GET(LOOK_UP_VLAN_SELm, LOOK_UP_VLAN_SEL_LOOK_UP_VLAN_SELf, &entry, &macmask);
    if(YT_VLAN_TYPE_SVLAN == mode )
    {
        SET_BIT(macmask, macid);
    }
    else
    {
        CLEAR_BIT(macmask, macid);
    }
    HAL_FIELD_SET(LOOK_UP_VLAN_SELm, LOOK_UP_VLAN_SEL_LOOK_UP_VLAN_SELf, &entry, macmask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOK_UP_VLAN_SELm, 0, sizeof(look_up_vlan_sel_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_port_vidTypeSel_get(yt_unit_t unit, yt_port_t port, yt_vlan_type_t *pMode)
{
    look_up_vlan_sel_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&entry, sizeof(look_up_vlan_sel_t), 0, sizeof(look_up_vlan_sel_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOK_UP_VLAN_SELm, 0, sizeof(look_up_vlan_sel_t), &entry), ret);
    HAL_FIELD_GET(LOOK_UP_VLAN_SELm, LOOK_UP_VLAN_SEL_LOOK_UP_VLAN_SELf, &entry, &macmask);
    if (IS_BIT_SET(macmask, macid))
    {
        *pMode = YT_VLAN_TYPE_SVLAN;
    }
    else
    {
        *pMode = YT_VLAN_TYPE_CVLAN;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_protocolBasedVlan_group_set(yt_unit_t unit, uint8_t groupId, const yt_vlan_protocol_key_t *pKey)
{
    protocol_based_vlann_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t l2TypeTmp = 0;
	
    osal_memset(&entry, sizeof(protocol_based_vlann_t), 0, sizeof(protocol_based_vlann_t));

    if (CAL_SWCHIP_ID(unit) != YT_SW_ID_9218)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    if (pKey->l2Type == L2_ETHV2)
    {
        l2TypeTmp = L2_TYPE_BITMAP_ETHV2;
    }
    else if (pKey->l2Type == L2_ETHSAP)
    {
        l2TypeTmp = L2_TYPE_BITMAP_ETHSAP;
    }
    else if (pKey->l2Type == L2_ETHSNAP)
    {
        l2TypeTmp = L2_TYPE_BITMAP_ETHSNAP;
    }
    else if (pKey->l2Type >= L2_ETHMAX)
    {
        return CMM_ERR_EXCEED_RANGE;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PROTOCOL_BASED_VLANNm, groupId, sizeof(protocol_based_vlann_t), &entry), ret);
   
    HAL_FIELD_SET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_L2_TYPE_BITMAPf, &entry, l2TypeTmp);
    HAL_FIELD_SET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_ETH_TYPEf, &entry, pKey->ethType);
    HAL_FIELD_SET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_VALIDf, &entry, pKey->keyEn);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PROTOCOL_BASED_VLANNm, groupId, sizeof(port_vlan_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_protocolBasedVlan_group_get(yt_unit_t unit, uint8_t groupId, yt_vlan_protocol_key_t *pKey)
{
    protocol_based_vlann_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t l2TypeTmp = 0;

    osal_memset(&entry, sizeof(protocol_based_vlann_t), 0, sizeof(protocol_based_vlann_t));

    if (CAL_SWCHIP_ID(unit) != YT_SW_ID_9218)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PROTOCOL_BASED_VLANNm, groupId, sizeof(protocol_based_vlann_t), &entry), ret);
   
    HAL_FIELD_GET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_L2_TYPE_BITMAPf, &entry, &l2TypeTmp);
    HAL_FIELD_GET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_ETH_TYPEf, &entry, &(pKey->ethType));
    HAL_FIELD_GET(PROTOCOL_BASED_VLANNm, PROTOCOL_BASED_VLANN_VALIDf, &entry, &(pKey->keyEn));

    if (l2TypeTmp == L2_TYPE_BITMAP_ETHV2)
    {
        pKey->l2Type = L2_ETHV2;
    }
    else if (l2TypeTmp == L2_TYPE_BITMAP_ETHSAP)
    {
        pKey->l2Type = L2_ETHSAP;
    }
    else if (l2TypeTmp == L2_TYPE_BITMAP_ETHSNAP)
    {
        pKey->l2Type = L2_ETHSNAP;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_add(yt_unit_t unit, yt_port_t port, uint8_t groupId, const yt_vlan_protocol_action_t *pAction)
{
    protocol_based_vlan_data_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint32_t protoNum = YT_PROTOCOL_BASED_VLAN_NUM(unit);
    uint32_t proto_idx = 0;

    osal_memset(&entry, sizeof(protocol_based_vlan_data_tbl_t), 0, sizeof(protocol_based_vlan_data_tbl_t));

    if (CAL_SWCHIP_ID(unit) != YT_SW_ID_9218)
    {
        return CMM_ERR_NOT_SUPPORT;
    }
	
    proto_idx = macid*protoNum + groupId;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PROTOCOL_BASED_VLAN_DATA_TBLm, proto_idx, sizeof(protocol_based_vlan_data_tbl_t), &entry), ret);
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVIDf, &entry, pAction->newSvid); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVID_CMDf, &entry, pAction->svidAct); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVIDf, &entry, pAction->newCvid); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVID_CMDf, &entry, pAction->cvidAct); 
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PROTOCOL_BASED_VLAN_DATA_TBLm, proto_idx, sizeof(protocol_based_vlan_data_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_get(yt_unit_t unit, yt_port_t port, uint8_t  groupId, yt_vlan_protocol_action_t *pAction)
{
    protocol_based_vlan_data_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint32_t protoNum = YT_PROTOCOL_BASED_VLAN_NUM(unit);
    uint32_t proto_idx = 0;

    osal_memset(&entry, sizeof(protocol_based_vlan_data_tbl_t), 0, sizeof(protocol_based_vlan_data_tbl_t));

    if (CAL_SWCHIP_ID(unit) != YT_SW_ID_9218)
    {
        return CMM_ERR_NOT_SUPPORT;
    }
	
    proto_idx = macid*protoNum + groupId;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PROTOCOL_BASED_VLAN_DATA_TBLm, proto_idx, sizeof(protocol_based_vlan_data_tbl_t), &entry), ret);
    HAL_FIELD_GET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVIDf, &entry, &(pAction->newSvid));
    HAL_FIELD_GET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVID_CMDf, &entry, &(pAction->svidAct));
    HAL_FIELD_GET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVIDf, &entry, &(pAction->newCvid));
    HAL_FIELD_GET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVID_CMDf, &entry, &(pAction->cvidAct));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_protocolBasedVlan_table_del(yt_unit_t unit, yt_port_t port, uint8_t  groupId)
{
    protocol_based_vlan_data_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint32_t protoNum = YT_PROTOCOL_BASED_VLAN_NUM(unit);
    uint32_t proto_idx = 0;
    yt_vlan_protocol_action_t action;

    osal_memset(&entry, sizeof(protocol_based_vlan_data_tbl_t), 0, sizeof(protocol_based_vlan_data_tbl_t));
    osal_memset(&action, sizeof(yt_vlan_protocol_action_t), 0, sizeof(yt_vlan_protocol_action_t));

    if (CAL_SWCHIP_ID(unit) != YT_SW_ID_9218)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    proto_idx = macid*protoNum + groupId;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PROTOCOL_BASED_VLAN_DATA_TBLm, proto_idx, sizeof(protocol_based_vlan_data_tbl_t), &entry), ret);
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVIDf, &entry, action.newSvid); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_SVID_CMDf, &entry, action.svidAct); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVIDf, &entry, action.newCvid); 
    HAL_FIELD_SET(PROTOCOL_BASED_VLAN_DATA_TBLm, PROTOCOL_BASED_VLAN_DATA_TBL_CVID_CMDf, &entry, action.cvidAct); 
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PROTOCOL_BASED_VLAN_DATA_TBLm, proto_idx, sizeof(protocol_based_vlan_data_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_mac_ipSub_tbl_arrange_set(yt_unit_t unit, uint8_t macNum, uint8_t ipSubNum, yt_vlan_mac_ip_subnet_pri_t  pri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(macNum);
    CMM_UNUSED_PARAM(ipSubNum);
    CMM_UNUSED_PARAM(pri);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_mac_ipSub_tbl_arrange_get(yt_unit_t unit, uint8_t *pMacNum, uint8_t *pIpSubNum, yt_vlan_mac_ip_subnet_pri_t  *pPri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pMacNum);
    CMM_UNUSED_PARAM(pIpSubNum);
    CMM_UNUSED_PARAM(pPri);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_macBasedVlan_table_add(yt_unit_t unit, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction, yt_mac_vlan_table_idx_t *pTableIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pMacVlanRule);
    CMM_UNUSED_PARAM(pMacVlanAction);
    CMM_UNUSED_PARAM(pTableIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_macBasedVlan_table_add_by_index(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);
    CMM_UNUSED_PARAM(pMacVlanRule);
    CMM_UNUSED_PARAM(pMacVlanAction);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_macBasedVlan_table_get(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, yt_mac_vlan_rule_t *pMacVlanRule, yt_mac_vlan_action_t *pMacVlanAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);
    CMM_UNUSED_PARAM(pMacVlanRule);
    CMM_UNUSED_PARAM(pMacVlanAction);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_macBasedVlan_table_del(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_add(yt_unit_t unit, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction, yt_ip_subnet_vlan_table_idx_t *pTableIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pIpSubnetVlanRule);
    CMM_UNUSED_PARAM(pIpSubnetVlanAction);
    CMM_UNUSED_PARAM(pTableIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_add_by_index(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);
    CMM_UNUSED_PARAM(pIpSubnetVlanRule);
    CMM_UNUSED_PARAM(pIpSubnetVlanAction);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_get(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);
    CMM_UNUSED_PARAM(pIpSubnetVlanRule);
    CMM_UNUSED_PARAM(pIpSubnetVlanAction);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_table_del(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(tableIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(enable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_ipSubnetBasedVlan_bypass_arp_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pEnable);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_idx_mapping_set(yt_unit_t unit, yt_vlan_t vid,  uint8_t vlanPolicyIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(vlanPolicyIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_idx_mapping_get(yt_unit_t unit, yt_vlan_t vid,  uint8_t *pVlanPolicyIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(pVlanPolicyIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_table_add(yt_unit_t unit, const yt_vlan_policy_info_t *pVlanPolicyInfo,  uint8_t *pVlanPolicyIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pVlanPolicyInfo);
    CMM_UNUSED_PARAM(pVlanPolicyIdx);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_table_add_by_index(yt_unit_t unit, uint8_t vlanPolicyIdx, const yt_vlan_policy_info_t *pVlanPolicyInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vlanPolicyIdx);
    CMM_UNUSED_PARAM(pVlanPolicyInfo);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_table_get(yt_unit_t unit, uint8_t vlanPolicyIdx, yt_vlan_policy_info_t *pVlanPolicyInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vlanPolicyIdx);
    CMM_UNUSED_PARAM(pVlanPolicyInfo);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_vlan_policy_table_del(yt_unit_t unit, uint8_t vlanPolicyIdx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vlanPolicyIdx);

    return CMM_ERR_NOT_SUPPORT;
}
