/**
 * @file reg_dma.h
 * @brief Definitions of the DMA HW block registers and register access functions.
 *
 * @defgroup WIFI_PDMA_REG WIFI_PDMA_REG
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the DMA HW block registers and register access functions.
 */
#ifndef _WIFI_PDMA_REG_H_
#define _WIFI_PDMA_REG_H_

#include "co_int.h"
#include "_wifi_pdma_reg.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the WIFI_PDMA_REG peripheral.
 */
#define WIFI_PDMA_REG_COUNT 49

/** @brief Decoding mask of the WIFI_PDMA_REG peripheral registers from the CPU point of view.
 */
#define WIFI_PDMA_REG_DECODING_MASK 0x000000FF

/**
 * @name CH_LLI_ROOT register definitions
 *
 * @{
 */

/// Address of the CH_LLI_ROOT register
#define DMA_CH_LLI_ROOT_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000000)
/// Offset of the CH_LLI_ROOT register from the base address
#define DMA_CH_LLI_ROOT_OFFSET 0x00000000
/// Index of the CH_LLI_ROOT register
#define DMA_CH_LLI_ROOT_INDEX  0x00000000
/// Reset value of the CH_LLI_ROOT register
#define DMA_CH_LLI_ROOT_RESET  0x00000000
/// Number of elements of the CH_LLI_ROOT register array
#define DMA_CH_LLI_ROOT_COUNT  4

/**
 * @brief Returns the current value of the CH_LLI_ROOT register.
 * The CH_LLI_ROOT register will be read and its value returned.
 * @param[in] reg_idx Index of the register
 * @return The current value of the CH_LLI_ROOT register.
 */
__INLINE uint32_t dma_ch_lli_root_get(int reg_idx)
{
    ASSERT_ERR(reg_idx <= 3);
    return REG_PL_RD(DMA_CH_LLI_ROOT_ADDR + reg_idx * 4);
}

/**
 * @brief Sets the CH_LLI_ROOT register to a value.
 * The CH_LLI_ROOT register will be written.
 * @param[in] reg_idx Index of the register
 * @param value - The value to write.
 */
__INLINE void dma_ch_lli_root_set(int reg_idx, uint32_t value)
{
    ASSERT_ERR(reg_idx <= 3);
    REG_PL_WR(DMA_CH_LLI_ROOT_ADDR + reg_idx * 4, value);
}

/// @}

/**
 * @name DMA_STATUS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31          CH4_STOPPED   0
 *     30       MID_STREAM_BSY   0
 *     29      DOWN_STREAM_BSY   0
 *     28        UP_STREAM_BSY   0
 *     27         ARB_Q4_VALID   0
 *     26         ARB_Q3_VALID   0
 *     25         ARB_Q2_VALID   0
 *     24         ARB_Q1_VALID   0
 *     23         ARB_Q0_VALID   0
 *  22:20        REQUEST_STATE   0x0
 *     19          CH3_STOPPED   0
 *     18          CH2_STOPPED   0
 *     17          CH1_STOPPED   0
 *     16          CH0_STOPPED   0
 *  15:00             OFT_FREE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the DMA_STATUS register
#define DMA_DMA_STATUS_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000010)
/// Offset of the DMA_STATUS register from the base address
#define DMA_DMA_STATUS_OFFSET 0x00000010
/// Index of the DMA_STATUS register
#define DMA_DMA_STATUS_INDEX  0x00000004
/// Reset value of the DMA_STATUS register
#define DMA_DMA_STATUS_RESET  0x00000000

/**
 * @brief Returns the current value of the DMA_STATUS register.
 * The DMA_STATUS register will be read and its value returned.
 * @return The current value of the DMA_STATUS register.
 */
__INLINE uint32_t dma_dma_status_get(void)
{
    return REG_PL_RD(DMA_DMA_STATUS_ADDR);
}

// field definitions
/// CH4_STOPPED field bit
#define DMA_CH4_STOPPED_BIT        ((uint32_t)0x80000000)
/// CH4_STOPPED field position
#define DMA_CH4_STOPPED_POS        31
/// MID_STREAM_BSY field bit
#define DMA_MID_STREAM_BSY_BIT     ((uint32_t)0x40000000)
/// MID_STREAM_BSY field position
#define DMA_MID_STREAM_BSY_POS     30
/// DOWN_STREAM_BSY field bit
#define DMA_DOWN_STREAM_BSY_BIT    ((uint32_t)0x20000000)
/// DOWN_STREAM_BSY field position
#define DMA_DOWN_STREAM_BSY_POS    29
/// UP_STREAM_BSY field bit
#define DMA_UP_STREAM_BSY_BIT      ((uint32_t)0x10000000)
/// UP_STREAM_BSY field position
#define DMA_UP_STREAM_BSY_POS      28
/// ARB_Q4_VALID field bit
#define DMA_ARB_Q4_VALID_BIT       ((uint32_t)0x08000000)
/// ARB_Q4_VALID field position
#define DMA_ARB_Q4_VALID_POS       27
/// ARB_Q3_VALID field bit
#define DMA_ARB_Q3_VALID_BIT       ((uint32_t)0x04000000)
/// ARB_Q3_VALID field position
#define DMA_ARB_Q3_VALID_POS       26
/// ARB_Q2_VALID field bit
#define DMA_ARB_Q2_VALID_BIT       ((uint32_t)0x02000000)
/// ARB_Q2_VALID field position
#define DMA_ARB_Q2_VALID_POS       25
/// ARB_Q1_VALID field bit
#define DMA_ARB_Q1_VALID_BIT       ((uint32_t)0x01000000)
/// ARB_Q1_VALID field position
#define DMA_ARB_Q1_VALID_POS       24
/// ARB_Q0_VALID field bit
#define DMA_ARB_Q0_VALID_BIT       ((uint32_t)0x00800000)
/// ARB_Q0_VALID field position
#define DMA_ARB_Q0_VALID_POS       23
/// REQUEST_STATE field mask
#define DMA_REQUEST_STATE_MASK     ((uint32_t)0x00700000)
/// REQUEST_STATE field LSB position
#define DMA_REQUEST_STATE_LSB      20
/// REQUEST_STATE field width
#define DMA_REQUEST_STATE_WIDTH    ((uint32_t)0x00000003)
/// CH3_STOPPED field bit
#define DMA_CH3_STOPPED_BIT        ((uint32_t)0x00080000)
/// CH3_STOPPED field position
#define DMA_CH3_STOPPED_POS        19
/// CH2_STOPPED field bit
#define DMA_CH2_STOPPED_BIT        ((uint32_t)0x00040000)
/// CH2_STOPPED field position
#define DMA_CH2_STOPPED_POS        18
/// CH1_STOPPED field bit
#define DMA_CH1_STOPPED_BIT        ((uint32_t)0x00020000)
/// CH1_STOPPED field position
#define DMA_CH1_STOPPED_POS        17
/// CH0_STOPPED field bit
#define DMA_CH0_STOPPED_BIT        ((uint32_t)0x00010000)
/// CH0_STOPPED field position
#define DMA_CH0_STOPPED_POS        16
/// OFT_FREE field mask
#define DMA_OFT_FREE_MASK          ((uint32_t)0x0000FFFF)
/// OFT_FREE field LSB position
#define DMA_OFT_FREE_LSB           0
/// OFT_FREE field width
#define DMA_OFT_FREE_WIDTH         ((uint32_t)0x00000010)

/// CH4_STOPPED field reset value
#define DMA_CH4_STOPPED_RST        0x0
/// MID_STREAM_BSY field reset value
#define DMA_MID_STREAM_BSY_RST     0x0
/// DOWN_STREAM_BSY field reset value
#define DMA_DOWN_STREAM_BSY_RST    0x0
/// UP_STREAM_BSY field reset value
#define DMA_UP_STREAM_BSY_RST      0x0
/// ARB_Q4_VALID field reset value
#define DMA_ARB_Q4_VALID_RST       0x0
/// ARB_Q3_VALID field reset value
#define DMA_ARB_Q3_VALID_RST       0x0
/// ARB_Q2_VALID field reset value
#define DMA_ARB_Q2_VALID_RST       0x0
/// ARB_Q1_VALID field reset value
#define DMA_ARB_Q1_VALID_RST       0x0
/// ARB_Q0_VALID field reset value
#define DMA_ARB_Q0_VALID_RST       0x0
/// REQUEST_STATE field reset value
#define DMA_REQUEST_STATE_RST      0x0
/// CH3_STOPPED field reset value
#define DMA_CH3_STOPPED_RST        0x0
/// CH2_STOPPED field reset value
#define DMA_CH2_STOPPED_RST        0x0
/// CH1_STOPPED field reset value
#define DMA_CH1_STOPPED_RST        0x0
/// CH0_STOPPED field reset value
#define DMA_CH0_STOPPED_RST        0x0
/// OFT_FREE field reset value
#define DMA_OFT_FREE_RST           0x0

/**
 * @brief Unpacks DMA_STATUS's fields from current value of the DMA_STATUS register.
 *
 * Reads the DMA_STATUS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4stopped - Will be populated with the current value of this field from the register.
 * @param[out] midstreambsy - Will be populated with the current value of this field from the register.
 * @param[out] downstreambsy - Will be populated with the current value of this field from the register.
 * @param[out] upstreambsy - Will be populated with the current value of this field from the register.
 * @param[out] arbq4valid - Will be populated with the current value of this field from the register.
 * @param[out] arbq3valid - Will be populated with the current value of this field from the register.
 * @param[out] arbq2valid - Will be populated with the current value of this field from the register.
 * @param[out] arbq1valid - Will be populated with the current value of this field from the register.
 * @param[out] arbq0valid - Will be populated with the current value of this field from the register.
 * @param[out] requeststate - Will be populated with the current value of this field from the register.
 * @param[out] ch3stopped - Will be populated with the current value of this field from the register.
 * @param[out] ch2stopped - Will be populated with the current value of this field from the register.
 * @param[out] ch1stopped - Will be populated with the current value of this field from the register.
 * @param[out] ch0stopped - Will be populated with the current value of this field from the register.
 * @param[out] oftfree - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_dma_status_unpack(uint8_t* ch4stopped, uint8_t* midstreambsy, uint8_t* downstreambsy, uint8_t* upstreambsy, uint8_t* arbq4valid, uint8_t* arbq3valid, uint8_t* arbq2valid, uint8_t* arbq1valid, uint8_t* arbq0valid, uint8_t* requeststate, uint8_t* ch3stopped, uint8_t* ch2stopped, uint8_t* ch1stopped, uint8_t* ch0stopped, uint16_t* oftfree)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);

    *ch4stopped = (localVal & ((uint32_t)0x80000000)) >> 31;
    *midstreambsy = (localVal & ((uint32_t)0x40000000)) >> 30;
    *downstreambsy = (localVal & ((uint32_t)0x20000000)) >> 29;
    *upstreambsy = (localVal & ((uint32_t)0x10000000)) >> 28;
    *arbq4valid = (localVal & ((uint32_t)0x08000000)) >> 27;
    *arbq3valid = (localVal & ((uint32_t)0x04000000)) >> 26;
    *arbq2valid = (localVal & ((uint32_t)0x02000000)) >> 25;
    *arbq1valid = (localVal & ((uint32_t)0x01000000)) >> 24;
    *arbq0valid = (localVal & ((uint32_t)0x00800000)) >> 23;
    *requeststate = (localVal & ((uint32_t)0x00700000)) >> 20;
    *ch3stopped = (localVal & ((uint32_t)0x00080000)) >> 19;
    *ch2stopped = (localVal & ((uint32_t)0x00040000)) >> 18;
    *ch1stopped = (localVal & ((uint32_t)0x00020000)) >> 17;
    *ch0stopped = (localVal & ((uint32_t)0x00010000)) >> 16;
    *oftfree = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_STOPPED field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the CH4_STOPPED field's value will be returned.
 *
 * @return The current value of the CH4_STOPPED field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_ch4_stopped_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Returns the current value of the MID_STREAM_BSY field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the MID_STREAM_BSY field's value will be returned.
 *
 * @return The current value of the MID_STREAM_BSY field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_mid_stream_bsy_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

/**
 * @brief Returns the current value of the DOWN_STREAM_BSY field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the DOWN_STREAM_BSY field's value will be returned.
 *
 * @return The current value of the DOWN_STREAM_BSY field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_down_stream_bsy_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Returns the current value of the UP_STREAM_BSY field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the UP_STREAM_BSY field's value will be returned.
 *
 * @return The current value of the UP_STREAM_BSY field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_up_stream_bsy_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Returns the current value of the ARB_Q4_VALID field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the ARB_Q4_VALID field's value will be returned.
 *
 * @return The current value of the ARB_Q4_VALID field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_arb_q4_valid_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

/**
 * @brief Returns the current value of the ARB_Q3_VALID field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the ARB_Q3_VALID field's value will be returned.
 *
 * @return The current value of the ARB_Q3_VALID field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_arb_q3_valid_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x04000000)) >> 26);
}

/**
 * @brief Returns the current value of the ARB_Q2_VALID field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the ARB_Q2_VALID field's value will be returned.
 *
 * @return The current value of the ARB_Q2_VALID field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_arb_q2_valid_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

/**
 * @brief Returns the current value of the ARB_Q1_VALID field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the ARB_Q1_VALID field's value will be returned.
 *
 * @return The current value of the ARB_Q1_VALID field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_arb_q1_valid_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Returns the current value of the ARB_Q0_VALID field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the ARB_Q0_VALID field's value will be returned.
 *
 * @return The current value of the ARB_Q0_VALID field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_arb_q0_valid_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Returns the current value of the REQUEST_STATE field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the REQUEST_STATE field's value will be returned.
 *
 * @return The current value of the REQUEST_STATE field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_request_state_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Returns the current value of the CH3_STOPPED field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the CH3_STOPPED field's value will be returned.
 *
 * @return The current value of the CH3_STOPPED field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_ch3_stopped_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

/**
 * @brief Returns the current value of the CH2_STOPPED field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the CH2_STOPPED field's value will be returned.
 *
 * @return The current value of the CH2_STOPPED field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_ch2_stopped_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

/**
 * @brief Returns the current value of the CH1_STOPPED field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the CH1_STOPPED field's value will be returned.
 *
 * @return The current value of the CH1_STOPPED field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_ch1_stopped_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00020000)) >> 17);
}

/**
 * @brief Returns the current value of the CH0_STOPPED field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the CH0_STOPPED field's value will be returned.
 *
 * @return The current value of the CH0_STOPPED field in the DMA_STATUS register.
 */
__INLINE uint8_t dma_dma_status_ch0_stopped_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the OFT_FREE field in the DMA_STATUS register.
 *
 * The DMA_STATUS register will be read and the OFT_FREE field's value will be returned.
 *
 * @return The current value of the OFT_FREE field in the DMA_STATUS register.
 */
__INLINE uint16_t dma_dma_status_oft_free_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DMA_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/// @}

/**
 * @name INT_RAWSTATUS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     24              CH4_EOT   0
 *     23              CH3_EOT   0
 *     22              CH2_EOT   0
 *     21              CH1_EOT   0
 *     20              CH0_EOT   0
 *     16                ERROR   0
 *  15:00              LLI_IRQ   0x0
 * </pre>
 *
 * @{
 */

/// Address of the INT_RAWSTATUS register
#define DMA_INT_RAWSTATUS_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000014)
/// Offset of the INT_RAWSTATUS register from the base address
#define DMA_INT_RAWSTATUS_OFFSET 0x00000014
/// Index of the INT_RAWSTATUS register
#define DMA_INT_RAWSTATUS_INDEX  0x00000005
/// Reset value of the INT_RAWSTATUS register
#define DMA_INT_RAWSTATUS_RESET  0x00000000

/**
 * @brief Returns the current value of the INT_RAWSTATUS register.
 * The INT_RAWSTATUS register will be read and its value returned.
 * @return The current value of the INT_RAWSTATUS register.
 */
__INLINE uint32_t dma_int_rawstatus_get(void)
{
    return REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
}

// field definitions
#define DMA_CHAN_IRQ_MASK  ((uint32_t)0x01F00000)
/// CH4_EOT field bit
#define DMA_CH4_EOT_BIT    ((uint32_t)0x01000000)
/// CH4_EOT field position
#define DMA_CH4_EOT_POS    24
/// CH3_EOT field bit
#define DMA_CH3_EOT_BIT    ((uint32_t)0x00800000)
/// CH3_EOT field position
#define DMA_CH3_EOT_POS    23
/// CH2_EOT field bit
#define DMA_CH2_EOT_BIT    ((uint32_t)0x00400000)
/// CH2_EOT field position
#define DMA_CH2_EOT_POS    22
/// CH1_EOT field bit
#define DMA_CH1_EOT_BIT    ((uint32_t)0x00200000)
/// CH1_EOT field position
#define DMA_CH1_EOT_POS    21
/// CH0_EOT field bit
#define DMA_CH0_EOT_BIT    ((uint32_t)0x00100000)
/// CH0_EOT field position
#define DMA_CH0_EOT_POS    20
/// ERROR field bit
#define DMA_ERROR_BIT      ((uint32_t)0x00010000)
/// ERROR field position
#define DMA_ERROR_POS      16
/// LLI_IRQ field mask
#define DMA_LLI_IRQ_MASK   ((uint32_t)0x0000FFFF)
/// LLI_IRQ field LSB position
#define DMA_LLI_IRQ_LSB    0
/// LLI_IRQ field width
#define DMA_LLI_IRQ_WIDTH  ((uint32_t)0x00000010)

/// CH4_EOT field reset value
#define DMA_CH4_EOT_RST    0x0
/// CH3_EOT field reset value
#define DMA_CH3_EOT_RST    0x0
/// CH2_EOT field reset value
#define DMA_CH2_EOT_RST    0x0
/// CH1_EOT field reset value
#define DMA_CH1_EOT_RST    0x0
/// CH0_EOT field reset value
#define DMA_CH0_EOT_RST    0x0
/// ERROR field reset value
#define DMA_ERROR_RST      0x0
/// LLI_IRQ field reset value
#define DMA_LLI_IRQ_RST    0x0

/**
 * @brief Unpacks INT_RAWSTATUS's fields from current value of the INT_RAWSTATUS register.
 *
 * Reads the INT_RAWSTATUS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4eot - Will be populated with the current value of this field from the register.
 * @param[out] ch3eot - Will be populated with the current value of this field from the register.
 * @param[out] ch2eot - Will be populated with the current value of this field from the register.
 * @param[out] ch1eot - Will be populated with the current value of this field from the register.
 * @param[out] ch0eot - Will be populated with the current value of this field from the register.
 * @param[out] error - Will be populated with the current value of this field from the register.
 * @param[out] lliirq - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_int_rawstatus_unpack(uint8_t* ch4eot, uint8_t* ch3eot, uint8_t* ch2eot, uint8_t* ch1eot, uint8_t* ch0eot, uint8_t* error, uint16_t* lliirq)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);

    *ch4eot = (localVal & ((uint32_t)0x01000000)) >> 24;
    *ch3eot = (localVal & ((uint32_t)0x00800000)) >> 23;
    *ch2eot = (localVal & ((uint32_t)0x00400000)) >> 22;
    *ch1eot = (localVal & ((uint32_t)0x00200000)) >> 21;
    *ch0eot = (localVal & ((uint32_t)0x00100000)) >> 20;
    *error = (localVal & ((uint32_t)0x00010000)) >> 16;
    *lliirq = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_EOT field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the CH4_EOT field's value will be returned.
 *
 * @return The current value of the CH4_EOT field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_ch4_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Returns the current value of the CH3_EOT field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the CH3_EOT field's value will be returned.
 *
 * @return The current value of the CH3_EOT field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_ch3_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Returns the current value of the CH2_EOT field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the CH2_EOT field's value will be returned.
 *
 * @return The current value of the CH2_EOT field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_ch2_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Returns the current value of the CH1_EOT field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the CH1_EOT field's value will be returned.
 *
 * @return The current value of the CH1_EOT field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_ch1_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Returns the current value of the CH0_EOT field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the CH0_EOT field's value will be returned.
 *
 * @return The current value of the CH0_EOT field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_ch0_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Returns the current value of the ERROR field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the ERROR field's value will be returned.
 *
 * @return The current value of the ERROR field in the INT_RAWSTATUS register.
 */
__INLINE uint8_t dma_int_rawstatus_error_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the LLI_IRQ field in the INT_RAWSTATUS register.
 *
 * The INT_RAWSTATUS register will be read and the LLI_IRQ field's value will be returned.
 *
 * @return The current value of the LLI_IRQ field in the INT_RAWSTATUS register.
 */
__INLINE uint16_t dma_int_rawstatus_lli_irq_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_RAWSTATUS_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/// @}

/**
 * @name INT_UNMASK_SET register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     24              CH4_EOT   0
 *     23              CH3_EOT   0
 *     22              CH2_EOT   0
 *     21              CH1_EOT   0
 *     20              CH0_EOT   0
 *     16                ERROR   0
 *  15:00              LLI_IRQ   0x0
 * </pre>
 *
 * @{
 */

/// Address of the INT_UNMASK_SET register
#define DMA_INT_UNMASK_SET_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000018)
/// Offset of the INT_UNMASK_SET register from the base address
#define DMA_INT_UNMASK_SET_OFFSET 0x00000018
/// Index of the INT_UNMASK_SET register
#define DMA_INT_UNMASK_SET_INDEX  0x00000006
/// Reset value of the INT_UNMASK_SET register
#define DMA_INT_UNMASK_SET_RESET  0x00000000

/**
 * @brief Returns the current value of the INT_UNMASK_SET register.
 * The INT_UNMASK_SET register will be read and its value returned.
 * @return The current value of the INT_UNMASK_SET register.
 */
__INLINE uint32_t dma_int_unmask_get(void)
{
    return REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
}

/**
 * @brief Sets the INT_UNMASK_SET register to a value.
 * The INT_UNMASK_SET register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_int_unmask_set(uint32_t value)
{
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, value);
}

// field definitions
/// CH4_EOT field bit
#define DMA_CH4_EOT_BIT    ((uint32_t)0x01000000)
/// CH4_EOT field position
#define DMA_CH4_EOT_POS    24
/// CH3_EOT field bit
#define DMA_CH3_EOT_BIT    ((uint32_t)0x00800000)
/// CH3_EOT field position
#define DMA_CH3_EOT_POS    23
/// CH2_EOT field bit
#define DMA_CH2_EOT_BIT    ((uint32_t)0x00400000)
/// CH2_EOT field position
#define DMA_CH2_EOT_POS    22
/// CH1_EOT field bit
#define DMA_CH1_EOT_BIT    ((uint32_t)0x00200000)
/// CH1_EOT field position
#define DMA_CH1_EOT_POS    21
/// CH0_EOT field bit
#define DMA_CH0_EOT_BIT    ((uint32_t)0x00100000)
/// CH0_EOT field position
#define DMA_CH0_EOT_POS    20
/// ERROR field bit
#define DMA_ERROR_BIT      ((uint32_t)0x00010000)
/// ERROR field position
#define DMA_ERROR_POS      16
/// LLI_IRQ field mask
#define DMA_LLI_IRQ_MASK   ((uint32_t)0x0000FFFF)
/// LLI_IRQ field LSB position
#define DMA_LLI_IRQ_LSB    0
/// LLI_IRQ field width
#define DMA_LLI_IRQ_WIDTH  ((uint32_t)0x00000010)

/// CH4_EOT field reset value
#define DMA_CH4_EOT_RST    0x0
/// CH3_EOT field reset value
#define DMA_CH3_EOT_RST    0x0
/// CH2_EOT field reset value
#define DMA_CH2_EOT_RST    0x0
/// CH1_EOT field reset value
#define DMA_CH1_EOT_RST    0x0
/// CH0_EOT field reset value
#define DMA_CH0_EOT_RST    0x0
/// ERROR field reset value
#define DMA_ERROR_RST      0x0
/// LLI_IRQ field reset value
#define DMA_LLI_IRQ_RST    0x0

/**
 * @brief Constructs a value for the INT_UNMASK_SET register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ch4eot - The value to use for the CH4_EOT field.
 * @param[in] ch3eot - The value to use for the CH3_EOT field.
 * @param[in] ch2eot - The value to use for the CH2_EOT field.
 * @param[in] ch1eot - The value to use for the CH1_EOT field.
 * @param[in] ch0eot - The value to use for the CH0_EOT field.
 * @param[in] error - The value to use for the ERROR field.
 * @param[in] lliirq - The value to use for the LLI_IRQ field.
 */
__INLINE void dma_int_unmask_set_pack(uint8_t ch4eot, uint8_t ch3eot, uint8_t ch2eot, uint8_t ch1eot, uint8_t ch0eot, uint8_t error, uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR,  ((uint32_t)ch4eot << 24) | ((uint32_t)ch3eot << 23) | ((uint32_t)ch2eot << 22) | ((uint32_t)ch1eot << 21) | ((uint32_t)ch0eot << 20) | ((uint32_t)error << 16) | ((uint32_t)lliirq << 0));
}

/**
 * @brief Unpacks INT_UNMASK_SET's fields from current value of the INT_UNMASK_SET register.
 *
 * Reads the INT_UNMASK_SET register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4eot - Will be populated with the current value of this field from the register.
 * @param[out] ch3eot - Will be populated with the current value of this field from the register.
 * @param[out] ch2eot - Will be populated with the current value of this field from the register.
 * @param[out] ch1eot - Will be populated with the current value of this field from the register.
 * @param[out] ch0eot - Will be populated with the current value of this field from the register.
 * @param[out] error - Will be populated with the current value of this field from the register.
 * @param[out] lliirq - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_int_unmask_unpack(uint8_t* ch4eot, uint8_t* ch3eot, uint8_t* ch2eot, uint8_t* ch1eot, uint8_t* ch0eot, uint8_t* error, uint16_t* lliirq)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);

    *ch4eot = (localVal & ((uint32_t)0x01000000)) >> 24;
    *ch3eot = (localVal & ((uint32_t)0x00800000)) >> 23;
    *ch2eot = (localVal & ((uint32_t)0x00400000)) >> 22;
    *ch1eot = (localVal & ((uint32_t)0x00200000)) >> 21;
    *ch0eot = (localVal & ((uint32_t)0x00100000)) >> 20;
    *error = (localVal & ((uint32_t)0x00010000)) >> 16;
    *lliirq = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_EOT field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the CH4_EOT field's value will be returned.
 *
 * @return The current value of the CH4_EOT field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_ch4_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the CH4_EOT field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch4_eot_setf(uint8_t ch4eot)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)ch4eot << 24);
}

/**
 * @brief Returns the current value of the CH3_EOT field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the CH3_EOT field's value will be returned.
 *
 * @return The current value of the CH3_EOT field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_ch3_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the CH3_EOT field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch3eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch3_eot_setf(uint8_t ch3eot)
{
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)ch3eot << 23);
}

/**
 * @brief Returns the current value of the CH2_EOT field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the CH2_EOT field's value will be returned.
 *
 * @return The current value of the CH2_EOT field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_ch2_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Sets the CH2_EOT field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch2eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch2_eot_setf(uint8_t ch2eot)
{
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)ch2eot << 22);
}

/**
 * @brief Returns the current value of the CH1_EOT field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the CH1_EOT field's value will be returned.
 *
 * @return The current value of the CH1_EOT field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_ch1_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Sets the CH1_EOT field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch1eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch1_eot_setf(uint8_t ch1eot)
{
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)ch1eot << 21);
}

/**
 * @brief Returns the current value of the CH0_EOT field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the CH0_EOT field's value will be returned.
 *
 * @return The current value of the CH0_EOT field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_ch0_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the CH0_EOT field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch0eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch0_eot_setf(uint8_t ch0eot)
{
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)ch0eot << 20);
}

/**
 * @brief Returns the current value of the ERROR field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the ERROR field's value will be returned.
 *
 * @return The current value of the ERROR field in the INT_UNMASK_SET register.
 */
__INLINE uint8_t dma_int_unmask_error_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the ERROR field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] error - The value to set the field to.
 */
__INLINE void dma_int_unmask_error_setf(uint8_t error)
{
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)error << 16);
}

/**
 * @brief Returns the current value of the LLI_IRQ field in the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read and the LLI_IRQ field's value will be returned.
 *
 * @return The current value of the LLI_IRQ field in the INT_UNMASK_SET register.
 */
__INLINE uint16_t dma_int_unmask_lli_irq_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_UNMASK_SET_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the LLI_IRQ field of the INT_UNMASK_SET register.
 *
 * The INT_UNMASK_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] lliirq - The value to set the field to.
 */
__INLINE void dma_int_unmask_lli_irq_setf(uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_SET_ADDR, (uint32_t)lliirq << 0);
}

/// @}

/**
 * @name INT_UNMASK_CLEAR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     24              CH4_EOT   0
 *     23              CH3_EOT   0
 *     22              CH2_EOT   0
 *     21              CH1_EOT   0
 *     20              CH0_EOT   0
 *     16                ERROR   0
 *  15:00              LLI_IRQ   0x0
 * </pre>
 *
 * @{
 */

/// Address of the INT_UNMASK_CLEAR register
#define DMA_INT_UNMASK_CLEAR_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x0000001C)
/// Offset of the INT_UNMASK_CLEAR register from the base address
#define DMA_INT_UNMASK_CLEAR_OFFSET 0x0000001C
/// Index of the INT_UNMASK_CLEAR register
#define DMA_INT_UNMASK_CLEAR_INDEX  0x00000007
/// Reset value of the INT_UNMASK_CLEAR register
#define DMA_INT_UNMASK_CLEAR_RESET  0x00000000

/**
 * @brief Returns the current value of the INT_UNMASK_CLEAR register.
 * The INT_UNMASK_CLEAR register will be read and its value returned.
 * @return The current value of the INT_UNMASK_CLEAR register.
 */
__INLINE uint32_t dma_int_unmask_clear_get(void)
{
    return REG_PL_RD(DMA_INT_UNMASK_CLEAR_ADDR);
}

/**
 * @brief Sets the INT_UNMASK_CLEAR register to a value.
 * The INT_UNMASK_CLEAR register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_int_unmask_clear(uint32_t value)
{
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, value);
}

// fields defined in symmetrical set/clear register
/**
 * @brief Constructs a value for the INT_UNMASK_CLEAR register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ch4eot - The value to use for the CH4_EOT field.
 * @param[in] ch3eot - The value to use for the CH3_EOT field.
 * @param[in] ch2eot - The value to use for the CH2_EOT field.
 * @param[in] ch1eot - The value to use for the CH1_EOT field.
 * @param[in] ch0eot - The value to use for the CH0_EOT field.
 * @param[in] error - The value to use for the ERROR field.
 * @param[in] lliirq - The value to use for the LLI_IRQ field.
 */
__INLINE void dma_int_unmask_clear_pack(uint8_t ch4eot, uint8_t ch3eot, uint8_t ch2eot, uint8_t ch1eot, uint8_t ch0eot, uint8_t error, uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR,  ((uint32_t)ch4eot << 24) | ((uint32_t)ch3eot << 23) | ((uint32_t)ch2eot << 22) | ((uint32_t)ch1eot << 21) | ((uint32_t)ch0eot << 20) | ((uint32_t)error << 16) | ((uint32_t)lliirq << 0));
}

/**
 * @brief Sets the CH4_EOT field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch4_eot_clearf(uint8_t ch4eot)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)ch4eot << 24);
}

/**
 * @brief Sets the CH3_EOT field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch3eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch3_eot_clearf(uint8_t ch3eot)
{
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)ch3eot << 23);
}

/**
 * @brief Sets the CH2_EOT field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch2eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch2_eot_clearf(uint8_t ch2eot)
{
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)ch2eot << 22);
}

/**
 * @brief Sets the CH1_EOT field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch1eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch1_eot_clearf(uint8_t ch1eot)
{
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)ch1eot << 21);
}

/**
 * @brief Sets the CH0_EOT field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch0eot - The value to set the field to.
 */
__INLINE void dma_int_unmask_ch0_eot_clearf(uint8_t ch0eot)
{
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)ch0eot << 20);
}

/**
 * @brief Sets the ERROR field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] error - The value to set the field to.
 */
__INLINE void dma_int_unmask_error_clearf(uint8_t error)
{
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)error << 16);
}

/**
 * @brief Sets the LLI_IRQ field of the INT_UNMASK_CLEAR register.
 *
 * The INT_UNMASK_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] lliirq - The value to set the field to.
 */
__INLINE void dma_int_unmask_lli_irq_clearf(uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_UNMASK_CLEAR_ADDR, (uint32_t)lliirq << 0);
}

/// @}

/**
 * @name INT_ACK register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     24              CH4_EOT   0
 *     23              CH3_EOT   0
 *     22              CH2_EOT   0
 *     21              CH1_EOT   0
 *     20              CH0_EOT   0
 *     16                ERROR   0
 *  15:00              LLI_IRQ   0x0
 * </pre>
 *
 * @{
 */

/// Address of the INT_ACK register
#define DMA_INT_ACK_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000020)
/// Offset of the INT_ACK register from the base address
#define DMA_INT_ACK_OFFSET 0x00000020
/// Index of the INT_ACK register
#define DMA_INT_ACK_INDEX  0x00000008
/// Reset value of the INT_ACK register
#define DMA_INT_ACK_RESET  0x00000000

/**
 * @brief Returns the current value of the INT_ACK register.
 * The INT_ACK register will be read and its value returned.
 * @return The current value of the INT_ACK register.
 */
__INLINE uint32_t dma_int_ack_get(void)
{
    return REG_PL_RD(DMA_INT_ACK_ADDR);
}

/**
 * @brief Sets the INT_ACK register to a value.
 * The INT_ACK register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_int_ack_clear(uint32_t value)
{
    REG_PL_WR(DMA_INT_ACK_ADDR, value);
}

// field definitions
/// CH4_EOT field bit
#define DMA_CH4_EOT_BIT    ((uint32_t)0x01000000)
/// CH4_EOT field position
#define DMA_CH4_EOT_POS    24
/// CH3_EOT field bit
#define DMA_CH3_EOT_BIT    ((uint32_t)0x00800000)
/// CH3_EOT field position
#define DMA_CH3_EOT_POS    23
/// CH2_EOT field bit
#define DMA_CH2_EOT_BIT    ((uint32_t)0x00400000)
/// CH2_EOT field position
#define DMA_CH2_EOT_POS    22
/// CH1_EOT field bit
#define DMA_CH1_EOT_BIT    ((uint32_t)0x00200000)
/// CH1_EOT field position
#define DMA_CH1_EOT_POS    21
/// CH0_EOT field bit
#define DMA_CH0_EOT_BIT    ((uint32_t)0x00100000)
/// CH0_EOT field position
#define DMA_CH0_EOT_POS    20
/// ERROR field bit
#define DMA_ERROR_BIT      ((uint32_t)0x00010000)
/// ERROR field position
#define DMA_ERROR_POS      16
/// LLI_IRQ field mask
#define DMA_LLI_IRQ_MASK   ((uint32_t)0x0000FFFF)
/// LLI_IRQ field LSB position
#define DMA_LLI_IRQ_LSB    0
/// LLI_IRQ field width
#define DMA_LLI_IRQ_WIDTH  ((uint32_t)0x00000010)

/// CH4_EOT field reset value
#define DMA_CH4_EOT_RST    0x0
/// CH3_EOT field reset value
#define DMA_CH3_EOT_RST    0x0
/// CH2_EOT field reset value
#define DMA_CH2_EOT_RST    0x0
/// CH1_EOT field reset value
#define DMA_CH1_EOT_RST    0x0
/// CH0_EOT field reset value
#define DMA_CH0_EOT_RST    0x0
/// ERROR field reset value
#define DMA_ERROR_RST      0x0
/// LLI_IRQ field reset value
#define DMA_LLI_IRQ_RST    0x0

/**
 * @brief Constructs a value for the INT_ACK register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ch4eot - The value to use for the CH4_EOT field.
 * @param[in] ch3eot - The value to use for the CH3_EOT field.
 * @param[in] ch2eot - The value to use for the CH2_EOT field.
 * @param[in] ch1eot - The value to use for the CH1_EOT field.
 * @param[in] ch0eot - The value to use for the CH0_EOT field.
 * @param[in] error - The value to use for the ERROR field.
 * @param[in] lliirq - The value to use for the LLI_IRQ field.
 */
__INLINE void dma_int_ack_pack(uint8_t ch4eot, uint8_t ch3eot, uint8_t ch2eot, uint8_t ch1eot, uint8_t ch0eot, uint8_t error, uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR,  ((uint32_t)ch4eot << 24) | ((uint32_t)ch3eot << 23) | ((uint32_t)ch2eot << 22) | ((uint32_t)ch1eot << 21) | ((uint32_t)ch0eot << 20) | ((uint32_t)error << 16) | ((uint32_t)lliirq << 0));
}

/**
 * @brief Unpacks INT_ACK's fields from current value of the INT_ACK register.
 *
 * Reads the INT_ACK register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4eot - Will be populated with the current value of this field from the register.
 * @param[out] ch3eot - Will be populated with the current value of this field from the register.
 * @param[out] ch2eot - Will be populated with the current value of this field from the register.
 * @param[out] ch1eot - Will be populated with the current value of this field from the register.
 * @param[out] ch0eot - Will be populated with the current value of this field from the register.
 * @param[out] error - Will be populated with the current value of this field from the register.
 * @param[out] lliirq - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_int_ack_unpack(uint8_t* ch4eot, uint8_t* ch3eot, uint8_t* ch2eot, uint8_t* ch1eot, uint8_t* ch0eot, uint8_t* error, uint16_t* lliirq)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);

    *ch4eot = (localVal & ((uint32_t)0x01000000)) >> 24;
    *ch3eot = (localVal & ((uint32_t)0x00800000)) >> 23;
    *ch2eot = (localVal & ((uint32_t)0x00400000)) >> 22;
    *ch1eot = (localVal & ((uint32_t)0x00200000)) >> 21;
    *ch0eot = (localVal & ((uint32_t)0x00100000)) >> 20;
    *error = (localVal & ((uint32_t)0x00010000)) >> 16;
    *lliirq = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_EOT field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the CH4_EOT field's value will be returned.
 *
 * @return The current value of the CH4_EOT field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_ch4_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the CH4_EOT field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4eot - The value to set the field to.
 */
__INLINE void dma_int_ack_ch4_eot_clearf(uint8_t ch4eot)
{
    ASSERT_ERR((((uint32_t)ch4eot << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)ch4eot << 24);
}

/**
 * @brief Returns the current value of the CH3_EOT field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the CH3_EOT field's value will be returned.
 *
 * @return The current value of the CH3_EOT field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_ch3_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the CH3_EOT field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch3eot - The value to set the field to.
 */
__INLINE void dma_int_ack_ch3_eot_clearf(uint8_t ch3eot)
{
    ASSERT_ERR((((uint32_t)ch3eot << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)ch3eot << 23);
}

/**
 * @brief Returns the current value of the CH2_EOT field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the CH2_EOT field's value will be returned.
 *
 * @return The current value of the CH2_EOT field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_ch2_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Sets the CH2_EOT field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch2eot - The value to set the field to.
 */
__INLINE void dma_int_ack_ch2_eot_clearf(uint8_t ch2eot)
{
    ASSERT_ERR((((uint32_t)ch2eot << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)ch2eot << 22);
}

/**
 * @brief Returns the current value of the CH1_EOT field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the CH1_EOT field's value will be returned.
 *
 * @return The current value of the CH1_EOT field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_ch1_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Sets the CH1_EOT field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch1eot - The value to set the field to.
 */
__INLINE void dma_int_ack_ch1_eot_clearf(uint8_t ch1eot)
{
    ASSERT_ERR((((uint32_t)ch1eot << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)ch1eot << 21);
}

/**
 * @brief Returns the current value of the CH0_EOT field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the CH0_EOT field's value will be returned.
 *
 * @return The current value of the CH0_EOT field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_ch0_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the CH0_EOT field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch0eot - The value to set the field to.
 */
__INLINE void dma_int_ack_ch0_eot_clearf(uint8_t ch0eot)
{
    ASSERT_ERR((((uint32_t)ch0eot << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)ch0eot << 20);
}

/**
 * @brief Returns the current value of the ERROR field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the ERROR field's value will be returned.
 *
 * @return The current value of the ERROR field in the INT_ACK register.
 */
__INLINE uint8_t dma_int_ack_error_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the ERROR field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] error - The value to set the field to.
 */
__INLINE void dma_int_ack_error_clearf(uint8_t error)
{
    ASSERT_ERR((((uint32_t)error << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)error << 16);
}

/**
 * @brief Returns the current value of the LLI_IRQ field in the INT_ACK register.
 *
 * The INT_ACK register will be read and the LLI_IRQ field's value will be returned.
 *
 * @return The current value of the LLI_IRQ field in the INT_ACK register.
 */
__INLINE uint16_t dma_int_ack_lli_irq_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_ACK_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the LLI_IRQ field of the INT_ACK register.
 *
 * The INT_ACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] lliirq - The value to set the field to.
 */
__INLINE void dma_int_ack_lli_irq_clearf(uint16_t lliirq)
{
    ASSERT_ERR((((uint32_t)lliirq << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(DMA_INT_ACK_ADDR, (uint32_t)lliirq << 0);
}

/// @}

/**
 * @name INT_STATUS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     24              CH4_EOT   0
 *     23              CH3_EOT   0
 *     22              CH2_EOT   0
 *     21              CH1_EOT   0
 *     20              CH0_EOT   0
 *     16                ERROR   0
 *  15:00              LLI_IRQ   0x0
 * </pre>
 *
 * @{
 */

/// Address of the INT_STATUS register
#define DMA_INT_STATUS_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000024)
/// Offset of the INT_STATUS register from the base address
#define DMA_INT_STATUS_OFFSET 0x00000024
/// Index of the INT_STATUS register
#define DMA_INT_STATUS_INDEX  0x00000009
/// Reset value of the INT_STATUS register
#define DMA_INT_STATUS_RESET  0x00000000

/**
 * @brief Returns the current value of the INT_STATUS register.
 * The INT_STATUS register will be read and its value returned.
 * @return The current value of the INT_STATUS register.
 */
__INLINE uint32_t dma_int_status_get(void)
{
    return REG_PL_RD(DMA_INT_STATUS_ADDR);
}

// field definitions
/// CH4_EOT field bit
#define DMA_CH4_EOT_BIT    ((uint32_t)0x01000000)
/// CH4_EOT field position
#define DMA_CH4_EOT_POS    24
/// CH3_EOT field bit
#define DMA_CH3_EOT_BIT    ((uint32_t)0x00800000)
/// CH3_EOT field position
#define DMA_CH3_EOT_POS    23
/// CH2_EOT field bit
#define DMA_CH2_EOT_BIT    ((uint32_t)0x00400000)
/// CH2_EOT field position
#define DMA_CH2_EOT_POS    22
/// CH1_EOT field bit
#define DMA_CH1_EOT_BIT    ((uint32_t)0x00200000)
/// CH1_EOT field position
#define DMA_CH1_EOT_POS    21
/// CH0_EOT field bit
#define DMA_CH0_EOT_BIT    ((uint32_t)0x00100000)
/// CH0_EOT field position
#define DMA_CH0_EOT_POS    20
/// ERROR field bit
#define DMA_ERROR_BIT      ((uint32_t)0x00010000)
/// ERROR field position
#define DMA_ERROR_POS      16
/// LLI_IRQ field mask
#define DMA_LLI_IRQ_MASK   ((uint32_t)0x0000FFFF)
/// LLI_IRQ field LSB position
#define DMA_LLI_IRQ_LSB    0
/// LLI_IRQ field width
#define DMA_LLI_IRQ_WIDTH  ((uint32_t)0x00000010)

/// CH4_EOT field reset value
#define DMA_CH4_EOT_RST    0x0
/// CH3_EOT field reset value
#define DMA_CH3_EOT_RST    0x0
/// CH2_EOT field reset value
#define DMA_CH2_EOT_RST    0x0
/// CH1_EOT field reset value
#define DMA_CH1_EOT_RST    0x0
/// CH0_EOT field reset value
#define DMA_CH0_EOT_RST    0x0
/// ERROR field reset value
#define DMA_ERROR_RST      0x0
/// LLI_IRQ field reset value
#define DMA_LLI_IRQ_RST    0x0

/**
 * @brief Unpacks INT_STATUS's fields from current value of the INT_STATUS register.
 *
 * Reads the INT_STATUS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4eot - Will be populated with the current value of this field from the register.
 * @param[out] ch3eot - Will be populated with the current value of this field from the register.
 * @param[out] ch2eot - Will be populated with the current value of this field from the register.
 * @param[out] ch1eot - Will be populated with the current value of this field from the register.
 * @param[out] ch0eot - Will be populated with the current value of this field from the register.
 * @param[out] error - Will be populated with the current value of this field from the register.
 * @param[out] lliirq - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_int_status_unpack(uint8_t* ch4eot, uint8_t* ch3eot, uint8_t* ch2eot, uint8_t* ch1eot, uint8_t* ch0eot, uint8_t* error, uint16_t* lliirq)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);

    *ch4eot = (localVal & ((uint32_t)0x01000000)) >> 24;
    *ch3eot = (localVal & ((uint32_t)0x00800000)) >> 23;
    *ch2eot = (localVal & ((uint32_t)0x00400000)) >> 22;
    *ch1eot = (localVal & ((uint32_t)0x00200000)) >> 21;
    *ch0eot = (localVal & ((uint32_t)0x00100000)) >> 20;
    *error = (localVal & ((uint32_t)0x00010000)) >> 16;
    *lliirq = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_EOT field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the CH4_EOT field's value will be returned.
 *
 * @return The current value of the CH4_EOT field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_ch4_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Returns the current value of the CH3_EOT field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the CH3_EOT field's value will be returned.
 *
 * @return The current value of the CH3_EOT field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_ch3_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Returns the current value of the CH2_EOT field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the CH2_EOT field's value will be returned.
 *
 * @return The current value of the CH2_EOT field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_ch2_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Returns the current value of the CH1_EOT field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the CH1_EOT field's value will be returned.
 *
 * @return The current value of the CH1_EOT field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_ch1_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Returns the current value of the CH0_EOT field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the CH0_EOT field's value will be returned.
 *
 * @return The current value of the CH0_EOT field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_ch0_eot_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Returns the current value of the ERROR field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the ERROR field's value will be returned.
 *
 * @return The current value of the ERROR field in the INT_STATUS register.
 */
__INLINE uint8_t dma_int_status_error_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the LLI_IRQ field in the INT_STATUS register.
 *
 * The INT_STATUS register will be read and the LLI_IRQ field's value will be returned.
 *
 * @return The current value of the LLI_IRQ field in the INT_STATUS register.
 */
__INLINE uint16_t dma_int_status_lli_irq_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_INT_STATUS_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/// @}

/**
 * @name ARBITRATION register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  03:00   DOWNSTREAM_TAG_USAGE   0xC
 * </pre>
 *
 * @{
 */

/// Address of the ARBITRATION register
#define DMA_ARBITRATION_ADDR   (WIFI_PDMA_REG_BASE_ADDR+0x0034)
/// Offset of the ARBITRATION register from the base address
#define DMA_ARBITRATION_OFFSET 0x00000034
/// Index of the ARBITRATION register
#define DMA_ARBITRATION_INDEX  0x0000000D
/// Reset value of the ARBITRATION register
#define DMA_ARBITRATION_RESET  0x0000000C

/**
 * @brief Returns the current value of the ARBITRATION register.
 * The ARBITRATION register will be read and its value returned.
 * @return The current value of the ARBITRATION register.
 */
__INLINE uint32_t dma_arbitration_get(void)
{
    return REG_PL_RD(DMA_ARBITRATION_ADDR);
}

/**
 * @brief Sets the ARBITRATION register to a value.
 * The ARBITRATION register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_arbitration_set(uint32_t value)
{
    REG_PL_WR(DMA_ARBITRATION_ADDR, value);
}

// field definitions
/// DOWNSTREAM_TAG_USAGE field mask
#define DMA_DOWNSTREAM_TAG_USAGE_MASK   ((uint32_t)0x0000000F)
/// DOWNSTREAM_TAG_USAGE field LSB position
#define DMA_DOWNSTREAM_TAG_USAGE_LSB    0
/// DOWNSTREAM_TAG_USAGE field width
#define DMA_DOWNSTREAM_TAG_USAGE_WIDTH  ((uint32_t)0x00000004)

/// DOWNSTREAM_TAG_USAGE field reset value
#define DMA_DOWNSTREAM_TAG_USAGE_RST    0xC

/**
 * @brief Returns the current value of the DOWNSTREAM_TAG_USAGE field in the ARBITRATION register.
 *
 * The ARBITRATION register will be read and the DOWNSTREAM_TAG_USAGE field's value will be returned.
 *
 * @return The current value of the DOWNSTREAM_TAG_USAGE field in the ARBITRATION register.
 */
__INLINE uint8_t dma_arbitration_downstream_tag_usage_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_ARBITRATION_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000000F)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the DOWNSTREAM_TAG_USAGE field of the ARBITRATION register.
 *
 * The ARBITRATION register will be read, modified to contain the new field value, and written.
 *
 * @param[in] downstreamtagusage - The value to set the field to.
 */
__INLINE void dma_arbitration_downstream_tag_usage_setf(uint8_t downstreamtagusage)
{
    ASSERT_ERR((((uint32_t)downstreamtagusage << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(DMA_ARBITRATION_ADDR, (uint32_t)downstreamtagusage << 0);
}

/// @}

/**
 * @name CHANNEL_MUTEX_SET register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     04            CH4_MUTEX   0
 *     03            CH3_MUTEX   0
 *     02            CH2_MUTEX   0
 *     01            CH1_MUTEX   0
 *     00            CH0_MUTEX   0
 * </pre>
 *
 * @{
 */

/// Address of the CHANNEL_MUTEX_SET register
#define DMA_CHANNEL_MUTEX_SET_ADDR   (WIFI_PDMA_REG_BASE_ADDR+0x0038)
/// Offset of the CHANNEL_MUTEX_SET register from the base address
#define DMA_CHANNEL_MUTEX_SET_OFFSET 0x00000038
/// Index of the CHANNEL_MUTEX_SET register
#define DMA_CHANNEL_MUTEX_SET_INDEX  0x0000000E
/// Reset value of the CHANNEL_MUTEX_SET register
#define DMA_CHANNEL_MUTEX_SET_RESET  0x00000000

/**
 * @brief Returns the current value of the CHANNEL_MUTEX_SET register.
 * The CHANNEL_MUTEX_SET register will be read and its value returned.
 * @return The current value of the CHANNEL_MUTEX_SET register.
 */
__INLINE uint32_t dma_channel_mutex_get(void)
{
    return REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
}

/**
 * @brief Sets the CHANNEL_MUTEX_SET register to a value.
 * The CHANNEL_MUTEX_SET register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_channel_mutex_set(uint32_t value)
{
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, value);
}

// field definitions
/// CH4_MUTEX field bit
#define DMA_CH4_MUTEX_BIT    ((uint32_t)0x00000010)
/// CH4_MUTEX field position
#define DMA_CH4_MUTEX_POS    4
/// CH3_MUTEX field bit
#define DMA_CH3_MUTEX_BIT    ((uint32_t)0x00000008)
/// CH3_MUTEX field position
#define DMA_CH3_MUTEX_POS    3
/// CH2_MUTEX field bit
#define DMA_CH2_MUTEX_BIT    ((uint32_t)0x00000004)
/// CH2_MUTEX field position
#define DMA_CH2_MUTEX_POS    2
/// CH1_MUTEX field bit
#define DMA_CH1_MUTEX_BIT    ((uint32_t)0x00000002)
/// CH1_MUTEX field position
#define DMA_CH1_MUTEX_POS    1
/// CH0_MUTEX field bit
#define DMA_CH0_MUTEX_BIT    ((uint32_t)0x00000001)
/// CH0_MUTEX field position
#define DMA_CH0_MUTEX_POS    0

/// CH4_MUTEX field reset value
#define DMA_CH4_MUTEX_RST    0x0
/// CH3_MUTEX field reset value
#define DMA_CH3_MUTEX_RST    0x0
/// CH2_MUTEX field reset value
#define DMA_CH2_MUTEX_RST    0x0
/// CH1_MUTEX field reset value
#define DMA_CH1_MUTEX_RST    0x0
/// CH0_MUTEX field reset value
#define DMA_CH0_MUTEX_RST    0x0

/**
 * @brief Constructs a value for the CHANNEL_MUTEX_SET register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ch4mutex - The value to use for the CH4_MUTEX field.
 * @param[in] ch3mutex - The value to use for the CH3_MUTEX field.
 * @param[in] ch2mutex - The value to use for the CH2_MUTEX field.
 * @param[in] ch1mutex - The value to use for the CH1_MUTEX field.
 * @param[in] ch0mutex - The value to use for the CH0_MUTEX field.
 */
__INLINE void dma_channel_mutex_set_pack(uint8_t ch4mutex, uint8_t ch3mutex, uint8_t ch2mutex, uint8_t ch1mutex, uint8_t ch0mutex)
{
    ASSERT_ERR((((uint32_t)ch4mutex << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)ch3mutex << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)ch2mutex << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)ch1mutex << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)ch0mutex << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR,  ((uint32_t)ch4mutex << 4) | ((uint32_t)ch3mutex << 3) | ((uint32_t)ch2mutex << 2) | ((uint32_t)ch1mutex << 1) | ((uint32_t)ch0mutex << 0));
}

/**
 * @brief Unpacks CHANNEL_MUTEX_SET's fields from current value of the CHANNEL_MUTEX_SET register.
 *
 * Reads the CHANNEL_MUTEX_SET register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ch4mutex - Will be populated with the current value of this field from the register.
 * @param[out] ch3mutex - Will be populated with the current value of this field from the register.
 * @param[out] ch2mutex - Will be populated with the current value of this field from the register.
 * @param[out] ch1mutex - Will be populated with the current value of this field from the register.
 * @param[out] ch0mutex - Will be populated with the current value of this field from the register.
 */
__INLINE void dma_channel_mutex_unpack(uint8_t* ch4mutex, uint8_t* ch3mutex, uint8_t* ch2mutex, uint8_t* ch1mutex, uint8_t* ch0mutex)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);

    *ch4mutex = (localVal & ((uint32_t)0x00000010)) >> 4;
    *ch3mutex = (localVal & ((uint32_t)0x00000008)) >> 3;
    *ch2mutex = (localVal & ((uint32_t)0x00000004)) >> 2;
    *ch1mutex = (localVal & ((uint32_t)0x00000002)) >> 1;
    *ch0mutex = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the CH4_MUTEX field in the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read and the CH4_MUTEX field's value will be returned.
 *
 * @return The current value of the CH4_MUTEX field in the CHANNEL_MUTEX_SET register.
 */
__INLINE uint8_t dma_channel_mutex_ch4_mutex_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the CH4_MUTEX field of the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch4_mutex_setf(uint8_t ch4mutex)
{
    ASSERT_ERR((((uint32_t)ch4mutex << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, (uint32_t)ch4mutex << 4);
}

/**
 * @brief Returns the current value of the CH3_MUTEX field in the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read and the CH3_MUTEX field's value will be returned.
 *
 * @return The current value of the CH3_MUTEX field in the CHANNEL_MUTEX_SET register.
 */
__INLINE uint8_t dma_channel_mutex_ch3_mutex_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the CH3_MUTEX field of the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch3mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch3_mutex_setf(uint8_t ch3mutex)
{
    ASSERT_ERR((((uint32_t)ch3mutex << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, (uint32_t)ch3mutex << 3);
}

/**
 * @brief Returns the current value of the CH2_MUTEX field in the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read and the CH2_MUTEX field's value will be returned.
 *
 * @return The current value of the CH2_MUTEX field in the CHANNEL_MUTEX_SET register.
 */
__INLINE uint8_t dma_channel_mutex_ch2_mutex_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the CH2_MUTEX field of the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch2mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch2_mutex_setf(uint8_t ch2mutex)
{
    ASSERT_ERR((((uint32_t)ch2mutex << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, (uint32_t)ch2mutex << 2);
}

/**
 * @brief Returns the current value of the CH1_MUTEX field in the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read and the CH1_MUTEX field's value will be returned.
 *
 * @return The current value of the CH1_MUTEX field in the CHANNEL_MUTEX_SET register.
 */
__INLINE uint8_t dma_channel_mutex_ch1_mutex_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the CH1_MUTEX field of the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch1mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch1_mutex_setf(uint8_t ch1mutex)
{
    ASSERT_ERR((((uint32_t)ch1mutex << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, (uint32_t)ch1mutex << 1);
}

/**
 * @brief Returns the current value of the CH0_MUTEX field in the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read and the CH0_MUTEX field's value will be returned.
 *
 * @return The current value of the CH0_MUTEX field in the CHANNEL_MUTEX_SET register.
 */
__INLINE uint8_t dma_channel_mutex_ch0_mutex_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CHANNEL_MUTEX_SET_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the CH0_MUTEX field of the CHANNEL_MUTEX_SET register.
 *
 * The CHANNEL_MUTEX_SET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch0mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch0_mutex_setf(uint8_t ch0mutex)
{
    ASSERT_ERR((((uint32_t)ch0mutex << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_SET_ADDR, (uint32_t)ch0mutex << 0);
}

/// @}

/**
 * @name CHANNEL_MUTEX_CLEAR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     04            CH4_MUTEX   0
 *     03            CH3_MUTEX   0
 *     02            CH2_MUTEX   0
 *     01            CH1_MUTEX   0
 *     00            CH0_MUTEX   0
 * </pre>
 *
 * @{
 */

/// Address of the CHANNEL_MUTEX_CLEAR register
#define DMA_CHANNEL_MUTEX_CLEAR_ADDR   (WIFI_PDMA_REG_BASE_ADDR+0x003C)
/// Offset of the CHANNEL_MUTEX_CLEAR register from the base address
#define DMA_CHANNEL_MUTEX_CLEAR_OFFSET 0x0000003C
/// Index of the CHANNEL_MUTEX_CLEAR register
#define DMA_CHANNEL_MUTEX_CLEAR_INDEX  0x0000000F
/// Reset value of the CHANNEL_MUTEX_CLEAR register
#define DMA_CHANNEL_MUTEX_CLEAR_RESET  0x00000000

/**
 * @brief Sets the CHANNEL_MUTEX_CLEAR register to a value.
 * The CHANNEL_MUTEX_CLEAR register will be written.
 * @param value - The value to write.
 */
__INLINE uint32_t dma_channel_mutex_clear_get(void)
{
    return REG_PL_RD(DMA_CHANNEL_MUTEX_CLEAR_ADDR);
}

/**
 * @brief Sets the CHANNEL_MUTEX_CLEAR register to a value.
 * The CHANNEL_MUTEX_CLEAR register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_channel_mutex_clear(uint32_t value)
{
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, value);
}

// fields defined in symmetrical set/clear register
/**
 * @brief Constructs a value for the CHANNEL_MUTEX_CLEAR register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ch4mutex - The value to use for the CH4_MUTEX field.
 * @param[in] ch3mutex - The value to use for the CH3_MUTEX field.
 * @param[in] ch2mutex - The value to use for the CH2_MUTEX field.
 * @param[in] ch1mutex - The value to use for the CH1_MUTEX field.
 * @param[in] ch0mutex - The value to use for the CH0_MUTEX field.
 */
__INLINE void dma_channel_mutex_clear_pack(uint8_t ch4mutex, uint8_t ch3mutex, uint8_t ch2mutex, uint8_t ch1mutex, uint8_t ch0mutex)
{
    ASSERT_ERR((((uint32_t)ch4mutex << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)ch3mutex << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)ch2mutex << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)ch1mutex << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)ch0mutex << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR,  ((uint32_t)ch4mutex << 4) | ((uint32_t)ch3mutex << 3) | ((uint32_t)ch2mutex << 2) | ((uint32_t)ch1mutex << 1) | ((uint32_t)ch0mutex << 0));
}

/**
 * @brief Sets the CH4_MUTEX field of the CHANNEL_MUTEX_CLEAR register.
 *
 * The CHANNEL_MUTEX_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch4_mutex_clearf(uint8_t ch4mutex)
{
    ASSERT_ERR((((uint32_t)ch4mutex << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, (uint32_t)ch4mutex << 4);
}

/**
 * @brief Sets the CH3_MUTEX field of the CHANNEL_MUTEX_CLEAR register.
 *
 * The CHANNEL_MUTEX_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch3mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch3_mutex_clearf(uint8_t ch3mutex)
{
    ASSERT_ERR((((uint32_t)ch3mutex << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, (uint32_t)ch3mutex << 3);
}

/**
 * @brief Sets the CH2_MUTEX field of the CHANNEL_MUTEX_CLEAR register.
 *
 * The CHANNEL_MUTEX_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch2mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch2_mutex_clearf(uint8_t ch2mutex)
{
    ASSERT_ERR((((uint32_t)ch2mutex << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, (uint32_t)ch2mutex << 2);
}

/**
 * @brief Sets the CH1_MUTEX field of the CHANNEL_MUTEX_CLEAR register.
 *
 * The CHANNEL_MUTEX_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch1mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch1_mutex_clearf(uint8_t ch1mutex)
{
    ASSERT_ERR((((uint32_t)ch1mutex << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, (uint32_t)ch1mutex << 1);
}

/**
 * @brief Sets the CH0_MUTEX field of the CHANNEL_MUTEX_CLEAR register.
 *
 * The CHANNEL_MUTEX_CLEAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch0mutex - The value to set the field to.
 */
__INLINE void dma_channel_mutex_ch0_mutex_clearf(uint8_t ch0mutex)
{
    ASSERT_ERR((((uint32_t)ch0mutex << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(DMA_CHANNEL_MUTEX_CLEAR_ADDR, (uint32_t)ch0mutex << 0);
}

/// @}

/**
 * @name CH4_LLI_ROOT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00         CH4_LLI_ROOT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the CH4_LLI_ROOT register
#define DMA_CH4_LLI_ROOT_ADDR   (WIFI_PDMA_REG_BASE_ADDR+0x0040)
/// Offset of the CH4_LLI_ROOT register from the base address
#define DMA_CH4_LLI_ROOT_OFFSET 0x00000040
/// Index of the CH4_LLI_ROOT register
#define DMA_CH4_LLI_ROOT_INDEX  0x00000010
/// Reset value of the CH4_LLI_ROOT register
#define DMA_CH4_LLI_ROOT_RESET  0x00000000

/**
 * @brief Returns the current value of the CH4_LLI_ROOT register.
 * The CH4_LLI_ROOT register will be read and its value returned.
 * @return The current value of the CH4_LLI_ROOT register.
 */
__INLINE uint32_t dma_ch4_lli_root_get(void)
{
    return REG_PL_RD(DMA_CH4_LLI_ROOT_ADDR);
}

/**
 * @brief Sets the CH4_LLI_ROOT register to a value.
 * The CH4_LLI_ROOT register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_ch4_lli_root_set(uint32_t value)
{
    REG_PL_WR(DMA_CH4_LLI_ROOT_ADDR, value);
}

// field definitions
/// CH4_LLI_ROOT field mask
#define DMA_CH4_LLI_ROOT_MASK   ((uint32_t)0xFFFFFFFF)
/// CH4_LLI_ROOT field LSB position
#define DMA_CH4_LLI_ROOT_LSB    0
/// CH4_LLI_ROOT field width
#define DMA_CH4_LLI_ROOT_WIDTH  ((uint32_t)0x00000020)

/// CH4_LLI_ROOT field reset value
#define DMA_CH4_LLI_ROOT_RST    0x0

/**
 * @brief Returns the current value of the CH4_LLI_ROOT field in the CH4_LLI_ROOT register.
 *
 * The CH4_LLI_ROOT register will be read and the CH4_LLI_ROOT field's value will be returned.
 *
 * @return The current value of the CH4_LLI_ROOT field in the CH4_LLI_ROOT register.
 */
__INLINE uint32_t dma_ch4_lli_root_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_CH4_LLI_ROOT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the CH4_LLI_ROOT field of the CH4_LLI_ROOT register.
 *
 * The CH4_LLI_ROOT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ch4lliroot - The value to set the field to.
 */
__INLINE void dma_ch4_lli_root_setf(uint32_t ch4lliroot)
{
    ASSERT_ERR((((uint32_t)ch4lliroot << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(DMA_CH4_LLI_ROOT_ADDR, (uint32_t)ch4lliroot << 0);
}

/// @}

/**
 * @name LLI_COUNTER register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00              COUNTER   0x0
 * </pre>
 *
 * @{
 */

/// Address of the LLI_COUNTER register
#define DMA_LLI_COUNTER_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000080)
/// Offset of the LLI_COUNTER register from the base address
#define DMA_LLI_COUNTER_OFFSET 0x00000080
/// Index of the LLI_COUNTER register
#define DMA_LLI_COUNTER_INDEX  0x00000020
/// Reset value of the LLI_COUNTER register
#define DMA_LLI_COUNTER_RESET  0x00000000
/// Number of elements of the LLI_COUNTER register array
#define DMA_LLI_COUNTER_COUNT  16

/**
 * @brief Returns the current value of the LLI_COUNTER register.
 * The LLI_COUNTER register will be read and its value returned.
 * @param[in] reg_idx Index of the register
 * @return The current value of the LLI_COUNTER register.
 */
__INLINE uint32_t dma_lli_counter_get(int reg_idx)
{
    ASSERT_ERR(reg_idx <= 15);
    return REG_PL_RD(DMA_LLI_COUNTER_ADDR + reg_idx * 4);
}

// field definitions
/// COUNTER field mask
#define DMA_COUNTER_MASK   ((uint32_t)0x0000FFFF)
/// COUNTER field LSB position
#define DMA_COUNTER_LSB    0
/// COUNTER field width
#define DMA_COUNTER_WIDTH  ((uint32_t)0x00000010)

/// COUNTER field reset value
#define DMA_COUNTER_RST    0x0

/**
 * @brief Returns the current value of the COUNTER field in the LLI_COUNTER register.
 *
 * The LLI_COUNTER register will be read and the COUNTER field's value will be returned.
 *
 * @param[in] reg_idx Index of the register
 * @return The current value of the COUNTER field in the LLI_COUNTER register.
 */
__INLINE uint16_t dma_lli_counter_counter_getf(int reg_idx)
{
    ASSERT_ERR(reg_idx <= 15);
    uint32_t localVal = REG_PL_RD(DMA_LLI_COUNTER_ADDR + reg_idx * 4);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name DUMMY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00                DUMMY   0
 * </pre>
 *
 * @{
 */

/// Address of the DUMMY register
#define DMA_DUMMY_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x000000C0)
/// Offset of the DUMMY register from the base address
#define DMA_DUMMY_OFFSET 0x000000C0
/// Index of the DUMMY register
#define DMA_DUMMY_INDEX  0x00000030
/// Reset value of the DUMMY register
#define DMA_DUMMY_RESET  0x00000000

/**
 * @brief Returns the current value of the DUMMY register.
 * The DUMMY register will be read and its value returned.
 * @return The current value of the DUMMY register.
 */
__INLINE uint32_t dma_dummy_get(void)
{
    return REG_PL_RD(DMA_DUMMY_ADDR);
}

/**
 * @brief Sets the DUMMY register to a value.
 * The DUMMY register will be written.
 * @param value - The value to write.
 */
__INLINE void dma_dummy_set(uint32_t value)
{
    REG_PL_WR(DMA_DUMMY_ADDR, value);
}

// field definitions
/// DUMMY field bit
#define DMA_DUMMY_BIT    ((uint32_t)0x00000001)
/// DUMMY field position
#define DMA_DUMMY_POS    0

/// DUMMY field reset value
#define DMA_DUMMY_RST    0x0

/**
 * @brief Returns the current value of the DUMMY field in the DUMMY register.
 *
 * The DUMMY register will be read and the DUMMY field's value will be returned.
 *
 * @return The current value of the DUMMY field in the DUMMY register.
 */
__INLINE uint8_t dma_dummy_getf(void)
{
    uint32_t localVal = REG_PL_RD(DMA_DUMMY_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the DUMMY field of the DUMMY register.
 *
 * The DUMMY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dummy - The value to set the field to.
 */
__INLINE void dma_dummy_setf(uint8_t dummy)
{
    ASSERT_ERR((((uint32_t)dummy << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(DMA_DUMMY_ADDR, (uint32_t)dummy << 0);
}

/// @}

/// Address of the DUMMY register
#define DMA_FSM_CTRL_ADDR   (WIFI_PDMA_REG_BASE_ADDR + 0x00000048)
/// Offset of the DUMMY register from the base address
#define DMA_FSM_CTRL_OFFSET 0x00000048
/// Index of the DUMMY register
#define DMA_FSM_CTRL_INDEX  (DMA_FSM_CTRL_OFFSET>>2)
/// Reset value of the DUMMY register
#define DMA_FSM_CTRL_RESET  0x00000004

__INLINE void dma_unordered_mode_support_set(uint32_t enable)
{
    uint32_t localVal = REG_PL_RD(DMA_FSM_CTRL_ADDR);
    REG_PL_WR(DMA_FSM_CTRL_ADDR, (localVal & (~0x2)) | ((enable&0x1)<<1));
}

__INLINE void dma_null_desc_support_set(uint32_t enable)
{
    uint32_t localVal = REG_PL_RD(DMA_FSM_CTRL_ADDR);
    REG_PL_WR(DMA_FSM_CTRL_ADDR, (localVal & (~0x1)) | ((enable&0x1)<<0));
}

#endif // _WIFI_PDMA_REG_H_

/// @}

