CONFIG_SF16A18_USE_FMAC=y

ifneq ($(CONFIG_SF16A18_TXQ_MAX_CNT),)
CONFIG_SIWIFI_TXDESC_MAX=$(CONFIG_SF16A18_TXQ_MAX_CNT)
export CONFIG_SIWIFI_TXDESC_MAX
endif

# Enable Monitor+Data interface support (need FW support)
CONFIG_SIWIFI_MON_DATA ?= n

include $(lmac_dir)/lmac_config.mk
include $(umac_dir)/umac_config.mk

fullmac_ref_dir=$(root_src_ref_dir)/fullmac

SF16A18_FULLMAC_SRC_OBJS := $(SF16A18_UMAC_SRC_OBJS)
SF16A18_FULLMAC_SRC_OBJS +=						 \
			  $(fullmac_ref_dir)/siwifi_rx.o              \
              $(fullmac_ref_dir)/siwifi_tx.o              \
              $(fullmac_ref_dir)/siwifi_main.o

ifeq ($(CONFIG_SIWIFI_TDLS),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_tdls.o
endif

ifeq ($(CONFIG_SF19A28_WIFI_LED),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_led.o
endif

ifeq ($(CONFIG_SIWIFI_ACS),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_acs.o
else ifeq ($(CONFIG_SIWIFI_ACS_INTERNAL),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_acs.o
endif

ifeq ($(CONFIG_SIWIFI_MESH),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_mesh.o
endif

ifeq ($(CONFIG_SF16A18_WIFI_ATE_TOOLS),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_ioctl.o
endif

ifeq ($(CONFIG_SIWIFI_IGMP),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_igmp.o
endif

CONFIG_SIWIFI_IQE = y
ifeq ($(CONFIG_SIWIFI_IQE),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_iqengine.o
ccflags-y += -DCONFIG_SIWIFI_IQENGINE
endif

ifeq ($(CONFIG_SIWIFI_REPEATER),y)
SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_repeater.o
endif

SF16A18_FULLMAC_SRC_OBJS += $(fullmac_ref_dir)/siwifi_frame.o

ccflags-y += -DLOCAL_RECOVERY

# on lite memory platform, we disable TOKEN enable before
# we have a nice aggregation encourage mechanism
#ifneq ($(CONFIG_WIFI_LITE_MEMORY),y)
ifeq ($(CONFIG_SFFMAC_ENABLE_TOKEN), y)
ccflags-y += -DTOKEN_ENABLE
endif
#endif

ccflags-y += -I$(umac_dir)/fullmac/
ccflags-y += -I$(umac_dir)/
ccflags-$(CONFIG_SIWIFI_MON_DATA) += -DCONFIG_SIWIFI_MON_DATA
ccflags-$(CONFIG_SIWIFI_TDLS) += -DCONFIG_SIWIFI_TDLS
ccflags-$(CONFIG_SIWIFI_MESH) += -DCONFIG_SIWIFI_MESH
ifeq ($(CONFIG_SIWIFI_SPLIT_TX_BUF), y)
ccflags-y += -DCONFIG_SIWIFI_AMSDUS_TX
endif

ifneq ($(CONFIG_SIWIFI_DISABLE_BRIDGE_ACCEL),y)
ccflags-y += -DCONFIG_BRIDGE_ACCELERATE
endif

# user private skb buffer pool
# When defined this, we will use our own private memory pool to manage rx skb pool
# that means we keep serveral rx skb buffer in our own list and did not return it to system memory
# use this method to speed up the rx skb alloc flow
ifeq ($(CONFIG_SFFMAC_ENABLE_RX_MEM_POOL), y)
ccflags-y += -DCONFIG_PRIV_RX_BUFFER_POOL
endif

ccflags-y += -DCONFIG_SIWIFI_SAVE_TXHDR_ALLOC

# use this to debug ndevq stop/restart problem
#ccflags-y += -DCONFIG_DEBUG_TXQ_STOP
# dedicated RX thread
# When defined this, we will create a dedicated rx thrread to process rx skb
# on some platform, they did not use rps, so the rx flow is too long unless we use a rx thread
# to cut it off
#ccflags-y += -DCONFIG_WIFI_RX_THREAD

quiet_cmd_genvers = GENVERSION $@
      cmd_genvers = ($(if $(KBUILD_EXTMOD),,$(srctree)/)$(src)/../mkvers.sh $@)

$(obj)/siwifi_main.o: $(obj)/siwifi_version_gen.h

$(obj)/siwifi_version_gen.h: FORCE
	$(call cmd,genvers)

clean-files := siwifi_version_gen.h
