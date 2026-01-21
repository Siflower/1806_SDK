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
#include "yt_rate.h"
#include "fal_tiger_rate.h"

static uint32_t gPortRateInit;
static uint32_t gFlowMeterInit[FAL_TIGER_RATE_FLOWMETER_WORD_NUM];
static uint32_t gPortShapeInit;
static uint32_t gQshapeInit[FAL_TIGER_RATE_QSHP_WORD_NUM];
uint8_t g_cycle_time[YT_MAX_UNIT];

static uint32_t rate_tran_usr2reg(uint8_t rateMode, uint8_t tokenLevel, uint32_t timeSlot, uint32_t rate)
{
    uint32_t retRate;
    uint32_t divisor;
    uint64 dividend;

    divisor = 1000000000;
    if (rateMode == YT_RATE_MODE_BPS)
    {
        if (0 < (11 - 2 * tokenLevel))
        {
            dividend = ((uint64)rate * timeSlot) << (11 - 2 * tokenLevel);
        }
        else
        {
            dividend = ((uint64)rate * timeSlot) >> (2 * tokenLevel - 11);
        }
        
    }
    else
    {
        if (0 < (21 - 2 * tokenLevel))
        {
            dividend = ((uint64)rate * timeSlot) << (21 - 2 * tokenLevel);
        }
        else
        {
            dividend = ((uint64)rate * timeSlot) >> (2 * tokenLevel - 21);
        }
    }
    do_div64(dividend, divisor);
    retRate = (uint32_t)dividend;
    
    return retRate;
}

static uint32_t rate_tran_reg2usr(uint8_t rateMode, uint8_t tokenLevel, uint32_t timeSlot, uint32_t rate)
{
    uint32_t retRate;
    uint32_t divisor;
    uint64 dividend;

    divisor = timeSlot;
    if (rateMode == YT_RATE_MODE_BPS)
    {
        if (0 < (11 - 2 * tokenLevel))
        {
            dividend = ((uint64)rate * 1000000000) >> (11 - 2 * tokenLevel);
        }
        else
        {
            dividend = ((uint64)rate * 1000000000) << (2 * tokenLevel - 11);
        }
        
    }
    else
    {
        if (0 < (21 - 2 * tokenLevel))
        {
            dividend = ((uint64)rate * 1000000000) >> (21 - 2 * tokenLevel);
        }
        else
        {
            dividend = ((uint64)rate * 1000000000) << (2 * tokenLevel - 21);
        }
    }
    do_div64(dividend, divisor);
    retRate = (uint32_t)dividend;
    
    return retRate;
}

    
yt_ret_t fal_tiger_rate_init(yt_unit_t unit)
{
    global_ctrl1_t globalCtrl;
    meter_timeslot_t timeSlotEntry;
    psch_shp_slot_time_cfg_t psch_time_slot;
    qsch_shp_slot_time_cfg_t qsch_time_slot;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t clkSel;
    uint32_t timeSlot = 245;

    gPortRateInit = 0;
    gPortShapeInit = 0;
    osal_memset(gFlowMeterInit, sizeof(gFlowMeterInit), 0, sizeof(gFlowMeterInit));
    osal_memset(gQshapeInit, sizeof(gQshapeInit), 0, sizeof(gQshapeInit));

    /* get cycle time*/
    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, FAL_SYS_CLK_REG, &clkSel), ret);
    switch(clkSel&0x3)
    {
        case 0:/*125M for 9215, 167M for 9218*/
            if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9218)
            {
                FAL_CHIP_CYCLE_TIME(unit) = 6;
                timeSlot = 326;
            }
            else if (CAL_SWCHIP_ID(unit) == YT_SW_ID_9215)
            {
                FAL_CHIP_CYCLE_TIME(unit) = 8;
                timeSlot = 245;
            }
            break;
        case 1:/*143M*/
            FAL_CHIP_CYCLE_TIME(unit) = 7;
            timeSlot = 280;
            break;
        default:
            FAL_CHIP_CYCLE_TIME(unit) = 8;
            timeSlot = 245;
            break;
    }

    /* set meter global state enable */
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &globalCtrl), ret);
    HAL_FIELD_SET(GLOBAL_CTRL1m, GLOBAL_CTRL1_METER_ENf, &globalCtrl, YT_ENABLE);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, GLOBAL_CTRL1m, 0, sizeof(global_ctrl1_t), &globalCtrl), ret);

    /* meter timeSlot */
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    HAL_FIELD_SET(METER_TIMESLOTm, METER_TIMESLOT_TIMESLOTf, &timeSlotEntry, timeSlot);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    
    /* port shaping timeSlot */
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(psch_shp_slot_time_cfg_t), &psch_time_slot), ret);
    HAL_FIELD_SET(PSCH_SHP_SLOT_TIME_CFGm, PSCH_SHP_SLOT_TIME_CFG_PSCH_SHP_SLOT_TIMEf, &psch_time_slot, timeSlot);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(psch_shp_slot_time_cfg_t), &psch_time_slot), ret);


    /* queue shaping timeSlot */
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(qsch_shp_slot_time_cfg_t), &qsch_time_slot), ret);
    HAL_FIELD_SET(QSCH_SHP_SLOT_TIME_CFGm, QSCH_SHP_SLOT_TIME_CFG_QSCH_SHP_SLOT_TIMEf, &qsch_time_slot, timeSlot);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(qsch_shp_slot_time_cfg_t), &qsch_time_slot), ret);

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_rate_igrBandwidthCtrl_cfg_set(yt_unit_t unit, yt_macid_t macId, yt_meterid_t meterId)
{
    meter_config_tbl_t cfgTbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t cbs;

    if (IS_BIT_SET(gPortRateInit, macId))
    {
        return CMM_ERR_OK;
    }

    meterId += CAL_MAX_METER_ENTRY_NUM(unit);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &cfgTbl), ret);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &cfgTbl, FAL_TIGER_DEFAULT_TOKEN_UNIT);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &cfgTbl, YT_RATE_MODE_BPS);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_ENf, &cfgTbl, YT_ENABLE);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_MODEf, &cfgTbl, YT_METER_MODE_RFC4115);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_COLOR_MODEf, &cfgTbl, YT_COLOR_BLIND);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_DROP_COLORf, &cfgTbl, YT_DROP_COLOR_YR);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CFf, &cfgTbl, YT_CF_MODE_NONE);
    /* 1*unit for 2^15 tokens */
    cbs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 15));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CBS_0f, &cfgTbl, (cbs & 0xfff));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CBS_1f, &cfgTbl, (cbs >> 12));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EIRf, &cfgTbl, 0);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EBS_0f, &cfgTbl, 0);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EBS_1f, &cfgTbl, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &cfgTbl), ret);

    SET_BIT(gPortRateInit, macId);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_igrBandwidthCtrlEnable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    port_meter_ctrln_t port_meter_ctrl;
    yt_meterid_t meterId;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    meterId = macId;
    CMM_ERR_CHK(fal_tiger_rate_igrBandwidthCtrl_cfg_set(unit, macId, meterId), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_METER_CTRLNm, macId, sizeof(port_meter_ctrln_t), &port_meter_ctrl), ret);
    HAL_FIELD_SET(PORT_METER_CTRLNm, PORT_METER_CTRLN_METER_IDf, &port_meter_ctrl, meterId);
    HAL_FIELD_SET(PORT_METER_CTRLNm, PORT_METER_CTRLN_METER_ENf, &port_meter_ctrl, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PORT_METER_CTRLNm, macId, sizeof(port_meter_ctrln_t), &port_meter_ctrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_igrBandwidthCtrlEnable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    port_meter_ctrln_t port_meter_ctrl;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PORT_METER_CTRLNm, macid, sizeof(port_meter_ctrln_t), &port_meter_ctrl), ret);
    HAL_FIELD_GET(PORT_METER_CTRLNm, PORT_METER_CTRLN_METER_ENf, &port_meter_ctrl, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_igrBandwidthCtrlMode_set(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode)
{
    meter_config_tbl_t meter_config_tbl;
    yt_meterid_t meterId;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    meterId = macid + CAL_MAX_METER_ENTRY_NUM(unit);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, pRateMode->rateMode);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_BYTE_RATE_MODEf, &meter_config_tbl, pRateMode->gapMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_igrBandwidthCtrlMode_get(yt_unit_t unit, yt_port_t port, yt_port_rate_mode_t *pRateMode)
{
    meter_config_tbl_t meter_config_tbl;
    yt_meterid_t meterId;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);
    uint8_t rateMode = 0;
    uint8_t byte_rate_mode = 0;

    meterId = macid + CAL_MAX_METER_ENTRY_NUM(unit);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, &rateMode);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_BYTE_RATE_MODEf, &meter_config_tbl, &byte_rate_mode);
    pRateMode->rateMode = rateMode;
    pRateMode->gapMode = byte_rate_mode;

    return CMM_ERR_OK;
}
yt_ret_t fal_tiger_rate_igrBandwidthCtrlRate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate)
{
    meter_timeslot_t timeSlotEntry;
    meter_config_tbl_t meter_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, rateMode;
    uint32_t timeSlot;
    uint32_t cir, meterId;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    meterId = macid + CAL_MAX_METER_ENTRY_NUM(unit);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(METER_TIMESLOTm, METER_TIMESLOT_TIMESLOTf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &meter_config_tbl, &tokenLevel);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, &rateMode);
    cir = rate_tran_usr2reg(rateMode, tokenLevel, timeSlot, rate);

    if (cir > (uint32_t)(0x40000 - 1))
    {
        osal_printf("Exceed CIR max tokens: timeSlot=%d, tokenLevel=%d, cir=0x%x.\r\n", timeSlot, tokenLevel, cir);
        return CMM_ERR_NOT_SUPPORT;
    }

    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CIRf, &meter_config_tbl, cir);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_igrBandwidthCtrlRate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate)
{
    meter_timeslot_t timeSlotEntry;
    meter_config_tbl_t meter_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, rateMode;
    uint32_t timeSlot;
    uint32_t cir, meterId;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    meterId = macid + CAL_MAX_METER_ENTRY_NUM(unit);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(METER_TIMESLOTm, METER_TIMESLOT_TIMESLOTf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &meter_config_tbl, &tokenLevel);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CIRf, &meter_config_tbl, &cir);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, &rateMode);
    *pRate = rate_tran_reg2usr(rateMode, tokenLevel, timeSlot, cir);
    
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_vlan_enable_set(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t meterId, yt_enable_t enable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_METER_IDf, &entry, meterId);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_METER_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_vlan_enable_get(yt_unit_t unit, yt_vlan_t vid, yt_meterid_t *pMeter_id, yt_enable_t *pEnable)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_METER_IDf, &entry, pMeter_id);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_METER_ENf, &entry, pEnable);

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_rate_meter_cfg_set(yt_unit_t unit, yt_meterid_t meterId)
{
    meter_config_tbl_t cfgTbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t wordPos;
    uint32_t bitPos;
    uint32_t cbs;
    uint32_t ebs;

    wordPos = meterId / FAL_TIGER_RATE_WORD_SIZE;
    bitPos = meterId % FAL_TIGER_RATE_WORD_SIZE;

    if (IS_BIT_SET(gFlowMeterInit[wordPos], bitPos))
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &cfgTbl), ret);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &cfgTbl, FAL_TIGER_DEFAULT_TOKEN_UNIT);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &cfgTbl, YT_RATE_MODE_BPS);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_MODEf, &cfgTbl, YT_METER_MODE_RFC4115);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_COLOR_MODEf, &cfgTbl, YT_COLOR_BLIND);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_DROP_COLORf, &cfgTbl, YT_DROP_COLOR_R);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CFf, &cfgTbl, YT_CF_MODE_NONE);

     /* 1*unit for 2^15 tokens */
    cbs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 15));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CBS_0f, &cfgTbl, (cbs & 0xfff));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CBS_1f, &cfgTbl, (cbs >> 12));
    /* 1*unit for 2^15 tokens */
    ebs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 15));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EBS_0f, &cfgTbl, (ebs & 0x3fff));
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EBS_1f, &cfgTbl, (ebs >> 14));
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &cfgTbl), ret);

    SET_BIT(gFlowMeterInit[wordPos], bitPos);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_enable_set(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t enable)
{
    meter_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(fal_tiger_rate_meter_cfg_set(unit, meterId), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_enable_get(yt_unit_t unit, yt_meterid_t meterId, yt_enable_t *pEnable)
{
    meter_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_ENf, &entry, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_mode_set(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode)
{
    meter_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);
    if (pMode->flags & YT_RATE_METER_FLAG_METERMODE)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_MODEf, &entry, pMode->meterMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_RATEMODE)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &entry, pMode->rateMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_COLORMODE)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_COLOR_MODEf, &entry, pMode->colorMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_DROPCOLOR)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_DROP_COLORf, &entry, pMode->dropColor);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_GAP)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_BYTE_RATE_MODEf, &entry, pMode->gapMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_CFMODE)
    {
        HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CFf, &entry, pMode->cfMode);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_mode_get(yt_unit_t unit, yt_meterid_t meterId, yt_rate_meter_mode_t *pMode)
{
    meter_config_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &entry), ret);
    if (pMode->flags & YT_RATE_METER_FLAG_METERMODE)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_MODEf, &entry, &pMode->meterMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_RATEMODE)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &entry, &pMode->rateMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_DROPCOLOR)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_DROP_COLORf, &entry, &pMode->dropColor);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_COLORMODE)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_COLOR_MODEf, &entry, &pMode->colorMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_GAP)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_BYTE_RATE_MODEf, &entry, &pMode->gapMode);
    }
    if (pMode->flags & YT_RATE_METER_FLAG_CFMODE)
    {
        HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CFf, &entry, &pMode->cfMode);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_rate_set(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate)
{
    meter_timeslot_t timeSlotEntry;
    meter_config_tbl_t meter_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, rateMode, meterMode;
    uint32_t timeSlot;
    uint32_t eir, cir;

    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(METER_TIMESLOTm, METER_TIMESLOT_TIMESLOTf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &meter_config_tbl, &tokenLevel);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, &rateMode);
    eir = rate_tran_usr2reg(rateMode, tokenLevel, timeSlot, pRate->eir);
    cir = rate_tran_usr2reg(rateMode, tokenLevel, timeSlot, pRate->cir);

    if (eir > (uint32_t)(0x40000 - 1) || cir > (uint32_t)(0x40000 - 1))
    {
        osal_printf("Exceed CIR/EIR max tokens: timeSlot=%d, tokenLevel=%d, eir=0x%x, cir=0x%x.\r\n", timeSlot, tokenLevel, eir, cir);
        return CMM_ERR_NOT_SUPPORT;
    }

    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_METER_MODEf, &meter_config_tbl, &meterMode);
    if (meterMode == YT_METER_MODE_RFC2698)
    {
        if ((eir < cir))
        {
            osal_printf("EIR should be larger than CIR in RFC2698 mode.\r\n");
            return CMM_ERR_NOT_SUPPORT;
        }
    }

    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EIRf, &meter_config_tbl, eir);
    HAL_FIELD_SET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CIRf, &meter_config_tbl, cir);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_meter_rate_get(yt_unit_t unit, yt_meterid_t meterId, yt_qos_two_rate_t *pRate)
{
    meter_timeslot_t timeSlotEntry;
    meter_config_tbl_t meter_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, rateMode;
    uint32_t timeSlot;
    uint32_t eir, cir;

    CMM_PARAM_CHK(CAL_MAX_METER_ENTRY_NUM(unit) <= meterId, CMM_ERR_INPUT);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_TIMESLOTm, 0, sizeof(meter_timeslot_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(METER_TIMESLOTm, METER_TIMESLOT_TIMESLOTf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, METER_CONFIG_TBLm, meterId, sizeof(meter_config_tbl_t), &meter_config_tbl), ret);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_TOKEN_UNITf, &meter_config_tbl, &tokenLevel);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_EIRf, &meter_config_tbl, &eir);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_CIRf, &meter_config_tbl, &cir);
    HAL_FIELD_GET(METER_CONFIG_TBLm, METER_CONFIG_TBL_RATE_MODEf, &meter_config_tbl, &rateMode);
    pRate->cir = rate_tran_reg2usr(rateMode, tokenLevel, timeSlot, cir);
    pRate->eir = rate_tran_reg2usr(rateMode, tokenLevel, timeSlot, eir);
    
    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_rate_shaping_port_cfg_set(yt_unit_t unit, yt_macid_t macId)
{
    psch_shp_cfg_tbl_t cfgTbl;
    uint32_t cbs;
    cmm_err_t ret = CMM_ERR_OK;

    if (IS_BIT_SET(gPortShapeInit, macId))
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macId, sizeof(psch_shp_cfg_tbl_t), &cfgTbl), ret);
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_TOKEN_LEVELf, &cfgTbl, FAL_TIGER_DEFAULT_TOKEN_UNIT);
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_SHAPER_MODEf, &cfgTbl, YT_RATE_MODE_BPS);
    /* 1*unit for 2^15 tokens */
    cbs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 16));
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_CBSf, &cfgTbl, cbs);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_SHP_CFG_TBLm, macId, sizeof(psch_shp_cfg_tbl_t), &cfgTbl), ret);

    SET_BIT(gPortShapeInit, macId);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    psch_shp_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(fal_tiger_rate_shaping_port_cfg_set(unit, macId), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macId, sizeof(psch_shp_cfg_tbl_t), &entry), ret);
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_C_SHAPER_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_SHP_CFG_TBLm, macId, sizeof(psch_shp_cfg_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    psch_shp_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &entry), ret);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_C_SHAPER_ENf, &entry, pEnable);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_mode_set(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode)
{
    psch_shp_cfg_tbl_t shpEntry;
    psch_meter_cfg_tbl_t meterEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    if ((YT_RATE_MODE_BPS == pShpMode->shpMode) && (YT_RATE_BPS_GAP_EXCLUDE == pShpMode->schMode))
    {
            osal_printf("%s Only include gap is supported in bps mode\n", __func__);
            return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &shpEntry), ret);
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_SHAPER_MODEf, &shpEntry, pShpMode->shpMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &shpEntry), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_METER_CFG_TBLm, macid, sizeof(psch_meter_cfg_tbl_t), &meterEntry), ret);
    HAL_FIELD_SET(PSCH_METER_CFG_TBLm, PSCH_METER_CFG_TBL_METER_LENGTHf, &meterEntry, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_METER_CFG_TBLm, macid, sizeof(psch_meter_cfg_tbl_t), &meterEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_mode_get(yt_unit_t unit, yt_port_t port, yt_shaping_mode_t *pShpMode)
{
    psch_shp_cfg_tbl_t shpEntry;
    psch_meter_cfg_tbl_t meterEntry;
    uint8_t schMode;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &shpEntry), ret);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_SHAPER_MODEf, &shpEntry, &pShpMode->shpMode);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_METER_CFG_TBLm, macid, sizeof(psch_meter_cfg_tbl_t), &meterEntry), ret);
    HAL_FIELD_GET(PSCH_METER_CFG_TBLm, PSCH_METER_CFG_TBL_METER_LENGTHf, &meterEntry, &schMode);
    pShpMode->schMode = YT_RATE_BPS_GAP_INCLUDE - schMode;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_rate_set(yt_unit_t unit, yt_port_t port, yt_rate_t rate)
{
    psch_shp_slot_time_cfg_t timeSlotEntry;
    psch_shp_cfg_tbl_t psch_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, shaperMode;
    uint32_t timeSlot;
    uint32_t cir;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(psch_shp_slot_time_cfg_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(PSCH_SHP_SLOT_TIME_CFGm, PSCH_SHP_SLOT_TIME_CFG_PSCH_SHP_SLOT_TIMEf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &psch_config_tbl), ret);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_TOKEN_LEVELf, &psch_config_tbl, &tokenLevel);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_SHAPER_MODEf, &psch_config_tbl, &shaperMode);
    cir = rate_tran_usr2reg(shaperMode, tokenLevel, timeSlot, rate);

    if (cir > (uint32_t)(0x40000 - 1))
    {
        osal_printf("Exceed CIR max tokens: timeSlot=%d, tokenLevel=%d, cir=0x%x.\r\n", timeSlot, tokenLevel, cir);
        return CMM_ERR_NOT_SUPPORT;
    }
    HAL_FIELD_SET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_CIRf, &psch_config_tbl, cir);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &psch_config_tbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_port_rate_get(yt_unit_t unit, yt_port_t port, yt_rate_t *pRate)
{
    psch_shp_slot_time_cfg_t timeSlotEntry;
    psch_shp_cfg_tbl_t psch_config_tbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, shaperMode;
    uint32_t timeSlot;
    uint32_t cir;
    yt_macid_t macid = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(psch_shp_slot_time_cfg_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(PSCH_SHP_SLOT_TIME_CFGm, PSCH_SHP_SLOT_TIME_CFG_PSCH_SHP_SLOT_TIMEf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PSCH_SHP_CFG_TBLm, macid, sizeof(psch_shp_cfg_tbl_t), &psch_config_tbl), ret);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_TOKEN_LEVELf, &psch_config_tbl, &tokenLevel);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_SHAPER_MODEf, &psch_config_tbl, &shaperMode);
    HAL_FIELD_GET(PSCH_SHP_CFG_TBLm, PSCH_SHP_CFG_TBL_CIRf, &psch_config_tbl, &cir);
    *pRate = rate_tran_reg2usr(shaperMode, tokenLevel, timeSlot, cir);

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_rate_shaping_queue_cfg_set(yt_unit_t unit, uint8_t qid)
{
    qsch_shp_cfg_tbl_t cfgTbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t cbs, ebs;
    uint32_t wordPos, bitPos;

    wordPos = qid / FAL_TIGER_RATE_WORD_SIZE;
    bitPos = qid % FAL_TIGER_RATE_WORD_SIZE;

    if (IS_BIT_SET(gQshapeInit[wordPos], bitPos))
    {
        return CMM_ERR_OK;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, qid, sizeof(qsch_shp_cfg_tbl_t), &cfgTbl), ret);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_TOKEN_LEVELf, &cfgTbl, FAL_TIGER_DEFAULT_TOKEN_UNIT);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_SHAPER_MODEf, &cfgTbl, YT_RATE_MODE_BPS);
    /* 1*unit for 2^15 tokens */
    cbs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 16));
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_CBSf, &cfgTbl, cbs);
     /* 1*unit for 2^15 tokens */
    ebs = (uint32_t)((FAL_TIGER_DEFAULT_BURST_SIZE << (14 - 2 * FAL_TIGER_DEFAULT_TOKEN_UNIT) >> 16));
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_EBSf, &cfgTbl, ebs);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_CFG_TBLm, qid, sizeof(qsch_shp_cfg_tbl_t), &cfgTbl), ret);

    SET_BIT(gQshapeInit[wordPos], bitPos);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t cShpEn, yt_enable_t eShpEn)
{
    qsch_shp_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(fal_tiger_rate_shaping_queue_cfg_set(unit, idx), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &entry), ret);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_C_SHAPER_ENf, &entry, cShpEn);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_E_SHAPER_ENf, &entry, eShpEn);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pCshpEn, yt_enable_t *pEshpEn)
{
    qsch_shp_cfg_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &entry), ret);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_C_SHAPER_ENf, &entry, pCshpEn);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_E_SHAPER_ENf, &entry, pEshpEn);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode)
{
    qsch_shp_cfg_tbl_t shpEntry;
    qsch_meter_cfg_tbl_t meterEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    if ((YT_RATE_MODE_BPS == pShpMode->shpMode) && (YT_RATE_BPS_GAP_EXCLUDE == pShpMode->schMode))
    {
            osal_printf("%s Only include gap is supported in bps mode\n", __func__);
            return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &shpEntry), ret);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_SHAPER_MODEf, &shpEntry, pShpMode->shpMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &shpEntry), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_METER_CFG_TBLm, idx, sizeof(qsch_meter_cfg_tbl_t), &meterEntry), ret);
    HAL_FIELD_SET(QSCH_METER_CFG_TBLm, QSCH_METER_CFG_TBL_METER_LENGTHf, &meterEntry, 0);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_METER_CFG_TBLm, idx, sizeof(qsch_meter_cfg_tbl_t), &meterEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_shaping_mode_t *pShpMode)
{
    qsch_shp_cfg_tbl_t shpEntry;
    qsch_meter_cfg_tbl_t meterEntry;
    uint8_t schMode;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &shpEntry), ret);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_SHAPER_MODEf, &shpEntry, &pShpMode->shpMode);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_METER_CFG_TBLm, idx, sizeof(qsch_meter_cfg_tbl_t), &meterEntry), ret);
    HAL_FIELD_GET(QSCH_METER_CFG_TBLm, QSCH_METER_CFG_TBL_METER_LENGTHf, &meterEntry, &schMode);
    pShpMode->schMode = YT_RATE_BPS_GAP_INCLUDE - schMode;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_rate_set(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate)
{
    qsch_shp_slot_time_cfg_t timeSlotEntry;
    qsch_shp_cfg_tbl_t qShpTbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, shaperMode;
    uint32_t timeSlot;
    uint32_t eir, cir;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(qsch_shp_slot_time_cfg_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(QSCH_SHP_SLOT_TIME_CFGm, QSCH_SHP_SLOT_TIME_CFG_QSCH_SHP_SLOT_TIMEf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &qShpTbl), ret);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_TOKEN_LEVELf, &qShpTbl, &tokenLevel);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_SHAPER_MODEf, &qShpTbl, &shaperMode);
    eir = rate_tran_usr2reg(shaperMode, tokenLevel, timeSlot, pRate->eir);
    cir = rate_tran_usr2reg(shaperMode, tokenLevel, timeSlot, pRate->cir);

    if (eir > (uint32_t)(0x40000 - 1) || cir > (uint32_t)(0x40000 - 1))
    {
        osal_printf("Exceed CIR/EIR max tokens: timeSlot=%d, tokenLevel=%d.\r\n", timeSlot, tokenLevel);
        return CMM_ERR_NOT_SUPPORT;
    }

    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_EIRf, &qShpTbl, eir);
    HAL_FIELD_SET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_CIRf, &qShpTbl, cir);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &qShpTbl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_rate_shaping_queue_rate_get(yt_unit_t unit, yt_qid_t qinfo, yt_qos_two_rate_t *pRate)
{
    qsch_shp_slot_time_cfg_t timeSlotEntry;
    qsch_shp_cfg_tbl_t qShpTbl;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t tokenLevel, shaperMode;
    uint32_t timeSlot;
    uint32_t eir, cir;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_SLOT_TIME_CFGm, 0, sizeof(qsch_shp_slot_time_cfg_t), &timeSlotEntry), ret);
    HAL_FIELD_GET(QSCH_SHP_SLOT_TIME_CFGm, QSCH_SHP_SLOT_TIME_CFG_QSCH_SHP_SLOT_TIMEf, &timeSlotEntry, &timeSlot);
    timeSlot = timeSlot * 8 * FAL_CHIP_CYCLE_TIME(unit); //ns
    if (0 == timeSlot)
    {
        return CMM_ERR_NOT_INIT;

    }
    
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_SHP_CFG_TBLm, idx, sizeof(qsch_shp_cfg_tbl_t), &qShpTbl), ret);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_EIRf, &qShpTbl, &eir);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_CIRf, &qShpTbl, &cir);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_TOKEN_LEVELf, &qShpTbl, &tokenLevel);
    HAL_FIELD_GET(QSCH_SHP_CFG_TBLm, QSCH_SHP_CFG_TBL_SHAPER_MODEf, &qShpTbl, &shaperMode);
    pRate->cir = rate_tran_reg2usr(shaperMode, tokenLevel, timeSlot, cir);
    pRate->eir = rate_tran_reg2usr(shaperMode, tokenLevel, timeSlot, eir);

    return CMM_ERR_OK;
}
