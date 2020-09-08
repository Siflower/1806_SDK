#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sf_gswitch
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_gswitch
  SUBMENU:=Network Devices
  TITLE:=Siflower SFAX8 gigabit switch support
  DEPENDS:=+kmod-sf_gmac \
			+kmod-sf_qos
  FILES:=$(PKG_BUILD_DIR)/gswitch.ko
  AUTOLOAD:=$(call AutoLoad,61,gswitch,1)
  KCONFIG:=
endef

EXTRA_KCONFIG:= \
	CONFIG_NET_VENDOR_SIFLOWER=y \
	CONFIG_SFAX8_GSWITCH=m \

ifdef CONFIG_TARGET_siflower_sf19a28_mpw1
NOSTDINC_FLAGS += -DCONFIG_SF19A28_MPW1
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

$(eval $(call KernelPackage,sf_gswitch))
