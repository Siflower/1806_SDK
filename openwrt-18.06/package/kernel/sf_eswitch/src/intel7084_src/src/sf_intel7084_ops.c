/*
* Description
*
*
* Siflower software
*/

#include "../../sf_eswitch.h"
#include "../../sf_mc_io.h"
#include "sf_intel7084_ops.h"
#include "gsw_sw_init.h"


extern ethsw_api_dev_t *pedev0[GSW_DEV_MAX];
extern struct vlan_entry vlan_entries;
extern int check_port_in_portlist(struct sf_eswitch_priv *pesw_priv, int port);

int intel7084_phy_rd(GSW_MDIO_data_t *parm){

	return GSW_MDIO_DataRead((void*)pedev0[0], parm);
}

int intel7084_phy_wr(GSW_MDIO_data_t *parm){

	return GSW_MDIO_DataWrite((void*)pedev0[0], parm);
}

int intel7084_mdio_rd(u16 Offset, u16 Shift, u16 Size, ur *value){
	return gsw_reg_rd((void*)pedev0[0], Offset, Shift, Size, value);
}

int intel7084_mdio_wr(u16 Offset, u16 Shift, u16 Size, u32 value){
	 return gsw_reg_wr((void*)pedev0[0], Offset, Shift, Size, value);
}

int intel7084_mmd_rd(GSW_MMD_data_t *parm){
	return GSW_MmdDataRead((void *)pedev0[0], parm);
}

int intel7084_mmd_wr(GSW_MMD_data_t *parm){
	return GSW_MmdDataWrite((void *)pedev0[0], parm);
}

int intel7084_count_rd(GSW_RMON_Port_cnt_t *parm){
	return GSW_RMON_Port_Get((void*)pedev0[0], parm);
}

int intel7084_count_clear(GSW_RMON_clear_t *parm){
	return GSW_RMON_Clear((void*)pedev0[0], parm);
}

int intel7084_multicast_set(void){
	GSW_multicastSnoopCfg_t parm;
	GSW_multicastRouter_t parm1;

	GSW_MulticastSnoopCfgGet((void *)pedev0[0],
		&parm);

	parm1.nPortId = 5;
	GSW_MulticastRouterPortAdd((void *)pedev0[0],
			&parm1);

	parm.eIGMP_Mode = 2;
	parm.bMulticastUnknownDrop = 1;
	parm.eForwardPort = 3;
	parm.nForwardPortId = 5;

	GSW_MulticastSnoopCfgSet((void *)pedev0[0],
			&parm);
	return 0;
}

int intel7084_multicast_entry_set(u32 port, u8 type,u32 mc_ip){
	GSW_multicastTable_t parm;

	memset((void *)&parm, 0, sizeof(GSW_multicastTable_t));

	parm.nPortId = port;
	parm.eIPVersion = 0;
	parm.eModeMember = 2;
	parm.uIP_Gda.nIPv4 = htonl(mc_ip);//0xe001010b;

	if(type){
//		printk("add port %d to 224.1.1.11\n",port);
		GSW_MulticastTableEntryAdd((void *)pedev0[0],
				&parm);
	}
	else{
		//printk("del port %d from 224.1.1.11\n",port);
		GSW_MulticastTableEntryRemove((void *)pedev0[0],
				&parm);
	}
	return 0;
}

int intel7084_multicast_entry_get(void){
	GSW_multicastTableRead_t parm;
	int i = 0;

	memset((void *)&parm, 0, sizeof(GSW_multicastTableRead_t));
	parm.bInitial = 1;
	while(!parm.bLast){
		GSW_MulticastTableEntryRead((void *)pedev0[0],
				&parm);
		printk(">>>>entry %d:\n",i);
		printk("port %d\n",parm.nPortId);
		printk("ipv%d\n",(parm.eIPVersion == 0)?4:6);
		printk("dip = 0x%x\n",*(unsigned int *)&parm.uIP_Gda);
		printk("sip = 0x%x\n",*(unsigned int *)&parm.uIP_Gsa);
		printk("member %d\n",parm.eModeMember);
		i++;
		if(i == 64)
			break;
	}
	return 0;
}

int getportbyName(char * name){
	int i;

	for(i = 0;i < SF_DEFAULT_PORT_NO;i++){
		if(memcmp(name, &default_p2d_match[i], IF_NAMESIZE) == 0)
			return i;
	}
	return -1;
}

struct sf_intelsw_mc_list * intel7084_mc_list_search (u32 mcast_addr){
	struct sf_intelsw_mc_list * l_list = mc_list;

	while(l_list){
		if(l_list->mcast_addr == mcast_addr)
			break;
		l_list = (struct sf_intelsw_mc_list *)l_list->list.next;
	}
	return l_list;
}

int intel7084_mc_list_join (u32 mcast_addr, u32 port_list){
	int i;
	struct sf_intelsw_mc_list * l_list = intel7084_mc_list_search(mcast_addr);

	if(l_list == NULL){
		//new mcast group list
		l_list = vmalloc(sizeof(struct sf_intelsw_mc_list));
		memset(l_list , 0, sizeof(struct sf_intelsw_mc_list));

		if(mc_list == NULL){
			mc_list = l_list;
			mc_list_tail = l_list;
		}else{
			mc_list_tail->list.next = (struct list_head *)l_list;
			l_list->list.prev = (struct list_head *)mc_list_tail;
			mc_list_tail = l_list;
		}

		l_list->mcast_addr = mcast_addr;
		l_list->port_list = port_list;

		for(i = 0;i < SF_MAX_VIF;i++){
			if(BIT_TST(port_list, i))
				intel7084_multicast_entry_set(i, 1, mcast_addr);
		}

		intel7084_multicast_entry_set(RGMII_PORT0, 1, mcast_addr);//cpu port should join the group
	}else{
		//update port list in the group
		if(l_list->port_list != port_list){
			for(i = 0;i < SF_MAX_VIF;i++){
				if(BIT_TST(port_list, i) && !BIT_TST(l_list->port_list, i))
					intel7084_multicast_entry_set(i, 1, mcast_addr);
			}
			l_list->port_list = port_list;
		}
	}

	return 0;
}

int intel7084_mc_list_leave (u32 mcast_addr, u32 port_list){
	int i;
	struct sf_intelsw_mc_list * l_list = intel7084_mc_list_search(mcast_addr);

	if(l_list == NULL)
		return 0;

	if(port_list == 0){
		//destroy the mcast group
		for(i = 0;i < SF_MAX_VIF;i++){
			if(BIT_TST(l_list->port_list, i))
				intel7084_multicast_entry_set(i, 0, mcast_addr);
		}

		if(mc_list == mc_list_tail){//only one list
			mc_list = NULL;
			mc_list_tail = NULL;
		}else{
			if(l_list->list.prev == NULL){
				//the first one
				mc_list = (struct sf_intelsw_mc_list *)(mc_list->list.next);
				mc_list->list.prev = NULL;
			}else if(l_list->list.next == NULL){
				//the last one
				mc_list_tail = (struct sf_intelsw_mc_list *)(mc_list_tail->list.prev);
				mc_list_tail->list.next = NULL;
			}else{
				//the mid one
				list_del(&l_list->list);
			}
		}

		intel7084_multicast_entry_set(RGMII_PORT0, 0, mcast_addr);//cpu port should leave the group
		vfree(l_list);
	}else{
		//update port list in the group
		for(i = 0;i < SF_MAX_VIF;i++){
			if(BIT_TST(l_list->port_list, i) && !BIT_TST(port_list, i))
				intel7084_multicast_entry_set(i, 0, mcast_addr);
		}
		l_list->port_list = port_list;
	}

	return 0;
}

int intel7084_mc_list_clear (void){
	struct sf_intelsw_mc_list * l_list = mc_list;

	while(l_list){
		intel7084_mc_list_leave (l_list->mcast_addr, 0);
		l_list = (struct sf_intelsw_mc_list *)l_list->list.next;
	}

	return 0;
}

int intel7084_mc_ioctl (unsigned int type, unsigned long arg){
	struct sf_mc_route args;
	struct sf_mc_route * p_args = &args;
	int i,port;
	u32 port_list = 0;

//	printk("??????>>>>>%s start\n", __FUNCTION__);
//	printk("type %d\n",type);
	if (copy_from_user(&args, (const void *)arg, sizeof(struct sf_mc_route))){
		p_args = NULL;
		goto do_cmd;
	}

//	printk("mcast group = 0x%x\n", args.mcast_addr);

	for(i = 0; i < SF_MAX_VIF;i++){
		if(args.ovifs[i].s_addr != 0){
//			printk("index %d, name %s,ip = 0x%x\n",i, args.ovifs[i].Name,args.ovifs[i].s_addr);
			port = getportbyName(&args.ovifs[i].Name[0]);
			if(port >= 0)
				BIT_SET(port_list, port);
//			printk("port %d\n", port);
		}
	}

do_cmd:
	switch(type){
		case SF_INIT_MC_ROUTE:
			intel7084_multicast_set();
			break;

		case SF_ADD_MC_ROUTE:
			if(p_args == NULL)
				return 0;
			intel7084_mc_list_join(args.mcast_addr, port_list);
			break;

		case SF_DEL_MC_ROUTE:
			if(p_args == NULL)
				return 0;
			intel7084_mc_list_leave(args.mcast_addr, port_list);
			break;

		case SF_CLEAR_MC_ROUTE:
			intel7084_mc_list_clear();
			break;

		default:
			break;
	}

	return 0;
}


#ifdef CONFIG_SWCONFIG
static int
intel7084_get_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	struct switch_port *port;
	GSW_VLAN_portMemberRead_t parm;
	int i;

	if (val->port_vlan < 0)
		return -EINVAL;

	memset((void *)&parm, 0, sizeof(GSW_VLAN_portMemberRead_t));
	SF_MDIO_LOCK();
	GSW_VLAN_PortMemberRead((void *)pedev0[0], &parm);
	SF_MDIO_UNLOCK();

	port = &val->value.ports[0];
	val->len = 0;
	for (i = 0; i < INTEL_SWITCH_PORT_NUM; i++) {
		if (!(parm.nPortId & BIT(i)))
			continue;

		port->id = i;
		port->flags = (parm.nTagId & BIT(i)) ?
					0 : BIT(SWITCH_PORT_FLAG_TAGGED);
		val->len++;
		port++;
	}

	return 0;
}

static int
intel7084_set_vlan_ports(struct switch_dev *dev, struct switch_val *val)
{
	GSW_VLAN_IdCreate_t vlan;
	GSW_VLAN_portMemberAdd_t parm;
	struct vlan_entry *entry = NULL;
	struct list_head *pos;
	int i = 0, member = 0;

	if (val->port_vlan < 0 || val->port_vlan > VLAN_MAP_TBL_SIZE)
		return -EINVAL;

	if ((val->port_vlan >= 4000) && (val->port_vlan < 4032))
		return -EINVAL; // reserve such vlan for hnat to wifi

	memset((void *)&vlan, 0, sizeof(GSW_VLAN_IdCreate_t));
	memset((void *)&parm, 0, sizeof(GSW_VLAN_portMemberAdd_t));
	vlan.nVId = val->port_vlan;
	vlan.nFId = vlan.nVId; // RM#8744 fix lan-wan bridge fail
	SF_MDIO_LOCK();
	GSW_VLAN_IdCreate((void *)pedev0[0], &vlan);
	SF_MDIO_UNLOCK();

	parm.nVId = val->port_vlan;
	for (i = 0; i < val->len; i++) {
		struct switch_port *p = &val->value.ports[i];
		if (p->id > INTEL_SWITCH_PORT_NUM)
			return -EINVAL;

		parm.nPortId = p->id;
		if (p->flags & BIT(SWITCH_PORT_FLAG_TAGGED))
			parm.bVLAN_TagEgress = 1;
		else
			parm.bVLAN_TagEgress = 0;

		member |= BIT(p->id);
		SF_MDIO_LOCK();
		GSW_VLAN_PortMemberAdd((void *)pedev0[0], &parm);
		SF_MDIO_UNLOCK();
		printk("add port:%d for vlan:%d tagged:%d\n", p->id,
				val->port_vlan, parm.bVLAN_TagEgress);
	}

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

static int intel7084_get_port_pvid(struct switch_dev *dev, int port, int *val)
{
	GSW_return_t s;
	ur nPortVId;

	SF_MDIO_LOCK();
	s = gsw_reg_rd((void *)pedev0[0],
		(PCE_DEFPVID_PVID_OFFSET + (10 * port)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, &nPortVId);
	SF_MDIO_UNLOCK();
	if (s != GSW_statusOk)
		return s;

	*val = nPortVId;
	return 0;
}

static int intel7084_set_port_pvid(struct switch_dev *dev, int port, int pvid)
{
	GSW_return_t s;

	if ((pvid >= 4000) && (pvid < 4032))
		return -EINVAL; // reserve such vlan for hnat to wifi

	printk("set port:%d pvid:%d\n", port, pvid);
	SF_MDIO_LOCK();
	s = gsw_reg_wr((void *)pedev0[0],
		(PCE_DEFPVID_PVID_OFFSET + (10 * port)),
		PCE_DEFPVID_PVID_SHIFT,
		PCE_DEFPVID_PVID_SIZE, pvid);
	SF_MDIO_UNLOCK();
	if (s != GSW_statusOk)
		return s;

	return 0;
}

static int intel7084_get_port_link(struct switch_dev *dev,  int port,
		struct switch_port_link *link)
{
	GSW_portLinkCfg_t parm;

	memset((void *)&parm, 0, sizeof(GSW_portLinkCfg_t));
	parm.nPortId = port;
	SF_MDIO_LOCK();
	GSW_PortLinkCfgGet((void *)pedev0[0], &parm);
	SF_MDIO_UNLOCK();

	link->link = !parm.eLink;
	link->duplex = !parm.eDuplex;
	link->speed = parm.eSpeed;
	// printk("get port:%d linkStatus:%d duplex:%d speed:%d\n",
			// port, parm.eLink, parm.eDuplex, parm.eSpeed);

	return 0;
}

static int intel7084_apply_vlan_config(struct switch_dev *dev)
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

static int intel7084_reset_vlan(struct switch_dev *dev)
{
	struct vlan_entry *entry = NULL;
	struct list_head *pos = NULL;
	struct list_head *tmp = NULL;

	list_for_each_safe(pos, tmp, &(vlan_entries.entry_list)) {
		entry = list_entry(pos, struct vlan_entry, entry_list);
		list_del(pos);
		kfree(entry);
	}
	return 0;
}

static int intel7084_get_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	ur r;
	GSW_return_t s;
	SF_MDIO_LOCK();
	s = gsw_reg_rd((void *)pedev0[0],
		PCE_GCTRL_0_VLAN_OFFSET,
		PCE_GCTRL_0_VLAN_SHIFT,
		PCE_GCTRL_0_VLAN_SIZE, &r);
	SF_MDIO_UNLOCK();
	if (s != GSW_statusOk)
		return s;

	val->value.i = r;
	return 0;
}

static int intel7084_set_vlan_enable(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	// enable vlan aware
	GSW_return_t s;
	SF_MDIO_LOCK();
	s = gsw_reg_wr((void *)pedev0[0],
		PCE_GCTRL_0_VLAN_OFFSET,
		PCE_GCTRL_0_VLAN_SHIFT,
		PCE_GCTRL_0_VLAN_SIZE, val->value.i);
	SF_MDIO_UNLOCK();
	if (s != GSW_statusOk)
		return s;

	return 0;
}

static int intel7084_get_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	GSW_VLAN_IdGet_t parm;

	memset((void *)&parm, 0, sizeof(GSW_VLAN_IdGet_t));
	parm.nVId = val->port_vlan;
	SF_MDIO_LOCK();
	GSW_VLAN_IdGet((void *)pedev0[0], &parm);
	SF_MDIO_UNLOCK();

	val->value.i = parm.nFId;

	return 0;
}

static int intel7084_set_vlan_fid(struct switch_dev *dev,
		const struct switch_attr *attr,
		struct switch_val *val)
{
	GSW_VLAN_IdCreate_t parm;

	memset((void *)&parm, 0, sizeof(GSW_VLAN_IdCreate_t));
	parm.nVId = val->port_vlan;
	parm.nFId = val->value.i;
	SF_MDIO_LOCK();
	GSW_VLAN_IdCreate((void *)pedev0[0], &parm);
	SF_MDIO_UNLOCK();

	return 0;
}

static struct switch_attr intel7084_globals[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "enable_vlan",
		.description = "Enable VLAN mode",
		.get = intel7084_get_vlan_enable,
		.set = intel7084_set_vlan_enable,
		.max = 1,
		.ofs = 1
	}
};

static struct switch_attr intel7084_port[] = {
};

static struct switch_attr intel7084_vlan[] = {
	{
		.type = SWITCH_TYPE_INT,
		.name = "fid",
		.description = "Get/Set vlan FID",
		.set = intel7084_set_vlan_fid,
		.get = intel7084_get_vlan_fid,
	},
};

struct switch_dev_ops intel7084_switch_ops = {
	.attr_global = {
		.attr = intel7084_globals,
		.n_attr = ARRAY_SIZE(intel7084_globals),
	},
	.attr_port = {
		.attr = intel7084_port,
		.n_attr = ARRAY_SIZE(intel7084_port),
	},
	.attr_vlan = {
		.attr = intel7084_vlan,
		.n_attr = ARRAY_SIZE(intel7084_vlan),
	},

	.get_vlan_ports = intel7084_get_vlan_ports,
	.set_vlan_ports = intel7084_set_vlan_ports,
	.get_port_pvid = intel7084_get_port_pvid,
	.set_port_pvid = intel7084_set_port_pvid,
	.get_port_link = intel7084_get_port_link,
	.apply_config = intel7084_apply_vlan_config,
	.reset_switch = intel7084_reset_vlan,
};
#endif

int intel7084_check_phy_linkup(int port)
{
	ur r;
	GSW_return_t s;

	SF_MDIO_LOCK();
	s = gsw_reg_rd((void *)pedev0[0],
		(MAC_PSTAT_LSTAT_OFFSET + (0xC * port)),
		MAC_PSTAT_LSTAT_SHIFT,
		MAC_PSTAT_LSTAT_SIZE, &r);
	SF_MDIO_UNLOCK();
	if (s != GSW_statusOk)
		return s;

	return r ? 1 : 0;
}

void intel7084_led_init(int led_mode)
{
	ethsw_api_dev_t *pedev = pedev0[0];
	GSW_MMD_data_t md;
	int i = 0, mode = 0;

	if (led_mode == LED_ALL_ON)
		mode = GROUND_MODE;
	else if (led_mode == LED_ALL_BLINK)
		return; // switch not support, please use all on/off
	else
		mode = POWER_MODE;

	SF_MDIO_LOCK();
	// port[0:4] <--> led[5:9]
	gsw_reg_wr((void *)pedev, LED_MD_CFG_LED5_OFFSET,
			LED_MD_CFG_LED5_SHIFT,
			LED_MD_CFG_LED5_SIZE,
			mode);
	gsw_reg_wr((void *)pedev, LED_MD_CFG_LED6_OFFSET,
			LED_MD_CFG_LED6_SHIFT,
			LED_MD_CFG_LED6_SIZE,
			mode);
	gsw_reg_wr((void *)pedev, LED_MD_CFG_LED7_OFFSET,
			LED_MD_CFG_LED7_SHIFT,
			LED_MD_CFG_LED7_SIZE,
			mode);
	gsw_reg_wr((void *)pedev, LED_MD_CFG_LED8_OFFSET,
			LED_MD_CFG_LED8_SHIFT,
			LED_MD_CFG_LED8_SIZE,
			mode);
	gsw_reg_wr((void *)pedev, LED_MD_CFG_LED9_OFFSET,
			LED_MD_CFG_LED9_SHIFT,
			LED_MD_CFG_LED9_SIZE,
			mode);

	// config led when link is 10/100/1000Mbit/s
	for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
		md.nAddressDev = i;
		md.nAddressReg = 0x1F01E4;
		if (led_mode == LED_ALL_OFF)
			md.nData = 0x0; // no blink
		else
			md.nData = 0x70; // active on 10/100/1000

		GSW_MmdDataWrite((void *)pedev0[0], &md);
	}
	SF_MDIO_UNLOCK();
}

void intel7084_ifg_init(void)
{
	// set ifg to 80 bit time
	intel7084_mdio_wr((MAC_CTRL_1_IPG_OFFSET + (RGMII_PORT0 * 0xc)),
			MAC_CTRL_1_IPG_SHIFT,
			MAC_CTRL_1_IPG_SIZE,
			0xa);
}

void intel7084_disable_all_phy(void)
{
	GSW_MDIO_data_t parm;
	int i;

	SF_MDIO_LOCK();
	parm.nAddressReg = 0;
	for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
		parm.nAddressDev = i;
		intel7084_phy_rd(&parm);
		parm.nData |= PHY_CTRL_ENABLE_POWER_DOWN;
		intel7084_phy_wr(&parm);
	}
	SF_MDIO_UNLOCK();
}

void intel7084_enable_all_phy(struct sf_eswitch_priv *pesw_priv)
{
	GSW_MDIO_data_t parm;
	int i;

	SF_MDIO_LOCK();
	parm.nAddressReg = 0;
	for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
		if (!check_port_in_portlist(pesw_priv, i))
			continue;

		parm.nAddressDev = i;
		intel7084_phy_rd(&parm);
		parm.nData &= ~PHY_CTRL_ENABLE_POWER_DOWN;
		intel7084_phy_wr(&parm);
	}
	SF_MDIO_UNLOCK();
}


static int intel_rgmii_init(int port)
{
	int s;
	GSW_MDIO_data_t md;
	uint r;

	s = intel7084_mdio_rd((MAC_PSTAT_PACT_OFFSET + (0xC * port)),
			MAC_PSTAT_PACT_SHIFT,
			MAC_PSTAT_PACT_SIZE, &r);
	if (s != 0)
		return s;

	if (r) {
		uint pa, fd, pc;
		s = intel7084_mdio_rd((PHY_ADDR_0_ADDR_OFFSET - port),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_ADDR_SIZE, &pa);
		if (s != 0)
			return s;

		fd = 1; //DUPLEX_FULL
		pc = PHY_AN_ADV_1000FDX;
		md.nAddressReg = 9;
		md.nAddressDev = pa;
		s = intel7084_phy_rd(&md);
		if (s != 0)
			return s;
		md.nData &= ~(PHY_AN_ADV_1000HDX
				| PHY_AN_ADV_1000FDX);
		md.nData |= pc;
		md.nAddressDev = pa;
		md.nAddressReg = 9;
		s = intel7084_phy_wr(&md);
		if (s != 0)
			return s;
		md.nAddressReg = 4;
		md.nAddressDev = pa;
		s = intel7084_phy_rd(&md);
		if (s != 0)
			return s;
		md.nData &= ~(PHY_AN_ADV_10HDX
				| PHY_AN_ADV_10FDX
				| PHY_AN_ADV_100HDX
				| PHY_AN_ADV_100FDX);
		md.nAddressReg = 4;
		md.nAddressDev = pa;
		s = intel7084_phy_wr(&md);
		if (s != 0)
			return s;

		md.nAddressDev = pa;
		md.nAddressReg = 0;
		s = intel7084_phy_rd(&md);
		if (s != 0)
			return s;
		md.nData = 0x1200;
		s = intel7084_phy_wr(&md);
		if (s != 0)
			return s;
	} else {
		uint pr;
		s = intel7084_mdio_rd((PHY_ADDR_0_ADDR_OFFSET - port),
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
		s = intel7084_mdio_wr((PHY_ADDR_0_ADDR_OFFSET - port),
				PHY_ADDR_0_ADDR_SHIFT,
				PHY_ADDR_0_REG_SIZE,
				pr);
		if (s != 0)
			return s;
	}
	//'Low Power Idle' Support for 'Energy Efficient Ethernet'. default dsiable.
	s = intel7084_mdio_wr((MAC_CTRL_4_LPIEN_OFFSET + (0xC * port)),
		MAC_CTRL_4_LPIEN_SHIFT,
		MAC_CTRL_4_LPIEN_SIZE, 0);
	printk("End %s\n", __func__);
	return 0;
}

// #ifdef CONFIG_SF19A28_V2
static int intel_port_rgmii_dalay_set(int port, int txDelay, int rxDelay)
{
	if ((txDelay > 7) || (rxDelay > 7))
	  return -1;

	if(port == 5) {
		intel7084_mdio_wr(PCDU_5_RXDLY_OFFSET,
					PCDU_5_RXDLY_SHIFT,
					PCDU_5_RXDLY_SIZE,
					rxDelay);
		intel7084_mdio_wr(PCDU_5_TXDLY_OFFSET,
					PCDU_5_TXDLY_SHIFT,
					PCDU_5_TXDLY_SIZE,
					txDelay);
	}else if(port == 6) {
		intel7084_mdio_wr(PCDU_6_RXDLY_OFFSET,
					PCDU_6_RXDLY_SHIFT,
					PCDU_6_RXDLY_SIZE,
					rxDelay);
		intel7084_mdio_wr(PCDU_6_TXDLY_OFFSET,
					PCDU_6_TXDLY_SHIFT,
					PCDU_6_TXDLY_SIZE,
					txDelay);
	}else
	  return -1;

	return 0;
}
// #endif

u32 intel7084_get_cpu_port_rx_mib(void)
{
    GSW_RMON_clear_t mp;
    GSW_RMON_Port_cnt_t parm;

    SF_MDIO_LOCK();
    parm.nPortId = RGMII_PORT0;
    intel7084_count_rd(&parm);
    // clear mib count after read
    mp.nRmonId = RGMII_PORT0;
    mp.eRmonType = GSW_RMON_ALL_TYPE;
    intel7084_count_clear(&mp);
    SF_MDIO_UNLOCK();
    return parm.nRxGoodPkts;
}

int intel7084_set_cpu_port_self_mirror(struct sf_eswitch_priv *pesw_priv, int port, int enable)
{
	GSW_portCfg_t pCfg;
	GSW_monitorPortCfg_t parm;
	GSW_MDIO_data_t mdioData;
	int i;

	SF_MDIO_LOCK();
	parm.nPortId = RGMII_PORT0;
	pCfg.nPortId = RGMII_PORT0;
	mdioData.nAddressReg = 0;
	GSW_PortCfgGet((void *)pedev0[0], &pCfg);
	GSW_MonitorPortCfgGet((void *)pedev0[0], &parm);

	if (enable) {
		parm.bMonitorPort = 1;
		pCfg.ePortMonitor = GSW_PORT_MONITOR_RX;
		// disable vlan
		gsw_reg_wr((void *)pedev0[0],
				PCE_GCTRL_0_VLAN_OFFSET,
				PCE_GCTRL_0_VLAN_SHIFT,
				PCE_GCTRL_0_VLAN_SIZE, 0);
		// disable all phy
		for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
			mdioData.nAddressDev = i;
			intel7084_phy_rd(&mdioData);
			mdioData.nData |= PHY_CTRL_ENABLE_POWER_DOWN;
			intel7084_phy_wr(&mdioData);
		}
	} else {
		parm.bMonitorPort = 0;
		pCfg.ePortMonitor = GSW_PORT_MONITOR_NONE;
		// enable vlan
		gsw_reg_wr((void *)pedev0[0],
				PCE_GCTRL_0_VLAN_OFFSET,
				PCE_GCTRL_0_VLAN_SHIFT,
				PCE_GCTRL_0_VLAN_SIZE, 1);
		// enable all phy
		for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
			if (!check_port_in_portlist(pesw_priv, i))
				continue;

			mdioData.nAddressDev = i;
			intel7084_phy_rd(&mdioData);
			mdioData.nData &= ~PHY_CTRL_ENABLE_POWER_DOWN;
			intel7084_phy_wr(&mdioData);
		}
	}
	GSW_MonitorPortCfgSet((void *)pedev0[0], &parm);
	GSW_PortCfgSet((void *)pedev0[0], &pCfg);
	SF_MDIO_UNLOCK();
	return 0;
}

static void sf_speedtest_speed_up(void)
{
	GSW_PCE_rule_t a;
	// set tcp dport 8080 packet not trigger acmDrop
	memset(&a, 0, sizeof(a));
	a.pattern.nIndex = 0;
	a.pattern.bEnable = 1;
	a.pattern.nProtocol = 0x6;
	a.pattern.nProtocolMask = 0;
	a.pattern.bProtocolEnable = 1;
	a.pattern.nAppDataLSB = 8080;
	a.pattern.bAppDataLSB_Enable = 1;
	a.action.eCritFrameAction = 2;
	GSW_PceRuleWrite((void*)pedev0[0], &a);
	// set tcp sport 8080 packet not trigger acmDrop
	memset(&a, 0, sizeof(a));
	a.pattern.nIndex = 1;
	a.pattern.bEnable = 1;
	a.pattern.nProtocol = 0x6;
	a.pattern.nProtocolMask = 0;
	a.pattern.bProtocolEnable = 1;
	a.pattern.nAppDataMSB = 8080;
	a.pattern.bAppDataMSB_Enable = 1;
	a.action.eCritFrameAction = 2;
	GSW_PceRuleWrite((void*)pedev0[0], &a);
}

void intel7084_init(struct sf_eswitch_priv *pesw_priv)
{
	GSW_MMD_data_t md;
#ifdef CONFIG_SWCONFIG
	GSW_VLAN_portCfg_t pVlanCfg;
	int i;
#endif

	intel_rgmii_init(5);
	if(pesw_priv->model == INTEL7082) {
		intel_port_rgmii_dalay_set(5, 0, 0);
	} else if(pesw_priv->model == INTEL7084) {
		intel_port_rgmii_dalay_set(5, 0, 2);
 	}
#ifdef CONFIG_SF19A28_V2
	intel_port_rgmii_dalay_set(5, 2, 0);
#endif
	intel7084_enable_all_phy(pesw_priv);

	ethsw_swapi_register();
	sf_speedtest_speed_up();

	/* RM#9120 disable auto downspeed */
	for (i = 0; i < INTEL_PHY_PORT_NUM; i++) {
		md.nAddressDev = i;
		md.nAddressReg = 0x1F01EF;
		md.nData = 0x2;
		GSW_MmdDataWrite((void *)pedev0[0], &md);
	}
#ifdef CONFIG_SWCONFIG
	/* port config init */
	for (i = 0; i < INTEL_SWITCH_PORT_NUM; i++) {
		pVlanCfg.nPortId = i;
		GSW_VLAN_PortCfgGet((void *)pedev0[0], &pVlanCfg);

		pVlanCfg.eAdmitMode = GSW_VLAN_ADMIT_ALL;
		pVlanCfg.eVLAN_MemberViolation = GSW_VLAN_MEMBER_VIOLATION_BOTH;
		GSW_VLAN_PortCfgSet((void *)pedev0[0], &pVlanCfg);
	}
#endif
}

void intel7084_deinit(struct sf_eswitch_priv *pesw_priv)
{
	intel7084_disable_all_phy();
	ethsw_swapi_unregister();
}

int sf_intel_getAsicReg(unsigned int Offset, unsigned int *value)
{
	u16 Shift = 0, Size = 16;
	return intel7084_mdio_rd(Offset, Shift, Size, value);
}

int sf_intel_setAsicReg(unsigned int Offset, unsigned int value)
{
	u16 Shift = 0, Size = 16;
	return intel7084_mdio_wr(Offset, Shift, Size, value);
}

void intel7084_dumpmac(char *macaddr, int port)
{
     int max_nAgeTimer = 0;
     GSW_MAC_tableRead_t mac_table;
     mac_table.bInitial = LTQ_TRUE;
     memset(&mac_table, 0, sizeof(GSW_MAC_tableRead_t));
     while(1){
         mac_table.bInitial = 0;
         mac_table.nPortId = port;
         SF_MDIO_LOCK();
         if (GSW_MAC_TableEntryRead((void *)pedev0[0], &mac_table) != GSW_statusOk)
             return;
         SF_MDIO_UNLOCK();
         if (mac_table.bLast == LTQ_TRUE)
         {    
             //This check looks if the last table entry was found.
             //This is done when no more values are found.
             break;
         }        
 
         if (port == (int)mac_table.nPortId){
             if (mac_table.nAgeTimer > max_nAgeTimer)
             {
                 max_nAgeTimer = mac_table.nAgeTimer;
                 sprintf(macaddr,"%pM", mac_table.nMAC);
             }
         }
     }
}

struct sf_eswitch_api_t intel7084_api = {
#ifdef CONFIG_SWCONFIG
	.ops = &intel7084_switch_ops,
#endif
	.vender_init = intel7084_init,
	.vender_deinit = intel7084_deinit,
	.led_init = intel7084_led_init,
	.ifg_init = intel7084_ifg_init,
	.enable_all_phy = intel7084_enable_all_phy,
	.disable_all_phy = intel7084_disable_all_phy,
	.check_phy_linkup = intel7084_check_phy_linkup,
	.get_cpu_port_rx_mib = intel7084_get_cpu_port_rx_mib,
	.set_cpu_port_self_mirror = intel7084_set_cpu_port_self_mirror,
	.getAsicReg = sf_intel_getAsicReg,
	.setAsicReg = sf_intel_setAsicReg,
	.dump_mac = intel7084_dumpmac,
};
