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
#include <linux/oom.h>
#include <linux/notifier.h>
#include <linux/list.h>
#include <net/xfrm.h>
#include "siwifi_defs.h"
#include "siwifi_mem.h"
#include "siwifi_debugfs.h"

#define ALLOC_RX_SKB_MAX_NUM 4100
#define ALLOC_RX_SKB_MAX_SIZE (4100 * 5 * 1024)
//#define OOM_DUMP_ALL 1
#define OOM_BUFFER_DUMP_SIZE (4096 * 2)

#define PRIV_SKB_MAGIC_VALUE    0xDEADBEEF

/*Memory chunck size structure*/
struct memory_chunck {
    // list for adding into global list
    struct list_head list;
    // size user want to alloc
    int size;
    // where this memroy allocate
    char where[256];
    // return this point to user
    char priv[0];
};

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

struct siwifi_mem_ctx {
    struct list_head memories;
    int refs;
    int total;
    spinlock_t lock;
    struct notifier_block oom_nb;
    char *oom_buffer;
    void *hw_ctx;

    // The list to store rx free buffer
    struct list_head rx_free_skbs;
    // The list to store rx used buffer
    struct list_head rx_used_skbs;
};

/* Global values*/
struct siwifi_mem_ctx *g_mem = NULL;
#ifdef CONFIG_PRIV_RX_BUFFER_POOL
static DEFINE_SPINLOCK(priv_rx_skbs_lock);
uint8_t g_rx_skb_pool_enable = 1;
static uint32_t g_total_rx_skb = 0;
static uint32_t g_rx_skb_size = 0;
// Rx free buffer cnt
static int g_rx_free_cnt = 0;
// Rx used buffer cnt
static int g_rx_used_cnt = 0;
// external allocate count, if this number too large, that means we should reserve more private rx skbs
static uint32_t g_max_rx_ext_alloc_cnt = 0;
static uint32_t g_rx_ext_alloc_cnt = 0;
static uint32_t g_total_rx_ext_alloc_cnt = 0;
// How many cloned skb rx free function received
static uint32_t g_rx_cloned_skb_cnt = 0;
// How many buffer reallocate during usage
static uint32_t g_rx_reallocate_cnt = 0;
// How man
static uint32_t g_rx_force_release_cnt = 0;

static uint32_t g_rx_chunck_alloc_free_debug = 0;
#endif

#ifdef MEMORY_USAGE_DEBUG
void* siwifi_kmalloc_l(int size, uint32_t flag, char *debug)
{
    struct memory_chunck *chunck;
    chunck = (struct memory_chunck *)kmalloc(size + sizeof(struct memory_chunck), flag);
    if (!chunck)
        return NULL;
    chunck->size = size;
    memset(chunck->where, 0, 256);
    strcpy(chunck->where, debug);
    spin_lock_bh(&g_mem->lock);
    list_add_tail(&chunck->list, &g_mem->memories);
    g_mem->total += chunck->size;
    spin_unlock_bh(&g_mem->lock);
    return (void *)(chunck->priv);
}

void* siwifi_kzalloc_l(int size, uint32_t flag, char *debug)
{
    struct memory_chunck *chunck;
    chunck = (struct memory_chunck *)kzalloc(size + sizeof(struct memory_chunck), flag);
    if (!chunck)
        return NULL;
    chunck->size = size;
    strcpy(chunck->where, debug);
    spin_lock_bh(&g_mem->lock);
    list_add_tail(&chunck->list, &g_mem->memories);
    g_mem->total += chunck->size;
    spin_unlock_bh(&g_mem->lock);
    return (void *)(chunck->priv);
}

void siwifi_kfree(const void *data)
{
    struct memory_chunck *chunck, *chunck1;
    bool found = false;

    spin_lock_bh(&g_mem->lock);
    list_for_each_entry_safe(chunck, chunck1, &g_mem->memories, list)
    {
        if (chunck->priv == data)
        {
            list_del(&chunck->list);
            g_mem->total -= chunck->size;
            kfree((const void *)chunck);
            found = true;
            break;
        }
    }
    spin_unlock_bh(&g_mem->lock);
    if (!found) {
        printk("can not find %p in memory list, still free it\n", data);
        kfree(data);
    }
}
#endif

static int siwifi_oom_notify(struct notifier_block *self,
                                 unsigned long dummy, void *parm)
{
#ifdef OOM_DUMP_ALL
    int idx;
#endif
    printk("siwifi_oom_notify");
    if (!g_mem)
        return NOTIFY_DONE;
    siwifi_print_mem_info();
    if (!g_mem->hw_ctx || !g_mem->oom_buffer)
        return NOTIFY_DONE;

#ifdef OOM_DUMP_ALL
    //dump txq
    siwifi_txq_dump_info((struct siwifi_hw *)g_mem->hw_ctx, g_mem->oom_buffer, OOM_BUFFER_DUMP_SIZE, &idx);
    g_mem->oom_buffer[idx + 1] = 0;
    printk("%s\n", g_mem->oom_buffer);

    siwifi_hwq_dump_info((struct siwifi_hw *)g_mem->hw_ctx, g_mem->oom_buffer, OOM_BUFFER_DUMP_SIZE, &idx);
    g_mem->oom_buffer[idx + 1] = 0;
    printk("%s\n", g_mem->oom_buffer);
#endif

    return NOTIFY_DONE;
}

void siwifi_init_debug_mem(void)
{
    g_mem = (struct siwifi_mem_ctx *)kzalloc(sizeof(struct siwifi_mem_ctx), GFP_KERNEL);
    if (!g_mem) {
        return ;
    }

    INIT_LIST_HEAD(&g_mem->memories);
    INIT_LIST_HEAD(&g_mem->rx_free_skbs);
    INIT_LIST_HEAD(&g_mem->rx_used_skbs);
    spin_lock_init(&g_mem->lock);

#ifdef OOM_DUMP_ALL
    // register oom notify
    g_mem->oom_buffer = (char *)kmalloc(OOM_BUFFER_DUMP_SIZE, GFP_KERNEL);
    if (!g_mem->oom_buffer)
        return;
#else
    g_mem->oom_buffer = NULL;
#endif
    g_mem->oom_nb.notifier_call = siwifi_oom_notify;
    g_mem->oom_nb.priority = 256;
    if (register_oom_notifier(&g_mem->oom_nb) < 0)
        printk("can not register oom notifier\n");
}

void siwifi_set_debug_mem_ctx(void *ctx)
{
    if (!g_mem)
        return;
    g_mem->hw_ctx = ctx;
}


void siwifi_print_mem_info(void)
{
    struct memory_chunck *chunck;
    int i = 0;
    printk("%s\n", __func__);

    spin_lock_bh(&g_mem->lock);
    printk("not free memory %d\n", g_mem->total);
    list_for_each_entry(chunck, &g_mem->memories, list)
    {
        printk("chunck %d, size : %d, where : %s\n", i, chunck->size, chunck->where);
        i++;
    }
    spin_unlock_bh(&g_mem->lock);

    //tx skb buffer info
    if (g_mem->hw_ctx) {
        struct siwifi_vif *vif;
        struct siwifi_hw *hw = (struct siwifi_hw *)(g_mem->hw_ctx);
        list_for_each_entry(vif, &hw->vifs, list) {
            for (i = 0; i < hw->tx_queue_num; i++) {
                printk("vif(%s) queue(%d) tx skb cnt : %d, memory : %d bytes\n",
                    vif->ndev->name, i, vif->lm_ctl[i].tx_cnt, vif->lm_ctl[i].tx_memory_usage);
            }
        }
    }

#ifdef CONFIG_PRIV_RX_BUFFER_POOL
    printk("total allocate private rx buffer : %d %d \n", g_total_rx_skb, g_rx_skb_size);
    printk("private rx free cnt : %d, used cnt : %d ,chunck_alloc_free_debug : %d\n",
            g_rx_free_cnt, g_rx_used_cnt, g_rx_chunck_alloc_free_debug);
    printk("total rx_ext_alloc_cnt : %d, continuous rx ext alloc cnt : %d, rx_cloned_skb_cnt : %d\n",
            g_total_rx_ext_alloc_cnt, g_max_rx_ext_alloc_cnt, g_rx_cloned_skb_cnt);
    printk("g_rx_reallocate_cnt : %d, g_rx_force_release_cnt : %d\n",
            g_rx_reallocate_cnt, g_rx_force_release_cnt);
#endif
}

void siwifi_deinit_debug_mem(void)
{
    printk("%s\n", __func__);
    // before exit, print memory info
    siwifi_print_mem_info();

    // unregister oom notifier
    unregister_oom_notifier(&g_mem->oom_nb);

    // free g_mem context
    if (g_mem->oom_buffer) {
        kfree(g_mem->oom_buffer);
        g_mem->oom_buffer = NULL;
    }

#ifdef CONFIG_PRIV_RX_BUFFER_POOL
    spin_lock_bh(&priv_rx_skbs_lock);
    kfree(g_mem);
    g_mem = NULL;
    spin_unlock_bh(&priv_rx_skbs_lock);
#else
    kfree(g_mem);
    g_mem = NULL;
#endif
}

//--------------------TX & RX buffer -------------------//

#ifdef CONFIG_PRIV_RX_BUFFER_POOL

// magic value first 4 bytes
#define RESET_PRIV_SKB_MAGIC(skb) { \
    skb->vendor_free_priv[0] = 0;   \
    skb->vendor_free_priv[1] = 0;   \
    skb->vendor_free = NULL; \
}
#define SET_PRIV_SKB_MAGIC(skb, chunck) {   \
    skb->vendor_free_priv[0] = PRIV_SKB_MAGIC_VALUE;    \
    skb->vendor_free_priv[1] = (uint32_t)chunck;    \
    skb->vendor_free = siwifi_dev_free_rxskb;    \
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
static bool siwifi_dev_free_rxskb(struct sk_buff *skb, bool force_release)
{
    struct priv_skb_chunck *chunck;
    bool ret = false;

    if (!CHECK_PRIV_SKB_MAGIC(skb)) {
        RESET_PRIV_SKB_MAGIC(skb);
        return false;
    }
    // TODO:
    // There is is a race-risk :
    // when wifi driver is removed, we free the chunck point in siwifi_deinit_private_rxskb
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
        } else if (force_release) {
            g_rx_force_release_cnt ++;
        } else if (skb->head != chunck->skb_head) {
            g_rx_reallocate_cnt ++;
        }

        RESET_PRIV_SKB_MAGIC(skb);
        // allocate new skb to free list
        new_skb = dev_alloc_skb(g_rx_skb_size);
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
    list_add_tail(&chunck->list, &g_mem->rx_free_skbs);
    g_rx_free_cnt ++;
    spin_unlock_bh(&priv_rx_skbs_lock);

    return ret;
}


void siwifi_deinit_private_rxskbs(void)
{
    struct priv_skb_chunck *chunck, *chunck1;

    printk("%s\n", __func__);
    spin_lock_bh(&priv_rx_skbs_lock);

    // just free the chunck in used memory memories list
    list_for_each_entry_safe(chunck, chunck1, &g_mem->rx_used_skbs, list)
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
    list_for_each_entry_safe(chunck, chunck1, &g_mem->rx_free_skbs, list)
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

    INIT_LIST_HEAD(&g_mem->rx_free_skbs);
    INIT_LIST_HEAD(&g_mem->rx_used_skbs);

    // reset the rx statiscs
    g_rx_free_cnt = 0;
    g_rx_used_cnt = 0;

    g_rx_ext_alloc_cnt = 0;
    g_max_rx_ext_alloc_cnt = 0;
    g_total_rx_ext_alloc_cnt = 0;
    g_rx_cloned_skb_cnt = 0;
    g_rx_reallocate_cnt = 0;
    g_rx_force_release_cnt = 0;

    spin_unlock_bh(&priv_rx_skbs_lock);
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
int siwifi_init_private_rxskbs(uint32_t num, uint32_t size)
{
    struct priv_skb_chunck *chunck;
    struct sk_buff *skb;
    int i;

    printk("siwifi_init_private_rxskbs, num %d, size %d\n", num, size);
    if (!list_empty(&g_mem->rx_free_skbs) ||
            !list_empty(&g_mem->rx_used_skbs)) {
        printk("rx free list and used list are not empty when init private skb pools\n");
        return -1;
    }

    for (i = 0; i < num; i++) {
        chunck = (struct priv_skb_chunck *)kzalloc(sizeof(struct priv_skb_chunck), GFP_KERNEL);
        if (!chunck) {
            printk("can not alloc priv_skb_chunck, oom\n");
            goto failed;
        }
        skb = dev_alloc_skb(size);
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
        list_add_tail(&chunck->list, &g_mem->rx_free_skbs);
        g_rx_free_cnt ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
    }
    g_total_rx_skb = num;
    g_rx_skb_size = size;

    return 0;

failed:
    siwifi_deinit_private_rxskbs();
    return -1;
}

struct sk_buff *siwifi_dev_alloc_rxskb(int size)
{
    struct sk_buff *skb;

    // only care about the main path rx buffer allocation
    if (size != g_rx_skb_size) {
        return dev_alloc_skb(size);
    }

    spin_lock_bh(&priv_rx_skbs_lock);
    // if all the free skbs has been consumed , then we need to ask help for system
    if (!g_rx_skb_pool_enable) {
        g_rx_ext_alloc_cnt ++;
        g_total_rx_ext_alloc_cnt ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
        skb = dev_alloc_skb(size);
    } else if (list_empty(&g_mem->rx_free_skbs)) {
        BUG_ON(g_rx_free_cnt != 0);
        g_rx_ext_alloc_cnt ++;
        g_total_rx_ext_alloc_cnt  ++;
        spin_unlock_bh(&priv_rx_skbs_lock);
        skb = dev_alloc_skb(size);
    } else {
        struct priv_skb_chunck *chunck;
        chunck = list_first_entry(&g_mem->rx_free_skbs, struct priv_skb_chunck, list);
        list_del(&chunck->list);
        g_rx_free_cnt --;
        BUG_ON(g_rx_free_cnt < 0);
        skb = chunck->skb;
        list_add_tail(&chunck->list, &g_mem->rx_used_skbs);
        g_rx_used_cnt ++;
        if (g_max_rx_ext_alloc_cnt < g_rx_ext_alloc_cnt)
            g_max_rx_ext_alloc_cnt = g_rx_ext_alloc_cnt;
        g_rx_ext_alloc_cnt = 0;
        spin_unlock_bh(&priv_rx_skbs_lock);
    }
    return skb;
}

#endif
