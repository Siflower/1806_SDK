/**  @file
  *  @brief    brief  description
  *  @author   
  *  @date     2020.5.7
  *  @version  1.0
  *  @note
  *  detailed  description
  *  $LastChangedDate$
  *  $LastChangedRevision$
  *  $LastChangedBy$
  */

#include "l2sw_switch.h"
#include "qos.h"
#include "sal_string.h"
#include "l2sw_asicdrv_qos.h"
#include "l2sw_asicdrv_fc.h"
#include "l2sw_asicdrv_scheduling.h"

static l2sw_api_ret_t _l2sw_qos_init(l2sw_uint8 units, l2sw_queue_num_t queueNum)
{
    CONST_T l2sw_uint16 g_prioritytToQid[8][8]= {
            {0, 0,0,0,0,0,0,0},
            {0, 0,0,0,7,7,7,7},
            {0, 0,0,0,1,1,7,7},
            {0, 0,1,1,2,2,7,7},
            {0, 0,1,1,2,3,7,7},
            {0, 0,1,2,3,4,7,7},
            {0, 0,1,2,3,4,5,7},
            {0, 1,2,3,4,5,6,7}
    };

    CONST_T l2sw_uint32 g_priorityDecision[8] = {0x01, 0x80,0x04,0x02,0x20,0x40,0x10,0x08};
    CONST_T l2sw_uint32 g_prioritytRemap[8] = {0,1,2,3,4,5,6,7};

    l2sw_api_ret_t retVal;
    l2sw_uint32 qmapidx;
    l2sw_uint32 priority;
    l2sw_uint32 priDec;
    l2sw_uint32 port;
    l2sw_uint32 dscp, index;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (queueNum <= 0 || queueNum > L2SW_MAX_NUM_OF_QUEUE)
        return L2SW_ERR_QUEUE_NUM;

    /*Set Output Queue Number*/
    if (L2SW_MAX_NUM_OF_QUEUE == queueNum)
        qmapidx = 0;
    else
        qmapidx = queueNum;

    L2SW_SCAN_ALL_PHY_PORTMASK(units, port)
    {
        if ((retVal = l2sw_setAsicOutputQueueMappingIndex(units, port, qmapidx)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Set Priority to Qid*/
    for(index = 0; index < L2SW_MAX_NUM_OF_QUEUE; index++)
    {    
        for (priority = 0; priority <= L2SW_PRIMAX; priority++)
        {
            if ((retVal = l2sw_setAsicPriorityToQIDMappingTable(units, index, priority, g_prioritytToQid[index][priority])) != L2SW_ERR_OK)
                return retVal;
        }
    }

    /*Set Flow Control Type to Ingress Flow Control*/
    if ((retVal = l2sw_setAsicFlowControlSelect(units, FC_INGRESS)) != L2SW_ERR_OK)
        return retVal;


    /*Priority Decision Order*/
    for (priDec = 0;priDec < PRIDEC_END;priDec++)
    {
        if ((retVal = l2sw_setAsicPriorityDecision(units, PRIDECTBL_IDX0, priDec, g_priorityDecision[priDec])) != L2SW_ERR_OK)
            return retVal;
        if ((retVal = l2sw_setAsicPriorityDecision(units, PRIDECTBL_IDX1, priDec, g_priorityDecision[priDec])) != L2SW_ERR_OK)
            return retVal;
    }

    /*Set Port-based Priority to 0*/
    L2SW_SCAN_ALL_PHY_PORTMASK(units, port)
    {
        if ((retVal = l2sw_setAsicPriorityPortBased(units, port, 0)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Disable 1p Remarking*/
    L2SW_SCAN_ALL_PHY_PORTMASK(units, port)
    {
        if ((retVal = l2sw_setAsicRemarkingDot1pAbility(units, port, L2SW_DISABLED)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Disable DSCP Remarking*/
    if ((retVal = l2sw_setAsicRemarkingDscpAbility(units, L2SW_DISABLED)) != L2SW_ERR_OK)
        return retVal;

    /*Set 1p & DSCP  Priority Remapping & Remarking*/
    for (priority = 0; priority <= L2SW_PRIMAX; priority++)
    {
        if ((retVal = l2sw_setAsicPriorityDot1qRemapping(units, priority, g_prioritytRemap[priority])) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicRemarkingDot1pParameter(units, priority, 0)) != L2SW_ERR_OK)
            return retVal;

        if ((retVal = l2sw_setAsicRemarkingDscpParameter(units, priority, 0)) != L2SW_ERR_OK)
            return retVal;
    }

    /*Set DSCP Priority*/
    for (dscp = 0; dscp <= 63; dscp++)
    {
        if ((retVal = l2sw_setAsicPriorityDscpBased(units, dscp, 0)) != L2SW_ERR_OK)
            return retVal;
    }

    /* Finetune B/T value */
    if((retVal = l2sw_setAsicReg(units, 0x1722, 0x1158)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_priSel_set(l2sw_uint8 units, l2sw_qos_priDecTbl_t index, l2sw_priority_select_t *pPriDec)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 port_pow;
    l2sw_uint32 dot1q_pow;
    l2sw_uint32 dscp_pow;
    l2sw_uint32 acl_pow;
    l2sw_uint32 svlan_pow;
    l2sw_uint32 cvlan_pow;
    l2sw_uint32 smac_pow;
    l2sw_uint32 dmac_pow;
    l2sw_uint32 i;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index < 0 || index >= PRIDECTBL_END)
        return L2SW_ERR_ENTRY_INDEX;

    if (pPriDec->port_pri >= 8 || pPriDec->dot1q_pri >= 8 || pPriDec->acl_pri >= 8 || pPriDec->dscp_pri >= 8 ||
       pPriDec->cvlan_pri >= 8 || pPriDec->svlan_pri >= 8 || pPriDec->dmac_pri >= 8 || pPriDec->smac_pri >= 8)
        return L2SW_ERR_QOS_SEL_PRI_SOURCE;

    port_pow = 1;
    for (i = pPriDec->port_pri; i > 0; i--)
        port_pow = (port_pow)*2;

    dot1q_pow = 1;
    for (i = pPriDec->dot1q_pri; i > 0; i--)
        dot1q_pow = (dot1q_pow)*2;

    acl_pow = 1;
    for (i = pPriDec->acl_pri; i > 0; i--)
        acl_pow = (acl_pow)*2;

    dscp_pow = 1;
    for (i = pPriDec->dscp_pri; i > 0; i--)
        dscp_pow = (dscp_pow)*2;

    svlan_pow = 1;
    for (i = pPriDec->svlan_pri; i > 0; i--)
        svlan_pow = (svlan_pow)*2;

    cvlan_pow = 1;
    for (i = pPriDec->cvlan_pri; i > 0; i--)
        cvlan_pow = (cvlan_pow)*2;

    dmac_pow = 1;
    for (i = pPriDec->dmac_pri; i > 0; i--)
        dmac_pow = (dmac_pow)*2;

    smac_pow = 1;
    for (i = pPriDec->smac_pri; i > 0; i--)
        smac_pow = (smac_pow)*2;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_PORT, port_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_ACL, acl_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_DSCP, dscp_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_1Q, dot1q_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_1AD, svlan_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_CVLAN, cvlan_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_DA, dmac_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_setAsicPriorityDecision(units, index, PRIDEC_SA, smac_pow)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_priSel_get(l2sw_uint8 units, l2sw_qos_priDecTbl_t index, l2sw_priority_select_t *pPriDec)
{

    l2sw_api_ret_t retVal;
    l2sw_int32 i;
    l2sw_uint32 port_pow;
    l2sw_uint32 dot1q_pow;
    l2sw_uint32 dscp_pow;
    l2sw_uint32 acl_pow;
    l2sw_uint32 svlan_pow;
    l2sw_uint32 cvlan_pow;
    l2sw_uint32 smac_pow;
    l2sw_uint32 dmac_pow;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (index < 0 || index >= PRIDECTBL_END)
        return L2SW_ERR_ENTRY_INDEX;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_PORT, &port_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_ACL, &acl_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_DSCP, &dscp_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_1Q, &dot1q_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_1AD, &svlan_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_CVLAN, &cvlan_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_DA, &dmac_pow)) != L2SW_ERR_OK)
        return retVal;

    if ((retVal = l2sw_getAsicPriorityDecision(units, index, PRIDEC_SA, &smac_pow)) != L2SW_ERR_OK)
        return retVal;

    for (i = 31; i >= 0; i--)
    {
        if (port_pow & (1 << i))
        {
            pPriDec->port_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (dot1q_pow & (1 << i))
        {
            pPriDec->dot1q_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (acl_pow & (1 << i))
        {
            pPriDec->acl_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (dscp_pow & (1 << i))
        {
            pPriDec->dscp_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (svlan_pow & (1 << i))
        {
            pPriDec->svlan_pri = i;
            break;
        }
    }

    for (i = 31;i  >= 0; i--)
    {
        if (cvlan_pow & (1 << i))
        {
            pPriDec->cvlan_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (dmac_pow&(1<<i))
        {
            pPriDec->dmac_pri = i;
            break;
        }
    }

    for (i = 31; i >= 0; i--)
    {
        if (smac_pow & (1 << i))
        {
            pPriDec->smac_pri = i;
            break;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pPriRemap_set(l2sw_uint8 units, l2sw_pri_t dot1p_pri, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (dot1p_pri > L2SW_PRIMAX || int_pri > L2SW_PRIMAX)
        return  L2SW_ERR_VLAN_PRIORITY;

    if ((retVal = l2sw_setAsicPriorityDot1qRemapping(units, dot1p_pri, int_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pPriRemap_get(l2sw_uint8 units, l2sw_pri_t dot1p_pri, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (dot1p_pri > L2SW_PRIMAX)
        return  L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_getAsicPriorityDot1qRemapping(units, dot1p_pri, pInt_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpPriRemap_set(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if (dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    if ((retVal = l2sw_setAsicPriorityDscpBased(units, dscp, int_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpPriRemap_get(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    if ((retVal = l2sw_getAsicPriorityDscpBased(units, dscp, pInt_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_portPri_set(l2sw_uint8 units, l2sw_port_t port, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_setAsicPriorityPortBased(units, l2sw_switch_port_L2P_get(units, port), int_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_portPri_get(l2sw_uint8 units, l2sw_port_t port, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicPriorityPortBased(units, l2sw_switch_port_L2P_get(units, port), pInt_pri)) != L2SW_ERR_OK)
        return retVal;


    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_queueNum_set(l2sw_uint8 units, l2sw_port_t port, l2sw_queue_num_t queue_num)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((0 == queue_num) || (queue_num > L2SW_MAX_NUM_OF_QUEUE))
        return L2SW_ERR_FAILED;

    if (L2SW_MAX_NUM_OF_QUEUE == queue_num)
        queue_num = 0;

    if ((retVal = l2sw_setAsicOutputQueueMappingIndex(units, l2sw_switch_port_L2P_get(units, port), queue_num)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_queueNum_get(l2sw_uint8 units, l2sw_port_t port, l2sw_queue_num_t *pQueue_num)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 qidx;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicOutputQueueMappingIndex(units, l2sw_switch_port_L2P_get(units, port), &qidx)) != L2SW_ERR_OK)
        return retVal;

    if (0 == qidx)
        *pQueue_num = 8;
    else
        *pQueue_num = qidx;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_priMap_set(l2sw_uint8 units, l2sw_queue_num_t queue_num, l2sw_qos_pri2queue_t *pPri2qid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pri;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((0 == queue_num) || (queue_num > L2SW_MAX_NUM_OF_QUEUE))
        return L2SW_ERR_QUEUE_NUM;

    if (L2SW_MAX_NUM_OF_QUEUE == queue_num)
        queue_num = 0;

    for (pri = 0; pri <= L2SW_PRIMAX; pri++)
    {
        if (pPri2qid->pri2queue[pri] > L2SW_QIDMAX)
            return L2SW_ERR_QUEUE_ID;

        if ((retVal = l2sw_setAsicPriorityToQIDMappingTable(units, queue_num, pri, pPri2qid->pri2queue[pri])) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_priMap_get(l2sw_uint8 units, l2sw_queue_num_t queue_num, l2sw_qos_pri2queue_t *pPri2qid)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 pri;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((0 == queue_num) || (queue_num > L2SW_MAX_NUM_OF_QUEUE))
        return L2SW_ERR_QUEUE_NUM;

    if (L2SW_MAX_NUM_OF_QUEUE == queue_num)
        queue_num = 0;

    for (pri = 0; pri <= L2SW_PRIMAX; pri++)
    {
        if ((retVal = l2sw_getAsicPriorityToQIDMappingTable(units, queue_num, pri, &pPri2qid->pri2queue[pri])) != L2SW_ERR_OK)
            return retVal;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_schedulingQueue_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_queue_weights_t *pQweights)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 qid;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    for (qid = 0; qid < L2SW_QUEUENO; qid ++)
    {

        if (pQweights->weights[qid] > QOS_WEIGHT_MAX)
            return L2SW_ERR_QOS_QUEUE_WEIGHT;

        if (0 == pQweights->weights[qid])
        {
            if ((retVal = l2sw_setAsicQueueType(units, l2sw_switch_port_L2P_get(units, port), qid, QTYPE_STRICT)) != L2SW_ERR_OK)
                return retVal;
        }
        else
        {
            if ((retVal = l2sw_setAsicQueueType(units, l2sw_switch_port_L2P_get(units, port), qid, QTYPE_WFQ)) != L2SW_ERR_OK)
                return retVal;

            if ((retVal = l2sw_setAsicWFQWeight(units, l2sw_switch_port_L2P_get(units, port),qid, pQweights->weights[qid])) != L2SW_ERR_OK)
                return retVal;
        }
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_schedulingQueue_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_queue_weights_t *pQweights)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 qid,qtype,qweight;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    for (qid = 0; qid < L2SW_QUEUENO; qid++)
    {
        if ((retVal = l2sw_getAsicQueueType(units, l2sw_switch_port_L2P_get(units, port), qid, &qtype)) != L2SW_ERR_OK)
            return retVal;

        if (QTYPE_STRICT == qtype)
        {
            pQweights->weights[qid] = 0;
        }
        else if (QTYPE_WFQ == qtype)
        {
            if ((retVal = l2sw_getAsicWFQWeight(units, l2sw_switch_port_L2P_get(units, port), qid, &qweight)) != L2SW_ERR_OK)
                return retVal;
            pQweights->weights[qid] = qweight;
        }
    }
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemarkEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicRemarkingDot1pAbility(units, l2sw_switch_port_L2P_get(units, port), enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemarkEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicRemarkingDot1pAbility(units, l2sw_switch_port_L2P_get(units, port), pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemark_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t dot1p_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if (dot1p_pri > L2SW_PRIMAX)
        return L2SW_ERR_VLAN_PRIORITY;

    if ((retVal = l2sw_setAsicRemarkingDot1pParameter(units, int_pri, dot1p_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemark_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t *pDot1p_pri)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_getAsicRemarkingDot1pParameter(units, int_pri, pDot1p_pri)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemarkSrcSel_set(l2sw_uint8 units, l2sw_qos_1pRmkSrc_t type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= DOT1P_RMK_SRC_END )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_setAsicRemarkingDot1pSrc(units, type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_1pRemarkSrcSel_get(l2sw_uint8 units, l2sw_qos_1pRmkSrc_t *pType)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicRemarkingDot1pSrc(units, pType)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemarkEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /*for whole system function, the port value should be 0xFF*/
    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if (enable >= L2SW_ENABLE_END)
        return L2SW_ERR_INPUT;

    if ((retVal = l2sw_setAsicRemarkingDscpAbility(units, enable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemarkEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /*for whole system function, the port value should be 0xFF*/
    if (port != L2SW_WHOLE_SYSTEM)
        return L2SW_ERR_PORT_ID;

    if ((retVal = l2sw_getAsicRemarkingDscpAbility(units, pEnable)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemark_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_dscp_t dscp)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if (dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    if ((retVal = l2sw_setAsicRemarkingDscpParameter(units, int_pri, dscp)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemark_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_dscp_t *pDscp)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (int_pri > L2SW_PRIMAX )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_getAsicRemarkingDscpParameter(units, int_pri, pDscp)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemarkSrcSel_set(l2sw_uint8 units, l2sw_qos_dscpRmkSrc_t type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (type >= DSCP_RMK_SRC_END )
        return L2SW_ERR_QOS_INT_PRIORITY;

    if ((retVal = l2sw_setAsicRemarkingDscpSrc(units, type)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemarkSrcSel_get(l2sw_uint8 units, l2sw_qos_dscpRmkSrc_t *pType)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_getAsicRemarkingDscpSrc(units, pType)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemark2Dscp_set(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_dscp_t rmkDscp)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((dscp > L2SW_DSCPMAX) || (rmkDscp > L2SW_DSCPMAX))
        return L2SW_ERR_QOS_DSCP_VALUE;

    if ((retVal = l2sw_setAsicRemarkingDscp2Dscp(units, dscp, rmkDscp)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_dscpRemark2Dscp_get(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_dscp_t *pDscp)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if (dscp > L2SW_DSCPMAX)
        return L2SW_ERR_QOS_DSCP_VALUE;

    if ((retVal = l2sw_getAsicRemarkingDscp2Dscp(units, dscp, pDscp)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_portPriSelIndex_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_priDecTbl_t index)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (index >= PRIDECTBL_END )
        return L2SW_ERR_ENTRY_INDEX;

    if ((retVal = l2sw_setAsicPortPriorityDecisionIndex(units, l2sw_switch_port_L2P_get(units, port), index)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_qos_portPriSelIndex_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_priDecTbl_t *pIndex)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check Port Valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_getAsicPortPriorityDecisionIndex(units, l2sw_switch_port_L2P_get(units, port), pIndex)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

/* Function Name:
 *      l2sw_qos_init
 * Description:
 *      Configure Qos default settings with queue number assigment to each port.
 * Input:
 *      queueNum - Queue number of each port.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_QUEUE_NUM    - Invalid queue number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      This API will initialize related Qos setting with queue number assigment.
 *      The queue number is from 1 to 8.
 */
l2sw_api_ret_t l2sw_qos_init(l2sw_uint8 units, l2sw_queue_num_t queueNum)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_init(units, queueNum);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_priSel_set
 * Description:
 *      Configure the priority order among different priority mechanism.
 * Input:
 *      index - Priority decision table index (0~1)
 *      pPriDec - Priority assign for port, dscp, 802.1p, cvlan, svlan, acl based priority decision.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK                   - OK
 *      L2SW_ERR_FAILED               - Failed
 *      L2SW_ERR_SMI                  - SMI access error
 *      L2SW_ERR_QOS_SEL_PRI_SOURCE   - Invalid priority decision source parameter.
 * Note:
 *      ASIC will follow user priority setting of mechanisms to select mapped queue priority for receiving frame.
 *      If two priority mechanisms are the same, the ASIC will chose the highest priority from mechanisms to
 *      assign queue priority to receiving frame.
 *      The priority sources are:
 *      - PRIDEC_PORT
 *      - PRIDEC_ACL
 *      - PRIDEC_DSCP
 *      - PRIDEC_1Q
 *      - PRIDEC_1AD
 *      - PRIDEC_CVLAN
 *      - PRIDEC_DA
 *      - PRIDEC_SA
 */
l2sw_api_ret_t l2sw_qos_priSel_set(l2sw_uint8 units, l2sw_qos_priDecTbl_t index, l2sw_priority_select_t *pPriDec)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_priSel_set(units, index, pPriDec);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_priSel_get
 * Description:
 *      Get the priority order configuration among different priority mechanism.
 * Input:
 *      index - Priority decision table index (0~1)
 * Output:
 *      pPriDec - Priority assign for port, dscp, 802.1p, cvlan, svlan, acl based priority decision .
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      ASIC will follow user priority setting of mechanisms to select mapped queue priority for receiving frame.
 *      If two priority mechanisms are the same, the ASIC will chose the highest priority from mechanisms to
 *      assign queue priority to receiving frame.
 *      The priority sources are:
 *      - PRIDEC_PORT,
 *      - PRIDEC_ACL,
 *      - PRIDEC_DSCP,
 *      - PRIDEC_1Q,
 *      - PRIDEC_1AD,
 *      - PRIDEC_CVLAN,
 *      - PRIDEC_DA,
 *      - PRIDEC_SA,
 */
l2sw_api_ret_t l2sw_qos_priSel_get(l2sw_uint8 units, l2sw_qos_priDecTbl_t index, l2sw_priority_select_t *pPriDec)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_priSel_get(units, index, pPriDec);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pPriRemap_set
 * Description:
 *      Configure 1Q priorities mapping to internal absolute priority.
 * Input:
 *      dot1p_pri   - 802.1p priority value.
 *      int_pri     - internal priority value.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid 1p priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      Priority of 802.1Q assignment for internal asic priority, and it is used for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_1pPriRemap_set(l2sw_uint8 units, l2sw_pri_t dot1p_pri, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pPriRemap_set(units, dot1p_pri, int_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pPriRemap_get
 * Description:
 *      Get 1Q priorities mapping to internal absolute priority.
 * Input:
 *      dot1p_pri - 802.1p priority value .
 * Output:
 *      pInt_pri - internal priority value.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      Priority of 802.1Q assigment for internal asic priority, and it is uesed for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_1pPriRemap_get(l2sw_uint8 units, l2sw_pri_t dot1p_pri, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pPriRemap_get(units, dot1p_pri, pInt_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpPriRemap_set
 * Description:
 *      Map dscp value to internal priority.
 * Input:
 *      dscp    - Dscp value of receiving frame
 *      int_pri - internal priority value .
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid DSCP value.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      The Differentiated Service Code Point is a selector for router's per-hop behaviors. As a selector, there is no implication that a numerically
 *      greater DSCP implies a better network service. As can be seen, the DSCP totally overlaps the old precedence field of TOS. So if values of
 *      DSCP are carefully chosen then backward compatibility can be achieved.
 */
l2sw_api_ret_t l2sw_qos_dscpPriRemap_set(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpPriRemap_set(units, dscp, int_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpPriRemap_get
 * Description:
 *      Get dscp value to internal priority.
 * Input:
 *      dscp - Dscp value of receiving frame
 * Output:
 *      pInt_pri - internal priority value.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid DSCP value.
 * Note:
 *      The Differentiated Service Code Point is a selector for router's per-hop behaviors. As a selector, there is no implication that a numerically
 *      greater DSCP implies a better network service. As can be seen, the DSCP totally overlaps the old precedence field of TOS. So if values of
 *      DSCP are carefully chosen then backward compatibility can be achieved.
 */
l2sw_api_ret_t l2sw_qos_dscpPriRemap_get(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpPriRemap_get(units, dscp, pInt_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_portPri_set
 * Description:
 *      Configure priority usage to each port.
 * Input:
 *      port - Port id.
 *      int_pri - internal priority value.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_QOS_SEL_PORT_PRI - Invalid port priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      The API can set priority of port assignments for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_portPri_set(l2sw_uint8 units, l2sw_port_t port, l2sw_pri_t int_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_portPri_set(units, port, int_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_portPri_get
 * Description:
 *      Get priority usage to each port.
 * Input:
 *      port - Port id.
 * Output:
 *      pInt_pri - internal priority value.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      The API can get priority of port assignments for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_portPri_get(l2sw_uint8 units, l2sw_port_t port, l2sw_pri_t *pInt_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_portPri_get(units, port, pInt_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_queueNum_set
 * Description:
 *      Set output queue number for each port.
 * Input:
 *      port    - Port id.
 *      index   - Mapping queue number (1~8)
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_QUEUE_NUM    - Invalid queue number.
 * Note:
 *      The API can set the output queue number of the specified port. The queue number is from 1 to 8.
 */
l2sw_api_ret_t l2sw_qos_queueNum_set(l2sw_uint8 units, l2sw_port_t port, l2sw_queue_num_t queue_num)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_queueNum_set(units, port, queue_num);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_queueNum_get
 * Description:
 *      Get output queue number.
 * Input:
 *      port - Port id.
 * Output:
 *      pQueue_num - Mapping queue number
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API will return the output queue number of the specified port. The queue number is from 1 to 8.
 */
l2sw_api_ret_t l2sw_qos_queueNum_get(l2sw_uint8 units, l2sw_port_t port, l2sw_queue_num_t *pQueue_num)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_queueNum_get(units, port, pQueue_num);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_priMap_set
 * Description:
 *      Set output queue number for each port.
 * Input:
 *      queue_num   - Queue number usage.
 *      pPri2qid    - Priority mapping to queue ID.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_INPUT            - Invalid input parameters.
 *      L2SW_ERR_QUEUE_NUM        - Invalid queue number.
 *      L2SW_ERR_QUEUE_ID         - Invalid queue id.
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      ASIC supports priority mapping to queue with different queue number from 1 to 8.
 *      For different queue numbers usage, ASIC supports different internal available queue IDs.
 */
l2sw_api_ret_t l2sw_qos_priMap_set(l2sw_uint8 units, l2sw_queue_num_t queue_num, l2sw_qos_pri2queue_t *pPri2qid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_priMap_set(units, queue_num, pPri2qid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_priMap_get
 * Description:
 *      Get priority to queue ID mapping table parameters.
 * Input:
 *      queue_num - Queue number usage.
 * Output:
 *      pPri2qid - Priority mapping to queue ID.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_QUEUE_NUM    - Invalid queue number.
 * Note:
 *      The API can return the mapping queue id of the specified priority and queue number.
 *      The queue number is from 1 to 8.
 */
l2sw_api_ret_t l2sw_qos_priMap_get(l2sw_uint8 units, l2sw_queue_num_t queue_num, l2sw_qos_pri2queue_t *pPri2qid)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_priMap_get(units, queue_num, pPri2qid);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_schedulingQueue_set
 * Description:
 *      Set weight and type of queues in dedicated port.
 * Input:
 *      port        - Port id.
 *      pQweights   - The array of weights for WRR/WFQ queue (0 for STRICT_PRIORITY queue).
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_QOS_QUEUE_WEIGHT - Invalid queue weight.
 * Note:
 *      The API can set weight and type, strict priority or weight fair queue (WFQ) for
 *      dedicated port for using queues. If queue id is not included in queue usage,
 *      then its type and weight setting in dummy for setting. There are priorities
 *      as queue id in strict queues. It means strict queue id 5 carrying higher priority
 *      than strict queue id 4. The WFQ queue weight is from 1 to 127, and weight 0 is
 *      for strict priority queue type.
 */
l2sw_api_ret_t l2sw_qos_schedulingQueue_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_queue_weights_t *pQweights)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_schedulingQueue_set(units, port, pQweights);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_schedulingQueue_get
 * Description:
 *      Get weight and type of queues in dedicated port.
 * Input:
 *      port - Port id.
 * Output:
 *      pQweights - The array of weights for WRR/WFQ queue (0 for STRICT_PRIORITY queue).
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get weight and type, strict priority or weight fair queue (WFQ) for dedicated port for using queues.
 *      The WFQ queue weight is from 1 to 127, and weight 0 is for strict priority queue type.
 */
l2sw_api_ret_t l2sw_qos_schedulingQueue_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_queue_weights_t *pQweights)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_schedulingQueue_get(units, port, pQweights);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pRemarkEnable_set
 * Description:
 *      Set 1p Remarking state
 * Input:
 *      port        - Port id.
 *      enable      - State of per-port 1p Remarking
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 *      L2SW_ERR_ENABLE       - Invalid enable parameter.
 * Note:
 *      The API can enable or disable 802.1p remarking ability for whole system.
 *      The status of 802.1p remark:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_qos_1pRemarkEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemarkEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pRemarkEnable_get
 * Description:
 *      Get 802.1p remarking ability.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - Status of 802.1p remark.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get 802.1p remarking ability.
 *      The status of 802.1p remark:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_qos_1pRemarkEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemarkEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pRemark_set
 * Description:
 *      Set 802.1p remarking parameter.
 * Input:
 *      int_pri     - Internal priority value.
 *      dot1p_pri   - 802.1p priority value.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_VLAN_PRIORITY    - Invalid 1p priority.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      The API can set 802.1p parameters source priority and new priority.
 */
l2sw_api_ret_t l2sw_qos_1pRemark_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t dot1p_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemark_set(units, int_pri, dot1p_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pRemark_get
 * Description:
 *      Get 802.1p remarking parameter.
 * Input:
 *      int_pri - Internal priority value.
 * Output:
 *      pDot1p_pri - 802.1p priority value.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      The API can get 802.1p remarking parameters. It would return new priority of ingress priority.
 */
l2sw_api_ret_t l2sw_qos_1pRemark_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_pri_t *pDot1p_pri)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemark_get(units, int_pri, pDot1p_pri);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_qos_1pRemarkSrcSel_set
 * Description:
 *      Set remarking source of 802.1p remarking.
 * Input:
 *      type      - remarking source
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID  - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter

 * Note:
 *      The API can configure 802.1p remark functionality to map original 802.1p value or internal
 *      priority to TX DSCP value.
 */
l2sw_api_ret_t l2sw_qos_1pRemarkSrcSel_set(l2sw_uint8 units, l2sw_qos_1pRmkSrc_t type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemarkSrcSel_set(units, type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_1pRemarkSrcSel_get
 * Description:
 *      Get remarking source of 802.1p remarking.
 * Input:
 *      none
 * Output:
 *      pType      - remarking source
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter
 *      L2SW_ERR_NULL_POINTER     - input parameter may be null pointer

 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_qos_1pRemarkSrcSel_get(l2sw_uint8 units, l2sw_qos_1pRmkSrc_t *pType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_1pRemarkSrcSel_get(units, pType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemarkEnable_set
 * Description:
 *      Set DSCP remarking ability.
 * Input:
 *      port    - Port id.
 *      enable  - status of DSCP remark.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 *      L2SW_ERR_ENABLE           - Invalid enable parameter.
 * Note:
 *      The API can enable or disable DSCP remarking ability for whole system.
 *      The status of DSCP remark:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_qos_dscpRemarkEnable_set(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t enable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemarkEnable_set(units, port, enable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemarkEnable_get
 * Description:
 *      Get DSCP remarking ability.
 * Input:
 *      port - Port id.
 * Output:
 *      pEnable - status of DSCP remarking.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get DSCP remarking ability.
 *      The status of DSCP remark:
 *      - DISABLED
 *      - ENABLED
 */
l2sw_api_ret_t l2sw_qos_dscpRemarkEnable_get(l2sw_uint8 units, l2sw_port_t port, l2sw_enable_t *pEnable)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemarkEnable_get(units, port, pEnable);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemark_set
 * Description:
 *      Set DSCP remarking parameter.
 * Input:
 *      int_pri - Internal priority value.
 *      dscp    - DSCP value.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid DSCP value.
 * Note:
 *      The API can set DSCP value and mapping priority.
 */
l2sw_api_ret_t l2sw_qos_dscpRemark_set(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_dscp_t dscp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemark_set(units, int_pri, dscp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemark_get
 * Description:
 *      Get DSCP remarking parameter.
 * Input:
 *      int_pri - Internal priority value.
 * Output:
 *      Dscp - DSCP value.
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_QOS_INT_PRIORITY - Invalid priority.
 * Note:
 *      The API can get DSCP parameters. It would return DSCP value for mapping priority.
 */
l2sw_api_ret_t l2sw_qos_dscpRemark_get(l2sw_uint8 units, l2sw_pri_t int_pri, l2sw_dscp_t *pDscp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemark_get(units, int_pri, pDscp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemarkSrcSel_set
 * Description:
 *      Set remarking source of DSCP remarking.
 * Input:
 *      type      - remarking source
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID  - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter

 * Note:
 *      The API can configure DSCP remark functionality to map original DSCP value or internal
 *      priority to TX DSCP value.
 */
l2sw_api_ret_t l2sw_qos_dscpRemarkSrcSel_set(l2sw_uint8 units, l2sw_qos_dscpRmkSrc_t type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemarkSrcSel_set(units, type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemarkSrcSel_get
 * Description:
 *      Get remarking source of DSCP remarking.
 * Input:
 *      none
 * Output:
 *      pType      - remarking source
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_NOT_INIT         - The module is not initial
 *      L2SW_ERR_PORT_ID          - invalid port id
 *      L2SW_ERR_INPUT            - invalid input parameter
 *      L2SW_ERR_NULL_POINTER     - input parameter may be null pointer

 * Note:
 *      None
 */
l2sw_api_ret_t l2sw_qos_dscpRemarkSrcSel_get(l2sw_uint8 units, l2sw_qos_dscpRmkSrc_t *pType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemarkSrcSel_get(units, pType);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemark2Dscp_set
 * Description:
 *      Set DSCP to remarked DSCP mapping.
 * Input:
 *      dscp    - DSCP value
 *      rmkDscp - remarked DSCP value
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid dscp value
 * Note:
 *      dscp parameter can be DSCP value or internal priority according to configuration of API
 *      dal_apollomp_qos_dscpRemarkSrcSel_set(), because DSCP remark functionality can map original DSCP
 *      value or internal priority to TX DSCP value.
 */
l2sw_api_ret_t l2sw_qos_dscpRemark2Dscp_set(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_dscp_t rmkDscp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemark2Dscp_set(units, dscp, rmkDscp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_dscpRemark2Dscp_get
 * Description:
 *      Get DSCP to remarked DSCP mapping.
 * Input:
 *      dscp    - DSCP value
 * Output:
 *      pDscp   - remarked DSCP value
 * Return:
 *      L2SW_ERR_OK
 *      L2SW_ERR_FAILED
 *      L2SW_ERR_QOS_DSCP_VALUE   - Invalid dscp value
 *      L2SW_ERR_NULL_POINTER     - NULL pointer
 * Note:
 *      None.
 */
l2sw_api_ret_t l2sw_qos_dscpRemark2Dscp_get(l2sw_uint8 units, l2sw_dscp_t dscp, l2sw_dscp_t *pDscp)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_dscpRemark2Dscp_get(units, dscp, pDscp);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_portPriSelIndex_set
 * Description:
 *      Configure priority decision index to each port.
 * Input:
 *      port - Port id.
 *      index - priority decision index.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK               - OK
 *      L2SW_ERR_FAILED           - Failed
 *      L2SW_ERR_SMI              - SMI access error
 *      L2SW_ERR_PORT_ID          - Invalid port number.
 *      L2SW_ERR_ENTRY_INDEX - Invalid entry index.
 * Note:
 *      The API can set priority of port assignments for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_portPriSelIndex_set(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_priDecTbl_t index)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_portPriSelIndex_set(units, port, index);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_qos_portPriSelIndex_get
 * Description:
 *      Get priority decision index from each port.
 * Input:
 *      port - Port id.
 * Output:
 *      pIndex - priority decision index.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_PORT_ID      - Invalid port number.
 * Note:
 *      The API can get priority of port assignments for queue usage and packet scheduling.
 */
l2sw_api_ret_t l2sw_qos_portPriSelIndex_get(l2sw_uint8 units, l2sw_port_t port, l2sw_qos_priDecTbl_t *pIndex)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_qos_portPriSelIndex_get(units, port, pIndex);
    L2SW_API_UNLOCK(units);

    return retVal;
}

