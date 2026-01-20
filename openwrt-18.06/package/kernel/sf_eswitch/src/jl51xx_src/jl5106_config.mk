#config

ccflags-y += -I$(jl5106_dir)\inc \
	-I$(jl5106_dir)\inc\driver \
	-I$(jl5106_dir)\inc\jl51xx

ccflags-y += -DCONFIG_JLFE_CHIP_ID_5106 \
	-DCONFIG_JLFE_EXT_PORT_MODE \
	-DCONFIG_JLFE_EXT_PORT0_RMII_PHY_MODE

SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/driver/jl_reg_io.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_acl.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_cpu.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_fc.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_l2.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_led.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_lpd.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_mib.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_mirror.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_phy.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_port.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_qos.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_sram_load.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_storm.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_stp.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_switch.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_drv_vlan.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl51xx/jl51xx_mac_uctrl.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/acl.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/cpu.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/fc.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl_error.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/jl_switch.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/l2.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/led.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/lpd.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/mib.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/mirror.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/port.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/qos.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/sram_load.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/storm.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/stp.o
SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/src/vlan.o

SFAX8_JL5106_SRC_OBJS   += $(jl5106_dir)/sf_jl5106_ops.o