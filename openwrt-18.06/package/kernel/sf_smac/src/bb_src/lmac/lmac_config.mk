#config
#Enable BEACON transmission (need FW support)
CONFIG_SIWIFI_BCN=y
# Enable A-MPDU support (need FW support)
CONFIG_SIWIFI_AGG_TX=y
# Enable A-MSDU support (need FW support)
CONFIG_SIWIFI_SPLIT_TX_BUF=y

# Enable Fullmac arch
CONFIG_SF16A18_USE_FMAC ?= n
# Enable interruption optimization
CONFIG_SIWIFI_IRQ_OPT=y
# Enable WiFi lite memory, once enable lite memory, we will keep a
# low memory usage
CONFIG_WIFI_LITE_MEMORY ?= n

# DONT use MASK to handle TXDESC CNT
CFG_DONT_USE_TXDESC_MASK=y

# Fragment attack patch
CONFIG_FA_PATCH=y

# Enable RF GAIN TABLE SET
CONFIG_RF_GAIN_TABLE ?= n

# Enable BFMER support (need FW support)
#CONFIG_SIWIFI_BFMER=y
# Enable BFMER support (need FW support)
#CONFIG_SIWIFI_BFMEE=y
#CONFIG_SIWIFI_MUMIMO_TX=y
ifeq ($(CONFIG_A28), y)
CONFIG_MULTI_NSS=y
CONFIG_NSS_2=y
ccflags-y += -DCFG_MDM_VER_V22
ccflags-y += -DCONFIG_SF19A28
#for handle dma overlap problem when dma 32-byte patten which will overlap bytes after that
# This has been fixed in A28 CS version
#ccflags-y += -DWORKAROUND_6251
else
#CONFIG_MULTI_NSS=y
#CONFIG_NSS_2=y
ccflags-y += -DWORKAROUND_4505
ccflags-y += -DWORKAROUND_2058
#workaround for mult ac's problems that our TXOP_LIMIT none zero feature has bug.
ccflags-y += -DCFG_WORK_ROUND_OF_FIXED_TXOP_LIMIT_TO_ZERO_FOR_MULT_AC_PROBLEM
ccflags-y += -DWORKAROUND_timings1
ccflags-y += -DCFG_MDM_VER_V20
endif
ccflags-y += -DWORKAROUND_7577

ccflags-y += -DCFG_SET_POWER_LVL
ccflags-y += -DCFG_RF_GET_TEMP
ccflags-y += -DCFG_COOLING_TEMP
ccflags-y += -DCFG_MU_CNT=1

CONFIG_DUAL_ANTENNA_CALIBRATE=y
ifeq ($(CFG_DONT_USE_TXDESC_MASK), y)
ccflags-y += -DCFG_DONT_USE_TXDESC_MASK
endif

ifeq ($(CONFIG_MULTI_NSS), y)
ccflags-y += -DCONFIG_MULTI_NSS
endif

ifeq ($(CONFIG_NSS_2), y)
ccflags-y += -DCONFIG_NSS_2
endif

ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
ccflags-y += -DCFG_LITE_MEM
endif

#Enable INLINE OPT
ifeq ($(CONFIG_INLINE_OPT), y)
ccflags-y += -DCONFIG_INLINE_OPT
endif

ifeq ($(CONFIG_SIWIFI_MUMIMO_TX), y)
ccflags-y += -DCFG_MU_CNT=2
endif

#Enable radar to handle all software level issues
ifeq ($(CONFIG_SF16A18_USE_FMAC), y)
CONFIG_SIWIFI_RADAR=y
endif

#enable heart beat
CONFIG_SIWIFI_HEART_BEAT ?= y

ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
CONFIG_DBG_DUMP=n
else
# Enable debuginfo alloc/dump
CONFIG_DBG_DUMP ?= n
endif

# Enable TDLS support
CONFIG_SIWIFI_TDLS=n

# Enable Mesh support
ifneq ($(CONFIG_SF16A18_USE_FMAC), n)
#fullmac need extra pay for mesh, keep it disable.
CONFIG_SIWIFI_MESH=n
# Lite memory now cannot support mesh
ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
CONFIG_SIWIFI_MESH=n
endif
else
CONFIG_SIWIFI_MESH=n
endif

# Channel Offset
CONFIG_SIWIFI_CH_OFFSET=n

# Enable TRX STAT DUMP
CONFIG_SIWIFI_TRX_STAT=y
ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
CONFIG_SIWIFI_TRX_STAT=n
endif

#Enable tx power calibrate logic, please note that tx power calibaration
# is disabled or enabled by the factory section content stored in flash
CONFIG_SIWIFI_TX_POWER_CALI = y

ifeq ($(CONFIG_SFA28_FULLMASK),y)
#enable digital gain table
ccflags-y += -DCONFIG_ENABLE_DIGGAINTABLE
#enable independent antenna control (changing 1*1 when needed)
#The number of antennas can not be set normally in A28 due to a hardware bug, so comment out it.
#ccflags-y += -DINDEPENDENT_ANTENNA_CONTROL
#enable txpower with gain table
ccflags-y += -DCONFIG_ENABLE_TXPOWERTABLE
endif

ifeq ($(CONFIG_RF_GAIN_TABLE),y)
ccflags-y += -DCONFIG_ENABLE_RFGAINTABLE
endif

#Enable unstandard wireless channel
CONFIG_SIWIFI_UNSTANDARD_CH_ENABLE = n

#sometimes we limit the tx desc max cnt for BE/VI
CONFIG_SIWIFI_TXDESC_MAX ?= 64

#Share below var define to LMAC
ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
CONFIG_SIWIFI_VIRT_DEV_MAX=4
CONFIG_SIWIFI_CHAN_CTXT_CNT=3
CONFIG_SIWIFI_REMOTE_STA_MAX=32
else
CONFIG_SIWIFI_VIRT_DEV_MAX=4
CONFIG_SIWIFI_CHAN_CTXT_CNT=3
CONFIG_SIWIFI_REMOTE_STA_MAX=64
endif
CONFIG_SIWIFI_TX_MAX_RATES=4
CONFIG_SIWIFI_MU_GROUP_MAX=62

CONFIG_SIWIFI_TXDESC0_CNT=$(CONFIG_SIWIFI_TXDESC_MAX)
CONFIG_SIWIFI_TXDESC1_CNT=$(CONFIG_SIWIFI_TXDESC_MAX)
CONFIG_SIWIFI_TXDESC2_CNT=$(CONFIG_SIWIFI_TXDESC_MAX)
CONFIG_SIWIFI_TXDESC3_CNT=$(CONFIG_SIWIFI_TXDESC_MAX)
CONFIG_SIWIFI_TXDESC4_CNT=32

CONFIG_SIWIFI_TXDESC0_MAX_CNT=1
CONFIG_SIWIFI_TXDESC1_MAX_CNT=1
CONFIG_SIWIFI_TXDESC2_MAX_CNT=1
CONFIG_SIWIFI_TXDESC3_MAX_CNT=1
CONFIG_SIWIFI_TXDESC4_MAX_CNT=1
ifeq ($(CONFIG_WIFI_LITE_MEMORY), y)
CONFIG_SIWIFI_TXDESC4_CNT=4
endif

#Header dir list
SF16A18_LMAC_INCLUDES = -I$(lmac_dir)	\
		 -I$(lmac_dir)/plf/refip/src/arch/mips/boot/mips-gcc/ \
         -I$(lmac_dir)/plf/refip/src/arch/mips/ll/mips-gcc/ \
         -I$(lmac_dir)/plf/refip/src/compiler/mips-gcc/ \
         -I$(lmac_dir)/plf/refip/src/driver/sysctrl/ \
         -I$(lmac_dir)/plf/refip/build/reg/mips/$(PLATFORM)/ \
         -I$(lmac_dir)/plf/refip/src/arch/mips/ \
         -I$(lmac_dir)/plf/refip/src/driver/crm/ \
         -I$(lmac_dir)/plf/refip/src/driver/reg/ \
         -I$(lmac_dir)/plf/refip/src/driver/intc/mips/ \
         -I$(lmac_dir)/plf/refip/src/driver/ipc/ \
         -I$(lmac_dir)/plf/refip/src/driver/dma/ \
         -I$(lmac_dir)/plf/refip/src/driver/phy/ \
         -I$(lmac_dir)/plf/refip/src/driver/la/ \
         -I$(lmac_dir)/plf/refip/src/driver/ldfw/ \
         -I$(lmac_dir)/plf/refip/src/driver/rf/ \
         -I$(lmac_dir)/ip/lmac/src/rx/ \
         -I$(lmac_dir)/ip/lmac/src/rx/rxl/ \
         -I$(lmac_dir)/ip/lmac/src/tx/txl/ \
         -I$(lmac_dir)/ip/lmac/src/sta/ \
         -I$(lmac_dir)/ip/lmac/src/hal/ \
         -I$(lmac_dir)/ip/lmac/src/tx/ \
         -I$(lmac_dir)/ip/lmac/src/mm/ \
         -I$(lmac_dir)/ip/lmac/src/p2p/ \
         -I$(lmac_dir)/ip/lmac/src/ps/ \
         -I$(lmac_dir)/ip/lmac/src/rd/ \
         -I$(lmac_dir)/ip/lmac/src/siwifi/ \
         -I$(lmac_dir)/ip/lmac/src/scan/ \
         -I$(lmac_dir)/ip/lmac/src/sta/ \
         -I$(lmac_dir)/ip/lmac/src/vif/ \
         -I$(lmac_dir)/ip/lmac/src/tdls/ \
         -I$(lmac_dir)/modules/app/src/ \
         -I$(lmac_dir)/modules/dbg/src/ \
         -I$(lmac_dir)/modules/common/src/ \
         -I$(lmac_dir)/modules/ke/src/ \
         -I$(lmac_dir)/modules/ke/src/template/ \
         -I$(lmac_dir)/modules/mac/src/ \
		 -I$(lmac_dir)/ip/lmac/src/chan/ \
		 -I$(lmac_dir)/ip/lmac/src/tpc/ \
		 -I$(lmac_dir)/ip/lmac/src/td/ \
		 -I$(lmac_dir)/ip/lmac/src/bfr/ \
		 -I$(lmac_dir)/ip/lmac/src/tdls/

ifeq ($(CONFIG_A28), y)
SF16A18_LMAC_INCLUDES += -I$(lmac_dir)/plf/refip/build/reg/mips/$(PLATFORM)/begonia/
else
SF16A18_LMAC_INCLUDES += -I$(lmac_dir)/plf/refip/build/reg/mips/$(PLATFORM)/aetnensis/
endif

ifeq ($(CONFIG_SIWIFI_MESH), y)
SF16A18_LMAC_INCLUDES += -I$(lmac_dir)/modules/mesh/src/
endif

ifneq ($(CONFIG_SF16A18_USE_FMAC), n)
SF16A18_LMAC_INCLUDES += -I$(lmac_dir)/ip/umac/src/scanu/	\
						 -I$(lmac_dir)/ip/umac/src/rxu/	\
						 -I$(lmac_dir)/ip/umac/src/txu/	\
						 -I$(lmac_dir)/ip/umac/src/me/	\
						 -I$(lmac_dir)/ip/umac/src/sm/	\
						 -I$(lmac_dir)/ip/umac/src/apm/	\
						 -I$(lmac_dir)/ip/umac/src/llc/	\
						 -I$(lmac_dir)/ip/umac/src/bam/	\
						 -I$(lmac_dir)/ip/umac/src/mfp/	\
						 -I$(lmac_dir)/ip/umac/src/rc/	\
						 -I$(lmac_dir)/ip/umac/src/hsu/
endif

#Define macro
#support context in lmac
ccflags-y += -DCFG_EXT_CONTEXT_SUPPORT
ccflags-y += -DCFG_SHAREMEM_ACCESS_ALIGNMENT=4
ccflags-y += -Wno-unused-function #TODO this should move to linux global compile option in future
ccflags-y += -Wno-unused-value #TODO this should move to linux global compile option in future

#workaround for dma's bug in v1 platform
ccflags-y += -DCFG_WORK_AROUND_DMA_READ_REORDER_ISSUE

ifeq ($(FULLMASK), TRUE)
#ccflags-y += -DCFGPDMA_UNORDERED_MODE_SUPPROT
endif

#following macro no use now
#ccflags-y += -DCFG_PRODUCT=lmac #only indicate some compile direction, no use now
#ccflags-y += -DCFG_ARCH=aps3 #only indicate some compile direction, no use now
#ccflags-y += -DCFG_OPTIM=1  #this is compile optim, we now use linux global optim
#ccflags-y += -DCFG_VERBOSE=0 #not use in code no care this
#ccflags-y += -DCFG_SAVETMP=0 #not use in code no care this
#ccflags-y += -DCFG_REM=0    #not use in code no care this

#ccflags-y += -DCONFIG_SIWIFI_SPLIT_TX_BUF=1 this not use in lmac
#ccflags-y += -DCONFIG_SIWIFI_DEBUGFS=1 this not use in lmac
#ccflags-y += -DCONFIG_SIWIFI_AGG_TX=1  this not use in lmac
#ccflags-y += -DCONFIG_SIWIFI_BCN=1 this not use in lmac
#ccflags-y += -DNX_TXQ_CNT=5 this not use in lmac
#ccflags-y += -DNX_VIRT_DEV_MAX=4 #this lmac used but already defined in siwifi_config.h
#ccflags-y += -DNX_TXDESC_CNT=64 this not use in lmac or umac
#ccflags-y += -DNX_TX_MAX_RATES=4 this not use in lmac
#ccflags-y += -DNX_CHAN_CTXT_CNT=3 this use in lmac but already defined in siwifi_config.h
#ccflags-y += -DCFG_RWTL no use

ccflags-$(CONFIG_SF16A18_USE_FMAC) += -DCFG_UMAC
ifneq ($(CONFIG_SF16A18_USE_FMAC), n)
ccflags-y += -DCFG_VHT
#enable MPF by default
#ccflags-y += -DCFG_MFP
ccflags-y += -DCFG_REORD_BUF=64
ccflags-y += -DCFG_BARX=$(CONFIG_SIWIFI_REMOTE_STA_MAX)
ccflags-y += -DCFG_BATX=$(CONFIG_SIWIFI_REMOTE_STA_MAX)
ccflags-y += -DCFG_DEAGG
endif
#force HW_ACTIVE case we may not have 32k clk
ccflags-y += -DFORCE_HW_ACTIVE

#enable heart beat by default
ccflags-$(CONFIG_SIWIFI_HEART_BEAT) += -DCFG_HB

#enable vendor hw interface config
#ccflags-y += -DCONFIG_VDR_HW

ifneq ($(CONFIG_WIFI_LITE_MEMORY), y)
ccflags-y += -DCFG_PROF
#ccflags-y += -DCFG_LA_ENABLE
#ccflags-y += -DCFG_A28_V_LA_CLK_BUG
#ccflags-y += -DCFG_A28_FULLMASK_LA_BUG
endif
ccflags-y += -DCFG_DBG=2
ccflags-$(CONFIG_SIWIFI_IRQ_OPT) += -DCFG_IRQ_OPT


ifeq ($(CONFIG_SIWIFI_BCN), y)
ccflags-y += -DCFG_BCN
endif
ifeq ($(CONFIG_SIWIFI_RADAR), y)
ccflags-y += -DCFG_RADAR
endif

#thisNX_REMOTE_STA_MAX is used for LMAC, sync to UMAC macro control CONFIG_SIWIFI_REMOTE_STA_MAX
ifdef CONFIG_SIWIFI_REMOTE_STA_MAX
ccflags-y += -DNX_REMOTE_STA_MAX=$(CONFIG_SIWIFI_REMOTE_STA_MAX)
else #default value set 10
ccflags-y += -DNX_REMOTE_STA_MAX=10
endif

#this NX_VIRT_DEV_MAX is used for LMAC, sync to UMAC macro control CONFIG_SIWIFI_VIRT_DEV_MAX
ifdef CONFIG_SIWIFI_VIRT_DEV_MAX
ccflags-y += -DNX_VIRT_DEV_MAX=$(CONFIG_SIWIFI_VIRT_DEV_MAX)
else #default value set 4
ccflags-y += -DNX_VIRT_DEV_MAX=4
endif

ifdef CONFIG_SIWIFI_TX_MAX_RATES
ccflags-y += -DNX_TX_MAX_RATES=$(CONFIG_SIWIFI_TX_MAX_RATES)
else
ccflags-y += -DNX_TX_MAX_RATES=4
endif

#this NX_CHAN_CTXT_CNT is used for LMAC, sync to UMAC macro control CONFIG_SIWIFI_CHAN_CTXT_CNT
ifdef CONFIG_SIWIFI_CHAN_CTXT_CNT
ccflags-y += -DNX_CHAN_CTXT_CNT=$(CONFIG_SIWIFI_CHAN_CTXT_CNT)
else #default value set 4
ccflags-y += -DNX_CHAN_CTXT_CNT=3
endif

ifdef CONFIG_SIWIFI_MU_GROUP_MAX
subdir-ccflags-y += -DNX_MU_GROUP_MAX=$(CONFIG_SIWIFI_MU_GROUP_MAX)
else
subdir-ccflags-y += -DNX_MU_GROUP_MAX=62
endif

#configs for AGG & BA
ccflags-$(CONFIG_SIWIFI_AGG_TX) += -DCFG_AGG
#ccflags-y += -DCFG_AMSDU_4K
ccflags-y += -DCFG_AMSDU_8K
#RM#13156 reduce ampdu density to 8 to improve thoughput when amsdu is off
ccflags-y += -DCFG_SPC=8
#this is ugly for lmac, because this micro is messed up with AMSDU in the original code
ccflags-$(CONFIG_SIWIFI_SPLIT_TX_BUF) += -DCFG_AMSDU
ccflags-y += -DCONFIG_SIWIFI_MAX_MSDU_PER_RX=32

#configs for txq
ccflags-y += -DCFG_TXDESC0=$(CONFIG_SIWIFI_TXDESC0_CNT)
ccflags-y += -DCFG_TXDESC1=$(CONFIG_SIWIFI_TXDESC1_CNT)
ccflags-y += -DCFG_TXDESC2=$(CONFIG_SIWIFI_TXDESC2_CNT)
ccflags-y += -DCFG_TXDESC3=$(CONFIG_SIWIFI_TXDESC3_CNT)
ccflags-y += -DCFG_TXDESC4=$(CONFIG_SIWIFI_TXDESC4_CNT)
#configs for skbs per description
ccflags-y += -DCONFIG_SIWIFI_TXDESC0_MAX_CNT=$(CONFIG_SIWIFI_TXDESC0_MAX_CNT)
ccflags-y += -DCONFIG_SIWIFI_TXDESC1_MAX_CNT=$(CONFIG_SIWIFI_TXDESC1_MAX_CNT)
ccflags-y += -DCONFIG_SIWIFI_TXDESC2_MAX_CNT=$(CONFIG_SIWIFI_TXDESC2_MAX_CNT)
ccflags-y += -DCONFIG_SIWIFI_TXDESC3_MAX_CNT=$(CONFIG_SIWIFI_TXDESC3_MAX_CNT)
ccflags-y += -DCONFIG_SIWIFI_TXDESC4_MAX_CNT=$(CONFIG_SIWIFI_TXDESC4_MAX_CNT)

#following should be match to  Dynamic config in siwifi_hw->mod_params
ccflags-y += -DCFG_AUTOBCN
ccflags-y += -DCFG_HWSCAN
ccflags-y += -DCFG_DPSM
ccflags-y += -DCFG_PS
ccflags-y += -DCFG_UAPSD
ccflags-y += -DCFG_CMON
ccflags-y += -DCFG_MROLE
ccflags-y += -DCFG_P2P=2
ccflags-y += -DCFG_P2P_DBG
ccflags-y += -DCFG_P2P_NOA_GO
ccflags-y += -DCFG_P2P_OPPPS_GO
#ccflags-y += -DCFG_WAPI
ifeq ($(CONFIG_A28), y)
ccflags-$(CONFIG_SIWIFI_BFMER) += -DCFG_BFMER
ccflags-$(CONFIG_SIWIFI_BFMEE) += -DCFG_BFMEE
endif
ifeq ($(CONFIG_DBG_DUMP), y)
ccflags-y += -DCFG_DBGDUMP
endif
ccflags-y += -DCFG_REC
ccflags-y += -DCFG_EMB
ccflags-y += -DCFG_BWLEN
#Compilation for a HW supporting Key RAM configuration
ccflags-y += -DCFG_KEYCFG
ccflags-y += -DCFG_RECALI
ifneq ($(CONFIG_WIFI_LITE_MEMORY), y)
ccflags-y += -DCFG_DEBUG_RF_CMD_DUMP
endif
ccflags-$(CONFIG_SIWIFI_TX_POWER_CALI) += -DCFG_TX_POWER_CALI
ccflags-$(CONFIG_DUAL_ANTENNA_CALIBRATE) += -DCFG_DUAL_ANTENNA_CALIBRATE
ccflags-$(CONFIG_SIWIFI_UNSTANDARD_CH_ENABLE) += -DCFG_UNSTANDARD_CH_ENABLE
ifneq ($(CONFIG_SF16A18_USE_FMAC), n)
ccflags-y += -DCFG_ATE_TOOLS
endif
ifeq ($(V1), TRUE)
ccflags-y += -DCFG_IPC_VER_V10
endif
ccflags-y += -DCFG_PLF_VER_V20

ccflags-y += -DCFG_MAC_VER_V10
#ccflags-y += -DEXPORT_SYMTAB

#mesh config
#todo: now lmac not support mesh commands
ifeq ($(CONFIG_SIWIFI_MESH), y)
ccflags-y += -DCFG_MESH
# Number of supported Mesh Point Interfaces
ccflags-y += -DCFG_MESH_VIF=1
# Number of supported Mesh Links (shared between the Mesh VIFs)
ccflags-y += -DCFG_MESH_LINK=2
# Number of supported Mesh Paths
ccflags-y += -DCFG_MESH_PATH=5
# Number of supported Mesh Proxy Information
ccflags-y += -DCFG_MESH_PROXY=10
endif

#ccflags-y += -DCFG_STATS
ccflags-$(CONFIG_SIWIFI_TRX_STAT) += -DCFG_TRX_STAT

ccflags-$(CONFIG_SIWIFI_CH_OFFSET) += -DCFG_CH_OFFSET

###Trace Buffer Support
#ccflags-y += -DCFG_TRACE
#todo: TRACE_FILE_ID is passed on the compilation line and is different for each file ,it's auto built by config/SConscript
#ccflags-y += -DTRACE_FILE_ID=1

#TDLS support
ccflags-$(CONFIG_SIWIFI_TDLS) += -DCFG_TDLS

# HSU support. Possible values are: \
 	0: Don't use HSU, use software implementation.	\
	1: Use HSU and fallback to software implementation if not available. \
	2: Only use HSU. (runtime error is generated if HSU is not available)
ifeq ($(V1), TRUE)
ccflags-y += -DCFG_HSU=0
else
ccflags-y += -DCFG_HSU=0
endif

ccflags-$(CONFIG_FA_PATCH) += -DCONFIG_FA_PATCH
