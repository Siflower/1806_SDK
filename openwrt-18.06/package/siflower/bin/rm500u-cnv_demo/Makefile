#
# Copyright (C) 2018 siflower.com.cn
#
include $(TOPDIR)/rules.mk

PKG_NAME:=rm500u-cnv-demo
PKG_VERSION:=1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
SECTION:=utils
CATEGORY:=Utilities
TITLE:=Quectel RM500U-CNV dial-up demo (USB/PCIe)
MAINTAINER:=siflower <mark.cai@siflower.com.cn>
DEPENDS:=+kmod-usb-net-rndis
endef

define Package/$(PKG_NAME)/description
USB/PCIe 5G modem auto dial-up shell script for RM500U-CNV.
Usage: /bin/auto_dial_up_networking [usb|pcie]
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) ./auto_dial_up_networking.sh $(1)/bin/auto_dial_up_networking
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
