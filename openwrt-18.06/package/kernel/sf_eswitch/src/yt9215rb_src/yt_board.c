/*******************************************************************************
*                                                                              *
*  Copyright (c), 2024, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "yt_types.h"
#include "yt_error.h"
#include "yt_board.h"
#include "cal_bprofile.h"

static uint32_t gBoardId = INVALID_BOARD_ID;

uint32_t yt_board_profile_id_set(uint32_t boardId)
{
    gBoardId = boardId;

    return CMM_ERR_OK;
}


void yt_board_profile_id_get(uint32_t *pBoardId)
{
    *pBoardId = gBoardId;

    return;
}

yt_ret_t yt_board_ext_phy_addr_set(yt_unit_t unit, yt_port_t port, uint8_t phyAddr)
{
    return cal_board_ext_phy_addr_set(unit, port, phyAddr);
}

yt_ret_t yt_board_ext_phy_addr_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr)
{
    return cal_board_ext_phy_addr_get(unit, port, pPhyAddr);
}


void yt_board_profile_clear(void)
{
    cal_board_profile_clear();

    return;
}

yt_ret_t yt_board_profile_set(yt_board_desc_cfg_t *pBoardDesc)
{
    return cal_board_profile_set(pBoardDesc);
}

yt_ret_t yt_board_port_num_get(yt_unit_t unit, uint32_t *pMaxPorts)
{
    return cal_board_port_num_get(unit, pMaxPorts);
}

#if defined(LINUX_KERNEL_MODE)
EXPORT_SYMBOL(yt_board_port_num_get);
#endif