#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/rhashtable.h>
#include <linux/netdevice.h>
#include <net/netfilter/nf_flow_table.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_tuple.h>

#ifdef NF_FLOW_TABLE_HW_ASYNC
static DEFINE_SPINLOCK(flow_offload_hw_pending_list_lock);
static LIST_HEAD(flow_offload_hw_pending_list);

static DEFINE_MUTEX(nf_flow_offload_hw_mutex);
#else
// static DEFINE_SPINLOCK(flow_offload_hw_sync_lock);
#endif

struct flow_offload_hw {
	struct list_head	list;
	enum flow_offload_type	type;
	struct flow_offload	*flow;
	struct nf_conn		*ct;
#ifdef NF_FLOW_TABLE_HW_ASYNC
	struct nf_flowtable_count * nf_count;
#endif

	struct flow_offload_hw_path src;
	struct flow_offload_hw_path dest;
};

static void flow_offload_check_ethernet(struct flow_offload_tuple *tuple,
					struct dst_entry *dst,
					struct flow_offload_hw_path *path)
{
	struct net_device *dev = path->dev;
	struct neighbour *n;

	if (dev->type != ARPHRD_ETHER)
		return;

	memcpy(path->eth_src, path->dev->dev_addr, ETH_ALEN);
	n = dst_neigh_lookup(dst, &tuple->src_v4);
	if (!n)
		return;

	memcpy(path->eth_dest, n->ha, ETH_ALEN);
	path->flags |= FLOW_OFFLOAD_PATH_ETHERNET;
	neigh_release(n);
}

static int flow_offload_check_path(struct net *net,
				   struct flow_offload_tuple *tuple,
				   struct dst_entry *dst,
				   struct flow_offload_hw_path *path)
{
	struct net_device *dev;

	dev = dev_get_by_index_rcu(net, tuple->iifidx);
	if (!dev)
		return -ENOENT;

	path->dev = dev;
	flow_offload_check_ethernet(tuple, dst, path);

	if (dev->netdev_ops->ndo_flow_offload_check)
		return dev->netdev_ops->ndo_flow_offload_check(path);

	return 0;
}

static int do_flow_offload_hw(struct flow_offload_hw *offload)
{
	struct net_device *src_dev = offload->src.dev;
	struct net_device *dest_dev = offload->dest.dev;
	int ret;

	ret = src_dev->netdev_ops->ndo_flow_offload(offload->type,
						    offload->flow,
						    &offload->src,
						    &offload->dest);

	/* restore devices in case the driver mangled them */
	offload->src.dev = src_dev;
	offload->dest.dev = dest_dev;

	return ret;
}

static void flow_offload_hw_free(struct flow_offload_hw *offload)
{
	dev_put(offload->src.dev);
	dev_put(offload->dest.dev);
	if (offload->ct)
		nf_conntrack_put(&offload->ct->ct_general);
#ifdef NF_FLOW_TABLE_HW_ASYNC
	list_del(&offload->list);
#endif
	kfree(offload);
}

#ifdef NF_FLOW_TABLE_HW_ASYNC
static void flow_offload_hw_work(struct work_struct *work)
{
	struct flow_offload_hw *offload, *next;
	LIST_HEAD(hw_offload_pending);

	spin_lock_bh(&flow_offload_hw_pending_list_lock);
	list_replace_init(&flow_offload_hw_pending_list, &hw_offload_pending);
	spin_unlock_bh(&flow_offload_hw_pending_list_lock);

	list_for_each_entry_safe(offload, next, &hw_offload_pending, list) {
		mutex_lock(&nf_flow_offload_hw_mutex);
		switch (offload->type) {
			case FLOW_OFFLOAD_ADD:
				if (nf_ct_is_dying(offload->ct))
				  break;

				if (do_flow_offload_hw(offload) >= 0){
					offload->flow->flags |= (FLOW_OFFLOAD_HW |FLOW_OFFLOAD_KEEP);
					atomic_inc(&(offload->nf_count->hw_total_count));
					if(offload->flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto == IPPROTO_UDP)
					  atomic_inc(&(offload->nf_count->hw_udp_count));
					else
					  atomic_inc(&(offload->nf_count->hw_tcp_count));
				}
				break;
			case FLOW_OFFLOAD_DEL:
				do_flow_offload_hw(offload);
				break;
		}
		mutex_unlock(&nf_flow_offload_hw_mutex);

		flow_offload_hw_free(offload);
	}
}

static void flow_offload_queue_work(struct flow_offload_hw *offload)
{
	spin_lock_bh(&flow_offload_hw_pending_list_lock);
	list_add_tail(&offload->list, &flow_offload_hw_pending_list);
	spin_unlock_bh(&flow_offload_hw_pending_list_lock);

	schedule_work(&nf_flow_offload_hw_work);
}
#endif

static struct flow_offload_hw *
flow_offload_hw_prepare(struct net *net, struct flow_offload *flow)
{
	struct flow_offload_hw_path src = {};
	struct flow_offload_hw_path dest = {};
	struct flow_offload_tuple *tuple_s, *tuple_d;
	struct flow_offload_hw *offload = NULL;

	rcu_read_lock_bh();

	tuple_s = &flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple;
	tuple_d = &flow->tuplehash[FLOW_OFFLOAD_DIR_REPLY].tuple;

	if (flow_offload_check_path(net, tuple_s, tuple_d->dst_cache, &src))
		goto out;

	if (flow_offload_check_path(net, tuple_d, tuple_s->dst_cache, &dest))
		goto out;

	if (!src.dev->netdev_ops->ndo_flow_offload)
		goto out;

	offload = kzalloc(sizeof(struct flow_offload_hw), GFP_ATOMIC);
	if (!offload)
		goto out;

	dev_hold(src.dev);
	dev_hold(dest.dev);
	offload->src = src;
	offload->dest = dest;
	offload->flow = flow;

out:
	rcu_read_unlock_bh();

	return offload;
}

static void flow_offload_hw_add(struct net *net, struct flow_offload *flow,
				struct nf_conn *ct, struct nf_flowtable_count * nf_count)
{
	struct flow_offload_hw *offload;
	int  ret = -1;

	offload = flow_offload_hw_prepare(net, flow);
	if (!offload)
		return;

	nf_conntrack_get(&ct->ct_general);
	offload->type = FLOW_OFFLOAD_ADD;
	offload->ct = ct;
#ifdef NF_FLOW_TABLE_HW_ASYNC
	offload->nf_count = nf_count;
	// not safe if core free flow before dequeue
	flow_offload_queue_work(offload);
#else
	if (nf_ct_is_dying(offload->ct)){
		// printk("ct dyin\n");
		goto err_free;
	}

	// spin_lock_bh(&flow_offload_hw_sync_lock);
	ret = do_flow_offload_hw(offload);
	if ( ret > 0 ){
		atomic_inc(&(nf_count->hw_total_count));
		if(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto == IPPROTO_UDP)
		  atomic_inc(&(nf_count->hw_udp_count));
		else
		  atomic_inc(&(nf_count->hw_tcp_count));
	}

	if ( ret >= 0 )
		offload->flow->flags |= (FLOW_OFFLOAD_HW | FLOW_OFFLOAD_KEEP);

	// spin_unlock_bh(&flow_offload_hw_sync_lock);

err_free:
	flow_offload_hw_free(offload);
	return;
#endif
}

static void flow_offload_hw_del(struct net *net, struct flow_offload *flow, struct nf_flowtable_count * nf_count)
{
	struct flow_offload_hw *offload;

	offload = flow_offload_hw_prepare(net, flow);
	if (!offload)
		return;

	offload->type = FLOW_OFFLOAD_DEL;

// // use index replace flow
// 	offload->flow = (struct flow_offload *)(flow->priv);

#ifdef NF_FLOW_TABLE_HW_ASYNC
// XC ADD
	// not safe if core free flow before dequeue
	flow_offload_queue_work(offload);
#else

	// spin_lock_bh(&flow_offload_hw_sync_lock);
	if(do_flow_offload_hw(offload)  >= 0 ){
		atomic_dec(&(nf_count->hw_total_count));
		if(flow->tuplehash[FLOW_OFFLOAD_DIR_ORIGINAL].tuple.l4proto == IPPROTO_UDP)
		  atomic_dec(&(nf_count->hw_udp_count));
		else
		  atomic_dec(&(nf_count->hw_tcp_count));
	}
	flow->flags &= ~(FLOW_OFFLOAD_KEEP|FLOW_OFFLOAD_HW);
	// spin_unlock_bh(&flow_offload_hw_sync_lock);

	flow_offload_hw_free(offload);
	return;
#endif
}

static const struct nf_flow_table_hw flow_offload_hw = {
	.add	= flow_offload_hw_add,
	.del	= flow_offload_hw_del,
	.owner	= THIS_MODULE,
};

static int __init nf_flow_table_hw_module_init(void)
{

#ifdef NF_FLOW_TABLE_HW_ASYNC
	INIT_WORK(&nf_flow_offload_hw_work, flow_offload_hw_work);
#endif
	nf_flow_table_hw_register(&flow_offload_hw);

	return 0;
}

static void __exit nf_flow_table_hw_module_exit(void)
{
#ifdef NF_FLOW_TABLE_HW_ASYNC
	struct flow_offload_hw *offload, *next;
	LIST_HEAD(hw_offload_pending);
#endif

	nf_flow_table_hw_unregister(&flow_offload_hw);

#ifdef NF_FLOW_TABLE_HW_ASYNC
	cancel_work_sync(&nf_flow_offload_hw_work);

	list_for_each_entry_safe(offload, next, &hw_offload_pending, list)
		flow_offload_hw_free(offload);
#endif
}

module_init(nf_flow_table_hw_module_init);
module_exit(nf_flow_table_hw_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_ALIAS("nf-flow-table-hw");
