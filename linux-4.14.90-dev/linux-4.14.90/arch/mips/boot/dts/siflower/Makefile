# SPDX-License-Identifier: GPL-2.0
dtb-$(CONFIG_DT_SF16A18_FULLMASK_EVB_V5)	+= sf16a18_fullmask_evb_v5.dtb

dtb-$(CONFIG_DT_SF19A28_FULLMASK_FPGA)	+= sf19a28_fullmask_fpga.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_EVB)	+= sf19a28_fullmask_evb.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_AC28)	+= sf19a28_fullmask_ac28.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_REP)	+= sf19a28_fullmask_rep.dtb

obj-y		+= $(patsubst %.dtb, %.dtb.o, $(dtb-y))
always				:= $(dtb-y)
clean-files			:= *.dtb *.dtb.S
