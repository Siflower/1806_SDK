#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sfax8-factory-read
PKG_RELEASE:=0
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/sfax8-factory-read
  SUBMENU:=Other modules
  TITLE:=Siflower factory read driver
  FILES:=$(PKG_BUILD_DIR)/sfax8_factory_read.ko
  AUTOLOAD:=$(call AutoLoad,01,sfax8_factory_read,1)
  KCONFIG:=
endef

define KernelPackage/sfax8-factory-read/description
  Kernel module to sfax8-factory-read.
endef

define KernelPackage/sfax8-factory-read/config
	if PACKAGE_kmod-sfax8-factory-read
		config SFAX8_FACTORY_READ
			bool "set factory read"
			default "y"
	endif
endef

SFAX8_KO_NAME="sfax8_factory_read.ko"
ifneq ($(findstring NF,$(PROFILE)),)
SFAX8_KO_NAME="nf_sfax8_factory_read.ko"
endif

ifneq ($(findstring AC28S,$(PROFILE)),)
SFAX8_KO_NAME="sfax8_factory_read_s.ko"
endif

ifneq ($(findstring NAND,$(PROFILE)),)
SFAX8_KO_NAME="sfax8_factory_read_nand.ko"
endif

EXTRA_KCONFIG:= \
	CONFIG_SFAX8_FACTORY_READ=m

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG)))) \

NOSTDINC_FLAGS = \
	-I$(PKG_BUILD_DIR)

MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Prepare
	$(CP) ./src/sfax8_factory_read.ko $(PKG_BUILD_DIR)/
endef

define Build/Compile
$(CP) ./src/$(SFAX8_KO_NAME) $(PKG_BUILD_DIR)/sfax8_factory_read.ko
endef

$(eval $(call KernelPackage,sfax8-factory-read))
