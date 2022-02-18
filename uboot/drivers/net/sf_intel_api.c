#include "sfa18_gmac.h"
#include "sf_intel_api.h"
#include <asm/io.h>

extern struct sgmac_priv *g_priv;
extern int sgmac_mdio_read(struct mii_dev *bus, int addr, int devad, int reg);
extern int sgmac_mdio_write(struct mii_dev *bus, int addr, int devad, int reg, u16 val);

static ur num_ports;
const PCE_MICROCODE pce_mc_sw2_3 = {
	/*-----------------------------------------------------------------*/
	/**   value    mask   ns  out_fields   L  type   flags   ipv4_len **/
	/*-----------------------------------------------------------------*/
	/* V22_2X (IPv6 issue fixed) */
	PCE_MC_M(0x88c3 , 0xFFFF , 1 , GOUT_ITAG0 , 4 , INSTR , GFLAG_ITAG , 0),
	PCE_MC_M(0x8100 , 0xFFFF , 4 , GOUT_STAG0 , 2 , INSTR , GFLAG_SVLAN, 0),
	PCE_MC_M(0x88A8 , 0xFFFF , 4 , GOUT_STAG0 , 2 , INSTR , GFLAG_SVLAN, 0),
	PCE_MC_M(0x9100 , 0xFFFF , 4 , GOUT_STAG0 , 2 , INSTR , GFLAG_SVLAN, 0),
	PCE_MC_M(0x8100 , 0xFFFF , 5 , GOUT_VTAG0 , 2 , INSTR , GFLAG_VLAN , 0),
	PCE_MC_M(0x88A8 , 0xFFFF , 6 , GOUT_VTAG0 , 2 , INSTR , GFLAG_VLAN , 0),
	PCE_MC_M(0x9100 , 0xFFFF , 4 , GOUT_VTAG0 , 2 , INSTR , GFLAG_VLAN , 0),
	PCE_MC_M(0x8864 , 0xFFFF , 20 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0800 , 0xFFFF , 24 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x86DD , 0xFFFF , 25 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x8863 , 0xFFFF , 19 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0xF800 , 13 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0600 , 0x0600 , 44 , GOUT_ETHTYP , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 15 , GOUT_NONE , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0xAAAA , 0xFFFF , 17 , GOUT_NONE , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0300 , 0xFF00 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_SNAP , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_DIP7 , 3 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 21 , GOUT_DIP7 , 3 , INSTR , GFLAG_PPPOE, 0),
	PCE_MC_M(0x0021 , 0xFFFF , 24 , GOUT_NONE , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0057 , 0xFFFF , 25 , GOUT_NONE , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x4000 , 0xF000 , 27 , GOUT_IP0 , 4 , INSTR , GFLAG_IPV4 , 1),
	PCE_MC_M(0x6000 , 0xF000 , 30 , GOUT_IP0 , 3 , INSTR , GFLAG_IPV6 , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 28 , GOUT_IP3 , 2 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 29 , GOUT_SIP0 , 4 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_NONE , 0 , LENACCU , GFLAG_NO , 0),
	PCE_MC_M(0x1100 , 0xFF00 , 43 , GOUT_PROT , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0600 , 0xFF00 , 43 , GOUT_PROT , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0xFF00 , 36 , GOUT_IP3 , 17 , INSTR , GFLAG_HOP , 0),
	PCE_MC_M(0x2B00 , 0xFF00 , 36 , GOUT_IP3 , 17 , INSTR , GFLAG_NN1 , 0),
	PCE_MC_M(0x3C00 , 0xFF00 , 36 , GOUT_IP3 , 17 , INSTR , GFLAG_NN2 , 0),
	PCE_MC_M(0x0000 , 0x0000 , 43 , GOUT_PROT , 1 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x00F0 , 38 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_NONE , 0 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0xFF00 , 36 , GOUT_NONE , 0 , IPV6 , GFLAG_HOP , 0),
	PCE_MC_M(0x2B00 , 0xFF00 , 36 , GOUT_NONE , 0 , IPV6 , GFLAG_NN1 , 0),
	PCE_MC_M(0x3C00 , 0xFF00 , 36 , GOUT_NONE , 0 , IPV6 , GFLAG_NN2 , 0),
	PCE_MC_M(0x0000 , 0x00FC , 44 , GOUT_PROT , 0 , IPV6 , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_NONE , 0 , IPV6 , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 44 , GOUT_SIP0 , 16 , INSTR , GFLAG_NO , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_APP0 , 4 , INSTR , GFLAG_IGMP , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
	PCE_MC_M(0x0000 , 0x0000 , 45 , GOUT_NONE , 0 , INSTR , GFLAG_END , 0),
};

int gsw_reg_rd(struct sgmac_priv *priv,u16 Offset, u16 Shift, u16 Size, uint *value)
{
	u16 ro, rv = 0, mask;

	if ((Offset & 0xD000) == 0xD000)
		ro = Offset;
	else
		ro = (Offset | 0xE000);

	sgmac_mdio_write(priv->bus, 0, 0, SMDIO_WRADDR, ro);
	rv = sgmac_mdio_read(priv->bus, 0, 0, SMDIO_RDADDR);

	mask = (1 << Size) - 1;
	rv = (rv >> Shift);
	*value = (rv & mask);
	return 0;
}

int gsw_reg_wr(struct sgmac_priv *priv,u16 Offset, u16 Shift, u16 Size, u32 value)
{
	u16 ro, rv = 0, mask;

	if ((Offset & 0xD000) == 0xD000)
		ro = Offset;
	else
		ro = (Offset | 0xE000);

	sgmac_mdio_write(priv->bus, 0, 0, SMDIO_WRADDR, ro);

	if (Size != 16) {
		rv = sgmac_mdio_read(priv->bus, 0, 0, SMDIO_RDADDR);
		mask = (1 << Size) - 1;
		mask = (mask << Shift);
		value = ((value << Shift) & mask);
		value = ((rv & ~mask) | value);
		sgmac_mdio_write(priv->bus, 0, 0, SMDIO_WRADDR, ro);
	}

	sgmac_mdio_write(priv->bus, 0, 0, SMDIO_RDADDR, value);
	return 0;
}

int GSW_MDIO_DataRead(struct sgmac_priv *priv, GSW_MDIO_data_t *parm)
{
	int s;
	uint r;

	do {
		s = gsw_reg_rd(priv,MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != 0)
			return s;
	} while (r);
	r = ((0x2 << 10) | ((parm->nAddressDev & 0x1F) << 5)
		| (parm->nAddressReg & 0x1F));
	s = gsw_reg_wr(priv,MMDIO_CTRL_MBUSY_OFFSET,
		MMDIO_CTRL_REGAD_SHIFT,
		MMDIO_CTRL_REG_SIZE, r);
	if (s != 0)
		return s;
	do {
		s = gsw_reg_rd(priv,MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != 0)
			return s;
	} while (r);
	s = gsw_reg_rd(priv,MMDIO_READ_RDATA_OFFSET,
		MMDIO_READ_RDATA_SHIFT,
		MMDIO_READ_RDATA_SIZE, &r);
	if (s != 0)
		return s;
	parm->nData = (r & 0xFFFF);
	return 0;
}

int GSW_MDIO_DataWrite(struct sgmac_priv *priv, GSW_MDIO_data_t *parm)
{
	int s;
	uint r;

	do {
		s = gsw_reg_rd(priv,MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != 0)
			return s;
	} while (r);

	r = parm->nData & 0xFFFF;
	s = gsw_reg_wr(priv,MMDIO_WRITE_WDATA_OFFSET,
		MMDIO_WRITE_WDATA_SHIFT,
		MMDIO_WRITE_WDATA_SIZE, r);
	if (s != 0)
		return s;
	r = ((0x1 << 10) | ((parm->nAddressDev & 0x1F) << 5)
		| (parm->nAddressReg & 0x1F));
	s = gsw_reg_wr(priv,MMDIO_CTRL_MBUSY_OFFSET,
		MMDIO_CTRL_REGAD_SHIFT,
		MMDIO_CTRL_REG_SIZE, r);
	if (s != 0)
		return s;
	do {
		s = gsw_reg_rd(priv,MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != 0)
			return s;
	} while (r);
	return 0;
}

int intel_rgmii_init(struct sgmac_priv *priv, int port)
{
	int s;
	GSW_MDIO_data_t md;
	uint r;

	s = gsw_reg_rd(priv,(MAC_PSTAT_PACT_OFFSET + (0xC * port)),
			MAC_PSTAT_PACT_SHIFT,
			MAC_PSTAT_PACT_SIZE, &r);
	if (s != 0)
		return s;

	if (r) {
		uint pa, pc;
		s = gsw_reg_rd(priv,(PHY_ADDR_0_ADDR_OFFSET - port),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_ADDR_SIZE, &pa);
		if (s != 0)
			return s;

		pc = PHY_AN_ADV_1000FDX;
		md.nAddressReg = 9;
		md.nAddressDev = pa;
		s = GSW_MDIO_DataRead(priv, &md);
		if (s != 0)
			return s;
		md.nData &= ~(PHY_AN_ADV_1000HDX
				| PHY_AN_ADV_1000FDX);
		md.nData |= pc;
		md.nAddressDev = pa;
		md.nAddressReg = 9;
		s = GSW_MDIO_DataWrite(priv, &md);
		if (s != 0)
			return s;
		md.nAddressReg = 4;
		md.nAddressDev = pa;
		s = GSW_MDIO_DataRead(priv, &md);
		if (s != 0)
			return s;
		md.nData &= ~(PHY_AN_ADV_10HDX
				| PHY_AN_ADV_10FDX
				| PHY_AN_ADV_100HDX
				| PHY_AN_ADV_100FDX);
		md.nAddressReg = 4;
		md.nAddressDev = pa;
		s = GSW_MDIO_DataWrite(priv, &md);
		if (s != 0)
			return s;

		md.nAddressDev = pa;
		md.nAddressReg = 0;
		s = GSW_MDIO_DataRead(priv, &md);
		if (s != 0)
			return s;
		md.nData = 0x1200;
		s = GSW_MDIO_DataWrite(priv, &md);
		if (s != 0)
			return s;
	} else {
		uint pr;
		s = gsw_reg_rd(priv,(PHY_ADDR_0_ADDR_OFFSET - port),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_REG_SIZE,
				&pr);
		if (s != 0)
			return s;

		//set GSW_DUPLEX_FULL
		pr &= ~(3 << 9);
		pr |= (1 << 9);

		//set GSW_PORT_LINK_UP
		pr &= ~(3 << 13);
		pr |= (1 << 13);

		//set GSW_PORT_SPEED_1000
		pr &= ~(3 << 11);
		pr |= (2 << 11);
		s = gsw_reg_wr(priv,(PHY_ADDR_0_ADDR_OFFSET - port),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_REG_SIZE,
				pr);
		if (s != 0)
			return s;
	}
	//'Low Power Idle' Support for 'Energy Efficient Ethernet'. default dsiable.
	s = gsw_reg_wr(priv,(MAC_CTRL_4_LPIEN_OFFSET + (0xC * port)),
		MAC_CTRL_4_LPIEN_SHIFT,
		MAC_CTRL_4_LPIEN_SIZE, 0);
	printf("End %s\n", __func__);
	return 0;
}

int GSW_MmdDataWrite(GSW_MMD_data_t *parm)
{
	int s;
	GSW_MDIO_data_t md;
	u32 found = 0, pn, pa, mr, dev, pi;
	s = gsw_reg_rd(g_priv,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &pn);
	if (s != 0)
		return s;
	for (pi = 0; pi < pn; pi++) {
		s = gsw_reg_rd(g_priv,
			(PHY_ADDR_0_ADDR_OFFSET - pi),
			PHY_ADDR_0_ADDR_SHIFT,
			PHY_ADDR_0_ADDR_SIZE, &pa);
		if (s != 0)
			return s;
		if (pa == parm->nAddressDev) {
			found = 1;
			break;
		}
	}
	if (found) {
		s = gsw_reg_rd(g_priv,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, &mr);
		if (s != 0)
			return s;
		mr &= ~(1 << pi);
		dev = ((parm->nAddressReg >> 16) & 0x1F);
		s = gsw_reg_wr(g_priv,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != 0)
			return s;

		udelay(800);
		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = dev;
		s = GSW_MDIO_DataWrite(g_priv, &md);
		if (s != 0)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		md.nData = parm->nAddressReg & 0xFFFF;
		s = GSW_MDIO_DataWrite(g_priv, &md);
		if (s != 0)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = ((0x4000) | dev);
		s = GSW_MDIO_DataWrite(g_priv, &md);
		if (s != 0)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		md.nData = parm->nData;
		s = GSW_MDIO_DataWrite(g_priv, &md);
		if (s != 0)
			return s;
		mr |= (1 << pi);
		s = gsw_reg_wr(g_priv,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != 0)
			return s;
	} else {
		return -1;
	}
	return 0;
}

void intel7084_led_init(int led_mode)
{
	GSW_MMD_data_t md;
	int i = 0, mode = 0;

	if (led_mode == LED_ALL_ON)
		mode = GROUND_MODE;
	else if (led_mode == LED_ALL_BLINK)
		return; // switch not support, please use all on/off
	else
		mode = POWER_MODE;

	// port[0:4] <--> led[5:9]
	gsw_reg_wr(g_priv, LED_MD_CFG_LED5_OFFSET,
			LED_MD_CFG_LED5_SHIFT,
			LED_MD_CFG_LED5_SIZE,
			mode);
	gsw_reg_wr(g_priv, LED_MD_CFG_LED6_OFFSET,
			LED_MD_CFG_LED6_SHIFT,
			LED_MD_CFG_LED6_SIZE,
			mode);
	gsw_reg_wr(g_priv, LED_MD_CFG_LED7_OFFSET,
			LED_MD_CFG_LED7_SHIFT,
			LED_MD_CFG_LED7_SIZE,
			mode);
	gsw_reg_wr(g_priv, LED_MD_CFG_LED8_OFFSET,
			LED_MD_CFG_LED8_SHIFT,
			LED_MD_CFG_LED8_SIZE,
			mode);
	gsw_reg_wr(g_priv,LED_MD_CFG_LED9_OFFSET,
			LED_MD_CFG_LED9_SHIFT,
			LED_MD_CFG_LED9_SIZE,
			mode);

	// config led when link is 10/100/1000Mbit/s
	for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
		md.nAddressDev = i;
		md.nAddressReg = 0x1F01E4;
		if (led_mode == LED_ALL_OFF)
			md.nData = 0x0; // no blink
		else{
			md.nData = 0x70; // active on 10/100/1000
			return;
		}

		//md.nData = 0x70; // active on 10/100/1000
		GSW_MmdDataWrite(&md);
	}
}

int npport(u8 port)
{
	ur r;
	int s;
	s = gsw_reg_rd(g_priv,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != 0)
		return s;
	num_ports = r;
	if (port >= r)
		return -1;
	return 0;
}

int pctrl_bas_set(void)
{
	int s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_CTRL_BAS_OFFSET,
		PCE_TBL_CTRL_BAS_SHIFT,
		PCE_TBL_CTRL_BAS_SIZE, 1);
	if (s != 0)
		return s;
	return 0;
}

int pctrl_bas_busy(void)
{
	ur r;
	int s;
	do {
		s = gsw_reg_rd(g_priv,
			PCE_TBL_CTRL_BAS_OFFSET,
			PCE_TBL_CTRL_BAS_SHIFT,
			PCE_TBL_CTRL_BAS_SIZE, &r);
		if (s != 0)
			return s;
	}	while (r);
	return 0;
}

int pctrl_reg_clr(void )
{
	int s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_REG_SIZE, 0);
	if (s != 0)
		return s;
	return 0;
}

int pctrl_addr_opmod(
	u16 addr, u16 opmode)
{
	ur r;
	int s;
	s = gsw_reg_rd(g_priv,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_OPMOD_SIZE, &r);
	if (s != 0)
		return s;
	r &= ~(0x7F);
	r |= (((opmode & 3) << 5) | (addr & 0x1F));
	s = gsw_reg_wr(g_priv,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_OPMOD_SIZE, r);
	if (s != 0)
		return s;
	return 0;
}

static int vlan_map_tbl_rd(pctbl_prog_t *pt)
{
	int s;
	ur r;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != 0)
		return s;
	s = pctrl_addr_opmod(pt->table, pt->opmode);
	if (s != 0)
		return s;
	s = pctrl_bas_set();
	if (s != 0)
		return s;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	s = gsw_reg_rd(g_priv,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE, &r);
	if (s != 0)
		return s;
	pt->val[0] = r;
	s = gsw_reg_rd(g_priv,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE, &r);
	if (s != 0)
		return s;
	pt->val[1] = r;
	s = gsw_reg_rd(g_priv,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE, &r);
	if (s != 0)
		return s;
	pt->val[2] = r;
	s = gsw_reg_rd(g_priv,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE, &r);
	if (s != 0)
		return s;
	pt->valid = r;
	return 0;
}

static int vlan_map_tbl_wr(
	pctbl_prog_t *pt)
{
	int s;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	s = pctrl_reg_clr();
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != 0)
		return s;
	s = pctrl_addr_opmod( pt->table, pt->opmode);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != 0)
		return s;
	s = pctrl_bas_set();
	if (s != 0)
		return s;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	return 0;
}

int GSW_VLAN_IdCreate(
	GSW_VLAN_IdCreate_t *parm)
{
	int s;
	pctbl_prog_t pt;

	memset(&pt, 0, sizeof(pctbl_prog_t));
	pt.pcindex = parm->nVId;
	pt.table = PCE_VLANMAP_INDEX;
	pt.val[0] = (parm->nFId & 0xFF);
	pt.opmode = PCE_OPMODE_ADWR;
	s = vlan_map_tbl_wr(&pt);
	if (s != 0)
		return s;
	return 0;
}

int GSW_VLAN_PortMemberAdd(
	GSW_VLAN_portMemberAdd_t *parm)
{
	int s;
	u32 pi = parm->nPortId;
	pctbl_prog_t pt;
	u16  pm, tm, val0;
	s = npport( (pi & 0xFF));
	if (s != 0)
		return s;

	if (parm->nVId >= VLAN_MAP_TBL_SIZE)
		return -1;

	memset(&pt, 0, sizeof(pctbl_prog_t));
	pt.table = PCE_VLANMAP_INDEX;
	pt.pcindex = parm->nVId;
	pt.opmode = PCE_OPMODE_ADRD;
	s = vlan_map_tbl_rd(&pt);
	if (s != 0)
		return s;
	pm	= (pt.val[1]);
	tm = (pt.val[2]);
	val0 = (pt.val[0]);
/*  Support  portmap information. */
/*  To differentiate between port index and portmap, */
/* the MSB (highest data bit) should be 1.*/
	if (pi & 0x80000000) { /*Port Map */
		pm |= (pi & 0xFFFF);
		if (parm->bVLAN_TagEgress)
			tm |= (pi & 0xFFFF);
		else
			tm &= ~(pi & 0xFFFF);
	} else {
		pm |= 1 << pi;
		if (parm->bVLAN_TagEgress)
			tm |= 1 << pi;
		else
			tm &= ~(1 << pi);
	}
	pt.table = PCE_VLANMAP_INDEX;
	pt.pcindex = parm->nVId;
	pt.opmode = PCE_OPMODE_ADWR;
	pt.val[0] = val0;
	pt.val[1] = pm;
	pt.val[2] = tm;
	s = vlan_map_tbl_wr( &pt);
	if (s != 0)
		return s;
	return 0;
}

int GSW_VLAN_PortCfgGet(
	GSW_VLAN_portCfg_t *parm)
{
	int s;
	u8 pi = parm->nPortId;
	ur r;
	s = npport(pi);
	if (s != 0)
		return s;
	s = gsw_reg_rd(g_priv,
		(PCE_DEFPVID_PVID_OFFSET + (10 * pi)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, &r);
	if (s != 0)
		return s;
	parm->nPortVId = r;
	s = gsw_reg_rd(g_priv,
		(PCE_VCTRL_UVR_OFFSET + (10 * pi)),
		PCE_VCTRL_UVR_SHIFT,
		PCE_VCTRL_UVR_SIZE, &r);
	if (s != 0)
		return s;
	parm->bVLAN_UnknownDrop = r;
	s = gsw_reg_rd(g_priv,
		(PCE_VCTRL_VSR_OFFSET + (10 * pi)),
		PCE_VCTRL_VSR_SHIFT,
		PCE_VCTRL_VSR_SIZE, &r);
	if (s != 0)
		return s;
	parm->bVLAN_ReAssign = r;
	s = gsw_reg_rd(g_priv,
		(PCE_VCTRL_VIMR_OFFSET + (10 * pi)),
		PCE_VCTRL_VIMR_SHIFT,
		PCE_VCTRL_VIMR_VEMR_SIZE, &r);
	if (s != 0)
		return s;
	parm->eVLAN_MemberViolation = r;
	s = gsw_reg_rd(g_priv,
		(PCE_VCTRL_VINR_OFFSET + (10 * pi)),
		PCE_VCTRL_VINR_SHIFT,
		PCE_VCTRL_VINR_SIZE, &r);
	if (s != 0)
		return s;
	switch (r) {
	case 0:
		parm->eAdmitMode = GSW_VLAN_ADMIT_ALL;
		break;
	case 1:
		parm->eAdmitMode = GSW_VLAN_ADMIT_TAGGED;
		break;
	case 2:
		parm->eAdmitMode = GSW_VLAN_ADMIT_UNTAGGED;
		break;
	default:
		parm->eAdmitMode = r;
		break;
	}
	s = gsw_reg_rd(g_priv,
		(PCE_PCTRL_0_TVM_OFFSET + (10 * pi)),
		PCE_PCTRL_0_TVM_SHIFT,
		PCE_PCTRL_0_TVM_SIZE, &r);
	if (s != 0)
		return s;
	parm->bTVM = r;
	return 0;
}

int GSW_VLAN_PortCfgSet(
	GSW_VLAN_portCfg_t *parm)
{
	ur r;
	int s;
	u8 pi = parm->nPortId;
	s = npport(pi);
	if (s != 0)
		return s;
	r = parm->nPortVId;
	s = gsw_reg_wr(g_priv,
		(PCE_DEFPVID_PVID_OFFSET + (10 * pi)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, r);
	if (s != 0)
		return s;
	r = (parm->bVLAN_UnknownDrop & 0x1);
	s = gsw_reg_wr(g_priv,
		(PCE_VCTRL_UVR_OFFSET + (10 * pi)),
		PCE_VCTRL_UVR_SHIFT,
		PCE_VCTRL_UVR_SIZE, r);
	if (s != 0)
		return s;
	r = parm->bVLAN_ReAssign;
	s = gsw_reg_wr(g_priv,
		(PCE_VCTRL_VSR_OFFSET + (10 * pi)),
		PCE_VCTRL_VSR_SHIFT,
		PCE_VCTRL_VSR_SIZE, r);
	if (s != 0)
		return s;
	r = (parm->eVLAN_MemberViolation & 0x3);
	s = gsw_reg_wr(g_priv,
		(PCE_VCTRL_VIMR_OFFSET + (10 * pi)),
		PCE_VCTRL_VIMR_SHIFT,
		PCE_VCTRL_VIMR_VEMR_SIZE, r);
	if (s != 0)
		return s;
	switch (parm->eAdmitMode) {
	case GSW_VLAN_ADMIT_ALL:
		r = 0;
		break;
	case GSW_VLAN_ADMIT_TAGGED:
		r = 1;
		break;
	case GSW_VLAN_ADMIT_UNTAGGED:
		r = 2;
		break;
	default:
		r = 0;
	}
	s = gsw_reg_wr(g_priv,
		(PCE_VCTRL_VINR_OFFSET + (10 * pi)),
		PCE_VCTRL_VINR_SHIFT,
		PCE_VCTRL_VINR_SIZE, r);
	if (s != 0)
		return s;
	r = (parm->bTVM);
	s = gsw_reg_wr(g_priv,
		(PCE_PCTRL_0_TVM_OFFSET + (10 * pi)),
		PCE_PCTRL_0_TVM_SHIFT,
		PCE_PCTRL_0_TVM_SIZE, r);
	if (s != 0)
		return s;
	return 0;
}

int intel7084_set_vlan_enable(u32 enable)
{
    // enable vlan aware
	gsw_reg_wr(g_priv,
			PCE_GCTRL_0_VLAN_OFFSET,
			PCE_GCTRL_0_VLAN_SHIFT,
			PCE_GCTRL_0_VLAN_SIZE,
			enable);

    return 0;
}

int GSW_Disable(void)
{
	ur r, i;
	int s;
	s = gsw_reg_rd(g_priv,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != 0)
		return s;
	for (i = 0; i < r; i++) {
		s = gsw_reg_wr(g_priv,
			(FDMA_PCTRL_EN_OFFSET + (i * 0x6)),
			FDMA_PCTRL_EN_SHIFT,
			FDMA_PCTRL_EN_SIZE, 0);
		if (s != 0)
			return s;
		s = gsw_reg_wr(g_priv,
			(SDMA_PCTRL_PEN_OFFSET + (i * 0x6)),
			SDMA_PCTRL_PEN_SHIFT,
			SDMA_PCTRL_PEN_SIZE, 0);
		if (s != 0)
			return s;
	}
	return 0;
}

static int parser_ll_twr(
	pctbl_prog_t *pt)
{
	int s;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	s = pctrl_reg_clr();
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != 0)
		return s;
	s = pctrl_addr_opmod(pt->table, pt->opmode);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_3_VAL3_OFFSET,
		PCE_TBL_VAL_3_VAL3_SHIFT,
		PCE_TBL_VAL_3_VAL3_SIZE,
		pt->val[3]);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != 0)
		return s;
	s = pctrl_bas_set();
	if (s != 0)
		return s;
	s = pctrl_bas_busy();
	if (s != 0)
		return s;
	return 0;
}

/* PCE Table Micro Code Init routine */
int gsw_uc_code_init(void)
{
	int s;
	u32 vn1, lcnt = 0;
	pctbl_prog_t pt;
	s = gsw_reg_wr(g_priv,
		PCE_GCTRL_0_MC_VALID_OFFSET,
		PCE_GCTRL_0_MC_VALID_SHIFT,
		PCE_GCTRL_0_MC_VALID_SIZE, 0x0);
	if (s != 0)
		return s;
	s = gsw_reg_rd(g_priv,
		MANU_ID_PNUML_OFFSET,
		MANU_ID_PNUML_SHIFT,
		MANU_ID_PNUML_SIZE, &vn1);
	if (s != 0)
		return s;
	/*7084 (G)*/
	u32 r, p, t;
	s = gsw_reg_rd(g_priv,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			&r);
	if (s != 0)
		return s;
	s = gsw_reg_rd(g_priv,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			&p);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			1);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			0x1000);

	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			0);
	if (s != 0)
		return s;
	do {
		s = gsw_reg_rd(g_priv,
				RST_REQ_RD0_OFFSET,
				RST_REQ_RD0_SHIFT,
				RST_REQ_RD0_SIZE,
				&t);
		if (s != 0)
			return s;
		lcnt++;
	} while ((!t) && (lcnt < 10));
	s = gsw_reg_wr(g_priv,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			p);
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			r);
	if (s != 0)
		return s;
	s = gsw_reg_rd(g_priv,
			PCE_GCTRL_0_MC_VALID_OFFSET,
			PCE_GCTRL_0_MC_VALID_SHIFT,
			PCE_GCTRL_0_MC_VALID_SIZE,
			&vn1);
	if (s != 0)
		return s;
	if ((lcnt >= 10) && !vn1) {
		int i;
		for (i = 0; i < 64; i++) {
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.val[3] = pce_mc_sw2_3[i].val_3;
			pt.val[2] = pce_mc_sw2_3[i].val_2;
			pt.val[1] = pce_mc_sw2_3[i].val_1;
			pt.val[0] = pce_mc_sw2_3[i].val_0;
			pt.pcindex = i;
			pt.table = PCE_PARS_INDEX;
			pt.opmode = PCE_OPMODE_ADWR;
			s = parser_ll_twr(&pt);
			if (s != 0)
				return s;
		}
		s = gsw_reg_wr(g_priv,
				PCE_GCTRL_0_MC_VALID_OFFSET,
				PCE_GCTRL_0_MC_VALID_SHIFT,
				PCE_GCTRL_0_MC_VALID_SIZE,
				0x1);
		if (s != 0)
			return s;
	}
	return 0;
}

int GSW_Enable(void)
{
    ur r, i;
    int s;
    s = gsw_reg_rd(g_priv,
        ETHSW_CAP_1_PPORTS_OFFSET,
        ETHSW_CAP_1_PPORTS_SHIFT,
        ETHSW_CAP_1_PPORTS_SIZE, &r);
    if (s != 0)
        return s;
    for (i = 0; i < r;  i++) {
        s = gsw_reg_wr(g_priv,
            (FDMA_PCTRL_EN_OFFSET + (i * 0x6)),
            FDMA_PCTRL_EN_SHIFT,
            FDMA_PCTRL_EN_SIZE, 1);
        if (s != 0)
            return s;
        s = gsw_reg_wr(g_priv,
            (SDMA_PCTRL_PEN_OFFSET + (i * 0x6)),
            SDMA_PCTRL_PEN_SHIFT,
            SDMA_PCTRL_PEN_SIZE, 1);
        if (s != 0)
            return s;
        s = gsw_reg_wr(g_priv,
            (BM_PCFG_CNTEN_OFFSET + (i * 2)),
            BM_PCFG_CNTEN_SHIFT,
            BM_PCFG_CNTEN_SIZE, 1);
        if (s != 0)
            return s;
    }
    return 0;
}

int config_sw_def_vals(void)
{
	int s;

	s = GSW_Disable();
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		GSWIP_CFG_SE_OFFSET,
		GSWIP_CFG_SE_SHIFT,
		GSWIP_CFG_SE_SIZE, 1);
	if (s != 0)
		return s;
	s = gsw_uc_code_init();
	if (s != 0)
		return s;
	s = GSW_Enable();
	if (s != 0)
		return s;
	s = gsw_reg_wr(g_priv,
		PCE_GCTRL_1_VLANMD_OFFSET,
		PCE_GCTRL_1_VLANMD_SHIFT,
		PCE_GCTRL_1_VLANMD_SIZE, 1);
	if (s != 0)
		return s;
	return 0;
}

int ethsw_swapi_register(void)
{
	gsw_reg_wr(g_priv,
			GSWIP_CFG_SWRES_OFFSET,
			GSWIP_CFG_SWRES_SHIFT,
			GSWIP_CFG_SWRES_SIZE, 1);
	config_sw_def_vals();
	return 0;
}

int intel7084_vlan_set(void){
	int i = 0;
	u16 port, pvid;
	GSW_VLAN_portCfg_t parm;
	GSW_VLAN_IdCreate_t vlan;
	GSW_VLAN_portMemberAdd_t vlan_port;

	ethsw_swapi_register();
	intel7084_set_vlan_enable(1);

	for(; i < 4; i++){
		port = i;
		pvid = port + 1;

		//set port vlan cfg
		memset((void *)&parm, 0, sizeof(GSW_VLAN_portCfg_t));
		parm.nPortId = port;
		GSW_VLAN_PortCfgGet(&parm);
		parm.eAdmitMode = 0;
		parm.eVLAN_MemberViolation = 3;
		parm.nPortVId = pvid;
		GSW_VLAN_PortCfgSet(&parm);

		//create vlan id
		memset((void *)&vlan, 0, sizeof(GSW_VLAN_IdCreate_t));
		vlan.nVId = pvid;
		vlan.nFId = 0;
		GSW_VLAN_IdCreate(&vlan);

		//set vlan port members
		memset((void *)&vlan_port, 0, sizeof(GSW_VLAN_portMemberAdd_t));
		vlan_port.nVId = pvid;
		vlan_port.nPortId = port;
		vlan_port.bVLAN_TagEgress = 0;
		GSW_VLAN_PortMemberAdd(&vlan_port);

		memset((void *)&vlan_port, 0, sizeof(GSW_VLAN_portMemberAdd_t));
		vlan_port.nVId = pvid;
		vlan_port.nPortId = 5;
		vlan_port.bVLAN_TagEgress = 1;
		GSW_VLAN_PortMemberAdd(&vlan_port);
	}

	return 0;
}
