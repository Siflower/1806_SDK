/*
 * =====================================================================================
 *
 *       Filename:  siwifi_mem.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/06/2021 07:29:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _SIWIFI_MEM_H_
#define _SIWIFI_MEM_H_
#include <linux/skbuff.h>

/* When defined this, we will replace the kmalloc/kzalloc with siwifi_kmalloc_l/siwifi_kzalloc_l
 * there are four function which contain memory alloc in wifi driver are not included:
    -alloc_netdev_mqs
    -kmem_cache_alloc
    -dev_alloc_skb
    -dma_pool_alloc
    Those function are special function, I think we can ignore this
 */
//#define MEMORY_USAGE_DEBUG 1


/*internal kmalloc with debug message*/
void *siwifi_kmalloc_l(int size, uint32_t flag, char *debug);

/*internal kzalloc with debug message*/
void* siwifi_kzalloc_l(int size, uint32_t flag, char *debug);

#ifdef MEMORY_USAGE_DEBUG
/*internal siwifi_kfree*/
void siwifi_kfree(const void *data);

#define siwifi_kzalloc(size, flags) \
({  \
    char debug_string[256]; \
    sprintf(debug_string, "%s %d\n", __func__,__LINE__); \
    siwifi_kzalloc_l(size, flags, debug_string);    \
})

#define siwifi_kmalloc(size, flags) \
({  \
    char debug_string[256]; \
    sprintf(debug_string, "%s %d\n", __func__,__LINE__); \
    siwifi_kmalloc_l(size, flags, debug_string);    \
})

#else
#define siwifi_kzalloc kzalloc
#define siwifi_kmalloc kmalloc
#define siwifi_kfree   kfree
#endif

// init debug memory system, allocate debug memory global variable
void siwifi_init_debug_mem(void);

// free all resources allocated in 'siwifi_init_debug_mem', print
// the memory usage information
void siwifi_deinit_debug_mem(void);

// print the memory info when necessary
void siwifi_print_mem_info(void);

#ifdef CONFIG_PRIV_RX_BUFFER_POOL

// allocate rx memory skb
struct sk_buff *siwifi_dev_alloc_rxskb(int size);

// init rx skb pools
// num: how many skbs we allocate
// size : the skb size of each skb
int siwifi_init_private_rxskbs(uint32_t num, uint32_t size);

void siwifi_deinit_private_rxskbs(void);
#else
static inline struct sk_buff *siwifi_dev_alloc_rxskb(int size)
{
    return dev_alloc_skb(size);
};

#endif

void siwifi_set_debug_mem_ctx(void *ctx);

#endif
