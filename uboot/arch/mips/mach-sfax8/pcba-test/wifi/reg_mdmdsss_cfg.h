/**
 * @file reg_mdmdsss_cfg.h
 * @brief Definitions of the MDM HW block registers and register access functions.
 *
 * @defgroup REG_MDMDSSS_CFG REG_MDMDSSS_CFG
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the MDM HW block registers and register access functions.
 */
#ifndef _REG_MDMDSSS_CFG_H_
#define _REG_MDMDSSS_CFG_H_

#include "co_int.h"
#include "_reg_mdmdsss_cfg.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_MDMDSSS_CFG peripheral.
 */
#define REG_MDMDSSS_CFG_COUNT 12

/** @brief Decoding mask of the REG_MDMDSSS_CFG peripheral registers from the CPU point of view.
 */
#define REG_MDMDSSS_CFG_DECODING_MASK 0x0000003F

/**
 * @name FEBCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:24               TXNORM   0x0
 *  21:16             MAXSTAGE   0x27
 *     03              FIRDISB   0
 *     02             GAINDISB   0
 *     01           INTERPDISB   0
 *     00           FIRCOEFSEL   0
 * </pre>
 *
 * @{
 */

/// Address of the FEBCNTL register
#define MDM_FEBCNTL_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3000)
/// Offset of the FEBCNTL register from the base address
#define MDM_FEBCNTL_OFFSET 0x00000000
/// Index of the FEBCNTL register
#define MDM_FEBCNTL_INDEX  0x00000000
/// Reset value of the FEBCNTL register
#define MDM_FEBCNTL_RESET  0x00270000

/**
 * @brief Returns the current value of the FEBCNTL register.
 * The FEBCNTL register will be read and its value returned.
 * @return The current value of the FEBCNTL register.
 */
__INLINE uint32_t mdm_febcntl_get(void)
{
    return REG_PL_RD(MDM_FEBCNTL_ADDR);
}

/**
 * @brief Sets the FEBCNTL register to a value.
 * The FEBCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_febcntl_set(uint32_t value)
{
    REG_PL_WR(MDM_FEBCNTL_ADDR, value);
}

// field definitions
/// TXNORM field mask
#define MDM_TXNORM_MASK       ((uint32_t)0x3F000000)
/// TXNORM field LSB position
#define MDM_TXNORM_LSB        24
/// TXNORM field width
#define MDM_TXNORM_WIDTH      ((uint32_t)0x00000006)
/// MAXSTAGE field mask
#define MDM_MAXSTAGE_MASK     ((uint32_t)0x003F0000)
/// MAXSTAGE field LSB position
#define MDM_MAXSTAGE_LSB      16
/// MAXSTAGE field width
#define MDM_MAXSTAGE_WIDTH    ((uint32_t)0x00000006)
/// FIRDISB field bit
#define MDM_FIRDISB_BIT       ((uint32_t)0x00000008)
/// FIRDISB field position
#define MDM_FIRDISB_POS       3
/// GAINDISB field bit
#define MDM_GAINDISB_BIT      ((uint32_t)0x00000004)
/// GAINDISB field position
#define MDM_GAINDISB_POS      2
/// INTERPDISB field bit
#define MDM_INTERPDISB_BIT    ((uint32_t)0x00000002)
/// INTERPDISB field position
#define MDM_INTERPDISB_POS    1
/// FIRCOEFSEL field bit
#define MDM_FIRCOEFSEL_BIT    ((uint32_t)0x00000001)
/// FIRCOEFSEL field position
#define MDM_FIRCOEFSEL_POS    0

/// TXNORM field reset value
#define MDM_TXNORM_RST        0x0
/// MAXSTAGE field reset value
#define MDM_MAXSTAGE_RST      0x27
/// FIRDISB field reset value
#define MDM_FIRDISB_RST       0x0
/// GAINDISB field reset value
#define MDM_GAINDISB_RST      0x0
/// INTERPDISB field reset value
#define MDM_INTERPDISB_RST    0x0
/// FIRCOEFSEL field reset value
#define MDM_FIRCOEFSEL_RST    0x0

/**
 * @brief Constructs a value for the FEBCNTL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] txnorm - The value to use for the TXNORM field.
 * @param[in] maxstage - The value to use for the MAXSTAGE field.
 * @param[in] firdisb - The value to use for the FIRDISB field.
 * @param[in] gaindisb - The value to use for the GAINDISB field.
 * @param[in] interpdisb - The value to use for the INTERPDISB field.
 * @param[in] fircoefsel - The value to use for the FIRCOEFSEL field.
 */
__INLINE void mdm_febcntl_pack(uint8_t txnorm, uint8_t maxstage, uint8_t firdisb, uint8_t gaindisb, uint8_t interpdisb, uint8_t fircoefsel)
{
    ASSERT_ERR((((uint32_t)txnorm << 24) & ~((uint32_t)0x3F000000)) == 0);
    ASSERT_ERR((((uint32_t)maxstage << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)firdisb << 3) & ~((uint32_t)0x00000008)) == 0);
    ASSERT_ERR((((uint32_t)gaindisb << 2) & ~((uint32_t)0x00000004)) == 0);
    ASSERT_ERR((((uint32_t)interpdisb << 1) & ~((uint32_t)0x00000002)) == 0);
    ASSERT_ERR((((uint32_t)fircoefsel << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR,  ((uint32_t)txnorm << 24) | ((uint32_t)maxstage << 16) | ((uint32_t)firdisb << 3) | ((uint32_t)gaindisb << 2) | ((uint32_t)interpdisb << 1) | ((uint32_t)fircoefsel << 0));
}

/**
 * @brief Unpacks FEBCNTL's fields from current value of the FEBCNTL register.
 *
 * Reads the FEBCNTL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] txnorm - Will be populated with the current value of this field from the register.
 * @param[out] maxstage - Will be populated with the current value of this field from the register.
 * @param[out] firdisb - Will be populated with the current value of this field from the register.
 * @param[out] gaindisb - Will be populated with the current value of this field from the register.
 * @param[out] interpdisb - Will be populated with the current value of this field from the register.
 * @param[out] fircoefsel - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_febcntl_unpack(uint8_t* txnorm, uint8_t* maxstage, uint8_t* firdisb, uint8_t* gaindisb, uint8_t* interpdisb, uint8_t* fircoefsel)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);

    *txnorm = (localVal & ((uint32_t)0x3F000000)) >> 24;
    *maxstage = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *firdisb = (localVal & ((uint32_t)0x00000008)) >> 3;
    *gaindisb = (localVal & ((uint32_t)0x00000004)) >> 2;
    *interpdisb = (localVal & ((uint32_t)0x00000002)) >> 1;
    *fircoefsel = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the TXNORM field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the TXNORM field's value will be returned.
 *
 * @return The current value of the TXNORM field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_txnorm_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x3F000000)) >> 24);
}

/**
 * @brief Sets the TXNORM field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txnorm - The value to set the field to.
 */
__INLINE void mdm_txnorm_setf(uint8_t txnorm)
{
    ASSERT_ERR((((uint32_t)txnorm << 24) & ~((uint32_t)0x3F000000)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x3F000000)) | ((uint32_t)txnorm << 24));
}

/**
 * @brief Returns the current value of the MAXSTAGE field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the MAXSTAGE field's value will be returned.
 *
 * @return The current value of the MAXSTAGE field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_maxstage_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the MAXSTAGE field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] maxstage - The value to set the field to.
 */
__INLINE void mdm_maxstage_setf(uint8_t maxstage)
{
    ASSERT_ERR((((uint32_t)maxstage << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)maxstage << 16));
}

/**
 * @brief Returns the current value of the FIRDISB field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the FIRDISB field's value will be returned.
 *
 * @return The current value of the FIRDISB field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_firdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Sets the FIRDISB field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] firdisb - The value to set the field to.
 */
__INLINE void mdm_firdisb_setf(uint8_t firdisb)
{
    ASSERT_ERR((((uint32_t)firdisb << 3) & ~((uint32_t)0x00000008)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x00000008)) | ((uint32_t)firdisb << 3));
}

/**
 * @brief Returns the current value of the GAINDISB field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the GAINDISB field's value will be returned.
 *
 * @return The current value of the GAINDISB field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_gaindisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Sets the GAINDISB field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] gaindisb - The value to set the field to.
 */
__INLINE void mdm_gaindisb_setf(uint8_t gaindisb)
{
    ASSERT_ERR((((uint32_t)gaindisb << 2) & ~((uint32_t)0x00000004)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x00000004)) | ((uint32_t)gaindisb << 2));
}

/**
 * @brief Returns the current value of the INTERPDISB field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the INTERPDISB field's value will be returned.
 *
 * @return The current value of the INTERPDISB field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_interpdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Sets the INTERPDISB field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] interpdisb - The value to set the field to.
 */
__INLINE void mdm_interpdisb_setf(uint8_t interpdisb)
{
    ASSERT_ERR((((uint32_t)interpdisb << 1) & ~((uint32_t)0x00000002)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x00000002)) | ((uint32_t)interpdisb << 1));
}

/**
 * @brief Returns the current value of the FIRCOEFSEL field in the FEBCNTL register.
 *
 * The FEBCNTL register will be read and the FIRCOEFSEL field's value will be returned.
 *
 * @return The current value of the FIRCOEFSEL field in the FEBCNTL register.
 */
__INLINE uint8_t mdm_fircoefsel_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FEBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/**
 * @brief Sets the FIRCOEFSEL field of the FEBCNTL register.
 *
 * The FEBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] fircoefsel - The value to set the field to.
 */
__INLINE void mdm_fircoefsel_setf(uint8_t fircoefsel)
{
    ASSERT_ERR((((uint32_t)fircoefsel << 0) & ~((uint32_t)0x00000001)) == 0);
    REG_PL_WR(MDM_FEBCNTL_ADDR, (REG_PL_RD(MDM_FEBCNTL_ADDR) & ~((uint32_t)0x00000001)) | ((uint32_t)fircoefsel << 0));
}

/// @}

/**
 * @name MDMBCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     28             IQMMDISB   0
 *     26          PRECOMPDISB   0
 *     25            DCOFFDISB   0
 *     24             COMPDISB   0
 *     23               EQDISB   0
 *     21           SPREADDISB   0
 *     20           SCRAMBDISB   0
 *  14:12               SFDERR   0x0
 *  10:08               SFDLEN   0x0
 *  05:00               PREPRE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the MDMBCNTL register
#define MDM_MDMBCNTL_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3004)
/// Offset of the MDMBCNTL register from the base address
#define MDM_MDMBCNTL_OFFSET 0x00000004
/// Index of the MDMBCNTL register
#define MDM_MDMBCNTL_INDEX  0x00000001
/// Reset value of the MDMBCNTL register
#define MDM_MDMBCNTL_RESET  0x00000000

/**
 * @brief Returns the current value of the MDMBCNTL register.
 * The MDMBCNTL register will be read and its value returned.
 * @return The current value of the MDMBCNTL register.
 */
__INLINE uint32_t mdm_mdmbcntl_get(void)
{
    return REG_PL_RD(MDM_MDMBCNTL_ADDR);
}

/**
 * @brief Sets the MDMBCNTL register to a value.
 * The MDMBCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbcntl_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBCNTL_ADDR, value);
}

// field definitions
/// IQMMDISB field bit
#define MDM_IQMMDISB_BIT       ((uint32_t)0x10000000)
/// IQMMDISB field position
#define MDM_IQMMDISB_POS       28
/// PRECOMPDISB field bit
#define MDM_PRECOMPDISB_BIT    ((uint32_t)0x04000000)
/// PRECOMPDISB field position
#define MDM_PRECOMPDISB_POS    26
/// DCOFFDISB field bit
#define MDM_DCOFFDISB_BIT      ((uint32_t)0x02000000)
/// DCOFFDISB field position
#define MDM_DCOFFDISB_POS      25
/// COMPDISB field bit
#define MDM_COMPDISB_BIT       ((uint32_t)0x01000000)
/// COMPDISB field position
#define MDM_COMPDISB_POS       24
/// EQDISB field bit
#define MDM_EQDISB_BIT         ((uint32_t)0x00800000)
/// EQDISB field position
#define MDM_EQDISB_POS         23
/// SPREADDISB field bit
#define MDM_SPREADDISB_BIT     ((uint32_t)0x00200000)
/// SPREADDISB field position
#define MDM_SPREADDISB_POS     21
/// SCRAMBDISB field bit
#define MDM_SCRAMBDISB_BIT     ((uint32_t)0x00100000)
/// SCRAMBDISB field position
#define MDM_SCRAMBDISB_POS     20
/// SFDERR field mask
#define MDM_SFDERR_MASK        ((uint32_t)0x00007000)
/// SFDERR field LSB position
#define MDM_SFDERR_LSB         12
/// SFDERR field width
#define MDM_SFDERR_WIDTH       ((uint32_t)0x00000003)
/// SFDLEN field mask
#define MDM_SFDLEN_MASK        ((uint32_t)0x00000700)
/// SFDLEN field LSB position
#define MDM_SFDLEN_LSB         8
/// SFDLEN field width
#define MDM_SFDLEN_WIDTH       ((uint32_t)0x00000003)
/// PREPRE field mask
#define MDM_PREPRE_MASK        ((uint32_t)0x0000003F)
/// PREPRE field LSB position
#define MDM_PREPRE_LSB         0
/// PREPRE field width
#define MDM_PREPRE_WIDTH       ((uint32_t)0x00000006)

/// IQMMDISB field reset value
#define MDM_IQMMDISB_RST       0x0
/// PRECOMPDISB field reset value
#define MDM_PRECOMPDISB_RST    0x0
/// DCOFFDISB field reset value
#define MDM_DCOFFDISB_RST      0x0
/// COMPDISB field reset value
#define MDM_COMPDISB_RST       0x0
/// EQDISB field reset value
#define MDM_EQDISB_RST         0x0
/// SPREADDISB field reset value
#define MDM_SPREADDISB_RST     0x0
/// SCRAMBDISB field reset value
#define MDM_SCRAMBDISB_RST     0x0
/// SFDERR field reset value
#define MDM_SFDERR_RST         0x0
/// SFDLEN field reset value
#define MDM_SFDLEN_RST         0x0
/// PREPRE field reset value
#define MDM_PREPRE_RST         0x0

/**
 * @brief Constructs a value for the MDMBCNTL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] iqmmdisb - The value to use for the IQMMDISB field.
 * @param[in] precompdisb - The value to use for the PRECOMPDISB field.
 * @param[in] dcoffdisb - The value to use for the DCOFFDISB field.
 * @param[in] compdisb - The value to use for the COMPDISB field.
 * @param[in] eqdisb - The value to use for the EQDISB field.
 * @param[in] spreaddisb - The value to use for the SPREADDISB field.
 * @param[in] scrambdisb - The value to use for the SCRAMBDISB field.
 * @param[in] sfderr - The value to use for the SFDERR field.
 * @param[in] sfdlen - The value to use for the SFDLEN field.
 * @param[in] prepre - The value to use for the PREPRE field.
 */
__INLINE void mdm_mdmbcntl_pack(uint8_t iqmmdisb, uint8_t precompdisb, uint8_t dcoffdisb, uint8_t compdisb, uint8_t eqdisb, uint8_t spreaddisb, uint8_t scrambdisb, uint8_t sfderr, uint8_t sfdlen, uint8_t prepre)
{
    ASSERT_ERR((((uint32_t)iqmmdisb << 28) & ~((uint32_t)0x10000000)) == 0);
    ASSERT_ERR((((uint32_t)precompdisb << 26) & ~((uint32_t)0x04000000)) == 0);
    ASSERT_ERR((((uint32_t)dcoffdisb << 25) & ~((uint32_t)0x02000000)) == 0);
    ASSERT_ERR((((uint32_t)compdisb << 24) & ~((uint32_t)0x01000000)) == 0);
    ASSERT_ERR((((uint32_t)eqdisb << 23) & ~((uint32_t)0x00800000)) == 0);
    ASSERT_ERR((((uint32_t)spreaddisb << 21) & ~((uint32_t)0x00200000)) == 0);
    ASSERT_ERR((((uint32_t)scrambdisb << 20) & ~((uint32_t)0x00100000)) == 0);
    ASSERT_ERR((((uint32_t)sfderr << 12) & ~((uint32_t)0x00007000)) == 0);
    ASSERT_ERR((((uint32_t)sfdlen << 8) & ~((uint32_t)0x00000700)) == 0);
    ASSERT_ERR((((uint32_t)prepre << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR,  ((uint32_t)iqmmdisb << 28) | ((uint32_t)precompdisb << 26) | ((uint32_t)dcoffdisb << 25) | ((uint32_t)compdisb << 24) | ((uint32_t)eqdisb << 23) | ((uint32_t)spreaddisb << 21) | ((uint32_t)scrambdisb << 20) | ((uint32_t)sfderr << 12) | ((uint32_t)sfdlen << 8) | ((uint32_t)prepre << 0));
}

/**
 * @brief Unpacks MDMBCNTL's fields from current value of the MDMBCNTL register.
 *
 * Reads the MDMBCNTL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] iqmmdisb - Will be populated with the current value of this field from the register.
 * @param[out] precompdisb - Will be populated with the current value of this field from the register.
 * @param[out] dcoffdisb - Will be populated with the current value of this field from the register.
 * @param[out] compdisb - Will be populated with the current value of this field from the register.
 * @param[out] eqdisb - Will be populated with the current value of this field from the register.
 * @param[out] spreaddisb - Will be populated with the current value of this field from the register.
 * @param[out] scrambdisb - Will be populated with the current value of this field from the register.
 * @param[out] sfderr - Will be populated with the current value of this field from the register.
 * @param[out] sfdlen - Will be populated with the current value of this field from the register.
 * @param[out] prepre - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbcntl_unpack(uint8_t* iqmmdisb, uint8_t* precompdisb, uint8_t* dcoffdisb, uint8_t* compdisb, uint8_t* eqdisb, uint8_t* spreaddisb, uint8_t* scrambdisb, uint8_t* sfderr, uint8_t* sfdlen, uint8_t* prepre)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);

    *iqmmdisb = (localVal & ((uint32_t)0x10000000)) >> 28;
    *precompdisb = (localVal & ((uint32_t)0x04000000)) >> 26;
    *dcoffdisb = (localVal & ((uint32_t)0x02000000)) >> 25;
    *compdisb = (localVal & ((uint32_t)0x01000000)) >> 24;
    *eqdisb = (localVal & ((uint32_t)0x00800000)) >> 23;
    *spreaddisb = (localVal & ((uint32_t)0x00200000)) >> 21;
    *scrambdisb = (localVal & ((uint32_t)0x00100000)) >> 20;
    *sfderr = (localVal & ((uint32_t)0x00007000)) >> 12;
    *sfdlen = (localVal & ((uint32_t)0x00000700)) >> 8;
    *prepre = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the IQMMDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the IQMMDISB field's value will be returned.
 *
 * @return The current value of the IQMMDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_iqmmdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the IQMMDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] iqmmdisb - The value to set the field to.
 */
__INLINE void mdm_iqmmdisb_setf(uint8_t iqmmdisb)
{
    ASSERT_ERR((((uint32_t)iqmmdisb << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x10000000)) | ((uint32_t)iqmmdisb << 28));
}

/**
 * @brief Returns the current value of the PRECOMPDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the PRECOMPDISB field's value will be returned.
 *
 * @return The current value of the PRECOMPDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_precompdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x04000000)) >> 26);
}

/**
 * @brief Sets the PRECOMPDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] precompdisb - The value to set the field to.
 */
__INLINE void mdm_precompdisb_setf(uint8_t precompdisb)
{
    ASSERT_ERR((((uint32_t)precompdisb << 26) & ~((uint32_t)0x04000000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x04000000)) | ((uint32_t)precompdisb << 26));
}

/**
 * @brief Returns the current value of the DCOFFDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the DCOFFDISB field's value will be returned.
 *
 * @return The current value of the DCOFFDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_dcoffdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x02000000)) >> 25);
}

/**
 * @brief Sets the DCOFFDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] dcoffdisb - The value to set the field to.
 */
__INLINE void mdm_dcoffdisb_setf(uint8_t dcoffdisb)
{
    ASSERT_ERR((((uint32_t)dcoffdisb << 25) & ~((uint32_t)0x02000000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x02000000)) | ((uint32_t)dcoffdisb << 25));
}

/**
 * @brief Returns the current value of the COMPDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the COMPDISB field's value will be returned.
 *
 * @return The current value of the COMPDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_compdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x01000000)) >> 24);
}

/**
 * @brief Sets the COMPDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] compdisb - The value to set the field to.
 */
__INLINE void mdm_compdisb_setf(uint8_t compdisb)
{
    ASSERT_ERR((((uint32_t)compdisb << 24) & ~((uint32_t)0x01000000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x01000000)) | ((uint32_t)compdisb << 24));
}

/**
 * @brief Returns the current value of the EQDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the EQDISB field's value will be returned.
 *
 * @return The current value of the EQDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_eqdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Sets the EQDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] eqdisb - The value to set the field to.
 */
__INLINE void mdm_eqdisb_setf(uint8_t eqdisb)
{
    ASSERT_ERR((((uint32_t)eqdisb << 23) & ~((uint32_t)0x00800000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x00800000)) | ((uint32_t)eqdisb << 23));
}

/**
 * @brief Returns the current value of the SPREADDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the SPREADDISB field's value will be returned.
 *
 * @return The current value of the SPREADDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_spreaddisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00200000)) >> 21);
}

/**
 * @brief Sets the SPREADDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] spreaddisb - The value to set the field to.
 */
__INLINE void mdm_spreaddisb_setf(uint8_t spreaddisb)
{
    ASSERT_ERR((((uint32_t)spreaddisb << 21) & ~((uint32_t)0x00200000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x00200000)) | ((uint32_t)spreaddisb << 21));
}

/**
 * @brief Returns the current value of the SCRAMBDISB field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the SCRAMBDISB field's value will be returned.
 *
 * @return The current value of the SCRAMBDISB field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_scrambdisb_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Sets the SCRAMBDISB field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] scrambdisb - The value to set the field to.
 */
__INLINE void mdm_scrambdisb_setf(uint8_t scrambdisb)
{
    ASSERT_ERR((((uint32_t)scrambdisb << 20) & ~((uint32_t)0x00100000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x00100000)) | ((uint32_t)scrambdisb << 20));
}

/**
 * @brief Returns the current value of the SFDERR field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the SFDERR field's value will be returned.
 *
 * @return The current value of the SFDERR field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_sfderr_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00007000)) >> 12);
}

/**
 * @brief Sets the SFDERR field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] sfderr - The value to set the field to.
 */
__INLINE void mdm_sfderr_setf(uint8_t sfderr)
{
    ASSERT_ERR((((uint32_t)sfderr << 12) & ~((uint32_t)0x00007000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x00007000)) | ((uint32_t)sfderr << 12));
}

/**
 * @brief Returns the current value of the SFDLEN field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the SFDLEN field's value will be returned.
 *
 * @return The current value of the SFDLEN field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_sfdlen_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00000700)) >> 8);
}

/**
 * @brief Sets the SFDLEN field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] sfdlen - The value to set the field to.
 */
__INLINE void mdm_sfdlen_setf(uint8_t sfdlen)
{
    ASSERT_ERR((((uint32_t)sfdlen << 8) & ~((uint32_t)0x00000700)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x00000700)) | ((uint32_t)sfdlen << 8));
}

/**
 * @brief Returns the current value of the PREPRE field in the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read and the PREPRE field's value will be returned.
 *
 * @return The current value of the PREPRE field in the MDMBCNTL register.
 */
__INLINE uint8_t mdm_prepre_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/**
 * @brief Sets the PREPRE field of the MDMBCNTL register.
 *
 * The MDMBCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] prepre - The value to set the field to.
 */
__INLINE void mdm_prepre_setf(uint8_t prepre)
{
    ASSERT_ERR((((uint32_t)prepre << 0) & ~((uint32_t)0x0000003F)) == 0);
    REG_PL_WR(MDM_MDMBCNTL_ADDR, (REG_PL_RD(MDM_MDMBCNTL_ADDR) & ~((uint32_t)0x0000003F)) | ((uint32_t)prepre << 0));
}

/// @}

/**
 * @name MDMBPRMINIT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  21:20                  RHO   0x0
 *  17:16                   MU   0x1
 *  05:04                 BETA   0x2
 *  01:00                ALPHA   0x2
 * </pre>
 *
 * @{
 */

/// Address of the MDMBPRMINIT register
#define MDM_MDMBPRMINIT_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3008)
/// Offset of the MDMBPRMINIT register from the base address
#define MDM_MDMBPRMINIT_OFFSET 0x00000008
/// Index of the MDMBPRMINIT register
#define MDM_MDMBPRMINIT_INDEX  0x00000002
/// Reset value of the MDMBPRMINIT register
#define MDM_MDMBPRMINIT_RESET  0x00010022

/**
 * @brief Returns the current value of the MDMBPRMINIT register.
 * The MDMBPRMINIT register will be read and its value returned.
 * @return The current value of the MDMBPRMINIT register.
 */
__INLINE uint32_t mdm_mdmbprminit_get(void)
{
    return REG_PL_RD(MDM_MDMBPRMINIT_ADDR);
}

/**
 * @brief Sets the MDMBPRMINIT register to a value.
 * The MDMBPRMINIT register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbprminit_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR, value);
}

// field definitions
/// RHO field mask
#define MDM_RHO_MASK     ((uint32_t)0x00300000)
/// RHO field LSB position
#define MDM_RHO_LSB      20
/// RHO field width
#define MDM_RHO_WIDTH    ((uint32_t)0x00000002)
/// MU field mask
#define MDM_MU_MASK      ((uint32_t)0x00030000)
/// MU field LSB position
#define MDM_MU_LSB       16
/// MU field width
#define MDM_MU_WIDTH     ((uint32_t)0x00000002)
/// BETA field mask
#define MDM_BETA_MASK    ((uint32_t)0x00000030)
/// BETA field LSB position
#define MDM_BETA_LSB     4
/// BETA field width
#define MDM_BETA_WIDTH   ((uint32_t)0x00000002)
/// ALPHA field mask
#define MDM_ALPHA_MASK   ((uint32_t)0x00000003)
/// ALPHA field LSB position
#define MDM_ALPHA_LSB    0
/// ALPHA field width
#define MDM_ALPHA_WIDTH  ((uint32_t)0x00000002)

/// RHO field reset value
#define MDM_RHO_RST      0x0
/// MU field reset value
#define MDM_MU_RST       0x1
/// BETA field reset value
#define MDM_BETA_RST     0x2
/// ALPHA field reset value
#define MDM_ALPHA_RST    0x2

/**
 * @brief Constructs a value for the MDMBPRMINIT register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rho - The value to use for the RHO field.
 * @param[in] mu - The value to use for the MU field.
 * @param[in] beta - The value to use for the BETA field.
 * @param[in] alpha - The value to use for the ALPHA field.
 */
__INLINE void mdm_mdmbprminit_pack(uint8_t rho, uint8_t mu, uint8_t beta, uint8_t alpha)
{
    ASSERT_ERR((((uint32_t)rho << 20) & ~((uint32_t)0x00300000)) == 0);
    ASSERT_ERR((((uint32_t)mu << 16) & ~((uint32_t)0x00030000)) == 0);
    ASSERT_ERR((((uint32_t)beta << 4) & ~((uint32_t)0x00000030)) == 0);
    ASSERT_ERR((((uint32_t)alpha << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR,  ((uint32_t)rho << 20) | ((uint32_t)mu << 16) | ((uint32_t)beta << 4) | ((uint32_t)alpha << 0));
}

/**
 * @brief Unpacks MDMBPRMINIT's fields from current value of the MDMBPRMINIT register.
 *
 * Reads the MDMBPRMINIT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rho - Will be populated with the current value of this field from the register.
 * @param[out] mu - Will be populated with the current value of this field from the register.
 * @param[out] beta - Will be populated with the current value of this field from the register.
 * @param[out] alpha - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbprminit_unpack(uint8_t* rho, uint8_t* mu, uint8_t* beta, uint8_t* alpha)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBPRMINIT_ADDR);

    *rho = (localVal & ((uint32_t)0x00300000)) >> 20;
    *mu = (localVal & ((uint32_t)0x00030000)) >> 16;
    *beta = (localVal & ((uint32_t)0x00000030)) >> 4;
    *alpha = (localVal & ((uint32_t)0x00000003)) >> 0;
}

/**
 * @brief Returns the current value of the RHO field in the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read and the RHO field's value will be returned.
 *
 * @return The current value of the RHO field in the MDMBPRMINIT register.
 */
__INLINE uint8_t mdm_rho_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBPRMINIT_ADDR);
    return ((localVal & ((uint32_t)0x00300000)) >> 20);
}

/**
 * @brief Sets the RHO field of the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rho - The value to set the field to.
 */
__INLINE void mdm_rho_setf(uint8_t rho)
{
    ASSERT_ERR((((uint32_t)rho << 20) & ~((uint32_t)0x00300000)) == 0);
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR, (REG_PL_RD(MDM_MDMBPRMINIT_ADDR) & ~((uint32_t)0x00300000)) | ((uint32_t)rho << 20));
}

/**
 * @brief Returns the current value of the MU field in the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read and the MU field's value will be returned.
 *
 * @return The current value of the MU field in the MDMBPRMINIT register.
 */
__INLINE uint8_t mdm_mu_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBPRMINIT_ADDR);
    return ((localVal & ((uint32_t)0x00030000)) >> 16);
}

/**
 * @brief Sets the MU field of the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] mu - The value to set the field to.
 */
__INLINE void mdm_mu_setf(uint8_t mu)
{
    ASSERT_ERR((((uint32_t)mu << 16) & ~((uint32_t)0x00030000)) == 0);
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR, (REG_PL_RD(MDM_MDMBPRMINIT_ADDR) & ~((uint32_t)0x00030000)) | ((uint32_t)mu << 16));
}

/**
 * @brief Returns the current value of the BETA field in the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read and the BETA field's value will be returned.
 *
 * @return The current value of the BETA field in the MDMBPRMINIT register.
 */
__INLINE uint8_t mdm_beta_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBPRMINIT_ADDR);
    return ((localVal & ((uint32_t)0x00000030)) >> 4);
}

/**
 * @brief Sets the BETA field of the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] beta - The value to set the field to.
 */
__INLINE void mdm_beta_setf(uint8_t beta)
{
    ASSERT_ERR((((uint32_t)beta << 4) & ~((uint32_t)0x00000030)) == 0);
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR, (REG_PL_RD(MDM_MDMBPRMINIT_ADDR) & ~((uint32_t)0x00000030)) | ((uint32_t)beta << 4));
}

/**
 * @brief Returns the current value of the ALPHA field in the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read and the ALPHA field's value will be returned.
 *
 * @return The current value of the ALPHA field in the MDMBPRMINIT register.
 */
__INLINE uint8_t mdm_alpha_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBPRMINIT_ADDR);
    return ((localVal & ((uint32_t)0x00000003)) >> 0);
}

/**
 * @brief Sets the ALPHA field of the MDMBPRMINIT register.
 *
 * The MDMBPRMINIT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] alpha - The value to set the field to.
 */
__INLINE void mdm_alpha_setf(uint8_t alpha)
{
    ASSERT_ERR((((uint32_t)alpha << 0) & ~((uint32_t)0x00000003)) == 0);
    REG_PL_WR(MDM_MDMBPRMINIT_ADDR, (REG_PL_RD(MDM_MDMBPRMINIT_ADDR) & ~((uint32_t)0x00000003)) | ((uint32_t)alpha << 0));
}

/// @}

/**
 * @name MDMBTALPHA register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:12              TALPHA3   0x6
 *  11:08              TALPHA2   0x2
 *  07:04              TALPHA1   0x3
 *  03:00              TALPHA0   0x6
 * </pre>
 *
 * @{
 */

/// Address of the MDMBTALPHA register
#define MDM_MDMBTALPHA_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x300C)
/// Offset of the MDMBTALPHA register from the base address
#define MDM_MDMBTALPHA_OFFSET 0x0000000C
/// Index of the MDMBTALPHA register
#define MDM_MDMBTALPHA_INDEX  0x00000003
/// Reset value of the MDMBTALPHA register
#define MDM_MDMBTALPHA_RESET  0x00006236

/**
 * @brief Returns the current value of the MDMBTALPHA register.
 * The MDMBTALPHA register will be read and its value returned.
 * @return The current value of the MDMBTALPHA register.
 */
__INLINE uint32_t mdm_mdmbtalpha_get(void)
{
    return REG_PL_RD(MDM_MDMBTALPHA_ADDR);
}

/**
 * @brief Sets the MDMBTALPHA register to a value.
 * The MDMBTALPHA register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbtalpha_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBTALPHA_ADDR, value);
}

// field definitions
/// TALPHA3 field mask
#define MDM_TALPHA3_MASK   ((uint32_t)0x0000F000)
/// TALPHA3 field LSB position
#define MDM_TALPHA3_LSB    12
/// TALPHA3 field width
#define MDM_TALPHA3_WIDTH  ((uint32_t)0x00000004)
/// TALPHA2 field mask
#define MDM_TALPHA2_MASK   ((uint32_t)0x00000F00)
/// TALPHA2 field LSB position
#define MDM_TALPHA2_LSB    8
/// TALPHA2 field width
#define MDM_TALPHA2_WIDTH  ((uint32_t)0x00000004)
/// TALPHA1 field mask
#define MDM_TALPHA1_MASK   ((uint32_t)0x000000F0)
/// TALPHA1 field LSB position
#define MDM_TALPHA1_LSB    4
/// TALPHA1 field width
#define MDM_TALPHA1_WIDTH  ((uint32_t)0x00000004)
/// TALPHA0 field mask
#define MDM_TALPHA0_MASK   ((uint32_t)0x0000000F)
/// TALPHA0 field LSB position
#define MDM_TALPHA0_LSB    0
/// TALPHA0 field width
#define MDM_TALPHA0_WIDTH  ((uint32_t)0x00000004)

/// TALPHA3 field reset value
#define MDM_TALPHA3_RST    0x6
/// TALPHA2 field reset value
#define MDM_TALPHA2_RST    0x2
/// TALPHA1 field reset value
#define MDM_TALPHA1_RST    0x3
/// TALPHA0 field reset value
#define MDM_TALPHA0_RST    0x6

/**
 * @brief Constructs a value for the MDMBTALPHA register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] talpha3 - The value to use for the TALPHA3 field.
 * @param[in] talpha2 - The value to use for the TALPHA2 field.
 * @param[in] talpha1 - The value to use for the TALPHA1 field.
 * @param[in] talpha0 - The value to use for the TALPHA0 field.
 */
__INLINE void mdm_mdmbtalpha_pack(uint8_t talpha3, uint8_t talpha2, uint8_t talpha1, uint8_t talpha0)
{
    ASSERT_ERR((((uint32_t)talpha3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    ASSERT_ERR((((uint32_t)talpha2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)talpha1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)talpha0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTALPHA_ADDR,  ((uint32_t)talpha3 << 12) | ((uint32_t)talpha2 << 8) | ((uint32_t)talpha1 << 4) | ((uint32_t)talpha0 << 0));
}

/**
 * @brief Unpacks MDMBTALPHA's fields from current value of the MDMBTALPHA register.
 *
 * Reads the MDMBTALPHA register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] talpha3 - Will be populated with the current value of this field from the register.
 * @param[out] talpha2 - Will be populated with the current value of this field from the register.
 * @param[out] talpha1 - Will be populated with the current value of this field from the register.
 * @param[out] talpha0 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbtalpha_unpack(uint8_t* talpha3, uint8_t* talpha2, uint8_t* talpha1, uint8_t* talpha0)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTALPHA_ADDR);

    *talpha3 = (localVal & ((uint32_t)0x0000F000)) >> 12;
    *talpha2 = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *talpha1 = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *talpha0 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the TALPHA3 field in the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read and the TALPHA3 field's value will be returned.
 *
 * @return The current value of the TALPHA3 field in the MDMBTALPHA register.
 */
__INLINE uint8_t mdm_talpha3_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTALPHA_ADDR);
    return ((localVal & ((uint32_t)0x0000F000)) >> 12);
}

/**
 * @brief Sets the TALPHA3 field of the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] talpha3 - The value to set the field to.
 */
__INLINE void mdm_talpha3_setf(uint8_t talpha3)
{
    ASSERT_ERR((((uint32_t)talpha3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    REG_PL_WR(MDM_MDMBTALPHA_ADDR, (REG_PL_RD(MDM_MDMBTALPHA_ADDR) & ~((uint32_t)0x0000F000)) | ((uint32_t)talpha3 << 12));
}

/**
 * @brief Returns the current value of the TALPHA2 field in the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read and the TALPHA2 field's value will be returned.
 *
 * @return The current value of the TALPHA2 field in the MDMBTALPHA register.
 */
__INLINE uint8_t mdm_talpha2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTALPHA_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the TALPHA2 field of the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] talpha2 - The value to set the field to.
 */
__INLINE void mdm_talpha2_setf(uint8_t talpha2)
{
    ASSERT_ERR((((uint32_t)talpha2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(MDM_MDMBTALPHA_ADDR, (REG_PL_RD(MDM_MDMBTALPHA_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)talpha2 << 8));
}

/**
 * @brief Returns the current value of the TALPHA1 field in the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read and the TALPHA1 field's value will be returned.
 *
 * @return The current value of the TALPHA1 field in the MDMBTALPHA register.
 */
__INLINE uint8_t mdm_talpha1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTALPHA_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Sets the TALPHA1 field of the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] talpha1 - The value to set the field to.
 */
__INLINE void mdm_talpha1_setf(uint8_t talpha1)
{
    ASSERT_ERR((((uint32_t)talpha1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_PL_WR(MDM_MDMBTALPHA_ADDR, (REG_PL_RD(MDM_MDMBTALPHA_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)talpha1 << 4));
}

/**
 * @brief Returns the current value of the TALPHA0 field in the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read and the TALPHA0 field's value will be returned.
 *
 * @return The current value of the TALPHA0 field in the MDMBTALPHA register.
 */
__INLINE uint8_t mdm_talpha0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTALPHA_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the TALPHA0 field of the MDMBTALPHA register.
 *
 * The MDMBTALPHA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] talpha0 - The value to set the field to.
 */
__INLINE void mdm_talpha0_setf(uint8_t talpha0)
{
    ASSERT_ERR((((uint32_t)talpha0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTALPHA_ADDR, (REG_PL_RD(MDM_MDMBTALPHA_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)talpha0 << 0));
}

/// @}

/**
 * @name MDMBTBETA register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:12               TBETA3   0x6
 *  11:08               TBETA2   0x2
 *  07:04               TBETA1   0x3
 *  03:00               TBETA0   0x6
 * </pre>
 *
 * @{
 */

/// Address of the MDMBTBETA register
#define MDM_MDMBTBETA_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3010)
/// Offset of the MDMBTBETA register from the base address
#define MDM_MDMBTBETA_OFFSET 0x00000010
/// Index of the MDMBTBETA register
#define MDM_MDMBTBETA_INDEX  0x00000004
/// Reset value of the MDMBTBETA register
#define MDM_MDMBTBETA_RESET  0x00006236

/**
 * @brief Returns the current value of the MDMBTBETA register.
 * The MDMBTBETA register will be read and its value returned.
 * @return The current value of the MDMBTBETA register.
 */
__INLINE uint32_t mdm_mdmbtbeta_get(void)
{
    return REG_PL_RD(MDM_MDMBTBETA_ADDR);
}

/**
 * @brief Sets the MDMBTBETA register to a value.
 * The MDMBTBETA register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbtbeta_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBTBETA_ADDR, value);
}

// field definitions
/// TBETA3 field mask
#define MDM_TBETA3_MASK   ((uint32_t)0x0000F000)
/// TBETA3 field LSB position
#define MDM_TBETA3_LSB    12
/// TBETA3 field width
#define MDM_TBETA3_WIDTH  ((uint32_t)0x00000004)
/// TBETA2 field mask
#define MDM_TBETA2_MASK   ((uint32_t)0x00000F00)
/// TBETA2 field LSB position
#define MDM_TBETA2_LSB    8
/// TBETA2 field width
#define MDM_TBETA2_WIDTH  ((uint32_t)0x00000004)
/// TBETA1 field mask
#define MDM_TBETA1_MASK   ((uint32_t)0x000000F0)
/// TBETA1 field LSB position
#define MDM_TBETA1_LSB    4
/// TBETA1 field width
#define MDM_TBETA1_WIDTH  ((uint32_t)0x00000004)
/// TBETA0 field mask
#define MDM_TBETA0_MASK   ((uint32_t)0x0000000F)
/// TBETA0 field LSB position
#define MDM_TBETA0_LSB    0
/// TBETA0 field width
#define MDM_TBETA0_WIDTH  ((uint32_t)0x00000004)

/// TBETA3 field reset value
#define MDM_TBETA3_RST    0x6
/// TBETA2 field reset value
#define MDM_TBETA2_RST    0x2
/// TBETA1 field reset value
#define MDM_TBETA1_RST    0x3
/// TBETA0 field reset value
#define MDM_TBETA0_RST    0x6

/**
 * @brief Constructs a value for the MDMBTBETA register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tbeta3 - The value to use for the TBETA3 field.
 * @param[in] tbeta2 - The value to use for the TBETA2 field.
 * @param[in] tbeta1 - The value to use for the TBETA1 field.
 * @param[in] tbeta0 - The value to use for the TBETA0 field.
 */
__INLINE void mdm_mdmbtbeta_pack(uint8_t tbeta3, uint8_t tbeta2, uint8_t tbeta1, uint8_t tbeta0)
{
    ASSERT_ERR((((uint32_t)tbeta3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    ASSERT_ERR((((uint32_t)tbeta2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)tbeta1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)tbeta0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTBETA_ADDR,  ((uint32_t)tbeta3 << 12) | ((uint32_t)tbeta2 << 8) | ((uint32_t)tbeta1 << 4) | ((uint32_t)tbeta0 << 0));
}

/**
 * @brief Unpacks MDMBTBETA's fields from current value of the MDMBTBETA register.
 *
 * Reads the MDMBTBETA register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tbeta3 - Will be populated with the current value of this field from the register.
 * @param[out] tbeta2 - Will be populated with the current value of this field from the register.
 * @param[out] tbeta1 - Will be populated with the current value of this field from the register.
 * @param[out] tbeta0 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbtbeta_unpack(uint8_t* tbeta3, uint8_t* tbeta2, uint8_t* tbeta1, uint8_t* tbeta0)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTBETA_ADDR);

    *tbeta3 = (localVal & ((uint32_t)0x0000F000)) >> 12;
    *tbeta2 = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *tbeta1 = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *tbeta0 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the TBETA3 field in the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read and the TBETA3 field's value will be returned.
 *
 * @return The current value of the TBETA3 field in the MDMBTBETA register.
 */
__INLINE uint8_t mdm_tbeta3_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTBETA_ADDR);
    return ((localVal & ((uint32_t)0x0000F000)) >> 12);
}

/**
 * @brief Sets the TBETA3 field of the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbeta3 - The value to set the field to.
 */
__INLINE void mdm_tbeta3_setf(uint8_t tbeta3)
{
    ASSERT_ERR((((uint32_t)tbeta3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    REG_PL_WR(MDM_MDMBTBETA_ADDR, (REG_PL_RD(MDM_MDMBTBETA_ADDR) & ~((uint32_t)0x0000F000)) | ((uint32_t)tbeta3 << 12));
}

/**
 * @brief Returns the current value of the TBETA2 field in the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read and the TBETA2 field's value will be returned.
 *
 * @return The current value of the TBETA2 field in the MDMBTBETA register.
 */
__INLINE uint8_t mdm_tbeta2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTBETA_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the TBETA2 field of the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbeta2 - The value to set the field to.
 */
__INLINE void mdm_tbeta2_setf(uint8_t tbeta2)
{
    ASSERT_ERR((((uint32_t)tbeta2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(MDM_MDMBTBETA_ADDR, (REG_PL_RD(MDM_MDMBTBETA_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)tbeta2 << 8));
}

/**
 * @brief Returns the current value of the TBETA1 field in the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read and the TBETA1 field's value will be returned.
 *
 * @return The current value of the TBETA1 field in the MDMBTBETA register.
 */
__INLINE uint8_t mdm_tbeta1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTBETA_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Sets the TBETA1 field of the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbeta1 - The value to set the field to.
 */
__INLINE void mdm_tbeta1_setf(uint8_t tbeta1)
{
    ASSERT_ERR((((uint32_t)tbeta1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_PL_WR(MDM_MDMBTBETA_ADDR, (REG_PL_RD(MDM_MDMBTBETA_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)tbeta1 << 4));
}

/**
 * @brief Returns the current value of the TBETA0 field in the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read and the TBETA0 field's value will be returned.
 *
 * @return The current value of the TBETA0 field in the MDMBTBETA register.
 */
__INLINE uint8_t mdm_tbeta0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTBETA_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the TBETA0 field of the MDMBTBETA register.
 *
 * The MDMBTBETA register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tbeta0 - The value to set the field to.
 */
__INLINE void mdm_tbeta0_setf(uint8_t tbeta0)
{
    ASSERT_ERR((((uint32_t)tbeta0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTBETA_ADDR, (REG_PL_RD(MDM_MDMBTBETA_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)tbeta0 << 0));
}

/// @}

/**
 * @name MDMBTMU register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:12                 TMU3   0x5
 *  11:08                 TMU2   0x5
 *  07:04                 TMU1   0x5
 *  03:00                 TMU0   0x5
 * </pre>
 *
 * @{
 */

/// Address of the MDMBTMU register
#define MDM_MDMBTMU_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3014)
/// Offset of the MDMBTMU register from the base address
#define MDM_MDMBTMU_OFFSET 0x00000014
/// Index of the MDMBTMU register
#define MDM_MDMBTMU_INDEX  0x00000005
/// Reset value of the MDMBTMU register
#define MDM_MDMBTMU_RESET  0x00005555

/**
 * @brief Returns the current value of the MDMBTMU register.
 * The MDMBTMU register will be read and its value returned.
 * @return The current value of the MDMBTMU register.
 */
__INLINE uint32_t mdm_mdmbtmu_get(void)
{
    return REG_PL_RD(MDM_MDMBTMU_ADDR);
}

/**
 * @brief Sets the MDMBTMU register to a value.
 * The MDMBTMU register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbtmu_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBTMU_ADDR, value);
}

// field definitions
/// TMU3 field mask
#define MDM_TMU3_MASK   ((uint32_t)0x0000F000)
/// TMU3 field LSB position
#define MDM_TMU3_LSB    12
/// TMU3 field width
#define MDM_TMU3_WIDTH  ((uint32_t)0x00000004)
/// TMU2 field mask
#define MDM_TMU2_MASK   ((uint32_t)0x00000F00)
/// TMU2 field LSB position
#define MDM_TMU2_LSB    8
/// TMU2 field width
#define MDM_TMU2_WIDTH  ((uint32_t)0x00000004)
/// TMU1 field mask
#define MDM_TMU1_MASK   ((uint32_t)0x000000F0)
/// TMU1 field LSB position
#define MDM_TMU1_LSB    4
/// TMU1 field width
#define MDM_TMU1_WIDTH  ((uint32_t)0x00000004)
/// TMU0 field mask
#define MDM_TMU0_MASK   ((uint32_t)0x0000000F)
/// TMU0 field LSB position
#define MDM_TMU0_LSB    0
/// TMU0 field width
#define MDM_TMU0_WIDTH  ((uint32_t)0x00000004)

/// TMU3 field reset value
#define MDM_TMU3_RST    0x5
/// TMU2 field reset value
#define MDM_TMU2_RST    0x5
/// TMU1 field reset value
#define MDM_TMU1_RST    0x5
/// TMU0 field reset value
#define MDM_TMU0_RST    0x5

/**
 * @brief Constructs a value for the MDMBTMU register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] tmu3 - The value to use for the TMU3 field.
 * @param[in] tmu2 - The value to use for the TMU2 field.
 * @param[in] tmu1 - The value to use for the TMU1 field.
 * @param[in] tmu0 - The value to use for the TMU0 field.
 */
__INLINE void mdm_mdmbtmu_pack(uint8_t tmu3, uint8_t tmu2, uint8_t tmu1, uint8_t tmu0)
{
    ASSERT_ERR((((uint32_t)tmu3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    ASSERT_ERR((((uint32_t)tmu2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    ASSERT_ERR((((uint32_t)tmu1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    ASSERT_ERR((((uint32_t)tmu0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTMU_ADDR,  ((uint32_t)tmu3 << 12) | ((uint32_t)tmu2 << 8) | ((uint32_t)tmu1 << 4) | ((uint32_t)tmu0 << 0));
}

/**
 * @brief Unpacks MDMBTMU's fields from current value of the MDMBTMU register.
 *
 * Reads the MDMBTMU register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tmu3 - Will be populated with the current value of this field from the register.
 * @param[out] tmu2 - Will be populated with the current value of this field from the register.
 * @param[out] tmu1 - Will be populated with the current value of this field from the register.
 * @param[out] tmu0 - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbtmu_unpack(uint8_t* tmu3, uint8_t* tmu2, uint8_t* tmu1, uint8_t* tmu0)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTMU_ADDR);

    *tmu3 = (localVal & ((uint32_t)0x0000F000)) >> 12;
    *tmu2 = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *tmu1 = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *tmu0 = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the TMU3 field in the MDMBTMU register.
 *
 * The MDMBTMU register will be read and the TMU3 field's value will be returned.
 *
 * @return The current value of the TMU3 field in the MDMBTMU register.
 */
__INLINE uint8_t mdm_tmu3_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTMU_ADDR);
    return ((localVal & ((uint32_t)0x0000F000)) >> 12);
}

/**
 * @brief Sets the TMU3 field of the MDMBTMU register.
 *
 * The MDMBTMU register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tmu3 - The value to set the field to.
 */
__INLINE void mdm_tmu3_setf(uint8_t tmu3)
{
    ASSERT_ERR((((uint32_t)tmu3 << 12) & ~((uint32_t)0x0000F000)) == 0);
    REG_PL_WR(MDM_MDMBTMU_ADDR, (REG_PL_RD(MDM_MDMBTMU_ADDR) & ~((uint32_t)0x0000F000)) | ((uint32_t)tmu3 << 12));
}

/**
 * @brief Returns the current value of the TMU2 field in the MDMBTMU register.
 *
 * The MDMBTMU register will be read and the TMU2 field's value will be returned.
 *
 * @return The current value of the TMU2 field in the MDMBTMU register.
 */
__INLINE uint8_t mdm_tmu2_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTMU_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Sets the TMU2 field of the MDMBTMU register.
 *
 * The MDMBTMU register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tmu2 - The value to set the field to.
 */
__INLINE void mdm_tmu2_setf(uint8_t tmu2)
{
    ASSERT_ERR((((uint32_t)tmu2 << 8) & ~((uint32_t)0x00000F00)) == 0);
    REG_PL_WR(MDM_MDMBTMU_ADDR, (REG_PL_RD(MDM_MDMBTMU_ADDR) & ~((uint32_t)0x00000F00)) | ((uint32_t)tmu2 << 8));
}

/**
 * @brief Returns the current value of the TMU1 field in the MDMBTMU register.
 *
 * The MDMBTMU register will be read and the TMU1 field's value will be returned.
 *
 * @return The current value of the TMU1 field in the MDMBTMU register.
 */
__INLINE uint8_t mdm_tmu1_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTMU_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Sets the TMU1 field of the MDMBTMU register.
 *
 * The MDMBTMU register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tmu1 - The value to set the field to.
 */
__INLINE void mdm_tmu1_setf(uint8_t tmu1)
{
    ASSERT_ERR((((uint32_t)tmu1 << 4) & ~((uint32_t)0x000000F0)) == 0);
    REG_PL_WR(MDM_MDMBTMU_ADDR, (REG_PL_RD(MDM_MDMBTMU_ADDR) & ~((uint32_t)0x000000F0)) | ((uint32_t)tmu1 << 4));
}

/**
 * @brief Returns the current value of the TMU0 field in the MDMBTMU register.
 *
 * The MDMBTMU register will be read and the TMU0 field's value will be returned.
 *
 * @return The current value of the TMU0 field in the MDMBTMU register.
 */
__INLINE uint8_t mdm_tmu0_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBTMU_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the TMU0 field of the MDMBTMU register.
 *
 * The MDMBTMU register will be read, modified to contain the new field value, and written.
 *
 * @param[in] tmu0 - The value to set the field to.
 */
__INLINE void mdm_tmu0_setf(uint8_t tmu0)
{
    ASSERT_ERR((((uint32_t)tmu0 << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBTMU_ADDR, (REG_PL_RD(MDM_MDMBTMU_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)tmu0 << 0));
}

/// @}

/**
 * @name MDMBCNTL1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     12           RXLENCHKEN   1
 *  11:00          RXMAXLENGTH   0x92A
 * </pre>
 *
 * @{
 */

/// Address of the MDMBCNTL1 register
#define MDM_MDMBCNTL1_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3018)
/// Offset of the MDMBCNTL1 register from the base address
#define MDM_MDMBCNTL1_OFFSET 0x00000018
/// Index of the MDMBCNTL1 register
#define MDM_MDMBCNTL1_INDEX  0x00000006
/// Reset value of the MDMBCNTL1 register
#define MDM_MDMBCNTL1_RESET  0x0000192A

/**
 * @brief Returns the current value of the MDMBCNTL1 register.
 * The MDMBCNTL1 register will be read and its value returned.
 * @return The current value of the MDMBCNTL1 register.
 */
__INLINE uint32_t mdm_mdmbcntl1_get(void)
{
    return REG_PL_RD(MDM_MDMBCNTL1_ADDR);
}

/**
 * @brief Sets the MDMBCNTL1 register to a value.
 * The MDMBCNTL1 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbcntl1_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBCNTL1_ADDR, value);
}

// field definitions
/// RXLENCHKEN field bit
#define MDM_RXLENCHKEN_BIT     ((uint32_t)0x00001000)
/// RXLENCHKEN field position
#define MDM_RXLENCHKEN_POS     12
/// RXMAXLENGTH field mask
#define MDM_RXMAXLENGTH_MASK   ((uint32_t)0x00000FFF)
/// RXMAXLENGTH field LSB position
#define MDM_RXMAXLENGTH_LSB    0
/// RXMAXLENGTH field width
#define MDM_RXMAXLENGTH_WIDTH  ((uint32_t)0x0000000C)

/// RXLENCHKEN field reset value
#define MDM_RXLENCHKEN_RST     0x1
/// RXMAXLENGTH field reset value
#define MDM_RXMAXLENGTH_RST    0x92A

/**
 * @brief Constructs a value for the MDMBCNTL1 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] rxlenchken - The value to use for the RXLENCHKEN field.
 * @param[in] rxmaxlength - The value to use for the RXMAXLENGTH field.
 */
__INLINE void mdm_mdmbcntl1_pack(uint8_t rxlenchken, uint16_t rxmaxlength)
{
    ASSERT_ERR((((uint32_t)rxlenchken << 12) & ~((uint32_t)0x00001000)) == 0);
    ASSERT_ERR((((uint32_t)rxmaxlength << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(MDM_MDMBCNTL1_ADDR,  ((uint32_t)rxlenchken << 12) | ((uint32_t)rxmaxlength << 0));
}

/**
 * @brief Unpacks MDMBCNTL1's fields from current value of the MDMBCNTL1 register.
 *
 * Reads the MDMBCNTL1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxlenchken - Will be populated with the current value of this field from the register.
 * @param[out] rxmaxlength - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbcntl1_unpack(uint8_t* rxlenchken, uint16_t* rxmaxlength)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL1_ADDR);

    *rxlenchken = (localVal & ((uint32_t)0x00001000)) >> 12;
    *rxmaxlength = (localVal & ((uint32_t)0x00000FFF)) >> 0;
}

/**
 * @brief Returns the current value of the RXLENCHKEN field in the MDMBCNTL1 register.
 *
 * The MDMBCNTL1 register will be read and the RXLENCHKEN field's value will be returned.
 *
 * @return The current value of the RXLENCHKEN field in the MDMBCNTL1 register.
 */
__INLINE uint8_t mdm_rxlenchken_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the RXLENCHKEN field of the MDMBCNTL1 register.
 *
 * The MDMBCNTL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxlenchken - The value to set the field to.
 */
__INLINE void mdm_rxlenchken_setf(uint8_t rxlenchken)
{
    ASSERT_ERR((((uint32_t)rxlenchken << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL1_ADDR, (REG_PL_RD(MDM_MDMBCNTL1_ADDR) & ~((uint32_t)0x00001000)) | ((uint32_t)rxlenchken << 12));
}

/**
 * @brief Returns the current value of the RXMAXLENGTH field in the MDMBCNTL1 register.
 *
 * The MDMBCNTL1 register will be read and the RXMAXLENGTH field's value will be returned.
 *
 * @return The current value of the RXMAXLENGTH field in the MDMBCNTL1 register.
 */
__INLINE uint16_t mdm_rxmaxlength_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL1_ADDR);
    return ((localVal & ((uint32_t)0x00000FFF)) >> 0);
}

/**
 * @brief Sets the RXMAXLENGTH field of the MDMBCNTL1 register.
 *
 * The MDMBCNTL1 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] rxmaxlength - The value to set the field to.
 */
__INLINE void mdm_rxmaxlength_setf(uint16_t rxmaxlength)
{
    ASSERT_ERR((((uint32_t)rxmaxlength << 0) & ~((uint32_t)0x00000FFF)) == 0);
    REG_PL_WR(MDM_MDMBCNTL1_ADDR, (REG_PL_RD(MDM_MDMBCNTL1_ADDR) & ~((uint32_t)0x00000FFF)) | ((uint32_t)rxmaxlength << 0));
}

/// @}

/**
 * @name MDMBRFCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  07:00             TXENDDEL   0x30
 * </pre>
 *
 * @{
 */

/// Address of the MDMBRFCNTL register
#define MDM_MDMBRFCNTL_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x301C)
/// Offset of the MDMBRFCNTL register from the base address
#define MDM_MDMBRFCNTL_OFFSET 0x0000001C
/// Index of the MDMBRFCNTL register
#define MDM_MDMBRFCNTL_INDEX  0x00000007
/// Reset value of the MDMBRFCNTL register
#define MDM_MDMBRFCNTL_RESET  0x00000030

/**
 * @brief Returns the current value of the MDMBRFCNTL register.
 * The MDMBRFCNTL register will be read and its value returned.
 * @return The current value of the MDMBRFCNTL register.
 */
__INLINE uint32_t mdm_mdmbrfcntl_get(void)
{
    return REG_PL_RD(MDM_MDMBRFCNTL_ADDR);
}

/**
 * @brief Sets the MDMBRFCNTL register to a value.
 * The MDMBRFCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbrfcntl_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBRFCNTL_ADDR, value);
}

// field definitions
/// TXENDDEL field mask
#define MDM_TXENDDEL_MASK   ((uint32_t)0x000000FF)
/// TXENDDEL field LSB position
#define MDM_TXENDDEL_LSB    0
/// TXENDDEL field width
#define MDM_TXENDDEL_WIDTH  ((uint32_t)0x00000008)

/// TXENDDEL field reset value
#define MDM_TXENDDEL_RST    0x30

/**
 * @brief Returns the current value of the TXENDDEL field in the MDMBRFCNTL register.
 *
 * The MDMBRFCNTL register will be read and the TXENDDEL field's value will be returned.
 *
 * @return The current value of the TXENDDEL field in the MDMBRFCNTL register.
 */
__INLINE uint8_t mdm_txenddel_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBRFCNTL_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x000000FF)) == 0);
    return (localVal >> 0);
}

/**
 * @brief Sets the TXENDDEL field of the MDMBRFCNTL register.
 *
 * The MDMBRFCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] txenddel - The value to set the field to.
 */
__INLINE void mdm_txenddel_setf(uint8_t txenddel)
{
    ASSERT_ERR((((uint32_t)txenddel << 0) & ~((uint32_t)0x000000FF)) == 0);
    REG_PL_WR(MDM_MDMBRFCNTL_ADDR, (uint32_t)txenddel << 0);
}

/// @}

/**
 * @name MDMBEQCNTL register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  27:16               EQHOLD   0xFFF
 *  14:10             COMPTIME   0x0
 *  09:05              ESTTIME   0x0
 *  03:00               EQTIME   0x1
 * </pre>
 *
 * @{
 */

/// Address of the MDMBEQCNTL register
#define MDM_MDMBEQCNTL_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3020)
/// Offset of the MDMBEQCNTL register from the base address
#define MDM_MDMBEQCNTL_OFFSET 0x00000020
/// Index of the MDMBEQCNTL register
#define MDM_MDMBEQCNTL_INDEX  0x00000008
/// Reset value of the MDMBEQCNTL register
#define MDM_MDMBEQCNTL_RESET  0x0FFF0001

/**
 * @brief Returns the current value of the MDMBEQCNTL register.
 * The MDMBEQCNTL register will be read and its value returned.
 * @return The current value of the MDMBEQCNTL register.
 */
__INLINE uint32_t mdm_mdmbeqcntl_get(void)
{
    return REG_PL_RD(MDM_MDMBEQCNTL_ADDR);
}

/**
 * @brief Sets the MDMBEQCNTL register to a value.
 * The MDMBEQCNTL register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbeqcntl_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR, value);
}

// field definitions
/// EQHOLD field mask
#define MDM_EQHOLD_MASK     ((uint32_t)0x0FFF0000)
/// EQHOLD field LSB position
#define MDM_EQHOLD_LSB      16
/// EQHOLD field width
#define MDM_EQHOLD_WIDTH    ((uint32_t)0x0000000C)
/// COMPTIME field mask
#define MDM_COMPTIME_MASK   ((uint32_t)0x00007C00)
/// COMPTIME field LSB position
#define MDM_COMPTIME_LSB    10
/// COMPTIME field width
#define MDM_COMPTIME_WIDTH  ((uint32_t)0x00000005)
/// ESTTIME field mask
#define MDM_ESTTIME_MASK    ((uint32_t)0x000003E0)
/// ESTTIME field LSB position
#define MDM_ESTTIME_LSB     5
/// ESTTIME field width
#define MDM_ESTTIME_WIDTH   ((uint32_t)0x00000005)
/// EQTIME field mask
#define MDM_EQTIME_MASK     ((uint32_t)0x0000000F)
/// EQTIME field LSB position
#define MDM_EQTIME_LSB      0
/// EQTIME field width
#define MDM_EQTIME_WIDTH    ((uint32_t)0x00000004)

/// EQHOLD field reset value
#define MDM_EQHOLD_RST      0xFFF
/// COMPTIME field reset value
#define MDM_COMPTIME_RST    0x0
/// ESTTIME field reset value
#define MDM_ESTTIME_RST     0x0
/// EQTIME field reset value
#define MDM_EQTIME_RST      0x1

/**
 * @brief Constructs a value for the MDMBEQCNTL register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] eqhold - The value to use for the EQHOLD field.
 * @param[in] comptime - The value to use for the COMPTIME field.
 * @param[in] esttime - The value to use for the ESTTIME field.
 * @param[in] eqtime - The value to use for the EQTIME field.
 */
__INLINE void mdm_mdmbeqcntl_pack(uint16_t eqhold, uint8_t comptime, uint8_t esttime, uint8_t eqtime)
{
    ASSERT_ERR((((uint32_t)eqhold << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    ASSERT_ERR((((uint32_t)comptime << 10) & ~((uint32_t)0x00007C00)) == 0);
    ASSERT_ERR((((uint32_t)esttime << 5) & ~((uint32_t)0x000003E0)) == 0);
    ASSERT_ERR((((uint32_t)eqtime << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR,  ((uint32_t)eqhold << 16) | ((uint32_t)comptime << 10) | ((uint32_t)esttime << 5) | ((uint32_t)eqtime << 0));
}

/**
 * @brief Unpacks MDMBEQCNTL's fields from current value of the MDMBEQCNTL register.
 *
 * Reads the MDMBEQCNTL register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] eqhold - Will be populated with the current value of this field from the register.
 * @param[out] comptime - Will be populated with the current value of this field from the register.
 * @param[out] esttime - Will be populated with the current value of this field from the register.
 * @param[out] eqtime - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbeqcntl_unpack(uint16_t* eqhold, uint8_t* comptime, uint8_t* esttime, uint8_t* eqtime)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBEQCNTL_ADDR);

    *eqhold = (localVal & ((uint32_t)0x0FFF0000)) >> 16;
    *comptime = (localVal & ((uint32_t)0x00007C00)) >> 10;
    *esttime = (localVal & ((uint32_t)0x000003E0)) >> 5;
    *eqtime = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the EQHOLD field in the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read and the EQHOLD field's value will be returned.
 *
 * @return The current value of the EQHOLD field in the MDMBEQCNTL register.
 */
__INLINE uint16_t mdm_eqhold_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBEQCNTL_ADDR);
    return ((localVal & ((uint32_t)0x0FFF0000)) >> 16);
}

/**
 * @brief Sets the EQHOLD field of the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] eqhold - The value to set the field to.
 */
__INLINE void mdm_eqhold_setf(uint16_t eqhold)
{
    ASSERT_ERR((((uint32_t)eqhold << 16) & ~((uint32_t)0x0FFF0000)) == 0);
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR, (REG_PL_RD(MDM_MDMBEQCNTL_ADDR) & ~((uint32_t)0x0FFF0000)) | ((uint32_t)eqhold << 16));
}

/**
 * @brief Returns the current value of the COMPTIME field in the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read and the COMPTIME field's value will be returned.
 *
 * @return The current value of the COMPTIME field in the MDMBEQCNTL register.
 */
__INLINE uint8_t mdm_comptime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBEQCNTL_ADDR);
    return ((localVal & ((uint32_t)0x00007C00)) >> 10);
}

/**
 * @brief Sets the COMPTIME field of the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] comptime - The value to set the field to.
 */
__INLINE void mdm_comptime_setf(uint8_t comptime)
{
    ASSERT_ERR((((uint32_t)comptime << 10) & ~((uint32_t)0x00007C00)) == 0);
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR, (REG_PL_RD(MDM_MDMBEQCNTL_ADDR) & ~((uint32_t)0x00007C00)) | ((uint32_t)comptime << 10));
}

/**
 * @brief Returns the current value of the ESTTIME field in the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read and the ESTTIME field's value will be returned.
 *
 * @return The current value of the ESTTIME field in the MDMBEQCNTL register.
 */
__INLINE uint8_t mdm_esttime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBEQCNTL_ADDR);
    return ((localVal & ((uint32_t)0x000003E0)) >> 5);
}

/**
 * @brief Sets the ESTTIME field of the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] esttime - The value to set the field to.
 */
__INLINE void mdm_esttime_setf(uint8_t esttime)
{
    ASSERT_ERR((((uint32_t)esttime << 5) & ~((uint32_t)0x000003E0)) == 0);
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR, (REG_PL_RD(MDM_MDMBEQCNTL_ADDR) & ~((uint32_t)0x000003E0)) | ((uint32_t)esttime << 5));
}

/**
 * @brief Returns the current value of the EQTIME field in the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read and the EQTIME field's value will be returned.
 *
 * @return The current value of the EQTIME field in the MDMBEQCNTL register.
 */
__INLINE uint8_t mdm_eqtime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBEQCNTL_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the EQTIME field of the MDMBEQCNTL register.
 *
 * The MDMBEQCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] eqtime - The value to set the field to.
 */
__INLINE void mdm_eqtime_setf(uint8_t eqtime)
{
    ASSERT_ERR((((uint32_t)eqtime << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBEQCNTL_ADDR, (REG_PL_RD(MDM_MDMBEQCNTL_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)eqtime << 0));
}

/// @}

/**
 * @name MDMBCNTL2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  21:16              PRECOMP   0x38
 *  13:08             SYNCTIME   0x12
 *  03:00             LOOPTIME   0x5
 * </pre>
 *
 * @{
 */

/// Address of the MDMBCNTL2 register
#define MDM_MDMBCNTL2_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3024)
/// Offset of the MDMBCNTL2 register from the base address
#define MDM_MDMBCNTL2_OFFSET 0x00000024
/// Index of the MDMBCNTL2 register
#define MDM_MDMBCNTL2_INDEX  0x00000009
/// Reset value of the MDMBCNTL2 register
#define MDM_MDMBCNTL2_RESET  0x00381205

/**
 * @brief Returns the current value of the MDMBCNTL2 register.
 * The MDMBCNTL2 register will be read and its value returned.
 * @return The current value of the MDMBCNTL2 register.
 */
__INLINE uint32_t mdm_mdmbcntl2_get(void)
{
    return REG_PL_RD(MDM_MDMBCNTL2_ADDR);
}

/**
 * @brief Sets the MDMBCNTL2 register to a value.
 * The MDMBCNTL2 register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_mdmbcntl2_set(uint32_t value)
{
    REG_PL_WR(MDM_MDMBCNTL2_ADDR, value);
}

// field definitions
/// PRECOMP field mask
#define MDM_PRECOMP_MASK    ((uint32_t)0x003F0000)
/// PRECOMP field LSB position
#define MDM_PRECOMP_LSB     16
/// PRECOMP field width
#define MDM_PRECOMP_WIDTH   ((uint32_t)0x00000006)
/// SYNCTIME field mask
#define MDM_SYNCTIME_MASK   ((uint32_t)0x00003F00)
/// SYNCTIME field LSB position
#define MDM_SYNCTIME_LSB    8
/// SYNCTIME field width
#define MDM_SYNCTIME_WIDTH  ((uint32_t)0x00000006)
/// LOOPTIME field mask
#define MDM_LOOPTIME_MASK   ((uint32_t)0x0000000F)
/// LOOPTIME field LSB position
#define MDM_LOOPTIME_LSB    0
/// LOOPTIME field width
#define MDM_LOOPTIME_WIDTH  ((uint32_t)0x00000004)

/// PRECOMP field reset value
#define MDM_PRECOMP_RST     0x38
/// SYNCTIME field reset value
#define MDM_SYNCTIME_RST    0x12
/// LOOPTIME field reset value
#define MDM_LOOPTIME_RST    0x5

/**
 * @brief Constructs a value for the MDMBCNTL2 register given values for its fields
 * and writes the value to the register.
 *
 * @param[in] precomp - The value to use for the PRECOMP field.
 * @param[in] synctime - The value to use for the SYNCTIME field.
 * @param[in] looptime - The value to use for the LOOPTIME field.
 */
__INLINE void mdm_mdmbcntl2_pack(uint8_t precomp, uint8_t synctime, uint8_t looptime)
{
    ASSERT_ERR((((uint32_t)precomp << 16) & ~((uint32_t)0x003F0000)) == 0);
    ASSERT_ERR((((uint32_t)synctime << 8) & ~((uint32_t)0x00003F00)) == 0);
    ASSERT_ERR((((uint32_t)looptime << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBCNTL2_ADDR,  ((uint32_t)precomp << 16) | ((uint32_t)synctime << 8) | ((uint32_t)looptime << 0));
}

/**
 * @brief Unpacks MDMBCNTL2's fields from current value of the MDMBCNTL2 register.
 *
 * Reads the MDMBCNTL2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] precomp - Will be populated with the current value of this field from the register.
 * @param[out] synctime - Will be populated with the current value of this field from the register.
 * @param[out] looptime - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbcntl2_unpack(uint8_t* precomp, uint8_t* synctime, uint8_t* looptime)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL2_ADDR);

    *precomp = (localVal & ((uint32_t)0x003F0000)) >> 16;
    *synctime = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *looptime = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the PRECOMP field in the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read and the PRECOMP field's value will be returned.
 *
 * @return The current value of the PRECOMP field in the MDMBCNTL2 register.
 */
__INLINE uint8_t mdm_precomp_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x003F0000)) >> 16);
}

/**
 * @brief Sets the PRECOMP field of the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] precomp - The value to set the field to.
 */
__INLINE void mdm_precomp_setf(uint8_t precomp)
{
    ASSERT_ERR((((uint32_t)precomp << 16) & ~((uint32_t)0x003F0000)) == 0);
    REG_PL_WR(MDM_MDMBCNTL2_ADDR, (REG_PL_RD(MDM_MDMBCNTL2_ADDR) & ~((uint32_t)0x003F0000)) | ((uint32_t)precomp << 16));
}

/**
 * @brief Returns the current value of the SYNCTIME field in the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read and the SYNCTIME field's value will be returned.
 *
 * @return The current value of the SYNCTIME field in the MDMBCNTL2 register.
 */
__INLINE uint8_t mdm_synctime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Sets the SYNCTIME field of the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] synctime - The value to set the field to.
 */
__INLINE void mdm_synctime_setf(uint8_t synctime)
{
    ASSERT_ERR((((uint32_t)synctime << 8) & ~((uint32_t)0x00003F00)) == 0);
    REG_PL_WR(MDM_MDMBCNTL2_ADDR, (REG_PL_RD(MDM_MDMBCNTL2_ADDR) & ~((uint32_t)0x00003F00)) | ((uint32_t)synctime << 8));
}

/**
 * @brief Returns the current value of the LOOPTIME field in the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read and the LOOPTIME field's value will be returned.
 *
 * @return The current value of the LOOPTIME field in the MDMBCNTL2 register.
 */
__INLINE uint8_t mdm_looptime_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBCNTL2_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/**
 * @brief Sets the LOOPTIME field of the MDMBCNTL2 register.
 *
 * The MDMBCNTL2 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] looptime - The value to set the field to.
 */
__INLINE void mdm_looptime_setf(uint8_t looptime)
{
    ASSERT_ERR((((uint32_t)looptime << 0) & ~((uint32_t)0x0000000F)) == 0);
    REG_PL_WR(MDM_MDMBCNTL2_ADDR, (REG_PL_RD(MDM_MDMBCNTL2_ADDR) & ~((uint32_t)0x0000000F)) | ((uint32_t)looptime << 0));
}

/// @}

/**
 * @name MDMBSTAT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:24               EQSUMQ   0x0
 *  23:16               EQSUMI   0x0
 *  13:08            DCOFFSETQ   0x0
 *  05:00            DCOFFSETI   0x0
 * </pre>
 *
 * @{
 */

/// Address of the MDMBSTAT0 register
#define MDM_MDMBSTAT0_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x3028)
/// Offset of the MDMBSTAT0 register from the base address
#define MDM_MDMBSTAT0_OFFSET 0x00000028
/// Index of the MDMBSTAT0 register
#define MDM_MDMBSTAT0_INDEX  0x0000000A
/// Reset value of the MDMBSTAT0 register
#define MDM_MDMBSTAT0_RESET  0x00000000

/**
 * @brief Returns the current value of the MDMBSTAT0 register.
 * The MDMBSTAT0 register will be read and its value returned.
 * @return The current value of the MDMBSTAT0 register.
 */
__INLINE uint32_t mdm_mdmbstat0_get(void)
{
    return REG_PL_RD(MDM_MDMBSTAT0_ADDR);
}

// field definitions
/// EQSUMQ field mask
#define MDM_EQSUMQ_MASK      ((uint32_t)0xFF000000)
/// EQSUMQ field LSB position
#define MDM_EQSUMQ_LSB       24
/// EQSUMQ field width
#define MDM_EQSUMQ_WIDTH     ((uint32_t)0x00000008)
/// EQSUMI field mask
#define MDM_EQSUMI_MASK      ((uint32_t)0x00FF0000)
/// EQSUMI field LSB position
#define MDM_EQSUMI_LSB       16
/// EQSUMI field width
#define MDM_EQSUMI_WIDTH     ((uint32_t)0x00000008)
/// DCOFFSETQ field mask
#define MDM_DCOFFSETQ_MASK   ((uint32_t)0x00003F00)
/// DCOFFSETQ field LSB position
#define MDM_DCOFFSETQ_LSB    8
/// DCOFFSETQ field width
#define MDM_DCOFFSETQ_WIDTH  ((uint32_t)0x00000006)
/// DCOFFSETI field mask
#define MDM_DCOFFSETI_MASK   ((uint32_t)0x0000003F)
/// DCOFFSETI field LSB position
#define MDM_DCOFFSETI_LSB    0
/// DCOFFSETI field width
#define MDM_DCOFFSETI_WIDTH  ((uint32_t)0x00000006)

/// EQSUMQ field reset value
#define MDM_EQSUMQ_RST       0x0
/// EQSUMI field reset value
#define MDM_EQSUMI_RST       0x0
/// DCOFFSETQ field reset value
#define MDM_DCOFFSETQ_RST    0x0
/// DCOFFSETI field reset value
#define MDM_DCOFFSETI_RST    0x0

/**
 * @brief Unpacks MDMBSTAT0's fields from current value of the MDMBSTAT0 register.
 *
 * Reads the MDMBSTAT0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] eqsumq - Will be populated with the current value of this field from the register.
 * @param[out] eqsumi - Will be populated with the current value of this field from the register.
 * @param[out] dcoffsetq - Will be populated with the current value of this field from the register.
 * @param[out] dcoffseti - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbstat0_unpack(uint8_t* eqsumq, uint8_t* eqsumi, uint8_t* dcoffsetq, uint8_t* dcoffseti)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT0_ADDR);

    *eqsumq = (localVal & ((uint32_t)0xFF000000)) >> 24;
    *eqsumi = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *dcoffsetq = (localVal & ((uint32_t)0x00003F00)) >> 8;
    *dcoffseti = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the EQSUMQ field in the MDMBSTAT0 register.
 *
 * The MDMBSTAT0 register will be read and the EQSUMQ field's value will be returned.
 *
 * @return The current value of the EQSUMQ field in the MDMBSTAT0 register.
 */
__INLINE uint8_t mdm_eqsumq_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT0_ADDR);
    return ((localVal & ((uint32_t)0xFF000000)) >> 24);
}

/**
 * @brief Returns the current value of the EQSUMI field in the MDMBSTAT0 register.
 *
 * The MDMBSTAT0 register will be read and the EQSUMI field's value will be returned.
 *
 * @return The current value of the EQSUMI field in the MDMBSTAT0 register.
 */
__INLINE uint8_t mdm_eqsumi_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT0_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Returns the current value of the DCOFFSETQ field in the MDMBSTAT0 register.
 *
 * The MDMBSTAT0 register will be read and the DCOFFSETQ field's value will be returned.
 *
 * @return The current value of the DCOFFSETQ field in the MDMBSTAT0 register.
 */
__INLINE uint8_t mdm_dcoffsetq_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT0_ADDR);
    return ((localVal & ((uint32_t)0x00003F00)) >> 8);
}

/**
 * @brief Returns the current value of the DCOFFSETI field in the MDMBSTAT0 register.
 *
 * The MDMBSTAT0 register will be read and the DCOFFSETI field's value will be returned.
 *
 * @return The current value of the DCOFFSETI field in the MDMBSTAT0 register.
 */
__INLINE uint8_t mdm_dcoffseti_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/// @}

/**
 * @name MDMBSTAT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  31:30          RXERRORSTAT   0x0
 *  28:16                  EVM   0x0
 *  14:08          IQGAINESTIM   0x0
 *  07:00         FREQOFFESTIM   0x0
 * </pre>
 *
 * @{
 */

/// Address of the MDMBSTAT1 register
#define MDM_MDMBSTAT1_ADDR   (REG_MDMDSSS_CFG_BASE_ADDR+0x302C)
/// Offset of the MDMBSTAT1 register from the base address
#define MDM_MDMBSTAT1_OFFSET 0x0000002C
/// Index of the MDMBSTAT1 register
#define MDM_MDMBSTAT1_INDEX  0x0000000B
/// Reset value of the MDMBSTAT1 register
#define MDM_MDMBSTAT1_RESET  0x00000000

/**
 * @brief Returns the current value of the MDMBSTAT1 register.
 * The MDMBSTAT1 register will be read and its value returned.
 * @return The current value of the MDMBSTAT1 register.
 */
__INLINE uint32_t mdm_mdmbstat1_get(void)
{
    return REG_PL_RD(MDM_MDMBSTAT1_ADDR);
}

// field definitions
/// RXERRORSTAT field mask
#define MDM_RXERRORSTAT_MASK    ((uint32_t)0xC0000000)
/// RXERRORSTAT field LSB position
#define MDM_RXERRORSTAT_LSB     30
/// RXERRORSTAT field width
#define MDM_RXERRORSTAT_WIDTH   ((uint32_t)0x00000002)
/// EVM field mask
#define MDM_EVM_MASK            ((uint32_t)0x1FFF0000)
/// EVM field LSB position
#define MDM_EVM_LSB             16
/// EVM field width
#define MDM_EVM_WIDTH           ((uint32_t)0x0000000D)
/// IQGAINESTIM field mask
#define MDM_IQGAINESTIM_MASK    ((uint32_t)0x00007F00)
/// IQGAINESTIM field LSB position
#define MDM_IQGAINESTIM_LSB     8
/// IQGAINESTIM field width
#define MDM_IQGAINESTIM_WIDTH   ((uint32_t)0x00000007)
/// FREQOFFESTIM field mask
#define MDM_FREQOFFESTIM_MASK   ((uint32_t)0x000000FF)
/// FREQOFFESTIM field LSB position
#define MDM_FREQOFFESTIM_LSB    0
/// FREQOFFESTIM field width
#define MDM_FREQOFFESTIM_WIDTH  ((uint32_t)0x00000008)

/// RXERRORSTAT field reset value
#define MDM_RXERRORSTAT_RST     0x0
/// EVM field reset value
#define MDM_EVM_RST             0x0
/// IQGAINESTIM field reset value
#define MDM_IQGAINESTIM_RST     0x0
/// FREQOFFESTIM field reset value
#define MDM_FREQOFFESTIM_RST    0x0

/**
 * @brief Unpacks MDMBSTAT1's fields from current value of the MDMBSTAT1 register.
 *
 * Reads the MDMBSTAT1 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] rxerrorstat - Will be populated with the current value of this field from the register.
 * @param[out] evm - Will be populated with the current value of this field from the register.
 * @param[out] iqgainestim - Will be populated with the current value of this field from the register.
 * @param[out] freqoffestim - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_mdmbstat1_unpack(uint8_t* rxerrorstat, uint16_t* evm, uint8_t* iqgainestim, uint8_t* freqoffestim)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT1_ADDR);

    *rxerrorstat = (localVal & ((uint32_t)0xC0000000)) >> 30;
    *evm = (localVal & ((uint32_t)0x1FFF0000)) >> 16;
    *iqgainestim = (localVal & ((uint32_t)0x00007F00)) >> 8;
    *freqoffestim = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the RXERRORSTAT field in the MDMBSTAT1 register.
 *
 * The MDMBSTAT1 register will be read and the RXERRORSTAT field's value will be returned.
 *
 * @return The current value of the RXERRORSTAT field in the MDMBSTAT1 register.
 */
__INLINE uint8_t mdm_rxerrorstat_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT1_ADDR);
    return ((localVal & ((uint32_t)0xC0000000)) >> 30);
}

/**
 * @brief Returns the current value of the EVM field in the MDMBSTAT1 register.
 *
 * The MDMBSTAT1 register will be read and the EVM field's value will be returned.
 *
 * @return The current value of the EVM field in the MDMBSTAT1 register.
 */
__INLINE uint16_t mdm_evm_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT1_ADDR);
    return ((localVal & ((uint32_t)0x1FFF0000)) >> 16);
}

/**
 * @brief Returns the current value of the IQGAINESTIM field in the MDMBSTAT1 register.
 *
 * The MDMBSTAT1 register will be read and the IQGAINESTIM field's value will be returned.
 *
 * @return The current value of the IQGAINESTIM field in the MDMBSTAT1 register.
 */
__INLINE uint8_t mdm_iqgainestim_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT1_ADDR);
    return ((localVal & ((uint32_t)0x00007F00)) >> 8);
}

/**
 * @brief Returns the current value of the FREQOFFESTIM field in the MDMBSTAT1 register.
 *
 * The MDMBSTAT1 register will be read and the FREQOFFESTIM field's value will be returned.
 *
 * @return The current value of the FREQOFFESTIM field in the MDMBSTAT1 register.
 */
__INLINE uint8_t mdm_freqoffestim_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_MDMBSTAT1_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}


#endif // _REG_MDMDSSS_CFG_H_

/// @}

