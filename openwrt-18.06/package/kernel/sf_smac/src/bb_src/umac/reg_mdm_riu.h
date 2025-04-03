/*
 * =====================================================================================
 *
 *       Filename:  reg_mdm_cfg.h
 *
 *    Description:  Definitions of the MDM HW block registers and register access functions.
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *        Company:  Siflower
 *
 * =====================================================================================
 */

#ifndef _REG_MDM_RIU_H_
#define _REG_MDM_RIU_H_
#include "reg_access.h"

//#define REG_MDM_CFG_BASE_ADDR (WIFI_BASE_ADDR+0x00100000)

/*  Modem Config */
#define MDM_CLKGATEFCTRL0_ADDR(band)   (REG_MDM_CFG_BASE_ADDR(band)+0x0874)


/**
 * @brief Returns the current value of the AGCCLKFORCE field in the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read and the AGCCLKFORCE field's value will be returned.
 *
 * @return The current value of the AGCCLKFORCE field in the CLKGATEFCTRL0 register.
 */
__INLINE uint8_t mdm_agcclkforce_getf(struct siwifi_hw *siwifi_hw)
{
    uint32_t localVal = REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR(siwifi_hw->mod_params->is_hb));
    return ((localVal & ((uint32_t)0x10000000)) >> 28);
}

/**
 * @brief Sets the AGCCLKFORCE field of the CLKGATEFCTRL0 register.
 *
 * The CLKGATEFCTRL0 register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcclkforce - The value to set the field to.
 */
__INLINE void mdm_agcclkforce_setf(struct siwifi_hw *siwifi_hw, uint8_t agcclkforce)
{
    ASSERT_ERR((((uint32_t)agcclkforce << 28) & ~((uint32_t)0x10000000)) == 0);
    REG_PL_WR(MDM_CLKGATEFCTRL0_ADDR(siwifi_hw->mod_params->is_hb), (REG_PL_RD(MDM_CLKGATEFCTRL0_ADDR(siwifi_hw->mod_params->is_hb)) & ~((uint32_t)0x10000000)) | ((uint32_t)agcclkforce << 28));
}


//#define REG_RIU_BASE_ADDR (WIFI_BASE_ADDR+0x00100000)
/// Address of the SIWIFIAGCCNTL register
#define RIU_SIWIFIAGCCNTL_ADDR(band)   (REG_RIU_BASE_ADDR(band)+0xB390)

/// Address of the SIWIFIDYNAMICCONFIG register
#define REG_RIU_SIWIFIDYNAMICCONFIG_OFFSET 0xB008
#define BIT_RIU_SIWIFIDYNAMICCONFIG_AGCMEMBISTSTART 28

///Address of the AGCMEMBISTSTAT register
#define REG_RIU_AGCMEMBISTSTAT_OFFSET 0xB238


/**
 * @brief Returns the current value of the AGCFSMRESET field in the SIWIFIAGCCNTL register.
 *
 * The SIWIFIAGCCNTL register will be read and the AGCFSMRESET field's value will be returned.
 *
 * @return The current value of the AGCFSMRESET field in the SIWIFIAGCCNTL register.
 */
__INLINE uint8_t riu_agcfsmreset_getf(struct siwifi_hw *siwifi_hw)
{
    uint32_t localVal = REG_PL_RD(RIU_SIWIFIAGCCNTL_ADDR(siwifi_hw->mod_params->is_hb));
    return ((localVal & ((uint32_t)0x00001000)) >> 12);
}

/**
 * @brief Sets the AGCFSMRESET field of the SIWIFIAGCCNTL register.
 *
 * The SIWIFIAGCCNTL register will be read, modified to contain the new field value, and written.
 *
 * @param[in] agcfsmreset - The value to set the field to.
 */
__INLINE void riu_agcfsmreset_setf(struct siwifi_hw *siwifi_hw, uint8_t agcfsmreset)
{
    ASSERT_ERR((((uint32_t)agcfsmreset << 12) & ~((uint32_t)0x00001000)) == 0);
    REG_PL_WR(RIU_SIWIFIAGCCNTL_ADDR(siwifi_hw->mod_params->is_hb), (REG_PL_RD(RIU_SIWIFIAGCCNTL_ADDR(siwifi_hw->mod_params->is_hb)) & ~((uint32_t)0x00001000)) | ((uint32_t)agcfsmreset << 12));
}


#endif
