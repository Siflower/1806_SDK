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
#include "stat.h"
#include "sal_string.h"
#include "l2sw_asicdrv_mib.h"

#define MIB_NOT_SUPPORT     (0xFFFF)
static l2sw_api_ret_t _get_asic_mib_idx(l2sw_uint8 units, l2sw_stat_port_type_t cnt_idx, L2SW_MIBCOUNTER *pMib_idx)
{
    L2SW_MIBCOUNTER mib_asic_idx[STAT_PORT_CNTR_END]=
    {
        ifInOctets,                     /* STAT_IfInOctets */
        dot3StatsFCSErrors,             /* STAT_Dot3StatsFCSErrors */
        dot3StatsSymbolErrors,          /* STAT_Dot3StatsSymbolErrors */
        dot3InPauseFrames,              /* STAT_Dot3InPauseFrames */
        dot3ControlInUnknownOpcodes,    /* STAT_Dot3ControlInUnknownOpcodes */
        etherStatsFragments,            /* STAT_EtherStatsFragments */
        etherStatsJabbers,              /* STAT_EtherStatsJabbers */
        ifInUcastPkts,                  /* STAT_IfInUcastPkts */
        etherStatsDropEvents,           /* STAT_EtherStatsDropEvents */
        etherStatsOctets,               /* STAT_EtherStatsOctets */
        etherStatsUnderSizePkts,        /* STAT_EtherStatsUnderSizePkts */
        etherOversizeStats,             /* STAT_EtherOversizeStats */
        etherStatsPkts64Octets,         /* STAT_EtherStatsPkts64Octets */
        etherStatsPkts65to127Octets,    /* STAT_EtherStatsPkts65to127Octets */
        etherStatsPkts128to255Octets,   /* STAT_EtherStatsPkts128to255Octets */
        etherStatsPkts256to511Octets,   /* STAT_EtherStatsPkts256to511Octets */
        etherStatsPkts512to1023Octets,  /* STAT_EtherStatsPkts512to1023Octets */
        etherStatsPkts1024to1518Octets, /* STAT_EtherStatsPkts1024to1518Octets */
        ifInMulticastPkts,              /* STAT_EtherStatsMulticastPkts */
        ifInBroadcastPkts,              /* STAT_EtherStatsBroadcastPkts */
        ifOutOctets,                    /* STAT_IfOutOctets */
        dot3StatsSingleCollisionFrames, /* STAT_Dot3StatsSingleCollisionFrames */
        dot3StatMultipleCollisionFrames,/* STAT_Dot3StatsMultipleCollisionFrames */
        dot3sDeferredTransmissions,     /* STAT_Dot3StatsDeferredTransmissions */
        dot3StatsLateCollisions,        /* STAT_Dot3StatsLateCollisions */
        etherStatsCollisions,           /* STAT_EtherStatsCollisions */
        dot3StatsExcessiveCollisions,   /* STAT_Dot3StatsExcessiveCollisions */
        dot3OutPauseFrames,             /* STAT_Dot3OutPauseFrames */
        MIB_NOT_SUPPORT,                /* STAT_Dot1dBasePortDelayExceededDiscards */
        dot1dTpPortInDiscards,          /* STAT_Dot1dTpPortInDiscards */
        ifOutUcastPkts,                 /* STAT_IfOutUcastPkts */
        ifOutMulticastPkts,             /* STAT_IfOutMulticastPkts */
        ifOutBroadcastPkts,             /* STAT_IfOutBroadcastPkts */
        outOampduPkts,                  /* STAT_OutOampduPkts */
        inOampduPkts,                   /* STAT_InOampduPkts */
        MIB_NOT_SUPPORT,                /* STAT_PktgenPkts */
        inMldChecksumError,             /* STAT_InMldChecksumError */
        inIgmpChecksumError,            /* STAT_InIgmpChecksumError */
        inMldSpecificQuery,             /* STAT_InMldSpecificQuery */
        inMldGeneralQuery,              /* STAT_InMldGeneralQuery */
        inIgmpSpecificQuery,            /* STAT_InIgmpSpecificQuery */
        inIgmpGeneralQuery,             /* STAT_InIgmpGeneralQuery */
        inMldLeaves,                    /* STAT_InMldLeaves */
        inIgmpLeaves,                   /* STAT_InIgmpInterfaceLeaves */
        inIgmpJoinsSuccess,             /* STAT_InIgmpJoinsSuccess */
        inIgmpJoinsFail,                /* STAT_InIgmpJoinsFail */
        inMldJoinsSuccess,              /* STAT_InMldJoinsSuccess */
        inMldJoinsFail,                 /* STAT_InMldJoinsFail */
        inReportSuppressionDrop,        /* STAT_InReportSuppressionDrop */
        inLeaveSuppressionDrop,         /* STAT_InLeaveSuppressionDrop */
        outIgmpReports,                 /* STAT_OutIgmpReports */
        outIgmpLeaves,                  /* STAT_OutIgmpLeaves */
        outIgmpGeneralQuery,            /* STAT_OutIgmpGeneralQuery */
        outIgmpSpecificQuery,           /* STAT_OutIgmpSpecificQuery */
        outMldReports,                  /* STAT_OutMldReports */
        outMldLeaves,                   /* STAT_OutMldLeaves */
        outMldGeneralQuery,             /* STAT_OutMldGeneralQuery */
        outMldSpecificQuery,            /* STAT_OutMldSpecificQuery */
        inKnownMulticastPkts,           /* STAT_InKnownMulticastPkts */
        ifInMulticastPkts,              /* STAT_IfInMulticastPkts */
        ifInBroadcastPkts,              /* STAT_IfInBroadcastPkts */
        ifOutDiscards                   /* STAT_IfOutDiscards */
    };

    if(cnt_idx >= STAT_PORT_CNTR_END)
        return L2SW_ERR_STAT_INVALID_PORT_CNTR;

    if(mib_asic_idx[cnt_idx] == MIB_NOT_SUPPORT)
        return L2SW_ERR_CHIP_NOT_SUPPORTED;

    *pMib_idx = mib_asic_idx[cnt_idx];
    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_global_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicMIBsCounterReset(units, L2SW_TRUE,L2SW_FALSE, 0)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_port_reset(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if ((retVal = l2sw_setAsicMIBsCounterReset(units, L2SW_FALSE,L2SW_FALSE,1 << l2sw_switch_port_L2P_get(units, port))) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_queueManage_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if ((retVal = l2sw_setAsicMIBsCounterReset(units, L2SW_FALSE,L2SW_TRUE, 0)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_global_get(l2sw_uint8 units, l2sw_stat_global_type_t cntr_idx, l2sw_stat_counter_t *pCntr)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pCntr)
        return L2SW_ERR_NULL_POINTER;

    if (cntr_idx!=DOT1D_TP_LEARNED_ENTRY_DISCARDS_INDEX)
        return L2SW_ERR_STAT_INVALID_GLOBAL_CNTR;

    if ((retVal = l2sw_getAsicMIBsCounter(units, 0, dot1dTpLearnedEntryDiscards, pCntr)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_global_getAll(l2sw_uint8 units, l2sw_stat_global_cntr_t *pGlobal_cntrs)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pGlobal_cntrs)
        return L2SW_ERR_NULL_POINTER;

    if ((retVal = l2sw_getAsicMIBsCounter(units, 0, DOT1D_TP_LEARNED_ENTRY_DISCARDS_INDEX, &pGlobal_cntrs->dot1dTpLearnedEntryDiscards)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_port_get(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_type_t cntr_idx, l2sw_stat_counter_t *pCntr)
{
    l2sw_api_ret_t       retVal;
    L2SW_MIBCOUNTER mib_idx;
    l2sw_stat_counter_t  second_cnt;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pCntr)
        return L2SW_ERR_NULL_POINTER;

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    if (cntr_idx>=STAT_PORT_CNTR_END)
        return L2SW_ERR_STAT_INVALID_PORT_CNTR;

    if((retVal = _get_asic_mib_idx(units, cntr_idx, &mib_idx)) != L2SW_ERR_OK)
        return retVal;

    if(mib_idx == MIB_NOT_SUPPORT)
        return L2SW_ERR_CHIP_NOT_SUPPORTED;

    if ((retVal = l2sw_getAsicMIBsCounter(units, l2sw_switch_port_L2P_get(units, port), mib_idx, pCntr)) != L2SW_ERR_OK)
        return retVal;

    if(cntr_idx == STAT_EtherStatsMulticastPkts)
    {
        if((retVal = _get_asic_mib_idx(units, STAT_IfOutMulticastPkts, &mib_idx)) != L2SW_ERR_OK)
            return retVal;

        if((retVal = l2sw_getAsicMIBsCounter(units, l2sw_switch_port_L2P_get(units, port), mib_idx, &second_cnt)) != L2SW_ERR_OK)
            return retVal;

        *pCntr += second_cnt;
    }

    if(cntr_idx == STAT_EtherStatsBroadcastPkts)
    {
        if((retVal = _get_asic_mib_idx(units, STAT_IfOutBroadcastPkts, &mib_idx)) != L2SW_ERR_OK)
            return retVal;

        if((retVal = l2sw_getAsicMIBsCounter(units, l2sw_switch_port_L2P_get(units, port), mib_idx, &second_cnt)) != L2SW_ERR_OK)
            return retVal;

        *pCntr += second_cnt;
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_port_getAll(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_cntr_t *pPort_cntrs)
{
    l2sw_api_ret_t retVal;
    l2sw_uint32 mibIndex;
    l2sw_uint64 mibCounter;
    l2sw_uint32 *accessPtr;
    /* address offset to MIBs counter */
    CONST_T l2sw_uint16 mibLength[STAT_PORT_CNTR_END]= {
        2,1,1,1,1,1,1,1,1,
        2,1,1,1,1,1,1,1,1,1,1,
        2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pPort_cntrs)
        return L2SW_ERR_NULL_POINTER;

    /* Check port valid */
    L2SW_CHK_PORT_VALID(units, port);

    accessPtr = (l2sw_uint32*)pPort_cntrs;
    for (mibIndex=0;mibIndex<STAT_PORT_CNTR_END;mibIndex++)
    {
        if ((retVal = _l2sw_stat_port_get(units, port, mibIndex, &mibCounter)) != L2SW_ERR_OK)
        {
            if (retVal == L2SW_ERR_CHIP_NOT_SUPPORTED)
                mibCounter = 0;
            else
                return retVal;
        }

        if (2 == mibLength[mibIndex])
            *(l2sw_uint64*)accessPtr = mibCounter;
        else if (1 == mibLength[mibIndex])
            *accessPtr = mibCounter;
        else
            return L2SW_ERR_FAILED;

        accessPtr+=mibLength[mibIndex];
    }

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_logging_counterCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t mode, l2sw_logging_counter_type_t type)
{
    l2sw_api_ret_t retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(idx > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((idx % 2) == 1)
        return L2SW_ERR_INPUT;

    if(mode >= LOGGING_MODE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if(type >= LOGGING_TYPE_END)
        return L2SW_ERR_OUT_OF_RANGE;

    if((retVal = l2sw_setAsicMIBsLoggingType(units, (idx / 2), (l2sw_uint32)type)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_setAsicMIBsLoggingMode(units, (idx / 2), (l2sw_uint32)mode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_logging_counterCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t *pMode, l2sw_logging_counter_type_t *pType)
{
    l2sw_api_ret_t   retVal;
    l2sw_uint32      type, mode;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(idx > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((idx % 2) == 1)
        return L2SW_ERR_INPUT;

    if(pMode == NULL)
        return L2SW_ERR_NULL_POINTER;

    if(pType == NULL)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicMIBsLoggingType(units, (idx / 2), &type)) != L2SW_ERR_OK)
        return retVal;

    if((retVal = l2sw_getAsicMIBsLoggingMode(units, (idx / 2), &mode)) != L2SW_ERR_OK)
        return retVal;

    *pMode = (l2sw_logging_counter_mode_t)mode;
    *pType = (l2sw_logging_counter_type_t)type;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_logging_counter_reset(l2sw_uint8 units, l2sw_uint32 idx)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(idx > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((retVal = l2sw_setAsicMIBsResetLoggingCounter(units, idx)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_logging_counter_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_uint32 *pCnt)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pCnt)
        return L2SW_ERR_NULL_POINTER;

    if(idx > L2SW_MIB_MAX_LOG_CNT_IDX)
        return L2SW_ERR_OUT_OF_RANGE;

    if((retVal = l2sw_getAsicMIBsLogCounter(units, idx, pCnt)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_lengthMode_set(l2sw_uint8 units, l2sw_stat_lengthMode_t txMode, l2sw_stat_lengthMode_t rxMode)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(txMode >= LENGTH_MODE_END)
        return L2SW_ERR_INPUT;

    if(rxMode >= LENGTH_MODE_END)
        return L2SW_ERR_INPUT;

    if((retVal = l2sw_setAsicMIBsLength(units, (l2sw_uint32)txMode, (l2sw_uint32)rxMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}

static l2sw_api_ret_t _l2sw_stat_lengthMode_get(l2sw_uint8 units, l2sw_stat_lengthMode_t *pTxMode, l2sw_stat_lengthMode_t *pRxMode)
{
    l2sw_api_ret_t   retVal;

    /* Check initialization state */
    L2SW_CHK_INIT_STATE(units);

    if(NULL == pTxMode)
        return L2SW_ERR_NULL_POINTER;

    if(NULL == pRxMode)
        return L2SW_ERR_NULL_POINTER;

    if((retVal = l2sw_getAsicMIBsLength(units, (l2sw_uint32 *)pTxMode, (l2sw_uint32 *)pRxMode)) != L2SW_ERR_OK)
        return retVal;

    return L2SW_ERR_OK;
}


/* Function Name:
 *      l2sw_stat_global_reset
 * Description:
 *      Reset global MIB counter.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Reset MIB counter of ports. API will use global reset while port mask is all-ports.
 */
l2sw_api_ret_t l2sw_stat_global_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_global_reset(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_port_reset
 * Description:
 *      Reset per port MIB counter by port.
 * Input:
 *      port - port id.
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_stat_port_reset(l2sw_uint8 units, l2sw_port_t port)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_port_reset(units, port);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_queueManage_reset
 * Description:
 *      Reset queue manage MIB counter.
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_stat_queueManage_reset(l2sw_uint8 units)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_queueManage_reset(units);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_stat_global_get
 * Description:
 *      Get global MIB counter
 * Input:
 *      cntr_idx - global counter index.
 * Output:
 *      pCntr - global counter value.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      Get global MIB counter by index definition.
 */
l2sw_api_ret_t l2sw_stat_global_get(l2sw_uint8 units, l2sw_stat_global_type_t cntr_idx, l2sw_stat_counter_t *pCntr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_global_get(units, cntr_idx, pCntr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_global_getAll
 * Description:
 *      Get all global MIB counter
 * Input:
 *      None
 * Output:
 *      pGlobal_cntrs - global counter structure.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      Get all global MIB counter by index definition.
 */
l2sw_api_ret_t l2sw_stat_global_getAll(l2sw_uint8 units, l2sw_stat_global_cntr_t *pGlobal_cntrs)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_global_getAll(units, pGlobal_cntrs);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_port_get
 * Description:
 *      Get per port MIB counter by index
 * Input:
 *      port        - port id.
 *      cntr_idx    - port counter index.
 * Output:
 *      pCntr - MIB retrived counter.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Get per port MIB counter by index definition.
 */
l2sw_api_ret_t l2sw_stat_port_get(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_type_t cntr_idx, l2sw_stat_counter_t *pCntr)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_port_get(units, port, cntr_idx, pCntr);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_port_getAll
 * Description:
 *      Get all counters of one specified port in the specified device.
 * Input:
 *      port - port id.
 * Output:
 *      pPort_cntrs - buffer pointer of counter value.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      Get all MIB counters of one port.
 */
l2sw_api_ret_t l2sw_stat_port_getAll(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_cntr_t *pPort_cntrs)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_port_getAll(units, port, pPort_cntrs);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_logging_counterCfg_set
 * Description:
 *      Set the type and mode of Logging Counter
 * Input:
 *      idx     - The index of Logging Counter. Should be even number only.(0,2,4,6,8.....30)
 *      mode    - 32 bits or 64 bits mode
 *      type    - Packet counter or byte counter
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_OUT_OF_RANGE - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      Set the type and mode of Logging Counter.
 */
l2sw_api_ret_t l2sw_stat_logging_counterCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t mode, l2sw_logging_counter_type_t type)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_logging_counterCfg_set(units, idx, mode, type);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_logging_counterCfg_get
 * Description:
 *      Get the type and mode of Logging Counter
 * Input:
 *      idx     - The index of Logging Counter. Should be even number only.(0,2,4,6,8.....30)
 * Output:
 *      pMode   - 32 bits or 64 bits mode
 *      pType   - Packet counter or byte counter
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_OUT_OF_RANGE - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_NULL_POINTER - NULL Pointer
 *      L2SW_ERR_SMI          - SMI access error
 *      L2SW_ERR_INPUT        - Invalid input parameters.
 * Note:
 *      Get the type and mode of Logging Counter.
 */
l2sw_api_ret_t l2sw_stat_logging_counterCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t *pMode, l2sw_logging_counter_type_t *pType)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_logging_counterCfg_get(units, idx, pMode, pType);
    L2SW_API_UNLOCK(units);

    return retVal;
}


/* Function Name:
 *      l2sw_stat_logging_counter_reset
 * Description:
 *      Reset Logging Counter
 * Input:
 *      idx     - The index of Logging Counter. (0~31)
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_OUT_OF_RANGE - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Reset Logging Counter.
 */
l2sw_api_ret_t l2sw_stat_logging_counter_reset(l2sw_uint8 units, l2sw_uint32 idx)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_logging_counter_reset(units, idx);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_logging_counter_get
 * Description:
 *      Get Logging Counter
 * Input:
 *      idx     - The index of Logging Counter. (0~31)
 * Output:
 *      pCnt    - Logging counter value
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_OUT_OF_RANGE - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *      Get Logging Counter.
 */
l2sw_api_ret_t l2sw_stat_logging_counter_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_uint32 *pCnt)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_logging_counter_get(units, idx, pCnt);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_lengthMode_set
 * Description:
 *      Set Legnth mode.
 * Input:
 *      txMode     - The length counting mode
 *      rxMode     - The length counting mode
 * Output:
 *      None.
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_INPUT        - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 *
 */
l2sw_api_ret_t l2sw_stat_lengthMode_set(l2sw_uint8 units, l2sw_stat_lengthMode_t txMode, l2sw_stat_lengthMode_t rxMode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_lengthMode_set(units, txMode, rxMode);
    L2SW_API_UNLOCK(units);

    return retVal;
}

/* Function Name:
 *      l2sw_stat_lengthMode_get
 * Description:
 *      Get Legnth mode.
 * Input:
 *      None.
 * Output:
 *      pTxMode       - The length counting mode
 *      pRxMode       - The length counting mode
 * Return:
 *      L2SW_ERR_OK           - OK
 *      L2SW_ERR_INPUT        - Out of range.
 *      L2SW_ERR_FAILED       - Failed
 *      L2SW_ERR_SMI          - SMI access error
 * Note:
 */
l2sw_api_ret_t l2sw_stat_lengthMode_get(l2sw_uint8 units, l2sw_stat_lengthMode_t *pTxMode, l2sw_stat_lengthMode_t *pRxMode)
{
    l2sw_api_ret_t retVal;

    L2SW_API_LOCK(units);
    retVal = _l2sw_stat_lengthMode_get(units, pTxMode, pRxMode);
    L2SW_API_UNLOCK(units);

    return retVal;
}


