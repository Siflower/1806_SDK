#include "sf_common.h"
#include "sf_eth.h"
#include "sf_mdio.h"
#ifdef CONFIG_SFAX8_SWITCH_VLAN
#include "sf_switch.h"
#endif
#include "sf_switch_init.h"
#include "sf_switch_hash_table.h"
#include <linux/if_vlan.h>
#include <linux/if_ether.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/switch.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include "sf_debug.h"

extern int g_user_port;
extern unsigned long g_user_speed;
extern struct sf_priv * g_sf_priv;
unsigned char detect_irq_per_link[SF_MAC_PORTS] = {0};
extern unsigned char start_tx_shutdown[SF_MAC_PORTS];
extern volatile int sync_flag;

static struct of_device_id switch_match[] = {
	{ .compatible = "siflower,sfax8-switch" },
	{}
};

#ifdef CONFIG_SFAX8_SWITCH_DPS

int notify_link_event(struct genl_family *family, int port, int updown, int is_wan,int vlanid, char *ifname)
{
	struct sk_buff *skb;
	int ret = 0;
	void *msg_head;

	skb = genlmsg_new(MAX_MSG_SIZE, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	msg_head = genlmsg_put(skb, 0, 0, family, 0, SF_CMD_GENERIC);
	if (!msg_head){
		printk("%s : add genlmsg header error!\n", __func__);
		ret = -ENOMEM;
		goto err;
	}

	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_PORT, port);
	if (ret < 0)
		goto err;

	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_LINK, updown);
	if (ret < 0)
		goto err;

	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_IS_WAN, is_wan);
	if (ret < 0)
		goto err;

	ret = nla_put_u32(skb, SF_ETH_CMD_ATTR_DPS_VLANID, vlanid);
	if (ret < 0)
		goto err;

	ret = nla_put_string(skb, SF_ETH_CMD_ATTR_DPS_IFNAME, ifname);
	if (ret < 0)
		goto err;

	genlmsg_end(skb, msg_head);

	ret = genlmsg_multicast(family, skb, 0, 0, GFP_KERNEL);
	return 0;

err:
	nlmsg_free(skb);
	return ret;
}

/* send netlink message for detect port status change */
void dps_genl_sendmsg(struct work_struct *work){
	struct sf_priv *priv = container_of(work, struct sf_priv, dpswork);
	int is_wan = 0;
	int vlanid = 0;
#ifdef CONFIG_SFAX8_SWITCH_VLAN
	struct sfax8_vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	list_for_each(pos, &(priv->sfax8_switch->vlan_entries.entry_list)) {
		entry = list_entry(pos, struct sfax8_vlan_entry, entry_list);
		if (entry->member & (0x1 << priv->updown_port) && entry->vid == 2) {
			is_wan = 1;
			vlanid = 2;
			break;
		}
	}
#endif

	notify_link_event(&(priv->dps_family.family), priv->updown_port, priv->port_status, is_wan, vlanid, priv->netdev->name);
}
#endif

void sfax8_link_adjust(struct sf_priv *priv, int port, int lock)
{
	int  speed = 0, duplex = 0, i = 0;
	// when receive 0x40 irq link up bit is not set
	for(;i < 3; i ++){
		if(check_phy_linkup(port, priv, lock)){
			speed = get_link_speed(port, priv);
			duplex= get_link_duplex(port, priv);
			print_msg(SWITCH_INIT_DEBUG, NULL,"link speed %d  link duplex %d port %d\n", speed, duplex, port);
			sf_mac_set_link(port,speed, duplex);
			break;
		}
		udelay(1);
	}
}

int sfax8_has_carrier(struct sf_priv *priv, unsigned int port_list, int lock)
{
	int i, ret = 0;
	/*cause there is a condition which receive irq but doesn't check phy link up after reboot, so we add this delay*/
	for (i = 0; i < SF_MAC_PORTS; i++){
		if(port_list & (0x1 << i)){
			if(check_phy_linkup(i, priv, lock)){
				sfax8_link_adjust(priv, i, lock);
				print_msg(SWITCH_INIT_DEBUG,NULL,"check phy link up, port number is %d\n",i);
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

static void sfax8_handle_carrier(struct sf_priv *priv, int status_reg, int port, int lock)
{
	if (netif_carrier_ok(priv->netdev) && status_reg & (0x1 << 4)){
		if (!sfax8_has_carrier(priv, SF_MAC_PORTS_LIST & ~(0x1 << port), lock)){
			netif_carrier_off(priv->netdev);
			//print_msg(SWITCH_INIT_DEBUG,NULL,"carrier off\n");
		}
	}
	else if(!netif_carrier_ok(priv->netdev) && status_reg & (0x1 << 6)){
			netif_carrier_on(priv->netdev);
			//print_msg(SWITCH_INIT_DEBUG,NULL,"carrier on\n");
	}
}

void switch_irq_task(unsigned long data)
{
	struct sf_priv *priv = g_sf_priv;
	int status_reg = 0;
#ifdef SF_TX_SHUTDOWN
    int addr2 = 0, value2 = 0;
#endif
    int port = (int)data;

    sync_flag = 1;
    status_reg = read_phy_reg(port, OMINI_PHY_INTSRC_REGISTER, priv->phy_dev, 0);
    print_msg(SWITCH_INIT_DEBUG,NULL,"===irq port %d,status 0x%x \n", port , status_reg);
    /*enable Auto-negotiation and AutoMDIX to link for disable them while enable energy detect accroding to ominiphy*/

#ifdef SF_TX_SHUTDOWN
    if(status_reg & (0x1 << 7)) {
		if((1 << port) & priv->shutdown_list){
			// sf_phy_init_normal(priv, port);
			if (!detect_irq_per_link[port] ) {
				sf_phy_reset(port, priv);
				print_msg(ETH_RESET_DEBUG,NULL,"reset here \n");
				if (port == g_user_port && g_user_speed != 0){
					// set user advertise speed
					write_phy_reg(port, PHY_AUTO_NEGOTIATION_ADEWS, g_user_speed, priv->phy_dev, 0);
				}
			}
			// add here for when siflower connect self, will cause ping pong reset issue
			// after first triger, every 3 irq will trigger reset
			detect_irq_per_link[port] = (detect_irq_per_link[port]  + 1) % 3;
			// phy soft reset will not reset value for  this bit 7 which is auto mdix -- verified from IC
			write_phy_reg(port, OMINI_PHY_MODEL_CTRL_REGISTER, 0x80, priv->phy_dev,0);
		}
	}

	if(status_reg & (0x1 << 2) || status_reg & (0x1 << 5)) {
		if((1 << port) & priv->shutdown_list){
			detect_irq_per_link[port] = 0;
			// sf_phy_init_low_power(priv, port);
			//trigger re auto
			sf_phy_re_auto(priv, port, 0);
		}
	}
#endif

	if(status_reg & (0x1 << 4 | 0x1 << 6)) {

#ifdef SF_TX_SHUTDOWN
		if((1 << port) & priv->shutdown_list)
		  detect_irq_per_link[port] = 0;
#endif
		if(status_reg & (0x1 << 6)){
#ifdef SF_TX_SHUTDOWN
			if((1 << port) & priv->shutdown_list){
				addr2 = get_port_address(port,2);
				value2 = sf_readl(addr2);
				value2 &= ~(0x200);
				sf_writel(value2, addr2);
			}
#endif
			sfax8_link_adjust(priv, port, 0);
			// sf_mac_enable(0x1 << port);
			sfax8_handle_carrier(priv, status_reg, port, 0);
		}
#ifndef CONFIG_SFAX8_SWITCH_POWERSAVE
		else {
			sfax8_handle_carrier(priv, status_reg, port, 0);
#ifdef SF_TX_SHUTDOWN
			if((1 << port) & priv->shutdown_list){
				addr2 = get_port_address(port,2);
				value2 = sf_readl(addr2);
				value2 |= 0x200;
				sf_writel(value2, addr2);
				// sf_mac_disable_rx(0x1 << port);
				// sf_mac_disable_tx(0x1 << port);
				start_tx_shutdown[port] = 0;
				write_phy_reg(port, PHY_BASE_CONTROL_REGISTER, 0x0, priv->phy_dev, 0);
				// sf_phy_init_low_power(priv, port);
			}
#endif
		}
#endif


#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
		if(port != 4){
			spin_lock(&priv->switch_lock);
			priv->count++;
			spin_unlock(&priv->switch_lock);
			wake_up_process(priv->sfax8_switch->switch_kthread);
		}
#endif
#ifdef CONFIG_SFAX8_SWITCH_DPS
		if(status_reg & (0x1 << 6))
		  priv->port_status = 1;
		else
		  priv->port_status = 0;
		priv->updown_port = port;
		schedule_work(&priv->dpswork);
#endif
	}

	//enable_irq(SIFLOWER_NPU_PHY_IRQ_BASE + port);
	write_phy_reg(port, OMINI_PHY_INTMASK_REGISTER, PHY_INT_ENABLE_ALL_NORMAL, g_sf_priv->phy_dev, 0);
        sync_flag = 0;
	//TODO: for write irq on fail
	// phy_value = read_phy_reg(port, OMINI_PHY_INTMASK_REGISTER, priv->phy_dev);
}

static irqreturn_t sfax8_switch_irq_handle(int irq, void *_priv)
{
	struct sf_priv *priv = (struct sf_priv *) _priv;
    int port = -1;
    int i;
	int has_locked = 0;

	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
#ifdef CONFIG_SFAX8_SWITCH_VLAN
        if(priv->sfax8_switch->irq_base[i] == irq){
#else
        if(priv->phy_irq_base[i] == irq){
#endif
            port = i;
            break;
        }
    }
    //printk("sfax8_switch_irq_handle--irq=%d port=%d\n",irq, port);
	/* Interrupt is not ours */
	if (port < 0 || port > 4)
	  return IRQ_NONE;

	//disable_irq_nosync(irq);
        has_locked = sf_try_lock_phy(port);
	write_phy_reg(port, OMINI_PHY_INTMASK_REGISTER, 0, priv->phy_dev, 0);
        if (has_locked)
                sf_try_unlock_phy(port);
	tasklet_schedule(&priv->task[port]);

	return IRQ_HANDLED;
}

void sfax8_switch_disable_int(struct sf_priv *priv){
	int i = 0;
	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, 0, priv->phy_dev, 1);
	}
	return;
}

void sfax8_set_mac(unsigned char *mac) {
	int  i = 0, tmp = 0;

	for (i = 0; i < 5; i++){
		sf_writel(htonl(*(uint32_t *)mac), get_mac_address_addr(i, 0));
		tmp = sf_readl(get_mac_address_addr(i, 1));
		tmp = (tmp & 0xffff0000) | htons(*(uint32_t *)&mac[4]);
		sf_writel(tmp, get_mac_address_addr(i, 1));
	}
	return;
}

#ifdef CONFIG_SFAX8_SWITCH_VLAN
static int
sfax8_get_vlan_info(struct switch_dev *dev, const struct switch_attr *attr,
			struct switch_val *val)
{
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);
	struct sfax8_vlan_entry *entry = NULL;
	struct list_head *pos;
	char *buf = sfax8_switch->buf;
	int member = 0, len = 0, i = 0;

	list_for_each(pos, &(sfax8_switch->vlan_entries.entry_list)) {
		entry = list_entry(pos, struct sfax8_vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			member = entry->member;
			break;
		}
	}

	len += snprintf(buf + len, sizeof(sfax8_switch->buf) - len, "VLAN %d: Ports: ", val->port_vlan);

	for (i = 0; i < SF_MAX_PORTS; i++) {
		if (!(member & BIT(i)))
		  continue;

		len += snprintf(buf + len, sizeof(sfax8_switch->buf) - len, "%d ", i);
	}

	val->len = len;
	val->value.s = buf;

	return 0;
}

static const struct switch_attr sfax8_vlan[] = {
	{
		.type = SWITCH_TYPE_STRING,
		.name = "info",
		.description = "Get vlan information",
		.max = 1,
		.set = NULL,
		.get = sfax8_get_vlan_info,
	},
};

static int sfax8_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	u32 member = 0;
	int i;
	val->len = 0;

	if (val->port_vlan < 0)
	  return -EINVAL;

	member = vlan_hash_table_search(val->port_vlan);
	if(member == -1)
		return -EINVAL;
	else
		member = member & 0xFF;

	for (i = 0; i < SF_MAX_PORTS; i++) {
		struct switch_port *p;

		if (!(member & BIT(i)))
		  continue;

		p = &val->value.ports[val->len++];
		p->id = i;
	}
	return 0;
}

static int sfax8_set_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	struct sfax8_switch_t *sfax8_switch= container_of(dev, struct sfax8_switch_t, swdev);
	struct sfax8_vlan_entry *entry = NULL;
	struct list_head *pos;
	int member = 0;
	int i;

	if (val->port_vlan < 0 || val->len > SF_MAX_PORTS)
	  return -EINVAL;

	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];

		if (p->id >= SF_MAX_PORTS)
			return -EINVAL;

		member |= BIT(p->id);
	}

	list_for_each(pos, &(sfax8_switch->vlan_entries.entry_list)) {
		entry = list_entry(pos, struct sfax8_vlan_entry, entry_list);
		if (entry->vid == val->port_vlan) {
			entry->member = member;
			print_msg(SWITCH_INIT_DEBUG,NULL,"set old port_vlan:%d vlan ports:0x%x\n", val->port_vlan, member);
			return 0;
		}
	}

	print_msg(SWITCH_INIT_DEBUG,NULL,"set new port_vlan:%d vlan ports:0x%x\n", val->port_vlan, member);
	entry = (struct sfax8_vlan_entry *)kzalloc(sizeof(struct sfax8_vlan_entry), GFP_KERNEL);
	entry->vid = val->port_vlan;
	entry->member = member;
	list_add(&(entry->entry_list), &(sfax8_switch->vlan_entries.entry_list));

	return 0;
}

static int sfax8_get_port_pvid(struct switch_dev *dev, int port, int *val) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);

	if (port >= SF_MAX_PORTS)
	  return -EINVAL;

	*val = sfax8_switch->port[port].fallback_bd_id;
	*val &= 0xfff;

	return 0;
}

static int sfax8_set_port_pvid(struct switch_dev *dev, int port, int pvid) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);

	if (port >= SF_MAX_PORTS)
	  return -EINVAL;

	if (pvid < 0 || pvid > 4095)
	  return -EINVAL;

	sfax8_switch->port[port].fallback_bd_id = pvid;

	printk("set port pvid 0x%x port %d \n",pvid ,port);
	return 0;
}

static int sfax8_get_port_link(struct switch_dev *dev,  int port, struct switch_port_link *link) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);
	uint speed;
	struct sf_priv *priv = (struct sf_priv *)netdev_priv(sfax8_switch->netdev);

	if (port < 0 || port >= SF_MAX_PORTS)
	  return -EINVAL;

	link->link = check_phy_linkup(port, priv, 1);
	if(link->link){
		link->duplex = get_link_duplex(port, priv);
		speed = get_link_speed(port, priv);

		switch (speed) {
			case 0:
				link->speed = SWITCH_PORT_SPEED_10;
				break;
			case 1:
				link->speed = SWITCH_PORT_SPEED_100;
				break;
			case 2:
				link->speed = SWITCH_PORT_SPEED_1000;
				break;
			default:
				link->speed = SWITCH_PORT_SPEED_UNKNOWN;
				break;
		}
	}
	return 0;
}

static int sfax8_apply_config(struct switch_dev *dev) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);
	struct sf_priv *priv = (struct sf_priv *)netdev_priv(sfax8_switch->netdev);
	struct sfax8_vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	int i, addr1 = 0, addr2 = 0, value = 0, member = 0;
	int action_entry = 0;
	u16 vid;

	if(vlan_hash_table_flush() == -1) {
		printk(" flush vlan table fail\n");
		return -1;
	}
	// vlan 0 use fallback bd entry not need to add
	list_for_each(pos, &(sfax8_switch->vlan_entries.entry_list)) {
		entry = list_entry(pos, struct sfax8_vlan_entry, entry_list);
		vid = entry->vid;
		member = entry->member;
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
		int j = 0;
		u8 member_check = 0;
		/*If phy link up,insert port to vlan table*/
		if(member == 0x2f){
			for (j = 0; j < SF_POWERSAVE_PORTS; j++){
				if(check_phy_linkup(j, priv, 1))
				  member_check |= 1 << j;
			}
			member = member_check | 1 << SF_HOST_PORT;
		}
#endif

		action_entry =  ((member << BRENTRY_FWD_PORT_LIST_START_POS) & BRENTRY_FWD_PORT_LIST_MASK) | ((member << BRENTRY_UNTAG_LIST_START_POS) & BRENTRY_UNTAG_LIST_MASK);
		print_msg(SWITCH_INIT_DEBUG,NULL,"add vlan entry member 0x%x, vid %d\n",member,vid);
		vlan_hash_table_insert(vid, 0, action_entry);
		// sf_switch_init_broadcast_mac(vid, member);
	}

	if (list_empty(&(sfax8_switch->vlan_entries.entry_list))) {
		// if list empty, means valn disable, should set host port ANY_TAGGING and all port to default vlan entry
		addr2 = get_port_address(SF_HOST_PORT, 2);
		value = sf_readl(addr2) & (~PORT_AFT_MASK);
		sf_writel(value |((ANY_TAGGING << PORT_AFT_START_POS) & PORT_AFT_MASK), addr2);
		action_entry =  ((priv->port_list << BRENTRY_FWD_PORT_LIST_START_POS) & BRENTRY_FWD_PORT_LIST_MASK) | ((priv->port_list << BRENTRY_UNTAG_LIST_START_POS) & BRENTRY_UNTAG_LIST_MASK);
		print_msg(SWITCH_INIT_DEBUG,NULL,"add default vlan entry member 0x%x, vid %d\n",priv->port_list,DEF_PORT_FALLBACK_BD_ID);
		vlan_hash_table_insert(DEF_PORT_FALLBACK_BD_ID, 0, action_entry);
		for (i = 0; i < SF_MAX_PORTS; i++)
			priv->sfax8_switch->port[i].fallback_bd_id = DEF_PORT_FALLBACK_BD_ID;
	}else {
		addr2 = get_port_address(SF_HOST_PORT, 2);
		value = sf_readl(addr2) & (~PORT_AFT_MASK);
		sf_writel(value |((TAGGED_ONLY << PORT_AFT_START_POS) & PORT_AFT_MASK), addr2);
	}

	for (i = 0; i < sfax8_switch->max_port_num; i++) {
		addr1 = get_port_address(i,1);
		/* class hw port csr reg:
		 *======================================================*
		 *res |block_state |aft |shutdown |fallback_bd_id |tpid *
		 *======================================================*
		 *20-bit| 4-bit   |4-bit| 1-bit   | 16-bit      |16-bit *
		 *=====================================================*/
		member = sf_readl(addr1);
		member &= ~PORT_FALLBACK_BDID_MASK;
		member |= (sfax8_switch->port[i].fallback_bd_id << PORT_FALLBACK_BDID_START_POS) & PORT_FALLBACK_BDID_MASK;
		print_msg(SWITCH_INIT_DEBUG,NULL,"set fallback bdid 0x%x  port %d\n",sfax8_switch->port[i].fallback_bd_id, i);
		sf_writel(member, addr1);
	}
	//sync port_list to global fall bd entry, bit 0 --l2 special punt enable for all ports bit 31:1 - global fall back bd entry, 16 bit default forward list for bd entry,punt to all ports if miss for unicast/multicast
	sf_writel((priv->port_list|1<<BRENTRY_UCAST_MISS_ACT_START_POS|1<<BRENTRY_MCAST_MISS_ACT_START_POS)<<1, REG_CLASS_GLOBAL_CFG);
	/* disable unused ports */
	writeb(priv->port_list & SF_MAC_PORTS_LIST, (void*)(OMINI_PHY_ENABLE));

	return 0;
}

int sfax8_reset_switch(struct switch_dev *dev) {
	struct sfax8_switch_t *sfax8_switch = container_of(dev, struct sfax8_switch_t, swdev);
	struct sfax8_vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;
	int i;

	printk("Enter:%s\n", __func__);
	memset(&(sfax8_switch->vlan_entries), 0, sizeof(struct sfax8_vlan_entry));

	/* set default vid of each vlan to the same number of vlan, so the vid
	 * won't need be set explicitly.
	 */
	if (!sfax8_switch->vlan_entries.entry_list.prev)
		INIT_LIST_HEAD(&(sfax8_switch->vlan_entries.entry_list));

	list_for_each_safe(pos, tmp, &(sfax8_switch->vlan_entries.entry_list)) {
		entry = list_entry(pos, struct sfax8_vlan_entry, entry_list);
		printk("free vlan list:%d member:0x%x\n", entry->vid, entry->member);
		list_del(pos);
		kfree(entry);
	}

	/* init port data structure */
	for (i = 0; i < SF_MAX_PORTS; i++) {
		sfax8_switch->port[i].fallback_bd_id = DEF_PORT_FALLBACK_BD_ID;
	}

	return 0;
}

enum {
	/* Global attributes. */
	SF_ATTR_ENABLE_VLAN,
};
static int
sf_get_vlan_enable(struct switch_dev *dev,
			const struct switch_attr *attr,
			struct switch_val *val)
{
	return 0;
}

static int
sf_set_vlan_enable(struct switch_dev *dev,
			const struct switch_attr *attr,
			struct switch_val *val)
{
	return 0;
}
static const struct switch_attr sf_global[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "VLAN mode (1:enabled)",
		.max = 1,
		.id = SF_ATTR_ENABLE_VLAN,
		.get = sf_get_vlan_enable,
		.set = sf_set_vlan_enable,
	},
};
static const struct switch_attr sf_port[] = {
};
static const struct switch_dev_ops sfax8_switch_ops = {
	.attr_global = {
		.attr = sf_global,
		.n_attr = ARRAY_SIZE(sf_global),
	},
	.attr_port = {
		.attr = sf_port,
		.n_attr = ARRAY_SIZE(sf_port),
	},
	.attr_vlan = {
		.attr = sfax8_vlan,
		.n_attr = ARRAY_SIZE(sfax8_vlan),
	},
	.get_vlan_ports = sfax8_get_vlan_ports,
	.set_vlan_ports = sfax8_set_vlan_ports,
	.get_port_pvid = sfax8_get_port_pvid,
	.set_port_pvid = sfax8_set_port_pvid,
	.get_port_link = sfax8_get_port_link,
	.apply_config = sfax8_apply_config,
	.reset_switch = sfax8_reset_switch,
};

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
static int sfax8_switch_thread(void *data) {
	struct sf_priv *priv = data;

	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		spin_lock_bh(&priv->switch_lock);
		if (!priv->count){
			spin_unlock_bh(&priv->switch_lock);
			schedule();
		}
		set_current_state(TASK_RUNNING);
		vlan_hash_table_refresh(&priv->sfax8_switch->swdev);

		spin_lock_bh(&priv->switch_lock);
		priv->count = 0;
		spin_unlock_bh(&priv->switch_lock);
	}
	return 0;
}
#endif

int sfax8_switch_open(struct sf_priv *priv){
	int ret = -1, i = 0;

	priv->sfax8_switch->max_port_num     = SF_MAX_PORTS;
	priv->sfax8_switch->host_port     = SF_HOST_PORT;
	priv->sfax8_switch->fwd_port_list = FWD_PORT_LIST_MASK;
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	priv->sfax8_switch->switch_kthread = NULL;
#endif

	spin_lock_init(&priv->switch_lock);
	sf_port_init((void*)priv->sfax8_switch);

	if (!priv->sfax8_switch->vlan_entries.entry_list.prev)
		INIT_LIST_HEAD(&(priv->sfax8_switch->vlan_entries.entry_list));
	sfax8_apply_config(&priv->sfax8_switch->swdev);

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	priv->sfax8_switch->switch_kthread = kthread_create(sfax8_switch_thread, priv, "%s", (&priv->sfax8_switch->swdev)->name);
	if (IS_ERR(priv->sfax8_switch->switch_kthread)){
		netdev_err(priv->netdev,"Could not allocate kthread for swicth\n");
		return PTR_ERR(priv->sfax8_switch->switch_kthread);
	}
#endif

	// init switch irq task
	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		tasklet_init(&priv->task[i], switch_irq_task, (unsigned long)i);
		ret = devm_request_irq(priv->device, priv->sfax8_switch->irq_base[i], sfax8_switch_irq_handle, 0, dev_name(priv->device), priv);
		if (ret){
		  dev_err(priv->device, "devm_request_irq-----irq=%d ret=%d\n",priv->sfax8_switch->irq_base[i] ,ret);
		  goto err_irq;
		}
	}

	// if set tx shutdown, link must fail when irq is not handled
	if (sfax8_has_carrier(priv, SF_MAC_PORTS_LIST, 1)){
		netif_carrier_on(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier on\n");
	}
	else{
		netif_carrier_off(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier off\n");
	}

	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, 0, priv->phy_dev, 1);
		read_phy_reg(i, OMINI_PHY_INTSRC_REGISTER, priv->phy_dev, 1);
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, PHY_INT_ENABLE_ALL_NORMAL, priv->phy_dev, 1);
	}

	dev_info(priv->device, "loaded %s driver \n", (&priv->sfax8_switch->swdev)->name);
	return 0;

err_irq:
#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	kthread_stop(priv->sfax8_switch->switch_kthread);
	priv->sfax8_switch->switch_kthread = NULL;
#endif

	for (; i > 0; i--){
		tasklet_kill(&priv->task[i - 1]);
#ifdef CONFIG_SMP
		irq_set_affinity_hint(priv->sfax8_switch->irq_base[i - 1], NULL);
#endif
		devm_free_irq(priv->device, priv->sfax8_switch->irq_base[i - 1], priv);
	}

	return ret;
}


int sfax8_switch_stop(struct sf_priv *priv){
	int i;
	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		tasklet_kill(&priv->task[i]);
#ifdef CONFIG_SMP
		irq_set_affinity_hint(priv->sfax8_switch->irq_base[i], NULL);
#endif
		devm_free_irq(priv->device, priv->sfax8_switch->irq_base[i], priv);
	}
	priv->task[0].func = 0;

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
	kthread_stop(priv->sfax8_switch->switch_kthread);
	priv->sfax8_switch->switch_kthread = NULL;
#endif

	return 0;
}

int sfax8_switch_probe(struct sf_priv *priv){
	struct sfax8_switch_t *sfax8_switch;
	struct switch_dev	*swdev;
	struct device_node *np = NULL;
	const char *map;
	int i, ret = -1;

	np = of_find_matching_node(NULL, switch_match);
	if (!np) {
		dev_err(priv->device, "no gsw node found\n");
		return -EINVAL;
	}
	np = of_node_get(np);

	sfax8_switch = devm_kzalloc(priv->device, sizeof(struct sfax8_switch_t), GFP_KERNEL);
	if (!sfax8_switch) {
		dev_err(priv->device, "no gsw memory for private data\n");
		return -ENOMEM;
	}

	sfax8_switch->base = of_iomap(np,0);
	if (!sfax8_switch->base) {
		dev_err(priv->device, "switch ioremap failed\n");
		ret = -ENOMEM;
		goto no_base;
	}

	for(i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
        sfax8_switch->irq_base[i] = irq_of_parse_and_map(np, i);
        if (!sfax8_switch->irq_base[i]){
            dev_err(priv->device, "switch get irq[%d] failed\n",i);
            ret = -EINVAL;
            goto no_irq;
        }
    }

	if (of_property_read_string(np, "sfax8,port-map", &map)) {
		dev_err(priv->device, "switch get map failed\n");
		ret = -EINVAL;
		goto no_map;
	}

	for(i = 0; i < SF_MAC_PORTS; i++) {
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		if(map[i] != '*')
			priv->port_list += 1<<i;
	}

	priv->port_list |= 1<<SF_HOST_PORT;


#ifdef CONFIG_SFAX8_SWITCH_DPS
	INIT_WORK(&priv->dpswork, dps_genl_sendmsg);
#endif
	sfax8_switch->netdev = priv->netdev;
	priv->sfax8_switch = sfax8_switch;

	swdev = &sfax8_switch->swdev;
	swdev->alias = "sfax8_sw";
	swdev->name = "sfax8_switch";
	swdev->cpu_port = SF_HOST_PORT;
	swdev->ports = SF_MAX_PORTS;
	// max hash table entries is 128
	swdev->vlans = 4096;
	swdev->ops = &sfax8_switch_ops;

	ret = register_switch(swdev, NULL);
	if (ret) {
		dev_err(priv->device, "failed to register sfax8\n");
		goto no_register_switch;
	}

	return 0;

no_register_switch:
    iounmap(priv->sfax8_switch->base);
no_map:
no_irq:
no_base:
    devm_kfree(priv->device, sfax8_switch);
	return ret;
}

int sfax8_switch_remove(struct sf_priv *priv){
	int i;

	if (priv->task[0].func) {
		for (i = 0; i < SF_MAC_PORTS; i++){
			if (!(SF_MAC_PORTS_LIST & (1 << i)))
				continue;
			tasklet_kill(&priv->task[i]);
			devm_free_irq(priv->device, priv->sfax8_switch->irq_base[i], priv);
		}

#ifdef CONFIG_SFAX8_SWITCH_POWERSAVE
		kthread_stop(priv->sfax8_switch->switch_kthread);
		priv->sfax8_switch->switch_kthread = NULL;
#endif
	}

#ifdef CONFIG_SFAX8_SWITCH_DPS
	cancel_work_sync(&priv->dpswork);
#endif
	unregister_switch(&priv->sfax8_switch->swdev);

	if(priv->sfax8_switch->base)
	  iounmap(priv->sfax8_switch->base);

	devm_kfree(priv->device, priv->sfax8_switch);
	priv->sfax8_switch = NULL;
	print_msg(ETH_INIT_DEBUG,priv->netdev, "clean up switch module");

	return 0;
}

#else

int sfax8_switch_probe(struct sf_priv *priv)
{
	struct device_node *np = NULL;
	const char *map;
	int i = 0;

	np = of_find_matching_node(NULL, switch_match);
	if (!np) {
		dev_err(priv->device, "no gsw node found\n");
		return -EINVAL;
	}
	np = of_node_get(np);

	for(i = 0; i < SF_MAC_PORTS; i++)
    {
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
	    priv->phy_irq_base[i] = irq_of_parse_and_map(np, i);
    }
	if (of_property_read_string(np, "sfax8,port-map", &map))
		return -EINVAL;

	for(i = 0; i < SF_MAC_PORTS; i++) {
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		if(map[i] != '*')
			priv->port_list += 1<<i;
	}

	priv->port_list |= 1<<SF_HOST_PORT;

#ifdef CONFIG_SFAX8_SWITCH_DPS
	INIT_WORK(&priv->dpswork, dps_genl_sendmsg);
#endif

	return 0;
}

int sfax8_switch_remove(struct sf_priv *priv){
#ifdef CONFIG_SFAX8_SWITCH_DPS
	cancel_work_sync(&priv->dpswork);
#endif
	return 0;
}

int sfax8_switch_open(struct sf_priv *priv){
	int ret = -1, i = 0, flag = 0;
#ifdef CONFIG_SMP
	struct cpumask irq_affi;
	unsigned int cpu = 0;
#endif

	sf_port_init((void*) priv->port);
	add_vlan_entry_args(DEF_PORT_FALLBACK_BD_ID, 0, priv->port_list, 0);
	//sync port_list to global fall bd entry
	sf_writel((priv->port_list|1<<BRENTRY_UCAST_MISS_ACT_START_POS|1<<BRENTRY_MCAST_MISS_ACT_START_POS)<<1, REG_CLASS_GLOBAL_CFG);

	/* disable unused ports */
	writeb((priv->port_list & SF_MAC_PORTS_LIST), (void*)(OMINI_PHY_ENABLE));

#ifndef CONFIG_SFAX8_SWITCH_FPGA
	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		/*read for clean up all irq*/
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, 0, priv->phy_dev, 1);
		read_phy_reg(i, OMINI_PHY_INTSRC_REGISTER, priv->phy_dev, 1);
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, PHY_INT_ENABLE_ALL_NORMAL, priv->phy_dev, 1);
		tasklet_init(&priv->task[i], switch_irq_task, (unsigned long)i);
		ret = devm_request_irq(priv->device, priv->phy_irq_base[i], sfax8_switch_irq_handle, 0, dev_name(priv->device), priv);
		if (ret)
		  goto err_irq;

#ifdef CONFIG_SMP
		if (of_property_read_u32(priv->device->of_node, "smp-affinity", &cpu) == 0) {
			if (cpu < NR_CPUS) {
				memset(&irq_affi, 0, sizeof(struct cpumask));
				cpumask_set_cpu(cpu, &irq_affi);
				if (irq_set_affinity_hint(priv->phy_irq_base[i], &irq_affi))
					netdev_err(priv->netdev, "can not set the affinity for irq : %d\n", priv->phy_irq_base[i]);
			}
		}
#endif
	}
#endif
	/*while systerm upgrade, cpu loading too much, phy already link up, but we clean up all phy irq, so we may lose phy irq. then we will not check phy link up and carrier on.*/
	/*so that we should check phy linkup betwen clean up all irq and request irq*/

	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		if(check_phy_linkup(i, priv, 1)){
			sfax8_link_adjust(priv, i, 1);
			flag = 1;
			print_msg(SWITCH_INIT_DEBUG,NULL,"check phy link up, port number is %d\n",i);
		}
	}
	if (flag){
		netif_carrier_on(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier on\n");
	}
	else{
		netif_carrier_off(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier off\n");
	}

	return 0;

#ifndef CONFIG_SFAX8_SWITCH_FPGA
err_irq:
	for (; i > 0; i--){
		tasklet_kill(&priv->task[i - 1]);
#ifdef CONFIG_SMP
		irq_set_affinity_hint(priv->phy_irq_base[i - 1], NULL);
#endif
		devm_free_irq(priv->device, priv->phy_irq_base[i - 1], priv);
	}
#endif

	return ret;
}

int sfax8_switch_stop(struct sf_priv *priv){
#ifndef CONFIG_SFAX8_SWITCH_FPGA
	int i;

	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		tasklet_kill(&priv->task[i]);
#ifdef CONFIG_SMP
		irq_set_affinity_hint(priv->phy_irq_base[i], NULL);
#endif
		devm_free_irq(priv->device, priv->phy_irq_base[i], priv);
	}
#endif

	return 0;
}
#endif


int sfax8_switch_reset(struct sf_priv *priv){
	int i = 0, flag = 0;

#ifdef CONFIG_SFAX8_SWITCH_VLAN
	sf_port_init((void*)priv->sfax8_switch);
	sfax8_apply_config(&priv->sfax8_switch->swdev);
#else
	sf_port_init((void*) priv->port);

	add_vlan_entry_args(DEF_PORT_FALLBACK_BD_ID, 0, priv->port_list, 0);
	//sync port_list to global fall bd entry
	sf_writel((priv->port_list|1<<BRENTRY_UCAST_MISS_ACT_START_POS|1<<BRENTRY_MCAST_MISS_ACT_START_POS)<<1, REG_CLASS_GLOBAL_CFG);

	/* disable unused ports */
	writeb((priv->port_list & SF_MAC_PORTS_LIST), (void*)(OMINI_PHY_ENABLE));
#endif

	// if set tx shutdown, link must fail when irq is not handled
	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		if(check_phy_linkup(i, priv, 1)){
			sfax8_link_adjust(priv, i, 1);
			flag = 1;
			print_msg(SWITCH_INIT_DEBUG,NULL,"check phy link up, port number is %d\n",i);
		}
	}
	if (flag){
		netif_carrier_on(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier on\n");
	}
	else{
		netif_carrier_off(priv->netdev);
		print_msg(SWITCH_INIT_DEBUG,NULL,"carrier off\n");
	}

	for (i = 0; i < SF_MAC_PORTS; i++){
		if (!(SF_MAC_PORTS_LIST & (1 << i)))
			continue;
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, 0, priv->phy_dev, 1);
		read_phy_reg(i, OMINI_PHY_INTSRC_REGISTER, priv->phy_dev, 1);
		write_phy_reg(i, OMINI_PHY_INTMASK_REGISTER, PHY_INT_ENABLE_ALL_NORMAL, priv->phy_dev, 1);
	}

	dev_info(priv->device, "reset switch driver\n");

	return 0;
}

struct sf_switch_data_t sf_switch_data = {
	.set_mac = sfax8_set_mac,
	.switch_open = sfax8_switch_open,
	.switch_stop = sfax8_switch_stop,
	.switch_probe = sfax8_switch_probe,
	.switch_remove = sfax8_switch_remove,
	.switch_reset = sfax8_switch_reset,
	.has_carrier = sfax8_has_carrier,
	.adjust_link = sfax8_link_adjust,
};
