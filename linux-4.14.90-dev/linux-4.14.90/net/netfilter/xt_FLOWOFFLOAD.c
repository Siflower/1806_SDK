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
#include <net/netfilter/nf_flow_table.h>

static struct nf_flowtable nf_flowtable;
static HLIST_HEAD(hooks);
static DEFINE_SPINLOCK(hooks_lock);
static struct delayed_work hook_work;
static struct hlist_head ts_blacklist;

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
xt_flowoffload_skip(struct sk_buff *skb)
{
	struct ip_options *opt = &(IPCB(skb)->opt);

	if (unlikely(opt->optlen))
		return true;
	if (skb_sec_path(skb))
		return true;

	return false;
}

static struct dst_entry *
xt_flowoffload_dst(const struct nf_conn *ct, enum ip_conntrack_dir dir,
		   const struct xt_action_param *par)
{
	struct dst_entry *dst = NULL;
	struct flowi fl;

	memset(&fl, 0, sizeof(fl));
	switch (xt_family(par)) {
	case NFPROTO_IPV4:
		fl.u.ip4.daddr = ct->tuplehash[dir].tuple.src.u3.ip;
		break;
	case NFPROTO_IPV6:
		fl.u.ip6.saddr = ct->tuplehash[dir].tuple.dst.u3.in6;
		fl.u.ip6.daddr = ct->tuplehash[dir].tuple.src.u3.in6;
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

	this_dst = xt_flowoffload_dst(ct, dir, par);
	other_dst = xt_flowoffload_dst(ct, !dir, par);
	if (!this_dst || !other_dst)
		return -ENOENT;

	if (dst_xfrm(this_dst) || dst_xfrm(other_dst))
		return -EINVAL;

	route->tuple[dir].dst		= this_dst;
	route->tuple[dir].ifindex	= xt_in(par)->ifindex;
	route->tuple[!dir].dst		= other_dst;
	route->tuple[!dir].ifindex	= xt_out(par)->ifindex;

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
	struct flow_offload_tuple *tuple;
	struct neighbour *n;

	tuple = &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple;
	n = dst_neigh_lookup(tuple->dst_cache, &tuple->src_v4);
	if (n && check_dev_in_blacklist(n->ha)) {
		neigh_release(n);
		return 1;
	}

	if (n)
		neigh_release(n);
	tuple = &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple;
	n = dst_neigh_lookup(tuple->dst_cache, &tuple->src_v4);
	if (n && check_dev_in_blacklist(n->ha)) {
		neigh_release(n);
		return 1;
	}

	if (n)
		neigh_release(n);
	return 0;
}

static void sf_flow_table_do_delete(struct flow_offload *flow, void *data)
{
	struct flow_offload_tuple *tuple;
	struct neighbour *n;
	u8 *mac = data;

	rcu_read_lock_bh();
	tuple = &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple;
	n = dst_neigh_lookup(tuple->dst_cache, &tuple->src_v4);
	if (n && ether_addr_equal(mac, n->ha))
		flow_offload_dead(flow);

	if (n)
		neigh_release(n);
	tuple = &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple;
	n = dst_neigh_lookup(tuple->dst_cache, &tuple->src_v4);
	if (n && ether_addr_equal(mac, n->ha))
		flow_offload_dead(flow);

	if (n)
		neigh_release(n);
	rcu_read_unlock_bh();
}

extern void nf_flow_offload_work_gc(struct work_struct *work);
static void sf_free_exist_flow(u8 *mac_addr)
{
	struct nf_flowtable *table = &nf_flowtable;

	cancel_delayed_work_sync(&table->gc_work);
	nf_flow_table_iterate(table, sf_flow_table_do_delete, mac_addr);
	INIT_DEFERRABLE_WORK(&table->gc_work, nf_flow_offload_work_gc);
	nf_flow_offload_work_gc(&table->gc_work.work);
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
	enum ip_conntrack_info ctinfo;
	enum ip_conntrack_dir dir;
	struct nf_flow_route route;
	struct flow_offload *flow;
	struct nf_conn *ct;

	if (xt_flowoffload_skip(skb))
		return XT_CONTINUE;

	ct = nf_ct_get(skb, &ctinfo);
	if (ct == NULL)
		return XT_CONTINUE;

	switch (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum) {
	case IPPROTO_TCP:
		if (ct->proto.tcp.state != TCP_CONNTRACK_ESTABLISHED)
			return XT_CONTINUE;
		break;
	case IPPROTO_UDP:
		break;
	default:
		return XT_CONTINUE;
	}

	if (test_bit(IPS_HELPER_BIT, &ct->status))
		return XT_CONTINUE;

	if (ctinfo == IP_CT_NEW && atomic_read(&ct->ct_general.use) == 1){
		return XT_CONTINUE;
	}//RM6897,first pkt of udp conntrack without reply can't go on,other pkts can go on

	if (ctinfo == IP_CT_RELATED)
		return XT_CONTINUE;

	if (!xt_in(par) || !xt_out(par))
		return XT_CONTINUE;

	if (test_and_set_bit(IPS_OFFLOAD_BIT, &ct->status))
		return XT_CONTINUE;

	dir = CTINFO2DIR(ctinfo);

	if (xt_flowoffload_route(skb, ct, par, &route, dir) < 0)
		goto err_flow_route;

	flow = flow_offload_alloc(ct, &route);
	if (!flow)
		goto err_flow_alloc;

//RM#8396  avoid delete flow offload called before hw add
	xt_flowoffload_check_device(xt_in(par));
	xt_flowoffload_check_device(xt_out(par));

	if (info->flags & XT_FLOWOFFLOAD_HW) {
		if (!check_flow_in_blacklist(xt_net(par), flow))
			nf_flow_offload_hw_add(xt_net(par), flow, ct);
	}

	if (flow_offload_add(&nf_flowtable, flow) < 0)
		goto err_flow_add;


	return XT_CONTINUE;

err_flow_add:
	if (flow->flags & FLOW_OFFLOAD_HW)
		nf_flow_offload_hw_del(xt_net(par), flow);
	flow_offload_free(flow);
err_flow_alloc:
	dst_release(route.tuple[!dir].dst);
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

	if (event != NETDEV_UNREGISTER)
		return NOTIFY_DONE;

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
