include $(TOPDIR)/rules.mk

PKG_NAME:=sf_hnat
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk


define KernelPackage/sf_hnat
  SUBMENU:=Network Devices
  TITLE:=Siflower SFA28 hnat support
  FILES:=$(PKG_BUILD_DIR)/sfhnat.ko
  AUTOLOAD:=$(call AutoLoad,54,sfhnat,1)
  KCONFIG:=
endef

define KernelPackage/sf_hnat/config
	if PACKAGE_kmod-sf_hnat
		config SFAX8_HNAT_TEST_TOOL
			bool "set test tool"
			default "n"
		config SFAX8_HNAT_MULTI_WAN
			bool "support hnat multi wan"
			default "n"
	endif
endef

EXTRA_KCONFIG:= \
	CONFIG_SFAX8_HNAT_DRIVER=m \

ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
EXTRA_KCONFIG += CONFIG_SFAX8_HNAT_TEST_TOOL=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HNAT_TEST_TOOL
endif

ifdef CONFIG_SFAX8_HNAT_MULTI_WAN
EXTRA_KCONFIG += CONFIG_SFAX8_HNAT_MULTI_WAN=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HNAT_MULTI_WAN
endif

ifdef CONFIG_TARGET_siflower_sf19a28_v2
NOSTDINC_FLAGS += -DCONFIG_SF19A28_V2
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
NOSTDINC_FLAGS += -DCONFIG_SF19A28_FULLMASK
endif


define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		$(EXTRA_KCONFIG)  \
		modules
endef

define KernelPackage/sf_hnat/install
	$(INSTALL_DIR) $(1)/sbin/
	$(CP) ./file/hnat_onoff.sh $(1)/sbin/
ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
	$(INSTALL_DIR) $(1)/etc/pkt
	$(CP) -r ./file/test_tool_pkt/* $(1)/etc/pkt/
endif
endef

$(eval $(call KernelPackage,sf_hnat))
