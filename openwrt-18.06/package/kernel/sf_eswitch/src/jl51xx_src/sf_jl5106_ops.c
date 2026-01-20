#include "../sf_eswitch.h"
#include "jl.h"

#define CPU_RESERVED0_OFFSET                     ((u32)0x00000006U)
#define APB_FRONTEND_BASE                        ((u32)0x00500000U)

struct smi_device_s g_smi_dev;
extern struct vlan_entry vlan_entries;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);
extern int mdio_read_ext(int phyaddr, int phyreg, int *phydata);
extern int mdio_write_ext(int phyaddr, int phyreg, int phydata);

static int lib_jl5106_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int lib_jl5106_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	return 0;
}

static int lib_jl5106_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	//no method to set fid only
	return 0;
}

static int lib_jl5106_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	struct vlan_config vlan_cfg_tmp = {0};
	int vlan_table_index;
	int ret;

	for (vlan_table_index = 1; vlan_table_index <= 5; vlan_table_index++) {
		ret = jl_vlan_get(vlan_table_index, &vlan_cfg_tmp);
		if (val->port_vlan == vlan_cfg_tmp.vid)
			val->value.i = vlan_cfg_tmp.fid;
	}

	return 0;
}

static struct switch_attr lib_jl5106_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = lib_jl5106_get_vlan_enable,
		.set = lib_jl5106_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}
};

static struct switch_attr lib_jl5106_port[] = {

};

static struct switch_attr lib_jl5106_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = lib_jl5106_set_vlan_fid,
		.get = lib_jl5106_get_vlan_fid,
	},
};

static int
lib_jl5106_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
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
	for (i = 0; i < JL_PORT_MAX; i++) {
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
lib_jl5106_set_vlan_ports(struct switch_dev *dev, struct switch_val *val) {
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i, untag = 0, member = 0;

	if (val->port_vlan < 0 || val->len > JL5106_NUM_PORTS)
		return -EINVAL;

	if ((val->port_vlan >= 4000) && (val->port_vlan < 4032))
		return -EINVAL; // reserve such vlan for hnat to wifi

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= JL5106_NUM_PORTS)
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
	//printk("vid:%d, member:%d, untag:%d\n",entry->vid,  entry->member, entry->untag);
	list_add(&(entry->entry_list), &(vlan_entries.entry_list));
	return 0;
}

static int lib_jl5106_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	SF_MDIO_LOCK();
	jl_port_vlan_get(port, val);
	SF_MDIO_UNLOCK();
	return 0;
}

static int lib_jl5106_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	SF_MDIO_LOCK();
	jl_port_vlan_set(port, pvid);
	SF_MDIO_UNLOCK();
	return 0;
}

static int lib_jl5106_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	jl_port_ext_mac_ability_t pability = {};
	jl_port_link_status_t pstatus;
	jl_port_duplex_t pduplex;
	jl_port_speed_t pspeed;

	if (!jl_switch_phyport_check_ext(port)) {
		SF_MDIO_LOCK();
		jl_port_mac_force_link_ext_get(port, &pability);
		SF_MDIO_UNLOCK();

		link->link = pability.link;
		link->duplex = pability.duplex;
		link->speed =  (pability.speed == 1) ?
			SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;
	}
	else if(!jl_switch_phyport_check_utp(port)) {
		SF_MDIO_LOCK();
		jl_port_phy_link_status_get(port, &pstatus, &pduplex, &pspeed);
		SF_MDIO_UNLOCK();

		link->link = pstatus;
		link->duplex = pduplex;
		link->speed =  (pspeed == 1) ?
			SWITCH_PORT_SPEED_100 : SWITCH_PORT_SPEED_10;
	}

	return 0;
}

static int lib_jl5106_apply_vlan_config(struct switch_dev *dev)
{
	struct sf_eswitch_priv *pesw_priv = container_of(dev, struct sf_eswitch_priv, swdev);
	struct vlan_entry *entry = NULL;
	struct vlan_config vlan_cfg = {0};
	jl_uint32 vlan_table_index = 0;
	struct list_head *pos;
	int port_list = 0;
	jl_api_ret_t ret = 0;

	list_for_each(pos, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		printk("vid:%d, member:0x%x, untag:0x%x\n",entry->vid,  entry->member, entry->untag);
		port_list |= entry->member;
		memset(&vlan_cfg, 0, sizeof(vlan_cfg));
		vlan_cfg.valid = 1;
		vlan_cfg.vid = entry->vid;
		vlan_cfg.port_mask = entry->member;
		vlan_cfg.fid = entry->vid;

		SF_MDIO_LOCK();
		ret = jl_vlan_set(vlan_table_index, &vlan_cfg);
		SF_MDIO_UNLOCK();
		if (ret != JL_ERR_OK) {
			printk("jl_vlan_set fail, index %d return  %d!\n", vlan_table_index, ret);
		}
		vlan_table_index++;
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

static int lib_jl5106_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;

	SF_MDIO_LOCK();
	__vlan_table_reset();
	SF_MDIO_UNLOCK();

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;
}

struct switch_dev_ops lib_jl5106_switch_ops = {
	.attr_global = {
		.attr = lib_jl5106_globals,
		.n_attr = ARRAY_SIZE(lib_jl5106_globals),
	},
	.attr_port = {
		.attr = lib_jl5106_port,
		.n_attr = ARRAY_SIZE(lib_jl5106_port),
	},
	.attr_vlan = {
		.attr = lib_jl5106_vlan,
		.n_attr = ARRAY_SIZE(lib_jl5106_vlan),
	},

	.get_vlan_ports = lib_jl5106_get_vlan_ports,
	.set_vlan_ports = lib_jl5106_set_vlan_ports,
	.get_port_pvid = lib_jl5106_get_port_pvid,
	.set_port_pvid = lib_jl5106_set_port_pvid,
	.get_port_link = lib_jl5106_get_port_link,
	.apply_config = lib_jl5106_apply_vlan_config,
	.reset_switch = lib_jl5106_reset_vlan,
};

void lib_jl5106_init(struct sf_eswitch_priv *pesw_priv)
{
	int i;

	jl_switch_init();
	jl_vlan_init();

	for (i = 0; i < JL5106_PHY_PORT_NUM; i++) {
		jl_port_vlan_assignment_set(i, VLAN_ASSIGN_PORT);
		jl_vlan_acpt_frmt_set(i, VLAN_ACCEPT_FRAME_TYPE_UNTAG_ONLY);
		jl_port_vlan_operation_set(i, PORT_DIR_INGRESS, VLAN_OPERATION_PUSH);
		jl_port_vlan_operation_set(i, PORT_DIR_EGRESS, VLAN_OPERATION_REMOVE_ALL);
	}
}

void lib_jl5106_deinit(struct sf_eswitch_priv *pesw_priv)
{
	jl_switch_deinit();
}

void lib_jl5106_led_init(int led_mode)
{
	SF_MDIO_LOCK();
	jl_led_enable_set(LED_GROUP1, ((1 << UTP_PORT0) | (1 << UTP_PORT1) | (1 << UTP_PORT2) \
		| (1 << UTP_PORT3) | (1 << UTP_PORT4) | (1 << UTP_PORT5) | (1 << UTP_PORT6) | (1 << UTP_PORT7)));

	jl_led_blink_rate_set(LED_BLINKRATE_128MS);
	jl_led_group_config_set(LED_GROUP1, led_mode);
	SF_MDIO_UNLOCK();
}

void lib_jl5106_ifg_init(void)
{
	// no method to set ifg
	return;
}

void lib_jl5106_enable_all_phy(struct sf_eswitch_priv *pesw_priv)
{
	int i;
	SF_MDIO_LOCK();
	for (i = 0; i < JL5106_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		jl_port_phy_enable_set(i, true);
	}
	SF_MDIO_UNLOCK();
}

void lib_jl5106_disable_all_phy(void)
{
	int i;
	SF_MDIO_LOCK();
	for (i = 0; i < JL5106_PHY_PORT_NUM; i++) {
		jl_port_phy_enable_set(i, false);
	}
	SF_MDIO_UNLOCK();
}

int lib_jl5106_check_phy_link(int port)
{
	int ret = 0;
	jl_port_speed_t speed = 0;
	jl_port_duplex_t duplex = 0;
	jl_port_link_status_t linkst = 0;

	SF_MDIO_LOCK();
	ret = jl_port_phy_link_status_get(port, &linkst, &duplex, &speed);
	SF_MDIO_UNLOCK();
	if (ret)
		printk("jl_port_phy_link_status_get ret err %d\n", ret);

	return linkst ? 1 : 0;
}

u32 lib_jl5106_get_cpu_port_rx_mib(void)
{
	jl_uint64 counter = 0;

	SF_MDIO_LOCK();
	jl_mib_port_get(EXT_PORT0, PORT_MIB_RX_PKT_UC_CNT, &counter);
	jl_mib_port_clear_all(EXT_PORT0);
	SF_MDIO_UNLOCK();

	return counter;
}

// int lib_jl5106_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
// {
// 	return 0;
// }

int lib_jl5106_setAsicReg(unsigned int reg, unsigned int pValue)
{
	jl_apb_reg_write(reg, pValue);
	return 0;
}

int lib_jl5106_getAsicReg(unsigned int reg, unsigned int *pValue)
{
	jl_apb_reg_read(reg, pValue);
	return 0;
}

void jl_smi_write(u8 phy, u8 reg, u32 val)
{
	mdio_write_ext(phy, reg, val);
}

int jl_smi_read(u8 phy, u8 reg)
{
	int val;
	mdio_read_ext(phy, reg, &val);
	return val;
}

int jl5106_phy_write(u32 port_num, u32 reg, u32 val)
{
	jl_phy_reg_write_ext(port_num, 0, reg, val);
	return 0;
}

int jl5106_phy_read(u32 port_num, u32 reg, u32 *p_val)
{
	*p_val = jl_phy_reg_read_ext(port_num, 0, reg);
	return 0;
}

void lib_jl5106_dump_mac(char macaddr[], int port)
{
	int i;
	jl_l2_fib_key_t key;
	jl_l2_fib_config_t config;

	SF_MDIO_LOCK();
	for (i = 0; i < JL_L2_DA_TABLE_SIZE; i++) {
		jl_l2_fib_config_get_by_index(i, &key, &config);

		if (config.dest.port == port) {
			sprintf(macaddr,"%pM", key.mac_addr.addr);
		}
	}
	SF_MDIO_UNLOCK();
}

int jl_get_chip_id(void)
{
	u32 reg_addr = APB_FRONTEND_BASE + CPU_RESERVED0_OFFSET;
	u32 reg_val[1] = {0};
	u8 *preg_val = (u8 *)&reg_val[0];
	u64 tmp = 0;
	u32 chip_id;
	int ret;

	ret = jl_apb_reg_burst_read(reg_addr, &reg_val[0], 1);
	if (ret)
		return ret;

	jl_read_bits(preg_val, &tmp, 28, 4);
	chip_id = tmp;
	switch (chip_id) {
		case (CHIP_ID_JL5104):
		case (CHIP_ID_JL5105):
		case (CHIP_ID_JL5106):
		case (CHIP_ID_JL5108):
		case (CHIP_ID_JL5109):
		case (CHIP_ID_JL5110):
			break;
		default:
			return -1;
	}

	return chip_id;
}
EXPORT_SYMBOL(jl_get_chip_id);

int jl5106_get_mib_counters_and_print(unsigned int port)
{
	jl_mib_counter_t counters[PORT_MIB_TYPE_END] = {0};
	jl_port_mib_type_t type;
	int ret;

	for (type = 0; type < PORT_MIB_TYPE_END; type++) {
		ret = jl_mib_port_get(port, type, &counters[type]);
		if (ret != 0) {
		printk("Failed to get MIB counter for type %d\n", type);
		return ret;
		}
	}

	printk("===== get switch port:%u counter =====\n", port);
	printk("PORT_MIB_TX_PKT_CNT:      %-20llu PORT_MIB_TX_PKT_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_TX_PKT_CNT], counters[PORT_MIB_TX_PKT_BYTE_CNT]);
	printk("PORT_MIB_TX_PKT_UC_CNT:      %-20llu PORT_MIB_TX_PKT_UC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_TX_PKT_UC_CNT], counters[PORT_MIB_TX_PKT_UC_BYTE_CNT]);
	printk("PORT_MIB_TX_PKT_MC_CNT:      %-20llu PORT_MIB_TX_PKT_MC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_TX_PKT_MC_CNT], counters[PORT_MIB_TX_PKT_MC_BYTE_CNT]);
	printk("PORT_MIB_TX_PKT_BC_CNT:      %-20llu PORT_MIB_TX_PKT_BC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_TX_PKT_BC_CNT], counters[PORT_MIB_TX_PKT_BC_BYTE_CNT]);
	printk("PORT_MIB_TX_PKT_DRP_CNT:    %-20llu PORT_MIB_TX_PKT_ERR_CNT:%-20llu\n",
           counters[PORT_MIB_TX_PKT_DRP_CNT], counters[PORT_MIB_TX_PKT_ERR_CNT]);
	printk("PORT_MIB_RX_PKT_CNT:      %-20llu PORT_MIB_RX_PKT_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_RX_PKT_CNT], counters[PORT_MIB_RX_PKT_BYTE_CNT]);
	printk("PORT_MIB_RX_PKT_UC_CNT:      %-20llu PORT_MIB_RX_PKT_UC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_RX_PKT_UC_CNT], counters[PORT_MIB_RX_PKT_UC_BYTE_CNT]);
	printk("PORT_MIB_RX_PKT_MC_CNT:      %-20llu PORT_MIB_RX_PKT_MC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_RX_PKT_MC_CNT], counters[PORT_MIB_RX_PKT_MC_BYTE_CNT]);
	printk("PORT_MIB_RX_PKT_BC_CNT:      %-20llu PORT_MIB_RX_PKT_BC_BYTE_CNT:%-20llu\n",
           counters[PORT_MIB_RX_PKT_BC_CNT], counters[PORT_MIB_RX_PKT_BC_BYTE_CNT]);
	printk("PORT_MIB_RX_PKT_DRP_CNT:      %-20llu PORT_MIB_RX_PKT_CRC_ERR_CNT:%-20llu\n",
           counters[PORT_MIB_RX_PKT_DRP_CNT], counters[PORT_MIB_RX_PKT_CRC_ERR_CNT]);
	printk("PORT_MIB_RX_FRAG_ERR_CNT:      %-20llu\n",
           counters[PORT_MIB_RX_FRAG_ERR_CNT]);

	return 0;
}
EXPORT_SYMBOL(jl5106_get_mib_counters_and_print);

int jl5106_mib_port_clear_all(unsigned int port)
{
	int ret = 0;

	ret = jl_mib_port_clear_all(port);

	return ret;
}
EXPORT_SYMBOL(jl5106_mib_port_clear_all);

struct sf_eswitch_api_t jl5106_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &lib_jl5106_switch_ops,
#endif
	.vender_init = lib_jl5106_init,
	.vender_deinit = lib_jl5106_deinit,
	.led_init = lib_jl5106_led_init,
	.ifg_init = lib_jl5106_ifg_init,
	.enable_all_phy = lib_jl5106_enable_all_phy,
	.disable_all_phy = lib_jl5106_disable_all_phy,
	.check_phy_linkup = lib_jl5106_check_phy_link,
	.get_cpu_port_rx_mib = lib_jl5106_get_cpu_port_rx_mib,
	//.set_cpu_port_self_mirror = lib_jl5106_set_cpu_port_self_mirror,
	.setAsicReg = lib_jl5106_setAsicReg,
	.getAsicReg = lib_jl5106_getAsicReg,
	.setAsicPHYReg = jl5106_phy_write,
	.getAsicPHYReg = jl5106_phy_read,
	.dumpmac = lib_jl5106_dump_mac,
};
