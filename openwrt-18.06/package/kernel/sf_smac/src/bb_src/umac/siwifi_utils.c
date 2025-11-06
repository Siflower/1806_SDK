/**
 * siwifi_utils.c
 *
 * IPC utility function definitions
 *
 * Copyright (C) Siflower 2018-2025
 */
#include <net/mac80211.h>
#include "siwifi_utils.h"
#include "siwifi_defs.h"
#include "siwifi_rx.h"
#include "siwifi_tx.h"
#include "siwifi_msg_rx.h"
#include "siwifi_msg_tx.h"
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
#include "siwifi_debugfs.h"
#endif
#include "siwifi_prof.h"
#include "ipc_host.h"
#include "siwifi_mem.h"

extern void siwifi_reprobe_driver(struct siwifi_hw *siwifi_hw);

/**
 * siwifi_ipc_elem_pool_allocs() - Allocate and push to fw a pool of buffer.
 *
 * @siwifi_hw: Main driver structure
 * @pool: Pool to allocate
 * @nb: Size of the pool to allocate
 * @elem_size: SIze of one pool element
 * @pool_name: Name of the pool
 * @push: Function to push one pool element to fw
 *
 * This function will allocate an array to store the list of element addresses,
 * a dma pool and @nb element in the dma pool.
 * Each element is set with '0' and then push to fw using the @push function.
 * It assumes that pointer inside @ipc parameter are set to NULL at start.
 *
 * Return: 0 on success and <0 upon error. If error is returned any allocated
 * memory is NOT freed and siwifi_ipc_elem_pool_deallocs() must be called.
 */
static int siwifi_ipc_elem_pool_allocs(struct siwifi_hw *siwifi_hw,
                                     struct siwifi_ipc_elem_pool *pool,
                                     int nb, size_t elem_size, char *pool_name,
                                     int (*push)(struct ipc_host_env_tag *,
                                                 void *, uint32_t))
{
    struct siwifi_ipc_elem *buf;
    int i;

    pool->nb = 0;

    /* allocate buf array */
    pool->buf = siwifi_kmalloc(nb * sizeof(struct siwifi_ipc_elem), GFP_KERNEL);
    if (!pool->buf) {
        dev_err(siwifi_hw->dev, "Allocation of buffer array for %s failed\n",
                pool_name);
        return -ENOMEM;
    }

    /* allocate dma pool */
    pool->pool = dma_pool_create(pool_name, siwifi_hw->dev, elem_size,
                                 cache_line_size(), 0);
    if (!pool->pool) {
        dev_err(siwifi_hw->dev, "Allocation of dma pool %s failed\n",
                pool_name);
        return -ENOMEM;
    }

    for (i = 0, buf = pool->buf; i < nb; buf++, i++) {

        /* allocate an elem */
        buf->addr = dma_pool_alloc(pool->pool, GFP_KERNEL, &buf->dma_addr);
        if (!buf->addr) {
            dev_err(siwifi_hw->dev, "Allocation of block %d/%d in %s failed\n",
                    (i + 1), nb, pool_name);
            return -ENOMEM;
        }
        pool->nb++;

        /* reset the element */
        memset(buf->addr, 0, elem_size);

        /* push it to FW */
        if (push) push(siwifi_hw->ipc_env, buf, (uint32_t)buf->dma_addr);
    }

    return 0;
}

/**
 * siwifi_ipc_elem_pool_deallocs() - Free all memory allocated for a pool
 *
 * @pool: Pool to free
 *
 * Must be call once after siwifi_ipc_elem_pool_allocs(), even if it returned
 * an error
 */
static void siwifi_ipc_elem_pool_deallocs(struct siwifi_ipc_elem_pool *pool)
{
    struct siwifi_ipc_elem *buf;
    int i;

    for (i = 0, buf = pool->buf; i < pool->nb ; buf++, i++) {
        dma_pool_free(pool->pool, buf->addr, buf->dma_addr);
    }
    pool->nb = 0;

    if (pool->pool)
        dma_pool_destroy(pool->pool);
    pool->pool = NULL;

    if (pool->buf)
        siwifi_kfree(pool->buf);
    pool->buf = NULL;
}

/**
 * siwifi_ipc_elem_var_allocs - Alloc a single ipc buffer and push it to fw
 *
 * @siwifi_hw: Main driver structure
 * @elem: Element to allocate
 * @elem_size: Size of the element to allcoate
 * @dir: DMA direction
 * @buf: If not NULL, used this buffer instead of allocating a new one. It must
 * be @elem_size long and be allocated by siwifi_kmalloc as siwifi_kfree will be called.
 * @init: Pointer to initial data to write in buffer before DMA sync. Needed
 * only if direction is DMA_TO_DEVICE. If set it is assume that its size is
 * @elem_size.
 * @push: Function to push the element to fw. May be set to NULL.
 * @real_size: If bu is not NULL,indicates the real buffer alloc address
 *
 * It allocates a buffer (or use the one provided with @buf), initializes it if
 * @init is set, map buffer for DMA transfer, initializes @elem and push buffer
 * to FW if @push is seet.
 *
 * Return: 0 on success and <0 upon error. If error is returned any allocated
 * memory has been freed (including @buf if set).
 */
int siwifi_ipc_elem_var_allocs(struct siwifi_hw *siwifi_hw,
                             struct siwifi_ipc_elem_var *elem, size_t elem_size,
                             enum dma_data_direction dir,
                             void *buf, const void *init,
                             void (*push)(struct ipc_host_env_tag *, uint32_t),
                             void *real_addr)
{
    if (buf) {
        elem->addr = buf;
        elem->real_addr = real_addr;
    } else {
        elem->real_size = elem_size + 2 * cache_line_size();
        elem->real_addr = siwifi_kmalloc(elem->real_size, GFP_KERNEL);
        elem->addr = (void*)((uint32_t)elem->real_addr + cache_line_size());
        if (!elem->real_addr) {
            dev_err(siwifi_hw->dev, "Allocation of ipc buffer failed size=%d\n", elem_size);
            return -ENOMEM;
        }
    }
    elem->size = elem_size;

    if ((dir == DMA_TO_DEVICE) && init) {
        memcpy(elem->addr, init, elem_size);
    }

    elem->dma_addr = dma_map_single(siwifi_hw->dev, elem->addr, elem_size, dir);
    if (dma_mapping_error(siwifi_hw->dev, elem->dma_addr)) {
        dev_err(siwifi_hw->dev, "DMA mapping failed\n");
        siwifi_kfree(elem->real_addr);
        elem->addr = NULL;
        elem->real_addr = NULL;
        return -EIO;
    }

    if (push)
        push(siwifi_hw->ipc_env, elem->dma_addr);
    return 0;
}

/**
 * siwifi_ipc_elem_var_deallocs() - Free memory allocated for a single ipc buffer
 *
 * @siwifi_hw: Main driver structure
 * @elem: Element to free
 */
void siwifi_ipc_elem_var_deallocs(struct siwifi_hw *siwifi_hw,
                                struct siwifi_ipc_elem_var *elem)
{
    if (!elem->real_addr)
        return;
    dma_unmap_single(siwifi_hw->dev, elem->dma_addr, elem->size, DMA_TO_DEVICE);
    siwifi_kfree(elem->real_addr);
    elem->addr = NULL;
    elem->real_addr = NULL;
}

/**
 * siwifi_ipc_skb_elem_allocs() - Allocate and push a skb buffer for the FW
 *
 * @siwifi_hw: Main driver data
 * @elem: Pointer to the skb elem that will contain the address of the buffer
 */
int siwifi_ipc_skb_elem_allocs(struct siwifi_hw *siwifi_hw,
                                 struct siwifi_ipc_skb_elem *elem, size_t skb_size,
                                 enum dma_data_direction dir,
                                 int (*push)(struct ipc_host_env_tag *,
                                             void *, uint32_t))
{
/*
ensure  __netdev_alloc_skb_from_pool would be used in single thread  in siwifi_ipc_rxbuf_elem_allocs
comment here
*/
//#ifdef CONFIG_SF_SKB_POOL
//    elem->skb = __netdev_alloc_skb_from_pool(NULL, skb_size,GFP_ATOMIC,siwifi_hw->skb_pool_dev_param);
//#else
    elem->skb = siwifi_dev_alloc_rxskb(skb_size);
//#endif

    if (unlikely(!elem->skb)) {
        dev_err(siwifi_hw->dev, "Allocation of ipc skb failed\n");
        return -ENOMEM;
    }

    elem->dma_addr = dma_map_single(siwifi_hw->dev, elem->skb->data, skb_size, dir);
    if (unlikely(dma_mapping_error(siwifi_hw->dev, elem->dma_addr))) {
        dev_err(siwifi_hw->dev, "DMA mapping failed\n");
        dev_kfree_skb(elem->skb);
        elem->skb = NULL;
        return -EIO;
    }

    if (push){
        push(siwifi_hw->ipc_env, elem, elem->dma_addr);
    }
    return 0;
}

/**
 * siwifi_ipc_skb_elem_deallocs() - Free a skb buffer allocated for the FW
 *
 * @siwifi_hw: Main driver data
 * @elem: Pointer to the skb elem that contains the address of the buffer
 * @skb_size: size of the skb buffer data
 * @dir: DMA direction
 */
static void siwifi_ipc_skb_elem_deallocs(struct siwifi_hw *siwifi_hw,
                                       struct siwifi_ipc_skb_elem *elem,
                                       size_t skb_size, enum dma_data_direction dir) {
    if (elem->skb) {
        dma_unmap_single(siwifi_hw->dev, elem->dma_addr, skb_size, dir);
        dev_kfree_skb(elem->skb);
        elem->skb = NULL;
    }
}

/**
 * siwifi_ipc_unsup_rx_vec_elem_allocs() - Allocate and push an unsupported
 *                                       RX vector buffer for the FW
 *
 * @siwifi_hw: Main driver data
 * @elem: Pointer to the skb elem that will contain the address of the buffer
 */
int siwifi_ipc_unsup_rx_vec_elem_allocs(struct siwifi_hw *siwifi_hw,
                                      struct siwifi_ipc_skb_elem *elem)
{
    struct rx_vector_desc *rxdesc;

    if (siwifi_ipc_skb_elem_allocs(siwifi_hw, elem,
            siwifi_hw->ipc_env->unsuprxvec_bufsz, DMA_FROM_DEVICE, NULL))
        return -ENOMEM;

    rxdesc = (struct rx_vector_desc *) elem->skb->data;
    rxdesc->pattern = 0;
    dma_sync_single_for_device(siwifi_hw->dev,
                        elem->dma_addr + offsetof(struct rx_vector_desc, pattern),
                        sizeof(rxdesc->pattern), DMA_BIDIRECTIONAL);

    ipc_host_unsup_rx_vec_buf_push(siwifi_hw->ipc_env, elem, (u32) elem->dma_addr);

    return 0;
}

/**
 * siwifi_ipc_rxbuf_elems_deallocs() - Free all unsupported rx vector buffer
 *                                   allocated for the FW
 *
 * @siwifi_hw: Main driver data
 */
static void siwifi_ipc_unsup_rx_vec_elems_deallocs(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_ipc_skb_elem *elem;
    int i, nb = siwifi_hw->ipc_env->unsuprxvec_bufnb;

    if (!siwifi_hw->e2aunsuprxvec_elems)
        return;

    for (i = 0, elem = siwifi_hw->e2aunsuprxvec_elems; i < nb; i++, elem++) {
        siwifi_ipc_skb_elem_deallocs(siwifi_hw, elem, siwifi_hw->ipc_env->unsuprxvec_bufsz, DMA_FROM_DEVICE);
    }

    siwifi_kfree(siwifi_hw->e2aunsuprxvec_elems);
    siwifi_hw->e2aunsuprxvec_elems = NULL;
}

/**
* siwifi_ipc_unsup_rx_vec_elems_allocs() - Allocate and push all unsupported RX
*                                        vector buffer for the FW
*
* @siwifi_hw: Main driver data
*/
static int siwifi_ipc_unsup_rx_vec_elems_allocs(struct siwifi_hw *siwifi_hw)
{
   struct siwifi_ipc_skb_elem *elem;
   int i, nb = siwifi_hw->ipc_env->unsuprxvec_bufnb;

   siwifi_hw->e2aunsuprxvec_elems = siwifi_kzalloc(nb * sizeof(struct siwifi_ipc_skb_elem),
                                   GFP_KERNEL);
   if (!siwifi_hw->e2aunsuprxvec_elems) {
       dev_err(siwifi_hw->dev, "Failed to allocate unsuprxvec_elems\n");
       return -ENOMEM;
   }

   for (i = 0, elem = siwifi_hw->e2aunsuprxvec_elems; i < nb; i++, elem++)
   {
       if (siwifi_ipc_unsup_rx_vec_elem_allocs(siwifi_hw, elem)) {
           dev_err(siwifi_hw->dev, "Failed to allocate unsuprxvec buf %d/%d\n",
                   i + 1, nb);
           return -ENOMEM;
       }
   }

   return 0;
}

#ifdef CONFIG_ERROR_DUMP
int siwifi_errorinfo_allocs(struct siwifi_hw *siwifi_hw)
{
    struct error_info *info[ERROR_INFO_MAX_NUM];
    int i,ret;
	ret = 0;
    /* Allocate the assert information buffer */
	for (i = 0; i < ERROR_INFO_MAX_NUM; i++){
		info[i] = siwifi_kzalloc(sizeof(struct error_info),GFP_KERNEL);
		if (!info[i]) {
			printk(KERN_CRIT "%s:%d: error_info alloc of size %u failed\n\n",
               __func__, __LINE__, sizeof(struct error_info));
			ret = -ENOMEM;
			goto ERROR;
		}
		siwifi_hw->dbgdump_elem.errorinfo[i] = info[i];
		siwifi_hw->dbgdump_elem.errorinfo[i]->count = 0;
		siwifi_hw->dbgdump_elem.errorinfo[i]->buf = NULL;
	}
	siwifi_hw->dbgdump_elem.err_total_count = 0;
	siwifi_hw->dbgdump_elem.err_cur_idx = 0;
	spin_lock_init(&siwifi_hw->dbgdump_elem.err_lock);
	return 0;
ERROR:
    for(i = 0; i < ERROR_INFO_MAX_NUM; i++) {
		if(info[i])
			siwifi_kfree(info[i]);
	}
    return ret;
}

void siwifi_errorinfo_deallocs(struct siwifi_hw *siwifi_hw)
{
	int i;
	printk("siwifi_errorinfo_deallocs\n");
	for(i = 0; i < ERROR_INFO_MAX_NUM; i++){
		if (siwifi_hw->dbgdump_elem.errorinfo[i]) {
			if(siwifi_hw->dbgdump_elem.errorinfo[i]->buf){
				siwifi_kfree(siwifi_hw->dbgdump_elem.errorinfo[i]->buf);
				siwifi_hw->dbgdump_elem.errorinfo[i]->buf = NULL;
			}
			siwifi_kfree(siwifi_hw->dbgdump_elem.errorinfo[i]);
			siwifi_hw->dbgdump_elem.errorinfo[i] = NULL;
		}
	}
}
#endif

/**
 * siwifi_ipc_rxdesc_elem_repush() - Repush a rxdesc to FW
 *
 * @siwifi_hw: Main driver data
 * @elem: Rx desc to repush
 *
 * Once rx buffer has been received, the rxdesc used by FW to upload this
 * buffer can be re-used for another rx buffer.
 */
void siwifi_ipc_rxdesc_elem_repush(struct siwifi_hw *siwifi_hw,
                                 struct siwifi_ipc_elem *elem)
{
    struct rxdesc_tag *rxdesc = elem->addr;
    rxdesc->status = 0;
    // The address of dma_pool_alloc does not need to be flush cache
    //dma_sync_single_for_device(siwifi_hw->dev, elem->dma_addr,
    //                           sizeof(struct rxdesc_tag), DMA_BIDIRECTIONAL);
    ipc_host_rxdesc_push(siwifi_hw->ipc_env, elem, (u32)elem->dma_addr);
}

/**
 * siwifi_ipc_rxbuf_elem_allocs() - Allocate and push a RX buffer for the FW
 *
 * @siwifi_hw: Main driver data
 */
#ifdef DEBUG_CONFIG_PRIV_RX_BUFFER_POOL
uint32_t g_debug_alloc_time_cnt = 0;
u64 g_debug_alloc_start, g_debug_alloc_stop, g_debug_alloc_time_sum;
#endif

int siwifi_ipc_rxbuf_elem_allocs(struct siwifi_hw *siwifi_hw)
{
    struct sk_buff *skb;
    struct hw_rxhdr *hw_rxhdr;
    dma_addr_t dma_addr;
    int size = siwifi_hw->ipc_env->rx_bufsz;
    int nb, idx;

#ifdef DEBUG_CONFIG_PRIV_RX_BUFFER_POOL
    g_debug_alloc_start = local_clock();
#endif

#ifdef CONFIG_SF_SKB_POOL
    skb = __netdev_alloc_skb_from_pool(NULL, size, GFP_ATOMIC, siwifi_hw->skb_pool_dev_param);
#else
    unsigned long long free_mem;
    free_mem = global_zone_page_state(NR_FREE_PAGES) << (PAGE_SHIFT -10);
    if (free_mem < SIWIFI_RX_LOW_MEM_SIZE) {
        siwifi_hw->rx_skb_alloc_fail_cnt++;
        return 1;
    }
    skb = siwifi_dev_alloc_rxskb(size);
#endif  //

    if (unlikely(!skb)) {
        dev_err(siwifi_hw->dev, "Failed to allocate rx buffer\n");
        siwifi_hw->rx_skb_alloc_fail_cnt++;
        return -ENOMEM;
    }
#ifdef DEBUG_CONFIG_PRIV_RX_BUFFER_POOL
    g_debug_alloc_stop = local_clock();
    g_debug_alloc_time_sum += (g_debug_alloc_stop - g_debug_alloc_start);
    g_debug_alloc_time_cnt ++;
    if (g_debug_alloc_time_cnt == 10000) {
        printk("10000 alloc take time : %lld\n", g_debug_alloc_time_sum);
        g_debug_alloc_time_cnt = 0;
        g_debug_alloc_time_sum = 0;
    }
#endif

    dma_addr = dma_map_single(siwifi_hw->dev, skb->data, size, DMA_FROM_DEVICE);

    if (unlikely(dma_mapping_error(siwifi_hw->dev, dma_addr))) {
        dev_err(siwifi_hw->dev, "Failed to map rx buffer\n");
        siwifi_hw->rx_skb_alloc_fail_cnt++;
        goto err_skb;
    }

    hw_rxhdr = (struct hw_rxhdr *)skb->data;
    hw_rxhdr->pattern = 0;
    dma_sync_single_for_device(siwifi_hw->dev,
                               dma_addr + offsetof(struct hw_rxhdr, pattern),
                               sizeof(hw_rxhdr->pattern), DMA_BIDIRECTIONAL);

    /* Find first free slot */
    nb = 0;
    idx = siwifi_hw->rxbuf_elems.idx;
#if DEBUG_ARRAY_CHECK
    BUG_ON(idx >= SIWIFI_RXBUFF_MAX);
#endif
    while (siwifi_hw->rxbuf_elems.skb[idx] && nb < SIWIFI_RXBUFF_MAX) {
        idx = ( idx + 1 ) % SIWIFI_RXBUFF_MAX;
        nb++;
    }

    if (nb == SIWIFI_RXBUFF_MAX)
        printk("siwifi_hw->reord_alloc_cnt = %d, siwifi_hw->reord_release_cnt = %d\n", siwifi_hw->reord_alloc_cnt, siwifi_hw->reord_release_cnt);

    if (WARN((nb == SIWIFI_RXBUFF_MAX), "No more free space for rxbuff")) {
        siwifi_hw->rx_skb_alloc_fail_cnt++;
        goto err_dma;
    }

    siwifi_hw->rxbuf_elems.skb[idx] = skb;

    /* Save info in skb control buffer  */
    SIWIFI_RXBUFF_DMA_ADDR_SET(skb, dma_addr);
    SIWIFI_RXBUFF_PATTERN_SET(skb, siwifi_rxbuff_pattern);
    SIWIFI_RXBUFF_IDX_SET(skb, idx);


    /* Push buffer to FW */
    ipc_host_rxbuf_push(siwifi_hw->ipc_env, SIWIFI_RXBUFF_IDX_TO_HOSTID(idx),
                        dma_addr);

    /* Save idx so that on next push the free slot will be found quicker */
    siwifi_hw->rxbuf_elems.idx = ( idx + 1 ) % SIWIFI_RXBUFF_MAX;

    return 0;

  err_dma:
    dma_unmap_single(siwifi_hw->dev, dma_addr, size, DMA_FROM_DEVICE);
  err_skb:
    dev_kfree_skb(skb);
    return -ENOMEM;
}

/**
 * siwifi_ipc_rxbuf_elem_repush() - Repush a rxbuf to FW
 *
 * @siwifi_hw: Main driver data
 * @skb: Skb to repush
 *
 * In case a skb is not forwarded to upper layer it can be re-used.
 * It is assumed that @skb has been verified before calling this function and
 * that it is a valid rx buffer
 * (i.e. skb == siwifi_hw->rxbuf_elems.skb[SIWIFI_RXBUFF_IDX_GET(skb)])
 */
void siwifi_ipc_rxbuf_elem_repush(struct siwifi_hw *siwifi_hw,
                                struct sk_buff *skb)
{
    dma_addr_t dma_addr;
    struct hw_rxhdr *hw_rxhdr = (struct hw_rxhdr *)skb->data;
    int idx;

    /* reset pattern */
    hw_rxhdr->pattern = 0;
    dma_addr = SIWIFI_RXBUFF_DMA_ADDR_GET(skb);
    dma_sync_single_for_device(siwifi_hw->dev,
                               dma_addr + offsetof(struct hw_rxhdr, pattern),
                               sizeof(hw_rxhdr->pattern), DMA_BIDIRECTIONAL);

    /* re-push buffer to FW */
    idx = SIWIFI_RXBUFF_IDX_GET(skb);
    ipc_host_rxbuf_push(siwifi_hw->ipc_env, SIWIFI_RXBUFF_IDX_TO_HOSTID(idx),
                        dma_addr);
}

/**
 * siwifi_ipc_rxbuf_elems_allocs() - Allocate and push all RX buffer for the FW
 *
 * @siwifi_hw: Main driver data
 */
static int siwifi_ipc_rxbuf_elems_allocs(struct siwifi_hw *siwifi_hw)
{
    int i, nb = siwifi_hw->ipc_env->rx_bufnb;

    for (i = 0; i < SIWIFI_RXBUFF_MAX; i++) {
        siwifi_hw->rxbuf_elems.skb[i] = NULL;
    }
    siwifi_hw->rxbuf_elems.idx = 0;

    for (i = 0; i < nb; i++) {
        if (siwifi_ipc_rxbuf_elem_allocs(siwifi_hw)) {
            //dev_err(siwifi_hw->dev, "Failed to allocate rx buf %d/%d\n",
            //        i + 1, nb);
            return -ENOMEM;
        }
    }

    siwifi_hw->rx_skb_alloc_fail_cnt = 0;
    return 0;
}

/**
 * siwifi_ipc_rxbuf_elems_deallocs() - Free all RX buffer allocated for the FW
 *
 * @siwifi_hw: Main driver data
 */
static void siwifi_ipc_rxbuf_elems_deallocs(struct siwifi_hw *siwifi_hw)
{
    struct sk_buff *skb;
    int i;

    for (i = 0; i < SIWIFI_RXBUFF_MAX; i++) {
        if (siwifi_hw->rxbuf_elems.skb[i]) {
            skb = siwifi_hw->rxbuf_elems.skb[i];
            dma_unmap_single(siwifi_hw->dev, SIWIFI_RXBUFF_DMA_ADDR_GET(skb),
                             siwifi_hw->ipc_env->rx_bufsz, DMA_FROM_DEVICE);
            dev_kfree_skb(skb);
            siwifi_hw->rxbuf_elems.skb[i] = NULL;
        }
    }
}

/**
 * siwifi_ipc_rxbuf_elem_pull() - Extract a skb from local table
 *
 * @siwifi_hw: Main driver data
 * @skb: SKb to extract for table
 *
 * After checking that skb is actually a pointer of local table, extract it
 * from the table.
 * When buffer is removed, DMA mapping is remove which has the effect to
 * synchronize the buffer for the cpu.
 * To be called before passing skb to upper layer.
 */
void siwifi_ipc_rxbuf_elem_pull_inv(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, bool inv)
{
    unsigned int idx = SIWIFI_RXBUFF_IDX_GET(skb);

    if (SIWIFI_RXBUFF_VALID_IDX(idx) && (siwifi_hw->rxbuf_elems.skb[idx] == skb)) {
        dma_addr_t dma_addr = SIWIFI_RXBUFF_DMA_ADDR_GET(skb);
        siwifi_hw->rxbuf_elems.skb[idx] = NULL;
        dma_unmap_single(siwifi_hw->dev, dma_addr,
                         siwifi_hw->ipc_env->rx_bufsz, DMA_FROM_DEVICE);
        if (inv) {
            dma_sync_single_for_device(siwifi_hw->dev, dma_addr,
                                   siwifi_hw->ipc_env->rx_bufsz, DMA_FROM_DEVICE);
        }
    } else {
        WARN(1, "Incorrect rxbuff idx skb=%p table[%u]=%p", skb, idx,
             idx < SIWIFI_RXBUFF_MAX ? siwifi_hw->rxbuf_elems.skb[idx] : NULL);
    }

    /* Reset the pattern and idx */
    SIWIFI_RXBUFF_PATTERN_SET(skb, 0);
    SIWIFI_RXBUFF_IDX_SET(skb, SIWIFI_RXBUFF_MAX);
}

void siwifi_ipc_rxbuf_elem_pull(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    siwifi_ipc_rxbuf_elem_pull_inv(siwifi_hw, skb, false);
}

/**
 * siwifi_ipc_rxbuf_elem_sync() - Sync part of a RX buffer
 *
 * @siwifi_hw: Main driver data
 * @skb: SKb to sync
 * @len: Len to sync
 *
 * After checking that skb is actually a pointer of local table, sync @p len
 * bytes of the buffer for CPU. Buffer is not removed from the table
 */
void siwifi_ipc_rxbuf_elem_sync(struct siwifi_hw *siwifi_hw, struct sk_buff *skb,
                              int len)
{
    unsigned int idx = SIWIFI_RXBUFF_IDX_GET(skb);

    if (SIWIFI_RXBUFF_VALID_IDX(idx) && (siwifi_hw->rxbuf_elems.skb[idx] == skb)) {
        dma_addr_t dma_addr = SIWIFI_RXBUFF_DMA_ADDR_GET(skb);
        dma_sync_single_for_cpu(siwifi_hw->dev, dma_addr, len, DMA_FROM_DEVICE);
    } else {
        WARN(1, "Incorrect rxbuff idx skb=%p table[%u]=%p", skb, idx,
             idx < SIWIFI_RXBUFF_MAX ? siwifi_hw->rxbuf_elems.skb[idx] : NULL);
    }
}

#ifdef NEW_SCHEDULE
#define SIWIFI_IPC_A2ETXDESC_POOL(a, b) "#siwifi_ipc_a2erxdesc_pool##a_##b#"
static void siwifi_ipc_txdesc_deallocs(struct siwifi_hw *siwifi_hw)
{
    int i, j;
    for (i = 0; i < CONFIG_USER_MAX; i++) {
        for (j = 0; j < NX_TXQ_CNT; j++) {
            siwifi_ipc_elem_pool_deallocs(&siwifi_hw->a2etxdesc_pool[i][j]);
        }
    }
}

static int siwifi_ipc_txdesc_allocs(struct siwifi_hw *siwifi_hw)
{
    int i, j, k;
    struct siwifi_ipc_elem_pool *pool;

    for (i = 0; i < CONFIG_USER_MAX; i++) {
        for (j = 0; j < NX_TXQ_CNT; j++) {
            pool = &siwifi_hw->a2etxdesc_pool[i][j];
            if (siwifi_ipc_elem_pool_allocs(siwifi_hw, pool,
                    nx_txdesc_cnt[j],
                    sizeof(struct txdesc_api) * MAX_SKBS_PER_TOKEN,
                    SIWIFI_IPC_A2ETXDESC_POOL(i,j),
                    NULL)) {
                printk("can not allocate txdesc\n");
                return -1;
            }
            for (k = 0; k < nx_txdesc_cnt[j]; k++) {
                ipc_host_txdesc_map(siwifi_hw->ipc_env, i, j, k, pool->buf[k].dma_addr);
            }
        }
    }
    return 0;
}
#endif

/**
 * siwifi_elems_deallocs() - Deallocate IPC storage elements.
 * @siwifi_hw: Main driver data
 *
 * This function deallocates all the elements required for communications with
 * LMAC, such as Rx Data elements, MSGs elements, ...
 * This function should be called in correspondence with the allocation function.
 */
static void siwifi_elems_deallocs(struct siwifi_hw *siwifi_hw)
{
    siwifi_ipc_rxbuf_elems_deallocs(siwifi_hw);
    siwifi_ipc_unsup_rx_vec_elems_deallocs(siwifi_hw);
#ifdef NEW_SCHEDULE
    siwifi_ipc_txdesc_deallocs(siwifi_hw);
#endif
    siwifi_ipc_elem_pool_deallocs(&siwifi_hw->e2arxdesc_pool);
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->dbg_mgmt_info_elem);
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->dbg_ctrl_info_elem);
    siwifi_ipc_elem_pool_deallocs(&siwifi_hw->e2amsgs_pool);
    siwifi_ipc_elem_pool_deallocs(&siwifi_hw->dbgmsgs_pool);
#ifdef CONFIG_SIWIFI_RADAR
    siwifi_ipc_elem_pool_deallocs(&siwifi_hw->e2aradars_pool);
#endif
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->pattern_elem);
#ifdef CFG_DBGDUMP
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->dbgdump_elem.buf);
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
	if(siwifi_hw->dbgdump_elem.la_mem_addr)
		dma_unmap_single(siwifi_hw->dev, (dma_addr_t)siwifi_hw->dbgdump_elem.la_mem_addr, siwifi_hw->dbgdump_elem.la_len, DMA_TO_DEVICE);
    siwifi_hw->dbgdump_elem.la_mem_addr = NULL;
#endif /* CONFIG_SF16A18_WIFI_LA_ENABLE */
#endif /* CFG_DBGDUMP*/
#ifdef CONFIG_SIWIFI_TRX_STAT
    siwifi_ipc_elem_var_deallocs(siwifi_hw, &siwifi_hw->dbg_trx_stat_elem);
#endif
}

/**
 * siwifi_elems_allocs() - Allocate IPC storage elements.
 * @siwifi_hw: Main driver data
 *
 * This function allocates all the elements required for communications with
 * LMAC, such as Rx Data elements, MSGs elements, ...
 * This function should be called in correspondence with the deallocation function.
 */
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
	u32_l g_la_mem[LA_MEM_LEN/4];
#endif /* CONFIG_SF16A18_WIFI_LA_ENABLE */
static int siwifi_elems_allocs(struct siwifi_hw *siwifi_hw)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (dma_set_coherent_mask(siwifi_hw->dev, DMA_BIT_MASK(32)))
        goto err_alloc;

    if (siwifi_ipc_elem_pool_allocs(siwifi_hw, &siwifi_hw->e2amsgs_pool,
                                  siwifi_hw->ipc_env->ipc_e2amsg_bufnb,
                                  siwifi_hw->ipc_env->ipc_e2amsg_bufsz,
                                  "siwifi_ipc_e2amsgs_pool",
                                  ipc_host_msgbuf_push))
        goto err_alloc;

    if (siwifi_ipc_elem_pool_allocs(siwifi_hw, &siwifi_hw->dbgmsgs_pool,
                                  siwifi_hw->ipc_env->ipc_dbg_bufnb,
                                  siwifi_hw->ipc_env->ipc_dbg_bufsz,
                                  "siwifi_ipc_dbgmsgs_pool",
                                  ipc_host_dbgbuf_push))
        goto err_alloc;

#ifdef CONFIG_SIWIFI_RADAR
    if (siwifi_ipc_elem_pool_allocs(siwifi_hw, &siwifi_hw->e2aradars_pool,
                                  siwifi_hw->ipc_env->radar_bufnb,
                                  siwifi_hw->ipc_env->radar_bufsz,
                                  "siwifi_ipc_e2aradars_pool",
                                  ipc_host_radarbuf_push))
        goto err_alloc;
#endif

    if (siwifi_ipc_unsup_rx_vec_elems_allocs(siwifi_hw))
        goto err_alloc;

    if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->pattern_elem,
                                 sizeof(u32), DMA_TO_DEVICE,
                                 NULL, &siwifi_rxbuff_pattern,
                                 ipc_host_patt_addr_push, NULL))
        goto err_alloc;

#ifdef CFG_DBGDUMP
    if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->dbgdump_elem.buf,
                                 sizeof(struct dbg_debug_dump_tag),
                                 DMA_FROM_DEVICE, NULL, NULL,
                                 ipc_host_dbginfobuf_push, NULL))
	{
        goto err_alloc;
	}
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
	else
	{
        struct dbg_debug_dump_tag *dbg_debug_dump;
		dbg_debug_dump = (struct dbg_debug_dump_tag *)(siwifi_hw->dbgdump_elem.buf.addr);
		siwifi_hw->dbgdump_elem.la_mem_addr = (void *)&g_la_mem;
		siwifi_hw->dbgdump_elem.la_len = (u32)sizeof(g_la_mem);

		dbg_debug_dump->la_mem_dma_addr = (u32_l)(uint32_t)dma_map_single(siwifi_hw->dev, (siwifi_hw->dbgdump_elem.la_mem_addr),
				sizeof(g_la_mem), DMA_TO_DEVICE);

		if (dma_mapping_error(siwifi_hw->dev, (dma_addr_t)dbg_debug_dump->la_mem_dma_addr)) {
			dev_err(siwifi_hw->dev, "DMA mapping failed\n");
			siwifi_hw->dbgdump_elem.la_mem_addr = NULL;
			dbg_debug_dump->la_mem_dma_addr = 0;
			goto err_alloc;
		}
	}
#endif /* CONFIG_SF16A18_WIFI_LA_ENABLE */
#endif

    /*
     * Note that the RX buffers are no longer allocated here as their size depends on the
     * FW configuration, which is not available at that time.
     * They will be allocated when checking the parameter compatibility between the driver
     * and the underlying components (i.e. during the siwifi_handle_dynparams() execution)
     */

    if (siwifi_ipc_elem_pool_allocs(siwifi_hw, &siwifi_hw->e2arxdesc_pool,
                                  siwifi_hw->ipc_env->rxdesc_nb,
                                  sizeof(struct rxdesc_tag),
                                  "siwifi_ipc_e2arxdesc_pool",
                                  ipc_host_rxdesc_push))
        goto err_alloc;
    if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->dbg_mgmt_info_elem,
                                    sizeof(struct dbg_mgmt_info),
                                    DMA_FROM_DEVICE,
                                    NULL, NULL, NULL, NULL))
        goto err_alloc;
    if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->dbg_ctrl_info_elem,
                                    sizeof(struct dbg_ctrl_info),
                                    DMA_FROM_DEVICE,
                                    NULL, NULL, NULL, NULL))
        goto err_alloc;
#ifdef NEW_SCHEDULE
    if (siwifi_ipc_txdesc_allocs(siwifi_hw))
        goto err_alloc;
#endif
    // Repush the dying gasp information to lmac
    // dying gasp information should be allocated in wiphy init stage,
    // Here only need to repush it to lmac
    if (siwifi_hw->dying_gasp_info_elem.dma_addr) {
        ipc_host_dying_gasp_addr_push(siwifi_hw->ipc_env, siwifi_hw->dying_gasp_info_elem.dma_addr);
    }

#ifdef CONFIG_SIWIFI_TRX_STAT
    if (siwifi_ipc_elem_var_allocs(siwifi_hw, &siwifi_hw->dbg_trx_stat_elem,
                                    max(sizeof(struct dbg_tx_stat), sizeof(struct dbg_rx_stat)),
                                    DMA_FROM_DEVICE,
                                    NULL, NULL, NULL, NULL))
        goto err_alloc;
#endif
    return 0;

err_alloc:
    siwifi_elems_deallocs(siwifi_hw);
    return -ENOMEM;
}

/**
 * siwifi_ipc_msg_push() - Push a msg to IPC queue
 *
 * @siwifi_hw: Main driver data
 * @msg_buf: Pointer to message
 * @len: Size, in bytes, of message
 */
void siwifi_ipc_msg_push(struct siwifi_hw *siwifi_hw, void *msg_buf, uint16_t len)
{
    ipc_host_msg_push(siwifi_hw->ipc_env, msg_buf, len);
}

#ifndef NEW_SCHEDULE
/**
 * siwifi_ipc_txdesc_push() - Push a txdesc to FW
 *
 * @siwifi_hw: Main driver data
 * @tx_desc: Pointer on &struct txdesc_api to push to FW
 * @hostid: Pointer save in ipc env to retrieve tx buffer upon confirmation.
 * @hw_queue: Hw queue to push txdesc to
 * @user: User position to push the txdesc to. It must be set to 0 if  MU-MIMMO
 * is not used.
 */
void siwifi_ipc_txdesc_push(struct siwifi_hw *siwifi_hw, void *tx_desc,
                          void *hostid, int hw_queue, int user, u16 wmm_param)
{
    volatile struct txdesc_host *txdesc_host;

    txdesc_host = ipc_host_txdesc_get(siwifi_hw->ipc_env, hw_queue, user);
    BUG_ON(!txdesc_host);
#if 0
    /* check potential platform bug on multiple stores */
    memcpy(&txdesc_host->api, tx_desc, sizeof(*desc));
#else
    {
        u32 *src, *dst;
        int i;
        dst = (typeof(dst))&txdesc_host->api;
        src = (typeof(src))tx_desc;
        for (i = 0; i < sizeof(txdesc_host->api) / sizeof(*src); i++)
            *dst++ = *src++;
    }
    txdesc_host->wmm_param = wmm_param;
#endif
    wmb(); /* vs desc */
    ipc_host_txdesc_push(siwifi_hw->ipc_env, hw_queue, user, hostid);
}
#endif

/**
 * siwifi_ipc_fw_trace_desc_get() - Return pointer to the start of trace
 * description in IPC environment
 *
 * @siwifi_hw: Main driver data
 */
void *siwifi_ipc_fw_trace_desc_get(struct siwifi_hw *siwifi_hw)
{
    return (void *)&(siwifi_hw->ipc_env->shared->trace_pattern);
}


/**
 * siwifi_msgind() - IRQ handler callback for %IPC_IRQ_E2A_MSG
 *
 * @pthis: Pointer to main driver data
 * @hostid: Pointer to IPC elem from e2amsgs_pool
 */
static u8 siwifi_msgind(void *pthis, void *hostid)
{
    struct siwifi_hw *siwifi_hw = pthis;
    struct siwifi_ipc_elem *elem = hostid;
    struct ipc_e2a_msg *msg = elem->addr;
    u8 ret = 0;

    REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_MSGIND);

    /* Look for pattern which means that this hostbuf has been used for a MSG */
    if (msg->pattern != IPC_MSGE2A_VALID_PATTERN) {
        ret = -1;
        goto msg_no_push;
    }
    /* Relay further actions to the msg parser */
    siwifi_rx_handle_msg(siwifi_hw, msg);

    /* Reset the msg element and re-use it */
    msg->pattern = 0;
    wmb();

    /* Push back the buffer to the LMAC */
    ipc_host_msgbuf_push(siwifi_hw->ipc_env, elem, (u32)elem->dma_addr);

msg_no_push:
    REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_MSGIND);
    return ret;
}

/**
 * siwifi_msgackind() - IRQ handler callback for %IPC_IRQ_E2A_MSG_ACK
 *
 * @pthis: Pointer to main driver data
 * @hostid: Pointer to command acknoledged
 */
static u8 siwifi_msgackind(void *pthis, void *hostid)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)pthis;
    siwifi_hw->cmd_mgr.llind(&siwifi_hw->cmd_mgr, (struct siwifi_cmd *)hostid);
    return -1;
}

/**
 * siwifi_radarind() - IRQ handler callback for %IPC_IRQ_E2A_RADAR
 *
 * @pthis: Pointer to main driver data
 * @hostid: Pointer to IPC elem from e2aradars_pool
 */
static u8 siwifi_radarind(void *pthis, void *hostid)
{
#ifdef CONFIG_SIWIFI_RADAR
    struct siwifi_hw *siwifi_hw = pthis;
    struct siwifi_ipc_elem *elem = hostid;
    struct radar_pulse_array_desc *pulses = elem->addr;
    u8 ret = 0;
    int i;

    /* Look for pulse count meaning that this hostbuf contains RADAR pulses */
    if (pulses->cnt == 0) {
        ret = -1;
        goto radar_no_push;
    }

    if (siwifi_radar_detection_is_enable(&siwifi_hw->radar, pulses->idx)) {
        /* Save the received pulses only if radar detection is enabled */
        for (i = 0; i < pulses->cnt; i++) {
            struct siwifi_radar_pulses *p = &siwifi_hw->radar.pulses[pulses->idx];
#if 1
            struct radar_pulse *rp = (struct radar_pulse*)&pulses->pulse[i];
            if (rp->rep <= 1)
                continue;
            //printk("rp[%d] freq %02d fom %02d len %04d (%4d)rep %04d n",
            //           i, rp->freq, rp->fom, rp->len, RADAR_LENGTH(rp->len), rp->rep);
#endif
            p->buffer[p->index] = pulses->pulse[i];
            p->index = (p->index + 1) % SIWIFI_RADAR_PULSE_MAX;
            if (p->count < SIWIFI_RADAR_PULSE_MAX)
                p->count++;
        }

        /* Defer pulse processing in separate work */
        if (! work_pending(&siwifi_hw->radar.detection_work))
            schedule_work(&siwifi_hw->radar.detection_work);
    }

    /* Reset the radar element and re-use it */
    pulses->cnt = 0;
    wmb();

    /* Push back the buffer to the LMAC */
    ipc_host_radarbuf_push(siwifi_hw->ipc_env, elem, (u32)elem->dma_addr);

radar_no_push:
    return ret;
#else
    return -1;
#endif
}

/**
 * siwifi_prim_tbtt_ind() - IRQ handler callback for %IPC_IRQ_E2A_TBTT_PRIM
 *
 * @pthis: Pointer to main driver data
 */
static void siwifi_prim_tbtt_ind(void *pthis)
{
    return;
}

/**
 * siwifi_sec_tbtt_ind() - IRQ handler callback for %IPC_IRQ_E2A_TBTT_SEC
 *
 * @pthis: Pointer to main driver data
 */
static void siwifi_sec_tbtt_ind(void *pthis)
{
}

#ifdef CONFIG_ERROR_DUMP
void siwifi_save_error_info(struct siwifi_hw *siwifi_hw, char *msg, u8 len){
	u32 idx = 0;
	u32 time_idx = 0;
	u16 length = len;
	unsigned long now = jiffies - 0xFFFF8AF5;
	char *buf = NULL;
		//printk("~now:%lu\n",now);
		//check the length
		if(len > ERROR_BUF_MAX_SIZE)
			length = ERROR_BUF_MAX_SIZE;

		spin_lock(&siwifi_hw->dbgdump_elem.err_lock);
		idx = siwifi_hw->dbgdump_elem.err_cur_idx;
		if(siwifi_hw->dbgdump_elem.errorinfo[idx]){
			//if the error msg is same as the last one, do not save again
			if(siwifi_hw->dbgdump_elem.errorinfo[idx]->buf &&
				(!strncmp(msg, siwifi_hw->dbgdump_elem.errorinfo[idx]->buf, length)) &&
				(siwifi_hw->dbgdump_elem.errorinfo[idx]->count < MAX_TIME_NUM)){
				time_idx = siwifi_hw->dbgdump_elem.errorinfo[idx]->count;
				siwifi_hw->dbgdump_elem.errorinfo[idx]->time[time_idx] = now;
				siwifi_hw->dbgdump_elem.errorinfo[idx]->count ++;
			}
			else{//a new error msg come
				//check the rest memroy
				if(siwifi_hw->dbgdump_elem.errorinfo[idx]->buf){
					printk("siwifi_hw->dbgdump_elem.err_cur_idx %d,%d,%p,%d,%d\n",idx,siwifi_hw->dbgdump_elem.err_cur_idx,siwifi_hw->dbgdump_elem.errorinfo[idx]->buf,
						strncmp(msg, siwifi_hw->dbgdump_elem.errorinfo[idx]->buf, length),
						siwifi_hw->dbgdump_elem.errorinfo[idx]->count);
				}
				if((siwifi_hw->dbgdump_elem.err_cur_idx >= (ERROR_INFO_MAX_NUM - 1)) || (siwifi_hw->dbgdump_elem.err_total_count == 0))
					siwifi_hw->dbgdump_elem.err_cur_idx = 0;
				else
					siwifi_hw->dbgdump_elem.err_cur_idx ++;

				idx = siwifi_hw->dbgdump_elem.err_cur_idx;

				if(siwifi_hw->dbgdump_elem.errorinfo[idx]->buf){
					siwifi_kfree(siwifi_hw->dbgdump_elem.errorinfo[idx]->buf);
					siwifi_hw->dbgdump_elem.errorinfo[idx]->len = 0;
					siwifi_hw->dbgdump_elem.errorinfo[idx]->count = 0;
					siwifi_hw->dbgdump_elem.errorinfo[idx]->buf = NULL;
				}

				//alloc a new buffer to save
				if(!siwifi_hw->dbgdump_elem.errorinfo[idx]->buf){
					buf = siwifi_kzalloc(length, GFP_ATOMIC);
					if (!buf) {
						printk(KERN_CRIT "%s:%d: error_info alloc of size %u failed\n\n",
								__func__, __LINE__, sizeof(struct error_info));
						spin_unlock(&siwifi_hw->dbgdump_elem.err_lock);
						return;
					}
					siwifi_hw->dbgdump_elem.errorinfo[idx]->buf = buf;
					siwifi_hw->dbgdump_elem.errorinfo[idx]->len = length;
				}
				//copy error msg
				memcpy(siwifi_hw->dbgdump_elem.errorinfo[idx]->buf, msg, length);
				time_idx = siwifi_hw->dbgdump_elem.errorinfo[idx]->count;
				siwifi_hw->dbgdump_elem.errorinfo[idx]->time[time_idx] = now;
				siwifi_hw->dbgdump_elem.errorinfo[idx]->count ++;
			}
			siwifi_hw->dbgdump_elem.err_total_count ++;
			spin_unlock(&siwifi_hw->dbgdump_elem.err_lock);
		}
		else{
			spin_unlock(&siwifi_hw->dbgdump_elem.err_lock);
			printk(KERN_CRIT "errorinfo[%d] is null\n",idx);
		}
}
#endif
/**
 * siwifi_dbgind() - IRQ handler callback for %IPC_IRQ_E2A_DBG
 *
 * @pthis: Pointer to main driver data
 * @hostid: Pointer to IPC elem from dbgmsgs_pool
 */
static u8 siwifi_dbgind(void *pthis, void *hostid)
{
    struct siwifi_hw *siwifi_hw = (struct siwifi_hw *)pthis;
    struct siwifi_ipc_elem *elem = hostid;
    struct ipc_dbg_msg *dbg_msg = elem->addr;
    u8 ret = 0;

    REG_SW_SET_PROFILING(siwifi_hw, SW_PROF_DBGIND);

    /* Look for pattern which means that this hostbuf has been used for a MSG */
    if (dbg_msg->pattern != IPC_DBG_VALID_PATTERN) {
        ret = -1;
        goto dbg_no_push;
    }

    /* Display the LMAC string */
    printk("lmac[%d] %s", siwifi_hw->mod_params->is_hb, (char *)dbg_msg->string);
#ifdef CONFIG_ERROR_DUMP
	if(!strncmp((char *)dbg_msg->string,"ASSERT", 6)){
		siwifi_save_error_info(siwifi_hw,(char *)dbg_msg->string, strlen((char *)dbg_msg->string));
	}
#endif

    /* Reset the msg element and re-use it */
    dbg_msg->pattern = 0;
    wmb();

    /* Push back the buffer to the LMAC */
    ipc_host_dbgbuf_push(siwifi_hw->ipc_env, elem, (u32)elem->dma_addr);

dbg_no_push:
    REG_SW_CLEAR_PROFILING(siwifi_hw, SW_PROF_DBGIND);

    return ret;
}

/**
 * siwifi_ipc_rxbuf_init() - Allocate and initialize RX buffers.
 *
 * @siwifi_hw: Main driver data
 * @rx_bufsz: Size of the buffer to be allocated
 *
 * This function updates the RX buffer size according to the parameter and allocates the
 * RX buffers
 */
int siwifi_ipc_rxbuf_init(struct siwifi_hw *siwifi_hw, uint32_t rx_bufsz)
{
    siwifi_hw->ipc_env->rx_bufsz = rx_bufsz;
    siwifi_hw->ipc_env->rx_pd_offset =
        max(SIWIFI_RX_HEADROOM, (uint32_t)max(SIWIFI_TX_HEADROOM, SIWIFI_TX_AMSDU_HEADROOM));
    printk("tx headroom : %d, tx amsdu headroom : %d, rx headroom : %d, rx pd offset %d\n",
            SIWIFI_TX_HEADROOM, SIWIFI_TX_AMSDU_HEADROOM, SIWIFI_RX_HEADROOM, siwifi_hw->ipc_env->rx_pd_offset);
    return(siwifi_ipc_rxbuf_elems_allocs(siwifi_hw));
}

/**
 * siwifi_ipc_init() - Initialize IPC interface.
 *
 * @siwifi_hw: Main driver data
 * @shared_ram: Pointer to shared memory that contains IPC shared struct
 *
 * This function initializes IPC interface by registering callbacks, setting
 * shared memory area and calling IPC Init function.
 * It should be called only once during driver's lifetime.
 */
int siwifi_ipc_init(struct siwifi_hw *siwifi_hw, u8 *shared_ram, void *reg_base)
{
    struct ipc_host_cb_tag cb;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    /* initialize the API interface */
    cb.recv_data_ind   = siwifi_rxdataind;
    cb.recv_radar_ind  = siwifi_radarind;
    cb.recv_msg_ind    = siwifi_msgind;
    cb.recv_msgack_ind = siwifi_msgackind;
    cb.recv_dbg_ind    = siwifi_dbgind;
#ifdef NEW_SCHEDULE
    cb.send_data_cfm_burst   = siwifi_txdatacfm_burst;
#else
    cb.send_data_cfm   = siwifi_txdatacfm;
#endif
    cb.prim_tbtt_ind   = siwifi_prim_tbtt_ind;
    cb.sec_tbtt_ind    = siwifi_sec_tbtt_ind;
    cb.recv_unsup_rx_vec_ind = siwifi_unsup_rx_vec_ind;

    /* set the IPC environment */
    siwifi_hw->ipc_env = (struct ipc_host_env_tag *)
                       siwifi_kzalloc(sizeof(struct ipc_host_env_tag), GFP_KERNEL);
    siwifi_hw->ipc_env->mod_params = siwifi_hw->mod_params;
    /* call the initialization of the IPC */
    ipc_host_init(siwifi_hw->ipc_env, &cb,
                  (struct ipc_shared_env_tag *)shared_ram, reg_base, siwifi_hw);

    siwifi_cmd_mgr_init(&siwifi_hw->cmd_mgr);

    return siwifi_elems_allocs(siwifi_hw);
}

/**
 * siwifi_ipc_deinit() - Release IPC interface
 *
 * @siwifi_hw: Main driver data
 */
void siwifi_ipc_deinit(struct siwifi_hw *siwifi_hw)
{
    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    siwifi_ipc_tx_drain(siwifi_hw);
    siwifi_cmd_mgr_deinit(&siwifi_hw->cmd_mgr);
    siwifi_elems_deallocs(siwifi_hw);
#ifdef NEW_SCHEDULE
    ipc_host_deinit(siwifi_hw->ipc_env);
#endif
    siwifi_kfree(siwifi_hw->ipc_env);
    siwifi_hw->ipc_env = NULL;
}

/**
 * siwifi_ipc_start() - Start IPC interface
 *
 * @siwifi_hw: Main driver data
 */
void siwifi_ipc_start(struct siwifi_hw *siwifi_hw)
{
    ipc_host_enable_irq(siwifi_hw->ipc_env, IPC_IRQ_E2A_ALL);
}

/**
 * siwifi_ipc_stop() - Stop IPC interface
 *
 * @siwifi_hw: Main driver data
 */
void siwifi_ipc_stop(struct siwifi_hw *siwifi_hw)
{
    ipc_host_disable_irq(siwifi_hw->ipc_env, IPC_IRQ_E2A_ALL);
}

/**
 * siwifi_ipc_tx_drain() - Flush IPC TX buffers
 *
 * @siwifi_hw: Main driver data
 *
 * This assumes LMAC is still (tx wise) and there's no TX race until LMAC is up
 * tx wise.
 * This also lets both IPC sides remain in sync before resetting the LMAC,
 * e.g with siwifi_send_reset.
 */
void siwifi_ipc_tx_drain(struct siwifi_hw *siwifi_hw)
{
    int i, j;

    SIWIFI_DBG(SIWIFI_FN_ENTRY_STR);

    if (!siwifi_hw->ipc_env) {
        printk(KERN_CRIT "%s: bypassing (restart must have failed)\n", __func__);
        return;
    }

    for (i = 0; i < SIWIFI_HWQ_NB; i++) {
        for (j = 0; j < nx_txuser_cnt[i]; j++) {
            struct sk_buff *skb;
#ifdef NEW_SCHEDULE
            struct sk_buff_head *skb_list;
            while((skb_list = (struct sk_buff_head *)ipc_host_tx_flush(siwifi_hw->ipc_env, i, j))) {
                while ((skb = __skb_dequeue(skb_list)) != NULL) {
                    struct siwifi_sw_txhdr *sw_txhdr =
                        ((struct siwifi_txhdr *)skb->data)->sw_hdr;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
                    if (sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU) {
                        siwifi_free_amsdu(siwifi_hw, sw_txhdr);
                    }
#endif
                    dma_unmap_single(siwifi_hw->dev, sw_txhdr->dma_addr,
                                     sw_txhdr->map_len, DMA_TO_DEVICE);
                    skb_pull(skb, sw_txhdr->headroom);
                    siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
                    dev_kfree_skb_any(skb);
                }
            }
#else
            while ((skb = (struct sk_buff *)ipc_host_tx_flush(siwifi_hw->ipc_env, i, j))) {
                struct siwifi_sw_txhdr *sw_txhdr =
                    ((struct siwifi_txhdr *)skb->data)->sw_hdr;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
                if (sw_txhdr->desc.host.flags & TXU_CNTRL_AMSDU) {
                    siwifi_free_amsdu(siwifi_hw, sw_txhdr);
                }
#endif
                dma_unmap_single(siwifi_hw->dev, sw_txhdr->dma_addr,
                                 sw_txhdr->map_len, DMA_TO_DEVICE);
                skb_pull(skb, sw_txhdr->headroom);
                siwifi_free_swtxhdr(siwifi_hw, sw_txhdr);
                dev_kfree_skb_any(skb);
            }
#endif
        }
    }
}

/**
 * siwifi_ipc_tx_pending() - Check if TX pframes are pending at FW level
 *
 * @siwifi_hw: Main driver data
 */
bool siwifi_ipc_tx_pending(struct siwifi_hw *siwifi_hw)
{
    return ipc_host_tx_frames_pending(siwifi_hw->ipc_env);
}

void siwifi_trigger_restart(struct siwifi_hw *siwifi_hw)
{
    //need we wait the um helper finish, because maybe user want the debug
    //trace files
    set_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags);
    clear_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags);

#ifdef LOCAL_RECOVERY
    schedule_work(&siwifi_hw->restart_work);
#endif
}

/**
 * siwifi_error_ind() - %DBG_ERROR_IND message callback
 *
 * @siwifi_hw: Main driver data
 *
 * This function triggers the UMH script call that will indicate to the user
 * space the error that occurred and stored the debug dump. Once the UMH script
 * is executed, the siwifi_umh_done() function has to be called.
 */
void siwifi_error_ind(struct siwifi_hw *siwifi_hw, bool restart, bool dump_en)
{
#ifdef CFG_DBGDUMP
    struct siwifi_ipc_elem_var *elem = &siwifi_hw->dbgdump_elem.buf;
#endif /* CFG_DBGDUMP */
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
    struct dbg_debug_dump_tag *dump = elem->addr;
#endif
#ifdef CFG_DBGDUMP
    dma_sync_single_for_device(siwifi_hw->dev, elem->dma_addr, elem->size,
                    DMA_FROM_DEVICE);
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
	dma_sync_single_for_device(siwifi_hw->dev, (dma_addr_t)dump->la_mem_dma_addr,
                        siwifi_hw->dbgdump_elem.la_len, DMA_FROM_DEVICE);
#endif /* CONFIG_SF16A18_WIFI_LA_ENABLE */

    //printk(KERN_CRIT "%s(%d) (type %d, restart : %d, dump_en : %d): dump received\n", __func__, CONFIG_BAND_TYPE,
    //                dump->dbg_info.error_type, restart, dump_en);
#endif /* CFG_DBGDUMP */
    if (
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->debugfs.recovery &&
#endif /* CONFIG_SIWIFI_DEBUGFS */
        restart) {
        //reprobe driver if crashed in ops_start
        if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags)) {
            siwifi_reprobe_driver(siwifi_hw);
        } else if (test_bit(SIWIFI_DEV_STACK_RESTARTING, &siwifi_hw->drv_flags)) {
            siwifi_reprobe_driver(siwifi_hw);
        } else if (list_empty(&siwifi_hw->vifs)) {
            //see RM#1704
            //if interface has not been added yet,hw reconfig schedule will not take effect in mac80211,so we have to reporbe driver this time
            SIWIFI_DBG("vifs is empty, we can't use restart_hw now\n");
            siwifi_reprobe_driver(siwifi_hw);
        } else {
#ifndef LOCAL_RECOVERY
            //now fullmac recovery is to do,so reprobe driver directly
            siwifi_reprobe_driver(siwifi_hw);
            return;
#endif /* LOCAL_RECOVERY */
#if defined(CFG_DBGDUMP)
#if defined(CONFIG_SIWIFI_DEBUGFS) || defined(CONFIG_SIWIFI_PROCFS)
            if (dump_en) {
                siwifi_hw->debugfs.trace_prst = true;
                siwifi_trigger_um_helper(&siwifi_hw->debugfs);
            }
#endif /* CFG_DBGDUMP*/
#endif/* (CONFIG_SIWIFI_DEBUGFS || CONFIG_SIWIFI_PROCFS)*/
            siwifi_trigger_restart(siwifi_hw);
            return;
        }
    }
#if defined(CFG_DBGDUMP)
#if defined(CONFIG_SIWIFI_DEBUGFS) || defined(CONFIG_SIWIFI_PROCFS)
    else if (dump_en) {
        siwifi_hw->debugfs.trace_prst = true;
        siwifi_trigger_um_helper(&siwifi_hw->debugfs);
    }
#endif /* CFG_DBGDUMP*/
#endif/* (CONFIG_SIWIFI_DEBUGFS || CONFIG_SIWIFI_PROCFS)*/
}

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
/**
 * siwifi_umh_done() - Indicate User Mode helper finished
 *
 * @siwifi_hw: Main driver data
 *
 */
void siwifi_umh_done(struct siwifi_hw *siwifi_hw)
{
    if (!test_bit(SIWIFI_DEV_STARTED, &siwifi_hw->drv_flags))
        return;

    /* this assumes error_ind won't trigger before ipc_host_dbginfobuf_push
       is called and so does not irq protect (TODO) against error_ind */
    siwifi_hw->debugfs.trace_prst = false;
#ifdef CFG_DBGDUMP
    ipc_host_dbginfobuf_push(siwifi_hw->ipc_env, siwifi_hw->dbgdump_elem.buf.dma_addr);
#endif
}
#endif

/*
   @siwifi_hw: Main driver data
   check if rx buffer has ever alloc fail
 */
void siwifi_ipc_rxbuf_elem_fail_check(struct siwifi_hw *siwifi_hw)
{
    while (siwifi_hw->rx_skb_alloc_fail_cnt) {
        siwifi_hw->rx_skb_alloc_fail_cnt--;
        if (siwifi_ipc_rxbuf_elem_allocs(siwifi_hw)) {
            printk("rx_skb_alloc_fail_check still cannot alloc skb! \n");
            break;
        }
    }
}

void siwifi_sta_hash_add(struct siwifi_vif *vif, struct siwifi_sta *sta)
{
	int hash_idx = STA_HASH(sta->mac_addr);
	if (vif->sta_hash[hash_idx]) {
		struct siwifi_sta *prev_sta = (struct siwifi_sta *)(vif->sta_hash[hash_idx]);
		vif->sta_hash[hash_idx] = (uint32_t)sta;
		sta->hnext = prev_sta;
	} else {
		sta->hnext = NULL;
		vif->sta_hash[hash_idx] = (uint32_t)sta;
	}
	printk("add sta(%pM) to hash table %d!\n", sta->mac_addr, hash_idx);
}

void siwifi_sta_hash_del(struct siwifi_vif *vif, struct siwifi_sta *sta)
{
    struct siwifi_sta *s;

    s = (struct siwifi_sta *)(vif->sta_hash[STA_HASH(sta->mac_addr)]);
    if (s == NULL) return;
    if (ether_addr_equal(s->mac_addr, sta->mac_addr)) {
        vif->sta_hash[STA_HASH(sta->mac_addr)] = (uint32_t)s->hnext;
        printk("remove STA %pM from hash table success\n", sta->mac_addr);
        return;
    }

    while (s->hnext != NULL && !ether_addr_equal(s->hnext->mac_addr, sta->mac_addr))
        s = s->hnext;
    if (s->hnext != NULL) {
        s->hnext = s->hnext->hnext;
        printk("remove STA %pM from hash table success\n", sta->mac_addr);
	} else {
        printk("could not remove STA %pM from hash table\n", sta->mac_addr);
	}
}

struct siwifi_sta *siwifi_sta_hash_get(struct siwifi_vif *vif, const u8 *addr)
{
	struct siwifi_sta *s = NULL;

	s = (struct siwifi_sta *)(vif->sta_hash[STA_HASH(addr)]);
	while (s != NULL && !ether_addr_equal(s->mac_addr, addr))
		s = s->hnext;
	return s;
}

// call this function with "spin_lock_bh(&siwifi_vif->src_filter_lock)"
void siwifi_src_filter_hash_del(struct siwifi_vif *vif, unsigned char *src_mac)
{
    struct siwifi_src_filter *src_filter = NULL;
    struct siwifi_src_filter *src_filter_tmp = NULL;

    src_filter = (struct siwifi_src_filter *)(vif->src_filter_hash[STA_HASH(src_mac)]);
    if (!src_filter) {
        return;
    }
    if (ether_addr_equal(src_filter->src_macaddr, src_mac)) {
        vif->src_filter_hash[STA_HASH(src_mac)] = (uint32_t)src_filter->next;
        //printk("del src filter %pM\n", src_filter->src_macaddr);
        siwifi_kfree(src_filter);
        return;
    }
    while (src_filter->next && !ether_addr_equal(src_filter->next->src_macaddr, src_mac))
        src_filter = src_filter->next;
    if (src_filter->next) {
        src_filter_tmp = src_filter->next;
        src_filter->next = src_filter->next->next;
        //printk("del src filter %pM\n", src_filter_tmp->src_macaddr);
        siwifi_kfree(src_filter_tmp);
    } else {
        printk("fail del src filter %pM\n", src_mac);
    }

    return;
}

void src_filter_aging(unsigned long ptr)
{
    struct siwifi_vif *siwifi_vif = (struct siwifi_vif *)ptr;
    struct siwifi_src_filter *src_filter = NULL;
    struct siwifi_src_filter *src_filter_tmp = NULL;
    int i;

    spin_lock_bh(&siwifi_vif->src_filter_lock);
    for (i = 0; i < STA_HASH_SIZE; i++) {
        src_filter = (struct siwifi_src_filter *)(siwifi_vif->src_filter_hash[i]);
        while (src_filter) {
            src_filter_tmp = src_filter->next;
            if (time_after_eq(jiffies, src_filter->last_use + SRC_FILTER_AGING_TIME * HZ)) {
                //printk("aging src filter %pM\n", src_filter->src_macaddr);
                siwifi_src_filter_hash_del(siwifi_vif, src_filter->src_macaddr);
            }
            src_filter = src_filter_tmp;
        }
    }
    spin_unlock_bh(&siwifi_vif->src_filter_lock);
    mod_timer(&(siwifi_vif->src_filter_timer), jiffies + SRC_FILTER_AGING_TIME * HZ);
}

void siwifi_src_filter_hash_free(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_src_filter *src_filter = NULL;
    struct siwifi_src_filter *src_filter_tmp = NULL;
    int i;
    if (siwifi_vif == NULL)
        return;
    if (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_STATION)
        return;

    spin_lock_bh(&siwifi_vif->src_filter_lock);
    for (i = 0; i < STA_HASH_SIZE; i++) {
        src_filter = (struct siwifi_src_filter *)(siwifi_vif->src_filter_hash[i]);
        while (src_filter) {
            src_filter_tmp = src_filter->next;
            siwifi_src_filter_hash_del(siwifi_vif, src_filter->src_macaddr);
            src_filter = src_filter_tmp;
        }
    }
    printk("free src filter hash table\n");

    spin_unlock_bh(&siwifi_vif->src_filter_lock);
    return;
}

struct siwifi_src_filter *siwifi_src_filter_hash_search(struct siwifi_vif *siwifi_vif, unsigned char *src_mac)
{
    struct siwifi_src_filter *src_filter = NULL;
    if (siwifi_vif == NULL || src_mac == NULL)
        return NULL;
    if (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_STATION)
        return NULL;

    spin_lock_bh(&siwifi_vif->src_filter_lock);
    src_filter = (struct siwifi_src_filter *)(siwifi_vif->src_filter_hash[STA_HASH(src_mac)]);
    while (src_filter != NULL && !ether_addr_equal(src_filter->src_macaddr, src_mac))
        src_filter = src_filter->next;

    spin_unlock_bh(&siwifi_vif->src_filter_lock);
    return src_filter;
}

//Record the MAC address of the TX SKB
//Drop the RX skb whose src is the recorded macaddr
void siwifi_update_src_filter(struct siwifi_vif *siwifi_vif, unsigned char *src_mac)
{
    int hash_idx = STA_HASH(src_mac);
    struct siwifi_src_filter *src_filter = NULL;
    struct siwifi_src_filter *src_filter_prev = NULL;
    if (siwifi_vif == NULL || src_mac == NULL)
        return;
    if (SIWIFI_VIF_TYPE(siwifi_vif) != NL80211_IFTYPE_STATION)
        return;
    if ((src_filter = siwifi_src_filter_hash_search(siwifi_vif, src_mac))) {
        src_filter->last_use = jiffies;
        return;
    }
    src_filter = (struct siwifi_src_filter *)siwifi_kzalloc(sizeof(struct siwifi_src_filter), GFP_ATOMIC);
    if (src_filter)
        memcpy(src_filter->src_macaddr, src_mac, ETH_ALEN);
    else
        return;

    spin_lock_bh(&siwifi_vif->src_filter_lock);
    if (siwifi_vif->src_filter_hash[hash_idx]) {
        src_filter_prev = (struct siwifi_src_filter *)(siwifi_vif->src_filter_hash[hash_idx]);
        siwifi_vif->src_filter_hash[hash_idx] = (uint32_t)src_filter;
        src_filter->next = src_filter_prev;
    } else {
        src_filter->next = NULL;
        siwifi_vif->src_filter_hash[hash_idx] = (uint32_t)src_filter;
    }
    src_filter->last_use = jiffies;
    //printk("add new src filter %pM\n", src_mac);

    spin_unlock_bh(&siwifi_vif->src_filter_lock);
    return;
}

/*
 * check whether the skb is icmp or not
 *
 * Return 1 for yes, 0 for no
 */
bool siwifi_check_skb_is_icmp(struct sk_buff *skb)
{
    struct ethhdr *eth;
    struct iphdr *ipdata;

    eth = (struct ethhdr *)(skb->data);
    if (skb->len > sizeof(struct ethhdr) + 19) {
        if (eth->h_proto == htons(ETH_P_IP))
        {
            ipdata = (struct iphdr *)((char *)eth + sizeof(struct ethhdr));
            if (ipdata->version == 4) {
                if (ipdata->protocol == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*
 * check whether the skb is dhcp or not
 *
 * Return 1 for yes, 0 for no
 */
int siwifi_check_skb_is_dhcp(struct sk_buff *skb)
{
    struct ethhdr *ethhdr = NULL;
    struct iphdr *iphdr = NULL;
    struct ipv6hdr *ipv6hdr = NULL;
    struct udphdr *udphdr = NULL;
    __be16 proto;
    unsigned int header_len = 0;

    ethhdr = (struct ethhdr *)skb->data;
    proto = ethhdr->h_proto;
    header_len += ETH_HLEN;

    /* check for ip header */
    switch (proto) {
        case htons(ETH_P_IP):
            if (!pskb_may_pull(skb, header_len + sizeof(*iphdr)))
                return 0;
            iphdr = (struct iphdr *)(skb->data + header_len);
            header_len += iphdr->ihl * 4;
            /* check for udp header */
            if (iphdr->protocol != IPPROTO_UDP)
                return 0;
            break;
        case htons(ETH_P_IPV6):
            if (!pskb_may_pull(skb, header_len + sizeof(*ipv6hdr)))
                return 0;
            ipv6hdr = (struct ipv6hdr *)(skb->data + header_len);
            header_len += sizeof(*ipv6hdr);
            /* check for udp header */
            if (ipv6hdr->nexthdr != IPPROTO_UDP)
                return 0;
            break;
        default:
            return 0;
    }
    if (!pskb_may_pull(skb, header_len + sizeof(*udphdr)))
        return 0;
    udphdr = (struct udphdr *)(skb->data + header_len);
    header_len += sizeof(*udphdr);

    /* check for bootp port */
    switch (proto) {
        case htons(ETH_P_IP):
            if (udphdr->dest == htons(67) || udphdr->source == htons(67))
                return 1;
            break;
        case htons(ETH_P_IPV6):
            if (udphdr->dest == htons(547) || udphdr->source == htons(547))
                return 1;
            break;
    }
    return 0;
}

void set_repeater_status(struct siwifi_vif *siwifi_vif, u8 status)
{
    if (SIWIFI_VIF_TYPE(siwifi_vif) == NL80211_IFTYPE_STATION)
        siwifi_vif->repeater_status = status;
}

int siwifi_set_assoc_req_insert_info(struct siwifi_hw *siwifi_hw, char *insert_info, int insert_length)
{
    u32_l dma_addr;
    if (!siwifi_hw || !insert_info || !insert_length)
        return 0;
    if (siwifi_hw->assoc_req_insert_info)
        siwifi_kfree(siwifi_hw->assoc_req_insert_info);
    // TODO:Check the correctness of ie field
    siwifi_hw->assoc_req_insert_info = (char *)siwifi_kzalloc(insert_length, GFP_KERNEL);
    if (!siwifi_hw->assoc_req_insert_info)
        return 0;
    memcpy(siwifi_hw->assoc_req_insert_info, insert_info, insert_length);
    siwifi_hw->assoc_insert.info_dmalength = insert_length;
    dma_addr = dma_map_single(siwifi_hw->dev, siwifi_hw->assoc_req_insert_info, siwifi_hw->assoc_insert.info_dmalength, DMA_BIDIRECTIONAL);
    if (dma_mapping_error(siwifi_hw->dev, dma_addr))
        return 0;
    siwifi_hw->assoc_insert.info_dmaaddr = dma_addr;
    siwifi_send_assoc_req_insert_info(siwifi_hw);
    return 1;
}

extern int siwifi_send_get_channel_info_req(struct siwifi_hw *siwifi_hw, struct mm_get_channel_info_cfm *cfm);
extern uint16_t siwifi_freq_to_channel(uint16_t freq);
int siwifi_channel_recovery_check(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_vif *vif = NULL;
    struct siwifi_vif *vif_sta = NULL;
    uint8_t channel_idx = 0;
    int channel_freq = 0;
    enum nl80211_chan_width channel_width = NL80211_CHAN_WIDTH_20;
    struct mm_get_channel_info_cfm cfm;
    struct siwifi_bcn *bcn = NULL;
    const uint8_t *ie_ds = NULL;
    uint8_t ie_offset = 0;
    uint8_t channel_num = 0;
    //printk("band %d : %s start\n", CONFIG_BAND_TYPE, __func__);
    // only for repeater
    list_for_each_entry(vif, &siwifi_hw->vifs, list)
    {
        if (vif->up && (SIWIFI_VIF_TYPE(vif) == NL80211_IFTYPE_STATION))
        {
            vif_sta = vif;
            //printk("%s vif %s freq %d chidx %d \n", __func__,
            //        vif->ndev->name, siwifi_hw->chanctx_table[vif->ch_index].chan_def.chan?siwifi_hw->chanctx_table[vif->ch_index].chan_def.chan->center_freq:0, vif->ch_index);
            break;
        }
    }
    if (!vif_sta) {
        return 0;
    }
    memset(&cfm, 0, sizeof(struct mm_get_channel_info_cfm));
    if (!siwifi_send_get_channel_info_req(siwifi_hw, &cfm)) {
        //printk("get lmac channel idx %d freq %d width %d freq1 %d freq2 %d \n",
        //        cfm.channel_idx, cfm.channel_freq, cfm.channel_width, cfm.center_freq1, cfm.center_freq2);
        channel_idx = cfm.channel_idx;
        channel_freq = cfm.channel_freq;
        switch (cfm.channel_width) {
            case 0:
                channel_width = NL80211_CHAN_WIDTH_20;
                break;
            case 1:
                channel_width = NL80211_CHAN_WIDTH_40;
                break;
            case 2:
                channel_width = NL80211_CHAN_WIDTH_80;
                break;
        }
    } else {
        printk("send siwifi_send_get_channel_info_req fail!\n");
        return 0;
    }
    spin_lock_bh(&siwifi_hw->cb_lock);
    // only check vif_sta, vif_ap will follow vif_sta when siwifi_chanctx_link
    if (!(siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan)) {
        struct cfg80211_chan_def chandef;
        struct ieee80211_channel *chan = ieee80211_get_channel(siwifi_hw->wiphy, channel_freq);
        if (chan) {
            cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_NO_HT);
            chandef.width = channel_width;
            chandef.center_freq1 = cfm.center_freq1;
            chandef.center_freq2 = cfm.center_freq2;
            siwifi_chanctx_unlink(vif_sta);
            siwifi_chanctx_link(vif_sta, channel_idx, &chandef);
            printk("vif_sta %s channel restored: freq=%d, width=%d\n",
                vif_sta->ndev->name, channel_freq, channel_width);
        } else {
            printk("vif_sta %s channel recovery failed: unable to retrieve channel object\n", vif_sta->ndev->name);
        }
        // It shouldn't have happened
        printk("vif_sta %s [ch_idx %d].chan_def.chan is NULL\n", vif_sta->ndev->name, vif_sta->ch_index);
        spin_unlock_bh(&siwifi_hw->cb_lock);
        return 0;
    }
    if (channel_freq != siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan->center_freq ||
            channel_width != siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.width ||
            channel_idx != vif_sta->ch_index) {
        struct cfg80211_chan_def chandef;
        struct ieee80211_channel *chan;
        printk("ERROR: ch_freq %d width %d idx %d is different from vif_sta(%s) freq %d width %d idx %d \n",
                channel_freq, channel_width, channel_idx,
                vif_sta->ndev->name,
                siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan->center_freq,
                siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.width,
                vif_sta->ch_index);
        if(!channel_freq || (channel_idx == 255) ||
                !(siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan->center_freq)
                || (vif_sta->ch_index == 255))
        {
            spin_unlock_bh(&siwifi_hw->cb_lock);
            return 0;
        }
        siwifi_chanctx_unlink(vif_sta);
        chan = ieee80211_get_channel(siwifi_hw->wiphy, channel_freq);
        cfg80211_chandef_create(&chandef, chan, NL80211_CHAN_NO_HT);
        if (!siwifi_hw->mod_params->ht_on)
            chandef.width = NL80211_CHAN_WIDTH_20_NOHT;
        else
            chandef.width = channel_width;
        chandef.center_freq1 = cfm.center_freq1;
        chandef.center_freq2 = cfm.center_freq2;
        siwifi_chanctx_link(vif_sta, channel_idx, &chandef);
    }
    list_for_each_entry(vif, &siwifi_hw->vifs, list)
    {
        struct siwifi_chanctx *ctxt_sta = NULL;
        uint8_t vif_sta_channel_num = 0;
        enum nl80211_chan_width vif_sta_channel_width = NL80211_CHAN_WIDTH_20;
        enum nl80211_chan_width vif_channel_width = NL80211_CHAN_WIDTH_20;
        // only ap use beacon
        if (!vif->up || (SIWIFI_VIF_TYPE(vif) != NL80211_IFTYPE_AP))
            continue;
        if (!siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan)
            continue;
        vif_sta_channel_num = siwifi_freq_to_channel(siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.chan->center_freq);
        vif_sta_channel_width = siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def.width;
        ctxt_sta = &vif_sta->siwifi_hw->chanctx_table[vif_sta->ch_index];

        vif_channel_width = siwifi_hw->chanctx_table[vif->ch_index].chan_def.width;
        // check bcn channel IE (WLAN_EID_DS_PARAMS)
        bcn = &vif->ap.bcn;
        // channel info is before tim, so it is in bcn->head
        ie_offset = offsetof(struct ieee80211_mgmt, u.beacon.variable);
        ie_ds = cfg80211_find_ie(WLAN_EID_DS_PARAMS, bcn->head + ie_offset, bcn->head_len - ie_offset);
        if (ie_ds) {
            channel_num = *(ie_ds + 2);
            //printk("bcn IE channel %d \n", channel_num);
            if (channel_num != vif_sta_channel_num || vif_channel_width != vif_sta_channel_width) {
                printk("ERROR:vif(%s) hostapd beacon channel %d width %dis diff from channel %d width %d\n",
                        vif->ndev->name, channel_num, vif_channel_width, vif_sta_channel_num, vif_sta_channel_width);
                cfg80211_ch_switch_notify(vif->ndev, &siwifi_hw->chanctx_table[vif_sta->ch_index].chan_def);
                siwifi_chanctx_unlink(vif);
                vif->ch_index = vif_sta->ch_index;
                ctxt_sta->count++;
                siwifi_txq_vif_start(vif, SIWIFI_TXQ_STOP_CHAN, vif->siwifi_hw);
            }
        }
    }
    spin_unlock_bh(&siwifi_hw->cb_lock);
    return 1;
}
