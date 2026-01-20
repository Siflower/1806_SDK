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

#ifndef _L2SW_ASICDRV_PORTSECURITY_H_
#define _L2SW_ASICDRV_PORTSECURITY_H_

#include "l2sw_asicdrv_unknownMulticast.h"
#include "l2sw_asicdrv_phy.h"

/****************************************************************/
/* Type Definition                                              */
/****************************************************************/

#define L2SW_MAC7       7
#define L2SW_EXTNO      3

#define L2SW_RTCT_PAGE          (11)
#define L2SW_RTCT_RESULT_A_REG  (27)
#define L2SW_RTCT_RESULT_B_REG  (28)
#define L2SW_RTCT_RESULT_C_REG  (29)
#define L2SW_RTCT_RESULT_D_REG  (30)
#define L2SW_RTCT_STATUS_REG    (26)

enum L2_SECURITY_BEHAVE
{
    L2_BEHAVE_FLOODING = 0,
    L2_BEHAVE_DROP,
    L2_BEHAVE_TRAP,
    L2_BEHAVE_END
};

enum L2_UNDA_BEHAVE
{
    L2_UNDA_BEHAVE_FLOODING_PMASK = 0,
    L2_UNDA_BEHAVE_DROP,
    L2_UNDA_BEHAVE_TRAP,
    L2_UNDA_BEHAVE_FLOODING,
    L2_UNDA_BEHAVE_END
};

enum L2_SECURITY_SA_BEHAVE
{
    L2_BEHAVE_SA_FLOODING = 0,
    L2_BEHAVE_SA_DROP,
    L2_BEHAVE_SA_TRAP,
    L2_BEHAVE_SA_COPY28051,
    L2_BEHAVE_SA_END
};

/* enum for port current link speed */
enum SPEEDMODE
{
    SPD_10M = 0,
    SPD_100M,
    SPD_1000M,
    SPD_2500M
};

/* enum for mac link mode */
enum LINKMODE
{
    MAC_NORMAL = 0,
    MAC_FORCE,
};

/* enum for port current link duplex mode */
enum DUPLEXMODE
{
    HALF_DUPLEX = 0,
    FULL_DUPLEX
};

/* enum for port current MST mode */
enum MSTMODE
{
    SLAVE_MODE= 0,
    MASTER_MODE
};


enum EXTMODE
{
    EXT_DISABLE = 0,
    EXT_RGMII,
    EXT_MII_MAC,
    EXT_MII_PHY,
    EXT_TMII_MAC,
    EXT_TMII_PHY,
    EXT_GMII,
    EXT_RMII_MAC,
    EXT_RMII_PHY,
    EXT_SGMII,
    EXT_HSGMII,
    EXT_1000X_100FX,
    EXT_1000X,
    EXT_100FX,
    EXT_RGMII_2,
    EXT_MII_MAC_2,
    EXT_MII_PHY_2,
    EXT_TMII_MAC_2,
    EXT_TMII_PHY_2,
    EXT_RMII_MAC_2,
    EXT_RMII_PHY_2,
    EXT_END
};

enum DOSTYPE
{
    DOS_DAEQSA = 0,
    DOS_LANDATTACKS,
    DOS_BLATATTACKS,
    DOS_SYNFINSCAN,
    DOS_XMASCAN,
    DOS_NULLSCAN,
    DOS_SYN1024,
    DOS_TCPSHORTHDR,
    DOS_TCPFRAGERROR,
    DOS_ICMPFRAGMENT,
    DOS_SYNRST,
    DOS_END,

};

typedef struct  l2sw_port_ability_s{
    l2sw_uint16 forcemode;
    l2sw_uint16 mstfault;
    l2sw_uint16 mstmode;
    l2sw_uint16 nway;
    l2sw_uint16 txpause;
    l2sw_uint16 rxpause;
    l2sw_uint16 link;
    l2sw_uint16 duplex;
    l2sw_uint16 speed;
}l2sw_port_ability_t;

typedef struct  l2sw_port_status_s{

    l2sw_uint16 lpi1000;
    l2sw_uint16 lpi100;
    l2sw_uint16 mstfault;
    l2sw_uint16 mstmode;
    l2sw_uint16 nway;
    l2sw_uint16 txpause;
    l2sw_uint16 rxpause;
    l2sw_uint16 link;
    l2sw_uint16 duplex;
    l2sw_uint16 speed;

}l2sw_port_status_t;

typedef struct rtct_result_s
{
    l2sw_uint32      channelAShort;
    l2sw_uint32      channelBShort;
    l2sw_uint32      channelCShort;
    l2sw_uint32      channelDShort;

    l2sw_uint32      channelAOpen;
    l2sw_uint32      channelBOpen;
    l2sw_uint32      channelCOpen;
    l2sw_uint32      channelDOpen;

    l2sw_uint32      channelAMismatch;
    l2sw_uint32      channelBMismatch;
    l2sw_uint32      channelCMismatch;
    l2sw_uint32      channelDMismatch;

    l2sw_uint32      channelALinedriver;
    l2sw_uint32      channelBLinedriver;
    l2sw_uint32      channelCLinedriver;
    l2sw_uint32      channelDLinedriver;

    l2sw_uint32      channelALen;
    l2sw_uint32      channelBLen;
    l2sw_uint32      channelCLen;
    l2sw_uint32      channelDLen;
} l2sw_port_rtct_result_t;


/****************************************************************/
/* Driver Proto Type Definition                                 */
/****************************************************************/
extern ret_t l2sw_setAsicPortUnknownDaBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 behavior);
extern ret_t l2sw_getAsicPortUnknownDaBehavior(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBehavior);
extern ret_t l2sw_setAsicPortUnknownSaBehavior(l2sw_uint8 units, l2sw_uint32 behavior);
extern ret_t l2sw_getAsicPortUnknownSaBehavior(l2sw_uint8 units, l2sw_uint32 *pBehavior);
extern ret_t l2sw_setAsicPortUnmatchedSaBehavior(l2sw_uint8 units, l2sw_uint32 behavior);
extern ret_t l2sw_getAsicPortUnmatchedSaBehavior(l2sw_uint8 units, l2sw_uint32 *pBehavior);
extern ret_t l2sw_setAsicPortUnmatchedSaMoving(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enabled);
extern ret_t l2sw_getAsicPortUnmatchedSaMoving(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnabled);
extern ret_t l2sw_setAsicPortUnknownDaFloodingPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicPortUnknownDaFloodingPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicPortUnknownMulticastFloodingPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicPortUnknownMulticastFloodingPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicPortBcastFloodingPortmask(l2sw_uint8 units, l2sw_uint32 portmask);
extern ret_t l2sw_getAsicPortBcastFloodingPortmask(l2sw_uint8 units, l2sw_uint32 *pPortmask);
extern ret_t l2sw_setAsicPortBlockSpa(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 block);
extern ret_t l2sw_getAsicPortBlockSpa(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pBlock);
extern ret_t l2sw_setAsicPortForceLink(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_ability_t *pPortAbility);
extern ret_t l2sw_getAsicPortForceLink(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_ability_t *pPortAbility);
extern ret_t l2sw_getAsicPortStatus(l2sw_uint8 units, l2sw_uint32 port, l2sw_port_status_t *pPortStatus);
extern ret_t l2sw_setAsicPortForceLinkExt(l2sw_uint8 units, l2sw_uint32 id, l2sw_port_ability_t *pPortAbility);
extern ret_t l2sw_getAsicPortForceLinkExt(l2sw_uint8 units, l2sw_uint32 id, l2sw_port_ability_t *pPortAbility);
extern ret_t l2sw_setAsicPortExtMode(l2sw_uint8 units, l2sw_uint32 id, l2sw_uint32 mode);
extern ret_t l2sw_getAsicPortExtMode(l2sw_uint8 units, l2sw_uint32 id, l2sw_uint32 *pMode);
extern ret_t l2sw_setAsicPortDos(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32 drop);
extern ret_t l2sw_getAsicPortDos(l2sw_uint8 units, l2sw_uint32 type, l2sw_uint32* pDrop);
extern ret_t l2sw_setAsicPortEnableAll(l2sw_uint8 units, l2sw_uint32 enable);
extern ret_t l2sw_getAsicPortEnableAll(l2sw_uint8 units, l2sw_uint32 *pEnable);
extern ret_t l2sw_setAsicPortLoopback(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 enable);
extern ret_t l2sw_getAsicPortLoopback(l2sw_uint8 units, l2sw_uint32 port, l2sw_uint32 *pEnable);

#endif /*_L2SW_ASICDRV_PORTSECURITY_H_*/

