/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_cmm.h"
#include "fal_tiger_l2.h"
#include "fal_tiger_lag.h"
#include "yt_l2.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

typedef l2_fdb_tbl_bin0_t l2_fdb_tbl_t;

static uint32_t  fal_tiger_l2_fdb_addr_flush(yt_unit_t unit, yt_l2_tbl_flush_ctrl_t opFlush);
static uint32_t  fal_tiger_l2_fdb_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, 
                                                     yt_l2_fdb_info_t *pFdbInfo, uint16_t *pLookupIndex);
static uint32_t  fal_tiger_l2_fdb_op_add(yt_unit_t unit,  yt_l2_fdb_op_mode_t opMode,
                     uint16_t fid, yt_mac_addr_t macAddr, yt_port_mask_t portMask,
                     yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_add_cfg(yt_unit_t unit, yt_l2_fdb_op_mode_t opMode, 
                                                                 l2_fdb_tbl_t l2Fdb, yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_flush(yt_unit_t unit, yt_l2_tbl_flush_ctrl_t opFlush, yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_get_one(yt_unit_t unit,uint16_t fid, yt_mac_addr_t macAddr, 
                                                                 yt_l2_fdb_info_t *pFdbInfo, yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_get_one_withidx(yt_unit_t unit, uint16_t idx, 
                                                                             yt_mac_addr_t *pMacAddr, uint16_t *pFid, yt_l2_fdb_info_t *pFdbInfo,
                                                                             yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_fdb_op_get_next_withidx(yt_unit_t unit, uint16_t idx, 
                                                                    yt_mac_addr_t *pMacAddr, uint16_t *pFid, uint16_t *pNextIndex, 
                                                                    yt_l2_fdb_info_t *pFdbInfo, yt_l2_fdb_op_result_t *pOpResult);
static uint32_t  fal_tiger_l2_op_info_get(yt_unit_t unit,  yt_mac_addr_t *pMacAddr, uint16_t *pFid, yt_l2_fdb_info_t *pFdbInfo);
static  uint32_t  fal_tiger_l2_op_result_get(yt_unit_t unit,  yt_l2_fdb_op_result_t *pOpResult);
static uint32_t fal_tiger_l2_tbl_read(yt_unit_t unit, uint16_t lookupIndex, l2_fdb_tbl_t *pL2Fdb);
static uint32_t fal_tiger_l2_tbl_write(yt_unit_t unit, uint16_t lookupIndex, l2_fdb_tbl_t *pL2Fdb);

static uint32_t fal_tiger_l2_tbl_write(yt_unit_t unit, uint16_t lookupIndex, l2_fdb_tbl_t *pL2Fdb)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_op_mode_t opMode;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&opMode, sizeof(yt_l2_fdb_op_mode_t), 0, sizeof(yt_l2_fdb_op_mode_t));

    if(NULL == pL2Fdb)
    {
        return CMM_ERR_NULL_POINT;
    }

    opMode.l2FdbOpMode = L2_FDB_OP_MODE_INDEX;
    opMode.entryIdx = lookupIndex;
    
    CMM_ERR_CHK(fal_tiger_l2_fdb_op_add_cfg(unit, opMode, *pL2Fdb, &opResult),ret);
    
    if(opResult.lookupFail)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_l2_tbl_read(yt_unit_t unit, uint16_t lookupIndex, l2_fdb_tbl_t *pL2Fdb)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_info_t fdbInfo;
    yt_mac_addr_t macAddr;
    uint16_t fid = 0;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&macAddr, sizeof(yt_mac_addr_t), 0, sizeof(yt_mac_addr_t));

    if(NULL == pL2Fdb) 
    {
        return CMM_ERR_NULL_POINT;
    }
    
    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_one_withidx(unit, lookupIndex, &macAddr, &fid, &fdbInfo, &opResult),ret);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_0f, pL2Fdb, ((macAddr.addr[0] << 24) | (macAddr.addr[1] << 16) | (macAddr.addr[2] << 8) | macAddr.addr[3]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_1f, pL2Fdb, ((macAddr.addr[4] << 8) | macAddr.addr[5]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_FIDf, pL2Fdb, fid);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_STATUSf, pL2Fdb, fdbInfo.status);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_INT_PRI_ENf, pL2Fdb, fdbInfo.dmacIntPriEn);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_NEW_VIDf, pL2Fdb, fdbInfo.newVid);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_INT_PRIf, pL2Fdb, fdbInfo.intPri);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_INT_PRI_ENf, pL2Fdb, fdbInfo.smacIntPriEn);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, pL2Fdb, fdbInfo.copyToCpu);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, pL2Fdb, fdbInfo.dmacDrop);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DST_PORT_MASKf, pL2Fdb, fdbInfo.dstPortMask);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, pL2Fdb, fdbInfo.smacDrop);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_init(yt_unit_t unit)
{
    uint8_t id = 0;

    fal_tiger_l2_system_learnlimit_cnt_set(unit, CAL_L2_FDB_NUM_MAX(unit));
    for(id = 0; id < CAL_PORT_NUM_ON_UNIT(unit); id++)
    {
        fal_tiger_l2_port_learnlimit_cnt_set(unit, id, CAL_L2_FDB_NUM_MAX(unit));
    }
    for(id = 0; id < FAL_MAX_LAG_NUM; id++)
    {
        fal_tiger_l2_lag_learnlimit_cnt_set(unit, id, CAL_L2_FDB_NUM_MAX(unit));
    }
    fal_tiger_l2_fdb_linkdownFlush_en_set(unit, YT_DISABLE);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_mcast_addr_add(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_port_mask_t portMask)
{
    cmm_err_t ret           = CMM_ERR_OK;
    
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_op_mode_t opMode;
    yt_port_mask_t  macmask;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&opMode, sizeof(yt_l2_fdb_op_mode_t), 0, sizeof(yt_l2_fdb_op_mode_t));
    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

    opMode.l2FdbOpMode = L2_FDB_OP_MODE_HASH;
    opMode.entryIdx = 0;

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_add(unit, opMode, vid, macAddr, macmask, &opResult),ret);
    if ((opResult.opResult == 1U) && (opResult.lookupFail == 1U))
    {
        return CMM_ERR_ENTRY_FULL;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_ucast_addr_add(yt_unit_t unit, const l2_ucastMacAddr_info_t *pUcastMac)
{
    yt_port_mask_t portMask;
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_op_mode_t opMode;
    l2_fdb_tbl_t l2Fdb;
    yt_l2_fdb_info_t fdbInfo;
    uint16_t lookupIndex = 0;
    yt_port_mask_t  macmask;
#ifdef LAG_INCLUDED
    uint8_t  lagGroupId = FAL_MAX_LAG_NUM;
#endif

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&opMode, sizeof(yt_l2_fdb_op_mode_t), 0, sizeof(yt_l2_fdb_op_mode_t));
    osal_memset(&l2Fdb, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));
    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));

    if(pUcastMac->isLag == FALSE)/* phy port */
    {
        CMM_CLEAR_MEMBER_PORT(portMask);
        CMM_SET_MEMBER_PORT(portMask, pUcastMac->port);
#ifdef LAG_INCLUDED
        CMM_ERR_CHK(fal_tiger_lag_phyPort_belong_lagPort_get(unit, pUcastMac->port, &lagGroupId),ret);

        if (lagGroupId != FAL_MAX_LAG_NUM)
        {
            return CMM_ERR_INPUT;
        }
#endif
    }
    else if(pUcastMac->isLag == TRUE) /* check lag port is valid or not */
    {
#ifdef LAG_INCLUDED
        CMM_PARAM_CHK((FAL_MAX_LAG_NUM <= pUcastMac->port), CMM_ERR_INPUT);
        CMM_ERR_CHK(fal_tiger_lag_group_port_get(unit, pUcastMac->port, &portMask), ret);
        if (0 == portMask.portbits[0])
        {
            return CMM_ERR_PORT;
        }
#else
        return CMM_ERR_NOT_SUPPORT;
#endif
    }


    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

    opMode.l2FdbOpMode = L2_FDB_OP_MODE_HASH;
    opMode.entryIdx = 0;

    if (fal_tiger_l2_fdb_get(unit, pUcastMac->vid,  pUcastMac->macaddr,  &fdbInfo, &lookupIndex) == CMM_ERR_OK)
    {
        CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit, lookupIndex, &l2Fdb),ret);
    }

    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_0f, &l2Fdb, ((pUcastMac->macaddr.addr[0] << 24) | (pUcastMac->macaddr.addr[1] << 16) | (pUcastMac->macaddr.addr[2] << 8) | pUcastMac->macaddr.addr[3]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_1f, &l2Fdb, ((pUcastMac->macaddr.addr[4] << 8) | pUcastMac->macaddr.addr[5]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_FIDf, &l2Fdb, pUcastMac->vid);
    if (pUcastMac->type == YT_L2_FDB_TYPE_STATIC)
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_STATUSf, &l2Fdb, FDB_STATUS_STATIC);
    }
    else if (pUcastMac->type == YT_L2_FDB_TYPE_DYNAMIC)
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_STATUSf, &l2Fdb, FDB_STATUS_MAX_TIME);
    }
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DST_PORT_MASKf, &l2Fdb, macmask.portbits[0]);
    if (pUcastMac->action == YT_L2_FDB_ACTION_TYPE_DA_DROP)
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2Fdb, YT_ENABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2Fdb, YT_DISABLE);
    }
    else if (pUcastMac->action == YT_L2_FDB_ACTION_TYPE_SA_DROP)
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2Fdb, YT_ENABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2Fdb, YT_DISABLE);
    }
    else if (pUcastMac->action == YT_L2_FDB_ACTION_TYPE_COPY_TO_CPU)
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2Fdb, YT_ENABLE);
    }
    else
    {
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2Fdb, YT_DISABLE);
        HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2Fdb, YT_DISABLE);
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_add_cfg(unit, opMode, l2Fdb, &opResult), ret);

    if ((opResult.opResult == 1U) && (opResult.lookupFail == 1U))
    {
        if (pUcastMac->forceFlag && (opResult.overwrite == 1U))
        {
            opMode.l2FdbOpMode = L2_FDB_OP_MODE_INDEX;
            opMode.entryIdx = opResult.entryIdx;

            CMM_ERR_CHK(fal_tiger_l2_fdb_op_add_cfg(unit, opMode, l2Fdb, &opResult), ret);

            if (opResult.opResult == 1U)
            {
                return CMM_ERR_FAIL;
            }
        }
        else
        {
            return CMM_ERR_ENTRY_FULL;
        }
    }

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_del(unit,  vid,  macAddr, &opResult), ret);
    if(opResult.lookupFail)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_ucast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr)
{
    return fal_tiger_l2_fdb_addr_del(unit, vid, macAddr);
}

yt_ret_t  fal_tiger_l2_fdb_mcast_addr_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr)
{
    return fal_tiger_l2_fdb_addr_del(unit, vid, macAddr);
}

yt_ret_t  fal_tiger_l2_fdb_linkdownFlush_en_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_hw_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(l2_fdb_hw_flush_ctrl_t), 0, sizeof(l2_fdb_hw_flush_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_HW_FLUSH_CTRLm, 0, sizeof(l2_fdb_hw_flush_ctrl_t), &flushOp), ret);
    HAL_FIELD_SET(L2_FDB_HW_FLUSH_CTRLm, L2_FDB_HW_FLUSH_CTRL_GLOBAL_ENf, &flushOp, enable);
    HAL_FIELD_SET(L2_FDB_HW_FLUSH_CTRLm, L2_FDB_HW_FLUSH_CTRL_LAG_CTRL_ENf, &flushOp, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_HW_FLUSH_CTRLm, 0, sizeof(l2_fdb_hw_flush_ctrl_t), &flushOp), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_linkdownFlush_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_hw_flush_ctrl_t flushOp;
    uint32 enable = 0;

    osal_memset(&flushOp, sizeof(l2_fdb_hw_flush_ctrl_t), 0, sizeof(l2_fdb_hw_flush_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_HW_FLUSH_CTRLm, 0, sizeof(l2_fdb_hw_flush_ctrl_t), &flushOp), ret);

    HAL_FIELD_GET(L2_FDB_HW_FLUSH_CTRLm, L2_FDB_HW_FLUSH_CTRL_GLOBAL_ENf, &flushOp, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_fdb_all_ucast_flush(yt_unit_t unit)
{
    yt_l2_tbl_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));
    
    flushOp.mode = L2_FDB_FLUSH_MODE_UFDB_ALL;

    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t fal_tiger_l2_fdb_port_ucast_flush(yt_unit_t unit, yt_port_t port, yt_bool_t isLag)
{
    yt_l2_tbl_flush_ctrl_t flushOp;
#ifdef LAG_INCLUDED
    cmm_err_t ret           = CMM_ERR_OK;
#endif

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));

    if(isLag == FALSE)/* phy port */
    {
        CMM_CLEAR_MEMBER_PORT(flushOp.portMask);
        CMM_SET_MEMBER_PORT(flushOp.portMask, CAL_YTP_TO_MAC(unit,port));
    }
    else if(isLag == TRUE) /* check lag port is valid or not */
    {
#ifdef LAG_INCLUDED
        CMM_PARAM_CHK((FAL_MAX_LAG_NUM <= port), CMM_ERR_INPUT);
        CMM_ERR_CHK(fal_tiger_lag_group_port_get(unit, port, &flushOp.portMask), ret);
        if (0 == flushOp.portMask.portbits[0])
        {
            return CMM_ERR_PORT;
        }
#else
        return CMM_ERR_NOT_SUPPORT;
#endif
    }


    flushOp.mode = L2_FDB_FLUSH_MODE_UFDB_PORT;

    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t fal_tiger_l2_fdb_vlan_ucast_flush(yt_unit_t unit, yt_vlan_t vid)
{
    yt_l2_tbl_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));

    flushOp.fid = vid;

    flushOp.mode = L2_FDB_FLUSH_MODE_UFDB_FID;
        
    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t fal_tiger_l2_fdb_all_mcast_flush(yt_unit_t unit)
{
    yt_l2_tbl_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));
    
    flushOp.mode = L2_FDB_FLUSH_MODE_MFDB_ALL;
    flushOp.flushStaticEn = TRUE;
        
    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t fal_tiger_l2_fdb_port_mcast_flush(yt_unit_t unit, yt_port_t port)
{
    yt_l2_tbl_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));

    CMM_CLEAR_MEMBER_PORT(flushOp.portMask);
    CMM_SET_MEMBER_PORT(flushOp.portMask, CAL_YTP_TO_MAC(unit,port));

    flushOp.mode = L2_FDB_FLUSH_MODE_MFDB_PORT;
    flushOp.flushStaticEn = TRUE;

    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t fal_tiger_l2_fdb_vlan_mcast_flush(yt_unit_t unit, yt_vlan_t vid)
{
    yt_l2_tbl_flush_ctrl_t flushOp;

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));

    flushOp.fid = vid;

    flushOp.mode = L2_FDB_FLUSH_MODE_MFDB_FID;
    flushOp.flushStaticEn = TRUE;
        
    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

static uint32_t  fal_tiger_l2_fdb_addr_flush(yt_unit_t unit, yt_l2_tbl_flush_ctrl_t opFlush)
{
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_enable_t enable = 0;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));

    CMM_ERR_CHK(fal_tiger_l2_fdb_linkdownFlush_en_get(unit, &enable), ret);
    if (enable == YT_ENABLE)
    {
        return CMM_ERR_FORBIDDEN;
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_flush(unit, opFlush, &opResult), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, 
                                            yt_l2_fdb_info_t *pFdbInfo, uint16_t *pLookupIndex)
{
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));

    CMM_PARAM_CHK((NULL == pFdbInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pLookupIndex), CMM_ERR_NULL_POINT);
        
    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_one(unit, vid, macAddr, pFdbInfo, &opResult), ret);
    
    if(opResult.lookupFail)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }
    
    *pLookupIndex = opResult.entryIdx;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_type_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_l2_fdb_type_t *ptype)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));

    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr, &fdbInfo, &lookupIndex), ret);

    *ptype = (fdbInfo.status == FDB_STATUS_STATIC) ? YT_L2_FDB_TYPE_STATIC : YT_L2_FDB_TYPE_DYNAMIC;

    if (fdbInfo.status == FDB_STATUS_PENDING)
    {
        *ptype = YT_L2_FDB_TYPE_PENDING;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_ENf, &l2LearnPerPortCtrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;
    uint32_t enable = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_ENf, &l2LearnPerPortCtrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_cnt_set(yt_unit_t unit, yt_port_t port, uint32_t maxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    if(maxCnt > CAL_L2_FDB_NUM_MAX(unit))
        return CMM_ERR_INPUT;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_NUMf, &l2LearnPerPortCtrl, maxCnt);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_cnt_get(yt_unit_t unit, yt_port_t port, uint32_t *pMaxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;
    uint32_t maxCnt = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_NUMf, &l2LearnPerPortCtrl, &maxCnt);
    *pMaxCnt = maxCnt;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    if ((ACT_TYPE_FWD != actType) && (ACT_TYPE_DROP != actType))
    {
            return CMM_ERR_INPUT;
    }

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_DROPf, &l2LearnPerPortCtrl, actType);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_port_learnlimit_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    yt_macid_t  macid = 0;
    uint32_t enable = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_LIMIT_DROPf, &l2LearnPerPortCtrl, &enable);
    *pActType = enable;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_en_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_ENf, &l2LearnGlobalCtrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_en_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;
    uint32_t enable = 0;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_ENf, &l2LearnGlobalCtrl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_cnt_set(yt_unit_t unit, uint32_t maxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    if(maxCnt > CAL_L2_FDB_NUM_MAX(unit))
        return CMM_ERR_INPUT;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_NUMf, &l2LearnGlobalCtrl, maxCnt);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_cnt_get(yt_unit_t unit, uint32_t *pMaxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;
    uint32_t maxCnt = 0;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_NUMf, &l2LearnGlobalCtrl, &maxCnt);
    *pMaxCnt = maxCnt;
   
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_act_set(yt_unit_t unit, yt_act_type_t actType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    if ((ACT_TYPE_FWD != actType) && (ACT_TYPE_DROP != actType))
    {
            return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_SET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_DROPf, &l2LearnGlobalCtrl, actType);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_system_learnlimit_act_get(yt_unit_t unit, yt_act_type_t *pActType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_learn_global_ctrl_t l2LearnGlobalCtrl;
    uint32_t enable = 0;

    osal_memset(&l2LearnGlobalCtrl, sizeof(l2_learn_global_ctrl_t), 0, sizeof(l2_learn_global_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_GLOBAL_CTRLm, 0, sizeof(l2_learn_global_ctrl_t), &l2LearnGlobalCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_GLOBAL_CTRLm, L2_LEARN_GLOBAL_CTRL_LEARN_LIMIT_DROPf, &l2LearnGlobalCtrl, &enable);
    *pActType = enable;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_drop_sa_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_enable_t enable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2FdbTbl, enable);
    CMM_ERR_CHK(fal_tiger_l2_tbl_write(unit,lookupIndex, &l2FdbTbl),ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_drop_sa_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr,  yt_enable_t *pEnable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;
    uint32_t enable = 0;

    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2FdbTbl, &enable);
    *pEnable        = enable ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_drop_da_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_enable_t enable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2FdbTbl, enable);
    CMM_ERR_CHK(fal_tiger_l2_tbl_write(unit,lookupIndex, &l2FdbTbl),ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_drop_da_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr,  yt_enable_t *pEnable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;
    uint32_t enable = 0;

    CMM_PARAM_CHK((NULL == pEnable), CMM_ERR_NULL_POINT);

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2FdbTbl, &enable);
    *pEnable        = enable ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_copy2cpu_set(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_enable_t enable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2FdbTbl, enable);
    CMM_ERR_CHK(fal_tiger_l2_tbl_write(unit,lookupIndex, &l2FdbTbl),ret);
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_copy2cpu_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_enable_t *pEnable)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    l2_fdb_tbl_t l2FdbTbl;
    uint32_t enable = 0;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&l2FdbTbl, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));

#ifdef SDK_DATABASE_HW_EN
    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr,  &fdbInfo, &lookupIndex), ret);
    CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit,lookupIndex, &l2FdbTbl),ret);
#endif

    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2FdbTbl, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_mcast_set(yt_unit_t unit, yt_port_mask_t portMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_mcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

#ifndef INTER_MCU
    macmask.portbits[0] |= (1<<FAL_INTERNAL_CPU_MACID);
#endif
    HAL_FIELD_SET(L2_MCAST_FILTER_MASKm, L2_MCAST_FILTER_MASK_MCAST_FILTER_MASKf, &filterMask, macmask.portbits[0]);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_MCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_mcast_get(yt_unit_t unit, yt_port_mask_t *pPortMask)
{
    cmm_err_t ret       = CMM_ERR_OK;
    l2_mcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;
    uint32_t portMask = 0;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CMM_CLEAR_MEMBER_PORT(macmask);

    CMM_PARAM_CHK((NULL == pPortMask), CMM_ERR_NULL_POINT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    HAL_FIELD_GET(L2_MCAST_FILTER_MASKm, L2_MCAST_FILTER_MASK_MCAST_FILTER_MASKf, &filterMask, &portMask);
    macmask.portbits[0] = portMask;

    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pPortMask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_bcast_set(yt_unit_t unit, yt_port_mask_t portMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_bcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

#ifndef INTER_MCU
    macmask.portbits[0] |= (1<<FAL_INTERNAL_CPU_MACID);
#endif
    HAL_FIELD_SET(L2_BCAST_FILTER_MASKm, L2_BCAST_FILTER_MASK_BCAST_FILTER_MASKf, &filterMask, macmask.portbits[0]);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_BCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_bcast_get(yt_unit_t unit, yt_port_mask_t *pPortMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_bcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;
    uint32_t portMask = 0;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CMM_CLEAR_MEMBER_PORT(macmask);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_BCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    HAL_FIELD_GET(L2_BCAST_FILTER_MASKm, L2_BCAST_FILTER_MASK_BCAST_FILTER_MASKf, &filterMask, &portMask);
    macmask.portbits[0] = portMask;
    
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pPortMask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_unknown_ucast_set(yt_unit_t unit, yt_port_mask_t portMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_unknown_ucast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

#ifndef INTER_MCU
    macmask.portbits[0] |= (1<<FAL_INTERNAL_CPU_MACID);
#endif
    HAL_FIELD_SET(L2_UNKNOWN_UCAST_FILTER_MASKm, L2_UNKNOWN_UCAST_FILTER_MASK_UNKNOWN_UCAST_FILTER_MASKf, &filterMask, macmask.portbits[0]);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_UNKNOWN_UCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_unknown_ucast_get(yt_unit_t unit, yt_port_mask_t *pPortMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_unknown_ucast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;
    uint32_t portMask = 0;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CMM_CLEAR_MEMBER_PORT(macmask);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_UNKNOWN_UCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    HAL_FIELD_GET(L2_UNKNOWN_UCAST_FILTER_MASKm, L2_UNKNOWN_UCAST_FILTER_MASK_UNKNOWN_UCAST_FILTER_MASKf, &filterMask, &portMask);
    macmask.portbits[0] = portMask;
    
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pPortMask));

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_unknown_mcast_set(yt_unit_t unit, yt_port_mask_t portMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_unknown_mcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;

    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CAL_YTPLIST_TO_MLIST(unit,portMask, macmask);

#ifndef INTER_MCU
    macmask.portbits[0] |= (1<<FAL_INTERNAL_CPU_MACID);
#endif
    HAL_FIELD_SET(L2_UNKNOWN_MCAST_FILTER_MASKm, L2_UNKNOWN_MCAST_FILTER_MASK_UNKNOWN_MCAST_FILTER_MASKf, &filterMask, macmask.portbits[0]);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_UNKNOWN_MCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_filter_unknown_mcast_get(yt_unit_t unit, yt_port_mask_t *pPortMask)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_unknown_mcast_filter_mask_t filterMask;
    yt_port_mask_t  macmask;
    uint32_t portMask = 0;
    
    osal_memset(&filterMask, sizeof(l2_mcast_filter_mask_t), 0, sizeof(l2_mcast_filter_mask_t));
    CMM_CLEAR_MEMBER_PORT(macmask);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_UNKNOWN_MCAST_FILTER_MASKm, 0, sizeof(filterMask), &filterMask), ret);

    HAL_FIELD_GET(L2_UNKNOWN_MCAST_FILTER_MASKm, L2_UNKNOWN_MCAST_FILTER_MASK_UNKNOWN_MCAST_FILTER_MASKf, &filterMask, &portMask);
    macmask.portbits[0] = portMask;
    
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pPortMask));

    return CMM_ERR_OK;
}

uint32_t  fal_tiger_l2_igmp_bypass_unknown_mcast_filter_set(yt_unit_t unit, yt_enable_t enable)
{
    cmm_err_t ret       = CMM_ERR_OK;
    l2_mc_unknown_act_ctrl_t mcAct;

    osal_memset(&mcAct, sizeof(l2_mc_unknown_act_ctrl_t), 0, sizeof(l2_mc_unknown_act_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &mcAct), ret);

    HAL_FIELD_SET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_DROP_IGMPBYPASSf, &mcAct, enable);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &mcAct), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_port_uc_cnt_get(yt_unit_t unit, yt_port_t port, uint32 *pCnt)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_port_learn_mac_cntn_t l2PortLearnMacCntn;
    yt_macid_t macid = 0;
    uint32_t macCnt = 0;

    osal_memset(&l2PortLearnMacCntn, sizeof(l2_port_learn_mac_cntn_t), 0, sizeof(l2_port_learn_mac_cntn_t));

    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);

    macid = CAL_YTP_TO_MAC(unit,port);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_LEARN_MAC_CNTNm, macid, sizeof(l2PortLearnMacCntn), &l2PortLearnMacCntn), ret);
    HAL_FIELD_GET(L2_PORT_LEARN_MAC_CNTNm, L2_PORT_LEARN_MAC_CNTN_MAC_CNTf, &l2PortLearnMacCntn, &macCnt);
    *pCnt = macCnt;

    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_fdb_lag_uc_cnt_get(yt_unit_t unit, uint8_t groupId, uint32_t *pCnt)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_port_learn_mac_cntn_t l2PortLearnMacCntn;
    uint32_t macCnt = 0;

    osal_memset(&l2PortLearnMacCntn, sizeof(l2_port_learn_mac_cntn_t), 0, sizeof(l2_port_learn_mac_cntn_t));

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_PORT_LEARN_MAC_CNTNm, FAL_MAX_PORT_NUM + groupId, sizeof(l2PortLearnMacCntn), &l2PortLearnMacCntn), ret);
    HAL_FIELD_GET(L2_PORT_LEARN_MAC_CNTNm, L2_PORT_LEARN_MAC_CNTN_MAC_CNTf, &l2PortLearnMacCntn, &macCnt);
    *pCnt = macCnt;

    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_en_set(yt_unit_t unit, uint8_t groupId,  yt_enable_t enable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    HAL_FIELD_SET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_ENf, &l2LagLearnLimitCtrl, enable);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_en_get(yt_unit_t unit, uint8_t groupId,  yt_enable_t *pEnable)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;
    uint32_t enable = 0;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);
    HAL_FIELD_GET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_ENf, &l2LagLearnLimitCtrl, &enable);
    *pEnable = enable;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_cnt_set(yt_unit_t unit, uint8_t groupId,  uint32_t maxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    if(maxCnt > CAL_L2_FDB_NUM_MAX(unit))
        return CMM_ERR_INPUT;

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    HAL_FIELD_SET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_NUMf, &l2LagLearnLimitCtrl, maxCnt);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_cnt_get(yt_unit_t unit, uint8_t groupId,  uint32_t *pMaxCnt)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;
    uint32_t maxCnt = 0;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    HAL_FIELD_GET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_NUMf, &l2LagLearnLimitCtrl, &maxCnt);
    *pMaxCnt = maxCnt;
    

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_act_set(yt_unit_t unit, uint8_t groupId, yt_act_type_t actType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    if ((ACT_TYPE_FWD != actType) && (ACT_TYPE_DROP != actType))
    {
            return CMM_ERR_INPUT;
    }

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    HAL_FIELD_SET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_DROPf, &l2LagLearnLimitCtrl, actType);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_lag_learnlimit_act_get(yt_unit_t unit, uint8_t groupId, yt_act_type_t *pActType)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_lag_learn_limit_ctrln_t l2LagLearnLimitCtrl;
    uint32_t enable = 0;

    osal_memset(&l2LagLearnLimitCtrl, sizeof(l2_lag_learn_limit_ctrln_t), 0, sizeof(l2_lag_learn_limit_ctrln_t));

    CMM_PARAM_CHK((FAL_MAX_LAG_NUM - 1< groupId), CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LAG_LEARN_LIMIT_CTRLNm, groupId, sizeof(l2LagLearnLimitCtrl), &l2LagLearnLimitCtrl), ret);

    HAL_FIELD_GET(L2_LAG_LEARN_LIMIT_CTRLNm, L2_LAG_LEARN_LIMIT_CTRLN_LEARN_LIMIT_DROPf, &l2LagLearnLimitCtrl, &enable);
    *pActType = enable;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_uc_cnt_get(yt_unit_t unit,  uint32 *pCnt)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_learn_mac_cnt_t l2LearnMacCntn;
    uint32_t macCnt = 0;

    osal_memset(&l2LearnMacCntn, sizeof(l2_learn_mac_cnt_t), 0, sizeof(l2_learn_mac_cnt_t));

    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_MAC_CNTm, 0, sizeof(l2LearnMacCntn), &l2LearnMacCntn), ret);
    HAL_FIELD_GET(L2_LEARN_MAC_CNTm, L2_LEARN_MAC_CNT_MAC_CNTf, &l2LearnMacCntn, &macCnt);
    *pCnt = macCnt;

    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_fdb_mc_cnt_get(yt_unit_t unit,  uint32 *pCnt)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_igmp_learn_group_cnt_t l2IgmpLearnGroupCnt;
    uint32_t groupCnt = 0;

    osal_memset(&l2IgmpLearnGroupCnt, sizeof(l2_igmp_learn_group_cnt_t), 0, sizeof(l2_igmp_learn_group_cnt_t));

    CMM_PARAM_CHK((NULL == pCnt), CMM_ERR_NULL_POINT);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_IGMP_LEARN_GROUP_CNTm, 0, sizeof(l2IgmpLearnGroupCnt), &l2IgmpLearnGroupCnt), ret);
    HAL_FIELD_GET(L2_IGMP_LEARN_GROUP_CNTm, L2_IGMP_LEARN_GROUP_CNT_CNTf, &l2IgmpLearnGroupCnt, &groupCnt);
    *pCnt = groupCnt;


    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_fdb_aging_port_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_aging_per_port_ctrl_t l2AgingPerPortCtrl;
    yt_macid_t macid = 0;
    uint32_t portmask = 0;

    osal_memset(&l2AgingPerPortCtrl, sizeof(l2_aging_per_port_ctrl_t), 0, sizeof(l2_aging_per_port_ctrl_t));

    macid = CAL_YTP_TO_MAC(unit, port);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_AGING_PER_PORT_CTRLm, 0, sizeof(l2AgingPerPortCtrl), &l2AgingPerPortCtrl), ret);
    HAL_FIELD_GET(L2_AGING_PER_PORT_CTRLm, L2_AGING_PER_PORT_CTRL_AGING_ENf, &l2AgingPerPortCtrl, &portmask);
    if (enable == YT_ENABLE)
    {
        SET_BIT(portmask, macid);
    }
    else if (enable == YT_DISABLE)
    {
        CLEAR_BIT(portmask, macid);
    }
    else
    {
        return CMM_ERR_INPUT;
    }
    HAL_FIELD_SET(L2_AGING_PER_PORT_CTRLm, L2_AGING_PER_PORT_CTRL_AGING_ENf, &l2AgingPerPortCtrl, portmask);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_AGING_PER_PORT_CTRLm, 0, sizeof(l2AgingPerPortCtrl), &l2AgingPerPortCtrl), ret);

    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_fdb_aging_port_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_aging_per_port_ctrl_t l2AgingPerPortCtrl;
    yt_macid_t macid = 0;
    uint32_t portMask = 0;

    osal_memset(&l2AgingPerPortCtrl, sizeof(l2_aging_per_port_ctrl_t), 0, sizeof(l2_aging_per_port_ctrl_t));

    macid = CAL_YTP_TO_MAC(unit,port);
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_AGING_PER_PORT_CTRLm, 0, sizeof(l2AgingPerPortCtrl), &l2AgingPerPortCtrl), ret);
    HAL_FIELD_GET(L2_AGING_PER_PORT_CTRLm, L2_AGING_PER_PORT_CTRL_AGING_ENf, &l2AgingPerPortCtrl, &portMask);
    *pEnable = IS_BIT_SET(portMask, macid) ? YT_ENABLE : YT_DISABLE;
    
    return CMM_ERR_OK; 
}


yt_ret_t  fal_tiger_l2_fdb_aging_time_set(yt_unit_t unit,  uint32_t sec)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_aging_ctrl_t l2AgingCtrl;
    uint32_t ageTime = 0;

    osal_memset(&l2AgingCtrl, sizeof(l2_aging_ctrl_t), 0, sizeof(l2_aging_ctrl_t));

    if(sec > (FDB_STATUS_MAX_TIME*65535))
    {
        return CMM_ERR_INPUT;
    }

    if(FDB_STATUS_MAX_TIME > sec)
    {
        ageTime = 1;
    }
    else
    {
        ageTime  = (sec / FDB_STATUS_MAX_TIME);
    }
    HAL_FIELD_SET(L2_AGING_CTRLm, L2_AGING_CTRL_AGING_INTERVALf, &l2AgingCtrl, ageTime);
       
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_AGING_CTRLm, 0, sizeof(l2AgingCtrl), &l2AgingCtrl), ret);

    return CMM_ERR_OK; 
}

yt_ret_t  fal_tiger_l2_fdb_aging_time_get(yt_unit_t unit,  uint32_t *psec)
{
    cmm_err_t ret                   = CMM_ERR_OK;
    l2_aging_ctrl_t l2AgingCtrl;
    uint32_t ageTime = 0;

    osal_memset(&l2AgingCtrl, sizeof(l2_aging_ctrl_t), 0, sizeof(l2_aging_ctrl_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_AGING_CTRLm, 0, sizeof(l2AgingCtrl), &l2AgingCtrl), ret);
    HAL_FIELD_GET(L2_AGING_CTRLm, L2_AGING_CTRL_AGING_INTERVALf, &l2AgingCtrl, &ageTime);
    *psec = ageTime * FDB_STATUS_MAX_TIME;

    return CMM_ERR_OK; 
}

static  uint32_t  fal_tiger_l2_op_result_get(yt_unit_t unit,  yt_l2_fdb_op_result_t *pOpResult)
{
    l2_fdb_tbl_op_result_t l2FdbTblOpResult;
    uint32_t opNode = 0;
    uint16_t l2FdbTblOpBusyCnt = FDB_BUSY_CHECK_NUMBER;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index = 0;
    uint32_t opResult = 0;
    uint32_t overwrite = 0;
    uint32_t lookupFail = 0;

    osal_memset(&l2FdbTblOpResult, sizeof(l2_fdb_tbl_op_result_t), 0, sizeof(l2_fdb_tbl_op_result_t));

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);

    while (l2FdbTblOpBusyCnt)
    {
        osal_memset(&l2FdbTblOpResult, sizeof(l2_fdb_tbl_op_result_t), 0, sizeof(l2_fdb_tbl_op_result_t));
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_TBL_OP_RESULTm, 0, sizeof(l2FdbTblOpResult), &l2FdbTblOpResult), ret);
        HAL_FIELD_GET(L2_FDB_TBL_OP_RESULTm, L2_FDB_TBL_OP_RESULT_OP_DONEf, &l2FdbTblOpResult, &opNode);
        if(TRUE == opNode)
        {
            HAL_FIELD_GET(L2_FDB_TBL_OP_RESULTm, L2_FDB_TBL_OP_RESULT_ENTRY_INDEXf, &l2FdbTblOpResult, &index);
            HAL_FIELD_GET(L2_FDB_TBL_OP_RESULTm, L2_FDB_TBL_OP_RESULT_OP_RESULTf, &l2FdbTblOpResult, &opResult);
            HAL_FIELD_GET(L2_FDB_TBL_OP_RESULTm, L2_FDB_TBL_OP_RESULT_OVERWRITEf, &l2FdbTblOpResult, &overwrite);
            HAL_FIELD_GET(L2_FDB_TBL_OP_RESULTm, L2_FDB_TBL_OP_RESULT_LOOKUP_FAILf, &l2FdbTblOpResult, &lookupFail);
            
            pOpResult->entryIdx = index;
            pOpResult->opResult   = opResult;
            pOpResult->overwrite   = overwrite;
            pOpResult->lookupFail = lookupFail;
            pOpResult->opDone     = TRUE;
            break;
        }
        
        l2FdbTblOpBusyCnt--;
        
        if(0 == l2FdbTblOpBusyCnt)
        {
            pOpResult->lookupFail = TRUE;
            pOpResult->opDone     = FALSE;
                
            return CMM_ERR_FDB_OP_BUSY;
        }
        
    }
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_op_info_get(yt_unit_t unit,  yt_mac_addr_t *pMacAddr, uint16_t *pFid, yt_l2_fdb_info_t *pFdbInfo)
{
  
    l2_fdb_tbl_op_data_0_t l2FdbTblOpData0;
    l2_fdb_tbl_op_data_1_t l2FdbTblOpData1;
    l2_fdb_tbl_op_data_2_t l2FdbTblOpData2;
    cmm_err_t ret           = CMM_ERR_OK;
    uint32_t macAddr0 = 0;
    uint32_t macAddr1 = 0;
    uint32_t status = 0;
    uint32_t dmacIntPriEn = 0;
    uint32_t fid = 0;
    uint32_t newVid = 0;
    uint32_t intPri = 0;
    uint32_t smacIntPriEn = 0;
    uint32_t copyToCpu = 0;
    uint32_t dmacDrop = 0;
    uint32_t dstPortMask = 0;
    uint32_t smacDrop = 0;
    
    CMM_PARAM_CHK((NULL == pMacAddr), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFid), CMM_ERR_NULL_POINT);
    
    osal_memset(&l2FdbTblOpData0, sizeof(l2_fdb_tbl_op_data_0_t), 0, sizeof(l2_fdb_tbl_op_data_0_t));
    osal_memset(&l2FdbTblOpData1, sizeof(l2_fdb_tbl_op_data_1_t), 0, sizeof(l2_fdb_tbl_op_data_1_t));
    osal_memset(&l2FdbTblOpData2, sizeof(l2_fdb_tbl_op_data_2_t), 0, sizeof(l2_fdb_tbl_op_data_2_t));

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_TBL_OP_DATA_0_DUMMYm, 0, sizeof(l2FdbTblOpData0), &l2FdbTblOpData0), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_TBL_OP_DATA_1_DUMMYm, 0, sizeof(l2FdbTblOpData1), &l2FdbTblOpData1), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_FDB_TBL_OP_DATA_2_DUMMYm, 0, sizeof(l2FdbTblOpData2), &l2FdbTblOpData2), ret);
    
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_1_DUMMYm, L2_FDB_TBL_OP_DATA_1_DUMMY_STATUSf, &l2FdbTblOpData1, &status);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_1_DUMMYm, L2_FDB_TBL_OP_DATA_1_DUMMY_DMAC_INT_PRI_ENf, &l2FdbTblOpData1, &dmacIntPriEn);
    pFdbInfo->status           = status;
    pFdbInfo->dmacIntPriEn  = dmacIntPriEn;

    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_NEW_VIDf, &l2FdbTblOpData2, &newVid);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_INT_PRIf, &l2FdbTblOpData2, &intPri);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_SMAC_INT_PRI_ENf, &l2FdbTblOpData2, &smacIntPriEn);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_COPY_TO_CPUf, &l2FdbTblOpData2, &copyToCpu);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_DMAC_DROPf, &l2FdbTblOpData2, &dmacDrop);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_DST_PORT_MASKf, &l2FdbTblOpData2, &dstPortMask);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_2_DUMMYm, L2_FDB_TBL_OP_DATA_2_DUMMY_SMAC_DROPf, &l2FdbTblOpData2, &smacDrop);
    pFdbInfo->newVid         = newVid;
    pFdbInfo->intPri          = intPri;
    pFdbInfo->smacIntPriEn  = smacIntPriEn;
    pFdbInfo->copyToCpu      = copyToCpu;
    pFdbInfo->dmacDrop        = dmacDrop;
    pFdbInfo->dstPortMask    = dstPortMask;
    pFdbInfo->smacDrop        = smacDrop;

    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_1_DUMMYm, L2_FDB_TBL_OP_DATA_1_DUMMY_FIDf, &l2FdbTblOpData1, &fid);
    *pFid = fid;

    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_0_DUMMYm, L2_FDB_TBL_OP_DATA_0_DUMMY_MAC_DA_0f, &l2FdbTblOpData0, &macAddr0);
    HAL_FIELD_GET(L2_FDB_TBL_OP_DATA_1_DUMMYm, L2_FDB_TBL_OP_DATA_1_DUMMY_MAC_DA_1f, &l2FdbTblOpData1, &macAddr1);
    pMacAddr->addr[0] = ((macAddr0 & 0xFF000000) >> 24);
    pMacAddr->addr[1] = ((macAddr0 & 0xFF0000) >> 16) ;
    pMacAddr->addr[2] = ((macAddr0 & 0xFF00) >> 8);
    pMacAddr->addr[3] = (macAddr0 & 0xFF);
    pMacAddr->addr[4] = ((macAddr1 & 0xFF00) >> 8);
    pMacAddr->addr[5] = (macAddr1 & 0xFF);
    
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_add_cfg(yt_unit_t unit, yt_l2_fdb_op_mode_t opMode, 
                                                        l2_fdb_tbl_t l2Fdb, yt_l2_fdb_op_result_t *pOpResult)
{
    l2_fdb_tbl_op_t l2FdbTblOp;
    l2_fdb_tbl_op_data_0_t l2FdbTblOpData0;
    l2_fdb_tbl_op_data_1_t l2FdbTblOpData1;
    l2_fdb_tbl_op_data_2_t l2FdbTblOpData2;
    cmm_err_t ret           = CMM_ERR_OK;
    uint32_t macAddr0 = 0;
    uint32_t macAddr1 = 0;
    uint32_t status = 0;
    uint32_t dmacIntPriEn = 0;
    uint32_t fid = 0;
    uint32_t newVid = 0;
    uint32_t intPri = 0;
    uint32_t smacIntPriEn = 0;
    uint32_t copyToCpu = 0;
    uint32_t dmacDrop = 0;
    uint32_t dstPortMask = 0;
    uint32_t smacDrop = 0;
    uint32_t moveAgingStatus = 0;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    osal_memset(&l2FdbTblOpData0, sizeof(l2_fdb_tbl_op_data_0_t), 0, sizeof(l2_fdb_tbl_op_data_0_t));
    osal_memset(&l2FdbTblOpData1, sizeof(l2_fdb_tbl_op_data_1_t), 0, sizeof(l2_fdb_tbl_op_data_1_t));
    osal_memset(&l2FdbTblOpData2, sizeof(l2_fdb_tbl_op_data_2_t), 0, sizeof(l2_fdb_tbl_op_data_2_t));

    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_0f, &l2Fdb, &macAddr0);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_1f, &l2Fdb, &macAddr1);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_FIDf, &l2Fdb, &fid);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_STATUSf, &l2Fdb, &status);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_INT_PRI_ENf, &l2Fdb, &dmacIntPriEn);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_NEW_VIDf, &l2Fdb, &newVid);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_INT_PRIf, &l2Fdb, &intPri);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_INT_PRI_ENf, &l2Fdb, &smacIntPriEn);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_COPY_TO_CPUf, &l2Fdb, &copyToCpu);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DMAC_DROPf, &l2Fdb, &dmacDrop);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DST_PORT_MASKf, &l2Fdb, &dstPortMask);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_SMAC_DROPf, &l2Fdb, &smacDrop);
    HAL_FIELD_GET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MOVE_AGING_STATUSf, &l2Fdb, &moveAgingStatus);

    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_0m, L2_FDB_TBL_OP_DATA_0_MAC_DA_0f, &l2FdbTblOpData0, macAddr0);
    
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_MAC_DA_1f, &l2FdbTblOpData1, macAddr1);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_FIDf, &l2FdbTblOpData1, fid);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_STATUSf, &l2FdbTblOpData1, status);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_DMAC_INT_PRI_ENf, &l2FdbTblOpData1, dmacIntPriEn);

    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_NEW_VIDf, &l2FdbTblOpData2, newVid);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_INT_PRIf, &l2FdbTblOpData2, intPri);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_SMAC_INT_PRI_ENf, &l2FdbTblOpData2, smacIntPriEn);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_COPY_TO_CPUf, &l2FdbTblOpData2, copyToCpu);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_DMAC_DROPf, &l2FdbTblOpData2, dmacDrop);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_DST_PORT_MASKf, &l2FdbTblOpData2, dstPortMask);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_SMAC_DROPf, &l2FdbTblOpData2, smacDrop);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_MOVE_AGING_STATUSf, &l2FdbTblOpData2, moveAgingStatus);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_0m, 0, sizeof(l2FdbTblOpData0), &l2FdbTblOpData0), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_1m, 0, sizeof(l2FdbTblOpData1), &l2FdbTblOpData1), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_2m, 0, sizeof(l2FdbTblOpData2), &l2FdbTblOpData2), ret);
    osal_memset(&l2FdbTblOp, sizeof(l2FdbTblOp), 0, sizeof(l2FdbTblOp));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_ADD);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2FdbTblOp, opMode.l2FdbOpMode);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_ENTRY_INDEXf, &l2FdbTblOp, opMode.entryIdx);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);
    
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);
    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_add(yt_unit_t unit,  yt_l2_fdb_op_mode_t opMode,
            uint16_t fid, yt_mac_addr_t macAddr, yt_port_mask_t portMask,
            yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_t l2Fdb;
    yt_l2_fdb_info_t fdbInfo;
    uint16_t lookupIndex = 0;
    
    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);

    osal_memset(&l2Fdb, sizeof(l2_fdb_tbl_t), 0, sizeof(l2_fdb_tbl_t));
    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));

    if (fal_tiger_l2_fdb_get(unit, fid,  macAddr,  &fdbInfo, &lookupIndex) == CMM_ERR_OK)
        CMM_ERR_CHK(fal_tiger_l2_tbl_read(unit, lookupIndex, &l2Fdb),ret);

    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_0f, &l2Fdb, ((macAddr.addr[0] << 24) | (macAddr.addr[1] << 16) | (macAddr.addr[2] << 8) | macAddr.addr[3]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_MAC_DA_1f, &l2Fdb, ((macAddr.addr[4] << 8) | macAddr.addr[5]));
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_FIDf, &l2Fdb, fid);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_STATUSf, &l2Fdb, FDB_STATUS_STATIC);
    HAL_FIELD_SET(L2_FDB_TBL_BIN0m, L2_FDB_TBL_BIN0_DST_PORT_MASKf, &l2Fdb, portMask.portbits[0]);
       
    CMM_ERR_CHK(fal_tiger_l2_fdb_op_add_cfg(unit, opMode, l2Fdb, pOpResult), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_del(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2FdbTblOp;
    l2_fdb_tbl_op_data_0_t l2FdbTblOpData0;
    l2_fdb_tbl_op_data_1_t l2FdbTblOpData1;
    l2_fdb_tbl_op_data_2_t l2FdbTblOpData2;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    osal_memset(&l2FdbTblOpData0, sizeof(l2_fdb_tbl_op_data_0_t), 0, sizeof(l2_fdb_tbl_op_data_0_t));
    osal_memset(&l2FdbTblOpData1, sizeof(l2_fdb_tbl_op_data_1_t), 0, sizeof(l2_fdb_tbl_op_data_1_t));
    osal_memset(&l2FdbTblOpData2, sizeof(l2_fdb_tbl_op_data_2_t), 0, sizeof(l2_fdb_tbl_op_data_2_t));

    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_0m, L2_FDB_TBL_OP_DATA_0_MAC_DA_0f, &l2FdbTblOpData0, ((macAddr.addr[0] << 24) | (macAddr.addr[1] << 16) | (macAddr.addr[2] << 8) | macAddr.addr[3]));
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_MAC_DA_1f, &l2FdbTblOpData1, ((macAddr.addr[4] << 8) | macAddr.addr[5]));
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_FIDf, &l2FdbTblOpData1, vid);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OP_DATA_0m, 0, sizeof(l2FdbTblOpData0), &l2FdbTblOpData0), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OP_DATA_1m, 0, sizeof(l2FdbTblOpData1), &l2FdbTblOpData1), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OP_DATA_2m, 0, sizeof(l2FdbTblOpData2), &l2FdbTblOpData2), ret);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_DEL);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2FdbTblOp, L2_FDB_OP_MODE_HASH);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);
    
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_get_one_withidx(yt_unit_t unit, uint16_t idx, 
                                                                    yt_mac_addr_t *pMacAddr, uint16_t *pFid, yt_l2_fdb_info_t *pFdbInfo,
                                                                    yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2FdbTblOp;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFdbInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMacAddr), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_GET_ONE);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2FdbTblOp, L2_FDB_OP_MODE_INDEX);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_ENTRY_INDEXf, &l2FdbTblOp, idx);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_info_get(unit, pMacAddr, pFid,  pFdbInfo), ret);
    
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_get_next_withidx(yt_unit_t unit, uint16_t idx, 
                                                                    yt_mac_addr_t *pMacAddr, uint16_t *pFid, uint16_t *pNextIndex, 
                                                                    yt_l2_fdb_info_t *pFdbInfo, yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2FdbTblOp;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFdbInfo), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pMacAddr), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pNextIndex), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_GET_NEXT);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_GET_NEXT_TYPEf, &l2FdbTblOp, FAL_TIGER_FDB_GET_NEXT_UCAST_ONE);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2FdbTblOp, L2_FDB_OP_MODE_INDEX);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_ENTRY_INDEXf, &l2FdbTblOp, idx);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);

    if (pOpResult->lookupFail == TRUE)
        return CMM_ERR_ENTRY_NOT_FOUND;

    *pNextIndex = pOpResult->entryIdx;

    CMM_ERR_CHK(fal_tiger_l2_op_info_get(unit, pMacAddr, pFid,  pFdbInfo), ret);
    
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_get_next_withidx_for_mc(yt_unit_t unit, uint16_t idx, 
                                                                    yt_mac_addr_t *pmac_addr, uint16_t *pfid, uint16_t *pnext_index, 
                                                                    yt_l2_fdb_info_t *pfdb_info, yt_l2_fdb_op_result_t *pop_result)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2_fdb_tbl_op;

    CMM_PARAM_CHK((NULL == pop_result), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pfdb_info), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pfid), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pmac_addr), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pnext_index), CMM_ERR_NULL_POINT);

    osal_memset(&l2_fdb_tbl_op, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2_fdb_tbl_op, FAL_TIGER_FDB_OP_CMD_GET_NEXT);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_GET_NEXT_TYPEf, &l2_fdb_tbl_op, FAL_TIGER_FDB_GET_NEXT_MCAST_ONE);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2_fdb_tbl_op, L2_FDB_OP_MODE_INDEX);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_ENTRY_INDEXf, &l2_fdb_tbl_op, idx);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2_fdb_tbl_op, TRUE);

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2_fdb_tbl_op), &l2_fdb_tbl_op), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pop_result), ret);

    if (pop_result->lookupFail == TRUE)
        return CMM_ERR_ENTRY_NOT_FOUND;

    *pnext_index = pop_result->entryIdx;

    CMM_ERR_CHK(fal_tiger_l2_op_info_get(unit, pmac_addr, pfid,  pfdb_info), ret);
    
    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_get_one(yt_unit_t unit,uint16_t fid, yt_mac_addr_t macAddr, 
                                                        yt_l2_fdb_info_t *pFdbInfo, yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2FdbTblOp;
    l2_fdb_tbl_op_data_0_t l2FdbTblOpData0;
    l2_fdb_tbl_op_data_1_t l2FdbTblOpData1;
    l2_fdb_tbl_op_data_2_t l2FdbTblOpData2;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);
    CMM_PARAM_CHK((NULL == pFdbInfo), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    osal_memset(&l2FdbTblOpData0, sizeof(l2_fdb_tbl_op_data_0_t), 0, sizeof(l2_fdb_tbl_op_data_0_t));
    osal_memset(&l2FdbTblOpData1, sizeof(l2_fdb_tbl_op_data_1_t), 0, sizeof(l2_fdb_tbl_op_data_1_t));
    osal_memset(&l2FdbTblOpData2, sizeof(l2_fdb_tbl_op_data_2_t), 0, sizeof(l2_fdb_tbl_op_data_2_t));

    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_0m, L2_FDB_TBL_OP_DATA_0_MAC_DA_0f, &l2FdbTblOpData0, ((macAddr.addr[0] << 24) | (macAddr.addr[1] << 16) | (macAddr.addr[2] << 8) | macAddr.addr[3]));
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_MAC_DA_1f, &l2FdbTblOpData1, ((macAddr.addr[4] << 8) | macAddr.addr[5]));
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_FIDf, &l2FdbTblOpData1, fid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_0m, 0, sizeof(l2FdbTblOpData0), &l2FdbTblOpData0), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_1m, 0, sizeof(l2FdbTblOpData1), &l2FdbTblOpData1), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_2m, 0, sizeof(l2FdbTblOpData2), &l2FdbTblOpData2), ret);


    osal_memset(&l2FdbTblOp, sizeof(l2FdbTblOp), 0, sizeof(l2FdbTblOp));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_GET_ONE);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_MODEf, &l2FdbTblOp, L2_FDB_OP_MODE_HASH);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);
    
    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_info_get(unit, &macAddr, &fid, pFdbInfo), ret);

    return CMM_ERR_OK;
}

static uint32_t  fal_tiger_l2_fdb_op_flush(yt_unit_t unit, yt_l2_tbl_flush_ctrl_t opFlush, yt_l2_fdb_op_result_t *pOpResult)
{
    cmm_err_t ret           = CMM_ERR_OK;
    l2_fdb_tbl_op_t l2FdbTblOp;
    l2_fdb_tbl_op_data_0_t l2FdbTblOpData0;
    l2_fdb_tbl_op_data_1_t l2FdbTblOpData1;
    l2_fdb_tbl_op_data_2_t l2FdbTblOpData2;

    CMM_PARAM_CHK((NULL == pOpResult), CMM_ERR_NULL_POINT);

    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    osal_memset(&l2FdbTblOpData0, sizeof(l2_fdb_tbl_op_data_0_t), 0, sizeof(l2_fdb_tbl_op_data_0_t));
    osal_memset(&l2FdbTblOpData1, sizeof(l2_fdb_tbl_op_data_1_t), 0, sizeof(l2_fdb_tbl_op_data_1_t));
    osal_memset(&l2FdbTblOpData2, sizeof(l2_fdb_tbl_op_data_2_t), 0, sizeof(l2_fdb_tbl_op_data_2_t));

    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_1m, L2_FDB_TBL_OP_DATA_1_FIDf, &l2FdbTblOpData1, opFlush.fid);
    HAL_FIELD_SET(L2_FDB_TBL_OP_DATA_2m, L2_FDB_TBL_OP_DATA_2_DST_PORT_MASKf, &l2FdbTblOpData2, opFlush.portMask.portbits[0]);
      
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_0m, 0, sizeof(l2FdbTblOpData0), &l2FdbTblOpData0), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_1m, 0, sizeof(l2FdbTblOpData1), &l2FdbTblOpData1), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit,L2_FDB_TBL_OP_DATA_2m, 0, sizeof(l2FdbTblOpData2), &l2FdbTblOpData2), ret);


    osal_memset(&l2FdbTblOp, sizeof(l2_fdb_tbl_op_t), 0, sizeof(l2_fdb_tbl_op_t));
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_CMDf, &l2FdbTblOp, FAL_TIGER_FDB_OP_CMD_FLUSH);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_FLUSH_MODEf, &l2FdbTblOp, opFlush.mode);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_FLUSH_STATIC_ENf, &l2FdbTblOp, opFlush.flushStaticEn);
    HAL_FIELD_SET(L2_FDB_TBL_OPm, L2_FDB_TBL_OP_OP_STARTf, &l2FdbTblOp, TRUE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_FDB_TBL_OPm, 0, sizeof(l2FdbTblOp), &l2FdbTblOp), ret);

    CMM_ERR_CHK(fal_tiger_l2_op_result_get(unit, pOpResult), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_fdb_uc_withindex_get(yt_unit_t unit, uint16_t index, l2_ucastMacAddr_info_t *pUcastMac)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_info_t fdbInfo;
    yt_mac_addr_t macAddr;
    yt_port_t port = 0;
    yt_macid_t macid = 0;
    uint16_t fid = 0;
#ifdef LAG_INCLUDED
    uint8_t  lagGroupId = FAL_MAX_LAG_NUM;
#endif

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&macAddr, sizeof(yt_mac_addr_t), 0, sizeof(yt_mac_addr_t));

    if(CAL_L2_FDB_NUM_MAX(unit) <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_one_withidx(unit, index, &macAddr, &fid, &fdbInfo, &opResult),ret);

    if(FDB_STATUS_INVALID == fdbInfo.status ||
        IS_MCAST_ADDR(macAddr.addr))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    for(port =0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdbInfo.dstPortMask, macid))
            break;
    }

    pUcastMac->port = port;

#ifdef LAG_INCLUDED
    CMM_ERR_CHK(fal_tiger_lag_phyPort_belong_lagPort_get(unit, pUcastMac->port, &lagGroupId),ret);
    if (lagGroupId != FAL_MAX_LAG_NUM)
    {
        pUcastMac->port = lagGroupId;
        pUcastMac->isLag = TRUE;
    }
    else
    {
        pUcastMac->isLag = FALSE;
    }
#endif

    pUcastMac->vid = fid;
    osal_memcpy(pUcastMac->macaddr.addr, MAC_ADDR_LEN, macAddr.addr, MAC_ADDR_LEN);
    switch(fdbInfo.status)
    {
        case FDB_STATUS_PENDING:
            pUcastMac->type = YT_L2_FDB_TYPE_PENDING;
            break;
        case FDB_STATUS_STATIC:
            pUcastMac->type = YT_L2_FDB_TYPE_STATIC;
            break;
        default:
            pUcastMac->type = YT_L2_FDB_TYPE_DYNAMIC;
            break;
    }

    if (fdbInfo.dmacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_DA_DROP;
    }
    else if (fdbInfo.smacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_SA_DROP;
    }
    else if (fdbInfo.copyToCpu)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_COPY_TO_CPU;
    }
    else
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_FORWARD;
    }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_uc_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac)
{
    uint16_t lookupIndex   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdbInfo;
    yt_port_t port = 0;
    yt_macid_t macid = 0;
    uint8_t  lagGroupId = FAL_MAX_LAG_NUM;

    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));

    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  macAddr, &fdbInfo, &lookupIndex), ret);

    if(FDB_STATUS_INVALID == fdbInfo.status ||
        IS_MCAST_ADDR(macAddr.addr))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    switch(fdbInfo.status)
    {
        case FDB_STATUS_PENDING:
            pUcastMac->type = YT_L2_FDB_TYPE_PENDING;
            break;
        case FDB_STATUS_STATIC:
            pUcastMac->type = YT_L2_FDB_TYPE_STATIC;
            break;
        default:
            pUcastMac->type = YT_L2_FDB_TYPE_DYNAMIC;
            break;
    }

    pUcastMac->vid = vid;
    osal_memcpy(pUcastMac->macaddr.addr, MAC_ADDR_LEN, macAddr.addr, MAC_ADDR_LEN);

    for(port =0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdbInfo.dstPortMask, macid))
            break;
    }

    pUcastMac->port = port;
#ifdef LAG_INCLUDED
    CMM_ERR_CHK(fal_tiger_lag_phyPort_belong_lagPort_get(unit, pUcastMac->port, &lagGroupId),ret);
#endif
    if (lagGroupId != FAL_MAX_LAG_NUM)
    {
        pUcastMac->port = lagGroupId;
        pUcastMac->isLag = TRUE;
    }
    else
    {
        pUcastMac->isLag = FALSE;
    }
    if (fdbInfo.dmacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_DA_DROP;
    }
    else if (fdbInfo.smacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_SA_DROP;
    }
    else if (fdbInfo.copyToCpu)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_COPY_TO_CPU;
    }
    else
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_FORWARD;
    }
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_fdb_uc_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNext_index, l2_ucastMacAddr_info_t *pUcastMac)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t opResult;
    yt_l2_fdb_info_t fdbInfo;
    yt_mac_addr_t macAddr;
    yt_port_t port = 0;
    yt_macid_t macid = 0;
    uint16_t fid = 0;
    uint8_t  lagGroupId = FAL_MAX_LAG_NUM;

    osal_memset(&opResult, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&fdbInfo, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&macAddr, sizeof(yt_mac_addr_t), 0, sizeof(yt_mac_addr_t));

    if(CAL_L2_FDB_NUM_MAX(unit) - 1 <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_next_withidx(unit, index, &macAddr, &fid, pNext_index, &fdbInfo, &opResult),ret);

    if(FDB_STATUS_INVALID == fdbInfo.status ||
        IS_MCAST_ADDR(macAddr.addr))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    for(port =0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdbInfo.dstPortMask, macid))
            break;
    }

    pUcastMac->port = port;
#ifdef LAG_INCLUDED
    CMM_ERR_CHK(fal_tiger_lag_phyPort_belong_lagPort_get(unit, pUcastMac->port, &lagGroupId),ret);
#endif
    if (lagGroupId != FAL_MAX_LAG_NUM)
    {
        pUcastMac->port = lagGroupId;
        pUcastMac->isLag = TRUE;
    }
    else
    {
        pUcastMac->isLag = FALSE;
    }
    pUcastMac->vid = fid;
    osal_memcpy(pUcastMac->macaddr.addr, MAC_ADDR_LEN, macAddr.addr, MAC_ADDR_LEN);
    switch(fdbInfo.status)
    {
        case FDB_STATUS_PENDING:
            pUcastMac->type = YT_L2_FDB_TYPE_PENDING;
            break;
        case FDB_STATUS_STATIC:
            pUcastMac->type = YT_L2_FDB_TYPE_STATIC;
            break;
        default:
            pUcastMac->type = YT_L2_FDB_TYPE_DYNAMIC;
            break;
    }

    if (fdbInfo.dmacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_DA_DROP;
    }
    else if (fdbInfo.smacDrop)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_SA_DROP;
    }
    else if (fdbInfo.copyToCpu)
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_COPY_TO_CPU;
    }
    else
    {
        pUcastMac->action = YT_L2_FDB_ACTION_TYPE_FORWARD;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_port_learn_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    /* disable learn fdb*/
    HAL_FIELD_SET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2LearnPerPortCtrl, enable ? YT_DISABLE : YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_port_learn_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_learn_per_port_ctrln_t l2LearnPerPortCtrl;
    uint32 enable = 0;

    osal_memset(&l2LearnPerPortCtrl, sizeof(l2_learn_per_port_ctrln_t), 0, sizeof(l2_learn_per_port_ctrln_t));
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_LEARN_PER_PORT_CTRLNm, macid, sizeof(l2LearnPerPortCtrl), &l2LearnPerPortCtrl), ret);
    HAL_FIELD_GET(L2_LEARN_PER_PORT_CTRLNm, L2_LEARN_PER_PORT_CTRLN_LEARN_DISABLEf, &l2LearnPerPortCtrl, &enable);
    *pEnable = enable ? YT_DISABLE : YT_ENABLE;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_vlan_port_ucast_flush(yt_unit_t unit, yt_vlan_t vid, yt_port_t port, yt_bool_t isLag)
{
    yt_l2_tbl_flush_ctrl_t flushOp;
#ifdef LAG_INCLUDED
    cmm_err_t ret           = CMM_ERR_OK;
#endif

    osal_memset(&flushOp, sizeof(yt_l2_tbl_flush_ctrl_t), 0, sizeof(yt_l2_tbl_flush_ctrl_t));

    if(isLag == FALSE)/* phy port */
    {
        CMM_CLEAR_MEMBER_PORT(flushOp.portMask);
        CMM_SET_MEMBER_PORT(flushOp.portMask, CAL_YTP_TO_MAC(unit,port));
    }
    else if(isLag == TRUE) /* check lag port is valid or not */
    {
#ifdef LAG_INCLUDED
        CMM_PARAM_CHK((FAL_MAX_LAG_NUM <= port), CMM_ERR_INPUT);
        CMM_ERR_CHK(fal_tiger_lag_group_port_get(unit, port, &flushOp.portMask), ret);
        if (0 == flushOp.portMask.portbits[0])
        {
            return CMM_ERR_PORT;
        }
#else
        return CMM_ERR_NOT_SUPPORT;
#endif
    }
    
    flushOp.fid = vid;
    flushOp.mode = L2_FDB_FLUSH_MODE_UFDB_PORT_FID;

    return fal_tiger_l2_fdb_addr_flush(unit, flushOp);
}

yt_ret_t  fal_tiger_l2_port_learn_mode_set(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t mode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(mode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_port_learn_mode_get(yt_unit_t unit, yt_port_t port, yt_l2_learn_mode_t *pMode)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pMode);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_port_suspend_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(actType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_port_suspend_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pActType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_new_sa_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t actType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(actType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_new_sa_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pActType)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(port);
    CMM_UNUSED_PARAM(pActType);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_unknown_ucast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_uc_unknown_act_ctrl_t act;
    uint32_t l2_act = 0;
    uint32_t actionTmp = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    if (action == ACT_TYPE_FWD)
    {
        actionTmp = L2_UNKNOWN_ACTION_FWD;
    }
    else if (action == ACT_TYPE_DROP)
    {
        actionTmp = L2_UNKNOWN_ACTION_DROP;
    }
    else if (action == ACT_TYPE_COPY)
    {
        actionTmp = L2_UNKNOWN_ACTION_COPY;
    }
    else if (action == ACT_TYPE_TRAP)
    {
        actionTmp = L2_UNKNOWN_ACTION_TRAP;
    }
    else
    {
        ;
     }

    osal_memset(&act, sizeof(l2_uc_unknown_act_ctrl_t), 0, sizeof(l2_uc_unknown_act_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_UC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_uc_unknown_act_ctrl_t), &act), ret);    
    HAL_FIELD_GET(L2_UC_UNKNOWN_ACT_CTRLm, L2_UC_UNKNOWN_ACT_CTRL_L2_UC_UNKNOWN_ACTf, &act, &l2_act);
    l2_act &= (~(3 << (macid * 2)));
    l2_act |= actionTmp << (macid * 2);
    HAL_FIELD_SET(L2_UC_UNKNOWN_ACT_CTRLm, L2_UC_UNKNOWN_ACT_CTRL_L2_UC_UNKNOWN_ACTf, &act, l2_act);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_UC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_uc_unknown_act_ctrl_t), &act), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_unknown_ucast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_uc_unknown_act_ctrl_t act;
    uint32_t l2_act = 0;
    uint32_t actionTmp = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    osal_memset(&act, sizeof(l2_uc_unknown_act_ctrl_t), 0, sizeof(l2_uc_unknown_act_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_UC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_uc_unknown_act_ctrl_t), &act), ret);
    HAL_FIELD_GET(L2_UC_UNKNOWN_ACT_CTRLm, L2_UC_UNKNOWN_ACT_CTRL_L2_UC_UNKNOWN_ACTf, &act, &l2_act);
    actionTmp = (l2_act >> (macid * 2)) & 0x3;

    if (actionTmp == L2_UNKNOWN_ACTION_FWD)
    {
        *pAction = ACT_TYPE_FWD;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_DROP)
    {
        *pAction = ACT_TYPE_DROP;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_COPY)
    {
        *pAction = ACT_TYPE_COPY;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_TRAP)
    {
        *pAction = ACT_TYPE_TRAP;
    }
    else
    {
        ;
     }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_unknown_mcast_act_set(yt_unit_t unit, yt_port_t port, yt_act_type_t action)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_mc_unknown_act_ctrl_t act;
    uint32_t l2_act = 0;
    uint32_t actionTmp = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    if (action == ACT_TYPE_FWD)
    {
        actionTmp = L2_UNKNOWN_ACTION_FWD;
    }
    else if (action == ACT_TYPE_DROP)
    {
        actionTmp = L2_UNKNOWN_ACTION_DROP;
    }
    else if (action == ACT_TYPE_COPY)
    {
        actionTmp = L2_UNKNOWN_ACTION_COPY;
    }
    else if (action == ACT_TYPE_TRAP)
    {
        actionTmp = L2_UNKNOWN_ACTION_TRAP;
    }
    else
    {
        ;
     }

    osal_memset(&act, sizeof(l2_mc_unknown_act_ctrl_t), 0, sizeof(l2_mc_unknown_act_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &act), ret);
    HAL_FIELD_GET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_ACTf, &act, &l2_act);
    l2_act &= (~(3 << (macid * 2)));
    l2_act |= actionTmp << (macid * 2);
    HAL_FIELD_SET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_ACTf, &act, l2_act);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &act), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_unknown_mcast_act_get(yt_unit_t unit, yt_port_t port, yt_act_type_t *pAction)
{
    cmm_err_t ret = CMM_ERR_OK;
    l2_mc_unknown_act_ctrl_t act;
    uint32_t l2_act = 0;
    uint32_t actionTmp = 0;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit,port);

    osal_memset(&act, sizeof(l2_mc_unknown_act_ctrl_t), 0, sizeof(l2_mc_unknown_act_ctrl_t));
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_MC_UNKNOWN_ACT_CTRLm, 0, sizeof(l2_mc_unknown_act_ctrl_t), &act), ret);
    HAL_FIELD_GET(L2_MC_UNKNOWN_ACT_CTRLm, L2_MC_UNKNOWN_ACT_CTRL_L2_MC_UNKNOWN_ACTf, &act, &l2_act);
    actionTmp = (l2_act >> (macid * 2)) & 0x3;

    if (actionTmp == L2_UNKNOWN_ACTION_FWD)
    {
        *pAction = ACT_TYPE_FWD;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_DROP)
    {
        *pAction = ACT_TYPE_DROP;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_COPY)
    {
        *pAction = ACT_TYPE_COPY;
    }
    else if (actionTmp == L2_UNKNOWN_ACTION_TRAP)
    {
        *pAction = ACT_TYPE_TRAP;
    }
    else
    {
        ;
     }

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_fdb_add_notify_register(yt_unit_t unit, l2_fdb_add_notify func, yt_l2_fdb_notify_type_t type)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(func);
    CMM_UNUSED_PARAM(type);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_fdb_del_notify_register(yt_unit_t unit, l2_fdb_del_notify func, yt_l2_fdb_notify_type_t type)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(func);
    CMM_UNUSED_PARAM(type);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_fdb_uc_get_first_from_db(yt_unit_t unit, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pUcastMac);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t  fal_tiger_l2_fdb_uc_get_next_from_db(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t macAddr, l2_ucastMacAddr_info_t *pUcastMac)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(vid);
    CMM_UNUSED_PARAM(macAddr);
    CMM_UNUSED_PARAM(pUcastMac);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_l2_mcast_addr_withindex_get(yt_unit_t unit, uint16_t index, yt_mcast_mac_data_t *pMcastMac)
{
    cmm_err_t ret = CMM_ERR_OK;
    yt_l2_fdb_op_result_t op_result;
    yt_l2_fdb_info_t fdb_info;
    yt_mac_addr_t mac_addr;
    yt_port_t port = 0;
    yt_macid_t macid = 0;
    uint16_t fid = 0;

    osal_memset(&op_result, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&fdb_info, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&mac_addr, sizeof(yt_mac_addr_t), 0, sizeof(yt_mac_addr_t));

    if(CAL_L2_FDB_NUM_MAX(unit) <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_one_withidx(unit, index, &mac_addr, &fid, &fdb_info, &op_result),ret);

    if(FDB_STATUS_INVALID == fdb_info.status ||
        (!IS_MCAST_ADDR(mac_addr.addr)))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    CMM_CLEAR_MEMBER_PORT(pMcastMac->portMask);
    for(port = 0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdb_info.dstPortMask, macid))
        {
            SET_BIT(pMcastMac->portMask.portbits[0], port);
        }
    }
    
    pMcastMac->vid = fid;
    osal_memcpy(pMcastMac->macAddr.addr, MAC_ADDR_LEN, mac_addr.addr, MAC_ADDR_LEN);
    if (fdb_info.status == FDB_STATUS_STATIC)
    {
        pMcastMac->type = YT_MULTI_TYPE_STATIC;
    }
    else
    {
        pMcastMac->type = YT_MULTI_TYPE_DYNAMIC;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_l2_mcast_addr_withMacAndVid_get(yt_unit_t unit, yt_vlan_t vid, yt_mac_addr_t mac_addr, yt_mcast_mac_data_t *pMcastMac)
{
    uint16_t lookup_index   = 0;
    cmm_err_t ret           = CMM_ERR_OK;
    yt_l2_fdb_info_t fdb_info;
    yt_macid_t macid = 0;
    yt_port_t port = 0;

    osal_memset(&fdb_info, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));

    CMM_ERR_CHK(fal_tiger_l2_fdb_get(unit, vid,  mac_addr, &fdb_info, &lookup_index), ret);

    if(FDB_STATUS_INVALID == fdb_info.status)
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    pMcastMac->vid = vid;
    osal_memcpy(pMcastMac->macAddr.addr, MAC_ADDR_LEN, mac_addr.addr, MAC_ADDR_LEN);

    CMM_CLEAR_MEMBER_PORT(pMcastMac->portMask);
    for(port = 0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdb_info.dstPortMask, macid))
        {
            SET_BIT(pMcastMac->portMask.portbits[0], port);
        }
    }

    if (fdb_info.status == FDB_STATUS_STATIC)
    {
        pMcastMac->type = YT_MULTI_TYPE_STATIC;
    }
    else
    {
        pMcastMac->type = YT_MULTI_TYPE_DYNAMIC;
    }
    
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_l2_mcast_addr_withindex_getnext(yt_unit_t unit, uint16_t index, uint16_t *pNext_index, yt_mcast_mac_data_t *pMcastMac)
{
    cmm_err_t ret = CMM_ERR_OK;

    yt_l2_fdb_op_result_t op_result;
    yt_l2_fdb_info_t fdb_info;
    yt_mac_addr_t mac_addr;
    yt_macid_t macid = 0;
    yt_port_t port = 0;
    uint16_t fid = 0;

    osal_memset(&op_result, sizeof(yt_l2_fdb_op_result_t), 0, sizeof(yt_l2_fdb_op_result_t));
    osal_memset(&fdb_info, sizeof(yt_l2_fdb_info_t), 0, sizeof(yt_l2_fdb_info_t));
    osal_memset(&mac_addr, sizeof(yt_mac_addr_t), 0, sizeof(yt_mac_addr_t));

    if(CAL_L2_FDB_NUM_MAX(unit) - 1 <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }

    CMM_ERR_CHK(fal_tiger_l2_fdb_op_get_next_withidx_for_mc(unit, index, &mac_addr, &fid, pNext_index, &fdb_info, &op_result),ret);

    if(FDB_STATUS_INVALID == fdb_info.status ||
        (!IS_MCAST_ADDR(mac_addr.addr)))
    {
        return CMM_ERR_ENTRY_NOT_FOUND;
    }

    CMM_CLEAR_MEMBER_PORT(pMcastMac->portMask);
    for(port = 0; port < CAL_MAX_PORT_NUM_ON_UNIT(unit); port++)
    {
        macid = CAL_YTP_TO_MAC(unit,port);
        if(IS_BIT_SET(fdb_info.dstPortMask, macid))
        {
            SET_BIT(pMcastMac->portMask.portbits[0], port);
        }
    }

    pMcastMac->vid = fid;
    osal_memcpy(pMcastMac->macAddr.addr, MAC_ADDR_LEN, mac_addr.addr, MAC_ADDR_LEN);
    if (fdb_info.status == FDB_STATUS_STATIC)
    {
        pMcastMac->type = YT_MULTI_TYPE_STATIC;
    }
    else
    {
        pMcastMac->type = YT_MULTI_TYPE_DYNAMIC;
    }

    return CMM_ERR_OK;
}