/**
 * siwifi_ipc_utils.h
 *
 * IPC utility function declarations
 *
 * Copyright (C) Siflower 2018-2025
 */
#ifndef _SIWIFI_IPC_UTILS_H_
#define _SIWIFI_IPC_UTILS_H_

#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/skbuff.h>

#include "lmac_msg.h"

#ifdef CONFIG_SIWIFI_DBG
/*  #define SIWIFI_DBG(format, arg...) pr_warn(format, ## arg) */
#define SIWIFI_DBG printk
#else
#define SIWIFI_DBG(a...) do {} while (0)
#endif

#ifdef DEBUG_EARLY_SKB
/*  #define SIWIFI_DBG(format, arg...) pr_warn(format, ## arg) */
#define SIWIFI_DBG_EARLY_SKB printk
#else
#define SIWIFI_DBG_EARLY_SKB(a...) do {} while (0)
#endif

#ifdef CONFIG_SIWIFI_DBG_ASSERT
#define SIWIFI_DBG_ASSERT_BLOCK(x) do { \
    if(!x){WARN_ON(1);while(1){}} while (0)
#else
#define SIWIFI_DBG_ASSERT_BLOCK(x) do {} while (0)
#endif

#define SIWIFI_FN_ENTRY_STR ">>> %s(%d)\n", __func__ ,__LINE__
#define SIWIFI_FN_EXIT_STR "<<< %s(%d)\n", __func__ ,__LINE__

enum siwifi_dev_flag {
    SIWIFI_DEV_STACK_RESTARTING,
    SIWIFI_DEV_STARTED,
#ifdef CONFIG_SIWIFI_RF_RECALI
    SIWIFI_DEV_CALIBRATING,
    //indicates hardware is dead,used when reprobe driver if some serious errors occur
    SIWIFI_DEV_HW_DEAD
#endif
};

struct siwifi_hw;

/**
 * struct siwifi_ipc_elem - Generic IPC buffer of fixed size
 *
 * @addr: Host address of the buffer.
 * @dma_addr: DMA address of the buffer.
 */
struct siwifi_ipc_elem {
    void *addr;
    dma_addr_t dma_addr;
};

/**
 * struct siwifi_ipc_elem_pool - Generic pool of IPC buffers of fixed size
 *
 * @nb: Number of buffers currenlty allocated in the pool
 * @buf: Array of buffers (size of array is @nb)
 * @pool: DMA pool in which buffers have been allocated
 */
struct siwifi_ipc_elem_pool {
    int nb;
    struct siwifi_ipc_elem *buf;
    struct dma_pool *pool;
};

/**
 * struct siwifi_ipc_elem - Generic IPC buffer of variable size
 *
 * @addr: Host address of the buffer.
 * @dma_addr: DMA address of the buffer.
 * @size: Size, in bytes, of the buffer
 */
struct siwifi_ipc_elem_var {
    void *addr;
    void *real_addr;
    dma_addr_t dma_addr;
    size_t size;
    size_t real_size;
};

#ifdef CONFIG_ERROR_DUMP
#define ERROR_BUF_MAX_SIZE		512
#define ERROR_INFO_MAX_NUM		20
#define	MAX_TIME_NUM			10
struct error_info {
	char *buf;
	u8 len;
	u8 count;
	u64 time[MAX_TIME_NUM];
};
#endif

/**
 * struct siwifi_ipc_dbgdump_elem - IPC buffer for debug dump
 *
 * @mutex: Mutex to protect access to debug dump
 * @buf: IPC buffer
 */
struct siwifi_ipc_dbgdump_elem {
    struct mutex mutex;
    struct siwifi_ipc_elem_var buf;
#ifdef CONFIG_ERROR_DUMP
	struct error_info *errorinfo[ERROR_INFO_MAX_NUM];
	u32	err_total_count;
	u32 err_cur_idx;
	spinlock_t err_lock;
#endif
#ifdef CONFIG_SF16A18_WIFI_LA_ENABLE
	void *la_mem_addr;
	u32 la_len;
#endif /* CONFIG_SF16A18_WIFI_LA_ENABLE */
};

static const u32 siwifi_rxbuff_pattern = 0xCAFEFADE;

/*
 * Maximum Length of Radiotap header vendor specific data(in bytes)
 */
#define RADIOTAP_HDR_VEND_MAX_LEN   16

/*
 * Maximum Radiotap Header Length without vendor specific data (in bytes)
 */
#define RADIOTAP_HDR_MAX_LEN        80

/*
 * Unsupported HT Frame data length (in bytes)
 */
#define UNSUP_RX_VEC_DATA_LEN       2

/**
 * struct siwifi_ipc_skb_elem - IPC buffer for SKB element
 *
 * @skb: Pointer to the skb buffer allocated
 * @dma_addr: DMA address of the data buffer fo skb
 *
 */
struct siwifi_ipc_skb_elem {
    struct sk_buff *skb;
    dma_addr_t dma_addr;
};

/* Maximum number of rx buffer the fw may use at the same time */
#define SIWIFI_RXBUFF_MAX (64 * NX_REMOTE_STA_MAX)

/**
 * struct siwifi_ipc_rxbuf_elems - IPC buffers for RX
 *
 * @skb: Array of buffer push to FW.
 * @idx: Index of the last pushed skb.(Use to find the next free entry quicker)
 *
 * Note: contrary to softmac version, dma_addr are stored inside skb->cb.
 * (cf &struct siwifi_skb_cb)
 */
struct siwifi_ipc_rxbuf_elems {
    struct sk_buff *skb[SIWIFI_RXBUFF_MAX];
    int idx;
};

/**
 * struct siwifi_skb_cb - Control Buffer structure for RX buffer
 *
 * @dma_addr: DMA address of the data buffer
 * @pattern: Known pattern (used to check pointer on skb)
 * @idx: Index in &struct siwifi_hw.rxbuff_table that contains address of this
 * buffer
 */
struct siwifi_skb_cb {
    dma_addr_t dma_addr;
    uint32_t pattern;
    uint32_t idx;
};

#define SIWIFI_RXBUFF_DMA_ADDR_SET(skbuff, addr)          \
    ((struct siwifi_skb_cb *)(skbuff->cb))->dma_addr = addr
#define SIWIFI_RXBUFF_DMA_ADDR_GET(skbuff)                \
    ((struct siwifi_skb_cb *)(skbuff->cb))->dma_addr

#define SIWIFI_RXBUFF_PATTERN_SET(skbuff, pat)                \
    ((struct siwifi_skb_cb *)(skbuff->cb))->pattern = pat
#define SIWIFI_RXBUFF_PATTERN_GET(skbuff)         \
    ((struct siwifi_skb_cb *)(skbuff->cb))->pattern

#define SIWIFI_RXBUFF_IDX_SET(skbuff, val)                \
    ((struct siwifi_skb_cb *)(skbuff->cb))->idx = val
#define SIWIFI_RXBUFF_IDX_GET(skbuff)             \
    ((struct siwifi_skb_cb *)(skbuff->cb))->idx

#define SIWIFI_RXBUFF_VALID_IDX(idx) ((idx) < SIWIFI_RXBUFF_MAX)

/* Used to ensure that hostid set to fw is never 0 */
#define SIWIFI_RXBUFF_IDX_TO_HOSTID(idx) ((idx) + 1)
#define SIWIFI_RXBUFF_HOSTID_TO_IDX(hostid) ((hostid) - 1)

int siwifi_ipc_rxbuf_elem_allocs(struct siwifi_hw *siwifi_hw);
void siwifi_ipc_rxbuf_elem_pull(struct siwifi_hw *siwifi_hw, struct sk_buff *skb);
void siwifi_ipc_rxbuf_elem_pull_inv(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, bool inv);
void siwifi_ipc_rxbuf_elem_sync(struct siwifi_hw *siwifi_hw, struct sk_buff *skb,
                              int len);
void siwifi_ipc_rxdesc_elem_repush(struct siwifi_hw *siwifi_hw,
                                 struct siwifi_ipc_elem *elem);
void siwifi_ipc_rxbuf_elem_repush(struct siwifi_hw *siwifi_hw,
                                struct sk_buff *skb);

void siwifi_ipc_msg_push(struct siwifi_hw *siwifi_hw, void *msg_buf, uint16_t len);
#ifndef NEW_SCHEDULE
void siwifi_ipc_txdesc_push(struct siwifi_hw *siwifi_hw, void *tx_desc,
                          void *hostid, int hw_queue, int user, uint16_t wmm_param);
#endif

void *siwifi_ipc_fw_trace_desc_get(struct siwifi_hw *siwifi_hw);

int siwifi_ipc_rxbuf_init(struct siwifi_hw *siwifi_hw, uint32_t rx_bufsz);
int siwifi_ipc_init(struct siwifi_hw *siwifi_hw, u8 *shared_ram, void *reg_base);
void siwifi_ipc_deinit(struct siwifi_hw *siwifi_hw);
void siwifi_ipc_start(struct siwifi_hw *siwifi_hw);
void siwifi_ipc_stop(struct siwifi_hw *siwifi_hw);
void siwifi_ipc_tx_drain(struct siwifi_hw *siwifi_hw);
bool siwifi_ipc_tx_pending(struct siwifi_hw *siwifi_hw);

struct ipc_host_env_tag;
int siwifi_ipc_elem_var_allocs(struct siwifi_hw *siwifi_hw,
                             struct siwifi_ipc_elem_var *elem, size_t elem_size,
                             enum dma_data_direction dir,
                             void *buf, const void *init,
                             void (*push)(struct ipc_host_env_tag *, uint32_t),
                             void *real_addr);
void siwifi_ipc_elem_var_deallocs(struct siwifi_hw *siwifi_hw,
                                struct siwifi_ipc_elem_var *elem);
int siwifi_ipc_unsup_rx_vec_elem_allocs(struct siwifi_hw *siwifi_hw,
                                      struct siwifi_ipc_skb_elem *elem);

void siwifi_trigger_restart(struct siwifi_hw *siwifi_hw);
void siwifi_error_ind(struct siwifi_hw *siwifi_hw, bool restart, bool dump_en);
void siwifi_umh_done(struct siwifi_hw *siwifi_hw);

#ifdef CONFIG_ERROR_DUMP
/**
 * siwifi_errorrinfo_allocs() - allocate a memory to save assert info
 * @siwifi_hw: Main driver data
 */
int siwifi_errorinfo_allocs(struct siwifi_hw *siwifi_hw);
/**
 * siwifi_errorinfo_deallocs() - free the memory which save assert info
 * @siwifi_hw: Main driver data
 */
void siwifi_errorinfo_deallocs(struct siwifi_hw *siwifi_hw);
/**
 * siwifi_save_error_info() - save error info to memory
 * @siwifi_hw: Main driver data
 */
void siwifi_save_error_info(struct siwifi_hw *siwifi_hw, char *msg, u8 len);
#endif /*CONFIG_ERROR_DUMP*/

/**
 * check whether the skb is icmp or not
 * @skb: packet for xmit
 * Return 1 for yes, 0 for no
 */
bool siwifi_check_skb_is_icmp(struct sk_buff *skb);

void siwifi_ipc_rxbuf_elem_fail_check(struct siwifi_hw *siwifi_hw);
#endif /* _SIWIFI_IPC_UTILS_H_ */
