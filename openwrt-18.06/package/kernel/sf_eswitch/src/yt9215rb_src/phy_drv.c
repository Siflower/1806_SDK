/*******************************************************************************
*                                                                              *
*  Copyright (c), 2022, Motorcomm Electronic Technology Co.,Ltd.               *
*  Motorcomm Confidential and Proprietary.                                     *
*                                                                              *
********************************************************************************
*/

#include "phy_drv.h"
#include "hal_ctrl.h"
#include "phy_int_yt861x.h"
#include "phy_yt8531.h"
#include "phy_int_serdes.h"
#include "phy_yt8618.h"
#include "phy_yt8614.h"
#include "phy_yt8824.h"
#include "phy_yt8821.h"
#include "phy_int_yt882x.h"
#include "yt_phy.h"

const yt_phy_drv_t *gpPhyDrvList[] =
{
#if !defined(SWITCH_SERIES_WHALE)
    [YT_PHY_MODEL_INT861X] = &int_yt861x_drv,
#else
    [YT_PHY_MODEL_INT882X] = &int_yt882x_drv,
#endif
    [YT_PHY_MODEL_INTSERDES] = &int_serdes_drv,
#ifdef PHY_YT8531_INCLUDED
    [YT_PHY_MODEL_8531] = &yt8531_drv,
#else
    [YT_PHY_MODEL_8531] = NULL,
#endif
#ifdef PHY_YT8618_INCLUDED
    [YT_PHY_MODEL_8618] = &yt8618_drv,
#else
    [YT_PHY_MODEL_8618] = NULL,
#endif
#ifdef PHY_YT8614_INCLUDED
    [YT_PHY_MODEL_8614] = &yt8614_drv,
#else
    [YT_PHY_MODEL_8614] = NULL,
#endif
#ifdef PHY_YT8824_INCLUDED
    [YT_PHY_MODEL_8824] = &yt8824_drv,
#else
    [YT_PHY_MODEL_8824] = NULL,
#endif
#ifdef PHY_YT8821_INCLUDED
    [YT_PHY_MODEL_8821] = &yt8821_drv,
#else
    [YT_PHY_MODEL_8821] = NULL,
#endif
};

static yt_phy_smi_bus_t gSwIntSmi[YT_MAX_UNIT];
static yt_phy_smi_bus_t gSwExtSmi[YT_MAX_UNIT];
static yt_phy_smi_bus_t gExternalSmi;
static yt_bool_t gPhyDebugEn = FALSE;

void phy_smi_init(void)
{
    yt_unit_t unit;
    uint8_t phyAddr;

    osal_memset(&gSwIntSmi, sizeof(gSwIntSmi), 0, sizeof(gSwIntSmi));
    osal_memset(&gSwExtSmi, sizeof(gSwExtSmi), 0, sizeof(gSwExtSmi));
    osal_memset(&gExternalSmi, sizeof(gExternalSmi), 0, sizeof(gExternalSmi));

    for (unit = 0; unit < YT_MAX_UNIT; unit++)
    {
        gSwIntSmi[unit].autoLoad = 1;
        gSwIntSmi[unit].phyAddrMask = YT_PHYADDR_ALLMASK;
        gSwExtSmi[unit].autoLoad = 1;
        gSwExtSmi[unit].phyAddrMask = YT_PHYADDR_ALLMASK;
        for (phyAddr = 0; phyAddr < YT_PHY_MAX_ADDR; phyAddr++)
        {
            gSwIntSmi[unit].phyDesc[phyAddr].phyChipModel = INVALID_ID;
            gSwIntSmi[unit].phyDesc[phyAddr].phyBaseAddr = INVALID_ID;
            gSwExtSmi[unit].phyDesc[phyAddr].phyChipModel = INVALID_ID;
            gSwExtSmi[unit].phyDesc[phyAddr].phyBaseAddr = INVALID_ID;
        }
    }

    gExternalSmi.autoLoad = 1;
    gExternalSmi.phyAddrMask = YT_PHYADDR_ALLMASK;
    for (phyAddr = 0; phyAddr < YT_PHY_MAX_ADDR; phyAddr++)
    {
        gExternalSmi.phyDesc[phyAddr].phyChipModel = INVALID_ID;
        gExternalSmi.phyDesc[phyAddr].phyBaseAddr = INVALID_ID;
    }

    OSAL_DEBUG(gPhyDebugEn, ("phy smi init done\n"));

    return;
}

/* ignore autoLoad, load according board configure */
static yt_ret_t phy_smi_swIntPhy_load(yt_unit_t unit, yt_port_t portId)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    uint8_t intPhyAddr;
    yt_port_attri_t attribute; 
    yt_phy_chip_model_t phyModel = INVALID_ID;
    yt_switch_chip_id_t swChipId = INVALID_ID;

    if ((unit >= YT_MAX_UNIT) || !CMM_PORT_VALID(unit, portId))
    {
        return CMM_ERR_INPUT;
    }

    intPhyAddr = CAL_YTP_TO_INTPHYADDR(unit, portId);
    if (intPhyAddr == INVALID_ID)
    {
        return CMM_ERR_FAIL;
    }

    attribute = CAL_PORT_ATTRIBUTE(unit, portId);
    pSmiBus = &gSwIntSmi[unit];
    if (attribute == PORT_ATTR_SERDES)
    {
        phyModel = YT_PHY_MODEL_INTSERDES;
        
    }
    else if ((attribute == PORT_ATTR_PHY))
    {
        swChipId = UNITINFO(unit)->chip_id;
        if (swChipId == YT_SW_ID_922X)
        {
            phyModel = YT_PHY_MODEL_INT882X;
        }
        else if ((swChipId == YT_SW_ID_9215) || (swChipId == YT_SW_ID_9218) || (swChipId == YT_SW_ID_923X))
        {
            phyModel = YT_PHY_MODEL_INT861X;
        }
    }
    OSAL_DEBUG(gPhyDebugEn, ("sw internal phy: unit %u port %u phyAddr %u attribute %u phyModel %u\n",
                            unit, portId, intPhyAddr, attribute, phyModel));
    if (phyModel != INVALID_ID)
    {
        pSmiBus->phyDesc[intPhyAddr].phyChipModel = phyModel;
        pSmiBus->phyDesc[intPhyAddr].phyDrv = gpPhyDrvList[phyModel]->pDrvFunc;
    }
    else
    {
        pSmiBus->phyDesc[intPhyAddr].phyChipModel = INVALID_ID;
        pSmiBus->phyDesc[intPhyAddr].phyDrv = NULL;       
    }

    return CMM_ERR_OK;
}

static yt_ret_t phy_id_get(yt_phy_comm_cfg_t cfg, uint32_t *pId)
{
    uint16_t regVal2 = 0;
    uint16_t regVal3 = 0;
    yt_ret_t ret;

    if ((cfg.unit >= YT_MAX_UNIT) || (cfg.smiType > YT_PHY_SMITYPE_EXTERNAL) || (cfg.phyAddr >= YT_PHY_MAX_ADDR))
    {
        return CMM_ERR_INPUT;       
    }

    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_MII_REG_2, &regVal2), ret);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, PHY_MII_REG_3, &regVal3), ret);

    *pId = (regVal2 << 16) | regVal3;

    return CMM_ERR_OK;
}

static const yt_phy_drv_t *phy_find_drv_by_id(uint32_t id)
{
    uint32_t len = sizeof(gpPhyDrvList) / sizeof(gpPhyDrvList[0]);
    uint32_t i;

    for (i = 0; i < len; i++)
    {
        if ((gpPhyDrvList[i] != NULL) && (gpPhyDrvList[i]->phyChipId == id))
        {
            return gpPhyDrvList[i];
        }
    }

    return NULL;
}

static yt_ret_t phy_smi_swExtPhy_load(yt_unit_t unit, yt_port_t portId)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    yt_phy_comm_cfg_t cfg;
    uint8_t extPhyAddr;
    yt_phy_chip_model_t phyModel = INVALID_ID;
    uint32_t phyId = 0;
    const yt_phy_drv_t *phyDriver = NULL;
    yt_ret_t ret;

    if ((unit >= YT_MAX_UNIT) || !CMM_PORT_VALID(unit, portId))
    {
        return CMM_ERR_INPUT;
    }

    extPhyAddr = CAL_YTP_TO_EXTPHYADDR(unit, portId);
    if (extPhyAddr == INVALID_ID)
    {
        return CMM_ERR_FAIL;
    }
    /* (phyType + phy address) decide whether to hang the driver or not */
    pSmiBus = &gSwExtSmi[unit];
    if (pSmiBus->autoLoad)
    {
        pSmiBus->phyDesc[extPhyAddr].phyChipModel = INVALID_ID;
        pSmiBus->phyDesc[extPhyAddr].phyDrv = NULL;
        /* currently use board configure phyAddr to load */
        cfg.phyAddr = extPhyAddr;
        cfg.unit = unit;
        cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
        ret = phy_id_get(cfg, &phyId);
        OSAL_DEBUG(gPhyDebugEn, ("sw external phy: unit %u port %u phyAddr %u get phyId ret %u phyId 0x%08x\n",
            unit, portId, extPhyAddr, ret, phyId));
        if (ret == CMM_ERR_OK)
        {
            phyDriver = phy_find_drv_by_id(phyId);
            if (phyDriver)
            {
                pSmiBus->phyDesc[extPhyAddr].phyChipModel = phyDriver->phyChipModel;
                pSmiBus->phyDesc[extPhyAddr].phyDrv = phyDriver->pDrvFunc;
                OSAL_DEBUG(gPhyDebugEn, ("sw external phy: update phyDrv port %u phyChipModel %u\n",
                    portId, phyDriver->phyChipModel));
            }
        }
    }
    else
    {
        phyModel = pSmiBus->phyDesc[extPhyAddr].phyChipModel;
        if (phyModel < YT_PHY_MODEL_END)
        {
            pSmiBus->phyDesc[extPhyAddr].phyDrv = gpPhyDrvList[phyModel]->pDrvFunc;
            OSAL_DEBUG(gPhyDebugEn, ("sw external phy manual: unit %u port %u phyModel %u\n",
                unit, portId, phyModel));
        }
        else
        {
            pSmiBus->phyDesc[extPhyAddr].phyDrv = NULL;
        }
    }

    return CMM_ERR_OK;
}

static yt_ret_t phy_smi_external_phy_load(void)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    yt_phy_comm_cfg_t cfg;
    uint8_t extPhyAddr;
    yt_phy_chip_model_t phyModel = INVALID_ID;
    uint32_t phyId = 0;
    const yt_phy_drv_t *phyDriver = NULL;
    yt_ret_t ret;

    pSmiBus = &gExternalSmi;
    for (extPhyAddr = 0; extPhyAddr < YT_PHY_MAX_ADDR; extPhyAddr++)
    {
        /* currently scan all phy address, use phyAddrMask to speed up the scan */
        if (!(pSmiBus->phyAddrMask & (1 << extPhyAddr)))
        {
            pSmiBus->phyDesc[extPhyAddr].phyChipModel = INVALID_ID;
            pSmiBus->phyDesc[extPhyAddr].phyDrv = NULL;
            continue;
        }

        if (pSmiBus->autoLoad)
        {
            pSmiBus->phyDesc[extPhyAddr].phyChipModel = INVALID_ID;
            pSmiBus->phyDesc[extPhyAddr].phyDrv = NULL;   
            cfg.phyAddr = extPhyAddr;
            cfg.unit = 0;
            cfg.smiType = YT_PHY_SMITYPE_EXTERNAL;
            ret = phy_id_get(cfg, &phyId);
            OSAL_DEBUG(gPhyDebugEn, ("external phy: phyAddr %u get phyId ret %u phyId 0x%08x\n",
                extPhyAddr, ret, phyId));
            if (ret == CMM_ERR_OK)
            {
                phyDriver = phy_find_drv_by_id(phyId);
                if (phyDriver)
                {
                    pSmiBus->phyDesc[extPhyAddr].phyChipModel = phyDriver->phyChipModel;
                    pSmiBus->phyDesc[extPhyAddr].phyDrv = phyDriver->pDrvFunc;
                    OSAL_DEBUG(gPhyDebugEn, ("external phy: update phyDrv phyAddr %u phyChipModel %u\n",
                        extPhyAddr, phyDriver->phyChipModel));
                }              
            }
        }
        else
        {
            phyModel = pSmiBus->phyDesc[extPhyAddr].phyChipModel;
            if (phyModel < YT_PHY_MODEL_END)
            {
                pSmiBus->phyDesc[extPhyAddr].phyDrv = gpPhyDrvList[phyModel]->pDrvFunc;
                OSAL_DEBUG(gPhyDebugEn, ("external phy manual: phyAddr %u phyModel %u\n",
                    extPhyAddr, phyModel));
            }
            else
            {
                pSmiBus->phyDesc[extPhyAddr].phyDrv = NULL;
            }
        }
    }

    return CMM_ERR_OK;
}

static yt_ret_t phy_get_max_num_by_model(yt_phy_chip_model_t phyModel, uint8_t *maxNum)
{
    if ((phyModel >= YT_PHY_MODEL_END) || (!maxNum))
    {
        return CMM_ERR_INPUT;
    }

    switch (phyModel)
    {
        case YT_PHY_MODEL_8614:
        case YT_PHY_MODEL_8824:
        case YT_PHY_MODEL_INT882X:
            *maxNum = 4;
            break;
        case YT_PHY_MODEL_8618:
        case YT_PHY_MODEL_INT861X:
            *maxNum = 8;
            break;
        default:
            *maxNum = 1;   
            break;
    }
    
    return CMM_ERR_OK;
}

/* 
The phy address used by the multi-port PHY are to be sequential, 
and the behaviour of this function may be problematic if it is not sequential. 
*/
static yt_ret_t phy_baseAddr_update(yt_unit_t unit, yt_phy_smi_type_t smiType)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    uint8_t phyAddr;
    uint8_t maxPhyAddr = INVALID_ID;
    yt_phy_chip_model_t prePhyModel = INVALID_ID;
    yt_phy_chip_model_t phyModel = INVALID_ID;
    uint8_t phyBaseAddr = 0;
    uint8_t maxPhyNum = 0;
    yt_ret_t ret;

    pSmiBus = phy_smi_bus_get(unit, smiType);
    if (!pSmiBus)
    {
        return CMM_ERR_NULL_POINT;
    }
 
    for (phyAddr = 0; phyAddr < YT_PHY_MAX_ADDR; phyAddr++)
    {
        if (!pSmiBus->phyDesc[phyAddr].phyDrv)
        {
            continue;
        }

        phyModel = pSmiBus->phyDesc[phyAddr].phyChipModel;
        if ((prePhyModel == INVALID_ID) || (prePhyModel != phyModel) || (phyAddr > maxPhyAddr))
        {
            phyBaseAddr = phyAddr;
            CMM_ERR_CHK(phy_get_max_num_by_model(phyModel, &maxPhyNum), ret);
            prePhyModel = phyModel;
            maxPhyAddr = phyAddr + maxPhyNum - 1;
            pSmiBus->phyDesc[phyAddr].phyBaseAddr = phyBaseAddr;
            OSAL_DEBUG(gPhyDebugEn, ("first, unit %u smi type %u phyModel %u phyAddr %u maxPhyNum %u phyBaseAddr %u\n",
                unit, smiType, phyModel, phyAddr, maxPhyNum, phyBaseAddr));
            maxPhyNum--;
            if (!maxPhyNum)
            {
                prePhyModel = INVALID_ID;
            }
            
        }
        else if ((prePhyModel == phyModel) && (maxPhyNum > 0))
        {
            pSmiBus->phyDesc[phyAddr].phyBaseAddr = phyBaseAddr;
            OSAL_DEBUG(gPhyDebugEn, ("continued, unit %u smi type %u phyModel %u phyAddr %u maxPhyNum %u phyBaseAddr %u\n",
                unit, smiType, phyModel, phyAddr, maxPhyNum, phyBaseAddr));
            maxPhyNum--;
            if (!maxPhyNum)
            {
                prePhyModel = INVALID_ID;
            }
        }
    }

    return CMM_ERR_OK;
}

yt_ret_t phy_drv_load(void)
{
    yt_unit_t unit;
    yt_port_t portId;
    uint8_t phyType;

    for(unit = 0; unit < YT_UNIT_NUM; unit++)
    {
        for(portId = 0; portId < CAL_PORT_NUM_ON_UNIT(unit); portId++)
        { 
            phyType = CAL_YTP_TO_PHYTYPE(unit, portId);
            if ((phyType & YT_PHY_INT))
            {
                phy_smi_swIntPhy_load(unit, portId);
            }

            if (phyType & YT_PHY_EXT)
            {
                phy_smi_swExtPhy_load(unit, portId);
            }
        }
        phy_baseAddr_update(unit, YT_PHY_SMITYPE_SW_INT);
        phy_baseAddr_update(unit, YT_PHY_SMITYPE_SW_EXT);
    }

    phy_smi_external_phy_load();
    phy_baseAddr_update(0, YT_PHY_SMITYPE_EXTERNAL);

    return CMM_ERR_OK;
}

yt_phy_smi_bus_t *phy_smi_bus_get(yt_unit_t unit, yt_phy_smi_type_t smiType)
{

    if ((smiType == YT_PHY_SMITYPE_SW_INT) && (unit < YT_MAX_UNIT))
    {
        return &gSwIntSmi[unit];
    }
    else if ((smiType == YT_PHY_SMITYPE_SW_EXT) && (unit < YT_MAX_UNIT))
    {
        return &gSwExtSmi[unit];
    }
    else if (smiType == YT_PHY_SMITYPE_EXTERNAL)
    {
        return &gExternalSmi;
    }
    else
    {
        return NULL;
    }
}

static yt_phy_chip_model_t phy_model_get_by_phyAaddr(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *pSmiBus;

    pSmiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    if (!pSmiBus)
    {
        return INVALID_ID;
    }

    return pSmiBus->phyDesc[cfg.phyAddr].phyChipModel;
}

yt_phy_chip_model_t phy_model_get_by_port(yt_unit_t unit, yt_port_t port)
{
    uint8_t intPhyAddr;
    uint8_t extPhyAddr;
    yt_phy_comm_cfg_t cfg;
    yt_phy_chip_model_t model = INVALID_ID;

    if ((unit >= YT_MAX_UNIT) || !CMM_PORT_VALID(unit, port))
    {
        return INVALID_ID;
    }

    intPhyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    if (intPhyAddr != INVALID_ID)
    {
        cfg.unit = unit;
        cfg.phyAddr = intPhyAddr;
        cfg.smiType = YT_PHY_SMITYPE_SW_INT;
        model = phy_model_get_by_phyAaddr(cfg);
        if ((model == YT_PHY_MODEL_INT861X) || (model == YT_PHY_MODEL_INT882X))
        {
            return model;
        }
    }

    extPhyAddr = CAL_YTP_TO_EXTPHYADDR(unit, port);
    if (extPhyAddr != INVALID_ID)
    {
        cfg.unit = unit;
        cfg.phyAddr = extPhyAddr;
        cfg.smiType = YT_PHY_SMITYPE_SW_EXT;
        model = phy_model_get_by_phyAaddr(cfg);
        if ((model >= YT_PHY_MODEL_8531) || (model <= YT_PHY_MODEL_8824))
        {
            return model;
        } 
    }

    return INVALID_ID;
}

uint8_t phy_baseAddr_get(yt_phy_comm_cfg_t cfg)
{
    yt_phy_smi_bus_t *pSmiBus;

    pSmiBus = phy_smi_bus_get(cfg.unit, cfg.smiType);
    if (!pSmiBus)
    {
        return INVALID_ID;
    }

    return pSmiBus->phyDesc[cfg.phyAddr].phyBaseAddr;
}

yt_bool_t phy_is_serdes(yt_unit_t unit, yt_port_t port)
{
    uint8_t intPhyAddr;
    yt_phy_comm_cfg_t cfg;
    yt_phy_chip_model_t model = INVALID_ID;

    if ((unit >= YT_MAX_UNIT) || !CMM_PORT_VALID(unit, port))
    {
        return FALSE;
    }

    intPhyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    if (intPhyAddr != INVALID_ID)
    {
        cfg.unit = unit;
        cfg.phyAddr = intPhyAddr;
        cfg.smiType = YT_PHY_SMITYPE_SW_INT;
        model = phy_model_get_by_phyAaddr(cfg);
        if (model == YT_PHY_MODEL_INTSERDES)
        {
            return TRUE;
        }
    }

    return FALSE;
}

yt_ret_t phy_comm_param_check(yt_phy_comm_cfg_t cfg)
{
    CMM_PARAM_CHK((cfg.smiType > YT_PHY_SMITYPE_EXTERNAL), CMM_ERR_INPUT);
    CMM_PARAM_CHK((cfg.phyAddr >= YT_PHY_MAX_ADDR), CMM_ERR_NOT_SUPPORT);
    if (cfg.smiType != YT_PHY_SMITYPE_EXTERNAL)
    {
        CMM_PARAM_CHK((YT_UNIT_NUM <= cfg.unit), CMM_ERR_INPUT);
    }

    return CMM_ERR_OK;
}

void phy_external_smi_phyAddrMask_set(uint32_t phyAddrMask)
{
    gExternalSmi.phyAddrMask = phyAddrMask;

    return;
}

yt_ret_t phy_external_smi_cl22_register(smi_cl22_read smiRead, smi_cl22_write smiWrite)
{
    CMM_PARAM_CHK((!smiRead || !smiWrite), CMM_ERR_NULL_POINT);

    gExternalSmi.smiRead = smiRead;
    gExternalSmi.smiWrite = smiWrite;

    return CMM_ERR_OK;
}

yt_ret_t phy_swIntPhy_drv_update(yt_unit_t unit, yt_port_t port, yt_extif_mode_t mode)
{
    yt_phy_chip_model_t phyModel = INVALID_ID;   
    yt_port_attri_t attribute = INVALID_ID; 
    yt_phy_smi_bus_t *pSmiBus = NULL;
    uint8_t intPhyAddr;

    if ((unit >= YT_MAX_UNIT) || !CMM_PORT_VALID(unit, port) || (mode > YT_EXTIF_MODE_USXGMII_10G))
    {
        return CMM_ERR_INPUT;
    }

    intPhyAddr = CAL_YTP_TO_INTPHYADDR(unit, port);
    if (intPhyAddr >= YT_PHY_MAX_ADDR)
    {
        return CMM_ERR_FAIL;
    }

    switch (mode)
    {
        case YT_EXTIF_MODE_SG_MAC:
        case YT_EXTIF_MODE_SG_PHY:
        case YT_EXTIF_MODE_FIB_1000:
        case YT_EXTIF_MODE_FIB_100:
        case YT_EXTIF_MODE_BX2500:
        case YT_EXTIF_MODE_SGFIB_AS:
        case YT_EXTIF_MODE_QSGMII:
        case YT_EXTIF_MODE_10GQXGMII:
        case YT_EXTIF_MODE_USXGMII_10G:
            phyModel = YT_PHY_MODEL_INTSERDES;
            attribute = PORT_ATTR_SERDES;
            break;

        case YT_EXTIF_MODE_MII:
        case YT_EXTIF_MODE_REMII:
        case YT_EXTIF_MODE_RMII_MAC:
        case YT_EXTIF_MODE_RMII_PHY:
        case YT_EXTIF_MODE_RGMII:
        case YT_EXTIF_MODE_XMII_DISABLE:
            attribute = PORT_ATTR_XMII;
            break;
        default:
            break;
    }

    pSmiBus = &gSwIntSmi[unit];
   if (phyModel < YT_PHY_MODEL_END)
   {
        pSmiBus->phyDesc[intPhyAddr].phyChipModel = phyModel;
        pSmiBus->phyDesc[intPhyAddr].phyDrv = gpPhyDrvList[phyModel]->pDrvFunc;    
   }
   else
   {
        pSmiBus->phyDesc[intPhyAddr].phyChipModel = INVALID_ID;
        pSmiBus->phyDesc[intPhyAddr].phyDrv = NULL;        
   }

   if (attribute != INVALID_ID)
   {
        cal_board_port_attribute_set(unit, port, attribute);
   }
   OSAL_DEBUG(gPhyDebugEn, ("sw internal phy update drv: unit %u port %u mode %u phyAddr %u attribute %u phyModel %u\n",
            unit, port, mode, intPhyAddr, attribute, phyModel));

    return CMM_ERR_OK;
}

uint8_t phy_addr_smi_type_get(yt_unit_t unit, yt_port_t port, uint8_t *pPhyAddr, yt_phy_smi_type_t *pSmiType)
{
    uint8_t phyAddrInt = INVALID_ID;
    uint8_t phyAddrExt = INVALID_ID;
    uint8_t  phyType;
    yt_port_attri_t attribute; 

    if(!CMM_PORT_VALID(unit, port))
    {
        return CMM_ERR_INPUT;
    }

    phyType = UNITINFO(unit)->pPortDescp[port]->phyType;
    if (phyType == YT_PHY_NONE)
    {
        return CMM_ERR_FAIL;
    }

    phyAddrInt = CAL_YTP_TO_INTPHYADDR(unit, port);
    phyAddrExt = CAL_YTP_TO_EXTPHYADDR(unit, port);
    if (phyType == YT_PHY_INT)
    {
        attribute = CAL_PORT_ATTRIBUTE(unit, port);
        if ((phyAddrInt != INVALID_ID) && ((attribute == PORT_ATTR_PHY) || (attribute == PORT_ATTR_SERDES)))
        {
            *pPhyAddr = phyAddrInt;
            *pSmiType = YT_PHY_SMITYPE_SW_INT;
            OSAL_DEBUG(gPhyDebugEn, ("get smiType and phyAddr, unit %u port %u phyAddrInt %u smiType %u attribute %u phyType %u\n",
                unit, port, phyAddrInt, *pSmiType, attribute, phyType));

            return CMM_ERR_OK;
        }
    }
    else if ((phyType == YT_PHY_INTEXT) || (phyType == YT_PHY_EXT))
    {
        if (phyAddrExt != INVALID_ID)
        {
            *pPhyAddr = phyAddrExt;
            *pSmiType = YT_PHY_SMITYPE_SW_EXT;
            OSAL_DEBUG(gPhyDebugEn, ("get smiType and phyAddr, unit %u port %u phyAddrExt %u smiType %u phyType %u\n",
                unit, port, phyAddrExt, *pSmiType, phyType));
        
            return CMM_ERR_OK;
        }
    }

    return CMM_ERR_FAIL;
}

yt_ret_t phy_mii_reg_read(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t *pRegValue)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    yt_ret_t ret = CMM_ERR_NOT_SUPPORT;

    if (pRegValue == NULL)
    {
        return CMM_ERR_NULL_POINT;
    }

    if ((cfg.smiType == YT_PHY_SMITYPE_SW_INT) && (cfg.unit < YT_MAX_UNIT))
    {
        pSmiBus = &gSwIntSmi[cfg.unit];
        if (pSmiBus->smiRead != NULL)
        {
            ret = pSmiBus->smiRead(cfg.phyAddr, regAddr, pRegValue);
        }
        else
        {
            if ((HALSWDRV(cfg.unit) != NULL) && (HALSWDRV_FUNC(cfg.unit) != NULL))
            {
                ret = HALSWDRV_FUNC(cfg.unit)->switch_intif_read(cfg.unit, cfg.phyAddr, regAddr, pRegValue);
            }
        }
    }
    else if ((cfg.smiType == YT_PHY_SMITYPE_SW_EXT) && (cfg.unit < YT_MAX_UNIT))
    {
        pSmiBus = &gSwExtSmi[cfg.unit];
        if (pSmiBus->smiRead != NULL)
        {
            ret = pSmiBus->smiRead(cfg.phyAddr, regAddr, pRegValue);
        }
        else
        {
            if ((HALSWDRV(cfg.unit) != NULL) && (HALSWDRV_FUNC(cfg.unit) != NULL))
            {
                ret = HALSWDRV_FUNC(cfg.unit)->switch_extif_read(cfg.unit, cfg.phyAddr, regAddr, pRegValue);
            }
        }
    }
    else if (cfg.smiType == YT_PHY_SMITYPE_EXTERNAL)
    {
        if (gExternalSmi.smiRead != NULL)
        {
            ret = gExternalSmi.smiRead(cfg.phyAddr, regAddr, pRegValue);
        }    
    }

    return ret;
}

yt_ret_t phy_mii_reg_write(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t regValue)
{
    yt_phy_smi_bus_t *pSmiBus = NULL;
    yt_ret_t ret = CMM_ERR_NOT_SUPPORT;

    if ((cfg.smiType == YT_PHY_SMITYPE_SW_INT) && (cfg.unit < YT_MAX_UNIT))
    {
        pSmiBus = &gSwIntSmi[cfg.unit];
        if (pSmiBus->smiWrite != NULL)
        {
            ret = pSmiBus->smiWrite(cfg.phyAddr, regAddr, regValue);
        }
        else
        {
            if ((HALSWDRV(cfg.unit) != NULL) && (HALSWDRV_FUNC(cfg.unit) != NULL))
            {
                ret = HALSWDRV_FUNC(cfg.unit)->switch_intif_write(cfg.unit, cfg.phyAddr, regAddr, regValue);
            }
        }
    }
    else if ((cfg.smiType == YT_PHY_SMITYPE_SW_EXT) && (cfg.unit < YT_MAX_UNIT))
    {
        pSmiBus = &gSwExtSmi[cfg.unit];
        if (pSmiBus->smiWrite != NULL)
        {
            ret = pSmiBus->smiWrite(cfg.phyAddr, regAddr, regValue);
        }
        else
        {
            if ((HALSWDRV(cfg.unit) != NULL) && (HALSWDRV_FUNC(cfg.unit) != NULL))
            {
                ret = HALSWDRV_FUNC(cfg.unit)->switch_extif_write(cfg.unit, cfg.phyAddr, regAddr, regValue);
            }
        }
    }
    else if (cfg.smiType == YT_PHY_SMITYPE_EXTERNAL)
    {
        if (gExternalSmi.smiWrite != NULL)
        {
            ret = gExternalSmi.smiWrite(cfg.phyAddr, regAddr, regValue);
        }
    }

    return ret;
}

yt_ret_t phy_ext_reg_read(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t *pRegValue)
{
    yt_ret_t ret;

    if(pRegValue == NULL)
    {
        return CMM_ERR_NULL_POINT;
    }

    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x1e, regAddr), ret);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0x1f, pRegValue), ret);

    return CMM_ERR_OK;
}

yt_ret_t phy_ext_reg_write(yt_phy_comm_cfg_t cfg, uint16_t regAddr, uint16_t regValue)
{
    yt_ret_t ret;

    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x1e, regAddr), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0x1f, regValue), ret);

    return CMM_ERR_OK;
}


yt_ret_t phy_mmd_reg_read(yt_phy_comm_cfg_t cfg, uint8_t mmdId, uint16_t regAddr, uint16_t *pRegValue)
{
    yt_ret_t ret;

    if(pRegValue == NULL)
    {
        return CMM_ERR_NULL_POINT;
    }

    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xd, mmdId), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xe, regAddr), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xd, 0x4000 + mmdId), ret);
    CMM_ERR_CHK(phy_mii_reg_read(cfg, 0xe, pRegValue), ret);

    return CMM_ERR_OK;
}

yt_ret_t phy_mmd_reg_write(yt_phy_comm_cfg_t cfg, uint8_t mmdId, uint16_t regAddr, uint16_t regValue)
{
    yt_ret_t ret;

    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xd, mmdId), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xe, regAddr), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xd, 0x4000 + mmdId), ret);
    CMM_ERR_CHK(phy_mii_reg_write(cfg, 0xe, regValue), ret);

    return CMM_ERR_OK;
}