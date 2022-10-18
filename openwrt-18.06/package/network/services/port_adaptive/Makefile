
include $(TOPDIR)/rules.mk

PKG_NAME:=port_adaptive
PKG_VERSION:=1
PKG_RELEASE:=2
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define Package/port_adaptive
  SECTION:=net
  CATEGORY:=Network
  TITLE:=wan/lan auto adapt demo
  DEPENDS:= +libuci
endef

define Package/port_adaptive/description
 WAN/LAN auto adapt demo
endef

define Build/Compile
endef

define Build/Configure
endef

define Package/port_adaptive/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) ./files/send $(1)/bin
	$(INSTALL_BIN) ./files/auto_adapt.sh $(1)/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/port_adaptive $(1)/etc/init.d
endef

$(eval $(call BuildPackage,port_adaptive))
