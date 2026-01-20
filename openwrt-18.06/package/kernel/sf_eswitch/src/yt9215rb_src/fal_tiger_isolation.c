/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_isolation.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"


yt_ret_t fal_tiger_isolation_port_set(yt_unit_t unit, yt_port_t port, yt_port_mask_t isoPortMask)
{
    l2_port_isolation_ctrln_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    yt_port_mask_t macmask;

    macid = CAL_YTP_TO_MAC(unit,port);
    CAL_YTPLIST_TO_MLIST(unit,isoPortMask, macmask);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_ISOLATION_CTRLNm, macid, sizeof(l2_port_isolation_ctrln_t), &entry), ret); 
    HAL_FIELD_SET(L2_PORT_ISOLATION_CTRLNm, L2_PORT_ISOLATION_CTRLN_ISOLATED_PORT_MASKf, &entry, macmask.portbits[0]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_PORT_ISOLATION_CTRLNm, macid, sizeof(l2_port_isolation_ctrln_t), &entry), ret); 
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_port_get(yt_unit_t unit, yt_port_t port, yt_port_mask_t *pIsoPortMask)
{
    l2_port_isolation_ctrln_t  entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    yt_port_mask_t macMask;
    uint32_t portMask;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_CLEAR_MEMBER_PORT(macMask);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_ISOLATION_CTRLNm, macid, sizeof(l2_port_isolation_ctrln_t), &entry), ret); 
    HAL_FIELD_GET(L2_PORT_ISOLATION_CTRLNm, L2_PORT_ISOLATION_CTRLN_ISOLATED_PORT_MASKf, &entry, &portMask);
    macMask.portbits[0] = portMask;

    CAL_MLIST_TO_YTPLIST(unit, macMask, (*pIsoPortMask));
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_vlan_add(yt_unit_t unit, yt_iso_vlan_info_t isoInfo, uint16_t *idx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(isoInfo);
    CMM_UNUSED_PARAM(idx);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_vlan_del_by_index(yt_unit_t unit, uint16_t idx)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(idx);

    return CMM_ERR_OK;

}

yt_ret_t fal_tiger_isolation_vlan_set_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t isoInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(isoInfo);
    CMM_UNUSED_PARAM(idx);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_vlan_get_by_index(yt_unit_t unit, uint16_t idx, yt_iso_vlan_info_t *pIsoInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pIsoInfo);
    CMM_UNUSED_PARAM(idx);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_vlan_type_set(yt_unit_t unit, yt_iso_vlan_type_t type)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(type);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_isolation_vlan_type_get(yt_unit_t unit, yt_iso_vlan_type_t *pType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pType);

    return CMM_ERR_OK;
}

