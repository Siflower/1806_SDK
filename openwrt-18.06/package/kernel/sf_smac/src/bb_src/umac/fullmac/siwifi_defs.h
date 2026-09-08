/**
 ******************************************************************************
 *
 * @file siwifi_defs.h
 *
 * @brief Main driver structure declarations for fullmac driver
 *
 * Copyright (C) Siflower 2018-2025
 *
 ******************************************************************************
 */

#ifndef _SIWIFI_DEFS_H_
#define _SIWIFI_DEFS_H_

#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/dmapool.h>
#include <linux/skbuff.h>
#include <net/cfg80211.h>
#include <linux/slab.h>
#include <net/mac80211.h>

#include "siwifi_mod_params.h"
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
#include "siwifi_debugfs.h"
#endif
#include "siwifi_tx.h"
#include "siwifi_rx.h"
#include "siwifi_radar.h"
#include "siwifi_utils.h"
#include "siwifi_mu_group.h"
#include "rf_pl_ref.h"
//todo:ycchang: sync with siwifi_platform.h
#if 1
#include "siwifi_v1.h"
#else
#include "siwifi_platform.h"
#endif
#include "siwifi_cmds.h"

#ifdef CONFIG_SIWIFI_TX_POWER_CALI
#include "siwifi_factory_info.h"
#endif

#ifdef CONFIG_SF19A28_WIFI_LED
#include <linux/leds.h>
#endif

#ifdef CONFIG_SIWIFI_IGMP
#include "siwifi_igmp.h"
#endif

#include "siwifi_trace.h"

#ifdef CONFIG_SIWIFI_REPEATER
#include "siwifi_repeater.h"
#endif

#define WPI_HDR_LEN    18
#define WPI_PN_LEN     16
#define WPI_PN_OFST     2
#define WPI_MIC_LEN    16
#define WPI_KEY_LEN    32
#define WPI_SUBKEY_LEN 16 // WPI key is actually two 16bytes key

#define LEGACY_PS_ID   0
#define UAPSD_ID       1

#define LB_MODULE 1 //bit(0)
#define HB_MODULE 2 //bit(1)

#define STA_HASH_SIZE 256
#define STA_HASH(sta) (sta[3])

#define DEBUG_ARRAY_CHECK 1

#ifdef CONFIG_SIWIFI_SAVE_TXHDR_ALLOC
#define SIWIFI_TX_HEADROOM (uint32_t)(sizeof(struct siwifi_txhdr) + SIWIFI_SWTXHDR_ALIGN_SZ + sizeof(struct siwifi_sw_txhdr))
#else
#define SIWIFI_TX_HEADROOM  (uint32_t)(sizeof(struct siwifi_txhdr) + SIWIFI_SWTXHDR_ALIGN_SZ)
#endif

#ifdef CONFIG_SIWIFI_AMSDUS_TX
#define SIWIFI_TX_AMSDU_HEADROOM ((uint32_t)(sizeof(struct siwifi_amsdu_txhdr) + sizeof(struct ethhdr) + 4 + sizeof(rfc1042_header) + 2))
#else
#define SIWIFI_TX_AMSDU_HEADROOM (uint32_t)(0)
#endif

#define SIWIFI_RX_HEADROOM (uint32_t)((sizeof(struct hw_rxhdr) + 2))

// 30 seconds
#define SRC_FILTER_AGING_TIME 30

#define SIWIFI_USER_TID_NOT_SET 0xFF

/**
 * struct siwifi_bcn - Information of the beacon in used (AP mode)
 *
 * @head: head portion of beacon (before TIM IE)
 * @tail: tail portion of beacon (after TIM IE)
 * @ies: extra IEs (not used ?)
 * @head_len: length of head data
 * @tail_len: length of tail data
 * @ies_len: length of extra IEs data
 * @tim_len: length of TIM IE
 * @len: Total beacon len (head + tim + tail + extra)
 * @dtim: dtim period
 */
struct siwifi_bcn {
    u8 *head;
    u8 *tail;
    u8 *ies;
    size_t head_len;
    size_t tail_len;
    size_t ies_len;
    size_t tim_len;
    size_t len;
    u8 dtim;
};

/**
 * struct siwifi_key - Key information
 *
 * @hw_idx: Idx of the key from hardware point of view
 */
struct siwifi_key {
    u8 valid;
    u8 hw_idx;
};

/**
 * Structure containing information about a Mesh Path
 */
struct siwifi_mesh_path {
    struct list_head list;          /* For siwifi_vif.mesh_paths */
    u8 path_idx;                    /* Path Index */
    struct mac_addr tgt_mac_addr;   /* Target MAC Address */
    struct siwifi_sta *p_nhop_sta;    /* Pointer to the Next Hop STA */
};

struct siwifi_mesh_proxy {
    struct list_head list;          /* For siwifi_vif.mesh_proxy */
    struct mac_addr ext_sta_addr;   /* Address of the External STA */
    struct mac_addr proxy_addr;     /* Proxy MAC Address */
    bool local;                     /* Indicate if interface is a proxy for the device */
};

/**
 * struct siwifi_csa - Information for CSA (Channel Switch Announcement)
 *
 * @vif: Pointer to the vif doing the CSA
 * @bcn: Beacon to use after CSA
 * @elem: IPC buffer to send the new beacon to the fw
 * @chandef: defines the channel to use after the switch
 * @count: Current csa counter
 * @status: Status of the CSA at fw level
 * @ch_idx: Index of the new channel context
 * @work: work scheduled at the end of CSA
 */
struct siwifi_csa {
    struct siwifi_vif *vif;
    struct siwifi_bcn bcn;
    struct siwifi_ipc_elem_var elem;
    struct cfg80211_chan_def chandef;
    int count;
    int status;
    int ch_idx;
    struct work_struct work;
};

/// Possible States of the TDLS link.
enum tdls_status_tag {
        /// TDLS link is not active (no TDLS peer connected)
        TDLS_LINK_IDLE,
        /// TDLS Setup Request transmitted
        TDLS_SETUP_REQ_TX,
        /// TDLS Setup Response transmitted
        TDLS_SETUP_RSP_TX,
        /// TDLS link is active (TDLS peer connected)
        TDLS_LINK_ACTIVE,
        /// TDLS Max Number of states.
        TDLS_STATE_MAX
};

/*
 * Structure used to save information relative to the TDLS peer.
 * This is also linked within the siwifi_hw vifs list.
 *
 */
struct siwifi_tdls {
    bool active;                /* Indicate if TDLS link is active */
    bool initiator;             /* Indicate if TDLS peer is the TDLS initiator */
    bool chsw_en;               /* Indicate if channel switch is enabled */
    u8 last_tid;                /* TID of the latest MPDU transmitted over the
                                   TDLS direct link to the TDLS STA */
    u16 last_sn;                /* Sequence number of the latest MPDU transmitted
                                   over the TDLS direct link to the TDLS STA */
    bool ps_on;                 /* Indicate if the power save is enabled on the
                                   TDLS STA */
    bool chsw_allowed;          /* Indicate if TDLS channel switch is allowed */
};


/**
 * enum siwifi_ap_flags - AP flags
 *
 * @SIWIFI_AP_ISOLATE Isolate clients (i.e. Don't brige packets transmitted by
 *                                   one client for another one)
 */
enum siwifi_ap_flags {
    SIWIFI_AP_ISOLATE = BIT(0),
};

struct siwifi_mgmt_status
{
    u32 rx_bcn_cnts;
    u32 rx_non_cnts;
    u32 rx_ft_cnts;
    u32 rx_mgmt_cnts;
    u32 tx_mgmt;
    u32 tx_mgmt_drop;
};

/*
 * Structure used to save information relative to the managed interfaces.
 * This is also linked within the siwifi_hw vifs list.
 *
 */
struct siwifi_key_recovery_info {
    bool valid;
    u8 key_index;
    bool pairwise;
    struct key_params *params;
};

struct siwifi_lite_mem_ctrl {
    u32 tx_memory_usage;
    u32 tx_cnt;
    //Excluding the first skb, it has already been calculated into tx_cnt
    u32 amsdu_tx_cnt;
    //Excluding the first skb, it has already been calculated into tx_memory_usage
    u32 amsdu_tx_memory_usage;

    u32 tx_memory_limit;
    u32 tx_cnt_limit;
};

#ifdef CONFIG_DEBUG_TXQ_STOP
struct siwifi_txq_record
{
    uint8_t pos;
    uint32_t txq;
    uint32_t txqidx;
    uint32_t txq_status;
    uint32_t txqsize;
    uint32_t memctl_balance;
    struct siwifi_lite_mem_ctrl memctrl;
};
#endif

struct siwifi_src_filter {
    unsigned char src_macaddr[ETH_ALEN];
    unsigned int drop_count;
    struct siwifi_src_filter *next;
    unsigned long last_use;
};

//#define MAX_VIF_KEY_NUM 6
struct siwifi_vif {
    struct list_head list;
    struct siwifi_hw *siwifi_hw;
    struct wireless_dev wdev;
    struct net_device *ndev;
    struct net_device_stats net_stats;
    struct siwifi_mgmt_status mgmt_stats;
    struct siwifi_key key[MAX_VIF_KEY_NUM];
    //add for recovery hold ap information
    struct cfg80211_ap_settings *ap_settings;
    struct siwifi_key_recovery_info rec_key[MAX_VIF_KEY_NUM];
    u8 drv_vif_index;           /* Identifier of the VIF in driver */
    u8 vif_index;               /* Identifier of the station in FW */
    u8 ch_index;                /* Channel context identifier */
    bool up;                    /* Indicate if associated netdev is up
                                   (i.e. Interface is created at fw level) */
    bool use_4addr;             /* Should we use 4addresses mode */
    bool is_resending;          /* Indicate if a frame is being resent on this interface */
    bool user_mpm;              /* In case of Mesh Point VIF, indicate if MPM is handled by userspace */
    bool roc_tdls;              /* Indicate if the ROC has been called by a
                                   TDLS station */
    u8 tdls_status;             /* Status of the TDLS link */
    bool tdls_chsw_prohibited;  /* Indicate if TDLS Channel Switch is prohibited */
    s8 txpower;                 /* actual power set for the vif (dBm)*/
    u8 txpower_idx;             /* Idx to be used in policy table */
    union
    {
        struct
        {
            struct siwifi_sta *ap; /* Pointer to the peer STA entry allocated for
                                    the AP */
            struct siwifi_sta *tdls_sta; /* Pointer to the TDLS station */
            bool external_auth;  /* Indicate if external authentication is in progress */
        } sta;
        struct
        {
            u16 flags_recovery;
            u16 flags;                 /* see siwifi_ap_flags */
            struct list_head sta_list; /* List of STA connected to the AP */
            struct list_head local_sta_list; /* local copy List of STA connected to the AP,used for local recovery */
            struct siwifi_bcn bcn;       /* beacon */
            u8 bcmc_index;             /* Index of the BCMC sta to use */
            struct siwifi_csa *csa;
            bool channel_switching;    /* indicate whether vif is in csa state */

            struct list_head mpath_list; /* List of Mesh Paths used on this interface */
            struct list_head proxy_list; /* List of Proxies Information used on this interface */
            bool create_path;            /* Indicate if we are waiting for a MESH_CREATE_PATH_CFM
                                            message */
            int generation;              /* Increased each time the list of Mesh Paths is updated */
        } ap;
        struct
        {
            struct siwifi_vif *master;   /* pointer on master interface */
            struct siwifi_sta *sta_4a;
        } ap_vlan;
    };
#if defined(CONFIG_SF19A28_FULLMASK) && IS_ENABLED(CONFIG_SFAX8_HNAT_DRIVER) && IS_ENABLED(CONFIG_NF_FLOW_TABLE)
	void * phnat_priv;
	struct platform_device *hnat_pdev;
#endif
    /* Lite memory control */
    struct siwifi_lite_mem_ctrl *lm_ctl;
    u32 be_wmm_param;
    u64 total_rx;
    u32 hw_be_cnt;
    u32 hw_vi_cnt;
    u32 hw_vo_cnt;
    u32 hw_all_cnt;
    u64 hw_rx_cnt;
    u32 hw_last_wmm_step;
    u32 hw_wmm_counter;

	uint32_t sta_hash[STA_HASH_SIZE];

    //only used for NL80211_IFTYPE_STATION
    uint32_t src_filter_hash[STA_HASH_SIZE];
    struct timer_list src_filter_timer;
    spinlock_t src_filter_lock;

    u8 repeater_status;
#ifdef CONFIG_SIWIFI_REPEATER
    struct repeater_info * rp_info;
#endif

#ifdef CONFIG_SIWIFI_IGMP
    uint32_t enable_multicast_to_unicast;
#endif
    int wds_success;
};

enum repeater_status
{
    REPEATER_FAIL = 0,
    REPEATER_VIF_UP,
    REPEATER_AUTH,
    REPEATER_ASSOC,
    REPEATER_EAPOL,
};

#define SIWIFI_VIF_TYPE(siwifi_vif) (siwifi_vif->wdev.iftype)

/**
 * Structure used to store information relative to PS mode.
 *
 * @active: True when the sta is in PS mode.
 *          If false, other values should be ignored
 * @pkt_ready: Number of packets buffered for the sta in drv's txq
 *             (1 counter for Legacy PS and 1 for U-APSD)
 * @sp_cnt: Number of packets that remain to be pushed in the service period.
 *          0 means that no service period is in progress
 *          (1 counter for Legacy PS and 1 for U-APSD)
 */
struct siwifi_sta_ps {
    bool active;
    u16 pkt_ready[2];
    u16 sp_cnt[2];
};

/**
 * struct siwifi_rx_rate_stats - Store statistics for RX rates
 *
 * @table: Table indicating how many frame has been receive which each
 * rate index. Rate index is the same as the one used by RC algo for TX
 * @size: Size of the table array
 * @cpt: number of frames received
 */
struct siwifi_rx_rate_stats {
    uint64_t *table;
    int size;
    uint64_t cpt;
};

/**
 * struct siwifi_sta_stats - Structure Used to store statistics specific to a STA
 *
 * @last_rx: Hardware vector of the last received frame
 * @rx_rate: Statistics of the received rates
 */
struct siwifi_sta_stats {
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    struct hw_vect last_rx;
    struct siwifi_rx_rate_stats rx_rate;
#endif
    u32 connected_time;     /*  seconds elapsed since associated */
    u32 idle;   /*  time since data pkt rx'd from sta */
    u32 tx_packets;
    u32 rx_packets;
    u64 tx_bytes;
    u64 rx_bytes;
    u32 tx_failed;
    u32 rx_dropped;
    u32 tx_dropped;
    u8 short_gi;
    s8 data_rssi;
    u8 data_rx_bw;
    u8 format_mod;
    u8 data_rx_mcs;
    u8 data_rx_nss;
    u16 data_rx_rate;
    u32 leg_rate;
    s8 data_rssi_avg;
    s8 data_rssi_max;
    s8 data_rssi_min;
    // store rate info of tx
    u16 last_tx_info;
    u16 last_tx_rate_config;
    // for signal average calculating
    u16 count;
    s16 data_rssi_sum;
    s8 data_rssi_old[256];
    u8 preamble;
    u64 last_rx_bytes;
    u64 last_tx_bytes;
    u32 last_rx_tp;
    u32 last_tx_tp;
    u32 last_calc_tp;
    u32 last_th_tp;
#ifdef CONFIG_VDR_HW
    u64 tx_unicast_bytes;
    u64 rx_unicast_bytes;
    u64 tx_mcast_bytes;
    u64 rx_mcast_bytes;
    u64 last_rx_bytes;
    u64 last_tx_bytes;
    u32 ac_pkts[AC_MAX];
    u32 last_rx_tp;
    u32 last_tx_tp;
    u32 last_calc_tp;   /* jiffies */
    u32 last_ps_ts;     /* jiffies */
    u16 ps_times;       /* ms */
    u16 data_rx_rate;
    u8 no_ss;
    u8 short_gi;
    u8 data_rssi;
    u8 data_rx_bw;
#endif
};

struct siwifi_sta_rec_info {
    // list to add to hw global list
    struct list_head list;
    // mac addr for this station
    u8 mac_addr[ETH_ALEN];
    // station params
    struct station_parameters *params;
    // recovery key
    struct siwifi_key_recovery_info rec_key;
    //if control port is open or not
    bool cp_open_set;
    int control_port_open;
    // plink station for mesh
    bool pstate_set;
    int plink_state;
    // if this station is support qos
    int qos;
    // vlan
    void *vlan;
    // store the sta idx
    u8 sta_idx;
};

/*
 * Structure used to save information relative to the managed stations.
 */
struct siwifi_sta {
    struct list_head list;
    u16 aid;                /* association ID */
    u8 sta_idx;             /* Identifier of the station */
    u8 vif_idx;             /* Identifier of the VIF (fw id) the station
                               belongs to */
    u8 vlan_idx;            /* Identifier of the VLAN VIF (fw id) the station
                               belongs to (= vif_idx if no vlan in used) */
    enum nl80211_band band; /* Band */
    enum nl80211_chan_width width; /* Channel width */
    u16 center_freq;        /* Center frequency */
    u32 center_freq1;       /* Center frequency 1 */
    u32 center_freq2;       /* Center frequency 2 */
    u8 ch_idx;              /* Identifier of the channel
                               context the station belongs to */
    bool qos;               /* Flag indicating if the station
                               supports QoS */
    u8 acm;                 /* Bitfield indicating which queues
                               have AC mandatory */
    u16 uapsd_tids;         /* Bitfield indicating which tids are subject to
                               UAPSD */
    u8 mac_addr[ETH_ALEN];  /* MAC address of the station */
    struct siwifi_key key;
    bool valid;             /* Flag indicating if the entry is valid */
    struct siwifi_sta_ps ps;  /* Information when STA is in PS (AP only) */
#ifdef CONFIG_SIWIFI_BFMER
    struct siwifi_bfmer_report *bfm_report;     /* Beamforming report to be used for
                                                 VHT TX Beamforming */
#ifdef CONFIG_SIWIFI_MUMIMO_TX
    struct siwifi_sta_group_info group_info; /* MU grouping information for the STA */
#endif /* CONFIG_SIWIFI_MUMIMO_TX */
#endif /* CONFIG_SIWIFI_BFMER */

    bool ht;               /* Flag indicating if the station
                               supports HT */
    bool vht;               /* Flag indicating if the station
                               supports VHT */
    u32 ac_param[AC_MAX];  /* EDCA parameters */
    struct siwifi_tdls tdls; /* TDLS station information */
    struct siwifi_sta_stats stats;
    // recovery information
    struct siwifi_sta_rec_info rec_info;
	/* next entry in hash table list */
	struct siwifi_sta *hnext;
/*
#ifdef CONFIG_VDR_HW
    uint64_t tx_unicast_bytes;
    uint64_t rx_unicast_bytes;
    uint64_t tx_mcast_bytes;
    uint64_t rx_mcast_bytes;
    uint64_t last_rx_bytes;
    uint64_t last_tx_bytes;
    uint32_t ac_pkts[AC_MAX];
    uint32_t tx_pkts;
    uint32_t rx_pkts;
    uint32_t last_rx_tp;
    uint32_t last_tx_tp;
    uint32_t last_calc_tp;
    uint32_t connected_ts;
    uint32_t last_ps_ts;
    uint16_t sleep_times;
    uint8_t no_ss;
    uint8_t short_gi;
    uint8_t qos_enabled;
    uint8_t data_rssi;
    uint8_t data_rx_bw;
    uint8_t data_rx_rate;
#endif */
    // RM#10611 count of times to update idle time
    u16 update_time_count;
    int user_tid;

#ifdef CONFIG_SIWIFI_EASYMESH
    bool remove_sta;
#endif /* CONFIG_SIWIFI_EASYMESH */
};

#define SIWIFI_STATIC_INFO_MAX 36
#define SIWIFI_STATIC_TIME_MAX 60
#define SIWIFI_STATIC_INTERVAL (1 * HZ) // 1s
#define SIWIFI_STATIC_BREAK_REASON_MAX 3
struct siwifi_static_txinfo {
    uint16_t siwifi_static_txxmit;
    uint16_t siwifi_static_txpush;
    uint16_t siwifi_static_txcfm;
    uint16_t siwifi_static_cfm_ack;
    uint16_t siwifi_static_cfm_ret;
    uint16_t siwifi_static_a_tt;
    uint16_t siwifi_static_s_tt;
    uint16_t siwifi_static_rx;

    uint16_t siwifi_static_drop_skb;
    uint16_t siwifi_static_intxq;
    uint16_t siwifi_static_ps_drop;
    uint16_t siwifi_static_ps_change[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_ready[NX_NB_TXQ_PER_STA];
    uint8_t  siwifi_static_ps_state;

    uint16_t siwifi_static_txq_start[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_txq_stop[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_txq_process[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_pro_in_hwq[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_pro_reorder[NX_NB_TXQ_PER_STA];
    uint16_t siwifi_static_not_ready[NX_NB_TXQ_PER_STA];

    uint16_t siwifi_static_hwq_process;
    uint16_t siwifi_static_hwq_push[NX_TXQ_CNT];
    uint16_t siwifi_static_break_reason[SIWIFI_STATIC_BREAK_REASON_MAX];

    uint8_t  siwifi_static_use;
};
void siwifi_static_timer_clear(struct siwifi_hw *siwifi_hw);
void siwifi_static_timer_start(struct siwifi_hw *siwifi_hw);
void siwifi_static_timer_print(struct siwifi_hw *siwifi_hw, int sta_idx);

static inline const u8 *siwifi_sta_addr(struct siwifi_sta *siwifi_sta) {
    return siwifi_sta->mac_addr;
}

#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
struct siwifi_amsdu_stats {
    int done;
    int failed;
};
#endif

#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
struct siwifi_stats {
    int cfm_balance[NX_TXQ_CNT];
    unsigned long last_rx, last_tx; /* jiffies */
    int ampdus_tx[IEEE80211_MAX_AMPDU_BUF];
    int ampdus_rx[IEEE80211_MAX_AMPDU_BUF];
    int ampdus_rx_map[4];
    u32 ampdus_rx_miss;
#ifdef CONFIG_SIWIFI_SPLIT_TX_BUF
    struct siwifi_amsdu_stats amsdus[NX_TX_PAYLOAD_MAX];
#endif
    u32 tx_failed;
    u32 tx_copy_expand;
    u32 tx_drop_full;
    u32 tx_drop_sta_null;
    u32 tx_drop_multicast;
    u32 tx_drop_txq_inactive;
    u32 tx_drop_hdr_alloc_fail;
    u32 tx_drop_prep_tx;
    u32 tx_retry;
    u32 single_retry;
    u32 tx_flush;
    u32 tx_discard;
    u64 tx_pkt;
    u32 amsdus_rx[64];
	unsigned int delete_rx;
    unsigned int rx_monitor_interface_close;
	unsigned int monitor_rx;
	unsigned int len_update_rx;
	unsigned int mgmt_rx;
	unsigned int spurious_rx;
    u32 ldpc_rx;
    u32 bcc_rx;
    u32 stbc_rx;
    u32 ipc_pending_cnt;
    u64 total_rx;
    u64 rxind_cnts;
	/* TX queues stats */
    int queues_stops;
    int queues_starts;
};
#endif

struct siwifi_sec_phy_chan {
    u16 prim20_freq;
    u16 center_freq1;
    u16 center_freq2;
    enum nl80211_band band;
    u8 type;
};

/* Structure that will contains all RoC information received from cfg80211 */
struct siwifi_roc_elem {
    struct wireless_dev *wdev;
    struct ieee80211_channel *chan;
    unsigned int duration;
    /* Used to avoid call of CFG80211 callback upon expiration of RoC */
    bool mgmt_roc;
    /* Indicate if we have switch on the RoC channel */
    bool on_chan;
};

/* Structure containing channel survey information received from MAC */
struct siwifi_survey_info {
    // Filled
    u32 filled;
    // Amount of time in ms the radio spent on the channel
    u32 chan_time_ms;
    // Amount of time the primary channel was sensed busy
    u32 chan_time_busy_ms;
    // Noise in dbm
    s8 noise_dbm;
};

#define SIWIFI_CH_NOT_SET 0xFF
#define SIWIFI_INVALID_VIF 0xFF
#define SIWIFI_INVALID_STA 0xFF

/* Structure containing channel context information */
struct siwifi_chanctx {
    struct cfg80211_chan_def chan_def; /* channel description */
    u8 count;                          /* number of vif using this ctxt */
};

#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
enum ate_op_flag {
    CREATE_CHANTX_OP = 0,
    UPDATE_CHANTX_OP,
    DELETE_CHANTX_OP,
    SET_HW_ACTIVE,
    SET_HW_IDLE,
};

enum format_mode {
    NON_HT_MODE = 0,
    NON_HT_DUP_MODE,
    HT_MM_MODE,
    HT_GF_MODE,
    VHT_MODE,
};

enum payload_format {
    user_payload_format = 1,
    inc_payload_format,
};

struct siwifi_ate_rate_params {
    enum format_mode mode;
    unsigned int rate_idx;
    bool use_short_preamble; //only for 2.4G CCK mode rate_idx 1~3
    bool use_short_gi;       //for HT VHT mode but not HT_GF_MODE
};

struct siwifi_ate_package_params {
    //TODO
    unsigned int frame_num;
    unsigned int frame_len;
    unsigned int tx_power;
    uint8_t payload[4096];
    unsigned int payload_mode;
    enum payload_format mode;
    unsigned int frame_bw;
    bool cont_flag;
    unsigned int macbypass_interval;
};

struct siwifi_ate_machdr_params {
    unsigned char siwifi_da[6];
    unsigned char siwifi_sa[6];
    unsigned char siwifi_bssid[6];
    unsigned short fc;
    unsigned short dur;
    unsigned short seqc;
};

struct siwifi_ate_qos_machdr_params {
    unsigned char siwifi_da[6];
    unsigned char siwifi_sa[6];
    unsigned char siwifi_bssid[6];
    unsigned short fc;
    unsigned short dur;
    unsigned short seqc;
    unsigned short qosc;
};

struct siwifi_ate_config_params {
    //ate channel parameters from user
    struct cfg80211_chan_def chandef;

    //ate rate parameters from user
    struct siwifi_ate_rate_params rate;

    //ate packet parameters from user
    struct siwifi_ate_package_params pkg;

    //ate mac heaedr parameters from user
    struct siwifi_ate_machdr_params machdr;

    //ate qos mac heaedr parameters from user
    struct siwifi_ate_qos_machdr_params qos_machdr;
};

struct siwifi_ate_reg {
    //read register
    unsigned int read_reg_val;
    unsigned int origi_val1;
    //write
    int status;
};

/* Structure containing all informations which are for ate */
struct siwifi_ate_env {

    //we enter into ate mode
    bool ate_start;
    //vif ctx exist
    bool vif_ctx_flag;
    //for tx test
    bool tx_frame_start;
	bool tx_macbypass_start;
    bool agg_tx;
    bool agg_action;
    int tx_total_count;
    int tx_left_count;
    int tx_count;
    int tx_retry_count;
    int tx_successful_count;
    int tx_agg_done;
    int tx_agg_retry;
    int hw_queue;
    int tx_queue;
    struct siwifi_vif *siwifi_vif;
    struct siwifi_chanctx *siwifi_chanctx;
    struct ieee80211_tx_control *control;
    struct ieee80211_ampdu_params *params;
    struct tasklet_struct tx_task;

    //for rx test
    bool rx_frame_start;
	bool rx_macbypass_start;
    unsigned int rec_rx_count;
    unsigned int all_err;
    unsigned int phy_err;
    unsigned int fcs_err;
	unsigned int fcs_ok;
    unsigned int per;
	unsigned int fcs_ok_for_macaddr;
	unsigned int fcs_group;
    int rssi;
    //ate config from user
    struct siwifi_ate_config_params conf;
    struct siwifi_ate_reg reg;
    //ate operation function
    struct cfg80211_ops ate_ops;


};
#endif
#if 0
#define CB_LOCK_INIT() spin_lock_init(&siwifi_hw->cb_lock)
#define CB_LOCK(bh)    \
    do {    \
        bh ? spin_lock_bh(&siwifi_hw->cb_lock) : spin_lock(&siwifi_hw->cb_lock); \
    }while(0)

#define CB_UNLOCK(bh)  \
    do {    \
        bh ? spin_unlock_bh(&siwifi_hw->cb_lock) : spin_unlock(&siwifi_hw->cb_lock);  \
    }while(0)
#endif

/*add for acs based on drv*/
#ifdef CONFIG_SIWIFI_ACS
struct vendor_channel_data {
    // Channel number (IEEE 802.11)
    u8 chan;
    enum nl80211_band band;
    // Frequency in MHz
    u32 freq;
    // Channel flags (HOSTAPD_CHAN_*)
    u8 flag;
    // Allowed channel width bitmask
    // See enum hostapd_chan_width_attr.
    u32 allowed_bw;
    // Linked list of surveys (struct survey)
    struct list_head survey_list;
    // Minimum observed noise floor, in dBm, based on all
    // surveyed channel data
    s8 min_nf;
    // Computed interference factor on this channel
    u32 interference_factor;
};
struct hostapd_acs_chan_param {
	uint32_t vif_index;
	uint16_t ht_enabled:1,
			 ht40_enabled:1,
			 vht_enabled:1;
	uint8_t primary_chan;
	uint8_t sec_chan;
	uint8_t hw_mode; //0:11g, 1:11b, 2:11a
	uint8_t ch_width;
	uint8_t seg0_chan;
	uint8_t seg1_chan;
};
struct hostapd_acs_chan_result {
	uint32_t last_time;
	uint8_t primary_chan;
	uint8_t sec_chan;
	uint8_t hw_mode;
	uint8_t ch_width;
	uint8_t seg0_chan;
	uint8_t seg1_chan;
};
#endif

#ifdef CONFIG_SF19A28_WIFI_LED
#define SIWIFI_LED_MAX_NAME_LEN 31
#define LED_GPIO_LABEL "gpio-leds"

struct siwifi_led {
	char name[SIWIFI_LED_MAX_NAME_LEN + 1];
	struct led_classdev led_dev;
	struct siwifi_hw *hw;
};
#endif


#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
#define TEMP_CTL_DEFAULT_TRIGGER_TEMP 40
#define TEMP_CTL_DEFAULT_JUMP_LIMIT 5
struct siwifi_temp_ctl {
    /* temp_ctl_parameters[0]:thermal control enable
       temp_ctl_parameters[1-4]:thermal control level 1-4 temperture
       temp_ctl_parameters[5-8]:cooling limit level 1-4 */
    int temp_ctl_parameters[9];
    int temp_ctl_level;
    int last_change_temp;
};
#endif

#ifdef CONFIG_WIFI_RX_THREAD
struct rx_thread_dbginfo {
    uint32_t elt_cur_cnt;
    uint32_t elt_max_cnt;

    uint32_t skb_cur_cnt;
    uint32_t skb_max_cnt;

    uint32_t skb_max_per_elt;
    uint32_t elt_loop_max_cnt;
};

#endif

/**
 * struct siwifi_atf - the struct about atf (airtime fairness)
 * @enable: enabled atf
 * @last_update_time: the time update txq->atf.record_rateinfo in function siwifi_update_atf
 * @txq_nosent_cnt: the number of txq that haven't sent skb since the update
 * @max_rateinfo: the maximum rate in all txq->atf.record_rateinfo
 * @update_cnt: the number of updates
 */
struct siwifi_atf {
    uint8_t enable;
    uint32_t last_update_time;
    uint8_t txq_nosent_cnt;
    uint32_t max_rateinfo;
    uint32_t update_cnt;
};

#ifdef CONFIG_SIWIFI_IQENGINE
struct siwifi_iqe {
    bool init;
    uint32_t error;
    int mode;
    void *iq_buffer;
    uint32_t iq_buffer_phy;
    uint32_t iq_buffer_len;
    uint32_t iq_irq;
};
#endif

#ifdef CONFIG_SIWIFI_EASYMESH
#define WLAN_WNM_BTM_QUERY 6       // BSS Transition Management Query
#define WLAN_WNM_BTM_RESPONSE 8    // BSS Transition Management Response
#define WLAN_PA_GAS_INITIAL_REQ 10 // Generic Advertisement Service (GAS) Initial Request
#define WLAN_WNM_NOTIFY_REQ 26     // WNM (Wireless Network Management) Notification Request
#define RADIO_MEASUREMENT_REPORT 1 // Radio Mesurement Report

#define DEL_STA_CB_POSITION 40     // Callback position for STA deletion
#define DEL_STA_CB_CODE 0x0c       // Callback code for STA deletion

/**
 * sf_tunneled_frame_type - Enumeration type representing different types of tunneled management frames.
 */
typedef enum {
    SF_TUNNELED_MSG_ASSOC_REQ = 0x00,                   // Association Request frame
    SF_TUNNELED_MSG_REASSOC_REQ = 0x01,                 // Reassociation Request frame
    SF_TUNNELED_MSG_BTM_QUERY = 0x02,                   // BSS Transition Management (BTM) Query frame
    SF_TUNNELED_MSG_WNM_REQ = 0x03,                     // Wireless Network Management (WNM) Notification Request frame
    SF_TUNNELED_MSG_ANQP_NEIGHBOR_REQ = 0x04,           // Access Network Query Protocol (ANQP) Request frame
    SF_TUNNELED_MSG_RADIO_MEASUREMENT_REPORT = 0x05,    // Radio Measurement Report
    SF_TUNNELED_MSG_BTM_RESPONSE = 0x06,                // BSS Transition Management (BTM) Response frame
} sf_tunneled_frame_type;

/**
 * struct sf_sta_timer_info - A structure to store timer data.
 *
 * @timer: Points to the timer structure used for scheduling events.
 * @timeout: Duration in seconds after which the timer expires.
 * @siwifi_hw: SIWIFI driver main data.
 *
 * This structure is used to manage timer-related data, which includes the timer
 * itself and its timeout duration.
 */
typedef struct {
    struct timer_list timer;     // Linux kernel timer structure
    uint8_t timeout;             // Timeout duration in seconds
    struct siwifi_hw *siwifi_hw; // SIWIFI driver main data
} sf_sta_timer_info;

/**
 * struct sf_block_sta_info - A structure to store blocked station data.
 *
 * @mac: The MAC address of the blocked station.
 * @bssid: The BSSID of the AP that blocked the station.
 * @list: Linked list to manage multiple blocked stations.
 * @timer: Timer data to handle the block duration.
 *
 * This structure is used to manage information about blocked stations, including
 * their MAC address, BSSID, and the duration for which they are blocked. It also
 * includes a linked list structure for managing multiple blocked stations.
 */
typedef struct {
    uint8_t mac[ETH_ALEN];   // MAC address of the blocked station
    uint8_t bssid[ETH_ALEN]; // BSSID of the AP that blocked the station
    struct list_head list;   // Linked list structure for multiple blocked stations
    sf_sta_timer_info timer; // Timer structure to handle the block duration
} sf_block_sta_info;

/**
 * struct sf_easymesh_scan_result - A structure to store scan result information.
 *
 * @channel: The channel number on which the scanned network is operating.
 * @oper_class: The operational class of the channel used by the scanned network.
 *
 * @bssid: The BSSID (Basic Service Set Identifier) of the scanned network.
 *         It is a unique identifier for the access point.
 * @ssid_len: The length of the SSID (Service Set Identifier) of the scanned network.
 * @ssid: The SSID of the scanned network. It identifies the network name.
 * @rssi: The Received Signal Strength Indicator (RSSI) of the scanned network.
 *        It measures the signal strength of the network.
 * @chan_bw_len: The length of the channel bandwidth string.
 * @chan_bw: The channel bandwidth of the scanned network, represented as a string.
 * @has_bss_load: A flag indicating whether BSS load information is present (1 if present, 0 otherwise).
 * @chan_util: Percentage of time the channel is busy, scaled from 0 (0%) to 255 (100%).
 * @sta_count: The number of stations currently associated with the BSS (Basic Service Set).
 *
 * @list: A list entry used to link this scan result to a list of scan results.
 *
 * This structure is used to store all relevant information obtained from a network scan, including the BSSID, SSID,
 * signal strength, channel information, bandwidth, BSS load, channel utilization, and the number of stations associated
 * with the scanned BSS.
 */
typedef struct {
    uint8_t channel;         // Channel number of the scanned network
    uint8_t oper_class;      // Operational class of the channel

    uint8_t bssid[ETH_ALEN]; // BSSID of the scanned network
    uint8_t ssid_len;        // Length of the SSID
    char ssid[32];           // SSID of the scanned network
    int8_t rssi;             // Received Signal Strength Indicator (RSSI)
    uint8_t chan_bw_len;     // Length of the channel bandwidth string
    char chan_bw[6];         // Channel bandwidth of the scanned network
    bool has_bss_load;       // Flag indicating if BSS load information is present
    uint8_t chan_util;       // Percentage of time the channel is busy, scaled from 0 (0%) to 255 (100%)
    uint16_t sta_count;      // Number of stations associated with the BSS

    struct list_head list;   // List entry for linking scan results
} sf_easymesh_scan_result;

/**
 * sf_notify_wifi_event_type - Enumeration representing different types of WiFi events.
 *
 * This enumeration defines the possible WiFi events that the system can notify.
 */
typedef enum {
    SF_NOTIFY_WIFI_STA_CHANGE_EVENT = 0,   // Event indicating a change in the Station's (STA) connection state.
    SF_NOTIFY_WIFI_STA_INFO_EVENT,         // Event providing information about the Station (STA).
    SF_NOTIFY_WIFI_MGMT_RX_EVENT,          // Event triggered by the reception of a management frame.
    SF_NOTIFY_WIFI_SCAN_DONE_EVENT,        // Event indicating the completion of a channel scan.
    SF_NOTIFY_WIFI_BLOCK_CONNECT_EVENT,    // Event indicating that connections are being blocked.
    SF_NOTIFY_WIFI_BBSS_BLOCK_CHECK_EVENT, // Event to check if a STA is allowed to connect to the current BBSS.
} sf_notify_wifi_event_type;

/**
 * sf_wifi_event_data - Structure to hold WiFi notification event data.
 *
 * This structure encapsulates the data related to various WiFi events that are processed by the system.
 * The specific event data is stored in a union, with different members representing different types of events.
 */
typedef struct {
    sf_notify_wifi_event_type type; // Event type indicating which WiFi event has occurred.

    union {
        struct {
            uint8_t sta_mac[ETH_ALEN];  // Client's current MAC address.
            uint8_t prev_mac[ETH_ALEN]; // Client's previous MAC address.
            bool updown;                // Connection state: true if connected, false if disconnected.
        } sta_change_event;             // Data structure for station (STA) connection state change event.

        struct {
            uint8_t sta_mac[ETH_ALEN];  // Client's current MAC address.
            uint8_t prev_mac[ETH_ALEN]; // Client's previous MAC address.
            int8_t rssi;                // Received Signal Strength Indication for the client.
            /* TODO: Add more info as needed. */
        } sta_info_event; // Data structure for station (STA) information event.

        struct {
            uint8_t *frame_data;               // Pointer to the raw management frame data.
            uint16_t frame_length;             // Length of the management frame data.
            sf_tunneled_frame_type frame_type; // Type of the management frame (e.g., deauth, probe request).
        } mgmt_rx_event;                       // Data structure for received management frame event.

        struct {
            bool band;              // Band of the device: 0 for 2.4 GHz, 1 for 5 GHz.
            struct list_head *list; // Pointer to the linked list head storing scan results.
            spinlock_t *lock;       // Pointer to the spinlock protecting the linked list operations.
        } scan_done_event;          // Data structure for scan completion event.

        struct {
            // Placeholder structure, currently no variables defined.
        } block_connect_event; // Data structure for the event where connections are being blocked.

        struct {
            uint8_t sta_mac[ETH_ALEN];    // Client's current MAC address.
            uint8_t bssid[ETH_ALEN];      // BSSID of the BSS.
            bool band;                          // Band of the device: 0 for 2.4 GHz, 1 for 5 GHz.
            bool *block;                        // Pointer to a boolean indicating whether the STA should be blocked.
                                                // True if the STA is not allowed to connect to the current BBSS,
                                                // False otherwise.
        } bbss_block_check_event;   // Data structure for checking if a STA is allowed to connect to the current BBSS.
    } data;                         // Union holding specific event data based on the event type.
} sf_wifi_event_data;

/**
 * sf_notify_easymesh_event_type - Enumeration representing different types of EasyMesh events.
 *
 * This enumeration defines the possible EasyMesh events that the system can notify.
 */
typedef enum {
    SF_NOTIFY_EASYMESH_STA_DEL_EVENT = 0,     // Event indicating the deletion of a Station (STA).
    SF_NOTIFY_EASYMESH_STA_BLOCK_EVENT,       // Event indicating that a Station (STA) has been blocked.
    SF_NOTIFY_EASYMESH_TRIGGER_SCAN_EVENT,    // Event indicating that a scan operation has been triggered.
    SF_NOTIFY_EASYMESH_UNBLOCK_CONNECT_EVENT, // Event indicating the removal of a restriction, allowing all stations
                                              // (STA) to connect to the network.
    SF_NOTIFY_EASYMESH_AP_VLAN_DEL_EVENT      // Event indicating the deletion of AP VLAN interfaces.
} sf_notify_easymesh_event_type;

/**
 * sf_easymesh_event_data - Structure to hold EasyMesh notification event data.
 *
 * This structure encapsulates the data related to various EasyMesh events that need to be processed.
 * The specific event data is stored in a union, with different members for different types of events.
 */
typedef struct {
    sf_notify_easymesh_event_type type; // Event type indicating which event has occurred.

    union {
        struct {
            struct net_device *dev; // Pointer to the network device where the event occurred.
            uint8_t *mac;           // MAC address of the station (STA) that is to be deleted.
        } sta_del_event;            // Data structure for station (STA) deletion event.

        struct {
            struct net_device *dev; // Pointer to the network device where the event occurred.
            uint8_t **sta_mac;      // Array of pointers to MAC addresses of stations to be blocked.
            uint16_t count;         // Number of stations to be blocked.
            uint16_t timeout;       // Duration for which the stations will be blocked (in seconds).
        } sta_block_event;          // Data structure for station (STA) block event.

        struct {
            struct net_device *dev;  // Pointer to the network device where the event occurred.
            uint8_t *scan_chan_list; // Pointer to the list of channels to be scanned.
            uint8_t scan_chan_count; // Number of channels in the scan list.
        } trigger_scan_event;        // Data structure for the scan trigger event.

        struct {
            struct net_device *dev; // Pointer to the network device where the event occurred.
        } unblock_connect_event; // Data structure for the event that removes restrictions and allows all stations (STA)
                                 // to connect.

        struct {
            struct net_device *dev;    // Pointer to the network device where the event occurred.
            struct net_device *br_dev; // Pointer to the bridge device associated with the event.
        } ap_vlan_del_event;           // Data structure for AP VLAN inetefaces deletion event.
    } data;                            // Union holding specific event data based on the event type.
} sf_easymesh_event_data;

typedef void (*siwifi_event_callback)(sf_wifi_event_data *event_data);
void siwifi_report_event_to_easymesh(sf_wifi_event_data *event_data);

#endif /* CONFIG_SIWIFI_EASYMESH */

#ifdef CONFIG_SIWIFI_SORT_SCAN
#define MAX_SCAN_BSS_CNT 64
#endif

struct siwifi_hw {
    struct siwifi_mod_params *mod_params;
    bool use_phy_bw_tweaks;
    struct device *dev;
    struct wiphy *wiphy;
    struct list_head vifs;
    struct siwifi_vif *vif_table[NX_VIRT_DEV_MAX + NX_REMOTE_STA_MAX]; /* indexed with fw id */
    struct siwifi_sta sta_table[NX_REMOTE_STA_MAX + NX_VIRT_DEV_MAX];
#ifdef  LOCAL_RECOVERY
    /* hold local sta table,use for local recovery */
    struct siwifi_sta_rec_info local_sta_table[NX_REMOTE_STA_MAX];
    struct siwifi_sta *recovering_sta;
    u8 recovering_sta_idx;
#endif
    bool dying_gasp_valid;
    struct siwifi_ipc_elem_var dying_gasp_info_elem;

    struct siwifi_survey_info survey[SCAN_CHANNEL_MAX];
    struct cfg80211_scan_request *scan_request;
    int scan_timeout;
    struct siwifi_chanctx chanctx_table[NX_CHAN_CTXT_CNT];
    u8 cur_chanctx;
    struct siwifi_trace_ctx trace_ctx;

#ifdef CONFIG_SIWIFI_ACS_INTERNAL
    bool acs_internal;
    struct work_struct csa_work;
    struct cfg80211_chan_def csa_chandef;
    uint32_t last_time;
#endif
#if defined (CONFIG_SIWIFI_ACS) || defined (CONFIG_SIWIFI_ACS_INTERNAL)
    bool acs_scan;
    struct cfg80211_scan_request *acs_request;
#endif

#ifdef CONFIG_SIWIFI_ACS
	struct vendor_channel_data chan_data[SCAN_CHANNEL_MAX];
	struct hostapd_acs_chan_param acs_params[NX_VIRT_DEV_MAX];
	struct hostapd_acs_chan_result acs_result;
#endif

    /* Virtual Monitor Interface */
    u8 monitor_vif;

    /* RoC Management */
    struct siwifi_roc_elem *roc_elem;             /* Information provided by cfg80211 in its remain on channel request */
    u32 roc_cookie_cnt;                         /* Counter used to identify RoC request sent by cfg80211 */

    struct siwifi_cmd_mgr cmd_mgr;

#ifdef CONFIG_SF19A28_WIFI_LED
	struct siwifi_led rx_led;
	struct siwifi_led tx_led;
	unsigned int led_pin,led_on;

	//read from struct ieee80211_hw
	struct led_trigger local_tx_led, local_rx_led;
	atomic_t tx_led_active, rx_led_active;
#endif

    unsigned long drv_flags;
    struct siwifi_plat *plat;


    spinlock_t tx_lock;
    spinlock_t cb_lock;
    struct mutex mutex;                         /* per-device perimeter lock */

    struct tasklet_struct task;
    struct mm_version_cfm version_cfm;          /* Lower layers versions - obtained via MM_VERSION_REQ */
#ifdef CONFIG_HEART_BEAT
    struct timer_list heart_timer;
    struct work_struct heart_work;
    unsigned int recovery_hb_num;
#endif

    struct timer_list txq_stat_timer;
    struct work_struct txq_stat_work;

    u32 tcp_pacing_shift;
    u32 task_max_process_time;
    u64 total_rx;
    /* WMM EDCA traffic detect counters */
    u32 hw_be_cnt;
    u32 hw_all_cnt;
    unsigned long hw_next_jiffies;
    u32 hw_vi_cnt;
    u32 hw_vo_cnt;
    u64 hw_rx_cnt;
    u32 hw_last_wmm_step;
    u32 hw_wmm_counter;
    struct siwifi_ipc_elem_var debug_frame_buf;

    /* IPC */
    struct ipc_host_env_tag *ipc_env;
    struct siwifi_ipc_elem_pool e2amsgs_pool;
    struct siwifi_ipc_elem_pool dbgmsgs_pool;
    struct siwifi_ipc_elem_pool e2aradars_pool;
    struct siwifi_ipc_elem_var pattern_elem;
    struct siwifi_ipc_dbgdump_elem dbgdump_elem;
    struct siwifi_ipc_elem_pool e2arxdesc_pool;
    struct siwifi_ipc_skb_elem *e2aunsuprxvec_elems;
    struct siwifi_ipc_rxbuf_elems rxbuf_elems;
    struct siwifi_ipc_elem_var scan_ie;
    struct siwifi_ipc_elem_var dbg_mgmt_info_elem;
    struct siwifi_ipc_elem_var dbg_ctrl_info_elem;
#ifdef CONFIG_SIWIFI_TRX_STAT
    struct siwifi_ipc_elem_var dbg_trx_stat_elem;
#endif
#ifdef NEW_SCHEDULE
    struct siwifi_ipc_elem_pool a2etxdesc_pool[CONFIG_USER_MAX][NX_TXQ_CNT];
#endif
#ifdef CONFIG_SIWIFI_CACHE_ALLOC
    struct kmem_cache      *sw_txhdr_cache;
#endif
#if defined (CONFIG_SIWIFI_DEBUGFS) || defined (CONFIG_SIWIFI_PROCFS)
    struct siwifi_debugfs     debugfs;
    struct siwifi_stats       stats;
    uint32_t enable_rc;
    uint32_t tx_ctrl;
    uint32_t rx_ctrl;
    uint32_t debug_get_survey;
    uint32_t disable_wmm_edca;
    uint32_t wmm_edca_interval;
    uint32_t wmm_edca_counter_drop;
    uint32_t wmm_edca_pkt_threshold;
    uint32_t wmm_debug_enable;
    uint32_t amsdu_threshold;
    uint32_t debug_frame_statinfo;
#endif
#ifdef CONFIG_SIWIFI_PROCFS
	struct proc_dir_entry *procfsdir;
#endif

    struct siwifi_txq txq[NX_NB_TXQ];
    struct siwifi_hwq hwq[NX_TXQ_CNT];

    struct siwifi_sec_phy_chan sec_phy_chan;
    u8 phy_cnt;
    u8 chan_ctxt_req;
    u8 avail_idx_map;
    u8 vif_started;
    bool adding_sta;
    struct phy_cfg_tag phy_config;

    struct siwifi_radar radar;

    /* extended capabilities supported */
    u8 ext_capa[8];

#ifdef CONFIG_SIWIFI_MUMIMO_TX
    struct siwifi_mu_info mu;
#endif
    /* schedule work for local recovery */
#ifdef LOCAL_RECOVERY
    struct work_struct restart_work;
#endif
	/* schedule work when lmac send rfcmd failed */
	struct work_struct restart_work_rfcmd;
#ifdef CONFIG_SF16A18_WIFI_ATE_TOOLS
    struct siwifi_ate_env ate_env;
#endif
    s16 hwq_credits_dec[NX_TXQ_CNT];
#ifdef CONFIG_SIWIFI_TX_POWER_CALI
    struct siwifi_factory_info factory_info;
#endif
#ifdef CONFIG_SIWIFI_COOLING_TEMP
	int change_power;
    int change_power_trend;
    uint32_t temp_disable;
    u8 max_txpower_idx;
    enum sfax8_rf_cooling_state cool_state;
#endif
#ifdef CONFIG_SIWIFI_IGMP
    struct multicast_group *mul_group;
    uint32_t enable_multicast_to_unicast;
#endif
#ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
struct siwifi_temp_ctl temp_ctl;
#endif
    bool scaning;
	struct completion scan_complete;
	bool wait_scan_complete;
    int tx_queue_num;
    uint32_t rx_skb_alloc_fail_cnt;
    bool lm_ctrl_enable;

    struct list_head stuck_check_list;
    uint32_t stuck_check_time;


#ifdef CONFIG_WIFI_RX_THREAD
    struct task_struct *rx_kthread;
    spinlock_t rx_lock;
    struct list_head rx_skbs;
    atomic_t rx_pending_count;
    wait_queue_head_t rx_wq;

    struct rx_thread_dbginfo rx_thread_dbg;

#endif

#ifdef CONFIG_SIWIFI_SORT_SCAN
    struct ipc_e2a_msg scan_results[MAX_SCAN_BSS_CNT];
    struct ipc_e2a_msg result_temp;
    struct cfg80211_bss *bss_results[MAX_SCAN_BSS_CNT];
    uint8_t scan_num;
#endif
    struct siwifi_atf atf;

#ifdef CONFIG_SF_SKB_POOL
	struct skb_pool_param_t *skb_pool_dev_param;
#endif
    //fixed gain set to lmac
    u32 fixed_gain;
    uint32_t fast_csa_time;
    uint32_t disable_cca_channel_switch;
    uint32_t reord_release_cnt;
    uint32_t reord_alloc_cnt;
#ifdef CONFIG_SIWIFI_CH_OFFSET
    int ch_offset;
#endif

#ifdef CONFIG_SIWIFI_IQENGINE
    struct siwifi_iqe iqe;
#endif
    char *beacon_insert_info;
    int beacon_insert_info_len;
    char *probe_insert_info;
    int probe_insert_info_len;
    char *assoc_insert_info;
    struct me_assoc_insert_info_req assoc_insert;
    char *auth_insert_info;
    struct me_auth_insert_info_req auth_insert;

#ifdef CONFIG_SIWIFI_EASYMESH
    sf_sta_timer_info sta_timer_info;

    struct list_head blocked_sta_list;
    spinlock_t blocked_sta_list_lock;

    bool easymesh_scan_enbale;
    struct list_head easymesh_scan_list;
    spinlock_t easymesh_scan_list_lock;

    wait_queue_head_t del_sta_wq;
    bool remove_sta_receive_cfm;
    bool remove_sta_success;

    bool block_connection;
#endif /* CONFIG_SIWIFI_EASYMESH */
    // statistics for debugging
    int siwifi_static_enable_tcp_check;
    int siwifi_static_timer_cur;
    int siwifi_static_timer_stop;
    struct timer_list siwifi_static_timer;
    struct siwifi_static_txinfo siwifi_static_txinfo[SIWIFI_STATIC_INFO_MAX];
    struct siwifi_static_txinfo siwifi_static_timeinfo[SIWIFI_STATIC_TIME_MAX][SIWIFI_STATIC_INFO_MAX];
    uint16_t siwifi_static_hwq_process;
    uint16_t siwifi_static_hwq_push[NX_TXQ_CNT];
    uint16_t siwifi_static_break_reason[SIWIFI_STATIC_BREAK_REASON_MAX];
    uint16_t ave_speed_cnt_thres;
    uint16_t ave_speed_credits_low;
    uint16_t ave_speed_credits_up;
};




#ifdef CONFIG_SF_SKB_POOL
#define MAX_WIFI_POOL_SKB_RAW_SIZE     ( 2816)
#endif

#ifdef CONFIG_SIWIFI_CACHE_ALLOC
extern struct kmem_cache *sw_txhdr_cache;
#endif

u8 *siwifi_build_bcn(struct siwifi_bcn *bcn, struct cfg80211_beacon_data *new, u8 **real_addr);

void siwifi_chanctx_link(struct siwifi_vif *vif, u8 idx,
                        struct cfg80211_chan_def *chandef);
void siwifi_chanctx_unlink(struct siwifi_vif *vif);
int  siwifi_chanctx_valid(struct siwifi_hw *siwifi_hw, u8 idx);

static inline bool is_multicast_sta(int sta_idx)
{
    return (sta_idx >= NX_REMOTE_STA_MAX);
}
struct siwifi_sta *siwifi_get_sta(struct siwifi_hw *siwifi_hw, const u8 *mac_addr);

static inline uint8_t master_vif_idx(struct siwifi_vif *vif)
{
    if (unlikely(vif->wdev.iftype == NL80211_IFTYPE_AP_VLAN)) {
        return vif->ap_vlan.master->vif_index;
    } else {
        return vif->vif_index;
    }
}

void siwifi_external_auth_enable(struct siwifi_vif *vif);
void siwifi_external_auth_disable(struct siwifi_vif *vif);

struct siwifi_sw_txhdr *siwifi_alloc_swtxhdr(struct siwifi_hw *siwifi_hw, struct sk_buff *skb);
void siwifi_free_swtxhdr(struct siwifi_hw *siwifi_hw, struct siwifi_sw_txhdr *sw_txhdr);

void siwifi_sta_hash_del(struct siwifi_vif *vif, struct siwifi_sta *sta);
void siwifi_sta_hash_add(struct siwifi_vif *vif, struct siwifi_sta *sta);
struct siwifi_sta *siwifi_sta_hash_get(struct siwifi_vif *vif, const u8 *addr);
int siwifi_check_skb_is_dhcp(struct sk_buff *skb);
void siwifi_update_src_filter(struct siwifi_vif *siwifi_vif, unsigned char *src_mac);
struct siwifi_src_filter *siwifi_src_filter_hash_search(struct siwifi_vif *siwifi_vif, unsigned char *src_mac);
void siwifi_src_filter_hash_free(struct siwifi_vif *siwifi_vif);
int siwifi_set_assoc_req_insert_info(struct siwifi_hw *siwifi_hw, char *insert_info, int insert_length);
void src_filter_aging(unsigned long ptr);
void set_repeater_status(struct siwifi_vif *siwifi_vif, u8 status);
int siwifi_channel_recovery_check(struct siwifi_hw *siwifi_hw);
bool siwifi_params_5g_channel_check(const uint8_t *data, uint8_t len);
int siwifi_set_assoc_insert_info(struct siwifi_hw *siwifi_hw, char *insert_info, int insert_length);
int siwifi_set_auth_insert_info(struct siwifi_hw *siwifi_hw, char *insert_info, int insert_length);
int siwifi_set_probe_insert_info(struct siwifi_hw *siwifi_hw, char *insert_info, int insert_length);

#ifdef CONFIG_SIWIFI_EASYMESH
void siwifi_easymesh_update_scan_result(struct siwifi_hw *siwifi_hw, struct scanu_result_ind *ind);
void siwifi_easymesh_scan_done_hook(struct siwifi_hw *siwifi_hw);
bool siwifi_easymesh_is_sta_blocked(struct siwifi_hw *siwifi_hw, const uint8_t *mac, const uint8_t *bssid);
bool siwifi_check_connection_block(struct siwifi_hw *siwifi_hw, struct ieee80211_mgmt *mgmt);
#endif /* CONFIG_SIWIFI_EASYMESH */

#endif /* _SIWIFI_DEFS_H_ */
