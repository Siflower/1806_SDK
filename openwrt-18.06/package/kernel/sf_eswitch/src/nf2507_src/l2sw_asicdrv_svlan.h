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

#ifndef _L2SW_ASICDRV_SVLAN_H_
#define _L2SW_ASICDRV_SVLAN_H_

#include "l2sw_asicdrv.h"

#define L2SW_C2SIDXNO               128
#define L2SW_C2SIDXMAX              (L2SW_C2SIDXNO-1)
#define L2SW_MC2SIDXNO              32
#define L2SW_MC2SIDXMAX             (L2SW_MC2SIDXNO-1)
#define L2SW_SP2CIDXNO              128
#define L2SW_SP2CMAX                (L2SW_SP2CIDXNO-1)

#define L2SW_SVLAN_MEMCOL2SW_LEN      4
#define L2SW_SVLAN_MC2S_LEN         5
#define L2SW_SVLAN_SP2C_LEN         2

enum L2SW_SPRISEL
{
    SPRISEL_INTERNALPRI =  0,
    SPRISEL_CTAGPRI,
    SPRISEL_VSPRI,
    SPRISEL_PBPRI,
    SPRISEL_END
};

enum L2SW_SUNACCEPT
{
    SUNACCEPT_DROP =  0,
    SUNACCEPT_TRAP,
    SUNACCEPT_SVLAN,
    SUNACCEPT_END
};

enum L2SW_SVLAN_MC2S_MODE
{
    SVLAN_MC2S_MODE_MAC =  0,
    SVLAN_MC2S_MODE_IP,
    SVLAN_MC2S_MODE_END
};

typedef struct  l2sw_svlan_memconf_s{

    l2sw_uint16 vs_member:11;
    l2sw_uint16 vs_untag:11;

    l2sw_uint16 vs_fid_msti:4;
    l2sw_uint16 vs_priority:3;
    l2sw_uint16 vs_force_fid:1;
    l2sw_uint16 reserved:8;

    l2sw_uint16 vs_svid:12;
    l2sw_uint16 vs_efiden:1;
    l2sw_uint16 vs_efid:3;


}l2sw_svlan_memconf_t;


typedef struct  l2sw_svlan_mc2s_s{

    l2sw_uint16 valid:1;
    l2sw_uint16 format:1;
    l2sw_uint16 svidx:6;
    l2sw_uint32 sdata;
    l2sw_uint32 smask;
}l2sw_svlan_mc2s_t;


typedef struct  l2sw_svlan_s2c_s{

    l2sw_uint16 valid:1;
    l2sw_uint16 svidx:6;
    l2sw_uint16 dstport:4;
    l2sw_uint32 vid:12;
}l2sw_svlan_s2c_t;

extern ret_t l2sw_setAsicSvlanIngressUntag(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicSvlanIngressUntag(l2sw_uint8 units, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicSvlanIngressUnmatch(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicSvlanIngressUnmatch(l2sw_uint8 units, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicSvlanTrapPriority(l2sw_uint8 units, l2sw_uint32 priority);
extern ret_t l2sw_getAsicSvlanTrapPriority(l2sw_uint8 units, l2sw_uint32* pPriority);
extern ret_t l2sw_setAsicSvlanDefaultVlan(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 index);
extern ret_t l2sw_getAsicSvlanDefaultVlan(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pIndex);

extern ret_t l2sw_setAsicSvlanMemberConfiguration(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_memconf_t* pSvlanMemCfg);
extern ret_t l2sw_getAsicSvlanMemberConfiguration(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_memconf_t* pSvlanMemCfg);

extern ret_t l2sw_setAsicSvlanPrioritySel(l2sw_uint8 units, l2sw_uint32 priSel);
extern ret_t l2sw_getAsicSvlanPrioritySel(l2sw_uint8 units, l2sw_uint32* pPriSel);
extern ret_t l2sw_setAsicSvlanTpid(l2sw_uint8 units, l2sw_uint32 protocolType);
extern ret_t l2sw_getAsicSvlanTpid(l2sw_uint8 units, l2sw_uint32* pProtocolType);
extern ret_t l2sw_setAsicSvlanUplinkPortMask(l2sw_uint8 units, l2sw_uint32 portMask);
extern ret_t l2sw_getAsicSvlanUplinkPortMask(l2sw_uint8 units, l2sw_uint32* pPortmask);
extern ret_t l2sw_setAsicSvlanEgressUnassign(l2sw_uint8 units, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicSvlanEgressUnassign(l2sw_uint8 units, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicSvlanC2SConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 evid, l2sw_uint32 portmask, l2sw_uint32 svidx);
extern ret_t l2sw_getAsicSvlanC2SConf(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pEvid, l2sw_uint32* pPortmask, l2sw_uint32* pSvidx);
extern ret_t l2sw_setAsicSvlanMC2SConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_mc2s_t* pSvlanMc2sCfg);
extern ret_t l2sw_getAsicSvlanMC2SConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_mc2s_t* pSvlanMc2sCfg);
extern ret_t l2sw_setAsicSvlanSP2CConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_s2c_t* pSvlanSp2cCfg);
extern ret_t l2sw_getAsicSvlanSP2CConf(l2sw_uint8 units, l2sw_uint32 index,l2sw_svlan_s2c_t* pSvlanSp2cCfg);
extern ret_t l2sw_setAsicSvlanDmacCvidSel(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicSvlanDmacCvidSel(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32* pEnabled);
extern ret_t l2sw_setAsicSvlanUntagVlan(l2sw_uint8 units, l2sw_uint32 index);
extern ret_t l2sw_getAsicSvlanUntagVlan(l2sw_uint8 units, l2sw_uint32* pIndex);
extern ret_t l2sw_setAsicSvlanUnmatchVlan(l2sw_uint8 units, l2sw_uint32 index);
extern ret_t l2sw_getAsicSvlanUnmatchVlan(l2sw_uint8 units, l2sw_uint32* pIndex);
extern ret_t l2sw_setAsicSvlanLookupType(l2sw_uint8 units, l2sw_uint32 type);
extern ret_t l2sw_getAsicSvlanLookupType(l2sw_uint8 units, l2sw_uint32* pType);
extern ret_t l2sw_svlan_s2c_add_cpri_set(l2sw_uint8 units, l2sw_uint32 cpri_type);
extern ret_t l2sw_svlan_s2c_add_cpri_get(l2sw_uint8 units, l2sw_uint32* pcpri_type);


#endif /*#ifndef _L2SW_ASICDRV_SVLAN_H_*/

