#include "yt_error.h"
#include "cal_mgm.h"
#include "cal_cmm.h"

uint32_t cal_ytportmask_isvalid(yt_unit_t unit, yt_port_mask_t port_mask, yt_bool_t incCpuPort)
{
    uint8_t i = 0;

    CMM_PARAM_CHK(!gcal_inited, CMM_ERR_FAIL);

    for(i = 0; i < YT_PORTS_WORD_NUM; i++)
    {
        if (TRUE == incCpuPort)
        {
            if(port_mask.portbits[i] & (~gBoardInfo.allportmask[unit].portbits[i]))
            {
                return CMM_ERR_FAIL;
            }
        }
        else
        {
            if(port_mask.portbits[i] & (~gBoardInfo.portmask[unit].portbits[i]))
            {
                return CMM_ERR_FAIL;
            }
        }
    }

    return CMM_ERR_OK;
}

/* add or remove port to yt port mask, for cpu port set */
uint32_t cal_ytportmask_update(yt_unit_t unit, yt_port_t port, yt_bool_t flag)
{
    if(TRUE == flag)
    {
        CMM_SET_MEMBER_PORT(gBoardInfo.portmask[unit], port);
    }
    else
    {
        CMM_RESET_MEMBER_PORT(gBoardInfo.portmask[unit], port);
    }
    
    return CMM_ERR_OK;
}

uint8_t cal_phy_addr_get(yt_unit_t unit, yt_port_t port)
{
    uint8_t phyAddrInt = INVALID_ID;
    uint8_t phyAddrExt = INVALID_ID;
    uint8_t  phyType;

    if(!CMM_PORT_VALID(unit, port))
    {
        return INVALID_ID;
    }

    phyType = UNITINFO(unit)->pPortDescp[port]->phyType;
    if (phyType == YT_PHY_NONE)
    {
        return INVALID_ID;
    }
    phyAddrInt = CAL_YTP_TO_INTPHYADDR(unit, port);
    phyAddrExt = CAL_YTP_TO_EXTPHYADDR(unit, port);
    if (phyType == YT_PHY_INT)
    {
        return phyAddrInt;
    }
    else if ((phyType == YT_PHY_INTEXT) || (phyType == YT_PHY_EXT))
    {
        return phyAddrExt;
    }
    else
    {
        return (phyAddrInt != INVALID_ID) ? phyAddrInt : phyAddrExt;
    }
}

yt_bool_t cal_is_combo_port(yt_unit_t unit, yt_port_t port)
{
    yt_port_medium_t media;

    if(!CMM_PORT_VALID(unit, port))
    {
        return INVALID_ID;
    }

    media = UNITINFO(unit)->pPortDescp[port]->medium;

    if(PORT_MEDI_COMBO_FIBER == media ||
        PORT_MEDI_COMBO_COPPER == media)
    {
        return TRUE;
    }

    return FALSE;
}

