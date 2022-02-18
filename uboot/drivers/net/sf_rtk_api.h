#ifndef _SF_RTK_API_H_
#define _SF_RTK_API_H_

//rtk 8367 regs
#define    RTL8367C_REG_BYPASS_LINE_RATE            0x03f7
#define    RTL8367C_REG_SDS_MISC                    0x1d11
#define    RTL8367C_REG_DIGITAL_INTERFACE_SELECT    0x1305
#define    RTL8367C_REG_REG_TO_ECO4                 0x1d41
#define    RTL8367C_REG_DIGITAL_INTERFACE0_FORCE    0x1310
#define    RTL8367C_PHY_BASE						0x2000
#define    RTL8367C_PHY_OFFSET						5
#define    RTL8367C_REG_GPHY_OCP_MSB_0				0x1d15
#define    RTL8367C_CFG_CPU_OCPADR_MSB_MASK			0xFC0

#define    RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET       6
#define    RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET      11
#define    RTL8367C_SELECT_GMII_0_MASK              0xF
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

#define    RTK_SWITCH_PORT_NUM (32)

#define    RTK_PORTMASK_CLEAR(__portmask__)                    ((__portmask__).bits[0] = 0)
#define    RTK_PORTMASK_PORT_SET(__portmask__, __port__)       ((__portmask__).bits[0] |= (0x00000001 << __port__))
 #define RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__)    (((__portmask__).bits[0] &       (0x00000001 << __port__)) ? 1 : 0)
#define RTK_PORTMASK_SCAN(__portmask__, __port__)           for(__port__ = 0; __port__ <     RTK_SWITCH_PORT_NUM; __port__++)  if(RTK_PORTMASK_IS_PORT_SET(__portmask__, __port__))

typedef struct rtk_portmask_s
{
    unsigned int bits[1];
} rtk_portmask_t;


int rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port);
int rtk_port_isolation_set(struct sgmac_priv *priv, int port, rtk_portmask_t *pPortmask);
int smi_read(struct sgmac_priv *priv, int reg_addr, int *pdata);
int smi_write(struct sgmac_priv *priv, int reg_addr, int data);
int rtl8367c_setAsicReg(struct sgmac_priv *priv, int reg, int value);
int rtl8367c_setAsicRegBit(struct sgmac_priv *priv, int reg, int bit, int value);
int rtl8367c_setAsicRegBits(struct sgmac_priv *priv, int reg, int bits, int value);
int rtl8367c_getAsicReg(struct sgmac_priv *priv, int reg, int *pValue);
int rtl8367c_getAsicPHYReg(struct sgmac_priv *priv, int phyNo, int phyReg, int *phyData);
#ifdef CONFIG_SOC_SFA28_MPW0
int rtk_port_rgmiiDelayExt_set(struct sgmac_priv *priv, int port, int txDelay, int rxDelay);
#endif

#endif
