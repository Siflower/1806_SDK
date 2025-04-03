/**
 * @file reg_mdm_cfg.h
 * @brief Definitions of the MDM HW block registers and register access functions.
 *
 * @defgroup REG_MDM_CFG REG_MDM_CFG
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the MDM HW block registers and register access functions.
 */
#ifndef _REG_MDM_CFG_H_
#define _REG_MDM_CFG_H_

//#include "co_int.h"
//#include "_reg_mdm_cfg.h"
//#include "compiler.h"
//#include "arch.h"
#include "reg_access.h"

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_LDPC_DEC                  1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_MUMIMO_RX_EN                 1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_BFMEE_EN                 1

/** @brief Number of registers in the REG_MDM_CFG peripheral.
 */
#define REG_MDM_CFG_COUNT 45

/** @brief Decoding mask of the REG_MDM_CFG peripheral registers from the CPU point of view.
 */
#define REG_MDM_CFG_DECODING_MASK 0x000000FF
#if 0
/**
 * @name MDMCONF register definitions
 * <table>
 * <caption id="MDMCONF_BF">MDMCONF bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>03:00 <td>           MDMCONF <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the MDMCONF register
#define MDM_MDMCONF_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0800)
/// Offset of the MDMCONF register from the base address
#define MDM_MDMCONF_OFFSET 0x00000000
/// Index of the MDMCONF register
#define MDM_MDMCONF_INDEX  0x00000000
/// Reset value of the MDMCONF register
#define MDM_MDMCONF_RESET  0x00000000

/**
 * @brief Returns the current value of the MDMCONF register.
 * The MDMCONF register will be read and its value returned.
 * @return The current value of the MDMCONF register.
 */
__INLINE uint32_t mdm_mdmconf_get(void)
{
    return REG_PL_RD(MDM_MDMCONF_ADDR);
}

/**
 * @brief Sets the MDMCONF register to a value.
 * The MDMCONF register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmconf_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMCONF_ADDR, value);
}

// field definitions
/// MDMCONF field mask
#define MDM_MDMCONF_MASK   ((uint32_t)0x0000000F)
/// MDMCONF field LSB position
#define MDM_MDMCONF_LSB    0
/// MDMCONF field width
#define MDM_MDMCONF_WIDTH  ((uint32_t)0x00000004)

/// MDMCONF field reset value
#define MDM_MDMCONF_RST    0x0

/**
 * @brief Returns the current value of the MDMCONF field in the MDMCONF register.
 *
 * The MDMCONF register will be read and the MDMCONF field's value will be returned.
 *
 * @return The current value of the MDMCONF field in the MDMCONF register.
 */
__INLINE uint8_t mdm_mdmconf_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMCONF_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000000F)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the MDMCONF field of the MDMCONF register.
 *
 * The MDMCONF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mdmconf - The value to set the field to.
 */
__INLINE void mdm_mdmconf_setf(uint8_t mdmconf)
{
    ASSERT_ERR((((uint32_t)mdmconf << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMCONF_ADDR, (uint32_t)mdmconf << 0);
}

/// @}

/**
 * @name RXTDCTRL0 register definitions
 * <table>
 * <caption id="RXTDCTRL0_BF">RXTDCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:24 <td>     TDSYNCOFF4080 <td>R <td>R/W <td>0x12
 * <tr><td>23:16 <td>     TDSYNCOFF2080 <td>R <td>R/W <td>0x40
 * <tr><td>15:08 <td>     TDSYNCOFF4020 <td>R <td>R/W <td>0xFC
 * <tr><td>07:00 <td>     TDSYNCOFF2040 <td>R <td>R/W <td>0xD
 * </table>
 *
 * @{
 */

/// Address of the RXTDCTRL0 register
#define MDM_RXTDCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0804)
/// Offset of the RXTDCTRL0 register from the base address
#define MDM_RXTDCTRL0_OFFSET 0x00000004
/// Index of the RXTDCTRL0 register
#define MDM_RXTDCTRL0_INDEX  0x00000001
/// Reset value of the RXTDCTRL0 register
#define MDM_RXTDCTRL0_RESET  0x1240FC0D

/**
 * @brief Returns the current value of the RXTDCTRL0 register.
 * The RXTDCTRL0 register will be read and its value returned.
 * @return The current value of the RXTDCTRL0 register.
 */
__INLINE uint32_t mdm_rxtdctrl0_get(void)
{
    return REG_PL_RD(MDM_RXTDCTRL0_ADDR);
}

/**
 * @brief Sets the RXTDCTRL0 register to a value.
 * The RXTDCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxtdctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_RXTDCTRL0_ADDR, value);
}

// field definitions
/// TDSYNCOFF4080 field mask
#define MDM_TDSYNCOFF4080_MASK   ((uint32_t)0xFF000000)
/// TDSYNCOFF4080 field LSB position
#define MDM_TDSYNCOFF4080_LSB    24
/// TDSYNCOFF4080 field width
#define MDM_TDSYNCOFF4080_WIDTH  ((uint32_t)0x00000008)
/// TDSYNCOFF2080 field mask
#define MDM_TDSYNCOFF2080_MASK   ((uint32_t)0x00FF0000)
/// TDSYNCOFF2080 field LSB position
#define MDM_TDSYNCOFF2080_LSB    16
/// TDSYNCOFF2080 field width
#define MDM_TDSYNCOFF2080_WIDTH  ((uint32_t)0x00000008)
/// TDSYNCOFF4020 field mask
#define MDM_TDSYNCOFF4020_MASK   ((uint32_t)0x0000FF00)
/// TDSYNCOFF4020 field LSB position
#define MDM_TDSYNCOFF4020_LSB    8
/// TDSYNCOFF4020 field width
#define MDM_TDSYNCOFF4020_WIDTH  ((uint32_t)0x00000008)
/// TDSYNCOFF2040 field mask
#define MDM_TDSYNCOFF2040_MASK   ((uint32_t)0x000000FF)
/// TDSYNCOFF2040 field LSB position
#define MDM_TDSYNCOFF2040_LSB    0
/// TDSYNCOFF2040 field width
#define MDM_TDSYNCOFF2040_WIDTH  ((uint32_t)0x00000008)

/// TDSYNCOFF4080 field reset value
#define MDM_TDSYNCOFF4080_RST    0x12
/// TDSYNCOFF2080 field reset value
#define MDM_TDSYNCOFF2080_RST    0x40
/// TDSYNCOFF4020 field reset value
#define MDM_TDSYNCOFF4020_RST    0xFC
/// TDSYNCOFF2040 field reset value
#define MDM_TDSYNCOFF2040_RST    0xD

/**
 * @brief Constructs a value for the RXTDCTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tdsyncoff4080 - The value to use for the TDSYNCOFF4080 field.
 * @param[in] tdsyncoff2080 - The value to use for the TDSYNCOFF2080 field.
 * @param[in] tdsyncoff4020 - The value to use for the TDSYNCOFF4020 field.
 * @param[in] tdsyncoff2040 - The value to use for the TDSYNCOFF2040 field.
 */
__INLINE void mdm_rxtdctrl0_pack(uint8_t tdsyncoff4080, uint8_t tdsyncoff2080, uint8_t tdsyncoff4020, uint8_t tdsyncoff2040)
{
    ASSERT_ERR((((uint32_t)tdsyncoff4080 << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)tdsyncoff2080 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)tdsyncoff4020 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)tdsyncoff2040 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_RXTDCTRL0_ADDR,  ((uint32_t)tdsyncoff4080 << 24) | ((uint32_t)tdsyncoff2080 << 16) | ((uint32_t)tdsyncoff4020 << 8) | ((uint32_t)tdsyncoff2040 << 0));
}

/**
 * @brief Unpacks RXTDCTRL0's fields from current value of the RXTDCTRL0 register.
 *
 * Reads the RXTDCTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tdsyncoff4080 - Will be populated with the current value of this field from the register.
 * @param[out] tdsyncoff2080 - Will be populated with the current value of this field from the register.
 * @param[out] tdsyncoff4020 - Will be populated with the current value of this field from the register.
 * @param[out] tdsyncoff2040 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_rxtdctrl0_unpack(uint8_t* tdsyncoff4080, uint8_t* tdsyncoff2080, uint8_t* tdsyncoff4020, uint8_t* tdsyncoff2040)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL0_ADDR);

    *tdsyncoff4080 = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *tdsyncoff2080 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *tdsyncoff4020 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *tdsyncoff2040 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TDSYNCOFF4080 field in the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read and the TDSYNCOFF4080 field's value will be returned.
 *
 * @return The current value of the TDSYNCOFF4080 field in the RXTDCTRL0 register.
 */
__INLINE uint8_t mdm_tdsyncoff4080_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL0_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the TDSYNCOFF4080 field of the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdsyncoff4080 - The value to set the field to.
 */
__INLINE void mdm_tdsyncoff4080_setf(uint8_t tdsyncoff4080)
{
    ASSERT_ERR((((uint32_t)tdsyncoff4080 << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(MDM_RXTDCTRL0_ADDR, (REG_PL_RD(MDM_RXTDCTRL0_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)tdsyncoff4080 << 24));
}

/**
 * @brief Returns the current value of the TDSYNCOFF2080 field in the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read and the TDSYNCOFF2080 field's value will be returned.
 *
 * @return The current value of the TDSYNCOFF2080 field in the RXTDCTRL0 register.
 */
__INLINE uint8_t mdm_tdsyncoff2080_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the TDSYNCOFF2080 field of the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdsyncoff2080 - The value to set the field to.
 */
__INLINE void mdm_tdsyncoff2080_setf(uint8_t tdsyncoff2080)
{
    ASSERT_ERR((((uint32_t)tdsyncoff2080 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(MDM_RXTDCTRL0_ADDR, (REG_PL_RD(MDM_RXTDCTRL0_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)tdsyncoff2080 << 16));
}

/**
 * @brief Returns the current value of the TDSYNCOFF4020 field in the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read and the TDSYNCOFF4020 field's value will be returned.
 *
 * @return The current value of the TDSYNCOFF4020 field in the RXTDCTRL0 register.
 */
__INLINE uint8_t mdm_tdsyncoff4020_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the TDSYNCOFF4020 field of the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdsyncoff4020 - The value to set the field to.
 */
__INLINE void mdm_tdsyncoff4020_setf(uint8_t tdsyncoff4020)
{
    ASSERT_ERR((((uint32_t)tdsyncoff4020 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_RXTDCTRL0_ADDR, (REG_PL_RD(MDM_RXTDCTRL0_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)tdsyncoff4020 << 8));
}

/**
 * @brief Returns the current value of the TDSYNCOFF2040 field in the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read and the TDSYNCOFF2040 field's value will be returned.
 *
 * @return The current value of the TDSYNCOFF2040 field in the RXTDCTRL0 register.
 */
__INLINE uint8_t mdm_tdsyncoff2040_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TDSYNCOFF2040 field of the RXTDCTRL0 register.
 *
 * The RXTDCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdsyncoff2040 - The value to set the field to.
 */
__INLINE void mdm_tdsyncoff2040_setf(uint8_t tdsyncoff2040)
{
    ASSERT_ERR((((uint32_t)tdsyncoff2040 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_RXTDCTRL0_ADDR, (REG_PL_RD(MDM_RXTDCTRL0_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)tdsyncoff2040 << 0));
}

/// @}

/**
 * @name FDOCTRL0 register definitions
 * <table>
 * <caption id="FDOCTRL0_BF">FDOCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31 <td>     CFGSTO4TDCOMP <td>R <td>R/W <td>1
 * <tr><td>30 <td>        CFGCPEWALK <td>R <td>R/W <td>1
 * <tr><td>29 <td>         CFGSTOREF <td>R <td>R/W <td>1
 * <tr><td>28 <td>         CFGCPEREF <td>R <td>R/W <td>1
 * <tr><td>25 <td>      CPESLOPELGHT <td>R <td>R/W <td>1
 * <tr><td>24 <td>      STOSLOPELGHT <td>R <td>R/W <td>1
 * <tr><td>23 <td>           CPEMODE <td>R <td>R/W <td>1
 * <tr><td>22 <td>           STOMODE <td>R <td>R/W <td>1
 * <tr><td>21:17 <td>       STOCOMPTIME <td>R <td>R/W <td>0x2
 * <tr><td>16:11 <td>    MAXNTDSYNCHOFF <td>R <td>R/W <td>0x1F
 * <tr><td>10:05 <td>    MAXPTDSYNCHOFF <td>R <td>R/W <td>0x1F
 * <tr><td>04 <td>       STOFDCOMPEN <td>R <td>R/W <td>1
 * <tr><td>03 <td>     CPESLOPEESTEN <td>R <td>R/W <td>1
 * <tr><td>02 <td>       CPEFDCOMPEN <td>R <td>R/W <td>1
 * <tr><td>01 <td>     STOSLOPEESTEN <td>R <td>R/W <td>1
 * <tr><td>00 <td>       STOTDCOMPEN <td>R <td>R/W <td>1
 * </table>
 *
 * @{
 */

/// Address of the FDOCTRL0 register
#define MDM_FDOCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0808)
/// Offset of the FDOCTRL0 register from the base address
#define MDM_FDOCTRL0_OFFSET 0x00000008
/// Index of the FDOCTRL0 register
#define MDM_FDOCTRL0_INDEX  0x00000002
/// Reset value of the FDOCTRL0 register
#define MDM_FDOCTRL0_RESET  0xF3C4FBFF

/**
 * @brief Returns the current value of the FDOCTRL0 register.
 * The FDOCTRL0 register will be read and its value returned.
 * @return The current value of the FDOCTRL0 register.
 */
__INLINE uint32_t mdm_fdoctrl0_get(void)
{
    return REG_PL_RD(MDM_FDOCTRL0_ADDR);
}

/**
 * @brief Sets the FDOCTRL0 register to a value.
 * The FDOCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_fdoctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_FDOCTRL0_ADDR, value);
}

// field definitions
/// CFGSTO4TDCOMP field bit
#define MDM_CFGSTO4TDCOMP_BIT     ((uint32_t)0x80000000)
/// CFGSTO4TDCOMP field position
#define MDM_CFGSTO4TDCOMP_POS     31
/// CFGCPEWALK field bit
#define MDM_CFGCPEWALK_BIT        ((uint32_t)0x40000000)
/// CFGCPEWALK field position
#define MDM_CFGCPEWALK_POS        30
/// CFGSTOREF field bit
#define MDM_CFGSTOREF_BIT         ((uint32_t)0x20000000)
/// CFGSTOREF field position
#define MDM_CFGSTOREF_POS         29
/// CFGCPEREF field bit
#define MDM_CFGCPEREF_BIT         ((uint32_t)0x10000000)
/// CFGCPEREF field position
#define MDM_CFGCPEREF_POS         28
/// CPESLOPELGHT field bit
#define MDM_CPESLOPELGHT_BIT      ((uint32_t)0x02000000)
/// CPESLOPELGHT field position
#define MDM_CPESLOPELGHT_POS      25
/// STOSLOPELGHT field bit
#define MDM_STOSLOPELGHT_BIT      ((uint32_t)0x01000000)
/// STOSLOPELGHT field position
#define MDM_STOSLOPELGHT_POS      24
/// CPEMODE field bit
#define MDM_CPEMODE_BIT           ((uint32_t)0x00800000)
/// CPEMODE field position
#define MDM_CPEMODE_POS           23
/// STOMODE field bit
#define MDM_STOMODE_BIT           ((uint32_t)0x00400000)
/// STOMODE field position
#define MDM_STOMODE_POS           22
/// STOCOMPTIME field mask
#define MDM_STOCOMPTIME_MASK      ((uint32_t)0x003E0000)
/// STOCOMPTIME field LSB position
#define MDM_STOCOMPTIME_LSB       17
/// STOCOMPTIME field width
#define MDM_STOCOMPTIME_WIDTH     ((uint32_t)0x00000005)
/// MAXNTDSYNCHOFF field mask
#define MDM_MAXNTDSYNCHOFF_MASK   ((uint32_t)0x0001F800)
/// MAXNTDSYNCHOFF field LSB position
#define MDM_MAXNTDSYNCHOFF_LSB    11
/// MAXNTDSYNCHOFF field width
#define MDM_MAXNTDSYNCHOFF_WIDTH  ((uint32_t)0x00000006)
/// MAXPTDSYNCHOFF field mask
#define MDM_MAXPTDSYNCHOFF_MASK   ((uint32_t)0x000007E0)
/// MAXPTDSYNCHOFF field LSB position
#define MDM_MAXPTDSYNCHOFF_LSB    5
/// MAXPTDSYNCHOFF field width
#define MDM_MAXPTDSYNCHOFF_WIDTH  ((uint32_t)0x00000006)
/// STOFDCOMPEN field bit
#define MDM_STOFDCOMPEN_BIT       ((uint32_t)0x00000010)
/// STOFDCOMPEN field position
#define MDM_STOFDCOMPEN_POS       4
/// CPESLOPEESTEN field bit
#define MDM_CPESLOPEESTEN_BIT     ((uint32_t)0x00000008)
/// CPESLOPEESTEN field position
#define MDM_CPESLOPEESTEN_POS     3
/// CPEFDCOMPEN field bit
#define MDM_CPEFDCOMPEN_BIT       ((uint32_t)0x00000004)
/// CPEFDCOMPEN field position
#define MDM_CPEFDCOMPEN_POS       2
/// STOSLOPEESTEN field bit
#define MDM_STOSLOPEESTEN_BIT     ((uint32_t)0x00000002)
/// STOSLOPEESTEN field position
#define MDM_STOSLOPEESTEN_POS     1
/// STOTDCOMPEN field bit
#define MDM_STOTDCOMPEN_BIT       ((uint32_t)0x00000001)
/// STOTDCOMPEN field position
#define MDM_STOTDCOMPEN_POS       0

/// CFGSTO4TDCOMP field reset value
#define MDM_CFGSTO4TDCOMP_RST     0x1
/// CFGCPEWALK field reset value
#define MDM_CFGCPEWALK_RST        0x1
/// CFGSTOREF field reset value
#define MDM_CFGSTOREF_RST         0x1
/// CFGCPEREF field reset value
#define MDM_CFGCPEREF_RST         0x1
/// CPESLOPELGHT field reset value
#define MDM_CPESLOPELGHT_RST      0x1
/// STOSLOPELGHT field reset value
#define MDM_STOSLOPELGHT_RST      0x1
/// CPEMODE field reset value
#define MDM_CPEMODE_RST           0x1
/// STOMODE field reset value
#define MDM_STOMODE_RST           0x1
/// STOCOMPTIME field reset value
#define MDM_STOCOMPTIME_RST       0x2
/// MAXNTDSYNCHOFF field reset value
#define MDM_MAXNTDSYNCHOFF_RST    0x1F
/// MAXPTDSYNCHOFF field reset value
#define MDM_MAXPTDSYNCHOFF_RST    0x1F
/// STOFDCOMPEN field reset value
#define MDM_STOFDCOMPEN_RST       0x1
/// CPESLOPEESTEN field reset value
#define MDM_CPESLOPEESTEN_RST     0x1
/// CPEFDCOMPEN field reset value
#define MDM_CPEFDCOMPEN_RST       0x1
/// STOSLOPEESTEN field reset value
#define MDM_STOSLOPEESTEN_RST     0x1
/// STOTDCOMPEN field reset value
#define MDM_STOTDCOMPEN_RST       0x1

/**
 * @brief Constructs a value for the FDOCTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] cfgsto4tdcomp - The value to use for the CFGSTO4TDCOMP field.
 * @param[in] cfgcpewalk - The value to use for the CFGCPEWALK field.
 * @param[in] cfgstoref - The value to use for the CFGSTOREF field.
 * @param[in] cfgcperef - The value to use for the CFGCPEREF field.
 * @param[in] cpeslopelght - The value to use for the CPESLOPELGHT field.
 * @param[in] stoslopelght - The value to use for the STOSLOPELGHT field.
 * @param[in] cpemode - The value to use for the CPEMODE field.
 * @param[in] stomode - The value to use for the STOMODE field.
 * @param[in] stocomptime - The value to use for the STOCOMPTIME field.
 * @param[in] maxntdsynchoff - The value to use for the MAXNTDSYNCHOFF field.
 * @param[in] maxptdsynchoff - The value to use for the MAXPTDSYNCHOFF field.
 * @param[in] stofdcompen - The value to use for the STOFDCOMPEN field.
 * @param[in] cpeslopeesten - The value to use for the CPESLOPEESTEN field.
 * @param[in] cpefdcompen - The value to use for the CPEFDCOMPEN field.
 * @param[in] stoslopeesten - The value to use for the STOSLOPEESTEN field.
 * @param[in] stotdcompen - The value to use for the STOTDCOMPEN field.
 */
__INLINE void mdm_fdoctrl0_pack(uint8_t cfgsto4tdcomp, uint8_t cfgcpewalk, uint8_t cfgstoref, uint8_t cfgcperef, uint8_t cpeslopelght, uint8_t stoslopelght, uint8_t cpemode, uint8_t stomode, uint8_t stocomptime, uint8_t maxntdsynchoff, uint8_t maxptdsynchoff, uint8_t stofdcompen, uint8_t cpeslopeesten, uint8_t cpefdcompen, uint8_t stoslopeesten, uint8_t stotdcompen)
{
    ASSERT_ERR((((uint32_t)cfgsto4tdcomp << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)cfgcpewalk << 30) & ~((uint32_t)0x40000000)) == 0);
    ASSERT_ERR((((uint32_t)cfgstoref << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)cfgcperef << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)cpeslopelght << 25) & ~((uint32_t)0x02000000)) == 0);
    ASSERT_ERR((((uint32_t)stoslopelght << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)cpemode << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)stomode << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)stocomptime << 17) & ~((uint32_t)0x003E0000)) == 0);
    ASSERT_ERR((((uint32_t)maxntdsynchoff << 11) & ~((uint32_t)0x0001F800)) == 0);
    ASSERT_ERR((((uint32_t)maxptdsynchoff << 5) & ~((uint32_t)0x000007E0)) == 0);
    ASSERT_ERR((((uint32_t)stofdcompen << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)cpeslopeesten << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)cpefdcompen << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)stoslopeesten << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)stotdcompen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR,  ((uint32_t)cfgsto4tdcomp << 31) | ((uint32_t)cfgcpewalk << 30) | ((uint32_t)cfgstoref << 29) | ((uint32_t)cfgcperef << 28) | ((uint32_t)cpeslopelght << 25) | ((uint32_t)stoslopelght << 24) | ((uint32_t)cpemode << 23) | ((uint32_t)stomode << 22) | ((uint32_t)stocomptime << 17) | ((uint32_t)maxntdsynchoff << 11) | ((uint32_t)maxptdsynchoff << 5) | ((uint32_t)stofdcompen << 4) | ((uint32_t)cpeslopeesten << 3) | ((uint32_t)cpefdcompen << 2) | ((uint32_t)stoslopeesten << 1) | ((uint32_t)stotdcompen << 0));
}

/**
 * @brief Unpacks FDOCTRL0's fields from current value of the FDOCTRL0 register.
 *
 * Reads the FDOCTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] cfgsto4tdcomp - Will be populated with the current value of this field from the register.
 * @param[out] cfgcpewalk - Will be populated with the current value of this field from the register.
 * @param[out] cfgstoref - Will be populated with the current value of this field from the register.
 * @param[out] cfgcperef - Will be populated with the current value of this field from the register.
 * @param[out] cpeslopelght - Will be populated with the current value of this field from the register.
 * @param[out] stoslopelght - Will be populated with the current value of this field from the register.
 * @param[out] cpemode - Will be populated with the current value of this field from the register.
 * @param[out] stomode - Will be populated with the current value of this field from the register.
 * @param[out] stocomptime - Will be populated with the current value of this field from the register.
 * @param[out] maxntdsynchoff - Will be populated with the current value of this field from the register.
 * @param[out] maxptdsynchoff - Will be populated with the current value of this field from the register.
 * @param[out] stofdcompen - Will be populated with the current value of this field from the register.
 * @param[out] cpeslopeesten - Will be populated with the current value of this field from the register.
 * @param[out] cpefdcompen - Will be populated with the current value of this field from the register.
 * @param[out] stoslopeesten - Will be populated with the current value of this field from the register.
 * @param[out] stotdcompen - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_fdoctrl0_unpack(uint8_t* cfgsto4tdcomp, uint8_t* cfgcpewalk, uint8_t* cfgstoref, uint8_t* cfgcperef, uint8_t* cpeslopelght, uint8_t* stoslopelght, uint8_t* cpemode, uint8_t* stomode, uint8_t* stocomptime, uint8_t* maxntdsynchoff, uint8_t* maxptdsynchoff, uint8_t* stofdcompen, uint8_t* cpeslopeesten, uint8_t* cpefdcompen, uint8_t* stoslopeesten, uint8_t* stotdcompen)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);

    *cfgsto4tdcomp = (localVal & ((uint32_t)0x80000000)) >> 31;
    *cfgcpewalk = (localVal & ((uint32_t)0x40000000)) >> 30;
    *cfgstoref = (localVal & ((uint32_t)0x20000000)) >> 29;
    *cfgcperef = (localVal & ((uint32_t)0x10000000)) >> 28;
    *cpeslopelght = (localVal & ((uint32_t)0x02000000)) >> 25;
    *stoslopelght = (localVal & ((uint32_t)0x01000000)) >> 24;
    *cpemode = (localVal & ((uint32_t)0x00800000)) >> 23;
    *stomode = (localVal & ((uint32_t)0x00400000)) >> 22;
    *stocomptime = (localVal & ((uint32_t)0x003E0000)) >> 17;
    *maxntdsynchoff = (localVal & ((uint32_t)0x0001F800)) >> 11;
    *maxptdsynchoff = (localVal & ((uint32_t)0x000007E0)) >> 5;
    *stofdcompen = (localVal & ((uint32_t)0x00000010)) >> 4;
    *cpeslopeesten = (localVal & ((uint32_t)0x00000008)) >> 3;
    *cpefdcompen = (localVal & ((uint32_t)0x00000004)) >> 2;
    *stoslopeesten = (localVal & ((uint32_t)0x00000002)) >> 1;
    *stotdcompen = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the CFGSTO4TDCOMP field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CFGSTO4TDCOMP field's value will be returned.
 *
 * @return The current value of the CFGSTO4TDCOMP field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cfgsto4tdcomp_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the CFGSTO4TDCOMP field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgsto4tdcomp - The value to set the field to.
 */
__INLINE void mdm_cfgsto4tdcomp_setf(uint8_t cfgsto4tdcomp)
{
    ASSERT_ERR((((uint32_t)cfgsto4tdcomp << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)cfgsto4tdcomp << 31));
}

/**
 * @brief Returns the current value of the CFGCPEWALK field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CFGCPEWALK field's value will be returned.
 *
 * @return The current value of the CFGCPEWALK field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cfgcpewalk_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

/**
 * @brief Sets the CFGCPEWALK field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgcpewalk - The value to set the field to.
 */
__INLINE void mdm_cfgcpewalk_setf(uint8_t cfgcpewalk)
{
    ASSERT_ERR((((uint32_t)cfgcpewalk << 30) & ~((uint32_t)0x40000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x40000000)) | ((uint32_t)cfgcpewalk << 30));
}

/**
 * @brief Returns the current value of the CFGSTOREF field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CFGSTOREF field's value will be returned.
 *
 * @return The current value of the CFGSTOREF field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cfgstoref_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Sets the CFGSTOREF field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgstoref - The value to set the field to.
 */
__INLINE void mdm_cfgstoref_setf(uint8_t cfgstoref)
{
    ASSERT_ERR((((uint32_t)cfgstoref << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)cfgstoref << 29));
}

/**
 * @brief Returns the current value of the CFGCPEREF field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CFGCPEREF field's value will be returned.
 *
 * @return The current value of the CFGCPEREF field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cfgcperef_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the CFGCPEREF field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgcperef - The value to set the field to.
 */
__INLINE void mdm_cfgcperef_setf(uint8_t cfgcperef)
{
    ASSERT_ERR((((uint32_t)cfgcperef << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)cfgcperef << 28));
}

/**
 * @brief Returns the current value of the CPESLOPELGHT field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CPESLOPELGHT field's value will be returned.
 *
 * @return The current value of the CPESLOPELGHT field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cpeslopelght_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

/**
 * @brief Sets the CPESLOPELGHT field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cpeslopelght - The value to set the field to.
 */
__INLINE void mdm_cpeslopelght_setf(uint8_t cpeslopelght)
{
    ASSERT_ERR((((uint32_t)cpeslopelght << 25) & ~((uint32_t)0x02000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x02000000)) | ((uint32_t)cpeslopelght << 25));
}

/**
 * @brief Returns the current value of the STOSLOPELGHT field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOSLOPELGHT field's value will be returned.
 *
 * @return The current value of the STOSLOPELGHT field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stoslopelght_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the STOSLOPELGHT field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stoslopelght - The value to set the field to.
 */
__INLINE void mdm_stoslopelght_setf(uint8_t stoslopelght)
{
    ASSERT_ERR((((uint32_t)stoslopelght << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)stoslopelght << 24));
}

/**
 * @brief Returns the current value of the CPEMODE field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CPEMODE field's value will be returned.
 *
 * @return The current value of the CPEMODE field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cpemode_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the CPEMODE field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cpemode - The value to set the field to.
 */
__INLINE void mdm_cpemode_setf(uint8_t cpemode)
{
    ASSERT_ERR((((uint32_t)cpemode << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)cpemode << 23));
}

/**
 * @brief Returns the current value of the STOMODE field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOMODE field's value will be returned.
 *
 * @return The current value of the STOMODE field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stomode_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Sets the STOMODE field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stomode - The value to set the field to.
 */
__INLINE void mdm_stomode_setf(uint8_t stomode)
{
    ASSERT_ERR((((uint32_t)stomode << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)stomode << 22));
}

/**
 * @brief Returns the current value of the STOCOMPTIME field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOCOMPTIME field's value will be returned.
 *
 * @return The current value of the STOCOMPTIME field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stocomptime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x003E0000)) >> 17);
}

/**
 * @brief Sets the STOCOMPTIME field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stocomptime - The value to set the field to.
 */
__INLINE void mdm_stocomptime_setf(uint8_t stocomptime)
{
    ASSERT_ERR((((uint32_t)stocomptime << 17) & ~((uint32_t)0x003E0000)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x003E0000)) | ((uint32_t)stocomptime << 17));
}

/**
 * @brief Returns the current value of the MAXNTDSYNCHOFF field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the MAXNTDSYNCHOFF field's value will be returned.
 *
 * @return The current value of the MAXNTDSYNCHOFF field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_maxntdsynchoff_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x0001F800)) >> 11);
}

/**
 * @brief Sets the MAXNTDSYNCHOFF field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] maxntdsynchoff - The value to set the field to.
 */
__INLINE void mdm_maxntdsynchoff_setf(uint8_t maxntdsynchoff)
{
    ASSERT_ERR((((uint32_t)maxntdsynchoff << 11) & ~((uint32_t)0x0001F800)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x0001F800)) | ((uint32_t)maxntdsynchoff << 11));
}

/**
 * @brief Returns the current value of the MAXPTDSYNCHOFF field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the MAXPTDSYNCHOFF field's value will be returned.
 *
 * @return The current value of the MAXPTDSYNCHOFF field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_maxptdsynchoff_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x000007E0)) >> 5);
}

/**
 * @brief Sets the MAXPTDSYNCHOFF field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] maxptdsynchoff - The value to set the field to.
 */
__INLINE void mdm_maxptdsynchoff_setf(uint8_t maxptdsynchoff)
{
    ASSERT_ERR((((uint32_t)maxptdsynchoff << 5) & ~((uint32_t)0x000007E0)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x000007E0)) | ((uint32_t)maxptdsynchoff << 5));
}

/**
 * @brief Returns the current value of the STOFDCOMPEN field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOFDCOMPEN field's value will be returned.
 *
 * @return The current value of the STOFDCOMPEN field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stofdcompen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the STOFDCOMPEN field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stofdcompen - The value to set the field to.
 */
__INLINE void mdm_stofdcompen_setf(uint8_t stofdcompen)
{
    ASSERT_ERR((((uint32_t)stofdcompen << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)stofdcompen << 4));
}

/**
 * @brief Returns the current value of the CPESLOPEESTEN field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CPESLOPEESTEN field's value will be returned.
 *
 * @return The current value of the CPESLOPEESTEN field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cpeslopeesten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the CPESLOPEESTEN field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cpeslopeesten - The value to set the field to.
 */
__INLINE void mdm_cpeslopeesten_setf(uint8_t cpeslopeesten)
{
    ASSERT_ERR((((uint32_t)cpeslopeesten << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)cpeslopeesten << 3));
}

/**
 * @brief Returns the current value of the CPEFDCOMPEN field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the CPEFDCOMPEN field's value will be returned.
 *
 * @return The current value of the CPEFDCOMPEN field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_cpefdcompen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the CPEFDCOMPEN field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cpefdcompen - The value to set the field to.
 */
__INLINE void mdm_cpefdcompen_setf(uint8_t cpefdcompen)
{
    ASSERT_ERR((((uint32_t)cpefdcompen << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)cpefdcompen << 2));
}

/**
 * @brief Returns the current value of the STOSLOPEESTEN field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOSLOPEESTEN field's value will be returned.
 *
 * @return The current value of the STOSLOPEESTEN field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stoslopeesten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the STOSLOPEESTEN field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stoslopeesten - The value to set the field to.
 */
__INLINE void mdm_stoslopeesten_setf(uint8_t stoslopeesten)
{
    ASSERT_ERR((((uint32_t)stoslopeesten << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)stoslopeesten << 1));
}

/**
 * @brief Returns the current value of the STOTDCOMPEN field in the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read and the STOTDCOMPEN field's value will be returned.
 *
 * @return The current value of the STOTDCOMPEN field in the FDOCTRL0 register.
 */
__INLINE uint8_t mdm_stotdcompen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the STOTDCOMPEN field of the FDOCTRL0 register.
 *
 * The FDOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stotdcompen - The value to set the field to.
 */
__INLINE void mdm_stotdcompen_setf(uint8_t stotdcompen)
{
    ASSERT_ERR((((uint32_t)stotdcompen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_FDOCTRL0_ADDR, (REG_PL_RD(MDM_FDOCTRL0_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)stotdcompen << 0));
}

/// @}

/**
 * @name FDOCTRL1 register definitions
 * <table>
 * <caption id="FDOCTRL1_BF">FDOCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>17:00 <td>    STOSLOPEFORCED <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the FDOCTRL1 register
#define MDM_FDOCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x080C)
/// Offset of the FDOCTRL1 register from the base address
#define MDM_FDOCTRL1_OFFSET 0x0000000C
/// Index of the FDOCTRL1 register
#define MDM_FDOCTRL1_INDEX  0x00000003
/// Reset value of the FDOCTRL1 register
#define MDM_FDOCTRL1_RESET  0x00000000

/**
 * @brief Returns the current value of the FDOCTRL1 register.
 * The FDOCTRL1 register will be read and its value returned.
 * @return The current value of the FDOCTRL1 register.
 */
__INLINE uint32_t mdm_fdoctrl1_get(void)
{
    return REG_PL_RD(MDM_FDOCTRL1_ADDR);
}

/**
 * @brief Sets the FDOCTRL1 register to a value.
 * The FDOCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_fdoctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_FDOCTRL1_ADDR, value);
}

// field definitions
/// STOSLOPEFORCED field mask
#define MDM_STOSLOPEFORCED_MASK   ((uint32_t)0x0003FFFF)
/// STOSLOPEFORCED field LSB position
#define MDM_STOSLOPEFORCED_LSB    0
/// STOSLOPEFORCED field width
#define MDM_STOSLOPEFORCED_WIDTH  ((uint32_t)0x00000012)

/// STOSLOPEFORCED field reset value
#define MDM_STOSLOPEFORCED_RST    0x0

/**
 * @brief Returns the current value of the STOSLOPEFORCED field in the FDOCTRL1 register.
 *
 * The FDOCTRL1 register will be read and the STOSLOPEFORCED field's value will be returned.
 *
 * @return The current value of the STOSLOPEFORCED field in the FDOCTRL1 register.
 */
__INLINE uint32_t mdm_stoslopeforced_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0003FFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the STOSLOPEFORCED field of the FDOCTRL1 register.
 *
 * The FDOCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] stoslopeforced - The value to set the field to.
 */
__INLINE void mdm_stoslopeforced_setf(uint32_t stoslopeforced)
{
    ASSERT_ERR((((uint32_t)stoslopeforced << 0) & ~((uint32_t)0x0003FFFF)) == 0);
    REG_PL_WR(MDM_FDOCTRL1_ADDR, (uint32_t)stoslopeforced << 0);
}

/// @}

/**
 * @name FDOCTRL2 register definitions
 * <table>
 * <caption id="FDOCTRL2_BF">FDOCTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>17:00 <td>    CPESLOPEFORCED <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the FDOCTRL2 register
#define MDM_FDOCTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0810)
/// Offset of the FDOCTRL2 register from the base address
#define MDM_FDOCTRL2_OFFSET 0x00000010
/// Index of the FDOCTRL2 register
#define MDM_FDOCTRL2_INDEX  0x00000004
/// Reset value of the FDOCTRL2 register
#define MDM_FDOCTRL2_RESET  0x00000000

/**
 * @brief Returns the current value of the FDOCTRL2 register.
 * The FDOCTRL2 register will be read and its value returned.
 * @return The current value of the FDOCTRL2 register.
 */
__INLINE uint32_t mdm_fdoctrl2_get(void)
{
    return REG_PL_RD(MDM_FDOCTRL2_ADDR);
}

/**
 * @brief Sets the FDOCTRL2 register to a value.
 * The FDOCTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_fdoctrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_FDOCTRL2_ADDR, value);
}

// field definitions
/// CPESLOPEFORCED field mask
#define MDM_CPESLOPEFORCED_MASK   ((uint32_t)0x0003FFFF)
/// CPESLOPEFORCED field LSB position
#define MDM_CPESLOPEFORCED_LSB    0
/// CPESLOPEFORCED field width
#define MDM_CPESLOPEFORCED_WIDTH  ((uint32_t)0x00000012)

/// CPESLOPEFORCED field reset value
#define MDM_CPESLOPEFORCED_RST    0x0

/**
 * @brief Returns the current value of the CPESLOPEFORCED field in the FDOCTRL2 register.
 *
 * The FDOCTRL2 register will be read and the CPESLOPEFORCED field's value will be returned.
 *
 * @return The current value of the CPESLOPEFORCED field in the FDOCTRL2 register.
 */
__INLINE uint32_t mdm_cpeslopeforced_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDOCTRL2_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0003FFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the CPESLOPEFORCED field of the FDOCTRL2 register.
 *
 * The FDOCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cpeslopeforced - The value to set the field to.
 */
__INLINE void mdm_cpeslopeforced_setf(uint32_t cpeslopeforced)
{
    ASSERT_ERR((((uint32_t)cpeslopeforced << 0) & ~((uint32_t)0x0003FFFF)) == 0);
    REG_PL_WR(MDM_FDOCTRL2_ADDR, (uint32_t)cpeslopeforced << 0);
}

/// @}

/**
 * @name EQUALCTRL0 register definitions
 * <table>
 * <caption id="EQUALCTRL0_BF">EQUALCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:29 <td>      FDDCWEIGHTP2 <td>R <td>R/W <td>0x2
 * <tr><td>28:26 <td>      FDDCWEIGHTP1 <td>R <td>R/W <td>0x3
 * <tr><td>25:23 <td>      FDDCWEIGHTM1 <td>R <td>R/W <td>0x3
 * <tr><td>22:20 <td>      FDDCWEIGHTM2 <td>R <td>R/W <td>0x2
 * <tr><td>19:12 <td>  NOISEVARADJUSTDB <td>R <td>R/W <td>0x0
 * <tr><td>11:04 <td>  FDDCSNRTHRESHOLD <td>R <td>R/W <td>0x18
 * <tr><td>03 <td>        FDDCCOMPEN <td>R <td>R/W <td>1
 * <tr><td>00 <td>   EQNOISEVARESTEN <td>R <td>R/W <td>1
 * </table>
 *
 * @{
 */

/// Address of the EQUALCTRL0 register
#define MDM_EQUALCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0814)
/// Offset of the EQUALCTRL0 register from the base address
#define MDM_EQUALCTRL0_OFFSET 0x00000014
/// Index of the EQUALCTRL0 register
#define MDM_EQUALCTRL0_INDEX  0x00000005
/// Reset value of the EQUALCTRL0 register
#define MDM_EQUALCTRL0_RESET  0x4DA00189

/**
 * @brief Returns the current value of the EQUALCTRL0 register.
 * The EQUALCTRL0 register will be read and its value returned.
 * @return The current value of the EQUALCTRL0 register.
 */
__INLINE uint32_t mdm_equalctrl0_get(void)
{
    return REG_PL_RD(MDM_EQUALCTRL0_ADDR);
}

/**
 * @brief Sets the EQUALCTRL0 register to a value.
 * The EQUALCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_equalctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, value);
}

// field definitions
/// FDDCWEIGHTP2 field mask
#define MDM_FDDCWEIGHTP2_MASK       ((uint32_t)0xE0000000)
/// FDDCWEIGHTP2 field LSB position
#define MDM_FDDCWEIGHTP2_LSB        29
/// FDDCWEIGHTP2 field width
#define MDM_FDDCWEIGHTP2_WIDTH      ((uint32_t)0x00000003)
/// FDDCWEIGHTP1 field mask
#define MDM_FDDCWEIGHTP1_MASK       ((uint32_t)0x1C000000)
/// FDDCWEIGHTP1 field LSB position
#define MDM_FDDCWEIGHTP1_LSB        26
/// FDDCWEIGHTP1 field width
#define MDM_FDDCWEIGHTP1_WIDTH      ((uint32_t)0x00000003)
/// FDDCWEIGHTM1 field mask
#define MDM_FDDCWEIGHTM1_MASK       ((uint32_t)0x03800000)
/// FDDCWEIGHTM1 field LSB position
#define MDM_FDDCWEIGHTM1_LSB        23
/// FDDCWEIGHTM1 field width
#define MDM_FDDCWEIGHTM1_WIDTH      ((uint32_t)0x00000003)
/// FDDCWEIGHTM2 field mask
#define MDM_FDDCWEIGHTM2_MASK       ((uint32_t)0x00700000)
/// FDDCWEIGHTM2 field LSB position
#define MDM_FDDCWEIGHTM2_LSB        20
/// FDDCWEIGHTM2 field width
#define MDM_FDDCWEIGHTM2_WIDTH      ((uint32_t)0x00000003)
/// NOISEVARADJUSTDB field mask
#define MDM_NOISEVARADJUSTDB_MASK   ((uint32_t)0x000FF000)
/// NOISEVARADJUSTDB field LSB position
#define MDM_NOISEVARADJUSTDB_LSB    12
/// NOISEVARADJUSTDB field width
#define MDM_NOISEVARADJUSTDB_WIDTH  ((uint32_t)0x00000008)
/// FDDCSNRTHRESHOLD field mask
#define MDM_FDDCSNRTHRESHOLD_MASK   ((uint32_t)0x00000FF0)
/// FDDCSNRTHRESHOLD field LSB position
#define MDM_FDDCSNRTHRESHOLD_LSB    4
/// FDDCSNRTHRESHOLD field width
#define MDM_FDDCSNRTHRESHOLD_WIDTH  ((uint32_t)0x00000008)
/// FDDCCOMPEN field bit
#define MDM_FDDCCOMPEN_BIT          ((uint32_t)0x00000008)
/// FDDCCOMPEN field position
#define MDM_FDDCCOMPEN_POS          3
/// EQNOISEVARESTEN field bit
#define MDM_EQNOISEVARESTEN_BIT     ((uint32_t)0x00000001)
/// EQNOISEVARESTEN field position
#define MDM_EQNOISEVARESTEN_POS     0

/// FDDCWEIGHTP2 field reset value
#define MDM_FDDCWEIGHTP2_RST        0x2
/// FDDCWEIGHTP1 field reset value
#define MDM_FDDCWEIGHTP1_RST        0x3
/// FDDCWEIGHTM1 field reset value
#define MDM_FDDCWEIGHTM1_RST        0x3
/// FDDCWEIGHTM2 field reset value
#define MDM_FDDCWEIGHTM2_RST        0x2
/// NOISEVARADJUSTDB field reset value
#define MDM_NOISEVARADJUSTDB_RST    0x0
/// FDDCSNRTHRESHOLD field reset value
#define MDM_FDDCSNRTHRESHOLD_RST    0x18
/// FDDCCOMPEN field reset value
#define MDM_FDDCCOMPEN_RST          0x1
/// EQNOISEVARESTEN field reset value
#define MDM_EQNOISEVARESTEN_RST     0x1

/**
 * @brief Constructs a value for the EQUALCTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] fddcweightp2 - The value to use for the FDDCWEIGHTP2 field.
 * @param[in] fddcweightp1 - The value to use for the FDDCWEIGHTP1 field.
 * @param[in] fddcweightm1 - The value to use for the FDDCWEIGHTM1 field.
 * @param[in] fddcweightm2 - The value to use for the FDDCWEIGHTM2 field.
 * @param[in] noisevaradjustdb - The value to use for the NOISEVARADJUSTDB field.
 * @param[in] fddcsnrthreshold - The value to use for the FDDCSNRTHRESHOLD field.
 * @param[in] fddccompen - The value to use for the FDDCCOMPEN field.
 * @param[in] eqnoisevaresten - The value to use for the EQNOISEVARESTEN field.
 */
__INLINE void mdm_equalctrl0_pack(uint8_t fddcweightp2, uint8_t fddcweightp1, uint8_t fddcweightm1, uint8_t fddcweightm2, uint8_t noisevaradjustdb, uint8_t fddcsnrthreshold, uint8_t fddccompen, uint8_t eqnoisevaresten)
{
    ASSERT_ERR((((uint32_t)fddcweightp2 << 29) & ~((uint32_t)0xE0000000)) == 0);
    ASSERT_ERR((((uint32_t)fddcweightp1 << 26) & ~((uint32_t)0x1C000000)) == 0);
    ASSERT_ERR((((uint32_t)fddcweightm1 << 23) & ~((uint32_t)0x03800000)) == 0);
    ASSERT_ERR((((uint32_t)fddcweightm2 << 20) & ~((uint32_t)0x00700000)) == 0);
    ASSERT_ERR((((uint32_t)noisevaradjustdb << 12) & ~((uint32_t)0x000FF000)) == 0);
    ASSERT_ERR((((uint32_t)fddcsnrthreshold << 4) & ~((uint32_t)0x00000FF0)) == 0);
    ASSERT_ERR((((uint32_t)fddccompen << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)eqnoisevaresten << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR,  ((uint32_t)fddcweightp2 << 29) | ((uint32_t)fddcweightp1 << 26) | ((uint32_t)fddcweightm1 << 23) | ((uint32_t)fddcweightm2 << 20) | ((uint32_t)noisevaradjustdb << 12) | ((uint32_t)fddcsnrthreshold << 4) | ((uint32_t)fddccompen << 3) | ((uint32_t)eqnoisevaresten << 0));
}

/**
 * @brief Unpacks EQUALCTRL0's fields from current value of the EQUALCTRL0 register.
 *
 * Reads the EQUALCTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] fddcweightp2 - Will be populated with the current value of this field from the register.
 * @param[out] fddcweightp1 - Will be populated with the current value of this field from the register.
 * @param[out] fddcweightm1 - Will be populated with the current value of this field from the register.
 * @param[out] fddcweightm2 - Will be populated with the current value of this field from the register.
 * @param[out] noisevaradjustdb - Will be populated with the current value of this field from the register.
 * @param[out] fddcsnrthreshold - Will be populated with the current value of this field from the register.
 * @param[out] fddccompen - Will be populated with the current value of this field from the register.
 * @param[out] eqnoisevaresten - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_equalctrl0_unpack(uint8_t* fddcweightp2, uint8_t* fddcweightp1, uint8_t* fddcweightm1, uint8_t* fddcweightm2, uint8_t* noisevaradjustdb, uint8_t* fddcsnrthreshold, uint8_t* fddccompen, uint8_t* eqnoisevaresten)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);

    *fddcweightp2 = (localVal & ((uint32_t)0xE0000000)) >> 29;
    *fddcweightp1 = (localVal & ((uint32_t)0x1C000000)) >> 26;
    *fddcweightm1 = (localVal & ((uint32_t)0x03800000)) >> 23;
    *fddcweightm2 = (localVal & ((uint32_t)0x00700000)) >> 20;
    *noisevaradjustdb = (localVal & ((uint32_t)0x000FF000)) >> 12;
    *fddcsnrthreshold = (localVal & ((uint32_t)0x00000FF0)) >> 4;
    *fddccompen = (localVal & ((uint32_t)0x00000008)) >> 3;
    *eqnoisevaresten = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the FDDCWEIGHTP2 field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCWEIGHTP2 field's value will be returned.
 *
 * @return The current value of the FDDCWEIGHTP2 field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddcweightp2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0xE0000000)) >> 29);
}

/**
 * @brief Sets the FDDCWEIGHTP2 field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddcweightp2 - The value to set the field to.
 */
__INLINE void mdm_fddcweightp2_setf(uint8_t fddcweightp2)
{
    ASSERT_ERR((((uint32_t)fddcweightp2 << 29) & ~((uint32_t)0xE0000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0xE0000000)) | ((uint32_t)fddcweightp2 << 29));
}

/**
 * @brief Returns the current value of the FDDCWEIGHTP1 field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCWEIGHTP1 field's value will be returned.
 *
 * @return The current value of the FDDCWEIGHTP1 field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddcweightp1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x1C000000)) >> 26);
}

/**
 * @brief Sets the FDDCWEIGHTP1 field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddcweightp1 - The value to set the field to.
 */
__INLINE void mdm_fddcweightp1_setf(uint8_t fddcweightp1)
{
    ASSERT_ERR((((uint32_t)fddcweightp1 << 26) & ~((uint32_t)0x1C000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x1C000000)) | ((uint32_t)fddcweightp1 << 26));
}

/**
 * @brief Returns the current value of the FDDCWEIGHTM1 field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCWEIGHTM1 field's value will be returned.
 *
 * @return The current value of the FDDCWEIGHTM1 field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddcweightm1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x03800000)) >> 23);
}

/**
 * @brief Sets the FDDCWEIGHTM1 field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddcweightm1 - The value to set the field to.
 */
__INLINE void mdm_fddcweightm1_setf(uint8_t fddcweightm1)
{
    ASSERT_ERR((((uint32_t)fddcweightm1 << 23) & ~((uint32_t)0x03800000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x03800000)) | ((uint32_t)fddcweightm1 << 23));
}

/**
 * @brief Returns the current value of the FDDCWEIGHTM2 field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCWEIGHTM2 field's value will be returned.
 *
 * @return The current value of the FDDCWEIGHTM2 field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddcweightm2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Sets the FDDCWEIGHTM2 field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddcweightm2 - The value to set the field to.
 */
__INLINE void mdm_fddcweightm2_setf(uint8_t fddcweightm2)
{
    ASSERT_ERR((((uint32_t)fddcweightm2 << 20) & ~((uint32_t)0x00700000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x00700000)) | ((uint32_t)fddcweightm2 << 20));
}

/**
 * @brief Returns the current value of the NOISEVARADJUSTDB field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the NOISEVARADJUSTDB field's value will be returned.
 *
 * @return The current value of the NOISEVARADJUSTDB field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_noisevaradjustdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x000FF000)) >> 12);
}

/**
 * @brief Sets the NOISEVARADJUSTDB field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] noisevaradjustdb - The value to set the field to.
 */
__INLINE void mdm_noisevaradjustdb_setf(uint8_t noisevaradjustdb)
{
    ASSERT_ERR((((uint32_t)noisevaradjustdb << 12) & ~((uint32_t)0x000FF000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x000FF000)) | ((uint32_t)noisevaradjustdb << 12));
}

/**
 * @brief Returns the current value of the FDDCSNRTHRESHOLD field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCSNRTHRESHOLD field's value will be returned.
 *
 * @return The current value of the FDDCSNRTHRESHOLD field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddcsnrthreshold_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000FF0)) >> 4);
}

/**
 * @brief Sets the FDDCSNRTHRESHOLD field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddcsnrthreshold - The value to set the field to.
 */
__INLINE void mdm_fddcsnrthreshold_setf(uint8_t fddcsnrthreshold)
{
    ASSERT_ERR((((uint32_t)fddcsnrthreshold << 4) & ~((uint32_t)0x00000FF0)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x00000FF0)) | ((uint32_t)fddcsnrthreshold << 4));
}

/**
 * @brief Returns the current value of the FDDCCOMPEN field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the FDDCCOMPEN field's value will be returned.
 *
 * @return The current value of the FDDCCOMPEN field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_fddccompen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the FDDCCOMPEN field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fddccompen - The value to set the field to.
 */
__INLINE void mdm_fddccompen_setf(uint8_t fddccompen)
{
    ASSERT_ERR((((uint32_t)fddccompen << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)fddccompen << 3));
}

/**
 * @brief Returns the current value of the EQNOISEVARESTEN field in the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read and the EQNOISEVARESTEN field's value will be returned.
 *
 * @return The current value of the EQNOISEVARESTEN field in the EQUALCTRL0 register.
 */
__INLINE uint8_t mdm_eqnoisevaresten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the EQNOISEVARESTEN field of the EQUALCTRL0 register.
 *
 * The EQUALCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] eqnoisevaresten - The value to set the field to.
 */
__INLINE void mdm_eqnoisevaresten_setf(uint8_t eqnoisevaresten)
{
    ASSERT_ERR((((uint32_t)eqnoisevaresten << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_EQUALCTRL0_ADDR, (REG_PL_RD(MDM_EQUALCTRL0_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)eqnoisevaresten << 0));
}

/// @}

/**
 * @name SMOOTHCTRL register definitions
 * <table>
 * <caption id="SMOOTHCTRL_BF">SMOOTHCTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>27:20 <td>     TDCYCROTVAL80 <td>R <td>R/W <td>0x18
 * <tr><td>19 <td>          SMOOTHEN <td>R <td>R/W <td>1
 * <tr><td>15:08 <td>     TDCYCROTVAL40 <td>R <td>R/W <td>0xC
 * <tr><td>07:00 <td>     TDCYCROTVAL20 <td>R <td>R/W <td>0x6
 * </table>
 *
 * @{
 */

/// Address of the SMOOTHCTRL register
#define MDM_SMOOTHCTRL_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0818)
/// Offset of the SMOOTHCTRL register from the base address
#define MDM_SMOOTHCTRL_OFFSET 0x00000018
/// Index of the SMOOTHCTRL register
#define MDM_SMOOTHCTRL_INDEX  0x00000006
/// Reset value of the SMOOTHCTRL register
#define MDM_SMOOTHCTRL_RESET  0x01880C06

/**
 * @brief Returns the current value of the SMOOTHCTRL register.
 * The SMOOTHCTRL register will be read and its value returned.
 * @return The current value of the SMOOTHCTRL register.
 */
__INLINE uint32_t mdm_smoothctrl_get(void)
{
    return REG_PL_RD(MDM_SMOOTHCTRL_ADDR);
}

/**
 * @brief Sets the SMOOTHCTRL register to a value.
 * The SMOOTHCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_smoothctrl_set(uint32_t value)
{
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR, value);
}

// field definitions
/// TDCYCROTVAL80 field mask
#define MDM_TDCYCROTVAL80_MASK   ((uint32_t)0x0FF00000)
/// TDCYCROTVAL80 field LSB position
#define MDM_TDCYCROTVAL80_LSB    20
/// TDCYCROTVAL80 field width
#define MDM_TDCYCROTVAL80_WIDTH  ((uint32_t)0x00000008)
/// SMOOTHEN field bit
#define MDM_SMOOTHEN_BIT         ((uint32_t)0x00080000)
/// SMOOTHEN field position
#define MDM_SMOOTHEN_POS         19
/// TDCYCROTVAL40 field mask
#define MDM_TDCYCROTVAL40_MASK   ((uint32_t)0x0000FF00)
/// TDCYCROTVAL40 field LSB position
#define MDM_TDCYCROTVAL40_LSB    8
/// TDCYCROTVAL40 field width
#define MDM_TDCYCROTVAL40_WIDTH  ((uint32_t)0x00000008)
/// TDCYCROTVAL20 field mask
#define MDM_TDCYCROTVAL20_MASK   ((uint32_t)0x000000FF)
/// TDCYCROTVAL20 field LSB position
#define MDM_TDCYCROTVAL20_LSB    0
/// TDCYCROTVAL20 field width
#define MDM_TDCYCROTVAL20_WIDTH  ((uint32_t)0x00000008)

/// TDCYCROTVAL80 field reset value
#define MDM_TDCYCROTVAL80_RST    0x18
/// SMOOTHEN field reset value
#define MDM_SMOOTHEN_RST         0x1
/// TDCYCROTVAL40 field reset value
#define MDM_TDCYCROTVAL40_RST    0xC
/// TDCYCROTVAL20 field reset value
#define MDM_TDCYCROTVAL20_RST    0x6

/**
 * @brief Constructs a value for the SMOOTHCTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tdcycrotval80 - The value to use for the TDCYCROTVAL80 field.
 * @param[in] smoothen - The value to use for the SMOOTHEN field.
 * @param[in] tdcycrotval40 - The value to use for the TDCYCROTVAL40 field.
 * @param[in] tdcycrotval20 - The value to use for the TDCYCROTVAL20 field.
 */
__INLINE void mdm_smoothctrl_pack(uint8_t tdcycrotval80, uint8_t smoothen, uint8_t tdcycrotval40, uint8_t tdcycrotval20)
{
    ASSERT_ERR((((uint32_t)tdcycrotval80 << 20) & ~((uint32_t)0x0FF00000)) == 0);
    ASSERT_ERR((((uint32_t)smoothen << 19) & ~((uint32_t)0x00080000)) == 0);
    ASSERT_ERR((((uint32_t)tdcycrotval40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)tdcycrotval20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR,  ((uint32_t)tdcycrotval80 << 20) | ((uint32_t)smoothen << 19) | ((uint32_t)tdcycrotval40 << 8) | ((uint32_t)tdcycrotval20 << 0));
}

/**
 * @brief Unpacks SMOOTHCTRL's fields from current value of the SMOOTHCTRL register.
 *
 * Reads the SMOOTHCTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tdcycrotval80 - Will be populated with the current value of this field from the register.
 * @param[out] smoothen - Will be populated with the current value of this field from the register.
 * @param[out] tdcycrotval40 - Will be populated with the current value of this field from the register.
 * @param[out] tdcycrotval20 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_smoothctrl_unpack(uint8_t* tdcycrotval80, uint8_t* smoothen, uint8_t* tdcycrotval40, uint8_t* tdcycrotval20)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHCTRL_ADDR);

    *tdcycrotval80 = (localVal & ((uint32_t)0x0FF00000)) >> 20;
    *smoothen = (localVal & ((uint32_t)0x00080000)) >> 19;
    *tdcycrotval40 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *tdcycrotval20 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TDCYCROTVAL80 field in the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read and the TDCYCROTVAL80 field's value will be returned.
 *
 * @return The current value of the TDCYCROTVAL80 field in the SMOOTHCTRL register.
 */
__INLINE uint8_t mdm_tdcycrotval80_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHCTRL_ADDR);
    return ((localVal & ((uint32_t)0x0FF00000)) >> 20);
}

/**
 * @brief Sets the TDCYCROTVAL80 field of the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdcycrotval80 - The value to set the field to.
 */
__INLINE void mdm_tdcycrotval80_setf(uint8_t tdcycrotval80)
{
    ASSERT_ERR((((uint32_t)tdcycrotval80 << 20) & ~((uint32_t)0x0FF00000)) == 0);
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR, (REG_PL_RD(MDM_SMOOTHCTRL_ADDR) & ~((uint32_t)0x0FF00000)) | ((uint32_t)tdcycrotval80 << 20));
}

/**
 * @brief Returns the current value of the SMOOTHEN field in the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read and the SMOOTHEN field's value will be returned.
 *
 * @return The current value of the SMOOTHEN field in the SMOOTHCTRL register.
 */
__INLINE uint8_t mdm_smoothen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

/**
 * @brief Sets the SMOOTHEN field of the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] smoothen - The value to set the field to.
 */
__INLINE void mdm_smoothen_setf(uint8_t smoothen)
{
    ASSERT_ERR((((uint32_t)smoothen << 19) & ~((uint32_t)0x00080000)) == 0);
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR, (REG_PL_RD(MDM_SMOOTHCTRL_ADDR) & ~((uint32_t)0x00080000)) | ((uint32_t)smoothen << 19));
}

/**
 * @brief Returns the current value of the TDCYCROTVAL40 field in the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read and the TDCYCROTVAL40 field's value will be returned.
 *
 * @return The current value of the TDCYCROTVAL40 field in the SMOOTHCTRL register.
 */
__INLINE uint8_t mdm_tdcycrotval40_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHCTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the TDCYCROTVAL40 field of the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdcycrotval40 - The value to set the field to.
 */
__INLINE void mdm_tdcycrotval40_setf(uint8_t tdcycrotval40)
{
    ASSERT_ERR((((uint32_t)tdcycrotval40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR, (REG_PL_RD(MDM_SMOOTHCTRL_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)tdcycrotval40 << 8));
}

/**
 * @brief Returns the current value of the TDCYCROTVAL20 field in the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read and the TDCYCROTVAL20 field's value will be returned.
 *
 * @return The current value of the TDCYCROTVAL20 field in the SMOOTHCTRL register.
 */
__INLINE uint8_t mdm_tdcycrotval20_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHCTRL_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TDCYCROTVAL20 field of the SMOOTHCTRL register.
 *
 * The SMOOTHCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdcycrotval20 - The value to set the field to.
 */
__INLINE void mdm_tdcycrotval20_setf(uint8_t tdcycrotval20)
{
    ASSERT_ERR((((uint32_t)tdcycrotval20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_SMOOTHCTRL_ADDR, (REG_PL_RD(MDM_SMOOTHCTRL_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)tdcycrotval20 << 0));
}

/// @}

/**
 * @name DCESTIMCTRL register definitions
 * <table>
 * <caption id="DCESTIMCTRL_BF">DCESTIMCTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:28 <td>         STARTHTDC <td>R <td>R/W <td>0x0
 * <tr><td>27:24 <td>           STARTDC <td>R <td>R/W <td>0x0
 * <tr><td>22:16 <td>     DELAYSYNCTD20 <td>R <td>R/W <td>0x0
 * <tr><td>14:08 <td>         DELAYSYNC <td>R <td>R/W <td>0xF
 * <tr><td>06:00 <td>         WAITHTSTF <td>R <td>R/W <td>0xF
 * </table>
 *
 * @{
 */

/// Address of the DCESTIMCTRL register
#define MDM_DCESTIMCTRL_ADDR   (REG_MDM_CFG_BASE_ADDR + 0x081C)
/// Offset of the DCESTIMCTRL register from the base address
#define MDM_DCESTIMCTRL_OFFSET 0x0000001C
/// Index of the DCESTIMCTRL register
#define MDM_DCESTIMCTRL_INDEX  0x00000007
/// Reset value of the DCESTIMCTRL register
#define MDM_DCESTIMCTRL_RESET  0x00000F0F

/**
 * @brief Returns the current value of the DCESTIMCTRL register.
 * The DCESTIMCTRL register will be read and its value returned.
 * @return The current value of the DCESTIMCTRL register.
 */
__INLINE uint32_t mdm_dcestimctrl_get(void)
{
    return REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
}

/**
 * @brief Sets the DCESTIMCTRL register to a value.
 * The DCESTIMCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_dcestimctrl_set(uint32_t value)
{
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, value);
}

// field definitions
/// STARTHTDC field mask
#define MDM_STARTHTDC_MASK       ((uint32_t)0xF0000000)
/// STARTHTDC field LSB position
#define MDM_STARTHTDC_LSB        28
/// STARTHTDC field width
#define MDM_STARTHTDC_WIDTH      ((uint32_t)0x00000004)
/// STARTDC field mask
#define MDM_STARTDC_MASK         ((uint32_t)0x0F000000)
/// STARTDC field LSB position
#define MDM_STARTDC_LSB          24
/// STARTDC field width
#define MDM_STARTDC_WIDTH        ((uint32_t)0x00000004)
/// DELAYSYNCTD20 field mask
#define MDM_DELAYSYNCTD20_MASK   ((uint32_t)0x007F0000)
/// DELAYSYNCTD20 field LSB position
#define MDM_DELAYSYNCTD20_LSB    16
/// DELAYSYNCTD20 field width
#define MDM_DELAYSYNCTD20_WIDTH  ((uint32_t)0x00000007)
/// DELAYSYNC field mask
#define MDM_DELAYSYNC_MASK       ((uint32_t)0x00007F00)
/// DELAYSYNC field LSB position
#define MDM_DELAYSYNC_LSB        8
/// DELAYSYNC field width
#define MDM_DELAYSYNC_WIDTH      ((uint32_t)0x00000007)
/// WAITHTSTF field mask
#define MDM_WAITHTSTF_MASK       ((uint32_t)0x0000007F)
/// WAITHTSTF field LSB position
#define MDM_WAITHTSTF_LSB        0
/// WAITHTSTF field width
#define MDM_WAITHTSTF_WIDTH      ((uint32_t)0x00000007)

/// STARTHTDC field reset value
#define MDM_STARTHTDC_RST        0x0
/// STARTDC field reset value
#define MDM_STARTDC_RST          0x0
/// DELAYSYNCTD20 field reset value
#define MDM_DELAYSYNCTD20_RST    0x0
/// DELAYSYNC field reset value
#define MDM_DELAYSYNC_RST        0xF
/// WAITHTSTF field reset value
#define MDM_WAITHTSTF_RST        0xF

/**
 * @brief Constructs a value for the DCESTIMCTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] starthtdc - The value to use for the STARTHTDC field.
 * @param[in] startdc - The value to use for the STARTDC field.
 * @param[in] delaysynctd20 - The value to use for the DELAYSYNCTD20 field.
 * @param[in] delaysync - The value to use for the DELAYSYNC field.
 * @param[in] waithtstf - The value to use for the WAITHTSTF field.
 */
__INLINE void mdm_dcestimctrl_pack(uint8_t starthtdc, uint8_t startdc, uint8_t delaysynctd20, uint8_t delaysync, uint8_t waithtstf)
{
    ASSERT_ERR((((uint32_t)starthtdc << 28) & ~((uint32_t)0xF0000000)) == 0);
    ASSERT_ERR((((uint32_t)startdc << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)delaysynctd20 << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)delaysync << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)waithtstf << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR,  ((uint32_t)starthtdc << 28) | ((uint32_t)startdc << 24) | ((uint32_t)delaysynctd20 << 16) | ((uint32_t)delaysync << 8) | ((uint32_t)waithtstf << 0));
}

/**
 * @brief Unpacks DCESTIMCTRL's fields from current value of the DCESTIMCTRL register.
 *
 * Reads the DCESTIMCTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] starthtdc - Will be populated with the current value of this field from the register.
 * @param[out] startdc - Will be populated with the current value of this field from the register.
 * @param[out] delaysynctd20 - Will be populated with the current value of this field from the register.
 * @param[out] delaysync - Will be populated with the current value of this field from the register.
 * @param[out] waithtstf - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_dcestimctrl_unpack(uint8_t* starthtdc, uint8_t* startdc, uint8_t* delaysynctd20, uint8_t* delaysync, uint8_t* waithtstf)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);

    *starthtdc = (localVal & ((uint32_t)0xF0000000)) >> 28;
    *startdc = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *delaysynctd20 = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *delaysync = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *waithtstf = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the STARTHTDC field in the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read and the STARTHTDC field's value will be returned.
 *
 * @return The current value of the STARTHTDC field in the DCESTIMCTRL register.
 */
__INLINE uint8_t mdm_starthtdc_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
    return ((localVal & ((uint32_t)0xF0000000)) >> 28);
}

/**
 * @brief Sets the STARTHTDC field of the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] starthtdc - The value to set the field to.
 */
__INLINE void mdm_starthtdc_setf(uint8_t starthtdc)
{
    ASSERT_ERR((((uint32_t)starthtdc << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, (REG_PL_RD(MDM_DCESTIMCTRL_ADDR) & ~((uint32_t)0xF0000000)) | ((uint32_t)starthtdc << 28));
}

/**
 * @brief Returns the current value of the STARTDC field in the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read and the STARTDC field's value will be returned.
 *
 * @return The current value of the STARTDC field in the DCESTIMCTRL register.
 */
__INLINE uint8_t mdm_startdc_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the STARTDC field of the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] startdc - The value to set the field to.
 */
__INLINE void mdm_startdc_setf(uint8_t startdc)
{
    ASSERT_ERR((((uint32_t)startdc << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, (REG_PL_RD(MDM_DCESTIMCTRL_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)startdc << 24));
}

/**
 * @brief Returns the current value of the DELAYSYNCTD20 field in the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read and the DELAYSYNCTD20 field's value will be returned.
 *
 * @return The current value of the DELAYSYNCTD20 field in the DCESTIMCTRL register.
 */
__INLINE uint8_t mdm_delaysynctd20_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the DELAYSYNCTD20 field of the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] delaysynctd20 - The value to set the field to.
 */
__INLINE void mdm_delaysynctd20_setf(uint8_t delaysynctd20)
{
    ASSERT_ERR((((uint32_t)delaysynctd20 << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, (REG_PL_RD(MDM_DCESTIMCTRL_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)delaysynctd20 << 16));
}

/**
 * @brief Returns the current value of the DELAYSYNC field in the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read and the DELAYSYNC field's value will be returned.
 *
 * @return The current value of the DELAYSYNC field in the DCESTIMCTRL register.
 */
__INLINE uint8_t mdm_delaysync_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the DELAYSYNC field of the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] delaysync - The value to set the field to.
 */
__INLINE void mdm_delaysync_setf(uint8_t delaysync)
{
    ASSERT_ERR((((uint32_t)delaysync << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, (REG_PL_RD(MDM_DCESTIMCTRL_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)delaysync << 8));
}

/**
 * @brief Returns the current value of the WAITHTSTF field in the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read and the WAITHTSTF field's value will be returned.
 *
 * @return The current value of the WAITHTSTF field in the DCESTIMCTRL register.
 */
__INLINE uint8_t mdm_waithtstf_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_DCESTIMCTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the WAITHTSTF field of the DCESTIMCTRL register.
 *
 * The DCESTIMCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] waithtstf - The value to set the field to.
 */
__INLINE void mdm_waithtstf_setf(uint8_t waithtstf)
{
    ASSERT_ERR((((uint32_t)waithtstf << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MDM_DCESTIMCTRL_ADDR, (REG_PL_RD(MDM_DCESTIMCTRL_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)waithtstf << 0));
}

/// @}

/**
 * @name RXMODE register definitions
 * <table>
 * <caption id="RXMODE_BF">RXMODE bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>20 <td> RXMUMIMOAPEPLENEN <td>R <td>R/W <td>1
 * <tr><td>16 <td>        RXMUMIMOEN <td>R <td>R/W <td>0
 * <tr><td>14:12 <td>      RXNDPNSTSMAX <td>R <td>R/W <td>0x3
 * <tr><td>11 <td>           RXESSEN <td>R <td>R/W <td>0
 * <tr><td>10 <td>       RXUNEQMODEN <td>R <td>R/W <td>0
 * <tr><td>09 <td>          RXSTBCEN <td>R <td>R/W <td>1
 * <tr><td>08 <td>          RXLDPCEN <td>R <td>R/W <td>1
 * <tr><td>06:04 <td>          RXNSSMAX <td>R <td>R/W <td>0x1
 * <tr><td>03 <td>            RXGFEN <td>R <td>R/W <td>1
 * <tr><td>02 <td>            RXMMEN <td>R <td>R/W <td>1
 * <tr><td>01 <td>           RXVHTEN <td>R <td>R/W <td>1
 * <tr><td>00 <td>          RXDSSSEN <td>R <td>R/W <td>1
 * </table>
 *
 * @{
 */

/// Address of the RXMODE register
#define MDM_RXMODE_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0820)
/// Offset of the RXMODE register from the base address
#define MDM_RXMODE_OFFSET 0x00000020
/// Index of the RXMODE register
#define MDM_RXMODE_INDEX  0x00000008
/// Reset value of the RXMODE register
#define MDM_RXMODE_RESET  0x0010331F

/**
 * @brief Returns the current value of the RXMODE register.
 * The RXMODE register will be read and its value returned.
 * @return The current value of the RXMODE register.
 */
__INLINE uint32_t mdm_rxmode_get(void)
{
    return REG_PL_RD(MDM_RXMODE_ADDR);
}

/**
 * @brief Sets the RXMODE register to a value.
 * The RXMODE register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxmode_set(uint32_t value)
{
    REG_PL_WR(MDM_RXMODE_ADDR, value);
}

// field definitions
/// RXMUMIMOAPEPLENEN field bit
#define MDM_RXMUMIMOAPEPLENEN_BIT    ((uint32_t)0x00100000)
/// RXMUMIMOAPEPLENEN field position
#define MDM_RXMUMIMOAPEPLENEN_POS    20
/// RXMUMIMOEN field bit
#define MDM_RXMUMIMOEN_BIT           ((uint32_t)0x00010000)
/// RXMUMIMOEN field position
#define MDM_RXMUMIMOEN_POS           16
/// RXNDPNSTSMAX field mask
#define MDM_RXNDPNSTSMAX_MASK        ((uint32_t)0x00007000)
/// RXNDPNSTSMAX field LSB position
#define MDM_RXNDPNSTSMAX_LSB         12
/// RXNDPNSTSMAX field width
#define MDM_RXNDPNSTSMAX_WIDTH       ((uint32_t)0x00000003)
/// RXESSEN field bit
#define MDM_RXESSEN_BIT              ((uint32_t)0x00000800)
/// RXESSEN field position
#define MDM_RXESSEN_POS              11
/// RXUNEQMODEN field bit
#define MDM_RXUNEQMODEN_BIT          ((uint32_t)0x00000400)
/// RXUNEQMODEN field position
#define MDM_RXUNEQMODEN_POS          10
/// RXSTBCEN field bit
#define MDM_RXSTBCEN_BIT             ((uint32_t)0x00000200)
/// RXSTBCEN field position
#define MDM_RXSTBCEN_POS             9
/// RXLDPCEN field bit
#define MDM_RXLDPCEN_BIT             ((uint32_t)0x00000100)
/// RXLDPCEN field position
#define MDM_RXLDPCEN_POS             8
/// RXNSSMAX field mask
#define MDM_RXNSSMAX_MASK            ((uint32_t)0x00000070)
/// RXNSSMAX field LSB position
#define MDM_RXNSSMAX_LSB             4
/// RXNSSMAX field width
#define MDM_RXNSSMAX_WIDTH           ((uint32_t)0x00000003)
/// RXGFEN field bit
#define MDM_RXGFEN_BIT               ((uint32_t)0x00000008)
/// RXGFEN field position
#define MDM_RXGFEN_POS               3
/// RXMMEN field bit
#define MDM_RXMMEN_BIT               ((uint32_t)0x00000004)
/// RXMMEN field position
#define MDM_RXMMEN_POS               2
/// RXVHTEN field bit
#define MDM_RXVHTEN_BIT              ((uint32_t)0x00000002)
/// RXVHTEN field position
#define MDM_RXVHTEN_POS              1
/// RXDSSSEN field bit
#define MDM_RXDSSSEN_BIT             ((uint32_t)0x00000001)
/// RXDSSSEN field position
#define MDM_RXDSSSEN_POS             0

/// RXMUMIMOAPEPLENEN field reset value
#define MDM_RXMUMIMOAPEPLENEN_RST    0x1
/// RXMUMIMOEN field reset value
#define MDM_RXMUMIMOEN_RST           0x0
/// RXNDPNSTSMAX field reset value
#define MDM_RXNDPNSTSMAX_RST         0x3
/// RXESSEN field reset value
#define MDM_RXESSEN_RST              0x0
/// RXUNEQMODEN field reset value
#define MDM_RXUNEQMODEN_RST          0x0
/// RXSTBCEN field reset value
#define MDM_RXSTBCEN_RST             0x1
/// RXLDPCEN field reset value
#define MDM_RXLDPCEN_RST             0x1
/// RXNSSMAX field reset value
#define MDM_RXNSSMAX_RST             0x1
/// RXGFEN field reset value
#define MDM_RXGFEN_RST               0x1
/// RXMMEN field reset value
#define MDM_RXMMEN_RST               0x1
/// RXVHTEN field reset value
#define MDM_RXVHTEN_RST              0x1
/// RXDSSSEN field reset value
#define MDM_RXDSSSEN_RST             0x1

/**
 * @brief Constructs a value for the RXMODE register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxmumimoapeplenen - The value to use for the RXMUMIMOAPEPLENEN field.
 * @param[in] rxmumimoen - The value to use for the RXMUMIMOEN field.
 * @param[in] rxndpnstsmax - The value to use for the RXNDPNSTSMAX field.
 * @param[in] rxessen - The value to use for the RXESSEN field.
 * @param[in] rxuneqmoden - The value to use for the RXUNEQMODEN field.
 * @param[in] rxstbcen - The value to use for the RXSTBCEN field.
 * @param[in] rxldpcen - The value to use for the RXLDPCEN field.
 * @param[in] rxnssmax - The value to use for the RXNSSMAX field.
 * @param[in] rxgfen - The value to use for the RXGFEN field.
 * @param[in] rxmmen - The value to use for the RXMMEN field.
 * @param[in] rxvhten - The value to use for the RXVHTEN field.
 * @param[in] rxdsssen - The value to use for the RXDSSSEN field.
 */
__INLINE void mdm_rxmode_pack(uint8_t rxmumimoapeplenen, uint8_t rxmumimoen, uint8_t rxndpnstsmax, uint8_t rxessen, uint8_t rxuneqmoden, uint8_t rxstbcen, uint8_t rxldpcen, uint8_t rxnssmax, uint8_t rxgfen, uint8_t rxmmen, uint8_t rxvhten, uint8_t rxdsssen)
{
    ASSERT_ERR((((uint32_t)rxmumimoapeplenen << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)rxmumimoen << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)rxndpnstsmax << 12) & ~((uint32_t)0x00007000)) == 0);
    ASSERT_ERR((((uint32_t)rxessen << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)rxuneqmoden << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)rxstbcen << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)rxldpcen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)rxnssmax << 4) & ~((uint32_t)0x00000070)) == 0);
    ASSERT_ERR((((uint32_t)rxgfen << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)rxmmen << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)rxvhten << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)rxdsssen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR,  ((uint32_t)rxmumimoapeplenen << 20) | ((uint32_t)rxmumimoen << 16) | ((uint32_t)rxndpnstsmax << 12) | ((uint32_t)rxessen << 11) | ((uint32_t)rxuneqmoden << 10) | ((uint32_t)rxstbcen << 9) | ((uint32_t)rxldpcen << 8) | ((uint32_t)rxnssmax << 4) | ((uint32_t)rxgfen << 3) | ((uint32_t)rxmmen << 2) | ((uint32_t)rxvhten << 1) | ((uint32_t)rxdsssen << 0));
}

/**
 * @brief Unpacks RXMODE's fields from current value of the RXMODE register.
 *
 * Reads the RXMODE register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxmumimoapeplenen - Will be populated with the current value of this field from the register.
 * @param[out] rxmumimoen - Will be populated with the current value of this field from the register.
 * @param[out] rxndpnstsmax - Will be populated with the current value of this field from the register.
 * @param[out] rxessen - Will be populated with the current value of this field from the register.
 * @param[out] rxuneqmoden - Will be populated with the current value of this field from the register.
 * @param[out] rxstbcen - Will be populated with the current value of this field from the register.
 * @param[out] rxldpcen - Will be populated with the current value of this field from the register.
 * @param[out] rxnssmax - Will be populated with the current value of this field from the register.
 * @param[out] rxgfen - Will be populated with the current value of this field from the register.
 * @param[out] rxmmen - Will be populated with the current value of this field from the register.
 * @param[out] rxvhten - Will be populated with the current value of this field from the register.
 * @param[out] rxdsssen - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_rxmode_unpack(uint8_t* rxmumimoapeplenen, uint8_t* rxmumimoen, uint8_t* rxndpnstsmax, uint8_t* rxessen, uint8_t* rxuneqmoden, uint8_t* rxstbcen, uint8_t* rxldpcen, uint8_t* rxnssmax, uint8_t* rxgfen, uint8_t* rxmmen, uint8_t* rxvhten, uint8_t* rxdsssen)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);

    *rxmumimoapeplenen = (localVal & ((uint32_t)0x00100000)) >> 20;
    *rxmumimoen = (localVal & ((uint32_t)0x00010000)) >> 16;
    *rxndpnstsmax = (localVal & ((uint32_t)0x00007000)) >> 12;
    *rxessen = (localVal & ((uint32_t)0x00000800)) >> 11;
    *rxuneqmoden = (localVal & ((uint32_t)0x00000400)) >> 10;
    *rxstbcen = (localVal & ((uint32_t)0x00000200)) >> 9;
    *rxldpcen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *rxnssmax = (localVal & ((uint32_t)0x00000070)) >> 4;
    *rxgfen = (localVal & ((uint32_t)0x00000008)) >> 3;
    *rxmmen = (localVal & ((uint32_t)0x00000004)) >> 2;
    *rxvhten = (localVal & ((uint32_t)0x00000002)) >> 1;
    *rxdsssen = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the RXMUMIMOAPEPLENEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXMUMIMOAPEPLENEN field's value will be returned.
 *
 * @return The current value of the RXMUMIMOAPEPLENEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxmumimoapeplenen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the RXMUMIMOAPEPLENEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxmumimoapeplenen - The value to set the field to.
 */
__INLINE void mdm_rxmumimoapeplenen_setf(uint8_t rxmumimoapeplenen)
{
    ASSERT_ERR((((uint32_t)rxmumimoapeplenen << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)rxmumimoapeplenen << 20));
}

/**
 * @brief Returns the current value of the RXMUMIMOEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXMUMIMOEN field's value will be returned.
 *
 * @return The current value of the RXMUMIMOEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxmumimoen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the RXMUMIMOEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxmumimoen - The value to set the field to.
 */
__INLINE void mdm_rxmumimoen_setf(uint8_t rxmumimoen)
{
    ASSERT_ERR((((uint32_t)rxmumimoen << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)rxmumimoen << 16));
}

/**
 * @brief Returns the current value of the RXNDPNSTSMAX field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXNDPNSTSMAX field's value will be returned.
 *
 * @return The current value of the RXNDPNSTSMAX field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxndpnstsmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00007000)) >> 12);
}

/**
 * @brief Sets the RXNDPNSTSMAX field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxndpnstsmax - The value to set the field to.
 */
__INLINE void mdm_rxndpnstsmax_setf(uint8_t rxndpnstsmax)
{
    ASSERT_ERR((((uint32_t)rxndpnstsmax << 12) & ~((uint32_t)0x00007000)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00007000)) | ((uint32_t)rxndpnstsmax << 12));
}

/**
 * @brief Returns the current value of the RXESSEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXESSEN field's value will be returned.
 *
 * @return The current value of the RXESSEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxessen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

/**
 * @brief Sets the RXESSEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxessen - The value to set the field to.
 */
__INLINE void mdm_rxessen_setf(uint8_t rxessen)
{
    ASSERT_ERR((((uint32_t)rxessen << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)rxessen << 11));
}

/**
 * @brief Returns the current value of the RXUNEQMODEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXUNEQMODEN field's value will be returned.
 *
 * @return The current value of the RXUNEQMODEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxuneqmoden_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

/**
 * @brief Sets the RXUNEQMODEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxuneqmoden - The value to set the field to.
 */
__INLINE void mdm_rxuneqmoden_setf(uint8_t rxuneqmoden)
{
    ASSERT_ERR((((uint32_t)rxuneqmoden << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)rxuneqmoden << 10));
}

/**
 * @brief Returns the current value of the RXSTBCEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXSTBCEN field's value will be returned.
 *
 * @return The current value of the RXSTBCEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxstbcen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the RXSTBCEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxstbcen - The value to set the field to.
 */
__INLINE void mdm_rxstbcen_setf(uint8_t rxstbcen)
{
    ASSERT_ERR((((uint32_t)rxstbcen << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)rxstbcen << 9));
}

/**
 * @brief Returns the current value of the RXLDPCEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXLDPCEN field's value will be returned.
 *
 * @return The current value of the RXLDPCEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxldpcen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the RXLDPCEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxldpcen - The value to set the field to.
 */
__INLINE void mdm_rxldpcen_setf(uint8_t rxldpcen)
{
    ASSERT_ERR((((uint32_t)rxldpcen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)rxldpcen << 8));
}

/**
 * @brief Returns the current value of the RXNSSMAX field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXNSSMAX field's value will be returned.
 *
 * @return The current value of the RXNSSMAX field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxnssmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000070)) >> 4);
}

/**
 * @brief Sets the RXNSSMAX field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxnssmax - The value to set the field to.
 */
__INLINE void mdm_rxnssmax_setf(uint8_t rxnssmax)
{
    ASSERT_ERR((((uint32_t)rxnssmax << 4) & ~((uint32_t)0x00000070)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000070)) | ((uint32_t)rxnssmax << 4));
}

/**
 * @brief Returns the current value of the RXGFEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXGFEN field's value will be returned.
 *
 * @return The current value of the RXGFEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxgfen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the RXGFEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxgfen - The value to set the field to.
 */
__INLINE void mdm_rxgfen_setf(uint8_t rxgfen)
{
    ASSERT_ERR((((uint32_t)rxgfen << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)rxgfen << 3));
}

/**
 * @brief Returns the current value of the RXMMEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXMMEN field's value will be returned.
 *
 * @return The current value of the RXMMEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxmmen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the RXMMEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxmmen - The value to set the field to.
 */
__INLINE void mdm_rxmmen_setf(uint8_t rxmmen)
{
    ASSERT_ERR((((uint32_t)rxmmen << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)rxmmen << 2));
}

/**
 * @brief Returns the current value of the RXVHTEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXVHTEN field's value will be returned.
 *
 * @return The current value of the RXVHTEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxvhten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the RXVHTEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxvhten - The value to set the field to.
 */
__INLINE void mdm_rxvhten_setf(uint8_t rxvhten)
{
    ASSERT_ERR((((uint32_t)rxvhten << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)rxvhten << 1));
}

/**
 * @brief Returns the current value of the RXDSSSEN field in the RXMODE register.
 *
 * The RXMODE register will be read and the RXDSSSEN field's value will be returned.
 *
 * @return The current value of the RXDSSSEN field in the RXMODE register.
 */
__INLINE uint8_t mdm_rxdsssen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the RXDSSSEN field of the RXMODE register.
 *
 * The RXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxdsssen - The value to set the field to.
 */
__INLINE void mdm_rxdsssen_setf(uint8_t rxdsssen)
{
    ASSERT_ERR((((uint32_t)rxdsssen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_RXMODE_ADDR, (REG_PL_RD(MDM_RXMODE_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)rxdsssen << 0));
}

/// @}

/**
 * @name TXMODE register definitions
 * <table>
 * <caption id="TXMODE_BF">TXMODE bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>25:24 <td>          TXCBWMAX <td>R <td>R/W <td>0x2
 * <tr><td>22:20 <td>            NTXMAX <td>R <td>R/W <td>0x1
 * <tr><td>16 <td>        TXMUMIMOEN <td>R <td>R/W <td>0
 * <tr><td>11 <td>           TXESSEN <td>R <td>R/W <td>0
 * <tr><td>10 <td>       TXUNEQMODEN <td>R <td>R/W <td>0
 * <tr><td>09 <td>          TXSTBCEN <td>R <td>R/W <td>1
 * <tr><td>08 <td>          TXLDPCEN <td>R <td>R/W <td>1
 * <tr><td>06:04 <td>          TXNSSMAX <td>R <td>R/W <td>0x1
 * <tr><td>03 <td>            TXGFEN <td>R <td>R/W <td>1
 * <tr><td>02 <td>            TXMMEN <td>R <td>R/W <td>1
 * <tr><td>01 <td>           TXVHTEN <td>R <td>R/W <td>1
 * <tr><td>00 <td>          TXDSSSEN <td>R <td>R/W <td>1
 * </table>
 *
 * @{
 */

/// Address of the TXMODE register
#define MDM_TXMODE_ADDR   (REG_MDM_CFG_BASE_ADDR + 0x0824)
/// Offset of the TXMODE register from the base address
#define MDM_TXMODE_OFFSET 0x00000024
/// Index of the TXMODE register
#define MDM_TXMODE_INDEX  0x00000009
/// Reset value of the TXMODE register
#define MDM_TXMODE_RESET  0x0210031F

/**
 * @brief Returns the current value of the TXMODE register.
 * The TXMODE register will be read and its value returned.
 * @return The current value of the TXMODE register.
 */
__INLINE uint32_t mdm_txmode_get(void)
{
    return REG_PL_RD(MDM_TXMODE_ADDR);
}

/**
 * @brief Sets the TXMODE register to a value.
 * The TXMODE register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_txmode_set(uint32_t value)
{
    REG_PL_WR(MDM_TXMODE_ADDR, value);
}

// field definitions
/// TXCBWMAX field mask
#define MDM_TXCBWMAX_MASK      ((uint32_t)0x03000000)
/// TXCBWMAX field LSB position
#define MDM_TXCBWMAX_LSB       24
/// TXCBWMAX field width
#define MDM_TXCBWMAX_WIDTH     ((uint32_t)0x00000002)
/// NTXMAX field mask
#define MDM_NTXMAX_MASK        ((uint32_t)0x00700000)
/// NTXMAX field LSB position
#define MDM_NTXMAX_LSB         20
/// NTXMAX field width
#define MDM_NTXMAX_WIDTH       ((uint32_t)0x00000003)
/// TXMUMIMOEN field bit
#define MDM_TXMUMIMOEN_BIT     ((uint32_t)0x00010000)
/// TXMUMIMOEN field position
#define MDM_TXMUMIMOEN_POS     16
/// TXESSEN field bit
#define MDM_TXESSEN_BIT        ((uint32_t)0x00000800)
/// TXESSEN field position
#define MDM_TXESSEN_POS        11
/// TXUNEQMODEN field bit
#define MDM_TXUNEQMODEN_BIT    ((uint32_t)0x00000400)
/// TXUNEQMODEN field position
#define MDM_TXUNEQMODEN_POS    10
/// TXSTBCEN field bit
#define MDM_TXSTBCEN_BIT       ((uint32_t)0x00000200)
/// TXSTBCEN field position
#define MDM_TXSTBCEN_POS       9
/// TXLDPCEN field bit
#define MDM_TXLDPCEN_BIT       ((uint32_t)0x00000100)
/// TXLDPCEN field position
#define MDM_TXLDPCEN_POS       8
/// TXNSSMAX field mask
#define MDM_TXNSSMAX_MASK      ((uint32_t)0x00000070)
/// TXNSSMAX field LSB position
#define MDM_TXNSSMAX_LSB       4
/// TXNSSMAX field width
#define MDM_TXNSSMAX_WIDTH     ((uint32_t)0x00000003)
/// TXGFEN field bit
#define MDM_TXGFEN_BIT         ((uint32_t)0x00000008)
/// TXGFEN field position
#define MDM_TXGFEN_POS         3
/// TXMMEN field bit
#define MDM_TXMMEN_BIT         ((uint32_t)0x00000004)
/// TXMMEN field position
#define MDM_TXMMEN_POS         2
/// TXVHTEN field bit
#define MDM_TXVHTEN_BIT        ((uint32_t)0x00000002)
/// TXVHTEN field position
#define MDM_TXVHTEN_POS        1
/// TXDSSSEN field bit
#define MDM_TXDSSSEN_BIT       ((uint32_t)0x00000001)
/// TXDSSSEN field position
#define MDM_TXDSSSEN_POS       0

/// TXCBWMAX field reset value
#define MDM_TXCBWMAX_RST       0x2
/// NTXMAX field reset value
#define MDM_NTXMAX_RST         0x1
/// TXMUMIMOEN field reset value
#define MDM_TXMUMIMOEN_RST     0x0
/// TXESSEN field reset value
#define MDM_TXESSEN_RST        0x0
/// TXUNEQMODEN field reset value
#define MDM_TXUNEQMODEN_RST    0x0
/// TXSTBCEN field reset value
#define MDM_TXSTBCEN_RST       0x1
/// TXLDPCEN field reset value
#define MDM_TXLDPCEN_RST       0x1
/// TXNSSMAX field reset value
#define MDM_TXNSSMAX_RST       0x1
/// TXGFEN field reset value
#define MDM_TXGFEN_RST         0x1
/// TXMMEN field reset value
#define MDM_TXMMEN_RST         0x1
/// TXVHTEN field reset value
#define MDM_TXVHTEN_RST        0x1
/// TXDSSSEN field reset value
#define MDM_TXDSSSEN_RST       0x1

/**
 * @brief Constructs a value for the TXMODE register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txcbwmax - The value to use for the TXCBWMAX field.
 * @param[in] ntxmax - The value to use for the NTXMAX field.
 * @param[in] txmumimoen - The value to use for the TXMUMIMOEN field.
 * @param[in] txessen - The value to use for the TXESSEN field.
 * @param[in] txuneqmoden - The value to use for the TXUNEQMODEN field.
 * @param[in] txstbcen - The value to use for the TXSTBCEN field.
 * @param[in] txldpcen - The value to use for the TXLDPCEN field.
 * @param[in] txnssmax - The value to use for the TXNSSMAX field.
 * @param[in] txgfen - The value to use for the TXGFEN field.
 * @param[in] txmmen - The value to use for the TXMMEN field.
 * @param[in] txvhten - The value to use for the TXVHTEN field.
 * @param[in] txdsssen - The value to use for the TXDSSSEN field.
 */
__INLINE void mdm_txmode_pack(uint8_t txcbwmax, uint8_t ntxmax, uint8_t txmumimoen, uint8_t txessen, uint8_t txuneqmoden, uint8_t txstbcen, uint8_t txldpcen, uint8_t txnssmax, uint8_t txgfen, uint8_t txmmen, uint8_t txvhten, uint8_t txdsssen)
{
    ASSERT_ERR((((uint32_t)txcbwmax << 24) & ~((uint32_t)0x03000000)) == 0);
    ASSERT_ERR((((uint32_t)ntxmax << 20) & ~((uint32_t)0x00700000)) == 0);
    ASSERT_ERR((((uint32_t)txmumimoen << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)txessen << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)txuneqmoden << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)txstbcen << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)txldpcen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)txnssmax << 4) & ~((uint32_t)0x00000070)) == 0);
    ASSERT_ERR((((uint32_t)txgfen << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)txmmen << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)txvhten << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)txdsssen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR,  ((uint32_t)txcbwmax << 24) | ((uint32_t)ntxmax << 20) | ((uint32_t)txmumimoen << 16) | ((uint32_t)txessen << 11) | ((uint32_t)txuneqmoden << 10) | ((uint32_t)txstbcen << 9) | ((uint32_t)txldpcen << 8) | ((uint32_t)txnssmax << 4) | ((uint32_t)txgfen << 3) | ((uint32_t)txmmen << 2) | ((uint32_t)txvhten << 1) | ((uint32_t)txdsssen << 0));
}

/**
 * @brief Unpacks TXMODE's fields from current value of the TXMODE register.
 *
 * Reads the TXMODE register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txcbwmax - Will be populated with the current value of this field from the register.
 * @param[out] ntxmax - Will be populated with the current value of this field from the register.
 * @param[out] txmumimoen - Will be populated with the current value of this field from the register.
 * @param[out] txessen - Will be populated with the current value of this field from the register.
 * @param[out] txuneqmoden - Will be populated with the current value of this field from the register.
 * @param[out] txstbcen - Will be populated with the current value of this field from the register.
 * @param[out] txldpcen - Will be populated with the current value of this field from the register.
 * @param[out] txnssmax - Will be populated with the current value of this field from the register.
 * @param[out] txgfen - Will be populated with the current value of this field from the register.
 * @param[out] txmmen - Will be populated with the current value of this field from the register.
 * @param[out] txvhten - Will be populated with the current value of this field from the register.
 * @param[out] txdsssen - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_txmode_unpack(uint8_t* txcbwmax, uint8_t* ntxmax, uint8_t* txmumimoen, uint8_t* txessen, uint8_t* txuneqmoden, uint8_t* txstbcen, uint8_t* txldpcen, uint8_t* txnssmax, uint8_t* txgfen, uint8_t* txmmen, uint8_t* txvhten, uint8_t* txdsssen)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);

    *txcbwmax = (localVal & ((uint32_t)0x03000000)) >> 24;
    *ntxmax = (localVal & ((uint32_t)0x00700000)) >> 20;
    *txmumimoen = (localVal & ((uint32_t)0x00010000)) >> 16;
    *txessen = (localVal & ((uint32_t)0x00000800)) >> 11;
    *txuneqmoden = (localVal & ((uint32_t)0x00000400)) >> 10;
    *txstbcen = (localVal & ((uint32_t)0x00000200)) >> 9;
    *txldpcen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *txnssmax = (localVal & ((uint32_t)0x00000070)) >> 4;
    *txgfen = (localVal & ((uint32_t)0x00000008)) >> 3;
    *txmmen = (localVal & ((uint32_t)0x00000004)) >> 2;
    *txvhten = (localVal & ((uint32_t)0x00000002)) >> 1;
    *txdsssen = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TXCBWMAX field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXCBWMAX field's value will be returned.
 *
 * @return The current value of the TXCBWMAX field in the TXMODE register.
 */
__INLINE uint8_t mdm_txcbwmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x03000000)) >> 24);
}

/**
 * @brief Sets the TXCBWMAX field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txcbwmax - The value to set the field to.
 */
__INLINE void mdm_txcbwmax_setf(uint8_t txcbwmax)
{
    ASSERT_ERR((((uint32_t)txcbwmax << 24) & ~((uint32_t)0x03000000)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x03000000)) | ((uint32_t)txcbwmax << 24));
}

/**
 * @brief Returns the current value of the NTXMAX field in the TXMODE register.
 *
 * The TXMODE register will be read and the NTXMAX field's value will be returned.
 *
 * @return The current value of the NTXMAX field in the TXMODE register.
 */
__INLINE uint8_t mdm_ntxmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Sets the NTXMAX field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ntxmax - The value to set the field to.
 */
__INLINE void mdm_ntxmax_setf(uint8_t ntxmax)
{
    ASSERT_ERR((((uint32_t)ntxmax << 20) & ~((uint32_t)0x00700000)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00700000)) | ((uint32_t)ntxmax << 20));
}

/**
 * @brief Returns the current value of the TXMUMIMOEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXMUMIMOEN field's value will be returned.
 *
 * @return The current value of the TXMUMIMOEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txmumimoen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the TXMUMIMOEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txmumimoen - The value to set the field to.
 */
__INLINE void mdm_txmumimoen_setf(uint8_t txmumimoen)
{
    ASSERT_ERR((((uint32_t)txmumimoen << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)txmumimoen << 16));
}

/**
 * @brief Returns the current value of the TXESSEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXESSEN field's value will be returned.
 *
 * @return The current value of the TXESSEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txessen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

/**
 * @brief Sets the TXESSEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txessen - The value to set the field to.
 */
__INLINE void mdm_txessen_setf(uint8_t txessen)
{
    ASSERT_ERR((((uint32_t)txessen << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)txessen << 11));
}

/**
 * @brief Returns the current value of the TXUNEQMODEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXUNEQMODEN field's value will be returned.
 *
 * @return The current value of the TXUNEQMODEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txuneqmoden_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

/**
 * @brief Sets the TXUNEQMODEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txuneqmoden - The value to set the field to.
 */
__INLINE void mdm_txuneqmoden_setf(uint8_t txuneqmoden)
{
    ASSERT_ERR((((uint32_t)txuneqmoden << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)txuneqmoden << 10));
}

/**
 * @brief Returns the current value of the TXSTBCEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXSTBCEN field's value will be returned.
 *
 * @return The current value of the TXSTBCEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txstbcen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the TXSTBCEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txstbcen - The value to set the field to.
 */
__INLINE void mdm_txstbcen_setf(uint8_t txstbcen)
{
    ASSERT_ERR((((uint32_t)txstbcen << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)txstbcen << 9));
}

/**
 * @brief Returns the current value of the TXLDPCEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXLDPCEN field's value will be returned.
 *
 * @return The current value of the TXLDPCEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txldpcen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the TXLDPCEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txldpcen - The value to set the field to.
 */
__INLINE void mdm_txldpcen_setf(uint8_t txldpcen)
{
    ASSERT_ERR((((uint32_t)txldpcen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)txldpcen << 8));
}

/**
 * @brief Returns the current value of the TXNSSMAX field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXNSSMAX field's value will be returned.
 *
 * @return The current value of the TXNSSMAX field in the TXMODE register.
 */
__INLINE uint8_t mdm_txnssmax_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000070)) >> 4);
}

/**
 * @brief Sets the TXNSSMAX field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txnssmax - The value to set the field to.
 */
__INLINE void mdm_txnssmax_setf(uint8_t txnssmax)
{
    ASSERT_ERR((((uint32_t)txnssmax << 4) & ~((uint32_t)0x00000070)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000070)) | ((uint32_t)txnssmax << 4));
}

/**
 * @brief Returns the current value of the TXGFEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXGFEN field's value will be returned.
 *
 * @return The current value of the TXGFEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txgfen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the TXGFEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txgfen - The value to set the field to.
 */
__INLINE void mdm_txgfen_setf(uint8_t txgfen)
{
    ASSERT_ERR((((uint32_t)txgfen << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)txgfen << 3));
}

/**
 * @brief Returns the current value of the TXMMEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXMMEN field's value will be returned.
 *
 * @return The current value of the TXMMEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txmmen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the TXMMEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txmmen - The value to set the field to.
 */
__INLINE void mdm_txmmen_setf(uint8_t txmmen)
{
    ASSERT_ERR((((uint32_t)txmmen << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)txmmen << 2));
}

/**
 * @brief Returns the current value of the TXVHTEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXVHTEN field's value will be returned.
 *
 * @return The current value of the TXVHTEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txvhten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the TXVHTEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txvhten - The value to set the field to.
 */
__INLINE void mdm_txvhten_setf(uint8_t txvhten)
{
    ASSERT_ERR((((uint32_t)txvhten << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)txvhten << 1));
}

/**
 * @brief Returns the current value of the TXDSSSEN field in the TXMODE register.
 *
 * The TXMODE register will be read and the TXDSSSEN field's value will be returned.
 *
 * @return The current value of the TXDSSSEN field in the TXMODE register.
 */
__INLINE uint8_t mdm_txdsssen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXMODE_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the TXDSSSEN field of the TXMODE register.
 *
 * The TXMODE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txdsssen - The value to set the field to.
 */
__INLINE void mdm_txdsssen_setf(uint8_t txdsssen)
{
    ASSERT_ERR((((uint32_t)txdsssen << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_TXMODE_ADDR, (REG_PL_RD(MDM_TXMODE_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)txdsssen << 0));
}

/// @}

/**
 * @name EQUALCTRL1 register definitions
 * <table>
 * <caption id="EQUALCTRL1_BF">EQUALCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>30:28 <td>   SATSB_LDPCNSTS1 <td>R <td>R/W <td>0x1
 * <tr><td>26:24 <td>    SATSB_BCCNSTS1 <td>R <td>R/W <td>0x1
 * <tr><td>22:20 <td>   SATSB_LDPCNSTS0 <td>R <td>R/W <td>0x1
 * <tr><td>18:16 <td>    SATSB_BCCNSTS0 <td>R <td>R/W <td>0x0
 * <tr><td>14:12 <td>        MHSHIFTVAL <td>R <td>R/W <td>0x1
 * <tr><td>07:00 <td>        MHSHIFTTHR <td>R <td>R/W <td>0x28
 * </table>
 *
 * @{
 */

/// Address of the EQUALCTRL1 register
#define MDM_EQUALCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0828)
/// Offset of the EQUALCTRL1 register from the base address
#define MDM_EQUALCTRL1_OFFSET 0x00000028
/// Index of the EQUALCTRL1 register
#define MDM_EQUALCTRL1_INDEX  0x0000000A
/// Reset value of the EQUALCTRL1 register
#define MDM_EQUALCTRL1_RESET  0x11101028

/**
 * @brief Returns the current value of the EQUALCTRL1 register.
 * The EQUALCTRL1 register will be read and its value returned.
 * @return The current value of the EQUALCTRL1 register.
 */
__INLINE uint32_t mdm_equalctrl1_get(void)
{
    return REG_PL_RD(MDM_EQUALCTRL1_ADDR);
}

/**
 * @brief Sets the EQUALCTRL1 register to a value.
 * The EQUALCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_equalctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, value);
}

// field definitions
/// SATSB_LDPCNSTS1 field mask
#define MDM_SATSB_LDPCNSTS1_MASK   ((uint32_t)0x70000000)
/// SATSB_LDPCNSTS1 field LSB position
#define MDM_SATSB_LDPCNSTS1_LSB    28
/// SATSB_LDPCNSTS1 field width
#define MDM_SATSB_LDPCNSTS1_WIDTH  ((uint32_t)0x00000003)
/// SATSB_BCCNSTS1 field mask
#define MDM_SATSB_BCCNSTS1_MASK    ((uint32_t)0x07000000)
/// SATSB_BCCNSTS1 field LSB position
#define MDM_SATSB_BCCNSTS1_LSB     24
/// SATSB_BCCNSTS1 field width
#define MDM_SATSB_BCCNSTS1_WIDTH   ((uint32_t)0x00000003)
/// SATSB_LDPCNSTS0 field mask
#define MDM_SATSB_LDPCNSTS0_MASK   ((uint32_t)0x00700000)
/// SATSB_LDPCNSTS0 field LSB position
#define MDM_SATSB_LDPCNSTS0_LSB    20
/// SATSB_LDPCNSTS0 field width
#define MDM_SATSB_LDPCNSTS0_WIDTH  ((uint32_t)0x00000003)
/// SATSB_BCCNSTS0 field mask
#define MDM_SATSB_BCCNSTS0_MASK    ((uint32_t)0x00070000)
/// SATSB_BCCNSTS0 field LSB position
#define MDM_SATSB_BCCNSTS0_LSB     16
/// SATSB_BCCNSTS0 field width
#define MDM_SATSB_BCCNSTS0_WIDTH   ((uint32_t)0x00000003)
/// MHSHIFTVAL field mask
#define MDM_MHSHIFTVAL_MASK        ((uint32_t)0x00007000)
/// MHSHIFTVAL field LSB position
#define MDM_MHSHIFTVAL_LSB         12
/// MHSHIFTVAL field width
#define MDM_MHSHIFTVAL_WIDTH       ((uint32_t)0x00000003)
/// MHSHIFTTHR field mask
#define MDM_MHSHIFTTHR_MASK        ((uint32_t)0x000000FF)
/// MHSHIFTTHR field LSB position
#define MDM_MHSHIFTTHR_LSB         0
/// MHSHIFTTHR field width
#define MDM_MHSHIFTTHR_WIDTH       ((uint32_t)0x00000008)

/// SATSB_LDPCNSTS1 field reset value
#define MDM_SATSB_LDPCNSTS1_RST    0x1
/// SATSB_BCCNSTS1 field reset value
#define MDM_SATSB_BCCNSTS1_RST     0x1
/// SATSB_LDPCNSTS0 field reset value
#define MDM_SATSB_LDPCNSTS0_RST    0x1
/// SATSB_BCCNSTS0 field reset value
#define MDM_SATSB_BCCNSTS0_RST     0x0
/// MHSHIFTVAL field reset value
#define MDM_MHSHIFTVAL_RST         0x1
/// MHSHIFTTHR field reset value
#define MDM_MHSHIFTTHR_RST         0x28

/**
 * @brief Constructs a value for the EQUALCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] satsbldpcnsts1 - The value to use for the SATSB_LDPCNSTS1 field.
 * @param[in] satsbbccnsts1 - The value to use for the SATSB_BCCNSTS1 field.
 * @param[in] satsbldpcnsts0 - The value to use for the SATSB_LDPCNSTS0 field.
 * @param[in] satsbbccnsts0 - The value to use for the SATSB_BCCNSTS0 field.
 * @param[in] mhshiftval - The value to use for the MHSHIFTVAL field.
 * @param[in] mhshiftthr - The value to use for the MHSHIFTTHR field.
 */
__INLINE void mdm_equalctrl1_pack(uint8_t satsbldpcnsts1, uint8_t satsbbccnsts1, uint8_t satsbldpcnsts0, uint8_t satsbbccnsts0, uint8_t mhshiftval, uint8_t mhshiftthr)
{
    ASSERT_ERR((((uint32_t)satsbldpcnsts1 << 28) & ~((uint32_t)0x70000000)) == 0);
    ASSERT_ERR((((uint32_t)satsbbccnsts1 << 24) & ~((uint32_t)0x07000000)) == 0);
    ASSERT_ERR((((uint32_t)satsbldpcnsts0 << 20) & ~((uint32_t)0x00700000)) == 0);
    ASSERT_ERR((((uint32_t)satsbbccnsts0 << 16) & ~((uint32_t)0x00070000)) == 0);
    ASSERT_ERR((((uint32_t)mhshiftval << 12) & ~((uint32_t)0x00007000)) == 0);
    ASSERT_ERR((((uint32_t)mhshiftthr << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR,  ((uint32_t)satsbldpcnsts1 << 28) | ((uint32_t)satsbbccnsts1 << 24) | ((uint32_t)satsbldpcnsts0 << 20) | ((uint32_t)satsbbccnsts0 << 16) | ((uint32_t)mhshiftval << 12) | ((uint32_t)mhshiftthr << 0));
}

/**
 * @brief Unpacks EQUALCTRL1's fields from current value of the EQUALCTRL1 register.
 *
 * Reads the EQUALCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] satsbldpcnsts1 - Will be populated with the current value of this field from the register.
 * @param[out] satsbbccnsts1 - Will be populated with the current value of this field from the register.
 * @param[out] satsbldpcnsts0 - Will be populated with the current value of this field from the register.
 * @param[out] satsbbccnsts0 - Will be populated with the current value of this field from the register.
 * @param[out] mhshiftval - Will be populated with the current value of this field from the register.
 * @param[out] mhshiftthr - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_equalctrl1_unpack(uint8_t* satsbldpcnsts1, uint8_t* satsbbccnsts1, uint8_t* satsbldpcnsts0, uint8_t* satsbbccnsts0, uint8_t* mhshiftval, uint8_t* mhshiftthr)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);

    *satsbldpcnsts1 = (localVal & ((uint32_t)0x70000000)) >> 28;
    *satsbbccnsts1 = (localVal & ((uint32_t)0x07000000)) >> 24;
    *satsbldpcnsts0 = (localVal & ((uint32_t)0x00700000)) >> 20;
    *satsbbccnsts0 = (localVal & ((uint32_t)0x00070000)) >> 16;
    *mhshiftval = (localVal & ((uint32_t)0x00007000)) >> 12;
    *mhshiftthr = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the SATSB_LDPCNSTS1 field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the SATSB_LDPCNSTS1 field's value will be returned.
 *
 * @return The current value of the SATSB_LDPCNSTS1 field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_satsb_ldpcnsts1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x70000000)) >> 28);
}

/**
 * @brief Sets the SATSB_LDPCNSTS1 field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbldpcnsts1 - The value to set the field to.
 */
__INLINE void mdm_satsb_ldpcnsts1_setf(uint8_t satsbldpcnsts1)
{
    ASSERT_ERR((((uint32_t)satsbldpcnsts1 << 28) & ~((uint32_t)0x70000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x70000000)) | ((uint32_t)satsbldpcnsts1 << 28));
}

/**
 * @brief Returns the current value of the SATSB_BCCNSTS1 field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the SATSB_BCCNSTS1 field's value will be returned.
 *
 * @return The current value of the SATSB_BCCNSTS1 field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_satsb_bccnsts1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x07000000)) >> 24);
}

/**
 * @brief Sets the SATSB_BCCNSTS1 field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbbccnsts1 - The value to set the field to.
 */
__INLINE void mdm_satsb_bccnsts1_setf(uint8_t satsbbccnsts1)
{
    ASSERT_ERR((((uint32_t)satsbbccnsts1 << 24) & ~((uint32_t)0x07000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x07000000)) | ((uint32_t)satsbbccnsts1 << 24));
}

/**
 * @brief Returns the current value of the SATSB_LDPCNSTS0 field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the SATSB_LDPCNSTS0 field's value will be returned.
 *
 * @return The current value of the SATSB_LDPCNSTS0 field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_satsb_ldpcnsts0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Sets the SATSB_LDPCNSTS0 field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbldpcnsts0 - The value to set the field to.
 */
__INLINE void mdm_satsb_ldpcnsts0_setf(uint8_t satsbldpcnsts0)
{
    ASSERT_ERR((((uint32_t)satsbldpcnsts0 << 20) & ~((uint32_t)0x00700000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x00700000)) | ((uint32_t)satsbldpcnsts0 << 20));
}

/**
 * @brief Returns the current value of the SATSB_BCCNSTS0 field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the SATSB_BCCNSTS0 field's value will be returned.
 *
 * @return The current value of the SATSB_BCCNSTS0 field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_satsb_bccnsts0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00070000)) >> 16);
}

/**
 * @brief Sets the SATSB_BCCNSTS0 field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbbccnsts0 - The value to set the field to.
 */
__INLINE void mdm_satsb_bccnsts0_setf(uint8_t satsbbccnsts0)
{
    ASSERT_ERR((((uint32_t)satsbbccnsts0 << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x00070000)) | ((uint32_t)satsbbccnsts0 << 16));
}

/**
 * @brief Returns the current value of the MHSHIFTVAL field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the MHSHIFTVAL field's value will be returned.
 *
 * @return The current value of the MHSHIFTVAL field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_mhshiftval_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00007000)) >> 12);
}

/**
 * @brief Sets the MHSHIFTVAL field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mhshiftval - The value to set the field to.
 */
__INLINE void mdm_mhshiftval_setf(uint8_t mhshiftval)
{
    ASSERT_ERR((((uint32_t)mhshiftval << 12) & ~((uint32_t)0x00007000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x00007000)) | ((uint32_t)mhshiftval << 12));
}

/**
 * @brief Returns the current value of the MHSHIFTTHR field in the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read and the MHSHIFTTHR field's value will be returned.
 *
 * @return The current value of the MHSHIFTTHR field in the EQUALCTRL1 register.
 */
__INLINE uint8_t mdm_mhshiftthr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the MHSHIFTTHR field of the EQUALCTRL1 register.
 *
 * The EQUALCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mhshiftthr - The value to set the field to.
 */
__INLINE void mdm_mhshiftthr_setf(uint8_t mhshiftthr)
{
    ASSERT_ERR((((uint32_t)mhshiftthr << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_EQUALCTRL1_ADDR, (REG_PL_RD(MDM_EQUALCTRL1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)mhshiftthr << 0));
}

/// @}

#if RW_MUMIMO_RX_EN
/**
 * @name EQUALCTRL2 register definitions
 * <table>
 * <caption id="EQUALCTRL2_BF">EQUALCTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>30:28 <td>SATSB_MU_LDPCNSTS1 <td>R <td>R/W <td>0x2
 * <tr><td>26:24 <td> SATSB_MU_BCCNSTS1 <td>R <td>R/W <td>0x1
 * <tr><td>22:20 <td>SATSB_MU_LDPCNSTS0 <td>R <td>R/W <td>0x0
 * <tr><td>18:16 <td> SATSB_MU_BCCNSTS0 <td>R <td>R/W <td>0x1
 * </table>
 *
 * @{
 */

/// Address of the EQUALCTRL2 register
#define MDM_EQUALCTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x082C)
/// Offset of the EQUALCTRL2 register from the base address
#define MDM_EQUALCTRL2_OFFSET 0x0000002C
/// Index of the EQUALCTRL2 register
#define MDM_EQUALCTRL2_INDEX  0x0000000B
/// Reset value of the EQUALCTRL2 register
#define MDM_EQUALCTRL2_RESET  0x21010000

/**
 * @brief Returns the current value of the EQUALCTRL2 register.
 * The EQUALCTRL2 register will be read and its value returned.
 * @return The current value of the EQUALCTRL2 register.
 */
__INLINE uint32_t mdm_equalctrl2_get(void)
{
    return REG_PL_RD(MDM_EQUALCTRL2_ADDR);
}

/**
 * @brief Sets the EQUALCTRL2 register to a value.
 * The EQUALCTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_equalctrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_EQUALCTRL2_ADDR, value);
}

// field definitions
/// SATSB_MU_LDPCNSTS1 field mask
#define MDM_SATSB_MU_LDPCNSTS1_MASK   ((uint32_t)0x70000000)
/// SATSB_MU_LDPCNSTS1 field LSB position
#define MDM_SATSB_MU_LDPCNSTS1_LSB    28
/// SATSB_MU_LDPCNSTS1 field width
#define MDM_SATSB_MU_LDPCNSTS1_WIDTH  ((uint32_t)0x00000003)
/// SATSB_MU_BCCNSTS1 field mask
#define MDM_SATSB_MU_BCCNSTS1_MASK    ((uint32_t)0x07000000)
/// SATSB_MU_BCCNSTS1 field LSB position
#define MDM_SATSB_MU_BCCNSTS1_LSB     24
/// SATSB_MU_BCCNSTS1 field width
#define MDM_SATSB_MU_BCCNSTS1_WIDTH   ((uint32_t)0x00000003)
/// SATSB_MU_LDPCNSTS0 field mask
#define MDM_SATSB_MU_LDPCNSTS0_MASK   ((uint32_t)0x00700000)
/// SATSB_MU_LDPCNSTS0 field LSB position
#define MDM_SATSB_MU_LDPCNSTS0_LSB    20
/// SATSB_MU_LDPCNSTS0 field width
#define MDM_SATSB_MU_LDPCNSTS0_WIDTH  ((uint32_t)0x00000003)
/// SATSB_MU_BCCNSTS0 field mask
#define MDM_SATSB_MU_BCCNSTS0_MASK    ((uint32_t)0x00070000)
/// SATSB_MU_BCCNSTS0 field LSB position
#define MDM_SATSB_MU_BCCNSTS0_LSB     16
/// SATSB_MU_BCCNSTS0 field width
#define MDM_SATSB_MU_BCCNSTS0_WIDTH   ((uint32_t)0x00000003)

/// SATSB_MU_LDPCNSTS1 field reset value
#define MDM_SATSB_MU_LDPCNSTS1_RST    0x2
/// SATSB_MU_BCCNSTS1 field reset value
#define MDM_SATSB_MU_BCCNSTS1_RST     0x1
/// SATSB_MU_LDPCNSTS0 field reset value
#define MDM_SATSB_MU_LDPCNSTS0_RST    0x0
/// SATSB_MU_BCCNSTS0 field reset value
#define MDM_SATSB_MU_BCCNSTS0_RST     0x1

/**
 * @brief Constructs a value for the EQUALCTRL2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] satsbmuldpcnsts1 - The value to use for the SATSB_MU_LDPCNSTS1 field.
 * @param[in] satsbmubccnsts1 - The value to use for the SATSB_MU_BCCNSTS1 field.
 * @param[in] satsbmuldpcnsts0 - The value to use for the SATSB_MU_LDPCNSTS0 field.
 * @param[in] satsbmubccnsts0 - The value to use for the SATSB_MU_BCCNSTS0 field.
 */
__INLINE void mdm_equalctrl2_pack(uint8_t satsbmuldpcnsts1, uint8_t satsbmubccnsts1, uint8_t satsbmuldpcnsts0, uint8_t satsbmubccnsts0)
{
    ASSERT_ERR((((uint32_t)satsbmuldpcnsts1 << 28) & ~((uint32_t)0x70000000)) == 0);
    ASSERT_ERR((((uint32_t)satsbmubccnsts1 << 24) & ~((uint32_t)0x07000000)) == 0);
    ASSERT_ERR((((uint32_t)satsbmuldpcnsts0 << 20) & ~((uint32_t)0x00700000)) == 0);
    ASSERT_ERR((((uint32_t)satsbmubccnsts0 << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL2_ADDR,  ((uint32_t)satsbmuldpcnsts1 << 28) | ((uint32_t)satsbmubccnsts1 << 24) | ((uint32_t)satsbmuldpcnsts0 << 20) | ((uint32_t)satsbmubccnsts0 << 16));
}

/**
 * @brief Unpacks EQUALCTRL2's fields from current value of the EQUALCTRL2 register.
 *
 * Reads the EQUALCTRL2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] satsbmuldpcnsts1 - Will be populated with the current value of this field from the register.
 * @param[out] satsbmubccnsts1 - Will be populated with the current value of this field from the register.
 * @param[out] satsbmuldpcnsts0 - Will be populated with the current value of this field from the register.
 * @param[out] satsbmubccnsts0 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_equalctrl2_unpack(uint8_t* satsbmuldpcnsts1, uint8_t* satsbmubccnsts1, uint8_t* satsbmuldpcnsts0, uint8_t* satsbmubccnsts0)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL2_ADDR);

    *satsbmuldpcnsts1 = (localVal & ((uint32_t)0x70000000)) >> 28;
    *satsbmubccnsts1 = (localVal & ((uint32_t)0x07000000)) >> 24;
    *satsbmuldpcnsts0 = (localVal & ((uint32_t)0x00700000)) >> 20;
    *satsbmubccnsts0 = (localVal & ((uint32_t)0x00070000)) >> 16;
}

/**
 * @brief Returns the current value of the SATSB_MU_LDPCNSTS1 field in the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read and the SATSB_MU_LDPCNSTS1 field's value will be returned.
 *
 * @return The current value of the SATSB_MU_LDPCNSTS1 field in the EQUALCTRL2 register.
 */
__INLINE uint8_t mdm_satsb_mu_ldpcnsts1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x70000000)) >> 28);
}

/**
 * @brief Sets the SATSB_MU_LDPCNSTS1 field of the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbmuldpcnsts1 - The value to set the field to.
 */
__INLINE void mdm_satsb_mu_ldpcnsts1_setf(uint8_t satsbmuldpcnsts1)
{
    ASSERT_ERR((((uint32_t)satsbmuldpcnsts1 << 28) & ~((uint32_t)0x70000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL2_ADDR, (REG_PL_RD(MDM_EQUALCTRL2_ADDR) & ~((uint32_t)0x70000000)) | ((uint32_t)satsbmuldpcnsts1 << 28));
}

/**
 * @brief Returns the current value of the SATSB_MU_BCCNSTS1 field in the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read and the SATSB_MU_BCCNSTS1 field's value will be returned.
 *
 * @return The current value of the SATSB_MU_BCCNSTS1 field in the EQUALCTRL2 register.
 */
__INLINE uint8_t mdm_satsb_mu_bccnsts1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x07000000)) >> 24);
}

/**
 * @brief Sets the SATSB_MU_BCCNSTS1 field of the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbmubccnsts1 - The value to set the field to.
 */
__INLINE void mdm_satsb_mu_bccnsts1_setf(uint8_t satsbmubccnsts1)
{
    ASSERT_ERR((((uint32_t)satsbmubccnsts1 << 24) & ~((uint32_t)0x07000000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL2_ADDR, (REG_PL_RD(MDM_EQUALCTRL2_ADDR) & ~((uint32_t)0x07000000)) | ((uint32_t)satsbmubccnsts1 << 24));
}

/**
 * @brief Returns the current value of the SATSB_MU_LDPCNSTS0 field in the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read and the SATSB_MU_LDPCNSTS0 field's value will be returned.
 *
 * @return The current value of the SATSB_MU_LDPCNSTS0 field in the EQUALCTRL2 register.
 */
__INLINE uint8_t mdm_satsb_mu_ldpcnsts0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Sets the SATSB_MU_LDPCNSTS0 field of the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbmuldpcnsts0 - The value to set the field to.
 */
__INLINE void mdm_satsb_mu_ldpcnsts0_setf(uint8_t satsbmuldpcnsts0)
{
    ASSERT_ERR((((uint32_t)satsbmuldpcnsts0 << 20) & ~((uint32_t)0x00700000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL2_ADDR, (REG_PL_RD(MDM_EQUALCTRL2_ADDR) & ~((uint32_t)0x00700000)) | ((uint32_t)satsbmuldpcnsts0 << 20));
}

/**
 * @brief Returns the current value of the SATSB_MU_BCCNSTS0 field in the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read and the SATSB_MU_BCCNSTS0 field's value will be returned.
 *
 * @return The current value of the SATSB_MU_BCCNSTS0 field in the EQUALCTRL2 register.
 */
__INLINE uint8_t mdm_satsb_mu_bccnsts0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EQUALCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00070000)) >> 16);
}

/**
 * @brief Sets the SATSB_MU_BCCNSTS0 field of the EQUALCTRL2 register.
 *
 * The EQUALCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satsbmubccnsts0 - The value to set the field to.
 */
__INLINE void mdm_satsb_mu_bccnsts0_setf(uint8_t satsbmubccnsts0)
{
    ASSERT_ERR((((uint32_t)satsbmubccnsts0 << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(MDM_EQUALCTRL2_ADDR, (REG_PL_RD(MDM_EQUALCTRL2_ADDR) & ~((uint32_t)0x00070000)) | ((uint32_t)satsbmubccnsts0 << 16));
}

#endif // RW_MUMIMO_RX_EN
/// @}

/**
 * @name SMOOTHSNRTHR register definitions
 * <table>
 * <caption id="SMOOTHSNRTHR_BF">SMOOTHSNRTHR bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>15:08 <td>  SMOOTHSNRTHRHIGH <td>R <td>R/W <td>0x1B
 * <tr><td>07:00 <td>   SMOOTHSNRTHRMID <td>R <td>R/W <td>0xF
 * </table>
 *
 * @{
 */

/// Address of the SMOOTHSNRTHR register
#define MDM_SMOOTHSNRTHR_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0830)
/// Offset of the SMOOTHSNRTHR register from the base address
#define MDM_SMOOTHSNRTHR_OFFSET 0x00000030
/// Index of the SMOOTHSNRTHR register
#define MDM_SMOOTHSNRTHR_INDEX  0x0000000C
/// Reset value of the SMOOTHSNRTHR register
#define MDM_SMOOTHSNRTHR_RESET  0x00001B0F

/**
 * @brief Returns the current value of the SMOOTHSNRTHR register.
 * The SMOOTHSNRTHR register will be read and its value returned.
 * @return The current value of the SMOOTHSNRTHR register.
 */
__INLINE uint32_t mdm_smoothsnrthr_get(void)
{
    return REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR);
}

/**
 * @brief Sets the SMOOTHSNRTHR register to a value.
 * The SMOOTHSNRTHR register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_smoothsnrthr_set(uint32_t value)
{
    REG_PL_WR(MDM_SMOOTHSNRTHR_ADDR, value);
}

// field definitions
/// SMOOTHSNRTHRHIGH field mask
#define MDM_SMOOTHSNRTHRHIGH_MASK   ((uint32_t)0x0000FF00)
/// SMOOTHSNRTHRHIGH field LSB position
#define MDM_SMOOTHSNRTHRHIGH_LSB    8
/// SMOOTHSNRTHRHIGH field width
#define MDM_SMOOTHSNRTHRHIGH_WIDTH  ((uint32_t)0x00000008)
/// SMOOTHSNRTHRMID field mask
#define MDM_SMOOTHSNRTHRMID_MASK    ((uint32_t)0x000000FF)
/// SMOOTHSNRTHRMID field LSB position
#define MDM_SMOOTHSNRTHRMID_LSB     0
/// SMOOTHSNRTHRMID field width
#define MDM_SMOOTHSNRTHRMID_WIDTH   ((uint32_t)0x00000008)

/// SMOOTHSNRTHRHIGH field reset value
#define MDM_SMOOTHSNRTHRHIGH_RST    0x1B
/// SMOOTHSNRTHRMID field reset value
#define MDM_SMOOTHSNRTHRMID_RST     0xF

/**
 * @brief Constructs a value for the SMOOTHSNRTHR register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] smoothsnrthrhigh - The value to use for the SMOOTHSNRTHRHIGH field.
 * @param[in] smoothsnrthrmid - The value to use for the SMOOTHSNRTHRMID field.
 */
__INLINE void mdm_smoothsnrthr_pack(uint8_t smoothsnrthrhigh, uint8_t smoothsnrthrmid)
{
    ASSERT_ERR((((uint32_t)smoothsnrthrhigh << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)smoothsnrthrmid << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_SMOOTHSNRTHR_ADDR,  ((uint32_t)smoothsnrthrhigh << 8) | ((uint32_t)smoothsnrthrmid << 0));
}

/**
 * @brief Unpacks SMOOTHSNRTHR's fields from current value of the SMOOTHSNRTHR register.
 *
 * Reads the SMOOTHSNRTHR register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] smoothsnrthrhigh - Will be populated with the current value of this field from the register.
 * @param[out] smoothsnrthrmid - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_smoothsnrthr_unpack(uint8_t* smoothsnrthrhigh, uint8_t* smoothsnrthrmid)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR);

    *smoothsnrthrhigh = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *smoothsnrthrmid = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the SMOOTHSNRTHRHIGH field in the SMOOTHSNRTHR register.
 *
 * The SMOOTHSNRTHR register will be read and the SMOOTHSNRTHRHIGH field's value will be returned.
 *
 * @return The current value of the SMOOTHSNRTHRHIGH field in the SMOOTHSNRTHR register.
 */
__INLINE uint8_t mdm_smoothsnrthrhigh_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the SMOOTHSNRTHRHIGH field of the SMOOTHSNRTHR register.
 *
 * The SMOOTHSNRTHR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] smoothsnrthrhigh - The value to set the field to.
 */
__INLINE void mdm_smoothsnrthrhigh_setf(uint8_t smoothsnrthrhigh)
{
    ASSERT_ERR((((uint32_t)smoothsnrthrhigh << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_SMOOTHSNRTHR_ADDR, (REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)smoothsnrthrhigh << 8));
}

/**
 * @brief Returns the current value of the SMOOTHSNRTHRMID field in the SMOOTHSNRTHR register.
 *
 * The SMOOTHSNRTHR register will be read and the SMOOTHSNRTHRMID field's value will be returned.
 *
 * @return The current value of the SMOOTHSNRTHRMID field in the SMOOTHSNRTHR register.
 */
__INLINE uint8_t mdm_smoothsnrthrmid_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the SMOOTHSNRTHRMID field of the SMOOTHSNRTHR register.
 *
 * The SMOOTHSNRTHR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] smoothsnrthrmid - The value to set the field to.
 */
__INLINE void mdm_smoothsnrthrmid_setf(uint8_t smoothsnrthrmid)
{
    ASSERT_ERR((((uint32_t)smoothsnrthrmid << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_SMOOTHSNRTHR_ADDR, (REG_PL_RD(MDM_SMOOTHSNRTHR_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)smoothsnrthrmid << 0));
}

/// @}

/**
 * @name RXTDCTRL1 register definitions
 * <table>
 * <caption id="RXTDCTRL1_BF">RXTDCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:24 <td>   TDDCHTSTFMARGIN <td>R <td>R/W <td>0x6
 * <tr><td>20:16 <td>     TDBWDETWEIGHT <td>R <td>R/W <td>0x16
 * <tr><td>08 <td>          RXTDDCEN <td>R <td>R/W <td>1
 * <tr><td>05 <td>       TDBWFORCE40 <td>R <td>R/W <td>0
 * <tr><td>04 <td>       TDBWFORCE20 <td>R <td>R/W <td>0
 * <tr><td>00 <td>        AGCRSTMODE <td>R <td>R/W <td>0
 * </table>
 *
 * @{
 */

/// Address of the RXTDCTRL1 register
#define MDM_RXTDCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0834)
/// Offset of the RXTDCTRL1 register from the base address
#define MDM_RXTDCTRL1_OFFSET 0x00000034
/// Index of the RXTDCTRL1 register
#define MDM_RXTDCTRL1_INDEX  0x0000000D
/// Reset value of the RXTDCTRL1 register
#define MDM_RXTDCTRL1_RESET  0x06160100

/**
 * @brief Returns the current value of the RXTDCTRL1 register.
 * The RXTDCTRL1 register will be read and its value returned.
 * @return The current value of the RXTDCTRL1 register.
 */
__INLINE uint32_t mdm_rxtdctrl1_get(void)
{
    return REG_PL_RD(MDM_RXTDCTRL1_ADDR);
}

/**
 * @brief Sets the RXTDCTRL1 register to a value.
 * The RXTDCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxtdctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, value);
}

// field definitions
/// TDDCHTSTFMARGIN field mask
#define MDM_TDDCHTSTFMARGIN_MASK   ((uint32_t)0xFF000000)
/// TDDCHTSTFMARGIN field LSB position
#define MDM_TDDCHTSTFMARGIN_LSB    24
/// TDDCHTSTFMARGIN field width
#define MDM_TDDCHTSTFMARGIN_WIDTH  ((uint32_t)0x00000008)
/// TDBWDETWEIGHT field mask
#define MDM_TDBWDETWEIGHT_MASK     ((uint32_t)0x001F0000)
/// TDBWDETWEIGHT field LSB position
#define MDM_TDBWDETWEIGHT_LSB      16
/// TDBWDETWEIGHT field width
#define MDM_TDBWDETWEIGHT_WIDTH    ((uint32_t)0x00000005)
/// RXTDDCEN field bit
#define MDM_RXTDDCEN_BIT           ((uint32_t)0x00000100)
/// RXTDDCEN field position
#define MDM_RXTDDCEN_POS           8
/// TDBWFORCE40 field bit
#define MDM_TDBWFORCE40_BIT        ((uint32_t)0x00000020)
/// TDBWFORCE40 field position
#define MDM_TDBWFORCE40_POS        5
/// TDBWFORCE20 field bit
#define MDM_TDBWFORCE20_BIT        ((uint32_t)0x00000010)
/// TDBWFORCE20 field position
#define MDM_TDBWFORCE20_POS        4
/// AGCRSTMODE field bit
#define MDM_AGCRSTMODE_BIT         ((uint32_t)0x00000001)
/// AGCRSTMODE field position
#define MDM_AGCRSTMODE_POS         0

/// TDDCHTSTFMARGIN field reset value
#define MDM_TDDCHTSTFMARGIN_RST    0x6
/// TDBWDETWEIGHT field reset value
#define MDM_TDBWDETWEIGHT_RST      0x16
/// RXTDDCEN field reset value
#define MDM_RXTDDCEN_RST           0x1
/// TDBWFORCE40 field reset value
#define MDM_TDBWFORCE40_RST        0x0
/// TDBWFORCE20 field reset value
#define MDM_TDBWFORCE20_RST        0x0
/// AGCRSTMODE field reset value
#define MDM_AGCRSTMODE_RST         0x0

/**
 * @brief Constructs a value for the RXTDCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tddchtstfmargin - The value to use for the TDDCHTSTFMARGIN field.
 * @param[in] tdbwdetweight - The value to use for the TDBWDETWEIGHT field.
 * @param[in] rxtddcen - The value to use for the RXTDDCEN field.
 * @param[in] tdbwforce40 - The value to use for the TDBWFORCE40 field.
 * @param[in] tdbwforce20 - The value to use for the TDBWFORCE20 field.
 * @param[in] agcrstmode - The value to use for the AGCRSTMODE field.
 */
__INLINE void mdm_rxtdctrl1_pack(uint8_t tddchtstfmargin, uint8_t tdbwdetweight, uint8_t rxtddcen, uint8_t tdbwforce40, uint8_t tdbwforce20, uint8_t agcrstmode)
{
    ASSERT_ERR((((uint32_t)tddchtstfmargin << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)tdbwdetweight << 16) & ~((uint32_t)0x001F0000)) == 0);
    ASSERT_ERR((((uint32_t)rxtddcen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)tdbwforce40 << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)tdbwforce20 << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)agcrstmode << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR,  ((uint32_t)tddchtstfmargin << 24) | ((uint32_t)tdbwdetweight << 16) | ((uint32_t)rxtddcen << 8) | ((uint32_t)tdbwforce40 << 5) | ((uint32_t)tdbwforce20 << 4) | ((uint32_t)agcrstmode << 0));
}

/**
 * @brief Unpacks RXTDCTRL1's fields from current value of the RXTDCTRL1 register.
 *
 * Reads the RXTDCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tddchtstfmargin - Will be populated with the current value of this field from the register.
 * @param[out] tdbwdetweight - Will be populated with the current value of this field from the register.
 * @param[out] rxtddcen - Will be populated with the current value of this field from the register.
 * @param[out] tdbwforce40 - Will be populated with the current value of this field from the register.
 * @param[out] tdbwforce20 - Will be populated with the current value of this field from the register.
 * @param[out] agcrstmode - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_rxtdctrl1_unpack(uint8_t* tddchtstfmargin, uint8_t* tdbwdetweight, uint8_t* rxtddcen, uint8_t* tdbwforce40, uint8_t* tdbwforce20, uint8_t* agcrstmode)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);

    *tddchtstfmargin = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *tdbwdetweight = (localVal & ((uint32_t)0x001F0000)) >> 16;
    *rxtddcen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *tdbwforce40 = (localVal & ((uint32_t)0x00000020)) >> 5;
    *tdbwforce20 = (localVal & ((uint32_t)0x00000010)) >> 4;
    *agcrstmode = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TDDCHTSTFMARGIN field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the TDDCHTSTFMARGIN field's value will be returned.
 *
 * @return The current value of the TDDCHTSTFMARGIN field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_tddchtstfmargin_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the TDDCHTSTFMARGIN field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tddchtstfmargin - The value to set the field to.
 */
__INLINE void mdm_tddchtstfmargin_setf(uint8_t tddchtstfmargin)
{
    ASSERT_ERR((((uint32_t)tddchtstfmargin << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)tddchtstfmargin << 24));
}

/**
 * @brief Returns the current value of the TDBWDETWEIGHT field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the TDBWDETWEIGHT field's value will be returned.
 *
 * @return The current value of the TDBWDETWEIGHT field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_tdbwdetweight_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x001F0000)) >> 16);
}

/**
 * @brief Sets the TDBWDETWEIGHT field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdbwdetweight - The value to set the field to.
 */
__INLINE void mdm_tdbwdetweight_setf(uint8_t tdbwdetweight)
{
    ASSERT_ERR((((uint32_t)tdbwdetweight << 16) & ~((uint32_t)0x001F0000)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0x001F0000)) | ((uint32_t)tdbwdetweight << 16));
}

/**
 * @brief Returns the current value of the RXTDDCEN field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the RXTDDCEN field's value will be returned.
 *
 * @return The current value of the RXTDDCEN field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_rxtddcen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the RXTDDCEN field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxtddcen - The value to set the field to.
 */
__INLINE void mdm_rxtddcen_setf(uint8_t rxtddcen)
{
    ASSERT_ERR((((uint32_t)rxtddcen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)rxtddcen << 8));
}

/**
 * @brief Returns the current value of the TDBWFORCE40 field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the TDBWFORCE40 field's value will be returned.
 *
 * @return The current value of the TDBWFORCE40 field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_tdbwforce40_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

/**
 * @brief Sets the TDBWFORCE40 field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdbwforce40 - The value to set the field to.
 */
__INLINE void mdm_tdbwforce40_setf(uint8_t tdbwforce40)
{
    ASSERT_ERR((((uint32_t)tdbwforce40 << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)tdbwforce40 << 5));
}

/**
 * @brief Returns the current value of the TDBWFORCE20 field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the TDBWFORCE20 field's value will be returned.
 *
 * @return The current value of the TDBWFORCE20 field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_tdbwforce20_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the TDBWFORCE20 field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdbwforce20 - The value to set the field to.
 */
__INLINE void mdm_tdbwforce20_setf(uint8_t tdbwforce20)
{
    ASSERT_ERR((((uint32_t)tdbwforce20 << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)tdbwforce20 << 4));
}

/**
 * @brief Returns the current value of the AGCRSTMODE field in the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read and the AGCRSTMODE field's value will be returned.
 *
 * @return The current value of the AGCRSTMODE field in the RXTDCTRL1 register.
 */
__INLINE uint8_t mdm_agcrstmode_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXTDCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the AGCRSTMODE field of the RXTDCTRL1 register.
 *
 * The RXTDCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcrstmode - The value to set the field to.
 */
__INLINE void mdm_agcrstmode_setf(uint8_t agcrstmode)
{
    ASSERT_ERR((((uint32_t)agcrstmode << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_RXTDCTRL1_ADDR, (REG_PL_RD(MDM_RXTDCTRL1_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)agcrstmode << 0));
}

/// @}

/**
 * @name TXCTRL0 register definitions
 * <table>
 * <caption id="TXCTRL0_BF">TXCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>09:00 <td>      TXSTARTDELAY <td>R <td>R/W <td>0xB4
 * </table>
 *
 * @{
 */

/// Address of the TXCTRL0 register
#define MDM_TXCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0838)
/// Offset of the TXCTRL0 register from the base address
#define MDM_TXCTRL0_OFFSET 0x00000038
/// Index of the TXCTRL0 register
#define MDM_TXCTRL0_INDEX  0x0000000E
/// Reset value of the TXCTRL0 register
#define MDM_TXCTRL0_RESET  0x000000B4

/**
 * @brief Returns the current value of the TXCTRL0 register.
 * The TXCTRL0 register will be read and its value returned.
 * @return The current value of the TXCTRL0 register.
 */
__INLINE uint32_t mdm_txctrl0_get(void)
{
    return REG_PL_RD(MDM_TXCTRL0_ADDR);
}

/**
 * @brief Sets the TXCTRL0 register to a value.
 * The TXCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_txctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_TXCTRL0_ADDR, value);
}

// field definitions
/// TXSTARTDELAY field mask
#define MDM_TXSTARTDELAY_MASK   ((uint32_t)0x000003FF)
/// TXSTARTDELAY field LSB position
#define MDM_TXSTARTDELAY_LSB    0
/// TXSTARTDELAY field width
#define MDM_TXSTARTDELAY_WIDTH  ((uint32_t)0x0000000A)

/// TXSTARTDELAY field reset value
#define MDM_TXSTARTDELAY_RST    0xB4

/**
 * @brief Returns the current value of the TXSTARTDELAY field in the TXCTRL0 register.
 *
 * The TXCTRL0 register will be read and the TXSTARTDELAY field's value will be returned.
 *
 * @return The current value of the TXSTARTDELAY field in the TXCTRL0 register.
 */
__INLINE uint16_t mdm_txstartdelay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL0_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000003FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXSTARTDELAY field of the TXCTRL0 register.
 *
 * The TXCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txstartdelay - The value to set the field to.
 */
__INLINE void mdm_txstartdelay_setf(uint16_t txstartdelay)
{
    ASSERT_ERR((((uint32_t)txstartdelay << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(MDM_TXCTRL0_ADDR, (uint32_t)txstartdelay << 0);
}

/// @}
#endif

/**
 * @name RXCTRL1 register definitions
 * <table>
 * <caption id="RXCTRL1_BF">RXCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:16 <td>  RXBCCNDBPSMAXSGI <td>R <td>R/W <td>0xC30
 * <tr><td>15:00 <td>  RXBCCNDBPSMAXLGI <td>R <td>R/W <td>0xC30
 * </table>
 *
 * @{
 */

/// Address of the RXCTRL1 register
#define MDM_RXCTRL1_ADDR(band)   (REG_MDM_CFG_BASE_ADDR(band)+0x083C)
/// Offset of the RXCTRL1 register from the base address
#define MDM_RXCTRL1_OFFSET 0x0000003C
/// Index of the RXCTRL1 register
#define MDM_RXCTRL1_INDEX  0x0000000F
/// Reset value of the RXCTRL1 register
#define MDM_RXCTRL1_RESET  0x0C300C30

#if 0
/**
 * @brief Returns the current value of the RXCTRL1 register.
 * The RXCTRL1 register will be read and its value returned.
 * @return The current value of the RXCTRL1 register.
 */
__INLINE uint32_t mdm_rxctrl1_get(void)
{
    return REG_PL_RD(MDM_RXCTRL1_ADDR);
}

/**
 * @brief Sets the RXCTRL1 register to a value.
 * The RXCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_RXCTRL1_ADDR, value);
}

// field definitions
/// RXBCCNDBPSMAXSGI field mask
#define MDM_RXBCCNDBPSMAXSGI_MASK   ((uint32_t)0xFFFF0000)
/// RXBCCNDBPSMAXSGI field LSB position
#define MDM_RXBCCNDBPSMAXSGI_LSB    16
/// RXBCCNDBPSMAXSGI field width
#define MDM_RXBCCNDBPSMAXSGI_WIDTH  ((uint32_t)0x00000010)
/// RXBCCNDBPSMAXLGI field mask
#define MDM_RXBCCNDBPSMAXLGI_MASK   ((uint32_t)0x0000FFFF)
/// RXBCCNDBPSMAXLGI field LSB position
#define MDM_RXBCCNDBPSMAXLGI_LSB    0
/// RXBCCNDBPSMAXLGI field width
#define MDM_RXBCCNDBPSMAXLGI_WIDTH  ((uint32_t)0x00000010)

/// RXBCCNDBPSMAXSGI field reset value
#define MDM_RXBCCNDBPSMAXSGI_RST    0xC30
/// RXBCCNDBPSMAXLGI field reset value
#define MDM_RXBCCNDBPSMAXLGI_RST    0xC30

/**
 * @brief Constructs a value for the RXCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxbccndbpsmaxsgi - The value to use for the RXBCCNDBPSMAXSGI field.
 * @param[in] rxbccndbpsmaxlgi - The value to use for the RXBCCNDBPSMAXLGI field.
 */
__INLINE void mdm_rxctrl1_pack(uint16_t rxbccndbpsmaxsgi, uint16_t rxbccndbpsmaxlgi)
{
    ASSERT_ERR((((uint32_t)rxbccndbpsmaxsgi << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)rxbccndbpsmaxlgi << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(MDM_RXCTRL1_ADDR,  ((uint32_t)rxbccndbpsmaxsgi << 16) | ((uint32_t)rxbccndbpsmaxlgi << 0));
}

/**
 * @brief Unpacks RXCTRL1's fields from current value of the RXCTRL1 register.
 *
 * Reads the RXCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxbccndbpsmaxsgi - Will be populated with the current value of this field from the register.
 * @param[out] rxbccndbpsmaxlgi - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_rxctrl1_unpack(uint16_t* rxbccndbpsmaxsgi, uint16_t* rxbccndbpsmaxlgi)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL1_ADDR);

    *rxbccndbpsmaxsgi = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *rxbccndbpsmaxlgi = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the RXBCCNDBPSMAXSGI field in the RXCTRL1 register.
 *
 * The RXCTRL1 register will be read and the RXBCCNDBPSMAXSGI field's value will be returned.
 *
 * @return The current value of the RXBCCNDBPSMAXSGI field in the RXCTRL1 register.
 */
__INLINE uint16_t mdm_rxbccndbpsmaxsgi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the RXBCCNDBPSMAXSGI field of the RXCTRL1 register.
 *
 * The RXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxbccndbpsmaxsgi - The value to set the field to.
 */
__INLINE void mdm_rxbccndbpsmaxsgi_setf(uint16_t rxbccndbpsmaxsgi)
{
    ASSERT_ERR((((uint32_t)rxbccndbpsmaxsgi << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(MDM_RXCTRL1_ADDR, (REG_PL_RD(MDM_RXCTRL1_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)rxbccndbpsmaxsgi << 16));
}

/**
 * @brief Returns the current value of the RXBCCNDBPSMAXLGI field in the RXCTRL1 register.
 *
 * The RXCTRL1 register will be read and the RXBCCNDBPSMAXLGI field's value will be returned.
 *
 * @return The current value of the RXBCCNDBPSMAXLGI field in the RXCTRL1 register.
 */
__INLINE uint16_t mdm_rxbccndbpsmaxlgi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the RXBCCNDBPSMAXLGI field of the RXCTRL1 register.
 *
 * The RXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxbccndbpsmaxlgi - The value to set the field to.
 */
__INLINE void mdm_rxbccndbpsmaxlgi_setf(uint16_t rxbccndbpsmaxlgi)
{
    ASSERT_ERR((((uint32_t)rxbccndbpsmaxlgi << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(MDM_RXCTRL1_ADDR, (REG_PL_RD(MDM_RXCTRL1_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)rxbccndbpsmaxlgi << 0));
}
#endif
/// @}
#if 0
/**
 * @name SMOOTHFORCECTRL register definitions
 * <table>
 * <caption id="SMOOTHFORCECTRL_BF">SMOOTHFORCECTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>29 <td>     MUSMOOTHFORCE <td>R <td>R/W <td>0
 * <tr><td>28 <td>     BFSMOOTHFORCE <td>R <td>R/W <td>0
 * <tr><td>25 <td>CFGVHTSTS2SMOOTHFORCE <td>R <td>R/W <td>1
 * <tr><td>24 <td>CFGVHTSTS1SMOOTHFORCE <td>R <td>R/W <td>1
 * <tr><td>21 <td>CFGHTSTS2SMOOTHFORCE <td>R <td>R/W <td>1
 * <tr><td>20 <td>CFGHTSTS1SMOOTHFORCE <td>R <td>R/W <td>1
 * <tr><td>16 <td>CFGNONHTSMOOTHFORCE <td>R <td>R/W <td>1
 * <tr><td>11:10 <td>  CFGVHTSTS2SMOOTH <td>R <td>R/W <td>0x0
 * <tr><td>09:08 <td>  CFGVHTSTS1SMOOTH <td>R <td>R/W <td>0x0
 * <tr><td>07:06 <td>   CFGHTSTS2SMOOTH <td>R <td>R/W <td>0x0
 * <tr><td>05:04 <td>   CFGHTSTS1SMOOTH <td>R <td>R/W <td>0x0
 * <tr><td>01:00 <td>    CFGNONHTSMOOTH <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the SMOOTHFORCECTRL register
#define MDM_SMOOTHFORCECTRL_ADDR   (REG_MDM_CFG_BASE_ADDR + 0x0840)
/// Offset of the SMOOTHFORCECTRL register from the base address
#define MDM_SMOOTHFORCECTRL_OFFSET 0x00000040
/// Index of the SMOOTHFORCECTRL register
#define MDM_SMOOTHFORCECTRL_INDEX  0x00000010
/// Reset value of the SMOOTHFORCECTRL register
#define MDM_SMOOTHFORCECTRL_RESET  0x03310000

/**
 * @brief Returns the current value of the SMOOTHFORCECTRL register.
 * The SMOOTHFORCECTRL register will be read and its value returned.
 * @return The current value of the SMOOTHFORCECTRL register.
 */
__INLINE uint32_t mdm_smoothforcectrl_get(void)
{
    return REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
}

/**
 * @brief Sets the SMOOTHFORCECTRL register to a value.
 * The SMOOTHFORCECTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_smoothforcectrl_set(uint32_t value)
{
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, value);
}

// field definitions
/// MUSMOOTHFORCE field bit
#define MDM_MUSMOOTHFORCE_BIT            ((uint32_t)0x20000000)
/// MUSMOOTHFORCE field position
#define MDM_MUSMOOTHFORCE_POS            29
/// BFSMOOTHFORCE field bit
#define MDM_BFSMOOTHFORCE_BIT            ((uint32_t)0x10000000)
/// BFSMOOTHFORCE field position
#define MDM_BFSMOOTHFORCE_POS            28
/// CFGVHTSTS2SMOOTHFORCE field bit
#define MDM_CFGVHTSTS2SMOOTHFORCE_BIT    ((uint32_t)0x02000000)
/// CFGVHTSTS2SMOOTHFORCE field position
#define MDM_CFGVHTSTS2SMOOTHFORCE_POS    25
/// CFGVHTSTS1SMOOTHFORCE field bit
#define MDM_CFGVHTSTS1SMOOTHFORCE_BIT    ((uint32_t)0x01000000)
/// CFGVHTSTS1SMOOTHFORCE field position
#define MDM_CFGVHTSTS1SMOOTHFORCE_POS    24
/// CFGHTSTS2SMOOTHFORCE field bit
#define MDM_CFGHTSTS2SMOOTHFORCE_BIT     ((uint32_t)0x00200000)
/// CFGHTSTS2SMOOTHFORCE field position
#define MDM_CFGHTSTS2SMOOTHFORCE_POS     21
/// CFGHTSTS1SMOOTHFORCE field bit
#define MDM_CFGHTSTS1SMOOTHFORCE_BIT     ((uint32_t)0x00100000)
/// CFGHTSTS1SMOOTHFORCE field position
#define MDM_CFGHTSTS1SMOOTHFORCE_POS     20
/// CFGNONHTSMOOTHFORCE field bit
#define MDM_CFGNONHTSMOOTHFORCE_BIT      ((uint32_t)0x00010000)
/// CFGNONHTSMOOTHFORCE field position
#define MDM_CFGNONHTSMOOTHFORCE_POS      16
/// CFGVHTSTS2SMOOTH field mask
#define MDM_CFGVHTSTS2SMOOTH_MASK        ((uint32_t)0x00000C00)
/// CFGVHTSTS2SMOOTH field LSB position
#define MDM_CFGVHTSTS2SMOOTH_LSB         10
/// CFGVHTSTS2SMOOTH field width
#define MDM_CFGVHTSTS2SMOOTH_WIDTH       ((uint32_t)0x00000002)
/// CFGVHTSTS1SMOOTH field mask
#define MDM_CFGVHTSTS1SMOOTH_MASK        ((uint32_t)0x00000300)
/// CFGVHTSTS1SMOOTH field LSB position
#define MDM_CFGVHTSTS1SMOOTH_LSB         8
/// CFGVHTSTS1SMOOTH field width
#define MDM_CFGVHTSTS1SMOOTH_WIDTH       ((uint32_t)0x00000002)
/// CFGHTSTS2SMOOTH field mask
#define MDM_CFGHTSTS2SMOOTH_MASK         ((uint32_t)0x000000C0)
/// CFGHTSTS2SMOOTH field LSB position
#define MDM_CFGHTSTS2SMOOTH_LSB          6
/// CFGHTSTS2SMOOTH field width
#define MDM_CFGHTSTS2SMOOTH_WIDTH        ((uint32_t)0x00000002)
/// CFGHTSTS1SMOOTH field mask
#define MDM_CFGHTSTS1SMOOTH_MASK         ((uint32_t)0x00000030)
/// CFGHTSTS1SMOOTH field LSB position
#define MDM_CFGHTSTS1SMOOTH_LSB          4
/// CFGHTSTS1SMOOTH field width
#define MDM_CFGHTSTS1SMOOTH_WIDTH        ((uint32_t)0x00000002)
/// CFGNONHTSMOOTH field mask
#define MDM_CFGNONHTSMOOTH_MASK          ((uint32_t)0x00000003)
/// CFGNONHTSMOOTH field LSB position
#define MDM_CFGNONHTSMOOTH_LSB           0
/// CFGNONHTSMOOTH field width
#define MDM_CFGNONHTSMOOTH_WIDTH         ((uint32_t)0x00000002)

/// MUSMOOTHFORCE field reset value
#define MDM_MUSMOOTHFORCE_RST            0x0
/// BFSMOOTHFORCE field reset value
#define MDM_BFSMOOTHFORCE_RST            0x0
/// CFGVHTSTS2SMOOTHFORCE field reset value
#define MDM_CFGVHTSTS2SMOOTHFORCE_RST    0x1
/// CFGVHTSTS1SMOOTHFORCE field reset value
#define MDM_CFGVHTSTS1SMOOTHFORCE_RST    0x1
/// CFGHTSTS2SMOOTHFORCE field reset value
#define MDM_CFGHTSTS2SMOOTHFORCE_RST     0x1
/// CFGHTSTS1SMOOTHFORCE field reset value
#define MDM_CFGHTSTS1SMOOTHFORCE_RST     0x1
/// CFGNONHTSMOOTHFORCE field reset value
#define MDM_CFGNONHTSMOOTHFORCE_RST      0x1
/// CFGVHTSTS2SMOOTH field reset value
#define MDM_CFGVHTSTS2SMOOTH_RST         0x0
/// CFGVHTSTS1SMOOTH field reset value
#define MDM_CFGVHTSTS1SMOOTH_RST         0x0
/// CFGHTSTS2SMOOTH field reset value
#define MDM_CFGHTSTS2SMOOTH_RST          0x0
/// CFGHTSTS1SMOOTH field reset value
#define MDM_CFGHTSTS1SMOOTH_RST          0x0
/// CFGNONHTSMOOTH field reset value
#define MDM_CFGNONHTSMOOTH_RST           0x0

/**
 * @brief Constructs a value for the SMOOTHFORCECTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] musmoothforce - The value to use for the MUSMOOTHFORCE field.
 * @param[in] bfsmoothforce - The value to use for the BFSMOOTHFORCE field.
 * @param[in] cfgvhtsts2smoothforce - The value to use for the CFGVHTSTS2SMOOTHFORCE field.
 * @param[in] cfgvhtsts1smoothforce - The value to use for the CFGVHTSTS1SMOOTHFORCE field.
 * @param[in] cfghtsts2smoothforce - The value to use for the CFGHTSTS2SMOOTHFORCE field.
 * @param[in] cfghtsts1smoothforce - The value to use for the CFGHTSTS1SMOOTHFORCE field.
 * @param[in] cfgnonhtsmoothforce - The value to use for the CFGNONHTSMOOTHFORCE field.
 * @param[in] cfgvhtsts2smooth - The value to use for the CFGVHTSTS2SMOOTH field.
 * @param[in] cfgvhtsts1smooth - The value to use for the CFGVHTSTS1SMOOTH field.
 * @param[in] cfghtsts2smooth - The value to use for the CFGHTSTS2SMOOTH field.
 * @param[in] cfghtsts1smooth - The value to use for the CFGHTSTS1SMOOTH field.
 * @param[in] cfgnonhtsmooth - The value to use for the CFGNONHTSMOOTH field.
 */
__INLINE void mdm_smoothforcectrl_pack(uint8_t musmoothforce, uint8_t bfsmoothforce, uint8_t cfgvhtsts2smoothforce, uint8_t cfgvhtsts1smoothforce, uint8_t cfghtsts2smoothforce, uint8_t cfghtsts1smoothforce, uint8_t cfgnonhtsmoothforce, uint8_t cfgvhtsts2smooth, uint8_t cfgvhtsts1smooth, uint8_t cfghtsts2smooth, uint8_t cfghtsts1smooth, uint8_t cfgnonhtsmooth)
{
    ASSERT_ERR((((uint32_t)musmoothforce << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)bfsmoothforce << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)cfgvhtsts2smoothforce << 25) & ~((uint32_t)0x02000000)) == 0);
    ASSERT_ERR((((uint32_t)cfgvhtsts1smoothforce << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)cfghtsts2smoothforce << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)cfghtsts1smoothforce << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)cfgnonhtsmoothforce << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)cfgvhtsts2smooth << 10) & ~((uint32_t)0x00000C00)) == 0);
    ASSERT_ERR((((uint32_t)cfgvhtsts1smooth << 8) & ~((uint32_t)0x00000300)) == 0);
    ASSERT_ERR((((uint32_t)cfghtsts2smooth << 6) & ~((uint32_t)0x000000C0)) == 0);
    ASSERT_ERR((((uint32_t)cfghtsts1smooth << 4) & ~((uint32_t)0x00000030)) == 0);
    ASSERT_ERR((((uint32_t)cfgnonhtsmooth << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR,  ((uint32_t)musmoothforce << 29) | ((uint32_t)bfsmoothforce << 28) | ((uint32_t)cfgvhtsts2smoothforce << 25) | ((uint32_t)cfgvhtsts1smoothforce << 24) | ((uint32_t)cfghtsts2smoothforce << 21) | ((uint32_t)cfghtsts1smoothforce << 20) | ((uint32_t)cfgnonhtsmoothforce << 16) | ((uint32_t)cfgvhtsts2smooth << 10) | ((uint32_t)cfgvhtsts1smooth << 8) | ((uint32_t)cfghtsts2smooth << 6) | ((uint32_t)cfghtsts1smooth << 4) | ((uint32_t)cfgnonhtsmooth << 0));
}

/**
 * @brief Unpacks SMOOTHFORCECTRL's fields from current value of the SMOOTHFORCECTRL register.
 *
 * Reads the SMOOTHFORCECTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] musmoothforce - Will be populated with the current value of this field from the register.
 * @param[out] bfsmoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfgvhtsts2smoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfgvhtsts1smoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfghtsts2smoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfghtsts1smoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfgnonhtsmoothforce - Will be populated with the current value of this field from the register.
 * @param[out] cfgvhtsts2smooth - Will be populated with the current value of this field from the register.
 * @param[out] cfgvhtsts1smooth - Will be populated with the current value of this field from the register.
 * @param[out] cfghtsts2smooth - Will be populated with the current value of this field from the register.
 * @param[out] cfghtsts1smooth - Will be populated with the current value of this field from the register.
 * @param[out] cfgnonhtsmooth - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_smoothforcectrl_unpack(uint8_t* musmoothforce, uint8_t* bfsmoothforce, uint8_t* cfgvhtsts2smoothforce, uint8_t* cfgvhtsts1smoothforce, uint8_t* cfghtsts2smoothforce, uint8_t* cfghtsts1smoothforce, uint8_t* cfgnonhtsmoothforce, uint8_t* cfgvhtsts2smooth, uint8_t* cfgvhtsts1smooth, uint8_t* cfghtsts2smooth, uint8_t* cfghtsts1smooth, uint8_t* cfgnonhtsmooth)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);

    *musmoothforce = (localVal & ((uint32_t)0x20000000)) >> 29;
    *bfsmoothforce = (localVal & ((uint32_t)0x10000000)) >> 28;
    *cfgvhtsts2smoothforce = (localVal & ((uint32_t)0x02000000)) >> 25;
    *cfgvhtsts1smoothforce = (localVal & ((uint32_t)0x01000000)) >> 24;
    *cfghtsts2smoothforce = (localVal & ((uint32_t)0x00200000)) >> 21;
    *cfghtsts1smoothforce = (localVal & ((uint32_t)0x00100000)) >> 20;
    *cfgnonhtsmoothforce = (localVal & ((uint32_t)0x00010000)) >> 16;
    *cfgvhtsts2smooth = (localVal & ((uint32_t)0x00000C00)) >> 10;
    *cfgvhtsts1smooth = (localVal & ((uint32_t)0x00000300)) >> 8;
    *cfghtsts2smooth = (localVal & ((uint32_t)0x000000C0)) >> 6;
    *cfghtsts1smooth = (localVal & ((uint32_t)0x00000030)) >> 4;
    *cfgnonhtsmooth = (localVal & ((uint32_t)0x00000003)) >> 0;
}

/**
 * @brief Returns the current value of the MUSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the MUSMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the MUSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_musmoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Sets the MUSMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] musmoothforce - The value to set the field to.
 */
__INLINE void mdm_musmoothforce_setf(uint8_t musmoothforce)
{
    ASSERT_ERR((((uint32_t)musmoothforce << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)musmoothforce << 29));
}

/**
 * @brief Returns the current value of the BFSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the BFSMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the BFSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_bfsmoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the BFSMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] bfsmoothforce - The value to set the field to.
 */
__INLINE void mdm_bfsmoothforce_setf(uint8_t bfsmoothforce)
{
    ASSERT_ERR((((uint32_t)bfsmoothforce << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)bfsmoothforce << 28));
}

/**
 * @brief Returns the current value of the CFGVHTSTS2SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGVHTSTS2SMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the CFGVHTSTS2SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgvhtsts2smoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

/**
 * @brief Sets the CFGVHTSTS2SMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgvhtsts2smoothforce - The value to set the field to.
 */
__INLINE void mdm_cfgvhtsts2smoothforce_setf(uint8_t cfgvhtsts2smoothforce)
{
    ASSERT_ERR((((uint32_t)cfgvhtsts2smoothforce << 25) & ~((uint32_t)0x02000000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x02000000)) | ((uint32_t)cfgvhtsts2smoothforce << 25));
}

/**
 * @brief Returns the current value of the CFGVHTSTS1SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGVHTSTS1SMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the CFGVHTSTS1SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgvhtsts1smoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the CFGVHTSTS1SMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgvhtsts1smoothforce - The value to set the field to.
 */
__INLINE void mdm_cfgvhtsts1smoothforce_setf(uint8_t cfgvhtsts1smoothforce)
{
    ASSERT_ERR((((uint32_t)cfgvhtsts1smoothforce << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)cfgvhtsts1smoothforce << 24));
}

/**
 * @brief Returns the current value of the CFGHTSTS2SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGHTSTS2SMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the CFGHTSTS2SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfghtsts2smoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Sets the CFGHTSTS2SMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfghtsts2smoothforce - The value to set the field to.
 */
__INLINE void mdm_cfghtsts2smoothforce_setf(uint8_t cfghtsts2smoothforce)
{
    ASSERT_ERR((((uint32_t)cfghtsts2smoothforce << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)cfghtsts2smoothforce << 21));
}

/**
 * @brief Returns the current value of the CFGHTSTS1SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGHTSTS1SMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the CFGHTSTS1SMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfghtsts1smoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the CFGHTSTS1SMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfghtsts1smoothforce - The value to set the field to.
 */
__INLINE void mdm_cfghtsts1smoothforce_setf(uint8_t cfghtsts1smoothforce)
{
    ASSERT_ERR((((uint32_t)cfghtsts1smoothforce << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)cfghtsts1smoothforce << 20));
}

/**
 * @brief Returns the current value of the CFGNONHTSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGNONHTSMOOTHFORCE field's value will be returned.
 *
 * @return The current value of the CFGNONHTSMOOTHFORCE field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgnonhtsmoothforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the CFGNONHTSMOOTHFORCE field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgnonhtsmoothforce - The value to set the field to.
 */
__INLINE void mdm_cfgnonhtsmoothforce_setf(uint8_t cfgnonhtsmoothforce)
{
    ASSERT_ERR((((uint32_t)cfgnonhtsmoothforce << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)cfgnonhtsmoothforce << 16));
}

/**
 * @brief Returns the current value of the CFGVHTSTS2SMOOTH field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGVHTSTS2SMOOTH field's value will be returned.
 *
 * @return The current value of the CFGVHTSTS2SMOOTH field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgvhtsts2smooth_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000C00)) >> 10);
}

/**
 * @brief Sets the CFGVHTSTS2SMOOTH field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgvhtsts2smooth - The value to set the field to.
 */
__INLINE void mdm_cfgvhtsts2smooth_setf(uint8_t cfgvhtsts2smooth)
{
    ASSERT_ERR((((uint32_t)cfgvhtsts2smooth << 10) & ~((uint32_t)0x00000C00)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00000C00)) | ((uint32_t)cfgvhtsts2smooth << 10));
}

/**
 * @brief Returns the current value of the CFGVHTSTS1SMOOTH field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGVHTSTS1SMOOTH field's value will be returned.
 *
 * @return The current value of the CFGVHTSTS1SMOOTH field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgvhtsts1smooth_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000300)) >> 8);
}

/**
 * @brief Sets the CFGVHTSTS1SMOOTH field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgvhtsts1smooth - The value to set the field to.
 */
__INLINE void mdm_cfgvhtsts1smooth_setf(uint8_t cfgvhtsts1smooth)
{
    ASSERT_ERR((((uint32_t)cfgvhtsts1smooth << 8) & ~((uint32_t)0x00000300)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00000300)) | ((uint32_t)cfgvhtsts1smooth << 8));
}

/**
 * @brief Returns the current value of the CFGHTSTS2SMOOTH field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGHTSTS2SMOOTH field's value will be returned.
 *
 * @return The current value of the CFGHTSTS2SMOOTH field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfghtsts2smooth_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x000000C0)) >> 6);
}

/**
 * @brief Sets the CFGHTSTS2SMOOTH field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfghtsts2smooth - The value to set the field to.
 */
__INLINE void mdm_cfghtsts2smooth_setf(uint8_t cfghtsts2smooth)
{
    ASSERT_ERR((((uint32_t)cfghtsts2smooth << 6) & ~((uint32_t)0x000000C0)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x000000C0)) | ((uint32_t)cfghtsts2smooth << 6));
}

/**
 * @brief Returns the current value of the CFGHTSTS1SMOOTH field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGHTSTS1SMOOTH field's value will be returned.
 *
 * @return The current value of the CFGHTSTS1SMOOTH field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfghtsts1smooth_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000030)) >> 4);
}

/**
 * @brief Sets the CFGHTSTS1SMOOTH field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfghtsts1smooth - The value to set the field to.
 */
__INLINE void mdm_cfghtsts1smooth_setf(uint8_t cfghtsts1smooth)
{
    ASSERT_ERR((((uint32_t)cfghtsts1smooth << 4) & ~((uint32_t)0x00000030)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00000030)) | ((uint32_t)cfghtsts1smooth << 4));
}

/**
 * @brief Returns the current value of the CFGNONHTSMOOTH field in the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read and the CFGNONHTSMOOTH field's value will be returned.
 *
 * @return The current value of the CFGNONHTSMOOTH field in the SMOOTHFORCECTRL register.
 */
__INLINE uint8_t mdm_cfgnonhtsmooth_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

/**
 * @brief Sets the CFGNONHTSMOOTH field of the SMOOTHFORCECTRL register.
 *
 * The SMOOTHFORCECTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] cfgnonhtsmooth - The value to set the field to.
 */
__INLINE void mdm_cfgnonhtsmooth_setf(uint8_t cfgnonhtsmooth)
{
    ASSERT_ERR((((uint32_t)cfgnonhtsmooth << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MDM_SMOOTHFORCECTRL_ADDR, (REG_PL_RD(MDM_SMOOTHFORCECTRL_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)cfgnonhtsmooth << 0));
}

/// @}

/**
 * @name SCRAMBLERCTRL register definitions
 * <table>
 * <caption id="SCRAMBLERCTRL_BF">SCRAMBLERCTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>07:00 <td>         SCRAMSEED <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the SCRAMBLERCTRL register
#define MDM_SCRAMBLERCTRL_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0854)
/// Offset of the SCRAMBLERCTRL register from the base address
#define MDM_SCRAMBLERCTRL_OFFSET 0x00000054
/// Index of the SCRAMBLERCTRL register
#define MDM_SCRAMBLERCTRL_INDEX  0x00000015
/// Reset value of the SCRAMBLERCTRL register
#define MDM_SCRAMBLERCTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the SCRAMBLERCTRL register.
 * The SCRAMBLERCTRL register will be read and its value returned.
 * @return The current value of the SCRAMBLERCTRL register.
 */
__INLINE uint32_t mdm_scramblerctrl_get(void)
{
    return REG_PL_RD(MDM_SCRAMBLERCTRL_ADDR);
}

/**
 * @brief Sets the SCRAMBLERCTRL register to a value.
 * The SCRAMBLERCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_scramblerctrl_set(uint32_t value)
{
    REG_PL_WR(MDM_SCRAMBLERCTRL_ADDR, value);
}

// field definitions
/// SCRAMSEED field mask
#define MDM_SCRAMSEED_MASK   ((uint32_t)0x000000FF)
/// SCRAMSEED field LSB position
#define MDM_SCRAMSEED_LSB    0
/// SCRAMSEED field width
#define MDM_SCRAMSEED_WIDTH  ((uint32_t)0x00000008)

/// SCRAMSEED field reset value
#define MDM_SCRAMSEED_RST    0x0

/**
 * @brief Returns the current value of the SCRAMSEED field in the SCRAMBLERCTRL register.
 *
 * The SCRAMBLERCTRL register will be read and the SCRAMSEED field's value will be returned.
 *
 * @return The current value of the SCRAMSEED field in the SCRAMBLERCTRL register.
 */
__INLINE uint8_t mdm_scramseed_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SCRAMBLERCTRL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the SCRAMSEED field of the SCRAMBLERCTRL register.
 *
 * The SCRAMBLERCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] scramseed - The value to set the field to.
 */
__INLINE void mdm_scramseed_setf(uint8_t scramseed)
{
    ASSERT_ERR((((uint32_t)scramseed << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_SCRAMBLERCTRL_ADDR, (uint32_t)scramseed << 0);
}

/// @}

/**
 * @name TBECTRL0 register definitions
 * <table>
 * <caption id="TBECTRL0_BF">TBECTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:24 <td>CrossCorrSNRThreshold <td>R <td>R/W <td>0xC
 * <tr><td>23:16 <td>  TBECountAdjust80 <td>R <td>R/W <td>0x13
 * <tr><td>15:08 <td>  TBECountAdjust40 <td>R <td>R/W <td>0x8
 * <tr><td>07:00 <td>  TBECountAdjust20 <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the TBECTRL0 register
#define MDM_TBECTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0858)
/// Offset of the TBECTRL0 register from the base address
#define MDM_TBECTRL0_OFFSET 0x00000058
/// Index of the TBECTRL0 register
#define MDM_TBECTRL0_INDEX  0x00000016
/// Reset value of the TBECTRL0 register
#define MDM_TBECTRL0_RESET  0x0C130800

/**
 * @brief Returns the current value of the TBECTRL0 register.
 * The TBECTRL0 register will be read and its value returned.
 * @return The current value of the TBECTRL0 register.
 */
__INLINE uint32_t mdm_tbectrl0_get(void)
{
    return REG_PL_RD(MDM_TBECTRL0_ADDR);
}

/**
 * @brief Sets the TBECTRL0 register to a value.
 * The TBECTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tbectrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_TBECTRL0_ADDR, value);
}

// field definitions
/// CROSS_CORR_SNR_THRESHOLD field mask
#define MDM_CROSS_CORR_SNR_THRESHOLD_MASK   ((uint32_t)0xFF000000)
/// CROSS_CORR_SNR_THRESHOLD field LSB position
#define MDM_CROSS_CORR_SNR_THRESHOLD_LSB    24
/// CROSS_CORR_SNR_THRESHOLD field width
#define MDM_CROSS_CORR_SNR_THRESHOLD_WIDTH  ((uint32_t)0x00000008)
/// TBE_COUNT_ADJUST_80 field mask
#define MDM_TBE_COUNT_ADJUST_80_MASK        ((uint32_t)0x00FF0000)
/// TBE_COUNT_ADJUST_80 field LSB position
#define MDM_TBE_COUNT_ADJUST_80_LSB         16
/// TBE_COUNT_ADJUST_80 field width
#define MDM_TBE_COUNT_ADJUST_80_WIDTH       ((uint32_t)0x00000008)
/// TBE_COUNT_ADJUST_40 field mask
#define MDM_TBE_COUNT_ADJUST_40_MASK        ((uint32_t)0x0000FF00)
/// TBE_COUNT_ADJUST_40 field LSB position
#define MDM_TBE_COUNT_ADJUST_40_LSB         8
/// TBE_COUNT_ADJUST_40 field width
#define MDM_TBE_COUNT_ADJUST_40_WIDTH       ((uint32_t)0x00000008)
/// TBE_COUNT_ADJUST_20 field mask
#define MDM_TBE_COUNT_ADJUST_20_MASK        ((uint32_t)0x000000FF)
/// TBE_COUNT_ADJUST_20 field LSB position
#define MDM_TBE_COUNT_ADJUST_20_LSB         0
/// TBE_COUNT_ADJUST_20 field width
#define MDM_TBE_COUNT_ADJUST_20_WIDTH       ((uint32_t)0x00000008)

/// CROSS_CORR_SNR_THRESHOLD field reset value
#define MDM_CROSS_CORR_SNR_THRESHOLD_RST    0xC
/// TBE_COUNT_ADJUST_80 field reset value
#define MDM_TBE_COUNT_ADJUST_80_RST         0x13
/// TBE_COUNT_ADJUST_40 field reset value
#define MDM_TBE_COUNT_ADJUST_40_RST         0x8
/// TBE_COUNT_ADJUST_20 field reset value
#define MDM_TBE_COUNT_ADJUST_20_RST         0x0

/**
 * @brief Constructs a value for the TBECTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] crosscorrsnrthreshold - The value to use for the CrossCorrSNRThreshold field.
 * @param[in] tbecountadjust80 - The value to use for the TBECountAdjust80 field.
 * @param[in] tbecountadjust40 - The value to use for the TBECountAdjust40 field.
 * @param[in] tbecountadjust20 - The value to use for the TBECountAdjust20 field.
 */
__INLINE void mdm_tbectrl0_pack(uint8_t crosscorrsnrthreshold, uint8_t tbecountadjust80, uint8_t tbecountadjust40, uint8_t tbecountadjust20)
{
    ASSERT_ERR((((uint32_t)crosscorrsnrthreshold << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)tbecountadjust80 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)tbecountadjust40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)tbecountadjust20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TBECTRL0_ADDR,  ((uint32_t)crosscorrsnrthreshold << 24) | ((uint32_t)tbecountadjust80 << 16) | ((uint32_t)tbecountadjust40 << 8) | ((uint32_t)tbecountadjust20 << 0));
}

/**
 * @brief Unpacks TBECTRL0's fields from current value of the TBECTRL0 register.
 *
 * Reads the TBECTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] crosscorrsnrthreshold - Will be populated with the current value of this field from the register.
 * @param[out] tbecountadjust80 - Will be populated with the current value of this field from the register.
 * @param[out] tbecountadjust40 - Will be populated with the current value of this field from the register.
 * @param[out] tbecountadjust20 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tbectrl0_unpack(uint8_t* crosscorrsnrthreshold, uint8_t* tbecountadjust80, uint8_t* tbecountadjust40, uint8_t* tbecountadjust20)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL0_ADDR);

    *crosscorrsnrthreshold = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *tbecountadjust80 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *tbecountadjust40 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *tbecountadjust20 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the CrossCorrSNRThreshold field in the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read and the CrossCorrSNRThreshold field's value will be returned.
 *
 * @return The current value of the CrossCorrSNRThreshold field in the TBECTRL0 register.
 */
__INLINE uint8_t mdm_cross_corr_snr_threshold_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL0_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the CrossCorrSNRThreshold field of the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] crosscorrsnrthreshold - The value to set the field to.
 */
__INLINE void mdm_cross_corr_snr_threshold_setf(uint8_t crosscorrsnrthreshold)
{
    ASSERT_ERR((((uint32_t)crosscorrsnrthreshold << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(MDM_TBECTRL0_ADDR, (REG_PL_RD(MDM_TBECTRL0_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)crosscorrsnrthreshold << 24));
}

/**
 * @brief Returns the current value of the TBECountAdjust80 field in the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read and the TBECountAdjust80 field's value will be returned.
 *
 * @return The current value of the TBECountAdjust80 field in the TBECTRL0 register.
 */
__INLINE uint8_t mdm_tbe_count_adjust_80_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the TBECountAdjust80 field of the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbecountadjust80 - The value to set the field to.
 */
__INLINE void mdm_tbe_count_adjust_80_setf(uint8_t tbecountadjust80)
{
    ASSERT_ERR((((uint32_t)tbecountadjust80 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(MDM_TBECTRL0_ADDR, (REG_PL_RD(MDM_TBECTRL0_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)tbecountadjust80 << 16));
}

/**
 * @brief Returns the current value of the TBECountAdjust40 field in the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read and the TBECountAdjust40 field's value will be returned.
 *
 * @return The current value of the TBECountAdjust40 field in the TBECTRL0 register.
 */
__INLINE uint8_t mdm_tbe_count_adjust_40_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the TBECountAdjust40 field of the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbecountadjust40 - The value to set the field to.
 */
__INLINE void mdm_tbe_count_adjust_40_setf(uint8_t tbecountadjust40)
{
    ASSERT_ERR((((uint32_t)tbecountadjust40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_TBECTRL0_ADDR, (REG_PL_RD(MDM_TBECTRL0_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)tbecountadjust40 << 8));
}

/**
 * @brief Returns the current value of the TBECountAdjust20 field in the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read and the TBECountAdjust20 field's value will be returned.
 *
 * @return The current value of the TBECountAdjust20 field in the TBECTRL0 register.
 */
__INLINE uint8_t mdm_tbe_count_adjust_20_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TBECountAdjust20 field of the TBECTRL0 register.
 *
 * The TBECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbecountadjust20 - The value to set the field to.
 */
__INLINE void mdm_tbe_count_adjust_20_setf(uint8_t tbecountadjust20)
{
    ASSERT_ERR((((uint32_t)tbecountadjust20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TBECTRL0_ADDR, (REG_PL_RD(MDM_TBECTRL0_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)tbecountadjust20 << 0));
}

/// @}

/**
 * @name TBECTRL1 register definitions
 * <table>
 * <caption id="TBECTRL1_BF">TBECTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>30:24 <td>PeakSearchStopHighSNR <td>R <td>R/W <td>0x3C
 * <tr><td>22:16 <td>PeakSearchStartHighSNR <td>R <td>R/W <td>0x14
 * <tr><td>14:08 <td>PeakSearchStopLowSNR <td>R <td>R/W <td>0x3E
 * <tr><td>06:00 <td>PeakSearchStartLowSNR <td>R <td>R/W <td>0x14
 * </table>
 *
 * @{
 */

/// Address of the TBECTRL1 register
#define MDM_TBECTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x085C)
/// Offset of the TBECTRL1 register from the base address
#define MDM_TBECTRL1_OFFSET 0x0000005C
/// Index of the TBECTRL1 register
#define MDM_TBECTRL1_INDEX  0x00000017
/// Reset value of the TBECTRL1 register
#define MDM_TBECTRL1_RESET  0x3C143E14

/**
 * @brief Returns the current value of the TBECTRL1 register.
 * The TBECTRL1 register will be read and its value returned.
 * @return The current value of the TBECTRL1 register.
 */
__INLINE uint32_t mdm_tbectrl1_get(void)
{
    return REG_PL_RD(MDM_TBECTRL1_ADDR);
}

/**
 * @brief Sets the TBECTRL1 register to a value.
 * The TBECTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tbectrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_TBECTRL1_ADDR, value);
}

// field definitions
/// PEAK_SEARCH_STOP_HIGH_SNR field mask
#define MDM_PEAK_SEARCH_STOP_HIGH_SNR_MASK    ((uint32_t)0x7F000000)
/// PEAK_SEARCH_STOP_HIGH_SNR field LSB position
#define MDM_PEAK_SEARCH_STOP_HIGH_SNR_LSB     24
/// PEAK_SEARCH_STOP_HIGH_SNR field width
#define MDM_PEAK_SEARCH_STOP_HIGH_SNR_WIDTH   ((uint32_t)0x00000007)
/// PEAK_SEARCH_START_HIGH_SNR field mask
#define MDM_PEAK_SEARCH_START_HIGH_SNR_MASK   ((uint32_t)0x007F0000)
/// PEAK_SEARCH_START_HIGH_SNR field LSB position
#define MDM_PEAK_SEARCH_START_HIGH_SNR_LSB    16
/// PEAK_SEARCH_START_HIGH_SNR field width
#define MDM_PEAK_SEARCH_START_HIGH_SNR_WIDTH  ((uint32_t)0x00000007)
/// PEAK_SEARCH_STOP_LOW_SNR field mask
#define MDM_PEAK_SEARCH_STOP_LOW_SNR_MASK     ((uint32_t)0x00007F00)
/// PEAK_SEARCH_STOP_LOW_SNR field LSB position
#define MDM_PEAK_SEARCH_STOP_LOW_SNR_LSB      8
/// PEAK_SEARCH_STOP_LOW_SNR field width
#define MDM_PEAK_SEARCH_STOP_LOW_SNR_WIDTH    ((uint32_t)0x00000007)
/// PEAK_SEARCH_START_LOW_SNR field mask
#define MDM_PEAK_SEARCH_START_LOW_SNR_MASK    ((uint32_t)0x0000007F)
/// PEAK_SEARCH_START_LOW_SNR field LSB position
#define MDM_PEAK_SEARCH_START_LOW_SNR_LSB     0
/// PEAK_SEARCH_START_LOW_SNR field width
#define MDM_PEAK_SEARCH_START_LOW_SNR_WIDTH   ((uint32_t)0x00000007)

/// PEAK_SEARCH_STOP_HIGH_SNR field reset value
#define MDM_PEAK_SEARCH_STOP_HIGH_SNR_RST     0x3C
/// PEAK_SEARCH_START_HIGH_SNR field reset value
#define MDM_PEAK_SEARCH_START_HIGH_SNR_RST    0x14
/// PEAK_SEARCH_STOP_LOW_SNR field reset value
#define MDM_PEAK_SEARCH_STOP_LOW_SNR_RST      0x3E
/// PEAK_SEARCH_START_LOW_SNR field reset value
#define MDM_PEAK_SEARCH_START_LOW_SNR_RST     0x14

/**
 * @brief Constructs a value for the TBECTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] peaksearchstophighsnr - The value to use for the PeakSearchStopHighSNR field.
 * @param[in] peaksearchstarthighsnr - The value to use for the PeakSearchStartHighSNR field.
 * @param[in] peaksearchstoplowsnr - The value to use for the PeakSearchStopLowSNR field.
 * @param[in] peaksearchstartlowsnr - The value to use for the PeakSearchStartLowSNR field.
 */
__INLINE void mdm_tbectrl1_pack(uint8_t peaksearchstophighsnr, uint8_t peaksearchstarthighsnr, uint8_t peaksearchstoplowsnr, uint8_t peaksearchstartlowsnr)
{
    ASSERT_ERR((((uint32_t)peaksearchstophighsnr << 24) & ~((uint32_t)0x7F000000)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchstarthighsnr << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchstoplowsnr << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchstartlowsnr << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MDM_TBECTRL1_ADDR,  ((uint32_t)peaksearchstophighsnr << 24) | ((uint32_t)peaksearchstarthighsnr << 16) | ((uint32_t)peaksearchstoplowsnr << 8) | ((uint32_t)peaksearchstartlowsnr << 0));
}

/**
 * @brief Unpacks TBECTRL1's fields from current value of the TBECTRL1 register.
 *
 * Reads the TBECTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] peaksearchstophighsnr - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchstarthighsnr - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchstoplowsnr - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchstartlowsnr - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tbectrl1_unpack(uint8_t* peaksearchstophighsnr, uint8_t* peaksearchstarthighsnr, uint8_t* peaksearchstoplowsnr, uint8_t* peaksearchstartlowsnr)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL1_ADDR);

    *peaksearchstophighsnr = (localVal & ((uint32_t)0x7F000000)) >> 24;
    *peaksearchstarthighsnr = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *peaksearchstoplowsnr = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *peaksearchstartlowsnr = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the PeakSearchStopHighSNR field in the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read and the PeakSearchStopHighSNR field's value will be returned.
 *
 * @return The current value of the PeakSearchStopHighSNR field in the TBECTRL1 register.
 */
__INLINE uint8_t mdm_peak_search_stop_high_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x7F000000)) >> 24);
}

/**
 * @brief Sets the PeakSearchStopHighSNR field of the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchstophighsnr - The value to set the field to.
 */
__INLINE void mdm_peak_search_stop_high_snr_setf(uint8_t peaksearchstophighsnr)
{
    ASSERT_ERR((((uint32_t)peaksearchstophighsnr << 24) & ~((uint32_t)0x7F000000)) == 0);
    REG_PL_WR(MDM_TBECTRL1_ADDR, (REG_PL_RD(MDM_TBECTRL1_ADDR) & ~((uint32_t)0x7F000000)) | ((uint32_t)peaksearchstophighsnr << 24));
}

/**
 * @brief Returns the current value of the PeakSearchStartHighSNR field in the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read and the PeakSearchStartHighSNR field's value will be returned.
 *
 * @return The current value of the PeakSearchStartHighSNR field in the TBECTRL1 register.
 */
__INLINE uint8_t mdm_peak_search_start_high_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the PeakSearchStartHighSNR field of the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchstarthighsnr - The value to set the field to.
 */
__INLINE void mdm_peak_search_start_high_snr_setf(uint8_t peaksearchstarthighsnr)
{
    ASSERT_ERR((((uint32_t)peaksearchstarthighsnr << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(MDM_TBECTRL1_ADDR, (REG_PL_RD(MDM_TBECTRL1_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)peaksearchstarthighsnr << 16));
}

/**
 * @brief Returns the current value of the PeakSearchStopLowSNR field in the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read and the PeakSearchStopLowSNR field's value will be returned.
 *
 * @return The current value of the PeakSearchStopLowSNR field in the TBECTRL1 register.
 */
__INLINE uint8_t mdm_peak_search_stop_low_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the PeakSearchStopLowSNR field of the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchstoplowsnr - The value to set the field to.
 */
__INLINE void mdm_peak_search_stop_low_snr_setf(uint8_t peaksearchstoplowsnr)
{
    ASSERT_ERR((((uint32_t)peaksearchstoplowsnr << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(MDM_TBECTRL1_ADDR, (REG_PL_RD(MDM_TBECTRL1_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)peaksearchstoplowsnr << 8));
}

/**
 * @brief Returns the current value of the PeakSearchStartLowSNR field in the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read and the PeakSearchStartLowSNR field's value will be returned.
 *
 * @return The current value of the PeakSearchStartLowSNR field in the TBECTRL1 register.
 */
__INLINE uint8_t mdm_peak_search_start_low_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the PeakSearchStartLowSNR field of the TBECTRL1 register.
 *
 * The TBECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchstartlowsnr - The value to set the field to.
 */
__INLINE void mdm_peak_search_start_low_snr_setf(uint8_t peaksearchstartlowsnr)
{
    ASSERT_ERR((((uint32_t)peaksearchstartlowsnr << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(MDM_TBECTRL1_ADDR, (REG_PL_RD(MDM_TBECTRL1_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)peaksearchstartlowsnr << 0));
}

/// @}

/**
 * @name TBECTRL2 register definitions
 * <table>
 * <caption id="TBECTRL2_BF">TBECTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:28 <td> TDAdjust80ShortGI <td>R <td>R/W <td>0x0
 * <tr><td>27:24 <td> TDAdjust40ShortGI <td>R <td>R/W <td>0x0
 * <tr><td>23:20 <td> TDAdjust20ShortGI <td>R <td>R/W <td>0x0
 * <tr><td>16:12 <td>   PeakSearchDelta <td>R <td>R/W <td>0x7
 * <tr><td>11 <td>   PeakSearchNdlEn <td>R <td>R/W <td>1
 * <tr><td>10:08 <td>PeakSearchNdlIndex <td>R <td>R/W <td>0x7
 * <tr><td>07:00 <td>           TBEBias <td>R <td>R/W <td>0x5
 * </table>
 *
 * @{
 */

/// Address of the TBECTRL2 register
#define MDM_TBECTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0860)
/// Offset of the TBECTRL2 register from the base address
#define MDM_TBECTRL2_OFFSET 0x00000060
/// Index of the TBECTRL2 register
#define MDM_TBECTRL2_INDEX  0x00000018
/// Reset value of the TBECTRL2 register
#define MDM_TBECTRL2_RESET  0x00007F05

/**
 * @brief Returns the current value of the TBECTRL2 register.
 * The TBECTRL2 register will be read and its value returned.
 * @return The current value of the TBECTRL2 register.
 */
__INLINE uint32_t mdm_tbectrl2_get(void)
{
    return REG_PL_RD(MDM_TBECTRL2_ADDR);
}

/**
 * @brief Sets the TBECTRL2 register to a value.
 * The TBECTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tbectrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_TBECTRL2_ADDR, value);
}

// field definitions
/// TD_ADJUST_80_SHORT_GI field mask
#define MDM_TD_ADJUST_80_SHORT_GI_MASK   ((uint32_t)0xF0000000)
/// TD_ADJUST_80_SHORT_GI field LSB position
#define MDM_TD_ADJUST_80_SHORT_GI_LSB    28
/// TD_ADJUST_80_SHORT_GI field width
#define MDM_TD_ADJUST_80_SHORT_GI_WIDTH  ((uint32_t)0x00000004)
/// TD_ADJUST_40_SHORT_GI field mask
#define MDM_TD_ADJUST_40_SHORT_GI_MASK   ((uint32_t)0x0F000000)
/// TD_ADJUST_40_SHORT_GI field LSB position
#define MDM_TD_ADJUST_40_SHORT_GI_LSB    24
/// TD_ADJUST_40_SHORT_GI field width
#define MDM_TD_ADJUST_40_SHORT_GI_WIDTH  ((uint32_t)0x00000004)
/// TD_ADJUST_20_SHORT_GI field mask
#define MDM_TD_ADJUST_20_SHORT_GI_MASK   ((uint32_t)0x00F00000)
/// TD_ADJUST_20_SHORT_GI field LSB position
#define MDM_TD_ADJUST_20_SHORT_GI_LSB    20
/// TD_ADJUST_20_SHORT_GI field width
#define MDM_TD_ADJUST_20_SHORT_GI_WIDTH  ((uint32_t)0x00000004)
/// PEAK_SEARCH_DELTA field mask
#define MDM_PEAK_SEARCH_DELTA_MASK       ((uint32_t)0x0001F000)
/// PEAK_SEARCH_DELTA field LSB position
#define MDM_PEAK_SEARCH_DELTA_LSB        12
/// PEAK_SEARCH_DELTA field width
#define MDM_PEAK_SEARCH_DELTA_WIDTH      ((uint32_t)0x00000005)
/// PEAK_SEARCH_NDL_EN field bit
#define MDM_PEAK_SEARCH_NDL_EN_BIT       ((uint32_t)0x00000800)
/// PEAK_SEARCH_NDL_EN field position
#define MDM_PEAK_SEARCH_NDL_EN_POS       11
/// PEAK_SEARCH_NDL_INDEX field mask
#define MDM_PEAK_SEARCH_NDL_INDEX_MASK   ((uint32_t)0x00000700)
/// PEAK_SEARCH_NDL_INDEX field LSB position
#define MDM_PEAK_SEARCH_NDL_INDEX_LSB    8
/// PEAK_SEARCH_NDL_INDEX field width
#define MDM_PEAK_SEARCH_NDL_INDEX_WIDTH  ((uint32_t)0x00000003)
/// TBE_BIAS field mask
#define MDM_TBE_BIAS_MASK                ((uint32_t)0x000000FF)
/// TBE_BIAS field LSB position
#define MDM_TBE_BIAS_LSB                 0
/// TBE_BIAS field width
#define MDM_TBE_BIAS_WIDTH               ((uint32_t)0x00000008)

/// TD_ADJUST_80_SHORT_GI field reset value
#define MDM_TD_ADJUST_80_SHORT_GI_RST    0x0
/// TD_ADJUST_40_SHORT_GI field reset value
#define MDM_TD_ADJUST_40_SHORT_GI_RST    0x0
/// TD_ADJUST_20_SHORT_GI field reset value
#define MDM_TD_ADJUST_20_SHORT_GI_RST    0x0
/// PEAK_SEARCH_DELTA field reset value
#define MDM_PEAK_SEARCH_DELTA_RST        0x7
/// PEAK_SEARCH_NDL_EN field reset value
#define MDM_PEAK_SEARCH_NDL_EN_RST       0x1
/// PEAK_SEARCH_NDL_INDEX field reset value
#define MDM_PEAK_SEARCH_NDL_INDEX_RST    0x7
/// TBE_BIAS field reset value
#define MDM_TBE_BIAS_RST                 0x5

/**
 * @brief Constructs a value for the TBECTRL2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tdadjust80shortgi - The value to use for the TDAdjust80ShortGI field.
 * @param[in] tdadjust40shortgi - The value to use for the TDAdjust40ShortGI field.
 * @param[in] tdadjust20shortgi - The value to use for the TDAdjust20ShortGI field.
 * @param[in] peaksearchdelta - The value to use for the PeakSearchDelta field.
 * @param[in] peaksearchndlen - The value to use for the PeakSearchNdlEn field.
 * @param[in] peaksearchndlindex - The value to use for the PeakSearchNdlIndex field.
 * @param[in] tbebias - The value to use for the TBEBias field.
 */
__INLINE void mdm_tbectrl2_pack(uint8_t tdadjust80shortgi, uint8_t tdadjust40shortgi, uint8_t tdadjust20shortgi, uint8_t peaksearchdelta, uint8_t peaksearchndlen, uint8_t peaksearchndlindex, uint8_t tbebias)
{
    ASSERT_ERR((((uint32_t)tdadjust80shortgi << 28) & ~((uint32_t)0xF0000000)) == 0);
    ASSERT_ERR((((uint32_t)tdadjust40shortgi << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)tdadjust20shortgi << 20) & ~((uint32_t)0x00F00000)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchdelta << 12) & ~((uint32_t)0x0001F000)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchndlen << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)peaksearchndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    ASSERT_ERR((((uint32_t)tbebias << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR,  ((uint32_t)tdadjust80shortgi << 28) | ((uint32_t)tdadjust40shortgi << 24) | ((uint32_t)tdadjust20shortgi << 20) | ((uint32_t)peaksearchdelta << 12) | ((uint32_t)peaksearchndlen << 11) | ((uint32_t)peaksearchndlindex << 8) | ((uint32_t)tbebias << 0));
}

/**
 * @brief Unpacks TBECTRL2's fields from current value of the TBECTRL2 register.
 *
 * Reads the TBECTRL2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tdadjust80shortgi - Will be populated with the current value of this field from the register.
 * @param[out] tdadjust40shortgi - Will be populated with the current value of this field from the register.
 * @param[out] tdadjust20shortgi - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchdelta - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchndlen - Will be populated with the current value of this field from the register.
 * @param[out] peaksearchndlindex - Will be populated with the current value of this field from the register.
 * @param[out] tbebias - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tbectrl2_unpack(uint8_t* tdadjust80shortgi, uint8_t* tdadjust40shortgi, uint8_t* tdadjust20shortgi, uint8_t* peaksearchdelta, uint8_t* peaksearchndlen, uint8_t* peaksearchndlindex, uint8_t* tbebias)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);

    *tdadjust80shortgi = (localVal & ((uint32_t)0xF0000000)) >> 28;
    *tdadjust40shortgi = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *tdadjust20shortgi = (localVal & ((uint32_t)0x00F00000)) >> 20;
    *peaksearchdelta = (localVal & ((uint32_t)0x0001F000)) >> 12;
    *peaksearchndlen = (localVal & ((uint32_t)0x00000800)) >> 11;
    *peaksearchndlindex = (localVal & ((uint32_t)0x00000700)) >> 8;
    *tbebias = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TDAdjust80ShortGI field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the TDAdjust80ShortGI field's value will be returned.
 *
 * @return The current value of the TDAdjust80ShortGI field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_td_adjust_80_short_gi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0xF0000000)) >> 28);
}

/**
 * @brief Sets the TDAdjust80ShortGI field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdadjust80shortgi - The value to set the field to.
 */
__INLINE void mdm_td_adjust_80_short_gi_setf(uint8_t tdadjust80shortgi)
{
    ASSERT_ERR((((uint32_t)tdadjust80shortgi << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0xF0000000)) | ((uint32_t)tdadjust80shortgi << 28));
}

/**
 * @brief Returns the current value of the TDAdjust40ShortGI field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the TDAdjust40ShortGI field's value will be returned.
 *
 * @return The current value of the TDAdjust40ShortGI field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_td_adjust_40_short_gi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the TDAdjust40ShortGI field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdadjust40shortgi - The value to set the field to.
 */
__INLINE void mdm_td_adjust_40_short_gi_setf(uint8_t tdadjust40shortgi)
{
    ASSERT_ERR((((uint32_t)tdadjust40shortgi << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)tdadjust40shortgi << 24));
}

/**
 * @brief Returns the current value of the TDAdjust20ShortGI field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the TDAdjust20ShortGI field's value will be returned.
 *
 * @return The current value of the TDAdjust20ShortGI field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_td_adjust_20_short_gi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00F00000)) >> 20);
}

/**
 * @brief Sets the TDAdjust20ShortGI field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdadjust20shortgi - The value to set the field to.
 */
__INLINE void mdm_td_adjust_20_short_gi_setf(uint8_t tdadjust20shortgi)
{
    ASSERT_ERR((((uint32_t)tdadjust20shortgi << 20) & ~((uint32_t)0x00F00000)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x00F00000)) | ((uint32_t)tdadjust20shortgi << 20));
}

/**
 * @brief Returns the current value of the PeakSearchDelta field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the PeakSearchDelta field's value will be returned.
 *
 * @return The current value of the PeakSearchDelta field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_peak_search_delta_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x0001F000)) >> 12);
}

/**
 * @brief Sets the PeakSearchDelta field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchdelta - The value to set the field to.
 */
__INLINE void mdm_peak_search_delta_setf(uint8_t peaksearchdelta)
{
    ASSERT_ERR((((uint32_t)peaksearchdelta << 12) & ~((uint32_t)0x0001F000)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x0001F000)) | ((uint32_t)peaksearchdelta << 12));
}

/**
 * @brief Returns the current value of the PeakSearchNdlEn field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the PeakSearchNdlEn field's value will be returned.
 *
 * @return The current value of the PeakSearchNdlEn field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_peak_search_ndl_en_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

/**
 * @brief Sets the PeakSearchNdlEn field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchndlen - The value to set the field to.
 */
__INLINE void mdm_peak_search_ndl_en_setf(uint8_t peaksearchndlen)
{
    ASSERT_ERR((((uint32_t)peaksearchndlen << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)peaksearchndlen << 11));
}

/**
 * @brief Returns the current value of the PeakSearchNdlIndex field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the PeakSearchNdlIndex field's value will be returned.
 *
 * @return The current value of the PeakSearchNdlIndex field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_peak_search_ndl_index_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00000700)) >> 8);
}

/**
 * @brief Sets the PeakSearchNdlIndex field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] peaksearchndlindex - The value to set the field to.
 */
__INLINE void mdm_peak_search_ndl_index_setf(uint8_t peaksearchndlindex)
{
    ASSERT_ERR((((uint32_t)peaksearchndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x00000700)) | ((uint32_t)peaksearchndlindex << 8));
}

/**
 * @brief Returns the current value of the TBEBias field in the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read and the TBEBias field's value will be returned.
 *
 * @return The current value of the TBEBias field in the TBECTRL2 register.
 */
__INLINE uint8_t mdm_tbe_bias_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBECTRL2_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TBEBias field of the TBECTRL2 register.
 *
 * The TBECTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbebias - The value to set the field to.
 */
__INLINE void mdm_tbe_bias_setf(uint8_t tbebias)
{
    ASSERT_ERR((((uint32_t)tbebias << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TBECTRL2_ADDR, (REG_PL_RD(MDM_TBECTRL2_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)tbebias << 0));
}

/// @}

/**
 * @name TDFOCTRL0 register definitions
 * <table>
 * <caption id="TDFOCTRL0_BF">TDFOCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>21:12 <td>AutoCorrCompareRatioHighSNR <td>R <td>R/W <td>0x200
 * <tr><td>09:00 <td>AutoCorrCompareRatioLowSNR <td>R <td>R/W <td>0x19A
 * </table>
 *
 * @{
 */

/// Address of the TDFOCTRL0 register
#define MDM_TDFOCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0864)
/// Offset of the TDFOCTRL0 register from the base address
#define MDM_TDFOCTRL0_OFFSET 0x00000064
/// Index of the TDFOCTRL0 register
#define MDM_TDFOCTRL0_INDEX  0x00000019
/// Reset value of the TDFOCTRL0 register
#define MDM_TDFOCTRL0_RESET  0x0020019A

/**
 * @brief Returns the current value of the TDFOCTRL0 register.
 * The TDFOCTRL0 register will be read and its value returned.
 * @return The current value of the TDFOCTRL0 register.
 */
__INLINE uint32_t mdm_tdfoctrl0_get(void)
{
    return REG_PL_RD(MDM_TDFOCTRL0_ADDR);
}

/**
 * @brief Sets the TDFOCTRL0 register to a value.
 * The TDFOCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tdfoctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_TDFOCTRL0_ADDR, value);
}

// field definitions
/// AUTO_CORR_COMPARE_RATIO_HIGH_SNR field mask
#define MDM_AUTO_CORR_COMPARE_RATIO_HIGH_SNR_MASK   ((uint32_t)0x003FF000)
/// AUTO_CORR_COMPARE_RATIO_HIGH_SNR field LSB position
#define MDM_AUTO_CORR_COMPARE_RATIO_HIGH_SNR_LSB    12
/// AUTO_CORR_COMPARE_RATIO_HIGH_SNR field width
#define MDM_AUTO_CORR_COMPARE_RATIO_HIGH_SNR_WIDTH  ((uint32_t)0x0000000A)
/// AUTO_CORR_COMPARE_RATIO_LOW_SNR field mask
#define MDM_AUTO_CORR_COMPARE_RATIO_LOW_SNR_MASK    ((uint32_t)0x000003FF)
/// AUTO_CORR_COMPARE_RATIO_LOW_SNR field LSB position
#define MDM_AUTO_CORR_COMPARE_RATIO_LOW_SNR_LSB     0
/// AUTO_CORR_COMPARE_RATIO_LOW_SNR field width
#define MDM_AUTO_CORR_COMPARE_RATIO_LOW_SNR_WIDTH   ((uint32_t)0x0000000A)

/// AUTO_CORR_COMPARE_RATIO_HIGH_SNR field reset value
#define MDM_AUTO_CORR_COMPARE_RATIO_HIGH_SNR_RST    0x200
/// AUTO_CORR_COMPARE_RATIO_LOW_SNR field reset value
#define MDM_AUTO_CORR_COMPARE_RATIO_LOW_SNR_RST     0x19A

/**
 * @brief Constructs a value for the TDFOCTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] autocorrcompareratiohighsnr - The value to use for the AutoCorrCompareRatioHighSNR field.
 * @param[in] autocorrcompareratiolowsnr - The value to use for the AutoCorrCompareRatioLowSNR field.
 */
__INLINE void mdm_tdfoctrl0_pack(uint16_t autocorrcompareratiohighsnr, uint16_t autocorrcompareratiolowsnr)
{
    ASSERT_ERR((((uint32_t)autocorrcompareratiohighsnr << 12) & ~((uint32_t)0x003FF000)) == 0);
    ASSERT_ERR((((uint32_t)autocorrcompareratiolowsnr << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL0_ADDR,  ((uint32_t)autocorrcompareratiohighsnr << 12) | ((uint32_t)autocorrcompareratiolowsnr << 0));
}

/**
 * @brief Unpacks TDFOCTRL0's fields from current value of the TDFOCTRL0 register.
 *
 * Reads the TDFOCTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] autocorrcompareratiohighsnr - Will be populated with the current value of this field from the register.
 * @param[out] autocorrcompareratiolowsnr - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tdfoctrl0_unpack(uint16_t* autocorrcompareratiohighsnr, uint16_t* autocorrcompareratiolowsnr)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL0_ADDR);

    *autocorrcompareratiohighsnr = (localVal & ((uint32_t)0x003FF000)) >> 12;
    *autocorrcompareratiolowsnr = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the AutoCorrCompareRatioHighSNR field in the TDFOCTRL0 register.
 *
 * The TDFOCTRL0 register will be read and the AutoCorrCompareRatioHighSNR field's value will be returned.
 *
 * @return The current value of the AutoCorrCompareRatioHighSNR field in the TDFOCTRL0 register.
 */
__INLINE uint16_t mdm_auto_corr_compare_ratio_high_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x003FF000)) >> 12);
}

/**
 * @brief Sets the AutoCorrCompareRatioHighSNR field of the TDFOCTRL0 register.
 *
 * The TDFOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrcompareratiohighsnr - The value to set the field to.
 */
__INLINE void mdm_auto_corr_compare_ratio_high_snr_setf(uint16_t autocorrcompareratiohighsnr)
{
    ASSERT_ERR((((uint32_t)autocorrcompareratiohighsnr << 12) & ~((uint32_t)0x003FF000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL0_ADDR, (REG_PL_RD(MDM_TDFOCTRL0_ADDR) & ~((uint32_t)0x003FF000)) | ((uint32_t)autocorrcompareratiohighsnr << 12));
}

/**
 * @brief Returns the current value of the AutoCorrCompareRatioLowSNR field in the TDFOCTRL0 register.
 *
 * The TDFOCTRL0 register will be read and the AutoCorrCompareRatioLowSNR field's value will be returned.
 *
 * @return The current value of the AutoCorrCompareRatioLowSNR field in the TDFOCTRL0 register.
 */
__INLINE uint16_t mdm_auto_corr_compare_ratio_low_snr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the AutoCorrCompareRatioLowSNR field of the TDFOCTRL0 register.
 *
 * The TDFOCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrcompareratiolowsnr - The value to set the field to.
 */
__INLINE void mdm_auto_corr_compare_ratio_low_snr_setf(uint16_t autocorrcompareratiolowsnr)
{
    ASSERT_ERR((((uint32_t)autocorrcompareratiolowsnr << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL0_ADDR, (REG_PL_RD(MDM_TDFOCTRL0_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)autocorrcompareratiolowsnr << 0));
}

/// @}

/**
 * @name TDFOCTRL1 register definitions
 * <table>
 * <caption id="TDFOCTRL1_BF">TDFOCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>20:12 <td>AutoCorrPlatSumEnd <td>R <td>R/W <td>0x42
 * <tr><td>08:00 <td>AutoCorrPlatSumStart <td>R <td>R/W <td>0x32
 * </table>
 *
 * @{
 */

/// Address of the TDFOCTRL1 register
#define MDM_TDFOCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0868)
/// Offset of the TDFOCTRL1 register from the base address
#define MDM_TDFOCTRL1_OFFSET 0x00000068
/// Index of the TDFOCTRL1 register
#define MDM_TDFOCTRL1_INDEX  0x0000001A
/// Reset value of the TDFOCTRL1 register
#define MDM_TDFOCTRL1_RESET  0x00042032

/**
 * @brief Returns the current value of the TDFOCTRL1 register.
 * The TDFOCTRL1 register will be read and its value returned.
 * @return The current value of the TDFOCTRL1 register.
 */
__INLINE uint32_t mdm_tdfoctrl1_get(void)
{
    return REG_PL_RD(MDM_TDFOCTRL1_ADDR);
}

/**
 * @brief Sets the TDFOCTRL1 register to a value.
 * The TDFOCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tdfoctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_TDFOCTRL1_ADDR, value);
}

// field definitions
/// AUTO_CORR_PLAT_SUM_END field mask
#define MDM_AUTO_CORR_PLAT_SUM_END_MASK     ((uint32_t)0x001FF000)
/// AUTO_CORR_PLAT_SUM_END field LSB position
#define MDM_AUTO_CORR_PLAT_SUM_END_LSB      12
/// AUTO_CORR_PLAT_SUM_END field width
#define MDM_AUTO_CORR_PLAT_SUM_END_WIDTH    ((uint32_t)0x00000009)
/// AUTO_CORR_PLAT_SUM_START field mask
#define MDM_AUTO_CORR_PLAT_SUM_START_MASK   ((uint32_t)0x000001FF)
/// AUTO_CORR_PLAT_SUM_START field LSB position
#define MDM_AUTO_CORR_PLAT_SUM_START_LSB    0
/// AUTO_CORR_PLAT_SUM_START field width
#define MDM_AUTO_CORR_PLAT_SUM_START_WIDTH  ((uint32_t)0x00000009)

/// AUTO_CORR_PLAT_SUM_END field reset value
#define MDM_AUTO_CORR_PLAT_SUM_END_RST      0x42
/// AUTO_CORR_PLAT_SUM_START field reset value
#define MDM_AUTO_CORR_PLAT_SUM_START_RST    0x32

/**
 * @brief Constructs a value for the TDFOCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] autocorrplatsumend - The value to use for the AutoCorrPlatSumEnd field.
 * @param[in] autocorrplatsumstart - The value to use for the AutoCorrPlatSumStart field.
 */
__INLINE void mdm_tdfoctrl1_pack(uint16_t autocorrplatsumend, uint16_t autocorrplatsumstart)
{
    ASSERT_ERR((((uint32_t)autocorrplatsumend << 12) & ~((uint32_t)0x001FF000)) == 0);
    ASSERT_ERR((((uint32_t)autocorrplatsumstart << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL1_ADDR,  ((uint32_t)autocorrplatsumend << 12) | ((uint32_t)autocorrplatsumstart << 0));
}

/**
 * @brief Unpacks TDFOCTRL1's fields from current value of the TDFOCTRL1 register.
 *
 * Reads the TDFOCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] autocorrplatsumend - Will be populated with the current value of this field from the register.
 * @param[out] autocorrplatsumstart - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tdfoctrl1_unpack(uint16_t* autocorrplatsumend, uint16_t* autocorrplatsumstart)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL1_ADDR);

    *autocorrplatsumend = (localVal & ((uint32_t)0x001FF000)) >> 12;
    *autocorrplatsumstart = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the AutoCorrPlatSumEnd field in the TDFOCTRL1 register.
 *
 * The TDFOCTRL1 register will be read and the AutoCorrPlatSumEnd field's value will be returned.
 *
 * @return The current value of the AutoCorrPlatSumEnd field in the TDFOCTRL1 register.
 */
__INLINE uint16_t mdm_auto_corr_plat_sum_end_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x001FF000)) >> 12);
}

/**
 * @brief Sets the AutoCorrPlatSumEnd field of the TDFOCTRL1 register.
 *
 * The TDFOCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrplatsumend - The value to set the field to.
 */
__INLINE void mdm_auto_corr_plat_sum_end_setf(uint16_t autocorrplatsumend)
{
    ASSERT_ERR((((uint32_t)autocorrplatsumend << 12) & ~((uint32_t)0x001FF000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL1_ADDR, (REG_PL_RD(MDM_TDFOCTRL1_ADDR) & ~((uint32_t)0x001FF000)) | ((uint32_t)autocorrplatsumend << 12));
}

/**
 * @brief Returns the current value of the AutoCorrPlatSumStart field in the TDFOCTRL1 register.
 *
 * The TDFOCTRL1 register will be read and the AutoCorrPlatSumStart field's value will be returned.
 *
 * @return The current value of the AutoCorrPlatSumStart field in the TDFOCTRL1 register.
 */
__INLINE uint16_t mdm_auto_corr_plat_sum_start_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the AutoCorrPlatSumStart field of the TDFOCTRL1 register.
 *
 * The TDFOCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrplatsumstart - The value to set the field to.
 */
__INLINE void mdm_auto_corr_plat_sum_start_setf(uint16_t autocorrplatsumstart)
{
    ASSERT_ERR((((uint32_t)autocorrplatsumstart << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL1_ADDR, (REG_PL_RD(MDM_TDFOCTRL1_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)autocorrplatsumstart << 0));
}

/// @}

/**
 * @name TDFOCTRL2 register definitions
 * <table>
 * <caption id="TDFOCTRL2_BF">TDFOCTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>28:20 <td>  AutoCorrPhSumEnd <td>R <td>R/W <td>0x4C
 * <tr><td>18:10 <td>AutoCorrPhSumStart <td>R <td>R/W <td>0x1C
 * <tr><td>08:00 <td>AutoCorrPlatFallSearchStart <td>R <td>R/W <td>0x44
 * </table>
 *
 * @{
 */

/// Address of the TDFOCTRL2 register
#define MDM_TDFOCTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x086C)
/// Offset of the TDFOCTRL2 register from the base address
#define MDM_TDFOCTRL2_OFFSET 0x0000006C
/// Index of the TDFOCTRL2 register
#define MDM_TDFOCTRL2_INDEX  0x0000001B
/// Reset value of the TDFOCTRL2 register
#define MDM_TDFOCTRL2_RESET  0x04C07044

/**
 * @brief Returns the current value of the TDFOCTRL2 register.
 * The TDFOCTRL2 register will be read and its value returned.
 * @return The current value of the TDFOCTRL2 register.
 */
__INLINE uint32_t mdm_tdfoctrl2_get(void)
{
    return REG_PL_RD(MDM_TDFOCTRL2_ADDR);
}

/**
 * @brief Sets the TDFOCTRL2 register to a value.
 * The TDFOCTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tdfoctrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_TDFOCTRL2_ADDR, value);
}

// field definitions
/// AUTO_CORR_PH_SUM_END field mask
#define MDM_AUTO_CORR_PH_SUM_END_MASK               ((uint32_t)0x1FF00000)
/// AUTO_CORR_PH_SUM_END field LSB position
#define MDM_AUTO_CORR_PH_SUM_END_LSB                20
/// AUTO_CORR_PH_SUM_END field width
#define MDM_AUTO_CORR_PH_SUM_END_WIDTH              ((uint32_t)0x00000009)
/// AUTO_CORR_PH_SUM_START field mask
#define MDM_AUTO_CORR_PH_SUM_START_MASK             ((uint32_t)0x0007FC00)
/// AUTO_CORR_PH_SUM_START field LSB position
#define MDM_AUTO_CORR_PH_SUM_START_LSB              10
/// AUTO_CORR_PH_SUM_START field width
#define MDM_AUTO_CORR_PH_SUM_START_WIDTH            ((uint32_t)0x00000009)
/// AUTO_CORR_PLAT_FALL_SEARCH_START field mask
#define MDM_AUTO_CORR_PLAT_FALL_SEARCH_START_MASK   ((uint32_t)0x000001FF)
/// AUTO_CORR_PLAT_FALL_SEARCH_START field LSB position
#define MDM_AUTO_CORR_PLAT_FALL_SEARCH_START_LSB    0
/// AUTO_CORR_PLAT_FALL_SEARCH_START field width
#define MDM_AUTO_CORR_PLAT_FALL_SEARCH_START_WIDTH  ((uint32_t)0x00000009)

/// AUTO_CORR_PH_SUM_END field reset value
#define MDM_AUTO_CORR_PH_SUM_END_RST                0x4C
/// AUTO_CORR_PH_SUM_START field reset value
#define MDM_AUTO_CORR_PH_SUM_START_RST              0x1C
/// AUTO_CORR_PLAT_FALL_SEARCH_START field reset value
#define MDM_AUTO_CORR_PLAT_FALL_SEARCH_START_RST    0x44

/**
 * @brief Constructs a value for the TDFOCTRL2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] autocorrphsumend - The value to use for the AutoCorrPhSumEnd field.
 * @param[in] autocorrphsumstart - The value to use for the AutoCorrPhSumStart field.
 * @param[in] autocorrplatfallsearchstart - The value to use for the AutoCorrPlatFallSearchStart field.
 */
__INLINE void mdm_tdfoctrl2_pack(uint16_t autocorrphsumend, uint16_t autocorrphsumstart, uint16_t autocorrplatfallsearchstart)
{
    ASSERT_ERR((((uint32_t)autocorrphsumend << 20) & ~((uint32_t)0x1FF00000)) == 0);
    ASSERT_ERR((((uint32_t)autocorrphsumstart << 10) & ~((uint32_t)0x0007FC00)) == 0);
    ASSERT_ERR((((uint32_t)autocorrplatfallsearchstart << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL2_ADDR,  ((uint32_t)autocorrphsumend << 20) | ((uint32_t)autocorrphsumstart << 10) | ((uint32_t)autocorrplatfallsearchstart << 0));
}

/**
 * @brief Unpacks TDFOCTRL2's fields from current value of the TDFOCTRL2 register.
 *
 * Reads the TDFOCTRL2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] autocorrphsumend - Will be populated with the current value of this field from the register.
 * @param[out] autocorrphsumstart - Will be populated with the current value of this field from the register.
 * @param[out] autocorrplatfallsearchstart - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tdfoctrl2_unpack(uint16_t* autocorrphsumend, uint16_t* autocorrphsumstart, uint16_t* autocorrplatfallsearchstart)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL2_ADDR);

    *autocorrphsumend = (localVal & ((uint32_t)0x1FF00000)) >> 20;
    *autocorrphsumstart = (localVal & ((uint32_t)0x0007FC00)) >> 10;
    *autocorrplatfallsearchstart = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the AutoCorrPhSumEnd field in the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read and the AutoCorrPhSumEnd field's value will be returned.
 *
 * @return The current value of the AutoCorrPhSumEnd field in the TDFOCTRL2 register.
 */
__INLINE uint16_t mdm_auto_corr_ph_sum_end_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x1FF00000)) >> 20);
}

/**
 * @brief Sets the AutoCorrPhSumEnd field of the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrphsumend - The value to set the field to.
 */
__INLINE void mdm_auto_corr_ph_sum_end_setf(uint16_t autocorrphsumend)
{
    ASSERT_ERR((((uint32_t)autocorrphsumend << 20) & ~((uint32_t)0x1FF00000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL2_ADDR, (REG_PL_RD(MDM_TDFOCTRL2_ADDR) & ~((uint32_t)0x1FF00000)) | ((uint32_t)autocorrphsumend << 20));
}

/**
 * @brief Returns the current value of the AutoCorrPhSumStart field in the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read and the AutoCorrPhSumStart field's value will be returned.
 *
 * @return The current value of the AutoCorrPhSumStart field in the TDFOCTRL2 register.
 */
__INLINE uint16_t mdm_auto_corr_ph_sum_start_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x0007FC00)) >> 10);
}

/**
 * @brief Sets the AutoCorrPhSumStart field of the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrphsumstart - The value to set the field to.
 */
__INLINE void mdm_auto_corr_ph_sum_start_setf(uint16_t autocorrphsumstart)
{
    ASSERT_ERR((((uint32_t)autocorrphsumstart << 10) & ~((uint32_t)0x0007FC00)) == 0);
    REG_PL_WR(MDM_TDFOCTRL2_ADDR, (REG_PL_RD(MDM_TDFOCTRL2_ADDR) & ~((uint32_t)0x0007FC00)) | ((uint32_t)autocorrphsumstart << 10));
}

/**
 * @brief Returns the current value of the AutoCorrPlatFallSearchStart field in the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read and the AutoCorrPlatFallSearchStart field's value will be returned.
 *
 * @return The current value of the AutoCorrPlatFallSearchStart field in the TDFOCTRL2 register.
 */
__INLINE uint16_t mdm_auto_corr_plat_fall_search_start_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the AutoCorrPlatFallSearchStart field of the TDFOCTRL2 register.
 *
 * The TDFOCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrplatfallsearchstart - The value to set the field to.
 */
__INLINE void mdm_auto_corr_plat_fall_search_start_setf(uint16_t autocorrplatfallsearchstart)
{
    ASSERT_ERR((((uint32_t)autocorrplatfallsearchstart << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(MDM_TDFOCTRL2_ADDR, (REG_PL_RD(MDM_TDFOCTRL2_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)autocorrplatfallsearchstart << 0));
}

/// @}

/**
 * @name TDFOCTRL3 register definitions
 * <table>
 * <caption id="TDFOCTRL3_BF">TDFOCTRL3 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31 <td>        TDFOCOMPEN <td>R <td>R/W <td>1
 * <tr><td>27 <td>     TDFOFINEESTEN <td>R <td>R/W <td>1
 * <tr><td>23:12 <td>TDFOPlatFallTimeOut <td>R <td>R/W <td>0x1F4
 * <tr><td>11:04 <td>  TDFOSNRThreshold <td>R <td>R/W <td>0xC
 * <tr><td>03:00 <td> AutoCorrFallCount <td>R <td>R/W <td>0x1
 * </table>
 *
 * @{
 */

/// Address of the TDFOCTRL3 register
#define MDM_TDFOCTRL3_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0870)
/// Offset of the TDFOCTRL3 register from the base address
#define MDM_TDFOCTRL3_OFFSET 0x00000070
/// Index of the TDFOCTRL3 register
#define MDM_TDFOCTRL3_INDEX  0x0000001C
/// Reset value of the TDFOCTRL3 register
#define MDM_TDFOCTRL3_RESET  0x881F40C1

/**
 * @brief Returns the current value of the TDFOCTRL3 register.
 * The TDFOCTRL3 register will be read and its value returned.
 * @return The current value of the TDFOCTRL3 register.
 */
__INLINE uint32_t mdm_tdfoctrl3_get(void)
{
    return REG_PL_RD(MDM_TDFOCTRL3_ADDR);
}

/**
 * @brief Sets the TDFOCTRL3 register to a value.
 * The TDFOCTRL3 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_tdfoctrl3_set(uint32_t value)
{
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, value);
}

// field definitions
/// TDFOCOMPEN field bit
#define MDM_TDFOCOMPEN_BIT                 ((uint32_t)0x80000000)
/// TDFOCOMPEN field position
#define MDM_TDFOCOMPEN_POS                 31
/// TDFOFINEESTEN field bit
#define MDM_TDFOFINEESTEN_BIT              ((uint32_t)0x08000000)
/// TDFOFINEESTEN field position
#define MDM_TDFOFINEESTEN_POS              27
/// TDFO_PLAT_FALL_TIME_OUT field mask
#define MDM_TDFO_PLAT_FALL_TIME_OUT_MASK   ((uint32_t)0x00FFF000)
/// TDFO_PLAT_FALL_TIME_OUT field LSB position
#define MDM_TDFO_PLAT_FALL_TIME_OUT_LSB    12
/// TDFO_PLAT_FALL_TIME_OUT field width
#define MDM_TDFO_PLAT_FALL_TIME_OUT_WIDTH  ((uint32_t)0x0000000C)
/// TDFOSNR_THRESHOLD field mask
#define MDM_TDFOSNR_THRESHOLD_MASK         ((uint32_t)0x00000FF0)
/// TDFOSNR_THRESHOLD field LSB position
#define MDM_TDFOSNR_THRESHOLD_LSB          4
/// TDFOSNR_THRESHOLD field width
#define MDM_TDFOSNR_THRESHOLD_WIDTH        ((uint32_t)0x00000008)
/// AUTO_CORR_FALL_COUNT field mask
#define MDM_AUTO_CORR_FALL_COUNT_MASK      ((uint32_t)0x0000000F)
/// AUTO_CORR_FALL_COUNT field LSB position
#define MDM_AUTO_CORR_FALL_COUNT_LSB       0
/// AUTO_CORR_FALL_COUNT field width
#define MDM_AUTO_CORR_FALL_COUNT_WIDTH     ((uint32_t)0x00000004)

/// TDFOCOMPEN field reset value
#define MDM_TDFOCOMPEN_RST                 0x1
/// TDFOFINEESTEN field reset value
#define MDM_TDFOFINEESTEN_RST              0x1
/// TDFO_PLAT_FALL_TIME_OUT field reset value
#define MDM_TDFO_PLAT_FALL_TIME_OUT_RST    0x1F4
/// TDFOSNR_THRESHOLD field reset value
#define MDM_TDFOSNR_THRESHOLD_RST          0xC
/// AUTO_CORR_FALL_COUNT field reset value
#define MDM_AUTO_CORR_FALL_COUNT_RST       0x1

/**
 * @brief Constructs a value for the TDFOCTRL3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tdfocompen - The value to use for the TDFOCOMPEN field.
 * @param[in] tdfofineesten - The value to use for the TDFOFINEESTEN field.
 * @param[in] tdfoplatfalltimeout - The value to use for the TDFOPlatFallTimeOut field.
 * @param[in] tdfosnrthreshold - The value to use for the TDFOSNRThreshold field.
 * @param[in] autocorrfallcount - The value to use for the AutoCorrFallCount field.
 */
__INLINE void mdm_tdfoctrl3_pack(uint8_t tdfocompen, uint8_t tdfofineesten, uint16_t tdfoplatfalltimeout, uint8_t tdfosnrthreshold, uint8_t autocorrfallcount)
{
    ASSERT_ERR((((uint32_t)tdfocompen << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)tdfofineesten << 27) & ~((uint32_t)0x08000000)) == 0);
    ASSERT_ERR((((uint32_t)tdfoplatfalltimeout << 12) & ~((uint32_t)0x00FFF000)) == 0);
    ASSERT_ERR((((uint32_t)tdfosnrthreshold << 4) & ~((uint32_t)0x00000FF0)) == 0);
    ASSERT_ERR((((uint32_t)autocorrfallcount << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR,  ((uint32_t)tdfocompen << 31) | ((uint32_t)tdfofineesten << 27) | ((uint32_t)tdfoplatfalltimeout << 12) | ((uint32_t)tdfosnrthreshold << 4) | ((uint32_t)autocorrfallcount << 0));
}

/**
 * @brief Unpacks TDFOCTRL3's fields from current value of the TDFOCTRL3 register.
 *
 * Reads the TDFOCTRL3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tdfocompen - Will be populated with the current value of this field from the register.
 * @param[out] tdfofineesten - Will be populated with the current value of this field from the register.
 * @param[out] tdfoplatfalltimeout - Will be populated with the current value of this field from the register.
 * @param[out] tdfosnrthreshold - Will be populated with the current value of this field from the register.
 * @param[out] autocorrfallcount - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tdfoctrl3_unpack(uint8_t* tdfocompen, uint8_t* tdfofineesten, uint16_t* tdfoplatfalltimeout, uint8_t* tdfosnrthreshold, uint8_t* autocorrfallcount)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);

    *tdfocompen = (localVal & ((uint32_t)0x80000000)) >> 31;
    *tdfofineesten = (localVal & ((uint32_t)0x08000000)) >> 27;
    *tdfoplatfalltimeout = (localVal & ((uint32_t)0x00FFF000)) >> 12;
    *tdfosnrthreshold = (localVal & ((uint32_t)0x00000FF0)) >> 4;
    *autocorrfallcount = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the TDFOCOMPEN field in the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read and the TDFOCOMPEN field's value will be returned.
 *
 * @return The current value of the TDFOCOMPEN field in the TDFOCTRL3 register.
 */
__INLINE uint8_t mdm_tdfocompen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the TDFOCOMPEN field of the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdfocompen - The value to set the field to.
 */
__INLINE void mdm_tdfocompen_setf(uint8_t tdfocompen)
{
    ASSERT_ERR((((uint32_t)tdfocompen << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, (REG_PL_RD(MDM_TDFOCTRL3_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)tdfocompen << 31));
}

/**
 * @brief Returns the current value of the TDFOFINEESTEN field in the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read and the TDFOFINEESTEN field's value will be returned.
 *
 * @return The current value of the TDFOFINEESTEN field in the TDFOCTRL3 register.
 */
__INLINE uint8_t mdm_tdfofineesten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

/**
 * @brief Sets the TDFOFINEESTEN field of the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdfofineesten - The value to set the field to.
 */
__INLINE void mdm_tdfofineesten_setf(uint8_t tdfofineesten)
{
    ASSERT_ERR((((uint32_t)tdfofineesten << 27) & ~((uint32_t)0x08000000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, (REG_PL_RD(MDM_TDFOCTRL3_ADDR) & ~((uint32_t)0x08000000)) | ((uint32_t)tdfofineesten << 27));
}

/**
 * @brief Returns the current value of the TDFOPlatFallTimeOut field in the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read and the TDFOPlatFallTimeOut field's value will be returned.
 *
 * @return The current value of the TDFOPlatFallTimeOut field in the TDFOCTRL3 register.
 */
__INLINE uint16_t mdm_tdfo_plat_fall_time_out_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x00FFF000)) >> 12);
}

/**
 * @brief Sets the TDFOPlatFallTimeOut field of the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdfoplatfalltimeout - The value to set the field to.
 */
__INLINE void mdm_tdfo_plat_fall_time_out_setf(uint16_t tdfoplatfalltimeout)
{
    ASSERT_ERR((((uint32_t)tdfoplatfalltimeout << 12) & ~((uint32_t)0x00FFF000)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, (REG_PL_RD(MDM_TDFOCTRL3_ADDR) & ~((uint32_t)0x00FFF000)) | ((uint32_t)tdfoplatfalltimeout << 12));
}

/**
 * @brief Returns the current value of the TDFOSNRThreshold field in the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read and the TDFOSNRThreshold field's value will be returned.
 *
 * @return The current value of the TDFOSNRThreshold field in the TDFOCTRL3 register.
 */
__INLINE uint8_t mdm_tdfosnr_threshold_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x00000FF0)) >> 4);
}

/**
 * @brief Sets the TDFOSNRThreshold field of the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdfosnrthreshold - The value to set the field to.
 */
__INLINE void mdm_tdfosnr_threshold_setf(uint8_t tdfosnrthreshold)
{
    ASSERT_ERR((((uint32_t)tdfosnrthreshold << 4) & ~((uint32_t)0x00000FF0)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, (REG_PL_RD(MDM_TDFOCTRL3_ADDR) & ~((uint32_t)0x00000FF0)) | ((uint32_t)tdfosnrthreshold << 4));
}

/**
 * @brief Returns the current value of the AutoCorrFallCount field in the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read and the AutoCorrFallCount field's value will be returned.
 *
 * @return The current value of the AutoCorrFallCount field in the TDFOCTRL3 register.
 */
__INLINE uint8_t mdm_auto_corr_fall_count_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDFOCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the AutoCorrFallCount field of the TDFOCTRL3 register.
 *
 * The TDFOCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] autocorrfallcount - The value to set the field to.
 */
__INLINE void mdm_auto_corr_fall_count_setf(uint8_t autocorrfallcount)
{
    ASSERT_ERR((((uint32_t)autocorrfallcount << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_TDFOCTRL3_ADDR, (REG_PL_RD(MDM_TDFOCTRL3_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)autocorrfallcount << 0));
}

/// @}

/**
 * @name CLKGATEFCTRL0 register definitions
 * <table>
 * <caption id="CLKGATEFCTRL0_BF">CLKGATEFCTRL0 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31 <td>     PHYTXCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>30 <td>      BDTXCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>29 <td>    AGCMEMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>28 <td>       AGCCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>27 <td>        RCCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>24 <td>        FECLKFORCE <td>R <td>R/W <td>0
 * <tr><td>23 <td>       FDOCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>22 <td>       EQUCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>21 <td>    PHYSVDCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>20 <td>   PHYTXBFCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>18 <td>    TDCOMPCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>17 <td>   TDFOESTCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>16 <td>       TBECLKFORCE <td>R <td>R/W <td>0
 * <tr><td>10 <td>   FFT1MEMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>09 <td>      FFT1CLKFORCE <td>R <td>R/W <td>0
 * <tr><td>08 <td>   FFT0MEMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>07 <td>      FFT0CLKFORCE <td>R <td>R/W <td>0
 * <tr><td>06 <td>     CHESTCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>04 <td>      HMEMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>03 <td>  INTLVMEMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>02 <td>      VTB1CLKFORCE <td>R <td>R/W <td>0
 * <tr><td>01 <td>      VTB0CLKFORCE <td>R <td>R/W <td>0
 * <tr><td>00 <td>      BDRXCLKFORCE <td>R <td>R/W <td>0
 * </table>
 *
 * @{
 */

/// Address of the CLKGATEFCTRL0 register
#define MDM_CLKGATEFCTRL0_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0874)
/// Offset of the CLKGATEFCTRL0 register from the base address
#define MDM_CLKGATEFCTRL0_OFFSET 0x00000074
/// Index of the CLKGATEFCTRL0 register
#define MDM_CLKGATEFCTRL0_INDEX  0x0000001D
/// Reset value of the CLKGATEFCTRL0 register
#define MDM_CLKGATEFCTRL0_RESET  0x00000000

/**
 * @brief Returns the current value of the CLKGATEFCTRL0 register.
 * The CLKGATEFCTRL0 register will be read and its value returned.
 * @return The current value of the CLKGATEFCTRL0 register.
 */
__INLINE uint32_t mdm_clkgatefctrl0_get(void)
{
    return REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
}

/**
 * @brief Sets the CLKGATEFCTRL0 register to a value.
 * The CLKGATEFCTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_clkgatefctrl0_set(uint32_t value)
{
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, value);
}

// field definitions
/// PHYTXCLKFORCE field bit
#define MDM_PHYTXCLKFORCE_BIT       ((uint32_t)0x80000000)
/// PHYTXCLKFORCE field position
#define MDM_PHYTXCLKFORCE_POS       31
/// BDTXCLKFORCE field bit
#define MDM_BDTXCLKFORCE_BIT        ((uint32_t)0x40000000)
/// BDTXCLKFORCE field position
#define MDM_BDTXCLKFORCE_POS        30
/// AGCMEMCLKFORCE field bit
#define MDM_AGCMEMCLKFORCE_BIT      ((uint32_t)0x20000000)
/// AGCMEMCLKFORCE field position
#define MDM_AGCMEMCLKFORCE_POS      29
/// AGCCLKFORCE field bit
#define MDM_AGCCLKFORCE_BIT         ((uint32_t)0x10000000)
/// AGCCLKFORCE field position
#define MDM_AGCCLKFORCE_POS         28
/// RCCLKFORCE field bit
#define MDM_RCCLKFORCE_BIT          ((uint32_t)0x08000000)
/// RCCLKFORCE field position
#define MDM_RCCLKFORCE_POS          27
/// FECLKFORCE field bit
#define MDM_FECLKFORCE_BIT          ((uint32_t)0x01000000)
/// FECLKFORCE field position
#define MDM_FECLKFORCE_POS          24
/// FDOCLKFORCE field bit
#define MDM_FDOCLKFORCE_BIT         ((uint32_t)0x00800000)
/// FDOCLKFORCE field position
#define MDM_FDOCLKFORCE_POS         23
/// EQUCLKFORCE field bit
#define MDM_EQUCLKFORCE_BIT         ((uint32_t)0x00400000)
/// EQUCLKFORCE field position
#define MDM_EQUCLKFORCE_POS         22
/// PHYSVDCLKFORCE field bit
#define MDM_PHYSVDCLKFORCE_BIT      ((uint32_t)0x00200000)
/// PHYSVDCLKFORCE field position
#define MDM_PHYSVDCLKFORCE_POS      21
/// PHYTXBFCLKFORCE field bit
#define MDM_PHYTXBFCLKFORCE_BIT     ((uint32_t)0x00100000)
/// PHYTXBFCLKFORCE field position
#define MDM_PHYTXBFCLKFORCE_POS     20
/// TDCOMPCLKFORCE field bit
#define MDM_TDCOMPCLKFORCE_BIT      ((uint32_t)0x00040000)
/// TDCOMPCLKFORCE field position
#define MDM_TDCOMPCLKFORCE_POS      18
/// TDFOESTCLKFORCE field bit
#define MDM_TDFOESTCLKFORCE_BIT     ((uint32_t)0x00020000)
/// TDFOESTCLKFORCE field position
#define MDM_TDFOESTCLKFORCE_POS     17
/// TBECLKFORCE field bit
#define MDM_TBECLKFORCE_BIT         ((uint32_t)0x00010000)
/// TBECLKFORCE field position
#define MDM_TBECLKFORCE_POS         16
/// FFT1MEMCLKFORCE field bit
#define MDM_FFT1MEMCLKFORCE_BIT     ((uint32_t)0x00000400)
/// FFT1MEMCLKFORCE field position
#define MDM_FFT1MEMCLKFORCE_POS     10
/// FFT1CLKFORCE field bit
#define MDM_FFT1CLKFORCE_BIT        ((uint32_t)0x00000200)
/// FFT1CLKFORCE field position
#define MDM_FFT1CLKFORCE_POS        9
/// FFT0MEMCLKFORCE field bit
#define MDM_FFT0MEMCLKFORCE_BIT     ((uint32_t)0x00000100)
/// FFT0MEMCLKFORCE field position
#define MDM_FFT0MEMCLKFORCE_POS     8
/// FFT0CLKFORCE field bit
#define MDM_FFT0CLKFORCE_BIT        ((uint32_t)0x00000080)
/// FFT0CLKFORCE field position
#define MDM_FFT0CLKFORCE_POS        7
/// CHESTCLKFORCE field bit
#define MDM_CHESTCLKFORCE_BIT       ((uint32_t)0x00000040)
/// CHESTCLKFORCE field position
#define MDM_CHESTCLKFORCE_POS       6
/// HMEMCLKFORCE field bit
#define MDM_HMEMCLKFORCE_BIT        ((uint32_t)0x00000010)
/// HMEMCLKFORCE field position
#define MDM_HMEMCLKFORCE_POS        4
/// INTLVMEMCLKFORCE field bit
#define MDM_INTLVMEMCLKFORCE_BIT    ((uint32_t)0x00000008)
/// INTLVMEMCLKFORCE field position
#define MDM_INTLVMEMCLKFORCE_POS    3
/// VTB1CLKFORCE field bit
#define MDM_VTB1CLKFORCE_BIT        ((uint32_t)0x00000004)
/// VTB1CLKFORCE field position
#define MDM_VTB1CLKFORCE_POS        2
/// VTB0CLKFORCE field bit
#define MDM_VTB0CLKFORCE_BIT        ((uint32_t)0x00000002)
/// VTB0CLKFORCE field position
#define MDM_VTB0CLKFORCE_POS        1
/// BDRXCLKFORCE field bit
#define MDM_BDRXCLKFORCE_BIT        ((uint32_t)0x00000001)
/// BDRXCLKFORCE field position
#define MDM_BDRXCLKFORCE_POS        0

/// PHYTXCLKFORCE field reset value
#define MDM_PHYTXCLKFORCE_RST       0x0
/// BDTXCLKFORCE field reset value
#define MDM_BDTXCLKFORCE_RST        0x0
/// AGCMEMCLKFORCE field reset value
#define MDM_AGCMEMCLKFORCE_RST      0x0
/// AGCCLKFORCE field reset value
#define MDM_AGCCLKFORCE_RST         0x0
/// RCCLKFORCE field reset value
#define MDM_RCCLKFORCE_RST          0x0
/// FECLKFORCE field reset value
#define MDM_FECLKFORCE_RST          0x0
/// FDOCLKFORCE field reset value
#define MDM_FDOCLKFORCE_RST         0x0
/// EQUCLKFORCE field reset value
#define MDM_EQUCLKFORCE_RST         0x0
/// PHYSVDCLKFORCE field reset value
#define MDM_PHYSVDCLKFORCE_RST      0x0
/// PHYTXBFCLKFORCE field reset value
#define MDM_PHYTXBFCLKFORCE_RST     0x0
/// TDCOMPCLKFORCE field reset value
#define MDM_TDCOMPCLKFORCE_RST      0x0
/// TDFOESTCLKFORCE field reset value
#define MDM_TDFOESTCLKFORCE_RST     0x0
/// TBECLKFORCE field reset value
#define MDM_TBECLKFORCE_RST         0x0
/// FFT1MEMCLKFORCE field reset value
#define MDM_FFT1MEMCLKFORCE_RST     0x0
/// FFT1CLKFORCE field reset value
#define MDM_FFT1CLKFORCE_RST        0x0
/// FFT0MEMCLKFORCE field reset value
#define MDM_FFT0MEMCLKFORCE_RST     0x0
/// FFT0CLKFORCE field reset value
#define MDM_FFT0CLKFORCE_RST        0x0
/// CHESTCLKFORCE field reset value
#define MDM_CHESTCLKFORCE_RST       0x0
/// HMEMCLKFORCE field reset value
#define MDM_HMEMCLKFORCE_RST        0x0
/// INTLVMEMCLKFORCE field reset value
#define MDM_INTLVMEMCLKFORCE_RST    0x0
/// VTB1CLKFORCE field reset value
#define MDM_VTB1CLKFORCE_RST        0x0
/// VTB0CLKFORCE field reset value
#define MDM_VTB0CLKFORCE_RST        0x0
/// BDRXCLKFORCE field reset value
#define MDM_BDRXCLKFORCE_RST        0x0

/**
 * @brief Constructs a value for the CLKGATEFCTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] phytxclkforce - The value to use for the PHYTXCLKFORCE field.
 * @param[in] bdtxclkforce - The value to use for the BDTXCLKFORCE field.
 * @param[in] agcmemclkforce - The value to use for the AGCMEMCLKFORCE field.
 * @param[in] agcclkforce - The value to use for the AGCCLKFORCE field.
 * @param[in] rcclkforce - The value to use for the RCCLKFORCE field.
 * @param[in] feclkforce - The value to use for the FECLKFORCE field.
 * @param[in] fdoclkforce - The value to use for the FDOCLKFORCE field.
 * @param[in] equclkforce - The value to use for the EQUCLKFORCE field.
 * @param[in] physvdclkforce - The value to use for the PHYSVDCLKFORCE field.
 * @param[in] phytxbfclkforce - The value to use for the PHYTXBFCLKFORCE field.
 * @param[in] tdcompclkforce - The value to use for the TDCOMPCLKFORCE field.
 * @param[in] tdfoestclkforce - The value to use for the TDFOESTCLKFORCE field.
 * @param[in] tbeclkforce - The value to use for the TBECLKFORCE field.
 * @param[in] fft1memclkforce - The value to use for the FFT1MEMCLKFORCE field.
 * @param[in] fft1clkforce - The value to use for the FFT1CLKFORCE field.
 * @param[in] fft0memclkforce - The value to use for the FFT0MEMCLKFORCE field.
 * @param[in] fft0clkforce - The value to use for the FFT0CLKFORCE field.
 * @param[in] chestclkforce - The value to use for the CHESTCLKFORCE field.
 * @param[in] hmemclkforce - The value to use for the HMEMCLKFORCE field.
 * @param[in] intlvmemclkforce - The value to use for the INTLVMEMCLKFORCE field.
 * @param[in] vtb1clkforce - The value to use for the VTB1CLKFORCE field.
 * @param[in] vtb0clkforce - The value to use for the VTB0CLKFORCE field.
 * @param[in] bdrxclkforce - The value to use for the BDRXCLKFORCE field.
 */
__INLINE void mdm_clkgatefctrl0_pack(uint8_t phytxclkforce, uint8_t bdtxclkforce, uint8_t agcmemclkforce, uint8_t agcclkforce, uint8_t rcclkforce, uint8_t feclkforce, uint8_t fdoclkforce, uint8_t equclkforce, uint8_t physvdclkforce, uint8_t phytxbfclkforce, uint8_t tdcompclkforce, uint8_t tdfoestclkforce, uint8_t tbeclkforce, uint8_t fft1memclkforce, uint8_t fft1clkforce, uint8_t fft0memclkforce, uint8_t fft0clkforce, uint8_t chestclkforce, uint8_t hmemclkforce, uint8_t intlvmemclkforce, uint8_t vtb1clkforce, uint8_t vtb0clkforce, uint8_t bdrxclkforce)
{
    ASSERT_ERR((((uint32_t)phytxclkforce << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)bdtxclkforce << 30) & ~((uint32_t)0x40000000)) == 0);
    ASSERT_ERR((((uint32_t)agcmemclkforce << 29) & ~((uint32_t)0x20000000)) == 0);
    ASSERT_ERR((((uint32_t)agcclkforce << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)rcclkforce << 27) & ~((uint32_t)0x08000000)) == 0);
    ASSERT_ERR((((uint32_t)feclkforce << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)fdoclkforce << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)equclkforce << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)physvdclkforce << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)phytxbfclkforce << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)tdcompclkforce << 18) & ~((uint32_t)0x00040000)) == 0);
    ASSERT_ERR((((uint32_t)tdfoestclkforce << 17) & ~((uint32_t)0x00020000)) == 0);
    ASSERT_ERR((((uint32_t)tbeclkforce << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)fft1memclkforce << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)fft1clkforce << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)fft0memclkforce << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)fft0clkforce << 7) & ~((uint32_t)0x00000080)) == 0);
    ASSERT_ERR((((uint32_t)chestclkforce << 6) & ~((uint32_t)0x00000040)) == 0);
    ASSERT_ERR((((uint32_t)hmemclkforce << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)intlvmemclkforce << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)vtb1clkforce << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)vtb0clkforce << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)bdrxclkforce << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR,  ((uint32_t)phytxclkforce << 31) | ((uint32_t)bdtxclkforce << 30) | ((uint32_t)agcmemclkforce << 29) | ((uint32_t)agcclkforce << 28) | ((uint32_t)rcclkforce << 27) | ((uint32_t)feclkforce << 24) | ((uint32_t)fdoclkforce << 23) | ((uint32_t)equclkforce << 22) | ((uint32_t)physvdclkforce << 21) | ((uint32_t)phytxbfclkforce << 20) | ((uint32_t)tdcompclkforce << 18) | ((uint32_t)tdfoestclkforce << 17) | ((uint32_t)tbeclkforce << 16) | ((uint32_t)fft1memclkforce << 10) | ((uint32_t)fft1clkforce << 9) | ((uint32_t)fft0memclkforce << 8) | ((uint32_t)fft0clkforce << 7) | ((uint32_t)chestclkforce << 6) | ((uint32_t)hmemclkforce << 4) | ((uint32_t)intlvmemclkforce << 3) | ((uint32_t)vtb1clkforce << 2) | ((uint32_t)vtb0clkforce << 1) | ((uint32_t)bdrxclkforce << 0));
}

/**
 * @brief Unpacks CLKGATEFCTRL0's fields from current value of the CLKGATEFCTRL0 register.
 *
 * Reads the CLKGATEFCTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] phytxclkforce - Will be populated with the current value of this field from the register.
 * @param[out] bdtxclkforce - Will be populated with the current value of this field from the register.
 * @param[out] agcmemclkforce - Will be populated with the current value of this field from the register.
 * @param[out] agcclkforce - Will be populated with the current value of this field from the register.
 * @param[out] rcclkforce - Will be populated with the current value of this field from the register.
 * @param[out] feclkforce - Will be populated with the current value of this field from the register.
 * @param[out] fdoclkforce - Will be populated with the current value of this field from the register.
 * @param[out] equclkforce - Will be populated with the current value of this field from the register.
 * @param[out] physvdclkforce - Will be populated with the current value of this field from the register.
 * @param[out] phytxbfclkforce - Will be populated with the current value of this field from the register.
 * @param[out] tdcompclkforce - Will be populated with the current value of this field from the register.
 * @param[out] tdfoestclkforce - Will be populated with the current value of this field from the register.
 * @param[out] tbeclkforce - Will be populated with the current value of this field from the register.
 * @param[out] fft1memclkforce - Will be populated with the current value of this field from the register.
 * @param[out] fft1clkforce - Will be populated with the current value of this field from the register.
 * @param[out] fft0memclkforce - Will be populated with the current value of this field from the register.
 * @param[out] fft0clkforce - Will be populated with the current value of this field from the register.
 * @param[out] chestclkforce - Will be populated with the current value of this field from the register.
 * @param[out] hmemclkforce - Will be populated with the current value of this field from the register.
 * @param[out] intlvmemclkforce - Will be populated with the current value of this field from the register.
 * @param[out] vtb1clkforce - Will be populated with the current value of this field from the register.
 * @param[out] vtb0clkforce - Will be populated with the current value of this field from the register.
 * @param[out] bdrxclkforce - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_clkgatefctrl0_unpack(uint8_t* phytxclkforce, uint8_t* bdtxclkforce, uint8_t* agcmemclkforce, uint8_t* agcclkforce, uint8_t* rcclkforce, uint8_t* feclkforce, uint8_t* fdoclkforce, uint8_t* equclkforce, uint8_t* physvdclkforce, uint8_t* phytxbfclkforce, uint8_t* tdcompclkforce, uint8_t* tdfoestclkforce, uint8_t* tbeclkforce, uint8_t* fft1memclkforce, uint8_t* fft1clkforce, uint8_t* fft0memclkforce, uint8_t* fft0clkforce, uint8_t* chestclkforce, uint8_t* hmemclkforce, uint8_t* intlvmemclkforce, uint8_t* vtb1clkforce, uint8_t* vtb0clkforce, uint8_t* bdrxclkforce)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);

    *phytxclkforce = (localVal & ((uint32_t)0x80000000)) >> 31;
    *bdtxclkforce = (localVal & ((uint32_t)0x40000000)) >> 30;
    *agcmemclkforce = (localVal & ((uint32_t)0x20000000)) >> 29;
    *agcclkforce = (localVal & ((uint32_t)0x10000000)) >> 28;
    *rcclkforce = (localVal & ((uint32_t)0x08000000)) >> 27;
    *feclkforce = (localVal & ((uint32_t)0x01000000)) >> 24;
    *fdoclkforce = (localVal & ((uint32_t)0x00800000)) >> 23;
    *equclkforce = (localVal & ((uint32_t)0x00400000)) >> 22;
    *physvdclkforce = (localVal & ((uint32_t)0x00200000)) >> 21;
    *phytxbfclkforce = (localVal & ((uint32_t)0x00100000)) >> 20;
    *tdcompclkforce = (localVal & ((uint32_t)0x00040000)) >> 18;
    *tdfoestclkforce = (localVal & ((uint32_t)0x00020000)) >> 17;
    *tbeclkforce = (localVal & ((uint32_t)0x00010000)) >> 16;
    *fft1memclkforce = (localVal & ((uint32_t)0x00000400)) >> 10;
    *fft1clkforce = (localVal & ((uint32_t)0x00000200)) >> 9;
    *fft0memclkforce = (localVal & ((uint32_t)0x00000100)) >> 8;
    *fft0clkforce = (localVal & ((uint32_t)0x00000080)) >> 7;
    *chestclkforce = (localVal & ((uint32_t)0x00000040)) >> 6;
    *hmemclkforce = (localVal & ((uint32_t)0x00000010)) >> 4;
    *intlvmemclkforce = (localVal & ((uint32_t)0x00000008)) >> 3;
    *vtb1clkforce = (localVal & ((uint32_t)0x00000004)) >> 2;
    *vtb0clkforce = (localVal & ((uint32_t)0x00000002)) >> 1;
    *bdrxclkforce = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the PHYTXCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the PHYTXCLKFORCE field's value will be returned.
 *
 * @return The current value of the PHYTXCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_phytxclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the PHYTXCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] phytxclkforce - The value to set the field to.
 */
__INLINE void mdm_phytxclkforce_setf(uint8_t phytxclkforce)
{
    ASSERT_ERR((((uint32_t)phytxclkforce << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)phytxclkforce << 31));
}

/**
 * @brief Returns the current value of the BDTXCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the BDTXCLKFORCE field's value will be returned.
 *
 * @return The current value of the BDTXCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_bdtxclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

/**
 * @brief Sets the BDTXCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] bdtxclkforce - The value to set the field to.
 */
__INLINE void mdm_bdtxclkforce_setf(uint8_t bdtxclkforce)
{
    ASSERT_ERR((((uint32_t)bdtxclkforce << 30) & ~((uint32_t)0x40000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x40000000)) | ((uint32_t)bdtxclkforce << 30));
}

/**
 * @brief Returns the current value of the AGCMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the AGCMEMCLKFORCE field's value will be returned.
 *
 * @return The current value of the AGCMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_agcmemclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Sets the AGCMEMCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcmemclkforce - The value to set the field to.
 */
__INLINE void mdm_agcmemclkforce_setf(uint8_t agcmemclkforce)
{
    ASSERT_ERR((((uint32_t)agcmemclkforce << 29) & ~((uint32_t)0x20000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x20000000)) | ((uint32_t)agcmemclkforce << 29));
}

/**
 * @brief Returns the current value of the AGCCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the AGCCLKFORCE field's value will be returned.
 *
 * @return The current value of the AGCCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_agcclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the AGCCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcclkforce - The value to set the field to.
 */
__INLINE void mdm_agcclkforce_setf(uint8_t agcclkforce)
{
    ASSERT_ERR((((uint32_t)agcclkforce << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)agcclkforce << 28));
}

/**
 * @brief Returns the current value of the RCCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the RCCLKFORCE field's value will be returned.
 *
 * @return The current value of the RCCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_rcclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

/**
 * @brief Sets the RCCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rcclkforce - The value to set the field to.
 */
__INLINE void mdm_rcclkforce_setf(uint8_t rcclkforce)
{
    ASSERT_ERR((((uint32_t)rcclkforce << 27) & ~((uint32_t)0x08000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x08000000)) | ((uint32_t)rcclkforce << 27));
}

/**
 * @brief Returns the current value of the FECLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FECLKFORCE field's value will be returned.
 *
 * @return The current value of the FECLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_feclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the FECLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] feclkforce - The value to set the field to.
 */
__INLINE void mdm_feclkforce_setf(uint8_t feclkforce)
{
    ASSERT_ERR((((uint32_t)feclkforce << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)feclkforce << 24));
}

/**
 * @brief Returns the current value of the FDOCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FDOCLKFORCE field's value will be returned.
 *
 * @return The current value of the FDOCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_fdoclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the FDOCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fdoclkforce - The value to set the field to.
 */
__INLINE void mdm_fdoclkforce_setf(uint8_t fdoclkforce)
{
    ASSERT_ERR((((uint32_t)fdoclkforce << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)fdoclkforce << 23));
}

/**
 * @brief Returns the current value of the EQUCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the EQUCLKFORCE field's value will be returned.
 *
 * @return The current value of the EQUCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_equclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Sets the EQUCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] equclkforce - The value to set the field to.
 */
__INLINE void mdm_equclkforce_setf(uint8_t equclkforce)
{
    ASSERT_ERR((((uint32_t)equclkforce << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)equclkforce << 22));
}

/**
 * @brief Returns the current value of the PHYSVDCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the PHYSVDCLKFORCE field's value will be returned.
 *
 * @return The current value of the PHYSVDCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_physvdclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Sets the PHYSVDCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] physvdclkforce - The value to set the field to.
 */
__INLINE void mdm_physvdclkforce_setf(uint8_t physvdclkforce)
{
    ASSERT_ERR((((uint32_t)physvdclkforce << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)physvdclkforce << 21));
}

/**
 * @brief Returns the current value of the PHYTXBFCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the PHYTXBFCLKFORCE field's value will be returned.
 *
 * @return The current value of the PHYTXBFCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_phytxbfclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the PHYTXBFCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] phytxbfclkforce - The value to set the field to.
 */
__INLINE void mdm_phytxbfclkforce_setf(uint8_t phytxbfclkforce)
{
    ASSERT_ERR((((uint32_t)phytxbfclkforce << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)phytxbfclkforce << 20));
}

/**
 * @brief Returns the current value of the TDCOMPCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the TDCOMPCLKFORCE field's value will be returned.
 *
 * @return The current value of the TDCOMPCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_tdcompclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

/**
 * @brief Sets the TDCOMPCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdcompclkforce - The value to set the field to.
 */
__INLINE void mdm_tdcompclkforce_setf(uint8_t tdcompclkforce)
{
    ASSERT_ERR((((uint32_t)tdcompclkforce << 18) & ~((uint32_t)0x00040000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00040000)) | ((uint32_t)tdcompclkforce << 18));
}

/**
 * @brief Returns the current value of the TDFOESTCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the TDFOESTCLKFORCE field's value will be returned.
 *
 * @return The current value of the TDFOESTCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_tdfoestclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00020000)) >> 17);
}

/**
 * @brief Sets the TDFOESTCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tdfoestclkforce - The value to set the field to.
 */
__INLINE void mdm_tdfoestclkforce_setf(uint8_t tdfoestclkforce)
{
    ASSERT_ERR((((uint32_t)tdfoestclkforce << 17) & ~((uint32_t)0x00020000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00020000)) | ((uint32_t)tdfoestclkforce << 17));
}

/**
 * @brief Returns the current value of the TBECLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the TBECLKFORCE field's value will be returned.
 *
 * @return The current value of the TBECLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_tbeclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the TBECLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbeclkforce - The value to set the field to.
 */
__INLINE void mdm_tbeclkforce_setf(uint8_t tbeclkforce)
{
    ASSERT_ERR((((uint32_t)tbeclkforce << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)tbeclkforce << 16));
}

/**
 * @brief Returns the current value of the FFT1MEMCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FFT1MEMCLKFORCE field's value will be returned.
 *
 * @return The current value of the FFT1MEMCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_fft1memclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

/**
 * @brief Sets the FFT1MEMCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fft1memclkforce - The value to set the field to.
 */
__INLINE void mdm_fft1memclkforce_setf(uint8_t fft1memclkforce)
{
    ASSERT_ERR((((uint32_t)fft1memclkforce << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)fft1memclkforce << 10));
}

/**
 * @brief Returns the current value of the FFT1CLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FFT1CLKFORCE field's value will be returned.
 *
 * @return The current value of the FFT1CLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_fft1clkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the FFT1CLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fft1clkforce - The value to set the field to.
 */
__INLINE void mdm_fft1clkforce_setf(uint8_t fft1clkforce)
{
    ASSERT_ERR((((uint32_t)fft1clkforce << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)fft1clkforce << 9));
}

/**
 * @brief Returns the current value of the FFT0MEMCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FFT0MEMCLKFORCE field's value will be returned.
 *
 * @return The current value of the FFT0MEMCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_fft0memclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the FFT0MEMCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fft0memclkforce - The value to set the field to.
 */
__INLINE void mdm_fft0memclkforce_setf(uint8_t fft0memclkforce)
{
    ASSERT_ERR((((uint32_t)fft0memclkforce << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)fft0memclkforce << 8));
}

/**
 * @brief Returns the current value of the FFT0CLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the FFT0CLKFORCE field's value will be returned.
 *
 * @return The current value of the FFT0CLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_fft0clkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000080)) >> 7);
}

/**
 * @brief Sets the FFT0CLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fft0clkforce - The value to set the field to.
 */
__INLINE void mdm_fft0clkforce_setf(uint8_t fft0clkforce)
{
    ASSERT_ERR((((uint32_t)fft0clkforce << 7) & ~((uint32_t)0x00000080)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000080)) | ((uint32_t)fft0clkforce << 7));
}

/**
 * @brief Returns the current value of the CHESTCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the CHESTCLKFORCE field's value will be returned.
 *
 * @return The current value of the CHESTCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_chestclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000040)) >> 6);
}

/**
 * @brief Sets the CHESTCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] chestclkforce - The value to set the field to.
 */
__INLINE void mdm_chestclkforce_setf(uint8_t chestclkforce)
{
    ASSERT_ERR((((uint32_t)chestclkforce << 6) & ~((uint32_t)0x00000040)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000040)) | ((uint32_t)chestclkforce << 6));
}

/**
 * @brief Returns the current value of the HMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the HMEMCLKFORCE field's value will be returned.
 *
 * @return The current value of the HMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_hmemclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the HMEMCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] hmemclkforce - The value to set the field to.
 */
__INLINE void mdm_hmemclkforce_setf(uint8_t hmemclkforce)
{
    ASSERT_ERR((((uint32_t)hmemclkforce << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)hmemclkforce << 4));
}

/**
 * @brief Returns the current value of the INTLVMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the INTLVMEMCLKFORCE field's value will be returned.
 *
 * @return The current value of the INTLVMEMCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_intlvmemclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the INTLVMEMCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] intlvmemclkforce - The value to set the field to.
 */
__INLINE void mdm_intlvmemclkforce_setf(uint8_t intlvmemclkforce)
{
    ASSERT_ERR((((uint32_t)intlvmemclkforce << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)intlvmemclkforce << 3));
}

/**
 * @brief Returns the current value of the VTB1CLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the VTB1CLKFORCE field's value will be returned.
 *
 * @return The current value of the VTB1CLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_vtb1clkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the VTB1CLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] vtb1clkforce - The value to set the field to.
 */
__INLINE void mdm_vtb1clkforce_setf(uint8_t vtb1clkforce)
{
    ASSERT_ERR((((uint32_t)vtb1clkforce << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)vtb1clkforce << 2));
}

/**
 * @brief Returns the current value of the VTB0CLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the VTB0CLKFORCE field's value will be returned.
 *
 * @return The current value of the VTB0CLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_vtb0clkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the VTB0CLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] vtb0clkforce - The value to set the field to.
 */
__INLINE void mdm_vtb0clkforce_setf(uint8_t vtb0clkforce)
{
    ASSERT_ERR((((uint32_t)vtb0clkforce << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)vtb0clkforce << 1));
}

/**
 * @brief Returns the current value of the BDRXCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the BDRXCLKFORCE field's value will be returned.
 *
 * @return The current value of the BDRXCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_bdrxclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the BDRXCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] bdrxclkforce - The value to set the field to.
 */
__INLINE void mdm_bdrxclkforce_setf(uint8_t bdrxclkforce)
{
    ASSERT_ERR((((uint32_t)bdrxclkforce << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)bdrxclkforce << 0));
}

/// @}

/**
 * @name CLKGATEFCTRL1 register definitions
 * <table>
 * <caption id="CLKGATEFCTRL1_BF">CLKGATEFCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>05 <td>  RADARTIMCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>04 <td>    MDMBTXCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>03 <td>    MDMBRXCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>01 <td>   LDPCENCCLKFORCE <td>R <td>R/W <td>0
 * <tr><td>00 <td>   LDPCDECCLKFORCE <td>R <td>R/W <td>0
 * </table>
 *
 * @{
 */

/// Address of the CLKGATEFCTRL1 register
#define MDM_CLKGATEFCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0878)
/// Offset of the CLKGATEFCTRL1 register from the base address
#define MDM_CLKGATEFCTRL1_OFFSET 0x00000078
/// Index of the CLKGATEFCTRL1 register
#define MDM_CLKGATEFCTRL1_INDEX  0x0000001E
/// Reset value of the CLKGATEFCTRL1 register
#define MDM_CLKGATEFCTRL1_RESET  0x00000000

/**
 * @brief Returns the current value of the CLKGATEFCTRL1 register.
 * The CLKGATEFCTRL1 register will be read and its value returned.
 * @return The current value of the CLKGATEFCTRL1 register.
 */
__INLINE uint32_t mdm_clkgatefctrl1_get(void)
{
    return REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
}

/**
 * @brief Sets the CLKGATEFCTRL1 register to a value.
 * The CLKGATEFCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_clkgatefctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, value);
}

// field definitions
/// RADARTIMCLKFORCE field bit
#define MDM_RADARTIMCLKFORCE_BIT    ((uint32_t)0x00000020)
/// RADARTIMCLKFORCE field position
#define MDM_RADARTIMCLKFORCE_POS    5
/// MDMBTXCLKFORCE field bit
#define MDM_MDMBTXCLKFORCE_BIT      ((uint32_t)0x00000010)
/// MDMBTXCLKFORCE field position
#define MDM_MDMBTXCLKFORCE_POS      4
/// MDMBRXCLKFORCE field bit
#define MDM_MDMBRXCLKFORCE_BIT      ((uint32_t)0x00000008)
/// MDMBRXCLKFORCE field position
#define MDM_MDMBRXCLKFORCE_POS      3
/// LDPCENCCLKFORCE field bit
#define MDM_LDPCENCCLKFORCE_BIT     ((uint32_t)0x00000002)
/// LDPCENCCLKFORCE field position
#define MDM_LDPCENCCLKFORCE_POS     1
/// LDPCDECCLKFORCE field bit
#define MDM_LDPCDECCLKFORCE_BIT     ((uint32_t)0x00000001)
/// LDPCDECCLKFORCE field position
#define MDM_LDPCDECCLKFORCE_POS     0

/// RADARTIMCLKFORCE field reset value
#define MDM_RADARTIMCLKFORCE_RST    0x0
/// MDMBTXCLKFORCE field reset value
#define MDM_MDMBTXCLKFORCE_RST      0x0
/// MDMBRXCLKFORCE field reset value
#define MDM_MDMBRXCLKFORCE_RST      0x0
/// LDPCENCCLKFORCE field reset value
#define MDM_LDPCENCCLKFORCE_RST     0x0
/// LDPCDECCLKFORCE field reset value
#define MDM_LDPCDECCLKFORCE_RST     0x0

/**
 * @brief Constructs a value for the CLKGATEFCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] radartimclkforce - The value to use for the RADARTIMCLKFORCE field.
 * @param[in] mdmbtxclkforce - The value to use for the MDMBTXCLKFORCE field.
 * @param[in] mdmbrxclkforce - The value to use for the MDMBRXCLKFORCE field.
 * @param[in] ldpcencclkforce - The value to use for the LDPCENCCLKFORCE field.
 * @param[in] ldpcdecclkforce - The value to use for the LDPCDECCLKFORCE field.
 */
__INLINE void mdm_clkgatefctrl1_pack(uint8_t radartimclkforce, uint8_t mdmbtxclkforce, uint8_t mdmbrxclkforce, uint8_t ldpcencclkforce, uint8_t ldpcdecclkforce)
{
    ASSERT_ERR((((uint32_t)radartimclkforce << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)mdmbtxclkforce << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)mdmbrxclkforce << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)ldpcencclkforce << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)ldpcdecclkforce << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR,  ((uint32_t)radartimclkforce << 5) | ((uint32_t)mdmbtxclkforce << 4) | ((uint32_t)mdmbrxclkforce << 3) | ((uint32_t)ldpcencclkforce << 1) | ((uint32_t)ldpcdecclkforce << 0));
}

/**
 * @brief Unpacks CLKGATEFCTRL1's fields from current value of the CLKGATEFCTRL1 register.
 *
 * Reads the CLKGATEFCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] radartimclkforce - Will be populated with the current value of this field from the register.
 * @param[out] mdmbtxclkforce - Will be populated with the current value of this field from the register.
 * @param[out] mdmbrxclkforce - Will be populated with the current value of this field from the register.
 * @param[out] ldpcencclkforce - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdecclkforce - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_clkgatefctrl1_unpack(uint8_t* radartimclkforce, uint8_t* mdmbtxclkforce, uint8_t* mdmbrxclkforce, uint8_t* ldpcencclkforce, uint8_t* ldpcdecclkforce)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);

    *radartimclkforce = (localVal & ((uint32_t)0x00000020)) >> 5;
    *mdmbtxclkforce = (localVal & ((uint32_t)0x00000010)) >> 4;
    *mdmbrxclkforce = (localVal & ((uint32_t)0x00000008)) >> 3;
    *ldpcencclkforce = (localVal & ((uint32_t)0x00000002)) >> 1;
    *ldpcdecclkforce = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the RADARTIMCLKFORCE field in the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read and the RADARTIMCLKFORCE field's value will be returned.
 *
 * @return The current value of the RADARTIMCLKFORCE field in the CLKGATEFCTRL1 register.
 */
__INLINE uint8_t mdm_radartimclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

/**
 * @brief Sets the RADARTIMCLKFORCE field of the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] radartimclkforce - The value to set the field to.
 */
__INLINE void mdm_radartimclkforce_setf(uint8_t radartimclkforce)
{
    ASSERT_ERR((((uint32_t)radartimclkforce << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)radartimclkforce << 5));
}

/**
 * @brief Returns the current value of the MDMBTXCLKFORCE field in the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read and the MDMBTXCLKFORCE field's value will be returned.
 *
 * @return The current value of the MDMBTXCLKFORCE field in the CLKGATEFCTRL1 register.
 */
__INLINE uint8_t mdm_mdmbtxclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the MDMBTXCLKFORCE field of the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mdmbtxclkforce - The value to set the field to.
 */
__INLINE void mdm_mdmbtxclkforce_setf(uint8_t mdmbtxclkforce)
{
    ASSERT_ERR((((uint32_t)mdmbtxclkforce << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)mdmbtxclkforce << 4));
}

/**
 * @brief Returns the current value of the MDMBRXCLKFORCE field in the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read and the MDMBRXCLKFORCE field's value will be returned.
 *
 * @return The current value of the MDMBRXCLKFORCE field in the CLKGATEFCTRL1 register.
 */
__INLINE uint8_t mdm_mdmbrxclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the MDMBRXCLKFORCE field of the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mdmbrxclkforce - The value to set the field to.
 */
__INLINE void mdm_mdmbrxclkforce_setf(uint8_t mdmbrxclkforce)
{
    ASSERT_ERR((((uint32_t)mdmbrxclkforce << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)mdmbrxclkforce << 3));
}

/**
 * @brief Returns the current value of the LDPCENCCLKFORCE field in the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read and the LDPCENCCLKFORCE field's value will be returned.
 *
 * @return The current value of the LDPCENCCLKFORCE field in the CLKGATEFCTRL1 register.
 */
__INLINE uint8_t mdm_ldpcencclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the LDPCENCCLKFORCE field of the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcencclkforce - The value to set the field to.
 */
__INLINE void mdm_ldpcencclkforce_setf(uint8_t ldpcencclkforce)
{
    ASSERT_ERR((((uint32_t)ldpcencclkforce << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)ldpcencclkforce << 1));
}

/**
 * @brief Returns the current value of the LDPCDECCLKFORCE field in the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read and the LDPCDECCLKFORCE field's value will be returned.
 *
 * @return The current value of the LDPCDECCLKFORCE field in the CLKGATEFCTRL1 register.
 */
__INLINE uint8_t mdm_ldpcdecclkforce_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the LDPCDECCLKFORCE field of the CLKGATEFCTRL1 register.
 *
 * The CLKGATEFCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecclkforce - The value to set the field to.
 */
__INLINE void mdm_ldpcdecclkforce_setf(uint8_t ldpcdecclkforce)
{
    ASSERT_ERR((((uint32_t)ldpcdecclkforce << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL1_ADDR, (REG_PL_RD(MDM_CLKGATEFCTRL1_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)ldpcdecclkforce << 0));
}

/// @}

/**
 * @name SWRESET register definitions
 * <table>
 * <caption id="SWRESET_BF">SWRESET bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>12 <td>        AGCSWRESET <td>R <td>R/W <td>0
 * <tr><td>08 <td>        DSPSWRESET <td>R <td>R/W <td>0
 * <tr><td>00 <td>        MDMSWRESET <td>R <td>R/W <td>0
 * </table>
 *
 * @{
 */

/// Address of the SWRESET register
#define MDM_SWRESET_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0888)
/// Offset of the SWRESET register from the base address
#define MDM_SWRESET_OFFSET 0x00000088
/// Index of the SWRESET register
#define MDM_SWRESET_INDEX  0x00000022
/// Reset value of the SWRESET register
#define MDM_SWRESET_RESET  0x00000000

/**
 * @brief Returns the current value of the SWRESET register.
 * The SWRESET register will be read and its value returned.
 * @return The current value of the SWRESET register.
 */
__INLINE uint32_t mdm_swreset_get(void)
{
    return REG_PL_RD(MDM_SWRESET_ADDR);
}

/**
 * @brief Sets the SWRESET register to a value.
 * The SWRESET register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_swreset_set(uint32_t value)
{
    REG_PL_WR(MDM_SWRESET_ADDR, value);
}

// field definitions
/// AGCSWRESET field bit
#define MDM_AGCSWRESET_BIT    ((uint32_t)0x00001000)
/// AGCSWRESET field position
#define MDM_AGCSWRESET_POS    12
/// DSPSWRESET field bit
#define MDM_DSPSWRESET_BIT    ((uint32_t)0x00000100)
/// DSPSWRESET field position
#define MDM_DSPSWRESET_POS    8
/// MDMSWRESET field bit
#define MDM_MDMSWRESET_BIT    ((uint32_t)0x00000001)
/// MDMSWRESET field position
#define MDM_MDMSWRESET_POS    0

/// AGCSWRESET field reset value
#define MDM_AGCSWRESET_RST    0x0
/// DSPSWRESET field reset value
#define MDM_DSPSWRESET_RST    0x0
/// MDMSWRESET field reset value
#define MDM_MDMSWRESET_RST    0x0

/**
 * @brief Constructs a value for the SWRESET register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcswreset - The value to use for the AGCSWRESET field.
 * @param[in] dspswreset - The value to use for the DSPSWRESET field.
 * @param[in] mdmswreset - The value to use for the MDMSWRESET field.
 */
__INLINE void mdm_swreset_pack(uint8_t agcswreset, uint8_t dspswreset, uint8_t mdmswreset)
{
    ASSERT_ERR((((uint32_t)agcswreset << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)dspswreset << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)mdmswreset << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_SWRESET_ADDR,  ((uint32_t)agcswreset << 12) | ((uint32_t)dspswreset << 8) | ((uint32_t)mdmswreset << 0));
}

/**
 * @brief Unpacks SWRESET's fields from current value of the SWRESET register.
 *
 * Reads the SWRESET register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcswreset - Will be populated with the current value of this field from the register.
 * @param[out] dspswreset - Will be populated with the current value of this field from the register.
 * @param[out] mdmswreset - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_swreset_unpack(uint8_t* agcswreset, uint8_t* dspswreset, uint8_t* mdmswreset)
{
    uint32_t localVal = REG_PL_RD(MDM_SWRESET_ADDR);

    *agcswreset = (localVal & ((uint32_t)0x00001000)) >> 12;
    *dspswreset = (localVal & ((uint32_t)0x00000100)) >> 8;
    *mdmswreset = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the AGCSWRESET field in the SWRESET register.
 *
 * The SWRESET register will be read and the AGCSWRESET field's value will be returned.
 *
 * @return The current value of the AGCSWRESET field in the SWRESET register.
 */
__INLINE uint8_t mdm_agcswreset_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SWRESET_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the AGCSWRESET field of the SWRESET register.
 *
 * The SWRESET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcswreset - The value to set the field to.
 */
__INLINE void mdm_agcswreset_setf(uint8_t agcswreset)
{
    ASSERT_ERR((((uint32_t)agcswreset << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(MDM_SWRESET_ADDR, (REG_PL_RD(MDM_SWRESET_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)agcswreset << 12));
}

/**
 * @brief Returns the current value of the DSPSWRESET field in the SWRESET register.
 *
 * The SWRESET register will be read and the DSPSWRESET field's value will be returned.
 *
 * @return The current value of the DSPSWRESET field in the SWRESET register.
 */
__INLINE uint8_t mdm_dspswreset_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SWRESET_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the DSPSWRESET field of the SWRESET register.
 *
 * The SWRESET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dspswreset - The value to set the field to.
 */
__INLINE void mdm_dspswreset_setf(uint8_t dspswreset)
{
    ASSERT_ERR((((uint32_t)dspswreset << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_SWRESET_ADDR, (REG_PL_RD(MDM_SWRESET_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)dspswreset << 8));
}

/**
 * @brief Returns the current value of the MDMSWRESET field in the SWRESET register.
 *
 * The SWRESET register will be read and the MDMSWRESET field's value will be returned.
 *
 * @return The current value of the MDMSWRESET field in the SWRESET register.
 */
__INLINE uint8_t mdm_mdmswreset_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SWRESET_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the MDMSWRESET field of the SWRESET register.
 *
 * The SWRESET register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mdmswreset - The value to set the field to.
 */
__INLINE void mdm_mdmswreset_setf(uint8_t mdmswreset)
{
    ASSERT_ERR((((uint32_t)mdmswreset << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_SWRESET_ADDR, (REG_PL_RD(MDM_SWRESET_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)mdmswreset << 0));
}

/// @}

/**
 * @name TXCTRL1 register definitions
 * <table>
 * <caption id="TXCTRL1_BF">TXCTRL1 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:24 <td>   TXFEOFDM80DELAY <td>R <td>R/W <td>0x0
 * <tr><td>23:16 <td>   TXFEOFDM40DELAY <td>R <td>R/W <td>0x0
 * <tr><td>15:08 <td>   TXFEOFDM20DELAY <td>R <td>R/W <td>0x1C
 * <tr><td>07:00 <td>     TXFEDSSSDELAY <td>R <td>R/W <td>0x30
 * </table>
 *
 * @{
 */

/// Address of the TXCTRL1 register
#define MDM_TXCTRL1_ADDR   (REG_MDM_CFG_BASE_ADDR+0x088C)
/// Offset of the TXCTRL1 register from the base address
#define MDM_TXCTRL1_OFFSET 0x0000008C
/// Index of the TXCTRL1 register
#define MDM_TXCTRL1_INDEX  0x00000023
/// Reset value of the TXCTRL1 register
#define MDM_TXCTRL1_RESET  0x00001C30

/**
 * @brief Returns the current value of the TXCTRL1 register.
 * The TXCTRL1 register will be read and its value returned.
 * @return The current value of the TXCTRL1 register.
 */
__INLINE uint32_t mdm_txctrl1_get(void)
{
    return REG_PL_RD(MDM_TXCTRL1_ADDR);
}

/**
 * @brief Sets the TXCTRL1 register to a value.
 * The TXCTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_txctrl1_set(uint32_t value)
{
    REG_PL_WR(MDM_TXCTRL1_ADDR, value);
}

// field definitions
/// TXFEOFDM80DELAY field mask
#define MDM_TXFEOFDM80DELAY_MASK   ((uint32_t)0xFF000000)
/// TXFEOFDM80DELAY field LSB position
#define MDM_TXFEOFDM80DELAY_LSB    24
/// TXFEOFDM80DELAY field width
#define MDM_TXFEOFDM80DELAY_WIDTH  ((uint32_t)0x00000008)
/// TXFEOFDM40DELAY field mask
#define MDM_TXFEOFDM40DELAY_MASK   ((uint32_t)0x00FF0000)
/// TXFEOFDM40DELAY field LSB position
#define MDM_TXFEOFDM40DELAY_LSB    16
/// TXFEOFDM40DELAY field width
#define MDM_TXFEOFDM40DELAY_WIDTH  ((uint32_t)0x00000008)
/// TXFEOFDM20DELAY field mask
#define MDM_TXFEOFDM20DELAY_MASK   ((uint32_t)0x0000FF00)
/// TXFEOFDM20DELAY field LSB position
#define MDM_TXFEOFDM20DELAY_LSB    8
/// TXFEOFDM20DELAY field width
#define MDM_TXFEOFDM20DELAY_WIDTH  ((uint32_t)0x00000008)
/// TXFEDSSSDELAY field mask
#define MDM_TXFEDSSSDELAY_MASK     ((uint32_t)0x000000FF)
/// TXFEDSSSDELAY field LSB position
#define MDM_TXFEDSSSDELAY_LSB      0
/// TXFEDSSSDELAY field width
#define MDM_TXFEDSSSDELAY_WIDTH    ((uint32_t)0x00000008)

/// TXFEOFDM80DELAY field reset value
#define MDM_TXFEOFDM80DELAY_RST    0x0
/// TXFEOFDM40DELAY field reset value
#define MDM_TXFEOFDM40DELAY_RST    0x0
/// TXFEOFDM20DELAY field reset value
#define MDM_TXFEOFDM20DELAY_RST    0x1C
/// TXFEDSSSDELAY field reset value
#define MDM_TXFEDSSSDELAY_RST      0x30

/**
 * @brief Constructs a value for the TXCTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txfeofdm80delay - The value to use for the TXFEOFDM80DELAY field.
 * @param[in] txfeofdm40delay - The value to use for the TXFEOFDM40DELAY field.
 * @param[in] txfeofdm20delay - The value to use for the TXFEOFDM20DELAY field.
 * @param[in] txfedsssdelay - The value to use for the TXFEDSSSDELAY field.
 */
__INLINE void mdm_txctrl1_pack(uint8_t txfeofdm80delay, uint8_t txfeofdm40delay, uint8_t txfeofdm20delay, uint8_t txfedsssdelay)
{
    ASSERT_ERR((((uint32_t)txfeofdm80delay << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)txfeofdm40delay << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)txfeofdm20delay << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)txfedsssdelay << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TXCTRL1_ADDR,  ((uint32_t)txfeofdm80delay << 24) | ((uint32_t)txfeofdm40delay << 16) | ((uint32_t)txfeofdm20delay << 8) | ((uint32_t)txfedsssdelay << 0));
}

/**
 * @brief Unpacks TXCTRL1's fields from current value of the TXCTRL1 register.
 *
 * Reads the TXCTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txfeofdm80delay - Will be populated with the current value of this field from the register.
 * @param[out] txfeofdm40delay - Will be populated with the current value of this field from the register.
 * @param[out] txfeofdm20delay - Will be populated with the current value of this field from the register.
 * @param[out] txfedsssdelay - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_txctrl1_unpack(uint8_t* txfeofdm80delay, uint8_t* txfeofdm40delay, uint8_t* txfeofdm20delay, uint8_t* txfedsssdelay)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL1_ADDR);

    *txfeofdm80delay = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *txfeofdm40delay = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *txfeofdm20delay = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *txfedsssdelay = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TXFEOFDM80DELAY field in the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read and the TXFEOFDM80DELAY field's value will be returned.
 *
 * @return The current value of the TXFEOFDM80DELAY field in the TXCTRL1 register.
 */
__INLINE uint8_t mdm_txfeofdm80delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the TXFEOFDM80DELAY field of the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txfeofdm80delay - The value to set the field to.
 */
__INLINE void mdm_txfeofdm80delay_setf(uint8_t txfeofdm80delay)
{
    ASSERT_ERR((((uint32_t)txfeofdm80delay << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(MDM_TXCTRL1_ADDR, (REG_PL_RD(MDM_TXCTRL1_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)txfeofdm80delay << 24));
}

/**
 * @brief Returns the current value of the TXFEOFDM40DELAY field in the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read and the TXFEOFDM40DELAY field's value will be returned.
 *
 * @return The current value of the TXFEOFDM40DELAY field in the TXCTRL1 register.
 */
__INLINE uint8_t mdm_txfeofdm40delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the TXFEOFDM40DELAY field of the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txfeofdm40delay - The value to set the field to.
 */
__INLINE void mdm_txfeofdm40delay_setf(uint8_t txfeofdm40delay)
{
    ASSERT_ERR((((uint32_t)txfeofdm40delay << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(MDM_TXCTRL1_ADDR, (REG_PL_RD(MDM_TXCTRL1_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)txfeofdm40delay << 16));
}

/**
 * @brief Returns the current value of the TXFEOFDM20DELAY field in the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read and the TXFEOFDM20DELAY field's value will be returned.
 *
 * @return The current value of the TXFEOFDM20DELAY field in the TXCTRL1 register.
 */
__INLINE uint8_t mdm_txfeofdm20delay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the TXFEOFDM20DELAY field of the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txfeofdm20delay - The value to set the field to.
 */
__INLINE void mdm_txfeofdm20delay_setf(uint8_t txfeofdm20delay)
{
    ASSERT_ERR((((uint32_t)txfeofdm20delay << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_TXCTRL1_ADDR, (REG_PL_RD(MDM_TXCTRL1_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)txfeofdm20delay << 8));
}

/**
 * @brief Returns the current value of the TXFEDSSSDELAY field in the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read and the TXFEDSSSDELAY field's value will be returned.
 *
 * @return The current value of the TXFEDSSSDELAY field in the TXCTRL1 register.
 */
__INLINE uint8_t mdm_txfedsssdelay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TXFEDSSSDELAY field of the TXCTRL1 register.
 *
 * The TXCTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txfedsssdelay - The value to set the field to.
 */
__INLINE void mdm_txfedsssdelay_setf(uint8_t txfedsssdelay)
{
    ASSERT_ERR((((uint32_t)txfedsssdelay << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TXCTRL1_ADDR, (REG_PL_RD(MDM_TXCTRL1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)txfedsssdelay << 0));
}

/// @}

/**
 * @name TXCTRL2 register definitions
 * <table>
 * <caption id="TXCTRL2_BF">TXCTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>23:16 <td>         TXTDCSD80 <td>R <td>R/W <td>0xF0
 * <tr><td>15:08 <td>         TXTDCSD40 <td>R <td>R/W <td>0xF8
 * <tr><td>07:00 <td>         TXTDCSD20 <td>R <td>R/W <td>0xFC
 * </table>
 *
 * @{
 */

/// Address of the TXCTRL2 register
#define MDM_TXCTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0890)
/// Offset of the TXCTRL2 register from the base address
#define MDM_TXCTRL2_OFFSET 0x00000090
/// Index of the TXCTRL2 register
#define MDM_TXCTRL2_INDEX  0x00000024
/// Reset value of the TXCTRL2 register
#define MDM_TXCTRL2_RESET  0x00F0F8FC

/**
 * @brief Returns the current value of the TXCTRL2 register.
 * The TXCTRL2 register will be read and its value returned.
 * @return The current value of the TXCTRL2 register.
 */
__INLINE uint32_t mdm_txctrl2_get(void)
{
    return REG_PL_RD(MDM_TXCTRL2_ADDR);
}

/**
 * @brief Sets the TXCTRL2 register to a value.
 * The TXCTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_txctrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_TXCTRL2_ADDR, value);
}

// field definitions
/// TXTDCSD80 field mask
#define MDM_TXTDCSD80_MASK   ((uint32_t)0x00FF0000)
/// TXTDCSD80 field LSB position
#define MDM_TXTDCSD80_LSB    16
/// TXTDCSD80 field width
#define MDM_TXTDCSD80_WIDTH  ((uint32_t)0x00000008)
/// TXTDCSD40 field mask
#define MDM_TXTDCSD40_MASK   ((uint32_t)0x0000FF00)
/// TXTDCSD40 field LSB position
#define MDM_TXTDCSD40_LSB    8
/// TXTDCSD40 field width
#define MDM_TXTDCSD40_WIDTH  ((uint32_t)0x00000008)
/// TXTDCSD20 field mask
#define MDM_TXTDCSD20_MASK   ((uint32_t)0x000000FF)
/// TXTDCSD20 field LSB position
#define MDM_TXTDCSD20_LSB    0
/// TXTDCSD20 field width
#define MDM_TXTDCSD20_WIDTH  ((uint32_t)0x00000008)

/// TXTDCSD80 field reset value
#define MDM_TXTDCSD80_RST    0xF0
/// TXTDCSD40 field reset value
#define MDM_TXTDCSD40_RST    0xF8
/// TXTDCSD20 field reset value
#define MDM_TXTDCSD20_RST    0xFC

/**
 * @brief Constructs a value for the TXCTRL2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txtdcsd80 - The value to use for the TXTDCSD80 field.
 * @param[in] txtdcsd40 - The value to use for the TXTDCSD40 field.
 * @param[in] txtdcsd20 - The value to use for the TXTDCSD20 field.
 */
__INLINE void mdm_txctrl2_pack(uint8_t txtdcsd80, uint8_t txtdcsd40, uint8_t txtdcsd20)
{
    ASSERT_ERR((((uint32_t)txtdcsd80 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)txtdcsd40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)txtdcsd20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TXCTRL2_ADDR,  ((uint32_t)txtdcsd80 << 16) | ((uint32_t)txtdcsd40 << 8) | ((uint32_t)txtdcsd20 << 0));
}

/**
 * @brief Unpacks TXCTRL2's fields from current value of the TXCTRL2 register.
 *
 * Reads the TXCTRL2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txtdcsd80 - Will be populated with the current value of this field from the register.
 * @param[out] txtdcsd40 - Will be populated with the current value of this field from the register.
 * @param[out] txtdcsd20 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_txctrl2_unpack(uint8_t* txtdcsd80, uint8_t* txtdcsd40, uint8_t* txtdcsd20)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL2_ADDR);

    *txtdcsd80 = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *txtdcsd40 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *txtdcsd20 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TXTDCSD80 field in the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read and the TXTDCSD80 field's value will be returned.
 *
 * @return The current value of the TXTDCSD80 field in the TXCTRL2 register.
 */
__INLINE uint8_t mdm_txtdcsd80_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the TXTDCSD80 field of the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txtdcsd80 - The value to set the field to.
 */
__INLINE void mdm_txtdcsd80_setf(uint8_t txtdcsd80)
{
    ASSERT_ERR((((uint32_t)txtdcsd80 << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(MDM_TXCTRL2_ADDR, (REG_PL_RD(MDM_TXCTRL2_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)txtdcsd80 << 16));
}

/**
 * @brief Returns the current value of the TXTDCSD40 field in the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read and the TXTDCSD40 field's value will be returned.
 *
 * @return The current value of the TXTDCSD40 field in the TXCTRL2 register.
 */
__INLINE uint8_t mdm_txtdcsd40_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the TXTDCSD40 field of the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txtdcsd40 - The value to set the field to.
 */
__INLINE void mdm_txtdcsd40_setf(uint8_t txtdcsd40)
{
    ASSERT_ERR((((uint32_t)txtdcsd40 << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(MDM_TXCTRL2_ADDR, (REG_PL_RD(MDM_TXCTRL2_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)txtdcsd40 << 8));
}

/**
 * @brief Returns the current value of the TXTDCSD20 field in the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read and the TXTDCSD20 field's value will be returned.
 *
 * @return The current value of the TXTDCSD20 field in the TXCTRL2 register.
 */
__INLINE uint8_t mdm_txtdcsd20_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL2_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the TXTDCSD20 field of the TXCTRL2 register.
 *
 * The TXCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txtdcsd20 - The value to set the field to.
 */
__INLINE void mdm_txtdcsd20_setf(uint8_t txtdcsd20)
{
    ASSERT_ERR((((uint32_t)txtdcsd20 << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_TXCTRL2_ADDR, (REG_PL_RD(MDM_TXCTRL2_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)txtdcsd20 << 0));
}

/// @}

#if RW_NX_LDPC_DEC
/**
 * @name LDPCCTRL register definitions
 * <table>
 * <caption id="LDPCCTRL_BF">LDPCCTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>25:20 <td>   LDPCDECSPLITCFG <td>R <td>R/W <td>0x3F
 * <tr><td>16 <td>    LDPCDECSPLITEN <td>R <td>R/W <td>1
 * <tr><td>12 <td>LDPCDECBEATTIMELINE <td>R <td>R/W <td>0
 * <tr><td>08 <td> LDPCDECENLOSTTIME <td>R <td>R/W <td>0
 * <tr><td>05:00 <td>   LDPCDECLLRUNITY <td>R <td>R/W <td>0x2
 * </table>
 *
 * @{
 */

/// Address of the LDPCCTRL register
#define MDM_LDPCCTRL_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0894)
/// Offset of the LDPCCTRL register from the base address
#define MDM_LDPCCTRL_OFFSET 0x00000094
/// Index of the LDPCCTRL register
#define MDM_LDPCCTRL_INDEX  0x00000025
/// Reset value of the LDPCCTRL register
#define MDM_LDPCCTRL_RESET  0x03F10002

/**
 * @brief Returns the current value of the LDPCCTRL register.
 * The LDPCCTRL register will be read and its value returned.
 * @return The current value of the LDPCCTRL register.
 */
__INLINE uint32_t mdm_ldpcctrl_get(void)
{
    return REG_PL_RD(MDM_LDPCCTRL_ADDR);
}

/**
 * @brief Sets the LDPCCTRL register to a value.
 * The LDPCCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_ldpcctrl_set(uint32_t value)
{
    REG_PL_WR(MDM_LDPCCTRL_ADDR, value);
}

// field definitions
/// LDPCDECSPLITCFG field mask
#define MDM_LDPCDECSPLITCFG_MASK       ((uint32_t)0x03F00000)
/// LDPCDECSPLITCFG field LSB position
#define MDM_LDPCDECSPLITCFG_LSB        20
/// LDPCDECSPLITCFG field width
#define MDM_LDPCDECSPLITCFG_WIDTH      ((uint32_t)0x00000006)
/// LDPCDECSPLITEN field bit
#define MDM_LDPCDECSPLITEN_BIT         ((uint32_t)0x00010000)
/// LDPCDECSPLITEN field position
#define MDM_LDPCDECSPLITEN_POS         16
/// LDPCDECBEATTIMELINE field bit
#define MDM_LDPCDECBEATTIMELINE_BIT    ((uint32_t)0x00001000)
/// LDPCDECBEATTIMELINE field position
#define MDM_LDPCDECBEATTIMELINE_POS    12
/// LDPCDECENLOSTTIME field bit
#define MDM_LDPCDECENLOSTTIME_BIT      ((uint32_t)0x00000100)
/// LDPCDECENLOSTTIME field position
#define MDM_LDPCDECENLOSTTIME_POS      8
/// LDPCDECLLRUNITY field mask
#define MDM_LDPCDECLLRUNITY_MASK       ((uint32_t)0x0000003F)
/// LDPCDECLLRUNITY field LSB position
#define MDM_LDPCDECLLRUNITY_LSB        0
/// LDPCDECLLRUNITY field width
#define MDM_LDPCDECLLRUNITY_WIDTH      ((uint32_t)0x00000006)

/// LDPCDECSPLITCFG field reset value
#define MDM_LDPCDECSPLITCFG_RST        0x3F
/// LDPCDECSPLITEN field reset value
#define MDM_LDPCDECSPLITEN_RST         0x1
/// LDPCDECBEATTIMELINE field reset value
#define MDM_LDPCDECBEATTIMELINE_RST    0x0
/// LDPCDECENLOSTTIME field reset value
#define MDM_LDPCDECENLOSTTIME_RST      0x0
/// LDPCDECLLRUNITY field reset value
#define MDM_LDPCDECLLRUNITY_RST        0x2

/**
 * @brief Constructs a value for the LDPCCTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ldpcdecsplitcfg - The value to use for the LDPCDECSPLITCFG field.
 * @param[in] ldpcdecspliten - The value to use for the LDPCDECSPLITEN field.
 * @param[in] ldpcdecbeattimeline - The value to use for the LDPCDECBEATTIMELINE field.
 * @param[in] ldpcdecenlosttime - The value to use for the LDPCDECENLOSTTIME field.
 * @param[in] ldpcdecllrunity - The value to use for the LDPCDECLLRUNITY field.
 */
__INLINE void mdm_ldpcctrl_pack(uint8_t ldpcdecsplitcfg, uint8_t ldpcdecspliten, uint8_t ldpcdecbeattimeline, uint8_t ldpcdecenlosttime, uint8_t ldpcdecllrunity)
{
    ASSERT_ERR((((uint32_t)ldpcdecsplitcfg << 20) & ~((uint32_t)0x03F00000)) == 0);
    ASSERT_ERR((((uint32_t)ldpcdecspliten << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)ldpcdecbeattimeline << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)ldpcdecenlosttime << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)ldpcdecllrunity << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR,  ((uint32_t)ldpcdecsplitcfg << 20) | ((uint32_t)ldpcdecspliten << 16) | ((uint32_t)ldpcdecbeattimeline << 12) | ((uint32_t)ldpcdecenlosttime << 8) | ((uint32_t)ldpcdecllrunity << 0));
}

/**
 * @brief Unpacks LDPCCTRL's fields from current value of the LDPCCTRL register.
 *
 * Reads the LDPCCTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ldpcdecsplitcfg - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdecspliten - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdecbeattimeline - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdecenlosttime - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdecllrunity - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_ldpcctrl_unpack(uint8_t* ldpcdecsplitcfg, uint8_t* ldpcdecspliten, uint8_t* ldpcdecbeattimeline, uint8_t* ldpcdecenlosttime, uint8_t* ldpcdecllrunity)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);

    *ldpcdecsplitcfg = (localVal & ((uint32_t)0x03F00000)) >> 20;
    *ldpcdecspliten = (localVal & ((uint32_t)0x00010000)) >> 16;
    *ldpcdecbeattimeline = (localVal & ((uint32_t)0x00001000)) >> 12;
    *ldpcdecenlosttime = (localVal & ((uint32_t)0x00000100)) >> 8;
    *ldpcdecllrunity = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the LDPCDECSPLITCFG field in the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read and the LDPCDECSPLITCFG field's value will be returned.
 *
 * @return The current value of the LDPCDECSPLITCFG field in the LDPCCTRL register.
 */
__INLINE uint8_t mdm_ldpcdecsplitcfg_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);
    return ((localVal & ((uint32_t)0x03F00000)) >> 20);
}

/**
 * @brief Sets the LDPCDECSPLITCFG field of the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecsplitcfg - The value to set the field to.
 */
__INLINE void mdm_ldpcdecsplitcfg_setf(uint8_t ldpcdecsplitcfg)
{
    ASSERT_ERR((((uint32_t)ldpcdecsplitcfg << 20) & ~((uint32_t)0x03F00000)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR, (REG_PL_RD(MDM_LDPCCTRL_ADDR) & ~((uint32_t)0x03F00000)) | ((uint32_t)ldpcdecsplitcfg << 20));
}

/**
 * @brief Returns the current value of the LDPCDECSPLITEN field in the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read and the LDPCDECSPLITEN field's value will be returned.
 *
 * @return The current value of the LDPCDECSPLITEN field in the LDPCCTRL register.
 */
__INLINE uint8_t mdm_ldpcdecspliten_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the LDPCDECSPLITEN field of the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecspliten - The value to set the field to.
 */
__INLINE void mdm_ldpcdecspliten_setf(uint8_t ldpcdecspliten)
{
    ASSERT_ERR((((uint32_t)ldpcdecspliten << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR, (REG_PL_RD(MDM_LDPCCTRL_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)ldpcdecspliten << 16));
}

/**
 * @brief Returns the current value of the LDPCDECBEATTIMELINE field in the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read and the LDPCDECBEATTIMELINE field's value will be returned.
 *
 * @return The current value of the LDPCDECBEATTIMELINE field in the LDPCCTRL register.
 */
__INLINE uint8_t mdm_ldpcdecbeattimeline_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the LDPCDECBEATTIMELINE field of the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecbeattimeline - The value to set the field to.
 */
__INLINE void mdm_ldpcdecbeattimeline_setf(uint8_t ldpcdecbeattimeline)
{
    ASSERT_ERR((((uint32_t)ldpcdecbeattimeline << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR, (REG_PL_RD(MDM_LDPCCTRL_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)ldpcdecbeattimeline << 12));
}

/**
 * @brief Returns the current value of the LDPCDECENLOSTTIME field in the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read and the LDPCDECENLOSTTIME field's value will be returned.
 *
 * @return The current value of the LDPCDECENLOSTTIME field in the LDPCCTRL register.
 */
__INLINE uint8_t mdm_ldpcdecenlosttime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the LDPCDECENLOSTTIME field of the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecenlosttime - The value to set the field to.
 */
__INLINE void mdm_ldpcdecenlosttime_setf(uint8_t ldpcdecenlosttime)
{
    ASSERT_ERR((((uint32_t)ldpcdecenlosttime << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR, (REG_PL_RD(MDM_LDPCCTRL_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)ldpcdecenlosttime << 8));
}

/**
 * @brief Returns the current value of the LDPCDECLLRUNITY field in the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read and the LDPCDECLLRUNITY field's value will be returned.
 *
 * @return The current value of the LDPCDECLLRUNITY field in the LDPCCTRL register.
 */
__INLINE uint8_t mdm_ldpcdecllrunity_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_LDPCCTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the LDPCDECLLRUNITY field of the LDPCCTRL register.
 *
 * The LDPCCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ldpcdecllrunity - The value to set the field to.
 */
__INLINE void mdm_ldpcdecllrunity_setf(uint8_t ldpcdecllrunity)
{
    ASSERT_ERR((((uint32_t)ldpcdecllrunity << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MDM_LDPCCTRL_ADDR, (REG_PL_RD(MDM_LDPCCTRL_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)ldpcdecllrunity << 0));
}

#endif // RW_NX_LDPC_DEC
/// @}

/**
 * @name TXCTRL3 register definitions
 * <table>
 * <caption id="TXCTRL3_BF">TXCTRL3 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>27:16 <td>   TXPHYRDYHTDELAY <td>R <td>R/W <td>0x2D0
 * <tr><td>11:00 <td>TXPHYRDYNONHTDELAY <td>R <td>R/W <td>0x438
 * </table>
 *
 * @{
 */

/// Address of the TXCTRL3 register
#define MDM_TXCTRL3_ADDR   (REG_MDM_CFG_BASE_ADDR+0x0898)
/// Offset of the TXCTRL3 register from the base address
#define MDM_TXCTRL3_OFFSET 0x00000098
/// Index of the TXCTRL3 register
#define MDM_TXCTRL3_INDEX  0x00000026
/// Reset value of the TXCTRL3 register
#define MDM_TXCTRL3_RESET  0x02D00438

/**
 * @brief Returns the current value of the TXCTRL3 register.
 * The TXCTRL3 register will be read and its value returned.
 * @return The current value of the TXCTRL3 register.
 */
__INLINE uint32_t mdm_txctrl3_get(void)
{
    return REG_PL_RD(MDM_TXCTRL3_ADDR);
}

/**
 * @brief Sets the TXCTRL3 register to a value.
 * The TXCTRL3 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_txctrl3_set(uint32_t value)
{
    REG_PL_WR(MDM_TXCTRL3_ADDR, value);
}

// field definitions
/// TXPHYRDYHTDELAY field mask
#define MDM_TXPHYRDYHTDELAY_MASK      ((uint32_t)0x0FFF0000)
/// TXPHYRDYHTDELAY field LSB position
#define MDM_TXPHYRDYHTDELAY_LSB       16
/// TXPHYRDYHTDELAY field width
#define MDM_TXPHYRDYHTDELAY_WIDTH     ((uint32_t)0x0000000C)
/// TXPHYRDYNONHTDELAY field mask
#define MDM_TXPHYRDYNONHTDELAY_MASK   ((uint32_t)0x00000FFF)
/// TXPHYRDYNONHTDELAY field LSB position
#define MDM_TXPHYRDYNONHTDELAY_LSB    0
/// TXPHYRDYNONHTDELAY field width
#define MDM_TXPHYRDYNONHTDELAY_WIDTH  ((uint32_t)0x0000000C)

/// TXPHYRDYHTDELAY field reset value
#define MDM_TXPHYRDYHTDELAY_RST       0x2D0
/// TXPHYRDYNONHTDELAY field reset value
#define MDM_TXPHYRDYNONHTDELAY_RST    0x438

/**
 * @brief Constructs a value for the TXCTRL3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txphyrdyhtdelay - The value to use for the TXPHYRDYHTDELAY field.
 * @param[in] txphyrdynonhtdelay - The value to use for the TXPHYRDYNONHTDELAY field.
 */
__INLINE void mdm_txctrl3_pack(uint16_t txphyrdyhtdelay, uint16_t txphyrdynonhtdelay)
{
    ASSERT_ERR((((uint32_t)txphyrdyhtdelay << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    ASSERT_ERR((((uint32_t)txphyrdynonhtdelay << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(MDM_TXCTRL3_ADDR,  ((uint32_t)txphyrdyhtdelay << 16) | ((uint32_t)txphyrdynonhtdelay << 0));
}

/**
 * @brief Unpacks TXCTRL3's fields from current value of the TXCTRL3 register.
 *
 * Reads the TXCTRL3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txphyrdyhtdelay - Will be populated with the current value of this field from the register.
 * @param[out] txphyrdynonhtdelay - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_txctrl3_unpack(uint16_t* txphyrdyhtdelay, uint16_t* txphyrdynonhtdelay)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL3_ADDR);

    *txphyrdyhtdelay = (localVal & ((uint32_t)0x0FFF0000)) >> 16;
    *txphyrdynonhtdelay = (localVal & ((uint32_t)0x00000FFF)) >> 0;
}

/**
 * @brief Returns the current value of the TXPHYRDYHTDELAY field in the TXCTRL3 register.
 *
 * The TXCTRL3 register will be read and the TXPHYRDYHTDELAY field's value will be returned.
 *
 * @return The current value of the TXPHYRDYHTDELAY field in the TXCTRL3 register.
 */
__INLINE uint16_t mdm_txphyrdyhtdelay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x0FFF0000)) >> 16);
}

/**
 * @brief Sets the TXPHYRDYHTDELAY field of the TXCTRL3 register.
 *
 * The TXCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txphyrdyhtdelay - The value to set the field to.
 */
__INLINE void mdm_txphyrdyhtdelay_setf(uint16_t txphyrdyhtdelay)
{
    ASSERT_ERR((((uint32_t)txphyrdyhtdelay << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    REG_PL_WR(MDM_TXCTRL3_ADDR, (REG_PL_RD(MDM_TXCTRL3_ADDR) & ~((uint32_t)0x0FFF0000)) | ((uint32_t)txphyrdyhtdelay << 16));
}

/**
 * @brief Returns the current value of the TXPHYRDYNONHTDELAY field in the TXCTRL3 register.
 *
 * The TXCTRL3 register will be read and the TXPHYRDYNONHTDELAY field's value will be returned.
 *
 * @return The current value of the TXPHYRDYNONHTDELAY field in the TXCTRL3 register.
 */
__INLINE uint16_t mdm_txphyrdynonhtdelay_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TXCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x00000FFF)) >> 0);
}

/**
 * @brief Sets the TXPHYRDYNONHTDELAY field of the TXCTRL3 register.
 *
 * The TXCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txphyrdynonhtdelay - The value to set the field to.
 */
__INLINE void mdm_txphyrdynonhtdelay_setf(uint16_t txphyrdynonhtdelay)
{
    ASSERT_ERR((((uint32_t)txphyrdynonhtdelay << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(MDM_TXCTRL3_ADDR, (REG_PL_RD(MDM_TXCTRL3_ADDR) & ~((uint32_t)0x00000FFF)) | ((uint32_t)txphyrdynonhtdelay << 0));
}

/// @}

/**
 * @name RXCTRL2 register definitions
 * <table>
 * <caption id="RXCTRL2_BF">RXCTRL2 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:00 <td>RXFRAMEVIOLATIONMASK <td>R <td>R/W <td>0xFFFFFFFF
 * </table>
 *
 * @{
 */

/// Address of the RXCTRL2 register
#define MDM_RXCTRL2_ADDR   (REG_MDM_CFG_BASE_ADDR+0x089C)
/// Offset of the RXCTRL2 register from the base address
#define MDM_RXCTRL2_OFFSET 0x0000009C
/// Index of the RXCTRL2 register
#define MDM_RXCTRL2_INDEX  0x00000027
/// Reset value of the RXCTRL2 register
#define MDM_RXCTRL2_RESET  0xFFFFFFFF

/**
 * @brief Returns the current value of the RXCTRL2 register.
 * The RXCTRL2 register will be read and its value returned.
 * @return The current value of the RXCTRL2 register.
 */
__INLINE uint32_t mdm_rxctrl2_get(void)
{
    return REG_PL_RD(MDM_RXCTRL2_ADDR);
}

/**
 * @brief Sets the RXCTRL2 register to a value.
 * The RXCTRL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxctrl2_set(uint32_t value)
{
    REG_PL_WR(MDM_RXCTRL2_ADDR, value);
}

// field definitions
/// RXFRAMEVIOLATIONMASK field mask
#define MDM_RXFRAMEVIOLATIONMASK_MASK   ((uint32_t)0xFFFFFFFF)
/// RXFRAMEVIOLATIONMASK field LSB position
#define MDM_RXFRAMEVIOLATIONMASK_LSB    0
/// RXFRAMEVIOLATIONMASK field width
#define MDM_RXFRAMEVIOLATIONMASK_WIDTH  ((uint32_t)0x00000020)

/// RXFRAMEVIOLATIONMASK field reset value
#define MDM_RXFRAMEVIOLATIONMASK_RST    0xFFFFFFFF

/**
 * @brief Returns the current value of the RXFRAMEVIOLATIONMASK field in the RXCTRL2 register.
 *
 * The RXCTRL2 register will be read and the RXFRAMEVIOLATIONMASK field's value will be returned.
 *
 * @return The current value of the RXFRAMEVIOLATIONMASK field in the RXCTRL2 register.
 */
__INLINE uint32_t mdm_rxframeviolationmask_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL2_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the RXFRAMEVIOLATIONMASK field of the RXCTRL2 register.
 *
 * The RXCTRL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxframeviolationmask - The value to set the field to.
 */
__INLINE void mdm_rxframeviolationmask_setf(uint32_t rxframeviolationmask)
{
    ASSERT_ERR((((uint32_t)rxframeviolationmask << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MDM_RXCTRL2_ADDR, (uint32_t)rxframeviolationmask << 0);
}

/// @}
#endif
#if RW_NX_LDPC_DEC
/**
 * @name RXCTRL3 register definitions
 * <table>
 * <caption id="RXCTRL3_BF">RXCTRL3 bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:16 <td> RXLDPCNDBPSMAXSGI <td>R <td>R/W <td>0xC30
 * <tr><td>15:00 <td> RXLDPCNDBPSMAXLGI <td>R <td>R/W <td>0xC30
 * </table>
 *
 * @{
 */

/// Address of the RXCTRL3 register
#define MDM_RXCTRL3_ADDR(band)   (REG_MDM_CFG_BASE_ADDR(band)+0x08A0)
/// Offset of the RXCTRL3 register from the base address
#define MDM_RXCTRL3_OFFSET 0x000000A0
/// Index of the RXCTRL3 register
#define MDM_RXCTRL3_INDEX  0x00000028
/// Reset value of the RXCTRL3 register
#define MDM_RXCTRL3_RESET  0x0C300C30

#if 0
/**
 * @brief Returns the current value of the RXCTRL3 register.
 * The RXCTRL3 register will be read and its value returned.
 * @return The current value of the RXCTRL3 register.
 */
__INLINE uint32_t mdm_rxctrl3_get(void)
{
    return REG_PL_RD(MDM_RXCTRL3_ADDR);
}

/**
 * @brief Sets the RXCTRL3 register to a value.
 * The RXCTRL3 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_rxctrl3_set(uint32_t value)
{
    REG_PL_WR(MDM_RXCTRL3_ADDR, value);
}

// field definitions
/// RXLDPCNDBPSMAXSGI field mask
#define MDM_RXLDPCNDBPSMAXSGI_MASK   ((uint32_t)0xFFFF0000)
/// RXLDPCNDBPSMAXSGI field LSB position
#define MDM_RXLDPCNDBPSMAXSGI_LSB    16
/// RXLDPCNDBPSMAXSGI field width
#define MDM_RXLDPCNDBPSMAXSGI_WIDTH  ((uint32_t)0x00000010)
/// RXLDPCNDBPSMAXLGI field mask
#define MDM_RXLDPCNDBPSMAXLGI_MASK   ((uint32_t)0x0000FFFF)
/// RXLDPCNDBPSMAXLGI field LSB position
#define MDM_RXLDPCNDBPSMAXLGI_LSB    0
/// RXLDPCNDBPSMAXLGI field width
#define MDM_RXLDPCNDBPSMAXLGI_WIDTH  ((uint32_t)0x00000010)

/// RXLDPCNDBPSMAXSGI field reset value
#define MDM_RXLDPCNDBPSMAXSGI_RST    0xC30
/// RXLDPCNDBPSMAXLGI field reset value
#define MDM_RXLDPCNDBPSMAXLGI_RST    0xC30

/**
 * @brief Constructs a value for the RXCTRL3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxldpcndbpsmaxsgi - The value to use for the RXLDPCNDBPSMAXSGI field.
 * @param[in] rxldpcndbpsmaxlgi - The value to use for the RXLDPCNDBPSMAXLGI field.
 */
__INLINE void mdm_rxctrl3_pack(uint16_t rxldpcndbpsmaxsgi, uint16_t rxldpcndbpsmaxlgi)
{
    ASSERT_ERR((((uint32_t)rxldpcndbpsmaxsgi << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    ASSERT_ERR((((uint32_t)rxldpcndbpsmaxlgi << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(MDM_RXCTRL3_ADDR,  ((uint32_t)rxldpcndbpsmaxsgi << 16) | ((uint32_t)rxldpcndbpsmaxlgi << 0));
}

/**
 * @brief Unpacks RXCTRL3's fields from current value of the RXCTRL3 register.
 *
 * Reads the RXCTRL3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxldpcndbpsmaxsgi - Will be populated with the current value of this field from the register.
 * @param[out] rxldpcndbpsmaxlgi - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_rxctrl3_unpack(uint16_t* rxldpcndbpsmaxsgi, uint16_t* rxldpcndbpsmaxlgi)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL3_ADDR);

    *rxldpcndbpsmaxsgi = (localVal & ((uint32_t)0xFFFF0000)) >> 16;
    *rxldpcndbpsmaxlgi = (localVal & ((uint32_t)0x0000FFFF)) >> 0;
}

/**
 * @brief Returns the current value of the RXLDPCNDBPSMAXSGI field in the RXCTRL3 register.
 *
 * The RXCTRL3 register will be read and the RXLDPCNDBPSMAXSGI field's value will be returned.
 *
 * @return The current value of the RXLDPCNDBPSMAXSGI field in the RXCTRL3 register.
 */
__INLINE uint16_t mdm_rxldpcndbpsmaxsgi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL3_ADDR);
    return ((localVal & ((uint32_t)0xFFFF0000)) >> 16);
}

/**
 * @brief Sets the RXLDPCNDBPSMAXSGI field of the RXCTRL3 register.
 *
 * The RXCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxldpcndbpsmaxsgi - The value to set the field to.
 */
__INLINE void mdm_rxldpcndbpsmaxsgi_setf(uint16_t rxldpcndbpsmaxsgi)
{
    ASSERT_ERR((((uint32_t)rxldpcndbpsmaxsgi << 16) & ~((uint32_t)0xFFFF0000)) == 0);
    REG_PL_WR(MDM_RXCTRL3_ADDR, (REG_PL_RD(MDM_RXCTRL3_ADDR) & ~((uint32_t)0xFFFF0000)) | ((uint32_t)rxldpcndbpsmaxsgi << 16));
}

/**
 * @brief Returns the current value of the RXLDPCNDBPSMAXLGI field in the RXCTRL3 register.
 *
 * The RXCTRL3 register will be read and the RXLDPCNDBPSMAXLGI field's value will be returned.
 *
 * @return The current value of the RXLDPCNDBPSMAXLGI field in the RXCTRL3 register.
 */
__INLINE uint16_t mdm_rxldpcndbpsmaxlgi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_RXCTRL3_ADDR);
    return ((localVal & ((uint32_t)0x0000FFFF)) >> 0);
}

/**
 * @brief Sets the RXLDPCNDBPSMAXLGI field of the RXCTRL3 register.
 *
 * The RXCTRL3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxldpcndbpsmaxlgi - The value to set the field to.
 */
__INLINE void mdm_rxldpcndbpsmaxlgi_setf(uint16_t rxldpcndbpsmaxlgi)
{
    ASSERT_ERR((((uint32_t)rxldpcndbpsmaxlgi << 0) & ~((uint32_t)0x0000FFFF)) == 0);
    REG_PL_WR(MDM_RXCTRL3_ADDR, (REG_PL_RD(MDM_RXCTRL3_ADDR) & ~((uint32_t)0x0000FFFF)) | ((uint32_t)rxldpcndbpsmaxlgi << 0));
}
#endif
#endif // RW_NX_LDPC_DEC
/// @}

#if 0
#if RW_BFMEE_EN
/**
 * @name SVDCTRL register definitions
 * <table>
 * <caption id="SVDCTRL_BF">SVDCTRL bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>23:20 <td>          RXANTSEL <td>R <td>R/W <td>0x3
 * <tr><td>09:00 <td>        SNRSCALING <td>R <td>R/W <td>0x56
 * </table>
 *
 * @{
 */

/// Address of the SVDCTRL register
#define MDM_SVDCTRL_ADDR   (REG_MDM_CFG_BASE_ADDR+0x08A4)
/// Offset of the SVDCTRL register from the base address
#define MDM_SVDCTRL_OFFSET 0x000000A4
/// Index of the SVDCTRL register
#define MDM_SVDCTRL_INDEX  0x00000029
/// Reset value of the SVDCTRL register
#define MDM_SVDCTRL_RESET  0x00300056

/**
 * @brief Returns the current value of the SVDCTRL register.
 * The SVDCTRL register will be read and its value returned.
 * @return The current value of the SVDCTRL register.
 */
__INLINE uint32_t mdm_svdctrl_get(void)
{
    return REG_PL_RD(MDM_SVDCTRL_ADDR);
}

/**
 * @brief Sets the SVDCTRL register to a value.
 * The SVDCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_svdctrl_set(uint32_t value)
{
    REG_PL_WR(MDM_SVDCTRL_ADDR, value);
}

// field definitions
/// RXANTSEL field mask
#define MDM_RXANTSEL_MASK     ((uint32_t)0x00F00000)
/// RXANTSEL field LSB position
#define MDM_RXANTSEL_LSB      20
/// RXANTSEL field width
#define MDM_RXANTSEL_WIDTH    ((uint32_t)0x00000004)
/// SNRSCALING field mask
#define MDM_SNRSCALING_MASK   ((uint32_t)0x000003FF)
/// SNRSCALING field LSB position
#define MDM_SNRSCALING_LSB    0
/// SNRSCALING field width
#define MDM_SNRSCALING_WIDTH  ((uint32_t)0x0000000A)

/// RXANTSEL field reset value
#define MDM_RXANTSEL_RST      0x3
/// SNRSCALING field reset value
#define MDM_SNRSCALING_RST    0x56

/**
 * @brief Constructs a value for the SVDCTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxantsel - The value to use for the RXANTSEL field.
 * @param[in] snrscaling - The value to use for the SNRSCALING field.
 */
__INLINE void mdm_svdctrl_pack(uint8_t rxantsel, uint16_t snrscaling)
{
    ASSERT_ERR((((uint32_t)rxantsel << 20) & ~((uint32_t)0x00F00000)) == 0);
    ASSERT_ERR((((uint32_t)snrscaling << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(MDM_SVDCTRL_ADDR,  ((uint32_t)rxantsel << 20) | ((uint32_t)snrscaling << 0));
}

/**
 * @brief Unpacks SVDCTRL's fields from current value of the SVDCTRL register.
 *
 * Reads the SVDCTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxantsel - Will be populated with the current value of this field from the register.
 * @param[out] snrscaling - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_svdctrl_unpack(uint8_t* rxantsel, uint16_t* snrscaling)
{
    uint32_t localVal = REG_PL_RD(MDM_SVDCTRL_ADDR);

    *rxantsel = (localVal & ((uint32_t)0x00F00000)) >> 20;
    *snrscaling = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the RXANTSEL field in the SVDCTRL register.
 *
 * The SVDCTRL register will be read and the RXANTSEL field's value will be returned.
 *
 * @return The current value of the RXANTSEL field in the SVDCTRL register.
 */
__INLINE uint8_t mdm_rxantsel_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SVDCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00F00000)) >> 20);
}

/**
 * @brief Sets the RXANTSEL field of the SVDCTRL register.
 *
 * The SVDCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxantsel - The value to set the field to.
 */
__INLINE void mdm_rxantsel_setf(uint8_t rxantsel)
{
    ASSERT_ERR((((uint32_t)rxantsel << 20) & ~((uint32_t)0x00F00000)) == 0);
    REG_PL_WR(MDM_SVDCTRL_ADDR, (REG_PL_RD(MDM_SVDCTRL_ADDR) & ~((uint32_t)0x00F00000)) | ((uint32_t)rxantsel << 20));
}

/**
 * @brief Returns the current value of the SNRSCALING field in the SVDCTRL register.
 *
 * The SVDCTRL register will be read and the SNRSCALING field's value will be returned.
 *
 * @return The current value of the SNRSCALING field in the SVDCTRL register.
 */
__INLINE uint16_t mdm_snrscaling_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_SVDCTRL_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the SNRSCALING field of the SVDCTRL register.
 *
 * The SVDCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] snrscaling - The value to set the field to.
 */
__INLINE void mdm_snrscaling_setf(uint16_t snrscaling)
{
    ASSERT_ERR((((uint32_t)snrscaling << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(MDM_SVDCTRL_ADDR, (REG_PL_RD(MDM_SVDCTRL_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)snrscaling << 0));
}

#endif // RW_BFMEE_EN
/// @}

#if RW_MUMIMO_RX_EN
/**
 * @name MUMIMO_GROUPID_TAB register definitions
 * <table>
 * <caption id="MUMIMO_GROUPID_TAB_BF">MUMIMO_GROUPID_TAB bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:00 <td>    MEMBERSHIPSTAT <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the MUMIMO_GROUPID_TAB register
#define MDM_MUMIMO_GROUPID_TAB_ADDR   (REG_MDM_CFG_BASE_ADDR+0x08A8)
/// Offset of the MUMIMO_GROUPID_TAB register from the base address
#define MDM_MUMIMO_GROUPID_TAB_OFFSET 0x000000A8
/// Index of the MUMIMO_GROUPID_TAB register
#define MDM_MUMIMO_GROUPID_TAB_INDEX  0x0000002A
/// Reset value of the MUMIMO_GROUPID_TAB register
#define MDM_MUMIMO_GROUPID_TAB_RESET  0x00000000
/// Number of elements of the MUMIMO_GROUPID_TAB register array
#define MDM_MUMIMO_GROUPID_TAB_COUNT  2

/**
 * @brief Returns the current value of the MUMIMO_GROUPID_TAB register.
 * The MUMIMO_GROUPID_TAB register will be read and its value returned.
 * @param[in] reg_idx Index of the register
 * @return The current value of the MUMIMO_GROUPID_TAB register.
 */
__INLINE uint32_t mdm_mumimo_groupid_tab_get(int reg_idx)
{
    ASSERT_ERR(reg_idx <= 1);
    return REG_PL_RD(MDM_MUMIMO_GROUPID_TAB_ADDR + reg_idx * 4);
}

/**
 * @brief Sets the MUMIMO_GROUPID_TAB register to a value.
 * The MUMIMO_GROUPID_TAB register will be written.
 * @param[in] reg_idx Index of the register
 * @param value - The value to write.
 */
__INLINE void mdm_mumimo_groupid_tab_set(int reg_idx, uint32_t value)
{
    ASSERT_ERR(reg_idx <= 1);
    REG_PL_WR(MDM_MUMIMO_GROUPID_TAB_ADDR + reg_idx * 4, value);
}

// field definitions
/// MEMBERSHIPSTAT field mask
#define MDM_MEMBERSHIPSTAT_MASK   ((uint32_t)0xFFFFFFFF)
/// MEMBERSHIPSTAT field LSB position
#define MDM_MEMBERSHIPSTAT_LSB    0
/// MEMBERSHIPSTAT field width
#define MDM_MEMBERSHIPSTAT_WIDTH  ((uint32_t)0x00000020)

/// MEMBERSHIPSTAT field reset value
#define MDM_MEMBERSHIPSTAT_RST    0x0

/**
 * @brief Returns the current value of the MEMBERSHIPSTAT field in the MUMIMO_GROUPID_TAB register.
 *
 * The MUMIMO_GROUPID_TAB register will be read and the MEMBERSHIPSTAT field's value will be returned.
 *
 * @param[in] reg_idx Index of the register
 * @return The current value of the MEMBERSHIPSTAT field in the MUMIMO_GROUPID_TAB register.
 */
__INLINE uint32_t mdm_membershipstat_getf(int reg_idx)
{
    uint32_t localVal = REG_PL_RD(MDM_MUMIMO_GROUPID_TAB_ADDR + reg_idx * 4);
    ASSERT_ERR(reg_idx <= 1);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the MEMBERSHIPSTAT field of the MUMIMO_GROUPID_TAB register.
 *
 * The MUMIMO_GROUPID_TAB register will be read, modified to contain the new field value, and written.
 *
 * @param[in] reg_idx Index of the register
 * @param[in] membershipstat - The value to set the field to.
 */
__INLINE void mdm_membershipstat_setf(int reg_idx, uint32_t membershipstat)
{
    ASSERT_ERR(reg_idx <= 1);
    ASSERT_ERR((((uint32_t)membershipstat << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MDM_MUMIMO_GROUPID_TAB_ADDR + reg_idx * 4, (uint32_t)membershipstat << 0);
}

#endif // RW_MUMIMO_RX_EN
/// @}

#if RW_MUMIMO_RX_EN
/**
 * @name MUMIMO_USERPOSITION_TAB register definitions
 * <table>
 * <caption id="MUMIMO_USERPOSITION_TAB_BF">MUMIMO_USERPOSITION_TAB bitfields</caption>
 * <tr><th>Bits <th>Field Name <th>HW Access <th>SW Access <th>Reset Value
 * <tr><td>31:00 <td>         USERPOSID <td>R <td>R/W <td>0x0
 * </table>
 *
 * @{
 */

/// Address of the MUMIMO_USERPOSITION_TAB register
#define MDM_MUMIMO_USERPOSITION_TAB_ADDR   (REG_MDM_CFG_BASE_ADDR+0x08B0)
/// Offset of the MUMIMO_USERPOSITION_TAB register from the base address
#define MDM_MUMIMO_USERPOSITION_TAB_OFFSET 0x000000B0
/// Index of the MUMIMO_USERPOSITION_TAB register
#define MDM_MUMIMO_USERPOSITION_TAB_INDEX  0x0000002C
/// Reset value of the MUMIMO_USERPOSITION_TAB register
#define MDM_MUMIMO_USERPOSITION_TAB_RESET  0x00000000
/// Number of elements of the MUMIMO_USERPOSITION_TAB register array
#define MDM_MUMIMO_USERPOSITION_TAB_COUNT  4

/**
 * @brief Returns the current value of the MUMIMO_USERPOSITION_TAB register.
 * The MUMIMO_USERPOSITION_TAB register will be read and its value returned.
 * @param[in] reg_idx Index of the register
 * @return The current value of the MUMIMO_USERPOSITION_TAB register.
 */
__INLINE uint32_t mdm_mumimo_userposition_tab_get(int reg_idx)
{
    ASSERT_ERR(reg_idx <= 3);
    return REG_PL_RD(MDM_MUMIMO_USERPOSITION_TAB_ADDR + reg_idx * 4);
}

/**
 * @brief Sets the MUMIMO_USERPOSITION_TAB register to a value.
 * The MUMIMO_USERPOSITION_TAB register will be written.
 * @param[in] reg_idx Index of the register
 * @param value - The value to write.
 */
__INLINE void mdm_mumimo_userposition_tab_set(int reg_idx, uint32_t value)
{
    ASSERT_ERR(reg_idx <= 3);
    REG_PL_WR(MDM_MUMIMO_USERPOSITION_TAB_ADDR + reg_idx * 4, value);
}

// field definitions
/// USERPOSID field mask
#define MDM_USERPOSID_MASK   ((uint32_t)0xFFFFFFFF)
/// USERPOSID field LSB position
#define MDM_USERPOSID_LSB    0
/// USERPOSID field width
#define MDM_USERPOSID_WIDTH  ((uint32_t)0x00000020)

/// USERPOSID field reset value
#define MDM_USERPOSID_RST    0x0

/**
 * @brief Returns the current value of the USERPOSID field in the MUMIMO_USERPOSITION_TAB register.
 *
 * The MUMIMO_USERPOSITION_TAB register will be read and the USERPOSID field's value will be returned.
 *
 * @param[in] reg_idx Index of the register
 * @return The current value of the USERPOSID field in the MUMIMO_USERPOSITION_TAB register.
 */
__INLINE uint32_t mdm_userposid_getf(int reg_idx)
{
    uint32_t localVal = REG_PL_RD(MDM_MUMIMO_USERPOSITION_TAB_ADDR + reg_idx * 4);
    ASSERT_ERR(reg_idx <= 3);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the USERPOSID field of the MUMIMO_USERPOSITION_TAB register.
 *
 * The MUMIMO_USERPOSITION_TAB register will be read, modified to contain the new field value, and written.
 *
 * @param[in] reg_idx Index of the register
 * @param[in] userposid - The value to set the field to.
 */
__INLINE void mdm_userposid_setf(int reg_idx, uint32_t userposid)
{
    ASSERT_ERR(reg_idx <= 3);
    ASSERT_ERR((((uint32_t)userposid << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(MDM_MUMIMO_USERPOSITION_TAB_ADDR + reg_idx * 4, (uint32_t)userposid << 0);
}

#endif // RW_MUMIMO_RX_EN
/// @}
#endif

#endif // _REG_MDM_CFG_H_

/// @}
