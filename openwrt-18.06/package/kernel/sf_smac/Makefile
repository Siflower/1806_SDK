include $(TOPDIR)/rules.mk

PKG_NAME:=sf_smac
PKG_RELEASE=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_smac
  SUBMENU:=Wireless Drivers
  TITLE:=SIFLOWER sf16a18 wireless driver
  DEPENDS:=+kmod-sfax8-factory-read +kmod-cfg80211 +@DRIVER_11N_SUPPORT +@DRIVER_11AC_SUPPORT
  FILES:= \
	  	$(PKG_BUILD_DIR)/sf16a18_rf.ko \
		$(PKG_BUILD_DIR)/startcore.ko
ifdef CONFIG_PACKAGE_SFUMAC_FMAC
ifndef CONFIG_FLASH_OPTIMIZE
	FILES += $(PKG_BUILD_DIR)/sf16a18_fmac.ko
endif
else
	FILES += $(PKG_BUILD_DIR)/sf16a18_smac.ko
endif
endef

define KernelPackage/sf_smac/config
	if PACKAGE_kmod-sf_smac
		config PACKAGE_SFSMAC_DBGINFO_ALLOCS
			bool "enable dbginfo alloc memory"
			default "y"

		config PACKAGE_SFSMAC_LB_LA_ENABLE
			bool "enable LA function for low band"
			default "n"

		config PACKAGE_SFSMAC_HB_LA_ENABLE
			bool "enable LA function for high band"
			depends on !PACKAGE_SFSMAC_LB_LA_ENABLE
			default "n"

		config PACKAGE_SFUMAC_WIFI_TEST_SCRIPTS
			bool "enable test shell scripts"
			default "y"

		config SF16A18_COOLING_TEMP
			bool "enable openwrt themal control"
			default "n"

		config PACKAGE_SFUMAC_WIFI_ATE_TOOLS
			bool "enable wifi ate tool"
			default "y"

		config PACKAGE_SFUMAC_LB_EXT_PA_SUPPORT
			bool "enable support lb ext pa detect"
			default "y"

		config PACKAGE_SFUMAC_HB_EXT_PA_SUPPORT
			bool "enable support hb ext pa detect"
			default "y"

		config SFFMAC_ENABLE_TOKEN
			bool "enable token for fmac"
			default "n"

		config SFFMAC_ENABLE_RX_MEM_POOL
			bool "enable rx memery pool for fmac"
			default "y"

		menu "softmac/fullmac Configuration"
			choice
				prompt "decide if use fullmac when bootup"
				default PACKAGE_SFUMAC_SMAC

			config PACKAGE_SFUMAC_SMAC
				bool "use softmac"

			config PACKAGE_SFUMAC_FMAC
				bool "use fullmac"
			endchoice
		endmenu
	endif
endef

EXTRA_KCONFIG = \
	CONFIG_SF16A18_WIFI_MAC_HOST_OFFLOAD=m	\
	CONFIG_SF16A18_WIFI_SOFT_MAC=m	\
	CONFIG_SF16A18_WIFI_RF=m

ifndef CONFIG_FLASH_OPTIMIZE
	EXTRA_KCONFIG += CONFIG_SF16A18_WIFI_FULL_MAC=m
endif

ifdef CONFIG_TARGET_siflower_sf16a18_mpw0
SF_ARCH=mpw0
endif
ifdef CONFIG_TARGET_siflower_sf16a18_mpw1
SF_ARCH=mpw1
#for mpw1,rf is not avaiable,so we use fpga macro to setup mac only
EXTRA_KCONFIG += CONFIG_SF16A18_WIFI_MPW1_FPGA=y
endif
ifdef CONFIG_TARGET_siflower_sf19a28_mpw1
SF_ARCH=a28mpw0
endif
ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
SF_ARCH=a28fullmask
endif

SF_UMAC_TYPE?=smac
ifdef CONFIG_PACKAGE_SFUMAC_SMAC
SF_UMAC_TYPE=smac
endif
ifdef CONFIG_PACKAGE_SFUMAC_FMAC
SF_UMAC_TYPE=fmac
endif

ifdef CONFIG_TARGET_siflower_sf16a18_fullmask
SF_ARCH=fullmask
endif

KO_FILE_NAME="src"
ifneq ($(findstring NF,$(PROFILE)),)
KO_FILE_NAME="nf_src"
endif

ifneq ($(findstring AC28S,$(PROFILE)),)
KO_FILE_NAME="src_s"
endif

ifneq ($(findstring NAND,$(PROFILE)),)
KO_FILE_NAME="src_nand"
endif

define KernelPackage/sf_smac/description
 Kernel module to sf16a18 smac && rf.
endef

NOSTDINC_FLAGS = \
	-I$(PKG_BUILD_DIR) \
	-I$(STAGING_DIR)/usr/include/mac80211-backport/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211-backport \
	-I$(STAGING_DIR)/usr/include/mac80211/uapi \
	-I$(STAGING_DIR)/usr/include/mac80211 \
	-include backport/autoconf.h \
	-include backport/backport.h

ifdef CONFIG_PACKAGE_MAC80211_MESH
  NOSTDINC_FLAGS += -DCONFIG_MAC80211_MESH
endif

ifdef CONFIG_PACKAGE_MAC80211_DEBUGFS
  NOSTDINC_FLAGS += -DCONFIG_MAC80211_DEBUGFS
endif

ifndef CONFIG_PACKAGE_SFSMAC_DBGINFO_ALLOCS
EXTRA_KCONFIG += CONFIG_DBG_DUMP=n
endif

ifdef CONFIG_PACKAGE_SFSMAC_LB_LA_ENABLE
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_LA_ENABLE
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_LB_LA_ENABLE
endif

ifdef CONFIG_PACKAGE_SFSMAC_HB_LA_ENABLE
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_LA_ENABLE
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_HB_LA_ENABLE
endif

ifdef CONFIG_PACKAGE_SFUMAC_LB_EXT_PA_SUPPORT
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_LB_EXT_PA_ENABLE
endif

ifdef CONFIG_PACKAGE_SFUMAC_HB_EXT_PA_SUPPORT
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_HB_EXT_PA_ENABLE
endif

ifdef CONFIG_SFFMAC_ENABLE_TOKEN
EXTRA_KCONFIG += CONFIG_SFFMAC_ENABLE_TOKEN=y
NOSTDINC_FLAGS += -DCONFIG_SFFMAC_ENABLE_TOKEN
endif

ifdef CONFIG_SFFMAC_ENABLE_RX_MEM_POOL
EXTRA_KCONFIG += CONFIG_SFFMAC_ENABLE_RX_MEM_POOL=y
NOSTDINC_FLAGS += -DCONFIG_SFFMAC_ENABLE_RX_MEM_POOL
endif

ifdef CONFIG_PACKAGE_SFUMAC_WIFI_ATE_TOOLS
EXTRA_KCONFIG += CONFIG_SF16A18_WIFI_ATE_TOOLS=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_ATE_TOOLS
endif

ifdef CONFIG_PACKAGE_kmod-sf_hnat
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HNAT_DRIVER
endif

NOSTDINC_FLAGS += -DCONFIG_SF16A18_WIFI_RF

ifeq ($(SF_ARCH), fullmask)
#use m_SFDSP as default
EXTRA_KCONFIG += CONFIG_SFA18_FULLMASK=y
CONFIG_SF16A18_LMAC_USE_M_SFDSP=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_LMAC_USE_M_SFDSP
else
ifeq ($(SF_ARCH), a28mpw0)
CONFIG_SF16A18_LMAC_USE_M_SFDSP=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_LMAC_USE_M_SFDSP
NOSTDINC_FLAGS += -DCONFIG_SFA28_MPW0
EXTRA_KCONFIG += CONFIG_SFA28_MPW0=y
EXTRA_KCONFIG += CONFIG_SFA28=y
else
ifeq ($(SF_ARCH), a28fullmask)
CONFIG_SF16A18_LMAC_USE_M_SFDSP=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_LMAC_USE_M_SFDSP
NOSTDINC_FLAGS += -DCONFIG_SFA28_FULLMASK
EXTRA_KCONFIG += CONFIG_SFA28_FULLMASK=y
else
#if use interaptiv,you must set CONFIG_NR_CPUS to 2
CONFIG_SF16A18_LMAC_USE_INTERPTIV=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_LMAC_USE_INTERPTIV
endif
endif
endif

#if driver is final release
#EXTRA_KCONFIG += CONFIG_SF16A18_RELEASE=y
ifdef CONFIG_FLASH_OPTIMIZE
EXTRA_KCONFIG += CONFIG_SIWIFI_TINY_DRIVER=y
EXTRA_KCONFIG += CONFIG_SF16A18_STARTCORE_USE_PIC=y
endif
ifdef CONFIG_MEMORY_OPTIMIZE
#use 32 tx queue count to balance the memory and the speed
#EXTRA_KCONFIG += CONFIG_SF16A18_TXQ_MAX_CNT=32
EXTRA_KCONFIG += CONFIG_WIFI_LITE_MEMORY=y
endif
ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-A28-HWA
#CONFIG_DUAL_ANTENNA_CALIBRATE=y
endif
ifdef CONFIG_DUAL_ANTENNA_CALIBRATE
EXTRA_KCONFIG += CONFIG_DUAL_ANTENNA_CALIBRATE=y
endif
ifdef CONFIG_POWER_OPTIMIZE
#that must be sync with lmac binaries if defined,here defined 8 to save power
EXTRA_KCONFIG += CONFIG_SF16A18_COOLING_TEMP=y
endif
ifdef CONFIG_SF16A18_COOLING_TEMP
EXTRA_KCONFIG += CONFIG_SF16A18_COOLING_TEMP=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_COOLING_TEMP
endif
ifeq ($(CONFIG_NEW_LEDS),y)
EXTRA_KCONFIG += CONFIG_SF16A18_WIFI_LED=y
EXTRA_KCONFIG += CONFIG_SF19A28_WIFI_LED=y
endif
#EXTRA_KCONFIG += CONFIG_SIWIFI_IGMP=y
#CONFIG_SIWIFI_TEMPERATURE_CONTROL=y
ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
EXTRA_KCONFIG += CONFIG_SIWIFI_TEMPERATURE_CONTROL=y
endif

ifdef CONFIG_SFAX8_FACTORY_READ
NOSTDINC_FLAGS += -DCONFIG_SFAX8_FACTORY_READ
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./$(KO_FILE_NAME)/* $(PKG_BUILD_DIR)/
	$(CP) -r ./config $(PKG_BUILD_DIR)/
	$(CP) -r ./test $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

define KernelPackage/sf_smac/install
	$(INSTALL_DIR) $(1)/lib/firmware
	cp \
		$(PKG_BUILD_DIR)/config/siwifi_aetnensis.ini \
		$(PKG_BUILD_DIR)/config/siwifi_settings.ini \
		$(PKG_BUILD_DIR)/config/tx_adjust_gain_table.bin \
		$(PKG_BUILD_DIR)/config/$(SF_ARCH)/agcram.bin \
		$(PKG_BUILD_DIR)/config/$(SF_ARCH)/agcram_24g.bin \
		$(PKG_BUILD_DIR)/config/$(SF_ARCH)/ldpcram.bin \
		$(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_pmem.bin \
		$(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_default_reg.bin \
		$(1)/lib/firmware

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-A28-HWA
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf_rf_expa_config_hwa.ini $(1)/lib/firmware/sf_rf_expa_config.ini
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf_rf_expa_config.ini $(1)/lib/firmware/sf_rf_expa_config.ini
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/dig_gaintable.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/dig_gaintable_expa.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_hb_txpower_table.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_lb_txpower_table.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_hb_txpower_table_expa.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_lb_txpower_table_expa.ini $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/cali_table/default_txpower_calibrate_table.bin $(1)/lib/firmware

#ifndef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-REP
	cp $(PKG_BUILD_DIR)/config/cali_table/default_txpower_calibrate_expa_table.bin $(1)/lib/firmware
#endif

	cp $(PKG_BUILD_DIR)/config/cali_table/txp_offset_sleepmode_low.ini $(1)/lib/firmware
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/config/cali_table/txpower_calibrate_table.sh $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/config/cali_table/txpower_calibrate_table.txt $(1)/usr/bin
endif
#ifdef CONFIG_TARGET_siflower_sf19a28_mpw1
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_trx_path.ini $(1)/lib/firmware
#endif

ifeq ($(SF_ARCH), fullmask)
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_xdma_reg.bin $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/sfwifi.sh $(1)/lib/sfwifi.sh
else
	cp $(PKG_BUILD_DIR)/config/sfwifi-a28.sh $(1)/lib/sfwifi.sh
endif
	$(INSTALL_DIR) $(1)/bin
	cp $(PKG_BUILD_DIR)/config/sfwifi $(1)/bin/
	cp $(PKG_BUILD_DIR)/config/rfcmd_err $(1)/bin/
	cp $(PKG_BUILD_DIR)/config/reload_wifi.sh $(1)/bin/
ifdef CONFIG_PACKAGE_SFUMAC_FMAC
ifdef CONFIG_MEMORY_OPTIMIZE
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/memory-opt/sf1688_lb_fmac.bin $(1)/lib/firmware/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/memory-opt/sf1688_hb_fmac.bin $(1)/lib/firmware/
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_lb_fmac.bin $(1)/lib/firmware/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_hb_fmac.bin $(1)/lib/firmware/
endif
else
ifdef CONFIG_FLASH_OPTIMIZE
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/flash-opt/sf1688_smac.bin $(1)/lib/firmware/
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_lb_smac.bin $(1)/lib/firmware/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_hb_smac.bin $(1)/lib/firmware/
endif
endif



	$(INSTALL_DIR) $(1)/lib/preinit
	cp $(PKG_BUILD_DIR)/config/99_rf_misc $(1)/lib/preinit/

ifdef CONFIG_PACKAGE_SFUMAC_FMAC
	echo 'boot_hook_add preinit_main insmod_cfg80211' >> $(1)/lib/preinit/99_rf_misc
endif
ifdef CONFIG_PACKAGE_SFUMAC_SMAC
	echo 'boot_hook_add preinit_main insmod_mac80211' >> $(1)/lib/preinit/99_rf_misc
endif
ifndef CONFIG_PACKAGE_SFSMAC_HB_LA_ENABLE
	echo 'boot_hook_add preinit_main insmod_$(SF_UMAC_TYPE)_lb' >> $(1)/lib/preinit/99_rf_misc
endif
ifndef CONFIG_PACKAGE_SFSMAC_LB_LA_ENABLE
	echo 'boot_hook_add preinit_main insmod_$(SF_UMAC_TYPE)_hb' >> $(1)/lib/preinit/99_rf_misc
endif
ifndef CONFIG_FLASH_OPTIMIZE
	$(INSTALL_DIR) $(1)/sbin
	cp $(PKG_BUILD_DIR)/config/rf_recali.sh $(1)/sbin/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_check/* $(1)/sbin/
	$(INSTALL_DIR) $(1)/bin
endif

ifdef CONFIG_PACKAGE_SFUMAC_WIFI_TEST_SCRIPTS
	cp -r $(PKG_BUILD_DIR)/test/. $(1)/bin/
	cp -r $(PKG_BUILD_DIR)/test/rf_test/. $(1)/bin/
endif

ifndef CONFIG_PACKAGE_siwifitools
	cp $(PKG_BUILD_DIR)/config/siwifi_umh_nothing.sh  $(1)/bin/siwifi_umh.sh
endif

endef

$(eval $(call KernelPackage,sf_smac))
