/*
* Description
*
* Copyright (C) 2016-2020 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include "rtl8367c_asicdrv_phy.h"
#include "rtl8367c_asicdrv_led.h"
#include "rtl8367c_asicdrv_vlan.h"
#include "../sf_eswitch.h"
#include "rtk_switch.h"
#include "rtk_error.h"
#include "vlan.h"
#include "port.h"
#include "led.h"
#include "stat.h"
#include "mirror.h"
#include "../realtek8367c_src/l2.h"

extern struct vlan_entry vlan_entries;
extern int rtk_rgmii_port;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);

#ifdef CONFIG_SWCONFIG
static int
rtk8367c_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	struct switch_port *port;
	int i;
	rtk_vlan_cfg_t pVlanCfg;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	rtk_vlan_get(val->port_vlan, &pVlanCfg);
	SF_MDIO_UNLOCK();

	port = &val->value.ports[0];
	val->len = 0;
	for (i = 0; i < RTK_SWITCH_PORT_NUM; i++) {
		if (!RTK_PORTMASK_IS_PORT_SET(pVlanCfg.mbr, i))
			continue;

		port->id = i;
		port->flags = RTK_PORTMASK_IS_PORT_SET(pVlanCfg.untag, i) ?
					0 : BIT(SWITCH_PORT_FLAG_TAGGED);
		val->len++;
		port++;
	}

	return 0;
}

static int
rtk8367c_set_vlan_ports(struct switch_dev *dev, struct switch_val *val) {
	rtk_vlan_cfg_t pVlanCfg;
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i, mbr_list, untag_list, member = 0;

	memset(&pVlanCfg, 0x0, sizeof(rtk_vlan_cfg_t));
	if (val->port_vlan < 0 || val->len > RTK_SWITCH_PORT_NUM)
		return -EINVAL;

	if ((val->port_vlan >= 4000) && (val->port_vlan < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= RTK_SWITCH_PORT_NUM)
			return -EINVAL;

		if (!(p->flags & BIT(SWITCH_PORT_FLAG_TAGGED)))
			RTK_PORTMASK_PORT_SET(pVlanCfg.untag, p->id);

		member |= BIT(p->id);
		RTK_PORTMASK_PORT_SET(pVlanCfg.mbr, p->id);
	}

	pVlanCfg.fid_msti = val->port_vlan;
	rtk_switch_portmask_L2P_get(&pVlanCfg.mbr, &mbr_list);
	rtk_switch_portmask_L2P_get(&pVlanCfg.untag, &untag_list);
	printk("add mbr_list:0x%x untag_list:0x%x for vlan:%d fid:%d\n",
			mbr_list, untag_list, val->port_vlan, val->port_vlan);
	SF_MDIO_LOCK();
	rtk_vlan_set(val->port_vlan, &pVlanCfg);
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

static int rtk8367c_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	rtk_pri_t priority;

	SF_MDIO_LOCK();
	rtk_vlan_portPvid_get(port, val, &priority);
	SF_MDIO_UNLOCK();
	return 0;
}

static int rtk8367c_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	if ((pvid >= 4000) && (pvid < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

	//specify the priority to 0 if you don’t turn on the QoS
	printk("set port:%d pvid:%d\n", port, pvid);
	SF_MDIO_LOCK();
	rtk_vlan_portPvid_set(port, pvid, 0);
	SF_MDIO_UNLOCK();
	return 0;
}

static int rtk8367c_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	rtk_api_ret_t retVal;
	rtk_mode_ext_t extPortMode;
	rtk_port_mac_ability_t extPortStatus;
	rtk_port_linkStatus_t linkStatus;
	rtk_port_speed_t speed;
	rtk_port_duplex_t duplex;

	if (rtk_switch_isUtpPort(port) == RT_ERR_OK) {
		SF_MDIO_LOCK();
		retVal = rtk_port_phyStatus_get(port, &linkStatus,
					&speed, &duplex);
		SF_MDIO_UNLOCK();
		if (retVal != RT_ERR_OK)
			return retVal;
		link->link = linkStatus;
		link->duplex = duplex;
		link->speed = (speed == PORT_SPEED_1000M) ?
			SWITCH_PORT_SPEED_1000 : (speed == PORT_SPEED_100M) ?
			SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;
	}else if (!rtk_switch_isExtPort(port) && port == dev->cpu_port) {
		memset(&extPortStatus, 0, sizeof(extPortStatus));
		SF_MDIO_LOCK();
		retVal = rtk_port_macForceLinkExt_get(port, &extPortMode,
				&extPortStatus);
		SF_MDIO_UNLOCK();
		if (retVal != RT_ERR_OK)
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

static int rtk8367c_apply_vlan_config(struct switch_dev *dev)
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

static int rtk8367c_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;

	SF_MDIO_LOCK();
	rtk_vlan_reset();
	SF_MDIO_UNLOCK();

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;
}

static int rtk8367c_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int rtk8367c_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int rtk8367c_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	rtk_vlan_cfg_t pVlanCfg;
	rtk_enable_t pEnable;
	rtk_fid_t pFid;
	int i;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	rtk_vlan_get(val->port_vlan, &pVlanCfg);
	for (i = 0; i < RTK_SWITCH_PORT_NUM; i++)
	{
		if (RTK_PORTMASK_IS_PORT_SET(pVlanCfg.mbr, i))
		{
			rtk_vlan_portFid_get(i, &pEnable, &pFid);
			val->value.i = pFid;
			break;
		}
	}
	SF_MDIO_UNLOCK();

	return 0;
}

static int rtk8367c_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	int i;
	if (val->port_vlan < 0 || val->len > RTK_SWITCH_PORT_NUM)
		return -EINVAL;

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= RTK_SWITCH_PORT_NUM)
			return -EINVAL;

		printk("set port:%d fid:%d\n", p->id, val->value.i);
		SF_MDIO_LOCK();
		rtk_vlan_portFid_set(p->id, 1, val->value.i);
		SF_MDIO_UNLOCK();
	}

	return 0;
}

static struct switch_attr rtk8367c_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = rtk8367c_get_vlan_enable,
		.set = rtk8367c_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}
};

static struct switch_attr rtk8367c_port[] = {
};

static struct switch_attr rtk8367c_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = rtk8367c_set_vlan_fid,
		.get = rtk8367c_get_vlan_fid,
	},
};


struct switch_dev_ops rtk8367c_switch_ops = {
	.attr_global = {
		.attr = rtk8367c_globals,
		.n_attr = ARRAY_SIZE(rtk8367c_globals),
	},
	.attr_port = {
		.attr = rtk8367c_port,
		.n_attr = ARRAY_SIZE(rtk8367c_port),
	},
	.attr_vlan = {
		.attr = rtk8367c_vlan,
		.n_attr = ARRAY_SIZE(rtk8367c_vlan),
	},

	.get_vlan_ports = rtk8367c_get_vlan_ports,
	.set_vlan_ports = rtk8367c_set_vlan_ports,
	.get_port_pvid = rtk8367c_get_port_pvid,
	.set_port_pvid = rtk8367c_set_port_pvid,
	.get_port_link = rtk8367c_get_port_link,
	.apply_config = rtk8367c_apply_vlan_config,
	.reset_switch = rtk8367c_reset_vlan,
};
#endif

int rtk8367c_check_phy_link(int port)
{
	rtk_api_ret_t retVal;
	rtk_uint32 phyData;

	SF_MDIO_LOCK();
	retVal = rtl8367c_getAsicPHYReg(rtk_switch_port_L2P_get(port),
				PHY_RESOLVED_REG, &phyData);
	SF_MDIO_UNLOCK();
	if (retVal != RT_ERR_OK)
		return retVal;

	return (phyData & (1<<2)) ? 1 : 0;
}

int rtk8367_extPort_rgmii_init(int port) {
	int retVal;
	int ext_id;
	int regValue;
	int reg_data = 0, mode = 1, forcemode = 1, nway = 0, link = 1,
	    speed = 2, duplex = 1, txpause = 1, rxpause = 1;

	ext_id = port - 15;

	if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_BYPASS_LINE_RATE,
					ext_id, 0)) != 0)
		return retVal;

	if(ext_id == 1) {
		if ((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_MAC8_SEL_SGMII_OFFSET, 0)) != 0)
			return retVal;

		if( (retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_MAC8_SEL_HSGMII_OFFSET, 0)) != 0)
			return retVal;

		if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT,
				RTL8367C_SELECT_GMII_0_MASK <<
				(ext_id * RTL8367C_SELECT_GMII_1_OFFSET), mode)) != 0)
			return retVal;
	} else if(ext_id == 2) {
		if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_DIGITAL_INTERFACE_SELECT_1,
					RTL8367C_SELECT_GMII_2_MASK, mode)) != 0)
			return retVal;

		if ((retVal = rtl8367c_getAsicReg(RTL8367C_REG_DIGITAL_INTERFACE2_FORCE,
					&reg_data))!= 0)
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

	if(ext_id == 1) {
		if (rtl8367c_getAsicReg(RTL8367C_REG_REG_TO_ECO4, &regValue))
			return 0;

		if((regValue & (0x0001 << 5)) && (regValue & (0x0001 << 7)))
			return 0;

		if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_SGMII_FDUP_OFFSET, duplex)) != 0)
			return retVal;

		if((retVal = rtl8367c_setAsicRegBits(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_SGMII_SPD_MASK, speed)) != 0)
			return retVal;

		if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_SGMII_LINK_OFFSET, link)) != 0)
			return retVal;

		if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_SGMII_TXFC_OFFSET, txpause)) != 0)
			return retVal;

		if((retVal = rtl8367c_setAsicRegBit(RTL8367C_REG_SDS_MISC,
					RTL8367C_CFG_SGMII_RXFC_OFFSET, rxpause)) != 0)
			return retVal;

		retVal = rtl8367c_setAsicReg(RTL8367C_REG_DIGITAL_INTERFACE0_FORCE
			+ ext_id, reg_data);
	} else if(ext_id == 2) {
		retVal = rtl8367c_setAsicReg(RTL8367C_REG_DIGITAL_INTERFACE2_FORCE, reg_data);
	}

	printk("End %s\n", __func__);
	return retVal;
}

void rtk8367_enable_all_phy(struct sf_eswitch_priv *pesw_priv) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		rtl8367c_getAsicPHYReg(i, 0, &phyData);
		phyData &= ~PHY_CTRL_ENABLE_POWER_DOWN;
		rtl8367c_setAsicPHYReg(i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

void rtk8367_disable_all_phy(void) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
		rtl8367c_getAsicPHYReg(i, 0, &phyData);
		phyData |= PHY_CTRL_ENABLE_POWER_DOWN;
		rtl8367c_setAsicPHYReg(i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

#ifdef CONFIG_SF19A28_V2
#define    RTL8367C_REG_EXT1_RGMXF    0x1307
#define    RTL8367C_REG_EXT2_RGMXF    0x13c5
static int rtk8367_port_rgmiiDelayExt_set(int port, int txDelay, int rxDelay)
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

	if ((retVal = rtl8367c_getAsicReg(regAddr, &regData)) != 0)
		return retVal;

	regData = (regData & 0xFFF0) | ((txDelay << 3) & 0x0008)
		| (rxDelay & 0x0007);

	if ((retVal = rtl8367c_setAsicReg(regAddr, regData)) != 0)
		return retVal;

	return 0;
}
#endif

void rtk8367c_led_init(int led_mode)
{
	SF_MDIO_LOCK();
	switch (led_mode) {
		case LED_NORMAL:
			{
				rtk_led_ability_t pAbility;
				pAbility.link_10m = ENABLED;
				pAbility.link_100m = ENABLED;
				pAbility.link_500m = ENABLED;
				pAbility.link_1000m = ENABLED;
				pAbility.act_rx = ENABLED;
				pAbility.act_tx = ENABLED;
				rtl8367c_setAsicForceGroupLed(RTL8367C_LEDGROUPMASK, LEDFORCEMODE_NORMAL);
				rtk_led_groupAbility_set(LED_GROUP_1, &pAbility);
				rtk_led_groupConfig_set(LED_GROUP_1, LED_CONFIG_ACT);
			}
			break;

		case LED_ALL_ON:
			rtl8367c_setAsicForceGroupLed(RTL8367C_LEDGROUPMASK, LEDFORCEMODE_ON);
			break;

		case LED_ALL_OFF:
			rtl8367c_setAsicForceGroupLed(RTL8367C_LEDGROUPMASK, LEDFORCEMODE_OFF);
			break;

		case LED_ALL_BLINK:
			rtl8367c_setAsicForceGroupLed(RTL8367C_LEDGROUPMASK, LEDFORCEMODE_BLINK);
			break;

		default:
			printk("unknown led mode\n");
			break;
	}
	SF_MDIO_UNLOCK();
}

void rtk8367c_ifg_init(void)
{
	// no method to set ifg
	return;
}

u32 rtk8367c_get_cpu_port_rx_mib(void)
{
	rtk_stat_counter_t count = 0;
	SF_MDIO_LOCK();
	rtk_stat_port_get(rtk_rgmii_port, STAT_IfInUcastPkts, &count);
	// clear mib count after read
	rtk_stat_port_reset(rtk_rgmii_port);
	SF_MDIO_UNLOCK();
	return count;
}

int rtk8367c_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
{
	unsigned int i = 0, test_port = UTP_PORT1, regValue = 0, timer = 0;

	if (enable) {
		for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			if (rtk8367c_check_phy_link(i) == 0)
			{
				test_port = i;
				break;
			}
		}

		if (i == RTK_PHY_PORT_NUM) {
			printk("all port has linked, not do gmac auto calibration\n");
			return -1;
		}

		SF_MDIO_LOCK();
		// set test_port phy loopback and disable phy auto-negotiation
		rtl8367c_getAsicPHYReg(test_port, 0, &regValue);
		regValue &= ~(1 << 12); // disable auto-negotiation to speed up loopback link
		rtl8367c_setAsicPHYReg(test_port, 0, regValue);
		rtk_port_macLocalLoopbackEnable_set(test_port, 1);
		rtl8367c_setAsicVlanFilter(DISABLED);

		// set other port disable
		for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
			if (i != test_port) {
				rtl8367c_getAsicPHYReg(i, 0, &regValue);
				regValue |= PHY_CTRL_ENABLE_POWER_DOWN;
				rtl8367c_setAsicPHYReg(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
		// must add delay here, or gmac calibration will fail
		while(timer < 2000) {
			mdelay(100);
			timer += 100;
			if (rtk8367c_check_phy_link(test_port))
				break;
		}
	} else {
		SF_MDIO_LOCK();
		// loopback port will be linked, so use port here
		rtk_port_macLocalLoopbackEnable_set(port, 0);
		rtl8367c_setAsicVlanFilter(ENABLED);
		rtl8367c_getAsicPHYReg(port, 0, &regValue);
		regValue |= (1 << 12);
		rtl8367c_setAsicPHYReg(port, 0, regValue);
		for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			// enable other port
			if (i != port) {
				rtl8367c_getAsicPHYReg(i, 0, &regValue);
				regValue &= ~PHY_CTRL_ENABLE_POWER_DOWN;
				rtl8367c_setAsicPHYReg(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
	}
	return test_port;
}

void rtk8367c_init(struct sf_eswitch_priv *pesw_priv)
{
	int phyData = 0;
	rtk_port_t port;

	rtk8367_extPort_rgmii_init(rtk_rgmii_port);
#ifdef CONFIG_SF19A28_V2
	rtk8367_port_rgmiiDelayExt_set(16, 1, 1);
#endif
	rtk8367_enable_all_phy(pesw_priv);

	rtk_switch_init();
	rtk_vlan_init();
	// disable flow control for ixia test
	RTK_SCAN_ALL_LOG_PORT(port)
	{
		if(rtk_switch_isUtpPort(port) == RT_ERR_OK)
		{
			rtl8367c_getAsicPHYReg(port,
					RTL8367C_PHY_AUTONEG_ADV_REG, &phyData);
			rtl8367c_setAsicPHYReg(port,
					RTL8367C_PHY_AUTONEG_ADV_REG, phyData &
					(~RTL8367C_PHY_FLOW_CONTROL_EN));
			// reset phy to take effect
			rtl8367c_getAsicPHYReg(port, RTL8367C_PHY_CONTROL_REG,
					&phyData);
			rtl8367c_setAsicPHYReg(port, RTL8367C_PHY_CONTROL_REG,
					(phyData | RTL8367C_PHY_RESET_EN));
		}
	}
}

void rtk8367c_deinit(struct sf_eswitch_priv *pesw_priv) {
		rtk8367_disable_all_phy();
	return;
}

void rtk8367c_dumpmac(char macaddr[], int port)
{
	rtk_uint32 address = 0;
	rtk_l2_ucastAddr_t l2_data;

	SF_MDIO_LOCK();
	while(1){
		if((rtk_l2_addr_next_get(READMETHOD_NEXT_L2UCSPA, port, &address, &l2_data))!=RT_ERR_OK)
			break;
		sprintf(macaddr,"%pM", l2_data.mac.octet);
		address++;
	}
	SF_MDIO_UNLOCK();
}

struct sf_eswitch_api_t rtk8367c_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &rtk8367c_switch_ops,
#endif
	.vender_init = rtk8367c_init,
	.vender_deinit = rtk8367c_deinit,
	.led_init = rtk8367c_led_init,
	.ifg_init = rtk8367c_ifg_init,
	.enable_all_phy = rtk8367_enable_all_phy,
	.disable_all_phy = rtk8367_disable_all_phy,
	.check_phy_linkup = rtk8367c_check_phy_link,
	.get_cpu_port_rx_mib = rtk8367c_get_cpu_port_rx_mib,
	.set_cpu_port_self_mirror = rtk8367c_set_cpu_port_self_mirror,
	.setAsicReg = rtl8367c_setAsicReg,
	.getAsicReg = rtl8367c_getAsicReg,
	.setAsicPHYReg = rtl8367c_setAsicPHYReg,
	.getAsicPHYReg = rtl8367c_getAsicPHYReg,
	.dumpmac = rtk8367c_dumpmac,
};
