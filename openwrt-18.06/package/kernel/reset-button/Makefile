#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=reset-button
PKG_RELEASE:=3
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/reset-button
  SUBMENU:=Other modules
  TITLE:=Reset Button top driver
  DEPENDS:=+kmod-button-hotplug +kernel
  FILES:=$(PKG_BUILD_DIR)/sfax8-reset-button-top.ko
  AUTOLOAD:=$(call AutoLoad,31,sfax8-reset-button-top,1)
  KCONFIG:=
endef

define KernelPackage/reset-button/description
  Kernel module to reset button.
endef

define KernelPackage/reset-button/config
	if PACKAGE_kmod-reset-button
		config PACKAGE_KERNEL_BUTTON_WPS
		bool "enable wps connect button"
		default "n"
	endif
endef

EXTRA_KCONFIG:= \
	CONFIG_SIFLOWER_RESET_BUTTON_TOP=m

ifdef CONFIG_PACKAGE_KERNEL_BUTTON_WPS
EXTRA_KCONFIG += CONFIG_SF_NETWORK_WPS=y
endif

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,reset-button))
