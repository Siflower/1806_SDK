#include "sfa18_gmac.h"
#include "sf_rtk_api.h"


extern int sgmac_mdio_read(struct mii_dev *bus, int addr, int devad, int reg);
extern int sgmac_mdio_write(struct mii_dev *bus, int addr, int devad, int reg, u16 val);
extern int rtk_phy_id;

int smi_read(struct sgmac_priv *priv, int reg_addr, int *pdata)
{
	int data;
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 31, 0xe);
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 23, reg_addr);
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 21, 1);
	data = sgmac_mdio_read(priv->bus, rtk_phy_id, 0, 25);
	*pdata = data;

	return 0;
}

int smi_write(struct sgmac_priv *priv, int reg_addr, int data)
{
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 31, 0xe);
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 23, reg_addr);
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 24, data);
	sgmac_mdio_write(priv->bus, rtk_phy_id, 0, 21, 0x3);

	return 0;
}

int rtl8367_setAsicReg(struct sgmac_priv *priv, int reg, int value)
{
	int retVal;

	retVal = smi_write(priv, reg, value);
	if(retVal != 0)
		return -1;

	return 0;
}

int rtl8367_setAsicRegBit(struct sgmac_priv *priv, int reg, int bit, int value)
{
	int regData;
	int retVal;

	if(bit >= 16)
		return -1;

	retVal = smi_read(priv, reg, &regData);
	if(retVal != 0)
		return -1;

	if(value)
		regData = regData | (1 << bit);
	else
		regData = regData & (~(1 << bit));

	retVal = smi_write(priv, reg, regData);
	if(retVal != 0)
		return -1;

	return 0;
}

int rtl8367_setAsicRegBits(struct sgmac_priv *priv, int reg, int bits, int value)
{
	int regData;
	int retVal;
	int bitsShift;
	int valueShifted;

	if(bits >= (1 << 16) )
		return -1;

	bitsShift = 0;
	while(!(bits & (1 << bitsShift)))
	{
		bitsShift++;
		if(bitsShift >= 16)
			return -1;
	}
	valueShifted = value << bitsShift;

	if(valueShifted > 0xFFFF)
		return -1;

	retVal = smi_read(priv, reg, &regData);
	if(retVal != 0)
		return -1;

	regData = regData & (~bits);
	regData = regData | (valueShifted & bits);

	retVal = smi_write(priv, reg, regData);
	if(retVal != 0)
		return -1;

	return 0;
}

int rtl8367_getAsicReg(struct sgmac_priv *priv, int reg, int *pValue)
{
	int regData;
	int retVal;

	retVal = smi_read(priv, reg, &regData);
	if(retVal != 0)
		return -1;

	*pValue = regData;

	return 0;
}

int rtl8367_getAsicRegBit(struct sgmac_priv *priv, int reg, int bit, int *pValue)
{
	int regData;
	int retVal;

	retVal = smi_read(priv, reg, &regData);
	if(retVal != 0)
	return -1;

	*pValue = (regData & (0x1 << bit)) >> bit;

	return 0;
}

static int dal_rtl8367d_setAsicPortExtMode(struct sgmac_priv *priv, int id, int mode)
{
	int retVal;
	int mux;

	if( (retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_BYPASS_LINE_RATE, id, 0)) != 0)
            return retVal;

	/* Configure RGMII DP, DN, E2, MODE */
	if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_CHIP_DEBUG0, RTL8367D_SEL33_EXT1_OFFSET, 1)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_CHIP_DEBUG0, RTL8367D_DRI_EXT1_RG_OFFSET, 1)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_CHIP_DEBUG0, RTL8367D_DRI_EXT1_OFFSET, 1)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_CHIP_DEBUG0, RTL8367D_SLR_EXT1_OFFSET, 1)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBits(priv, RTL8367D_REG_CHIP_DEBUG1, RTL8367D_RG1_DN_MASK, 7)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBits(priv, RTL8367D_REG_CHIP_DEBUG1, RTL8367D_RG1_DP_MASK, 5)) != 0)
		return retVal;

	if ((retVal = rtl8367_setAsicRegBits(priv, RTL8367D_REG_EXT_TXC_DLY, RTL8367D_EXT1_RGMII_TX_DELAY_MASK, 0)) != 0)
		return retVal;

	/* Configure RGMII/MII mux to port 7 if RTK_UTP_PORT4 is not RGMII mode */
	if ((retVal = rtl8367_getAsicRegBit(priv, RTL8367D_REG_TOP_CON0, RTL8367D_MAC4_SEL_EXT1_OFFSET, &mux)) != 0)
		return retVal;

	if (mux == 0 && (retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_TOP_CON0, RTL8367D_MAC7_SEL_EXT1_OFFSET, 1)) != 0)
    		return retVal;

	if ((retVal = rtl8367_setAsicRegBits(priv, RTL8367D_REG_DIGITAL_INTERFACE_SELECT, RTL8367D_SELECT_GMII_1_MASK, mode)) != 0)
		return retVal;

	if((retVal = rtl8367_setAsicRegBits(priv, RTL8367D_REG_SDS1_MISC0, RTL8367D_SDS1_MODE_MASK, RTL8367D_PORT_SDS_MODE_DISABLE)) != 0)
		return retVal;

	return 0;
}

int dal_rtl8367d_port_macForceLink_set(struct sgmac_priv *priv, int port, rtk_port_mac_ability_t *pPortability)
{
	int retVal;
	int reg_data = 0;
	int reg_data2;
	int rtl8367d_speed;

	rtl8367d_speed = RTL8367D_EXT_PORT_SPEED_1000M;

	reg_data |= ((rtl8367d_speed & 0x0C) >> 2) << 12;
	reg_data |= pPortability->nway << 7;
	reg_data |= pPortability->txpause << 6;
	reg_data |= pPortability->rxpause << 5;
	reg_data |= pPortability->link << 4;
	reg_data |= pPortability->duplex << 2;
	reg_data |= rtl8367d_speed & 0x03;

	if(pPortability->forcemode)
		reg_data2 = 0xFFFF;
	else
		reg_data2 = 0;

	/* Link down */
	if((retVal = rtl8367_setAsicRegBit(priv, RTL8367D_REG_MAC0_FORCE_SELECT + (port - 10), RTL8367D_MAC0_FORCE_SELECT_LINK_ABLTY_OFFSET, 0)) != 0)
		return retVal;

	/* Configure ability without link */
	if((retVal = rtl8367_setAsicReg(priv, RTL8367D_REG_MAC0_FORCE_SELECT + (port - 10), reg_data & ~(0x0010))) != 0)
		return retVal;

	if((retVal = rtl8367_setAsicReg(priv, RTL8367D_REG_MAC0_FORCE_SELECT_EN + (port - 10), reg_data2)) != 0)
		return retVal;

	if((retVal = rtl8367_setAsicReg(priv, RTL8367D_REG_MAC0_FORCE_SELECT + (port - 10), reg_data)) != 0)
		return retVal;

	return 0;
}

int dal_rtl8367d_port_macForceLinkExt_set(struct sgmac_priv *priv, int port, int mode, rtk_port_mac_ability_t *pPortability)
{
	rtk_port_mac_ability_t ability;
	int retVal;
	int ext_id;

	ext_id = port - 15;

	/* Configure EXT port mode */
	if ((retVal = dal_rtl8367d_setAsicPortExtMode(priv, ext_id, mode)) != 0)
		return retVal;

	/* Configure Ability */
	memset(&ability, 0x00, sizeof(rtk_port_mac_ability_t));

	ability.forcemode = pPortability->forcemode;
	ability.duplex    = pPortability->duplex;
	ability.link      = pPortability->link;
	ability.nway      = pPortability->nway;
	ability.txpause   = pPortability->txpause;
	ability.rxpause   = pPortability->rxpause;
	ability.speed     = pPortability->speed;

	if ((retVal = dal_rtl8367d_port_macForceLink_set(priv, port, &ability)) != 0)
		return retVal;

	return 0;
}

int rtk8367d_rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port)
{
	rtk_port_mac_ability_t pPortability = {
			.forcemode = 1,
			.speed = PORT_SPEED_1000M,
			.duplex = 1,
			.link = 1,
			.nway = 0,
			.txpause = 1,
			.rxpause = 1,
	};
	dal_rtl8367d_port_macForceLinkExt_set(priv, port, MODE_EXT_RGMII, &pPortability);

	return 0;
}

int rtk8367c_rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port)
{
	int retVal;
	int ext_id;
	int regValue;
	int reg_data = 0, mode = 1, forcemode = 1, nway = 0, link = 1, speed = 2, duplex = 1, txpause = 1, rxpause = 1;

	ext_id = port - 15;

	if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_BYPASS_LINE_RATE, ext_id, 0)) != 0)
		return retVal;

	if (ext_id == 1) {
		if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != 0)
			return retVal;

		if ((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != 0)
			return retVal;

		if ((retVal = rtl8367_setAsicRegBits(priv, RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (ext_id * RTL8367C_SELECT_GMII_1_OFFSET), mode)) != 0)
			return retVal;
	} else if (ext_id == 2) {
		if((retVal = rtl8367_setAsicRegBits(priv, RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1, RTL8367C_SELECT_GMII_2_MASK, mode)) != 0)
			return retVal;

		if ((retVal = rtl8367_getAsicReg(priv, RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, &reg_data)) != 0)
			return retVal;
	}

	reg_data |= forcemode << 12;
	//reg_data |= mstfault << 9;
	//reg_data |= mstmode << 8;
	reg_data |= nway << 7;
	reg_data |= txpause << 6;
	reg_data |= rxpause << 5;
	reg_data |= link << 4;
	reg_data |= duplex << 2;
	reg_data |= speed;

	if(ext_id == 1)
	{
		if ((retVal = rtl8367_getAsicReg(priv, RTL8367C_REG_REG_TO_ECO4, &regValue)) != 0)
			return retVal;

		if((regValue & (0x0001 << 5)) && (regValue & (0x0001 << 7)))
		{
			return 0;
		}

		if((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, duplex)) != 0)
			return retVal;

		if((retVal = rtl8367_setAsicRegBits(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, speed)) != 0)
			return retVal;

		if((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, link)) != 0)
			return retVal;

		if((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, txpause)) != 0)
			return retVal;

		if((retVal = rtl8367_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, rxpause)) != 0)
			return retVal;

		retVal = rtl8367_setAsicReg(priv, RTL8367C_REG_DIGITAL_INTERFACE0_FORCE + ext_id, reg_data);

	} else if(ext_id == 2) {
		retVal = rtl8367_setAsicReg(priv, RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, reg_data);
	}

	printf("End %s port %d\n", __func__, port);
	return retVal;
}

int rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port)
{
	if (priv->chip_id == 0x6642)
		rtk8367d_rtk_extPort_rgmii_init(priv, port);
	else if (priv->chip_id == 0x6367)
		rtk8367c_rtk_extPort_rgmii_init(priv, port);
	else
		printf("rtk8367 chip id is wrong not init rgmii\n");

	return 0;
}


int rtl8367_getAsicPHYReg(struct sgmac_priv *priv, int phyNo, int phyReg, int *phyData)
{
	int regAddr, ocpAddr, ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

	ocpAddr	= 0xa400 + phyReg*2;
	ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
	if (rtl8367_setAsicRegBits(priv, RTL8367C_REG_GPHY_OCP_MSB_0,
				RTL8367C_CFG_CPU_OCPADR_MSB_MASK,
				ocpAddrPrefix))
		return -1;

	/*prepare access address*/
	ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
	ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
	regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) |
		(phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
	if (rtl8367_getAsicReg(priv, regAddr, phyData))
		return -1;

	return 0;
}

int rtl8367_setAsicPHYReg(struct sgmac_priv *priv, int phyNo, int phyReg, int phyData)
{
	int regAddr, ocpAddr, ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

	ocpAddr	= 0xa400 + phyReg*2;
	ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
	if (rtl8367_setAsicRegBits(priv, RTL8367C_REG_GPHY_OCP_MSB_0,
				RTL8367C_CFG_CPU_OCPADR_MSB_MASK,
				ocpAddrPrefix))
		return -1;

	/*prepare access address*/
	ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
	ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
	regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) |
		(phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;

    if(rtl8367_setAsicReg(priv, regAddr, phyData))
        return -1;

    return 0;

}

int rtk_port_isolation_set(struct sgmac_priv *priv, int port, rtk_portmask_t *pPortmask)
{
	rtl8367_setAsicReg(priv, (0x08a2+port), pPortmask->bits[0]);
    return 0;
}

#if defined(CONFIG_SOC_SFA28_MPW0)
int rtk_port_rgmiiDelayExt_set(struct sgmac_priv *priv, int port, int txDelay, int rxDelay)
{
	int retVal;
	int regAddr, regData;

	if ((txDelay > 1) || (rxDelay > 7))
		return -1;

	if(port == 16)
		regAddr = RTL8367C_REG_EXT1_RGMXF;
	else if(port == 17)
		regAddr = RTL8367C_REG_EXT2_RGMXF;
	else
		return -1;

	if ((retVal = rtl8367_getAsicReg(priv, regAddr, &regData)) != 0)
		return retVal;

	regData = (regData & 0xFFF0) | ((txDelay << 3) & 0x0008) | (rxDelay & 0x0007);

	if ((retVal = rtl8367_setAsicReg(priv, regAddr, regData)) != 0)
		return retVal;

	return 0;
}
#endif

int rtl8367_setAsicPortEnableAll(struct sgmac_priv *priv, int  enable)
{
	if(enable >= 2)
		return 1;

	return rtl8367_setAsicRegBit(priv, RTL8367C_REG_PHY_AD, RTL8367C_PDNPHY_OFFSET, !enable);
}

int dal_rtl8367_port_phyEnableAll_set(struct sgmac_priv *priv, int enable)
{
	int port;
	int data;

	rtl8367_setAsicPortEnableAll(priv,1);
	for(port = 0; port < 4; port++)
	{

		if (rtl8367_getAsicPHYReg(priv, port, 0, &data))
			return -1;

		if ( 1 == enable)
		{
			data &= 0xF7FF;
			data |= 0x0200;
		}
		else
		{
			data |= 0x0800;
		}

		if (rtl8367_setAsicPHYReg(priv, port, 0, data))
			return -1;
	}

	return 0;

}