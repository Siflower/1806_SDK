/*******************************************************************************
*                                                                              *
*  Copyright (c), 2024, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file fal_tiger_gpio.h
*
* @brief x
*
********************************************************************************
*/

#ifndef FAL_TIGER_GPIO_H
#define FAL_TIGER_GPIO_H

#include "yt_types.h"
#include "yt_gpio.h"


/*
 * Macro Declaration
 */
#define MODE_SEL        0x80358


extern yt_ret_t  fal_tiger_gpio_init(yt_unit_t unit);
extern yt_ret_t  fal_tiger_gpio_mdioMaster_pinGroup_set(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t enable);
extern yt_ret_t  fal_tiger_gpio_mdioMaster_pinGroup_get(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t *pEnable);


#endif