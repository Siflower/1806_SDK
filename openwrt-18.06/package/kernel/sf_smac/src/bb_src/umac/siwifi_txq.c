/**
 ******************************************************************************
 *
 * @file siwifi_txq.c
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#include <net/mac80211.h>
#include "siwifi_defs.h"
#include "siwifi_trace.h"
#include "siwifi_tx.h"
#include "ipc_host.h"
#include "siwifi_events.h"
//#include "reg_mac_core.h"

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
#include "siwifi_ioctl.h"
#endif
#include "siwifi_traffic.h"

/* RM#7577 workaround for ixia wmm test
    if be,check whether need to change wmm param for be
    one error,when disable ampdu,credits are always 63,will not change from 0x211 to 0xa43,wmm test will error
    TOOD: modify wmm paramtes to match 80211 spec
*/

#ifdef WORKAROUND_7577
#include "reg_access.h"

#define EDCA_AC_1_ADDR(band) (WIFI_BASE_ADDR(band) + 0x00080000 + 0x0204)
#define WORKAROUND_RM7577(a)   \
{   \
    static unsigned int g_hw_be_cnt = 0;    \
    static unsigned int g_txq_push_cnt = 0; \
    g_txq_push_cnt += a;   \
    if (hwq->id == AC_BE) g_hw_be_cnt += a; \
    if (g_txq_push_cnt >= 1000) {   \
        if (g_hw_be_cnt < 950) {    \
            writel(0xa43, (volatile void *)EDCA_AC_1_ADDR);    \
        } else {    \
            writel(0x211, (volatile void *)EDCA_AC_1_ADDR);    \
        }   \
        g_txq_push_cnt = 0; \
        g_hw_be_cnt = 0;    \
    }   \
}
#endif

#ifdef TOKEN_ENABLE
int tx_descs_num = NUM_TX_DESCS_PER_AC_L;

extern int siwifi_vif_sta_count(struct siwifi_hw *siwifi_hw);
int siwifi_get_num_tx_descs_per_ac(struct siwifi_hw *siwifi_hw)
{
    int sta_num = 0;
    sta_num = siwifi_vif_sta_count(siwifi_hw);
    return (sta_num <= 2) ? NUM_TX_DESCS_PER_AC_L : NUM_TX_DESCS_PER_AC;
}
#endif

//warning don't add () with this macro, otherwise the value will be always be zero
#define LM_TXM_RQ_RATIO 2/3
#define LM_TXC_RQ_RATIO 2/3

/******************************************************************************
 * Utils functions
 *****************************************************************************/
const int nx_tid_prio[NX_NB_TID_PER_STA] = {7, 6, 5, 4, 3, 0, 2, 1};

static inline int siwifi_txq_sta_idx(struct siwifi_sta *sta, u8 tid)
{
    if (is_multicast_sta(sta->sta_idx))
        return NX_FIRST_VIF_TXQ_IDX + sta->vif_idx;
    else
        return (sta->sta_idx * NX_NB_TXQ_PER_STA) + tid;
}

static inline int siwifi_txq_vif_idx(struct siwifi_vif *vif, u8 type)
{
    return NX_FIRST_VIF_TXQ_IDX + master_vif_idx(vif) + (type * NX_VIRT_DEV_MAX);
}

struct siwifi_txq *siwifi_txq_sta_get(struct siwifi_sta *sta, u8 tid,
                                  struct siwifi_hw * siwifi_hw)
{
    if (tid >= NX_NB_TXQ_PER_STA)
        tid = 0;

    return &siwifi_hw->txq[siwifi_txq_sta_idx(sta, tid)];
}

struct siwifi_txq *siwifi_txq_vif_get(struct siwifi_vif *vif, u8 type)
{
    if (type > NX_UNK_TXQ_TYPE)
        type = NX_BCMC_TXQ_TYPE;

    return &vif->siwifi_hw->txq[siwifi_txq_vif_idx(vif, type)];
}

static inline struct siwifi_sta *siwifi_txq_2_sta(struct siwifi_txq *txq)
{
    return txq->sta;
}

/******************************************************************************
 * Init/Deinit functions
 *****************************************************************************/
/**
 * siwifi_txq_init - Initialize a TX queue
 *
 * @txq: TX queue to be initialized
 * @idx: TX queue index
 * @status: TX queue initial status
 * @hwq: Associated HW queue
 * @ndev: Net device this queue belongs to
 *        (may be null for non netdev txq)
 *
 * Each queue is initialized with the credit of @NX_TXQ_INITIAL_CREDITS.
 */
static void siwifi_txq_init(struct siwifi_txq *txq, int idx, u8 status,
                          struct siwifi_hwq *hwq,
                          struct siwifi_sta *sta, struct net_device *ndev,
                          struct siwifi_hw *siwifi_hw
                          )
{
    int i;
	int j;
#ifdef CONFIG_SIWIFI_SOFTMAC
    int txq_tid = tid;
#else
    int txq_tid = (idx < NX_FIRST_VIF_TXQ_IDX) ? (idx - (sta->sta_idx * NX_NB_TXQ_PER_STA)) : 1;
#endif
    txq->idx = idx;
    txq->status = status;
    txq->credits = ((txq_tid == 0) ? NX_TXQ_INITIAL_CREDITS_TID0 : NX_TXQ_INITIAL_CREDITS);
    txq->pkt_sent = 0;
    txq->pkt_send_total = 0;
    txq->pkt_send_success = 0;
    skb_queue_head_init(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
    skb_queue_head_init(&txq->accel_sk_list);
#endif
    txq->last_retry_skb = NULL;
    txq->nb_retry = 0;
    txq->hwq = hwq;
    txq->sta = sta;
    for (i = 0; i < CONFIG_USER_MAX ; i++)
        txq->pkt_pushed[i] = 0;
    txq->push_limit = 0;
#ifdef CONFIG_MAC80211_TXQ
    txq->nb_ready_mac80211 = 0;
#endif
    txq->ps_active_change = 0;
    txq->ps_id = LEGACY_PS_ID;
    if (idx < NX_FIRST_VIF_TXQ_IDX) {
        int sta_idx = sta->sta_idx;
        int tid = idx - (sta_idx * NX_NB_TXQ_PER_STA);
        if (tid < NX_NB_TID_PER_STA)
			txq->ndev_idx = txq->hwq->id % siwifi_hw->tx_queue_num;
        else if (siwifi_hw->tx_queue_num <= 1)
            txq->ndev_idx = SIWIFI_HWQ_BK;
        else if (siwifi_hw->tx_queue_num <= IEEE80211_NUM_ACS)
            txq->ndev_idx = SIWIFI_HWQ_BE;
        else
            txq->ndev_idx = SIWIFI_HWQ_BCMC;

    } else {
        if (siwifi_hw->tx_queue_num > IEEE80211_NUM_ACS) {
            txq->ndev_idx = SIWIFI_HWQ_BCMC;
        } else {
            txq->ndev_idx = txq->hwq->id % siwifi_hw->tx_queue_num;
        }
    }

    //else if (idx < NX_FIRST_UNK_TXQ_IDX) {
	//	txq->ndev_idx = txq->hwq->id % siwifi_hw->tx_queue_num;
    //} else {
    //    if (siwifi_hw->tx_queue_num <= 1)
    //        txq->ndev_idx = SIWIFI_HWQ_BK;
    //    else if (siwifi_hw->tx_queue_num <= IEEE80211_NUM_ACS)
    //        txq->ndev_idx = SIWIFI_HWQ_BE;
    //    else
    //        txq->ndev_idx = SIWIFI_HWQ_BCMC;
    //}

    if (txq->hwq->id == SIWIFI_HWQ_BE)
        txq->atf.enable = 1;

    txq->ndev = ndev;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    txq->amsdu = NULL;
    txq->amsdu_len = 0;
    txq->amsdu_maxnb = 0;
#endif /* CONFIG_SIWIFI_AMSDUS_TX */
	for (j = 0;j < NX_TXQ_CNT;j ++)
	{
		for (i = 0;i < NUM_TX_DESCS_PER_AC;i ++)
			txq->token_pkt_num[j][i] = 0;
	}
    txq->stop_num = 0;
    txq->wake_num = 0;
    // increase txq init time, if the txq init time is changed
    // even txq is active, but we still could not trust all information of txq when skb back
    txq->init_time++;
    txq->ps_on_drop = 0;
    txq->ps_active_drop = 0;
    txq->ps_off_drop = 0;
    txq->siwifi_hw = siwifi_hw;
    txq->stuck_time = 0;
    txq->memctl_balance = 0;
}

#ifdef CONFIG_DEBUG_TXQ_STOP
int g_txq_record_idx = 0;
struct siwifi_txq_record g_txq_record[1024];

void siwifi_trace_txq_stop(uint8_t pos, struct siwifi_txq *txq, struct siwifi_vif *vif)
{
    g_txq_record[g_txq_record_idx].pos = pos;
    g_txq_record[g_txq_record_idx].txq = (uint32_t)(txq);
    g_txq_record[g_txq_record_idx].txqidx = txq->idx;
    g_txq_record[g_txq_record_idx].txqsize = skb_queue_len(&txq->sk_list);
    g_txq_record[g_txq_record_idx].txq_status = txq->status;
    g_txq_record[g_txq_record_idx].memctl_balance = txq->memctl_balance;
    memcpy(&g_txq_record[g_txq_record_idx].memctrl, &(vif->lm_ctl[txq->ndev_idx]), sizeof(struct siwifi_lite_mem_ctrl));
    g_txq_record_idx = (g_txq_record_idx + 1) % 1024;
}

void siwifi_print_txq_trace(void)
{
    int index = g_txq_record_idx;
    int index_max = index + 1024;
    int pos = 0;
    while(index < index_max)
    {
        pos = index % 1024;
        printk("[%d] step %d txq %x idx %d status %4x cnt : %d, size :%d asmdu(cnt: %d size: %d) balance : %d txq size : %d\n",
                pos, g_txq_record[pos].pos, g_txq_record[pos].txq, g_txq_record[pos].txqidx,g_txq_record[pos].txq_status,
                g_txq_record[pos].memctrl.tx_cnt, g_txq_record[pos].memctrl.tx_memory_usage,
                g_txq_record[pos].memctrl.amsdu_tx_cnt, g_txq_record[pos].memctrl.amsdu_tx_memory_usage,
                g_txq_record[pos].memctl_balance,
                g_txq_record[pos].txqsize);
        index++;
    }
}
#define SIWIFI_TRACE_TXQ_STOP siwifi_trace_txq_stop
#else
#define SIWIFI_TRACE_TXQ_STOP(a...) do {} while (0)
#endif //CONFIG_DEBUG_TXQ_STOP

static void siwifi_txq_free_skb(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    struct siwifi_sw_txhdr *hdr = ((struct siwifi_txhdr *)skb->data)->sw_hdr;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    if (hdr->desc.host.flags & TXU_CNTRL_AMSDU) {
        siwifi_free_amsdu(siwifi_hw, hdr);
    }
#endif
    dma_unmap_single(siwifi_hw->dev, hdr->dma_addr, hdr->map_len,
            DMA_TO_DEVICE);

    skb_pull(skb, hdr->headroom);
    siwifi_free_swtxhdr(siwifi_hw, hdr);
    dev_kfree_skb_any(skb);
}

void siwifi_txq_ps_drop_skb(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq)
{
    // drop half of skb in this txq
    struct sk_buff *skb;
    struct siwifi_vif *vif;
#ifdef CONFIG_BRIDGE_ACCELERATE
    int accel_skb_len;
#endif
    int drop_num = 0;
    struct sk_buff_head sk_list_retry;
    skb_queue_head_init(&sk_list_retry);

#ifdef CONFIG_BRIDGE_ACCELERATE
    accel_skb_len = skb_queue_len(&txq->accel_sk_list);
#endif
    BUG_ON(txq->ndev == NULL);
    vif = netdev_priv(txq->ndev);
#ifdef CONFIG_BRIDGE_ACCELERATE
    // first drop accelerate frame, all this frame should data application frame
    if (accel_skb_len > 0) {
        while ((skb = skb_dequeue(&txq->accel_sk_list)) != NULL)
        {
            // Reduce the memroy usage
            vif->lm_ctl[txq->ndev_idx].tx_memory_usage -= skb->truesize;
            vif->lm_ctl[txq->ndev_idx].tx_cnt --;
            siwifi_txq_free_skb(siwifi_hw, skb);
            drop_num++;
            if (drop_num >= (accel_skb_len / 2))
                break;
        }
        txq->ps_on_drop = drop_num;
    }
    // drop the normal skb list, but skip the key skb
    drop_num = 0;
#endif
    while ((skb = skb_dequeue(&txq->sk_list)) != NULL)
    {
        if (txq->nb_retry) {
            txq->nb_retry --;
            skb_queue_head(&sk_list_retry, skb);
            continue;
        }
        // Reduce the memroy usage
        vif->lm_ctl[txq->ndev_idx].tx_memory_usage -= skb->truesize;
        vif->lm_ctl[txq->ndev_idx].tx_cnt --;
        siwifi_txq_free_skb(siwifi_hw, skb);
        drop_num ++;
        if (skb_queue_len(&txq->sk_list) <= SIWIFI_NDEV_FLOW_CTRL_RESTART)
            break;
    }
    while ((skb = skb_dequeue(&sk_list_retry)) != NULL) {
        skb_queue_head(&txq->sk_list, skb);
        txq->nb_retry++;
    }
    if (txq->nb_retry == 0 ||
            !skb_queue_len(&txq->sk_list)) {
        txq->last_retry_skb = NULL;
    }
    WARN_ON(skb_queue_len(&txq->sk_list) > SIWIFI_NDEV_FLOW_CTRL_STOP);
    txq->ps_on_drop += drop_num;

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    if (likely(!siwifi_hw->ate_env.tx_frame_start))
    {
#endif
        SIWIFI_TRACE_TXQ_STOP(1, txq, vif);
        // cancel any flow ctrl bits
        if (txq->status & SIWIFI_TXQ_MEMORY_CTRL) {
            txq->memctl_balance --;
            txq->status &= ~SIWIFI_TXQ_MEMORY_CTRL;
        }
        txq->status &= ~SIWIFI_TXQ_NDEV_FLOW_CTRL;
        netif_wake_subqueue(txq->ndev, txq->ndev_idx);
        trace_txq_flowctrl_restart(txq);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.queues_starts++;
#endif
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    }
#endif
}

extern void siwifi_flush_txq_ref(struct siwifi_hw *siwifi_hw, uint32_t hwq_index, uint32_t txq, const int user_pos);
/**
 * siwifi_txq_flush - Flush all buffers queued for a TXQ
 *
 * @siwifi_hw: main driver data
 * @txq: txq to flush
 */
void siwifi_txq_flush(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq)
{
    struct sk_buff *skb;
    struct sk_buff_head free_list;
    struct siwifi_vif *vif;
	int skb_list_len = 0;

    if (siwifi_txq_has_data(txq)) {
        skb_list_len = skb_queue_len(&txq->sk_list);
        BUG_ON(txq->ndev == NULL);
        vif = netdev_priv(txq->ndev);
        skb_queue_head_init(&free_list);
        skb_queue_splice_init(&txq->sk_list, &free_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
        skb_list_len += skb_queue_len(&txq->accel_sk_list);
        skb_queue_splice_tail_init(&txq->accel_sk_list, &free_list);
#endif
        while ((skb = skb_dequeue(&free_list)) != NULL) {
            // Reduce the memroy usage
            vif->lm_ctl[txq->ndev_idx].tx_memory_usage -= skb->truesize;
            vif->lm_ctl[txq->ndev_idx].tx_cnt --;
            if (unlikely(txq->sta && txq->sta->ps.active))
                txq->sta->ps.pkt_ready[txq->ps_id]--;
            siwifi_txq_free_skb(siwifi_hw, skb);
        }
    }

    txq->last_retry_skb = NULL;
    txq->nb_retry = 0;
    // wakeup the txq if it has been stopped by memory ctrl
    if (((txq->status & (SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL)))
            == (SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL)) {
        vif = netdev_priv(txq->ndev);
        SIWIFI_TRACE_TXQ_STOP(2, txq, vif);
        txq->status &= ~(SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL);
        txq->memctl_balance --;
        netif_wake_subqueue(txq->ndev, txq->ndev_idx);
        trace_txq_flowctrl_restart(txq);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        siwifi_hw->stats.queues_starts++;
#endif
    }

   /* restart netdev queue if number of queued buffer is not zero
     * if number of queued buffer is zero,means is stopped by other txq,will be restarted by other txq
     * */
    if (unlikely(txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL) &&
            skb_list_len > 0
            && !(txq->status & SIWIFI_TXQ_MEMORY_CTRL)
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
            && !siwifi_hw->ate_env.tx_frame_start
#endif
       ) {
        vif = netdev_priv(txq->ndev);
        SIWIFI_TRACE_TXQ_STOP(3, txq, vif);
        txq->status &= ~SIWIFI_TXQ_NDEV_FLOW_CTRL;
        //netif_wake_subqueue(txq->ndev, txq->ndev_idx);
        trace_txq_flowctrl_restart(txq);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        //siwifi_hw->stats.queues_starts++;
#endif
    }
}

/**
 * siwifi_txq_deinit - De-initialize a TX queue
 *
 * @siwifi_hw: Driver main data
 * @txq: TX queue to be de-initialized
 * Any buffer stuck in a queue will be freed.
 */
static void siwifi_txq_deinit(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq)
{
#ifdef TOKEN_ENABLE
	int token_id, hwq_id;
#endif
    if (txq->idx == TXQ_INACTIVE) {
        WARN_ON(siwifi_txq_has_data(txq));
        return;
    }

#ifdef TOKEN_ENABLE
	//when txq deinit, must give token back to hwq if txq has
	for (hwq_id = 0; hwq_id < NX_TXQ_CNT; hwq_id ++)
	{
		for (token_id = 0; token_id < NUM_TX_DESCS_PER_AC; token_id ++)
		{
			if (txq->token_pkt_num[hwq_id][token_id] > 0)
			{
				siwifi_hw->hwq[hwq_id].token_status[token_id] = 0;
				siwifi_hw->hwq[hwq_id].outstanding_tokens--;
			}
		}
	}
#endif
    siwifi_txq_flush(siwifi_hw, txq);
    siwifi_txq_del_from_hw_list(txq);
    siwifi_txq_del_from_stuck_check_list(txq);
    txq->idx = TXQ_INACTIVE;
}

/**
 * siwifi_txq_vif_init - Initialize all TXQ linked to a vif
 *
 * @siwifi_hw: main driver data
 * @siwifi_vif: Pointer on VIF
 * @status: Intial txq status
 *
 * Softmac : 1 VIF TXQ per HWQ
 *
 * Fullmac : 1 VIF TXQ for BC/MC
 *           1 VIF TXQ for MGMT to unknown STA
 */
void siwifi_txq_vif_init(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                       u8 status)
{
    struct siwifi_txq *txq;
    int idx;

    txq = siwifi_txq_vif_get(siwifi_vif, NX_BCMC_TXQ_TYPE);
    idx = siwifi_txq_vif_idx(siwifi_vif, NX_BCMC_TXQ_TYPE);
#if DEBUG_ARRAY_CHECK
    BUG_ON(siwifi_vif->ap.bcmc_index >= NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX);
#endif
    siwifi_txq_init(txq, idx, status, &siwifi_hw->hwq[SIWIFI_HWQ_BE],
                  &siwifi_hw->sta_table[siwifi_vif->ap.bcmc_index], siwifi_vif->ndev, siwifi_hw);
	//update bcmc txq credits to 32
	txq->credits = 32;

    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    idx = siwifi_txq_vif_idx(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_init(txq, idx, status, &siwifi_hw->hwq[SIWIFI_HWQ_VO],
                  NULL, siwifi_vif->ndev, siwifi_hw);

}

/**
 * siwifi_txq_vif_deinit - Deinitialize all TXQ linked to a vif
 *
 * @siwifi_hw: main driver data
 * @siwifi_vif: Pointer on VIF
 */
void siwifi_txq_vif_deinit(struct siwifi_hw * siwifi_hw, struct siwifi_vif *siwifi_vif)
{
    struct siwifi_txq *txq;

    txq = siwifi_txq_vif_get(siwifi_vif, NX_BCMC_TXQ_TYPE);
    siwifi_txq_deinit(siwifi_hw, txq);

    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_deinit(siwifi_hw, txq);

}


/**
 * siwifi_txq_sta_init - Initialize TX queues for a STA
 *
 * @siwifi_hw: Main driver data
 * @siwifi_sta: STA for which tx queues need to be initialized
 * @status: Intial txq status
 *
 * This function initialize all the TXQ associated to a STA.
 * Softmac : 1 TXQ per TID
 *
 * Fullmac : 1 TXQ per TID (limited to 8)
 *           1 TXQ for MGMT
 */
void siwifi_txq_sta_init(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta,
                       u8 status)
{
    struct siwifi_txq *txq;
    int tid, idx;
    struct siwifi_vif *siwifi_vif = siwifi_hw->vif_table[siwifi_sta->vif_idx];
    idx = siwifi_txq_sta_idx(siwifi_sta, 0);

#if DEBUG_ARRAY_CHECK
    BUG_ON(siwifi_sta->vif_idx >= NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX);
#endif
    foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
        siwifi_txq_init(txq, idx, status, &siwifi_hw->hwq[siwifi_tid2hwq[tid]],
                      siwifi_sta, siwifi_vif->ndev, siwifi_hw);
        txq->ps_id = siwifi_sta->uapsd_tids & (1 << tid) ? UAPSD_ID : LEGACY_PS_ID;
        idx++;
    }

}

/**
 * siwifi_txq_sta_deinit - Deinitialize TX queues for a STA
 *
 * @siwifi_hw: Main driver data
 * @siwifi_sta: STA for which tx queues need to be deinitialized
 */
void siwifi_txq_sta_deinit(struct siwifi_hw *siwifi_hw, struct siwifi_sta *siwifi_sta)
{
    struct siwifi_txq *txq;
    int tid;

    foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
        siwifi_txq_deinit(siwifi_hw, txq);
    }
}

/**
 * siwifi_txq_unk_vif_init - Initialize TXQ for unknown STA linked to a vif
 *
 * @siwifi_vif: Pointer on VIF
 */
void siwifi_txq_unk_vif_init(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
    struct siwifi_txq *txq;
    int idx;

    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    idx = siwifi_txq_vif_idx(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_init(txq, idx, 0, &siwifi_hw->hwq[SIWIFI_HWQ_VO], NULL, siwifi_vif->ndev, siwifi_hw);
}

/**
 * siwifi_txq_tdls_vif_deinit - Deinitialize TXQ for unknown STA linked to a vif
 *
 * @siwifi_vif: Pointer on VIF
 */
void siwifi_txq_unk_vif_deinit(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_txq *txq;

    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_deinit(siwifi_vif->siwifi_hw, txq);
}

/**
 * siwifi_init_unk_txq - Initialize TX queue for the transmission on a offchannel
 *
 * @vif: Interface for which the queue has to be initialized
 *
 * NOTE: Offchannel txq is only active for the duration of the ROC
 */
void siwifi_txq_offchan_init(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;
    struct siwifi_txq *txq;

    txq = &siwifi_hw->txq[NX_OFF_CHAN_TXQ_IDX];
    siwifi_txq_init(txq, NX_OFF_CHAN_TXQ_IDX, SIWIFI_TXQ_STOP_CHAN,
                  &siwifi_hw->hwq[SIWIFI_HWQ_VO], NULL, siwifi_vif->ndev, siwifi_hw);
}

/**
 * siwifi_deinit_offchan_txq - Deinitialize TX queue for offchannel
 *
 * @vif: Interface that manages the STA
 *
 * This function deintialize txq for one STA.
 * Any buffer stuck in a queue will be freed.
 */
void siwifi_txq_offchan_deinit(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_txq *txq;

    txq = &siwifi_vif->siwifi_hw->txq[NX_OFF_CHAN_TXQ_IDX];
    siwifi_txq_deinit(siwifi_vif->siwifi_hw, txq);
}


/**
 * siwifi_txq_tdls_vif_init - Initialize TXQ vif for TDLS
 *
 * @siwifi_vif: Pointer on VIF
 */
void siwifi_txq_tdls_vif_init(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;

    if (!(siwifi_hw->wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
        return;

    siwifi_txq_unk_vif_init(siwifi_vif);
}

/**
 * siwifi_txq_tdls_vif_deinit - Deinitialize TXQ vif for TDLS
 *
 * @siwifi_vif: Pointer on VIF
 */
void siwifi_txq_tdls_vif_deinit(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_hw *siwifi_hw = siwifi_vif->siwifi_hw;

    if (!(siwifi_hw->wiphy->flags & WIPHY_FLAG_SUPPORTS_TDLS))
        return;

    siwifi_txq_unk_vif_deinit(siwifi_vif);
}

/******************************************************************************
 * Start/Stop functions
 *****************************************************************************/

void siwifi_txq_add_to_stuck_check_list(struct siwifi_txq *txq)
{
    if (!(txq->status & SIWIFI_TXQ_IN_STUCK_CHECK_LIST))
    {
        WARN_ON(txq->status & SIWIFI_TXQ_IN_HWQ_LIST);
        list_add_tail(&txq->sched_list, &txq->siwifi_hw->stuck_check_list);
        txq->status |= SIWIFI_TXQ_IN_STUCK_CHECK_LIST;
        txq->check_stuck_start = local_clock();
    }
}

void siwifi_txq_del_from_stuck_check_list(struct siwifi_txq *txq)
{
    if (txq->status & SIWIFI_TXQ_IN_STUCK_CHECK_LIST) {
        list_del(&txq->sched_list);
        txq->status &= ~SIWIFI_TXQ_IN_STUCK_CHECK_LIST;
    }
}

/**
 * siwifi_txq_add_to_hw_list - Add TX queue to a HW queue schedule list.
 *
 * @txq: TX queue to add
 *
 * Add the TX queue if not already present in the HW queue list.
 * To be called with tx_lock hold
 */
void _siwifi_txq_add_to_hw_list(struct siwifi_txq *txq, int ps_start)
{
    if (!(txq->status & SIWIFI_TXQ_IN_HWQ_LIST)) {
        trace_txq_add_to_hw(txq);
        // If has been added into check list, remove it
        siwifi_txq_del_from_stuck_check_list(txq);
        txq->status |= SIWIFI_TXQ_IN_HWQ_LIST;
        if (ps_start)
            list_add(&txq->sched_list, &txq->hwq->list);
        else
            list_add_tail(&txq->sched_list, &txq->hwq->list);
        txq->hwq->need_processing = true;
        txq->siwifi_static_last_inhwq = jiffies;
    }
}

void siwifi_txq_add_to_hw_list(struct siwifi_txq *txq)
{
    _siwifi_txq_add_to_hw_list(txq, 0);
}


/**
 * siwifi_txq_del_from_hw_list - Delete TX queue from a HW queue schedule list.
 *
 * @txq: TX queue to delete
 *
 * Remove the TX queue from the HW queue list if present.
 * To be called with tx_lock hold
 */
void siwifi_txq_del_from_hw_list(struct siwifi_txq *txq)
{
    if (txq->status & SIWIFI_TXQ_IN_HWQ_LIST) {
        trace_txq_del_from_hw(txq);
        txq->status &= ~SIWIFI_TXQ_IN_HWQ_LIST;
        list_del(&txq->sched_list);
        // add this txq to global stuck check list
        if (siwifi_txq_has_data(txq)) {
            siwifi_txq_add_to_stuck_check_list(txq);
        }
    }
}

/**
 * siwifi_txq_skb_ready - Check if skb are available for the txq
 *
 * @txq: Pointer on txq
 * @return True if there are buffer ready to be pushed on this txq,
 * false otherwise
 */
static inline bool siwifi_txq_skb_ready(struct siwifi_txq *txq)
{
#ifdef CONFIG_MAC80211_TXQ
    if (txq->nb_ready_mac80211 != NOT_MAC80211_TXQ)
        return ((txq->nb_ready_mac80211 > 0) || !skb_queue_empty(&txq->sk_list));
    else
#endif
#ifdef CONFIG_BRIDGE_ACCELERATE
    return (!skb_queue_empty(&txq->sk_list) || (!skb_queue_empty(&txq->accel_sk_list)));
#else
    return !skb_queue_empty(&txq->sk_list);
#endif
}

/**
 * siwifi_txq_start - Try to Start one TX queue
 *
 * @txq: TX queue to start
 * @reason: reason why the TX queue is started (among SIWIFI_TXQ_STOP_xxx)
 *
 * Re-start the TX queue for one reason.
 * If after this the txq is no longer stopped and some buffers are ready,
 * the TX queue is also added to HW queue list.
 * To be called with tx_lock hold
 */
void siwifi_txq_start(struct siwifi_txq *txq, u16 reason)
{
    BUG_ON(txq == NULL);
    if (txq->idx != TXQ_INACTIVE && (txq->status & reason))
    {
        trace_txq_start(txq, reason);
        txq->status &= ~reason;
        if (!siwifi_txq_is_stopped(txq) && siwifi_txq_skb_ready(txq))
            _siwifi_txq_add_to_hw_list(txq, (reason & SIWIFI_TXQ_STOP_STA_PS));
        txq->siwifi_static_start++;
    }
}

/**
 * siwifi_txq_stop - Stop one TX queue
 *
 * @txq: TX queue to stop
 * @reason: reason why the TX queue is stopped (among SIWIFI_TXQ_STOP_xxx)
 *
 * Stop the TX queue. It will remove the TX queue from HW queue list
 * To be called with tx_lock hold
 */
void siwifi_txq_stop(struct siwifi_txq *txq, u16 reason, u16 pos)
{
    BUG_ON( txq== NULL);
    if (txq->idx != TXQ_INACTIVE)
    {
        trace_txq_stop(txq, reason);
        txq->status |= reason;
        txq->last_stop_pos = pos;
        siwifi_txq_del_from_hw_list(txq);
    }
    txq->siwifi_static_stop++;
}


/**
 * siwifi_txq_sta_start - Start all the TX queue linked to a STA
 *
 * @sta: STA whose TX queues must be re-started
 * @reason: Reason why the TX queue are restarted (among SIWIFI_TXQ_STOP_xxx)
 * @siwifi_hw: Driver main data
 *
 * This function will re-start all the TX queues of the STA for the reason
 * specified. It can be :
 * - SIWIFI_TXQ_STOP_STA_PS: the STA is no longer in power save mode
 * - SIWIFI_TXQ_STOP_VIF_PS: the VIF is in power save mode (p2p absence)
 * - SIWIFI_TXQ_STOP_CHAN: the STA's VIF is now on the current active channel
 *
 * Any TX queue with buffer ready and not Stopped for other reasons, will be
 * added to the HW queue list
 * To be called with tx_lock hold
 */
void siwifi_txq_sta_start(struct siwifi_sta *siwifi_sta, u16 reason, struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq;
    int tid;

    trace_txq_sta_start(siwifi_sta->sta_idx);

    foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
        siwifi_txq_start(txq, reason);
    }
}


/**
 * siwifi_stop_sta_txq - Stop all the TX queue linked to a STA
 *
 * @sta: STA whose TX queues must be stopped
 * @reason: Reason why the TX queue are stopped (among SIWIFI_TX_STOP_xxx)
 * @siwifi_hw: Driver main data
 *
 * This function will stop all the TX queues of the STA for the reason
 * specified. It can be :
 * - SIWIFI_TXQ_STOP_STA_PS: the STA is in power save mode
 * - SIWIFI_TXQ_STOP_VIF_PS: the VIF is in power save mode (p2p absence)
 * - SIWIFI_TXQ_STOP_CHAN: the STA's VIF is not on the current active channel
 *
 * Any TX queue present in a HW queue list will be removed from this list.
 * To be called with tx_lock hold
 */
void siwifi_txq_sta_stop(struct siwifi_sta *siwifi_sta, u16 reason, struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq;
    int tid;

    if (!siwifi_sta)
        return;

    trace_txq_sta_stop(siwifi_sta->sta_idx);
    foreach_sta_txq(siwifi_sta, txq, tid, siwifi_hw) {
        siwifi_txq_stop(txq, reason, SIWIFI_TXQ_STOP_POS_STATION_STOP);
    }
}

void siwifi_txq_tdls_sta_start(struct siwifi_vif *siwifi_vif, u16 reason,
                             struct siwifi_hw *siwifi_hw)
{
    trace_txq_vif_start(siwifi_vif->vif_index);
    spin_lock_bh(&siwifi_hw->tx_lock);

    if (siwifi_vif->sta.tdls_sta)
        siwifi_txq_sta_start(siwifi_vif->sta.tdls_sta, reason, siwifi_hw);

    spin_unlock_bh(&siwifi_hw->tx_lock);
}

void siwifi_txq_tdls_sta_stop(struct siwifi_vif *siwifi_vif, u16 reason,
                            struct siwifi_hw *siwifi_hw)
{
    trace_txq_vif_stop(siwifi_vif->vif_index);

    spin_lock_bh(&siwifi_hw->tx_lock);

    if (siwifi_vif->sta.tdls_sta)
        siwifi_txq_sta_stop(siwifi_vif->sta.tdls_sta, reason, siwifi_hw);

    spin_unlock_bh(&siwifi_hw->tx_lock);
}

static inline
void siwifi_txq_vif_for_each_sta(struct siwifi_hw *siwifi_hw, struct siwifi_vif *siwifi_vif,
                               void (*f)(struct siwifi_sta *, u16, struct siwifi_hw *),
                               u16 reason)
{

    switch (SIWIFI_VIF_TYPE(siwifi_vif)) {
    case NL80211_IFTYPE_STATION:
    case NL80211_IFTYPE_P2P_CLIENT:
    {
        if (siwifi_vif->tdls_status == TDLS_LINK_ACTIVE)
            f(siwifi_vif->sta.tdls_sta, reason, siwifi_hw);
		if (siwifi_vif->sta.ap)
            f(siwifi_vif->sta.ap, reason, siwifi_hw);
        break;
    }
    case NL80211_IFTYPE_AP_VLAN:
        siwifi_vif = siwifi_vif->ap_vlan.master;
    case NL80211_IFTYPE_AP:
    case NL80211_IFTYPE_MESH_POINT:
    case NL80211_IFTYPE_P2P_GO:
    {
        struct siwifi_sta *sta;
        list_for_each_entry(sta, &siwifi_vif->ap.sta_list, list) {
            f(sta, reason, siwifi_hw);
        }
        break;
    }
    default:
        BUG();
        break;
    }
}

/**
 * siwifi_txq_vif_start - START TX queues of all STA associated to the vif
 *                      and vif's TXQ
 *
 * @vif: Interface to start
 * @reason: Start reason (SIWIFI_TXQ_STOP_CHAN or SIWIFI_TXQ_STOP_VIF_PS)
 * @siwifi_hw: Driver main data
 *
 * Iterate over all the STA associated to the vif and re-start them for the
 * reason @reason
 * Take tx_lock
 */
void siwifi_txq_vif_start(struct siwifi_vif *siwifi_vif, u16 reason,
                        struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq;

    trace_txq_vif_start(siwifi_vif->vif_index);

    spin_lock_bh(&siwifi_hw->tx_lock);

    //Reject if monitor interface
    if (siwifi_vif->wdev.iftype == NL80211_IFTYPE_MONITOR)
        goto end;

    if (siwifi_vif->roc_tdls && siwifi_vif->sta.tdls_sta && siwifi_vif->sta.tdls_sta->tdls.chsw_en) {
        siwifi_txq_sta_start(siwifi_vif->sta.tdls_sta, reason, siwifi_hw);
    }
    if (!siwifi_vif->roc_tdls) {
        siwifi_txq_vif_for_each_sta(siwifi_hw, siwifi_vif, siwifi_txq_sta_start, reason);
    }

    txq = siwifi_txq_vif_get(siwifi_vif, NX_BCMC_TXQ_TYPE);
    siwifi_txq_start(txq, reason);
    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_start(txq, reason);

end:

    spin_unlock_bh(&siwifi_hw->tx_lock);
}


/**
 * siwifi_txq_vif_stop - STOP TX queues of all STA associated to the vif
 *
 * @vif: Interface to stop
 * @arg: Stop reason (SIWIFI_TXQ_STOP_CHAN or SIWIFI_TXQ_STOP_VIF_PS)
 * @siwifi_hw: Driver main data
 *
 * Iterate over all the STA associated to the vif and stop them for the
 * reason SIWIFI_TXQ_STOP_CHAN or SIWIFI_TXQ_STOP_VIF_PS
 * Take tx_lock
 */
void siwifi_txq_vif_stop(struct siwifi_vif *siwifi_vif, u16 reason,
                       struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq;

    trace_txq_vif_stop(siwifi_vif->vif_index);
    spin_lock_bh(&siwifi_hw->tx_lock);

    //Reject if monitor interface
    if (siwifi_vif->wdev.iftype == NL80211_IFTYPE_MONITOR)
        goto end;

    siwifi_txq_vif_for_each_sta(siwifi_hw, siwifi_vif, siwifi_txq_sta_stop, reason);

    txq = siwifi_txq_vif_get(siwifi_vif, NX_BCMC_TXQ_TYPE);
    siwifi_txq_stop(txq, reason, SIWIFI_TXQ_STOP_POS_VIF_STOP);
    txq = siwifi_txq_vif_get(siwifi_vif, NX_UNK_TXQ_TYPE);
    siwifi_txq_stop(txq, reason, SIWIFI_TXQ_STOP_POS_VIF_STOP);

end:

    spin_unlock_bh(&siwifi_hw->tx_lock);
}

/**
 * siwifi_start_offchan_txq - START TX queue for offchannel frame
 *
 * @siwifi_hw: Driver main data
 */
void siwifi_txq_offchan_start(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq;

    spin_lock_bh(&siwifi_hw->tx_lock);
    txq = &siwifi_hw->txq[NX_OFF_CHAN_TXQ_IDX];
    siwifi_txq_start(txq, SIWIFI_TXQ_STOP_CHAN);
    spin_unlock_bh(&siwifi_hw->tx_lock);
}

/**
 * siwifi_switch_vif_sta_txq - Associate TXQ linked to a STA to a new vif
 *
 * @sta: STA whose txq must be switched
 * @old_vif: Vif currently associated to the STA (may no longer be active)
 * @new_vif: vif which should be associated to the STA for now on
 *
 * This function will switch the vif (i.e. the netdev) associated to all STA's
 * TXQ. This is used when AP_VLAN interface are created.
 * If one STA is associated to an AP_vlan vif, it will be moved from the master
 * AP vif to the AP_vlan vif.
 * If an AP_vlan vif is removed, then STA will be moved back to mastert AP vif.
 *
 */
void siwifi_txq_sta_switch_vif(struct siwifi_sta *sta, struct siwifi_vif *old_vif,
                             struct siwifi_vif *new_vif)
{
    struct siwifi_hw *siwifi_hw = new_vif->siwifi_hw;
    struct siwifi_txq *txq;
    int i;

    /* start TXQ on the new interface, and update ndev field in txq */
    if (!netif_carrier_ok(new_vif->ndev))
        netif_carrier_on(new_vif->ndev);
    txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);
    for (i = 0; i < NX_NB_TID_PER_STA; i++, txq++) {
        txq->ndev = new_vif->ndev;
        netif_wake_subqueue(txq->ndev, txq->ndev_idx);
    }
}

/******************************************************************************
 * TXQ queue/schedule functions
 *****************************************************************************/

static bool siwifi_txq_check_flow_ctrl(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq, bool retry)
{
    bool stop_ndev_queue = false;
    /* Flowctrl corresponding netdev queue if needed */
    struct siwifi_vif *vif;
    BUG_ON(txq->ndev == NULL);
    vif = netdev_priv(txq->ndev);
    // Total count each our limit or total memory reach the limit
    // stop the queue
    if (siwifi_hw->lm_ctrl_enable &&
            ((vif->lm_ctl[txq->ndev_idx].tx_cnt + vif->lm_ctl[txq->ndev_idx].amsdu_tx_cnt) > vif->lm_ctl[txq->ndev_idx].tx_cnt_limit ||
            (vif->lm_ctl[txq->ndev_idx].tx_memory_usage + vif->lm_ctl[txq->ndev_idx].amsdu_tx_memory_usage) > vif->lm_ctl[txq->ndev_idx].tx_memory_limit) &&
            !(txq->status & SIWIFI_TXQ_MEMORY_CTRL))
    {
        //printk("[band %d]txq[%3d,%p], Memory limit  ON, status : 0x%4x, cnt : %d, size :%d, balance : %d, txq size : %d\n", CONFIG_BAND_TYPE,
        //        txq->idx, txq, txq->status, vif->lm_ctl[txq->ndev_idx].tx_cnt, vif->lm_ctl[txq->ndev_idx].tx_memory_usage, txq->memctl_balance, skb_queue_len(&txq->sk_list));
        SIWIFI_TRACE_TXQ_STOP(4, txq, vif);
        txq->memctl_balance ++;
        txq->status |= SIWIFI_TXQ_MEMORY_CTRL;
        if (!(txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL)) {
            txq->status |= SIWIFI_TXQ_NDEV_FLOW_CTRL;
            netif_stop_subqueue(txq->ndev, txq->ndev_idx);
            trace_txq_flowctrl_stop(txq);
            stop_ndev_queue = true;
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
            siwifi_hw->stats.queues_stops++;
#endif
        }
    }
    /* If too many buffer are queued for this TXQ stop netdev queue */
    BUG_ON(txq->ndev_idx >= siwifi_hw->tx_queue_num);
    if (skb_queue_len(&txq->sk_list) > SIWIFI_NDEV_FLOW_CTRL_STOP
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
            && !siwifi_hw->ate_env.tx_frame_start
#endif
         && !(txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL)
        ) {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
        //siwifi_hw->stats.queues_stops++;
#endif
        SIWIFI_TRACE_TXQ_STOP(5, txq, vif);
        txq->status |= SIWIFI_TXQ_NDEV_FLOW_CTRL;
        //RM#10847 netif_stop_subqueue will stop other txq. Now there is SIWIFI_TXQ_MEMORY_CTRL,so don't be afraid of oom
        //netif_stop_subqueue(txq->ndev, txq->ndev_idx);
        //trace_txq_flowctrl_stop(txq);
        stop_ndev_queue = true;
    }
    return stop_ndev_queue;
}

/**
 * siwifi_txq_queue_skb - Queue a buffer in a TX queue
 *
 * @skb: Buffer to queue
 * @txq: TX Queue in which the buffer must be added
 * @siwifi_hw: Driver main data
 * @retry: Should it be queued in the retry list
 *
 * @return: Retrun 1 if txq has been added to hwq list, 0 otherwise
 *
 * Add a buffer in the buffer list of the TX queue
 * and add this TX queue in the HW queue list if the txq is not stopped.
 * If this is a retry packet it is added after the last retry packet or at the
 * beginning if there is no retry packet queued.
 *
 * If the STA is in PS mode and this is the first packet queued for this txq
 * update TIM.
 *
 * To be called with tx_lock hold
 */
int siwifi_txq_queue_skb(struct sk_buff *skb, struct siwifi_txq *txq,
                       struct siwifi_hw *siwifi_hw,  bool retry)
{
    struct siwifi_vif *vif;
    if (unlikely(txq->sta && txq->sta->ps.active)) {
        // when a station is in power saving, we can not buffering to much frames
        // Fixme : maybe we need to add memory limit for each station instead of this simple
        // txq count limit per txq since it is not unfair for some txq which have
        // much more business in normal user application
#if DEBUG_ARRAY_CHECK
        BUG_ON(txq->ps_id >= 2);
#endif
         if (txq->sta->ps.pkt_ready[txq->ps_id] > SIWIFI_NDEV_PS_ACTIVE_DROP && !retry)
        {
            struct siwifi_sw_txhdr *hdr = ((struct siwifi_txhdr *)skb->data)->sw_hdr;
            //drop the this skb
            if (!hdr->flags.mgmt_frame) {
                txq->ps_active_drop ++;
                if (txq && txq->sta)
                    siwifi_hw->siwifi_static_txinfo[txq->sta->sta_idx].siwifi_static_ps_drop++;
                siwifi_txq_free_skb(siwifi_hw, skb);
                goto out;
            }
        }
        txq->sta->ps.pkt_ready[txq->ps_id]++;
        trace_ps_queue(txq->sta);

        if (txq->sta->ps.pkt_ready[txq->ps_id] == 1) {
            siwifi_set_traffic_status(siwifi_hw, txq->sta, true, txq->ps_id);
        }
    }

    if (!retry) {
        /* add buffer in the sk_list */
#ifdef CONFIG_BRIDGE_ACCELERATE
        // If this skb is accel, put it into accelerate list
        if (siwifi_skb_is_acceled(skb))
            skb_queue_tail(&txq->accel_sk_list, skb);
        else
#endif
            skb_queue_tail(&txq->sk_list, skb);
    } else {
        if (txq->last_retry_skb)
            skb_append(txq->last_retry_skb, skb, &txq->sk_list);
        else
            skb_queue_head(&txq->sk_list, skb);

        txq->last_retry_skb = skb;
        txq->nb_retry++;
    }

    trace_txq_queue_skb(skb, txq, retry);

    vif = netdev_priv(txq->ndev);
    vif->lm_ctl[txq->ndev_idx].tx_memory_usage += skb->truesize;
    vif->lm_ctl[txq->ndev_idx].tx_cnt ++;
    /* Flowctrl corresponding netdev queue if needed */
    siwifi_txq_check_flow_ctrl(siwifi_hw, txq, retry);
    if (txq && txq->sta)
        siwifi_hw->siwifi_static_txinfo[txq->sta->sta_idx].siwifi_static_intxq++;
out:
    /* add it in the hwq list if not stopped and not yet present */
    if (!siwifi_txq_is_stopped(txq)) {
        siwifi_txq_add_to_hw_list(txq);
        return 1;
    } else if (siwifi_txq_has_data(txq) &&
            !(txq->status & SIWIFI_TXQ_IN_HWQ_LIST)) {
        // If this txq is no in hwq list and has data,
        // we need to add it into stuck check list
        siwifi_txq_add_to_stuck_check_list(txq);
    }
    return 0;
}

/**
 * siwifi_txq_confirm_any - Process buffer confirmed by fw
 *
 * @siwifi_hw: Driver main data
 * @txq: TX Queue
 * @hwq: HW Queue
 * @sw_txhdr: software descriptor of the confirmed packet
 *
 * Process a buffer returned by the fw. It doesn't check buffer status
 * and only does systematic counter update:
 * - hw credit
 * - buffer pushed to fw
 *
 * To be called with tx_lock hold
 */
void siwifi_txq_confirm_any(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq,
                          struct siwifi_hwq *hwq, struct siwifi_sw_txhdr *sw_txhdr, int release_hwq)
{
    int user = 0;

#ifdef CONFIG_SIWIFI_MUMIMO_TX
    int group_id;

    user = SIWIFI_MUMIMO_INFO_POS_ID(sw_txhdr->desc.host.mumimo_info);
    group_id = SIWIFI_MUMIMO_INFO_GROUP_ID(sw_txhdr->desc.host.mumimo_info);

    if (txq && (txq->idx != TXQ_INACTIVE) &&
        (txq->pkt_pushed[user] == 1) &&
        (txq->status & SIWIFI_TXQ_STOP_MU_POS))
        siwifi_txq_start(txq, SIWIFI_TXQ_STOP_MU_POS);

#endif /* CONFIG_SIWIFI_MUMIMO_TX */


    if (txq && txq->pkt_pushed[user]) {
        struct siwifi_sta *sta = sw_txhdr->siwifi_sta;
        txq->pkt_pushed[user]--;
        if ((!txq->pkt_pushed) && sta && sta->ps.active && !sta->ps.pkt_ready[txq->ps_id])
            siwifi_set_traffic_status(siwifi_hw, sta, false, txq->ps_id);
    }

    if (release_hwq) {
    	hwq->credits[user]++;
    	hwq->need_processing = true;
        hwq->cfm_cnt ++;
    }
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    siwifi_hw->stats.cfm_balance[hwq->id]--;
#endif
}

/******************************************************************************
 * HWQ processing
 *****************************************************************************/
static inline
bool siwifi_txq_take_mu_lock(struct siwifi_hw *siwifi_hw)
{
    bool res = false;
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    if (siwifi_hw->mod_params->mutx)
        res = (down_trylock(&siwifi_hw->mu.lock) == 0);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
    return res;
}

static inline
void siwifi_txq_release_mu_lock(struct siwifi_hw *siwifi_hw)
{
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    up(&siwifi_hw->mu.lock);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
}

static inline
void siwifi_txq_set_mu_info(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq,
                          int group_id, int pos)
{
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    trace_txq_select_mu_group(txq, group_id, pos);
    if (group_id) {
        txq->mumimo_info = group_id | (pos << 6);
        siwifi_mu_set_active_group(siwifi_hw, group_id);
    } else
        txq->mumimo_info = 0;
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
}

static inline
s16 siwifi_txq_get_credits(struct siwifi_txq *txq)
{
    s16 cred = txq->credits;
    /* if destination is in PS mode, push_limit indicates the maximum
       number of packet that can be pushed on this txq. */
    if (txq->push_limit && (cred > txq->push_limit)) {
        cred = txq->push_limit;
    }
    return cred;
}

/**
 * skb_queue_extract - Extract buffer from skb list
 *
 * @list: List of skb to extract from
 * @head: List of skb to append to
 * @nb_elt: Number of skb to extract
 *
 * extract the first @nb_elt of @list and append them to @head
 * It is assume that:
 * - @list contains more that @nb_elt
 * - There is no need to take @list nor @head lock to modify them
 */
static inline void skb_queue_extract(struct sk_buff_head *list,
                                     struct sk_buff_head *head, int nb_elt)
{
    int i;
    struct sk_buff *first, *last, *ptr;

    first = ptr = list->next;
    for (i = 0; i < nb_elt; i++) {
        ptr = ptr->next;
    }
    last = ptr->prev;

    /* unlink nb_elt in list */
    list->qlen -= nb_elt;
    list->next = ptr;
    ptr->prev = (struct sk_buff *)list;

    /* append nb_elt at end of head */
    head->qlen += nb_elt;
    last->next = (struct sk_buff *)head;
    head->prev->next = first;
    first->prev = head->prev;
    head->prev = last;
}


#ifdef CONFIG_MAC80211_TXQ
/**
 * siwifi_txq_mac80211_dequeue - Dequeue buffer from mac80211 txq and
 *                             add them to push list
 *
 * @siwifi_hw: Main driver data
 * @sk_list: List of buffer to push (initialized without lock)
 * @txq: TXQ to dequeue buffers from
 * @max: Max number of buffer to dequeue
 *
 * Dequeue buffer from mac80211 txq, prepare them for transmission and chain them
 * to the list of buffer to push.
 *
 * @return true if no more buffer are queued in mac80211 txq and false otherwise.
 */
static bool siwifi_txq_mac80211_dequeue(struct siwifi_hw *siwifi_hw,
                                      struct sk_buff_head *sk_list,
                                      struct siwifi_txq *txq, int max)
{
    struct ieee80211_txq *mac_txq;
    struct sk_buff *skb;
    unsigned long mac_txq_len;

    if (txq->nb_ready_mac80211 == NOT_MAC80211_TXQ)
        return true;

    mac_txq = container_of((void *)txq, struct ieee80211_txq, drv_priv);

    for (; max > 0; max--) {
        skb = siwifi_tx_dequeue_prep(siwifi_hw, mac_txq);
        if (skb == NULL)
            return true;

        __skb_queue_tail(sk_list, skb);
    }

    /* re-read mac80211 txq current length.
       It is mainly for debug purpose to trace dropped packet. There is no
       problems to have nb_ready_mac80211 != actual mac80211 txq length */
    ieee80211_txq_get_depth(mac_txq, &mac_txq_len, NULL);
    if (txq->nb_ready_mac80211 > mac_txq_len)
        trace_txq_drop(txq, txq->nb_ready_mac80211 - mac_txq_len);
    txq->nb_ready_mac80211 = mac_txq_len;

    return (txq->nb_ready_mac80211 == 0);
}
#endif

/**
 * siwifi_txq_get_skb_to_push - Get list of buffer to push for one txq
 *
 * @siwifi_hw: main driver data
 * @hwq: HWQ on wich buffers will be pushed
 * @txq: TXQ to get buffers from
 * @user: user postion to use
 * @sk_list_push: list to update
 *
 *
 * This function will returned a list of buffer to push for one txq.
 * It will take into account the number of credit of the HWQ for this user
 * position and TXQ (and push_limit).
 * This allow to get a list that can be pushed without having to test for
 * hwq/txq status after each push
 *
 * If a MU group has been selected for this txq, it will also update the
 * counter for the group
 *
 * @return true if txq no longer have buffer ready after the ones returned.
 *         false otherwise
 */
static
bool siwifi_txq_get_skb_to_push(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq,
                              struct siwifi_txq *txq, int user,
                              struct sk_buff_head *sk_list_push)
{
    bool res = false;
    int nb_ready = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
    int nb_accel = skb_queue_len(&txq->accel_sk_list);
#endif
#ifdef NEW_SCHEDULE
    int credits = hwq->max_skb_per_desc[user];
#else
    int credits = min_t(int, siwifi_txq_get_credits(txq), hwq->credits[user]);
#ifdef TOKEN_ENABLE
	credits = min_t(int, hwq->size / tx_descs_num, credits);
#endif /* TOKEN_ENABLE */
#endif /* NEW_SCHEDULE */

    __skb_queue_head_init(sk_list_push);
    if (hwq->ave_speed.ave_speed_enable) {
        credits = min_t(int, txq->as_ave_pkt, credits);
    }

    if (credits <= 0) {
        if (nb_ready == 0
#ifdef CONFIG_BRIDGE_ACCELERATE
                && nb_accel == 0
#endif
                )
            return true;

        return false;
    }

#ifdef CONFIG_BRIDGE_ACCELERATE
    //FIXME:
    // This has conflicts with CONFIG_MAC80211_TXQ
    /*all skbs could be sent at this time */
    if (credits >= (nb_ready + nb_accel)) {
        skb_queue_splice_init(&txq->sk_list, sk_list_push);
        skb_queue_splice_tail_init(&txq->accel_sk_list, sk_list_push);
        res = true;
        credits = (nb_ready + nb_accel);
    } else {
        if (credits > nb_ready) {
            skb_queue_splice_init(&txq->sk_list, sk_list_push);
            skb_queue_extract(&txq->accel_sk_list, sk_list_push, credits - nb_ready);
        } else if (credits == nb_ready) {
            skb_queue_splice_init(&txq->sk_list, sk_list_push);
        } else {
            skb_queue_extract(&txq->sk_list, sk_list_push, credits);
        }
    }
    if (!res) {
        /* When processing PS service period (i.e. push_limit != 0), no longer
           process this txq if the buffers extracted will complete the SP for
           this txq
         */
        if (txq->push_limit && (credits == txq->push_limit))
            res = true;
    }
#else //CONFIG_BRIDGE_ACCELERATE

    if (credits >= nb_ready) {
        skb_queue_splice_init(&txq->sk_list, sk_list_push);
#ifdef CONFIG_MAC80211_TXQ
        res = siwifi_txq_mac80211_dequeue(siwifi_hw, sk_list_push, txq, credits - nb_ready);
        credits = skb_queue_len(sk_list_push);
#else
        res = true;
        credits = nb_ready;
#endif
    } else {
        skb_queue_extract(&txq->sk_list, sk_list_push, credits);

        /* When processing PS service period (i.e. push_limit != 0), no longer
           process this txq if the buffers extracted will complete the SP for
           this txq */
        if (txq->push_limit && (credits == txq->push_limit))
            res = true;
    }
#endif //CONFIG_BRIDGE_ACCELERATE

    if (hwq->ave_speed.ave_speed_enable) {
        txq->as_ave_pkt -= credits;
        hwq->ave_speed.as_ave_pkt_total -= credits;
        if (!(skb_queue_len(&txq->sk_list) + skb_queue_len(&txq->accel_sk_list))) {
            hwq->ave_speed.as_ave_pkt_total -= txq->as_ave_pkt;
            txq->as_ave_pkt = 0;
        }
    }
    siwifi_mu_set_active_sta(siwifi_hw, siwifi_txq_2_sta(txq), credits);

    return res;
}

/**
 * siwifi_txq_select_user - Select User queue for a txq
 *
 * @siwifi_hw: main driver data
 * @mu_lock: true is MU lock is taken
 * @txq: TXQ to select MU group for
 * @hwq: HWQ for the TXQ
 * @user: Updated with user position selected
 *
 * @return false if it is no possible to process this txq.
 *         true otherwise
 *
 * This function selects the MU group to use for a TXQ.
 * The selection is done as follow:
 *
 * - return immediately for STA that don't belongs to any group and select
 *   group 0 / user 0
 *
 * - If MU tx is disabled (by user mutx_on, or because mu group are being
 *   updated !mu_lock), select group 0 / user 0
 *
 * - Use the best group selected by @siwifi_mu_group_sta_select.
 *
 *   Each time a group is selected (except for the first case where sta
 *   doesn't belongs to a MU group), the function checks that no buffer is
 *   pending for this txq on another user position. If this is the case stop
 *   the txq (SIWIFI_TXQ_STOP_MU_POS) and return false.
 *
 */
static
bool siwifi_txq_select_user(struct siwifi_hw *siwifi_hw, bool mu_lock,
                          struct siwifi_txq *txq, struct siwifi_hwq *hwq, int *user)
{
    int pos = 0;
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    int id, group_id = 0;
    struct siwifi_sta *sta = siwifi_txq_2_sta(txq);

    /* for sta that belong to no group return immediately */
    if (!sta || !sta->group_info.cnt)
        goto end;

    /* If MU is disabled, need to check user */
    if (!siwifi_hw->mod_params->mutx_on || !mu_lock)
        goto check_user;

    /* Use the "best" group selected */
    group_id = sta->group_info.group;

    if (group_id > 0)
        pos = siwifi_mu_group_sta_get_pos(siwifi_hw, sta, group_id);

  check_user:
    /* check that we can push on this user position */
#if CONFIG_USER_MAX == 2
    id = (pos + 1) & 0x1;
    if (txq->pkt_pushed[id]) {
        siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_MU_POS, SIWIFI_TXQ_STOP_POS_SELECT_USER);
        return false;
    }

#else
    for (id = 0 ; id < CONFIG_USER_MAX ; id++) {
        if (id != pos && txq->pkt_pushed[id]) {
            siwifi_txq_stop(txq, SIWIFI_TXQ_STOP_MU_POS, SIWIFI_TXQ_STOP_POS_SELECT_USER);
            return false;
        }
    }
#endif

  end:
    siwifi_txq_set_mu_info(siwifi_hw, txq, group_id, pos);
#endif /* CONFIG_SIWIFI_MUMIMO_TX */

    *user = pos;
    return true;
}

#ifdef TOKEN_ENABLE
/**
 * siwifi_hwq_get_token_id - get token before push skb to hardware
 * @return -1 : there is no token left
 *          1 : refuse to give token because of atf strategy
 *          0 : get token normally
 */
static int siwifi_hwq_get_token_id(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq, struct siwifi_txq *txq, int *token)
{
    int i;

    // Init the token, -1 means invalid
    *token = -1;
    /* If the number of outstanding Tx tokens is greater than
     * NUM_TX_DESCS_PER_AC we try and encourage aggregation to the max size
     * supported
     */
    if (hwq->outstanding_tokens >= tx_descs_num)
        return -1;

    /* Find if any available token we can get*/
    for (i = 0; i < tx_descs_num; i++)
    {
        if (hwq->token_status[i] == 0)
        {
            *token = i;
            break;
        }
    }

    // No tokens
    if (*token == -1)
        return -1;

    //if sta is new, let it get token anyway
    if (txq->atf.record_rateinfo != 0 && txq->atf.enable && siwifi_hw->atf.enable) {
        txq->atf.addup_rateinfo += txq->atf.record_rateinfo;
        if (txq->atf.addup_rateinfo < siwifi_hw->atf.max_rateinfo){
            *token == -1;
            txq->atf.debug_skip_token_cnt++;
            return 1;
        } else {
            txq->atf.addup_rateinfo -= siwifi_hw->atf.max_rateinfo;
            txq->atf.debug_get_token_cnt++;
            if (txq->atf.have_sent == 0) {
                txq->atf.have_sent = 1;
                siwifi_hw->atf.txq_nosent_cnt--;
            }
        }
    }

    hwq->token_status[*token] = 1;
    hwq->outstanding_tokens++;

    txq->token_pkt_num[hwq->id][*token] = 0;
    return 0;
}

static void siwifi_txq_tokens_record(struct siwifi_hwq *hwq, struct siwifi_txq *txq, int token_id)
{
#if DEBUG_ARRAY_CHECK
    BUG_ON(hwq->id >= NX_TXQ_CNT);
    BUG_ON(token_id >= NUM_TX_DESCS_PER_AC);
    BUG_ON(txq->token_pkt_num[hwq->id][token_id] > 32);
    BUG_ON(hwq->current_record_num >= NUM_PER_HWQ_RECORD);
#endif

    hwq->ampdu_num[txq->token_pkt_num[hwq->id][token_id] - 1]++;
    hwq->record_txq[hwq->current_record_num] = txq->idx;
    hwq->record_txq_ready[hwq->current_record_num] = skb_queue_len(&txq->sk_list);
#ifdef CONFIG_BRIDGE_ACCELERATE
    hwq->record_accel_txq_ready[hwq->current_record_num] = skb_queue_len(&txq->accel_sk_list);
#endif
    hwq->record_txq_ready[hwq->current_record_num] = skb_queue_len(&txq->sk_list);
    hwq->record_ampdu_num[hwq->current_record_num] = txq->token_pkt_num[hwq->id][token_id];
    hwq->record_hwq_credit[hwq->current_record_num] = hwq->credits[0];
    hwq->record_txq_credit[hwq->current_record_num] = siwifi_txq_get_credits(txq);
    hwq->current_record_num++;
    if (hwq->current_record_num == NUM_PER_HWQ_RECORD)
        hwq->current_record_num = 0;
}
#endif /*TOKEN_ENABLE*/

/**
 * siwifi_hwq_process - Process one HW queue list
 *
 * @siwifi_hw: Driver main data
 * @hw_queue: HW queue index to process
 *
 * The function will iterate over all the TX queues linked in this HW queue
 * list. For each TX queue, push as many buffers as possible in the HW queue.
 * (NB: TX queue have at least 1 buffer, otherwise it wouldn't be in the list)
 * - If TX queue no longer have buffer, remove it from the list and check next
 *   TX queue
 * - If TX queue no longer have credits or has a push_limit (PS mode) and it
 *   is reached , remove it from the list and check next TX queue
 * - If HW queue is full, update list head to start with the next TX queue on
 *   next call if current TX queue already pushed "too many" pkt in a row, and
 *   return
 *
 * To be called when HW queue list is modified:
 * - when a buffer is pushed on a TX queue
 * - when new credits are received
 * - when a STA returns from Power Save mode or receives traffic request.
 * - when Channel context change
 *
 * To be called with tx_lock hold
 */
#define ALL_HWQ_MASK  ((1 << CONFIG_USER_MAX) - 1)

void siwifi_hwq_process(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq)
{
    struct siwifi_txq *txq, *next;
    int user, credit_map = 0;
    bool mu_enable;
#ifndef NEW_SCHEDULE
    int push_success = 0;
    int pushed_total = 0;
#endif

#ifdef NEW_SCHEDULE
    int i = 0;
    uint32_t total_credits = 0;
#endif
    uint32_t as_ave_pkt_total_now = 0;
    int siwifi_static_break_reason = 0;
    trace_process_hw_queue(hwq);
    siwifi_hw->siwifi_static_hwq_process++;

#ifdef NEW_SCHEDULE
    for (; i < CONFIG_USER_MAX; i++)
        total_credits |= hwq->credits[i];

    if (!total_credits)
        return;
#endif

    hwq->need_processing = false;

    mu_enable = siwifi_txq_take_mu_lock(siwifi_hw);
    if (!mu_enable)
        credit_map = ALL_HWQ_MASK - 1;

    if (siwifi_hw->atf.enable) {
        uint32_t hwq_max_rateinfo = 0;
        list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
            if (txq->atf.enable == 0)
                continue;
            if (txq->atf.record_rateinfo > hwq_max_rateinfo)
                hwq_max_rateinfo = txq->atf.record_rateinfo;
        }
        siwifi_hw->atf.max_rateinfo = hwq_max_rateinfo;
    }

     hwq->ave_speed.as_txq_cnt = 0;
    list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
        hwq->ave_speed.as_txq_cnt++;
        as_ave_pkt_total_now += txq->as_ave_pkt;
    }
    if ((hwq->ave_speed.as_txq_cnt > siwifi_hw->ave_speed_cnt_thres) && !hwq->ave_speed.ave_speed_enable)
        hwq->ave_speed.ave_speed_enable = 1;
    if (!as_ave_pkt_total_now) {
        if (hwq->ave_speed.as_txq_cnt > siwifi_hw->ave_speed_cnt_thres)
            hwq->ave_speed.ave_speed_enable = 1;
        else
            hwq->ave_speed.ave_speed_enable = 0;
        hwq->ave_speed.as_ave_pkt_total = 0;
    }

    if (hwq->ave_speed.ave_speed_enable && !hwq->ave_speed.as_ave_pkt_total) {
        hwq->ave_speed.as_ave_pkt_total  = 0;
        hwq->ave_speed.as_rate_total = 0;
        hwq->ave_speed.as_pkt_total  = 0;

        list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
            hwq->ave_speed.as_rate_total += txq->atf.record_rateinfo;
            hwq->ave_speed.as_pkt_total  += (skb_queue_len(&txq->sk_list) + skb_queue_len(&txq->accel_sk_list));
        }

        list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
            if (!hwq->ave_speed.as_rate_total) {
                txq->as_ave_pkt = siwifi_hw->ave_speed_credits_low;
            } else {
                txq->as_ave_pkt = hwq->ave_speed.as_pkt_total * txq->atf.record_rateinfo / hwq->ave_speed.as_rate_total;
            }
            if (txq->as_ave_pkt < siwifi_hw->ave_speed_credits_low)
                txq->as_ave_pkt = siwifi_hw->ave_speed_credits_low;
            if (txq->as_ave_pkt > siwifi_hw->ave_speed_credits_up)
                txq->as_ave_pkt = siwifi_hw->ave_speed_credits_up;
            hwq->ave_speed.as_ave_pkt_total += txq->as_ave_pkt;
        }
    }

    list_for_each_entry_safe (txq, next, &hwq->list, sched_list) {
        if (jiffies_to_msecs(jiffies - txq->siwifi_static_last_process) > 3000) {
            list_del(&txq->sched_list);
            list_add(&txq->sched_list, &txq->hwq->list);
            txq->siwifi_static_pro_reorder++;
        }
        txq->siwifi_static_pro_in_hwq++;
    }

	list_for_each_entry_safe(txq, next, &hwq->list, sched_list) {
		struct sk_buff_head sk_list_push;
#ifndef NEW_SCHEDULE
        struct siwifi_txhdr *txhdr = NULL;
		struct sk_buff *skb;
#endif
#ifdef TOKEN_ENABLE
        int token_id = -1;
        int ret = -1;
#endif
		bool txq_empty;
        struct siwifi_vif *vif;

		trace_process_txq(txq);
        txq->siwifi_static_process++;

		/* sanity check for debug */
		BUG_ON(!(txq->status & SIWIFI_TXQ_IN_HWQ_LIST));
		BUG_ON(txq->idx == TXQ_INACTIVE);
		BUG_ON(txq->credits <= 0);
		BUG_ON(!siwifi_txq_skb_ready(txq));
        vif = netdev_priv(txq->ndev);

		if (!siwifi_txq_select_user(siwifi_hw, mu_enable, txq, hwq, &user))
			continue;

		if (!hwq->credits[user]) {
			credit_map |= BIT(user);
			if (credit_map == ALL_HWQ_MASK){
                siwifi_static_break_reason = 1;
                break;
            }
		}
#ifdef TOKEN_ENABLE
        // Before get skb list to push ,we check if this hwq has free token ids
        // If there are no free token ids, which means this ac is busy, still have pending
        // skb to be processed or transmitted by lmac, so break here, encourage aggregation

		//must use txq->hwq->id, can not use hwq->id, txq->hwq may have been changed
  if (!hwq->ave_speed.ave_speed_enable) {
#if DEBUG_ARRAY_CHECK
            BUG_ON(txq->hwq->id >= NX_TXQ_CNT);
#endif
            ret = siwifi_hwq_get_token_id(siwifi_hw, &siwifi_hw->hwq[txq->hwq->id], txq, &token_id);
            if (ret < 0) {
                // set pushed_total as zero, to make need_processing be false
                pushed_total = 0;
                siwifi_static_break_reason = 2;
                break;
            }
            if (ret == 1)
                continue;
        }
#endif /* TOKEN_ENABLE */


        BUG_ON(hwq != txq->hwq);
		txq_empty = siwifi_txq_get_skb_to_push(siwifi_hw, hwq, txq, user,
				&sk_list_push);
#ifdef NEW_SCHEDULE
        if (skb_queue_len(&sk_list_push) > 0) {
            if (siwifi_tx_push_burst(siwifi_hw, hwq, txq, &sk_list_push)) {
                printk("siwifi_tx_push_burst failed");
            } else {
                hwq->credits[user]--;
                //RM#7577 workaround for ixia wmm test
                //if be,check whether need to change wmm param for be
                //one error,when disable ampdu,credits are always 63,will not change from 0x211 to 0xa43,wmm test will error
                //TOOD: modify wmm paramtes to match 80211 spec
#ifdef CONFIG_SIWIFI_AMSDUS_TX
                if (txhdr->sw_hdr->amsdu.nb > 0)
                    traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, txhdr->sw_hdr->amsdu.nb);
                else
                    traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, 1);
#else
                traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, 1);
#endif
            }
        }
#else //NEW_SCHEDULE
		while ((skb = __skb_dequeue(&sk_list_push)) != NULL) {
            txq->siwifi_static_last_process = jiffies;
			txhdr = (struct siwifi_txhdr *)skb->data;
			BUG_ON(skb != txhdr->sw_hdr->skb);
			pushed_total++;
            vif->lm_ctl[txq->ndev_idx].tx_memory_usage -= skb->truesize;
            vif->lm_ctl[txq->ndev_idx].tx_cnt --;
            //RM#7577 workaround for ixia wmm test
#ifdef CONFIG_SIWIFI_AMSDUS_TX
            if (txhdr->sw_hdr->amsdu.nb > 0)
                traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, txhdr->sw_hdr->amsdu.nb);
            else
                traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, 1);
#else
            traffic_detect_be_edca(siwifi_hw, txhdr->sw_hdr->siwifi_vif, hwq->id, 1);
#endif
            if (hwq->ave_speed.ave_speed_enable)
                txq->as_enable_pkt++;
            else
                txq->as_disable_pkt++;

            if (!siwifi_tx_push(siwifi_hw, txhdr, 0))
            {
#ifdef TOKEN_ENABLE
				txhdr->token_id = token_id;
                if (!hwq->ave_speed.ave_speed_enable) {

#if DEBUG_ARRAY_CHECK
                BUG_ON(txhdr->sw_hdr->txq->hwq->id >= NX_TXQ_CNT);
                BUG_ON(token_id >= NUM_TX_DESCS_PER_AC);
                }
#endif

				//must use txhdr->sw_hdr->txq->hwq->id,can not use hwq->id,txq->hwq may have been changed
				txq->token_pkt_num[txhdr->sw_hdr->txq->hwq->id][token_id] ++;
#endif /*TOKEN_ENABLE*/
				txhdr->sw_hdr->txq->hwq->push_cnt ++;
				push_success++;
			}
		}
#ifdef TOKEN_ENABLE
       if (!hwq->ave_speed.ave_speed_enable && ((token_id > 0) && token_id < NUM_TX_DESCS_PER_AC))
            siwifi_txq_tokens_record(hwq, txq, token_id);
#endif
#endif /*NEW_SCHEDULE*/
		if (txq_empty) {
			siwifi_txq_del_from_hw_list(txq);
			txq->pkt_sent = 0;
        } else if ((hwq->credits[user] == 0) &&
                   siwifi_txq_is_scheduled(txq)) {
            /* txq not empty,
               - To avoid starving need to process other txq in the list
               - For better aggregation, need to send "as many consecutive
               pkt as possible" for the same txq
               ==> Add counter to trigger txq switch
            */
            if (txq->pkt_sent > hwq->size) {
                txq->pkt_sent = 0;
                list_rotate_left(&hwq->list);
            }
        }

        /* Unable to complete PS traffic request because of hwq credit */
        if (txq->push_limit && txq->sta) {
            if (txq->ps_id == LEGACY_PS_ID) {
                /* for legacy PS abort SP and wait next ps-poll */
                txq->sta->ps.sp_cnt[txq->ps_id] -= txq->push_limit;
                txq->push_limit = 0;
            }
            /* for u-apsd need to complete the SP to send EOSP frame */
        }

        if ((txq->status & (SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL))
                    == (SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL)) {
            // Check if Memory now is enough now, restart the netdevice queue
            if ((vif->lm_ctl[txq->ndev_idx].tx_memory_usage + vif->lm_ctl[txq->ndev_idx].amsdu_tx_memory_usage) <= (vif->lm_ctl[txq->ndev_idx].tx_memory_limit * LM_TXM_RQ_RATIO) &&
                    (vif->lm_ctl[txq->ndev_idx].tx_cnt + vif->lm_ctl[txq->ndev_idx].amsdu_tx_cnt) <= (vif->lm_ctl[txq->ndev_idx].tx_cnt_limit * LM_TXC_RQ_RATIO))
            {
                //printk("[band %d]txq[%3d,%p], Memory limit OFF, status : 0x%4x, cnt : %d, size :%d, balance : %d, txq size : %d\n", CONFIG_BAND_TYPE,
                //        txq->idx, txq, txq->status, vif->lm_ctl[txq->ndev_idx].tx_cnt, vif->lm_ctl[txq->ndev_idx].tx_memory_usage, txq->memctl_balance, skb_queue_len(&txq->sk_list));
                SIWIFI_TRACE_TXQ_STOP(6, txq, vif);
                txq->status &= ~(SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL);
                txq->memctl_balance --;
                netif_wake_subqueue(txq->ndev, txq->ndev_idx);
                trace_txq_flowctrl_restart(txq);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
                siwifi_hw->stats.queues_starts++;
#endif
            }
            // There is no skb in this txq, clear the status, wakeup subqueue whenever
            else if (!siwifi_txq_has_data(txq)) {
                //printk("[band %d]txq[%3d,%p], Memory limit OFF, status : 0x%4x, cnt : %d, size :%d, balance : %d, txq empty\n", CONFIG_BAND_TYPE,
                //        txq->idx, txq, txq->status, vif->lm_ctl[txq->ndev_idx].tx_cnt, vif->lm_ctl[txq->ndev_idx].tx_memory_usage, txq->memctl_balance);
                SIWIFI_TRACE_TXQ_STOP(7, txq, vif);
                txq->status &= ~(SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL);
                txq->memctl_balance --;
                // If this txq has flag SIWIFI_TXQ_NDEV_FLOW_CTRL | SIWIFI_TXQ_MEMORY_CTRL,
                // it must has stopped the ndevq, so here make the queues_starts be correct.
                netif_wake_subqueue(txq->ndev, txq->ndev_idx);
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
                siwifi_hw->stats.queues_starts++;
#endif
            }

        }

        /* restart netdev queue if number of queued buffer is below threshold */
        if (unlikely(txq->status & SIWIFI_TXQ_NDEV_FLOW_CTRL) &&
            skb_queue_len(&txq->sk_list) < SIWIFI_NDEV_FLOW_CTRL_RESTART
            && !(txq->status & SIWIFI_TXQ_MEMORY_CTRL)
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
            && !siwifi_hw->ate_env.tx_frame_start
#endif
            ) {
            SIWIFI_TRACE_TXQ_STOP(8, txq, vif);
            txq->status &= ~SIWIFI_TXQ_NDEV_FLOW_CTRL;
            trace_txq_flowctrl_restart(txq);
        }
        if (hwq->ave_speed.ave_speed_enable && !hwq->ave_speed.as_ave_pkt_total) {
            hwq->ave_speed.ave_speed_enable = 0;
            break;
        }
    }

#ifndef NEW_SCHEDULE
    if (pushed_total != 0 && push_success == 0) {
        SIWIFI_DBG("set need_processing(%d) case all pushed fail pushed_total=%d\n",
                siwifi_hw->mod_params->is_hb, pushed_total);
        hwq->need_processing = true;
    }
#endif

    if (mu_enable)
        siwifi_txq_release_mu_lock(siwifi_hw);
     siwifi_hw->siwifi_static_break_reason[siwifi_static_break_reason]++;
}

//40 senconds
#define SIWIFI_STUCK_CHECK_MAX_TIME (40000000000)
#define SWIFI_STUCK_CHECK_TIMES (512)
void siwifi_txq_stuck_list_process(struct siwifi_hw *siwifi_hw)
{
    struct siwifi_txq *txq, *next;
    // Get current time
    u64 now = local_clock();
    list_for_each_entry_safe(txq, next, &siwifi_hw->stuck_check_list, sched_list) {
        WARN_ON(!(txq->status & SIWIFI_TXQ_IN_STUCK_CHECK_LIST));
        WARN_ON((txq->status & SIWIFI_TXQ_IN_HWQ_LIST));
        //WARN_ON(!siwifi_txq_has_data(txq));
        if (!siwifi_txq_has_data(txq)) {
            siwifi_txq_del_from_stuck_check_list(txq);
            continue;
        }
        if (!time_after64(now, txq->check_stuck_start))
        {
            printk("time range over 64\n");
            txq->check_stuck_start = 0;
        }
        // the txq is not processed over than SIWIFI_STUCK_CHECK_MAX_TIME
        // must be something wrong, we need to flush this txq
        if ((now - txq->check_stuck_start) > SIWIFI_STUCK_CHECK_MAX_TIME)
        {
            //printk("[Warning] txq[%d] stucked, flush it\n", txq->idx);
            //#ifdef CONFIG_BRIDGE_ACCELERATE
            //printk("txq status :0x%x, sklist : %d, accel list : %d, credits :%d, push limit %d\n",
            //        txq->status, skb_queue_len(&txq->sk_list), skb_queue_len(&txq->accel_sk_list), txq->credits, txq->push_limit);
            //#else
            //printk("txq status :0x%x, sklist : %d, credits :%d, push limit %d\n",
            //        txq->status, skb_queue_len(&txq->sk_list), txq->credits, txq->push_limit);
            //#endif
            siwifi_txq_flush(siwifi_hw, txq);
            siwifi_txq_del_from_stuck_check_list(txq);
            txq->stuck_time ++;
            // clear any other information on this station ?? for example, ps active???
            //if (txq->status & SIWIFI_TXQ_STOP_STA_PS) {
            //}
        }
    }
}

/**
 * siwifi_hwq_process_all - Process all HW queue list
 *
 * @siwifi_hw: Driver main data
 *
 * Loop over all HWQ, and process them if needed
 * To be called with tx_lock hold
 */
void siwifi_hwq_process_all(struct siwifi_hw *siwifi_hw)
{
    int id;

    siwifi_mu_group_sta_select(siwifi_hw);

    for (id = ARRAY_SIZE(siwifi_hw->hwq) - 1; id >= 0 ; id--) {
        if (siwifi_hw->hwq[id].need_processing) {
            siwifi_hwq_process(siwifi_hw, &siwifi_hw->hwq[id]);
        }
    }

    if (((++siwifi_hw->stuck_check_time) % SWIFI_STUCK_CHECK_TIMES) == 0
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
     && likely(!siwifi_hw->ate_env.tx_frame_start)
#endif
        )
        // Process stuck list check to see if any txq blocked
        siwifi_txq_stuck_list_process(siwifi_hw);

}

/**
 * siwifi_hwq_init - Initialize all hwq structures
 *
 * @siwifi_hw: Driver main data
 *
 */
void siwifi_hwq_init(struct siwifi_hw *siwifi_hw)
{
    int i, j;
#ifdef TOKEN_ENABLE
    tx_descs_num = NUM_TX_DESCS_PER_AC_L;
#endif
#ifdef CONFIG_DEBUG_TXQ_STOP
    memset(g_txq_record, 0 ,sizeof(g_txq_record));
#endif
    for (i = 0; i < ARRAY_SIZE(siwifi_hw->hwq); i++) {
        struct siwifi_hwq *hwq = &siwifi_hw->hwq[i];

        for (j = 0 ; j < CONFIG_USER_MAX; j++) {
            hwq->credits[j] = nx_txdesc_cnt[i];
#ifdef NEW_SCHEDULE
            hwq->max_skb_per_desc[j] = nx_max_skb_txdesc_cnt[i] ;
#endif
        }
        hwq->id = i;
        hwq->size = nx_txdesc_cnt[i];
        INIT_LIST_HEAD(&hwq->list);
		siwifi_hw->hwq_credits_dec[i] = 0;

#ifdef TOKEN_ENABLE
		hwq->outstanding_tokens = 0;
		for (j = 0; j < NUM_TX_DESCS_PER_AC; j++)
			hwq->token_status[j] = 0;

		for (j = 0; j < NUM_PER_HWQ_RECORD; j++) {
			hwq->record_txq[j] = 0;
            hwq->record_txq_ready[j] = 0;
        }

		hwq->current_record_num = 0;
#endif
		hwq->cfm_cnt = 0;
		hwq->push_cnt = 0;
        hwq->ave_speed.ave_speed_enable = 0;
    }
}

/**
 * siwifi_adjust_hwq_credits - adjust all hwq credits by credit_inc
 *
 * @siwifi_hw: Driver main data
 * @credit_dec: credit decress by each hwq, -1 will reduce the min hwq by its (hwq.credits -1)
 * 0 will restore the credits by init value
 * To be called with tx_lock hold
 *
 */
int siwifi_adjust_hwq_credits(struct siwifi_hw *siwifi_hw,s16 credit_dec)
{
#ifdef NEW_SCHEDULE
    //TODO
    return 0;
#else
    int ret = 0;
    int i, j;
    s16 dec_credit = 0;

    if (credit_dec < -1)
        return -1;

    for (i = 0; i < ARRAY_SIZE(siwifi_hw->hwq); i++) {
        struct siwifi_hwq *hwq = &siwifi_hw->hwq[i];
		if (credit_dec == -1) {
			dec_credit = nx_txdesc_cnt[i] - 1;
	    } else {
			dec_credit = (credit_dec < (nx_txdesc_cnt[i] - 1)) ? credit_dec : (nx_txdesc_cnt[i] - 1);
		}
        for (j = 0; j < CONFIG_USER_MAX; j++)
            hwq->credits[j] -= (dec_credit - siwifi_hw->hwq_credits_dec[i]);
		siwifi_hw->hwq_credits_dec[i] = dec_credit;
    }
    return ret;
#endif
}
