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

#ifndef _L2SW_ASICDRV_VLAN_H_
#define _L2SW_ASICDRV_VLAN_H_

/****************************************************************/
/* Header File inclusion                                        */
/****************************************************************/
#include "l2sw_asicdrv.h"

/****************************************************************/
/* Constant Definition                                          */
/****************************************************************/
#define L2SW_PROTOVLAN_GIDX_MAX 3
#define L2SW_PROTOVLAN_GROUPNO  4

#define L2SW_VLAN_BUSY_CHECK_NO     (10)

#define L2SW_VLAN_MBRCFG_LEN    (4)
#define L2SW_VLAN_4KTABLE_LEN   (3)

/****************************************************************/
/* Type Definition                                              */
/****************************************************************/
typedef struct  VLANCONFIGUSER
{
    l2sw_uint16  evid;
    l2sw_uint16  mbr;
    l2sw_uint16  fid_msti;
    l2sw_uint16  envlanpol;
    l2sw_uint16  meteridx;
    l2sw_uint16  vbpen;
    l2sw_uint16  vbpri;
}l2sw_vlanconfiguser;

typedef struct  USER_VLANTABLE{

    l2sw_uint16  vid;
    l2sw_uint16  mbr;
    l2sw_uint16  untag;
    l2sw_uint16  fid_msti;
    l2sw_uint16  envlanpol;
    l2sw_uint16  meteridx;
    l2sw_uint16  vbpen;
    l2sw_uint16  vbpri;
    l2sw_uint16  ivl_svl;

}l2sw_user_vlan4kentry;

typedef enum
{
    FRAME_TYPE_BOTH = 0,
    FRAME_TYPE_TAGGED_ONLY,
    FRAME_TYPE_UNTAGGED_ONLY,
    FRAME_TYPE_MAX_BOUND
} l2sw_accframetype;

typedef enum
{
    EG_TAG_MODE_ORI = 0,
    EG_TAG_MODE_KEEP,
    EG_TAG_MODE_PRI_TAG,
    EG_TAG_MODE_REAL_KEEP,
    EG_TAG_MODE_END
} l2sw_egtagmode;

typedef enum
{
    PPVLAN_FRAME_TYPE_ETHERNET = 0,
    PPVLAN_FRAME_TYPE_LLC,
    PPVLAN_FRAME_TYPE_RFC1042,
    PPVLAN_FRAME_TYPE_END
} l2sw_provlan_frametype;

enum L2SW_STPST
{
    STPST_DISABLED = 0,
    STPST_BLOCKING,
    STPST_LEARNING,
    STPST_FORWARDING
};

enum L2SW_RESVIDACT
{
    RES_VID_ACT_UNTAG = 0,
    RES_VID_ACT_TAG,
    RES_VID_ACT_END
};

typedef struct
{
    l2sw_provlan_frametype  frameType;
    l2sw_uint32                      etherType;
} l2sw_protocolgdatacfg;

typedef struct
{
    l2sw_uint32 valid;
    l2sw_uint32 vlan_idx;
    l2sw_uint32 priority;
} l2sw_protocolvlancfg;

extern ret_t l2sw_setAsicVlanMemberConfig(l2sw_uint8 units, l2sw_uint32 index, l2sw_vlanconfiguser *pVlanCg);
extern ret_t l2sw_getAsicVlanMemberConfig(l2sw_uint8 units, l2sw_uint32 index, l2sw_vlanconfiguser *pVlanCg);
extern ret_t l2sw_setAsicVlan4kEntry(l2sw_uint8 units, l2sw_user_vlan4kentry *pVlan4kEntry );
extern ret_t l2sw_getAsicVlan4kEntry(l2sw_uint8 units, l2sw_user_vlan4kentry *pVlan4kEntry );
extern ret_t l2sw_setAsicVlanAccpetFrameType(l2sw_uint8 units, l2sw_uint32 port, l2sw_accframetype frameType);
extern ret_t l2sw_getAsicVlanAccpetFrameType(l2sw_uint8 units, l2sw_uint32 port, l2sw_accframetype *pFrameType);
extern ret_t l2sw_setAsicVlanIngressFilter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicVlanIngressFilter(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable);
extern ret_t l2sw_setAsicVlanEgressTagMode(l2sw_uint8 units, l2sw_uint32 port, l2sw_egtagmode tagMode);
extern ret_t l2sw_getAsicVlanEgressTagMode(l2sw_uint8 units, l2sw_uint32 port, l2sw_egtagmode *pTagMode);
extern ret_t l2sw_setAsicVlanPortBasedVID(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_uint32 pri);
extern ret_t l2sw_getAsicVlanPortBasedVID(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pIndex, l2sw_uint32 *pPri);
extern ret_t l2sw_setAsicVlanProtocolBasedGroupData(l2sw_uint8 units, l2sw_uint32 index, l2sw_protocolgdatacfg *pPbCfg);
extern ret_t l2sw_getAsicVlanProtocolBasedGroupData(l2sw_uint8 units, l2sw_uint32 index, l2sw_protocolgdatacfg *pPbCfg);
extern ret_t l2sw_setAsicVlanPortAndProtocolBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_protocolvlancfg *pPpbCfg);
extern ret_t l2sw_getAsicVlanPortAndProtocolBased(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index, l2sw_protocolvlancfg *pPpbCfg);
extern ret_t l2sw_setAsicVlanFilter(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicVlanFilter(l2sw_uint8 units, l2sw_uint32* pEnabled);

extern ret_t l2sw_setAsicPortBasedFid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 fid);
extern ret_t l2sw_getAsicPortBasedFid(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pFid);
extern ret_t l2sw_setAsicPortBasedFidEn(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortBasedFidEn(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicSpanningTreeStatus(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 msti, l2sw_uint32 state);
extern ret_t l2sw_getAsicSpanningTreeStatus(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 msti, l2sw_uint32* pState);
extern ret_t l2sw_setAsicVlanUntagDscpPriorityEn(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicVlanUntagDscpPriorityEn(l2sw_uint8 units, l2sw_uint32* enabled);
extern ret_t l2sw_setAsicVlanTransparent(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicVlanTransparent(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicVlanEgressKeep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicVlanEgressKeep(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pPortmask);
extern ret_t l2sw_setReservedVidAction(l2sw_uint8 units, l2sw_uint32 vid0Action, l2sw_uint32 vid4095Action);
extern ret_t l2sw_getReservedVidAction(l2sw_uint8 units, l2sw_uint32 *pVid0Action, l2sw_uint32 *pVid4095Action);
extern ret_t l2sw_setRealKeepRemarkEn(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getRealKeepRemarkEn(l2sw_uint8 units, l2sw_uint32 *pEnabled);
extern ret_t l2sw_resetVlan(l2sw_uint8 units);
extern ret_t l2sw_getresetVlan(l2sw_uint8 units, l2sw_uint32 *pEnabled);

#endif /*#ifndef _L2SW_ASICDRV_VLAN_H_*/

