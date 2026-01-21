/*
* Description
*
* Copyright (C) 2016-2020 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include "l2sw_asicdrv_phy.h"
#include "l2sw_asicdrv_led.h"
#include "l2sw_asicdrv_vlan.h"
#include "../sf_eswitch.h"
#include "l2sw_switch.h"
#include "l2sw_error.h"
#include "vlan.h"
#include "port.h"
#include "led.h"
#include "stat.h"
#include "mirror.h"
#include "l2sw_reg.h"


extern struct vlan_entry vlan_entries;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);

#ifdef CONFIG_SWCONFIG
static int
l2sw_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	struct switch_port *port;
	int i;
	l2sw_vlan_cfg_t pVlanCfg;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	l2sw_vlan_get(0, val->port_vlan, &pVlanCfg);
	SF_MDIO_UNLOCK();

	port = &val->value.ports[0];
	val->len = 0;
	for (i = 0; i < L2SW_SWITCH_PORT_NUM; i++) {
		if (!L2SW_PORTMASK_IS_PORT_SET(pVlanCfg.mbr, i))
			continue;

		port->id = i;
		port->flags = L2SW_PORTMASK_IS_PORT_SET(pVlanCfg.untag, i) ?
					0 : BIT(SWITCH_PORT_FLAG_TAGGED);
		val->len++;
		port++;
	}

	return 0;
}

static int
l2sw_set_vlan_ports(struct switch_dev *dev, struct switch_val *val) {
	l2sw_vlan_cfg_t pVlanCfg;
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i, mbr_list, untag_list, member = 0;

	memset(&pVlanCfg, 0x0, sizeof(l2sw_vlan_cfg_t));
	if (val->port_vlan < 0 || val->len > L2SW_SWITCH_PORT_NUM)
		return -EINVAL;

	if ((val->port_vlan >= 4000) && (val->port_vlan < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= L2SW_SWITCH_PORT_NUM)
			return -EINVAL;

		if (!(p->flags & BIT(SWITCH_PORT_FLAG_TAGGED)))
			L2SW_PORTMASK_PORT_SET(pVlanCfg.untag, p->id);

		member |= BIT(p->id);
		L2SW_PORTMASK_PORT_SET(pVlanCfg.mbr, p->id);
	}

	pVlanCfg.fid_msti = val->port_vlan;
	l2sw_switch_portmask_L2P_get(0, &pVlanCfg.mbr, &mbr_list);
	l2sw_switch_portmask_L2P_get(0, &pVlanCfg.untag, &untag_list);
	printk("add mbr_list:0x%x untag_list:0x%x for vlan:%d fid:%d\n",
			mbr_list, untag_list, val->port_vlan, val->port_vlan);
	SF_MDIO_LOCK();
	l2sw_vlan_set(0, val->port_vlan, &pVlanCfg);
	SF_MDIO_UNLOCK();

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			entry->member = member;
			return 0;
		}
	}

	entry = (struct vlan_entry *)kzalloc(sizeof(struct vlan_entry), GFP_KERNEL);
	entry->vid = val->port_vlan;
	entry->member = member;
	list_add(&(entry->entry_list), &(vlan_entries.entry_list));
	return 0;
}

static int l2sw_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	l2sw_pri_t priority;

	SF_MDIO_LOCK();
	l2sw_vlan_portPvid_get(0, port, val, &priority);
	SF_MDIO_UNLOCK();
	return 0;
}

static int l2sw_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	if ((pvid >= 4000) && (pvid < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

	//specify the priority to 0 if you don’t turn on the QoS
	printk("set port:%d pvid:%d\n", port, pvid);
	SF_MDIO_LOCK();
	l2sw_vlan_portPvid_set(0, port, pvid, 0);
	SF_MDIO_UNLOCK();
	return 0;
}

static int l2sw_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	l2sw_api_ret_t retVal;
	l2sw_mode_ext_t extPortMode;
	l2sw_port_mac_ability_t extPortStatus;
	l2sw_port_linkStatus_t linkStatus;
	l2sw_port_speed_t speed;
	l2sw_port_duplex_t duplex;

	if (l2sw_switch_isUtpPort(0, port) == L2SW_ERR_OK) {
		SF_MDIO_LOCK();
		retVal = l2sw_port_phyStatus_get(0, port, &linkStatus,
					&speed, &duplex);
		SF_MDIO_UNLOCK();
		if (retVal != L2SW_ERR_OK)
			return retVal;
		link->link = linkStatus;
		link->duplex = duplex;
		link->speed = (speed == PORT_SPEED_1000M) ?
			SWITCH_PORT_SPEED_1000 : (speed == PORT_SPEED_100M) ?
			SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;
	}else if (!l2sw_switch_isExtPort(0, port) && port == dev->cpu_port) {
		memset(&extPortStatus, 0, sizeof(extPortStatus));
		SF_MDIO_LOCK();
		retVal = l2sw_port_macForceLinkExt_get(0, port, &extPortMode,
				&extPortStatus);
		SF_MDIO_UNLOCK();
		if (retVal != L2SW_ERR_OK)
			return retVal;
		link->link = extPortStatus.link;
		link->duplex = extPortStatus.duplex;
		link->speed = (extPortStatus.speed == PORT_SPEED_1000M) ?
			SWITCH_PORT_SPEED_1000 :
			(extPortStatus.speed == PORT_SPEED_100M) ?
			SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;
	} else
		return -EINVAL;

	//printk("get port:%d linkStatus:%d duplex:%d speed:%d\n",
	//		port, link->link, link->duplex, link->speed);

	return 0;
}

static int l2sw_apply_vlan_config(struct switch_dev *dev)
{
	struct sf_eswitch_priv *pesw_priv = container_of(dev, struct sf_eswitch_priv, swdev);
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int port_list = 0;

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		port_list |= entry->member;
	}

	// check if cpu port include, if not return here
	if (!(port_list & (1 << pesw_priv->swdev.cpu_port))) {
		printk("cpu port not correct, port_list:0x%x\n", port_list);
		return 0;
	}

	if (port_list != 0)
		pesw_priv->port_list = port_list & ~(1 << pesw_priv->swdev.cpu_port);

	return 0;
}

static int l2sw_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;

	SF_MDIO_LOCK();
	l2sw_vlan_reset(0);
	SF_MDIO_UNLOCK();

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;
}

static int l2sw_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int l2sw_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int l2sw_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	l2sw_vlan_cfg_t pVlanCfg;
	l2sw_enable_t pEnable;
	l2sw_fid_t pFid;
	int i;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	l2sw_vlan_get(0, val->port_vlan, &pVlanCfg);
	for (i = 0; i < L2SW_SWITCH_PORT_NUM; i++)
	{
		if (L2SW_PORTMASK_IS_PORT_SET(pVlanCfg.mbr, i))
		{
			l2sw_vlan_portFid_get(0, i, &pEnable, &pFid);
			val->value.i = pFid;
			break;
		}
	}
	SF_MDIO_UNLOCK();

	return 0;
}

static int l2sw_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	int i;
	if (val->port_vlan < 0 || val->len > L2SW_SWITCH_PORT_NUM)
		return -EINVAL;

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= L2SW_SWITCH_PORT_NUM)
			return -EINVAL;

		printk("set port:%d fid:%d\n", p->id, val->value.i);
		SF_MDIO_LOCK();
		l2sw_vlan_portFid_set(0, p->id, 1, val->value.i);
		SF_MDIO_UNLOCK();
	}

	return 0;
}

static struct switch_attr l2sw_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = l2sw_get_vlan_enable,
		.set = l2sw_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}
};

static struct switch_attr l2sw_port[] = {
};

static struct switch_attr l2sw_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = l2sw_set_vlan_fid,
		.get = l2sw_get_vlan_fid,
	},
};


struct switch_dev_ops l2sw_switch_ops = {
	.attr_global = {
		.attr = l2sw_globals,
		.n_attr = ARRAY_SIZE(l2sw_globals),
	},
	.attr_port = {
		.attr = l2sw_port,
		.n_attr = ARRAY_SIZE(l2sw_port),
	},
	.attr_vlan = {
		.attr = l2sw_vlan,
		.n_attr = ARRAY_SIZE(l2sw_vlan),
	},

	.get_vlan_ports = l2sw_get_vlan_ports,
	.set_vlan_ports = l2sw_set_vlan_ports,
	.get_port_pvid = l2sw_get_port_pvid,
	.set_port_pvid = l2sw_set_port_pvid,
	.get_port_link = l2sw_get_port_link,
	.apply_config = l2sw_apply_vlan_config,
	.reset_switch = l2sw_reset_vlan,
};
#endif

int l2sw_check_phy_link(int port)
{
	l2sw_api_ret_t retVal;
	l2sw_uint32 phyData;

	SF_MDIO_LOCK();
	retVal = l2sw_getAsicPHYReg(0, l2sw_switch_port_L2P_get(0, port),
				PHY_RESOLVED_REG, &phyData);
	SF_MDIO_UNLOCK();
	if (retVal != L2SW_ERR_OK)
		return retVal;

	return (phyData & (1<<2)) ? 1 : 0;
}

int l2sw_extPort_rgmii_init(int port) {
	int retVal;
	int ext_id;
	int regValue;
	int reg_data = 0, mode = 1, forcemode = 1, nway = 0, link = 1,
	    speed = 2, duplex = 1, txpause = 1, rxpause = 1;

	ext_id = port - 15;

	if( (retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
					ext_id, 0)) != 0)
		return retVal;

	if( (retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != 0)
		return retVal;

	if( (retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != 0)
		return retVal;

	if((retVal = l2sw_setAsicRegBits(0, L2SW_REG_DIGITAL_INTERFACE_SELECT,
			L2SW_SELECT_GMII_0_MASK <<
			(ext_id * L2SW_SELECT_GMII_1_OFFSET), mode)) != 0)
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

	if (l2sw_getAsicReg(0, L2SW_REG_REG_TO_ECO4, &regValue))
		return 0;

	if((regValue & (0x0001 << 5)) && (regValue & (0x0001 << 7)))
	{
		return 0;
	}

	if((retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_SGMII_FDUP_OFFSET, duplex)) != 0)
		return retVal;

	if((retVal = l2sw_setAsicRegBits(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_SGMII_SPD_MASK, speed)) != 0)
		return retVal;

	if((retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_SGMII_LINK_OFFSET, link)) != 0)
		return retVal;

	if((retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_SGMII_TXFC_OFFSET, txpause)) != 0)
		return retVal;

	if((retVal = l2sw_setAsicRegBit(0, L2SW_REG_BYPASS_LINE_RATE,
				L2SW_CFG_SGMII_RXFC_OFFSET, rxpause)) != 0)
		return retVal;

	printk("End %s\n", __func__);
	return l2sw_setAsicReg(0, L2SW_REG_DIGITAL_INTERFACE0_FORCE
			+ ext_id, reg_data);
}

void l2sw_enable_all_phy(struct sf_eswitch_priv *pesw_priv) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < L2SW_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		l2sw_getAsicPHYReg(0, i, 0, &phyData);
		phyData &= ~PHY_CTRL_ENABLE_POWER_DOWN;
		l2sw_setAsicPHYReg(0, i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

void l2sw_disable_all_phy(void) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < L2SW_PHY_PORT_NUM; i++) {
		l2sw_getAsicPHYReg(0, i, 0, &phyData);
		phyData |= PHY_CTRL_ENABLE_POWER_DOWN;
		l2sw_setAsicPHYReg(0, i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

#ifdef CONFIG_SF19A28_V2
#define    L2SW_REG_EXT1_RGMXF    0x1307
#define    L2SW_REG_EXT2_RGMXF    0x13c5
static int l2sw_port_rgmiiDelayExt_set(int port, int txDelay, int rxDelay)
{
	int retVal;
	int regAddr, regData;

	if ((txDelay > 1) || (rxDelay > 7))
		return -1;

	if(port == 16)
		regAddr = L2SW_REG_EXT1_RGMXF;
	else if(port == 17)
		regAddr = L2SW_REG_EXT2_RGMXF;
	else
		return -1;

	if ((retVal = l2sw_getAsicReg(0, regAddr, &regData)) != 0)
		return retVal;

	regData = (regData & 0xFFF0) | ((txDelay << 3) & 0x0008)
		| (rxDelay & 0x0007);

	if ((retVal = l2sw_setAsicReg(0, regAddr, regData)) != 0)
		return retVal;

	return 0;
}
#endif

void l2sw_led_init(int led_mode)
{
	SF_MDIO_LOCK();
	switch (led_mode) {
		case LED_NORMAL:
			{
#if 0
				l2sw_led_ability_t pAbility;
				pAbility.link_10m = ENABLED;
				pAbility.link_100m = ENABLED;
				pAbility.link_500m = ENABLED;
				pAbility.link_1000m = ENABLED;
				pAbility.act_rx = ENABLED;
				pAbility.act_tx = ENABLED;
				l2sw_setAsicForceGroupLed(0, L2SW_LEDGROUPMASK, LEDFORCEMODE_NORMAL);
				//TODO
				//l2sw_led_groupAbility_set(LED_GROUP_1, &pAbility);
				l2sw_led_groupConfig_set(0, LED_GROUP_1, LED_CONFIG_ACT);
#endif
			}
			break;

		case LED_ALL_ON:
			l2sw_setAsicForceGroupLed(0, L2SW_LEDGROUPMASK, LEDFORCEMODE_ON);
			break;

		case LED_ALL_OFF:
			l2sw_setAsicForceGroupLed(0, L2SW_LEDGROUPMASK, LEDFORCEMODE_OFF);
			break;

		case LED_ALL_BLINK:
			l2sw_setAsicForceGroupLed(0, L2SW_LEDGROUPMASK, LEDFORCEMODE_BLINK);
			break;

		default:
			printk("unknown led mode\n");
			break;
	}
	SF_MDIO_UNLOCK();
}

void l2sw_ifg_init(void)
{
	// no method to set ifg
	return;
}

u32 l2sw_get_cpu_port_rx_mib(void)
{
	l2sw_stat_counter_t count = 0;
	SF_MDIO_LOCK();
	l2sw_stat_port_get(0, EXT_PORT0, STAT_IfInUcastPkts, &count);
	// clear mib count after read
	l2sw_stat_port_reset(0, EXT_PORT0);
	SF_MDIO_UNLOCK();
	return count;
}

int l2sw_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
{
	unsigned int i = 0, test_port = UTP_PORT1, regValue = 0;

	if (enable) {
		for (i = 0; i < L2SW_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			if (l2sw_check_phy_link(i) == 0)
			{
				test_port = i;
				break;
			}
		}

		if (i == L2SW_PHY_PORT_NUM) {
			printk("all port has linked, not do gmac auto calibration\n");
			return -1;
		}

		SF_MDIO_LOCK();
		// set test_port loopback insted of mirror
		l2sw_port_macLocalLoopbackEnable_set(0, test_port, 1);
		l2sw_setAsicVlanFilter(0, DISABLED);

		// set other port disable
		for (i = 0; i < L2SW_PHY_PORT_NUM; i++) {
			if (i != test_port) {
				l2sw_getAsicPHYReg(0, i, 0, &regValue);
				regValue |= PHY_CTRL_ENABLE_POWER_DOWN;
				l2sw_setAsicPHYReg(0, i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
		// must add delay here, or gmac calibration will fail
		mdelay(100);
	} else {
		SF_MDIO_LOCK();
		// loopback port will be linked, so use port here
		l2sw_port_macLocalLoopbackEnable_set(0, port, 0);
		l2sw_setAsicVlanFilter(0, ENABLED);
		for (i = 0; i < L2SW_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			// enable other port
			if (i != port) {
				l2sw_getAsicPHYReg(0, i, 0, &regValue);
				regValue &= ~PHY_CTRL_ENABLE_POWER_DOWN;
				l2sw_setAsicPHYReg(0, i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
	}
	return test_port;
}

void l2sw_init(struct sf_eswitch_priv *pesw_priv)
{
	//int phyData = 0;
	//l2sw_port_t port;

#ifdef CONFIG_SF19A28_V2
	l2sw_port_rgmiiDelayExt_set(17, 1, 1);
#endif
	l2sw_enable_all_phy(pesw_priv);

	l2sw_switch_init(0);
}

void l2sw_deinit(struct sf_eswitch_priv *pesw_priv) {
		l2sw_disable_all_phy();
	return;
}

struct sf_eswitch_api_t nf2507_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &l2sw_switch_ops,
#endif
	.vender_init = l2sw_init,
	.vender_deinit = l2sw_deinit,
	.led_init = l2sw_led_init,
	.ifg_init = l2sw_ifg_init,
	.enable_all_phy = l2sw_enable_all_phy,
	.disable_all_phy = l2sw_disable_all_phy,
	.check_phy_linkup = l2sw_check_phy_link,
	.get_cpu_port_rx_mib = l2sw_get_cpu_port_rx_mib,
	.set_cpu_port_self_mirror = l2sw_set_cpu_port_self_mirror,
	.getAsicReg = sf_l2sw_getAsicReg,
	.setAsicReg = sf_l2sw_setAsicReg,
	.getAsicPHYReg = sf_l2sw_getAsicPHYReg,
	.setAsicPHYReg = sf_l2sw_setAsicPHYReg,
};
