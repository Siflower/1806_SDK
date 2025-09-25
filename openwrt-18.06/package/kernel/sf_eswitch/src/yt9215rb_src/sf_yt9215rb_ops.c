/*
* Description
*
*
* Siflower software
*/
#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/of_mdio.h>
#include <linux/gpio/consumer.h>
#include <linux/switch.h>

#include "cal_bprofile.h"
#include "cal_cmm.h"
#include "hal_mem.h"
#include "yt_init.h"
#include "yt_exit.h"
#include "yt_port.h"
#include "yt_l2.h"
#include "yt_stat.h"
#include "yt_vlan.h"
#include "yt_nic.h"
#include "yt_mirror.h"
#include "yt_port.h"
#include "sw_yt9215.h"
#include "hal_mem.h"
#include "../sf_eswitch.h"

#include "sf_yt9215rb_ops.h"

extern struct mii_bus *gp_mii_bus;
extern struct vlan_entry vlan_entries;
extern yt_swDescp_t yt9215rb_swDescp;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);
extern int notify_link_event(struct sf_eswitch_priv *pesw_priv, int port,
	int updown, char *ifname, uint8_t *mac, uint16_t vlan_id, bool flag);

uint32_t yt9215_port_nums;
uint32_t yt9215_cpu_port;
static uint32_t yt_smi0_cl22_write(uint8_t phyAddr, uint8_t regAddr, uint16_t regValue) {
	return mdiobus_write(gp_mii_bus, phyAddr, regAddr, regValue);
}
static uint32_t yt_smi0_cl22_read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pRegValue) {
	*pRegValue = mdiobus_read(gp_mii_bus, phyAddr, regAddr);
	return 0;
}

int yt9215rb_getAsicReg(u32 reg_addr, u32 *reg_val)
{
	u32 rData;
	u8 regAddr;
	u16 regVal;

	regAddr = ((YT_DEFAULT_ID<<2) | (REG_ADDR_BIT1_ADDR<<1) | (REG_ADDR_BIT0_READ));
	/* Set reg_addr[31:16] */
	regVal = (reg_addr >> 16) & 0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);

	/* Set reg_addr[15:0] */
	regVal = reg_addr & 0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);

	regAddr = ((YT_DEFAULT_ID<<2) | (REG_ADDR_BIT1_DATA<<1) | ((REG_ADDR_BIT0_READ)));
	/* Read Data [31:16] */
	regVal = 0x0;
	yt_smi0_cl22_read(YT9215_PHY_ADDR, regAddr, &regVal);
	rData = (u32)(regVal<<16);

	/* Read Data [15:0] */
	regVal = 0x0;
	yt_smi0_cl22_read(YT9215_PHY_ADDR, regAddr, &regVal);

	rData |= regVal;
	*reg_val = rData;

	return 0;
}

int yt9215rb_setAsicReg(u32 reg_addr, u32 reg_value)
{
	u8 regAddr;
	u16 regVal;

	regAddr = ((YT_DEFAULT_ID<<2) | (REG_ADDR_BIT1_ADDR<<1) | (REG_ADDR_BIT0_WRITE));

	/* Set reg_addr[31:16] */
	regVal = (reg_addr >> 16) & 0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);
	/* Set reg_addr[15:0] */
	regVal = reg_addr & 0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);

	/* Write Data [31:16] out */
	regAddr = ((YT_DEFAULT_ID<<2) | (REG_ADDR_BIT1_DATA<<1) | (REG_ADDR_BIT0_WRITE));
	regVal = (reg_value >> 16) & 0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);

	/* Write Data [15:0] out */
	regVal = reg_value&0xffff;
	yt_smi0_cl22_write(YT9215_PHY_ADDR, regAddr, regVal);

	return 0;
}

int yt9215rb_setAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int pRegData)
{
	return yt_port_phy_reg_set(0, phyNo, phyAddr, (u16)pRegData, PHY_INTERNAL);
}

int yt9215rb_getAsicPHYReg(unsigned int phyNo, unsigned int phyAddr, unsigned int *pRegData)
{
	uint16_t pData;
	yt_port_phy_reg_get(0, phyNo, phyAddr, &pData, PHY_INTERNAL);
	*pRegData = pData;
	return 0;

}

#ifdef CONFIG_SWCONFIG
static int
yt9215rb_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	yt_port_mask_t pMember_portmask = {0};
	yt_port_mask_t pUntag_portmask = {0};
	struct switch_port *port;
	u32 i;

    if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	yt_vlan_port_get(0, val->port_vlan, &pMember_portmask, &pUntag_portmask);
	SF_MDIO_UNLOCK();

	port = &val->value.ports[0];
	val->len = 0;

	for (i = 0; i < yt9215_port_nums; i++) {
		if(!(pMember_portmask.portbits[0] & BIT(i)))
			continue;

		port->id = i;
		port->flags = (pUntag_portmask.portbits[0] & BIT(i)) ? 0 : BIT(SWITCH_PORT_FLAG_TAGGED);
		val->len++;
		port++;
	}

	return 0;
}

static int
yt9215rb_set_vlan_ports(struct switch_dev *dev, struct switch_val *val) {
	yt_port_mask_t pMember_portmask;
	yt_port_mask_t pUntag_portmask;

	struct vlan_entry *entry = NULL;
    struct list_head *pos;
    u32 i;

    memset(&pMember_portmask, 0x0, sizeof(yt_port_mask_t));
    memset(&pUntag_portmask, 0x0, sizeof(yt_port_mask_t));

	if (val->port_vlan < 0 || val->len > yt9215_port_nums)
		return -EINVAL;

    if ((val->port_vlan >= 4000) && (val->port_vlan < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

    for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id > yt9215_port_nums)
			return -EINVAL;

		if (!(p->flags & BIT(SWITCH_PORT_FLAG_TAGGED)))
			pUntag_portmask.portbits[0] |= BIT(p->id);

		pMember_portmask.portbits[0] |= BIT(p->id);
    }

	printk("add vid:%d pMember_portmask:0x%x pUntag_portmask:0x%x\n",
					val->port_vlan, pMember_portmask.portbits[0], pUntag_portmask.portbits[0]);
	SF_MDIO_LOCK();
	yt_vlan_port_set(0, val->port_vlan, pMember_portmask, pUntag_portmask);
	yt_vlan_fid_set(0, val->port_vlan, val->port_vlan);
	SF_MDIO_UNLOCK();

    list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			entry->member = pMember_portmask.portbits[0];
			return 0;
		}
    }

    entry = (struct vlan_entry *)kzalloc(sizeof(struct vlan_entry), GFP_KERNEL);
    entry->vid = val->port_vlan;
    entry->member = pMember_portmask.portbits[0];
    list_add(&(entry->entry_list), &(vlan_entries.entry_list));

	return 0;
}

static int yt9215rb_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	yt_vlan_t pvid;

	SF_MDIO_LOCK();
	yt_vlan_port_igrPvid_get(0, VLAN_TYPE_CVLAN, port, &pvid);
	SF_MDIO_UNLOCK();
	*val = pvid;
	return 0;
}

static int yt9215rb_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	if ((pvid >= 4000) && (pvid < 4016))
		return -EINVAL; // reserve such vlan for hnat to wifi

	//specify the priority to 0 if you don’t turn on the QoS
	printk("set port:%d pvid:%d\n", port, pvid);
	SF_MDIO_LOCK();
	yt_vlan_port_igrPvid_set(0, VLAN_TYPE_CVLAN, port, pvid);
	SF_MDIO_UNLOCK();

	return 0;
}

static int yt9215rb_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	yt_port_linkStatus_all_t status;
	yt_enable_t aneg_enable;
	int ret;
	if (port > yt9215_port_nums - 1)
		return -EINVAL;

	SF_MDIO_LOCK();
	ret = yt_port_link_status_all_get(0, port, &status);
	if (ret)
		goto err_out;

	ret = yt_port_macAutoNeg_enable_get(0, port, &aneg_enable);
	if (ret)
		goto err_out;
	SF_MDIO_UNLOCK();

	link->link = status.link_status == PORT_LINK_UP;
	link->duplex = status.link_duplex == PORT_DUPLEX_FULL;
	link->aneg = aneg_enable == YT_ENABLE;
	link->tx_flow = status.tx_fc_en;
	link->rx_flow = status.rx_fc_en;
	switch (status.link_speed) {
		case PORT_SPEED_10M:
			link->speed = SWITCH_PORT_SPEED_10;
			break;
    	case PORT_SPEED_100M:
			link->speed = SWITCH_PORT_SPEED_100;
			break;
    	case PORT_SPEED_1000M:
			link->speed = SWITCH_PORT_SPEED_1000;
			break;
		default:
			link->speed = SWITCH_PORT_SPEED_UNKNOWN;
			break;
	}

	return 0;

err_out:
	SF_MDIO_UNLOCK();
	return ret;
}

static int yt9215rb_apply_vlan_config(struct switch_dev *dev)
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
		printk("cpu port: %d not correct, port_list:0x%x\n", pesw_priv->swdev.cpu_port, port_list);
		return 0;
    }

    if (port_list != 0)
		pesw_priv->port_list = port_list & ~(1 << pesw_priv->swdev.cpu_port);

	return 0;
}

static int yt9215rb_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;
    int i;
	// clear hardware vlan table
	SF_MDIO_LOCK();
    for (i = 1; i < YT_VLAN_ID_MAX; i++) {
        yt_port_mask_t member_mask;
        yt_port_mask_t untagged_mask;
        member_mask.portbits[0] = 0;
        untagged_mask.portbits[0] = 0;
        yt_vlan_port_set(0, i, member_mask, untagged_mask);
    }
	SF_MDIO_UNLOCK();

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;

}

static int yt9215rb_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int yt9215rb_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int yt9215_get_port_fdb_uc_entry(struct switch_dev *dev, const struct switch_attr *attr, struct switch_val *val)
{
	struct sf_eswitch_priv *priv = container_of(dev, struct sf_eswitch_priv, swdev);
	l2_ucastMacAddr_info_t pUcastMac;
	uint16_t fdb_index = 0, next_fdb_index;
	static char buf[64];
	int len = 0;
	yt_port_t port;
	yt_vlan_t vid;

	if (val->port_vlan >= YT9215S_NUM_PORTS)
		return -EINVAL;

	memset(&pUcastMac, 0, sizeof(pUcastMac));

	while ( fdb_index <= 4096 ) {
		next_fdb_index = 0;
		yt_l2_fdb_uc_withindex_getnext(0, fdb_index, &next_fdb_index, &pUcastMac);
		if (next_fdb_index) {
			fdb_index = next_fdb_index;
			port = pUcastMac.port;
			vid = pUcastMac.vid;
			if (val->port_vlan == port) {
				printk("The mac:%pM vlan_id:%d is at the yt9215s's port:%d\n",
					pUcastMac.macaddr.addr, vid, port);
					notify_link_event(priv, port, priv->phy_status[port], "eth0", pUcastMac.macaddr.addr, vid, false);
			}
		fdb_index++;
		} else {
			break;
		}
	}
	len += snprintf(buf + len, sizeof(buf) - len, "These are the port %d of yt9215s's fdb entry.\n", val->port_vlan);

	val->value.s = buf;
	val->len = len;

 	return 0;
}

static int yt9215_get_info_fdb_uc_entry(struct switch_dev *dev, const struct switch_attr *attr, struct switch_val *val)
{
	l2_ucastMacAddr_info_t pUcastMac;
	static char buf[64];
	int len = 0;
	uint16_t fdb_index = 0, next_fdb_index;
	yt_port_t port;
	yt_vlan_t vid;

	memset(&pUcastMac, 0, sizeof(pUcastMac));

	while ( fdb_index <= 4096 ) {
		next_fdb_index = 0;
		yt_l2_fdb_uc_withindex_getnext(0, fdb_index, &next_fdb_index, &pUcastMac);
		if (next_fdb_index) {
			fdb_index = next_fdb_index;
			port = pUcastMac.port;
			vid = pUcastMac.vid;
			printk("The mac:%pM vlan_id:%d is at the yt9215s's port:%d, the index is %d\n",
					pUcastMac.macaddr.addr, vid, port, next_fdb_index );
		fdb_index++;
		} else {
			break;
		}
	}
	len += snprintf(buf + len, sizeof(buf) - len, "These are the yt9215s's fdb entry.\n");

	val->value.s = buf;
	val->len = len;

 	return 0;
}

static int yt9215rb_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	yt_fid_t pFid;

	if (val->port_vlan < 0)
		return -EINVAL;

	SF_MDIO_LOCK();
	yt_vlan_fid_get(0, val->port_vlan, &pFid);
	SF_MDIO_UNLOCK();
	val->value.i = pFid;

	return 0;
}

static int yt9215rb_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	if (val->port_vlan < 0 || val->len > yt9215_port_nums)
		return -EINVAL;

	printk("vid:%d set fid:%d\n", val->port_vlan, val->value.i);
	SF_MDIO_LOCK();
	yt_vlan_fid_set(0, val->port_vlan, val->value.i);
	SF_MDIO_UNLOCK();
	return 0;
}

static struct switch_attr yt9215rb_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = yt9215rb_get_vlan_enable,
		.set = yt9215rb_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}, {
		.type = SWITCH_TYPE_STRING,
		.name = "fdb_entry",
		.description = "Get the switch's fdb unicast entry",
		.get = yt9215_get_info_fdb_uc_entry,
	}
};

static struct switch_attr yt9215rb_port[] = {
	{
		.type = SWITCH_TYPE_STRING,
		.name = "fdb_entry",
		.description = "Get the switch's each port's fdb unicast entry",
		.get = yt9215_get_port_fdb_uc_entry,
	}
};

static struct switch_attr yt9215rb_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = yt9215rb_set_vlan_fid,
		.get = yt9215rb_get_vlan_fid,
	},
};


struct switch_dev_ops yt9215rb_switch_ops = {
	.attr_global = {
		.attr = yt9215rb_globals,
		.n_attr = ARRAY_SIZE(yt9215rb_globals),
	},
	.attr_port = {
		.attr = yt9215rb_port,
		.n_attr = ARRAY_SIZE(yt9215rb_port),
	},
	.attr_vlan = {
		.attr = yt9215rb_vlan,
		.n_attr = ARRAY_SIZE(yt9215rb_vlan),
	},

	.get_vlan_ports = yt9215rb_get_vlan_ports,
	.set_vlan_ports = yt9215rb_set_vlan_ports,
	.get_port_pvid = yt9215rb_get_port_pvid,
	.set_port_pvid = yt9215rb_set_port_pvid,
	.get_port_link = yt9215rb_get_port_link,
	.apply_config = yt9215rb_apply_vlan_config,
	.reset_switch = yt9215rb_reset_vlan,
};
#endif

int yt9215rb_check_phy_link(int port)
{
	yt_port_linkStatus_all_t LinkStatus;

	SF_MDIO_LOCK();
	yt_port_phy_linkstatus_get(0, port, &LinkStatus);
	SF_MDIO_UNLOCK();
	return LinkStatus.link_status;
}

int yt9215rb_extPort_rgmii_init(int port) {
	yt_port_force_ctrl_t port_ctrl = {
		.speed_dup = PORT_SPEED_DUP_1000FULL,
		.rx_fc_en = true,
		.tx_fc_en = true,
	};

	// set rgmii 8/9 init
	yt_port_extif_mode_set(0, port, YT_EXTIF_MODE_RGMII);
	// force 1G full
	yt_port_mac_force_set(0, port, port_ctrl);

	yt_stat_mib_enable_set(0, YT_ENABLE);

	yt_nic_cpuport_mode_set(0, CPUPORT_MODE_EXTERNAL);
	yt_nic_ext_cpuport_en_set(0, YT_DISABLE);
	yt_nic_ext_cpuport_port_set(0, port);
	yt_nic_ext_cputag_en_set(0, YT_DISABLE);

	printk("End port:%d %s\n", port, __func__);
	return 0;
}

void yt9215rb_enable_all_phy(struct sf_eswitch_priv *pesw_priv) {
	int i;
	SF_MDIO_LOCK();
	for (i = 0; i < YT9215RB_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		yt_port_enable_set(0, i, YT_ENABLE);
	}
	SF_MDIO_UNLOCK();
}

void yt9215rb_disable_all_phy(void) {
	int i;
	SF_MDIO_LOCK();
	for (i = 0; i < YT9215RB_PHY_PORT_NUM; i++) {
		yt_port_enable_set(0, i, YT_DISABLE);
	}
	SF_MDIO_UNLOCK();
}

void yt9215rb_led_init(int led_mode)
{
	//fal_tiger_led_init(0);
}

void yt9215rb_ifg_init(void)
{
	// no method to set ifg
	return;
}

u32 yt9215rb_get_cpu_port_rx_mib(void)
{
	yt_stat_mib_port_cnt_t count;

	SF_MDIO_LOCK();
	yt_stat_mib_port_get(0, yt9215_port_nums - 1, &count);
	yt_stat_mib_clear(0, yt9215_port_nums - 1);
	SF_MDIO_UNLOCK();

	return count.RX_UNICAST;
}

int yt9215rb_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
{
	unsigned int i = 0, test_port = 1, regValue = 0, timer = 0;

	if (enable) {
		for (i = 0; i < YT9215RB_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			if (yt9215rb_check_phy_link(i) == 0)
			{
				test_port = i;
				break;
			}
		}

		if (i == YT9215RB_PHY_PORT_NUM) {
			printk("all port has linked, not do gmac auto calibration\n");
			return -1;
		}

		SF_MDIO_LOCK();
		// set test_port phy loopback and disable phy auto-negotiation
		yt9215rb_getAsicPHYReg(test_port, 0, &regValue);
		regValue &= ~(1 << 12); // disable auto-negotiation to speed up loopback link
		yt9215rb_setAsicPHYReg(test_port, 0, regValue);

		yt9215rb_getAsicPHYReg(test_port, 0, &regValue);
		regValue |= (1 << 14);
		yt9215rb_setAsicPHYReg(test_port, 0, regValue);

		// set other port disable
		for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
			if (i != test_port) {
				yt9215rb_getAsicPHYReg(i, 0, &regValue);
				regValue |= PHY_CTRL_ENABLE_POWER_DOWN;
				yt9215rb_setAsicPHYReg(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();

		// must add delay here, or gmac calibration will fail
		while(timer < 2000) {
			mdelay(100);
			timer += 100;
			if (yt9215rb_check_phy_link(test_port))
				break;
		}
	} else {
		SF_MDIO_LOCK();
		// loopback port will be linked, so use port here
		yt9215rb_getAsicPHYReg(port, 0, &regValue);
		regValue &= ~(1 << 14);
		yt9215rb_setAsicPHYReg(port, 0, regValue);

		yt9215rb_getAsicPHYReg(port, 0, &regValue);
		regValue |= (1 << 12);
		yt9215rb_setAsicPHYReg(port, 0, regValue);
		for (i = 0; i < RTK_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			// enable other port
			if (i != port) {
				yt9215rb_getAsicPHYReg(i, 0, &regValue);
				regValue &= ~PHY_CTRL_ENABLE_POWER_DOWN;
				yt9215rb_setAsicPHYReg(i, 0, regValue);
			}
		}
		SF_MDIO_UNLOCK();

	}
	return test_port;
}

void yt9215rb_port_vlan_init(uint32_t cpu_port)
{
	int i;
	yt_tpid_profiles_t tpids;

	tpids.tpid[0] = 0x8100;
	tpids.tpid[1] = 0x8100;
	tpids.tpid[2] = 0x88a8;
	tpids.tpid[3] = 0x88a8;
	yt_vlan_egrTpid_set(0, tpids);
	yt_vlan_port_igrTpidSel_set(0, VLAN_TYPE_CVLAN, 1, 1);
	yt_vlan_port_igrTpidSel_set(0, VLAN_TYPE_CVLAN, 2, 1);
	yt_vlan_port_egrTagMode_set(0, VLAN_TYPE_CVLAN, cpu_port, VLAN_TAG_MODE_ENTRY_BASED);

	for(i = 0; i < YT9215RB_PHY_PORT_NUM; i++)
	{
		yt_vlan_port_egrTagMode_set(0, VLAN_TYPE_CVLAN, i, VLAN_TAG_MODE_ENTRY_BASED);
		yt_vlan_port_igrFilter_enable_set(0, i, YT_ENABLE);
		yt_vlan_port_egrFilter_enable_set(0, i, YT_ENABLE);
	}
}

void yt9215rb_init(struct sf_eswitch_priv *pesw_priv)
{
	int err;

	yt9215rb_swDescp.sw_access.swreg_acc_method = SWCHIP_ACC_SMI;
	yt9215rb_swDescp.sw_access.controller.smi_controller.smi_read = yt_smi0_cl22_read;
	yt9215rb_swDescp.sw_access.controller.smi_controller.smi_write = yt_smi0_cl22_write;
	yt9215rb_swDescp.sw_access.controller.smi_controller.phyAddr = YT9215_PHY_ADDR;
	yt9215rb_swDescp.sw_access.controller.smi_controller.switchId = 0x0;

	//init port/mac/led
	err = yt_init();
	if (err) {
		printk("init error %d\n", err);
		return;
	}

	yt9215rb_extPort_rgmii_init(yt9215_cpu_port);
	yt9215rb_port_vlan_init(yt9215_cpu_port);

	//yt_port_enable_set(0, 5, YT_ENABLE);
	//yt_stat_mib_enable_set(0, YT_ENABLE);
	//yt9215rb_setAsicReg(MAC8_SPEED_SET, 0x1fa);
	//yt9215rb_setAsicReg(CHIP_INTERFACE_MAC8, 0x841c4100);

	//yt9215rb_enable_all_phy(pesw_priv);

}

void yt9215rb_dumpmac(char macaddr[], int port)
{
	l2_ucastMacAddr_info_t pUcastMac;
	uint16_t pNext_index = 0;
	int address = 0;

	SF_MDIO_LOCK();
	while(1){
		if ((yt_l2_fdb_uc_withindex_getnext(0, pNext_index, &pNext_index, &pUcastMac)) != 0)
			break;

		sprintf(macaddr,"%pM", pUcastMac.macaddr.addr);
		address++;
	}
	SF_MDIO_UNLOCK();
}


void yt9215rb_deinit(struct sf_eswitch_priv *pesw_priv) {
	yt9215rb_disable_all_phy();
	return;
}

struct sf_eswitch_api_t yt9215rb_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &yt9215rb_switch_ops,
#endif
	.vender_init = yt9215rb_init,
	.vender_deinit = yt9215rb_deinit,
	.led_init = yt9215rb_led_init,
	.ifg_init = yt9215rb_ifg_init,
	.enable_all_phy = yt9215rb_enable_all_phy,
	.disable_all_phy = yt9215rb_disable_all_phy,
	.check_phy_linkup = yt9215rb_check_phy_link,
	.get_cpu_port_rx_mib = yt9215rb_get_cpu_port_rx_mib,
	.set_cpu_port_self_mirror = yt9215rb_set_cpu_port_self_mirror,
	.getAsicReg = yt9215rb_getAsicReg,
	.setAsicReg = yt9215rb_setAsicReg,
	.getAsicPHYReg = yt9215rb_getAsicPHYReg,
	.setAsicPHYReg = yt9215rb_setAsicPHYReg,
	.dump_mac = yt9215rb_dumpmac,
};
