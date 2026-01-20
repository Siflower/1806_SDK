/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/
#include "cal_bprofile.h"
#include "yt_error.h"
#include "cal_cmm.h"
#include "yt_cmm.h"
#include "yt_board.h"

static yt_ret_t cal_profile_cfgBoard_init(yt_hwProfile_info_t *hwProfile);
static yt_ret_t yt_board_profile_cfgBoard_register(void);

/*
 * Data Type Definition
 */

yt_hwProfile_info_t	gBoardInfo;
const yt_swDescp_t	*gpSwitchUnit[YT_MAX_UNIT];

/* list of hardware profiles */
static yt_bool_t gBoardDebugEn = FALSE;
static const yt_hwProfile_t *gHwProfile[BOARD_PROFILE_MAX_NUM];
static yt_cfgBoardInfo_t cfgBoardInfo;
const yt_hwProfile_t cfgBoardHwProfile =
{
    .pIdentifier = &cfgBoardInfo.boardIdentify,
    .profile_init = cal_profile_cfgBoard_init,
};

/*
 * Function Declaration
 */

/*****************************************************************************
*   Prototype    : cal_board_profile_init
*   Description  : init board profile info
*   Input        : void
*   Output       : None
*   Return Value : uint32_t
*
*****************************************************************************/
uint32_t cal_board_profile_init(void)
{
    const yt_hwProfile_t **pbProfile = gHwProfile;
    uint8_t	i;
    uint8_t j = 0;
    uint32_t boardId;
    
    if (cfgBoardInfo.valid)
    {
        OSAL_DEBUG(gBoardDebugEn, ("register for manual cfg board profile\n"));
        yt_board_profile_cfgBoard_register();
    }

    yt_board_profile_id_get(&boardId);
    OSAL_DEBUG(gBoardDebugEn, ("boardId %u\n", boardId));
    while (*pbProfile != NULL)
    {
        if ((boardId == INVALID_BOARD_ID) || (boardId == (*pbProfile)->pIdentifier->id))
        {
            if ((*pbProfile)->profile_init)
            {
                (*pbProfile)->profile_init(&gBoardInfo);
            }

            for (i=0; i<gBoardInfo.switch_count; i++)
            {
                gpSwitchUnit[i] = gBoardInfo.pSwDescp[i];

                for(j = 0; j < gBoardInfo.pSwDescp[i]->port_num; j++)
                {
                    if(gBoardInfo.pSwDescp[i]->pPortDescp[j] &&
                        gBoardInfo.pSwDescp[i]->pPortDescp[j]->mac_id != INVALID_ID)
                    {
                        CMM_SET_MEMBER_PORT(gBoardInfo.portmask[i], j);
                        CMM_SET_MEMBER_PORT(gBoardInfo.allportmask[i], j);
                        CMM_SET_MEMBER_PORT(gBoardInfo.macmask[i],
                            gBoardInfo.pSwDescp[i]->pPortDescp[j]->mac_id);
                    }
                }
                OSAL_DEBUG(gBoardDebugEn, ("switch %u portnum %u portmask 0x%x macmask 0x%x\n", 
                            i, gBoardInfo.pSwDescp[i]->port_num, gBoardInfo.portmask[i].portbits[0], gBoardInfo.macmask[i].portbits[0]));

                gBoardInfo.pSwDescp[i]->pChipCap = gpChipCapList[gBoardInfo.pSwDescp[i]->chip_model];
                gBoardInfo.pSwDescp[i]->pChipIfInfo = gpChipIntfInfoList[gBoardInfo.pSwDescp[i]->chip_model];
            }
            
            return CMM_ERR_OK;
        }
        pbProfile++;
    }

    return CMM_ERR_FAIL;
}

yt_ret_t cal_board_ext_phy_addr_set(yt_unit_t unit, yt_port_t port, uint8_t phyAddr)
{
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);

    UNITINFO(unit)->pPortDescp[port]->phy_addr &= 0xFF;
    UNITINFO(unit)->pPortDescp[port]->phy_addr |= phyAddr << 8;
    
    return CMM_ERR_OK;

}

yt_ret_t cal_board_ext_phy_addr_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr)
{
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);

    *pPhyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);

    return CMM_ERR_OK;
}

yt_ret_t cal_board_port_attribute_set(yt_unit_t unit, yt_port_t port, yt_port_attri_t attribute)
{
    CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, port))), CMM_ERR_PORT);
    CMM_PARAM_CHK((YT_UNIT_NUM <= unit), CMM_ERR_INPUT);

    UNITINFO(unit)->pPortDescp[port]->attribute = attribute;
    
    return CMM_ERR_OK;

}

yt_ret_t cal_board_port_num_get(yt_unit_t unit, uint32_t *pMaxPorts)
{
    CMM_PARAM_CHK((NULL == pMaxPorts), CMM_ERR_INPUT);
    if (unit >= YT_UNIT_NUM)
    {
        *pMaxPorts = 0;
    }
    else
    {
        *pMaxPorts = CAL_PORT_NUM_ON_UNIT(unit);
    }

    return CMM_ERR_OK;
}

yt_ret_t cal_board_profile_register(const yt_hwProfile_t *profile)
{
    uint32_t i;
    uint32_t pos = INVALID_BOARD_ID;

    CMM_PARAM_CHK(NULL == profile, CMM_ERR_INPUT);
    for (i = 0; i < BOARD_PROFILE_MAX_NUM; i++)
    {
        if (gHwProfile[i] == NULL)
        {
            if (pos == INVALID_BOARD_ID)
            {
                pos = i;
            }
        }
        else
        {
            if (gHwProfile[i] == profile)
            {
                return CMM_ERR_SAMEENTRY_EXIST;
            }
        }
    }

    if (pos == INVALID_BOARD_ID)
    {
        return CMM_ERR_ENTRY_FULL;
    }

    gHwProfile[pos] = profile;

    return CMM_ERR_OK;
}

yt_ret_t cal_board_profile_unregister(uint32_t boardId)
{
    uint32_t i;
    uint32_t found = 0U;

    for (i = 0; i < BOARD_PROFILE_MAX_NUM; i++)
    {
        if (gHwProfile[i] != NULL)
        {
            if ((gHwProfile[i]->pIdentifier != NULL) && (gHwProfile[i]->pIdentifier->id == boardId))
            {
                gHwProfile[i] = NULL;
                found = 1U;
            }
        }
    }

    if (found == 0U)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }
    else
    {
        return CMM_ERR_OK;
    }
}

static yt_ret_t cal_profile_cfgBoard_init(yt_hwProfile_info_t *hwProfile)
{
    uint8_t i = 0;

    if (!cfgBoardInfo.valid)
    {
        return CMM_ERR_FAIL;
    }

    hwProfile->pIdentifier = &cfgBoardInfo.boardIdentify;
    hwProfile->switch_count = cfgBoardInfo.switchCnt;
    OSAL_DEBUG(gBoardDebugEn, ("switchCnt %u board ID %u\n", cfgBoardInfo.switchCnt, cfgBoardInfo.boardIdentify.id));
    for (i = 0; i < cfgBoardInfo.switchCnt; i++)
    {
        hwProfile->pSwDescp[i] = &cfgBoardInfo.swDesc[i];  
    }

    return CMM_ERR_OK;
}

static yt_ret_t yt_board_profile_cfgBoard_register(void)
{
    return cal_board_profile_register(&cfgBoardHwProfile);
}

void cal_board_profile_clear(void)
{
    if (cfgBoardInfo.valid)
    {
        cal_board_profile_unregister(cfgBoardInfo.boardIdentify.id);
    }

    osal_memset(&cfgBoardInfo, sizeof(cfgBoardInfo), 0, sizeof(cfgBoardInfo));

    return;
}

yt_ret_t cal_board_profile_set(yt_board_desc_cfg_t *pBoardDesc)
{
    uint32_t i = 0;
    uint8_t portId = 0;
    yt_switch_desc_cfg_t *pSwDesc = NULL;

    CMM_PARAM_CHK((pBoardDesc == NULL), CMM_ERR_NULL_POINT);
    if (pBoardDesc->valid)
    {
        CMM_PARAM_CHK(((pBoardDesc->switchCnt == 0) || (pBoardDesc->switchCnt > YT_MAX_UNIT)), CMM_ERR_INPUT);
        for (i = 0; i < pBoardDesc->switchCnt; i++)
        {
            pSwDesc = &pBoardDesc->swDesc[i];
            CMM_PARAM_CHK(((pSwDesc->portNum == 0) || (pSwDesc->portNum > YT_MAX_PORT_PER_UNIT)), CMM_ERR_INPUT);
            CMM_PARAM_CHK((pSwDesc->swChipModel >= YT_SW_MODEL_END), CMM_ERR_INPUT); 
            for (portId = 0; portId < pSwDesc->portNum; portId++) 
            {
                CMM_PARAM_CHK((pSwDesc->portDesc[portId].mac_id > YT_MAX_PORT_PER_UNIT), CMM_ERR_EXCEED_RANGE);
                CMM_PARAM_CHK((pSwDesc->portDesc[portId].phyType > YT_PHY_INTEXT), CMM_ERR_INPUT);   
            }
        }

        cal_board_profile_clear();
    
        cfgBoardInfo.boardIdentify.id = pBoardDesc->boardId;
        cfgBoardInfo.switchCnt = pBoardDesc->switchCnt;
        OSAL_DEBUG(gBoardDebugEn, ("set board cfg switchCnt %u board ID %u\n", cfgBoardInfo.switchCnt, cfgBoardInfo.boardIdentify.id));
        for (i = 0; i < pBoardDesc->switchCnt; i++)
        {
            pSwDesc = &pBoardDesc->swDesc[i];
            cfgBoardInfo.swDesc[i].chip_id = pSwDesc->swChipId;
            cfgBoardInfo.swDesc[i].chip_model = pSwDesc->swChipModel;
            cfgBoardInfo.swDesc[i].port_num = pSwDesc->portNum;
            cfgBoardInfo.swDesc[i].sw_access = pSwDesc->swAccess;
            OSAL_DEBUG(gBoardDebugEn, ("swChipId 0x%x swChipModel %u portNum %u\n",
                        pSwDesc->swChipId, pSwDesc->swChipModel, pSwDesc->portNum));
            for (portId = 0; portId < pSwDesc->portNum; portId++) 
            {
                cfgBoardInfo.portDesc[i][portId] = pSwDesc->portDesc[portId];
                cfgBoardInfo.swDesc[i].pPortDescp[portId] = &cfgBoardInfo.portDesc[i][portId];
                OSAL_DEBUG(gBoardDebugEn, ("sw %u port %u macId %u attribute %u phyAddr 0x%x extifMode %u speedDuplex %u medium %u phyType %u\n",
                    i, portId, cfgBoardInfo.portDesc[i][portId].mac_id, cfgBoardInfo.portDesc[i][portId].attribute,
                    cfgBoardInfo.portDesc[i][portId].phy_addr, cfgBoardInfo.portDesc[i][portId].extifMode,
                    cfgBoardInfo.portDesc[i][portId].speedDuplex, cfgBoardInfo.portDesc[i][portId].medium,
                    cfgBoardInfo.portDesc[i][portId].phyType));
            }
        }
        cfgBoardInfo.valid = pBoardDesc->valid;
    }
    else
    {
        /* clear soft data when valid is 0 */
        OSAL_DEBUG(gBoardDebugEn, ("clear board cfg\n"));
        cal_board_profile_clear();
    }

    return CMM_ERR_OK;
}

/* TODO: select default board profile according to chip id and model, if did not spcific */
