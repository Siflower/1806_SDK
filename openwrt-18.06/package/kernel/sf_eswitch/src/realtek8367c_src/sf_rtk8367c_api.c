/*
* Description
*
* Copyright (C) 2016-2021 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include <linux/skbuff.h>
#include "smi.h"
#include "led.h"
#include "stat.h"
#include "vlan.h"
#include "port.h"
#include "rtk_types.h"
#include "rtk_error.h"
#include "rtk_switch.h"
#include "rtl8367c_asicdrv.h"
#include "rtl8367c_asicdrv_phy.h"
#include "rtl8367c_asicdrv_mib.h"
#include "rtl8367c_asicdrv_vlan.h"
#include "rtl8367c_asicdrv_port.h"
#include "rtl8367c_asicdrv_led.h"
#include "rtl8367c_asicdrv_rma.h"
#include "rtl8367c_asicdrv_lut.h"
#include "l2.h"

extern int mdio_read_ext(int phyaddr, int phyreg, int *phydata);
extern int mdio_write_ext(int phyaddr, int phyreg, int phydata);

extern int rtk_phy_id;

#define MDC_MDIO_WRITE(preamableLength, phyID, regID, data) mdio_write_ext(phyID, regID, data)
#define MDC_MDIO_READ(preamableLength, phyID, regID, pData) mdio_read_ext(phyID, regID, pData)
#define RTK_MAX_METER_ID            (rtk_switch_maxMeterId_get())
#define MIB_NOT_SUPPORT     (0xFFFF)

typedef enum vlan_mbrCfgType_e
{
    MBRCFG_UNUSED = 0,
    MBRCFG_USED_BY_VLAN,
    MBRCFG_END
}vlan_mbrCfgType_t;

static init_state_t    init_state = INIT_COMPLETED;
static rtk_switch_halCtrl_t rtl8367c_hal_Ctrl =
{
    /* Switch Chip */
    CHIP_RTL8367C,

    /* Logical to Physical */
    {0, 1, 2, 3, 4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
     6, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },

    /* Physical to Logical */
    {UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, UNDEFINE_PORT, EXT_PORT0, EXT_PORT1,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT,
     UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT, UNDEFINE_PORT},

    /* Port Type */
    {UTP_PORT, UTP_PORT, UTP_PORT, UTP_PORT, UTP_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     EXT_PORT, EXT_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT,
     UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT, UNKNOWN_PORT},

    /* PTP port */
    {1, 1, 1, 1, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0 },

    /* Valid port mask */
    ( (0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid UTP port mask */
    ( (0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) ),

    /* Valid EXT port mask */
    ( (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid CPU port mask */
    0x00,

    /* Minimum physical port number */
    0,

    /* Maxmum physical port number */
    7,

    /* Physical port mask */
    0xDF,

    /* Combo Logical port ID */
    4,

    /* HSG Logical port ID */
    EXT_PORT0,

    /* SGMII Logical portmask */
    (0x1 << EXT_PORT0),

    /* Max Meter ID */
    31,

    /* MAX LUT Address Number */
    2112,

    /* Trunk Group Mask */
    0x03
};

static rtk_switch_halCtrl_t *halCtrl = &rtl8367c_hal_Ctrl;
static rtk_vlan_t           vlan_mbrCfgVid[RTL8367C_CVIDXNO];
static vlan_mbrCfgType_t    vlan_mbrCfgUsage[RTL8367C_CVIDXNO];

init_state_t rtk_switch_initialState_get(void)
{
    return init_state;
}

rtk_uint32 rtk_switch_port_L2P_get(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return UNDEFINE_PHY_PORT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return UNDEFINE_PHY_PORT;

    return (halCtrl->l2p_port[logicalPort]);
}

rtk_uint32 rtk_switch_phyPortMask_get(void)
{
    if(init_state != INIT_COMPLETED)
        return 0x00; /* No port in portmask */

    return (halCtrl->phy_portmask);
}

rtk_api_ret_t rtk_switch_logicalPortCheck(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->l2p_port[logicalPort] == 0xFF)
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

rtk_uint32 rtk_switch_maxMeterId_get(void)
{
    if(init_state != INIT_COMPLETED)
        return 0x00;

    return (halCtrl->max_meter_id);
}

rtk_api_ret_t rtk_switch_isUtpPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->log_port_type[logicalPort] == UTP_PORT)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

rtk_api_ret_t rtk_switch_isExtPort(rtk_port_t logicalPort)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(logicalPort >= RTK_SWITCH_PORT_NUM)
        return RT_ERR_FAILED;

    if(halCtrl->log_port_type[logicalPort] == EXT_PORT)
        return RT_ERR_OK;
    else
        return RT_ERR_FAILED;
}

/*
 * =================================================================
 *  MDIO API
 * =================================================================
 * */
rtk_int32 smi_read(rtk_uint32 mAddrs, rtk_uint32 *rData)
{
    if(mAddrs > 0xFFFF)
        return RT_ERR_INPUT;

    if(rData == NULL)
        return RT_ERR_NULL_POINTER;

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

    /* Write address to register 23 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_ADDRESS_REG, mAddrs);

    /* Write read control code to register 21 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_CTRL1_REG, MDC_MDIO_READ_OP);

    /* Read data from register 25 */
    MDC_MDIO_READ(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_DATA_READ_REG, rData);

    return RT_ERR_OK;
}

rtk_int32 smi_write(rtk_uint32 mAddrs, rtk_uint32 rData)
{
    if(mAddrs > 0xFFFF)
        return RT_ERR_INPUT;

    if(rData > 0xFFFF)
        return RT_ERR_INPUT;

    /* Write address control code to register 31 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_CTRL0_REG, MDC_MDIO_ADDR_OP);

    /* Write address to register 23 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_ADDRESS_REG, mAddrs);

    /* Write data to register 24 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_DATA_WRITE_REG, rData);

    /* Write data control code to register 21 */
    MDC_MDIO_WRITE(MDC_MDIO_PREAMBLE_LEN, rtk_phy_id, MDC_MDIO_CTRL1_REG, MDC_MDIO_WRITE_OP);

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicReg(rtk_uint32 reg, rtk_uint32 *pValue)
{
    rtk_uint32 regData;
    ret_t retVal;

    retVal = smi_read(reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    *pValue = regData;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicReg(rtk_uint32 reg, rtk_uint32 value)
{
    ret_t retVal;

    retVal = smi_write(reg, value);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicRegBit(rtk_uint32 reg, rtk_uint32 bit, rtk_uint32 *pValue)
{
    rtk_uint32 regData;
    ret_t retVal;

    retVal = smi_read(reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    *pValue = (regData & (0x1 << bit)) >> bit;

    return RT_ERR_OK;
}


ret_t rtl8367c_setAsicRegBit(rtk_uint32 reg, rtk_uint32 bit, rtk_uint32 value)
{
    rtk_uint32 regData;
    ret_t retVal;

    if(bit >= RTL8367C_REGBITLENGTH)
        return RT_ERR_INPUT;

    retVal = smi_read(reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    if(value)
        regData = regData | (1 << bit);
    else
        regData = regData & (~(1 << bit));

    retVal = smi_write(reg, regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicRegBits(rtk_uint32 reg, rtk_uint32 bits, rtk_uint32 *pValue)
{
    rtk_uint32 regData;
    ret_t retVal;
    rtk_uint32 bitsShift;

    if(bits>= (1<<RTL8367C_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367C_REGBITLENGTH)
            return RT_ERR_INPUT;
    }

    retVal = smi_read(reg, &regData);
    if(retVal != RT_ERR_OK) return RT_ERR_SMI;

    *pValue = (regData & bits) >> bitsShift;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicRegBits(rtk_uint32 reg, rtk_uint32 bits, rtk_uint32 value)
{
    rtk_uint32 regData;
    ret_t retVal;
    rtk_uint32 bitsShift;
    rtk_uint32 valueShifted;

    if(bits >= (1 << RTL8367C_REGBITLENGTH) )
        return RT_ERR_INPUT;

    bitsShift = 0;
    while(!(bits & (1 << bitsShift)))
    {
        bitsShift++;
        if(bitsShift >= RTL8367C_REGBITLENGTH)
            return RT_ERR_INPUT;
    }
    valueShifted = value << bitsShift;

    if(valueShifted > RTL8367C_REGDATAMAX)
        return RT_ERR_INPUT;

    retVal = smi_read(reg, &regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    regData = regData & (~bits);
    regData = regData | (valueShifted & bits);

    retVal = smi_write(reg, regData);
    if(retVal != RT_ERR_OK)
        return RT_ERR_SMI;

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicPHYOCPReg(rtk_uint32 phyNo, rtk_uint32 ocpAddr, rtk_uint32 *pRegData )
{
    ret_t retVal;
    rtk_uint32 regData;
    rtk_uint32 busyFlag,checkCounter;
    rtk_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;
    /*Check internal phy access busy or not*/
    /*retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_INDRECT_ACCESS_STATUS, RTL8367C_INDRECT_ACCESS_STATUS_OFFSET,&busyFlag);*/
    retVal = rtl8367c_getAsicReg(RTL8367C_REG_INDRECT_ACCESS_STATUS,&busyFlag);
    if(retVal != RT_ERR_OK)
        return retVal;

    if(busyFlag)
        return RT_ERR_BUSYWAIT_TIMEOUT;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_GPHY_OCP_MSB_0, RTL8367C_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != RT_ERR_OK)
        return retVal;

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
    regData = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
    retVal = rtl8367c_setAsicReg(RTL8367C_REG_INDRECT_ACCESS_ADDRESS, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    /*Set READ Command*/
    retVal = rtl8367c_setAsicReg(RTL8367C_REG_INDRECT_ACCESS_CTRL, RTL8367C_CMD_MASK );
    if(retVal != RT_ERR_OK)
        return retVal;

    checkCounter = 100;
    while(checkCounter)
    {
        retVal = rtl8367c_getAsicReg(RTL8367C_REG_INDRECT_ACCESS_STATUS,&busyFlag);
        if((retVal != RT_ERR_OK) || busyFlag)
        {
            checkCounter --;
            if(0 == checkCounter)
                return RT_ERR_FAILED;
        }
        else
        {
            checkCounter = 0;
        }
    }

    /*get PHY register*/
    retVal = rtl8367c_getAsicReg(RTL8367C_REG_INDRECT_ACCESS_READ_DATA, &regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    *pRegData = regData;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicPHYOCPReg(rtk_uint32 phyNo, rtk_uint32 ocpAddr, rtk_uint32 ocpData )
{
    ret_t retVal;
    rtk_uint32 regAddr;
    rtk_uint32 ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

    /* OCP prefix */
    ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_GPHY_OCP_MSB_0, RTL8367C_CFG_CPU_OCPADR_MSB_MASK, ocpAddrPrefix)) != RT_ERR_OK)
        return retVal;

    /*prepare access address*/
    ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
    ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
    regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) | (phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
    if((retVal = rtl8367c_setAsicReg(regAddr, ocpData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicPHYReg(rtk_uint32 phyNo, rtk_uint32 phyAddr, rtk_uint32 *pRegData)
{
    rtk_uint32 ocp_addr;

    if(phyAddr > RTL8367C_PHY_REGNOMAX)
        return RT_ERR_PHY_REG_ID;

    ocp_addr = 0xa400 + phyAddr*2;

    return rtl8367c_getAsicPHYOCPReg(phyNo, ocp_addr, pRegData);
}

ret_t rtl8367c_setAsicPHYReg(rtk_uint32 phyNo, rtk_uint32 phyAddr, rtk_uint32 phyData )
{
    rtk_uint32 ocp_addr;

    if(phyAddr > RTL8367C_PHY_REGNOMAX)
        return RT_ERR_PHY_REG_ID;

    ocp_addr = 0xa400 + phyAddr*2;

    return rtl8367c_setAsicPHYOCPReg(phyNo, ocp_addr, phyData);
}

ret_t rtl8367c_getAsicPortBasedFidEn(rtk_uint32 port, rtk_uint32* pEnabled)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    return rtl8367c_getAsicRegBit(RTL8367C_REG_PORT_PBFIDEN,port, pEnabled);
}

ret_t rtl8367c_setAsicPortBasedFidEn(rtk_uint32 port, rtk_uint32 enabled)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_REG_PORT_PBFIDEN,port, enabled);
}

ret_t rtl8367c_getAsicPortBasedFid(rtk_uint32 port, rtk_uint32* pFid)
{
    rtk_uint32  reg_addr;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(port < 8)
        return rtl8367c_getAsicReg(RTL8367C_PORT_PBFID_REG(port), pFid);
    else{
        reg_addr = RTL8367C_REG_PORT8_PBFID + port-8;
        return rtl8367c_getAsicReg(reg_addr, pFid);
    }
}

ret_t rtl8367c_setAsicPortBasedFid(rtk_uint32 port, rtk_uint32 fid)
{
    rtk_uint32  reg_addr;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(fid > RTL8367C_FIDMAX)
        return RT_ERR_L2_FID;

    if(port < 8)
        return rtl8367c_setAsicReg(RTL8367C_PORT_PBFID_REG(port),fid);
    else {
        reg_addr = RTL8367C_REG_PORT8_PBFID + port-8;
        return rtl8367c_setAsicReg(reg_addr, fid);
    }
}

rtk_api_ret_t rtk_vlan_portFid_get(rtk_port_t port, rtk_enable_t *pEnable, rtk_fid_t *pFid)
{
    rtk_api_ret_t retVal;

	/* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if(NULL == pEnable)
        return RT_ERR_NULL_POINTER;

    if(NULL == pFid)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicPortBasedFidEn(rtk_switch_port_L2P_get(port), pEnable))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortBasedFid(rtk_switch_port_L2P_get(port), pFid))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_portFid_set(rtk_port_t port, rtk_enable_t enable, rtk_fid_t fid)
{
    rtk_api_ret_t retVal;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if (enable>=RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    /* fid must be 0~4095 */
    if (fid > RTK_FID_MAX)
        return RT_ERR_L2_FID;

    if ((retVal = rtl8367c_setAsicPortBasedFidEn(rtk_switch_port_L2P_get(port), enable))!=RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPortBasedFid(rtk_switch_port_L2P_get(port), fid))!=RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

static void _rtl8367c_Vlan4kStUser2Smi(rtl8367c_user_vlan4kentry *pUserVlan4kEntry, rtk_uint16 *pSmiVlan4kEntry)
{
    pSmiVlan4kEntry[0] |= (pUserVlan4kEntry->mbr & 0x00FF);
    pSmiVlan4kEntry[0] |= (pUserVlan4kEntry->untag & 0x00FF) << 8;

    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->fid_msti & 0x000F);
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->vbpen & 0x0001) << 4;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->vbpri & 0x0007) << 5;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->envlanpol & 0x0001) << 8;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->meteridx & 0x001F) << 9;
    pSmiVlan4kEntry[1] |= (pUserVlan4kEntry->ivl_svl & 0x0001) << 14;

    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->mbr & 0x0700) >> 8);
    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->untag & 0x0700) >> 8) << 3;
    pSmiVlan4kEntry[2] |= ((pUserVlan4kEntry->meteridx & 0x0020) >> 5) << 6;
}

static void _rtl8367c_Vlan4kStSmi2User(rtk_uint16 *pSmiVlan4kEntry, rtl8367c_user_vlan4kentry *pUserVlan4kEntry)
{
    pUserVlan4kEntry->mbr = (pSmiVlan4kEntry[0] & 0x00FF) | ((pSmiVlan4kEntry[2] & 0x0007) << 8);
    pUserVlan4kEntry->untag = ((pSmiVlan4kEntry[0] & 0xFF00) >> 8) | (((pSmiVlan4kEntry[2] & 0x0038) >> 3) << 8);
    pUserVlan4kEntry->fid_msti = pSmiVlan4kEntry[1] & 0x000F;
    pUserVlan4kEntry->vbpen = (pSmiVlan4kEntry[1] & 0x0010) >> 4;
    pUserVlan4kEntry->vbpri = (pSmiVlan4kEntry[1] & 0x00E0) >> 5;
    pUserVlan4kEntry->envlanpol = (pSmiVlan4kEntry[1] & 0x0100) >> 8;
    pUserVlan4kEntry->meteridx = ((pSmiVlan4kEntry[1] & 0x3E00) >> 9) | (((pSmiVlan4kEntry[2] & 0x0040) >> 6) << 5);
    pUserVlan4kEntry->ivl_svl = (pSmiVlan4kEntry[1] & 0x4000) >> 14;
}

ret_t rtl8367c_getAsicVlan4kEntry(rtl8367c_user_vlan4kentry *pVlan4kEntry )
{
    rtk_uint16                  vlan_4k_entry[RTL8367C_VLAN_4KTABLE_LEN];
    rtk_uint32                  page_idx;
    rtk_uint16                  *tableAddr;
    ret_t                       retVal;
    rtk_uint32                  regData;
    rtk_uint32                  busyCounter;

    if(pVlan4kEntry->vid > RTL8367C_VIDMAX)
        return RT_ERR_VLAN_VID;

    /* Polling status */
    busyCounter = RTL8367C_VLAN_BUSY_CHECK_NO;
    while(busyCounter)
    {
        retVal = rtl8367c_getAsicRegBit(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        if(regData == 0)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return RT_ERR_BUSYWAIT_TIMEOUT;
    }

    /* Write Address (VLAN_ID) */
    regData = pVlan4kEntry->vid;
    retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_ADDR_REG, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    /* Read Command */
    retVal = rtl8367c_setAsicRegBits(RTL8367C_TABLE_ACCESS_CTRL_REG, RTL8367C_TABLE_TYPE_MASK | RTL8367C_COMMAND_TYPE_MASK, RTL8367C_TABLE_ACCESS_REG_DATA(TB_OP_READ,TB_TARGET_CVLAN));
    if(retVal != RT_ERR_OK)
        return retVal;

    /* Polling status */
    busyCounter = RTL8367C_VLAN_BUSY_CHECK_NO;
    while(busyCounter)
    {
        retVal = rtl8367c_getAsicRegBit(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,&regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        if(regData == 0)
            break;

        busyCounter --;
        if(busyCounter == 0)
            return RT_ERR_BUSYWAIT_TIMEOUT;
    }

    /* Read VLAN data from register */
    tableAddr = vlan_4k_entry;
    for(page_idx = 0; page_idx < RTL8367C_VLAN_4KTABLE_LEN; page_idx++)
    {
        retVal = rtl8367c_getAsicReg(RTL8367C_TABLE_ACCESS_RDDATA_BASE + page_idx, &regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        *tableAddr = regData;
        tableAddr++;
    }

    _rtl8367c_Vlan4kStSmi2User(vlan_4k_entry, pVlan4kEntry);

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicVlan4kEntry(rtl8367c_user_vlan4kentry *pVlan4kEntry )
{
    rtk_uint16              vlan_4k_entry[RTL8367C_VLAN_4KTABLE_LEN];
    rtk_uint32                  page_idx;
    rtk_uint16                  *tableAddr;
    ret_t                   retVal;
    rtk_uint32                  regData;

    if(pVlan4kEntry->vid > RTL8367C_VIDMAX)
        return RT_ERR_VLAN_VID;

    if(pVlan4kEntry->mbr > RTL8367C_PORTMASK)
        return RT_ERR_PORT_MASK;

    if(pVlan4kEntry->untag > RTL8367C_PORTMASK)
        return RT_ERR_PORT_MASK;

    if(pVlan4kEntry->fid_msti > RTL8367C_FIDMAX)
        return RT_ERR_L2_FID;

    if(pVlan4kEntry->meteridx > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    if(pVlan4kEntry->vbpri > RTL8367C_PRIMAX)
        return RT_ERR_QOS_INT_PRIORITY;

    memset(vlan_4k_entry, 0x00, sizeof(rtk_uint16) * RTL8367C_VLAN_4KTABLE_LEN);
    _rtl8367c_Vlan4kStUser2Smi(pVlan4kEntry, vlan_4k_entry);

    /* Prepare Data */
    tableAddr = vlan_4k_entry;
    for(page_idx = 0; page_idx < RTL8367C_VLAN_4KTABLE_LEN; page_idx++)
    {
        regData = *tableAddr;
        retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_WRDATA_BASE + page_idx, regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        tableAddr++;
    }

    /* Write Address (VLAN_ID) */
    regData = pVlan4kEntry->vid;
    retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_ADDR_REG, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    /* Write Command */
    retVal = rtl8367c_setAsicRegBits(RTL8367C_TABLE_ACCESS_CTRL_REG, RTL8367C_TABLE_TYPE_MASK | RTL8367C_COMMAND_TYPE_MASK,RTL8367C_TABLE_ACCESS_REG_DATA(TB_OP_WRITE,TB_TARGET_CVLAN));
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

static void _rtl8367c_VlanMCStSmi2User(rtk_uint16 *pSmiVlanCfg, rtl8367c_vlanconfiguser *pVlanCg)
{
    pVlanCg->mbr            = pSmiVlanCfg[0] & 0x07FF;
    pVlanCg->fid_msti       = pSmiVlanCfg[1] & 0x000F;
    pVlanCg->meteridx       = (pSmiVlanCfg[2] >> 5) & 0x003F;
    pVlanCg->envlanpol      = (pSmiVlanCfg[2] >> 4) & 0x0001;
    pVlanCg->vbpri          = (pSmiVlanCfg[2] >> 1) & 0x0007;
    pVlanCg->vbpen          = pSmiVlanCfg[2] & 0x0001;
    pVlanCg->evid           = pSmiVlanCfg[3] & 0x1FFF;
}

static void _rtl8367c_VlanMCStUser2Smi(rtl8367c_vlanconfiguser *pVlanCg, rtk_uint16 *pSmiVlanCfg)
{
    pSmiVlanCfg[0] |= pVlanCg->mbr & 0x07FF;

    pSmiVlanCfg[1] |= pVlanCg->fid_msti & 0x000F;

    pSmiVlanCfg[2] |= pVlanCg->vbpen & 0x0001;
    pSmiVlanCfg[2] |= (pVlanCg->vbpri & 0x0007) << 1;
    pSmiVlanCfg[2] |= (pVlanCg->envlanpol & 0x0001) << 4;
    pSmiVlanCfg[2] |= (pVlanCg->meteridx & 0x003F) << 5;

    pSmiVlanCfg[3] |= pVlanCg->evid & 0x1FFF;
}

rtk_port_t rtk_switch_port_P2L_get(rtk_uint32 physicalPort)
{
    if(init_state != INIT_COMPLETED)
        return UNDEFINE_PORT;

    if(physicalPort >= RTK_SWITCH_PORT_NUM)
        return UNDEFINE_PORT;

    return (halCtrl->p2l_port[physicalPort]);
}

rtk_api_ret_t rtk_switch_portmask_P2L_get(rtk_uint32 physicalPortmask, rtk_portmask_t *pLogicalPmask)
{
    rtk_uint32 log_port, phy_port;

    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(NULL == pLogicalPmask)
        return RT_ERR_NULL_POINTER;

    RTK_PORTMASK_CLEAR(*pLogicalPmask);

    for(phy_port = halCtrl->min_phy_port; phy_port <= halCtrl->max_phy_port; phy_port++)
    {
        if(physicalPortmask & (0x0001 << phy_port))
        {
            log_port = rtk_switch_port_P2L_get(phy_port);
            if(log_port != UNDEFINE_PORT)
            {
                RTK_PORTMASK_PORT_SET(*pLogicalPmask, log_port);
            }
        }
    }

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicVlanMemberConfig(rtk_uint32 index, rtl8367c_vlanconfiguser *pVlanCg)
{
    ret_t  retVal;
    rtk_uint32 page_idx;
    rtk_uint32 regAddr;
    rtk_uint32 regData;
    rtk_uint16 *tableAddr;
    rtk_uint16 smi_vlancfg[RTL8367C_VLAN_MBRCFG_LEN];

    if(index > RTL8367C_CVIDXMAX)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;

    memset(smi_vlancfg, 0x00, sizeof(rtk_uint16) * RTL8367C_VLAN_MBRCFG_LEN);
    tableAddr  = smi_vlancfg;

    for(page_idx = 0; page_idx < 4; page_idx++)  /* 4 pages per VLAN Member Config */
    {
        regAddr = RTL8367C_VLAN_MEMBER_CONFIGURATION_BASE + (index * 4) + page_idx;

        retVal = rtl8367c_getAsicReg(regAddr, &regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        *tableAddr = (rtk_uint16)regData;
        tableAddr++;
    }

    _rtl8367c_VlanMCStSmi2User(smi_vlancfg, pVlanCg);
    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicVlanMemberConfig(rtk_uint32 index, rtl8367c_vlanconfiguser *pVlanCg)
{
    ret_t  retVal;
    rtk_uint32 regAddr;
    rtk_uint32 regData;
    rtk_uint16 *tableAddr;
    rtk_uint32 page_idx;
    rtk_uint16 smi_vlancfg[RTL8367C_VLAN_MBRCFG_LEN];

    /* Error Checking  */
    if(index > RTL8367C_CVIDXMAX)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;

    if(pVlanCg->evid > RTL8367C_EVIDMAX)
        return RT_ERR_INPUT;


    if(pVlanCg->mbr > RTL8367C_PORTMASK)
        return RT_ERR_PORT_MASK;

    if(pVlanCg->fid_msti > RTL8367C_FIDMAX)
        return RT_ERR_L2_FID;

    if(pVlanCg->meteridx > RTL8367C_METERMAX)
        return RT_ERR_FILTER_METER_ID;

    if(pVlanCg->vbpri > RTL8367C_PRIMAX)
        return RT_ERR_QOS_INT_PRIORITY;

    memset(smi_vlancfg, 0x00, sizeof(rtk_uint16) * RTL8367C_VLAN_MBRCFG_LEN);
    _rtl8367c_VlanMCStUser2Smi(pVlanCg, smi_vlancfg);
    tableAddr = smi_vlancfg;

    for(page_idx = 0; page_idx < 4; page_idx++)  /* 4 pages per VLAN Member Config */
    {
        regAddr = RTL8367C_VLAN_MEMBER_CONFIGURATION_BASE + (index * 4) + page_idx;
        regData = *tableAddr;

        retVal = rtl8367c_setAsicReg(regAddr, regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        tableAddr++;
    }

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_get(rtk_vlan_t vid, rtk_vlan_cfg_t *pVlanCfg)
{
    rtk_api_ret_t retVal;
	rtk_uint32 phyMbrPmask;
    rtk_uint32 phyUntagPmask;
    rtl8367c_user_vlan4kentry vlan4K;
    rtl8367c_vlanconfiguser vlanMC;
    rtk_uint32 idx;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* vid must be 0~8191 */
    if (vid > RTL8367C_EVIDMAX)
        return RT_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pVlanCfg)
        return RT_ERR_NULL_POINTER;

    if (vid <= RTL8367C_VIDMAX)
    {
        vlan4K.vid = vid;

        if ((retVal = rtl8367c_getAsicVlan4kEntry(&vlan4K)) != RT_ERR_OK)
            return retVal;

        phyMbrPmask   = vlan4K.mbr;
        phyUntagPmask = vlan4K.untag;
        if(rtk_switch_portmask_P2L_get(phyMbrPmask, &(pVlanCfg->mbr)) != RT_ERR_OK)
            return RT_ERR_FAILED;

        if(rtk_switch_portmask_P2L_get(phyUntagPmask, &(pVlanCfg->untag)) != RT_ERR_OK)
            return RT_ERR_FAILED;

        pVlanCfg->ivl_en    = vlan4K.ivl_svl;
        pVlanCfg->fid_msti  = vlan4K.fid_msti;
        pVlanCfg->envlanpol = vlan4K.envlanpol;
        pVlanCfg->meteridx  = vlan4K.meteridx;
        pVlanCfg->vbpen     = vlan4K.vbpen;
        pVlanCfg->vbpri     = vlan4K.vbpri;
    }
    else
    {
        for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[idx] == vid)
                {
                    if ((retVal = rtl8367c_getAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
                        return retVal;

                    phyMbrPmask   = vlanMC.mbr;
                    if(rtk_switch_portmask_P2L_get(phyMbrPmask, &(pVlanCfg->mbr)) != RT_ERR_OK)
                        return RT_ERR_FAILED;

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

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_switch_isPortMaskValid(rtk_portmask_t *pPmask)
{
    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(NULL == pPmask)
        return RT_ERR_NULL_POINTER;

    if( (pPmask->bits[0] | halCtrl->valid_portmask) != halCtrl->valid_portmask )
        return RT_ERR_FAILED;
    else
        return RT_ERR_OK;
}

rtk_api_ret_t rtk_switch_portmask_L2P_get(rtk_portmask_t *pLogicalPmask, rtk_uint32 *pPhysicalPortmask)
{
    rtk_uint32 log_port, phy_port;

    if(init_state != INIT_COMPLETED)
        return RT_ERR_NOT_INIT;

    if(NULL == pLogicalPmask)
        return RT_ERR_NULL_POINTER;

    if(NULL == pPhysicalPortmask)
        return RT_ERR_NULL_POINTER;

    if(rtk_switch_isPortMaskValid(pLogicalPmask) != RT_ERR_OK)
        return RT_ERR_PORT_MASK;

    /* reset physical port mask */
    *pPhysicalPortmask = 0;

    RTK_PORTMASK_SCAN((*pLogicalPmask), log_port)
    {
        phy_port = rtk_switch_port_L2P_get((rtk_port_t)log_port);
        *pPhysicalPortmask |= (0x0001 << phy_port);
    }

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_set(rtk_vlan_t vid, rtk_vlan_cfg_t *pVlanCfg)
{
    rtk_api_ret_t retVal;
	rtk_uint32 phyMbrPmask;
    rtk_uint32 phyUntagPmask;
    rtl8367c_user_vlan4kentry vlan4K;
    rtl8367c_vlanconfiguser vlanMC;
    rtk_uint32 idx;
    rtk_uint32 empty_index = 0xffff;
    rtk_uint32 update_evid = 0;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* vid must be 0~8191 */
    if (vid > RTL8367C_EVIDMAX)
        return RT_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pVlanCfg)
        return RT_ERR_NULL_POINTER;

    /* Check port mask valid */
    RTK_CHK_PORTMASK_VALID(&(pVlanCfg->mbr));

    if (vid <= RTL8367C_VIDMAX)
    {
        /* Check untag port mask valid */
        RTK_CHK_PORTMASK_VALID(&(pVlanCfg->untag));
    }

    /* IVL_EN */
    if(pVlanCfg->ivl_en >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    /* fid must be 0~15 */
    if(pVlanCfg->fid_msti > RTL8367C_FIDMAX)
        return RT_ERR_L2_FID;

    /* Policing */
    if(pVlanCfg->envlanpol >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    /* Meter ID */
    if(pVlanCfg->meteridx > RTK_MAX_METER_ID)
        return RT_ERR_INPUT;

    /* VLAN based priority */
    if(pVlanCfg->vbpen >= RTK_ENABLE_END)
        return RT_ERR_ENABLE;

    /* Priority */
    if(pVlanCfg->vbpri > RTL8367C_PRIMAX)
        return RT_ERR_INPUT;

    /* Get physical port mask */
    if(rtk_switch_portmask_L2P_get(&(pVlanCfg->mbr), &phyMbrPmask) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if(rtk_switch_portmask_L2P_get(&(pVlanCfg->untag), &phyUntagPmask) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (vid <= RTL8367C_VIDMAX)
    {
        /* update 4K table */
        memset(&vlan4K, 0, sizeof(rtl8367c_user_vlan4kentry));
        vlan4K.vid = vid;

        vlan4K.mbr    = (phyMbrPmask & 0xFFFF);
        vlan4K.untag  = (phyUntagPmask & 0xFFFF);

        vlan4K.ivl_svl      = pVlanCfg->ivl_en;
        vlan4K.fid_msti     = pVlanCfg->fid_msti;
        vlan4K.envlanpol    = pVlanCfg->envlanpol;
        vlan4K.meteridx     = pVlanCfg->meteridx;
        vlan4K.vbpen        = pVlanCfg->vbpen;
        vlan4K.vbpri        = pVlanCfg->vbpri;

        if ((retVal = rtl8367c_setAsicVlan4kEntry(&vlan4K)) != RT_ERR_OK)
            return retVal;

        /* Update Member configuration if exist */
        for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[idx] == vid)
                {
                    /* Found! Update */
                    if(phyMbrPmask == 0x00)
                    {
                        /* Member port = 0x00, delete this VLAN from Member Configuration */
                        memset(&vlanMC, 0x00, sizeof(rtl8367c_vlanconfiguser));
                        if ((retVal = rtl8367c_setAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
                            return retVal;

                        /* Clear Database */
                        vlan_mbrCfgUsage[idx] = MBRCFG_UNUSED;
                        vlan_mbrCfgVid[idx]   = 0;
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
                        if ((retVal = rtl8367c_setAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
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
        for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
        {
            if(vlan_mbrCfgUsage[idx] == MBRCFG_USED_BY_VLAN)
            {
                if(vlan_mbrCfgVid[idx] == vid)
                {
                    /* Found! Update */
                    if(phyMbrPmask == 0x00)
                    {
                        /* Member port = 0x00, delete this VLAN from Member Configuration */
                        memset(&vlanMC, 0x00, sizeof(rtl8367c_vlanconfiguser));
                        if ((retVal = rtl8367c_setAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
                            return retVal;

                        /* Clear Database */
                        vlan_mbrCfgUsage[idx] = MBRCFG_UNUSED;
                        vlan_mbrCfgVid[idx]   = 0;
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
                        if ((retVal = rtl8367c_setAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
                            return retVal;

                        break;
                    }

                    update_evid = 1;
                }
            }

            if(vlan_mbrCfgUsage[idx] == MBRCFG_UNUSED)
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
            if ((retVal = rtl8367c_setAsicVlanMemberConfig(empty_index, &vlanMC)) != RT_ERR_OK)
                return retVal;

            vlan_mbrCfgUsage[empty_index] = MBRCFG_USED_BY_VLAN;
            vlan_mbrCfgVid[empty_index] = vid;
        }
    }

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicVlanPortBasedVID(rtk_uint32 port, rtk_uint32 *pIndex, rtk_uint32 *pPri)
{
    rtk_uint32 regAddr,bit_mask;
    ret_t  retVal;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    regAddr = RTL8367C_VLAN_PVID_CTRL_REG(port);
    bit_mask = RTL8367C_PORT_VIDX_MASK(port);
    retVal = rtl8367c_getAsicRegBits(regAddr, bit_mask, pIndex);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_VLAN_PORTBASED_PRIORITY_REG(port);
    bit_mask = RTL8367C_VLAN_PORTBASED_PRIORITY_MASK(port);
    retVal = rtl8367c_getAsicRegBits(regAddr, bit_mask, pPri);
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicVlanPortBasedVID(rtk_uint32 port, rtk_uint32 index, rtk_uint32 pri)
{
    rtk_uint32 regAddr, bit_mask;
    ret_t  retVal;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(index > RTL8367C_CVIDXMAX)
        return RT_ERR_VLAN_ENTRY_NOT_FOUND;

    if(pri > RTL8367C_PRIMAX)
        return RT_ERR_QOS_INT_PRIORITY;

    regAddr = RTL8367C_VLAN_PVID_CTRL_REG(port);
    bit_mask = RTL8367C_PORT_VIDX_MASK(port);
    retVal = rtl8367c_setAsicRegBits(regAddr, bit_mask, index);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_VLAN_PORTBASED_PRIORITY_REG(port);
    bit_mask = RTL8367C_VLAN_PORTBASED_PRIORITY_MASK(port);
    retVal = rtl8367c_setAsicRegBits(regAddr, bit_mask, pri);
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_checkAndCreateMbr(rtk_vlan_t vid, rtk_uint32 *pIndex)
{
    rtk_api_ret_t retVal;
    rtl8367c_user_vlan4kentry vlan4K;
    rtl8367c_vlanconfiguser vlanMC;
    rtk_uint32 idx;
    rtk_uint32 empty_idx = 0xFFFF;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* vid must be 0~8191 */
    if (vid > RTL8367C_EVIDMAX)
        return RT_ERR_VLAN_VID;

    /* Null pointer check */
    if(NULL == pIndex)
        return RT_ERR_NULL_POINTER;

    /* Get 4K VLAN */
    if (vid <= RTL8367C_VIDMAX)
    {
        memset(&vlan4K, 0x00, sizeof(rtl8367c_user_vlan4kentry));
        vlan4K.vid = vid;
        if ((retVal = rtl8367c_getAsicVlan4kEntry(&vlan4K)) != RT_ERR_OK)
            return retVal;
    }

    /* Search exist entry */
    for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[idx] == MBRCFG_USED_BY_VLAN)
        {
            if(vlan_mbrCfgVid[idx] == vid)
            {
                /* Found! return index */
                *pIndex = idx;
                return RT_ERR_OK;
            }
        }
    }

    /* Not found, Read H/W Member Configuration table to update database */
    for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
    {
        if ((retVal = rtl8367c_getAsicVlanMemberConfig(idx, &vlanMC)) != RT_ERR_OK)
            return retVal;

        if( (vlanMC.evid == 0) && (vlanMC.mbr == 0x00))
        {
            vlan_mbrCfgUsage[idx]   = MBRCFG_UNUSED;
            vlan_mbrCfgVid[idx]     = 0;
        }
        else
        {
            vlan_mbrCfgUsage[idx]   = MBRCFG_USED_BY_VLAN;
            vlan_mbrCfgVid[idx]     = vlanMC.evid;
        }
    }

    /* Search exist entry again */
    for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[idx] == MBRCFG_USED_BY_VLAN)
        {
            if(vlan_mbrCfgVid[idx] == vid)
            {
                /* Found! return index */
                *pIndex = idx;
                return RT_ERR_OK;
            }
        }
    }

    /* try to look up an empty index */
    for (idx = 0; idx <= RTL8367C_CVIDXMAX; idx++)
    {
        if(vlan_mbrCfgUsage[idx] == MBRCFG_UNUSED)
        {
            empty_idx = idx;
            break;
        }
    }

    if(empty_idx == 0xFFFF)
    {
        /* No empty index */
        return RT_ERR_TBL_FULL;
    }

    if (vid > RTL8367C_VIDMAX)
    {
        /* > 4K, there is no 4K entry, create on member configuration directly */
        memset(&vlanMC, 0x00, sizeof(rtl8367c_vlanconfiguser));
        vlanMC.evid = vid;
        if ((retVal = rtl8367c_setAsicVlanMemberConfig(empty_idx, &vlanMC)) != RT_ERR_OK)
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
        if ((retVal = rtl8367c_setAsicVlanMemberConfig(empty_idx, &vlanMC)) != RT_ERR_OK)
            return retVal;
    }

    /* Update Database */
    vlan_mbrCfgUsage[empty_idx] = MBRCFG_USED_BY_VLAN;
    vlan_mbrCfgVid[empty_idx] = vid;

    *pIndex = empty_idx;
    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_portPvid_get(rtk_port_t port, rtk_vlan_t *pPvid, rtk_pri_t *pPriority)
{
    rtk_api_ret_t retVal;
	rtk_uint32 index, pri;
    rtl8367c_vlanconfiguser mbrCfg;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if(NULL == pPvid)
        return RT_ERR_NULL_POINTER;

    if(NULL == pPriority)
        return RT_ERR_NULL_POINTER;

    if ((retVal = rtl8367c_getAsicVlanPortBasedVID(rtk_switch_port_L2P_get(port), &index, &pri)) != RT_ERR_OK)
        return retVal;

    memset(&mbrCfg, 0x00, sizeof(rtl8367c_vlanconfiguser));
    if ((retVal = rtl8367c_getAsicVlanMemberConfig(index, &mbrCfg)) != RT_ERR_OK)
        return retVal;

    *pPvid = mbrCfg.evid;
    *pPriority = pri;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_portPvid_set(rtk_port_t port, rtk_vlan_t pvid, rtk_pri_t priority)
{
    rtk_api_ret_t retVal;
	rtk_uint32 index;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    /* vid must be 0~8191 */
    if (pvid > RTL8367C_EVIDMAX)
        return RT_ERR_VLAN_VID;

    /* priority must be 0~7 */
    if (priority > RTL8367C_PRIMAX)
        return RT_ERR_VLAN_PRIORITY;

    if((retVal = rtk_vlan_checkAndCreateMbr(pvid, &index)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicVlanPortBasedVID(rtk_switch_port_L2P_get(port), index, priority)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_phyStatus_get(rtk_port_t port, rtk_port_linkStatus_t *pLinkStatus, rtk_port_speed_t *pSpeed, rtk_port_duplex_t *pDuplex)
{
    rtk_api_ret_t retVal;
	rtk_uint32 phyData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_IS_UTP(port);

    if( (NULL == pLinkStatus) || (NULL == pSpeed) || (NULL == pDuplex) )
        return RT_ERR_NULL_POINTER;

    /*Get PHY resolved register*/
    if ((retVal = rtl8367c_getAsicPHYReg(rtk_switch_port_L2P_get(port), PHY_RESOLVED_REG, &phyData)) != RT_ERR_OK)
        return retVal;

    /*check link status*/
    if (phyData & (1<<2))
    {
        *pLinkStatus = 1;

        /*check link speed*/
        *pSpeed = (phyData&0x0030) >> 4;

        /*check link duplex*/
        *pDuplex = (phyData&0x0008) >> 3;
    }
    else
    {
        *pLinkStatus = 0;
        *pSpeed = 0;
        *pDuplex = 0;
    }

    return RT_ERR_OK;
}

ret_t rtl8367c_resetVlan(void)
{
    ret_t   retVal;

    if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_VLAN_EXT_CTRL2, RTL8367C_VLAN_EXT_CTRL2_OFFSET, 1)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_vlan_reset(void)
{
    rtk_api_ret_t retVal;

	/* Check initialization state */
    RTK_CHK_INIT_STATE();

    if ((retVal = rtl8367c_resetVlan()) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicPortExtMode(rtk_uint32 id, rtk_uint32 *pMode)
{
    ret_t   retVal;
    rtk_uint32 regData, regValue, type;

    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }


    if (1 == type)
    {

        if (1 == id)
        {
            if( (retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, &regData)) != RT_ERR_OK)
                return retVal;

            if(1 == regData)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            if( (retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, &regData)) != RT_ERR_OK)
                return retVal;

            if(1 == regData)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
        }

        if(0 == id || 1 == id)
            return rtl8367c_getAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (id * RTL8367C_SELECT_GMII_1_OFFSET), pMode);
        else
           return rtl8367c_getAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, pMode);

    }
    else if (2 == type)
    {
        if (1 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x4000)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            else if (((regData >> 8) & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if (((regData >> 8) & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(0x1305, 0xf0, pMode);
        }
        else if (2 == id)
        {
#if 0
            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 6, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }

            if ((retVal = rtl8367c_getAsicRegBit(0x1d92, 7, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData == 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
#endif
            if ((retVal = rtl8367c_getAsicReg(0x1d92, &regData))!=RT_ERR_OK)
                return retVal;

            if (regData & 0x40)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if (regData & 0x80)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 4)
            {
                *pMode = EXT_1000X;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 5)
            {
                *pMode = EXT_100FX;
                return RT_ERR_OK;
            }
            else if ((regData & 0x1f) == 7)
            {
                *pMode = EXT_1000X_100FX;
                return RT_ERR_OK;
            }

            return rtl8367c_getAsicRegBits(0x1305, 0xf, pMode);
        }
    }
    else if(3 == type)
    {
        if (1 == id)
        {
            /* SDS_CFG_NEW */
            if ((retVal = rtl8367c_getAsicReg(0x1d95, &regData))!=RT_ERR_OK)
                return retVal;

            if ((retVal = rtl8367c_getAsicReg(0x1d41, &regValue))!=RT_ERR_OK)
                return retVal;


            if((regValue & 0xa0)  == 0xa0 )
            {

                regData = regData >> 8;
                if((regData & 0x1f) == 4)
                {
                    *pMode = EXT_1000X;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 5)
                {
                    *pMode = EXT_100FX;
                     return RT_ERR_OK;
                }
                else if((regData & 0x1f) == 7)
                {
                    *pMode = EXT_1000X_100FX;
                     return RT_ERR_OK;
                }

            }


            if ((retVal = rtl8367c_getAsicReg(0x1d11, &regData))!=RT_ERR_OK)
                return retVal;

            /* check cfg_mac6_sel_sgmii */
            if((regData >> 6) & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else if((regData >> 11) & 1)
            {
                *pMode = EXT_HSGMII;
                return RT_ERR_OK;
            }
            else
            {
                /* check port6 MAC mode */
                if ((retVal = rtl8367c_getAsicRegBits(0x1305, 0xf0, &regData))!=RT_ERR_OK)
                    return retVal;

                *pMode = regData;
                return RT_ERR_OK;
            }
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1d95, &regData))!=RT_ERR_OK)
                return retVal;


            if(((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x4))
            {
                *pMode = EXT_1000X;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x5))
            {
                *pMode = EXT_100FX;
                    return RT_ERR_OK;
            }
            else if (((regData & 0x3) == 3) && (((regData >> 8) & 0x1f) == 0x7))
            {
                *pMode = EXT_1000X_100FX;
                    return RT_ERR_OK;
            }
            else if(regData & 1)
            {
                *pMode = EXT_SGMII;
                return RT_ERR_OK;
            }
            else
            {

                if ((retVal = rtl8367c_getAsicRegBits(0x13c3, 0xf, &regData))!=RT_ERR_OK)
                    return retVal;

                *pMode = regData;

                return RT_ERR_OK;
            }
        }
    }

    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicPortForceLinkExt(rtk_uint32 id, rtl8367c_port_ability_t *pPortAbility)
{
    rtk_uint32  reg_data, regValue, type;
    rtk_uint32  sgmiiSel;
    rtk_uint32  hsgmiiSel;
    rtk_uint32  Mode;
    ret_t       retVal;


    if(id >= RTL8367C_EXTNO)
        return RT_ERR_OUT_OF_RANGE;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_getAsicReg(0x1300, &regValue)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13C2, 0x0000)) != RT_ERR_OK)
        return retVal;

    type = 0;

    switch (regValue)
    {
        case 0x0276:
        case 0x0597:
        case 0x6367:
            type = 1;
            break;
        case 0x0652:
        case 0x6368:
            type = 2;
            break;
        case 0x0801:
        case 0x6511:
            type = 3;
            break;
        default:
            return RT_ERR_FAILED;
    }

    if (1 == type)
    {
        if(1 == id)
        {
            if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, &sgmiiSel)) != RT_ERR_OK)
                return retVal;

            if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, &hsgmiiSel)) != RT_ERR_OK)
                return retVal;

            if( (sgmiiSel == 1) || (hsgmiiSel == 1) )
            {
                memset(pPortAbility, 0x00, sizeof(rtl8367c_port_ability_t));
                pPortAbility->forcemode = 1;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->duplex = reg_data;

                if((retVal = rtl8367c_getAsicRegBits(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->speed = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->link = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->txpause = reg_data;

                if((retVal = rtl8367c_getAsicRegBit(RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, &reg_data)) != RT_ERR_OK)
                    return retVal;

                pPortAbility->rxpause = reg_data;

                return RT_ERR_OK;
            }
        }

        if(0 == id || 1 == id)
            retVal = rtl8367c_getAsicReg(RTL8367C_REG_DIGITAL_INTERFACE0_FORCE+id, &reg_data);
        else
            retVal = rtl8367c_getAsicReg(RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, &reg_data);

        if(retVal != RT_ERR_OK)
            return retVal;

        pPortAbility->forcemode = (reg_data >> 12) & 0x0001;
        pPortAbility->mstfault  = (reg_data >> 9) & 0x0001;
        pPortAbility->mstmode   = (reg_data >> 8) & 0x0001;
        pPortAbility->nway      = (reg_data >> 7) & 0x0001;
        pPortAbility->txpause   = (reg_data >> 6) & 0x0001;
        pPortAbility->rxpause   = (reg_data >> 5) & 0x0001;
        pPortAbility->link      = (reg_data >> 4) & 0x0001;
        pPortAbility->duplex    = (reg_data >> 2) & 0x0001;
        pPortAbility->speed     = reg_data & 0x0003;
    }
    else if (2 == type)
    {
        if (id == 1)
        {
            if ((retVal = rtl8367c_getAsicReg(0x1311, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
        else if (2 == id)
        {
            if ((retVal = rtl8367c_getAsicReg(0x13c4, &reg_data))!=RT_ERR_OK)
                return retVal;

            pPortAbility->forcemode = (reg_data >> 12) & 1;
            pPortAbility->duplex = (reg_data >> 2) & 1;
            pPortAbility->link = (reg_data >> 4) & 1;
            pPortAbility->speed = reg_data & 3;
            pPortAbility->rxpause = (reg_data >> 5) & 1;
            pPortAbility->txpause = (reg_data >> 6) & 1;
        }
    }
    else if (3 == type)
    {
        if (id == 1)
        {

            if((retVal = rtl8367c_getAsicPortExtMode(id, &Mode))!=RT_ERR_OK)
                return retVal;
            if(Mode < EXT_SGMII)
            {

                if ((retVal = rtl8367c_getAsicReg(0x1311, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_1000X_100FX)
            {
                if ((retVal = rtl8367c_getAsicReg(0x1d11, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 10) & 1;
                pPortAbility->link = (reg_data >> 9) & 1;
                pPortAbility->speed = (reg_data >> 7) & 3;
                pPortAbility->rxpause = (reg_data >> 14) & 1;
                pPortAbility->txpause = (reg_data >> 13) & 1;
            }
            else if(Mode < EXT_RGMII_2)
            {
                if ((retVal = rtl8367c_getAsicReg(0x1358, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }

        }
        else if (2 == id)
        {
            if((retVal = rtl8367c_getAsicPortExtMode(id, &Mode))!=RT_ERR_OK)
                return retVal;
            if(Mode < EXT_SGMII)
            {

                if ((retVal = rtl8367c_getAsicReg(0x13c4, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_1000X_100FX)
            {
                if ((retVal = rtl8367c_getAsicReg(0x1d11, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 10) & 1;
                pPortAbility->link = (reg_data >> 9) & 1;
                pPortAbility->speed = (reg_data >> 7) & 3;
                pPortAbility->rxpause = (reg_data >> 14) & 1;
                pPortAbility->txpause = (reg_data >> 13) & 1;
            }
            else if(Mode < EXT_RGMII_2)
            {
                if ((retVal = rtl8367c_getAsicReg(0x1359, &reg_data))!=RT_ERR_OK)
                    return retVal;

                //pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
            else if(Mode < EXT_END)
            {

                if ((retVal = rtl8367c_getAsicReg(0x1311, &reg_data))!=RT_ERR_OK)
                    return retVal;

                pPortAbility->forcemode = (reg_data >> 12) & 1;
                pPortAbility->duplex = (reg_data >> 2) & 1;
                pPortAbility->link = (reg_data >> 4) & 1;
                pPortAbility->speed = reg_data & 3;
                pPortAbility->rxpause = (reg_data >> 5) & 1;
                pPortAbility->txpause = (reg_data >> 6) & 1;
            }
        }
    }
    return RT_ERR_OK;
}

rtk_api_ret_t rtk_port_macForceLinkExt_get(rtk_port_t port, rtk_mode_ext_t *pMode, rtk_port_mac_ability_t *pPortability)
{
    rtk_api_ret_t retVal;
	rtl8367c_port_ability_t ability;
    rtk_uint32 ext_id;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_IS_EXT(port);

    if(NULL == pMode)
        return RT_ERR_NULL_POINTER;

    if(NULL == pPortability)
        return RT_ERR_NULL_POINTER;

    ext_id = port - 15;

    if ((retVal = rtl8367c_getAsicPortExtMode(ext_id, (rtk_uint32 *)pMode)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_getAsicPortForceLinkExt(ext_id, &ability)) != RT_ERR_OK)
        return retVal;

    pPortability->forcemode = ability.forcemode;
    pPortability->speed     = (*pMode == MODE_EXT_HSGMII) ? PORT_SPEED_2500M : ability.speed;
    pPortability->duplex    = ability.duplex;
    pPortability->link      = ability.link;
    pPortability->nway      = ability.nway;
    pPortability->txpause   = ability.txpause;
    pPortability->rxpause   = ability.rxpause;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicVlanEgressTagMode(rtk_uint32 port, rtl8367c_egtagmode tagMode)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(tagMode >= EG_TAG_MODE_END)
        return RT_ERR_INPUT;

    return rtl8367c_setAsicRegBits(RTL8367C_PORT_MISC_CFG_REG(port), RTL8367C_VLAN_EGRESS_MDOE_MASK, tagMode);
}

ret_t rtl8367c_setAsicVlanIngressFilter(rtk_uint32 port, rtk_uint32 enabled)
{
    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_VLAN_INGRESS_REG, port, enabled);
}

ret_t rtl8367c_setAsicVlanFilter(rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(RTL8367C_REG_VLAN_CTRL, RTL8367C_VLAN_CTRL_OFFSET, enabled);
}

rtk_api_ret_t rtk_vlan_init(void)
{
    rtk_api_ret_t retVal;
	rtk_uint32 i;
    rtl8367c_user_vlan4kentry vlan4K;
    rtl8367c_vlanconfiguser vlanMC;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Clean Database */
    memset(vlan_mbrCfgVid, 0x00, sizeof(rtk_vlan_t) * RTL8367C_CVIDXNO);
    memset(vlan_mbrCfgUsage, 0x00, sizeof(vlan_mbrCfgType_t) * RTL8367C_CVIDXNO);

    /* clean 32 VLAN member configuration */
    for (i = 0; i <= RTL8367C_CVIDXMAX; i++)
    {
        vlanMC.evid = 0;
        vlanMC.mbr = 0;
        vlanMC.fid_msti = 0;
        vlanMC.envlanpol = 0;
        vlanMC.meteridx = 0;
        vlanMC.vbpen = 0;
        vlanMC.vbpri = 0;
        if ((retVal = rtl8367c_setAsicVlanMemberConfig(i, &vlanMC)) != RT_ERR_OK)
            return retVal;
    }

    /* Set a default VLAN with vid 1 to 4K table for all ports */
    memset(&vlan4K, 0, sizeof(rtl8367c_user_vlan4kentry));
    vlan4K.vid = 1;
    vlan4K.mbr = RTK_PHY_PORTMASK_ALL;
    vlan4K.untag = RTK_PHY_PORTMASK_ALL;
    vlan4K.fid_msti = 0;
    if ((retVal = rtl8367c_setAsicVlan4kEntry(&vlan4K)) != RT_ERR_OK)
        return retVal;

    /* Also set the default VLAN to 32 member configuration index 0 */
    memset(&vlanMC, 0, sizeof(rtl8367c_vlanconfiguser));
    vlanMC.evid = 1;
    vlanMC.mbr = RTK_PHY_PORTMASK_ALL;
    vlanMC.fid_msti = 0;
    if ((retVal = rtl8367c_setAsicVlanMemberConfig(0, &vlanMC)) != RT_ERR_OK)
            return retVal;

    /* Set all ports PVID to default VLAN and tag-mode to original */
    RTK_SCAN_ALL_PHY_PORTMASK(i)
    {
        if ((retVal = rtl8367c_setAsicVlanPortBasedVID(i, 0, 0)) != RT_ERR_OK)
            return retVal;
        if ((retVal = rtl8367c_setAsicVlanEgressTagMode(i, EG_TAG_MODE_ORI)) != RT_ERR_OK)
            return retVal;
    }

    /* Updata Databse */
    vlan_mbrCfgUsage[0] = MBRCFG_USED_BY_VLAN;
    vlan_mbrCfgVid[0] = 1;

    /* Enable Ingress filter */
    RTK_SCAN_ALL_PHY_PORTMASK(i)
    {
        if ((retVal = rtl8367c_setAsicVlanIngressFilter(i, ENABLED)) != RT_ERR_OK)
            return retVal;
    }

    /* enable VLAN */
    if ((retVal = rtl8367c_setAsicVlanFilter(ENABLED)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicForceGroupLed(rtk_uint32 groupmask, rtk_uint32 mode)
{
    ret_t retVal;
    rtk_uint32 i,bitmask;
    CONST rtk_uint16 bits[3]= {0x0004,0x0010,0x0040};

    /* Invalid input parameter */
    if(groupmask > RTL8367C_LEDGROUPMASK)
        return RT_ERR_OUT_OF_RANGE;

    if(mode >= LEDFORCEMODE_END)
        return RT_ERR_OUT_OF_RANGE;

    bitmask = 0;
    for(i = 0; i <  RTL8367C_LEDGROUPNO; i++)
    {
        if(groupmask & (1 << i))
        {
            bitmask = bitmask | bits[i];
        }

    }

    retVal = rtl8367c_setAsicRegBits(RTL8367C_LED_FORCE_CTRL, RTL8367C_LED_FORCE_MODE_MASK, bitmask);

    retVal = rtl8367c_setAsicRegBits(RTL8367C_LED_FORCE_CTRL, RTL8367C_FORCE_MODE_MASK, mode);

    if(LEDFORCEMODE_NORMAL == mode)
        retVal = rtl8367c_setAsicRegBits(RTL8367C_LED_FORCE_CTRL, RTL8367C_LED_FORCE_MODE_MASK, 0);

    return retVal;
}

rtk_api_ret_t rtk_led_groupAbility_set(rtk_led_group_t group, rtk_led_ability_t *pAbility)
{
	rtk_api_ret_t retVal;
	rtk_uint32 regData;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if (LED_GROUP_END <= group)
        return RT_ERR_FAILED;

    if(pAbility == NULL)
        return RT_ERR_NULL_POINTER;

    if( (pAbility->link_10m >= RTK_ENABLE_END) || (pAbility->link_100m >= RTK_ENABLE_END)||
        (pAbility->link_500m >= RTK_ENABLE_END) || (pAbility->link_1000m >= RTK_ENABLE_END)||
        (pAbility->act_rx >= RTK_ENABLE_END) || (pAbility->act_tx >= RTK_ENABLE_END) )
    {
        return RT_ERR_INPUT;
    }

    if ((retVal = rtl8367c_getAsicReg(RTL8367C_REG_LED0_DATA_CTRL + (rtk_uint32)group, &regData)) != RT_ERR_OK)
        return retVal;

    if(pAbility->link_10m == ENABLED)
        regData |= 0x0001;
    else
        regData &= ~0x0001;

    if(pAbility->link_100m == ENABLED)
        regData |= 0x0002;
    else
        regData &= ~0x0002;

    if(pAbility->link_500m == ENABLED)
        regData |= 0x0004;
    else
        regData &= ~0x0004;

    if(pAbility->link_1000m == ENABLED)
        regData |= 0x0008;
    else
        regData &= ~0x0008;

    if(pAbility->act_rx == ENABLED)
        regData |= 0x0010;
    else
        regData &= ~0x0010;

    if(pAbility->act_tx == ENABLED)
        regData |= 0x0020;
    else
        regData &= ~0x0020;

    regData |= (0x0001 << 6);

    if ((retVal = rtl8367c_setAsicReg(RTL8367C_REG_LED0_DATA_CTRL + (rtk_uint32)group, regData)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicLedIndicateInfoConfig(rtk_uint32 ledno, rtk_uint32 config)
{
    ret_t   retVal;
    CONST rtk_uint16 bits[RTL8367C_LEDGROUPNO] = {RTL8367C_LED0_CFG_MASK, RTL8367C_LED1_CFG_MASK, RTL8367C_LED2_CFG_MASK};

    if(ledno >= RTL8367C_LEDGROUPNO)
        return RT_ERR_OUT_OF_RANGE;

    if(config >= LEDCONF_END)
        return RT_ERR_OUT_OF_RANGE;

    retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_LED_CONFIGURATION, RTL8367C_LED_CONFIG_SEL_OFFSET, 0);
    if(retVal != RT_ERR_OK)
        return retVal;

    return rtl8367c_setAsicRegBits(RTL8367C_REG_LED_CONFIGURATION, bits[ledno], config);
}

rtk_api_ret_t rtk_led_groupConfig_set(rtk_led_group_t group, rtk_led_congig_t config)
{
    rtk_api_ret_t retVal;

	/* Check initialization state */
    RTK_CHK_INIT_STATE();

    if (LED_GROUP_END <= group)
        return RT_ERR_FAILED;

    if (LED_CONFIG_END <= config)
        return RT_ERR_FAILED;

    if ((retVal = rtl8367c_setAsicLedIndicateInfoConfig(group, config)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_switch_initialState_set(init_state_t state)
{
    if(state >= INIT_STATE_END)
        return RT_ERR_FAILED;

    init_state = state;
    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicPortIngressBandwidth(rtk_uint32 port, rtk_uint32 bandwidth, rtk_uint32 preifg, rtk_uint32 enableFC)
{
    ret_t retVal;
    rtk_uint32 regData;
    rtk_uint32 regAddr;

    /* Invalid input parameter */
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    if(bandwidth > RTL8367C_QOS_GRANULARTY_MAX)
        return RT_ERR_OUT_OF_RANGE;

    regAddr = RTL8367C_INGRESSBW_PORT_RATE_LSB_REG(port);
    regData = bandwidth & RTL8367C_QOS_GRANULARTY_LSB_MASK;
    retVal = rtl8367c_setAsicReg(regAddr, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr += 1;
    regData = (bandwidth & RTL8367C_QOS_GRANULARTY_MSB_MASK) >> RTL8367C_QOS_GRANULARTY_MSB_OFFSET;
    retVal = rtl8367c_setAsicRegBits(regAddr, RTL8367C_INGRESSBW_PORT0_RATE_CTRL1_INGRESSBW_RATE16_MASK, regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_MISC_CFG_REG(port);
    retVal = rtl8367c_setAsicRegBit(regAddr, RTL8367C_PORT0_MISC_CFG_INGRESSBW_IFG_OFFSET, preifg);
    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_MISC_CFG_REG(port);
    retVal = rtl8367c_setAsicRegBit(regAddr, RTL8367C_PORT0_MISC_CFG_INGRESSBW_FLOWCTRL_OFFSET, enableFC);
    if(retVal != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicPortEgressRate(rtk_uint32 port, rtk_uint32 rate)
{
    ret_t retVal;
    rtk_uint32 regAddr, regData;

    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(rate > RTL8367C_QOS_GRANULARTY_MAX)
        return RT_ERR_QOS_EBW_RATE;

    regAddr = RTL8367C_PORT_EGRESSBW_LSB_REG(port);
    regData = RTL8367C_QOS_GRANULARTY_LSB_MASK & rate;

    retVal = rtl8367c_setAsicReg(regAddr, regData);

    if(retVal != RT_ERR_OK)
        return retVal;

    regAddr = RTL8367C_PORT_EGRESSBW_MSB_REG(port);
    regData = (RTL8367C_QOS_GRANULARTY_MSB_MASK & rate) >> RTL8367C_QOS_GRANULARTY_MSB_OFFSET;

    retVal = rtl8367c_setAsicRegBits(regAddr, RTL8367C_PORT6_EGRESSBW_CTRL1_MASK, regData);

    return retVal;
}

ret_t rtl8367c_setAsicPortEgressRateIfg(rtk_uint32 ifg)
{
    ret_t retVal;

    retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SCHEDULE_WFQ_CTRL, RTL8367C_SCHEDULE_WFQ_CTRL_OFFSET, ifg);

    return retVal;
}

ret_t rtl8367c_setAsicLutIpLookupMethod(rtk_uint32 type)
{
    return rtl8367c_setAsicRegBit(RTL8367C_REG_LUT_CFG, RTL8367C_LUT_IPMC_LOOKUP_OP_OFFSET, type);
}

ret_t rtl8367c_setAsicRma(rtk_uint32 index, rtl8367c_rma_t* pRmacfg)
{
    rtk_uint32 regData = 0;
    ret_t retVal;

    if(index > RTL8367C_RMAMAX)
        return RT_ERR_RMA_ADDR;

    regData |= (pRmacfg->portiso_leaky & 0x0001);
    regData |= ((pRmacfg->vlan_leaky & 0x0001) << 1);
    regData |= ((pRmacfg->keep_format & 0x0001) << 2);
    regData |= ((pRmacfg->trap_priority & 0x0007) << 3);
    regData |= ((pRmacfg->discard_storm_filter & 0x0001) << 6);
    regData |= ((pRmacfg->operation & 0x0003) << 7);

    if( (index >= 0x4 && index <= 0x7) || (index >= 0x9 && index <= 0x0C) || (0x0F == index))
        index = 0x04;
    else if((index >= 0x13 && index <= 0x17) || (0x19 == index) || (index >= 0x1B && index <= 0x1f))
        index = 0x13;
    else if(index >= 0x22 && index <= 0x2F)
        index = 0x22;

    retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_RMA_CTRL00, RTL8367C_TRAP_PRIORITY_MASK, pRmacfg->trap_priority);
    if(retVal != RT_ERR_OK)
        return retVal;

    return rtl8367c_setAsicReg(RTL8367C_REG_RMA_CTRL00+index, regData);
}

ret_t rtl8367c_setAsicPortMirrorIsolationTxLeaky(rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(RTL8367C_REG_MIRROR_CTRL2, RTL8367C_MIRROR_TX_ISOLATION_LEAKY_OFFSET, enabled);
}

static rtk_api_ret_t _rtk_switch_init_8367c(void)
{
    rtk_port_t port;
    rtk_uint32 retVal;
    rtk_uint32 regData;
    rtk_uint32 regValue;

    if( (retVal = rtl8367c_setAsicReg(0x13c2, 0x0249)) != RT_ERR_OK)
        return retVal;

    if( (retVal = rtl8367c_getAsicReg(0x1301, &regValue)) != RT_ERR_OK)
        return retVal;

    if( (retVal = rtl8367c_setAsicReg(0x13c2, 0x0000)) != RT_ERR_OK)
        return retVal;

    RTK_SCAN_ALL_LOG_PORT(port)
    {
         if(rtk_switch_isUtpPort(port) == RT_ERR_OK)
         {
             if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_PORT0_EEECFG + (0x20 * port), RTL8367C_PORT0_EEECFG_EEE_100M_OFFSET, 1)) != RT_ERR_OK)
                 return retVal;

             if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_PORT0_EEECFG + (0x20 * port), RTL8367C_PORT0_EEECFG_EEE_GIGA_500M_OFFSET, 1)) != RT_ERR_OK)
                 return retVal;

             if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_PORT0_EEECFG + (0x20 * port), RTL8367C_PORT0_EEECFG_EEE_TX_OFFSET, 1)) != RT_ERR_OK)
                 return retVal;

             if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_PORT0_EEECFG + (0x20 * port), RTL8367C_PORT0_EEECFG_EEE_RX_OFFSET, 1)) != RT_ERR_OK)
                 return retVal;

             if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA428, &regData)) != RT_ERR_OK)
                return retVal;

             regData &= ~(0x0200);
             if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA428, regData)) != RT_ERR_OK)
                 return retVal;

             if((regValue & 0x00F0) == 0x00A0)
             {
                 if((retVal = rtl8367c_getAsicPHYOCPReg(port, 0xA5D0, &regData)) != RT_ERR_OK)
                     return retVal;

                 regData |= 0x0006;
                 if((retVal = rtl8367c_setAsicPHYOCPReg(port, 0xA5D0, regData)) != RT_ERR_OK)
                     return retVal;
             }
         }
    }

    if((retVal = rtl8367c_setAsicReg(RTL8367C_REG_UTP_FIB_DET, 0x15BB)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1303, 0x06D6)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1304, 0x0700)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13E2, 0x003F)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x13F9, 0x0090)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x121e, 0x03CA)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1233, 0x0352)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1237, 0x00a0)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x123a, 0x0030)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1239, 0x0084)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x0301, 0x1000)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x1349, 0x001F)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicRegBit(0x18e0, 0, 0)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicRegBit(0x122b, 14, 1)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicRegBits(0x1305, 0xC000, 3)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_switch_init(void)
{
    rtk_api_ret_t retVal;
	rtl8367c_rma_t rmaCfg;

    /* probe switch */
    halCtrl = &rtl8367c_hal_Ctrl;

    /* Set initial state */

    if((retVal = rtk_switch_initialState_set(INIT_COMPLETED)) != RT_ERR_OK)
        return retVal;

    /* Initial */
	if((retVal = _rtk_switch_init_8367c()) != RT_ERR_OK)
		return retVal;

    /* Set Old max packet length to 16K */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_MAX_LENGTH_LIMINT_IPG, RTL8367C_MAX_LENTH_CTRL_MASK, 3)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_MAX_LEN_RX_TX, RTL8367C_MAX_LEN_RX_TX_MASK, 3)) != RT_ERR_OK)
        return retVal;

    /* ACL Mode */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_ACL_ACCESS_MODE, RTL8367C_ACL_ACCESS_MODE_MASK, 1)) != RT_ERR_OK)
        return retVal;

    /* Max rate */
    if((retVal = rtl8367c_setAsicPortIngressBandwidth(rtk_switch_port_L2P_get(halCtrl->hsg_logical_port), RTL8367C_QOS_RATE_INPUT_MAX_HSG>>3, DISABLED, ENABLED)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPortEgressRate(rtk_switch_port_L2P_get(halCtrl->hsg_logical_port), RTL8367C_QOS_RATE_INPUT_MAX_HSG>>3)) != RT_ERR_OK)
        return retVal;

    if ((retVal = rtl8367c_setAsicPortEgressRateIfg(ENABLED)) != RT_ERR_OK)
        return retVal;

    if((retVal = rtl8367c_setAsicReg(0x03fa, 0x0007)) != RT_ERR_OK)
        return retVal;

    /* Change unknown DA to per port setting */
    if((retVal = rtl8367c_setAsicRegBits(RTL8367C_PORT_SECURIT_CTRL_REG, RTL8367C_UNKNOWN_UNICAST_DA_BEHAVE_MASK, 3)) != RT_ERR_OK)
        return retVal;

    /* LUT lookup OP = 1 */
    if ((retVal = rtl8367c_setAsicLutIpLookupMethod(1))!=RT_ERR_OK)
        return retVal;

    /* Set RMA */
    rmaCfg.portiso_leaky = 0;
    rmaCfg.vlan_leaky = 0;
    rmaCfg.keep_format = 0;
    rmaCfg.trap_priority = 0;
    rmaCfg.discard_storm_filter = 0;
    rmaCfg.operation = 0;
    if ((retVal = rtl8367c_setAsicRma(2, &rmaCfg))!=RT_ERR_OK)
        return retVal;

    /* Enable TX Mirror isolation leaky */
    if ((retVal = rtl8367c_setAsicPortMirrorIsolationTxLeaky(ENABLED)) != RT_ERR_OK)
        return retVal;

    /* INT EN */
    if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_IO_MISC_FUNC, RTL8367C_INT_EN_OFFSET, 1)) != RT_ERR_OK)
        return retVal;

	PRINT("End %s\n", __func__);
    return RT_ERR_OK;
}

static rtk_api_ret_t _get_asic_mib_idx(rtk_stat_port_type_t cnt_idx, RTL8367C_MIBCOUNTER *pMib_idx)
{
    RTL8367C_MIBCOUNTER mib_asic_idx[STAT_PORT_CNTR_END]=
    {
        ifInOctets,                     /* STAT_IfInOctets */
        dot3StatsFCSErrors,             /* STAT_Dot3StatsFCSErrors */
        dot3StatsSymbolErrors,          /* STAT_Dot3StatsSymbolErrors */
        dot3InPauseFrames,              /* STAT_Dot3InPauseFrames */
        dot3ControlInUnknownOpcodes,    /* STAT_Dot3ControlInUnknownOpcodes */
        etherStatsFragments,            /* STAT_EtherStatsFragments */
        etherStatsJabbers,              /* STAT_EtherStatsJabbers */
        ifInUcastPkts,                  /* STAT_IfInUcastPkts */
        etherStatsDropEvents,           /* STAT_EtherStatsDropEvents */
        etherStatsOctets,               /* STAT_EtherStatsOctets */
        etherStatsUnderSizePkts,        /* STAT_EtherStatsUnderSizePkts */
        etherOversizeStats,             /* STAT_EtherOversizeStats */
        etherStatsPkts64Octets,         /* STAT_EtherStatsPkts64Octets */
        etherStatsPkts65to127Octets,    /* STAT_EtherStatsPkts65to127Octets */
        etherStatsPkts128to255Octets,   /* STAT_EtherStatsPkts128to255Octets */
        etherStatsPkts256to511Octets,   /* STAT_EtherStatsPkts256to511Octets */
        etherStatsPkts512to1023Octets,  /* STAT_EtherStatsPkts512to1023Octets */
        etherStatsPkts1024to1518Octets, /* STAT_EtherStatsPkts1024to1518Octets */
        ifInMulticastPkts,              /* STAT_EtherStatsMulticastPkts */
        ifInBroadcastPkts,              /* STAT_EtherStatsBroadcastPkts */
        ifOutOctets,                    /* STAT_IfOutOctets */
        dot3StatsSingleCollisionFrames, /* STAT_Dot3StatsSingleCollisionFrames */
        dot3StatMultipleCollisionFrames,/* STAT_Dot3StatsMultipleCollisionFrames */
        dot3sDeferredTransmissions,     /* STAT_Dot3StatsDeferredTransmissions */
        dot3StatsLateCollisions,        /* STAT_Dot3StatsLateCollisions */
        etherStatsCollisions,           /* STAT_EtherStatsCollisions */
        dot3StatsExcessiveCollisions,   /* STAT_Dot3StatsExcessiveCollisions */
        dot3OutPauseFrames,             /* STAT_Dot3OutPauseFrames */
        MIB_NOT_SUPPORT,                /* STAT_Dot1dBasePortDelayExceededDiscards */
        dot1dTpPortInDiscards,          /* STAT_Dot1dTpPortInDiscards */
        ifOutUcastPkts,                 /* STAT_IfOutUcastPkts */
        ifOutMulticastPkts,             /* STAT_IfOutMulticastPkts */
        ifOutBroadcastPkts,             /* STAT_IfOutBroadcastPkts */
        outOampduPkts,                  /* STAT_OutOampduPkts */
        inOampduPkts,                   /* STAT_InOampduPkts */
        MIB_NOT_SUPPORT,                /* STAT_PktgenPkts */
        inMldChecksumError,             /* STAT_InMldChecksumError */
        inIgmpChecksumError,            /* STAT_InIgmpChecksumError */
        inMldSpecificQuery,             /* STAT_InMldSpecificQuery */
        inMldGeneralQuery,              /* STAT_InMldGeneralQuery */
        inIgmpSpecificQuery,            /* STAT_InIgmpSpecificQuery */
        inIgmpGeneralQuery,             /* STAT_InIgmpGeneralQuery */
        inMldLeaves,                    /* STAT_InMldLeaves */
        inIgmpLeaves,                   /* STAT_InIgmpInterfaceLeaves */
        inIgmpJoinsSuccess,             /* STAT_InIgmpJoinsSuccess */
        inIgmpJoinsFail,                /* STAT_InIgmpJoinsFail */
        inMldJoinsSuccess,              /* STAT_InMldJoinsSuccess */
        inMldJoinsFail,                 /* STAT_InMldJoinsFail */
        inReportSuppressionDrop,        /* STAT_InReportSuppressionDrop */
        inLeaveSuppressionDrop,         /* STAT_InLeaveSuppressionDrop */
        outIgmpReports,                 /* STAT_OutIgmpReports */
        outIgmpLeaves,                  /* STAT_OutIgmpLeaves */
        outIgmpGeneralQuery,            /* STAT_OutIgmpGeneralQuery */
        outIgmpSpecificQuery,           /* STAT_OutIgmpSpecificQuery */
        outMldReports,                  /* STAT_OutMldReports */
        outMldLeaves,                   /* STAT_OutMldLeaves */
        outMldGeneralQuery,             /* STAT_OutMldGeneralQuery */
        outMldSpecificQuery,            /* STAT_OutMldSpecificQuery */
        inKnownMulticastPkts,           /* STAT_InKnownMulticastPkts */
        ifInMulticastPkts,              /* STAT_IfInMulticastPkts */
        ifInBroadcastPkts,              /* STAT_IfInBroadcastPkts */
        ifOutDiscards                   /* STAT_IfOutDiscards */
    };

    if(cnt_idx >= STAT_PORT_CNTR_END)
        return RT_ERR_STAT_INVALID_PORT_CNTR;

    if(mib_asic_idx[cnt_idx] == MIB_NOT_SUPPORT)
        return RT_ERR_CHIP_NOT_SUPPORTED;

    *pMib_idx = mib_asic_idx[cnt_idx];
    return RT_ERR_OK;
}

ret_t rtl8367c_getAsicMIBsCounter(rtk_uint32 port, RTL8367C_MIBCOUNTER mibIdx, rtk_uint64* pCounter)
{
    ret_t retVal;
    rtk_uint32 regAddr;
    rtk_uint32 regData;
    rtk_uint32 mibAddr;
    rtk_uint32 mibOff=0;

    /* address offset to MIBs counter */
    CONST rtk_uint16 mibLength[RTL8367C_MIBS_NUMBER]= {
        4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        4,2,2,2,2,2,2,2,2,
        4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

    rtk_uint16 i;
    rtk_uint64 mibCounter;


    if(port > RTL8367C_PORTIDMAX)
        return RT_ERR_PORT_ID;

    if(mibIdx >= RTL8367C_MIBS_NUMBER)
        return RT_ERR_STAT_INVALID_CNTR;

    if(dot1dTpLearnedEntryDiscards == mibIdx)
    {
        mibAddr = RTL8367C_MIB_LEARNENTRYDISCARD_OFFSET;
    }
    else
    {
        i = 0;
        mibOff = RTL8367C_MIB_PORT_OFFSET * port;

        if(port > 7)
            mibOff = mibOff + 68;

        while(i < mibIdx)
        {
            mibOff += mibLength[i];
            i++;
        }

        mibAddr = mibOff;
    }

    /* Read MIB addr before writing */
    retVal = rtl8367c_getAsicReg(RTL8367C_REG_MIB_ADDRESS, &regData);
    if(retVal != RT_ERR_OK)
        return retVal;

    if (regData == (mibAddr >> 2))
    {
        /* Write MIB addr to an alternate value */
        retVal = rtl8367c_setAsicReg(RTL8367C_REG_MIB_ADDRESS, (mibAddr >> 2) + 1);
        if(retVal != RT_ERR_OK)
            return retVal;

        while(1)
        {
            retVal = rtl8367c_getAsicReg(RTL8367C_REG_MIB_ADDRESS, &regData);
            if(retVal != RT_ERR_OK)
                return retVal;

            if(regData == ((mibAddr >> 2) + 1))
            {
                break;
            }

            retVal = rtl8367c_setAsicReg(RTL8367C_REG_MIB_ADDRESS, (mibAddr >> 2) + 1);
            if(retVal != RT_ERR_OK)
                return retVal;
        }

        /* polling busy flag */
        i = 100;
        while(i > 0)
        {
            /*read MIB control register*/
            retVal = rtl8367c_getAsicReg(RTL8367C_MIB_CTRL_REG,&regData);
            if(retVal != RT_ERR_OK)
                return retVal;

            if((regData & RTL8367C_MIB_CTRL0_BUSY_FLAG_MASK) == 0)
            {
                break;
            }

            i--;
        }

        if(regData & RTL8367C_MIB_CTRL0_BUSY_FLAG_MASK)
            return RT_ERR_BUSYWAIT_TIMEOUT;

        if(regData & RTL8367C_RESET_FLAG_MASK)
            return RT_ERR_STAT_CNTR_FAIL;
    }

    /*writing access counter address first*/
    /*This address is SRAM address, and SRAM address = MIB register address >> 2*/
    /*then ASIC will prepare 64bits counter wait for being retrived*/
    /*Write Mib related address to access control register*/
    retVal = rtl8367c_setAsicReg(RTL8367C_REG_MIB_ADDRESS, (mibAddr >> 2));
    if(retVal != RT_ERR_OK)
        return retVal;

    /* polling MIB Addr register */
    while(1)
    {
        retVal = rtl8367c_getAsicReg(RTL8367C_REG_MIB_ADDRESS, &regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        if(regData == (mibAddr >> 2))
        {
            break;
        }

        retVal = rtl8367c_setAsicReg(RTL8367C_REG_MIB_ADDRESS, (mibAddr >> 2));
        if(retVal != RT_ERR_OK)
            return retVal;
    }

    /* polling busy flag */
    i = 100;
    while(i > 0)
    {
        /*read MIB control register*/
        retVal = rtl8367c_getAsicReg(RTL8367C_MIB_CTRL_REG,&regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        if((regData & RTL8367C_MIB_CTRL0_BUSY_FLAG_MASK) == 0)
        {
            break;
        }

        i--;
    }

    if(regData & RTL8367C_MIB_CTRL0_BUSY_FLAG_MASK)
        return RT_ERR_BUSYWAIT_TIMEOUT;

    if(regData & RTL8367C_RESET_FLAG_MASK)
        return RT_ERR_STAT_CNTR_FAIL;

    mibCounter = 0;
    i = mibLength[mibIdx];
    if(4 == i)
        regAddr = RTL8367C_MIB_COUNTER_BASE_REG + 3;
    else
        regAddr = RTL8367C_MIB_COUNTER_BASE_REG + ((mibOff + 1) % 4);

    while(i)
    {
        retVal = rtl8367c_getAsicReg(regAddr, &regData);
        if(retVal != RT_ERR_OK)
            return retVal;

        mibCounter = (mibCounter << 16) | (regData & 0xFFFF);

        regAddr --;
        i --;

    }

    *pCounter = mibCounter;

    return RT_ERR_OK;
}

rtk_api_ret_t rtk_stat_port_get(rtk_port_t port, rtk_stat_port_type_t cntr_idx, rtk_stat_counter_t *pCntr)
{
    rtk_api_ret_t retVal;
    RTL8367C_MIBCOUNTER mib_idx;
    rtk_stat_counter_t  second_cnt;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pCntr)
        return RT_ERR_NULL_POINTER;

    /* Check port valid */
    RTK_CHK_PORT_VALID(port);

    if (cntr_idx>=STAT_PORT_CNTR_END)
        return RT_ERR_STAT_INVALID_PORT_CNTR;

    if((retVal = _get_asic_mib_idx(cntr_idx, &mib_idx)) != RT_ERR_OK)
        return retVal;

    if(mib_idx == MIB_NOT_SUPPORT)
        return RT_ERR_CHIP_NOT_SUPPORTED;

    if ((retVal = rtl8367c_getAsicMIBsCounter(rtk_switch_port_L2P_get(port), mib_idx, pCntr)) != RT_ERR_OK)
        return retVal;

    if(cntr_idx == STAT_EtherStatsMulticastPkts)
    {
        if((retVal = _get_asic_mib_idx(STAT_IfOutMulticastPkts, &mib_idx)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_getAsicMIBsCounter(rtk_switch_port_L2P_get(port), mib_idx, &second_cnt)) != RT_ERR_OK)
            return retVal;

        *pCntr += second_cnt;
    }

    if(cntr_idx == STAT_EtherStatsBroadcastPkts)
    {
        if((retVal = _get_asic_mib_idx(STAT_IfOutBroadcastPkts, &mib_idx)) != RT_ERR_OK)
            return retVal;

        if((retVal = rtl8367c_getAsicMIBsCounter(rtk_switch_port_L2P_get(port), mib_idx, &second_cnt)) != RT_ERR_OK)
            return retVal;

        *pCntr += second_cnt;
    }

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicMIBsCounterReset(rtk_uint32 greset, rtk_uint32 qmreset, rtk_uint32 portmask)
{
    ret_t retVal;
    rtk_uint32 regData;
    rtk_uint32 regBits;

    regBits = RTL8367C_GLOBAL_RESET_MASK |
                RTL8367C_QM_RESET_MASK |
                    RTL8367C_MIB_PORT07_MASK |
                    ((rtk_uint32)0x7 << 13);
    regData = ((greset << RTL8367C_GLOBAL_RESET_OFFSET) & RTL8367C_GLOBAL_RESET_MASK) |
                ((qmreset << RTL8367C_QM_RESET_OFFSET) & RTL8367C_QM_RESET_MASK) |
                (((portmask & 0xFF) << RTL8367C_PORT0_RESET_OFFSET) & RTL8367C_MIB_PORT07_MASK) |
                (((portmask >> 8)&0x7) << 13);


    retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_MIB_CTRL0, regBits, (regData >> RTL8367C_PORT0_RESET_OFFSET));

    return retVal;
}

rtk_api_ret_t rtk_stat_port_reset(rtk_port_t port)
{
    rtk_api_ret_t retVal;

	/* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check port valid */
    RTK_CHK_PORT_VALID(port);

    if ((retVal = rtl8367c_setAsicMIBsCounterReset(FALSE,FALSE,1 << rtk_switch_port_L2P_get(port))) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

rtk_port_t rtk_switch_maxLogicalPort_get(void)
{
    rtk_port_t port, maxLogicalPort = 0;

    /* Check initialization state */
    if(rtk_switch_initialState_get() != INIT_COMPLETED)
    {
        return UNDEFINE_PORT;
    }

    for(port = 0; port < RTK_SWITCH_PORT_NUM; port++)
    {
        if( (halCtrl->log_port_type[port] == UTP_PORT) || (halCtrl->log_port_type[port] == EXT_PORT) )
            maxLogicalPort = port;
    }

    return maxLogicalPort;
}

ret_t rtl8367c_setAsicPortMirror(rtk_uint32 source, rtk_uint32 monitor)
{
    ret_t retVal;

    if((source > RTL8367C_PORTIDMAX) || (monitor > RTL8367C_PORTIDMAX))
        return RT_ERR_PORT_ID;

    retVal = rtl8367c_setAsicRegBits(RTL8367C_MIRROR_CTRL_REG, RTL8367C_MIRROR_SOURCE_PORT_MASK, source);
    if(retVal != RT_ERR_OK)
        return retVal;


    return rtl8367c_setAsicRegBits(RTL8367C_MIRROR_CTRL_REG, RTL8367C_MIRROR_MONITOR_PORT_MASK, monitor);
}

ret_t rtl8367c_setAsicPortMirrorMask(rtk_uint32 SourcePortmask)
{
    if( SourcePortmask > RTL8367C_PORTMASK)
        return RT_ERR_PORT_MASK;

    return rtl8367c_setAsicRegBits(RTL8367C_REG_MIRROR_SRC_PMSK, RTL8367C_MIRROR_SRC_PMSK_MASK, SourcePortmask);
}

ret_t rtl8367c_setAsicPortMirrorRxFunction(rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(RTL8367C_MIRROR_CTRL_REG, RTL8367C_MIRROR_RX_OFFSET, enabled);
}

ret_t rtl8367c_setAsicPortMirrorTxFunction(rtk_uint32 enabled)
{
    return rtl8367c_setAsicRegBit(RTL8367C_MIRROR_CTRL_REG, RTL8367C_MIRROR_TX_OFFSET, enabled);
}

rtk_api_ret_t rtk_mirror_portBased_set(rtk_port_t mirroring_port, rtk_portmask_t *pMirrored_rx_portmask, rtk_portmask_t *pMirrored_tx_portmask)
{
	rtk_api_ret_t retVal;
	rtk_enable_t mirRx, mirTx;
    rtk_uint32 i, pmask;
    rtk_port_t source_port;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check port valid */
    RTK_CHK_PORT_VALID(mirroring_port);

    if(NULL == pMirrored_rx_portmask)
        return RT_ERR_NULL_POINTER;

    if(NULL == pMirrored_tx_portmask)
        return RT_ERR_NULL_POINTER;

    RTK_CHK_PORTMASK_VALID(pMirrored_rx_portmask);

    RTK_CHK_PORTMASK_VALID(pMirrored_tx_portmask);

    /*Mirror Sorce Port Mask Check*/
    if (pMirrored_tx_portmask->bits[0]!=pMirrored_rx_portmask->bits[0]&&pMirrored_tx_portmask->bits[0]!=0&&pMirrored_rx_portmask->bits[0]!=0)
        return RT_ERR_PORT_MASK;

     /*mirror port != source port*/
    if(RTK_PORTMASK_IS_PORT_SET((*pMirrored_tx_portmask), mirroring_port) || RTK_PORTMASK_IS_PORT_SET((*pMirrored_rx_portmask), mirroring_port))
        return RT_ERR_PORT_MASK;

    source_port = rtk_switch_maxLogicalPort_get();

    RTK_SCAN_ALL_LOG_PORT(i)
    {
        if (pMirrored_tx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }

        if (pMirrored_rx_portmask->bits[0]&(1<<i))
        {
            source_port = i;
            break;
        }
    }

    if ((retVal = rtl8367c_setAsicPortMirror(rtk_switch_port_L2P_get(source_port), rtk_switch_port_L2P_get(mirroring_port))) != RT_ERR_OK)
        return retVal;
    if(pMirrored_rx_portmask->bits[0] != 0)
    {
        if ((retVal = rtk_switch_portmask_L2P_get(pMirrored_rx_portmask, &pmask)) != RT_ERR_OK)
            return retVal;
        if ((retVal = rtl8367c_setAsicPortMirrorMask(pmask)) != RT_ERR_OK)
            return retVal;
    }
    else
    {
        if ((retVal = rtk_switch_portmask_L2P_get(pMirrored_tx_portmask, &pmask)) != RT_ERR_OK)
            return retVal;
        if ((retVal = rtl8367c_setAsicPortMirrorMask(pmask)) != RT_ERR_OK)
            return retVal;
    }


    if (pMirrored_rx_portmask->bits[0])
        mirRx = ENABLED;
    else
        mirRx = DISABLED;

    if ((retVal = rtl8367c_setAsicPortMirrorRxFunction(mirRx)) != RT_ERR_OK)
        return retVal;

    if (pMirrored_tx_portmask->bits[0])
        mirTx = ENABLED;
    else
        mirTx = DISABLED;

    if ((retVal = rtl8367c_setAsicPortMirrorTxFunction(mirTx)) != RT_ERR_OK)
        return retVal;

    return RT_ERR_OK;
}

ret_t rtl8367c_setAsicPortLoopback(rtk_uint32 port, rtk_uint32 enable)
{
    if(port >= RTL8367C_PORTNO)
        return RT_ERR_PORT_ID;

    return rtl8367c_setAsicRegBit(RTL8367C_PORT_MISC_CFG_REG(port), RTL8367C_PORT0_MISC_CFG_MAC_LOOPBACK_OFFSET, enable);
}

rtk_api_ret_t rtk_port_macLocalLoopbackEnable_set(rtk_port_t port, rtk_enable_t enable)
{
    rtk_api_ret_t retVal;
	rtk_uint32      data;

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    /* Check Port Valid */
    RTK_CHK_PORT_VALID(port);

    if(enable >= RTK_ENABLE_END)
        return RT_ERR_INPUT;

    if ((retVal = rtl8367c_setAsicPortLoopback(rtk_switch_port_L2P_get(port), enable)) != RT_ERR_OK)
        return retVal;

    if(rtk_switch_isUtpPort(port) == RT_ERR_OK)
    {
        if ((retVal = rtl8367c_getAsicPHYReg(rtk_switch_port_L2P_get(port), PHY_CONTROL_REG, &data)) != RT_ERR_OK)
            return retVal;

        if(enable == ENABLED)
            data |= (0x0001 << 14);
        else
            data &= ~(0x0001 << 14);

        if ((retVal = rtl8367c_setAsicPHYReg(rtk_switch_port_L2P_get(port), PHY_CONTROL_REG, data)) != RT_ERR_OK)
            return retVal;
    }

	return RT_ERR_OK;
}

rtk_api_ret_t rtk_stat_port_getAll(rtk_port_t port, rtk_stat_port_cntr_t *pPort_cntrs)
{
    rtk_api_ret_t retVal;
	rtk_uint32 mibIndex;
    rtk_uint64 mibCounter;
    rtk_uint32 *accessPtr;
    /* address offset to MIBs counter */
    CONST_T rtk_uint16 mibLength[STAT_PORT_CNTR_END]= {
        2,1,1,1,1,1,1,1,1,
        2,1,1,1,1,1,1,1,1,1,1,
        2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    /* Check initialization state */
    RTK_CHK_INIT_STATE();

    if(NULL == pPort_cntrs)
        return RT_ERR_NULL_POINTER;

    /* Check port valid */
    RTK_CHK_PORT_VALID(port);

    accessPtr = (rtk_uint32*)pPort_cntrs;
    for (mibIndex=0;mibIndex<STAT_PORT_CNTR_END;mibIndex++)
    {
        if ((retVal = rtk_stat_port_get(port, mibIndex, &mibCounter)) != RT_ERR_OK)
        {
            if (retVal == RT_ERR_CHIP_NOT_SUPPORTED)
                mibCounter = 0;
            else
                return retVal;
        }

        if (2 == mibLength[mibIndex])
            *(rtk_uint64*)accessPtr = mibCounter;
        else if (1 == mibLength[mibIndex])
            *accessPtr = mibCounter;
        else
            return RT_ERR_FAILED;

        accessPtr+=mibLength[mibIndex];
    }

    return RT_ERR_OK;
}

static void _rtl8367c_fdbStSmi2User( rtl8367c_luttb *pLutSt, rtk_uint16 *pFdbSmi)
{
    /*L3 lookup*/
    if(pFdbSmi[3] & 0x1000)
    {
        if(pFdbSmi[3] & 0x2000)
        {
            pLutSt->sip             = pFdbSmi[0] | (pFdbSmi[1] << 16);
            pLutSt->dip             = 0xE0000000 | pFdbSmi[2] | ((pFdbSmi[3] & 0x0FFF) << 16);

            pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
            pLutSt->l3_vid          = ((pFdbSmi[4] & 0xFF00) >> 8) | (pFdbSmi[5] & 0x000F);

            pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
            pLutSt->l3vidlookup     = (pFdbSmi[3] & 0x2000) >> 13;
            pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
        }
        else
        {
            pLutSt->sip             = pFdbSmi[0] | (pFdbSmi[1] << 16);
            pLutSt->dip             = 0xE0000000 | pFdbSmi[2] | ((pFdbSmi[3] & 0x0FFF) << 16);

            pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
            pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

            pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
            pLutSt->igmpidx         = (pFdbSmi[4] & 0xFF00) >> 8;

            pLutSt->igmp_asic       = (pFdbSmi[5] & 0x0001);
            pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
            pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
        }
    }
    else if(pFdbSmi[2] & 0x0100) /*Multicast L2 Lookup*/
    {
        pLutSt->mac.octet[0]    = (pFdbSmi[2] & 0xFF00) >> 8;
        pLutSt->mac.octet[1]    = (pFdbSmi[2] & 0x00FF);
        pLutSt->mac.octet[2]    = (pFdbSmi[1] & 0xFF00) >> 8;
        pLutSt->mac.octet[3]    = (pFdbSmi[1] & 0x00FF);
        pLutSt->mac.octet[4]    = (pFdbSmi[0] & 0xFF00) >> 8;
        pLutSt->mac.octet[5]    = (pFdbSmi[0] & 0x00FF);

        pLutSt->cvid_fid        = pFdbSmi[3] & 0x0FFF;
        pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
        pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

        pLutSt->mbr             = (pFdbSmi[4] & 0x00FF) | (((pFdbSmi[3] & 0xC000) >> 14) << 8) | (((pFdbSmi[5] & 0x0080) >> 7) << 10);
        pLutSt->igmpidx         = (pFdbSmi[4] & 0xFF00) >> 8;

        pLutSt->igmp_asic       = (pFdbSmi[5] & 0x0001);
        pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
        pLutSt->ivl_svl         = (pFdbSmi[3] & 0x2000) >> 13;
        pLutSt->nosalearn       = (pFdbSmi[5] & 0x0020) >> 5;
    }
    else /*Asic auto-learning*/
    {
        pLutSt->mac.octet[0]    = (pFdbSmi[2] & 0xFF00) >> 8;
        pLutSt->mac.octet[1]    = (pFdbSmi[2] & 0x00FF);
        pLutSt->mac.octet[2]    = (pFdbSmi[1] & 0xFF00) >> 8;
        pLutSt->mac.octet[3]    = (pFdbSmi[1] & 0x00FF);
        pLutSt->mac.octet[4]    = (pFdbSmi[0] & 0xFF00) >> 8;
        pLutSt->mac.octet[5]    = (pFdbSmi[0] & 0x00FF);

        pLutSt->cvid_fid        = pFdbSmi[3] & 0x0FFF;
        pLutSt->lut_pri         = (pFdbSmi[5] & 0x000E) >> 1;
        pLutSt->fwd_en          = (pFdbSmi[5] & 0x0010) >> 4;

        pLutSt->sa_en           = (pFdbSmi[4] & 0x0080) >> 7;
        pLutSt->auth            = (pFdbSmi[4] & 0x4000) >> 14;
        pLutSt->spa             = ((pFdbSmi[4] & 0x0700) >> 8) | (((pFdbSmi[3] & 0x8000) >> 15) << 3);
        pLutSt->age             = (pFdbSmi[4] & 0x3800) >> 11;
        pLutSt->fid             = (pFdbSmi[4] & 0x0078) >> 3;
        pLutSt->efid            = (pFdbSmi[4] & 0x0007);
        pLutSt->sa_block        = (pFdbSmi[4] & 0x8000) >> 15;

        pLutSt->da_block        = (pFdbSmi[5] & 0x0001);
        pLutSt->l3lookup        = (pFdbSmi[3] & 0x1000) >> 12;
        pLutSt->ivl_svl         = (pFdbSmi[3] & 0x2000) >> 13;
        pLutSt->nosalearn       = (pFdbSmi[3] & 0x0020) >> 5;
    }
}

static void _rtl8367c_fdbStUser2Smi( rtl8367c_luttb *pLutSt, rtk_uint16 *pFdbSmi)
{
    /* L3 lookup */
    if(pLutSt->l3lookup)
    {
        if(pLutSt->l3vidlookup)
        {
            pFdbSmi[0] = (pLutSt->sip & 0x0000FFFF);
            pFdbSmi[1] = (pLutSt->sip & 0xFFFF0000) >> 16;

            pFdbSmi[2] = (pLutSt->dip & 0x0000FFFF);
            pFdbSmi[3] = (pLutSt->dip & 0x0FFF0000) >> 16;

            pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
            pFdbSmi[3] |= (pLutSt->l3vidlookup & 0x0001) << 13;
            pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

            pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
            pFdbSmi[4] |= (pLutSt->l3_vid & 0x00FF) << 8;

            pFdbSmi[5] |= ((pLutSt->l3_vid & 0x0F00) >> 8);
            pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
            pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
        }
        else
        {
            pFdbSmi[0] = (pLutSt->sip & 0x0000FFFF);
            pFdbSmi[1] = (pLutSt->sip & 0xFFFF0000) >> 16;

            pFdbSmi[2] = (pLutSt->dip & 0x0000FFFF);
            pFdbSmi[3] = (pLutSt->dip & 0x0FFF0000) >> 16;

            pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
            pFdbSmi[3] |= (pLutSt->l3vidlookup & 0x0001) << 13;
            pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

            pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
            pFdbSmi[4] |= (pLutSt->igmpidx & 0x00FF) << 8;

            pFdbSmi[5] |= (pLutSt->igmp_asic & 0x0001);
            pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
            pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
            pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
            pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
        }
    }
    else if(pLutSt->mac.octet[0] & 0x01) /*Multicast L2 Lookup*/
    {
        pFdbSmi[0] |= pLutSt->mac.octet[5];
        pFdbSmi[0] |= pLutSt->mac.octet[4] << 8;

        pFdbSmi[1] |= pLutSt->mac.octet[3];
        pFdbSmi[1] |= pLutSt->mac.octet[2] << 8;

        pFdbSmi[2] |= pLutSt->mac.octet[1];
        pFdbSmi[2] |= pLutSt->mac.octet[0] << 8;

        pFdbSmi[3] |= pLutSt->cvid_fid;
        pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
        pFdbSmi[3] |= (pLutSt->ivl_svl & 0x0001) << 13;
        pFdbSmi[3] |= ((pLutSt->mbr & 0x0300) >> 8) << 14;

        pFdbSmi[4] |= (pLutSt->mbr & 0x00FF);
        pFdbSmi[4] |= (pLutSt->igmpidx & 0x00FF) << 8;

        pFdbSmi[5] |= pLutSt->igmp_asic;
        pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
        pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
        pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
        pFdbSmi[5] |= ((pLutSt->mbr & 0x0400) >> 10) << 7;
    }
    else /*Asic auto-learning*/
    {
        pFdbSmi[0] |= pLutSt->mac.octet[5];
        pFdbSmi[0] |= pLutSt->mac.octet[4] << 8;

        pFdbSmi[1] |= pLutSt->mac.octet[3];
        pFdbSmi[1] |= pLutSt->mac.octet[2] << 8;

        pFdbSmi[2] |= pLutSt->mac.octet[1];
        pFdbSmi[2] |= pLutSt->mac.octet[0] << 8;

        pFdbSmi[3] |= pLutSt->cvid_fid;
        pFdbSmi[3] |= (pLutSt->l3lookup & 0x0001) << 12;
        pFdbSmi[3] |= (pLutSt->ivl_svl & 0x0001) << 13;
        pFdbSmi[3] |= ((pLutSt->spa & 0x0008) >> 3) << 15;

        pFdbSmi[4] |= pLutSt->efid;
        pFdbSmi[4] |= (pLutSt->fid & 0x000F) << 3;
        pFdbSmi[4] |= (pLutSt->sa_en & 0x0001) << 7;
        pFdbSmi[4] |= (pLutSt->spa & 0x0007) << 8;
        pFdbSmi[4] |= (pLutSt->age & 0x0007) << 11;
        pFdbSmi[4] |= (pLutSt->auth & 0x0001) << 14;
        pFdbSmi[4] |= (pLutSt->sa_block & 0x0001) << 15;

        pFdbSmi[5] |= pLutSt->da_block;
        pFdbSmi[5] |= (pLutSt->lut_pri & 0x0007) << 1;
        pFdbSmi[5] |= (pLutSt->fwd_en & 0x0001) << 4;
        pFdbSmi[5] |= (pLutSt->nosalearn & 0x0001) << 5;
    }
}

ret_t rtl8367c_getAsicL2LookupTb(rtk_uint32 method, rtl8367c_luttb *pL2Table)
{
    ret_t retVal;
    rtk_uint32 regData;
    rtk_uint16* accessPtr;
    rtk_uint32 i;
    rtk_uint16 smil2Table[RTL8367C_LUT_TABLE_SIZE];
    rtk_uint32 busyCounter;
    rtk_uint32 tblCmd;

	if(pL2Table->wait_time == 0)
		busyCounter = RTL8367C_LUT_BUSY_CHECK_NO;
    else
		busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
       retVal = rtl8367c_getAsicRegBit(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,        &regData);
       if(retVal != RT_ERR_OK)
          return retVal;

	   pL2Table->lookup_busy = regData;
       if(!pL2Table->lookup_busy)
          break;

       busyCounter --;
       if(busyCounter == 0)
          return RT_ERR_BUSYWAIT_TIMEOUT;
    }


    tblCmd = (method << RTL8367C_ACCESS_METHOD_OFFSET) & RTL8367C_ACCESS_METHOD_MASK;

    switch(method)
    {
       case LUTREADMETHOD_ADDRESS:
       case LUTREADMETHOD_NEXT_ADDRESS:
       case LUTREADMETHOD_NEXT_L2UC:
       case LUTREADMETHOD_NEXT_L2MC:
       case LUTREADMETHOD_NEXT_L3MC:
       case LUTREADMETHOD_NEXT_L2L3MC:
          retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_ADDR_REG, pL2Table->address);
          if(retVal != RT_ERR_OK)
             return retVal;
          break;
       case LUTREADMETHOD_MAC:
          memset(smil2Table, 0x00, sizeof(rtk_uint16) * RTL8367C_LUT_TABLE_SIZE);
          _rtl8367c_fdbStUser2Smi(pL2Table, smil2Table);

          accessPtr = smil2Table;
          regData = *accessPtr;
          for(i=0; i<RTL8367C_LUT_ENTRY_SIZE; i++)
          {
             retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_WRDATA_BASE + i, regData);
             if(retVal != RT_ERR_OK)
                return retVal;

             accessPtr ++;
             regData = *accessPtr;

          }
          break;
       case LUTREADMETHOD_NEXT_L2UCSPA:
          retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_ADDR_REG, pL2Table->address);
          if(retVal != RT_ERR_OK)
             return retVal;

          tblCmd = tblCmd | ((pL2Table->spa << RTL8367C_TABLE_ACCESS_CTRL_SPA_OFFSET) &                                  RTL8367C_TABLE_ACCESS_CTRL_SPA_MASK);

          break;
       default:
          return RT_ERR_INPUT;
    }

    tblCmd = tblCmd | ((RTL8367C_TABLE_ACCESS_REG_DATA(TB_OP_READ,TB_TARGET_L2)) & (RTL8367C_TABLE_TYPE_MASK  |            RTL8367C_COMMAND_TYPE_MASK));
    /* Read Command */
    retVal = rtl8367c_setAsicReg(RTL8367C_TABLE_ACCESS_CTRL_REG, tblCmd);
    if(retVal != RT_ERR_OK)
       return retVal;

    if(pL2Table->wait_time == 0)
       busyCounter = RTL8367C_LUT_BUSY_CHECK_NO;
    else
       busyCounter = pL2Table->wait_time;

    while(busyCounter)
    {
       retVal = rtl8367c_getAsicRegBit(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_TABLE_LUT_ADDR_BUSY_FLAG_OFFSET,        &regData);
       if(retVal != RT_ERR_OK)
          return retVal;

       pL2Table->lookup_busy = regData;
       if(!pL2Table->lookup_busy)
          break;

       busyCounter --;
       if(busyCounter == 0)
          return RT_ERR_BUSYWAIT_TIMEOUT;
    }

    retVal = rtl8367c_getAsicRegBit(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_HIT_STATUS_OFFSET,&regData);
    if(retVal != RT_ERR_OK)
          return retVal;
    pL2Table->lookup_hit = regData;
    if(!pL2Table->lookup_hit)
       return RT_ERR_L2_ENTRY_NOTFOUND;

    /*Read access address*/
    //retVal = rtl8367c_getAsicRegBits(RTL8367C_TABLE_ACCESS_STATUS_REG, RTL8367C_TABLE_LUT_ADDR_TYPE_MASK |               RTL8367C_TABLE_LUT_ADDR_ADDRESS_MASK,&regData);
    retVal = rtl8367c_getAsicReg(RTL8367C_TABLE_ACCESS_STATUS_REG, &regData);
    if(retVal != RT_ERR_OK)
       return retVal;

    pL2Table->address = (regData & 0x7ff) | ((regData & 0x4000) >> 3) | ((regData & 0x800) << 1);

    /*read L2 entry */
    memset(smil2Table, 0x00, sizeof(rtk_uint16) * RTL8367C_LUT_TABLE_SIZE);

    accessPtr = smil2Table;

    for(i = 0; i < RTL8367C_LUT_ENTRY_SIZE; i++)
    {
       retVal = rtl8367c_getAsicReg(RTL8367C_TABLE_ACCESS_RDDATA_BASE + i, &regData);
       if(retVal != RT_ERR_OK)
          return retVal;

       *accessPtr = regData;

       accessPtr ++;
    }

    _rtl8367c_fdbStSmi2User(pL2Table, smil2Table);

    return RT_ERR_OK;
}

rtk_uint32 rtk_switch_maxLutAddrNumber_get(void)
{
	if(init_state != INIT_COMPLETED)
		return 0x00;

	return (halCtrl->max_lut_addr_num);
}

static rtk_api_ret_t _rtk_l2_addr_next_get(rtk_l2_read_method_t read_method, rtk_port_t port, rtk_uint32 *pAddress,        rtk_l2_ucastAddr_t *pL2_data)
{
	rtk_api_ret_t   retVal;
	rtk_uint32      method;
	rtl8367c_luttb  l2Table;

	/* Check initialization state */
	RTK_CHK_INIT_STATE();

	/* Error Checking */
	if ((pL2_data == NULL) || (pAddress == NULL))
		return RT_ERR_MAC;

	if(read_method == READMETHOD_NEXT_L2UC)
		method = LUTREADMETHOD_NEXT_L2UC;
	else if(read_method == READMETHOD_NEXT_L2UCSPA)
		method = LUTREADMETHOD_NEXT_L2UCSPA;
	else
		return RT_ERR_INPUT;

	if(read_method == READMETHOD_NEXT_L2UCSPA)
	{
		/* Check Port Valid */
		RTK_CHK_PORT_VALID(port);
	}

	if(*pAddress > RTK_MAX_LUT_ADDR_ID )
		return RT_ERR_L2_L2UNI_PARAM;

	memset(&l2Table, 0, sizeof(rtl8367c_luttb));
	l2Table.address = *pAddress;

	if(read_method == READMETHOD_NEXT_L2UCSPA)
		l2Table.spa = rtk_switch_port_L2P_get(port);

	if ((retVal = rtl8367c_getAsicL2LookupTb(method, &l2Table)) != RT_ERR_OK)
		return retVal;

	if(l2Table.address < *pAddress)
		return RT_ERR_L2_ENTRY_NOTFOUND;

	memcpy(pL2_data->mac.octet, l2Table.mac.octet, ETHER_ADDR_LEN);
	pL2_data->port      = rtk_switch_port_P2L_get(l2Table.spa);
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

	return RT_ERR_OK;

}

rtk_api_ret_t rtk_l2_addr_next_get(rtk_l2_read_method_t read_method, rtk_port_t port, rtk_uint32 *pAddress,             rtk_l2_ucastAddr_t *pL2_data)
{
    rtk_api_ret_t retVal;

    RTK_API_LOCK();
    retVal = _rtk_l2_addr_next_get(read_method, port, pAddress, pL2_data);
    RTK_API_UNLOCK();

    return retVal;
}
