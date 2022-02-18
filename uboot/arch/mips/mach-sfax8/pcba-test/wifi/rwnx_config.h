/**
 ****************************************************************************************
 *
 * @file rwnx_config.h
 *
 * @brief Inclusion of appropriate config files.
 *
 * This file should be included each time an NX_xxx definition is needed.
 * CFG_xxx macros must no be used in the code, only NX_xxx, defined in
 * the included config files.
 *
 * Copyright (C) Siflower 2015-2019
 *
 ****************************************************************************************
 */

#ifndef _RWNX_CONFIG_H_
#define _RWNX_CONFIG_H_

#define WITH_CATRF 1
/**
 ****************************************************************************************
 * @addtogroup MACSW
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "mac.h"

//bandwidth defines
//this should be delete when we are complete testing the simulation.
//Becase our configuration maybe fixed to RW_NX_DERIV_CHBW804020ONLY
#define RW_NX_DERIV_CHBW20ONLY 0
#define RW_NX_DERIV_CHBW4020ONLY 0
#define RW_NX_DERIV_CHBW804020ONLY 1
#define RW_NX_AGC_ON 1


#if RW_NX_DERIV_CHBW20ONLY
#define TXRFRAMPUP 240
#define BW_PARAM_CONFIG 20
#define PHYCLKEDGECNT 3
#define MPIFCLKEDGECNT 7
#elif RW_NX_DERIV_CHBW4020ONLY
#define TXRFRAMPUP 480
#define BW_PARAM_CONFIG 40
#define PHYCLKEDGECNT 1
#define MPIFCLKEDGECNT 3
#elif RW_NX_DERIV_CHBW804020ONLY
#define TXRFRAMPUP 480
#define BW_PARAM_CONFIG 80
#define PHYCLKEDGECNT 1
#define MPIFCLKEDGECNT 3
#else
#define TXRFRAMPUP 240
#define BW_PARAM_CONFIG 20
#define PHYCLKEDGECNT 3
#define MPIFCLKEDGECNT 7
#endif


// The CFG_xxx macros should be added on the compilation command line
// by the SCons scripts. Because it has been not implemented yet, any
// undefined option fall back on the default behavior.


/// Max number of virtual interfaces managed. MAC HW is limited to 6, but the LMAC assumes
/// that this number is a power of 2, thus only 1, 2 or 4 are allowed
#define NX_VIRT_DEV_MAX      4

/// Max number of peer devices managed
#define CFG_STA_MAX 10
#define NX_REMOTE_STA_MAX    CFG_STA_MAX

/// UMAC compiled with LMAC or not
#if defined CFG_UMAC
#define NX_UMAC_PRESENT  1
#define NX_STA 1
#define NX_AP 0
#define NX_ROAMING 0
#undef CFG_CMON
#define CFG_CMON
#undef CFG_HWSCAN
#define CFG_HWSCAN
#undef CFG_MROLE
#define CFG_MROLE
#undef CFG_AUTOBCN
#if defined CFG_BCN
#define CFG_AUTOBCN
#endif
/// RX Packet Reordering support
#define NX_REORD CFG_BARX
#define NX_REORD_BUF_SIZE CFG_REORD_BUF
#if (NX_REORD && ((NX_REORD_BUF_SIZE < 4) || (NX_REORD_BUF_SIZE > 64)))
#error "Incorrect reordering buffer size"
#endif
#else
#define NX_UMAC_PRESENT  0
#undef CFG_REORD
#define NX_REORD 0
#endif

/// Modem version
#define NX_MDM_VER     20

/// Beaconing support
#if defined CFG_BCN
#define NX_BEACONING   1
#else
#define NX_BEACONING   0
#endif

/// Maximum size of A-MSDU supported in reception
#if defined CFG_AMSDU_4K
#define RWNX_MAX_AMSDU_RX    4096
#elif defined CFG_AMSDU_8K
#define RWNX_MAX_AMSDU_RX    8192
#elif defined CFG_AMSDU_12K
#define RWNX_MAX_AMSDU_RX    12288
#endif

/// Minimal MPDU spacing we support in TX
#define NX_TX_MPDU_SPACING   CFG_SPC

/// Number of TX queues in the LMAC
#define NX_TXQ_CNT          (AC_MAX + NX_BEACONING)

#define CFG_TXDESC0 8
#define CFG_TXDESC1 64
#define CFG_TXDESC2 64
#define CFG_TXDESC3 8
/// Number of TX descriptors available in the system (BK)
#define NX_TXDESC_CNT0       CFG_TXDESC0
#if (NX_TXDESC_CNT0 & (NX_TXDESC_CNT0 - 1))
#error "Not a power of 2"
#endif
/// Number of TX descriptors available in the system (BE)
#define NX_TXDESC_CNT1       CFG_TXDESC1
#if (NX_TXDESC_CNT1 & (NX_TXDESC_CNT1 - 1))
#error "Not a power of 2"
#endif
/// Number of TX descriptors available in the system (VI)
#define NX_TXDESC_CNT2       CFG_TXDESC2
#if (NX_TXDESC_CNT2 & (NX_TXDESC_CNT2 - 1))
#error "Not a power of 2"
#endif
/// Number of TX descriptors available in the system (VO)
#define NX_TXDESC_CNT3       CFG_TXDESC3
#if (NX_TXDESC_CNT3 & (NX_TXDESC_CNT3 - 1))
#error "Not a power of 2"
#endif
#if (NX_BEACONING)
/// Number of TX descriptors available in the system (BCN)
#define NX_TXDESC_CNT4       CFG_TXDESC4
#if (NX_TXDESC_CNT4 & (NX_TXDESC_CNT4 - 1))
#error "Not a power of 2"
#endif
#endif

#if NX_UMAC_PRESENT
/// Number of TX flow control credits allocated by default per RA/TID
#define NX_DEFAULT_TX_CREDIT_CNT    4

/// VHT support
#if defined CFG_VHT
#define NX_VHT                      1
// Force MFP support if VHT is enabled, as it is mandatory for 802.11ac WiFi certification
#undef CFG_MFP
#define CFG_MFP
#else
#define NX_VHT                      0
#undef CFG_BFMER
#endif
#endif

/// Define the A-MSDU option for TX
#if defined CFG_AMSDU
#define NX_AMSDU_TX          1
// Force the aggregation to be supported
#undef CFG_AGG
#define CFG_AGG
#else
#define NX_AMSDU_TX          0
#endif


/// Define the Aggregation+Block Ack option for TX
#if defined CFG_AGG
#define NX_AMPDU_TX          1
#if NX_UMAC_PRESENT
#define NX_MAX_BA_TX         CFG_BATX
#if (NX_MAX_BA_TX == 0)
#error "At least one BA TX agreement shall be allowed"
#endif
#else
#define NX_MAX_BA_TX         0
#endif
#else
#define NX_AMPDU_TX          0
#define NX_MAX_BA_TX         0
#undef CFG_BWLEN
#endif

// Define the per-BW length adaptation
#if defined CFG_BWLEN
#define NX_BW_LEN_ADAPT      1
#define NX_BW_LEN_STEPS      4
#else
#define NX_BW_LEN_ADAPT      0
#define NX_BW_LEN_STEPS      1
#endif

/// Define the Radar detection option
#if defined CFG_RADAR
#define NX_RADAR_DETECT      1
#define RW_RADAR_EN          1
#else
#define NX_RADAR_DETECT      0
#define RW_RADAR_EN          0
#endif

#if defined CFG_EMB
    /// Code compiled for EMB platform
    #define NX_EMB  1
#else
    /// Code compiled for APP platform
    #define NX_EMB  0
#endif


/// Recovery mechanism
#if defined CFG_REC
#define NX_RECOVERY       1
#else
#define NX_RECOVERY       0
#endif


// Debug features
/// Debug dump forwarding
#if defined CFG_DBGDUMP
#define NX_DEBUG_DUMP     1
#else
#define NX_DEBUG_DUMP     0
#endif

/// Debug support
#define NX_DEBUG          2

/// Debug print output
#if NX_DEBUG
#define NX_PRINT          NX_PRINT_IPC
#else
#define NX_PRINT          NX_PRINT_NONE
#endif

/// Profiling support
#if defined CFG_PROF
#define NX_PROFILING_ON   1
#else
#define NX_PROFILING_ON   0
#endif

/// Heap size
#define NX_HEAP_SIZE     (2048 + 256 * NX_VIRT_DEV_MAX + 64 * NX_REMOTE_STA_MAX)

/// Deep Sleep
#define DEEP_SLEEP        0

/// UAPSD support
#if defined CFG_UAPSD
    #define NX_UAPSD   1
    // If UAPSD is enabled, we force the legacy PS mode to be enabled
    #undef CFG_PS
    #define CFG_PS
#else
    #define NX_UAPSD   0
#endif

/// DPSM support
#if defined CFG_DPSM
    #define NX_DPSM   1
    // If DPSM is enabled, we force the legacy PS mode to be enabled
    #undef CFG_PS
    #define CFG_PS
#else
    #define NX_DPSM   0
#endif

/// Legacy power save support
#if defined CFG_PS
    #define NX_POWERSAVE   1
#else
    #define NX_POWERSAVE   0
#endif //#if defined CFG_PS

/// Connection monitoring support
#if defined CFG_CMON
    #define NX_CONNECTION_MONITOR   1
#else
    #define NX_CONNECTION_MONITOR   0
#endif //#if defined CFG_CMON

/// Multi-role support (AP+STA, STA+STA)
#if defined CFG_MROLE
    #define NX_MULTI_ROLE           1
#else
    #define NX_MULTI_ROLE           0
#endif //#if defined CFG_MROLE

/// HW scan support
#if defined CFG_HWSCAN
    #define NX_HW_SCAN              1
#else
    #define NX_HW_SCAN              0
#endif

/// Autonomous beacon transmission support
#if defined CFG_AUTOBCN
    #define NX_BCN_AUTONOMOUS_TX        1
#else
    #define NX_BCN_AUTONOMOUS_TX        0
#endif

/// P2P support
#if CFG_P2P
    #define NX_P2P             1
    /// Maximum number of simultaneous P2P connections
    #define NX_P2P_VIF_MAX     CFG_P2P

    /// P2P GO Support
    #if defined CFG_P2P_GO
    /// AUTOBCN must be supported
    #if !(defined CFG_AUTOBCN)
    #error 'Autonomous Beacon TX feature (AUTOBCN) must be enabled'
    #endif
    #define NX_P2P_GO         1
    #else
    #define NX_P2P_GO          0
    #endif //(defined CFG_P2P_GO)
#else
    #define NX_P2P             0
    #define NX_P2P_VIF_MAX     0
    /// P2P GO Support
    #define NX_P2P_GO          0
#endif //(CFG_P2P)

/// MFP support (for UMAC only)
#if NX_UMAC_PRESENT && defined CFG_MFP
    #define NX_MFP             1
#else
    #define NX_MFP             0
#endif

/// WAPI support
#if defined CFG_WAPI
    #define RW_WAPI_EN                  1
#else
    #define RW_WAPI_EN                  0
#endif

/// WLAN coexistence support
#if defined CFG_COEX
    #define RW_WLAN_COEX_EN             1
#else
    #define RW_WLAN_COEX_EN             0
#endif

/// Beamformee support
#if defined CFG_BFMEE
    #define RW_BFMEE_EN                 1
#else
    #define RW_BFMEE_EN                 0
    // Disable MU-MIMO RX if Beamformee is not supported
    #undef CFG_MU_RX
#endif

/// Beamformer support
#if defined CFG_BFMER
    #define RW_BFMER_EN                 1
    #define RW_BFR_TXFRAME_CNT          NX_REMOTE_STA_MAX

    #if defined CFG_BFR_S_2K
    #define RW_BFR_MEM_SIZE             2048
    #elif defined CFG_BFR_S_4K
    #define RW_BFR_MEM_SIZE             4096
    #elif defined CFG_BFR_S_8K
    #define RW_BFR_MEM_SIZE             8192
    #elif defined CFG_BFR_S_16K
    #define RW_BFR_MEM_SIZE             16384
    #endif
#else //defined CFG_BFMER
    #define RW_BFMER_EN                 0
    #define RW_BFR_MEM_SIZE             0
    #define RW_BFR_TXFRAME_CNT          0
    // Disable MU-MIMO TX if Beamformer is not supported
    #undef CFG_MU_CNT
    #define CFG_MU_CNT                  1
#endif //defined CFG_BFMER

/// Wireless Mesh Networking support
#if defined CFG_MESH
    #define RW_MESH_EN                  (NX_MULTI_ROLE)
#else
    #define RW_MESH_EN                  0
#endif //defined CFG_MESH

/// Number of users supported
#define RW_USER_MAX                 CFG_MU_CNT
/// MU-MIMO TX support
#define RW_MUMIMO_TX_EN             (RW_USER_MAX > 1)
/// Support for up to one secondary user
#define RW_MUMIMO_SEC_USER1_EN      (RW_USER_MAX > 1)
/// Support for up to two secondary users
#define RW_MUMIMO_SEC_USER2_EN      (RW_USER_MAX > 2)
/// Support for up to three secondary users
#define RW_MUMIMO_SEC_USER3_EN      (RW_USER_MAX > 3)

/// MU-MIMO RX support
#ifdef CFG_MU_RX
#define RW_MUMIMO_RX_EN                1
#else
#define RW_MUMIMO_RX_EN                0
#endif

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_LDPC_DEC                  1

/// This macro appears in some generated header files, define it to avoid warning
#define RW_NX_AGC_SNR_EN                1

/// Number of TX frame descriptors and buffers available for frames generated internally
#define NX_TXFRAME_CNT       NX_VIRT_DEV_MAX + RW_BFR_TXFRAME_CNT

/// Maximum size of a TX frame generated internally
#if NX_UMAC_PRESENT
    #if (NX_P2P)
        #define NX_TXFRAME_LEN       384
    #else
        #define NX_TXFRAME_LEN       256
    #endif //(NX_P2P)
#else
    #define NX_TXFRAME_LEN       128
#endif

/// Maximum size of a beacon frame
#define NX_BCNFRAME_LEN      512

/// Number of RX descriptors (SW and Header descriptors)
#define NX_RXDESC_CNT        17

/// RX Payload buffer size
#define NX_RX_PAYLOAD_LEN    512

/// Maximum number of operating channel contexts
#define NX_CHAN_CTXT_CNT     3

/// Number of RX payload descriptors - defined to be twice the maximum A-MSDU size
/// plus one extra one used for HW flow control
#define NX_RX_PAYLOAD_DESC_CNT ((RWNX_MAX_AMSDU_RX / NX_RX_PAYLOAD_LEN) * 2 + 1)

/// Number of payloads per TX descriptor
#if NX_AMSDU_TX
#define NX_TX_PAYLOAD_MAX           6
#endif

/// Channel Contexts support
#define NX_CHNL_CTXT         (NX_HW_SCAN || NX_P2P)

/// General Purpose DMA module
#define NX_GP_DMA            (NX_CHNL_CTXT || NX_BCN_AUTONOMOUS_TX || NX_RADAR_DETECT || RW_BFMER_EN)

/// Internal handling of received frame
#define NX_RX_FRAME_HANDLING (NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_UMAC_PRESENT || RW_BFMER_EN)

/// Internal frame generation support
#define NX_TX_FRAME          (NX_POWERSAVE || NX_CONNECTION_MONITOR || NX_CHNL_CTXT || NX_BCN_AUTONOMOUS_TX || RW_BFMER_EN)

/// Traffic Detection support
#define NX_TD                (NX_DPSM || NX_CHNL_CTXT || NX_P2P_GO)

/// MAC Management timer support
#define NX_MM_TIMER          (NX_MULTI_ROLE || NX_CHNL_CTXT || NX_DPSM || NX_P2P || NX_UAPSD)

/// Default sleep duration if no time based procedure are ongoing
#define NX_DEFAULT_SLEEP_DURATION       60000000 // 60s

/// Rate control algorithm support
#ifdef CFG_RC
#define RC_ENABLE 1
#else
#define RC_ENABLE 0
#endif

/// @}

#endif // _RWNX_CONFIG_H_


