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
#include "port.h"
#ifdef L2SW_LINUX_KERNEL
#include "linux/kernel.h"
#endif
#include "sal_string.h"
#include "vlan.h"
#include "svlan.h"
#include "acl.h"
#include "rate.h"
#include "l2sw_asicdrv_misc.h"
#include "l2sw_asicdrv_lut.h"
#include "l2sw_asicdrv_rma.h"
#include "l2sw_asicdrv_mirror.h"
#include "l2sw_asicdrv_scheduling.h"
#include "l2sw_asicdrv_inbwctrl.h"
#include "l2sw_asicdrv_phy.h"
#include "l2sw_reg.h"
#include "smi.h"


init_state_t init_state[L2SW_MAX_NUM_OF_UNIT]={INIT_COMPLETED};

#if defined(L2SW_LINUX_KERNEL)
struct mutex api_mutex[L2SW_MAX_NUM_OF_UNIT];
#endif


static l2sw_switch_halCtrl_t l2sw_hal_Ctrl =
{
    /* Switch Chip */
    CHIP_SF2507,

    /* Logical to Physical */
    {0, 1, 2, 3, 4, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 5,
     6, 7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },

    /* Physical to Logical */
    {UTP_PORT0, UTP_PORT1, UTP_PORT2, UTP_PORT3, UTP_PORT4, INTERNAL_PORT, EXT_PORT0, EXT_PORT1,
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
    ( (0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) |
        (0x1 << INTERNAL_PORT) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1) ),

    /* Valid UTP port mask */
    ( (0x1 << UTP_PORT0) | (0x1 << UTP_PORT1) | (0x1 << UTP_PORT2) | (0x1 << UTP_PORT3) | (0x1 << UTP_PORT4) ),

    /* Valid EXT port mask */
    ((0x1 << INTERNAL_PORT) | (0x1 << EXT_PORT0) | (0x1 << EXT_PORT1)),

    /* Valid CPU port mask */
    0x00,

    /* Minimum physical port number */
    0,

    /* Maxmum physical port number */
    7,

    /* Physical port mask */
    0xFF,

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

static l2sw_switch_halCtrl_t *halCtrl = &l2sw_hal_Ctrl;

void rgmii_set(l2sw_uint8 units)
{
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x10);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x1);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1016);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1016);
    l2sw_setAsicReg(units, L2SW_REG_EXT1_RGMXF, 0x986);
    l2sw_setAsicReg(units, L2SW_REG_EXT2_RGMXF, 0x986);
    l2sw_setAsicReg(units, L2SW_REG_ECO_0_REG, 0x11);
    l2sw_setAsicReg(units, 0x3b0c, 0x7);
    l2sw_setAsicReg(units, 0x3b0d, 0x7);
}

void mii_mac_set(l2sw_uint8 units)
{
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x20);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x2);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1005);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1005);
}

void rmii_mac_set(l2sw_uint8 units)
{
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x70);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x7);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1005);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1005);
}

void mii_phy_set(l2sw_uint8 units)
{
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x30);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x3);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1005);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1005);
}

void rmii_phy_set(l2sw_uint8 units)
{
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT, 0x80);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE_SELECT_1, 0x8);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE1_FORCE, 0x1005);
    l2sw_setAsicReg(units, L2SW_REG_DIGITAL_INTERFACE2_FORCE, 0x1005);
}

void chip_reset(l2sw_uint8 units)
{
    l2sw_switch_ahb_write(units, 0x90000800, 0xffe0cff);
    l2sw_switch_ahb_write(units, 0x90000804, 0xffe0cff);
    l2sw_switch_ahb_write(units, 0x90000020, 0x400);
}

static void _l2sw_switch_tx_fifo(int units)
{
    int port;

    for(port = 0; port < 5; port++)
    {
        l2sw_port_phyReg_set(units, port, 31, 0x2);
        l2sw_port_phyReg_set(units, port, 20, 0x0492);
        l2sw_port_phyReg_set(units, port, 31, 0x0);
    }

}

l2sw_uint32 l2sw_switch_reset_init(l2sw_uint8 units, l2sw_mode_ext_t mode)
{
    l2sw_uint32 data;
    l2sw_uint32 index;

    while(1)
    {
        while(1)
        {
            l2sw_setAsicReg(units, 0x1601, 0x123);

            l2sw_getAsicReg(units, 0x1601, &data);
            PRINT("0x1601:0x%x  before reset\n", data);

            //chip soft reset
            chip_reset(units);

            DELAY_MS(50);
            l2sw_getAsicReg(units, 0x1601, &data);
            l2sw_getAsicReg(units, 0x1601, &data);

            if(data == 0xbfff)
            {
                PRINT("chip reset ok\n");
                break;
            }
            else
                PRINT("0x1601:0x%x  chip reset is not ok\n", data);
        }

		mii_phy_set(units);

        //cpu get pkt init
        l2sw_setAsicReg(units, L2SW_REG_RX_ADDR0, 0x7000);
        l2sw_setAsicReg(units, L2SW_REG_RX_ADDR1, 0x3000);
        l2sw_setAsicReg(units, L2SW_REG_RX_ADDR_RENEW, 0x1);

        data = 0x1fff;
        //release phy mac
        for(index = 12; index > 0; index--)
        {
            DELAY_MS(50);
            data &= (~(1<<index));
            l2sw_setAsicReg(units, L2SW_REG_GEPHY_MAC_SOFT_RST, data);
        }

        //to solve link fail in AC-coupling mode.
        _l2sw_switch_tx_fifo(units);

        for(index = 1; index <= 12; index++)
        {
            DELAY_MS(50);
            data |= (1<<index);
            l2sw_setAsicReg(units, L2SW_REG_GEPHY_MAC_SOFT_RST, data);
        }

        DELAY_MS(50);
        DELAY_MS(3000);

        //check reset works or not
        l2sw_getAsicReg(units, L2SW_REG_DICE_INT1_GROUP2, &data);
        PRINT("L2SW_REG_DICE_INT1_GROUP2:0x%x\n", data);
        if(data == 0)
        {
            break;
        }
        else if(data == 0xffff)
        {
            PRINT("driver is not ok\n");
            break;
        }
    }

    //set port 6.7 mode
    if(mode == MODE_EXT_RGMII)
        rgmii_set(units);
    else if(mode == MODE_EXT_RMII_MAC)
        rmii_mac_set(units);
    else if(mode == MODE_EXT_MII_MAC)
        mii_mac_set(units);
    else if(mode == MODE_EXT_MII_PHY)
        mii_phy_set(units);
    else if(mode == MODE_EXT_RMII_PHY)
        rmii_phy_set(units);
    else
        rgmii_set(units);

    PRINT("ok\n");
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_init_2507(l2sw_uint8 units)
{
    l2sw_port_t port;
    l2sw_uint32 retVal;
    l2sw_uint32 regData;
    l2sw_uint32 regValue;

    if( (retVal = l2sw_setAsicReg(units, 0x13c2, 0x0249)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_getAsicReg(units, 0x1301, &regValue)) != L2SW_ERR_OK)
        return retVal;

    if( (retVal = l2sw_setAsicReg(units, 0x13c2, 0x0000)) != L2SW_ERR_OK)
        return retVal;

    L2SW_SCAN_ALL_LOG_PORT(units, port)
    {
         if(l2sw_switch_isUtpPort(units, port) == L2SW_ERR_OK)
         {
             if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_PORT0_EEECFG + (0x20 * port), L2SW_PORT0_EEECFG_EEE_100M_OFFSET, 1)) != L2SW_ERR_OK)
                 return retVal;

             if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_PORT0_EEECFG + (0x20 * port), L2SW_PORT0_EEECFG_EEE_GIGA_500M_OFFSET, 1)) != L2SW_ERR_OK)
                 return retVal;

             if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_PORT0_EEECFG + (0x20 * port), L2SW_PORT0_EEECFG_EEE_TX_OFFSET, 1)) != L2SW_ERR_OK)
                 return retVal;

             if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_PORT0_EEECFG + (0x20 * port), L2SW_PORT0_EEECFG_EEE_RX_OFFSET, 1)) != L2SW_ERR_OK)
                 return retVal;

             if((retVal = l2sw_getAsicPHYOCPReg(units, port, 0xA428, &regData)) != L2SW_ERR_OK)
                return retVal;

             regData &= ~(0x0200);
             if((retVal = l2sw_setAsicPHYOCPReg(units, port, 0xA428, regData)) != L2SW_ERR_OK)
                 return retVal;

             if((regValue & 0x00F0) == 0x00A0)
             {
                 if((retVal = l2sw_getAsicPHYOCPReg(units, port, 0xA5D0, &regData)) != L2SW_ERR_OK)
                     return retVal;

                 regData |= 0x0006;
                 if((retVal = l2sw_setAsicPHYOCPReg(units, port, 0xA5D0, regData)) != L2SW_ERR_OK)
                     return retVal;
             }
         }
    }

    if((retVal = l2sw_setAsicReg(units, L2SW_REG_UTP_FIB_DET, 0x15BB)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1303, 0x06D6)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1304, 0x0700)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x13E2, 0x003F)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x13F9, 0x0090)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x121e, 0x03CA)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1233, 0x0352)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1237, 0x00a0)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x123a, 0x0030)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1239, 0x0084)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x0301, 0x1000)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x1349, 0x001F)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBit(units, 0x18e0, 0, 0)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBit(units, 0x122b, 14, 1)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBits(units, 0x1305, 0xC000, 3)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_init(l2sw_uint8 units)
{
    l2sw_uint32  retVal;
    l2sw_rma_t rmaCfg;
    switch_chip_t   switchChip;
    l2sw_uint32 index=0;
    l2sw_uint32 data;
	l2sw_uint8 eco;

#if defined(L2SW_LINUX_KERNEL)
    mutex_init(&api_mutex[units]);
#else
	fun_init();

    //user needs to modify implementations of this function according to the different platforms
    fun_regist();
#endif

	/* Set initial state */
    if((retVal = l2sw_switch_initialState_set(units, INIT_COMPLETED)) != L2SW_ERR_OK)
        return retVal;

    l2sw_getAsicReg(units, 0x13c3, &data);
    l2sw_getAsicReg(units, 0x13c3, &data);
    if(data == 0)
        eco = 0;
    else
        eco = 1;

    if(eco == 0)
        l2sw_switch_reset_init(units, MODE_EXT_RGMII);
    else if(eco == 1)
    {
        rgmii_set(units);
        l2sw_setAsicReg(units, 0x3e04, 0x1);

        data = 0x1fff;
        for(index = 12; index > 0; index--)
        {
            DELAY_MS(50);
            data &= (~(1<<index));
            l2sw_setAsicReg(units, L2SW_REG_GEPHY_MAC_SOFT_RST, data);
        }
        //to solve link fail in AC-coupling mode.
        _l2sw_switch_tx_fifo(units);

        for(index = 1; index <= 12; index++)
        {
            DELAY_MS(50);
            data |= (1<<index);
            l2sw_setAsicReg(units, L2SW_REG_GEPHY_MAC_SOFT_RST, data);
        }
    }

    /*module init*/
    l2sw_vlan_init(units);
    l2sw_svlan_init(units);
    l2sw_filter_igrAcl_init(units);

    /* probe switch */
    if((retVal = l2sw_switch_probe(units, &switchChip)) != L2SW_ERR_OK)
        return retVal;

    /* Initial */
    switch(switchChip)
    {
        case CHIP_SF2507:
            if((retVal = _l2sw_switch_init_2507(units)) != L2SW_ERR_OK)
                return retVal;
            break;
        default:
            return L2SW_ERR_CHIP_NOT_FOUND;
    }

    /* Set flowcontrol drop on threshold */
    for(index = 0; index <= 7; index++)
    {
        if((retVal = l2sw_setAsicRegBits(units, L2SW_FLOWCTRL_PORT_DROP_ON_REG(index), L2SW_FLOWCTRL_PORT0_DROP_ON_MASK, 0x250)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Set Old max packet length to 16K */
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_MAX_LENGTH_LIMINT_IPG, L2SW_MAX_LENTH_CTRL_MASK, 3)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_MAX_LEN_RX_TX, L2SW_MAX_LEN_RX_TX_MASK, 3)) != L2SW_ERR_OK)
        return retVal;

    /* ACL Mode */
    if((retVal = l2sw_setAsicRegBits(units, L2SW_REG_ACL_ACCESS_MODE, L2SW_ACL_ACCESS_MODE_MASK, 1)) != L2SW_ERR_OK)
        return retVal;

    /* Max rate */
    if((retVal = l2sw_setAsicPortIngressBandwidth(units, l2sw_switch_port_L2P_get(units, halCtrl->hsg_logical_port), L2SW_QOS_RATE_INPUT_MAX_HSG>>3, L2SW_DISABLED, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPortEgressRate(units, l2sw_switch_port_L2P_get(units, halCtrl->hsg_logical_port), L2SW_QOS_RATE_INPUT_MAX_HSG>>3)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPortEgressRateIfg(units, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x03fa, 0x0007)) != L2SW_ERR_OK)
        return retVal;

    /* Change unknown DA to per port setting */
    if((retVal = l2sw_setAsicRegBits(units, L2SW_PORT_SECURIT_CTRL_REG, L2SW_UNKNOWN_UNICAST_DA_BEHAVE_MASK, 3)) != L2SW_ERR_OK)
        return retVal;

    /* LUT lookup OP = 1 */
    if ((retVal = l2sw_setAsicLutIpLookupMethod(units, 1))!=L2SW_ERR_OK)
        return retVal;

    /* Set RMA */
    rmaCfg.portiso_leaky = 0;
    rmaCfg.vlan_leaky = 0;
    rmaCfg.keep_format = 0;
    rmaCfg.trap_priority = 0;
    rmaCfg.discard_storm_filter = 0;
    rmaCfg.operation = 0;
    if ((retVal = l2sw_setAsicRma(units, 2, &rmaCfg))!=L2SW_ERR_OK)
        return retVal;

    /* Enable TX Mirror isolation leaky */
    if ((retVal = l2sw_setAsicPortMirrorIsolationTxLeaky(units, L2SW_ENABLED)) != L2SW_ERR_OK)
        return retVal;

    /* INT EN */
    if((retVal = l2sw_setAsicRegBit(units, L2SW_REG_IO_MISC_FUNC, L2SW_INT_EN_OFFSET, 1)) != L2SW_ERR_OK)
        return retVal;

    /*clear cb table counter*/
    /*IQ table*/
    for(index=0;index<=31;index++){
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0 >> 4)+index) << 4))) != L2SW_ERR_OK)
            return retVal;
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0 >> 4)+index) << 4)| 0x2)) != L2SW_ERR_OK)
            return retVal;
    }
    /*OQ table*/
    for(index=0;index<=15;index++){
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0x2000 >> 4)+index) << 4))) != L2SW_ERR_OK)
            return retVal;
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0x2000 >> 4)+index) << 4)| 0x2)) != L2SW_ERR_OK)
            return retVal;
    }
    /*DROP table*/
    for(index=0;index<=31;index++){
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0x4000 >> 4)+index) << 4))) != L2SW_ERR_OK)
            return retVal;
        if((retVal = l2sw_setAsicRegBits(units, 0x3208,0xffff, (((0x4000 >> 4)+index) << 4)| 0x2)) != L2SW_ERR_OK)
            return retVal;
    }


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_portMaxPktLen_set(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 cfgId)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(speed >= MAXPKTLEN_LINK_SPEED_END)
        return L2SW_ERR_INPUT;

    if(cfgId > MAXPKTLEN_CFG_ID_MAX)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_setAsicMaxLength(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)speed, cfgId)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_portMaxPktLen_get(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 *pCfgId)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if(speed >= MAXPKTLEN_LINK_SPEED_END)
        return L2SW_ERR_INPUT;

    if(NULL == pCfgId)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicMaxLength(units, l2sw_switch_port_L2P_get(units, port), (l2sw_uint32)speed, pCfgId)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_maxPktLenCfg_set(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 pktLen)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(cfgId > MAXPKTLEN_CFG_ID_MAX)
        return L2SW_ERR_INPUT;

    if(pktLen > L2SW_SWITCH_MAX_PKTLEN)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_setAsicMaxLengthCfg(units, cfgId, pktLen)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_switch_maxPktLenCfg_get(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 *pPktLen)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(cfgId > MAXPKTLEN_CFG_ID_MAX)
        return L2SW_ERR_INPUT;

    if(NULL == pPktLen)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicMaxLengthCfg(units, cfgId, pPktLen)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_probe
 * Description:
 *      Probe switch
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Switch probed
 *      L2SW_ERR_FAILED   - Switch Unprobed.
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_probe(l2sw_uint8 units, switch_chip_t *pSwitchChip)
{

    l2sw_uint32 retVal;
    l2sw_uint32 data, regValue;

    if((retVal = l2sw_setAsicReg(units, 0x13C2, 0x0249)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, 0x1300, &data)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, 0x1301, &regValue)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicReg(units, 0x13C2, 0x0000)) != L2SW_ERR_OK)
        return retVal;

    switch (data)
    {
        case 0x1619:
        default:
            *pSwitchChip = CHIP_SF2507;
            return L2SW_ERR_OK;
    }
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_initialState_set
 * Description:
 *      Set initial status
 * Input:
 *      state   - Initial state;
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Initialized
 *      L2SW_ERR_FAILED   - Uninitialized
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_initialState_set(l2sw_uint8 units, init_state_t state)
{
    if(state >= INIT_STATE_END)
        return L2SW_ERR_FAILED;

    init_state[units] = state;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_initialState_get
 * Description:
 *      Get initial status
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      INIT_COMPLETED     - Initialized
 *      INIT_NOT_COMPLETED - Uninitialized
 * Note:
 *
 */
init_state_t l2sw_switch_initialState_get(l2sw_uint8 units)
{
    return init_state[units];
}

/* Function Name:
 *      l2sw_switch_logicalPortCheck
 * Description:
 *      Check logical port ID.
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is correct
 *      L2SW_ERR_FAILED   - Port ID is not correct
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_logicalPortCheck(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(halCtrl->l2p_port[logicalPort] == 0xFF)
        return L2SW_ERR_FAILED;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_isUtpPort
 * Description:
 *      Check is logical port a UTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a UTP port
 *      L2SW_ERR_FAILED   - Port ID is not a UTP port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isUtpPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(halCtrl->log_port_type[logicalPort] == UTP_PORT)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_isExtPort
 * Description:
 *      Check is logical port a Extension port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a EXT port
 *      L2SW_ERR_FAILED   - Port ID is not a EXT port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isExtPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(halCtrl->log_port_type[logicalPort] == EXT_PORT)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}


/* Function Name:
 *      l2sw_switch_isHsgPort
 * Description:
 *      Check is logical port a HSG port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a HSG port
 *      L2SW_ERR_FAILED   - Port ID is not a HSG port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isHsgPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(logicalPort == halCtrl->hsg_logical_port)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_isSgmiiPort
 * Description:
 *      Check is logical port a SGMII port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a SGMII port
 *      L2SW_ERR_FAILED   - Port ID is not a SGMII port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isSgmiiPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if( ((0x01 << logicalPort) & halCtrl->sg_logical_portmask) != 0)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_isCPUPort
 * Description:
 *      Check is logical port a CPU port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a CPU port
 *      L2SW_ERR_FAILED   - Port ID is not a CPU port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isCPUPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if( ((0x01 << logicalPort) & halCtrl->valid_cpu_portmask) != 0)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_isComboPort
 * Description:
 *      Check is logical port a Combo port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a combo port
 *      L2SW_ERR_FAILED   - Port ID is not a combo port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isComboPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(halCtrl->combo_logical_port == logicalPort)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_ComboPort_get
 * Description:
 *      Get Combo port ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      Port ID of combo port
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_ComboPort_get(l2sw_uint8 units)
{
    return halCtrl->combo_logical_port;
}

/* Function Name:
 *      l2sw_switch_isPtpPort
 * Description:
 *      Check is logical port a PTP port
 * Input:
 *      logicalPort     - logical port ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK       - Port ID is a PTP port
 *      L2SW_ERR_FAILED   - Port ID is not a PTP port
 *      L2SW_ERR_NOT_INIT - Not Initialize
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isPtpPort(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return L2SW_ERR_FAILED;

    if(halCtrl->ptp_port[logicalPort] == 1)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_FAILED;
}

/* Function Name:
 *      l2sw_switch_port_L2P_get
 * Description:
 *      Get physical port ID
 * Input:
 *      logicalPort       - logical port ID
 * Output:
 *      None
 * Return:
 *      Physical port ID
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_port_L2P_get(l2sw_uint8 units, l2sw_port_t logicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return UNDEFINE_PHY_PORT;

    if(logicalPort >= L2SW_SWITCH_PORT_NUM)
        return UNDEFINE_PHY_PORT;

    return (halCtrl->l2p_port[logicalPort]);
}

/* Function Name:
 *      l2sw_switch_port_P2L_get
 * Description:
 *      Get logical port ID
 * Input:
 *      physicalPort       - physical port ID
 * Output:
 *      None
 * Return:
 *      logical port ID
 * Note:
 *
 */
l2sw_port_t l2sw_switch_port_P2L_get(l2sw_uint8 units, l2sw_uint32 physicalPort)
{
    if(init_state[units] != INIT_COMPLETED)
        return UNDEFINE_PORT;

    if(physicalPort >= L2SW_SWITCH_PORT_NUM)
        return UNDEFINE_PORT;

    return (halCtrl->p2l_port[physicalPort]);
}

/* Function Name:
 *      l2sw_switch_isPortMaskValid
 * Description:
 *      Check portmask is valid or not
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - port mask is valid
 *      L2SW_ERR_FAILED       - port mask is not valid
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isPortMaskValid(l2sw_uint8 units, l2sw_portmask_t *pPmask)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == pPmask)
        return L2SW_ERR_NULL_POINTER;

    if( (pPmask->bits[0] | halCtrl->valid_portmask) != halCtrl->valid_portmask )
        return L2SW_ERR_FAILED;
    else
        return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_isPortMaskUtp
 * Description:
 *      Check all ports in portmask are only UTP port
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Only UTP port in port mask
 *      L2SW_ERR_FAILED       - Not only UTP port in port mask
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isPortMaskUtp(l2sw_uint8 units, l2sw_portmask_t *pPmask)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == pPmask)
        return L2SW_ERR_NULL_POINTER;

    if( (pPmask->bits[0] | halCtrl->valid_utp_portmask) != halCtrl->valid_utp_portmask )
        return L2SW_ERR_FAILED;
    else
        return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_isPortMaskExt
 * Description:
 *      Check all ports in portmask are only EXT port
 * Input:
 *      pPmask       - logical port mask
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Only EXT port in port mask
 *      L2SW_ERR_FAILED       - Not only EXT port in port mask
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_isPortMaskExt(l2sw_uint8 units, l2sw_portmask_t *pPmask)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == pPmask)
        return L2SW_ERR_NULL_POINTER;

    if( (pPmask->bits[0] | halCtrl->valid_ext_portmask) != halCtrl->valid_ext_portmask )
        return L2SW_ERR_FAILED;
    else
        return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_portmask_L2P_get
 * Description:
 *      Get physicl portmask from logical portmask
 * Input:
 *      pLogicalPmask       - logical port mask
 * Output:
 *      pPhysicalPortmask   - physical port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 *      L2SW_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_portmask_L2P_get(l2sw_uint8 units, l2sw_portmask_t *pLogicalPmask, l2sw_uint32 *pPhysicalPortmask)
{
    l2sw_uint32 log_port, phy_port;

    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == pLogicalPmask)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pPhysicalPortmask)
        return L2SW_ERR_NULL_POINTER;

    if(l2sw_switch_isPortMaskValid(units, pLogicalPmask) != L2SW_ERR_OK)
        return L2SW_ERR_PORT_MASK;

    /* reset physical port mask */
    *pPhysicalPortmask = 0;

    L2SW_PORTMASK_SCAN((*pLogicalPmask), log_port)
    {
        phy_port = l2sw_switch_port_L2P_get(units, (l2sw_port_t)log_port);
        *pPhysicalPortmask |= (0x0001 << phy_port);
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_portmask_P2L_get
 * Description:
 *      Get logical portmask from physical portmask
 * Input:
 *      physicalPortmask    - physical port mask
 * Output:
 *      pLogicalPmask       - logical port mask
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 *      L2SW_ERR_PORT_MASK    - Error port mask
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_portmask_P2L_get(l2sw_uint8 units, l2sw_uint32 physicalPortmask, l2sw_portmask_t *pLogicalPmask)
{
    l2sw_uint32 log_port, phy_port;

    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == pLogicalPmask)
        return L2SW_ERR_NULL_POINTER;

    L2SW_PORTMASK_CLEAR(*pLogicalPmask);

    for(phy_port = halCtrl->min_phy_port; phy_port <= halCtrl->max_phy_port; phy_port++)
    {
        if(physicalPortmask & (0x0001 << phy_port))
        {
            log_port = l2sw_switch_port_P2L_get(units, phy_port);
            if(log_port != UNDEFINE_PORT)
            {
                L2SW_PORTMASK_PORT_SET(*pLogicalPmask, log_port);
            }
        }
    }

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_phyPortMask_get
 * Description:
 *      Get physical portmask
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Physical port mask
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_phyPortMask_get(l2sw_uint8 units)
{
    if(init_state[units] != INIT_COMPLETED)
        return 0x00; /* No port in portmask */

    return (halCtrl->phy_portmask);
}

/* Function Name:
 *      l2sw_switch_logPortMask_get
 * Description:
 *      Get Logical portmask
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_NOT_INIT     - Not Initialize
 *      L2SW_ERR_NULL_POINTER - Null pointer
 * Note:
 *
 */
l2sw_api_ret_t l2sw_switch_logPortMask_get(l2sw_uint8 units, l2sw_portmask_t *pPortmask)
{
    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_FAILED;

    if(NULL == pPortmask)
        return L2SW_ERR_NULL_POINTER;

    pPortmask->bits[0] = halCtrl->valid_portmask;
    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_init
 * Description:
 *      Set chip to default configuration enviroment
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      The API can set chip registers to default configuration for different release chip model.
 */
l2sw_api_ret_t l2sw_switch_init(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    retVal = _l2sw_switch_init(units);

    return retVal;
}

/* Function Name:
 *      l2sw_switch_portMaxPktLen_set
 * Description:
 *      Set Max packet length
 * Input:
 *      port    - Port ID
 *      speed   - Speed
 *      cfgId   - Configuration ID
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
l2sw_api_ret_t l2sw_switch_portMaxPktLen_set(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 cfgId)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_switch_portMaxPktLen_set(units, port, speed, cfgId);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_switch_portMaxPktLen_get
 * Description:
 *      Get Max packet length
 * Input:
 *      port    - Port ID
 *      speed   - Speed
 * Output:
 *      pCfgId  - Configuration ID
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
l2sw_api_ret_t l2sw_switch_portMaxPktLen_get(l2sw_uint8 units, l2sw_port_t port, l2sw_switch_maxPktLen_linkSpeed_t speed, l2sw_uint32 *pCfgId)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_switch_portMaxPktLen_get(units, port, speed, pCfgId);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_switch_maxPktLenCfg_set
 * Description:
 *      Set Max packet length configuration
 * Input:
 *      cfgId   - Configuration ID
 *      pktLen  - Max packet length
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
l2sw_api_ret_t l2sw_switch_maxPktLenCfg_set(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 pktLen)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_switch_maxPktLenCfg_set(units, cfgId, pktLen);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_switch_maxPktLenCfg_get
 * Description:
 *      Get Max packet length configuration
 * Input:
 *      cfgId   - Configuration ID
 *      pPktLen - Max packet length
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Error Input
 * Note:
 */
l2sw_api_ret_t l2sw_switch_maxPktLenCfg_get(l2sw_uint8 units, l2sw_uint32 cfgId, l2sw_uint32 *pPktLen)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_switch_maxPktLenCfg_get(units, cfgId, pPktLen);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_switch_maxLogicalPort_get
 * Description:
 *      Get Max logical port ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      Max logical port
 * Note:
 *      This API can get max logical port
 */
l2sw_port_t l2sw_switch_maxLogicalPort_get(l2sw_uint8 units)
{
    l2sw_port_t port, maxLogicalPort = 0;

    /* Check initialization state */
    if(l2sw_switch_initialState_get(units) != INIT_COMPLETED)
    {
        return UNDEFINE_PORT;
    }

    for(port = 0; port < L2SW_SWITCH_PORT_NUM; port++)
    {
        if( (halCtrl->log_port_type[port] == UTP_PORT) || (halCtrl->log_port_type[port] == EXT_PORT) )
            maxLogicalPort = port;
    }

    return maxLogicalPort;
}

/* Function Name:
 *      l2sw_switch_maxMeterId_get
 * Description:
 *      Get Max Meter ID
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max Meter ID
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_maxMeterId_get(l2sw_uint8 units)
{
    if(init_state[units] != INIT_COMPLETED)
        return 0x00;

    return (halCtrl->max_meter_id);
}

/* Function Name:
 *      l2sw_switch_maxLutAddrNumber_get
 * Description:
 *      Get Max LUT Address number
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0x00                - Not Initialize
 *      Other value         - Max LUT Address number
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_maxLutAddrNumber_get(l2sw_uint8 units)
{
    if(init_state[units] != INIT_COMPLETED)
        return 0x00;

    return (halCtrl->max_lut_addr_num);
}

/* Function Name:
 *      l2sw_switch_isValidTrunkGrpId
 * Description:
 *      Check if trunk group is valid or not
 * Input:
 *      grpId       - Group ID
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - Trunk Group ID is valid
 *      L2SW_ERR_LA_TRUNK_ID  - Trunk Group ID is not valid
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_isValidTrunkGrpId(l2sw_uint8 units, l2sw_uint32 grpId)
{
    if(init_state[units] != INIT_COMPLETED)
        return 0x00;

    if( (halCtrl->trunk_group_mask & (0x01 << grpId)) != 0)
        return L2SW_ERR_OK;
    else
        return L2SW_ERR_LA_TRUNK_ID;

}

/* Function Name:
 *      l2sw_switch_ahb_read
 * Description:
 *      Read soc memery by apb to ahb
 * Input:
 *      addr       - soc addr
 * Output:
 *      data
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_ahb_read(l2sw_uint8 units, l2sw_uint32 addr, l2sw_uint32 *data)
{
    l2sw_int32 retVal = 0;
    l2sw_int32 data2;

    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    if(NULL == data)
        return L2SW_ERR_NULL_POINTER;

    //reg addr low
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_ADDR_L, addr&0xffff)) != L2SW_ERR_OK)
        return retVal;

    //reg addr high
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_ADDR_H, (addr&0xffff0000)>>16)) != L2SW_ERR_OK)
        return retVal;

    //write read bit and start
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_SOC_CMD, AHB_READ_BIT | AHB_START_BIT)) != L2SW_ERR_OK)
        return retVal;

    DELAY_MS(100);

    //read data
    if((retVal = l2sw_getAsicReg(units, L2SW_REG_APB_RDATA_H, data)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicReg(units, L2SW_REG_APB_RDATA_L, &data2)) != L2SW_ERR_OK)
        return retVal;

    *data = ((*data<<16) | data2);

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_switch_ahb_write
 * Description:
 *      Write soc memery by apb to ahb
 * Input:
 *      addr       - soc addr
 *      data       - Data to write
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 * Note:
 *
 */
l2sw_uint32 l2sw_switch_ahb_write(l2sw_uint8 units, l2sw_uint32 addr, l2sw_uint32 data)
{
    l2sw_int32 retVal = 0;

    if(init_state[units] != INIT_COMPLETED)
        return L2SW_ERR_NOT_INIT;

    //set data to write low
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_WDATA_L, data&0xffff)) != L2SW_ERR_OK)
        return retVal;

    //set data to write high
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_WDATA_H, (data&0xffff0000)>>16)) != L2SW_ERR_OK)
        return retVal;

    //reg addr low
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_ADDR_L, addr&0xffff)) != L2SW_ERR_OK)
        return retVal;
    //reg addr high
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_ADDR_H, (addr&0xffff0000)>>16)) != L2SW_ERR_OK)
        return retVal;

    //start
    if((retVal = l2sw_setAsicReg(units, L2SW_REG_APB_SOC_CMD, AHB_WRITE_BIT | AHB_START_BIT)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}
