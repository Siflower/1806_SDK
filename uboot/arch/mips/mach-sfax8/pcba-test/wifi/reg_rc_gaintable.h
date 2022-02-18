/**
 * @file reg_rc_gaintable.h
 * @brief Definitions of the RCG HW block registers and register access functions.
 *
 * @defgroup REG_RC_GAINTABLE REG_RC_GAINTABLE
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the RCG HW block registers and register access functions.
 */
#ifndef _REG_RC_GAINTABLE_H_
#define _REG_RC_GAINTABLE_H_

#include "co_int.h"
#include "_reg_rc.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_RC_GAINTABLE peripheral.
 */
#define REG_RC_GAINTABLE_COUNT 16

/** @brief Decoding mask of the REG_RC_GAINTABLE peripheral registers from the CPU point of view.
 */
#define REG_RC_GAINTABLE_DECODING_MASK 0x0000003F

/**
 * @name RWNXAGCGT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24        AGCGAINTABLE3   0x0
 *  23:16        AGCGAINTABLE2   0x0
 *  15:08        AGCGAINTABLE1   0x0
 *  07:00        AGCGAINTABLE0   0xFF
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT0 register
#define RCG_RWNXAGCGT0_ADDR   (REG_RC_BASE_ADDR+0xC040)
/// Offset of the RWNXAGCGT0 register from the base address
#define RCG_RWNXAGCGT0_OFFSET 0x00000000
/// Index of the RWNXAGCGT0 register
#define RCG_RWNXAGCGT0_INDEX  0x00000000
/// Reset value of the RWNXAGCGT0 register
#define RCG_RWNXAGCGT0_RESET  0x000000FF

/**
 * @brief Returns the current value of the RWNXAGCGT0 register.
 * The RWNXAGCGT0 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT0 register.
 */
__INLINE uint32_t rcg_rwnxagcgt0_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT0_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT0 register to a value.
 * The RWNXAGCGT0 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt0_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR, value);
}

// field definitions
/// AGCGAINTABLE3 field mask
#define RCG_AGCGAINTABLE3_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE3 field LSB position
#define RCG_AGCGAINTABLE3_LSB    24
/// AGCGAINTABLE3 field width
#define RCG_AGCGAINTABLE3_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE2 field mask
#define RCG_AGCGAINTABLE2_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE2 field LSB position
#define RCG_AGCGAINTABLE2_LSB    16
/// AGCGAINTABLE2 field width
#define RCG_AGCGAINTABLE2_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE1 field mask
#define RCG_AGCGAINTABLE1_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE1 field LSB position
#define RCG_AGCGAINTABLE1_LSB    8
/// AGCGAINTABLE1 field width
#define RCG_AGCGAINTABLE1_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE0 field mask
#define RCG_AGCGAINTABLE0_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE0 field LSB position
#define RCG_AGCGAINTABLE0_LSB    0
/// AGCGAINTABLE0 field width
#define RCG_AGCGAINTABLE0_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE3 field reset value
#define RCG_AGCGAINTABLE3_RST    0x0
/// AGCGAINTABLE2 field reset value
#define RCG_AGCGAINTABLE2_RST    0x0
/// AGCGAINTABLE1 field reset value
#define RCG_AGCGAINTABLE1_RST    0x0
/// AGCGAINTABLE0 field reset value
#define RCG_AGCGAINTABLE0_RST    0xFF

/**
 * @brief Constructs a value for the RWNXAGCGT0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable3 - The value to use for the AGCGAINTABLE3 field.
 * @param[in] agcgaintable2 - The value to use for the AGCGAINTABLE2 field.
 * @param[in] agcgaintable1 - The value to use for the AGCGAINTABLE1 field.
 * @param[in] agcgaintable0 - The value to use for the AGCGAINTABLE0 field.
 */
__INLINE void rcg_rwnxagcgt0_pack(uint8_t agcgaintable3, uint8_t agcgaintable2, uint8_t agcgaintable1, uint8_t agcgaintable0)
{
    ASSERT_ERR((((uint32_t)agcgaintable3 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable2 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable1 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable0 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR,  ((uint32_t)agcgaintable3 << 24) | ((uint32_t)agcgaintable2 << 16) | ((uint32_t)agcgaintable1 << 8) | ((uint32_t)agcgaintable0 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT0's fields from current value of the RWNXAGCGT0 register.
 *
 * Reads the RWNXAGCGT0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable3 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable2 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable1 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable0 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt0_unpack(uint8_t* agcgaintable3, uint8_t* agcgaintable2, uint8_t* agcgaintable1, uint8_t* agcgaintable0)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT0_ADDR);

    *agcgaintable3 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable2 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable1 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable0 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE3 field in the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read and the AGCGAINTABLE3 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE3 field in the RWNXAGCGT0 register.
 */
__INLINE uint8_t rcg_agcgaintable3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT0_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE3 field of the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable3 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable3_setf(uint8_t agcgaintable3)
{
    ASSERT_ERR((((uint32_t)agcgaintable3 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR, (REG_PL_RD(RCG_RWNXAGCGT0_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable3 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE2 field in the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read and the AGCGAINTABLE2 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE2 field in the RWNXAGCGT0 register.
 */
__INLINE uint8_t rcg_agcgaintable2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE2 field of the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable2 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable2_setf(uint8_t agcgaintable2)
{
    ASSERT_ERR((((uint32_t)agcgaintable2 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR, (REG_PL_RD(RCG_RWNXAGCGT0_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable2 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE1 field in the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read and the AGCGAINTABLE1 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE1 field in the RWNXAGCGT0 register.
 */
__INLINE uint8_t rcg_agcgaintable1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT0_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE1 field of the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable1 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable1_setf(uint8_t agcgaintable1)
{
    ASSERT_ERR((((uint32_t)agcgaintable1 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR, (REG_PL_RD(RCG_RWNXAGCGT0_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable1 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE0 field in the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read and the AGCGAINTABLE0 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE0 field in the RWNXAGCGT0 register.
 */
__INLINE uint8_t rcg_agcgaintable0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT0_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE0 field of the RWNXAGCGT0 register.
 *
 * The RWNXAGCGT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable0 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable0_setf(uint8_t agcgaintable0)
{
    ASSERT_ERR((((uint32_t)agcgaintable0 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT0_ADDR, (REG_PL_RD(RCG_RWNXAGCGT0_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable0 << 0));
}

/// @}

/**
 * @name RWNXAGCGT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24        AGCGAINTABLE7   0x0
 *  23:16        AGCGAINTABLE6   0x0
 *  15:08        AGCGAINTABLE5   0x0
 *  07:00        AGCGAINTABLE4   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT1 register
#define RCG_RWNXAGCGT1_ADDR   (REG_RC_BASE_ADDR+0xC044)
/// Offset of the RWNXAGCGT1 register from the base address
#define RCG_RWNXAGCGT1_OFFSET 0x00000004
/// Index of the RWNXAGCGT1 register
#define RCG_RWNXAGCGT1_INDEX  0x00000001
/// Reset value of the RWNXAGCGT1 register
#define RCG_RWNXAGCGT1_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCGT1 register.
 * The RWNXAGCGT1 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT1 register.
 */
__INLINE uint32_t rcg_rwnxagcgt1_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT1_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT1 register to a value.
 * The RWNXAGCGT1 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt1_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR, value);
}

// field definitions
/// AGCGAINTABLE7 field mask
#define RCG_AGCGAINTABLE7_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE7 field LSB position
#define RCG_AGCGAINTABLE7_LSB    24
/// AGCGAINTABLE7 field width
#define RCG_AGCGAINTABLE7_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE6 field mask
#define RCG_AGCGAINTABLE6_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE6 field LSB position
#define RCG_AGCGAINTABLE6_LSB    16
/// AGCGAINTABLE6 field width
#define RCG_AGCGAINTABLE6_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE5 field mask
#define RCG_AGCGAINTABLE5_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE5 field LSB position
#define RCG_AGCGAINTABLE5_LSB    8
/// AGCGAINTABLE5 field width
#define RCG_AGCGAINTABLE5_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE4 field mask
#define RCG_AGCGAINTABLE4_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE4 field LSB position
#define RCG_AGCGAINTABLE4_LSB    0
/// AGCGAINTABLE4 field width
#define RCG_AGCGAINTABLE4_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE7 field reset value
#define RCG_AGCGAINTABLE7_RST    0x0
/// AGCGAINTABLE6 field reset value
#define RCG_AGCGAINTABLE6_RST    0x0
/// AGCGAINTABLE5 field reset value
#define RCG_AGCGAINTABLE5_RST    0x0
/// AGCGAINTABLE4 field reset value
#define RCG_AGCGAINTABLE4_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCGT1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable7 - The value to use for the AGCGAINTABLE7 field.
 * @param[in] agcgaintable6 - The value to use for the AGCGAINTABLE6 field.
 * @param[in] agcgaintable5 - The value to use for the AGCGAINTABLE5 field.
 * @param[in] agcgaintable4 - The value to use for the AGCGAINTABLE4 field.
 */
__INLINE void rcg_rwnxagcgt1_pack(uint8_t agcgaintable7, uint8_t agcgaintable6, uint8_t agcgaintable5, uint8_t agcgaintable4)
{
    ASSERT_ERR((((uint32_t)agcgaintable7 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable6 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable5 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable4 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR,  ((uint32_t)agcgaintable7 << 24) | ((uint32_t)agcgaintable6 << 16) | ((uint32_t)agcgaintable5 << 8) | ((uint32_t)agcgaintable4 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT1's fields from current value of the RWNXAGCGT1 register.
 *
 * Reads the RWNXAGCGT1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable7 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable6 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable5 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable4 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt1_unpack(uint8_t* agcgaintable7, uint8_t* agcgaintable6, uint8_t* agcgaintable5, uint8_t* agcgaintable4)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT1_ADDR);

    *agcgaintable7 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable6 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable5 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable4 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE7 field in the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read and the AGCGAINTABLE7 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE7 field in the RWNXAGCGT1 register.
 */
__INLINE uint8_t rcg_agcgaintable7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT1_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE7 field of the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable7 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable7_setf(uint8_t agcgaintable7)
{
    ASSERT_ERR((((uint32_t)agcgaintable7 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR, (REG_PL_RD(RCG_RWNXAGCGT1_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable7 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE6 field in the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read and the AGCGAINTABLE6 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE6 field in the RWNXAGCGT1 register.
 */
__INLINE uint8_t rcg_agcgaintable6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT1_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE6 field of the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable6 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable6_setf(uint8_t agcgaintable6)
{
    ASSERT_ERR((((uint32_t)agcgaintable6 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR, (REG_PL_RD(RCG_RWNXAGCGT1_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable6 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE5 field in the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read and the AGCGAINTABLE5 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE5 field in the RWNXAGCGT1 register.
 */
__INLINE uint8_t rcg_agcgaintable5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT1_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE5 field of the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable5 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable5_setf(uint8_t agcgaintable5)
{
    ASSERT_ERR((((uint32_t)agcgaintable5 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR, (REG_PL_RD(RCG_RWNXAGCGT1_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable5 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE4 field in the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read and the AGCGAINTABLE4 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE4 field in the RWNXAGCGT1 register.
 */
__INLINE uint8_t rcg_agcgaintable4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE4 field of the RWNXAGCGT1 register.
 *
 * The RWNXAGCGT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable4 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable4_setf(uint8_t agcgaintable4)
{
    ASSERT_ERR((((uint32_t)agcgaintable4 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT1_ADDR, (REG_PL_RD(RCG_RWNXAGCGT1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable4 << 0));
}

/// @}

/**
 * @name RWNXAGCGT2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE11   0x0
 *  23:16       AGCGAINTABLE10   0x0
 *  15:08        AGCGAINTABLE9   0x0
 *  07:00        AGCGAINTABLE8   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT2 register
#define RCG_RWNXAGCGT2_ADDR   (REG_RC_BASE_ADDR+0xC048)
/// Offset of the RWNXAGCGT2 register from the base address
#define RCG_RWNXAGCGT2_OFFSET 0x00000008
/// Index of the RWNXAGCGT2 register
#define RCG_RWNXAGCGT2_INDEX  0x00000002
/// Reset value of the RWNXAGCGT2 register
#define RCG_RWNXAGCGT2_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCGT2 register.
 * The RWNXAGCGT2 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT2 register.
 */
__INLINE uint32_t rcg_rwnxagcgt2_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT2_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT2 register to a value.
 * The RWNXAGCGT2 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt2_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR, value);
}

// field definitions
/// AGCGAINTABLE11 field mask
#define RCG_AGCGAINTABLE11_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE11 field LSB position
#define RCG_AGCGAINTABLE11_LSB    24
/// AGCGAINTABLE11 field width
#define RCG_AGCGAINTABLE11_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE10 field mask
#define RCG_AGCGAINTABLE10_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE10 field LSB position
#define RCG_AGCGAINTABLE10_LSB    16
/// AGCGAINTABLE10 field width
#define RCG_AGCGAINTABLE10_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE9 field mask
#define RCG_AGCGAINTABLE9_MASK    ((uint32_t)0x0000FF00)
/// AGCGAINTABLE9 field LSB position
#define RCG_AGCGAINTABLE9_LSB     8
/// AGCGAINTABLE9 field width
#define RCG_AGCGAINTABLE9_WIDTH   ((uint32_t)0x00000008)
/// AGCGAINTABLE8 field mask
#define RCG_AGCGAINTABLE8_MASK    ((uint32_t)0x000000FF)
/// AGCGAINTABLE8 field LSB position
#define RCG_AGCGAINTABLE8_LSB     0
/// AGCGAINTABLE8 field width
#define RCG_AGCGAINTABLE8_WIDTH   ((uint32_t)0x00000008)

/// AGCGAINTABLE11 field reset value
#define RCG_AGCGAINTABLE11_RST    0x0
/// AGCGAINTABLE10 field reset value
#define RCG_AGCGAINTABLE10_RST    0x0
/// AGCGAINTABLE9 field reset value
#define RCG_AGCGAINTABLE9_RST     0x0
/// AGCGAINTABLE8 field reset value
#define RCG_AGCGAINTABLE8_RST     0x0

/**
 * @brief Constructs a value for the RWNXAGCGT2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable11 - The value to use for the AGCGAINTABLE11 field.
 * @param[in] agcgaintable10 - The value to use for the AGCGAINTABLE10 field.
 * @param[in] agcgaintable9 - The value to use for the AGCGAINTABLE9 field.
 * @param[in] agcgaintable8 - The value to use for the AGCGAINTABLE8 field.
 */
__INLINE void rcg_rwnxagcgt2_pack(uint8_t agcgaintable11, uint8_t agcgaintable10, uint8_t agcgaintable9, uint8_t agcgaintable8)
{
    ASSERT_ERR((((uint32_t)agcgaintable11 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable10 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable9 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable8 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR,  ((uint32_t)agcgaintable11 << 24) | ((uint32_t)agcgaintable10 << 16) | ((uint32_t)agcgaintable9 << 8) | ((uint32_t)agcgaintable8 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT2's fields from current value of the RWNXAGCGT2 register.
 *
 * Reads the RWNXAGCGT2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable11 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable10 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable9 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable8 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt2_unpack(uint8_t* agcgaintable11, uint8_t* agcgaintable10, uint8_t* agcgaintable9, uint8_t* agcgaintable8)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT2_ADDR);

    *agcgaintable11 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable10 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable9 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable8 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE11 field in the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read and the AGCGAINTABLE11 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE11 field in the RWNXAGCGT2 register.
 */
__INLINE uint8_t rcg_agcgaintable11_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT2_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE11 field of the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable11 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable11_setf(uint8_t agcgaintable11)
{
    ASSERT_ERR((((uint32_t)agcgaintable11 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR, (REG_PL_RD(RCG_RWNXAGCGT2_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable11 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE10 field in the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read and the AGCGAINTABLE10 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE10 field in the RWNXAGCGT2 register.
 */
__INLINE uint8_t rcg_agcgaintable10_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT2_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE10 field of the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable10 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable10_setf(uint8_t agcgaintable10)
{
    ASSERT_ERR((((uint32_t)agcgaintable10 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR, (REG_PL_RD(RCG_RWNXAGCGT2_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable10 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE9 field in the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read and the AGCGAINTABLE9 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE9 field in the RWNXAGCGT2 register.
 */
__INLINE uint8_t rcg_agcgaintable9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT2_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE9 field of the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable9 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable9_setf(uint8_t agcgaintable9)
{
    ASSERT_ERR((((uint32_t)agcgaintable9 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR, (REG_PL_RD(RCG_RWNXAGCGT2_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable9 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE8 field in the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read and the AGCGAINTABLE8 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE8 field in the RWNXAGCGT2 register.
 */
__INLINE uint8_t rcg_agcgaintable8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT2_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE8 field of the RWNXAGCGT2 register.
 *
 * The RWNXAGCGT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable8 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable8_setf(uint8_t agcgaintable8)
{
    ASSERT_ERR((((uint32_t)agcgaintable8 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT2_ADDR, (REG_PL_RD(RCG_RWNXAGCGT2_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable8 << 0));
}

/// @}

/**
 * @name RWNXAGCGT3 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE15   0x0
 *  23:16       AGCGAINTABLE14   0x0
 *  15:08       AGCGAINTABLE13   0x0
 *  07:00       AGCGAINTABLE12   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT3 register
#define RCG_RWNXAGCGT3_ADDR   (REG_RC_BASE_ADDR+0xC04C)
/// Offset of the RWNXAGCGT3 register from the base address
#define RCG_RWNXAGCGT3_OFFSET 0x0000000C
/// Index of the RWNXAGCGT3 register
#define RCG_RWNXAGCGT3_INDEX  0x00000003
/// Reset value of the RWNXAGCGT3 register
#define RCG_RWNXAGCGT3_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCGT3 register.
 * The RWNXAGCGT3 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT3 register.
 */
__INLINE uint32_t rcg_rwnxagcgt3_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT3_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT3 register to a value.
 * The RWNXAGCGT3 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt3_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR, value);
}

// field definitions
/// AGCGAINTABLE15 field mask
#define RCG_AGCGAINTABLE15_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE15 field LSB position
#define RCG_AGCGAINTABLE15_LSB    24
/// AGCGAINTABLE15 field width
#define RCG_AGCGAINTABLE15_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE14 field mask
#define RCG_AGCGAINTABLE14_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE14 field LSB position
#define RCG_AGCGAINTABLE14_LSB    16
/// AGCGAINTABLE14 field width
#define RCG_AGCGAINTABLE14_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE13 field mask
#define RCG_AGCGAINTABLE13_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE13 field LSB position
#define RCG_AGCGAINTABLE13_LSB    8
/// AGCGAINTABLE13 field width
#define RCG_AGCGAINTABLE13_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE12 field mask
#define RCG_AGCGAINTABLE12_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE12 field LSB position
#define RCG_AGCGAINTABLE12_LSB    0
/// AGCGAINTABLE12 field width
#define RCG_AGCGAINTABLE12_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE15 field reset value
#define RCG_AGCGAINTABLE15_RST    0x0
/// AGCGAINTABLE14 field reset value
#define RCG_AGCGAINTABLE14_RST    0x0
/// AGCGAINTABLE13 field reset value
#define RCG_AGCGAINTABLE13_RST    0x0
/// AGCGAINTABLE12 field reset value
#define RCG_AGCGAINTABLE12_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCGT3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable15 - The value to use for the AGCGAINTABLE15 field.
 * @param[in] agcgaintable14 - The value to use for the AGCGAINTABLE14 field.
 * @param[in] agcgaintable13 - The value to use for the AGCGAINTABLE13 field.
 * @param[in] agcgaintable12 - The value to use for the AGCGAINTABLE12 field.
 */
__INLINE void rcg_rwnxagcgt3_pack(uint8_t agcgaintable15, uint8_t agcgaintable14, uint8_t agcgaintable13, uint8_t agcgaintable12)
{
    ASSERT_ERR((((uint32_t)agcgaintable15 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable14 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable13 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable12 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR,  ((uint32_t)agcgaintable15 << 24) | ((uint32_t)agcgaintable14 << 16) | ((uint32_t)agcgaintable13 << 8) | ((uint32_t)agcgaintable12 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT3's fields from current value of the RWNXAGCGT3 register.
 *
 * Reads the RWNXAGCGT3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable15 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable14 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable13 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable12 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt3_unpack(uint8_t* agcgaintable15, uint8_t* agcgaintable14, uint8_t* agcgaintable13, uint8_t* agcgaintable12)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT3_ADDR);

    *agcgaintable15 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable14 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable13 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable12 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE15 field in the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read and the AGCGAINTABLE15 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE15 field in the RWNXAGCGT3 register.
 */
__INLINE uint8_t rcg_agcgaintable15_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT3_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE15 field of the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable15 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable15_setf(uint8_t agcgaintable15)
{
    ASSERT_ERR((((uint32_t)agcgaintable15 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR, (REG_PL_RD(RCG_RWNXAGCGT3_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable15 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE14 field in the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read and the AGCGAINTABLE14 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE14 field in the RWNXAGCGT3 register.
 */
__INLINE uint8_t rcg_agcgaintable14_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT3_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE14 field of the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable14 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable14_setf(uint8_t agcgaintable14)
{
    ASSERT_ERR((((uint32_t)agcgaintable14 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR, (REG_PL_RD(RCG_RWNXAGCGT3_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable14 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE13 field in the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read and the AGCGAINTABLE13 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE13 field in the RWNXAGCGT3 register.
 */
__INLINE uint8_t rcg_agcgaintable13_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT3_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE13 field of the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable13 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable13_setf(uint8_t agcgaintable13)
{
    ASSERT_ERR((((uint32_t)agcgaintable13 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR, (REG_PL_RD(RCG_RWNXAGCGT3_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable13 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE12 field in the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read and the AGCGAINTABLE12 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE12 field in the RWNXAGCGT3 register.
 */
__INLINE uint8_t rcg_agcgaintable12_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT3_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE12 field of the RWNXAGCGT3 register.
 *
 * The RWNXAGCGT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable12 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable12_setf(uint8_t agcgaintable12)
{
    ASSERT_ERR((((uint32_t)agcgaintable12 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT3_ADDR, (REG_PL_RD(RCG_RWNXAGCGT3_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable12 << 0));
}

/// @}

/**
 * @name RWNXAGCGT4 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE19   0x0
 *  23:16       AGCGAINTABLE18   0x0
 *  15:08       AGCGAINTABLE17   0x0
 *  07:00       AGCGAINTABLE16   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT4 register
#define RCG_RWNXAGCGT4_ADDR   (REG_RC_BASE_ADDR+0xC050)
/// Offset of the RWNXAGCGT4 register from the base address
#define RCG_RWNXAGCGT4_OFFSET 0x00000010
/// Index of the RWNXAGCGT4 register
#define RCG_RWNXAGCGT4_INDEX  0x00000004
/// Reset value of the RWNXAGCGT4 register
#define RCG_RWNXAGCGT4_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCGT4 register.
 * The RWNXAGCGT4 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT4 register.
 */
__INLINE uint32_t rcg_rwnxagcgt4_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT4_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT4 register to a value.
 * The RWNXAGCGT4 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt4_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR, value);
}

// field definitions
/// AGCGAINTABLE19 field mask
#define RCG_AGCGAINTABLE19_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE19 field LSB position
#define RCG_AGCGAINTABLE19_LSB    24
/// AGCGAINTABLE19 field width
#define RCG_AGCGAINTABLE19_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE18 field mask
#define RCG_AGCGAINTABLE18_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE18 field LSB position
#define RCG_AGCGAINTABLE18_LSB    16
/// AGCGAINTABLE18 field width
#define RCG_AGCGAINTABLE18_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE17 field mask
#define RCG_AGCGAINTABLE17_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE17 field LSB position
#define RCG_AGCGAINTABLE17_LSB    8
/// AGCGAINTABLE17 field width
#define RCG_AGCGAINTABLE17_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE16 field mask
#define RCG_AGCGAINTABLE16_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE16 field LSB position
#define RCG_AGCGAINTABLE16_LSB    0
/// AGCGAINTABLE16 field width
#define RCG_AGCGAINTABLE16_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE19 field reset value
#define RCG_AGCGAINTABLE19_RST    0x0
/// AGCGAINTABLE18 field reset value
#define RCG_AGCGAINTABLE18_RST    0x0
/// AGCGAINTABLE17 field reset value
#define RCG_AGCGAINTABLE17_RST    0x0
/// AGCGAINTABLE16 field reset value
#define RCG_AGCGAINTABLE16_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCGT4 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable19 - The value to use for the AGCGAINTABLE19 field.
 * @param[in] agcgaintable18 - The value to use for the AGCGAINTABLE18 field.
 * @param[in] agcgaintable17 - The value to use for the AGCGAINTABLE17 field.
 * @param[in] agcgaintable16 - The value to use for the AGCGAINTABLE16 field.
 */
__INLINE void rcg_rwnxagcgt4_pack(uint8_t agcgaintable19, uint8_t agcgaintable18, uint8_t agcgaintable17, uint8_t agcgaintable16)
{
    ASSERT_ERR((((uint32_t)agcgaintable19 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable18 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable17 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable16 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR,  ((uint32_t)agcgaintable19 << 24) | ((uint32_t)agcgaintable18 << 16) | ((uint32_t)agcgaintable17 << 8) | ((uint32_t)agcgaintable16 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT4's fields from current value of the RWNXAGCGT4 register.
 *
 * Reads the RWNXAGCGT4 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable19 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable18 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable17 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable16 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt4_unpack(uint8_t* agcgaintable19, uint8_t* agcgaintable18, uint8_t* agcgaintable17, uint8_t* agcgaintable16)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT4_ADDR);

    *agcgaintable19 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable18 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable17 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable16 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE19 field in the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read and the AGCGAINTABLE19 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE19 field in the RWNXAGCGT4 register.
 */
__INLINE uint8_t rcg_agcgaintable19_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT4_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE19 field of the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable19 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable19_setf(uint8_t agcgaintable19)
{
    ASSERT_ERR((((uint32_t)agcgaintable19 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR, (REG_PL_RD(RCG_RWNXAGCGT4_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable19 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE18 field in the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read and the AGCGAINTABLE18 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE18 field in the RWNXAGCGT4 register.
 */
__INLINE uint8_t rcg_agcgaintable18_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT4_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE18 field of the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable18 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable18_setf(uint8_t agcgaintable18)
{
    ASSERT_ERR((((uint32_t)agcgaintable18 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR, (REG_PL_RD(RCG_RWNXAGCGT4_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable18 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE17 field in the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read and the AGCGAINTABLE17 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE17 field in the RWNXAGCGT4 register.
 */
__INLINE uint8_t rcg_agcgaintable17_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT4_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE17 field of the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable17 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable17_setf(uint8_t agcgaintable17)
{
    ASSERT_ERR((((uint32_t)agcgaintable17 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR, (REG_PL_RD(RCG_RWNXAGCGT4_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable17 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE16 field in the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read and the AGCGAINTABLE16 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE16 field in the RWNXAGCGT4 register.
 */
__INLINE uint8_t rcg_agcgaintable16_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT4_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE16 field of the RWNXAGCGT4 register.
 *
 * The RWNXAGCGT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable16 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable16_setf(uint8_t agcgaintable16)
{
    ASSERT_ERR((((uint32_t)agcgaintable16 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT4_ADDR, (REG_PL_RD(RCG_RWNXAGCGT4_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable16 << 0));
}

/// @}

/**
 * @name RWNXAGCGT5 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE23   0x0
 *  23:16       AGCGAINTABLE22   0x0
 *  15:08       AGCGAINTABLE21   0x0
 *  07:00       AGCGAINTABLE20   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT5 register
#define RCG_RWNXAGCGT5_ADDR   (REG_RC_BASE_ADDR+0xC054)
/// Offset of the RWNXAGCGT5 register from the base address
#define RCG_RWNXAGCGT5_OFFSET 0x00000014
/// Index of the RWNXAGCGT5 register
#define RCG_RWNXAGCGT5_INDEX  0x00000005
/// Reset value of the RWNXAGCGT5 register
#define RCG_RWNXAGCGT5_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCGT5 register.
 * The RWNXAGCGT5 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT5 register.
 */
__INLINE uint32_t rcg_rwnxagcgt5_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT5_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT5 register to a value.
 * The RWNXAGCGT5 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt5_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR, value);
}

// field definitions
/// AGCGAINTABLE23 field mask
#define RCG_AGCGAINTABLE23_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE23 field LSB position
#define RCG_AGCGAINTABLE23_LSB    24
/// AGCGAINTABLE23 field width
#define RCG_AGCGAINTABLE23_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE22 field mask
#define RCG_AGCGAINTABLE22_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE22 field LSB position
#define RCG_AGCGAINTABLE22_LSB    16
/// AGCGAINTABLE22 field width
#define RCG_AGCGAINTABLE22_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE21 field mask
#define RCG_AGCGAINTABLE21_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE21 field LSB position
#define RCG_AGCGAINTABLE21_LSB    8
/// AGCGAINTABLE21 field width
#define RCG_AGCGAINTABLE21_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE20 field mask
#define RCG_AGCGAINTABLE20_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE20 field LSB position
#define RCG_AGCGAINTABLE20_LSB    0
/// AGCGAINTABLE20 field width
#define RCG_AGCGAINTABLE20_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE23 field reset value
#define RCG_AGCGAINTABLE23_RST    0x0
/// AGCGAINTABLE22 field reset value
#define RCG_AGCGAINTABLE22_RST    0x0
/// AGCGAINTABLE21 field reset value
#define RCG_AGCGAINTABLE21_RST    0x0
/// AGCGAINTABLE20 field reset value
#define RCG_AGCGAINTABLE20_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCGT5 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable23 - The value to use for the AGCGAINTABLE23 field.
 * @param[in] agcgaintable22 - The value to use for the AGCGAINTABLE22 field.
 * @param[in] agcgaintable21 - The value to use for the AGCGAINTABLE21 field.
 * @param[in] agcgaintable20 - The value to use for the AGCGAINTABLE20 field.
 */
__INLINE void rcg_rwnxagcgt5_pack(uint8_t agcgaintable23, uint8_t agcgaintable22, uint8_t agcgaintable21, uint8_t agcgaintable20)
{
    ASSERT_ERR((((uint32_t)agcgaintable23 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable22 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable21 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR,  ((uint32_t)agcgaintable23 << 24) | ((uint32_t)agcgaintable22 << 16) | ((uint32_t)agcgaintable21 << 8) | ((uint32_t)agcgaintable20 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT5's fields from current value of the RWNXAGCGT5 register.
 *
 * Reads the RWNXAGCGT5 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable23 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable22 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable21 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable20 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt5_unpack(uint8_t* agcgaintable23, uint8_t* agcgaintable22, uint8_t* agcgaintable21, uint8_t* agcgaintable20)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT5_ADDR);

    *agcgaintable23 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable22 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable21 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable20 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE23 field in the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read and the AGCGAINTABLE23 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE23 field in the RWNXAGCGT5 register.
 */
__INLINE uint8_t rcg_agcgaintable23_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT5_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE23 field of the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable23 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable23_setf(uint8_t agcgaintable23)
{
    ASSERT_ERR((((uint32_t)agcgaintable23 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR, (REG_PL_RD(RCG_RWNXAGCGT5_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable23 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE22 field in the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read and the AGCGAINTABLE22 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE22 field in the RWNXAGCGT5 register.
 */
__INLINE uint8_t rcg_agcgaintable22_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT5_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE22 field of the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable22 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable22_setf(uint8_t agcgaintable22)
{
    ASSERT_ERR((((uint32_t)agcgaintable22 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR, (REG_PL_RD(RCG_RWNXAGCGT5_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable22 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE21 field in the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read and the AGCGAINTABLE21 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE21 field in the RWNXAGCGT5 register.
 */
__INLINE uint8_t rcg_agcgaintable21_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT5_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE21 field of the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable21 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable21_setf(uint8_t agcgaintable21)
{
    ASSERT_ERR((((uint32_t)agcgaintable21 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR, (REG_PL_RD(RCG_RWNXAGCGT5_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable21 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE20 field in the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read and the AGCGAINTABLE20 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE20 field in the RWNXAGCGT5 register.
 */
__INLINE uint8_t rcg_agcgaintable20_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT5_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE20 field of the RWNXAGCGT5 register.
 *
 * The RWNXAGCGT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable20 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable20_setf(uint8_t agcgaintable20)
{
    ASSERT_ERR((((uint32_t)agcgaintable20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT5_ADDR, (REG_PL_RD(RCG_RWNXAGCGT5_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable20 << 0));
}

/// @}

/**
 * @name RWNXAGCGT6 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE27   0x11
 *  23:16       AGCGAINTABLE26   0x0
 *  15:08       AGCGAINTABLE25   0x0
 *  07:00       AGCGAINTABLE24   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT6 register
#define RCG_RWNXAGCGT6_ADDR   (REG_RC_BASE_ADDR+0xC058)
/// Offset of the RWNXAGCGT6 register from the base address
#define RCG_RWNXAGCGT6_OFFSET 0x00000018
/// Index of the RWNXAGCGT6 register
#define RCG_RWNXAGCGT6_INDEX  0x00000006
/// Reset value of the RWNXAGCGT6 register
#define RCG_RWNXAGCGT6_RESET  0x11000000

/**
 * @brief Returns the current value of the RWNXAGCGT6 register.
 * The RWNXAGCGT6 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT6 register.
 */
__INLINE uint32_t rcg_rwnxagcgt6_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT6_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT6 register to a value.
 * The RWNXAGCGT6 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt6_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR, value);
}

// field definitions
/// AGCGAINTABLE27 field mask
#define RCG_AGCGAINTABLE27_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE27 field LSB position
#define RCG_AGCGAINTABLE27_LSB    24
/// AGCGAINTABLE27 field width
#define RCG_AGCGAINTABLE27_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE26 field mask
#define RCG_AGCGAINTABLE26_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE26 field LSB position
#define RCG_AGCGAINTABLE26_LSB    16
/// AGCGAINTABLE26 field width
#define RCG_AGCGAINTABLE26_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE25 field mask
#define RCG_AGCGAINTABLE25_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE25 field LSB position
#define RCG_AGCGAINTABLE25_LSB    8
/// AGCGAINTABLE25 field width
#define RCG_AGCGAINTABLE25_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE24 field mask
#define RCG_AGCGAINTABLE24_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE24 field LSB position
#define RCG_AGCGAINTABLE24_LSB    0
/// AGCGAINTABLE24 field width
#define RCG_AGCGAINTABLE24_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE27 field reset value
#define RCG_AGCGAINTABLE27_RST    0x11
/// AGCGAINTABLE26 field reset value
#define RCG_AGCGAINTABLE26_RST    0x0
/// AGCGAINTABLE25 field reset value
#define RCG_AGCGAINTABLE25_RST    0x0
/// AGCGAINTABLE24 field reset value
#define RCG_AGCGAINTABLE24_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCGT6 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable27 - The value to use for the AGCGAINTABLE27 field.
 * @param[in] agcgaintable26 - The value to use for the AGCGAINTABLE26 field.
 * @param[in] agcgaintable25 - The value to use for the AGCGAINTABLE25 field.
 * @param[in] agcgaintable24 - The value to use for the AGCGAINTABLE24 field.
 */
__INLINE void rcg_rwnxagcgt6_pack(uint8_t agcgaintable27, uint8_t agcgaintable26, uint8_t agcgaintable25, uint8_t agcgaintable24)
{
    ASSERT_ERR((((uint32_t)agcgaintable27 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable26 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable25 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable24 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR,  ((uint32_t)agcgaintable27 << 24) | ((uint32_t)agcgaintable26 << 16) | ((uint32_t)agcgaintable25 << 8) | ((uint32_t)agcgaintable24 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT6's fields from current value of the RWNXAGCGT6 register.
 *
 * Reads the RWNXAGCGT6 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable27 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable26 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable25 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable24 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt6_unpack(uint8_t* agcgaintable27, uint8_t* agcgaintable26, uint8_t* agcgaintable25, uint8_t* agcgaintable24)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT6_ADDR);

    *agcgaintable27 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable26 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable25 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable24 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE27 field in the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read and the AGCGAINTABLE27 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE27 field in the RWNXAGCGT6 register.
 */
__INLINE uint8_t rcg_agcgaintable27_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT6_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE27 field of the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable27 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable27_setf(uint8_t agcgaintable27)
{
    ASSERT_ERR((((uint32_t)agcgaintable27 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR, (REG_PL_RD(RCG_RWNXAGCGT6_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable27 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE26 field in the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read and the AGCGAINTABLE26 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE26 field in the RWNXAGCGT6 register.
 */
__INLINE uint8_t rcg_agcgaintable26_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT6_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE26 field of the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable26 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable26_setf(uint8_t agcgaintable26)
{
    ASSERT_ERR((((uint32_t)agcgaintable26 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR, (REG_PL_RD(RCG_RWNXAGCGT6_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable26 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE25 field in the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read and the AGCGAINTABLE25 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE25 field in the RWNXAGCGT6 register.
 */
__INLINE uint8_t rcg_agcgaintable25_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT6_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE25 field of the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable25 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable25_setf(uint8_t agcgaintable25)
{
    ASSERT_ERR((((uint32_t)agcgaintable25 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR, (REG_PL_RD(RCG_RWNXAGCGT6_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable25 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE24 field in the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read and the AGCGAINTABLE24 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE24 field in the RWNXAGCGT6 register.
 */
__INLINE uint8_t rcg_agcgaintable24_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT6_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE24 field of the RWNXAGCGT6 register.
 *
 * The RWNXAGCGT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable24 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable24_setf(uint8_t agcgaintable24)
{
    ASSERT_ERR((((uint32_t)agcgaintable24 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT6_ADDR, (REG_PL_RD(RCG_RWNXAGCGT6_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable24 << 0));
}

/// @}

/**
 * @name RWNXAGCGT7 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE31   0x10
 *  23:16       AGCGAINTABLE30   0x10
 *  15:08       AGCGAINTABLE29   0x10
 *  07:00       AGCGAINTABLE28   0x10
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT7 register
#define RCG_RWNXAGCGT7_ADDR   (REG_RC_BASE_ADDR+0xC05C)
/// Offset of the RWNXAGCGT7 register from the base address
#define RCG_RWNXAGCGT7_OFFSET 0x0000001C
/// Index of the RWNXAGCGT7 register
#define RCG_RWNXAGCGT7_INDEX  0x00000007
/// Reset value of the RWNXAGCGT7 register
#define RCG_RWNXAGCGT7_RESET  0x10101010

/**
 * @brief Returns the current value of the RWNXAGCGT7 register.
 * The RWNXAGCGT7 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT7 register.
 */
__INLINE uint32_t rcg_rwnxagcgt7_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT7_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT7 register to a value.
 * The RWNXAGCGT7 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt7_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR, value);
}

// field definitions
/// AGCGAINTABLE31 field mask
#define RCG_AGCGAINTABLE31_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE31 field LSB position
#define RCG_AGCGAINTABLE31_LSB    24
/// AGCGAINTABLE31 field width
#define RCG_AGCGAINTABLE31_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE30 field mask
#define RCG_AGCGAINTABLE30_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE30 field LSB position
#define RCG_AGCGAINTABLE30_LSB    16
/// AGCGAINTABLE30 field width
#define RCG_AGCGAINTABLE30_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE29 field mask
#define RCG_AGCGAINTABLE29_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE29 field LSB position
#define RCG_AGCGAINTABLE29_LSB    8
/// AGCGAINTABLE29 field width
#define RCG_AGCGAINTABLE29_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE28 field mask
#define RCG_AGCGAINTABLE28_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE28 field LSB position
#define RCG_AGCGAINTABLE28_LSB    0
/// AGCGAINTABLE28 field width
#define RCG_AGCGAINTABLE28_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE31 field reset value
#define RCG_AGCGAINTABLE31_RST    0x10
/// AGCGAINTABLE30 field reset value
#define RCG_AGCGAINTABLE30_RST    0x10
/// AGCGAINTABLE29 field reset value
#define RCG_AGCGAINTABLE29_RST    0x10
/// AGCGAINTABLE28 field reset value
#define RCG_AGCGAINTABLE28_RST    0x10

/**
 * @brief Constructs a value for the RWNXAGCGT7 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable31 - The value to use for the AGCGAINTABLE31 field.
 * @param[in] agcgaintable30 - The value to use for the AGCGAINTABLE30 field.
 * @param[in] agcgaintable29 - The value to use for the AGCGAINTABLE29 field.
 * @param[in] agcgaintable28 - The value to use for the AGCGAINTABLE28 field.
 */
__INLINE void rcg_rwnxagcgt7_pack(uint8_t agcgaintable31, uint8_t agcgaintable30, uint8_t agcgaintable29, uint8_t agcgaintable28)
{
    ASSERT_ERR((((uint32_t)agcgaintable31 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable30 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable29 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable28 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR,  ((uint32_t)agcgaintable31 << 24) | ((uint32_t)agcgaintable30 << 16) | ((uint32_t)agcgaintable29 << 8) | ((uint32_t)agcgaintable28 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT7's fields from current value of the RWNXAGCGT7 register.
 *
 * Reads the RWNXAGCGT7 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable31 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable30 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable29 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable28 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt7_unpack(uint8_t* agcgaintable31, uint8_t* agcgaintable30, uint8_t* agcgaintable29, uint8_t* agcgaintable28)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT7_ADDR);

    *agcgaintable31 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable30 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable29 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable28 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE31 field in the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read and the AGCGAINTABLE31 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE31 field in the RWNXAGCGT7 register.
 */
__INLINE uint8_t rcg_agcgaintable31_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT7_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE31 field of the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable31 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable31_setf(uint8_t agcgaintable31)
{
    ASSERT_ERR((((uint32_t)agcgaintable31 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR, (REG_PL_RD(RCG_RWNXAGCGT7_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable31 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE30 field in the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read and the AGCGAINTABLE30 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE30 field in the RWNXAGCGT7 register.
 */
__INLINE uint8_t rcg_agcgaintable30_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT7_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE30 field of the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable30 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable30_setf(uint8_t agcgaintable30)
{
    ASSERT_ERR((((uint32_t)agcgaintable30 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR, (REG_PL_RD(RCG_RWNXAGCGT7_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable30 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE29 field in the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read and the AGCGAINTABLE29 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE29 field in the RWNXAGCGT7 register.
 */
__INLINE uint8_t rcg_agcgaintable29_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT7_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE29 field of the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable29 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable29_setf(uint8_t agcgaintable29)
{
    ASSERT_ERR((((uint32_t)agcgaintable29 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR, (REG_PL_RD(RCG_RWNXAGCGT7_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable29 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE28 field in the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read and the AGCGAINTABLE28 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE28 field in the RWNXAGCGT7 register.
 */
__INLINE uint8_t rcg_agcgaintable28_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT7_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE28 field of the RWNXAGCGT7 register.
 *
 * The RWNXAGCGT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable28 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable28_setf(uint8_t agcgaintable28)
{
    ASSERT_ERR((((uint32_t)agcgaintable28 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT7_ADDR, (REG_PL_RD(RCG_RWNXAGCGT7_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable28 << 0));
}

/// @}

/**
 * @name RWNXAGCGT8 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE35   0x10
 *  23:16       AGCGAINTABLE34   0x10
 *  15:08       AGCGAINTABLE33   0x10
 *  07:00       AGCGAINTABLE32   0x10
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT8 register
#define RCG_RWNXAGCGT8_ADDR   (REG_RC_BASE_ADDR+0xC060)
/// Offset of the RWNXAGCGT8 register from the base address
#define RCG_RWNXAGCGT8_OFFSET 0x00000020
/// Index of the RWNXAGCGT8 register
#define RCG_RWNXAGCGT8_INDEX  0x00000008
/// Reset value of the RWNXAGCGT8 register
#define RCG_RWNXAGCGT8_RESET  0x10101010

/**
 * @brief Returns the current value of the RWNXAGCGT8 register.
 * The RWNXAGCGT8 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT8 register.
 */
__INLINE uint32_t rcg_rwnxagcgt8_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT8_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT8 register to a value.
 * The RWNXAGCGT8 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt8_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR, value);
}

// field definitions
/// AGCGAINTABLE35 field mask
#define RCG_AGCGAINTABLE35_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE35 field LSB position
#define RCG_AGCGAINTABLE35_LSB    24
/// AGCGAINTABLE35 field width
#define RCG_AGCGAINTABLE35_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE34 field mask
#define RCG_AGCGAINTABLE34_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE34 field LSB position
#define RCG_AGCGAINTABLE34_LSB    16
/// AGCGAINTABLE34 field width
#define RCG_AGCGAINTABLE34_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE33 field mask
#define RCG_AGCGAINTABLE33_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE33 field LSB position
#define RCG_AGCGAINTABLE33_LSB    8
/// AGCGAINTABLE33 field width
#define RCG_AGCGAINTABLE33_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE32 field mask
#define RCG_AGCGAINTABLE32_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE32 field LSB position
#define RCG_AGCGAINTABLE32_LSB    0
/// AGCGAINTABLE32 field width
#define RCG_AGCGAINTABLE32_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE35 field reset value
#define RCG_AGCGAINTABLE35_RST    0x10
/// AGCGAINTABLE34 field reset value
#define RCG_AGCGAINTABLE34_RST    0x10
/// AGCGAINTABLE33 field reset value
#define RCG_AGCGAINTABLE33_RST    0x10
/// AGCGAINTABLE32 field reset value
#define RCG_AGCGAINTABLE32_RST    0x10

/**
 * @brief Constructs a value for the RWNXAGCGT8 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable35 - The value to use for the AGCGAINTABLE35 field.
 * @param[in] agcgaintable34 - The value to use for the AGCGAINTABLE34 field.
 * @param[in] agcgaintable33 - The value to use for the AGCGAINTABLE33 field.
 * @param[in] agcgaintable32 - The value to use for the AGCGAINTABLE32 field.
 */
__INLINE void rcg_rwnxagcgt8_pack(uint8_t agcgaintable35, uint8_t agcgaintable34, uint8_t agcgaintable33, uint8_t agcgaintable32)
{
    ASSERT_ERR((((uint32_t)agcgaintable35 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable34 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable33 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable32 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR,  ((uint32_t)agcgaintable35 << 24) | ((uint32_t)agcgaintable34 << 16) | ((uint32_t)agcgaintable33 << 8) | ((uint32_t)agcgaintable32 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT8's fields from current value of the RWNXAGCGT8 register.
 *
 * Reads the RWNXAGCGT8 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable35 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable34 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable33 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable32 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt8_unpack(uint8_t* agcgaintable35, uint8_t* agcgaintable34, uint8_t* agcgaintable33, uint8_t* agcgaintable32)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT8_ADDR);

    *agcgaintable35 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable34 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable33 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable32 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE35 field in the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read and the AGCGAINTABLE35 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE35 field in the RWNXAGCGT8 register.
 */
__INLINE uint8_t rcg_agcgaintable35_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT8_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE35 field of the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable35 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable35_setf(uint8_t agcgaintable35)
{
    ASSERT_ERR((((uint32_t)agcgaintable35 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR, (REG_PL_RD(RCG_RWNXAGCGT8_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable35 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE34 field in the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read and the AGCGAINTABLE34 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE34 field in the RWNXAGCGT8 register.
 */
__INLINE uint8_t rcg_agcgaintable34_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT8_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE34 field of the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable34 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable34_setf(uint8_t agcgaintable34)
{
    ASSERT_ERR((((uint32_t)agcgaintable34 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR, (REG_PL_RD(RCG_RWNXAGCGT8_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable34 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE33 field in the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read and the AGCGAINTABLE33 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE33 field in the RWNXAGCGT8 register.
 */
__INLINE uint8_t rcg_agcgaintable33_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT8_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE33 field of the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable33 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable33_setf(uint8_t agcgaintable33)
{
    ASSERT_ERR((((uint32_t)agcgaintable33 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR, (REG_PL_RD(RCG_RWNXAGCGT8_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable33 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE32 field in the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read and the AGCGAINTABLE32 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE32 field in the RWNXAGCGT8 register.
 */
__INLINE uint8_t rcg_agcgaintable32_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT8_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE32 field of the RWNXAGCGT8 register.
 *
 * The RWNXAGCGT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable32 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable32_setf(uint8_t agcgaintable32)
{
    ASSERT_ERR((((uint32_t)agcgaintable32 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT8_ADDR, (REG_PL_RD(RCG_RWNXAGCGT8_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable32 << 0));
}

/// @}

/**
 * @name RWNXAGCGT9 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE39   0x20
 *  23:16       AGCGAINTABLE38   0x21
 *  15:08       AGCGAINTABLE37   0x10
 *  07:00       AGCGAINTABLE36   0x10
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT9 register
#define RCG_RWNXAGCGT9_ADDR   (REG_RC_BASE_ADDR+0xC064)
/// Offset of the RWNXAGCGT9 register from the base address
#define RCG_RWNXAGCGT9_OFFSET 0x00000024
/// Index of the RWNXAGCGT9 register
#define RCG_RWNXAGCGT9_INDEX  0x00000009
/// Reset value of the RWNXAGCGT9 register
#define RCG_RWNXAGCGT9_RESET  0x20211010

/**
 * @brief Returns the current value of the RWNXAGCGT9 register.
 * The RWNXAGCGT9 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT9 register.
 */
__INLINE uint32_t rcg_rwnxagcgt9_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT9_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT9 register to a value.
 * The RWNXAGCGT9 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt9_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR, value);
}

// field definitions
/// AGCGAINTABLE39 field mask
#define RCG_AGCGAINTABLE39_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE39 field LSB position
#define RCG_AGCGAINTABLE39_LSB    24
/// AGCGAINTABLE39 field width
#define RCG_AGCGAINTABLE39_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE38 field mask
#define RCG_AGCGAINTABLE38_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE38 field LSB position
#define RCG_AGCGAINTABLE38_LSB    16
/// AGCGAINTABLE38 field width
#define RCG_AGCGAINTABLE38_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE37 field mask
#define RCG_AGCGAINTABLE37_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE37 field LSB position
#define RCG_AGCGAINTABLE37_LSB    8
/// AGCGAINTABLE37 field width
#define RCG_AGCGAINTABLE37_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE36 field mask
#define RCG_AGCGAINTABLE36_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE36 field LSB position
#define RCG_AGCGAINTABLE36_LSB    0
/// AGCGAINTABLE36 field width
#define RCG_AGCGAINTABLE36_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE39 field reset value
#define RCG_AGCGAINTABLE39_RST    0x20
/// AGCGAINTABLE38 field reset value
#define RCG_AGCGAINTABLE38_RST    0x21
/// AGCGAINTABLE37 field reset value
#define RCG_AGCGAINTABLE37_RST    0x10
/// AGCGAINTABLE36 field reset value
#define RCG_AGCGAINTABLE36_RST    0x10

/**
 * @brief Constructs a value for the RWNXAGCGT9 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable39 - The value to use for the AGCGAINTABLE39 field.
 * @param[in] agcgaintable38 - The value to use for the AGCGAINTABLE38 field.
 * @param[in] agcgaintable37 - The value to use for the AGCGAINTABLE37 field.
 * @param[in] agcgaintable36 - The value to use for the AGCGAINTABLE36 field.
 */
__INLINE void rcg_rwnxagcgt9_pack(uint8_t agcgaintable39, uint8_t agcgaintable38, uint8_t agcgaintable37, uint8_t agcgaintable36)
{
    ASSERT_ERR((((uint32_t)agcgaintable39 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable38 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable37 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable36 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR,  ((uint32_t)agcgaintable39 << 24) | ((uint32_t)agcgaintable38 << 16) | ((uint32_t)agcgaintable37 << 8) | ((uint32_t)agcgaintable36 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT9's fields from current value of the RWNXAGCGT9 register.
 *
 * Reads the RWNXAGCGT9 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable39 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable38 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable37 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable36 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt9_unpack(uint8_t* agcgaintable39, uint8_t* agcgaintable38, uint8_t* agcgaintable37, uint8_t* agcgaintable36)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT9_ADDR);

    *agcgaintable39 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable38 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable37 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable36 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE39 field in the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read and the AGCGAINTABLE39 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE39 field in the RWNXAGCGT9 register.
 */
__INLINE uint8_t rcg_agcgaintable39_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT9_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE39 field of the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable39 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable39_setf(uint8_t agcgaintable39)
{
    ASSERT_ERR((((uint32_t)agcgaintable39 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR, (REG_PL_RD(RCG_RWNXAGCGT9_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable39 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE38 field in the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read and the AGCGAINTABLE38 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE38 field in the RWNXAGCGT9 register.
 */
__INLINE uint8_t rcg_agcgaintable38_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT9_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE38 field of the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable38 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable38_setf(uint8_t agcgaintable38)
{
    ASSERT_ERR((((uint32_t)agcgaintable38 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR, (REG_PL_RD(RCG_RWNXAGCGT9_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable38 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE37 field in the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read and the AGCGAINTABLE37 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE37 field in the RWNXAGCGT9 register.
 */
__INLINE uint8_t rcg_agcgaintable37_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT9_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE37 field of the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable37 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable37_setf(uint8_t agcgaintable37)
{
    ASSERT_ERR((((uint32_t)agcgaintable37 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR, (REG_PL_RD(RCG_RWNXAGCGT9_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable37 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE36 field in the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read and the AGCGAINTABLE36 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE36 field in the RWNXAGCGT9 register.
 */
__INLINE uint8_t rcg_agcgaintable36_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT9_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE36 field of the RWNXAGCGT9 register.
 *
 * The RWNXAGCGT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable36 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable36_setf(uint8_t agcgaintable36)
{
    ASSERT_ERR((((uint32_t)agcgaintable36 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT9_ADDR, (REG_PL_RD(RCG_RWNXAGCGT9_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable36 << 0));
}

/// @}

/**
 * @name RWNXAGCGT10 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE43   0x20
 *  23:16       AGCGAINTABLE42   0x20
 *  15:08       AGCGAINTABLE41   0x20
 *  07:00       AGCGAINTABLE40   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT10 register
#define RCG_RWNXAGCGT10_ADDR   (REG_RC_BASE_ADDR+0xC068)
/// Offset of the RWNXAGCGT10 register from the base address
#define RCG_RWNXAGCGT10_OFFSET 0x00000028
/// Index of the RWNXAGCGT10 register
#define RCG_RWNXAGCGT10_INDEX  0x0000000A
/// Reset value of the RWNXAGCGT10 register
#define RCG_RWNXAGCGT10_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT10 register.
 * The RWNXAGCGT10 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT10 register.
 */
__INLINE uint32_t rcg_rwnxagcgt10_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT10_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT10 register to a value.
 * The RWNXAGCGT10 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt10_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR, value);
}

// field definitions
/// AGCGAINTABLE43 field mask
#define RCG_AGCGAINTABLE43_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE43 field LSB position
#define RCG_AGCGAINTABLE43_LSB    24
/// AGCGAINTABLE43 field width
#define RCG_AGCGAINTABLE43_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE42 field mask
#define RCG_AGCGAINTABLE42_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE42 field LSB position
#define RCG_AGCGAINTABLE42_LSB    16
/// AGCGAINTABLE42 field width
#define RCG_AGCGAINTABLE42_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE41 field mask
#define RCG_AGCGAINTABLE41_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE41 field LSB position
#define RCG_AGCGAINTABLE41_LSB    8
/// AGCGAINTABLE41 field width
#define RCG_AGCGAINTABLE41_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE40 field mask
#define RCG_AGCGAINTABLE40_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE40 field LSB position
#define RCG_AGCGAINTABLE40_LSB    0
/// AGCGAINTABLE40 field width
#define RCG_AGCGAINTABLE40_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE43 field reset value
#define RCG_AGCGAINTABLE43_RST    0x20
/// AGCGAINTABLE42 field reset value
#define RCG_AGCGAINTABLE42_RST    0x20
/// AGCGAINTABLE41 field reset value
#define RCG_AGCGAINTABLE41_RST    0x20
/// AGCGAINTABLE40 field reset value
#define RCG_AGCGAINTABLE40_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT10 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable43 - The value to use for the AGCGAINTABLE43 field.
 * @param[in] agcgaintable42 - The value to use for the AGCGAINTABLE42 field.
 * @param[in] agcgaintable41 - The value to use for the AGCGAINTABLE41 field.
 * @param[in] agcgaintable40 - The value to use for the AGCGAINTABLE40 field.
 */
__INLINE void rcg_rwnxagcgt10_pack(uint8_t agcgaintable43, uint8_t agcgaintable42, uint8_t agcgaintable41, uint8_t agcgaintable40)
{
    ASSERT_ERR((((uint32_t)agcgaintable43 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable42 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable41 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable40 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR,  ((uint32_t)agcgaintable43 << 24) | ((uint32_t)agcgaintable42 << 16) | ((uint32_t)agcgaintable41 << 8) | ((uint32_t)agcgaintable40 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT10's fields from current value of the RWNXAGCGT10 register.
 *
 * Reads the RWNXAGCGT10 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable43 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable42 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable41 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable40 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt10_unpack(uint8_t* agcgaintable43, uint8_t* agcgaintable42, uint8_t* agcgaintable41, uint8_t* agcgaintable40)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT10_ADDR);

    *agcgaintable43 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable42 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable41 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable40 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE43 field in the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read and the AGCGAINTABLE43 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE43 field in the RWNXAGCGT10 register.
 */
__INLINE uint8_t rcg_agcgaintable43_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT10_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE43 field of the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable43 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable43_setf(uint8_t agcgaintable43)
{
    ASSERT_ERR((((uint32_t)agcgaintable43 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR, (REG_PL_RD(RCG_RWNXAGCGT10_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable43 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE42 field in the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read and the AGCGAINTABLE42 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE42 field in the RWNXAGCGT10 register.
 */
__INLINE uint8_t rcg_agcgaintable42_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT10_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE42 field of the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable42 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable42_setf(uint8_t agcgaintable42)
{
    ASSERT_ERR((((uint32_t)agcgaintable42 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR, (REG_PL_RD(RCG_RWNXAGCGT10_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable42 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE41 field in the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read and the AGCGAINTABLE41 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE41 field in the RWNXAGCGT10 register.
 */
__INLINE uint8_t rcg_agcgaintable41_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT10_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE41 field of the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable41 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable41_setf(uint8_t agcgaintable41)
{
    ASSERT_ERR((((uint32_t)agcgaintable41 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR, (REG_PL_RD(RCG_RWNXAGCGT10_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable41 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE40 field in the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read and the AGCGAINTABLE40 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE40 field in the RWNXAGCGT10 register.
 */
__INLINE uint8_t rcg_agcgaintable40_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT10_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE40 field of the RWNXAGCGT10 register.
 *
 * The RWNXAGCGT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable40 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable40_setf(uint8_t agcgaintable40)
{
    ASSERT_ERR((((uint32_t)agcgaintable40 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT10_ADDR, (REG_PL_RD(RCG_RWNXAGCGT10_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable40 << 0));
}

/// @}

/**
 * @name RWNXAGCGT11 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE47   0x20
 *  23:16       AGCGAINTABLE46   0x20
 *  15:08       AGCGAINTABLE45   0x20
 *  07:00       AGCGAINTABLE44   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT11 register
#define RCG_RWNXAGCGT11_ADDR   (REG_RC_BASE_ADDR+0xC06C)
/// Offset of the RWNXAGCGT11 register from the base address
#define RCG_RWNXAGCGT11_OFFSET 0x0000002C
/// Index of the RWNXAGCGT11 register
#define RCG_RWNXAGCGT11_INDEX  0x0000000B
/// Reset value of the RWNXAGCGT11 register
#define RCG_RWNXAGCGT11_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT11 register.
 * The RWNXAGCGT11 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT11 register.
 */
__INLINE uint32_t rcg_rwnxagcgt11_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT11_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT11 register to a value.
 * The RWNXAGCGT11 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt11_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR, value);
}

// field definitions
/// AGCGAINTABLE47 field mask
#define RCG_AGCGAINTABLE47_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE47 field LSB position
#define RCG_AGCGAINTABLE47_LSB    24
/// AGCGAINTABLE47 field width
#define RCG_AGCGAINTABLE47_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE46 field mask
#define RCG_AGCGAINTABLE46_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE46 field LSB position
#define RCG_AGCGAINTABLE46_LSB    16
/// AGCGAINTABLE46 field width
#define RCG_AGCGAINTABLE46_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE45 field mask
#define RCG_AGCGAINTABLE45_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE45 field LSB position
#define RCG_AGCGAINTABLE45_LSB    8
/// AGCGAINTABLE45 field width
#define RCG_AGCGAINTABLE45_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE44 field mask
#define RCG_AGCGAINTABLE44_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE44 field LSB position
#define RCG_AGCGAINTABLE44_LSB    0
/// AGCGAINTABLE44 field width
#define RCG_AGCGAINTABLE44_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE47 field reset value
#define RCG_AGCGAINTABLE47_RST    0x20
/// AGCGAINTABLE46 field reset value
#define RCG_AGCGAINTABLE46_RST    0x20
/// AGCGAINTABLE45 field reset value
#define RCG_AGCGAINTABLE45_RST    0x20
/// AGCGAINTABLE44 field reset value
#define RCG_AGCGAINTABLE44_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT11 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable47 - The value to use for the AGCGAINTABLE47 field.
 * @param[in] agcgaintable46 - The value to use for the AGCGAINTABLE46 field.
 * @param[in] agcgaintable45 - The value to use for the AGCGAINTABLE45 field.
 * @param[in] agcgaintable44 - The value to use for the AGCGAINTABLE44 field.
 */
__INLINE void rcg_rwnxagcgt11_pack(uint8_t agcgaintable47, uint8_t agcgaintable46, uint8_t agcgaintable45, uint8_t agcgaintable44)
{
    ASSERT_ERR((((uint32_t)agcgaintable47 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable46 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable45 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable44 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR,  ((uint32_t)agcgaintable47 << 24) | ((uint32_t)agcgaintable46 << 16) | ((uint32_t)agcgaintable45 << 8) | ((uint32_t)agcgaintable44 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT11's fields from current value of the RWNXAGCGT11 register.
 *
 * Reads the RWNXAGCGT11 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable47 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable46 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable45 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable44 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt11_unpack(uint8_t* agcgaintable47, uint8_t* agcgaintable46, uint8_t* agcgaintable45, uint8_t* agcgaintable44)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT11_ADDR);

    *agcgaintable47 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable46 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable45 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable44 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE47 field in the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read and the AGCGAINTABLE47 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE47 field in the RWNXAGCGT11 register.
 */
__INLINE uint8_t rcg_agcgaintable47_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT11_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE47 field of the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable47 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable47_setf(uint8_t agcgaintable47)
{
    ASSERT_ERR((((uint32_t)agcgaintable47 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR, (REG_PL_RD(RCG_RWNXAGCGT11_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable47 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE46 field in the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read and the AGCGAINTABLE46 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE46 field in the RWNXAGCGT11 register.
 */
__INLINE uint8_t rcg_agcgaintable46_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT11_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE46 field of the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable46 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable46_setf(uint8_t agcgaintable46)
{
    ASSERT_ERR((((uint32_t)agcgaintable46 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR, (REG_PL_RD(RCG_RWNXAGCGT11_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable46 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE45 field in the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read and the AGCGAINTABLE45 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE45 field in the RWNXAGCGT11 register.
 */
__INLINE uint8_t rcg_agcgaintable45_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT11_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE45 field of the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable45 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable45_setf(uint8_t agcgaintable45)
{
    ASSERT_ERR((((uint32_t)agcgaintable45 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR, (REG_PL_RD(RCG_RWNXAGCGT11_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable45 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE44 field in the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read and the AGCGAINTABLE44 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE44 field in the RWNXAGCGT11 register.
 */
__INLINE uint8_t rcg_agcgaintable44_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT11_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE44 field of the RWNXAGCGT11 register.
 *
 * The RWNXAGCGT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable44 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable44_setf(uint8_t agcgaintable44)
{
    ASSERT_ERR((((uint32_t)agcgaintable44 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT11_ADDR, (REG_PL_RD(RCG_RWNXAGCGT11_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable44 << 0));
}

/// @}

/**
 * @name RWNXAGCGT12 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE51   0x20
 *  23:16       AGCGAINTABLE50   0x20
 *  15:08       AGCGAINTABLE49   0x20
 *  07:00       AGCGAINTABLE48   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT12 register
#define RCG_RWNXAGCGT12_ADDR   (REG_RC_BASE_ADDR+0xC070)
/// Offset of the RWNXAGCGT12 register from the base address
#define RCG_RWNXAGCGT12_OFFSET 0x00000030
/// Index of the RWNXAGCGT12 register
#define RCG_RWNXAGCGT12_INDEX  0x0000000C
/// Reset value of the RWNXAGCGT12 register
#define RCG_RWNXAGCGT12_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT12 register.
 * The RWNXAGCGT12 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT12 register.
 */
__INLINE uint32_t rcg_rwnxagcgt12_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT12_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT12 register to a value.
 * The RWNXAGCGT12 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt12_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR, value);
}

// field definitions
/// AGCGAINTABLE51 field mask
#define RCG_AGCGAINTABLE51_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE51 field LSB position
#define RCG_AGCGAINTABLE51_LSB    24
/// AGCGAINTABLE51 field width
#define RCG_AGCGAINTABLE51_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE50 field mask
#define RCG_AGCGAINTABLE50_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE50 field LSB position
#define RCG_AGCGAINTABLE50_LSB    16
/// AGCGAINTABLE50 field width
#define RCG_AGCGAINTABLE50_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE49 field mask
#define RCG_AGCGAINTABLE49_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE49 field LSB position
#define RCG_AGCGAINTABLE49_LSB    8
/// AGCGAINTABLE49 field width
#define RCG_AGCGAINTABLE49_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE48 field mask
#define RCG_AGCGAINTABLE48_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE48 field LSB position
#define RCG_AGCGAINTABLE48_LSB    0
/// AGCGAINTABLE48 field width
#define RCG_AGCGAINTABLE48_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE51 field reset value
#define RCG_AGCGAINTABLE51_RST    0x20
/// AGCGAINTABLE50 field reset value
#define RCG_AGCGAINTABLE50_RST    0x20
/// AGCGAINTABLE49 field reset value
#define RCG_AGCGAINTABLE49_RST    0x20
/// AGCGAINTABLE48 field reset value
#define RCG_AGCGAINTABLE48_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT12 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable51 - The value to use for the AGCGAINTABLE51 field.
 * @param[in] agcgaintable50 - The value to use for the AGCGAINTABLE50 field.
 * @param[in] agcgaintable49 - The value to use for the AGCGAINTABLE49 field.
 * @param[in] agcgaintable48 - The value to use for the AGCGAINTABLE48 field.
 */
__INLINE void rcg_rwnxagcgt12_pack(uint8_t agcgaintable51, uint8_t agcgaintable50, uint8_t agcgaintable49, uint8_t agcgaintable48)
{
    ASSERT_ERR((((uint32_t)agcgaintable51 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable50 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable49 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable48 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR,  ((uint32_t)agcgaintable51 << 24) | ((uint32_t)agcgaintable50 << 16) | ((uint32_t)agcgaintable49 << 8) | ((uint32_t)agcgaintable48 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT12's fields from current value of the RWNXAGCGT12 register.
 *
 * Reads the RWNXAGCGT12 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable51 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable50 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable49 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable48 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt12_unpack(uint8_t* agcgaintable51, uint8_t* agcgaintable50, uint8_t* agcgaintable49, uint8_t* agcgaintable48)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT12_ADDR);

    *agcgaintable51 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable50 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable49 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable48 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE51 field in the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read and the AGCGAINTABLE51 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE51 field in the RWNXAGCGT12 register.
 */
__INLINE uint8_t rcg_agcgaintable51_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT12_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE51 field of the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable51 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable51_setf(uint8_t agcgaintable51)
{
    ASSERT_ERR((((uint32_t)agcgaintable51 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR, (REG_PL_RD(RCG_RWNXAGCGT12_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable51 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE50 field in the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read and the AGCGAINTABLE50 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE50 field in the RWNXAGCGT12 register.
 */
__INLINE uint8_t rcg_agcgaintable50_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT12_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE50 field of the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable50 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable50_setf(uint8_t agcgaintable50)
{
    ASSERT_ERR((((uint32_t)agcgaintable50 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR, (REG_PL_RD(RCG_RWNXAGCGT12_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable50 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE49 field in the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read and the AGCGAINTABLE49 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE49 field in the RWNXAGCGT12 register.
 */
__INLINE uint8_t rcg_agcgaintable49_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT12_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE49 field of the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable49 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable49_setf(uint8_t agcgaintable49)
{
    ASSERT_ERR((((uint32_t)agcgaintable49 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR, (REG_PL_RD(RCG_RWNXAGCGT12_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable49 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE48 field in the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read and the AGCGAINTABLE48 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE48 field in the RWNXAGCGT12 register.
 */
__INLINE uint8_t rcg_agcgaintable48_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT12_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE48 field of the RWNXAGCGT12 register.
 *
 * The RWNXAGCGT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable48 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable48_setf(uint8_t agcgaintable48)
{
    ASSERT_ERR((((uint32_t)agcgaintable48 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT12_ADDR, (REG_PL_RD(RCG_RWNXAGCGT12_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable48 << 0));
}

/// @}

/**
 * @name RWNXAGCGT13 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE55   0x20
 *  23:16       AGCGAINTABLE54   0x20
 *  15:08       AGCGAINTABLE53   0x20
 *  07:00       AGCGAINTABLE52   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT13 register
#define RCG_RWNXAGCGT13_ADDR   (REG_RC_BASE_ADDR+0xC074)
/// Offset of the RWNXAGCGT13 register from the base address
#define RCG_RWNXAGCGT13_OFFSET 0x00000034
/// Index of the RWNXAGCGT13 register
#define RCG_RWNXAGCGT13_INDEX  0x0000000D
/// Reset value of the RWNXAGCGT13 register
#define RCG_RWNXAGCGT13_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT13 register.
 * The RWNXAGCGT13 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT13 register.
 */
__INLINE uint32_t rcg_rwnxagcgt13_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT13_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT13 register to a value.
 * The RWNXAGCGT13 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt13_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR, value);
}

// field definitions
/// AGCGAINTABLE55 field mask
#define RCG_AGCGAINTABLE55_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE55 field LSB position
#define RCG_AGCGAINTABLE55_LSB    24
/// AGCGAINTABLE55 field width
#define RCG_AGCGAINTABLE55_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE54 field mask
#define RCG_AGCGAINTABLE54_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE54 field LSB position
#define RCG_AGCGAINTABLE54_LSB    16
/// AGCGAINTABLE54 field width
#define RCG_AGCGAINTABLE54_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE53 field mask
#define RCG_AGCGAINTABLE53_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE53 field LSB position
#define RCG_AGCGAINTABLE53_LSB    8
/// AGCGAINTABLE53 field width
#define RCG_AGCGAINTABLE53_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE52 field mask
#define RCG_AGCGAINTABLE52_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE52 field LSB position
#define RCG_AGCGAINTABLE52_LSB    0
/// AGCGAINTABLE52 field width
#define RCG_AGCGAINTABLE52_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE55 field reset value
#define RCG_AGCGAINTABLE55_RST    0x20
/// AGCGAINTABLE54 field reset value
#define RCG_AGCGAINTABLE54_RST    0x20
/// AGCGAINTABLE53 field reset value
#define RCG_AGCGAINTABLE53_RST    0x20
/// AGCGAINTABLE52 field reset value
#define RCG_AGCGAINTABLE52_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT13 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable55 - The value to use for the AGCGAINTABLE55 field.
 * @param[in] agcgaintable54 - The value to use for the AGCGAINTABLE54 field.
 * @param[in] agcgaintable53 - The value to use for the AGCGAINTABLE53 field.
 * @param[in] agcgaintable52 - The value to use for the AGCGAINTABLE52 field.
 */
__INLINE void rcg_rwnxagcgt13_pack(uint8_t agcgaintable55, uint8_t agcgaintable54, uint8_t agcgaintable53, uint8_t agcgaintable52)
{
    ASSERT_ERR((((uint32_t)agcgaintable55 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable54 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable53 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable52 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR,  ((uint32_t)agcgaintable55 << 24) | ((uint32_t)agcgaintable54 << 16) | ((uint32_t)agcgaintable53 << 8) | ((uint32_t)agcgaintable52 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT13's fields from current value of the RWNXAGCGT13 register.
 *
 * Reads the RWNXAGCGT13 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable55 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable54 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable53 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable52 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt13_unpack(uint8_t* agcgaintable55, uint8_t* agcgaintable54, uint8_t* agcgaintable53, uint8_t* agcgaintable52)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT13_ADDR);

    *agcgaintable55 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable54 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable53 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable52 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE55 field in the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read and the AGCGAINTABLE55 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE55 field in the RWNXAGCGT13 register.
 */
__INLINE uint8_t rcg_agcgaintable55_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT13_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE55 field of the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable55 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable55_setf(uint8_t agcgaintable55)
{
    ASSERT_ERR((((uint32_t)agcgaintable55 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR, (REG_PL_RD(RCG_RWNXAGCGT13_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable55 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE54 field in the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read and the AGCGAINTABLE54 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE54 field in the RWNXAGCGT13 register.
 */
__INLINE uint8_t rcg_agcgaintable54_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT13_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE54 field of the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable54 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable54_setf(uint8_t agcgaintable54)
{
    ASSERT_ERR((((uint32_t)agcgaintable54 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR, (REG_PL_RD(RCG_RWNXAGCGT13_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable54 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE53 field in the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read and the AGCGAINTABLE53 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE53 field in the RWNXAGCGT13 register.
 */
__INLINE uint8_t rcg_agcgaintable53_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT13_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE53 field of the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable53 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable53_setf(uint8_t agcgaintable53)
{
    ASSERT_ERR((((uint32_t)agcgaintable53 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR, (REG_PL_RD(RCG_RWNXAGCGT13_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable53 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE52 field in the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read and the AGCGAINTABLE52 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE52 field in the RWNXAGCGT13 register.
 */
__INLINE uint8_t rcg_agcgaintable52_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT13_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE52 field of the RWNXAGCGT13 register.
 *
 * The RWNXAGCGT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable52 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable52_setf(uint8_t agcgaintable52)
{
    ASSERT_ERR((((uint32_t)agcgaintable52 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT13_ADDR, (REG_PL_RD(RCG_RWNXAGCGT13_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable52 << 0));
}

/// @}

/**
 * @name RWNXAGCGT14 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE59   0x20
 *  23:16       AGCGAINTABLE58   0x20
 *  15:08       AGCGAINTABLE57   0x20
 *  07:00       AGCGAINTABLE56   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT14 register
#define RCG_RWNXAGCGT14_ADDR   (REG_RC_BASE_ADDR+0xC078)
/// Offset of the RWNXAGCGT14 register from the base address
#define RCG_RWNXAGCGT14_OFFSET 0x00000038
/// Index of the RWNXAGCGT14 register
#define RCG_RWNXAGCGT14_INDEX  0x0000000E
/// Reset value of the RWNXAGCGT14 register
#define RCG_RWNXAGCGT14_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT14 register.
 * The RWNXAGCGT14 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT14 register.
 */
__INLINE uint32_t rcg_rwnxagcgt14_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT14_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT14 register to a value.
 * The RWNXAGCGT14 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt14_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR, value);
}

// field definitions
/// AGCGAINTABLE59 field mask
#define RCG_AGCGAINTABLE59_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE59 field LSB position
#define RCG_AGCGAINTABLE59_LSB    24
/// AGCGAINTABLE59 field width
#define RCG_AGCGAINTABLE59_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE58 field mask
#define RCG_AGCGAINTABLE58_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE58 field LSB position
#define RCG_AGCGAINTABLE58_LSB    16
/// AGCGAINTABLE58 field width
#define RCG_AGCGAINTABLE58_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE57 field mask
#define RCG_AGCGAINTABLE57_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE57 field LSB position
#define RCG_AGCGAINTABLE57_LSB    8
/// AGCGAINTABLE57 field width
#define RCG_AGCGAINTABLE57_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE56 field mask
#define RCG_AGCGAINTABLE56_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE56 field LSB position
#define RCG_AGCGAINTABLE56_LSB    0
/// AGCGAINTABLE56 field width
#define RCG_AGCGAINTABLE56_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE59 field reset value
#define RCG_AGCGAINTABLE59_RST    0x20
/// AGCGAINTABLE58 field reset value
#define RCG_AGCGAINTABLE58_RST    0x20
/// AGCGAINTABLE57 field reset value
#define RCG_AGCGAINTABLE57_RST    0x20
/// AGCGAINTABLE56 field reset value
#define RCG_AGCGAINTABLE56_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT14 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable59 - The value to use for the AGCGAINTABLE59 field.
 * @param[in] agcgaintable58 - The value to use for the AGCGAINTABLE58 field.
 * @param[in] agcgaintable57 - The value to use for the AGCGAINTABLE57 field.
 * @param[in] agcgaintable56 - The value to use for the AGCGAINTABLE56 field.
 */
__INLINE void rcg_rwnxagcgt14_pack(uint8_t agcgaintable59, uint8_t agcgaintable58, uint8_t agcgaintable57, uint8_t agcgaintable56)
{
    ASSERT_ERR((((uint32_t)agcgaintable59 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable58 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable57 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable56 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR,  ((uint32_t)agcgaintable59 << 24) | ((uint32_t)agcgaintable58 << 16) | ((uint32_t)agcgaintable57 << 8) | ((uint32_t)agcgaintable56 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT14's fields from current value of the RWNXAGCGT14 register.
 *
 * Reads the RWNXAGCGT14 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable59 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable58 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable57 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable56 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt14_unpack(uint8_t* agcgaintable59, uint8_t* agcgaintable58, uint8_t* agcgaintable57, uint8_t* agcgaintable56)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT14_ADDR);

    *agcgaintable59 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable58 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable57 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable56 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE59 field in the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read and the AGCGAINTABLE59 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE59 field in the RWNXAGCGT14 register.
 */
__INLINE uint8_t rcg_agcgaintable59_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT14_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE59 field of the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable59 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable59_setf(uint8_t agcgaintable59)
{
    ASSERT_ERR((((uint32_t)agcgaintable59 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR, (REG_PL_RD(RCG_RWNXAGCGT14_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable59 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE58 field in the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read and the AGCGAINTABLE58 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE58 field in the RWNXAGCGT14 register.
 */
__INLINE uint8_t rcg_agcgaintable58_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT14_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE58 field of the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable58 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable58_setf(uint8_t agcgaintable58)
{
    ASSERT_ERR((((uint32_t)agcgaintable58 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR, (REG_PL_RD(RCG_RWNXAGCGT14_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable58 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE57 field in the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read and the AGCGAINTABLE57 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE57 field in the RWNXAGCGT14 register.
 */
__INLINE uint8_t rcg_agcgaintable57_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT14_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE57 field of the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable57 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable57_setf(uint8_t agcgaintable57)
{
    ASSERT_ERR((((uint32_t)agcgaintable57 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR, (REG_PL_RD(RCG_RWNXAGCGT14_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable57 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE56 field in the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read and the AGCGAINTABLE56 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE56 field in the RWNXAGCGT14 register.
 */
__INLINE uint8_t rcg_agcgaintable56_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT14_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE56 field of the RWNXAGCGT14 register.
 *
 * The RWNXAGCGT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable56 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable56_setf(uint8_t agcgaintable56)
{
    ASSERT_ERR((((uint32_t)agcgaintable56 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT14_ADDR, (REG_PL_RD(RCG_RWNXAGCGT14_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable56 << 0));
}

/// @}

/**
 * @name RWNXAGCGT15 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24       AGCGAINTABLE63   0x20
 *  23:16       AGCGAINTABLE62   0x20
 *  15:08       AGCGAINTABLE61   0x20
 *  07:00       AGCGAINTABLE60   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGT15 register
#define RCG_RWNXAGCGT15_ADDR   (REG_RC_BASE_ADDR+0xC07C)
/// Offset of the RWNXAGCGT15 register from the base address
#define RCG_RWNXAGCGT15_OFFSET 0x0000003C
/// Index of the RWNXAGCGT15 register
#define RCG_RWNXAGCGT15_INDEX  0x0000000F
/// Reset value of the RWNXAGCGT15 register
#define RCG_RWNXAGCGT15_RESET  0x20202020

/**
 * @brief Returns the current value of the RWNXAGCGT15 register.
 * The RWNXAGCGT15 register will be read and its value returned.
 * @return The current value of the RWNXAGCGT15 register.
 */
__INLINE uint32_t rcg_rwnxagcgt15_get(void)
{
    return REG_PL_RD(RCG_RWNXAGCGT15_ADDR);
}

/**
 * @brief Sets the RWNXAGCGT15 register to a value.
 * The RWNXAGCGT15 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcg_rwnxagcgt15_set(uint32_t value)
{
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR, value);
}

// field definitions
/// AGCGAINTABLE63 field mask
#define RCG_AGCGAINTABLE63_MASK   ((uint32_t)0xFF000000)
/// AGCGAINTABLE63 field LSB position
#define RCG_AGCGAINTABLE63_LSB    24
/// AGCGAINTABLE63 field width
#define RCG_AGCGAINTABLE63_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE62 field mask
#define RCG_AGCGAINTABLE62_MASK   ((uint32_t)0x00FF0000)
/// AGCGAINTABLE62 field LSB position
#define RCG_AGCGAINTABLE62_LSB    16
/// AGCGAINTABLE62 field width
#define RCG_AGCGAINTABLE62_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE61 field mask
#define RCG_AGCGAINTABLE61_MASK   ((uint32_t)0x0000FF00)
/// AGCGAINTABLE61 field LSB position
#define RCG_AGCGAINTABLE61_LSB    8
/// AGCGAINTABLE61 field width
#define RCG_AGCGAINTABLE61_WIDTH  ((uint32_t)0x00000008)
/// AGCGAINTABLE60 field mask
#define RCG_AGCGAINTABLE60_MASK   ((uint32_t)0x000000FF)
/// AGCGAINTABLE60 field LSB position
#define RCG_AGCGAINTABLE60_LSB    0
/// AGCGAINTABLE60 field width
#define RCG_AGCGAINTABLE60_WIDTH  ((uint32_t)0x00000008)

/// AGCGAINTABLE63 field reset value
#define RCG_AGCGAINTABLE63_RST    0x20
/// AGCGAINTABLE62 field reset value
#define RCG_AGCGAINTABLE62_RST    0x20
/// AGCGAINTABLE61 field reset value
#define RCG_AGCGAINTABLE61_RST    0x20
/// AGCGAINTABLE60 field reset value
#define RCG_AGCGAINTABLE60_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCGT15 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcgaintable63 - The value to use for the AGCGAINTABLE63 field.
 * @param[in] agcgaintable62 - The value to use for the AGCGAINTABLE62 field.
 * @param[in] agcgaintable61 - The value to use for the AGCGAINTABLE61 field.
 * @param[in] agcgaintable60 - The value to use for the AGCGAINTABLE60 field.
 */
__INLINE void rcg_rwnxagcgt15_pack(uint8_t agcgaintable63, uint8_t agcgaintable62, uint8_t agcgaintable61, uint8_t agcgaintable60)
{
    ASSERT_ERR((((uint32_t)agcgaintable63 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable62 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable61 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)agcgaintable60 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR,  ((uint32_t)agcgaintable63 << 24) | ((uint32_t)agcgaintable62 << 16) | ((uint32_t)agcgaintable61 << 8) | ((uint32_t)agcgaintable60 << 0));
}

/**
 * @brief Unpacks RWNXAGCGT15's fields from current value of the RWNXAGCGT15 register.
 *
 * Reads the RWNXAGCGT15 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgaintable63 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable62 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable61 - Will be populated with the current value of this field from the register.
 * @param[out] agcgaintable60 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcg_rwnxagcgt15_unpack(uint8_t* agcgaintable63, uint8_t* agcgaintable62, uint8_t* agcgaintable61, uint8_t* agcgaintable60)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT15_ADDR);

    *agcgaintable63 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *agcgaintable62 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *agcgaintable61 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgaintable60 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAINTABLE63 field in the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read and the AGCGAINTABLE63 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE63 field in the RWNXAGCGT15 register.
 */
__INLINE uint8_t rcg_agcgaintable63_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT15_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the AGCGAINTABLE63 field of the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable63 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable63_setf(uint8_t agcgaintable63)
{
    ASSERT_ERR((((uint32_t)agcgaintable63 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR, (REG_PL_RD(RCG_RWNXAGCGT15_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)agcgaintable63 << 24));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE62 field in the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read and the AGCGAINTABLE62 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE62 field in the RWNXAGCGT15 register.
 */
__INLINE uint8_t rcg_agcgaintable62_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT15_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the AGCGAINTABLE62 field of the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable62 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable62_setf(uint8_t agcgaintable62)
{
    ASSERT_ERR((((uint32_t)agcgaintable62 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR, (REG_PL_RD(RCG_RWNXAGCGT15_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)agcgaintable62 << 16));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE61 field in the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read and the AGCGAINTABLE61 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE61 field in the RWNXAGCGT15 register.
 */
__INLINE uint8_t rcg_agcgaintable61_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT15_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the AGCGAINTABLE61 field of the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable61 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable61_setf(uint8_t agcgaintable61)
{
    ASSERT_ERR((((uint32_t)agcgaintable61 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR, (REG_PL_RD(RCG_RWNXAGCGT15_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)agcgaintable61 << 8));
}

/**
 * @brief Returns the current value of the AGCGAINTABLE60 field in the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read and the AGCGAINTABLE60 field's value will be returned.
 *
 * @return The current value of the AGCGAINTABLE60 field in the RWNXAGCGT15 register.
 */
__INLINE uint8_t rcg_agcgaintable60_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCG_RWNXAGCGT15_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the AGCGAINTABLE60 field of the RWNXAGCGT15 register.
 *
 * The RWNXAGCGT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcgaintable60 - The value to set the field to.
 */
__INLINE void rcg_agcgaintable60_setf(uint8_t agcgaintable60)
{
    ASSERT_ERR((((uint32_t)agcgaintable60 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RCG_RWNXAGCGT15_ADDR, (REG_PL_RD(RCG_RWNXAGCGT15_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)agcgaintable60 << 0));
}

/// @}


#endif // _REG_RC_GAINTABLE_H_

/// @}

