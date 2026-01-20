/**  @file
  *  @brief    brief  description
  *  @author
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#include "l2sw_switch.h"
#include "svlan.h"
#include "vlan.h"
#include "sal_string.h"
#include "l2sw_asicdrv_svlan.h"

l2sw_uint8               svlan_mbrCfgUsage[L2SW_MAX_NUM_OF_UNIT][L2SW_SVIDXNO];
l2sw_uint16              svlan_mbrCfgVid[L2SW_MAX_NUM_OF_UNIT][L2SW_SVIDXNO] = {0};
l2sw_svlan_lookupType_t  svlan_lookupType[L2SW_MAX_NUM_OF_UNIT];

static l2sw_api_ret_t _l2sw_svlan_init(l2sw_uint8 units)
{
    l2sw_uint32 i;
    l2sw_api_ret_t retVal;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_s2c_t svlanSP2CConf;
    l2sw_svlan_mc2s_t svlanMC2SConf;
    l2sw_uint32 svidx;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /*default use C-priority*/
    if ((retVal = l2sw_setAsicSvlanPrioritySel(units, SPRISEL_CTAGPRI)) != L2SW_ERR_OK)
        return retVal;

    /*Drop SVLAN untag frame*/
    if ((retVal = l2sw_setAsicSvlanIngressUntag(units, UNTAG_DROP)) != L2SW_ERR_OK)
        return retVal;

    /*Drop SVLAN unmatch frame*/
    if ((retVal = l2sw_setAsicSvlanIngressUnmatch(units, UNMATCH_DROP)) != L2SW_ERR_OK)
        return retVal;

    /*Set TPID to 0x88a8*/
    if ((retVal = l2sw_setAsicSvlanTpid(units, 0x88a8)) != L2SW_ERR_OK)
        return retVal;

    /*Clean Uplink Port Mask to none*/
    if ((retVal = l2sw_setAsicSvlanUplinkPortMask(units, 0)) != L2SW_ERR_OK)
        return retVal;

    /*Clean SVLAN Member Configuration*/
    for (i=0; i<= L2SW_SVIDXMAX; i++)
    {
        MEMSET(&svlanMemConf, 0, sizeof(l2sw_svlan_memconf_t));
        if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Clean C2S Configuration*/
    for (i=0; i<= L2SW_C2SIDXMAX; i++)
    {
        if ((retVal = l2sw_setAsicSvlanC2SConf(units, i, 0,0,0)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Clean SP2C Configuration*/
    for (i=0; i <= L2SW_SP2CMAX ; i++)
    {
        MEMSET(&svlanSP2CConf, 0, sizeof(l2sw_svlan_s2c_t));
        if ((retVal = l2sw_setAsicSvlanSP2CConf(units, i, &svlanSP2CConf)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Clean MC2S Configuration*/
    for (i=0 ; i<= L2SW_MC2SIDXMAX; i++)
    {
        MEMSET(&svlanMC2SConf, 0, sizeof(l2sw_svlan_mc2s_t));
        if ((retVal = l2sw_setAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;
    }


    svlan_lookupType[units] = SVLAN_LOOKUP_S64MBRCGF;


    for (svidx = 0; svidx <= L2SW_SVIDXMAX; svidx++)
    {
        svlan_mbrCfgUsage[units][svidx] = L2SW_FALSE;
    }


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_servicePort_add(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicSvlanUplinkPortMask(units, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    pmsk = pmsk | (1<<l2sw_switch_port_L2P_get(units, port));

    if ((retVal = l2sw_setAsicSvlanUplinkPortMask(units, pmsk)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_servicePort_get(l2sw_uint8 units, l2sw_portmask_t *pSvlan_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 phyMbrPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_portmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSvlanUplinkPortMask(units, &phyMbrPmask)) != L2SW_ERR_OK)
        return retVal;

    if(l2sw_switch_portmask_P2L_get(units, phyMbrPmask, pSvlan_portmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_servicePort_del(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmsk;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicSvlanUplinkPortMask(units, &pmsk)) != L2SW_ERR_OK)
        return retVal;

    pmsk = pmsk & ~(1<<l2sw_switch_port_L2P_get(units, port));

    if ((retVal = l2sw_setAsicSvlanUplinkPortMask(units, pmsk)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_tpidEntry_set(l2sw_uint8 units, l2sw_svlan_tpid_t svlan_tag_id)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (svlan_tag_id>L2SW_MAX_NUM_OF_PROTO_TYPE)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicSvlanTpid(units, svlan_tag_id)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_tpidEntry_get(l2sw_uint8 units, l2sw_svlan_tpid_t *pSvlan_tag_id)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_tag_id)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSvlanTpid(units, pSvlan_tag_id)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_priorityRef_set(l2sw_uint8 units, l2sw_svlan_pri_ref_t ref)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (ref >= REF_PRI_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicSvlanPrioritySel(units, ref)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_priorityRef_get(l2sw_uint8 units, l2sw_svlan_pri_ref_t *pRef)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pRef)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSvlanPrioritySel(units, pRef)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_memberPortEntry_set(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;
    l2sw_int32 i;
    l2sw_uint32 empty_idx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_uint32 phyMbrPmask;
    l2sw_vlan_cfg_t vlanCfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_cfg)
        return L2SW_ERR_NULL_POINTER;

    if(svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    L2SW_CHK_PORTMASK_VALID(units, &(pSvlan_cfg->memberport));

    L2SW_CHK_PORTMASK_VALID(units, &(pSvlan_cfg->untagport));

    if (pSvlan_cfg->fiden > L2SW_ENABLED)
        return L2SW_ERR_ENABLE;

    if (pSvlan_cfg->fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if (pSvlan_cfg->priority > L2SW_PRIMAX)
        return L2SW_ERR_VLAN_PRIORITY;

    if (pSvlan_cfg->efiden > L2SW_ENABLED)
        return L2SW_ERR_ENABLE;

    if (pSvlan_cfg->efid > L2SW_EFIDMAX)
        return L2SW_ERR_L2_FID;

    if(SVLAN_LOOKUP_C4KVLAN == svlan_lookupType[units])
    {
        if ((retVal = l2sw_vlan_get(units, svid, &vlanCfg)) != L2SW_ERR_OK)
            return retVal;

        vlanCfg.mbr = pSvlan_cfg->memberport;
        vlanCfg.untag = pSvlan_cfg->untagport;

        if ((retVal = l2sw_vlan_set(units, svid, &vlanCfg)) != L2SW_ERR_OK)
            return retVal;

        empty_idx = 0xFF;

        for (i = 0; i<= L2SW_SVIDXMAX; i++)
        {
            if (svid == svlan_mbrCfgVid[units][i] && L2SW_TRUE == svlan_mbrCfgUsage[units][i])
            {
                MEMSET(&svlanMemConf, 0, sizeof(l2sw_svlan_memconf_t));
                svlanMemConf.vs_svid        = svid;
                svlanMemConf.vs_efiden      = pSvlan_cfg->efiden;
                svlanMemConf.vs_efid        = pSvlan_cfg->efid;
                svlanMemConf.vs_priority    = pSvlan_cfg->priority;

                /*for create check*/
                if(0 == svlanMemConf.vs_efiden && 0 == svlanMemConf.vs_efid)
                    svlanMemConf.vs_efid = 1;

                if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
                    return retVal;

                return L2SW_ERR_OK;
            }
            else if (L2SW_FALSE == svlan_mbrCfgUsage[units][i] && 0xFF == empty_idx)
            {
                empty_idx = i;
            }
        }

        if (empty_idx != 0xFF)
        {
            svlan_mbrCfgUsage[units][empty_idx] = L2SW_TRUE;
            svlan_mbrCfgVid[units][empty_idx] = svid;

            MEMSET(&svlanMemConf, 0, sizeof(l2sw_svlan_memconf_t));
            svlanMemConf.vs_svid        = svid;
            svlanMemConf.vs_efiden      = pSvlan_cfg->efiden;
            svlanMemConf.vs_efid        = pSvlan_cfg->efid;
            svlanMemConf.vs_priority    = pSvlan_cfg->priority;

            /*for create check*/
            if(0 == svlanMemConf.vs_efiden && 0 == svlanMemConf.vs_efid)
                svlanMemConf.vs_efid = 1;

            if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, empty_idx, &svlanMemConf)) != L2SW_ERR_OK)
                return retVal;

        }

        return L2SW_ERR_OK;
    }


    empty_idx = 0xFF;

    for (i = 0; i<= L2SW_SVIDXMAX; i++)
    {
        /*
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;
        */
        if (svid == svlan_mbrCfgVid[units][i] && L2SW_TRUE == svlan_mbrCfgUsage[units][i])
        {
            svlanMemConf.vs_svid = svid;

            if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->memberport), &phyMbrPmask) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;

            svlanMemConf.vs_member = phyMbrPmask;

            if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->untagport), &phyMbrPmask) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;

            svlanMemConf.vs_untag = phyMbrPmask;

            svlanMemConf.vs_force_fid   = pSvlan_cfg->fiden;
            svlanMemConf.vs_fid_msti    = pSvlan_cfg->fid;
            svlanMemConf.vs_priority    = pSvlan_cfg->priority;
            svlanMemConf.vs_efiden      = pSvlan_cfg->efiden;
            svlanMemConf.vs_efid        = pSvlan_cfg->efid;

            /*all items are reset means deleting*/
            if( 0 == svlanMemConf.vs_member &&
                0 == svlanMemConf.vs_untag &&
                0 == svlanMemConf.vs_force_fid &&
                0 == svlanMemConf.vs_fid_msti &&
                0 == svlanMemConf.vs_priority &&
                0 == svlanMemConf.vs_efiden &&
                0 == svlanMemConf.vs_efid)
            {
                svlan_mbrCfgUsage[units][i] = L2SW_FALSE;
                svlan_mbrCfgVid[units][i] = 0;

                /* Clear SVID also */
                svlanMemConf.vs_svid = 0;
            }
            else
            {
                svlan_mbrCfgUsage[units][i] = L2SW_TRUE;
                svlan_mbrCfgVid[units][i] = svlanMemConf.vs_svid;

                if(0 == svlanMemConf.vs_svid)
                {
                    /*for create check*/
                    if(0 == svlanMemConf.vs_efiden && 0 == svlanMemConf.vs_efid)
                    {
                        svlanMemConf.vs_efid = 1;
                    }
                }
            }

            if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
                return retVal;

            return L2SW_ERR_OK;
        }
        else if (L2SW_FALSE == svlan_mbrCfgUsage[units][i] && 0xFF == empty_idx)
        {
            empty_idx = i;
        }
    }

    if (empty_idx != 0xFF)
    {
        MEMSET(&svlanMemConf, 0, sizeof(l2sw_svlan_memconf_t));
        svlanMemConf.vs_svid = svid;

        if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->memberport), &phyMbrPmask) != L2SW_ERR_OK)
            return L2SW_ERR_FAILED;

        svlanMemConf.vs_member = phyMbrPmask;

        if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->untagport), &phyMbrPmask) != L2SW_ERR_OK)
            return L2SW_ERR_FAILED;

        svlanMemConf.vs_untag = phyMbrPmask;

        svlanMemConf.vs_force_fid   = pSvlan_cfg->fiden;
        svlanMemConf.vs_fid_msti    = pSvlan_cfg->fid;
        svlanMemConf.vs_priority    = pSvlan_cfg->priority;

        svlanMemConf.vs_efiden      = pSvlan_cfg->efiden;
        svlanMemConf.vs_efid        = pSvlan_cfg->efid;

        /*change efid for empty svid 0*/
        if(0 == svlanMemConf.vs_svid)
        {   /*for create check*/
            if(0 == svlanMemConf.vs_efiden && 0 == svlanMemConf.vs_efid)
            {
                svlanMemConf.vs_efid = 1;
            }
        }

        svlan_mbrCfgUsage[units][empty_idx] = L2SW_TRUE;
        svlan_mbrCfgVid[units][empty_idx] = svlanMemConf.vs_svid;

        if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, empty_idx, &svlanMemConf)) != L2SW_ERR_OK)
        {
            return retVal;
        }

        return L2SW_ERR_OK;
    }

    return L2SW_ERR_SVLAN_TABLE_FULL;
}

static l2sw_api_ret_t _l2sw_svlan_memberPortEntry_get(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_cfg)
        return L2SW_ERR_NULL_POINTER;

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;


    for (i = 0; i<= L2SW_SVIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            pSvlan_cfg->svid        = svlanMemConf.vs_svid;

            if(l2sw_switch_portmask_P2L_get(units, svlanMemConf.vs_member,&(pSvlan_cfg->memberport)) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;

            if(l2sw_switch_portmask_P2L_get(units, svlanMemConf.vs_untag,&(pSvlan_cfg->untagport)) != L2SW_ERR_OK)
                return L2SW_ERR_FAILED;

            pSvlan_cfg->fiden       = svlanMemConf.vs_force_fid;
            pSvlan_cfg->fid         = svlanMemConf.vs_fid_msti;
            pSvlan_cfg->priority    = svlanMemConf.vs_priority;
            pSvlan_cfg->efiden      = svlanMemConf.vs_efiden;
            pSvlan_cfg->efid        = svlanMemConf.vs_efid;

            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

}

static l2sw_api_ret_t _l2sw_svlan_memberPortEntry_adv_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_uint32 phyMbrPmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_cfg)
        return L2SW_ERR_NULL_POINTER;

    if (idx > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    if (pSvlan_cfg->svid>L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    L2SW_CHK_PORTMASK_VALID(units, &(pSvlan_cfg->memberport));

    L2SW_CHK_PORTMASK_VALID(units, &(pSvlan_cfg->untagport));

    if (pSvlan_cfg->fiden > L2SW_ENABLED)
        return L2SW_ERR_ENABLE;

    if (pSvlan_cfg->fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if (pSvlan_cfg->priority > L2SW_PRIMAX)
        return L2SW_ERR_VLAN_PRIORITY;

    if (pSvlan_cfg->efiden > L2SW_ENABLED)
        return L2SW_ERR_ENABLE;

    if (pSvlan_cfg->efid > L2SW_EFIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&svlanMemConf, 0, sizeof(l2sw_svlan_memconf_t));
    svlanMemConf.vs_svid        = pSvlan_cfg->svid;
    if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->memberport), &phyMbrPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    svlanMemConf.vs_member = phyMbrPmask;

    if(l2sw_switch_portmask_L2P_get(units, &(pSvlan_cfg->untagport), &phyMbrPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    svlanMemConf.vs_untag = phyMbrPmask;


    svlanMemConf.vs_force_fid   = pSvlan_cfg->fiden;
    svlanMemConf.vs_fid_msti    = pSvlan_cfg->fid;
    svlanMemConf.vs_priority    = pSvlan_cfg->priority;
    svlanMemConf.vs_efiden      = pSvlan_cfg->efiden;
    svlanMemConf.vs_efid        = pSvlan_cfg->efid;

    if(0 == svlanMemConf.vs_svid &&
        0 == svlanMemConf.vs_member &&
        0 == svlanMemConf.vs_untag &&
        0 == svlanMemConf.vs_force_fid &&
        0 == svlanMemConf.vs_fid_msti &&
        0 == svlanMemConf.vs_priority &&
        0 == svlanMemConf.vs_efiden &&
        0 == svlanMemConf.vs_efid)
    {
        svlan_mbrCfgUsage[units][idx] = L2SW_FALSE;
        svlan_mbrCfgVid[units][idx] = 0;
    }
    else
    {
        svlan_mbrCfgUsage[units][idx] = L2SW_TRUE;
        svlan_mbrCfgVid[units][idx] = svlanMemConf.vs_svid;
    }

    if ((retVal = l2sw_setAsicSvlanMemberConfiguration(units, idx, &svlanMemConf)) != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_memberPortEntry_adv_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvlan_cfg)
        return L2SW_ERR_NULL_POINTER;

    if (idx > L2SW_SVIDXMAX)
        return L2SW_ERR_SVLAN_ENTRY_INDEX;

    if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, idx, &svlanMemConf)) != L2SW_ERR_OK)
        return retVal;

    pSvlan_cfg->svid        = svlanMemConf.vs_svid;
    if(l2sw_switch_portmask_P2L_get(units, svlanMemConf.vs_member,&(pSvlan_cfg->memberport)) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    if(l2sw_switch_portmask_P2L_get(units, svlanMemConf.vs_untag,&(pSvlan_cfg->untagport)) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    pSvlan_cfg->fiden       = svlanMemConf.vs_force_fid;
    pSvlan_cfg->fid         = svlanMemConf.vs_fid_msti;
    pSvlan_cfg->priority    = svlanMemConf.vs_priority;
    pSvlan_cfg->efiden      = svlanMemConf.vs_efiden;
    pSvlan_cfg->efid        = svlanMemConf.vs_efid;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_svlan_defaultSvlan_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    /* svid must be 0~4095 */
    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            if ((retVal = l2sw_setAsicSvlanDefaultVlan(units, l2sw_switch_port_L2P_get(units, port), i)) != L2SW_ERR_OK)
                return retVal;

            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;
}

static l2sw_api_ret_t _l2sw_svlan_defaultSvlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 idx;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicSvlanDefaultVlan(units, l2sw_switch_port_L2P_get(units, port), &idx)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, idx, &svlanMemConf)) != L2SW_ERR_OK)
        return retVal;

    *pSvid = svlanMemConf.vs_svid;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_c2s_add(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal, i;
    l2sw_uint32 empty_idx;
    l2sw_uint32 evid, pmsk, svidx, c2s_svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_port_t phyPort;
    l2sw_uint16 doneFlag;


    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);


    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, src_port);

    phyPort = l2sw_switch_port_L2P_get(units, src_port);

    empty_idx = 0xFFFF;
    svidx = 0xFFFF;
    doneFlag = L2SW_FALSE;

    for (i = 0; i<= L2SW_SVIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

    for (i=L2SW_C2SIDXMAX; i>=0; i--)
    {
        if ((retVal = l2sw_getAsicSvlanC2SConf(units, i, &evid, &pmsk, &c2s_svidx)) != L2SW_ERR_OK)
                return retVal;

        if (evid == vid)
        {
            /* Check Src_port */
            if(pmsk & (1 << phyPort))
            {
                /* Check SVIDX */
                if(c2s_svidx == svidx)
                {
                    /* All the same, do nothing */
                }
                else
                {
                    /* New svidx, remove src_port and find a new slot to add a new enrty */
                    pmsk = pmsk & ~(1 << phyPort);
                    if(pmsk == 0)
                        c2s_svidx = 0;

                    if ((retVal = l2sw_setAsicSvlanC2SConf(units, i, vid, pmsk, c2s_svidx)) != L2SW_ERR_OK)
                        return retVal;
                }
            }
            else
            {
                if(c2s_svidx == svidx && doneFlag == L2SW_FALSE)
                {
                    pmsk = pmsk | (1 << phyPort);
                    if ((retVal = l2sw_setAsicSvlanC2SConf(units, i, vid, pmsk, svidx)) != L2SW_ERR_OK)
                        return retVal;

                    doneFlag = L2SW_TRUE;
                }
            }
        }
        else if (evid==0&&pmsk==0)
        {
            empty_idx = i;
        }
    }

    if (0xFFFF != empty_idx && doneFlag ==L2SW_FALSE)
    {
       if ((retVal = l2sw_setAsicSvlanC2SConf(units, empty_idx, vid, (1<<phyPort), svidx)) != L2SW_ERR_OK)
           return retVal;

       return L2SW_ERR_OK;
    }
    else if(doneFlag == L2SW_TRUE)
    {
        return L2SW_ERR_OK;
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_c2s_del(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 evid, pmsk, svidx;
    l2sw_port_t phyPort;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (vid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, src_port);
    phyPort = l2sw_switch_port_L2P_get(units, src_port);

    for (i = 0; i <= L2SW_C2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanC2SConf(units, i, &evid, &pmsk, &svidx)) != L2SW_ERR_OK)
            return retVal;

        if (evid == vid)
        {
            if(pmsk & (1 << phyPort))
            {
                pmsk = pmsk & ~(1 << phyPort);
                if(pmsk == 0)
                {
                    vid = 0;
                    svidx = 0;
                }

                if ((retVal = l2sw_setAsicSvlanC2SConf(units, i, vid, pmsk, svidx)) != L2SW_ERR_OK)
                    return retVal;

                return L2SW_ERR_OK;
            }
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_c2s_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 evid, pmsk, svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_port_t phyPort;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, src_port);
    phyPort = l2sw_switch_port_L2P_get(units, src_port);

    for (i = 0; i <= L2SW_C2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanC2SConf(units, i, &evid, &pmsk, &svidx)) != L2SW_ERR_OK)
            return retVal;

        if (evid == vid)
        {
            if(pmsk & (1 << phyPort))
            {
                if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, svidx, &svlanMemConf)) != L2SW_ERR_OK)
                    return retVal;

                *pSvid = svlanMemConf.vs_svid;
                return L2SW_ERR_OK;
            }
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_untag_action_set(l2sw_uint8 units, l2sw_svlan_untag_action_t action, l2sw_vlan_t svid)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      i;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (action >= UNTAG_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(action == UNTAG_ASSIGN)
    {
        if (svid > L2SW_VIDMAX)
            return L2SW_ERR_SVLAN_VID;
    }

    if ((retVal = l2sw_setAsicSvlanIngressUntag(units, (l2sw_uint32)action)) != L2SW_ERR_OK)
        return retVal;

    if(action == UNTAG_ASSIGN)
    {
        for (i = 0; i < L2SW_SVIDXNO; i++)
        {
            if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
                return retVal;

            if (svid == svlanMemConf.vs_svid)
            {
                if ((retVal = l2sw_setAsicSvlanUntagVlan(units, i)) != L2SW_ERR_OK)
                    return retVal;

                return L2SW_ERR_OK;
            }
        }

        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_untag_action_get(l2sw_uint8 units, l2sw_svlan_untag_action_t *pAction, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      svidx;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction || NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSvlanIngressUntag(units, pAction)) != L2SW_ERR_OK)
        return retVal;

    if(*pAction == UNTAG_ASSIGN)
    {
        if ((retVal = l2sw_getAsicSvlanUntagVlan(units, &svidx)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, svidx, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        *pSvid = svlanMemConf.vs_svid;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_unmatch_action_set(l2sw_uint8 units, l2sw_svlan_unmatch_action_t action, l2sw_vlan_t svid)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      i;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (action >= UNMATCH_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if (action == UNMATCH_ASSIGN || action == UNMATCH_ASSIGN_KEEP)
    {
        if (svid > L2SW_VIDMAX)
            return L2SW_ERR_SVLAN_VID;
    }

    if ((retVal = l2sw_setAsicSvlanIngressUnmatch(units, (l2sw_uint32)action)) != L2SW_ERR_OK)
        return retVal;

    if (action == UNMATCH_ASSIGN || action == UNMATCH_ASSIGN_KEEP)
    {
        for (i = 0; i < L2SW_SVIDXNO; i++)
        {
            if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
                return retVal;

            if (svid == svlanMemConf.vs_svid)
            {
                if ((retVal = l2sw_setAsicSvlanUnmatchVlan(units, i)) != L2SW_ERR_OK)
                    return retVal;

                return L2SW_ERR_OK;
            }
        }

        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_unmatch_action_get(l2sw_uint8 units, l2sw_svlan_unmatch_action_t *pAction, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      svidx;
    l2sw_svlan_memconf_t svlanMemConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction || NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSvlanIngressUnmatch(units, pAction)) != L2SW_ERR_OK)
        return retVal;

    if(*pAction == UNMATCH_ASSIGN || *pAction == UNMATCH_ASSIGN_KEEP)
    {
        if ((retVal = l2sw_getAsicSvlanUnmatchVlan(units, &svidx)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, svidx, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        *pSvid = svlanMemConf.vs_svid;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_unassign_action_set(l2sw_uint8 units, l2sw_svlan_unassign_action_t action)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (action >= UNASSIGN_END)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicSvlanEgressUnassign(units, (l2sw_uint32)action);

    return retVal;
}

static l2sw_api_ret_t _l2sw_svlan_unassign_action_get(l2sw_uint8 units, l2sw_svlan_unassign_action_t *pAction)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    retVal = l2sw_getAsicSvlanEgressUnassign(units, pAction);

    return retVal;
}

static l2sw_api_ret_t _l2sw_svlan_dmac_vidsel_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicSvlanDmacCvidSel(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
            return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_dmac_vidsel_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicSvlanDmacCvidSel(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
            return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_ipmc2s_add(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk,l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal, i;
    l2sw_uint32 empty_idx;
    l2sw_uint32 svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    if ((ipmc&0xF0000000)!=0xE0000000)
        return L2SW_ERR_INPUT;

    svidx = 0xFFFF;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
            return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;


    empty_idx = 0xFFFF;

    for (i = L2SW_MC2SIDXMAX; i >= 0; i--)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid)
        {
            if (svlanMC2SConf.format == SVLAN_MC2S_MODE_IP &&
                svlanMC2SConf.sdata==ipmc&&
                svlanMC2SConf.smask==ipmcMsk)
            {
                svlanMC2SConf.svidx = svidx;
                if ((retVal = l2sw_setAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
                    return retVal;
            }
        }
        else
        {
            empty_idx = i;
        }
    }

    if (empty_idx!=0xFFFF)
    {
        svlanMC2SConf.valid = L2SW_TRUE;
        svlanMC2SConf.svidx = svidx;
        svlanMC2SConf.format = SVLAN_MC2S_MODE_IP;
        svlanMC2SConf.sdata = ipmc;
        svlanMC2SConf.smask = ipmcMsk;
        if ((retVal = l2sw_setAsicSvlanMC2SConf(units, empty_idx, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;
        return L2SW_ERR_OK;
    }

    return L2SW_ERR_OUT_OF_RANGE;

}

static l2sw_api_ret_t _l2sw_svlan_ipmc2s_del(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((ipmc&0xF0000000)!=0xE0000000)
        return L2SW_ERR_INPUT;

    for (i = 0; i <= L2SW_MC2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid)
        {
            if (svlanMC2SConf.format == SVLAN_MC2S_MODE_IP &&
                svlanMC2SConf.sdata==ipmc&&
                svlanMC2SConf.smask==ipmcMsk)
            {
                MEMSET(&svlanMC2SConf, 0, sizeof(l2sw_svlan_mc2s_t));
                if ((retVal = l2sw_setAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
                    return retVal;
                return L2SW_ERR_OK;
            }
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_ipmc2s_get(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    if ((ipmc&0xF0000000)!=0xE0000000)
        return L2SW_ERR_INPUT;

    for (i = 0; i <= L2SW_MC2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid &&
            svlanMC2SConf.format == SVLAN_MC2S_MODE_IP &&
            svlanMC2SConf.sdata == ipmc &&
            svlanMC2SConf.smask == ipmcMsk)
        {
            if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, svlanMC2SConf.svidx, &svlanMemConf)) != L2SW_ERR_OK)
                return retVal;
            *pSvid = svlanMemConf.vs_svid;
            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_l2mc2s_add(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal, i;
    l2sw_uint32 empty_idx;
    l2sw_uint32 svidx, l2add, l2Mask;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    if (mac.octet[0]!= 1&&mac.octet[1]!=0)
        return L2SW_ERR_INPUT;

    l2add = (mac.octet[2] << 24) | (mac.octet[3] << 16) | (mac.octet[4] << 8) | mac.octet[5];
    l2Mask = (macMsk.octet[2] << 24) | (macMsk.octet[3] << 16) | (macMsk.octet[4] << 8) | macMsk.octet[5];

    svidx = 0xFFFF;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

    empty_idx = 0xFFFF;

    for (i = L2SW_MC2SIDXMAX; i >=0; i--)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid)
        {
            if (svlanMC2SConf.format == SVLAN_MC2S_MODE_MAC &&
                svlanMC2SConf.sdata==l2add&&
                svlanMC2SConf.smask==l2Mask)
            {
                svlanMC2SConf.svidx = svidx;
                if ((retVal = l2sw_setAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
                    return retVal;
            }
        }
        else
        {
            empty_idx = i;
        }
    }

    if (empty_idx!=0xFFFF)
    {
        svlanMC2SConf.valid = L2SW_TRUE;
        svlanMC2SConf.svidx = svidx;
        svlanMC2SConf.format = SVLAN_MC2S_MODE_MAC;
        svlanMC2SConf.sdata = l2add;
        svlanMC2SConf.smask = l2Mask;

        if ((retVal = l2sw_setAsicSvlanMC2SConf(units, empty_idx, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;
        return L2SW_ERR_OK;
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_l2mc2s_del(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 l2add, l2Mask;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (mac.octet[0]!= 1&&mac.octet[1]!=0)
        return L2SW_ERR_INPUT;

    l2add = (mac.octet[2] << 24) | (mac.octet[3] << 16) | (mac.octet[4] << 8) | mac.octet[5];
    l2Mask = (macMsk.octet[2] << 24) | (macMsk.octet[3] << 16) | (macMsk.octet[4] << 8) | macMsk.octet[5];

    for (i = 0; i <= L2SW_MC2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid)
        {
            if (svlanMC2SConf.format == SVLAN_MC2S_MODE_MAC &&
                svlanMC2SConf.sdata==l2add&&
                svlanMC2SConf.smask==l2Mask)
            {
                MEMSET(&svlanMC2SConf, 0, sizeof(l2sw_svlan_mc2s_t));
                if ((retVal = l2sw_setAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
                    return retVal;
                return L2SW_ERR_OK;
            }
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_l2mc2s_get(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 l2add,l2Mask;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_mc2s_t svlanMC2SConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pSvid)
        return L2SW_ERR_NULL_POINTER;

    if (mac.octet[0]!= 1&&mac.octet[1]!=0)
        return L2SW_ERR_INPUT;

    l2add = (mac.octet[2] << 24) | (mac.octet[3] << 16) | (mac.octet[4] << 8) | mac.octet[5];
    l2Mask = (macMsk.octet[2] << 24) | (macMsk.octet[3] << 16) | (macMsk.octet[4] << 8) | macMsk.octet[5];

    for (i = 0; i <= L2SW_MC2SIDXMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMC2SConf(units, i, &svlanMC2SConf)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == svlanMC2SConf.valid)
        {
            if (svlanMC2SConf.format == SVLAN_MC2S_MODE_MAC &&
                svlanMC2SConf.sdata==l2add&&
                svlanMC2SConf.smask==l2Mask)
            {
                if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, svlanMC2SConf.svidx, &svlanMemConf)) != L2SW_ERR_OK)
                    return retVal;
                *pSvid = svlanMemConf.vs_svid;

                return L2SW_ERR_OK;
            }
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_sp2c_add(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t cvid)
{
    l2sw_api_ret_t retVal, i;
    l2sw_uint32 empty_idx, svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_s2c_t svlanSP2CConf;
    l2sw_port_t port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    if (cvid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, dst_port);
    port = l2sw_switch_port_L2P_get(units, dst_port);

    svidx = 0xFFFF;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

    empty_idx = 0xFFFF;

    for (i=L2SW_SP2CMAX; i >=0 ; i--)
    {
        if ((retVal = l2sw_getAsicSvlanSP2CConf(units, i, &svlanSP2CConf)) != L2SW_ERR_OK)
            return retVal;

        if ( (svlanSP2CConf.svidx == svidx) && (svlanSP2CConf.dstport == port) && (svlanSP2CConf.valid == 1))
        {
            empty_idx = i;
            break;
        }
        else if (svlanSP2CConf.valid == 0)
        {
            empty_idx = i;
        }
    }

    if (empty_idx!=0xFFFF)
    {
        svlanSP2CConf.valid     = 1;
        svlanSP2CConf.vid       = cvid;
        svlanSP2CConf.svidx     = svidx;
        svlanSP2CConf.dstport   = port;

        if ((retVal = l2sw_setAsicSvlanSP2CConf(units, empty_idx, &svlanSP2CConf)) != L2SW_ERR_OK)
            return retVal;
        return L2SW_ERR_OK;
    }

    return L2SW_ERR_OUT_OF_RANGE;

}

static l2sw_api_ret_t _l2sw_svlan_sp2c_get(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t *pCvid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i, svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_s2c_t svlanSP2CConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pCvid)
        return L2SW_ERR_NULL_POINTER;

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, dst_port);
    dst_port = l2sw_switch_port_L2P_get(units, dst_port);

    svidx = 0xFFFF;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

    for (i = 0; i <= L2SW_SP2CMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanSP2CConf(units, i, &svlanSP2CConf)) != L2SW_ERR_OK)
            return retVal;

        if ( (svlanSP2CConf.svidx == svidx) && (svlanSP2CConf.dstport == dst_port) && (svlanSP2CConf.valid == 1) )
        {
            *pCvid = svlanSP2CConf.vid;
            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_OUT_OF_RANGE;
}

static l2sw_api_ret_t _l2sw_svlan_sp2c_del(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i, svidx;
    l2sw_svlan_memconf_t svlanMemConf;
    l2sw_svlan_s2c_t svlanSP2CConf;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (svid > L2SW_VIDMAX)
        return L2SW_ERR_SVLAN_VID;

    /* Check port Valid */
    L2SW_CHK_PORT_VALID(units, dst_port);
    dst_port = l2sw_switch_port_L2P_get(units, dst_port);

    svidx = 0xFFFF;

    for (i = 0; i < L2SW_SVIDXNO; i++)
    {
        if ((retVal = l2sw_getAsicSvlanMemberConfiguration(units, i, &svlanMemConf)) != L2SW_ERR_OK)
            return retVal;

        if (svid == svlanMemConf.vs_svid)
        {
            svidx = i;
            break;
        }
    }

    if (0xFFFF == svidx)
        return L2SW_ERR_SVLAN_ENTRY_NOT_FOUND;

    for (i = 0; i <= L2SW_SP2CMAX; i++)
    {
        if ((retVal = l2sw_getAsicSvlanSP2CConf(units, i, &svlanSP2CConf)) != L2SW_ERR_OK)
            return retVal;

        if ( (svlanSP2CConf.svidx == svidx) && (svlanSP2CConf.dstport == dst_port) && (svlanSP2CConf.valid == 1) )
        {
            svlanSP2CConf.valid     = 0;
            svlanSP2CConf.vid       = 0;
            svlanSP2CConf.svidx     = 0;
            svlanSP2CConf.dstport   = 0;

            if ((retVal = l2sw_setAsicSvlanSP2CConf(units, i, &svlanSP2CConf)) != L2SW_ERR_OK)
                return retVal;
            return L2SW_ERR_OK;
        }

    }

    return L2SW_ERR_OUT_OF_RANGE;
}

#if 0
static l2sw_api_ret_t _l2sw_svlan_lookupType_set(l2sw_uint8 units, l2sw_svlan_lookupType_t type)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= SVLAN_LOOKUP_END)
        return L2SW_ERR_CHIP_NOT_SUPPORTED;


    svlan_lookupType[units] = type;

    retVal = l2sw_setAsicSvlanLookupType(units, (l2sw_uint32)type);

    return retVal;
}

static l2sw_api_ret_t _l2sw_svlan_lookupType_get(l2sw_uint8 units, l2sw_svlan_lookupType_t *pType)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pType)
        return L2SW_ERR_NULL_POINTER;

    retVal = l2sw_getAsicSvlanLookupType(units, pType);

    svlan_lookupType[units] = *pType;

    return retVal;
}

#endif

static l2sw_api_ret_t _l2sw_svlan_trapPri_set(l2sw_uint8 units, l2sw_pri_t priority)
{
    l2sw_api_ret_t   retVal;

    L2SW_CHK_INIT_STATE(units);

    if(priority > L2SW_PRIMAX)
        return L2SW_ERR_OUT_OF_RANGE;

    retVal = l2sw_setAsicSvlanTrapPriority(units, priority);

    return retVal;
}

static l2sw_api_ret_t _l2sw_svlan_trapPri_get(l2sw_uint8 units, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t   retVal;

    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPriority)
        return L2SW_ERR_NULL_POINTER;

    retVal = l2sw_getAsicSvlanTrapPriority(units, pPriority);

    return retVal;
}   /* end of l2sw_svlan_trapPri_get */


/* Function Name:
 *      l2sw_svlan_init
 * Description:
 *      Initialize SVLAN Configuration
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Ether type of S-tag in 802.1ad is 0x88a8 and there are existed ether type 0x9100 and 0x9200 for Q-in-Q SLAN design.
 *      User can set mathced ether type as service provider supported protocol.
 */
l2sw_api_ret_t l2sw_svlan_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_init(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_servicePort_add
 * Description:
 *      Add one service port in the specified device
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
l2sw_api_ret_t l2sw_svlan_servicePort_add(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_servicePort_add(units, port);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_servicePort_get
 * Description:
 *      Get service ports in the specified device.
 * Input:
 *      None
 * Output:
 *      pSvlan_portmask - pointer buffer of svlan ports.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
l2sw_api_ret_t l2sw_svlan_servicePort_get(l2sw_uint8 units, l2sw_portmask_t *pSvlan_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_servicePort_get(units, pSvlan_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_servicePort_del
 * Description:
 *      Delete one service port in the specified device
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API is removing SVLAN service port in the specified device.
 */
l2sw_api_ret_t l2sw_svlan_servicePort_del(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_servicePort_del(units, port);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_tpidEntry_set
 * Description:
 *      Configure accepted S-VLAN ether type.
 * Input:
 *      svlan_tag_id - Ether type of S-tag frame parsing in uplink ports.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      Ether type of S-tag in 802.1ad is 0x88a8 and there are existed ether type 0x9100 and 0x9200 for Q-in-Q SLAN design.
 *      User can set mathced ether type as service provider supported protocol.
 */
l2sw_api_ret_t l2sw_svlan_tpidEntry_set(l2sw_uint8 units, l2sw_svlan_tpid_t svlan_tag_id)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_tpidEntry_set(units, svlan_tag_id);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_tpidEntry_get
 * Description:
 *      Get accepted S-VLAN ether type setting.
 * Input:
 *      None
 * Output:
 *      pSvlan_tag_id -  Ether type of S-tag frame parsing in uplink ports.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      This API is setting which port is connected to provider switch. All frames receiving from this port must
 *      contain accept SVID in S-tag field.
 */
l2sw_api_ret_t l2sw_svlan_tpidEntry_get(l2sw_uint8 units, l2sw_svlan_tpid_t *pSvlan_tag_id)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_tpidEntry_get(units, pSvlan_tag_id);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_priorityRef_set
 * Description:
 *      Set S-VLAN upstream priority reference setting.
 * Input:
 *      ref - reference selection parameter.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 * Note:
 *      The API can set the upstream SVLAN tag priority reference source. The related priority
 *      sources are as following:
 *      - REF_INTERNAL_PRI,
 *      - REF_CTAG_PRI,
 *      - REF_SVLAN_PRI,
 *      - REF_PB_PRI.
 */
l2sw_api_ret_t l2sw_svlan_priorityRef_set(l2sw_uint8 units, l2sw_svlan_pri_ref_t ref)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_priorityRef_set(units, ref);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_priorityRef_get
 * Description:
 *      Get S-VLAN upstream priority reference setting.
 * Input:
 *      None
 * Output:
 *      pRef - reference selection parameter.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      The API can get the upstream SVLAN tag priority reference source. The related priority
 *      sources are as following:
 *      - REF_INTERNAL_PRI,
 *      - REF_CTAG_PRI,
 *      - REF_SVLAN_PRI,
 *      - REF_PB_PRI
 */
l2sw_api_ret_t l2sw_svlan_priorityRef_get(l2sw_uint8 units, l2sw_svlan_pri_ref_t *pRef)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_priorityRef_get(units, pRef);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_memberPortEntry_set
 * Description:
 *      Configure system SVLAN member content
 * Input:
 *      svid - SVLAN id
 *      psvlan_cfg - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_SVLAN_TABLE_FULL - SVLAN configuration is full.
 * Note:
 *      The API can set system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped by default setup.
 *      - l2sw_svlan_memberCfg_t->svid is SVID of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->memberport is member port mask of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->fid is filtering database of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->priority is priority of SVLAN member configuration.
 */
l2sw_api_ret_t l2sw_svlan_memberPortEntry_set(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_memberPortEntry_set(units, svid, pSvlan_cfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_memberPortEntry_get
 * Description:
 *      Get SVLAN member Configure.
 * Input:
 *      svid - SVLAN id
 * Output:
 *      pSvlan_cfg - SVLAN member configuration
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can get system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped.
 */
l2sw_api_ret_t l2sw_svlan_memberPortEntry_get(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_memberPortEntry_get(units, svid, pSvlan_cfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_memberPortEntry_adv_set
 * Description:
 *      Configure system SVLAN member by index
 * Input:
 *      idx         - Index (0 ~ 63)
 *      psvlan_cfg  - SVLAN member configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_SVLAN_TABLE_FULL - SVLAN configuration is full.
 * Note:
 *      The API can set system 64 accepted s-tag frame format by index.
 *      - l2sw_svlan_memberCfg_t->svid is SVID of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->memberport is member port mask of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->fid is filtering database of SVLAN member configuration.
 *      - l2sw_svlan_memberCfg_t->priority is priority of SVLAN member configuration.
 */
l2sw_api_ret_t l2sw_svlan_memberPortEntry_adv_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_memberPortEntry_adv_set(units, idx, pSvlan_cfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_memberPortEntry_adv_get
 * Description:
 *      Get SVLAN member Configure by index.
 * Input:
 *      idx         - Index (0 ~ 63)
 * Output:
 *      pSvlan_cfg  - SVLAN member configuration
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can get system 64 accepted s-tag frame format. Only 64 SVID S-tag frame will be accpeted
 *      to receiving from uplink ports. Other SVID S-tag frame or S-untagged frame will be droped.
 */
l2sw_api_ret_t l2sw_svlan_memberPortEntry_adv_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_svlan_memberCfg_t *pSvlan_cfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_memberPortEntry_adv_get(units, idx, pSvlan_cfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_defaultSvlan_set
 * Description:
 *      Configure default egress SVLAN.
 * Input:
 *      port - Source port
 *      svid - SVLAN id
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_INPUT                    - Invalid input parameter.
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 * Note:
 *      The API can set port n S-tag format index while receiving frame from port n
 *      is transmit through uplink port with s-tag field
 */
l2sw_api_ret_t l2sw_svlan_defaultSvlan_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t svid)
{

    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_defaultSvlan_set(units, port, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_defaultSvlan_get
 * Description:
 *      Get the configure default egress SVLAN.
 * Input:
 *      port - Source port
 * Output:
 *      pSvid - SVLAN VID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get port n S-tag format index while receiving frame from port n
 *      is transmit through uplink port with s-tag field
 */
l2sw_api_ret_t l2sw_svlan_defaultSvlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pSvid)
{

    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_defaultSvlan_get(units, port, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_c2s_add
 * Description:
 *      Configure SVLAN C2S table
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 *      svid - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port ID.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can set system C2S configuration. ASIC will check upstream's VID and assign related
 *      SVID to mathed packet. There are 128 SVLAN C2S configurations.
 */
l2sw_api_ret_t l2sw_svlan_c2s_add(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t svid)
{

    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_c2s_add(units, vid, src_port, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_c2s_del
 * Description:
 *      Delete one C2S entry
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 *      svid - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_VLAN_VID         - Invalid VID parameter.
 *      L2SW_ERR_PORT_ID          - Invalid port ID.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete system C2S configuration. There are 128 SVLAN C2S configurations.
 */
l2sw_api_ret_t l2sw_svlan_c2s_del(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_c2s_del(units, vid, src_port);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_c2s_get
 * Description:
 *      Get configure SVLAN C2S table
 * Input:
 *      vid - VLAN ID
 *      src_port - Ingress Port
 * Output:
 *      pSvid - SVLAN ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port ID.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *     The API can get system C2S configuration. There are 128 SVLAN C2S configurations.
 */
l2sw_api_ret_t l2sw_svlan_c2s_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_port_t src_port, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_c2s_get(units, vid, src_port, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_untag_action_set
 * Description:
 *      Configure Action of downstream UnStag packet
 * Input:
 *      action  - Action for UnStag
 *      svid    - The SVID assigned to UnStag packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of downstream Un-Stag packet. A SVID assigned
 *      to the un-stag is also supported by this API. The parameter of svid is
 *      only referenced when the action is set to UNTAG_ASSIGN
 */
l2sw_api_ret_t l2sw_svlan_untag_action_set(l2sw_uint8 units, l2sw_svlan_untag_action_t action, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_untag_action_set(units, action, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_untag_action_get
 * Description:
 *      Get Action of downstream UnStag packet
 * Input:
 *      None
 * Output:
 *      pAction  - Action for UnStag
 *      pSvid    - The SVID assigned to UnStag packet
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can Get action of downstream Un-Stag packet. A SVID assigned
 *      to the un-stag is also retrieved by this API. The parameter pSvid is
 *      only refernced when the action is UNTAG_ASSIGN
 */
l2sw_api_ret_t l2sw_svlan_untag_action_get(l2sw_uint8 units, l2sw_svlan_untag_action_t *pAction, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_untag_action_get(units, pAction, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_unmatch_action_set
 * Description:
 *      Configure Action of downstream Unmatch packet
 * Input:
 *      action  - Action for Unmatch
 *      svid    - The SVID assigned to Unmatch packet
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of downstream Un-match packet. A SVID assigned
 *      to the un-match is also supported by this API. The parameter od svid is
 *      only refernced when the action is set to UNMATCH_ASSIGN
 */
l2sw_api_ret_t l2sw_svlan_unmatch_action_set(l2sw_uint8 units, l2sw_svlan_unmatch_action_t action, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_unmatch_action_set(units, action, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_unmatch_action_get
 * Description:
 *      Get Action of downstream Unmatch packet
 * Input:
 *      None
 * Output:
 *      pAction  - Action for Unmatch
 *      pSvid    - The SVID assigned to Unmatch packet
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can Get action of downstream Un-match packet. A SVID assigned
 *      to the un-match is also retrieved by this API. The parameter pSvid is
 *      only refernced when the action is UNMATCH_ASSIGN
 */
l2sw_api_ret_t l2sw_svlan_unmatch_action_get(l2sw_uint8 units, l2sw_svlan_unmatch_action_t *pAction, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_unmatch_action_get(units, pAction, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_unassign_action_set
 * Description:
 *      Configure Action of upstream without svid assign action
 * Input:
 *      action  - Action for Un-assign
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can configure action of upstream Un-assign svid packet. If action is not
 *      trap to CPU, the port-based SVID sure be assign as system need
 */
l2sw_api_ret_t l2sw_svlan_unassign_action_set(l2sw_uint8 units, l2sw_svlan_unassign_action_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_unassign_action_set(units, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_unassign_action_get
 * Description:
 *      Get action of upstream without svid assignment
 * Input:
 *      None
 * Output:
 *      pAction  - Action for Un-assign
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_svlan_unassign_action_get(l2sw_uint8 units, l2sw_svlan_unassign_action_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_unassign_action_get(units, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_dmac_vidsel_set
 * Description:
 *      Set DMAC CVID selection
 * Input:
 *      port    - Port
 *      enable  - state of DMAC CVID Selection
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      This API can set DMAC CVID Selection state
 */
l2sw_api_ret_t l2sw_svlan_dmac_vidsel_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_dmac_vidsel_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_dmac_vidsel_get
 * Description:
 *      Get DMAC CVID selection
 * Input:
 *      port    - Port
 * Output:
 *      pEnable - state of DMAC CVID Selection
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      This API can get DMAC CVID Selection state
 */
l2sw_api_ret_t l2sw_svlan_dmac_vidsel_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_dmac_vidsel_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_ipmc2s_add
 * Description:
 *      add ip multicast address to SVLAN
 * Input:
 *      svid    - SVLAN VID
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can set IP mutlicast to SVID configuration. If upstream packet is IPv4 multicast
 *      packet and DIP is matched MC2S configuration, ASIC will assign egress SVID to the packet.
 *      There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_ipmc2s_add(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk,l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_ipmc2s_add(units, ipmc, ipmcMsk, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_ipmc2s_del
 * Description:
 *      delete ip multicast address to SVLAN
 * Input:
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete IP mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_ipmc2s_del(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_ipmc2s_del(units, ipmc, ipmcMsk);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_ipmc2s_get
 * Description:
 *      Get ip multicast address to SVLAN
 * Input:
 *      ipmc    - ip multicast address
 *      ipmcMsk - ip multicast mask
 * Output:
 *      pSvid - SVLAN VID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *      The API can get IP mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_ipmc2s_get(l2sw_uint8 units, ipaddr_t ipmc, ipaddr_t ipmcMsk, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_ipmc2s_get(units, ipmc, ipmcMsk, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_svlan_l2mc2s_add
 * Description:
 *      Add L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 *      svid    - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_SVLAN_VID                - Invalid SVLAN VID parameter.
 *      L2SW_ERR_SVLAN_ENTRY_NOT_FOUND    - specified svlan entry not found.
 *      L2SW_ERR_OUT_OF_RANGE             - input out of range.
 *      L2SW_ERR_INPUT                    - Invalid input parameters.
 * Note:
 *      The API can set L2 Mutlicast to SVID configuration. If upstream packet is L2 multicast
 *      packet and DMAC is matched, ASIC will assign egress SVID to the packet. There are 32
 *      SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_l2mc2s_add(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t svid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_l2mc2s_add(units, mac, macMsk, svid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_l2mc2s_del
 * Description:
 *      delete L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_SVLAN_VID        - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can delete Mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_l2mc2s_del(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_l2mc2s_del(units, mac, macMsk);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_l2mc2s_get
 * Description:
 *      Get L2 multicast address to SVLAN
 * Input:
 *      mac     - L2 multicast address
 *      macMsk  - L2 multicast address mask
 * Output:
 *      pSvid - SVLAN VID
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can get L2 mutlicast to SVID configuration. There are 32 SVLAN multicast configurations for IP and L2 multicast.
 */
l2sw_api_ret_t l2sw_svlan_l2mc2s_get(l2sw_uint8 units, l2sw_mac_t mac, l2sw_mac_t macMsk, l2sw_vlan_t *pSvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_l2mc2s_get(units, mac, macMsk, pSvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_sp2c_add
 * Description:
 *      Add system SP2C configuration
 * Input:
 *      cvid        - VLAN ID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 *      svid        - SVLAN VID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can add SVID & Destination Port to CVLAN configuration. The downstream frames with assigned
 *      SVID will be add C-tag with assigned CVID if the output port is the assigned destination port.
 *      There are 128 SP2C configurations.
 */
l2sw_api_ret_t l2sw_svlan_sp2c_add(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t cvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_sp2c_add(units, svid, dst_port, cvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_sp2c_get
 * Description:
 *      Get configure system SP2C content
 * Input:
 *      svid        - SVLAN VID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 * Output:
 *      pCvid - VLAN ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 * Note:
 *     The API can get SVID & Destination Port to CVLAN configuration. There are 128 SP2C configurations.
 */
l2sw_api_ret_t l2sw_svlan_sp2c_get(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port, l2sw_vlan_t *pCvid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_sp2c_get(units, svid, dst_port, pCvid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_sp2c_del
 * Description:
 *      Delete system SP2C configuration
 * Input:
 *      svid        - SVLAN VID
 *      dst_port    - Destination port of SVLAN to CVLAN configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *      The API can delete SVID & Destination Port to CVLAN configuration. There are 128 SP2C configurations.
 */
l2sw_api_ret_t l2sw_svlan_sp2c_del(l2sw_uint8 units, l2sw_vlan_t svid, l2sw_port_t dst_port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_sp2c_del(units, svid, dst_port);
    L2SW_API_UNLOCK(units);

    return retVal;
}

#if 0
/* Function Name:
 *      l2sw_svlan_lookupType_set
 * Description:
 *      Set lookup type of SVLAN
 * Input:
 *      type        - lookup type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_svlan_lookupType_set(l2sw_uint8 units, l2sw_svlan_lookupType_t type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_lookupType_set(units, type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_svlan_lookupType_get
 * Description:
 *      Get lookup type of SVLAN
 * Input:
 *      pType       - lookup type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 * Note:
 *      none
 */
l2sw_api_ret_t l2sw_svlan_lookupType_get(l2sw_uint8 units, l2sw_svlan_lookupType_t *pType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_lookupType_get(units, pType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

#endif

/* Function Name:
 *      l2sw_svlan_trapPri_set
 * Description:
 *      Set svlan trap priority
 * Input:
 *      priority - priority for trap packets
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_QOS_INT_PRIORITY
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_svlan_trapPri_set(l2sw_uint8 units, l2sw_pri_t priority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_trapPri_set(units, priority);
    L2SW_API_UNLOCK(units);

    return retVal;
}   /* end of l2sw_svlan_trapPri_set */

/* Function Name:
 *      l2sw_svlan_trapPri_get
 * Description:
 *      Get svlan trap priority
 * Input:
 *      None
 * Output:
 *      pPriority - priority for trap packets
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NULL_POINTER - input parameter may be null pointer
 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_svlan_trapPri_get(l2sw_uint8 units, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_trapPri_get(units, pPriority);
    L2SW_API_UNLOCK(units);

    return retVal;
}   /* end of l2sw_svlan_trapPri_get */


/*Don't lock mutex in following API*/

/* Function Name:
 *      l2sw_svlan_checkAndCreateMbr
 * Description:
 *      Check and create Member configuration and return index
 * Input:
 *      vid  - VLAN id.
 * Output:
 *      pIndex  - Member configuration index
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_VLAN_VID     - Invalid VLAN ID.
 *      L2SW_ERR_TBL_FULL     - Member Configuration table full
 * Note:
 *
 */
l2sw_api_ret_t l2sw_svlan_checkAndCreateMbr(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 svidx;
    l2sw_uint32 empty_idx = 0xFFFF;
    l2sw_svlan_memconf_t svlan_cfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~4095 */
    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pIndex)
        return L2SW_ERR_NULL_POINTER;

    /* Search exist entry */
    for (svidx = 0; svidx <= L2SW_SVIDXMAX; svidx++)
    {
        if(svlan_mbrCfgUsage[units][svidx] == L2SW_TRUE)
        {
            if(svlan_mbrCfgVid[units][svidx] == vid)
            {
                /* Found! return index */
                *pIndex = svidx;
                return L2SW_ERR_OK;
            }
        }
        else if(empty_idx == 0xFFFF)
        {
            empty_idx = svidx;
        }

    }

    if(empty_idx == 0xFFFF)
    {
        /* No empty index */
        return L2SW_ERR_TBL_FULL;
    }

    svlan_mbrCfgUsage[units][empty_idx] = L2SW_TRUE;
    svlan_mbrCfgVid[units][empty_idx] = vid;

    MEMSET(&svlan_cfg, 0, sizeof(l2sw_svlan_memconf_t));

    svlan_cfg.vs_svid = vid;
    /*for create check*/
    if(vid == 0)
    {
        svlan_cfg.vs_efid = 1;
    }

    if((retVal = l2sw_setAsicSvlanMemberConfiguration(units, empty_idx, &svlan_cfg)) != L2SW_ERR_OK)
        return retVal;

    *pIndex = empty_idx;
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_svlan_s2c_add_cpri_set(l2sw_uint8 units, l2sw_cpri_t cpri_type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (cpri_type >= SVLAN_CPRI_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_svlan_s2c_add_cpri_set(units, (l2sw_uint32)cpri_type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_set
 * Description:
 *      Set the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      cpri_type   - CPRI value in CTAG
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *
 */
l2sw_api_ret_t l2swu_svlan_s2c_add_cpri_set(l2sw_uint8 units, l2sw_cpri_t cpri_type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_s2c_add_cpri_set(units, cpri_type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

static l2sw_api_ret_t _l2sw_svlan_s2c_add_cpri_get(l2sw_uint8 units, l2sw_cpri_t *pcpri_type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pcpri_type)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_svlan_s2c_add_cpri_get(units, (l2sw_uint32 *)pcpri_type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_svlan_s2c_add_cpri_get
 * Description:
 *      Get the source of priority field in the new c-tag of S2C downstream
 * Input:
 *      None
 * Output:
 *      pcpri_type   - CPRI value in CTAG
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_SVLAN_VID    - Invalid SVLAN VID parameter.
 *      L2SW_ERR_OUT_OF_RANGE - input out of range.
 * Note:
 *
 */
l2sw_api_ret_t l2swu_svlan_s2c_add_cpri_get(l2sw_uint8 units, l2sw_cpri_t *pcpri_type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_svlan_s2c_add_cpri_get(units, pcpri_type);
    L2SW_API_UNLOCK(units);

    return retVal;
}
