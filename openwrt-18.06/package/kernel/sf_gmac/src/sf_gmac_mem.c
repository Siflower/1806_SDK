/*
 * =====================================================================================
 *
 *       Filename:  os_memory.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  08/21/2018 07:39:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#include <linux/spinlock.h>
#include <linux/list.h>
#include "sf_gmac_mem.h"

#define PRIV_SKB_MAGIC_VALUE    0xDEADBEEF

struct priv_skb_chunck {
    // list for adding into global list
    struct list_head list;
    struct sk_buff *skb;

    // skb refresh information
    unsigned char       *skb_head;
    unsigned char       *skb_data;
    sk_buff_data_t      skb_tail;
    sk_buff_data_t      skb_end;
    unsigned int        skb_truesize;
    uint8_t             skb_pfmemalloc;
    uint8_t             skb_head_frag;
};


// The list to store rx free buffer
struct list_head g_rx_free_skbs;
// The list to store rx used buffer
struct list_head g_rx_used_skbs;
static DEFINE_SPINLOCK(priv_rx_skbs_lock);
uint8_t g_rx_skb_pool_enable = 1;
static uint32_t g_total_rx_skb = 0;
static uint32_t g_rx_skb_size = 0;
// Rx free buffer cnt
static int g_rx_free_cnt = 0;
// Rx used buffer cnt
static int g_rx_used_cnt = 0;
// external allocate count, if this number too large, that means we should reserve more private rx skbs
static uint32_t g_rx_ext_alloc_cnt = 0;
static uint32_t g_rx_max_ext_alloc_cnt = 0;
// How many cloned skb rx free function received
static uint32_t g_rx_cloned_skb_cnt = 0;
// How many buffer reallocate during usage
static uint32_t g_rx_reallocate_cnt = 0;
// How man
static uint32_t g_rx_force_release_cnt = 0;

static uint32_t g_rx_chunck_alloc_free_debug = 0;
struct net_device *g_priv_dev = NULL;


void sgmac_print_mem_info(void)
{
    printk("total allocate private rx buffer : %d %d \n", g_total_rx_skb, g_rx_skb_size);
    printk("private rx free cnt : %d, used cnt : %d ,chunck_alloc_free_debug : %d\n",
            g_rx_free_cnt, g_rx_used_cnt, g_rx_chunck_alloc_free_debug);
    printk("rx_ext_alloc_cnt : %d, rx_cloned_skb_cnt : %d, g_rx_max_ext_alloc_cnt : %d\n",
            g_rx_ext_alloc_cnt, g_rx_cloned_skb_cnt, g_rx_max_ext_alloc_cnt);
    printk("g_rx_reallocate_cnt : %d, g_rx_force_release_cnt : %d\n",
            g_rx_reallocate_cnt, g_rx_force_release_cnt);
}

//--------------------TX & RX buffer -------------------//


// magic value first 4 bytes
#define RESET_PRIV_SKB_MAGIC(skb) { \
    skb->vendor_free_priv[0] = 0;   \
    skb->vendor_free_priv[1] = 0;   \
    skb->vendor_free = NULL; \
}
#define SET_PRIV_SKB_MAGIC(skb, chunck) {   \
    skb->vendor_free_priv[0] = PRIV_SKB_MAGIC_VALUE;    \
    skb->vendor_free_priv[1] = (uint32_t)chunck;    \
    skb->vendor_free = sgmac_dev_free_rxskb;    \
}

#define CHECK_PRIV_SKB_MAGIC(skb) (skb->vendor_free_priv[0] == PRIV_SKB_MAGIC_VALUE)

#define GET_PRIV_SKB_CHUNCK_POINT(skb)  (skb->vendor_free_priv[1])


static inline void save_skb_info(struct priv_skb_chunck *chunck, struct sk_buff *skb)
{
#define CCS(x) chunck->skb_##x = skb->x
    CCS(tail);
    CCS(end);
    CCS(head);
    CCS(head_frag);
    CCS(data);
    CCS(truesize);
    CCS(pfmemalloc);
#undef CCS
}

static inline void restore_skb_info(struct priv_skb_chunck *chunck, struct sk_buff *skb)
{
#define RCS(x) skb->x = chunck->skb_##x
    RCS(tail);
    RCS(end);
    RCS(head);
    RCS(head_frag);
    RCS(data);
    RCS(truesize);
    RCS(pfmemalloc);
#undef RCS
}

/*skb vendor free callback*/
static bool sgmac_dev_free_rxskb(struct sk_buff *skb, bool force_release)
{
    struct priv_skb_chunck *chunck;
    bool ret = false;

    if (!CHECK_PRIV_SKB_MAGIC(skb)) {
        RESET_PRIV_SKB_MAGIC(skb);
        return false;
    }
    // TODO:
    // There is is a race-risk :
    // when wifi driver is removed, we free the chunck point in sgmac_deinit_private_rxskb
    // But at this time a used skb is during vendor_free callback, then it will be crash
    // We could add lock to protect chunck acess, but that will take lots of cpu resources
    // I dont think it is worth to add lock-protection for this small chance event
    chunck = (struct priv_skb_chunck *)GET_PRIV_SKB_CHUNCK_POINT(skb);
    if (!chunck) {
        RESET_PRIV_SKB_MAGIC(skb);
        return false;
    }
    BUG_ON(chunck->skb != skb);

    // If this skb is cloend, if the skb ref is
    if (unlikely(skb_cloned(skb) ||
            skb->head != chunck->skb_head ||
            force_release ||
            !g_rx_skb_pool_enable)) {
        struct sk_buff *new_skb;
        if (skb_cloned(skb)) {
            g_rx_cloned_skb_cnt ++;
            // printk("this skb was cloned without notify us, build new one!\n");
        } else if (force_release) {
            g_rx_force_release_cnt ++;
            // printk("this skb was forced to be released, build new one\n");
        } else if (skb->head != chunck->skb_head) {
            g_rx_reallocate_cnt ++;
            printk("this skb was reallocated, build new one\n");
        }

        RESET_PRIV_SKB_MAGIC(skb);
        // allocate new skb to free list
        new_skb = netdev_alloc_skb_ip_align(g_priv_dev, g_rx_skb_size);
        if (!new_skb) {
            printk("can not alloc skb(%d), oom\n", g_rx_skb_size);
            spin_lock_bh(&priv_rx_skbs_lock);
            list_del(&chunck->list);
            g_rx_chunck_alloc_free_debug --;
            spin_unlock_bh(&priv_rx_skbs_lock);
            kfree(chunck);
            return false;
        }

        SET_PRIV_SKB_MAGIC(new_skb, chunck);

        chunck->skb = new_skb;
        save_skb_info(chunck, new_skb);
    } else {
        struct skb_shared_info *shinfo;
        int i = 0;

        shinfo = skb_shinfo(skb);
        // clean the frags and frag_list information
        for (i = 0; i < shinfo->nr_frags; i++)
            __skb_frag_unref(&shinfo->frags[i]);
        if (shinfo->frag_list)
            kfree_skb_list(shinfo->frag_list);
        skb_zcopy_clear(skb, true);

        // build the new skb, this will make skb fresh
        memset(skb, 0, offsetof(struct sk_buff, tail));
        refcount_set(&skb->users, 1);
        restore_skb_info(chunck, skb);
        skb->mac_header         = (typeof(skb->mac_header))~0U;
        skb->transport_header   = (typeof(skb->transport_header))~0U;
        /* make sure we initialize shinfo sequentially */
        memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));
        atomic_set(&shinfo->dataref, 1);

        SET_PRIV_SKB_MAGIC(skb, chunck);
        ret = true;
    }

    spin_lock_bh(&priv_rx_skbs_lock);
    list_del(&chunck->list);
    g_rx_used_cnt --;
    BUG_ON(g_rx_used_cnt < 0);
    list_add_tail(&chunck->list, &g_rx_free_skbs);
    g_rx_free_cnt ++;
    spin_unlock_bh(&priv_rx_skbs_lock);

    return ret;
}


void sgmac_deinit_private_rxskbs(void)
{
    struct priv_skb_chunck *chunck, *chunck1;

    printk("%s\n", __func__);

	sgmac_print_mem_info();
    spin_lock_bh(&priv_rx_skbs_lock);

    // just free the chunck in used memory memories list
    list_for_each_entry_safe(chunck, chunck1, &g_rx_used_skbs, list)
    {
        if (chunck->skb) {
            // reset the skb magic value, free skb totally
            RESET_PRIV_SKB_MAGIC(chunck->skb);
        }
        list_del(&chunck->list);
        kfree((const void *)chunck);
        g_rx_chunck_alloc_free_debug --;
    }

    // just free all skbs in free memories list
    list_for_each_entry_safe(chunck, chunck1, &g_rx_free_skbs, list)
    {
        if (chunck->skb) {
            // reset the skb magic value, free skb totally
            RESET_PRIV_SKB_MAGIC(chunck->skb);
            dev_kfree_skb(chunck->skb);
        } else {
            BUG();
        }
        list_del(&chunck->list);
        kfree((const void *)chunck);
        g_rx_chunck_alloc_free_debug --;
    }


    printk("g_rx_free_cnt = %d, rx_used_cnt = %d, chunck_alloc_free = %d\n",
            g_rx_free_cnt, g_rx_used_cnt, g_rx_chunck_alloc_free_debug);

    INIT_LIST_HEAD(&g_rx_free_skbs);
    INIT_LIST_HEAD(&g_rx_used_skbs);

    // reset the rx statiscs
    g_rx_free_cnt = 0;
    g_rx_used_cnt = 0;

    g_rx_ext_alloc_cnt = 0;
    g_rx_max_ext_alloc_cnt = 0;
    g_rx_cloned_skb_cnt = 0;
    g_rx_reallocate_cnt = 0;
    g_rx_force_release_cnt = 0;

    spin_unlock_bh(&priv_rx_skbs_lock);
}

void sgmac_mem_init(void)
{
	// init
	INIT_LIST_HEAD(&g_rx_free_skbs);
	INIT_LIST_HEAD(&g_rx_used_skbs);
}
/*
Func:
    allocate all private rx skb buffer pool, this will take lots of memories
    When load driver ,we allocate all skbs we want, and add them into free list, at the same time we set the skb destructor function,
    When need skbs , we just provide skb from free list, and drag it into used list
    When skb free, we could use skb destructor function to block the skb free into system memory, but recycle the skb by puting it into
    free list

    This could save time for skb allocation and free
params:
    num : How many rx skbs user want us to allocate, if all skbs was used, we will allocate skb from system memory
    size : each skb size

return:
    0 success, otherwise failure
 */
int sgmac_init_private_rxskbs(struct net_device *dev, uint32_t num, uint32_t size)
{
    struct priv_skb_chunck *chunck;
    struct sk_buff *skb;
    int i;
    if (!list_empty(&g_rx_free_skbs) ||
			!list_empty(&g_rx_used_skbs)) {
		printk("net device are opened multiple times????!\n");
		sgmac_deinit_private_rxskbs();
    }

    printk("sgmac_init_private_rxskbs, num %d, size %d\n", num, size);

    for (i = 0; i < num; i++) {
        chunck = (struct priv_skb_chunck *)kzalloc(sizeof(struct priv_skb_chunck), GFP_KERNEL);
        if (!chunck) {
            printk("can not alloc priv_skb_chunck, oom\n");
            goto failed;
        }
        skb = netdev_alloc_skb_ip_align(dev, size);
        if (!skb) {
            printk("can not alloc skb(%d), oom\n", size);
            kfree(chunck);
            goto failed;
        }
        g_rx_chunck_alloc_free_debug ++;

        SET_PRIV_SKB_MAGIC(skb, chunck);
        chunck->skb = skb;
        save_skb_info(chunck, skb);

        // push it into free list
        spin_lock_bh(&priv_rx_skbs_lock);
        list_add_tail(&chunck->list, &g_rx_free_skbs);
        g_rx_free_cnt ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
    }
    g_total_rx_skb = num;
    g_rx_skb_size = size;
	g_priv_dev = dev;

    return 0;

failed:
    sgmac_deinit_private_rxskbs();
    return -1;
}

struct sk_buff *sgmac_dev_alloc_rxskb(int size)
{
    struct sk_buff *skb = NULL;

    // only care about the main path rx buffer allocation
    if (size != g_rx_skb_size) {
        return netdev_alloc_skb_ip_align(g_priv_dev, size);
    }

    spin_lock_bh(&priv_rx_skbs_lock);
    // if all the free skbs has been consumed , then we need to ask help for system
    if (!g_rx_skb_pool_enable) {
        g_rx_ext_alloc_cnt ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
        skb = netdev_alloc_skb_ip_align(g_priv_dev, size);
    } else if (list_empty(&g_rx_free_skbs)) {
        BUG_ON(g_rx_free_cnt != 0);
        g_rx_ext_alloc_cnt ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
#ifdef CONFIG_SFAX8_GMAC_RX_BUFFER_POOL_LIMIT
		return NULL;
#else
        skb = netdev_alloc_skb_ip_align(g_priv_dev, size);
#endif
    } else {
        struct priv_skb_chunck *chunck;
        chunck = list_first_entry(&g_rx_free_skbs, struct priv_skb_chunck, list);
        list_del(&chunck->list);
        g_rx_free_cnt --;
        BUG_ON(g_rx_free_cnt < 0);
        skb = chunck->skb;
        list_add_tail(&chunck->list, &g_rx_used_skbs);
        g_rx_used_cnt ++;
		if (g_rx_ext_alloc_cnt > g_rx_max_ext_alloc_cnt)
			g_rx_max_ext_alloc_cnt = g_rx_ext_alloc_cnt;
		g_rx_ext_alloc_cnt = 0;
        spin_unlock_bh(&priv_rx_skbs_lock);
    }
    return skb;
}
