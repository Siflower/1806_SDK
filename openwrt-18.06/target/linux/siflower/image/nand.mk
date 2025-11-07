define Device/siflower_ac28nand
	ATH_SOC := sf19a28
	DEVICE_TITLE := AC28NAND
	KERNEL_SIZE := 4096k
	BLOCKSIZE := 128k
	PAGESIZE := 2048
	VID_HDR_OFFSET := 2048
	IMAGES += factory.img sysupgrade.tar
	IMAGE/sysupgrade.tar := sysupgrade-tar | append-sf-metadata
	IMAGE/factory.img := append-kernel | pad-to $$$$(KERNEL_SIZE) | append-ubi
endef

ifdef CONFIG_TARGET_siflower_sf19a28_fullmask_SF19A28-AC28NAND
	TARGET_DEVICES += siflower_ac28nand
endif
