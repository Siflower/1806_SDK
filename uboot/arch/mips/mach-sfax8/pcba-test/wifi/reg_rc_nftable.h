/**
 * @file reg_rc_nftable.h
 * @brief Definitions of the RCN HW block registers and register access functions.
 *
 * @defgroup REG_RC_NFTABLE REG_RC_NFTABLE
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the RCN HW block registers and register access functions.
 */
#ifndef _REG_RC_NFTABLE_H_
#define _REG_RC_NFTABLE_H_

#include "co_int.h"
#include "_reg_rc.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_RC_NFTABLE peripheral.
 */
#define REG_RC_NFTABLE_COUNT 16

/** @brief Decoding mask of the REG_RC_NFTABLE peripheral registers from the CPU point of view.
 */
#define REG_RC_NFTABLE_DECODING_MASK 0x0000003F

/**
 * @name RWNXAGCNFT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24          AGCNFTABLE3   0x1F
 *  21:16          AGCNFTABLE2   0x1F
 *  13:08          AGCNFTABLE1   0x1F
 *  05:00          AGCNFTABLE0   0x1F
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT0 register
#define RCN_RWNXAGCNFT0_ADDR   (REG_RC_BASE_ADDR+0xC080)
/// Offset of the RWNXAGCNFT0 register from the base address
#define RCN_RWNXAGCNFT0_OFFSET 0x00000000
/// Index of the RWNXAGCNFT0 register
#define RCN_RWNXAGCNFT0_INDEX  0x00000000
/// Reset value of the RWNXAGCNFT0 register
#define RCN_RWNXAGCNFT0_RESET  0x1F1F1F1F

/**
 * @brief Returns the current value of the RWNXAGCNFT0 register.
 * The RWNXAGCNFT0 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT0 register.
 */
__INLINE uint32_t rcn_rwnxagcnft0_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT0 register to a value.
 * The RWNXAGCNFT0 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft0_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR, value);
}

// field definitions
/// AGCNFTABLE3 field mask
#define RCN_AGCNFTABLE3_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE3 field LSB position
#define RCN_AGCNFTABLE3_LSB    24
/// AGCNFTABLE3 field width
#define RCN_AGCNFTABLE3_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE2 field mask
#define RCN_AGCNFTABLE2_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE2 field LSB position
#define RCN_AGCNFTABLE2_LSB    16
/// AGCNFTABLE2 field width
#define RCN_AGCNFTABLE2_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE1 field mask
#define RCN_AGCNFTABLE1_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE1 field LSB position
#define RCN_AGCNFTABLE1_LSB    8
/// AGCNFTABLE1 field width
#define RCN_AGCNFTABLE1_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE0 field mask
#define RCN_AGCNFTABLE0_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE0 field LSB position
#define RCN_AGCNFTABLE0_LSB    0
/// AGCNFTABLE0 field width
#define RCN_AGCNFTABLE0_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE3 field reset value
#define RCN_AGCNFTABLE3_RST    0x1F
/// AGCNFTABLE2 field reset value
#define RCN_AGCNFTABLE2_RST    0x1F
/// AGCNFTABLE1 field reset value
#define RCN_AGCNFTABLE1_RST    0x1F
/// AGCNFTABLE0 field reset value
#define RCN_AGCNFTABLE0_RST    0x1F

/**
 * @brief Constructs a value for the RWNXAGCNFT0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable3 - The value to use for the AGCNFTABLE3 field.
 * @param[in] agcnftable2 - The value to use for the AGCNFTABLE2 field.
 * @param[in] agcnftable1 - The value to use for the AGCNFTABLE1 field.
 * @param[in] agcnftable0 - The value to use for the AGCNFTABLE0 field.
 */
__INLINE void rcn_rwnxagcnft0_pack(uint8_t agcnftable3, uint8_t agcnftable2, uint8_t agcnftable1, uint8_t agcnftable0)
{
    ASSERT_ERR((((uint32_t)agcnftable3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR,  ((uint32_t)agcnftable3 << 24) | ((uint32_t)agcnftable2 << 16) | ((uint32_t)agcnftable1 << 8) | ((uint32_t)agcnftable0 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT0's fields from current value of the RWNXAGCNFT0 register.
 *
 * Reads the RWNXAGCNFT0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable3 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable2 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable1 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable0 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft0_unpack(uint8_t* agcnftable3, uint8_t* agcnftable2, uint8_t* agcnftable1, uint8_t* agcnftable0)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);

    *agcnftable3 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable2 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable1 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable0 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE3 field in the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read and the AGCNFTABLE3 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE3 field in the RWNXAGCNFT0 register.
 */
__INLINE uint8_t rcn_agcnftable3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE3 field of the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable3 - The value to set the field to.
 */
__INLINE void rcn_agcnftable3_setf(uint8_t agcnftable3)
{
    ASSERT_ERR((((uint32_t)agcnftable3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT0_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable3 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE2 field in the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read and the AGCNFTABLE2 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE2 field in the RWNXAGCNFT0 register.
 */
__INLINE uint8_t rcn_agcnftable2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE2 field of the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable2 - The value to set the field to.
 */
__INLINE void rcn_agcnftable2_setf(uint8_t agcnftable2)
{
    ASSERT_ERR((((uint32_t)agcnftable2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT0_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable2 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE1 field in the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read and the AGCNFTABLE1 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE1 field in the RWNXAGCNFT0 register.
 */
__INLINE uint8_t rcn_agcnftable1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE1 field of the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable1 - The value to set the field to.
 */
__INLINE void rcn_agcnftable1_setf(uint8_t agcnftable1)
{
    ASSERT_ERR((((uint32_t)agcnftable1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT0_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable1 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE0 field in the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read and the AGCNFTABLE0 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE0 field in the RWNXAGCNFT0 register.
 */
__INLINE uint8_t rcn_agcnftable0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE0 field of the RWNXAGCNFT0 register.
 *
 * The RWNXAGCNFT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable0 - The value to set the field to.
 */
__INLINE void rcn_agcnftable0_setf(uint8_t agcnftable0)
{
    ASSERT_ERR((((uint32_t)agcnftable0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT0_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT0_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable0 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24          AGCNFTABLE7   0x1F
 *  21:16          AGCNFTABLE6   0x1F
 *  13:08          AGCNFTABLE5   0x1F
 *  05:00          AGCNFTABLE4   0x1F
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT1 register
#define RCN_RWNXAGCNFT1_ADDR   (REG_RC_BASE_ADDR+0xC084)
/// Offset of the RWNXAGCNFT1 register from the base address
#define RCN_RWNXAGCNFT1_OFFSET 0x00000004
/// Index of the RWNXAGCNFT1 register
#define RCN_RWNXAGCNFT1_INDEX  0x00000001
/// Reset value of the RWNXAGCNFT1 register
#define RCN_RWNXAGCNFT1_RESET  0x1F1F1F1F

/**
 * @brief Returns the current value of the RWNXAGCNFT1 register.
 * The RWNXAGCNFT1 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT1 register.
 */
__INLINE uint32_t rcn_rwnxagcnft1_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT1 register to a value.
 * The RWNXAGCNFT1 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft1_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR, value);
}

// field definitions
/// AGCNFTABLE7 field mask
#define RCN_AGCNFTABLE7_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE7 field LSB position
#define RCN_AGCNFTABLE7_LSB    24
/// AGCNFTABLE7 field width
#define RCN_AGCNFTABLE7_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE6 field mask
#define RCN_AGCNFTABLE6_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE6 field LSB position
#define RCN_AGCNFTABLE6_LSB    16
/// AGCNFTABLE6 field width
#define RCN_AGCNFTABLE6_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE5 field mask
#define RCN_AGCNFTABLE5_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE5 field LSB position
#define RCN_AGCNFTABLE5_LSB    8
/// AGCNFTABLE5 field width
#define RCN_AGCNFTABLE5_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE4 field mask
#define RCN_AGCNFTABLE4_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE4 field LSB position
#define RCN_AGCNFTABLE4_LSB    0
/// AGCNFTABLE4 field width
#define RCN_AGCNFTABLE4_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE7 field reset value
#define RCN_AGCNFTABLE7_RST    0x1F
/// AGCNFTABLE6 field reset value
#define RCN_AGCNFTABLE6_RST    0x1F
/// AGCNFTABLE5 field reset value
#define RCN_AGCNFTABLE5_RST    0x1F
/// AGCNFTABLE4 field reset value
#define RCN_AGCNFTABLE4_RST    0x1F

/**
 * @brief Constructs a value for the RWNXAGCNFT1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable7 - The value to use for the AGCNFTABLE7 field.
 * @param[in] agcnftable6 - The value to use for the AGCNFTABLE6 field.
 * @param[in] agcnftable5 - The value to use for the AGCNFTABLE5 field.
 * @param[in] agcnftable4 - The value to use for the AGCNFTABLE4 field.
 */
__INLINE void rcn_rwnxagcnft1_pack(uint8_t agcnftable7, uint8_t agcnftable6, uint8_t agcnftable5, uint8_t agcnftable4)
{
    ASSERT_ERR((((uint32_t)agcnftable7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR,  ((uint32_t)agcnftable7 << 24) | ((uint32_t)agcnftable6 << 16) | ((uint32_t)agcnftable5 << 8) | ((uint32_t)agcnftable4 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT1's fields from current value of the RWNXAGCNFT1 register.
 *
 * Reads the RWNXAGCNFT1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable7 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable6 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable5 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable4 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft1_unpack(uint8_t* agcnftable7, uint8_t* agcnftable6, uint8_t* agcnftable5, uint8_t* agcnftable4)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);

    *agcnftable7 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable6 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable5 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable4 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE7 field in the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read and the AGCNFTABLE7 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE7 field in the RWNXAGCNFT1 register.
 */
__INLINE uint8_t rcn_agcnftable7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE7 field of the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable7 - The value to set the field to.
 */
__INLINE void rcn_agcnftable7_setf(uint8_t agcnftable7)
{
    ASSERT_ERR((((uint32_t)agcnftable7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT1_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable7 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE6 field in the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read and the AGCNFTABLE6 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE6 field in the RWNXAGCNFT1 register.
 */
__INLINE uint8_t rcn_agcnftable6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE6 field of the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable6 - The value to set the field to.
 */
__INLINE void rcn_agcnftable6_setf(uint8_t agcnftable6)
{
    ASSERT_ERR((((uint32_t)agcnftable6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT1_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable6 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE5 field in the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read and the AGCNFTABLE5 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE5 field in the RWNXAGCNFT1 register.
 */
__INLINE uint8_t rcn_agcnftable5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE5 field of the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable5 - The value to set the field to.
 */
__INLINE void rcn_agcnftable5_setf(uint8_t agcnftable5)
{
    ASSERT_ERR((((uint32_t)agcnftable5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT1_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable5 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE4 field in the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read and the AGCNFTABLE4 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE4 field in the RWNXAGCNFT1 register.
 */
__INLINE uint8_t rcn_agcnftable4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT1_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE4 field of the RWNXAGCNFT1 register.
 *
 * The RWNXAGCNFT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable4 - The value to set the field to.
 */
__INLINE void rcn_agcnftable4_setf(uint8_t agcnftable4)
{
    ASSERT_ERR((((uint32_t)agcnftable4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT1_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT1_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable4 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE11   0x1C
 *  21:16         AGCNFTABLE10   0x1D
 *  13:08          AGCNFTABLE9   0x1E
 *  05:00          AGCNFTABLE8   0x1F
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT2 register
#define RCN_RWNXAGCNFT2_ADDR   (REG_RC_BASE_ADDR+0xC088)
/// Offset of the RWNXAGCNFT2 register from the base address
#define RCN_RWNXAGCNFT2_OFFSET 0x00000008
/// Index of the RWNXAGCNFT2 register
#define RCN_RWNXAGCNFT2_INDEX  0x00000002
/// Reset value of the RWNXAGCNFT2 register
#define RCN_RWNXAGCNFT2_RESET  0x1C1D1E1F

/**
 * @brief Returns the current value of the RWNXAGCNFT2 register.
 * The RWNXAGCNFT2 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT2 register.
 */
__INLINE uint32_t rcn_rwnxagcnft2_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT2 register to a value.
 * The RWNXAGCNFT2 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft2_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR, value);
}

// field definitions
/// AGCNFTABLE11 field mask
#define RCN_AGCNFTABLE11_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE11 field LSB position
#define RCN_AGCNFTABLE11_LSB    24
/// AGCNFTABLE11 field width
#define RCN_AGCNFTABLE11_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE10 field mask
#define RCN_AGCNFTABLE10_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE10 field LSB position
#define RCN_AGCNFTABLE10_LSB    16
/// AGCNFTABLE10 field width
#define RCN_AGCNFTABLE10_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE9 field mask
#define RCN_AGCNFTABLE9_MASK    ((uint32_t)0x00003F00)
/// AGCNFTABLE9 field LSB position
#define RCN_AGCNFTABLE9_LSB     8
/// AGCNFTABLE9 field width
#define RCN_AGCNFTABLE9_WIDTH   ((uint32_t)0x00000006)
/// AGCNFTABLE8 field mask
#define RCN_AGCNFTABLE8_MASK    ((uint32_t)0x0000003F)
/// AGCNFTABLE8 field LSB position
#define RCN_AGCNFTABLE8_LSB     0
/// AGCNFTABLE8 field width
#define RCN_AGCNFTABLE8_WIDTH   ((uint32_t)0x00000006)

/// AGCNFTABLE11 field reset value
#define RCN_AGCNFTABLE11_RST    0x1C
/// AGCNFTABLE10 field reset value
#define RCN_AGCNFTABLE10_RST    0x1D
/// AGCNFTABLE9 field reset value
#define RCN_AGCNFTABLE9_RST     0x1E
/// AGCNFTABLE8 field reset value
#define RCN_AGCNFTABLE8_RST     0x1F

/**
 * @brief Constructs a value for the RWNXAGCNFT2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable11 - The value to use for the AGCNFTABLE11 field.
 * @param[in] agcnftable10 - The value to use for the AGCNFTABLE10 field.
 * @param[in] agcnftable9 - The value to use for the AGCNFTABLE9 field.
 * @param[in] agcnftable8 - The value to use for the AGCNFTABLE8 field.
 */
__INLINE void rcn_rwnxagcnft2_pack(uint8_t agcnftable11, uint8_t agcnftable10, uint8_t agcnftable9, uint8_t agcnftable8)
{
    ASSERT_ERR((((uint32_t)agcnftable11 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable10 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR,  ((uint32_t)agcnftable11 << 24) | ((uint32_t)agcnftable10 << 16) | ((uint32_t)agcnftable9 << 8) | ((uint32_t)agcnftable8 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT2's fields from current value of the RWNXAGCNFT2 register.
 *
 * Reads the RWNXAGCNFT2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable11 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable10 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable9 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable8 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft2_unpack(uint8_t* agcnftable11, uint8_t* agcnftable10, uint8_t* agcnftable9, uint8_t* agcnftable8)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);

    *agcnftable11 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable10 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable9 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable8 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE11 field in the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read and the AGCNFTABLE11 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE11 field in the RWNXAGCNFT2 register.
 */
__INLINE uint8_t rcn_agcnftable11_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE11 field of the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable11 - The value to set the field to.
 */
__INLINE void rcn_agcnftable11_setf(uint8_t agcnftable11)
{
    ASSERT_ERR((((uint32_t)agcnftable11 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT2_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable11 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE10 field in the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read and the AGCNFTABLE10 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE10 field in the RWNXAGCNFT2 register.
 */
__INLINE uint8_t rcn_agcnftable10_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE10 field of the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable10 - The value to set the field to.
 */
__INLINE void rcn_agcnftable10_setf(uint8_t agcnftable10)
{
    ASSERT_ERR((((uint32_t)agcnftable10 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT2_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable10 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE9 field in the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read and the AGCNFTABLE9 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE9 field in the RWNXAGCNFT2 register.
 */
__INLINE uint8_t rcn_agcnftable9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE9 field of the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable9 - The value to set the field to.
 */
__INLINE void rcn_agcnftable9_setf(uint8_t agcnftable9)
{
    ASSERT_ERR((((uint32_t)agcnftable9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT2_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable9 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE8 field in the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read and the AGCNFTABLE8 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE8 field in the RWNXAGCNFT2 register.
 */
__INLINE uint8_t rcn_agcnftable8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT2_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE8 field of the RWNXAGCNFT2 register.
 *
 * The RWNXAGCNFT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable8 - The value to set the field to.
 */
__INLINE void rcn_agcnftable8_setf(uint8_t agcnftable8)
{
    ASSERT_ERR((((uint32_t)agcnftable8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT2_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT2_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable8 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT3 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE15   0x1A
 *  21:16         AGCNFTABLE14   0x13
 *  13:08         AGCNFTABLE13   0x1B
 *  05:00         AGCNFTABLE12   0x1C
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT3 register
#define RCN_RWNXAGCNFT3_ADDR   (REG_RC_BASE_ADDR+0xC08C)
/// Offset of the RWNXAGCNFT3 register from the base address
#define RCN_RWNXAGCNFT3_OFFSET 0x0000000C
/// Index of the RWNXAGCNFT3 register
#define RCN_RWNXAGCNFT3_INDEX  0x00000003
/// Reset value of the RWNXAGCNFT3 register
//#define RCN_RWNXAGCNFT3_RESET  0x1A131B1C
#define RCN_RWNXAGCNFT3_RESET  0x1A1B1B1C

/**
 * @brief Returns the current value of the RWNXAGCNFT3 register.
 * The RWNXAGCNFT3 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT3 register.
 */
__INLINE uint32_t rcn_rwnxagcnft3_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT3 register to a value.
 * The RWNXAGCNFT3 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft3_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR, value);
}

// field definitions
/// AGCNFTABLE15 field mask
#define RCN_AGCNFTABLE15_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE15 field LSB position
#define RCN_AGCNFTABLE15_LSB    24
/// AGCNFTABLE15 field width
#define RCN_AGCNFTABLE15_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE14 field mask
#define RCN_AGCNFTABLE14_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE14 field LSB position
#define RCN_AGCNFTABLE14_LSB    16
/// AGCNFTABLE14 field width
#define RCN_AGCNFTABLE14_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE13 field mask
#define RCN_AGCNFTABLE13_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE13 field LSB position
#define RCN_AGCNFTABLE13_LSB    8
/// AGCNFTABLE13 field width
#define RCN_AGCNFTABLE13_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE12 field mask
#define RCN_AGCNFTABLE12_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE12 field LSB position
#define RCN_AGCNFTABLE12_LSB    0
/// AGCNFTABLE12 field width
#define RCN_AGCNFTABLE12_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE15 field reset value
#define RCN_AGCNFTABLE15_RST    0x1A
/// AGCNFTABLE14 field reset value
#define RCN_AGCNFTABLE14_RST    0x13
/// AGCNFTABLE13 field reset value
#define RCN_AGCNFTABLE13_RST    0x1B
/// AGCNFTABLE12 field reset value
#define RCN_AGCNFTABLE12_RST    0x1C

/**
 * @brief Constructs a value for the RWNXAGCNFT3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable15 - The value to use for the AGCNFTABLE15 field.
 * @param[in] agcnftable14 - The value to use for the AGCNFTABLE14 field.
 * @param[in] agcnftable13 - The value to use for the AGCNFTABLE13 field.
 * @param[in] agcnftable12 - The value to use for the AGCNFTABLE12 field.
 */
__INLINE void rcn_rwnxagcnft3_pack(uint8_t agcnftable15, uint8_t agcnftable14, uint8_t agcnftable13, uint8_t agcnftable12)
{
    ASSERT_ERR((((uint32_t)agcnftable15 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable14 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable13 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable12 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR,  ((uint32_t)agcnftable15 << 24) | ((uint32_t)agcnftable14 << 16) | ((uint32_t)agcnftable13 << 8) | ((uint32_t)agcnftable12 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT3's fields from current value of the RWNXAGCNFT3 register.
 *
 * Reads the RWNXAGCNFT3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable15 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable14 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable13 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable12 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft3_unpack(uint8_t* agcnftable15, uint8_t* agcnftable14, uint8_t* agcnftable13, uint8_t* agcnftable12)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);

    *agcnftable15 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable14 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable13 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable12 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE15 field in the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read and the AGCNFTABLE15 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE15 field in the RWNXAGCNFT3 register.
 */
__INLINE uint8_t rcn_agcnftable15_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE15 field of the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable15 - The value to set the field to.
 */
__INLINE void rcn_agcnftable15_setf(uint8_t agcnftable15)
{
    ASSERT_ERR((((uint32_t)agcnftable15 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT3_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable15 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE14 field in the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read and the AGCNFTABLE14 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE14 field in the RWNXAGCNFT3 register.
 */
__INLINE uint8_t rcn_agcnftable14_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE14 field of the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable14 - The value to set the field to.
 */
__INLINE void rcn_agcnftable14_setf(uint8_t agcnftable14)
{
    ASSERT_ERR((((uint32_t)agcnftable14 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT3_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable14 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE13 field in the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read and the AGCNFTABLE13 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE13 field in the RWNXAGCNFT3 register.
 */
__INLINE uint8_t rcn_agcnftable13_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE13 field of the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable13 - The value to set the field to.
 */
__INLINE void rcn_agcnftable13_setf(uint8_t agcnftable13)
{
    ASSERT_ERR((((uint32_t)agcnftable13 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT3_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable13 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE12 field in the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read and the AGCNFTABLE12 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE12 field in the RWNXAGCNFT3 register.
 */
__INLINE uint8_t rcn_agcnftable12_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT3_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE12 field of the RWNXAGCNFT3 register.
 *
 * The RWNXAGCNFT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable12 - The value to set the field to.
 */
__INLINE void rcn_agcnftable12_setf(uint8_t agcnftable12)
{
    ASSERT_ERR((((uint32_t)agcnftable12 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT3_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT3_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable12 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT4 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE19   0x17
 *  21:16         AGCNFTABLE18   0x18
 *  13:08         AGCNFTABLE17   0x19
 *  05:00         AGCNFTABLE16   0x1A
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT4 register
#define RCN_RWNXAGCNFT4_ADDR   (REG_RC_BASE_ADDR+0xC090)
/// Offset of the RWNXAGCNFT4 register from the base address
#define RCN_RWNXAGCNFT4_OFFSET 0x00000010
/// Index of the RWNXAGCNFT4 register
#define RCN_RWNXAGCNFT4_INDEX  0x00000004
/// Reset value of the RWNXAGCNFT4 register
#define RCN_RWNXAGCNFT4_RESET  0x1718191A

/**
 * @brief Returns the current value of the RWNXAGCNFT4 register.
 * The RWNXAGCNFT4 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT4 register.
 */
__INLINE uint32_t rcn_rwnxagcnft4_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT4 register to a value.
 * The RWNXAGCNFT4 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft4_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR, value);
}

// field definitions
/// AGCNFTABLE19 field mask
#define RCN_AGCNFTABLE19_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE19 field LSB position
#define RCN_AGCNFTABLE19_LSB    24
/// AGCNFTABLE19 field width
#define RCN_AGCNFTABLE19_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE18 field mask
#define RCN_AGCNFTABLE18_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE18 field LSB position
#define RCN_AGCNFTABLE18_LSB    16
/// AGCNFTABLE18 field width
#define RCN_AGCNFTABLE18_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE17 field mask
#define RCN_AGCNFTABLE17_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE17 field LSB position
#define RCN_AGCNFTABLE17_LSB    8
/// AGCNFTABLE17 field width
#define RCN_AGCNFTABLE17_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE16 field mask
#define RCN_AGCNFTABLE16_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE16 field LSB position
#define RCN_AGCNFTABLE16_LSB    0
/// AGCNFTABLE16 field width
#define RCN_AGCNFTABLE16_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE19 field reset value
#define RCN_AGCNFTABLE19_RST    0x17
/// AGCNFTABLE18 field reset value
#define RCN_AGCNFTABLE18_RST    0x18
/// AGCNFTABLE17 field reset value
#define RCN_AGCNFTABLE17_RST    0x19
/// AGCNFTABLE16 field reset value
#define RCN_AGCNFTABLE16_RST    0x1A

/**
 * @brief Constructs a value for the RWNXAGCNFT4 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable19 - The value to use for the AGCNFTABLE19 field.
 * @param[in] agcnftable18 - The value to use for the AGCNFTABLE18 field.
 * @param[in] agcnftable17 - The value to use for the AGCNFTABLE17 field.
 * @param[in] agcnftable16 - The value to use for the AGCNFTABLE16 field.
 */
__INLINE void rcn_rwnxagcnft4_pack(uint8_t agcnftable19, uint8_t agcnftable18, uint8_t agcnftable17, uint8_t agcnftable16)
{
    ASSERT_ERR((((uint32_t)agcnftable19 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable18 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable17 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable16 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR,  ((uint32_t)agcnftable19 << 24) | ((uint32_t)agcnftable18 << 16) | ((uint32_t)agcnftable17 << 8) | ((uint32_t)agcnftable16 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT4's fields from current value of the RWNXAGCNFT4 register.
 *
 * Reads the RWNXAGCNFT4 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable19 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable18 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable17 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable16 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft4_unpack(uint8_t* agcnftable19, uint8_t* agcnftable18, uint8_t* agcnftable17, uint8_t* agcnftable16)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);

    *agcnftable19 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable18 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable17 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable16 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE19 field in the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read and the AGCNFTABLE19 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE19 field in the RWNXAGCNFT4 register.
 */
__INLINE uint8_t rcn_agcnftable19_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE19 field of the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable19 - The value to set the field to.
 */
__INLINE void rcn_agcnftable19_setf(uint8_t agcnftable19)
{
    ASSERT_ERR((((uint32_t)agcnftable19 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT4_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable19 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE18 field in the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read and the AGCNFTABLE18 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE18 field in the RWNXAGCNFT4 register.
 */
__INLINE uint8_t rcn_agcnftable18_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE18 field of the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable18 - The value to set the field to.
 */
__INLINE void rcn_agcnftable18_setf(uint8_t agcnftable18)
{
    ASSERT_ERR((((uint32_t)agcnftable18 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT4_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable18 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE17 field in the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read and the AGCNFTABLE17 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE17 field in the RWNXAGCNFT4 register.
 */
__INLINE uint8_t rcn_agcnftable17_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE17 field of the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable17 - The value to set the field to.
 */
__INLINE void rcn_agcnftable17_setf(uint8_t agcnftable17)
{
    ASSERT_ERR((((uint32_t)agcnftable17 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT4_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable17 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE16 field in the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read and the AGCNFTABLE16 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE16 field in the RWNXAGCNFT4 register.
 */
__INLINE uint8_t rcn_agcnftable16_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT4_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE16 field of the RWNXAGCNFT4 register.
 *
 * The RWNXAGCNFT4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable16 - The value to set the field to.
 */
__INLINE void rcn_agcnftable16_setf(uint8_t agcnftable16)
{
    ASSERT_ERR((((uint32_t)agcnftable16 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT4_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT4_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable16 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT5 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE23   0x14
 *  21:16         AGCNFTABLE22   0x15
 *  13:08         AGCNFTABLE21   0x16
 *  05:00         AGCNFTABLE20   0x16
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT5 register
#define RCN_RWNXAGCNFT5_ADDR   (REG_RC_BASE_ADDR+0xC094)
/// Offset of the RWNXAGCNFT5 register from the base address
#define RCN_RWNXAGCNFT5_OFFSET 0x00000014
/// Index of the RWNXAGCNFT5 register
#define RCN_RWNXAGCNFT5_INDEX  0x00000005
/// Reset value of the RWNXAGCNFT5 register
#define RCN_RWNXAGCNFT5_RESET  0x14151616

/**
 * @brief Returns the current value of the RWNXAGCNFT5 register.
 * The RWNXAGCNFT5 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT5 register.
 */
__INLINE uint32_t rcn_rwnxagcnft5_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT5 register to a value.
 * The RWNXAGCNFT5 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft5_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR, value);
}

// field definitions
/// AGCNFTABLE23 field mask
#define RCN_AGCNFTABLE23_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE23 field LSB position
#define RCN_AGCNFTABLE23_LSB    24
/// AGCNFTABLE23 field width
#define RCN_AGCNFTABLE23_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE22 field mask
#define RCN_AGCNFTABLE22_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE22 field LSB position
#define RCN_AGCNFTABLE22_LSB    16
/// AGCNFTABLE22 field width
#define RCN_AGCNFTABLE22_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE21 field mask
#define RCN_AGCNFTABLE21_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE21 field LSB position
#define RCN_AGCNFTABLE21_LSB    8
/// AGCNFTABLE21 field width
#define RCN_AGCNFTABLE21_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE20 field mask
#define RCN_AGCNFTABLE20_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE20 field LSB position
#define RCN_AGCNFTABLE20_LSB    0
/// AGCNFTABLE20 field width
#define RCN_AGCNFTABLE20_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE23 field reset value
#define RCN_AGCNFTABLE23_RST    0x14
/// AGCNFTABLE22 field reset value
#define RCN_AGCNFTABLE22_RST    0x15
/// AGCNFTABLE21 field reset value
#define RCN_AGCNFTABLE21_RST    0x16
/// AGCNFTABLE20 field reset value
#define RCN_AGCNFTABLE20_RST    0x16

/**
 * @brief Constructs a value for the RWNXAGCNFT5 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable23 - The value to use for the AGCNFTABLE23 field.
 * @param[in] agcnftable22 - The value to use for the AGCNFTABLE22 field.
 * @param[in] agcnftable21 - The value to use for the AGCNFTABLE21 field.
 * @param[in] agcnftable20 - The value to use for the AGCNFTABLE20 field.
 */
__INLINE void rcn_rwnxagcnft5_pack(uint8_t agcnftable23, uint8_t agcnftable22, uint8_t agcnftable21, uint8_t agcnftable20)
{
    ASSERT_ERR((((uint32_t)agcnftable23 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable22 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable21 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable20 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR,  ((uint32_t)agcnftable23 << 24) | ((uint32_t)agcnftable22 << 16) | ((uint32_t)agcnftable21 << 8) | ((uint32_t)agcnftable20 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT5's fields from current value of the RWNXAGCNFT5 register.
 *
 * Reads the RWNXAGCNFT5 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable23 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable22 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable21 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable20 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft5_unpack(uint8_t* agcnftable23, uint8_t* agcnftable22, uint8_t* agcnftable21, uint8_t* agcnftable20)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);

    *agcnftable23 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable22 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable21 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable20 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE23 field in the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read and the AGCNFTABLE23 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE23 field in the RWNXAGCNFT5 register.
 */
__INLINE uint8_t rcn_agcnftable23_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE23 field of the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable23 - The value to set the field to.
 */
__INLINE void rcn_agcnftable23_setf(uint8_t agcnftable23)
{
    ASSERT_ERR((((uint32_t)agcnftable23 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT5_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable23 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE22 field in the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read and the AGCNFTABLE22 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE22 field in the RWNXAGCNFT5 register.
 */
__INLINE uint8_t rcn_agcnftable22_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE22 field of the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable22 - The value to set the field to.
 */
__INLINE void rcn_agcnftable22_setf(uint8_t agcnftable22)
{
    ASSERT_ERR((((uint32_t)agcnftable22 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT5_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable22 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE21 field in the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read and the AGCNFTABLE21 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE21 field in the RWNXAGCNFT5 register.
 */
__INLINE uint8_t rcn_agcnftable21_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE21 field of the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable21 - The value to set the field to.
 */
__INLINE void rcn_agcnftable21_setf(uint8_t agcnftable21)
{
    ASSERT_ERR((((uint32_t)agcnftable21 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT5_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable21 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE20 field in the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read and the AGCNFTABLE20 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE20 field in the RWNXAGCNFT5 register.
 */
__INLINE uint8_t rcn_agcnftable20_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT5_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE20 field of the RWNXAGCNFT5 register.
 *
 * The RWNXAGCNFT5 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable20 - The value to set the field to.
 */
__INLINE void rcn_agcnftable20_setf(uint8_t agcnftable20)
{
    ASSERT_ERR((((uint32_t)agcnftable20 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT5_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT5_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable20 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT6 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE27   0xF
 *  21:16         AGCNFTABLE26   0x11
 *  13:08         AGCNFTABLE25   0x12
 *  05:00         AGCNFTABLE24   0x13
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT6 register
#define RCN_RWNXAGCNFT6_ADDR   (REG_RC_BASE_ADDR+0xC098)
/// Offset of the RWNXAGCNFT6 register from the base address
#define RCN_RWNXAGCNFT6_OFFSET 0x00000018
/// Index of the RWNXAGCNFT6 register
#define RCN_RWNXAGCNFT6_INDEX  0x00000006
/// Reset value of the RWNXAGCNFT6 register
#define RCN_RWNXAGCNFT6_RESET  0x0F111213

/**
 * @brief Returns the current value of the RWNXAGCNFT6 register.
 * The RWNXAGCNFT6 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT6 register.
 */
__INLINE uint32_t rcn_rwnxagcnft6_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT6 register to a value.
 * The RWNXAGCNFT6 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft6_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR, value);
}

// field definitions
/// AGCNFTABLE27 field mask
#define RCN_AGCNFTABLE27_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE27 field LSB position
#define RCN_AGCNFTABLE27_LSB    24
/// AGCNFTABLE27 field width
#define RCN_AGCNFTABLE27_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE26 field mask
#define RCN_AGCNFTABLE26_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE26 field LSB position
#define RCN_AGCNFTABLE26_LSB    16
/// AGCNFTABLE26 field width
#define RCN_AGCNFTABLE26_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE25 field mask
#define RCN_AGCNFTABLE25_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE25 field LSB position
#define RCN_AGCNFTABLE25_LSB    8
/// AGCNFTABLE25 field width
#define RCN_AGCNFTABLE25_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE24 field mask
#define RCN_AGCNFTABLE24_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE24 field LSB position
#define RCN_AGCNFTABLE24_LSB    0
/// AGCNFTABLE24 field width
#define RCN_AGCNFTABLE24_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE27 field reset value
#define RCN_AGCNFTABLE27_RST    0xF
/// AGCNFTABLE26 field reset value
#define RCN_AGCNFTABLE26_RST    0x11
/// AGCNFTABLE25 field reset value
#define RCN_AGCNFTABLE25_RST    0x12
/// AGCNFTABLE24 field reset value
#define RCN_AGCNFTABLE24_RST    0x13

/**
 * @brief Constructs a value for the RWNXAGCNFT6 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable27 - The value to use for the AGCNFTABLE27 field.
 * @param[in] agcnftable26 - The value to use for the AGCNFTABLE26 field.
 * @param[in] agcnftable25 - The value to use for the AGCNFTABLE25 field.
 * @param[in] agcnftable24 - The value to use for the AGCNFTABLE24 field.
 */
__INLINE void rcn_rwnxagcnft6_pack(uint8_t agcnftable27, uint8_t agcnftable26, uint8_t agcnftable25, uint8_t agcnftable24)
{
    ASSERT_ERR((((uint32_t)agcnftable27 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable26 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable25 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable24 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR,  ((uint32_t)agcnftable27 << 24) | ((uint32_t)agcnftable26 << 16) | ((uint32_t)agcnftable25 << 8) | ((uint32_t)agcnftable24 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT6's fields from current value of the RWNXAGCNFT6 register.
 *
 * Reads the RWNXAGCNFT6 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable27 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable26 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable25 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable24 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft6_unpack(uint8_t* agcnftable27, uint8_t* agcnftable26, uint8_t* agcnftable25, uint8_t* agcnftable24)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);

    *agcnftable27 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable26 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable25 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable24 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE27 field in the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read and the AGCNFTABLE27 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE27 field in the RWNXAGCNFT6 register.
 */
__INLINE uint8_t rcn_agcnftable27_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE27 field of the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable27 - The value to set the field to.
 */
__INLINE void rcn_agcnftable27_setf(uint8_t agcnftable27)
{
    ASSERT_ERR((((uint32_t)agcnftable27 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT6_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable27 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE26 field in the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read and the AGCNFTABLE26 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE26 field in the RWNXAGCNFT6 register.
 */
__INLINE uint8_t rcn_agcnftable26_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE26 field of the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable26 - The value to set the field to.
 */
__INLINE void rcn_agcnftable26_setf(uint8_t agcnftable26)
{
    ASSERT_ERR((((uint32_t)agcnftable26 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT6_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable26 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE25 field in the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read and the AGCNFTABLE25 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE25 field in the RWNXAGCNFT6 register.
 */
__INLINE uint8_t rcn_agcnftable25_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE25 field of the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable25 - The value to set the field to.
 */
__INLINE void rcn_agcnftable25_setf(uint8_t agcnftable25)
{
    ASSERT_ERR((((uint32_t)agcnftable25 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT6_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable25 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE24 field in the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read and the AGCNFTABLE24 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE24 field in the RWNXAGCNFT6 register.
 */
__INLINE uint8_t rcn_agcnftable24_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT6_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE24 field of the RWNXAGCNFT6 register.
 *
 * The RWNXAGCNFT6 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable24 - The value to set the field to.
 */
__INLINE void rcn_agcnftable24_setf(uint8_t agcnftable24)
{
    ASSERT_ERR((((uint32_t)agcnftable24 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT6_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT6_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable24 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT7 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE31   0xB
 *  21:16         AGCNFTABLE30   0xC
 *  13:08         AGCNFTABLE29   0xC
 *  05:00         AGCNFTABLE28   0xD
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT7 register
#define RCN_RWNXAGCNFT7_ADDR   (REG_RC_BASE_ADDR+0xC09C)
/// Offset of the RWNXAGCNFT7 register from the base address
#define RCN_RWNXAGCNFT7_OFFSET 0x0000001C
/// Index of the RWNXAGCNFT7 register
#define RCN_RWNXAGCNFT7_INDEX  0x00000007
/// Reset value of the RWNXAGCNFT7 register
#define RCN_RWNXAGCNFT7_RESET  0x0B0C0C0D

/**
 * @brief Returns the current value of the RWNXAGCNFT7 register.
 * The RWNXAGCNFT7 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT7 register.
 */
__INLINE uint32_t rcn_rwnxagcnft7_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT7 register to a value.
 * The RWNXAGCNFT7 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft7_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR, value);
}

// field definitions
/// AGCNFTABLE31 field mask
#define RCN_AGCNFTABLE31_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE31 field LSB position
#define RCN_AGCNFTABLE31_LSB    24
/// AGCNFTABLE31 field width
#define RCN_AGCNFTABLE31_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE30 field mask
#define RCN_AGCNFTABLE30_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE30 field LSB position
#define RCN_AGCNFTABLE30_LSB    16
/// AGCNFTABLE30 field width
#define RCN_AGCNFTABLE30_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE29 field mask
#define RCN_AGCNFTABLE29_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE29 field LSB position
#define RCN_AGCNFTABLE29_LSB    8
/// AGCNFTABLE29 field width
#define RCN_AGCNFTABLE29_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE28 field mask
#define RCN_AGCNFTABLE28_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE28 field LSB position
#define RCN_AGCNFTABLE28_LSB    0
/// AGCNFTABLE28 field width
#define RCN_AGCNFTABLE28_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE31 field reset value
#define RCN_AGCNFTABLE31_RST    0xB
/// AGCNFTABLE30 field reset value
#define RCN_AGCNFTABLE30_RST    0xC
/// AGCNFTABLE29 field reset value
#define RCN_AGCNFTABLE29_RST    0xC
/// AGCNFTABLE28 field reset value
#define RCN_AGCNFTABLE28_RST    0xD

/**
 * @brief Constructs a value for the RWNXAGCNFT7 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable31 - The value to use for the AGCNFTABLE31 field.
 * @param[in] agcnftable30 - The value to use for the AGCNFTABLE30 field.
 * @param[in] agcnftable29 - The value to use for the AGCNFTABLE29 field.
 * @param[in] agcnftable28 - The value to use for the AGCNFTABLE28 field.
 */
__INLINE void rcn_rwnxagcnft7_pack(uint8_t agcnftable31, uint8_t agcnftable30, uint8_t agcnftable29, uint8_t agcnftable28)
{
    ASSERT_ERR((((uint32_t)agcnftable31 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable30 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable29 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable28 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR,  ((uint32_t)agcnftable31 << 24) | ((uint32_t)agcnftable30 << 16) | ((uint32_t)agcnftable29 << 8) | ((uint32_t)agcnftable28 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT7's fields from current value of the RWNXAGCNFT7 register.
 *
 * Reads the RWNXAGCNFT7 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable31 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable30 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable29 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable28 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft7_unpack(uint8_t* agcnftable31, uint8_t* agcnftable30, uint8_t* agcnftable29, uint8_t* agcnftable28)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);

    *agcnftable31 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable30 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable29 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable28 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE31 field in the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read and the AGCNFTABLE31 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE31 field in the RWNXAGCNFT7 register.
 */
__INLINE uint8_t rcn_agcnftable31_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE31 field of the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable31 - The value to set the field to.
 */
__INLINE void rcn_agcnftable31_setf(uint8_t agcnftable31)
{
    ASSERT_ERR((((uint32_t)agcnftable31 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT7_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable31 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE30 field in the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read and the AGCNFTABLE30 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE30 field in the RWNXAGCNFT7 register.
 */
__INLINE uint8_t rcn_agcnftable30_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE30 field of the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable30 - The value to set the field to.
 */
__INLINE void rcn_agcnftable30_setf(uint8_t agcnftable30)
{
    ASSERT_ERR((((uint32_t)agcnftable30 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT7_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable30 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE29 field in the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read and the AGCNFTABLE29 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE29 field in the RWNXAGCNFT7 register.
 */
__INLINE uint8_t rcn_agcnftable29_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE29 field of the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable29 - The value to set the field to.
 */
__INLINE void rcn_agcnftable29_setf(uint8_t agcnftable29)
{
    ASSERT_ERR((((uint32_t)agcnftable29 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT7_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable29 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE28 field in the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read and the AGCNFTABLE28 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE28 field in the RWNXAGCNFT7 register.
 */
__INLINE uint8_t rcn_agcnftable28_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT7_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE28 field of the RWNXAGCNFT7 register.
 *
 * The RWNXAGCNFT7 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable28 - The value to set the field to.
 */
__INLINE void rcn_agcnftable28_setf(uint8_t agcnftable28)
{
    ASSERT_ERR((((uint32_t)agcnftable28 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT7_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT7_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable28 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT8 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE35   0x9
 *  21:16         AGCNFTABLE34   0x9
 *  13:08         AGCNFTABLE33   0xA
 *  05:00         AGCNFTABLE32   0xB
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT8 register
#define RCN_RWNXAGCNFT8_ADDR   (REG_RC_BASE_ADDR+0xC0A0)
/// Offset of the RWNXAGCNFT8 register from the base address
#define RCN_RWNXAGCNFT8_OFFSET 0x00000020
/// Index of the RWNXAGCNFT8 register
#define RCN_RWNXAGCNFT8_INDEX  0x00000008
/// Reset value of the RWNXAGCNFT8 register
#define RCN_RWNXAGCNFT8_RESET  0x09090A0B

/**
 * @brief Returns the current value of the RWNXAGCNFT8 register.
 * The RWNXAGCNFT8 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT8 register.
 */
__INLINE uint32_t rcn_rwnxagcnft8_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT8 register to a value.
 * The RWNXAGCNFT8 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft8_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR, value);
}

// field definitions
/// AGCNFTABLE35 field mask
#define RCN_AGCNFTABLE35_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE35 field LSB position
#define RCN_AGCNFTABLE35_LSB    24
/// AGCNFTABLE35 field width
#define RCN_AGCNFTABLE35_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE34 field mask
#define RCN_AGCNFTABLE34_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE34 field LSB position
#define RCN_AGCNFTABLE34_LSB    16
/// AGCNFTABLE34 field width
#define RCN_AGCNFTABLE34_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE33 field mask
#define RCN_AGCNFTABLE33_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE33 field LSB position
#define RCN_AGCNFTABLE33_LSB    8
/// AGCNFTABLE33 field width
#define RCN_AGCNFTABLE33_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE32 field mask
#define RCN_AGCNFTABLE32_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE32 field LSB position
#define RCN_AGCNFTABLE32_LSB    0
/// AGCNFTABLE32 field width
#define RCN_AGCNFTABLE32_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE35 field reset value
#define RCN_AGCNFTABLE35_RST    0x9
/// AGCNFTABLE34 field reset value
#define RCN_AGCNFTABLE34_RST    0x9
/// AGCNFTABLE33 field reset value
#define RCN_AGCNFTABLE33_RST    0xA
/// AGCNFTABLE32 field reset value
#define RCN_AGCNFTABLE32_RST    0xB

/**
 * @brief Constructs a value for the RWNXAGCNFT8 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable35 - The value to use for the AGCNFTABLE35 field.
 * @param[in] agcnftable34 - The value to use for the AGCNFTABLE34 field.
 * @param[in] agcnftable33 - The value to use for the AGCNFTABLE33 field.
 * @param[in] agcnftable32 - The value to use for the AGCNFTABLE32 field.
 */
__INLINE void rcn_rwnxagcnft8_pack(uint8_t agcnftable35, uint8_t agcnftable34, uint8_t agcnftable33, uint8_t agcnftable32)
{
    ASSERT_ERR((((uint32_t)agcnftable35 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable34 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable33 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable32 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR,  ((uint32_t)agcnftable35 << 24) | ((uint32_t)agcnftable34 << 16) | ((uint32_t)agcnftable33 << 8) | ((uint32_t)agcnftable32 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT8's fields from current value of the RWNXAGCNFT8 register.
 *
 * Reads the RWNXAGCNFT8 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable35 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable34 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable33 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable32 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft8_unpack(uint8_t* agcnftable35, uint8_t* agcnftable34, uint8_t* agcnftable33, uint8_t* agcnftable32)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);

    *agcnftable35 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable34 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable33 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable32 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE35 field in the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read and the AGCNFTABLE35 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE35 field in the RWNXAGCNFT8 register.
 */
__INLINE uint8_t rcn_agcnftable35_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE35 field of the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable35 - The value to set the field to.
 */
__INLINE void rcn_agcnftable35_setf(uint8_t agcnftable35)
{
    ASSERT_ERR((((uint32_t)agcnftable35 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT8_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable35 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE34 field in the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read and the AGCNFTABLE34 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE34 field in the RWNXAGCNFT8 register.
 */
__INLINE uint8_t rcn_agcnftable34_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE34 field of the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable34 - The value to set the field to.
 */
__INLINE void rcn_agcnftable34_setf(uint8_t agcnftable34)
{
    ASSERT_ERR((((uint32_t)agcnftable34 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT8_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable34 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE33 field in the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read and the AGCNFTABLE33 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE33 field in the RWNXAGCNFT8 register.
 */
__INLINE uint8_t rcn_agcnftable33_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE33 field of the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable33 - The value to set the field to.
 */
__INLINE void rcn_agcnftable33_setf(uint8_t agcnftable33)
{
    ASSERT_ERR((((uint32_t)agcnftable33 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT8_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable33 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE32 field in the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read and the AGCNFTABLE32 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE32 field in the RWNXAGCNFT8 register.
 */
__INLINE uint8_t rcn_agcnftable32_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT8_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE32 field of the RWNXAGCNFT8 register.
 *
 * The RWNXAGCNFT8 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable32 - The value to set the field to.
 */
__INLINE void rcn_agcnftable32_setf(uint8_t agcnftable32)
{
    ASSERT_ERR((((uint32_t)agcnftable32 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT8_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT8_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable32 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT9 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE39   0x7
 *  21:16         AGCNFTABLE38   0x7
 *  13:08         AGCNFTABLE37   0x8
 *  05:00         AGCNFTABLE36   0x8
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT9 register
#define RCN_RWNXAGCNFT9_ADDR   (REG_RC_BASE_ADDR+0xC0A4)
/// Offset of the RWNXAGCNFT9 register from the base address
#define RCN_RWNXAGCNFT9_OFFSET 0x00000024
/// Index of the RWNXAGCNFT9 register
#define RCN_RWNXAGCNFT9_INDEX  0x00000009
/// Reset value of the RWNXAGCNFT9 register
#define RCN_RWNXAGCNFT9_RESET  0x07070808

/**
 * @brief Returns the current value of the RWNXAGCNFT9 register.
 * The RWNXAGCNFT9 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT9 register.
 */
__INLINE uint32_t rcn_rwnxagcnft9_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT9 register to a value.
 * The RWNXAGCNFT9 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft9_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR, value);
}

// field definitions
/// AGCNFTABLE39 field mask
#define RCN_AGCNFTABLE39_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE39 field LSB position
#define RCN_AGCNFTABLE39_LSB    24
/// AGCNFTABLE39 field width
#define RCN_AGCNFTABLE39_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE38 field mask
#define RCN_AGCNFTABLE38_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE38 field LSB position
#define RCN_AGCNFTABLE38_LSB    16
/// AGCNFTABLE38 field width
#define RCN_AGCNFTABLE38_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE37 field mask
#define RCN_AGCNFTABLE37_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE37 field LSB position
#define RCN_AGCNFTABLE37_LSB    8
/// AGCNFTABLE37 field width
#define RCN_AGCNFTABLE37_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE36 field mask
#define RCN_AGCNFTABLE36_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE36 field LSB position
#define RCN_AGCNFTABLE36_LSB    0
/// AGCNFTABLE36 field width
#define RCN_AGCNFTABLE36_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE39 field reset value
#define RCN_AGCNFTABLE39_RST    0x7
/// AGCNFTABLE38 field reset value
#define RCN_AGCNFTABLE38_RST    0x7
/// AGCNFTABLE37 field reset value
#define RCN_AGCNFTABLE37_RST    0x8
/// AGCNFTABLE36 field reset value
#define RCN_AGCNFTABLE36_RST    0x8

/**
 * @brief Constructs a value for the RWNXAGCNFT9 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable39 - The value to use for the AGCNFTABLE39 field.
 * @param[in] agcnftable38 - The value to use for the AGCNFTABLE38 field.
 * @param[in] agcnftable37 - The value to use for the AGCNFTABLE37 field.
 * @param[in] agcnftable36 - The value to use for the AGCNFTABLE36 field.
 */
__INLINE void rcn_rwnxagcnft9_pack(uint8_t agcnftable39, uint8_t agcnftable38, uint8_t agcnftable37, uint8_t agcnftable36)
{
    ASSERT_ERR((((uint32_t)agcnftable39 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable38 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable37 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable36 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR,  ((uint32_t)agcnftable39 << 24) | ((uint32_t)agcnftable38 << 16) | ((uint32_t)agcnftable37 << 8) | ((uint32_t)agcnftable36 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT9's fields from current value of the RWNXAGCNFT9 register.
 *
 * Reads the RWNXAGCNFT9 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable39 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable38 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable37 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable36 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft9_unpack(uint8_t* agcnftable39, uint8_t* agcnftable38, uint8_t* agcnftable37, uint8_t* agcnftable36)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);

    *agcnftable39 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable38 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable37 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable36 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE39 field in the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read and the AGCNFTABLE39 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE39 field in the RWNXAGCNFT9 register.
 */
__INLINE uint8_t rcn_agcnftable39_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE39 field of the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable39 - The value to set the field to.
 */
__INLINE void rcn_agcnftable39_setf(uint8_t agcnftable39)
{
    ASSERT_ERR((((uint32_t)agcnftable39 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT9_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable39 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE38 field in the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read and the AGCNFTABLE38 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE38 field in the RWNXAGCNFT9 register.
 */
__INLINE uint8_t rcn_agcnftable38_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE38 field of the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable38 - The value to set the field to.
 */
__INLINE void rcn_agcnftable38_setf(uint8_t agcnftable38)
{
    ASSERT_ERR((((uint32_t)agcnftable38 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT9_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable38 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE37 field in the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read and the AGCNFTABLE37 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE37 field in the RWNXAGCNFT9 register.
 */
__INLINE uint8_t rcn_agcnftable37_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE37 field of the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable37 - The value to set the field to.
 */
__INLINE void rcn_agcnftable37_setf(uint8_t agcnftable37)
{
    ASSERT_ERR((((uint32_t)agcnftable37 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT9_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable37 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE36 field in the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read and the AGCNFTABLE36 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE36 field in the RWNXAGCNFT9 register.
 */
__INLINE uint8_t rcn_agcnftable36_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT9_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE36 field of the RWNXAGCNFT9 register.
 *
 * The RWNXAGCNFT9 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable36 - The value to set the field to.
 */
__INLINE void rcn_agcnftable36_setf(uint8_t agcnftable36)
{
    ASSERT_ERR((((uint32_t)agcnftable36 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT9_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT9_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable36 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT10 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE43   0x4
 *  21:16         AGCNFTABLE42   0x5
 *  13:08         AGCNFTABLE41   0x5
 *  05:00         AGCNFTABLE40   0x6
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT10 register
#define RCN_RWNXAGCNFT10_ADDR   (REG_RC_BASE_ADDR+0xC0A8)
/// Offset of the RWNXAGCNFT10 register from the base address
#define RCN_RWNXAGCNFT10_OFFSET 0x00000028
/// Index of the RWNXAGCNFT10 register
#define RCN_RWNXAGCNFT10_INDEX  0x0000000A
/// Reset value of the RWNXAGCNFT10 register
#define RCN_RWNXAGCNFT10_RESET  0x04050506

/**
 * @brief Returns the current value of the RWNXAGCNFT10 register.
 * The RWNXAGCNFT10 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT10 register.
 */
__INLINE uint32_t rcn_rwnxagcnft10_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT10 register to a value.
 * The RWNXAGCNFT10 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft10_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR, value);
}

// field definitions
/// AGCNFTABLE43 field mask
#define RCN_AGCNFTABLE43_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE43 field LSB position
#define RCN_AGCNFTABLE43_LSB    24
/// AGCNFTABLE43 field width
#define RCN_AGCNFTABLE43_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE42 field mask
#define RCN_AGCNFTABLE42_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE42 field LSB position
#define RCN_AGCNFTABLE42_LSB    16
/// AGCNFTABLE42 field width
#define RCN_AGCNFTABLE42_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE41 field mask
#define RCN_AGCNFTABLE41_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE41 field LSB position
#define RCN_AGCNFTABLE41_LSB    8
/// AGCNFTABLE41 field width
#define RCN_AGCNFTABLE41_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE40 field mask
#define RCN_AGCNFTABLE40_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE40 field LSB position
#define RCN_AGCNFTABLE40_LSB    0
/// AGCNFTABLE40 field width
#define RCN_AGCNFTABLE40_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE43 field reset value
#define RCN_AGCNFTABLE43_RST    0x4
/// AGCNFTABLE42 field reset value
#define RCN_AGCNFTABLE42_RST    0x5
/// AGCNFTABLE41 field reset value
#define RCN_AGCNFTABLE41_RST    0x5
/// AGCNFTABLE40 field reset value
#define RCN_AGCNFTABLE40_RST    0x6

/**
 * @brief Constructs a value for the RWNXAGCNFT10 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable43 - The value to use for the AGCNFTABLE43 field.
 * @param[in] agcnftable42 - The value to use for the AGCNFTABLE42 field.
 * @param[in] agcnftable41 - The value to use for the AGCNFTABLE41 field.
 * @param[in] agcnftable40 - The value to use for the AGCNFTABLE40 field.
 */
__INLINE void rcn_rwnxagcnft10_pack(uint8_t agcnftable43, uint8_t agcnftable42, uint8_t agcnftable41, uint8_t agcnftable40)
{
    ASSERT_ERR((((uint32_t)agcnftable43 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable42 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable41 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable40 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR,  ((uint32_t)agcnftable43 << 24) | ((uint32_t)agcnftable42 << 16) | ((uint32_t)agcnftable41 << 8) | ((uint32_t)agcnftable40 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT10's fields from current value of the RWNXAGCNFT10 register.
 *
 * Reads the RWNXAGCNFT10 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable43 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable42 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable41 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable40 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft10_unpack(uint8_t* agcnftable43, uint8_t* agcnftable42, uint8_t* agcnftable41, uint8_t* agcnftable40)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);

    *agcnftable43 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable42 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable41 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable40 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE43 field in the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read and the AGCNFTABLE43 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE43 field in the RWNXAGCNFT10 register.
 */
__INLINE uint8_t rcn_agcnftable43_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE43 field of the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable43 - The value to set the field to.
 */
__INLINE void rcn_agcnftable43_setf(uint8_t agcnftable43)
{
    ASSERT_ERR((((uint32_t)agcnftable43 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT10_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable43 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE42 field in the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read and the AGCNFTABLE42 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE42 field in the RWNXAGCNFT10 register.
 */
__INLINE uint8_t rcn_agcnftable42_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE42 field of the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable42 - The value to set the field to.
 */
__INLINE void rcn_agcnftable42_setf(uint8_t agcnftable42)
{
    ASSERT_ERR((((uint32_t)agcnftable42 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT10_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable42 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE41 field in the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read and the AGCNFTABLE41 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE41 field in the RWNXAGCNFT10 register.
 */
__INLINE uint8_t rcn_agcnftable41_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE41 field of the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable41 - The value to set the field to.
 */
__INLINE void rcn_agcnftable41_setf(uint8_t agcnftable41)
{
    ASSERT_ERR((((uint32_t)agcnftable41 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT10_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable41 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE40 field in the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read and the AGCNFTABLE40 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE40 field in the RWNXAGCNFT10 register.
 */
__INLINE uint8_t rcn_agcnftable40_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT10_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE40 field of the RWNXAGCNFT10 register.
 *
 * The RWNXAGCNFT10 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable40 - The value to set the field to.
 */
__INLINE void rcn_agcnftable40_setf(uint8_t agcnftable40)
{
    ASSERT_ERR((((uint32_t)agcnftable40 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT10_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT10_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable40 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT11 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE47   0x4
 *  21:16         AGCNFTABLE46   0x4
 *  13:08         AGCNFTABLE45   0x4
 *  05:00         AGCNFTABLE44   0x4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT11 register
#define RCN_RWNXAGCNFT11_ADDR   (REG_RC_BASE_ADDR+0xC0AC)
/// Offset of the RWNXAGCNFT11 register from the base address
#define RCN_RWNXAGCNFT11_OFFSET 0x0000002C
/// Index of the RWNXAGCNFT11 register
#define RCN_RWNXAGCNFT11_INDEX  0x0000000B
/// Reset value of the RWNXAGCNFT11 register
#define RCN_RWNXAGCNFT11_RESET  0x04040404

/**
 * @brief Returns the current value of the RWNXAGCNFT11 register.
 * The RWNXAGCNFT11 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT11 register.
 */
__INLINE uint32_t rcn_rwnxagcnft11_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT11 register to a value.
 * The RWNXAGCNFT11 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft11_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR, value);
}

// field definitions
/// AGCNFTABLE47 field mask
#define RCN_AGCNFTABLE47_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE47 field LSB position
#define RCN_AGCNFTABLE47_LSB    24
/// AGCNFTABLE47 field width
#define RCN_AGCNFTABLE47_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE46 field mask
#define RCN_AGCNFTABLE46_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE46 field LSB position
#define RCN_AGCNFTABLE46_LSB    16
/// AGCNFTABLE46 field width
#define RCN_AGCNFTABLE46_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE45 field mask
#define RCN_AGCNFTABLE45_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE45 field LSB position
#define RCN_AGCNFTABLE45_LSB    8
/// AGCNFTABLE45 field width
#define RCN_AGCNFTABLE45_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE44 field mask
#define RCN_AGCNFTABLE44_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE44 field LSB position
#define RCN_AGCNFTABLE44_LSB    0
/// AGCNFTABLE44 field width
#define RCN_AGCNFTABLE44_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE47 field reset value
#define RCN_AGCNFTABLE47_RST    0x4
/// AGCNFTABLE46 field reset value
#define RCN_AGCNFTABLE46_RST    0x4
/// AGCNFTABLE45 field reset value
#define RCN_AGCNFTABLE45_RST    0x4
/// AGCNFTABLE44 field reset value
#define RCN_AGCNFTABLE44_RST    0x4

/**
 * @brief Constructs a value for the RWNXAGCNFT11 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable47 - The value to use for the AGCNFTABLE47 field.
 * @param[in] agcnftable46 - The value to use for the AGCNFTABLE46 field.
 * @param[in] agcnftable45 - The value to use for the AGCNFTABLE45 field.
 * @param[in] agcnftable44 - The value to use for the AGCNFTABLE44 field.
 */
__INLINE void rcn_rwnxagcnft11_pack(uint8_t agcnftable47, uint8_t agcnftable46, uint8_t agcnftable45, uint8_t agcnftable44)
{
    ASSERT_ERR((((uint32_t)agcnftable47 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable46 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable45 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable44 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR,  ((uint32_t)agcnftable47 << 24) | ((uint32_t)agcnftable46 << 16) | ((uint32_t)agcnftable45 << 8) | ((uint32_t)agcnftable44 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT11's fields from current value of the RWNXAGCNFT11 register.
 *
 * Reads the RWNXAGCNFT11 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable47 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable46 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable45 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable44 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft11_unpack(uint8_t* agcnftable47, uint8_t* agcnftable46, uint8_t* agcnftable45, uint8_t* agcnftable44)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);

    *agcnftable47 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable46 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable45 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable44 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE47 field in the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read and the AGCNFTABLE47 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE47 field in the RWNXAGCNFT11 register.
 */
__INLINE uint8_t rcn_agcnftable47_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE47 field of the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable47 - The value to set the field to.
 */
__INLINE void rcn_agcnftable47_setf(uint8_t agcnftable47)
{
    ASSERT_ERR((((uint32_t)agcnftable47 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT11_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable47 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE46 field in the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read and the AGCNFTABLE46 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE46 field in the RWNXAGCNFT11 register.
 */
__INLINE uint8_t rcn_agcnftable46_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE46 field of the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable46 - The value to set the field to.
 */
__INLINE void rcn_agcnftable46_setf(uint8_t agcnftable46)
{
    ASSERT_ERR((((uint32_t)agcnftable46 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT11_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable46 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE45 field in the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read and the AGCNFTABLE45 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE45 field in the RWNXAGCNFT11 register.
 */
__INLINE uint8_t rcn_agcnftable45_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE45 field of the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable45 - The value to set the field to.
 */
__INLINE void rcn_agcnftable45_setf(uint8_t agcnftable45)
{
    ASSERT_ERR((((uint32_t)agcnftable45 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT11_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable45 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE44 field in the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read and the AGCNFTABLE44 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE44 field in the RWNXAGCNFT11 register.
 */
__INLINE uint8_t rcn_agcnftable44_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT11_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE44 field of the RWNXAGCNFT11 register.
 *
 * The RWNXAGCNFT11 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable44 - The value to set the field to.
 */
__INLINE void rcn_agcnftable44_setf(uint8_t agcnftable44)
{
    ASSERT_ERR((((uint32_t)agcnftable44 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT11_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT11_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable44 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT12 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE51   0x4
 *  21:16         AGCNFTABLE50   0x4
 *  13:08         AGCNFTABLE49   0x4
 *  05:00         AGCNFTABLE48   0x4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT12 register
#define RCN_RWNXAGCNFT12_ADDR   (REG_RC_BASE_ADDR+0xC0B0)
/// Offset of the RWNXAGCNFT12 register from the base address
#define RCN_RWNXAGCNFT12_OFFSET 0x00000030
/// Index of the RWNXAGCNFT12 register
#define RCN_RWNXAGCNFT12_INDEX  0x0000000C
/// Reset value of the RWNXAGCNFT12 register
#define RCN_RWNXAGCNFT12_RESET  0x04040404

/**
 * @brief Returns the current value of the RWNXAGCNFT12 register.
 * The RWNXAGCNFT12 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT12 register.
 */
__INLINE uint32_t rcn_rwnxagcnft12_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT12 register to a value.
 * The RWNXAGCNFT12 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft12_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR, value);
}

// field definitions
/// AGCNFTABLE51 field mask
#define RCN_AGCNFTABLE51_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE51 field LSB position
#define RCN_AGCNFTABLE51_LSB    24
/// AGCNFTABLE51 field width
#define RCN_AGCNFTABLE51_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE50 field mask
#define RCN_AGCNFTABLE50_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE50 field LSB position
#define RCN_AGCNFTABLE50_LSB    16
/// AGCNFTABLE50 field width
#define RCN_AGCNFTABLE50_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE49 field mask
#define RCN_AGCNFTABLE49_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE49 field LSB position
#define RCN_AGCNFTABLE49_LSB    8
/// AGCNFTABLE49 field width
#define RCN_AGCNFTABLE49_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE48 field mask
#define RCN_AGCNFTABLE48_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE48 field LSB position
#define RCN_AGCNFTABLE48_LSB    0
/// AGCNFTABLE48 field width
#define RCN_AGCNFTABLE48_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE51 field reset value
#define RCN_AGCNFTABLE51_RST    0x4
/// AGCNFTABLE50 field reset value
#define RCN_AGCNFTABLE50_RST    0x4
/// AGCNFTABLE49 field reset value
#define RCN_AGCNFTABLE49_RST    0x4
/// AGCNFTABLE48 field reset value
#define RCN_AGCNFTABLE48_RST    0x4

/**
 * @brief Constructs a value for the RWNXAGCNFT12 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable51 - The value to use for the AGCNFTABLE51 field.
 * @param[in] agcnftable50 - The value to use for the AGCNFTABLE50 field.
 * @param[in] agcnftable49 - The value to use for the AGCNFTABLE49 field.
 * @param[in] agcnftable48 - The value to use for the AGCNFTABLE48 field.
 */
__INLINE void rcn_rwnxagcnft12_pack(uint8_t agcnftable51, uint8_t agcnftable50, uint8_t agcnftable49, uint8_t agcnftable48)
{
    ASSERT_ERR((((uint32_t)agcnftable51 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable50 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable49 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable48 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR,  ((uint32_t)agcnftable51 << 24) | ((uint32_t)agcnftable50 << 16) | ((uint32_t)agcnftable49 << 8) | ((uint32_t)agcnftable48 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT12's fields from current value of the RWNXAGCNFT12 register.
 *
 * Reads the RWNXAGCNFT12 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable51 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable50 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable49 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable48 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft12_unpack(uint8_t* agcnftable51, uint8_t* agcnftable50, uint8_t* agcnftable49, uint8_t* agcnftable48)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);

    *agcnftable51 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable50 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable49 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable48 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE51 field in the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read and the AGCNFTABLE51 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE51 field in the RWNXAGCNFT12 register.
 */
__INLINE uint8_t rcn_agcnftable51_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE51 field of the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable51 - The value to set the field to.
 */
__INLINE void rcn_agcnftable51_setf(uint8_t agcnftable51)
{
    ASSERT_ERR((((uint32_t)agcnftable51 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT12_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable51 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE50 field in the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read and the AGCNFTABLE50 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE50 field in the RWNXAGCNFT12 register.
 */
__INLINE uint8_t rcn_agcnftable50_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE50 field of the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable50 - The value to set the field to.
 */
__INLINE void rcn_agcnftable50_setf(uint8_t agcnftable50)
{
    ASSERT_ERR((((uint32_t)agcnftable50 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT12_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable50 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE49 field in the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read and the AGCNFTABLE49 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE49 field in the RWNXAGCNFT12 register.
 */
__INLINE uint8_t rcn_agcnftable49_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE49 field of the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable49 - The value to set the field to.
 */
__INLINE void rcn_agcnftable49_setf(uint8_t agcnftable49)
{
    ASSERT_ERR((((uint32_t)agcnftable49 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT12_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable49 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE48 field in the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read and the AGCNFTABLE48 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE48 field in the RWNXAGCNFT12 register.
 */
__INLINE uint8_t rcn_agcnftable48_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT12_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE48 field of the RWNXAGCNFT12 register.
 *
 * The RWNXAGCNFT12 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable48 - The value to set the field to.
 */
__INLINE void rcn_agcnftable48_setf(uint8_t agcnftable48)
{
    ASSERT_ERR((((uint32_t)agcnftable48 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT12_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT12_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable48 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT13 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE55   0x4
 *  21:16         AGCNFTABLE54   0x4
 *  13:08         AGCNFTABLE53   0x4
 *  05:00         AGCNFTABLE52   0x4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT13 register
#define RCN_RWNXAGCNFT13_ADDR   (REG_RC_BASE_ADDR+0xC0B4)
/// Offset of the RWNXAGCNFT13 register from the base address
#define RCN_RWNXAGCNFT13_OFFSET 0x00000034
/// Index of the RWNXAGCNFT13 register
#define RCN_RWNXAGCNFT13_INDEX  0x0000000D
/// Reset value of the RWNXAGCNFT13 register
#define RCN_RWNXAGCNFT13_RESET  0x04040404

/**
 * @brief Returns the current value of the RWNXAGCNFT13 register.
 * The RWNXAGCNFT13 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT13 register.
 */
__INLINE uint32_t rcn_rwnxagcnft13_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT13 register to a value.
 * The RWNXAGCNFT13 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft13_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR, value);
}

// field definitions
/// AGCNFTABLE55 field mask
#define RCN_AGCNFTABLE55_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE55 field LSB position
#define RCN_AGCNFTABLE55_LSB    24
/// AGCNFTABLE55 field width
#define RCN_AGCNFTABLE55_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE54 field mask
#define RCN_AGCNFTABLE54_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE54 field LSB position
#define RCN_AGCNFTABLE54_LSB    16
/// AGCNFTABLE54 field width
#define RCN_AGCNFTABLE54_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE53 field mask
#define RCN_AGCNFTABLE53_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE53 field LSB position
#define RCN_AGCNFTABLE53_LSB    8
/// AGCNFTABLE53 field width
#define RCN_AGCNFTABLE53_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE52 field mask
#define RCN_AGCNFTABLE52_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE52 field LSB position
#define RCN_AGCNFTABLE52_LSB    0
/// AGCNFTABLE52 field width
#define RCN_AGCNFTABLE52_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE55 field reset value
#define RCN_AGCNFTABLE55_RST    0x4
/// AGCNFTABLE54 field reset value
#define RCN_AGCNFTABLE54_RST    0x4
/// AGCNFTABLE53 field reset value
#define RCN_AGCNFTABLE53_RST    0x4
/// AGCNFTABLE52 field reset value
#define RCN_AGCNFTABLE52_RST    0x4

/**
 * @brief Constructs a value for the RWNXAGCNFT13 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable55 - The value to use for the AGCNFTABLE55 field.
 * @param[in] agcnftable54 - The value to use for the AGCNFTABLE54 field.
 * @param[in] agcnftable53 - The value to use for the AGCNFTABLE53 field.
 * @param[in] agcnftable52 - The value to use for the AGCNFTABLE52 field.
 */
__INLINE void rcn_rwnxagcnft13_pack(uint8_t agcnftable55, uint8_t agcnftable54, uint8_t agcnftable53, uint8_t agcnftable52)
{
    ASSERT_ERR((((uint32_t)agcnftable55 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable54 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable53 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable52 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR,  ((uint32_t)agcnftable55 << 24) | ((uint32_t)agcnftable54 << 16) | ((uint32_t)agcnftable53 << 8) | ((uint32_t)agcnftable52 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT13's fields from current value of the RWNXAGCNFT13 register.
 *
 * Reads the RWNXAGCNFT13 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable55 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable54 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable53 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable52 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft13_unpack(uint8_t* agcnftable55, uint8_t* agcnftable54, uint8_t* agcnftable53, uint8_t* agcnftable52)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);

    *agcnftable55 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable54 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable53 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable52 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE55 field in the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read and the AGCNFTABLE55 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE55 field in the RWNXAGCNFT13 register.
 */
__INLINE uint8_t rcn_agcnftable55_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE55 field of the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable55 - The value to set the field to.
 */
__INLINE void rcn_agcnftable55_setf(uint8_t agcnftable55)
{
    ASSERT_ERR((((uint32_t)agcnftable55 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT13_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable55 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE54 field in the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read and the AGCNFTABLE54 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE54 field in the RWNXAGCNFT13 register.
 */
__INLINE uint8_t rcn_agcnftable54_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE54 field of the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable54 - The value to set the field to.
 */
__INLINE void rcn_agcnftable54_setf(uint8_t agcnftable54)
{
    ASSERT_ERR((((uint32_t)agcnftable54 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT13_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable54 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE53 field in the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read and the AGCNFTABLE53 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE53 field in the RWNXAGCNFT13 register.
 */
__INLINE uint8_t rcn_agcnftable53_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE53 field of the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable53 - The value to set the field to.
 */
__INLINE void rcn_agcnftable53_setf(uint8_t agcnftable53)
{
    ASSERT_ERR((((uint32_t)agcnftable53 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT13_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable53 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE52 field in the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read and the AGCNFTABLE52 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE52 field in the RWNXAGCNFT13 register.
 */
__INLINE uint8_t rcn_agcnftable52_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT13_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE52 field of the RWNXAGCNFT13 register.
 *
 * The RWNXAGCNFT13 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable52 - The value to set the field to.
 */
__INLINE void rcn_agcnftable52_setf(uint8_t agcnftable52)
{
    ASSERT_ERR((((uint32_t)agcnftable52 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT13_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT13_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable52 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT14 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE59   0x4
 *  21:16         AGCNFTABLE58   0x4
 *  13:08         AGCNFTABLE57   0x4
 *  05:00         AGCNFTABLE56   0x4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT14 register
#define RCN_RWNXAGCNFT14_ADDR   (REG_RC_BASE_ADDR+0xC0B8)
/// Offset of the RWNXAGCNFT14 register from the base address
#define RCN_RWNXAGCNFT14_OFFSET 0x00000038
/// Index of the RWNXAGCNFT14 register
#define RCN_RWNXAGCNFT14_INDEX  0x0000000E
/// Reset value of the RWNXAGCNFT14 register
#define RCN_RWNXAGCNFT14_RESET  0x04040404

/**
 * @brief Returns the current value of the RWNXAGCNFT14 register.
 * The RWNXAGCNFT14 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT14 register.
 */
__INLINE uint32_t rcn_rwnxagcnft14_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT14 register to a value.
 * The RWNXAGCNFT14 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft14_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR, value);
}

// field definitions
/// AGCNFTABLE59 field mask
#define RCN_AGCNFTABLE59_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE59 field LSB position
#define RCN_AGCNFTABLE59_LSB    24
/// AGCNFTABLE59 field width
#define RCN_AGCNFTABLE59_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE58 field mask
#define RCN_AGCNFTABLE58_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE58 field LSB position
#define RCN_AGCNFTABLE58_LSB    16
/// AGCNFTABLE58 field width
#define RCN_AGCNFTABLE58_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE57 field mask
#define RCN_AGCNFTABLE57_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE57 field LSB position
#define RCN_AGCNFTABLE57_LSB    8
/// AGCNFTABLE57 field width
#define RCN_AGCNFTABLE57_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE56 field mask
#define RCN_AGCNFTABLE56_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE56 field LSB position
#define RCN_AGCNFTABLE56_LSB    0
/// AGCNFTABLE56 field width
#define RCN_AGCNFTABLE56_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE59 field reset value
#define RCN_AGCNFTABLE59_RST    0x4
/// AGCNFTABLE58 field reset value
#define RCN_AGCNFTABLE58_RST    0x4
/// AGCNFTABLE57 field reset value
#define RCN_AGCNFTABLE57_RST    0x4
/// AGCNFTABLE56 field reset value
#define RCN_AGCNFTABLE56_RST    0x4

/**
 * @brief Constructs a value for the RWNXAGCNFT14 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable59 - The value to use for the AGCNFTABLE59 field.
 * @param[in] agcnftable58 - The value to use for the AGCNFTABLE58 field.
 * @param[in] agcnftable57 - The value to use for the AGCNFTABLE57 field.
 * @param[in] agcnftable56 - The value to use for the AGCNFTABLE56 field.
 */
__INLINE void rcn_rwnxagcnft14_pack(uint8_t agcnftable59, uint8_t agcnftable58, uint8_t agcnftable57, uint8_t agcnftable56)
{
    ASSERT_ERR((((uint32_t)agcnftable59 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable58 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable57 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable56 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR,  ((uint32_t)agcnftable59 << 24) | ((uint32_t)agcnftable58 << 16) | ((uint32_t)agcnftable57 << 8) | ((uint32_t)agcnftable56 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT14's fields from current value of the RWNXAGCNFT14 register.
 *
 * Reads the RWNXAGCNFT14 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable59 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable58 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable57 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable56 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft14_unpack(uint8_t* agcnftable59, uint8_t* agcnftable58, uint8_t* agcnftable57, uint8_t* agcnftable56)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);

    *agcnftable59 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable58 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable57 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable56 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE59 field in the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read and the AGCNFTABLE59 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE59 field in the RWNXAGCNFT14 register.
 */
__INLINE uint8_t rcn_agcnftable59_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE59 field of the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable59 - The value to set the field to.
 */
__INLINE void rcn_agcnftable59_setf(uint8_t agcnftable59)
{
    ASSERT_ERR((((uint32_t)agcnftable59 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT14_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable59 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE58 field in the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read and the AGCNFTABLE58 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE58 field in the RWNXAGCNFT14 register.
 */
__INLINE uint8_t rcn_agcnftable58_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE58 field of the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable58 - The value to set the field to.
 */
__INLINE void rcn_agcnftable58_setf(uint8_t agcnftable58)
{
    ASSERT_ERR((((uint32_t)agcnftable58 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT14_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable58 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE57 field in the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read and the AGCNFTABLE57 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE57 field in the RWNXAGCNFT14 register.
 */
__INLINE uint8_t rcn_agcnftable57_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE57 field of the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable57 - The value to set the field to.
 */
__INLINE void rcn_agcnftable57_setf(uint8_t agcnftable57)
{
    ASSERT_ERR((((uint32_t)agcnftable57 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT14_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable57 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE56 field in the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read and the AGCNFTABLE56 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE56 field in the RWNXAGCNFT14 register.
 */
__INLINE uint8_t rcn_agcnftable56_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT14_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE56 field of the RWNXAGCNFT14 register.
 *
 * The RWNXAGCNFT14 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable56 - The value to set the field to.
 */
__INLINE void rcn_agcnftable56_setf(uint8_t agcnftable56)
{
    ASSERT_ERR((((uint32_t)agcnftable56 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT14_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT14_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable56 << 0));
}

/// @}

/**
 * @name RWNXAGCNFT15 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24         AGCNFTABLE63   0x4
 *  21:16         AGCNFTABLE62   0x4
 *  13:08         AGCNFTABLE61   0x4
 *  05:00         AGCNFTABLE60   0x4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNFT15 register
#define RCN_RWNXAGCNFT15_ADDR   (REG_RC_BASE_ADDR+0xC0BC)
/// Offset of the RWNXAGCNFT15 register from the base address
#define RCN_RWNXAGCNFT15_OFFSET 0x0000003C
/// Index of the RWNXAGCNFT15 register
#define RCN_RWNXAGCNFT15_INDEX  0x0000000F
/// Reset value of the RWNXAGCNFT15 register
#define RCN_RWNXAGCNFT15_RESET  0x04040404

/**
 * @brief Returns the current value of the RWNXAGCNFT15 register.
 * The RWNXAGCNFT15 register will be read and its value returned.
 * @return The current value of the RWNXAGCNFT15 register.
 */
__INLINE uint32_t rcn_rwnxagcnft15_get(void)
{
    return REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);
}

/**
 * @brief Sets the RWNXAGCNFT15 register to a value.
 * The RWNXAGCNFT15 register will be written.
 * @param value - The value to write.
 */
__INLINE void rcn_rwnxagcnft15_set(uint32_t value)
{
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR, value);
}

// field definitions
/// AGCNFTABLE63 field mask
#define RCN_AGCNFTABLE63_MASK   ((uint32_t)0x3F000000)
/// AGCNFTABLE63 field LSB position
#define RCN_AGCNFTABLE63_LSB    24
/// AGCNFTABLE63 field width
#define RCN_AGCNFTABLE63_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE62 field mask
#define RCN_AGCNFTABLE62_MASK   ((uint32_t)0x003F0000)
/// AGCNFTABLE62 field LSB position
#define RCN_AGCNFTABLE62_LSB    16
/// AGCNFTABLE62 field width
#define RCN_AGCNFTABLE62_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE61 field mask
#define RCN_AGCNFTABLE61_MASK   ((uint32_t)0x00003F00)
/// AGCNFTABLE61 field LSB position
#define RCN_AGCNFTABLE61_LSB    8
/// AGCNFTABLE61 field width
#define RCN_AGCNFTABLE61_WIDTH  ((uint32_t)0x00000006)
/// AGCNFTABLE60 field mask
#define RCN_AGCNFTABLE60_MASK   ((uint32_t)0x0000003F)
/// AGCNFTABLE60 field LSB position
#define RCN_AGCNFTABLE60_LSB    0
/// AGCNFTABLE60 field width
#define RCN_AGCNFTABLE60_WIDTH  ((uint32_t)0x00000006)

/// AGCNFTABLE63 field reset value
#define RCN_AGCNFTABLE63_RST    0x4
/// AGCNFTABLE62 field reset value
#define RCN_AGCNFTABLE62_RST    0x4
/// AGCNFTABLE61 field reset value
#define RCN_AGCNFTABLE61_RST    0x4
/// AGCNFTABLE60 field reset value
#define RCN_AGCNFTABLE60_RST    0x4

/**
 * @brief Constructs a value for the RWNXAGCNFT15 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcnftable63 - The value to use for the AGCNFTABLE63 field.
 * @param[in] agcnftable62 - The value to use for the AGCNFTABLE62 field.
 * @param[in] agcnftable61 - The value to use for the AGCNFTABLE61 field.
 * @param[in] agcnftable60 - The value to use for the AGCNFTABLE60 field.
 */
__INLINE void rcn_rwnxagcnft15_pack(uint8_t agcnftable63, uint8_t agcnftable62, uint8_t agcnftable61, uint8_t agcnftable60)
{
    ASSERT_ERR((((uint32_t)agcnftable63 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable62 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable61 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)agcnftable60 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR,  ((uint32_t)agcnftable63 << 24) | ((uint32_t)agcnftable62 << 16) | ((uint32_t)agcnftable61 << 8) | ((uint32_t)agcnftable60 << 0));
}

/**
 * @brief Unpacks RWNXAGCNFT15's fields from current value of the RWNXAGCNFT15 register.
 *
 * Reads the RWNXAGCNFT15 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcnftable63 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable62 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable61 - Will be populated with the current value of this field from the register.
 * @param[out] agcnftable60 - Will be populated with the current value of this field from the register.
 */
__INLINE void rcn_rwnxagcnft15_unpack(uint8_t* agcnftable63, uint8_t* agcnftable62, uint8_t* agcnftable61, uint8_t* agcnftable60)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);

    *agcnftable63 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *agcnftable62 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *agcnftable61 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *agcnftable60 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the AGCNFTABLE63 field in the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read and the AGCNFTABLE63 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE63 field in the RWNXAGCNFT15 register.
 */
__INLINE uint8_t rcn_agcnftable63_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the AGCNFTABLE63 field of the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable63 - The value to set the field to.
 */
__INLINE void rcn_agcnftable63_setf(uint8_t agcnftable63)
{
    ASSERT_ERR((((uint32_t)agcnftable63 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT15_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)agcnftable63 << 24));
}

/**
 * @brief Returns the current value of the AGCNFTABLE62 field in the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read and the AGCNFTABLE62 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE62 field in the RWNXAGCNFT15 register.
 */
__INLINE uint8_t rcn_agcnftable62_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the AGCNFTABLE62 field of the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable62 - The value to set the field to.
 */
__INLINE void rcn_agcnftable62_setf(uint8_t agcnftable62)
{
    ASSERT_ERR((((uint32_t)agcnftable62 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT15_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)agcnftable62 << 16));
}

/**
 * @brief Returns the current value of the AGCNFTABLE61 field in the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read and the AGCNFTABLE61 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE61 field in the RWNXAGCNFT15 register.
 */
__INLINE uint8_t rcn_agcnftable61_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the AGCNFTABLE61 field of the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable61 - The value to set the field to.
 */
__INLINE void rcn_agcnftable61_setf(uint8_t agcnftable61)
{
    ASSERT_ERR((((uint32_t)agcnftable61 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT15_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)agcnftable61 << 8));
}

/**
 * @brief Returns the current value of the AGCNFTABLE60 field in the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read and the AGCNFTABLE60 field's value will be returned.
 *
 * @return The current value of the AGCNFTABLE60 field in the RWNXAGCNFT15 register.
 */
__INLINE uint8_t rcn_agcnftable60_getf(void)
{
    uint32_t localVal = REG_PL_RD(RCN_RWNXAGCNFT15_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the AGCNFTABLE60 field of the RWNXAGCNFT15 register.
 *
 * The RWNXAGCNFT15 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcnftable60 - The value to set the field to.
 */
__INLINE void rcn_agcnftable60_setf(uint8_t agcnftable60)
{
    ASSERT_ERR((((uint32_t)agcnftable60 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RCN_RWNXAGCNFT15_ADDR, (REG_PL_RD(RCN_RWNXAGCNFT15_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)agcnftable60 << 0));
}

/// @}


#endif // _REG_RC_NFTABLE_H_

/// @}

