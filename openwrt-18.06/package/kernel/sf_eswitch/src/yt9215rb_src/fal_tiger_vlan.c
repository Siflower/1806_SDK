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
    yt_port_t port;

    fal_tiger_vlan_port_set(unit, 1, YT_MAC_ALL_PORT_MASK(unit), YT_MAC_ALL_PORT_MASK(unit));

    tpids.tpid[0] = 0x8100;
    tpids.tpid[1] = 0x88a8;
    fal_tiger_vlan_egrTpid_set(unit, tpids);
    for(port = 0; port < YT_PORT_NUM; port++)
    {
        fal_tiger_vlan_port_egrTpidIdx_set(unit, VLAN_TYPE_CVLAN, port, 0);
        fal_tiger_vlan_port_egrTpidIdx_set(unit, VLAN_TYPE_SVLAN, port, 1);
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_set(yt_unit_t unit,  yt_vlan_t vid,  yt_port_mask_t  member_portmask, yt_port_mask_t  untag_portmask)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_port_mask_t  utagmacmask;

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
    yt_port_mask_t mac_mask;
    yt_port_mask_t utagmacmask;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_PORT_MEMBER_BITMAPf, &entry, mac_mask.portbits);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_UNTAG_MEMBER_BITMAPf, &entry, utagmacmask.portbits);

    CAL_MLIST_TO_YTPLIST(unit, mac_mask, (*pMember_portmask));
    CAL_MLIST_TO_YTPLIST(unit, utagmacmask, (*pUntag_portmask));

    return 0;
}

yt_ret_t  fal_tiger_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t enable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_SVL_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid,  yt_enable_t *pEnable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_SVL_ENf, &entry, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid,  yt_fid_t fid)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

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
    yt_fid_t fid0 = 0,fid1 = 0;

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
    parser_port_ctrln_t parser_port_ctrln;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    if(type > VLAN_TYPE_SVLAN ||
        tpidIdxMask >= (1<<CAL_VLAN_TPID_PROFILE_NUM))
    {
            return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PARSER_PORT_CTRLNm, macid, sizeof(parser_port_ctrln_t), &parser_port_ctrln), ret);

    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_STAG_TPID_MASKf, &parser_port_ctrln, (tpidIdxMask & 0xF));
    }
    else if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_CTAG_TPID_MASKf, &parser_port_ctrln, (tpidIdxMask & 0xF));
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PARSER_PORT_CTRLNm, macid, sizeof(parser_port_ctrln), &parser_port_ctrln), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrTpidMask_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask)
{
    parser_port_ctrln_t parser_port_ctrln;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PARSER_PORT_CTRLNm, macid, sizeof(parser_port_ctrln_t), &parser_port_ctrln), ret);

    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_STAG_TPID_MASKf, &parser_port_ctrln, pTpidIdxMask);
    }
    else if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PARSER_PORT_CTRLNm, PARSER_PORT_CTRLN_CTAG_TPID_MASKf, &parser_port_ctrln, pTpidIdxMask);
    }


    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrDefaultVlan_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t vid)
{
    port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, vid);
    }
    else if (VLAN_TYPE_CVLAN == type)
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

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRLNm, macid, sizeof(port_vlan_ctrln_t), &entry), ret);
    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_SVIDf, &entry, pVid);
    }
    else if (VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRLNm, PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, pVid);
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_ingrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enabled)
{
    l2_vlan_ingress_filter_en_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint16_t macmask;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

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
    yt_port_mask_t mac_mask;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_VLAN_TRANSPARENT_CTRLm, macid, sizeof(l2_port_vlan_transparent_ctrl_t), &entry), ret);
    HAL_FIELD_GET(L2_PORT_VLAN_TRANSPARENT_CTRLm, L2_PORT_VLAN_TRANSPARENT_CTRL_PORTMASKf, &entry, mac_mask.portbits);
    CAL_MLIST_TO_YTPLIST(unit, mac_mask, (*pPort_mask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t tagAFT)
{
    port_vlan_ctrl1n_t tmpCtrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    osal_memset(&tmpCtrl, 0, sizeof(tmpCtrl));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(port_vlan_ctrl1n_t), &tmpCtrl), ret);

    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_STAG_AFTf, &tmpCtrl, tagAFT);
    }
    else if (VLAN_TYPE_CVLAN == type)
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

    osal_memset(&tmpCtrl, 0, sizeof(tmpCtrl));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_VLAN_CTRL1Nm, macid, sizeof(port_vlan_ctrl1n_t), &tmpCtrl), ret);
    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_STAG_AFTf, &tmpCtrl, pTagAFT);
    }
    else if (VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(PORT_VLAN_CTRL1Nm, PORT_VLAN_CTRL1N_CTAG_AFTf, &tmpCtrl, pTagAFT);
    }

    return CMM_ERR_OK;
}

/*
 * VLAN_TAG_MODE_KEEP_ALL and VLAN_TAG_MODE_KEEP_TAGGED_MODE map to internal 4
 * VLAN_TAG_MODE_ENTRY_BASED maps to internal mode 5
 */
yt_ret_t fal_tiger_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
	uint8_t tagVal = tagMode;
	yt_enable_t tagKeepTagOnly = YT_DISABLE;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

	if(tagMode == VLAN_TAG_MODE_KEEP_TAGGED_MODE || tagMode == VLAN_TAG_MODE_ENTRY_BASED)
	{
		tagVal = tagMode - 1;
	}

	/* keep all or keep tag only*/
	if(tagMode == VLAN_TAG_MODE_KEEP_TAGGED_MODE || tagMode == VLAN_TAG_MODE_KEEP_ALL)
	{
		tagKeepTagOnly = tagMode==VLAN_TAG_MODE_KEEP_ALL ? YT_DISABLE : YT_ENABLE;
		CMM_ERR_CHK(fal_tiger_vlan_port_egrTagKeep_set(unit, type, port, tagKeepTagOnly), ret);
	}

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_MODEf, &entry, tagVal);
    }
    else if(VLAN_TYPE_SVLAN == type)
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


    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_MODEf, &entry, &tagMode);
    }
    else  if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_MODEf, &entry, &tagMode);
    }

	*pTagMode = tagMode;

	if(tagMode == VLAN_TAG_MODE_ENTRY_BASED - 1)
	{
		*pTagMode = VLAN_TAG_MODE_ENTRY_BASED;
		return CMM_ERR_OK;
	}

	/* keep all or keep tag only*/
	if(tagMode == VLAN_TAG_MODE_KEEP_ALL)
	{
		CMM_ERR_CHK(fal_tiger_vlan_port_egrTagKeep_get(unit, type, port, &tagKeepTagOnly), ret);
		*pTagMode = tagKeepTagOnly == YT_ENABLE ? VLAN_TAG_MODE_KEEP_TAGGED_MODE : VLAN_TAG_MODE_KEEP_ALL;
	}

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrDefaultVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t default_vid)
{
    egr_port_vlan_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, default_vid);
    }
    else  if(VLAN_TYPE_SVLAN == type)
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

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_DEFAULT_CVIDf, &entry, pDefault_vid);
    }
    else  if(VLAN_TYPE_SVLAN == type)
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

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid ,sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_KEEP_MODEf, &entry, enable);
    }
    else  if(VLAN_TYPE_SVLAN == type)
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

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_VLAN_CTRLNm, macid, sizeof(entry), &entry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_CTAG_KEEP_MODEf, &entry, pEnable);
    }
    else  if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_VLAN_CTRLNm, EGR_PORT_VLAN_CTRLN_STAG_KEEP_MODEf, &entry, pEnable);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids)
{
    egr_tpid_profile_t tpid_entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t i = 0;

    for(i=0; i< CAL_VLAN_TPID_PROFILE_NUM; i++)
    {
        HAL_FIELD_SET(EGR_TPID_PROFILEm, EGR_TPID_PROFILE_TPIDf, &tpid_entry, tpids.tpid[i]);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_TPID_PROFILEm, i, sizeof(tpid_entry), &tpid_entry), ret);
    }

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids)
{
    egr_tpid_profile_t tpid_entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t i = 0;

    for(i=0; i< CAL_VLAN_TPID_PROFILE_NUM; i++)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_TPID_PROFILEm, i, sizeof(tpid_entry), &tpid_entry), ret);
        HAL_FIELD_GET(EGR_TPID_PROFILEm, EGR_TPID_PROFILE_TPIDf, &tpid_entry, &(pTpids->tpid[i]));
    }

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTpidIdx_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx)
{
    egr_port_ctrln_t port_ntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    if(type > VLAN_TYPE_SVLAN || tpidIdx >= CAL_VLAN_TPID_PROFILE_NUM)
    {
            return CMM_ERR_INPUT;
    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macid, sizeof(port_ntry), &port_ntry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CTAG_TPID_SELf, &port_ntry, tpidIdx);
    }
    else  if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_STAG_TPID_SELf, &port_ntry, tpidIdx);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_CTRLNm, macid, sizeof(port_ntry), &port_ntry), ret);

     return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTpidIdx_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx)
{
    egr_port_ctrln_t port_ntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macid ,sizeof(port_ntry), &port_ntry), ret);
    if(VLAN_TYPE_CVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CTAG_TPID_SELf, &port_ntry, pTpidIdx);
    }
    else  if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_STAG_TPID_SELf, &port_ntry, pTpidIdx);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t port_mask)
{
    egr_vlan_tag_transparent_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_port_mask_t  macmask;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CAL_YTPLIST_TO_MLIST(unit, port_mask, macmask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TAG_TRANSPARENT_CTRLm, macid, sizeof(egr_vlan_tag_transparent_ctrl_t), &entry), ret);
    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_TRANSPARENT_ENf, &entry, enable);
        HAL_FIELD_SET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits[0]);
    }
    else if (VLAN_TYPE_CVLAN == type)
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

    CMM_CLEAR_MEMBER_PORT(macmask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_VLAN_TAG_TRANSPARENT_CTRLm, macid, sizeof(egr_vlan_tag_transparent_ctrl_t), &entry), ret);
    if(VLAN_TYPE_SVLAN == type)
    {
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_TRANSPARENT_ENf, &entry, pEnable);
        HAL_FIELD_GET(EGR_VLAN_TAG_TRANSPARENT_CTRLm, EGR_VLAN_TAG_TRANSPARENT_CTRL_STAG_KEEP_SRCPORT_MASKf, &entry, macmask.portbits);
    }
    else if (VLAN_TYPE_CVLAN == type)
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

    osal_memset(&entry, 0, sizeof(entry));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOK_UP_VLAN_SELm, 0, sizeof(look_up_vlan_sel_t), &entry), ret);
    HAL_FIELD_GET(LOOK_UP_VLAN_SELm, LOOK_UP_VLAN_SEL_LOOK_UP_VLAN_SELf, &entry, &macmask);
    if(VLAN_TYPE_SVLAN == mode )
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

    osal_memset(&entry, 0, sizeof(entry));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOK_UP_VLAN_SELm, 0, sizeof(look_up_vlan_sel_t), &entry), ret);
    HAL_FIELD_GET(LOOK_UP_VLAN_SELm, LOOK_UP_VLAN_SEL_LOOK_UP_VLAN_SELf, &entry, &macmask);
    if (IS_BIT_SET(macmask, macid))
    {
        *pMode = VLAN_TYPE_SVLAN;
    }
    else
    {
        *pMode = VLAN_TYPE_CVLAN;
    }

    return CMM_ERR_OK;
}
