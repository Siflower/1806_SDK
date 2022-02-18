/*
 * =====================================================================================
 *
 *       Filename:  wifi_pdma.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/15/2016 10:15:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  franklin (), franklin.wang@siflower.com.cn
 *        Company:  Siflower Communication Tenology Co.,Ltd
 *
 * =====================================================================================
 */

#ifndef _WIFI_PDMA_H_
#define _WIFI_PDMA_H_

#include "co_int.h"
// for CO_BIT macro
#include "co_math.h"
// for __INLINE
#include "wifi_compiler.h"
// for register address
#include "wifi_pdma_reg.h"


/** @defgroup DMA DMA
 *  @ingroup PLATFORM_DRIVERS
 *  @brief DMA utility functions
 *  @{
 */

/// DMA channel indexes
enum
{
    /// Upload control channel
    WIFI_PDMA_CHANNEL_CTRL_RX,
    /// Upload data channel
    WIFI_PDMA_CHANNEL_DATA_RX,
    /// Download control channel
    WIFI_PDMA_CHANNEL_CTRL_TX,
    /// Download data channel
    WIFI_PDMA_CHANNEL_DATA_TX,
    /// Number of DMA channels
    WIFI_PDMA_CHANNEL_MAX,
};

/** DMA LLI assignment.  This mapping is more important than it looks because of the way
 *  the interrupts are mapped on the reference IP interrupt controller:
 *   - LLI0 to 3 are mapped to a single interrupt and therefore the handler of the TX can
 *     be called directly since the interrupt controller is indexed
 *   - LLI4 to 7 are mapped to a single interrupt and therefore the handler of the RX can
 *     be called directly since the interrupt controller is indexed
 */
enum
{
    /// LLI for BK TX queue downloads
    WIFI_PMDA_LLI_DATA_AC0_TX,
    /// LLI for BE TX queue downloads
    WIFI_PMDA_LLI_DATA_AC1_TX,
    /// LLI for VI TX queue downloads
    WIFI_PMDA_LLI_DATA_AC2_TX,
    /// LLI for VO TX queue downloads
    WIFI_PMDA_LLI_DATA_AC3_TX,
    /// LLI for BCN TX queue downloads
    WIFI_PMDA_LLI_DATA_BCN_TX,
    /// LLI for RX transfers
    WIFI_PMDA_LLI_DATA_RX0,
    /// LLI for kernel messages upload
    WIFI_PMDA_LLI_MSG,
    /// LLI for debug messages upload
    WIFI_PMDA_LLI_DBG,
    /// LLI for BK TX queue confirmation uploads
    WIFI_PMDA_LLI_CFM_AC0_TX,
    /// LLI for BE TX queue confirmation uploads
    WIFI_PMDA_LLI_CFM_AC1_TX,
    /// LLI for VI TX queue confirmation uploads
    WIFI_PMDA_LLI_CFM_AC2_TX,
    /// LLI for VO TX queue confirmation uploads
    WIFI_PMDA_LLI_CFM_AC3_TX,
    /// LLI for BCN TX queue confirmation uploads
    WIFI_PMDA_LLI_CFM_BCN_TX,
    /// LLI for general purpose DMA downloads
    WIFI_PMDA_LLI_GP_DL,
    /// LLI for debug dump uploads
    WIFI_PMDA_LLI_DBG_DUMP,
    /// LLI for radar pulses uploads
    WIFI_PMDA_LLI_RD,
    /// Number of LLI used
    WIFI_PMDA_LLI_MAX
};


/// Mask of the DMA LLI IRQs status bits that are assigned for TX
#define WIFI_PDMA_LLI_TX_MASK         0x001F
/// Mask of the DMA LLI IRQs status bits that are assigned for CFM
#define WIFI_PDMA_LLI_CFM_MASK        0x1F00

/// Mask of the DMA LLI IRQs status bits that are assigned for RX
#define WIFI_PDMA_LLI_RX_MASK         0x0020

/// Indicate that an LLI counter must be incremented at the end of the LLI process
#define WIFI_PDMA_LLI_COUNTER_EN      0x0010
/// Mask of the LLI counter index to increment at the end of the LLI process
#define WIFI_PDMA_LLI_COUNTER_MASK    0x000F
/// Bit position of the LLI counter index
#define WIFI_PDMA_LLI_COUNTER_POS     0

/// Indicate that an IRQ must be generated at the end of the LLI process
#define WIFI_PDMA_LLI_IRQ_EN          0x1000
/// Mask of the LLI IRQ index to generate at the end of the LLI process
#define WIFI_PDMA_LLI_IRQ_MASK        0x0F00
/// Bit position of the LLI IRQ index
#define WIFI_PDMA_LLI_IRQ_POS         8


/// Element in the pool of TX DMA bridge descriptors.
struct dma_desc
{
    /** Application subsystem address which is used as source address for DMA payload
      * transfer*/
    uint32_t            src;
    /** Points to the start of the embedded data buffer associated with this descriptor.
     *  This address acts as the destination address for the DMA payload transfer*/
    uint32_t            dest;
    /// Complete length of the buffer in memory
    uint16_t            length;
    /// Control word for the DMA engine (e.g. for interrupt generation)
    uint16_t            ctrl;
    /// Pointer to the next element of the chained list
    uint32_t            next;
};

/// Structure describing the DMA driver environment
struct dma_env_tag
{
    /*last DMA descriptor pushed for each channel, can point to descriptor already
    * deallocated, but then will not be use`d because root register will be NULL
    */
    volatile struct dma_desc *last_dma[WIFI_PDMA_CHANNEL_MAX];

};

/// DMA environment structure
extern struct dma_env_tag dma_env;

/**
 ****************************************************************************************
 * @brief Initialize the bridge DMA registers
 ****************************************************************************************
 */
void dma_init(void);


/**
 ****************************************************************************************
 * @brief Chains a chained list of descriptors in the DMA
 *
 * @param[in]   first First DMA descriptor of the list (filled by the caller)
 * @param[in]   last last DMA descriptor of the list (filled by the caller)
 * @param[in]   channel_idx Channel index
 *
 ****************************************************************************************
 */
void dma_push(struct dma_desc *first, struct dma_desc *last, int channel_idx);


/**
 ****************************************************************************************
 * @brief Disable an LLI IRQ.
 *
 * @param[in]   lli LLI IRQ index (must be in range 0..15)
 *
 ****************************************************************************************
 */
__INLINE void dma_lli_disable(int lli)
{
    dma_int_unmask_lli_irq_clearf(CO_BIT(lli));
}

/**
 ****************************************************************************************
 * @brief Enable an LLI IRQ.
 *
 * @param[in]   lli LLI IRQ index (must be in range 0..15)
 *
 ****************************************************************************************
 */
__INLINE void dma_lli_enable(int lli)
{
    dma_int_unmask_lli_irq_setf(CO_BIT(lli));
}


/**
 ****************************************************************************************
 * @brief Poll for an LLI IRQ.
 *
 * @param[in]   lli LLI IRQ index (must be in range 0..15)
 *
 ****************************************************************************************
 */
__INLINE void dma_lli_poll(int lli)
{
    while (!(dma_int_status_get() & CO_BIT(lli)));
}

#endif
