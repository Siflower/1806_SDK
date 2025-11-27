/**
 ****************************************************************************************
 *
 * @file siwifi_prof.h
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */

#ifndef _SIWIFI_PROF_H_
#define _SIWIFI_PROF_H_

#include "reg_access.h"
//todo: ycchang sync with siwifi_platform.h
#if 1
static inline void siwifi_prof_set(struct siwifi_hw *siwifi_hw, int val)
{
    *(volatile u32 *)(NXMAC_SW_SET_PROFILING_ADDR(siwifi_hw->mod_params->is_hb)) = val;
}

static inline void siwifi_prof_clear(struct siwifi_hw *siwifi_hw, int val)
{
    *(volatile u32 *)(NXMAC_SW_CLEAR_PROFILING_ADDR(siwifi_hw->mod_params->is_hb)) = val;
}

#else
#include "siwifi_platform.h"

static inline void siwifi_prof_set(struct siwifi_hw *siwifi_hw, int val)
{
    struct siwifi_plat *siwifi_plat = siwifi_hw->plat;
    SIWIFI_REG_WRITE(val, siwifi_plat, SIWIFI_ADDR_SYSTEM, NXMAC_SW_SET_PROFILING_ADDR);
}

static inline void siwifi_prof_clear(struct siwifi_hw *siwifi_hw, int val)
{
    struct siwifi_plat *siwifi_plat = siwifi_hw->plat;
    SIWIFI_REG_WRITE(val, siwifi_plat, SIWIFI_ADDR_SYSTEM, NXMAC_SW_CLEAR_PROFILING_ADDR);
}
#endif

#if 0
/* Defines for SW Profiling registers values */
enum {
    TX_IPC_IRQ,
    TX_IPC_EVT,
    TX_PREP_EVT,
    TX_DMA_IRQ,
    TX_MAC_IRQ,
    TX_PAYL_HDL,
    TX_CFM_EVT,
    TX_IPC_CFM,
    RX_MAC_IRQ,                 // 8
    RX_TRIGGER_EVT,
    RX_DMA_IRQ,
    RX_DMA_EVT,
    RX_IPC_IND,
    RX_MPDU_XFER,
    DBG_PROF_MAX
};
#endif

enum {
    SW_PROF_HOSTBUF_IDX = 12,
    /****** IPC IRQs related signals ******/
    /* E2A direction */
    SW_PROF_IRQ_E2A_RXDESC = 16,    // to make sure we let 16 bits available for LMAC FW
    SW_PROF_IRQ_E2A_TXCFM,
    SW_PROF_IRQ_E2A_DBG,
    SW_PROF_IRQ_E2A_MSG,
    SW_PROF_IPC_MSGPUSH,
    SW_PROF_MSGALLOC,
    SW_PROF_MSGIND,
    SW_PROF_DBGIND,

    /* A2E direction */
    SW_PROF_IRQ_A2E_TXCFM_BACK,

    /****** Driver functions related signals ******/
    SW_PROF_WAIT_QUEUE_STOP,
    SW_PROF_WAIT_QUEUE_WAKEUP,
    SW_PROF_SIWIFIDATAIND,
    SW_PROF_SIWIFI_IPC_IRQ_HDLR,
    SW_PROF_SIWIFI_IPC_THR_IRQ_HDLR,
    SW_PROF_IEEE80211RX,
    SW_PROF_SIWIFI_PATTERN,
    SW_PROF_MAX
};

// [LT]For debug purpose only
#if (0)
#define SW_PROF_CHAN_CTXT_CFM_HDL_BIT       (21)
#define SW_PROF_CHAN_CTXT_CFM_BIT           (22)
#define SW_PROF_CHAN_CTXT_CFM_SWDONE_BIT    (23)
#define SW_PROF_CHAN_CTXT_PUSH_BIT          (24)
#define SW_PROF_CHAN_CTXT_QUEUE_BIT         (25)
#define SW_PROF_CHAN_CTXT_TX_BIT            (26)
#define SW_PROF_CHAN_CTXT_TX_PAUSE_BIT      (27)
#define SW_PROF_CHAN_CTXT_PSWTCH_BIT        (28)
#define SW_PROF_CHAN_CTXT_SWTCH_BIT         (29)

// TO DO: update this

#define REG_SW_SET_PROFILING_CHAN(env, bit)             \
    siwifi_prof_set((struct siwifi_hw*)env, BIT(bit))

#define REG_SW_CLEAR_PROFILING_CHAN(env, bit) \
    siwifi_prof_clear((struct siwifi_hw*)env, BIT(bit))

#else
#define SW_PROF_CHAN_CTXT_CFM_HDL_BIT       (0)
#define SW_PROF_CHAN_CTXT_CFM_BIT           (0)
#define SW_PROF_CHAN_CTXT_CFM_SWDONE_BIT    (0)
#define SW_PROF_CHAN_CTXT_PUSH_BIT          (0)
#define SW_PROF_CHAN_CTXT_QUEUE_BIT         (0)
#define SW_PROF_CHAN_CTXT_TX_BIT            (0)
#define SW_PROF_CHAN_CTXT_TX_PAUSE_BIT      (0)
#define SW_PROF_CHAN_CTXT_PSWTCH_BIT        (0)
#define SW_PROF_CHAN_CTXT_SWTCH_BIT         (0)

#define REG_SW_SET_PROFILING_CHAN(env, bit)            do {} while (0)
#define REG_SW_CLEAR_PROFILING_CHAN(env, bit)          do {} while (0)
#endif

#ifdef CONFIG_SIWIFI_SW_PROFILING
/* Macros for SW PRofiling registers access */
#define REG_SW_SET_PROFILING(env, bit)                  \
    siwifi_prof_set((struct siwifi_hw*)env, BIT(bit))

#define REG_SW_SET_HOSTBUF_IDX_PROFILING(env, val)      \
    siwifi_prof_set((struct siwifi_hw*)env, val<<(SW_PROF_HOSTBUF_IDX))

#define REG_SW_CLEAR_PROFILING(env, bit)                \
    siwifi_prof_clear((struct siwifi_hw*)env, BIT(bit))

#define REG_SW_CLEAR_HOSTBUF_IDX_PROFILING(env)                         \
    siwifi_prof_clear((struct siwifi_hw*)env,0x0F<<(SW_PROF_HOSTBUF_IDX))

#else
#define REG_SW_SET_PROFILING(env, value)            do {} while (0)
#define REG_SW_CLEAR_PROFILING(env, value)          do {} while (0)
#define REG_SW_SET_HOSTBUF_IDX_PROFILING(env, val)  do {} while (0)
#define REG_SW_CLEAR_HOSTBUF_IDX_PROFILING(env)     do {} while (0)
#endif

#endif /* _SIWIFI_PROF_H_ */
