/**
 * @file reg_mdm_stat.h
 * @brief Definitions of the MDM HW block registers and register access functions.
 *
 * @defgroup REG_MDM_STAT REG_MDM_STAT
 * @ingroup REG
 * @{
 *
 * @brief Definitions of the MDM HW block registers and register access functions.
 */
#ifndef _REG_MDM_STAT_H_
#define _REG_MDM_STAT_H_

#include "co_int.h"
#include "_reg_mdm_stat.h"
#include "wifi_compiler.h"
#include "wifi_arch.h"
#include "wifi_reg_access.h"

/** @brief Number of registers in the REG_MDM_STAT peripheral.
 */
#define REG_MDM_STAT_COUNT 11

/** @brief Decoding mask of the REG_MDM_STAT peripheral registers from the CPU point of view.
 */
#define REG_MDM_STAT_DECODING_MASK 0x0000003F

/**
 * @name NXVERSION register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31             MUMIMOTX   0
 *     30             MUMIMORX   0
 *     29                BFMER   0
 *     28                BFMEE   0
 *     27              LDPCDEC   0
 *     26              LDPCENC   0
 *  25:24                 CHBW   0x0
 *     23              DSSSCCK   1
 *  22:20                 NESS   0x0
 *  19:16               RFMODE   0x2
 *  15:12                 NSTS   0x2
 *  11:08                  NSS   0x2
 *  07:04                  NTX   0x2
 *  03:00                  NRX   0x2
 * </pre>
 *
 * @{
 */

/// Address of the NXVERSION register
#define MDM_NXVERSION_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0000)
/// Offset of the NXVERSION register from the base address
#define MDM_NXVERSION_OFFSET 0x00000000
/// Index of the NXVERSION register
#define MDM_NXVERSION_INDEX  0x00000000
/// Reset value of the NXVERSION register
///#define MDM_NXVERSION_RESET  0x00822222
///#define MDM_NXVERSION_RESET  0x0ec22111
#define MDM_NXVERSION_RESET  0x0ec32111

/**
 * @brief Returns the current value of the NXVERSION register.
 * The NXVERSION register will be read and its value returned.
 * @return The current value of the NXVERSION register.
 */
__INLINE uint32_t mdm_nxversion_get(void)
{
    return REG_PL_RD(MDM_NXVERSION_ADDR);
}

// field definitions
/// MUMIMOTX field bit
#define MDM_MUMIMOTX_BIT    ((uint32_t)0x80000000)
/// MUMIMOTX field position
#define MDM_MUMIMOTX_POS    31
/// MUMIMORX field bit
#define MDM_MUMIMORX_BIT    ((uint32_t)0x40000000)
/// MUMIMORX field position
#define MDM_MUMIMORX_POS    30
/// BFMER field bit
#define MDM_BFMER_BIT       ((uint32_t)0x20000000)
/// BFMER field position
#define MDM_BFMER_POS       29
/// BFMEE field bit
#define MDM_BFMEE_BIT       ((uint32_t)0x10000000)
/// BFMEE field position
#define MDM_BFMEE_POS       28
/// LDPCDEC field bit
#define MDM_LDPCDEC_BIT     ((uint32_t)0x08000000)
/// LDPCDEC field position
#define MDM_LDPCDEC_POS     27
/// LDPCENC field bit
#define MDM_LDPCENC_BIT     ((uint32_t)0x04000000)
/// LDPCENC field position
#define MDM_LDPCENC_POS     26
/// CHBW field mask
#define MDM_CHBW_MASK       ((uint32_t)0x03000000)
/// CHBW field LSB position
#define MDM_CHBW_LSB        24
/// CHBW field width
#define MDM_CHBW_WIDTH      ((uint32_t)0x00000002)
/// DSSSCCK field bit
#define MDM_DSSSCCK_BIT     ((uint32_t)0x00800000)
/// DSSSCCK field position
#define MDM_DSSSCCK_POS     23
/// NESS field mask
#define MDM_NESS_MASK       ((uint32_t)0x00700000)
/// NESS field LSB position
#define MDM_NESS_LSB        20
/// NESS field width
#define MDM_NESS_WIDTH      ((uint32_t)0x00000003)
/// RFMODE field mask
#define MDM_RFMODE_MASK     ((uint32_t)0x000F0000)
/// RFMODE field LSB position
#define MDM_RFMODE_LSB      16
/// RFMODE field width
#define MDM_RFMODE_WIDTH    ((uint32_t)0x00000004)
/// NSTS field mask
#define MDM_NSTS_MASK       ((uint32_t)0x0000F000)
/// NSTS field LSB position
#define MDM_NSTS_LSB        12
/// NSTS field width
#define MDM_NSTS_WIDTH      ((uint32_t)0x00000004)
/// NSS field mask
#define MDM_NSS_MASK        ((uint32_t)0x00000F00)
/// NSS field LSB position
#define MDM_NSS_LSB         8
/// NSS field width
#define MDM_NSS_WIDTH       ((uint32_t)0x00000004)
/// NTX field mask
#define MDM_NTX_MASK        ((uint32_t)0x000000F0)
/// NTX field LSB position
#define MDM_NTX_LSB         4
/// NTX field width
#define MDM_NTX_WIDTH       ((uint32_t)0x00000004)
/// NRX field mask
#define MDM_NRX_MASK        ((uint32_t)0x0000000F)
/// NRX field LSB position
#define MDM_NRX_LSB         0
/// NRX field width
#define MDM_NRX_WIDTH       ((uint32_t)0x00000004)

/// MUMIMOTX field reset value
#define MDM_MUMIMOTX_RST    0x0
/// MUMIMORX field reset value
#define MDM_MUMIMORX_RST    0x0
/// BFMER field reset value
#define MDM_BFMER_RST       0x0
/// BFMEE field reset value
#define MDM_BFMEE_RST       0x0
/// LDPCDEC field reset value
#define MDM_LDPCDEC_RST     0x0
/// LDPCENC field reset value
#define MDM_LDPCENC_RST     0x0
/// CHBW field reset value
#define MDM_CHBW_RST        0x0
/// DSSSCCK field reset value
#define MDM_DSSSCCK_RST     0x1
/// NESS field reset value
#define MDM_NESS_RST        0x0
/// RFMODE field reset value
#define MDM_RFMODE_RST      0x2
/// NSTS field reset value
#define MDM_NSTS_RST        0x2
/// NSS field reset value
#define MDM_NSS_RST         0x2
/// NTX field reset value
#define MDM_NTX_RST         0x2
/// NRX field reset value
#define MDM_NRX_RST         0x2

/**
 * @brief Unpacks NXVERSION's fields from current value of the NXVERSION register.
 *
 * Reads the NXVERSION register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] mumimotx - Will be populated with the current value of this field from the register.
 * @param[out] mumimorx - Will be populated with the current value of this field from the register.
 * @param[out] bfmer - Will be populated with the current value of this field from the register.
 * @param[out] bfmee - Will be populated with the current value of this field from the register.
 * @param[out] ldpcdec - Will be populated with the current value of this field from the register.
 * @param[out] ldpcenc - Will be populated with the current value of this field from the register.
 * @param[out] chbw - Will be populated with the current value of this field from the register.
 * @param[out] dssscck - Will be populated with the current value of this field from the register.
 * @param[out] ness - Will be populated with the current value of this field from the register.
 * @param[out] rfmode - Will be populated with the current value of this field from the register.
 * @param[out] nsts - Will be populated with the current value of this field from the register.
 * @param[out] nss - Will be populated with the current value of this field from the register.
 * @param[out] ntx - Will be populated with the current value of this field from the register.
 * @param[out] nrx - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_nxversion_unpack(uint8_t* mumimotx, uint8_t* mumimorx, uint8_t* bfmer, uint8_t* bfmee, uint8_t* ldpcdec, uint8_t* ldpcenc, uint8_t* chbw, uint8_t* dssscck, uint8_t* ness, uint8_t* rfmode, uint8_t* nsts, uint8_t* nss, uint8_t* ntx, uint8_t* nrx)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);

    *mumimotx = (localVal & ((uint32_t)0x80000000)) >> 31;
    *mumimorx = (localVal & ((uint32_t)0x40000000)) >> 30;
    *bfmer = (localVal & ((uint32_t)0x20000000)) >> 29;
    *bfmee = (localVal & ((uint32_t)0x10000000)) >> 28;
    *ldpcdec = (localVal & ((uint32_t)0x08000000)) >> 27;
    *ldpcenc = (localVal & ((uint32_t)0x04000000)) >> 26;
    *chbw = (localVal & ((uint32_t)0x03000000)) >> 24;
    *dssscck = (localVal & ((uint32_t)0x00800000)) >> 23;
    *ness = (localVal & ((uint32_t)0x00700000)) >> 20;
    *rfmode = (localVal & ((uint32_t)0x000F0000)) >> 16;
    *nsts = (localVal & ((uint32_t)0x0000F000)) >> 12;
    *nss = (localVal & ((uint32_t)0x00000F00)) >> 8;
    *ntx = (localVal & ((uint32_t)0x000000F0)) >> 4;
    *nrx = (localVal & ((uint32_t)0x0000000F)) >> 0;
}

/**
 * @brief Returns the current value of the MUMIMOTX field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the MUMIMOTX field's value will be returned.
 *
 * @return The current value of the MUMIMOTX field in the NXVERSION register.
 */
__INLINE uint8_t mdm_mumimotx_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Returns the current value of the MUMIMORX field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the MUMIMORX field's value will be returned.
 *
 * @return The current value of the MUMIMORX field in the NXVERSION register.
 */
__INLINE uint8_t mdm_mumimorx_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x40000000)) >> 30);
}

/**
 * @brief Returns the current value of the BFMER field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the BFMER field's value will be returned.
 *
 * @return The current value of the BFMER field in the NXVERSION register.
 */
__INLINE uint8_t mdm_bfmer_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x20000000)) >> 29);
}

/**
 * @brief Returns the current value of the BFMEE field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the BFMEE field's value will be returned.
 *
 * @return The current value of the BFMEE field in the NXVERSION register.
 */
__INLINE uint8_t mdm_bfmee_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Returns the current value of the LDPCDEC field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the LDPCDEC field's value will be returned.
 *
 * @return The current value of the LDPCDEC field in the NXVERSION register.
 */
__INLINE uint8_t mdm_ldpcdec_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x08000000)) >> 27);
}

/**
 * @brief Returns the current value of the LDPCENC field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the LDPCENC field's value will be returned.
 *
 * @return The current value of the LDPCENC field in the NXVERSION register.
 */
__INLINE uint8_t mdm_ldpcenc_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x04000000)) >> 26);
}

/**
 * @brief Returns the current value of the CHBW field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the CHBW field's value will be returned.
 *
 * @return The current value of the CHBW field in the NXVERSION register.
 */
__INLINE uint8_t mdm_chbw_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x03000000)) >> 24);
}

/**
 * @brief Returns the current value of the DSSSCCK field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the DSSSCCK field's value will be returned.
 *
 * @return The current value of the DSSSCCK field in the NXVERSION register.
 */
__INLINE uint8_t mdm_dssscck_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x00800000)) >> 23);
}

/**
 * @brief Returns the current value of the NESS field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the NESS field's value will be returned.
 *
 * @return The current value of the NESS field in the NXVERSION register.
 */
__INLINE uint8_t mdm_ness_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x00700000)) >> 20);
}

/**
 * @brief Returns the current value of the RFMODE field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the RFMODE field's value will be returned.
 *
 * @return The current value of the RFMODE field in the NXVERSION register.
 */
__INLINE uint8_t mdm_rfmode_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x000F0000)) >> 16);
}

/**
 * @brief Returns the current value of the NSTS field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the NSTS field's value will be returned.
 *
 * @return The current value of the NSTS field in the NXVERSION register.
 */
__INLINE uint8_t mdm_nsts_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x0000F000)) >> 12);
}

/**
 * @brief Returns the current value of the NSS field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the NSS field's value will be returned.
 *
 * @return The current value of the NSS field in the NXVERSION register.
 */
__INLINE uint8_t mdm_nss_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x00000F00)) >> 8);
}

/**
 * @brief Returns the current value of the NTX field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the NTX field's value will be returned.
 *
 * @return The current value of the NTX field in the NXVERSION register.
 */
__INLINE uint8_t mdm_ntx_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x000000F0)) >> 4);
}

/**
 * @brief Returns the current value of the NRX field in the NXVERSION register.
 *
 * The NXVERSION register will be read and the NRX field's value will be returned.
 *
 * @return The current value of the NRX field in the NXVERSION register.
 */
__INLINE uint8_t mdm_nrx_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_NXVERSION_ADDR);
    return ((localVal & ((uint32_t)0x0000000F)) >> 0);
}

/// @}

/**
 * @name TDOFFSETSTAT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  20:00        COARSEFOANGLE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TDOFFSETSTAT0 register
#define MDM_TDOFFSETSTAT0_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0004)
/// Offset of the TDOFFSETSTAT0 register from the base address
#define MDM_TDOFFSETSTAT0_OFFSET 0x00000004
/// Index of the TDOFFSETSTAT0 register
#define MDM_TDOFFSETSTAT0_INDEX  0x00000001
/// Reset value of the TDOFFSETSTAT0 register
#define MDM_TDOFFSETSTAT0_RESET  0x00000000

/**
 * @brief Returns the current value of the TDOFFSETSTAT0 register.
 * The TDOFFSETSTAT0 register will be read and its value returned.
 * @return The current value of the TDOFFSETSTAT0 register.
 */
__INLINE uint32_t mdm_tdoffsetstat0_get(void)
{
    return REG_PL_RD(MDM_TDOFFSETSTAT0_ADDR);
}

// field definitions
/// COARSEFOANGLE field mask
#define MDM_COARSEFOANGLE_MASK   ((uint32_t)0x001FFFFF)
/// COARSEFOANGLE field LSB position
#define MDM_COARSEFOANGLE_LSB    0
/// COARSEFOANGLE field width
#define MDM_COARSEFOANGLE_WIDTH  ((uint32_t)0x00000015)

/// COARSEFOANGLE field reset value
#define MDM_COARSEFOANGLE_RST    0x0

/**
 * @brief Returns the current value of the COARSEFOANGLE field in the TDOFFSETSTAT0 register.
 *
 * The TDOFFSETSTAT0 register will be read and the COARSEFOANGLE field's value will be returned.
 *
 * @return The current value of the COARSEFOANGLE field in the TDOFFSETSTAT0 register.
 */
__INLINE uint32_t mdm_coarsefoangle_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDOFFSETSTAT0_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x001FFFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name TDOFFSETSTAT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  20:00          FINEFOANGLE   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TDOFFSETSTAT1 register
#define MDM_TDOFFSETSTAT1_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0008)
/// Offset of the TDOFFSETSTAT1 register from the base address
#define MDM_TDOFFSETSTAT1_OFFSET 0x00000008
/// Index of the TDOFFSETSTAT1 register
#define MDM_TDOFFSETSTAT1_INDEX  0x00000002
/// Reset value of the TDOFFSETSTAT1 register
#define MDM_TDOFFSETSTAT1_RESET  0x00000000

/**
 * @brief Returns the current value of the TDOFFSETSTAT1 register.
 * The TDOFFSETSTAT1 register will be read and its value returned.
 * @return The current value of the TDOFFSETSTAT1 register.
 */
__INLINE uint32_t mdm_tdoffsetstat1_get(void)
{
    return REG_PL_RD(MDM_TDOFFSETSTAT1_ADDR);
}

// field definitions
/// FINEFOANGLE field mask
#define MDM_FINEFOANGLE_MASK   ((uint32_t)0x001FFFFF)
/// FINEFOANGLE field LSB position
#define MDM_FINEFOANGLE_LSB    0
/// FINEFOANGLE field width
#define MDM_FINEFOANGLE_WIDTH  ((uint32_t)0x00000015)

/// FINEFOANGLE field reset value
#define MDM_FINEFOANGLE_RST    0x0

/**
 * @brief Returns the current value of the FINEFOANGLE field in the TDOFFSETSTAT1 register.
 *
 * The TDOFFSETSTAT1 register will be read and the FINEFOANGLE field's value will be returned.
 *
 * @return The current value of the FINEFOANGLE field in the TDOFFSETSTAT1 register.
 */
__INLINE uint32_t mdm_finefoangle_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TDOFFSETSTAT1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x001FFFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name FDTOFFSETSTAT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  29:12         STOSLOPESTAT   0x0
 *  05:00       TDSYNCHOFFSTAT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the FDTOFFSETSTAT0 register
#define MDM_FDTOFFSETSTAT0_ADDR   (REG_MDM_STAT_BASE_ADDR+0x000C)
/// Offset of the FDTOFFSETSTAT0 register from the base address
#define MDM_FDTOFFSETSTAT0_OFFSET 0x0000000C
/// Index of the FDTOFFSETSTAT0 register
#define MDM_FDTOFFSETSTAT0_INDEX  0x00000003
/// Reset value of the FDTOFFSETSTAT0 register
#define MDM_FDTOFFSETSTAT0_RESET  0x00000000

/**
 * @brief Returns the current value of the FDTOFFSETSTAT0 register.
 * The FDTOFFSETSTAT0 register will be read and its value returned.
 * @return The current value of the FDTOFFSETSTAT0 register.
 */
__INLINE uint32_t mdm_fdtoffsetstat0_get(void)
{
    return REG_PL_RD(MDM_FDTOFFSETSTAT0_ADDR);
}

// field definitions
/// STOSLOPESTAT field mask
#define MDM_STOSLOPESTAT_MASK     ((uint32_t)0x3FFFF000)
/// STOSLOPESTAT field LSB position
#define MDM_STOSLOPESTAT_LSB      12
/// STOSLOPESTAT field width
#define MDM_STOSLOPESTAT_WIDTH    ((uint32_t)0x00000012)
/// TDSYNCHOFFSTAT field mask
#define MDM_TDSYNCHOFFSTAT_MASK   ((uint32_t)0x0000003F)
/// TDSYNCHOFFSTAT field LSB position
#define MDM_TDSYNCHOFFSTAT_LSB    0
/// TDSYNCHOFFSTAT field width
#define MDM_TDSYNCHOFFSTAT_WIDTH  ((uint32_t)0x00000006)

/// STOSLOPESTAT field reset value
#define MDM_STOSLOPESTAT_RST      0x0
/// TDSYNCHOFFSTAT field reset value
#define MDM_TDSYNCHOFFSTAT_RST    0x0

/**
 * @brief Unpacks FDTOFFSETSTAT0's fields from current value of the FDTOFFSETSTAT0 register.
 *
 * Reads the FDTOFFSETSTAT0 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] stoslopestat - Will be populated with the current value of this field from the register.
 * @param[out] tdsynchoffstat - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_fdtoffsetstat0_unpack(uint32_t* stoslopestat, uint8_t* tdsynchoffstat)
{
    uint32_t localVal = REG_PL_RD(MDM_FDTOFFSETSTAT0_ADDR);

    *stoslopestat = (localVal & ((uint32_t)0x3FFFF000)) >> 12;
    *tdsynchoffstat = (localVal & ((uint32_t)0x0000003F)) >> 0;
}

/**
 * @brief Returns the current value of the STOSLOPESTAT field in the FDTOFFSETSTAT0 register.
 *
 * The FDTOFFSETSTAT0 register will be read and the STOSLOPESTAT field's value will be returned.
 *
 * @return The current value of the STOSLOPESTAT field in the FDTOFFSETSTAT0 register.
 */
__INLINE uint32_t mdm_stoslopestat_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDTOFFSETSTAT0_ADDR);
    return ((localVal & ((uint32_t)0x3FFFF000)) >> 12);
}

/**
 * @brief Returns the current value of the TDSYNCHOFFSTAT field in the FDTOFFSETSTAT0 register.
 *
 * The FDTOFFSETSTAT0 register will be read and the TDSYNCHOFFSTAT field's value will be returned.
 *
 * @return The current value of the TDSYNCHOFFSTAT field in the FDTOFFSETSTAT0 register.
 */
__INLINE uint8_t mdm_tdsynchoffstat_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDTOFFSETSTAT0_ADDR);
    return ((localVal & ((uint32_t)0x0000003F)) >> 0);
}

/// @}

/**
 * @name FDTOFFSETSTAT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  17:00         CPESLOPESTAT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the FDTOFFSETSTAT1 register
#define MDM_FDTOFFSETSTAT1_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0010)
/// Offset of the FDTOFFSETSTAT1 register from the base address
#define MDM_FDTOFFSETSTAT1_OFFSET 0x00000010
/// Index of the FDTOFFSETSTAT1 register
#define MDM_FDTOFFSETSTAT1_INDEX  0x00000004
/// Reset value of the FDTOFFSETSTAT1 register
#define MDM_FDTOFFSETSTAT1_RESET  0x00000000

/**
 * @brief Returns the current value of the FDTOFFSETSTAT1 register.
 * The FDTOFFSETSTAT1 register will be read and its value returned.
 * @return The current value of the FDTOFFSETSTAT1 register.
 */
__INLINE uint32_t mdm_fdtoffsetstat1_get(void)
{
    return REG_PL_RD(MDM_FDTOFFSETSTAT1_ADDR);
}

// field definitions
/// CPESLOPESTAT field mask
#define MDM_CPESLOPESTAT_MASK   ((uint32_t)0x0003FFFF)
/// CPESLOPESTAT field LSB position
#define MDM_CPESLOPESTAT_LSB    0
/// CPESLOPESTAT field width
#define MDM_CPESLOPESTAT_WIDTH  ((uint32_t)0x00000012)

/// CPESLOPESTAT field reset value
#define MDM_CPESLOPESTAT_RST    0x0

/**
 * @brief Returns the current value of the CPESLOPESTAT field in the FDTOFFSETSTAT1 register.
 *
 * The FDTOFFSETSTAT1 register will be read and the CPESLOPESTAT field's value will be returned.
 *
 * @return The current value of the CPESLOPESTAT field in the FDTOFFSETSTAT1 register.
 */
__INLINE uint32_t mdm_cpeslopestat_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_FDTOFFSETSTAT1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0003FFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name TBESTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  20:08     TBEMaxCorrValSum   0x0
 *  07:00             TBECOUNT   0x0
 * </pre>
 *
 * @{
 */

/// Address of the TBESTAT register
#define MDM_TBESTAT_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0014)
/// Offset of the TBESTAT register from the base address
#define MDM_TBESTAT_OFFSET 0x00000014
/// Index of the TBESTAT register
#define MDM_TBESTAT_INDEX  0x00000005
/// Reset value of the TBESTAT register
#define MDM_TBESTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the TBESTAT register.
 * The TBESTAT register will be read and its value returned.
 * @return The current value of the TBESTAT register.
 */
__INLINE uint32_t mdm_tbestat_get(void)
{
    return REG_PL_RD(MDM_TBESTAT_ADDR);
}

// field definitions
/// TBE_MAX_CORR_VAL_SUM field mask
#define MDM_TBE_MAX_CORR_VAL_SUM_MASK   ((uint32_t)0x001FFF00)
/// TBE_MAX_CORR_VAL_SUM field LSB position
#define MDM_TBE_MAX_CORR_VAL_SUM_LSB    8
/// TBE_MAX_CORR_VAL_SUM field width
#define MDM_TBE_MAX_CORR_VAL_SUM_WIDTH  ((uint32_t)0x0000000D)
/// TBECOUNT field mask
#define MDM_TBECOUNT_MASK               ((uint32_t)0x000000FF)
/// TBECOUNT field LSB position
#define MDM_TBECOUNT_LSB                0
/// TBECOUNT field width
#define MDM_TBECOUNT_WIDTH              ((uint32_t)0x00000008)

/// TBE_MAX_CORR_VAL_SUM field reset value
#define MDM_TBE_MAX_CORR_VAL_SUM_RST    0x0
/// TBECOUNT field reset value
#define MDM_TBECOUNT_RST                0x0

/**
 * @brief Unpacks TBESTAT's fields from current value of the TBESTAT register.
 *
 * Reads the TBESTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] tbemaxcorrvalsum - Will be populated with the current value of this field from the register.
 * @param[out] tbecount - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_tbestat_unpack(uint16_t* tbemaxcorrvalsum, uint8_t* tbecount)
{
    uint32_t localVal = REG_PL_RD(MDM_TBESTAT_ADDR);

    *tbemaxcorrvalsum = (localVal & ((uint32_t)0x001FFF00)) >> 8;
    *tbecount = (localVal & ((uint32_t)0x000000FF)) >> 0;
}

/**
 * @brief Returns the current value of the TBEMaxCorrValSum field in the TBESTAT register.
 *
 * The TBESTAT register will be read and the TBEMaxCorrValSum field's value will be returned.
 *
 * @return The current value of the TBEMaxCorrValSum field in the TBESTAT register.
 */
__INLINE uint16_t mdm_tbe_max_corr_val_sum_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBESTAT_ADDR);
    return ((localVal & ((uint32_t)0x001FFF00)) >> 8);
}

/**
 * @brief Returns the current value of the TBECOUNT field in the TBESTAT register.
 *
 * The TBESTAT register will be read and the TBECOUNT field's value will be returned.
 *
 * @return The current value of the TBECOUNT field in the TBESTAT register.
 */
__INLINE uint8_t mdm_tbecount_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_TBESTAT_ADDR);
    return ((localVal & ((uint32_t)0x000000FF)) >> 0);
}

/// @}

/**
 * @name BFMERSTAT0 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  04:00         BFMERMemSize   0x0
 * </pre>
 *
 * @{
 */

/// Address of the BFMERSTAT0 register
#define MDM_BFMERSTAT0_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0018)
/// Offset of the BFMERSTAT0 register from the base address
#define MDM_BFMERSTAT0_OFFSET 0x00000018
/// Index of the BFMERSTAT0 register
#define MDM_BFMERSTAT0_INDEX  0x00000006
/// Reset value of the BFMERSTAT0 register
#define MDM_BFMERSTAT0_RESET  0x00000000

/**
 * @brief Returns the current value of the BFMERSTAT0 register.
 * The BFMERSTAT0 register will be read and its value returned.
 * @return The current value of the BFMERSTAT0 register.
 */
__INLINE uint32_t mdm_bfmerstat0_get(void)
{
    return REG_PL_RD(MDM_BFMERSTAT0_ADDR);
}

// field definitions
/// BFMER_MEM_SIZE field mask
#define MDM_BFMER_MEM_SIZE_MASK   ((uint32_t)0x0000001F)
/// BFMER_MEM_SIZE field LSB position
#define MDM_BFMER_MEM_SIZE_LSB    0
/// BFMER_MEM_SIZE field width
#define MDM_BFMER_MEM_SIZE_WIDTH  ((uint32_t)0x00000005)

/// BFMER_MEM_SIZE field reset value
#define MDM_BFMER_MEM_SIZE_RST    0x0

/**
 * @brief Returns the current value of the BFMERMemSize field in the BFMERSTAT0 register.
 *
 * The BFMERSTAT0 register will be read and the BFMERMemSize field's value will be returned.
 *
 * @return The current value of the BFMERMemSize field in the BFMERSTAT0 register.
 */
__INLINE uint8_t mdm_bfmer_mem_size_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERSTAT0_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x0000001F)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name EVENTFLAGSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     31           EVENTCLEAR   0
 *  30:00            EVENTFLAG   0x0
 * </pre>
 *
 * @{
 */

/// Address of the EVENTFLAGSTAT register
#define MDM_EVENTFLAGSTAT_ADDR   (REG_MDM_STAT_BASE_ADDR+0x001C)
/// Offset of the EVENTFLAGSTAT register from the base address
#define MDM_EVENTFLAGSTAT_OFFSET 0x0000001C
/// Index of the EVENTFLAGSTAT register
#define MDM_EVENTFLAGSTAT_INDEX  0x00000007
/// Reset value of the EVENTFLAGSTAT register
#define MDM_EVENTFLAGSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the EVENTFLAGSTAT register.
 * The EVENTFLAGSTAT register will be read and its value returned.
 * @return The current value of the EVENTFLAGSTAT register.
 */
__INLINE uint32_t mdm_eventflagstat_get(void)
{
    return REG_PL_RD(MDM_EVENTFLAGSTAT_ADDR);
}

/**
 * @brief Sets the EVENTFLAGSTAT register to a value.
 * The EVENTFLAGSTAT register will be written.
 * @param value - The value to write.
 */
__INLINE void mdm_eventflagstat_set(uint32_t value)
{
    REG_PL_WR(MDM_EVENTFLAGSTAT_ADDR, value);
}

// field definitions
/// EVENTCLEAR field bit
#define MDM_EVENTCLEAR_BIT    ((uint32_t)0x80000000)
/// EVENTCLEAR field position
#define MDM_EVENTCLEAR_POS    31
/// EVENTFLAG field mask
#define MDM_EVENTFLAG_MASK    ((uint32_t)0x7FFFFFFF)
/// EVENTFLAG field LSB position
#define MDM_EVENTFLAG_LSB     0
/// EVENTFLAG field width
#define MDM_EVENTFLAG_WIDTH   ((uint32_t)0x0000001F)

/// EVENTCLEAR field reset value
#define MDM_EVENTCLEAR_RST    0x0
/// EVENTFLAG field reset value
#define MDM_EVENTFLAG_RST     0x0

/**
 * @brief Unpacks EVENTFLAGSTAT's fields from current value of the EVENTFLAGSTAT register.
 *
 * Reads the EVENTFLAGSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] eventclear - Will be populated with the current value of this field from the register.
 * @param[out] eventflag - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_eventflagstat_unpack(uint8_t* eventclear, uint32_t* eventflag)
{
    uint32_t localVal = REG_PL_RD(MDM_EVENTFLAGSTAT_ADDR);

    *eventclear = (localVal & ((uint32_t)0x80000000)) >> 31;
    *eventflag = (localVal & ((uint32_t)0x7FFFFFFF)) >> 0;
}

/**
 * @brief Returns the current value of the EVENTCLEAR field in the EVENTFLAGSTAT register.
 *
 * The EVENTFLAGSTAT register will be read and the EVENTCLEAR field's value will be returned.
 *
 * @return The current value of the EVENTCLEAR field in the EVENTFLAGSTAT register.
 */
__INLINE uint8_t mdm_eventclear_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EVENTFLAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0x80000000)) >> 31);
}

/**
 * @brief Sets the EVENTCLEAR field of the EVENTFLAGSTAT register.
 *
 * The EVENTFLAGSTAT register will be read, modified to contain the new field value, and written.
 *
 * @param[in] eventclear - The value to set the field to.
 */
__INLINE void mdm_eventclear_setf(uint8_t eventclear)
{
    ASSERT_ERR((((uint32_t)eventclear << 31) & ~((uint32_t)0x80000000)) == 0);
    REG_PL_WR(MDM_EVENTFLAGSTAT_ADDR, (uint32_t)eventclear << 31);
}

/**
 * @brief Returns the current value of the EVENTFLAG field in the EVENTFLAGSTAT register.
 *
 * The EVENTFLAGSTAT register will be read and the EVENTFLAG field's value will be returned.
 *
 * @return The current value of the EVENTFLAG field in the EVENTFLAGSTAT register.
 */
__INLINE uint32_t mdm_eventflag_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_EVENTFLAGSTAT_ADDR);
    return ((localVal & ((uint32_t)0x7FFFFFFF)) >> 0);
}

/// @}

/**
 * @name BFMERSTAT1 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  21:00        BFMERRhoMuAcc   0x0
 * </pre>
 *
 * @{
 */

/// Address of the BFMERSTAT1 register
#define MDM_BFMERSTAT1_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0020)
/// Offset of the BFMERSTAT1 register from the base address
#define MDM_BFMERSTAT1_OFFSET 0x00000020
/// Index of the BFMERSTAT1 register
#define MDM_BFMERSTAT1_INDEX  0x00000008
/// Reset value of the BFMERSTAT1 register
#define MDM_BFMERSTAT1_RESET  0x00000000

/**
 * @brief Returns the current value of the BFMERSTAT1 register.
 * The BFMERSTAT1 register will be read and its value returned.
 * @return The current value of the BFMERSTAT1 register.
 */
__INLINE uint32_t mdm_bfmerstat1_get(void)
{
    return REG_PL_RD(MDM_BFMERSTAT1_ADDR);
}

// field definitions
/// BFMER_RHO_MU_ACC field mask
#define MDM_BFMER_RHO_MU_ACC_MASK   ((uint32_t)0xFF3FFFFF)
/// BFMER_RHO_MU_ACC field LSB position
#define MDM_BFMER_RHO_MU_ACC_LSB    0
/// BFMER_RHO_MU_ACC field width
#define MDM_BFMER_RHO_MU_ACC_WIDTH  ((uint32_t)0x00000016)

/// BFMER_RHO_MU_ACC field reset value
#define MDM_BFMER_RHO_MU_ACC_RST    0x0

/**
 * @brief Returns the current value of the BFMERRhoMuAcc field in the BFMERSTAT1 register.
 *
 * The BFMERSTAT1 register will be read and the BFMERRhoMuAcc field's value will be returned.
 *
 * @return The current value of the BFMERRhoMuAcc field in the BFMERSTAT1 register.
 */
__INLINE uint32_t mdm_bfmer_rho_mu_acc_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERSTAT1_ADDR);
    ASSERT_ERR((localVal & ~((uint32_t)0x003FFFFF)) == 0);
    return (localVal >> 0);
}

/// @}

/**
 * @name BFMERSTAT2 register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  23:16             BFMERNSD   0x0
 *  12:00        BFMERRhoMuMax   0x0
 * </pre>
 *
 * @{
 */

/// Address of the BFMERSTAT2 register
#define MDM_BFMERSTAT2_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0024)
/// Offset of the BFMERSTAT2 register from the base address
#define MDM_BFMERSTAT2_OFFSET 0x00000024
/// Index of the BFMERSTAT2 register
#define MDM_BFMERSTAT2_INDEX  0x00000009
/// Reset value of the BFMERSTAT2 register
#define MDM_BFMERSTAT2_RESET  0x00000000

/**
 * @brief Returns the current value of the BFMERSTAT2 register.
 * The BFMERSTAT2 register will be read and its value returned.
 * @return The current value of the BFMERSTAT2 register.
 */
__INLINE uint32_t mdm_bfmerstat2_get(void)
{
    return REG_PL_RD(MDM_BFMERSTAT2_ADDR);
}

// field definitions
/// BFMERNSD field mask
#define MDM_BFMERNSD_MASK           ((uint32_t)0x00FF0000)
/// BFMERNSD field LSB position
#define MDM_BFMERNSD_LSB            16
/// BFMERNSD field width
#define MDM_BFMERNSD_WIDTH          ((uint32_t)0x00000008)
/// BFMER_RHO_MU_MAX field mask
#define MDM_BFMER_RHO_MU_MAX_MASK   ((uint32_t)0x00001FFF)
/// BFMER_RHO_MU_MAX field LSB position
#define MDM_BFMER_RHO_MU_MAX_LSB    0
/// BFMER_RHO_MU_MAX field width
#define MDM_BFMER_RHO_MU_MAX_WIDTH  ((uint32_t)0x0000000D)

/// BFMERNSD field reset value
#define MDM_BFMERNSD_RST            0x0
/// BFMER_RHO_MU_MAX field reset value
#define MDM_BFMER_RHO_MU_MAX_RST    0x0

/**
 * @brief Unpacks BFMERSTAT2's fields from current value of the BFMERSTAT2 register.
 *
 * Reads the BFMERSTAT2 register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] bfmernsd - Will be populated with the current value of this field from the register.
 * @param[out] bfmerrhomumax - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_bfmerstat2_unpack(uint8_t* bfmernsd, uint16_t* bfmerrhomumax)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERSTAT2_ADDR);

    *bfmernsd = (localVal & ((uint32_t)0x00FF0000)) >> 16;
    *bfmerrhomumax = (localVal & ((uint32_t)0x00001FFF)) >> 0;
}

/**
 * @brief Returns the current value of the BFMERNSD field in the BFMERSTAT2 register.
 *
 * The BFMERSTAT2 register will be read and the BFMERNSD field's value will be returned.
 *
 * @return The current value of the BFMERNSD field in the BFMERSTAT2 register.
 */
__INLINE uint8_t mdm_bfmernsd_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERSTAT2_ADDR);
    return ((localVal & ((uint32_t)0x00FF0000)) >> 16);
}

/**
 * @brief Returns the current value of the BFMERRhoMuMax field in the BFMERSTAT2 register.
 *
 * The BFMERSTAT2 register will be read and the BFMERRhoMuMax field's value will be returned.
 *
 * @return The current value of the BFMERRhoMuMax field in the BFMERSTAT2 register.
 */
__INLINE uint16_t mdm_bfmer_rho_mu_max_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERSTAT2_ADDR);
    return ((localVal & ((uint32_t)0x00001FFF)) >> 0);
}

/// @}

/**
 * @name BFMERERRSTAT register definitions
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *     20        BFMERSecBwErr   0
 *     19   BFMERSecGroupingErr   0
 *     18        BFMERSecNcErr   0
 *     17        BFMERSecNrErr   0
 *     16   BFMERSecHdrAddrErr   0
 *     04       BFMERPrimBwErr   0
 *     03   BFMERPrimGroupingErr   0
 *     02       BFMERPrimNcErr   0
 *     01       BFMERPrimNrErr   0
 *     00   BFMERPrimHdrAddrErr   0
 * </pre>
 *
 * @{
 */

/// Address of the BFMERERRSTAT register
#define MDM_BFMERERRSTAT_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0028)
/// Offset of the BFMERERRSTAT register from the base address
#define MDM_BFMERERRSTAT_OFFSET 0x00000028
/// Index of the BFMERERRSTAT register
#define MDM_BFMERERRSTAT_INDEX  0x0000000A
/// Reset value of the BFMERERRSTAT register
#define MDM_BFMERERRSTAT_RESET  0x00000000

/**
 * @brief Returns the current value of the BFMERERRSTAT register.
 * The BFMERERRSTAT register will be read and its value returned.
 * @return The current value of the BFMERERRSTAT register.
 */
__INLINE uint32_t mdm_bfmererrstat_get(void)
{
    return REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
}

// field definitions
/// BFMER_SEC_BW_ERR field bit
#define MDM_BFMER_SEC_BW_ERR_BIT           ((uint32_t)0x00100000)
/// BFMER_SEC_BW_ERR field position
#define MDM_BFMER_SEC_BW_ERR_POS           20
/// BFMER_SEC_GROUPING_ERR field bit
#define MDM_BFMER_SEC_GROUPING_ERR_BIT     ((uint32_t)0x00080000)
/// BFMER_SEC_GROUPING_ERR field position
#define MDM_BFMER_SEC_GROUPING_ERR_POS     19
/// BFMER_SEC_NC_ERR field bit
#define MDM_BFMER_SEC_NC_ERR_BIT           ((uint32_t)0x00040000)
/// BFMER_SEC_NC_ERR field position
#define MDM_BFMER_SEC_NC_ERR_POS           18
/// BFMER_SEC_NR_ERR field bit
#define MDM_BFMER_SEC_NR_ERR_BIT           ((uint32_t)0x00020000)
/// BFMER_SEC_NR_ERR field position
#define MDM_BFMER_SEC_NR_ERR_POS           17
/// BFMER_SEC_HDR_ADDR_ERR field bit
#define MDM_BFMER_SEC_HDR_ADDR_ERR_BIT     ((uint32_t)0x00010000)
/// BFMER_SEC_HDR_ADDR_ERR field position
#define MDM_BFMER_SEC_HDR_ADDR_ERR_POS     16
/// BFMER_PRIM_BW_ERR field bit
#define MDM_BFMER_PRIM_BW_ERR_BIT          ((uint32_t)0x00000010)
/// BFMER_PRIM_BW_ERR field position
#define MDM_BFMER_PRIM_BW_ERR_POS          4
/// BFMER_PRIM_GROUPING_ERR field bit
#define MDM_BFMER_PRIM_GROUPING_ERR_BIT    ((uint32_t)0x00000008)
/// BFMER_PRIM_GROUPING_ERR field position
#define MDM_BFMER_PRIM_GROUPING_ERR_POS    3
/// BFMER_PRIM_NC_ERR field bit
#define MDM_BFMER_PRIM_NC_ERR_BIT          ((uint32_t)0x00000004)
/// BFMER_PRIM_NC_ERR field position
#define MDM_BFMER_PRIM_NC_ERR_POS          2
/// BFMER_PRIM_NR_ERR field bit
#define MDM_BFMER_PRIM_NR_ERR_BIT          ((uint32_t)0x00000002)
/// BFMER_PRIM_NR_ERR field position
#define MDM_BFMER_PRIM_NR_ERR_POS          1
/// BFMER_PRIM_HDR_ADDR_ERR field bit
#define MDM_BFMER_PRIM_HDR_ADDR_ERR_BIT    ((uint32_t)0x00000001)
/// BFMER_PRIM_HDR_ADDR_ERR field position
#define MDM_BFMER_PRIM_HDR_ADDR_ERR_POS    0

/// BFMER_SEC_BW_ERR field reset value
#define MDM_BFMER_SEC_BW_ERR_RST           0x0
/// BFMER_SEC_GROUPING_ERR field reset value
#define MDM_BFMER_SEC_GROUPING_ERR_RST     0x0
/// BFMER_SEC_NC_ERR field reset value
#define MDM_BFMER_SEC_NC_ERR_RST           0x0
/// BFMER_SEC_NR_ERR field reset value
#define MDM_BFMER_SEC_NR_ERR_RST           0x0
/// BFMER_SEC_HDR_ADDR_ERR field reset value
#define MDM_BFMER_SEC_HDR_ADDR_ERR_RST     0x0
/// BFMER_PRIM_BW_ERR field reset value
#define MDM_BFMER_PRIM_BW_ERR_RST          0x0
/// BFMER_PRIM_GROUPING_ERR field reset value
#define MDM_BFMER_PRIM_GROUPING_ERR_RST    0x0
/// BFMER_PRIM_NC_ERR field reset value
#define MDM_BFMER_PRIM_NC_ERR_RST          0x0
/// BFMER_PRIM_NR_ERR field reset value
#define MDM_BFMER_PRIM_NR_ERR_RST          0x0
/// BFMER_PRIM_HDR_ADDR_ERR field reset value
#define MDM_BFMER_PRIM_HDR_ADDR_ERR_RST    0x0

/**
 * @brief Unpacks BFMERERRSTAT's fields from current value of the BFMERERRSTAT register.
 *
 * Reads the BFMERERRSTAT register and populates all the _field variables with the corresponding
 * values from the register.
 *
 * @param[out] bfmersecbwerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmersecgroupingerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmersecncerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmersecnrerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmersechdraddrerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmerprimbwerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmerprimgroupingerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmerprimncerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmerprimnrerr - Will be populated with the current value of this field from the register.
 * @param[out] bfmerprimhdraddrerr - Will be populated with the current value of this field from the register.
 */
__INLINE void mdm_bfmererrstat_unpack(uint8_t* bfmersecbwerr, uint8_t* bfmersecgroupingerr, uint8_t* bfmersecncerr, uint8_t* bfmersecnrerr, uint8_t* bfmersechdraddrerr, uint8_t* bfmerprimbwerr, uint8_t* bfmerprimgroupingerr, uint8_t* bfmerprimncerr, uint8_t* bfmerprimnrerr, uint8_t* bfmerprimhdraddrerr)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);

    *bfmersecbwerr = (localVal & ((uint32_t)0x00100000)) >> 20;
    *bfmersecgroupingerr = (localVal & ((uint32_t)0x00080000)) >> 19;
    *bfmersecncerr = (localVal & ((uint32_t)0x00040000)) >> 18;
    *bfmersecnrerr = (localVal & ((uint32_t)0x00020000)) >> 17;
    *bfmersechdraddrerr = (localVal & ((uint32_t)0x00010000)) >> 16;
    *bfmerprimbwerr = (localVal & ((uint32_t)0x00000010)) >> 4;
    *bfmerprimgroupingerr = (localVal & ((uint32_t)0x00000008)) >> 3;
    *bfmerprimncerr = (localVal & ((uint32_t)0x00000004)) >> 2;
    *bfmerprimnrerr = (localVal & ((uint32_t)0x00000002)) >> 1;
    *bfmerprimhdraddrerr = (localVal & ((uint32_t)0x00000001)) >> 0;
}

/**
 * @brief Returns the current value of the BFMERSecBwErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERSecBwErr field's value will be returned.
 *
 * @return The current value of the BFMERSecBwErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_sec_bw_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00100000)) >> 20);
}

/**
 * @brief Returns the current value of the BFMERSecGroupingErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERSecGroupingErr field's value will be returned.
 *
 * @return The current value of the BFMERSecGroupingErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_sec_grouping_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00080000)) >> 19);
}

/**
 * @brief Returns the current value of the BFMERSecNcErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERSecNcErr field's value will be returned.
 *
 * @return The current value of the BFMERSecNcErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_sec_nc_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00040000)) >> 18);
}

/**
 * @brief Returns the current value of the BFMERSecNrErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERSecNrErr field's value will be returned.
 *
 * @return The current value of the BFMERSecNrErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_sec_nr_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00020000)) >> 17);
}

/**
 * @brief Returns the current value of the BFMERSecHdrAddrErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERSecHdrAddrErr field's value will be returned.
 *
 * @return The current value of the BFMERSecHdrAddrErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_sec_hdr_addr_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00010000)) >> 16);
}

/**
 * @brief Returns the current value of the BFMERPrimBwErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERPrimBwErr field's value will be returned.
 *
 * @return The current value of the BFMERPrimBwErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_prim_bw_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000010)) >> 4);
}

/**
 * @brief Returns the current value of the BFMERPrimGroupingErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERPrimGroupingErr field's value will be returned.
 *
 * @return The current value of the BFMERPrimGroupingErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_prim_grouping_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000008)) >> 3);
}

/**
 * @brief Returns the current value of the BFMERPrimNcErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERPrimNcErr field's value will be returned.
 *
 * @return The current value of the BFMERPrimNcErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_prim_nc_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000004)) >> 2);
}

/**
 * @brief Returns the current value of the BFMERPrimNrErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERPrimNrErr field's value will be returned.
 *
 * @return The current value of the BFMERPrimNrErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_prim_nr_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000002)) >> 1);
}

/**
 * @brief Returns the current value of the BFMERPrimHdrAddrErr field in the BFMERERRSTAT register.
 *
 * The BFMERERRSTAT register will be read and the BFMERPrimHdrAddrErr field's value will be returned.
 *
 * @return The current value of the BFMERPrimHdrAddrErr field in the BFMERERRSTAT register.
 */
__INLINE uint8_t mdm_bfmer_prim_hdr_addr_err_getf(void)
{
    uint32_t localVal = REG_PL_RD(MDM_BFMERERRSTAT_ADDR);
    return ((localVal & ((uint32_t)0x00000001)) >> 0);
}

/// @}


/// Address of the BFMERSTAT3 register
#define MDM_BFMERSTAT3_ADDR   (REG_MDM_STAT_BASE_ADDR+0x002C)
/// Offset of the BFMERSTAT3 register from the base address
#define MDM_BFMERSTAT3_OFFSET 0x0000002C
/// Index of the BFMERSTAT3 register
#define MDM_BFMERSTAT3_INDEX  (MDM_BFMERSTAT3_OFFSET>>2)
/// Reset value of the BFMERSTAT3 register
#define MDM_BFMERSTAT3_RESET  0x00000000


/// Address of the BFMERSTAT4 register
#define MDM_BFMERSTAT4_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0030)
/// Offset of the BFMERSTAT4 register from the base address
#define MDM_BFMERSTAT4_OFFSET 0x00000030
/// Index of the BFMERSTAT4 register
#define MDM_BFMERSTAT4_INDEX  (MDM_BFMERSTAT4_OFFSET>>2)
/// Reset value of the BFMERSTAT4 register
#define MDM_BFMERSTAT4_RESET  0x00000000


/// Address of the EQUALSTAT0 register
#define MDM_EQUALSTAT0_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0034)
/// Offset of the EQUALSTAT0 register from the base address
#define MDM_EQUALSTAT0_OFFSET 0x00000030
/// Index of the EQUALSTAT0 register

/// Reset value of the EQUALSTAT0 register
#define MDM_EQUALSTAT0_RESET  0x00000000


/// Address of the EQUALSTAT1 register
#define MDM_EQUALSTAT1_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0038)
/// Offset of the EQUALSTAT1 register from the base address
#define MDM_EQUALSTAT1_OFFSET 0x00000038
/// Index of the EQUALSTAT1 register
#define MDM_EQUALSTAT1_INDEX  (MDM_EQUALSTAT0_OFFSET>>2)
/// Reset value of the EQUALSTAT1 register
#define MDM_EQUALSTAT1_RESET  0x00000000


/// Address of the HDMVERSION register
#define HDMVERSION_ADDR   (REG_MDM_STAT_BASE_ADDR+0x003C)
/// Offset of the HDMVERSION register from the base address
#define HDMVERSION_OFFSET 0x0000003C
/// Index of the HDMVERSION register
#define HDMVERSION_INDEX  (HDMVERSION_OFFSET>>2)
/// Reset value of the HDMVERSION register
#define HDMVERSION_RESET  0x00010001


/// Address of the MDM_BFMERSTAT5 register
#define MDM_BFMERSTAT5_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0040)
/// Offset of the BFMERSTAT5 register from the base address
#define MDM_BFMERSTAT5_OFFSET 0x00000040
/// Index of the BFMERSTAT5 register
#define MDM_BFMERSTAT5_INDEX  (MDM_BFMERSTAT5_OFFSET>>2)
/// Reset value of the BFMERSTAT5 register
#define MDM_BFMERSTAT5_RESET  0x00000000


/// Address of the MDM_BFMERSTAT6 register
#define MDM_BFMERSTAT6_ADDR   (REG_MDM_STAT_BASE_ADDR+0x0044)
/// Offset of the BFMERSTAT6 register from the base address
#define MDM_BFMERSTAT6_OFFSET 0x00000044
/// Index of the BFMERSTAT6 register
#define MDM_BFMERSTAT6_INDEX  (MDM_BFMERSTAT6_OFFSET>>2)
/// Reset value of the BFMERSTAT6 register
#define MDM_BFMERSTAT6_RESET  0x00000000

#endif // _REG_MDM_STAT_H_

/// @}

