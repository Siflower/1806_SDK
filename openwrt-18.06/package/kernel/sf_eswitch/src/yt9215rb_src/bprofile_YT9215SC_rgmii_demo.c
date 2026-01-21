/******************************************************************************
*                                                                             *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.              *
*  Motorcomm Confidential and Proprietary.                                    *
*                                                                             *
*******************************************************************************
*  File Name     : bprofile_YT9215SC_rgmii_demo.c
*  Version       :
*  Created       :
*  Last Modified :
*  Description   : board profile for yt9215sc board,port(5+ SerDes(gmac8/port5) + rgmii(gmac9/port6))
*
******************************************************************************/

/**************************************************
 *      Include header files                       *
 **************************************************/
#include "cal_bprofile.h"
#include "yt_error.h"
/**************************************************
 *      Constants or macros Declaration            *
 **************************************************/

/**************************************************
 *      Global variables                           *
 **************************************************/

/**************************************************
 *      Functions Declaration                      *
 **************************************************/
yt_ret_t cal_profile_yt9215sc_rgmii_init(yt_hwProfile_info_t *hwprofile_info);

yt_swDescp_t yt9215sc_rgmii_swDescp;

const board_profile_identify_t YT9215scRgmiiProfileIdentifier = {BOARD_ID_YT9215SC, "YT9215SC SerDes + rgmii(gmac9)"};

/* port descp */
const yt_portDescp_t YT9215scRgmiiPortDescp[] =
{
    /*macid	attribute		phy_index	phy_addr	serdes_index	ethtype			medium				smi */
    {0,		PORT_ATTR_ETH,	0,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {1,		PORT_ATTR_ETH,	0,			1,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {2,		PORT_ATTR_ETH,	0,			2,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {3,		PORT_ATTR_ETH,	0,			3,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {4,		PORT_ATTR_ETH,	0,			4,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_INT},
    {8,		PORT_ATTR_ETH,	INVALID_ID,	8,	        0,              ETH_TYPE_GE,	PORT_MEDI_FIBER,	YT_SMI_EXT},
    {9,		PORT_ATTR_EXT_RGMII,	INVALID_ID,			0,			INVALID_ID,		ETH_TYPE_GE,	PORT_MEDI_COPPER,	YT_SMI_EXT},
#ifdef INTER_MCU
    /* internel cpu port */
    {10,    PORT_ATTR_INT_CPU,			    INVALID_ID,	INVALID_ID,	PORT_ATTR_ETH,	ETH_TYPE_GE,	PORT_MEDI_COPPER,	INVALID_ID},
#endif
    {INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID,YT_SMI_NONE},
};

/* serdes.descp */
const yt_serdesDescp_t YT9215scSdsDescp[] =
{
	/*serdes_id mode*/
	{0,              SERDES_MODE_1000BX},
	{INVALID_ID,INVALID_ID},
};

/* phy.descp */
const yt_phyDescp_t YT9215scRgmiiPhyDescp[] =
{
	/*chip_id chip_model				start_mac_id	phy_max*/
	{0,		  YT_PHY_MODEL_INT861X,	0,				5},
	{INVALID_ID,INVALID_ID,INVALID_ID,INVALID_ID},
};

/* LED description */
const yt_sled_remapInfo_t YT9215scRgmiiRemapInfo[] =
{
    {3, 0}, {2, 0}, {1, 0}, {0, 0},{6, 1}, {5, 1}, {4, 1},
    {3, 1}, {2, 1}, {1, 1}, {0, 1},{6, 2}, {5, 2}, {4, 2},
    {3, 2}, {2, 2}, {1, 2}, {0, 2}, {6, 0}, {5, 0}, {4, 0}
};

yt_sled_param_t YT9215scRgmiiSLEDParam = {
    LED_SERIAL_ACTIVE_MODE_LOW,
    SLED_DATANUM_21,
    0,
    YT9215scRgmiiRemapInfo
};

const yt_ledDescp_t YT9215scRgmiiLEDDescp = {LED_MODE_SERIAL, &YT9215scRgmiiSLEDParam};


/* hardware profile */
const yt_hwProfile_t yt9215sc_rgmii_default_demo =
{
    .pIdentifier = &YT9215scRgmiiProfileIdentifier,
    .profile_init = cal_profile_yt9215sc_rgmii_init,
};

yt_ret_t cal_profile_yt9215sc_rgmii_init(yt_hwProfile_info_t *hwprofile_info)
{
    uint8_t	i;

    hwprofile_info->pIdentifier = &YT9215scRgmiiProfileIdentifier;

    /* switch info */
    hwprofile_info->switch_count = 1;

    yt9215sc_rgmii_swDescp.chip_id = YT_SW_ID_9215;
    yt9215sc_rgmii_swDescp.chip_model = YT_SW_MODEL_9215;

    i = 0;
    while(YT9215scRgmiiPortDescp[i].mac_id != INVALID_ID)
    {
        yt9215sc_rgmii_swDescp.pPortDescp[i] = &YT9215scRgmiiPortDescp[i];
        i++;
    }
    yt9215sc_rgmii_swDescp.port_num = i;

    i = 0;
    while(YT9215scSdsDescp[i].serdes_id!= INVALID_ID)
    {
        yt9215sc_rgmii_swDescp.pSerdesDescp[i] = &YT9215scSdsDescp[i];
        i++;
    }

    i = 0;
    while(YT9215scRgmiiPhyDescp[i].phy_index!= INVALID_ID)
    {
        yt9215sc_rgmii_swDescp.pPhyDescp[i] = &YT9215scRgmiiPhyDescp[i];
        i++;
    }

    YT9215scRgmiiSLEDParam.remapInfoNum = sizeof(YT9215scRgmiiRemapInfo)/sizeof(yt_sled_remapInfo_t);
    yt9215sc_rgmii_swDescp.pLEDDescp = &YT9215scRgmiiLEDDescp;

    hwprofile_info->pSwDescp[0] = &yt9215sc_rgmii_swDescp;

    return CMM_ERR_OK;
}
