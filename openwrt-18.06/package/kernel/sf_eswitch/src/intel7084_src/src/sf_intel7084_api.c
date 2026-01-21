/*
* Description
*
* Copyright (C) 2016-2021 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include "gsw_sw_init.h"


#define ETHSW_API_DEV_NAME "switch_api"
#define SWAPI_MAJOR_NUMBER	81
#define SMDIO_DEFAULT_BUSADDR   (0x0)
#define SMDIO_DEFAULT_PHYADDR	(0x0)
#define DEFAULT_SWITCH_NUM      (1)
#define SMDIO_WRADDR	(0x1F)
#define SMDIO_RDADDR	(0x0)
#define RGMII_PORT0	0x5
#define PQM_CONTEXT_TBL 0x09
#define GSW_SW_FKT(x, y) x ? (gsw_ll_fkt)y : NULL
#define MAC_TABLE_SIZE 2048


extern int mdio_read_ext(int phyaddr, int phyreg, int *phydata);
extern int mdio_write_ext(int phyaddr, int phyreg, int phydata);

static u8 prdflag;
static u8 mpnum;
static ur num_ports;
static u16 vlan_rd_index;
static u32 firstmac, mac_rd_index;
static DEFINE_SEMAPHORE(swapi_sem);
static int gsw_num = DEFAULT_SWITCH_NUM, hw_init = 1, id_argc = 0, addr_argc = 0;
int gsw_mdio_id[5] = { SMDIO_DEFAULT_BUSADDR, 0, 0, 0, 0 };
int gsw_mdio_addr[5] = { SMDIO_DEFAULT_PHYADDR, 0x1F, 0, 0, 0 };
ethsw_api_dev_t *pedev0[GSW_DEV_MAX];
port_config_t gpc[MAX_PORT_NUMBER];
stp8021x_t gstpconfig;
module_param(gsw_num, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(gsw_num, "No.of Switches supports");
module_param(hw_init, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(hw_init, "Switch HW Init during module load");
module_param_array(gsw_mdio_id, int, &id_argc, 0000);
MODULE_PARM_DESC(gsw_mdio_id, "An array of mdio bus id's");
module_param_array(gsw_mdio_addr, int, &addr_argc, 0000);
MODULE_PARM_DESC(gsw_mdio_addr, "An array of mdio address");


#define PCE_MC_M(val, msk, ns, out, len, type, flags, ipv4_len) \
	{ val, msk, (ns<<10 | out<<4 | len>>1),\
		(len&1)<<15 | type<<13 | flags<<9 | ipv4_len<<8 }
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


static GSW_return_t WriteMdio(u8 mdio_id, u16 phyaddr, u16 regaddr, u16 data)
{
	int result;

	/* Access MDIO */
	result = mdio_write_ext(phyaddr, regaddr, data);
	return (result != 0) ? GSW_statusErr : GSW_statusOk;
}

/* Customer can modify MDIO routines depends on SOC supports
*/
static GSW_return_t ReadMdio(u8 mdio_id, u16 phyaddr, u16 regaddr, u16 *data)
{
	int result;
	int value;

	/* Access MDIO */
	result = mdio_read_ext(phyaddr, regaddr, &value);
	*data = value;
	return (result != 0) ? GSW_statusErr : GSW_statusOk;
}

static GSW_return_t smdio_reg_rd(u8 mdio_id, u16 phyaddr, u16 regaddr, u16 *rdata)
{
	int rs;
	u16 d = 0;
	rs = ReadMdio(mdio_id, phyaddr, regaddr, &d);
	if (rs != GSW_statusOk) {
		GSW_PRINT("%s:%d pa=%d, ra=0x%x, rs:%d\n", __func__, __LINE__, \
			phyaddr, regaddr, rs);
		return rs;
	}
	*rdata = d;
	return GSW_statusOk;
}

static GSW_return_t smdio_reg_wr(u8 mdio_id, u16 phyaddr, u16 regaddr, u16 data)
{
	int rs;
	rs = WriteMdio(mdio_id, phyaddr, regaddr, data);
	if (rs != GSW_statusOk) {
		GSW_PRINT("%s:%d pa=%d, ra=0x%x, data=%d, rs:%d\n", __func__, __LINE__, \
			phyaddr, regaddr, data, rs);
		return rs;
	}
	return GSW_statusOk;
}

GSW_return_t gsw_reg_rd(void *pdev, u16 Offset, u16 Shift, u16 Size, ur *value)
{
	GSW_return_t s;
	u16 ro, rv = 0, mask;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL)
	if ((Offset & 0xD000) == 0xD000)
		ro = Offset;
	else
		ro = (Offset | 0xE000);

	s = smdio_reg_wr(pd->mdio_id, pd->mdio_addr, SMDIO_WRADDR, ro);
	if (s != GSW_statusOk)
		return s;
	s = smdio_reg_rd(pd->mdio_id, pd->mdio_addr, SMDIO_RDADDR, &rv);
	if (s != GSW_statusOk)
		return s;

	mask = (1 << Size) - 1 ;
	rv = (rv >> Shift);
	*value = (rv & mask);
	return GSW_statusOk;
}

GSW_return_t gsw_reg_wr(void *pdev, u16 Offset, u16 Shift, u16 Size, u32 value)
{
	GSW_return_t s;
	u16 ro, rv = 0, mask;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	if ((Offset & 0xD000) == 0xD000)
		ro = Offset;
	else
		ro = (Offset | 0xE000);

	s = smdio_reg_wr(pd->mdio_id, pd->mdio_addr, SMDIO_WRADDR, ro);
	if (s != GSW_statusOk)
		return s;

	if (Size != 16) {
		s = smdio_reg_rd(pd->mdio_id, pd->mdio_addr, SMDIO_RDADDR, &rv);
		if (s != GSW_statusOk)
			return s;
		mask = (1 << Size) - 1;
		mask = (mask << Shift);
		value = ((value << Shift) & mask);
		value = ((rv & ~mask) | value);
		s = smdio_reg_wr(pd->mdio_id, pd->mdio_addr, SMDIO_WRADDR, ro);
		if (s != GSW_statusOk)
			return s;
	}

	s = smdio_reg_wr(pd->mdio_id, pd->mdio_addr, SMDIO_RDADDR, value);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_MDIO_DataRead(void *pdev,
	GSW_MDIO_data_t *parm)
{
	GSW_return_t s;
	ur r;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	do {
		s = gsw_reg_rd(pdev,
			MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	} while (r);
	r = ((0x2 << 10) | ((parm->nAddressDev & 0x1F) << 5)
		| (parm->nAddressReg & 0x1F));
	s = gsw_reg_wr(pdev,
		MMDIO_CTRL_MBUSY_OFFSET,
		MMDIO_CTRL_REGAD_SHIFT,
		MMDIO_CTRL_REG_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	do {
		s = gsw_reg_rd(pdev,
			MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	} while (r);
	s = gsw_reg_rd(pdev,
		MMDIO_READ_RDATA_OFFSET,
		MMDIO_READ_RDATA_SHIFT,
		MMDIO_READ_RDATA_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->nData = (r & 0xFFFF);
	return GSW_statusOk;
}

GSW_return_t GSW_MDIO_DataWrite(void *pdev,
	GSW_MDIO_data_t *parm)
{
	GSW_return_t s;
	ur r;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	do {
		s = gsw_reg_rd(pdev,
			MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	} while (r);
	r = parm->nData & 0xFFFF;
	s = gsw_reg_wr(pdev,
		MMDIO_WRITE_WDATA_OFFSET,
		MMDIO_WRITE_WDATA_SHIFT,
		MMDIO_WRITE_WDATA_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	r = ((0x1 << 10) | ((parm->nAddressDev & 0x1F) << 5)
		| (parm->nAddressReg & 0x1F));
	s = gsw_reg_wr(pdev,
		MMDIO_CTRL_MBUSY_OFFSET,
		MMDIO_CTRL_REGAD_SHIFT,
		MMDIO_CTRL_REG_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	do {
		s = gsw_reg_rd(pdev,
			MMDIO_CTRL_MBUSY_OFFSET,
			MMDIO_CTRL_MBUSY_SHIFT,
			MMDIO_CTRL_MBUSY_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	} while (r);
	return GSW_statusOk;
}

GSW_return_t GSW_MmdDataRead(void *pdev,
	GSW_MMD_data_t *parm)
{
	GSW_return_t s;
	GSW_MDIO_data_t md;
	u8 found = 0, dev, pi;
	ur pn, pa, mr;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &pn);
	if (s != GSW_statusOk)
		return s;
	for (pi = 0; pi < (pn - 1); pi++) {
		s = gsw_reg_rd(pdev,
			(PHY_ADDR_0_ADDR_OFFSET - pi),
			PHY_ADDR_0_ADDR_SHIFT,
			PHY_ADDR_0_ADDR_SIZE, &pa);
		if (s != GSW_statusOk)
			return s;
		if (pa == parm->nAddressDev) {
			found = 1;
			break;
		}
	}
	if (found) {
		s = gsw_reg_rd(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, &mr);
		if (s != GSW_statusOk)
			return s;
		mr &= ~(1 << pi);
		dev = ((parm->nAddressReg >> 16) & 0x1F);
		s = gsw_reg_wr(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != GSW_statusOk)
			return s;

		udelay(800);
		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = dev;
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		md.nData = parm->nAddressReg & 0xFFFF;
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = ((0x4000) | dev);
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		s = GSW_MDIO_DataRead(pdev, &md);
		if (s != GSW_statusOk)
			return s;
		parm->nData = md.nData;

		mr |= (1 << pi);
		s = gsw_reg_wr(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != GSW_statusOk)
			return s;
	} else {
		return GSW_statusNoSupport;
	}
	return GSW_statusOk;
}

GSW_return_t GSW_MmdDataWrite(void *pdev,
	GSW_MMD_data_t *parm)
{
	GSW_return_t s;
	GSW_MDIO_data_t md;
	u32 found = 0, pn, pa, mr, dev, pi;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &pn);
	if (s != GSW_statusOk)
		return s;
	for (pi = 0; pi < pn; pi++) {
		s = gsw_reg_rd(pdev,
			(PHY_ADDR_0_ADDR_OFFSET - pi),
			PHY_ADDR_0_ADDR_SHIFT,
			PHY_ADDR_0_ADDR_SIZE, &pa);
		if (s != GSW_statusOk)
			return s;
		if (pa == parm->nAddressDev) {
			found = 1;
			break;
		}
	}
	if (found) {
		s = gsw_reg_rd(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, &mr);
		if (s != GSW_statusOk)
			return s;
		mr &= ~(1 << pi);
		dev = ((parm->nAddressReg >> 16) & 0x1F);
		s = gsw_reg_wr(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != GSW_statusOk)
			return s;

		udelay(800);
		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = dev;
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		md.nData = parm->nAddressReg & 0xFFFF;
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xd;
		md.nData = ((0x4000) | dev);
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;

		md.nAddressDev = parm->nAddressDev;
		md.nAddressReg = 0xe;
		md.nData = parm->nData;
		s = GSW_MDIO_DataWrite(pdev, &md);
		if (s != GSW_statusOk)
			return s;
		mr |= (1 << pi);
		s = gsw_reg_wr(pdev,
			MMDC_CFG_0_PEN_ALL_OFFSET,
			MMDC_CFG_0_PEN_ALL_SHIFT,
			MMDC_CFG_0_PEN_ALL_SIZE, mr);
		if (s != GSW_statusOk)
			return s;
	} else {
		return GSW_statusNoSupport;
	}
	return GSW_statusOk;
}

GSW_return_t npport(void *pdev, u8 port)
{
	ur r;
	GSW_return_t s;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	num_ports = r;
	if (port >= r)
		return GSW_statusNoSupport;
	return GSW_statusOk;
}

GSW_return_t nsqueue(void *pdev, u8 queue)
{
	ur r;
	GSW_return_t s;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_QUEUE_OFFSET,
		ETHSW_CAP_1_QUEUE_SHIFT,
		ETHSW_CAP_1_QUEUE_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (queue >= r)
		return GSW_statusValueRange;
	return GSW_statusOk;
}

GSW_return_t bmctrl_bas_busy(void *pdev)
{
	ur r;
	GSW_return_t s;
	do {
		s = gsw_reg_rd(pdev,
			BM_RAM_CTRL_BAS_OFFSET,
			BM_RAM_CTRL_BAS_SHIFT,
			BM_RAM_CTRL_BAS_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	} while (r);
	return GSW_statusOk;
}

GSW_return_t bmctrl_bas_set(void *pdev)
{
	GSW_return_t s;
	s = gsw_reg_wr(pdev,
		BM_RAM_CTRL_BAS_OFFSET,
		BM_RAM_CTRL_BAS_SHIFT,
		BM_RAM_CTRL_BAS_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t bm_ram_tbl_rd(void *pdev,
	bm_tbl_prog_t *pb)
{
	ur r;
	GSW_return_t s;
	s = gsw_reg_wr(pdev,
		BM_RAM_ADDR_ADDR_OFFSET,
		BM_RAM_ADDR_ADDR_SHIFT,
		BM_RAM_ADDR_ADDR_SIZE,
		pb->bmindex);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_CTRL_ADDR_OFFSET,
		BM_RAM_CTRL_ADDR_SHIFT,
		BM_RAM_CTRL_ADDR_SIZE,
		pb->bmtable);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_CTRL_OPMOD_OFFSET,
		BM_RAM_CTRL_OPMOD_SHIFT,
		BM_RAM_CTRL_OPMOD_SIZE,
		pb->bmopmode);
	if (s != GSW_statusOk)
		return s;
	s = bmctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = bmctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		BM_RAM_VAL_0_VAL0_OFFSET,
		BM_RAM_VAL_0_VAL0_SHIFT,
		BM_RAM_VAL_0_VAL0_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pb->bmval[0] = r;
	s = gsw_reg_rd(pdev,
		BM_RAM_VAL_1_VAL1_OFFSET,
		BM_RAM_VAL_1_VAL1_SHIFT,
		BM_RAM_VAL_1_VAL1_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pb->bmval[1] = r;

	return GSW_statusOk;
}

GSW_return_t GSW_RMON_Port_Get(void *pdev,
	GSW_RMON_Port_cnt_t *parm)
{
	u8 i, pi = parm->nPortId;
	ur r0, r1, bc;
	GSW_return_t s;
	bm_tbl_prog_t pb;
	u32 data, rf = 0, ru = 0, rm = 0, tf = 0, tu = 0, tm = 0;
	u32 rgbcl = 0, rbbcl = 0, tgbcl = 0;
	unsigned long long int rgbch = 0, rbbch = 0, tgbch = 0;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_PCFG_CNTEN_OFFSET + (pi * 2),
		BM_PCFG_CNTEN_SHIFT,
		BM_PCFG_CNTEN_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	memset(parm, 0, sizeof(GSW_RMON_Port_cnt_t));
	parm->nPortId = pi;
	s = gsw_reg_rd(pdev,
		BM_RMON_CTRL_BCAST_CNT_OFFSET + (pi * 2),
		BM_RMON_CTRL_BCAST_CNT_SHIFT,
		BM_RMON_CTRL_BCAST_CNT_SIZE, &bc);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < RMON_COUNTER_OFFSET; i++) {
		memset(&pb, 0, sizeof(bm_tbl_prog_t));
		pb.bmindex = i;
		pb.bmtable = pi;
		pb.bmopmode = BM_OPMODE_RD;
		s = bm_ram_tbl_rd(pdev, &pb);
		if (s != GSW_statusOk)
			return s;
		r0 = pb.bmval[0];
		r1 = pb.bmval[1];
		data = (r1 << 16 | r0);
		switch (i) {
		case 0x1F: /* Receive Frme Count */
			if (bc == 1)
				parm->nRxBroadcastPkts = data;
			else
				parm->nRxGoodPkts = data;
			rf = data;
			break;
		case 0x23:
			/* Receive Unicast Frame Count */
			parm->nRxUnicastPkts = data;
			ru = data;
			break;
		case 0x22:
			/* Receive Multicast Frame Count1 */
			parm->nRxMulticastPkts = data;
			rm = data;
			break;
		case 0x21:
			/* Receive CRC Errors Count */
			parm->nRxFCSErrorPkts = data;
			break;
		case 0x1D:
			/* Receive Undersize Good Count */
			parm->nRxUnderSizeGoodPkts = data;
			break;
		case 0x1B:
			/* Receive Oversize Good Count */
			parm->nRxOversizeGoodPkts = data;
			break;
		case 0x1E:
			/* Receive Undersize Bad Count */
			parm->nRxUnderSizeErrorPkts = data;
			break;
		case 0x20:
			/* Receive Pause Good Count */
			parm->nRxGoodPausePkts = data;
			break;
		case 0x1C:
			/* Receive Oversize Bad Count */
			parm->nRxOversizeErrorPkts = data;
			break;
		case 0x1A:
			/* Receive Alignment Errors Count */
			parm->nRxAlignErrorPkts = data;
			break;
		case 0x12:
			/* Receive Size 64 Frame Count1 */
			parm->nRx64BytePkts = data;
			break;
		case 0x13:
			/* Receive Size 65-127 Frame Count */
			parm->nRx127BytePkts = data;
			break;
		case 0x14:
			/* Receive Size 128-255 Frame Count */
			parm->nRx255BytePkts = data;
			break;
		case 0x15:
			/* Receive Size 256-511 Frame Count */
			parm->nRx511BytePkts = data;
			break;
		case 0x16:
			/* Receive Size 512-1023 Frame Count */
			parm->nRx1023BytePkts = data;
			break;
		case 0x17:
			/* Receive Size Greater 1023 Frame Count */
			parm->nRxMaxBytePkts = data;
			break;
		case 0x18:
			/* Receive Discard (Tail-Drop) Frame Count */
			parm->nRxDroppedPkts = data;
			break;
		case 0x19:
			/* Receive Drop (Filter) Frame Count */
			parm->nRxFilteredPkts = data;
			break;
		case 0x24:
			/* Receive Good Byte Count (Low) */
			rgbcl = data;
			break;
		case 0x25:
			/* Receive Good Byte Count (High) */
			rgbch = data;
			break;
		case 0x26:
			/* Receive Bad Byte Count (Low) */
			rbbcl = data;
			break;
		case 0x27:
			/* Receive Bad Byte Count (High) */
			rbbch = data;
			break;
		case 0x0C: /* Transmit Frame Count */
			if (bc == 1)
				parm->nTxBroadcastPkts = data;
			else
				parm->nTxGoodPkts = data;
			tf = data;
			break;
		case 0x06:
			/* Transmit Unicast Frame Count */
			parm->nTxUnicastPkts = data;
			tu = data;
			break;
		case 0x07:
			/* Transmit Multicast Frame Count1 */
			parm->nTxMulticastPkts = data;
			tm = data;
			break;
		case 0x00:
			/* Transmit Size 64 Frame Count */
			parm->nTx64BytePkts = data;
			break;
		case 0x01:
			/* Transmit Size 65-127 Frame Count */
			parm->nTx127BytePkts = data;
			break;
		case 0x02:
			/* Transmit Size 128-255 Frame Count */
			parm->nTx255BytePkts = data;
			break;
		case 0x03:
			/* Transmit Size 256-511 Frame Count */
			parm->nTx511BytePkts = data;
			break;
		case 0x04:
			/* Transmit Size 512-1023 Frame Count */
			parm->nTx1023BytePkts = data;
			break;
		case 0x05:
			/* Transmit Size Greater 1024 Frame Count */
			parm->nTxMaxBytePkts = data;
			break;
		case 0x08:
			/* Transmit Single Collision Count. */
			parm->nTxSingleCollCount = data;
			break;
		case 0x09:
			/* Transmit Multiple Collision Count */
			parm->nTxMultCollCount = data;
			break;
		case 0x0A:
			/* Transmit Late Collision Count */
			parm->nTxLateCollCount = data;
			break;
		case 0x0B:
			/* Transmit Excessive Collision.*/
			parm->nTxExcessCollCount = data;
			break;
		case 0x0D:
			/* Transmit Pause Frame Count */
			parm->nTxPauseCount = data;
			break;
		case 0x10:
			/* Transmit Drop Frame Count */
			parm->nTxDroppedPkts = data;
			break;
		case 0x0E:
			/* Transmit Good Byte Count (Low) */
			tgbcl = data;
			break;
		case 0x0F:
			/* Transmit Good Byte Count (High) */
			tgbch = data;
			break;
		case 0x11:
/* Tx Dropped Pkt cnt, based on Congestion Management.*/
			parm->nTxAcmDroppedPkts = data;
			break;
		}
	}
	if (bc == 1) {
		parm->nRxGoodPkts = rf + ru + rm;
		parm->nTxGoodPkts = tf + tu + tm;
	} else {
		/* Receive Broadcase Frme Count */
		parm->nRxBroadcastPkts = rf - ru - rm;
		/* Transmit Broadcase Frme Count */
		parm->nTxBroadcastPkts = tf - tu - tm;
	}
	/* Receive Good Byte Count */
	parm->nRxGoodBytes = (u64)(((rgbch & 0xFFFFFFFF) << 32)
		| (rgbcl & 0xFFFFFFFF));
	/* Receive Bad Byte Count */
	parm->nRxBadBytes = (u64)(((rbbch & 0xFFFFFFFF) << 32)
		| (rbbcl & 0xFFFFFFFF));
	/* Transmit Good Byte Count */
	parm->nTxGoodBytes = (u64)(((tgbch & 0xFFFFFFFF) << 32)
		| (tgbcl & 0xFFFFFFFF));
	return GSW_statusOk;
}

GSW_return_t GSW_RMON_Clear(void *pdev,
	GSW_RMON_clear_t *parm)
{
	ur pn;
	GSW_return_t s;
	u8 i, pi = parm->nRmonId;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &pn);
	if (s != GSW_statusOk)
		return s;
	switch (parm->eRmonType) {
	case GSW_RMON_ALL_TYPE:
		/* Reset all port based RMON counter */
		for (i = 0; i < pn; i++) {
			s = gsw_reg_wr(pdev,
				BM_RMON_CTRL_RAM1_RES_OFFSET + (i * 2),
				BM_RMON_CTRL_RAM1_RES_SHIFT,
				BM_RMON_CTRL_RAM1_RES_SIZE, 0x1);
			if (s != GSW_statusOk)
				return s;
			s = gsw_reg_wr(pdev,
				BM_RMON_CTRL_RAM2_RES_OFFSET + (i * 2),
				BM_RMON_CTRL_RAM2_RES_SHIFT,
				BM_RMON_CTRL_RAM2_RES_SIZE, 0x1);
			if (s != GSW_statusOk)
				return s;
		}
		break;
	case GSW_RMON_PORT_TYPE:
		s = npport(pdev, pi);
		if (s != GSW_statusOk)
			return s;
		/* Reset all RMON counter */
		s = gsw_reg_wr(pdev,
			BM_RMON_CTRL_RAM1_RES_OFFSET + (pi * 2),
			BM_RMON_CTRL_RAM1_RES_SHIFT,
			BM_RMON_CTRL_RAM1_RES_SIZE, 0x1);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			BM_RMON_CTRL_RAM2_RES_OFFSET + (pi * 2),
			BM_RMON_CTRL_RAM2_RES_SHIFT,
			BM_RMON_CTRL_RAM2_RES_SIZE, 0x1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_RMON_METER_TYPE:
	case GSW_RMON_IF_TYPE:
	case GSW_RMON_PMAC_TYPE:
	case GSW_RMON_ROUTE_TYPE:
		break;
	default:
		break;
	}
	return GSW_statusOk;
}

GSW_return_t GSW_PortCfgGet(void *pdev,
	GSW_portCfg_t *parm)
{
	u8 pi = parm->nPortId;
	GSW_return_t s;
	ur r, sn, fn;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	/* See if PORT enable or not */
	s = gsw_reg_rd(pdev,
		(SDMA_PCTRL_PEN_OFFSET + (0x6 * pi)),
		SDMA_PCTRL_PEN_SHIFT,
		SDMA_PCTRL_PEN_SIZE, &sn);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		(FDMA_PCTRL_EN_OFFSET + (0x6 * pi)),
		FDMA_PCTRL_EN_SHIFT,
		FDMA_PCTRL_EN_SIZE, &fn);
	if (s != GSW_statusOk)
		return s;
	/* Port Enable feature only support 6 port */
	 if (pi >= num_ports) {
		/* ToDo*/
		parm->eEnable = 1;
	} else {
		if ((sn == 1) && (fn == 1))
			parm->eEnable = GSW_PORT_ENABLE_RXTX;
		else if ((sn == 1) && (fn == 0))
			parm->eEnable = GSW_PORT_ENABLE_RX;
		else if ((sn == 0) && (fn == 1))
			parm->eEnable = GSW_PORT_ENABLE_TX;
		else
			parm->eEnable = GSW_PORT_DISABLE;
	}
	/* Learning Limit */
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_1_LRNLIM_OFFSET + (0xA * pi)),
		PCE_PCTRL_1_LRNLIM_SHIFT,
		PCE_PCTRL_1_LRNLIM_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->nLearningLimit = r;

	/* Learning Limit Port Lock */
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_0_PLOCK_OFFSET + (0xA * pi)),
		PCE_PCTRL_0_PLOCK_SHIFT,
		PCE_PCTRL_0_PLOCK_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bLearningMAC_PortLock = r;
	/* Aging */
	s = gsw_reg_rd(pdev,
		PCE_PCTRL_0_AGEDIS_OFFSET + (0xA * pi),
		PCE_PCTRL_0_AGEDIS_SHIFT,
		PCE_PCTRL_0_AGEDIS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bAging = r;

	/** MAC address table learning on the port specified. */
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_LNDIS_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_LNDIS_SHIFT,
		PCE_PCTRL_3_LNDIS_SIZE,
		&parm->bLearning);
	if (s != GSW_statusOk)
		return s;
	/** MAC spoofing detection. */
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_0_SPFDIS_OFFSET + (0xA * pi)),
		PCE_PCTRL_0_SPFDIS_SHIFT,
		PCE_PCTRL_0_SPFDIS_SIZE,
		&parm->bMAC_SpoofingDetection);
	if (s != GSW_statusOk)
		return s;
/* UnicastUnknownDrop */
s = gsw_reg_rd(pdev,
		PCE_PMAP_3_UUCMAP_OFFSET,
		PCE_PMAP_3_UUCMAP_SHIFT,
		PCE_PMAP_3_UUCMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	/* UnicastUnknownDrop feature  support  */
	if ((r & (1 << pi)) == 0)
		parm->bUnicastUnknownDrop = 1;
	else
		parm->bUnicastUnknownDrop = 0;
	/* MulticastUnknownDrop */
	s = gsw_reg_rd(pdev,
		PCE_PMAP_2_DMCPMAP_OFFSET,
		PCE_PMAP_2_DMCPMAP_SHIFT,
		PCE_PMAP_2_DMCPMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	/* MulticastUnknownDrop feature  support  */
	if ((r & (1 << pi)) == 0) {
		parm->bMulticastUnknownDrop = 1;
		parm->bBroadcastDrop = 1;
	} else {
		parm->bMulticastUnknownDrop = 0;
		parm->bBroadcastDrop = 0;
	}
	/* Require to check later - 3M */
	parm->bReservedPacketDrop = 0;
	/* Port Monitor */
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_TXMIR_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_TXMIR_SHIFT,
		PCE_PCTRL_3_RX_TXMIR_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->ePortMonitor = r;
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_VIO_2_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_VIO_2_SHIFT,
		PCE_PCTRL_3_VIO_2_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r == 1)
		parm->ePortMonitor |= GSW_PORT_MONITOR_VLAN_UNKNOWN;
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_VIO_4_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_VIO_4_SHIFT,
		PCE_PCTRL_3_VIO_4_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r == 1)
		parm->ePortMonitor |= GSW_PORT_MONITOR_VLAN_MEMBERSHIP;
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_VIO_5_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_VIO_5_SHIFT,
		PCE_PCTRL_3_VIO_5_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r == 1)
		parm->ePortMonitor |= GSW_PORT_MONITOR_PORT_STATE;
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_VIO_6_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_VIO_6_SHIFT,
		PCE_PCTRL_3_VIO_6_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r == 1)
		parm->ePortMonitor |= GSW_PORT_MONITOR_LEARNING_LIMIT;
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_3_VIO_7_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_VIO_7_SHIFT,
		PCE_PCTRL_3_VIO_7_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r == 1)
		parm->ePortMonitor |= GSW_PORT_MONITOR_PORT_LOCK;
	s = gsw_reg_rd(pdev,
		(MAC_PSTAT_TXPAUEN_OFFSET + (0xC * pi)),
		MAC_PSTAT_TXPAUEN_SHIFT,
		MAC_PSTAT_TXRXPAUEN_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->eFlowCtrl = r;
	return GSW_statusOk;
}

GSW_return_t GSW_PortCfgSet(void *pdev,
	GSW_portCfg_t *parm)
{
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ur r, fn, sn, pact;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	/* Learning Limit Port Lock */
	s = gsw_reg_wr(pdev,
		(PCE_PCTRL_0_PLOCK_OFFSET + (0xA * pi)),
		PCE_PCTRL_0_PLOCK_SHIFT,
		PCE_PCTRL_0_PLOCK_SIZE,
		parm->bLearningMAC_PortLock);
	if (s != GSW_statusOk)
		return s;
	/* Learning Limit Action */
	if (parm->nLearningLimit == 0)
		r = 0;
	else if (parm->nLearningLimit == 0xFFFF)
		r = 0xFF;
	else
		r = parm->nLearningLimit;
	gpc[pi].llimit = r;
	/* Learning Limit */
	s = gsw_reg_wr(pdev,
		(PCE_PCTRL_1_LRNLIM_OFFSET + (0xA * pi)),
		PCE_PCTRL_1_LRNLIM_SHIFT,
		PCE_PCTRL_1_LRNLIM_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	/** MAC address table learning on the port specified */
	s = gsw_reg_wr(pdev,
		(PCE_PCTRL_3_LNDIS_OFFSET + (0xA * pi)),
		PCE_PCTRL_3_LNDIS_SHIFT,
		PCE_PCTRL_3_LNDIS_SIZE,
		parm->bLearning);
	if (s != GSW_statusOk)
		return s;
	/** MAC spoofing detection. */
	s = gsw_reg_wr(pdev,
		(PCE_PCTRL_0_SPFDIS_OFFSET + (0xA * pi)),
		PCE_PCTRL_0_SPFDIS_SHIFT,
		PCE_PCTRL_0_SPFDIS_SIZE,
		parm->bMAC_SpoofingDetection);
	if (s != GSW_statusOk)
		return s;
	/* Aging */
	s = gsw_reg_wr(pdev,
		PCE_PCTRL_0_AGEDIS_OFFSET + (0xA * pi),
		PCE_PCTRL_0_AGEDIS_SHIFT,
		PCE_PCTRL_0_AGEDIS_SIZE,
		parm->bAging);
	if (s != GSW_statusOk)
		return s;
	/* UnicastUnknownDrop Read first */
	s = gsw_reg_rd(pdev,
		PCE_PMAP_3_UUCMAP_OFFSET,
		PCE_PMAP_3_UUCMAP_SHIFT,
		PCE_PMAP_3_UUCMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (parm->bUnicastUnknownDrop == 1)
		r &= ~(1 << pi);
	else
		r |= 1 << pi;
	/* UnicastUnknownDrop write back */
	s = gsw_reg_wr(pdev,
		PCE_PMAP_3_UUCMAP_OFFSET,
		PCE_PMAP_3_UUCMAP_SHIFT,
		PCE_PMAP_3_UUCMAP_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	/* MulticastUnknownDrop */
	s = gsw_reg_rd(pdev,
		PCE_PMAP_2_DMCPMAP_OFFSET,
		PCE_PMAP_2_DMCPMAP_SHIFT,
		PCE_PMAP_2_DMCPMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (parm->bMulticastUnknownDrop == 1)
		r &= ~(1 << pi);
	else
		r |= 1 << pi;
	/* MulticastUnknownDrop */
	s = gsw_reg_wr(pdev,
		PCE_PMAP_2_DMCPMAP_OFFSET,
		PCE_PMAP_2_DMCPMAP_SHIFT,
		PCE_PMAP_2_DMCPMAP_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	/* Flow Control */
	if (pi < num_ports) {
		s = gsw_reg_rd(pdev,
			MMDC_CFG_0_PEN_0_OFFSET,
			(MMDC_CFG_0_PEN_0_SHIFT + pi),
			MMDC_CFG_0_PEN_0_SIZE,
			&sn);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_rd(pdev,
			(MMDIO_STAT_0_PACT_OFFSET + pi),
			MMDIO_STAT_0_PACT_SHIFT,
			MMDIO_STAT_0_PACT_SIZE,
			&pact);
		if (s != GSW_statusOk)
			return s;
		if ((sn == 1) && (pact == 1)) {
			GSW_MDIO_data_t md;
			s = gsw_reg_rd(pdev,
				(PHY_ADDR_0_ADDR_OFFSET - pi),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_ADDR_SIZE,
				&r);
			if (s != GSW_statusOk)
				return s;
			md.nAddressDev = r;
			md.nAddressReg = 0x4;
			s = GSW_MDIO_DataRead(pdev, &md);
			if (s != GSW_statusOk)
				return s;
			md.nData &= ~(0xC00);
			switch (parm->eFlowCtrl) {
			case GSW_FLOW_OFF:
				break;
			case GSW_FLOW_TX:
				md.nData |= 0x800;
				break;
			case GSW_FLOW_RXTX:
				md.nData |= 0x400;
				break;
			case GSW_FLOW_RX:
			case GSW_FLOW_AUTO:
				md.nData |= 0xC00;
				break;
			}
			s = GSW_MDIO_DataWrite(pdev, &md);
			if (s != GSW_statusOk)
				return s;
			/* Restart Auto negotiation */
			md.nAddressReg = 0x0;
			s = GSW_MDIO_DataRead(pdev, &md);
			if (s != GSW_statusOk)
				return s;
			md.nData |= 0x1200;
			s = GSW_MDIO_DataWrite(pdev, &md);
			if (s != GSW_statusOk)
				return s;
		} else {
			u32 r, t;
			switch (parm->eFlowCtrl) {
			case GSW_FLOW_AUTO:
			case GSW_FLOW_OFF:
				r = 0; t = 0;
				break;
			case GSW_FLOW_RXTX:
				r = 1; t = 1;
				break;
			case GSW_FLOW_RX:
				r = 1; t = 0;
				break;
			case GSW_FLOW_TX:
				r = 0; t = 1;
				break;
			default:
				r = 0; t = 0;
			}
			s = gsw_reg_wr(pdev,
				(MAC_CTRL_0_FCON_OFFSET + (0xC * pi)),
				MAC_CTRL_0_FCON_SHIFT,
				MAC_CTRL_0_FCON_SIZE,
				parm->eFlowCtrl);
			if (s != GSW_statusOk)
				return s;
			s = gsw_reg_wr(pdev,
				(PHY_ADDR_0_FCONTX_OFFSET - (0x1 * pi)),
				PHY_ADDR_0_FCONTX_SHIFT,
				PHY_ADDR_0_FCONTX_SIZE ,
				t);
			if (s != GSW_statusOk)
				return s;
			s = gsw_reg_wr(pdev,
				(PHY_ADDR_0_FCONRX_OFFSET - (0x1 * pi)),
				PHY_ADDR_0_FCONRX_SHIFT,
				PHY_ADDR_0_FCONRX_SIZE,
				r);
			if (s != GSW_statusOk)
				return s;
		}
	}

	switch (parm->ePortMonitor) {
	case GSW_PORT_MONITOR_NONE:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_RXVMIR_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_RXVMIR_SHIFT,
			PCE_PCTRL_3_RXVMIR_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_TXMIR_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_TXMIR_SHIFT,
			PCE_PCTRL_3_TXMIR_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_2_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_2_SHIFT,
			PCE_PCTRL_3_VIO_2_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_4_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_4_SHIFT,
			PCE_PCTRL_3_VIO_4_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_5_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_5_SHIFT,
			PCE_PCTRL_3_VIO_5_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_6_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_6_SHIFT,
			PCE_PCTRL_3_VIO_6_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_7_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_7_SHIFT,
			PCE_PCTRL_3_VIO_7_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_RX:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_TXMIR_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_TXMIR_SHIFT,
			PCE_PCTRL_3_RX_TXMIR_SIZE, 2);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_TX:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_TXMIR_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_TXMIR_SHIFT,
			PCE_PCTRL_3_RX_TXMIR_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_RXTX:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_TXMIR_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_TXMIR_SHIFT,
			PCE_PCTRL_3_RX_TXMIR_SIZE, 3);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_VLAN_UNKNOWN:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_2_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_2_SHIFT,
			PCE_PCTRL_3_VIO_2_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_VLAN_MEMBERSHIP:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_4_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_4_SHIFT,
			PCE_PCTRL_3_VIO_4_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_PORT_STATE:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_5_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_5_SHIFT,
			PCE_PCTRL_3_VIO_5_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_LEARNING_LIMIT:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_6_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_6_SHIFT,
			PCE_PCTRL_3_VIO_6_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	case GSW_PORT_MONITOR_PORT_LOCK:
		s = gsw_reg_wr(pdev,
			(PCE_PCTRL_3_VIO_7_OFFSET + (0xA * pi)),
			PCE_PCTRL_3_VIO_7_SHIFT,
			PCE_PCTRL_3_VIO_7_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		break;
	}
	if (parm->eEnable == GSW_PORT_ENABLE_RXTX) {
		sn = 1; fn = 1;
	} else if (parm->eEnable == GSW_PORT_ENABLE_RX) {
		sn = 1; fn = 0;
	} else if (parm->eEnable == GSW_PORT_ENABLE_TX) {
		sn = 0; fn = 1;
	} else {
		sn = 0; fn = 0;
	}
	/* Set SDMA_PCTRL_PEN PORT enable */
	s = gsw_reg_wr(pdev,
		(SDMA_PCTRL_PEN_OFFSET + (6 * pi)),
		SDMA_PCTRL_PEN_SHIFT,
		SDMA_PCTRL_PEN_SIZE, sn);
	if (s != GSW_statusOk)
		return s;
	/* Set FDMA_PCTRL_EN PORT enable  */
	s = gsw_reg_wr(pdev,
		(FDMA_PCTRL_EN_OFFSET + (0x6 * pi)),
		FDMA_PCTRL_EN_SHIFT,
		FDMA_PCTRL_EN_SIZE, fn);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_MonitorPortCfgGet(void *pdev,
	GSW_monitorPortCfg_t *parm)
{
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ur r;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		PCE_PMAP_1_MPMAP_OFFSET ,
		PCE_PMAP_1_MPMAP_SHIFT,
		PCE_PMAP_1_MPMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (((r & (1 << pi)) >> pi) == 1)
		parm->bMonitorPort = 1;
	else
		parm->bMonitorPort = 0;
	return GSW_statusOk;
}

GSW_return_t GSW_MonitorPortCfgSet(void *pdev,
	GSW_monitorPortCfg_t *parm)
{
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ur r;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		PCE_PMAP_1_MPMAP_OFFSET ,
		PCE_PMAP_1_MPMAP_SHIFT,
		PCE_PMAP_1_MPMAP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (parm->bMonitorPort == 1)
		r |= (parm->bMonitorPort << pi);
	else
		r = (r & ~(1 << pi));
	s = gsw_reg_wr(pdev,
		PCE_PMAP_1_MPMAP_OFFSET ,
		PCE_PMAP_1_MPMAP_SHIFT,
		PCE_PMAP_1_MPMAP_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastSnoopCfgGet(void *pdev,
	GSW_multicastSnoopCfg_t *parm)
{
	u32 d0, d1, v;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	parm->eIGMP_Mode = pd->iflag.igmode;
	parm->bIGMPv3 = pd->iflag.igv3;
	parm->bCrossVLAN = pd->iflag.igxvlan;
	parm->eForwardPort = pd->iflag.igfport;
	parm->nForwardPortId = pd->iflag.igfpid;
	parm->nClassOfService = pd->iflag.igcos;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_ROB_OFFSET,
		PCE_IGMP_CTRL_ROB_SHIFT,
		PCE_IGMP_CTRL_ROB_SIZE, &v);
	if (s != GSW_statusOk)
		return s;
	parm->nRobust = v;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_DMRT_OFFSET,
		PCE_IGMP_CTRL_DMRT_SHIFT,
		PCE_IGMP_CTRL_DMRT_SIZE, &v);
	if (s != GSW_statusOk)
		return s;
	parm->nQueryInterval = v;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_REPSUP_OFFSET,
		PCE_IGMP_CTRL_REPSUP_SHIFT,
		PCE_IGMP_CTRL_REPSUP_SIZE, &d0);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_JASUP_OFFSET,
		PCE_IGMP_CTRL_JASUP_SHIFT,
		PCE_IGMP_CTRL_JASUP_SIZE, &d1);
	if (s != GSW_statusOk)
		return s;
	if (d0 == 0 && d1 == 0)
		parm->eSuppressionAggregation = GSW_MULTICAST_TRANSPARENT;
	else if (d0 == 1 && d1 == 0)
		parm->eSuppressionAggregation = GSW_MULTICAST_REPORT;
	else if (d0 == 1 && d1 == 1)
		parm->eSuppressionAggregation = GSW_MULTICAST_REPORT_JOIN;
	else
		parm->eSuppressionAggregation = GSW_MULTICAST_TRANSPARENT;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_FLEAVE_OFFSET,
		PCE_IGMP_CTRL_FLEAVE_SHIFT,
		PCE_IGMP_CTRL_FLEAVE_SIZE, &v);
	if (s != GSW_statusOk)
		return s;
	if (v == 1)
		parm->bFastLeave = 1;
	else
		parm->bFastLeave = 0;
	s = gsw_reg_rd(pd, PCE_IGMP_CTRL_SRPEN_OFFSET,
		PCE_IGMP_CTRL_SRPEN_SHIFT,
		PCE_IGMP_CTRL_SRPEN_SIZE, &v);
	if (s != GSW_statusOk)
		return s;
	parm->bLearningRouter = v;
	s = gsw_reg_rd(pd, PCE_GCTRL_1_UKIPMC_OFFSET,
		PCE_GCTRL_1_UKIPMC_SHIFT,
		PCE_GCTRL_1_UKIPMC_SIZE,
		&parm->bMulticastUnknownDrop);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t pctrl_bas_busy(void *pdev)
{
	ur r;
	GSW_return_t s;
	do {
		s = gsw_reg_rd(pdev,
			PCE_TBL_CTRL_BAS_OFFSET,
			PCE_TBL_CTRL_BAS_SHIFT,
			PCE_TBL_CTRL_BAS_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
	}	while (r);
	return GSW_statusOk;
}

GSW_return_t pctrl_reg_clr(void *pdev)
{
	GSW_return_t s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_REG_SIZE, 0);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t pctrl_addr_opmod(void *pdev,
	u16 addr, u16 opmode)
{
	ur r;
	GSW_return_t s;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_OPMOD_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	r &= ~(0x7F);
	r |= (((opmode & 3) << 5) | (addr & 0x1F));
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_ADDR_OFFSET,
		PCE_TBL_CTRL_ADDR_SHIFT,
		PCE_TBL_CTRL_ADDR_OPMOD_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t pctrl_bas_set(void *pdev)
{
	GSW_return_t s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_BAS_OFFSET,
		PCE_TBL_CTRL_BAS_SHIFT,
		PCE_TBL_CTRL_BAS_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static GSW_return_t tflow_tbl_wr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_8_KEY8_OFFSET,
		PCE_TBL_KEY_8_KEY8_SHIFT,
		PCE_TBL_KEY_8_KEY8_SIZE,
		pt->key[8]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_7_KEY7_OFFSET,
		PCE_TBL_KEY_7_KEY7_SHIFT,
		PCE_TBL_KEY_7_KEY7_SIZE,
		pt->key[7]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_6_KEY6_OFFSET,
		PCE_TBL_KEY_6_KEY6_SHIFT,
		PCE_TBL_KEY_6_KEY6_SIZE,
		pt->key[6]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_5_KEY5_OFFSET,
		PCE_TBL_KEY_5_KEY5_SHIFT,
		PCE_TBL_KEY_5_KEY5_SIZE,
		pt->key[5]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_4_KEY4_OFFSET,
		PCE_TBL_KEY_4_KEY4_SHIFT,
		PCE_TBL_KEY_4_KEY4_SIZE,
		pt->key[4]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_3_KEY3_OFFSET,
		PCE_TBL_KEY_3_KEY3_SHIFT,
		PCE_TBL_KEY_3_KEY3_SIZE,
		pt->key[3]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE,
		pt->key[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE,
		pt->key[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_6_VAL6_OFFSET,
		PCE_TBL_VAL_6_VAL6_SHIFT,
		PCE_TBL_VAL_6_VAL6_SIZE,
		pt->val[6]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_5_VAL5_OFFSET,
		PCE_TBL_VAL_5_VAL5_SHIFT,
		PCE_TBL_VAL_5_VAL5_SIZE,
		pt->val[5]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_4_VAL4_OFFSET,
		PCE_TBL_VAL_4_VAL4_SHIFT,
		PCE_TBL_VAL_4_VAL4_SIZE,
		pt->val[4]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_3_VAL3_OFFSET,
		PCE_TBL_VAL_3_VAL3_SHIFT,
		PCE_TBL_VAL_3_VAL3_SIZE,
		pt->val[3]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_KEYFORM_OFFSET,
		PCE_TBL_CTRL_KEYFORM_SHIFT,
		PCE_TBL_CTRL_KEYFORM_SIZE,
		pt->kformat);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_TYPE_OFFSET,
		PCE_TBL_CTRL_TYPE_SHIFT,
		PCE_TBL_CTRL_TYPE_SIZE,
		pt->type);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_GMAP_OFFSET,
		PCE_TBL_CTRL_GMAP_SHIFT,
		PCE_TBL_CTRL_GMAP_SIZE,
		pt->group);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

int pce_action_delete(void *pdev, tft_tbl_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PCE_TABLE_SIZE);
	memset(&tpt->pce_act[index], 0, sizeof(GSW_PCE_action_t));
	pt.pcindex = index;
	pt.table = PCE_TFLOW_INDEX;
	pt.valid = 0;
	pt.opmode = PCE_OPMODE_ADWR;
	s = tflow_tbl_wr(pdev, &pt);
	if (s != GSW_statusOk)
		return s;
	return 0;
}

static GSW_return_t ip_plen_ll_tbl_wr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE, pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int pkt_len_tdel(void *pdev,
	pcetbl_prog_t *pst, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PKG_LNG_TSIZE);
	if (pst->trc_pkg_lng[index] > 0) {
		pst->trc_pkg_lng[index]--;
		if (pst->trc_pkg_lng[index] == 0) {
			memset((((char *)pst->ct_pkg_lng)
			+ (index * sizeof(pkt_len_t))),
			 0, sizeof(pkt_len_t));
			/* initialize the data structure before using it */
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_PACKET_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = ip_plen_ll_tbl_wr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static GSW_return_t mac_dasa_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE,
		pt->key[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE,
		pt->key[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_KEYFORM_OFFSET,
		PCE_TBL_CTRL_KEYFORM_SHIFT,
		PCE_TBL_CTRL_KEYFORM_SIZE,
		pt->kformat);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_TYPE_OFFSET,
		PCE_TBL_CTRL_TYPE_SHIFT,
		PCE_TBL_CTRL_TYPE_SIZE,
		pt->type);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_GMAP_OFFSET,
		PCE_TBL_CTRL_GMAP_SHIFT,
		PCE_TBL_CTRL_GMAP_SIZE,
		pt->group);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int dst_mac_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= DASA_MAC_TBL_SIZE);
	if (tpt->dst_mac_cnt[index] > 0) {
		tpt->dst_mac_cnt[index]--;
		if (tpt->dst_mac_cnt[index] == 0) {
			memset((((char *)tpt->da_mac_tbl)
			+ (index * sizeof(mac_tbl_t))),
			0, sizeof(mac_tbl_t));
			/* initialize the data structure before using it */
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MACDA_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = mac_dasa_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static int src_mac_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= DASA_MAC_TBL_SIZE);
	if (tpt->src_mac_cnt[index] > 0) {
		tpt->src_mac_cnt[index]--;
		if (tpt->src_mac_cnt[index] == 0) {
			memset((((char *)tpt->ct_src_mac)
			+ (index * sizeof(mac_tbl_t))),
			0, sizeof(mac_tbl_t));
			/* initialize the data structure before using it */
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MACSA_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = mac_dasa_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static GSW_return_t app_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_TYPE_OFFSET,
		PCE_TBL_CTRL_TYPE_SHIFT,
		PCE_TBL_CTRL_TYPE_SIZE,
		pt->type);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int app_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PCE_APPL_TBL_SIZE);
	if (tpt->appl_tbl_cnt[index] > 0) {
		tpt->appl_tbl_cnt[index]--;
		if (tpt->appl_tbl_cnt[index] == 0) {
			memset((((char *)tpt->appl_tbl)
			+ (index * sizeof(app_tbl_t))),
			0, sizeof(app_tbl_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table	= PCE_APPLICATION_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = app_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

GSW_return_t ip_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_3_KEY3_OFFSET,
		PCE_TBL_KEY_3_KEY3_SHIFT,
		PCE_TBL_KEY_3_KEY3_SIZE,
		pt->key[3]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE,
		pt->key[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE,
		pt->key[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

int ip_dasa_msb_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= IP_DASA_MSB_SIZE);
	if (tpt->ipmsbtcnt[index] > 0) {
		tpt->ipmsbtcnt[index]--;
		if (tpt->ipmsbtcnt[index] == 0) {
			memset((((char *)tpt->ip_dasa_msb_tbl)
			+ (index * sizeof(ip_dasa_t))),
			0, sizeof(ip_dasa_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_IP_DASA_MSB_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = ip_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

int ip_dasa_lsb_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= IP_DASA_LSB_SIZE);
	if (tpt->iplsbtcnt[index] > 0) {
		tpt->iplsbtcnt[index]--;
		if (tpt->iplsbtcnt[index] == 0) {
			memset((((char *)tpt->ip_dasa_lsb_tbl)
			+ (index * sizeof(ip_dasa_t))),
			0, sizeof(ip_dasa_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_IP_DASA_LSB_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = ip_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static GSW_return_t protocol_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int ptcl_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PCE_PTCL_TBL_SIZE);
	if (tpt->ptcl_tbl_cnt[index] > 0) {
		tpt->ptcl_tbl_cnt[index]--;
		if (tpt->ptcl_tbl_cnt[index] == 0) {
			memset((((char *)tpt->ptcl_tbl)
			+ (index * sizeof(protocol_tb_t))),
			0, sizeof(protocol_tb_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_PROTOCOL_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = protocol_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static GSW_return_t pppoe_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int pppoe_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PCE_PPPOE_TBL_SIZE);
	if (tpt->pppoe_tbl_cnt[index] > 0) {
		tpt->pppoe_tbl_cnt[index]--;
		if (tpt->pppoe_tbl_cnt[index] == 0) {
			memset((((char *)tpt->pppoe_tbl)
			+ (index * sizeof(ppoe_tbl_t))),
			0, sizeof(ppoe_tbl_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_PPPOE_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = pppoe_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

static GSW_return_t actvlan_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		pt->mask[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_TYPE_OFFSET,
		PCE_TBL_CTRL_TYPE_SHIFT,
		PCE_TBL_CTRL_TYPE_SIZE,
		pt->type);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int act_vlan_tdel(void *pdev,
	pcetbl_prog_t *tpt, u32 index)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	PCE_ASSERT(index >= PCE_VLAN_ACT_TBL_SIZE);
	if (tpt->vlan_act_tbl_cnt[index] > 0) {
		tpt->vlan_act_tbl_cnt[index]--;
		if (tpt->vlan_act_tbl_cnt[index] == 0) {
			memset((((char *)tpt->avlan_tbl)
			+ (index * sizeof(avlan_tbl_t))),
			0, sizeof(avlan_tbl_t));
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_ACTVLAN_INDEX;
			pt.pcindex = index;
			pt.opmode = PCE_OPMODE_ADWR;
			s = actvlan_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
	}
	return 0;
}

int pce_pattern_delete(void *pdev, tft_tbl_t *tpt,
	u32 index)
{
	sub_tbl_t *ptable;
	PCE_ASSERT(index >= PCE_TABLE_SIZE);
	/* Check if an entry is currently programmed and remove that one. */
	if (tpt->ptblused[index] == 0)
		return 0;
	ptable = &(tpt->sub_tbl[index]);
#define TFT_IDX_DEL(x, y, z) { \
	if (x != y) \
		if (0 != z(pdev, &tpt->tsub_tbl, y)) \
			return -1; \
	}
	/* Packet length */
	TFT_IDX_DEL(0x1F, ptable->pkt_len_idx, pkt_len_tdel)
	/* Destination MAC address */
	TFT_IDX_DEL(0xFF, ptable->dst_mac_idx, dst_mac_tdel)
	/* Source MAC address */
	TFT_IDX_DEL(0xFF, ptable->src_mac_idx, src_mac_tdel)
	/* Destination Application field */
	TFT_IDX_DEL(0xFF, ptable->dst_appl_idx, app_tdel)
	/* Source Application field */
	TFT_IDX_DEL(0xFF, ptable->src_appl_idx, app_tdel)
	/* DIP MSB */
	TFT_IDX_DEL(0xFF, ptable->dip_msb_idx, ip_dasa_msb_tdel)
	/* DIP LSB */
	TFT_IDX_DEL(0xFF, ptable->dip_lsb_idx, ip_dasa_lsb_tdel)
	/* SIP MSB */
	TFT_IDX_DEL(0xFF, ptable->sip_msb_idx, ip_dasa_msb_tdel)
	/* SIP LSB */
	TFT_IDX_DEL(0xFF, ptable->sip_lsb_idx, ip_dasa_lsb_tdel)
	/* IP protocol */
	TFT_IDX_DEL(0xFF, ptable->ip_prot_idx, ptcl_tdel)
	/* Ethertype */
	TFT_IDX_DEL(0xFF, ptable->ethertype_idx, ptcl_tdel)
	/* PPPoE */
	TFT_IDX_DEL(0x1F, ptable->pppoe_idx, pppoe_tdel)
   /* VLAN */
	TFT_IDX_DEL(0x7F, ptable->vlan_idx, act_vlan_tdel)
	 /* SVLAN */
	TFT_IDX_DEL(0x7F, ptable->svlan_idx, act_vlan_tdel)
	/* reset the flag that this rule line is used */
	tpt->ptblused[index] = 0;
	/* reset the rule line */
	memset(ptable, 0xFF, sizeof(sub_tbl_t));
	return pce_action_delete(pdev, tpt, index);
}

static int find_tbl_idx(void *ts, u8 *rcnt,
	void *parm, u32 tsize, u32 tnum)
{
	int i;
	/* search if the entry is already available and can be re-used */
	for (i = 0; i < tnum; i++) {
		if (rcnt[i] > 0) {
			/* entry is used, check if the entry content fits */
			if (memcmp(((char *)ts) + i * tsize, parm, (u8)tsize) == 0) {
				rcnt[i]++;
				return i;
			}
		}
	}
	return tnum;
}

static int tbl_write(void *ts, u8 *rcnt,
	void *parm, u32 tsize, u32 tnum)
{
	int i;
	/* find an empty entry and add information */
	for (i = 0; i < tnum; i++) {
		if (rcnt[i] == 0) {
			memcpy(((char *)ts) + i * tsize, parm, (u8)tsize);
			rcnt[i]++;
			return i;
		}
	}
	/* table is full, return an error */
	GSW_PRINT("ERROR:\n\tFile %s\n\tLine %d\n", __FILE__, __LINE__);
	return GSW_statusTblFull;
}



static int pkg_len_twr(void *pdev,
	pcetbl_prog_t *pst, pkt_len_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret;
	ret = find_tbl_idx(pst->ct_pkg_lng, pst->trc_pkg_lng,
		parm, sizeof(pkt_len_t), PKG_LNG_TSIZE);
	if (ret == PKG_LNG_TSIZE) {
		ret = tbl_write(pst->ct_pkg_lng, pst->trc_pkg_lng,
			parm, sizeof(pkt_len_t), PKG_LNG_TSIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_PACKET_INDEX;
		pt.pcindex = ret;
		pt.key[0]	= parm->pkt_len;
		pt.mask[0] = parm->pkt_len_range;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = ip_plen_ll_tbl_wr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

/* MAC DA Table index write */
static int dst_mac_twr(void *pdev,
	pcetbl_prog_t *pst, mac_tbl_t *parm)
{
	pctbl_prog_t pt;
	GSW_return_t s;
	int ret;
	ret = find_tbl_idx(pst->da_mac_tbl, pst->dst_mac_cnt,
		parm, sizeof(mac_tbl_t), DASA_MAC_TBL_SIZE);
	if (ret == DASA_MAC_TBL_SIZE) {
		ret = tbl_write(pst->da_mac_tbl, pst->dst_mac_cnt,
			parm, sizeof(mac_tbl_t), DASA_MAC_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_MACDA_INDEX;
		pt.pcindex = ret;
		pt.key[0] = (parm->mac[4] << 8 | parm->mac[5]);
		pt.key[1] = (parm->mac[2] << 8 | parm->mac[3]);
		pt.key[2] = (parm->mac[0] << 8 | parm->mac[1]);
		pt.mask[0] = parm->mmask;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = mac_dasa_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

/* MAC SA Table index write */
static int src_mac_twr(void *pdev,
	pcetbl_prog_t *pst, mac_tbl_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret;
	ret = find_tbl_idx(pst->ct_src_mac, pst->src_mac_cnt,
		parm, sizeof(mac_tbl_t), DASA_MAC_TBL_SIZE);
	if (ret == DASA_MAC_TBL_SIZE) {
		ret = tbl_write(pst->ct_src_mac, pst->src_mac_cnt,
			parm, sizeof(mac_tbl_t), DASA_MAC_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_MACSA_INDEX;
		pt.pcindex = ret;
		pt.key[0] = (parm->mac[4] << 8 | parm->mac[5]);
		pt.key[1] = (parm->mac[2] << 8 | parm->mac[3]);
		pt.key[2] = (parm->mac[0] << 8 | parm->mac[1]);
		pt.mask[0] = parm->mmask;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = mac_dasa_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

static int app_twr(void *pdev,
pcetbl_prog_t *tpt, app_tbl_t *parm)
{
	pctbl_prog_t pt;
	GSW_return_t s;
	int ret;
	ret = find_tbl_idx(tpt->appl_tbl, tpt->appl_tbl_cnt,
		parm, sizeof(app_tbl_t), PCE_APPL_TBL_SIZE);
	if (ret == PCE_APPL_TBL_SIZE) {
		ret = tbl_write(tpt->appl_tbl, tpt->appl_tbl_cnt,
			parm, sizeof(app_tbl_t), PCE_APPL_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table	= PCE_APPLICATION_INDEX;
		pt.pcindex	= ret;
		pt.key[0] = parm->app_data;
		pt.mask[0] = parm->app_mr_data;
		pt.type = parm->app_mr_sel;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = app_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

/* IP DA/SA msb Table write */
GSW_return_t dasa_ip_msb_twr(void *pdev,
	pcetbl_prog_t *tpt, ip_dasa_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret, i;
	ret = find_tbl_idx(tpt->ip_dasa_msb_tbl, tpt->ipmsbtcnt,
			parm, sizeof(ip_dasa_t), IP_DASA_MSB_SIZE);
	if (ret == IP_DASA_MSB_SIZE) {
		ret = tbl_write(tpt->ip_dasa_msb_tbl, tpt->ipmsbtcnt,
			parm, sizeof(ip_dasa_t), IP_DASA_MSB_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_IP_DASA_MSB_INDEX;
		pt.pcindex	= ret;
		for (i = 0; i < 4; i++)
			pt.key[i] = ((parm->ip_val[((i*2)+1)] << 8)
			| parm->ip_val[(i*2)]);
		pt.mask[0] = parm->ip_mask[0];
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = ip_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

int dasa_lsb_twr(void *pdev,
	pcetbl_prog_t *tpt, ip_dasa_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret, i;
	ret = find_tbl_idx(tpt->ip_dasa_lsb_tbl, tpt->iplsbtcnt,
		parm, sizeof(ip_dasa_t), IP_DASA_LSB_SIZE);
	if (ret == IP_DASA_LSB_SIZE) {
		ret = tbl_write(tpt->ip_dasa_lsb_tbl, tpt->iplsbtcnt,
			parm, sizeof(ip_dasa_t), IP_DASA_LSB_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_IP_DASA_LSB_INDEX;
		pt.pcindex = ret;
		for (i = 0; i < 4; i++)
			pt.key[i]	= ((parm->ip_val[((i*2)+1)] << 8)
			| parm->ip_val[(i*2)]);
		pt.mask[0] = parm->ip_mask[0];
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = ip_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

static int protocol_twr(void *pdev,
	pcetbl_prog_t *tpt, protocol_tb_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret;
	ret = find_tbl_idx(tpt->ptcl_tbl, tpt->ptcl_tbl_cnt,
		parm, sizeof(protocol_tb_t), PCE_PTCL_TBL_SIZE);
	if (ret == PCE_PTCL_TBL_SIZE) {
		ret = tbl_write(tpt->ptcl_tbl, tpt->ptcl_tbl_cnt,
			parm, sizeof(protocol_tb_t), PCE_PTCL_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_PROTOCOL_INDEX;
		pt.pcindex = ret;
		pt.key[0] = parm->protocol_type;
		pt.mask[0] = parm->protocol_mask;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = protocol_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

static int pppoe_twr(void *pdev,
	pcetbl_prog_t *tpt, ppoe_tbl_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret;
	ret = find_tbl_idx(tpt->pppoe_tbl, tpt->pppoe_tbl_cnt,
		parm,	sizeof(ppoe_tbl_t), PCE_PPPOE_TBL_SIZE);
	if (ret == PCE_PPPOE_TBL_SIZE) {
		ret = tbl_write(tpt->pppoe_tbl, tpt->pppoe_tbl_cnt,
			parm,	sizeof(ppoe_tbl_t), PCE_PPPOE_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.table = PCE_PPPOE_INDEX;
		pt.pcindex = ret;
		pt.key[0] = parm->ppsoe_sid;
		pt.valid = 1;
		pt.opmode = PCE_OPMODE_ADWR;
		s = pppoe_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

int actvlan_twr(void *pdev, pcetbl_prog_t *tpt,
	avlan_tbl_t *parm, int range_flag, u16 range_val)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	int ret;
	ret = find_tbl_idx(tpt->avlan_tbl, tpt->vlan_act_tbl_cnt,
		parm, sizeof(avlan_tbl_t), PCE_VLAN_ACT_TBL_SIZE);
	if (ret == PCE_VLAN_ACT_TBL_SIZE) {
		ret = tbl_write(tpt->avlan_tbl, tpt->vlan_act_tbl_cnt,
			parm, sizeof(avlan_tbl_t), PCE_VLAN_ACT_TBL_SIZE);
		if (ret < 0)
			return ret;
		memset(&pt, 0, sizeof(pctbl_prog_t));
		pt.pcindex = ret;
		pt.table = PCE_ACTVLAN_INDEX;
		pt.key[0] = parm->act_vid;
		pt.mask[0] = range_val;
		pt.valid = 1;
		pt.type = range_flag;
		pt.opmode = PCE_OPMODE_ADWR;
		s = actvlan_ll_twr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
	}
	return ret;
}

int pce_rule_write(void *pdev, tft_tbl_t *tpt, GSW_PCE_rule_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	u8 idx = parm->pattern.nIndex;
	sub_tbl_t  *ptable;
	GSW_PCE_action_t *paction;
	int ti, i;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	PCE_ASSERT(idx >= PCE_TABLE_SIZE);
	if (parm->pattern.bEnable == 0)
		return pce_pattern_delete(pdev, tpt, idx);
	memset(&pt, 0, sizeof(pctbl_prog_t));
	/* Delete the old entry before adding the new one. */
	if (tpt->ptblused[idx] != 0) {
		s = pce_pattern_delete(pdev, tpt, idx);
		if (s != GSW_statusOk)
			return s;
	}
	/* Mark the entry as taken already and then program it. */
	tpt->ptblused[idx] = 1;
	/* Now convert the parameter and add to the table. */
	ptable = &(tpt->sub_tbl[idx]);
	/* Port ID */
	if (parm->pattern.bPortIdEnable == 1)
		pt.key[0] = (parm->pattern.nPortId & 0xFF);
	else
		pt.key[0] = 0xFF;
	/* DSCP value used */
	if (parm->pattern.bDSCP_Enable == 1)
		pt.key[6] |= ((parm->pattern.nDSCP & 0x3F) << 8);
	else
		pt.key[6] |= (0x7F << 8);
	/* PCP value used */
	if (parm->pattern.bPCP_Enable == 1)
		pt.key[6] |= (parm->pattern.nPCP & 0x7);
	else
		pt.key[6] |= 0xF;
	/* PCP DEI value used */
	pt.key[8] &= ~(0x1F);
	if (parm->pattern.bSTAG_PCP_DEI_Enable == 1)
		pt.key[8] |= (parm->pattern.nSTAG_PCP_DEI & 0x1F);
	else
		pt.key[8] |= 0x1F;

	/* Packet length used */
	if (parm->pattern.bPktLngEnable == 1) {
		pkt_len_t ln;
		/* Packet length */
		ln.pkt_len = parm->pattern.nPktLng;
		/* Packet length range, in number of bytes */
		ln.pkt_len_range = parm->pattern.nPktLngRange;
		ln.pkt_valid = 1;
		ti = pkg_len_twr(pdev, &tpt->tsub_tbl, &ln);
		if (ti < 0)
			return ti;
		ptable->pkt_len_idx = ti;
	} else {
		ptable->pkt_len_idx =  0x1F;
	}
	pt.key[7] = ptable->pkt_len_idx;

	/* Destination MAC address used */
	if (parm->pattern.bMAC_DstEnable == 1) {
		mac_tbl_t dmac;
		for (i = 0; i < 6; i++)
			dmac.mac[i] = parm->pattern.nMAC_Dst[i];
		dmac.mmask = parm->pattern.nMAC_DstMask;
		dmac.mac_valid = 1;
		ti = dst_mac_twr(pdev, &tpt->tsub_tbl, &dmac);
		if (ti < 0)
			return ti;
		ptable->dst_mac_idx = ti;
	} else {
		ptable->dst_mac_idx = 0xFF;
	}
	pt.key[5] |=  (ptable->dst_mac_idx << 8);

	/* Source MAC address used */
	if (parm->pattern.bMAC_SrcEnable == 1) {
		mac_tbl_t smac;
		for (i = 0; i < 6; i++)
			smac.mac[i] = parm->pattern.nMAC_Src[i];
		/* Source MAC address mask */
		smac.mmask = parm->pattern.nMAC_SrcMask;
		smac.mac_valid = 1;
		ti = src_mac_twr(pdev, &tpt->tsub_tbl, &smac);
		if (ti < 0)
			return ti;
		ptable->src_mac_idx = ti;
	} else {
		ptable->src_mac_idx = 0xFF;
	}
	pt.key[5] |= ptable->src_mac_idx;

	/* Destination Application used */
	if (parm->pattern.bAppDataMSB_Enable == 1) {
		app_tbl_t appm;
		appm.app_mr_sel = parm->pattern.bAppMaskRangeMSB_Select;
		/* Destination Application field */
		appm.app_data = parm->pattern.nAppDataMSB;
		/* Destination Application mask/range */
		appm.app_mr_data = parm->pattern.nAppMaskRangeMSB;
		appm.app_valid = 1;
		ti = app_twr(pdev, &tpt->tsub_tbl, &appm);
		if (ti < 0)
			return ti;
		ptable->dst_appl_idx = ti;
	} else {
		ptable->dst_appl_idx = 0xFF;
	}
	pt.key[4] |= ptable->dst_appl_idx;

	/* Source Application field used */
	if (parm->pattern.bAppDataLSB_Enable == 1) {
		app_tbl_t appl;
		appl.app_mr_sel =  parm->pattern.bAppMaskRangeLSB_Select;
		/* Source Application field */
		appl.app_data = parm->pattern.nAppDataLSB;
		/* Source Application mask/range */
		appl.app_mr_data = parm->pattern.nAppMaskRangeLSB;
		appl.app_valid = 1;
		ti = app_twr(pdev, &tpt->tsub_tbl, &appl);
		if (ti < 0)
			return ti;
		ptable->src_appl_idx = ti;
	} else {
		ptable->src_appl_idx = 0xFF;
	}
	pt.key[4] |= (ptable->src_appl_idx << 8);

	ptable->dip_msb_idx = 0xFF;
	ptable->dip_lsb_idx = 0xFF;
	/* DIP MSB used */
	if (parm->pattern.eDstIP_Select == /*2*/GSW_PCE_IP_V6) {
		ip_dasa_t  mtbl;
		ip_dasa_t  ltbl;
		int	j;
		memset(&mtbl, 0, sizeof(ip_dasa_t));
		memset(&ltbl, 0, sizeof(ip_dasa_t));
		/* First, search for DIP in the DA/SA table (DIP MSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			mtbl.ip_val[j-1]
			= (parm->pattern.nDstIP.nIPv6[i] & 0xFF);
			mtbl.ip_val[j]
			= ((parm->pattern.nDstIP.nIPv6[i] >> 8) & 0xFF);
		}
		mtbl.ip_mask[0] = ((parm->pattern.nDstIP_Mask >> 16) & 0xFFFF);
		mtbl.ip_valid = 1;
		ti = dasa_ip_msb_twr(pdev, &tpt->tsub_tbl, &mtbl);
		if (ti < 0)
			return ti;
		ptable->dip_msb_idx = ti;

		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			ltbl.ip_val[j-1] = (parm->pattern.nDstIP.nIPv6[i+4]
				& 0xFF);
			ltbl.ip_val[j] = ((parm->pattern.nDstIP.nIPv6[i+4] >> 8)
				& 0xFF);
		}
		ltbl.ip_mask[0] = (parm->pattern.nDstIP_Mask & 0xFFFF);
		ltbl.ip_valid = 1;
		ti = dasa_lsb_twr(pdev,	&tpt->tsub_tbl, &ltbl);
		if (ti < 0)
			return ti;
		ptable->dip_lsb_idx = ti;
	} else  if (parm->pattern.eDstIP_Select == GSW_PCE_IP_V4) {
		/* DIP LSB used */
		ip_dasa_t  ltbl;
		memset(&ltbl, 0, sizeof(ip_dasa_t));
		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0; i < 4; i++)
			ltbl.ip_val[i]
			= (((parm->pattern.nDstIP.nIPv4) >> (i * 8)) & 0xFF);
		/* DIP LSB Nibble Mask */
		ltbl.ip_mask[0] = (parm->pattern.nDstIP_Mask & 0xFFFF);
		ltbl.ip_valid = 1;
		ti = dasa_lsb_twr(pdev, &tpt->tsub_tbl, &ltbl);
		if (ti < 0)
			return ti;
		ptable->dip_lsb_idx = ti;
	}
	pt.key[3] |= (ptable->dip_msb_idx << 8);
	pt.key[3] |= ptable->dip_lsb_idx;

		/* SIP MSB used */
	ptable->sip_msb_idx = 0xFF;
	ptable->sip_lsb_idx = 0xFF;
	if (parm->pattern.eSrcIP_Select == GSW_PCE_IP_V6) {
		ip_dasa_t  mtbl;
		ip_dasa_t  ltbl;
		int	j;
		memset(&mtbl, 0, sizeof(ip_dasa_t));
		memset(&ltbl, 0, sizeof(ip_dasa_t));
		/* First, search for DIP in the DA/SA table (DIP MSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			mtbl.ip_val[j-1] = (parm->pattern.nSrcIP.nIPv6[i]
				& 0xFF);
			mtbl.ip_val[j]	= ((parm->pattern.nSrcIP.nIPv6[i] >> 8)
				& 0xFF);
		}
		mtbl.ip_mask[0] = ((parm->pattern.nSrcIP_Mask >> 16) & 0xFFFF);
		mtbl.ip_valid = 1;
		ti = dasa_ip_msb_twr(pdev, &tpt->tsub_tbl, &mtbl);
		if (ti < 0)
			return ti;
		ptable->sip_msb_idx = ti;
		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			ltbl.ip_val[j-1] = (parm->pattern.nSrcIP.nIPv6[i+4]
				& 0xFF);
			ltbl.ip_val[j] = ((parm->pattern.nSrcIP.nIPv6[i+4] >> 8)
				& 0xFF);
		}
		ltbl.ip_mask[0] = (parm->pattern.nSrcIP_Mask & 0xFFFF);
		ltbl.ip_valid = 1;
		ti = dasa_lsb_twr(pdev, &tpt->tsub_tbl, &ltbl);
		if (ti < 0)
			return ti;
		ptable->sip_lsb_idx = ti;
	} else  if (parm->pattern.eSrcIP_Select == GSW_PCE_IP_V4) {
		/* SIP LSB used */
		ip_dasa_t  ltbl;
		memset(&ltbl, 0, sizeof(ip_dasa_t));
	/* Second, search for SIP in the DA/SA table (SIP LSB) */
		for (i = 0; i < 4; i++)
			ltbl.ip_val[i]
			= ((parm->pattern.nSrcIP.nIPv4 >> (i * 8)) & 0xFF);
		/* DIP LSB Nibble Mask */
		ltbl.ip_mask[0] = (parm->pattern.nSrcIP_Mask | 0xFF00) & 0xFFFF;
		ltbl.ip_valid = 1;
		ti = dasa_lsb_twr(pdev, &tpt->tsub_tbl, &ltbl);
		if (ti < 0)
			return ti;
		ptable->sip_lsb_idx = ti;
	}
	pt.key[2] |= (ptable->sip_msb_idx << 8);
	pt.key[2] |= ptable->sip_lsb_idx;
	/* Ethertype used */
	if (parm->pattern.bEtherTypeEnable == 1) {
		protocol_tb_t et;
		memset(&et, 0, sizeof(protocol_tb_t));
		et.protocol_type = parm->pattern.nEtherType;
		et.protocol_mask = parm->pattern.nEtherTypeMask;
		et.protocol_valid = 1;
		ti = protocol_twr(pdev,	&tpt->tsub_tbl, &et);
		if (ti < 0)
			return ti;
		ptable->ethertype_idx = ti;
	} else {
		ptable->ethertype_idx = 0xFF;
	}
	pt.key[1] |= ptable->ethertype_idx;
	 if (parm->pattern.bProtocolEnable == 1) {
		protocol_tb_t pctl;
		memset(&pctl, 0, sizeof(protocol_tb_t));
		pctl.protocol_type = ((parm->pattern.nProtocol & 0xFF) << 8);
		pctl.protocol_type |= (0xFF);
		pctl.protocol_mask
		= ((parm->pattern.nProtocolMask  & 0x3) << 2);
		pctl.protocol_mask |= (0x3);
		pctl.protocol_valid = 1;
		ti = protocol_twr(pdev, &tpt->tsub_tbl, &pctl);
		if (ti < 0)
			return ti;
		ptable->ip_prot_idx = ti;
	} else {
		ptable->ip_prot_idx = 0xFF;
	}
	pt.key[1] |= (ptable->ip_prot_idx << 8);

	/* PPPoE used */
	if (parm->pattern.bSessionIdEnable == 1) {
		ppoe_tbl_t ppe;
		ppe.ppsoe_sid = parm->pattern.nSessionId;
		ppe.pppoe_valid = 1;
		ti = pppoe_twr(pdev, &tpt->tsub_tbl, &ppe);
		if (ti < 0)
			return ti;
		ptable->pppoe_idx = ti;
	} else {
		ptable->pppoe_idx = 0x1F;
	}
	pt.key[7] |= (ptable->pppoe_idx << 8);
	/* VLAN used */
	ti = 0x7F;
	if (parm->pattern.bVid == 1) {
		avlan_tbl_t avt;
		memset(&avt, 0, sizeof(avlan_tbl_t));
		avt.act_vid = parm->pattern.nVid;
		avt.avlan_valid = 1;
		ti = actvlan_twr(pdev, &tpt->tsub_tbl, &avt,
			parm->pattern.bVidRange_Select,
			parm->pattern.nVidRange);
		if (ti < 0)
			return ti;
	}
	ptable->vlan_idx = ti;
	pt.key[0] &= ~(0xFF << 8);
	/* Exclude mode for the VLAN range*/
	if ((parm->pattern.bVidRange_Select == 2) && (parm->pattern.bVid == 1))
		pt.key[0] |= (0xFE << 8);
	else
		pt.key[0] |= (ptable->vlan_idx << 8);

	ti = 0x7F;
	if (parm->pattern.bSLAN_Vid == 1) {
		avlan_tbl_t avt;
		memset(&avt, 0, sizeof(avlan_tbl_t));
		avt.act_vid = parm->pattern.nSLAN_Vid;
		avt.avlan_valid = 1;
		ti = actvlan_twr(pdev, &tpt->tsub_tbl,	&avt, 0, 0);
		if (ti < 0)
			return ti;
	}
	ptable->svlan_idx = ti;
	pt.key[8] &= ~(0xFF << 8);
	pt.key[8] |= (ptable->svlan_idx << 8);

	paction = &(tpt->pce_act[idx]);
	memcpy(paction, &parm->action, sizeof(GSW_PCE_action_t));
	if (paction->ePortMapAction
		!= GSW_PCE_ACTION_PORTMAP_DISABLE) {
		if (paction->eSnoopingTypeAction
			== GSW_PCE_ACTION_IGMP_SNOOP_DISABLE) {
			pt.val[0] = 1;
			pt.val[4] |= (0x3 << 2);
			switch (paction->ePortMapAction) {
			case GSW_PCE_ACTION_PORTMAP_REGULAR:
					pt.val[4] &= ~(0x3 << 2);
					break;
			case GSW_PCE_ACTION_PORTMAP_DISCARD:
					pt.val[1] = 0;
					break;
			case GSW_PCE_ACTION_PORTMAP_CPU:
					pt.val[1] = (paction->nForwardPortMap
						& 0xFFFF);
					pt.val[4] &= ~(0x3 << 2);
					pt.val[4] |= (0x1 << 2);
					break;
			case GSW_PCE_ACTION_PORTMAP_ALTERNATIVE:
					pt.val[1] = (paction->nForwardPortMap
						& 0xFFFF);
					break;
					/* To fix compilation warnings*/
			case GSW_PCE_ACTION_PORTMAP_DISABLE:
			case GSW_PCE_ACTION_PORTMAP_MULTICAST_ROUTER:
			case GSW_PCE_ACTION_PORTMAP_MULTICAST_HW_TABLE:
			case GSW_PCE_ACTION_PORTMAP_ALTERNATIVE_VLAN:
			case GSW_PCE_ACTION_PORTMAP_ALTERNATIVE_STAG_VLAN:
					break;
			}
		} else {
			switch (paction->eSnoopingTypeAction) {
			case GSW_PCE_ACTION_IGMP_SNOOP_REPORT:
			case GSW_PCE_ACTION_IGMP_SNOOP_LEAVE:
					pt.val[0] = 1;
					pt.val[4] &= ~(0x3 << 2);
				/* Multicast router portmap */
					pt.val[4] |= (0x1 << 2);
					break;
			default:
					pt.val[0] = 1;
					pt.val[4] &= ~(0x3 << 2);
					/* default port map */
					pt.val[4] |= (0x0 << 2);
					break;
				}
		}
	} else {
		pt.val[0] = 0;
		pt.val[1] = 0xFFFF;
		pt.val[4] &= ~(0x3 << 2);
	}
	if (paction->bFlowID_Action != 0) {
		if (paction->ePortMapAction
			== GSW_PCE_ACTION_PORTMAP_DISABLE) {
			/* enable Flow ID action */
			pt.val[4] |= (0x1 << 4);
			pt.val[1] = paction->nFlowID & 0xFFFF;
		} else {
			return GSW_statusNoSupport;
		}
	}
	/** Action "VLAN" Group. VLAN action enable */
	pt.val[2] = 0;
	/* for GSW_PCE_ACTION_VLAN_REGULAR  also*/
	pt.val[0] &= ~(1 << 13);
	if (paction->eVLAN_Action
		!= GSW_PCE_ACTION_VLAN_DISABLE) {
		pt.val[0] |= (1 << 1);
		if (paction->eVLAN_Action
			== GSW_PCE_ACTION_VLAN_ALTERNATIVE) {
			pt.val[5] |=
			((paction->nVLAN_Id & 0xFFF) << 4);
			pt.val[2]
				|= ((paction->nFId & 0xFF) << 8);
			/* alternative CTAG VLAN ID and FID */
			pt.val[5] |= (1 << 3);
			pt.val[0] |= (1 << 13);
		}
	}
	/** Action "SVLAN" Group. SVLAN action enable */
	pt.val[6] = 0;
	if (paction->eSVLAN_Action
		!= GSW_PCE_ACTION_VLAN_DISABLE) {
		pt.val[0] |= (1 << 1);
		/*  default CTAG VLAN ID and FID */
		pt.val[6] &= ~(1 << 3);
	if (paction->eSVLAN_Action
		== GSW_PCE_ACTION_VLAN_ALTERNATIVE) {
			pt.val[6]
				|= ((paction->nSVLAN_Id & 0xFFF) << 4);
			pt.val[0] |= (1 << 13);
			pt.val[6] |= (1 << 3);
		}
	}
	/** Action "Traffic Class" Group. Traffic class action enable */
	if (paction->eTrafficClassAction
		!= GSW_PCE_ACTION_TRAFFIC_CLASS_DISABLE) {
		pt.val[0] |= (1 << 2);
		switch (paction->eTrafficClassAction) {
		case GSW_PCE_ACTION_TRAFFIC_CLASS_REGULAR:
				pt.val[0] &= ~(1 << 14);
				break;
		case GSW_PCE_ACTION_TRAFFIC_CLASS_ALTERNATIVE:
				pt.val[0] |= (1 << 14);
				pt.val[3] &= ~(0xF << 8);
				pt.val[3]
				|= (paction->nTrafficClassAlternate & 0xF) << 8;
				break;
		case GSW_PCE_ACTION_TRAFFIC_CLASS_DISABLE:
				break;
		}
	} else {
		pt.val[0] &= ~((1 << 2) | (1 << 14));
		pt.val[3] |= (0xF << 8);
	}
	/** Action "Remarking" Group. Remarking action enable */
	if (paction->bRemarkAction != 0)
		pt.val[0] |= (1 << 3);
	else
		pt.val[0] &= ~(1 << 3);
	/** Action "Cross VLAN" Group. Cross VLAN action enable */
	if (paction->eVLAN_CrossAction
		!= GSW_PCE_ACTION_CROSS_VLAN_DISABLE) {
		pt.val[0] |= (1 << 4);
		if (paction->eVLAN_CrossAction
			== GSW_PCE_ACTION_CROSS_VLAN_REGULAR)
			pt.val[3] &= ~(1 << 15);
		else
			pt.val[3] |= (1 << 15);
	} else {
		pt.val[0] &= ~(1 << 4);
		pt.val[3] &= ~(1 << 15);
	}
/** Action "Cross State" Group. Cross state action control and enable */
	if (paction->eCrossStateAction
		!= GSW_PCE_ACTION_CROSS_STATE_DISABLE) {
		pt.val[0] |= (1 << 5);
		if (paction->eCrossStateAction
			== GSW_PCE_ACTION_CROSS_STATE_CROSS)
			pt.val[4] |= (1 << 13);
		else
			pt.val[4] &= ~(1 << 13);
	} else {
		pt.val[4] &= ~(1 << 13);
		pt.val[0] &= ~(1 << 5);
	}
/* Action "Critical Frames" Group. Critical Frame action control and enable*/
	if (paction->eCritFrameAction
		!= GSW_PCE_ACTION_CRITICAL_FRAME_DISABLE) {
		pt.val[0] |= (1 << 6);
		if (paction->eCritFrameAction
			== GSW_PCE_ACTION_CRITICAL_FRAME_CRITICAL)
			pt.val[4] |= (1 << 14);
		else
			pt.val[4] &= ~(1 << 14);
	} else {
		pt.val[0] &= ~(1 << 6);
		pt.val[4] &= ~(1 << 14);
	}
	/** Action "Timestamp" Group. Time stamp action control and enable */
	if (paction->eTimestampAction
		!= GSW_PCE_ACTION_TIMESTAMP_DISABLE) {
		pt.val[0] |= (1 << 7);
		if (paction->eTimestampAction
			== GSW_PCE_ACTION_TIMESTAMP_STORED)
			pt.val[4] |= (1 << 15);
		else
			pt.val[4] &= ~(1 << 15);
	} else {
		pt.val[0] &= ~(1 << 7);
		pt.val[4] &= ~(1 << 15);
	}
/** Action "Interrupt" Group. Interrupt action generate and enable */
	if (paction->eIrqAction != GSW_PCE_ACTION_IRQ_DISABLE) {
		pt.val[0] |= (1 << 8);
		if (paction->eIrqAction == GSW_PCE_ACTION_IRQ_EVENT)
			pt.val[0] |= (1 << 15);
		else
			pt.val[0] &= ~(1 << 15);
	} else {
		pt.val[0] &= ~((1 << 8) | (1 << 15));
	}
	/** Action "Learning" Group. Learning action control and enable */
	if (paction->eLearningAction
		!= GSW_PCE_ACTION_LEARNING_DISABLE) {
		pt.val[0] |= (1 << 9);
		/* Todo: Learning Rule need to be check */
		switch (paction->eLearningAction) {
		case GSW_PCE_ACTION_LEARNING_REGULAR:
				pt.val[4] &= ~0x3;
				break;
		case GSW_PCE_ACTION_LEARNING_FORCE_NOT:
				pt.val[4] =
					(pt.val[4] & ~0x3) | 0x2;
				break;
		case GSW_PCE_ACTION_LEARNING_FORCE:
				pt.val[4] |= 0x3;
				break;
		case GSW_PCE_ACTION_LEARNING_DISABLE:
				break;
		}
	} else {
		pt.val[0] &= ~(1 << 9);
		pt.val[4] &= ~0x3;
	}
	/** Action "IGMP Snooping" Group. */
	if (paction->eSnoopingTypeAction
		!= GSW_PCE_ACTION_IGMP_SNOOP_DISABLE) {
		pt.val[0] |= (1 << 10);
		pt.val[4] &= ~(0x7 << 5);
		switch (paction->eSnoopingTypeAction) {
		case GSW_PCE_ACTION_IGMP_SNOOP_REGULAR:
				pt.val[4] |= (0 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_REPORT:
				pt.val[4] |= (1 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_LEAVE:
				pt.val[4] |= (2 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_AD:
				pt.val[4] |= (3 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_QUERY:
				pt.val[4] |= (4 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_QUERY_GROUP:
				pt.val[4] |= (5 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_QUERY_NO_ROUTER:
				pt.val[4] |= (6 << 5);
				break;
		case GSW_PCE_ACTION_IGMP_SNOOP_DISABLE:
				break;
		}
	} else {
		pt.val[0] &= ~(1 << 10);
		pt.val[4] &= ~(0x7 << 5);
	}
	/** Action "Meter" Group. Meter action control and enable. */
	if (paction->eMeterAction
		!= GSW_PCE_ACTION_METER_DISABLE) {
		pt.val[0] |= (1 << 11);
		pt.val[3] = (paction->nMeterId & 0x1F);
		switch (paction->eMeterAction) {
		case GSW_PCE_ACTION_METER_REGULAR:
				pt.val[3] |= 0 << 6;
				break;
		case GSW_PCE_ACTION_METER_1:
				pt.val[3] |= 1 << 6;
				break;
/*	case GSW_PCE_ACTION_METER_2: */
/*				pt.val[3] |= 2 << 6; */
/*				break; */
		case GSW_PCE_ACTION_METER_1_2:
				pt.val[3] |= 3 << 6;
				break;
		case GSW_PCE_ACTION_METER_DISABLE:
				break;
		}
	} else {
		pt.val[0] &= ~(1 << 11);
		pt.val[3] |= 0x1F;
	}
   /** Action "RMON" Group. RMON action enable */
	if (paction->bRMON_Action != 0) {
		pt.val[0] |= (1 << 12);
		pt.val[4] &= ~(0x1F << 8);
		if (paction->nRMON_Id < 24)  {
			/*RMON_ID will support from 1 to 24 */
			pt.val[4] |= ((paction->nRMON_Id + 1) << 8);
		} else  {
			return -1;
		}
	} else {
		pt.val[0] &= ~(1 << 12);
		pt.val[4] &= ~(0x1F << 8);
	}
	pt.val[3] |= (0x7 << 12);
	if (paction->bRemarkDSCP == 1)
		pt.val[3] &= ~(1 << 12);
	if (paction->bRemarkClass == 1)
		pt.val[3] &= ~(1 << 13);
	if (paction->bRemarkPCP == 1)
		pt.val[3] &= ~(1 << 14);
	if (paction->bRemarkSTAG_PCP == 1)
		pt.val[6] &= ~(1 << 1);
	else
		pt.val[6] |= (1 << 1);
	if (paction->bRemarkSTAG_DEI == 1)
		pt.val[6] &= ~(1 << 2);
	else
		pt.val[6] |= (1 << 2);
	if (paction->bPortBitMapMuxControl == 1) {
		pt.val[6] |= (1 << 0);
		pt.val[1] = (paction->nForwardPortMap & 0xFFFF);
	}
	else
		pt.val[6] &= ~(1 << 0);
	/* Trunking action enable */
	if (paction->bPortTrunkAction == 1)
		pt.val[5] |= (1 << 0);
	else
		pt.val[5] &= ~(1 << 0);
		/* Port Link Selection control */
	if (paction->bPortLinkSelection == 1)
		pt.val[5] |= (1 << 1);
	else
		pt.val[5] &= ~(1 << 1);
	/*	CVLAN Ignore control */
	if (paction->bCVLAN_Ignore_Control == 1) {
		pt.val[5] |= (1 << 2);
			pt.val[0] |= (1 << 1);
	}	else {
		pt.val[5] &= ~(1 << 2);
	}
	pt.pcindex = idx;
	pt.table = PCE_TFLOW_INDEX;
	pt.valid = 1;
	pt.opmode = PCE_OPMODE_ADWR;
#if 0
	GSW_PRINT("\n");
	for (i = 0; i < 16; i++)
		GSW_PRINT("Val[%d]: 0x%08x, key[%d]: 0x%08x\n",
			i, pt.val[i], i, pt.key[i]);
#endif
	s = tflow_tbl_wr(pdev, &pt);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastSnoopCfgSet(void *pdev,
	GSW_multicastSnoopCfg_t *parm)
{
	GSW_return_t s;
	GSW_PCE_rule_t pr;
	u32 i, d0 = 0, d1 = 0, pmcindex = MPCE_RULES_INDEX;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	/* Choose IGMP Mode */
	switch (parm->eIGMP_Mode) {
	case GSW_MULTICAST_SNOOP_MODE_DISABLED:
		/* Snooping of Router Port Disable */
		s = gsw_reg_wr(pd,
			PCE_IGMP_CTRL_SRPEN_OFFSET,
			PCE_IGMP_CTRL_SRPEN_SHIFT,
			PCE_IGMP_CTRL_SRPEN_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pd, PCE_GCTRL_0_IGMP_OFFSET,
			PCE_GCTRL_0_IGMP_SHIFT,
			PCE_GCTRL_0_IGMP_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		for (i = 0; i <= pd->tpnum; i++) {
			s = gsw_reg_wr(pd,
				PCE_PCTRL_0_MCST_OFFSET + (0xA * i),
				PCE_PCTRL_0_MCST_SHIFT,
				PCE_PCTRL_0_MCST_SIZE, 0);
			if (s != GSW_statusOk)
				return s;
		}
		break;
	case GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING:
		/* Snooping of Router Port Enable */
		s = gsw_reg_wr(pd, PCE_GCTRL_0_IGMP_OFFSET,
			PCE_GCTRL_0_IGMP_SHIFT,
			PCE_GCTRL_0_IGMP_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_SRPEN_OFFSET,
			PCE_IGMP_CTRL_SRPEN_SHIFT,
			PCE_IGMP_CTRL_SRPEN_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		for (i = 0; i <= pd->tpnum; i++) {
			s = gsw_reg_wr(pd, PCE_PCTRL_0_MCST_OFFSET + (0xA * i),
				PCE_PCTRL_0_MCST_SHIFT,
				PCE_PCTRL_0_MCST_SIZE, 1);
			if (s != GSW_statusOk)
				return s;
		}
		break;
	case GSW_MULTICAST_SNOOP_MODE_FORWARD:
		/* Snooping of Router Port Forward */
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_SRPEN_OFFSET,
			PCE_IGMP_CTRL_SRPEN_SHIFT,
			PCE_IGMP_CTRL_SRPEN_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pd, PCE_GCTRL_0_IGMP_OFFSET,
			PCE_GCTRL_0_IGMP_SHIFT,
			PCE_GCTRL_0_IGMP_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		for (i = 0; i <= pd->tpnum; i++) {
			s = gsw_reg_wr(pd, PCE_PCTRL_0_MCST_OFFSET + (0xA * i),
				PCE_PCTRL_0_MCST_SHIFT,
				PCE_PCTRL_0_MCST_SIZE, 1);
			if (s != GSW_statusOk)
				return s;
		}
		break;
	default:
			GSW_PRINT("This Mode doesn't exists\n");
			return GSW_statusErr;
	}
	/* Set the Flag for eIGMP_Mode flag*/
	pd->iflag.igmode = parm->eIGMP_Mode;
	/* Set bIGMPv3 flag*/
	pd->iflag.igv3 =  parm->bIGMPv3;
	/* Set bCrossVLAN flag*/
	pd->iflag.igxvlan = parm->bCrossVLAN;
	/* Set eForwardPort flag */
	pd->iflag.igfport = parm->eForwardPort;
	/* Set nForwardPortId */
	if (parm->eForwardPort == GSW_PORT_FORWARD_CPU)
		pd->iflag.igfpid = (1 << pd->cport);
	else
		pd->iflag.igfpid = parm->nForwardPortId;
	pd->iflag.igcos = parm->nClassOfService;
/* If IGMP mode set to AutoLearning then the following Rule have to add it */
	if (parm->eIGMP_Mode ==
		GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		for (i = pmcindex; i <= (pmcindex + 7); i++) {
			memset(&pr, 0, sizeof(GSW_PCE_rule_t));
			pr.pattern.nIndex = i;
			pr.pattern.bEnable = 1;
			pr.pattern.bAppDataMSB_Enable = 1;
			if ((i == pmcindex + 0) ||
				(i == pmcindex + 1) ||
				(i == pmcindex + 2))
				pr.pattern.nAppDataMSB = 0x1100;
			else if (i == pmcindex + 3)
				pr.pattern.nAppDataMSB = 0x1200;
			else if (i == pmcindex + 4)
				pr.pattern.nAppDataMSB = 0x1600;
			else if (i == pmcindex + 5)
				pr.pattern.nAppDataMSB = 0x1700;
			else if (i == pmcindex + 6)
				pr.pattern.nAppDataMSB = 0x3100;
			else if (i == pmcindex + 7)
				pr.pattern.nAppDataMSB = 0x3000;

			pr.pattern.bAppMaskRangeMSB_Select = 0;
			pr.pattern.nAppMaskRangeMSB = 0x3;
			if ((i == pmcindex + 0) ||
				(i == pmcindex + 1) ||
				(i == pmcindex + 6) ||
				(i == pmcindex + 7))
				pr.pattern.eDstIP_Select = 1;
			if ((i == pmcindex + 0) ||
				(i == pmcindex + 1))
				pr.pattern.nDstIP.nIPv4 = 0xE0000001;
			else if (i == pmcindex + 6)
				pr.pattern.nDstIP.nIPv4 = 0xE0000002;
			else if (i == pmcindex + 7)
				pr.pattern.nDstIP.nIPv4 = 0xE00000A6;
			pr.pattern.nDstIP_Mask = 0xFF00;
			if (i == pmcindex + 1)
				pr.pattern.eSrcIP_Select = 1;
			else
				pr.pattern.eSrcIP_Select = 0;
			if (i == pmcindex + 1)
				pr.pattern.nSrcIP_Mask = 0xFF00;
			else
				pr.pattern.nSrcIP_Mask = 0xFFFF;
			pr.pattern.bProtocolEnable = 1;
			pr.pattern.nProtocol = 0x2;
			if (pd->iflag.igcos == 0) {
				pr.action.eTrafficClassAction = 0;
				pr.action.nTrafficClassAlternate = 0;
			} else {
				pr.action.eTrafficClassAction = 1;
				pr.action.nTrafficClassAlternate =
					pd->iflag.igcos;
			}
			if (i == pmcindex + 0)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_QUERY;
			else if (i == pmcindex + 1)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_QUERY_NO_ROUTER;
			else if (i == pmcindex + 2)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_QUERY_GROUP;
			else if (i == pmcindex + 3)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_REPORT;
			else if (i == pmcindex + 4)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_REPORT;
			else if (i == pmcindex + 5)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_LEAVE;
			else if (i == pmcindex + 6)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_AD;
			else if (i == pmcindex + 7)
				pr.action.eSnoopingTypeAction =
				GSW_PCE_ACTION_IGMP_SNOOP_AD;
			pr.action.ePortMapAction =
			GSW_PCE_ACTION_PORTMAP_MULTICAST_ROUTER;
			if (parm->bCrossVLAN)
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_CROSS;
			else
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_DISABLE;
			/* We prepare everything and write into PCE Table */
			if (0 != pce_rule_write(pd,
				&pd->ptft, &pr))
				return GSW_statusErr;
		}
	}
	/* If IGMP mode set to forwarding then the */
	/* following Rule have to add it */
	if (parm->eIGMP_Mode == GSW_MULTICAST_SNOOP_MODE_FORWARD) {
		for (i = pmcindex; i <= (pmcindex + 7); i++) {
			memset(&pr, 0, sizeof(GSW_PCE_rule_t));
			pr.pattern.nIndex = i;
			pr.pattern.bEnable = 1;
			pr.pattern.bProtocolEnable = 1;
			switch (i - pmcindex) {
/*		case 0: */
/*Rule added by Router port ADD function based on router port for IPv4*/
/*					break; */
			case 1:
/*	Avoid IGMP Packets Redirection when seen on Management Port */
				pr.pattern.nProtocol = 0x2; /* for IPv4 */
				pr.pattern.bPortIdEnable = 1;
	/* Action Enabled, no redirection (default portmap) */
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_REGULAR;
				break;
			case 2:
				/* IGMPv1/2/3 IPv4 */
				pr.pattern.nProtocol = 0x2; /* for IPv4 */
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
				break;
/*		case 3: */
	/*Rules added by Router port ADD function */
	/* based on router port for IPv6 */
/*			break; */
			case 4:
	/*	Managemnt Port Remaining ICMPv6/MLD packets */
	/* (Avoiding Loopback and Disacard) */
				pr.pattern.bPortIdEnable = 1;
				pr.pattern.nPortId = parm->nForwardPortId;
				pr.pattern.nProtocol = 0x3A;  /*for IPv6*/
				pr.pattern.bPortIdEnable = 1;
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_REGULAR;
				break;
			case 5:
	/* ICMPv6 Multicast Listener Query/Report/Done(Leave) */
				pr.pattern.bAppDataMSB_Enable	= 1;
				pr.pattern.bAppMaskRangeMSB_Select = 1;
				pr.pattern.nAppDataMSB = 0x8200;
				pr.pattern.nAppMaskRangeMSB	= 0x2FF;
				pr.pattern.nProtocol = 0x3A;  /*for IPv6*/
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
				break;
			case 6:
	/* ICMPv6 Multicast Listener Report */
				pr.pattern.bAppDataMSB_Enable	= 1;
				pr.pattern.nAppDataMSB = 0x8F00;
				pr.pattern.nAppMaskRangeMSB = 0x3;
				pr.pattern.nProtocol = 0x3A;  /*for IPv6*/
				pr.action.ePortMapAction =
					GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
				break;
			case 7:
	/* ICMPv6 Multicast Router Advertisement/Solicitation/Termination */
				pr.pattern.bAppDataMSB_Enable	= 1;
				pr.pattern.bAppMaskRangeMSB_Select = 1;
				pr.pattern.nAppDataMSB = 0x9700;
				pr.pattern.nAppMaskRangeMSB	= 0x2FF;
				pr.pattern.nProtocol = 0x3A;  /*for IPv6*/
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
				break;
			default:
				continue;
			}
			if (pd->iflag.igcos != 0) {
				pr.action.eTrafficClassAction = 1;
				pr.action.nTrafficClassAlternate =
				pd->iflag.igcos;
			}
			/*  Set eForwardPort */
			if (parm->eForwardPort == GSW_PORT_FORWARD_PORT) {
				pr.action.nForwardPortMap =
				(1 << parm->nForwardPortId);
				pr.pattern.nPortId = parm->nForwardPortId;
			} else if (parm->eForwardPort == GSW_PORT_FORWARD_CPU) {
				pr.action.nForwardPortMap =
				(1 << pd->cport);
				pr.pattern.nPortId = pd->cport;
			}
			if (parm->bCrossVLAN)
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_CROSS;
			else
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_DISABLE;
			/* We prepare everything and write into PCE Table */
			if ((pce_rule_write(pd,
				&pd->ptft, &pr)) != 0)
				return GSW_statusErr;
		}

	}
	if (parm->eIGMP_Mode ==
		GSW_MULTICAST_SNOOP_MODE_DISABLED) {
		pmcindex = MPCE_RULES_INDEX;
		for (i = pmcindex; i <= (pmcindex + 7); i++) {
			pr.pattern.nIndex = i;
			pr.pattern.bEnable = 0;
			/* We prepare everything and write into PCE Table */
			if (0 != pce_pattern_delete(pd, &pd->ptft, i))
				return GSW_statusErr;
		}
	}
	if (parm->nRobust < 4) {
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_ROB_OFFSET,
			PCE_IGMP_CTRL_ROB_SHIFT,
			PCE_IGMP_CTRL_ROB_SIZE,
			parm->nRobust);
		if (s != GSW_statusOk)
			return s;
	} else {
		GSW_PRINT("The Robust time would only support 0..3\n");
		return GSW_statusErr;
	}
	s = gsw_reg_wr(pd, PCE_IGMP_CTRL_DMRTEN_OFFSET,
		PCE_IGMP_CTRL_DMRTEN_SHIFT,
		PCE_IGMP_CTRL_DMRTEN_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pd, PCE_IGMP_CTRL_DMRT_OFFSET,
		PCE_IGMP_CTRL_DMRT_SHIFT,
		PCE_IGMP_CTRL_DMRT_SIZE,
		parm->nQueryInterval);
	if (s != GSW_statusOk)
		return s;

	if (parm->eIGMP_Mode ==
		GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		switch (parm->eSuppressionAggregation) {
		case GSW_MULTICAST_REPORT_JOIN:
			d1 = 1;	d0 = 1;
			break;
		case GSW_MULTICAST_REPORT:
			d1 = 0;	d0 = 1;
			break;
		case GSW_MULTICAST_TRANSPARENT:
			d1 = 0;	d0 = 0;
			break;
		}
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_REPSUP_OFFSET,
			PCE_IGMP_CTRL_REPSUP_SHIFT,
			PCE_IGMP_CTRL_REPSUP_SIZE, d0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_JASUP_OFFSET,
			PCE_IGMP_CTRL_JASUP_SHIFT,
			PCE_IGMP_CTRL_JASUP_SIZE, d1);
		if (s != GSW_statusOk)
			return s;
	}

	if (parm->eIGMP_Mode ==
		GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_SRPEN_OFFSET,
			PCE_IGMP_CTRL_SRPEN_SHIFT,
			PCE_IGMP_CTRL_SRPEN_SIZE,
			parm->bLearningRouter);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_FLEAVE_OFFSET,
			PCE_IGMP_CTRL_FLEAVE_SHIFT,
			PCE_IGMP_CTRL_FLEAVE_SIZE, parm->bFastLeave);
		if (s != GSW_statusOk)
			return s;
	} else {
		s = gsw_reg_wr(pd, PCE_IGMP_CTRL_FLEAVE_OFFSET,
			PCE_IGMP_CTRL_FLEAVE_SHIFT,
			PCE_IGMP_CTRL_FLEAVE_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
	}
	s = gsw_reg_wr(pd, PCE_GCTRL_1_UKIPMC_OFFSET,
		PCE_GCTRL_1_UKIPMC_SHIFT,
		PCE_GCTRL_1_UKIPMC_SIZE,
		parm->bMulticastUnknownDrop);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastRouterPortAdd(void *pdev,
	GSW_multicastRouter_t *parm)
{
	GSW_return_t s;
	u32 v;
	u8 pi = parm->nPortId;
	ethsw_api_dev_t	*pd = (ethsw_api_dev_t *)pdev;
	gsw_igmp_t *hitbl = &pd->iflag;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pd, pi);
	if (s != GSW_statusOk)
		return s;
	/* Read the Default Router Port Map - DRPM */
	s = gsw_reg_rd(pd, PCE_IGMP_DRPM_DRPM_OFFSET,
		PCE_IGMP_DRPM_DRPM_SHIFT,
		PCE_IGMP_DRPM_DRPM_SIZE, &v);
	if (s != GSW_statusOk)
		return s;
	if (((v >> pi) & 0x1) == 1) {
		/*"Error: the prot was already in the member */
		return GSW_statusParam;
	} else {
		v = (v | (1 << pi));
		/* Write the Default Router Port Map - DRPM  */
		s = gsw_reg_wr(pd, PCE_IGMP_DRPM_DRPM_OFFSET,
			PCE_IGMP_DRPM_DRPM_SHIFT,
			PCE_IGMP_DRPM_DRPM_SIZE, v);
		if (s != GSW_statusOk)
			return s;
	}
	if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_FORWARD) {
		GSW_PCE_rule_t pr;
		u8 i;
		for (i = 0; i < 2; i++) {
			memset(&pr, 0, sizeof(GSW_PCE_rule_t));
			pr.pattern.bEnable = 1;
			pr.pattern.bProtocolEnable = 1;
			switch (i) {
			case 0:
	/*	Management port remaining IGMP packets (forwarding */
	/* them to Router Ports) */
				pr.pattern.nIndex = MPCE_RULES_INDEX;
				pr.pattern.nProtocol = 0x2; /* for IPv4 */
				pr.pattern.bAppMaskRangeMSB_Select = 1;
				pr.pattern.bAppDataMSB_Enable	= 1;
				pr.pattern.nAppDataMSB = 0x1200;
				pr.pattern.nAppMaskRangeMSB	= 0x1DFF;
				break;
			case 1:
	/* Management Port ICMPv6 Multicast Listerner Report */
	/* & Leave (Avoiding Loopback abd Discard) */
				pr.pattern.nIndex = MPCE_RULES_INDEX + 3;
				pr.pattern.bAppDataMSB_Enable	= 1;
				pr.pattern.bAppMaskRangeMSB_Select = 1;
				pr.pattern.nAppDataMSB = 0x8300;
				pr.pattern.nAppMaskRangeMSB	= 0x1FF;
				pr.pattern.nProtocol = 0x3A;  /*for IPv6*/
				pr.action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
				pr.action.nForwardPortMap = v;
				break;
			}
			/* Router portmap */
			pr.action.ePortMapAction =
			GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
			pr.action.nForwardPortMap = v;
			if (hitbl->igcos != 0) {
				pr.action.eTrafficClassAction = 1;
				pr.action.nTrafficClassAlternate = pd->iflag.igcos;
			}
			/*  Set eForwardPort */
			pr.pattern.bPortIdEnable = 1;
			if (hitbl->igfport == GSW_PORT_FORWARD_PORT)
				pr.pattern.nPortId = hitbl->igfpid;
			else if (hitbl->igfport == GSW_PORT_FORWARD_CPU)
				pr.pattern.nPortId = (pd->cport);

			if (hitbl->igxvlan)
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_CROSS;
			else
				pr.action.eVLAN_CrossAction =
				GSW_PCE_ACTION_CROSS_VLAN_DISABLE;
			/* We prepare everything and write into PCE Table */
			if (0 != pce_rule_write(pd, &pd->ptft, &pr))
				return GSW_statusErr;
		}
	}
	return GSW_statusOk;
}

GSW_return_t gsw_mcast_tbl_rd(void *pdev,
	pctbl_prog_t *pt)
{
	ur r;
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->key[2] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->key[1] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->key[0] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_4_VAL4_OFFSET,
		PCE_TBL_VAL_4_VAL4_SHIFT,
		PCE_TBL_VAL_4_VAL4_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[4] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_3_VAL3_OFFSET,
		PCE_TBL_VAL_3_VAL3_SHIFT,
		PCE_TBL_VAL_3_VAL3_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[3] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[2] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[1] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[0] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->valid = r;
	return GSW_statusOk;
}

static GSW_return_t mcast_tbl_wr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE,
		pt->key[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE,
		pt->key[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		pt->key[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_4_VAL4_OFFSET,
		PCE_TBL_VAL_4_VAL4_SHIFT,
		PCE_TBL_VAL_4_VAL4_SIZE,
		pt->val[4]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_3_VAL3_OFFSET,
		PCE_TBL_VAL_3_VAL3_SHIFT,
		PCE_TBL_VAL_3_VAL3_SIZE,
		pt->val[3]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		pt->valid);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static int get_tbl_index(void *ts, void *parm,
	u32 tsize, u32 tnum)
{
	int i;
	/* search if the entry is already available and can be re-used */
	for (i = 0; i < tnum; i++) {
		/* entry is used, check if the entry content fits */
		if (memcmp(((char *)ts) + i * tsize, parm, (u8)tsize) == 0)
			return i;
	}
	return 0xFF;
}

int find_msb_tbl_entry(pcetbl_prog_t *tpt,
	ip_dasa_t *parm)
{
	return get_tbl_index(tpt->ip_dasa_msb_tbl, parm,
		sizeof(ip_dasa_t), IP_DASA_MSB_SIZE);
}

int find_dasa_tbl_entry(pcetbl_prog_t *tpt,
	ip_dasa_t *parm)
{
	return get_tbl_index(tpt->ip_dasa_lsb_tbl, parm,
		sizeof(ip_dasa_t), IP_DASA_LSB_SIZE);
}

static int gsw2x_msw_table_wr(void *pdev,
	GSW_multicastTable_t *parm)
{
	GSW_return_t s;
	int	i, j, exclude_rule = 0;
	int	dlix = 0x7F, dmix = 0x7F;
	int slix = 0x7F, smix = 0x7F;
	int dlflag = 0, dmflag = 0, slflag = 0, smflag = 0;
	ltq_bool_t new_entry = 0;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	tft_tbl_t *hpctbl = &pd->ptft;
	gsw_igmp_t *hitbl = &pd->iflag;
	pctbl_prog_t pt;
	ip_dasa_t	ltbl;
	ip_dasa_t	mtbl;
	SWAPI_ASSERT(pd == NULL);
	memset(&pt, 0, sizeof(pctbl_prog_t));
	memset(&ltbl, 0, sizeof(ip_dasa_t));
	memset(&mtbl, 0, sizeof(ip_dasa_t));
	if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
		|| (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
		if (pd->iflag.igv3 != 1)
			return GSW_statusParam;
	}
	if ((parm->eIPVersion != GSW_IP_SELECT_IPV4)
		&& (parm->eIPVersion != GSW_IP_SELECT_IPV6)) {
		GSW_PRINT("(IPv4/IPV6 need to enable)\n");
		return GSW_statusErr;
	}
	if (parm->eIPVersion == GSW_IP_SELECT_IPV4) {
		for (i = 0; i < 4; i++)
			ltbl.ip_val[i] =
			((parm->uIP_Gda.nIPv4 >> (i * 8)) & 0xFF);
		ltbl.ip_mask[0] = 0xFF00;
	}
	if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			mtbl.ip_val[j-1] = (parm->uIP_Gda.nIPv6[i] & 0xFF);
			mtbl.ip_val[j] = ((parm->uIP_Gda.nIPv6[i] >> 8) & 0xFF);
		}
		mtbl.ip_mask[0] = 0;
		mtbl.ip_valid = 1;
		dmix = find_msb_tbl_entry(&hpctbl->tsub_tbl, &mtbl);
		if (dmix == 0xFF) {
			dmix = dasa_ip_msb_twr(pdev, &hpctbl->tsub_tbl, &mtbl);
			dmflag = 1;
		}
		if (dmix < 0) {
			GSW_PRINT("%s:%s:%d (IGMP Table full)\n",
				__FILE__, __func__, __LINE__);
			return GSW_statusErr;
		}
		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			ltbl.ip_val[j-1] = (parm->uIP_Gda.nIPv6[i+4] & 0xFF);
			ltbl.ip_val[j] = ((parm->uIP_Gda.nIPv6[i+4] >> 8) & 0xFF);
		}
		ltbl.ip_mask[0] = 0;/* DIP LSB Nibble Mask */
	}
	ltbl.ip_valid = 1;
	dlix = find_dasa_tbl_entry(&hpctbl->tsub_tbl, &ltbl);
	if (dlix == 0xFF) {
		dlix = dasa_lsb_twr(pdev, &hpctbl->tsub_tbl, &ltbl);
		dlflag = 1;
	}
	if (dlix < 0) {
		GSW_PRINT("%s:%s:%d (IGMP Table full)\n",
			__FILE__, __func__, __LINE__);
		return GSW_statusErr;
	}
	if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
		|| (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
		if (parm->eIPVersion == GSW_IP_SELECT_IPV4) {
			for (i = 0; i < 4; i++)
				ltbl.ip_val[i] =
				((parm->uIP_Gsa.nIPv4 >> (i * 8)) & 0xFF);
				/* DIP LSB Nibble Mask */
			ltbl.ip_mask[0] = 0xFF00;
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if ((ltbl.ip_val[3] == 0) &&
					(ltbl.ip_val[2] == 0) &&
					(ltbl.ip_val[1] == 0) &&
					(ltbl.ip_val[0] == 0)) {
						GSW_PRINT("%s:%s:%d (Exclude Rule Source IP is Wildcard)\n",
						__FILE__, __func__, __LINE__);
						return GSW_statusErr;
				}
			}
		}
		if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
			int src_zero = 0;
		/*First, search for DIP in the DA/SA table (DIP MSB)*/
			for (i = 0, j = 7; i < 4; i++, j -= 2) {
				mtbl.ip_val[j-1] =
				(parm->uIP_Gsa.nIPv6[i] & 0xFF);
				mtbl.ip_val[j] =
				((parm->uIP_Gsa.nIPv6[i] >> 8) & 0xFF);
			}
			mtbl.ip_mask[0] = 0;/* DIP MSB Nibble Mask */
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if ((mtbl.ip_val[0] == 0) &&
					(mtbl.ip_val[1] == 0) &&
					(mtbl.ip_val[2] == 0) &&
					(mtbl.ip_val[3] == 0) &&
					(mtbl.ip_val[4] == 0) &&
					(mtbl.ip_val[5] == 0) &&
					(mtbl.ip_val[6] == 0) &&
					(mtbl.ip_val[7] == 0)) {
						src_zero = 1;
				}
			}
			/* First, search for DIP in the DA/SA table (DIP LSB) */
			for (i = 0, j = 7; i < 4; i++, j -= 2) {
				ltbl.ip_val[j-1] =
				(parm->uIP_Gsa.nIPv6[i+4] & 0xFF);
				ltbl.ip_val[j] =
				((parm->uIP_Gsa.nIPv6[i+4] >> 8) & 0xFF);
			}
			ltbl.ip_mask[0] = 0;/* DIP LSB Nibble Mask */
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if ((ltbl.ip_val[0] == 0) &&
					(ltbl.ip_val[1] == 0) &&
					(ltbl.ip_val[2] == 0) &&
					(ltbl.ip_val[3] == 0) &&
					(ltbl.ip_val[4] == 0) &&
					(ltbl.ip_val[5] == 0) &&
					(ltbl.ip_val[6] == 0) &&
					(ltbl.ip_val[7] == 0)) {
					if (src_zero) {
						GSW_PRINT("%s:%s:%d (Exclude Rule Source IP is Wildcard)\n",
						__FILE__, __func__, __LINE__);
						return GSW_statusErr;
					}
				}
			}
			mtbl.ip_valid = 1;
			smix = find_msb_tbl_entry(&hpctbl->tsub_tbl, &mtbl);
			if (smix == 0xFF) {
				smix = dasa_ip_msb_twr(pdev, &hpctbl->tsub_tbl, &mtbl);
				smflag = 1;
			}
			if (smix < 0) {
				GSW_PRINT("%s:%s:%d (IGMP Table full)\n",
					__FILE__, __func__, __LINE__);
				return GSW_statusErr;
			}
		}
		ltbl.ip_valid = 1;
		slix = find_dasa_tbl_entry(&hpctbl->tsub_tbl, &ltbl);
		if (slix == 0xFF) {
			slix = dasa_lsb_twr(pdev,
				&hpctbl->tsub_tbl, &ltbl);
			slflag = 1;
		}
		if (slix < 0) {
			GSW_PRINT("%s:%s:%d (IGMP Table full)\n",
				__FILE__, __func__, __LINE__);
			return GSW_statusErr;
		}
	}
	/* update the entry for another port number if already exists*/
	for (i = 0; i < pd->iflag.itblsize; i++) {
		/* Check if port was already exist */
		if ((hitbl->mctable[i].dlsbindex == dlix) &&
			(hitbl->mctable[i].dmsbindex == dmix) &&
			(hitbl->mctable[i].slsbindex == slix) &&
			(hitbl->mctable[i].smsbindex == smix) &&
			(hitbl->mctable[i].valid == 1)) {
			if (((hitbl->mctable[i].pmap >> parm->nPortId)
				& 0x1) == 1)
				return GSW_statusOk;
			switch (hitbl->mctable[i].mcmode) {
			case GSW_IGMP_MEMBER_DONT_CARE:
				SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
				hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
				if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
					SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
					hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
				}
				/* Add the port */
				hitbl->mctable[i].pmap |= (1 << parm->nPortId);
				break;
			case GSW_IGMP_MEMBER_EXCLUDE:
					exclude_rule = 1;
			case GSW_IGMP_MEMBER_INCLUDE:
				/* Add the port */
				hitbl->mctable[i].pmap |= (1 << parm->nPortId);
				SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
				SWAPI_ASSERT(slix >=  IP_DASA_LSB_SIZE);
				hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
				hpctbl->tsub_tbl.iplsbtcnt[slix]++;
				if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
					SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
					SWAPI_ASSERT(smix >=  IP_DASA_MSB_SIZE);
					hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
					hpctbl->tsub_tbl.ipmsbtcnt[smix]++;
				}
				break;
			} /* end switch */
			/* Now, we write into Multicast SW Table */
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MULTICAST_SW_INDEX;
			pt.pcindex = i;
			pt.key[1] = (hitbl->mctable[i].smsbindex << 8)
				| hitbl->mctable[i].slsbindex;
			pt.key[0] = (hitbl->mctable[i].dmsbindex << 8)
				| hitbl->mctable[i].dlsbindex;
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)
				pt.val[0] = (0 << parm->nPortId);
			else
				pt.val[0] = hitbl->mctable[i].pmap;
			pt.valid = hitbl->mctable[i].valid;
			pt.opmode = PCE_OPMODE_ADWR;
			s = mcast_tbl_wr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			new_entry = 1;
			if (exclude_rule == 0)
				return GSW_statusOk;
		}
	}

	/* wildcard entry for EXCLUDE rule for  port number if already exists*/
	if ((exclude_rule == 1) && (new_entry == 1)) {
		for (i = 0; i < pd->iflag.itblsize; i++) {
			/* Check if port was already exist */
			if ((hitbl->mctable[i].dlsbindex == dlix) &&
			(hitbl->mctable[i].dmsbindex == dmix) &&
			(hitbl->mctable[i].slsbindex == 0x7F) &&
			(hitbl->mctable[i].smsbindex == 0x7F) &&
			(hitbl->mctable[i].valid == 1)) {
				if (((hitbl->mctable[i].pmap >>
					parm->nPortId) & 0x1) == 1) {
					return GSW_statusOk;
				} else {
					SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
					hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
					if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
						SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
						hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
					}
					/* Add the port */
					hitbl->mctable[i].pmap |=
					(1 << parm->nPortId);
				}
				hitbl->mctable[i].mcmode =
				GSW_IGMP_MEMBER_DONT_CARE;
				memset(&pt, 0, sizeof(pctbl_prog_t));
				pt.table = PCE_MULTICAST_SW_INDEX;
				pt.pcindex = i;
				pt.key[1] =
				((hitbl->mctable[i].smsbindex << 8)
					| (hitbl->mctable[i].slsbindex));
				pt.key[0] =
				((hitbl->mctable[i].dmsbindex << 8)
					| (hitbl->mctable[i].dlsbindex));
				pt.val[0] = hitbl->mctable[i].pmap;
				pt.valid = hitbl->mctable[i].valid;
				pt.opmode = PCE_OPMODE_ADWR;
				s = mcast_tbl_wr(pdev, &pt);
				if (s != GSW_statusOk)
					return s;
				return GSW_statusOk;
			}
		}
	}
/*	}*/
/* Create the new DstIP & SrcIP entry */
	if (new_entry == 0) {
		if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
			|| (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
			i = 0;
			while (i < pd->iflag.itblsize) {
				/* Find a new empty entry to add */
				if (hitbl->mctable[i].valid == 0)
					break;
				i++;
			}
		} else if (parm->eModeMember == GSW_IGMP_MEMBER_DONT_CARE) {
			i = 63;
			while (i > 0) {
				/* Find a new empty entry to add */
				if (hitbl->mctable[i].valid == 0)
					break;
				i--;
			}
		}
		if (i >= 0 && i < pd->iflag.itblsize) {
			hitbl->mctable[i].dlsbindex = dlix;
			hitbl->mctable[i].dmsbindex = dmix;
			hitbl->mctable[i].pmap |= (1 << parm->nPortId);
			SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
			if (dlflag)
				hpctbl->tsub_tbl.iplsbtcnt[dlix] = 1;
			else
				hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
			if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
				SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
				if (dmflag)
					hpctbl->tsub_tbl.ipmsbtcnt[dmix] = 1;
				else
					hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
			}
			hitbl->mctable[i].valid = 1;
			hitbl->mctable[i].mcmode = parm->eModeMember;
			if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
			|| (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
				hitbl->mctable[i].slsbindex = slix;
				hitbl->mctable[i].smsbindex = smix;
				SWAPI_ASSERT(slix >=  IP_DASA_LSB_SIZE);
				if (slflag)
					hpctbl->tsub_tbl.iplsbtcnt[slix] = 1;
				else
					hpctbl->tsub_tbl.iplsbtcnt[slix]++;
				if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
					SWAPI_ASSERT(smix >=  IP_DASA_MSB_SIZE);
					if (smflag)
						hpctbl->tsub_tbl.ipmsbtcnt[smix] = 1;
					else
						hpctbl->tsub_tbl.ipmsbtcnt[smix]++;
				}
			} else if (parm->eModeMember ==
					GSW_IGMP_MEMBER_DONT_CARE) {
				hitbl->mctable[i].slsbindex = 0x7F;
				hitbl->mctable[i].smsbindex = 0x7F;
			}
		}
		memset(&pt, 0, sizeof(pctbl_prog_t));
		/* Now, we write into Multicast SW Table */
		pt.table = PCE_MULTICAST_SW_INDEX;
		pt.pcindex = i;
		pt.key[1] = ((hitbl->mctable[i].smsbindex << 8)
			| hitbl->mctable[i].slsbindex);
		pt.key[0] = ((hitbl->mctable[i].dmsbindex << 8)
			| hitbl->mctable[i].dlsbindex);

		if ((parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE))
			pt.val[0] = (0 << parm->nPortId);
		else
			pt.val[0] = hitbl->mctable[i].pmap;
		pt.valid = hitbl->mctable[i].valid;
		pt.opmode = PCE_OPMODE_ADWR;
		s = mcast_tbl_wr(pdev, &pt);
		if (s != GSW_statusOk)
			return s;

		if ((parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
			for (i = 0; i < pd->iflag.itblsize; i++) {
				/* Check if port was already exist */
				if ((hitbl->mctable[i].dlsbindex == dlix) &&
				(hitbl->mctable[i].dmsbindex == dmix) &&
				(hitbl->mctable[i].slsbindex == 0x7F) &&
				(hitbl->mctable[i].smsbindex == 0x7F) &&
				(hitbl->mctable[i].valid == 1)) {
					if (((hitbl->mctable[i].pmap >>
						parm->nPortId) & 0x1) == 1)
						return GSW_statusOk;
					SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
					hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
					if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
						SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
						hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
					}
					hitbl->mctable[i].mcmode =
					GSW_IGMP_MEMBER_DONT_CARE;
					/* Add the port */
					hitbl->mctable[i].pmap |=
					(1 << parm->nPortId);
					memset(&pt, 0, sizeof(pctbl_prog_t));
					pt.table = PCE_MULTICAST_SW_INDEX;
					pt.pcindex = i;
					pt.key[1] =
					((hitbl->mctable[i].smsbindex << 8)
					| (hitbl->mctable[i].slsbindex));
					pt.key[0] =
					((hitbl->mctable[i].dmsbindex << 8)
					| (hitbl->mctable[i].dlsbindex));
					pt.val[0] = hitbl->mctable[i].pmap;
					pt.valid = hitbl->mctable[i].valid;
					pt.opmode = PCE_OPMODE_ADWR;
					s = mcast_tbl_wr(pdev, &pt);
					if (s != GSW_statusOk)
						return s;
					return GSW_statusOk;
				}
			}
			i = 63;
			while (i > 0) {
				/* Find a new empty entry to add */
				if (hitbl->mctable[i].valid == 0)
					break;
				i--;
			}
		if (i >= 0 && i < pd->iflag.itblsize) {
				/* Now, we write into Multicast SW Table */
			hitbl->mctable[i].dlsbindex = dlix;
			hitbl->mctable[i].dmsbindex = dmix;
			hitbl->mctable[i].slsbindex = 0x7F;
			hitbl->mctable[i].smsbindex = 0x7F;
			hitbl->mctable[i].pmap |= (1 << parm->nPortId);
			hitbl->mctable[i].mcmode = GSW_IGMP_MEMBER_DONT_CARE;
			hitbl->mctable[i].valid = 1;
			SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
			hpctbl->tsub_tbl.iplsbtcnt[dlix]++;
			if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
				SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
				hpctbl->tsub_tbl.ipmsbtcnt[dmix]++;
			}
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MULTICAST_SW_INDEX;
			pt.pcindex = i;
			pt.key[1] = ((hitbl->mctable[i].smsbindex << 8)
				| hitbl->mctable[i].slsbindex);
			pt.key[0] = ((hitbl->mctable[i].dmsbindex << 8)
				| hitbl->mctable[i].dlsbindex);
			pt.val[0] = hitbl->mctable[i].pmap;
			pt.valid = hitbl->mctable[i].valid;
			pt.opmode = PCE_OPMODE_ADWR;
			s = mcast_tbl_wr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			} else {
				GSW_PRINT("%s:%s:%d (IGMP Table full)\n",
					__FILE__, __func__, __LINE__);
			}
		}
	}
	/* Debug */
	return GSW_statusOk;
}

static GSW_return_t tbl_idx_delete(u8 *rcnt,
	u32 index, u32 tsize)
{
	PCE_ASSERT(index >= tsize);
	if (rcnt[index] > 0)
		rcnt[index]--;
	return 0;
}

int ipdslsb_tblidx_del(pcetbl_prog_t *tpt,
	u32 index)
{
	return tbl_idx_delete(tpt->iplsbtcnt,
		index, IP_DASA_LSB_SIZE);
}

int ipdsmsb_tblidx_del(pcetbl_prog_t *tpt,
	u32 index)
{
	return tbl_idx_delete(tpt->ipmsbtcnt,
		index, IP_DASA_MSB_SIZE);
}

/* Multicast Software Table Include/Exclude Remove function */
static int gsw2x_msw_table_rm(void *pdev,
	GSW_multicastTable_t *parm)
{
	GSW_return_t s;
	u8	i, j;
	ltq_bool_t MATCH = 0;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	tft_tbl_t *hpctbl = &pd->ptft;
	gsw_igmp_t *hitbl = &pd->iflag;
	pctbl_prog_t pt;
	ip_dasa_t ltbl;
	ip_dasa_t mtbl;
	int dlix = 0x7F, dmix = 0x7F, slix = 0x7F, smix = 0x7F;
	SWAPI_ASSERT(pd == NULL);
	memset(&pt, 0, sizeof(pctbl_prog_t));
	memset(&ltbl, 0, sizeof(ip_dasa_t));
	memset(&mtbl, 0, sizeof(ip_dasa_t));
	if ((parm->eIPVersion != GSW_IP_SELECT_IPV4)
		&& (parm->eIPVersion != GSW_IP_SELECT_IPV6)) {
		GSW_PRINT("%s:%s:%d (IPv4/IPV6 need to enable!!!)\n",
			__FILE__, __func__, __LINE__);
		return GSW_statusErr;
	}
	if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
		&& (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)
		&& (parm->eModeMember == GSW_IGMP_MEMBER_DONT_CARE)) {
		GSW_PRINT("%s:%s:%d (!!!)\n", __FILE__, __func__, __LINE__);
		return GSW_statusErr;
	}
	if (parm->eIPVersion == GSW_IP_SELECT_IPV4) {
		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0; i < 4; i++)
			ltbl.ip_val[i] = ((parm->uIP_Gda.nIPv4 >> (i * 8)) & 0xFF);
			/* DIP LSB Nibble Mask */
		ltbl.ip_mask[0] = 0xFF00;
	}
	if (parm->eIPVersion == GSW_IP_SELECT_IPV6 /* IPv6 */) {
		/* First, search for DIP in the DA/SA table (DIP MSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			mtbl.ip_val[j-1] = (parm->uIP_Gda.nIPv6[i] & 0xFF);
			mtbl.ip_val[j] = ((parm->uIP_Gda.nIPv6[i] >> 8) & 0xFF);
		}
		mtbl.ip_mask[0] = 0;/* DIP MSB Nibble Mask */
		mtbl.ip_valid = 1;
		dmix = find_msb_tbl_entry(&hpctbl->tsub_tbl, &mtbl);
		if (dmix == 0xFF) {
			GSW_PRINT("%s:%s:%d (IGMP Entry not found)\n",
				__FILE__, __func__, __LINE__);
			return GSW_statusErr;
		}
		/* First, search for DIP in the DA/SA table (DIP LSB) */
		for (i = 0, j = 7; i < 4; i++, j -= 2) {
			ltbl.ip_val[j-1] = (parm->uIP_Gda.nIPv6[i+4] & 0xFF);
			ltbl.ip_val[j] = ((parm->uIP_Gda.nIPv6[i+4] >> 8) & 0xFF);
		}
		ltbl.ip_mask[0] = 0;/* DIP LSB Nibble Mask */
	}
	ltbl.ip_valid = 1;
	dlix = find_dasa_tbl_entry(&hpctbl->tsub_tbl, &ltbl);
	if (dlix == 0xFF) {
		GSW_PRINT("%s:%s:%d (IGMP Entry not found)\n",
			__FILE__, __func__, __LINE__);
		return GSW_statusErr;
	}
	if ((parm->eModeMember == GSW_IGMP_MEMBER_INCLUDE)
		|| (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE)) {
		if (parm->eIPVersion == GSW_IP_SELECT_IPV4) {
			/* First, search for DIP in the DA/SA table (DIP LSB) */
			for (i = 0; i < 4; i++)
				ltbl.ip_val[i] = ((parm->uIP_Gsa.nIPv4 >> (i * 8)) & 0xFF);
				/* DIP LSB Nibble Mask */
			ltbl.ip_mask[0] = 0xFF00;
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if (ltbl.ip_val[3] == 0 && ltbl.ip_val[2] == 0
					&& ltbl.ip_val[1] == 0 && ltbl.ip_val[0] == 0) {
					GSW_PRINT("%s:%s:%d (Exclude Rule Source IP is Wildcard)\n",
						__FILE__, __func__, __LINE__);
					return GSW_statusErr;
				}
			}
		}
		if (parm->eIPVersion == GSW_IP_SELECT_IPV6) {
			int src_zero = 0;
			/* First, search for DIP in the DA/SA table (DIP MSB) */
			for (i = 0, j = 7; i < 4; i++, j -= 2) {
				mtbl.ip_val[j-1] = (parm->uIP_Gsa.nIPv6[i] & 0xFF);
				mtbl.ip_val[j] = ((parm->uIP_Gsa.nIPv6[i] >> 8) & 0xFF);
			}
			mtbl.ip_mask[0] = 0;/* DIP MSB Nibble Mask */
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if ((mtbl.ip_val[0] == 0) &&
					(mtbl.ip_val[1] == 0) &&
					(mtbl.ip_val[2] == 0) &&
					(mtbl.ip_val[3] == 0) &&
					(mtbl.ip_val[4] == 0) &&
					(mtbl.ip_val[5] == 0) &&
					(mtbl.ip_val[6] == 0) &&
					(mtbl.ip_val[7] == 0))
					src_zero = 1;
			}
			mtbl.ip_valid = 1;
			smix = find_msb_tbl_entry(&hpctbl->tsub_tbl,
				&mtbl);
			if (smix == 0xFF) {
				GSW_PRINT("%s:%s:%d (IGMP Entry not found)\n",
					__FILE__, __func__, __LINE__);
				return GSW_statusErr;
			}
			/* First, search for DIP in the DA/SA table (DIP LSB) */
			for (i = 0, j = 7; i < 4; i++, j -= 2) {
				ltbl.ip_val[j-1] = (parm->uIP_Gsa.nIPv6[i+4] & 0xFF);
				ltbl.ip_val[j] = ((parm->uIP_Gsa.nIPv6[i+4] >> 8) & 0xFF);
			}
			ltbl.ip_mask[0] = 0;/* DIP LSB Nibble Mask */
			if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
				if ((ltbl.ip_val[0] == 0) &&
					(ltbl.ip_val[1] == 0) &&
					(ltbl.ip_val[2] == 0) &&
					(ltbl.ip_val[3] == 0) &&
					(ltbl.ip_val[4] == 0) &&
					(ltbl.ip_val[5] == 0) &&
					(ltbl.ip_val[6] == 0) &&
					(ltbl.ip_val[7] == 0)) {
					if (src_zero) {
						GSW_PRINT("%s:%s:%d (Exclude Rule Source IP is Wildcard)\n",
						__FILE__, __func__, __LINE__);
						return GSW_statusErr;
					}
				}
			}
		}
		ltbl.ip_valid = 1;
		slix = find_dasa_tbl_entry(&hpctbl->tsub_tbl, &ltbl);
		if (slix == 0xFF) {
			GSW_PRINT("%s:%s:%d (IGMP Entry not found)\n",
				__FILE__, __func__, __LINE__);
			return GSW_statusErr;
		}
	}
	for (i = 0; i < pd->iflag.itblsize; i++) {
		if ((hitbl->mctable[i].dlsbindex == dlix) &&
			(hitbl->mctable[i].slsbindex == slix) &&
			(hitbl->mctable[i].dmsbindex == dmix) &&
			(hitbl->mctable[i].smsbindex == smix) &&
			(hitbl->mctable[i].valid == 1)) {

			switch (hitbl->mctable[i].mcmode) {
			case GSW_IGMP_MEMBER_DONT_CARE:
				if (((hitbl->mctable[i].pmap >> parm->nPortId) & 0x1) == 1) {
					hitbl->mctable[i].pmap &= ~(1 << parm->nPortId);
					SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
					if (hpctbl->tsub_tbl.iplsbtcnt[dlix] > 0) {
						ipdslsb_tblidx_del(&hpctbl->tsub_tbl, dlix);
						if (hpctbl->tsub_tbl.iplsbtcnt[dlix] == 0) {
							/* Delet the sub table */
							ip_dasa_lsb_tdel(pdev,
								&hpctbl->tsub_tbl, dlix);
						}
					}
		/* Delet the sub table */
//					SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
					if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] > 0) {
						ipdsmsb_tblidx_del(&hpctbl->tsub_tbl, dmix);
						if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] == 0) {
							if (parm->eIPVersion == GSW_IP_SELECT_IPV6)
								ip_dasa_msb_tdel(pdev,
									&hpctbl->tsub_tbl, dmix);
						}
					}
			/* Check the port map status */
			/* Delet the entry from Multicast sw Table */
					if (hitbl->mctable[i].pmap == 0)
						hitbl->mctable[i].valid = 0;
					MATCH = 1;
				}
				break;
			case GSW_IGMP_MEMBER_INCLUDE:
			case GSW_IGMP_MEMBER_EXCLUDE:
				if (((hitbl->mctable[i].pmap >> parm->nPortId) & 0x1) == 1) {
					hitbl->mctable[i].pmap &= ~(1 << parm->nPortId);
					SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
					if (hpctbl->tsub_tbl.iplsbtcnt[dlix] > 0) {
						ipdslsb_tblidx_del(&hpctbl->tsub_tbl, dlix);
					/* Delet the sub table */
						if (hpctbl->tsub_tbl.iplsbtcnt[dlix] == 0) {
							ip_dasa_lsb_tdel(pdev,
								&hpctbl->tsub_tbl, dlix);
						}
					}
					SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
					if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] > 0) {
						ipdsmsb_tblidx_del(&hpctbl->tsub_tbl, dmix);
		/* Delet the sub table */
						if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] == 0) {
							ip_dasa_msb_tdel(pdev,
								&hpctbl->tsub_tbl, dmix);
						}
					}
					SWAPI_ASSERT(slix >=  IP_DASA_LSB_SIZE);
					if (hpctbl->tsub_tbl.iplsbtcnt[slix] > 0) {
						ipdslsb_tblidx_del(&hpctbl->tsub_tbl, slix);
		/* Delet the sub table */
						if (hpctbl->tsub_tbl.iplsbtcnt[slix] == 0) {
							ip_dasa_lsb_tdel(pdev,
								&hpctbl->tsub_tbl, slix);
						}
					}
					SWAPI_ASSERT(smix >=  IP_DASA_MSB_SIZE);
					if (hpctbl->tsub_tbl.ipmsbtcnt[smix] > 0) {
						ipdsmsb_tblidx_del(&hpctbl->tsub_tbl, smix);
		/* Delet the sub table */
						if (hpctbl->tsub_tbl.ipmsbtcnt[smix] == 0) {
							ip_dasa_msb_tdel(pdev,
								&hpctbl->tsub_tbl, smix);
						}
					}
			/* Check the port map status */
			/* Delet the entry from Multicast sw Table */
					if (hitbl->mctable[i].pmap == 0)
						hitbl->mctable[i].valid = 0;

					MATCH = 1;
					if (parm->eModeMember == GSW_IGMP_MEMBER_EXCLUDE) {
						for (j = 0; j < pd->iflag.itblsize; j++) {
							if ((hitbl->mctable[j].dlsbindex == dlix) &&
								(hitbl->mctable[j].slsbindex == 0x7F) &&
								(hitbl->mctable[j].dmsbindex == dmix) &&
								(hitbl->mctable[j].smsbindex == 0x7F) &&
								(hitbl->mctable[j].valid == 1)) {
								if (((hitbl->mctable[j].pmap >> parm->nPortId) & 0x1) == 1) {
									hitbl->mctable[j].pmap &= ~(1 << parm->nPortId);
									SWAPI_ASSERT(dlix >=  IP_DASA_LSB_SIZE);
									if (hpctbl->tsub_tbl.iplsbtcnt[dlix] > 0) {
										ipdslsb_tblidx_del(&hpctbl->tsub_tbl, dlix);
										if (hpctbl->tsub_tbl.iplsbtcnt[dlix] == 0) {
						/* Delet the sub table */
											ip_dasa_lsb_tdel(pdev,
												&hpctbl->tsub_tbl, dlix);
										}
									}
								SWAPI_ASSERT(dmix >=  IP_DASA_MSB_SIZE);
								if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] > 0) {
									ipdsmsb_tblidx_del(&hpctbl->tsub_tbl, dmix);
									if (hpctbl->tsub_tbl.ipmsbtcnt[dmix] == 0) {
							/* Delet the sub table */
										ip_dasa_msb_tdel(pdev,
											&hpctbl->tsub_tbl, dmix);
									}
								}
								/* Check the port map status */
								if (hitbl->mctable[j].pmap == 0) {
									/* Delet the entry from Multicast sw Table */
									hitbl->mctable[j].valid = 0;
									hitbl->mctable[i].valid = 0;
								}
								memset(&pt, 0, sizeof(pctbl_prog_t));
								pt.table = PCE_MULTICAST_SW_INDEX;
								pt.pcindex = j;
								pt.key[1] = ((0x7F << 8) | 0x7F);
								pt.key[0] = ((hitbl->mctable[j].dmsbindex << 8)
									| (hitbl->mctable[i].dlsbindex));
								pt.val[0] = hitbl->mctable[j].pmap;
								pt.valid = hitbl->mctable[j].valid;
								pt.opmode = PCE_OPMODE_ADWR;
								s = mcast_tbl_wr(pdev, &pt);
								if (s != GSW_statusOk)
									return s;
							}
						}
					}
				}
			}
			break;
			}
			if (MATCH == 1) {
				memset(&pt, 0, sizeof(pctbl_prog_t));
				pt.table = PCE_MULTICAST_SW_INDEX;
				pt.pcindex = i;
				pt.key[1] = ((hitbl->mctable[i].smsbindex << 8)
					| (hitbl->mctable[i].slsbindex));
				pt.key[0] = ((hitbl->mctable[i].dmsbindex << 8)
					| (hitbl->mctable[i].dlsbindex));
				pt.val[0] = hitbl->mctable[i].pmap;
				pt.valid = hitbl->mctable[i].valid;
				pt.opmode = PCE_OPMODE_ADWR;
				s = mcast_tbl_wr(pdev, &pt);
				if (s != GSW_statusOk)
					return s;
			}
		}
	}
	if (MATCH == 0)
		GSW_PRINT("The GIP/SIP not found\n");
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastTableEntryAdd(void *pdev,
	GSW_multicastTable_t *parm)
{
	GSW_return_t s;
	u8 pi = parm->nPortId;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	gsw_igmp_t	*hitbl = &pd->iflag;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, parm->nPortId);
	if (s != GSW_statusOk)
		return s;
	memset(&pt, 0, sizeof(pctbl_prog_t));
	if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		u32 index = 0, i, available = 0;
		if ((pd->iflag.igv3 == 1) ||
			(parm->eIPVersion == GSW_IP_SELECT_IPV6))
			return GSW_statusErr;
		/* Read Out all of the HW Table */
		for (i = 0; i < pd->mctblsize; i++) {
			pt.table = PCE_MULTICAST_HW_INDEX;
			pt.pcindex = i;
			pt.opmode = PCE_OPMODE_ADRD;
			s = gsw_mcast_tbl_rd(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			if (pt.valid) {
				if ((pt.key[0] ==
				(parm->uIP_Gda.nIPv4 & 0xFFFF)) &&
				(pt.key[1] ==
				((parm->uIP_Gda.nIPv4 >> 16) & 0xFFFF))) {
					index = i;
					available = 1;
					break;
				}
			}
		}
		pt.table = PCE_MULTICAST_HW_INDEX;
		if (available == 0) {
			index = pd->mctblsize;
			for (i = 0; i < pd->mctblsize; i++) {
				pt.pcindex = i;
				pt.opmode = PCE_OPMODE_ADRD;
				s = gsw_mcast_tbl_rd(pdev, &pt);
				if (s != GSW_statusOk)
					return s;
				if (pt.valid == 0) {
					index = i;  /* Free index */
					break;
				}
			}
		}
		if (index < pd->mctblsize) {
			pt.table = PCE_MULTICAST_HW_INDEX;
			pt.pcindex = index;
			pt.key[1] = ((parm->uIP_Gda.nIPv4 >> 16) & 0xFFFF);
			pt.key[0] = (parm->uIP_Gda.nIPv4 & 0xFFFF);
			pt.val[0] |= (1 << pi);
			pt.val[4] |= (1 << 14);
			pt.valid = 1;
			pt.opmode = PCE_OPMODE_ADWR;
			s = mcast_tbl_wr(pd, &pt);
			if (s != GSW_statusOk)
				return s;
		} else {
			GSW_PRINT("Error: (IGMP HW Table is full) %s:%s:%d\n",
			__FILE__, __func__, __LINE__);
			return GSW_statusErr;
		}

	} else if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_FORWARD) {
		/* Program the Multicast SW Table */
		s = gsw2x_msw_table_wr(pdev, parm);
		if (s != GSW_statusOk)
			return s;
	} else {
		/* Disable All Multicast SW Table */
		GSW_PRINT("Select IGMP mode using Snooping Config API\n");
	}
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastTableEntryRemove(void *pdev,
	GSW_multicastTable_t *parm)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	gsw_igmp_t *hitbl = &pd->iflag;
	u8 pi = parm->nPortId;
	pctbl_prog_t pt;
	ltq_bool_t dflag = 0;
	u32 port = 0, i;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, parm->nPortId);
	if (s != GSW_statusOk)
		return s;
	memset(&pt, 0, sizeof(pctbl_prog_t));
	if (hitbl->igmode ==
		GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		if (pd->iflag.igv3 == 1)
			return GSW_statusErr;
		/* Read Out all of the HW Table */
		for (i = 0; i < pd->mctblsize; i++) {
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MULTICAST_HW_INDEX;
			pt.pcindex = i;
			pt.opmode = PCE_OPMODE_ADRD;
			s = gsw_mcast_tbl_rd(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			/* Fill into Structure */
			if (((pt.val[0] >> pi) & 0x1) == 1) {
				if (parm->uIP_Gda.nIPv4 ==
					((pt.key[1] << 16)
						| (pt.key[0]))) {
					port = (pt.val[0] & (~(1 << pi)));
					if (port == 0) {
						pt.val[0] = 0;
						pt.key[1] = 0;
						pt.val[4] = 0;
					} else {
						pt.val[0] &= ~(1 << pi);
					}
					dflag = 1;
					pt.opmode = PCE_OPMODE_ADWR;
					s = mcast_tbl_wr(pdev, &pt);
					if (s != GSW_statusOk)
						return s;
				}
			}
		}
		if (dflag == 0)
			GSW_PRINT("The input did not found\n");
	} else if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_FORWARD) {
		s = gsw2x_msw_table_rm(pd, parm);
		if (s != GSW_statusOk)
			return s;
	}
	return GSW_statusOk;
}

GSW_return_t ip_ll_trd(void *pdev, pctbl_prog_t *pt)
{
	ur r;
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_3_KEY3_OFFSET,
		PCE_TBL_KEY_3_KEY3_SHIFT,
		PCE_TBL_KEY_3_KEY3_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->key[3] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_2_KEY2_OFFSET,
		PCE_TBL_KEY_2_KEY2_SHIFT,
		PCE_TBL_KEY_2_KEY2_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->key[2] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_1_KEY1_OFFSET,
		PCE_TBL_KEY_1_KEY1_SHIFT,
		PCE_TBL_KEY_1_KEY1_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->key[1] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_KEY_0_KEY0_OFFSET,
		PCE_TBL_KEY_0_KEY0_SHIFT,
		PCE_TBL_KEY_0_KEY0_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->key[0] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_MASK_0_MASK0_OFFSET,
		PCE_TBL_MASK_0_MASK0_SHIFT,
		PCE_TBL_MASK_0_MASK0_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->mask[0] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_TYPE_OFFSET,
		PCE_TBL_CTRL_TYPE_SHIFT,
		PCE_TBL_CTRL_TYPE_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->type = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->valid = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_GMAP_OFFSET,
		PCE_TBL_CTRL_GMAP_SHIFT,
		PCE_TBL_CTRL_GMAP_SIZE,
		&r);
	if (s != GSW_statusOk)
		return s;
	pt->group = r;
	return GSW_statusOk;
}

GSW_return_t GSW_MulticastTableEntryRead(void *pdev,
	GSW_multicastTableRead_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	gsw_igmp_t	*hitbl = &pd->iflag;
	SWAPI_ASSERT(pd == NULL);
	if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_DISABLED) {
		GSW_PRINT("Error: (IGMP snoop is not enabled) %s:%s:%d\n",
		__FILE__, __func__, __LINE__);
		return GSW_statusErr;
	}
	if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_AUTOLEARNING) {
		if (parm->bInitial == 1) {
			pd->mhw_rinx = 0; /*Start from the index 0 */
			parm->bInitial = 0;
		}
		if (pd->mhw_rinx >= pd->mctblsize) {
			memset(parm, 0, sizeof(GSW_multicastTableRead_t));
			parm->bLast = 1;
			pd->mhw_rinx = 0;
			return GSW_statusOk;
		}

		do {
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MULTICAST_HW_INDEX;
			pt.pcindex = pd->mhw_rinx;
			pt.opmode = PCE_OPMODE_ADRD;
			s = gsw_mcast_tbl_rd(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			pd->mhw_rinx++;
			if (pt.valid != 0)
				break;
		} while (pd->mhw_rinx < pd->mctblsize);
		if (pt.valid != 0) {
			parm->nPortId = pt.val[0] | 0x80000000;
			parm->uIP_Gda.nIPv4 =
			((pt.key[1] << 16) |
			pt.key[0]);
			parm->uIP_Gsa.nIPv4 = 0;
			parm->eModeMember = GSW_IGMP_MEMBER_DONT_CARE;
			parm->eIPVersion = GSW_IP_SELECT_IPV4;
			parm->bInitial = 0;
			parm->bLast = 0;
		} else {
			memset(parm, 0, sizeof(GSW_multicastTableRead_t));
			parm->bLast = 1;
			pd->mhw_rinx = 0;
		}
	}
	/*Snooping in Forward mode */
	if (hitbl->igmode == GSW_MULTICAST_SNOOP_MODE_FORWARD) {
		u32 dlsbid, slsbid, dmsbid, smsbid;
		if (parm->bInitial == 1) {
			pd->msw_rinx = 0; /*Start from the index 0 */
			parm->bInitial = 0;
		}
		if (pd->msw_rinx >= pd->mctblsize) {
			memset(parm, 0, sizeof(GSW_multicastTableRead_t));
			parm->bLast = 1;
			pd->msw_rinx = 0;
			return GSW_statusOk;
		}

		do {
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.table = PCE_MULTICAST_SW_INDEX;
			pt.pcindex = pd->msw_rinx;
			pt.opmode = PCE_OPMODE_ADRD;
			s = gsw_mcast_tbl_rd(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			pd->msw_rinx++;
			if (pt.valid != 0)
				break;
		} while (pd->msw_rinx < pd->mctblsize);
		if (pt.valid == 1) {
			pctbl_prog_t iptbl;
			parm->nPortId = (pt.val[0] | 0x80000000);
			dlsbid = pt.key[0] & 0xFF;
			dmsbid = (pt.key[0] >> 8) & 0xFF;
			slsbid = pt.key[1] & 0xFF;
			smsbid = (pt.key[1] >> 8) & 0xFF;
			if (dlsbid <= 0x3F) {
				memset(&iptbl, 0, sizeof(pctbl_prog_t));
				iptbl.table = PCE_IP_DASA_LSB_INDEX;
				/* Search the DIP */
				iptbl.pcindex = dlsbid;
				iptbl.opmode = PCE_OPMODE_ADRD;
				s = ip_ll_trd(pdev, &iptbl);
				if (s != GSW_statusOk)
					return s;
				if (iptbl.valid == 1) {
					if (iptbl.mask[0] == 0xFF00) {
						parm->uIP_Gda.nIPv4 =
						((iptbl.key[1] << 16)
						| (iptbl.key[0]));
						parm->eIPVersion =
						GSW_IP_SELECT_IPV4;
					} else if (iptbl.mask[0] == 0x0) {
						parm->uIP_Gda.nIPv6[4] =
							(iptbl.key[3]);
						parm->uIP_Gda.nIPv6[5] =
							(iptbl.key[2]);
						parm->uIP_Gda.nIPv6[6] =
							(iptbl.key[1]);
						parm->uIP_Gda.nIPv6[7] =
							(iptbl.key[0]);
						parm->eIPVersion =
						GSW_IP_SELECT_IPV6;
					}
				}
			}
			if (slsbid <= 0x3F) {
				memset(&iptbl, 0, sizeof(pctbl_prog_t));
				iptbl.table = PCE_IP_DASA_LSB_INDEX;
				/* Search the SIP */
				iptbl.pcindex = slsbid;
				iptbl.opmode = PCE_OPMODE_ADRD;
				s = ip_ll_trd(pdev, &iptbl);
				if (s != GSW_statusOk)
					return s;
				if (iptbl.valid == 1) {
					if (iptbl.mask[0] == 0xFF00) {
						parm->uIP_Gsa.nIPv4 =
						((iptbl.key[1] << 16)
						| (iptbl.key[0]));
						parm->eIPVersion =
						GSW_IP_SELECT_IPV4;
					} else if (iptbl.mask == 0x0) {
						parm->uIP_Gsa.nIPv6[4] =
							(iptbl.key[3]);
						parm->uIP_Gsa.nIPv6[5] =
							(iptbl.key[2]);
						parm->uIP_Gsa.nIPv6[6] =
							(iptbl.key[1]);
						parm->uIP_Gsa.nIPv6[7] =
							(iptbl.key[0]);
					}
				}
			}
			if (dmsbid <= 0xF) {
				memset(&iptbl, 0, sizeof(pctbl_prog_t));
				iptbl.table = PCE_IP_DASA_MSB_INDEX;
				/* Search the DIP */
				iptbl.pcindex = dmsbid;
				iptbl.opmode = PCE_OPMODE_ADRD;
				s = ip_ll_trd(pdev, &iptbl);
				if (s != GSW_statusOk)
					return s;
				if (iptbl.valid == 1) {
					if (iptbl.mask[0]  == 0) {
						parm->uIP_Gda.nIPv6[0] =
							(iptbl.key[3]);
						parm->uIP_Gda.nIPv6[1] =
							(iptbl.key[2]);
						parm->uIP_Gda.nIPv6[2] =
							(iptbl.key[1]);
						parm->uIP_Gda.nIPv6[3] =
							(iptbl.key[0]);
					}
				}
			}
			if (smsbid <= 0xF) {
				memset(&iptbl, 0, sizeof(pctbl_prog_t));
				iptbl.table = PCE_IP_DASA_MSB_INDEX;
				/* Search the DIP */
				iptbl.pcindex = smsbid;
				iptbl.opmode = PCE_OPMODE_ADRD;
				s = ip_ll_trd(pdev, &iptbl);
				if (s != GSW_statusOk)
					return s;
				if (iptbl.valid == 1) {
					if (iptbl.mask[0] == 0) {
						parm->uIP_Gsa.nIPv6[0] =
							(iptbl.key[3]);
						parm->uIP_Gsa.nIPv6[1] =
							(iptbl.key[2]);
						parm->uIP_Gsa.nIPv6[2] =
							(iptbl.key[1]);
						parm->uIP_Gsa.nIPv6[3] =
							(iptbl.key[0]);
					}
				}
			}
			parm->eModeMember =
				hitbl->mctable[pd->msw_rinx-1].mcmode;
			parm->bInitial = 0;
			parm->bLast = 0;
		} else {
			memset(parm, 0, sizeof(GSW_multicastTableRead_t));
			parm->bLast = 1;
			pd->msw_rinx = 0;
		}
	}
	return GSW_statusOk;
}

/*
 * =======================================================
 * SWITCH VLAN API
 * =======================================================
 * */
static GSW_return_t vlan_map_tbl_rd(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	ur r;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[0] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[1] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->val[2] = r;
	s = gsw_reg_rd(pdev,
		PCE_TBL_CTRL_VLD_OFFSET,
		PCE_TBL_CTRL_VLD_SHIFT,
		PCE_TBL_CTRL_VLD_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pt->valid = r;
	return GSW_statusOk;
}

static GSW_return_t vlan_map_tbl_wr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_PortMemberRead(void *pdev,
	GSW_VLAN_portMemberRead_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);

	if (parm->bInitial == 1) {
		vlan_rd_index = 0; /* Start from the index 0 */
		pt.table = PCE_VLANMAP_INDEX;
		pt.pcindex = vlan_rd_index;
		pt.opmode = PCE_OPMODE_ADRD;
		s = vlan_map_tbl_rd(pdev, &pt);
		if (s != GSW_statusOk)
			return s;
		parm->nVId = vlan_rd_index;
		parm->nPortId = pt.val[1];
		parm->nTagId = pt.val[2];
		parm->bInitial = 0;
		parm->bLast = 0;
	}
	if (parm->bLast != 1) {
		if (vlan_rd_index < VLAN_MAP_TBL_SIZE) {
			vlan_rd_index++;
			pt.table = PCE_VLANMAP_INDEX;
			pt.pcindex = vlan_rd_index;
			pt.opmode = PCE_OPMODE_ADRD;
			s = vlan_map_tbl_rd(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
			parm->nVId = vlan_rd_index;
			parm->nPortId = pt.val[1];
			parm->nTagId = pt.val[2];
		} else {
			parm->bLast = 1;
			vlan_rd_index = 0;
		}
	}
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_IdGet(void *pdev,
	GSW_VLAN_IdGet_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);

	memset(&pt, 0, sizeof(pctbl_prog_t));
	pt.pcindex = parm->nVId;
	pt.table = PCE_VLANMAP_INDEX;
	pt.opmode = PCE_OPMODE_ADRD;
	s = vlan_map_tbl_rd(pdev, &pt);
	if (s != GSW_statusOk)
		return s;
	parm->nFId = (pt.val[0] & 0xFF);
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_IdCreate(void *pdev,
	GSW_VLAN_IdCreate_t *parm)
{
	GSW_return_t s;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);

	memset(&pt, 0, sizeof(pctbl_prog_t));
	pt.pcindex = parm->nVId;
	pt.table = PCE_VLANMAP_INDEX;
	pt.val[0] = (parm->nFId & 0xFF);
	pt.opmode = PCE_OPMODE_ADWR;
	s = vlan_map_tbl_wr(pdev, &pt);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_PortMemberAdd(void *pdev,
	GSW_VLAN_portMemberAdd_t *parm)
{
	GSW_return_t s;
	u32 pi = parm->nPortId;
	pctbl_prog_t pt;
	u16  pm, tm, val0;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, (pi & 0xFF));
	if (s != GSW_statusOk)
		return s;

	if (parm->nVId >= VLAN_MAP_TBL_SIZE)
		return GSW_statusValueRange;

	memset(&pt, 0, sizeof(pctbl_prog_t));
	pt.table = PCE_VLANMAP_INDEX;
	pt.pcindex = parm->nVId;
	pt.opmode = PCE_OPMODE_ADRD;
	s = vlan_map_tbl_rd(pdev, &pt);
	if (s != GSW_statusOk)
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
	s = vlan_map_tbl_wr(pdev, &pt);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_PortLinkCfgGet(void *pdev,
	GSW_portLinkCfg_t *parm)
{
	ur r;
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		(MAC_PSTAT_FDUP_OFFSET + (0xC * pi)),
		MAC_PSTAT_FDUP_SHIFT,
		MAC_PSTAT_FDUP_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r)
		parm->eDuplex = GSW_DUPLEX_FULL;
	else
		parm->eDuplex = GSW_DUPLEX_HALF;
	s = gsw_reg_rd(pdev,
		(MAC_PSTAT_GBIT_OFFSET + (0xC * pi)),
		MAC_PSTAT_GBIT_SHIFT,
		MAC_PSTAT_GBIT_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r) {
		parm->eSpeed = GSW_PORT_SPEED_1000;
	} else {
		s = gsw_reg_rd(pdev,
			(MAC_PSTAT_MBIT_OFFSET + (0xC * pi)),
			MAC_PSTAT_MBIT_SHIFT,
			MAC_PSTAT_MBIT_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
		if (r)
			parm->eSpeed = GSW_PORT_SPEED_100;
		else
			parm->eSpeed = GSW_PORT_SPEED_10;
	}
	/* Low-power Idle Mode  configuration*/
	s = gsw_reg_rd(pdev,
		(MAC_CTRL_4_LPIEN_OFFSET + (0xC * pi)),
		MAC_CTRL_4_LPIEN_SHIFT,
		MAC_CTRL_4_LPIEN_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bLPI = r;
	s = gsw_reg_rd(pdev,
		(MAC_PSTAT_LSTAT_OFFSET + (0xC * pi)),
		MAC_PSTAT_LSTAT_SHIFT,
		MAC_PSTAT_LSTAT_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	if (r)
		parm->eLink = GSW_PORT_LINK_UP;
	else
		parm->eLink = GSW_PORT_LINK_DOWN;
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_PortCfgGet(void *pdev,
	GSW_VLAN_portCfg_t *parm)
{
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ur r;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		(PCE_DEFPVID_PVID_OFFSET + (10 * pi)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->nPortVId = r;
	s = gsw_reg_rd(pdev,
		(PCE_VCTRL_UVR_OFFSET + (10 * pi)),
		PCE_VCTRL_UVR_SHIFT,
		PCE_VCTRL_UVR_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bVLAN_UnknownDrop = r;
	s = gsw_reg_rd(pdev,
		(PCE_VCTRL_VSR_OFFSET + (10 * pi)),
		PCE_VCTRL_VSR_SHIFT,
		PCE_VCTRL_VSR_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bVLAN_ReAssign = r;
	s = gsw_reg_rd(pdev,
		(PCE_VCTRL_VIMR_OFFSET + (10 * pi)),
		PCE_VCTRL_VIMR_SHIFT,
		PCE_VCTRL_VIMR_VEMR_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->eVLAN_MemberViolation = r;
	s = gsw_reg_rd(pdev,
		(PCE_VCTRL_VINR_OFFSET + (10 * pi)),
		PCE_VCTRL_VINR_SHIFT,
		PCE_VCTRL_VINR_SIZE, &r);
	if (s != GSW_statusOk)
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
	s = gsw_reg_rd(pdev,
		(PCE_PCTRL_0_TVM_OFFSET + (10 * pi)),
		PCE_PCTRL_0_TVM_SHIFT,
		PCE_PCTRL_0_TVM_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	parm->bTVM = r;
	return GSW_statusOk;
}

GSW_return_t GSW_VLAN_PortCfgSet(void *pdev,
	GSW_VLAN_portCfg_t *parm)
{
	ur r;
	GSW_return_t s;
	u8 pi = parm->nPortId;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pi);
	if (s != GSW_statusOk)
		return s;
	r = parm->nPortVId;
	s = gsw_reg_wr(pdev,
		(PCE_DEFPVID_PVID_OFFSET + (10 * pi)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	r = (parm->bVLAN_UnknownDrop & 0x1);
	s = gsw_reg_wr(pdev,
		(PCE_VCTRL_UVR_OFFSET + (10 * pi)),
		PCE_VCTRL_UVR_SHIFT,
		PCE_VCTRL_UVR_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	r = parm->bVLAN_ReAssign;
	s = gsw_reg_wr(pdev,
		(PCE_VCTRL_VSR_OFFSET + (10 * pi)),
		PCE_VCTRL_VSR_SHIFT,
		PCE_VCTRL_VSR_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	r = (parm->eVLAN_MemberViolation & 0x3);
	s = gsw_reg_wr(pdev,
		(PCE_VCTRL_VIMR_OFFSET + (10 * pi)),
		PCE_VCTRL_VIMR_SHIFT,
		PCE_VCTRL_VIMR_VEMR_SIZE, r);
	if (s != GSW_statusOk)
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
	s = gsw_reg_wr(pdev,
		(PCE_VCTRL_VINR_OFFSET + (10 * pi)),
		PCE_VCTRL_VINR_SHIFT,
		PCE_VCTRL_VINR_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	r = (parm->bTVM);
	s = gsw_reg_wr(pdev,
		(PCE_PCTRL_0_TVM_OFFSET + (10 * pi)),
		PCE_PCTRL_0_TVM_SHIFT,
		PCE_PCTRL_0_TVM_SIZE, r);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

static GSW_return_t get_hw_cap(void *pdev)
{
	ur r;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->pnum = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_VPORTS_OFFSET,
		ETHSW_CAP_1_VPORTS_SHIFT,
		ETHSW_CAP_1_VPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->tpnum = r + pd->pnum;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_QUEUE_OFFSET,
		ETHSW_CAP_1_QUEUE_SHIFT,
		ETHSW_CAP_1_QUEUE_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->nqueues = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_3_METERS_OFFSET,
		ETHSW_CAP_3_METERS_SHIFT,
		ETHSW_CAP_3_METERS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->nmeters = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_3_SHAPERS_OFFSET,
		ETHSW_CAP_3_SHAPERS_SHIFT,
		ETHSW_CAP_3_SHAPERS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->nshapers = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_4_PPPOE_OFFSET,
		ETHSW_CAP_4_PPPOE_SHIFT,
		ETHSW_CAP_4_PPPOE_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->npppoe = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_4_VLAN_OFFSET,
		ETHSW_CAP_4_VLAN_SHIFT,
		ETHSW_CAP_4_VLAN_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->avlantsz = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_5_IPPLEN_OFFSET,
		ETHSW_CAP_5_IPPLEN_SHIFT,
		ETHSW_CAP_5_IPPLEN_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->ip_pkt_lnt_size = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_5_PROT_OFFSET,
		ETHSW_CAP_5_PROT_SHIFT,
		ETHSW_CAP_5_PROT_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->prot_table_size = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_6_MACDASA_OFFSET,
		ETHSW_CAP_6_MACDASA_SHIFT,
		ETHSW_CAP_6_MACDASA_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->mac_dasa_table_size = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_6_APPL_OFFSET,
		ETHSW_CAP_6_APPL_SHIFT,
		ETHSW_CAP_6_APPL_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->app_table_size = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_7_IPDASAM_OFFSET,
		ETHSW_CAP_7_IPDASAM_SHIFT,
		ETHSW_CAP_7_IPDASAM_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->idsmtblsize = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_7_IPDASAL_OFFSET,
		ETHSW_CAP_7_IPDASAL_SHIFT,
		ETHSW_CAP_7_IPDASAL_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->idsltblsize = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_8_MCAST_OFFSET,
		ETHSW_CAP_8_MCAST_SHIFT,
		ETHSW_CAP_8_MCAST_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->mctblsize = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_9_FLAGG_OFFSET,
		ETHSW_CAP_9_FLAGG_SHIFT,
		ETHSW_CAP_9_FLAGG_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->tftblsize = r;
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_10_MACBT_OFFSET,
		ETHSW_CAP_10_MACBT_SHIFT,
		ETHSW_CAP_10_MACBT_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->mactblsize = r;
	s = gsw_reg_rd(pdev,
		ETHSW_VERSION_REV_ID_OFFSET,
		ETHSW_VERSION_REV_ID_SHIFT,
		ETHSW_VERSION_REV_ID_REG_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	pd->gipver = r;
	pd->cport = GSW_2X_SOC_CPU_PORT;
	return GSW_statusOk;
}

static GSW_return_t port_cfg_init(void *pdev)
{
	ur r, i;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < r; i++) {
		memset(&gpc[i], 0, sizeof(port_config_t));
		gpc[i].llimit = 0xFF;
		gpc[i].penable = 1;
	}
	gstpconfig.sfport = GSW_PORT_FORWARD_DEFAULT;
	gstpconfig.fpid8021x = pd->cport;
	return GSW_statusOk;
}

void multi_table_init(void *pdev)
{
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	u32 i;
	memset(&pd->iflag, 0, sizeof(gsw_igmp_t));
	pd->iflag.itblsize = pd->mctblsize;
	for (i = 0; i < pd->iflag.itblsize; i++) {
		pd->iflag.mctable[i].slsbindex = 0x7F;
		pd->iflag.mctable[i].dlsbindex = 0x7F;
		pd->iflag.mctable[i].smsbindex = 0x1F;
		pd->iflag.mctable[i].dmsbindex = 0x1F;
	}
}

static void pce_table_init(tft_tbl_t *tf)
{
	int i;
	memset(&tf->tsub_tbl, 0, sizeof(pcetbl_prog_t));
	memset(&tf->sub_tbl, 0, sizeof(sub_tbl_t));
	memset(&tf->pce_act, 0, sizeof(GSW_PCE_action_t));
	for (i = 0; i < PCE_TABLE_SIZE; i++)
		tf->ptblused[i] = 0;
}

GSW_return_t GSW_Enable(void *pdev)
{
	ur r, i;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < r;  i++) {
		s = gsw_reg_wr(pdev,
			(FDMA_PCTRL_EN_OFFSET + (i * 0x6)),
			FDMA_PCTRL_EN_SHIFT,
			FDMA_PCTRL_EN_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(SDMA_PCTRL_PEN_OFFSET + (i * 0x6)),
			SDMA_PCTRL_PEN_SHIFT,
			SDMA_PCTRL_PEN_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(BM_PCFG_CNTEN_OFFSET + (i * 2)),
			BM_PCFG_CNTEN_SHIFT,
			BM_PCFG_CNTEN_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
	}
	return GSW_statusOk;
}

GSW_return_t GSW_Disable(void *pdev)
{
	ur r, i;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < r; i++) {
		s = gsw_reg_wr(pdev,
			(FDMA_PCTRL_EN_OFFSET + (i * 0x6)),
			FDMA_PCTRL_EN_SHIFT,
			FDMA_PCTRL_EN_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			(SDMA_PCTRL_PEN_OFFSET + (i * 0x6)),
			SDMA_PCTRL_PEN_SHIFT,
			SDMA_PCTRL_PEN_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
	}
	return GSW_statusOk;
}

static GSW_return_t parser_ll_twr(void *pdev,
	pctbl_prog_t *pt)
{
	GSW_return_t s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_reg_clr(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_ADDR_ADDR_OFFSET,
		PCE_TBL_ADDR_ADDR_SHIFT,
		PCE_TBL_ADDR_ADDR_SIZE,
		pt->pcindex);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_addr_opmod(pdev, pt->table, pt->opmode);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_3_VAL3_OFFSET,
		PCE_TBL_VAL_3_VAL3_SHIFT,
		PCE_TBL_VAL_3_VAL3_SIZE,
		pt->val[3]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_2_VAL2_OFFSET,
		PCE_TBL_VAL_2_VAL2_SHIFT,
		PCE_TBL_VAL_2_VAL2_SIZE,
		pt->val[2]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_1_VAL1_OFFSET,
		PCE_TBL_VAL_1_VAL1_SHIFT,
		PCE_TBL_VAL_1_VAL1_SIZE,
		pt->val[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_TBL_VAL_0_VAL0_OFFSET,
		PCE_TBL_VAL_0_VAL0_SHIFT,
		PCE_TBL_VAL_0_VAL0_SIZE,
		pt->val[0]);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = pctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t gsw_uc_code_init(void *pdev)
{
	GSW_return_t s;
	u32 vn1, lcnt = 0;
	pctbl_prog_t pt;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_wr(pdev,
		PCE_GCTRL_0_MC_VALID_OFFSET,
		PCE_GCTRL_0_MC_VALID_SHIFT,
		PCE_GCTRL_0_MC_VALID_SIZE, 0x0);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_rd(pdev,
		MANU_ID_PNUML_OFFSET,
		MANU_ID_PNUML_SHIFT,
		MANU_ID_PNUML_SIZE, &vn1);
	if (s != GSW_statusOk)
		return s;
	if (vn1 == 1) {
		/* 7085/7082 (S) */
		u32 r, p, t;
		s = gsw_reg_rd(pdev,
			RST_REQ_RD4_OFFSET,
			RST_REQ_RD4_SHIFT,
			RST_REQ_RD4_SIZE, &r);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_rd(pdev,
			GPHY4_FCR_FCR_OFFSET,
			GPHY4_FCR_FCR_SHIFT,
			GPHY4_FCR_FCR_SIZE, &p);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD4_OFFSET,
			RST_REQ_RD4_SHIFT,
			RST_REQ_RD4_SIZE, 1);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			GPHY4_FCR_FCR_OFFSET,
			GPHY4_FCR_FCR_SHIFT,
			GPHY4_FCR_FCR_SIZE,
			0x1000);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD4_OFFSET,
			RST_REQ_RD4_SHIFT,
			RST_REQ_RD4_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
		do {
			s = gsw_reg_rd(pdev,
				RST_REQ_RD4_OFFSET,
				RST_REQ_RD4_SHIFT,
				RST_REQ_RD4_SIZE,
				&t);
			if (s != GSW_statusOk)
				return s;
			lcnt++;
		} while ((!t) && (lcnt < 10));
		s = gsw_reg_wr(pdev,
			GPHY4_FCR_FCR_OFFSET,
			GPHY4_FCR_FCR_SHIFT,
			GPHY4_FCR_FCR_SIZE,
			p);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD4_OFFSET,
			RST_REQ_RD4_SHIFT,
			RST_REQ_RD4_SIZE,
			r);
		if (s != GSW_statusOk)
			return s;
	} else if (vn1 == 0) {
		/*7084 (G)*/
		u32 r, p, t;
		s = gsw_reg_rd(pdev,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			&r);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_rd(pdev,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			&p);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			1);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			0x1000);

		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			0);
		if (s != GSW_statusOk)
			return s;
		do {
			s = gsw_reg_rd(pdev,
				RST_REQ_RD0_OFFSET,
				RST_REQ_RD0_SHIFT,
				RST_REQ_RD0_SIZE,
				&t);
			if (s != GSW_statusOk)
				return s;
			lcnt++;
		} while ((!t) && (lcnt < 10));
		s = gsw_reg_wr(pdev,
			GPHY0_FCR_FCR_OFFSET,
			GPHY0_FCR_FCR_SHIFT,
			GPHY0_FCR_FCR_SIZE,
			p);
		if (s != GSW_statusOk)
			return s;
		s = gsw_reg_wr(pdev,
			RST_REQ_RD0_OFFSET,
			RST_REQ_RD0_SHIFT,
			RST_REQ_RD0_SIZE,
			r);
		if (s != GSW_statusOk)
			return s;
	}
	s = gsw_reg_rd(pdev,
		PCE_GCTRL_0_MC_VALID_OFFSET,
		PCE_GCTRL_0_MC_VALID_SHIFT,
		PCE_GCTRL_0_MC_VALID_SIZE,
		&vn1);
	if (s != GSW_statusOk)
		return s;
	if ((lcnt >= 10) && !vn1) {
		int i;
		/* Download the microcode over SMDIO interface */
		for (i = 0; i < 64; i++) {
			memset(&pt, 0, sizeof(pctbl_prog_t));
			pt.val[3] = pce_mc_sw2_3[i].val_3;
			pt.val[2] = pce_mc_sw2_3[i].val_2;
			pt.val[1] = pce_mc_sw2_3[i].val_1;
			pt.val[0] = pce_mc_sw2_3[i].val_0;
			pt.pcindex = i;
			pt.table = PCE_PARS_INDEX;
			pt.opmode = PCE_OPMODE_ADWR;
			s = parser_ll_twr(pdev, &pt);
			if (s != GSW_statusOk)
				return s;
		}
		s = gsw_reg_wr(pdev,
			PCE_GCTRL_0_MC_VALID_OFFSET,
			PCE_GCTRL_0_MC_VALID_SHIFT,
			PCE_GCTRL_0_MC_VALID_SIZE,
			0x1);
		if (s != GSW_statusOk)
			return s;
	}
	return GSW_statusOk;
}

GSW_return_t bm_ram_tbl_wr(void *pdev,
	bm_tbl_prog_t *pb)
{
	GSW_return_t s;
	s = gsw_reg_wr(pdev,
		BM_RAM_ADDR_ADDR_OFFSET,
		BM_RAM_ADDR_ADDR_SHIFT,
		BM_RAM_ADDR_ADDR_SIZE,
		pb->bmindex);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_VAL_0_VAL0_OFFSET,
		BM_RAM_VAL_0_VAL0_SHIFT,
		BM_RAM_VAL_0_VAL0_SIZE,
		pb->bmval[0]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_VAL_1_VAL1_OFFSET,
		BM_RAM_VAL_1_VAL1_SHIFT,
		BM_RAM_VAL_1_VAL1_SIZE,
		pb->bmval[1]);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_CTRL_ADDR_OFFSET,
		BM_RAM_CTRL_ADDR_SHIFT,
		BM_RAM_CTRL_ADDR_SIZE,
		pb->bmtable);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_RAM_CTRL_OPMOD_OFFSET,
		BM_RAM_CTRL_OPMOD_SHIFT,
		BM_RAM_CTRL_OPMOD_SIZE,
		pb->bmopmode);
	if (s != GSW_statusOk)
		return s;
	s = bmctrl_bas_set(pdev);
	if (s != GSW_statusOk)
		return s;
	s = bmctrl_bas_busy(pdev);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_QoS_QueueBufferReserveCfgSet(void *pdev,
	GSW_QoS_QueueBufferReserveCfg_t *parm)
{
	u16 ind;
	GSW_return_t s;
	u8 qid = parm->nQueueId;
	bm_tbl_prog_t pb;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = nsqueue(pdev, qid);
	if (s != GSW_statusOk)
		return s;
	memset(&pb, 0, sizeof(bm_tbl_prog_t));
	/* Colourcode = 0  and fixed offset = 0 */
	ind = (((qid << 3) & 0xF8));
	pb.bmindex = ind;
	pb.bmtable = PQM_CONTEXT_TBL;
	pb.bmval[0] = parm->nBufferReserved;
	pb.bmopmode = BM_OPMODE_WR;
	s = bm_ram_tbl_wr(pdev, &pb);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_QoS_WredCfgSet(void *pdev,
	GSW_QoS_WRED_Cfg_t *parm)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	/* Description: 'Drop Probability Profile' */
	s = gsw_reg_wr(pdev,
		BM_QUEUE_GCTRL_DPROB_OFFSET,
		BM_QUEUE_GCTRL_DPROB_SHIFT,
		BM_QUEUE_GCTRL_DPROB_SIZE,
		parm->eProfile);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		BM_QUEUE_GCTRL_GL_MOD_OFFSET,
		BM_QUEUE_GCTRL_GL_MOD_SHIFT,
		BM_QUEUE_GCTRL_GL_MOD_SIZE,
		parm->eThreshMode);
	if (s != GSW_statusOk)
		return s;
	/* WRED Red Threshold - Minimum */
	s = gsw_reg_wr(pdev,
		BM_WRED_RTH_0_MINTH_OFFSET,
		BM_WRED_RTH_0_MINTH_SHIFT,
		BM_WRED_RTH_0_MINTH_SIZE,
		parm->nRed_Min);
	if (s != GSW_statusOk)
		return s;
	/* WRED Red Threshold - Maximum */
	s = gsw_reg_wr(pdev,
		BM_WRED_RTH_1_MAXTH_OFFSET,
		BM_WRED_RTH_1_MAXTH_SHIFT,
		BM_WRED_RTH_1_MAXTH_SIZE,
		parm->nRed_Max);
	if (s != GSW_statusOk)
		return s;
	/* WRED Yellow Threshold - Minimum */
	s = gsw_reg_wr(pdev,
		BM_WRED_YTH_0_MINTH_OFFSET,
		BM_WRED_YTH_0_MINTH_SHIFT,
		BM_WRED_YTH_0_MINTH_SIZE,
		parm->nYellow_Min);
	if (s != GSW_statusOk)
		return s;
	/* WRED Yellow Threshold - Maximum */
	s = gsw_reg_wr(pdev,
		BM_WRED_YTH_1_MAXTH_OFFSET,
		BM_WRED_YTH_1_MAXTH_SHIFT,
		BM_WRED_YTH_1_MAXTH_SIZE,
		parm->nYellow_Max);
	if (s != GSW_statusOk)
		return s;
	/* WRED Green Threshold - Minimum */
	s = gsw_reg_wr(pdev,
		BM_WRED_GTH_0_MINTH_OFFSET,
		BM_WRED_GTH_0_MINTH_SHIFT,
		BM_WRED_GTH_0_MINTH_SIZE,
		parm->nGreen_Min);
	if (s != GSW_statusOk)
		return s;
	/* WRED Green Threshold - Maximum */
	s = gsw_reg_wr(pdev,
		BM_WRED_GTH_1_MAXTH_OFFSET,
		BM_WRED_GTH_1_MAXTH_SHIFT,
		BM_WRED_GTH_1_MAXTH_SIZE,
		parm->nGreen_Max);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_QoS_FlowctrlPortCfgSet(void *pdev,
	GSW_QoS_FlowCtrlPortCfg_t *parm)
{
	GSW_return_t s;
	u8 pn = parm->nPortId;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = npport(pdev, pn);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		(SDMA_PFCTHR8_THR8_OFFSET22 + (pn * 0x4)),
		SDMA_PFCTHR8_THR8_SHIFT,
		SDMA_PFCTHR8_THR8_SIZE,
		parm->nFlowCtrl_Min);
	if (s != GSW_statusOk)
		return s;
/** Ingress Port occupied Buffer Flow Control Threshold */
/* Maximum [number of segments]. */
	s = gsw_reg_wr(pdev,
		(SDMA_PFCTHR9_THR9_OFFSET22 + (pn * 0x4)),
		SDMA_PFCTHR9_THR9_SHIFT,
		SDMA_PFCTHR9_THR9_SIZE,
		parm->nFlowCtrl_Max);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_QoS_FlowctrlCfgSet(void *pdev,
	GSW_QoS_FlowCtrlCfg_t *parm)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_wr(pdev,
		(SDMA_FCTHR1_THR1_OFFSET),
		SDMA_FCTHR1_THR1_SHIFT,
		SDMA_FCTHR1_THR1_SIZE,
		parm->nFlowCtrlNonConform_Min);
	if (s != GSW_statusOk)
		return s;
/** Global Buffer Non Conforming Flow Control */
/* Threshold Maximum [number of segments]. */
	s = gsw_reg_wr(pdev,
		(SDMA_FCTHR2_THR2_OFFSET),
		SDMA_FCTHR2_THR2_SHIFT,
		SDMA_FCTHR2_THR2_SIZE,
		parm->nFlowCtrlNonConform_Max);
	if (s != GSW_statusOk)
		return s;
/** Global Buffer Conforming Flow Control Threshold */
/*  Minimum [number of segments]. */
	s = gsw_reg_wr(pdev,
		(SDMA_FCTHR3_THR3_OFFSET),
		SDMA_FCTHR3_THR3_SHIFT,
		SDMA_FCTHR3_THR3_SIZE,
		parm->nFlowCtrlConform_Min);
	if (s != GSW_statusOk)
		return s;
/** Global Buffer Conforming Flow Control Threshold */
/*  Maximum [number of segments]. */
	s = gsw_reg_wr(pdev,
		(SDMA_FCTHR4_THR4_OFFSET),
		SDMA_FCTHR4_THR4_SHIFT,
		SDMA_FCTHR4_THR4_SIZE,
		parm->nFlowCtrlConform_Max);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t en_buffer_res(void *pdev)
{
	u8 i;
	ur r;
	GSW_return_t s;
	GSW_QoS_QueueBufferReserveCfg_t br;
	GSW_QoS_WRED_Cfg_t wr;
	GSW_QoS_FlowCtrlPortCfg_t pfc;
	GSW_QoS_FlowCtrlCfg_t fc;
//	GSW_QoS_WRED_QueueCfg_t wq;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	memset(&br, 0, sizeof(GSW_QoS_QueueBufferReserveCfg_t));
	memset(&wr, 0, sizeof(GSW_QoS_WRED_Cfg_t));
	memset(&pfc, 0, sizeof(GSW_QoS_FlowCtrlPortCfg_t));
	memset(&fc, 0, sizeof(GSW_QoS_FlowCtrlCfg_t));
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < 32; i++) {
		br.nBufferReserved = 0x1E;
		br.nQueueId = i;
		s = GSW_QoS_QueueBufferReserveCfgSet(pdev, &br);
		if (s != GSW_statusOk)
			return s;
	}
	wr.eProfile = 0;
	wr.eThreshMode = 0;
	wr.nRed_Min = 0x3ff;
	wr.nRed_Max =0x3ff;
	wr.nYellow_Min = 0x3ff;
	wr.nYellow_Max = 0x3ff;
	wr.nGreen_Min = 0x100;
	wr.nGreen_Max =0x100;
	s = GSW_QoS_WredCfgSet(pdev, &wr);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < r; i++) {
		pfc.nPortId = i;
		pfc.nFlowCtrl_Min = 0x18;
		pfc.nFlowCtrl_Max = 0x1E;
		s = GSW_QoS_FlowctrlPortCfgSet(pdev, &pfc);
		if (s != GSW_statusOk) {
			return s;
		}
	}
	fc.nFlowCtrlNonConform_Min = 0x3FF;
	fc.nFlowCtrlNonConform_Max = 0x3FF;
	fc.nFlowCtrlConform_Min = 0x3FF;
	fc.nFlowCtrlConform_Max = 0x3FF;
	s = GSW_QoS_FlowctrlCfgSet(pdev, &fc);
	if (s != GSW_statusOk)
		return s;
/*
	for (i = 0; i < 32; i++) {
		memset(&wq, 0, sizeof(GSW_QoS_WRED_QueueCfg_t));
		wq.nRed_Min = 80;
		wq.nRed_Max = 80;
		wq.nYellow_Min = 80;
		wq.nYellow_Max = 80;
		wq.nGreen_Min = 0x1ff;
		wq.nGreen_Max = 0x1ff;
		wq.nQueueId = i;
		s = GSW_QoS_WredQueueCfgSet(pdev, &wq);
		if (s != GSW_statusOk) {
			return s;
		}
	}
*/
	return GSW_statusOk;
}

static GSW_return_t mac_lpi_mode(void *pdev, int en)
{
	ur r, i;
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = gsw_reg_rd(pdev,
		ETHSW_CAP_1_PPORTS_OFFSET,
		ETHSW_CAP_1_PPORTS_SHIFT,
		ETHSW_CAP_1_PPORTS_SIZE, &r);
	if (s != GSW_statusOk)
		return s;
	for (i = 0; i < r;  i++) {
		if (en == 1)
			s = gsw_reg_wr(pdev,
				(MAC_CTRL_4_LPIEN_OFFSET + (0xC * i)),
				MAC_CTRL_4_LPIEN_SHIFT,
				MAC_CTRL_4_LPIEN_SIZE, 1);
		else
			s = gsw_reg_wr(pdev,
				(MAC_CTRL_4_LPIEN_OFFSET + (0xC * i)),
				MAC_CTRL_4_LPIEN_SHIFT,
				MAC_CTRL_4_LPIEN_SIZE, 0);
		if (s != GSW_statusOk)
			return s;
	}
	return GSW_statusOk;
}

GSW_return_t config_sw_def_vals(void *pdev)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	prdflag = 0;
	mpnum = 0;
	num_ports = 0;
	firstmac = 0;
	mac_rd_index = 0;
	s = get_hw_cap(pdev);
	if (s != GSW_statusOk)
		return s;
	s = port_cfg_init(pdev);
	if (s != GSW_statusOk)
		return s;
#if defined(CONFIG_LTQ_MULTICAST) && CONFIG_LTQ_MULTICAST
	multi_table_init(pdev);
#endif /*CONFIG_LTQ_MULTICAST*/
	pce_table_init(&pd->ptft);
	pd->matimer = DEFAULT_AGING_TIMEOUT;
	s = GSW_Disable(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		GSWIP_CFG_SE_OFFSET,
		GSWIP_CFG_SE_SHIFT,
		GSWIP_CFG_SE_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	s = gsw_uc_code_init(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_PMAP_2_DMCPMAP_OFFSET,
		PCE_PMAP_2_DMCPMAP_SHIFT,
		PCE_PMAP_2_DMCPMAP_SIZE, 0x7F);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_PMAP_3_UUCMAP_OFFSET,
		PCE_PMAP_3_UUCMAP_SHIFT,
		PCE_PMAP_3_UUCMAP_SIZE, 0x7F);
	if (s != GSW_statusOk)
		return s;
	s = en_buffer_res(pdev);
	if (s != GSW_statusOk)
		return s;
	s = mac_lpi_mode (pdev, 1);
	if (s != GSW_statusOk)
		return s;
	s = GSW_Enable(pdev);
	if (s != GSW_statusOk)
		return s;
	s = gsw_reg_wr(pdev,
		PCE_GCTRL_1_VLANMD_OFFSET,
		PCE_GCTRL_1_VLANMD_SHIFT,
		PCE_GCTRL_1_VLANMD_SIZE, 1);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

void ethsw_init_pedev0()
{
	ethsw_api_dev_t *pd;
	ethsw_core_init_t core_init;

#if defined(KERNEL_MODE) && KERNEL_MODE
	pd = (ethsw_api_dev_t *)kmalloc(sizeof(ethsw_api_dev_t), GFP_KERNEL);
#else
	pd = (ethsw_api_dev_t *)malloc(sizeof(ethsw_api_dev_t));
#endif /* KERNEL_MODE */
	if (!pd) {
		GSW_PRINT("%s:%s:%d (malloc failed)\n",
		__FILE__, __func__, __LINE__);
		return;
	}

	memset(pd, 0, sizeof(ethsw_api_dev_t));
	core_init.sdev = 0;
	pd->ecint = &core_init;
	pd->edev = core_init.sdev;
	pd->mdio_id = gsw_mdio_id[0];
	pd->mdio_addr = gsw_mdio_addr[0];
	pedev0[0] = pd;
}

static int gsw_api_open(struct inode *inode, struct file *filp)
{
	u32 minornum, majornum;
	dev_minor_num_t *p;

	minornum = MINOR(inode->i_rdev);
	majornum = MAJOR(inode->i_rdev);
	p = kmalloc(sizeof(dev_minor_num_t), GFP_KERNEL);
	if (!p) {
		GSW_PRINT("%s[%d]: memory allocation failed !!\n",
		__func__, __LINE__);
		return -ENOMEM;
	}
	p->mn = minornum;
	filp->private_data = p;
	return 0;
}

static int gsw_api_release(struct inode *inode,
	struct file *filp)
{
	if (filp->private_data) {
		kfree(filp->private_data);
		filp->private_data = NULL;
	}
	return 0;
}

const struct file_operations swapi_fops = {
	owner:THIS_MODULE,
	open : gsw_api_open,
	release : gsw_api_release
};

int gsw_api_drv_register(u32 major)
{
	int result;
	result = register_chrdev(major, ETHSW_API_DEV_NAME, &swapi_fops);
	if (result < 0) {
		GSW_PRINT("SWAPI: Register Char Dev failed with %d!!!\n", result);
		return result;
	}
	pr_info("SWAPI: Registered char device [%s] with major no [%d]\n",
	ETHSW_API_DEV_NAME, major);
	return 0;
}

int gsw_api_drv_unregister(u32 major)
{
	unregister_chrdev(major, ETHSW_API_DEV_NAME);
	return 0;
}

void *ethsw_api_core_init(ethsw_core_init_t *einit)
{
	ethsw_api_dev_t *pd;
	if (einit->sdev >= GSW_DEV_MAX) {
		GSW_PRINT("%s:%s:%d (Error)\n",
			__FILE__, __func__, __LINE__);
		return 0;
	}
	pd = (ethsw_api_dev_t *)kmalloc(sizeof(ethsw_api_dev_t), GFP_KERNEL);
	if (!pd) {
		GSW_PRINT("%s:%s:%d (malloc failed)\n",
		__FILE__, __func__, __LINE__);
		return pd;
	}
	memset(pd, 0, sizeof(ethsw_api_dev_t));
	pd->ecint = einit;
	pd->edev = einit->sdev;
	return pd;
}

void ethsw_api_core_exit(void *pdev)
{
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	if (pd) {
		kfree(pd);
	}
}

int ethsw_swapi_register(void)
{
	int ret, devid;
	ethsw_core_init_t core_init;

	ret = gsw_api_drv_register(SWAPI_MAJOR_NUMBER);
	if (ret != 0) {
		GSW_PRINT("%s:%s:%d (Error)\n", __FILE__, __func__, __LINE__);
		return ret;
	}
	for ( devid = 0; devid < gsw_num; devid++) {
		if (devid >= GSW_DEV_MAX)
			return -1;
		core_init.sdev = devid;
		if (pedev0[devid] == NULL) {
			pedev0[devid] = ethsw_api_core_init(&core_init);
			if (pedev0[devid] == NULL) {
				GSW_PRINT("%s:%s:%d (Error)\n", __FILE__, __func__, __LINE__);
				return -1;
			}
		}
		/* Change depends on switch supported port number*/
		pedev0[devid]->cport = RGMII_PORT0;
		pedev0[devid]->gsw_dev = devid;
	    if ( hw_init == 1) {
			/* gsw software reset */
			gsw_reg_wr(pedev0[devid],
					GSWIP_CFG_SWRES_OFFSET,
					GSWIP_CFG_SWRES_SHIFT,
					GSWIP_CFG_SWRES_SIZE, 1);
		    if (config_sw_def_vals(pedev0[devid]) != GSW_statusOk) {
			    GSW_PRINT("%s:%s:%d (config_sw_def_vals Error)\n", __FILE__, __func__, __LINE__);
			    return 0;
		    }
	    }
	}
	printk("End %s\n", __func__);
	return 0;
}

int ethsw_swapi_unregister(void)
{
	int devid;
	/* Free the device data block */
	gsw_api_drv_unregister(SWAPI_MAJOR_NUMBER);
	for ( devid = 0; devid < gsw_num; devid++) {
		ethsw_api_core_exit(pedev0[devid]);
	}
	printk("End %s\n", __func__);
	return 0;
}

static GSW_return_t mac_br_rd(void *pdev,
    pctbl_prog_t *pt)
{
    ur r;
    GSW_return_t s;
    s = pctrl_bas_busy(pdev);
    if (s != GSW_statusOk)
        return s;
    s = gsw_reg_wr(pdev,
        PCE_TBL_CTRL_ADDR_OFFSET,
        PCE_TBL_CTRL_ADDR_SHIFT,
        PCE_TBL_CTRL_ADDR_REG_SIZE, 0x000B);
    if (s != GSW_statusOk)
        return s;
    s = pctrl_bas_set(pdev);
    if (s != GSW_statusOk)
        return s;
    s = pctrl_bas_busy(pdev);
    if (s != GSW_statusOk)
        return s;
    s = gsw_reg_rd(pdev,
        PCE_TBL_CTRL_VLD_OFFSET,
        PCE_TBL_CTRL_VLD_SHIFT,
        PCE_TBL_CTRL_VLD_SIZE, &r);
    if (s != GSW_statusOk)
        return s;
    pt->valid = r;
    if (pt->valid == 1) {
        s = gsw_reg_rd(pdev,
            PCE_TBL_KEY_3_KEY3_OFFSET,
            PCE_TBL_KEY_3_KEY3_SHIFT,
            PCE_TBL_KEY_3_KEY3_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->key[3] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_KEY_2_KEY2_OFFSET,
            PCE_TBL_KEY_2_KEY2_SHIFT,
            PCE_TBL_KEY_2_KEY2_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->key[2] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_KEY_1_KEY1_OFFSET,
            PCE_TBL_KEY_1_KEY1_SHIFT,
            PCE_TBL_KEY_1_KEY1_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->key[1] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_KEY_0_KEY0_OFFSET,
            PCE_TBL_KEY_0_KEY0_SHIFT,
            PCE_TBL_KEY_0_KEY0_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->key[0] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_VAL_1_VAL1_OFFSET,
            PCE_TBL_VAL_1_VAL1_SHIFT,
            PCE_TBL_VAL_1_VAL1_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->val[1] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_VAL_0_VAL0_OFFSET,
            PCE_TBL_VAL_0_VAL0_SHIFT,
            PCE_TBL_VAL_0_VAL0_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->val[0] = r;
        s = gsw_reg_rd(pdev,
            PCE_TBL_MASK_0_MASK0_OFFSET,
            PCE_TBL_MASK_0_MASK0_SHIFT,
            PCE_TBL_MASK_0_MASK0_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        pt->mask[0] = r;
    }
    return GSW_statusOk;
}

GSW_return_t GSW_MAC_TableEntryRead(void *pdev,
    GSW_MAC_tableRead_t *parm)
{
    GSW_return_t s;
    pctbl_prog_t pt;
    ur r = 1;
    ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
    SWAPI_ASSERT(pd == NULL);
    memset(&pt, 0, sizeof(pctbl_prog_t));
    if (parm->bInitial == 1) {
        /*Start from the index 0 */
        mac_rd_index = 0;
        firstmac = 0;
        parm->bInitial = 0;
        s = gsw_reg_wr(pdev,
            PCE_TBL_ADDR_ADDR_OFFSET,
            PCE_TBL_ADDR_ADDR_SHIFT,
            PCE_TBL_ADDR_ADDR_SIZE, 0);
        if (s != GSW_statusOk)
            return s;
        s = gsw_reg_wr(pdev,
            PCE_TBL_CTRL_ADDR_OFFSET,
            PCE_TBL_CTRL_ADDR_SHIFT,
            PCE_TBL_CTRL_ADDR_REG_SIZE,
            0xC00B);
        if (s != GSW_statusOk)
            return s;
        s = pctrl_bas_busy(pdev);
        if (s != GSW_statusOk)
            return s;
        s = gsw_reg_rd(pdev,
            PCE_TBL_ADDR_ADDR_OFFSET,
            PCE_TBL_ADDR_ADDR_SHIFT,
            PCE_TBL_ADDR_ADDR_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        firstmac = r;
    }
    if ((mac_rd_index >= MAC_TABLE_SIZE /*pd->mactblsize*/) || (r == 0)) {
        memset(parm, 0, sizeof(GSW_MAC_tableRead_t));
        parm->bLast = 1;
        mac_rd_index = 0;
        s = gsw_reg_wr(pdev,
            PCE_TBL_CTRL_ADDR_OFFSET,
            PCE_TBL_CTRL_ADDR_SHIFT,
            PCE_TBL_CTRL_ADDR_REG_SIZE,
            0x000B);
        if (s != GSW_statusOk)
            return s;
        return GSW_statusOk;
    }
    mac_rd_index++;
    if ((parm->bInitial == 0) && (mac_rd_index >= 1)) {
        s = gsw_reg_wr(pdev,
            PCE_TBL_CTRL_ADDR_OFFSET,
            PCE_TBL_CTRL_ADDR_SHIFT,
            PCE_TBL_CTRL_ADDR_REG_SIZE,
            0xC00B);
        if (s != GSW_statusOk)
            return s;
        s = pctrl_bas_busy(pdev);
        if (s != GSW_statusOk)
            return s;
        s = gsw_reg_rd(pdev,
            PCE_TBL_ADDR_ADDR_OFFSET,
            PCE_TBL_ADDR_ADDR_SHIFT,
            PCE_TBL_ADDR_ADDR_SIZE, &r);
        if (s != GSW_statusOk)
            return s;
        if (firstmac == r) {
            s = mac_br_rd(pdev, &pt);
            if (s != GSW_statusOk)
                return s;
        if ((pt.valid == 1)) {
            parm->nFId = pt.key[3] & 0x3F;
            parm->bStaticEntry = (pt.val[1] & 0x1);
            if (parm->bStaticEntry == 1) {
                parm->nAgeTimer = 0;
                parm->nPortId = pt.val[0];
            } else {
                u32 timer = 300, mant;
                ur rreg;
                /* Aging Counter Mantissa Value */
                s = gsw_reg_rd(pdev,
                    PCE_AGE_1_MANT_OFFSET,
                    PCE_AGE_1_MANT_SHIFT,
                    PCE_AGE_1_MANT_SIZE,
                    &rreg);
                if (s != GSW_statusOk)
                    return s;
                mant = rreg;
                switch (mant) {
                case AGETIMER_1_DAY:
                    timer = 86400;
                    break;
                case AGETIMER_1_HOUR:
                    timer = 3600;
                    break;
                case AGETIMER_300_SEC:
                    timer = 300;
                    break;
                case AGETIMER_10_SEC:
                    timer = 10;
                    break;
                case AGETIMER_1_SEC:
                    timer = 1;
                    break;
                }
                parm->nAgeTimer =   pt.val[0] & 0xF;
                parm->nAgeTimer =   (timer * parm->nAgeTimer)/0xF;
                parm->nPortId = (pt.val[0] >> 4) & 0xF;
            }
            parm->nMAC[0] = pt.key[2] >> 8;
            parm->nMAC[1] = pt.key[2] & 0xFF;
            parm->nMAC[2] = pt.key[1] >> 8;
            parm->nMAC[3] = pt.key[1] & 0xFF;
            parm->nMAC[4] = pt.key[0] >> 8;
            parm->nMAC[5] = pt.key[0] & 0xFF;
            parm->nSVLAN_Id = ((pt.val[1] >> 4) & 0xFFF);
        }
            parm->bLast = 1;
            mac_rd_index = 0;
            s = gsw_reg_wr(pdev,
                PCE_TBL_CTRL_ADDR_OFFSET,
                PCE_TBL_CTRL_ADDR_SHIFT,
                PCE_TBL_CTRL_ADDR_REG_SIZE,
                0x000B);
            if (s != GSW_statusOk)
                return s;
            return GSW_statusOk;
        }
    }
    s = mac_br_rd(pdev, &pt);
    if (s != GSW_statusOk)
        return s;
    if ((pt.valid == 1)) {
        parm->nFId = pt.key[3] & 0x3F;
        parm->bStaticEntry = (pt.val[1] & 0x1);
        if (parm->bStaticEntry == 1) {
            parm->nAgeTimer = 0;
            parm->nPortId = pt.val[0];
        } else {
            u32 timer = 300, mant;
            ur rreg;
            /* Aging Counter Mantissa Value */
            s = gsw_reg_rd(pdev,
                PCE_AGE_1_MANT_OFFSET,
                PCE_AGE_1_MANT_SHIFT,
                PCE_AGE_1_MANT_SIZE, &rreg);
            if (s != GSW_statusOk)
                return s;
            mant = rreg;
            switch (mant) {
            case AGETIMER_1_DAY:
                timer = 86400;
                break;
            case AGETIMER_1_HOUR:
                timer = 3600;
                break;
            case AGETIMER_300_SEC:
                timer = 300;
                break;
            case AGETIMER_10_SEC:
                timer = 10;
                break;
            case AGETIMER_1_SEC:
                timer = 1;
                break;
            }
            parm->nAgeTimer =   pt.val[0] & 0xF;
            parm->nAgeTimer =   (timer * parm->nAgeTimer)/0xF;
            parm->nPortId = (pt.val[0] >> 4) & 0xF;
        }
        parm->nMAC[0] = pt.key[2] >> 8;
        parm->nMAC[1] = pt.key[2] & 0xFF;
        parm->nMAC[2] = pt.key[1] >> 8;
        parm->nMAC[3] = pt.key[1] & 0xFF;
        parm->nMAC[4] = pt.key[0] >> 8;
        parm->nMAC[5] = pt.key[0] & 0xFF;
        parm->nSVLAN_Id = ((pt.val[1] >> 4) & 0xFFF);
        parm->bInitial = 0;
        parm->bLast = 0;
    } else {
        memset(parm, 0, sizeof(GSW_MAC_tableRead_t));
        parm->bLast = 1;
        s = gsw_reg_wr(pdev,
            PCE_TBL_CTRL_ADDR_OFFSET,
            PCE_TBL_CTRL_ADDR_SHIFT,
            PCE_TBL_CTRL_ADDR_REG_SIZE, 0x000B);
        if (s != GSW_statusOk)
            return s;
    }
    return GSW_statusOk;
}

GSW_return_t GSW_PceRuleWrite(void *pdev,
	GSW_PCE_rule_t *parm)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = pce_rule_write(pdev, &pd->ptft, parm);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}

GSW_return_t GSW_PceRuleDelete(void *pdev,
	GSW_PCE_ruleDelete_t *parm)
{
	GSW_return_t s;
	ethsw_api_dev_t *pd = (ethsw_api_dev_t *)pdev;
	SWAPI_ASSERT(pd == NULL);
	s = pce_pattern_delete(pdev, &pd->ptft, parm->nIndex);
	if (s != GSW_statusOk)
		return s;
	s =  pce_action_delete(pd, &pd->ptft, parm->nIndex);
	if (s != GSW_statusOk)
		return s;
	return GSW_statusOk;
}
