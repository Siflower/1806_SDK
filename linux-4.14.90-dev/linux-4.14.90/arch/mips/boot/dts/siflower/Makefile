# SPDX-License-Identifier: GPL-2.0
dtb-$(CONFIG_DT_SF19A28_FULLMASK_EVB)	+= sf19a28_fullmask_evb.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_AC28)	+= sf19a28_fullmask_ac28.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_PHY)	+= sf19a28_fullmask_phy.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_AC28NAND)	+= sf19a28_fullmask_ac28nand.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_BPI)	+= sf19a28_fullmask_bpi.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_BPI_LUCI2)	+= sf19a28_fullmask_bpi_luci2.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_BPI_LUCI2_NEUTRAL)	+= sf19a28_fullmask_bpi_luci2_neutral.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_BPI_LUCI2_SIFLOWER)	+= sf19a28_fullmask_bpi_luci2_siflower.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_AC28S)	+= sf19a28_fullmask_ac28s.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_YTS)	+= sf19a28_fullmask_yts.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_ROUTER_1211)	+= sf19a28_fullmask_router_1211.dtb

obj-y		+= $(patsubst %.dtb, %.dtb.o, $(dtb-y))
always				:= $(dtb-y)
clean-files			:= *.dtb *.dtb.S
