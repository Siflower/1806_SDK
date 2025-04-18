/*
 * =====================================================================================
 *
 *       Filename:  siwifi_trace_trx.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  05/19/2021 11:17:36 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */
#include "siwifi_defs.h"
#include "siwifi_trace.h"
#include "siwifi_mem.h"

#ifdef CONFIG_SIWIFI_TRACE_SKB

void siwifi_trace_init(struct siwifi_hw *siwifi_hw)
{
    memset(&siwifi_hw->trace_ctx, 0, sizeof(struct siwifi_trace_ctx));
    // did not trace any station
    siwifi_hw->trace_ctx.debug_sta_idx = 0xFFFFFFFF;
    siwifi_hw->trace_ctx.debug_flag = 0;
    siwifi_hw->trace_ctx.debug_sta_record_buf = NULL;
    siwifi_hw->trace_ctx.debug_sta_record_pos = 0;
}

void siwifi_trace_deinit(struct siwifi_hw *siwifi_hw)
{
    if(siwifi_hw->trace_ctx.debug_sta_record_buf){
        siwifi_kfree(siwifi_hw->trace_ctx.debug_sta_record_buf);
        siwifi_hw->trace_ctx.debug_sta_record_buf = NULL;
    }
}

void siwifi_trace_record_reset(struct siwifi_hw *siwifi_hw)
{
    if(siwifi_hw->trace_ctx.debug_sta_record_buf){
        memset(siwifi_hw->trace_ctx.debug_sta_record_buf, 0, sizeof(SIWIFI_TRACE_RECORD_NUM * sizeof(struct siwifi_trace_sta_record)));
        siwifi_hw->trace_ctx.debug_sta_record_pos = 0;
    }
}

void siwifi_trace_record_sta_event(struct siwifi_hw *siwifi_hw, enum siwifi_trace_sta_event event, struct siwifi_sta *sta,
        struct sk_buff *skb, uint32_t status)
{
    struct siwifi_trace_sta_record *record;
    struct siwifi_trace_sta_record *pool;
    if(!skb) return;
    if(!siwifi_hw->trace_ctx.debug_sta_record_buf){
        siwifi_hw->trace_ctx.debug_sta_record_buf = siwifi_kmalloc(SIWIFI_TRACE_RECORD_NUM * sizeof(struct siwifi_trace_sta_record), GFP_ATOMIC);
        memset(siwifi_hw->trace_ctx.debug_sta_record_buf, 0, sizeof(SIWIFI_TRACE_RECORD_NUM * sizeof(struct siwifi_trace_sta_record)));
        siwifi_hw->trace_ctx.debug_sta_record_pos = 0;
    }
    pool = (struct siwifi_trace_sta_record *)siwifi_hw->trace_ctx.debug_sta_record_buf;
    record = (struct siwifi_trace_sta_record *)(&pool[siwifi_hw->trace_ctx.debug_sta_record_pos]);
    record->event = (u8)event;
    record->sta_idx = sta ? sta->sta_idx : 0;
    record->tid = skb->priority;
    record->skb_len = skb->len;
    record->skb_addr = (uint32_t)skb;
    record->status = status;
    record->ts_nsec = local_clock();

    siwifi_hw->trace_ctx.debug_sta_record_pos = (siwifi_hw->trace_ctx.debug_sta_record_pos + 1) % SIWIFI_TRACE_RECORD_NUM;
}

void siwifi_trace_tx_in(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb)
{
    struct ethhdr *eth;
    struct iphdr *ipdata;

    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PAE){
        u8 *ptr;
        eth = (struct ethhdr *)(skb->data);
        ptr = (u8*)eth;
        if(eth->h_proto == cpu_to_be16(ETH_P_PAE)){
            printk("tx(%d) eapol key(%d) V(%d) len %d to mac[%02x:%02x:%02x:%02x:%02x:%02x]\n",
                    siwifi_hw->mod_params->is_hb, *(ptr + 15), *(ptr + 14), skb->len,
                    eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
        }
    }

    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !sta)
        return;
    if (sta->sta_idx != siwifi_hw->trace_ctx.debug_sta_idx)
        return;
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PING){
        eth = (struct ethhdr *)(skb->data);
        if (skb->len < sizeof(struct ethhdr) + 20) {
            printk("skb length is too small\n");
        }else{
            if (eth->h_proto == htons(ETH_P_IP))
            {
                ipdata = (struct iphdr *)((char *)eth + sizeof(struct ethhdr));
                if (ipdata->version == 4) {
                    if (ipdata->protocol == 1) {
                        siwifi_hw->trace_ctx.debug_icmp_tx_num ++;
                        skb->cb[PING_CB_POSITION] = PING_CB_CODE;
                    }
                }
            }
        }
    }
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_ALL){
        eth = (struct ethhdr *)(skb->data);
        printk("tx skb(%p) len %d proto %x to [%02x:%02x:%02x:%02x:%02x:%02x]\n",
                skb, skb->len, eth->h_proto, eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    }
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_RECORD){
        siwifi_trace_record_sta_event(siwifi_hw, SIWIFI_TRACE_EVENT_TX_IN, sta, skb, 0);
        skb->cb[RECORD_CB_POSITION] = RECORD_CB_CODE;
    }
}

void siwifi_trace_tx_push(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb)
{
    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !sta)
        return;

    if (sta->sta_idx != siwifi_hw->trace_ctx.debug_sta_idx)
        return;

    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_RECORD){
        siwifi_trace_record_sta_event(siwifi_hw, SIWIFI_TRACE_EVENT_TX_PUSH, sta, skb, 0);
    }
}

void siwifi_trace_tx_end(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, union siwifi_hw_txstatus *status, uint16_t head_room)
{
    if(!siwifi_hw->trace_ctx.debug_flag || !skb)
        return;

    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PAE){
        u8 *ptr;
        const struct ethhdr *eth;
        eth = (struct ethhdr *)(skb->data + head_room);
        ptr = (u8*)eth_hdr;
        if(eth->h_proto == cpu_to_be16(ETH_P_PAE)){
            printk("txi-cfm(%d) eapol key(%d) V(%d) len %d to mac[%02x:%02x:%02x:%02x:%02x:%02x] status=%x\n",
                    siwifi_hw->mod_params->is_hb, *(ptr + 15), *(ptr + 14), skb->len - head_room,
                    eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5],
                    status->value);
        }
    }


    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PING){
        //tx_successful
        if ((skb->cb[PING_CB_POSITION] == PING_CB_CODE)) {
                if (status->tx_successful)
                    siwifi_hw->trace_ctx.debug_icmp_tx_ok_num ++;
        }
    }
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_ALL){
        printk("tx-cfm skb(%p) len %d status=%x scuccess=%d\n",
                skb, skb->len, status->value,
                status->tx_successful
                );
    }
    if((siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_RECORD) && skb->cb[RECORD_CB_POSITION] == RECORD_CB_CODE){
        siwifi_trace_record_sta_event(siwifi_hw, SIWIFI_TRACE_EVENT_TX_CFM, NULL, skb, status->value);
    }
}

void siwifi_trace_rx_in(struct siwifi_hw *siwifi_hw, struct siwifi_sta *sta, struct sk_buff *skb)
{
    struct ethhdr *eth_hdr;
    struct iphdr *ipdata;

    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PAE){
        u8 *ptr;
        eth_hdr = (struct ethhdr *)skb->data;
        ptr = (u8*)eth_hdr;
        if(eth_hdr->h_proto == cpu_to_be16(ETH_P_PAE)){
            printk("rx(%d) eapol key(%d) V(%d) len %d fm mac[%02x:%02x:%02x:%02x:%02x:%02x]\n",
                    siwifi_hw->mod_params->is_hb, *(ptr + 15), *(ptr + 14), skb->len,
                    eth_hdr->h_source[0], eth_hdr->h_source[1], eth_hdr->h_source[2], eth_hdr->h_source[3], eth_hdr->h_source[4], eth_hdr->h_source[5]);
        }
    }

    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !sta)
        return;
    if (sta->sta_idx != siwifi_hw->trace_ctx.debug_sta_idx)
        return;

    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_PING){
        eth_hdr = (struct ethhdr *)(skb->data);
        if(eth_hdr->h_proto == htons(ETH_P_IP))
        {
            ipdata = (struct iphdr *)((char *)eth_hdr + sizeof(struct ethhdr));
            if (ipdata->version == 4) {
                if (ipdata->protocol == 1) {
                    siwifi_hw->trace_ctx.debug_icmp_reply++;
                }
            }
        }
    }
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_ALL){
        eth_hdr = (struct ethhdr *)(skb->data);
        printk("rx skb(%p) len %d proto %x from [%02x:%02x:%02x:%02x:%02x:%02x]\n",
                skb, skb->len, eth_hdr->h_proto,
                eth_hdr->h_source[0], eth_hdr->h_source[1], eth_hdr->h_source[2], eth_hdr->h_source[3], eth_hdr->h_source[4], eth_hdr->h_source[5]);
    }
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_RECORD){
        siwifi_trace_record_sta_event(siwifi_hw, SIWIFI_TRACE_EVENT_TX_RX, sta, skb, 0);
    }
}

static char *siwifi_get_mgmt_str(struct ieee80211_mgmt *mgmt)
{
    char *mgmt_str = NULL;
    if(!mgmt)
        goto end;
    if (ieee80211_is_probe_req(mgmt->frame_control)){
        mgmt_str = "probe req";
    }else if (ieee80211_is_probe_resp(mgmt->frame_control)){
        mgmt_str = "probe resp";
    }else if (ieee80211_is_deauth(mgmt->frame_control)){
        mgmt_str = "deauth";
    }else if(ieee80211_is_disassoc(mgmt->frame_control)){
        mgmt_str = "disassoc";
    }else if (ieee80211_is_auth(mgmt->frame_control)){
        mgmt_str = "auth";
    }else if (ieee80211_is_assoc_req(mgmt->frame_control)){
        mgmt_str = "assoc";
    }else if (ieee80211_is_reassoc_req(mgmt->frame_control)){
        mgmt_str = "reassoc";
    }
end:
    return mgmt_str;
}

void siwifi_trace_mgmt_rx_in(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !mgmt)
        return;
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_MGMT){
        char *mgmt_str = siwifi_get_mgmt_str(mgmt);
        if(!ieee80211_is_beacon(mgmt->frame_control)){
            printk("rx pkt(%d) skb(%p) %s frmctl 0x%x dura 0x%x seq 0x%x from [%02x:%02x:%02x:%02x:%02x:%02x] reasoncode: %d\n",
                siwifi_hw->mod_params->is_hb, skb, mgmt_str ? mgmt_str : "unknown", mgmt->frame_control, mgmt->duration, mgmt->seq_ctrl,
                mgmt->sa[0], mgmt->sa[1], mgmt->sa[2], mgmt->sa[3], mgmt->sa[4], mgmt->sa[5], mgmt->u.deauth.reason_code);
        }
    }
}

void siwifi_trace_mgmt_tx_end(struct siwifi_hw *siwifi_hw, struct sk_buff *skb, union siwifi_hw_txstatus *status)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !mgmt)
        return;
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_MGMT){
        printk("tx-mgmt-cfm skb(%p) status=%x scuccess=%d\n",
                skb, status->value,
                status->tx_successful
        );
    }
}

void siwifi_trace_mgmt_tx_in(struct siwifi_hw *siwifi_hw, struct sk_buff *skb)
{
    struct ieee80211_mgmt *mgmt = (struct ieee80211_mgmt *)skb->data;
    if(!siwifi_hw->trace_ctx.debug_flag || !skb || !mgmt)
        return;
    if(siwifi_hw->trace_ctx.debug_flag & SIWIFI_SKB_DEBUG_FLAG_MGMT){
        char *mgmt_str = siwifi_get_mgmt_str(mgmt);
        printk("tx pkt(%d) skb(%p) %s frmctl 0x%x dura 0x%x seq 0x%x to [%02x:%02x:%02x:%02x:%02x:%02x] reasoncode: %d\n",
                siwifi_hw->mod_params->is_hb, skb, mgmt_str ? mgmt_str : "unknown", mgmt->frame_control, mgmt->duration, mgmt->seq_ctrl,
                mgmt->da[0], mgmt->da[1], mgmt->da[2], mgmt->da[3], mgmt->da[4], mgmt->da[5], mgmt->u.deauth.reason_code);
    }
}
#endif
