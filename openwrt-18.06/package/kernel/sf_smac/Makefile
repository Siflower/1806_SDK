include $(TOPDIR)/rules.mk

PKG_NAME:=sf_smac
PKG_RELEASE=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_smac
  SUBMENU:=Wireless Drivers
  TITLE:=SIFLOWER sf19a28 wireless driver
  DEPENDS:=+kmod-sfax8-factory-read +kmod-cfg80211 +@DRIVER_11N_SUPPORT +@DRIVER_11AC_SUPPORT
  FILES:= \
	$(PKG_BUILD_DIR)/rf/sf16a18_rf.ko \
	$(PKG_BUILD_DIR)/startcore/startcore.ko \
	$(PKG_BUILD_DIR)/fmac/sf16a18_fmac.ko
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
			default "y"

		config SFFMAC_ENABLE_RX_MEM_POOL
			bool "enable rx memery pool for fmac"
			default "n"

		config LED_MULTICOLOUR
			bool "enable multicolour led control"
			default "n"

		config LED_PIN_ACTIVE_LOW
			bool "enable low active led gpio control"
			default "n"
	endif
endef

EXTRA_KCONFIG = \
	CONFIG_SF16A18_WIFI_MAC_HOST_OFFLOAD=m	\
	CONFIG_SF16A18_WIFI_RF=m \
	CONFIG_SF16A18_WIFI_FULL_MAC=m

SF_ARCH=a28fullmask
SF_UMAC_TYPE=fmac

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

ifeq ($(SF_ARCH), a28fullmask)
CONFIG_SF16A18_LMAC_USE_M_SFDSP=y
NOSTDINC_FLAGS += -DCONFIG_SF16A18_LMAC_USE_M_SFDSP
NOSTDINC_FLAGS += -DCONFIG_SFA28_FULLMASK
EXTRA_KCONFIG += CONFIG_SFA28_FULLMASK=y
endif

#if driver is final release
#EXTRA_KCONFIG += CONFIG_SF16A18_RELEASE=y

ifdef CONFIG_MEMORY_OPTIMIZE
#use 32 tx queue count to balance the memory and the speed
#EXTRA_KCONFIG += CONFIG_SF16A18_TXQ_MAX_CNT=32
EXTRA_KCONFIG += CONFIG_WIFI_LITE_MEMORY=y
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-A28-HWA
#CONFIG_DUAL_ANTENNA_CALIBRATE=y
endif
CONFIG_DUAL_ANTENNA_CALIBRATE=y
ifdef CONFIG_DUAL_ANTENNA_CALIBRATE
EXTRA_KCONFIG += CONFIG_DUAL_ANTENNA_CALIBRATE=y
endif
ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-AC28CSD
CONFIG_DUAL_ANTENNA_CALIBRATE_CSD=y
else
CONFIG_UPDATE_GAIN_TABLE=y
endif
ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-AC28S-CSD
CONFIG_DUAL_ANTENNA_CALIBRATE_CSD=y
else
CONFIG_UPDATE_GAIN_TABLE=y
endif
ifdef CONFIG_DUAL_ANTENNA_CALIBRATE_CSD
EXTRA_KCONFIG += CONFIG_DUAL_ANTENNA_CALIBRATE_CSD=y
endif
ifdef CONFIG_UPDATE_GAIN_TABLE
EXTRA_KCONFIG += CONFIG_UPDATE_GAIN_TABLE=y
NOSTDINC_FLAGS += -DCONFIG_UPDATE_GAIN_TABLE
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
EXTRA_KCONFIG += CONFIG_SIWIFI_REPEATER=y
#EXTRA_KCONFIG += CONFIG_SIWIFI_REPEATER_IPV6=y
EXTRA_KCONFIG += CONFIG_SIWIFI_ACS_INTERNAL=y
EXTRA_KCONFIG += CONFIG_SIWIFI_IGMP=y
#CONFIG_SIWIFI_TEMPERATURE_CONTROL=y
ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
EXTRA_KCONFIG += CONFIG_SIWIFI_TEMPERATURE_CONTROL=y
endif

ifdef CONFIG_SFAX8_FACTORY_READ
NOSTDINC_FLAGS += -DCONFIG_SFAX8_FACTORY_READ
endif

ifdef CONFIG_LED_MULTICOLOUR
EXTRA_KCONFIG += CONFIG_LED_MULTICOLOUR=y
NOSTDINC_FLAGS += -DCONFIG_LED_MULTICOLOUR
endif

ifdef CONFIG_LED_PIN_ACTIVE_LOW
EXTRA_KCONFIG += CONFIG_LED_PIN_ACTIVE_LOW=y
NOSTDINC_FLAGS += -DCONFIG_LED_PIN_ACTIVE_LOW
endif

SFWIFI_COMPILE_TIME=$(shell date +%s)
NOSTDINC_FLAGS += -DCONFIG_COMPILE_TIME=$(SFWIFI_COMPILE_TIME)

NOSTDINC_FLAGS += \
	-I$(PKG_BUILD_DIR)  \
    -Werror

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) -r ./config $(PKG_BUILD_DIR)/
	$(CP) -r ./test $(PKG_BUILD_DIR)/
	$(CP) ./src/bb_src/ $(PKG_BUILD_DIR)/fmac/ -rf
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/fmac/bb_src/umac/fullmac/
	$(CP) ../siflower_include/sfax8_factory_read.h $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/fmac/bb_src/umac/fullmac/
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		$(EXTRA_KCONFIG)  \
		modules
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
		$(1)/lib/firmware

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-A28-HWA
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/1.10.003T/rf_pmem.bin $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/1.10.003T/rf_default_reg.bin $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf_rf_expa_config_hwa.ini $(1)/lib/firmware/sf_rf_expa_config.ini
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_pmem.bin $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_default_reg.bin $(1)/lib/firmware
ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-YTS
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf_rf_expa_config_ac10.ini $(1)/lib/firmware/sf_rf_expa_config.ini
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf_rf_expa_config.ini $(1)/lib/firmware/sf_rf_expa_config.ini
endif
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/dig_gaintable.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/dig_gaintable_expa.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_hb_txpower_table.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_lb_txpower_table.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_hb_txpower_table_expa.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_hb_txpower_table_expa_low_power.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/default_lb_txpower_table_expa.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_gain_table.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/temperature_lb_control.ini $(1)/lib/firmware
	cp  $(PKG_BUILD_DIR)/config/$(SF_ARCH)/temperature_hb_control.ini $(1)/lib/firmware
	cp $(PKG_BUILD_DIR)/config/cali_table/default_txpower_calibrate_table.bin $(1)/lib/firmware

#ifndef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-REP
	cp $(PKG_BUILD_DIR)/config/cali_table/default_txpower_calibrate_expa_table.bin $(1)/lib/firmware
#endif

	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/rf_trx_path.ini $(1)/lib/firmware

ifdef CONFIG_SIWIFI_TEMPERATURE_CONTROL
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/temperature_control.ini $(1)/lib/firmware
endif
	cp $(PKG_BUILD_DIR)/config/cali_table/txp_offset_sleepmode_low.ini $(1)/lib/firmware
ifdef CONFIG_DUAL_ANTENNA_CALIBRATE
	cp $(PKG_BUILD_DIR)/config/cali_table/txp_offset_sleepmode_low_second.ini $(1)/lib/firmware
endif
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/config/cali_table/txpower_calibrate_table.sh $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/config/cali_table/txpower_calibrate_table.txt $(1)/usr/bin
endif
	cp $(PKG_BUILD_DIR)/config/sfwifi-a28.sh $(1)/lib/sfwifi.sh
	$(INSTALL_DIR) $(1)/bin
	cp $(PKG_BUILD_DIR)/config/sfwifi $(1)/bin/
	cp $(PKG_BUILD_DIR)/config/rfcmd_err $(1)/bin/
	cp $(PKG_BUILD_DIR)/config/reload_wifi.sh $(1)/bin/
ifdef CONFIG_MEMORY_OPTIMIZE
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/memory-opt/sf1688_lb_fmac.bin $(1)/lib/firmware/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/memory-opt/sf1688_hb_fmac.bin $(1)/lib/firmware/
else
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_lb_fmac.bin $(1)/lib/firmware/
	cp $(PKG_BUILD_DIR)/config/$(SF_ARCH)/sf1688_hb_fmac.bin $(1)/lib/firmware/
endif


	$(INSTALL_DIR) $(1)/lib/preinit
	cp $(PKG_BUILD_DIR)/config/99_rf_misc $(1)/lib/preinit/

	echo 'boot_hook_add preinit_main insmod_cfg80211' >> $(1)/lib/preinit/99_rf_misc
	echo 'boot_hook_add preinit_main insmod_fmac' >> $(1)/lib/preinit/99_rf_misc

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
