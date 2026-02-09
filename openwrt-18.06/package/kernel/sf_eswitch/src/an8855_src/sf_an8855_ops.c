/*
* Description
*
* Copyright (C) 2016-2020 Qin.Xia <qin.xia@siflower.com.cn>
*
* Siflower software
*/

#include "../sf_eswitch.h"
#include "an8855_mdio.h"

#ifdef CONFIG_SFAX8_ESWITCH_REDIRECT
#define DHCP_DPORT67_ACL_RULE_IDX    (32)
#define DNS_DPORT53_ACL_RULE_IDX   (33)
#endif

extern struct vlan_entry vlan_entries;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);
extern int notify_link_event(struct sf_eswitch_priv *pesw_priv, int port, int updown, char *ifname, uint8_t *mac, uint16_t vlan_id, bool flag);

AIR_ERROR_NO_T air_printf(C8_T* fmt, ...)
{
	return printk(fmt);
}

void air_udelay(UI32_T us)
{
	udelay(us);
}

void *air_malloc(UI32_T size)
{
	return kmalloc(size, GFP_KERNEL);
}

void air_free(void *ptr)
{
	kfree(ptr);
}

int air_an8855_getAsicReg(unsigned int reg, unsigned int *pValue)
{
	*pValue = an8855_reg_read(reg);
	return 0;
}

int air_an8855_setAsicReg(unsigned int reg, unsigned int pValue)
{
	an8855_reg_write(reg, pValue);
	return 0;
}

AIR_ERROR_NO_T __switch_read(u32 unit, u32 reg, u32 *pValue)
{
	*pValue = an8855_reg_read(reg);
	return 0;
}

AIR_ERROR_NO_T __switch_write(u32 unit, u32 reg, u32 pValue)
{
	an8855_reg_write(reg, pValue);
	return 0;
}

AIR_ERROR_NO_T __switch_phy_read(u32 unit, u32 phyNo, u32 phyAddr, u32 *pRegData)
{
	return an8855_phy_read(phyNo, phyAddr, pRegData);
}

AIR_ERROR_NO_T __switch_phy_write(u32 unit, u32 phyNo, u32 phyAddr, u32 pRegData)
{
	return an8855_phy_write(phyNo, phyAddr, pRegData);
}

AIR_ERROR_NO_T __switch_phy_c45_read(u32 unit, u32 phyNo, u32 devad, u32 phyAddr, u32 *pRegData)
{
	return an8855_phy_read_cl45(phyNo, devad, phyAddr, pRegData);
}

AIR_ERROR_NO_T __switch_phy_c45_write(u32 unit, u32 phyNo, u32 devad, u32 phyAddr, u32 pRegData)
{
	return an8855_phy_write_cl45(phyNo, devad, phyAddr, pRegData);
}


#ifdef CONFIG_SWCONFIG
static int
air_an8855_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	struct vlan_entry *entry, *found = NULL;
	struct list_head *pos;
	struct switch_port *port;
	int i;

	if (val->port_vlan < 0)
		return -EINVAL;

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			found = entry;
			break;
		}
	}

	if (!found)
		return -EINVAL;

	port = &val->value.ports[0];
	val->len = 0;
	for (i = 0; i < AN8855_NUM_PORTS; i++) {
		if (!(found->member & BIT(i)))
			continue;

		port->id = i;
		port->flags = (found->untag & BIT(i)) ?
					0 : BIT(SWITCH_PORT_FLAG_TAGGED);
		val->len++;
		port++;
	}

	return 0;
}

static int
air_an8855_set_vlan_ports(struct switch_dev *dev, struct switch_val *val) {
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i, untag = 0, member = 0;

	if (val->port_vlan < 0 || val->len > AN8855_NUM_PORTS)
		return -EINVAL;

	if ((val->port_vlan >= 4000) && (val->port_vlan < 4032))
		return -EINVAL; // reserve such vlan for hnat to wifi

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= AN8855_NUM_PORTS)
			return -EINVAL;

		if (!(p->flags & BIT(SWITCH_PORT_FLAG_TAGGED)))
			untag |= BIT(p->id);

		member |= BIT(p->id);
	}

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			entry->member = member;
			entry->untag = untag;
			return 0;
		}
	}

	entry = (struct vlan_entry *)kzalloc(sizeof(struct vlan_entry), GFP_KERNEL);
	entry->vid = val->port_vlan;
	entry->member = member;
	entry->untag = untag;
	list_add(&(entry->entry_list), &(vlan_entries.entry_list));
	return 0;
}

static int air_an8855_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	u16 pvid = 0;

	if (port > AN8855_NUM_PORTS || port < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	air_vlan_getPortPVID(0, port, &pvid);
	SF_MDIO_UNLOCK();
	*val = pvid;
	return 0;
}

static int air_an8855_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	if ((pvid >= 4000) && (pvid < 4032))
		return -EINVAL; // reserve such vlan for hnat to wifi

	SF_MDIO_LOCK();
	air_vlan_setPortPVID(0, port, pvid);
	SF_MDIO_UNLOCK();
	return 0;
}

static int air_an8855_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	AIR_PORT_STATUS_T status;

	SF_MDIO_LOCK();
	air_port_getLink(0, port, &status);
	SF_MDIO_UNLOCK();

	link->link = status.link;
	link->duplex = status.duplex;
	link->speed = (status.speed == AIR_PORT_SPEED_1000M) ?
		SWITCH_PORT_SPEED_1000 : (status.speed == AIR_PORT_SPEED_100M) ?
		SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;

	return 0;
}

static int air_an8855_apply_vlan_config(struct switch_dev *dev)
{
	struct sf_eswitch_priv *pesw_priv = container_of(dev, struct sf_eswitch_priv, swdev);
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i, port_list = 0;

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		port_list |= entry->member;
		SF_MDIO_LOCK();
		air_vlan_create(0, entry->vid, NULL);
		air_vlan_setIVL(0, entry->vid, 1);
		air_vlan_setFid(0, entry->vid, entry->vid);
		air_vlan_setMemberPort(0, entry->vid, entry->member);

		air_vlan_setEgsTagCtlEnable(0, entry->vid, 1);
		air_vlan_setEgsTagConsistent(0, entry->vid, 0);

		for (i = 0; i < AN8855_NUM_PORTS; i++) {
			if (!(entry->member & BIT(i)))
				continue;

			if (entry->untag & BIT(i))
				air_vlan_setPortEgsTagCtl(0, entry->vid, i, AIR_VLAN_PORT_EGS_TAG_CTL_TYPE_UNTAGGED);
			else
				air_vlan_setPortEgsTagCtl(0, entry->vid, i, AIR_VLAN_PORT_EGS_TAG_CTL_TYPE_TAGGED);

		}
		SF_MDIO_UNLOCK();
	}

	// enable cpu port flow control
	SF_MDIO_LOCK();
	air_port_setFlowCtrl(0, pesw_priv->swdev.cpu_port, AIR_PORT_TX, TRUE);
	air_port_setFlowCtrl(0, pesw_priv->swdev.cpu_port, AIR_PORT_RX, TRUE);
	SF_MDIO_UNLOCK();

	// check if cpu port include, if not return here
	if (!(port_list & (1 << pesw_priv->swdev.cpu_port))) {
		printk("cpu port not correct, port_list:0x%x\n", port_list);
		return 0;
	}

	if (port_list != 0)
		pesw_priv->port_list = port_list & ~(1 << pesw_priv->swdev.cpu_port);

	return 0;
}

static int air_an8855_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;

	SF_MDIO_LOCK();
	air_vlan_destroyAll(0, 1);
	SF_MDIO_UNLOCK();

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;
}

static int air_an8855_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int air_an8855_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int air_an8855_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	u8 fid;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	air_vlan_getFid(0, val->port_vlan, &fid);
	val->value.i = fid;
	SF_MDIO_UNLOCK();

	return 0;
}

static int air_an8855_set_fld_mode(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	int ptr_fld_en;
	int type;

	if (val->port_vlan < 0)
		return -EINVAL;

	if (!strncmp(val->value.s, "true", 4))
		ptr_fld_en = 1;
	else if (!strncmp(val->value.s, "false", 5))
		ptr_fld_en = 0;
	else
		return -EINVAL;

	if (!strncmp(val->value.s, "true_b", 6))
		type = AIR_FLOOD_TYPE_BCST;
	else if (!strncmp(val->value.s, "false_b", 7))
		type = AIR_FLOOD_TYPE_BCST;
	else if (!strncmp(val->value.s, "true_m", 6))
		type = AIR_FLOOD_TYPE_MCST;
	else if (!strncmp(val->value.s, "false_m", 7))
		type = AIR_FLOOD_TYPE_MCST;
	else if (!strncmp(val->value.s, "true_u", 6))
		type = AIR_FLOOD_TYPE_UCST;
	else if (!strncmp(val->value.s, "false_u", 7))
		type = AIR_FLOOD_TYPE_UCST;
	else if (!strncmp(val->value.s, "true_u", 6))
		type = AIR_FLOOD_TYPE_QURY;
	else if (!strncmp(val->value.s, "false_u", 7))
		type = AIR_FLOOD_TYPE_QURY;
	else
		return -EINVAL;

	SF_MDIO_LOCK();
	air_sec_setFldMode(0, val->port_vlan, type, ptr_fld_en);
	SF_MDIO_UNLOCK();

	return 0;
}

static int air_an8855_get_fld_mode(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	int ptr_fld_en;
	int type;
	char stype[4][5] = {"Bcst", "Mcst", "Ucst", "Qury"};
	static char buf[64];
	int len = 0;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	for (type = 0; type < AIR_FLOOD_TYPE_LAST; type++) {
		air_sec_getFldMode(0, val->port_vlan, type, &ptr_fld_en);
		printk("now the type %s's flood mode is %d.\n", stype[type], ptr_fld_en);
	}
	SF_MDIO_UNLOCK();

	len += snprintf(buf + len, sizeof(buf) - len, "These are the port %d of an8855's flood mode.\n", val->port_vlan);

	val->value.s = buf;
	val->len = len;
 	return 0;
}

static int air_an8855_get_port_fdb_entry(struct switch_dev *dev, const struct switch_attr *attr, struct switch_val *val)
{
	struct sf_eswitch_priv *priv = container_of(dev, struct sf_eswitch_priv, swdev);
	u8 count = 0;
	u32 bucket_size = 0;
	AIR_MAC_ENTRY_T *entry = NULL;
	AIR_ERROR_NO_T ret = AIR_E_OK;
	static char buf[64];
	int len = 0;

	SF_MDIO_LOCK();
	air_l2_getMacBucketSize(0, &bucket_size);
	SF_MDIO_UNLOCK();

	entry = (AIR_MAC_ENTRY_T *)air_malloc(sizeof(AIR_MAC_ENTRY_T) * bucket_size);
	if (entry) {
		memset(entry, 0, sizeof(AIR_MAC_ENTRY_T) * bucket_size);

		SF_MDIO_LOCK();
		for (ret = air_l2_getMacAddr(0, &count, entry); \
		     ret == AIR_E_OK; \
		     ret = air_l2_getNextMacAddr(0, &count, entry)) {
			if (entry->port_bitmap[0] == BIT(val->port_vlan)) {
				printk("dump mac:%pM, vlan_id is %d\n", entry->mac, entry->cvid);
				notify_link_event(priv, val->port_vlan, priv->phy_status[val->port_vlan], "eth0", entry->mac, entry->cvid, false);
			}
			memset(entry, 0, sizeof(AIR_MAC_ENTRY_T) * bucket_size);
		}
		SF_MDIO_UNLOCK();

		air_free(entry);
	}

	len += snprintf(buf + len, sizeof(buf) - len, "These are the port %d of an8855's fdb entry.\n", val->port_vlan);

	val->value.s = buf;
	val->len = len;
	return 0;
}

static int air_an8855_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	air_vlan_setFid(0, val->port_vlan, val->value.i);
	SF_MDIO_UNLOCK();

	return 0;
}

static struct switch_attr air_an8855_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = air_an8855_get_vlan_enable,
		.set = air_an8855_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}
};

static struct switch_attr air_an8855_port[] = {
	{
		.type = SWITCH_TYPE_STRING,
		.name = "fdb_entry",
		.description = "Get the switch's each port's fdb unicast entry",
		.get = air_an8855_get_port_fdb_entry,
	},
	{
		.type = SWITCH_TYPE_STRING,
		.name = "fld_mode",
		.description = "Set/Get the switch's each port's flood mode",
		.set = air_an8855_set_fld_mode,
		.get = air_an8855_get_fld_mode,
	},
};

static struct switch_attr air_an8855_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = air_an8855_set_vlan_fid,
		.get = air_an8855_get_vlan_fid,
	},
};


struct switch_dev_ops air_an8855_switch_ops = {
	.attr_global = {
		.attr = air_an8855_globals,
		.n_attr = ARRAY_SIZE(air_an8855_globals),
	},
	.attr_port = {
		.attr = air_an8855_port,
		.n_attr = ARRAY_SIZE(air_an8855_port),
	},
	.attr_vlan = {
		.attr = air_an8855_vlan,
		.n_attr = ARRAY_SIZE(air_an8855_vlan),
	},

	.get_vlan_ports = air_an8855_get_vlan_ports,
	.set_vlan_ports = air_an8855_set_vlan_ports,
	.get_port_pvid = air_an8855_get_port_pvid,
	.set_port_pvid = air_an8855_set_port_pvid,
	.get_port_link = air_an8855_get_port_link,
	.apply_config = air_an8855_apply_vlan_config,
	.reset_switch = air_an8855_reset_vlan,
};
#endif

int air_an8855_check_phy_link(int port)
{
	AIR_PORT_STATUS_T status = {};

	SF_MDIO_LOCK();
	air_port_getLink(0, port, &status);
	SF_MDIO_UNLOCK();

	return status.link ? 1 : 0;
}

void air_an8855_enable_all_phy(struct sf_eswitch_priv *pesw_priv) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		aml_readPhyReg(0, i, 0, &phyData);
		phyData &= ~PHY_CTRL_ENABLE_POWER_DOWN;
		aml_writePhyReg(0, i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

void air_an8855_disable_all_phy(void) {
	int i, phyData;
	SF_MDIO_LOCK();
	for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
		aml_readPhyReg(0, i, 0, &phyData);
		phyData |= PHY_CTRL_ENABLE_POWER_DOWN;
		aml_writePhyReg(0, i, 0, phyData);
	}
	SF_MDIO_UNLOCK();
}

/*
   port  0    1    2    3
   gpio  3    5    9    0
   LED   D    F    I    A
*/
static void air_led_init(int led_mode)
{
	UI32_T data = 0;

	//gpio inverse
	aml_readReg(0, 0x10000010, &data);
	data &= ~(0x229);

	/*Key configuration, confirm if it is reversed polarity*/
	// data |= (0x208);//0010 0000 1000  bit3=1, bit5=0, bit9=1, bit0=0
	data |= (0x209);   //0010 0000 1001  bit3=1, bit5=0, bit9=1, bit0=1

	aml_writeReg(0, 0x10000010, data);

	//force gpio disable
	aml_readReg(0, 0x1000007c, &data);
	data &= ~(0x229);
	aml_writeReg(0, 0x1000007c, data);

	//gpio led enable
	aml_readReg(0, 0x10000054, &data);
	data |= (0x229);
	aml_writeReg(0, 0x10000054, data);

	//gpio select
	aml_readReg(0, 0x10000058, &data);
	data = (12) + (0 << 24);
	aml_writeReg(0, 0x10000058, data);

	aml_readReg(0, 0x1000005c, &data);
	data = (4 << 8);
	aml_writeReg(0, 0x1000005c, data);

	aml_readReg(0, 0x10000060, &data);
	data = (8);
	aml_writeReg(0, 0x10000060, data);
}

void air_an8855_led_init(int led_mode)
{
	int i;
	SF_MDIO_LOCK();
	switch (led_mode) {
		case LED_NORMAL:
			air_led_init(led_mode);
			break;

		case LED_ALL_ON:
			for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
				air_led_setState(0, i, 0, TRUE);
				air_led_setState(0, i, 1, TRUE);
			}
			break;

		case LED_ALL_OFF:
			for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
				air_led_setState(0, i, 0, FALSE);
				air_led_setState(0, i, 1, FALSE);
			}
			break;

		case LED_ALL_BLINK:
			for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
				air_led_setMode(0, i, AIR_LED_MODE_2LED_MODE0);
			}
			break;

		default:
			printk("unknown led mode\n");
			break;
	}
	SF_MDIO_UNLOCK();
}

void air_an8855_ifg_init(void)
{
	// no method to set ifg
	return;
}

u32 air_an8855_get_cpu_port_rx_mib(void)
{
	AIR_MIB_CNT_RX_T rx_mib;
	AIR_MIB_CNT_TX_T tx_mib;

	SF_MDIO_LOCK();
	air_mib_get(0, AN8855_CPU_PORT, &rx_mib, &tx_mib);
	// clear mib count after read
	air_mib_clear_by_port(0, AN8855_CPU_PORT);
	SF_MDIO_UNLOCK();
	return rx_mib.RUPC;
}

int air_an8855_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
{
	unsigned int i = 0, test_port = 1, regValue = 0, timer = 0;

	if (enable) {
		for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			if (air_an8855_check_phy_link(i) == 0)
			{
				test_port = i;
				break;
			}
		}

		if (i == AN8855_PHY_PORT_NUM) {
			printk("all port has linked, not do gmac auto calibration\n");
			return -1;
		}

		SF_MDIO_LOCK();
		// set test_port phy loopback and disable phy auto-negotiation
		an8855_phy_read(test_port, 0, &regValue);
		regValue &= ~(1 << 12); // disable auto-negotiation to speed up loopback link
		regValue |= (1 << 14);
		an8855_phy_write(test_port, 0, regValue);

		// set other port disable
		for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
			if (i != test_port) {
				an8855_phy_read(i, 0, &regValue);
				regValue |= PHY_CTRL_ENABLE_POWER_DOWN;
				an8855_phy_write(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
		// must add delay here, or gmac calibration will fail
		while(timer < 2000) {
			mdelay(100);
			timer += 100;
			if (air_an8855_check_phy_link(test_port))
				break;
		}
	} else {
		SF_MDIO_LOCK();
		// loopback port will be linked, so use port here
		an8855_phy_read(port, 0, &regValue);
		regValue &= ~(1 << 14);
		regValue |= (1 << 12);
		an8855_phy_write(port, 0, regValue);
		for (i = 0; i < AN8855_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			// enable other port
			if (i != port) {
				an8855_phy_read(i, 0, &regValue);
				regValue &= ~PHY_CTRL_ENABLE_POWER_DOWN;
				an8855_phy_write(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();
	}
	return test_port;
}

void air_port_air_port_setRgmiiDelay(void)
{
	u32 regValue;

	//disable tx delay
	regValue = an8855_reg_read(0x1028C854);
	regValue &= ~BIT(24);
	an8855_reg_write(0x1028C854, regValue);

	//disable rx delay
	regValue = an8855_reg_read(0x1028C84c);
	regValue &= ~BIT(24);
	an8855_reg_write(0x1028C84c, regValue);
}

#ifdef CONFIG_SFAX8_ESWITCH_REDIRECT
void air_an8855_redirect_dhcp(void)
{
	int port;
	AIR_ACL_RULE_T rule;
	AIR_ACL_ACTION_T action;
	AIR_ERROR_NO_T ret = AIR_E_OK;

	memset(&rule, 0, sizeof(rule));
	memset(&action, 0, sizeof(action));

	//ACL enable
	air_acl_setGlobalState(0, TRUE);
	for (port = 0; port < 4; port++)
	{
		air_acl_setPortEnable(0, port, TRUE);
	}

	//ACL rule
	rule.key.protocol = 17; //udp协议
	rule.key.dport = 67;
	rule.key.fieldmap = (1 << AIR_ACL_PROTOCOL) | (1 << AIR_ACL_DPORT);
	rule.key.portmap = 0xf;
	rule.mask.protocol = 0xff;
	rule.mask.dport = 0xffff;
	rule.mask.fieldmap = rule.key.fieldmap;
	rule.mask.portmap = (~rule.key.portmap) & AIR_ALL_PORT_BITMAP;
	rule.ctrl.rule_en = TRUE;
	rule.ctrl.end = TRUE;

	ret = air_acl_setRule(0, DHCP_DPORT67_ACL_RULE_IDX, rule);
	if(ret)
		pr_info("Set ACL Rule(%u): %s\n", 0, air_error_getString(ret));

	//ACL action
	action.port_en = TRUE;
	action.dest_port_sel = TRUE;
	action.portmap = (1 << 5);

	ret = air_acl_setAction(0, DHCP_DPORT67_ACL_RULE_IDX, action);
	if(ret)
		pr_info("Set ACL Action(%u): %s\n", 0, air_error_getString(ret));
	else
		pr_info("Enable to upload DHCP\n");
}

void air_an8855_redirect_dhcp_disabled(void)
{
	AIR_ERROR_NO_T ret = AIR_E_OK;

	ret = air_acl_delRule(0, DHCP_DPORT67_ACL_RULE_IDX);
	if(ret)
		pr_info("Del ACL Rule(%u): %s\n", 0, air_error_getString(ret));

	ret = air_acl_delAction(0, DHCP_DPORT67_ACL_RULE_IDX);
	if(ret)
		pr_info("Del ACL Action(%u): %s\n", 0, air_error_getString(ret));
	else
		pr_info("Disable to upload DHCP\n");
}

void air_an8855_redirect_dns(void)
{
    int port;
    AIR_ACL_RULE_T rule;
    AIR_ACL_ACTION_T action;
    AIR_ERROR_NO_T ret = AIR_E_OK;

    memset(&rule, 0, sizeof(rule));
    memset(&action, 0, sizeof(action));

    //ACL enable
    air_acl_setGlobalState(0, TRUE);
    for (port = 0; port < 4; port++)
    {
        air_acl_setPortEnable(0, port, TRUE);
    }

    //ACL rule（UDP 53）
    rule.key.protocol = 17; // UDP
    rule.key.dport = 53;    // DNS port
    rule.key.fieldmap = (1 << AIR_ACL_PROTOCOL) | (1 << AIR_ACL_DPORT);
    rule.key.portmap = 0xf; // Match all ports
    rule.mask.protocol = 0xff;
    rule.mask.dport = 0xffff;
    rule.mask.fieldmap = rule.key.fieldmap;
    rule.mask.portmap = (~rule.key.portmap) & AIR_ALL_PORT_BITMAP;
    rule.ctrl.rule_en = TRUE;
    rule.ctrl.end = TRUE;

    ret = air_acl_setRule(0, DNS_DPORT53_ACL_RULE_IDX, rule);
    if(ret)
        pr_info("Set DNS ACL Rule(%u): %s\n", 0, air_error_getString(ret));

    // Redirect to a cpu port (such as port 5)
    action.port_en = TRUE;
    action.dest_port_sel = TRUE;
    action.portmap = (1 << 5);

    ret = air_acl_setAction(0, DNS_DPORT53_ACL_RULE_IDX, action);
    if(ret)
        pr_info("Set DNS ACL Action(%u): %s\n", 0, air_error_getString(ret));
	else
		pr_info("Enable to upload DNS\n");
}

void air_an8855_redirect_dns_disabled(void)
{
    AIR_ERROR_NO_T ret = AIR_E_OK;

    ret = air_acl_delRule(0, DNS_DPORT53_ACL_RULE_IDX);
    if(ret)
        pr_info("Del DNS ACL Rule(%u): %s\n", 0, air_error_getString(ret));

    ret = air_acl_delAction(0, DNS_DPORT53_ACL_RULE_IDX);
    if(ret)
        pr_info("Del DNS ACL Action(%u): %s\n", 0, air_error_getString(ret));
	else
		pr_info("Disable to upload DNS\n");
}
#endif

extern int an8855_init(void);
void air_an8855_init(struct sf_eswitch_priv *pesw_priv)
{
	int i;
	u32 pvc_mode = 0x8100 << PVC_STAG_VPID_OFFT;

	AIR_INIT_PARAM_T param = {
		.dev_access = {
			.read_callback = __switch_read,
			.write_callback = __switch_write,
			.phy_read_callback = __switch_phy_read,
			.phy_write_callback = __switch_phy_write,
			.phy_cl45_read_callback = __switch_phy_c45_read,
			.phy_cl45_write_callback = __switch_phy_c45_write
		},
		// .printf = air_printf,
		.udelay = air_udelay,
		.malloc = air_malloc,
		.free = air_free
	};

	air_init(0, &param);
	an8855_init();

	air_port_setRgmiiMode(0, AIR_PORT_SPEED_1000M);
	air_port_air_port_setRgmiiDelay();

	for (i = 0; i < AN8855_NUM_PORTS; i++) {
		an8855_reg_write(PVC(i), pvc_mode);
		air_port_setVlanMode(0, i, AIR_PORT_VLAN_MODE_SECURITY);
		air_vlan_setPortAttr(0, i, AIR_VLAN_PORT_ATTR_USER_PORT);
	}

	memset(&vlan_entries, 0, sizeof(struct vlan_entry));
	INIT_LIST_HEAD(&(vlan_entries.entry_list));
}

void air_an8855_deinit(struct sf_eswitch_priv *pesw_priv) {
	air_an8855_disable_all_phy();
	return;
}

void air_an8855_dump_mac(char *macaddr, int port)
{
	u8 count = 0;
	u32 bucket_size = 0;
	AIR_MAC_ENTRY_T *entry = NULL;
	AIR_ERROR_NO_T ret = AIR_E_OK;

	SF_MDIO_LOCK();
	air_l2_getMacBucketSize(0, &bucket_size);
	SF_MDIO_UNLOCK();

	entry = (AIR_MAC_ENTRY_T *)air_malloc(sizeof(AIR_MAC_ENTRY_T) * bucket_size);
	if (entry) {
		memset(entry, 0, sizeof(AIR_MAC_ENTRY_T) * bucket_size);

		SF_MDIO_LOCK();
		for (ret = air_l2_getMacAddr(0, &count, entry); \
		     ret == AIR_E_OK; \
		     ret = air_l2_getNextMacAddr(0, &count, entry)) {
			if (entry->port_bitmap[0] & BIT(port)) {
				sprintf(macaddr, "%pM", entry->mac);
				break;
			}

			memset(entry, 0, sizeof(AIR_MAC_ENTRY_T) * bucket_size);
		}
		SF_MDIO_UNLOCK();

		air_free(entry);
	}
}

struct sf_eswitch_api_t an8855_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &air_an8855_switch_ops,
#endif
	.vender_init = air_an8855_init,
	.vender_deinit = air_an8855_deinit,
	.led_init = air_an8855_led_init,
	.ifg_init = air_an8855_ifg_init,
	.enable_all_phy = air_an8855_enable_all_phy,
	.disable_all_phy = air_an8855_disable_all_phy,
	.check_phy_linkup = air_an8855_check_phy_link,
	.get_cpu_port_rx_mib = air_an8855_get_cpu_port_rx_mib,
	.set_cpu_port_self_mirror = air_an8855_set_cpu_port_self_mirror,
	.setAsicReg = air_an8855_setAsicReg,
	.getAsicReg = air_an8855_getAsicReg,
	.setAsicPHYReg = an8855_phy_write,
	.getAsicPHYReg = an8855_phy_read,
	.dumpmac = air_an8855_dump_mac,
};
