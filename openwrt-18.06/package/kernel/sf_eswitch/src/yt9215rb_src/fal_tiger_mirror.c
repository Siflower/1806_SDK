/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_mirror.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"
#include "fal_tiger_qos.h"

yt_ret_t fal_tiger_mirror_init(yt_unit_t unit)
{
    mirror_ctrl_t mirrorCtrl;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_CTRLm, 0, sizeof(mirror_ctrl_t), &mirrorCtrl), ret);
    HAL_FIELD_SET(MIRROR_CTRLm, MIRROR_CTRL_MIRROR_PORTf, &mirrorCtrl, YT_MIRROR_INVALID_PORT);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MIRROR_CTRLm, 0, sizeof(mirror_ctrl_t), &mirrorCtrl), ret);

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_que_colorAware_enable_set(yt_unit_t unit, yt_macid_t macId, yt_enable_t enable)
{
    uint32_t regAddr;
    uint32_t regVal;
    uint32_t regVal2;
    uint32_t qid;
    cmm_err_t ret = CMM_ERR_OK;

    for (qid = 0; qid < CAL_MAX_UCAST_QUEUE_NUM(unit); qid++)
    {
        regAddr = QOS_FORCEAC_UCASTQUE_REG(unit, macId, qid);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regVal2), ret);
        regAddr += 4;
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regVal), ret);
        if (enable)
        {
            SET_BIT(regVal, 27);
        }
        else
        {
            CLEAR_BIT(regVal, 27);
        }
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr - 4, regVal2), ret);
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal), ret);
    }

    for (qid = 0; qid < CAL_MAX_MCAST_QUEUE_NUM(unit); qid++)
    {
        regAddr = QOS_FORCEAC_MCASTQUE_REG(unit, macId, qid);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regAddr, &regVal), ret);
        if (enable)
        {
            SET_BIT(regVal, 28);
        }
        else
        {
            CLEAR_BIT(regVal, 28);
        }
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regAddr, regVal), ret);
    }

    return CMM_ERR_OK;
}

static uint32_t fal_tiger_mirror_group_chk(yt_unit_t unit, yt_mirror_group_t grpId, yt_mirror_entry_t *pMirrorEntry)
{
    yt_ret_t ret = CMM_ERR_OK;
    yt_mirror_entry_t entry;
    yt_bool_t isEnable = FALSE;

    entry.flags = YT_MIRROR_FLAG_ALL;
    CMM_ERR_CHK(fal_tiger_mirror_group_get(unit, grpId, &entry), ret);

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_MIRROR_PORT)
    {
        CMM_PARAM_CHK((TRUE == pMirrorEntry->isLag), CMM_ERR_NOT_SUPPORT);
        if (pMirrorEntry->mirrorPort == YT_MIRROR_INVALID_PORT)
        {
            entry.mirrorPort = YT_MIRROR_INVALID_PORT;
        }
        else
        {
            CMM_PARAM_CHK((!(CMM_PORT_VALID(unit, pMirrorEntry->mirrorPort))), CMM_ERR_PORT);
            entry.mirrorPort = pMirrorEntry->mirrorPort;
        }
    }

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_INGRESS)
    {
        CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pMirrorEntry->rxPortMask))), CMM_ERR_PORTLIST);
        entry.rxPortMask = pMirrorEntry->rxPortMask;
    }
    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_EGRESS)
    {
        CMM_PARAM_CHK((!(CMM_PLIST_VALID(unit, pMirrorEntry->txPortMask))), CMM_ERR_PORTLIST);
        entry.txPortMask = pMirrorEntry->txPortMask;
    }

    if (entry.mirrorPort != YT_MIRROR_INVALID_PORT)
    {
        /* rx/tx port shouldn't contain mirror port itself */
        CMM_IS_MEMBER_PORT(entry.rxPortMask, entry.mirrorPort, isEnable);
        CMM_PARAM_CHK((TRUE == isEnable), CMM_ERR_PORTLIST);
        CMM_IS_MEMBER_PORT(entry.txPortMask, entry.mirrorPort, isEnable);
        CMM_PARAM_CHK((TRUE == isEnable), CMM_ERR_PORTLIST);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_mirror_group_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_mirror_entry_t *pMirrorEntry)
{
    mirror_ctrl_t mirrorCtrl;
    yt_macid_t macId;
    yt_macid_t orgMacid;
    yt_port_mask_t macMsk;
    cmm_err_t ret = CMM_ERR_OK;

    /* parameter check */
    CMM_ERR_CHK(fal_tiger_mirror_group_chk(unit, grpId, pMirrorEntry), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_CTRLm, 0, sizeof(mirror_ctrl_t), &mirrorCtrl), ret);
    if (pMirrorEntry->flags & YT_MIRROR_FLAG_MIRROR_PORT)
    {
        HAL_FIELD_GET(MIRROR_CTRLm, MIRROR_CTRL_MIRROR_PORTf, &mirrorCtrl, &orgMacid);
        /* disable port mirror */
        if (pMirrorEntry->mirrorPort == YT_MIRROR_INVALID_PORT)
        {
            HAL_FIELD_SET(MIRROR_CTRLm, MIRROR_CTRL_MIRROR_PORTf, &mirrorCtrl, YT_MIRROR_INVALID_PORT);
        }
        else
        {
            macId = CAL_YTP_TO_MAC(unit, pMirrorEntry->mirrorPort);
            HAL_FIELD_SET(MIRROR_CTRLm, MIRROR_CTRL_MIRROR_PORTf, &mirrorCtrl, macId);
            fal_tiger_que_colorAware_enable_set(unit, macId, YT_DISABLE);
        }
        if (orgMacid != YT_MIRROR_INVALID_PORT)
        {
            fal_tiger_que_colorAware_enable_set(unit, orgMacid, YT_ENABLE);
        }
    }

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_INGRESS)
    {
        CAL_YTPLIST_TO_MLIST(unit, pMirrorEntry->rxPortMask, macMsk);
        HAL_FIELD_SET(MIRROR_CTRLm, MIRROR_CTRL_INGR_MIRROR_ENf, &mirrorCtrl, macMsk.portbits[0]);
    }

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_EGRESS)
    {
        CAL_YTPLIST_TO_MLIST(unit, pMirrorEntry->txPortMask, macMsk);
        HAL_FIELD_SET(MIRROR_CTRLm, MIRROR_CTRL_EGR_MIRROR_ENf, &mirrorCtrl, macMsk.portbits[0]);
    }

    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MIRROR_CTRLm, 0, sizeof(mirror_ctrl_t), &mirrorCtrl), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_mirror_group_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_mirror_entry_t *pMirrorEntry)
{
    mirror_ctrl_t mirrorCtrl;
    yt_port_mask_t macMsk;
    uint32_t macId;
    uint32_t igrPortMsk;
    uint32_t egrPortMsk;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_UNUSED_PARAM(grpId);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_CTRLm, 0, sizeof(mirror_ctrl_t), &mirrorCtrl), ret);
    if (pMirrorEntry->flags & YT_MIRROR_FLAG_MIRROR_PORT)
    {
        HAL_FIELD_GET(MIRROR_CTRLm, MIRROR_CTRL_MIRROR_PORTf, &mirrorCtrl, &macId);
        if (macId == YT_MIRROR_INVALID_PORT)
        {
            pMirrorEntry->mirrorPort = YT_MIRROR_INVALID_PORT;
        }
        else
        {
            CAL_MAC_TO_YTP(unit, macId, pMirrorEntry->mirrorPort);
        }
        pMirrorEntry->isLag = FALSE;
    }

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_INGRESS)
    {
        CMM_CLEAR_MEMBER_PORT(macMsk);
        HAL_FIELD_GET(MIRROR_CTRLm, MIRROR_CTRL_INGR_MIRROR_ENf, &mirrorCtrl, &igrPortMsk);
        macMsk.portbits[0] = igrPortMsk;
        CAL_MLIST_TO_YTPLIST(unit, macMsk, pMirrorEntry->rxPortMask);
    }

    if (pMirrorEntry->flags & YT_MIRROR_FLAG_PORT_EGRESS)
    {
        CMM_CLEAR_MEMBER_PORT(macMsk);
        HAL_FIELD_GET(MIRROR_CTRLm, MIRROR_CTRL_EGR_MIRROR_ENf, &mirrorCtrl, &egrPortMsk);
        macMsk.portbits[0] = egrPortMsk;
        CAL_MLIST_TO_YTPLIST(unit, macMsk, pMirrorEntry->txPortMask);
    }

    return CMM_ERR_OK;
}
