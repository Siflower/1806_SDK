/**
 * @file reg_macbypass.h
 * @brief Definitions of the MACBYP HW block registers and register access functions.
 *
 * @defgroup REG_MACBYPASS REG_MACBYPASS
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the MACBYP HW block registers and register access functions.
 */
#ifndef _REG_MACBYPASS_H_
#define _REG_MACBYPASS_H_

#include "co_int.h"
#include "_reg_macbypass.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_MACBYPASS peripheral.
 */
#define REG_MACBYPASS_COUNT 144

/** @brief Decoding mask of the REG_MACBYPASS peripheral registers from the CPU point of view.
 */
#define REG_MACBYPASS_DECODING_MASK 0x000003FF

/**
 * @name CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31              TX_DONE   0
 *  11:08                 MODE   0x0
 *     04           CLEAR_STAT   0
 *     00               BYPASS   0
 * </pre>
 *
 * @{
 */

/// Address of the CTRL register
#define MACBYP_CTRL_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0000)
/// Offset of the CTRL register from the base address
#define MACBYP_CTRL_OFFSET 0x00000000
/// Index of the CTRL register
#define MACBYP_CTRL_INDEX  0x00000000
/// Reset value of the CTRL register
#define MACBYP_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the CTRL register.
 * The CTRL register will be read and its value returned.
 * @return The current value of the CTRL register.
 */
__INLINE uint32_t macbyp_ctrl_get(void)
{
    return REG_PL_RD(MACBYP_CTRL_ADDR);
}

/**
 * @brief Sets the CTRL register to a value.
 * The CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_ctrl_set(uint32_t value)
{
    REG_PL_WR(MACBYP_CTRL_ADDR, value);
}

// field definitions
/// TX_DONE field bit
#define MACBYP_TX_DONE_BIT       ((uint32_t)0x80000000)
/// TX_DONE field position
#define MACBYP_TX_DONE_POS       31
/// MODE field mask
#define MACBYP_MODE_MASK         ((uint32_t)0x00000F00)
/// MODE field LSB position
#define MACBYP_MODE_LSB          8
/// MODE field width
#define MACBYP_MODE_WIDTH        ((uint32_t)0x00000004)
/// CLEAR_STAT field bit
#define MACBYP_CLEAR_STAT_BIT    ((uint32_t)0x00000010)
/// CLEAR_STAT field position
#define MACBYP_CLEAR_STAT_POS    4
/// BYPASS field bit
#define MACBYP_BYPASS_BIT        ((uint32_t)0x00000001)
/// BYPASS field position
#define MACBYP_BYPASS_POS        0

/// TX_DONE field reset value
#define MACBYP_TX_DONE_RST       0x0
/// MODE field reset value
#define MACBYP_MODE_RST          0x0
/// CLEAR_STAT field reset value
#define MACBYP_CLEAR_STAT_RST    0x0
/// BYPASS field reset value
#define MACBYP_BYPASS_RST        0x0

/**
 * @brief Constructs a value for the CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txdone - The value to use for the TX_DONE field.
 * @param[in] mode - The value to use for the MODE field.
 * @param[in] clearstat - The value to use for the CLEAR_STAT field.
 * @param[in] bypass - The value to use for the BYPASS field.
 */
__INLINE void macbyp_ctrl_pack(uint8_t txdone, uint8_t mode, uint8_t clearstat, uint8_t bypass)
{
    ASSERT_ERR((((uint32_t)txdone << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)mode << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)clearstat << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)bypass << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_CTRL_ADDR,  ((uint32_t)txdone << 31) | ((uint32_t)mode << 8) | ((uint32_t)clearstat << 4) | ((uint32_t)bypass << 0));
}

/**
 * @brief Unpacks CTRL's fields from current value of the CTRL register.
 *
 * Reads the CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txdone - Will be populated with the current value of this field from the register.
 * @param[out] mode - Will be populated with the current value of this field from the register.
 * @param[out] clearstat - Will be populated with the current value of this field from the register.
 * @param[out] bypass - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_ctrl_unpack(uint8_t* txdone, uint8_t* mode, uint8_t* clearstat, uint8_t* bypass)
{
    uint32_t localVal = REG_PL_RD(MACBYP_CTRL_ADDR);

    *txdone = (localVal & ((uint32_t)0x80000000)) >> 31;
    *mode = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *clearstat = (localVal & ((uint32_t)0x00000010)) >> 4;
    *bypass = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TX_DONE field in the CTRL register.
 *
 * The CTRL register will be read and the TX_DONE field's value will be returned.
 *
 * @return The current value of the TX_DONE field in the CTRL register.
 */
__INLINE uint8_t macbyp_tx_done_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TX_DONE field of the CTRL register.
 *
 * The CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txdone - The value to set the field to.
 */
__INLINE void macbyp_tx_done_setf(uint8_t txdone)
{
    ASSERT_ERR((((uint32_t)txdone << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_CTRL_ADDR, (uint32_t)txdone << 31);
}

/**
 * @brief Returns the current value of the MODE field in the CTRL register.
 *
 * The CTRL register will be read and the MODE field's value will be returned.
 *
 * @return The current value of the MODE field in the CTRL register.
 */
__INLINE uint8_t macbyp_mode_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the MODE field of the CTRL register.
 *
 * The CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mode - The value to set the field to.
 */
__INLINE void macbyp_mode_setf(uint8_t mode)
{
    ASSERT_ERR((((uint32_t)mode << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(MACBYP_CTRL_ADDR, (uint32_t)mode << 8);
}

/**
 * @brief Returns the current value of the CLEAR_STAT field in the CTRL register.
 *
 * The CTRL register will be read and the CLEAR_STAT field's value will be returned.
 *
 * @return The current value of the CLEAR_STAT field in the CTRL register.
 */
__INLINE uint8_t macbyp_clear_stat_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the CLEAR_STAT field of the CTRL register.
 *
 * The CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] clearstat - The value to set the field to.
 */
__INLINE void macbyp_clear_stat_setf(uint8_t clearstat)
{
    ASSERT_ERR((((uint32_t)clearstat << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(MACBYP_CTRL_ADDR, (uint32_t)clearstat << 4);
}

/**
 * @brief Returns the current value of the BYPASS field in the CTRL register.
 *
 * The CTRL register will be read and the BYPASS field's value will be returned.
 *
 * @return The current value of the BYPASS field in the CTRL register.
 */
__INLINE uint8_t macbyp_bypass_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the BYPASS field of the CTRL register.
 *
 * The CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] bypass - The value to set the field to.
 */
__INLINE void macbyp_bypass_setf(uint8_t bypass)
{
    ASSERT_ERR((((uint32_t)bypass << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_CTRL_ADDR, (uint32_t)bypass << 0);
}

/// @}

/**
 * @name PAYLOAD register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  03:00              PAYLOAD   0x0
 * </pre>
 *
 * @{
 */

/// Address of the PAYLOAD register
#define MACBYP_PAYLOAD_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0004)
/// Offset of the PAYLOAD register from the base address
#define MACBYP_PAYLOAD_OFFSET 0x00000004
/// Index of the PAYLOAD register
#define MACBYP_PAYLOAD_INDEX  0x00000001
/// Reset value of the PAYLOAD register
#define MACBYP_PAYLOAD_RESET  0x00000000

/**
 * @brief Returns the current value of the PAYLOAD register.
 * The PAYLOAD register will be read and its value returned.
 * @return The current value of the PAYLOAD register.
 */
__INLINE uint32_t macbyp_payload_get(void)
{
    return REG_PL_RD(MACBYP_PAYLOAD_ADDR);
}

/**
 * @brief Sets the PAYLOAD register to a value.
 * The PAYLOAD register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_payload_set(uint32_t value)
{
    REG_PL_WR(MACBYP_PAYLOAD_ADDR, value);
}

// field definitions
/// PAYLOAD field mask
#define MACBYP_PAYLOAD_MASK   ((uint32_t)0x0000000F)
/// PAYLOAD field LSB position
#define MACBYP_PAYLOAD_LSB    0
/// PAYLOAD field width
#define MACBYP_PAYLOAD_WIDTH  ((uint32_t)0x00000004)

/// PAYLOAD field reset value
#define MACBYP_PAYLOAD_RST    0x0

/**
 * @brief Returns the current value of the PAYLOAD field in the PAYLOAD register.
 *
 * The PAYLOAD register will be read and the PAYLOAD field's value will be returned.
 *
 * @return The current value of the PAYLOAD field in the PAYLOAD register.
 */
__INLINE uint8_t macbyp_payload_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_PAYLOAD_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000000F)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the PAYLOAD field of the PAYLOAD register.
 *
 * The PAYLOAD register will be read, modified to contain the new field value, and written.
 *
 * @param[in] payload - The value to set the field to.
 */
__INLINE void macbyp_payload_setf(uint8_t payload)
{
    ASSERT_ERR((((uint32_t)payload << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MACBYP_PAYLOAD_ADDR, (uint32_t)payload << 0);
}

/// @}

/**
 * @name TRIGGER register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:04             TRIGGER1   0x0
 *  03:00             TRIGGER0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIGGER register
#define MACBYP_TRIGGER_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0008)
/// Offset of the TRIGGER register from the base address
#define MACBYP_TRIGGER_OFFSET 0x00000008
/// Index of the TRIGGER register
#define MACBYP_TRIGGER_INDEX  0x00000002
/// Reset value of the TRIGGER register
#define MACBYP_TRIGGER_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIGGER register.
 * The TRIGGER register will be read and its value returned.
 * @return The current value of the TRIGGER register.
 */
__INLINE uint32_t macbyp_trigger_get(void)
{
    return REG_PL_RD(MACBYP_TRIGGER_ADDR);
}

/**
 * @brief Sets the TRIGGER register to a value.
 * The TRIGGER register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trigger_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIGGER_ADDR, value);
}

// field definitions
/// TRIGGER1 field mask
#define MACBYP_TRIGGER1_MASK   ((uint32_t)0x000000F0)
/// TRIGGER1 field LSB position
#define MACBYP_TRIGGER1_LSB    4
/// TRIGGER1 field width
#define MACBYP_TRIGGER1_WIDTH  ((uint32_t)0x00000004)
/// TRIGGER0 field mask
#define MACBYP_TRIGGER0_MASK   ((uint32_t)0x0000000F)
/// TRIGGER0 field LSB position
#define MACBYP_TRIGGER0_LSB    0
/// TRIGGER0 field width
#define MACBYP_TRIGGER0_WIDTH  ((uint32_t)0x00000004)

/// TRIGGER1 field reset value
#define MACBYP_TRIGGER1_RST    0x0
/// TRIGGER0 field reset value
#define MACBYP_TRIGGER0_RST    0x0

/**
 * @brief Constructs a value for the TRIGGER register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigger1 - The value to use for the TRIGGER1 field.
 * @param[in] trigger0 - The value to use for the TRIGGER0 field.
 */
__INLINE void macbyp_trigger_pack(uint8_t trigger1, uint8_t trigger0)
{
    ASSERT_ERR((((uint32_t)trigger1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)trigger0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MACBYP_TRIGGER_ADDR,  ((uint32_t)trigger1 << 4) | ((uint32_t)trigger0 << 0));
}

/**
 * @brief Unpacks TRIGGER's fields from current value of the TRIGGER register.
 *
 * Reads the TRIGGER register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigger1 - Will be populated with the current value of this field from the register.
 * @param[out] trigger0 - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trigger_unpack(uint8_t* trigger1, uint8_t* trigger0)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIGGER_ADDR);

    *trigger1 = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *trigger0 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the TRIGGER1 field in the TRIGGER register.
 *
 * The TRIGGER register will be read and the TRIGGER1 field's value will be returned.
 *
 * @return The current value of the TRIGGER1 field in the TRIGGER register.
 */
__INLINE uint8_t macbyp_trigger1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIGGER_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Sets the TRIGGER1 field of the TRIGGER register.
 *
 * The TRIGGER register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigger1 - The value to set the field to.
 */
__INLINE void macbyp_trigger1_setf(uint8_t trigger1)
{
    ASSERT_ERR((((uint32_t)trigger1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_PL_WR(MACBYP_TRIGGER_ADDR, (REG_PL_RD(MACBYP_TRIGGER_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)trigger1 << 4));
}

/**
 * @brief Returns the current value of the TRIGGER0 field in the TRIGGER register.
 *
 * The TRIGGER register will be read and the TRIGGER0 field's value will be returned.
 *
 * @return The current value of the TRIGGER0 field in the TRIGGER register.
 */
__INLINE uint8_t macbyp_trigger0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIGGER_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the TRIGGER0 field of the TRIGGER register.
 *
 * The TRIGGER register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigger0 - The value to set the field to.
 */
__INLINE void macbyp_trigger0_setf(uint8_t trigger0)
{
    ASSERT_ERR((((uint32_t)trigger0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MACBYP_TRIGGER_ADDR, (REG_PL_RD(MACBYP_TRIGGER_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)trigger0 << 0));
}

/// @}

/**
 * @name PRE_TX_DELAY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  19:00         PRE_TX_DELAY   0x0
 * </pre>
 *
 * @{
 */

/// Address of the PRE_TX_DELAY register
#define MACBYP_PRE_TX_DELAY_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0048)
/// Offset of the PRE_TX_DELAY register from the base address
#define MACBYP_PRE_TX_DELAY_OFFSET 0x00000048
/// Index of the PRE_TX_DELAY register
#define MACBYP_PRE_TX_DELAY_INDEX  0x00000012
/// Reset value of the PRE_TX_DELAY register
#define MACBYP_PRE_TX_DELAY_RESET  0x00000000

/**
 * @brief Returns the current value of the PRE_TX_DELAY register.
 * The PRE_TX_DELAY register will be read and its value returned.
 * @return The current value of the PRE_TX_DELAY register.
 */
__INLINE uint32_t macbyp_pre_tx_delay_get(void)
{
    return REG_PL_RD(MACBYP_PRE_TX_DELAY_ADDR);
}

/**
 * @brief Sets the PRE_TX_DELAY register to a value.
 * The PRE_TX_DELAY register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_pre_tx_delay_set(uint32_t value)
{
    REG_PL_WR(MACBYP_PRE_TX_DELAY_ADDR, value);
}

// field definitions
/// PRE_TX_DELAY field mask
#define MACBYP_PRE_TX_DELAY_MASK   ((uint32_t)0x000FFFFF)
/// PRE_TX_DELAY field LSB position
#define MACBYP_PRE_TX_DELAY_LSB    0
/// PRE_TX_DELAY field width
#define MACBYP_PRE_TX_DELAY_WIDTH  ((uint32_t)0x00000014)

/// PRE_TX_DELAY field reset value
#define MACBYP_PRE_TX_DELAY_RST    0x0

/**
 * @brief Returns the current value of the PRE_TX_DELAY field in the PRE_TX_DELAY register.
 *
 * The PRE_TX_DELAY register will be read and the PRE_TX_DELAY field's value will be returned.
 *
 * @return The current value of the PRE_TX_DELAY field in the PRE_TX_DELAY register.
 */
__INLINE uint32_t macbyp_pre_tx_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_PRE_TX_DELAY_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000FFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the PRE_TX_DELAY field of the PRE_TX_DELAY register.
 *
 * The PRE_TX_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] pretxdelay - The value to set the field to.
 */
__INLINE void macbyp_pre_tx_delay_setf(uint32_t pretxdelay)
{
    ASSERT_ERR((((uint32_t)pretxdelay << 0) & ~((uint32_t)0x000FFFFF)) == 0);
    REG_PL_WR(MACBYP_PRE_TX_DELAY_ADDR, (uint32_t)pretxdelay << 0);
}

/// @}

/**
 * @name STAT_FRAME_OK register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00        STAT_FRAME_OK   0x0
 * </pre>
 *
 * @{
 */

/// Address of the STAT_FRAME_OK register
#define MACBYP_STAT_FRAME_OK_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0080)
/// Offset of the STAT_FRAME_OK register from the base address
#define MACBYP_STAT_FRAME_OK_OFFSET 0x00000080
/// Index of the STAT_FRAME_OK register
#define MACBYP_STAT_FRAME_OK_INDEX  0x00000020
/// Reset value of the STAT_FRAME_OK register
#define MACBYP_STAT_FRAME_OK_RESET  0x00000000

/**
 * @brief Returns the current value of the STAT_FRAME_OK register.
 * The STAT_FRAME_OK register will be read and its value returned.
 * @return The current value of the STAT_FRAME_OK register.
 */
__INLINE uint32_t macbyp_stat_frame_ok_get(void)
{
    return REG_PL_RD(MACBYP_STAT_FRAME_OK_ADDR);
}

/**
 * @brief Sets the STAT_FRAME_OK register to a value.
 * The STAT_FRAME_OK register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_stat_frame_ok_set(uint32_t value)
{
    REG_PL_WR(MACBYP_STAT_FRAME_OK_ADDR, value);
}

// field definitions
/// STAT_FRAME_OK field mask
#define MACBYP_STAT_FRAME_OK_MASK   ((uint32_t)0xFFFFFFFF)
/// STAT_FRAME_OK field LSB position
#define MACBYP_STAT_FRAME_OK_LSB    0
/// STAT_FRAME_OK field width
#define MACBYP_STAT_FRAME_OK_WIDTH  ((uint32_t)0x00000020)

/// STAT_FRAME_OK field reset value
#define MACBYP_STAT_FRAME_OK_RST    0x0

/**
 * @brief Returns the current value of the STAT_FRAME_OK field in the STAT_FRAME_OK register.
 *
 * The STAT_FRAME_OK register will be read and the STAT_FRAME_OK field's value will be returned.
 *
 * @return The current value of the STAT_FRAME_OK field in the STAT_FRAME_OK register.
 */
__INLINE uint32_t macbyp_stat_frame_ok_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_STAT_FRAME_OK_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the STAT_FRAME_OK field of the STAT_FRAME_OK register.
 *
 * The STAT_FRAME_OK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] statframeok - The value to set the field to.
 */
__INLINE void macbyp_stat_frame_ok_setf(uint32_t statframeok)
{
    ASSERT_ERR((((uint32_t)statframeok << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MACBYP_STAT_FRAME_OK_ADDR, (uint32_t)statframeok << 0);
}

/// @}

/**
 * @name STAT_FRAME_BAD register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00       STAT_FRAME_BAD   0x0
 * </pre>
 *
 * @{
 */

/// Address of the STAT_FRAME_BAD register
#define MACBYP_STAT_FRAME_BAD_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0084)
/// Offset of the STAT_FRAME_BAD register from the base address
#define MACBYP_STAT_FRAME_BAD_OFFSET 0x00000084
/// Index of the STAT_FRAME_BAD register
#define MACBYP_STAT_FRAME_BAD_INDEX  0x00000021
/// Reset value of the STAT_FRAME_BAD register
#define MACBYP_STAT_FRAME_BAD_RESET  0x00000000

/**
 * @brief Returns the current value of the STAT_FRAME_BAD register.
 * The STAT_FRAME_BAD register will be read and its value returned.
 * @return The current value of the STAT_FRAME_BAD register.
 */
__INLINE uint32_t macbyp_stat_frame_bad_get(void)
{
    return REG_PL_RD(MACBYP_STAT_FRAME_BAD_ADDR);
}

/**
 * @brief Sets the STAT_FRAME_BAD register to a value.
 * The STAT_FRAME_BAD register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_stat_frame_bad_set(uint32_t value)
{
    REG_PL_WR(MACBYP_STAT_FRAME_BAD_ADDR, value);
}

// field definitions
/// STAT_FRAME_BAD field mask
#define MACBYP_STAT_FRAME_BAD_MASK   ((uint32_t)0xFFFFFFFF)
/// STAT_FRAME_BAD field LSB position
#define MACBYP_STAT_FRAME_BAD_LSB    0
/// STAT_FRAME_BAD field width
#define MACBYP_STAT_FRAME_BAD_WIDTH  ((uint32_t)0x00000020)

/// STAT_FRAME_BAD field reset value
#define MACBYP_STAT_FRAME_BAD_RST    0x0

/**
 * @brief Returns the current value of the STAT_FRAME_BAD field in the STAT_FRAME_BAD register.
 *
 * The STAT_FRAME_BAD register will be read and the STAT_FRAME_BAD field's value will be returned.
 *
 * @return The current value of the STAT_FRAME_BAD field in the STAT_FRAME_BAD register.
 */
__INLINE uint32_t macbyp_stat_frame_bad_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_STAT_FRAME_BAD_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the STAT_FRAME_BAD field of the STAT_FRAME_BAD register.
 *
 * The STAT_FRAME_BAD register will be read, modified to contain the new field value, and written.
 *
 * @param[in] statframebad - The value to set the field to.
 */
__INLINE void macbyp_stat_frame_bad_setf(uint32_t statframebad)
{
    ASSERT_ERR((((uint32_t)statframebad << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MACBYP_STAT_FRAME_BAD_ADDR, (uint32_t)statframebad << 0);
}

/// @}

/**
 * @name STAT_RXEND register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00     STAT_FRAME_RXEND   0x0
 * </pre>
 *
 * @{
 */

/// Address of the STAT_RXEND register
#define MACBYP_STAT_RXEND_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0088)
/// Offset of the STAT_RXEND register from the base address
#define MACBYP_STAT_RXEND_OFFSET 0x00000088
/// Index of the STAT_RXEND register
#define MACBYP_STAT_RXEND_INDEX  0x00000022
/// Reset value of the STAT_RXEND register
#define MACBYP_STAT_RXEND_RESET  0x00000000

/**
 * @brief Returns the current value of the STAT_RXEND register.
 * The STAT_RXEND register will be read and its value returned.
 * @return The current value of the STAT_RXEND register.
 */
__INLINE uint32_t macbyp_stat_rxend_get(void)
{
    return REG_PL_RD(MACBYP_STAT_RXEND_ADDR);
}

/**
 * @brief Sets the STAT_RXEND register to a value.
 * The STAT_RXEND register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_stat_rxend_set(uint32_t value)
{
    REG_PL_WR(MACBYP_STAT_RXEND_ADDR, value);
}

// field definitions
/// STAT_FRAME_RXEND field mask
#define MACBYP_STAT_FRAME_RXEND_MASK   ((uint32_t)0xFFFFFFFF)
/// STAT_FRAME_RXEND field LSB position
#define MACBYP_STAT_FRAME_RXEND_LSB    0
/// STAT_FRAME_RXEND field width
#define MACBYP_STAT_FRAME_RXEND_WIDTH  ((uint32_t)0x00000020)

/// STAT_FRAME_RXEND field reset value
#define MACBYP_STAT_FRAME_RXEND_RST    0x0

/**
 * @brief Returns the current value of the STAT_FRAME_RXEND field in the STAT_RXEND register.
 *
 * The STAT_RXEND register will be read and the STAT_FRAME_RXEND field's value will be returned.
 *
 * @return The current value of the STAT_FRAME_RXEND field in the STAT_RXEND register.
 */
__INLINE uint32_t macbyp_stat_frame_rxend_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_STAT_RXEND_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the STAT_FRAME_RXEND field of the STAT_RXEND register.
 *
 * The STAT_RXEND register will be read, modified to contain the new field value, and written.
 *
 * @param[in] statframerxend - The value to set the field to.
 */
__INLINE void macbyp_stat_frame_rxend_setf(uint32_t statframerxend)
{
    ASSERT_ERR((((uint32_t)statframerxend << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MACBYP_STAT_RXEND_ADDR, (uint32_t)statframerxend << 0);
}

/// @}

/**
 * @name STAT_RXERROR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00   STAT_FRAME_RXERROR   0x0
 * </pre>
 *
 * @{
 */

/// Address of the STAT_RXERROR register
#define MACBYP_STAT_RXERROR_ADDR   (REG_MACBYPASS_BASE_ADDR+0x008C)
/// Offset of the STAT_RXERROR register from the base address
#define MACBYP_STAT_RXERROR_OFFSET 0x0000008C
/// Index of the STAT_RXERROR register
#define MACBYP_STAT_RXERROR_INDEX  0x00000023
/// Reset value of the STAT_RXERROR register
#define MACBYP_STAT_RXERROR_RESET  0x00000000

/**
 * @brief Returns the current value of the STAT_RXERROR register.
 * The STAT_RXERROR register will be read and its value returned.
 * @return The current value of the STAT_RXERROR register.
 */
__INLINE uint32_t macbyp_stat_rxerror_get(void)
{
    return REG_PL_RD(MACBYP_STAT_RXERROR_ADDR);
}

/**
 * @brief Sets the STAT_RXERROR register to a value.
 * The STAT_RXERROR register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_stat_rxerror_set(uint32_t value)
{
    REG_PL_WR(MACBYP_STAT_RXERROR_ADDR, value);
}

// field definitions
/// STAT_FRAME_RXERROR field mask
#define MACBYP_STAT_FRAME_RXERROR_MASK   ((uint32_t)0xFFFFFFFF)
/// STAT_FRAME_RXERROR field LSB position
#define MACBYP_STAT_FRAME_RXERROR_LSB    0
/// STAT_FRAME_RXERROR field width
#define MACBYP_STAT_FRAME_RXERROR_WIDTH  ((uint32_t)0x00000020)

/// STAT_FRAME_RXERROR field reset value
#define MACBYP_STAT_FRAME_RXERROR_RST    0x0

/**
 * @brief Returns the current value of the STAT_FRAME_RXERROR field in the STAT_RXERROR register.
 *
 * The STAT_RXERROR register will be read and the STAT_FRAME_RXERROR field's value will be returned.
 *
 * @return The current value of the STAT_FRAME_RXERROR field in the STAT_RXERROR register.
 */
__INLINE uint32_t macbyp_stat_frame_rxerror_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_STAT_RXERROR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the STAT_FRAME_RXERROR field of the STAT_RXERROR register.
 *
 * The STAT_RXERROR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] statframerxerror - The value to set the field to.
 */
__INLINE void macbyp_stat_frame_rxerror_setf(uint32_t statframerxerror)
{
    ASSERT_ERR((((uint32_t)statframerxerror << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MACBYP_STAT_RXERROR_ADDR, (uint32_t)statframerxerror << 0);
}

/// @}

/**
 * @name EVM register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                  EVM   0x0
 * </pre>
 *
 * @{
 */

/// Address of the EVM register
#define MACBYP_EVM_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0090)
/// Offset of the EVM register from the base address
#define MACBYP_EVM_OFFSET 0x00000090
/// Index of the EVM register
#define MACBYP_EVM_INDEX  0x00000024
/// Reset value of the EVM register
#define MACBYP_EVM_RESET  0x00000000

/**
 * @brief Returns the current value of the EVM register.
 * The EVM register will be read and its value returned.
 * @return The current value of the EVM register.
 */
__INLINE uint32_t macbyp_evm_get(void)
{
    return REG_PL_RD(MACBYP_EVM_ADDR);
}

/**
 * @brief Sets the EVM register to a value.
 * The EVM register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_evm_set(uint32_t value)
{
    REG_PL_WR(MACBYP_EVM_ADDR, value);
}

// field definitions
/// EVM field mask
#define MACBYP_EVM_MASK   ((uint32_t)0x000000FF)
/// EVM field LSB position
#define MACBYP_EVM_LSB    0
/// EVM field width
#define MACBYP_EVM_WIDTH  ((uint32_t)0x00000008)

/// EVM field reset value
#define MACBYP_EVM_RST    0x0

/**
 * @brief Returns the current value of the EVM field in the EVM register.
 *
 * The EVM register will be read and the EVM field's value will be returned.
 *
 * @return The current value of the EVM field in the EVM register.
 */
__INLINE uint8_t macbyp_evm_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_EVM_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the EVM field of the EVM register.
 *
 * The EVM register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evm - The value to set the field to.
 */
__INLINE void macbyp_evm_setf(uint8_t evm)
{
    ASSERT_ERR((((uint32_t)evm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_EVM_ADDR, (uint32_t)evm << 0);
}

/// @}

/**
 * @name RSSI register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 RSSI   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RSSI register
#define MACBYP_RSSI_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0094)
/// Offset of the RSSI register from the base address
#define MACBYP_RSSI_OFFSET 0x00000094
/// Index of the RSSI register
#define MACBYP_RSSI_INDEX  0x00000025
/// Reset value of the RSSI register
#define MACBYP_RSSI_RESET  0x00000000

/**
 * @brief Returns the current value of the RSSI register.
 * The RSSI register will be read and its value returned.
 * @return The current value of the RSSI register.
 */
__INLINE uint32_t macbyp_rssi_get(void)
{
    return REG_PL_RD(MACBYP_RSSI_ADDR);
}

/**
 * @brief Sets the RSSI register to a value.
 * The RSSI register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_rssi_set(uint32_t value)
{
    REG_PL_WR(MACBYP_RSSI_ADDR, value);
}

// field definitions
/// RSSI field mask
#define MACBYP_RSSI_MASK   ((uint32_t)0x000000FF)
/// RSSI field LSB position
#define MACBYP_RSSI_LSB    0
/// RSSI field width
#define MACBYP_RSSI_WIDTH  ((uint32_t)0x00000008)

/// RSSI field reset value
#define MACBYP_RSSI_RST    0x0

/**
 * @brief Returns the current value of the RSSI field in the RSSI register.
 *
 * The RSSI register will be read and the RSSI field's value will be returned.
 *
 * @return The current value of the RSSI field in the RSSI register.
 */
__INLINE uint8_t macbyp_rssi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_RSSI_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the RSSI field of the RSSI register.
 *
 * The RSSI register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rssi - The value to set the field to.
 */
__INLINE void macbyp_rssi_setf(uint8_t rssi)
{
    ASSERT_ERR((((uint32_t)rssi << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_RSSI_ADDR, (uint32_t)rssi << 0);
}

/// @}

/**
 * @name TRIG_FORMATMOD register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31    TRIG_FORMATMOD_EN   0
 *  02:00       TRIG_FORMATMOD   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_FORMATMOD register
#define MACBYP_TRIG_FORMATMOD_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0100)
/// Offset of the TRIG_FORMATMOD register from the base address
#define MACBYP_TRIG_FORMATMOD_OFFSET 0x00000100
/// Index of the TRIG_FORMATMOD register
#define MACBYP_TRIG_FORMATMOD_INDEX  0x00000040
/// Reset value of the TRIG_FORMATMOD register
#define MACBYP_TRIG_FORMATMOD_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_FORMATMOD register.
 * The TRIG_FORMATMOD register will be read and its value returned.
 * @return The current value of the TRIG_FORMATMOD register.
 */
__INLINE uint32_t macbyp_trig_formatmod_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR);
}

/**
 * @brief Sets the TRIG_FORMATMOD register to a value.
 * The TRIG_FORMATMOD register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_formatmod_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_FORMATMOD_ADDR, value);
}

// field definitions
/// TRIG_FORMATMOD_EN field bit
#define MACBYP_TRIG_FORMATMOD_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_FORMATMOD_EN field position
#define MACBYP_TRIG_FORMATMOD_EN_POS    31
/// TRIG_FORMATMOD field mask
#define MACBYP_TRIG_FORMATMOD_MASK      ((uint32_t)0x00000007)
/// TRIG_FORMATMOD field LSB position
#define MACBYP_TRIG_FORMATMOD_LSB       0
/// TRIG_FORMATMOD field width
#define MACBYP_TRIG_FORMATMOD_WIDTH     ((uint32_t)0x00000003)

/// TRIG_FORMATMOD_EN field reset value
#define MACBYP_TRIG_FORMATMOD_EN_RST    0x0
/// TRIG_FORMATMOD field reset value
#define MACBYP_TRIG_FORMATMOD_RST       0x0

/**
 * @brief Constructs a value for the TRIG_FORMATMOD register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigformatmoden - The value to use for the TRIG_FORMATMOD_EN field.
 * @param[in] trigformatmod - The value to use for the TRIG_FORMATMOD field.
 */
__INLINE void macbyp_trig_formatmod_pack(uint8_t trigformatmoden, uint8_t trigformatmod)
{
    ASSERT_ERR((((uint32_t)trigformatmoden << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigformatmod << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(MACBYP_TRIG_FORMATMOD_ADDR,  ((uint32_t)trigformatmoden << 31) | ((uint32_t)trigformatmod << 0));
}

/**
 * @brief Unpacks TRIG_FORMATMOD's fields from current value of the TRIG_FORMATMOD register.
 *
 * Reads the TRIG_FORMATMOD register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigformatmoden - Will be populated with the current value of this field from the register.
 * @param[out] trigformatmod - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_formatmod_unpack(uint8_t* trigformatmoden, uint8_t* trigformatmod)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR);

    *trigformatmoden = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigformatmod = (localVal & ((uint32_t)0x00000007)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_FORMATMOD_EN field in the TRIG_FORMATMOD register.
 *
 * The TRIG_FORMATMOD register will be read and the TRIG_FORMATMOD_EN field's value will be returned.
 *
 * @return The current value of the TRIG_FORMATMOD_EN field in the TRIG_FORMATMOD register.
 */
__INLINE uint8_t macbyp_trig_formatmod_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_FORMATMOD_EN field of the TRIG_FORMATMOD register.
 *
 * The TRIG_FORMATMOD register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigformatmoden - The value to set the field to.
 */
__INLINE void macbyp_trig_formatmod_en_setf(uint8_t trigformatmoden)
{
    ASSERT_ERR((((uint32_t)trigformatmoden << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_FORMATMOD_ADDR, (REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigformatmoden << 31));
}

/**
 * @brief Returns the current value of the TRIG_FORMATMOD field in the TRIG_FORMATMOD register.
 *
 * The TRIG_FORMATMOD register will be read and the TRIG_FORMATMOD field's value will be returned.
 *
 * @return The current value of the TRIG_FORMATMOD field in the TRIG_FORMATMOD register.
 */
__INLINE uint8_t macbyp_trig_formatmod_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR);
    return ((localVal & ((uint32_t)0x00000007)) >> 0);
}

/**
 * @brief Sets the TRIG_FORMATMOD field of the TRIG_FORMATMOD register.
 *
 * The TRIG_FORMATMOD register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigformatmod - The value to set the field to.
 */
__INLINE void macbyp_trig_formatmod_setf(uint8_t trigformatmod)
{
    ASSERT_ERR((((uint32_t)trigformatmod << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(MACBYP_TRIG_FORMATMOD_ADDR, (REG_PL_RD(MACBYP_TRIG_FORMATMOD_ADDR) & ~((uint32_t)0x00000007)) | ((uint32_t)trigformatmod << 0));
}

/// @}

/**
 * @name TRIG_MCS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31          TRIG_MCS_EN   0
 *  06:00             TRIG_MCS   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_MCS register
#define MACBYP_TRIG_MCS_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0104)
/// Offset of the TRIG_MCS register from the base address
#define MACBYP_TRIG_MCS_OFFSET 0x00000104
/// Index of the TRIG_MCS register
#define MACBYP_TRIG_MCS_INDEX  0x00000041
/// Reset value of the TRIG_MCS register
#define MACBYP_TRIG_MCS_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_MCS register.
 * The TRIG_MCS register will be read and its value returned.
 * @return The current value of the TRIG_MCS register.
 */
__INLINE uint32_t macbyp_trig_mcs_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_MCS_ADDR);
}

/**
 * @brief Sets the TRIG_MCS register to a value.
 * The TRIG_MCS register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_mcs_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_MCS_ADDR, value);
}

// field definitions
/// TRIG_MCS_EN field bit
#define MACBYP_TRIG_MCS_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_MCS_EN field position
#define MACBYP_TRIG_MCS_EN_POS    31
/// TRIG_MCS field mask
#define MACBYP_TRIG_MCS_MASK      ((uint32_t)0x0000007F)
/// TRIG_MCS field LSB position
#define MACBYP_TRIG_MCS_LSB       0
/// TRIG_MCS field width
#define MACBYP_TRIG_MCS_WIDTH     ((uint32_t)0x00000007)

/// TRIG_MCS_EN field reset value
#define MACBYP_TRIG_MCS_EN_RST    0x0
/// TRIG_MCS field reset value
#define MACBYP_TRIG_MCS_RST       0x0

/**
 * @brief Constructs a value for the TRIG_MCS register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigmcsen - The value to use for the TRIG_MCS_EN field.
 * @param[in] trigmcs - The value to use for the TRIG_MCS field.
 */
__INLINE void macbyp_trig_mcs_pack(uint8_t trigmcsen, uint8_t trigmcs)
{
    ASSERT_ERR((((uint32_t)trigmcsen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigmcs << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MACBYP_TRIG_MCS_ADDR,  ((uint32_t)trigmcsen << 31) | ((uint32_t)trigmcs << 0));
}

/**
 * @brief Unpacks TRIG_MCS's fields from current value of the TRIG_MCS register.
 *
 * Reads the TRIG_MCS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigmcsen - Will be populated with the current value of this field from the register.
 * @param[out] trigmcs - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_mcs_unpack(uint8_t* trigmcsen, uint8_t* trigmcs)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_MCS_ADDR);

    *trigmcsen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigmcs = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_MCS_EN field in the TRIG_MCS register.
 *
 * The TRIG_MCS register will be read and the TRIG_MCS_EN field's value will be returned.
 *
 * @return The current value of the TRIG_MCS_EN field in the TRIG_MCS register.
 */
__INLINE uint8_t macbyp_trig_mcs_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_MCS_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_MCS_EN field of the TRIG_MCS register.
 *
 * The TRIG_MCS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigmcsen - The value to set the field to.
 */
__INLINE void macbyp_trig_mcs_en_setf(uint8_t trigmcsen)
{
    ASSERT_ERR((((uint32_t)trigmcsen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_MCS_ADDR, (REG_PL_RD(MACBYP_TRIG_MCS_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigmcsen << 31));
}

/**
 * @brief Returns the current value of the TRIG_MCS field in the TRIG_MCS register.
 *
 * The TRIG_MCS register will be read and the TRIG_MCS field's value will be returned.
 *
 * @return The current value of the TRIG_MCS field in the TRIG_MCS register.
 */
__INLINE uint8_t macbyp_trig_mcs_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_MCS_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the TRIG_MCS field of the TRIG_MCS register.
 *
 * The TRIG_MCS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigmcs - The value to set the field to.
 */
__INLINE void macbyp_trig_mcs_setf(uint8_t trigmcs)
{
    ASSERT_ERR((((uint32_t)trigmcs << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MACBYP_TRIG_MCS_ADDR, (REG_PL_RD(MACBYP_TRIG_MCS_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)trigmcs << 0));
}

/// @}

/**
 * @name TRIG_SGI register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31          TRIG_SGI_EN   0
 *     00             TRIG_SGI   0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_SGI register
#define MACBYP_TRIG_SGI_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0108)
/// Offset of the TRIG_SGI register from the base address
#define MACBYP_TRIG_SGI_OFFSET 0x00000108
/// Index of the TRIG_SGI register
#define MACBYP_TRIG_SGI_INDEX  0x00000042
/// Reset value of the TRIG_SGI register
#define MACBYP_TRIG_SGI_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_SGI register.
 * The TRIG_SGI register will be read and its value returned.
 * @return The current value of the TRIG_SGI register.
 */
__INLINE uint32_t macbyp_trig_sgi_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_SGI_ADDR);
}

/**
 * @brief Sets the TRIG_SGI register to a value.
 * The TRIG_SGI register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_sgi_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_SGI_ADDR, value);
}

// field definitions
/// TRIG_SGI_EN field bit
#define MACBYP_TRIG_SGI_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_SGI_EN field position
#define MACBYP_TRIG_SGI_EN_POS    31
/// TRIG_SGI field bit
#define MACBYP_TRIG_SGI_BIT       ((uint32_t)0x00000001)
/// TRIG_SGI field position
#define MACBYP_TRIG_SGI_POS       0

/// TRIG_SGI_EN field reset value
#define MACBYP_TRIG_SGI_EN_RST    0x0
/// TRIG_SGI field reset value
#define MACBYP_TRIG_SGI_RST       0x0

/**
 * @brief Constructs a value for the TRIG_SGI register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigsgien - The value to use for the TRIG_SGI_EN field.
 * @param[in] trigsgi - The value to use for the TRIG_SGI field.
 */
__INLINE void macbyp_trig_sgi_pack(uint8_t trigsgien, uint8_t trigsgi)
{
    ASSERT_ERR((((uint32_t)trigsgien << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigsgi << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_SGI_ADDR,  ((uint32_t)trigsgien << 31) | ((uint32_t)trigsgi << 0));
}

/**
 * @brief Unpacks TRIG_SGI's fields from current value of the TRIG_SGI register.
 *
 * Reads the TRIG_SGI register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigsgien - Will be populated with the current value of this field from the register.
 * @param[out] trigsgi - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_sgi_unpack(uint8_t* trigsgien, uint8_t* trigsgi)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SGI_ADDR);

    *trigsgien = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigsgi = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_SGI_EN field in the TRIG_SGI register.
 *
 * The TRIG_SGI register will be read and the TRIG_SGI_EN field's value will be returned.
 *
 * @return The current value of the TRIG_SGI_EN field in the TRIG_SGI register.
 */
__INLINE uint8_t macbyp_trig_sgi_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SGI_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_SGI_EN field of the TRIG_SGI register.
 *
 * The TRIG_SGI register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigsgien - The value to set the field to.
 */
__INLINE void macbyp_trig_sgi_en_setf(uint8_t trigsgien)
{
    ASSERT_ERR((((uint32_t)trigsgien << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_SGI_ADDR, (REG_PL_RD(MACBYP_TRIG_SGI_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigsgien << 31));
}

/**
 * @brief Returns the current value of the TRIG_SGI field in the TRIG_SGI register.
 *
 * The TRIG_SGI register will be read and the TRIG_SGI field's value will be returned.
 *
 * @return The current value of the TRIG_SGI field in the TRIG_SGI register.
 */
__INLINE uint8_t macbyp_trig_sgi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SGI_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the TRIG_SGI field of the TRIG_SGI register.
 *
 * The TRIG_SGI register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigsgi - The value to set the field to.
 */
__INLINE void macbyp_trig_sgi_setf(uint8_t trigsgi)
{
    ASSERT_ERR((((uint32_t)trigsgi << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_SGI_ADDR, (REG_PL_RD(MACBYP_TRIG_SGI_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)trigsgi << 0));
}

/// @}

/**
 * @name TRIG_FEC register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31          TRIG_FEC_EN   0
 *     00             TRIG_FEC   0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_FEC register
#define MACBYP_TRIG_FEC_ADDR   (REG_MACBYPASS_BASE_ADDR+0x010C)
/// Offset of the TRIG_FEC register from the base address
#define MACBYP_TRIG_FEC_OFFSET 0x0000010C
/// Index of the TRIG_FEC register
#define MACBYP_TRIG_FEC_INDEX  0x00000043
/// Reset value of the TRIG_FEC register
#define MACBYP_TRIG_FEC_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_FEC register.
 * The TRIG_FEC register will be read and its value returned.
 * @return The current value of the TRIG_FEC register.
 */
__INLINE uint32_t macbyp_trig_fec_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_FEC_ADDR);
}

/**
 * @brief Sets the TRIG_FEC register to a value.
 * The TRIG_FEC register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_fec_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_FEC_ADDR, value);
}

// field definitions
/// TRIG_FEC_EN field bit
#define MACBYP_TRIG_FEC_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_FEC_EN field position
#define MACBYP_TRIG_FEC_EN_POS    31
/// TRIG_FEC field bit
#define MACBYP_TRIG_FEC_BIT       ((uint32_t)0x00000001)
/// TRIG_FEC field position
#define MACBYP_TRIG_FEC_POS       0

/// TRIG_FEC_EN field reset value
#define MACBYP_TRIG_FEC_EN_RST    0x0
/// TRIG_FEC field reset value
#define MACBYP_TRIG_FEC_RST       0x0

/**
 * @brief Constructs a value for the TRIG_FEC register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigfecen - The value to use for the TRIG_FEC_EN field.
 * @param[in] trigfec - The value to use for the TRIG_FEC field.
 */
__INLINE void macbyp_trig_fec_pack(uint8_t trigfecen, uint8_t trigfec)
{
    ASSERT_ERR((((uint32_t)trigfecen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigfec << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_FEC_ADDR,  ((uint32_t)trigfecen << 31) | ((uint32_t)trigfec << 0));
}

/**
 * @brief Unpacks TRIG_FEC's fields from current value of the TRIG_FEC register.
 *
 * Reads the TRIG_FEC register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigfecen - Will be populated with the current value of this field from the register.
 * @param[out] trigfec - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_fec_unpack(uint8_t* trigfecen, uint8_t* trigfec)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FEC_ADDR);

    *trigfecen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigfec = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_FEC_EN field in the TRIG_FEC register.
 *
 * The TRIG_FEC register will be read and the TRIG_FEC_EN field's value will be returned.
 *
 * @return The current value of the TRIG_FEC_EN field in the TRIG_FEC register.
 */
__INLINE uint8_t macbyp_trig_fec_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FEC_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_FEC_EN field of the TRIG_FEC register.
 *
 * The TRIG_FEC register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigfecen - The value to set the field to.
 */
__INLINE void macbyp_trig_fec_en_setf(uint8_t trigfecen)
{
    ASSERT_ERR((((uint32_t)trigfecen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_FEC_ADDR, (REG_PL_RD(MACBYP_TRIG_FEC_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigfecen << 31));
}

/**
 * @brief Returns the current value of the TRIG_FEC field in the TRIG_FEC register.
 *
 * The TRIG_FEC register will be read and the TRIG_FEC field's value will be returned.
 *
 * @return The current value of the TRIG_FEC field in the TRIG_FEC register.
 */
__INLINE uint8_t macbyp_trig_fec_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_FEC_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the TRIG_FEC field of the TRIG_FEC register.
 *
 * The TRIG_FEC register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigfec - The value to set the field to.
 */
__INLINE void macbyp_trig_fec_setf(uint8_t trigfec)
{
    ASSERT_ERR((((uint32_t)trigfec << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_FEC_ADDR, (REG_PL_RD(MACBYP_TRIG_FEC_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)trigfec << 0));
}

/// @}

/**
 * @name TRIG_STBC register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31         TRIG_STBC_EN   0
 *  01:00            TRIG_STBC   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_STBC register
#define MACBYP_TRIG_STBC_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0110)
/// Offset of the TRIG_STBC register from the base address
#define MACBYP_TRIG_STBC_OFFSET 0x00000110
/// Index of the TRIG_STBC register
#define MACBYP_TRIG_STBC_INDEX  0x00000044
/// Reset value of the TRIG_STBC register
#define MACBYP_TRIG_STBC_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_STBC register.
 * The TRIG_STBC register will be read and its value returned.
 * @return The current value of the TRIG_STBC register.
 */
__INLINE uint32_t macbyp_trig_stbc_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_STBC_ADDR);
}

/**
 * @brief Sets the TRIG_STBC register to a value.
 * The TRIG_STBC register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_stbc_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_STBC_ADDR, value);
}

// field definitions
/// TRIG_STBC_EN field bit
#define MACBYP_TRIG_STBC_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_STBC_EN field position
#define MACBYP_TRIG_STBC_EN_POS    31
/// TRIG_STBC field mask
#define MACBYP_TRIG_STBC_MASK      ((uint32_t)0x00000003)
/// TRIG_STBC field LSB position
#define MACBYP_TRIG_STBC_LSB       0
/// TRIG_STBC field width
#define MACBYP_TRIG_STBC_WIDTH     ((uint32_t)0x00000002)

/// TRIG_STBC_EN field reset value
#define MACBYP_TRIG_STBC_EN_RST    0x0
/// TRIG_STBC field reset value
#define MACBYP_TRIG_STBC_RST       0x0

/**
 * @brief Constructs a value for the TRIG_STBC register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigstbcen - The value to use for the TRIG_STBC_EN field.
 * @param[in] trigstbc - The value to use for the TRIG_STBC field.
 */
__INLINE void macbyp_trig_stbc_pack(uint8_t trigstbcen, uint8_t trigstbc)
{
    ASSERT_ERR((((uint32_t)trigstbcen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigstbc << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MACBYP_TRIG_STBC_ADDR,  ((uint32_t)trigstbcen << 31) | ((uint32_t)trigstbc << 0));
}

/**
 * @brief Unpacks TRIG_STBC's fields from current value of the TRIG_STBC register.
 *
 * Reads the TRIG_STBC register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigstbcen - Will be populated with the current value of this field from the register.
 * @param[out] trigstbc - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_stbc_unpack(uint8_t* trigstbcen, uint8_t* trigstbc)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_STBC_ADDR);

    *trigstbcen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigstbc = (localVal & ((uint32_t)0x00000003)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_STBC_EN field in the TRIG_STBC register.
 *
 * The TRIG_STBC register will be read and the TRIG_STBC_EN field's value will be returned.
 *
 * @return The current value of the TRIG_STBC_EN field in the TRIG_STBC register.
 */
__INLINE uint8_t macbyp_trig_stbc_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_STBC_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_STBC_EN field of the TRIG_STBC register.
 *
 * The TRIG_STBC register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigstbcen - The value to set the field to.
 */
__INLINE void macbyp_trig_stbc_en_setf(uint8_t trigstbcen)
{
    ASSERT_ERR((((uint32_t)trigstbcen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_STBC_ADDR, (REG_PL_RD(MACBYP_TRIG_STBC_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigstbcen << 31));
}

/**
 * @brief Returns the current value of the TRIG_STBC field in the TRIG_STBC register.
 *
 * The TRIG_STBC register will be read and the TRIG_STBC field's value will be returned.
 *
 * @return The current value of the TRIG_STBC field in the TRIG_STBC register.
 */
__INLINE uint8_t macbyp_trig_stbc_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_STBC_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

/**
 * @brief Sets the TRIG_STBC field of the TRIG_STBC register.
 *
 * The TRIG_STBC register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigstbc - The value to set the field to.
 */
__INLINE void macbyp_trig_stbc_setf(uint8_t trigstbc)
{
    ASSERT_ERR((((uint32_t)trigstbc << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MACBYP_TRIG_STBC_ADDR, (REG_PL_RD(MACBYP_TRIG_STBC_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)trigstbc << 0));
}

/// @}

/**
 * @name TRIG_CHBW register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31         TRIG_CHBW_EN   0
 *  01:00            TRIG_CHBW   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_CHBW register
#define MACBYP_TRIG_CHBW_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0114)
/// Offset of the TRIG_CHBW register from the base address
#define MACBYP_TRIG_CHBW_OFFSET 0x00000114
/// Index of the TRIG_CHBW register
#define MACBYP_TRIG_CHBW_INDEX  0x00000045
/// Reset value of the TRIG_CHBW register
#define MACBYP_TRIG_CHBW_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_CHBW register.
 * The TRIG_CHBW register will be read and its value returned.
 * @return The current value of the TRIG_CHBW register.
 */
__INLINE uint32_t macbyp_trig_chbw_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_CHBW_ADDR);
}

/**
 * @brief Sets the TRIG_CHBW register to a value.
 * The TRIG_CHBW register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_chbw_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_CHBW_ADDR, value);
}

// field definitions
/// TRIG_CHBW_EN field bit
#define MACBYP_TRIG_CHBW_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_CHBW_EN field position
#define MACBYP_TRIG_CHBW_EN_POS    31
/// TRIG_CHBW field mask
#define MACBYP_TRIG_CHBW_MASK      ((uint32_t)0x00000003)
/// TRIG_CHBW field LSB position
#define MACBYP_TRIG_CHBW_LSB       0
/// TRIG_CHBW field width
#define MACBYP_TRIG_CHBW_WIDTH     ((uint32_t)0x00000002)

/// TRIG_CHBW_EN field reset value
#define MACBYP_TRIG_CHBW_EN_RST    0x0
/// TRIG_CHBW field reset value
#define MACBYP_TRIG_CHBW_RST       0x0

/**
 * @brief Constructs a value for the TRIG_CHBW register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigchbwen - The value to use for the TRIG_CHBW_EN field.
 * @param[in] trigchbw - The value to use for the TRIG_CHBW field.
 */
__INLINE void macbyp_trig_chbw_pack(uint8_t trigchbwen, uint8_t trigchbw)
{
    ASSERT_ERR((((uint32_t)trigchbwen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigchbw << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MACBYP_TRIG_CHBW_ADDR,  ((uint32_t)trigchbwen << 31) | ((uint32_t)trigchbw << 0));
}

/**
 * @brief Unpacks TRIG_CHBW's fields from current value of the TRIG_CHBW register.
 *
 * Reads the TRIG_CHBW register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigchbwen - Will be populated with the current value of this field from the register.
 * @param[out] trigchbw - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_chbw_unpack(uint8_t* trigchbwen, uint8_t* trigchbw)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_CHBW_ADDR);

    *trigchbwen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigchbw = (localVal & ((uint32_t)0x00000003)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_CHBW_EN field in the TRIG_CHBW register.
 *
 * The TRIG_CHBW register will be read and the TRIG_CHBW_EN field's value will be returned.
 *
 * @return The current value of the TRIG_CHBW_EN field in the TRIG_CHBW register.
 */
__INLINE uint8_t macbyp_trig_chbw_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_CHBW_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_CHBW_EN field of the TRIG_CHBW register.
 *
 * The TRIG_CHBW register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigchbwen - The value to set the field to.
 */
__INLINE void macbyp_trig_chbw_en_setf(uint8_t trigchbwen)
{
    ASSERT_ERR((((uint32_t)trigchbwen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_CHBW_ADDR, (REG_PL_RD(MACBYP_TRIG_CHBW_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigchbwen << 31));
}

/**
 * @brief Returns the current value of the TRIG_CHBW field in the TRIG_CHBW register.
 *
 * The TRIG_CHBW register will be read and the TRIG_CHBW field's value will be returned.
 *
 * @return The current value of the TRIG_CHBW field in the TRIG_CHBW register.
 */
__INLINE uint8_t macbyp_trig_chbw_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_CHBW_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

/**
 * @brief Sets the TRIG_CHBW field of the TRIG_CHBW register.
 *
 * The TRIG_CHBW register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigchbw - The value to set the field to.
 */
__INLINE void macbyp_trig_chbw_setf(uint8_t trigchbw)
{
    ASSERT_ERR((((uint32_t)trigchbw << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MACBYP_TRIG_CHBW_ADDR, (REG_PL_RD(MACBYP_TRIG_CHBW_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)trigchbw << 0));
}

/// @}

/**
 * @name TRIG_NSTS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31         TRIG_NSTS_EN   0
 *  02:00            TRIG_NSTS   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_NSTS register
#define MACBYP_TRIG_NSTS_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0118)
/// Offset of the TRIG_NSTS register from the base address
#define MACBYP_TRIG_NSTS_OFFSET 0x00000118
/// Index of the TRIG_NSTS register
#define MACBYP_TRIG_NSTS_INDEX  0x00000046
/// Reset value of the TRIG_NSTS register
#define MACBYP_TRIG_NSTS_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_NSTS register.
 * The TRIG_NSTS register will be read and its value returned.
 * @return The current value of the TRIG_NSTS register.
 */
__INLINE uint32_t macbyp_trig_nsts_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_NSTS_ADDR);
}

/**
 * @brief Sets the TRIG_NSTS register to a value.
 * The TRIG_NSTS register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_nsts_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_NSTS_ADDR, value);
}

// field definitions
/// TRIG_NSTS_EN field bit
#define MACBYP_TRIG_NSTS_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_NSTS_EN field position
#define MACBYP_TRIG_NSTS_EN_POS    31
/// TRIG_NSTS field mask
#define MACBYP_TRIG_NSTS_MASK      ((uint32_t)0x00000007)
/// TRIG_NSTS field LSB position
#define MACBYP_TRIG_NSTS_LSB       0
/// TRIG_NSTS field width
#define MACBYP_TRIG_NSTS_WIDTH     ((uint32_t)0x00000003)

/// TRIG_NSTS_EN field reset value
#define MACBYP_TRIG_NSTS_EN_RST    0x0
/// TRIG_NSTS field reset value
#define MACBYP_TRIG_NSTS_RST       0x0

/**
 * @brief Constructs a value for the TRIG_NSTS register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trignstsen - The value to use for the TRIG_NSTS_EN field.
 * @param[in] trignsts - The value to use for the TRIG_NSTS field.
 */
__INLINE void macbyp_trig_nsts_pack(uint8_t trignstsen, uint8_t trignsts)
{
    ASSERT_ERR((((uint32_t)trignstsen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trignsts << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(MACBYP_TRIG_NSTS_ADDR,  ((uint32_t)trignstsen << 31) | ((uint32_t)trignsts << 0));
}

/**
 * @brief Unpacks TRIG_NSTS's fields from current value of the TRIG_NSTS register.
 *
 * Reads the TRIG_NSTS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trignstsen - Will be populated with the current value of this field from the register.
 * @param[out] trignsts - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_nsts_unpack(uint8_t* trignstsen, uint8_t* trignsts)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_NSTS_ADDR);

    *trignstsen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trignsts = (localVal & ((uint32_t)0x00000007)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_NSTS_EN field in the TRIG_NSTS register.
 *
 * The TRIG_NSTS register will be read and the TRIG_NSTS_EN field's value will be returned.
 *
 * @return The current value of the TRIG_NSTS_EN field in the TRIG_NSTS register.
 */
__INLINE uint8_t macbyp_trig_nsts_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_NSTS_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_NSTS_EN field of the TRIG_NSTS register.
 *
 * The TRIG_NSTS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trignstsen - The value to set the field to.
 */
__INLINE void macbyp_trig_nsts_en_setf(uint8_t trignstsen)
{
    ASSERT_ERR((((uint32_t)trignstsen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_NSTS_ADDR, (REG_PL_RD(MACBYP_TRIG_NSTS_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trignstsen << 31));
}

/**
 * @brief Returns the current value of the TRIG_NSTS field in the TRIG_NSTS register.
 *
 * The TRIG_NSTS register will be read and the TRIG_NSTS field's value will be returned.
 *
 * @return The current value of the TRIG_NSTS field in the TRIG_NSTS register.
 */
__INLINE uint8_t macbyp_trig_nsts_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_NSTS_ADDR);
    return ((localVal & ((uint32_t)0x00000007)) >> 0);
}

/**
 * @brief Sets the TRIG_NSTS field of the TRIG_NSTS register.
 *
 * The TRIG_NSTS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trignsts - The value to set the field to.
 */
__INLINE void macbyp_trig_nsts_setf(uint8_t trignsts)
{
    ASSERT_ERR((((uint32_t)trignsts << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(MACBYP_TRIG_NSTS_ADDR, (REG_PL_RD(MACBYP_TRIG_NSTS_ADDR) & ~((uint32_t)0x00000007)) | ((uint32_t)trignsts << 0));
}

/// @}

/**
 * @name TRIG_SOUNDING register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31     TRIG_SOUNDING_EN   0
 *     00        TRIG_SOUNDING   0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_SOUNDING register
#define MACBYP_TRIG_SOUNDING_ADDR   (REG_MACBYPASS_BASE_ADDR+0x011C)
/// Offset of the TRIG_SOUNDING register from the base address
#define MACBYP_TRIG_SOUNDING_OFFSET 0x0000011C
/// Index of the TRIG_SOUNDING register
#define MACBYP_TRIG_SOUNDING_INDEX  0x00000047
/// Reset value of the TRIG_SOUNDING register
#define MACBYP_TRIG_SOUNDING_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_SOUNDING register.
 * The TRIG_SOUNDING register will be read and its value returned.
 * @return The current value of the TRIG_SOUNDING register.
 */
__INLINE uint32_t macbyp_trig_sounding_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR);
}

/**
 * @brief Sets the TRIG_SOUNDING register to a value.
 * The TRIG_SOUNDING register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_sounding_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_SOUNDING_ADDR, value);
}

// field definitions
/// TRIG_SOUNDING_EN field bit
#define MACBYP_TRIG_SOUNDING_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_SOUNDING_EN field position
#define MACBYP_TRIG_SOUNDING_EN_POS    31
/// TRIG_SOUNDING field bit
#define MACBYP_TRIG_SOUNDING_BIT       ((uint32_t)0x00000001)
/// TRIG_SOUNDING field position
#define MACBYP_TRIG_SOUNDING_POS       0

/// TRIG_SOUNDING_EN field reset value
#define MACBYP_TRIG_SOUNDING_EN_RST    0x0
/// TRIG_SOUNDING field reset value
#define MACBYP_TRIG_SOUNDING_RST       0x0

/**
 * @brief Constructs a value for the TRIG_SOUNDING register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigsoundingen - The value to use for the TRIG_SOUNDING_EN field.
 * @param[in] trigsounding - The value to use for the TRIG_SOUNDING field.
 */
__INLINE void macbyp_trig_sounding_pack(uint8_t trigsoundingen, uint8_t trigsounding)
{
    ASSERT_ERR((((uint32_t)trigsoundingen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigsounding << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_SOUNDING_ADDR,  ((uint32_t)trigsoundingen << 31) | ((uint32_t)trigsounding << 0));
}

/**
 * @brief Unpacks TRIG_SOUNDING's fields from current value of the TRIG_SOUNDING register.
 *
 * Reads the TRIG_SOUNDING register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigsoundingen - Will be populated with the current value of this field from the register.
 * @param[out] trigsounding - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_sounding_unpack(uint8_t* trigsoundingen, uint8_t* trigsounding)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR);

    *trigsoundingen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigsounding = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_SOUNDING_EN field in the TRIG_SOUNDING register.
 *
 * The TRIG_SOUNDING register will be read and the TRIG_SOUNDING_EN field's value will be returned.
 *
 * @return The current value of the TRIG_SOUNDING_EN field in the TRIG_SOUNDING register.
 */
__INLINE uint8_t macbyp_trig_sounding_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_SOUNDING_EN field of the TRIG_SOUNDING register.
 *
 * The TRIG_SOUNDING register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigsoundingen - The value to set the field to.
 */
__INLINE void macbyp_trig_sounding_en_setf(uint8_t trigsoundingen)
{
    ASSERT_ERR((((uint32_t)trigsoundingen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_SOUNDING_ADDR, (REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigsoundingen << 31));
}

/**
 * @brief Returns the current value of the TRIG_SOUNDING field in the TRIG_SOUNDING register.
 *
 * The TRIG_SOUNDING register will be read and the TRIG_SOUNDING field's value will be returned.
 *
 * @return The current value of the TRIG_SOUNDING field in the TRIG_SOUNDING register.
 */
__INLINE uint8_t macbyp_trig_sounding_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the TRIG_SOUNDING field of the TRIG_SOUNDING register.
 *
 * The TRIG_SOUNDING register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigsounding - The value to set the field to.
 */
__INLINE void macbyp_trig_sounding_setf(uint8_t trigsounding)
{
    ASSERT_ERR((((uint32_t)trigsounding << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MACBYP_TRIG_SOUNDING_ADDR, (REG_PL_RD(MACBYP_TRIG_SOUNDING_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)trigsounding << 0));
}

/// @}

/**
 * @name TRIG_PARTIALAID register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31   TRIG_PARTIALAID_EN   0
 *  08:00      TRIG_PARTIALAID   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_PARTIALAID register
#define MACBYP_TRIG_PARTIALAID_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0120)
/// Offset of the TRIG_PARTIALAID register from the base address
#define MACBYP_TRIG_PARTIALAID_OFFSET 0x00000120
/// Index of the TRIG_PARTIALAID register
#define MACBYP_TRIG_PARTIALAID_INDEX  0x00000048
/// Reset value of the TRIG_PARTIALAID register
#define MACBYP_TRIG_PARTIALAID_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_PARTIALAID register.
 * The TRIG_PARTIALAID register will be read and its value returned.
 * @return The current value of the TRIG_PARTIALAID register.
 */
__INLINE uint32_t macbyp_trig_partialaid_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR);
}

/**
 * @brief Sets the TRIG_PARTIALAID register to a value.
 * The TRIG_PARTIALAID register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_partialaid_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_PARTIALAID_ADDR, value);
}

// field definitions
/// TRIG_PARTIALAID_EN field bit
#define MACBYP_TRIG_PARTIALAID_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_PARTIALAID_EN field position
#define MACBYP_TRIG_PARTIALAID_EN_POS    31
/// TRIG_PARTIALAID field mask
#define MACBYP_TRIG_PARTIALAID_MASK      ((uint32_t)0x000001FF)
/// TRIG_PARTIALAID field LSB position
#define MACBYP_TRIG_PARTIALAID_LSB       0
/// TRIG_PARTIALAID field width
#define MACBYP_TRIG_PARTIALAID_WIDTH     ((uint32_t)0x00000009)

/// TRIG_PARTIALAID_EN field reset value
#define MACBYP_TRIG_PARTIALAID_EN_RST    0x0
/// TRIG_PARTIALAID field reset value
#define MACBYP_TRIG_PARTIALAID_RST       0x0

/**
 * @brief Constructs a value for the TRIG_PARTIALAID register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trigpartialaiden - The value to use for the TRIG_PARTIALAID_EN field.
 * @param[in] trigpartialaid - The value to use for the TRIG_PARTIALAID field.
 */
__INLINE void macbyp_trig_partialaid_pack(uint8_t trigpartialaiden, uint16_t trigpartialaid)
{
    ASSERT_ERR((((uint32_t)trigpartialaiden << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)trigpartialaid << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MACBYP_TRIG_PARTIALAID_ADDR,  ((uint32_t)trigpartialaiden << 31) | ((uint32_t)trigpartialaid << 0));
}

/**
 * @brief Unpacks TRIG_PARTIALAID's fields from current value of the TRIG_PARTIALAID register.
 *
 * Reads the TRIG_PARTIALAID register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trigpartialaiden - Will be populated with the current value of this field from the register.
 * @param[out] trigpartialaid - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_partialaid_unpack(uint8_t* trigpartialaiden, uint16_t* trigpartialaid)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR);

    *trigpartialaiden = (localVal & ((uint32_t)0x80000000)) >> 31;
    *trigpartialaid = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_PARTIALAID_EN field in the TRIG_PARTIALAID register.
 *
 * The TRIG_PARTIALAID register will be read and the TRIG_PARTIALAID_EN field's value will be returned.
 *
 * @return The current value of the TRIG_PARTIALAID_EN field in the TRIG_PARTIALAID register.
 */
__INLINE uint8_t macbyp_trig_partialaid_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_PARTIALAID_EN field of the TRIG_PARTIALAID register.
 *
 * The TRIG_PARTIALAID register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigpartialaiden - The value to set the field to.
 */
__INLINE void macbyp_trig_partialaid_en_setf(uint8_t trigpartialaiden)
{
    ASSERT_ERR((((uint32_t)trigpartialaiden << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_PARTIALAID_ADDR, (REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)trigpartialaiden << 31));
}

/**
 * @brief Returns the current value of the TRIG_PARTIALAID field in the TRIG_PARTIALAID register.
 *
 * The TRIG_PARTIALAID register will be read and the TRIG_PARTIALAID field's value will be returned.
 *
 * @return The current value of the TRIG_PARTIALAID field in the TRIG_PARTIALAID register.
 */
__INLINE uint16_t macbyp_trig_partialaid_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the TRIG_PARTIALAID field of the TRIG_PARTIALAID register.
 *
 * The TRIG_PARTIALAID register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trigpartialaid - The value to set the field to.
 */
__INLINE void macbyp_trig_partialaid_setf(uint16_t trigpartialaid)
{
    ASSERT_ERR((((uint32_t)trigpartialaid << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MACBYP_TRIG_PARTIALAID_ADDR, (REG_PL_RD(MACBYP_TRIG_PARTIALAID_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)trigpartialaid << 0));
}

/// @}

/**
 * @name TRIG_GROUPID register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31      TRIG_GROUPID_EN   0
 *  05:00         TRIG_GROUPID   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TRIG_GROUPID register
#define MACBYP_TRIG_GROUPID_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0124)
/// Offset of the TRIG_GROUPID register from the base address
#define MACBYP_TRIG_GROUPID_OFFSET 0x00000124
/// Index of the TRIG_GROUPID register
#define MACBYP_TRIG_GROUPID_INDEX  0x00000049
/// Reset value of the TRIG_GROUPID register
#define MACBYP_TRIG_GROUPID_RESET  0x00000000

/**
 * @brief Returns the current value of the TRIG_GROUPID register.
 * The TRIG_GROUPID register will be read and its value returned.
 * @return The current value of the TRIG_GROUPID register.
 */
__INLINE uint32_t macbyp_trig_groupid_get(void)
{
    return REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR);
}

/**
 * @brief Sets the TRIG_GROUPID register to a value.
 * The TRIG_GROUPID register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_trig_groupid_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TRIG_GROUPID_ADDR, value);
}

// field definitions
/// TRIG_GROUPID_EN field bit
#define MACBYP_TRIG_GROUPID_EN_BIT    ((uint32_t)0x80000000)
/// TRIG_GROUPID_EN field position
#define MACBYP_TRIG_GROUPID_EN_POS    31
/// TRIG_GROUPID field mask
#define MACBYP_TRIG_GROUPID_MASK      ((uint32_t)0x0000003F)
/// TRIG_GROUPID field LSB position
#define MACBYP_TRIG_GROUPID_LSB       0
/// TRIG_GROUPID field width
#define MACBYP_TRIG_GROUPID_WIDTH     ((uint32_t)0x00000006)

/// TRIG_GROUPID_EN field reset value
#define MACBYP_TRIG_GROUPID_EN_RST    0x0
/// TRIG_GROUPID field reset value
#define MACBYP_TRIG_GROUPID_RST       0x0

/**
 * @brief Constructs a value for the TRIG_GROUPID register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] triggroupiden - The value to use for the TRIG_GROUPID_EN field.
 * @param[in] triggroupid - The value to use for the TRIG_GROUPID field.
 */
__INLINE void macbyp_trig_groupid_pack(uint8_t triggroupiden, uint8_t triggroupid)
{
    ASSERT_ERR((((uint32_t)triggroupiden << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)triggroupid << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MACBYP_TRIG_GROUPID_ADDR,  ((uint32_t)triggroupiden << 31) | ((uint32_t)triggroupid << 0));
}

/**
 * @brief Unpacks TRIG_GROUPID's fields from current value of the TRIG_GROUPID register.
 *
 * Reads the TRIG_GROUPID register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] triggroupiden - Will be populated with the current value of this field from the register.
 * @param[out] triggroupid - Will be populated with the current value of this field from the register.
 */
__INLINE void macbyp_trig_groupid_unpack(uint8_t* triggroupiden, uint8_t* triggroupid)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR);

    *triggroupiden = (localVal & ((uint32_t)0x80000000)) >> 31;
    *triggroupid = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the TRIG_GROUPID_EN field in the TRIG_GROUPID register.
 *
 * The TRIG_GROUPID register will be read and the TRIG_GROUPID_EN field's value will be returned.
 *
 * @return The current value of the TRIG_GROUPID_EN field in the TRIG_GROUPID register.
 */
__INLINE uint8_t macbyp_trig_groupid_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TRIG_GROUPID_EN field of the TRIG_GROUPID register.
 *
 * The TRIG_GROUPID register will be read, modified to contain the new field value, and written.
 *
 * @param[in] triggroupiden - The value to set the field to.
 */
__INLINE void macbyp_trig_groupid_en_setf(uint8_t triggroupiden)
{
    ASSERT_ERR((((uint32_t)triggroupiden << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MACBYP_TRIG_GROUPID_ADDR, (REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)triggroupiden << 31));
}

/**
 * @brief Returns the current value of the TRIG_GROUPID field in the TRIG_GROUPID register.
 *
 * The TRIG_GROUPID register will be read and the TRIG_GROUPID field's value will be returned.
 *
 * @return The current value of the TRIG_GROUPID field in the TRIG_GROUPID register.
 */
__INLINE uint8_t macbyp_trig_groupid_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the TRIG_GROUPID field of the TRIG_GROUPID register.
 *
 * The TRIG_GROUPID register will be read, modified to contain the new field value, and written.
 *
 * @param[in] triggroupid - The value to set the field to.
 */
__INLINE void macbyp_trig_groupid_setf(uint8_t triggroupid)
{
    ASSERT_ERR((((uint32_t)triggroupid << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MACBYP_TRIG_GROUPID_ADDR, (REG_PL_RD(MACBYP_TRIG_GROUPID_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)triggroupid << 0));
}

/// @}

/**
 * @name TXV0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV0 register
#define MACBYP_TXV0_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0200)
/// Offset of the TXV0 register from the base address
#define MACBYP_TXV0_OFFSET 0x00000200
/// Index of the TXV0 register
#define MACBYP_TXV0_INDEX  0x00000080
/// Reset value of the TXV0 register
#define MACBYP_TXV0_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV0 register.
 * The TXV0 register will be read and its value returned.
 * @return The current value of the TXV0 register.
 */
__INLINE uint32_t macbyp_txv0_get(void)
{
    return REG_PL_RD(MACBYP_TXV0_ADDR);
}

/**
 * @brief Sets the TXV0 register to a value.
 * The TXV0 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv0_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV0_ADDR, value);
}

// field definitions
/// TXV0 field mask
#define MACBYP_TXV0_MASK   ((uint32_t)0x000000FF)
/// TXV0 field LSB position
#define MACBYP_TXV0_LSB    0
/// TXV0 field width
#define MACBYP_TXV0_WIDTH  ((uint32_t)0x00000008)

/// TXV0 field reset value
#define MACBYP_TXV0_RST    0x0

/**
 * @brief Returns the current value of the TXV0 field in the TXV0 register.
 *
 * The TXV0 register will be read and the TXV0 field's value will be returned.
 *
 * @return The current value of the TXV0 field in the TXV0 register.
 */
__INLINE uint8_t macbyp_txv0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV0_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV0 field of the TXV0 register.
 *
 * The TXV0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv0 - The value to set the field to.
 */
__INLINE void macbyp_txv0_setf(uint8_t txv0)
{
    ASSERT_ERR((((uint32_t)txv0 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV0_ADDR, (uint32_t)txv0 << 0);
}

/// @}

/**
 * @name TXV1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV1   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV1 register
#define MACBYP_TXV1_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0204)
/// Offset of the TXV1 register from the base address
#define MACBYP_TXV1_OFFSET 0x00000204
/// Index of the TXV1 register
#define MACBYP_TXV1_INDEX  0x00000081
/// Reset value of the TXV1 register
#define MACBYP_TXV1_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV1 register.
 * The TXV1 register will be read and its value returned.
 * @return The current value of the TXV1 register.
 */
__INLINE uint32_t macbyp_txv1_get(void)
{
    return REG_PL_RD(MACBYP_TXV1_ADDR);
}

/**
 * @brief Sets the TXV1 register to a value.
 * The TXV1 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv1_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV1_ADDR, value);
}

// field definitions
/// TXV1 field mask
#define MACBYP_TXV1_MASK   ((uint32_t)0x000000FF)
/// TXV1 field LSB position
#define MACBYP_TXV1_LSB    0
/// TXV1 field width
#define MACBYP_TXV1_WIDTH  ((uint32_t)0x00000008)

/// TXV1 field reset value
#define MACBYP_TXV1_RST    0x0

/**
 * @brief Returns the current value of the TXV1 field in the TXV1 register.
 *
 * The TXV1 register will be read and the TXV1 field's value will be returned.
 *
 * @return The current value of the TXV1 field in the TXV1 register.
 */
__INLINE uint8_t macbyp_txv1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV1 field of the TXV1 register.
 *
 * The TXV1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv1 - The value to set the field to.
 */
__INLINE void macbyp_txv1_setf(uint8_t txv1)
{
    ASSERT_ERR((((uint32_t)txv1 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV1_ADDR, (uint32_t)txv1 << 0);
}

/// @}

/**
 * @name TXV2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV2   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV2 register
#define MACBYP_TXV2_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0208)
/// Offset of the TXV2 register from the base address
#define MACBYP_TXV2_OFFSET 0x00000208
/// Index of the TXV2 register
#define MACBYP_TXV2_INDEX  0x00000082
/// Reset value of the TXV2 register
#define MACBYP_TXV2_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV2 register.
 * The TXV2 register will be read and its value returned.
 * @return The current value of the TXV2 register.
 */
__INLINE uint32_t macbyp_txv2_get(void)
{
    return REG_PL_RD(MACBYP_TXV2_ADDR);
}

/**
 * @brief Sets the TXV2 register to a value.
 * The TXV2 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv2_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV2_ADDR, value);
}

// field definitions
/// TXV2 field mask
#define MACBYP_TXV2_MASK   ((uint32_t)0x000000FF)
/// TXV2 field LSB position
#define MACBYP_TXV2_LSB    0
/// TXV2 field width
#define MACBYP_TXV2_WIDTH  ((uint32_t)0x00000008)

/// TXV2 field reset value
#define MACBYP_TXV2_RST    0x0

/**
 * @brief Returns the current value of the TXV2 field in the TXV2 register.
 *
 * The TXV2 register will be read and the TXV2 field's value will be returned.
 *
 * @return The current value of the TXV2 field in the TXV2 register.
 */
__INLINE uint8_t macbyp_txv2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV2_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV2 field of the TXV2 register.
 *
 * The TXV2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv2 - The value to set the field to.
 */
__INLINE void macbyp_txv2_setf(uint8_t txv2)
{
    ASSERT_ERR((((uint32_t)txv2 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV2_ADDR, (uint32_t)txv2 << 0);
}

/// @}

/**
 * @name TXV3 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV3   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV3 register
#define MACBYP_TXV3_ADDR   (REG_MACBYPASS_BASE_ADDR+0x020C)
/// Offset of the TXV3 register from the base address
#define MACBYP_TXV3_OFFSET 0x0000020C
/// Index of the TXV3 register
#define MACBYP_TXV3_INDEX  0x00000083
/// Reset value of the TXV3 register
#define MACBYP_TXV3_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV3 register.
 * The TXV3 register will be read and its value returned.
 * @return The current value of the TXV3 register.
 */
__INLINE uint32_t macbyp_txv3_get(void)
{
    return REG_PL_RD(MACBYP_TXV3_ADDR);
}

/**
 * @brief Sets the TXV3 register to a value.
 * The TXV3 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv3_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV3_ADDR, value);
}

// field definitions
/// TXV3 field mask
#define MACBYP_TXV3_MASK   ((uint32_t)0x000000FF)
/// TXV3 field LSB position
#define MACBYP_TXV3_LSB    0
/// TXV3 field width
#define MACBYP_TXV3_WIDTH  ((uint32_t)0x00000008)

/// TXV3 field reset value
#define MACBYP_TXV3_RST    0x0

/**
 * @brief Returns the current value of the TXV3 field in the TXV3 register.
 *
 * The TXV3 register will be read and the TXV3 field's value will be returned.
 *
 * @return The current value of the TXV3 field in the TXV3 register.
 */
__INLINE uint8_t macbyp_txv3_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV3_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV3 field of the TXV3 register.
 *
 * The TXV3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv3 - The value to set the field to.
 */
__INLINE void macbyp_txv3_setf(uint8_t txv3)
{
    ASSERT_ERR((((uint32_t)txv3 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV3_ADDR, (uint32_t)txv3 << 0);
}

/// @}

/**
 * @name TXV4 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV4   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV4 register
#define MACBYP_TXV4_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0210)
/// Offset of the TXV4 register from the base address
#define MACBYP_TXV4_OFFSET 0x00000210
/// Index of the TXV4 register
#define MACBYP_TXV4_INDEX  0x00000084
/// Reset value of the TXV4 register
#define MACBYP_TXV4_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV4 register.
 * The TXV4 register will be read and its value returned.
 * @return The current value of the TXV4 register.
 */
__INLINE uint32_t macbyp_txv4_get(void)
{
    return REG_PL_RD(MACBYP_TXV4_ADDR);
}

/**
 * @brief Sets the TXV4 register to a value.
 * The TXV4 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv4_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV4_ADDR, value);
}

// field definitions
/// TXV4 field mask
#define MACBYP_TXV4_MASK   ((uint32_t)0x000000FF)
/// TXV4 field LSB position
#define MACBYP_TXV4_LSB    0
/// TXV4 field width
#define MACBYP_TXV4_WIDTH  ((uint32_t)0x00000008)

/// TXV4 field reset value
#define MACBYP_TXV4_RST    0x0

/**
 * @brief Returns the current value of the TXV4 field in the TXV4 register.
 *
 * The TXV4 register will be read and the TXV4 field's value will be returned.
 *
 * @return The current value of the TXV4 field in the TXV4 register.
 */
__INLINE uint8_t macbyp_txv4_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV4_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV4 field of the TXV4 register.
 *
 * The TXV4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv4 - The value to set the field to.
 */
__INLINE void macbyp_txv4_setf(uint8_t txv4)
{
    ASSERT_ERR((((uint32_t)txv4 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV4_ADDR, (uint32_t)txv4 << 0);
}

/// @}

/**
 * @name TXV5 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV5   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV5 register
#define MACBYP_TXV5_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0214)
/// Offset of the TXV5 register from the base address
#define MACBYP_TXV5_OFFSET 0x00000214
/// Index of the TXV5 register
#define MACBYP_TXV5_INDEX  0x00000085
/// Reset value of the TXV5 register
#define MACBYP_TXV5_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV5 register.
 * The TXV5 register will be read and its value returned.
 * @return The current value of the TXV5 register.
 */
__INLINE uint32_t macbyp_txv5_get(void)
{
    return REG_PL_RD(MACBYP_TXV5_ADDR);
}

/**
 * @brief Sets the TXV5 register to a value.
 * The TXV5 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv5_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV5_ADDR, value);
}

// field definitions
/// TXV5 field mask
#define MACBYP_TXV5_MASK   ((uint32_t)0x000000FF)
/// TXV5 field LSB position
#define MACBYP_TXV5_LSB    0
/// TXV5 field width
#define MACBYP_TXV5_WIDTH  ((uint32_t)0x00000008)

/// TXV5 field reset value
#define MACBYP_TXV5_RST    0x0

/**
 * @brief Returns the current value of the TXV5 field in the TXV5 register.
 *
 * The TXV5 register will be read and the TXV5 field's value will be returned.
 *
 * @return The current value of the TXV5 field in the TXV5 register.
 */
__INLINE uint8_t macbyp_txv5_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV5_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV5 field of the TXV5 register.
 *
 * The TXV5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv5 - The value to set the field to.
 */
__INLINE void macbyp_txv5_setf(uint8_t txv5)
{
    ASSERT_ERR((((uint32_t)txv5 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV5_ADDR, (uint32_t)txv5 << 0);
}

/// @}

/**
 * @name TXV6 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV6   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV6 register
#define MACBYP_TXV6_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0218)
/// Offset of the TXV6 register from the base address
#define MACBYP_TXV6_OFFSET 0x00000218
/// Index of the TXV6 register
#define MACBYP_TXV6_INDEX  0x00000086
/// Reset value of the TXV6 register
#define MACBYP_TXV6_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV6 register.
 * The TXV6 register will be read and its value returned.
 * @return The current value of the TXV6 register.
 */
__INLINE uint32_t macbyp_txv6_get(void)
{
    return REG_PL_RD(MACBYP_TXV6_ADDR);
}

/**
 * @brief Sets the TXV6 register to a value.
 * The TXV6 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv6_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV6_ADDR, value);
}

// field definitions
/// TXV6 field mask
#define MACBYP_TXV6_MASK   ((uint32_t)0x000000FF)
/// TXV6 field LSB position
#define MACBYP_TXV6_LSB    0
/// TXV6 field width
#define MACBYP_TXV6_WIDTH  ((uint32_t)0x00000008)

/// TXV6 field reset value
#define MACBYP_TXV6_RST    0x0

/**
 * @brief Returns the current value of the TXV6 field in the TXV6 register.
 *
 * The TXV6 register will be read and the TXV6 field's value will be returned.
 *
 * @return The current value of the TXV6 field in the TXV6 register.
 */
__INLINE uint8_t macbyp_txv6_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV6_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV6 field of the TXV6 register.
 *
 * The TXV6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv6 - The value to set the field to.
 */
__INLINE void macbyp_txv6_setf(uint8_t txv6)
{
    ASSERT_ERR((((uint32_t)txv6 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV6_ADDR, (uint32_t)txv6 << 0);
}

/// @}

/**
 * @name TXV7 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV7   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV7 register
#define MACBYP_TXV7_ADDR   (REG_MACBYPASS_BASE_ADDR+0x021C)
/// Offset of the TXV7 register from the base address
#define MACBYP_TXV7_OFFSET 0x0000021C
/// Index of the TXV7 register
#define MACBYP_TXV7_INDEX  0x00000087
/// Reset value of the TXV7 register
#define MACBYP_TXV7_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV7 register.
 * The TXV7 register will be read and its value returned.
 * @return The current value of the TXV7 register.
 */
__INLINE uint32_t macbyp_txv7_get(void)
{
    return REG_PL_RD(MACBYP_TXV7_ADDR);
}

/**
 * @brief Sets the TXV7 register to a value.
 * The TXV7 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv7_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV7_ADDR, value);
}

// field definitions
/// TXV7 field mask
#define MACBYP_TXV7_MASK   ((uint32_t)0x000000FF)
/// TXV7 field LSB position
#define MACBYP_TXV7_LSB    0
/// TXV7 field width
#define MACBYP_TXV7_WIDTH  ((uint32_t)0x00000008)

/// TXV7 field reset value
#define MACBYP_TXV7_RST    0x0

/**
 * @brief Returns the current value of the TXV7 field in the TXV7 register.
 *
 * The TXV7 register will be read and the TXV7 field's value will be returned.
 *
 * @return The current value of the TXV7 field in the TXV7 register.
 */
__INLINE uint8_t macbyp_txv7_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV7_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV7 field of the TXV7 register.
 *
 * The TXV7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv7 - The value to set the field to.
 */
__INLINE void macbyp_txv7_setf(uint8_t txv7)
{
    ASSERT_ERR((((uint32_t)txv7 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV7_ADDR, (uint32_t)txv7 << 0);
}

/// @}

/**
 * @name TXV8 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV8   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV8 register
#define MACBYP_TXV8_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0220)
/// Offset of the TXV8 register from the base address
#define MACBYP_TXV8_OFFSET 0x00000220
/// Index of the TXV8 register
#define MACBYP_TXV8_INDEX  0x00000088
/// Reset value of the TXV8 register
#define MACBYP_TXV8_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV8 register.
 * The TXV8 register will be read and its value returned.
 * @return The current value of the TXV8 register.
 */
__INLINE uint32_t macbyp_txv8_get(void)
{
    return REG_PL_RD(MACBYP_TXV8_ADDR);
}

/**
 * @brief Sets the TXV8 register to a value.
 * The TXV8 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv8_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV8_ADDR, value);
}

// field definitions
/// TXV8 field mask
#define MACBYP_TXV8_MASK   ((uint32_t)0x000000FF)
/// TXV8 field LSB position
#define MACBYP_TXV8_LSB    0
/// TXV8 field width
#define MACBYP_TXV8_WIDTH  ((uint32_t)0x00000008)

/// TXV8 field reset value
#define MACBYP_TXV8_RST    0x0

/**
 * @brief Returns the current value of the TXV8 field in the TXV8 register.
 *
 * The TXV8 register will be read and the TXV8 field's value will be returned.
 *
 * @return The current value of the TXV8 field in the TXV8 register.
 */
__INLINE uint8_t macbyp_txv8_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV8_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV8 field of the TXV8 register.
 *
 * The TXV8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv8 - The value to set the field to.
 */
__INLINE void macbyp_txv8_setf(uint8_t txv8)
{
    ASSERT_ERR((((uint32_t)txv8 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV8_ADDR, (uint32_t)txv8 << 0);
}

/// @}

/**
 * @name TXV9 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                 TXV9   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV9 register
#define MACBYP_TXV9_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0224)
/// Offset of the TXV9 register from the base address
#define MACBYP_TXV9_OFFSET 0x00000224
/// Index of the TXV9 register
#define MACBYP_TXV9_INDEX  0x00000089
/// Reset value of the TXV9 register
#define MACBYP_TXV9_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV9 register.
 * The TXV9 register will be read and its value returned.
 * @return The current value of the TXV9 register.
 */
__INLINE uint32_t macbyp_txv9_get(void)
{
    return REG_PL_RD(MACBYP_TXV9_ADDR);
}

/**
 * @brief Sets the TXV9 register to a value.
 * The TXV9 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv9_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV9_ADDR, value);
}

// field definitions
/// TXV9 field mask
#define MACBYP_TXV9_MASK   ((uint32_t)0x000000FF)
/// TXV9 field LSB position
#define MACBYP_TXV9_LSB    0
/// TXV9 field width
#define MACBYP_TXV9_WIDTH  ((uint32_t)0x00000008)

/// TXV9 field reset value
#define MACBYP_TXV9_RST    0x0

/**
 * @brief Returns the current value of the TXV9 field in the TXV9 register.
 *
 * The TXV9 register will be read and the TXV9 field's value will be returned.
 *
 * @return The current value of the TXV9 field in the TXV9 register.
 */
__INLINE uint8_t macbyp_txv9_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV9_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV9 field of the TXV9 register.
 *
 * The TXV9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv9 - The value to set the field to.
 */
__INLINE void macbyp_txv9_setf(uint8_t txv9)
{
    ASSERT_ERR((((uint32_t)txv9 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV9_ADDR, (uint32_t)txv9 << 0);
}

/// @}

/**
 * @name TXV10 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV10   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV10 register
#define MACBYP_TXV10_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0228)
/// Offset of the TXV10 register from the base address
#define MACBYP_TXV10_OFFSET 0x00000228
/// Index of the TXV10 register
#define MACBYP_TXV10_INDEX  0x0000008A
/// Reset value of the TXV10 register
#define MACBYP_TXV10_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV10 register.
 * The TXV10 register will be read and its value returned.
 * @return The current value of the TXV10 register.
 */
__INLINE uint32_t macbyp_txv10_get(void)
{
    return REG_PL_RD(MACBYP_TXV10_ADDR);
}

/**
 * @brief Sets the TXV10 register to a value.
 * The TXV10 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv10_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV10_ADDR, value);
}

// field definitions
/// TXV10 field mask
#define MACBYP_TXV10_MASK   ((uint32_t)0x000000FF)
/// TXV10 field LSB position
#define MACBYP_TXV10_LSB    0
/// TXV10 field width
#define MACBYP_TXV10_WIDTH  ((uint32_t)0x00000008)

/// TXV10 field reset value
#define MACBYP_TXV10_RST    0x0

/**
 * @brief Returns the current value of the TXV10 field in the TXV10 register.
 *
 * The TXV10 register will be read and the TXV10 field's value will be returned.
 *
 * @return The current value of the TXV10 field in the TXV10 register.
 */
__INLINE uint8_t macbyp_txv10_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV10_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV10 field of the TXV10 register.
 *
 * The TXV10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv10 - The value to set the field to.
 */
__INLINE void macbyp_txv10_setf(uint8_t txv10)
{
    ASSERT_ERR((((uint32_t)txv10 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV10_ADDR, (uint32_t)txv10 << 0);
}

/// @}

/**
 * @name TXV11 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV11   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV11 register
#define MACBYP_TXV11_ADDR   (REG_MACBYPASS_BASE_ADDR+0x022C)
/// Offset of the TXV11 register from the base address
#define MACBYP_TXV11_OFFSET 0x0000022C
/// Index of the TXV11 register
#define MACBYP_TXV11_INDEX  0x0000008B
/// Reset value of the TXV11 register
#define MACBYP_TXV11_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV11 register.
 * The TXV11 register will be read and its value returned.
 * @return The current value of the TXV11 register.
 */
__INLINE uint32_t macbyp_txv11_get(void)
{
    return REG_PL_RD(MACBYP_TXV11_ADDR);
}

/**
 * @brief Sets the TXV11 register to a value.
 * The TXV11 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv11_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV11_ADDR, value);
}

// field definitions
/// TXV11 field mask
#define MACBYP_TXV11_MASK   ((uint32_t)0x000000FF)
/// TXV11 field LSB position
#define MACBYP_TXV11_LSB    0
/// TXV11 field width
#define MACBYP_TXV11_WIDTH  ((uint32_t)0x00000008)

/// TXV11 field reset value
#define MACBYP_TXV11_RST    0x0

/**
 * @brief Returns the current value of the TXV11 field in the TXV11 register.
 *
 * The TXV11 register will be read and the TXV11 field's value will be returned.
 *
 * @return The current value of the TXV11 field in the TXV11 register.
 */
__INLINE uint8_t macbyp_txv11_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV11_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV11 field of the TXV11 register.
 *
 * The TXV11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv11 - The value to set the field to.
 */
__INLINE void macbyp_txv11_setf(uint8_t txv11)
{
    ASSERT_ERR((((uint32_t)txv11 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV11_ADDR, (uint32_t)txv11 << 0);
}

/// @}

/**
 * @name TXV12 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV12   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV12 register
#define MACBYP_TXV12_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0230)
/// Offset of the TXV12 register from the base address
#define MACBYP_TXV12_OFFSET 0x00000230
/// Index of the TXV12 register
#define MACBYP_TXV12_INDEX  0x0000008C
/// Reset value of the TXV12 register
#define MACBYP_TXV12_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV12 register.
 * The TXV12 register will be read and its value returned.
 * @return The current value of the TXV12 register.
 */
__INLINE uint32_t macbyp_txv12_get(void)
{
    return REG_PL_RD(MACBYP_TXV12_ADDR);
}

/**
 * @brief Sets the TXV12 register to a value.
 * The TXV12 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv12_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV12_ADDR, value);
}

// field definitions
/// TXV12 field mask
#define MACBYP_TXV12_MASK   ((uint32_t)0x000000FF)
/// TXV12 field LSB position
#define MACBYP_TXV12_LSB    0
/// TXV12 field width
#define MACBYP_TXV12_WIDTH  ((uint32_t)0x00000008)

/// TXV12 field reset value
#define MACBYP_TXV12_RST    0x0

/**
 * @brief Returns the current value of the TXV12 field in the TXV12 register.
 *
 * The TXV12 register will be read and the TXV12 field's value will be returned.
 *
 * @return The current value of the TXV12 field in the TXV12 register.
 */
__INLINE uint8_t macbyp_txv12_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV12_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV12 field of the TXV12 register.
 *
 * The TXV12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv12 - The value to set the field to.
 */
__INLINE void macbyp_txv12_setf(uint8_t txv12)
{
    ASSERT_ERR((((uint32_t)txv12 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV12_ADDR, (uint32_t)txv12 << 0);
}

/// @}

/**
 * @name TXV13 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV13   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV13 register
#define MACBYP_TXV13_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0234)
/// Offset of the TXV13 register from the base address
#define MACBYP_TXV13_OFFSET 0x00000234
/// Index of the TXV13 register
#define MACBYP_TXV13_INDEX  0x0000008D
/// Reset value of the TXV13 register
#define MACBYP_TXV13_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV13 register.
 * The TXV13 register will be read and its value returned.
 * @return The current value of the TXV13 register.
 */
__INLINE uint32_t macbyp_txv13_get(void)
{
    return REG_PL_RD(MACBYP_TXV13_ADDR);
}

/**
 * @brief Sets the TXV13 register to a value.
 * The TXV13 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv13_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV13_ADDR, value);
}

// field definitions
/// TXV13 field mask
#define MACBYP_TXV13_MASK   ((uint32_t)0x000000FF)
/// TXV13 field LSB position
#define MACBYP_TXV13_LSB    0
/// TXV13 field width
#define MACBYP_TXV13_WIDTH  ((uint32_t)0x00000008)

/// TXV13 field reset value
#define MACBYP_TXV13_RST    0x0

/**
 * @brief Returns the current value of the TXV13 field in the TXV13 register.
 *
 * The TXV13 register will be read and the TXV13 field's value will be returned.
 *
 * @return The current value of the TXV13 field in the TXV13 register.
 */
__INLINE uint8_t macbyp_txv13_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV13_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV13 field of the TXV13 register.
 *
 * The TXV13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv13 - The value to set the field to.
 */
__INLINE void macbyp_txv13_setf(uint8_t txv13)
{
    ASSERT_ERR((((uint32_t)txv13 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV13_ADDR, (uint32_t)txv13 << 0);
}

/// @}

/**
 * @name TXV14 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV14   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV14 register
#define MACBYP_TXV14_ADDR   (REG_MACBYPASS_BASE_ADDR+0x0238)
/// Offset of the TXV14 register from the base address
#define MACBYP_TXV14_OFFSET 0x00000238
/// Index of the TXV14 register
#define MACBYP_TXV14_INDEX  0x0000008E
/// Reset value of the TXV14 register
#define MACBYP_TXV14_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV14 register.
 * The TXV14 register will be read and its value returned.
 * @return The current value of the TXV14 register.
 */
__INLINE uint32_t macbyp_txv14_get(void)
{
    return REG_PL_RD(MACBYP_TXV14_ADDR);
}

/**
 * @brief Sets the TXV14 register to a value.
 * The TXV14 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv14_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV14_ADDR, value);
}

// field definitions
/// TXV14 field mask
#define MACBYP_TXV14_MASK   ((uint32_t)0x000000FF)
/// TXV14 field LSB position
#define MACBYP_TXV14_LSB    0
/// TXV14 field width
#define MACBYP_TXV14_WIDTH  ((uint32_t)0x00000008)

/// TXV14 field reset value
#define MACBYP_TXV14_RST    0x0

/**
 * @brief Returns the current value of the TXV14 field in the TXV14 register.
 *
 * The TXV14 register will be read and the TXV14 field's value will be returned.
 *
 * @return The current value of the TXV14 field in the TXV14 register.
 */
__INLINE uint8_t macbyp_txv14_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV14_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV14 field of the TXV14 register.
 *
 * The TXV14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv14 - The value to set the field to.
 */
__INLINE void macbyp_txv14_setf(uint8_t txv14)
{
    ASSERT_ERR((((uint32_t)txv14 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV14_ADDR, (uint32_t)txv14 << 0);
}

/// @}

/**
 * @name TXV15 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00                TXV15   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TXV15 register
#define MACBYP_TXV15_ADDR   (REG_MACBYPASS_BASE_ADDR+0x023C)
/// Offset of the TXV15 register from the base address
#define MACBYP_TXV15_OFFSET 0x0000023C
/// Index of the TXV15 register
#define MACBYP_TXV15_INDEX  0x0000008F
/// Reset value of the TXV15 register
#define MACBYP_TXV15_RESET  0x00000000

/**
 * @brief Returns the current value of the TXV15 register.
 * The TXV15 register will be read and its value returned.
 * @return The current value of the TXV15 register.
 */
__INLINE uint32_t macbyp_txv15_get(void)
{
    return REG_PL_RD(MACBYP_TXV15_ADDR);
}

/**
 * @brief Sets the TXV15 register to a value.
 * The TXV15 register will be written.
 * @param value - The value to write.
 */
__INLINE void macbyp_txv15_set(uint32_t value)
{
    REG_PL_WR(MACBYP_TXV15_ADDR, value);
}

// field definitions
/// TXV15 field mask
#define MACBYP_TXV15_MASK   ((uint32_t)0x000000FF)
/// TXV15 field LSB position
#define MACBYP_TXV15_LSB    0
/// TXV15 field width
#define MACBYP_TXV15_WIDTH  ((uint32_t)0x00000008)

/// TXV15 field reset value
#define MACBYP_TXV15_RST    0x0

/**
 * @brief Returns the current value of the TXV15 field in the TXV15 register.
 *
 * The TXV15 register will be read and the TXV15 field's value will be returned.
 *
 * @return The current value of the TXV15 field in the TXV15 register.
 */
__INLINE uint8_t macbyp_txv15_getf(void)
{
    uint32_t localVal = REG_PL_RD(MACBYP_TXV15_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXV15 field of the TXV15 register.
 *
 * The TXV15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txv15 - The value to set the field to.
 */
__INLINE void macbyp_txv15_setf(uint8_t txv15)
{
    ASSERT_ERR((((uint32_t)txv15 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MACBYP_TXV15_ADDR, (uint32_t)txv15 << 0);
}

/// @}


#endif // _REG_MACBYPASS_H_

/// @}

