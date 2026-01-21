/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_vlan.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_vlan.h"
#include "fal_dispatch.h"

yt_ret_t  yt_vlan_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_init(unit);
}

yt_ret_t  yt_vlan_port_set(yt_unit_t unit, yt_vlan_t vid, yt_port_mask_t memberPortmask, yt_port_mask_t untagPortmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, memberPortmask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, untagPortmask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_set(unit, vid, memberPortmask, untagPortmask);
}

yt_ret_t  yt_vlan_port_get(yt_unit_t unit, yt_vlan_t vid, yt_port_mask_t *pMemberPortmask, yt_port_mask_t *pUntagPortmask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pMemberPortmask), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pUntagPortmask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_get(unit, vid, pMemberPortmask, pUntagPortmask);
}

yt_ret_t  yt_vlan_svlMode_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_svlMode_enable_set(unit, vid, enable);
}

yt_ret_t  yt_vlan_svlMode_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_svlMode_enable_get(unit, vid, pEnable);
}

yt_ret_t  yt_vlan_fid_set(yt_unit_t unit, yt_vlan_t vid, yt_fid_t fid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < fid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_fid_set(unit, vid, fid);
}

yt_ret_t  yt_vlan_fid_get(yt_unit_t unit, yt_vlan_t vid, yt_fid_t *pFid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pFid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_fid_get(unit, vid, pFid);
}

yt_ret_t  yt_vlan_igrTpid_set(yt_unit_t unit, yt_tpid_profiles_t tpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_igrTpid_set(unit, tpid);
}

yt_ret_t  yt_vlan_igrTpid_get(yt_unit_t unit, yt_tpid_profiles_t *pTpid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igrTpid_get(unit, pTpid);
}

yt_ret_t  yt_vlan_port_igrTpidSel_set(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_tpidprofile_id_mask_t tpidIdxMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_igrTpidSel_set(unit, type, port, tpidIdxMask);
}

yt_ret_t  yt_vlan_port_igrTpidSel_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_tpidprofile_id_mask_t *pTpidIdxMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTpidIdxMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrTpidSel_get(unit, type, port, pTpidIdxMask);
}

yt_ret_t  yt_vlan_port_igrPvid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_t vid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < vid || YT_VLAN_ID_MIN == vid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_igrPvid_set(unit, type, port, vid);
}

yt_ret_t  yt_vlan_port_igrPvid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pVid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pVid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrPvid_get(unit, type, port, pVid);
}

yt_ret_t  yt_vlan_port_igrDefPri_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pri), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_igrDefPri_set(unit, type, port, pri);
}

yt_ret_t  yt_vlan_port_igrDefPri_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, uint8_t *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrDefPri_get(unit, type, port, pPri);
}

yt_ret_t  yt_vlan_port_igrFilter_enable_set(yt_unit_t unit, yt_port_t  port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_igrFilter_enable_set(unit, port, enable);
}

yt_ret_t  yt_vlan_port_igrFilter_enable_get(yt_unit_t unit, yt_port_t  port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_igrFilter_enable_get(unit, port, pEnable);
}

yt_ret_t  yt_vlan_igrTransparent_set(yt_unit_t unit, yt_port_t  port, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->vlan_igrTransparent_set(unit, port, portMask);
}

yt_ret_t  yt_vlan_igrTransparent_get(yt_unit_t unit, yt_port_t  port, yt_port_mask_t *pPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_igrTransparent_get(unit, port, pPortMask);
}

yt_ret_t  yt_vlan_port_aft_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, yt_vlan_aft_t aft)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_AFT_NONE < aft || YT_VLAN_AFT_ALL > aft), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_aft_set(unit, type, port, aft);
}

yt_ret_t  yt_vlan_port_aft_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, yt_vlan_aft_t *pAft)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pAft), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_aft_get(unit, type, port, pAft);
}

yt_ret_t yt_vlan_port_egrTagMode_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t tagMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_VLAN_TAG_MODE_ENTRY_BASED < tagMode || YT_VLAN_TAG_MODE_UNTAGGED > tagMode), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTagMode_set(unit, type, port, tagMode);
}

yt_ret_t yt_vlan_port_egrTagMode_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port,  yt_egr_tag_mode_t *pTagMode)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTagMode), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTagMode_get(unit, type, port, pTagMode);
}

yt_ret_t yt_vlan_port_egrDefVid_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t defaultVid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < defaultVid) || (YT_VLAN_ID_MIN == defaultVid)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_port_egrDefVid_set(unit, type, port, defaultVid);
}

yt_ret_t yt_vlan_port_egrDefVid_get(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t  port, yt_vlan_t *pDefaultVid)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pDefaultVid), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrDefVid_get(unit, type, port, pDefaultVid);
}

yt_ret_t yt_vlan_egrTpid_set(yt_unit_t unit,  yt_tpid_profiles_t tpids)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->vlan_egrTpid_set(unit, tpids);
}

yt_ret_t yt_vlan_egrTpid_get(yt_unit_t unit,  yt_tpid_profiles_t *pTpids)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pTpids), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_egrTpid_get(unit, pTpids);
}

yt_ret_t yt_vlan_port_egrTpidSel_set(yt_unit_t unit, yt_vlan_type_t  type, yt_port_t port, uint8_t tpidIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_TPID_PROFILE_NUM <= tpidIdx), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTpidSel_set(unit, type, port, tpidIdx);
}

yt_ret_t yt_vlan_port_egrTpidSel_get(yt_unit_t unit, yt_vlan_type_t type, yt_port_t port, uint8_t *pTpidIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pTpidIdx), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTpidSel_get(unit, type, port, pTpidIdx);
}

yt_ret_t yt_vlan_port_egrTransparent_set(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t enable, yt_port_mask_t portMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, portMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrTransparent_set(unit, type, port, enable, portMask);
}

yt_ret_t yt_vlan_port_egrTransparent_get(yt_unit_t unit, yt_vlan_type_t type,  yt_port_t port,  yt_enable_t *pEnable, yt_port_mask_t *pPortMask)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrTransparent_get(unit, type, port, pEnable, pPortMask);
}

yt_ret_t  yt_vlan_port_egrFilter_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_egrFilter_enable_set(unit, port, enable);
}

yt_ret_t  yt_vlan_port_egrFilter_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_egrFilter_enable_get(unit, port, pEnable);
}

yt_ret_t  yt_vlan_port_vidTypeSel_set(yt_unit_t unit, yt_port_t port, yt_vlan_type_t type)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_VLAN_TYPE_SVLAN < type || YT_VLAN_TYPE_CVLAN > type), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_port_vidTypeSel_set(unit, port, type);
}

yt_ret_t  yt_vlan_port_vidTypeSel_get(yt_unit_t unit, yt_port_t port, yt_vlan_type_t *pType)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit,port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((NULL == pType), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_port_vidTypeSel_get(unit, port, pType);
}

yt_ret_t  yt_vlan_protocolBasedVlan_group_set(yt_unit_t unit, uint8_t groupId, const yt_vlan_protocol_key_t *pKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_PROTOCOL_BASED_VLAN_NUM(unit) <= groupId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((L2_ETHMAX <= pKey->l2Type), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_DISABLE > pKey->keyEn || YT_ENABLE < pKey->keyEn), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_protocolBasedVlan_group_set(unit, groupId, pKey);
}

yt_ret_t  yt_vlan_protocolBasedVlan_group_get(yt_unit_t unit, uint8_t groupId, yt_vlan_protocol_key_t *pKey)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_PROTOCOL_BASED_VLAN_NUM(unit) <= groupId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pKey), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_protocolBasedVlan_group_get(unit, groupId, pKey);
}

yt_ret_t  yt_vlan_protocolBasedVlan_table_add(yt_unit_t unit, yt_port_t port, uint8_t groupId, const yt_vlan_protocol_action_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_PROTOCOL_BASED_VLAN_NUM(unit) <= groupId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pAction->svidAct || YT_VLAN_ACTION_NO_CHANGE > pAction->svidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pAction->cvidAct || YT_VLAN_ACTION_NO_CHANGE > pAction->cvidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->newSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pAction->newCvid), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_protocolBasedVlan_table_add(unit, port, groupId, pAction);
}

yt_ret_t  yt_vlan_protocolBasedVlan_table_get(yt_unit_t unit, yt_port_t port, uint8_t  groupId, yt_vlan_protocol_action_t *pAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_PROTOCOL_BASED_VLAN_NUM(unit) <= groupId), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_protocolBasedVlan_table_get(unit, port, groupId, pAction);
}

yt_ret_t  yt_vlan_protocolBasedVlan_table_del(yt_unit_t unit, yt_port_t port, uint8_t  groupId)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_PROTOCOL_BASED_VLAN_NUM(unit) <= groupId), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_protocolBasedVlan_table_del(unit, port, groupId);
}

yt_ret_t  yt_vlan_mac_ipSub_tbl_arrange_set(yt_unit_t unit, uint8_t macNum, uint8_t ipSubNum, yt_vlan_mac_ip_subnet_pri_t  pri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < (macNum + ipSubNum)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TYPE_MAC_VLAN_FIRST != pri && YT_VLAN_TYPE_IPSUB_VLAN_FIRST != pri), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_mac_ipSub_tbl_arrange_set(unit, macNum, ipSubNum, pri);
}

yt_ret_t  yt_vlan_mac_ipSub_tbl_arrange_get(yt_unit_t unit, uint8_t *pMacNum, uint8_t *pIpSubNum, yt_vlan_mac_ip_subnet_pri_t  *pPri)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMacNum), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pIpSubNum), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pPri), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_mac_ipSub_tbl_arrange_get(unit, pMacNum, pIpSubNum, pPri);
}

yt_ret_t  yt_vlan_macBasedVlan_table_add(yt_unit_t unit, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction, yt_mac_vlan_table_idx_t *pTableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMacVlanRule), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMacVlanAction), CMM_ERR_NULL_POINT);
    if(!pMacVlanRule->isLagId)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pMacVlanRule->port))), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_VLAN_MAC_VLAN_MAX_PREFIX_LEN < pMacVlanRule->saPrefix), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pMacVlanRule->ctagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pMacVlanRule->stagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < pMacVlanAction->fwdAct || ACT_TYPE_FWD > pMacVlanAction->fwdAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pMacVlanAction->spriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pMacVlanAction->spriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pMacVlanAction->cpriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pMacVlanAction->cpriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pMacVlanAction->svidAct || YT_VLAN_ACTION_NO_CHANGE > pMacVlanAction->svidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pMacVlanAction->cvidAct || YT_VLAN_ACTION_NO_CHANGE > pMacVlanAction->cvidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pMacVlanAction->newSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pMacVlanAction->newCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pMacVlanAction->newSpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pMacVlanAction->newCpri), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_macBasedVlan_table_add(unit, pMacVlanRule, pMacVlanAction, pTableIdx);
}

yt_ret_t  yt_vlan_macBasedVlan_table_add_by_index(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, const yt_mac_vlan_rule_t *pMacVlanRule, const yt_mac_vlan_action_t *pMacVlanAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pMacVlanRule), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMacVlanAction), CMM_ERR_NULL_POINT);
    if(!pMacVlanRule->isLagId)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pMacVlanRule->port))), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_VLAN_MAC_VLAN_MAX_PREFIX_LEN < pMacVlanRule->saPrefix), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pMacVlanRule->ctagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pMacVlanRule->stagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < pMacVlanAction->fwdAct || ACT_TYPE_FWD > pMacVlanAction->fwdAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pMacVlanAction->spriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pMacVlanAction->spriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pMacVlanAction->cpriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pMacVlanAction->cpriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pMacVlanAction->svidAct || YT_VLAN_ACTION_NO_CHANGE > pMacVlanAction->svidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pMacVlanAction->cvidAct || YT_VLAN_ACTION_NO_CHANGE > pMacVlanAction->cvidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pMacVlanAction->newSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pMacVlanAction->newCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pMacVlanAction->newSpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pMacVlanAction->newCpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_macBasedVlan_table_add_by_index(unit, tableIdx, pMacVlanRule, pMacVlanAction);
}

yt_ret_t  yt_vlan_macBasedVlan_table_get(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx, yt_mac_vlan_rule_t *pMacVlanRule, yt_mac_vlan_action_t *pMacVlanAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pMacVlanRule), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMacVlanAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_macBasedVlan_table_get(unit, tableIdx, pMacVlanRule, pMacVlanAction);
}

yt_ret_t  yt_vlan_macBasedVlan_table_del(yt_unit_t unit, yt_mac_vlan_table_idx_t tableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_macBasedVlan_table_del(unit, tableIdx);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_add(yt_unit_t unit, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction, yt_ip_subnet_vlan_table_idx_t *pTableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(!pIpSubnetVlanRule->isLagId)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pIpSubnetVlanRule->port))), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_VLAN_IP_SUBNET_VLAN_MAX_PREFIX_LEN < pIpSubnetVlanRule->sipPrefix), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pIpSubnetVlanRule->ctagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pIpSubnetVlanRule->stagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < pIpSubnetVlanAction->fwdAct || ACT_TYPE_FWD > pIpSubnetVlanAction->fwdAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pIpSubnetVlanAction->spriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pIpSubnetVlanAction->spriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pIpSubnetVlanAction->cpriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pIpSubnetVlanAction->cpriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pIpSubnetVlanAction->svidAct || YT_VLAN_ACTION_NO_CHANGE > pIpSubnetVlanAction->svidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pIpSubnetVlanAction->cvidAct || YT_VLAN_ACTION_NO_CHANGE > pIpSubnetVlanAction->cvidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pIpSubnetVlanAction->newSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pIpSubnetVlanAction->newCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pIpSubnetVlanAction->newSpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pIpSubnetVlanAction->newCpri), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_table_add(unit, pIpSubnetVlanRule, pIpSubnetVlanAction, pTableIdx);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_add_by_index(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, const yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, const yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    if(!pIpSubnetVlanRule->isLagId)
    {
        CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pIpSubnetVlanRule->port))), CMM_ERR_PORT);
    }
    CMM_PARAM_CHK((YT_VLAN_IP_SUBNET_VLAN_MAX_PREFIX_LEN < pIpSubnetVlanRule->sipPrefix), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pIpSubnetVlanRule->ctagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_TAG_FMT_MAX <= pIpSubnetVlanRule->stagFmt), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((ACT_TYPE_TRAP < pIpSubnetVlanAction->fwdAct || ACT_TYPE_FWD > pIpSubnetVlanAction->fwdAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pIpSubnetVlanAction->spriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pIpSubnetVlanAction->spriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_MACIP_VLAN_PRI_ACT_COPY < pIpSubnetVlanAction->cpriAct || YT_MACIP_VLAN_PRI_ACT_NO_CHANGE > pIpSubnetVlanAction->cpriAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pIpSubnetVlanAction->svidAct || YT_VLAN_ACTION_NO_CHANGE > pIpSubnetVlanAction->svidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ACTION_COPY < pIpSubnetVlanAction->cvidAct || YT_VLAN_ACTION_NO_CHANGE > pIpSubnetVlanAction->cvidAct), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pIpSubnetVlanAction->newSvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_ID_MAX < pIpSubnetVlanAction->newCvid), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pIpSubnetVlanAction->newSpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_PRI_MAX < pIpSubnetVlanAction->newCpri), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_table_add_by_index(unit, tableIdx, pIpSubnetVlanRule, pIpSubnetVlanAction);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_get(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx, yt_ip_subnet_vlan_rule_t *pIpSubnetVlanRule, yt_ip_subnet_vlan_action_t *pIpSubnetVlanAction)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pIpSubnetVlanRule), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pIpSubnetVlanAction), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_table_get(unit, tableIdx, pIpSubnetVlanRule, pIpSubnetVlanAction);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_table_del(yt_unit_t unit, yt_ip_subnet_vlan_table_idx_t tableIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_VLAN_MAC_IP_SUBNET_VLAN_MAX_NUM < tableIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_table_del(unit, tableIdx);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_bypass_arp_set(yt_unit_t unit, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((YT_ENABLE < enable || YT_DISABLE > enable), CMM_ERR_INPUT);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_bypass_arp_set(unit, enable);
}

yt_ret_t  yt_vlan_ipSubnetBasedVlan_bypass_arp_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_ipSubnetBasedVlan_bypass_arp_get(unit, pEnable);
}

yt_ret_t  yt_vlan_policy_idx_mapping_set(yt_unit_t unit, yt_vlan_t vid,  uint8_t vlanPolicyIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((YT_VLAN_POLICY_MAX_NUM <= vlanPolicyIdx), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_policy_idx_mapping_set(unit, vid, vlanPolicyIdx);
}

yt_ret_t  yt_vlan_policy_idx_mapping_get(yt_unit_t unit, yt_vlan_t vid,  uint8_t *pVlanPolicyIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_ID_MAX < vid) || (YT_VLAN_ID_MIN == vid)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVlanPolicyIdx), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_policy_idx_mapping_get(unit, vid, pVlanPolicyIdx);
}

yt_ret_t  yt_vlan_policy_table_add(yt_unit_t unit, const yt_vlan_policy_info_t *pVlanPolicyInfo,  uint8_t *pVlanPolicyIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK((NULL == pVlanPolicyInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pVlanPolicyIdx), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_IPMC_MODE_SIP_GIP_VID < pVlanPolicyInfo->ipv4LearnMode) || (YT_VLAN_IPMC_MODE_MAC > pVlanPolicyInfo->ipv4LearnMode)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((YT_VLAN_IPMC_MODE_SIP_GIP_VID < pVlanPolicyInfo->ipv6LearnMode) || (YT_VLAN_IPMC_MODE_MAC > pVlanPolicyInfo->ipv6LearnMode)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->igmpAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->igmpAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->mldAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->mldAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange1Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange1Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange2Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange2Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange3Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange3Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6McIpRange1Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6McIpRange1Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6McIpRange2Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6McIpRange2Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->l2UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->l2UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->l2UnknownMcPortMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->ipv4UnknownMcPortMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->ipv6UnknownMcPortMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->vlan_policy_table_add(unit, pVlanPolicyInfo, pVlanPolicyIdx);
}

yt_ret_t  yt_vlan_policy_table_add_by_index(yt_unit_t unit, uint8_t vlanPolicyIdx, const yt_vlan_policy_info_t *pVlanPolicyInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_POLICY_MAX_NUM <= vlanPolicyIdx) || (0 == vlanPolicyIdx)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVlanPolicyInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK(((YT_VLAN_IPMC_MODE_SIP_GIP_VID < pVlanPolicyInfo->ipv4LearnMode) || (YT_VLAN_IPMC_MODE_MAC > pVlanPolicyInfo->ipv4LearnMode)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((YT_VLAN_IPMC_MODE_SIP_GIP_VID < pVlanPolicyInfo->ipv6LearnMode) || (YT_VLAN_IPMC_MODE_MAC > pVlanPolicyInfo->ipv6LearnMode)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->igmpAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->igmpAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->mldAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->mldAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange1Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange1Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange2Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange2Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4McIpRange3Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4McIpRange3Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6McIpRange1Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6McIpRange1Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6McIpRange2Act) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6McIpRange2Act)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->l2UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->l2UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv4UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv4UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK(((ACT_TYPE_TRAP < pVlanPolicyInfo->ipv6UnknownMcAct) || (ACT_TYPE_FWD > pVlanPolicyInfo->ipv6UnknownMcAct)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->l2UnknownMcPortMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->ipv4UnknownMcPortMask))), CMM_ERR_PORTLIST);
    CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pVlanPolicyInfo->ipv6UnknownMcPortMask))), CMM_ERR_PORTLIST);

    return YT_DISPATCH(unit)->vlan_policy_table_add_by_index(unit, vlanPolicyIdx, pVlanPolicyInfo);
}

yt_ret_t  yt_vlan_policy_table_get(yt_unit_t unit, uint8_t vlanPolicyIdx, yt_vlan_policy_info_t *pVlanPolicyInfo)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_POLICY_MAX_NUM <= vlanPolicyIdx) || (0 == vlanPolicyIdx)), CMM_ERR_EXCEED_RANGE);
    CMM_PARAM_CHK((NULL == pVlanPolicyInfo), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->vlan_policy_table_get(unit, vlanPolicyIdx, pVlanPolicyInfo);
}

yt_ret_t  yt_vlan_policy_table_del(yt_unit_t unit, uint8_t vlanPolicyIdx)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);
    CMM_PARAM_CHK(((YT_VLAN_POLICY_MAX_NUM <= vlanPolicyIdx) || (0 == vlanPolicyIdx)), CMM_ERR_EXCEED_RANGE);

    return YT_DISPATCH(unit)->vlan_policy_table_del(unit, vlanPolicyIdx);
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_vlan_port_igrPvid_set);
EXPORT_SYMBOL(yt_vlan_port_igrPvid_get);
EXPORT_SYMBOL(yt_vlan_port_set);
EXPORT_SYMBOL(yt_vlan_port_get);
EXPORT_SYMBOL(yt_vlan_port_egrFilter_enable_set);
EXPORT_SYMBOL(yt_vlan_port_egrFilter_enable_get);
EXPORT_SYMBOL(yt_vlan_port_igrFilter_enable_set);
EXPORT_SYMBOL(yt_vlan_port_igrFilter_enable_get);
#endif
