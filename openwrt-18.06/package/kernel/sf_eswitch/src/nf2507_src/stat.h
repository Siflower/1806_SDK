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

#ifndef __L2SW_API_STAT_H__
#define __L2SW_API_STAT_H__

/*
 * Data Type Declaration
 */
typedef l2sw_u_long_t l2sw_stat_counter_t;

/* global statistic counter structure */
typedef struct l2sw_stat_global_cntr_s
{
    l2sw_uint64 dot1dTpLearnedEntryDiscards;
}l2sw_stat_global_cntr_t;

typedef enum l2sw_stat_global_type_e
{
    DOT1D_TP_LEARNED_ENTRY_DISCARDS_INDEX = 58,
    MIB_GLOBAL_CNTR_END
}l2sw_stat_global_type_t;

/* port statistic counter structure */
typedef struct l2sw_stat_port_cntr_s
{
    l2sw_uint64 ifInOctets;
    l2sw_uint32 dot3StatsFCSErrors;
    l2sw_uint32 dot3StatsSymbolErrors;
    l2sw_uint32 dot3InPauseFrames;
    l2sw_uint32 dot3ControlInUnknownOpcodes;
    l2sw_uint32 etherStatsFragments;
    l2sw_uint32 etherStatsJabbers;
    l2sw_uint32 ifInUcastPkts;
    l2sw_uint32 etherStatsDropEvents;
    l2sw_uint64 etherStatsOctets;
    l2sw_uint32 etherStatsUndersizePkts;
    l2sw_uint32 etherStatsOversizePkts;
    l2sw_uint32 etherStatsPkts64Octets;
    l2sw_uint32 etherStatsPkts65to127Octets;
    l2sw_uint32 etherStatsPkts128to255Octets;
    l2sw_uint32 etherStatsPkts256to511Octets;
    l2sw_uint32 etherStatsPkts512to1023Octets;
    l2sw_uint32 etherStatsPkts1024toMaxOctets;
    l2sw_uint32 etherStatsMcastPkts;
    l2sw_uint32 etherStatsBcastPkts;
    l2sw_uint64 ifOutOctets;
    l2sw_uint32 dot3StatsSingleCollisionFrames;
    l2sw_uint32 dot3StatsMultipleCollisionFrames;
    l2sw_uint32 dot3StatsDeferredTransmissions;
    l2sw_uint32 dot3StatsLateCollisions;
    l2sw_uint32 etherStatsCollisions;
    l2sw_uint32 dot3StatsExcessiveCollisions;
    l2sw_uint32 dot3OutPauseFrames;
    l2sw_uint32 dot1dBasePortDelayExceededDiscards;
    l2sw_uint32 dot1dTpPortInDiscards;
    l2sw_uint32 ifOutUcastPkts;
    l2sw_uint32 ifOutMulticastPkts;
    l2sw_uint32 ifOutBrocastPkts;
    l2sw_uint32 outOampduPkts;
    l2sw_uint32 inOampduPkts;
    l2sw_uint32 pktgenPkts;
    l2sw_uint32 inMldChecksumError;
    l2sw_uint32 inIgmpChecksumError;
    l2sw_uint32 inMldSpecificQuery;
    l2sw_uint32 inMldGeneralQuery;
    l2sw_uint32 inIgmpSpecificQuery;
    l2sw_uint32 inIgmpGeneralQuery;
    l2sw_uint32 inMldLeaves;
    l2sw_uint32 inIgmpLeaves;
    l2sw_uint32 inIgmpJoinsSuccess;
    l2sw_uint32 inIgmpJoinsFail;
    l2sw_uint32 inMldJoinsSuccess;
    l2sw_uint32 inMldJoinsFail;
    l2sw_uint32 inReportSuppressionDrop;
    l2sw_uint32 inLeaveSuppressionDrop;
    l2sw_uint32 outIgmpReports;
    l2sw_uint32 outIgmpLeaves;
    l2sw_uint32 outIgmpGeneralQuery;
    l2sw_uint32 outIgmpSpecificQuery;
    l2sw_uint32 outMldReports;
    l2sw_uint32 outMldLeaves;
    l2sw_uint32 outMldGeneralQuery;
    l2sw_uint32 outMldSpecificQuery;
    l2sw_uint32 inKnownMulticastPkts;
    l2sw_uint32 ifInMulticastPkts;
    l2sw_uint32 ifInBroadcastPkts;
    l2sw_uint32 ifOutDiscards;
}l2sw_stat_port_cntr_t;

/* port statistic counter index */
typedef enum l2sw_stat_port_type_e
{
    STAT_IfInOctets = 0,
    STAT_Dot3StatsFCSErrors,
    STAT_Dot3StatsSymbolErrors,
    STAT_Dot3InPauseFrames,
    STAT_Dot3ControlInUnknownOpcodes,
    STAT_EtherStatsFragments,
    STAT_EtherStatsJabbers,
    STAT_IfInUcastPkts,
    STAT_EtherStatsDropEvents,
    STAT_EtherStatsOctets,
    STAT_EtherStatsUnderSizePkts,
    STAT_EtherOversizeStats,
    STAT_EtherStatsPkts64Octets,
    STAT_EtherStatsPkts65to127Octets,
    STAT_EtherStatsPkts128to255Octets,
    STAT_EtherStatsPkts256to511Octets,
    STAT_EtherStatsPkts512to1023Octets,
    STAT_EtherStatsPkts1024to1518Octets,
    STAT_EtherStatsMulticastPkts,
    STAT_EtherStatsBroadcastPkts,
    STAT_IfOutOctets,
    STAT_Dot3StatsSingleCollisionFrames,
    STAT_Dot3StatsMultipleCollisionFrames,
    STAT_Dot3StatsDeferredTransmissions,
    STAT_Dot3StatsLateCollisions,
    STAT_EtherStatsCollisions,
    STAT_Dot3StatsExcessiveCollisions,
    STAT_Dot3OutPauseFrames,
    STAT_Dot1dBasePortDelayExceededDiscards,
    STAT_Dot1dTpPortInDiscards,
    STAT_IfOutUcastPkts,
    STAT_IfOutMulticastPkts,
    STAT_IfOutBroadcastPkts,
    STAT_OutOampduPkts,
    STAT_InOampduPkts,
    STAT_PktgenPkts,
    STAT_InMldChecksumError,
    STAT_InIgmpChecksumError,
    STAT_InMldSpecificQuery,
    STAT_InMldGeneralQuery,
    STAT_InIgmpSpecificQuery,
    STAT_InIgmpGeneralQuery,
    STAT_InMldLeaves,
    STAT_InIgmpInterfaceLeaves,
    STAT_InIgmpJoinsSuccess,
    STAT_InIgmpJoinsFail,
    STAT_InMldJoinsSuccess,
    STAT_InMldJoinsFail,
    STAT_InReportSuppressionDrop,
    STAT_InLeaveSuppressionDrop,
    STAT_OutIgmpReports,
    STAT_OutIgmpLeaves,
    STAT_OutIgmpGeneralQuery,
    STAT_OutIgmpSpecificQuery,
    STAT_OutMldReports,
    STAT_OutMldLeaves,
    STAT_OutMldGeneralQuery,
    STAT_OutMldSpecificQuery,
    STAT_InKnownMulticastPkts,
    STAT_IfInMulticastPkts,
    STAT_IfInBroadcastPkts,
    STAT_IfOutDiscards,
    STAT_PORT_CNTR_END
}l2sw_stat_port_type_t;
    
typedef enum l2sw_stat_port5_type_e{

    STAT5_inmcursvoctets = 0,
    STAT5_outmcusendoctets,
    
    STAT5_inrldprsvoctets,
    STAT5_outrldpsendoctets,
    
    STAT5_outmcursvpkts,
    STAT5_outmcusendpkts,
    STAT5_outrldprsvpkts,
    STAT5_outrldpsendpkts,
    STAT5_reserved1,
    
    STAT5_InIgmpSpecificQuery,
    STAT5_InIgmpInterfaceleaves,
    STAT5_InIgmpJoinsSuccess,
    STAT5_InIgmpJoinsFail,
    STAT5_InIgmpGeneralQuery,
    STAT5_InIgmpChecksumError,
    STAT5_reserved2,
    
    STAT5_InmldSpecificQuery,
    STAT5_InmldInterfaceleaves,
    STAT5_InmldJoinsSuccess,
    STAT5_InmldJoinsFail,
    STAT5_InmldGeneralQuery,
    STAT5_InmldchecksumError,
    STAT5_reserved3,
    
    STAT5_InKnownMulticastPkts,
    STAT5_InOampduPkts,
    STAT5_InReportSuppressionDrop,
    STAT5_InLeaveSuppressionDrop,
    STAT5_reserved4,
    
    STAT5_OutmldSpecificQuery,
    STAT5_OutmldReports,
    STAT5_OutmldLeaves,
    STAT5_OutmldGeneralQuery,
    STAT5_OutIgmpSpecificQuery,
    STAT5_OutIgmpReports,
    STAT5_OutIgmpLeaves,
    STAT5_OutIgmpGeneralQuery,
    STAT5_outOampduPkts1,
    STAT5_reserved5,
    
    STAT_L2SW_MIBCOUNTER_NUMBER,

}l2sw_stat_port5_type_t;

typedef enum l2sw_logging_counter_mode_e
{
    LOGGING_MODE_32BIT = 0,
    LOGGING_MODE_64BIT,
    LOGGING_MODE_END
}l2sw_logging_counter_mode_t;

typedef enum l2sw_logging_counter_type_e
{
    LOGGING_TYPE_PACKET = 0,
    LOGGING_TYPE_BYTE,
    LOGGING_TYPE_END
}l2sw_logging_counter_type_t;

typedef enum l2sw_stat_lengthMode_e
{
    LENGTH_MODE_EXC_TAG = 0,
    LENGTH_MODE_INC_TAG,
    LENGTH_MODE_END
}l2sw_stat_lengthMode_t;



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
extern l2sw_api_ret_t l2sw_stat_global_reset(l2sw_uint8 units);

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
extern l2sw_api_ret_t l2sw_stat_port_reset(l2sw_uint8 units, l2sw_port_t port);

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
extern l2sw_api_ret_t l2sw_stat_queueManage_reset(l2sw_uint8 units);

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
extern l2sw_api_ret_t l2sw_stat_global_get(l2sw_uint8 units, l2sw_stat_global_type_t cntr_idx, l2sw_stat_counter_t *pCntr);

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
extern l2sw_api_ret_t l2sw_stat_global_getAll(l2sw_uint8 units, l2sw_stat_global_cntr_t *pGlobal_cntrs);

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
extern l2sw_api_ret_t l2sw_stat_port_get(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_type_t cntr_idx, l2sw_stat_counter_t *pCntr);

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
extern l2sw_api_ret_t l2sw_stat_port_getAll(l2sw_uint8 units, l2sw_port_t port, l2sw_stat_port_cntr_t *pPort_cntrs);

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
extern l2sw_api_ret_t l2sw_stat_logging_counterCfg_set(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t mode, l2sw_logging_counter_type_t type);

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
extern l2sw_api_ret_t l2sw_stat_logging_counterCfg_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_logging_counter_mode_t *pMode, l2sw_logging_counter_type_t *pType);

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
extern l2sw_api_ret_t l2sw_stat_logging_counter_reset(l2sw_uint8 units, l2sw_uint32 idx);

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
extern l2sw_api_ret_t l2sw_stat_logging_counter_get(l2sw_uint8 units, l2sw_uint32 idx, l2sw_uint32 *pCnt);

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
extern l2sw_api_ret_t l2sw_stat_lengthMode_set(l2sw_uint8 units, l2sw_stat_lengthMode_t txMode, l2sw_stat_lengthMode_t rxMode);

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
extern l2sw_api_ret_t l2sw_stat_lengthMode_get(l2sw_uint8 units, l2sw_stat_lengthMode_t *pTxMode, l2sw_stat_lengthMode_t *pRxMode);

#endif /* __L2SW_API_STAT_H__ */

