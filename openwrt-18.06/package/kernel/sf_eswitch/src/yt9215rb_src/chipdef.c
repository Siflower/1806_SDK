/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "chipdef.h"
#include "yt_board.h"


/* data define */
const yt_swchip_cap_t * const gpChipCapList[] =
{
#ifdef SWITCH_SERIES_TIGER
	[YT_SW_MODEL_9215] = &yt9215_capacity,
	[YT_SW_MODEL_9218] = &yt9218_capacity,
	[YT_SW_MODEL_9218N] = &yt9218_capacity,
	[YT_SW_MODEL_9213] = &yt9215_capacity,
	[YT_SW_MODEL_9214] = &yt9215_capacity,
#endif
#ifdef SWITCH_SERIES_SHARK
	[YT_SW_MODEL_9232] = &yt9232_capacity,
	[YT_SW_MODEL_9230] = &yt9232_capacity,
	[YT_SW_MODEL_9231] = &yt9232_capacity,
	[YT_SW_MODEL_9231_6] = &yt9232_capacity,
	[YT_SW_MODEL_9232_26] = &yt9232_capacity,
#endif
#ifdef SWITCH_SERIES_WHALE
	[YT_SW_MODEL_9228] = &yt922x_capacity,
	[YT_SW_MODEL_9224] = &yt922x_capacity,
#endif
};

const yt_swchip_if_info_t * const gpChipIntfInfoList[] =
{
#ifdef SWITCH_SERIES_TIGER
	[YT_SW_MODEL_9215] = &yt9215_intf_info,
	[YT_SW_MODEL_9218] = &yt9218_intf_info,
	[YT_SW_MODEL_9218N] = &yt9218n_intf_info,
	[YT_SW_MODEL_9213] = &yt9213_intf_info,
	[YT_SW_MODEL_9214] = &yt9214_intf_info,
#endif
#ifdef SWITCH_SERIES_SHARK
	[YT_SW_MODEL_9230] = &yt9230_intf_info,
	[YT_SW_MODEL_9231] = &yt9231_intf_info,
	[YT_SW_MODEL_9231_6] = &yt9231_6_intf_info,
	[YT_SW_MODEL_9232] = &yt9232_intf_info,
	[YT_SW_MODEL_9232_26] = &yt9232_26_intf_info,
#endif
#ifdef SWITCH_SERIES_WHALE
	[YT_SW_MODEL_9228] = &yt9228_intf_info,
	[YT_SW_MODEL_9224] = &yt9224_intf_info,
#endif
};

uint8_t chipdef_get_extif_by_macid(yt_macid_t mac_id, const yt_swchip_if_info_t *pChipIfInfo)
{
    uint8_t extif_id = INVALID_ID;
    uint8_t i;

    for(i = 0; i < pChipIfInfo->extif_num; i++)
    {
        if(mac_id >= pChipIfInfo->extIf[i].mac_start_id &&
            mac_id <= pChipIfInfo->extIf[i].mac_end_id)
        {
            extif_id = pChipIfInfo->extIf[i].extif_id;
            break;
        }
    }

    return extif_id;
}
