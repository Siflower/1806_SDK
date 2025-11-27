/**
 ****************************************************************************************
 *
 * @file lmac_msg.h
 *
 * @brief Main definitions for message exchanges with LMAC
 *
 * Copyright (C) Siflower 2018-2025
 *
 ****************************************************************************************
 */

#ifndef LMAC_MSG_H_
#define LMAC_MSG_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// for MAC related elements (mac_addr, mac_ssid...)
#include "lmac_types.h"
#include "lmac_mac.h"

/*
 ****************************************************************************************
 */
/////////////////////////////////////////////////////////////////////////////////
// COMMUNICATION WITH LMAC LAYER
/////////////////////////////////////////////////////////////////////////////////
/* Task identifiers for communication between LMAC and DRIVER */
enum
{
    TASK_NONE = (u8_l) -1,

    // MAC Management task.
    TASK_MM = 0,
    // DEBUG task
    TASK_DBG,
    /// SCAN task
    TASK_SCAN,
    /// TDLS task
    TASK_TDLS,
    /// SCANU task
    TASK_SCANU,
    /// ME task
    TASK_ME,
    /// SM task
    TASK_SM,
    /// APM task
    TASK_APM,
    /// BAM task
    TASK_BAM,
    /// MESH task
    TASK_MESH,
    /// RXU task
    TASK_RXU,
    // This is used to define the last task that is running on the EMB processor
    TASK_LAST_EMB = TASK_RXU,
    // nX API task
    TASK_API,
    TASK_MAX,
};


/// For MAC HW States copied from "hal_machw.h"
enum
{
    /// MAC HW IDLE State.
    HW_IDLE = 0,
    /// MAC HW RESERVED State.
    HW_RESERVED,
    /// MAC HW DOZE State.
    HW_DOZE,
    /// MAC HW ACTIVE State.
    HW_ACTIVE
};

/// Power Save mode setting
enum mm_ps_mode_state
{
    MM_PS_MODE_OFF,
    MM_PS_MODE_ON,
    MM_PS_MODE_ON_DYN,
    MM_PS_MODE_OFF_FORCE,
};

/// Status/error codes used in the MAC software.
enum
{
    CO_OK,
    CO_FAIL,
    CO_EMPTY,
    CO_FULL,
    CO_BAD_PARAM,
    CO_NOT_FOUND,
    CO_NO_MORE_ELT_AVAILABLE,
    CO_NO_ELT_IN_USE,
    CO_BUSY,
    CO_OP_IN_PROGRESS,
};

/// Remain on channel operation codes
enum mm_remain_on_channel_op
{
    MM_ROC_OP_START = 0,
    MM_ROC_OP_CANCEL,
};

#define DRV_TASK_ID 100

/// Message Identifier. The number of messages is limited to 0xFFFF.
/// The message ID is divided in two parts:
/// - bits[15..10] : task index (no more than 64 tasks supported).
/// - bits[9..0] : message index (no more that 1024 messages per task).
typedef u16 lmac_msg_id_t;

typedef u16 lmac_task_id_t;

/// Build the first message ID of a task.
#define LMAC_FIRST_MSG(task) ((lmac_msg_id_t)((task) << 10))

#define MSG_T(msg) ((lmac_task_id_t)((msg) >> 10))
#define MSG_I(msg) ((msg) & ((1<<10)-1))

/// The two following definitions are needed for message structure consistency
/// Structure of a list element header
struct co_list_hdr
{
    struct co_list_hdr *next;
};

/// Structure of a list
struct co_list
{
    /// pointer to first element of the list
    struct co_list_hdr *first;
    /// pointer to the last element
    struct co_list_hdr *last;
    /// number of element in the list
    u32_l cnt;

//      #if NX_DEBUG
//      /// max number of element in the list
//      u32_l maxcnt;
//      /// min number of element in the list
//      u32_l mincnt;
//      #endif // NX_DEBUG
};

/// Message structure.
struct lmac_msg
{
    lmac_msg_id_t     id;         ///< Message id.
    lmac_task_id_t    dest_id;    ///< Destination kernel identifier.
    lmac_task_id_t    src_id;     ///< Source kernel identifier.
    u16        param_len;  ///< Parameter embedded struct length.
    u32        param[];   ///< Parameter embedded struct. Must be word-aligned.
};

/// List of messages related to the task.
enum mm_msg_tag
{
    /// RESET Request.
    MM_RESET_REQ = LMAC_FIRST_MSG(TASK_MM),
    /// RESET Confirmation.
    MM_RESET_CFM,
    /// START Request.
    MM_START_REQ,
    /// START Confirmation.
    MM_START_CFM,
    /// Read Version Request.
    MM_VERSION_REQ,
    /// Read Version Confirmation.
    MM_VERSION_CFM,
    /// ADD INTERFACE Request.
    MM_ADD_IF_REQ,
    /// ADD INTERFACE Confirmation.
    MM_ADD_IF_CFM,
    /// REMOVE INTERFACE Request.
    MM_REMOVE_IF_REQ,
    /// REMOVE INTERFACE Confirmation.
    MM_REMOVE_IF_CFM,
    /// STA ADD Request.
    MM_STA_ADD_REQ,
    /// STA ADD Confirm.
    MM_STA_ADD_CFM,
    /// STA DEL Request.
    MM_STA_DEL_REQ,
    /// STA DEL Confirm.
    MM_STA_DEL_CFM,
    /// RX FILTER CONFIGURATION Request.
    MM_SET_FILTER_REQ,
    /// RX FILTER CONFIGURATION Confirmation.
    MM_SET_FILTER_CFM,
    /// CHANNEL CONFIGURATION Request.
    MM_SET_CHANNEL_REQ,
    /// CHANNEL CONFIGURATION Confirmation.
    MM_SET_CHANNEL_CFM,
    /// DTIM PERIOD CONFIGURATION Request.
    MM_SET_DTIM_REQ,
    /// DTIM PERIOD CONFIGURATION Confirmation.
    MM_SET_DTIM_CFM,
    /// BEACON INTERVAL CONFIGURATION Request.
    MM_SET_BEACON_INT_REQ,
    /// BEACON INTERVAL CONFIGURATION Confirmation.
    MM_SET_BEACON_INT_CFM,
    /// BASIC RATES CONFIGURATION Request.
    MM_SET_BASIC_RATES_REQ,
    /// BASIC RATES CONFIGURATION Confirmation.
    MM_SET_BASIC_RATES_CFM,
    /// BSSID CONFIGURATION Request.
    MM_SET_BSSID_REQ,
    /// BSSID CONFIGURATION Confirmation.
    MM_SET_BSSID_CFM,
    /// EDCA PARAMETERS CONFIGURATION Request.
    MM_SET_EDCA_REQ,
    /// EDCA PARAMETERS CONFIGURATION Confirmation.
    MM_SET_EDCA_CFM,
    /// ABGN MODE CONFIGURATION Request.
    MM_SET_MODE_REQ,
    /// ABGN MODE CONFIGURATION Confirmation.
    MM_SET_MODE_CFM,
    /// Request setting the VIF active state (i.e associated or AP started)
    MM_SET_VIF_STATE_REQ,
    /// Confirmation of the @ref MM_SET_VIF_STATE_REQ message.
    MM_SET_VIF_STATE_CFM,
    /// SLOT TIME PARAMETERS CONFIGURATION Request.
    MM_SET_SLOTTIME_REQ,
    /// SLOT TIME PARAMETERS CONFIGURATION Confirmation.
    MM_SET_SLOTTIME_CFM,
    /// Power Mode Change Request.
    MM_SET_IDLE_REQ,
    /// Power Mode Change Confirm.
    MM_SET_IDLE_CFM,
    /// KEY ADD Request.
    MM_KEY_ADD_REQ,
    /// KEY ADD Confirm.
    MM_KEY_ADD_CFM,
    /// KEY DEL Request.
    MM_KEY_DEL_REQ,
    /// KEY DEL Confirm.
    MM_KEY_DEL_CFM,
    /// DEFAULT KEY ADD Request.
    MM_DEFAULT_KEY_ADD_REQ,
    /// DEFAULT KEY ADD Confirm.
    MM_DEFAULT_KEY_ADD_CFM,
    /// Block Ack agreement info addition
    MM_BA_ADD_REQ,
    /// Block Ack agreement info addition confirmation
    MM_BA_ADD_CFM,
    /// Block Ack agreement info deletion
    MM_BA_DEL_REQ,
    /// Block Ack agreement info deletion confirmation
    MM_BA_DEL_CFM,
    /// Indication of the primary TBTT to the upper MAC. Upon the reception of this// message the upper MAC has to push the beacon(s) to the beacon transmission queue.
    MM_PRIMARY_TBTT_IND,
    /// Indication of the secondary TBTT to the upper MAC. Upon the reception of this// message the upper MAC has to push the beacon(s) to the beacon transmission queue.
    MM_SECONDARY_TBTT_IND,
    /// Request for changing the TX power
    MM_SET_POWER_REQ,
    /// Confirmation of the TX power change
    MM_SET_POWER_CFM,
    /// Request to the LMAC to trigger the embedded logic analyzer and forward the debug
    MM_DBG_TRIGGER_REQ,
    /// Set Power Save mode
    MM_SET_PS_MODE_REQ,
    /// Set Power Save mode confirmation
    MM_SET_PS_MODE_CFM,
    /// Request to add a channel context
    MM_CHAN_CTXT_ADD_REQ,
    /// Confirmation of the channel context addition
    MM_CHAN_CTXT_ADD_CFM,
    /// Request to delete a channel context
    MM_CHAN_CTXT_DEL_REQ,
    /// Confirmation of the channel context deletion
    MM_CHAN_CTXT_DEL_CFM,
    /// Request to link a channel context to a VIF
    MM_CHAN_CTXT_LINK_REQ,
    /// Confirmation of the channel context link
    MM_CHAN_CTXT_LINK_CFM,
    /// Request to unlink a channel context from a VIF
    MM_CHAN_CTXT_UNLINK_REQ,
    /// Confirmation of the channel context unlink
    MM_CHAN_CTXT_UNLINK_CFM,
    /// Request to update a channel context
    MM_CHAN_CTXT_UPDATE_REQ,
    /// Confirmation of the channel context update
    MM_CHAN_CTXT_UPDATE_CFM,
    /// Request to schedule a channel context
    MM_CHAN_CTXT_SCHED_REQ,
    /// Confirmation of the channel context scheduling
    MM_CHAN_CTXT_SCHED_CFM,
    /// Request to change the beacon template in LMAC
    MM_BCN_CHANGE_REQ,
    /// Confirmation of the beacon change
    MM_BCN_CHANGE_CFM,
    /// Request to update the TIM in the beacon (i.e to indicate traffic bufferized at AP)
    MM_TIM_UPDATE_REQ,
    /// Confirmation of the TIM update
    MM_TIM_UPDATE_CFM,
    /// Connection loss indication
    MM_CONNECTION_LOSS_IND,
    /// Channel context switch indication to the upper layers
    MM_CHANNEL_SWITCH_IND,
    /// Channel context pre-switch indication to the upper layers
    MM_CHANNEL_PRE_SWITCH_IND,
    /// Request to remain on channel or cancel remain on channel
    MM_REMAIN_ON_CHANNEL_REQ,
    /// Confirmation of the (cancel) remain on channel request
    MM_REMAIN_ON_CHANNEL_CFM,
    /// Remain on channel expired indication
    MM_REMAIN_ON_CHANNEL_EXP_IND,
    /// Indication of a PS state change of a peer device
    MM_PS_CHANGE_IND,
    /// Indication that some buffered traffic should be sent to the peer device
    MM_TRAFFIC_REQ_IND,
    /// Request to modify the STA Power-save mode options
    MM_SET_PS_OPTIONS_REQ,
    /// Confirmation of the PS options setting
    MM_SET_PS_OPTIONS_CFM,
    /// Indication of PS state change for a P2P VIF
    MM_P2P_VIF_PS_CHANGE_IND,
    /// Indication that CSA counter has been updated
    MM_CSA_COUNTER_IND,
    /// Channel occupation report indication
    MM_CHANNEL_SURVEY_IND,
    /// Message containing Beamformer Information
    MM_BFMER_ENABLE_REQ,
    /// Request to Start/Stop/Update NOA - GO Only
    MM_SET_P2P_NOA_REQ,
    /// Request to Start/Stop/Update Opportunistic PS - GO Only
    MM_SET_P2P_OPPPS_REQ,
    /// Start/Stop/Update NOA Confirmation
    MM_SET_P2P_NOA_CFM,
    /// Start/Stop/Update Opportunistic PS Confirmation
    MM_SET_P2P_OPPPS_CFM,
    /// P2P NoA Update Indication - GO Only
    MM_P2P_NOA_UPD_IND,
    /// Request to set RSSI threshold and RSSI hysteresis
    MM_CFG_RSSI_REQ,
    /// Indication that RSSI level is below or above the threshold
    MM_RSSI_STATUS_IND,
    /// Indication that CSA is done
    MM_CSA_FINISH_IND,
    /// Indication that CSA is in prorgess (resp. done) and traffic must be stopped (resp. restarted)
    MM_CSA_TRAFFIC_IND,
    /// Request to update the group information of a station
    MM_MU_GROUP_UPDATE_REQ,
    /// Confirmation of the @ref MM_MU_GROUP_UPDATE_REQ message
    MM_MU_GROUP_UPDATE_CFM,
    /// Request to initialize the antenna diversity algorithm
    MM_ANT_DIV_INIT_REQ,
    /// Request to stop the antenna diversity algorithm
    MM_ANT_DIV_STOP_REQ,
    /// Request to update the antenna switch status
    MM_ANT_DIV_UPDATE_REQ,
    /// Request to switch the antenna connected to path_0
    MM_SWITCH_ANTENNA_REQ,
    /// Indication that a packet loss has occurred
    MM_PKTLOSS_IND,
    /// Request to control rf by outside
    MM_RF_CTRL_REQ,
    /// confirmation that RF CTRL request is done
    MM_RF_CTRL_CFM,
    /// Request to do rf calibration by outside
    MM_RF_CALI_REQ,
    /// confirmation that RF calibration is done
    MM_RF_CALI_CFM,
    /// Request to dump the RF cmds
    MM_RF_CMD_DUMP_REQ,
    /// Confirmation that the dumping address is received
    MM_RF_CMD_DUMP_CFM,
    ///* Section of internal MM messages. No MM API messages should be defined below this point; Internal request to force the HW going to IDLE
    MM_FORCE_IDLE_REQ,
    /// Message indicating that the switch to the scan channel is done
    MM_SCAN_CHANNEL_START_IND,
    /// Message indicating that the scan on the channel is finished
    MM_SCAN_CHANNEL_END_IND,
    /// Internal request to move the AP TBTT by an offset
    MM_TBTT_MOVE_REQ,

    /// Message indicating that the RF ctrl has been processed
    MM_RF_CTRL_DONE,
    /// Message indicating that the lmac prepare complete
    MM_INIT_COMPLETE,
    /// Message indicating that the lmac request init la
    MM_LA_INIT_IND,
    /// Request to do heart_beat
    MM_HEART_REQ,
    /// Confirmation that the heart_beat is done
    MM_HEART_CFM,
    /// Message to enable tx power calibration
    MM_TXPOWER_CALI_ENABLE_REQ,
    /// Message to request to show rf temperature
    MM_RF_GET_TEMP_REQ,
    /// Confirmation that rf temperature is get
    MM_RF_GET_TEMP_CFM,
    /// Message to request to show channel distribution infos
    MM_GET_CHAN_INFO,
    /// Message to set power lvl in lmac
    MM_SET_POWER_LVL_REQ,
	/// Message to set the number of antenna in lmac
	MM_SET_ANTENNA_NUMBER_REQ,
    // Message to cooling temperatue
    MM_COOLING_TEMP_REQ,
	//MESSAGE TO ADD POWER
	MM_CHANGE_POWER_REQ,
    MM_VIF_DUMP_REQ,
    MM_VIF_DUMP_CFM,
    MM_DYING_GASP_IND,
    // Message to update sta idle time
    MM_UPDATE_IDLE_TIME_IND,
    // Message to request to send null frame to sta
    MM_SEND_NULL_FRAME_REQ,
    //set gain control
    MM_GAIN_CONTROL_REQ,
    // Message to notify CCA step
    MM_CCA_DROP_STEP_IND,
    // Message to get sta interface channel info
    MM_GET_CHANNEL_INFO_REQ,
    // Confirmation that sta interface channel info
    MM_GET_CHANNEL_INFO_CFM,
    // Message to send dbg frame
    MM_SEND_DEBUG_FRAME_REQ,
    // Confirmation that dbg frame
    MM_SEND_DEBUG_FRAME_IND,
    /// MAX number of messages
    MM_MAX,
};

struct mm_send_debug_frame_ind
{
    /// Status info
    u32_l statinfo;
};

struct mm_send_debug_frame_req
{
    /// Vif index associated the vif will send the debug frame
    u8_l vif_idx;
    /// Access category
    u8_l ac;
    /// Bandwidth
    u8_l bw;
    /// MCS
    u8_l mcs;
    /// NSS(0 or 1)
    u8_l nss;
    /// Format mode
    u8_l format_mod;
    /// GI
    u8_l gi;
    /// Enable LDPC
    u8_l ldpc;
    /// expect ACK or not
    u8_l expect_ack;
    /// Frame length of payload
    u16_l frmlen;
    /// Address of payload
    u32_l payload_addr;
};

/// Interface types
enum
{
    /// ESS STA interface
    MM_STA,
    /// IBSS STA interface
    MM_IBSS,
    /// AP interface
    MM_AP,
    // Mesh Point interface
    MM_MESH_POINT,
    // Monitor interface
    MM_MONITOR,
};

///BA agreement types
enum
{
    ///BlockAck agreement for TX
    BA_AGMT_TX,
    ///BlockAck agreement for RX
    BA_AGMT_RX,
};

///BA agreement related status
enum
{
    ///Correct BA agreement establishment
    BA_AGMT_ESTABLISHED,
    ///BA agreement already exists for STA+TID requested, cannot override it (should have been deleted first)
    BA_AGMT_ALREADY_EXISTS,
    ///Correct BA agreement deletion
    BA_AGMT_DELETED,
    ///BA agreement for the (STA, TID) doesn't exist so nothing to delete
    BA_AGMT_DOESNT_EXIST,
};

/// Features supported by LMAC - Positions
enum mm_features
{
    /// Beaconing
    MM_FEAT_BCN_BIT         = 0,
    /// Autonomous Beacon Transmission
    MM_FEAT_AUTOBCN_BIT,
    /// Scan in LMAC
    MM_FEAT_HWSCAN_BIT,
    /// Connection Monitoring
    MM_FEAT_CMON_BIT,
    /// Multi Role
    MM_FEAT_MROLE_BIT,
    /// Radar Detection
    MM_FEAT_RADAR_BIT,
    /// Power Save
    MM_FEAT_PS_BIT,
    /// UAPSD
    MM_FEAT_UAPSD_BIT,
    /// DPSM
    MM_FEAT_DPSM_BIT,
    /// A-MPDU
    MM_FEAT_AMPDU_BIT,
    /// A-MSDU
    MM_FEAT_AMSDU_BIT,
    /// Channel Context
    MM_FEAT_CHNL_CTXT_BIT,
    /// Packet reordering
    MM_FEAT_REORD_BIT,
    /// P2P
    MM_FEAT_P2P_BIT,
    /// P2P Go
    MM_FEAT_P2P_GO_BIT,
    /// UMAC Present
    MM_FEAT_UMAC_BIT,
    /// VHT support
    MM_FEAT_VHT_BIT,
    /// Beamformee
    MM_FEAT_BFMEE_BIT,
    /// Beamformer
    MM_FEAT_BFMER_BIT,
    /// WAPI
    MM_FEAT_WAPI_BIT,
    /// MFP
    MM_FEAT_MFP_BIT,
    /// Mu-MIMO RX support
    MM_FEAT_MU_MIMO_RX_BIT,
    /// Mu-MIMO TX support
    MM_FEAT_MU_MIMO_TX_BIT,
    /// Wireless Mesh Networking
    MM_FEAT_MESH_BIT,
    /// TDLS support
    MM_FEAT_TDLS_BIT,
    /// Antenna Diversity support
    MM_FEAT_ANT_DIV_BIT,
    /// UF support
    MM_FEAT_UF_BIT,
    /// A-MSDU maximum size (bit0)
    MM_AMSDU_MAX_SIZE_BIT0,
    /// A-MSDU maximum size (bit1)
    MM_AMSDU_MAX_SIZE_BIT1,
    /// MON_DATA support
    MM_FEAT_MON_DATA_BIT,
    /// HE (802.11ax) support
    MM_FEAT_HE_BIT,
    /// lite memory system
    MM_FEAT_LITE_MEM_BIT,
    // austin: now MM_FEAT_LITE_MEM_BIT is 31, and we can't add more MM_BIT here.
};

/// Maximum number of words in the configuration buffer
#define PHY_CFG_BUF_SIZE     16

/// Structure containing the parameters of the PHY configuration
struct phy_cfg_tag
{
    /// Buffer containing the parameters specific for the PHY used
    u32_l parameters[PHY_CFG_BUF_SIZE];
#ifdef CONFIG_ENABLE_DIGGAINTABLE
	/// Digital gain table
	uint32_t digtable[7];
	uint32_t digtable_max[7];
#endif
#ifdef CONFIG_ENABLE_TXPOWERTABLE
	/// Txpower gain table
	uint8_t lb_power_gain_tb[31];
	uint8_t hb_power_gain_tb[31];
#endif
};

/// Structure containing the parameters of the Trident PHY configuration
struct phy_trd_cfg_tag
{
    /// MDM type(nxm)(upper nibble) and MDM2RF path mapping(lower nibble)
    u8_l path_mapping;
    /// TX DC offset compensation
    u32_l tx_dc_off_comp;
};

/// Structure containing the parameters of the Karst PHY configuration
struct phy_karst_cfg_tag
{
    /// TX IQ mismatch compensation in 2.4GHz
    u32_l tx_iq_comp_2_4G[2];
    /// RX IQ mismatch compensation in 2.4GHz
    u32_l rx_iq_comp_2_4G[2];
    /// TX IQ mismatch compensation in 5GHz
    u32_l tx_iq_comp_5G[2];
    /// RX IQ mismatch compensation in 5GHz
    u32_l rx_iq_comp_5G[2];
    /// RF path used by default (0 or 1)
    u8_l path_used;
};

enum phy_cfg_tag_flag
{
    POWER_CALI_DISABLE = 0,
    RF_ENHANCE_LOWRATE_POWER = 1 << 0,
    TXPOWER_CALI_TABLE = 1 << 1,
    RF_EXTERNAL_PA_ENABLE = 1 << 2,
    ATE_TEST_ON = 1 << 3,
	DUAL_ANTENNA_CALIBRATE = 1 << 4,
};

/// Structure containing the parameters of the aetnensis PHY configuration
/// tx gain table
struct phy_aetnensis_cfg_tag
{
    /// gain table version
    u32_l version;
    /// mapping len
    u32_l mapping_len;
    /// gain address
    u32_l address;
	/// sleep mode power gain address
	u32_l sleepmode_address;
	/// low power gain address
	u32_l low_address;
	/// normal power gain address
    u32_l normal_address;
#ifdef CFG_DUAL_ANTENNA_CALIBRATE
    /// gain address for second antenna
    u32_l address_second;
	/// sleep mode power gain address for second antenna
	u32_l sleepmode_address_second;
	/// low power gain address for second antenna
	u32_l low_address_second;
	/// normal power gain address for second antenna
	u32_l normal_address_second;
#endif
    /// high power gain address
    u32_l high_address;
    /// gain num for one band
    u32_l gain_num;
    /// num of channel;
    u32_l chan_num;
    /// enable flag
    u32_l flag;
};

/// for global flags passed to lmac
enum mm_start_flags{
    /// if set ,addr_mask will be set to 0xFFFFFFFFFFFF
    MM_START_GLOBAL_FLAG_ADDR_MASK_ALL = 0x1 << 0,
	MM_START_GLOBAL_FLAG_NOT_SEND_NULL = 0x1 << 1,
};

#define MM_ENV_CFG_MAX 8

enum MM_GLOBAL_PARAMETERS
{
    MM_AMPDU_MAX_CNT = 0,
#ifdef INDEPENDENT_ANTENNA_CONTROL
	MM_INDEPENDENT_ANTENNA_CONTROL = 1,
#endif
	RTS_CTS_CHANGE = 1,
};

/// Structure containing the parameters of the @ref MM_START_REQ message
struct mm_start_req
{
    /// PHY configuration
    struct phy_cfg_tag phy_cfg;
    /// UAPSD timeout
    u32_l uapsd_timeout;
    /// Local LP clock accuracy (in ppm)
    u16_l lp_clk_accuracy;
    /// Global dynamic flags for both umac/lmac
    u32_l global_flags;
    //global parameters passed from umac layer
    uint32_t parameters[MM_ENV_CFG_MAX];
};

/// Structure containing the parameters of the @ref MM_SET_CHANNEL_REQ message
struct mm_set_channel_req
{
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// Index of the RF for which the channel has to be set (0: operating (primary), 1: secondary
    /// RF (used for additional radar detection). This parameter is reserved if no secondary RF
    /// is available in the system
    u8_l index;
    /// Max tx power for this channel
    s8_l tx_power;
	//set chnanel
	u8_l setchannel;
#ifdef CONFIG_SIWIFI_CH_OFFSET
    /// set channel offset
    int channel_off;
    /// vif_type
    int8_t mode;
#endif
};

/// Structure containing the parameters of the @ref MM_SET_CHANNEL_CFM message
struct mm_set_channel_cfm
{
    /// Radio index to be used in policy table
    u8_l radio_idx;
    /// TX power configured (in dBm)
    s8_l power;
};

/// Structure containing the parameters of the @ref MM_SET_DTIM_REQ message
struct mm_set_dtim_req
{
    /// DTIM period
    u8_l dtim_period;
};

/// Structure containing the parameters of the @ref MM_SET_POWER_REQ message
struct mm_set_power_req
{
    /// Index of the interface for which the parameter is configured
    u8_l inst_nbr;
    /// TX power (in dBm)
    s8_l power;
};

/// Structure containing the parameters of the @ref MM_SET_POWER_LVL_REQ message
struct mm_set_power_lvl_req
{
    /// TX power lvl(see siwifi_phy_txpower_lvl)
    u8_l power_lvl;
};

/// Structure containing the parameters of the @ref MM_SET_ANTENNA_NUMBER_REQ message
struct mm_set_antenna_number_req
{
    /// the number of antenna(see siwifi_phy_antenna_number)
    u8_l antenna_number;
};

/// Structure containing the parameters of the @ref MM_SET_POWER_CFM message
struct mm_set_power_cfm
{
    /// Radio index to be used in policy table
    u8_l radio_idx;
    /// TX power configured (in dBm)
    s8_l power;
};

#define MM_GAIN_CONTROL_ACTION_FIX_GAIN 0x1
/// Structure containing the parameters of the @ref MM_SET_POWER_LVL_REQ message
struct mm_set_gain_control_req
{
    u8_l action;
    int param1;
    int param2;
};

/// Structure containing the parameters of the @ref MM_SET_BEACON_INT_REQ message
struct mm_set_beacon_int_req
{
    /// Beacon interval
    u16_l beacon_int;
    /// Index of the interface for which the parameter is configured
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_BASIC_RATES_REQ message
struct mm_set_basic_rates_req
{
    /// Basic rate set (as expected by bssBasicRateSet field of Rates MAC HW register)
    u32_l rates;
    /// Index of the interface for which the parameter is configured
    u8_l inst_nbr;
    /// Band on which the interface will operate
    u8_l band;
};

/// Structure containing the parameters of the @ref MM_SET_BSSID_REQ message
struct mm_set_bssid_req
{
    /// BSSID to be configured in HW
    struct mac_addr bssid;
    /// Index of the interface for which the parameter is configured
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_SET_FILTER_REQ message
struct mm_set_filter_req
{
    /// RX filter to be put into rxCntrlReg HW register
    u32_l filter;
};

/// Structure containing the parameters of the @ref MM_ADD_IF_REQ message.
struct mm_add_if_req
{
    /// Type of the interface (AP, STA, ADHOC, ...)
    u8_l type;
    /// MAC ADDR of the interface to start
    struct mac_addr addr;
    /// P2P Interface
    bool_l p2p;
    /// Unsupported HT Frame Logging
    bool_l uf;
    /// the req vif index is valid or not
    bool_l req_index_valid;
    /// requested driver index
    u8_l req_index;
    /// Interface use 4addr
    bool_l use_4addr;
};

/// Structure containing the parameters of the @ref MM_SET_EDCA_REQ message
struct mm_set_edca_req
{
    /// EDCA parameters of the queue (as expected by edcaACxReg HW register)
    u32_l ac_param;
    /// Flag indicating if UAPSD can be used on this queue
    bool_l uapsd;
    /// HW queue for which the parameters are configured
    u8_l hw_queue;
    /// Index of the interface for which the parameters are configured
    u8_l inst_nbr;
};

struct mm_set_idle_req
{
    u8_l hw_idle;
};

/// Structure containing the parameters of the @ref MM_SET_SLOTTIME_REQ message
struct mm_set_slottime_req
{
    /// Slot time expressed in us
    u8_l slottime;
};

/// Structure containing the parameters of the @ref MM_SET_MODE_REQ message
struct mm_set_mode_req
{
    /// abgnMode field of macCntrl1Reg register
    u8_l abgnmode;
};

/// Structure containing the parameters of the @ref MM_SET_VIF_STATE_REQ message
struct mm_set_vif_state_req
{
    /// Association Id received from the AP (valid only if the VIF is of STA type)
    u16_l aid;
    /// Flag indicating if the VIF is active or not
    bool_l active;
    /// Interface index
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_ADD_IF_CFM message.
struct mm_add_if_cfm
{
    /// Status of operation (different from 0 if unsuccessful)
    u8_l status;
    /// Interface index assigned by the LMAC
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_REMOVE_IF_REQ message.
struct mm_remove_if_req
{
    /// Interface index assigned by the LMAC
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_VERSION_CFM message.
struct mm_version_cfm
{
    /// Version of the LMAC FW
    u32_l version_lmac;
    /// Version1 of the MAC HW (as encoded in version1Reg MAC HW register)
    u32_l version_machw_1;
    /// Version2 of the MAC HW (as encoded in version2Reg MAC HW register)
    u32_l version_machw_2;
    /// Version1 of the PHY (depends on actual PHY)
    u32_l version_phy_1;
    /// Version2 of the PHY (depends on actual PHY)
    u32_l version_phy_2;
    /// Supported Features
    u32_l features;
    /// Maximum number of supported stations
    u16_l max_sta_nb;
    /// Maximum number of supported virtual interfaces
    u8_l max_vif_nb;
};

/// Structure containing the parameters of the @ref MM_STA_ADD_REQ message.
struct mm_sta_add_req
{
    /// Maximum A-MPDU size, in bytes, for HE frames
    u32_l ampdu_size_max_he;
    /// Maximum A-MPDU size, in bytes, for VHT frames
    u32_l ampdu_size_max_vht;
    /// PAID/GID
    u32_l paid_gid;
    /// Maximum A-MPDU size, in bytes, for HT frames
    u16_l ampdu_size_max_ht;
    /// MAC address of the station to be added
    struct mac_addr mac_addr;
    /// A-MPDU spacing, in us
    u8_l ampdu_spacing_min;
    /// Interface index
    u8_l inst_nbr;
    /// TDLS station
    bool_l tdls_sta;
    /// Indicate if the station is TDLS link initiator station
    bool_l tdls_sta_initiator;
    /// Indicate if the TDLS Channel Switch is allowed
    bool_l tdls_chsw_allowed;
};

/// Structure containing the parameters of the @ref MM_STA_ADD_CFM message.
struct mm_sta_add_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    u8_l status;
    /// Index assigned by the LMAC to the newly added station
    u8_l sta_idx;
    /// MAC HW index of the newly added station
    u8_l hw_sta_idx;
};

/// Structure containing the parameters of the @ref MM_STA_DEL_REQ message.
struct mm_sta_del_req
{
    /// Index of the station to be deleted
    u8_l sta_idx;
};

/// Structure containing the parameters of the @ref MM_STA_DEL_CFM message.
struct mm_sta_del_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    u8_l     status;
};

/// Structure containing the parameters of the SET_POWER_MODE REQ message.
struct mm_setpowermode_req
{
    u8_l mode;
    u8_l sta_idx;
};

/// Structure containing the parameters of the SET_POWER_MODE CFM message.
struct mm_setpowermode_cfm
{
    u8_l     status;
};

/// Structure containing the parameters of the @ref MM_KEY_ADD REQ message.
struct mm_key_add_req
{
    /// Key index (valid only for default keys)
    u8_l key_idx;
    /// STA index (valid only for pairwise or mesh group keys)
    u8_l sta_idx;
    /// Key material
    struct mac_sec_key key;
    /// Cipher suite (WEP64, WEP128, TKIP, CCMP)
    u8_l cipher_suite;
    /// Index of the interface for which the key is set (valid only for default keys or mesh group keys)
    u8_l inst_nbr;
    /// A-MSDU SPP parameter
    u8_l spp;
    /// Indicate if provided key is a pairwise key or not
    bool_l pairwise;
    /// Indicate if this add action is during recovering
    bool_l recovering;
    // tx package number for this key, when recovering is true, this value make sense
    u64_l tx_pn;

};

/// Structure containing the parameters of the @ref MM_KEY_ADD_CFM message.
struct mm_key_add_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    u8_l status;
    /// HW index of the key just added
    u8_l hw_key_idx;
};

/// Structure containing the parameters of the @ref MM_KEY_DEL_REQ message.
struct mm_key_del_req
{
    /// HW index of the key to be deleted
    u8_l hw_key_idx;
};

/// Structure containing the parameters of the @ref MM_DEFAULT_KEY_ADD_REQ message.
struct mm_default_key_add_req
{
    /// Key index (valid only for default keys)
    uint8_t default_key_idx;
    /// Index of the interface for which the key is set (valid only for default keys or mesh group keys)
    uint8_t inst_nbr;
};

/// Structure containing the parameters of the @ref MM_DEFAULT_KEY_ADD_CFM message.
struct mm_default_key_add_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    uint8_t status;
};

/// Structure containing the parameters of the @ref MM_BA_ADD_REQ message.
struct mm_ba_add_req
{
    ///Type of agreement (0: TX, 1: RX)
    u8_l  type;
    ///Index of peer station with which the agreement is made
    u8_l  sta_idx;
    ///TID for which the agreement is made with peer station
    u8_l  tid;
    ///Buffer size - number of MPDUs that can be held in its buffer per TID
    u8_l  bufsz;
    /// Start sequence number negotiated during BA setup - the one in first aggregated MPDU counts more
    u16_l ssn;
};

/// Structure containing the parameters of the @ref MM_BA_ADD_CFM message.
struct mm_ba_add_cfm
{
    ///Index of peer station for which the agreement is being confirmed
    u8_l sta_idx;
    ///TID for which the agreement is being confirmed
    u8_l tid;
    /// Status of ba establishment
    u8_l status;
};

/// Structure containing the parameters of the @ref MM_BA_DEL_REQ message.
struct mm_ba_del_req
{
    ///Type of agreement (0: TX, 1: RX)
    u8_l type;
    ///Index of peer station for which the agreement is being deleted
    u8_l sta_idx;
    ///TID for which the agreement is being deleted
    u8_l tid;
};

/// Structure containing the parameters of the @ref MM_BA_DEL_CFM message.
struct mm_ba_del_cfm
{
    ///Index of peer station for which the agreement deletion is being confirmed
    u8_l sta_idx;
    ///TID for which the agreement deletion is being confirmed
    u8_l tid;
    /// Status of ba deletion
    u8_l status;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_ADD_REQ message
struct mm_chan_ctxt_add_req
{
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// Max tx power for this channel
    s8_l tx_power;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_ADD_REQ message
struct mm_chan_ctxt_add_cfm
{
    /// Status of the addition
    u8_l status;
    /// Index of the new channel context
    u8_l index;
};


/// Structure containing the parameters of the @ref MM_CHAN_CTXT_DEL_REQ message
struct mm_chan_ctxt_del_req
{
    /// Index of the new channel context to be deleted
    u8_l index;
};


/// Structure containing the parameters of the @ref MM_CHAN_CTXT_LINK_REQ message
struct mm_chan_ctxt_link_req
{
    /// VIF index
    u8_l vif_index;
    /// Channel context index
    u8_l chan_index;
    /// Indicate if this is a channel switch (unlink current ctx first if true)
    u8_l chan_switch;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_UNLINK_REQ message
struct mm_chan_ctxt_unlink_req
{
    /// VIF index
    u8_l vif_index;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_UPDATE_REQ message
struct mm_chan_ctxt_update_req
{
    /// Channel context index
    u8_l chan_index;
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// TX power (in dBm)
    s8_l tx_power;
};

/// Structure containing the parameters of the @ref MM_CHAN_CTXT_SCHED_REQ message
struct mm_chan_ctxt_sched_req
{
    /// VIF index
    u8_l vif_index;
    /// Channel context index
    u8_l chan_index;
    /// Type of the scheduling request (0: normal scheduling, 1: derogatory
    /// scheduling)
    u8_l type;
};

/// Structure containing the parameters of the @ref mm_rf_ctrl_req
struct mm_rf_ctrl_req
{
    ///ctrl command id
    uint8_t ctrl_id;
};

/// Structure containing the parameters of the @ref mm_rf_cali_req
struct mm_rf_cali_req
{
    /// configuration for calibration
    uint16_t conf;
    /// channel for calibration, 0 use the default value
    uint16_t chan;
    /// bw for calibration,0  use the default value
    uint16_t bw;
};

enum mm_rf_ctrl_id
{
    MM_RF_CTRL_STOP_COMMUNICATION = 0,
    MM_RF_CTRL_START_COMMUNICATION,

    MM_RF_CTRL_MAX,
};

#ifdef CONFIG_SIWIFI_RF_CMD_DUMP
/// Structure containing the parameters of the @ref mm_rf_cmd_dump_req
struct mm_rf_cmd_dump_req
{
    /// dump address
    uint32_t address;
    /// size of dump
    uint32_t size;
    /// dump flag
    uint8_t flag;
};
/// Structure containing the parameters of the @ref mm_rf_cmd_dump_cfm
struct mm_rf_cmd_dump_cfm
{
    /// counting address of dump table
    uint32_t count;
    ///Status of rf cmd dump
    uint8_t status;
};
#endif
// Structure containing the parameters of the @mm_rf_get_temp_cfm
struct mm_rf_get_temp_cfm
{
    /// value of temperature
    uint16_t value;
};
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
/// Structure containing the parameters of the @ref mm_txpower_cali_enable_req
struct mm_txpower_cali_enable_req
{
    ///Flag to enable of disable tx power cali
    u32_l enable_flag;
};
#endif

/// Structure containing the parameters of the @ref MM_CHANNEL_SWITCH_IND message
struct mm_channel_switch_ind
{
    /// Index of the channel context we will switch to
    u8_l chan_index;
    /// Indicate if the switch has been triggered by a Remain on channel request
    bool_l roc;
    /// VIF on which remain on channel operation has been started (if roc == 1)
    u8_l vif_index;
    /// Indicate if the switch has been triggered by a TDLS Remain on channel request
    bool_l roc_tdls;
};

/// Structure containing the parameters of the @ref MM_CHANNEL_PRE_SWITCH_IND message
struct mm_channel_pre_switch_ind
{
    /// Index of the channel context we will switch to
    u8_l chan_index;
};

/// Structure containing the parameters of the @ref MM_CONNECTION_LOSS_IND message.
struct mm_connection_loss_ind
{
    /// VIF instance number
    u8_l inst_nbr;
};


/// Structure containing the parameters of the @ref MM_DBG_TRIGGER_REQ message.
struct mm_dbg_trigger_req
{
    /// Error type, 0 recovery error , 1 fatal error
    uint8_t type;
    /// Error trace to be reported by the LMAC
    char error[64];
};

/// Structure containing the parameters of the @ref MM_SET_PS_MODE_REQ message.
struct mm_set_ps_mode_req
{
    /// Power Save is activated or deactivated
    u8_l  new_state;
};

/// Structure containing the parameters of the @ref MM_BCN_CHANGE_REQ message.
#define BCN_MAX_CSA_CPT 2
struct mm_bcn_change_req
{
    /// Pointer, in host memory, to the new beacon template
    u32_l bcn_ptr;
    /// Length of the beacon template
    u16_l bcn_len;
    /// Offset of the TIM IE in the beacon
    u16_l tim_oft;
    /// Length of the TIM IE
    u8_l tim_len;
    /// Index of the VIF for which the beacon is updated
    u8_l inst_nbr;
    /// Offset of CSA (channel switch announcement) counters (0 means no counter)
    u8_l csa_oft[BCN_MAX_CSA_CPT];
};


/// Structure containing the parameters of the @ref MM_TIM_UPDATE_REQ message.
struct mm_tim_update_req
{
    /// Association ID of the STA the bit of which has to be updated (0 for BC/MC traffic)
    u16_l aid;
    /// Flag indicating the availability of data packets for the given STA
    u8_l tx_avail;
    /// Index of the VIF for which the TIM is updated
    u8_l inst_nbr;
};

/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_REQ message.
struct mm_remain_on_channel_req
{
    /// Operation Code
    u8_l op_code;
    /// VIF Index
    u8_l vif_index;
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// Duration (in ms)
    u32_l duration_ms;
    /// TX power (in dBm)
    s8_l tx_power;
};

/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_CFM message
struct mm_remain_on_channel_cfm
{
    /// Operation Code
    u8_l op_code;
    /// Status of the operation
    u8_l status;
    /// Channel Context index
    u8_l chan_ctxt_index;
};

/// Structure containing the parameters of the @ref MM_REMAIN_ON_CHANNEL_EXP_IND message
struct mm_remain_on_channel_exp_ind
{
    /// VIF Index
    u8_l vif_index;
    /// Channel Context index
    u8_l chan_ctxt_index;
};

/// Structure containing the parameters of the @ref MM_SET_UAPSD_TMR_REQ message.
struct mm_set_uapsd_tmr_req
{
    /// action: Start or Stop the timer
    u8_l  action;
    /// timeout value, in milliseconds
    u32_l  timeout;
};

/// Structure containing the parameters of the @ref MM_SET_UAPSD_TMR_CFM message.
struct mm_set_uapsd_tmr_cfm
{
    /// Status of the operation (different from 0 if unsuccessful)
    u8_l     status;
};


/// Structure containing the parameters of the @ref MM_PS_CHANGE_IND message
struct mm_ps_change_ind
{
    /// Index of the peer device that is switching its PS state
    u8_l sta_idx;
    /// New PS state of the peer device (0: active, 1: sleeping)
    u8_l ps_state;
};

/// Structure containing the parameters of the @ref MM_P2P_VIF_PS_CHANGE_IND message
struct mm_p2p_vif_ps_change_ind
{
    /// Index of the P2P VIF that is switching its PS state
    u8_l vif_index;
    /// New PS state of the P2P VIF interface (0: active, 1: sleeping)
    u8_l ps_state;
};

/// Structure containing the parameters of the @ref MM_TRAFFIC_REQ_IND message
struct mm_traffic_req_ind
{
    /// Index of the peer device that needs traffic
    u8_l sta_idx;
    /// Number of packets that need to be sent (if 0, all buffered traffic shall be sent)
    u8_l pkt_cnt;
    /// Flag indicating if the traffic request concerns U-APSD queues or not
    bool_l uapsd;
};

/// Structure containing the parameters of the @ref MM_SET_PS_OPTIONS_REQ message.
struct mm_set_ps_options_req
{
    /// VIF Index
    u8_l vif_index;
    /// Listen interval (0 if wake up shall be based on DTIM period)
    u16_l listen_interval;
    /// Flag indicating if we shall listen the BC/MC traffic or not
    bool_l dont_listen_bc_mc;
};

/// Structure containing the parameters of the @ref MM_CSA_COUNTER_IND message
struct mm_csa_counter_ind
{
    /// Index of the VIF
    u8_l vif_index;
    /// Updated CSA counter value
    u8_l csa_count;
};

/// Structure containing the parameters of the @ref MM_CHANNEL_SURVEY_IND message
struct mm_channel_survey_ind
{
    /// Frequency of the channel
    u16_l freq;
    /// Noise in dbm
    s8_l noise_dbm;
    /// Amount of time spent of the channel (in ms)
    u32_l chan_time_ms;
    /// Amount of time the primary channel was sensed busy
    u32_l chan_time_busy_ms;
};

/// Structure containing the parameters of the @ref MM_BFMER_ENABLE_REQ message.
struct mm_bfmer_enable_req
{
    /**
     * Address of the beamforming report space allocated in host memory
     * (Valid only if vht_su_bfmee is true)
     */
    u32_l host_bfr_addr;
    /**
     * Size of the beamforming report space allocated in host memory. This space should
     * be twice the maximum size of the expected beamforming reports as the FW will
     * divide it in two in order to be able to upload a new report while another one is
     * used in transmission
     */
    u16_l host_bfr_size;
    /// AID
    u16_l aid;
    /// Station Index
    u8_l sta_idx;
    /// Maximum number of spatial streams the station can receive
    u8_l rx_nss;
    /**
     * Indicate if peer STA is MU Beamformee (VHT) capable
     * (Valid only if vht_su_bfmee is true)
     */
    bool_l vht_mu_bfmee;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_NOA_REQ message.
struct mm_set_p2p_noa_req
{
    /// VIF Index
    u8_l vif_index;
    /// Allocated NOA Instance Number - Valid only if count = 0
    u8_l noa_inst_nb;
    /// Count
    u8_l count;
    /// Indicate if NoA can be paused for traffic reason
    bool_l dyn_noa;
    /// Duration (in us)
    u32_l duration_us;
    /// Interval (in us)
    u32_l interval_us;
    /// Start Time offset from next TBTT (in us)
    u32_l start_offset;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_OPPPS_REQ message.
struct mm_set_p2p_oppps_req
{
    /// VIF Index
    u8_l vif_index;
    /// CTWindow
    u8_l ctwindow;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_NOA_CFM message.
struct mm_set_p2p_noa_cfm
{
    /// Request status
    u8_l status;
};

/// Structure containing the parameters of the @ref MM_SET_P2P_OPPPS_CFM message.
struct mm_set_p2p_oppps_cfm
{
    /// Request status
    u8_l status;
};

/// Structure containing the parameters of the @ref MM_P2P_NOA_UPD_IND message.
struct mm_p2p_noa_upd_ind
{
    /// VIF Index
    u8_l vif_index;
    /// NOA Instance Number
    u8_l noa_inst_nb;
    /// NoA Type
    u8_l noa_type;
    /// Count
    u8_l count;
    /// Duration (in us)
    u32_l duration_us;
    /// Interval (in us)
    u32_l interval_us;
    /// Start Time
    u32_l start_time;
};

/// Structure containing the parameters of the @ref MM_CFG_RSSI_REQ message
struct mm_cfg_rssi_req
{
    /// Index of the VIF
    u8_l vif_index;
    /// RSSI threshold
    s8_l rssi_thold;
    /// RSSI hysteresis
    u8_l rssi_hyst;
};

/// Structure containing the parameters of the @ref MM_RSSI_STATUS_IND message
struct mm_rssi_status_ind
{
    /// Index of the VIF
    u8_l vif_index;
    /// Status of the RSSI
    bool_l rssi_status;
    /// Current RSSI
    s8_l rssi;
};

/// Structure containing the parameters of the @ref MM_PKTLOSS_IND message
struct mm_pktloss_ind
{
    /// Index of the VIF
    u8_l vif_index;
    /// Address of the STA for which there is a packet loss
    struct mac_addr mac_addr;
    /// Number of packets lost
    u32 num_packets;
};

/// Structure containing the parameters of the @ref MM_CSA_FINISH_IND message
struct mm_csa_finish_ind
{
    /// Index of the VIF
    u8_l vif_index;
    /// Status of the operation
    u8_l status;
    /// New channel ctx index
    u8_l chan_idx;
    /// New channel center_freq
    u16_l center_freq;
    /// New channel bw
    u8_l bw;
    /// New channel center_freq1
    u16_l center_freq1;
    /// New channel center_freq2
    u16_l center_freq2;
};

/// Structure containing the parameters of the @ref MM_CSA_TRAFFIC_IND message
struct mm_csa_traffic_ind
{
    /// Index of the VIF
    u8_l vif_index;
    /// Is tx traffic enable or disable
    bool_l enable;
};

/// Structure containing the parameters of the @ref MM_MU_GROUP_UPDATE_REQ message.
/// Size allocated for the structure depends of the number of group
struct mm_mu_group_update_req
{
    /// Station index
    u8_l sta_idx;
    /// Number of groups the STA belongs to
    u8_l group_cnt;
    /// Group information
    struct
    {
        /// Group Id
        u8_l group_id;
        /// User position
        u8_l user_pos;
    } groups[0];
};

/// Structure containing the parameters of the @ref MM_INIT_COMPLETE message.
struct mm_init_complete_ind
{
    /// complete
    uint8_t complete;
};

/// Structure containing the parameters of the @ref MM_LA_INIT_IND
struct mm_init_la_ind
{
    uint8_t type;
    uint8_t enable;
};

/// Structure contiaining the parameters of the @ref MM_COOLING_TEMP message
struct mm_cooling_temp_req
{
    uint8_t max_pow_idx;
};

struct mm_change_power_req
{
    int  change_power;
    int change_power_trend;
};

struct mm_send_null_frame_req
{
    uint8_t sta_idx;
};

#define MAX_VIF_KEY_NUM 6
// structure size : 10
struct key_dying_gasp {
    // key index
    uint8_t key_idx;
    // hw key index in key storage ram
    uint8_t hw_key_idx;
    // tx package number
    uint64_t tx_pn;
};

// The size : 62 bytes
struct vif_dying_gasp {
    // vif index number
    uint8_t index;
    // How many valid key_dying_gasp ?
    uint8_t key_len;
    // key dying gasp information
    struct key_dying_gasp keys[MAX_VIF_KEY_NUM];
};

// The size: 30 bytes
struct sta_dying_gasp {
    // Station index number
    uint8_t index;
    // The sta belong to which vif
    uint8_t vif_index;
    // Key dying gasp information
    struct key_dying_gasp key;
    // record the sequence number
    uint16_t seq_nbr[TID_MAX];
    // dialog token
    uint8_t dialog_token_allocator;
};

// structure size : 1 + 1 + 4 * 62 + 64 * 30 = 2170
struct mm_dying_gasp
{
    // How many vifs dying gasp to be provided
    uint8_t vif_len;
    // How many stas dying gasp to be provided
    uint8_t sta_len;
    struct vif_dying_gasp vifs[NX_VIRT_DEV_MAX];
    struct sta_dying_gasp stas[NX_REMOTE_STA_MAX];
};

struct mm_dying_gasp_ind
{
    uint32_t dma_addr;
};

/// Structure containing the parameters of the @ref MM_UPDATE_IDLE_TIME_IND message
struct mm_update_idle_time_ind
{
    /// Index of the peer device that should update idle time
    u8_l sta_idx;
};

/// Structure containing the parameters of the @ref MM_CCA_DROP_STEP_IND message
struct mm_cca_drop_step_ind
{
    /// cca step
    uint8_t cca_step;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For Scan messages
///////////////////////////////////////////////////////////////////////////////
enum scan_msg_tag
{
    /// Scanning start Request.
    SCAN_START_REQ = LMAC_FIRST_MSG(TASK_SCAN),
    /// Scanning start Confirmation.
    SCAN_START_CFM,
    /// End of scanning indication.
    SCAN_DONE_IND,
    /// Cancel scan request
    SCAN_CANCEL_REQ,
    /// Cancel scan confirmation
    SCAN_CANCEL_CFM,

    /// MAX number of messages
    SCAN_MAX,
};

/// Maximum number of SSIDs in a scan request
#define SCAN_SSID_MAX   2

/// Maximum number of 2.4GHz channels
#define SCAN_CHANNEL_2G4 14

/// Maximum number of 5GHz channels
#define SCAN_CHANNEL_5G  28

/// Maximum number of channels in a scan request
#define SCAN_CHANNEL_MAX (SCAN_CHANNEL_2G4 + SCAN_CHANNEL_5G)

/// Flag bits
#define SCAN_PASSIVE_BIT BIT(0)
#define SCAN_DISABLED_BIT BIT(1)

/// Maximum number of PHY bands supported
#define SCAN_BAND_MAX 2

/// Definition of a channel to be scanned
struct scan_chan_tag
{
    /// Frequency of the channel
    u16_l freq;
    /// RF band (0: 2.4GHz, 1: 5GHz)
    u8_l band;
    /// Bit field containing additional information about the channel
    u8_l flags;
    /// Max tx_power for this channel (dBm)
    s8_l tx_power;
};

/// Structure containing the parameters of the @ref SCAN_START_REQ message
struct scan_start_req
{
    /// List of channel to be scanned
    struct scan_chan_tag chan[SCAN_CHANNEL_MAX];
    /// List of SSIDs to be scanned
    struct mac_ssid ssid[SCAN_SSID_MAX];
    /// BSSID to be scanned
    struct mac_addr bssid;
    /// Pointer (in host memory) to the additional IEs that need to be added to the ProbeReq
    /// (following the SSID element)
    u32_l add_ies;
    /// Length of the additional IEs
    u16_l add_ie_len;
    /// Index of the VIF that is scanning
    u8_l vif_idx;
    /// Number of channels to scan
    u8_l chan_cnt;
    /// Number of SSIDs to scan for
    u8_l ssid_cnt;
    /// no CCK - For P2P frames not being sent at CCK rate in 2GHz band.
    bool no_cck;
};

/// Structure containing the parameters of the @ref SCAN_START_CFM message
struct scan_start_cfm
{
    /// Status of the request
    u8_l status;
};

/// Structure containing the parameters of the @ref SCAN_CANCEL_REQ message
struct scan_cancel_req
{
};

/// Structure containing the parameters of the @ref SCAN_START_CFM message
struct scan_cancel_cfm
{
    /// Status of the request
    u8_l status;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For Scanu messages
///////////////////////////////////////////////////////////////////////////////
/// Messages that are logically related to the task.
enum
{
    /// Scan request from host.
    SCANU_START_REQ = LMAC_FIRST_MSG(TASK_SCANU),
    /// Scanning start Confirmation.
    SCANU_START_CFM,
    /// Join request
    SCANU_JOIN_REQ,
    /// Join confirmation.
    SCANU_JOIN_CFM,
    /// Scan result indication.
    SCANU_RESULT_IND,
    /// Fast scan request from any other module.
    SCANU_FAST_REQ,
    /// Confirmation of fast scan request.
    SCANU_FAST_CFM,

    /// MAX number of messages
    SCANU_MAX,
};

/// Structure containing the parameters of the @ref SCANU_START_REQ message
struct scanu_start_req
{
    /// List of channel to be scanned
    struct scan_chan_tag chan[SCAN_CHANNEL_MAX];
    /// List of SSIDs to be scanned
    struct mac_ssid ssid[SCAN_SSID_MAX];
    /// BSSID to be scanned (or WILDCARD BSSID if no BSSID is searched in particular)
    struct mac_addr bssid;
    /// Address (in host memory) of the additional IEs that need to be added to the ProbeReq
    /// (following the SSID element)
    u32_l add_ies;
    /// Length of the additional IEs
    u16_l add_ie_len;
    /// Index of the VIF that is scanning
    u8_l vif_idx;
    /// Number of channels to scan
    u8_l chan_cnt;
    /// Number of SSIDs to scan for
    u8_l ssid_cnt;
    /// no CCK - For P2P frames not being sent at CCK rate in 2GHz band.
    bool no_cck;
};

/// Structure containing the parameters of the @ref SCANU_START_CFM message
struct scanu_start_cfm
{
    /// Status of the request
    u8_l status;
};

/// Parameters of the @SCANU_RESULT_IND message
struct scanu_result_ind
{
    /// Length of the frame
    u16_l length;
    /// Frame control field of the frame.
    u16_l framectrl;
    /// Center frequency on which we received the packet
    u16_l center_freq;
    /// PHY band
    u8_l band;
    /// Index of the station that sent the frame. 0xFF if unknown.
    u8_l sta_idx;
    /// Index of the VIF that received the frame. 0xFF if unknown.
    u8_l inst_nbr;
    /// RSSI of the received frame.
    s8_l rssi;
    //for cache align,should be L1_CACHE_BYTES.but maybe different from lmac.so set 64 directly
    char cache_align[64];
    /// Frame payload.
    u32_l payload[];
};

/// Structure containing the parameters of the message.
struct scanu_fast_req
{
    /// The SSID to scan in the channel.
    struct mac_ssid ssid;
    /// BSSID.
    struct mac_addr bssid;
    /// Probe delay.
    u16_l probe_delay;
    /// Minimum channel time.
    u16_l minch_time;
    /// Maximum channel time.
    u16_l maxch_time;
    /// The channel number to scan.
    u16_l ch_nbr;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For ME messages
///////////////////////////////////////////////////////////////////////////////
/// Messages that are logically related to the task.
enum
{
    /// Configuration request from host.
    ME_CONFIG_REQ = LMAC_FIRST_MSG(TASK_ME),
    /// Configuration confirmation.
    ME_CONFIG_CFM,
    /// Configuration request from host.
    ME_CHAN_CONFIG_REQ,
    /// Configuration confirmation.
    ME_CHAN_CONFIG_CFM,
    /// Set control port state for a station.
    ME_SET_CONTROL_PORT_REQ,
    /// Control port setting confirmation.
    ME_SET_CONTROL_PORT_CFM,
    /// TKIP MIC failure indication.
    ME_TKIP_MIC_FAILURE_IND,
    /// Add a station to the FW (AP mode)
    ME_STA_ADD_REQ,
    /// Confirmation of the STA addition
    ME_STA_ADD_CFM,
    /// Delete a station from the FW (AP mode)
    ME_STA_DEL_REQ,
    /// Confirmation of the STA deletion
    ME_STA_DEL_CFM,
    /// Insert information in assoc req
    ME_ASSOC_INSERT_INFO_REQ,
    ///Insert information in auth
    ME_AUTH_INSERT_INFO_REQ,
    /// Indication of a TX RA/TID queue credit update
    ME_TX_CREDITS_UPDATE_IND,
    /// Request indicating to the FW that there is traffic buffered on host
    ME_TRAFFIC_IND_REQ,
    /// Confirmation that the @ref ME_TRAFFIC_IND_REQ has been executed
    ME_TRAFFIC_IND_CFM,
    /// Request of RC statistics to a station
    ME_RC_STATS_REQ,
    /// RC statistics confirmation
    ME_RC_STATS_CFM,
    /// RC fixed rate request
    ME_RC_SET_RATE_REQ,
    /// RC set no_ss request
    ME_RC_SET_NO_SS_REQ,
    /// Configure monitor interface
    ME_CONFIG_MONITOR_REQ,
    /// Configure monitor interface response
    ME_CONFIG_MONITOR_CFM,
    // Request to handle operation from ate tools
    ME_ATE_TOOLS_OP_REQ,
    // Confirmation that the @ref ME_ATE_TOOLS_OP_REQ has been executed
    ME_ATE_TOOLS_OP_CFM,
    /// MAX number of messages
    ME_MAX,
};

/// Structure containing the parameters of the @ref ME_START_REQ message
struct me_config_req
{
    /// HT Capabilities
    struct mac_htcapability ht_cap;
    /// VHT Capabilities
    struct mac_vhtcapability vht_cap;
    /// HE capabilities
    struct mac_hecapability he_cap;
    /// Lifetime of packets sent under a BlockAck agreement (expressed in TUs)
    u16_l tx_lft;
    /// Boolean indicating if HT is supported or not
    bool_l ht_supp;
    /// Boolean indicating if VHT is supported or not
    bool_l vht_supp;
    /// Boolean indicating if HE is supported or not
    bool_l he_supp;
    /// Boolean indicating if PS mode shall be enabled or not
    bool_l ps_on;
    /// Boolean indicating if Antenna Diversity shall be enabled or not
    bool_l ant_div_on;
    /// Rx memory buffer host memory allocate
    u32_l rx_bufsz;
    /// Rx payload offset in host skb buffer
    u32_l rx_pd_offset;
};

/// Structure containing the parameters of the @ref ME_CHAN_CONFIG_REQ message
struct me_chan_config_req
{
    /// List of 2.4GHz supported channels
    struct scan_chan_tag chan2G4[SCAN_CHANNEL_2G4];
    /// List of 5GHz supported channels
    struct scan_chan_tag chan5G[SCAN_CHANNEL_5G];
    /// Number of 2.4GHz channels in the list
    u8_l chan2G4_cnt;
    /// Number of 5GHz channels in the list
    u8_l chan5G_cnt;
};

/// Structure containing the parameters of the @ref ME_SET_CONTROL_PORT_REQ message
struct me_set_control_port_req
{
    /// Index of the station for which the control port is opened
    u8_l sta_idx;
    /// Control port state
    bool_l control_port_open;
};

/// Structure containing the parameters of the @ref ME_TKIP_MIC_FAILURE_IND message
struct me_tkip_mic_failure_ind
{
    /// Address of the sending STA
    struct mac_addr addr;
    /// TSC value
    u64_l tsc;
    /// Boolean indicating if the packet was a group or unicast one (true if group)
    bool_l ga;
    /// Key Id
    u8_l keyid;
    /// VIF index
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref ME_STA_ADD_REQ message
struct me_sta_add_req
{
    /// MAC address of the station to be added
    struct mac_addr mac_addr;
    /// Supported legacy rates
    struct mac_rateset rate_set;
    /// HT Capabilities
    struct mac_htcapability ht_cap;
    /// VHT Capabilities
    struct mac_vhtcapability vht_cap;
    /// HE capabilities
    struct mac_hecapability he_cap;
    /// Flags giving additional information about the station
    u32_l flags;
    /// Association ID of the station
    u16_l aid;
    /// Bit field indicating which queues have U-APSD enabled
    u8_l uapsd_queues;
    /// Maximum size, in frames, of a APSD service period
    u8_l max_sp_len;
    /// Operation mode information (valid if bit @ref STA_OPMOD_NOTIF is
    /// set in the flags)
    u8_l opmode;
    /// Index of the VIF the station is attached to
    u8_l vif_idx;
    /// Whether the the station is TDLS station
    bool_l tdls_sta;
    /// Indicate if the station is TDLS link initiator station
    bool_l tdls_sta_initiator;
    /// Indicate if the TDLS Channel Switch is allowed
    bool_l tdls_chsw_allowed;
    /// Indicate req index is valid or not
    bool_l req_index_valid;
    /// request station index
    u8_l req_index;
    /// Indicate if this station is add during
    bool_l recovering;
    /// sequence number per tid
    u16_l seq_nbr[TID_MAX];
    // add/del ba token number
    u8_l dialog_token_allocator;
};

/// Structure containing the parameters of the @ref ME_STA_ADD_CFM message
struct me_sta_add_cfm
{
    /// Station index
    u8_l sta_idx;
    /// Status of the station addition
    u8_l status;
    /// PM state of the station
    u8_l pm_state;
};

/// Structure containing the parameters of the @ref ME_STA_DEL_REQ message.
struct me_sta_del_req
{
    /// Index of the station to be deleted
    u8_l sta_idx;
    /// Whether the the station is TDLS station
    bool_l tdls_sta;
};

/// Structure containing the parameters of the @ref ME_TX_CREDITS_UPDATE_IND message.
struct me_tx_credits_update_ind
{
    /// Index of the station for which the credits are updated
    u8_l sta_idx;
    /// TID for which the credits are updated
    u8_l tid;
    /// Offset to be applied on the credit count
    s8_l credits;
};

/// Structure containing the parameters of the @ref ME_TRAFFIC_IND_REQ message.
struct me_traffic_ind_req
{
    /// Index of the station for which UAPSD traffic is available on host
    u8_l sta_idx;
    /// Flag indicating the availability of UAPSD packets for the given STA
    u8_l tx_avail;
    /// Indicate if traffic is on uapsd-enabled queues
    bool_l uapsd;
};

/// Structure containing the parameters of the @ref ME_RC_STATS_REQ message.
struct me_rc_stats_req
{
    /// Index of the station for which the RC statistics are requested
    u8_l sta_idx;
};

/// Structure containing the rate control statistics
struct rc_rate_stats
{
    /// Number of attempts (per sampling interval)
    u32_l attempts;
    /// Number of success (per sampling interval)
    u32_l success;
    /// Estimated probability of success (EWMA)
    u16_l probability;
    /// Rate configuration of the sample
    u16_l rate_config;
    /// Number of times the sample has been skipped (per sampling interval)
    u8_l sample_skipped;
    /// Whether the old probability is available
    bool_l old_prob_available;
    /// Number of times the AMPDU has been retried with this rate
    u8_l n_retry;
    // Whether the rate can be used in the retry chain
    bool_l rate_allowed;
};

/// Structure containing the parameters of the @ref ME_RC_STATS_CFM message.
struct me_rc_stats_cfm
{
    /// Index of the station for which the RC statistics are provided
    u8_l sta_idx;
    /// Number of samples used in the RC algorithm
    u16_l no_samples;
    /// Number of MPDUs transmitted (per sampling interval)
    u16_l ampdu_len;
    /// Number of AMPDUs transmitted (per sampling interval)
    u16_l ampdu_packets;
    /// Average number of MPDUs in each AMPDU frame (EWMA)
    u32_l avg_ampdu_len;
    // Current step 0 of the retry chain
    u8_l sw_retry_step;
    /// Trial transmission period
    u8_l sample_wait;
    /// Retry chain steps
    u16_l retry_step_idx[4];
    /// RC statistics
    struct rc_rate_stats rate_stats[10];
    /// Throughput
    u32_l tp[10];
};

/// Structure containing the parameters of the @ref ME_RC_SET_RATE_REQ message.
struct me_rc_set_rate_req
{
    /// Index of the station for which the fixed rate is set
    u8_l sta_idx;
    /// Rate configuration to be set
    u16_l fixed_rate_cfg;
};

/// Structure containing the parameters of the @ref ME_RC_SET_NO_SS_REQ message.
struct me_rc_set_no_ss_req
{
    /// number of spatial streams to set
    u8_l no_ss;
};

/// Structure containing the parameters of the @ref ME_ASSOC_INSERT_INFO_REQ message.
struct me_assoc_insert_info_req
{
    /// Length of inserted information
    u8_l info_dmalength;
    /// Dma address of insert information
    u32_l info_dmaaddr;
};

/// Structure containing the parameters of the @ref ME_AUTH_INSERT_INFO_REQ message.
struct me_auth_insert_info_req
{
    /// Length of inserted information
    u8_l info_dmalength;
    /// Dma address of insert information
    u32_l info_dmaaddr;
};

/// Structure containing the parameters of the @ref ME_CONFIG_MONITOR_REQ message.
struct me_config_monitor_req
{
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// Tx power for this channel (in dBm)
    s8_l tx_power;
};

/// Structure containing the parameters of the @ref ME_CONFIG_MONITOR_CFM message.
struct me_config_monitor_cfm
{
    /// Channel context index
    u8_l chan_index;
};

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
/// Structure containing the parameters of the @ref ME_ATE_TOOLS_OP_REQ message
struct me_ate_tools_op_req
{
    /// Control channel on which we have to enable the VIF
    struct scan_chan_tag chan;
    /// Center frequency of the first segment
    u32_l center_freq1;
    /// Center frequency of the second segment (only in 80+80 configuration)
    u32_l center_freq2;
    /// Width of channel
    u8_l ch_width;
    /// Index of the VIF which is started
    u8_l vif_idx;
    /// ATE TOOLS operation flag
    u8_l flag;
    /// Rate info
    /// 80211 mode
    u8_l mode;
    /// rate idx
    u8_l rate_idx;
    /// frame bw
    u8_l frame_bw;
    /// short preamble
    bool use_short_preamble;
    /// short gi
    bool use_short_gi;
    /// channel index
    u8_l ch_index;
    /// MAC address of the station to be added
    struct mac_addr sta_mac_addr;
    /// MAC address of the VIF to be added
    struct mac_addr vif_mac_addr;
};

/// Structure containing the parameters of the @ref ME_ATE_TOOLS_OP_CFM message.
struct me_ate_tools_op_cfm
{
    /// Status of the assigning chan_ctxt procedure
    u8_l status;
    /// Index of the VIF which is started
    u8_l vif_idx;
    /// Index of the channel context attached to the VIF
    u8_l ch_idx;
    /// Index of the STA used for BC/MC traffic
    u8_l bcmc_idx;
};
#endif

///////////////////////////////////////////////////////////////////////////////
/////////// For SM messages
///////////////////////////////////////////////////////////////////////////////
/// Message API of the SM task
enum sm_msg_tag
{
    /// Request to connect to an AP
    SM_CONNECT_REQ = LMAC_FIRST_MSG(TASK_SM),
    /// Confirmation of connection
    SM_CONNECT_CFM,
    /// Indicates that the SM associated to the AP
    SM_CONNECT_IND,
    /// Request to disconnect
    SM_DISCONNECT_REQ,
    /// Confirmation of disconnection
    SM_DISCONNECT_CFM,
    /// Indicates that the SM disassociated the AP
    SM_DISCONNECT_IND,
    /// Request to start external authentication
    SM_EXTERNAL_AUTH_REQUIRED_IND,
    /// Response to external authentication request
    SM_EXTERNAL_AUTH_REQUIRED_RSP,
    /// Indicates that the repeater_status about the sta vif
    SM_REPEATER_STATUS_IND,

    /// MAX number of messages
    SM_MAX,
};

/// Structure containing the parameters of the @ref SM_REPEATER_STATUS_IND message.
struct sm_repeater_status_ind
{
    /// Repeater status
    uint8_t status;
    /// VIF index
    uint8_t vif_idx;
};

/// Structure containing the parameters of @ref SM_CONNECT_REQ message.
struct sm_connect_req
{
    /// SSID to connect to
    struct mac_ssid ssid;
    /// BSSID to connect to (if not specified, set this field to WILDCARD BSSID)
    struct mac_addr bssid;
    /// Channel on which we have to connect (if not specified, set -1 in the chan.freq field)
    struct scan_chan_tag chan;
    /// Connection flags (see @ref sm_connect_flags)
    u32_l flags;
    /// Control port Ethertype (in network endianness)
    u16_l ctrl_port_ethertype;
    /// Length of the association request IEs
    u16_l ie_len;
    /// Listen interval to be used for this connection
    u16_l listen_interval;
    /// Flag indicating if the we have to wait for the BC/MC traffic after beacon or not
    bool_l dont_wait_bcmc;
    /// Authentication type
    u8_l auth_type;
    /// UAPSD queues (bit0: VO, bit1: VI, bit2: BE, bit3: BK)
    u8_l uapsd_queues;
    /// VIF index
    u8_l vif_idx;
    /// Buffer containing the additional information elements to be put in the
    /// association request
    u32_l ie_buf[64];
};

/// Structure containing the parameters of the @ref SM_CONNECT_CFM message.
struct sm_connect_cfm
{
    /// Status. If 0, it means that the connection procedure will be performed and that
    /// a subsequent @ref SM_CONNECT_IND message will be forwarded once the procedure is
    /// completed
    u8_l status;
};

#define SM_ASSOC_IE_LEN   800
/// Structure containing the parameters of the @ref SM_CONNECT_IND message.
struct sm_connect_ind
{
    /// Status code of the connection procedure
    u16_l status_code;
    /// BSSID
    struct mac_addr bssid;
    /// Flag indicating if the indication refers to an internal roaming or from a host request
    bool_l roamed;
    /// Index of the VIF for which the association process is complete
    u8_l vif_idx;
    /// Index of the STA entry allocated for the AP
    u8_l ap_idx;
    /// Index of the LMAC channel context the connection is attached to
    u8_l ch_idx;
    /// Flag indicating if the AP is supporting QoS
    bool_l qos;
    /// ACM bits set in the AP WMM parameter element
    u8_l acm;
    /// Length of the AssocReq IEs
    u16_l assoc_req_ie_len;
    /// Length of the AssocRsp IEs
    u16_l assoc_rsp_ie_len;
    /// IE buffer
    u32_l assoc_ie_buf[SM_ASSOC_IE_LEN/4];

    u16_l aid;
    u8_l band;
    u16_l center_freq;
    u8_l width;
    u32_l center_freq1;
    u32_l center_freq2;

    /// EDCA parameters
    u32_l ac_param[AC_MAX];
};

/// Structure containing the parameters of the @ref SM_DISCONNECT_REQ message.
struct sm_disconnect_req
{
    /// Reason of the deauthentication.
    u16_l reason_code;
    /// Index of the VIF.
    u8_l vif_idx;
};

/// Structure containing the parameters of SM_ASSOCIATION_IND the message
struct sm_association_ind
{
    // MAC ADDR of the STA
    struct mac_addr     me_mac_addr;
};


/// Structure containing the parameters of the @ref SM_DISCONNECT_IND message.
struct sm_disconnect_ind
{
    /// Reason of the disconnection.
    u16_l reason_code;
    /// Index of the VIF.
    u8_l vif_idx;
    /// FT over DS is ongoing
    bool_l ft_over_ds;
};

/// Structure containing the parameters of the @ref SM_EXTERNAL_AUTH_REQUIRED_IND
struct sm_external_auth_required_ind
{
    /// Index of the VIF.
    u8_l vif_idx;
    /// SSID to authenticate to
    struct mac_ssid ssid;
    /// BSSID to authenticate to
    struct mac_addr bssid;
    /// AKM suite of the respective authentication
    u32_l akm;
};

/// Structure containing the parameters of the @ref SM_EXTERNAL_AUTH_REQUIRED_RSP
struct sm_external_auth_required_rsp
{
    /// Index of the VIF.
    u8_l vif_idx;
    /// Authentication status
    u16_l status;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For APM messages
///////////////////////////////////////////////////////////////////////////////
/// Message API of the APM task
enum apm_msg_tag
{
    /// Request to start the AP.
    APM_START_REQ = LMAC_FIRST_MSG(TASK_APM),
    /// Confirmation of the AP start.
    APM_START_CFM,
    /// Request to stop the AP.
    APM_STOP_REQ,
    /// Confirmation of the AP stop.
    APM_STOP_CFM,
    /// Request to start CAC
    APM_START_CAC_REQ,
    /// Confirmation of the CAC start
    APM_START_CAC_CFM,
    /// Request to stop CAC
    APM_STOP_CAC_REQ,
    /// Confirmation of the CAC stop
    APM_STOP_CAC_CFM,
    /// for ap send deassoc in lmac
    APM_SEND_DEASSOC_REQ,
    /// for ap send deauth in lamc
    APM_SEND_DEAUTH_REQ,

    /// MAX number of messages
    APM_MAX,
};

/// Structure containing the parameters of the @ref APM_START_REQ message.
struct apm_start_req
{
    /// Basic rate set
    struct mac_rateset basic_rates;
    /// Control channel on which we have to enable the AP
    struct scan_chan_tag chan;
    /// Center frequency of the first segment
    u32_l center_freq1;
    /// Center frequency of the second segment (only in 80+80 configuration)
    u32_l center_freq2;
    /// Width of channel
    u8_l ch_width;
    /// whether is radar_channel
    bool is_radar_channel;
    /// Address, in host memory, to the beacon template
    u32_l bcn_addr;
    /// Length of the beacon template
    u16_l bcn_len;
    /// Offset of the TIM IE in the beacon
    u16_l tim_oft;
    /// Beacon interval
    u16_l bcn_int;
    /// Flags
    u32_l flags;
    /// Control port Ethertype
    u16_l ctrl_port_ethertype;
    /// Length of the TIM IE
    u8_l tim_len;
    /// Index of the VIF for which the AP is started
    u8_l vif_idx;
    /// If this start is caused by recoverying
    u8_l recovering;
};

/// Structure containing the parameters of the @ref APM_START_CFM message.
struct apm_start_cfm
{
    /// Status of the AP starting procedure
    u8_l status;
    /// Index of the VIF for which the AP is started
    u8_l vif_idx;
    /// Index of the channel context attached to the VIF
    u8_l ch_idx;
    /// Index of the STA used for BC/MC traffic
    u8_l bcmc_idx;
};

/// Structure containing the parameters of the @ref APM_STOP_REQ message.
struct apm_stop_req
{
    /// Index of the VIF for which the AP has to be stopped
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref APM_START_CAC_REQ message.
struct apm_start_cac_req
{
    /// Control channel on which we have to start the CAC
    struct scan_chan_tag chan;
    /// Center frequency of the first segment
    u32_l center_freq1;
    /// Center frequency of the second segment (only in 80+80 configuration)
    u32_l center_freq2;
    /// Width of channel
    u8_l ch_width;
    /// Index of the VIF for which the CAC is started
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref APM_START_CAC_CFM message.
struct apm_start_cac_cfm
{
    /// Status of the CAC starting procedure
    u8_l status;
    /// Index of the channel context attached to the VIF for CAC
    u8_l ch_idx;
};

/// Structure containing the parameters of the @ref APM_STOP_CAC_REQ message.
struct apm_stop_cac_req
{
    /// Index of the VIF for which the CAC has to be stopped
    u8_l vif_idx;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For MESH messages
///////////////////////////////////////////////////////////////////////////////

/// Maximum length of the Mesh ID
#define MESH_MESHID_MAX_LEN     (32)

/// Message API of the MESH task
enum mesh_msg_tag
{
    /// Request to start the MP
    MESH_START_REQ = LMAC_FIRST_MSG(TASK_MESH),
    /// Confirmation of the MP start.
    MESH_START_CFM,

    /// Request to stop the MP.
    MESH_STOP_REQ,
    /// Confirmation of the MP stop.
    MESH_STOP_CFM,

    // Request to update the MP
    MESH_UPDATE_REQ,
    /// Confirmation of the MP update
    MESH_UPDATE_CFM,

    /// Request information about a given link
    MESH_PEER_INFO_REQ,
    /// Response to the MESH_PEER_INFO_REQ message
    MESH_PEER_INFO_CFM,

    /// Request automatic establishment of a path with a given mesh STA
    MESH_PATH_CREATE_REQ,
    /// Confirmation to the MESH_PATH_CREATE_REQ message
    MESH_PATH_CREATE_CFM,

    /// Request a path update (delete path, modify next hop mesh STA)
    MESH_PATH_UPDATE_REQ,
    /// Confirmation to the MESH_PATH_UPDATE_REQ message
    MESH_PATH_UPDATE_CFM,

    /// Indication from Host that the indicated Mesh Interface is a proxy for an external STA
    MESH_PROXY_ADD_REQ,

    /// Indicate that a connection has been established or lost
    MESH_PEER_UPDATE_IND,
    /// Notification that a connection has been established or lost (when MPM handled by userspace)
    MESH_PEER_UPDATE_NTF = MESH_PEER_UPDATE_IND,

    /// Indicate that a path is now active or inactive
    MESH_PATH_UPDATE_IND,
    /// Indicate that proxy information have been updated
    MESH_PROXY_UPDATE_IND,

    /// MAX number of messages
    MESH_MAX,
};

/// Structure containing the parameters of the @ref MESH_START_REQ message.
struct mesh_start_req
{
    /// Basic rate set
    struct mac_rateset basic_rates;
    /// Control channel on which we have to enable the AP
    struct scan_chan_tag chan;
    /// Center frequency of the first segment
    u32_l center_freq1;
    /// Center frequency of the second segment (only in 80+80 configuration)
    u32_l center_freq2;
    /// Width of channel
    u8_l ch_width;
    /// DTIM Period
    u8_l dtim_period;
    /// Beacon Interval
    u16_l bcn_int;
    /// Index of the VIF for which the MP is started
    u8_l vif_index;
    /// Length of the Mesh ID
    u8_l mesh_id_len;
    /// Mesh ID
    u8_l mesh_id[MESH_MESHID_MAX_LEN];
    /// Address of the IEs to download
    u32_l ie_addr;
    /// Length of the provided IEs
    u8_l ie_len;
    /// Indicate if Mesh Peering Management (MPM) protocol is handled in userspace
    bool_l user_mpm;
    /// Indicate if Mesh Point is using authentication
    bool_l is_auth;
    /// Indicate which authentication method is used
    u8_l auth_id;
};

/// Structure containing the parameters of the @ref MESH_START_CFM message.
struct mesh_start_cfm
{
    /// Status of the MP starting procedure
    u8_l status;
    /// Index of the VIF for which the MP is started
    u8_l vif_idx;
    /// Index of the channel context attached to the VIF
    u8_l ch_idx;
    /// Index of the STA used for BC/MC traffic
    u8_l bcmc_idx;
};

/// Structure containing the parameters of the @ref MESH_STOP_REQ message.
struct mesh_stop_req
{
    /// Index of the VIF for which the MP has to be stopped
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref MESH_STOP_CFM message.
struct mesh_stop_cfm
{
    /// Index of the VIF for which the MP has to be stopped
    u8_l vif_idx;
   /// Status
    u8_l status;
};

/// Bit fields for mesh_update_req message's flags value
enum mesh_update_flags_bit
{
    /// Root Mode
    MESH_UPDATE_FLAGS_ROOT_MODE_BIT = 0,
    /// Gate Mode
    MESH_UPDATE_FLAGS_GATE_MODE_BIT,
    /// Mesh Forwarding
    MESH_UPDATE_FLAGS_MESH_FWD_BIT,
    /// Local Power Save Mode
    MESH_UPDATE_FLAGS_LOCAL_PSM_BIT,
};

/// Structure containing the parameters of the @ref MESH_UPDATE_REQ message.
struct mesh_update_req
{
    /// Flags, indicate fields which have been updated
    u8_l flags;
    /// VIF Index
    u8_l vif_idx;
    /// Root Mode
    u8_l root_mode;
    /// Gate Announcement
    bool_l gate_announ;
    /// Mesh Forwarding
    bool_l mesh_forward;
    /// Local PS Mode
    u8_l local_ps_mode;
};

/// Structure containing the parameters of the @ref MESH_UPDATE_CFM message.
struct mesh_update_cfm
{
    /// Status
    u8_l status;
};

/// Structure containing the parameters of the @ref MESH_PEER_INFO_REQ message.
struct mesh_peer_info_req
{
    ///Index of the station allocated for the peer
    u8_l sta_idx;
};

/// Structure containing the parameters of the @ref MESH_PEER_INFO_CFM message.
struct mesh_peer_info_cfm
{
    /// Response status
    u8_l status;
    /// Index of the station allocated for the peer
    u8_l sta_idx;
    /// Local Link ID
    u16_l local_link_id;
    /// Peer Link ID
    u16_l peer_link_id;
    /// Local PS Mode
    u8_l local_ps_mode;
    /// Peer PS Mode
    u8_l peer_ps_mode;
    /// Non-peer PS Mode
    u8_l non_peer_ps_mode;
    /// Link State
    u8_l link_state;
};

/// Structure containing the parameters of the @ref MESH_PATH_CREATE_REQ message.
struct mesh_path_create_req
{
    /// Index of the interface on which path has to be created
    u8_l vif_idx;
    /// Indicate if originator MAC Address is provided
    bool_l has_orig_addr;
    /// Path Target MAC Address
    struct mac_addr tgt_mac_addr;
    /// Originator MAC Address
    struct mac_addr orig_mac_addr;
};

/// Structure containing the parameters of the @ref MESH_PATH_CREATE_CFM message.
struct mesh_path_create_cfm
{
    /// Confirmation status
    u8_l status;
    /// VIF Index
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref MESH_PATH_UPDATE_REQ message.
struct mesh_path_update_req
{
    /// Indicate if path must be deleted
    bool_l delete;
    /// Index of the interface on which path has to be created
    u8_l vif_idx;
    /// Path Target MAC Address
    struct mac_addr tgt_mac_addr;
    /// Next Hop MAC Address
    struct mac_addr nhop_mac_addr;
};

/// Structure containing the parameters of the @ref MESH_PATH_UPDATE_CFM message.
struct mesh_path_update_cfm
{
    /// Confirmation status
    u8_l status;
    /// VIF Index
    u8_l vif_idx;
};

/// Structure containing the parameters of the @ref MESH_PROXY_ADD_REQ message.
struct mesh_proxy_add_req
{
    /// VIF Index
    u8_l vif_idx;
    /// MAC Address of the External STA
    struct mac_addr ext_sta_addr;
};

/// Structure containing the parameters of the @ref MESH_PROXY_UPDATE_IND
struct mesh_proxy_update_ind
{
    /// Indicate if proxy information has been added or deleted
    bool_l delete;
    /// Indicate if we are a proxy for the external STA
    bool_l local;
    /// VIF Index
    u8_l vif_idx;
    /// MAC Address of the External STA
    struct mac_addr ext_sta_addr;
    /// MAC Address of the proxy (only valid if local is false)
    struct mac_addr proxy_mac_addr;
};

/// Structure containing the parameters of the @ref MESH_PEER_UPDATE_IND message.
struct mesh_peer_update_ind
{
    /// Indicate if connection has been established or lost
    bool_l estab;
    /// VIF Index
    u8_l vif_idx;
    /// STA Index
    u8_l sta_idx;
    /// Peer MAC Address
    struct mac_addr peer_addr;
};

/// Structure containing the parameters of the @ref MESH_PEER_UPDATE_NTF message.
struct mesh_peer_update_ntf
{
    /// VIF Index
    u8_l vif_idx;
    /// STA Index
    u8_l sta_idx;
    /// Mesh Link State
    u8_l state;
};

/// Structure containing the parameters of the @ref MESH_PATH_UPDATE_IND message.
struct mesh_path_update_ind
{
    /// Indicate if path is deleted or not
    bool_l delete;
    /// Indicate if path is towards an external STA (not part of MBSS)
    bool_l ext_sta;
    /// VIF Index
    u8_l vif_idx;
    /// Path Index
    u8_l path_idx;
    /// Target MAC Address
    struct mac_addr tgt_mac_addr;
    /// External STA MAC Address (only if ext_sta is true)
    struct mac_addr ext_sta_mac_addr;
    /// Next Hop STA Index
    u8_l nhop_sta_idx;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For Debug messages
///////////////////////////////////////////////////////////////////////////////

/// Messages related to Debug Task
enum dbg_msg_tag
{
    /// Memory read request
    DBG_MEM_READ_REQ = LMAC_FIRST_MSG(TASK_DBG),
    /// Memory read confirm
    DBG_MEM_READ_CFM,
    /// Memory write request
    DBG_MEM_WRITE_REQ,
    /// Memory write confirm
    DBG_MEM_WRITE_CFM,
    /// Module filter request
    DBG_SET_MOD_FILTER_REQ,
    /// Module filter confirm
    DBG_SET_MOD_FILTER_CFM,
    /// Severity filter request
    DBG_SET_SEV_FILTER_REQ,
    /// Severity filter confirm
    DBG_SET_SEV_FILTER_CFM,
    /// LMAC/MAC HW fatal error indication
    DBG_ERROR_IND,
    /// Request to get system statistics
    DBG_GET_SYS_STAT_REQ,
    /// COnfirmation of system statistics
    DBG_GET_SYS_STAT_CFM,
    /// Request to get tx statistics
    DBG_GET_TX_STAT_REQ,
    /// Confirmation of tx statistics
    DBG_GET_TX_STAT_CFM,
    /// Request to get rx statistics
    DBG_GET_RX_STAT_REQ,
    /// Confirmation of rx statistics
    DBG_GET_RX_STAT_CFM,
#ifdef NEW_SCHEDULE
    DBG_PRINT_TX_BURST_INFO_REQ,
    ///
    DBG_PRINT_TX_BURST_INFO_CFM,
#endif
#ifdef CONFIG_VDR_HW
    /// Request to get vendor information
    DBG_GET_VDR_INFO_REQ,
    /// Confirmation of vendor information
    DBG_GET_VDR_INFO_CFM,
#endif
    /// Request to get last statistis in period from lmac
    DBG_VDR_GET_MP_INFO_REQ,
    /// Confirmation of vendor get statistis in period from lmac
    DBG_VDR_GET_MP_INFO_CFM,
    /// Request to get last statistis in period from lmac
    DBG_GET_MGMT_INFO_REQ,
    /// Confirmation of vendor get statistis in period from lmac
    DBG_GET_MGMT_INFO_CFM,
    /// Request to get last statistis in period from lmac
    DBG_GET_CTRL_INFO_REQ,
    /// Confirmation of vendor get statistis in period from lmac
    DBG_GET_CTRL_INFO_CFM,
    /// Request to set cca parameter
    DBG_SET_CCA_PARAMETER_REQ,
    /// Max number of Debug messages
    DBG_MAX,
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_REQ message.
struct dbg_mem_read_req
{
    u32_l memaddr;
};

/// Structure containing the parameters of the @ref DBG_MEM_READ_CFM message.
struct dbg_mem_read_cfm
{
    u32_l memaddr;
    u32_l memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_REQ message.
struct dbg_mem_write_req
{
    u32_l memaddr;
    u32_l memdata;
};

/// Structure containing the parameters of the @ref DBG_MEM_WRITE_CFM message.
struct dbg_mem_write_cfm
{
    u32_l memaddr;
    u32_l memdata;
};

/// Structure containing the parameters of the @ref DBG_SET_MOD_FILTER_REQ message.
struct dbg_set_mod_filter_req
{
    /// Bit field indicating for each module if the traces are enabled or not
    u32_l mod_filter;
};

/// Structure containing the parameters of the @ref DBG_SEV_MOD_FILTER_REQ message.
struct dbg_set_sev_filter_req
{
    /// Bit field indicating the severity threshold for the traces
    u32_l sev_filter;
};

/// Structure containing the parameters of the @ref DBG_GET_SYS_STAT_CFM message.
struct dbg_get_sys_stat_cfm
{
    /// Time spent in CPU sleep since last reset of the system statistics
    u32_l cpu_sleep_time;
    /// Time spent in DOZE since last reset of the system statistics
    u32_l doze_time;
    /// Total time spent since last reset of the system statistics
    u32_l stats_time;
};

/// Structure containing the parameters of the @ref DBG_GET_TX_STAT_REQ message.
struct dbg_get_tx_stat_req
{
    /// Bit field indicating if we need to clear the trx status after read
    u32_l clear;
    u32_l cfm_dma_addr;
};

struct dbg_tx_stat
{
    /// Whole frames txed: s_mpdu_tx + ampdu_whole_cnt
    /// Number of singleton mpdu
    u32_l s_mpdu_tx[AC_MAX];
    u32_l s_mpdu_successful[AC_MAX];
    /// Record the ampdu aggregation info
    u32_l a_mpdu_count[AC_MAX][64];
    u32_l a_mpdu_successful[AC_MAX][64];
    u32_l agg_finish_reason[8];
	u32_l pkt_cnt;
	u32_l transmitting_list_cnt[AC_MAX];
	u32_l cfm_list_cnt[AC_MAX];
	u32_l transmitting_list_add_cnt[AC_MAX];
	u32_l cfmlist_add_cnt[AC_MAX];
	u32_l discard_cnt[AC_MAX];
	u32_l total_transmitting_list_add_cnt[AC_MAX];
	u32_l total_cfmlist_add_cnt[AC_MAX];
	u32_l total_discard_cnt[AC_MAX];
	u32_l total_flush_cnt[AC_MAX];
    //reocord different type of cnt from lmac
	u32_l record_cnt[32];
    //stastics beacon tx failed cnt
    u32_l bcn_failed_cnt;
};

/// Structure containing the parameters used for the tx status
struct dbg_get_tx_stat_cfm
{
    // Point to the struct dbg_tx_stat content
    u32_l dma_addr;
};


/// Structure containing the parameters of the @ref DBG_GET_TX_STAT_REQ message.
struct dbg_get_rx_stat_req
{
    /// Bit field indicating if we need to clear the trx status after read
    u32_l clear;
    u32_l cfm_dma_addr;
};

/// Structure containing the parameters used for the rx status
struct dbg_rx_stat
{
    u32_l hostbuf_not_available;
    u32_l total;
    u32_l mib;
    u32_l upload;
    u32_l amsdu;
    u32_l key_invalid_spurious_data;
    u32_l key_invalid_duplicate_frame;
    u32_l key_invalid_spurious_mgmt;
    u32_l key_invalid_mgmt_handled;
    u32_l key_invalid_mgmt_upload;
    u32_l sta_nonregister;
    u32_l encrypted_fail;
    u32_l ctrl_frame;
    u32_l mgt_frame;
    u32_l data_frame;
    u32_l data_frame_reord_discard;
    u32_l data_frame_reord_duplicate_discard;
    u32_l data_frame_reord_pn_discard;
    u32_l data_frame_own_sa_discard;
    u32_l data_frame_mpdu1_discard;
    u32_l data_frame_duplicate_discard;
    u32_l data_frame_pn_failed_discard;
    u32_l defrag_frame;
    u32_l rx_pddesc_used_idx;
    u32_l rx_pddesc_free_idx;
    u32_l max_msdu_idx;
    u32_l max_dma_idx;
    u32_l max_used_pddesc;

    //For fragment attack
    u32_l cve_2020_26140;
    u32_l cve_2020_24588;
    u32_l cve_2020_24587;
    u32_l cve_2020_26146;

    //The length of MTU is greater than 1500
    uint32_t jumbo_frame;
    u32_l data_frame_pn_failed_upload;
    uint32_t reord_alloc_cnt;
    uint32_t reord_release_cnt;
    uint32_t reord_hostid_nozero_cnt;
};
struct dbg_get_rx_stat_cfm
{
    // Point to the struct dbg_tx_stat content
    u32_l dma_addr;
};

struct dbg_get_mgmt_info_req
{
    uint32_t clear;
    uint32_t cfm_dma_addr;
};
struct dbg_get_mgmt_info_cfm
{
    uint32_t dma_addr;
};
struct dbg_mgmt_info {
    //mgmt tx
    uint32_t assocreq_tx_packets;
    uint32_t assocrsp_tx_packets;
    uint32_t probereq_tx_packets;
    uint32_t probersp_tx_packets;
    uint64_t beacon_tx_packets;
    uint32_t authent_tx_packets;
    uint32_t deauthent_tx_packets;
    uint32_t disassoc_tx_packets;
    uint32_t action_tx_packets;
    //mgmt rx
    uint32_t assocreq_rx_packets;
    uint32_t assocrsp_rx_packets;
    uint32_t probereq_rx_packets;
    uint32_t probersp_rx_packets;
    uint64_t beacon_rx_packets;
    uint32_t authent_rx_packets;
    uint32_t deauthent_rx_packets;
    uint32_t disassoc_rx_packets;
    uint32_t action_rx_packets;
};
struct dbg_get_ctrl_info_req
{
    uint32_t clear;
    uint32_t cfm_dma_addr;
};
struct dbg_get_ctrl_info_cfm
{
    uint32_t dma_addr;
};
struct dbg_ctrl_info {
    //ctrl tx
    uint32_t ba_tx_packets;
    uint32_t bar_tx_packets;
    uint32_t pspoll_tx_packets;
    uint32_t rts_tx_packets;
    uint32_t cts_tx_packets;
    uint32_t ack_tx_packets;
    uint32_t cfend_tx_packets;
    //ctrlr rx
    uint32_t ba_rx_packets;
    uint32_t bar_rx_packets;
    uint32_t pspoll_rx_packets;
    uint32_t rts_rx_packets;
    uint32_t cts_rx_packets;
    uint32_t ack_rx_packets;
    uint32_t cfend_rx_packets;
};

/// Structure containing the parameters of the @ref MM_SET_CCA_PARAMETER message.
struct dbg_set_cca_parameter_req
{
    //set_cca
    uint8_t set_cca_parameter_enable;
    //BK
    uint32_t bk_timeout;
    //BE
    uint32_t be_timeout;
    //VI
    uint32_t vi_timeout;
    //VO
    uint32_t vo_timeout;
    //BCN
    uint32_t bcn_timeout;
    //CCA threshold
    uint8_t cca_threshold_rise;
    //CCA threshold
    uint8_t cca_threshold_fall;
    //Trigger interval
    uint8_t trigger_interval_low;
    //Trigger interval
    uint8_t trigger_interval_high;
    //DROP_STEP_DISTANCE
    uint8_t step_max;
    //STEP_MAX
    uint8_t step_distance;
    //default CCA threshold rise when phy init. for Struct alignment
    uint8_t default_cca_threshold_rise;
    //default CCA threshold fall when phy init. for Struct alignment
    uint8_t default_cca_threshold_fall;
};

#ifdef NEW_SCHEDULE
struct dbg_print_burst_info_req
{
    int32_t ac;
};

struct dbg_print_burst_info_cfm
{
    uint32_t reserved;
};
#endif

#ifdef CONFIG_VDR_HW
struct dbg_get_vendor_info_req
{
    /// Bit field indicating if we need to clear the trx status after read
    uint32_t clear;
    uint8_t vif_idx;
    uint8_t sta_idx;
};

struct dbg_get_vendor_info_cfm
{
    int32_t value;
    uint8_t ampdu_num; // max ampdu number in BE queue
    uint8_t amsdu_on; // amsdu status in BE queue
    uint8_t rts_rate; // rts rate config
    uint8_t rts_retry_cnt; // rts retry count
    uint16_t tx_rate; // current tx rate config
    uint16_t tx_prev_rate; // previous tx rate config
    uint16_t min_tx_rate; // min tx rate config in unit time
    uint16_t max_tx_rate; // max tx rate config in unit time
    uint32_t tx_retry_cnt;
    uint32_t rx_seq_jump_cnt;
};
#endif

struct dbg_get_vendor_mp_info_req
{
    uint32_t params;
};

struct dbg_get_vendor_mp_info_cfm
{
    //cca edca busy time(us) in last period
    uint32_t cca_busy_time_us;
    //cca 20s busy time(us) in last period
    uint32_t cca20_busy_time_us;
    //cca 40s busy time(us) in last period
    uint32_t cca40_busy_time_us;
    //whole rx time in last period
    uint32_t rx_frame_time_us;
    //whole tx time in last period
    uint32_t tx_frame_time_us;
    //while cal period
    uint32_t last_cal_time_us;
    // last noise0 dbm for current channel
    int8_t last_noise0_dbm;
    // last noise1 dbm for current channel
    int8_t last_noise1_dbm;
    //cpu idle time in last period
    uint32_t cpu_idle_time_us;
};

/// Struct containing the paramers of @ref APM_SEND_DEASSOC_REQ
struct apm_send_deassoc_req{
    uint8_t vif_idx;
    uint8_t macaddr[6];
    uint16_t reason_code;
};

/// Struct containing the paramers of @ref APM_SEND_DEAUTH_REQ
struct apm_send_deauth_req{
    uint8_t vif_idx;
    uint8_t macaddr[6];
    uint16_t reason_code;
};

/// Struct containing the paramers of @ref MM_GET_CHANNEL_INFO_REQ
struct mm_get_channel_info_req {
    uint32_t params;
};
/// Struct containing the paramers of @ref MM_GET_CHANNEL_INFO_CFM
struct mm_get_channel_info_cfm {
    uint8_t channel_idx;
    uint8_t channel_width;
    uint16_t center_freq1;
    uint16_t center_freq2;
    uint32_t channel_freq;
};

/// Structure containing the paramters of the @ref DBG_ERROR_IND message
struct dbg_error_ind
{
    /// Bit field indiicating that the dbg info is valid or not
    uint8_t dump_en;
    /// Bit field indicating which error lmac meet
    uint32_t error_type;
};

///////////////////////////////////////////////////////////////////////////////
/////////// For TDLS messages
///////////////////////////////////////////////////////////////////////////////

/// List of messages related to the task.
enum tdls_msg_tag
{
    /// TDLS channel Switch Request.
    TDLS_CHAN_SWITCH_REQ = LMAC_FIRST_MSG(TASK_TDLS),
    /// TDLS channel switch confirmation.
    TDLS_CHAN_SWITCH_CFM,
    /// TDLS channel switch indication.
    TDLS_CHAN_SWITCH_IND,
    /// TDLS channel switch to base channel indication.
    TDLS_CHAN_SWITCH_BASE_IND,
    /// TDLS cancel channel switch request.
    TDLS_CANCEL_CHAN_SWITCH_REQ,
    /// TDLS cancel channel switch confirmation.
    TDLS_CANCEL_CHAN_SWITCH_CFM,
    /// TDLS peer power save indication.
    TDLS_PEER_PS_IND,
    /// TDLS peer traffic indication request.
    TDLS_PEER_TRAFFIC_IND_REQ,
    /// TDLS peer traffic indication confirmation.
    TDLS_PEER_TRAFFIC_IND_CFM,
    /// MAX number of messages
    TDLS_MAX
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_REQ message
struct tdls_chan_switch_req
{
    /// Index of the VIF
    u8_l vif_index;
    /// STA Index
    u8_l sta_idx;
    /// MAC address of the TDLS station
    struct mac_addr peer_mac_addr;
    bool_l initiator;
    /// Band (2.4GHz or 5GHz)
    u8_l band;
    /// Channel type: 20,40,80,160 or 80+80 MHz
    u8_l type;
    /// Frequency for Primary 20MHz channel (in MHz)
    u16_l prim20_freq;
    /// Frequency for Center of the contiguous channel or center of Primary 80+80
    u16_l center1_freq;
    /// Frequency for Center of the non-contiguous secondary 80+80
    u16_l center2_freq;
    /// TX power (in dBm)
    s8_l tx_power;
    /// Operating class
    u8_l op_class;
};

/// Structure containing the parameters of the @ref TDLS_CANCEL_CHAN_SWITCH_REQ message
struct tdls_cancel_chan_switch_req
{
    /// Index of the VIF
    u8_l vif_index;
    /// STA Index
    u8_l sta_idx;
    /// MAC address of the TDLS station
    struct mac_addr peer_mac_addr;
};


/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_CFM message
struct tdls_chan_switch_cfm
{
    /// Status of the operation
    u8_l status;
};

/// Structure containing the parameters of the @ref TDLS_CANCEL_CHAN_SWITCH_CFM message
struct tdls_cancel_chan_switch_cfm
{
    /// Status of the operation
    u8_l status;
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_IND message
struct tdls_chan_switch_ind
{
    /// VIF Index
    u8_l vif_index;
    /// Channel Context Index
    u8_l chan_ctxt_index;
    /// Status of the operation
    u8_l status;
};

/// Structure containing the parameters of the @ref TDLS_CHAN_SWITCH_BASE_IND message
struct tdls_chan_switch_base_ind
{
    /// VIF Index
    u8_l vif_index;
    /// Channel Context index
    u8_l chan_ctxt_index;
};

/// Structure containing the parameters of the @ref TDLS_PEER_PS_IND message
struct tdls_peer_ps_ind
{
    /// VIF Index
    u8_l vif_index;
    /// STA Index
    u8_l sta_idx;
    /// MAC ADDR of the TDLS STA
    struct mac_addr peer_mac_addr;
    /// Flag to indicate if the TDLS peer is going to sleep
    bool ps_on;
};

/// Structure containing the parameters of the @ref TDLS_PEER_TRAFFIC_IND_REQ message
struct tdls_peer_traffic_ind_req
{
    /// VIF Index
    u8_l vif_index;
    /// STA Index
    u8_l sta_idx;
    // MAC ADDR of the TDLS STA
    struct mac_addr peer_mac_addr;
    /// Dialog token
    u8_l dialog_token;
    /// TID of the latest MPDU transmitted over the TDLS direct link to the TDLS STA
    u8_l last_tid;
    /// Sequence number of the latest MPDU transmitted over the TDLS direct link
    /// to the TDLS STA
    u16_l last_sn;
};

/// Structure containing the parameters of the @ref TDLS_PEER_TRAFFIC_IND_CFM message
struct tdls_peer_traffic_ind_cfm
{
    /// Status of the operation
    u8_l status;
};

// Structure containing information about radar pulse detected
struct phy_radar_pulse
{
// In our PHY a radar pulse is only one 32-bit word
    uint32_t pulse;
};
#endif // LMAC_MSG_H_
