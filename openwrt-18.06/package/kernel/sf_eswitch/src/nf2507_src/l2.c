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
#include "l2.h"
#include "sal_string.h"
#include "l2sw_asicdrv_lut.h"
#include "l2sw_asicdrv_port.h"

static l2sw_api_ret_t _l2sw_l2_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicLutIpMulticastLookup(units, L2SW_DISABLED)) != L2SW_ERR_OK)
        return retVal;

    /*Enable CAM Usage*/
    if ((retVal = l2sw_setAsicLutCamTbUsage(units, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicLutAgeTimerSpeed(units, 6,2)) != L2SW_ERR_OK)
        return retVal;

    L2SW_SCAN_ALL_LOG_PORT(units, port)
    {
        if ((retVal = l2sw_setAsicLutLearnLimitNo(units, l2sw_switch_port_L2P_get(units, port), l2sw_switch_maxLutAddrNumber_get(units))) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_addr_add(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pMac == NULL) || (pMac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    if(pL2_data == NULL)
        return L2SW_ERR_MAC;

    L2SW_CHK_PORT_VALID(units, pL2_data->port);

    if (pL2_data->ivl >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->cvid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if (pL2_data->fid > L2SW_FIDMAX)
        return L2SW_ERR_L2_FID;

    if (pL2_data->is_static>= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->sa_block>= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->da_block>= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->auth>= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->efid> L2SW_EFIDMAX)
        return L2SW_ERR_INPUT;

    if (pL2_data->priority > L2SW_PRIMAX)
        return L2SW_ERR_INPUT;

    if (pL2_data->sa_pri_en >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if (pL2_data->fwd_pri_en >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pL2_data->ivl;
    l2Table.fid         = pL2_data->fid;
    l2Table.cvid_fid    = pL2_data->cvid;
    l2Table.efid        = pL2_data->efid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal )
    {
        MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pL2_data->ivl;
        l2Table.cvid_fid    = pL2_data->cvid;
        l2Table.fid         = pL2_data->fid;
        l2Table.efid        = pL2_data->efid;
        l2Table.spa         = l2sw_switch_port_L2P_get(units, pL2_data->port);
        l2Table.nosalearn   = pL2_data->is_static;
        l2Table.sa_block    = pL2_data->sa_block;
        l2Table.da_block    = pL2_data->da_block;
        l2Table.l3lookup    = 0;
        l2Table.auth        = pL2_data->auth;
        l2Table.age         = 6;
        l2Table.lut_pri     = pL2_data->priority;
        l2Table.sa_en       = pL2_data->sa_pri_en;
        l2Table.fwd_en      = pL2_data->fwd_pri_en;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pL2_data->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal )
    {
        MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
        MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pL2_data->ivl;
        l2Table.cvid_fid    = pL2_data->cvid;
        l2Table.fid         = pL2_data->fid;
        l2Table.efid        = pL2_data->efid;
        l2Table.spa         = l2sw_switch_port_L2P_get(units, pL2_data->port);
        l2Table.nosalearn   = pL2_data->is_static;
        l2Table.sa_block    = pL2_data->sa_block;
        l2Table.da_block    = pL2_data->da_block;
        l2Table.l3lookup    = 0;
        l2Table.auth        = pL2_data->auth;
        l2Table.age         = 6;
        l2Table.lut_pri     = pL2_data->priority;
        l2Table.sa_en       = pL2_data->sa_pri_en;
        l2Table.fwd_en      = pL2_data->fwd_pri_en;

        if ((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pL2_data->address = l2Table.address;

        method = LUTREADMETHOD_MAC;
        retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
        if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal )
            return L2SW_ERR_L2_INDEXTBL_FULL;
        else
            return retVal;
    }
    else
        return retVal;

}

static l2sw_api_ret_t _l2sw_l2_addr_get(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pMac == NULL) || (pMac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    if (pL2_data->fid > L2SW_FIDMAX || pL2_data->efid > L2SW_EFIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pL2_data->ivl;
    l2Table.cvid_fid    = pL2_data->cvid;
    l2Table.fid         = pL2_data->fid;
    l2Table.efid        = pL2_data->efid;
    method = LUTREADMETHOD_MAC;

    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;
    
    MEMCPY(pL2_data->mac.octet, pMac->octet,ETHER_ADDR_LEN);
    pL2_data->port      = l2sw_switch_port_P2L_get(units, l2Table.spa);
    pL2_data->fid       = l2Table.fid;
    pL2_data->efid      = l2Table.efid;
    pL2_data->ivl       = l2Table.ivl_svl;
    pL2_data->cvid      = l2Table.cvid_fid;
    pL2_data->is_static = l2Table.nosalearn;
    pL2_data->auth      = l2Table.auth;
    pL2_data->sa_block  = l2Table.sa_block;
    pL2_data->da_block  = l2Table.da_block;
    pL2_data->priority  = l2Table.lut_pri;
    pL2_data->sa_pri_en = l2Table.sa_en;
    pL2_data->fwd_pri_en= l2Table.fwd_en;
    pL2_data->address   = l2Table.address;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_addr_next_get(l2sw_uint8 units, l2sw_l2_read_method_t read_method, l2sw_port_t port, l2sw_uint32 *pAddress, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      method;
    l2sw_luttb  l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error Checking */
    if ((pL2_data == NULL) || (pAddress == NULL))
        return L2SW_ERR_MAC;

    if(read_method == READMETHOD_NEXT_L2UC)
        method = LUTREADMETHOD_NEXT_L2UC;
    else if(read_method == READMETHOD_NEXT_L2UCSPA)
        method = LUTREADMETHOD_NEXT_L2UCSPA;
    else
        return L2SW_ERR_INPUT;

    if(read_method == READMETHOD_NEXT_L2UCSPA)
    {
        /* Check Port Valid */
        L2SW_CHK_PORT_VALID(units, port);
    }

    if(*pAddress > L2SW_MAX_LUT_ADDR_ID(units) )
        return L2SW_ERR_L2_L2UNI_PARAM;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
    l2Table.address = *pAddress;

    if(read_method == READMETHOD_NEXT_L2UCSPA)
        l2Table.spa = l2sw_switch_port_L2P_get(units, port);

    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    if(l2Table.address < *pAddress)
        return L2SW_ERR_L2_ENTRY_NOTFOUND;

    MEMCPY(pL2_data->mac.octet, l2Table.mac.octet, ETHER_ADDR_LEN);
    pL2_data->port      = l2sw_switch_port_P2L_get(units, l2Table.spa);
    pL2_data->fid       = l2Table.fid;
    pL2_data->efid      = l2Table.efid;
    pL2_data->ivl       = l2Table.ivl_svl;
    pL2_data->cvid      = l2Table.cvid_fid;
    pL2_data->is_static = l2Table.nosalearn;
    pL2_data->auth      = l2Table.auth;
    pL2_data->sa_block  = l2Table.sa_block;
    pL2_data->da_block  = l2Table.da_block;
    pL2_data->priority  = l2Table.lut_pri;
    pL2_data->sa_pri_en = l2Table.sa_en;
    pL2_data->fwd_pri_en= l2Table.fwd_en;
    pL2_data->address   = l2Table.address;

    *pAddress = l2Table.address;

    return L2SW_ERR_OK;

}

static l2sw_api_ret_t _l2sw_l2_addr_del(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* must be unicast address */
    if ((pMac == NULL) || (pMac->octet[0] & 0x1))
        return L2SW_ERR_MAC;

    if (pL2_data->fid > L2SW_FIDMAX || pL2_data->efid > L2SW_EFIDMAX)
        return L2SW_ERR_L2_FID;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pL2_data->ivl;
    l2Table.cvid_fid    = pL2_data->cvid;
    l2Table.fid         = pL2_data->fid;
    l2Table.efid        = pL2_data->efid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK ==  retVal)
    {
        MEMCPY(l2Table.mac.octet, pMac->octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pL2_data->ivl;
        l2Table.cvid_fid    = pL2_data->cvid;
        l2Table.fid = pL2_data->fid;
        l2Table.efid = pL2_data->efid;
        l2Table.spa = 0;
        l2Table.nosalearn = 0;
        l2Table.sa_block = 0;
        l2Table.da_block = 0;
        l2Table.auth = 0;
        l2Table.age = 0;
        l2Table.lut_pri = 0;
        l2Table.sa_en = 0;
        l2Table.fwd_en = 0;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pL2_data->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else
        return retVal;
}

static l2sw_api_ret_t _l2sw_l2_mcastAddr_add(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      method;
    l2sw_luttb  l2Table;
    l2sw_uint32      pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    /* must be L2 multicast address */
    if( (pMcastAddr->mac.octet[0] & 0x01) != 0x01)
        return L2SW_ERR_MAC;

    L2SW_CHK_PORTMASK_VALID(units, &pMcastAddr->portmask);

    if(pMcastAddr->ivl == 1)
    {
        if (pMcastAddr->vid > L2SW_VIDMAX)
            return L2SW_ERR_L2_VID;
    }
    else if(pMcastAddr->ivl == 0)
    {
        if (pMcastAddr->fid > L2SW_FIDMAX)
            return L2SW_ERR_L2_FID;
    }
    else
        return L2SW_ERR_INPUT;

    if(pMcastAddr->fwd_pri_en >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if(pMcastAddr->priority > L2SW_PRIMAX)
        return L2SW_ERR_INPUT;

    /* Get physical port mask */
    if ((retVal = l2sw_switch_portmask_L2P_get(units, &pMcastAddr->portmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pMcastAddr->ivl;

    if(pMcastAddr->ivl)
        l2Table.cvid_fid    = pMcastAddr->vid;
    else
        l2Table.cvid_fid    = pMcastAddr->fid;

    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pMcastAddr->ivl;

        if(pMcastAddr->ivl)
            l2Table.cvid_fid    = pMcastAddr->vid;
        else
            l2Table.cvid_fid    = pMcastAddr->fid;

        l2Table.mbr         = pmask;
        l2Table.nosalearn   = 1;
        l2Table.l3lookup    = 0;
        l2Table.lut_pri     = pMcastAddr->priority;
        l2Table.fwd_en      = pMcastAddr->fwd_pri_en;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
    {
        MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
        MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pMcastAddr->ivl;
        if(pMcastAddr->ivl)
            l2Table.cvid_fid    = pMcastAddr->vid;
        else
            l2Table.cvid_fid    = pMcastAddr->fid;

        l2Table.mbr         = pmask;
        l2Table.nosalearn   = 1;
        l2Table.l3lookup    = 0;
        l2Table.lut_pri     = pMcastAddr->priority;
        l2Table.fwd_en      = pMcastAddr->fwd_pri_en;
        if ((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pMcastAddr->address = l2Table.address;

        method = LUTREADMETHOD_MAC;
        retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
        if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
            return     L2SW_ERR_L2_INDEXTBL_FULL;
        else
            return retVal;
    }
    else
        return retVal;

}

static l2sw_api_ret_t _l2sw_l2_mcastAddr_get(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    /* must be L2 multicast address */
    if( (pMcastAddr->mac.octet[0] & 0x01) != 0x01)
        return L2SW_ERR_MAC;

    if(pMcastAddr->ivl == 1)
    {
        if (pMcastAddr->vid > L2SW_VIDMAX)
            return L2SW_ERR_L2_VID;
    }
    else if(pMcastAddr->ivl == 0)
    {
        if (pMcastAddr->fid > L2SW_FIDMAX)
            return L2SW_ERR_L2_FID;
    }
    else
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
    MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pMcastAddr->ivl;

    if(pMcastAddr->ivl)
        l2Table.cvid_fid    = pMcastAddr->vid;
    else
        l2Table.cvid_fid    = pMcastAddr->fid;

    method = LUTREADMETHOD_MAC;

    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    pMcastAddr->priority    = l2Table.lut_pri;
    pMcastAddr->fwd_pri_en  = l2Table.fwd_en;
    pMcastAddr->igmp_asic   = l2Table.igmp_asic;
    pMcastAddr->igmp_index  = l2Table.igmpidx;
    pMcastAddr->address     = l2Table.address;

    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_mcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t   retVal;
    l2sw_luttb  l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error Checking */
    if ((pAddress == NULL) || (pMcastAddr == NULL))
        return L2SW_ERR_INPUT;

    if(*pAddress > L2SW_MAX_LUT_ADDR_ID(units) )
        return L2SW_ERR_L2_L2UNI_PARAM;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
    l2Table.address = *pAddress;

    if ((retVal = l2sw_getAsicL2LookupTb(units, LUTREADMETHOD_NEXT_L2MC, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    if(l2Table.address < *pAddress)
        return L2SW_ERR_L2_ENTRY_NOTFOUND;

    MEMCPY(pMcastAddr->mac.octet, l2Table.mac.octet, ETHER_ADDR_LEN);
    pMcastAddr->ivl     = l2Table.ivl_svl;

    if(pMcastAddr->ivl)
        pMcastAddr->vid = l2Table.cvid_fid;
    else
        pMcastAddr->fid = l2Table.cvid_fid;

    pMcastAddr->priority    = l2Table.lut_pri;
    pMcastAddr->fwd_pri_en  = l2Table.fwd_en;
    pMcastAddr->igmp_asic   = l2Table.igmp_asic;
    pMcastAddr->igmp_index  = l2Table.igmpidx;
    pMcastAddr->address     = l2Table.address;

    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    *pAddress = l2Table.address;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_mcastAddr_del(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    /* must be L2 multicast address */
    if( (pMcastAddr->mac.octet[0] & 0x01) != 0x01)
        return L2SW_ERR_MAC;

    if(pMcastAddr->ivl == 1)
    {
        if (pMcastAddr->vid > L2SW_VIDMAX)
            return L2SW_ERR_L2_VID;
    }
    else if(pMcastAddr->ivl == 0)
    {
        if (pMcastAddr->fid > L2SW_FIDMAX)
            return L2SW_ERR_L2_FID;
    }
    else
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));

    /* fill key (MAC,FID) to get L2 entry */
    MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
    l2Table.ivl_svl     = pMcastAddr->ivl;

    if(pMcastAddr->ivl)
        l2Table.cvid_fid    = pMcastAddr->vid;
    else
        l2Table.cvid_fid    = pMcastAddr->fid;

    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        MEMCPY(l2Table.mac.octet, pMcastAddr->mac.octet, ETHER_ADDR_LEN);
        l2Table.ivl_svl     = pMcastAddr->ivl;

        if(pMcastAddr->ivl)
            l2Table.cvid_fid    = pMcastAddr->vid;
        else
            l2Table.cvid_fid    = pMcastAddr->fid;

        l2Table.mbr         = 0;
        l2Table.nosalearn   = 0;
        l2Table.sa_block    = 0;
        l2Table.l3lookup    = 0;
        l2Table.lut_pri     = 0;
        l2Table.fwd_en      = 0;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else
        return retVal;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pIpMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    /* check port mask */
    L2SW_CHK_PORTMASK_VALID(units, &pIpMcastAddr->portmask);

    if( (pIpMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    if(pIpMcastAddr->fwd_pri_en >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pIpMcastAddr->priority > L2SW_PRIMAX)
        return L2SW_ERR_INPUT;

    /* Get Physical port mask */
    if ((retVal = l2sw_switch_portmask_L2P_get(units, &pIpMcastAddr->portmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpMcastAddr->sip;
    l2Table.dip = pIpMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 0;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        l2Table.sip = pIpMcastAddr->sip;
        l2Table.dip = pIpMcastAddr->dip;
        l2Table.mbr = pmask;
        l2Table.nosalearn = 1;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 0;
        l2Table.lut_pri = pIpMcastAddr->priority;
        l2Table.fwd_en  = pIpMcastAddr->fwd_pri_en;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
    {
        MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
        l2Table.sip = pIpMcastAddr->sip;
        l2Table.dip = pIpMcastAddr->dip;
        l2Table.mbr = pmask;
        l2Table.nosalearn = 1;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 0;
        l2Table.lut_pri = pIpMcastAddr->priority;
        l2Table.fwd_en  = pIpMcastAddr->fwd_pri_en;
        if ((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpMcastAddr->address = l2Table.address;

        method = LUTREADMETHOD_MAC;
        retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
        if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
            return     L2SW_ERR_L2_INDEXTBL_FULL;
        else
            return retVal;

    }
    else
        return retVal;

}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pIpMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    if( (pIpMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpMcastAddr->sip;
    l2Table.dip = pIpMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 0;
    method = LUTREADMETHOD_MAC;
    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pIpMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    pIpMcastAddr->priority      = l2Table.lut_pri;
    pIpMcastAddr->fwd_pri_en    = l2Table.fwd_en;
    pIpMcastAddr->igmp_asic     = l2Table.igmp_asic;
    pIpMcastAddr->igmp_index    = l2Table.igmpidx;
    pIpMcastAddr->address       = l2Table.address;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t   retVal;
    l2sw_luttb  l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error Checking */
    if ((pAddress == NULL) || (pIpMcastAddr == NULL) )
        return L2SW_ERR_INPUT;

    if(*pAddress > L2SW_MAX_LUT_ADDR_ID(units) )
        return L2SW_ERR_L2_L2UNI_PARAM;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
    l2Table.address = *pAddress;

    do
    {
        if ((retVal = l2sw_getAsicL2LookupTb(units, LUTREADMETHOD_NEXT_L3MC, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        if(l2Table.address < *pAddress)
            return L2SW_ERR_L2_ENTRY_NOTFOUND;

    }while(l2Table.l3vidlookup == 1);

    pIpMcastAddr->sip = l2Table.sip;
    pIpMcastAddr->dip = l2Table.dip;

    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pIpMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    pIpMcastAddr->priority      = l2Table.lut_pri;
    pIpMcastAddr->fwd_pri_en    = l2Table.fwd_en;
    pIpMcastAddr->igmp_asic     = l2Table.igmp_asic;
    pIpMcastAddr->igmp_index    = l2Table.igmpidx;
    pIpMcastAddr->address       = l2Table.address;
    *pAddress = l2Table.address;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error Checking */
    if (pIpMcastAddr == NULL)
        return L2SW_ERR_INPUT;

    if( (pIpMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpMcastAddr->sip;
    l2Table.dip = pIpMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 0;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        l2Table.sip = pIpMcastAddr->sip;
        l2Table.dip = pIpMcastAddr->dip;
        l2Table.mbr = 0;
        l2Table.nosalearn = 0;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 0;
        l2Table.lut_pri = 0;
        l2Table.fwd_en  = 0;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else
        return retVal;
}

static l2sw_api_ret_t _l2sw_l2_ipVidMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pIpVidMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    /* check port mask */
    L2SW_CHK_PORTMASK_VALID(units, &pIpVidMcastAddr->portmask);

    if (pIpVidMcastAddr->vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if( (pIpVidMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    /* Get Physical port mask */
    if ((retVal = l2sw_switch_portmask_L2P_get(units, &pIpVidMcastAddr->portmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpVidMcastAddr->sip;
    l2Table.dip = pIpVidMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 1;
    l2Table.l3_vid = pIpVidMcastAddr->vid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        l2Table.sip = pIpVidMcastAddr->sip;
        l2Table.dip = pIpVidMcastAddr->dip;
        l2Table.mbr = pmask;
        l2Table.nosalearn = 1;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 1;
        l2Table.l3_vid = pIpVidMcastAddr->vid;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpVidMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
    {
        MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
        l2Table.sip = pIpVidMcastAddr->sip;
        l2Table.dip = pIpVidMcastAddr->dip;
        l2Table.mbr = pmask;
        l2Table.nosalearn = 1;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 1;
        l2Table.l3_vid = pIpVidMcastAddr->vid;
        if ((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpVidMcastAddr->address = l2Table.address;

        method = LUTREADMETHOD_MAC;
        retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
        if (L2SW_ERR_L2_ENTRY_NOTFOUND == retVal)
            return  L2SW_ERR_L2_INDEXTBL_FULL;
        else
            return retVal;

    }
    else
        return retVal;
}

static l2sw_api_ret_t _l2sw_l2_ipVidMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pIpVidMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    if (pIpVidMcastAddr->vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if( (pIpVidMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpVidMcastAddr->sip;
    l2Table.dip = pIpVidMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 1;
    l2Table.l3_vid = pIpVidMcastAddr->vid;
    method = LUTREADMETHOD_MAC;
    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    pIpVidMcastAddr->address = l2Table.address;

     /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pIpVidMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipVidMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t   retVal;
    l2sw_luttb  l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Error Checking */
    if ((pAddress == NULL) || (pIpVidMcastAddr == NULL))
        return L2SW_ERR_INPUT;

    if(*pAddress > L2SW_MAX_LUT_ADDR_ID(units) )
        return L2SW_ERR_L2_L2UNI_PARAM;

    MEMSET(&l2Table, 0, sizeof(l2sw_luttb));
    l2Table.address = *pAddress;

    do
    {
        if ((retVal = l2sw_getAsicL2LookupTb(units, LUTREADMETHOD_NEXT_L3MC, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        if(l2Table.address < *pAddress)
            return L2SW_ERR_L2_ENTRY_NOTFOUND;

    }while(l2Table.l3vidlookup == 0);

    pIpVidMcastAddr->sip        = l2Table.sip;
    pIpVidMcastAddr->dip        = l2Table.dip;
    pIpVidMcastAddr->vid        = l2Table.l3_vid;
    pIpVidMcastAddr->address    = l2Table.address;

    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &pIpVidMcastAddr->portmask)) != L2SW_ERR_OK)
        return retVal;

    *pAddress = l2Table.address;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipVidMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pIpVidMcastAddr)
        return L2SW_ERR_NULL_POINTER;

    if (pIpVidMcastAddr->vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if( (pIpVidMcastAddr->dip & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.sip = pIpVidMcastAddr->sip;
    l2Table.dip = pIpVidMcastAddr->dip;
    l2Table.l3lookup = 1;
    l2Table.l3vidlookup = 1;
    l2Table.l3_vid = pIpVidMcastAddr->vid;
    method = LUTREADMETHOD_MAC;
    retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table);
    if (L2SW_ERR_OK == retVal)
    {
        l2Table.sip = pIpVidMcastAddr->sip;
        l2Table.dip = pIpVidMcastAddr->dip;
        l2Table.mbr= 0;
        l2Table.nosalearn = 0;
        l2Table.l3lookup = 1;
        l2Table.l3vidlookup = 1;
        l2Table.l3_vid = pIpVidMcastAddr->vid;
        if((retVal = l2sw_setAsicL2LookupTb(units, &l2Table)) != L2SW_ERR_OK)
            return retVal;

        pIpVidMcastAddr->address = l2Table.address;
        return L2SW_ERR_OK;
    }
    else
        return retVal;
}

static l2sw_api_ret_t _l2sw_l2_ucastAddr_flush(l2sw_uint8 units, l2sw_l2_flushCfg_t *pConfig)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(pConfig == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(pConfig->flushByVid >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->flushByFid >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->flushByPort >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->flushByMac >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->flushStaticAddr >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->flushAddrOnAllPorts >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(pConfig->vid > L2SW_VIDMAX)
        return L2SW_ERR_VLAN_VID;

    if(pConfig->fid > L2SW_FIDMAX)
        return L2SW_ERR_INPUT;

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, pConfig->port);

    if(pConfig->flushByVid == L2SW_ENABLED)
    {
        if ((retVal = l2sw_setAsicLutFlushMode(units, FLUSHMDOE_VID)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutFlushVid(units, pConfig->vid)) != L2SW_ERR_OK)
                return retVal;

        if ((retVal = l2sw_setAsicLutFlushType(units, (pConfig->flushStaticAddr == L2SW_ENABLED) ? FLUSHTYPE_BOTH : FLUSHTYPE_DYNAMIC)) != L2SW_ERR_OK)
            return retVal;

        if(pConfig->flushAddrOnAllPorts == L2SW_ENABLED)
        {
            if ((retVal = l2sw_setAsicLutForceFlush(units, L2SW_PORTMASK)) != L2SW_ERR_OK)
                return retVal;
        }
        else if(pConfig->flushByPort == L2SW_ENABLED)
        {
            if ((retVal = l2sw_setAsicLutForceFlush(units, 1 << l2sw_switch_port_L2P_get(units, pConfig->port))) != L2SW_ERR_OK)
                return retVal;
        }
        else
            return L2SW_ERR_INPUT;
    }
    else if(pConfig->flushByFid == L2SW_ENABLED)
    {
        if ((retVal = l2sw_setAsicLutFlushMode(units, FLUSHMDOE_FID)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutFlushFid(units, pConfig->fid)) != L2SW_ERR_OK)
                return retVal;

        if ((retVal = l2sw_setAsicLutFlushType(units, (pConfig->flushStaticAddr == L2SW_ENABLED) ? FLUSHTYPE_BOTH : FLUSHTYPE_DYNAMIC)) != L2SW_ERR_OK)
            return retVal;

        if(pConfig->flushAddrOnAllPorts == L2SW_ENABLED)
        {
            if ((retVal = l2sw_setAsicLutForceFlush(units, L2SW_PORTMASK)) != L2SW_ERR_OK)
                return retVal;
        }
        else if(pConfig->flushByPort == L2SW_ENABLED)
        {
            if ((retVal = l2sw_setAsicLutForceFlush(units, 1 << l2sw_switch_port_L2P_get(units, pConfig->port))) != L2SW_ERR_OK)
                return retVal;
        }
        else
            return L2SW_ERR_INPUT;
    }
    else if(pConfig->flushByPort == L2SW_ENABLED)
    {
        if ((retVal = l2sw_setAsicLutFlushType(units, (pConfig->flushStaticAddr == L2SW_ENABLED) ? FLUSHTYPE_BOTH : FLUSHTYPE_DYNAMIC)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutFlushMode(units, FLUSHMDOE_PORT)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutForceFlush(units, 1 << l2sw_switch_port_L2P_get(units, pConfig->port))) != L2SW_ERR_OK)
            return retVal;
    }
    else if(pConfig->flushByMac == L2SW_ENABLED)
    {
        /* Should use API "l2sw_l2_addr_del" to remove a specified entry*/
        return L2SW_ERR_CHIP_NOT_SUPPORTED;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_table_clear(l2sw_uint8 units)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicLutFlushAll(units)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_table_clearStatus_get(l2sw_uint8 units, l2sw_l2_clearStatus_t *pStatus)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pStatus)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutFlushAllStatus(units, (l2sw_uint32 *)pStatus)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_flushLinkDownPortAddrEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicLutLinkDownForceAging(units, enable)) != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_flushLinkDownPortAddrEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutLinkDownForceAging(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_agingEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if(enable == 1)
        enable = 0;
    else
        enable = 1;

    if ((retVal = l2sw_setAsicLutDisableAging(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_agingEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnable)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutDisableAging(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    if(*pEnable == 1)
        *pEnable = 0;
    else
        *pEnable = 1;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitLearningCnt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (mac_cnt > l2sw_switch_maxLutAddrNumber_get(units))
        return L2SW_ERR_LIMITED_L2ENTRY_NUM;

    if ((retVal = l2sw_setAsicLutLearnLimitNo(units, l2sw_switch_port_L2P_get(units, port), mac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitLearningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pMac_cnt)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutLearnLimitNo(units, l2sw_switch_port_L2P_get(units, port), pMac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCnt_set(l2sw_uint8 units, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (mac_cnt > l2sw_switch_maxLutAddrNumber_get(units))
        return L2SW_ERR_LIMITED_L2ENTRY_NUM;

    if ((retVal = l2sw_setAsicSystemLutLearnLimitNo(units, mac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCnt_get(l2sw_uint8 units, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pMac_cnt)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSystemLutLearnLimitNo(units, pMac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitLearningCntAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t action)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 data;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if ( LIMIT_LEARN_CNT_ACTION_DROP == action )
        data = 1;
    else if ( LIMIT_LEARN_CNT_ACTION_FORWARD == action )
        data = 0;
    else if ( LIMIT_LEARN_CNT_ACTION_TO_CPU == action )
        data = 2;
    else
        return L2SW_ERR_NOT_ALLOWED;

    if ((retVal = l2sw_setAsicLutLearnOverAct(units, data)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitLearningCntAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t *pAction)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 action;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutLearnOverAct(units, &action)) != L2SW_ERR_OK)
        return retVal;

    if ( 1 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_DROP;
    else if ( 0 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_FORWARD;
    else if ( 2 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_TO_CPU;
    else
    *pAction = action;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCntAction_set(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t action)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 data;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ( LIMIT_LEARN_CNT_ACTION_DROP == action )
        data = 1;
    else if ( LIMIT_LEARN_CNT_ACTION_FORWARD == action )
        data = 0;
    else if ( LIMIT_LEARN_CNT_ACTION_TO_CPU == action )
        data = 2;
    else
        return L2SW_ERR_NOT_ALLOWED;

    if ((retVal = l2sw_setAsicSystemLutLearnOverAct(units, data)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCntAction_get(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t *pAction)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 action;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAction)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSystemLutLearnOverAct(units, &action)) != L2SW_ERR_OK)
        return retVal;

    if ( 1 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_DROP;
    else if ( 0 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_FORWARD;
    else if ( 2 == action )
        *pAction = LIMIT_LEARN_CNT_ACTION_TO_CPU;
    else
    *pAction = action;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCntPortMask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    /* Check port mask */
    L2SW_CHK_PORTMASK_VALID(units, pPortmask);

    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicSystemLutLearnPortMask(units, pmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitSystemLearningCntPortMask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicSystemLutLearnPortMask(units, &pmask)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitlearningSpeed_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enabled, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);
    
    if(mac_cnt > L2SW_LEARN_LIMIT)
        return L2SW_OVER_LEARN_LIMIT;
    
    if(enabled == 1)
        enabled = 0;
    else
        enabled = 1;
    
    if ((retVal = l2sw_setAsicLutLearnLimitPerSec(units, l2sw_switch_port_L2P_get(units, port), enabled, mac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_limitlearningSpeed_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnabled, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;
    
    if ((retVal = l2sw_getAsicLutLearnLimitPerSec(units, l2sw_switch_port_L2P_get(units, port), pEnabled, pMac_cnt)) != L2SW_ERR_OK)
        return retVal;

    if(*pEnabled == 1)
        *pEnabled = 0;
    else
        *pEnabled = 1;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_learningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pMac_cnt)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutLearnNo(units, l2sw_switch_port_L2P_get(units, port), pMac_cnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_floodPortMask_set(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (floood_type >= FLOOD_END)
        return L2SW_ERR_INPUT;
    
    /* check port valid */
    L2SW_CHK_PORTMASK_VALID(units, pFlood_portmask);
    
    /* Get Physical port mask */
    if ((retVal = l2sw_switch_portmask_L2P_get(units, pFlood_portmask, &pmask))!=L2SW_ERR_OK)
        return retVal;
    
    switch (floood_type)
    {
        case FLOOD_UNKNOWNDA:
            if ((retVal = l2sw_setAsicPortUnknownDaFloodingPortmask(units, pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        case FLOOD_UNKNOWNMC:
            if ((retVal = l2sw_setAsicPortUnknownMulticastFloodingPortmask(units, pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        case FLOOD_BC:
            if ((retVal = l2sw_setAsicPortBcastFloodingPortmask(units, pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_floodPortMask_get(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pmask = 0;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (floood_type >= FLOOD_END)
        return L2SW_ERR_INPUT;

    if(NULL == pFlood_portmask)
        return L2SW_ERR_NULL_POINTER;

    switch (floood_type)
    {
        case FLOOD_UNKNOWNDA:
            if ((retVal = l2sw_getAsicPortUnknownDaFloodingPortmask(units, &pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        case FLOOD_UNKNOWNMC:
            if ((retVal = l2sw_getAsicPortUnknownMulticastFloodingPortmask(units, &pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        case FLOOD_BC:
            if ((retVal = l2sw_getAsicPortBcastFloodingPortmask(units, &pmask)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            break;
    }
    
    /* Get Logical port mask */
    if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pFlood_portmask))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_localPktPermit_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t permit)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (permit >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if ((retVal = l2sw_setAsicPortBlockSpa(units, l2sw_switch_port_L2P_get(units, port), permit)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_localPktPermit_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pPermit)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(NULL == pPermit)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicPortBlockSpa(units, l2sw_switch_port_L2P_get(units, port), pPermit)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_aging_set(l2sw_uint8 units, l2sw_l2_age_time_t aging_time)
{
    l2sw_uint32 i;
    CONST_T l2sw_uint32 agePara[19][3] = {
        {15,  1, 0}, {28,  2, 0}, {42,  3, 0}, {57,  4, 0}, {72,  5, 0}, 
        {86,  6, 0}, {100, 7, 0}, {114, 4, 1}, {143, 5, 1}, {171, 6, 1},
        {200, 7, 1}, {236, 4, 2}, {295, 5, 2}, {354, 6, 2}, {413, 7, 2},
        {472, 4, 3}, {590, 5, 3}, {708, 6, 3}, {800, 7, 3}
    };

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (aging_time>agePara[18][0])
        return L2SW_ERR_OUT_OF_RANGE;

    for (i = 0; i<19; i++)
    {
        if (aging_time<=agePara[i][0])
        {
            return l2sw_setAsicLutAgeTimerSpeed(units, agePara[i][1], agePara[i][2]);
        }
    }

    return L2SW_ERR_FAILED;
}

static l2sw_api_ret_t _l2sw_l2_aging_get(l2sw_uint8 units, l2sw_l2_age_time_t *pAging_time)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 i,time, speed;
    CONST_T l2sw_uint32 agePara[19][3] = {
        {15,  1, 0}, {28,  2, 0}, {42,  3, 0}, {57,  4, 0}, {72,  5, 0}, 
        {86,  6, 0}, {100, 7, 0}, {114, 4, 1}, {143, 5, 1}, {171, 6, 1},
        {200, 7, 1}, {236, 4, 2}, {295, 5, 2}, {354, 6, 2}, {413, 7, 2},
        {472, 4, 3}, {590, 5, 3}, {708, 6, 3}, {800, 7, 3}
    };

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pAging_time)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicLutAgeTimerSpeed(units, &time, &speed)) != L2SW_ERR_OK)
        return retVal;

    for (i = 0; i<19; i++)
    {
        if (time==agePara[i][1]&&speed==agePara[i][2])
        {
            *pAging_time = agePara[i][0];
            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_FAILED;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddrLookup_set(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(type == LOOKUP_MAC)
    {
        if((retVal = l2sw_setAsicLutIpMulticastLookup(units, L2SW_DISABLED)) != L2SW_ERR_OK)
            return retVal;
    }
    else if(type == LOOKUP_IP)
    {
        if((retVal = l2sw_setAsicLutIpMulticastLookup(units, L2SW_ENABLED)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutIpMulticastVidLookup(units, L2SW_DISABLED))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutIpLookupMethod(units, 1))!=L2SW_ERR_OK)
            return retVal;
    }
    else if(type == LOOKUP_IP_VID)
    {
        if((retVal = l2sw_setAsicLutIpMulticastLookup(units, L2SW_ENABLED)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutIpMulticastVidLookup(units, L2SW_ENABLED))!=L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicLutIpLookupMethod(units, 1))!=L2SW_ERR_OK)
            return retVal;
    }
    else
        return L2SW_ERR_INPUT;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastAddrLookup_get(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t *pType)
{
    l2sw_api_ret_t       retVal;
    l2sw_uint32          enabled, vid_lookup;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pType)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicLutIpMulticastLookup(units, &enabled)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicLutIpMulticastVidLookup(units, &vid_lookup))!=L2SW_ERR_OK)
        return retVal;

    if(enabled == L2SW_ENABLED)
    {
        if(vid_lookup == L2SW_ENABLED)
            *pType = LOOKUP_IP_VID;
        else
            *pType = LOOKUP_IP;
    }
    else
        *pType = LOOKUP_MAC;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastForwardRouterPort_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t       retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (enabled >= L2SW_ENABLE_END)
        return L2SW_ERR_ENABLE;

    if((retVal = l2sw_setAsicLutIpmcFwdRouterPort(units, enabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastForwardRouterPort_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t       retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (NULL == pEnabled)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicLutIpmcFwdRouterPort(units, pEnabled)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastGroupEntry_add(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask)
{
    l2sw_uint32      empty_idx = 0xFFFF;
    l2sw_int32       index;
    ipaddr_t        group_addr;
    l2sw_uint32      group_vid;
    l2sw_uint32      pmask;
    l2sw_uint32      valid;
    l2sw_uint32      physicalPortmask;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    if((ip_addr & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    /* Get Physical port mask */
    if ((retVal = l2sw_switch_portmask_L2P_get(units, pPortmask, &physicalPortmask))!=L2SW_ERR_OK)
        return retVal;

    for(index = 0; index <= L2SW_LUT_IPMCGRP_TABLE_MAX; index++)
    {
        if ((retVal = l2sw_getAsicLutIPMCGroup(units, (l2sw_uint32)index, &group_addr, &group_vid, &pmask, &valid))!=L2SW_ERR_OK)
            return retVal;

        if( (valid == L2SW_ENABLED) && (group_addr == ip_addr) && (group_vid == vid) )
        {
            if(pmask != physicalPortmask)
            {
                pmask = physicalPortmask;
                if ((retVal = l2sw_setAsicLutIPMCGroup(units, index, ip_addr, vid, pmask, valid))!=L2SW_ERR_OK)
                    return retVal;
            }

            return L2SW_ERR_OK;
        }

        if( (valid == L2SW_DISABLED) && (empty_idx == 0xFFFF) ) /* Unused */
            empty_idx = (l2sw_uint32)index;
    }

    if(empty_idx == 0xFFFF)
        return L2SW_ERR_TBL_FULL;

    pmask = physicalPortmask;
    if ((retVal = l2sw_setAsicLutIPMCGroup(units, empty_idx, ip_addr, vid, pmask, L2SW_ENABLED))!=L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastGroupEntry_del(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid)
{
    l2sw_int32       index;
    ipaddr_t        group_addr;
    l2sw_uint32      group_vid;
    l2sw_uint32      pmask;
    l2sw_uint32      valid;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if((ip_addr & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    for(index = 0; index <= L2SW_LUT_IPMCGRP_TABLE_MAX; index++)
    {
        if ((retVal = l2sw_getAsicLutIPMCGroup(units, (l2sw_uint32)index, &group_addr, &group_vid, &pmask, &valid))!=L2SW_ERR_OK)
            return retVal;

        if( (valid == L2SW_ENABLED) && (group_addr == ip_addr) && (group_vid == vid) )
        {
            group_addr = 0xE0000000;
            group_vid = 0;
            pmask = 0;
            if ((retVal = l2sw_setAsicLutIPMCGroup(units, index, group_addr, group_vid, pmask, L2SW_DISABLED))!=L2SW_ERR_OK)
                return retVal;

            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_FAILED;
}

static l2sw_api_ret_t _l2sw_l2_ipMcastGroupEntry_get(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask)
{
    l2sw_int32       index;
    ipaddr_t        group_addr;
    l2sw_uint32      group_vid;
    l2sw_uint32      valid;
    l2sw_uint32      pmask;
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if((ip_addr & 0xF0000000) != 0xE0000000)
        return L2SW_ERR_INPUT;

    if (vid > L2SW_VIDMAX)
        return L2SW_ERR_L2_VID;

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    for(index = 0; index <= L2SW_LUT_IPMCGRP_TABLE_MAX; index++)
    {
        if ((retVal = l2sw_getAsicLutIPMCGroup(units, (l2sw_uint32)index, &group_addr, &group_vid, &pmask, &valid))!=L2SW_ERR_OK)
            return retVal;

        if( (valid == L2SW_ENABLED) && (group_addr == ip_addr) && (group_vid == vid) )
        {
            if ((retVal = l2sw_switch_portmask_P2L_get(units, pmask, pPortmask))!=L2SW_ERR_OK)
                return retVal;

            return L2SW_ERR_OK;
        }
    }

    return L2SW_ERR_FAILED;
}

static l2sw_api_ret_t _l2sw_l2_entry_get(l2sw_uint8 units, l2sw_l2_addr_table_t *pL2_entry)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 method;
    l2sw_luttb l2Table;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (pL2_entry->index >= l2sw_switch_maxLutAddrNumber_get(units))
        return L2SW_ERR_INPUT;

    MEMSET(&l2Table, 0x00, sizeof(l2sw_luttb));
    l2Table.address= pL2_entry->index;
    method = LUTREADMETHOD_ADDRESS;
    if ((retVal = l2sw_getAsicL2LookupTb(units, method, &l2Table)) != L2SW_ERR_OK)
        return retVal;

    if (l2Table.lookup_hit == 0)
         return L2SW_ERR_L2_EMPTY_ENTRY;

    if(l2Table.l3lookup)
    {
        if(l2Table.l3vidlookup)
        {
            MEMSET(&pL2_entry->mac, 0, sizeof(l2sw_mac_t));
            pL2_entry->is_ipmul  = l2Table.l3lookup;
            pL2_entry->sip       = l2Table.sip;
            pL2_entry->dip       = l2Table.dip;
            pL2_entry->is_static = l2Table.nosalearn;

            /* Get Logical port mask */
            if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &(pL2_entry->portmask)))!=L2SW_ERR_OK)
                return retVal;

            pL2_entry->fid       = 0;
            pL2_entry->age       = 0;
            pL2_entry->auth      = 0;
            pL2_entry->sa_block  = 0;
            pL2_entry->is_ipvidmul = 1;
            pL2_entry->l3_vid      = l2Table.l3_vid;
        }
        else
        {
            MEMSET(&pL2_entry->mac, 0, sizeof(l2sw_mac_t));
            pL2_entry->is_ipmul  = l2Table.l3lookup;
            pL2_entry->sip       = l2Table.sip;
            pL2_entry->dip       = l2Table.dip;
            pL2_entry->is_static = l2Table.nosalearn;

            /* Get Logical port mask */
            if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &(pL2_entry->portmask)))!=L2SW_ERR_OK)
                return retVal;

            pL2_entry->fid       = 0;
            pL2_entry->age       = 0;
            pL2_entry->auth      = 0;
            pL2_entry->sa_block  = 0;
            pL2_entry->is_ipvidmul = 0;
            pL2_entry->l3_vid      = 0;
        }
    }
    else if(l2Table.mac.octet[0]&0x01)
    {
        MEMSET(&pL2_entry->sip, 0, sizeof(ipaddr_t));
        MEMSET(&pL2_entry->dip, 0, sizeof(ipaddr_t));
        pL2_entry->mac.octet[0] = l2Table.mac.octet[0];
        pL2_entry->mac.octet[1] = l2Table.mac.octet[1];
        pL2_entry->mac.octet[2] = l2Table.mac.octet[2];
        pL2_entry->mac.octet[3] = l2Table.mac.octet[3];
        pL2_entry->mac.octet[4] = l2Table.mac.octet[4];
        pL2_entry->mac.octet[5] = l2Table.mac.octet[5];
        pL2_entry->is_ipmul  = l2Table.l3lookup;
        pL2_entry->is_static = l2Table.nosalearn;

        /* Get Logical port mask */
        if ((retVal = l2sw_switch_portmask_P2L_get(units, l2Table.mbr, &(pL2_entry->portmask)))!=L2SW_ERR_OK)
            return retVal;

        pL2_entry->ivl       = l2Table.ivl_svl;
        if(l2Table.ivl_svl == 1) /* IVL */
        {
            pL2_entry->cvid      = l2Table.cvid_fid;
            pL2_entry->fid       = 0;
        }
        else /* SVL*/
        {
            pL2_entry->cvid      = 0;
            pL2_entry->fid       = l2Table.cvid_fid;
        }
        pL2_entry->auth      = l2Table.auth;
        pL2_entry->sa_block  = l2Table.sa_block;
        pL2_entry->age       = 0;
        pL2_entry->is_ipvidmul = 0;
        pL2_entry->l3_vid      = 0;
    }
    else if((l2Table.age != 0)||(l2Table.nosalearn == 1))
    {
        MEMSET(&pL2_entry->sip, 0, sizeof(ipaddr_t));
        MEMSET(&pL2_entry->dip, 0, sizeof(ipaddr_t));
        pL2_entry->mac.octet[0] = l2Table.mac.octet[0];
        pL2_entry->mac.octet[1] = l2Table.mac.octet[1];
        pL2_entry->mac.octet[2] = l2Table.mac.octet[2];
        pL2_entry->mac.octet[3] = l2Table.mac.octet[3];
        pL2_entry->mac.octet[4] = l2Table.mac.octet[4];
        pL2_entry->mac.octet[5] = l2Table.mac.octet[5];
        pL2_entry->is_ipmul  = l2Table.l3lookup;
        pL2_entry->is_static = l2Table.nosalearn;

        /* Get Logical port mask */
        if ((retVal = l2sw_switch_portmask_P2L_get(units, 1<<(l2Table.spa), &(pL2_entry->portmask)))!=L2SW_ERR_OK)
            return retVal;

        pL2_entry->ivl       = l2Table.ivl_svl;
        pL2_entry->cvid      = l2Table.cvid_fid;
        pL2_entry->fid       = l2Table.fid;
        pL2_entry->auth      = l2Table.auth;
        pL2_entry->sa_block  = l2Table.sa_block;
        pL2_entry->age       = l2Table.age;
        pL2_entry->is_ipvidmul = 0;
        pL2_entry->l3_vid      = 0;
    }
    else
       return L2SW_ERR_L2_EMPTY_ENTRY;

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_l2_init
 * Description:
 *      Initialize l2 module of the specified device.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      Initialize l2 module before calling any l2 APIs.
 */
l2sw_api_ret_t l2sw_l2_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_init(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_l2_addr_add
 * Description:
 *      Add LUT unicast entry.
 * Input:
 *      pMac - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 *      pL2_data - Unicast entry parameter
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_MAC              - Invalid MAC address.
 *      L2SW_ERR_L2_FID           - Invalid FID .
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      If the unicast mac address already existed in LUT, it will udpate the status of the entry.
 *      Otherwise, it will find an empty or asic auto learned entry to write. If all the entries
 *      with the same hash value can't be replaced, ASIC will return a L2SW_ERR_L2_INDEXTBL_FULL error.
 */
l2sw_api_ret_t l2sw_l2_addr_add(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_addr_add(units, pMac, pL2_data);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_addr_get
 * Description:
 *      Get LUT unicast entry.
 * Input:
 *      pMac    - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 * Output:
 *      pL2_data - Unicast entry parameter
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the unicast mac address existed in LUT, it will return the port and fid where
 *      the mac is learned. Otherwise, it will return a L2SW_ERR_L2_ENTRY_NOTFOUND error.
 */
l2sw_api_ret_t l2sw_l2_addr_get(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_addr_get(units, pMac, pL2_data);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_addr_next_get
 * Description:
 *      Get Next LUT unicast entry.
 * Input:
 *      read_method     - The reading method.
 *      port            - The port number if the read_metohd is READMETHOD_NEXT_L2UCSPA
 *      pAddress        - The Address ID
 * Output:
 *      pL2_data - Unicast entry parameter
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next unicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all entries is LUT.
 */
l2sw_api_ret_t l2sw_l2_addr_next_get(l2sw_uint8 units, l2sw_l2_read_method_t read_method, l2sw_port_t port, l2sw_uint32 *pAddress, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_addr_next_get(units, read_method, port, pAddress, pL2_data);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_addr_del
 * Description:
 *      Delete LUT unicast entry.
 * Input:
 *      pMac - 6 bytes unicast(I/G bit is 0) mac address to be written into LUT.
 *      fid - Filtering database
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the mac has existed in the LUT, it will be deleted. Otherwise, it will return L2SW_ERR_L2_ENTRY_NOTFOUND.
 */
l2sw_api_ret_t l2sw_l2_addr_del(l2sw_uint8 units, l2sw_mac_t *pMac, l2sw_l2_ucastAddr_t *pL2_data)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_addr_del(units, pMac, pL2_data);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_mcastAddr_add
 * Description:
 *      Add LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_MAC              - Invalid MAC address.
 *      L2SW_ERR_L2_FID           - Invalid FID .
 *      L2SW_ERR_L2_VID           - Invalid VID .
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      If the multicast mac address already existed in the LUT, it will udpate the
 *      port mask of the entry. Otherwise, it will find an empty or asic auto learned
 *      entry to write. If all the entries with the same hash value can't be replaced,
 *      ASIC will return a L2SW_ERR_L2_INDEXTBL_FULL error.
 */
l2sw_api_ret_t l2sw_l2_mcastAddr_add(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_mcastAddr_add(units, pMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_mcastAddr_get
 * Description:
 *      Get LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      pMcastAddr  - L2 multicast entry structure
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_VID               - Invalid VID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the multicast mac address existed in the LUT, it will return the port where
 *      the mac is learned. Otherwise, it will return a L2SW_ERR_L2_ENTRY_NOTFOUND error.
 */
l2sw_api_ret_t l2sw_l2_mcastAddr_get(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_mcastAddr_get(units, pMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_mcastAddr_next_get
 * Description:
 *      Get Next L2 Multicast entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pMcastAddr  - L2 multicast entry structure
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next L2 multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all multicast entries is LUT.
 */
l2sw_api_ret_t l2sw_l2_mcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_mcastAddr_next_get(units, pAddress, pMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_mcastAddr_del
 * Description:
 *      Delete LUT multicast entry.
 * Input:
 *      pMcastAddr  - L2 multicast entry structure
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_MAC                  - Invalid MAC address.
 *      L2SW_ERR_L2_FID               - Invalid FID .
 *      L2SW_ERR_L2_VID               - Invalid VID .
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      If the mac has existed in the LUT, it will be deleted. Otherwise, it will return L2SW_ERR_L2_ENTRY_NOTFOUND.
 */
l2sw_api_ret_t l2sw_l2_mcastAddr_del(l2sw_uint8 units, l2sw_l2_mcastAddr_t *pMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_mcastAddr_del(units, pMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddr_add
 * Description:
 *      Add Lut IP multicast entry
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      System supports L2 entry with IP multicast DIP/SIP to forward IP multicasting frame as user
 *      desired. If this function is enabled, then system will be looked up L2 IP multicast entry to
 *      forward IP multicast frame directly without flooding.
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddr_add(units, pIpMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddr_get
 * Description:
 *      Get LUT IP multicast entry.
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      pIpMcastAddr    - IP Multicast entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      The API can get Lut table of IP multicast entry.
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddr_get(units, pIpMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddr_next_get
 * Description:
 *      Get Next IP Multicast entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pIpMcastAddr    - IP Multicast entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next IP multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all IP multicast entries is LUT.
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddr_next_get(units, pAddress, pIpMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddr_del
 * Description:
 *      Delete a ip multicast address entry from the specified device.
 * Input:
 *      pIpMcastAddr    - IP Multicast entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      The API can delete a IP multicast address entry from the specified device.
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipMcastAddr_t *pIpMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddr_del(units, pIpMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_add
 * Description:
 *      Add Lut IP multicast+VID entry
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_L2_INDEXTBL_FULL - hashed index is full of entries.
 *      L2SW_ERR_PORT_MASK        - Invalid portmask.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_add(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipVidMcastAddr_add(units, pIpVidMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_get
 * Description:
 *      Get LUT IP multicast+VID entry.
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_get(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipVidMcastAddr_get(units, pIpVidMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_next_get
 * Description:
 *      Get Next IP Multicast+VID entry.
 * Input:
 *      pAddress        - The Address ID
 * Output:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *      Get the next IP multicast entry after the current entry pointed by pAddress.
 *      The address of next entry is returned by pAddress. User can use (address + 1)
 *      as pAddress to call this API again for dumping all IP multicast entries is LUT.
 */
l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_next_get(l2sw_uint8 units, l2sw_uint32 *pAddress, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipVidMcastAddr_next_get(units, pAddress, pIpVidMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipVidMcastAddr_del
 * Description:
 *      Delete a ip multicast+VID address entry from the specified device.
 * Input:
 *      pIpVidMcastAddr - IP & VID multicast Entry
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_L2_ENTRY_NOTFOUND    - No such LUT entry.
 *      L2SW_ERR_INPUT                - Invalid input parameters.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_ipVidMcastAddr_del(l2sw_uint8 units, l2sw_l2_ipVidMcastAddr_t *pIpVidMcastAddr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipVidMcastAddr_del(units, pIpVidMcastAddr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ucastAddr_flush
 * Description:
 *      Flush L2 mac address by type in the specified device (both dynamic and static).
 * Input:
 *      pConfig - flush configuration
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_VLAN_VID     - Invalid VID parameter.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      flushByVid          - 1: Flush by VID, 0: Don't flush by VID
 *      vid                 - VID (0 ~ 4095)
 *      flushByFid          - 1: Flush by FID, 0: Don't flush by FID
 *      fid                 - FID (0 ~ 15)
 *      flushByPort         - 1: Flush by Port, 0: Don't flush by Port
 *      port                - Port ID
 *      flushByMac          - Not Supported
 *      ucastAddr           - Not Supported
 *      flushStaticAddr     - 1: Flush both Static and Dynamic entries, 0: Flush only Dynamic entries
 *      flushAddrOnAllPorts - 1: Flush VID-matched entries at all ports, 0: Flush VID-matched entries per port.
 */
l2sw_api_ret_t l2sw_l2_ucastAddr_flush(l2sw_uint8 units, l2sw_l2_flushCfg_t *pConfig)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ucastAddr_flush(units, pConfig);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_table_clear
 * Description:
 *      Flush all static & dynamic entries in LUT.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_table_clear(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_table_clear(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_table_clearStatus_get
 * Description:
 *      Get table clear status
 * Input:
 *      None
 * Output:
 *      pStatus - Clear status, 1:Busy, 0:finish
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_table_clearStatus_get(l2sw_uint8 units, l2sw_l2_clearStatus_t *pStatus)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_table_clearStatus_get(units, pStatus);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_flushLinkDownPortAddrEnable_set
 * Description:
 *      Set HW flush linkdown port mac configuration of the specified device.
 * Input:
 *      port - Port id.
 *      enable - link down flush status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      The status of flush linkdown port address is as following:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_l2_flushLinkDownPortAddrEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_flushLinkDownPortAddrEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_flushLinkDownPortAddrEnable_get
 * Description:
 *      Get HW flush linkdown port mac configuration of the specified device.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - link down flush status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The status of flush linkdown port address is as following:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_l2_flushLinkDownPortAddrEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_flushLinkDownPortAddrEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_agingEnable_set
 * Description:
 *      Set L2 LUT aging status per port setting.
 * Input:
 *      port    - Port id.
 *      enable  - Aging status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable input.
 * Note:
 *      This API can be used to set L2 LUT aging status per port.
 */
l2sw_api_ret_t l2sw_l2_agingEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_agingEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_agingEnable_get
 * Description:
 *      Get L2 LUT aging status per port setting.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Aging status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can be used to get L2 LUT aging function per port.
 */
l2sw_api_ret_t l2sw_l2_agingEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_agingEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitLearningCnt_set
 * Description:
 *      Set per-Port auto learning limit number
 * Input:
 *      port    - Port id.
 *      mac_cnt - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_PORT_ID              - Invalid port number.
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      The API can set per-port ASIC auto learning limit number from 0(disable learning)
 *      to 2112.
 */
l2sw_api_ret_t l2sw_l2_limitLearningCnt_set(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitLearningCnt_set(units, port, mac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitLearningCnt_get
 * Description:
 *      Get per-Port auto learning limit number
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt - Auto learning entries limit number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get per-port ASIC auto learning limit number.
 */
l2sw_api_ret_t l2sw_l2_limitLearningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitLearningCnt_get(units, port, pMac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCnt_set
 * Description:
 *      Set System auto learning limit number
 * Input:
 *      mac_cnt - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_LIMITED_L2ENTRY_NUM  - Invalid auto learning limit number
 * Note:
 *      The API can set system ASIC auto learning limit number from 0(disable learning)
 *      to 2112.
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCnt_set(l2sw_uint8 units, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCnt_set(units, mac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCnt_get
 * Description:
 *      Get System auto learning limit number
 * Input:
 *      None
 * Output:
 *      pMac_cnt - Auto learning entries limit number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get system ASIC auto learning limit number.
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCnt_get(l2sw_uint8 units, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCnt_get(units, pMac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitLearningCntAction_set
 * Description:
 *      Configure auto learn over limit number action.
 * Input:
 *      port - Port id.
 *      action - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      The API can set SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
l2sw_api_ret_t l2sw_l2_limitLearningCntAction_set(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitLearningCntAction_set(units, port, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitLearningCntAction_get
 * Description:
 *      Get auto learn over limit number action.
 * Input:
 *      port - Port id.
 * Output:
 *      pAction - Learn over action
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
l2sw_api_ret_t l2sw_l2_limitLearningCntAction_get(l2sw_uint8 units, l2sw_port_t port, l2sw_l2_limitLearnCntAction_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitLearningCntAction_get(units, port, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntAction_set
 * Description:
 *      Configure system auto learn over limit number action.
 * Input:
 *      port - Port id.
 *      action - Auto learning entries limit number
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_NOT_ALLOWED  - Invalid learn over action
 * Note:
 *      The API can set SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCntAction_set(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t action)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCntAction_set(units, action);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntAction_get
 * Description:
 *      Get system auto learn over limit number action.
 * Input:
 *      None.
 * Output:
 *      pAction - Learn over action
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get SA unknown packet action while auto learn limit number is over
 *      The action symbol as following:
 *      - LIMIT_LEARN_CNT_ACTION_DROP,
 *      - LIMIT_LEARN_CNT_ACTION_FORWARD,
 *      - LIMIT_LEARN_CNT_ACTION_TO_CPU,
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCntAction_get(l2sw_uint8 units, l2sw_l2_limitLearnCntAction_t *pAction)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCntAction_get(units, pAction);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntPortMask_set
 * Description:
 *      Configure system auto learn portmask
 * Input:
 *      pPortmask - Port Mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid port mask.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCntPortMask_set(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCntPortMask_set(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitSystemLearningCntPortMask_get
 * Description:
 *      get system auto learn portmask
 * Input:
 *      None
 * Output:
 *      pPortmask - Port Mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_NULL_POINTER - Null pointer.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_limitSystemLearningCntPortMask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitSystemLearningCntPortMask_get(units, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitlearningSpeed_set
 * Description:
 *      Get per-Port per-Second current auto learning number 
 * Input:
 *      port      - Port id.
 *      enabled   - Port Status.
 *      mac_cnt   - Auto learning entries limit number per-Second.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_OVER_LEARN_LIMIT     - Invalid auto learning limit number.
 * Note:
 *      The API can set per-Second per-port ASIC auto learning number enable or disable
 */
l2sw_api_ret_t l2sw_l2_limitlearningSpeed_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enabled, l2sw_mac_cnt_t mac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitlearningSpeed_set(units, port, enabled, mac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_limitlearningSpeed_get
 * Description:
 *      Get per-Port per-Second current auto learning number 
 * Input:
 *      port    - Port id.
 * Output:
 *      pMac_cnt  - Learning counts per-Second.   
 *      pEnabled  - Port Status.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_NULL_POINTER     - NULL Points.
 * Note:
 *      The API can set per-Second per-port ASIC auto learning number enable or disable
 */
l2sw_api_ret_t l2sw_l2_limitlearningSpeed_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnabled, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_limitlearningSpeed_get(units, port, pEnabled, pMac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_learningCnt_get
 * Description:
 *      Get per-Port current auto learning number
 * Input:
 *      port - Port id.
 * Output:
 *      pMac_cnt - ASIC auto learning entries number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get per-port ASIC auto learning number
 */
l2sw_api_ret_t l2sw_l2_learningCnt_get(l2sw_uint8 units, l2sw_port_t port, l2sw_mac_cnt_t *pMac_cnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_learningCnt_get(units, port, pMac_cnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_floodPortMask_set
 * Description:
 *      Set flooding portmask
 * Input:
 *      type - flooding type.
 *      pFlood_portmask - flooding porkmask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_MASK    - Invalid portmask.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API can set the flooding mask.
 *      The flooding type is as following:
 *      - FLOOD_UNKNOWNDA
 *      - FLOOD_UNKNOWNMC
 *      - FLOOD_BC
 */
l2sw_api_ret_t l2sw_l2_floodPortMask_set(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_floodPortMask_set(units, floood_type, pFlood_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}
/* Function Name:
 *      l2sw_l2_floodPortMask_get
 * Description:
 *      Get flooding portmask
 * Input:
 *      type - flooding type.
 * Output:
 *      pFlood_portmask - flooding porkmask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API can get the flooding mask.
 *      The flooding type is as following:
 *      - FLOOD_UNKNOWNDA
 *      - FLOOD_UNKNOWNMC
 *      - FLOOD_BC
 */
l2sw_api_ret_t l2sw_l2_floodPortMask_get(l2sw_uint8 units, l2sw_l2_flood_type_t floood_type, l2sw_portmask_t *pFlood_portmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_floodPortMask_get(units, floood_type, pFlood_portmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_localPktPermit_set
 * Description:
 *      Set permittion of frames if source port and destination port are the same.
 * Input:
 *      port - Port id.
 *      permit - permittion status
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid permit value.
 * Note:
 *      This API is setted to permit frame if its source port is equal to destination port.
 */
l2sw_api_ret_t l2sw_l2_localPktPermit_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t permit)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_localPktPermit_set(units, port, permit);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_localPktPermit_get
 * Description:
 *      Get permittion of frames if source port and destination port are the same.
 * Input:
 *      port - Port id.
 * Output:
 *      pPermit - permittion status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      This API is to get permittion status for frames if its source port is equal to destination port.
 */
l2sw_api_ret_t l2sw_l2_localPktPermit_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pPermit)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_localPktPermit_get(units, port, pPermit);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_aging_set
 * Description:
 *      Set LUT agging out speed
 * Input:
 *      aging_time - Agging out time.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_OUT_OF_RANGE     - input out of range.
 * Note:
 *      The API can set LUT agging out period for each entry and the range is from 45s to 458s.
 */
l2sw_api_ret_t l2sw_l2_aging_set(l2sw_uint8 units, l2sw_l2_age_time_t aging_time)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_aging_set(units, aging_time);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_aging_get
 * Description:
 *      Get LUT agging out time
 * Input:
 *      None
 * Output:
 *      pEnable - Aging status
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get LUT agging out period for each entry.
 */
l2sw_api_ret_t l2sw_l2_aging_get(l2sw_uint8 units, l2sw_l2_age_time_t *pAging_time)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_aging_get(units, pAging_time);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddrLookup_set
 * Description:
 *      Set Lut IP multicast lookup function
 * Input:
 *      type - Lookup type for IPMC packet.
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      LOOKUP_MAC      - Lookup by MAC address
 *      LOOKUP_IP       - Lookup by IP address
 *      LOOKUP_IP_VID   - Lookup by IP address & VLAN ID
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddrLookup_set(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddrLookup_set(units, type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastAddrLookup_get
 * Description:
 *      Get Lut IP multicast lookup function
 * Input:
 *      None.
 * Output:
 *      pType - Lookup type for IPMC packet.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_l2_ipMcastAddrLookup_get(l2sw_uint8 units, l2sw_l2_ipmc_lookup_type_t *pType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastAddrLookup_get(units, pType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastForwardRouterPort_set
 * Description:
 *      Set IPMC packet forward to rounter port also or not
 * Input:
 *      enabled - 1: Inlcude router port, 0, exclude router port
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_ipMcastForwardRouterPort_set(l2sw_uint8 units, l2sw_enable_t enabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastForwardRouterPort_set(units, enabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastForwardRouterPort_get
 * Description:
 *      Get IPMC packet forward to rounter port also or not
 * Input:
 *      None.
 * Output:
 *      pEnabled    - 1: Inlcude router port, 0, exclude router port
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_l2_ipMcastForwardRouterPort_get(l2sw_uint8 units, l2sw_enable_t *pEnabled)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastForwardRouterPort_get(units, pEnabled);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_add
 * Description:
 *      Add an IP Multicast entry to group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 *      pPortmask   - portmask
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Add an entry to IP Multicast Group table.
 */
l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_add(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastGroupEntry_add(units, ip_addr, vid, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_del
 * Description:
 *      Delete an entry from IP Multicast group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Delete an entry from IP Multicast group table.
 */
l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_del(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastGroupEntry_del(units, ip_addr, vid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_ipMcastGroupEntry_get
 * Description:
 *      get an entry from IP Multicast group table
 * Input:
 *      ip_addr     - IP address
 *      vid         - VLAN ID
 * Output:
 *      pPortmask   - member port mask
 * Return:
 *      L2SW_ERR_OK          - OK
 *      L2SW_ERR_FAILED      - Failed
 *      L2SW_ERR_SMI         - SMI access error
 *      L2SW_ERR_TBL_FULL    - Table Full
 * Note:
 *      Delete an entry from IP Multicast group table.
 */
l2sw_api_ret_t l2sw_l2_ipMcastGroupEntry_get(l2sw_uint8 units, ipaddr_t ip_addr, l2sw_uint32 vid, l2sw_portmask_t *pPortmask)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_ipMcastGroupEntry_get(units, ip_addr, vid, pPortmask);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_l2_entry_get
 * Description:
 *      Get LUT unicast entry.
 * Input:
 *      pL2_entry - Index field in the structure.
 * Output:
 *      pL2_entry - other fields such as MAC, port, age...
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_L2_EMPTY_ENTRY   - Empty LUT entry.
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 * Note:
 *      This API is used to get address by index from 0~2111.
 */
l2sw_api_ret_t l2sw_l2_entry_get(l2sw_uint8 units, l2sw_l2_addr_table_t *pL2_entry)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_l2_entry_get(units, pL2_entry);
    L2SW_API_UNLOCK(units);

    return retVal;
}



