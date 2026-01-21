/*******************************************************************************
*                                                                              *
*  Copyright (c), 2024, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_frer.h
*
* @brief x
*
********************************************************************************
*/
#ifndef YT_GPIO_H
#define YT_GPIO_H

#include "yt_cmm.h"

/*
typedef struct
{
  uint8 mdiom1_mode;
  uint8 mdiom2_mode;
#if defined(SWITCH_SERIES_TIGER)
  uint8 mdiom3_mode;
#endif
}yt_mdio_master_cfg;  */

typedef enum yt_mdio_master_e
{
    YT_MDIO_MASTER_1 = 1,
    YT_MDIO_MASTER_2,
#if defined(SWITCH_SERIES_TIGER)
    YT_MDIO_MASTER_3,
#endif
}yt_mdio_master_t;

/**
 * @internal    yt_gpion_init
 * @endinternal
 *
 * @brief       Init gipo module
 * @note        APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]   unit                -unit id
 * @retval      CMM_ERR_OK          -on success
 * @retval      CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_gpio_init(yt_unit_t unit);

/**
 * @internal    yt_gpio_mdioMaster_pinGroup_set
 * @endinternal
 *
 * @brief       Set mdio master pin group enable/disable
 * @note        APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]   unit                -unit id
 * @param[in]   mdiomcfg            -set mdio master parameters
 * @retval      CMM_ERR_OK          -on success
 * @retval      CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_gpio_mdioMaster_pinGroup_set(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t enable);

/**
 * @internal    yt_gpio_mdioMaster_pinGroup_get
 * @endinternal
 *
 * @brief       Get mdio master pin group enable/disable
 * @note        APPLICABLE DEVICES  -Tiger,Shark
 * @param[in]   unit                -unit id
 * @param[in]   mdiomcfg            -get mdio master parameters
 * @retval      CMM_ERR_OK          -on success
 * @retval      CMM_ERR_FAIL        -on fail
 */
extern yt_ret_t  yt_gpio_mdioMaster_pinGroup_get(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t *pEnable);

#endif