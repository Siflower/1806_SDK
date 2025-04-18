/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_oam.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

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

yt_ret_t fal_tiger_oam_en_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    oam_en_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t enable_mask;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_EN_CTRLm, 0, sizeof(oam_en_ctrl_t), &entry), ret);
    HAL_FIELD_GET(OAM_EN_CTRLm, OAM_EN_CTRL_ENf, &entry, &enable_mask);
    if (enable == YT_ENABLE)
    {
        enable_mask |= (1 << macid);
    }
    else
    {
        enable_mask &= ~(1 << macid);
    }
    HAL_FIELD_SET(OAM_EN_CTRLm, OAM_EN_CTRL_ENf, &entry, enable_mask);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, OAM_EN_CTRLm, 0, sizeof(oam_en_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_en_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    oam_en_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t enable_mask;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_EN_CTRLm, 0, sizeof(oam_en_ctrl_t), &entry), ret);
    HAL_FIELD_GET(OAM_EN_CTRLm, OAM_EN_CTRL_ENf, &entry, &enable_mask);
    *pEnable = IS_BIT_SET(enable_mask, macid) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_bypass_accessctrl_set(yt_unit_t unit, yt_enable_t enable)
{
    oam_par_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);
    HAL_FIELD_SET(OAM_PAR_ACTm, OAM_PAR_ACT_BYPASS_IM_AC_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_bypass_accessctrl_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    oam_par_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);
    HAL_FIELD_GET(OAM_PAR_ACTm, OAM_PAR_ACT_BYPASS_IM_AC_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_parser_act_set(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t action)
{
    oam_par_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t par_act;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);
    HAL_FIELD_GET(OAM_PAR_ACTm, OAM_PAR_ACT_PAR_ACTf, &entry, &par_act);
    par_act &= ~(0x3 << (macid * 2));
    par_act |= (action << (macid * 2));
    HAL_FIELD_SET(OAM_PAR_ACTm, OAM_PAR_ACT_PAR_ACTf, &entry, par_act);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_parser_act_get(yt_unit_t unit, yt_port_t port, yt_oam_parser_action_t *paction)
{
    oam_par_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t par_act;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_PAR_ACTm, 0, sizeof(oam_par_act_t), &entry), ret);
    HAL_FIELD_GET(OAM_PAR_ACTm, OAM_PAR_ACT_PAR_ACTf, &entry, &par_act);
    *paction = (par_act >> (macid * 2)) & 0x3;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_mux_act_set(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t action)
{
    oam_mux_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t mux_act;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_MUX_ACTm, 0, sizeof(oam_mux_act_t), &entry), ret);
    HAL_FIELD_GET(OAM_MUX_ACTm, OAM_MUX_ACT_MUX_ACTf, &entry, &mux_act);
    mux_act &= ~(0x3 << (macid * 2));
    mux_act |= (action << (macid * 2));
    HAL_FIELD_SET(OAM_MUX_ACTm, OAM_MUX_ACT_MUX_ACTf, &entry, mux_act);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, OAM_MUX_ACTm, 0, sizeof(oam_mux_act_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_oam_mux_act_get(yt_unit_t unit, yt_port_t port, yt_oam_mux_action_t *paction)
{
    oam_mux_act_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macid;
    uint32_t mux_act;

    macid = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, OAM_MUX_ACTm, 0, sizeof(oam_mux_act_t), &entry), ret);
    HAL_FIELD_GET(OAM_MUX_ACTm, OAM_MUX_ACT_MUX_ACTf, &entry, &mux_act);
    *paction = (mux_act >> (macid * 2)) & 0x3;

    return CMM_ERR_OK;
}
