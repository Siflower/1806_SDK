/*
 * Include Files
 */
#include "yt_error.h"
#include "osal_mem.h"
#include "osal_print.h"
#include "fal_tiger_qos.h"
#include "fal_cmm.h"
#include "hal_mem.h"
#include "fal_tiger_struct.h"
#include "fal_tiger_entry.h"
#include "fal_tiger_mem.h"

static yt_ret_t fal_tiger_qos_ac_queue_flush(yt_unit_t unit, yt_qid_t qinfo);
static yt_ret_t fal_tiger_qos_ac_enqueue_disable(yt_unit_t unit, yt_qid_t qinfo, uint8_t disable);

yt_ret_t fal_tiger_qos_init(yt_unit_t unit)
{
    egr_port_ctrln_t entry;
    yt_macid_t macId;
    yt_ret_t ret;

    for (macId = 0; macId < FAL_MAX_PORT_NUM; macId++)
    {
	    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macId, sizeof(egr_port_ctrln_t), &entry), ret);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CPRIO_REMARK_SELf, &entry, YT_CPRI_REMARK_INDEX);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_SPRIO_REMARK_SELf, &entry, YT_SPRI_REMARK_INDEX);
        CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_CTRLNm, macId, sizeof(egr_port_ctrln_t), &entry), ret);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_sel_precedence_set(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl)
{
    qos_merge_precedence_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit,port);

    if (pPriTbl->flags & (YT_QOS_SEL_PRECEDENCE_FLAG_INTCPRI | YT_QOS_SEL_PRECEDENCE_FLAG_INTSPRI))
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QOS_MERGE_PRECEDENCE_CTRLNm, macId, sizeof(qos_merge_precedence_ctrln_t), &entry), ret);
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_SMAC)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->smacPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_MAC_SA_INT_PRI_PRECEDENCEf, &entry, pPriTbl->smacPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_DMAC)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->dmacPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_MAC_DA_INT_PRI_PRECEDENCEf, &entry, pPriTbl->dmacPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_VLAN)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->vlanPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_VLAN_INT_PRI_PRECEDENCEf, &entry, pPriTbl->vlanPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_ACL)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->aclPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_ACL_INT_PRI_PRECEDENCEf, &entry, pPriTbl->aclPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_DSCP)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->dscpPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_DSCP2INT_PRI_PRECEDENCEf, &entry, pPriTbl->dscpPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_CPRI)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->cpri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_CPRI2INT_PRI_PRECEDENCEf, &entry, pPriTbl->cpri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_SPRI)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->spri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_SPRI2INT_PRI_PRECEDENCEf, &entry, pPriTbl->spri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_PORT)
    {
        CMM_PARAM_CHK(CAL_MAX_VALUE_INT_PRI(unit) < pPriTbl->portPri, CMM_ERR_INPUT);
        HAL_FIELD_SET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_PORT_PRI_PRECEDENCEf, &entry, pPriTbl->portPri);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QOS_MERGE_PRECEDENCE_CTRLNm, macId,sizeof(qos_merge_precedence_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_sel_precedence_get(yt_unit_t unit, yt_port_t port, yt_qos_intPri_sel_precedence_t *pPriTbl)
{
    qos_merge_precedence_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit,port);

    if (pPriTbl->flags & (YT_QOS_SEL_PRECEDENCE_FLAG_INTCPRI | YT_QOS_SEL_PRECEDENCE_FLAG_INTSPRI))
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QOS_MERGE_PRECEDENCE_CTRLNm, macId, sizeof(qos_merge_precedence_ctrln_t), &entry), ret);
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_SMAC)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_MAC_SA_INT_PRI_PRECEDENCEf, &entry, &pPriTbl->smacPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_DMAC)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_MAC_DA_INT_PRI_PRECEDENCEf, &entry, &pPriTbl->dmacPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_VLAN)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_VLAN_INT_PRI_PRECEDENCEf, &entry, &pPriTbl->vlanPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_ACL)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_ACL_INT_PRI_PRECEDENCEf, &entry, &pPriTbl->aclPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_DSCP)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_DSCP2INT_PRI_PRECEDENCEf, &entry, &pPriTbl->dscpPri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_CPRI)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_CPRI2INT_PRI_PRECEDENCEf, &entry, &pPriTbl->cpri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_SPRI)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_SPRI2INT_PRI_PRECEDENCEf, &entry, &pPriTbl->spri);
    }
    if (pPriTbl->flags & YT_QOS_SEL_PRECEDENCE_FLAG_PORT)
    {
        HAL_FIELD_GET(QOS_MERGE_PRECEDENCE_CTRLNm, QOS_MERGE_PRECEDENCE_CTRLN_PORT_PRI_PRECEDENCEf, &entry, &pPriTbl->portPri);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_portDefPri_set (yt_unit_t unit, yt_port_t port, yt_enable_t enable, yt_pri_t pri)
{
    qos_port_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit,port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QOS_PORT_CTRLNm, macId,sizeof(qos_port_ctrln_t), &entry), ret);
    HAL_FIELD_SET(QOS_PORT_CTRLNm, QOS_PORT_CTRLN_PORT_INTPRIf, &entry, pri);
    HAL_FIELD_SET(QOS_PORT_CTRLNm, QOS_PORT_CTRLN_PORT_INTPRI_ENf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QOS_PORT_CTRLNm, macId,sizeof(qos_port_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_qos_intPri_portDefPri_get (yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    qos_port_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit,port);
    uint32_t enable;
    uint32_t pri;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QOS_PORT_CTRLNm, macId,sizeof(qos_port_ctrln_t), &entry), ret);
    HAL_FIELD_GET(QOS_PORT_CTRLNm, QOS_PORT_CTRLN_PORT_INTPRIf, &entry, &pri);
    HAL_FIELD_GET(QOS_PORT_CTRLNm, QOS_PORT_CTRLN_PORT_INTPRI_ENf, &entry, &enable);
    *pEnable = enable?YT_ENABLE:YT_DISABLE;
    *pPri = pri;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_intCpri_map_set(yt_unit_t unit, yt_pri_t cpri, yt_pri_t pri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(cpri);
    CMM_UNUSED_PARAM(pri);

    return CMM_ERR_NOT_SUPPORT;
}
yt_ret_t fal_tiger_qos_intPri_intCpri_map_get(yt_unit_t unit, yt_pri_t cpri, yt_pri_t *pPri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(cpri);
    CMM_UNUSED_PARAM(pPri);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_qos_intPri_intSpri_map_set(yt_unit_t unit, yt_pri_t spri, yt_pri_t pri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(spri);
    CMM_UNUSED_PARAM(pri);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_qos_intPri_intSpri_map_get(yt_unit_t unit, yt_pri_t spri, yt_pri_t *pPri)
{
    CMM_UNUSED_PARAM(unit);
    CMM_UNUSED_PARAM(spri);
    CMM_UNUSED_PARAM(pPri);

    return CMM_ERR_NOT_SUPPORT;
}

yt_ret_t fal_tiger_qos_intPri_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t pri)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_CPRI_MAP_INDEX << 4) + (cpri.dei<<3) + cpri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_SET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_PRIOf, &priEntry, pri);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_pri_t *pPri)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_pri_index_t pri_map_sel;
    uint32_t index;
    uint32_t pri;

    pri_map_sel = YT_CPRI_MAP_INDEX;
    index = (pri_map_sel << 4) + (cpri.dei<<3) + cpri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_GET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_PRIOf, &priEntry, &pri);
    *pPri = pri;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t pri)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_SPRI_MAP_INDEX << 4) + (spri.dei<<3) + spri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_SET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_PRIOf, &priEntry, pri);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_pri_t *pPri)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_pri_index_t pri_map_sel;
    uint32_t index;
    uint32_t pri;

    pri_map_sel = YT_SPRI_MAP_INDEX;
    index = (pri_map_sel << 4) + (spri.dei<<3) + spri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_GET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_PRIOf, &priEntry, &pri);
    *pPri = pri;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t pri)
{
    dscp_to_int_prio_map_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);
    HAL_FIELD_SET(DSCP_TO_INT_PRIO_MAPm, DSCP_TO_INT_PRIO_MAP_INT_PRIOf, &entry, pri);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_pri_t *pPri)
{
    dscp_to_int_prio_map_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t pri;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);
    HAL_FIELD_GET(DSCP_TO_INT_PRIO_MAPm, DSCP_TO_INT_PRIO_MAP_INT_PRIOf, &entry, &pri);
    *pPri = pri;

    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_qos_intPri_vlan_map_set(yt_unit_t unit, yt_vlan_t vid, yt_enable_t enable, yt_pri_t pri)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_INT_PRI_VALIDf, &entry, enable);
    HAL_FIELD_SET(L2_VLAN_TBLm, L2_VLAN_TBL_INT_PRIf, &entry, pri);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, L2_VLAN_TBLm, vid, sizeof(l2_vlan_tbl_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t  fal_tiger_qos_intPri_vlan_map_get(yt_unit_t unit, yt_vlan_t vid, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    l2_vlan_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, L2_VLAN_TBLm, vid,sizeof(l2_vlan_tbl_t), &entry), ret);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_INT_PRI_VALIDf, &entry, pEnable);
    HAL_FIELD_GET(L2_VLAN_TBLm, L2_VLAN_TBL_INT_PRIf, &entry, pPri);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_map_igrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri)
{
    mirror_qos_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_UNUSED_PARAM(grpId);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_QOS_CTRLm, 0, sizeof(mirror_qos_ctrl_t), &entry), ret);
    HAL_FIELD_SET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_INGR_MIRROR_INT_PRIOf, &entry, pri);
    HAL_FIELD_SET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_INGR_MIRROR_INT_PRIO_VALIDf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MIRROR_QOS_CTRLm, 0, sizeof(mirror_qos_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_map_igrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    mirror_qos_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t pri;
    uint32_t enable;

    CMM_UNUSED_PARAM(grpId);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_QOS_CTRLm, 0, sizeof(mirror_qos_ctrl_t), &entry), ret);
    HAL_FIELD_GET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_INGR_MIRROR_INT_PRIOf, &entry, &pri);
    HAL_FIELD_GET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_INGR_MIRROR_INT_PRIO_VALIDf, &entry, &enable);
    *pPri = pri;
    *pEnable = enable;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_map_egrMirror_set(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t enable, yt_pri_t pri)
{
    mirror_qos_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_UNUSED_PARAM(grpId);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_QOS_CTRLm, 0,sizeof(mirror_qos_ctrl_t), &entry), ret);
    HAL_FIELD_SET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_EGR_MIRROR_INT_PRIOf, &entry, pri);
    HAL_FIELD_SET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_EGR_MIRROR_INT_PRIO_VALIDf, &entry, enable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, MIRROR_QOS_CTRLm, 0,sizeof(mirror_qos_ctrl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intPri_map_egrMirror_get(yt_unit_t unit, yt_mirror_group_t grpId, yt_enable_t *pEnable, yt_pri_t *pPri)
{
    mirror_qos_ctrl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t pri;
    uint32_t enable;

    CMM_UNUSED_PARAM(grpId);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, MIRROR_QOS_CTRLm, 0,sizeof(mirror_qos_ctrl_t), &entry), ret);
    HAL_FIELD_GET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_EGR_MIRROR_INT_PRIOf, &entry, &pri);
    HAL_FIELD_GET(MIRROR_QOS_CTRLm, MIRROR_QOS_CTRL_EGR_MIRROR_INT_PRIO_VALIDf, &entry, &enable);
    *pPri = pri;
    *pEnable = enable;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_cpri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t dp)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_CPRI_MAP_INDEX << 4) + (cpri.dei << 3) + cpri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index, sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_SET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_DPf, &priEntry, dp);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PRI_TO_INT_PRIO_MAPm, index, sizeof(pri_to_int_prio_map_t), &priEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_cpri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t cpri, yt_dp_t *pDp)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;
    uint32_t dp;

    index = (YT_CPRI_MAP_INDEX << 4) + (cpri.dei << 3) + cpri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_GET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_DPf, &priEntry, &dp);
    *pDp = dp;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_spri_map_set(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t dp)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_SPRI_MAP_INDEX << 4) + (spri.dei << 3) + spri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index, sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_SET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_DPf, &priEntry, dp);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, PRI_TO_INT_PRIO_MAPm, index, sizeof(pri_to_int_prio_map_t), &priEntry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_spri_map_get(yt_unit_t unit, yt_qos_pmap_tci_t spri, yt_dp_t *pDp)
{
    pri_to_int_prio_map_t priEntry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;
    uint32_t dp;

    index = (YT_SPRI_MAP_INDEX << 4) + (spri.dei << 3) + spri.prio;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, PRI_TO_INT_PRIO_MAPm, index,sizeof(pri_to_int_prio_map_t), &priEntry), ret);
    HAL_FIELD_GET(PRI_TO_INT_PRIO_MAPm, PRI_TO_INT_PRIO_MAP_INT_DPf, &priEntry, &dp);
	*pDp = dp;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_dscp_map_set(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t dp)
{
    dscp_to_int_prio_map_t entry;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);
    HAL_FIELD_SET(DSCP_TO_INT_PRIO_MAPm, DSCP_TO_INT_PRIO_MAP_INT_DPf, &entry, dp);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_intDP_dscp_map_get(yt_unit_t unit, yt_dscp_t dscp, yt_dp_t *pDp)
{
    dscp_to_int_prio_map_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t dp;

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, DSCP_TO_INT_PRIO_MAPm, dscp,sizeof(dscp_to_int_prio_map_t), &entry), ret);
    HAL_FIELD_GET(DSCP_TO_INT_PRIO_MAPm, DSCP_TO_INT_PRIO_MAP_INT_DPf, &entry, &dp);
    *pDp = dp;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_map_ucast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    int_prio_to_ucast_qid_mapn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INT_PRIO_TO_UCAST_QID_MAPNm, macId,sizeof(int_prio_to_ucast_qid_mapn_t), &entry), ret);
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI0_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_0_TO_QIDf, &entry, pQmap->qid[0]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI1_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_1_TO_QIDf, &entry, pQmap->qid[1]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI2_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_2_TO_QIDf, &entry, pQmap->qid[2]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI3_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_3_TO_QIDf, &entry, pQmap->qid[3]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI4_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_4_TO_QIDf, &entry, pQmap->qid[4]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI5_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_5_TO_QIDf, &entry, pQmap->qid[5]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI6_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_6_TO_QIDf, &entry, pQmap->qid[6]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI7_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_7_TO_QIDf, &entry, pQmap->qid[7]);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, INT_PRIO_TO_UCAST_QID_MAPNm, macId,sizeof(int_prio_to_ucast_qid_mapn_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_map_ucast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    int_prio_to_ucast_qid_mapn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INT_PRIO_TO_UCAST_QID_MAPNm, macId,sizeof(int_prio_to_ucast_qid_mapn_t), &entry), ret);
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI0_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_0_TO_QIDf, &entry, &pQmap->qid[0]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI1_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_1_TO_QIDf, &entry, &pQmap->qid[1]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI2_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_2_TO_QIDf, &entry, &pQmap->qid[2]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI3_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_3_TO_QIDf, &entry, &pQmap->qid[3]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI4_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_4_TO_QIDf, &entry, &pQmap->qid[4]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI5_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_5_TO_QIDf, &entry, &pQmap->qid[5]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI6_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_6_TO_QIDf, &entry, &pQmap->qid[6]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI7_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_UCAST_QID_MAPNm, INT_PRIO_TO_UCAST_QID_MAPN_PRIO_7_TO_QIDf, &entry, &pQmap->qid[7]);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_map_mcast_set(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    int_prio_to_mcast_qid_mapn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);
    int32_t i;

    for (i = 0; i < YT_QOS_MAX_QUEUE_NUM; i++)
    {
        if (IS_BIT_SET(pQmap->flags, i))
        {
            CMM_PARAM_CHK((CAL_MAX_MCAST_QUEUE_NUM(unit) <= pQmap->qid[i]), CMM_ERR_INPUT);
        }
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INT_PRIO_TO_MCAST_QID_MAPNm, macId,sizeof(int_prio_to_mcast_qid_mapn_t), &entry), ret);
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI0_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_0_TO_QIDf, &entry, pQmap->qid[0]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI1_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_1_TO_QIDf, &entry, pQmap->qid[1]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI2_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_2_TO_QIDf, &entry, pQmap->qid[2]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI3_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_3_TO_QIDf, &entry, pQmap->qid[3]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI4_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_4_TO_QIDf, &entry, pQmap->qid[4]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI5_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_5_TO_QIDf, &entry, pQmap->qid[5]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI6_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_6_TO_QIDf, &entry, pQmap->qid[6]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI7_QID)
    {
        HAL_FIELD_SET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_7_TO_QIDf, &entry, pQmap->qid[7]);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, INT_PRIO_TO_MCAST_QID_MAPNm, macId,sizeof(int_prio_to_mcast_qid_mapn_t), &entry), ret);
    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_map_mcast_get(yt_unit_t unit, yt_port_t port, yt_qos_qmap_t *pQmap)
{
    int_prio_to_mcast_qid_mapn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, INT_PRIO_TO_MCAST_QID_MAPNm, macId,sizeof(int_prio_to_mcast_qid_mapn_t), &entry), ret);
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI0_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_0_TO_QIDf, &entry, &pQmap->qid[0]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI1_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_1_TO_QIDf, &entry, &pQmap->qid[1]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI2_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_2_TO_QIDf, &entry, &pQmap->qid[2]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI3_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_3_TO_QIDf, &entry, &pQmap->qid[3]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI4_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_4_TO_QIDf, &entry, &pQmap->qid[4]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI5_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_5_TO_QIDf, &entry, &pQmap->qid[5]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI6_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_6_TO_QIDf, &entry, &pQmap->qid[6]);
    }
    if (pQmap->flags & YT_QOS_QMAP_FLAG_PRI7_QID)
    {
        HAL_FIELD_GET(INT_PRIO_TO_MCAST_QID_MAPNm, INT_PRIO_TO_MCAST_QID_MAPN_PRIO_7_TO_QIDf, &entry, &pQmap->qid[7]);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_forceDropPerQue_enable_set(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t enable)
{
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, qinfo.port);
    uint32_t regaddr;
    uint32_t regval;
    uint32_t regval2;
    uint32_t bit;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK(FAL_MAX_PORT_NUM < macId, CMM_ERR_INPUT);

    if (UNICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_UCAST_QUEUE_NUM(unit) < qinfo.qid, CMM_ERR_INPUT);
        regaddr = QOS_FORCEAC_UCASTQUE_REG(unit, macId, qinfo.qid);
        CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regaddr, &regval2), ret);
        regaddr += 4;
        bit = 26;
    }
    else if(MULTICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_MCAST_QUEUE_NUM(unit) < qinfo.qid, CMM_ERR_INPUT);
        regaddr = QOS_FORCEAC_MCASTQUE_REG(unit, macId, qinfo.qid);
        bit = 27;
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regaddr, &regval), ret);
    if (enable)
    {
        SET_BIT(regval, bit);
    }
    else
    {
        CLEAR_BIT(regval, bit);
    }

    if (UNICAST_QUEUE == qinfo.qtype)
    {
        CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regaddr - 4, regval2), ret);
    }
    CMM_ERR_CHK(HAL_MEM_DIRECT_WRITE(unit, regaddr, regval), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_forceDropPerQue_enable_get(yt_unit_t unit, yt_qid_t qinfo, yt_enable_t *pEnable)
{
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, qinfo.port);
    uint32_t regaddr;
    uint32_t regval;
    uint32_t bit;
    cmm_err_t ret = CMM_ERR_OK;

    CMM_PARAM_CHK(FAL_MAX_PORT_NUM < macId, CMM_ERR_INPUT);

    if (UNICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_UCAST_QUEUE_NUM(unit) < qinfo.qid, CMM_ERR_INPUT);
        regaddr = QOS_FORCEAC_UCASTQUE_REG(unit, macId, qinfo.qid) + 4;
        bit = 26;
    }
    else if(MULTICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_MCAST_QUEUE_NUM(unit) < qinfo.qid, CMM_ERR_INPUT);
        regaddr = QOS_FORCEAC_MCASTQUE_REG(unit, macId, qinfo.qid);
        bit = 27;
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_MEM_DIRECT_READ(unit, regaddr, &regval), ret);
    *pEnable = IS_BIT_SET(regval, bit) ? YT_ENABLE : YT_DISABLE;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_forceDrop_enable_set(yt_unit_t unit, yt_port_t port, yt_enable_t enable)
{
    yt_qid_t qinfo;
    uint32_t i;
    cmm_err_t ret = CMM_ERR_OK;

    qinfo.port = port;
    qinfo.qtype = UNICAST_QUEUE;
    for (i = 0; i < CAL_MAX_UCAST_QUEUE_NUM(unit); i++)
    {
        qinfo.qid = i;
        CMM_ERR_CHK(fal_tiger_qos_que_forceDropPerQue_enable_set(unit, qinfo, enable), ret);
    }

    qinfo.qtype = MULTICAST_QUEUE;
    for (i = 0; i < CAL_MAX_MCAST_QUEUE_NUM(unit); i++)
    {
        qinfo.qid = i;
        CMM_ERR_CHK(fal_tiger_qos_que_forceDropPerQue_enable_set(unit, qinfo, enable), ret);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_que_forceDrop_enable_get(yt_unit_t unit, yt_port_t port, yt_enable_t *pEnable)
{
    yt_qid_t qinfo;
    uint32_t ret;

    qinfo.port = port;
    qinfo.qtype = UNICAST_QUEUE;
    qinfo.qid = 0;
    CMM_ERR_CHK(fal_tiger_qos_que_forceDropPerQue_enable_get(unit, qinfo, pEnable), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_port_set(yt_unit_t unit, yt_port_t port, const yt_qos_remark_ctrl_t *pRemarkCtrl)
{
    egr_port_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    if (pRemarkCtrl->flags & (YT_QOS_REMARK_FLAG_CPRITYPE | YT_QOS_REMARK_FLAG_SPRITYPE))
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macId, sizeof(egr_port_ctrln_t), &entry), ret);
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_CPRI)
    {
        CMM_PARAM_CHK((YT_ENABLE < pRemarkCtrl->cpriEn), CMM_ERR_INPUT);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CPRIO_REMARK_ENf, &entry, pRemarkCtrl->cpriEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_CFI)
    {
        CMM_PARAM_CHK((YT_ENABLE < pRemarkCtrl->ccfiEn), CMM_ERR_INPUT);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CFI_REMARK_ENf, &entry, pRemarkCtrl->ccfiEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_SPRI)
    {
        CMM_PARAM_CHK((YT_ENABLE < pRemarkCtrl->spriEn), CMM_ERR_INPUT);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_SPRIO_REMARK_ENf, &entry, pRemarkCtrl->spriEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_SDEI)
    {
        CMM_PARAM_CHK((YT_ENABLE < pRemarkCtrl->sdeiEn), CMM_ERR_INPUT);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_DEI_REMARK_ENf, &entry, pRemarkCtrl->sdeiEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_DSCP)
    {
        CMM_PARAM_CHK((YT_ENABLE < pRemarkCtrl->dscpEn), CMM_ERR_INPUT);
        HAL_FIELD_SET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_DSCP_REMARK_ENf, &entry, pRemarkCtrl->dscpEn);
    }
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PORT_CTRLNm, macId, sizeof(egr_port_ctrln_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_port_get(yt_unit_t unit, yt_port_t port, yt_qos_remark_ctrl_t *pRemarkCtrl)
{
    egr_port_ctrln_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, port);

    if (pRemarkCtrl->flags & (YT_QOS_REMARK_FLAG_CPRITYPE | YT_QOS_REMARK_FLAG_SPRITYPE))
    {
        return CMM_ERR_NOT_SUPPORT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PORT_CTRLNm, macId, sizeof(egr_port_ctrln_t), &entry), ret);
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_SPRI)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_SPRIO_REMARK_ENf, &entry, &pRemarkCtrl->spriEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_SDEI)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_DEI_REMARK_ENf, &entry, &pRemarkCtrl->sdeiEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_CPRI)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CPRIO_REMARK_ENf, &entry, &pRemarkCtrl->cpriEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_CFI)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_CFI_REMARK_ENf, &entry, &pRemarkCtrl->ccfiEn);
    }
    if (pRemarkCtrl->flags & YT_QOS_REMARK_FLAG_DSCP)
    {
        HAL_FIELD_GET(EGR_PORT_CTRLNm, EGR_PORT_CTRLN_DSCP_REMARK_ENf, &entry,  &pRemarkCtrl->dscpEn);
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_dscp_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t newDscp)
{
    egr_dscp_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t index = 0;

    index = (srcInfo.prio << 2) | srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_DSCP_REMARKNm, index, sizeof(egr_dscp_remarkn_t), &entry), ret);
    HAL_FIELD_SET(EGR_DSCP_REMARKNm, EGR_DSCP_REMARKN_INT_PRIO_TO_DSCPf, &entry, newDscp);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_DSCP_REMARKNm, index, sizeof(egr_dscp_remarkn_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_dscp_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_dscp_t *pNewDscp)
{
    egr_dscp_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t index = 0;
    uint32_t newDscp;

    index = (srcInfo.prio << 2)| srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_DSCP_REMARKNm, index, sizeof(egr_dscp_remarkn_t), &entry), ret);
    HAL_FIELD_GET(EGR_DSCP_REMARKNm, EGR_DSCP_REMARKN_INT_PRIO_TO_DSCPf, &entry, &newDscp);
    *pNewDscp = newDscp;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_cpri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    egr_prio_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_CPRI_REMARK_INDEX << 5) | (srcInfo.prio << 2) | srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PRIO_REMARKNm, index, sizeof(egr_prio_remarkn_t), &entry), ret);
    HAL_FIELD_SET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_DEIf, &entry, dstInfo.dei);
    HAL_FIELD_SET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_PRIOf, &entry, dstInfo.prio);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PRIO_REMARKNm, index, sizeof(egr_prio_remarkn_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_cpri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    egr_prio_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_CPRI_REMARK_INDEX << 5) | (srcInfo.prio << 2) | srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PRIO_REMARKNm, index,sizeof(egr_prio_remarkn_t), &entry), ret);
    HAL_FIELD_GET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_DEIf, &entry, &pDstInfo->dei);
    HAL_FIELD_GET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_PRIOf, &entry, &pDstInfo->prio);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_spri_set(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t dstInfo)
{
    egr_prio_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_SPRI_REMARK_INDEX << 5) | (srcInfo.prio << 2) | srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PRIO_REMARKNm, index,sizeof(egr_prio_remarkn_t), &entry), ret);
    HAL_FIELD_SET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_DEIf, &entry, dstInfo.dei);
    HAL_FIELD_SET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_PRIOf, &entry, dstInfo.prio);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, EGR_PRIO_REMARKNm, index,sizeof(egr_prio_remarkn_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_remark_spri_get(yt_unit_t unit, yt_qos_remark_info_t srcInfo, yt_qos_pmap_tci_t *pDstInfo)
{
    egr_prio_remarkn_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint32_t index;

    index = (YT_SPRI_REMARK_INDEX << 5) | (srcInfo.prio << 2) | srcInfo.dp;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, EGR_PRIO_REMARKNm, index,sizeof(egr_prio_remarkn_t), &entry), ret);
    HAL_FIELD_GET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_DEIf, &entry, &pDstInfo->dei);
    HAL_FIELD_GET(EGR_PRIO_REMARKNm, EGR_PRIO_REMARKN_INT_PRIO_TO_PRIOf, &entry, &pDstInfo->prio);

    return CMM_ERR_OK;
}

uint32_t fal_tiger_qos_queue_id_get(yt_unit_t unit, yt_qid_t qinfo, uint8_t *pId)
{
    yt_macid_t macId = CAL_YTP_TO_MAC(unit, qinfo.port);

    CMM_PARAM_CHK(FAL_MAX_PORT_NUM <= macId, CMM_ERR_INPUT);

    if (UNICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_UCAST_QUEUE_NUM(unit) <= qinfo.qid, CMM_ERR_INPUT);
        *pId = macId * CAL_MAX_UCAST_QUEUE_NUM(unit) + qinfo.qid;
    }
    else if(MULTICAST_QUEUE == qinfo.qtype)
    {
        CMM_PARAM_CHK(CAL_MAX_MCAST_QUEUE_NUM(unit) <= qinfo.qid, CMM_ERR_INPUT);
        *pId = macId  * CAL_MAX_MCAST_QUEUE_NUM(unit) + qinfo.qid + (FAL_MAX_PORT_NUM * CAL_MAX_UCAST_QUEUE_NUM(unit));
    }
    else
    {
        return CMM_ERR_INPUT;
    }

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_queue_pri_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t qpri)
{
    qsch_flow_map_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(fal_tiger_qos_ac_enqueue_disable(unit, qinfo, TRUE), ret);
    CMM_ERR_CHK(fal_tiger_qos_ac_queue_flush(unit, qinfo), ret);

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);
    HAL_FIELD_SET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_C_PRIf, &entry, qpri);
    HAL_FIELD_SET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_E_PRIf, &entry, qpri);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);

    CMM_ERR_CHK(fal_tiger_qos_ac_enqueue_disable(unit, qinfo, FALSE), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_queue_pri_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_pri_t *pQpri)
{
    qsch_flow_map_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;
    uint32_t qpri;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);
    HAL_FIELD_GET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_C_PRIf, &entry, &qpri);
    *pQpri = qpri;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_dwrr_mode_set(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t dwrrMode)
{
    qsch_c_dwrr_cfg_tbl_t cEntry;
    qsch_e_dwrr_cfg_tbl_t eEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_queue_pri_t pri = 0;
    uint32_t idx;

    if(fal_tiger_qos_schedule_queue_pri_get(unit, qinfo, &pri) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    idx = (CAL_YTP_TO_MAC(unit, qinfo.port) * (CAL_MAX_UCAST_QUEUE_NUM(unit) + CAL_MAX_MCAST_QUEUE_NUM(unit))) + pri;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_C_DWRR_CFG_TBLm, idx, sizeof(qsch_c_dwrr_cfg_tbl_t), &cEntry), ret);
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_E_DWRR_CFG_TBLm, idx, sizeof(qsch_e_dwrr_cfg_tbl_t), &eEntry), ret);
    HAL_FIELD_SET(QSCH_C_DWRR_CFG_TBLm, QSCH_C_DWRR_CFG_TBL_DWRR_MODEf, &cEntry, dwrrMode);
    HAL_FIELD_SET(QSCH_E_DWRR_CFG_TBLm, QSCH_E_DWRR_CFG_TBL_DWRR_MODEf, &eEntry, dwrrMode);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_C_DWRR_CFG_TBLm, idx, sizeof(qsch_c_dwrr_cfg_tbl_t), &cEntry), ret);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_E_DWRR_CFG_TBLm, idx, sizeof(qsch_e_dwrr_cfg_tbl_t), &eEntry), ret);


    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_dwrr_mode_get(yt_unit_t unit, yt_qid_t qinfo, yt_rate_mode_t *pDwrrMode)
{
    qsch_c_dwrr_cfg_tbl_t cEntry;
    cmm_err_t ret = CMM_ERR_OK;
    yt_queue_pri_t pri = 0;
    uint32_t idx;
    uint32_t dwrrMode;

    if(fal_tiger_qos_schedule_queue_pri_get(unit, qinfo, &pri) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    idx = (CAL_YTP_TO_MAC(unit, qinfo.port) * (CAL_MAX_UCAST_QUEUE_NUM(unit) + CAL_MAX_MCAST_QUEUE_NUM(unit))) + pri;
    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_C_DWRR_CFG_TBLm, idx, sizeof(qsch_c_dwrr_cfg_tbl_t), &cEntry), ret);
    HAL_FIELD_GET(QSCH_C_DWRR_CFG_TBLm, QSCH_C_DWRR_CFG_TBL_DWRR_MODEf, &cEntry, &dwrrMode);
    *pDwrrMode = dwrrMode;

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_dwrr_weight_set(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t qweight)
{
    qsch_flow_map_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);
    HAL_FIELD_SET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_C_DWRR_WEIGHTf, &entry, qweight);
    HAL_FIELD_SET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_E_DWRR_WEIGHTf, &entry, qweight);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}

yt_ret_t fal_tiger_qos_schedule_dwrr_weight_get(yt_unit_t unit, yt_qid_t qinfo, yt_queue_weight_t *pQweight)
{
    qsch_flow_map_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;
    uint32_t qweight;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    CMM_ERR_CHK(HAL_TBL_REG_READ(unit, QSCH_FLOW_MAP_TBLm, idx, sizeof(qsch_flow_map_tbl_t), &entry), ret);
    HAL_FIELD_GET(QSCH_FLOW_MAP_TBLm, QSCH_FLOW_MAP_TBL_C_DWRR_WEIGHTf, &entry, &qweight);
    *pQweight = qweight;

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_qos_ac_queue_flush(yt_unit_t unit, yt_qid_t qinfo)
{
    flush_cfg_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t flush_qid = 0;
    uint32_t flush_done = 0;
    uint16_t busy_cnt = DONE_CHECK_NUMBER;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &flush_qid) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    HAL_FIELD_SET(FLUSH_CFGm, FLUSH_CFG_FLUSH_DONEf, &entry, 0);
    HAL_FIELD_SET(FLUSH_CFGm, FLUSH_CFG_FLUSH_REQf, &entry, 1);
    HAL_FIELD_SET(FLUSH_CFGm, FLUSH_CFG_FLUSH_QIDf, &entry, flush_qid);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, FLUSH_CFGm, 0, sizeof(flush_cfg_t), &entry), ret);
 
    while (busy_cnt)
    {
        flush_done = 0;
        CMM_ERR_CHK(HAL_TBL_REG_READ(unit, FLUSH_CFGm, 0, sizeof(flush_cfg_t), &entry), ret);
        HAL_FIELD_GET(FLUSH_CFGm, FLUSH_CFG_FLUSH_DONEf, &entry, &flush_done);
        if(flush_done)
        {
            break;
        }        
        
        busy_cnt--;
        
        if(0 == busy_cnt)
        {
            return CMM_ERR_FAIL;
        }
    }

    return CMM_ERR_OK;
}

static yt_ret_t fal_tiger_qos_ac_enqueue_disable(yt_unit_t unit, yt_qid_t qinfo, uint8_t disable)
{
    oq_enq_dis_tbl_t entry;
    cmm_err_t ret = CMM_ERR_OK;
    uint8_t idx = 0;

    if(fal_tiger_qos_queue_id_get(unit, qinfo, &idx) != CMM_ERR_OK)
    {
        return CMM_ERR_INPUT;
    }

    HAL_FIELD_SET(OQ_ENQ_DIS_TBLm, OQ_ENQ_DIS_TBL_ENQ_DISf, &entry, disable);
    CMM_ERR_CHK(HAL_TBL_REG_WRITE(unit, OQ_ENQ_DIS_TBLm, idx, sizeof(oq_enq_dis_tbl_t), &entry), ret);

    return CMM_ERR_OK;
}
