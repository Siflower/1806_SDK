/**
 ****************************************************************************************
 *
 * @file siwifi_txq.h
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */
#ifndef _SIWIFI_TXQ_H_
#define _SIWIFI_TXQ_H_

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/ieee80211.h>

/**
 * Fullmac TXQ configuration:
 *  - STA: 1 TXQ per TID (limited to 8)
 *         1 TXQ for bufferable MGT frames
 *  - VIF: 1 TXQ for Multi/Broadcast +
 *         1 TXQ for MGT for unknown STAs or non-bufferable MGT frames
 *  - 1 TXQ for offchannel transmissions
 *
 *
 * Txq mapping looks like
 * for NX_REMOTE_STA_MAX=10 and NX_VIRT_DEV_MAX=4
 *
 * | TXQ | NDEV_ID | VIF |   STA |  TID | HWQ |
 * |-----+---------+-----+-------+------+-----|-
 * |   0 |       0 |     |     0 |    0 |   1 | 9 TXQ per STA
 * |   1 |       1 |     |     0 |    1 |   0 | (8 data + 1 mgmt)
 * |   2 |       2 |     |     0 |    2 |   0 |
 * |   3 |       3 |     |     0 |    3 |   1 |
 * |   4 |       4 |     |     0 |    4 |   2 |
 * |   5 |       5 |     |     0 |    5 |   2 |
 * |   6 |       6 |     |     0 |    6 |   3 |
 * |   7 |       7 |     |     0 |    7 |   3 |
 * |   8 |     N/A |     |     0 | MGMT |   3 |
 * |-----+---------+-----+-------+------+-----|-
 * | ... |         |     |       |      |     | Same for all STAs
 * |-----+---------+-----+-------+------+-----|-
 * |  90 |      80 |   0 | BC/MC |    0 | 1/4 | 1 TXQ for BC/MC per VIF
 * | ... |         |     |       |      |     |
 * |  93 |      80 |   3 | BC/MC |    0 | 1/4 |
 * |-----+---------+-----+-------+------+-----|-
 * |  94 |     N/A |   0 |   N/A | MGMT |   3 | 1 TXQ for unknown STA per VIF
 * | ... |         |     |       |      |     |
 * |  97 |     N/A |   3 |   N/A | MGMT |   3 |
 * |-----+---------+-----+-------+------+-----|-
 * |  98 |     N/A |     |   N/A | MGMT |   3 | 1 TXQ for offchannel frame
 */
#define NX_NB_TID_PER_STA 8
#define NX_NB_TXQ_PER_STA (NX_NB_TID_PER_STA + 1)
#define NX_NB_TXQ_PER_VIF 2
#define NX_NB_TXQ ((NX_NB_TXQ_PER_STA * NX_REMOTE_STA_MAX) +    \
                   (NX_NB_TXQ_PER_VIF * NX_VIRT_DEV_MAX) + 1)

#define NX_FIRST_VIF_TXQ_IDX (NX_REMOTE_STA_MAX * NX_NB_TXQ_PER_STA)
#define NX_FIRST_BCMC_TXQ_IDX  NX_FIRST_VIF_TXQ_IDX
#define NX_FIRST_UNK_TXQ_IDX  (NX_FIRST_BCMC_TXQ_IDX + NX_VIRT_DEV_MAX)

#define NX_OFF_CHAN_TXQ_IDX (NX_FIRST_VIF_TXQ_IDX +                     \
                             (NX_VIRT_DEV_MAX * NX_NB_TXQ_PER_VIF))
#define NX_BCMC_TXQ_TYPE 0
#define NX_UNK_TXQ_TYPE  1

/**
 * Each data TXQ is a netdev queue. TXQ to send MGT are not data TXQ as
 * they did not recieved buffer from netdev interface.
 * Need to allocate the maximum case.
 * AP : all STAs + 1 BC/MC
 */
#define NX_NB_NDEV_TXQ ((NX_NB_TID_PER_STA * NX_REMOTE_STA_MAX) + 1 )
#define NX_BCMC_TXQ_NDEV_IDX (NX_NB_TID_PER_STA * NX_REMOTE_STA_MAX)
#define NX_STA_NDEV_IDX(tid, sta_idx) ((tid) + (sta_idx) * NX_NB_TID_PER_STA)
#define NDEV_NO_TXQ 0xffff
#if (NX_NB_NDEV_TXQ >= NDEV_NO_TXQ)
#error("Need to increase struct siwifi_txq->ndev_idx size")
#endif

/* stop netdev queue when number of queued buffers if greater than this  */
#define SIWIFI_NDEV_FLOW_CTRL_STOP    512
/* restart netdev queue when number of queued buffers is lower than this */
#define SIWIFI_NDEV_FLOW_CTRL_RESTART 256

#define SIWIFI_NDEV_PS_ACTIVE_DROP   ((SIWIFI_NDEV_FLOW_CTRL_STOP + SIWIFI_NDEV_FLOW_CTRL_RESTART) / 2)

#define TXQ_INACTIVE 0xffff
#if (NX_NB_TXQ >= TXQ_INACTIVE)
#error("Need to increase struct siwifi_txq->idx size")
#endif
#define NX_TXQ_INITIAL_CREDITS_TID0 32
#define NX_TXQ_INITIAL_CREDITS 4

#define MAX_NUM_HWQ_TOKENS 4
#define MAX_SKBS_PER_TOKEN 64

/**
 * TXQ tid sorted by decreasing priority
 */
extern const int nx_tid_prio[NX_NB_TID_PER_STA];

//This value should be big enough, in fact,
//the early skb list length can not reach the value big than 128. Before that, the net queue should be stopped
//by us. To be safe, here we still use a big value
#define MAX_EARLY_SKB_LEN 160

#define NUM_TX_DESCS_PER_AC 8
#define NUM_TX_DESCS_PER_AC_L 2
#define NUM_PER_HWQ_RECORD 320

#define AVE_SPEED_CNT_THRES 4
#define AVE_SPEED_CREDITS_LOW 16
#define AVE_SPEED_CREDITS_UP  32
struct siwifi_ave_speed  {
    uint8_t  ave_speed_enable;
    uint8_t  as_txq_cnt;
    uint32_t as_rate_total;
    uint32_t as_pkt_total;
    uint32_t as_ave_pkt_total;
};

/**
 * struct siwifi_hwq - Structure used to save information relative to
 *                   an AC TX queue (aka HW queue)
 * @list: List of TXQ, that have buffers ready for this HWQ
 * @credits: available credit for the queue (i.e. nb of buffers that
 *           can be pushed to FW )
 * @id Id of the HWQ among SIWIFI_HWQ_....
 * @size size of the queue
 * @need_processing Indicate if hwq should be processed
 * @len number of packet ready to be pushed to fw for this HW queue
 * @len_stop threshold to stop mac80211(i.e. netdev) queues. Stop queue when
 *           driver has more than @len_stop packets ready.
 * @len_start threshold to wake mac8011 queues. Wake queue when driver has
 *            less than @len_start packets ready.
 */
struct siwifi_hwq {
    struct list_head list;
    s16 credits[CONFIG_USER_MAX];
    u8 size;
    u8 id;
    bool need_processing;

#ifdef TOKEN_ENABLE
	//numbers ot token that has been used
	u8 outstanding_tokens;
	//status of every token,0 is not used,1 has been used
	u8 token_status[NUM_TX_DESCS_PER_AC];
	//numbers of every aggregate num(1,2,...31,32)
	u64 ampdu_num[32];
	u8 record_txq[NUM_PER_HWQ_RECORD];
	//numbers of ampdu(numbers of packets for one token) for current record
	u8 record_ampdu_num[NUM_PER_HWQ_RECORD];
	//record last NUM_PER_HWQ_RECORD times
	u16 current_record_num;
#ifdef CONFIG_BRIDGE_ACCELERATE
	//numbers of accel txq ready when record
    u8 record_accel_txq_ready[NUM_PER_HWQ_RECORD];
#endif
	//numbers of txq ready when record
    u8 record_txq_ready[NUM_PER_HWQ_RECORD];
	//numbers of hwq credits when record
    u8 record_hwq_credit[NUM_PER_HWQ_RECORD];
	//numbers of txq credits when record
    u8 record_txq_credit[NUM_PER_HWQ_RECORD];
#endif
	u32 cfm_cnt;
	u32 push_cnt;

#ifdef NEW_SCHEDULE
    u8 max_skb_per_desc[CONFIG_USER_MAX];
#endif
    struct siwifi_ave_speed ave_speed;
};

/**
 * enum siwifi_push_flags - Flags of pushed buffer
 *
 * @SIWIFI_PUSH_RETRY Pushing a buffer for retry
 * @SIWIFI_PUSH_IMMEDIATE Pushing a buffer without queuing it first
 */
enum siwifi_push_flags {
    SIWIFI_PUSH_RETRY  = BIT(0),
    SIWIFI_PUSH_IMMEDIATE = BIT(1),
};

/**
 * enum siwifi_txq_flags - TXQ status flag
 *
 * @SIWIFI_TXQ_IN_HWQ_LIST: The queue is scheduled for transmission
 * @SIWIFI_TXQ_STOP_FULL: No more credits for the queue
 * @SIWIFI_TXQ_STOP_CSA: CSA is in progress
 * @SIWIFI_TXQ_STOP_STA_PS: Destiniation sta is currently in power save mode
 * @SIWIFI_TXQ_STOP_VIF_PS: Vif owning this queue is currently in power save mode
 * @SIWIFI_TXQ_STOP_CHAN: Channel of this queue is not the current active channel
 * @SIWIFI_TXQ_STOP_MU_POS: TXQ is stopped waiting for all the buffers pushed to
 *                       fw to be confirmed
 * @SIWIFI_TXQ_STOP: All possible reason to have a txq stopped
 * @SIWIFI_TXQ_NDEV_FLOW_CTRL: associated netdev queue is currently stopped.
 *                          Note: when a TXQ is flowctrl it is NOT stopped
 * @SIWIFI_TXQ_MEMORY_CTRL: associated netdev queue is currently stopped when reach memory limit
 *                          Note: when a TXQ is flowctrl it is NOT stopped
 * @SIWIFI_TXQ_IN_STUCK_CHECK_LIST:
                        txq has been put into stuck check list queue
 */
enum siwifi_txq_flags {
    SIWIFI_TXQ_IN_HWQ_LIST  = BIT(0),
    SIWIFI_TXQ_STOP_FULL    = BIT(1),
    SIWIFI_TXQ_STOP_CSA     = BIT(2),
    SIWIFI_TXQ_STOP_STA_PS  = BIT(3),
    SIWIFI_TXQ_STOP_VIF_PS  = BIT(4),
    SIWIFI_TXQ_STOP_CHAN    = BIT(5),
    SIWIFI_TXQ_STOP_MU_POS  = BIT(6),
#ifdef CONFIG_SIWIFI_RF_RECALI
    SIWIFI_TXQ_STOP_RECALI  = BIT(7),
#endif
    SIWIFI_TXQ_STOP         = (SIWIFI_TXQ_STOP_FULL | SIWIFI_TXQ_STOP_CSA |
                             SIWIFI_TXQ_STOP_STA_PS | SIWIFI_TXQ_STOP_VIF_PS |
                             SIWIFI_TXQ_STOP_CHAN
#ifdef CONFIG_SIWIFI_RF_RECALI
                            | SIWIFI_TXQ_STOP_RECALI
#endif
                            ) ,
    SIWIFI_TXQ_NDEV_FLOW_CTRL = BIT(8),
    SIWIFI_TXQ_MEMORY_CTRL    = BIT(9),
    SIWIFI_TXQ_IN_STUCK_CHECK_LIST = BIT(10),
};


#define SIWIFI_TXQ_STOP_POS_VIF_STOP 1
#define SIWIFI_TXQ_STOP_POS_STATION_STOP 2
#define SIWIFI_TXQ_STOP_POS_DATA_CFM 3
#define SIWIFI_TXQ_STOP_POS_CREDIT_UPDATE 4
#define SIWIFI_TXQ_STOP_POS_PUSH_FULL 5
#define SIWIFI_TXQ_STOP_POS_IOCTL 6
#define SIWIFI_TXQ_STOP_POS_SELECT_USER 7

/**
 * struct txq_time_stat - statistics on txqs,update by txq_stat_handler
 * @inlmac_total : total skb count that (push to lmac and cfm)
 * @inlmac_retry : retry skb count that (push to lmac and cfm)
 * @inlmac_100ms : (delay > 100ms) skb cnt (delay = time_cfm_from_lmac - time_push_to_lmac)
 * @inlmac_50ms  : (100ms >= delay > 50ms) skb count
 * @inlmac_20ms  : (50ms  >= delay > 20ms) skb count
 * @inlmac_10ms  : (20ms  >= delay > 10ms) skb count
 * @inlmac_0ms   : (10ms  >= delay) skb count
 * @come_xmit    : the skb count that enter siwifi_start_xmit
 * @free_xmit    : the skb count that is freed by siwifi_start_xmit because of (SIWIFI_TXQ_NDEV_FLOW_CTRL or others)
 */
struct txq_time_stat {
    u32 inlmac_total;
    u32 inlmac_retry;
    u32 inlmac_100ms;
    u32 inlmac_50ms;
    u32 inlmac_20ms;
    u32 inlmac_10ms;
    u32 inlmac_0ms;
    u32 come_xmit;
    u32 free_xmit;
};

/**
 * struct txq_atf - atf (airtime fairness) information in txq
 * @enable : whether the txq use atf, now just enable in SIWIFI_HWQ_BE
 * @rateinfo : rc rate from the last cfm
 * @record_rateinfo : rate used in atf strategy
 * @addup_rateinfo : current weight in atf strategy
 * @have_sent : whether the txq have sent skb since the last siwifi_update_atf
 * @debug_get_token_cnt : total count that txq has get token (in atf strategy)
 * @debug_skip_token_cnt : total count that txq is refused to get token (in atf strategy)
 * @debug_get_token_cnt_last : count that txq has get token in last txq_stat_timer
 * @debug_skip_token_cnt_last : count that txq is refused to get token in last txq_stat_timer
 * @debug_get_token_cnt_record : record of @debug_get_token_cnt when last txq_stat_timer end
 * @debug_skip_token_cnt_record : record of @debug_skip_token_cnt_last when last txq_stat_timer end
 */
struct txq_atf {
    uint8_t enable;
    uint32_t rateinfo;
    uint32_t record_rateinfo;
    uint32_t addup_rateinfo;
    uint8_t have_sent;
    uint32_t debug_get_token_cnt;
    uint32_t debug_skip_token_cnt;
    uint32_t debug_get_token_cnt_last;
    uint32_t debug_skip_token_cnt_last;
    uint32_t debug_get_token_cnt_record;
    uint32_t debug_skip_token_cnt_record;
};

/**
 * struct siwifi_txq - Structure used to save information relative to
 *                   a RA/TID TX queue
 *
 * @idx: Unique txq idx. Set to TXQ_INACTIVE if txq is not used.
 * @status: bitfield of @siwifi_txq_flags.
 * @credits: available credit for the queue (i.e. nb of buffers that
 *           can be pushed to FW).
 * @pkt_sent: number of consecutive pkt sent without leaving HW queue list
 * @pkt_pushed: number of pkt currently pending for transmission confirmation
 * @sched_list: list node for HW queue schedule list (siwifi_hwq.list)
 * @sk_list: list of buffers to push to fw
 * @last_retry_skb: pointer on the last skb in @sk_list that is a retry.
 *                  (retry skb are stored at the beginning of the list)
 *                  NULL if no retry skb is queued in @sk_list
 * @nb_retry: Number of retry packet queued.
 * @hwq: Pointer on the associated HW queue.
 * @push_limit: number of packet to push before removing the txq from hwq list.
 *              (we always have push_limit < skb_queue_len(sk_list))
 *
 * SOFTMAC specific:
 * @baw: Block Ack window information
 * @amsdu_anchor: pointer to siwifi_sw_txhdr of the first subframe of the A-MSDU.
 *                NULL if no A-MSDU frame is in construction
 * @amsdu_ht_len_cap:
 * @amsdu_vht_len_cap:
 * @tid:
 * @nb_ready_mac80211: Number of buffer ready in mac80211 txq
 *
 * FULLMAC specific
 * @ps_id: Index to use for Power save mode (LEGACY or UAPSD)
 * @ndev_idx: txq idx from netdev point of view (0xFF for non netdev queue)
 * @ndev: pointer to ndev of the corresponding vif
 * @amsdu: pointer to siwifi_sw_txhdr of the first subframe of the A-MSDU.
 *         NULL if no A-MSDU frame is in construction
 * @amsdu_len: Maximum size allowed for an A-MSDU. 0 means A-MSDU not allowed
 * @amsdu_maxnb Maximum amsdu number for one single amsdu
 */
struct siwifi_txq {
    u16 idx;
    u16 status;
    s16 credits;
    u8 pkt_sent;
    u8 pkt_pushed[CONFIG_USER_MAX];
    uint32_t pkt_send_total;
    uint32_t pkt_send_success;
    struct list_head sched_list;
    struct sk_buff_head sk_list;
    struct sk_buff *last_retry_skb;
    struct siwifi_hwq *hwq;
    int nb_retry;
    u8 push_limit;
#ifdef CONFIG_MAC80211_TXQ
    unsigned long nb_ready_mac80211;
#endif
    struct siwifi_sta *sta;
    u8 ps_id;
    u32 init_time;
    u16 ndev_idx;
    struct net_device *ndev;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    struct siwifi_sw_txhdr *amsdu;
    u16 amsdu_len;
    u8 amsdu_maxnb;
#endif /* CONFIG_SIWIFI_AMSDUS_TX */
	int token_pkt_num[NX_TXQ_CNT][NUM_TX_DESCS_PER_AC];
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    u8 mumimo_info;
#endif
#ifdef CONFIG_BRIDGE_ACCELERATE
    struct sk_buff_head accel_sk_list;
#endif
    u32 ps_on_drop;
    u32 ps_active_drop;
    u32 ps_off_drop;
    int stop_num;
    int wake_num;
    u16 last_stop_pos;

    struct siwifi_hw *siwifi_hw;
    u64 check_stuck_start;
    u32 stuck_time;
    s16 memctl_balance;
    //all statistics, Start on the sta connection
    struct txq_time_stat time_stat;
    //record statistics at the last of txq_stat_timer, updated by txq_stat_handler
    struct txq_time_stat record_time_stat;
    //statistics in timer time(SIWIFI_TXQ_STAT_TIME_MS), updated by txq_stat_handler
    struct txq_time_stat last_timer_time_stat;
    struct txq_atf atf;
    u32 ps_active_change;
    //  statistics for debugging
    u32 siwifi_static_time_drop;
    u64 siwifi_static_last_inhwq;
    u64 siwifi_static_last_process;
    u16 siwifi_static_start;
    u16 siwifi_static_stop;
    u16 siwifi_static_process;
    u16 siwifi_static_pro_in_hwq;
    u16 siwifi_static_pro_reorder;
    u16 siwifi_static_not_ready;
    u32 as_ave_pkt;
    u32 as_enable_pkt;
    u32 as_disable_pkt;
};

struct siwifi_sta;
struct siwifi_vif;
struct siwifi_hw;
struct siwifi_sw_txhdr;
#ifdef TOKEN_ENABLE
extern int tx_descs_num;
#endif

#ifdef CONFIG_SIWIFI_MUMIMO_TX
#define SIWIFI_TXQ_GROUP_ID(txq) ((txq)->mumimo_info & 0x3f)
#define SIWIFI_TXQ_POS_ID(txq)   (((txq)->mumimo_info >> 6) & 0x3)
#else
#define SIWIFI_TXQ_GROUP_ID(txq) 0
#define SIWIFI_TXQ_POS_ID(txq)   0
#endif /* CONFIG_SIWIFI_MUMIMO_TX */

static inline bool siwifi_txq_is_stopped(struct siwifi_txq *txq)
{
    return (txq->status & SIWIFI_TXQ_STOP);
}

static inline bool siwifi_txq_is_full(struct siwifi_txq *txq)
{
    return (txq->status & SIWIFI_TXQ_STOP_FULL);
}

static inline bool siwifi_txq_is_scheduled(struct siwifi_txq *txq)
{
    return (txq->status & SIWIFI_TXQ_IN_HWQ_LIST);
}

static inline bool siwifi_txq_has_data(struct siwifi_txq *txq)
{
    return (skb_queue_len(&txq->sk_list)
#ifdef CONFIG_BRIDGE_ACCELERATE
            || skb_queue_len(&txq->accel_sk_list)
#endif
            );

}

#ifdef CONFIG_BRIDGE_ACCELERATE
static inline bool siwfi_txq_accel_is_full(struct siwifi_txq *txq)
{
    return (skb_queue_len(&txq->accel_sk_list) > SIWIFI_NDEV_FLOW_CTRL_STOP);
}
#endif

/**
 * siwifi_txq_is_ready_for_push - Check if a TXQ is ready for push
 *
 * @txq: txq pointer
 *
 * if
 * - txq is not stopped
 * - and hwq has credits
 * - and there is no buffer queued
 * then a buffer can be immediately pushed without having to queue it first
 * @return: true if the 3 conditions are met and false otherwise.
 */
static inline bool siwifi_txq_is_ready_for_push(struct siwifi_txq *txq)
{
    return (!siwifi_txq_is_stopped(txq) &&
            txq->hwq->credits[SIWIFI_TXQ_POS_ID(txq)] > 0 &&
            skb_queue_empty(&txq->sk_list));
}

/**
 * foreach_sta_txq - Macro to iterate over all TXQ of a STA in increasing
 *                   TID order
 *
 * @sta: pointer to siwifi_sta
 * @txq: pointer to siwifi_txq updated with the next TXQ at each iteration
 * @tid: int updated with the TXQ tid at each iteration
 * @siwifi_hw: main driver data
 */
#ifdef CONFIG_MAC80211_TXQ
#define foreach_sta_txq(sta, txq, tid, siwifi_hw)                         \
    for (tid = 0, txq = siwifi_txq_sta_get(sta, 0);                       \
         tid < NX_NB_TXQ_PER_STA;                                       \
         tid++, txq = siwifi_txq_sta_get(sta, tid))
#else /* CONFIG_MAC80211_TXQ */
#define foreach_sta_txq(sta, txq, tid, siwifi_hw)                          \
    for (tid = 0, txq = siwifi_txq_sta_get(sta, 0, siwifi_hw);               \
         tid < (is_multicast_sta(sta->sta_idx) ? 1 : NX_NB_TXQ_PER_STA); \
         tid++, txq++)

#endif

/**
 * foreach_sta_txq_prio - Macro to iterate over all TXQ of a STA in
 *                        decreasing priority order
 *
 * @sta: pointer to siwifi_sta
 * @txq: pointer to siwifi_txq updated with the next TXQ at each iteration
 * @tid: int updated with the TXQ tid at each iteration
 * @i: int updated with ieration count
 * @siwifi_hw: main driver data
 *
 * Note: For fullmac txq for mgmt frame is skipped
 */
//RM#7998 workaround here to avoid endless loop in 18.06 compile env
#if 0
#define foreach_sta_txq_prio(sta, txq, tid, i, siwifi_hw)                          \
    for (i = 0, tid = nx_tid_prio[0], txq = siwifi_txq_sta_get(sta, tid, siwifi_hw); \
         i < NX_NB_TID_PER_STA;                                                  \
         i++, tid = nx_tid_prio[i], txq = siwifi_txq_sta_get(sta, tid, siwifi_hw))
#endif
/**
 * foreach_vif_txq - Macro to iterate over all TXQ of a VIF (in AC order)
 *
 * @vif: pointer to siwifi_vif
 * @txq: pointer to siwifi_txq updated with the next TXQ at each iteration
 * @ac:  int updated with the TXQ ac at each iteration
 */
#ifdef CONFIG_MAC80211_TXQ
#define foreach_vif_txq(vif, txq, ac)                                   \
    for (ac = SIWIFI_HWQ_BK, txq = siwifi_txq_vif_get(vif, ac);             \
         ac < NX_NB_TXQ_PER_VIF;                                        \
         ac++, txq = siwifi_txq_vif_get(vif, ac))

#else
#define foreach_vif_txq(vif, txq, ac)                                   \
    for (ac = SIWIFI_HWQ_BK, txq = &vif->txqs[0];                         \
         ac < NX_NB_TXQ_PER_VIF;                                        \
         ac++, txq++)
#endif

struct siwifi_txq *siwifi_txq_sta_get(struct siwifi_sta *sta, u8 tid,
                                  struct siwifi_hw * siwifi_hw);
struct siwifi_txq *siwifi_txq_vif_get(struct siwifi_vif *vif, u8 type);

/**
 * siwifi_txq_vif_get_status - return status bits related to the vif
 *
 * @siwifi_vif: Pointer to vif structure
 */
static inline u8 siwifi_txq_vif_get_status(struct siwifi_vif *siwifi_vif)
{
    struct siwifi_txq *txq = siwifi_txq_vif_get(siwifi_vif, 0);
    return (txq->status & (SIWIFI_TXQ_STOP_CHAN | SIWIFI_TXQ_STOP_VIF_PS));
}

void siwifi_txq_vif_init(struct siwifi_hw * siwifi_hw, struct siwifi_vif *vif,
                       u8 status);
void siwifi_txq_vif_deinit(struct siwifi_hw * siwifi_hw, struct siwifi_vif *vif);
void siwifi_txq_sta_init(struct siwifi_hw * siwifi_hw, struct siwifi_sta *siwifi_sta,
                       u8 status);
void siwifi_txq_sta_deinit(struct siwifi_hw * siwifi_hw, struct siwifi_sta *siwifi_sta);
void siwifi_txq_unk_vif_init(struct siwifi_vif *siwifi_vif);
void siwifi_txq_unk_vif_deinit(struct siwifi_vif *vif);
void siwifi_txq_offchan_init(struct siwifi_vif *siwifi_vif);
void siwifi_txq_offchan_deinit(struct siwifi_vif *siwifi_vif);
void siwifi_txq_tdls_vif_init(struct siwifi_vif *siwifi_vif);
void siwifi_txq_tdls_vif_deinit(struct siwifi_vif *vif);
void siwifi_txq_tdls_sta_start(struct siwifi_vif *siwifi_vif, u16 reason,
                             struct siwifi_hw *siwifi_hw);
void siwifi_txq_tdls_sta_stop(struct siwifi_vif *siwifi_vif, u16 reason,
                            struct siwifi_hw *siwifi_hw);
void siwifi_txq_ps_drop_skb(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq);


void siwifi_txq_add_to_hw_list(struct siwifi_txq *txq);
void siwifi_txq_del_from_hw_list(struct siwifi_txq *txq);
void siwifi_txq_add_to_stuck_check_list(struct siwifi_txq *txq);
void siwifi_txq_del_from_stuck_check_list(struct siwifi_txq *txq);
void siwifi_txq_stop(struct siwifi_txq *txq, u16 reason, u16 pos);
void siwifi_txq_start(struct siwifi_txq *txq, u16 reason);
void siwifi_txq_vif_start(struct siwifi_vif *vif, u16 reason,
                        struct siwifi_hw *siwifi_hw);
void siwifi_txq_vif_stop(struct siwifi_vif *vif, u16 reason,
                       struct siwifi_hw *siwifi_hw);

void siwifi_txq_sta_start(struct siwifi_sta *sta, u16 reason,
                        struct siwifi_hw *siwifi_hw);
void siwifi_txq_sta_stop(struct siwifi_sta *sta, u16 reason,
                       struct siwifi_hw *siwifi_hw);
void siwifi_txq_offchan_start(struct siwifi_hw *siwifi_hw);
void siwifi_txq_sta_switch_vif(struct siwifi_sta *sta, struct siwifi_vif *old_vif,
        struct siwifi_vif *new_vif);
//give a chance for other module to flush the queue
void siwifi_txq_flush(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq);

int siwifi_txq_queue_skb(struct sk_buff *skb, struct siwifi_txq *txq,
                       struct siwifi_hw *siwifi_hw,  bool retry);
void siwifi_txq_confirm_any(struct siwifi_hw *siwifi_hw, struct siwifi_txq *txq,
                          struct siwifi_hwq *hwq, struct siwifi_sw_txhdr *sw_txhdr, int release_hwq);


void siwifi_hwq_init(struct siwifi_hw *siwifi_hw);
void siwifi_hwq_process(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq);
void siwifi_hwq_process_all(struct siwifi_hw *siwifi_hw);

int siwifi_adjust_hwq_credits(struct siwifi_hw *siwifi_hw,s16 credit_dec);
#ifdef TOKEN_ENABLE
int siwifi_get_num_tx_descs_per_ac(struct siwifi_hw *siwifi_hw);
#endif
#endif /* _SIWIFI_TXQ_H_ */
