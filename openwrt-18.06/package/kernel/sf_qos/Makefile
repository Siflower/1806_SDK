include $(TOPDIR)/rules.mk

PKG_NAME:=sf_qos
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk


define KernelPackage/sf_qos
  SUBMENU:=Network Devices
  TITLE:=Siflower Switch qos support
  FILES:=$(PKG_BUILD_DIR)/sfqos.ko
  AUTOLOAD:=$(call AutoLoad,54,sfqos,1)
  KCONFIG:=
endef


EXTRA_KCONFIG:= \
	CONFIG_NET_VENDOR_SIFLOWER=y \
	CONFIG_SFAX8_QOS=m \

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./* $(PKG_BUILD_DIR)/
endef

define Build/Compile
endef

$(eval $(call KernelPackage,sf_qos))
