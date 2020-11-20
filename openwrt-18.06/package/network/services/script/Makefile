
include $(TOPDIR)/rules.mk

PKG_NAME:=scripts
PKG_VERSION:=1
PKG_RELEASE:=2
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define Package/scripts
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Lua and shell scripts
  DEPENDS:= +libuci
endef

define Package/scripts/description
 Lua and shell scripts demo for different functions
endef

define Build/Compile
endef

define Build/Configure
endef

define Package/scripts/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./scripts/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,scripts))
