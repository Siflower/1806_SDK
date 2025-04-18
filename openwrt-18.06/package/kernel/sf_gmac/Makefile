include $(TOPDIR)/rules.mk

PKG_NAME:=sf_gmac
PKG_RELEASE:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/sf_gmac/config
	if PACKAGE_kmod-sf_gmac

		config SFAX8_RGMII_GMAC
			bool "select RGMII mode gmac"
			default "y"

		config SFAX8_RMII_GMAC
			bool "select RMII mode gmac"
			default "n"

		config SFAX8_FPGA_GMAC
			bool "select FPGA mode gmac"
			default "n"

		config SFAX8_HNAT_TEST_TOOL
			bool "set test tool"
			depends on PACKAGE_kmod-sf_hnat
			default "n"

		config SFAX8_IFG_ENABLE
			bool "gmac/eswitch IFG setting enable"
			default "n"

		config SFAX8_HW_RESET
			bool "eswitch hardware reset"
			depends on PACKAGE_kmod-sf_eswitch
			default "n"

		config SFAX8_GMAC_DELAY_AUTOCALI
			bool "gmac delay auto calibration"
			depends on PACKAGE_kmod-sf_eswitch
			default "n"

		config SFAX8_GMAC_TCLKCHOOSE
			bool "choose tclk to use for gmac."
			default "n"

		config SFAX8_PTP
			bool "ptp function enable"
			default "n"

	endif

endef

EXTRA_KCONFIG:= \
	CONFIG_NET_VENDOR_SIFLOWER=y \
	CONFIG_SFAX8_GMAC=m \


ifdef CONFIG_TARGET_siflower_sf16a18_fullmask
NOSTDINC_FLAGS += -DCONFIG_SF16A18_FULL_MASK
endif

ifdef CONFIG_TARGET_siflower_sf19a28_v2
NOSTDINC_FLAGS += -DCONFIG_SF19A28_V2
endif

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask
NOSTDINC_FLAGS += -DCONFIG_SF19A28_FULLMASK
endif

ifdef CONFIG_SFAX8_RGMII_GMAC
NOSTDINC_FLAGS += -DCONFIG_SFAX8_RGMII_GMAC
endif

ifdef CONFIG_SFAX8_HW_RESET
EXTRA_KCONFIG += CONFIG_SFAX8_HW_RESET=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HW_RESET
endif

ifdef CONFIG_PACKAGE_kmod-sf_hnat
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HNAT_DRIVER
ifdef CONFIG_SFAX8_HNAT_TEST_TOOL
NOSTDINC_FLAGS += -DCONFIG_SFAX8_HNAT_TEST_TOOL
endif
endif

ifdef CONFIG_PACKAGE_kmod-sfax8-factory-read
NOSTDINC_FLAGS += -DCONFIG_SFAX8_FACTORY_READ
endif

ifdef CONFIG_PACKAGE_kmod-sf_eswitch
EXTRA_KCONFIG += CONFIG_SFAX8_ESWITCH_DRIVER=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_ESWITCH_DRIVER
endif

ifdef CONFIG_SFAX8_IFG_ENABLE
EXTRA_KCONFIG += CONFIG_SFAX8_IFG_ENABLE=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_IFG_ENABLE
endif

ifdef CONFIG_MEMORY_OPTIMIZE
NOSTDINC_FLAGS += -DCONFIG_MEMORY_OPTIMIZE
endif

ifdef CONFIG_SFAX8_GMAC_DELAY_AUTOCALI
EXTRA_KCONFIG += CONFIG_SFAX8_GMAC_DELAY_AUTOCALI=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_GMAC_DELAY_AUTOCALI
endif

ifdef CONFIG_SFAX8_PTP
NOSTDINC_FLAGS += -DCONFIG_SFAX8_PTP
endif

#ifdef CONFIG_SFAX8_GMAC_TCLKCHOOSE
EXTRA_KCONFIG += CONFIG_SFAX8_GMAC_TCLKCHOOSE=y
NOSTDINC_FLAGS += -DCONFIG_SFAX8_GMAC_TCLKCHOOSE
#endif


define KernelPackage/sf_gmac
  SUBMENU:=Network Devices
  TITLE:=Siflower SFA18 gmac support
  DEPENDS:=+kmod-sfax8-factory-read +SFAX8_PTP:kmod-ptp
  FILES:=$(PKG_BUILD_DIR)/sgmac.ko
  AUTOLOAD:=$(call AutoLoad,60,sgmac,1)
  KCONFIG:=
endef


ifdef CONFIG_SFAX8_RMII_GMAC
NOSTDINC_FLAGS += -DCONFIG_SFAX8_RMII_GMAC
endif

ifdef CONFIG_SFAX8_FPGA_GMAC
NOSTDINC_FLAGS += -DCONFIG_SFAX8_FPGA_GMAC
endif

NOSTDINC_FLAGS += \
	-I$(PKG_BUILD_DIR)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(CP) ../siflower_include/* $(PKG_BUILD_DIR)/
	+$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		$(EXTRA_KCONFIG)  \
		modules
endef

$(eval $(call KernelPackage,sf_gmac))
