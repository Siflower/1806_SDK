/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_led.h"
#include "fal_tiger_sys.h"
#include "hal_mem.h"


/* remapping array for LED_DATA */
static const yt_led_slot_t remapData[SLED_DATANUM_MAX][30] = 
{
    /* SLED_DATANUM_YT9215_P5L1 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0}
    },

    /* SLED_DATANUM_YT9215_P7L1 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}
    },

    /* SLED_DATANUM_YT9215_P5L2 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0}, 
        {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}
    },

    /* SLED_DATANUM_YT9215_P7L2 */    
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0},
        {9, LED_ID_1}, {8, LED_ID_1}, {4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1}
    },
    
    /* SLED_DATANUM_YT9215_P5L3 */
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0},
        {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0}, 
        {9, LED_ID_1}, {8, LED_ID_1}, {7, LED_ID_1}, {6, LED_ID_1}, {5, LED_ID_1}
    },

    /* SLED_DATANUM_YT9215_P7L3 */
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0},
        {9, LED_ID_1}, {8, LED_ID_1}, {4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1},
        {9, LED_ID_2}, {8, LED_ID_2}, {4, LED_ID_2}, {3, LED_ID_2}, {2, LED_ID_2}, {1, LED_ID_2}, {0, LED_ID_2}
    },

    /* SLED_DATANUM_YT9218_P10L1 */    
    {
        {9, LED_ID_2}, {8, LED_ID_2}, {7, LED_ID_2}, {6, LED_ID_2}, {5, LED_ID_2},{4, LED_ID_2}, {3, LED_ID_2}, {2, LED_ID_2}, {1, LED_ID_2}, {0, LED_ID_2}
    },

    /* SLED_DATANUM_YT9218_P10L2 */
    {
        {9, LED_ID_2}, {8, LED_ID_2}, {7, LED_ID_2}, {6, LED_ID_2}, {5, LED_ID_2},{4, LED_ID_2}, {3, LED_ID_2}, {2, LED_ID_2}, {1, LED_ID_2}, {0, LED_ID_2},
        {9, LED_ID_1}, {8, LED_ID_1}, {7, LED_ID_1}, {6, LED_ID_1}, {5, LED_ID_1},{4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1}
    },

    /* SLED_DATANUM_YT9218_P10L3 */
    {
        {9, LED_ID_0}, {8, LED_ID_0}, {7, LED_ID_0}, {6, LED_ID_0}, {5, LED_ID_0},{4, LED_ID_0}, {3, LED_ID_0}, {2, LED_ID_0}, {1, LED_ID_0}, {0, LED_ID_0},
        {9, LED_ID_1}, {8, LED_ID_1}, {7, LED_ID_1}, {6, LED_ID_1}, {5, LED_ID_1},{4, LED_ID_1}, {3, LED_ID_1}, {2, LED_ID_1}, {1, LED_ID_1}, {0, LED_ID_1},
        {9, LED_ID_2}, {8, LED_ID_2}, {7, LED_ID_2}, {6, LED_ID_2}, {5, LED_ID_2},{4, LED_ID_2}, {3, LED_ID_2}, {2, LED_ID_2}, {1, LED_ID_2}, {0, LED_ID_2}
     }
};

static const uint8_t ledMaxNum[SLED_DATANUM_MAX] = {5, 7, 10, 14, 15, 21, 10, 20, 30};

static yt_sled_dataNum_t sledDataNum = SLED_DATANUM_YT9215_P7L3;

static uint8_t sledRemapChgFlag = 0;

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
    uint32_t ret;
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t port;
    uint8_t portNum;
    uint8_t ledNum;
    yt_led_mode_t mode;
    uint8_t ledIndex;
    yt_switch_chip_t chip = 0;

    /* active mode */
    CMM_ERR_CHK(fal_tiger_led_mode_get(unit, &mode), ret);

    /* get chip info */
    CMM_ERR_CHK(fal_tiger_sys_chipInfo_get(unit, &chip), ret);

    /* serial mode */
    if (LED_MODE_SERIAL == mode)
    {
        CMM_ERR_CHK(fal_tiger_led_serial_activeMode_set(unit, LED_SERIAL_ACTIVE_MODE_LOW), ret);
        
        switch(sledDataNum)
        {
            case SLED_DATANUM_YT9215_P5L1:
                portNum = 5;
                ledNum = 1;
                break;

            case SLED_DATANUM_YT9215_P7L1:
                portNum = 7;
                ledNum = 1;
                break;

           case SLED_DATANUM_YT9215_P5L2:
                portNum = 5;
                ledNum = 2;
                break;

            case SLED_DATANUM_YT9215_P7L2:
                portNum = 7;
                ledNum = 2;
                break;
                
            case SLED_DATANUM_YT9215_P5L3:
                portNum = 5;
                ledNum = 3;
                break;
                
            case SLED_DATANUM_YT9215_P7L3:
                portNum = 7;
                ledNum = 3;
                break;

            case SLED_DATANUM_YT9218_P10L1:
                portNum = 10;
                ledNum = 1;
                break;

            case SLED_DATANUM_YT9218_P10L2:
                portNum = 10;
                ledNum = 2;
                break;

            case SLED_DATANUM_YT9218_P10L3:
                portNum = 10;
                ledNum = 3;
                break;

            default:
                return CMM_ERR_NOT_SUPPORT;
        }

        /* serial port num-- start */
        ret = HAL_MEM_DIRECT_READ(unit, LED_GLB_CTRL, &regVal);
        if (CMM_ERR_OK != ret)
        {
            return CMM_ERR_FAIL;
        }
        regVal &= 0xfffe1fff;
        regVal |= ((portNum & 0xf) << 13);
        HAL_MEM_DIRECT_WRITE(unit, LED_GLB_CTRL, regVal);
        /* serial port num-- end */
        
        /* serial pin num-- start */
        ret = HAL_MEM_DIRECT_READ(unit, LED_SERIAL_CTRL, &regVal);
        if (CMM_ERR_OK != ret)
        {
            return CMM_ERR_FAIL;
        }
        regVal &= 0xfffffffc;
        regVal |= ((ledNum - 1) & 0x3);
        HAL_MEM_DIRECT_WRITE(unit, LED_SERIAL_CTRL, regVal);
        /* serial pin num-- end */

        /* set default remapping table */
        yt_led_remapping_t yt9215DefaultRemapInfo[21] = 
        {
            {6, 1}, {4, 0}, {5, 1}, {3, 0}, {2, 0}, {1, 0}, {0, 0},
            {6, 0}, {5, 0}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1},
            {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}
        };
        yt_led_remapping_t yt9215scRemapInfo[21] = 
        {
            {3, 0}, {2, 0}, {1, 0}, {0, 0},{6, 1}, {5, 1}, {4, 1}, 
            {3, 1}, {2, 1}, {1, 1}, {0, 1},{6, 2}, {5, 2}, {4, 2}, 
            {3, 2}, {2, 2}, {1, 2}, {0, 2}, {6, 0}, {5, 0}, {4, 0}
        };
        if ((SLED_DATANUM_YT9215_P7L3 == sledDataNum) && (sledRemapChgFlag == 0))
        {
            for (ledIndex=0; ledIndex<21; ledIndex++)
            {
                CMM_ERR_CHK(yt_led_serial_remapping_set(unit, ledIndex, (SWCHIP_YT9215SC == chip)?yt9215scRemapInfo[ledIndex]:yt9215DefaultRemapInfo[ledIndex]), ret);
            }
        }
    }
    else
    {
        yt_port_mask_t portmask;

        CMM_CLEAR_MEMBER_PORT(portmask);

        if (SWCHIP_YT9214NB == chip)
        {
            CMM_SET_MEMBER_PORT(portmask, 0);
            CMM_SET_MEMBER_PORT(portmask, 1);
            CMM_SET_MEMBER_PORT(portmask, 2);
            CMM_SET_MEMBER_PORT(portmask, 3);
            CMM_SET_MEMBER_PORT(portmask, 4);
            CMM_SET_MEMBER_PORT(portmask, 8);
            CMM_SET_MEMBER_PORT(portmask, 9);
            fal_tiger_led_parallel_output_set(unit, portmask);
        }
        else if (SWCHIP_YT9213NB == chip)
        {
            /* remapping led port */
            HAL_MEM_DIRECT_WRITE(unit, (LED_PARALLEL_REMAPPING_BASE + 2*4), 0xe34c);
            HAL_MEM_DIRECT_WRITE(unit, (LED_PARALLEL_REMAPPING_BASE + 3*4), 0xa248);
            HAL_MEM_DIRECT_WRITE(unit, (LED_PARALLEL_REMAPPING_BASE + 8*4), 0x2284e);
            HAL_MEM_DIRECT_WRITE(unit, LED_PARALLEL_OUTPUT_CTRL, 0x3ff);
        }
        else
        {
        }
    }

    /* preset disable_link_try bit of LED0 action(exclude CPU_PORT) */
    for (port = 0; port < FAL_MAX_PORT_NUM - 1; port++)
    {
        regAddr = LED_CTRL_0_BASE + port * 4;
        ret = HAL_MEM_DIRECT_READ(unit, regAddr, &regVal);
        if (CMM_ERR_OK != ret)
        {
            return CMM_ERR_FAIL;
        }

        regVal |= 0x20000;
        HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal);
    }

    /* set config done */
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
    yt_switch_chip_t chip = 0;

    fal_tiger_sys_chipInfo_get(unit, &chip);
    if (SWCHIP_YT9218N != chip)
    {
        if ((mode != LED_MODE_PARALLEL) && (mode != LED_MODE_SERIAL))
        {
            return CMM_ERR_INPUT;
        }
    }

    if (mode == LED_MODE_SERIAL)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, LED_PARALLEL_OUTPUT_CTRL, 0U), ret);
    }
    else
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, LED_PARALLEL_OUTPUT_CTRL, 0x3ffU), ret);
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

    osal_memset(pLedActCfg, sizeof(yt_led_act_cfg_t), 0, sizeof(yt_led_act_cfg_t));

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

yt_ret_t fal_tiger_led_serial_outputMode_set(yt_unit_t unit, yt_sled_dataNum_t mode)
{
    CMM_UNUSED_PARAM(unit);
    sledDataNum = mode;

    return CMM_ERR_OK;   
}

yt_ret_t fal_tiger_led_serial_outputMode_get(yt_unit_t unit, yt_sled_dataNum_t *pMode)
{
    CMM_UNUSED_PARAM(unit);
    *pMode = sledDataNum;

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
    yt_sled_dataNum_t dataNum = sledDataNum;

    if ((dstInfo.ledId < LED_ID_0) || (LED_ID_2 < dstInfo.ledId))
    {
        return CMM_ERR_INPUT;
    }
    
    sledRemapChgFlag = 1;

    /* remove led from boardprofile, led owner todo */ 
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
    yt_sled_dataNum_t dataNum = sledDataNum;

    /* remove led from boardprofile, led owner todo */ 
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

/**
 * @internal      fal_tiger_led_serial_port_info_set
 * @endinternal
 *
 * @brief         get per_port_en,is_combo_en,ledmode
 * @param[in]     unit                -unit id
 * @param[out]    portledInfo         -set per_port_en,is_combo_en,ledmode info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_port_info_set(yt_unit_t unit, yt_led_seled_info_t portledInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(portledInfo);
    
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_led_serial_port_info_get
 * @endinternal
 *
 * @brief         set per_port_en,is_combo_en,ledmode
 * @param[in]     unit                -unit id
 * @param[out]    pPortledInfo        -get per_port_en,is_combo_en,ledmode info
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_port_info_get(yt_unit_t unit, yt_led_seled_info_t *pPortledInfo)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pPortledInfo);
    
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_tiger_led_serial_total_ledNum_set
 * @endinternal
 *
 * @brief        set total led number
 * @param[in]     unit                -unit id
 * @param[in]     lednum              -total led num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_total_ledNum_set(yt_unit_t unit, yt_unit_t lednum)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(lednum);
    
    return CMM_ERR_NOT_SUPPORT;
}

/**
 * @internal      fal_shark_led_serial_total_ledNum_get
 * @endinternal
 *
 * @brief        get total led number
 * @param[in]     unit                -unit id
 * @param[in]     lednum              -total led num
 * @retval        CMM_ERR_OK          -on success
 * @retval        CMM_ERR_FAIL        -on fail
 */
yt_ret_t fal_tiger_led_serial_total_ledNum_get(yt_unit_t unit, yt_unit_t *pLednum)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(pLednum);
    
    return CMM_ERR_NOT_SUPPORT;
}

