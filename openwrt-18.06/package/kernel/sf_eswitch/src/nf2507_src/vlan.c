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
#include "vlan.h"
#include "rate.h"
#include "sal_string.h"
#include "l2sw_asicdrv_vlan.h"
#include "l2sw_asicdrv_dot1x.h"

typedef enum vlan_mbrCfgType_e
{
    MBRCFG_UNUSED = 0,
    MBRCFG_USED_BY_VLAN,
    MBRCFG_END
}vlan_mbrCfgType_t;

static l2sw_vlan_t           vlan_mbrCfgVid[L2SW_MAX_NUM_OF_UNIT][L2SW_CVIDXNO];
static vlan_mbrCfgType_t    vlan_mbrCfgUsage[L2SW_MAX_NUM_OF_UNIT][L2SW_CVIDXNO];

static l2sw_api_ret_t _l2sw_vlan_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_user_vlan4kentry vlan4K;
    l2sw_vlanconfiguser vlanMC;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Clean Database */
    MEMSET(vlan_mbrCfgVid, 0x00, sizeof(l2sw_vlan_t) * L2SW_CVIDXNO * L2SW_MAX_NUM_OF_UNIT);
    MEMSET(vlan_mbrCfgUsage, 0x00, sizeof(vlan_mbrCfgType_t) * L2SW_CVIDXNO * L2SW_MAX_NUM_OF_UNIT);

    /* clean 32 VLAN member configuration */
    for (i = 0; i <= L2SW_CVIDXMAX; i++)
    {
        vlanMC.evid = 0;
        vlanMC.mbr = 0;
        vlanMC.fid_msti = 0;
        vlanMC.envlanpol = 0;
        vlanMC.meteridx = 0;
        vlanMC.vbpen = 0;
        vlanMC.vbpri = 0;
        if ((retVal = l2sw_setAsicVlanMemberConfig(units, i, &vlanMC)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Set a default VLAN with vid 1 to 4K table for all ports */
    MEMSET(&vlan4K, 0, sizeof(l2sw_user_vlan4kentry));
    vlan4K.vid = 1;
    vlan4K.mbr = L2SW_PHY_PORTMASK_ALL(units);
    vlan4K.untag = L2SW_PHY_PORTMASK_ALL(units);
    vlan4K.fid_msti = 0;
    if ((retVal = l2sw_setAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
        return retVal;

    /* Also set the default VLAN to 32 member configuration index 0 */
    MEMSET(&vlanMC, 0, sizeof(l2sw_vlanconfiguser));
    vlanMC.evid = 1;
    vlanMC.mbr = L2SW_PHY_PORTMASK_ALL(units);
    vlanMC.fid_msti = 0;
    if ((retVal = l2sw_setAsicVlanMemberConfig(units, 0, &vlanMC)) != L2SW_ERR_OK)
            return retVal;

    /* Set all ports PVID to default VLAN and tag-mode to original */
    L2SW_SCAN_ALL_PHY_PORTMASK(units, i)
    {
        if ((retVal = l2sw_setAsicVlanPortBasedVID(units, i, 0, 0)) != L2SW_ERR_OK)
            return retVal;
        if ((retVal = l2sw_setAsicVlanEgressTagMode(units, i, EG_TAG_MODE_ORI)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Updata Databse */
    vlan_mbrCfgUsage[units][0] = MBRCFG_USED_BY_VLAN;
    vlan_mbrCfgVid[units][0] = 1;

    /* Enable Ingress filter */
    L2SW_SCAN_ALL_PHY_PORTMASK(units, i)
    {
        if ((retVal = l2sw_setAsicVlanIngressFilter(units, i, L2SW_ENABLED)) != L2SW_ERR_OK)
            return retVal;
    }

    /* enable VLAN */
    if ((retVal = l2sw_setAsicVlanFilter(units, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 phyMbrPmask;
    l2sw_uint32 phyUntagPmask;
    l2sw_user_vlan4kentry vlan4K;
    l2sw_vlanconfiguser vlanMC;
    l2sw_uint32 idx;
    l2sw_uint32 empty_index = 0xffff;
    l2sw_uint32 update_evid = 0;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~8191 */
    if (vid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pVlanCfg)
        return L2SW_ERR_NULL_POINTER;

    /* Check port mask valid */
    L2SW_CHK_PORTMASK_VALID(units, &(pVlanCfg->mbr));

    if (vid <= L2SW_VIDMAX)
    {
        /* Check untag port mask valid */
        L2SW_CHK_PORTMASK_VALID(units, &(pVlanCfg->untag));
    }

    /* IVL_EN */
    if(pVlanCfg->ivl_en >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    /* fid must be 0~15 */
    if(pVlanCfg->fid_msti > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    /* Policing */
    if(pVlanCfg->envlanpol >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    /* Meter ID */
    if(pVlanCfg->meteridx > L2SW_MAX_METER_ID)
        return L2SW_ERR_INPUT;

    /* VLAN based priority */
    if(pVlanCfg->vbpen >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    /* Priority */
    if(pVlanCfg->vbpri > L2SW_PRIMAX)
        return L2SW_ERR_INPUT;

    /* Get physical port mask */
    if(l2sw_switch_portmask_L2P_get(units, &(pVlanCfg->mbr), &phyMbrPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    if(l2sw_switch_portmask_L2P_get(units, &(pVlanCfg->untag), &phyUntagPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    if (vid <= L2SW_VIDMAX)
    {
        /* update 4K table */
        MEMSET(&vlan4K, 0, sizeof(l2sw_user_vlan4kentry));
        vlan4K.vid = vid;

        vlan4K.mbr    = (phyMbrPmask & 0xFFFF);
        vlan4K.untag  = (phyUntagPmask & 0xFFFF);

        vlan4K.ivl_svl      = pVlanCfg->ivl_en;
        vlan4K.fid_msti     = pVlanCfg->fid_msti;
        vlan4K.envlanpol    = pVlanCfg->envlanpol;
        vlan4K.meteridx     = pVlanCfg->meteridx;
        vlan4K.vbpen        = pVlanCfg->vbpen;
        vlan4K.vbpri        = pVlanCfg->vbpri;

        if ((retVal = l2sw_setAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
            return retVal;

        /* Update Member configuration if exist */
        for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[units][idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[units][idx] == vid)
                {
                    /* Found! Update */
                    if(phyMbrPmask == 0x00)
                    {
                        /* Member port = 0x00, delete this VLAN from Member Configuration */
                        MEMSET(&vlanMC, 0x00, sizeof(l2sw_vlanconfiguser));
                        if ((retVal = l2sw_setAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
                            return retVal;

                        /* Clear Database */
                        vlan_mbrCfgUsage[units][idx] = MBRCFG_UNUSED;
                        vlan_mbrCfgVid[units][idx]   = 0;
                    }
                    else
                    {
                        /* Normal VLAN config, update to member configuration */
                        vlanMC.evid = vid;
                        vlanMC.mbr = vlan4K.mbr;
                        vlanMC.fid_msti = vlan4K.fid_msti;
                        vlanMC.meteridx = vlan4K.meteridx;
                        vlanMC.envlanpol= vlan4K.envlanpol;
                        vlanMC.vbpen = vlan4K.vbpen;
                        vlanMC.vbpri = vlan4K.vbpri;
                        if ((retVal = l2sw_setAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
                            return retVal;
                    }

                    break;
                }
            }
        }
    }
    else
    {
        /* vid > 4095 */
        for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[units][idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[units][idx] == vid)
                {
                    /* Found! Update */
                    if(phyMbrPmask == 0x00)
                    {
                        /* Member port = 0x00, delete this VLAN from Member Configuration */
                        MEMSET(&vlanMC, 0x00, sizeof(l2sw_vlanconfiguser));
                        if ((retVal = l2sw_setAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
                            return retVal;

                        /* Clear Database */
                        vlan_mbrCfgUsage[units][idx] = MBRCFG_UNUSED;
                        vlan_mbrCfgVid[units][idx]   = 0;
                    }
                    else
                    {
                        /* Normal VLAN config, update to member configuration */
                        vlanMC.evid = vid;
                        vlanMC.mbr = phyMbrPmask;
                        vlanMC.fid_msti = pVlanCfg->fid_msti;
                        vlanMC.meteridx = pVlanCfg->meteridx;
                        vlanMC.envlanpol= pVlanCfg->envlanpol;
                        vlanMC.vbpen = pVlanCfg->vbpen;
                        vlanMC.vbpri = pVlanCfg->vbpri;
                        if ((retVal = l2sw_setAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
                            return retVal;

                        break;
                    }

                    update_evid = 1;
                }
            }

            if(vlan_mbrCfgUsage[units][idx] == MBRCFG_UNUSED)
            {
                if(0xffff == empty_index)
                    empty_index = idx;
            }
        }

        /* doesn't find out same EVID entry and there is empty index in member configuration */
        if( (phyMbrPmask != 0x00) && (update_evid == 0) && (empty_index != 0xFFFF) )
        {
            vlanMC.evid = vid;
            vlanMC.mbr = phyMbrPmask;
            vlanMC.fid_msti = pVlanCfg->fid_msti;
            vlanMC.meteridx = pVlanCfg->meteridx;
            vlanMC.envlanpol= pVlanCfg->envlanpol;
            vlanMC.vbpen = pVlanCfg->vbpen;
            vlanMC.vbpri = pVlanCfg->vbpri;
            if ((retVal = l2sw_setAsicVlanMemberConfig(units, empty_index, &vlanMC)) != L2SW_ERR_OK)
                return retVal;

            vlan_mbrCfgUsage[units][empty_index] = MBRCFG_USED_BY_VLAN;
            vlan_mbrCfgVid[units][empty_index] = vid;

        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 phyMbrPmask;
    l2sw_uint32 phyUntagPmask;
    l2sw_user_vlan4kentry vlan4K;
    l2sw_vlanconfiguser vlanMC;
    l2sw_uint32 idx;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~8191 */
    if (vid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pVlanCfg)
        return L2SW_ERR_NULL_POINTER;

    if (vid <= L2SW_VIDMAX)
    {
        vlan4K.vid = vid;

        if ((retVal = l2sw_getAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
            return retVal;

        phyMbrPmask   = vlan4K.mbr;
        phyUntagPmask = vlan4K.untag;
        if(l2sw_switch_portmask_P2L_get(units, phyMbrPmask, &(pVlanCfg->mbr)) != L2SW_ERR_OK)
            return L2SW_ERR_FAILED;

        if(l2sw_switch_portmask_P2L_get(units, phyUntagPmask, &(pVlanCfg->untag)) != L2SW_ERR_OK)
            return L2SW_ERR_FAILED;

        pVlanCfg->ivl_en    = vlan4K.ivl_svl;
        pVlanCfg->fid_msti  = vlan4K.fid_msti;
        pVlanCfg->envlanpol = vlan4K.envlanpol;
        pVlanCfg->meteridx  = vlan4K.meteridx;
        pVlanCfg->vbpen     = vlan4K.vbpen;
        pVlanCfg->vbpri     = vlan4K.vbpri;
    }
    else
    {
        for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[units][idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[units][idx] == vid)
                {
                    if ((retVal = l2sw_getAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
                        return retVal;

                    phyMbrPmask   = vlanMC.mbr;
                    if(l2sw_switch_portmask_P2L_get(units, phyMbrPmask, &(pVlanCfg->mbr)) != L2SW_ERR_OK)
                        return L2SW_ERR_FAILED;

                    pVlanCfg->untag.bits[0] = 0;
                    pVlanCfg->ivl_en    = 0;
                    pVlanCfg->fid_msti  = vlanMC.fid_msti;
                    pVlanCfg->envlanpol = vlanMC.envlanpol;
                    pVlanCfg->meteridx  = vlanMC.meteridx;
                    pVlanCfg->vbpen     = vlanMC.vbpen;
                    pVlanCfg->vbpri     = vlanMC.vbpri;
                }
            }
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_egrFilterEnable_set(l2sw_uint8 units, l2sw_enable_t egrFilter)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(egrFilter >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    /* enable VLAN */
    if ((retVal = l2sw_setAsicVlanFilter(units, (l2sw_uint32)egrFilter)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_egrFilterEnable_get(l2sw_uint8 units, l2sw_enable_t *pEgrFilter)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 state;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEgrFilter)
        return L2SW_ERR_NULL_POINTER;

    /* enable VLAN */
    if ((retVal = l2sw_getAsicVlanFilter(units, &state)) != L2SW_ERR_OK)
        return retVal;

    *pEgrFilter = (l2sw_enable_t)state;
    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_mbrCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg)
{
    l2sw_api_ret_t           retVal;
    l2sw_uint32              phyMbrPmask;
    l2sw_vlanconfiguser mbrCfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error check */
    if(pMbrcfg == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(idx > L2SW_CVIDXMAX)
        return L2SW_ERR_INPUT;

    if(pMbrcfg->evid > L2SW_EVIDMAX)
        return L2SW_ERR_INPUT;

    if(pMbrcfg->fid_msti > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if(pMbrcfg->envlanpol >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pMbrcfg->meteridx > L2SW_MAX_METER_ID)
        return L2SW_ERR_FILTER_METER_ID;

    if(pMbrcfg->vbpen >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pMbrcfg->vbpri > L2SW_PRIMAX)
        return L2SW_ERR_QOS_INT_PRIORITY;

    /* Check port mask valid */
    L2SW_CHK_PORTMASK_VALID(units, &(pMbrcfg->mbr));

    mbrCfg.evid         = pMbrcfg->evid;
    mbrCfg.fid_msti     = pMbrcfg->fid_msti;
    mbrCfg.envlanpol    = pMbrcfg->envlanpol;
    mbrCfg.meteridx     = pMbrcfg->meteridx;
    mbrCfg.vbpen        = pMbrcfg->vbpen;
    mbrCfg.vbpri        = pMbrcfg->vbpri;

    if(l2sw_switch_portmask_L2P_get(units, &(pMbrcfg->mbr), &phyMbrPmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    mbrCfg.mbr = phyMbrPmask;

    if ((retVal = l2sw_setAsicVlanMemberConfig(units, idx, &mbrCfg)) != L2SW_ERR_OK)
        return retVal;

    /* Update Database */
    if( (mbrCfg.evid == 0) && (mbrCfg.mbr == 0) )
    {
        vlan_mbrCfgUsage[units][idx] = MBRCFG_UNUSED;
        vlan_mbrCfgVid[units][idx] = 0;
    }
    else
    {
        vlan_mbrCfgUsage[units][idx] = MBRCFG_USED_BY_VLAN;
        vlan_mbrCfgVid[units][idx] = mbrCfg.evid;
    }

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_vlan_mbrCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg)
{
    l2sw_api_ret_t           retVal;
    l2sw_uint32              phyMbrPmask;
    l2sw_vlanconfiguser mbrCfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error check */
    if(pMbrcfg == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(idx > L2SW_CVIDXMAX)
        return L2SW_ERR_INPUT;

    MEMSET(&mbrCfg, 0x00, sizeof(l2sw_vlanconfiguser));
    if ((retVal = l2sw_getAsicVlanMemberConfig(units, idx, &mbrCfg)) != L2SW_ERR_OK)
        return retVal;

    pMbrcfg->evid       = mbrCfg.evid;
    pMbrcfg->fid_msti   = mbrCfg.fid_msti;
    pMbrcfg->envlanpol  = mbrCfg.envlanpol;
    pMbrcfg->meteridx   = mbrCfg.meteridx;
    pMbrcfg->vbpen      = mbrCfg.vbpen;
    pMbrcfg->vbpri      = mbrCfg.vbpri;

    phyMbrPmask = mbrCfg.mbr;
    if(l2sw_switch_portmask_P2L_get(units, phyMbrPmask, &(pMbrcfg->mbr)) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_portPvid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t pvid, l2sw_pri_t priority)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 index;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    /* vid must be 0~8191 */
    if (pvid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* priority must be 0~7 */
    if (priority > L2SW_PRIMAX)
        return L2SW_ERR_VLAN_PRIORITY;

    if((retVal = l2sw_vlan_checkAndCreateMbr(units, pvid, &index)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicVlanPortBasedVID(units, l2sw_switch_port_L2P_get(units,port), index, priority)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_portPvid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pPvid, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 index, pri;
    l2sw_vlanconfiguser mbrCfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pPvid)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pPriority)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicVlanPortBasedVID(units, l2sw_switch_port_L2P_get(units,port), &index, &pri)) != L2SW_ERR_OK)
        return retVal;

    MEMSET(&mbrCfg, 0x00, sizeof(l2sw_vlanconfiguser));
    if ((retVal = l2sw_getAsicVlanMemberConfig(units, index, &mbrCfg)) != L2SW_ERR_OK)
        return retVal;

    *pPvid = mbrCfg.evid;
    *pPriority = pri;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_portIgrFilterEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t igr_filter)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (igr_filter >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicVlanIngressFilter(units, l2sw_switch_port_L2P_get(units, port), igr_filter)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_portIgrFilterEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pIgr_filter)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pIgr_filter)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicVlanIngressFilter(units, l2sw_switch_port_L2P_get(units, port), pIgr_filter)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_portAcceptFrameType_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t accept_frame_type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (accept_frame_type >= ACCEPT_FRAME_TYPE_END)
        return L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE;

    if ((retVal = l2sw_setAsicVlanAccpetFrameType(units, l2sw_switch_port_L2P_get(units, port), (l2sw_accframetype)accept_frame_type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_portAcceptFrameType_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t *pAccept_frame_type)
{
    l2sw_api_ret_t retVal;
    l2sw_accframetype   acc_frm_type;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pAccept_frame_type)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicVlanAccpetFrameType(units, l2sw_switch_port_L2P_get(units, port), &acc_frm_type)) != L2SW_ERR_OK)
        return retVal;

    *pAccept_frame_type = (l2sw_vlan_acceptFrameType_t)acc_frm_type;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_protoAndPortBasedVlan_add(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal, i;
    l2sw_uint32 exist, empty, used, index;
    l2sw_protocolgdatacfg ppb_data_cfg;
    l2sw_protocolvlancfg ppb_vlan_cfg;
    l2sw_provlan_frametype tmp;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pInfo)
        return L2SW_ERR_NULL_POINTER;

    if (pInfo->proto_type > L2SW_MAX_NUM_OF_PROTO_TYPE)
        return L2SW_ERR_OUT_OF_RANGE;

    if (pInfo->frame_type >= FRAME_TYPE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if (pInfo->cvid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    if (pInfo->cpri > L2SW_PRIMAX)
        return L2SW_ERR_VLAN_PRIORITY;

    exist = 0xFF;
    empty = 0xFF;
    for (i = L2SW_PROTOVLAN_GIDX_MAX; i >= 0; i--)
    {
        if ((retVal = l2sw_getAsicVlanProtocolBasedGroupData(units, i, &ppb_data_cfg)) != L2SW_ERR_OK)
            return retVal;
        tmp = pInfo->frame_type;
        if (ppb_data_cfg.etherType == pInfo->proto_type && ppb_data_cfg.frameType == tmp)
        {
            /*Already exist*/
            exist = i;
            break;
        }
        else if (ppb_data_cfg.etherType == 0 && ppb_data_cfg.frameType == 0)
        {
            /*find empty index*/
            empty = i;
        }
    }

    used = 0xFF;
    /*No empty and exist index*/
    if (0xFF == exist && 0xFF == empty)
        return L2SW_ERR_TBL_FULL;
    else if (exist<L2SW_PROTOVLAN_GROUPNO)
    {
       /*exist index*/
       used = exist;
    }
    else if (empty<L2SW_PROTOVLAN_GROUPNO)
    {
        /*No exist index, but have empty index*/
        ppb_data_cfg.frameType = pInfo->frame_type;
        ppb_data_cfg.etherType = pInfo->proto_type;
        if ((retVal = l2sw_setAsicVlanProtocolBasedGroupData(units, empty, &ppb_data_cfg)) != L2SW_ERR_OK)
            return retVal;
        used = empty;
    }
    else
        return L2SW_ERR_FAILED;

    if((retVal = l2sw_vlan_checkAndCreateMbr(units, pInfo->cvid, &index)) != L2SW_ERR_OK)
        return retVal;

    ppb_vlan_cfg.vlan_idx = index;
    ppb_vlan_cfg.valid = L2SW_TRUE;
    ppb_vlan_cfg.priority = pInfo->cpri;
    if ((retVal = l2sw_setAsicVlanPortAndProtocolBased(units, l2sw_switch_port_L2P_get(units, port), used, &ppb_vlan_cfg)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_protoAndPortBasedVlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i;
    l2sw_uint32 ppb_idx;
    l2sw_protocolgdatacfg ppb_data_cfg;
    l2sw_protocolvlancfg ppb_vlan_cfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (proto_type > L2SW_MAX_NUM_OF_PROTO_TYPE)
        return L2SW_ERR_OUT_OF_RANGE;

    if (frame_type >= FRAME_TYPE_END)
        return L2SW_ERR_OUT_OF_RANGE;

   ppb_idx = 0;

    for (i = 0; i<= L2SW_PROTOVLAN_GIDX_MAX; i++)
    {
        if ((retVal = l2sw_getAsicVlanProtocolBasedGroupData(units, i, &ppb_data_cfg)) != L2SW_ERR_OK)
            return retVal;

        if ( (ppb_data_cfg.frameType == (l2sw_provlan_frametype)frame_type) && (ppb_data_cfg.etherType == proto_type) )
        {
            ppb_idx = i;
            break;
        }
        else if (L2SW_PROTOVLAN_GIDX_MAX == i)
            return L2SW_ERR_TBL_FULL;
    }

    if ((retVal = l2sw_getAsicVlanPortAndProtocolBased(units, l2sw_switch_port_L2P_get(units, port), ppb_idx, &ppb_vlan_cfg)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_FALSE == ppb_vlan_cfg.valid)
        return L2SW_ERR_FAILED;

    pInfo->frame_type = frame_type;
    pInfo->proto_type = proto_type;
    pInfo->cvid = vlan_mbrCfgVid[units][ppb_vlan_cfg.vlan_idx];
    pInfo->cpri = ppb_vlan_cfg.priority;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_protoAndPortBasedVlan_del(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i, bUsed;
    l2sw_uint32 ppb_idx;
    l2sw_protocolgdatacfg ppb_data_cfg;
    l2sw_protocolvlancfg ppb_vlan_cfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (proto_type > L2SW_MAX_NUM_OF_PROTO_TYPE)
        return L2SW_ERR_OUT_OF_RANGE;

    if (frame_type >= FRAME_TYPE_END)
        return L2SW_ERR_OUT_OF_RANGE;

   ppb_idx = 0;

    for (i = 0; i<= L2SW_PROTOVLAN_GIDX_MAX; i++)
    {
        if ((retVal = l2sw_getAsicVlanProtocolBasedGroupData(units, i, &ppb_data_cfg)) != L2SW_ERR_OK)
            return retVal;

        if ( (ppb_data_cfg.frameType == (l2sw_provlan_frametype)frame_type) && (ppb_data_cfg.etherType == proto_type) )
        {
            ppb_idx = i;
            ppb_vlan_cfg.valid = L2SW_FALSE;
            ppb_vlan_cfg.vlan_idx = 0;
            ppb_vlan_cfg.priority = 0;
            if ((retVal = l2sw_setAsicVlanPortAndProtocolBased(units, l2sw_switch_port_L2P_get(units,port), ppb_idx, &ppb_vlan_cfg)) != L2SW_ERR_OK)
                return retVal;
        }
    }

    bUsed = L2SW_FALSE;
    L2SW_SCAN_ALL_PHY_PORTMASK(units, i)
    {
        if ((retVal = l2sw_getAsicVlanPortAndProtocolBased(units, i, ppb_idx, &ppb_vlan_cfg)) != L2SW_ERR_OK)
            return retVal;

        if (L2SW_TRUE == ppb_vlan_cfg.valid)
        {
            bUsed = L2SW_TRUE;
                break;
        }
    }

    if (L2SW_FALSE == bUsed) /*No Port use this PPB Index, Delete it*/
    {
        ppb_data_cfg.etherType=0;
        ppb_data_cfg.frameType=0;
        if ((retVal = l2sw_setAsicVlanProtocolBasedGroupData(units, ppb_idx, &ppb_data_cfg)) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_protoAndPortBasedVlan_delAll(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i, j, bUsed[4];
    l2sw_protocolgdatacfg ppb_data_cfg;
    l2sw_protocolvlancfg ppb_vlan_cfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    for (i = 0; i <= L2SW_PROTOVLAN_GIDX_MAX; i++)
    {
        ppb_vlan_cfg.valid = L2SW_FALSE;
        ppb_vlan_cfg.vlan_idx = 0;
        ppb_vlan_cfg.priority = 0;
        if ((retVal = l2sw_setAsicVlanPortAndProtocolBased(units, l2sw_switch_port_L2P_get(units,port), i, &ppb_vlan_cfg)) != L2SW_ERR_OK)
            return retVal;
    }

    bUsed[0] = L2SW_FALSE;
    bUsed[1] = L2SW_FALSE;
    bUsed[2] = L2SW_FALSE;
    bUsed[3] = L2SW_FALSE;
    L2SW_SCAN_ALL_PHY_PORTMASK(units, i)
    {
        for (j = 0; j <= L2SW_PROTOVLAN_GIDX_MAX; j++)
        {
            if ((retVal = l2sw_getAsicVlanPortAndProtocolBased(units, i,j, &ppb_vlan_cfg)) != L2SW_ERR_OK)
                return retVal;

            if (L2SW_TRUE == ppb_vlan_cfg.valid)
            {
                bUsed[j] = L2SW_TRUE;
            }
        }
    }

    for (i = 0; i<= L2SW_PROTOVLAN_GIDX_MAX; i++)
    {
        if (L2SW_FALSE == bUsed[i]) /*No Port use this PPB Index, Delete it*/
        {
            ppb_data_cfg.etherType=0;
            ppb_data_cfg.frameType=0;
            if ((retVal = l2sw_setAsicVlanProtocolBasedGroupData(units, i, &ppb_data_cfg)) != L2SW_ERR_OK)
                return retVal;
        }
    }



    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_tagMode_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t tag_mode)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (tag_mode >= VLAN_TAG_MODE_END)
        return L2SW_ERR_PORT_ID;

    if ((retVal = l2sw_setAsicVlanEgressTagMode(units, l2sw_switch_port_L2P_get(units,port), tag_mode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_tagMode_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t *pTag_mode)
{
    l2sw_api_ret_t retVal;
    l2sw_egtagmode  mode;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pTag_mode)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicVlanEgressTagMode(units, l2sw_switch_port_L2P_get(units,port), &mode)) != L2SW_ERR_OK)
        return retVal;

    *pTag_mode = (l2sw_vlan_tagMode_t)mode;
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_transparent_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
     l2sw_api_ret_t retVal;
     l2sw_uint32    pmask;

     /* Check initialization state */
     L2SW_CHK_INIT_STATE(units);

     /* Check Port Valid */
     L2SW_CHK_PORT_VALID(units, egr_port);

     if(NULL == pIgr_pmask)
        return L2SW_ERR_NULL_POINTER;

     L2SW_CHK_PORTMASK_VALID(units, pIgr_pmask);

     if(l2sw_switch_portmask_L2P_get(units, pIgr_pmask, &pmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

     if ((retVal = l2sw_setAsicVlanTransparent(units, l2sw_switch_port_L2P_get(units, egr_port), pmask)) != L2SW_ERR_OK)
         return retVal;

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_transparent_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
     l2sw_api_ret_t retVal;
     l2sw_uint32    pmask;

     /* Check initialization state */
     L2SW_CHK_INIT_STATE(units);

     /* Check Port Valid */
     L2SW_CHK_PORT_VALID(units, egr_port);

     if(NULL == pIgr_pmask)
        return L2SW_ERR_NULL_POINTER;

     if ((retVal = l2sw_getAsicVlanTransparent(units, l2sw_switch_port_L2P_get(units,egr_port), &pmask)) != L2SW_ERR_OK)
         return retVal;

     if(l2sw_switch_portmask_P2L_get(units, pmask, pIgr_pmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_keep_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
     l2sw_api_ret_t retVal;
     l2sw_uint32    pmask;

     /* Check initialization state */
     L2SW_CHK_INIT_STATE(units);

     /* Check Port Valid */
     L2SW_CHK_PORT_VALID(units, egr_port);

     if(NULL == pIgr_pmask)
        return L2SW_ERR_NULL_POINTER;

     L2SW_CHK_PORTMASK_VALID(units, pIgr_pmask);

     if(l2sw_switch_portmask_L2P_get(units, pIgr_pmask, &pmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

     if ((retVal = l2sw_setAsicVlanEgressKeep(units, l2sw_switch_port_L2P_get(units,egr_port), pmask)) != L2SW_ERR_OK)
         return retVal;

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_keep_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
     l2sw_api_ret_t retVal;
     l2sw_uint32    pmask;

     /* Check initialization state */
     L2SW_CHK_INIT_STATE(units);

     /* Check Port Valid */
     L2SW_CHK_PORT_VALID(units, egr_port);

     if(NULL == pIgr_pmask)
        return L2SW_ERR_NULL_POINTER;

     if ((retVal = l2sw_getAsicVlanEgressKeep(units, l2sw_switch_port_L2P_get(units,egr_port), &pmask)) != L2SW_ERR_OK)
         return retVal;

     if(l2sw_switch_portmask_P2L_get(units, pmask, pIgr_pmask) != L2SW_ERR_OK)
        return L2SW_ERR_FAILED;

     return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_stg_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t stg)
{
    l2sw_api_ret_t retVal;
    l2sw_user_vlan4kentry vlan4K;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~4095 */
    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* priority must be 0~15 */
    if (stg > L2SW_MSTIMAX)
        return L2SW_ERR_MSTI;

    /* update 4K table */
    vlan4K.vid = vid;
    if ((retVal = l2sw_getAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
        return retVal;

    vlan4K.fid_msti= stg;
    if ((retVal = l2sw_setAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


static l2sw_api_ret_t _l2sw_vlan_stg_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t *pStg)
{
    l2sw_api_ret_t retVal;
    l2sw_user_vlan4kentry vlan4K;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* vid must be 0~4095 */
    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    if(NULL == pStg)
        return L2SW_ERR_NULL_POINTER;

    /* update 4K table */
    vlan4K.vid = vid;
    if ((retVal = l2sw_getAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
        return retVal;

    *pStg = vlan4K.fid_msti;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_portFid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable>=L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    /* fid must be 0~4095 */
    if (fid > L2SW_FID_MAX)
        return L2SW_ERR_L2_FID;

    if ((retVal = l2sw_setAsicPortBasedFidEn(units, l2sw_switch_port_L2P_get(units,port), enable))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPortBasedFid(units, l2sw_switch_port_L2P_get(units,port), fid))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_portFid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable, l2sw_fid_t *pFid)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pFid)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortBasedFidEn(units, l2sw_switch_port_L2P_get(units,port), pEnable))!=L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPortBasedFid(units, l2sw_switch_port_L2P_get(units,port), pFid))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_UntagDscpPriorityEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicVlanUntagDscpPriorityEn(units, (l2sw_uint32)enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_UntagDscpPriorityEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32  value;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicVlanUntagDscpPriorityEn(units, &value)) != L2SW_ERR_OK)
        return retVal;

    *pEnable = (l2sw_enable_t)value;
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stp_mstpState_set(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t stp_state)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (msti > L2SW_MAX_NUM_OF_MSTI)
        return L2SW_ERR_MSTI;

    if (stp_state >= STP_STATE_END)
        return L2SW_ERR_MSTP_STATE;

    if ((retVal = l2sw_setAsicSpanningTreeStatus(units, l2sw_switch_port_L2P_get(units,port), msti, stp_state)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stp_mstpState_get(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t *pStp_state)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (msti > L2SW_MAX_NUM_OF_MSTI)
        return L2SW_ERR_MSTI;

    if(NULL == pStp_state)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSpanningTreeStatus(units, l2sw_switch_port_L2P_get(units,port), msti, pStp_state)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_reservedVidAction_set(l2sw_uint8 units, l2sw_vlan_resVidAction_t action_vid0, l2sw_vlan_resVidAction_t action_vid4095)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(action_vid0 >= RESVID_ACTION_END)
        return L2SW_ERR_INPUT;

    if(action_vid4095 >= RESVID_ACTION_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setReservedVidAction(units, (l2sw_uint32)action_vid0, (l2sw_uint32)action_vid4095)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_reservedVidAction_get(l2sw_uint8 units, l2sw_vlan_resVidAction_t *pAction_vid0, l2sw_vlan_resVidAction_t *pAction_vid4095)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pAction_vid0 == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(pAction_vid4095 == NULL)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getReservedVidAction(units, (l2sw_uint32 *)pAction_vid0, (l2sw_uint32 *)pAction_vid4095)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_realKeepRemarkEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setRealKeepRemarkEn(units, (l2sw_uint32)enabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_realKeepRemarkEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getRealKeepRemarkEn(units, (l2sw_uint32 *)pEnabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_resetVlan(units)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

l2sw_api_ret_t _l2sw_vlan_protoAndPortBasedVlan_show(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 index, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal;
    l2sw_protocolgdatacfg ppb_data_cfg;
    l2sw_protocolvlancfg ppb_vlan_cfg;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicVlanProtocolBasedGroupData(units, index, &ppb_data_cfg)) != L2SW_ERR_OK)
        return retVal;
 
    if ((retVal = l2sw_getAsicVlanPortAndProtocolBased(units, l2sw_switch_port_L2P_get(units,port), index, &ppb_vlan_cfg)) != L2SW_ERR_OK)
        return retVal;

    if (L2SW_FALSE == ppb_vlan_cfg.valid)
        return L2SW_ERR_VLAN_PROTO_AND_PORT;

    pInfo->frame_type = (l2sw_vlan_protoVlan_frameType_t)ppb_data_cfg.frameType;
    pInfo->proto_type = ppb_data_cfg.etherType;
    pInfo->cvid = vlan_mbrCfgVid[units][ppb_vlan_cfg.vlan_idx];
    pInfo->cpri = ppb_vlan_cfg.priority;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_vlan_reset_show(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getresetVlan(units, (l2sw_uint32 *)pEnabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_vlan_init
 * Description:
 *      Initialize VLAN.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      VLAN is disabled by default. User has to call this API to enable VLAN before
 *      using it. And It will set a default VLAN(vid 1) including all ports and set
 *      all ports PVID to the default VLAN.
 */
l2sw_api_ret_t l2sw_vlan_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_init(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_set
 * Description:
 *      Set a VLAN entry.
 * Input:
 *      vid - VLAN ID to configure.
 *      pVlanCfg - VLAN Configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 *      L2SW_ERR_L2_FID               - Invalid FID.
 *      L2SW_ERR_VLAN_PORT_MBR_EXIST  - Invalid member port mask.
 *      L2SW_ERR_VLAN_VID             - Invalid VID parameter.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_set(units, vid, pVlanCfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_get
 * Description:
 *      Get a VLAN entry.
 * Input:
 *      vid - VLAN ID to configure.
 * Output:
 *      pVlanCfg - VLAN Configuration
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_vlan_cfg_t *pVlanCfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_get(units, vid, pVlanCfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_egrFilterEnable_set
 * Description:
 *      Set VLAN egress filter.
 * Input:
 *      egrFilter - Egress filtering
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_ENABLE       - Invalid input parameters.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_egrFilterEnable_set(l2sw_uint8 units, l2sw_enable_t egrFilter)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_egrFilterEnable_set(units, egrFilter);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_egrFilterEnable_get
 * Description:
 *      Get VLAN egress filter.
 * Input:
 *      pEgrFilter - Egress filtering
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - NULL Pointer.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_egrFilterEnable_get(l2sw_uint8 units, l2sw_enable_t *pEgrFilter)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_egrFilterEnable_get(units, pEgrFilter);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_mbrCfg_set
 * Description:
 *      Set a VLAN Member Configuration entry by index.
 * Input:
 *      idx     - Index of VLAN Member Configuration.
 *      pMbrcfg - VLAN member Configuration.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *     Set a VLAN Member Configuration entry by index.
 */
l2sw_api_ret_t l2sw_vlan_mbrCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_mbrCfg_set(units, idx, pMbrcfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_mbrCfg_get
 * Description:
 *      Get a VLAN Member Configuration entry by index.
 * Input:
 *      idx - Index of VLAN Member Configuration.
 * Output:
 *      pMbrcfg - VLAN member Configuration.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *     Get a VLAN Member Configuration entry by index.
 */
l2sw_api_ret_t l2sw_vlan_mbrCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_vlan_mbrcfg_t *pMbrcfg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_mbrCfg_get(units, idx, pMbrcfg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *     l2sw_vlan_portPvid_set
 * Description:
 *      Set port to specified VLAN ID(PVID).
 * Input:
 *      port - Port id.
 *      pvid - Specified VLAN ID.
 *      priority - 802.1p priority for the PVID.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_VLAN_PRIORITY        - Invalid priority.
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - VLAN entry not found.
 *      L2SW_ERR_VLAN_VID             - Invalid VID parameter.
 * Note:
 *       The API is used for Port-based VLAN. The untagged frame received from the
 *       port will be classified to the specified VLAN and assigned to the specified priority.
 */
l2sw_api_ret_t l2sw_vlan_portPvid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t pvid, l2sw_pri_t priority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portPvid_set(units, port, pvid, priority);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portPvid_get
 * Description:
 *      Get VLAN ID(PVID) on specified port.
 * Input:
 *      port - Port id.
 * Output:
 *      pPvid - Specified VLAN ID.
 *      pPriority - 802.1p priority for the PVID.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can get the PVID and 802.1p priority for the PVID of Port-based VLAN.
 */
l2sw_api_ret_t l2sw_vlan_portPvid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_t *pPvid, l2sw_pri_t *pPriority)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portPvid_get(units, port, pPvid, pPriority);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portIgrFilterEnable_set
 * Description:
 *      Set VLAN ingress for each port.
 * Input:
 *      port - Port id.
 *      igr_filter - VLAN ingress function enable status.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number
 *      L2SW_ERR_ENABLE       - Invalid enable input
 * Note:
 *      The status of vlan ingress filter is as following:
 *      - DISABLED
 *      - ENABLED
 *      While VLAN function is enabled, ASIC will decide VLAN ID for each received frame and get belonged member
 *      ports from VLAN table. If received port is not belonged to VLAN member ports, ASIC will drop received frame if VLAN ingress function is enabled.
 */
l2sw_api_ret_t l2sw_vlan_portIgrFilterEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t igr_filter)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portIgrFilterEnable_set(units, port, igr_filter);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portIgrFilterEnable_get
 * Description:
 *      Get VLAN Ingress Filter
 * Input:
 *      port        - Port id.
 * Output:
 *      pIgr_filter - VLAN ingress function enable status.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can Get the VLAN ingress filter status.
 *     The status of vlan ingress filter is as following:
 *     - DISABLED
 *     - ENABLED
 */
l2sw_api_ret_t l2sw_vlan_portIgrFilterEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pIgr_filter)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portIgrFilterEnable_get(units, port, pIgr_filter);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portAcceptFrameType_set
 * Description:
 *      Set VLAN accept_frame_type
 * Input:
 *      port                - Port id.
 *      accept_frame_type   - accept frame type
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                       - OK
 *      L2SW_ERR_FAILED                   - Failed
 *      L2SW_ERR_SMI                      - SMI access error
 *      L2SW_ERR_PORT_ID                  - Invalid port number.
 *      L2SW_ERR_VLAN_ACCEPT_FRAME_TYPE   - Invalid frame type.
 * Note:
 *      The API is used for checking 802.1Q tagged frames.
 *      The accept frame type as following:
 *      - ACCEPT_FRAME_TYPE_ALL
 *      - ACCEPT_FRAME_TYPE_TAG_ONLY
 *      - ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
l2sw_api_ret_t l2sw_vlan_portAcceptFrameType_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t accept_frame_type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portAcceptFrameType_set(units, port, accept_frame_type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portAcceptFrameType_get
 * Description:
 *      Get VLAN accept_frame_type
 * Input:
 *      port - Port id.
 * Output:
 *      pAccept_frame_type - accept frame type
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *     The API can Get the VLAN ingress filter.
 *     The accept frame type as following:
 *     - ACCEPT_FRAME_TYPE_ALL
 *     - ACCEPT_FRAME_TYPE_TAG_ONLY
 *     - ACCEPT_FRAME_TYPE_UNTAG_ONLY
 */
l2sw_api_ret_t l2sw_vlan_portAcceptFrameType_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_acceptFrameType_t *pAccept_frame_type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portAcceptFrameType_get(units, port, pAccept_frame_type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_add
 * Description:
 *      Add the protocol-and-port-based vlan to the specified port of device.
 * Input:
 *      port  - Port id.
 *      pInfo - Protocol and port based VLAN configuration information.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_VLAN_VID         - Invalid VID parameter.
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid priority.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *      The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_add(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_protoAndPortBasedVlan_add(units, port, pInfo);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_get
 * Description:
 *      Get the protocol-and-port-based vlan to the specified port of device.
 * Input:
 *      port - Port id.
 *      proto_type - protocol-and-port-based vlan protocol type.
 *      frame_type - protocol-and-port-based vlan frame type.
 * Output:
 *      pInfo - Protocol and port based VLAN configuration information.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_protoAndPortBasedVlan_get(units, port, proto_type, frame_type, pInfo);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_del
 * Description:
 *      Delete the protocol-and-port-based vlan from the specified port of device.
 * Input:
 *      port        - Port id.
 *      proto_type  - protocol-and-port-based vlan protocol type.
 *      frame_type  - protocol-and-port-based vlan frame type.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 *      L2SW_ERR_TBL_FULL         - Table is full.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     The frame type is shown in the following:
 *      - FRAME_TYPE_ETHERNET
 *      - FRAME_TYPE_RFC1042
 *      - FRAME_TYPE_LLCOTHER
 */
l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_del(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_proto_type_t proto_type, l2sw_vlan_protoVlan_frameType_t frame_type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_protoAndPortBasedVlan_del(units, port, proto_type, frame_type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_delAll
 * Description:
 *     Delete all protocol-and-port-based vlans from the specified port of device.
 * Input:
 *      port - Port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *     The incoming packet which match the protocol-and-port-based vlan will use the configure vid for ingress pipeline
 *     Delete all flow table protocol-and-port-based vlan entries.
 */
l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_delAll(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_protoAndPortBasedVlan_delAll(units, port);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_tagMode_set
 * Description:
 *      Set CVLAN egress tag mode
 * Input:
 *      port        - Port id.
 *      tag_mode    - The egress tag mode.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      The API can set Egress tag mode. There are 4 mode for egress tag:
 *      - VLAN_TAG_MODE_ORIGINAL,
 *      - VLAN_TAG_MODE_KEEP_FORMAT,
 *      - VLAN_TAG_MODE_PRI.
 *      - VLAN_TAG_MODE_REAL_KEEP_FORMAT,
 */
l2sw_api_ret_t l2sw_vlan_tagMode_set(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t tag_mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_tagMode_set(units, port, tag_mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_tagMode_get
 * Description:
 *      Get CVLAN egress tag mode
 * Input:
 *      port - Port id.
 * Output:
 *      pTag_mode - The egress tag mode.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get Egress tag mode. There are 4 mode for egress tag:
 *      - VLAN_TAG_MODE_ORIGINAL,
 *      - VLAN_TAG_MODE_KEEP_FORMAT,
 *      - VLAN_TAG_MODE_PRI.
 *      - VLAN_TAG_MODE_REAL_KEEP_FORMAT,
 */
l2sw_api_ret_t l2sw_vlan_tagMode_get(l2sw_uint8 units, l2sw_port_t port, l2sw_vlan_tagMode_t *pTag_mode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_tagMode_get(units, port, pTag_mode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_transparent_set
 * Description:
 *      Set VLAN transparent mode
 * Input:
 *      egr_port        - Egress Port id.
 *      pIgr_pmask      - Ingress Port Mask.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_vlan_transparent_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_transparent_set(units, egr_port, pIgr_pmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_transparent_get
 * Description:
 *      Get VLAN transparent mode
 * Input:
 *      egr_port        - Egress Port id.
 * Output:
 *      pIgr_pmask      - Ingress Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_vlan_transparent_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_transparent_get(units, egr_port, pIgr_pmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_keep_set
 * Description:
 *      Set VLAN egress keep mode
 * Input:
 *      egr_port        - Egress Port id.
 *      pIgr_pmask      - Ingress Port Mask.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_vlan_keep_set(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_keep_set(units, egr_port, pIgr_pmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_keep_get
 * Description:
 *      Get VLAN egress keep mode
 * Input:
 *      egr_port        - Egress Port id.
 * Output:
 *      pIgr_pmask      - Ingress Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_vlan_keep_get(l2sw_uint8 units, l2sw_port_t egr_port, l2sw_portmask_t *pIgr_pmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_keep_get(units, egr_port, pIgr_pmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_stg_set
 * Description:
 *      Set spanning tree group instance of the vlan to the specified device
 * Input:
 *      vid - Specified VLAN ID.
 *      stg - spanning tree group instance.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_MSTI         - Invalid msti parameter
 *      L2SW_ERR_INPUT        - Invalid input parameter.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *      The API can set spanning tree group instance of the vlan to the specified device.
 */
l2sw_api_ret_t l2sw_vlan_stg_set(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t stg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_stg_set(units, vid, stg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_stg_get
 * Description:
 *      Get spanning tree group instance of the vlan to the specified device
 * Input:
 *      vid - Specified VLAN ID.
 * Output:
 *      pStg - spanning tree group instance.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 * Note:
 *      The API can get spanning tree group instance of the vlan to the specified device.
 */
l2sw_api_ret_t l2sw_vlan_stg_get(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_stp_msti_id_t *pStg)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_stg_get(units, vid, pStg);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portFid_set
 * Description:
 *      Set port-based filtering database
 * Input:
 *      port - Port id.
 *      enable - ebable port-based FID
 *      fid - Specified filtering database.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_L2_FID - Invalid fid.
 *      L2SW_ERR_INPUT - Invalid input parameter.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can set port-based filtering database. If the function is enabled, all input
 *      packets will be assigned to the port-based fid regardless vlan tag.
 */
l2sw_api_ret_t l2sw_vlan_portFid_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable, l2sw_fid_t fid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portFid_set(units, port, enable, fid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_portFid_get
 * Description:
 *      Get port-based filtering database
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - ebable port-based FID
 *      pFid - Specified filtering database.
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_INPUT - Invalid input parameters.
 *      L2SW_ERR_PORT_ID - Invalid port ID.
 * Note:
 *      The API can get port-based filtering database status. If the function is enabled, all input
 *      packets will be assigned to the port-based fid regardless vlan tag.
 */
l2sw_api_ret_t l2sw_vlan_portFid_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable, l2sw_fid_t *pFid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_portFid_get(units, port, pEnable, pFid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_UntagDscpPriorityEnable_set
 * Description:
 *      Set Untag DSCP priority assign
 * Input:
 *      enable - state of Untag DSCP priority assign
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_ENABLE          - Invalid input parameters.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_UntagDscpPriorityEnable_set(l2sw_uint8 units, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_UntagDscpPriorityEnable_set(units, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_UntagDscpPriorityEnable_get
 * Description:
 *      Get Untag DSCP priority assign
 * Input:
 *      None
 * Output:
 *      pEnable - state of Untag DSCP priority assign
 * Return:
 *      L2SW_ERR_OK              - OK
 *      L2SW_ERR_FAILED          - Failed
 *      L2SW_ERR_SMI             - SMI access error
 *      L2SW_ERR_NULL_POINTER    - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_UntagDscpPriorityEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_UntagDscpPriorityEnable_get(units, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stp_mstpState_set
 * Description:
 *      Configure spanning tree state per each port.
 * Input:
 *      port - Port id
 *      msti - Multiple spanning tree instance.
 *      stp_state - Spanning tree state for msti
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_MSTI         - Invalid msti parameter.
 *      L2SW_ERR_MSTP_STATE   - Invalid STP state.
 * Note:
 *      System supports per-port multiple spanning tree state for each msti.
 *      There are four states supported by ASIC.
 *      - STP_STATE_DISABLED
 *      - STP_STATE_BLOCKING
 *      - STP_STATE_LEARNING
 *      - STP_STATE_FORWARDING
 */
l2sw_api_ret_t l2sw_stp_mstpState_set(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t stp_state)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stp_mstpState_set(units, msti, port, stp_state);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stp_mstpState_get
 * Description:
 *      Get spanning tree state per each port.
 * Input:
 *      port - Port id.
 *      msti - Multiple spanning tree instance.
 * Output:
 *      pStp_state - Spanning tree state for msti
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_MSTI         - Invalid msti parameter.
 * Note:
 *      System supports per-port multiple spanning tree state for each msti.
 *      There are four states supported by ASIC.
 *      - STP_STATE_DISABLED
 *      - STP_STATE_BLOCKING
 *      - STP_STATE_LEARNING
 *      - STP_STATE_FORWARDING
 */
l2sw_api_ret_t l2sw_stp_mstpState_get(l2sw_uint8 units, l2sw_stp_msti_id_t msti, l2sw_port_t port, l2sw_stp_state_t *pStp_state)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stp_mstpState_get(units, msti, port, pStp_state);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_reservedVidAction_set
 * Description:
 *      Set Action of VLAN ID = 0 & 4095 tagged packet
 * Input:
 *      action_vid0     - Action for VID 0.
 *      action_vid4095  - Action for VID 4095.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reservedVidAction_set(l2sw_uint8 units, l2sw_vlan_resVidAction_t action_vid0, l2sw_vlan_resVidAction_t action_vid4095)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_reservedVidAction_set(units, action_vid0, action_vid4095);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_reservedVidAction_get
 * Description:
 *      Get Action of VLAN ID = 0 & 4095 tagged packet
 * Input:
 *      pAction_vid0     - Action for VID 0.
 *      pAction_vid4095  - Action for VID 4095.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - NULL Pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reservedVidAction_get(l2sw_uint8 units, l2sw_vlan_resVidAction_t *pAction_vid0, l2sw_vlan_resVidAction_t *pAction_vid4095)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_reservedVidAction_get(units, pAction_vid0, pAction_vid4095);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_realKeepRemarkEnable_set
 * Description:
 *      Set Real keep 1p remarking feature
 * Input:
 *      enabled     - State of 1p remarking at real keep packet
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_realKeepRemarkEnable_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_realKeepRemarkEnable_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_realKeepRemarkEnable_get
 * Description:
 *      Get Real keep 1p remarking feature
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_realKeepRemarkEnable_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_realKeepRemarkEnable_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_reset
 * Description:
 *      Reset VLAN
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_reset(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/*Don't lock mutex in following API*/

/* Function Name:
 *      l2sw_vlan_checkAndCreateMbr
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
 *      L2SW_ERR_VLAN_ENTRY_NOT_FOUND - VLAN not found
 *      L2SW_ERR_TBL_FULL     - Member Configuration table full
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_checkAndCreateMbr(l2sw_uint8 units, l2sw_vlan_t vid, l2sw_uint32 *pIndex)
{
    l2sw_api_ret_t retVal;
    l2sw_user_vlan4kentry vlan4K;
    l2sw_vlanconfiguser vlanMC;
    l2sw_uint32 idx;
    l2sw_uint32 empty_idx = 0xFFFF;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    MEMSET(&vlan4K, 0x00, sizeof(l2sw_user_vlan4kentry));

    /* vid must be 0~8191 */
    if (vid > L2SW_EVIDMAX)
        return L2SW_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pIndex)
        return L2SW_ERR_NULL_POINTER;

    /* Get 4K VLAN */
    if (vid <= L2SW_VIDMAX)
    {
        MEMSET(&vlan4K, 0x00, sizeof(l2sw_user_vlan4kentry));
        vlan4K.vid = vid;
        if ((retVal = l2sw_getAsicVlan4kEntry(units, &vlan4K)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Search exist entry */
    for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[units][idx] == MBRCFG_USED_BY_VLAN)
        {
            if(vlan_mbrCfgVid[units][idx] == vid)
            {
                /* Found! return index */
                *pIndex = idx;
                return L2SW_ERR_OK;
            }
        }
    }

    /* Not found, Read H/W Member Configuration table to update database */
    for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
    {
        if ((retVal = l2sw_getAsicVlanMemberConfig(units, idx, &vlanMC)) != L2SW_ERR_OK)
            return retVal;

        if( (vlanMC.evid == 0) && (vlanMC.mbr == 0x00))
        {
            vlan_mbrCfgUsage[units][idx]   = MBRCFG_UNUSED;
            vlan_mbrCfgVid[units][idx]     = 0;
        }
        else
        {
            vlan_mbrCfgUsage[units][idx]   = MBRCFG_USED_BY_VLAN;
            vlan_mbrCfgVid[units][idx]     = vlanMC.evid;
        }
    }

    /* Search exist entry again */
    for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[units][idx] == MBRCFG_USED_BY_VLAN)
        {
            if(vlan_mbrCfgVid[units][idx] == vid)
            {
                /* Found! return index */
                *pIndex = idx;
                return L2SW_ERR_OK;
            }
        }
    }

    /* try to look up an empty index */
    for (idx = 0; idx <= L2SW_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[units][idx] == MBRCFG_UNUSED)
        {
            empty_idx = idx;
            break;
        }
    }

    if(empty_idx == 0xFFFF)
    {
        /* No empty index */
        return L2SW_ERR_TBL_FULL;
    }

    if (vid > L2SW_VIDMAX)
    {
        /* > 4K, there is no 4K entry, create on member configuration directly */
        MEMSET(&vlanMC, 0x00, sizeof(l2sw_vlanconfiguser));
        vlanMC.evid = vid;
        if ((retVal = l2sw_setAsicVlanMemberConfig(units, empty_idx, &vlanMC)) != L2SW_ERR_OK)
            return retVal;
    }
    else
    {
        /* Copy from 4K table */
        vlanMC.evid = vid;
        vlanMC.mbr = vlan4K.mbr;
        vlanMC.fid_msti = vlan4K.fid_msti;
        vlanMC.meteridx= vlan4K.meteridx;
        vlanMC.envlanpol= vlan4K.envlanpol;
        vlanMC.vbpen = vlan4K.vbpen;
        vlanMC.vbpri = vlan4K.vbpri;
        if ((retVal = l2sw_setAsicVlanMemberConfig(units, empty_idx, &vlanMC)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Update Database */
    vlan_mbrCfgUsage[units][empty_idx] = MBRCFG_USED_BY_VLAN;
    vlan_mbrCfgVid[units][empty_idx] = vid;

    *pIndex = empty_idx;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_vlan_protoAndPortBasedVlan_show
 * Description:
 *      Shows all protocol based VLAN configurations on the port
 * Input:
 *      port    -    Port id.
 *      index   -    register of index.
 *
 * Output:
 *      pInfo - Protocol and port based VLAN configuration information.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_protoAndPortBasedVlan_show(l2sw_uint8 units, l2sw_port_t port, l2sw_uint32 index, l2sw_vlan_protoAndPortInfo_t *pInfo)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_protoAndPortBasedVlan_show(units, port, index,  pInfo);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_vlan_reset_show
 * Description:
 *      Show Reset VLAN state
 * Input:
 *      None.
 * Output:
 *      pEnabled     - State of 1p remarking at real keep packet
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 *
 */
l2sw_api_ret_t l2sw_vlan_reset_show(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_vlan_reset_show(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}



