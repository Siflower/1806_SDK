SIWIFI_VERS_NUM=6.0.0


#
# WAITING FOR KCONFIG {
#

#
# DEBUG OPTIONS
CONFIG_SIWIFI_UM_HELPER_DFLT="/bin/siwifi_umh.sh"

#
# FW ARCH:
#CONFIG_SIWIFI_SDM=y
#CONFIG_SIWIFI_TL4=y

# Support of P2P DebugFS for enabling/disabling NoA and OppPS
CONFIG_SIWIFI_P2P_DEBUGFS=n
# Support fw softwace function trace
CONFIG_SIWIFI_FW_TRACE=n

# extra DEBUG config
CONFIG_SIWIFI_DBG = n
#CONFIG_SIWIFI_RF_CMD_DUMP = y
#Enable heart beat
CONFIG_SIWIFI_IRQ_OPT=y
CONFIG_SIWIFI_RF_RECALI=y

ifeq ($(CONFIG_SF16A18_RELEASE),y)
CONFIG_SIWIFI_HEART_BEAT = y
subdir-ccflags-y += -DCONFIG_SF16A18_RELEASE
endif
subdir-ccflags-$(CONFIG_SIWIFI_HEART_BEAT) += -DCONFIG_HEART_BEAT

#CONFIG_SIWIFI_DYNAMIC_TXPOWER = y

ifeq ($(CONFIG_DEBUG_FS), y)
CONFIG_SIWIFI_DEBUGFS = y
CONFIG_SIWIFI_PROCFS = n
else ifeq  ($(CONFIG_PROC_FS), y)
CONFIG_SIWIFI_DEBUGFS = n
CONFIG_SIWIFI_PROCFS = y
CONFIG_SIWIFI_FW_TRACE = n
endif

#Testmode, but this micro is only for softmac, we need to implement the testops in fullmac
CONFIG_NL80211_TESTMODE ?= n
CONFIG_SIWIFI_TESTMODE = y
ifneq ($(CONFIG_NL80211_TESTMODE), y)
CONFIG_SIWIFI_TESTMODE = n
endif

#read configuration from cfg files
CONFIG_SIWIFI_CFGFILE = y

#cmd string information
CONFIG_SIWIFI_CMDSTR = y

ifeq ($(CONFIG_WIFI_LITE_MEMORY),y)
CONFIG_SIWIFI_TESTMODE = n
CONFIG_SIWIFI_CMDSTR = n
CONFIG_ERROR_DUMP = n
subdir-ccflags-y += -DCONFIG_WIFI_LITE_MEMORY
# Support fw software and hardware diags trace
# if this was n, then CONFIG_SIWIFI_FW_TRACE is not active
CONFIG_DBG_DUMP ?= y
else
CONFIG_ERROR_DUMP = y
# force DBG_DUMP to create diags folder in debugfs dir
CONFIG_DBG_DUMP=y
endif

#enable cooling temperature
subdir-ccflags-$(CONFIG_SF16A18_COOLING_TEMP) += -DCONFIG_SIWIFI_COOLING_TEMP

#enable temperature control
subdir-ccflags-$(CONFIG_SIWIFI_TEMPERATURE_CONTROL) += -DCONFIG_SIWIFI_TEMPERATURE_CONTROL

#support multicast (IGMP protocol)
subdir-ccflags-$(CONFIG_SIWIFI_IGMP) += -DCONFIG_SIWIFI_IGMP

#support repeater
subdir-ccflags-$(CONFIG_SIWIFI_REPEATER) += -DCONFIG_SIWIFI_REPEATER
subdir-ccflags-$(CONFIG_SIWIFI_REPEATER_IPV6) += -DCONFIG_SIWIFI_REPEATER_IPV6

subdir-ccflags-$(CONFIG_SIWIFI_ACS_INTERNAL) += -DCONFIG_SIWIFI_ACS_INTERNAL
# } // WAITING FOR KCONFIG

# this is because we port the mac80211/cfg80211 from include/generated/uapi/linux/version.h
subdir-ccflags-y += -DMY_LINUX_VERSION_CODE=265818
subdir-ccflags-$(CONFIG_SF16A18_WIFI_LED) += -DCONFIG_SF16A18_WIFI_LED
subdir-ccflags-$(CONFIG_SF19A28_WIFI_LED) += -DCONFIG_SF19A28_WIFI_LED

CONFIG_SIWIFI_ACS = n
subdir-ccflags-$(CONFIG_SIWIFI_ACS) += -DCONFIG_SIWIFI_ACS

CONFIG_SIWIFI_SORT_SCAN = n
subdir-ccflags-$(CONFIG_SIWIFI_SORT_SCAN) += -DCONFIG_SIWIFI_SORT_SCAN

CONFIG_VDR_HW = n
subdir-ccflags-$(CONFIG_VDR_HW) += -DCONFIG_VDR_HW

SF16A18_UMAC_SRC_OBJS :=	\
	$(root_src_ref_dir)/siwifi_msg_tx.o	\
	$(root_src_ref_dir)/siwifi_msg_rx.o	\
	$(root_src_ref_dir)/siwifi_utils.o	\
	$(root_src_ref_dir)/siwifi_cmds.o	\
    $(root_src_ref_dir)/siwifi_v1.o	\
    $(root_src_ref_dir)/siwifi_mod_params.o	\
    $(root_src_ref_dir)/siwifi_txq.o	\
    $(root_src_ref_dir)/ipc_host.o	\
	$(root_src_ref_dir)/siwifi_lmac_glue_barematel.o \
	$(root_src_ref_dir)/siwifi_traffic.o    \
	$(root_src_ref_dir)/siwifi_mem.o    \
	$(root_src_ref_dir)/siwifi_trace.o

ifeq ($(CONFIG_VDR_HW),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/hw_interface.o
endif
### IPC txdesc did not use mask method
#   this will limited the txdesc number be 2^n
subdir-ccflags-${CFG_DONT_USE_TXDESC_MASK} += -DCONFIG_SIWIFI_DONT_USE_TXDESC_MASK

###read configuration from cfg files
ifeq ($(CONFIG_SIWIFI_CFGFILE), y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_cfgfile.o
endif
subdir-ccflags-$(CONFIG_SIWIFI_CFGFILE) += -DCONFIG_SIWIFI_CFGFILE


###BFMER
ifeq ($(CONFIG_SIWIFI_BFMER),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_bfmer.o
endif
subdir-ccflags-$(CONFIG_SIWIFI_BFMER) += -DCONFIG_SIWIFI_BFMER


###MUMIMO
ifeq ($(CONFIG_SIWIFI_MUMIMO_TX),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_mu_group.o
endif
ifeq ($(CONFIG_SIWIFI_MUMIMO_TX), y)
subdir-ccflags-y += -DCONFIG_USER_MAX=2
else
subdir-ccflags-y += -DCONFIG_USER_MAX=1
endif
subdir-ccflags-$(CONFIG_SIWIFI_MUMIMO_TX) += -DCONFIG_SIWIFI_MUMIMO_TX

###RADAR
ifeq ($(CONFIG_SIWIFI_RADAR),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_radar.o
endif
subdir-ccflags-$(CONFIG_SIWIFI_RADAR) += -DCONFIG_SIWIFI_RADAR

###DEBUGFS
ifeq ($(CONFIG_SIWIFI_DEBUGFS),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_debugfs.o
CFLAGS_siwifi_debugfs.o = -Wframe-larger-than=4096
ifeq ($(CONFIG_DBG_DUMP),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_fw_dump.o

ifeq ($(CONFIG_SIWIFI_FW_TRACE),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_fw_trace.o
CFLAGS_siwifi_fw_trace.o = -Wframe-larger-than=2048
endif # CONFIG_SIWIFI_FW_TRACE
subdir-ccflags-$(CONFIG_SIWIFI_FW_TRACE) += -DCONFIG_SIWIFI_FW_TRACE
subdir-ccflags-$(CONFIG_DBG_DUMP) += -DCONFIG_DBG_DUMP
subdir-ccflags-$(CONFIG_SIWIFI_DEBUGFS) += -DCONFIG_SIWIFI_UM_HELPER_DFLT=\"$(CONFIG_SIWIFI_UM_HELPER_DFLT)\"
subdir-ccflags-$(CONFIG_ERROR_DUMP) += -DCONFIG_ERROR_DUMP
endif # CONFIG_DBG_DUMP

else ifeq ($(CONFIG_SIWIFI_PROCFS),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_debugfs.o
CFLAGS_siwifi_debugfs.o = -Wframe-larger-than=4096
ifeq ($(CONFIG_DBG_DUMP),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_fw_dump.o
ifeq ($(CONFIG_SIWIFI_FW_TRACE),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_fw_trace.o
CFLAGS_siwifi_fw_trace.o = -Wframe-larger-than=2048
endif # CONFIG_SIWIFI_FW_TRACE
subdir-ccflags-$(CONFIG_SIWIFI_FW_TRACE) += -DCONFIG_SIWIFI_FW_TRACE
subdir-ccflags-$(CONFIG_DBG_DUMP) += -DCONFIG_DBG_DUMP
subdir-ccflags-$(CONFIG_SIWIFI_PROCFS) += -DCONFIG_SIWIFI_UM_HELPER_DFLT=\"$(CONFIG_SIWIFI_UM_HELPER_DFLT)\"
subdir-ccflags-$(CONFIG_ERROR_DUMP) += -DCONFIG_ERROR_DUMP
endif # CONFIG_DBG_DUMP
endif # CONFIG_SIWIFI_DEBUGFS || CONFIG_SIWIFI_PROCFS
ifeq ($(CONFIG_SIWIFI_DEBUGFS),y)
subdir-ccflags-$(CONFIG_SIWIFI_DEBUGFS) += -DCONFIG_SIWIFI_DEBUGFS
else ifeq ($(CONFIG_SIWIFI_PROCFS),y)
subdir-ccflags-$(CONFIG_SIWIFI_PROCFS) += -DCONFIG_SIWIFI_PROCFS
endif

subdir-ccflags-$(CONFIG_SIWIFI_P2P_DEBUGFS) += -DCONFIG_SIWIFI_P2P_DEBUGFS

###Testmode
ifeq ($(CONFIG_SIWIFI_TESTMODE),y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_testmode.o
endif
subdir-ccflags-$(CONFIG_SIWIFI_TESTMODE) += -DCONFIG_SIWIFI_TESTMODE


###Command string dump
ifeq ($(CONFIG_SIWIFI_CMDSTR), y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_strs.o
endif
subdir-ccflags-$(CONFIG_SIWIFI_CMDSTR) += -DCONFIG_SIWIFI_CMDSTR

# FW ARCH:
subdir-ccflags-$(CONFIG_SIWIFI_SDM) += -DCONFIG_SIWIFI_SDM
subdir-ccflags-$(CONFIG_SIWIFI_TL4) += -DCONFIG_SIWIFI_TL4

# Channel offset
subdir-ccflags-$(CONFIG_SIWIFI_CH_OFFSET) += -DCONFIG_SIWIFI_CH_OFFSET

# Debug
subdir-ccflags-$(CONFIG_SIWIFI_DBG) += -DCONFIG_SIWIFI_DBG
subdir-ccflags-$(CONFIG_SIWIFI_TRX_STAT) += -DCONFIG_SIWIFI_TRX_STAT
subdir-ccflags-$(CONFIG_SIWIFI_RF_CMD_DUMP) += -DCONFIG_SIWIFI_RF_CMD_DUMP
###RF recalibration
subdir-ccflags-$(CONFIG_SIWIFI_RF_RECALI) += -DCONFIG_SIWIFI_RF_RECALI

# IRQ optimization
subdir-ccflags-$(CONFIG_SIWIFI_IRQ_OPT) += -DCONFIG_SIWIFI_IRQ_OPT

###FPGA
subdir-ccflags-y += -DCONFIG_SF16A18_WIFI_RF
#tx power calibration
subdir-ccflags-$(CONFIG_SIWIFI_TX_POWER_CALI) += -DCONFIG_SIWIFI_TX_POWER_CALI
ifeq ($(CONFIG_SIWIFI_TX_POWER_CALI), y)
SF16A18_UMAC_SRC_OBJS += $(root_src_ref_dir)/siwifi_factory_info.o
endif

###TX SPLIT BUFFER
subdir-ccflags-$(CONFIG_SIWIFI_SPLIT_TX_BUF) += -DCONFIG_SIWIFI_SPLIT_TX_BUF


###HW BCN
ifeq ($(CONFIG_SIWIFI_BCN), y)
subdir-ccflags-y += -DNX_TXQ_CNT=5
else
subdir-ccflags-y += -DNX_TXQ_CNT=4
endif
subdir-ccflags-$(CONFIG_SIWIFI_BCN) += -DCONFIG_SIWIFI_BCN


###TX QUEUE CNT
subdir-ccflags-y += -DNX_TXDESC_CNT0=$(CONFIG_SIWIFI_TXDESC0_CNT)
subdir-ccflags-y += -DNX_TXDESC_CNT1=$(CONFIG_SIWIFI_TXDESC1_CNT)
subdir-ccflags-y += -DNX_TXDESC_CNT2=$(CONFIG_SIWIFI_TXDESC2_CNT)
subdir-ccflags-y += -DNX_TXDESC_CNT3=$(CONFIG_SIWIFI_TXDESC3_CNT)
subdir-ccflags-y += -DNX_TXDESC_CNT4=$(CONFIG_SIWIFI_TXDESC4_CNT)

subdir-ccflags-y += -DNX_MAX_SKB_TXDESC0=$(CONFIG_SIWIFI_TXDESC0_MAX_CNT)
subdir-ccflags-y += -DNX_MAX_SKB_TXDESC1=$(CONFIG_SIWIFI_TXDESC1_MAX_CNT)
subdir-ccflags-y += -DNX_MAX_SKB_TXDESC2=$(CONFIG_SIWIFI_TXDESC2_MAX_CNT)
subdir-ccflags-y += -DNX_MAX_SKB_TXDESC3=$(CONFIG_SIWIFI_TXDESC3_MAX_CNT)
subdir-ccflags-y += -DNX_MAX_SKB_TXDESC4=$(CONFIG_SIWIFI_TXDESC4_MAX_CNT)

subdir-ccflags-y += -DCONFIG_SIWIFI_CACHE_ALLOC
subdir-ccflags-y += -DCONFIG_SIWIFI_TRACE_SKB
