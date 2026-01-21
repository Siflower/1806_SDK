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

#ifndef _L2SW_ASICDRV_MIB_H_
#define _L2SW_ASICDRV_MIB_H_

#include "l2sw_asicdrv.h"

#define L2SW_MIB_PORT_OFFSET                (0x7C)
#define L2SW_MIB_LEARNENTRYDISCARD_OFFSET   (0x420)

#define L2SW_MAX_LOG_CNT_NUM                (32)
#define L2SW_MIB_MAX_LOG_CNT_IDX            (L2SW_MAX_LOG_CNT_NUM - 1)
#define L2SW_MIB_LOG_CNT_OFFSET             (0x3E0)
#define L2SW_MIB_MAX_LOG_MODE_IDX           (16-1)

typedef enum L2SW_MIBCOUNTER_E{

    /* RX */
    ifInOctets = 0,

    dot3StatsFCSErrors,
    dot3StatsSymbolErrors,
    dot3InPauseFrames,
    dot3ControlInUnknownOpcodes,

    etherStatsFragments,
    etherStatsJabbers,
    ifInUcastPkts,
    etherStatsDropEvents,

    ifInMulticastPkts,
    ifInBroadcastPkts,
    inMldChecksumError,
    inIgmpChecksumError,
    inMldSpecificQuery,
    inMldGeneralQuery,
    inIgmpSpecificQuery,
    inIgmpGeneralQuery,
    inMldLeaves,
    inIgmpLeaves,

    /* TX/RX */
    etherStatsOctets,

    etherStatsUnderSizePkts,
    etherOversizeStats,
    etherStatsPkts64Octets,
    etherStatsPkts65to127Octets,
    etherStatsPkts128to255Octets,
    etherStatsPkts256to511Octets,
    etherStatsPkts512to1023Octets,
    etherStatsPkts1024to1518Octets,

    /* TX */
    ifOutOctets,

    dot3StatsSingleCollisionFrames,
    dot3StatMultipleCollisionFrames,
    dot3sDeferredTransmissions,
    dot3StatsLateCollisions,
    etherStatsCollisions,
    dot3StatsExcessiveCollisions,
    dot3OutPauseFrames,
    ifOutDiscards,

    /* ALE */
    dot1dTpPortInDiscards,
    ifOutUcastPkts,
    ifOutMulticastPkts,
    ifOutBroadcastPkts,
    outOampduPkts,
    inOampduPkts,

    inIgmpJoinsSuccess,
    inIgmpJoinsFail,
    inMldJoinsSuccess,
    inMldJoinsFail,
    inReportSuppressionDrop,
    inLeaveSuppressionDrop,
    outIgmpReports,
    outIgmpLeaves,
    outIgmpGeneralQuery,
    outIgmpSpecificQuery,
    outMldReports,
    outMldLeaves,
    outMldGeneralQuery,
    outMldSpecificQuery,
    inKnownMulticastPkts,

    /*Device only */
    dot1dTpLearnedEntryDiscards,
    L2SW_MIBS_NUMBER,
    L2SW_MIBS_NUMBER_MIB_NOT_SUPPORT = 0xffff,/*add for avoid PC-LINT warning*/

}L2SW_MIBCOUNTER;

typedef enum L2SW_MIBCOUNTER_S{

    inmcursvoctets = 0,
    outmcusendoctets,
    
    inrldprsvoctets,
    outrldpsendoctets,
    
    outmcursvpkts,
    outmcusendpkts,
    outrldprsvpkts,
    outrldpsendpkts,
    reserved1,
    
    InIgmpSpecificQuery,
    InIgmpInterfaceleaves,
    InIgmpJoinsSuccess,
    InIgmpJoinsFail,
    InIgmpGeneralQuery,
    InIgmpChecksumError,
    reserved2,
    
    InmldSpecificQuery,
    InmldInterfaceleaves,
    InmldJoinsSuccess,
    InmldJoinsFail,
    InmldGeneralQuery,
    InmldchecksumError,
    reserved3,
    
    InKnownMulticastPkts,
    InOampduPkts,
    InReportSuppressionDrop,
    InLeaveSuppressionDrop,
    reserved4,
    
    OutmldSpecificQuery,
    OutmldReports,
    OutmldLeaves,
    OutmldGeneralQuery,
    OutIgmpSpecificQuery,
    OutIgmpReports,
    OutIgmpLeaves,
    OutIgmpGeneralQuery,
    outOampduPkts1,
    reserved5,
    
    L2SW_MIBCOUNTER_NUMBER,

}L2SW_MIBCOUNTER_T;

extern ret_t l2sw_setAsicMIBsCounterReset(l2sw_uint8 units, l2sw_uint32 greset, l2sw_uint32 qmreset, l2sw_uint32 pmask);
extern ret_t l2sw_getAsicMIBsCounter(l2sw_uint8 units, l2sw_uint32 port,L2SW_MIBCOUNTER mibIdx, l2sw_uint64* pCounter);
extern ret_t l2sw_getAsicMIBsLogCounter(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 *pCounter);
extern ret_t l2sw_getAsicMIBsControl(l2sw_uint8 units, l2sw_uint32* pMask);

extern ret_t l2sw_setAsicMIBsResetValue(l2sw_uint8 units, l2sw_uint32 value);
extern ret_t l2sw_getAsicMIBsResetValue(l2sw_uint8 units, l2sw_uint32* value);

extern ret_t l2sw_setAsicMIBsUsageMode(l2sw_uint8 units, l2sw_uint32 mode);
extern ret_t l2sw_getAsicMIBsUsageMode(l2sw_uint8 units, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicMIBsTimer(l2sw_uint8 units, l2sw_uint32 timer);
extern ret_t l2sw_getAsicMIBsTimer(l2sw_uint8 units, l2sw_uint32* pTimer);
extern ret_t l2sw_setAsicMIBsLoggingMode(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 mode);
extern ret_t l2sw_getAsicMIBsLoggingMode(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pMode);
extern ret_t l2sw_setAsicMIBsLoggingType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32 type);
extern ret_t l2sw_getAsicMIBsLoggingType(l2sw_uint8 units, l2sw_uint32 index, l2sw_uint32* pType);
extern ret_t l2sw_setAsicMIBsResetLoggingCounter(l2sw_uint8 units, l2sw_uint32 index);
extern ret_t l2sw_setAsicMIBsLength(l2sw_uint8 units, l2sw_uint32 txLengthMode, l2sw_uint32 rxLengthMode);
extern ret_t l2sw_getAsicMIBsLength(l2sw_uint8 units, l2sw_uint32 *pTxLengthMode, l2sw_uint32 *pRxLengthMode);

#endif /*#ifndef _L2SW_ASICDRV_MIB_H_*/

