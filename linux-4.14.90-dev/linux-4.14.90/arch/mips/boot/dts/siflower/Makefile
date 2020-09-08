# SPDX-License-Identifier: GPL-2.0
dtb-$(CONFIG_DT_SF16A18_FULLMASK_P20)  += sf16a18_fullmask_p20.dtb
dtb-$(CONFIG_DT_SF16A18_FULLMASK_EVB_V5)	+= sf16a18_fullmask_evb_v5.dtb
dtb-$(CONFIG_DT_SF16A18_FULLMASK_86V)    += sf16a18_fullmask_86v.dtb
dtb-$(CONFIG_DT_SF16A18_FULLMASK_AC20_SJBY)  += sf16a18_fullmask_ac20_sjby.dtb
dtb-$(CONFIG_DT_SF16A18_FULLMASK_AC20)  += sf16a18_fullmask_ac20.dtb
dtb-$(CONFIG_DT_SF16A18_FULLMASK_P10H)  += sf16a18_fullmask_p10h.dtb
dtb-$(CONFIG_DT_SF19A28_FULLMASK_FPGA)	+= sf19a28_fullmask_fpga.dtb
dtb-$(CONFIG_DT_SF19A28_MPW1_AC28)	+= sf19a28_mpw1_ac28.dtb
dtb-$(CONFIG_DT_SF19A28_MPW1_AC22)	+= sf19a28_mpw1_ac22.dtb

obj-y		+= $(patsubst %.dtb, %.dtb.o, $(dtb-y))
always				:= $(dtb-y)
clean-files			:= *.dtb *.dtb.S
