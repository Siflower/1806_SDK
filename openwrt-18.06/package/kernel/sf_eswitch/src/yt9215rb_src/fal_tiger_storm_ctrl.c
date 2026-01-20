/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "osal_math.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "hal_mem.h"
#include "yt_storm_ctrl.h"
#include "fal_tiger_storm_ctrl.h"

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */
static uint32_t fal_tiger_get_regidx(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint8_t *pIdx)
{
    yt_macid_t macId;

    macId = CAL_YTP_TO_MAC(unit,port);

    switch(stormType)
    {
        case YT_STORM_TYPE_BCAST:
            *pIdx = macId;
            break;
        case YT_STORM_TYPE_L2_MCAST:
        case YT_STORM_TYPE_L2_UNKNOWN_MCAST:
            *pIdx = FAL_MAX_PORT_NUM + macId;
            break;
        case YT_STORM_TYPE_UNKNOWN_UCAST:
            *pIdx = FAL_MAX_PORT_NUM * 2 + macId;
            break;
        default:
            return CMM_ERR_NOT_SUPPORT;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_init(yt_unit_t unit)
{
    global_ctrl1_t globalCtrlTbl;
    storm_ctrl_timeslot_t   stormCtrlTimeslot;
    cmm_err_t ret = CMM_ERR_OK;

    /* set meter global state enable */
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &globalCtrlTbl), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_METER_ENf, &globalCtrlTbl, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &globalCtrlTbl), ret);
    /*init timeslot*/
    HAL_FIELD_SET(STORM_CTRL_TIMESLOTm, STORM_CTRL_TIMESLOT_TIMESLOTf, &stormCtrlTimeslot, STORM_DEFAULT_TIMESLOT);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, STORM_CTRL_TIMESLOTm, 0, sizeof(storm_ctrl_timeslot_t), &stormCtrlTimeslot), ret);
    
    return CMM_ERR_OK;    
}

yt_ret_t fal_tiger_storm_ctrl_enable_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t enable)
{
    storm_ctrl_config_tbl_t entry;
    storm_ctrl_mc_type_ctrl_t mcEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;
    yt_macid_t macId;
    uint16_t mcType = 0;

    macId = CAL_YTP_TO_MAC(unit,port);

    if(fal_tiger_get_regidx(unit, port, stormType, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret);
    HAL_FIELD_SET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_STORM_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret);

    if(stormType == YT_STORM_TYPE_L2_UNKNOWN_MCAST)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_MC_TYPE_CTRLm, 0, sizeof(storm_ctrl_mc_type_ctrl_t), &mcEntry), ret);
        HAL_FIELD_GET(STORM_CTRL_MC_TYPE_CTRLm, STORM_CTRL_MC_TYPE_CTRL_STORM_CTRL_MC_TYPEf, &mcEntry, &mcType);
        if(enable == YT_ENABLE)
        {
            mcType |= (0x1 << macId);
        }
        else
        {
            mcType &= ~(0x1 << macId);
        }
        
        HAL_FIELD_SET(STORM_CTRL_MC_TYPE_CTRLm, STORM_CTRL_MC_TYPE_CTRL_STORM_CTRL_MC_TYPEf, &mcEntry, mcType);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, STORM_CTRL_MC_TYPE_CTRLm, 0, sizeof(storm_ctrl_mc_type_ctrl_t), &mcEntry), ret);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_enable_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_enable_t *pEnable)
{
    storm_ctrl_config_tbl_t entry;
    storm_ctrl_mc_type_ctrl_t mcEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;
    yt_macid_t macId;
    uint8_t stormEn = 0;
    uint16_t mcType = 0;

    macId = CAL_YTP_TO_MAC(unit,port);

    if(fal_tiger_get_regidx(unit, port, stormType, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret); 
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_STORM_ENf, &entry, &stormEn);
    *pEnable = stormEn ? YT_ENABLE : YT_DISABLE;

    if(stormType == YT_STORM_TYPE_L2_UNKNOWN_MCAST)
    {
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_MC_TYPE_CTRLm, 0, sizeof(storm_ctrl_mc_type_ctrl_t), &mcEntry), ret);
        HAL_FIELD_GET(STORM_CTRL_MC_TYPE_CTRLm, STORM_CTRL_MC_TYPE_CTRL_STORM_CTRL_MC_TYPEf, &mcEntry, &mcType);
        if((mcType & (0x1 << macId)))
        {
            *pEnable = YT_ENABLE;
        }
        else
        {
            *pEnable = YT_DISABLE;
        }
    }    

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_rate_mode_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t rateMode)
{
    storm_ctrl_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_get_regidx(unit, port, stormType, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }
        
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret); 
    HAL_FIELD_SET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_RATE_MODEf, &entry, rateMode.rateMode);
    HAL_FIELD_SET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_GAP_INCf, &entry, rateMode.gapMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_rate_mode_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, yt_port_rate_mode_t *pRateMode)
{
    storm_ctrl_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;
    uint8_t rateMode = 0;
    uint8_t gapInc = 0;

    if(fal_tiger_get_regidx(unit, port, stormType, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }
        
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &entry), ret); 
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_RATE_MODEf, &entry, &rateMode);
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_GAP_INCf, &entry, &gapInc);
    pRateMode->rateMode = rateMode ? YT_RATE_MODE_PPS : YT_RATE_MODE_BPS;
    pRateMode->gapMode = gapInc ? YT_RATE_BPS_GAP_INCLUDE : YT_RATE_BPS_GAP_EXCLUDE;
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_rate_set(yt_unit_t unit, yt_port_t port, yt_storm_type_t stormType, uint32_t rate)
{
    storm_ctrl_timeslot_t stormCtrlTimeslot;
    storm_ctrl_config_tbl_t stormCtrlConfigCbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t timeSlot;
    uint32_t cir = 0, cbs = 0;
    uint8_t idx = 0;
    uint8_t rateMode = 0;
    uint32_t divisor;
    uint64 dividend;

    if(fal_tiger_get_regidx(unit, port, stormType, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_TIMESLOTm, 0, sizeof(storm_ctrl_timeslot_t), &stormCtrlTimeslot), ret);
    HAL_FIELD_GET(STORM_CTRL_TIMESLOTm, STORM_CTRL_TIMESLOT_TIMESLOTf, &stormCtrlTimeslot, &timeSlot);
    timeSlot = timeSlot * 10000;/*10 * 1000,ns*/

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &stormCtrlConfigCbl), ret);
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_RATE_MODEf, &stormCtrlConfigCbl, &rateMode);
    divisor = 1000000000;
    if (rateMode == YT_RATE_MODE_BPS)
    {
        /* 1 token 1 byte */
        dividend = ((uint64)rate * timeSlot) >> 3;
        do_div64(dividend, divisor); /*us->s*/
        cir = (uint32_t)dividend;
        cir = (cir == 0) ? 1 : cir;
        
        cbs = STORM_DEFAULT_CBS_BYTE;/*x/512, 1*unit for 2^9 tokens*/
    }
    else 
    {
        /* 1 token 1/1024 packet */
        dividend = (uint64)rate * 1024 * timeSlot;
        do_div64(dividend, divisor); /*us->s*/
        cir = (uint32_t)dividend;
        cir = (cir <= 1) ? 2 : cir;
        
        cbs = STORM_DEFAULT_CBS_PACKET; /*x*1024 /512 1*unit for 2^9 tokens */
    }

    cbs = (cbs == 0) ? 1 : cbs;

    if (cir > (0x80000 - 1)) 
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    if (cbs > (0x400 - 1)) 
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    HAL_FIELD_SET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_CIRf, &stormCtrlConfigCbl, cir);
    HAL_FIELD_SET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_CBSf, &stormCtrlConfigCbl, cbs);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &stormCtrlConfigCbl), ret);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_storm_ctrl_rate_get(yt_unit_t unit, yt_port_t port, yt_storm_type_t storm_type, uint32_t *prate)
{
    storm_ctrl_timeslot_t stormCtrlTimeslot;
    storm_ctrl_config_tbl_t stormCtrlConfigCbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t timeSlot;
    uint8_t idx = 0;
    uint8_t rateMode = 0;
    uint32_t cir = 0;
    uint32_t divisor;
    uint64 dividend;

    if(fal_tiger_get_regidx(unit, port, storm_type, &idx) == CMM_ERR_NOT_SUPPORT)
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_TIMESLOTm, 0, sizeof(storm_ctrl_timeslot_t), &stormCtrlTimeslot), ret);
    HAL_FIELD_GET(STORM_CTRL_TIMESLOTm, STORM_CTRL_TIMESLOT_TIMESLOTf, &stormCtrlTimeslot, &timeSlot);
    timeSlot = timeSlot * 10000;/*10 * 1000,ns*/
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, STORM_CTRL_CONFIG_TBLm, idx, sizeof(storm_ctrl_config_tbl_t), &stormCtrlConfigCbl), ret); 
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_RATE_MODEf, &stormCtrlConfigCbl, &rateMode);
    HAL_FIELD_GET(STORM_CTRL_CONFIG_TBLm, STORM_CTRL_CONFIG_TBL_CIRf, &stormCtrlConfigCbl, &cir);
    divisor = timeSlot;
    if (rateMode == YT_RATE_MODE_BPS)
    {
        dividend = (uint64)cir * 8 * 1000000000;
    }
    else
    {
        dividend = ((uint64)cir * 1000000000) >> 10;  /* 2^10 */
    }
    do_div64(dividend, divisor); /*us->s*/
    *prate = (uint32_t)dividend;

    return CMM_ERR_OK;
}
