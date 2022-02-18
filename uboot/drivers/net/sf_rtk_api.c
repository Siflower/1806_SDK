#include "sfa18_gmac.h"
#include "sf_rtk_api.h"


extern int sgmac_mdio_read(struct mii_dev *bus, int addr, int devad, int reg);
extern int sgmac_mdio_write(struct mii_dev *bus, int addr, int devad, int reg, u16 val);

int smi_read(struct sgmac_priv *priv, int reg_addr, int *pdata)
{
	int data;
	sgmac_mdio_write(priv->bus, 0, 0, 31, 0xe);
	sgmac_mdio_write(priv->bus, 0, 0, 23, reg_addr);
	sgmac_mdio_write(priv->bus, 0, 0, 21, 1);
	data = sgmac_mdio_read(priv->bus, 0, 0, 25);
	*pdata = data;

	return 0;
}

int smi_write(struct sgmac_priv *priv, int reg_addr, int data)
{
	sgmac_mdio_write(priv->bus, 0, 0, 31, 0xe);
	sgmac_mdio_write(priv->bus, 0, 0, 23, reg_addr);
	sgmac_mdio_write(priv->bus, 0, 0, 24, data);
	sgmac_mdio_write(priv->bus, 0, 0, 21, 0x3);

	return 0;
}

int rtl8367c_setAsicReg(struct sgmac_priv *priv, int reg, int value)
{
	int retVal;

	retVal = smi_write(priv, reg, value);
	if(retVal != 0)
		return -1;

	return 0;
}

int rtl8367c_setAsicRegBit(struct sgmac_priv *priv, int reg, int bit, int value)
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

int rtl8367c_setAsicRegBits(struct sgmac_priv *priv, int reg, int bits, int value)
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

int rtl8367c_getAsicReg(struct sgmac_priv *priv, int reg, int *pValue)
{
	int regData;
	int retVal;

	retVal = smi_read(priv, reg, &regData);
	if(retVal != 0)
		return -1;

	*pValue = regData;

	return 0;
}

int rtk_extPort_rgmii_init(struct sgmac_priv *priv, int port)
{
	int retVal;
	int ext_id;
	int regValue;
	int reg_data = 0, mode = 1, forcemode = 1, nway = 0, link = 1, speed = 2, duplex = 1, txpause = 1, rxpause = 1;

	ext_id = port - 15;

	if( (retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_BYPASS_LINE_RATE, ext_id, 0)) != 0)
		return retVal;

	if( (retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != 0)
		return retVal;

	if( (retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != 0)
		return retVal;

	if((retVal = rtl8367c_setAsicRegBits(priv, RTL8367C_REG_DIGITAL_INTERFACE_SELECT, RTL8367C_SELECT_GMII_0_MASK << (ext_id * RTL8367C_SELECT_GMII_1_OFFSET), mode)) != 0)
		return retVal;

	reg_data |= forcemode << 12;
	//reg_data |= mstfault << 9;
	//reg_data |= mstmode << 8;
	reg_data |= nway << 7;
	reg_data |= txpause << 6;
	reg_data |= rxpause << 5;
	reg_data |= link << 4;
	reg_data |= duplex << 2;
	reg_data |= speed;

	if ((retVal = rtl8367c_getAsicReg(priv, RTL8367C_REG_REG_TO_ECO4, &regValue)) != 0)
		return retVal;

	if((regValue & (0x0001 << 5)) && (regValue & (0x0001 << 7)))
	{
		return 0;
	}

	if((retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_FDUP_OFFSET, duplex)) != 0)
		return retVal;

	if((retVal = rtl8367c_setAsicRegBits(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_SPD_MASK, speed)) != 0)
		return retVal;

	if((retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_LINK_OFFSET, link)) != 0)
		return retVal;

	if((retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_TXFC_OFFSET, txpause)) != 0)
		return retVal;

	if((retVal = rtl8367c_setAsicRegBit(priv, RTL8367C_REG_SDS_MISC, RTL8367C_CFG_SGMII_RXFC_OFFSET, rxpause)) != 0)
		return retVal;

	return rtl8367c_setAsicReg(priv, RTL8367C_REG_DIGITAL_INTERFACE0_FORCE + ext_id, reg_data);
}

int rtl8367c_getAsicPHYReg(struct sgmac_priv *priv, int phyNo, int phyReg, int *phyData)
{
	int regAddr, ocpAddr, ocpAddrPrefix, ocpAddr9_6, ocpAddr5_1;

	ocpAddr	= 0xa400 + phyReg*2;
	ocpAddrPrefix = ((ocpAddr & 0xFC00) >> 10);
	if (rtl8367c_setAsicRegBits(priv, RTL8367C_REG_GPHY_OCP_MSB_0,
				RTL8367C_CFG_CPU_OCPADR_MSB_MASK,
				ocpAddrPrefix))
		return -1;

	/*prepare access address*/
	ocpAddr9_6 = ((ocpAddr >> 6) & 0x000F);
	ocpAddr5_1 = ((ocpAddr >> 1) & 0x001F);
	regAddr = RTL8367C_PHY_BASE | (ocpAddr9_6 << 8) |
		(phyNo << RTL8367C_PHY_OFFSET) | ocpAddr5_1;
	if (rtl8367c_getAsicReg(priv, regAddr, phyData))
		return -1;

	return 0;
}

int rtk_port_isolation_set(struct sgmac_priv *priv, int port, rtk_portmask_t *pPortmask)
{
	rtl8367c_setAsicReg(priv, (0x08a2+port), pPortmask->bits[0]);
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

	if ((retVal = rtl8367c_getAsicReg(priv, regAddr, &regData)) != 0)
		return retVal;

	regData = (regData & 0xFFF0) | ((txDelay << 3) & 0x0008) | (rxDelay & 0x0007);

	if ((retVal = rtl8367c_setAsicReg(priv, regAddr, regData)) != 0)
		return retVal;

	return 0;
}
#endif
