/******************************************************************************
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
*******************************************************************************
*  File Name     : bprofile_YT9215RB_default_demo.c
*  Version       : Initial Draft
*  Created       : 
*  Last Modified :
*  Description   : board profile for yt9215rb board,port(5+2 rgmii)
*
******************************************************************************/

/**************************************************
 *      Include header files                       *
 **************************************************/
#include "cal_bprofile.h"
#include "yt_error.h"
#include "bprofile_default_demo.h"

/**************************************************
 *      Constants or macros Declaration            *
 **************************************************/

/**************************************************
 *      Global variables                           *
 **************************************************/

/**************************************************
 *      Functions Declaration                      *
 **************************************************/
yt_ret_t cal_profile_yt9215rb_init(yt_hwProfile_info_t *hwprofile_info);

yt_swDescp_t yt9215rb_swDescp;

const board_profile_identify_t YT9215rbProfileIdentifier = {BOARD_ID_YT9215RB, "YT9215RB Default Demo"};

/* port descp */
yt_portDescp_t YT9215rbPortDescp[] =
{
    /*macid	attribute		 phy_addr	                    extifMode	            speedDuplex			        medium				phyType */
    {0,		PORT_ATTR_PHY,	 CFG_PHYADDR(0, INVALID_ID),	INVALID_ID,		        PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_INT},
    {1,		PORT_ATTR_PHY,	 CFG_PHYADDR(1, INVALID_ID),	INVALID_ID,		        PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_INT},
    {2,		PORT_ATTR_PHY,	 CFG_PHYADDR(2, INVALID_ID),	INVALID_ID,		        PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_INT},
    {3,		PORT_ATTR_PHY,	 CFG_PHYADDR(3, INVALID_ID),	INVALID_ID,		        PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_INT},
    {4,		PORT_ATTR_PHY,	 CFG_PHYADDR(4, INVALID_ID),	INVALID_ID,		        PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_INT},
    {8,		PORT_ATTR_XMII,	 INVALID_PHYID_ALL,			    YT_EXTIF_MODE_RGMII,	PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_EXT},
    {9,		PORT_ATTR_XMII,	 INVALID_PHYID_ALL,			    YT_EXTIF_MODE_RGMII,	PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_EXT},
#ifdef INTER_MCU
    /* internel cpu port */
    {10,    PORT_ATTR_INT_CPU, INVALID_PHYID_ALL,	INVALID_ID,	PORT_SPEED_DUP_1000FULL,	PORT_MEDI_COPPER,	YT_PHY_NONE},
#endif
    {INVALID_ID,INVALID_ID,INVALID_PHYID_ALL,INVALID_ID,INVALID_ID,INVALID_ID,YT_PHY_NONE},
};

/* hardware profile */
const yt_hwProfile_t yt9215rb_default_demo =
{
    .pIdentifier = &YT9215rbProfileIdentifier,
    .profile_init = cal_profile_yt9215rb_init,
};

yt_ret_t cal_profile_yt9215rb_init(yt_hwProfile_info_t *hwprofile_info)
{
    uint8_t	i;

    hwprofile_info->pIdentifier = &YT9215rbProfileIdentifier;

    /* switch info */
    hwprofile_info->switch_count = 1;
    printk("%s %d \n", __func__, __LINE__);
    yt9215rb_swDescp.chip_id = YT_SW_ID_9215;
    yt9215rb_swDescp.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(YT9215rbPortDescp[i].mac_id != INVALID_ID)
    {
        yt9215rb_swDescp.pPortDescp[i] = &YT9215rbPortDescp[i];
        i++;
    }
    yt9215rb_swDescp.port_num = i;
    
    hwprofile_info->pSwDescp[0] = &yt9215rb_swDescp;

    return CMM_ERR_OK;
}

yt_ret_t yt_board_profile_yt9215rb_register(void)
{
    printk("%s %d \n", __func__, __LINE__);
    return cal_board_profile_register(&yt9215rb_default_demo);
}
