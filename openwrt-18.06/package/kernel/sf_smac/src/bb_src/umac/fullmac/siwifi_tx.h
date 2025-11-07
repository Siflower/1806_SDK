/**
 ******************************************************************************
 *
 * @file siwifi_tx.h
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */
#ifndef _SIWIFI_TX_H_
#define _SIWIFI_TX_H_

#include <linux/ieee80211.h>
#include <net/cfg80211.h>
#include <linux/netdevice.h>
#include "lmac_types.h"
#include "ipc_shared.h"
#include "siwifi_txq.h"
#include "hal_desc.h"

#define SIWIFI_HWQ_BK                     0
#define SIWIFI_HWQ_BE                     1
#define SIWIFI_HWQ_VI                     2
#define SIWIFI_HWQ_VO                     3
#define SIWIFI_HWQ_BCMC                   4
#define SIWIFI_HWQ_NB                     NX_TXQ_CNT
#define SIWIFI_HWQ_ALL_ACS (SIWIFI_HWQ_BK | SIWIFI_HWQ_BE | SIWIFI_HWQ_VI | SIWIFI_HWQ_VO)
#define SIWIFI_HWQ_ALL_ACS_BIT ( BIT(SIWIFI_HWQ_BK) | BIT(SIWIFI_HWQ_BE) |    \
                               BIT(SIWIFI_HWQ_VI) | BIT(SIWIFI_HWQ_VO) )

#define SIWIFI_TX_LIFETIME_MS             3000
#define SIWIFI_TX_MAX_RATES               NX_TX_MAX_RATES

#define SIWIFI_SWTXHDR_ALIGN_SZ           4
#define SIWIFI_SWTXHDR_ALIGN_MSK (SIWIFI_SWTXHDR_ALIGN_SZ - 1)
#define SIWIFI_SWTXHDR_ALIGN_PADS(x) \
                    ((SIWIFI_SWTXHDR_ALIGN_SZ - ((x) & SIWIFI_SWTXHDR_ALIGN_MSK)) \
                     & SIWIFI_SWTXHDR_ALIGN_MSK)
#if SIWIFI_SWTXHDR_ALIGN_SZ & SIWIFI_SWTXHDR_ALIGN_MSK
#error bad SIWIFI_SWTXHDR_ALIGN_SZ
#endif

#define AMSDU_PADDING(x) ((4 - ((x) & 0x3)) & 0x3)

#define TXU_CNTRL_RETRY        BIT(0)
#define TXU_CNTRL_MORE_DATA    BIT(2)
#define TXU_CNTRL_MGMT         BIT(3)
#define TXU_CNTRL_MGMT_NO_CCK  BIT(4)
#define TXU_CNTRL_AMSDU        BIT(6)
#define TXU_CNTRL_MGMT_ROBUST  BIT(7)
#define TXU_CNTRL_USE_4ADDR    BIT(8)
#define TXU_CNTRL_EOSP         BIT(9)
#define TXU_CNTRL_MESH_FWD     BIT(10)
#define TXU_CNTRL_TDLS         BIT(11)
#define TXU_CNTRL_MGMT_NO_ACK  BIT(14)
#define TXU_CNTRL_RATE_PAE     BIT(15)

extern const int siwifi_tid2hwq[IEEE80211_NUM_TIDS];

/**
 * struct siwifi_amsdu_txhdr - Structure added in skb headroom (instead of
 * siwifi_txhdr) for amsdu subframe buffer (except for the first subframe
 * that has a normal siwifi_txhdr)
 *
 * @list     List of other amsdu subframe (siwifi_sw_txhdr.amsdu.hdrs)
 * @map_len  Length to be downloaded for this subframe
 * @dma_addr Buffer address form embedded point of view
 * @skb      skb
 * @pad      padding added before this subframe
 *           (only use when amsdu must be dismantled)
 */
struct siwifi_amsdu_txhdr {
    struct list_head list;
    size_t map_len;
    dma_addr_t dma_addr;
    struct sk_buff *skb;
    u16 pad;
};

/**
 * struct siwifi_amsdu - Structure to manage creation of an A-MSDU, updated
 * only In the first subframe of an A-MSDU
 *
 * @hdrs List of subframe of siwifi_amsdu_txhdr
 * @len  Current size for this A-MDSU (doesn't take padding into account)
 *       0 means that no amsdu is in progress
 * @nb   Number of subframe in the amsdu
 * @pad  Padding to add before adding a new subframe
 */
struct siwifi_amsdu {
    struct list_head hdrs;
    u16 len;
    u8 nb;
    u8 pad;
    /// Memory of this amsdu usage (added by skb->truesize, Excluding the first skb)
    u32_l tx_amsdu_memory_usage;
};

/**
* struct siwifi_frame_flag - Bitfield of flags
 *
 * @mgmt_frame: BIT0 indicates MGMT frame.
 * @retry_frame: BIT1 indicates retry frame.
 * @reserved: Reserved.
 */
union siwifi_frame_flag {
    struct {
        u8 mgmt_frame : 1;
        u8 retry_frame : 1;
        u8 reserved : 6;
    };
    u8 value;
};

/**
 * struct siwifi_sw_txhdr - Software part of tx header
 *
 * @siwifi_sta sta to which this buffer is addressed
 * @siwifi_vif vif that send the buffer
 * @txq pointer to TXQ used to send the buffer
 * @hw_queue Index of the HWQ used to push the buffer.
 *           May be different than txq->hwq->id on confirmation.
 * @frame_len Size of the frame (doesn't not include mac header)
 *            (Only used to update stat, can't we use skb->len instead ?)
 * @headroom Headroom added in skb to add siwifi_txhdr
 *           (Only used to remove it before freeing skb, is it needed ?)
 * @amsdu Description of amsdu whose first subframe is this buffer
 *        (amsdu.nb = 0 means this buffer is not part of amsdu)
 * @skb skb received from transmission
 * @map_len  Length mapped for DMA (only siwifi_hw_txhdr and data are mapped)
 * @dma_addr DMA address after mapping
 * @desc Buffer description that will be copied in shared mem for FW
 * @flags BIT0 indicates MGMT frame, BIT1 indicates retry frame
 */
struct siwifi_sw_txhdr {
    struct siwifi_sta *siwifi_sta;
    struct siwifi_vif *siwifi_vif;
    struct siwifi_txq *txq;
    u32 txq_init_time;
    u8 mgmt_frame;
    u8 hw_queue;
    u16 frame_len;
    u16 headroom;
#ifdef CONFIG_SIWIFI_AMSDUS_TX
    struct siwifi_amsdu amsdu;
#endif
    struct sk_buff *skb;

    size_t map_len;
    dma_addr_t dma_addr;
    struct txdesc_api desc;
    u64 push_to_lmac_time;
    u64 confirm_time;
    union siwifi_frame_flag flags;
};

/**
 * struct siwifi_txhdr - Stucture to control transimission of packet
 * (Added in skb headroom)
 *
 * @sw_hdr: Information from driver
 * @cache_guard:
 * @hw_hdr: Information for/from hardware
 */
struct siwifi_txhdr {
    struct siwifi_sw_txhdr *sw_hdr;
#ifdef TOKEN_ENABLE
	int token_id;
#endif
    char cache_guard[L1_CACHE_BYTES];
    struct siwifi_hw_txhdr hw_hdr;
    char cache_guard2[L1_CACHE_BYTES];
};

u16 siwifi_select_txq(struct siwifi_vif *siwifi_vif, struct sk_buff *skb);
int siwifi_start_xmit(struct sk_buff *skb, struct net_device *dev);
int siwifi_start_mgmt_xmit(struct siwifi_vif *vif, struct siwifi_sta *sta,
                         struct cfg80211_mgmt_tx_params *params, bool offchan,
                         u64 *cookie);
#ifndef NEW_SCHEDULE
int siwifi_txdatacfm(void *pthis, void *host_id);
#else
int siwifi_txdatacfm_burst(void *pthis, void *host_id, uint32_t length);
#endif

struct siwifi_hw;
struct siwifi_sta;
void siwifi_set_traffic_status(struct siwifi_hw *siwifi_hw,
                             struct siwifi_sta *sta,
                             bool available,
                             u8 ps_id);
void siwifi_ps_bh_enable(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta,
                       uint8_t ps_state);
void siwifi_ps_bh_traffic_req(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta,
                            u16 pkt_req, u8 ps_id);

void siwifi_switch_vif_sta_txq(struct siwifi_sta *sta, struct siwifi_vif *old_vif,
                             struct siwifi_vif *new_vif);

int siwifi_dbgfs_print_sta(char *buf, size_t size, struct siwifi_sta *sta,
                         struct siwifi_hw *siwifi_hw);
void siwifi_txq_credit_update(struct siwifi_hw *siwifi_hw, int sta_idx, u8 tid,
                            s16 update);
int siwifi_tx_push(struct siwifi_hw *siwifi_hw, struct siwifi_txhdr *txhdr, int flags);
#ifdef NEW_SCHEDULE

int siwifi_tx_push_burst(struct siwifi_hw *siwifi_hw, struct siwifi_hwq *hwq, struct siwifi_txq *txq, struct sk_buff_head *sk_list);

#endif

#ifdef CONFIG_SIWIFI_AMSDUS_TX
//fremm asdus if flag TXU_CNTRL_AMSDU exist
void siwifi_free_amsdu(struct siwifi_hw *siwifi_hw, struct siwifi_sw_txhdr *sw_txhdr);
#endif

#define MSECS(t)        msecs_to_jiffies(t)
#define SIWIFI_TXQ_STAT_TIME_MS  (1 * 1000)
void txq_stat_timer(unsigned long data);
void txq_stat_handler(struct work_struct *wk);
#endif /* _SIWIFI_TX_H_ */
