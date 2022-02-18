#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sf_netlink
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_netlink
  SUBMENU:=Network Devices
  TITLE:=Siflower SFAX8 netlink support
  FILES:=$(PKG_BUILD_DIR)/sfax8_netlink.ko
  AUTOLOAD:=$(call AutoLoad,50,sfax8_netlink,1)
  KCONFIG:=
endef

EXTRA_KCONFIG:= \
	CONFIG_SF_NETLINK=m \


ifdef CONFIG_SEND_ERR
NOSTDINC_FLAGS += -DCONFIG_SEND_ERR
EXTRA_KCONFIG += CONFIG_SEND_ERR=y
endif

SFAX8_KO_NAME="sfax8_netlink.ko"
ifneq ($(findstring NF,$(PROFILE)),)
SFAX8_KO_NAME="nf_sfax8_netlink.ko"
endif

ifneq ($(findstring AC28S,$(PROFILE)),)
SFAX8_KO_NAME="sfax8_netlink_s.ko"
endif

ifneq ($(findstring NAND,$(PROFILE)),)
SFAX8_KO_NAME="sfax8_netlink_nand.ko"
endif

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Compile
$(CP) ./$(SFAX8_KO_NAME) $(PKG_BUILD_DIR)/sfax8_netlink.ko
endef

$(eval $(call KernelPackage,sf_netlink))
