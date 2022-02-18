/**
 * @file reg_riu.h
 * @brief Definitions of the RIU HW block registers and register access functions.
 *
 * @defgroup REG_RIU REG_RIU
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the RIU HW block registers and register access functions.
 */
#ifndef _REG_RIU_H_
#define _REG_RIU_H_

#include "co_int.h"
#include "_reg_riu.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_RIU peripheral.
 */
#define REG_RIU_COUNT 321

/** @brief Decoding mask of the REG_RIU peripheral registers from the CPU point of view.
 */
#define REG_RIU_DECODING_MASK 0x000007FF

/**
 * @name RWNXVERSION register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00              VERSION   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXVERSION register
#define RIU_RWNXVERSION_ADDR   (REG_RIU_BASE_ADDR+0xB000)
/// Offset of the RWNXVERSION register from the base address
#define RIU_RWNXVERSION_OFFSET 0x00000000
/// Index of the RWNXVERSION register
#define RIU_RWNXVERSION_INDEX  0x00000000
/// Reset value of the RWNXVERSION register
///#define RIU_RWNXVERSION_RESET  0x00000000
///#define RIU_RWNXVERSION_RESET  0x11010101
#define RIU_RWNXVERSION_RESET 0x00956111
/**
 * @brief Returns the current value of the RWNXVERSION register.
 * The RWNXVERSION register will be read and its value returned.
 * @return The current value of the RWNXVERSION register.
 */
__INLINE uint32_t riu_rwnxversion_get(void)
{
    return REG_PL_RD(RIU_RWNXVERSION_ADDR);
}

// field definitions
/// VERSION field mask
#define RIU_VERSION_MASK   ((uint32_t)0xFFFFFFFF)
/// VERSION field LSB position
#define RIU_VERSION_LSB    0
/// VERSION field width
#define RIU_VERSION_WIDTH  ((uint32_t)0x00000020)

/// VERSION field reset value
#define RIU_VERSION_RST    0x0

/**
 * @brief Returns the current value of the VERSION field in the RWNXVERSION register.
 *
 * The RWNXVERSION register will be read and the VERSION field's value will be returned.
 *
 * @return The current value of the VERSION field in the RWNXVERSION register.
 */
__INLINE uint32_t riu_version_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXVERSION_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name RWNXSTATICCONFIG register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  01:00            ACTIVEANT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXSTATICCONFIG register
#define RIU_RWNXSTATICCONFIG_ADDR   (REG_RIU_BASE_ADDR+0xB004)
/// Offset of the RWNXSTATICCONFIG register from the base address
#define RIU_RWNXSTATICCONFIG_OFFSET 0x00000004
/// Index of the RWNXSTATICCONFIG register
#define RIU_RWNXSTATICCONFIG_INDEX  0x00000001
/// Reset value of the RWNXSTATICCONFIG register
#define RIU_RWNXSTATICCONFIG_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXSTATICCONFIG register.
 * The RWNXSTATICCONFIG register will be read and its value returned.
 * @return The current value of the RWNXSTATICCONFIG register.
 */
__INLINE uint32_t riu_rwnxstaticconfig_get(void)
{
    return REG_PL_RD(RIU_RWNXSTATICCONFIG_ADDR);
}

/**
 * @brief Sets the RWNXSTATICCONFIG register to a value.
 * The RWNXSTATICCONFIG register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxstaticconfig_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXSTATICCONFIG_ADDR, value);
}

// field definitions
/// ACTIVEANT field mask
#define RIU_ACTIVEANT_MASK   ((uint32_t)0x00000003)
/// ACTIVEANT field LSB position
#define RIU_ACTIVEANT_LSB    0
/// ACTIVEANT field width
#define RIU_ACTIVEANT_WIDTH  ((uint32_t)0x00000002)

/// ACTIVEANT field reset value
#define RIU_ACTIVEANT_RST    0x0

/**
 * @brief Returns the current value of the ACTIVEANT field in the RWNXSTATICCONFIG register.
 *
 * The RWNXSTATICCONFIG register will be read and the ACTIVEANT field's value will be returned.
 *
 * @return The current value of the ACTIVEANT field in the RWNXSTATICCONFIG register.
 */
__INLINE uint8_t riu_activeant_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXSTATICCONFIG_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000003)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the ACTIVEANT field of the RWNXSTATICCONFIG register.
 *
 * The RWNXSTATICCONFIG register will be read, modified to contain the new field value, and written.
 *
 * @param[in] activeant - The value to set the field to.
 */
__INLINE void riu_activeant_setf(uint8_t activeant)
{
    ASSERT_ERR((((uint32_t)activeant << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(RIU_RWNXSTATICCONFIG_ADDR, (uint32_t)activeant << 0);
}

/// @}

/**
 * @name RWNXDYNAMICCONFIG register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  no use this bit   28      AGCMEMBISTSTART   0
 *     16   AGCINBDPOWSTARTCAPTURE   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXDYNAMICCONFIG register
#define RIU_RWNXDYNAMICCONFIG_ADDR   (REG_RIU_BASE_ADDR+0xB008)
/// Offset of the RWNXDYNAMICCONFIG register from the base address
#define RIU_RWNXDYNAMICCONFIG_OFFSET 0x00000008
/// Index of the RWNXDYNAMICCONFIG register
#define RIU_RWNXDYNAMICCONFIG_INDEX  0x00000002
/// Reset value of the RWNXDYNAMICCONFIG register
#define RIU_RWNXDYNAMICCONFIG_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXDYNAMICCONFIG register.
 * The RWNXDYNAMICCONFIG register will be read and its value returned.
 * @return The current value of the RWNXDYNAMICCONFIG register.
 */
__INLINE uint32_t riu_rwnxdynamicconfig_get(void)
{
    return REG_PL_RD(RIU_RWNXDYNAMICCONFIG_ADDR);
}

/**
 * @brief Sets the RWNXDYNAMICCONFIG register to a value.
 * The RWNXDYNAMICCONFIG register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxdynamicconfig_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXDYNAMICCONFIG_ADDR, value);
}

// field definitions
//No use bit 28
/// AGCMEMBISTSTART field bit
///#define RIU_AGCMEMBISTSTART_BIT           ((uint32_t)0x10000000)
/// AGCMEMBISTSTART field position
///#define RIU_AGCMEMBISTSTART_POS           28
/// AGCINBDPOWSTARTCAPTURE field bit
#define RIU_AGCINBDPOWSTARTCAPTURE_BIT    ((uint32_t)0x00010000)
/// AGCINBDPOWSTARTCAPTURE field position
#define RIU_AGCINBDPOWSTARTCAPTURE_POS    16

/// AGCMEMBISTSTART field reset value
#define RIU_AGCMEMBISTSTART_RST           0x0
/// AGCINBDPOWSTARTCAPTURE field reset value
#define RIU_AGCINBDPOWSTARTCAPTURE_RST    0x0

#if 0
/**
 * @brief Constructs a value for the RWNXDYNAMICCONFIG register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] agcmembiststart - The value to use for the AGCMEMBISTSTART field.
 * @param[in] agcinbdpowstartcapture - The value to use for the AGCINBDPOWSTARTCAPTURE field.
 */
__INLINE void riu_rwnxdynamicconfig_pack(uint8_t agcmembiststart, uint8_t agcinbdpowstartcapture)
{
    ASSERT_ERR((((uint32_t)agcmembiststart << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)agcinbdpowstartcapture << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXDYNAMICCONFIG_ADDR,  ((uint32_t)agcmembiststart << 28) | ((uint32_t)agcinbdpowstartcapture << 16));
}

/**
 * @brief Unpacks RWNXDYNAMICCONFIG's fields from current value of the RWNXDYNAMICCONFIG register.
 *
 * Reads the RWNXDYNAMICCONFIG register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcmembiststart - Will be populated with the current value of this field from the register.
 * @param[out] agcinbdpowstartcapture - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxdynamicconfig_unpack(uint8_t* agcmembiststart, uint8_t* agcinbdpowstartcapture)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXDYNAMICCONFIG_ADDR);

    *agcmembiststart = (localVal & ((uint32_t)0x10000000)) >> 28;
    *agcinbdpowstartcapture = (localVal & ((uint32_t)0x00010000)) >> 16;
}

/**
 * @brief Returns the current value of the AGCMEMBISTSTART field in the RWNXDYNAMICCONFIG register.
 *
 * The RWNXDYNAMICCONFIG register will be read and the AGCMEMBISTSTART field's value will be returned.
 *
 * @return The current value of the AGCMEMBISTSTART field in the RWNXDYNAMICCONFIG register.
 */
__INLINE uint8_t riu_agcmembiststart_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXDYNAMICCONFIG_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the AGCMEMBISTSTART field of the RWNXDYNAMICCONFIG register.
 *
 * The RWNXDYNAMICCONFIG register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcmembiststart - The value to set the field to.
 */
__INLINE void riu_agcmembiststart_setf(uint8_t agcmembiststart)
{
    ASSERT_ERR((((uint32_t)agcmembiststart << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(RIU_RWNXDYNAMICCONFIG_ADDR, (uint32_t)agcmembiststart << 28);
}
#endif
/**
 * @brief Returns the current value of the AGCINBDPOWSTARTCAPTURE field in the RWNXDYNAMICCONFIG register.
 *
 * The RWNXDYNAMICCONFIG register will be read and the AGCINBDPOWSTARTCAPTURE field's value will be returned.
 *
 * @return The current value of the AGCINBDPOWSTARTCAPTURE field in the RWNXDYNAMICCONFIG register.
 */
__INLINE uint8_t riu_agcinbdpowstartcapture_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXDYNAMICCONFIG_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the AGCINBDPOWSTARTCAPTURE field of the RWNXDYNAMICCONFIG register.
 *
 * The RWNXDYNAMICCONFIG register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcinbdpowstartcapture - The value to set the field to.
 */
__INLINE void riu_agcinbdpowstartcapture_setf(uint8_t agcinbdpowstartcapture)
{
    ASSERT_ERR((((uint32_t)agcinbdpowstartcapture << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXDYNAMICCONFIG_ADDR, (uint32_t)agcinbdpowstartcapture << 16);
}

/// @}

/**
 * @name RWNXIQCTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     28        TXIQSWAPPATH1   0
 *     27        TXIQSWAPPATH0   0
 *     23        RXIQSWAPPATH1   0
 *     22        RXIQSWAPPATH0   0
 *     15        TXIQDELPATHQ1   0
 *     14        TXIQDELPATHI1   0
 *     13        TXIQDELPATHQ0   0
 *     12        TXIQDELPATHI0   0
 *     05        RXIQDELPATHQ1   0
 *     04        RXIQDELPATHI1   0
 *     03        RXIQDELPATHQ0   0
 *     02        RXIQDELPATHI0   0
 *     01         TXC2SDISBCFG   0
 *     00         RXC2SDISBCFG   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXIQCTRL register
#define RIU_RWNXIQCTRL_ADDR   (REG_RIU_BASE_ADDR+0xB01C)
/// Offset of the RWNXIQCTRL register from the base address
#define RIU_RWNXIQCTRL_OFFSET 0x0000001C
/// Index of the RWNXIQCTRL register
#define RIU_RWNXIQCTRL_INDEX  0x00000007
/// Reset value of the RWNXIQCTRL register
#define RIU_RWNXIQCTRL_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXIQCTRL register.
 * The RWNXIQCTRL register will be read and its value returned.
 * @return The current value of the RWNXIQCTRL register.
 */
__INLINE uint32_t riu_rwnxiqctrl_get(void)
{
    return REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
}

/**
 * @brief Sets the RWNXIQCTRL register to a value.
 * The RWNXIQCTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxiqctrl_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, value);
}

// field definitions
/// TXIQSWAPPATH1 field bit
#define RIU_TXIQSWAPPATH1_BIT    ((uint32_t)0x10000000)
/// TXIQSWAPPATH1 field position
#define RIU_TXIQSWAPPATH1_POS    28
/// TXIQSWAPPATH0 field bit
#define RIU_TXIQSWAPPATH0_BIT    ((uint32_t)0x08000000)
/// TXIQSWAPPATH0 field position
#define RIU_TXIQSWAPPATH0_POS    27
/// RXIQSWAPPATH1 field bit
#define RIU_RXIQSWAPPATH1_BIT    ((uint32_t)0x00800000)
/// RXIQSWAPPATH1 field position
#define RIU_RXIQSWAPPATH1_POS    23
/// RXIQSWAPPATH0 field bit
#define RIU_RXIQSWAPPATH0_BIT    ((uint32_t)0x00400000)
/// RXIQSWAPPATH0 field position
#define RIU_RXIQSWAPPATH0_POS    22
/// TXIQDELPATHQ1 field bit
#define RIU_TXIQDELPATHQ1_BIT    ((uint32_t)0x00008000)
/// TXIQDELPATHQ1 field position
#define RIU_TXIQDELPATHQ1_POS    15
/// TXIQDELPATHI1 field bit
#define RIU_TXIQDELPATHI1_BIT    ((uint32_t)0x00004000)
/// TXIQDELPATHI1 field position
#define RIU_TXIQDELPATHI1_POS    14
/// TXIQDELPATHQ0 field bit
#define RIU_TXIQDELPATHQ0_BIT    ((uint32_t)0x00002000)
/// TXIQDELPATHQ0 field position
#define RIU_TXIQDELPATHQ0_POS    13
/// TXIQDELPATHI0 field bit
#define RIU_TXIQDELPATHI0_BIT    ((uint32_t)0x00001000)
/// TXIQDELPATHI0 field position
#define RIU_TXIQDELPATHI0_POS    12
/// RXIQDELPATHQ1 field bit
#define RIU_RXIQDELPATHQ1_BIT    ((uint32_t)0x00000020)
/// RXIQDELPATHQ1 field position
#define RIU_RXIQDELPATHQ1_POS    5
/// RXIQDELPATHI1 field bit
#define RIU_RXIQDELPATHI1_BIT    ((uint32_t)0x00000010)
/// RXIQDELPATHI1 field position
#define RIU_RXIQDELPATHI1_POS    4
/// RXIQDELPATHQ0 field bit
#define RIU_RXIQDELPATHQ0_BIT    ((uint32_t)0x00000008)
/// RXIQDELPATHQ0 field position
#define RIU_RXIQDELPATHQ0_POS    3
/// RXIQDELPATHI0 field bit
#define RIU_RXIQDELPATHI0_BIT    ((uint32_t)0x00000004)
/// RXIQDELPATHI0 field position
#define RIU_RXIQDELPATHI0_POS    2
/// TXC2SDISBCFG field bit
#define RIU_TXC2SDISBCFG_BIT     ((uint32_t)0x00000002)
/// TXC2SDISBCFG field position
#define RIU_TXC2SDISBCFG_POS     1
/// RXC2SDISBCFG field bit
#define RIU_RXC2SDISBCFG_BIT     ((uint32_t)0x00000001)
/// RXC2SDISBCFG field position
#define RIU_RXC2SDISBCFG_POS     0

/// TXIQSWAPPATH1 field reset value
#define RIU_TXIQSWAPPATH1_RST    0x0
/// TXIQSWAPPATH0 field reset value
#define RIU_TXIQSWAPPATH0_RST    0x0
/// RXIQSWAPPATH1 field reset value
#define RIU_RXIQSWAPPATH1_RST    0x0
/// RXIQSWAPPATH0 field reset value
#define RIU_RXIQSWAPPATH0_RST    0x0
/// TXIQDELPATHQ1 field reset value
#define RIU_TXIQDELPATHQ1_RST    0x0
/// TXIQDELPATHI1 field reset value
#define RIU_TXIQDELPATHI1_RST    0x0
/// TXIQDELPATHQ0 field reset value
#define RIU_TXIQDELPATHQ0_RST    0x0
/// TXIQDELPATHI0 field reset value
#define RIU_TXIQDELPATHI0_RST    0x0
/// RXIQDELPATHQ1 field reset value
#define RIU_RXIQDELPATHQ1_RST    0x0
/// RXIQDELPATHI1 field reset value
#define RIU_RXIQDELPATHI1_RST    0x0
/// RXIQDELPATHQ0 field reset value
#define RIU_RXIQDELPATHQ0_RST    0x0
/// RXIQDELPATHI0 field reset value
#define RIU_RXIQDELPATHI0_RST    0x0
/// TXC2SDISBCFG field reset value
#define RIU_TXC2SDISBCFG_RST     0x0
/// RXC2SDISBCFG field reset value
#define RIU_RXC2SDISBCFG_RST     0x0

/**
 * @brief Constructs a value for the RWNXIQCTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txiqswappath1 - The value to use for the TXIQSWAPPATH1 field.
 * @param[in] txiqswappath0 - The value to use for the TXIQSWAPPATH0 field.
 * @param[in] rxiqswappath1 - The value to use for the RXIQSWAPPATH1 field.
 * @param[in] rxiqswappath0 - The value to use for the RXIQSWAPPATH0 field.
 * @param[in] txiqdelpathq1 - The value to use for the TXIQDELPATHQ1 field.
 * @param[in] txiqdelpathi1 - The value to use for the TXIQDELPATHI1 field.
 * @param[in] txiqdelpathq0 - The value to use for the TXIQDELPATHQ0 field.
 * @param[in] txiqdelpathi0 - The value to use for the TXIQDELPATHI0 field.
 * @param[in] rxiqdelpathq1 - The value to use for the RXIQDELPATHQ1 field.
 * @param[in] rxiqdelpathi1 - The value to use for the RXIQDELPATHI1 field.
 * @param[in] rxiqdelpathq0 - The value to use for the RXIQDELPATHQ0 field.
 * @param[in] rxiqdelpathi0 - The value to use for the RXIQDELPATHI0 field.
 * @param[in] txc2sdisbcfg - The value to use for the TXC2SDISBCFG field.
 * @param[in] rxc2sdisbcfg - The value to use for the RXC2SDISBCFG field.
 */
__INLINE void riu_rwnxiqctrl_pack(uint8_t txiqswappath1, uint8_t txiqswappath0, uint8_t rxiqswappath1, uint8_t rxiqswappath0, uint8_t txiqdelpathq1, uint8_t txiqdelpathi1, uint8_t txiqdelpathq0, uint8_t txiqdelpathi0, uint8_t rxiqdelpathq1, uint8_t rxiqdelpathi1, uint8_t rxiqdelpathq0, uint8_t rxiqdelpathi0, uint8_t txc2sdisbcfg, uint8_t rxc2sdisbcfg)
{
    ASSERT_ERR((((uint32_t)txiqswappath1 << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)txiqswappath0 << 27) & ~((uint32_t)0x08000000)) == 0);
    ASSERT_ERR((((uint32_t)rxiqswappath1 << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)rxiqswappath0 << 22) & ~((uint32_t)0x00400000)) == 0);
    ASSERT_ERR((((uint32_t)txiqdelpathq1 << 15) & ~((uint32_t)0x00008000)) == 0);
    ASSERT_ERR((((uint32_t)txiqdelpathi1 << 14) & ~((uint32_t)0x00004000)) == 0);
    ASSERT_ERR((((uint32_t)txiqdelpathq0 << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)txiqdelpathi0 << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)rxiqdelpathq1 << 5) & ~((uint32_t)0x00000020)) == 0);
    ASSERT_ERR((((uint32_t)rxiqdelpathi1 << 4) & ~((uint32_t)0x00000010)) == 0);
    ASSERT_ERR((((uint32_t)rxiqdelpathq0 << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)rxiqdelpathi0 << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)txc2sdisbcfg << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)rxc2sdisbcfg << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR,  ((uint32_t)txiqswappath1 << 28) | ((uint32_t)txiqswappath0 << 27) | ((uint32_t)rxiqswappath1 << 23) | ((uint32_t)rxiqswappath0 << 22) | ((uint32_t)txiqdelpathq1 << 15) | ((uint32_t)txiqdelpathi1 << 14) | ((uint32_t)txiqdelpathq0 << 13) | ((uint32_t)txiqdelpathi0 << 12) | ((uint32_t)rxiqdelpathq1 << 5) | ((uint32_t)rxiqdelpathi1 << 4) | ((uint32_t)rxiqdelpathq0 << 3) | ((uint32_t)rxiqdelpathi0 << 2) | ((uint32_t)txc2sdisbcfg << 1) | ((uint32_t)rxc2sdisbcfg << 0));
}

/**
 * @brief Unpacks RWNXIQCTRL's fields from current value of the RWNXIQCTRL register.
 *
 * Reads the RWNXIQCTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txiqswappath1 - Will be populated with the current value of this field from the register.
 * @param[out] txiqswappath0 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqswappath1 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqswappath0 - Will be populated with the current value of this field from the register.
 * @param[out] txiqdelpathq1 - Will be populated with the current value of this field from the register.
 * @param[out] txiqdelpathi1 - Will be populated with the current value of this field from the register.
 * @param[out] txiqdelpathq0 - Will be populated with the current value of this field from the register.
 * @param[out] txiqdelpathi0 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqdelpathq1 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqdelpathi1 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqdelpathq0 - Will be populated with the current value of this field from the register.
 * @param[out] rxiqdelpathi0 - Will be populated with the current value of this field from the register.
 * @param[out] txc2sdisbcfg - Will be populated with the current value of this field from the register.
 * @param[out] rxc2sdisbcfg - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxiqctrl_unpack(uint8_t* txiqswappath1, uint8_t* txiqswappath0, uint8_t* rxiqswappath1, uint8_t* rxiqswappath0, uint8_t* txiqdelpathq1, uint8_t* txiqdelpathi1, uint8_t* txiqdelpathq0, uint8_t* txiqdelpathi0, uint8_t* rxiqdelpathq1, uint8_t* rxiqdelpathi1, uint8_t* rxiqdelpathq0, uint8_t* rxiqdelpathi0, uint8_t* txc2sdisbcfg, uint8_t* rxc2sdisbcfg)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);

    *txiqswappath1 = (localVal & ((uint32_t)0x10000000)) >> 28;
    *txiqswappath0 = (localVal & ((uint32_t)0x08000000)) >> 27;
    *rxiqswappath1 = (localVal & ((uint32_t)0x00800000)) >> 23;
    *rxiqswappath0 = (localVal & ((uint32_t)0x00400000)) >> 22;
    *txiqdelpathq1 = (localVal & ((uint32_t)0x00008000)) >> 15;
    *txiqdelpathi1 = (localVal & ((uint32_t)0x00004000)) >> 14;
    *txiqdelpathq0 = (localVal & ((uint32_t)0x00002000)) >> 13;
    *txiqdelpathi0 = (localVal & ((uint32_t)0x00001000)) >> 12;
    *rxiqdelpathq1 = (localVal & ((uint32_t)0x00000020)) >> 5;
    *rxiqdelpathi1 = (localVal & ((uint32_t)0x00000010)) >> 4;
    *rxiqdelpathq0 = (localVal & ((uint32_t)0x00000008)) >> 3;
    *rxiqdelpathi0 = (localVal & ((uint32_t)0x00000004)) >> 2;
    *txc2sdisbcfg = (localVal & ((uint32_t)0x00000002)) >> 1;
    *rxc2sdisbcfg = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TXIQSWAPPATH1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQSWAPPATH1 field's value will be returned.
 *
 * @return The current value of the TXIQSWAPPATH1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqswappath1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the TXIQSWAPPATH1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqswappath1 - The value to set the field to.
 */
__INLINE void riu_txiqswappath1_setf(uint8_t txiqswappath1)
{
    ASSERT_ERR((((uint32_t)txiqswappath1 << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)txiqswappath1 << 28));
}

/**
 * @brief Returns the current value of the TXIQSWAPPATH0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQSWAPPATH0 field's value will be returned.
 *
 * @return The current value of the TXIQSWAPPATH0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqswappath0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

/**
 * @brief Sets the TXIQSWAPPATH0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqswappath0 - The value to set the field to.
 */
__INLINE void riu_txiqswappath0_setf(uint8_t txiqswappath0)
{
    ASSERT_ERR((((uint32_t)txiqswappath0 << 27) & ~((uint32_t)0x08000000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x08000000)) | ((uint32_t)txiqswappath0 << 27));
}

/**
 * @brief Returns the current value of the RXIQSWAPPATH1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQSWAPPATH1 field's value will be returned.
 *
 * @return The current value of the RXIQSWAPPATH1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqswappath1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the RXIQSWAPPATH1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqswappath1 - The value to set the field to.
 */
__INLINE void riu_rxiqswappath1_setf(uint8_t rxiqswappath1)
{
    ASSERT_ERR((((uint32_t)rxiqswappath1 << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)rxiqswappath1 << 23));
}

/**
 * @brief Returns the current value of the RXIQSWAPPATH0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQSWAPPATH0 field's value will be returned.
 *
 * @return The current value of the RXIQSWAPPATH0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqswappath0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00400000)) >> 22);
}

/**
 * @brief Sets the RXIQSWAPPATH0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqswappath0 - The value to set the field to.
 */
__INLINE void riu_rxiqswappath0_setf(uint8_t rxiqswappath0)
{
    ASSERT_ERR((((uint32_t)rxiqswappath0 << 22) & ~((uint32_t)0x00400000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00400000)) | ((uint32_t)rxiqswappath0 << 22));
}

/**
 * @brief Returns the current value of the TXIQDELPATHQ1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQDELPATHQ1 field's value will be returned.
 *
 * @return The current value of the TXIQDELPATHQ1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqdelpathq1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00008000)) >> 15);
}

/**
 * @brief Sets the TXIQDELPATHQ1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqdelpathq1 - The value to set the field to.
 */
__INLINE void riu_txiqdelpathq1_setf(uint8_t txiqdelpathq1)
{
    ASSERT_ERR((((uint32_t)txiqdelpathq1 << 15) & ~((uint32_t)0x00008000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00008000)) | ((uint32_t)txiqdelpathq1 << 15));
}

/**
 * @brief Returns the current value of the TXIQDELPATHI1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQDELPATHI1 field's value will be returned.
 *
 * @return The current value of the TXIQDELPATHI1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqdelpathi1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00004000)) >> 14);
}

/**
 * @brief Sets the TXIQDELPATHI1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqdelpathi1 - The value to set the field to.
 */
__INLINE void riu_txiqdelpathi1_setf(uint8_t txiqdelpathi1)
{
    ASSERT_ERR((((uint32_t)txiqdelpathi1 << 14) & ~((uint32_t)0x00004000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00004000)) | ((uint32_t)txiqdelpathi1 << 14));
}

/**
 * @brief Returns the current value of the TXIQDELPATHQ0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQDELPATHQ0 field's value will be returned.
 *
 * @return The current value of the TXIQDELPATHQ0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqdelpathq0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

/**
 * @brief Sets the TXIQDELPATHQ0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqdelpathq0 - The value to set the field to.
 */
__INLINE void riu_txiqdelpathq0_setf(uint8_t txiqdelpathq0)
{
    ASSERT_ERR((((uint32_t)txiqdelpathq0 << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)txiqdelpathq0 << 13));
}

/**
 * @brief Returns the current value of the TXIQDELPATHI0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXIQDELPATHI0 field's value will be returned.
 *
 * @return The current value of the TXIQDELPATHI0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txiqdelpathi0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the TXIQDELPATHI0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txiqdelpathi0 - The value to set the field to.
 */
__INLINE void riu_txiqdelpathi0_setf(uint8_t txiqdelpathi0)
{
    ASSERT_ERR((((uint32_t)txiqdelpathi0 << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)txiqdelpathi0 << 12));
}

/**
 * @brief Returns the current value of the RXIQDELPATHQ1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQDELPATHQ1 field's value will be returned.
 *
 * @return The current value of the RXIQDELPATHQ1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqdelpathq1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000020)) >> 5);
}

/**
 * @brief Sets the RXIQDELPATHQ1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqdelpathq1 - The value to set the field to.
 */
__INLINE void riu_rxiqdelpathq1_setf(uint8_t rxiqdelpathq1)
{
    ASSERT_ERR((((uint32_t)rxiqdelpathq1 << 5) & ~((uint32_t)0x00000020)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000020)) | ((uint32_t)rxiqdelpathq1 << 5));
}

/**
 * @brief Returns the current value of the RXIQDELPATHI1 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQDELPATHI1 field's value will be returned.
 *
 * @return The current value of the RXIQDELPATHI1 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqdelpathi1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the RXIQDELPATHI1 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqdelpathi1 - The value to set the field to.
 */
__INLINE void riu_rxiqdelpathi1_setf(uint8_t rxiqdelpathi1)
{
    ASSERT_ERR((((uint32_t)rxiqdelpathi1 << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)rxiqdelpathi1 << 4));
}

/**
 * @brief Returns the current value of the RXIQDELPATHQ0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQDELPATHQ0 field's value will be returned.
 *
 * @return The current value of the RXIQDELPATHQ0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqdelpathq0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the RXIQDELPATHQ0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqdelpathq0 - The value to set the field to.
 */
__INLINE void riu_rxiqdelpathq0_setf(uint8_t rxiqdelpathq0)
{
    ASSERT_ERR((((uint32_t)rxiqdelpathq0 << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)rxiqdelpathq0 << 3));
}

/**
 * @brief Returns the current value of the RXIQDELPATHI0 field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXIQDELPATHI0 field's value will be returned.
 *
 * @return The current value of the RXIQDELPATHI0 field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxiqdelpathi0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the RXIQDELPATHI0 field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxiqdelpathi0 - The value to set the field to.
 */
__INLINE void riu_rxiqdelpathi0_setf(uint8_t rxiqdelpathi0)
{
    ASSERT_ERR((((uint32_t)rxiqdelpathi0 << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)rxiqdelpathi0 << 2));
}

/**
 * @brief Returns the current value of the TXC2SDISBCFG field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the TXC2SDISBCFG field's value will be returned.
 *
 * @return The current value of the TXC2SDISBCFG field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_txc2sdisbcfg_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the TXC2SDISBCFG field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txc2sdisbcfg - The value to set the field to.
 */
__INLINE void riu_txc2sdisbcfg_setf(uint8_t txc2sdisbcfg)
{
    ASSERT_ERR((((uint32_t)txc2sdisbcfg << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)txc2sdisbcfg << 1));
}

/**
 * @brief Returns the current value of the RXC2SDISBCFG field in the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read and the RXC2SDISBCFG field's value will be returned.
 *
 * @return The current value of the RXC2SDISBCFG field in the RWNXIQCTRL register.
 */
__INLINE uint8_t riu_rxc2sdisbcfg_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXIQCTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the RXC2SDISBCFG field of the RWNXIQCTRL register.
 *
 * The RWNXIQCTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxc2sdisbcfg - The value to set the field to.
 */
__INLINE void riu_rxc2sdisbcfg_setf(uint8_t rxc2sdisbcfg)
{
    ASSERT_ERR((((uint32_t)rxc2sdisbcfg << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(RIU_RWNXIQCTRL_ADDR, (REG_PL_RD(RIU_RWNXIQCTRL_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)rxc2sdisbcfg << 0));
}

/// @}

/**
 * @name RWNXMACSTATICCONFIG register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  01:00             PSSELECT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACSTATICCONFIG register
#define RIU_RWNXMACSTATICCONFIG_ADDR   (REG_RIU_BASE_ADDR+0xB080)
/// Offset of the RWNXMACSTATICCONFIG register from the base address
#define RIU_RWNXMACSTATICCONFIG_OFFSET 0x00000080
/// Index of the RWNXMACSTATICCONFIG register
#define RIU_RWNXMACSTATICCONFIG_INDEX  0x00000020
/// Reset value of the RWNXMACSTATICCONFIG register
#define RIU_RWNXMACSTATICCONFIG_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACSTATICCONFIG register.
 * The RWNXMACSTATICCONFIG register will be read and its value returned.
 * @return The current value of the RWNXMACSTATICCONFIG register.
 */
__INLINE uint32_t riu_rwnxmacstaticconfig_get(void)
{
    return REG_PL_RD(RIU_RWNXMACSTATICCONFIG_ADDR);
}

/**
 * @brief Sets the RWNXMACSTATICCONFIG register to a value.
 * The RWNXMACSTATICCONFIG register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxmacstaticconfig_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXMACSTATICCONFIG_ADDR, value);
}

// field definitions
/// PSSELECT field mask
#define RIU_PSSELECT_MASK   ((uint32_t)0x00000003)
/// PSSELECT field LSB position
#define RIU_PSSELECT_LSB    0
/// PSSELECT field width
#define RIU_PSSELECT_WIDTH  ((uint32_t)0x00000002)

/// PSSELECT field reset value
#define RIU_PSSELECT_RST    0x0

/**
 * @brief Returns the current value of the PSSELECT field in the RWNXMACSTATICCONFIG register.
 *
 * The RWNXMACSTATICCONFIG register will be read and the PSSELECT field's value will be returned.
 *
 * @return The current value of the PSSELECT field in the RWNXMACSTATICCONFIG register.
 */
__INLINE uint8_t riu_psselect_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACSTATICCONFIG_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000003)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the PSSELECT field of the RWNXMACSTATICCONFIG register.
 *
 * The RWNXMACSTATICCONFIG register will be read, modified to contain the new field value, and written.
 *
 * @param[in] psselect - The value to set the field to.
 */
__INLINE void riu_psselect_setf(uint8_t psselect)
{
    ASSERT_ERR((((uint32_t)psselect << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(RIU_RWNXMACSTATICCONFIG_ADDR, (uint32_t)psselect << 0);
}

/// @}

/**
 * @name RWNXFECTRL0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:16      TX80DIGGAINLIN0   0x20
 *  14:08      TX40DIGGAINLIN0   0x20
 *  06:00      TX20DIGGAINLIN0   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXFECTRL0 register
#define RIU_RWNXFECTRL0_ADDR   (REG_RIU_BASE_ADDR+0xB100)
/// Offset of the RWNXFECTRL0 register from the base address
#define RIU_RWNXFECTRL0_OFFSET 0x00000100
/// Index of the RWNXFECTRL0 register
#define RIU_RWNXFECTRL0_INDEX  0x00000040
/// Reset value of the RWNXFECTRL0 register
#define RIU_RWNXFECTRL0_RESET  0x00202020

/**
 * @brief Returns the current value of the RWNXFECTRL0 register.
 * The RWNXFECTRL0 register will be read and its value returned.
 * @return The current value of the RWNXFECTRL0 register.
 */
__INLINE uint32_t riu_rwnxfectrl0_get(void)
{
    return REG_PL_RD(RIU_RWNXFECTRL0_ADDR);
}

/**
 * @brief Sets the RWNXFECTRL0 register to a value.
 * The RWNXFECTRL0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxfectrl0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXFECTRL0_ADDR, value);
}

// field definitions
/// TX80DIGGAINLIN0 field mask
#define RIU_TX80DIGGAINLIN0_MASK   ((uint32_t)0x007F0000)
/// TX80DIGGAINLIN0 field LSB position
#define RIU_TX80DIGGAINLIN0_LSB    16
/// TX80DIGGAINLIN0 field width
#define RIU_TX80DIGGAINLIN0_WIDTH  ((uint32_t)0x00000007)
/// TX40DIGGAINLIN0 field mask
#define RIU_TX40DIGGAINLIN0_MASK   ((uint32_t)0x00007F00)
/// TX40DIGGAINLIN0 field LSB position
#define RIU_TX40DIGGAINLIN0_LSB    8
/// TX40DIGGAINLIN0 field width
#define RIU_TX40DIGGAINLIN0_WIDTH  ((uint32_t)0x00000007)
/// TX20DIGGAINLIN0 field mask
#define RIU_TX20DIGGAINLIN0_MASK   ((uint32_t)0x0000007F)
/// TX20DIGGAINLIN0 field LSB position
#define RIU_TX20DIGGAINLIN0_LSB    0
/// TX20DIGGAINLIN0 field width
#define RIU_TX20DIGGAINLIN0_WIDTH  ((uint32_t)0x00000007)

/// TX80DIGGAINLIN0 field reset value
#define RIU_TX80DIGGAINLIN0_RST    0x20
/// TX40DIGGAINLIN0 field reset value
#define RIU_TX40DIGGAINLIN0_RST    0x20
/// TX20DIGGAINLIN0 field reset value
#define RIU_TX20DIGGAINLIN0_RST    0x20

/**
 * @brief Constructs a value for the RWNXFECTRL0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tx80diggainlin0 - The value to use for the TX80DIGGAINLIN0 field.
 * @param[in] tx40diggainlin0 - The value to use for the TX40DIGGAINLIN0 field.
 * @param[in] tx20diggainlin0 - The value to use for the TX20DIGGAINLIN0 field.
 */
__INLINE void riu_rwnxfectrl0_pack(uint8_t tx80diggainlin0, uint8_t tx40diggainlin0, uint8_t tx20diggainlin0)
{
    ASSERT_ERR((((uint32_t)tx80diggainlin0 << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)tx40diggainlin0 << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)tx20diggainlin0 << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL0_ADDR,  ((uint32_t)tx80diggainlin0 << 16) | ((uint32_t)tx40diggainlin0 << 8) | ((uint32_t)tx20diggainlin0 << 0));
}

/**
 * @brief Unpacks RWNXFECTRL0's fields from current value of the RWNXFECTRL0 register.
 *
 * Reads the RWNXFECTRL0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tx80diggainlin0 - Will be populated with the current value of this field from the register.
 * @param[out] tx40diggainlin0 - Will be populated with the current value of this field from the register.
 * @param[out] tx20diggainlin0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxfectrl0_unpack(uint8_t* tx80diggainlin0, uint8_t* tx40diggainlin0, uint8_t* tx20diggainlin0)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL0_ADDR);

    *tx80diggainlin0 = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *tx40diggainlin0 = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *tx20diggainlin0 = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the TX80DIGGAINLIN0 field in the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read and the TX80DIGGAINLIN0 field's value will be returned.
 *
 * @return The current value of the TX80DIGGAINLIN0 field in the RWNXFECTRL0 register.
 */
__INLINE uint8_t riu_tx80diggainlin0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the TX80DIGGAINLIN0 field of the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx80diggainlin0 - The value to set the field to.
 */
__INLINE void riu_tx80diggainlin0_setf(uint8_t tx80diggainlin0)
{
    ASSERT_ERR((((uint32_t)tx80diggainlin0 << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL0_ADDR, (REG_PL_RD(RIU_RWNXFECTRL0_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)tx80diggainlin0 << 16));
}

/**
 * @brief Returns the current value of the TX40DIGGAINLIN0 field in the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read and the TX40DIGGAINLIN0 field's value will be returned.
 *
 * @return The current value of the TX40DIGGAINLIN0 field in the RWNXFECTRL0 register.
 */
__INLINE uint8_t riu_tx40diggainlin0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the TX40DIGGAINLIN0 field of the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx40diggainlin0 - The value to set the field to.
 */
__INLINE void riu_tx40diggainlin0_setf(uint8_t tx40diggainlin0)
{
    ASSERT_ERR((((uint32_t)tx40diggainlin0 << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL0_ADDR, (REG_PL_RD(RIU_RWNXFECTRL0_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)tx40diggainlin0 << 8));
}

/**
 * @brief Returns the current value of the TX20DIGGAINLIN0 field in the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read and the TX20DIGGAINLIN0 field's value will be returned.
 *
 * @return The current value of the TX20DIGGAINLIN0 field in the RWNXFECTRL0 register.
 */
__INLINE uint8_t riu_tx20diggainlin0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL0_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the TX20DIGGAINLIN0 field of the RWNXFECTRL0 register.
 *
 * The RWNXFECTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx20diggainlin0 - The value to set the field to.
 */
__INLINE void riu_tx20diggainlin0_setf(uint8_t tx20diggainlin0)
{
    ASSERT_ERR((((uint32_t)tx20diggainlin0 << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL0_ADDR, (REG_PL_RD(RIU_RWNXFECTRL0_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)tx20diggainlin0 << 0));
}

/// @}

/**
 * @name RWNXFECTRL1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:16      TX80DIGGAINLIN1   0x20
 *  14:08      TX40DIGGAINLIN1   0x20
 *  06:00      TX20DIGGAINLIN1   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXFECTRL1 register
#define RIU_RWNXFECTRL1_ADDR   (REG_RIU_BASE_ADDR+0xB104)
/// Offset of the RWNXFECTRL1 register from the base address
#define RIU_RWNXFECTRL1_OFFSET 0x00000104
/// Index of the RWNXFECTRL1 register
#define RIU_RWNXFECTRL1_INDEX  0x00000041
/// Reset value of the RWNXFECTRL1 register
#define RIU_RWNXFECTRL1_RESET  0x00202020

/**
 * @brief Returns the current value of the RWNXFECTRL1 register.
 * The RWNXFECTRL1 register will be read and its value returned.
 * @return The current value of the RWNXFECTRL1 register.
 */
__INLINE uint32_t riu_rwnxfectrl1_get(void)
{
    return REG_PL_RD(RIU_RWNXFECTRL1_ADDR);
}

/**
 * @brief Sets the RWNXFECTRL1 register to a value.
 * The RWNXFECTRL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxfectrl1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXFECTRL1_ADDR, value);
}

// field definitions
/// TX80DIGGAINLIN1 field mask
#define RIU_TX80DIGGAINLIN1_MASK   ((uint32_t)0x007F0000)
/// TX80DIGGAINLIN1 field LSB position
#define RIU_TX80DIGGAINLIN1_LSB    16
/// TX80DIGGAINLIN1 field width
#define RIU_TX80DIGGAINLIN1_WIDTH  ((uint32_t)0x00000007)
/// TX40DIGGAINLIN1 field mask
#define RIU_TX40DIGGAINLIN1_MASK   ((uint32_t)0x00007F00)
/// TX40DIGGAINLIN1 field LSB position
#define RIU_TX40DIGGAINLIN1_LSB    8
/// TX40DIGGAINLIN1 field width
#define RIU_TX40DIGGAINLIN1_WIDTH  ((uint32_t)0x00000007)
/// TX20DIGGAINLIN1 field mask
#define RIU_TX20DIGGAINLIN1_MASK   ((uint32_t)0x0000007F)
/// TX20DIGGAINLIN1 field LSB position
#define RIU_TX20DIGGAINLIN1_LSB    0
/// TX20DIGGAINLIN1 field width
#define RIU_TX20DIGGAINLIN1_WIDTH  ((uint32_t)0x00000007)

/// TX80DIGGAINLIN1 field reset value
#define RIU_TX80DIGGAINLIN1_RST    0x20
/// TX40DIGGAINLIN1 field reset value
#define RIU_TX40DIGGAINLIN1_RST    0x20
/// TX20DIGGAINLIN1 field reset value
#define RIU_TX20DIGGAINLIN1_RST    0x20

/**
 * @brief Constructs a value for the RWNXFECTRL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tx80diggainlin1 - The value to use for the TX80DIGGAINLIN1 field.
 * @param[in] tx40diggainlin1 - The value to use for the TX40DIGGAINLIN1 field.
 * @param[in] tx20diggainlin1 - The value to use for the TX20DIGGAINLIN1 field.
 */
__INLINE void riu_rwnxfectrl1_pack(uint8_t tx80diggainlin1, uint8_t tx40diggainlin1, uint8_t tx20diggainlin1)
{
    ASSERT_ERR((((uint32_t)tx80diggainlin1 << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)tx40diggainlin1 << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)tx20diggainlin1 << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL1_ADDR,  ((uint32_t)tx80diggainlin1 << 16) | ((uint32_t)tx40diggainlin1 << 8) | ((uint32_t)tx20diggainlin1 << 0));
}

/**
 * @brief Unpacks RWNXFECTRL1's fields from current value of the RWNXFECTRL1 register.
 *
 * Reads the RWNXFECTRL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tx80diggainlin1 - Will be populated with the current value of this field from the register.
 * @param[out] tx40diggainlin1 - Will be populated with the current value of this field from the register.
 * @param[out] tx20diggainlin1 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxfectrl1_unpack(uint8_t* tx80diggainlin1, uint8_t* tx40diggainlin1, uint8_t* tx20diggainlin1)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL1_ADDR);

    *tx80diggainlin1 = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *tx40diggainlin1 = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *tx20diggainlin1 = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the TX80DIGGAINLIN1 field in the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read and the TX80DIGGAINLIN1 field's value will be returned.
 *
 * @return The current value of the TX80DIGGAINLIN1 field in the RWNXFECTRL1 register.
 */
__INLINE uint8_t riu_tx80diggainlin1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the TX80DIGGAINLIN1 field of the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx80diggainlin1 - The value to set the field to.
 */
__INLINE void riu_tx80diggainlin1_setf(uint8_t tx80diggainlin1)
{
    ASSERT_ERR((((uint32_t)tx80diggainlin1 << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL1_ADDR, (REG_PL_RD(RIU_RWNXFECTRL1_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)tx80diggainlin1 << 16));
}

/**
 * @brief Returns the current value of the TX40DIGGAINLIN1 field in the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read and the TX40DIGGAINLIN1 field's value will be returned.
 *
 * @return The current value of the TX40DIGGAINLIN1 field in the RWNXFECTRL1 register.
 */
__INLINE uint8_t riu_tx40diggainlin1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the TX40DIGGAINLIN1 field of the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx40diggainlin1 - The value to set the field to.
 */
__INLINE void riu_tx40diggainlin1_setf(uint8_t tx40diggainlin1)
{
    ASSERT_ERR((((uint32_t)tx40diggainlin1 << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL1_ADDR, (REG_PL_RD(RIU_RWNXFECTRL1_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)tx40diggainlin1 << 8));
}

/**
 * @brief Returns the current value of the TX20DIGGAINLIN1 field in the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read and the TX20DIGGAINLIN1 field's value will be returned.
 *
 * @return The current value of the TX20DIGGAINLIN1 field in the RWNXFECTRL1 register.
 */
__INLINE uint8_t riu_tx20diggainlin1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFECTRL1_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the TX20DIGGAINLIN1 field of the RWNXFECTRL1 register.
 *
 * The RWNXFECTRL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tx20diggainlin1 - The value to set the field to.
 */
__INLINE void riu_tx20diggainlin1_setf(uint8_t tx20diggainlin1)
{
    ASSERT_ERR((((uint32_t)tx20diggainlin1 << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXFECTRL1_ADDR, (REG_PL_RD(RIU_RWNXFECTRL1_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)tx20diggainlin1 << 0));
}

/// @}

/**
 * @name AGCGAINSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08             AGCGAIN1   0x0
 *  07:00             AGCGAIN0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCGAINSTAT register
#define RIU_AGCGAINSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB200)
/// Offset of the AGCGAINSTAT register from the base address
#define RIU_AGCGAINSTAT_OFFSET 0x00000200
/// Index of the AGCGAINSTAT register
#define RIU_AGCGAINSTAT_INDEX  0x00000080
/// Reset value of the AGCGAINSTAT register
#define RIU_AGCGAINSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCGAINSTAT register.
 * The AGCGAINSTAT register will be read and its value returned.
 * @return The current value of the AGCGAINSTAT register.
 */
__INLINE uint32_t riu_agcgainstat_get(void)
{
    return REG_PL_RD(RIU_AGCGAINSTAT_ADDR);
}

// field definitions
/// AGCGAIN1 field mask
#define RIU_AGCGAIN1_MASK   ((uint32_t)0x0000FF00)
/// AGCGAIN1 field LSB position
#define RIU_AGCGAIN1_LSB    8
/// AGCGAIN1 field width
#define RIU_AGCGAIN1_WIDTH  ((uint32_t)0x00000008)
/// AGCGAIN0 field mask
#define RIU_AGCGAIN0_MASK   ((uint32_t)0x000000FF)
/// AGCGAIN0 field LSB position
#define RIU_AGCGAIN0_LSB    0
/// AGCGAIN0 field width
#define RIU_AGCGAIN0_WIDTH  ((uint32_t)0x00000008)

/// AGCGAIN1 field reset value
#define RIU_AGCGAIN1_RST    0x0
/// AGCGAIN0 field reset value
#define RIU_AGCGAIN0_RST    0x0

/**
 * @brief Unpacks AGCGAINSTAT's fields from current value of the AGCGAINSTAT register.
 *
 * Reads the AGCGAINSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] agcgain1 - Will be populated with the current value of this field from the register.
 * @param[out] agcgain0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcgainstat_unpack(uint8_t* agcgain1, uint8_t* agcgain0)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCGAINSTAT_ADDR);

    *agcgain1 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *agcgain0 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the AGCGAIN1 field in the AGCGAINSTAT register.
 *
 * The AGCGAINSTAT register will be read and the AGCGAIN1 field's value will be returned.
 *
 * @return The current value of the AGCGAIN1 field in the AGCGAINSTAT register.
 */
__INLINE uint8_t riu_agcgain1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCGAINSTAT_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Returns the current value of the AGCGAIN0 field in the AGCGAINSTAT register.
 *
 * The AGCGAINSTAT register will be read and the AGCGAIN0 field's value will be returned.
 *
 * @return The current value of the AGCGAIN0 field in the AGCGAINSTAT register.
 */
__INLINE uint8_t riu_agcgain0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCGAINSTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name AGCADCPOWSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  14:08           ADCPOWDBV1   0x0
 *  06:00           ADCPOWDBV0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCADCPOWSTAT register
#define RIU_AGCADCPOWSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB208)
/// Offset of the AGCADCPOWSTAT register from the base address
#define RIU_AGCADCPOWSTAT_OFFSET 0x00000208
/// Index of the AGCADCPOWSTAT register
#define RIU_AGCADCPOWSTAT_INDEX  0x00000082
/// Reset value of the AGCADCPOWSTAT register
#define RIU_AGCADCPOWSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCADCPOWSTAT register.
 * The AGCADCPOWSTAT register will be read and its value returned.
 * @return The current value of the AGCADCPOWSTAT register.
 */
__INLINE uint32_t riu_agcadcpowstat_get(void)
{
    return REG_PL_RD(RIU_AGCADCPOWSTAT_ADDR);
}

// field definitions
/// ADCPOWDBV1 field mask
#define RIU_ADCPOWDBV1_MASK   ((uint32_t)0x00007F00)
/// ADCPOWDBV1 field LSB position
#define RIU_ADCPOWDBV1_LSB    8
/// ADCPOWDBV1 field width
#define RIU_ADCPOWDBV1_WIDTH  ((uint32_t)0x00000007)
/// ADCPOWDBV0 field mask
#define RIU_ADCPOWDBV0_MASK   ((uint32_t)0x0000007F)
/// ADCPOWDBV0 field LSB position
#define RIU_ADCPOWDBV0_LSB    0
/// ADCPOWDBV0 field width
#define RIU_ADCPOWDBV0_WIDTH  ((uint32_t)0x00000007)

/// ADCPOWDBV1 field reset value
#define RIU_ADCPOWDBV1_RST    0x0
/// ADCPOWDBV0 field reset value
#define RIU_ADCPOWDBV0_RST    0x0

/**
 * @brief Unpacks AGCADCPOWSTAT's fields from current value of the AGCADCPOWSTAT register.
 *
 * Reads the AGCADCPOWSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] adcpowdbv1 - Will be populated with the current value of this field from the register.
 * @param[out] adcpowdbv0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcadcpowstat_unpack(uint8_t* adcpowdbv1, uint8_t* adcpowdbv0)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCADCPOWSTAT_ADDR);

    *adcpowdbv1 = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *adcpowdbv0 = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the ADCPOWDBV1 field in the AGCADCPOWSTAT register.
 *
 * The AGCADCPOWSTAT register will be read and the ADCPOWDBV1 field's value will be returned.
 *
 * @return The current value of the ADCPOWDBV1 field in the AGCADCPOWSTAT register.
 */
__INLINE uint8_t riu_adcpowdbv1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCADCPOWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Returns the current value of the ADCPOWDBV0 field in the AGCADCPOWSTAT register.
 *
 * The AGCADCPOWSTAT register will be read and the ADCPOWDBV0 field's value will be returned.
 *
 * @return The current value of the ADCPOWDBV0 field in the AGCADCPOWSTAT register.
 */
__INLINE uint8_t riu_adcpowdbv0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCADCPOWSTAT_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/// @}

/**
 * @name AGCINBDPOW20PSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08       INBDPOW20PDBM1   0x0
 *  07:00       INBDPOW20PDBM0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCINBDPOW20PSTAT register
#define RIU_AGCINBDPOW20PSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB20C)
/// Offset of the AGCINBDPOW20PSTAT register from the base address
#define RIU_AGCINBDPOW20PSTAT_OFFSET 0x0000020C
/// Index of the AGCINBDPOW20PSTAT register
#define RIU_AGCINBDPOW20PSTAT_INDEX  0x00000083
/// Reset value of the AGCINBDPOW20PSTAT register
#define RIU_AGCINBDPOW20PSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCINBDPOW20PSTAT register.
 * The AGCINBDPOW20PSTAT register will be read and its value returned.
 * @return The current value of the AGCINBDPOW20PSTAT register.
 */
__INLINE uint32_t riu_agcinbdpow20pstat_get(void)
{
    return REG_PL_RD(RIU_AGCINBDPOW20PSTAT_ADDR);
}

// field definitions
/// INBDPOW20PDBM1 field mask
#define RIU_INBDPOW20PDBM1_MASK   ((uint32_t)0x0000FF00)
/// INBDPOW20PDBM1 field LSB position
#define RIU_INBDPOW20PDBM1_LSB    8
/// INBDPOW20PDBM1 field width
#define RIU_INBDPOW20PDBM1_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20PDBM0 field mask
#define RIU_INBDPOW20PDBM0_MASK   ((uint32_t)0x000000FF)
/// INBDPOW20PDBM0 field LSB position
#define RIU_INBDPOW20PDBM0_LSB    0
/// INBDPOW20PDBM0 field width
#define RIU_INBDPOW20PDBM0_WIDTH  ((uint32_t)0x00000008)

/// INBDPOW20PDBM1 field reset value
#define RIU_INBDPOW20PDBM1_RST    0x0
/// INBDPOW20PDBM0 field reset value
#define RIU_INBDPOW20PDBM0_RST    0x0

/**
 * @brief Unpacks AGCINBDPOW20PSTAT's fields from current value of the AGCINBDPOW20PSTAT register.
 *
 * Reads the AGCINBDPOW20PSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpow20pdbm1 - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20pdbm0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcinbdpow20pstat_unpack(uint8_t* inbdpow20pdbm1, uint8_t* inbdpow20pdbm0)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PSTAT_ADDR);

    *inbdpow20pdbm1 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *inbdpow20pdbm0 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOW20PDBM1 field in the AGCINBDPOW20PSTAT register.
 *
 * The AGCINBDPOW20PSTAT register will be read and the INBDPOW20PDBM1 field's value will be returned.
 *
 * @return The current value of the INBDPOW20PDBM1 field in the AGCINBDPOW20PSTAT register.
 */
__INLINE uint8_t riu_inbdpow20pdbm1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PSTAT_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Returns the current value of the INBDPOW20PDBM0 field in the AGCINBDPOW20PSTAT register.
 *
 * The AGCINBDPOW20PSTAT register will be read and the INBDPOW20PDBM0 field's value will be returned.
 *
 * @return The current value of the INBDPOW20PDBM0 field in the AGCINBDPOW20PSTAT register.
 */
__INLINE uint8_t riu_inbdpow20pdbm0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PSTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name AGCDIGGAIN0STAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:16        DIGGAIN80ANT0   0x0
 *  14:08        DIGGAIN40ANT0   0x0
 *  06:00        DIGGAIN20ANT0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCDIGGAIN0STAT register
#define RIU_AGCDIGGAIN0STAT_ADDR   (REG_RIU_BASE_ADDR+0xB210)
/// Offset of the AGCDIGGAIN0STAT register from the base address
#define RIU_AGCDIGGAIN0STAT_OFFSET 0x00000210
/// Index of the AGCDIGGAIN0STAT register
#define RIU_AGCDIGGAIN0STAT_INDEX  0x00000084
/// Reset value of the AGCDIGGAIN0STAT register
#define RIU_AGCDIGGAIN0STAT_RESET  0x00000000
///#define RIU_AGCDIGGAIN0STAT_RESET  0x007A7D00
/**
 * @brief Returns the current value of the AGCDIGGAIN0STAT register.
 * The AGCDIGGAIN0STAT register will be read and its value returned.
 * @return The current value of the AGCDIGGAIN0STAT register.
 */
__INLINE uint32_t riu_agcdiggain0stat_get(void)
{
    return REG_PL_RD(RIU_AGCDIGGAIN0STAT_ADDR);
}

// field definitions
/// DIGGAIN80ANT0 field mask
#define RIU_DIGGAIN80ANT0_MASK   ((uint32_t)0x007F0000)
/// DIGGAIN80ANT0 field LSB position
#define RIU_DIGGAIN80ANT0_LSB    16
/// DIGGAIN80ANT0 field width
#define RIU_DIGGAIN80ANT0_WIDTH  ((uint32_t)0x00000007)
/// DIGGAIN40ANT0 field mask
#define RIU_DIGGAIN40ANT0_MASK   ((uint32_t)0x00007F00)
/// DIGGAIN40ANT0 field LSB position
#define RIU_DIGGAIN40ANT0_LSB    8
/// DIGGAIN40ANT0 field width
#define RIU_DIGGAIN40ANT0_WIDTH  ((uint32_t)0x00000007)
/// DIGGAIN20ANT0 field mask
#define RIU_DIGGAIN20ANT0_MASK   ((uint32_t)0x0000007F)
/// DIGGAIN20ANT0 field LSB position
#define RIU_DIGGAIN20ANT0_LSB    0
/// DIGGAIN20ANT0 field width
#define RIU_DIGGAIN20ANT0_WIDTH  ((uint32_t)0x00000007)

/// DIGGAIN80ANT0 field reset value
#define RIU_DIGGAIN80ANT0_RST    0x0
/// DIGGAIN40ANT0 field reset value
#define RIU_DIGGAIN40ANT0_RST    0x0
/// DIGGAIN20ANT0 field reset value
#define RIU_DIGGAIN20ANT0_RST    0x0

/**
 * @brief Unpacks AGCDIGGAIN0STAT's fields from current value of the AGCDIGGAIN0STAT register.
 *
 * Reads the AGCDIGGAIN0STAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] diggain80ant0 - Will be populated with the current value of this field from the register.
 * @param[out] diggain40ant0 - Will be populated with the current value of this field from the register.
 * @param[out] diggain20ant0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcdiggain0stat_unpack(uint8_t* diggain80ant0, uint8_t* diggain40ant0, uint8_t* diggain20ant0)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN0STAT_ADDR);

    *diggain80ant0 = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *diggain40ant0 = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *diggain20ant0 = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the DIGGAIN80ANT0 field in the AGCDIGGAIN0STAT register.
 *
 * The AGCDIGGAIN0STAT register will be read and the DIGGAIN80ANT0 field's value will be returned.
 *
 * @return The current value of the DIGGAIN80ANT0 field in the AGCDIGGAIN0STAT register.
 */
__INLINE uint8_t riu_diggain80ant0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN0STAT_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Returns the current value of the DIGGAIN40ANT0 field in the AGCDIGGAIN0STAT register.
 *
 * The AGCDIGGAIN0STAT register will be read and the DIGGAIN40ANT0 field's value will be returned.
 *
 * @return The current value of the DIGGAIN40ANT0 field in the AGCDIGGAIN0STAT register.
 */
__INLINE uint8_t riu_diggain40ant0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN0STAT_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Returns the current value of the DIGGAIN20ANT0 field in the AGCDIGGAIN0STAT register.
 *
 * The AGCDIGGAIN0STAT register will be read and the DIGGAIN20ANT0 field's value will be returned.
 *
 * @return The current value of the DIGGAIN20ANT0 field in the AGCDIGGAIN0STAT register.
 */
__INLINE uint8_t riu_diggain20ant0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN0STAT_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/// @}

/**
 * @name AGCDIGGAIN1STAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:16        DIGGAIN80ANT1   0x0
 *  14:08        DIGGAIN40ANT1   0x0
 *  06:00        DIGGAIN20ANT1   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCDIGGAIN1STAT register
#define RIU_AGCDIGGAIN1STAT_ADDR   (REG_RIU_BASE_ADDR+0xB214)
/// Offset of the AGCDIGGAIN1STAT register from the base address
#define RIU_AGCDIGGAIN1STAT_OFFSET 0x00000214
/// Index of the AGCDIGGAIN1STAT register
#define RIU_AGCDIGGAIN1STAT_INDEX  0x00000085
/// Reset value of the AGCDIGGAIN1STAT register
#define RIU_AGCDIGGAIN1STAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCDIGGAIN1STAT register.
 * The AGCDIGGAIN1STAT register will be read and its value returned.
 * @return The current value of the AGCDIGGAIN1STAT register.
 */
__INLINE uint32_t riu_agcdiggain1stat_get(void)
{
    return REG_PL_RD(RIU_AGCDIGGAIN1STAT_ADDR);
}

// field definitions
/// DIGGAIN80ANT1 field mask
#define RIU_DIGGAIN80ANT1_MASK   ((uint32_t)0x007F0000)
/// DIGGAIN80ANT1 field LSB position
#define RIU_DIGGAIN80ANT1_LSB    16
/// DIGGAIN80ANT1 field width
#define RIU_DIGGAIN80ANT1_WIDTH  ((uint32_t)0x00000007)
/// DIGGAIN40ANT1 field mask
#define RIU_DIGGAIN40ANT1_MASK   ((uint32_t)0x00007F00)
/// DIGGAIN40ANT1 field LSB position
#define RIU_DIGGAIN40ANT1_LSB    8
/// DIGGAIN40ANT1 field width
#define RIU_DIGGAIN40ANT1_WIDTH  ((uint32_t)0x00000007)
/// DIGGAIN20ANT1 field mask
#define RIU_DIGGAIN20ANT1_MASK   ((uint32_t)0x0000007F)
/// DIGGAIN20ANT1 field LSB position
#define RIU_DIGGAIN20ANT1_LSB    0
/// DIGGAIN20ANT1 field width
#define RIU_DIGGAIN20ANT1_WIDTH  ((uint32_t)0x00000007)

/// DIGGAIN80ANT1 field reset value
#define RIU_DIGGAIN80ANT1_RST    0x0
/// DIGGAIN40ANT1 field reset value
#define RIU_DIGGAIN40ANT1_RST    0x0
/// DIGGAIN20ANT1 field reset value
#define RIU_DIGGAIN20ANT1_RST    0x0

/**
 * @brief Unpacks AGCDIGGAIN1STAT's fields from current value of the AGCDIGGAIN1STAT register.
 *
 * Reads the AGCDIGGAIN1STAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] diggain80ant1 - Will be populated with the current value of this field from the register.
 * @param[out] diggain40ant1 - Will be populated with the current value of this field from the register.
 * @param[out] diggain20ant1 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcdiggain1stat_unpack(uint8_t* diggain80ant1, uint8_t* diggain40ant1, uint8_t* diggain20ant1)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN1STAT_ADDR);

    *diggain80ant1 = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *diggain40ant1 = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *diggain20ant1 = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the DIGGAIN80ANT1 field in the AGCDIGGAIN1STAT register.
 *
 * The AGCDIGGAIN1STAT register will be read and the DIGGAIN80ANT1 field's value will be returned.
 *
 * @return The current value of the DIGGAIN80ANT1 field in the AGCDIGGAIN1STAT register.
 */
__INLINE uint8_t riu_diggain80ant1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN1STAT_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Returns the current value of the DIGGAIN40ANT1 field in the AGCDIGGAIN1STAT register.
 *
 * The AGCDIGGAIN1STAT register will be read and the DIGGAIN40ANT1 field's value will be returned.
 *
 * @return The current value of the DIGGAIN40ANT1 field in the AGCDIGGAIN1STAT register.
 */
__INLINE uint8_t riu_diggain40ant1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN1STAT_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Returns the current value of the DIGGAIN20ANT1 field in the AGCDIGGAIN1STAT register.
 *
 * The AGCDIGGAIN1STAT register will be read and the DIGGAIN20ANT1 field's value will be returned.
 *
 * @return The current value of the DIGGAIN20ANT1 field in the AGCDIGGAIN1STAT register.
 */
__INLINE uint8_t riu_diggain20ant1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCDIGGAIN1STAT_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/// @}

#if RW_RADAR_EN
/**
 * @name RADARFIFOSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00         RADFIFOEMPTY   0
 * </pre>
 *
 * @{
 */

/// Address of the RADARFIFOSTAT register
#define RIU_RADARFIFOSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB220)
/// Offset of the RADARFIFOSTAT register from the base address
#define RIU_RADARFIFOSTAT_OFFSET 0x00000220
/// Index of the RADARFIFOSTAT register
#define RIU_RADARFIFOSTAT_INDEX  0x00000088
/// Reset value of the RADARFIFOSTAT register
///#define RIU_RADARFIFOSTAT_RESET  0x00000000
#define RIU_RADARFIFOSTAT_RESET  0x00000001
/**
 * @brief Returns the current value of the RADARFIFOSTAT register.
 * The RADARFIFOSTAT register will be read and its value returned.
 * @return The current value of the RADARFIFOSTAT register.
 */
__INLINE uint32_t riu_radarfifostat_get(void)
{
    return REG_PL_RD(RIU_RADARFIFOSTAT_ADDR);
}

// field definitions
/// RADFIFOEMPTY field bit
#define RIU_RADFIFOEMPTY_BIT    ((uint32_t)0x00000001)
/// RADFIFOEMPTY field position
#define RIU_RADFIFOEMPTY_POS    0

/// RADFIFOEMPTY field reset value
#define RIU_RADFIFOEMPTY_RST    0x0

/**
 * @brief Returns the current value of the RADFIFOEMPTY field in the RADARFIFOSTAT register.
 *
 * The RADARFIFOSTAT register will be read and the RADFIFOEMPTY field's value will be returned.
 *
 * @return The current value of the RADFIFOEMPTY field in the RADARFIFOSTAT register.
 */
__INLINE uint8_t riu_radfifoempty_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RADARFIFOSTAT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

#endif // RW_RADAR_EN
/// @}

/**
 * @name AGCINBDPOW20PNOISESTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08   INBDPOW20PNOISEDBM1   0x0
 *  07:00   INBDPOW20PNOISEDBM0   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCINBDPOW20PNOISESTAT register
#define RIU_AGCINBDPOW20PNOISESTAT_ADDR   (REG_RIU_BASE_ADDR+0xB228)
/// Offset of the AGCINBDPOW20PNOISESTAT register from the base address
#define RIU_AGCINBDPOW20PNOISESTAT_OFFSET 0x00000228
/// Index of the AGCINBDPOW20PNOISESTAT register
#define RIU_AGCINBDPOW20PNOISESTAT_INDEX  0x0000008A
/// Reset value of the AGCINBDPOW20PNOISESTAT register
#define RIU_AGCINBDPOW20PNOISESTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCINBDPOW20PNOISESTAT register.
 * The AGCINBDPOW20PNOISESTAT register will be read and its value returned.
 * @return The current value of the AGCINBDPOW20PNOISESTAT register.
 */
__INLINE uint32_t riu_agcinbdpow20pnoisestat_get(void)
{
    return REG_PL_RD(RIU_AGCINBDPOW20PNOISESTAT_ADDR);
}

// field definitions
/// INBDPOW20PNOISEDBM1 field mask
#define RIU_INBDPOW20PNOISEDBM1_MASK   ((uint32_t)0x0000FF00)
/// INBDPOW20PNOISEDBM1 field LSB position
#define RIU_INBDPOW20PNOISEDBM1_LSB    8
/// INBDPOW20PNOISEDBM1 field width
#define RIU_INBDPOW20PNOISEDBM1_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20PNOISEDBM0 field mask
#define RIU_INBDPOW20PNOISEDBM0_MASK   ((uint32_t)0x000000FF)
/// INBDPOW20PNOISEDBM0 field LSB position
#define RIU_INBDPOW20PNOISEDBM0_LSB    0
/// INBDPOW20PNOISEDBM0 field width
#define RIU_INBDPOW20PNOISEDBM0_WIDTH  ((uint32_t)0x00000008)

/// INBDPOW20PNOISEDBM1 field reset value
#define RIU_INBDPOW20PNOISEDBM1_RST    0x0
/// INBDPOW20PNOISEDBM0 field reset value
#define RIU_INBDPOW20PNOISEDBM0_RST    0x0

/**
 * @brief Unpacks AGCINBDPOW20PNOISESTAT's fields from current value of the AGCINBDPOW20PNOISESTAT register.
 *
 * Reads the AGCINBDPOW20PNOISESTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpow20pnoisedbm1 - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20pnoisedbm0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcinbdpow20pnoisestat_unpack(uint8_t* inbdpow20pnoisedbm1, uint8_t* inbdpow20pnoisedbm0)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PNOISESTAT_ADDR);

    *inbdpow20pnoisedbm1 = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *inbdpow20pnoisedbm0 = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOW20PNOISEDBM1 field in the AGCINBDPOW20PNOISESTAT register.
 *
 * The AGCINBDPOW20PNOISESTAT register will be read and the INBDPOW20PNOISEDBM1 field's value will be returned.
 *
 * @return The current value of the INBDPOW20PNOISEDBM1 field in the AGCINBDPOW20PNOISESTAT register.
 */
__INLINE uint8_t riu_inbdpow20pnoisedbm1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PNOISESTAT_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Returns the current value of the INBDPOW20PNOISEDBM0 field in the AGCINBDPOW20PNOISESTAT register.
 *
 * The AGCINBDPOW20PNOISESTAT register will be read and the INBDPOW20PNOISEDBM0 field's value will be returned.
 *
 * @return The current value of the INBDPOW20PNOISEDBM0 field in the AGCINBDPOW20PNOISESTAT register.
 */
__INLINE uint8_t riu_inbdpow20pnoisedbm0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOW20PNOISESTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name AGCINBDPOWSECSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  23:16        INBDPOW40SDBM   0x0
 *  07:00        INBDPOW20SDBM   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCINBDPOWSECSTAT register
#define RIU_AGCINBDPOWSECSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB22C)
/// Offset of the AGCINBDPOWSECSTAT register from the base address
#define RIU_AGCINBDPOWSECSTAT_OFFSET 0x0000022C
/// Index of the AGCINBDPOWSECSTAT register
#define RIU_AGCINBDPOWSECSTAT_INDEX  0x0000008B
/// Reset value of the AGCINBDPOWSECSTAT register
#define RIU_AGCINBDPOWSECSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCINBDPOWSECSTAT register.
 * The AGCINBDPOWSECSTAT register will be read and its value returned.
 * @return The current value of the AGCINBDPOWSECSTAT register.
 */
__INLINE uint32_t riu_agcinbdpowsecstat_get(void)
{
    return REG_PL_RD(RIU_AGCINBDPOWSECSTAT_ADDR);
}

// field definitions
/// INBDPOW40SDBM field mask
#define RIU_INBDPOW40SDBM_MASK   ((uint32_t)0x00FF0000)
/// INBDPOW40SDBM field LSB position
#define RIU_INBDPOW40SDBM_LSB    16
/// INBDPOW40SDBM field width
#define RIU_INBDPOW40SDBM_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20SDBM field mask
#define RIU_INBDPOW20SDBM_MASK   ((uint32_t)0x000000FF)
/// INBDPOW20SDBM field LSB position
#define RIU_INBDPOW20SDBM_LSB    0
/// INBDPOW20SDBM field width
#define RIU_INBDPOW20SDBM_WIDTH  ((uint32_t)0x00000008)

/// INBDPOW40SDBM field reset value
#define RIU_INBDPOW40SDBM_RST    0x0
/// INBDPOW20SDBM field reset value
#define RIU_INBDPOW20SDBM_RST    0x0

/**
 * @brief Unpacks AGCINBDPOWSECSTAT's fields from current value of the AGCINBDPOWSECSTAT register.
 *
 * Reads the AGCINBDPOWSECSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpow40sdbm - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20sdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcinbdpowsecstat_unpack(uint8_t* inbdpow40sdbm, uint8_t* inbdpow20sdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECSTAT_ADDR);

    *inbdpow40sdbm = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *inbdpow20sdbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOW40SDBM field in the AGCINBDPOWSECSTAT register.
 *
 * The AGCINBDPOWSECSTAT register will be read and the INBDPOW40SDBM field's value will be returned.
 *
 * @return The current value of the INBDPOW40SDBM field in the AGCINBDPOWSECSTAT register.
 */
__INLINE uint8_t riu_inbdpow40sdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Returns the current value of the INBDPOW20SDBM field in the AGCINBDPOWSECSTAT register.
 *
 * The AGCINBDPOWSECSTAT register will be read and the INBDPOW20SDBM field's value will be returned.
 *
 * @return The current value of the INBDPOW20SDBM field in the AGCINBDPOWSECSTAT register.
 */
__INLINE uint8_t riu_inbdpow20sdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECSTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name AGCINBDPOWSECNOISESTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  23:16   INBDPOW40SNOISEDBM   0x0
 *  07:00   INBDPOW20SNOISEDBM   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCINBDPOWSECNOISESTAT register
#define RIU_AGCINBDPOWSECNOISESTAT_ADDR   (REG_RIU_BASE_ADDR+0xB230)
/// Offset of the AGCINBDPOWSECNOISESTAT register from the base address
#define RIU_AGCINBDPOWSECNOISESTAT_OFFSET 0x00000230
/// Index of the AGCINBDPOWSECNOISESTAT register
#define RIU_AGCINBDPOWSECNOISESTAT_INDEX  0x0000008C
/// Reset value of the AGCINBDPOWSECNOISESTAT register
#define RIU_AGCINBDPOWSECNOISESTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCINBDPOWSECNOISESTAT register.
 * The AGCINBDPOWSECNOISESTAT register will be read and its value returned.
 * @return The current value of the AGCINBDPOWSECNOISESTAT register.
 */
__INLINE uint32_t riu_agcinbdpowsecnoisestat_get(void)
{
    return REG_PL_RD(RIU_AGCINBDPOWSECNOISESTAT_ADDR);
}

// field definitions
/// INBDPOW40SNOISEDBM field mask
#define RIU_INBDPOW40SNOISEDBM_MASK   ((uint32_t)0x00FF0000)
/// INBDPOW40SNOISEDBM field LSB position
#define RIU_INBDPOW40SNOISEDBM_LSB    16
/// INBDPOW40SNOISEDBM field width
#define RIU_INBDPOW40SNOISEDBM_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20SNOISEDBM field mask
#define RIU_INBDPOW20SNOISEDBM_MASK   ((uint32_t)0x000000FF)
/// INBDPOW20SNOISEDBM field LSB position
#define RIU_INBDPOW20SNOISEDBM_LSB    0
/// INBDPOW20SNOISEDBM field width
#define RIU_INBDPOW20SNOISEDBM_WIDTH  ((uint32_t)0x00000008)

/// INBDPOW40SNOISEDBM field reset value
#define RIU_INBDPOW40SNOISEDBM_RST    0x0
/// INBDPOW20SNOISEDBM field reset value
#define RIU_INBDPOW20SNOISEDBM_RST    0x0

/**
 * @brief Unpacks AGCINBDPOWSECNOISESTAT's fields from current value of the AGCINBDPOWSECNOISESTAT register.
 *
 * Reads the AGCINBDPOWSECNOISESTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpow40snoisedbm - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20snoisedbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_agcinbdpowsecnoisestat_unpack(uint8_t* inbdpow40snoisedbm, uint8_t* inbdpow20snoisedbm)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECNOISESTAT_ADDR);

    *inbdpow40snoisedbm = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *inbdpow20snoisedbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOW40SNOISEDBM field in the AGCINBDPOWSECNOISESTAT register.
 *
 * The AGCINBDPOWSECNOISESTAT register will be read and the INBDPOW40SNOISEDBM field's value will be returned.
 *
 * @return The current value of the INBDPOW40SNOISEDBM field in the AGCINBDPOWSECNOISESTAT register.
 */
__INLINE uint8_t riu_inbdpow40snoisedbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECNOISESTAT_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Returns the current value of the INBDPOW20SNOISEDBM field in the AGCINBDPOWSECNOISESTAT register.
 *
 * The AGCINBDPOWSECNOISESTAT register will be read and the INBDPOW20SNOISEDBM field's value will be returned.
 *
 * @return The current value of the INBDPOW20SNOISEDBM field in the AGCINBDPOWSECNOISESTAT register.
 */
__INLINE uint8_t riu_inbdpow20snoisedbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCINBDPOWSECNOISESTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name AGCMEMFIFOSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00      AGCMEMFIFOEMPTY   0
 * </pre>
 *
 * @{
 */

/// Address of the AGCMEMFIFOSTAT register
#define RIU_AGCMEMFIFOSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB234)
/// Offset of the AGCMEMFIFOSTAT register from the base address
#define RIU_AGCMEMFIFOSTAT_OFFSET 0x00000234
/// Index of the AGCMEMFIFOSTAT register
#define RIU_AGCMEMFIFOSTAT_INDEX  0x0000008D
/// Reset value of the AGCMEMFIFOSTAT register
#define RIU_AGCMEMFIFOSTAT_RESET  0x00000000
///#define RIU_AGCMEMFIFOSTAT_RESET  0x00000001

/**
 * @brief Returns the current value of the AGCMEMFIFOSTAT register.
 * The AGCMEMFIFOSTAT register will be read and its value returned.
 * @return The current value of the AGCMEMFIFOSTAT register.
 */
__INLINE uint32_t riu_agcmemfifostat_get(void)
{
    return REG_PL_RD(RIU_AGCMEMFIFOSTAT_ADDR);
}

// field definitions
/// AGCMEMFIFOEMPTY field bit
#define RIU_AGCMEMFIFOEMPTY_BIT    ((uint32_t)0x00000001)
/// AGCMEMFIFOEMPTY field position
#define RIU_AGCMEMFIFOEMPTY_POS    0

/// AGCMEMFIFOEMPTY field reset value
#define RIU_AGCMEMFIFOEMPTY_RST    0x0

/**
 * @brief Returns the current value of the AGCMEMFIFOEMPTY field in the AGCMEMFIFOSTAT register.
 *
 * The AGCMEMFIFOSTAT register will be read and the AGCMEMFIFOEMPTY field's value will be returned.
 *
 * @return The current value of the AGCMEMFIFOEMPTY field in the AGCMEMFIFOSTAT register.
 */
__INLINE uint8_t riu_agcmemfifoempty_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCMEMFIFOSTAT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name AGCMEMBISTSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     00      AGCMEMBISTMATCH   0
 * </pre>
 *
 * @{
 */

/// Address of the AGCMEMBISTSTAT register
#define RIU_AGCMEMBISTSTAT_ADDR   (REG_RIU_BASE_ADDR+0xB238)
/// Offset of the AGCMEMBISTSTAT register from the base address
#define RIU_AGCMEMBISTSTAT_OFFSET 0x00000238
/// Index of the AGCMEMBISTSTAT register
#define RIU_AGCMEMBISTSTAT_INDEX  0x0000008E
/// Reset value of the AGCMEMBISTSTAT register
#define RIU_AGCMEMBISTSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCMEMBISTSTAT register.
 * The AGCMEMBISTSTAT register will be read and its value returned.
 * @return The current value of the AGCMEMBISTSTAT register.
 */
__INLINE uint32_t riu_agcmembiststat_get(void)
{
    return REG_PL_RD(RIU_AGCMEMBISTSTAT_ADDR);
}

// field definitions
/// AGCMEMBISTMATCH field bit
#define RIU_AGCMEMBISTMATCH_BIT    ((uint32_t)0x00000001)
/// AGCMEMBISTMATCH field position
#define RIU_AGCMEMBISTMATCH_POS    0

/// AGCMEMBISTMATCH field reset value
#define RIU_AGCMEMBISTMATCH_RST    0x0

/**
 * @brief Returns the current value of the AGCMEMBISTMATCH field in the AGCMEMBISTSTAT register.
 *
 * The AGCMEMBISTSTAT register will be read and the AGCMEMBISTMATCH field's value will be returned.
 *
 * @return The current value of the AGCMEMBISTMATCH field in the AGCMEMBISTSTAT register.
 */
__INLINE uint8_t riu_agcmembistmatch_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCMEMBISTSTAT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x00000001)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name AGCMEMSIGNATURESTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00      AGCMEMSIGNATURE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the AGCMEMSIGNATURESTAT register
#define RIU_AGCMEMSIGNATURESTAT_ADDR   (REG_RIU_BASE_ADDR+0xB23C)
/// Offset of the AGCMEMSIGNATURESTAT register from the base address
#define RIU_AGCMEMSIGNATURESTAT_OFFSET 0x0000023C
/// Index of the AGCMEMSIGNATURESTAT register
#define RIU_AGCMEMSIGNATURESTAT_INDEX  0x0000008F
/// Reset value of the AGCMEMSIGNATURESTAT register
#define RIU_AGCMEMSIGNATURESTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the AGCMEMSIGNATURESTAT register.
 * The AGCMEMSIGNATURESTAT register will be read and its value returned.
 * @return The current value of the AGCMEMSIGNATURESTAT register.
 */
__INLINE uint32_t riu_agcmemsignaturestat_get(void)
{
    return REG_PL_RD(RIU_AGCMEMSIGNATURESTAT_ADDR);
}

// field definitions
/// AGCMEMSIGNATURE field mask
#define RIU_AGCMEMSIGNATURE_MASK   ((uint32_t)0xFFFFFFFF)
/// AGCMEMSIGNATURE field LSB position
#define RIU_AGCMEMSIGNATURE_LSB    0
/// AGCMEMSIGNATURE field width
#define RIU_AGCMEMSIGNATURE_WIDTH  ((uint32_t)0x00000020)

/// AGCMEMSIGNATURE field reset value
#define RIU_AGCMEMSIGNATURE_RST    0x0

/**
 * @brief Returns the current value of the AGCMEMSIGNATURE field in the AGCMEMSIGNATURESTAT register.
 *
 * The AGCMEMSIGNATURESTAT register will be read and the AGCMEMSIGNATURE field's value will be returned.
 *
 * @return The current value of the AGCMEMSIGNATURE field in the AGCMEMSIGNATURESTAT register.
 */
__INLINE uint32_t riu_agcmemsignature_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_AGCMEMSIGNATURESTAT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name RWNXAGCANTLOSS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  11:08         RFLOSSANT1DB   0x0
 *  03:00         RFLOSSANT0DB   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCANTLOSS register
#define RIU_RWNXAGCANTLOSS_ADDR   (REG_RIU_BASE_ADDR+0xB300)
/// Offset of the RWNXAGCANTLOSS register from the base address
#define RIU_RWNXAGCANTLOSS_OFFSET 0x00000300
/// Index of the RWNXAGCANTLOSS register
#define RIU_RWNXAGCANTLOSS_INDEX  0x000000C0
/// Reset value of the RWNXAGCANTLOSS register
#define RIU_RWNXAGCANTLOSS_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCANTLOSS register.
 * The RWNXAGCANTLOSS register will be read and its value returned.
 * @return The current value of the RWNXAGCANTLOSS register.
 */
__INLINE uint32_t riu_rwnxagcantloss_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR);
}

/**
 * @brief Sets the RWNXAGCANTLOSS register to a value.
 * The RWNXAGCANTLOSS register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcantloss_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCANTLOSS_ADDR, value);
}

// field definitions
/// RFLOSSANT1DB field mask
#define RIU_RFLOSSANT1DB_MASK   ((uint32_t)0x00000F00)
/// RFLOSSANT1DB field LSB position
#define RIU_RFLOSSANT1DB_LSB    8
/// RFLOSSANT1DB field width
#define RIU_RFLOSSANT1DB_WIDTH  ((uint32_t)0x00000004)
/// RFLOSSANT0DB field mask
#define RIU_RFLOSSANT0DB_MASK   ((uint32_t)0x0000000F)
/// RFLOSSANT0DB field LSB position
#define RIU_RFLOSSANT0DB_LSB    0
/// RFLOSSANT0DB field width
#define RIU_RFLOSSANT0DB_WIDTH  ((uint32_t)0x00000004)

/// RFLOSSANT1DB field reset value
#define RIU_RFLOSSANT1DB_RST    0x0
/// RFLOSSANT0DB field reset value
#define RIU_RFLOSSANT0DB_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCANTLOSS register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rflossant1db - The value to use for the RFLOSSANT1DB field.
 * @param[in] rflossant0db - The value to use for the RFLOSSANT0DB field.
 */
__INLINE void riu_rwnxagcantloss_pack(uint8_t rflossant1db, uint8_t rflossant0db)
{
    ASSERT_ERR((((uint32_t)rflossant1db << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)rflossant0db << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(RIU_RWNXAGCANTLOSS_ADDR,  ((uint32_t)rflossant1db << 8) | ((uint32_t)rflossant0db << 0));
}

/**
 * @brief Unpacks RWNXAGCANTLOSS's fields from current value of the RWNXAGCANTLOSS register.
 *
 * Reads the RWNXAGCANTLOSS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rflossant1db - Will be populated with the current value of this field from the register.
 * @param[out] rflossant0db - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcantloss_unpack(uint8_t* rflossant1db, uint8_t* rflossant0db)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR);

    *rflossant1db = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *rflossant0db = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the RFLOSSANT1DB field in the RWNXAGCANTLOSS register.
 *
 * The RWNXAGCANTLOSS register will be read and the RFLOSSANT1DB field's value will be returned.
 *
 * @return The current value of the RFLOSSANT1DB field in the RWNXAGCANTLOSS register.
 */
__INLINE uint8_t riu_rflossant1db_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the RFLOSSANT1DB field of the RWNXAGCANTLOSS register.
 *
 * The RWNXAGCANTLOSS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rflossant1db - The value to set the field to.
 */
__INLINE void riu_rflossant1db_setf(uint8_t rflossant1db)
{
    ASSERT_ERR((((uint32_t)rflossant1db << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCANTLOSS_ADDR, (REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)rflossant1db << 8));
}

/**
 * @brief Returns the current value of the RFLOSSANT0DB field in the RWNXAGCANTLOSS register.
 *
 * The RWNXAGCANTLOSS register will be read and the RFLOSSANT0DB field's value will be returned.
 *
 * @return The current value of the RFLOSSANT0DB field in the RWNXAGCANTLOSS register.
 */
__INLINE uint8_t riu_rflossant0db_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the RFLOSSANT0DB field of the RWNXAGCANTLOSS register.
 *
 * The RWNXAGCANTLOSS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rflossant0db - The value to set the field to.
 */
__INLINE void riu_rflossant0db_setf(uint8_t rflossant0db)
{
    ASSERT_ERR((((uint32_t)rflossant0db << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(RIU_RWNXAGCANTLOSS_ADDR, (REG_PL_RD(RIU_RWNXAGCANTLOSS_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)rflossant0db << 0));
}

/// @}

/**
 * @name RWNXAGCGAINRANGE register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  30:24      RFGAINMAX4DETDB   0x42
 *  22:16      RFGAINMIN4DETDB   0x0
 *  14:08          RFGAINMAXDB   0x42
 *  06:00          RFGAINMINDB   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCGAINRANGE register
#define RIU_RWNXAGCGAINRANGE_ADDR   (REG_RIU_BASE_ADDR+0xB304)
/// Offset of the RWNXAGCGAINRANGE register from the base address
#define RIU_RWNXAGCGAINRANGE_OFFSET 0x00000304
/// Index of the RWNXAGCGAINRANGE register
#define RIU_RWNXAGCGAINRANGE_INDEX  0x000000C1
/// Reset value of the RWNXAGCGAINRANGE register
#define RIU_RWNXAGCGAINRANGE_RESET  0x42004200

/**
 * @brief Returns the current value of the RWNXAGCGAINRANGE register.
 * The RWNXAGCGAINRANGE register will be read and its value returned.
 * @return The current value of the RWNXAGCGAINRANGE register.
 */
__INLINE uint32_t riu_rwnxagcgainrange_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);
}

/**
 * @brief Sets the RWNXAGCGAINRANGE register to a value.
 * The RWNXAGCGAINRANGE register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcgainrange_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR, value);
}

// field definitions
/// RFGAINMAX4DETDB field mask
#define RIU_RFGAINMAX4DETDB_MASK   ((uint32_t)0x7F000000)
/// RFGAINMAX4DETDB field LSB position
#define RIU_RFGAINMAX4DETDB_LSB    24
/// RFGAINMAX4DETDB field width
#define RIU_RFGAINMAX4DETDB_WIDTH  ((uint32_t)0x00000007)
/// RFGAINMIN4DETDB field mask
#define RIU_RFGAINMIN4DETDB_MASK   ((uint32_t)0x007F0000)
/// RFGAINMIN4DETDB field LSB position
#define RIU_RFGAINMIN4DETDB_LSB    16
/// RFGAINMIN4DETDB field width
#define RIU_RFGAINMIN4DETDB_WIDTH  ((uint32_t)0x00000007)
/// RFGAINMAXDB field mask
#define RIU_RFGAINMAXDB_MASK       ((uint32_t)0x00007F00)
/// RFGAINMAXDB field LSB position
#define RIU_RFGAINMAXDB_LSB        8
/// RFGAINMAXDB field width
#define RIU_RFGAINMAXDB_WIDTH      ((uint32_t)0x00000007)
/// RFGAINMINDB field mask
#define RIU_RFGAINMINDB_MASK       ((uint32_t)0x0000007F)
/// RFGAINMINDB field LSB position
#define RIU_RFGAINMINDB_LSB        0
/// RFGAINMINDB field width
#define RIU_RFGAINMINDB_WIDTH      ((uint32_t)0x00000007)

/// RFGAINMAX4DETDB field reset value
#define RIU_RFGAINMAX4DETDB_RST    0x42
/// RFGAINMIN4DETDB field reset value
#define RIU_RFGAINMIN4DETDB_RST    0x0
/// RFGAINMAXDB field reset value
#define RIU_RFGAINMAXDB_RST        0x42
/// RFGAINMINDB field reset value
#define RIU_RFGAINMINDB_RST        0x0

/**
 * @brief Constructs a value for the RWNXAGCGAINRANGE register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rfgainmax4detdb - The value to use for the RFGAINMAX4DETDB field.
 * @param[in] rfgainmin4detdb - The value to use for the RFGAINMIN4DETDB field.
 * @param[in] rfgainmaxdb - The value to use for the RFGAINMAXDB field.
 * @param[in] rfgainmindb - The value to use for the RFGAINMINDB field.
 */
__INLINE void riu_rwnxagcgainrange_pack(uint8_t rfgainmax4detdb, uint8_t rfgainmin4detdb, uint8_t rfgainmaxdb, uint8_t rfgainmindb)
{
    ASSERT_ERR((((uint32_t)rfgainmax4detdb << 24) & ~((uint32_t)0x7F000000)) == 0);
    ASSERT_ERR((((uint32_t)rfgainmin4detdb << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)rfgainmaxdb << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)rfgainmindb << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR,  ((uint32_t)rfgainmax4detdb << 24) | ((uint32_t)rfgainmin4detdb << 16) | ((uint32_t)rfgainmaxdb << 8) | ((uint32_t)rfgainmindb << 0));
}

/**
 * @brief Unpacks RWNXAGCGAINRANGE's fields from current value of the RWNXAGCGAINRANGE register.
 *
 * Reads the RWNXAGCGAINRANGE register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rfgainmax4detdb - Will be populated with the current value of this field from the register.
 * @param[out] rfgainmin4detdb - Will be populated with the current value of this field from the register.
 * @param[out] rfgainmaxdb - Will be populated with the current value of this field from the register.
 * @param[out] rfgainmindb - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcgainrange_unpack(uint8_t* rfgainmax4detdb, uint8_t* rfgainmin4detdb, uint8_t* rfgainmaxdb, uint8_t* rfgainmindb)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);

    *rfgainmax4detdb = (localVal & ((uint32_t)0x7F000000)) >> 24;
    *rfgainmin4detdb = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *rfgainmaxdb = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *rfgainmindb = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the RFGAINMAX4DETDB field in the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read and the RFGAINMAX4DETDB field's value will be returned.
 *
 * @return The current value of the RFGAINMAX4DETDB field in the RWNXAGCGAINRANGE register.
 */
__INLINE uint8_t riu_rfgainmax4detdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);
    return ((localVal & ((uint32_t)0x7F000000)) >> 24);
}

/**
 * @brief Sets the RFGAINMAX4DETDB field of the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rfgainmax4detdb - The value to set the field to.
 */
__INLINE void riu_rfgainmax4detdb_setf(uint8_t rfgainmax4detdb)
{
    ASSERT_ERR((((uint32_t)rfgainmax4detdb << 24) & ~((uint32_t)0x7F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR, (REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR) & ~((uint32_t)0x7F000000)) | ((uint32_t)rfgainmax4detdb << 24));
}

/**
 * @brief Returns the current value of the RFGAINMIN4DETDB field in the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read and the RFGAINMIN4DETDB field's value will be returned.
 *
 * @return The current value of the RFGAINMIN4DETDB field in the RWNXAGCGAINRANGE register.
 */
__INLINE uint8_t riu_rfgainmin4detdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the RFGAINMIN4DETDB field of the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rfgainmin4detdb - The value to set the field to.
 */
__INLINE void riu_rfgainmin4detdb_setf(uint8_t rfgainmin4detdb)
{
    ASSERT_ERR((((uint32_t)rfgainmin4detdb << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR, (REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)rfgainmin4detdb << 16));
}

/**
 * @brief Returns the current value of the RFGAINMAXDB field in the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read and the RFGAINMAXDB field's value will be returned.
 *
 * @return The current value of the RFGAINMAXDB field in the RWNXAGCGAINRANGE register.
 */
__INLINE uint8_t riu_rfgainmaxdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the RFGAINMAXDB field of the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rfgainmaxdb - The value to set the field to.
 */
__INLINE void riu_rfgainmaxdb_setf(uint8_t rfgainmaxdb)
{
    ASSERT_ERR((((uint32_t)rfgainmaxdb << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR, (REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)rfgainmaxdb << 8));
}

/**
 * @brief Returns the current value of the RFGAINMINDB field in the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read and the RFGAINMINDB field's value will be returned.
 *
 * @return The current value of the RFGAINMINDB field in the RWNXAGCGAINRANGE register.
 */
__INLINE uint8_t riu_rfgainmindb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the RFGAINMINDB field of the RWNXAGCGAINRANGE register.
 *
 * The RWNXAGCGAINRANGE register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rfgainmindb - The value to set the field to.
 */
__INLINE void riu_rfgainmindb_setf(uint8_t rfgainmindb)
{
    ASSERT_ERR((((uint32_t)rfgainmindb << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCGAINRANGE_ADDR, (REG_PL_RD(RIU_RWNXAGCGAINRANGE_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)rfgainmindb << 0));
}

/// @}

/**
 * @name RWNXAGCDIGGAINCONF register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  19:16        DIGGAIN80VS20   0x0
 *  03:00        DIGGAIN40VS20   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDIGGAINCONF register
#define RIU_RWNXAGCDIGGAINCONF_ADDR   (REG_RIU_BASE_ADDR+0xB308)
/// Offset of the RWNXAGCDIGGAINCONF register from the base address
#define RIU_RWNXAGCDIGGAINCONF_OFFSET 0x00000308
/// Index of the RWNXAGCDIGGAINCONF register
#define RIU_RWNXAGCDIGGAINCONF_INDEX  0x000000C2
/// Reset value of the RWNXAGCDIGGAINCONF register
#define RIU_RWNXAGCDIGGAINCONF_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCDIGGAINCONF register.
 * The RWNXAGCDIGGAINCONF register will be read and its value returned.
 * @return The current value of the RWNXAGCDIGGAINCONF register.
 */
__INLINE uint32_t riu_rwnxagcdiggainconf_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR);
}

/**
 * @brief Sets the RWNXAGCDIGGAINCONF register to a value.
 * The RWNXAGCDIGGAINCONF register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdiggainconf_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDIGGAINCONF_ADDR, value);
}

// field definitions
/// DIGGAIN80VS20 field mask
#define RIU_DIGGAIN80VS20_MASK   ((uint32_t)0x000F0000)
/// DIGGAIN80VS20 field LSB position
#define RIU_DIGGAIN80VS20_LSB    16
/// DIGGAIN80VS20 field width
#define RIU_DIGGAIN80VS20_WIDTH  ((uint32_t)0x00000004)
/// DIGGAIN40VS20 field mask
#define RIU_DIGGAIN40VS20_MASK   ((uint32_t)0x0000000F)
/// DIGGAIN40VS20 field LSB position
#define RIU_DIGGAIN40VS20_LSB    0
/// DIGGAIN40VS20 field width
#define RIU_DIGGAIN40VS20_WIDTH  ((uint32_t)0x00000004)

/// DIGGAIN80VS20 field reset value
#define RIU_DIGGAIN80VS20_RST    0x0
/// DIGGAIN40VS20 field reset value
#define RIU_DIGGAIN40VS20_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDIGGAINCONF register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] diggain80vs20 - The value to use for the DIGGAIN80VS20 field.
 * @param[in] diggain40vs20 - The value to use for the DIGGAIN40VS20 field.
 */
__INLINE void riu_rwnxagcdiggainconf_pack(uint8_t diggain80vs20, uint8_t diggain40vs20)
{
    ASSERT_ERR((((uint32_t)diggain80vs20 << 16) & ~((uint32_t)0x000F0000)) == 0);
    ASSERT_ERR((((uint32_t)diggain40vs20 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDIGGAINCONF_ADDR,  ((uint32_t)diggain80vs20 << 16) | ((uint32_t)diggain40vs20 << 0));
}

/**
 * @brief Unpacks RWNXAGCDIGGAINCONF's fields from current value of the RWNXAGCDIGGAINCONF register.
 *
 * Reads the RWNXAGCDIGGAINCONF register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] diggain80vs20 - Will be populated with the current value of this field from the register.
 * @param[out] diggain40vs20 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdiggainconf_unpack(uint8_t* diggain80vs20, uint8_t* diggain40vs20)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR);

    *diggain80vs20 = (localVal & ((uint32_t)0x000F0000)) >> 16;
    *diggain40vs20 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the DIGGAIN80VS20 field in the RWNXAGCDIGGAINCONF register.
 *
 * The RWNXAGCDIGGAINCONF register will be read and the DIGGAIN80VS20 field's value will be returned.
 *
 * @return The current value of the DIGGAIN80VS20 field in the RWNXAGCDIGGAINCONF register.
 */
__INLINE uint8_t riu_diggain80vs20_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR);
    return ((localVal & ((uint32_t)0x000F0000)) >> 16);
}

/**
 * @brief Sets the DIGGAIN80VS20 field of the RWNXAGCDIGGAINCONF register.
 *
 * The RWNXAGCDIGGAINCONF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] diggain80vs20 - The value to set the field to.
 */
__INLINE void riu_diggain80vs20_setf(uint8_t diggain80vs20)
{
    ASSERT_ERR((((uint32_t)diggain80vs20 << 16) & ~((uint32_t)0x000F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDIGGAINCONF_ADDR, (REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR) & ~((uint32_t)0x000F0000)) | ((uint32_t)diggain80vs20 << 16));
}

/**
 * @brief Returns the current value of the DIGGAIN40VS20 field in the RWNXAGCDIGGAINCONF register.
 *
 * The RWNXAGCDIGGAINCONF register will be read and the DIGGAIN40VS20 field's value will be returned.
 *
 * @return The current value of the DIGGAIN40VS20 field in the RWNXAGCDIGGAINCONF register.
 */
__INLINE uint8_t riu_diggain40vs20_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the DIGGAIN40VS20 field of the RWNXAGCDIGGAINCONF register.
 *
 * The RWNXAGCDIGGAINCONF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] diggain40vs20 - The value to set the field to.
 */
__INLINE void riu_diggain40vs20_setf(uint8_t diggain40vs20)
{
    ASSERT_ERR((((uint32_t)diggain40vs20 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDIGGAINCONF_ADDR, (REG_PL_RD(RIU_RWNXAGCDIGGAINCONF_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)diggain40vs20 << 0));
}

/// @}

#if RW_NX_AGC_SNR_EN
/**
 * @name RWNXAGCNOISECONF register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16           SNRMEASSEL   0
 *  no use this filed 07:00     NOISEVARADJUSTDB   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCNOISECONF register
#define RIU_RWNXAGCNOISECONF_ADDR   (REG_RIU_BASE_ADDR+0xB30C)
/// Offset of the RWNXAGCNOISECONF register from the base address
#define RIU_RWNXAGCNOISECONF_OFFSET 0x0000030C
/// Index of the RWNXAGCNOISECONF register
#define RIU_RWNXAGCNOISECONF_INDEX  0x000000C3
/// Reset value of the RWNXAGCNOISECONF register
#define RIU_RWNXAGCNOISECONF_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCNOISECONF register.
 * The RWNXAGCNOISECONF register will be read and its value returned.
 * @return The current value of the RWNXAGCNOISECONF register.
 */
__INLINE uint32_t riu_rwnxagcnoiseconf_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR);
}

/**
 * @brief Sets the RWNXAGCNOISECONF register to a value.
 * The RWNXAGCNOISECONF register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcnoiseconf_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCNOISECONF_ADDR, value);
}

// field definitions
/// SNRMEASSEL field bit
#define RIU_SNRMEASSEL_BIT          ((uint32_t)0x00010000)
/// SNRMEASSEL field position
#define RIU_SNRMEASSEL_POS          16
/// NOISEVARADJUSTDB field mask
#define RIU_NOISEVARADJUSTDB_MASK   ((uint32_t)0x000000FF)
/// NOISEVARADJUSTDB field LSB position
#define RIU_NOISEVARADJUSTDB_LSB    0
/// NOISEVARADJUSTDB field width
#define RIU_NOISEVARADJUSTDB_WIDTH  ((uint32_t)0x00000008)

/// SNRMEASSEL field reset value
#define RIU_SNRMEASSEL_RST          0x0
/// NOISEVARADJUSTDB field reset value
#define RIU_NOISEVARADJUSTDB_RST    0x0

#if 0
/**
 * @brief Constructs a value for the RWNXAGCNOISECONF register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] snrmeassel - The value to use for the SNRMEASSEL field.
 * @param[in] noisevaradjustdb - The value to use for the NOISEVARADJUSTDB field.
 */
__INLINE void riu_rwnxagcnoiseconf_pack(uint8_t snrmeassel, uint8_t noisevaradjustdb)
{
    ASSERT_ERR((((uint32_t)snrmeassel << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)noisevaradjustdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCNOISECONF_ADDR,  ((uint32_t)snrmeassel << 16) | ((uint32_t)noisevaradjustdb << 0));
}

/**
 * @brief Unpacks RWNXAGCNOISECONF's fields from current value of the RWNXAGCNOISECONF register.
 *
 * Reads the RWNXAGCNOISECONF register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] snrmeassel - Will be populated with the current value of this field from the register.
 * @param[out] noisevaradjustdb - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcnoiseconf_unpack(uint8_t* snrmeassel, uint8_t* noisevaradjustdb)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR);

    *snrmeassel = (localVal & ((uint32_t)0x00010000)) >> 16;
    *noisevaradjustdb = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

#endif

/**
 * @brief Returns the current value of the SNRMEASSEL field in the RWNXAGCNOISECONF register.
 *
 * The RWNXAGCNOISECONF register will be read and the SNRMEASSEL field's value will be returned.
 *
 * @return The current value of the SNRMEASSEL field in the RWNXAGCNOISECONF register.
 */
__INLINE uint8_t riu_snrmeassel_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the SNRMEASSEL field of the RWNXAGCNOISECONF register.
 *
 * The RWNXAGCNOISECONF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] snrmeassel - The value to set the field to.
 */
__INLINE void riu_snrmeassel_setf(uint8_t snrmeassel)
{
    ASSERT_ERR((((uint32_t)snrmeassel << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXAGCNOISECONF_ADDR, (REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)snrmeassel << 16));
}

#if 0
/**
 * @brief Returns the current value of the NOISEVARADJUSTDB field in the RWNXAGCNOISECONF register.
 *
 * The RWNXAGCNOISECONF register will be read and the NOISEVARADJUSTDB field's value will be returned.
 *
 * @return The current value of the NOISEVARADJUSTDB field in the RWNXAGCNOISECONF register.
 */
__INLINE uint8_t riu_noisevaradjustdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the NOISEVARADJUSTDB field of the RWNXAGCNOISECONF register.
 *
 * The RWNXAGCNOISECONF register will be read, modified to contain the new field value, and written.
 *
 * @param[in] noisevaradjustdb - The value to set the field to.
 */
__INLINE void riu_noisevaradjustdb_setf(uint8_t noisevaradjustdb)
{
    ASSERT_ERR((((uint32_t)noisevaradjustdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCNOISECONF_ADDR, (REG_PL_RD(RIU_RWNXAGCNOISECONF_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)noisevaradjustdb << 0));
}
#endif

#endif // RW_NX_AGC_SNR_EN
/// @}

/**
 * @name RWNXAGCACI20MARG0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI20MARGMCS3   0x2
 *  21:16        ACI20MARGMCS2   0x4
 *  13:08        ACI20MARGMCS1   0x5
 *  05:00        ACI20MARGMCS0   0x7
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI20MARG0 register
#define RIU_RWNXAGCACI20MARG0_ADDR   (REG_RIU_BASE_ADDR+0xB340)
/// Offset of the RWNXAGCACI20MARG0 register from the base address
#define RIU_RWNXAGCACI20MARG0_OFFSET 0x00000340
/// Index of the RWNXAGCACI20MARG0 register
#define RIU_RWNXAGCACI20MARG0_INDEX  0x000000D0
/// Reset value of the RWNXAGCACI20MARG0 register
#define RIU_RWNXAGCACI20MARG0_RESET  0x02040507

/**
 * @brief Returns the current value of the RWNXAGCACI20MARG0 register.
 * The RWNXAGCACI20MARG0 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI20MARG0 register.
 */
__INLINE uint32_t riu_rwnxagcaci20marg0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI20MARG0 register to a value.
 * The RWNXAGCACI20MARG0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci20marg0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR, value);
}

// field definitions
/// ACI20MARGMCS3 field mask
#define RIU_ACI20MARGMCS3_MASK   ((uint32_t)0x3F000000)
/// ACI20MARGMCS3 field LSB position
#define RIU_ACI20MARGMCS3_LSB    24
/// ACI20MARGMCS3 field width
#define RIU_ACI20MARGMCS3_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS2 field mask
#define RIU_ACI20MARGMCS2_MASK   ((uint32_t)0x003F0000)
/// ACI20MARGMCS2 field LSB position
#define RIU_ACI20MARGMCS2_LSB    16
/// ACI20MARGMCS2 field width
#define RIU_ACI20MARGMCS2_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS1 field mask
#define RIU_ACI20MARGMCS1_MASK   ((uint32_t)0x00003F00)
/// ACI20MARGMCS1 field LSB position
#define RIU_ACI20MARGMCS1_LSB    8
/// ACI20MARGMCS1 field width
#define RIU_ACI20MARGMCS1_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS0 field mask
#define RIU_ACI20MARGMCS0_MASK   ((uint32_t)0x0000003F)
/// ACI20MARGMCS0 field LSB position
#define RIU_ACI20MARGMCS0_LSB    0
/// ACI20MARGMCS0 field width
#define RIU_ACI20MARGMCS0_WIDTH  ((uint32_t)0x00000006)

/// ACI20MARGMCS3 field reset value
#define RIU_ACI20MARGMCS3_RST    0x2
/// ACI20MARGMCS2 field reset value
#define RIU_ACI20MARGMCS2_RST    0x4
/// ACI20MARGMCS1 field reset value
#define RIU_ACI20MARGMCS1_RST    0x5
/// ACI20MARGMCS0 field reset value
#define RIU_ACI20MARGMCS0_RST    0x7

/**
 * @brief Constructs a value for the RWNXAGCACI20MARG0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci20margmcs3 - The value to use for the ACI20MARGMCS3 field.
 * @param[in] aci20margmcs2 - The value to use for the ACI20MARGMCS2 field.
 * @param[in] aci20margmcs1 - The value to use for the ACI20MARGMCS1 field.
 * @param[in] aci20margmcs0 - The value to use for the ACI20MARGMCS0 field.
 */
__INLINE void riu_rwnxagcaci20marg0_pack(uint8_t aci20margmcs3, uint8_t aci20margmcs2, uint8_t aci20margmcs1, uint8_t aci20margmcs0)
{
    ASSERT_ERR((((uint32_t)aci20margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR,  ((uint32_t)aci20margmcs3 << 24) | ((uint32_t)aci20margmcs2 << 16) | ((uint32_t)aci20margmcs1 << 8) | ((uint32_t)aci20margmcs0 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI20MARG0's fields from current value of the RWNXAGCACI20MARG0 register.
 *
 * Reads the RWNXAGCACI20MARG0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci20margmcs3 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs2 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs1 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci20marg0_unpack(uint8_t* aci20margmcs3, uint8_t* aci20margmcs2, uint8_t* aci20margmcs1, uint8_t* aci20margmcs0)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);

    *aci20margmcs3 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci20margmcs2 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci20margmcs1 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci20margmcs0 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI20MARGMCS3 field in the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read and the ACI20MARGMCS3 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS3 field in the RWNXAGCACI20MARG0 register.
 */
__INLINE uint8_t riu_aci20margmcs3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI20MARGMCS3 field of the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs3 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs3_setf(uint8_t aci20margmcs3)
{
    ASSERT_ERR((((uint32_t)aci20margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci20margmcs3 << 24));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS2 field in the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read and the ACI20MARGMCS2 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS2 field in the RWNXAGCACI20MARG0 register.
 */
__INLINE uint8_t riu_aci20margmcs2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI20MARGMCS2 field of the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs2 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs2_setf(uint8_t aci20margmcs2)
{
    ASSERT_ERR((((uint32_t)aci20margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci20margmcs2 << 16));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS1 field in the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read and the ACI20MARGMCS1 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS1 field in the RWNXAGCACI20MARG0 register.
 */
__INLINE uint8_t riu_aci20margmcs1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI20MARGMCS1 field of the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs1 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs1_setf(uint8_t aci20margmcs1)
{
    ASSERT_ERR((((uint32_t)aci20margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci20margmcs1 << 8));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS0 field in the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read and the ACI20MARGMCS0 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS0 field in the RWNXAGCACI20MARG0 register.
 */
__INLINE uint8_t riu_aci20margmcs0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI20MARGMCS0 field of the RWNXAGCACI20MARG0 register.
 *
 * The RWNXAGCACI20MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs0 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs0_setf(uint8_t aci20margmcs0)
{
    ASSERT_ERR((((uint32_t)aci20margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG0_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci20margmcs0 << 0));
}

/// @}

/**
 * @name RWNXAGCACI20MARG1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI20MARGMCS7   0x0
 *  21:16        ACI20MARGMCS6   0x0
 *  13:08        ACI20MARGMCS5   0x0
 *  05:00        ACI20MARGMCS4   0x1
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI20MARG1 register
#define RIU_RWNXAGCACI20MARG1_ADDR   (REG_RIU_BASE_ADDR+0xB344)
/// Offset of the RWNXAGCACI20MARG1 register from the base address
#define RIU_RWNXAGCACI20MARG1_OFFSET 0x00000344
/// Index of the RWNXAGCACI20MARG1 register
#define RIU_RWNXAGCACI20MARG1_INDEX  0x000000D1
/// Reset value of the RWNXAGCACI20MARG1 register
#define RIU_RWNXAGCACI20MARG1_RESET  0x00000001

/**
 * @brief Returns the current value of the RWNXAGCACI20MARG1 register.
 * The RWNXAGCACI20MARG1 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI20MARG1 register.
 */
__INLINE uint32_t riu_rwnxagcaci20marg1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI20MARG1 register to a value.
 * The RWNXAGCACI20MARG1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci20marg1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR, value);
}

// field definitions
/// ACI20MARGMCS7 field mask
#define RIU_ACI20MARGMCS7_MASK   ((uint32_t)0x3F000000)
/// ACI20MARGMCS7 field LSB position
#define RIU_ACI20MARGMCS7_LSB    24
/// ACI20MARGMCS7 field width
#define RIU_ACI20MARGMCS7_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS6 field mask
#define RIU_ACI20MARGMCS6_MASK   ((uint32_t)0x003F0000)
/// ACI20MARGMCS6 field LSB position
#define RIU_ACI20MARGMCS6_LSB    16
/// ACI20MARGMCS6 field width
#define RIU_ACI20MARGMCS6_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS5 field mask
#define RIU_ACI20MARGMCS5_MASK   ((uint32_t)0x00003F00)
/// ACI20MARGMCS5 field LSB position
#define RIU_ACI20MARGMCS5_LSB    8
/// ACI20MARGMCS5 field width
#define RIU_ACI20MARGMCS5_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS4 field mask
#define RIU_ACI20MARGMCS4_MASK   ((uint32_t)0x0000003F)
/// ACI20MARGMCS4 field LSB position
#define RIU_ACI20MARGMCS4_LSB    0
/// ACI20MARGMCS4 field width
#define RIU_ACI20MARGMCS4_WIDTH  ((uint32_t)0x00000006)

/// ACI20MARGMCS7 field reset value
#define RIU_ACI20MARGMCS7_RST    0x0
/// ACI20MARGMCS6 field reset value
#define RIU_ACI20MARGMCS6_RST    0x0
/// ACI20MARGMCS5 field reset value
#define RIU_ACI20MARGMCS5_RST    0x0
/// ACI20MARGMCS4 field reset value
#define RIU_ACI20MARGMCS4_RST    0x1

/**
 * @brief Constructs a value for the RWNXAGCACI20MARG1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci20margmcs7 - The value to use for the ACI20MARGMCS7 field.
 * @param[in] aci20margmcs6 - The value to use for the ACI20MARGMCS6 field.
 * @param[in] aci20margmcs5 - The value to use for the ACI20MARGMCS5 field.
 * @param[in] aci20margmcs4 - The value to use for the ACI20MARGMCS4 field.
 */
__INLINE void riu_rwnxagcaci20marg1_pack(uint8_t aci20margmcs7, uint8_t aci20margmcs6, uint8_t aci20margmcs5, uint8_t aci20margmcs4)
{
    ASSERT_ERR((((uint32_t)aci20margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR,  ((uint32_t)aci20margmcs7 << 24) | ((uint32_t)aci20margmcs6 << 16) | ((uint32_t)aci20margmcs5 << 8) | ((uint32_t)aci20margmcs4 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI20MARG1's fields from current value of the RWNXAGCACI20MARG1 register.
 *
 * Reads the RWNXAGCACI20MARG1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci20margmcs7 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs6 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs5 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs4 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci20marg1_unpack(uint8_t* aci20margmcs7, uint8_t* aci20margmcs6, uint8_t* aci20margmcs5, uint8_t* aci20margmcs4)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);

    *aci20margmcs7 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci20margmcs6 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci20margmcs5 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci20margmcs4 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI20MARGMCS7 field in the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read and the ACI20MARGMCS7 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS7 field in the RWNXAGCACI20MARG1 register.
 */
__INLINE uint8_t riu_aci20margmcs7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI20MARGMCS7 field of the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs7 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs7_setf(uint8_t aci20margmcs7)
{
    ASSERT_ERR((((uint32_t)aci20margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci20margmcs7 << 24));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS6 field in the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read and the ACI20MARGMCS6 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS6 field in the RWNXAGCACI20MARG1 register.
 */
__INLINE uint8_t riu_aci20margmcs6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI20MARGMCS6 field of the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs6 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs6_setf(uint8_t aci20margmcs6)
{
    ASSERT_ERR((((uint32_t)aci20margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci20margmcs6 << 16));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS5 field in the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read and the ACI20MARGMCS5 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS5 field in the RWNXAGCACI20MARG1 register.
 */
__INLINE uint8_t riu_aci20margmcs5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI20MARGMCS5 field of the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs5 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs5_setf(uint8_t aci20margmcs5)
{
    ASSERT_ERR((((uint32_t)aci20margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci20margmcs5 << 8));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS4 field in the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read and the ACI20MARGMCS4 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS4 field in the RWNXAGCACI20MARG1 register.
 */
__INLINE uint8_t riu_aci20margmcs4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI20MARGMCS4 field of the RWNXAGCACI20MARG1 register.
 *
 * The RWNXAGCACI20MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs4 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs4_setf(uint8_t aci20margmcs4)
{
    ASSERT_ERR((((uint32_t)aci20margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG1_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci20margmcs4 << 0));
}

/// @}

/**
 * @name RWNXAGCACI20MARG2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  13:08        ACI20MARGMCS9   0x0
 *  05:00        ACI20MARGMCS8   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI20MARG2 register
#define RIU_RWNXAGCACI20MARG2_ADDR   (REG_RIU_BASE_ADDR+0xB348)
/// Offset of the RWNXAGCACI20MARG2 register from the base address
#define RIU_RWNXAGCACI20MARG2_OFFSET 0x00000348
/// Index of the RWNXAGCACI20MARG2 register
#define RIU_RWNXAGCACI20MARG2_INDEX  0x000000D2
/// Reset value of the RWNXAGCACI20MARG2 register
#define RIU_RWNXAGCACI20MARG2_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCACI20MARG2 register.
 * The RWNXAGCACI20MARG2 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI20MARG2 register.
 */
__INLINE uint32_t riu_rwnxagcaci20marg2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI20MARG2 register to a value.
 * The RWNXAGCACI20MARG2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci20marg2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI20MARG2_ADDR, value);
}

// field definitions
/// ACI20MARGMCS9 field mask
#define RIU_ACI20MARGMCS9_MASK   ((uint32_t)0x00003F00)
/// ACI20MARGMCS9 field LSB position
#define RIU_ACI20MARGMCS9_LSB    8
/// ACI20MARGMCS9 field width
#define RIU_ACI20MARGMCS9_WIDTH  ((uint32_t)0x00000006)
/// ACI20MARGMCS8 field mask
#define RIU_ACI20MARGMCS8_MASK   ((uint32_t)0x0000003F)
/// ACI20MARGMCS8 field LSB position
#define RIU_ACI20MARGMCS8_LSB    0
/// ACI20MARGMCS8 field width
#define RIU_ACI20MARGMCS8_WIDTH  ((uint32_t)0x00000006)

/// ACI20MARGMCS9 field reset value
#define RIU_ACI20MARGMCS9_RST    0x0
/// ACI20MARGMCS8 field reset value
#define RIU_ACI20MARGMCS8_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCACI20MARG2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci20margmcs9 - The value to use for the ACI20MARGMCS9 field.
 * @param[in] aci20margmcs8 - The value to use for the ACI20MARGMCS8 field.
 */
__INLINE void riu_rwnxagcaci20marg2_pack(uint8_t aci20margmcs9, uint8_t aci20margmcs8)
{
    ASSERT_ERR((((uint32_t)aci20margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci20margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG2_ADDR,  ((uint32_t)aci20margmcs9 << 8) | ((uint32_t)aci20margmcs8 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI20MARG2's fields from current value of the RWNXAGCACI20MARG2 register.
 *
 * Reads the RWNXAGCACI20MARG2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci20margmcs9 - Will be populated with the current value of this field from the register.
 * @param[out] aci20margmcs8 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci20marg2_unpack(uint8_t* aci20margmcs9, uint8_t* aci20margmcs8)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR);

    *aci20margmcs9 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci20margmcs8 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI20MARGMCS9 field in the RWNXAGCACI20MARG2 register.
 *
 * The RWNXAGCACI20MARG2 register will be read and the ACI20MARGMCS9 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS9 field in the RWNXAGCACI20MARG2 register.
 */
__INLINE uint8_t riu_aci20margmcs9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI20MARGMCS9 field of the RWNXAGCACI20MARG2 register.
 *
 * The RWNXAGCACI20MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs9 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs9_setf(uint8_t aci20margmcs9)
{
    ASSERT_ERR((((uint32_t)aci20margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci20margmcs9 << 8));
}

/**
 * @brief Returns the current value of the ACI20MARGMCS8 field in the RWNXAGCACI20MARG2 register.
 *
 * The RWNXAGCACI20MARG2 register will be read and the ACI20MARGMCS8 field's value will be returned.
 *
 * @return The current value of the ACI20MARGMCS8 field in the RWNXAGCACI20MARG2 register.
 */
__INLINE uint8_t riu_aci20margmcs8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI20MARGMCS8 field of the RWNXAGCACI20MARG2 register.
 *
 * The RWNXAGCACI20MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci20margmcs8 - The value to set the field to.
 */
__INLINE void riu_aci20margmcs8_setf(uint8_t aci20margmcs8)
{
    ASSERT_ERR((((uint32_t)aci20margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI20MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI20MARG2_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci20margmcs8 << 0));
}

/// @}

/**
 * @name RWNXAGCACI40MARG0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI40MARGMCS3   0x9
 *  21:16        ACI40MARGMCS2   0xB
 *  13:08        ACI40MARGMCS1   0xD
 *  05:00        ACI40MARGMCS0   0x10
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI40MARG0 register
#define RIU_RWNXAGCACI40MARG0_ADDR   (REG_RIU_BASE_ADDR+0xB34C)
/// Offset of the RWNXAGCACI40MARG0 register from the base address
#define RIU_RWNXAGCACI40MARG0_OFFSET 0x0000034C
/// Index of the RWNXAGCACI40MARG0 register
#define RIU_RWNXAGCACI40MARG0_INDEX  0x000000D3
/// Reset value of the RWNXAGCACI40MARG0 register
#define RIU_RWNXAGCACI40MARG0_RESET  0x090B0D10

/**
 * @brief Returns the current value of the RWNXAGCACI40MARG0 register.
 * The RWNXAGCACI40MARG0 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI40MARG0 register.
 */
__INLINE uint32_t riu_rwnxagcaci40marg0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI40MARG0 register to a value.
 * The RWNXAGCACI40MARG0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci40marg0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR, value);
}

// field definitions
/// ACI40MARGMCS3 field mask
#define RIU_ACI40MARGMCS3_MASK   ((uint32_t)0x3F000000)
/// ACI40MARGMCS3 field LSB position
#define RIU_ACI40MARGMCS3_LSB    24
/// ACI40MARGMCS3 field width
#define RIU_ACI40MARGMCS3_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS2 field mask
#define RIU_ACI40MARGMCS2_MASK   ((uint32_t)0x003F0000)
/// ACI40MARGMCS2 field LSB position
#define RIU_ACI40MARGMCS2_LSB    16
/// ACI40MARGMCS2 field width
#define RIU_ACI40MARGMCS2_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS1 field mask
#define RIU_ACI40MARGMCS1_MASK   ((uint32_t)0x00003F00)
/// ACI40MARGMCS1 field LSB position
#define RIU_ACI40MARGMCS1_LSB    8
/// ACI40MARGMCS1 field width
#define RIU_ACI40MARGMCS1_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS0 field mask
#define RIU_ACI40MARGMCS0_MASK   ((uint32_t)0x0000003F)
/// ACI40MARGMCS0 field LSB position
#define RIU_ACI40MARGMCS0_LSB    0
/// ACI40MARGMCS0 field width
#define RIU_ACI40MARGMCS0_WIDTH  ((uint32_t)0x00000006)

/// ACI40MARGMCS3 field reset value
#define RIU_ACI40MARGMCS3_RST    0x9
/// ACI40MARGMCS2 field reset value
#define RIU_ACI40MARGMCS2_RST    0xB
/// ACI40MARGMCS1 field reset value
#define RIU_ACI40MARGMCS1_RST    0xD
/// ACI40MARGMCS0 field reset value
#define RIU_ACI40MARGMCS0_RST    0x10

/**
 * @brief Constructs a value for the RWNXAGCACI40MARG0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci40margmcs3 - The value to use for the ACI40MARGMCS3 field.
 * @param[in] aci40margmcs2 - The value to use for the ACI40MARGMCS2 field.
 * @param[in] aci40margmcs1 - The value to use for the ACI40MARGMCS1 field.
 * @param[in] aci40margmcs0 - The value to use for the ACI40MARGMCS0 field.
 */
__INLINE void riu_rwnxagcaci40marg0_pack(uint8_t aci40margmcs3, uint8_t aci40margmcs2, uint8_t aci40margmcs1, uint8_t aci40margmcs0)
{
    ASSERT_ERR((((uint32_t)aci40margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR,  ((uint32_t)aci40margmcs3 << 24) | ((uint32_t)aci40margmcs2 << 16) | ((uint32_t)aci40margmcs1 << 8) | ((uint32_t)aci40margmcs0 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI40MARG0's fields from current value of the RWNXAGCACI40MARG0 register.
 *
 * Reads the RWNXAGCACI40MARG0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci40margmcs3 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs2 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs1 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci40marg0_unpack(uint8_t* aci40margmcs3, uint8_t* aci40margmcs2, uint8_t* aci40margmcs1, uint8_t* aci40margmcs0)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);

    *aci40margmcs3 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci40margmcs2 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci40margmcs1 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci40margmcs0 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI40MARGMCS3 field in the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read and the ACI40MARGMCS3 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS3 field in the RWNXAGCACI40MARG0 register.
 */
__INLINE uint8_t riu_aci40margmcs3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI40MARGMCS3 field of the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs3 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs3_setf(uint8_t aci40margmcs3)
{
    ASSERT_ERR((((uint32_t)aci40margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci40margmcs3 << 24));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS2 field in the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read and the ACI40MARGMCS2 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS2 field in the RWNXAGCACI40MARG0 register.
 */
__INLINE uint8_t riu_aci40margmcs2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI40MARGMCS2 field of the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs2 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs2_setf(uint8_t aci40margmcs2)
{
    ASSERT_ERR((((uint32_t)aci40margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci40margmcs2 << 16));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS1 field in the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read and the ACI40MARGMCS1 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS1 field in the RWNXAGCACI40MARG0 register.
 */
__INLINE uint8_t riu_aci40margmcs1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI40MARGMCS1 field of the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs1 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs1_setf(uint8_t aci40margmcs1)
{
    ASSERT_ERR((((uint32_t)aci40margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci40margmcs1 << 8));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS0 field in the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read and the ACI40MARGMCS0 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS0 field in the RWNXAGCACI40MARG0 register.
 */
__INLINE uint8_t riu_aci40margmcs0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI40MARGMCS0 field of the RWNXAGCACI40MARG0 register.
 *
 * The RWNXAGCACI40MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs0 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs0_setf(uint8_t aci40margmcs0)
{
    ASSERT_ERR((((uint32_t)aci40margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG0_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci40margmcs0 << 0));
}

/// @}

/**
 * @name RWNXAGCACI40MARG1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI40MARGMCS7   0x2
 *  21:16        ACI40MARGMCS6   0x3
 *  13:08        ACI40MARGMCS5   0x3
 *  05:00        ACI40MARGMCS4   0x5
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI40MARG1 register
#define RIU_RWNXAGCACI40MARG1_ADDR   (REG_RIU_BASE_ADDR+0xB350)
/// Offset of the RWNXAGCACI40MARG1 register from the base address
#define RIU_RWNXAGCACI40MARG1_OFFSET 0x00000350
/// Index of the RWNXAGCACI40MARG1 register
#define RIU_RWNXAGCACI40MARG1_INDEX  0x000000D4
/// Reset value of the RWNXAGCACI40MARG1 register
#define RIU_RWNXAGCACI40MARG1_RESET  0x02030305

/**
 * @brief Returns the current value of the RWNXAGCACI40MARG1 register.
 * The RWNXAGCACI40MARG1 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI40MARG1 register.
 */
__INLINE uint32_t riu_rwnxagcaci40marg1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI40MARG1 register to a value.
 * The RWNXAGCACI40MARG1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci40marg1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR, value);
}

// field definitions
/// ACI40MARGMCS7 field mask
#define RIU_ACI40MARGMCS7_MASK   ((uint32_t)0x3F000000)
/// ACI40MARGMCS7 field LSB position
#define RIU_ACI40MARGMCS7_LSB    24
/// ACI40MARGMCS7 field width
#define RIU_ACI40MARGMCS7_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS6 field mask
#define RIU_ACI40MARGMCS6_MASK   ((uint32_t)0x003F0000)
/// ACI40MARGMCS6 field LSB position
#define RIU_ACI40MARGMCS6_LSB    16
/// ACI40MARGMCS6 field width
#define RIU_ACI40MARGMCS6_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS5 field mask
#define RIU_ACI40MARGMCS5_MASK   ((uint32_t)0x00003F00)
/// ACI40MARGMCS5 field LSB position
#define RIU_ACI40MARGMCS5_LSB    8
/// ACI40MARGMCS5 field width
#define RIU_ACI40MARGMCS5_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS4 field mask
#define RIU_ACI40MARGMCS4_MASK   ((uint32_t)0x0000003F)
/// ACI40MARGMCS4 field LSB position
#define RIU_ACI40MARGMCS4_LSB    0
/// ACI40MARGMCS4 field width
#define RIU_ACI40MARGMCS4_WIDTH  ((uint32_t)0x00000006)

/// ACI40MARGMCS7 field reset value
#define RIU_ACI40MARGMCS7_RST    0x2
/// ACI40MARGMCS6 field reset value
#define RIU_ACI40MARGMCS6_RST    0x3
/// ACI40MARGMCS5 field reset value
#define RIU_ACI40MARGMCS5_RST    0x3
/// ACI40MARGMCS4 field reset value
#define RIU_ACI40MARGMCS4_RST    0x5

/**
 * @brief Constructs a value for the RWNXAGCACI40MARG1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci40margmcs7 - The value to use for the ACI40MARGMCS7 field.
 * @param[in] aci40margmcs6 - The value to use for the ACI40MARGMCS6 field.
 * @param[in] aci40margmcs5 - The value to use for the ACI40MARGMCS5 field.
 * @param[in] aci40margmcs4 - The value to use for the ACI40MARGMCS4 field.
 */
__INLINE void riu_rwnxagcaci40marg1_pack(uint8_t aci40margmcs7, uint8_t aci40margmcs6, uint8_t aci40margmcs5, uint8_t aci40margmcs4)
{
    ASSERT_ERR((((uint32_t)aci40margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR,  ((uint32_t)aci40margmcs7 << 24) | ((uint32_t)aci40margmcs6 << 16) | ((uint32_t)aci40margmcs5 << 8) | ((uint32_t)aci40margmcs4 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI40MARG1's fields from current value of the RWNXAGCACI40MARG1 register.
 *
 * Reads the RWNXAGCACI40MARG1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci40margmcs7 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs6 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs5 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs4 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci40marg1_unpack(uint8_t* aci40margmcs7, uint8_t* aci40margmcs6, uint8_t* aci40margmcs5, uint8_t* aci40margmcs4)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);

    *aci40margmcs7 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci40margmcs6 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci40margmcs5 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci40margmcs4 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI40MARGMCS7 field in the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read and the ACI40MARGMCS7 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS7 field in the RWNXAGCACI40MARG1 register.
 */
__INLINE uint8_t riu_aci40margmcs7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI40MARGMCS7 field of the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs7 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs7_setf(uint8_t aci40margmcs7)
{
    ASSERT_ERR((((uint32_t)aci40margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci40margmcs7 << 24));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS6 field in the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read and the ACI40MARGMCS6 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS6 field in the RWNXAGCACI40MARG1 register.
 */
__INLINE uint8_t riu_aci40margmcs6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI40MARGMCS6 field of the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs6 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs6_setf(uint8_t aci40margmcs6)
{
    ASSERT_ERR((((uint32_t)aci40margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci40margmcs6 << 16));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS5 field in the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read and the ACI40MARGMCS5 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS5 field in the RWNXAGCACI40MARG1 register.
 */
__INLINE uint8_t riu_aci40margmcs5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI40MARGMCS5 field of the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs5 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs5_setf(uint8_t aci40margmcs5)
{
    ASSERT_ERR((((uint32_t)aci40margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci40margmcs5 << 8));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS4 field in the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read and the ACI40MARGMCS4 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS4 field in the RWNXAGCACI40MARG1 register.
 */
__INLINE uint8_t riu_aci40margmcs4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI40MARGMCS4 field of the RWNXAGCACI40MARG1 register.
 *
 * The RWNXAGCACI40MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs4 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs4_setf(uint8_t aci40margmcs4)
{
    ASSERT_ERR((((uint32_t)aci40margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG1_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci40margmcs4 << 0));
}

/// @}

/**
 * @name RWNXAGCACI40MARG2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  13:08        ACI40MARGMCS9   0x1
 *  05:00        ACI40MARGMCS8   0x1
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI40MARG2 register
#define RIU_RWNXAGCACI40MARG2_ADDR   (REG_RIU_BASE_ADDR+0xB354)
/// Offset of the RWNXAGCACI40MARG2 register from the base address
#define RIU_RWNXAGCACI40MARG2_OFFSET 0x00000354
/// Index of the RWNXAGCACI40MARG2 register
#define RIU_RWNXAGCACI40MARG2_INDEX  0x000000D5
/// Reset value of the RWNXAGCACI40MARG2 register
#define RIU_RWNXAGCACI40MARG2_RESET  0x00000101

/**
 * @brief Returns the current value of the RWNXAGCACI40MARG2 register.
 * The RWNXAGCACI40MARG2 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI40MARG2 register.
 */
__INLINE uint32_t riu_rwnxagcaci40marg2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI40MARG2 register to a value.
 * The RWNXAGCACI40MARG2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci40marg2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI40MARG2_ADDR, value);
}

// field definitions
/// ACI40MARGMCS9 field mask
#define RIU_ACI40MARGMCS9_MASK   ((uint32_t)0x00003F00)
/// ACI40MARGMCS9 field LSB position
#define RIU_ACI40MARGMCS9_LSB    8
/// ACI40MARGMCS9 field width
#define RIU_ACI40MARGMCS9_WIDTH  ((uint32_t)0x00000006)
/// ACI40MARGMCS8 field mask
#define RIU_ACI40MARGMCS8_MASK   ((uint32_t)0x0000003F)
/// ACI40MARGMCS8 field LSB position
#define RIU_ACI40MARGMCS8_LSB    0
/// ACI40MARGMCS8 field width
#define RIU_ACI40MARGMCS8_WIDTH  ((uint32_t)0x00000006)

/// ACI40MARGMCS9 field reset value
#define RIU_ACI40MARGMCS9_RST    0x1
/// ACI40MARGMCS8 field reset value
#define RIU_ACI40MARGMCS8_RST    0x1

/**
 * @brief Constructs a value for the RWNXAGCACI40MARG2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci40margmcs9 - The value to use for the ACI40MARGMCS9 field.
 * @param[in] aci40margmcs8 - The value to use for the ACI40MARGMCS8 field.
 */
__INLINE void riu_rwnxagcaci40marg2_pack(uint8_t aci40margmcs9, uint8_t aci40margmcs8)
{
    ASSERT_ERR((((uint32_t)aci40margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci40margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG2_ADDR,  ((uint32_t)aci40margmcs9 << 8) | ((uint32_t)aci40margmcs8 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI40MARG2's fields from current value of the RWNXAGCACI40MARG2 register.
 *
 * Reads the RWNXAGCACI40MARG2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci40margmcs9 - Will be populated with the current value of this field from the register.
 * @param[out] aci40margmcs8 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci40marg2_unpack(uint8_t* aci40margmcs9, uint8_t* aci40margmcs8)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR);

    *aci40margmcs9 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci40margmcs8 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI40MARGMCS9 field in the RWNXAGCACI40MARG2 register.
 *
 * The RWNXAGCACI40MARG2 register will be read and the ACI40MARGMCS9 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS9 field in the RWNXAGCACI40MARG2 register.
 */
__INLINE uint8_t riu_aci40margmcs9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI40MARGMCS9 field of the RWNXAGCACI40MARG2 register.
 *
 * The RWNXAGCACI40MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs9 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs9_setf(uint8_t aci40margmcs9)
{
    ASSERT_ERR((((uint32_t)aci40margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci40margmcs9 << 8));
}

/**
 * @brief Returns the current value of the ACI40MARGMCS8 field in the RWNXAGCACI40MARG2 register.
 *
 * The RWNXAGCACI40MARG2 register will be read and the ACI40MARGMCS8 field's value will be returned.
 *
 * @return The current value of the ACI40MARGMCS8 field in the RWNXAGCACI40MARG2 register.
 */
__INLINE uint8_t riu_aci40margmcs8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI40MARGMCS8 field of the RWNXAGCACI40MARG2 register.
 *
 * The RWNXAGCACI40MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci40margmcs8 - The value to set the field to.
 */
__INLINE void riu_aci40margmcs8_setf(uint8_t aci40margmcs8)
{
    ASSERT_ERR((((uint32_t)aci40margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI40MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI40MARG2_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci40margmcs8 << 0));
}

/// @}

/**
 * @name RWNXAGCACI80MARG0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI80MARGMCS3   0x18
 *  21:16        ACI80MARGMCS2   0x1B
 *  13:08        ACI80MARGMCS1   0x1D
 *  05:00        ACI80MARGMCS0   0x20
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI80MARG0 register
#define RIU_RWNXAGCACI80MARG0_ADDR   (REG_RIU_BASE_ADDR+0xB358)
/// Offset of the RWNXAGCACI80MARG0 register from the base address
#define RIU_RWNXAGCACI80MARG0_OFFSET 0x00000358
/// Index of the RWNXAGCACI80MARG0 register
#define RIU_RWNXAGCACI80MARG0_INDEX  0x000000D6
/// Reset value of the RWNXAGCACI80MARG0 register
#define RIU_RWNXAGCACI80MARG0_RESET  0x181B1D20

/**
 * @brief Returns the current value of the RWNXAGCACI80MARG0 register.
 * The RWNXAGCACI80MARG0 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI80MARG0 register.
 */
__INLINE uint32_t riu_rwnxagcaci80marg0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI80MARG0 register to a value.
 * The RWNXAGCACI80MARG0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci80marg0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR, value);
}

// field definitions
/// ACI80MARGMCS3 field mask
#define RIU_ACI80MARGMCS3_MASK   ((uint32_t)0x3F000000)
/// ACI80MARGMCS3 field LSB position
#define RIU_ACI80MARGMCS3_LSB    24
/// ACI80MARGMCS3 field width
#define RIU_ACI80MARGMCS3_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS2 field mask
#define RIU_ACI80MARGMCS2_MASK   ((uint32_t)0x003F0000)
/// ACI80MARGMCS2 field LSB position
#define RIU_ACI80MARGMCS2_LSB    16
/// ACI80MARGMCS2 field width
#define RIU_ACI80MARGMCS2_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS1 field mask
#define RIU_ACI80MARGMCS1_MASK   ((uint32_t)0x00003F00)
/// ACI80MARGMCS1 field LSB position
#define RIU_ACI80MARGMCS1_LSB    8
/// ACI80MARGMCS1 field width
#define RIU_ACI80MARGMCS1_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS0 field mask
#define RIU_ACI80MARGMCS0_MASK   ((uint32_t)0x0000003F)
/// ACI80MARGMCS0 field LSB position
#define RIU_ACI80MARGMCS0_LSB    0
/// ACI80MARGMCS0 field width
#define RIU_ACI80MARGMCS0_WIDTH  ((uint32_t)0x00000006)

/// ACI80MARGMCS3 field reset value
#define RIU_ACI80MARGMCS3_RST    0x18
/// ACI80MARGMCS2 field reset value
#define RIU_ACI80MARGMCS2_RST    0x1B
/// ACI80MARGMCS1 field reset value
#define RIU_ACI80MARGMCS1_RST    0x1D
/// ACI80MARGMCS0 field reset value
#define RIU_ACI80MARGMCS0_RST    0x20

/**
 * @brief Constructs a value for the RWNXAGCACI80MARG0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci80margmcs3 - The value to use for the ACI80MARGMCS3 field.
 * @param[in] aci80margmcs2 - The value to use for the ACI80MARGMCS2 field.
 * @param[in] aci80margmcs1 - The value to use for the ACI80MARGMCS1 field.
 * @param[in] aci80margmcs0 - The value to use for the ACI80MARGMCS0 field.
 */
__INLINE void riu_rwnxagcaci80marg0_pack(uint8_t aci80margmcs3, uint8_t aci80margmcs2, uint8_t aci80margmcs1, uint8_t aci80margmcs0)
{
    ASSERT_ERR((((uint32_t)aci80margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR,  ((uint32_t)aci80margmcs3 << 24) | ((uint32_t)aci80margmcs2 << 16) | ((uint32_t)aci80margmcs1 << 8) | ((uint32_t)aci80margmcs0 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI80MARG0's fields from current value of the RWNXAGCACI80MARG0 register.
 *
 * Reads the RWNXAGCACI80MARG0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci80margmcs3 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs2 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs1 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs0 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci80marg0_unpack(uint8_t* aci80margmcs3, uint8_t* aci80margmcs2, uint8_t* aci80margmcs1, uint8_t* aci80margmcs0)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);

    *aci80margmcs3 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci80margmcs2 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci80margmcs1 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci80margmcs0 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI80MARGMCS3 field in the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read and the ACI80MARGMCS3 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS3 field in the RWNXAGCACI80MARG0 register.
 */
__INLINE uint8_t riu_aci80margmcs3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI80MARGMCS3 field of the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs3 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs3_setf(uint8_t aci80margmcs3)
{
    ASSERT_ERR((((uint32_t)aci80margmcs3 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci80margmcs3 << 24));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS2 field in the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read and the ACI80MARGMCS2 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS2 field in the RWNXAGCACI80MARG0 register.
 */
__INLINE uint8_t riu_aci80margmcs2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI80MARGMCS2 field of the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs2 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs2_setf(uint8_t aci80margmcs2)
{
    ASSERT_ERR((((uint32_t)aci80margmcs2 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci80margmcs2 << 16));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS1 field in the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read and the ACI80MARGMCS1 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS1 field in the RWNXAGCACI80MARG0 register.
 */
__INLINE uint8_t riu_aci80margmcs1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI80MARGMCS1 field of the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs1 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs1_setf(uint8_t aci80margmcs1)
{
    ASSERT_ERR((((uint32_t)aci80margmcs1 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci80margmcs1 << 8));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS0 field in the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read and the ACI80MARGMCS0 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS0 field in the RWNXAGCACI80MARG0 register.
 */
__INLINE uint8_t riu_aci80margmcs0_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI80MARGMCS0 field of the RWNXAGCACI80MARG0 register.
 *
 * The RWNXAGCACI80MARG0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs0 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs0_setf(uint8_t aci80margmcs0)
{
    ASSERT_ERR((((uint32_t)aci80margmcs0 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG0_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG0_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci80margmcs0 << 0));
}

/// @}

/**
 * @name RWNXAGCACI80MARG1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24        ACI80MARGMCS7   0xE
 *  21:16        ACI80MARGMCS6   0xF
 *  13:08        ACI80MARGMCS5   0x10
 *  05:00        ACI80MARGMCS4   0x14
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI80MARG1 register
#define RIU_RWNXAGCACI80MARG1_ADDR   (REG_RIU_BASE_ADDR+0xB35C)
/// Offset of the RWNXAGCACI80MARG1 register from the base address
#define RIU_RWNXAGCACI80MARG1_OFFSET 0x0000035C
/// Index of the RWNXAGCACI80MARG1 register
#define RIU_RWNXAGCACI80MARG1_INDEX  0x000000D7
/// Reset value of the RWNXAGCACI80MARG1 register
#define RIU_RWNXAGCACI80MARG1_RESET  0x0E0F1014

/**
 * @brief Returns the current value of the RWNXAGCACI80MARG1 register.
 * The RWNXAGCACI80MARG1 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI80MARG1 register.
 */
__INLINE uint32_t riu_rwnxagcaci80marg1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI80MARG1 register to a value.
 * The RWNXAGCACI80MARG1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci80marg1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR, value);
}

// field definitions
/// ACI80MARGMCS7 field mask
#define RIU_ACI80MARGMCS7_MASK   ((uint32_t)0x3F000000)
/// ACI80MARGMCS7 field LSB position
#define RIU_ACI80MARGMCS7_LSB    24
/// ACI80MARGMCS7 field width
#define RIU_ACI80MARGMCS7_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS6 field mask
#define RIU_ACI80MARGMCS6_MASK   ((uint32_t)0x003F0000)
/// ACI80MARGMCS6 field LSB position
#define RIU_ACI80MARGMCS6_LSB    16
/// ACI80MARGMCS6 field width
#define RIU_ACI80MARGMCS6_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS5 field mask
#define RIU_ACI80MARGMCS5_MASK   ((uint32_t)0x00003F00)
/// ACI80MARGMCS5 field LSB position
#define RIU_ACI80MARGMCS5_LSB    8
/// ACI80MARGMCS5 field width
#define RIU_ACI80MARGMCS5_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS4 field mask
#define RIU_ACI80MARGMCS4_MASK   ((uint32_t)0x0000003F)
/// ACI80MARGMCS4 field LSB position
#define RIU_ACI80MARGMCS4_LSB    0
/// ACI80MARGMCS4 field width
#define RIU_ACI80MARGMCS4_WIDTH  ((uint32_t)0x00000006)

/// ACI80MARGMCS7 field reset value
#define RIU_ACI80MARGMCS7_RST    0xE
/// ACI80MARGMCS6 field reset value
#define RIU_ACI80MARGMCS6_RST    0xF
/// ACI80MARGMCS5 field reset value
#define RIU_ACI80MARGMCS5_RST    0x10
/// ACI80MARGMCS4 field reset value
#define RIU_ACI80MARGMCS4_RST    0x14

/**
 * @brief Constructs a value for the RWNXAGCACI80MARG1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci80margmcs7 - The value to use for the ACI80MARGMCS7 field.
 * @param[in] aci80margmcs6 - The value to use for the ACI80MARGMCS6 field.
 * @param[in] aci80margmcs5 - The value to use for the ACI80MARGMCS5 field.
 * @param[in] aci80margmcs4 - The value to use for the ACI80MARGMCS4 field.
 */
__INLINE void riu_rwnxagcaci80marg1_pack(uint8_t aci80margmcs7, uint8_t aci80margmcs6, uint8_t aci80margmcs5, uint8_t aci80margmcs4)
{
    ASSERT_ERR((((uint32_t)aci80margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR,  ((uint32_t)aci80margmcs7 << 24) | ((uint32_t)aci80margmcs6 << 16) | ((uint32_t)aci80margmcs5 << 8) | ((uint32_t)aci80margmcs4 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI80MARG1's fields from current value of the RWNXAGCACI80MARG1 register.
 *
 * Reads the RWNXAGCACI80MARG1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci80margmcs7 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs6 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs5 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs4 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci80marg1_unpack(uint8_t* aci80margmcs7, uint8_t* aci80margmcs6, uint8_t* aci80margmcs5, uint8_t* aci80margmcs4)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);

    *aci80margmcs7 = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *aci80margmcs6 = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *aci80margmcs5 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci80margmcs4 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI80MARGMCS7 field in the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read and the ACI80MARGMCS7 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS7 field in the RWNXAGCACI80MARG1 register.
 */
__INLINE uint8_t riu_aci80margmcs7_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the ACI80MARGMCS7 field of the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs7 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs7_setf(uint8_t aci80margmcs7)
{
    ASSERT_ERR((((uint32_t)aci80margmcs7 << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)aci80margmcs7 << 24));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS6 field in the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read and the ACI80MARGMCS6 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS6 field in the RWNXAGCACI80MARG1 register.
 */
__INLINE uint8_t riu_aci80margmcs6_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the ACI80MARGMCS6 field of the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs6 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs6_setf(uint8_t aci80margmcs6)
{
    ASSERT_ERR((((uint32_t)aci80margmcs6 << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)aci80margmcs6 << 16));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS5 field in the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read and the ACI80MARGMCS5 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS5 field in the RWNXAGCACI80MARG1 register.
 */
__INLINE uint8_t riu_aci80margmcs5_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI80MARGMCS5 field of the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs5 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs5_setf(uint8_t aci80margmcs5)
{
    ASSERT_ERR((((uint32_t)aci80margmcs5 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci80margmcs5 << 8));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS4 field in the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read and the ACI80MARGMCS4 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS4 field in the RWNXAGCACI80MARG1 register.
 */
__INLINE uint8_t riu_aci80margmcs4_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI80MARGMCS4 field of the RWNXAGCACI80MARG1 register.
 *
 * The RWNXAGCACI80MARG1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs4 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs4_setf(uint8_t aci80margmcs4)
{
    ASSERT_ERR((((uint32_t)aci80margmcs4 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG1_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG1_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci80margmcs4 << 0));
}

/// @}

/**
 * @name RWNXAGCACI80MARG2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  13:08        ACI80MARGMCS9   0x8
 *  05:00        ACI80MARGMCS8   0xA
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCACI80MARG2 register
#define RIU_RWNXAGCACI80MARG2_ADDR   (REG_RIU_BASE_ADDR+0xB360)
/// Offset of the RWNXAGCACI80MARG2 register from the base address
#define RIU_RWNXAGCACI80MARG2_OFFSET 0x00000360
/// Index of the RWNXAGCACI80MARG2 register
#define RIU_RWNXAGCACI80MARG2_INDEX  0x000000D8
/// Reset value of the RWNXAGCACI80MARG2 register
#define RIU_RWNXAGCACI80MARG2_RESET  0x0000080A

/**
 * @brief Returns the current value of the RWNXAGCACI80MARG2 register.
 * The RWNXAGCACI80MARG2 register will be read and its value returned.
 * @return The current value of the RWNXAGCACI80MARG2 register.
 */
__INLINE uint32_t riu_rwnxagcaci80marg2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR);
}

/**
 * @brief Sets the RWNXAGCACI80MARG2 register to a value.
 * The RWNXAGCACI80MARG2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcaci80marg2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCACI80MARG2_ADDR, value);
}

// field definitions
/// ACI80MARGMCS9 field mask
#define RIU_ACI80MARGMCS9_MASK   ((uint32_t)0x00003F00)
/// ACI80MARGMCS9 field LSB position
#define RIU_ACI80MARGMCS9_LSB    8
/// ACI80MARGMCS9 field width
#define RIU_ACI80MARGMCS9_WIDTH  ((uint32_t)0x00000006)
/// ACI80MARGMCS8 field mask
#define RIU_ACI80MARGMCS8_MASK   ((uint32_t)0x0000003F)
/// ACI80MARGMCS8 field LSB position
#define RIU_ACI80MARGMCS8_LSB    0
/// ACI80MARGMCS8 field width
#define RIU_ACI80MARGMCS8_WIDTH  ((uint32_t)0x00000006)

/// ACI80MARGMCS9 field reset value
#define RIU_ACI80MARGMCS9_RST    0x8
/// ACI80MARGMCS8 field reset value
#define RIU_ACI80MARGMCS8_RST    0xA

/**
 * @brief Constructs a value for the RWNXAGCACI80MARG2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] aci80margmcs9 - The value to use for the ACI80MARGMCS9 field.
 * @param[in] aci80margmcs8 - The value to use for the ACI80MARGMCS8 field.
 */
__INLINE void riu_rwnxagcaci80marg2_pack(uint8_t aci80margmcs9, uint8_t aci80margmcs8)
{
    ASSERT_ERR((((uint32_t)aci80margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)aci80margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG2_ADDR,  ((uint32_t)aci80margmcs9 << 8) | ((uint32_t)aci80margmcs8 << 0));
}

/**
 * @brief Unpacks RWNXAGCACI80MARG2's fields from current value of the RWNXAGCACI80MARG2 register.
 *
 * Reads the RWNXAGCACI80MARG2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] aci80margmcs9 - Will be populated with the current value of this field from the register.
 * @param[out] aci80margmcs8 - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcaci80marg2_unpack(uint8_t* aci80margmcs9, uint8_t* aci80margmcs8)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR);

    *aci80margmcs9 = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *aci80margmcs8 = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the ACI80MARGMCS9 field in the RWNXAGCACI80MARG2 register.
 *
 * The RWNXAGCACI80MARG2 register will be read and the ACI80MARGMCS9 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS9 field in the RWNXAGCACI80MARG2 register.
 */
__INLINE uint8_t riu_aci80margmcs9_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the ACI80MARGMCS9 field of the RWNXAGCACI80MARG2 register.
 *
 * The RWNXAGCACI80MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs9 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs9_setf(uint8_t aci80margmcs9)
{
    ASSERT_ERR((((uint32_t)aci80margmcs9 << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)aci80margmcs9 << 8));
}

/**
 * @brief Returns the current value of the ACI80MARGMCS8 field in the RWNXAGCACI80MARG2 register.
 *
 * The RWNXAGCACI80MARG2 register will be read and the ACI80MARGMCS8 field's value will be returned.
 *
 * @return The current value of the ACI80MARGMCS8 field in the RWNXAGCACI80MARG2 register.
 */
__INLINE uint8_t riu_aci80margmcs8_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the ACI80MARGMCS8 field of the RWNXAGCACI80MARG2 register.
 *
 * The RWNXAGCACI80MARG2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] aci80margmcs8 - The value to set the field to.
 */
__INLINE void riu_aci80margmcs8_setf(uint8_t aci80margmcs8)
{
    ASSERT_ERR((((uint32_t)aci80margmcs8 << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCACI80MARG2_ADDR, (REG_PL_RD(RIU_RWNXAGCACI80MARG2_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)aci80margmcs8 << 0));
}

/// @}

/**
 * @name RWNXAGCSAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  28:24         SATDELAY50NS   0x8
 *  21:16        SATHIGHTHRDBV   0x39
 *  13:08         SATLOWTHRDBV   0x35
 *  05:00            SATTHRDBV   0x36
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCSAT register
#define RIU_RWNXAGCSAT_ADDR   (REG_RIU_BASE_ADDR+0xB364)
/// Offset of the RWNXAGCSAT register from the base address
#define RIU_RWNXAGCSAT_OFFSET 0x00000364
/// Index of the RWNXAGCSAT register
#define RIU_RWNXAGCSAT_INDEX  0x000000D9
/// Reset value of the RWNXAGCSAT register
#define RIU_RWNXAGCSAT_RESET  0x08393536

/**
 * @brief Returns the current value of the RWNXAGCSAT register.
 * The RWNXAGCSAT register will be read and its value returned.
 * @return The current value of the RWNXAGCSAT register.
 */
__INLINE uint32_t riu_rwnxagcsat_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCSAT_ADDR);
}

/**
 * @brief Sets the RWNXAGCSAT register to a value.
 * The RWNXAGCSAT register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcsat_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR, value);
}

// field definitions
/// SATDELAY50NS field mask
#define RIU_SATDELAY50NS_MASK    ((uint32_t)0x1F000000)
/// SATDELAY50NS field LSB position
#define RIU_SATDELAY50NS_LSB     24
/// SATDELAY50NS field width
#define RIU_SATDELAY50NS_WIDTH   ((uint32_t)0x00000005)
/// SATHIGHTHRDBV field mask
#define RIU_SATHIGHTHRDBV_MASK   ((uint32_t)0x003F0000)
/// SATHIGHTHRDBV field LSB position
#define RIU_SATHIGHTHRDBV_LSB    16
/// SATHIGHTHRDBV field width
#define RIU_SATHIGHTHRDBV_WIDTH  ((uint32_t)0x00000006)
/// SATLOWTHRDBV field mask
#define RIU_SATLOWTHRDBV_MASK    ((uint32_t)0x00003F00)
/// SATLOWTHRDBV field LSB position
#define RIU_SATLOWTHRDBV_LSB     8
/// SATLOWTHRDBV field width
#define RIU_SATLOWTHRDBV_WIDTH   ((uint32_t)0x00000006)
/// SATTHRDBV field mask
#define RIU_SATTHRDBV_MASK       ((uint32_t)0x0000003F)
/// SATTHRDBV field LSB position
#define RIU_SATTHRDBV_LSB        0
/// SATTHRDBV field width
#define RIU_SATTHRDBV_WIDTH      ((uint32_t)0x00000006)

/// SATDELAY50NS field reset value
#define RIU_SATDELAY50NS_RST     0x8
/// SATHIGHTHRDBV field reset value
#define RIU_SATHIGHTHRDBV_RST    0x39
/// SATLOWTHRDBV field reset value
#define RIU_SATLOWTHRDBV_RST     0x35
/// SATTHRDBV field reset value
#define RIU_SATTHRDBV_RST        0x36

/**
 * @brief Constructs a value for the RWNXAGCSAT register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] satdelay50ns - The value to use for the SATDELAY50NS field.
 * @param[in] sathighthrdbv - The value to use for the SATHIGHTHRDBV field.
 * @param[in] satlowthrdbv - The value to use for the SATLOWTHRDBV field.
 * @param[in] satthrdbv - The value to use for the SATTHRDBV field.
 */
__INLINE void riu_rwnxagcsat_pack(uint8_t satdelay50ns, uint8_t sathighthrdbv, uint8_t satlowthrdbv, uint8_t satthrdbv)
{
    ASSERT_ERR((((uint32_t)satdelay50ns << 24) & ~((uint32_t)0x1F000000)) == 0);
    ASSERT_ERR((((uint32_t)sathighthrdbv << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)satlowthrdbv << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)satthrdbv << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR,  ((uint32_t)satdelay50ns << 24) | ((uint32_t)sathighthrdbv << 16) | ((uint32_t)satlowthrdbv << 8) | ((uint32_t)satthrdbv << 0));
}

/**
 * @brief Unpacks RWNXAGCSAT's fields from current value of the RWNXAGCSAT register.
 *
 * Reads the RWNXAGCSAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] satdelay50ns - Will be populated with the current value of this field from the register.
 * @param[out] sathighthrdbv - Will be populated with the current value of this field from the register.
 * @param[out] satlowthrdbv - Will be populated with the current value of this field from the register.
 * @param[out] satthrdbv - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcsat_unpack(uint8_t* satdelay50ns, uint8_t* sathighthrdbv, uint8_t* satlowthrdbv, uint8_t* satthrdbv)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSAT_ADDR);

    *satdelay50ns = (localVal & ((uint32_t)0x1F000000)) >> 24;
    *sathighthrdbv = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *satlowthrdbv = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *satthrdbv = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the SATDELAY50NS field in the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read and the SATDELAY50NS field's value will be returned.
 *
 * @return The current value of the SATDELAY50NS field in the RWNXAGCSAT register.
 */
__INLINE uint8_t riu_satdelay50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSAT_ADDR);
    return ((localVal & ((uint32_t)0x1F000000)) >> 24);
}

/**
 * @brief Sets the SATDELAY50NS field of the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satdelay50ns - The value to set the field to.
 */
__INLINE void riu_satdelay50ns_setf(uint8_t satdelay50ns)
{
    ASSERT_ERR((((uint32_t)satdelay50ns << 24) & ~((uint32_t)0x1F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR, (REG_PL_RD(RIU_RWNXAGCSAT_ADDR) & ~((uint32_t)0x1F000000)) | ((uint32_t)satdelay50ns << 24));
}

/**
 * @brief Returns the current value of the SATHIGHTHRDBV field in the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read and the SATHIGHTHRDBV field's value will be returned.
 *
 * @return The current value of the SATHIGHTHRDBV field in the RWNXAGCSAT register.
 */
__INLINE uint8_t riu_sathighthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSAT_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the SATHIGHTHRDBV field of the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] sathighthrdbv - The value to set the field to.
 */
__INLINE void riu_sathighthrdbv_setf(uint8_t sathighthrdbv)
{
    ASSERT_ERR((((uint32_t)sathighthrdbv << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR, (REG_PL_RD(RIU_RWNXAGCSAT_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)sathighthrdbv << 16));
}

/**
 * @brief Returns the current value of the SATLOWTHRDBV field in the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read and the SATLOWTHRDBV field's value will be returned.
 *
 * @return The current value of the SATLOWTHRDBV field in the RWNXAGCSAT register.
 */
__INLINE uint8_t riu_satlowthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSAT_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the SATLOWTHRDBV field of the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satlowthrdbv - The value to set the field to.
 */
__INLINE void riu_satlowthrdbv_setf(uint8_t satlowthrdbv)
{
    ASSERT_ERR((((uint32_t)satlowthrdbv << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR, (REG_PL_RD(RIU_RWNXAGCSAT_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)satlowthrdbv << 8));
}

/**
 * @brief Returns the current value of the SATTHRDBV field in the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read and the SATTHRDBV field's value will be returned.
 *
 * @return The current value of the SATTHRDBV field in the RWNXAGCSAT register.
 */
__INLINE uint8_t riu_satthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSAT_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the SATTHRDBV field of the RWNXAGCSAT register.
 *
 * The RWNXAGCSAT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satthrdbv - The value to set the field to.
 */
__INLINE void riu_satthrdbv_setf(uint8_t satthrdbv)
{
    ASSERT_ERR((((uint32_t)satthrdbv << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCSAT_ADDR, (REG_PL_RD(RIU_RWNXAGCSAT_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)satthrdbv << 0));
}

/// @}

/**
 * @name RWNXAGCCROSS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  21:12       CROSSDNTHRQDBM   0x288
 *  09:00       CROSSUPTHRQDBM   0x288
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCROSS register
#define RIU_RWNXAGCCROSS_ADDR   (REG_RIU_BASE_ADDR+0xB368)
/// Offset of the RWNXAGCCROSS register from the base address
#define RIU_RWNXAGCCROSS_OFFSET 0x00000368
/// Index of the RWNXAGCCROSS register
#define RIU_RWNXAGCCROSS_INDEX  0x000000DA
/// Reset value of the RWNXAGCCROSS register
#define RIU_RWNXAGCCROSS_RESET  0x00288288

/**
 * @brief Returns the current value of the RWNXAGCCROSS register.
 * The RWNXAGCCROSS register will be read and its value returned.
 * @return The current value of the RWNXAGCCROSS register.
 */
__INLINE uint32_t riu_rwnxagccross_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCROSS_ADDR);
}

/**
 * @brief Sets the RWNXAGCCROSS register to a value.
 * The RWNXAGCCROSS register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccross_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCROSS_ADDR, value);
}

// field definitions
/// CROSSDNTHRQDBM field mask
#define RIU_CROSSDNTHRQDBM_MASK   ((uint32_t)0x003FF000)
/// CROSSDNTHRQDBM field LSB position
#define RIU_CROSSDNTHRQDBM_LSB    12
/// CROSSDNTHRQDBM field width
#define RIU_CROSSDNTHRQDBM_WIDTH  ((uint32_t)0x0000000A)
/// CROSSUPTHRQDBM field mask
#define RIU_CROSSUPTHRQDBM_MASK   ((uint32_t)0x000003FF)
/// CROSSUPTHRQDBM field LSB position
#define RIU_CROSSUPTHRQDBM_LSB    0
/// CROSSUPTHRQDBM field width
#define RIU_CROSSUPTHRQDBM_WIDTH  ((uint32_t)0x0000000A)

/// CROSSDNTHRQDBM field reset value
#define RIU_CROSSDNTHRQDBM_RST    0x288
/// CROSSUPTHRQDBM field reset value
#define RIU_CROSSUPTHRQDBM_RST    0x288

/**
 * @brief Constructs a value for the RWNXAGCCROSS register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] crossdnthrqdbm - The value to use for the CROSSDNTHRQDBM field.
 * @param[in] crossupthrqdbm - The value to use for the CROSSUPTHRQDBM field.
 */
__INLINE void riu_rwnxagccross_pack(uint16_t crossdnthrqdbm, uint16_t crossupthrqdbm)
{
    ASSERT_ERR((((uint32_t)crossdnthrqdbm << 12) & ~((uint32_t)0x003FF000)) == 0);
    ASSERT_ERR((((uint32_t)crossupthrqdbm << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCROSS_ADDR,  ((uint32_t)crossdnthrqdbm << 12) | ((uint32_t)crossupthrqdbm << 0));
}

/**
 * @brief Unpacks RWNXAGCCROSS's fields from current value of the RWNXAGCCROSS register.
 *
 * Reads the RWNXAGCCROSS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] crossdnthrqdbm - Will be populated with the current value of this field from the register.
 * @param[out] crossupthrqdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccross_unpack(uint16_t* crossdnthrqdbm, uint16_t* crossupthrqdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCROSS_ADDR);

    *crossdnthrqdbm = (localVal & ((uint32_t)0x003FF000)) >> 12;
    *crossupthrqdbm = (localVal & ((uint32_t)0x000003FF)) >> 0;
}

/**
 * @brief Returns the current value of the CROSSDNTHRQDBM field in the RWNXAGCCROSS register.
 *
 * The RWNXAGCCROSS register will be read and the CROSSDNTHRQDBM field's value will be returned.
 *
 * @return The current value of the CROSSDNTHRQDBM field in the RWNXAGCCROSS register.
 */
__INLINE uint16_t riu_crossdnthrqdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCROSS_ADDR);
    return ((localVal & ((uint32_t)0x003FF000)) >> 12);
}

/**
 * @brief Sets the CROSSDNTHRQDBM field of the RWNXAGCCROSS register.
 *
 * The RWNXAGCCROSS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] crossdnthrqdbm - The value to set the field to.
 */
__INLINE void riu_crossdnthrqdbm_setf(uint16_t crossdnthrqdbm)
{
    ASSERT_ERR((((uint32_t)crossdnthrqdbm << 12) & ~((uint32_t)0x003FF000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCROSS_ADDR, (REG_PL_RD(RIU_RWNXAGCCROSS_ADDR) & ~((uint32_t)0x003FF000)) | ((uint32_t)crossdnthrqdbm << 12));
}

/**
 * @brief Returns the current value of the CROSSUPTHRQDBM field in the RWNXAGCCROSS register.
 *
 * The RWNXAGCCROSS register will be read and the CROSSUPTHRQDBM field's value will be returned.
 *
 * @return The current value of the CROSSUPTHRQDBM field in the RWNXAGCCROSS register.
 */
__INLINE uint16_t riu_crossupthrqdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCROSS_ADDR);
    return ((localVal & ((uint32_t)0x000003FF)) >> 0);
}

/**
 * @brief Sets the CROSSUPTHRQDBM field of the RWNXAGCCROSS register.
 *
 * The RWNXAGCCROSS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] crossupthrqdbm - The value to set the field to.
 */
__INLINE void riu_crossupthrqdbm_setf(uint16_t crossupthrqdbm)
{
    ASSERT_ERR((((uint32_t)crossupthrqdbm << 0) & ~((uint32_t)0x000003FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCROSS_ADDR, (REG_PL_RD(RIU_RWNXAGCCROSS_ADDR) & ~((uint32_t)0x000003FF)) | ((uint32_t)crossupthrqdbm << 0));
}

/// @}

/**
 * @name RWNXAGCRAMP register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  26:24       RAMPDNNDLINDEX   0x5
 *  23:16         RAMPDNGAPQDB   0x20
 *  10:08       RAMPUPNDLINDEX   0x7
 *  07:00         RAMPUPGAPQDB   0x10
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCRAMP register
#define RIU_RWNXAGCRAMP_ADDR   (REG_RIU_BASE_ADDR+0xB36C)
/// Offset of the RWNXAGCRAMP register from the base address
#define RIU_RWNXAGCRAMP_OFFSET 0x0000036C
/// Index of the RWNXAGCRAMP register
#define RIU_RWNXAGCRAMP_INDEX  0x000000DB
/// Reset value of the RWNXAGCRAMP register
#define RIU_RWNXAGCRAMP_RESET  0x05200710

/**
 * @brief Returns the current value of the RWNXAGCRAMP register.
 * The RWNXAGCRAMP register will be read and its value returned.
 * @return The current value of the RWNXAGCRAMP register.
 */
__INLINE uint32_t riu_rwnxagcramp_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);
}

/**
 * @brief Sets the RWNXAGCRAMP register to a value.
 * The RWNXAGCRAMP register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcramp_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR, value);
}

// field definitions
/// RAMPDNNDLINDEX field mask
#define RIU_RAMPDNNDLINDEX_MASK   ((uint32_t)0x07000000)
/// RAMPDNNDLINDEX field LSB position
#define RIU_RAMPDNNDLINDEX_LSB    24
/// RAMPDNNDLINDEX field width
#define RIU_RAMPDNNDLINDEX_WIDTH  ((uint32_t)0x00000003)
/// RAMPDNGAPQDB field mask
#define RIU_RAMPDNGAPQDB_MASK     ((uint32_t)0x00FF0000)
/// RAMPDNGAPQDB field LSB position
#define RIU_RAMPDNGAPQDB_LSB      16
/// RAMPDNGAPQDB field width
#define RIU_RAMPDNGAPQDB_WIDTH    ((uint32_t)0x00000008)
/// RAMPUPNDLINDEX field mask
#define RIU_RAMPUPNDLINDEX_MASK   ((uint32_t)0x00000700)
/// RAMPUPNDLINDEX field LSB position
#define RIU_RAMPUPNDLINDEX_LSB    8
/// RAMPUPNDLINDEX field width
#define RIU_RAMPUPNDLINDEX_WIDTH  ((uint32_t)0x00000003)
/// RAMPUPGAPQDB field mask
#define RIU_RAMPUPGAPQDB_MASK     ((uint32_t)0x000000FF)
/// RAMPUPGAPQDB field LSB position
#define RIU_RAMPUPGAPQDB_LSB      0
/// RAMPUPGAPQDB field width
#define RIU_RAMPUPGAPQDB_WIDTH    ((uint32_t)0x00000008)

/// RAMPDNNDLINDEX field reset value
#define RIU_RAMPDNNDLINDEX_RST    0x5
/// RAMPDNGAPQDB field reset value
#define RIU_RAMPDNGAPQDB_RST      0x20
/// RAMPUPNDLINDEX field reset value
#define RIU_RAMPUPNDLINDEX_RST    0x7
/// RAMPUPGAPQDB field reset value
#define RIU_RAMPUPGAPQDB_RST      0x10

/**
 * @brief Constructs a value for the RWNXAGCRAMP register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rampdnndlindex - The value to use for the RAMPDNNDLINDEX field.
 * @param[in] rampdngapqdb - The value to use for the RAMPDNGAPQDB field.
 * @param[in] rampupndlindex - The value to use for the RAMPUPNDLINDEX field.
 * @param[in] rampupgapqdb - The value to use for the RAMPUPGAPQDB field.
 */
__INLINE void riu_rwnxagcramp_pack(uint8_t rampdnndlindex, uint8_t rampdngapqdb, uint8_t rampupndlindex, uint8_t rampupgapqdb)
{
    ASSERT_ERR((((uint32_t)rampdnndlindex << 24) & ~((uint32_t)0x07000000)) == 0);
    ASSERT_ERR((((uint32_t)rampdngapqdb << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)rampupndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    ASSERT_ERR((((uint32_t)rampupgapqdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR,  ((uint32_t)rampdnndlindex << 24) | ((uint32_t)rampdngapqdb << 16) | ((uint32_t)rampupndlindex << 8) | ((uint32_t)rampupgapqdb << 0));
}

/**
 * @brief Unpacks RWNXAGCRAMP's fields from current value of the RWNXAGCRAMP register.
 *
 * Reads the RWNXAGCRAMP register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rampdnndlindex - Will be populated with the current value of this field from the register.
 * @param[out] rampdngapqdb - Will be populated with the current value of this field from the register.
 * @param[out] rampupndlindex - Will be populated with the current value of this field from the register.
 * @param[out] rampupgapqdb - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcramp_unpack(uint8_t* rampdnndlindex, uint8_t* rampdngapqdb, uint8_t* rampupndlindex, uint8_t* rampupgapqdb)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);

    *rampdnndlindex = (localVal & ((uint32_t)0x07000000)) >> 24;
    *rampdngapqdb = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *rampupndlindex = (localVal & ((uint32_t)0x00000700)) >> 8;
    *rampupgapqdb = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the RAMPDNNDLINDEX field in the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read and the RAMPDNNDLINDEX field's value will be returned.
 *
 * @return The current value of the RAMPDNNDLINDEX field in the RWNXAGCRAMP register.
 */
__INLINE uint8_t riu_rampdnndlindex_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);
    return ((localVal & ((uint32_t)0x07000000)) >> 24);
}

/**
 * @brief Sets the RAMPDNNDLINDEX field of the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rampdnndlindex - The value to set the field to.
 */
__INLINE void riu_rampdnndlindex_setf(uint8_t rampdnndlindex)
{
    ASSERT_ERR((((uint32_t)rampdnndlindex << 24) & ~((uint32_t)0x07000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR, (REG_PL_RD(RIU_RWNXAGCRAMP_ADDR) & ~((uint32_t)0x07000000)) | ((uint32_t)rampdnndlindex << 24));
}

/**
 * @brief Returns the current value of the RAMPDNGAPQDB field in the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read and the RAMPDNGAPQDB field's value will be returned.
 *
 * @return The current value of the RAMPDNGAPQDB field in the RWNXAGCRAMP register.
 */
__INLINE uint8_t riu_rampdngapqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the RAMPDNGAPQDB field of the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rampdngapqdb - The value to set the field to.
 */
__INLINE void riu_rampdngapqdb_setf(uint8_t rampdngapqdb)
{
    ASSERT_ERR((((uint32_t)rampdngapqdb << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR, (REG_PL_RD(RIU_RWNXAGCRAMP_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)rampdngapqdb << 16));
}

/**
 * @brief Returns the current value of the RAMPUPNDLINDEX field in the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read and the RAMPUPNDLINDEX field's value will be returned.
 *
 * @return The current value of the RAMPUPNDLINDEX field in the RWNXAGCRAMP register.
 */
__INLINE uint8_t riu_rampupndlindex_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);
    return ((localVal & ((uint32_t)0x00000700)) >> 8);
}

/**
 * @brief Sets the RAMPUPNDLINDEX field of the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rampupndlindex - The value to set the field to.
 */
__INLINE void riu_rampupndlindex_setf(uint8_t rampupndlindex)
{
    ASSERT_ERR((((uint32_t)rampupndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR, (REG_PL_RD(RIU_RWNXAGCRAMP_ADDR) & ~((uint32_t)0x00000700)) | ((uint32_t)rampupndlindex << 8));
}

/**
 * @brief Returns the current value of the RAMPUPGAPQDB field in the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read and the RAMPUPGAPQDB field's value will be returned.
 *
 * @return The current value of the RAMPUPGAPQDB field in the RWNXAGCRAMP register.
 */
__INLINE uint8_t riu_rampupgapqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRAMP_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the RAMPUPGAPQDB field of the RWNXAGCRAMP register.
 *
 * The RWNXAGCRAMP register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rampupgapqdb - The value to set the field to.
 */
__INLINE void riu_rampupgapqdb_setf(uint8_t rampupgapqdb)
{
    ASSERT_ERR((((uint32_t)rampupgapqdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRAMP_ADDR, (REG_PL_RD(RIU_RWNXAGCRAMP_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)rampupgapqdb << 0));
}

/// @}

/**
 * @name RWNXAGCSTABDIS register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  27:24         ADCPOWDISNDL   0x8
 *  22:16      ADCPOWDISTHRDBV   0x54
 *  10:08         PLATNDLINDEX   0x3
 *  04:00            PLATDPQDB   0x8
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCSTABDIS register
#define RIU_RWNXAGCSTABDIS_ADDR   (REG_RIU_BASE_ADDR+0xB370)
/// Offset of the RWNXAGCSTABDIS register from the base address
#define RIU_RWNXAGCSTABDIS_OFFSET 0x00000370
/// Index of the RWNXAGCSTABDIS register
#define RIU_RWNXAGCSTABDIS_INDEX  0x000000DC
/// Reset value of the RWNXAGCSTABDIS register
#define RIU_RWNXAGCSTABDIS_RESET  0x08540308

/**
 * @brief Returns the current value of the RWNXAGCSTABDIS register.
 * The RWNXAGCSTABDIS register will be read and its value returned.
 * @return The current value of the RWNXAGCSTABDIS register.
 */
__INLINE uint32_t riu_rwnxagcstabdis_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);
}

/**
 * @brief Sets the RWNXAGCSTABDIS register to a value.
 * The RWNXAGCSTABDIS register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcstabdis_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR, value);
}

// field definitions
/// ADCPOWDISNDL field mask
#define RIU_ADCPOWDISNDL_MASK      ((uint32_t)0x0F000000)
/// ADCPOWDISNDL field LSB position
#define RIU_ADCPOWDISNDL_LSB       24
/// ADCPOWDISNDL field width
#define RIU_ADCPOWDISNDL_WIDTH     ((uint32_t)0x00000004)
/// ADCPOWDISTHRDBV field mask
#define RIU_ADCPOWDISTHRDBV_MASK   ((uint32_t)0x007F0000)
/// ADCPOWDISTHRDBV field LSB position
#define RIU_ADCPOWDISTHRDBV_LSB    16
/// ADCPOWDISTHRDBV field width
#define RIU_ADCPOWDISTHRDBV_WIDTH  ((uint32_t)0x00000007)
/// PLATNDLINDEX field mask
#define RIU_PLATNDLINDEX_MASK      ((uint32_t)0x00000700)
/// PLATNDLINDEX field LSB position
#define RIU_PLATNDLINDEX_LSB       8
/// PLATNDLINDEX field width
#define RIU_PLATNDLINDEX_WIDTH     ((uint32_t)0x00000003)
/// PLATDPQDB field mask
#define RIU_PLATDPQDB_MASK         ((uint32_t)0x0000001F)
/// PLATDPQDB field LSB position
#define RIU_PLATDPQDB_LSB          0
/// PLATDPQDB field width
#define RIU_PLATDPQDB_WIDTH        ((uint32_t)0x00000005)

/// ADCPOWDISNDL field reset value
#define RIU_ADCPOWDISNDL_RST       0x8
/// ADCPOWDISTHRDBV field reset value
#define RIU_ADCPOWDISTHRDBV_RST    0x54
/// PLATNDLINDEX field reset value
#define RIU_PLATNDLINDEX_RST       0x3
/// PLATDPQDB field reset value
#define RIU_PLATDPQDB_RST          0x8

/**
 * @brief Constructs a value for the RWNXAGCSTABDIS register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] adcpowdisndl - The value to use for the ADCPOWDISNDL field.
 * @param[in] adcpowdisthrdbv - The value to use for the ADCPOWDISTHRDBV field.
 * @param[in] platndlindex - The value to use for the PLATNDLINDEX field.
 * @param[in] platdpqdb - The value to use for the PLATDPQDB field.
 */
__INLINE void riu_rwnxagcstabdis_pack(uint8_t adcpowdisndl, uint8_t adcpowdisthrdbv, uint8_t platndlindex, uint8_t platdpqdb)
{
    ASSERT_ERR((((uint32_t)adcpowdisndl << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)adcpowdisthrdbv << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)platndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    ASSERT_ERR((((uint32_t)platdpqdb << 0) & ~((uint32_t)0x0000001F)) == 0);
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR,  ((uint32_t)adcpowdisndl << 24) | ((uint32_t)adcpowdisthrdbv << 16) | ((uint32_t)platndlindex << 8) | ((uint32_t)platdpqdb << 0));
}

/**
 * @brief Unpacks RWNXAGCSTABDIS's fields from current value of the RWNXAGCSTABDIS register.
 *
 * Reads the RWNXAGCSTABDIS register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] adcpowdisndl - Will be populated with the current value of this field from the register.
 * @param[out] adcpowdisthrdbv - Will be populated with the current value of this field from the register.
 * @param[out] platndlindex - Will be populated with the current value of this field from the register.
 * @param[out] platdpqdb - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcstabdis_unpack(uint8_t* adcpowdisndl, uint8_t* adcpowdisthrdbv, uint8_t* platndlindex, uint8_t* platdpqdb)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);

    *adcpowdisndl = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *adcpowdisthrdbv = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *platndlindex = (localVal & ((uint32_t)0x00000700)) >> 8;
    *platdpqdb = (localVal & ((uint32_t)0x0000001F)) >> 0;
}

/**
 * @brief Returns the current value of the ADCPOWDISNDL field in the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read and the ADCPOWDISNDL field's value will be returned.
 *
 * @return The current value of the ADCPOWDISNDL field in the RWNXAGCSTABDIS register.
 */
__INLINE uint8_t riu_adcpowdisndl_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the ADCPOWDISNDL field of the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowdisndl - The value to set the field to.
 */
__INLINE void riu_adcpowdisndl_setf(uint8_t adcpowdisndl)
{
    ASSERT_ERR((((uint32_t)adcpowdisndl << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR, (REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)adcpowdisndl << 24));
}

/**
 * @brief Returns the current value of the ADCPOWDISTHRDBV field in the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read and the ADCPOWDISTHRDBV field's value will be returned.
 *
 * @return The current value of the ADCPOWDISTHRDBV field in the RWNXAGCSTABDIS register.
 */
__INLINE uint8_t riu_adcpowdisthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the ADCPOWDISTHRDBV field of the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowdisthrdbv - The value to set the field to.
 */
__INLINE void riu_adcpowdisthrdbv_setf(uint8_t adcpowdisthrdbv)
{
    ASSERT_ERR((((uint32_t)adcpowdisthrdbv << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR, (REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)adcpowdisthrdbv << 16));
}

/**
 * @brief Returns the current value of the PLATNDLINDEX field in the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read and the PLATNDLINDEX field's value will be returned.
 *
 * @return The current value of the PLATNDLINDEX field in the RWNXAGCSTABDIS register.
 */
__INLINE uint8_t riu_platndlindex_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);
    return ((localVal & ((uint32_t)0x00000700)) >> 8);
}

/**
 * @brief Sets the PLATNDLINDEX field of the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] platndlindex - The value to set the field to.
 */
__INLINE void riu_platndlindex_setf(uint8_t platndlindex)
{
    ASSERT_ERR((((uint32_t)platndlindex << 8) & ~((uint32_t)0x00000700)) == 0);
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR, (REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR) & ~((uint32_t)0x00000700)) | ((uint32_t)platndlindex << 8));
}

/**
 * @brief Returns the current value of the PLATDPQDB field in the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read and the PLATDPQDB field's value will be returned.
 *
 * @return The current value of the PLATDPQDB field in the RWNXAGCSTABDIS register.
 */
__INLINE uint8_t riu_platdpqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR);
    return ((localVal & ((uint32_t)0x0000001F)) >> 0);
}

/**
 * @brief Sets the PLATDPQDB field of the RWNXAGCSTABDIS register.
 *
 * The RWNXAGCSTABDIS register will be read, modified to contain the new field value, and written.
 *
 * @param[in] platdpqdb - The value to set the field to.
 */
__INLINE void riu_platdpqdb_setf(uint8_t platdpqdb)
{
    ASSERT_ERR((((uint32_t)platdpqdb << 0) & ~((uint32_t)0x0000001F)) == 0);
    REG_PL_WR(RIU_RWNXAGCSTABDIS_ADDR, (REG_PL_RD(RIU_RWNXAGCSTABDIS_ADDR) & ~((uint32_t)0x0000001F)) | ((uint32_t)platdpqdb << 0));
}

/// @}

/**
 * @name RWNXAGCDCCOMP0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:28           DCDSSSTYPE   0x1
 *  25:24       DCCENTEREDTYPE   0x3
 *  21:20          DCAGC20TYPE   0x1
 *  17:16            DCADCTYPE   0x1
 *  14:08    DCADCHOLDTIME50NS   0x14
 *  06:00    DCADCZEROTIME50NS   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDCCOMP0 register
#define RIU_RWNXAGCDCCOMP0_ADDR   (REG_RIU_BASE_ADDR+0xB374)
/// Offset of the RWNXAGCDCCOMP0 register from the base address
#define RIU_RWNXAGCDCCOMP0_OFFSET 0x00000374
/// Index of the RWNXAGCDCCOMP0 register
#define RIU_RWNXAGCDCCOMP0_INDEX  0x000000DD
/// Reset value of the RWNXAGCDCCOMP0 register
#define RIU_RWNXAGCDCCOMP0_RESET  0x13111400

/**
 * @brief Returns the current value of the RWNXAGCDCCOMP0 register.
 * The RWNXAGCDCCOMP0 register will be read and its value returned.
 * @return The current value of the RWNXAGCDCCOMP0 register.
 */
__INLINE uint32_t riu_rwnxagcdccomp0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
}

/**
 * @brief Sets the RWNXAGCDCCOMP0 register to a value.
 * The RWNXAGCDCCOMP0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdccomp0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, value);
}

// field definitions
/// DCDSSSTYPE field mask
#define RIU_DCDSSSTYPE_MASK          ((uint32_t)0x30000000)
/// DCDSSSTYPE field LSB position
#define RIU_DCDSSSTYPE_LSB           28
/// DCDSSSTYPE field width
#define RIU_DCDSSSTYPE_WIDTH         ((uint32_t)0x00000002)
/// DCCENTEREDTYPE field mask
#define RIU_DCCENTEREDTYPE_MASK      ((uint32_t)0x03000000)
/// DCCENTEREDTYPE field LSB position
#define RIU_DCCENTEREDTYPE_LSB       24
/// DCCENTEREDTYPE field width
#define RIU_DCCENTEREDTYPE_WIDTH     ((uint32_t)0x00000002)
/// DCAGC20TYPE field mask
#define RIU_DCAGC20TYPE_MASK         ((uint32_t)0x00300000)
/// DCAGC20TYPE field LSB position
#define RIU_DCAGC20TYPE_LSB          20
/// DCAGC20TYPE field width
#define RIU_DCAGC20TYPE_WIDTH        ((uint32_t)0x00000002)
/// DCADCTYPE field mask
#define RIU_DCADCTYPE_MASK           ((uint32_t)0x00030000)
/// DCADCTYPE field LSB position
#define RIU_DCADCTYPE_LSB            16
/// DCADCTYPE field width
#define RIU_DCADCTYPE_WIDTH          ((uint32_t)0x00000002)
/// DCADCHOLDTIME50NS field mask
#define RIU_DCADCHOLDTIME50NS_MASK   ((uint32_t)0x00007F00)
/// DCADCHOLDTIME50NS field LSB position
#define RIU_DCADCHOLDTIME50NS_LSB    8
/// DCADCHOLDTIME50NS field width
#define RIU_DCADCHOLDTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DCADCZEROTIME50NS field mask
#define RIU_DCADCZEROTIME50NS_MASK   ((uint32_t)0x0000007F)
/// DCADCZEROTIME50NS field LSB position
#define RIU_DCADCZEROTIME50NS_LSB    0
/// DCADCZEROTIME50NS field width
#define RIU_DCADCZEROTIME50NS_WIDTH  ((uint32_t)0x00000007)

/// DCDSSSTYPE field reset value
#define RIU_DCDSSSTYPE_RST           0x1
/// DCCENTEREDTYPE field reset value
#define RIU_DCCENTEREDTYPE_RST       0x3
/// DCAGC20TYPE field reset value
#define RIU_DCAGC20TYPE_RST          0x1
/// DCADCTYPE field reset value
#define RIU_DCADCTYPE_RST            0x1
/// DCADCHOLDTIME50NS field reset value
#define RIU_DCADCHOLDTIME50NS_RST    0x14
/// DCADCZEROTIME50NS field reset value
#define RIU_DCADCZEROTIME50NS_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDCCOMP0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] dcdssstype - The value to use for the DCDSSSTYPE field.
 * @param[in] dccenteredtype - The value to use for the DCCENTEREDTYPE field.
 * @param[in] dcagc20type - The value to use for the DCAGC20TYPE field.
 * @param[in] dcadctype - The value to use for the DCADCTYPE field.
 * @param[in] dcadcholdtime50ns - The value to use for the DCADCHOLDTIME50NS field.
 * @param[in] dcadczerotime50ns - The value to use for the DCADCZEROTIME50NS field.
 */
__INLINE void riu_rwnxagcdccomp0_pack(uint8_t dcdssstype, uint8_t dccenteredtype, uint8_t dcagc20type, uint8_t dcadctype, uint8_t dcadcholdtime50ns, uint8_t dcadczerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcdssstype << 28) & ~((uint32_t)0x30000000)) == 0);
    ASSERT_ERR((((uint32_t)dccenteredtype << 24) & ~((uint32_t)0x03000000)) == 0);
    ASSERT_ERR((((uint32_t)dcagc20type << 20) & ~((uint32_t)0x00300000)) == 0);
    ASSERT_ERR((((uint32_t)dcadctype << 16) & ~((uint32_t)0x00030000)) == 0);
    ASSERT_ERR((((uint32_t)dcadcholdtime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)dcadczerotime50ns << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR,  ((uint32_t)dcdssstype << 28) | ((uint32_t)dccenteredtype << 24) | ((uint32_t)dcagc20type << 20) | ((uint32_t)dcadctype << 16) | ((uint32_t)dcadcholdtime50ns << 8) | ((uint32_t)dcadczerotime50ns << 0));
}

/**
 * @brief Unpacks RWNXAGCDCCOMP0's fields from current value of the RWNXAGCDCCOMP0 register.
 *
 * Reads the RWNXAGCDCCOMP0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] dcdssstype - Will be populated with the current value of this field from the register.
 * @param[out] dccenteredtype - Will be populated with the current value of this field from the register.
 * @param[out] dcagc20type - Will be populated with the current value of this field from the register.
 * @param[out] dcadctype - Will be populated with the current value of this field from the register.
 * @param[out] dcadcholdtime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dcadczerotime50ns - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdccomp0_unpack(uint8_t* dcdssstype, uint8_t* dccenteredtype, uint8_t* dcagc20type, uint8_t* dcadctype, uint8_t* dcadcholdtime50ns, uint8_t* dcadczerotime50ns)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);

    *dcdssstype = (localVal & ((uint32_t)0x30000000)) >> 28;
    *dccenteredtype = (localVal & ((uint32_t)0x03000000)) >> 24;
    *dcagc20type = (localVal & ((uint32_t)0x00300000)) >> 20;
    *dcadctype = (localVal & ((uint32_t)0x00030000)) >> 16;
    *dcadcholdtime50ns = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *dcadczerotime50ns = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the DCDSSSTYPE field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCDSSSTYPE field's value will be returned.
 *
 * @return The current value of the DCDSSSTYPE field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dcdssstype_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x30000000)) >> 28);
}

/**
 * @brief Sets the DCDSSSTYPE field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcdssstype - The value to set the field to.
 */
__INLINE void riu_dcdssstype_setf(uint8_t dcdssstype)
{
    ASSERT_ERR((((uint32_t)dcdssstype << 28) & ~((uint32_t)0x30000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x30000000)) | ((uint32_t)dcdssstype << 28));
}

/**
 * @brief Returns the current value of the DCCENTEREDTYPE field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCCENTEREDTYPE field's value will be returned.
 *
 * @return The current value of the DCCENTEREDTYPE field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dccenteredtype_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x03000000)) >> 24);
}

/**
 * @brief Sets the DCCENTEREDTYPE field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dccenteredtype - The value to set the field to.
 */
__INLINE void riu_dccenteredtype_setf(uint8_t dccenteredtype)
{
    ASSERT_ERR((((uint32_t)dccenteredtype << 24) & ~((uint32_t)0x03000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x03000000)) | ((uint32_t)dccenteredtype << 24));
}

/**
 * @brief Returns the current value of the DCAGC20TYPE field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCAGC20TYPE field's value will be returned.
 *
 * @return The current value of the DCAGC20TYPE field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dcagc20type_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x00300000)) >> 20);
}

/**
 * @brief Sets the DCAGC20TYPE field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcagc20type - The value to set the field to.
 */
__INLINE void riu_dcagc20type_setf(uint8_t dcagc20type)
{
    ASSERT_ERR((((uint32_t)dcagc20type << 20) & ~((uint32_t)0x00300000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x00300000)) | ((uint32_t)dcagc20type << 20));
}

/**
 * @brief Returns the current value of the DCADCTYPE field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCADCTYPE field's value will be returned.
 *
 * @return The current value of the DCADCTYPE field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dcadctype_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x00030000)) >> 16);
}

/**
 * @brief Sets the DCADCTYPE field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcadctype - The value to set the field to.
 */
__INLINE void riu_dcadctype_setf(uint8_t dcadctype)
{
    ASSERT_ERR((((uint32_t)dcadctype << 16) & ~((uint32_t)0x00030000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x00030000)) | ((uint32_t)dcadctype << 16));
}

/**
 * @brief Returns the current value of the DCADCHOLDTIME50NS field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCADCHOLDTIME50NS field's value will be returned.
 *
 * @return The current value of the DCADCHOLDTIME50NS field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dcadcholdtime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the DCADCHOLDTIME50NS field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcadcholdtime50ns - The value to set the field to.
 */
__INLINE void riu_dcadcholdtime50ns_setf(uint8_t dcadcholdtime50ns)
{
    ASSERT_ERR((((uint32_t)dcadcholdtime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)dcadcholdtime50ns << 8));
}

/**
 * @brief Returns the current value of the DCADCZEROTIME50NS field in the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read and the DCADCZEROTIME50NS field's value will be returned.
 *
 * @return The current value of the DCADCZEROTIME50NS field in the RWNXAGCDCCOMP0 register.
 */
__INLINE uint8_t riu_dcadczerotime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the DCADCZEROTIME50NS field of the RWNXAGCDCCOMP0 register.
 *
 * The RWNXAGCDCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcadczerotime50ns - The value to set the field to.
 */
__INLINE void riu_dcadczerotime50ns_setf(uint8_t dcadczerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcadczerotime50ns << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP0_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)dcadczerotime50ns << 0));
}

/// @}

/**
 * @name RWNXAGCDCCOMP1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  30:24   DCCENTEREDHOLDTIME50NS   0x8
 *  22:16   DCCENTEREDZEROTIME50NS   0x2
 *  14:08   DCAGC20HOLDTIME50NS   0x0
 *  06:00   DCAGC20ZEROTIME50NS   0x2
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDCCOMP1 register
#define RIU_RWNXAGCDCCOMP1_ADDR   (REG_RIU_BASE_ADDR+0xB378)
/// Offset of the RWNXAGCDCCOMP1 register from the base address
#define RIU_RWNXAGCDCCOMP1_OFFSET 0x00000378
/// Index of the RWNXAGCDCCOMP1 register
#define RIU_RWNXAGCDCCOMP1_INDEX  0x000000DE
/// Reset value of the RWNXAGCDCCOMP1 register
#define RIU_RWNXAGCDCCOMP1_RESET  0x08020002

/**
 * @brief Returns the current value of the RWNXAGCDCCOMP1 register.
 * The RWNXAGCDCCOMP1 register will be read and its value returned.
 * @return The current value of the RWNXAGCDCCOMP1 register.
 */
__INLINE uint32_t riu_rwnxagcdccomp1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);
}

/**
 * @brief Sets the RWNXAGCDCCOMP1 register to a value.
 * The RWNXAGCDCCOMP1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdccomp1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR, value);
}

// field definitions
/// DCCENTEREDHOLDTIME50NS field mask
#define RIU_DCCENTEREDHOLDTIME50NS_MASK   ((uint32_t)0x7F000000)
/// DCCENTEREDHOLDTIME50NS field LSB position
#define RIU_DCCENTEREDHOLDTIME50NS_LSB    24
/// DCCENTEREDHOLDTIME50NS field width
#define RIU_DCCENTEREDHOLDTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DCCENTEREDZEROTIME50NS field mask
#define RIU_DCCENTEREDZEROTIME50NS_MASK   ((uint32_t)0x007F0000)
/// DCCENTEREDZEROTIME50NS field LSB position
#define RIU_DCCENTEREDZEROTIME50NS_LSB    16
/// DCCENTEREDZEROTIME50NS field width
#define RIU_DCCENTEREDZEROTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DCAGC20HOLDTIME50NS field mask
#define RIU_DCAGC20HOLDTIME50NS_MASK      ((uint32_t)0x00007F00)
/// DCAGC20HOLDTIME50NS field LSB position
#define RIU_DCAGC20HOLDTIME50NS_LSB       8
/// DCAGC20HOLDTIME50NS field width
#define RIU_DCAGC20HOLDTIME50NS_WIDTH     ((uint32_t)0x00000007)
/// DCAGC20ZEROTIME50NS field mask
#define RIU_DCAGC20ZEROTIME50NS_MASK      ((uint32_t)0x0000007F)
/// DCAGC20ZEROTIME50NS field LSB position
#define RIU_DCAGC20ZEROTIME50NS_LSB       0
/// DCAGC20ZEROTIME50NS field width
#define RIU_DCAGC20ZEROTIME50NS_WIDTH     ((uint32_t)0x00000007)

/// DCCENTEREDHOLDTIME50NS field reset value
#define RIU_DCCENTEREDHOLDTIME50NS_RST    0x8
/// DCCENTEREDZEROTIME50NS field reset value
#define RIU_DCCENTEREDZEROTIME50NS_RST    0x2
/// DCAGC20HOLDTIME50NS field reset value
#define RIU_DCAGC20HOLDTIME50NS_RST       0x0
/// DCAGC20ZEROTIME50NS field reset value
#define RIU_DCAGC20ZEROTIME50NS_RST       0x2

/**
 * @brief Constructs a value for the RWNXAGCDCCOMP1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] dccenteredholdtime50ns - The value to use for the DCCENTEREDHOLDTIME50NS field.
 * @param[in] dccenteredzerotime50ns - The value to use for the DCCENTEREDZEROTIME50NS field.
 * @param[in] dcagc20holdtime50ns - The value to use for the DCAGC20HOLDTIME50NS field.
 * @param[in] dcagc20zerotime50ns - The value to use for the DCAGC20ZEROTIME50NS field.
 */
__INLINE void riu_rwnxagcdccomp1_pack(uint8_t dccenteredholdtime50ns, uint8_t dccenteredzerotime50ns, uint8_t dcagc20holdtime50ns, uint8_t dcagc20zerotime50ns)
{
    ASSERT_ERR((((uint32_t)dccenteredholdtime50ns << 24) & ~((uint32_t)0x7F000000)) == 0);
    ASSERT_ERR((((uint32_t)dccenteredzerotime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)dcagc20holdtime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    ASSERT_ERR((((uint32_t)dcagc20zerotime50ns << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR,  ((uint32_t)dccenteredholdtime50ns << 24) | ((uint32_t)dccenteredzerotime50ns << 16) | ((uint32_t)dcagc20holdtime50ns << 8) | ((uint32_t)dcagc20zerotime50ns << 0));
}

/**
 * @brief Unpacks RWNXAGCDCCOMP1's fields from current value of the RWNXAGCDCCOMP1 register.
 *
 * Reads the RWNXAGCDCCOMP1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] dccenteredholdtime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dccenteredzerotime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dcagc20holdtime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dcagc20zerotime50ns - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdccomp1_unpack(uint8_t* dccenteredholdtime50ns, uint8_t* dccenteredzerotime50ns, uint8_t* dcagc20holdtime50ns, uint8_t* dcagc20zerotime50ns)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);

    *dccenteredholdtime50ns = (localVal & ((uint32_t)0x7F000000)) >> 24;
    *dccenteredzerotime50ns = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *dcagc20holdtime50ns = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *dcagc20zerotime50ns = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the DCCENTEREDHOLDTIME50NS field in the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read and the DCCENTEREDHOLDTIME50NS field's value will be returned.
 *
 * @return The current value of the DCCENTEREDHOLDTIME50NS field in the RWNXAGCDCCOMP1 register.
 */
__INLINE uint8_t riu_dccenteredholdtime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x7F000000)) >> 24);
}

/**
 * @brief Sets the DCCENTEREDHOLDTIME50NS field of the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dccenteredholdtime50ns - The value to set the field to.
 */
__INLINE void riu_dccenteredholdtime50ns_setf(uint8_t dccenteredholdtime50ns)
{
    ASSERT_ERR((((uint32_t)dccenteredholdtime50ns << 24) & ~((uint32_t)0x7F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR) & ~((uint32_t)0x7F000000)) | ((uint32_t)dccenteredholdtime50ns << 24));
}

/**
 * @brief Returns the current value of the DCCENTEREDZEROTIME50NS field in the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read and the DCCENTEREDZEROTIME50NS field's value will be returned.
 *
 * @return The current value of the DCCENTEREDZEROTIME50NS field in the RWNXAGCDCCOMP1 register.
 */
__INLINE uint8_t riu_dccenteredzerotime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the DCCENTEREDZEROTIME50NS field of the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dccenteredzerotime50ns - The value to set the field to.
 */
__INLINE void riu_dccenteredzerotime50ns_setf(uint8_t dccenteredzerotime50ns)
{
    ASSERT_ERR((((uint32_t)dccenteredzerotime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)dccenteredzerotime50ns << 16));
}

/**
 * @brief Returns the current value of the DCAGC20HOLDTIME50NS field in the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read and the DCAGC20HOLDTIME50NS field's value will be returned.
 *
 * @return The current value of the DCAGC20HOLDTIME50NS field in the RWNXAGCDCCOMP1 register.
 */
__INLINE uint8_t riu_dcagc20holdtime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the DCAGC20HOLDTIME50NS field of the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcagc20holdtime50ns - The value to set the field to.
 */
__INLINE void riu_dcagc20holdtime50ns_setf(uint8_t dcagc20holdtime50ns)
{
    ASSERT_ERR((((uint32_t)dcagc20holdtime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)dcagc20holdtime50ns << 8));
}

/**
 * @brief Returns the current value of the DCAGC20ZEROTIME50NS field in the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read and the DCAGC20ZEROTIME50NS field's value will be returned.
 *
 * @return The current value of the DCAGC20ZEROTIME50NS field in the RWNXAGCDCCOMP1 register.
 */
__INLINE uint8_t riu_dcagc20zerotime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the DCAGC20ZEROTIME50NS field of the RWNXAGCDCCOMP1 register.
 *
 * The RWNXAGCDCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcagc20zerotime50ns - The value to set the field to.
 */
__INLINE void riu_dcagc20zerotime50ns_setf(uint8_t dcagc20zerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcagc20zerotime50ns << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP1_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)dcagc20zerotime50ns << 0));
}

/// @}

/**
 * @name RWNXAGCDCCOMP2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  30:24   DCDSSSHOLDTIME50NS   0x0
 *  22:16   DCDSSSZEROTIME50NS   0x2
 *  06:00        DELDCSTABLECC   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDCCOMP2 register
#define RIU_RWNXAGCDCCOMP2_ADDR   (REG_RIU_BASE_ADDR+0xB37C)
/// Offset of the RWNXAGCDCCOMP2 register from the base address
#define RIU_RWNXAGCDCCOMP2_OFFSET 0x0000037C
/// Index of the RWNXAGCDCCOMP2 register
#define RIU_RWNXAGCDCCOMP2_INDEX  0x000000DF
/// Reset value of the RWNXAGCDCCOMP2 register
#define RIU_RWNXAGCDCCOMP2_RESET  0x00020000

/**
 * @brief Returns the current value of the RWNXAGCDCCOMP2 register.
 * The RWNXAGCDCCOMP2 register will be read and its value returned.
 * @return The current value of the RWNXAGCDCCOMP2 register.
 */
__INLINE uint32_t riu_rwnxagcdccomp2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR);
}

/**
 * @brief Sets the RWNXAGCDCCOMP2 register to a value.
 * The RWNXAGCDCCOMP2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdccomp2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDCCOMP2_ADDR, value);
}

// field definitions
/// DCDSSSHOLDTIME50NS field mask
#define RIU_DCDSSSHOLDTIME50NS_MASK   ((uint32_t)0x7F000000)
/// DCDSSSHOLDTIME50NS field LSB position
#define RIU_DCDSSSHOLDTIME50NS_LSB    24
/// DCDSSSHOLDTIME50NS field width
#define RIU_DCDSSSHOLDTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DCDSSSZEROTIME50NS field mask
#define RIU_DCDSSSZEROTIME50NS_MASK   ((uint32_t)0x007F0000)
/// DCDSSSZEROTIME50NS field LSB position
#define RIU_DCDSSSZEROTIME50NS_LSB    16
/// DCDSSSZEROTIME50NS field width
#define RIU_DCDSSSZEROTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DELDCSTABLECC field mask
#define RIU_DELDCSTABLECC_MASK        ((uint32_t)0x0000007F)
/// DELDCSTABLECC field LSB position
#define RIU_DELDCSTABLECC_LSB         0
/// DELDCSTABLECC field width
#define RIU_DELDCSTABLECC_WIDTH       ((uint32_t)0x00000007)

/// DCDSSSHOLDTIME50NS field reset value
#define RIU_DCDSSSHOLDTIME50NS_RST    0x0
/// DCDSSSZEROTIME50NS field reset value
#define RIU_DCDSSSZEROTIME50NS_RST    0x2
/// DELDCSTABLECC field reset value
#define RIU_DELDCSTABLECC_RST         0x0

/**
 * @brief Constructs a value for the RWNXAGCDCCOMP2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] dcdsssholdtime50ns - The value to use for the DCDSSSHOLDTIME50NS field.
 * @param[in] dcdssszerotime50ns - The value to use for the DCDSSSZEROTIME50NS field.
 * @param[in] deldcstablecc - The value to use for the DELDCSTABLECC field.
 */
__INLINE void riu_rwnxagcdccomp2_pack(uint8_t dcdsssholdtime50ns, uint8_t dcdssszerotime50ns, uint8_t deldcstablecc)
{
    ASSERT_ERR((((uint32_t)dcdsssholdtime50ns << 24) & ~((uint32_t)0x7F000000)) == 0);
    ASSERT_ERR((((uint32_t)dcdssszerotime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)deldcstablecc << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP2_ADDR,  ((uint32_t)dcdsssholdtime50ns << 24) | ((uint32_t)dcdssszerotime50ns << 16) | ((uint32_t)deldcstablecc << 0));
}

/**
 * @brief Unpacks RWNXAGCDCCOMP2's fields from current value of the RWNXAGCDCCOMP2 register.
 *
 * Reads the RWNXAGCDCCOMP2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] dcdsssholdtime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dcdssszerotime50ns - Will be populated with the current value of this field from the register.
 * @param[out] deldcstablecc - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdccomp2_unpack(uint8_t* dcdsssholdtime50ns, uint8_t* dcdssszerotime50ns, uint8_t* deldcstablecc)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR);

    *dcdsssholdtime50ns = (localVal & ((uint32_t)0x7F000000)) >> 24;
    *dcdssszerotime50ns = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *deldcstablecc = (localVal & ((uint32_t)0x0000007F)) >> 0;
}

/**
 * @brief Returns the current value of the DCDSSSHOLDTIME50NS field in the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read and the DCDSSSHOLDTIME50NS field's value will be returned.
 *
 * @return The current value of the DCDSSSHOLDTIME50NS field in the RWNXAGCDCCOMP2 register.
 */
__INLINE uint8_t riu_dcdsssholdtime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR);
    return ((localVal & ((uint32_t)0x7F000000)) >> 24);
}

/**
 * @brief Sets the DCDSSSHOLDTIME50NS field of the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcdsssholdtime50ns - The value to set the field to.
 */
__INLINE void riu_dcdsssholdtime50ns_setf(uint8_t dcdsssholdtime50ns)
{
    ASSERT_ERR((((uint32_t)dcdsssholdtime50ns << 24) & ~((uint32_t)0x7F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR) & ~((uint32_t)0x7F000000)) | ((uint32_t)dcdsssholdtime50ns << 24));
}

/**
 * @brief Returns the current value of the DCDSSSZEROTIME50NS field in the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read and the DCDSSSZEROTIME50NS field's value will be returned.
 *
 * @return The current value of the DCDSSSZEROTIME50NS field in the RWNXAGCDCCOMP2 register.
 */
__INLINE uint8_t riu_dcdssszerotime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the DCDSSSZEROTIME50NS field of the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcdssszerotime50ns - The value to set the field to.
 */
__INLINE void riu_dcdssszerotime50ns_setf(uint8_t dcdssszerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcdssszerotime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)dcdssszerotime50ns << 16));
}

/**
 * @brief Returns the current value of the DELDCSTABLECC field in the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read and the DELDCSTABLECC field's value will be returned.
 *
 * @return The current value of the DELDCSTABLECC field in the RWNXAGCDCCOMP2 register.
 */
__INLINE uint8_t riu_deldcstablecc_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR);
    return ((localVal & ((uint32_t)0x0000007F)) >> 0);
}

/**
 * @brief Sets the DELDCSTABLECC field of the RWNXAGCDCCOMP2 register.
 *
 * The RWNXAGCDCCOMP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] deldcstablecc - The value to set the field to.
 */
__INLINE void riu_deldcstablecc_setf(uint8_t deldcstablecc)
{
    ASSERT_ERR((((uint32_t)deldcstablecc << 0) & ~((uint32_t)0x0000007F)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMP2_ADDR) & ~((uint32_t)0x0000007F)) | ((uint32_t)deldcstablecc << 0));
}

/// @}

/**
 * @name RWNXAGCEVT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:26              EVT0OP1   0x14
 *  25:20              EVT0OP2   0x10
 *  19:14              EVT0OP3   0x1
 *     13         EVT0PATHCOMB   0
 *  12:10           EVT0OPCOMB   0x1
 *  08:00           EVT0TGTADD   0x2
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCEVT0 register
#define RIU_RWNXAGCEVT0_ADDR   (REG_RIU_BASE_ADDR+0xB380)
/// Offset of the RWNXAGCEVT0 register from the base address
#define RIU_RWNXAGCEVT0_OFFSET 0x00000380
/// Index of the RWNXAGCEVT0 register
#define RIU_RWNXAGCEVT0_INDEX  0x000000E0
/// Reset value of the RWNXAGCEVT0 register
#define RIU_RWNXAGCEVT0_RESET  0x51004402

/**
 * @brief Returns the current value of the RWNXAGCEVT0 register.
 * The RWNXAGCEVT0 register will be read and its value returned.
 * @return The current value of the RWNXAGCEVT0 register.
 */
__INLINE uint32_t riu_rwnxagcevt0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
}

/**
 * @brief Sets the RWNXAGCEVT0 register to a value.
 * The RWNXAGCEVT0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcevt0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, value);
}

// field definitions
/// EVT0OP1 field mask
#define RIU_EVT0OP1_MASK        ((uint32_t)0xFC000000)
/// EVT0OP1 field LSB position
#define RIU_EVT0OP1_LSB         26
/// EVT0OP1 field width
#define RIU_EVT0OP1_WIDTH       ((uint32_t)0x00000006)
/// EVT0OP2 field mask
#define RIU_EVT0OP2_MASK        ((uint32_t)0x03F00000)
/// EVT0OP2 field LSB position
#define RIU_EVT0OP2_LSB         20
/// EVT0OP2 field width
#define RIU_EVT0OP2_WIDTH       ((uint32_t)0x00000006)
/// EVT0OP3 field mask
#define RIU_EVT0OP3_MASK        ((uint32_t)0x000FC000)
/// EVT0OP3 field LSB position
#define RIU_EVT0OP3_LSB         14
/// EVT0OP3 field width
#define RIU_EVT0OP3_WIDTH       ((uint32_t)0x00000006)
/// EVT0PATHCOMB field bit
#define RIU_EVT0PATHCOMB_BIT    ((uint32_t)0x00002000)
/// EVT0PATHCOMB field position
#define RIU_EVT0PATHCOMB_POS    13
/// EVT0OPCOMB field mask
#define RIU_EVT0OPCOMB_MASK     ((uint32_t)0x00001C00)
/// EVT0OPCOMB field LSB position
#define RIU_EVT0OPCOMB_LSB      10
/// EVT0OPCOMB field width
#define RIU_EVT0OPCOMB_WIDTH    ((uint32_t)0x00000003)
/// EVT0TGTADD field mask
#define RIU_EVT0TGTADD_MASK     ((uint32_t)0x000001FF)
/// EVT0TGTADD field LSB position
#define RIU_EVT0TGTADD_LSB      0
/// EVT0TGTADD field width
#define RIU_EVT0TGTADD_WIDTH    ((uint32_t)0x00000009)

/// EVT0OP1 field reset value
#define RIU_EVT0OP1_RST         0x14
/// EVT0OP2 field reset value
#define RIU_EVT0OP2_RST         0x10
/// EVT0OP3 field reset value
#define RIU_EVT0OP3_RST         0x1
/// EVT0PATHCOMB field reset value
#define RIU_EVT0PATHCOMB_RST    0x0
/// EVT0OPCOMB field reset value
#define RIU_EVT0OPCOMB_RST      0x1
/// EVT0TGTADD field reset value
#define RIU_EVT0TGTADD_RST      0x2

/**
 * @brief Constructs a value for the RWNXAGCEVT0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] evt0op1 - The value to use for the EVT0OP1 field.
 * @param[in] evt0op2 - The value to use for the EVT0OP2 field.
 * @param[in] evt0op3 - The value to use for the EVT0OP3 field.
 * @param[in] evt0pathcomb - The value to use for the EVT0PATHCOMB field.
 * @param[in] evt0opcomb - The value to use for the EVT0OPCOMB field.
 * @param[in] evt0tgtadd - The value to use for the EVT0TGTADD field.
 */
__INLINE void riu_rwnxagcevt0_pack(uint8_t evt0op1, uint8_t evt0op2, uint8_t evt0op3, uint8_t evt0pathcomb, uint8_t evt0opcomb, uint16_t evt0tgtadd)
{
    ASSERT_ERR((((uint32_t)evt0op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    ASSERT_ERR((((uint32_t)evt0op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    ASSERT_ERR((((uint32_t)evt0op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    ASSERT_ERR((((uint32_t)evt0pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)evt0opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    ASSERT_ERR((((uint32_t)evt0tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR,  ((uint32_t)evt0op1 << 26) | ((uint32_t)evt0op2 << 20) | ((uint32_t)evt0op3 << 14) | ((uint32_t)evt0pathcomb << 13) | ((uint32_t)evt0opcomb << 10) | ((uint32_t)evt0tgtadd << 0));
}

/**
 * @brief Unpacks RWNXAGCEVT0's fields from current value of the RWNXAGCEVT0 register.
 *
 * Reads the RWNXAGCEVT0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] evt0op1 - Will be populated with the current value of this field from the register.
 * @param[out] evt0op2 - Will be populated with the current value of this field from the register.
 * @param[out] evt0op3 - Will be populated with the current value of this field from the register.
 * @param[out] evt0pathcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt0opcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt0tgtadd - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcevt0_unpack(uint8_t* evt0op1, uint8_t* evt0op2, uint8_t* evt0op3, uint8_t* evt0pathcomb, uint8_t* evt0opcomb, uint16_t* evt0tgtadd)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);

    *evt0op1 = (localVal & ((uint32_t)0xFC000000)) >> 26;
    *evt0op2 = (localVal & ((uint32_t)0x03F00000)) >> 20;
    *evt0op3 = (localVal & ((uint32_t)0x000FC000)) >> 14;
    *evt0pathcomb = (localVal & ((uint32_t)0x00002000)) >> 13;
    *evt0opcomb = (localVal & ((uint32_t)0x00001C00)) >> 10;
    *evt0tgtadd = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the EVT0OP1 field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0OP1 field's value will be returned.
 *
 * @return The current value of the EVT0OP1 field in the RWNXAGCEVT0 register.
 */
__INLINE uint8_t riu_evt0op1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0xFC000000)) >> 26);
}

/**
 * @brief Sets the EVT0OP1 field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0op1 - The value to set the field to.
 */
__INLINE void riu_evt0op1_setf(uint8_t evt0op1)
{
    ASSERT_ERR((((uint32_t)evt0op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0xFC000000)) | ((uint32_t)evt0op1 << 26));
}

/**
 * @brief Returns the current value of the EVT0OP2 field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0OP2 field's value will be returned.
 *
 * @return The current value of the EVT0OP2 field in the RWNXAGCEVT0 register.
 */
__INLINE uint8_t riu_evt0op2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0x03F00000)) >> 20);
}

/**
 * @brief Sets the EVT0OP2 field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0op2 - The value to set the field to.
 */
__INLINE void riu_evt0op2_setf(uint8_t evt0op2)
{
    ASSERT_ERR((((uint32_t)evt0op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0x03F00000)) | ((uint32_t)evt0op2 << 20));
}

/**
 * @brief Returns the current value of the EVT0OP3 field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0OP3 field's value will be returned.
 *
 * @return The current value of the EVT0OP3 field in the RWNXAGCEVT0 register.
 */
__INLINE uint8_t riu_evt0op3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0x000FC000)) >> 14);
}

/**
 * @brief Sets the EVT0OP3 field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0op3 - The value to set the field to.
 */
__INLINE void riu_evt0op3_setf(uint8_t evt0op3)
{
    ASSERT_ERR((((uint32_t)evt0op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0x000FC000)) | ((uint32_t)evt0op3 << 14));
}

/**
 * @brief Returns the current value of the EVT0PATHCOMB field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0PATHCOMB field's value will be returned.
 *
 * @return The current value of the EVT0PATHCOMB field in the RWNXAGCEVT0 register.
 */
__INLINE uint8_t riu_evt0pathcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

/**
 * @brief Sets the EVT0PATHCOMB field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0pathcomb - The value to set the field to.
 */
__INLINE void riu_evt0pathcomb_setf(uint8_t evt0pathcomb)
{
    ASSERT_ERR((((uint32_t)evt0pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)evt0pathcomb << 13));
}

/**
 * @brief Returns the current value of the EVT0OPCOMB field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0OPCOMB field's value will be returned.
 *
 * @return The current value of the EVT0OPCOMB field in the RWNXAGCEVT0 register.
 */
__INLINE uint8_t riu_evt0opcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0x00001C00)) >> 10);
}

/**
 * @brief Sets the EVT0OPCOMB field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0opcomb - The value to set the field to.
 */
__INLINE void riu_evt0opcomb_setf(uint8_t evt0opcomb)
{
    ASSERT_ERR((((uint32_t)evt0opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0x00001C00)) | ((uint32_t)evt0opcomb << 10));
}

/**
 * @brief Returns the current value of the EVT0TGTADD field in the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read and the EVT0TGTADD field's value will be returned.
 *
 * @return The current value of the EVT0TGTADD field in the RWNXAGCEVT0 register.
 */
__INLINE uint16_t riu_evt0tgtadd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT0_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the EVT0TGTADD field of the RWNXAGCEVT0 register.
 *
 * The RWNXAGCEVT0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt0tgtadd - The value to set the field to.
 */
__INLINE void riu_evt0tgtadd_setf(uint16_t evt0tgtadd)
{
    ASSERT_ERR((((uint32_t)evt0tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT0_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT0_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)evt0tgtadd << 0));
}

/// @}

/**
 * @name RWNXAGCEVT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:26              EVT1OP1   0x15
 *  25:20              EVT1OP2   0x17
 *  19:14              EVT1OP3   0xF
 *     13         EVT1PATHCOMB   0
 *  12:10           EVT1OPCOMB   0x2
 *  08:00           EVT1TGTADD   0x5
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCEVT1 register
#define RIU_RWNXAGCEVT1_ADDR   (REG_RIU_BASE_ADDR+0xB384)
/// Offset of the RWNXAGCEVT1 register from the base address
#define RIU_RWNXAGCEVT1_OFFSET 0x00000384
/// Index of the RWNXAGCEVT1 register
#define RIU_RWNXAGCEVT1_INDEX  0x000000E1
/// Reset value of the RWNXAGCEVT1 register
#define RIU_RWNXAGCEVT1_RESET  0x5573C805

/**
 * @brief Returns the current value of the RWNXAGCEVT1 register.
 * The RWNXAGCEVT1 register will be read and its value returned.
 * @return The current value of the RWNXAGCEVT1 register.
 */
__INLINE uint32_t riu_rwnxagcevt1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
}

/**
 * @brief Sets the RWNXAGCEVT1 register to a value.
 * The RWNXAGCEVT1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcevt1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, value);
}

// field definitions
/// EVT1OP1 field mask
#define RIU_EVT1OP1_MASK        ((uint32_t)0xFC000000)
/// EVT1OP1 field LSB position
#define RIU_EVT1OP1_LSB         26
/// EVT1OP1 field width
#define RIU_EVT1OP1_WIDTH       ((uint32_t)0x00000006)
/// EVT1OP2 field mask
#define RIU_EVT1OP2_MASK        ((uint32_t)0x03F00000)
/// EVT1OP2 field LSB position
#define RIU_EVT1OP2_LSB         20
/// EVT1OP2 field width
#define RIU_EVT1OP2_WIDTH       ((uint32_t)0x00000006)
/// EVT1OP3 field mask
#define RIU_EVT1OP3_MASK        ((uint32_t)0x000FC000)
/// EVT1OP3 field LSB position
#define RIU_EVT1OP3_LSB         14
/// EVT1OP3 field width
#define RIU_EVT1OP3_WIDTH       ((uint32_t)0x00000006)
/// EVT1PATHCOMB field bit
#define RIU_EVT1PATHCOMB_BIT    ((uint32_t)0x00002000)
/// EVT1PATHCOMB field position
#define RIU_EVT1PATHCOMB_POS    13
/// EVT1OPCOMB field mask
#define RIU_EVT1OPCOMB_MASK     ((uint32_t)0x00001C00)
/// EVT1OPCOMB field LSB position
#define RIU_EVT1OPCOMB_LSB      10
/// EVT1OPCOMB field width
#define RIU_EVT1OPCOMB_WIDTH    ((uint32_t)0x00000003)
/// EVT1TGTADD field mask
#define RIU_EVT1TGTADD_MASK     ((uint32_t)0x000001FF)
/// EVT1TGTADD field LSB position
#define RIU_EVT1TGTADD_LSB      0
/// EVT1TGTADD field width
#define RIU_EVT1TGTADD_WIDTH    ((uint32_t)0x00000009)

/// EVT1OP1 field reset value
#define RIU_EVT1OP1_RST         0x15
/// EVT1OP2 field reset value
#define RIU_EVT1OP2_RST         0x17
/// EVT1OP3 field reset value
#define RIU_EVT1OP3_RST         0xF
/// EVT1PATHCOMB field reset value
#define RIU_EVT1PATHCOMB_RST    0x0
/// EVT1OPCOMB field reset value
#define RIU_EVT1OPCOMB_RST      0x2
/// EVT1TGTADD field reset value
#define RIU_EVT1TGTADD_RST      0x5

/**
 * @brief Constructs a value for the RWNXAGCEVT1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] evt1op1 - The value to use for the EVT1OP1 field.
 * @param[in] evt1op2 - The value to use for the EVT1OP2 field.
 * @param[in] evt1op3 - The value to use for the EVT1OP3 field.
 * @param[in] evt1pathcomb - The value to use for the EVT1PATHCOMB field.
 * @param[in] evt1opcomb - The value to use for the EVT1OPCOMB field.
 * @param[in] evt1tgtadd - The value to use for the EVT1TGTADD field.
 */
__INLINE void riu_rwnxagcevt1_pack(uint8_t evt1op1, uint8_t evt1op2, uint8_t evt1op3, uint8_t evt1pathcomb, uint8_t evt1opcomb, uint16_t evt1tgtadd)
{
    ASSERT_ERR((((uint32_t)evt1op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    ASSERT_ERR((((uint32_t)evt1op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    ASSERT_ERR((((uint32_t)evt1op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    ASSERT_ERR((((uint32_t)evt1pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)evt1opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    ASSERT_ERR((((uint32_t)evt1tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR,  ((uint32_t)evt1op1 << 26) | ((uint32_t)evt1op2 << 20) | ((uint32_t)evt1op3 << 14) | ((uint32_t)evt1pathcomb << 13) | ((uint32_t)evt1opcomb << 10) | ((uint32_t)evt1tgtadd << 0));
}

/**
 * @brief Unpacks RWNXAGCEVT1's fields from current value of the RWNXAGCEVT1 register.
 *
 * Reads the RWNXAGCEVT1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] evt1op1 - Will be populated with the current value of this field from the register.
 * @param[out] evt1op2 - Will be populated with the current value of this field from the register.
 * @param[out] evt1op3 - Will be populated with the current value of this field from the register.
 * @param[out] evt1pathcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt1opcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt1tgtadd - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcevt1_unpack(uint8_t* evt1op1, uint8_t* evt1op2, uint8_t* evt1op3, uint8_t* evt1pathcomb, uint8_t* evt1opcomb, uint16_t* evt1tgtadd)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);

    *evt1op1 = (localVal & ((uint32_t)0xFC000000)) >> 26;
    *evt1op2 = (localVal & ((uint32_t)0x03F00000)) >> 20;
    *evt1op3 = (localVal & ((uint32_t)0x000FC000)) >> 14;
    *evt1pathcomb = (localVal & ((uint32_t)0x00002000)) >> 13;
    *evt1opcomb = (localVal & ((uint32_t)0x00001C00)) >> 10;
    *evt1tgtadd = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the EVT1OP1 field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1OP1 field's value will be returned.
 *
 * @return The current value of the EVT1OP1 field in the RWNXAGCEVT1 register.
 */
__INLINE uint8_t riu_evt1op1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0xFC000000)) >> 26);
}

/**
 * @brief Sets the EVT1OP1 field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1op1 - The value to set the field to.
 */
__INLINE void riu_evt1op1_setf(uint8_t evt1op1)
{
    ASSERT_ERR((((uint32_t)evt1op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0xFC000000)) | ((uint32_t)evt1op1 << 26));
}

/**
 * @brief Returns the current value of the EVT1OP2 field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1OP2 field's value will be returned.
 *
 * @return The current value of the EVT1OP2 field in the RWNXAGCEVT1 register.
 */
__INLINE uint8_t riu_evt1op2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0x03F00000)) >> 20);
}

/**
 * @brief Sets the EVT1OP2 field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1op2 - The value to set the field to.
 */
__INLINE void riu_evt1op2_setf(uint8_t evt1op2)
{
    ASSERT_ERR((((uint32_t)evt1op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0x03F00000)) | ((uint32_t)evt1op2 << 20));
}

/**
 * @brief Returns the current value of the EVT1OP3 field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1OP3 field's value will be returned.
 *
 * @return The current value of the EVT1OP3 field in the RWNXAGCEVT1 register.
 */
__INLINE uint8_t riu_evt1op3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0x000FC000)) >> 14);
}

/**
 * @brief Sets the EVT1OP3 field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1op3 - The value to set the field to.
 */
__INLINE void riu_evt1op3_setf(uint8_t evt1op3)
{
    ASSERT_ERR((((uint32_t)evt1op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0x000FC000)) | ((uint32_t)evt1op3 << 14));
}

/**
 * @brief Returns the current value of the EVT1PATHCOMB field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1PATHCOMB field's value will be returned.
 *
 * @return The current value of the EVT1PATHCOMB field in the RWNXAGCEVT1 register.
 */
__INLINE uint8_t riu_evt1pathcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

/**
 * @brief Sets the EVT1PATHCOMB field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1pathcomb - The value to set the field to.
 */
__INLINE void riu_evt1pathcomb_setf(uint8_t evt1pathcomb)
{
    ASSERT_ERR((((uint32_t)evt1pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)evt1pathcomb << 13));
}

/**
 * @brief Returns the current value of the EVT1OPCOMB field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1OPCOMB field's value will be returned.
 *
 * @return The current value of the EVT1OPCOMB field in the RWNXAGCEVT1 register.
 */
__INLINE uint8_t riu_evt1opcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0x00001C00)) >> 10);
}

/**
 * @brief Sets the EVT1OPCOMB field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1opcomb - The value to set the field to.
 */
__INLINE void riu_evt1opcomb_setf(uint8_t evt1opcomb)
{
    ASSERT_ERR((((uint32_t)evt1opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0x00001C00)) | ((uint32_t)evt1opcomb << 10));
}

/**
 * @brief Returns the current value of the EVT1TGTADD field in the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read and the EVT1TGTADD field's value will be returned.
 *
 * @return The current value of the EVT1TGTADD field in the RWNXAGCEVT1 register.
 */
__INLINE uint16_t riu_evt1tgtadd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT1_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the EVT1TGTADD field of the RWNXAGCEVT1 register.
 *
 * The RWNXAGCEVT1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt1tgtadd - The value to set the field to.
 */
__INLINE void riu_evt1tgtadd_setf(uint16_t evt1tgtadd)
{
    ASSERT_ERR((((uint32_t)evt1tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT1_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT1_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)evt1tgtadd << 0));
}

/// @}

/**
 * @name RWNXAGCEVT2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:26              EVT2OP1   0x19
 *  25:20              EVT2OP2   0x0
 *  19:14              EVT2OP3   0xE
 *     13         EVT2PATHCOMB   1
 *  12:10           EVT2OPCOMB   0x2
 *  08:00           EVT2TGTADD   0x8
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCEVT2 register
#define RIU_RWNXAGCEVT2_ADDR   (REG_RIU_BASE_ADDR+0xB388)
/// Offset of the RWNXAGCEVT2 register from the base address
#define RIU_RWNXAGCEVT2_OFFSET 0x00000388
/// Index of the RWNXAGCEVT2 register
#define RIU_RWNXAGCEVT2_INDEX  0x000000E2
/// Reset value of the RWNXAGCEVT2 register
#define RIU_RWNXAGCEVT2_RESET  0x6403A808

/**
 * @brief Returns the current value of the RWNXAGCEVT2 register.
 * The RWNXAGCEVT2 register will be read and its value returned.
 * @return The current value of the RWNXAGCEVT2 register.
 */
__INLINE uint32_t riu_rwnxagcevt2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
}

/**
 * @brief Sets the RWNXAGCEVT2 register to a value.
 * The RWNXAGCEVT2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcevt2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, value);
}

// field definitions
/// EVT2OP1 field mask
#define RIU_EVT2OP1_MASK        ((uint32_t)0xFC000000)
/// EVT2OP1 field LSB position
#define RIU_EVT2OP1_LSB         26
/// EVT2OP1 field width
#define RIU_EVT2OP1_WIDTH       ((uint32_t)0x00000006)
/// EVT2OP2 field mask
#define RIU_EVT2OP2_MASK        ((uint32_t)0x03F00000)
/// EVT2OP2 field LSB position
#define RIU_EVT2OP2_LSB         20
/// EVT2OP2 field width
#define RIU_EVT2OP2_WIDTH       ((uint32_t)0x00000006)
/// EVT2OP3 field mask
#define RIU_EVT2OP3_MASK        ((uint32_t)0x000FC000)
/// EVT2OP3 field LSB position
#define RIU_EVT2OP3_LSB         14
/// EVT2OP3 field width
#define RIU_EVT2OP3_WIDTH       ((uint32_t)0x00000006)
/// EVT2PATHCOMB field bit
#define RIU_EVT2PATHCOMB_BIT    ((uint32_t)0x00002000)
/// EVT2PATHCOMB field position
#define RIU_EVT2PATHCOMB_POS    13
/// EVT2OPCOMB field mask
#define RIU_EVT2OPCOMB_MASK     ((uint32_t)0x00001C00)
/// EVT2OPCOMB field LSB position
#define RIU_EVT2OPCOMB_LSB      10
/// EVT2OPCOMB field width
#define RIU_EVT2OPCOMB_WIDTH    ((uint32_t)0x00000003)
/// EVT2TGTADD field mask
#define RIU_EVT2TGTADD_MASK     ((uint32_t)0x000001FF)
/// EVT2TGTADD field LSB position
#define RIU_EVT2TGTADD_LSB      0
/// EVT2TGTADD field width
#define RIU_EVT2TGTADD_WIDTH    ((uint32_t)0x00000009)

/// EVT2OP1 field reset value
#define RIU_EVT2OP1_RST         0x19
/// EVT2OP2 field reset value
#define RIU_EVT2OP2_RST         0x0
/// EVT2OP3 field reset value
#define RIU_EVT2OP3_RST         0xE
/// EVT2PATHCOMB field reset value
#define RIU_EVT2PATHCOMB_RST    0x1
/// EVT2OPCOMB field reset value
#define RIU_EVT2OPCOMB_RST      0x2
/// EVT2TGTADD field reset value
#define RIU_EVT2TGTADD_RST      0x8

/**
 * @brief Constructs a value for the RWNXAGCEVT2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] evt2op1 - The value to use for the EVT2OP1 field.
 * @param[in] evt2op2 - The value to use for the EVT2OP2 field.
 * @param[in] evt2op3 - The value to use for the EVT2OP3 field.
 * @param[in] evt2pathcomb - The value to use for the EVT2PATHCOMB field.
 * @param[in] evt2opcomb - The value to use for the EVT2OPCOMB field.
 * @param[in] evt2tgtadd - The value to use for the EVT2TGTADD field.
 */
__INLINE void riu_rwnxagcevt2_pack(uint8_t evt2op1, uint8_t evt2op2, uint8_t evt2op3, uint8_t evt2pathcomb, uint8_t evt2opcomb, uint16_t evt2tgtadd)
{
    ASSERT_ERR((((uint32_t)evt2op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    ASSERT_ERR((((uint32_t)evt2op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    ASSERT_ERR((((uint32_t)evt2op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    ASSERT_ERR((((uint32_t)evt2pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)evt2opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    ASSERT_ERR((((uint32_t)evt2tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR,  ((uint32_t)evt2op1 << 26) | ((uint32_t)evt2op2 << 20) | ((uint32_t)evt2op3 << 14) | ((uint32_t)evt2pathcomb << 13) | ((uint32_t)evt2opcomb << 10) | ((uint32_t)evt2tgtadd << 0));
}

/**
 * @brief Unpacks RWNXAGCEVT2's fields from current value of the RWNXAGCEVT2 register.
 *
 * Reads the RWNXAGCEVT2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] evt2op1 - Will be populated with the current value of this field from the register.
 * @param[out] evt2op2 - Will be populated with the current value of this field from the register.
 * @param[out] evt2op3 - Will be populated with the current value of this field from the register.
 * @param[out] evt2pathcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt2opcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt2tgtadd - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcevt2_unpack(uint8_t* evt2op1, uint8_t* evt2op2, uint8_t* evt2op3, uint8_t* evt2pathcomb, uint8_t* evt2opcomb, uint16_t* evt2tgtadd)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);

    *evt2op1 = (localVal & ((uint32_t)0xFC000000)) >> 26;
    *evt2op2 = (localVal & ((uint32_t)0x03F00000)) >> 20;
    *evt2op3 = (localVal & ((uint32_t)0x000FC000)) >> 14;
    *evt2pathcomb = (localVal & ((uint32_t)0x00002000)) >> 13;
    *evt2opcomb = (localVal & ((uint32_t)0x00001C00)) >> 10;
    *evt2tgtadd = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the EVT2OP1 field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2OP1 field's value will be returned.
 *
 * @return The current value of the EVT2OP1 field in the RWNXAGCEVT2 register.
 */
__INLINE uint8_t riu_evt2op1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0xFC000000)) >> 26);
}

/**
 * @brief Sets the EVT2OP1 field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2op1 - The value to set the field to.
 */
__INLINE void riu_evt2op1_setf(uint8_t evt2op1)
{
    ASSERT_ERR((((uint32_t)evt2op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0xFC000000)) | ((uint32_t)evt2op1 << 26));
}

/**
 * @brief Returns the current value of the EVT2OP2 field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2OP2 field's value will be returned.
 *
 * @return The current value of the EVT2OP2 field in the RWNXAGCEVT2 register.
 */
__INLINE uint8_t riu_evt2op2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0x03F00000)) >> 20);
}

/**
 * @brief Sets the EVT2OP2 field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2op2 - The value to set the field to.
 */
__INLINE void riu_evt2op2_setf(uint8_t evt2op2)
{
    ASSERT_ERR((((uint32_t)evt2op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0x03F00000)) | ((uint32_t)evt2op2 << 20));
}

/**
 * @brief Returns the current value of the EVT2OP3 field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2OP3 field's value will be returned.
 *
 * @return The current value of the EVT2OP3 field in the RWNXAGCEVT2 register.
 */
__INLINE uint8_t riu_evt2op3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0x000FC000)) >> 14);
}

/**
 * @brief Sets the EVT2OP3 field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2op3 - The value to set the field to.
 */
__INLINE void riu_evt2op3_setf(uint8_t evt2op3)
{
    ASSERT_ERR((((uint32_t)evt2op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0x000FC000)) | ((uint32_t)evt2op3 << 14));
}

/**
 * @brief Returns the current value of the EVT2PATHCOMB field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2PATHCOMB field's value will be returned.
 *
 * @return The current value of the EVT2PATHCOMB field in the RWNXAGCEVT2 register.
 */
__INLINE uint8_t riu_evt2pathcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

/**
 * @brief Sets the EVT2PATHCOMB field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2pathcomb - The value to set the field to.
 */
__INLINE void riu_evt2pathcomb_setf(uint8_t evt2pathcomb)
{
    ASSERT_ERR((((uint32_t)evt2pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)evt2pathcomb << 13));
}

/**
 * @brief Returns the current value of the EVT2OPCOMB field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2OPCOMB field's value will be returned.
 *
 * @return The current value of the EVT2OPCOMB field in the RWNXAGCEVT2 register.
 */
__INLINE uint8_t riu_evt2opcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0x00001C00)) >> 10);
}

/**
 * @brief Sets the EVT2OPCOMB field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2opcomb - The value to set the field to.
 */
__INLINE void riu_evt2opcomb_setf(uint8_t evt2opcomb)
{
    ASSERT_ERR((((uint32_t)evt2opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0x00001C00)) | ((uint32_t)evt2opcomb << 10));
}

/**
 * @brief Returns the current value of the EVT2TGTADD field in the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read and the EVT2TGTADD field's value will be returned.
 *
 * @return The current value of the EVT2TGTADD field in the RWNXAGCEVT2 register.
 */
__INLINE uint16_t riu_evt2tgtadd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT2_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the EVT2TGTADD field of the RWNXAGCEVT2 register.
 *
 * The RWNXAGCEVT2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt2tgtadd - The value to set the field to.
 */
__INLINE void riu_evt2tgtadd_setf(uint16_t evt2tgtadd)
{
    ASSERT_ERR((((uint32_t)evt2tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT2_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT2_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)evt2tgtadd << 0));
}

/// @}

/**
 * @name RWNXAGCEVT3 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:26              EVT3OP1   0x1
 *  25:20              EVT3OP2   0xF
 *  19:14              EVT3OP3   0x3F
 *     13         EVT3PATHCOMB   0
 *  12:10           EVT3OPCOMB   0x2
 *  08:00           EVT3TGTADD   0xB
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCEVT3 register
#define RIU_RWNXAGCEVT3_ADDR   (REG_RIU_BASE_ADDR+0xB38C)
/// Offset of the RWNXAGCEVT3 register from the base address
#define RIU_RWNXAGCEVT3_OFFSET 0x0000038C
/// Index of the RWNXAGCEVT3 register
#define RIU_RWNXAGCEVT3_INDEX  0x000000E3
/// Reset value of the RWNXAGCEVT3 register
#define RIU_RWNXAGCEVT3_RESET  0x04FFC80B

/**
 * @brief Returns the current value of the RWNXAGCEVT3 register.
 * The RWNXAGCEVT3 register will be read and its value returned.
 * @return The current value of the RWNXAGCEVT3 register.
 */
__INLINE uint32_t riu_rwnxagcevt3_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
}

/**
 * @brief Sets the RWNXAGCEVT3 register to a value.
 * The RWNXAGCEVT3 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcevt3_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, value);
}

// field definitions
/// EVT3OP1 field mask
#define RIU_EVT3OP1_MASK        ((uint32_t)0xFC000000)
/// EVT3OP1 field LSB position
#define RIU_EVT3OP1_LSB         26
/// EVT3OP1 field width
#define RIU_EVT3OP1_WIDTH       ((uint32_t)0x00000006)
/// EVT3OP2 field mask
#define RIU_EVT3OP2_MASK        ((uint32_t)0x03F00000)
/// EVT3OP2 field LSB position
#define RIU_EVT3OP2_LSB         20
/// EVT3OP2 field width
#define RIU_EVT3OP2_WIDTH       ((uint32_t)0x00000006)
/// EVT3OP3 field mask
#define RIU_EVT3OP3_MASK        ((uint32_t)0x000FC000)
/// EVT3OP3 field LSB position
#define RIU_EVT3OP3_LSB         14
/// EVT3OP3 field width
#define RIU_EVT3OP3_WIDTH       ((uint32_t)0x00000006)
/// EVT3PATHCOMB field bit
#define RIU_EVT3PATHCOMB_BIT    ((uint32_t)0x00002000)
/// EVT3PATHCOMB field position
#define RIU_EVT3PATHCOMB_POS    13
/// EVT3OPCOMB field mask
#define RIU_EVT3OPCOMB_MASK     ((uint32_t)0x00001C00)
/// EVT3OPCOMB field LSB position
#define RIU_EVT3OPCOMB_LSB      10
/// EVT3OPCOMB field width
#define RIU_EVT3OPCOMB_WIDTH    ((uint32_t)0x00000003)
/// EVT3TGTADD field mask
#define RIU_EVT3TGTADD_MASK     ((uint32_t)0x000001FF)
/// EVT3TGTADD field LSB position
#define RIU_EVT3TGTADD_LSB      0
/// EVT3TGTADD field width
#define RIU_EVT3TGTADD_WIDTH    ((uint32_t)0x00000009)

/// EVT3OP1 field reset value
#define RIU_EVT3OP1_RST         0x1
/// EVT3OP2 field reset value
#define RIU_EVT3OP2_RST         0xF
/// EVT3OP3 field reset value
#define RIU_EVT3OP3_RST         0x3F
/// EVT3PATHCOMB field reset value
#define RIU_EVT3PATHCOMB_RST    0x0
/// EVT3OPCOMB field reset value
#define RIU_EVT3OPCOMB_RST      0x2
/// EVT3TGTADD field reset value
#define RIU_EVT3TGTADD_RST      0xB

/**
 * @brief Constructs a value for the RWNXAGCEVT3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] evt3op1 - The value to use for the EVT3OP1 field.
 * @param[in] evt3op2 - The value to use for the EVT3OP2 field.
 * @param[in] evt3op3 - The value to use for the EVT3OP3 field.
 * @param[in] evt3pathcomb - The value to use for the EVT3PATHCOMB field.
 * @param[in] evt3opcomb - The value to use for the EVT3OPCOMB field.
 * @param[in] evt3tgtadd - The value to use for the EVT3TGTADD field.
 */
__INLINE void riu_rwnxagcevt3_pack(uint8_t evt3op1, uint8_t evt3op2, uint8_t evt3op3, uint8_t evt3pathcomb, uint8_t evt3opcomb, uint16_t evt3tgtadd)
{
    ASSERT_ERR((((uint32_t)evt3op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    ASSERT_ERR((((uint32_t)evt3op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    ASSERT_ERR((((uint32_t)evt3op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    ASSERT_ERR((((uint32_t)evt3pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    ASSERT_ERR((((uint32_t)evt3opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    ASSERT_ERR((((uint32_t)evt3tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR,  ((uint32_t)evt3op1 << 26) | ((uint32_t)evt3op2 << 20) | ((uint32_t)evt3op3 << 14) | ((uint32_t)evt3pathcomb << 13) | ((uint32_t)evt3opcomb << 10) | ((uint32_t)evt3tgtadd << 0));
}

/**
 * @brief Unpacks RWNXAGCEVT3's fields from current value of the RWNXAGCEVT3 register.
 *
 * Reads the RWNXAGCEVT3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] evt3op1 - Will be populated with the current value of this field from the register.
 * @param[out] evt3op2 - Will be populated with the current value of this field from the register.
 * @param[out] evt3op3 - Will be populated with the current value of this field from the register.
 * @param[out] evt3pathcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt3opcomb - Will be populated with the current value of this field from the register.
 * @param[out] evt3tgtadd - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcevt3_unpack(uint8_t* evt3op1, uint8_t* evt3op2, uint8_t* evt3op3, uint8_t* evt3pathcomb, uint8_t* evt3opcomb, uint16_t* evt3tgtadd)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);

    *evt3op1 = (localVal & ((uint32_t)0xFC000000)) >> 26;
    *evt3op2 = (localVal & ((uint32_t)0x03F00000)) >> 20;
    *evt3op3 = (localVal & ((uint32_t)0x000FC000)) >> 14;
    *evt3pathcomb = (localVal & ((uint32_t)0x00002000)) >> 13;
    *evt3opcomb = (localVal & ((uint32_t)0x00001C00)) >> 10;
    *evt3tgtadd = (localVal & ((uint32_t)0x000001FF)) >> 0;
}

/**
 * @brief Returns the current value of the EVT3OP1 field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3OP1 field's value will be returned.
 *
 * @return The current value of the EVT3OP1 field in the RWNXAGCEVT3 register.
 */
__INLINE uint8_t riu_evt3op1_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0xFC000000)) >> 26);
}

/**
 * @brief Sets the EVT3OP1 field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3op1 - The value to set the field to.
 */
__INLINE void riu_evt3op1_setf(uint8_t evt3op1)
{
    ASSERT_ERR((((uint32_t)evt3op1 << 26) & ~((uint32_t)0xFC000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0xFC000000)) | ((uint32_t)evt3op1 << 26));
}

/**
 * @brief Returns the current value of the EVT3OP2 field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3OP2 field's value will be returned.
 *
 * @return The current value of the EVT3OP2 field in the RWNXAGCEVT3 register.
 */
__INLINE uint8_t riu_evt3op2_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0x03F00000)) >> 20);
}

/**
 * @brief Sets the EVT3OP2 field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3op2 - The value to set the field to.
 */
__INLINE void riu_evt3op2_setf(uint8_t evt3op2)
{
    ASSERT_ERR((((uint32_t)evt3op2 << 20) & ~((uint32_t)0x03F00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0x03F00000)) | ((uint32_t)evt3op2 << 20));
}

/**
 * @brief Returns the current value of the EVT3OP3 field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3OP3 field's value will be returned.
 *
 * @return The current value of the EVT3OP3 field in the RWNXAGCEVT3 register.
 */
__INLINE uint8_t riu_evt3op3_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0x000FC000)) >> 14);
}

/**
 * @brief Sets the EVT3OP3 field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3op3 - The value to set the field to.
 */
__INLINE void riu_evt3op3_setf(uint8_t evt3op3)
{
    ASSERT_ERR((((uint32_t)evt3op3 << 14) & ~((uint32_t)0x000FC000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0x000FC000)) | ((uint32_t)evt3op3 << 14));
}

/**
 * @brief Returns the current value of the EVT3PATHCOMB field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3PATHCOMB field's value will be returned.
 *
 * @return The current value of the EVT3PATHCOMB field in the RWNXAGCEVT3 register.
 */
__INLINE uint8_t riu_evt3pathcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0x00002000)) >> 13);
}

/**
 * @brief Sets the EVT3PATHCOMB field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3pathcomb - The value to set the field to.
 */
__INLINE void riu_evt3pathcomb_setf(uint8_t evt3pathcomb)
{
    ASSERT_ERR((((uint32_t)evt3pathcomb << 13) & ~((uint32_t)0x00002000)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0x00002000)) | ((uint32_t)evt3pathcomb << 13));
}

/**
 * @brief Returns the current value of the EVT3OPCOMB field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3OPCOMB field's value will be returned.
 *
 * @return The current value of the EVT3OPCOMB field in the RWNXAGCEVT3 register.
 */
__INLINE uint8_t riu_evt3opcomb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0x00001C00)) >> 10);
}

/**
 * @brief Sets the EVT3OPCOMB field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3opcomb - The value to set the field to.
 */
__INLINE void riu_evt3opcomb_setf(uint8_t evt3opcomb)
{
    ASSERT_ERR((((uint32_t)evt3opcomb << 10) & ~((uint32_t)0x00001C00)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0x00001C00)) | ((uint32_t)evt3opcomb << 10));
}

/**
 * @brief Returns the current value of the EVT3TGTADD field in the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read and the EVT3TGTADD field's value will be returned.
 *
 * @return The current value of the EVT3TGTADD field in the RWNXAGCEVT3 register.
 */
__INLINE uint16_t riu_evt3tgtadd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCEVT3_ADDR);
    return ((localVal & ((uint32_t)0x000001FF)) >> 0);
}

/**
 * @brief Sets the EVT3TGTADD field of the RWNXAGCEVT3 register.
 *
 * The RWNXAGCEVT3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] evt3tgtadd - The value to set the field to.
 */
__INLINE void riu_evt3tgtadd_setf(uint16_t evt3tgtadd)
{
    ASSERT_ERR((((uint32_t)evt3tgtadd << 0) & ~((uint32_t)0x000001FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCEVT3_ADDR, (REG_PL_RD(RIU_RWNXAGCEVT3_ADDR) & ~((uint32_t)0x000001FF)) | ((uint32_t)evt3tgtadd << 0));
}

/// @}

/**
 * @name RWNXAGCCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16          HTSTFGAINEN   1
 *     12          AGCFSMRESET   0
 *     11           RADARDETEN   0
 *     10            RIFSDETEN   1
 *     09             DSSSONLY   0
 *     08             OFDMONLY   0
 *  07:04             GPSTATUS   0x0
 *  01:00          COMBPATHSEL   0x3
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCNTL register
#define RIU_RWNXAGCCNTL_ADDR   (REG_RIU_BASE_ADDR+0xB390)
/// Offset of the RWNXAGCCNTL register from the base address
#define RIU_RWNXAGCCNTL_OFFSET 0x00000390
/// Index of the RWNXAGCCNTL register
#define RIU_RWNXAGCCNTL_INDEX  0x000000E4
/// Reset value of the RWNXAGCCNTL register
#define RIU_RWNXAGCCNTL_RESET  0x00010403

/**
 * @brief Returns the current value of the RWNXAGCCNTL register.
 * The RWNXAGCCNTL register will be read and its value returned.
 * @return The current value of the RWNXAGCCNTL register.
 */
__INLINE uint32_t riu_rwnxagccntl_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
}

/**
 * @brief Sets the RWNXAGCCNTL register to a value.
 * The RWNXAGCCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccntl_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, value);
}

// field definitions
/// HTSTFGAINEN field bit
#define RIU_HTSTFGAINEN_BIT    ((uint32_t)0x00010000)
/// HTSTFGAINEN field position
#define RIU_HTSTFGAINEN_POS    16
/// AGCFSMRESET field bit
#define RIU_AGCFSMRESET_BIT    ((uint32_t)0x00001000)
/// AGCFSMRESET field position
#define RIU_AGCFSMRESET_POS    12
/// RADARDETEN field bit
#define RIU_RADARDETEN_BIT     ((uint32_t)0x00000800)
/// RADARDETEN field position
#define RIU_RADARDETEN_POS     11
/// RIFSDETEN field bit
#define RIU_RIFSDETEN_BIT      ((uint32_t)0x00000400)
/// RIFSDETEN field position
#define RIU_RIFSDETEN_POS      10
/// DSSSONLY field bit
#define RIU_DSSSONLY_BIT       ((uint32_t)0x00000200)
/// DSSSONLY field position
#define RIU_DSSSONLY_POS       9
/// OFDMONLY field bit
#define RIU_OFDMONLY_BIT       ((uint32_t)0x00000100)
/// OFDMONLY field position
#define RIU_OFDMONLY_POS       8
/// GPSTATUS field mask
#define RIU_GPSTATUS_MASK      ((uint32_t)0x000000F0)
/// GPSTATUS field LSB position
#define RIU_GPSTATUS_LSB       4
/// GPSTATUS field width
#define RIU_GPSTATUS_WIDTH     ((uint32_t)0x00000004)
/// COMBPATHSEL field mask
#define RIU_COMBPATHSEL_MASK   ((uint32_t)0x00000003)
/// COMBPATHSEL field LSB position
#define RIU_COMBPATHSEL_LSB    0
/// COMBPATHSEL field width
#define RIU_COMBPATHSEL_WIDTH  ((uint32_t)0x00000002)

/// HTSTFGAINEN field reset value
#define RIU_HTSTFGAINEN_RST    0x1
/// AGCFSMRESET field reset value
#define RIU_AGCFSMRESET_RST    0x0
/// RADARDETEN field reset value
#define RIU_RADARDETEN_RST     0x0
/// RIFSDETEN field reset value
#define RIU_RIFSDETEN_RST      0x1
/// DSSSONLY field reset value
#define RIU_DSSSONLY_RST       0x0
/// OFDMONLY field reset value
#define RIU_OFDMONLY_RST       0x0
/// GPSTATUS field reset value
#define RIU_GPSTATUS_RST       0x0
/// COMBPATHSEL field reset value
#define RIU_COMBPATHSEL_RST    0x3

/**
 * @brief Constructs a value for the RWNXAGCCNTL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] htstfgainen - The value to use for the HTSTFGAINEN field.
 * @param[in] agcfsmreset - The value to use for the AGCFSMRESET field.
 * @param[in] radardeten - The value to use for the RADARDETEN field.
 * @param[in] rifsdeten - The value to use for the RIFSDETEN field.
 * @param[in] dsssonly - The value to use for the DSSSONLY field.
 * @param[in] ofdmonly - The value to use for the OFDMONLY field.
 * @param[in] gpstatus - The value to use for the GPSTATUS field.
 * @param[in] combpathsel - The value to use for the COMBPATHSEL field.
 */
__INLINE void riu_rwnxagccntl_pack(uint8_t htstfgainen, uint8_t agcfsmreset, uint8_t radardeten, uint8_t rifsdeten, uint8_t dsssonly, uint8_t ofdmonly, uint8_t gpstatus, uint8_t combpathsel)
{
    ASSERT_ERR((((uint32_t)htstfgainen << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)agcfsmreset << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)radardeten << 11) & ~((uint32_t)0x00000800)) == 0);
    ASSERT_ERR((((uint32_t)rifsdeten << 10) & ~((uint32_t)0x00000400)) == 0);
    ASSERT_ERR((((uint32_t)dsssonly << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)ofdmonly << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)gpstatus << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)combpathsel << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR,  ((uint32_t)htstfgainen << 16) | ((uint32_t)agcfsmreset << 12) | ((uint32_t)radardeten << 11) | ((uint32_t)rifsdeten << 10) | ((uint32_t)dsssonly << 9) | ((uint32_t)ofdmonly << 8) | ((uint32_t)gpstatus << 4) | ((uint32_t)combpathsel << 0));
}

/**
 * @brief Unpacks RWNXAGCCNTL's fields from current value of the RWNXAGCCNTL register.
 *
 * Reads the RWNXAGCCNTL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] htstfgainen - Will be populated with the current value of this field from the register.
 * @param[out] agcfsmreset - Will be populated with the current value of this field from the register.
 * @param[out] radardeten - Will be populated with the current value of this field from the register.
 * @param[out] rifsdeten - Will be populated with the current value of this field from the register.
 * @param[out] dsssonly - Will be populated with the current value of this field from the register.
 * @param[out] ofdmonly - Will be populated with the current value of this field from the register.
 * @param[out] gpstatus - Will be populated with the current value of this field from the register.
 * @param[out] combpathsel - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccntl_unpack(uint8_t* htstfgainen, uint8_t* agcfsmreset, uint8_t* radardeten, uint8_t* rifsdeten, uint8_t* dsssonly, uint8_t* ofdmonly, uint8_t* gpstatus, uint8_t* combpathsel)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);

    *htstfgainen = (localVal & ((uint32_t)0x00010000)) >> 16;
    *agcfsmreset = (localVal & ((uint32_t)0x00001000)) >> 12;
    *radardeten = (localVal & ((uint32_t)0x00000800)) >> 11;
    *rifsdeten = (localVal & ((uint32_t)0x00000400)) >> 10;
    *dsssonly = (localVal & ((uint32_t)0x00000200)) >> 9;
    *ofdmonly = (localVal & ((uint32_t)0x00000100)) >> 8;
    *gpstatus = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *combpathsel = (localVal & ((uint32_t)0x00000003)) >> 0;
}

/**
 * @brief Returns the current value of the HTSTFGAINEN field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the HTSTFGAINEN field's value will be returned.
 *
 * @return The current value of the HTSTFGAINEN field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_htstfgainen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the HTSTFGAINEN field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] htstfgainen - The value to set the field to.
 */
__INLINE void riu_htstfgainen_setf(uint8_t htstfgainen)
{
    ASSERT_ERR((((uint32_t)htstfgainen << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)htstfgainen << 16));
}

/**
 * @brief Returns the current value of the AGCFSMRESET field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the AGCFSMRESET field's value will be returned.
 *
 * @return The current value of the AGCFSMRESET field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_agcfsmreset_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the AGCFSMRESET field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcfsmreset - The value to set the field to.
 */
__INLINE void riu_agcfsmreset_setf(uint8_t agcfsmreset)
{
    ASSERT_ERR((((uint32_t)agcfsmreset << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)agcfsmreset << 12));
}

/**
 * @brief Returns the current value of the RADARDETEN field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the RADARDETEN field's value will be returned.
 *
 * @return The current value of the RADARDETEN field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_radardeten_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000800)) >> 11);
}

/**
 * @brief Sets the RADARDETEN field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] radardeten - The value to set the field to.
 */
__INLINE void riu_radardeten_setf(uint8_t radardeten)
{
    ASSERT_ERR((((uint32_t)radardeten << 11) & ~((uint32_t)0x00000800)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00000800)) | ((uint32_t)radardeten << 11));
}

/**
 * @brief Returns the current value of the RIFSDETEN field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the RIFSDETEN field's value will be returned.
 *
 * @return The current value of the RIFSDETEN field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_rifsdeten_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000400)) >> 10);
}

/**
 * @brief Sets the RIFSDETEN field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rifsdeten - The value to set the field to.
 */
__INLINE void riu_rifsdeten_setf(uint8_t rifsdeten)
{
    ASSERT_ERR((((uint32_t)rifsdeten << 10) & ~((uint32_t)0x00000400)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00000400)) | ((uint32_t)rifsdeten << 10));
}

/**
 * @brief Returns the current value of the DSSSONLY field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the DSSSONLY field's value will be returned.
 *
 * @return The current value of the DSSSONLY field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_dsssonly_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the DSSSONLY field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dsssonly - The value to set the field to.
 */
__INLINE void riu_dsssonly_setf(uint8_t dsssonly)
{
    ASSERT_ERR((((uint32_t)dsssonly << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)dsssonly << 9));
}

/**
 * @brief Returns the current value of the OFDMONLY field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the OFDMONLY field's value will be returned.
 *
 * @return The current value of the OFDMONLY field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_ofdmonly_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the OFDMONLY field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ofdmonly - The value to set the field to.
 */
__INLINE void riu_ofdmonly_setf(uint8_t ofdmonly)
{
    ASSERT_ERR((((uint32_t)ofdmonly << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)ofdmonly << 8));
}

/**
 * @brief Returns the current value of the GPSTATUS field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the GPSTATUS field's value will be returned.
 *
 * @return The current value of the GPSTATUS field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_gpstatus_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Sets the GPSTATUS field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] gpstatus - The value to set the field to.
 */
__INLINE void riu_gpstatus_setf(uint8_t gpstatus)
{
    ASSERT_ERR((((uint32_t)gpstatus << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)gpstatus << 4));
}

/**
 * @brief Returns the current value of the COMBPATHSEL field in the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read and the COMBPATHSEL field's value will be returned.
 *
 * @return The current value of the COMBPATHSEL field in the RWNXAGCCNTL register.
 */
__INLINE uint8_t riu_combpathsel_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

/**
 * @brief Sets the COMBPATHSEL field of the RWNXAGCCNTL register.
 *
 * The RWNXAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] combpathsel - The value to set the field to.
 */
__INLINE void riu_combpathsel_setf(uint8_t combpathsel)
{
    ASSERT_ERR((((uint32_t)combpathsel << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(RIU_RWNXAGCCNTL_ADDR, (REG_PL_RD(RIU_RWNXAGCCNTL_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)combpathsel << 0));
}

/// @}

/**
 * @name RWNXAGCDSP0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  27:24              NBITADC   0xC
 *  23:16         VPEAKADCQDBV   0xE8
 *  15:08         ADCPOWMINDBV   0xB5
 *  07:00       ADCPOWBIASQDBV   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDSP0 register
#define RIU_RWNXAGCDSP0_ADDR   (REG_RIU_BASE_ADDR+0xB394)
/// Offset of the RWNXAGCDSP0 register from the base address
#define RIU_RWNXAGCDSP0_OFFSET 0x00000394
/// Index of the RWNXAGCDSP0 register
#define RIU_RWNXAGCDSP0_INDEX  0x000000E5
/// Reset value of the RWNXAGCDSP0 register
#define RIU_RWNXAGCDSP0_RESET  0x0CE8B500

/**
 * @brief Returns the current value of the RWNXAGCDSP0 register.
 * The RWNXAGCDSP0 register will be read and its value returned.
 * @return The current value of the RWNXAGCDSP0 register.
 */
__INLINE uint32_t riu_rwnxagcdsp0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);
}

/**
 * @brief Sets the RWNXAGCDSP0 register to a value.
 * The RWNXAGCDSP0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdsp0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR, value);
}

// field definitions
/// NBITADC field mask
#define RIU_NBITADC_MASK          ((uint32_t)0x0F000000)
/// NBITADC field LSB position
#define RIU_NBITADC_LSB           24
/// NBITADC field width
#define RIU_NBITADC_WIDTH         ((uint32_t)0x00000004)
/// VPEAKADCQDBV field mask
#define RIU_VPEAKADCQDBV_MASK     ((uint32_t)0x00FF0000)
/// VPEAKADCQDBV field LSB position
#define RIU_VPEAKADCQDBV_LSB      16
/// VPEAKADCQDBV field width
#define RIU_VPEAKADCQDBV_WIDTH    ((uint32_t)0x00000008)
/// ADCPOWMINDBV field mask
#define RIU_ADCPOWMINDBV_MASK     ((uint32_t)0x0000FF00)
/// ADCPOWMINDBV field LSB position
#define RIU_ADCPOWMINDBV_LSB      8
/// ADCPOWMINDBV field width
#define RIU_ADCPOWMINDBV_WIDTH    ((uint32_t)0x00000008)
/// ADCPOWBIASQDBV field mask
#define RIU_ADCPOWBIASQDBV_MASK   ((uint32_t)0x000000FF)
/// ADCPOWBIASQDBV field LSB position
#define RIU_ADCPOWBIASQDBV_LSB    0
/// ADCPOWBIASQDBV field width
#define RIU_ADCPOWBIASQDBV_WIDTH  ((uint32_t)0x00000008)

/// NBITADC field reset value
#define RIU_NBITADC_RST           0xC
/// VPEAKADCQDBV field reset value
#define RIU_VPEAKADCQDBV_RST      0xE8
/// ADCPOWMINDBV field reset value
#define RIU_ADCPOWMINDBV_RST      0xB5
/// ADCPOWBIASQDBV field reset value
#define RIU_ADCPOWBIASQDBV_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDSP0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] nbitadc - The value to use for the NBITADC field.
 * @param[in] vpeakadcqdbv - The value to use for the VPEAKADCQDBV field.
 * @param[in] adcpowmindbv - The value to use for the ADCPOWMINDBV field.
 * @param[in] adcpowbiasqdbv - The value to use for the ADCPOWBIASQDBV field.
 */
__INLINE void riu_rwnxagcdsp0_pack(uint8_t nbitadc, uint8_t vpeakadcqdbv, uint8_t adcpowmindbv, uint8_t adcpowbiasqdbv)
{
    ASSERT_ERR((((uint32_t)nbitadc << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)vpeakadcqdbv << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)adcpowmindbv << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)adcpowbiasqdbv << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR,  ((uint32_t)nbitadc << 24) | ((uint32_t)vpeakadcqdbv << 16) | ((uint32_t)adcpowmindbv << 8) | ((uint32_t)adcpowbiasqdbv << 0));
}

/**
 * @brief Unpacks RWNXAGCDSP0's fields from current value of the RWNXAGCDSP0 register.
 *
 * Reads the RWNXAGCDSP0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] nbitadc - Will be populated with the current value of this field from the register.
 * @param[out] vpeakadcqdbv - Will be populated with the current value of this field from the register.
 * @param[out] adcpowmindbv - Will be populated with the current value of this field from the register.
 * @param[out] adcpowbiasqdbv - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdsp0_unpack(uint8_t* nbitadc, uint8_t* vpeakadcqdbv, uint8_t* adcpowmindbv, uint8_t* adcpowbiasqdbv)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);

    *nbitadc = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *vpeakadcqdbv = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *adcpowmindbv = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *adcpowbiasqdbv = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the NBITADC field in the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read and the NBITADC field's value will be returned.
 *
 * @return The current value of the NBITADC field in the RWNXAGCDSP0 register.
 */
__INLINE uint8_t riu_nbitadc_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the NBITADC field of the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] nbitadc - The value to set the field to.
 */
__INLINE void riu_nbitadc_setf(uint8_t nbitadc)
{
    ASSERT_ERR((((uint32_t)nbitadc << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP0_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)nbitadc << 24));
}

/**
 * @brief Returns the current value of the VPEAKADCQDBV field in the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read and the VPEAKADCQDBV field's value will be returned.
 *
 * @return The current value of the VPEAKADCQDBV field in the RWNXAGCDSP0 register.
 */
__INLINE uint8_t riu_vpeakadcqdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the VPEAKADCQDBV field of the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] vpeakadcqdbv - The value to set the field to.
 */
__INLINE void riu_vpeakadcqdbv_setf(uint8_t vpeakadcqdbv)
{
    ASSERT_ERR((((uint32_t)vpeakadcqdbv << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP0_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)vpeakadcqdbv << 16));
}

/**
 * @brief Returns the current value of the ADCPOWMINDBV field in the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read and the ADCPOWMINDBV field's value will be returned.
 *
 * @return The current value of the ADCPOWMINDBV field in the RWNXAGCDSP0 register.
 */
__INLINE uint8_t riu_adcpowmindbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the ADCPOWMINDBV field of the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowmindbv - The value to set the field to.
 */
__INLINE void riu_adcpowmindbv_setf(uint8_t adcpowmindbv)
{
    ASSERT_ERR((((uint32_t)adcpowmindbv << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP0_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)adcpowmindbv << 8));
}

/**
 * @brief Returns the current value of the ADCPOWBIASQDBV field in the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read and the ADCPOWBIASQDBV field's value will be returned.
 *
 * @return The current value of the ADCPOWBIASQDBV field in the RWNXAGCDSP0 register.
 */
__INLINE uint8_t riu_adcpowbiasqdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP0_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the ADCPOWBIASQDBV field of the RWNXAGCDSP0 register.
 *
 * The RWNXAGCDSP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowbiasqdbv - The value to set the field to.
 */
__INLINE void riu_adcpowbiasqdbv_setf(uint8_t adcpowbiasqdbv)
{
    ASSERT_ERR((((uint32_t)adcpowbiasqdbv << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP0_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP0_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)adcpowbiasqdbv << 0));
}

/// @}

/**
 * @name RWNXAGCDSP1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08         ADCPOWMINDBM   0x9C
 *  07:00       ADCPOWBIASQDBM   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDSP1 register
#define RIU_RWNXAGCDSP1_ADDR   (REG_RIU_BASE_ADDR+0xB398)
/// Offset of the RWNXAGCDSP1 register from the base address
#define RIU_RWNXAGCDSP1_OFFSET 0x00000398
/// Index of the RWNXAGCDSP1 register
#define RIU_RWNXAGCDSP1_INDEX  0x000000E6
/// Reset value of the RWNXAGCDSP1 register
#define RIU_RWNXAGCDSP1_RESET  0x00009C00

/**
 * @brief Returns the current value of the RWNXAGCDSP1 register.
 * The RWNXAGCDSP1 register will be read and its value returned.
 * @return The current value of the RWNXAGCDSP1 register.
 */
__INLINE uint32_t riu_rwnxagcdsp1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDSP1_ADDR);
}

/**
 * @brief Sets the RWNXAGCDSP1 register to a value.
 * The RWNXAGCDSP1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdsp1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDSP1_ADDR, value);
}

// field definitions
/// ADCPOWMINDBM field mask
#define RIU_ADCPOWMINDBM_MASK     ((uint32_t)0x0000FF00)
/// ADCPOWMINDBM field LSB position
#define RIU_ADCPOWMINDBM_LSB      8
/// ADCPOWMINDBM field width
#define RIU_ADCPOWMINDBM_WIDTH    ((uint32_t)0x00000008)
/// ADCPOWBIASQDBM field mask
#define RIU_ADCPOWBIASQDBM_MASK   ((uint32_t)0x000000FF)
/// ADCPOWBIASQDBM field LSB position
#define RIU_ADCPOWBIASQDBM_LSB    0
/// ADCPOWBIASQDBM field width
#define RIU_ADCPOWBIASQDBM_WIDTH  ((uint32_t)0x00000008)

/// ADCPOWMINDBM field reset value
#define RIU_ADCPOWMINDBM_RST      0x9C
/// ADCPOWBIASQDBM field reset value
#define RIU_ADCPOWBIASQDBM_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDSP1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] adcpowmindbm - The value to use for the ADCPOWMINDBM field.
 * @param[in] adcpowbiasqdbm - The value to use for the ADCPOWBIASQDBM field.
 */
__INLINE void riu_rwnxagcdsp1_pack(uint8_t adcpowmindbm, uint8_t adcpowbiasqdbm)
{
    ASSERT_ERR((((uint32_t)adcpowmindbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)adcpowbiasqdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP1_ADDR,  ((uint32_t)adcpowmindbm << 8) | ((uint32_t)adcpowbiasqdbm << 0));
}

/**
 * @brief Unpacks RWNXAGCDSP1's fields from current value of the RWNXAGCDSP1 register.
 *
 * Reads the RWNXAGCDSP1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] adcpowmindbm - Will be populated with the current value of this field from the register.
 * @param[out] adcpowbiasqdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdsp1_unpack(uint8_t* adcpowmindbm, uint8_t* adcpowbiasqdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP1_ADDR);

    *adcpowmindbm = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *adcpowbiasqdbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the ADCPOWMINDBM field in the RWNXAGCDSP1 register.
 *
 * The RWNXAGCDSP1 register will be read and the ADCPOWMINDBM field's value will be returned.
 *
 * @return The current value of the ADCPOWMINDBM field in the RWNXAGCDSP1 register.
 */
__INLINE uint8_t riu_adcpowmindbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP1_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the ADCPOWMINDBM field of the RWNXAGCDSP1 register.
 *
 * The RWNXAGCDSP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowmindbm - The value to set the field to.
 */
__INLINE void riu_adcpowmindbm_setf(uint8_t adcpowmindbm)
{
    ASSERT_ERR((((uint32_t)adcpowmindbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP1_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)adcpowmindbm << 8));
}

/**
 * @brief Returns the current value of the ADCPOWBIASQDBM field in the RWNXAGCDSP1 register.
 *
 * The RWNXAGCDSP1 register will be read and the ADCPOWBIASQDBM field's value will be returned.
 *
 * @return The current value of the ADCPOWBIASQDBM field in the RWNXAGCDSP1 register.
 */
__INLINE uint8_t riu_adcpowbiasqdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the ADCPOWBIASQDBM field of the RWNXAGCDSP1 register.
 *
 * The RWNXAGCDSP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowbiasqdbm - The value to set the field to.
 */
__INLINE void riu_adcpowbiasqdbm_setf(uint8_t adcpowbiasqdbm)
{
    ASSERT_ERR((((uint32_t)adcpowbiasqdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP1_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)adcpowbiasqdbm << 0));
}

/// @}

/**
 * @name RWNXAGCDSP2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  23:16    INBDPOW40SBIASQDB   0x0
 *  15:08    INBDPOW20SBIASQDB   0x0
 *  07:00    INBDPOW20PBIASQDB   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDSP2 register
#define RIU_RWNXAGCDSP2_ADDR   (REG_RIU_BASE_ADDR+0xB39C)
/// Offset of the RWNXAGCDSP2 register from the base address
#define RIU_RWNXAGCDSP2_OFFSET 0x0000039C
/// Index of the RWNXAGCDSP2 register
#define RIU_RWNXAGCDSP2_INDEX  0x000000E7
/// Reset value of the RWNXAGCDSP2 register
#define RIU_RWNXAGCDSP2_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCDSP2 register.
 * The RWNXAGCDSP2 register will be read and its value returned.
 * @return The current value of the RWNXAGCDSP2 register.
 */
__INLINE uint32_t riu_rwnxagcdsp2_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDSP2_ADDR);
}

/**
 * @brief Sets the RWNXAGCDSP2 register to a value.
 * The RWNXAGCDSP2 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdsp2_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDSP2_ADDR, value);
}

// field definitions
/// INBDPOW40SBIASQDB field mask
#define RIU_INBDPOW40SBIASQDB_MASK   ((uint32_t)0x00FF0000)
/// INBDPOW40SBIASQDB field LSB position
#define RIU_INBDPOW40SBIASQDB_LSB    16
/// INBDPOW40SBIASQDB field width
#define RIU_INBDPOW40SBIASQDB_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20SBIASQDB field mask
#define RIU_INBDPOW20SBIASQDB_MASK   ((uint32_t)0x0000FF00)
/// INBDPOW20SBIASQDB field LSB position
#define RIU_INBDPOW20SBIASQDB_LSB    8
/// INBDPOW20SBIASQDB field width
#define RIU_INBDPOW20SBIASQDB_WIDTH  ((uint32_t)0x00000008)
/// INBDPOW20PBIASQDB field mask
#define RIU_INBDPOW20PBIASQDB_MASK   ((uint32_t)0x000000FF)
/// INBDPOW20PBIASQDB field LSB position
#define RIU_INBDPOW20PBIASQDB_LSB    0
/// INBDPOW20PBIASQDB field width
#define RIU_INBDPOW20PBIASQDB_WIDTH  ((uint32_t)0x00000008)

/// INBDPOW40SBIASQDB field reset value
#define RIU_INBDPOW40SBIASQDB_RST    0x0
/// INBDPOW20SBIASQDB field reset value
#define RIU_INBDPOW20SBIASQDB_RST    0x0
/// INBDPOW20PBIASQDB field reset value
#define RIU_INBDPOW20PBIASQDB_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDSP2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] inbdpow40sbiasqdb - The value to use for the INBDPOW40SBIASQDB field.
 * @param[in] inbdpow20sbiasqdb - The value to use for the INBDPOW20SBIASQDB field.
 * @param[in] inbdpow20pbiasqdb - The value to use for the INBDPOW20PBIASQDB field.
 */
__INLINE void riu_rwnxagcdsp2_pack(uint8_t inbdpow40sbiasqdb, uint8_t inbdpow20sbiasqdb, uint8_t inbdpow20pbiasqdb)
{
    ASSERT_ERR((((uint32_t)inbdpow40sbiasqdb << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)inbdpow20sbiasqdb << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)inbdpow20pbiasqdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP2_ADDR,  ((uint32_t)inbdpow40sbiasqdb << 16) | ((uint32_t)inbdpow20sbiasqdb << 8) | ((uint32_t)inbdpow20pbiasqdb << 0));
}

/**
 * @brief Unpacks RWNXAGCDSP2's fields from current value of the RWNXAGCDSP2 register.
 *
 * Reads the RWNXAGCDSP2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpow40sbiasqdb - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20sbiasqdb - Will be populated with the current value of this field from the register.
 * @param[out] inbdpow20pbiasqdb - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdsp2_unpack(uint8_t* inbdpow40sbiasqdb, uint8_t* inbdpow20sbiasqdb, uint8_t* inbdpow20pbiasqdb)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP2_ADDR);

    *inbdpow40sbiasqdb = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *inbdpow20sbiasqdb = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *inbdpow20pbiasqdb = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOW40SBIASQDB field in the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read and the INBDPOW40SBIASQDB field's value will be returned.
 *
 * @return The current value of the INBDPOW40SBIASQDB field in the RWNXAGCDSP2 register.
 */
__INLINE uint8_t riu_inbdpow40sbiasqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP2_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the INBDPOW40SBIASQDB field of the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpow40sbiasqdb - The value to set the field to.
 */
__INLINE void riu_inbdpow40sbiasqdb_setf(uint8_t inbdpow40sbiasqdb)
{
    ASSERT_ERR((((uint32_t)inbdpow40sbiasqdb << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP2_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)inbdpow40sbiasqdb << 16));
}

/**
 * @brief Returns the current value of the INBDPOW20SBIASQDB field in the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read and the INBDPOW20SBIASQDB field's value will be returned.
 *
 * @return The current value of the INBDPOW20SBIASQDB field in the RWNXAGCDSP2 register.
 */
__INLINE uint8_t riu_inbdpow20sbiasqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP2_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the INBDPOW20SBIASQDB field of the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpow20sbiasqdb - The value to set the field to.
 */
__INLINE void riu_inbdpow20sbiasqdb_setf(uint8_t inbdpow20sbiasqdb)
{
    ASSERT_ERR((((uint32_t)inbdpow20sbiasqdb << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP2_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)inbdpow20sbiasqdb << 8));
}

/**
 * @brief Returns the current value of the INBDPOW20PBIASQDB field in the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read and the INBDPOW20PBIASQDB field's value will be returned.
 *
 * @return The current value of the INBDPOW20PBIASQDB field in the RWNXAGCDSP2 register.
 */
__INLINE uint8_t riu_inbdpow20pbiasqdb_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP2_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the INBDPOW20PBIASQDB field of the RWNXAGCDSP2 register.
 *
 * The RWNXAGCDSP2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpow20pbiasqdb - The value to set the field to.
 */
__INLINE void riu_inbdpow20pbiasqdb_setf(uint8_t inbdpow20pbiasqdb)
{
    ASSERT_ERR((((uint32_t)inbdpow20pbiasqdb << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP2_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP2_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)inbdpow20pbiasqdb << 0));
}

/// @}

/**
 * @name RWNXAGCDSP3 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  27:20        INBDPOWMINDBV   0xBF
 *  17:16              INBDRND   0x3
 *  07:00        INBDPOWMINDBM   0x9C
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDSP3 register
#define RIU_RWNXAGCDSP3_ADDR   (REG_RIU_BASE_ADDR+0xB3A0)
/// Offset of the RWNXAGCDSP3 register from the base address
#define RIU_RWNXAGCDSP3_OFFSET 0x000003A0
/// Index of the RWNXAGCDSP3 register
#define RIU_RWNXAGCDSP3_INDEX  0x000000E8
/// Reset value of the RWNXAGCDSP3 register
#define RIU_RWNXAGCDSP3_RESET  0x0BF3009C

/**
 * @brief Returns the current value of the RWNXAGCDSP3 register.
 * The RWNXAGCDSP3 register will be read and its value returned.
 * @return The current value of the RWNXAGCDSP3 register.
 */
__INLINE uint32_t riu_rwnxagcdsp3_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDSP3_ADDR);
}

/**
 * @brief Sets the RWNXAGCDSP3 register to a value.
 * The RWNXAGCDSP3 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdsp3_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDSP3_ADDR, value);
}

// field definitions
/// INBDPOWMINDBV field mask
#define RIU_INBDPOWMINDBV_MASK   ((uint32_t)0x0FF00000)
/// INBDPOWMINDBV field LSB position
#define RIU_INBDPOWMINDBV_LSB    20
/// INBDPOWMINDBV field width
#define RIU_INBDPOWMINDBV_WIDTH  ((uint32_t)0x00000008)
/// INBDRND field mask
#define RIU_INBDRND_MASK         ((uint32_t)0x00030000)
/// INBDRND field LSB position
#define RIU_INBDRND_LSB          16
/// INBDRND field width
#define RIU_INBDRND_WIDTH        ((uint32_t)0x00000002)
/// INBDPOWMINDBM field mask
#define RIU_INBDPOWMINDBM_MASK   ((uint32_t)0x000000FF)
/// INBDPOWMINDBM field LSB position
#define RIU_INBDPOWMINDBM_LSB    0
/// INBDPOWMINDBM field width
#define RIU_INBDPOWMINDBM_WIDTH  ((uint32_t)0x00000008)

/// INBDPOWMINDBV field reset value
#define RIU_INBDPOWMINDBV_RST    0xBF
/// INBDRND field reset value
#define RIU_INBDRND_RST          0x3
/// INBDPOWMINDBM field reset value
#define RIU_INBDPOWMINDBM_RST    0x9C

/**
 * @brief Constructs a value for the RWNXAGCDSP3 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] inbdpowmindbv - The value to use for the INBDPOWMINDBV field.
 * @param[in] inbdrnd - The value to use for the INBDRND field.
 * @param[in] inbdpowmindbm - The value to use for the INBDPOWMINDBM field.
 */
__INLINE void riu_rwnxagcdsp3_pack(uint8_t inbdpowmindbv, uint8_t inbdrnd, uint8_t inbdpowmindbm)
{
    ASSERT_ERR((((uint32_t)inbdpowmindbv << 20) & ~((uint32_t)0x0FF00000)) == 0);
    ASSERT_ERR((((uint32_t)inbdrnd << 16) & ~((uint32_t)0x00030000)) == 0);
    ASSERT_ERR((((uint32_t)inbdpowmindbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP3_ADDR,  ((uint32_t)inbdpowmindbv << 20) | ((uint32_t)inbdrnd << 16) | ((uint32_t)inbdpowmindbm << 0));
}

/**
 * @brief Unpacks RWNXAGCDSP3's fields from current value of the RWNXAGCDSP3 register.
 *
 * Reads the RWNXAGCDSP3 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdpowmindbv - Will be populated with the current value of this field from the register.
 * @param[out] inbdrnd - Will be populated with the current value of this field from the register.
 * @param[out] inbdpowmindbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdsp3_unpack(uint8_t* inbdpowmindbv, uint8_t* inbdrnd, uint8_t* inbdpowmindbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP3_ADDR);

    *inbdpowmindbv = (localVal & ((uint32_t)0x0FF00000)) >> 20;
    *inbdrnd = (localVal & ((uint32_t)0x00030000)) >> 16;
    *inbdpowmindbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDPOWMINDBV field in the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read and the INBDPOWMINDBV field's value will be returned.
 *
 * @return The current value of the INBDPOWMINDBV field in the RWNXAGCDSP3 register.
 */
__INLINE uint8_t riu_inbdpowmindbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP3_ADDR);
    return ((localVal & ((uint32_t)0x0FF00000)) >> 20);
}

/**
 * @brief Sets the INBDPOWMINDBV field of the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpowmindbv - The value to set the field to.
 */
__INLINE void riu_inbdpowmindbv_setf(uint8_t inbdpowmindbv)
{
    ASSERT_ERR((((uint32_t)inbdpowmindbv << 20) & ~((uint32_t)0x0FF00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP3_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP3_ADDR) & ~((uint32_t)0x0FF00000)) | ((uint32_t)inbdpowmindbv << 20));
}

/**
 * @brief Returns the current value of the INBDRND field in the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read and the INBDRND field's value will be returned.
 *
 * @return The current value of the INBDRND field in the RWNXAGCDSP3 register.
 */
__INLINE uint8_t riu_inbdrnd_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP3_ADDR);
    return ((localVal & ((uint32_t)0x00030000)) >> 16);
}

/**
 * @brief Sets the INBDRND field of the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdrnd - The value to set the field to.
 */
__INLINE void riu_inbdrnd_setf(uint8_t inbdrnd)
{
    ASSERT_ERR((((uint32_t)inbdrnd << 16) & ~((uint32_t)0x00030000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP3_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP3_ADDR) & ~((uint32_t)0x00030000)) | ((uint32_t)inbdrnd << 16));
}

/**
 * @brief Returns the current value of the INBDPOWMINDBM field in the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read and the INBDPOWMINDBM field's value will be returned.
 *
 * @return The current value of the INBDPOWMINDBM field in the RWNXAGCDSP3 register.
 */
__INLINE uint8_t riu_inbdpowmindbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP3_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the INBDPOWMINDBM field of the RWNXAGCDSP3 register.
 *
 * The RWNXAGCDSP3 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpowmindbm - The value to set the field to.
 */
__INLINE void riu_inbdpowmindbm_setf(uint8_t inbdpowmindbm)
{
    ASSERT_ERR((((uint32_t)inbdpowmindbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP3_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP3_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)inbdpowmindbm << 0));
}

/// @}

/**
 * @name RWNXAGCDSP4 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:08             FE40GAIN   0x2
 *  07:00             FE20GAIN   0x2
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDSP4 register
#define RIU_RWNXAGCDSP4_ADDR   (REG_RIU_BASE_ADDR+0xB3A4)
/// Offset of the RWNXAGCDSP4 register from the base address
#define RIU_RWNXAGCDSP4_OFFSET 0x000003A4
/// Index of the RWNXAGCDSP4 register
#define RIU_RWNXAGCDSP4_INDEX  0x000000E9
/// Reset value of the RWNXAGCDSP4 register
#define RIU_RWNXAGCDSP4_RESET  0x00000202

/**
 * @brief Returns the current value of the RWNXAGCDSP4 register.
 * The RWNXAGCDSP4 register will be read and its value returned.
 * @return The current value of the RWNXAGCDSP4 register.
 */
__INLINE uint32_t riu_rwnxagcdsp4_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDSP4_ADDR);
}

/**
 * @brief Sets the RWNXAGCDSP4 register to a value.
 * The RWNXAGCDSP4 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdsp4_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDSP4_ADDR, value);
}

// field definitions
/// FE40GAIN field mask
#define RIU_FE40GAIN_MASK   ((uint32_t)0x0000FF00)
/// FE40GAIN field LSB position
#define RIU_FE40GAIN_LSB    8
/// FE40GAIN field width
#define RIU_FE40GAIN_WIDTH  ((uint32_t)0x00000008)
/// FE20GAIN field mask
#define RIU_FE20GAIN_MASK   ((uint32_t)0x000000FF)
/// FE20GAIN field LSB position
#define RIU_FE20GAIN_LSB    0
/// FE20GAIN field width
#define RIU_FE20GAIN_WIDTH  ((uint32_t)0x00000008)

/// FE40GAIN field reset value
#define RIU_FE40GAIN_RST    0x2
/// FE20GAIN field reset value
#define RIU_FE20GAIN_RST    0x2

/**
 * @brief Constructs a value for the RWNXAGCDSP4 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] fe40gain - The value to use for the FE40GAIN field.
 * @param[in] fe20gain - The value to use for the FE20GAIN field.
 */
__INLINE void riu_rwnxagcdsp4_pack(uint8_t fe40gain, uint8_t fe20gain)
{
    ASSERT_ERR((((uint32_t)fe40gain << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)fe20gain << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP4_ADDR,  ((uint32_t)fe40gain << 8) | ((uint32_t)fe20gain << 0));
}

/**
 * @brief Unpacks RWNXAGCDSP4's fields from current value of the RWNXAGCDSP4 register.
 *
 * Reads the RWNXAGCDSP4 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] fe40gain - Will be populated with the current value of this field from the register.
 * @param[out] fe20gain - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdsp4_unpack(uint8_t* fe40gain, uint8_t* fe20gain)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP4_ADDR);

    *fe40gain = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *fe20gain = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the FE40GAIN field in the RWNXAGCDSP4 register.
 *
 * The RWNXAGCDSP4 register will be read and the FE40GAIN field's value will be returned.
 *
 * @return The current value of the FE40GAIN field in the RWNXAGCDSP4 register.
 */
__INLINE uint8_t riu_fe40gain_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP4_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the FE40GAIN field of the RWNXAGCDSP4 register.
 *
 * The RWNXAGCDSP4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fe40gain - The value to set the field to.
 */
__INLINE void riu_fe40gain_setf(uint8_t fe40gain)
{
    ASSERT_ERR((((uint32_t)fe40gain << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP4_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP4_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)fe40gain << 8));
}

/**
 * @brief Returns the current value of the FE20GAIN field in the RWNXAGCDSP4 register.
 *
 * The RWNXAGCDSP4 register will be read and the FE20GAIN field's value will be returned.
 *
 * @return The current value of the FE20GAIN field in the RWNXAGCDSP4 register.
 */
__INLINE uint8_t riu_fe20gain_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDSP4_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the FE20GAIN field of the RWNXAGCDSP4 register.
 *
 * The RWNXAGCDSP4 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fe20gain - The value to set the field to.
 */
__INLINE void riu_fe20gain_setf(uint8_t fe20gain)
{
    ASSERT_ERR((((uint32_t)fe20gain << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCDSP4_ADDR, (REG_PL_RD(RIU_RWNXAGCDSP4_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)fe20gain << 0));
}

/// @}

/**
 * @name RWNXAGCCCA0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  22:20      DELCCARAMPDNTAP   0x5
 *  18:16      DELCCARAMPUPTAP   0x5
 *  13:08         CCADETRAMPDN   0x9
 *  05:00         CCADETRAMPUP   0x9
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCA0 register
#define RIU_RWNXAGCCCA0_ADDR   (REG_RIU_BASE_ADDR+0xB3A8)
/// Offset of the RWNXAGCCCA0 register from the base address
#define RIU_RWNXAGCCCA0_OFFSET 0x000003A8
/// Index of the RWNXAGCCCA0 register
#define RIU_RWNXAGCCCA0_INDEX  0x000000EA
/// Reset value of the RWNXAGCCCA0 register
#define RIU_RWNXAGCCCA0_RESET  0x00550909

/**
 * @brief Returns the current value of the RWNXAGCCCA0 register.
 * The RWNXAGCCCA0 register will be read and its value returned.
 * @return The current value of the RWNXAGCCCA0 register.
 */
__INLINE uint32_t riu_rwnxagccca0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCA0 register to a value.
 * The RWNXAGCCCA0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccca0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR, value);
}

// field definitions
/// DELCCARAMPDNTAP field mask
#define RIU_DELCCARAMPDNTAP_MASK   ((uint32_t)0x00700000)
/// DELCCARAMPDNTAP field LSB position
#define RIU_DELCCARAMPDNTAP_LSB    20
/// DELCCARAMPDNTAP field width
#define RIU_DELCCARAMPDNTAP_WIDTH  ((uint32_t)0x00000003)
/// DELCCARAMPUPTAP field mask
#define RIU_DELCCARAMPUPTAP_MASK   ((uint32_t)0x00070000)
/// DELCCARAMPUPTAP field LSB position
#define RIU_DELCCARAMPUPTAP_LSB    16
/// DELCCARAMPUPTAP field width
#define RIU_DELCCARAMPUPTAP_WIDTH  ((uint32_t)0x00000003)
/// CCADETRAMPDN field mask
#define RIU_CCADETRAMPDN_MASK      ((uint32_t)0x00003F00)
/// CCADETRAMPDN field LSB position
#define RIU_CCADETRAMPDN_LSB       8
/// CCADETRAMPDN field width
#define RIU_CCADETRAMPDN_WIDTH     ((uint32_t)0x00000006)
/// CCADETRAMPUP field mask
#define RIU_CCADETRAMPUP_MASK      ((uint32_t)0x0000003F)
/// CCADETRAMPUP field LSB position
#define RIU_CCADETRAMPUP_LSB       0
/// CCADETRAMPUP field width
#define RIU_CCADETRAMPUP_WIDTH     ((uint32_t)0x00000006)

/// DELCCARAMPDNTAP field reset value
#define RIU_DELCCARAMPDNTAP_RST    0x5
/// DELCCARAMPUPTAP field reset value
#define RIU_DELCCARAMPUPTAP_RST    0x5
/// CCADETRAMPDN field reset value
#define RIU_CCADETRAMPDN_RST       0x9
/// CCADETRAMPUP field reset value
#define RIU_CCADETRAMPUP_RST       0x9

/**
 * @brief Constructs a value for the RWNXAGCCCA0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] delccarampdntap - The value to use for the DELCCARAMPDNTAP field.
 * @param[in] delccarampuptap - The value to use for the DELCCARAMPUPTAP field.
 * @param[in] ccadetrampdn - The value to use for the CCADETRAMPDN field.
 * @param[in] ccadetrampup - The value to use for the CCADETRAMPUP field.
 */
__INLINE void riu_rwnxagccca0_pack(uint8_t delccarampdntap, uint8_t delccarampuptap, uint8_t ccadetrampdn, uint8_t ccadetrampup)
{
    ASSERT_ERR((((uint32_t)delccarampdntap << 20) & ~((uint32_t)0x00700000)) == 0);
    ASSERT_ERR((((uint32_t)delccarampuptap << 16) & ~((uint32_t)0x00070000)) == 0);
    ASSERT_ERR((((uint32_t)ccadetrampdn << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)ccadetrampup << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR,  ((uint32_t)delccarampdntap << 20) | ((uint32_t)delccarampuptap << 16) | ((uint32_t)ccadetrampdn << 8) | ((uint32_t)ccadetrampup << 0));
}

/**
 * @brief Unpacks RWNXAGCCCA0's fields from current value of the RWNXAGCCCA0 register.
 *
 * Reads the RWNXAGCCCA0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] delccarampdntap - Will be populated with the current value of this field from the register.
 * @param[out] delccarampuptap - Will be populated with the current value of this field from the register.
 * @param[out] ccadetrampdn - Will be populated with the current value of this field from the register.
 * @param[out] ccadetrampup - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccca0_unpack(uint8_t* delccarampdntap, uint8_t* delccarampuptap, uint8_t* ccadetrampdn, uint8_t* ccadetrampup)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);

    *delccarampdntap = (localVal & ((uint32_t)0x00700000)) >> 20;
    *delccarampuptap = (localVal & ((uint32_t)0x00070000)) >> 16;
    *ccadetrampdn = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *ccadetrampup = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the DELCCARAMPDNTAP field in the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read and the DELCCARAMPDNTAP field's value will be returned.
 *
 * @return The current value of the DELCCARAMPDNTAP field in the RWNXAGCCCA0 register.
 */
__INLINE uint8_t riu_delccarampdntap_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Sets the DELCCARAMPDNTAP field of the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] delccarampdntap - The value to set the field to.
 */
__INLINE void riu_delccarampdntap_setf(uint8_t delccarampdntap)
{
    ASSERT_ERR((((uint32_t)delccarampdntap << 20) & ~((uint32_t)0x00700000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA0_ADDR) & ~((uint32_t)0x00700000)) | ((uint32_t)delccarampdntap << 20));
}

/**
 * @brief Returns the current value of the DELCCARAMPUPTAP field in the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read and the DELCCARAMPUPTAP field's value will be returned.
 *
 * @return The current value of the DELCCARAMPUPTAP field in the RWNXAGCCCA0 register.
 */
__INLINE uint8_t riu_delccarampuptap_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);
    return ((localVal & ((uint32_t)0x00070000)) >> 16);
}

/**
 * @brief Sets the DELCCARAMPUPTAP field of the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] delccarampuptap - The value to set the field to.
 */
__INLINE void riu_delccarampuptap_setf(uint8_t delccarampuptap)
{
    ASSERT_ERR((((uint32_t)delccarampuptap << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA0_ADDR) & ~((uint32_t)0x00070000)) | ((uint32_t)delccarampuptap << 16));
}

/**
 * @brief Returns the current value of the CCADETRAMPDN field in the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read and the CCADETRAMPDN field's value will be returned.
 *
 * @return The current value of the CCADETRAMPDN field in the RWNXAGCCCA0 register.
 */
__INLINE uint8_t riu_ccadetrampdn_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the CCADETRAMPDN field of the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccadetrampdn - The value to set the field to.
 */
__INLINE void riu_ccadetrampdn_setf(uint8_t ccadetrampdn)
{
    ASSERT_ERR((((uint32_t)ccadetrampdn << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA0_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)ccadetrampdn << 8));
}

/**
 * @brief Returns the current value of the CCADETRAMPUP field in the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read and the CCADETRAMPUP field's value will be returned.
 *
 * @return The current value of the CCADETRAMPUP field in the RWNXAGCCCA0 register.
 */
__INLINE uint8_t riu_ccadetrampup_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the CCADETRAMPUP field of the RWNXAGCCCA0 register.
 *
 * The RWNXAGCCCA0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccadetrampup - The value to set the field to.
 */
__INLINE void riu_ccadetrampup_setf(uint8_t ccadetrampup)
{
    ASSERT_ERR((((uint32_t)ccadetrampup << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA0_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)ccadetrampup << 0));
}

/// @}

/**
 * @name RWNXAGCCCA1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  28:20     INBDCCAPOWMINDBM   0x1B5
 *  19:12        CCAFALLTHRDBM   0xBF
 *     09             DISCCAEN   1
 *     08             SATCCAEN   1
 *  07:00        CCARISETHRDBM   0xC2
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCA1 register
#define RIU_RWNXAGCCCA1_ADDR   (REG_RIU_BASE_ADDR+0xB3AC)
/// Offset of the RWNXAGCCCA1 register from the base address
#define RIU_RWNXAGCCCA1_OFFSET 0x000003AC
/// Index of the RWNXAGCCCA1 register
#define RIU_RWNXAGCCCA1_INDEX  0x000000EB
/// Reset value of the RWNXAGCCCA1 register
#define RIU_RWNXAGCCCA1_RESET  0x1B5BF3C2

/**
 * @brief Returns the current value of the RWNXAGCCCA1 register.
 * The RWNXAGCCCA1 register will be read and its value returned.
 * @return The current value of the RWNXAGCCCA1 register.
 */
__INLINE uint32_t riu_rwnxagccca1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCA1 register to a value.
 * The RWNXAGCCCA1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccca1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, value);
}

// field definitions
/// INBDCCAPOWMINDBM field mask
#define RIU_INBDCCAPOWMINDBM_MASK   ((uint32_t)0x1FF00000)
/// INBDCCAPOWMINDBM field LSB position
#define RIU_INBDCCAPOWMINDBM_LSB    20
/// INBDCCAPOWMINDBM field width
#define RIU_INBDCCAPOWMINDBM_WIDTH  ((uint32_t)0x00000009)
/// CCAFALLTHRDBM field mask
#define RIU_CCAFALLTHRDBM_MASK      ((uint32_t)0x000FF000)
/// CCAFALLTHRDBM field LSB position
#define RIU_CCAFALLTHRDBM_LSB       12
/// CCAFALLTHRDBM field width
#define RIU_CCAFALLTHRDBM_WIDTH     ((uint32_t)0x00000008)
/// DISCCAEN field bit
#define RIU_DISCCAEN_BIT            ((uint32_t)0x00000200)
/// DISCCAEN field position
#define RIU_DISCCAEN_POS            9
/// SATCCAEN field bit
#define RIU_SATCCAEN_BIT            ((uint32_t)0x00000100)
/// SATCCAEN field position
#define RIU_SATCCAEN_POS            8
/// CCARISETHRDBM field mask
#define RIU_CCARISETHRDBM_MASK      ((uint32_t)0x000000FF)
/// CCARISETHRDBM field LSB position
#define RIU_CCARISETHRDBM_LSB       0
/// CCARISETHRDBM field width
#define RIU_CCARISETHRDBM_WIDTH     ((uint32_t)0x00000008)

/// INBDCCAPOWMINDBM field reset value
#define RIU_INBDCCAPOWMINDBM_RST    0x1B5
/// CCAFALLTHRDBM field reset value
#define RIU_CCAFALLTHRDBM_RST       0xBF
/// DISCCAEN field reset value
#define RIU_DISCCAEN_RST            0x1
/// SATCCAEN field reset value
#define RIU_SATCCAEN_RST            0x1
/// CCARISETHRDBM field reset value
#define RIU_CCARISETHRDBM_RST       0xC2

/**
 * @brief Constructs a value for the RWNXAGCCCA1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] inbdccapowmindbm - The value to use for the INBDCCAPOWMINDBM field.
 * @param[in] ccafallthrdbm - The value to use for the CCAFALLTHRDBM field.
 * @param[in] disccaen - The value to use for the DISCCAEN field.
 * @param[in] satccaen - The value to use for the SATCCAEN field.
 * @param[in] ccarisethrdbm - The value to use for the CCARISETHRDBM field.
 */
__INLINE void riu_rwnxagccca1_pack(uint16_t inbdccapowmindbm, uint8_t ccafallthrdbm, uint8_t disccaen, uint8_t satccaen, uint8_t ccarisethrdbm)
{
    ASSERT_ERR((((uint32_t)inbdccapowmindbm << 20) & ~((uint32_t)0x1FF00000)) == 0);
    ASSERT_ERR((((uint32_t)ccafallthrdbm << 12) & ~((uint32_t)0x000FF000)) == 0);
    ASSERT_ERR((((uint32_t)disccaen << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)satccaen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)ccarisethrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR,  ((uint32_t)inbdccapowmindbm << 20) | ((uint32_t)ccafallthrdbm << 12) | ((uint32_t)disccaen << 9) | ((uint32_t)satccaen << 8) | ((uint32_t)ccarisethrdbm << 0));
}

/**
 * @brief Unpacks RWNXAGCCCA1's fields from current value of the RWNXAGCCCA1 register.
 *
 * Reads the RWNXAGCCCA1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] inbdccapowmindbm - Will be populated with the current value of this field from the register.
 * @param[out] ccafallthrdbm - Will be populated with the current value of this field from the register.
 * @param[out] disccaen - Will be populated with the current value of this field from the register.
 * @param[out] satccaen - Will be populated with the current value of this field from the register.
 * @param[out] ccarisethrdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccca1_unpack(uint16_t* inbdccapowmindbm, uint8_t* ccafallthrdbm, uint8_t* disccaen, uint8_t* satccaen, uint8_t* ccarisethrdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);

    *inbdccapowmindbm = (localVal & ((uint32_t)0x1FF00000)) >> 20;
    *ccafallthrdbm = (localVal & ((uint32_t)0x000FF000)) >> 12;
    *disccaen = (localVal & ((uint32_t)0x00000200)) >> 9;
    *satccaen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *ccarisethrdbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the INBDCCAPOWMINDBM field in the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read and the INBDCCAPOWMINDBM field's value will be returned.
 *
 * @return The current value of the INBDCCAPOWMINDBM field in the RWNXAGCCCA1 register.
 */
__INLINE uint16_t riu_inbdccapowmindbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
    return ((localVal & ((uint32_t)0x1FF00000)) >> 20);
}

/**
 * @brief Sets the INBDCCAPOWMINDBM field of the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdccapowmindbm - The value to set the field to.
 */
__INLINE void riu_inbdccapowmindbm_setf(uint16_t inbdccapowmindbm)
{
    ASSERT_ERR((((uint32_t)inbdccapowmindbm << 20) & ~((uint32_t)0x1FF00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA1_ADDR) & ~((uint32_t)0x1FF00000)) | ((uint32_t)inbdccapowmindbm << 20));
}

/**
 * @brief Returns the current value of the CCAFALLTHRDBM field in the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read and the CCAFALLTHRDBM field's value will be returned.
 *
 * @return The current value of the CCAFALLTHRDBM field in the RWNXAGCCCA1 register.
 */
__INLINE uint8_t riu_ccafallthrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
    return ((localVal & ((uint32_t)0x000FF000)) >> 12);
}

/**
 * @brief Sets the CCAFALLTHRDBM field of the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccafallthrdbm - The value to set the field to.
 */
__INLINE void riu_ccafallthrdbm_setf(uint8_t ccafallthrdbm)
{
    ASSERT_ERR((((uint32_t)ccafallthrdbm << 12) & ~((uint32_t)0x000FF000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA1_ADDR) & ~((uint32_t)0x000FF000)) | ((uint32_t)ccafallthrdbm << 12));
}

/**
 * @brief Returns the current value of the DISCCAEN field in the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read and the DISCCAEN field's value will be returned.
 *
 * @return The current value of the DISCCAEN field in the RWNXAGCCCA1 register.
 */
__INLINE uint8_t riu_disccaen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the DISCCAEN field of the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] disccaen - The value to set the field to.
 */
__INLINE void riu_disccaen_setf(uint8_t disccaen)
{
    ASSERT_ERR((((uint32_t)disccaen << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA1_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)disccaen << 9));
}

/**
 * @brief Returns the current value of the SATCCAEN field in the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read and the SATCCAEN field's value will be returned.
 *
 * @return The current value of the SATCCAEN field in the RWNXAGCCCA1 register.
 */
__INLINE uint8_t riu_satccaen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the SATCCAEN field of the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] satccaen - The value to set the field to.
 */
__INLINE void riu_satccaen_setf(uint8_t satccaen)
{
    ASSERT_ERR((((uint32_t)satccaen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA1_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)satccaen << 8));
}

/**
 * @brief Returns the current value of the CCARISETHRDBM field in the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read and the CCARISETHRDBM field's value will be returned.
 *
 * @return The current value of the CCARISETHRDBM field in the RWNXAGCCCA1 register.
 */
__INLINE uint8_t riu_ccarisethrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCA1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the CCARISETHRDBM field of the RWNXAGCCCA1 register.
 *
 * The RWNXAGCCCA1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccarisethrdbm - The value to set the field to.
 */
__INLINE void riu_ccarisethrdbm_setf(uint8_t ccarisethrdbm)
{
    ASSERT_ERR((((uint32_t)ccarisethrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCA1_ADDR, (REG_PL_RD(RIU_RWNXAGCCCA1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)ccarisethrdbm << 0));
}

/// @}

/**
 * @name RWNXAGCCCACTRL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:28         CCAFLAG3CTRL   0xA
 *  27:24         CCAFLAG2CTRL   0xA
 *  23:20         CCAFLAG1CTRL   0xA
 *  19:16         CCAFLAG0CTRL   0xA
 *  15:14         ANTSELCCA40S   0x0
 *  13:12         ANTSELCCA20S   0x0
 *     09             CCADEMOD   1
 *     08              CCACSEN   1
 *  06:04          CCARAMPUDEN   0x7
 *  02:00          CCAENERGYEN   0x7
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCACTRL register
#define RIU_RWNXAGCCCACTRL_ADDR   (REG_RIU_BASE_ADDR+0xB3B0)
/// Offset of the RWNXAGCCCACTRL register from the base address
#define RIU_RWNXAGCCCACTRL_OFFSET 0x000003B0
/// Index of the RWNXAGCCCACTRL register
#define RIU_RWNXAGCCCACTRL_INDEX  0x000000EC
/// Reset value of the RWNXAGCCCACTRL register
#define RIU_RWNXAGCCCACTRL_RESET  0xAAAA0377

/**
 * @brief Returns the current value of the RWNXAGCCCACTRL register.
 * The RWNXAGCCCACTRL register will be read and its value returned.
 * @return The current value of the RWNXAGCCCACTRL register.
 */
__INLINE uint32_t riu_rwnxagcccactrl_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCACTRL register to a value.
 * The RWNXAGCCCACTRL register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcccactrl_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, value);
}

// field definitions
/// CCAFLAG3CTRL field mask
#define RIU_CCAFLAG3CTRL_MASK   ((uint32_t)0xF0000000)
/// CCAFLAG3CTRL field LSB position
#define RIU_CCAFLAG3CTRL_LSB    28
/// CCAFLAG3CTRL field width
#define RIU_CCAFLAG3CTRL_WIDTH  ((uint32_t)0x00000004)
/// CCAFLAG2CTRL field mask
#define RIU_CCAFLAG2CTRL_MASK   ((uint32_t)0x0F000000)
/// CCAFLAG2CTRL field LSB position
#define RIU_CCAFLAG2CTRL_LSB    24
/// CCAFLAG2CTRL field width
#define RIU_CCAFLAG2CTRL_WIDTH  ((uint32_t)0x00000004)
/// CCAFLAG1CTRL field mask
#define RIU_CCAFLAG1CTRL_MASK   ((uint32_t)0x00F00000)
/// CCAFLAG1CTRL field LSB position
#define RIU_CCAFLAG1CTRL_LSB    20
/// CCAFLAG1CTRL field width
#define RIU_CCAFLAG1CTRL_WIDTH  ((uint32_t)0x00000004)
/// CCAFLAG0CTRL field mask
#define RIU_CCAFLAG0CTRL_MASK   ((uint32_t)0x000F0000)
/// CCAFLAG0CTRL field LSB position
#define RIU_CCAFLAG0CTRL_LSB    16
/// CCAFLAG0CTRL field width
#define RIU_CCAFLAG0CTRL_WIDTH  ((uint32_t)0x00000004)
/// ANTSELCCA40S field mask
#define RIU_ANTSELCCA40S_MASK   ((uint32_t)0x0000C000)
/// ANTSELCCA40S field LSB position
#define RIU_ANTSELCCA40S_LSB    14
/// ANTSELCCA40S field width
#define RIU_ANTSELCCA40S_WIDTH  ((uint32_t)0x00000002)
/// ANTSELCCA20S field mask
#define RIU_ANTSELCCA20S_MASK   ((uint32_t)0x00003000)
/// ANTSELCCA20S field LSB position
#define RIU_ANTSELCCA20S_LSB    12
/// ANTSELCCA20S field width
#define RIU_ANTSELCCA20S_WIDTH  ((uint32_t)0x00000002)
/// CCADEMOD field bit
#define RIU_CCADEMOD_BIT        ((uint32_t)0x00000200)
/// CCADEMOD field position
#define RIU_CCADEMOD_POS        9
/// CCACSEN field bit
#define RIU_CCACSEN_BIT         ((uint32_t)0x00000100)
/// CCACSEN field position
#define RIU_CCACSEN_POS         8
/// CCARAMPUDEN field mask
#define RIU_CCARAMPUDEN_MASK    ((uint32_t)0x00000070)
/// CCARAMPUDEN field LSB position
#define RIU_CCARAMPUDEN_LSB     4
/// CCARAMPUDEN field width
#define RIU_CCARAMPUDEN_WIDTH   ((uint32_t)0x00000003)
/// CCAENERGYEN field mask
#define RIU_CCAENERGYEN_MASK    ((uint32_t)0x00000007)
/// CCAENERGYEN field LSB position
#define RIU_CCAENERGYEN_LSB     0
/// CCAENERGYEN field width
#define RIU_CCAENERGYEN_WIDTH   ((uint32_t)0x00000003)

/// CCAFLAG3CTRL field reset value
#define RIU_CCAFLAG3CTRL_RST    0xA
/// CCAFLAG2CTRL field reset value
#define RIU_CCAFLAG2CTRL_RST    0xA
/// CCAFLAG1CTRL field reset value
#define RIU_CCAFLAG1CTRL_RST    0xA
/// CCAFLAG0CTRL field reset value
#define RIU_CCAFLAG0CTRL_RST    0xA
/// ANTSELCCA40S field reset value
#define RIU_ANTSELCCA40S_RST    0x0
/// ANTSELCCA20S field reset value
#define RIU_ANTSELCCA20S_RST    0x0
/// CCADEMOD field reset value
#define RIU_CCADEMOD_RST        0x1
/// CCACSEN field reset value
#define RIU_CCACSEN_RST         0x1
/// CCARAMPUDEN field reset value
#define RIU_CCARAMPUDEN_RST     0x7
/// CCAENERGYEN field reset value
#define RIU_CCAENERGYEN_RST     0x7

/**
 * @brief Constructs a value for the RWNXAGCCCACTRL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] ccaflag3ctrl - The value to use for the CCAFLAG3CTRL field.
 * @param[in] ccaflag2ctrl - The value to use for the CCAFLAG2CTRL field.
 * @param[in] ccaflag1ctrl - The value to use for the CCAFLAG1CTRL field.
 * @param[in] ccaflag0ctrl - The value to use for the CCAFLAG0CTRL field.
 * @param[in] antselcca40s - The value to use for the ANTSELCCA40S field.
 * @param[in] antselcca20s - The value to use for the ANTSELCCA20S field.
 * @param[in] ccademod - The value to use for the CCADEMOD field.
 * @param[in] ccacsen - The value to use for the CCACSEN field.
 * @param[in] ccarampuden - The value to use for the CCARAMPUDEN field.
 * @param[in] ccaenergyen - The value to use for the CCAENERGYEN field.
 */
__INLINE void riu_rwnxagcccactrl_pack(uint8_t ccaflag3ctrl, uint8_t ccaflag2ctrl, uint8_t ccaflag1ctrl, uint8_t ccaflag0ctrl, uint8_t antselcca40s, uint8_t antselcca20s, uint8_t ccademod, uint8_t ccacsen, uint8_t ccarampuden, uint8_t ccaenergyen)
{
    ASSERT_ERR((((uint32_t)ccaflag3ctrl << 28) & ~((uint32_t)0xF0000000)) == 0);
    ASSERT_ERR((((uint32_t)ccaflag2ctrl << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)ccaflag1ctrl << 20) & ~((uint32_t)0x00F00000)) == 0);
    ASSERT_ERR((((uint32_t)ccaflag0ctrl << 16) & ~((uint32_t)0x000F0000)) == 0);
    ASSERT_ERR((((uint32_t)antselcca40s << 14) & ~((uint32_t)0x0000C000)) == 0);
    ASSERT_ERR((((uint32_t)antselcca20s << 12) & ~((uint32_t)0x00003000)) == 0);
    ASSERT_ERR((((uint32_t)ccademod << 9) & ~((uint32_t)0x00000200)) == 0);
    ASSERT_ERR((((uint32_t)ccacsen << 8) & ~((uint32_t)0x00000100)) == 0);
    ASSERT_ERR((((uint32_t)ccarampuden << 4) & ~((uint32_t)0x00000070)) == 0);
    ASSERT_ERR((((uint32_t)ccaenergyen << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR,  ((uint32_t)ccaflag3ctrl << 28) | ((uint32_t)ccaflag2ctrl << 24) | ((uint32_t)ccaflag1ctrl << 20) | ((uint32_t)ccaflag0ctrl << 16) | ((uint32_t)antselcca40s << 14) | ((uint32_t)antselcca20s << 12) | ((uint32_t)ccademod << 9) | ((uint32_t)ccacsen << 8) | ((uint32_t)ccarampuden << 4) | ((uint32_t)ccaenergyen << 0));
}

/**
 * @brief Unpacks RWNXAGCCCACTRL's fields from current value of the RWNXAGCCCACTRL register.
 *
 * Reads the RWNXAGCCCACTRL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] ccaflag3ctrl - Will be populated with the current value of this field from the register.
 * @param[out] ccaflag2ctrl - Will be populated with the current value of this field from the register.
 * @param[out] ccaflag1ctrl - Will be populated with the current value of this field from the register.
 * @param[out] ccaflag0ctrl - Will be populated with the current value of this field from the register.
 * @param[out] antselcca40s - Will be populated with the current value of this field from the register.
 * @param[out] antselcca20s - Will be populated with the current value of this field from the register.
 * @param[out] ccademod - Will be populated with the current value of this field from the register.
 * @param[out] ccacsen - Will be populated with the current value of this field from the register.
 * @param[out] ccarampuden - Will be populated with the current value of this field from the register.
 * @param[out] ccaenergyen - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcccactrl_unpack(uint8_t* ccaflag3ctrl, uint8_t* ccaflag2ctrl, uint8_t* ccaflag1ctrl, uint8_t* ccaflag0ctrl, uint8_t* antselcca40s, uint8_t* antselcca20s, uint8_t* ccademod, uint8_t* ccacsen, uint8_t* ccarampuden, uint8_t* ccaenergyen)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);

    *ccaflag3ctrl = (localVal & ((uint32_t)0xF0000000)) >> 28;
    *ccaflag2ctrl = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *ccaflag1ctrl = (localVal & ((uint32_t)0x00F00000)) >> 20;
    *ccaflag0ctrl = (localVal & ((uint32_t)0x000F0000)) >> 16;
    *antselcca40s = (localVal & ((uint32_t)0x0000C000)) >> 14;
    *antselcca20s = (localVal & ((uint32_t)0x00003000)) >> 12;
    *ccademod = (localVal & ((uint32_t)0x00000200)) >> 9;
    *ccacsen = (localVal & ((uint32_t)0x00000100)) >> 8;
    *ccarampuden = (localVal & ((uint32_t)0x00000070)) >> 4;
    *ccaenergyen = (localVal & ((uint32_t)0x00000007)) >> 0;
}

/**
 * @brief Returns the current value of the CCAFLAG3CTRL field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCAFLAG3CTRL field's value will be returned.
 *
 * @return The current value of the CCAFLAG3CTRL field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccaflag3ctrl_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0xF0000000)) >> 28);
}

/**
 * @brief Sets the CCAFLAG3CTRL field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccaflag3ctrl - The value to set the field to.
 */
__INLINE void riu_ccaflag3ctrl_setf(uint8_t ccaflag3ctrl)
{
    ASSERT_ERR((((uint32_t)ccaflag3ctrl << 28) & ~((uint32_t)0xF0000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0xF0000000)) | ((uint32_t)ccaflag3ctrl << 28));
}

/**
 * @brief Returns the current value of the CCAFLAG2CTRL field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCAFLAG2CTRL field's value will be returned.
 *
 * @return The current value of the CCAFLAG2CTRL field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccaflag2ctrl_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the CCAFLAG2CTRL field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccaflag2ctrl - The value to set the field to.
 */
__INLINE void riu_ccaflag2ctrl_setf(uint8_t ccaflag2ctrl)
{
    ASSERT_ERR((((uint32_t)ccaflag2ctrl << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)ccaflag2ctrl << 24));
}

/**
 * @brief Returns the current value of the CCAFLAG1CTRL field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCAFLAG1CTRL field's value will be returned.
 *
 * @return The current value of the CCAFLAG1CTRL field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccaflag1ctrl_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00F00000)) >> 20);
}

/**
 * @brief Sets the CCAFLAG1CTRL field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccaflag1ctrl - The value to set the field to.
 */
__INLINE void riu_ccaflag1ctrl_setf(uint8_t ccaflag1ctrl)
{
    ASSERT_ERR((((uint32_t)ccaflag1ctrl << 20) & ~((uint32_t)0x00F00000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00F00000)) | ((uint32_t)ccaflag1ctrl << 20));
}

/**
 * @brief Returns the current value of the CCAFLAG0CTRL field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCAFLAG0CTRL field's value will be returned.
 *
 * @return The current value of the CCAFLAG0CTRL field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccaflag0ctrl_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x000F0000)) >> 16);
}

/**
 * @brief Sets the CCAFLAG0CTRL field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccaflag0ctrl - The value to set the field to.
 */
__INLINE void riu_ccaflag0ctrl_setf(uint8_t ccaflag0ctrl)
{
    ASSERT_ERR((((uint32_t)ccaflag0ctrl << 16) & ~((uint32_t)0x000F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x000F0000)) | ((uint32_t)ccaflag0ctrl << 16));
}

/**
 * @brief Returns the current value of the ANTSELCCA40S field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the ANTSELCCA40S field's value will be returned.
 *
 * @return The current value of the ANTSELCCA40S field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_antselcca40s_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x0000C000)) >> 14);
}

/**
 * @brief Sets the ANTSELCCA40S field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] antselcca40s - The value to set the field to.
 */
__INLINE void riu_antselcca40s_setf(uint8_t antselcca40s)
{
    ASSERT_ERR((((uint32_t)antselcca40s << 14) & ~((uint32_t)0x0000C000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x0000C000)) | ((uint32_t)antselcca40s << 14));
}

/**
 * @brief Returns the current value of the ANTSELCCA20S field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the ANTSELCCA20S field's value will be returned.
 *
 * @return The current value of the ANTSELCCA20S field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_antselcca20s_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00003000)) >> 12);
}

/**
 * @brief Sets the ANTSELCCA20S field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] antselcca20s - The value to set the field to.
 */
__INLINE void riu_antselcca20s_setf(uint8_t antselcca20s)
{
    ASSERT_ERR((((uint32_t)antselcca20s << 12) & ~((uint32_t)0x00003000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00003000)) | ((uint32_t)antselcca20s << 12));
}

/**
 * @brief Returns the current value of the CCADEMOD field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCADEMOD field's value will be returned.
 *
 * @return The current value of the CCADEMOD field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccademod_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000200)) >> 9);
}

/**
 * @brief Sets the CCADEMOD field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccademod - The value to set the field to.
 */
__INLINE void riu_ccademod_setf(uint8_t ccademod)
{
    ASSERT_ERR((((uint32_t)ccademod << 9) & ~((uint32_t)0x00000200)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00000200)) | ((uint32_t)ccademod << 9));
}

/**
 * @brief Returns the current value of the CCACSEN field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCACSEN field's value will be returned.
 *
 * @return The current value of the CCACSEN field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccacsen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the CCACSEN field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccacsen - The value to set the field to.
 */
__INLINE void riu_ccacsen_setf(uint8_t ccacsen)
{
    ASSERT_ERR((((uint32_t)ccacsen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)ccacsen << 8));
}

/**
 * @brief Returns the current value of the CCARAMPUDEN field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCARAMPUDEN field's value will be returned.
 *
 * @return The current value of the CCARAMPUDEN field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccarampuden_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000070)) >> 4);
}

/**
 * @brief Sets the CCARAMPUDEN field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccarampuden - The value to set the field to.
 */
__INLINE void riu_ccarampuden_setf(uint8_t ccarampuden)
{
    ASSERT_ERR((((uint32_t)ccarampuden << 4) & ~((uint32_t)0x00000070)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00000070)) | ((uint32_t)ccarampuden << 4));
}

/**
 * @brief Returns the current value of the CCAENERGYEN field in the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read and the CCAENERGYEN field's value will be returned.
 *
 * @return The current value of the CCAENERGYEN field in the RWNXAGCCCACTRL register.
 */
__INLINE uint8_t riu_ccaenergyen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR);
    return ((localVal & ((uint32_t)0x00000007)) >> 0);
}

/**
 * @brief Sets the CCAENERGYEN field of the RWNXAGCCCACTRL register.
 *
 * The RWNXAGCCCACTRL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccaenergyen - The value to set the field to.
 */
__INLINE void riu_ccaenergyen_setf(uint8_t ccaenergyen)
{
    ASSERT_ERR((((uint32_t)ccaenergyen << 0) & ~((uint32_t)0x00000007)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCACTRL_ADDR, (REG_PL_RD(RIU_RWNXAGCCCACTRL_ADDR) & ~((uint32_t)0x00000007)) | ((uint32_t)ccaenergyen << 0));
}

/// @}

/**
 * @name RWNXAGCCCASTATE0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  26:16     RXSTATECCA20SSEL   0xD4
 *  10:00     RXSTATECCA20PSEL   0x6FE
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCASTATE0 register
#define RIU_RWNXAGCCCASTATE0_ADDR   (REG_RIU_BASE_ADDR+0xB3B4)
/// Offset of the RWNXAGCCCASTATE0 register from the base address
#define RIU_RWNXAGCCCASTATE0_OFFSET 0x000003B4
/// Index of the RWNXAGCCCASTATE0 register
#define RIU_RWNXAGCCCASTATE0_INDEX  0x000000ED
/// Reset value of the RWNXAGCCCASTATE0 register
#define RIU_RWNXAGCCCASTATE0_RESET  0x00D406FE

/**
 * @brief Returns the current value of the RWNXAGCCCASTATE0 register.
 * The RWNXAGCCCASTATE0 register will be read and its value returned.
 * @return The current value of the RWNXAGCCCASTATE0 register.
 */
__INLINE uint32_t riu_rwnxagcccastate0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCASTATE0 register to a value.
 * The RWNXAGCCCASTATE0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcccastate0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCASTATE0_ADDR, value);
}

// field definitions
/// RXSTATECCA20SSEL field mask
#define RIU_RXSTATECCA20SSEL_MASK   ((uint32_t)0x07FF0000)
/// RXSTATECCA20SSEL field LSB position
#define RIU_RXSTATECCA20SSEL_LSB    16
/// RXSTATECCA20SSEL field width
#define RIU_RXSTATECCA20SSEL_WIDTH  ((uint32_t)0x0000000B)
/// RXSTATECCA20PSEL field mask
#define RIU_RXSTATECCA20PSEL_MASK   ((uint32_t)0x000007FF)
/// RXSTATECCA20PSEL field LSB position
#define RIU_RXSTATECCA20PSEL_LSB    0
/// RXSTATECCA20PSEL field width
#define RIU_RXSTATECCA20PSEL_WIDTH  ((uint32_t)0x0000000B)

/// RXSTATECCA20SSEL field reset value
#define RIU_RXSTATECCA20SSEL_RST    0xD4
/// RXSTATECCA20PSEL field reset value
#define RIU_RXSTATECCA20PSEL_RST    0x6FE

/**
 * @brief Constructs a value for the RWNXAGCCCASTATE0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxstatecca20ssel - The value to use for the RXSTATECCA20SSEL field.
 * @param[in] rxstatecca20psel - The value to use for the RXSTATECCA20PSEL field.
 */
__INLINE void riu_rwnxagcccastate0_pack(uint16_t rxstatecca20ssel, uint16_t rxstatecca20psel)
{
    ASSERT_ERR((((uint32_t)rxstatecca20ssel << 16) & ~((uint32_t)0x07FF0000)) == 0);
    ASSERT_ERR((((uint32_t)rxstatecca20psel << 0) & ~((uint32_t)0x000007FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCASTATE0_ADDR,  ((uint32_t)rxstatecca20ssel << 16) | ((uint32_t)rxstatecca20psel << 0));
}

/**
 * @brief Unpacks RWNXAGCCCASTATE0's fields from current value of the RWNXAGCCCASTATE0 register.
 *
 * Reads the RWNXAGCCCASTATE0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxstatecca20ssel - Will be populated with the current value of this field from the register.
 * @param[out] rxstatecca20psel - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcccastate0_unpack(uint16_t* rxstatecca20ssel, uint16_t* rxstatecca20psel)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR);

    *rxstatecca20ssel = (localVal & ((uint32_t)0x07FF0000)) >> 16;
    *rxstatecca20psel = (localVal & ((uint32_t)0x000007FF)) >> 0;
}

/**
 * @brief Returns the current value of the RXSTATECCA20SSEL field in the RWNXAGCCCASTATE0 register.
 *
 * The RWNXAGCCCASTATE0 register will be read and the RXSTATECCA20SSEL field's value will be returned.
 *
 * @return The current value of the RXSTATECCA20SSEL field in the RWNXAGCCCASTATE0 register.
 */
__INLINE uint16_t riu_rxstatecca20ssel_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR);
    return ((localVal & ((uint32_t)0x07FF0000)) >> 16);
}

/**
 * @brief Sets the RXSTATECCA20SSEL field of the RWNXAGCCCASTATE0 register.
 *
 * The RWNXAGCCCASTATE0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxstatecca20ssel - The value to set the field to.
 */
__INLINE void riu_rxstatecca20ssel_setf(uint16_t rxstatecca20ssel)
{
    ASSERT_ERR((((uint32_t)rxstatecca20ssel << 16) & ~((uint32_t)0x07FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCASTATE0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR) & ~((uint32_t)0x07FF0000)) | ((uint32_t)rxstatecca20ssel << 16));
}

/**
 * @brief Returns the current value of the RXSTATECCA20PSEL field in the RWNXAGCCCASTATE0 register.
 *
 * The RWNXAGCCCASTATE0 register will be read and the RXSTATECCA20PSEL field's value will be returned.
 *
 * @return The current value of the RXSTATECCA20PSEL field in the RWNXAGCCCASTATE0 register.
 */
__INLINE uint16_t riu_rxstatecca20psel_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR);
    return ((localVal & ((uint32_t)0x000007FF)) >> 0);
}

/**
 * @brief Sets the RXSTATECCA20PSEL field of the RWNXAGCCCASTATE0 register.
 *
 * The RWNXAGCCCASTATE0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxstatecca20psel - The value to set the field to.
 */
__INLINE void riu_rxstatecca20psel_setf(uint16_t rxstatecca20psel)
{
    ASSERT_ERR((((uint32_t)rxstatecca20psel << 0) & ~((uint32_t)0x000007FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCASTATE0_ADDR, (REG_PL_RD(RIU_RWNXAGCCCASTATE0_ADDR) & ~((uint32_t)0x000007FF)) | ((uint32_t)rxstatecca20psel << 0));
}

/// @}

/**
 * @name RWNXAGCCCASTATE1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  10:00     RXSTATECCA40SSEL   0x80
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCASTATE1 register
#define RIU_RWNXAGCCCASTATE1_ADDR   (REG_RIU_BASE_ADDR+0xB3B8)
/// Offset of the RWNXAGCCCASTATE1 register from the base address
#define RIU_RWNXAGCCCASTATE1_OFFSET 0x000003B8
/// Index of the RWNXAGCCCASTATE1 register
#define RIU_RWNXAGCCCASTATE1_INDEX  0x000000EE
/// Reset value of the RWNXAGCCCASTATE1 register
#define RIU_RWNXAGCCCASTATE1_RESET  0x00000080

/**
 * @brief Returns the current value of the RWNXAGCCCASTATE1 register.
 * The RWNXAGCCCASTATE1 register will be read and its value returned.
 * @return The current value of the RWNXAGCCCASTATE1 register.
 */
__INLINE uint32_t riu_rwnxagcccastate1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCASTATE1_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCASTATE1 register to a value.
 * The RWNXAGCCCASTATE1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcccastate1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCASTATE1_ADDR, value);
}

// field definitions
/// RXSTATECCA40SSEL field mask
#define RIU_RXSTATECCA40SSEL_MASK   ((uint32_t)0x000007FF)
/// RXSTATECCA40SSEL field LSB position
#define RIU_RXSTATECCA40SSEL_LSB    0
/// RXSTATECCA40SSEL field width
#define RIU_RXSTATECCA40SSEL_WIDTH  ((uint32_t)0x0000000B)

/// RXSTATECCA40SSEL field reset value
#define RIU_RXSTATECCA40SSEL_RST    0x80

/**
 * @brief Returns the current value of the RXSTATECCA40SSEL field in the RWNXAGCCCASTATE1 register.
 *
 * The RWNXAGCCCASTATE1 register will be read and the RXSTATECCA40SSEL field's value will be returned.
 *
 * @return The current value of the RXSTATECCA40SSEL field in the RWNXAGCCCASTATE1 register.
 */
__INLINE uint16_t riu_rxstatecca40ssel_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCASTATE1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000007FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the RXSTATECCA40SSEL field of the RWNXAGCCCASTATE1 register.
 *
 * The RWNXAGCCCASTATE1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxstatecca40ssel - The value to set the field to.
 */
__INLINE void riu_rxstatecca40ssel_setf(uint16_t rxstatecca40ssel)
{
    ASSERT_ERR((((uint32_t)rxstatecca40ssel << 0) & ~((uint32_t)0x000007FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCASTATE1_ADDR, (uint32_t)rxstatecca40ssel << 0);
}

/// @}

/**
 * @name RWNXAGCCCATIMEOUT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:00           CCATIMEOUT   0x88B80
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCCATIMEOUT register
#define RIU_RWNXAGCCCATIMEOUT_ADDR   (REG_RIU_BASE_ADDR+0xB3BC)
/// Offset of the RWNXAGCCCATIMEOUT register from the base address
#define RIU_RWNXAGCCCATIMEOUT_OFFSET 0x000003BC
/// Index of the RWNXAGCCCATIMEOUT register
#define RIU_RWNXAGCCCATIMEOUT_INDEX  0x000000EF
/// Reset value of the RWNXAGCCCATIMEOUT register
#define RIU_RWNXAGCCCATIMEOUT_RESET  0x00088B80

/**
 * @brief Returns the current value of the RWNXAGCCCATIMEOUT register.
 * The RWNXAGCCCATIMEOUT register will be read and its value returned.
 * @return The current value of the RWNXAGCCCATIMEOUT register.
 */
__INLINE uint32_t riu_rwnxagcccatimeout_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCCATIMEOUT_ADDR);
}

/**
 * @brief Sets the RWNXAGCCCATIMEOUT register to a value.
 * The RWNXAGCCCATIMEOUT register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcccatimeout_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCCATIMEOUT_ADDR, value);
}

// field definitions
/// CCATIMEOUT field mask
#define RIU_CCATIMEOUT_MASK   ((uint32_t)0xFFFFFFFF)
/// CCATIMEOUT field LSB position
#define RIU_CCATIMEOUT_LSB    0
/// CCATIMEOUT field width
#define RIU_CCATIMEOUT_WIDTH  ((uint32_t)0x00000020)

/// CCATIMEOUT field reset value
#define RIU_CCATIMEOUT_RST    0x88B80

/**
 * @brief Returns the current value of the CCATIMEOUT field in the RWNXAGCCCATIMEOUT register.
 *
 * The RWNXAGCCCATIMEOUT register will be read and the CCATIMEOUT field's value will be returned.
 *
 * @return The current value of the CCATIMEOUT field in the RWNXAGCCCATIMEOUT register.
 */
__INLINE uint32_t riu_ccatimeout_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCCATIMEOUT_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0xFFFFFFFF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the CCATIMEOUT field of the RWNXAGCCCATIMEOUT register.
 *
 * The RWNXAGCCCATIMEOUT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] ccatimeout - The value to set the field to.
 */
__INLINE void riu_ccatimeout_setf(uint32_t ccatimeout)
{
    ASSERT_ERR((((uint32_t)ccatimeout << 0) & ~((uint32_t)0xFFFFFFFF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCCATIMEOUT_ADDR, (uint32_t)ccatimeout << 0);
}

/// @}

/**
 * @name RWNXAGCCOMP0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24   IDINBDPOWGAPDNQDBM   0x28
 *  23:16   IDINBDPOWGAPUPQDBM   0x28
 *  15:08     INBDPOWINFTHRDBM   0xB0
 *  07:00     INBDPOWSUPTHRDBM   0xC4
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCOMP0 register
#define RIU_RWNXAGCCOMP0_ADDR   (REG_RIU_BASE_ADDR+0xB3C0)
/// Offset of the RWNXAGCCOMP0 register from the base address
#define RIU_RWNXAGCCOMP0_OFFSET 0x000003C0
/// Index of the RWNXAGCCOMP0 register
#define RIU_RWNXAGCCOMP0_INDEX  0x000000F0
/// Reset value of the RWNXAGCCOMP0 register
#define RIU_RWNXAGCCOMP0_RESET  0x2828B0C4

/**
 * @brief Returns the current value of the RWNXAGCCOMP0 register.
 * The RWNXAGCCOMP0 register will be read and its value returned.
 * @return The current value of the RWNXAGCCOMP0 register.
 */
__INLINE uint32_t riu_rwnxagccomp0_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);
}

/**
 * @brief Sets the RWNXAGCCOMP0 register to a value.
 * The RWNXAGCCOMP0 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccomp0_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR, value);
}

// field definitions
/// IDINBDPOWGAPDNQDBM field mask
#define RIU_IDINBDPOWGAPDNQDBM_MASK   ((uint32_t)0xFF000000)
/// IDINBDPOWGAPDNQDBM field LSB position
#define RIU_IDINBDPOWGAPDNQDBM_LSB    24
/// IDINBDPOWGAPDNQDBM field width
#define RIU_IDINBDPOWGAPDNQDBM_WIDTH  ((uint32_t)0x00000008)
/// IDINBDPOWGAPUPQDBM field mask
#define RIU_IDINBDPOWGAPUPQDBM_MASK   ((uint32_t)0x00FF0000)
/// IDINBDPOWGAPUPQDBM field LSB position
#define RIU_IDINBDPOWGAPUPQDBM_LSB    16
/// IDINBDPOWGAPUPQDBM field width
#define RIU_IDINBDPOWGAPUPQDBM_WIDTH  ((uint32_t)0x00000008)
/// INBDPOWINFTHRDBM field mask
#define RIU_INBDPOWINFTHRDBM_MASK     ((uint32_t)0x0000FF00)
/// INBDPOWINFTHRDBM field LSB position
#define RIU_INBDPOWINFTHRDBM_LSB      8
/// INBDPOWINFTHRDBM field width
#define RIU_INBDPOWINFTHRDBM_WIDTH    ((uint32_t)0x00000008)
/// INBDPOWSUPTHRDBM field mask
#define RIU_INBDPOWSUPTHRDBM_MASK     ((uint32_t)0x000000FF)
/// INBDPOWSUPTHRDBM field LSB position
#define RIU_INBDPOWSUPTHRDBM_LSB      0
/// INBDPOWSUPTHRDBM field width
#define RIU_INBDPOWSUPTHRDBM_WIDTH    ((uint32_t)0x00000008)

/// IDINBDPOWGAPDNQDBM field reset value
#define RIU_IDINBDPOWGAPDNQDBM_RST    0x28
/// IDINBDPOWGAPUPQDBM field reset value
#define RIU_IDINBDPOWGAPUPQDBM_RST    0x28
/// INBDPOWINFTHRDBM field reset value
#define RIU_INBDPOWINFTHRDBM_RST      0xB0
/// INBDPOWSUPTHRDBM field reset value
#define RIU_INBDPOWSUPTHRDBM_RST      0xC4

/**
 * @brief Constructs a value for the RWNXAGCCOMP0 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] idinbdpowgapdnqdbm - The value to use for the IDINBDPOWGAPDNQDBM field.
 * @param[in] idinbdpowgapupqdbm - The value to use for the IDINBDPOWGAPUPQDBM field.
 * @param[in] inbdpowinfthrdbm - The value to use for the INBDPOWINFTHRDBM field.
 * @param[in] inbdpowsupthrdbm - The value to use for the INBDPOWSUPTHRDBM field.
 */
__INLINE void riu_rwnxagccomp0_pack(uint8_t idinbdpowgapdnqdbm, uint8_t idinbdpowgapupqdbm, uint8_t inbdpowinfthrdbm, uint8_t inbdpowsupthrdbm)
{
    ASSERT_ERR((((uint32_t)idinbdpowgapdnqdbm << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)idinbdpowgapupqdbm << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)inbdpowinfthrdbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)inbdpowsupthrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR,  ((uint32_t)idinbdpowgapdnqdbm << 24) | ((uint32_t)idinbdpowgapupqdbm << 16) | ((uint32_t)inbdpowinfthrdbm << 8) | ((uint32_t)inbdpowsupthrdbm << 0));
}

/**
 * @brief Unpacks RWNXAGCCOMP0's fields from current value of the RWNXAGCCOMP0 register.
 *
 * Reads the RWNXAGCCOMP0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] idinbdpowgapdnqdbm - Will be populated with the current value of this field from the register.
 * @param[out] idinbdpowgapupqdbm - Will be populated with the current value of this field from the register.
 * @param[out] inbdpowinfthrdbm - Will be populated with the current value of this field from the register.
 * @param[out] inbdpowsupthrdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccomp0_unpack(uint8_t* idinbdpowgapdnqdbm, uint8_t* idinbdpowgapupqdbm, uint8_t* inbdpowinfthrdbm, uint8_t* inbdpowsupthrdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);

    *idinbdpowgapdnqdbm = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *idinbdpowgapupqdbm = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *inbdpowinfthrdbm = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *inbdpowsupthrdbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the IDINBDPOWGAPDNQDBM field in the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read and the IDINBDPOWGAPDNQDBM field's value will be returned.
 *
 * @return The current value of the IDINBDPOWGAPDNQDBM field in the RWNXAGCCOMP0 register.
 */
__INLINE uint8_t riu_idinbdpowgapdnqdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the IDINBDPOWGAPDNQDBM field of the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] idinbdpowgapdnqdbm - The value to set the field to.
 */
__INLINE void riu_idinbdpowgapdnqdbm_setf(uint8_t idinbdpowgapdnqdbm)
{
    ASSERT_ERR((((uint32_t)idinbdpowgapdnqdbm << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)idinbdpowgapdnqdbm << 24));
}

/**
 * @brief Returns the current value of the IDINBDPOWGAPUPQDBM field in the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read and the IDINBDPOWGAPUPQDBM field's value will be returned.
 *
 * @return The current value of the IDINBDPOWGAPUPQDBM field in the RWNXAGCCOMP0 register.
 */
__INLINE uint8_t riu_idinbdpowgapupqdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the IDINBDPOWGAPUPQDBM field of the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] idinbdpowgapupqdbm - The value to set the field to.
 */
__INLINE void riu_idinbdpowgapupqdbm_setf(uint8_t idinbdpowgapupqdbm)
{
    ASSERT_ERR((((uint32_t)idinbdpowgapupqdbm << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)idinbdpowgapupqdbm << 16));
}

/**
 * @brief Returns the current value of the INBDPOWINFTHRDBM field in the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read and the INBDPOWINFTHRDBM field's value will be returned.
 *
 * @return The current value of the INBDPOWINFTHRDBM field in the RWNXAGCCOMP0 register.
 */
__INLINE uint8_t riu_inbdpowinfthrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the INBDPOWINFTHRDBM field of the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpowinfthrdbm - The value to set the field to.
 */
__INLINE void riu_inbdpowinfthrdbm_setf(uint8_t inbdpowinfthrdbm)
{
    ASSERT_ERR((((uint32_t)inbdpowinfthrdbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)inbdpowinfthrdbm << 8));
}

/**
 * @brief Returns the current value of the INBDPOWSUPTHRDBM field in the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read and the INBDPOWSUPTHRDBM field's value will be returned.
 *
 * @return The current value of the INBDPOWSUPTHRDBM field in the RWNXAGCCOMP0 register.
 */
__INLINE uint8_t riu_inbdpowsupthrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the INBDPOWSUPTHRDBM field of the RWNXAGCCOMP0 register.
 *
 * The RWNXAGCCOMP0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] inbdpowsupthrdbm - The value to set the field to.
 */
__INLINE void riu_inbdpowsupthrdbm_setf(uint8_t inbdpowsupthrdbm)
{
    ASSERT_ERR((((uint32_t)inbdpowsupthrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP0_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP0_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)inbdpowsupthrdbm << 0));
}

/// @}

/**
 * @name RWNXAGCCOMP1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24      ADCPOWINFTHRDBV   0xDD
 *  23:16      ADCPOWSUPTHRDBV   0xDB
 *  15:08      ADCPOWINFTHRDBM   0xBF
 *  07:00      ADCPOWSUPTHRDBM   0xBE
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCCOMP1 register
#define RIU_RWNXAGCCOMP1_ADDR   (REG_RIU_BASE_ADDR+0xB3C4)
/// Offset of the RWNXAGCCOMP1 register from the base address
#define RIU_RWNXAGCCOMP1_OFFSET 0x000003C4
/// Index of the RWNXAGCCOMP1 register
#define RIU_RWNXAGCCOMP1_INDEX  0x000000F1
/// Reset value of the RWNXAGCCOMP1 register
#define RIU_RWNXAGCCOMP1_RESET  0xDDDBBFBE

/**
 * @brief Returns the current value of the RWNXAGCCOMP1 register.
 * The RWNXAGCCOMP1 register will be read and its value returned.
 * @return The current value of the RWNXAGCCOMP1 register.
 */
__INLINE uint32_t riu_rwnxagccomp1_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);
}

/**
 * @brief Sets the RWNXAGCCOMP1 register to a value.
 * The RWNXAGCCOMP1 register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagccomp1_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR, value);
}

// field definitions
/// ADCPOWINFTHRDBV field mask
#define RIU_ADCPOWINFTHRDBV_MASK   ((uint32_t)0xFF000000)
/// ADCPOWINFTHRDBV field LSB position
#define RIU_ADCPOWINFTHRDBV_LSB    24
/// ADCPOWINFTHRDBV field width
#define RIU_ADCPOWINFTHRDBV_WIDTH  ((uint32_t)0x00000008)
/// ADCPOWSUPTHRDBV field mask
#define RIU_ADCPOWSUPTHRDBV_MASK   ((uint32_t)0x00FF0000)
/// ADCPOWSUPTHRDBV field LSB position
#define RIU_ADCPOWSUPTHRDBV_LSB    16
/// ADCPOWSUPTHRDBV field width
#define RIU_ADCPOWSUPTHRDBV_WIDTH  ((uint32_t)0x00000008)
/// ADCPOWINFTHRDBM field mask
#define RIU_ADCPOWINFTHRDBM_MASK   ((uint32_t)0x0000FF00)
/// ADCPOWINFTHRDBM field LSB position
#define RIU_ADCPOWINFTHRDBM_LSB    8
/// ADCPOWINFTHRDBM field width
#define RIU_ADCPOWINFTHRDBM_WIDTH  ((uint32_t)0x00000008)
/// ADCPOWSUPTHRDBM field mask
#define RIU_ADCPOWSUPTHRDBM_MASK   ((uint32_t)0x000000FF)
/// ADCPOWSUPTHRDBM field LSB position
#define RIU_ADCPOWSUPTHRDBM_LSB    0
/// ADCPOWSUPTHRDBM field width
#define RIU_ADCPOWSUPTHRDBM_WIDTH  ((uint32_t)0x00000008)

/// ADCPOWINFTHRDBV field reset value
#define RIU_ADCPOWINFTHRDBV_RST    0xDD
/// ADCPOWSUPTHRDBV field reset value
#define RIU_ADCPOWSUPTHRDBV_RST    0xDB
/// ADCPOWINFTHRDBM field reset value
#define RIU_ADCPOWINFTHRDBM_RST    0xBF
/// ADCPOWSUPTHRDBM field reset value
#define RIU_ADCPOWSUPTHRDBM_RST    0xBE

/**
 * @brief Constructs a value for the RWNXAGCCOMP1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] adcpowinfthrdbv - The value to use for the ADCPOWINFTHRDBV field.
 * @param[in] adcpowsupthrdbv - The value to use for the ADCPOWSUPTHRDBV field.
 * @param[in] adcpowinfthrdbm - The value to use for the ADCPOWINFTHRDBM field.
 * @param[in] adcpowsupthrdbm - The value to use for the ADCPOWSUPTHRDBM field.
 */
__INLINE void riu_rwnxagccomp1_pack(uint8_t adcpowinfthrdbv, uint8_t adcpowsupthrdbv, uint8_t adcpowinfthrdbm, uint8_t adcpowsupthrdbm)
{
    ASSERT_ERR((((uint32_t)adcpowinfthrdbv << 24) & ~((uint32_t)0xFF000000)) == 0);
    ASSERT_ERR((((uint32_t)adcpowsupthrdbv << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)adcpowinfthrdbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    ASSERT_ERR((((uint32_t)adcpowsupthrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR,  ((uint32_t)adcpowinfthrdbv << 24) | ((uint32_t)adcpowsupthrdbv << 16) | ((uint32_t)adcpowinfthrdbm << 8) | ((uint32_t)adcpowsupthrdbm << 0));
}

/**
 * @brief Unpacks RWNXAGCCOMP1's fields from current value of the RWNXAGCCOMP1 register.
 *
 * Reads the RWNXAGCCOMP1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] adcpowinfthrdbv - Will be populated with the current value of this field from the register.
 * @param[out] adcpowsupthrdbv - Will be populated with the current value of this field from the register.
 * @param[out] adcpowinfthrdbm - Will be populated with the current value of this field from the register.
 * @param[out] adcpowsupthrdbm - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagccomp1_unpack(uint8_t* adcpowinfthrdbv, uint8_t* adcpowsupthrdbv, uint8_t* adcpowinfthrdbm, uint8_t* adcpowsupthrdbm)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);

    *adcpowinfthrdbv = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *adcpowsupthrdbv = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *adcpowinfthrdbm = (localVal & ((uint32_t)0x0000FF00)) >> 8;
    *adcpowsupthrdbm = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the ADCPOWINFTHRDBV field in the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read and the ADCPOWINFTHRDBV field's value will be returned.
 *
 * @return The current value of the ADCPOWINFTHRDBV field in the RWNXAGCCOMP1 register.
 */
__INLINE uint8_t riu_adcpowinfthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Sets the ADCPOWINFTHRDBV field of the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowinfthrdbv - The value to set the field to.
 */
__INLINE void riu_adcpowinfthrdbv_setf(uint8_t adcpowinfthrdbv)
{
    ASSERT_ERR((((uint32_t)adcpowinfthrdbv << 24) & ~((uint32_t)0xFF000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR) & ~((uint32_t)0xFF000000)) | ((uint32_t)adcpowinfthrdbv << 24));
}

/**
 * @brief Returns the current value of the ADCPOWSUPTHRDBV field in the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read and the ADCPOWSUPTHRDBV field's value will be returned.
 *
 * @return The current value of the ADCPOWSUPTHRDBV field in the RWNXAGCCOMP1 register.
 */
__INLINE uint8_t riu_adcpowsupthrdbv_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the ADCPOWSUPTHRDBV field of the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowsupthrdbv - The value to set the field to.
 */
__INLINE void riu_adcpowsupthrdbv_setf(uint8_t adcpowsupthrdbv)
{
    ASSERT_ERR((((uint32_t)adcpowsupthrdbv << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)adcpowsupthrdbv << 16));
}

/**
 * @brief Returns the current value of the ADCPOWINFTHRDBM field in the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read and the ADCPOWINFTHRDBM field's value will be returned.
 *
 * @return The current value of the ADCPOWINFTHRDBM field in the RWNXAGCCOMP1 register.
 */
__INLINE uint8_t riu_adcpowinfthrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x0000FF00)) >> 8);
}

/**
 * @brief Sets the ADCPOWINFTHRDBM field of the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowinfthrdbm - The value to set the field to.
 */
__INLINE void riu_adcpowinfthrdbm_setf(uint8_t adcpowinfthrdbm)
{
    ASSERT_ERR((((uint32_t)adcpowinfthrdbm << 8) & ~((uint32_t)0x0000FF00)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR) & ~((uint32_t)0x0000FF00)) | ((uint32_t)adcpowinfthrdbm << 8));
}

/**
 * @brief Returns the current value of the ADCPOWSUPTHRDBM field in the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read and the ADCPOWSUPTHRDBM field's value will be returned.
 *
 * @return The current value of the ADCPOWSUPTHRDBM field in the RWNXAGCCOMP1 register.
 */
__INLINE uint8_t riu_adcpowsupthrdbm_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the ADCPOWSUPTHRDBM field of the RWNXAGCCOMP1 register.
 *
 * The RWNXAGCCOMP1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] adcpowsupthrdbm - The value to set the field to.
 */
__INLINE void riu_adcpowsupthrdbm_setf(uint8_t adcpowsupthrdbm)
{
    ASSERT_ERR((((uint32_t)adcpowsupthrdbm << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCCOMP1_ADDR, (REG_PL_RD(RIU_RWNXAGCCOMP1_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)adcpowsupthrdbm << 0));
}

/// @}

#if RW_RADAR_EN
/**
 * @name RWNXAGCRADAR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:28          ANTSELRADAR   0x0
 *  27:24          FREQSINGGAP   0x0
 *  23:16             FREQSING   0x0
 *  11:08           FOMSINGGAP   0x0
 *  07:00              FOMSING   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCRADAR register
#define RIU_RWNXAGCRADAR_ADDR   (REG_RIU_BASE_ADDR+0xB3C8)
/// Offset of the RWNXAGCRADAR register from the base address
#define RIU_RWNXAGCRADAR_OFFSET 0x000003C8
/// Index of the RWNXAGCRADAR register
#define RIU_RWNXAGCRADAR_INDEX  0x000000F2
/// Reset value of the RWNXAGCRADAR register
#define RIU_RWNXAGCRADAR_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXAGCRADAR register.
 * The RWNXAGCRADAR register will be read and its value returned.
 * @return The current value of the RWNXAGCRADAR register.
 */
__INLINE uint32_t riu_rwnxagcradar_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
}

/**
 * @brief Sets the RWNXAGCRADAR register to a value.
 * The RWNXAGCRADAR register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcradar_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, value);
}

// field definitions
/// ANTSELRADAR field mask
#define RIU_ANTSELRADAR_MASK   ((uint32_t)0x30000000)
/// ANTSELRADAR field LSB position
#define RIU_ANTSELRADAR_LSB    28
/// ANTSELRADAR field width
#define RIU_ANTSELRADAR_WIDTH  ((uint32_t)0x00000002)
/// FREQSINGGAP field mask
#define RIU_FREQSINGGAP_MASK   ((uint32_t)0x0F000000)
/// FREQSINGGAP field LSB position
#define RIU_FREQSINGGAP_LSB    24
/// FREQSINGGAP field width
#define RIU_FREQSINGGAP_WIDTH  ((uint32_t)0x00000004)
/// FREQSING field mask
#define RIU_FREQSING_MASK      ((uint32_t)0x00FF0000)
/// FREQSING field LSB position
#define RIU_FREQSING_LSB       16
/// FREQSING field width
#define RIU_FREQSING_WIDTH     ((uint32_t)0x00000008)
/// FOMSINGGAP field mask
#define RIU_FOMSINGGAP_MASK    ((uint32_t)0x00000F00)
/// FOMSINGGAP field LSB position
#define RIU_FOMSINGGAP_LSB     8
/// FOMSINGGAP field width
#define RIU_FOMSINGGAP_WIDTH   ((uint32_t)0x00000004)
/// FOMSING field mask
#define RIU_FOMSING_MASK       ((uint32_t)0x000000FF)
/// FOMSING field LSB position
#define RIU_FOMSING_LSB        0
/// FOMSING field width
#define RIU_FOMSING_WIDTH      ((uint32_t)0x00000008)

/// ANTSELRADAR field reset value
#define RIU_ANTSELRADAR_RST    0x0
/// FREQSINGGAP field reset value
#define RIU_FREQSINGGAP_RST    0x0
/// FREQSING field reset value
#define RIU_FREQSING_RST       0x0
/// FOMSINGGAP field reset value
#define RIU_FOMSINGGAP_RST     0x0
/// FOMSING field reset value
#define RIU_FOMSING_RST        0x0

/**
 * @brief Constructs a value for the RWNXAGCRADAR register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] antselradar - The value to use for the ANTSELRADAR field.
 * @param[in] freqsinggap - The value to use for the FREQSINGGAP field.
 * @param[in] freqsing - The value to use for the FREQSING field.
 * @param[in] fomsinggap - The value to use for the FOMSINGGAP field.
 * @param[in] fomsing - The value to use for the FOMSING field.
 */
__INLINE void riu_rwnxagcradar_pack(uint8_t antselradar, uint8_t freqsinggap, uint8_t freqsing, uint8_t fomsinggap, uint8_t fomsing)
{
    ASSERT_ERR((((uint32_t)antselradar << 28) & ~((uint32_t)0x30000000)) == 0);
    ASSERT_ERR((((uint32_t)freqsinggap << 24) & ~((uint32_t)0x0F000000)) == 0);
    ASSERT_ERR((((uint32_t)freqsing << 16) & ~((uint32_t)0x00FF0000)) == 0);
    ASSERT_ERR((((uint32_t)fomsinggap << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)fomsing << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR,  ((uint32_t)antselradar << 28) | ((uint32_t)freqsinggap << 24) | ((uint32_t)freqsing << 16) | ((uint32_t)fomsinggap << 8) | ((uint32_t)fomsing << 0));
}

/**
 * @brief Unpacks RWNXAGCRADAR's fields from current value of the RWNXAGCRADAR register.
 *
 * Reads the RWNXAGCRADAR register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] antselradar - Will be populated with the current value of this field from the register.
 * @param[out] freqsinggap - Will be populated with the current value of this field from the register.
 * @param[out] freqsing - Will be populated with the current value of this field from the register.
 * @param[out] fomsinggap - Will be populated with the current value of this field from the register.
 * @param[out] fomsing - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcradar_unpack(uint8_t* antselradar, uint8_t* freqsinggap, uint8_t* freqsing, uint8_t* fomsinggap, uint8_t* fomsing)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);

    *antselradar = (localVal & ((uint32_t)0x30000000)) >> 28;
    *freqsinggap = (localVal & ((uint32_t)0x0F000000)) >> 24;
    *freqsing = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *fomsinggap = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *fomsing = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the ANTSELRADAR field in the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read and the ANTSELRADAR field's value will be returned.
 *
 * @return The current value of the ANTSELRADAR field in the RWNXAGCRADAR register.
 */
__INLINE uint8_t riu_antselradar_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
    return ((localVal & ((uint32_t)0x30000000)) >> 28);
}

/**
 * @brief Sets the ANTSELRADAR field of the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] antselradar - The value to set the field to.
 */
__INLINE void riu_antselradar_setf(uint8_t antselradar)
{
    ASSERT_ERR((((uint32_t)antselradar << 28) & ~((uint32_t)0x30000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCRADAR_ADDR) & ~((uint32_t)0x30000000)) | ((uint32_t)antselradar << 28));
}

/**
 * @brief Returns the current value of the FREQSINGGAP field in the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read and the FREQSINGGAP field's value will be returned.
 *
 * @return The current value of the FREQSINGGAP field in the RWNXAGCRADAR register.
 */
__INLINE uint8_t riu_freqsinggap_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
    return ((localVal & ((uint32_t)0x0F000000)) >> 24);
}

/**
 * @brief Sets the FREQSINGGAP field of the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] freqsinggap - The value to set the field to.
 */
__INLINE void riu_freqsinggap_setf(uint8_t freqsinggap)
{
    ASSERT_ERR((((uint32_t)freqsinggap << 24) & ~((uint32_t)0x0F000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCRADAR_ADDR) & ~((uint32_t)0x0F000000)) | ((uint32_t)freqsinggap << 24));
}

/**
 * @brief Returns the current value of the FREQSING field in the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read and the FREQSING field's value will be returned.
 *
 * @return The current value of the FREQSING field in the RWNXAGCRADAR register.
 */
__INLINE uint8_t riu_freqsing_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Sets the FREQSING field of the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] freqsing - The value to set the field to.
 */
__INLINE void riu_freqsing_setf(uint8_t freqsing)
{
    ASSERT_ERR((((uint32_t)freqsing << 16) & ~((uint32_t)0x00FF0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCRADAR_ADDR) & ~((uint32_t)0x00FF0000)) | ((uint32_t)freqsing << 16));
}

/**
 * @brief Returns the current value of the FOMSINGGAP field in the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read and the FOMSINGGAP field's value will be returned.
 *
 * @return The current value of the FOMSINGGAP field in the RWNXAGCRADAR register.
 */
__INLINE uint8_t riu_fomsinggap_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the FOMSINGGAP field of the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fomsinggap - The value to set the field to.
 */
__INLINE void riu_fomsinggap_setf(uint8_t fomsinggap)
{
    ASSERT_ERR((((uint32_t)fomsinggap << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCRADAR_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)fomsinggap << 8));
}

/**
 * @brief Returns the current value of the FOMSING field in the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read and the FOMSING field's value will be returned.
 *
 * @return The current value of the FOMSING field in the RWNXAGCRADAR register.
 */
__INLINE uint8_t riu_fomsing_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADAR_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the FOMSING field of the RWNXAGCRADAR register.
 *
 * The RWNXAGCRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fomsing - The value to set the field to.
 */
__INLINE void riu_fomsing_setf(uint8_t fomsing)
{
    ASSERT_ERR((((uint32_t)fomsing << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCRADAR_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)fomsing << 0));
}

#endif // RW_RADAR_EN
/// @}

#if RW_RADAR_EN
/**
 * @name RWNXAGCRADARTIM register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31      PULSETIMERCLEAR   0
 *  18:16      PULSEIRQFIFOTHR   0x3
 *  07:00      PULSEIRQTIMEOUT   0x8
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCRADARTIM register
#define RIU_RWNXAGCRADARTIM_ADDR   (REG_RIU_BASE_ADDR+0xB3CC)
/// Offset of the RWNXAGCRADARTIM register from the base address
#define RIU_RWNXAGCRADARTIM_OFFSET 0x000003CC
/// Index of the RWNXAGCRADARTIM register
#define RIU_RWNXAGCRADARTIM_INDEX  0x000000F3
/// Reset value of the RWNXAGCRADARTIM register
#define RIU_RWNXAGCRADARTIM_RESET  0x00030008

/**
 * @brief Returns the current value of the RWNXAGCRADARTIM register.
 * The RWNXAGCRADARTIM register will be read and its value returned.
 * @return The current value of the RWNXAGCRADARTIM register.
 */
__INLINE uint32_t riu_rwnxagcradartim_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR);
}

/**
 * @brief Sets the RWNXAGCRADARTIM register to a value.
 * The RWNXAGCRADARTIM register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcradartim_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCRADARTIM_ADDR, value);
}

// field definitions
/// PULSETIMERCLEAR field bit
#define RIU_PULSETIMERCLEAR_BIT    ((uint32_t)0x80000000)
/// PULSETIMERCLEAR field position
#define RIU_PULSETIMERCLEAR_POS    31
/// PULSEIRQFIFOTHR field mask
#define RIU_PULSEIRQFIFOTHR_MASK   ((uint32_t)0x00070000)
/// PULSEIRQFIFOTHR field LSB position
#define RIU_PULSEIRQFIFOTHR_LSB    16
/// PULSEIRQFIFOTHR field width
#define RIU_PULSEIRQFIFOTHR_WIDTH  ((uint32_t)0x00000003)
/// PULSEIRQTIMEOUT field mask
#define RIU_PULSEIRQTIMEOUT_MASK   ((uint32_t)0x000000FF)
/// PULSEIRQTIMEOUT field LSB position
#define RIU_PULSEIRQTIMEOUT_LSB    0
/// PULSEIRQTIMEOUT field width
#define RIU_PULSEIRQTIMEOUT_WIDTH  ((uint32_t)0x00000008)

/// PULSETIMERCLEAR field reset value
#define RIU_PULSETIMERCLEAR_RST    0x0
/// PULSEIRQFIFOTHR field reset value
#define RIU_PULSEIRQFIFOTHR_RST    0x3
/// PULSEIRQTIMEOUT field reset value
#define RIU_PULSEIRQTIMEOUT_RST    0x8

/**
 * @brief Constructs a value for the RWNXAGCRADARTIM register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] pulsetimerclear - The value to use for the PULSETIMERCLEAR field.
 * @param[in] pulseirqfifothr - The value to use for the PULSEIRQFIFOTHR field.
 * @param[in] pulseirqtimeout - The value to use for the PULSEIRQTIMEOUT field.
 */
__INLINE void riu_rwnxagcradartim_pack(uint8_t pulsetimerclear, uint8_t pulseirqfifothr, uint8_t pulseirqtimeout)
{
    ASSERT_ERR((((uint32_t)pulsetimerclear << 31) & ~((uint32_t)0x80000000)) == 0);
    ASSERT_ERR((((uint32_t)pulseirqfifothr << 16) & ~((uint32_t)0x00070000)) == 0);
    ASSERT_ERR((((uint32_t)pulseirqtimeout << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADARTIM_ADDR,  ((uint32_t)pulsetimerclear << 31) | ((uint32_t)pulseirqfifothr << 16) | ((uint32_t)pulseirqtimeout << 0));
}

/**
 * @brief Unpacks RWNXAGCRADARTIM's fields from current value of the RWNXAGCRADARTIM register.
 *
 * Reads the RWNXAGCRADARTIM register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] pulsetimerclear - Will be populated with the current value of this field from the register.
 * @param[out] pulseirqfifothr - Will be populated with the current value of this field from the register.
 * @param[out] pulseirqtimeout - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcradartim_unpack(uint8_t* pulsetimerclear, uint8_t* pulseirqfifothr, uint8_t* pulseirqtimeout)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR);

    *pulsetimerclear = (localVal & ((uint32_t)0x80000000)) >> 31;
    *pulseirqfifothr = (localVal & ((uint32_t)0x00070000)) >> 16;
    *pulseirqtimeout = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the PULSETIMERCLEAR field in the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read and the PULSETIMERCLEAR field's value will be returned.
 *
 * @return The current value of the PULSETIMERCLEAR field in the RWNXAGCRADARTIM register.
 */
__INLINE uint8_t riu_pulsetimerclear_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the PULSETIMERCLEAR field of the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read, modified to contain the new field value, and written.
 *
 * @param[in] pulsetimerclear - The value to set the field to.
 */
__INLINE void riu_pulsetimerclear_setf(uint8_t pulsetimerclear)
{
    ASSERT_ERR((((uint32_t)pulsetimerclear << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADARTIM_ADDR, (REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR) & ~((uint32_t)0x80000000)) | ((uint32_t)pulsetimerclear << 31));
}

/**
 * @brief Returns the current value of the PULSEIRQFIFOTHR field in the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read and the PULSEIRQFIFOTHR field's value will be returned.
 *
 * @return The current value of the PULSEIRQFIFOTHR field in the RWNXAGCRADARTIM register.
 */
__INLINE uint8_t riu_pulseirqfifothr_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR);
    return ((localVal & ((uint32_t)0x00070000)) >> 16);
}

/**
 * @brief Sets the PULSEIRQFIFOTHR field of the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read, modified to contain the new field value, and written.
 *
 * @param[in] pulseirqfifothr - The value to set the field to.
 */
__INLINE void riu_pulseirqfifothr_setf(uint8_t pulseirqfifothr)
{
    ASSERT_ERR((((uint32_t)pulseirqfifothr << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADARTIM_ADDR, (REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR) & ~((uint32_t)0x00070000)) | ((uint32_t)pulseirqfifothr << 16));
}

/**
 * @brief Returns the current value of the PULSEIRQTIMEOUT field in the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read and the PULSEIRQTIMEOUT field's value will be returned.
 *
 * @return The current value of the PULSEIRQTIMEOUT field in the RWNXAGCRADARTIM register.
 */
__INLINE uint8_t riu_pulseirqtimeout_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/**
 * @brief Sets the PULSEIRQTIMEOUT field of the RWNXAGCRADARTIM register.
 *
 * The RWNXAGCRADARTIM register will be read, modified to contain the new field value, and written.
 *
 * @param[in] pulseirqtimeout - The value to set the field to.
 */
__INLINE void riu_pulseirqtimeout_setf(uint8_t pulseirqtimeout)
{
    ASSERT_ERR((((uint32_t)pulseirqtimeout << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(RIU_RWNXAGCRADARTIM_ADDR, (REG_PL_RD(RIU_RWNXAGCRADARTIM_ADDR) & ~((uint32_t)0x000000FF)) | ((uint32_t)pulseirqtimeout << 0));
}

#endif // RW_RADAR_EN
/// @}

#if RW_RADAR_EN
/**
 * @name RWNXAGCDCCOMPRADAR register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:28          DCRADARTYPE   0x0
 *  22:16   DCRADARHOLDTIME50NS   0x8
 *  14:08   DCRADARZEROTIME50NS   0x0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXAGCDCCOMPRADAR register
#define RIU_RWNXAGCDCCOMPRADAR_ADDR   (REG_RIU_BASE_ADDR+0xB3D0)
/// Offset of the RWNXAGCDCCOMPRADAR register from the base address
#define RIU_RWNXAGCDCCOMPRADAR_OFFSET 0x000003D0
/// Index of the RWNXAGCDCCOMPRADAR register
#define RIU_RWNXAGCDCCOMPRADAR_INDEX  0x000000F4
/// Reset value of the RWNXAGCDCCOMPRADAR register
#define RIU_RWNXAGCDCCOMPRADAR_RESET  0x00080000

/**
 * @brief Returns the current value of the RWNXAGCDCCOMPRADAR register.
 * The RWNXAGCDCCOMPRADAR register will be read and its value returned.
 * @return The current value of the RWNXAGCDCCOMPRADAR register.
 */
__INLINE uint32_t riu_rwnxagcdccompradar_get(void)
{
    return REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR);
}

/**
 * @brief Sets the RWNXAGCDCCOMPRADAR register to a value.
 * The RWNXAGCDCCOMPRADAR register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxagcdccompradar_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXAGCDCCOMPRADAR_ADDR, value);
}

// field definitions
/// DCRADARTYPE field mask
#define RIU_DCRADARTYPE_MASK           ((uint32_t)0x30000000)
/// DCRADARTYPE field LSB position
#define RIU_DCRADARTYPE_LSB            28
/// DCRADARTYPE field width
#define RIU_DCRADARTYPE_WIDTH          ((uint32_t)0x00000002)
/// DCRADARHOLDTIME50NS field mask
#define RIU_DCRADARHOLDTIME50NS_MASK   ((uint32_t)0x007F0000)
/// DCRADARHOLDTIME50NS field LSB position
#define RIU_DCRADARHOLDTIME50NS_LSB    16
/// DCRADARHOLDTIME50NS field width
#define RIU_DCRADARHOLDTIME50NS_WIDTH  ((uint32_t)0x00000007)
/// DCRADARZEROTIME50NS field mask
#define RIU_DCRADARZEROTIME50NS_MASK   ((uint32_t)0x00007F00)
/// DCRADARZEROTIME50NS field LSB position
#define RIU_DCRADARZEROTIME50NS_LSB    8
/// DCRADARZEROTIME50NS field width
#define RIU_DCRADARZEROTIME50NS_WIDTH  ((uint32_t)0x00000007)

/// DCRADARTYPE field reset value
#define RIU_DCRADARTYPE_RST            0x0
/// DCRADARHOLDTIME50NS field reset value
#define RIU_DCRADARHOLDTIME50NS_RST    0x8
/// DCRADARZEROTIME50NS field reset value
#define RIU_DCRADARZEROTIME50NS_RST    0x0

/**
 * @brief Constructs a value for the RWNXAGCDCCOMPRADAR register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] dcradartype - The value to use for the DCRADARTYPE field.
 * @param[in] dcradarholdtime50ns - The value to use for the DCRADARHOLDTIME50NS field.
 * @param[in] dcradarzerotime50ns - The value to use for the DCRADARZEROTIME50NS field.
 */
__INLINE void riu_rwnxagcdccompradar_pack(uint8_t dcradartype, uint8_t dcradarholdtime50ns, uint8_t dcradarzerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcradartype << 28) & ~((uint32_t)0x30000000)) == 0);
    ASSERT_ERR((((uint32_t)dcradarholdtime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    ASSERT_ERR((((uint32_t)dcradarzerotime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMPRADAR_ADDR,  ((uint32_t)dcradartype << 28) | ((uint32_t)dcradarholdtime50ns << 16) | ((uint32_t)dcradarzerotime50ns << 8));
}

/**
 * @brief Unpacks RWNXAGCDCCOMPRADAR's fields from current value of the RWNXAGCDCCOMPRADAR register.
 *
 * Reads the RWNXAGCDCCOMPRADAR register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] dcradartype - Will be populated with the current value of this field from the register.
 * @param[out] dcradarholdtime50ns - Will be populated with the current value of this field from the register.
 * @param[out] dcradarzerotime50ns - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxagcdccompradar_unpack(uint8_t* dcradartype, uint8_t* dcradarholdtime50ns, uint8_t* dcradarzerotime50ns)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR);

    *dcradartype = (localVal & ((uint32_t)0x30000000)) >> 28;
    *dcradarholdtime50ns = (localVal & ((uint32_t)0x007F0000)) >> 16;
    *dcradarzerotime50ns = (localVal & ((uint32_t)0x00007F00)) >> 8;
}

/**
 * @brief Returns the current value of the DCRADARTYPE field in the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read and the DCRADARTYPE field's value will be returned.
 *
 * @return The current value of the DCRADARTYPE field in the RWNXAGCDCCOMPRADAR register.
 */
__INLINE uint8_t riu_dcradartype_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR);
    return ((localVal & ((uint32_t)0x30000000)) >> 28);
}

/**
 * @brief Sets the DCRADARTYPE field of the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcradartype - The value to set the field to.
 */
__INLINE void riu_dcradartype_setf(uint8_t dcradartype)
{
    ASSERT_ERR((((uint32_t)dcradartype << 28) & ~((uint32_t)0x30000000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMPRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR) & ~((uint32_t)0x30000000)) | ((uint32_t)dcradartype << 28));
}

/**
 * @brief Returns the current value of the DCRADARHOLDTIME50NS field in the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read and the DCRADARHOLDTIME50NS field's value will be returned.
 *
 * @return The current value of the DCRADARHOLDTIME50NS field in the RWNXAGCDCCOMPRADAR register.
 */
__INLINE uint8_t riu_dcradarholdtime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR);
    return ((localVal & ((uint32_t)0x007F0000)) >> 16);
}

/**
 * @brief Sets the DCRADARHOLDTIME50NS field of the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcradarholdtime50ns - The value to set the field to.
 */
__INLINE void riu_dcradarholdtime50ns_setf(uint8_t dcradarholdtime50ns)
{
    ASSERT_ERR((((uint32_t)dcradarholdtime50ns << 16) & ~((uint32_t)0x007F0000)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMPRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR) & ~((uint32_t)0x007F0000)) | ((uint32_t)dcradarholdtime50ns << 16));
}

/**
 * @brief Returns the current value of the DCRADARZEROTIME50NS field in the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read and the DCRADARZEROTIME50NS field's value will be returned.
 *
 * @return The current value of the DCRADARZEROTIME50NS field in the RWNXAGCDCCOMPRADAR register.
 */
__INLINE uint8_t riu_dcradarzerotime50ns_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Sets the DCRADARZEROTIME50NS field of the RWNXAGCDCCOMPRADAR register.
 *
 * The RWNXAGCDCCOMPRADAR register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcradarzerotime50ns - The value to set the field to.
 */
__INLINE void riu_dcradarzerotime50ns_setf(uint8_t dcradarzerotime50ns)
{
    ASSERT_ERR((((uint32_t)dcradarzerotime50ns << 8) & ~((uint32_t)0x00007F00)) == 0);
    REG_PL_WR(RIU_RWNXAGCDCCOMPRADAR_ADDR, (REG_PL_RD(RIU_RWNXAGCDCCOMPRADAR_ADDR) & ~((uint32_t)0x00007F00)) | ((uint32_t)dcradarzerotime50ns << 8));
}

#endif // RW_RADAR_EN
/// @}

/**
 * @name RWNXMACINTEN register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16        IRQMACRFIRQEN   0
 *     12     IRQMACRADARDETEN   0
 *     08   IRQMACCCATIMEOUTEN   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACINTEN register
#define RIU_RWNXMACINTEN_ADDR   (REG_RIU_BASE_ADDR+0xB414)
/// Offset of the RWNXMACINTEN register from the base address
#define RIU_RWNXMACINTEN_OFFSET 0x00000414
/// Index of the RWNXMACINTEN register
#define RIU_RWNXMACINTEN_INDEX  0x00000105
/// Reset value of the RWNXMACINTEN register
#define RIU_RWNXMACINTEN_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACINTEN register.
 * The RWNXMACINTEN register will be read and its value returned.
 * @return The current value of the RWNXMACINTEN register.
 */
__INLINE uint32_t riu_rwnxmacinten_get(void)
{
    return REG_PL_RD(RIU_RWNXMACINTEN_ADDR);
}

/**
 * @brief Sets the RWNXMACINTEN register to a value.
 * The RWNXMACINTEN register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxmacinten_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXMACINTEN_ADDR, value);
}

// field definitions
/// IRQMACRFIRQEN field bit
#define RIU_IRQMACRFIRQEN_BIT         ((uint32_t)0x00010000)
/// IRQMACRFIRQEN field position
#define RIU_IRQMACRFIRQEN_POS         16
/// IRQMACRADARDETEN field bit
#define RIU_IRQMACRADARDETEN_BIT      ((uint32_t)0x00001000)
/// IRQMACRADARDETEN field position
#define RIU_IRQMACRADARDETEN_POS      12
/// IRQMACCCATIMEOUTEN field bit
#define RIU_IRQMACCCATIMEOUTEN_BIT    ((uint32_t)0x00000100)
/// IRQMACCCATIMEOUTEN field position
#define RIU_IRQMACCCATIMEOUTEN_POS    8

/// IRQMACRFIRQEN field reset value
#define RIU_IRQMACRFIRQEN_RST         0x0
/// IRQMACRADARDETEN field reset value
#define RIU_IRQMACRADARDETEN_RST      0x0
/// IRQMACCCATIMEOUTEN field reset value
#define RIU_IRQMACCCATIMEOUTEN_RST    0x0

/**
 * @brief Constructs a value for the RWNXMACINTEN register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] irqmacrfirqen - The value to use for the IRQMACRFIRQEN field.
 * @param[in] irqmacradardeten - The value to use for the IRQMACRADARDETEN field.
 * @param[in] irqmacccatimeouten - The value to use for the IRQMACCCATIMEOUTEN field.
 */
__INLINE void riu_rwnxmacinten_pack(uint8_t irqmacrfirqen, uint8_t irqmacradardeten, uint8_t irqmacccatimeouten)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqen << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacradardeten << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacccatimeouten << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTEN_ADDR,  ((uint32_t)irqmacrfirqen << 16) | ((uint32_t)irqmacradardeten << 12) | ((uint32_t)irqmacccatimeouten << 8));
}

/**
 * @brief Unpacks RWNXMACINTEN's fields from current value of the RWNXMACINTEN register.
 *
 * Reads the RWNXMACINTEN register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] irqmacrfirqen - Will be populated with the current value of this field from the register.
 * @param[out] irqmacradardeten - Will be populated with the current value of this field from the register.
 * @param[out] irqmacccatimeouten - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxmacinten_unpack(uint8_t* irqmacrfirqen, uint8_t* irqmacradardeten, uint8_t* irqmacccatimeouten)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTEN_ADDR);

    *irqmacrfirqen = (localVal & ((uint32_t)0x00010000)) >> 16;
    *irqmacradardeten = (localVal & ((uint32_t)0x00001000)) >> 12;
    *irqmacccatimeouten = (localVal & ((uint32_t)0x00000100)) >> 8;
}

/**
 * @brief Returns the current value of the IRQMACRFIRQEN field in the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read and the IRQMACRFIRQEN field's value will be returned.
 *
 * @return The current value of the IRQMACRFIRQEN field in the RWNXMACINTEN register.
 */
__INLINE uint8_t riu_irqmacrfirqen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTEN_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the IRQMACRFIRQEN field of the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacrfirqen - The value to set the field to.
 */
__INLINE void riu_irqmacrfirqen_setf(uint8_t irqmacrfirqen)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqen << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTEN_ADDR, (REG_PL_RD(RIU_RWNXMACINTEN_ADDR) & ~((uint32_t)0x00010000)) | ((uint32_t)irqmacrfirqen << 16));
}

/**
 * @brief Returns the current value of the IRQMACRADARDETEN field in the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read and the IRQMACRADARDETEN field's value will be returned.
 *
 * @return The current value of the IRQMACRADARDETEN field in the RWNXMACINTEN register.
 */
__INLINE uint8_t riu_irqmacradardeten_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTEN_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the IRQMACRADARDETEN field of the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacradardeten - The value to set the field to.
 */
__INLINE void riu_irqmacradardeten_setf(uint8_t irqmacradardeten)
{
    ASSERT_ERR((((uint32_t)irqmacradardeten << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTEN_ADDR, (REG_PL_RD(RIU_RWNXMACINTEN_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)irqmacradardeten << 12));
}

/**
 * @brief Returns the current value of the IRQMACCCATIMEOUTEN field in the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read and the IRQMACCCATIMEOUTEN field's value will be returned.
 *
 * @return The current value of the IRQMACCCATIMEOUTEN field in the RWNXMACINTEN register.
 */
__INLINE uint8_t riu_irqmacccatimeouten_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTEN_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the IRQMACCCATIMEOUTEN field of the RWNXMACINTEN register.
 *
 * The RWNXMACINTEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacccatimeouten - The value to set the field to.
 */
__INLINE void riu_irqmacccatimeouten_setf(uint8_t irqmacccatimeouten)
{
    ASSERT_ERR((((uint32_t)irqmacccatimeouten << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTEN_ADDR, (REG_PL_RD(RIU_RWNXMACINTEN_ADDR) & ~((uint32_t)0x00000100)) | ((uint32_t)irqmacccatimeouten << 8));
}

/// @}

/**
 * @name RWNXMACINTSTATRAW register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16       IRQMACRFIRQRAW   0
 *     12    IRQMACRADARDETRAW   0
 *     08   IRQMACCCATIMEOUTRAW   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACINTSTATRAW register
#define RIU_RWNXMACINTSTATRAW_ADDR   (REG_RIU_BASE_ADDR+0xB418)
/// Offset of the RWNXMACINTSTATRAW register from the base address
#define RIU_RWNXMACINTSTATRAW_OFFSET 0x00000418
/// Index of the RWNXMACINTSTATRAW register
#define RIU_RWNXMACINTSTATRAW_INDEX  0x00000106
/// Reset value of the RWNXMACINTSTATRAW register
#define RIU_RWNXMACINTSTATRAW_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACINTSTATRAW register.
 * The RWNXMACINTSTATRAW register will be read and its value returned.
 * @return The current value of the RWNXMACINTSTATRAW register.
 */
__INLINE uint32_t riu_rwnxmacintstatraw_get(void)
{
    return REG_PL_RD(RIU_RWNXMACINTSTATRAW_ADDR);
}

// field definitions
/// IRQMACRFIRQRAW field bit
#define RIU_IRQMACRFIRQRAW_BIT         ((uint32_t)0x00010000)
/// IRQMACRFIRQRAW field position
#define RIU_IRQMACRFIRQRAW_POS         16
/// IRQMACRADARDETRAW field bit
#define RIU_IRQMACRADARDETRAW_BIT      ((uint32_t)0x00001000)
/// IRQMACRADARDETRAW field position
#define RIU_IRQMACRADARDETRAW_POS      12
/// IRQMACCCATIMEOUTRAW field bit
#define RIU_IRQMACCCATIMEOUTRAW_BIT    ((uint32_t)0x00000100)
/// IRQMACCCATIMEOUTRAW field position
#define RIU_IRQMACCCATIMEOUTRAW_POS    8

/// IRQMACRFIRQRAW field reset value
#define RIU_IRQMACRFIRQRAW_RST         0x0
/// IRQMACRADARDETRAW field reset value
#define RIU_IRQMACRADARDETRAW_RST      0x0
/// IRQMACCCATIMEOUTRAW field reset value
#define RIU_IRQMACCCATIMEOUTRAW_RST    0x0

/**
 * @brief Unpacks RWNXMACINTSTATRAW's fields from current value of the RWNXMACINTSTATRAW register.
 *
 * Reads the RWNXMACINTSTATRAW register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] irqmacrfirqraw - Will be populated with the current value of this field from the register.
 * @param[out] irqmacradardetraw - Will be populated with the current value of this field from the register.
 * @param[out] irqmacccatimeoutraw - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxmacintstatraw_unpack(uint8_t* irqmacrfirqraw, uint8_t* irqmacradardetraw, uint8_t* irqmacccatimeoutraw)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATRAW_ADDR);

    *irqmacrfirqraw = (localVal & ((uint32_t)0x00010000)) >> 16;
    *irqmacradardetraw = (localVal & ((uint32_t)0x00001000)) >> 12;
    *irqmacccatimeoutraw = (localVal & ((uint32_t)0x00000100)) >> 8;
}

/**
 * @brief Returns the current value of the IRQMACRFIRQRAW field in the RWNXMACINTSTATRAW register.
 *
 * The RWNXMACINTSTATRAW register will be read and the IRQMACRFIRQRAW field's value will be returned.
 *
 * @return The current value of the IRQMACRFIRQRAW field in the RWNXMACINTSTATRAW register.
 */
__INLINE uint8_t riu_irqmacrfirqraw_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATRAW_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the IRQMACRADARDETRAW field in the RWNXMACINTSTATRAW register.
 *
 * The RWNXMACINTSTATRAW register will be read and the IRQMACRADARDETRAW field's value will be returned.
 *
 * @return The current value of the IRQMACRADARDETRAW field in the RWNXMACINTSTATRAW register.
 */
__INLINE uint8_t riu_irqmacradardetraw_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATRAW_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Returns the current value of the IRQMACCCATIMEOUTRAW field in the RWNXMACINTSTATRAW register.
 *
 * The RWNXMACINTSTATRAW register will be read and the IRQMACCCATIMEOUTRAW field's value will be returned.
 *
 * @return The current value of the IRQMACCCATIMEOUTRAW field in the RWNXMACINTSTATRAW register.
 */
__INLINE uint8_t riu_irqmacccatimeoutraw_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATRAW_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/// @}

/**
 * @name RWNXMACINTSTATMASKED register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     12   IRQMACRADARDETMASKED   0
 *     08   IRQMACCCATIMEOUTMASKED   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACINTSTATMASKED register
#define RIU_RWNXMACINTSTATMASKED_ADDR   (REG_RIU_BASE_ADDR+0xB41C)
/// Offset of the RWNXMACINTSTATMASKED register from the base address
#define RIU_RWNXMACINTSTATMASKED_OFFSET 0x0000041C
/// Index of the RWNXMACINTSTATMASKED register
#define RIU_RWNXMACINTSTATMASKED_INDEX  0x00000107
/// Reset value of the RWNXMACINTSTATMASKED register
#define RIU_RWNXMACINTSTATMASKED_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACINTSTATMASKED register.
 * The RWNXMACINTSTATMASKED register will be read and its value returned.
 * @return The current value of the RWNXMACINTSTATMASKED register.
 */
__INLINE uint32_t riu_rwnxmacintstatmasked_get(void)
{
    return REG_PL_RD(RIU_RWNXMACINTSTATMASKED_ADDR);
}

// field definitions
/// IRQMACRFIRQMASKED field bit
#define RIU_IRQMACRFIRQMASKED_BIT         ((uint32_t)0x00010000)
/// IRQMACRFIRQMASKED field position
#define RIU_IRQMACRFIRQMASKED_POS         16
/// IRQMACRADARDETMASKED field bit
#define RIU_IRQMACRADARDETMASKED_BIT      ((uint32_t)0x00001000)
/// IRQMACRADARDETMASKED field position
#define RIU_IRQMACRADARDETMASKED_POS      12
/// IRQMACCCATIMEOUTMASKED field bit
#define RIU_IRQMACCCATIMEOUTMASKED_BIT    ((uint32_t)0x00000100)
/// IRQMACCCATIMEOUTMASKED field position
#define RIU_IRQMACCCATIMEOUTMASKED_POS    8

/// IRQMACRFIRQMASKED field reset value
#define RIU_IRQMACRFIRQMASKED_RST         0x0
/// IRQMACRADARDETMASKED field reset value
#define RIU_IRQMACRADARDETMASKED_RST      0x0
/// IRQMACCCATIMEOUTMASKED field reset value
#define RIU_IRQMACCCATIMEOUTMASKED_RST    0x0

/**
 * @brief Unpacks RWNXMACINTSTATMASKED's fields from current value of the RWNXMACINTSTATMASKED register.
 *
 * Reads the RWNXMACINTSTATMASKED register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] irqmacrfirqmasked - Will be populated with the current value of this field from the register.
 * @param[out] irqmacradardetmasked - Will be populated with the current value of this field from the register.
 * @param[out] irqmacccatimeoutmasked - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxmacintstatmasked_unpack(uint8_t* irqmacrfirqmasked, uint8_t* irqmacradardetmasked, uint8_t* irqmacccatimeoutmasked)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATMASKED_ADDR);

    *irqmacrfirqmasked = (localVal & ((uint32_t)0x00010000)) >> 16;
    *irqmacradardetmasked = (localVal & ((uint32_t)0x00001000)) >> 12;
    *irqmacccatimeoutmasked = (localVal & ((uint32_t)0x00000100)) >> 8;
}

/**
 * @brief Returns the current value of the IRQMACRFIRQMASKED field in the RWNXMACINTSTATMASKED register.
 *
 * The RWNXMACINTSTATMASKED register will be read and the IRQMACRFIRQMASKED field's value will be returned.
 *
 * @return The current value of the IRQMACRFIRQMASKED field in the RWNXMACINTSTATMASKED register.
 */
__INLINE uint8_t riu_irqmacrfirqmasked_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATMASKED_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the IRQMACRADARDETMASKED field in the RWNXMACINTSTATMASKED register.
 *
 * The RWNXMACINTSTATMASKED register will be read and the IRQMACRADARDETMASKED field's value will be returned.
 *
 * @return The current value of the IRQMACRADARDETMASKED field in the RWNXMACINTSTATMASKED register.
 */
__INLINE uint8_t riu_irqmacradardetmasked_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATMASKED_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Returns the current value of the IRQMACCCATIMEOUTMASKED field in the RWNXMACINTSTATMASKED register.
 *
 * The RWNXMACINTSTATMASKED register will be read and the IRQMACCCATIMEOUTMASKED field's value will be returned.
 *
 * @return The current value of the IRQMACCCATIMEOUTMASKED field in the RWNXMACINTSTATMASKED register.
 */
__INLINE uint8_t riu_irqmacccatimeoutmasked_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTSTATMASKED_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/// @}

/**
 * @name RWNXMACINTACK register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16       IRQMACRFIRQACK   0
 *     12    IRQMACRADARDETACK   0
 *     08   IRQMACCCATIMEOUTACK   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACINTACK register
#define RIU_RWNXMACINTACK_ADDR   (REG_RIU_BASE_ADDR+0xB420)
/// Offset of the RWNXMACINTACK register from the base address
#define RIU_RWNXMACINTACK_OFFSET 0x00000420
/// Index of the RWNXMACINTACK register
#define RIU_RWNXMACINTACK_INDEX  0x00000108
/// Reset value of the RWNXMACINTACK register
#define RIU_RWNXMACINTACK_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACINTACK register.
 * The RWNXMACINTACK register will be read and its value returned.
 * @return The current value of the RWNXMACINTACK register.
 */
__INLINE uint32_t riu_rwnxmacintack_get(void)
{
    return REG_PL_RD(RIU_RWNXMACINTACK_ADDR);
}

/**
 * @brief Sets the RWNXMACINTACK register to a value.
 * The RWNXMACINTACK register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxmacintack_clear(uint32_t value)
{
    REG_PL_WR(RIU_RWNXMACINTACK_ADDR, value);
}

// field definitions
/// IRQMACRFIRQACK field bit
#define RIU_IRQMACRFIRQACK_BIT         ((uint32_t)0x00010000)
/// IRQMACRFIRQACK field position
#define RIU_IRQMACRFIRQACK_POS         16
/// IRQMACRADARDETACK field bit
#define RIU_IRQMACRADARDETACK_BIT      ((uint32_t)0x00001000)
/// IRQMACRADARDETACK field position
#define RIU_IRQMACRADARDETACK_POS      12
/// IRQMACCCATIMEOUTACK field bit
#define RIU_IRQMACCCATIMEOUTACK_BIT    ((uint32_t)0x00000100)
/// IRQMACCCATIMEOUTACK field position
#define RIU_IRQMACCCATIMEOUTACK_POS    8

/// IRQMACRFIRQACK field reset value
#define RIU_IRQMACRFIRQACK_RST         0x0
/// IRQMACRADARDETACK field reset value
#define RIU_IRQMACRADARDETACK_RST      0x0
/// IRQMACCCATIMEOUTACK field reset value
#define RIU_IRQMACCCATIMEOUTACK_RST    0x0

/**
 * @brief Constructs a value for the RWNXMACINTACK register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] irqmacrfirqack - The value to use for the IRQMACRFIRQACK field.
 * @param[in] irqmacradardetack - The value to use for the IRQMACRADARDETACK field.
 * @param[in] irqmacccatimeoutack - The value to use for the IRQMACCCATIMEOUTACK field.
 */
__INLINE void riu_rwnxmacintack_pack(uint8_t irqmacrfirqack, uint8_t irqmacradardetack, uint8_t irqmacccatimeoutack)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqack << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacradardetack << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacccatimeoutack << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTACK_ADDR,  ((uint32_t)irqmacrfirqack << 16) | ((uint32_t)irqmacradardetack << 12) | ((uint32_t)irqmacccatimeoutack << 8));
}

/**
 * @brief Unpacks RWNXMACINTACK's fields from current value of the RWNXMACINTACK register.
 *
 * Reads the RWNXMACINTACK register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] irqmacrfirqack - Will be populated with the current value of this field from the register.
 * @param[out] irqmacradardetack - Will be populated with the current value of this field from the register.
 * @param[out] irqmacccatimeoutack - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxmacintack_unpack(uint8_t* irqmacrfirqack, uint8_t* irqmacradardetack, uint8_t* irqmacccatimeoutack)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTACK_ADDR);

    *irqmacrfirqack = (localVal & ((uint32_t)0x00010000)) >> 16;
    *irqmacradardetack = (localVal & ((uint32_t)0x00001000)) >> 12;
    *irqmacccatimeoutack = (localVal & ((uint32_t)0x00000100)) >> 8;
}

/**
 * @brief Returns the current value of the IRQMACRFIRQACK field in the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read and the IRQMACRFIRQACK field's value will be returned.
 *
 * @return The current value of the IRQMACRFIRQACK field in the RWNXMACINTACK register.
 */
__INLINE uint8_t riu_irqmacrfirqack_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTACK_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the IRQMACRFIRQACK field of the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacrfirqack - The value to set the field to.
 */
__INLINE void riu_irqmacrfirqack_clearf(uint8_t irqmacrfirqack)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqack << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTACK_ADDR, (uint32_t)irqmacrfirqack << 16);
}

/**
 * @brief Returns the current value of the IRQMACRADARDETACK field in the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read and the IRQMACRADARDETACK field's value will be returned.
 *
 * @return The current value of the IRQMACRADARDETACK field in the RWNXMACINTACK register.
 */
__INLINE uint8_t riu_irqmacradardetack_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTACK_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the IRQMACRADARDETACK field of the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacradardetack - The value to set the field to.
 */
__INLINE void riu_irqmacradardetack_clearf(uint8_t irqmacradardetack)
{
    ASSERT_ERR((((uint32_t)irqmacradardetack << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTACK_ADDR, (uint32_t)irqmacradardetack << 12);
}

/**
 * @brief Returns the current value of the IRQMACCCATIMEOUTACK field in the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read and the IRQMACCCATIMEOUTACK field's value will be returned.
 *
 * @return The current value of the IRQMACCCATIMEOUTACK field in the RWNXMACINTACK register.
 */
__INLINE uint8_t riu_irqmacccatimeoutack_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTACK_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the IRQMACCCATIMEOUTACK field of the RWNXMACINTACK register.
 *
 * The RWNXMACINTACK register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacccatimeoutack - The value to set the field to.
 */
__INLINE void riu_irqmacccatimeoutack_clearf(uint8_t irqmacccatimeoutack)
{
    ASSERT_ERR((((uint32_t)irqmacccatimeoutack << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTACK_ADDR, (uint32_t)irqmacccatimeoutack << 8);
}

/// @}

/**
 * @name RWNXMACINTGEN register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     16       IRQMACRFIRQGEN   0
 *     12    IRQMACRADARDETGEN   0
 *     08   IRQMACCCATIMEOUTGEN   0
 * </pre>
 *
 * @{
 */

/// Address of the RWNXMACINTGEN register
#define RIU_RWNXMACINTGEN_ADDR   (REG_RIU_BASE_ADDR+0xB424)
/// Offset of the RWNXMACINTGEN register from the base address
#define RIU_RWNXMACINTGEN_OFFSET 0x00000424
/// Index of the RWNXMACINTGEN register
#define RIU_RWNXMACINTGEN_INDEX  0x00000109
/// Reset value of the RWNXMACINTGEN register
#define RIU_RWNXMACINTGEN_RESET  0x00000000

/**
 * @brief Returns the current value of the RWNXMACINTGEN register.
 * The RWNXMACINTGEN register will be read and its value returned.
 * @return The current value of the RWNXMACINTGEN register.
 */
__INLINE uint32_t riu_rwnxmacintgen_get(void)
{
    return REG_PL_RD(RIU_RWNXMACINTGEN_ADDR);
}

/**
 * @brief Sets the RWNXMACINTGEN register to a value.
 * The RWNXMACINTGEN register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxmacintgen_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXMACINTGEN_ADDR, value);
}

// field definitions
/// IRQMACRFIRQGEN field bit
#define RIU_IRQMACRFIRQGEN_BIT         ((uint32_t)0x00010000)
/// IRQMACRFIRQGEN field position
#define RIU_IRQMACRFIRQGEN_POS         16
/// IRQMACRADARDETGEN field bit
#define RIU_IRQMACRADARDETGEN_BIT      ((uint32_t)0x00001000)
/// IRQMACRADARDETGEN field position
#define RIU_IRQMACRADARDETGEN_POS      12
/// IRQMACCCATIMEOUTGEN field bit
#define RIU_IRQMACCCATIMEOUTGEN_BIT    ((uint32_t)0x00000100)
/// IRQMACCCATIMEOUTGEN field position
#define RIU_IRQMACCCATIMEOUTGEN_POS    8

/// IRQMACRFIRQGEN field reset value
#define RIU_IRQMACRFIRQGEN_RST         0x0
/// IRQMACRADARDETGEN field reset value
#define RIU_IRQMACRADARDETGEN_RST      0x0
/// IRQMACCCATIMEOUTGEN field reset value
#define RIU_IRQMACCCATIMEOUTGEN_RST    0x0

/**
 * @brief Constructs a value for the RWNXMACINTGEN register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] irqmacrfirqgen - The value to use for the IRQMACRFIRQGEN field.
 * @param[in] irqmacradardetgen - The value to use for the IRQMACRADARDETGEN field.
 * @param[in] irqmacccatimeoutgen - The value to use for the IRQMACCCATIMEOUTGEN field.
 */
__INLINE void riu_rwnxmacintgen_pack(uint8_t irqmacrfirqgen, uint8_t irqmacradardetgen, uint8_t irqmacccatimeoutgen)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqgen << 16) & ~((uint32_t)0x00010000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacradardetgen << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)irqmacccatimeoutgen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTGEN_ADDR,  ((uint32_t)irqmacrfirqgen << 16) | ((uint32_t)irqmacradardetgen << 12) | ((uint32_t)irqmacccatimeoutgen << 8));
}

/**
 * @brief Unpacks RWNXMACINTGEN's fields from current value of the RWNXMACINTGEN register.
 *
 * Reads the RWNXMACINTGEN register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] irqmacrfirqgen - Will be populated with the current value of this field from the register.
 * @param[out] irqmacradardetgen - Will be populated with the current value of this field from the register.
 * @param[out] irqmacccatimeoutgen - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxmacintgen_unpack(uint8_t* irqmacrfirqgen, uint8_t* irqmacradardetgen, uint8_t* irqmacccatimeoutgen)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTGEN_ADDR);

    *irqmacrfirqgen = (localVal & ((uint32_t)0x00010000)) >> 16;
    *irqmacradardetgen = (localVal & ((uint32_t)0x00001000)) >> 12;
    *irqmacccatimeoutgen = (localVal & ((uint32_t)0x00000100)) >> 8;
}

/**
 * @brief Returns the current value of the IRQMACRFIRQGEN field in the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read and the IRQMACRFIRQGEN field's value will be returned.
 *
 * @return The current value of the IRQMACRFIRQGEN field in the RWNXMACINTGEN register.
 */
__INLINE uint8_t riu_irqmacrfirqgen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTGEN_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Sets the IRQMACRFIRQGEN field of the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacrfirqgen - The value to set the field to.
 */
__INLINE void riu_irqmacrfirqgen_setf(uint8_t irqmacrfirqgen)
{
    ASSERT_ERR((((uint32_t)irqmacrfirqgen << 16) & ~((uint32_t)0x00010000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTGEN_ADDR, (uint32_t)irqmacrfirqgen << 16);
}

/**
 * @brief Returns the current value of the IRQMACRADARDETGEN field in the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read and the IRQMACRADARDETGEN field's value will be returned.
 *
 * @return The current value of the IRQMACRADARDETGEN field in the RWNXMACINTGEN register.
 */
__INLINE uint8_t riu_irqmacradardetgen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTGEN_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the IRQMACRADARDETGEN field of the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacradardetgen - The value to set the field to.
 */
__INLINE void riu_irqmacradardetgen_setf(uint8_t irqmacradardetgen)
{
    ASSERT_ERR((((uint32_t)irqmacradardetgen << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_RWNXMACINTGEN_ADDR, (uint32_t)irqmacradardetgen << 12);
}

/**
 * @brief Returns the current value of the IRQMACCCATIMEOUTGEN field in the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read and the IRQMACCCATIMEOUTGEN field's value will be returned.
 *
 * @return The current value of the IRQMACCCATIMEOUTGEN field in the RWNXMACINTGEN register.
 */
__INLINE uint8_t riu_irqmacccatimeoutgen_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXMACINTGEN_ADDR);
    return ((localVal & ((uint32_t)0x00000100)) >> 8);
}

/**
 * @brief Sets the IRQMACCCATIMEOUTGEN field of the RWNXMACINTGEN register.
 *
 * The RWNXMACINTGEN register will be read, modified to contain the new field value, and written.
 *
 * @param[in] irqmacccatimeoutgen - The value to set the field to.
 */
__INLINE void riu_irqmacccatimeoutgen_setf(uint8_t irqmacccatimeoutgen)
{
    ASSERT_ERR((((uint32_t)irqmacccatimeoutgen << 8) & ~((uint32_t)0x00000100)) == 0);
    REG_PL_WR(RIU_RWNXMACINTGEN_ADDR, (uint32_t)irqmacccatimeoutgen << 8);
}

/// @}

/**
 * @name RWNXFEDSSSCCKCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  18:16          RXSHIFT4044   0x4
 *  13:12          TXSHIFT4044   0x1
 *  not use this bit now 11:08        BTXPATHSELREG   0xF
 *  06:05        BRXPATHSELREG   0x0
 *     04     RXPATHSELFROMREG   1
 * </pre>
 *
 * @{
 */

/// Address of the RWNXFEDSSSCCKCNTL register
#define RIU_RWNXFEDSSSCCKCNTL_ADDR   (REG_RIU_BASE_ADDR+0xB500)
/// Offset of the RWNXFEDSSSCCKCNTL register from the base address
#define RIU_RWNXFEDSSSCCKCNTL_OFFSET 0x00000500
/// Index of the RWNXFEDSSSCCKCNTL register
#define RIU_RWNXFEDSSSCCKCNTL_INDEX  0x00000140
/// Reset value of the RWNXFEDSSSCCKCNTL register
#define RIU_RWNXFEDSSSCCKCNTL_RESET  0x00041010
///#define RIU_RWNXFEDSSSCCKCNTL_RESET  0x00041F10

/**
 * @brief Returns the current value of the RWNXFEDSSSCCKCNTL register.
 * The RWNXFEDSSSCCKCNTL register will be read and its value returned.
 * @return The current value of the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint32_t riu_rwnxfedssscckcntl_get(void)
{
    return REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
}

/**
 * @brief Sets the RWNXFEDSSSCCKCNTL register to a value.
 * The RWNXFEDSSSCCKCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void riu_rwnxfedssscckcntl_set(uint32_t value)
{
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, value);
}

// field definitions
/// RXSHIFT4044 field mask
#define RIU_RXSHIFT4044_MASK        ((uint32_t)0x00070000)
/// RXSHIFT4044 field LSB position
#define RIU_RXSHIFT4044_LSB         16
/// RXSHIFT4044 field width
#define RIU_RXSHIFT4044_WIDTH       ((uint32_t)0x00000003)
/// TXSHIFT4044 field mask
#define RIU_TXSHIFT4044_MASK        ((uint32_t)0x00003000)
/// TXSHIFT4044 field LSB position
#define RIU_TXSHIFT4044_LSB         12
/// TXSHIFT4044 field width
#define RIU_TXSHIFT4044_WIDTH       ((uint32_t)0x00000002)
/// BTXPATHSELREG field mask
#define RIU_BTXPATHSELREG_MASK      ((uint32_t)0x00000F00)
/// BTXPATHSELREG field LSB position
#define RIU_BTXPATHSELREG_LSB       8
/// BTXPATHSELREG field width
#define RIU_BTXPATHSELREG_WIDTH     ((uint32_t)0x00000004)
/// BRXPATHSELREG field mask
#define RIU_BRXPATHSELREG_MASK      ((uint32_t)0x00000060)
/// BRXPATHSELREG field LSB position
#define RIU_BRXPATHSELREG_LSB       5
/// BRXPATHSELREG field width
#define RIU_BRXPATHSELREG_WIDTH     ((uint32_t)0x00000002)
/// RXPATHSELFROMREG field bit
#define RIU_RXPATHSELFROMREG_BIT    ((uint32_t)0x00000010)
/// RXPATHSELFROMREG field position
#define RIU_RXPATHSELFROMREG_POS    4

/// RXSHIFT4044 field reset value
#define RIU_RXSHIFT4044_RST         0x4
/// TXSHIFT4044 field reset value
#define RIU_TXSHIFT4044_RST         0x1
/// BTXPATHSELREG field reset value
#define RIU_BTXPATHSELREG_RST       0xF
/// BRXPATHSELREG field reset value
#define RIU_BRXPATHSELREG_RST       0x0
/// RXPATHSELFROMREG field reset value
#define RIU_RXPATHSELFROMREG_RST    0x1

/**
 * @brief Constructs a value for the RWNXFEDSSSCCKCNTL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxshift4044 - The value to use for the RXSHIFT4044 field.
 * @param[in] txshift4044 - The value to use for the TXSHIFT4044 field.
 * @param[in] btxpathselreg - The value to use for the BTXPATHSELREG field.
 * @param[in] brxpathselreg - The value to use for the BRXPATHSELREG field.
 * @param[in] rxpathselfromreg - The value to use for the RXPATHSELFROMREG field.
 */
__INLINE void riu_rwnxfedssscckcntl_pack(uint8_t rxshift4044, uint8_t txshift4044, uint8_t btxpathselreg, uint8_t brxpathselreg, uint8_t rxpathselfromreg)
{
    ASSERT_ERR((((uint32_t)rxshift4044 << 16) & ~((uint32_t)0x00070000)) == 0);
    ASSERT_ERR((((uint32_t)txshift4044 << 12) & ~((uint32_t)0x00003000)) == 0);
    ASSERT_ERR((((uint32_t)btxpathselreg << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)brxpathselreg << 5) & ~((uint32_t)0x00000060)) == 0);
    ASSERT_ERR((((uint32_t)rxpathselfromreg << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR,  ((uint32_t)rxshift4044 << 16) | ((uint32_t)txshift4044 << 12) | ((uint32_t)btxpathselreg << 8) | ((uint32_t)brxpathselreg << 5) | ((uint32_t)rxpathselfromreg << 4));
}

/**
 * @brief Unpacks RWNXFEDSSSCCKCNTL's fields from current value of the RWNXFEDSSSCCKCNTL register.
 *
 * Reads the RWNXFEDSSSCCKCNTL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxshift4044 - Will be populated with the current value of this field from the register.
 * @param[out] txshift4044 - Will be populated with the current value of this field from the register.
 * @param[out] btxpathselreg - Will be populated with the current value of this field from the register.
 * @param[out] brxpathselreg - Will be populated with the current value of this field from the register.
 * @param[out] rxpathselfromreg - Will be populated with the current value of this field from the register.
 */
__INLINE void riu_rwnxfedssscckcntl_unpack(uint8_t* rxshift4044, uint8_t* txshift4044, uint8_t* btxpathselreg, uint8_t* brxpathselreg, uint8_t* rxpathselfromreg)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);

    *rxshift4044 = (localVal & ((uint32_t)0x00070000)) >> 16;
    *txshift4044 = (localVal & ((uint32_t)0x00003000)) >> 12;
    *btxpathselreg = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *brxpathselreg = (localVal & ((uint32_t)0x00000060)) >> 5;
    *rxpathselfromreg = (localVal & ((uint32_t)0x00000010)) >> 4;
}

/**
 * @brief Returns the current value of the RXSHIFT4044 field in the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read and the RXSHIFT4044 field's value will be returned.
 *
 * @return The current value of the RXSHIFT4044 field in the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint8_t riu_rxshift4044_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00070000)) >> 16);
}

/**
 * @brief Sets the RXSHIFT4044 field of the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxshift4044 - The value to set the field to.
 */
__INLINE void riu_rxshift4044_setf(uint8_t rxshift4044)
{
    ASSERT_ERR((((uint32_t)rxshift4044 << 16) & ~((uint32_t)0x00070000)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, (REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR) & ~((uint32_t)0x00070000)) | ((uint32_t)rxshift4044 << 16));
}

/**
 * @brief Returns the current value of the TXSHIFT4044 field in the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read and the TXSHIFT4044 field's value will be returned.
 *
 * @return The current value of the TXSHIFT4044 field in the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint8_t riu_txshift4044_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00003000)) >> 12);
}

/**
 * @brief Sets the TXSHIFT4044 field of the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txshift4044 - The value to set the field to.
 */
__INLINE void riu_txshift4044_setf(uint8_t txshift4044)
{
    ASSERT_ERR((((uint32_t)txshift4044 << 12) & ~((uint32_t)0x00003000)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, (REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR) & ~((uint32_t)0x00003000)) | ((uint32_t)txshift4044 << 12));
}
#if 0
/**
 * @brief Returns the current value of the BTXPATHSELREG field in the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read and the BTXPATHSELREG field's value will be returned.
 *
 * @return The current value of the BTXPATHSELREG field in the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint8_t riu_btxpathselreg_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the BTXPATHSELREG field of the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] btxpathselreg - The value to set the field to.
 */
__INLINE void riu_btxpathselreg_setf(uint8_t btxpathselreg)
{
    ASSERT_ERR((((uint32_t)btxpathselreg << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, (REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)btxpathselreg << 8));
}
#endif
/**
 * @brief Returns the current value of the BRXPATHSELREG field in the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read and the BRXPATHSELREG field's value will be returned.
 *
 * @return The current value of the BRXPATHSELREG field in the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint8_t riu_brxpathselreg_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000060)) >> 5);
}

/**
 * @brief Sets the BRXPATHSELREG field of the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] brxpathselreg - The value to set the field to.
 */
__INLINE void riu_brxpathselreg_setf(uint8_t brxpathselreg)
{
    ASSERT_ERR((((uint32_t)brxpathselreg << 5) & ~((uint32_t)0x00000060)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, (REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR) & ~((uint32_t)0x00000060)) | ((uint32_t)brxpathselreg << 5));
}

/**
 * @brief Returns the current value of the RXPATHSELFROMREG field in the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read and the RXPATHSELFROMREG field's value will be returned.
 *
 * @return The current value of the RXPATHSELFROMREG field in the RWNXFEDSSSCCKCNTL register.
 */
__INLINE uint8_t riu_rxpathselfromreg_getf(void)
{
    uint32_t localVal = REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Sets the RXPATHSELFROMREG field of the RWNXFEDSSSCCKCNTL register.
 *
 * The RWNXFEDSSSCCKCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxpathselfromreg - The value to set the field to.
 */
__INLINE void riu_rxpathselfromreg_setf(uint8_t rxpathselfromreg)
{
    ASSERT_ERR((((uint32_t)rxpathselfromreg << 4) & ~((uint32_t)0x00000010)) == 0);
    REG_PL_WR(RIU_RWNXFEDSSSCCKCNTL_ADDR, (REG_PL_RD(RIU_RWNXFEDSSSCCKCNTL_ADDR) & ~((uint32_t)0x00000010)) | ((uint32_t)rxpathselfromreg << 4));
}

/// @}


#endif // _REG_RIU_H_

/// @}

