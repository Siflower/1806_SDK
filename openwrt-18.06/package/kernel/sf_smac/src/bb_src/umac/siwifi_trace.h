/*
 * =====================================================================================
 *
 *       Filename:  siwifi_trace_trx.hh
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/19/2021 11:21:01 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#ifndef _SIWIFI_TRACE_TRX_H_
#define _SIWIFI_TRACE_TRX_H_

#define SIWIFI_SKB_DEBUG_FLAG_PING                0x1
#define SIWIFI_SKB_DEBUG_FLAG_ALL                (0x1 << 1)
#define SIWIFI_SKB_DEBUG_FLAG_MGMT               (0x1 << 2)
#define SIWIFI_SKB_DEBUG_FLAG_RECORD             (0x1 << 3)
#define SIWIFI_SKB_DEBUG_FLAG_PAE                (0x1 << 4)

#define PING_CB_POSITION 45
#define PING_CB_CODE    0x0B

#define RECORD_CB_POSITION 46
#define RECORD_CB_CODE    0x0B

struct siwifi_trace_ctx {
    u32 debug_icmp_tx_num;
    u32 debug_icmp_tx_ok_num;
    u32 debug_icmp_reply;
    u32 debug_sta_idx;
    u32 debug_flag;
    //dynamic alloc
    u8 *debug_sta_record_buf;
    u32 debug_sta_record_pos;
};

enum siwifi_trace_sta_event{
    SIWIFI_TRACE_EVENT_TX_IN,
    SIWIFI_TRACE_EVENT_TX_CFM,
    SIWIFI_TRACE_EVENT_TX_RX,
    SIWIFI_TRACE_EVENT_TX_PUSH,
    SIWIFI_TRACE_EVENT_HWQ_PROCESS,
};

#define SIWIFI_TRACE_RECORD_NUM   100

struct siwifi_trace_sta_record{
    u8 event;
    u8 sta_idx;
    u8 tid;
    u32 skb_addr;
    u16 skb_len;
    u64 ts_nsec;
    u32 status;
};

#ifdef CONFIG_SIWIFI_TRACE_SKB
void siwifi_trace_init(struct siwifi_hw *siwifi_hw);
void siwifi_trace_deinit(struct siwifi_hw *siwifi_hw);
void siwifi_trace_record_reset(struct siwifi_hw *siwifi_hw);
void siwifi_trace_tx_in(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb);

void siwifi_trace_tx_push(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb);

void siwifi_trace_tx_end(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, union siwifi_hw_txstatus *status, uint16_t head_room);

void siwifi_trace_rx_in(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb);

void siwifi_trace_mgmt_rx_in(struct siwifi_hw *siwifi_hw, struct sk_buff *skb);
void siwifi_trace_mgmt_tx_end(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, union siwifi_hw_txstatus *status);
void siwifi_trace_mgmt_tx_in(struct siwifi_hw *siwifi_hw, struct sk_buff *skb);
#else
#define siwifi_trace_init(siwifi_hw)    do {} while(0)
#define siwifi_trace_deinit(siwifi_hw)    do {} while(0)
#define siwifi_trace_record_reset(siwifi_hw)    do {} while(0)
#define siwifi_trace_tx_in() do {} while(0)
#define siwifi_trace_tx_push() do {} while(0)
#define siwifi_trace_tx_end() do {} while(0)
#define siwifi_trace_rx_in() do {} while(0)
#define siwifi_trace_mgmt_rx_in() do {} while(0)
#define siwifi_trace_mgmt_tx_end() do {} while(0)
#define siwifi_trace_mgmt_tx_in() do {} while(0)

#endif

#endif
