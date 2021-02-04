
include $(TOPDIR)/rules.mk

PKG_NAME:=auto_adapt
PKG_VERSION:=1
PKG_RELEASE:=2
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define Package/auto_adapt
  SECTION:=net
  CATEGORY:=Network
  TITLE:=auto_adapt demo
  DEPENDS:= +libuci
endef

define Package/auto_adapt/description
 WAN/LAN auto adapt demo for ac22
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/auto_adapt/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) ./files/send $(1)/bin
	$(INSTALL_BIN) ./files/auto_adapt.sh $(1)/bin
	$(INSTALL_DIR) $(1)/usr/lib/lua/luci/view/admin_network
	$(INSTALL_DATA) ./files/auto_adapt.htm $(1)/usr/lib/lua/luci/view/admin_network
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) ./files/rc.local $(1)/etc
endef

$(eval $(call BuildPackage,auto_adapt))
