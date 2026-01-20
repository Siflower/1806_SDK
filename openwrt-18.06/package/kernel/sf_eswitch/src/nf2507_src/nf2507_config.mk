#config

SFAX8_NF2507_SRC_OBJS   := $(nf2507_dir)/l2sw_asicdrv.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_phy.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_vlan.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_svlan.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_port.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_inbwctrl.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_misc.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_rma.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_acl.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_hsb.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_scheduling.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_lut.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_mirror.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_portIsolation.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_cputag.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_led.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_mib.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_asicdrv_mirror.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2sw_switch.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/port.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/cpu.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/vlan.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/svlan.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/smi.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/acl.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/l2.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/led.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/stat.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/mirror.o
SFAX8_NF2507_SRC_OBJS   += $(nf2507_dir)/sf_nf2507_ops.o

ccflags-y += -DMDC_MDIO_OPERATION
