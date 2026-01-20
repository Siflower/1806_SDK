#ifndef _SF_RTK_API_H_
#define _SF_RTK_API_H_

//rtk 8367d regs
#define    MODE_EXT_RGMII    1
#define    PORT_SPEED_1000M    2
#define    RTL8367D_REG_CHIP_DEBUG0    0x1303
#define    RTL8367D_SEL33_EXT1_OFFSET    9
#define    RTL8367D_DRI_EXT1_RG_OFFSET    6
#define    RTL8367D_DRI_EXT1_OFFSET    4
#define    RTL8367D_SLR_EXT1_OFFSET    1
#define    RTL8367D_REG_CHIP_DEBUG1    0x1304
#define    RTL8367D_RG1_DN_MASK    0x7000
#define    RTL8367D_RG1_DP_MASK    0x700
#define    RTL8367D_REG_TOP_CON0    0x1d70
#define    RTL8367D_MAC4_SEL_EXT1_MASK    0x1000
#define    RTL8367D_MAC7_SEL_EXT1_OFFSET    13
#define    RTL8367D_REG_DIGITAL_INTERFACE_SELECT    0x1305
#define    RTL8367D_SELECT_GMII_1_MASK    0xF0
#define    RTL8367D_REG_SDS1_MISC0    0x1d78
#define    RTL8367D_SDS1_MODE_MASK    0x1F
#define    RTL8367D_PORT_SDS_MODE_DISABLE      0x1f
#define    RTL8367D_REG_MAC0_FORCE_SELECT    0x12c0
#define    RTL8367D_REG_MAC0_FORCE_SELECT_EN    0x12c8
#define    RTL8367D_MAC0_FORCE_SELECT_LINK_ABLTY_OFFSET    4
#define    RTL8367D_REG_BYPASS_LINE_RATE    0x03f7

//rtk 8367 regs
#define    RTL8367C_REG_BYPASS_LINE_RATE            0x03f7
#define    RTL8367C_REG_SDS_MISC                    0x1d11
#define    RTL8367C_REG_DIGITAL_INTERFACE_SELECT    0x1305
#define    RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1  0x13c3
#define    RTL8367C_REG_REG_TO_ECO4                 0x1d41
#define    RTL8367C_REG_DIGITAL_INTERFACE0_FORCE    0x1310
#define    RTL8367C_REG_DIGITAL_INTERFACE2_FORCE    0x13c4
#define    RTL8367C_PHY_BASE						0x2000
#define    RTL8367C_PHY_OFFSET						5
#define    RTL8367C_REG_GPHY_OCP_MSB_0				0x1d15
#define    RTL8367C_CFG_CPU_OCPADR_MSB_MASK			0xFC0

#define    RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET       6
#define    RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET      11
#define    RTL8367C_SELECT_GMII_0_MASK              0xF
#define    RTL8367C_SELECT_GMII_2_MASK    			0xF
#define    RTL8367C_SELECT_GMII_1_OFFSET            4
#define    RTL8367C_CFG_SGMII_FDUP_OFFSET           10
#define    RTL8367C_CFG_SGMII_SPD_MASK              0x180
#define    RTL8367C_CFG_SGMII_LINK_OFFSET           9
#define    RTL8367C_CFG_SGMII_TXFC_OFFSET           13
#define    RTL8367C_CFG_SGMII_RXFC_OFFSET           14

#define	   INTEL_PHY_PORT_NUM  5
#define	   CLE_VIRTUAL_REG_SIZE        0x10000
#define    RTL8367C_REG_MIB_COUNTER0    0x1000
#define    RTL8367C_REG_MIB_ADDRESS    0x1004

#define    RTL8367C_REG_VLAN_CTRL    0x07a8
#define    RTL8367C_VLAN_CTRL_OFFSET    0

#define    RTL8367C_REG_EXT1_RGMXF    0x1307
#define    RTL8367C_REG_EXT2_RGMXF    0x13c5

#define    RTL8367C_REG_PHY_AD    0x130f
#define    RTL8367C_PDNPHY_OFFSET	5

#define    RTK_SWITCH_PORT_NUM (32)

#define SFA18_RTK8367_GMAC_TX_DELAY	0x25
#define SFA18_RTK8367_GMAC_RX_DELAY	0x25

#define    RTK_PORTMASK_CLEAR(__portmask__)                    ((__portmask__).bits[0] = 0)
#define    RTK_PORTMASK_PORT_SET(__portmask__, __port__)       ((__portmask__).bits[0] |= (0x00000001 << __port__))
 #define RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__)    (((__portmask__).bits[0] &       (0x00000001 << __port__)) ? 1 : 0)
#define RTK_PORTMASK_SCAN(__portmask__, __port__)           for(__port__ = 0; __port__ <     RTK_SWITCH_PORT_NUM; __port__++)  if(RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__))

typedef struct rtk_portmask_s
{
    unsigned int bits[1];
} rtk_portmask_t;

#define RTL8367D_PORT_SDS_MODE_SGMII        0x2
#define RTL8367D_PORT_SDS_MODE_HSGMII       0x12
#define RTL8367D_PORT_SDS_MODE_1000X        0x4
#define RTL8367D_PORT_SDS_MODE_100FX        0x5
#define RTL8367D_PORT_SDS_MODE_1000X_100FX  0x7
#define RTL8367D_PORT_SDS_MODE_FIBER_2P5G   0x16

#define RTL8367D_EXT_PORT_SPEED_10M         0x0
#define RTL8367D_EXT_PORT_SPEED_100M        0x1
#define RTL8367D_EXT_PORT_SPEED_1000M       0x2
#define RTL8367D_EXT_PORT_SPEED_500M        0x3
#define RTL8367D_EXT_PORT_SPEED_2500M       0x5

#define    RTL8367D_MAC4_SEL_EXT1_OFFSET    12
#define    RTL8367D_EXT1_RGMII_TX_DELAY_MASK    0x38
#define    RTL8367D_MAC4_SEL_EXT1_OFFSET    12
#define    RTL8367D_REG_EXT_TXC_DLY    0x13f9

typedef struct  rtk_port_mac_ability_s
{
    int forcemode;
    int speed;
    int duplex;
    int link;
    int nway;
    int txpause;
    int rxpause;
}rtk_port_mac_ability_t;

int rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port);
int rtk_port_isolation_set(struct sgmac_priv *priv, int port, rtk_portmask_t *pPortmask);
int smi_read(struct sgmac_priv *priv, int reg_addr, int *pdata);
int smi_write(struct sgmac_priv *priv, int reg_addr, int data);
int rtl8367_setAsicReg(struct sgmac_priv *priv, int reg, int value);
int rtl8367_setAsicRegBit(struct sgmac_priv *priv, int reg, int bit, int value);
int rtl8367_setAsicRegBits(struct sgmac_priv *priv, int reg, int bits, int value);
int rtl8367_getAsicReg(struct sgmac_priv *priv, int reg, int *pValue);
int rtl8367_getAsicPHYReg(struct sgmac_priv *priv, int phyNo, int phyReg, int *phyData);
int dal_rtl8367_port_phyEnableAll_set(struct sgmac_priv *priv, int enable);
#ifdef CONFIG_SOC_SFA28_MPW0
int rtk_port_rgmiiDelayExt_set(struct sgmac_priv *priv, int port, int txDelay, int rxDelay);
#endif

#endif
