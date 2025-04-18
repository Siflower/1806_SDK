include $(TOPDIR)/rules.mk

PKG_NAME:=sf_eswitch
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_eswitch
  SUBMENU:=Network Devices
  TITLE:=Siflower SFAX8 extern gigabit switch support
  FILES:=$(PKG_BUILD_DIR)/sf_eswitch.ko
  DEPENDS+=+kmod-sf_gmac
  AUTOLOAD:=$(call AutoLoad,55,sf_eswitch,1)
  KCONFIG:=
endef

define KernelPackage/sf_eswitch/config
	if PACKAGE_kmod-sf_eswitch
		config SFAX8_ESWITCH_LIGHT
			bool "use light eswitch"
			default "n"
	endif
endef


EXTRA_KCONFIG:= \
	CONFIG_SFAX8_ESWITCH_DRIVER=m \

ifdef CONFIG_TARGET_siflower_sf19a28_v2
NOSTDINC_FLAGS += -DCONFIG_SF19A28_V2
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
NOSTDINC_FLAGS += -DCONFIG_SF19A28_FULLMASK
endif

ifdef CONFIG_PACKAGE_kmod-sf_netlink
NOSTDINC_FLAGS += -DCONFIG_SFAX8_GENL
endif

ifdef CONFIG_SFAX8_ESWITCH_LIGHT
EXTRA_KCONFIG += CONFIG_SFAX8_ESWITCH_LIGHT=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_ESWITCH_LIGHT
endif

NOSTDINC_FLAGS += -I$(PKG_BUILD_DIR)/an8855_src/api/inc \
				  -I$(PKG_BUILD_DIR)/an8855_src/core

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
	$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		$(EXTRA_KCONFIG)  \
		modules
endef

$(eval $(call KernelPackage,sf_eswitch))
