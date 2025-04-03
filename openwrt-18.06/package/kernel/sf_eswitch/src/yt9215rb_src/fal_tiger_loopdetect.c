/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_loopdetect.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

yt_ret_t fal_tiger_loop_detect_enable_set(yt_unit_t unit, yt_enable_t enable)
{
    loop_detect_top_ctrl_t entry;
    uint32_t regData;
    cmm_err_t ret = CMM_ERR_OK;

    if(enable == YT_ENABLE)
    {
        /*enable loopdetect strap*/
        HAL_MEM_DIRECT_READ(unit, 0x80328, &regData);
        if(regData&(1<<7))/*dis lpd strap on*/
        {
            HAL_MEM_DIRECT_READ(unit, 0x80320, &regData);
            regData |= (1 << 7);
            HAL_MEM_DIRECT_WRITE(unit, 0x80320, regData);
            HAL_MEM_DIRECT_READ(unit, 0x80324, &regData);
            regData &=~ (1 << 7);
            HAL_MEM_DIRECT_WRITE(unit, 0x80324, regData);
        }
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_enable_get(yt_unit_t unit, yt_enable_t *pEnable)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t enable;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_ENf, &entry, &enable);
    *pEnable = enable ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_tpid_set(yt_unit_t unit, yt_tpid_t tpid)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_LOOP_DETECT_TPIDf, &entry, tpid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_tpid_get(yt_unit_t unit, yt_tpid_t *pTpid)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t tpid;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_LOOP_DETECT_TPIDf, &entry, &tpid);
    *pTpid = tpid;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_generate_way_set(yt_unit_t unit, yt_generate_way_t way)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_GENERATE_WAYf, &entry, way);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_generate_way_get(yt_unit_t unit, yt_generate_way_t *pWay)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t way;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_GENERATE_WAYf, &entry, &way);
    *pWay = way;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_unitID_set(yt_unit_t unit, yt_local_id_t localID, yt_remote_id_t remoteID)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_LOCALf, &entry, localID);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_REMOTE0f, &entry, remoteID.remoteID[0]);
    HAL_FIELD_SET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_REMOTE1f, &entry, remoteID.remoteID[1]);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_loop_detect_unitID_get(yt_unit_t unit, yt_local_id_t *pLocalID, yt_remote_id_t *pRemoteID)
{
    loop_detect_top_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t localID;
    uint32_t remoteID0;
    uint32_t remoteID1;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, LOOP_DETECT_TOP_CTRLm, 0, sizeof(loop_detect_top_ctrl_t), &entry), ret);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_LOCALf, &entry, &localID);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_REMOTE0f, &entry, &remoteID0);
    HAL_FIELD_GET(LOOP_DETECT_TOP_CTRLm, LOOP_DETECT_TOP_CTRL_UNIT_ID_REMOTE1f, &entry, &remoteID1);
    *pLocalID = localID;
    pRemoteID->remoteID[0] = remoteID0;
    pRemoteID->remoteID[1] = remoteID1;

    return CMM_ERR_OK;
}
