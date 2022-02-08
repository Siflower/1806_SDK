/*
 * Copyright (C) 2018 Felix Fietkau <nbd@nbd.name>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter/xt_FLOWOFFLOAD.h>
#include <linux/etherdevice.h>
#include <net/ip.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_flow_table.h>
#include <linux/inetdevice.h>

#include <linux/rtnetlink.h>
#include <linux/socket.h>

static struct nf_flowtable nf_flowtable;
static HLIST_HEAD(hooks);
static DEFINE_SPINLOCK(hooks_lock);
static struct delayed_work hook_work;
static struct hlist_head ts_blacklist;
// #if IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER)

struct blacklist_dev {
    struct hlist_node node;
    u8 mac[ETH_ALEN];
};

struct xt_flowoffload_hook {
	struct hlist_node list;
	struct nf_hook_ops ops;
	struct net *net;
	bool registered;
	bool used;
};

static unsigned int
xt_flowoffload_net_hook(void *priv, struct sk_buff *skb,
			  const struct nf_hook_state *state)
{
	switch (skb->protocol) {
	case htons(ETH_P_IP):
		return nf_flow_offload_ip_hook(priv, skb, state);
	case htons(ETH_P_IPV6):
		return nf_flow_offload_ipv6_hook(priv, skb, state);
	}

	return NF_ACCEPT;
}

static int
xt_flowoffload_create_hook(struct net_device *dev)
{
	struct xt_flowoffload_hook *hook;
	struct nf_hook_ops *ops;

	hook = kzalloc(sizeof(*hook), GFP_ATOMIC);
	if (!hook)
		return -ENOMEM;

	ops = &hook->ops;
	ops->pf = NFPROTO_NETDEV;
	ops->hooknum = NF_NETDEV_INGRESS;
	ops->priority = 10;
	ops->priv = &nf_flowtable;
	ops->hook = xt_flowoffload_net_hook;
	ops->dev = dev;

	hlist_add_head(&hook->list, &hooks);
	mod_delayed_work(system_power_efficient_wq, &hook_work, 0);

	return 0;
}

static struct xt_flowoffload_hook *
flow_offload_lookup_hook(struct net_device *dev)
{
	struct xt_flowoffload_hook *hook;

	hlist_for_each_entry(hook, &hooks, list) {
		if (hook->ops.dev == dev)
			return hook;
	}

	return NULL;
}

static void
xt_flowoffload_check_device(struct net_device *dev)
{
	struct xt_flowoffload_hook *hook;

	spin_lock_bh(&hooks_lock);
	hook = flow_offload_lookup_hook(dev);
	if (hook)
		hook->used = true;
	else
		xt_flowoffload_create_hook(dev);
	spin_unlock_bh(&hooks_lock);
}

static void
xt_flowoffload_register_hooks(void)
{
	struct xt_flowoffload_hook *hook;

restart:
	hlist_for_each_entry(hook, &hooks, list) {
		if (hook->registered)
			continue;

		hook->registered = true;
		hook->net = dev_net(hook->ops.dev);
		spin_unlock_bh(&hooks_lock);
		nf_register_net_hook(hook->net, &hook->ops);
		spin_lock_bh(&hooks_lock);
		goto restart;
	}

}

static void
xt_flowoffload_cleanup_hooks(void)
{
	struct xt_flowoffload_hook *hook;

restart:
	hlist_for_each_entry(hook, &hooks, list) {
		if (hook->used || !hook->registered)
			continue;

		hlist_del(&hook->list);
		spin_unlock_bh(&hooks_lock);
		nf_unregister_net_hook(hook->net, &hook->ops);
		kfree(hook);
		spin_lock_bh(&hooks_lock);
		goto restart;
	}
}

static void
xt_flowoffload_check_hook(struct flow_offload *flow, void *data)
{
	struct flow_offload_tuple *tuple = &flow->tuplehash[0].tuple;
	struct xt_flowoffload_hook *hook;
	bool *found = data;

	spin_lock_bh(&hooks_lock);
	hlist_for_each_entry(hook, &hooks, list) {
		if (hook->ops.dev->ifindex != tuple->iifidx &&
		    hook->ops.dev->ifindex != tuple->oifidx)
			continue;

		hook->used = true;
		*found = true;
	}
	spin_unlock_bh(&hooks_lock);
}

static void
xt_flowoffload_hook_work(struct work_struct *work)
{
	struct xt_flowoffload_hook *hook;
	bool found = false;
	int err;

	spin_lock_bh(&hooks_lock);
	xt_flowoffload_register_hooks();
	hlist_for_each_entry(hook, &hooks, list)
		hook->used = false;
	spin_unlock_bh(&hooks_lock);

	err = nf_flow_table_iterate(&nf_flowtable, xt_flowoffload_check_hook,
				    &found);
	if (err && err != -EAGAIN)
	    goto out;

	spin_lock_bh(&hooks_lock);
	xt_flowoffload_cleanup_hooks();
	spin_unlock_bh(&hooks_lock);

out:
	if (found)
		queue_delayed_work(system_power_efficient_wq, &hook_work, HZ);
}

static bool
xt_flowoffload_skip(struct sk_buff *skb, int family)
{
	if (skb_sec_path(skb))
		return true;

	const struct ip_options *opt = &(IPCB(skb)->opt);

	if (family == NFPROTO_IPV4) {
		if (unlikely(opt->optlen))
		  return true;
	}

	return false;
}

static struct dst_entry *
xt_flowoffload_dst(const struct nf_conn *ct, enum ip_conntrack_dir dir,
		   const struct xt_action_param *par, int ifindex)
{
	struct dst_entry *dst = NULL;
	struct flowi fl;

	memset(&fl, 0, sizeof(fl));
	switch (xt_family(par)) {
	case NFPROTO_IPV4:
		fl.u.ip4.daddr = ct->tuplehash[dir].tuple.src.u3.ip;
		fl.u.ip4.flowi4_oif = ifindex;
		break;
	case NFPROTO_IPV6:
		fl.u.ip6.saddr = ct->tuplehash[dir].tuple.dst.u3.in6;
		fl.u.ip6.daddr = ct->tuplehash[dir].tuple.src.u3.in6;
		fl.u.ip6.flowi6_oif = ifindex;
		break;
	}

	nf_route(xt_net(par), &dst, &fl, false, xt_family(par));

	return dst;
}

static int
xt_flowoffload_route(struct sk_buff *skb, const struct nf_conn *ct,
		   const struct xt_action_param *par,
		   struct nf_flow_route *route, enum ip_conntrack_dir dir)
{
	struct dst_entry *this_dst, *other_dst;

	this_dst = xt_flowoffload_dst(ct, !dir, par, xt_out(par)->ifindex);
	other_dst = xt_flowoffload_dst(ct, dir, par, xt_in(par)->ifindex);

	route->tuple[dir].dst		= this_dst;
	route->tuple[!dir].dst		= other_dst;

	if (!this_dst || !other_dst)
		return -ENOENT;

	if (dst_xfrm(this_dst) || dst_xfrm(other_dst))
		return -EINVAL;


	return 0;
}

static struct blacklist_dev* check_dev_in_blacklist(u8 *mac)
{
	struct blacklist_dev *pos;
	if (mac == NULL)
		return NULL;

	hlist_for_each_entry(pos, &ts_blacklist, node) {
		if (ether_addr_equal(mac, pos->mac))
			return pos;
	}
	return NULL;
}

static int check_flow_in_blacklist(struct net *net, struct flow_offload *flow)
{
	struct flow_offload_tuple *tuple_s, *tuple_d;
	struct neighbour *n;

	tuple_s = &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple;
	tuple_d = &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple;

	n = dst_neigh_lookup(tuple_d->dst_cache, &tuple_s->src_v4);
	if (n && check_dev_in_blacklist(n->ha)) {
		neigh_release(n);
		return 1;
	}

	if (n)
		neigh_release(n);

	n = dst_neigh_lookup(tuple_s->dst_cache, &tuple_d->src_v4);
	if (n && check_dev_in_blacklist(n->ha)) {
		neigh_release(n);
		return 1;
	}

	if (n)
		neigh_release(n);

	return 0;
}

// #if IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER)
#if 1
static int sf_offload_hw_clean_flow_cb(struct flow_offload *flow, void *data)
{
	struct nf_flowtable *table = &nf_flowtable;
	struct flow_offload * hw_flow = (struct flow_offload *)data;
	unsigned char proto = 0;
	char ret = -1;
	if((hw_flow == flow) && (flow->flags & FLOW_OFFLOAD_HW)){

		flow->timeout = jiffies + 30*HZ;
		proto = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto;
		flow->hnat_idx = -1;
		flow->flags &= 0xff0000ff;
		flow->flags &= ~(FLOW_OFFLOAD_HW| FLOW_OFFLOAD_KEEP);

		atomic_dec(&(nf_flowtable.nf_count.hw_total_count));
		if(proto == IPPROTO_UDP)
		  atomic_dec(&(nf_flowtable.nf_count.hw_udp_count));
		else
		  atomic_dec(&(nf_flowtable.nf_count.hw_tcp_count));

		table->nf_count.clean_flow_count++;
		ret = 0;
	}
	return  ret;
}

static int sf_offload_hw_clean_flow_cb_no_dec(struct flow_offload *flow, void *data)
{
	struct flow_offload * hw_flow = (struct flow_offload *)data;
	char ret = -1;
	if((hw_flow == flow) && (flow->flags & FLOW_OFFLOAD_HW)){
		flow->timeout = jiffies + 30*HZ;
		flow->hnat_idx = -1;
		flow->flags &= 0xff0000ff;
		flow->flags &= ~(FLOW_OFFLOAD_HW| FLOW_OFFLOAD_KEEP);
		ret = 0;
	}
	return  ret;
}

static void sf_offload_hw_clean_all_cb(struct flow_offload *flow, void *data)
{
	if(flow->flags & FLOW_OFFLOAD_HW){
		flow->hnat_idx = -1;
		flow->flags &= 0xff0000ff;
		flow->flags &= ~(FLOW_OFFLOAD_HW| FLOW_OFFLOAD_KEEP);
		// flow->timeout = jiffies;
	}
	return;
}
static void sf_offload_hw_clean_lan_cb(struct flow_offload *flow, void *data)
{
	unsigned char lan_subnet_index = (unsigned char)data;
	unsigned char proto = 0;
	if((flow->flags & FLOW_OFFLOAD_HW) && (flow->flags & (0x1 << (lan_subnet_index + 8)))){

		proto = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto;
		flow->hnat_idx = -1;
		flow->flags &= 0xff0000ff;
		flow->flags &= ~(FLOW_OFFLOAD_HW| FLOW_OFFLOAD_KEEP);
		flow_offload_teardown(flow);

		atomic_dec(&(nf_flowtable.nf_count.hw_total_count));
		if(proto == IPPROTO_UDP)
		  atomic_dec(&(nf_flowtable.nf_count.hw_udp_count));
		else
		  atomic_dec(&(nf_flowtable.nf_count.hw_tcp_count));

	}

	return;
}

static void sf_offload_hw_clean_wan_cb(struct flow_offload *flow, void *data)
{
	unsigned char wan_subnet_index = (unsigned char)data;
	unsigned char proto = 0;
	if((flow->flags & FLOW_OFFLOAD_HW) && (flow->flags & (0x1 << (wan_subnet_index + 16)))){
		proto = flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto;
		flow->hnat_idx = -1;
		flow->flags &= 0xff0000ff;
		flow->flags &= ~(FLOW_OFFLOAD_HW| FLOW_OFFLOAD_KEEP);
		flow_offload_teardown(flow);

		atomic_dec(&(nf_flowtable.nf_count.hw_total_count));
		if(proto == IPPROTO_UDP)
		  atomic_dec(&(nf_flowtable.nf_count.hw_udp_count));
		else
		  atomic_dec(&(nf_flowtable.nf_count.hw_tcp_count));
	}

	return;
}
// mode 0  clean with flow ptr
// mode 1  clean with lan  index
// mode 2  clean with wan  index
// mode 3  clean  all
void sf_offload_hw_clean_flow(void *pclean, unsigned char clean_mode)
{
	struct nf_flowtable *table = &nf_flowtable;
	switch (clean_mode) {
		case 0:
			// under hight concurrency   flow maybe del from table not  del form  hw  yet. so  force del
			nf_flow_table_iterate_ret(table, sf_offload_hw_clean_flow_cb, pclean);
			break;
		case 1:
			nf_flow_table_iterate(table, sf_offload_hw_clean_lan_cb, pclean);
			break;
		case 2:
			nf_flow_table_iterate(table, sf_offload_hw_clean_wan_cb, pclean);
			break;
		case 3:
			nf_flow_table_iterate(table, sf_offload_hw_clean_all_cb, pclean);
			atomic_set(&(nf_flowtable.nf_count.hw_total_count),0);
			atomic_set(&(nf_flowtable.nf_count.hw_tcp_count),0);
			atomic_set(&(nf_flowtable.nf_count.hw_udp_count),0);
			break;
		case 4:
			nf_flow_table_iterate_ret(table, sf_offload_hw_clean_flow_cb_no_dec, pclean);
			break;
		default:
			printk("%s %d mode error %d\n",__FUNCTION__,__LINE__, clean_mode);
	}
	return;
}
EXPORT_SYMBOL(sf_offload_hw_clean_flow);

void sf_flow_dump_count(void){
	struct nf_flowtable_count *nf_count = &(nf_flowtable.nf_count);
	printk("nf dump total %d tcp %d udp %d \n", atomic_read(&nf_count->total_count),
				atomic_read(&nf_count->tcp_count), atomic_read(&nf_count->udp_count));

	printk(" hw total %d hw tcp %d hw udp %d crc clean flow %d\n", atomic_read(&nf_count->hw_total_count),
				atomic_read(&nf_count->hw_tcp_count), atomic_read(&nf_count->hw_udp_count), nf_count->clean_flow_count);

	printk("udp ageing  %d  full ageing %d\n", atomic_read(&nf_count->udp_age_count), atomic_read(&nf_count->full_age_count));
}
EXPORT_SYMBOL(sf_flow_dump_count);
#endif

static void sf_flow_table_do_delete(struct flow_offload *flow, void *data)
{
	struct flow_offload_tuple *tuple_s, *tuple_d;
	struct neighbour *n;
	u8 *mac = data;

	tuple_s = &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple;
	tuple_d = &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple;

	n = dst_neigh_lookup(tuple_d->dst_cache, &tuple_s->src_v4);
	if (n && ether_addr_equal(mac, n->ha))
	  flow_offload_teardown(flow);

	if (n)
	  neigh_release(n);

	n = dst_neigh_lookup(tuple_s->dst_cache, &tuple_d->src_v4);
	if (n && ether_addr_equal(mac, n->ha))
	  flow_offload_teardown(flow);

	if (n)
	  neigh_release(n);
}


static void sf_free_exist_flow(u8 *mac_addr)
{
	struct nf_flowtable *table = &nf_flowtable;
	nf_flow_table_iterate(table, sf_flow_table_do_delete, mac_addr);
}

int sf_add_dev_to_blacklist(u8 *mac)
{
	struct blacklist_dev *hnode = NULL;
	if (check_dev_in_blacklist(mac))
		return 0;

	hnode = kzalloc(sizeof(struct blacklist_dev), GFP_KERNEL);
	if (hnode == NULL) {
		printk("alloc blacklist_dev fail, no memory!");
		return -ENOMEM;
	}

	memcpy(hnode->mac, mac, ETH_ALEN);
	hlist_add_head(&hnode->node, &ts_blacklist);
	sf_free_exist_flow(mac);
	return 0;
}
EXPORT_SYMBOL(sf_add_dev_to_blacklist);

int sf_del_dev_from_blacklist(u8 *mac)
{
	struct blacklist_dev *pos = NULL;
	pos = check_dev_in_blacklist(mac);
	if (pos == NULL)
		return 0;

	hlist_del(&pos->node);
	kfree(pos);
	return 0;
}
EXPORT_SYMBOL(sf_del_dev_from_blacklist);

static void sf_destory_blacklist(void)
{
	struct blacklist_dev *pos;
	struct hlist_node *h;

	hlist_for_each_entry_safe(pos, h, &ts_blacklist, node) {
		hlist_del(&pos->node);
		kfree(pos);
	}
}

static unsigned int
flowoffload_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_flowoffload_target_info *info = par->targinfo;
	struct tcphdr _tcph, *tcph = NULL;
	struct udphdr _udph, *udph = NULL;
	enum ip_conntrack_info ctinfo;
	enum ip_conntrack_dir dir;
	struct nf_flow_route route;
	struct flow_offload *flow = NULL;
	struct nf_conn *ct;
	struct net *net;

	if (xt_flowoffload_skip(skb, xt_family(par)))
		return XT_CONTINUE;


	ct = nf_ct_get(skb, &ctinfo);
	if (ct == NULL)
		return XT_CONTINUE;

	switch (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum) {
	case IPPROTO_TCP:
		if (ct->proto.tcp.state != TCP_CONNTRACK_ESTABLISHED)
			return XT_CONTINUE;

		tcph = skb_header_pointer(skb, par->thoff,
					  sizeof(_tcph), &_tcph);
		if (unlikely(!tcph || tcph->fin || tcph->rst))
			return XT_CONTINUE;
		break;
	case IPPROTO_UDP:
		udph = skb_header_pointer(skb, par->thoff,
					  sizeof(_udph), &_udph);
// RM#9391 filter dhcp offer and dns packet
		if (ntohs(udph->source) == 0x43 || ntohs(udph->source) == 0x44 || ntohs(udph->dest) == 0x35){
			return XT_CONTINUE;
		}
//RM#10724 If UDP checksum is not zero,hardware acceleration will be entered.Open 5001 port for iperf.
#if 1
		if (ntohs(udph->source) == 5001 || ntohs(udph->dest) == 5001){
			//printk("============bypass port is 5001 sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
			break;
		}
		if(ct->status & IPS_SRC_NAT){
			if((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port == udph->source) && (udph->check != 0)){
				//printk("============bypass ORIGINAL client snat sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
				ct->udp_checksum_stat |= 0x1;
			}
			if((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port == udph->source) && (udph->check != 0)){
				//printk("============bypass ORIGINAL server dnat sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
				ct->udp_checksum_stat |= 0x2;
			}
		}else if(ct->status & IPS_DST_NAT){
			if((ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port == udph->source) && (udph->check != 0)){
					//printk("============bypass REPLY client snat sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
					ct->udp_checksum_stat |= 0x1;
			}
			if((ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port == udph->source) && (udph->check != 0)){
				//printk("============bypass REPLY server dnat sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
				ct->udp_checksum_stat |= 0x2;
			}
		}else{
			return XT_CONTINUE;
		}
		if (ct->udp_checksum_stat != 0x3){
			//printk("============bypass checksum!=3  sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
			return XT_CONTINUE;
		}else{
			//printk("============bypass checksum=3  sport is:%d dport is:%d\n",ntohs(udph->source), ntohs(udph->dest));
		}
#endif
		break;
	default:
		return XT_CONTINUE;
	}

	if (nf_ct_ext_exist(ct, NF_CT_EXT_HELPER) ||
	    ct->status & IPS_SEQ_ADJUST)
		return XT_CONTINUE;

	// if (ctinfo == IP_CT_NEW && atomic_read(&ct->ct_general.use) == 1){
	// 	return XT_CONTINUE;
	// }//RM6897,first pkt of udp conntrack without reply can't go on,other pkts can go on
//TODO: test udp here  iperf 2.1.1 verify ok
	if (!nf_ct_is_confirmed(ct))
		return XT_CONTINUE;

	if (!xt_in(par) || !xt_out(par))
		return XT_CONTINUE;

	if (test_and_set_bit(IPS_OFFLOAD_BIT, &ct->status))
		return XT_CONTINUE;

	dir = CTINFO2DIR(ctinfo);

	if (xt_flowoffload_route(skb, ct, par, &route, dir) == 0)
		flow = flow_offload_alloc(ct, &route);

	dst_release(route.tuple[dir].dst);
	dst_release(route.tuple[!dir].dst);

	if (!flow)
		goto err_flow_route;

	if (tcph) {
		ct->proto.tcp.seen[0].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
		ct->proto.tcp.seen[1].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
	}

	xt_flowoffload_check_device(xt_in(par));
	xt_flowoffload_check_device(xt_out(par));

	net = read_pnet(&nf_flowtable.ft_net);
	if (!net)
	  write_pnet(&nf_flowtable.ft_net, xt_net(par));


	if (flow_offload_add(&nf_flowtable, flow) < 0)
		goto err_flow_add;

	if ((info->flags & XT_FLOWOFFLOAD_HW) && (atomic_read(&(nf_flowtable.nf_count.hw_total_count)) < FLOWOFFLOAD_HW_MAX)) {
		if (!check_flow_in_blacklist(xt_net(par), flow))
			nf_flow_offload_hw_add(xt_net(par), flow, ct, &nf_flowtable.nf_count);
	}

	return XT_CONTINUE;

err_flow_add:
	flow_offload_free(flow);
err_flow_route:
	clear_bit(IPS_OFFLOAD_BIT, &ct->status);
	return XT_CONTINUE;
}


static int flowoffload_chk(const struct xt_tgchk_param *par)
{
	struct xt_flowoffload_target_info *info = par->targinfo;

	if (info->flags & ~XT_FLOWOFFLOAD_MASK)
		return -EINVAL;

	return 0;
}

static struct xt_target offload_tg_reg __read_mostly = {
	.family		= NFPROTO_UNSPEC,
	.name		= "FLOWOFFLOAD",
	.revision	= 0,
	.targetsize	= sizeof(struct xt_flowoffload_target_info),
	.usersize	= sizeof(struct xt_flowoffload_target_info),
	.checkentry	= flowoffload_chk,
	.target		= flowoffload_tg,
	.me		= THIS_MODULE,
};

static int xt_flowoffload_table_init(struct nf_flowtable *table)
{
	table->flags = NF_FLOWTABLE_F_HW;
	nf_flow_table_init(table);
	return 0;
}

static void xt_flowoffload_table_cleanup(struct nf_flowtable *table)
{
	nf_flow_table_free(table);
}


static int flow_offload_netdev_event(struct notifier_block *this,
				     unsigned long event, void *ptr)
{
	struct xt_flowoffload_hook *hook = NULL;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	if (event != NETDEV_UNREGISTER){
		return NOTIFY_DONE;
	}

	spin_lock_bh(&hooks_lock);
	hook = flow_offload_lookup_hook(dev);
	if (hook) {
		hlist_del(&hook->list);
	}
	spin_unlock_bh(&hooks_lock);
	if (hook) {
		nf_unregister_net_hook(hook->net, &hook->ops);
		kfree(hook);
	}

	nf_flow_table_cleanup(dev_net(dev), dev);

	return NOTIFY_DONE;
}



static struct notifier_block flow_offload_netdev_notifier = {
	.notifier_call	= flow_offload_netdev_event,
};

static int __init xt_flowoffload_tg_init(void)
{
	int ret;

	register_netdevice_notifier(&flow_offload_netdev_notifier);

	INIT_DELAYED_WORK(&hook_work, xt_flowoffload_hook_work);

	ret = xt_flowoffload_table_init(&nf_flowtable);
	if (ret)
	  return ret;

	ret = xt_register_target(&offload_tg_reg);
	if (ret)
	  xt_flowoffload_table_cleanup(&nf_flowtable);

	INIT_HLIST_HEAD(&ts_blacklist);
	return ret;
}

static void __exit xt_flowoffload_tg_exit(void)
{
	xt_unregister_target(&offload_tg_reg);
	xt_flowoffload_table_cleanup(&nf_flowtable);
	unregister_netdevice_notifier(&flow_offload_netdev_notifier);
	sf_destory_blacklist();
}

MODULE_LICENSE("GPL");
module_init(xt_flowoffload_tg_init);
module_exit(xt_flowoffload_tg_exit);
MODULE_ALIAS("ipt_FLOWOFFLOAD");
MODULE_ALIAS("ip6t_FLOWOFFLOAD");
