/**
 ******************************************************************************
 *
 * @file ipc_host.c
 *
 * @brief IPC module.
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

/*
 * INCLUDE FILES
 ******************************************************************************
 */
#ifndef __KERNEL__
#include <stdio.h>
#define REG_SW_SET_PROFILING(env, value)   do{  }while(0)
#define REG_SW_CLEAR_PROFILING(env, value)   do{  }while(0)
#define REG_SW_CLEAR_HOSTBUF_IDX_PROFILING(env)   do{  }while(0)
#define REG_SW_SET_HOSTBUF_IDX_PROFILING(env, val)   do{  }while(0)
#else
#include <linux/spinlock.h>
#include "siwifi_defs.h"
#include "siwifi_prof.h"
#endif
#include "reg_ipc_emb.h"
#include "reg_ipc_app.h"
#include "ipc_host.h"

#ifdef ASSERT_ERR
#undef ASSERT_ERR
#include <asm/bug.h>
#define ASSERT_ERR(x) BUG_ON(!(x))
#endif

//TODO, remove sk_buff_head structure from this, because only __KERNEL__ only ,we have this structure
//ipc should be pure c code, can be ported to any platform
#define ALLOC_HOST_ID_RES(index, user)    \
        {   \
            int j;  \
            struct sk_buff_head *base = siwifi_kmalloc(sizeof(struct sk_buff_head) * nx_txdesc_cnt[index], GFP_KERNEL);   \
            ASSERT_ERR(base);   \
            for (j = 0; j < nx_txdesc_cnt[index]; j++) {    \
                __skb_queue_head_init(base + j);    \
                env->tx_host_id##index[user][j] = (void *)(base + j);   \
            }   \
        }

#define DEALLOC_HOST_ID_RES(index, user)    \
        {   \
            siwifi_kfree(env->tx_host_id##index[user][0]); \
        }

#define IPC_WIFI_BASE(band) (void*)WIFI_BASE_ADDR(band)

/* This will catch if software access a user-defined memory region*/
//#define SIWIFI_DEBUG_BUS_TRAP

/*
 * TYPES DEFINITION
 ******************************************************************************
 */

const int nx_txdesc_cnt[] =
{
    NX_TXDESC_CNT0,
    NX_TXDESC_CNT1,
    NX_TXDESC_CNT2,
    NX_TXDESC_CNT3,
    #if NX_TXQ_CNT == 5
    NX_TXDESC_CNT4,
    #endif
};

#ifndef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
const int nx_txdesc_cnt_msk[] =
{
    NX_TXDESC_CNT0 - 1,
    NX_TXDESC_CNT1 - 1,
    NX_TXDESC_CNT2 - 1,
    NX_TXDESC_CNT3 - 1,
    #if NX_TXQ_CNT == 5
    NX_TXDESC_CNT4 - 1,
    #endif
};
#endif

const int nx_max_skb_txdesc_cnt[] =
{
    NX_MAX_SKB_TXDESC0,
    NX_MAX_SKB_TXDESC1,
    NX_MAX_SKB_TXDESC2,
    NX_MAX_SKB_TXDESC3,
    #if NX_TXQ_CNT == 5
    NX_MAX_SKB_TXDESC4,
#endif

};

const int nx_txuser_cnt[] =
{
    CONFIG_USER_MAX,
    CONFIG_USER_MAX,
    CONFIG_USER_MAX,
    CONFIG_USER_MAX,
    #if NX_TXQ_CNT == 5
    1,
    #endif
};


/*
 * FUNCTIONS DEFINITIONS
 ******************************************************************************
 */
/**
 * ipc_host_rxdesc_handler() - Handle the reception of a Rx Descriptor
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_RXDESC is set
 */
void ipc_host_rxdesc_handler(struct ipc_host_env_tag *env)
{
    uint32_t count = 0;
    // For profiling
    REG_SW_SET_PROFILING(env->pthis, SW_PROF_IRQ_E2A_RXDESC);

    // Clear pending status
    if (env->ipc_pending_status)
        env->ipc_pending_status = 0;

    // LMAC has triggered an IT saying that a reception has occurred.
    // Then we first need to check the validity of the current hostbuf, and the validity
    // of the next hostbufs too, because it is likely that several hostbufs have been
    // filled within the time needed for this irq handling
    do {
        // call the external function to indicate that a RX descriptor is received
        if (env->cb.recv_data_ind(env->pthis,
                                  env->ipc_host_rxdesc_array[env->ipc_host_rxdesc_idx].hostid) != 0)
            break;
        else
        {
            //RM#9129 avoid cmd queue timeout when endless loop in handle rx when test amsdu-rx small buffer
            if (count > env->max_rx_process_time) {
                env->ipc_pending_status |= IPC_IRQ_E2A_RXDESC;
                break;
            }
        }
        count++;

    } while(1);

    // For profiling
    REG_SW_CLEAR_PROFILING(env->pthis, SW_PROF_IRQ_E2A_RXDESC);
}

/**
 * ipc_host_radar_handler() - Handle the reception of radar events
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_RADAR is set
 */
static void ipc_host_radar_handler(struct ipc_host_env_tag *env)
{
#ifdef CONFIG_SIWIFI_RADAR
    // LMAC has triggered an IT saying that a radar event has been sent to upper layer.
    // Then we first need to check the validity of the current msg buf, and the validity
    // of the next buffers too, because it is likely that several buffers have been
    // filled within the time needed for this irq handling
    // call the external function to indicate that a RX packet is received
    spin_lock(&((struct siwifi_hw *)env->pthis)->radar.lock);
    while (env->cb.recv_radar_ind(env->pthis,
              env->ipc_host_radarbuf_array[env->ipc_host_radarbuf_idx].hostid) == 0)
        ;
    spin_unlock(&((struct siwifi_hw *)env->pthis)->radar.lock);
#endif /* CONFIG_SIWIFI_RADAR */
}

/**
 * ipc_host_unsup_rx_vec_handler() - Handle the reception of unsupported rx vector
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_UNSUP_RX_VEC is set
 */
static void ipc_host_unsup_rx_vec_handler(struct ipc_host_env_tag *env)
{
    while (env->cb.recv_unsup_rx_vec_ind(env->pthis,
              env->ipc_host_unsuprxvecbuf_array[env->ipc_host_unsuprxvecbuf_idx].hostid) == 0)
        ;
}

/**
 * ipc_host_msg_handler() - Handler for firmware message
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_MSG is set
 */
static void ipc_host_msg_handler(struct ipc_host_env_tag *env)
{
    // For profiling
    REG_SW_SET_PROFILING(env->pthis, SW_PROF_IRQ_E2A_MSG);

    // LMAC has triggered an IT saying that a message has been sent to upper layer.
    // Then we first need to check the validity of the current msg buf, and the validity
    // of the next buffers too, because it is likely that several buffers have been
    // filled within the time needed for this irq handling
    // call the external function to indicate that a RX packet is received
    while (env->cb.recv_msg_ind(env->pthis,
                    env->ipc_host_msgbuf_array[env->ipc_host_msge2a_idx].hostid) == 0)
        ;


    // For profiling
    REG_SW_CLEAR_PROFILING(env->pthis, SW_PROF_IRQ_E2A_MSG);

}

/**
 * ipc_host_msgack_handler() - Handle the reception of message acknowledgement
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_MSG_ACK is set
 */
static void ipc_host_msgack_handler(struct ipc_host_env_tag *env)
{
    void *hostid = env->msga2e_hostid;

    ASSERT_ERR(hostid);
    ASSERT_ERR(env->msga2e_cnt == (((struct lmac_msg *)(&env->shared->msg_a2e_buf.msg))->src_id & 0xFF));

    env->msga2e_hostid = NULL;
    env->msga2e_cnt++;
    env->cb.recv_msgack_ind(env->pthis, hostid);
}

/**
 * ipc_host_dbg_handler() - Handle the reception of Debug event
 *
 * @env: pointer to the IPC Host environment
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_DBG is set
 */
static void ipc_host_dbg_handler(struct ipc_host_env_tag *env)
{
    // For profiling
    REG_SW_SET_PROFILING(env->pthis, SW_PROF_IRQ_E2A_DBG);

    // LMAC has triggered an IT saying that a DBG message has been sent to upper layer.
    // Then we first need to check the validity of the current buffer, and the validity
    // of the next buffers too, because it is likely that several buffers have been
    // filled within the time needed for this irq handling
    // call the external function to indicate that a RX packet is received
    while(env->cb.recv_dbg_ind(env->pthis,
            env->ipc_host_dbgbuf_array[env->ipc_host_dbg_idx].hostid) == 0)
        ;

    // For profiling
    REG_SW_CLEAR_PROFILING(env->pthis, SW_PROF_IRQ_E2A_DBG);
}

/**
 * ipc_host_tx_cfm_handler() - Handle the reception of TX confirmation
 *
 * @env: pointer to the IPC Host environment
 * @queue_idx: index of the hardware on which the confirmation has been received
 * @user_pos: index of the user position
 *
 * Called from general IRQ handler when status %IPC_IRQ_E2A_TXCFM is set
 */
static void ipc_host_tx_cfm_handler(struct ipc_host_env_tag *env,
                                    const int queue_idx, const int user_pos)
{
    // TX confirmation descriptors have been received
    REG_SW_SET_PROFILING(env->pthis, SW_PROF_IRQ_E2A_TXCFM);
    while (1)
    {
        // Get the used index and increase it. We do the increase before knowing if the
        // current buffer is confirmed because the callback function may call the
        // ipc_host_txdesc_get() in case flow control was enabled and the index has to be
        // already at the good value to ensure that the test of FIFO full is correct
        uint32_t used_idx = env->txdesc_used_idx[queue_idx][user_pos]++;
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
        uint32_t used_idx_mod = used_idx % nx_txdesc_cnt[queue_idx];
#else
        uint32_t used_idx_mod = used_idx & nx_txdesc_cnt_msk[queue_idx];
#endif
        void *host_id = env->tx_host_id[queue_idx][user_pos][used_idx_mod];
#ifdef NEW_SCHEDULE
        volatile struct txdesc_host *txdesc = env->txdesc[queue_idx][user_pos] + used_idx_mod;
        if (txdesc->ready != IPC_TXDESC_CONFIRMED) {
            // No more confirmations, so put back the used index at its initial value
            env->txdesc_used_idx[queue_idx][user_pos] = used_idx;
            break;
        }

        if (env->cb.send_data_cfm_burst(env->pthis, host_id, txdesc->length) != 0)
        {
            //this can not happen!!
            ASSERT_ERR(1);
        }
        txdesc->ready = IPC_TXDESC_FREE;
#else

        // Reset the host id in the array
        env->tx_host_id[queue_idx][user_pos][used_idx_mod] = NULL;

        // call the external function to indicate that a TX packet is freed
        if (host_id == NULL)
        {
            // No more confirmations, so put back the used index at its initial value
            env->txdesc_used_idx[queue_idx][user_pos] = used_idx;
            break;
        }

        if (env->cb.send_data_cfm(env->pthis, host_id) != 0)
        {
            // No more confirmations, so put back the used index at its initial value
            env->txdesc_used_idx[queue_idx][user_pos] = used_idx;
            env->tx_host_id[queue_idx][user_pos][used_idx_mod] = host_id;
            // and exit the loop
            break;
        }
#endif

        REG_SW_SET_PROFILING_CHAN(env->pthis, SW_PROF_CHAN_CTXT_CFM_HDL_BIT);
        REG_SW_CLEAR_PROFILING_CHAN(env->pthis, SW_PROF_CHAN_CTXT_CFM_HDL_BIT);
    }

    REG_SW_CLEAR_PROFILING(env->pthis, SW_PROF_IRQ_E2A_TXCFM);
}

/**
 ******************************************************************************
 */
bool ipc_host_tx_frames_pending(struct ipc_host_env_tag *env)
{
    int i, j;
    bool tx_frames_pending = false;

    for (i = 0; (i < IPC_TXQUEUE_CNT) && !tx_frames_pending; i++)
    {
        for (j = 0; j < nx_txuser_cnt[i]; j++)
        {
            uint32_t used_idx = env->txdesc_used_idx[i][j];
            uint32_t free_idx = env->txdesc_free_idx[i][j];

            // Check if this queue is empty or not
            if (used_idx != free_idx)
            {
                // The queue is not empty, update the flag and exit
                tx_frames_pending = true;
                break;
            }
        }
    }

    return (tx_frames_pending);
}

/**
 ******************************************************************************
 */
void *ipc_host_tx_flush(struct ipc_host_env_tag *env, const int queue_idx, const int user_pos)
{
    uint32_t used_idx;
#ifdef NEW_SCHEDULE
    volatile struct txdesc_host *txdesc;
    used_idx = env->txdesc_used_idx[queue_idx][user_pos];
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
    txdesc = env->txdesc[queue_idx][user_pos] + (used_idx % nx_txdesc_cnt[queue_idx]);
#else
    txdesc = env->txdesc[queue_idx][user_pos] + (used_idx & nx_txdesc_cnt_msk[queue_idx]);
#endif
    if (txdesc->ready != IPC_TXDESC_FREE) {
        env->txdesc_used_idx[queue_idx][user_pos]++;
        txdesc->ready = IPC_TXDESC_FREE;
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
        return (env->tx_host_id[queue_idx][user_pos][used_idx % nx_txdesc_cnt[queue_idx]]);
#else
        return (env->tx_host_id[queue_idx][user_pos][used_idx & nx_txdesc_cnt_msk[queue_idx]]);
#endif
    } else {
        return NULL;
    }
#else
    void *host_id;
    used_idx = env->txdesc_used_idx[queue_idx][user_pos];
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
    host_id = env->tx_host_id[queue_idx][user_pos][used_idx % nx_txdesc_cnt[queue_idx]];
#else
    host_id = env->tx_host_id[queue_idx][user_pos][used_idx & nx_txdesc_cnt_msk[queue_idx]];
#endif
    // call the external function to indicate that a TX packet is freed
    if (host_id)
    {
        // Reset the host id in the array
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
        env->tx_host_id[queue_idx][user_pos][used_idx % nx_txdesc_cnt[queue_idx]] = NULL;
#else
        env->tx_host_id[queue_idx][user_pos][used_idx & nx_txdesc_cnt_msk[queue_idx]] = NULL;
#endif

        // Increment the used index
        env->txdesc_used_idx[queue_idx][user_pos]++;
    }

    return (host_id);
#endif
}

#ifdef NEW_SCHEDULE
void ipc_host_deinit(struct ipc_host_env_tag *env)
{
    int i;
    for (i = 0; i < CONFIG_USER_MAX; i++)
    {
        DEALLOC_HOST_ID_RES(0, i);
        DEALLOC_HOST_ID_RES(1, i);
        DEALLOC_HOST_ID_RES(2, i);
        DEALLOC_HOST_ID_RES(3, i);
    }

    #if NX_TXQ_CNT == 5
    DEALLOC_HOST_ID_RES(4, 0);
    #endif
}
#endif

/**
 ******************************************************************************
 */
void ipc_host_init(struct ipc_host_env_tag *env,
                  struct ipc_host_cb_tag *cb,
                  struct ipc_shared_env_tag *shared_env_ptr,
                  void *reg_base,
                  void *pthis)
{
    unsigned int i;

    // Reset the environments
    // Reset the IPC Shared memory
    memset(shared_env_ptr, 0, sizeof(struct ipc_shared_env_tag));
    // Reset the IPC Host environment
    //memset(env, 0, sizeof(struct ipc_host_env_tag));

    // Initialize the shared environment pointer
    env->shared = shared_env_ptr;

    // Save the register access base addr
    env->reg_base = reg_base;
    // Save the callbacks in our own environment
    env->cb = *cb;

    printk("addr cb env: %p\n", (void *)&env);
    // Save the pointer to the register base
    env->pthis = pthis;

    // Initialize buffers numbers and buffers sizes needed for DMA Receptions
    #ifdef CONFIG_WIFI_LITE_MEMORY
    printk("addr env_mod_params: %p\n", (void *)&env);
    if (env->mod_params->is_hb)
        env->rx_bufnb = RX_HB_RXSKB_CNT;
    else
        env->rx_bufnb = RX_LB_RXSKB_CNT;
    #else
    env->rx_bufnb = IPC_RXBUF_CNT;
    #endif
    env->max_rx_process_time = 1024;

    env->rxdesc_nb = IPC_RXDESC_CNT;
    #ifdef CONFIG_SIWIFI_RADAR
    env->radar_bufnb = IPC_RADARBUF_CNT;
    env->radar_bufsz = sizeof(struct radar_pulse_array_desc);
    #endif
    env->unsuprxvec_bufnb = IPC_UNSUPRXVECBUF_CNT;
    env->unsuprxvec_bufsz = max(sizeof(struct rx_vector_desc), (size_t) RADIOTAP_HDR_MAX_LEN) +
                            RADIOTAP_HDR_VEND_MAX_LEN +  UNSUP_RX_VEC_DATA_LEN;
    env->ipc_e2amsg_bufnb = IPC_MSGE2A_BUF_CNT;
    env->ipc_e2amsg_bufsz = sizeof(struct ipc_e2a_msg);
    env->ipc_dbg_bufnb = IPC_DBGBUF_CNT;
    env->ipc_dbg_bufsz = sizeof(struct ipc_dbg_msg);

    for (i = 0; i < CONFIG_USER_MAX; i++)
    {
        // Initialize the pointers to the hostid arrays
        #ifdef NEW_SCHEDULE
        ALLOC_HOST_ID_RES(0, i);
        ALLOC_HOST_ID_RES(1, i);
        ALLOC_HOST_ID_RES(2, i);
        ALLOC_HOST_ID_RES(3, i);
        #endif

        env->tx_host_id[0][i] = env->tx_host_id0[i];
        env->tx_host_id[1][i] = env->tx_host_id1[i];
        env->tx_host_id[2][i] = env->tx_host_id2[i];
        env->tx_host_id[3][i] = env->tx_host_id3[i];
        #if NX_TXQ_CNT == 5
        env->tx_host_id[4][i] = NULL;
        #endif

        // Initialize the pointers to the TX descriptor arrays
        env->txdesc[0][i] = shared_env_ptr->txdesc0[i];
        env->txdesc[1][i] = shared_env_ptr->txdesc1[i];
        env->txdesc[2][i] = shared_env_ptr->txdesc2[i];
        env->txdesc[3][i] = shared_env_ptr->txdesc3[i];
        #if NX_TXQ_CNT == 5
        env->txdesc[4][i] = NULL;
        #endif
    }

    #if NX_TXQ_CNT == 5
    #ifdef NEW_SCHEDULE
    ALLOC_HOST_ID_RES(4, 0);
    #endif
    env->tx_host_id[4][0] = env->tx_host_id4[0];
    env->txdesc[4][0] = shared_env_ptr->txdesc4[0];
    #endif
}

/**
 ******************************************************************************
 */
void ipc_host_patt_addr_push(struct ipc_host_env_tag *env, uint32_t addr)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Copy the address
    shared_env_ptr->pattern_addr = addr;
}

void ipc_host_dying_gasp_addr_push(struct ipc_host_env_tag *env, uint32_t addr)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Copy the address
    shared_env_ptr->dying_gasp_addr = addr;
}

/**
 ******************************************************************************
 */
int ipc_host_rxbuf_push(struct ipc_host_env_tag *env,
                        uint32_t hostid,
                        uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    REG_SW_CLEAR_HOSTBUF_IDX_PROFILING(env->pthis);
    REG_SW_SET_HOSTBUF_IDX_PROFILING(env->pthis, env->ipc_host_rxbuf_idx);


    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->host_rxbuf[env->ipc_host_rxbuf_idx].hostid   = hostid;
    shared_env_ptr->host_rxbuf[env->ipc_host_rxbuf_idx].dma_addr = hostbuf;

    // Signal to the embedded CPU that at least one buffer is available
    ipc_app2emb_trigger_set(IPC_WIFI_BASE(env->mod_params->is_hb), IPC_IRQ_A2E_RXBUF_BACK);

    // Increment the array index
    env->ipc_host_rxbuf_idx = (env->ipc_host_rxbuf_idx +1)%IPC_RXBUF_CNT;

    return (0);
}

/**
 ******************************************************************************
 */
int ipc_host_rxdesc_push(struct ipc_host_env_tag *env, void *hostid,
                         uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Reset the RX Descriptor DMA Address and increment the counter
    env->ipc_host_rxdesc_array[env->ipc_host_rxdesc_idx].dma_addr = hostbuf;
    env->ipc_host_rxdesc_array[env->ipc_host_rxdesc_idx].hostid = hostid;

    shared_env_ptr->host_rxdesc[env->ipc_host_rxdesc_idx].dma_addr = hostbuf;

    // Signal to the embedded CPU that at least one descriptor is available
    ipc_app2emb_trigger_set(IPC_WIFI_BASE(env->mod_params->is_hb), IPC_IRQ_A2E_RXDESC_BACK);

    env->ipc_host_rxdesc_idx = (env->ipc_host_rxdesc_idx + 1) % IPC_RXDESC_CNT;

    return (0);
}

#ifdef CONFIG_SIWIFI_RADAR
/**
 ******************************************************************************
 */
int ipc_host_radarbuf_push(struct ipc_host_env_tag *env, void *hostid,
                           uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Save the hostid and the hostbuf in global array
    env->ipc_host_radarbuf_array[env->ipc_host_radarbuf_idx].hostid = hostid;
    env->ipc_host_radarbuf_array[env->ipc_host_radarbuf_idx].dma_addr = hostbuf;

    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->radarbuf_hostbuf[env->ipc_host_radarbuf_idx] = hostbuf;

    // Increment the array index
    env->ipc_host_radarbuf_idx = (env->ipc_host_radarbuf_idx +1)%IPC_RADARBUF_CNT;

    return (0);
}
#endif

/**
 ******************************************************************************
 */

int ipc_host_unsup_rx_vec_buf_push(struct ipc_host_env_tag *env,
                                   void *hostid,
                                   uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    env->ipc_host_unsuprxvecbuf_array[env->ipc_host_unsuprxvecbuf_idx].hostid = hostid;
    env->ipc_host_unsuprxvecbuf_array[env->ipc_host_unsuprxvecbuf_idx].dma_addr = hostbuf;

    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->unsuprxvecbuf_hostbuf[env->ipc_host_unsuprxvecbuf_idx] = hostbuf;

    // Increment the array index
    env->ipc_host_unsuprxvecbuf_idx = (env->ipc_host_unsuprxvecbuf_idx + 1)%IPC_UNSUPRXVECBUF_CNT;

    return (0);
}

/**
 ******************************************************************************
 */
int ipc_host_msgbuf_push(struct ipc_host_env_tag *env, void *hostid,
                         uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Save the hostid and the hostbuf in global array
    env->ipc_host_msgbuf_array[env->ipc_host_msge2a_idx].hostid = hostid;
    env->ipc_host_msgbuf_array[env->ipc_host_msge2a_idx].dma_addr = hostbuf;

    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->msg_e2a_hostbuf_addr[env->ipc_host_msge2a_idx] = hostbuf;

    // Increment the array index
    env->ipc_host_msge2a_idx = (env->ipc_host_msge2a_idx +1)%IPC_MSGE2A_BUF_CNT;

    return (0);
}

/**
 ******************************************************************************
 */
int ipc_host_dbgbuf_push(struct ipc_host_env_tag *env, void *hostid,
                         uint32_t hostbuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Save the hostid and the hostbuf in global array
    env->ipc_host_dbgbuf_array[env->ipc_host_dbg_idx].hostid = hostid;
    env->ipc_host_dbgbuf_array[env->ipc_host_dbg_idx].dma_addr = hostbuf;

    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->dbg_hostbuf_addr[env->ipc_host_dbg_idx] = hostbuf;

    // Increment the array index
    env->ipc_host_dbg_idx = (env->ipc_host_dbg_idx +1)%IPC_DBGBUF_CNT;

    return (0);
}

/**
 ******************************************************************************
 */
void ipc_host_dbginfobuf_push(struct ipc_host_env_tag *env, uint32_t infobuf)
{
    struct ipc_shared_env_tag *shared_env_ptr = env->shared;

    // Copy the hostbuf (DMA address) in the ipc shared memory
    shared_env_ptr->la_dbginfo_addr = infobuf;
}

/**
 ******************************************************************************
 */
volatile struct txdesc_host *ipc_host_txdesc_get(struct ipc_host_env_tag *env, const int queue_idx, const int user_pos)
{
    volatile struct txdesc_host *txdesc_free;
    uint32_t used_idx = env->txdesc_used_idx[queue_idx][user_pos];
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos];

    ASSERT_ERR(queue_idx < IPC_TXQUEUE_CNT);
    ASSERT_ERR((free_idx - used_idx) <= nx_txdesc_cnt[queue_idx]);

    // Check if a free descriptor is available
    if (free_idx != (used_idx + nx_txdesc_cnt[queue_idx]))
    {
        // Get the pointer to the first free descriptor
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
        txdesc_free = env->txdesc[queue_idx][user_pos] + (free_idx % nx_txdesc_cnt[queue_idx]);
#else
        txdesc_free = env->txdesc[queue_idx][user_pos] + (free_idx & nx_txdesc_cnt_msk[queue_idx]);
#endif
        ASSERT_ERR(txdesc_free->ready == IPC_TXDESC_FREE);
    }
    else
    {
        txdesc_free = NULL;
    }

    return txdesc_free;
}
#ifdef NEW_SCHEDULE
void ipc_host_txdesc_map(struct ipc_host_env_tag *env,
                        const int user_pos,
                        const int hwq_index,
                        const int pos,
                        uint32_t phy_addr)
{
    volatile struct txdesc_host *txdesc_host = env->txdesc[hwq_index][user_pos] + pos;
    txdesc_host->ready = IPC_TXDESC_FREE;
    txdesc_host->length = 0;
    txdesc_host->phy_addr = phy_addr;
}

/**
 ******************************************************************************
 */
void ipc_host_txdesc_push_burst(struct ipc_host_env_tag *env, const int queue_idx,
                          const int user_pos, void (*store_host_id)(void *target, void *src), void *host_id)
{
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos] % nx_txdesc_cnt[queue_idx];
#else
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos] & nx_txdesc_cnt_msk[queue_idx];
#endif
    volatile struct txdesc_host *txdesc_pushed = env->txdesc[queue_idx][user_pos] + free_idx;


    // Descriptor is now ready
    txdesc_pushed->ready = IPC_TXDESC_READY;

    // Save the host id in the environment
    store_host_id(env->tx_host_id[queue_idx][user_pos][free_idx], host_id);

    // Increment the index
    env->txdesc_free_idx[queue_idx][user_pos]++;


    // trigger interrupt!!!
    //REG_SW_SET_PROFILING(env->pthis, CO_BIT(queue_idx+SW_PROF_IRQ_A2E_TXDESC_FIRSTBIT));
    ipc_app2emb_trigger_setf(IPC_WIFI_BASE, CO_BIT(user_pos + queue_idx * CONFIG_USER_MAX +
                                                 IPC_IRQ_A2E_TXDESC_FIRSTBIT));
}
#endif
/**
 ******************************************************************************
 */
void ipc_host_txdesc_push(struct ipc_host_env_tag *env, const int queue_idx,
                          const int user_pos, void *host_id)
{
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos] % nx_txdesc_cnt[queue_idx];
#else
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos] & nx_txdesc_cnt_msk[queue_idx];
#endif
    volatile struct txdesc_host *txdesc_pushed = env->txdesc[queue_idx][user_pos] + free_idx;


    // Descriptor is now ready
    txdesc_pushed->ready = IPC_TXDESC_READY;

    // Save the host id in the environment
    env->tx_host_id[queue_idx][user_pos][free_idx] = host_id;

    // Increment the index
    env->txdesc_free_idx[queue_idx][user_pos]++;

    // trigger interrupt!!!
    //REG_SW_SET_PROFILING(env->pthis, CO_BIT(queue_idx+SW_PROF_IRQ_A2E_TXDESC_FIRSTBIT));
    ipc_app2emb_trigger_setf(IPC_WIFI_BASE(env->mod_params->is_hb), CO_BIT(user_pos + queue_idx * CONFIG_USER_MAX +
                                                 IPC_IRQ_A2E_TXDESC_FIRSTBIT));
}

#ifdef SIWIFI_DEBUG_BUS_TRAP
#define _m32c0_mfc0(reg, sel) \
    __extension__ ({ \
            register unsigned long __r; \
            __asm__ __volatile ("mfc0 %0,$%1,%2" \
                : "=d" (__r) \
                : "JK" (reg), "JK" (sel)); \
            __r; \
            })

#define _m32c0_mtc0(reg, sel, val) \
    do { \
        __asm__ __volatile ("%(mtc0 %z0,$%1,%2; ehb%)" \
                : \
                : "dJ" ((uint32_t)(val)), "JK" (reg), "JK" (sel) \
                : "memory"); \
    } while (0)

void init_cpu2_debuginfo(void)
{
    static int init_debug_bus_trap = 0;
    if (!init_debug_bus_trap) {
        init_debug_bus_trap = 1;
        _m32c0_mtc0(18, 0, 0x90000007 | 0 << 12);
        _m32c0_mtc0(18, 1, 0x90000007 | 1 << 12);
        _m32c0_mtc0(18, 2, 0x90000007 | 0 << 12);
        _m32c0_mtc0(18, 3, 0x90000007 | 1 << 12);

        _m32c0_mtc0(19, 0, 1 << 30 | 0x1ff << 3);
        _m32c0_mtc0(19, 1, 1 << 30 | 0x1ff << 3);
        _m32c0_mtc0(19, 2, 1 << 30 | 0x1ff << 3);
        _m32c0_mtc0(19, 3, 1 << 30 | 0x1ff << 3);

        printk("DEBUG BUS TRAP low: 0x%lx, 0x%lx, 0x%lx, 0x%lx cpu=%d\n",
                _m32c0_mfc0(18, 0), _m32c0_mfc0(18, 1), _m32c0_mfc0(18, 2), _m32c0_mfc0(18, 3), smp_processor_id());
        printk("DEBUG BUS TRAP high: 0x%lx, 0x%lx, 0x%lx, 0x%lx  cpu=%d\n",
                _m32c0_mfc0(19, 0), _m32c0_mfc0(19, 1), _m32c0_mfc0(19, 2), _m32c0_mfc0(19, 3), smp_processor_id());
    }
}
#endif

void ipc_host_status_clear(struct ipc_host_env_tag *env, uint32_t status)
{
    // Acknowledge the pending interrupts
    ipc_emb2app_ack_clear(IPC_WIFI_BASE(env->mod_params->is_hb), status);
    // And re-read the status, just to be sure that the acknowledgment is
    // effective when we start the interrupt handling
    ipc_emb2app_status_get(IPC_WIFI_BASE(env->mod_params->is_hb));
}

/**
 ******************************************************************************
 */
void ipc_host_irq(struct ipc_host_env_tag *env, uint32_t status)
{
#ifdef SIWIFI_DEBUG_BUS_TRAP
    init_cpu2_debuginfo();
#endif

    ipc_host_status_clear(env, status);

    if (status & IPC_IRQ_E2A_MSG_ACK)
    {
        ipc_host_msgack_handler(env);
    }
    if (status & IPC_IRQ_E2A_MSG)
    {
        ipc_host_msg_handler(env);
    }
    if (status & IPC_IRQ_E2A_TXCFM)
    {
        int i;

        /*
         * cb_lock is to protect variable vif which is used in function siwifi_set_traffic_status
         * but some places,for example,in func "siwifi_rx_chan_switch_ind",first use cb_lock, then use siwifi_hw->tx_lock
         * in func "siwifi_txq_tdls_sta_start";
         * so we must first use cb_lock, then use tx_lock, otherwise will deadlock
         * siwifi_set_traffic_status is only used in fullmac
         * */
        spin_lock(&((struct siwifi_hw *)env->pthis)->cb_lock);
#ifdef __KERNEL__
        spin_lock(&((struct siwifi_hw *)env->pthis)->tx_lock);
#endif
        // handle the TX confirmation reception
        for (i = 0; i < IPC_TXQUEUE_CNT; i++)
        {
            int j = 0;
#ifdef CONFIG_SIWIFI_MUMIMO_TX
            for (; j < nx_txuser_cnt[i]; j++)
#endif
            {
                uint32_t q_bit = CO_BIT(j + i * CONFIG_USER_MAX + IPC_IRQ_E2A_TXCFM_POS);
                if (status & q_bit)
                {
                    // handle the confirmation
                    ipc_host_tx_cfm_handler(env, i, j);
                }
            }
        }
#ifdef __KERNEL__
        spin_unlock(&((struct siwifi_hw *)env->pthis)->tx_lock);
#endif
        spin_unlock(&((struct siwifi_hw *)env->pthis)->cb_lock);
    }
    if (status & IPC_IRQ_E2A_RADAR)
    {
        // handle the radar event reception
        ipc_host_radar_handler(env);
    }

    if (status & IPC_IRQ_E2A_UNSUP_RX_VEC)
    {
        // handle the unsupported rx vector reception
        ipc_host_unsup_rx_vec_handler(env);
    }

    if (status & IPC_IRQ_E2A_DBG)
    {
        ipc_host_dbg_handler(env);
    }

    if (status & IPC_IRQ_E2A_TBTT_PRIM)
    {
        env->cb.prim_tbtt_ind(env->pthis);
    }

    if (status & IPC_IRQ_E2A_TBTT_SEC)
    {
        env->cb.sec_tbtt_ind(env->pthis);
    }
}

/**
 ******************************************************************************
 */
int ipc_host_msg_push(struct ipc_host_env_tag *env, void *msg_buf, uint16_t len)
{
    //int i;
    uint32_t *src, *dst;

    REG_SW_SET_PROFILING(env->pthis, SW_PROF_IPC_MSGPUSH);

    ASSERT_ERR(!env->msga2e_hostid);
    ASSERT_ERR(round_up(len, 4) <= sizeof(env->shared->msg_a2e_buf.msg));

    // Copy the message into the IPC MSG buffer
#ifdef __KERNEL__
    src = (uint32_t*)((struct siwifi_cmd *)msg_buf)->a2e_msg;
#else
    src = (uint32_t*) msg_buf;
#endif
    dst = (uint32_t*)&(env->shared->msg_a2e_buf.msg);

    // Copy the message in the IPC queue
    //for (i=0; i<len; i+=4)
    //{
    //    *dst++ = *src++;
    //}
    memcpy(dst, src, len);

    env->msga2e_hostid = msg_buf;

    // Trigger the irq to send the message to EMB
    ipc_app2emb_trigger_set(IPC_WIFI_BASE(env->mod_params->is_hb), IPC_IRQ_A2E_MSG);

    REG_SW_CLEAR_PROFILING(env->pthis, SW_PROF_IPC_MSGPUSH);

    return (0);
}

/**
 ******************************************************************************
 */
void ipc_host_enable_irq(struct ipc_host_env_tag *env, uint32_t value)
{
    // Enable the handled interrupts
    ipc_emb2app_unmask_set(IPC_WIFI_BASE(env->mod_params->is_hb), value);
}

/**
 ******************************************************************************
 */
void ipc_host_disable_irq(struct ipc_host_env_tag *env, uint32_t value)
{
    // Enable the handled interrupts
    ipc_emb2app_unmask_clear(IPC_WIFI_BASE(env->mod_params->is_hb), value);
}

/**
 ******************************************************************************
 */
uint32_t ipc_host_get_status(struct ipc_host_env_tag *env)
{
    volatile uint32_t status;

    status = ipc_emb2app_status_get(IPC_WIFI_BASE(env->mod_params->is_hb));

    return status;
}

/**
 ******************************************************************************
 */
uint32_t ipc_host_get_rawstatus(struct ipc_host_env_tag *env)
{
    volatile uint32_t rawstatus;

    rawstatus = ipc_emb2app_rawstatus_get(IPC_WIFI_BASE(env->mod_params->is_hb));

    return rawstatus;
}

int32_t ipc_process_hostids(struct ipc_host_env_tag *env, uint32_t queue_idx,
                            const int user_pos, void (*process_host_id)(void *host_id,
                            uint32_t hwq_index, uint32_t data), uint32_t data)
{
    //release ref from ipc_host
    uint32_t used_idx = env->txdesc_used_idx[queue_idx][user_pos];
    uint32_t free_idx = env->txdesc_free_idx[queue_idx][user_pos];
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
    void *host_id = env->tx_host_id[queue_idx][user_pos][used_idx % nx_txdesc_cnt[queue_idx]];
#else
    void *host_id = env->tx_host_id[queue_idx][user_pos][used_idx & nx_txdesc_cnt_msk[queue_idx]];
#endif
    int32_t ret = -1;
    while (used_idx != free_idx) {
        if (host_id && process_host_id)
            process_host_id(host_id, queue_idx, data);
        used_idx++;
#ifdef CONFIG_SIWIFI_DONT_USE_TXDESC_MASK
        host_id = env->tx_host_id[queue_idx][user_pos][used_idx % nx_txdesc_cnt[queue_idx]];
#else
        host_id = env->tx_host_id[queue_idx][user_pos][used_idx & nx_txdesc_cnt_msk[queue_idx]];
#endif
    }
    return ret;
}
