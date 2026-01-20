/*******************************************************************************
*                                                                              *
*  Copyright (c), 2024, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
/**
********************************************************************************
* @file yt_gpio.c
*
* @brief x
*
********************************************************************************
*/
#include "yt_gpio.h"
#include "fal_dispatch.h"


yt_ret_t yt_gpio_init(yt_unit_t unit)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->gpio_init(unit);
}

yt_ret_t yt_gpio_mdioMaster_pinGroup_set(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t enable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    return YT_DISPATCH(unit)->gpio_mdioMaster_pinGroup_set(unit, mdiom, enable);
}

yt_ret_t yt_gpio_mdioMaster_pinGroup_get(yt_unit_t unit, yt_mdio_master_t mdiom, yt_enable_t *pEnable)
{
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);
    CMM_PARAM_CHK(NULL == YT_DISPATCH(unit), CMM_ERR_NOT_INIT);

    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    return YT_DISPATCH(unit)->gpio_mdioMaster_pinGroup_get(unit, mdiom, pEnable);
}