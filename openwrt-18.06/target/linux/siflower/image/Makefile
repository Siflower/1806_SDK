#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
ifeq ($(CONFIG_NAND_SUPPORT), y)
include ./nand.mk
endif

ifeq ($(SUBTARGET),sf23a2892-mpw0)
define Device/Default
  PROFILES = Default $$(DEVICE_NAME)
  KERNEL_INITRAMFS = kernel-bin | lzma | fit lzma $$(KDIR)/image-$$(firstword $$(DEVICE_DTS)).dtb
  KERNEL = kernel-bin | lzma
  FILESYSTEMS := squashfs
  DEVICE_DTS_DIR := $(DTS_DIR)
  IMAGES := sysupgrade.bin
  IMAGE/sysupgrade.bin = append-kernel | fit lzma $$(KDIR)/image-$$(firstword $$(DEVICE_DTS)).dtb | append-rootfs | pad-rootfs | append-metadata
endef

include $(SUBTARGET).mk

define Image/Build
	$(call Image/Build/$(1),$(1))
endef
else
define get_kernel_entry
0x$(shell $(TARGET_CROSS)nm $(1) 2>/dev/null | grep " kernel_entry" | cut -f1 -d ' ')
endef

SIGN := $(CURDIR)/sign_image
export SIGN
ifeq ($(CONFIG_TARGET_SIGNATURE_IMAGE), y)
	BOOT_FLAG := SECURITY_BOOT
else
	BOOT_FLAG :=
endif

#  $(STAGING_DIR_HOST)/bin/lzma e $(1) -lc1 -lp2 -pb2 $(2)
define CompressLzma
  lzma e $(1) -lc1 -lp2 -pb2 $(2)
endef

define CompressGzip
	gzip -9 -c $(1) > $(2)
endef

define MkuImage
	if [ "${BOOT_FLAG}" = "SECURITY_BOOT" ]; then \
		sh $(SIGN)/sign.sh $1 $(call get_kernel_entry,$(KDIR)/vmlinux.debug) $(3) $(4);	\
	else	\
		mkimage -A mips -O linux -T kernel -a 0x80100000 -C $(1) $(2) \
			-e $(call get_kernel_entry,$(KDIR)/vmlinux.debug) -n 'MIPS OpenWrt Linux-$(LINUX_VERSION)' \
			-d $(3) $(4); \
	fi
endef

define Image/Prepare
	cp $(KDIR)/vmlinux.elf $(BIN_DIR)/$(IMG_PREFIX)-vmlinux.elf
	$(call CompressLzma,$(KDIR)/vmlinux,$(KDIR)/vmlinux.bin.lzma)
	$(call MkuImage,lzma,,$(KDIR)/vmlinux.bin.lzma,$(KDIR)/uImage.lzma)
	$(call CompressGzip,$(KDIR)/vmlinux,$(KDIR)/vmlinux.bin.gz)
	$(call MkuImage,gzip,,$(KDIR)/vmlinux.bin.gz,$(KDIR)/uImage.gz)
endef

define Image/BuildKernel
	cp $(KDIR)/uImage.lzma $(BIN_DIR)/$(IMG_PREFIX)-uImage-lzma
	cp $(KDIR)/uImage.gz $(BIN_DIR)/$(IMG_PREFIX)-uImage-gzip
endef

#use lzma uimage and xt compressed suqashfs
define Image/MakeSysupgradeBinary
	$(eval output_name=$(IMG_PREFIX)-squashfs-sysupgrade.bin)
	$(call Image/mkfs/squashfs)
	cat $(KDIR)/uImage.lzma $(KDIR)/root.squashfs > $(KDIR)/$(output_name)
	$(call prepare_generic_squashfs,$(KDIR)/$(output_name))
	$(CP) $(KDIR)/$(output_name) $(BIN_DIR)/$(output_name)
ifeq ($(CONFIG_NAND_SUPPORT), y)
	$(call Build/append-metadata,$(BIN_DIR)/$(output_name))
endif
endef

define Image/Build/Initramfs
	$(call CompressLzma,$(KDIR)/vmlinux-initramfs,$(KDIR)/vmlinux-initramfs.bin.lzma)
	$(call MkuImage,lzma,,$(KDIR)/vmlinux-initramfs.bin.lzma,$(KDIR)/uImage-initramfs.lzma)
	cp $(KDIR)/vmlinux-initramfs.elf $(BIN_DIR)/$(IMG_PREFIX)-vmlinux-initramfs.elf
	cp $(KDIR)/vmlinux-initramfs $(BIN_DIR)/$(IMG_PREFIX)-vmlinux-initramfs.bin
	cp $(KDIR)/uImage-initramfs.lzma $(BIN_DIR)/uImage-initramfs.lzma
endef

define Image/Build
	$(call Image/Build/$(1))
	dd if=$(KDIR)/root.$(1) of=$(BIN_DIR)/$(IMG_PREFIX)-root.$(1) bs=128k conv=sync
	$(call Image/MakeSysupgradeBinary)
endef
endif

$(eval $(call BuildImage))
