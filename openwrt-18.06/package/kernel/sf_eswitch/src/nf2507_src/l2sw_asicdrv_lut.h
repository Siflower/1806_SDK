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

#ifndef _L2SW_ASICDRV_LUT_H_
#define _L2SW_ASICDRV_LUT_H_

#include "l2sw_asicdrv.h"

#define L2SW_LUT_AGETIMERMAX        (7)
#define L2SW_LUT_AGESPEEDMAX        (3)
#define L2SW_LUT_LEARNLIMITMAX      (0x1040)
#define L2SW_LUT_ADDRMAX            (0x103F)
#define L2SW_LUT_IPMCGRP_TABLE_MAX  (0x3F)
#define L2SW_LUT_ENTRY_SIZE         (6)
#define L2SW_LUT_BUSY_CHECK_NO      (10)

#define L2SW_LUT_TABLE_SIZE         (6)

enum L2SW_LUTHASHMETHOD{

    LUTHASHMETHOD_SVL=0,
    LUTHASHMETHOD_IVL,
    LUTHASHMETHOD_END,
};


enum L2SW_LRNOVERACT{

    LRNOVERACT_FORWARD=0,
    LRNOVERACT_DROP,
    LRNOVERACT_TRAP,
    LRNOVERACT_END,
};

enum L2SW_LUTREADMETHOD{

    LUTREADMETHOD_MAC =0,
    LUTREADMETHOD_ADDRESS,
    LUTREADMETHOD_NEXT_ADDRESS,
    LUTREADMETHOD_NEXT_L2UC,
    LUTREADMETHOD_NEXT_L2MC,
    LUTREADMETHOD_NEXT_L3MC,
    LUTREADMETHOD_NEXT_L2L3MC,
    LUTREADMETHOD_NEXT_L2UCSPA,
};

enum L2SW_FLUSHMODE
{
    FLUSHMDOE_PORT = 0,
    FLUSHMDOE_VID,
    FLUSHMDOE_FID,
    FLUSHMDOE_END,
};

enum L2SW_FLUSHTYPE
{
    FLUSHTYPE_DYNAMIC = 0,
    FLUSHTYPE_BOTH,
    FLUSHTYPE_END,
};


typedef struct LUTTABLE{

    ipaddr_t sip;
    ipaddr_t dip;
    ether_addr_t mac;
    l2sw_uint16 ivl_svl:1;
    l2sw_uint16 cvid_fid:12;
    l2sw_uint16 fid:4;
    l2sw_uint16 efid:3;

    l2sw_uint16 nosalearn:1;
    l2sw_uint16 da_block:1;
    l2sw_uint16 sa_block:1;
    l2sw_uint16 auth:1;
    l2sw_uint16 lut_pri:3;
    l2sw_uint16 sa_en:1;
    l2sw_uint16 fwd_en:1;
    l2sw_uint16 mbr:11;
    l2sw_uint16 spa:4;
    l2sw_uint16 age:3;
    l2sw_uint16 l3lookup:1;
    l2sw_uint16 igmp_asic:1;
    l2sw_uint16 igmpidx:8;

    l2sw_uint16 lookup_hit:1;
    l2sw_uint16 lookup_busy:1;
    l2sw_uint16 address:13;

    l2sw_uint16 l3vidlookup:1;
    l2sw_uint16 l3_vid:12;

    l2sw_uint16 wait_time;

}l2sw_luttb;

extern ret_t l2sw_setAsicLutIpMulticastLookup(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicLutIpMulticastLookup(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicLutIpMulticastVidLookup(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicLutIpMulticastVidLookup(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicLutAgeTimerSpeed(l2sw_uint8 units, l2sw_uint32 timer, l2sw_uint32 speed);
extern ret_t l2sw_getAsicLutAgeTimerSpeed(l2sw_uint8 units, l2sw_uint32* pTimer, l2sw_uint32* pSpeed);
extern ret_t l2sw_setAsicLutCamTbUsage(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicLutCamTbUsage(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_getAsicLutCamType(l2sw_uint8 units, l2sw_uint32* pType);
extern ret_t l2sw_setAsicLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 number);
extern ret_t l2sw_getAsicLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pNumber);
extern ret_t l2sw_setAsicSystemLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 number);
extern ret_t l2sw_getAsicSystemLutLearnLimitNo(l2sw_uint8 units, l2sw_uint32 *pNumber);
extern ret_t l2sw_setAsicLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 action);
extern ret_t l2sw_getAsicLutLearnOverAct(l2sw_uint8 units, l2sw_uint32* pAction);
extern ret_t l2sw_setAsicSystemLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 action);
extern ret_t l2sw_getAsicSystemLutLearnOverAct(l2sw_uint8 units, l2sw_uint32 *pAction);
extern ret_t l2sw_setAsicSystemLutLearnPortMask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicSystemLutLearnPortMask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicL2LookupTb(l2sw_uint8 units, l2sw_luttb *pL2Table);
extern ret_t l2sw_getAsicL2LookupTb(l2sw_uint8 units, l2sw_uint32 method, l2sw_luttb *pL2Table);
extern ret_t l2sw_getAsicLutLearnNo(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pNumber);
extern ret_t l2sw_setAsicLutIpLookupMethod(l2sw_uint8 units, l2sw_uint32 type);
extern ret_t l2sw_getAsicLutIpLookupMethod(l2sw_uint8 units, l2sw_uint32* pType);
extern ret_t l2sw_setAsicLutForceFlush(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicLutForceFlushStatus(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicLutFlushMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicLutFlushMode(l2sw_uint8 units, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicLutFlushType(l2sw_uint8 units, l2sw_uint32 type);
extern ret_t l2sw_getAsicLutFlushType(l2sw_uint8 units, l2sw_uint32* pType);
extern ret_t l2sw_setAsicLutFlushVid(l2sw_uint8 units, l2sw_uint32 vid);
extern ret_t l2sw_getAsicLutFlushVid(l2sw_uint8 units, l2sw_uint32* pVid);
extern ret_t l2sw_setAsicLutFlushFid(l2sw_uint8 units, l2sw_uint32 fid);
extern ret_t l2sw_getAsicLutFlushFid(l2sw_uint8 units, l2sw_uint32* pFid);
extern ret_t l2sw_setAsicLutDisableAging(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 disabled);
extern ret_t l2sw_getAsicLutDisableAging(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pDisabled);
extern ret_t l2sw_setAsicLutIPMCGroup(l2sw_uint8 units, l2sw_uint32 index, ipaddr_t group_addr, l2sw_uint32 vid, l2sw_uint32 pmask, l2sw_uint32 valid);
extern ret_t l2sw_getAsicLutIPMCGroup(l2sw_uint8 units, l2sw_uint32 index, ipaddr_t *pGroup_addr, l2sw_uint32 *pVid, l2sw_uint32 *pPmask, l2sw_uint32 *pValid);
extern ret_t l2sw_setAsicLutLinkDownForceAging(l2sw_uint8 units, l2sw_uint32 enable);
extern ret_t l2sw_getAsicLutLinkDownForceAging(l2sw_uint8 units, l2sw_uint32 *pEnable);
extern ret_t l2sw_setAsicLutFlushAll(l2sw_uint8 units);
extern ret_t l2sw_getAsicLutFlushAllStatus(l2sw_uint8 units, l2sw_uint32 *pBusyStatus);
extern ret_t l2sw_setAsicLutIpmcFwdRouterPort(l2sw_uint8 units, l2sw_uint32 enable);
extern ret_t l2sw_getAsicLutIpmcFwdRouterPort(l2sw_uint8 units, l2sw_uint32 *pEnable);
extern ret_t l2sw_setAsicLutLearnLimitPerSec(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled, l2sw_uint32 number);
extern ret_t l2sw_getAsicLutLearnLimitPerSec(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled, l2sw_uint32 *pNumber);


#endif /*_L2SW_ASICDRV_LUT_H_*/

