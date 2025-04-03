/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_led.h"
#include "hal_mem.h"

/*
 * Symbol Definition
 */

/*
 * Macro Declaration
 */

/*
 * Data Declaration
 */
typedef enum led_action_e {
    LED_ACTION_10M_BLINK = 0x1,
    LED_ACTION_100M_BLINK = 0x2,
    LED_ACTION_1000M_BLINK = 0x4,
    LED_ACTION_COLLISION_BLINK_ENABLE = 0x8,
    LED_ACTION_10M_ON = 0x10,
    LED_ACTION_100M_ON = 0x20,
    LED_ACTION_1000M_ON = 0x40,
    LED_ACTION_RXACT_ON = 0x80,
    LED_ACTION_TXACT_ON = 0x100,
    LED_ACTION_RXACT_BLINK = 0x200,
    LED_ACTION_TXACT_BLINK = 0x400,
    LED_ACTION_HALFDUPLEX_ON = 0x800,
    LED_ACTION_FULLDUPLEX_ON = 0x1000,
    LED_ACTION_ACTIVE_BLINK_INDICATE = 0x2000,
    LED_ACTION_LOOPDETECT_INDICATE = 0x4000,
    LED_ACTION_EEE_INDICATE = 0x8000,
    LED_ACTION_COLLISION_BLINK = 0x10000,         /* only for LED0 */
    LED_ACTION_DISABLE_LINK_TRY = 0x20000,        /* only for LED0 */
    LED_ACTION_NUM
}led_action_t;

typedef struct yt_led_slot_s
{
    uint8_t serialId;
    uint8_t ledId;
}yt_led_slot_t;

/* remapping array for LED_DATA */
const yt_led_slot_t remapData[SLED_DATANUM_MAX][21] = 
{
    /* SLED_DATANUM_5 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0}
    },

    /* SLED_DATANUM_7 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}
    },

    /* SLED_DATANUM_10 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0}, 
        {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}
    },

    /* SLED_DATANUM_14 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0},
        {9, LED_ID_1}, {8, LED_ID_1}, {4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1}
    },
    
    /* SLED_DATANUM_15 */
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0},
        {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}, 
        {9, LED_ID_1}, {8, LED_ID_1}, {7, LED_ID_1}, {6, LED_ID_1}, {5, LED_ID_1}
    },

    /* SLED_DATANUM_21 */
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0},
        {9, LED_ID_1}, {8, LED_ID_1}, {4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1},
        {9, LED_ID_2}, {8, LED_ID_2}, {4, LED_ID_2}, {3, LED_ID_2}, {2, LED_ID_2}, {1, LED_ID_2}, {0, LED_ID_2}
    }
};

const uint8_t ledMaxNum[SLED_DATANUM_MAX] = {5, 7, 10, 14, 15, 21};

/*
 * Function Declaration
 */

/**
 * @internal      fal_tiger_led_enable
 * @endinternal
 *
 * @brief         enabel LED
 * @param[in]     unit                -unit id
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_enable(yt_unit_t unit)
{
    uint32_t regVal;
    uint32_t ret;
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    regVal |= (0x1UL << 21);
    HAL_MEM_DIRECT_WRITE(unit, LED_GLB_CTRL, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_mode_set
 * @endinternal
 *
 * @brief         select the mode of led
 * @param[in]     unit                -unit id
 * @param[in]     mode                -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_mode_set(yt_unit_t unit, yt_led_mode_t mode)
{
    uint32_t regVal;
    uint32_t ret;
    
    if ((mode != LED_MODE_PARALLEL) && (mode != LED_MODE_SERIAL))
    {
        return CMM_ERR_INPUT;
    }

    ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    regVal &= 0xfffffffc;
    regVal |= (mode & 0x3);
    HAL_MEM_DIRECT_WRITE(unit, LED_GLB_CTRL, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_mode_get
 * @endinternal
 *
 * @brief         get the mode of led
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -serial or parallel mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_mode_get(yt_unit_t unit, yt_led_mode_t *pMode)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t mode;
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    mode = regVal & 0x3;
    if ((mode != LED_MODE_PARALLEL) && (mode != LED_MODE_SERIAL))
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    *pMode = mode;
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_action_set
 * @endinternal
 *
 * @brief         select the action of LED0~2 per port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     ledActCfg           -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_action_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t ledActCfg)
{
    uint32_t ret;
    uint32_t ledCtrlAddr[LED_ID_NUM] = {LED_CTRL_0_BASE, LED_CTRL_1_BASE, LED_CTRL_2_BASE};
    uint32_t actMask[LED_ID_NUM] = {0x3ffff, 0xffff, 0xffff};
    uint32 actionSet = 0;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
    {
        return CMM_ERR_INPUT;
    }

    if (ledActCfg.spd10m_blink_en)
    {
        actionSet |= LED_ACTION_10M_BLINK;
    }
    
    if (ledActCfg.spd100m_blink_en)
    {
        actionSet |= LED_ACTION_100M_BLINK;
    }
    
    if (ledActCfg.spd1000m_blink_en)
    {
        actionSet |= LED_ACTION_1000M_BLINK;
    }
    
    if (ledActCfg.collision_blink_en)
    {
        actionSet |= LED_ACTION_COLLISION_BLINK_ENABLE;
    }
    
    if (ledActCfg.spd10m_on_en)
    {
        actionSet |= LED_ACTION_10M_ON;
    }
    
    if (ledActCfg.spd100m_on_en)
    {
        actionSet |= LED_ACTION_100M_ON;
    }
    
    if (ledActCfg.spd1000m_on_en)
    {
        actionSet |= LED_ACTION_1000M_ON;
    }
    
    if (ledActCfg.rxact_on_en)
    {
        actionSet |= LED_ACTION_RXACT_ON;
    }
    
    if (ledActCfg.txact_on_en)
    {
        actionSet |= LED_ACTION_TXACT_ON;
    }

    if (ledActCfg.rxact_blink_en)
    {
        actionSet |= LED_ACTION_RXACT_BLINK;
    }
    
    if (ledActCfg.txact_blink_en)
    {
        actionSet |= LED_ACTION_TXACT_BLINK;
    }
    
    if (ledActCfg.half_duplex_en)
    {
        actionSet |= LED_ACTION_HALFDUPLEX_ON;
    }
    
    if (ledActCfg.full_duplex_en)
    {
        actionSet |= LED_ACTION_FULLDUPLEX_ON;
    }
    
    if (ledActCfg.active_blink_indicate_en)
    {
        actionSet |= LED_ACTION_ACTIVE_BLINK_INDICATE;
    }
    
    if (ledActCfg.loopdetect_indicate_en)
    {
        actionSet |= LED_ACTION_LOOPDETECT_INDICATE;
    }
    
    if (ledActCfg.eee_indicate_en)
    {
        actionSet |= LED_ACTION_EEE_INDICATE;
    }
    
    if (ledActCfg.collision_blink_indicate_en)
    {
        actionSet |= LED_ACTION_COLLISION_BLINK;
    }
    
	/* tiger must set disable_link_try bit to 1 */
	if (LED_ID_0 == ledId)
	{
		actionSet |= LED_ACTION_DISABLE_LINK_TRY;
	}
    
    if (actionSet & (~actMask[ledId]))
    {
        return CMM_ERR_NOT_SUPPORT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = ledCtrlAddr[ledId] + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    regVal &= (~actMask[ledId]);
    regVal |= (actionSet & actMask[ledId]);
    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_action_get
 * @endinternal
 *
 * @brief         get the action of LED0~2 per port
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     pLedActCfg          -config of led action
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_action_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_act_cfg_t *pLedActCfg)
{
    uint32_t ret;
    uint32_t ledCtrlAddr[LED_ID_NUM] = {LED_CTRL_0_BASE, LED_CTRL_1_BASE, LED_CTRL_2_BASE};
    uint32_t actMask[LED_ID_NUM] = {0x3ffff, 0xffff, 0xffff};
    uint32 actionSet = 0;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = ledCtrlAddr[ledId] + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    actionSet = regVal & actMask[ledId];
    if (actionSet & LED_ACTION_10M_BLINK)
    {
        pLedActCfg->spd10m_blink_en = 1;
    }
    
    if (actionSet & LED_ACTION_100M_BLINK)
    {
        pLedActCfg->spd100m_blink_en = 1;
    }
    
    if (actionSet & LED_ACTION_1000M_BLINK)
    {
        pLedActCfg->spd1000m_blink_en = 1;
    }

    if (actionSet & LED_ACTION_COLLISION_BLINK_ENABLE)
    {
        pLedActCfg->collision_blink_en = 1;
    }

    if (actionSet & LED_ACTION_10M_ON)
    {
        pLedActCfg->spd10m_on_en = 1;
    }
    
    if (actionSet & LED_ACTION_100M_ON)
    {
        pLedActCfg->spd100m_on_en = 1;
    }
    
    if (actionSet & LED_ACTION_1000M_ON)
    {
        pLedActCfg->spd1000m_on_en = 1;
    }
    
    if (actionSet & LED_ACTION_RXACT_ON)
    {
        pLedActCfg->rxact_on_en = 1;
    }
    
    if (actionSet & LED_ACTION_TXACT_ON)
    {
        pLedActCfg->txact_on_en = 1;
    }
    
    if (actionSet & LED_ACTION_RXACT_BLINK)
    {
        pLedActCfg->rxact_blink_en = 1;
    }

    if (actionSet & LED_ACTION_TXACT_BLINK)
    {
        pLedActCfg->txact_blink_en = 1;
    }
    
    if (actionSet & LED_ACTION_HALFDUPLEX_ON)
    {
        pLedActCfg->half_duplex_en = 1;
    }
    
    if (actionSet & LED_ACTION_FULLDUPLEX_ON)
    {
        pLedActCfg->full_duplex_en = 1;
    }
    
    if (actionSet & LED_ACTION_ACTIVE_BLINK_INDICATE)
    {
        pLedActCfg->active_blink_indicate_en = 1;
    }
    
    if (actionSet & LED_ACTION_LOOPDETECT_INDICATE)
    {
        pLedActCfg->loopdetect_indicate_en = 1;
    }
    
    if (actionSet & LED_ACTION_EEE_INDICATE)
    {
        pLedActCfg->eee_indicate_en = 1;
    }
    
    if (actionSet & LED_ACTION_COLLISION_BLINK)
    {
        pLedActCfg->collision_blink_indicate_en = 1;
    }
    
    if (actionSet & LED_ACTION_DISABLE_LINK_TRY)
    {
        pLedActCfg->disable_link_try_en = 1;
    }
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_blink_freq_set
 * @endinternal
 *
 * @brief         select the frequency of blink
 * @param[in]     unit                -unit id
 * @param[in]     event               -event associated with blink
 * @param[in]     freq                -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_blink_freq_set(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t freq)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask;

    if ((event < LED_BLINK_EVENT_EEE) || (LED_BLINK_EVENT_OTHERS < event) || (freq < LED_BLINK_FREQ_2HZ) || (LED_BLINK_FREQ_16HZ < freq))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_1_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    valMask = 0x3UL << (28 + event * 2);
    regVal &= (~valMask);
    regVal |= (freq << (28 + event * 2));
    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_blink_freq_get
 * @endinternal
 *
 * @brief         get the frequency of blink
 * @param[in]     unit                -unit id
 * @param[in]     event               -event associated with blink
 * @param[out]    pFreq               -frequency of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_blink_freq_get(yt_unit_t unit, yt_port_t port, yt_led_blink_event_t event, yt_led_blink_freq_t *pFreq)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((event < LED_BLINK_EVENT_EEE) || (LED_BLINK_EVENT_OTHERS < event))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_1_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal = (regVal >> (28 + event * 2));
    *pFreq = regVal & 0x3;
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_blink_duty_set
 * @endinternal
 *
 * @brief         select the duty of blink
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     duty                -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_blink_duty_set(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t duty)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask = 0xe000000;

    if ((duty < LED_BLINK_DUTY_50) || (LED_BLINK_DUTY_17 < duty))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_1_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal &= (~valMask);
    regVal |= (duty << 25);
    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_blink_duty_get
 * @endinternal
 *
 * @brief         get the duty of blink
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[out]    pDuty               -duty of blink
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_blink_duty_get(yt_unit_t unit, yt_port_t port, yt_led_blink_duty_t *pDuty)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_1_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
        
    *pDuty = (regVal >> 25) & 0x7;
        
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_loopdetect_blink_rate_set
 * @endinternal
 *
 * @brief         select blinking rate of loopdetect
 * @param[in]     unit                -unit id
 * @param[in]     rate                -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_loopdetect_blink_rate_set(yt_unit_t unit, yt_led_loopdetect_blink_rate_t rate)
{
    uint32_t regVal;
    uint32_t ret;

     if ((rate < LED_LOOPDETECT_RATE_512MS) || (LED_LOOPDETECT_RATE_2048MS < rate))
    {
        return CMM_ERR_INPUT;
    }
     
    ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    regVal &= 0xffe7ffff;
    regVal |= ((rate & 0x3) << 19);
    HAL_MEM_DIRECT_WRITE(unit, LED_GLB_CTRL, regVal);
    
    return CMM_ERR_OK;
 }

/**
 * @internal      fal_tiger_led_loopdetect_blink_rate_get
 * @endinternal
 *
 * @brief         get blinking rate of loopdetect
 * @param[in]     unit                -unit id
 * @param[out]    pRate               -blinking rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_loopdetect_blink_rate_get(yt_unit_t unit, yt_led_loopdetect_blink_rate_t *pRate)
{
    uint32_t regVal;
    uint32_t ret;
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    *pRate = ((regVal >> 19) & 0x3);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_force_mode_set
 * @endinternal
 *
 * @brief         select the cpu force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     mode                -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_force_mode_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t mode)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask = 0x3UL << (18 + ledId*4);

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId) || (mode < LED_FORCE_MODE_NORMAL) || (LED_FORCE_MODE_HIGH < mode))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_2_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal &= (~valMask);
    regVal |= (mode << (18 + ledId*4));
    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;    
}

/**
 * @internal      fal_tiger_led_force_mode_get
 * @endinternal
 *
 * @brief         get cpu force mode of LED
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pMode               -force mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_force_mode_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_mode_t *pMode)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_2_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    *pMode = (regVal >> (18 + ledId*4)) & 0x3;
    
    return CMM_ERR_OK;    
}

/**
 * @internal      fal_tiger_led_force_rate_set
 * @endinternal
 *
 * @brief         select the rate of force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     rate                -rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_force_rate_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t rate)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask = 0x3UL << (16 + ledId*4);

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId) || (rate < LED_FORCE_RATE_512MS) || (LED_FORCE_RATE_NORMAL < rate))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_2_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal &= (~valMask);
    regVal |= (rate << (16 + ledId*4));
    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;    
}

/**
 * @internal      fal_tiger_led_force_rate_get
 * @endinternal
 *
 * @brief         get the rate of force mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[out]    pRate               -rate
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_force_rate_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_led_force_rate_t *pRate)
{
    uint32_t ret;
    yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    macid = CAL_YTP_TO_MAC(unit,port);
    regAddr = LED_CTRL_2_BASE + macid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    *pRate = (regVal >> (16 + ledId*4)) & 0x3;
    
    return CMM_ERR_OK;    
}

/**
 * @internal      fal_tiger_led_serial_activeMode_set
 * @endinternal
 *
 * @brief         select the active mode of serial LED
 * @param[in]     unit                -unit id
 * @param[in]     mode                -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_activeMode_set(yt_unit_t unit, yt_sled_activeMode_t mode)
{
    uint32_t regVal;
    uint32_t ret;

    if ((mode < LED_SERIAL_ACTIVE_MODE_HIGH) || (LED_SERIAL_ACTIVE_MODE_LOW < mode))
    {
        return CMM_ERR_INPUT;
    }
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_SERIAL_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    regVal &= 0xffffffef;
    regVal |= ((mode & 0x1) << 4);
    HAL_MEM_DIRECT_WRITE(unit, LED_SERIAL_CTRL, regVal);
    
    return CMM_ERR_OK;
}


/**
 * @internal      fal_tiger_led_serial_activeMode_get
 * @endinternal
 *
 * @brief         get the active mode of serial LED
 * @param[in]     unit                -unit id
 * @param[out]    pMode               -active mode
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_activeMode_get(yt_unit_t unit, yt_sled_activeMode_t *pMode)
{
    uint32_t regVal;
    uint32_t ret;
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_SERIAL_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    *pMode = (regVal >> 4) & 0x1;
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_serial_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of serial LED(dst-->src)
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_remapping_set(yt_unit_t unit, uint8_t index, yt_led_remapping_t dstInfo)
{
    uint32_t ret;
    yt_macid_t srcMacid;
    yt_macid_t dstMacid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask;
    yt_sled_dataNum_t dataNum;

    if ((dstInfo.ledId < LED_ID_0) || (LED_ID_2 < dstInfo.ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    if ((NULL == LEDDSCP_ON_UNIT(unit))|| (NULL == SLED_PARAM(unit)))
    {
        return CMM_ERR_NULL_POINT;
    }
    dataNum = SLED_PARAM(unit)->dataNum;
    
    if (ledMaxNum[dataNum] <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }
    
    srcMacid = remapData[dataNum][index].serialId;
    regAddr = LED_SERIAL_REMAPPING_BASE + (LED_ID_2 - remapData[dataNum][index].ledId) * 8 + (srcMacid / 5) * 4;
    
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    /* port num */
    dstMacid = CAL_YTP_TO_MAC(unit, dstInfo.port);
    valMask = 0xfUL << ((srcMacid % 5) * 6 + 2);
    regVal &= (~valMask);
    regVal |= ((dstMacid & 0xf) << ((srcMacid % 5) * 6 + 2));

    /* LED ID */
    valMask = 0x3UL << ((srcMacid % 5) * 6);
    regVal &= (~valMask);
    regVal |= ((dstInfo.ledId & 0x3) << ((srcMacid % 5) * 6));

    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
       return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_serial_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of serial LED
 * @param[in]     unit                -unit id
 * @param[in]     index               -index of led_data(invert)
 * @param[out]    pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_remapping_get(yt_unit_t unit, uint8_t index, yt_led_remapping_t *pDstInfo)
{
    uint32_t ret;
    yt_macid_t srcMacid;
    yt_macid_t dstMacid;
    uint32_t regAddr;
    uint32_t regVal;
    yt_sled_dataNum_t dataNum;

    if ((NULL == LEDDSCP_ON_UNIT(unit))|| (NULL == SLED_PARAM(unit)))
    {
        return CMM_ERR_NULL_POINT;
    }
    dataNum = SLED_PARAM(unit)->dataNum;
    
    if (ledMaxNum[dataNum] <= index)
    {
        return CMM_ERR_EXCEED_RANGE;
    }
    
    srcMacid = remapData[dataNum][index].serialId;

    regAddr = LED_SERIAL_REMAPPING_BASE + (LED_ID_2 - remapData[dataNum][index].ledId) * 8 + (srcMacid / 5) * 4;
    
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    /* port num */
    dstMacid = (regVal >> ((srcMacid % 5) * 6 + 2)) & 0xf;
    CAL_MAC_TO_YTP(unit, dstMacid, pDstInfo->port);
    
    /* LED ID */
    pDstInfo->ledId = (regVal >> ((srcMacid % 5) * 6)) & 0x3;
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_serial_enable_set
 * @endinternal
 *
 * @brief         enabel/disable serial LED
 * @param[in]     unit                -unit id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t valMask = 0x3000000;

    ret = HAL_MEM_DIRECT_READ(unit, LED_SERIAL_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal &= (~valMask);
    if (YT_ENABLE == enable)
    {
        regVal |= valMask;
    }
    HAL_MEM_DIRECT_WRITE(unit, LED_SERIAL_CTRL, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_serial_enable_get
 * @endinternal
 *
 * @brief         get enable state of serial LED
 * @param[in]     unit                -unit id
 * @param[out]    pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t valMask = 0x3000000;

    ret = HAL_MEM_DIRECT_READ(unit, LED_SERIAL_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    regVal &= valMask;
    if (valMask == regVal)
    {
        *pEnable = YT_ENABLE;
    }
    else
    {
        *pEnable = YT_DISABLE;
    }
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_output_set
 * @endinternal
 *
 * @brief         select output port mask of parallel LED
 * @param[in]     unit                -unit id
 * @param[in]     port_mask           -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_output_set(yt_unit_t unit, yt_port_mask_t port_mask)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t valMask = 0x3ff;
    yt_port_mask_t  macmask;

    ret = HAL_MEM_DIRECT_READ(unit, LED_PARALLEL_OUTPUT_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    CAL_YTPLIST_TO_MLIST(unit,port_mask, macmask);
    regVal &= (~valMask);
    regVal |= (macmask.portbits[0] & valMask);
    HAL_MEM_DIRECT_WRITE(unit, LED_PARALLEL_OUTPUT_CTRL, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_output_get
 * @endinternal
 *
 * @brief         get output port mask of parallel LED
 * @param[in]     unit                -unit id
 * @param[out]    pport_mask          -port bit mask
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_output_get(yt_unit_t unit, yt_port_mask_t *pport_mask)
{
    uint32_t ret;
    uint32_t regVal;
    uint32_t valMask = 0x3ff;
    yt_port_mask_t macmask;
    
    ret = HAL_MEM_DIRECT_READ(unit, LED_PARALLEL_OUTPUT_CTRL, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }
    
    macmask.portbits[0] = regVal &= valMask;
    CAL_MLIST_TO_YTPLIST(unit, macmask, (*pport_mask));
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_remapping_set
 * @endinternal
 *
 * @brief         select the remapping state of parallel LED(dst-->src)
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[in]     dstInfo             -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_remapping_set(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t dstInfo)
{
    uint32_t ret;
    yt_macid_t srcMacid;
    yt_macid_t dstMacid;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t valMask;

    if ((srcInfo.ledId < LED_ID_0) || (LED_ID_2 < srcInfo.ledId) || (dstInfo.ledId < LED_ID_0) || (LED_ID_2 < dstInfo.ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    srcMacid = CAL_YTP_TO_MAC(unit,srcInfo.port);
    regAddr = LED_PARALLEL_REMAPPING_BASE + srcMacid * 4;
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    /* port num */
    dstMacid = CAL_YTP_TO_MAC(unit, dstInfo.port);
    valMask = 0xfUL << (srcInfo.ledId * 6 + 2);
    regVal &= (~valMask);
    regVal |= ((dstMacid & 0xf) << (srcInfo.ledId * 6 + 2));

    /* LED ID */
    valMask = 0x3UL << (srcInfo.ledId * 6);
    regVal &= (~valMask);
    regVal |= ((dstInfo.ledId & 0x3) << (srcInfo.ledId * 6));

    HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_remapping_get
 * @endinternal
 *
 * @brief         get the remapping state of parallel LED
 * @param[in]     unit                -unit id
 * @param[in]     srcInfo             -the source information of remapping
 * @param[out]    pDstInfo            -the destination information of remapping
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_remapping_get(yt_unit_t unit, yt_led_remapping_t srcInfo, yt_led_remapping_t *pDstInfo)
{
    uint32_t ret;
    yt_macid_t srcMacid;
    yt_macid_t dstMacid;
    uint32_t regAddr;
    uint32_t regVal;

    if ((srcInfo.ledId < LED_ID_0) || (LED_ID_2 < srcInfo.ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    srcMacid = CAL_YTP_TO_MAC(unit, srcInfo.port);
    regAddr = LED_PARALLEL_REMAPPING_BASE + srcMacid * 4;
    
    ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

    /* port num */
    dstMacid = (regVal >> (srcInfo.ledId * 6 + 2)) & 0xf;
    CAL_MAC_TO_YTP(unit, dstMacid, pDstInfo->port);
    
    /* LED ID */
    pDstInfo->ledId = (regVal >> (srcInfo.ledId * 6)) & 0x3;
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_pos_invert_set
 * @endinternal
 *
 * @brief         invert led_pos signal for parallel mode
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     enable              -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_pos_invert_set(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t enable)
{
	uint32_t ret;
	yt_macid_t macid;
    uint32_t regAddr;
    uint32_t regVal;
	uint32_t valMask;

    if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
    {
        return CMM_ERR_INPUT;
    }

	macid = CAL_YTP_TO_MAC(unit,port);
	valMask = (0x1UL << (ledId * 10 + macid));
	
	regAddr = LED_PARALLEL_POS_INVERT_CTRL;
	ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
    if (CMM_ERR_OK != ret)
    {
        return CMM_ERR_FAIL;
    }

	if (enable)
	{
		regVal |= valMask;
	}
	else
	{
		regVal &= (~valMask);
	}
	HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    
    return CMM_ERR_OK;
}

/**
 * @internal      fal_tiger_led_parallel_pos_invert_get
 * @endinternal
 *
 * @brief         get inverted status of led_pos signal
 * @param[in]     unit                -unit id
 * @param[in]     port                -port num
 * @param[in]     ledId               -led id
 * @param[in]     pEnable             -enable or disable
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_parallel_pos_invert_get(yt_unit_t unit, yt_port_t port, yt_led_id_t ledId, yt_enable_t *pEnable)
{
	uint32_t ret;
	yt_macid_t macid;
	uint32_t regAddr;
	uint32_t regVal;
	uint32_t valMask;

	if ((ledId < LED_ID_0) || (LED_ID_2 < ledId))
	{
		return CMM_ERR_INPUT;
	}

	macid = CAL_YTP_TO_MAC(unit,port);
	valMask = (0x1UL << (ledId * 10 + macid));

	regAddr = LED_PARALLEL_POS_INVERT_CTRL;
	ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
	if (CMM_ERR_OK != ret)
	{
		return CMM_ERR_FAIL;
	}
	*pEnable = (regVal & valMask) ? YT_ENABLE : YT_DISABLE;
	
	return CMM_ERR_OK;
}
