/**
 * @file reg_rc.h
 * @brief Definitions of the RC HW block registers and register access functions.
 *
 * @defgroup REG_RC REG_RC
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the RC HW block registers and register access functions.
 */
#ifndef _REG_RC_H_
#define _REG_RC_H_

#include "co_int.h"
#include "_reg_rc.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_RC peripheral.
 */
#define REG_RC_COUNT 16

/** @brief Decoding mask of the REG_RC peripheral registers from the CPU point of view.
 */
#define REG_RC_DECODING_MASK 0x0000003F

/**
 * @name STATIC_CFG register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00           RF_SEL_2G4   0
 * </pre>
 *
 * @{
 */

/// Address of the STATIC_CFG register
#define RC_STATIC_CFG_ADDR   (REG_RC_BASE_ADDR+0xC000)
/// Offset of the STATIC_CFG register from the base address
#define RC_STATIC_CFG_OFFSET 0x00000000
/// Index of the STATIC_CFG register
#define RC_STATIC_CFG_INDEX  0x00000000
/// Reset value of the STATIC_CFG register
#define RC_STATIC_CFG_RESET  0x00000000

/**
 * @brief Returns the current value of the STATIC_CFG register.
 * The STATIC_CFG register will be read and its value returned.
 * @return The current value of the STATIC_CFG register.
 */
__INLINE uint32_t rc_static_cfg_get(void)
{
    return REG_PL_RD(RC_STATIC_CFG_ADDR);
}

/**
 * @brief Sets the STATIC_CFG register to a value.
 * The STATIC_CFG register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_static_cfg_set(uint32_t value)
{
    REG_PL_WR(RC_STATIC_CFG_ADDR, value);
}

// field definitions
/// RF_SEL_2G4 field bit
#define RC_RF_SEL_2G4_BIT    ((uint32_t)0x00000001)
/// RF_SEL_2G4 field position
#define RC_RF_SEL_2G4_POS    0

/// RF_SEL_2G4 field reset value
#define RC_RF_SEL_2G4_RST    0x0

/**
 * @brief Returns the current value of the RF_SEL_2G4 field in the STATIC_CFG register.
 *
 * The STATIC_CFG register will be read and the RF_SEL_2G4 field's value will be returned.
 *
 * @return The current value of the RF_SEL_2G4 field in the STATIC_CFG register.
 */
__INLINE uint8_t rc_rf_sel_2g4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_STATIC_CFG_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the RF_SEL_2G4 field of the STATIC_CFG register.
 *
 * The STATIC_CFG register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rfsel2g4 - The value to set the field to.
 */
__INLINE void rc_rf_sel_2g4_setf(uint8_t rfsel2g4)
{
    ASSERT_ERR((((uint32_t)rfsel2g4 << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(RC_STATIC_CFG_ADDR, (uint32_t)rfsel2g4 << 0);
}

/// @}

/**
 * @name SW_RFAPICMD_ADDR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00           APICMDADDR   0x1440
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFAPICMD_ADDR register
#define RC_SW_RFAPICMD_ADDR_ADDR   (REG_RC_BASE_ADDR+0xC004)
/// Offset of the SW_RFAPICMD_ADDR register from the base address
#define RC_SW_RFAPICMD_ADDR_OFFSET 0x00000004
/// Index of the SW_RFAPICMD_ADDR register
#define RC_SW_RFAPICMD_ADDR_INDEX  0x00000001
/// Reset value of the SW_RFAPICMD_ADDR register
#define RC_SW_RFAPICMD_ADDR_RESET  0x00001440

/**
 * @brief Returns the current value of the SW_RFAPICMD_ADDR register.
 * The SW_RFAPICMD_ADDR register will be read and its value returned.
 * @return The current value of the SW_RFAPICMD_ADDR register.
 */
__INLINE uint32_t rc_sw_rfapicmd_addr_get(void)
{
    return REG_PL_RD(RC_SW_RFAPICMD_ADDR_ADDR);
}

/**
 * @brief Sets the SW_RFAPICMD_ADDR register to a value.
 * The SW_RFAPICMD_ADDR register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfapicmd_addr_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFAPICMD_ADDR_ADDR, value);
}

// field definitions
/// APICMDADDR field mask
#define RC_APICMDADDR_MASK   ((uint32_t)0x0000FFFF)
/// APICMDADDR field LSB position
#define RC_APICMDADDR_LSB    0
/// APICMDADDR field width
#define RC_APICMDADDR_WIDTH  ((uint32_t)0x00000010)

/// APICMDADDR field reset value
#define RC_APICMDADDR_RST    0x1440

/**
 * @brief Returns the current value of the APICMDADDR field in the SW_RFAPICMD_ADDR register.
 *
 * The SW_RFAPICMD_ADDR register will be read and the APICMDADDR field's value will be returned.
 *
 * @return The current value of the APICMDADDR field in the SW_RFAPICMD_ADDR register.
 */
__INLINE uint16_t rc_apicmdaddr_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFAPICMD_ADDR_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the APICMDADDR field of the SW_RFAPICMD_ADDR register.
 *
 * The SW_RFAPICMD_ADDR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] apicmdaddr - The value to set the field to.
 */
__INLINE void rc_apicmdaddr_setf(uint16_t apicmdaddr)
{
    ASSERT_ERR((((uint32_t)apicmdaddr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFAPICMD_ADDR_ADDR, (uint32_t)apicmdaddr << 0);
}

/// @}

/**
 * @name SW_RFCMD_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31           START_DONE   0
 *     30         READNOTWRITE   0
 *     29               APICMD   0
 *  15:00       HEADER_OR_ADDR   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFCMD_CTRL register
#define RC_SW_RFCMD_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC008)
/// Offset of the SW_RFCMD_CTRL register from the base address
#define RC_SW_RFCMD_CTRL_OFFSET 0x00000008
/// Index of the SW_RFCMD_CTRL register
#define RC_SW_RFCMD_CTRL_INDEX  0x00000002
/// Reset value of the SW_RFCMD_CTRL register
#define RC_SW_RFCMD_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFCMD_CTRL register.
 * The SW_RFCMD_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFCMD_CTRL register.
 */
__INLINE uint32_t rc_sw_rfcmd_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFCMD_CTRL register to a value.
 * The SW_RFCMD_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfcmd_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR, value);
}

// field definitions
/// START_DONE field bit
#define RC_START_DONE_BIT        ((uint32_t)0x80000000)
/// START_DONE field position
#define RC_START_DONE_POS        31
/// READNOTWRITE field bit
#define RC_READNOTWRITE_BIT      ((uint32_t)0x40000000)
/// READNOTWRITE field position
#define RC_READNOTWRITE_POS      30
/// APICMD field bit
#define RC_APICMD_BIT            ((uint32_t)0x20000000)
/// APICMD field position
#define RC_APICMD_POS            29
/// HEADER_OR_ADDR field mask
#define RC_HEADER_OR_ADDR_MASK   ((uint32_t)0x0000FFFF)
/// HEADER_OR_ADDR field LSB position
#define RC_HEADER_OR_ADDR_LSB    0
/// HEADER_OR_ADDR field width
#define RC_HEADER_OR_ADDR_WIDTH  ((uint32_t)0x00000010)

/// START_DONE field reset value
#define RC_START_DONE_RST        0x0
/// READNOTWRITE field reset value
#define RC_READNOTWRITE_RST      0x0
/// APICMD field reset value
#define RC_APICMD_RST            0x0
/// HEADER_OR_ADDR field reset value
#define RC_HEADER_OR_ADDR_RST    0x0

/**
 * @brief Constructs a value for the SW_RFCMD_CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] startdone - The value to use for the START_DONE field.
 * @param[in] readnotwrite - The value to use for the READNOTWRITE field.
 * @param[in] apicmd - The value to use for the APICMD field.
 * @param[in] headeroraddr - The value to use for the HEADER_OR_ADDR field.
 */
__INLINE void rc_sw_rfcmd_ctrl_pack(uint8_t startdone, uint8_t readnotwrite, uint8_t apicmd, uint16_t headeroraddr)
{
    ASSERT_ERR((((uint32_t)startdone << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)readnotwrite << 30) & ~((uint32_t)0x40000000)) == 0);
    ASSERT_ERR((((uint32_t)apicmd << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)headeroraddr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR,  ((uint32_t)startdone << 31) | ((uint32_t)readnotwrite << 30) | ((uint32_t)apicmd << 29) | ((uint32_t)headeroraddr << 0));
}

/**
 * @brief Unpacks SW_RFCMD_CTRL's fields from current value of the SW_RFCMD_CTRL register.
 *
 * Reads the SW_RFCMD_CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] startdone - Will be populated with the current value of this field from the register.
 * @param[out] readnotwrite - Will be populated with the current value of this field from the register.
 * @param[out] apicmd - Will be populated with the current value of this field from the register.
 * @param[out] headeroraddr - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_sw_rfcmd_ctrl_unpack(uint8_t* startdone, uint8_t* readnotwrite, uint8_t* apicmd, uint16_t* headeroraddr)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);

    *startdone = (localVal & ((uint32_t)0x80000000)) >> 31;
    *readnotwrite = (localVal & ((uint32_t)0x40000000)) >> 30;
    *apicmd = (localVal & ((uint32_t)0x20000000)) >> 29;
    *headeroraddr = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the START_DONE field in the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read and the START_DONE field's value will be returned.
 *
 * @return The current value of the START_DONE field in the SW_RFCMD_CTRL register.
 */
__INLINE uint8_t rc_start_done_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the START_DONE field of the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] startdone - The value to set the field to.
 */
__INLINE void rc_start_done_setf(uint8_t startdone)
{
    ASSERT_ERR((((uint32_t)startdone << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR, (REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)startdone << 31));
}

/**
 * @brief Returns the current value of the READNOTWRITE field in the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read and the READNOTWRITE field's value will be returned.
 *
 * @return The current value of the READNOTWRITE field in the SW_RFCMD_CTRL register.
 */
__INLINE uint8_t rc_readnotwrite_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

/**
 * @brief Sets the READNOTWRITE field of the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] readnotwrite - The value to set the field to.
 */
__INLINE void rc_readnotwrite_setf(uint8_t readnotwrite)
{
    ASSERT_ERR((((uint32_t)readnotwrite << 30) & ~((uint32_t)0x40000000)) == 0);
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR, (REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR) & ~((uint32_t)0x40000000)) | ((uint32_t)readnotwrite << 30));
}

/**
 * @brief Returns the current value of the APICMD field in the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read and the APICMD field's value will be returned.
 *
 * @return The current value of the APICMD field in the SW_RFCMD_CTRL register.
 */
__INLINE uint8_t rc_apicmd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Sets the APICMD field of the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] apicmd - The value to set the field to.
 */
__INLINE void rc_apicmd_setf(uint8_t apicmd)
{
    ASSERT_ERR((((uint32_t)apicmd << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR, (REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)apicmd << 29));
}

/**
 * @brief Returns the current value of the HEADER_OR_ADDR field in the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read and the HEADER_OR_ADDR field's value will be returned.
 *
 * @return The current value of the HEADER_OR_ADDR field in the SW_RFCMD_CTRL register.
 */
__INLINE uint16_t rc_header_or_addr_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the HEADER_OR_ADDR field of the SW_RFCMD_CTRL register.
 *
 * The SW_RFCMD_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] headeroraddr - The value to set the field to.
 */
__INLINE void rc_header_or_addr_setf(uint16_t headeroraddr)
{
    ASSERT_ERR((((uint32_t)headeroraddr << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFCMD_CTRL_ADDR, (REG_PL_RD(RC_SW_RFCMD_CTRL_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)headeroraddr << 0));
}

/// @}

/**
 * @name SW_RFDATA1_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16                DATA2   0x0
 *  15:00                DATA1   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFDATA1_CTRL register
#define RC_SW_RFDATA1_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC00C)
/// Offset of the SW_RFDATA1_CTRL register from the base address
#define RC_SW_RFDATA1_CTRL_OFFSET 0x0000000C
/// Index of the SW_RFDATA1_CTRL register
#define RC_SW_RFDATA1_CTRL_INDEX  0x00000003
/// Reset value of the SW_RFDATA1_CTRL register
#define RC_SW_RFDATA1_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFDATA1_CTRL register.
 * The SW_RFDATA1_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFDATA1_CTRL register.
 */
__INLINE uint32_t rc_sw_rfdata1_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFDATA1_CTRL register to a value.
 * The SW_RFDATA1_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfdata1_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFDATA1_CTRL_ADDR, value);
}

// field definitions
/// DATA2 field mask
#define RC_DATA2_MASK   ((uint32_t)0xFFFF0000)
/// DATA2 field LSB position
#define RC_DATA2_LSB    16
/// DATA2 field width
#define RC_DATA2_WIDTH  ((uint32_t)0x00000010)
/// DATA1 field mask
#define RC_DATA1_MASK   ((uint32_t)0x0000FFFF)
/// DATA1 field LSB position
#define RC_DATA1_LSB    0
/// DATA1 field width
#define RC_DATA1_WIDTH  ((uint32_t)0x00000010)

/// DATA2 field reset value
#define RC_DATA2_RST    0x0
/// DATA1 field reset value
#define RC_DATA1_RST    0x0

/**
 * @brief Constructs a value for the SW_RFDATA1_CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] data2 - The value to use for the DATA2 field.
 * @param[in] data1 - The value to use for the DATA1 field.
 */
__INLINE void rc_sw_rfdata1_ctrl_pack(uint16_t data2, uint16_t data1)
{
    ASSERT_ERR((((uint32_t)data2 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)data1 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA1_CTRL_ADDR,  ((uint32_t)data2 << 16) | ((uint32_t)data1 << 0));
}

/**
 * @brief Unpacks SW_RFDATA1_CTRL's fields from current value of the SW_RFDATA1_CTRL register.
 *
 * Reads the SW_RFDATA1_CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] data2 - Will be populated with the current value of this field from the register.
 * @param[out] data1 - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_sw_rfdata1_ctrl_unpack(uint16_t* data2, uint16_t* data1)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR);

    *data2 = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *data1 = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the DATA2 field in the SW_RFDATA1_CTRL register.
 *
 * The SW_RFDATA1_CTRL register will be read and the DATA2 field's value will be returned.
 *
 * @return The current value of the DATA2 field in the SW_RFDATA1_CTRL register.
 */
__INLINE uint16_t rc_data2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the DATA2 field of the SW_RFDATA1_CTRL register.
 *
 * The SW_RFDATA1_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data2 - The value to set the field to.
 */
__INLINE void rc_data2_setf(uint16_t data2)
{
    ASSERT_ERR((((uint32_t)data2 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(RC_SW_RFDATA1_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)data2 << 16));
}

/**
 * @brief Returns the current value of the DATA1 field in the SW_RFDATA1_CTRL register.
 *
 * The SW_RFDATA1_CTRL register will be read and the DATA1 field's value will be returned.
 *
 * @return The current value of the DATA1 field in the SW_RFDATA1_CTRL register.
 */
__INLINE uint16_t rc_data1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the DATA1 field of the SW_RFDATA1_CTRL register.
 *
 * The SW_RFDATA1_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data1 - The value to set the field to.
 */
__INLINE void rc_data1_setf(uint16_t data1)
{
    ASSERT_ERR((((uint32_t)data1 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA1_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA1_CTRL_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)data1 << 0));
}

/// @}

/**
 * @name SW_RFDATA2_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16                DATA4   0x0
 *  15:00                DATA3   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFDATA2_CTRL register
#define RC_SW_RFDATA2_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC010)
/// Offset of the SW_RFDATA2_CTRL register from the base address
#define RC_SW_RFDATA2_CTRL_OFFSET 0x00000010
/// Index of the SW_RFDATA2_CTRL register
#define RC_SW_RFDATA2_CTRL_INDEX  0x00000004
/// Reset value of the SW_RFDATA2_CTRL register
#define RC_SW_RFDATA2_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFDATA2_CTRL register.
 * The SW_RFDATA2_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFDATA2_CTRL register.
 */
__INLINE uint32_t rc_sw_rfdata2_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFDATA2_CTRL register to a value.
 * The SW_RFDATA2_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfdata2_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFDATA2_CTRL_ADDR, value);
}

// field definitions
/// DATA4 field mask
#define RC_DATA4_MASK   ((uint32_t)0xFFFF0000)
/// DATA4 field LSB position
#define RC_DATA4_LSB    16
/// DATA4 field width
#define RC_DATA4_WIDTH  ((uint32_t)0x00000010)
/// DATA3 field mask
#define RC_DATA3_MASK   ((uint32_t)0x0000FFFF)
/// DATA3 field LSB position
#define RC_DATA3_LSB    0
/// DATA3 field width
#define RC_DATA3_WIDTH  ((uint32_t)0x00000010)

/// DATA4 field reset value
#define RC_DATA4_RST    0x0
/// DATA3 field reset value
#define RC_DATA3_RST    0x0

/**
 * @brief Constructs a value for the SW_RFDATA2_CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] data4 - The value to use for the DATA4 field.
 * @param[in] data3 - The value to use for the DATA3 field.
 */
__INLINE void rc_sw_rfdata2_ctrl_pack(uint16_t data4, uint16_t data3)
{
    ASSERT_ERR((((uint32_t)data4 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)data3 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA2_CTRL_ADDR,  ((uint32_t)data4 << 16) | ((uint32_t)data3 << 0));
}

/**
 * @brief Unpacks SW_RFDATA2_CTRL's fields from current value of the SW_RFDATA2_CTRL register.
 *
 * Reads the SW_RFDATA2_CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] data4 - Will be populated with the current value of this field from the register.
 * @param[out] data3 - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_sw_rfdata2_ctrl_unpack(uint16_t* data4, uint16_t* data3)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR);

    *data4 = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *data3 = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the DATA4 field in the SW_RFDATA2_CTRL register.
 *
 * The SW_RFDATA2_CTRL register will be read and the DATA4 field's value will be returned.
 *
 * @return The current value of the DATA4 field in the SW_RFDATA2_CTRL register.
 */
__INLINE uint16_t rc_data4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the DATA4 field of the SW_RFDATA2_CTRL register.
 *
 * The SW_RFDATA2_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data4 - The value to set the field to.
 */
__INLINE void rc_data4_setf(uint16_t data4)
{
    ASSERT_ERR((((uint32_t)data4 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(RC_SW_RFDATA2_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)data4 << 16));
}

/**
 * @brief Returns the current value of the DATA3 field in the SW_RFDATA2_CTRL register.
 *
 * The SW_RFDATA2_CTRL register will be read and the DATA3 field's value will be returned.
 *
 * @return The current value of the DATA3 field in the SW_RFDATA2_CTRL register.
 */
__INLINE uint16_t rc_data3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the DATA3 field of the SW_RFDATA2_CTRL register.
 *
 * The SW_RFDATA2_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data3 - The value to set the field to.
 */
__INLINE void rc_data3_setf(uint16_t data3)
{
    ASSERT_ERR((((uint32_t)data3 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA2_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA2_CTRL_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)data3 << 0));
}

/// @}

/**
 * @name SW_RFDATA3_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16                DATA6   0x0
 *  15:00                DATA5   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFDATA3_CTRL register
#define RC_SW_RFDATA3_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC014)
/// Offset of the SW_RFDATA3_CTRL register from the base address
#define RC_SW_RFDATA3_CTRL_OFFSET 0x00000014
/// Index of the SW_RFDATA3_CTRL register
#define RC_SW_RFDATA3_CTRL_INDEX  0x00000005
/// Reset value of the SW_RFDATA3_CTRL register
#define RC_SW_RFDATA3_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFDATA3_CTRL register.
 * The SW_RFDATA3_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFDATA3_CTRL register.
 */
__INLINE uint32_t rc_sw_rfdata3_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFDATA3_CTRL register to a value.
 * The SW_RFDATA3_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfdata3_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFDATA3_CTRL_ADDR, value);
}

// field definitions
/// DATA6 field mask
#define RC_DATA6_MASK   ((uint32_t)0xFFFF0000)
/// DATA6 field LSB position
#define RC_DATA6_LSB    16
/// DATA6 field width
#define RC_DATA6_WIDTH  ((uint32_t)0x00000010)
/// DATA5 field mask
#define RC_DATA5_MASK   ((uint32_t)0x0000FFFF)
/// DATA5 field LSB position
#define RC_DATA5_LSB    0
/// DATA5 field width
#define RC_DATA5_WIDTH  ((uint32_t)0x00000010)

/// DATA6 field reset value
#define RC_DATA6_RST    0x0
/// DATA5 field reset value
#define RC_DATA5_RST    0x0

/**
 * @brief Constructs a value for the SW_RFDATA3_CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] data6 - The value to use for the DATA6 field.
 * @param[in] data5 - The value to use for the DATA5 field.
 */
__INLINE void rc_sw_rfdata3_ctrl_pack(uint16_t data6, uint16_t data5)
{
    ASSERT_ERR((((uint32_t)data6 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)data5 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA3_CTRL_ADDR,  ((uint32_t)data6 << 16) | ((uint32_t)data5 << 0));
}

/**
 * @brief Unpacks SW_RFDATA3_CTRL's fields from current value of the SW_RFDATA3_CTRL register.
 *
 * Reads the SW_RFDATA3_CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] data6 - Will be populated with the current value of this field from the register.
 * @param[out] data5 - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_sw_rfdata3_ctrl_unpack(uint16_t* data6, uint16_t* data5)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR);

    *data6 = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *data5 = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the DATA6 field in the SW_RFDATA3_CTRL register.
 *
 * The SW_RFDATA3_CTRL register will be read and the DATA6 field's value will be returned.
 *
 * @return The current value of the DATA6 field in the SW_RFDATA3_CTRL register.
 */
__INLINE uint16_t rc_data6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the DATA6 field of the SW_RFDATA3_CTRL register.
 *
 * The SW_RFDATA3_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data6 - The value to set the field to.
 */
__INLINE void rc_data6_setf(uint16_t data6)
{
    ASSERT_ERR((((uint32_t)data6 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(RC_SW_RFDATA3_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)data6 << 16));
}

/**
 * @brief Returns the current value of the DATA5 field in the SW_RFDATA3_CTRL register.
 *
 * The SW_RFDATA3_CTRL register will be read and the DATA5 field's value will be returned.
 *
 * @return The current value of the DATA5 field in the SW_RFDATA3_CTRL register.
 */
__INLINE uint16_t rc_data5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the DATA5 field of the SW_RFDATA3_CTRL register.
 *
 * The SW_RFDATA3_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data5 - The value to set the field to.
 */
__INLINE void rc_data5_setf(uint16_t data5)
{
    ASSERT_ERR((((uint32_t)data5 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA3_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA3_CTRL_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)data5 << 0));
}

/// @}

/**
 * @name SW_RFDATA4_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16                DATA8   0x0
 *  15:00                DATA7   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFDATA4_CTRL register
#define RC_SW_RFDATA4_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC018)
/// Offset of the SW_RFDATA4_CTRL register from the base address
#define RC_SW_RFDATA4_CTRL_OFFSET 0x00000018
/// Index of the SW_RFDATA4_CTRL register
#define RC_SW_RFDATA4_CTRL_INDEX  0x00000006
/// Reset value of the SW_RFDATA4_CTRL register
#define RC_SW_RFDATA4_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFDATA4_CTRL register.
 * The SW_RFDATA4_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFDATA4_CTRL register.
 */
__INLINE uint32_t rc_sw_rfdata4_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFDATA4_CTRL register to a value.
 * The SW_RFDATA4_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfdata4_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFDATA4_CTRL_ADDR, value);
}

// field definitions
/// DATA8 field mask
#define RC_DATA8_MASK   ((uint32_t)0xFFFF0000)
/// DATA8 field LSB position
#define RC_DATA8_LSB    16
/// DATA8 field width
#define RC_DATA8_WIDTH  ((uint32_t)0x00000010)
/// DATA7 field mask
#define RC_DATA7_MASK   ((uint32_t)0x0000FFFF)
/// DATA7 field LSB position
#define RC_DATA7_LSB    0
/// DATA7 field width
#define RC_DATA7_WIDTH  ((uint32_t)0x00000010)

/// DATA8 field reset value
#define RC_DATA8_RST    0x0
/// DATA7 field reset value
#define RC_DATA7_RST    0x0

/**
 * @brief Constructs a value for the SW_RFDATA4_CTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] data8 - The value to use for the DATA8 field.
 * @param[in] data7 - The value to use for the DATA7 field.
 */
__INLINE void rc_sw_rfdata4_ctrl_pack(uint16_t data8, uint16_t data7)
{
    ASSERT_ERR((((uint32_t)data8 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)data7 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA4_CTRL_ADDR,  ((uint32_t)data8 << 16) | ((uint32_t)data7 << 0));
}

/**
 * @brief Unpacks SW_RFDATA4_CTRL's fields from current value of the SW_RFDATA4_CTRL register.
 *
 * Reads the SW_RFDATA4_CTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] data8 - Will be populated with the current value of this field from the register.
 * @param[out] data7 - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_sw_rfdata4_ctrl_unpack(uint16_t* data8, uint16_t* data7)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR);

    *data8 = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *data7 = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the DATA8 field in the SW_RFDATA4_CTRL register.
 *
 * The SW_RFDATA4_CTRL register will be read and the DATA8 field's value will be returned.
 *
 * @return The current value of the DATA8 field in the SW_RFDATA4_CTRL register.
 */
__INLINE uint16_t rc_data8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the DATA8 field of the SW_RFDATA4_CTRL register.
 *
 * The SW_RFDATA4_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data8 - The value to set the field to.
 */
__INLINE void rc_data8_setf(uint16_t data8)
{
    ASSERT_ERR((((uint32_t)data8 << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(RC_SW_RFDATA4_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)data8 << 16));
}

/**
 * @brief Returns the current value of the DATA7 field in the SW_RFDATA4_CTRL register.
 *
 * The SW_RFDATA4_CTRL register will be read and the DATA7 field's value will be returned.
 *
 * @return The current value of the DATA7 field in the SW_RFDATA4_CTRL register.
 */
__INLINE uint16_t rc_data7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the DATA7 field of the SW_RFDATA4_CTRL register.
 *
 * The SW_RFDATA4_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data7 - The value to set the field to.
 */
__INLINE void rc_data7_setf(uint16_t data7)
{
    ASSERT_ERR((((uint32_t)data7 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA4_CTRL_ADDR, (REG_PL_RD(RC_SW_RFDATA4_CTRL_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)data7 << 0));
}

/// @}

/**
 * @name SW_RFDATA5_CTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00                DATA9   0x0
 * </pre>
 *
 * @{
 */

/// Address of the SW_RFDATA5_CTRL register
#define RC_SW_RFDATA5_CTRL_ADDR   (REG_RC_BASE_ADDR+0xC01C)
/// Offset of the SW_RFDATA5_CTRL register from the base address
#define RC_SW_RFDATA5_CTRL_OFFSET 0x0000001C
/// Index of the SW_RFDATA5_CTRL register
#define RC_SW_RFDATA5_CTRL_INDEX  0x00000007
/// Reset value of the SW_RFDATA5_CTRL register
#define RC_SW_RFDATA5_CTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SW_RFDATA5_CTRL register.
 * The SW_RFDATA5_CTRL register will be read and its value returned.
 * @return The current value of the SW_RFDATA5_CTRL register.
 */
__INLINE uint32_t rc_sw_rfdata5_ctrl_get(void)
{
    return REG_PL_RD(RC_SW_RFDATA5_CTRL_ADDR);
}

/**
 * @brief Sets the SW_RFDATA5_CTRL register to a value.
 * The SW_RFDATA5_CTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_sw_rfdata5_ctrl_set(uint32_t value)
{
    REG_PL_WR(RC_SW_RFDATA5_CTRL_ADDR, value);
}

// field definitions
/// DATA9 field mask
#define RC_DATA9_MASK   ((uint32_t)0x0000FFFF)
/// DATA9 field LSB position
#define RC_DATA9_LSB    0
/// DATA9 field width
#define RC_DATA9_WIDTH  ((uint32_t)0x00000010)

/// DATA9 field reset value
#define RC_DATA9_RST    0x0

/**
 * @brief Returns the current value of the DATA9 field in the SW_RFDATA5_CTRL register.
 *
 * The SW_RFDATA5_CTRL register will be read and the DATA9 field's value will be returned.
 *
 * @return The current value of the DATA9 field in the SW_RFDATA5_CTRL register.
 */
__INLINE uint16_t rc_data9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_SW_RFDATA5_CTRL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the DATA9 field of the SW_RFDATA5_CTRL register.
 *
 * The SW_RFDATA5_CTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] data9 - The value to set the field to.
 */
__INLINE void rc_data9_setf(uint16_t data9)
{
    ASSERT_ERR((((uint32_t)data9 << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(RC_SW_RFDATA5_CTRL_ADDR, (uint32_t)data9 << 0);
}

/// @}

/**
 * @name HW_SETMODE register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:28                TRXID   0x1
 *  27:24               TXMODE   0x4
 *  23:20               RXMODE   0x3
 *  09:00            SETOPMODE   0x3
 * </pre>
 *
 * @{
 */

/// Address of the HW_SETMODE register
#define RC_HW_SETMODE_ADDR   (REG_RC_BASE_ADDR+0xC020)
/// Offset of the HW_SETMODE register from the base address
#define RC_HW_SETMODE_OFFSET 0x00000020
/// Index of the HW_SETMODE register
#define RC_HW_SETMODE_INDEX  0x00000008
/// Reset value of the HW_SETMODE register
#define RC_HW_SETMODE_RESET  0x14300003

/**
 * @brief Returns the current value of the HW_SETMODE register.
 * The HW_SETMODE register will be read and its value returned.
 * @return The current value of the HW_SETMODE register.
 */
__INLINE uint32_t rc_hw_setmode_get(void)
{
    return REG_PL_RD(RC_HW_SETMODE_ADDR);
}

/**
 * @brief Sets the HW_SETMODE register to a value.
 * The HW_SETMODE register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_hw_setmode_set(uint32_t value)
{
    REG_PL_WR(RC_HW_SETMODE_ADDR, value);
}

// field definitions
/// TRXID field mask
#define RC_TRXID_MASK       ((uint32_t)0xF0000000)
/// TRXID field LSB position
#define RC_TRXID_LSB        28
/// TRXID field width
#define RC_TRXID_WIDTH      ((uint32_t)0x00000004)
/// TXMODE field mask
#define RC_TXMODE_MASK      ((uint32_t)0x0F000000)
/// TXMODE field LSB position
#define RC_TXMODE_LSB       24
/// TXMODE field width
#define RC_TXMODE_WIDTH     ((uint32_t)0x00000004)
/// RXMODE field mask
#define RC_RXMODE_MASK      ((uint32_t)0x00F00000)
/// RXMODE field LSB position
#define RC_RXMODE_LSB       20
/// RXMODE field width
#define RC_RXMODE_WIDTH     ((uint32_t)0x00000004)
/// SETOPMODE field mask
#define RC_SETOPMODE_MASK   ((uint32_t)0x000003FF)
/// SETOPMODE field LSB position
#define RC_SETOPMODE_LSB    0
/// SETOPMODE field width
#define RC_SETOPMODE_WIDTH  ((uint32_t)0x0000000A)

/// TRXID field reset value
#define RC_TRXID_RST        0x1
/// TXMODE field reset value
#define RC_TXMODE_RST       0x4
/// RXMODE field reset value
#define RC_RXMODE_RST       0x3
/// SETOPMODE field reset value
#define RC_SETOPMODE_RST    0x3

/**
 * @brief Constructs a value for the HW_SETMODE register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] trxid - The value to use for the TRXID field.
 * @param[in] txmode - The value to use for the TXMODE field.
 * @param[in] rxmode - The value to use for the RXMODE field.
 * @param[in] setopmode - The value to use for the SETOPMODE field.
 */
__INLINE void rc_hw_setmode_pack(uint8_t trxid, uint8_t txmode, uint8_t rxmode, uint16_t setopmode)
{
    ASSERT_ERR((((uint32_t)trxid << 28) & ~((uint32_t)0xF0000000)) == 0);
    ASSERT_ERR((((uint32_t)txmode << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)rxmode << 20) & ~((uint32_t)0x00F00000)) == 0);
    ASSERT_ERR((((uint32_t)setopmode << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_SETMODE_ADDR,  ((uint32_t)trxid << 28) | ((uint32_t)txmode << 24) | ((uint32_t)rxmode << 20) | ((uint32_t)setopmode << 0));
}

/**
 * @brief Unpacks HW_SETMODE's fields from current value of the HW_SETMODE register.
 *
 * Reads the HW_SETMODE register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] trxid - Will be populated with the current value of this field from the register.
 * @param[out] txmode - Will be populated with the current value of this field from the register.
 * @param[out] rxmode - Will be populated with the current value of this field from the register.
 * @param[out] setopmode - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_hw_setmode_unpack(uint8_t* trxid, uint8_t* txmode, uint8_t* rxmode, uint16_t* setopmode)
{
    uint32_t localVal = REG_PL_RD(RC_HW_SETMODE_ADDR);

    *trxid = (localVal & ((uint32_t)0xF0000000)) >> 28;
    *txmode = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *rxmode = (localVal & ((uint32_t)0x00F00000)) >> 20;
    *setopmode = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the TRXID field in the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read and the TRXID field's value will be returned.
 *
 * @return The current value of the TRXID field in the HW_SETMODE register.
 */
__INLINE uint8_t rc_trxid_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_SETMODE_ADDR);
    return ((localVal & ((uint32_t)0xF0000000)) >> 28);
}

/**
 * @brief Sets the TRXID field of the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] trxid - The value to set the field to.
 */
__INLINE void rc_trxid_setf(uint8_t trxid)
{
    ASSERT_ERR((((uint32_t)trxid << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_PL_WR(RC_HW_SETMODE_ADDR, (REG_PL_RD(RC_HW_SETMODE_ADDR) & ~((uint32_t)0xF0000000)) | ((uint32_t)trxid << 28));
}

/**
 * @brief Returns the current value of the TXMODE field in the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read and the TXMODE field's value will be returned.
 *
 * @return The current value of the TXMODE field in the HW_SETMODE register.
 */
__INLINE uint8_t rc_txmode_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_SETMODE_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the TXMODE field of the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txmode - The value to set the field to.
 */
__INLINE void rc_txmode_setf(uint8_t txmode)
{
    ASSERT_ERR((((uint32_t)txmode << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(RC_HW_SETMODE_ADDR, (REG_PL_RD(RC_HW_SETMODE_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)txmode << 24));
}

/**
 * @brief Returns the current value of the RXMODE field in the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read and the RXMODE field's value will be returned.
 *
 * @return The current value of the RXMODE field in the HW_SETMODE register.
 */
__INLINE uint8_t rc_rxmode_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_SETMODE_ADDR);
    return ((localVal & ((uint32_t)0x00F00000)) >> 20);
}

/**
 * @brief Sets the RXMODE field of the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxmode - The value to set the field to.
 */
__INLINE void rc_rxmode_setf(uint8_t rxmode)
{
    ASSERT_ERR((((uint32_t)rxmode << 20) & ~((uint32_t)0x00F00000)) == 0);
    REG_PL_WR(RC_HW_SETMODE_ADDR, (REG_PL_RD(RC_HW_SETMODE_ADDR) & ~((uint32_t)0x00F00000)) | ((uint32_t)rxmode << 20));
}

/**
 * @brief Returns the current value of the SETOPMODE field in the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read and the SETOPMODE field's value will be returned.
 *
 * @return The current value of the SETOPMODE field in the HW_SETMODE register.
 */
__INLINE uint16_t rc_setopmode_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_SETMODE_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the SETOPMODE field of the HW_SETMODE register.
 *
 * The HW_SETMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] setopmode - The value to set the field to.
 */
__INLINE void rc_setopmode_setf(uint16_t setopmode)
{
    ASSERT_ERR((((uint32_t)setopmode << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_SETMODE_ADDR, (REG_PL_RD(RC_HW_SETMODE_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)setopmode << 0));
}

/// @}

/**
 * @name HW_RXGAIN register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:16           RXGAINADDR   0x801
 *  07:00           RXGAININIT   0x3D
 * </pre>
 *
 * @{
 */

/// Address of the HW_RXGAIN register
#define RC_HW_RXGAIN_ADDR   (REG_RC_BASE_ADDR+0xC024)
/// Offset of the HW_RXGAIN register from the base address
#define RC_HW_RXGAIN_OFFSET 0x00000024
/// Index of the HW_RXGAIN register
#define RC_HW_RXGAIN_INDEX  0x00000009
/// Reset value of the HW_RXGAIN register
#define RC_HW_RXGAIN_RESET  0x0801003D

/**
 * @brief Returns the current value of the HW_RXGAIN register.
 * The HW_RXGAIN register will be read and its value returned.
 * @return The current value of the HW_RXGAIN register.
 */
__INLINE uint32_t rc_hw_rxgain_get(void)
{
    return REG_PL_RD(RC_HW_RXGAIN_ADDR);
}

/**
 * @brief Sets the HW_RXGAIN register to a value.
 * The HW_RXGAIN register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_hw_rxgain_set(uint32_t value)
{
    REG_PL_WR(RC_HW_RXGAIN_ADDR, value);
}

// field definitions
/// RXGAINADDR field mask
#define RC_RXGAINADDR_MASK   ((uint32_t)0xFFFF0000)
/// RXGAINADDR field LSB position
#define RC_RXGAINADDR_LSB    16
/// RXGAINADDR field width
#define RC_RXGAINADDR_WIDTH  ((uint32_t)0x00000010)
/// RXGAININIT field mask
#define RC_RXGAININIT_MASK   ((uint32_t)0x000000FF)
/// RXGAININIT field LSB position
#define RC_RXGAININIT_LSB    0
/// RXGAININIT field width
#define RC_RXGAININIT_WIDTH  ((uint32_t)0x00000008)

/// RXGAINADDR field reset value
#define RC_RXGAINADDR_RST    0x801
/// RXGAININIT field reset value
#define RC_RXGAININIT_RST    0x3D

/**
 * @brief Constructs a value for the HW_RXGAIN register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxgainaddr - The value to use for the RXGAINADDR field.
 * @param[in] rxgaininit - The value to use for the RXGAININIT field.
 */
__INLINE void rc_hw_rxgain_pack(uint16_t rxgainaddr, uint8_t rxgaininit)
{
    ASSERT_ERR((((uint32_t)rxgainaddr << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)rxgaininit << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RC_HW_RXGAIN_ADDR,  ((uint32_t)rxgainaddr << 16) | ((uint32_t)rxgaininit << 0));
}

/**
 * @brief Unpacks HW_RXGAIN's fields from current value of the HW_RXGAIN register.
 *
 * Reads the HW_RXGAIN register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxgainaddr - Will be populated with the current value of this field from the register.
 * @param[out] rxgaininit - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_hw_rxgain_unpack(uint16_t* rxgainaddr, uint8_t* rxgaininit)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXGAIN_ADDR);

    *rxgainaddr = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *rxgaininit = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the RXGAINADDR field in the HW_RXGAIN register.
 *
 * The HW_RXGAIN register will be read and the RXGAINADDR field's value will be returned.
 *
 * @return The current value of the RXGAINADDR field in the HW_RXGAIN register.
 */
__INLINE uint16_t rc_rxgainaddr_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXGAIN_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the RXGAINADDR field of the HW_RXGAIN register.
 *
 * The HW_RXGAIN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxgainaddr - The value to set the field to.
 */
__INLINE void rc_rxgainaddr_setf(uint16_t rxgainaddr)
{
    ASSERT_ERR((((uint32_t)rxgainaddr << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(RC_HW_RXGAIN_ADDR, (REG_PL_RD(RC_HW_RXGAIN_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)rxgainaddr << 16));
}

/**
 * @brief Returns the current value of the RXGAININIT field in the HW_RXGAIN register.
 *
 * The HW_RXGAIN register will be read and the RXGAININIT field's value will be returned.
 *
 * @return The current value of the RXGAININIT field in the HW_RXGAIN register.
 */
__INLINE uint8_t rc_rxgaininit_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXGAIN_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the RXGAININIT field of the HW_RXGAIN register.
 *
 * The HW_RXGAIN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxgaininit - The value to set the field to.
 */
__INLINE void rc_rxgaininit_setf(uint8_t rxgaininit)
{
    ASSERT_ERR((((uint32_t)rxgaininit << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RC_HW_RXGAIN_ADDR, (REG_PL_RD(RC_HW_RXGAIN_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)rxgaininit << 0));
}

/// @}

/**
 * @name HW_RX_AGCGAINREF register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:16      RXAGCGAININDMAX   0x42
 *  06:00      RXAGCGAININDMIN   0x3
 * </pre>
 *
 * @{
 */

/// Address of the HW_RX_AGCGAINREF register
#define RC_HW_RX_AGCGAINREF_ADDR   (REG_RC_BASE_ADDR+0xC028)
/// Offset of the HW_RX_AGCGAINREF register from the base address
#define RC_HW_RX_AGCGAINREF_OFFSET 0x00000028
/// Index of the HW_RX_AGCGAINREF register
#define RC_HW_RX_AGCGAINREF_INDEX  0x0000000A
/// Reset value of the HW_RX_AGCGAINREF register
#define RC_HW_RX_AGCGAINREF_RESET  0x00420003

/**
 * @brief Returns the current value of the HW_RX_AGCGAINREF register.
 * The HW_RX_AGCGAINREF register will be read and its value returned.
 * @return The current value of the HW_RX_AGCGAINREF register.
 */
__INLINE uint32_t rc_hw_rx_agcgainref_get(void)
{
    return REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR);
}

/**
 * @brief Sets the HW_RX_AGCGAINREF register to a value.
 * The HW_RX_AGCGAINREF register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_hw_rx_agcgainref_set(uint32_t value)
{
    REG_PL_WR(RC_HW_RX_AGCGAINREF_ADDR, value);
}

// field definitions
/// RXAGCGAININDMAX field mask
#define RC_RXAGCGAININDMAX_MASK   ((uint32_t)0x007F0000)
/// RXAGCGAININDMAX field LSB position
#define RC_RXAGCGAININDMAX_LSB    16
/// RXAGCGAININDMAX field width
#define RC_RXAGCGAININDMAX_WIDTH  ((uint32_t)0x00000007)
/// RXAGCGAININDMIN field mask
#define RC_RXAGCGAININDMIN_MASK   ((uint32_t)0x0000007F)
/// RXAGCGAININDMIN field LSB position
#define RC_RXAGCGAININDMIN_LSB    0
/// RXAGCGAININDMIN field width
#define RC_RXAGCGAININDMIN_WIDTH  ((uint32_t)0x00000007)

/// RXAGCGAININDMAX field reset value
#define RC_RXAGCGAININDMAX_RST    0x42
/// RXAGCGAININDMIN field reset value
#define RC_RXAGCGAININDMIN_RST    0x3

/**
 * @brief Constructs a value for the HW_RX_AGCGAINREF register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxagcgainindmax - The value to use for the RXAGCGAININDMAX field.
 * @param[in] rxagcgainindmin - The value to use for the RXAGCGAININDMIN field.
 */
__INLINE void rc_hw_rx_agcgainref_pack(uint8_t rxagcgainindmax, uint8_t rxagcgainindmin)
{
    ASSERT_ERR((((uint32_t)rxagcgainindmax << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)rxagcgainindmin << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RC_HW_RX_AGCGAINREF_ADDR,  ((uint32_t)rxagcgainindmax << 16) | ((uint32_t)rxagcgainindmin << 0));
}

/**
 * @brief Unpacks HW_RX_AGCGAINREF's fields from current value of the HW_RX_AGCGAINREF register.
 *
 * Reads the HW_RX_AGCGAINREF register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxagcgainindmax - Will be populated with the current value of this field from the register.
 * @param[out] rxagcgainindmin - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_hw_rx_agcgainref_unpack(uint8_t* rxagcgainindmax, uint8_t* rxagcgainindmin)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR);

    *rxagcgainindmax = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *rxagcgainindmin = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the RXAGCGAININDMAX field in the HW_RX_AGCGAINREF register.
 *
 * The HW_RX_AGCGAINREF register will be read and the RXAGCGAININDMAX field's value will be returned.
 *
 * @return The current value of the RXAGCGAININDMAX field in the HW_RX_AGCGAINREF register.
 */
__INLINE uint8_t rc_rxagcgainindmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the RXAGCGAININDMAX field of the HW_RX_AGCGAINREF register.
 *
 * The HW_RX_AGCGAINREF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxagcgainindmax - The value to set the field to.
 */
__INLINE void rc_rxagcgainindmax_setf(uint8_t rxagcgainindmax)
{
    ASSERT_ERR((((uint32_t)rxagcgainindmax << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RC_HW_RX_AGCGAINREF_ADDR, (REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)rxagcgainindmax << 16));
}

/**
 * @brief Returns the current value of the RXAGCGAININDMIN field in the HW_RX_AGCGAINREF register.
 *
 * The HW_RX_AGCGAINREF register will be read and the RXAGCGAININDMIN field's value will be returned.
 *
 * @return The current value of the RXAGCGAININDMIN field in the HW_RX_AGCGAINREF register.
 */
__INLINE uint8_t rc_rxagcgainindmin_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the RXAGCGAININDMIN field of the HW_RX_AGCGAINREF register.
 *
 * The HW_RX_AGCGAINREF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxagcgainindmin - The value to set the field to.
 */
__INLINE void rc_rxagcgainindmin_setf(uint8_t rxagcgainindmin)
{
    ASSERT_ERR((((uint32_t)rxagcgainindmin << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RC_HW_RX_AGCGAINREF_ADDR, (REG_PL_RD(RC_HW_RX_AGCGAINREF_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)rxagcgainindmin << 0));
}

/// @}

/**
 * @name HW_RX2TX_DELAY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  25:16        TXON2RX_DELAY   0x0
 *  09:00        RX2TXON_DELAY   0x0
 * </pre>
 *
 * @{
 */

/// Address of the HW_RX2TX_DELAY register
#define RC_HW_RX2TX_DELAY_ADDR   (REG_RC_BASE_ADDR+0xC02C)
/// Offset of the HW_RX2TX_DELAY register from the base address
#define RC_HW_RX2TX_DELAY_OFFSET 0x0000002C
/// Index of the HW_RX2TX_DELAY register
#define RC_HW_RX2TX_DELAY_INDEX  0x0000000B
/// Reset value of the HW_RX2TX_DELAY register
#define RC_HW_RX2TX_DELAY_RESET  0x00000000

/**
 * @brief Returns the current value of the HW_RX2TX_DELAY register.
 * The HW_RX2TX_DELAY register will be read and its value returned.
 * @return The current value of the HW_RX2TX_DELAY register.
 */
__INLINE uint32_t rc_hw_rx2tx_delay_get(void)
{
    return REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR);
}

/**
 * @brief Sets the HW_RX2TX_DELAY register to a value.
 * The HW_RX2TX_DELAY register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_hw_rx2tx_delay_set(uint32_t value)
{
    REG_PL_WR(RC_HW_RX2TX_DELAY_ADDR, value);
}

// field definitions
/// TXON2RX_DELAY field mask
#define RC_TXON2RX_DELAY_MASK   ((uint32_t)0x03FF0000)
/// TXON2RX_DELAY field LSB position
#define RC_TXON2RX_DELAY_LSB    16
/// TXON2RX_DELAY field width
#define RC_TXON2RX_DELAY_WIDTH  ((uint32_t)0x0000000A)
/// RX2TXON_DELAY field mask
#define RC_RX2TXON_DELAY_MASK   ((uint32_t)0x000003FF)
/// RX2TXON_DELAY field LSB position
#define RC_RX2TXON_DELAY_LSB    0
/// RX2TXON_DELAY field width
#define RC_RX2TXON_DELAY_WIDTH  ((uint32_t)0x0000000A)

/// TXON2RX_DELAY field reset value
#define RC_TXON2RX_DELAY_RST    0x0
/// RX2TXON_DELAY field reset value
#define RC_RX2TXON_DELAY_RST    0x0

/**
 * @brief Constructs a value for the HW_RX2TX_DELAY register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txon2rxdelay - The value to use for the TXON2RX_DELAY field.
 * @param[in] rx2txondelay - The value to use for the RX2TXON_DELAY field.
 */
__INLINE void rc_hw_rx2tx_delay_pack(uint16_t txon2rxdelay, uint16_t rx2txondelay)
{
    ASSERT_ERR((((uint32_t)txon2rxdelay << 16) & ~((uint32_t)0x03FF0000)) == 0);
    ASSERT_ERR((((uint32_t)rx2txondelay << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_RX2TX_DELAY_ADDR,  ((uint32_t)txon2rxdelay << 16) | ((uint32_t)rx2txondelay << 0));
}

/**
 * @brief Unpacks HW_RX2TX_DELAY's fields from current value of the HW_RX2TX_DELAY register.
 *
 * Reads the HW_RX2TX_DELAY register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txon2rxdelay - Will be populated with the current value of this field from the register.
 * @param[out] rx2txondelay - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_hw_rx2tx_delay_unpack(uint16_t* txon2rxdelay, uint16_t* rx2txondelay)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR);

    *txon2rxdelay = (localVal & ((uint32_t)0x03FF0000)) >> 16;
    *rx2txondelay = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the TXON2RX_DELAY field in the HW_RX2TX_DELAY register.
 *
 * The HW_RX2TX_DELAY register will be read and the TXON2RX_DELAY field's value will be returned.
 *
 * @return The current value of the TXON2RX_DELAY field in the HW_RX2TX_DELAY register.
 */
__INLINE uint16_t rc_txon2rx_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x03FF0000)) >> 16);
}

/**
 * @brief Sets the TXON2RX_DELAY field of the HW_RX2TX_DELAY register.
 *
 * The HW_RX2TX_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txon2rxdelay - The value to set the field to.
 */
__INLINE void rc_txon2rx_delay_setf(uint16_t txon2rxdelay)
{
    ASSERT_ERR((((uint32_t)txon2rxdelay << 16) & ~((uint32_t)0x03FF0000)) == 0);
    REG_PL_WR(RC_HW_RX2TX_DELAY_ADDR, (REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR) & ~((uint32_t)0x03FF0000)) | ((uint32_t)txon2rxdelay << 16));
}

/**
 * @brief Returns the current value of the RX2TXON_DELAY field in the HW_RX2TX_DELAY register.
 *
 * The HW_RX2TX_DELAY register will be read and the RX2TXON_DELAY field's value will be returned.
 *
 * @return The current value of the RX2TXON_DELAY field in the HW_RX2TX_DELAY register.
 */
__INLINE uint16_t rc_rx2txon_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the RX2TXON_DELAY field of the HW_RX2TX_DELAY register.
 *
 * The HW_RX2TX_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rx2txondelay - The value to set the field to.
 */
__INLINE void rc_rx2txon_delay_setf(uint16_t rx2txondelay)
{
    ASSERT_ERR((((uint32_t)rx2txondelay << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_RX2TX_DELAY_ADDR, (REG_PL_RD(RC_HW_RX2TX_DELAY_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)rx2txondelay << 0));
}

/// @}

/**
 * @name HW_RXIDLE2RX_DELAY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  25:16    RXON2RXIDLE_DELAY   0x0
 *  09:00    RXIDLE2RXON_DELAY   0x0
 * </pre>
 *
 * @{
 */

/// Address of the HW_RXIDLE2RX_DELAY register
#define RC_HW_RXIDLE2RX_DELAY_ADDR   (REG_RC_BASE_ADDR+0xC030)
/// Offset of the HW_RXIDLE2RX_DELAY register from the base address
#define RC_HW_RXIDLE2RX_DELAY_OFFSET 0x00000030
/// Index of the HW_RXIDLE2RX_DELAY register
#define RC_HW_RXIDLE2RX_DELAY_INDEX  0x0000000C
/// Reset value of the HW_RXIDLE2RX_DELAY register
#define RC_HW_RXIDLE2RX_DELAY_RESET  0x00000000

/**
 * @brief Returns the current value of the HW_RXIDLE2RX_DELAY register.
 * The HW_RXIDLE2RX_DELAY register will be read and its value returned.
 * @return The current value of the HW_RXIDLE2RX_DELAY register.
 */
__INLINE uint32_t rc_hw_rxidle2rx_delay_get(void)
{
    return REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR);
}

/**
 * @brief Sets the HW_RXIDLE2RX_DELAY register to a value.
 * The HW_RXIDLE2RX_DELAY register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_hw_rxidle2rx_delay_set(uint32_t value)
{
    REG_PL_WR(RC_HW_RXIDLE2RX_DELAY_ADDR, value);
}

// field definitions
/// RXON2RXIDLE_DELAY field mask
#define RC_RXON2RXIDLE_DELAY_MASK   ((uint32_t)0x03FF0000)
/// RXON2RXIDLE_DELAY field LSB position
#define RC_RXON2RXIDLE_DELAY_LSB    16
/// RXON2RXIDLE_DELAY field width
#define RC_RXON2RXIDLE_DELAY_WIDTH  ((uint32_t)0x0000000A)
/// RXIDLE2RXON_DELAY field mask
#define RC_RXIDLE2RXON_DELAY_MASK   ((uint32_t)0x000003FF)
/// RXIDLE2RXON_DELAY field LSB position
#define RC_RXIDLE2RXON_DELAY_LSB    0
/// RXIDLE2RXON_DELAY field width
#define RC_RXIDLE2RXON_DELAY_WIDTH  ((uint32_t)0x0000000A)

/// RXON2RXIDLE_DELAY field reset value
#define RC_RXON2RXIDLE_DELAY_RST    0x0
/// RXIDLE2RXON_DELAY field reset value
#define RC_RXIDLE2RXON_DELAY_RST    0x0

/**
 * @brief Constructs a value for the HW_RXIDLE2RX_DELAY register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxon2rxidledelay - The value to use for the RXON2RXIDLE_DELAY field.
 * @param[in] rxidle2rxondelay - The value to use for the RXIDLE2RXON_DELAY field.
 */
__INLINE void rc_hw_rxidle2rx_delay_pack(uint16_t rxon2rxidledelay, uint16_t rxidle2rxondelay)
{
    ASSERT_ERR((((uint32_t)rxon2rxidledelay << 16) & ~((uint32_t)0x03FF0000)) == 0);
    ASSERT_ERR((((uint32_t)rxidle2rxondelay << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_RXIDLE2RX_DELAY_ADDR,  ((uint32_t)rxon2rxidledelay << 16) | ((uint32_t)rxidle2rxondelay << 0));
}

/**
 * @brief Unpacks HW_RXIDLE2RX_DELAY's fields from current value of the HW_RXIDLE2RX_DELAY register.
 *
 * Reads the HW_RXIDLE2RX_DELAY register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxon2rxidledelay - Will be populated with the current value of this field from the register.
 * @param[out] rxidle2rxondelay - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_hw_rxidle2rx_delay_unpack(uint16_t* rxon2rxidledelay, uint16_t* rxidle2rxondelay)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR);

    *rxon2rxidledelay = (localVal & ((uint32_t)0x03FF0000)) >> 16;
    *rxidle2rxondelay = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the RXON2RXIDLE_DELAY field in the HW_RXIDLE2RX_DELAY register.
 *
 * The HW_RXIDLE2RX_DELAY register will be read and the RXON2RXIDLE_DELAY field's value will be returned.
 *
 * @return The current value of the RXON2RXIDLE_DELAY field in the HW_RXIDLE2RX_DELAY register.
 */
__INLINE uint16_t rc_rxon2rxidle_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x03FF0000)) >> 16);
}

/**
 * @brief Sets the RXON2RXIDLE_DELAY field of the HW_RXIDLE2RX_DELAY register.
 *
 * The HW_RXIDLE2RX_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxon2rxidledelay - The value to set the field to.
 */
__INLINE void rc_rxon2rxidle_delay_setf(uint16_t rxon2rxidledelay)
{
    ASSERT_ERR((((uint32_t)rxon2rxidledelay << 16) & ~((uint32_t)0x03FF0000)) == 0);
    REG_PL_WR(RC_HW_RXIDLE2RX_DELAY_ADDR, (REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR) & ~((uint32_t)0x03FF0000)) | ((uint32_t)rxon2rxidledelay << 16));
}

/**
 * @brief Returns the current value of the RXIDLE2RXON_DELAY field in the HW_RXIDLE2RX_DELAY register.
 *
 * The HW_RXIDLE2RX_DELAY register will be read and the RXIDLE2RXON_DELAY field's value will be returned.
 *
 * @return The current value of the RXIDLE2RXON_DELAY field in the HW_RXIDLE2RX_DELAY register.
 */
__INLINE uint16_t rc_rxidle2rxon_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the RXIDLE2RXON_DELAY field of the HW_RXIDLE2RX_DELAY register.
 *
 * The HW_RXIDLE2RX_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxidle2rxondelay - The value to set the field to.
 */
__INLINE void rc_rxidle2rxon_delay_setf(uint16_t rxidle2rxondelay)
{
    ASSERT_ERR((((uint32_t)rxidle2rxondelay << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RC_HW_RXIDLE2RX_DELAY_ADDR, (REG_PL_RD(RC_HW_RXIDLE2RX_DELAY_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)rxidle2rxondelay << 0));
}

/// @}

/**
 * @name EXTPA_SEQ_DELAY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  23:16   EXTPA_SEQ_OFF_DELAY   0x8
 *  07:00   EXTPA_SEQ_ON_DELAY   0x50
 * </pre>
 *
 * @{
 */

/// Address of the EXTPA_SEQ_DELAY register
#define RC_EXTPA_SEQ_DELAY_ADDR   (REG_RC_BASE_ADDR+0xC034)
/// Offset of the EXTPA_SEQ_DELAY register from the base address
#define RC_EXTPA_SEQ_DELAY_OFFSET 0x00000034
/// Index of the EXTPA_SEQ_DELAY register
#define RC_EXTPA_SEQ_DELAY_INDEX  0x0000000D
/// Reset value of the EXTPA_SEQ_DELAY register
#define RC_EXTPA_SEQ_DELAY_RESET  0x00080050

/**
 * @brief Returns the current value of the EXTPA_SEQ_DELAY register.
 * The EXTPA_SEQ_DELAY register will be read and its value returned.
 * @return The current value of the EXTPA_SEQ_DELAY register.
 */
__INLINE uint32_t rc_extpa_seq_delay_get(void)
{
    return REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR);
}

/**
 * @brief Sets the EXTPA_SEQ_DELAY register to a value.
 * The EXTPA_SEQ_DELAY register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_extpa_seq_delay_set(uint32_t value)
{
    REG_PL_WR(RC_EXTPA_SEQ_DELAY_ADDR, value);
}

// field definitions
/// EXTPA_SEQ_OFF_DELAY field mask
#define RC_EXTPA_SEQ_OFF_DELAY_MASK   ((uint32_t)0x00FF0000)
/// EXTPA_SEQ_OFF_DELAY field LSB position
#define RC_EXTPA_SEQ_OFF_DELAY_LSB    16
/// EXTPA_SEQ_OFF_DELAY field width
#define RC_EXTPA_SEQ_OFF_DELAY_WIDTH  ((uint32_t)0x00000008)
/// EXTPA_SEQ_ON_DELAY field mask
#define RC_EXTPA_SEQ_ON_DELAY_MASK    ((uint32_t)0x000000FF)
/// EXTPA_SEQ_ON_DELAY field LSB position
#define RC_EXTPA_SEQ_ON_DELAY_LSB     0
/// EXTPA_SEQ_ON_DELAY field width
#define RC_EXTPA_SEQ_ON_DELAY_WIDTH   ((uint32_t)0x00000008)

/// EXTPA_SEQ_OFF_DELAY field reset value
#define RC_EXTPA_SEQ_OFF_DELAY_RST    0x8
/// EXTPA_SEQ_ON_DELAY field reset value
#define RC_EXTPA_SEQ_ON_DELAY_RST     0x50

/**
 * @brief Constructs a value for the EXTPA_SEQ_DELAY register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] extpaseqoffdelay - The value to use for the EXTPA_SEQ_OFF_DELAY field.
 * @param[in] extpaseqondelay - The value to use for the EXTPA_SEQ_ON_DELAY field.
 */
__INLINE void rc_extpa_seq_delay_pack(uint8_t extpaseqoffdelay, uint8_t extpaseqondelay)
{
    ASSERT_ERR((((uint32_t)extpaseqoffdelay << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)extpaseqondelay << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RC_EXTPA_SEQ_DELAY_ADDR,  ((uint32_t)extpaseqoffdelay << 16) | ((uint32_t)extpaseqondelay << 0));
}

/**
 * @brief Unpacks EXTPA_SEQ_DELAY's fields from current value of the EXTPA_SEQ_DELAY register.
 *
 * Reads the EXTPA_SEQ_DELAY register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] extpaseqoffdelay - Will be populated with the current value of this field from the register.
 * @param[out] extpaseqondelay - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_extpa_seq_delay_unpack(uint8_t* extpaseqoffdelay, uint8_t* extpaseqondelay)
{
    uint32_t localVal = REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR);

    *extpaseqoffdelay = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *extpaseqondelay = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the EXTPA_SEQ_OFF_DELAY field in the EXTPA_SEQ_DELAY register.
 *
 * The EXTPA_SEQ_DELAY register will be read and the EXTPA_SEQ_OFF_DELAY field's value will be returned.
 *
 * @return The current value of the EXTPA_SEQ_OFF_DELAY field in the EXTPA_SEQ_DELAY register.
 */
__INLINE uint8_t rc_extpa_seq_off_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the EXTPA_SEQ_OFF_DELAY field of the EXTPA_SEQ_DELAY register.
 *
 * The EXTPA_SEQ_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] extpaseqoffdelay - The value to set the field to.
 */
__INLINE void rc_extpa_seq_off_delay_setf(uint8_t extpaseqoffdelay)
{
    ASSERT_ERR((((uint32_t)extpaseqoffdelay << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RC_EXTPA_SEQ_DELAY_ADDR, (REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)extpaseqoffdelay << 16));
}

/**
 * @brief Returns the current value of the EXTPA_SEQ_ON_DELAY field in the EXTPA_SEQ_DELAY register.
 *
 * The EXTPA_SEQ_DELAY register will be read and the EXTPA_SEQ_ON_DELAY field's value will be returned.
 *
 * @return The current value of the EXTPA_SEQ_ON_DELAY field in the EXTPA_SEQ_DELAY register.
 */
__INLINE uint8_t rc_extpa_seq_on_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the EXTPA_SEQ_ON_DELAY field of the EXTPA_SEQ_DELAY register.
 *
 * The EXTPA_SEQ_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] extpaseqondelay - The value to set the field to.
 */
__INLINE void rc_extpa_seq_on_delay_setf(uint8_t extpaseqondelay)
{
    ASSERT_ERR((((uint32_t)extpaseqondelay << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RC_EXTPA_SEQ_DELAY_ADDR, (REG_PL_RD(RC_EXTPA_SEQ_DELAY_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)extpaseqondelay << 0));
}

/// @}

/**
 * @name CMD_DELAY register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  27:16          SWCMD_DELAY   0x10
 *  11:00          HWCMD_DELAY   0x60
 * </pre>
 *
 * @{
 */

/// Address of the CMD_DELAY register
#define RC_CMD_DELAY_ADDR   (REG_RC_BASE_ADDR+0xC038)
/// Offset of the CMD_DELAY register from the base address
#define RC_CMD_DELAY_OFFSET 0x00000038
/// Index of the CMD_DELAY register
#define RC_CMD_DELAY_INDEX  0x0000000E
/// Reset value of the CMD_DELAY register
///#define RC_CMD_DELAY_RESET  0x00100060
#define RC_CMD_DELAY_RESET  0x00480048
/**
 * @brief Returns the current value of the CMD_DELAY register.
 * The CMD_DELAY register will be read and its value returned.
 * @return The current value of the CMD_DELAY register.
 */
__INLINE uint32_t rc_cmd_delay_get(void)
{
    return REG_PL_RD(RC_CMD_DELAY_ADDR);
}

/**
 * @brief Sets the CMD_DELAY register to a value.
 * The CMD_DELAY register will be written.
 * @param value - The value to write.
 */
__INLINE void rc_cmd_delay_set(uint32_t value)
{
    REG_PL_WR(RC_CMD_DELAY_ADDR, value);
}

// field definitions
/// SWCMD_DELAY field mask
#define RC_SWCMD_DELAY_MASK   ((uint32_t)0x0FFF0000)
/// SWCMD_DELAY field LSB position
#define RC_SWCMD_DELAY_LSB    16
/// SWCMD_DELAY field width
#define RC_SWCMD_DELAY_WIDTH  ((uint32_t)0x0000000C)
/// HWCMD_DELAY field mask
#define RC_HWCMD_DELAY_MASK   ((uint32_t)0x00000FFF)
/// HWCMD_DELAY field LSB position
#define RC_HWCMD_DELAY_LSB    0
/// HWCMD_DELAY field width
#define RC_HWCMD_DELAY_WIDTH  ((uint32_t)0x0000000C)

/// SWCMD_DELAY field reset value
#define RC_SWCMD_DELAY_RST    0x10
/// HWCMD_DELAY field reset value
#define RC_HWCMD_DELAY_RST    0x60

/**
 * @brief Constructs a value for the CMD_DELAY register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] swcmddelay - The value to use for the SWCMD_DELAY field.
 * @param[in] hwcmddelay - The value to use for the HWCMD_DELAY field.
 */
__INLINE void rc_cmd_delay_pack(uint16_t swcmddelay, uint16_t hwcmddelay)
{
    ASSERT_ERR((((uint32_t)swcmddelay << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    ASSERT_ERR((((uint32_t)hwcmddelay << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(RC_CMD_DELAY_ADDR,  ((uint32_t)swcmddelay << 16) | ((uint32_t)hwcmddelay << 0));
}

/**
 * @brief Unpacks CMD_DELAY's fields from current value of the CMD_DELAY register.
 *
 * Reads the CMD_DELAY register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] swcmddelay - Will be populated with the current value of this field from the register.
 * @param[out] hwcmddelay - Will be populated with the current value of this field from the register.
 */
__INLINE void rc_cmd_delay_unpack(uint16_t* swcmddelay, uint16_t* hwcmddelay)
{
    uint32_t localVal = REG_PL_RD(RC_CMD_DELAY_ADDR);

    *swcmddelay = (localVal & ((uint32_t)0x0FFF0000)) >> 16;
    *hwcmddelay = (localVal & ((uint32_t)0x00000FFF)) >> 0;
}

/**
 * @brief Returns the current value of the SWCMD_DELAY field in the CMD_DELAY register.
 *
 * The CMD_DELAY register will be read and the SWCMD_DELAY field's value will be returned.
 *
 * @return The current value of the SWCMD_DELAY field in the CMD_DELAY register.
 */
__INLINE uint16_t rc_swcmd_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_CMD_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x0FFF0000)) >> 16);
}

/**
 * @brief Sets the SWCMD_DELAY field of the CMD_DELAY register.
 *
 * The CMD_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] swcmddelay - The value to set the field to.
 */
__INLINE void rc_swcmd_delay_setf(uint16_t swcmddelay)
{
    ASSERT_ERR((((uint32_t)swcmddelay << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    REG_PL_WR(RC_CMD_DELAY_ADDR, (REG_PL_RD(RC_CMD_DELAY_ADDR) & ~((uint32_t)0x0FFF0000)) | ((uint32_t)swcmddelay << 16));
}

/**
 * @brief Returns the current value of the HWCMD_DELAY field in the CMD_DELAY register.
 *
 * The CMD_DELAY register will be read and the HWCMD_DELAY field's value will be returned.
 *
 * @return The current value of the HWCMD_DELAY field in the CMD_DELAY register.
 */
__INLINE uint16_t rc_hwcmd_delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_CMD_DELAY_ADDR);
    return ((localVal & ((uint32_t)0x00000FFF)) >> 0);
}

/**
 * @brief Sets the HWCMD_DELAY field of the CMD_DELAY register.
 *
 * The CMD_DELAY register will be read, modified to contain the new field value, and written.
 *
 * @param[in] hwcmddelay - The value to set the field to.
 */
__INLINE void rc_hwcmd_delay_setf(uint16_t hwcmddelay)
{
    ASSERT_ERR((((uint32_t)hwcmddelay << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(RC_CMD_DELAY_ADDR, (REG_PL_RD(RC_CMD_DELAY_ADDR) & ~((uint32_t)0x00000FFF)) | ((uint32_t)hwcmddelay << 0));
}

/// @}

/**
 * @name STATUS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  02:00             RC_STATE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the STATUS register
#define RC_STATUS_ADDR   (REG_RC_BASE_ADDR+0xC03C)
/// Offset of the STATUS register from the base address
#define RC_STATUS_OFFSET 0x0000003C
/// Index of the STATUS register
#define RC_STATUS_INDEX  0x0000000F
/// Reset value of the STATUS register
#define RC_STATUS_RESET  0x00000000

/**
 * @brief Returns the current value of the STATUS register.
 * The STATUS register will be read and its value returned.
 * @return The current value of the STATUS register.
 */
__INLINE uint32_t rc_status_get(void)
{
    return REG_PL_RD(RC_STATUS_ADDR);
}

// field definitions
/// RC_STATE field mask
#define RC_RC_STATE_MASK   ((uint32_t)0x00000007)
/// RC_STATE field LSB position
#define RC_RC_STATE_LSB    0
/// RC_STATE field width
#define RC_RC_STATE_WIDTH  ((uint32_t)0x00000003)

/// RC_STATE field reset value
#define RC_RC_STATE_RST    0x0

/**
 * @brief Returns the current value of the RC_STATE field in the STATUS register.
 *
 * The STATUS register will be read and the RC_STATE field's value will be returned.
 *
 * @return The current value of the RC_STATE field in the STATUS register.
 */
__INLINE uint8_t rc_rc_state_getf(void)
{
    uint32_t localVal = REG_PL_RD(RC_STATUS_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000007)) == 0);
    return (localVal >> 0);
}

/// @}


#endif // _REG_RC_H_

/// @}
