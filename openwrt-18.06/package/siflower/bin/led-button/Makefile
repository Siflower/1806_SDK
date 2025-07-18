#
# Copyright @ 2024 Siflower
#
# sifiower Software.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=led-button
PKG_VERSION:=1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/led-button
SECTION:=utils
CATEGORY:=Utilities
TITLE:=led and button centralized dispatcher.
MAINTAINER:=Nevermore <nevermore.wang@siflower.com.cn>
endef

define Package/led-button/description
SiFlower SF16A18 led and button control.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -Wall -Werror -mips32r2" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/led-button/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/share
	$(INSTALL_DIR) $(1)/usr/share/led-button
	$(INSTALL_CONF) ./files/led    $(1)/etc/config/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/led-button $(1)/bin/
	$(INSTALL_BIN) ./files/check_btn_cfg $(1)/bin/
	$(INSTALL_BIN) ./files/led_button_init  $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/wps.sh  $(1)/usr/share/led-button/
	$(INSTALL_BIN) ./files/wps_func.sh  $(1)/usr/share/led-button/
	$(INSTALL_BIN) ./files/wps_config.sh  $(1)/usr/share/led-button/
	$(INSTALL_BIN) ./files/wifi_ctrl.sh  $(1)/usr/share/led-button/
	$(INSTALL_BIN) ./files/button_test.sh  $(1)/usr/share/led-button/
endef


$(eval $(call BuildPackage,led-button))
