#config

ifdef CONFIG_SFAX8_ESWITCH_LIGHT
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/sf_rtk8367c_api.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/sf_rtk8367c_ops.o
else
SFAX8_REALTEK8367C_SRC_OBJS   := $(rtk_dir)/rtl8367c_asicdrv.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_phy.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_vlan.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_port.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_inbwctrl.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_green.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_misc.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_rma.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_scheduling.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_lut.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_mirror.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_portIsolation.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_cputag.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_led.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_mib.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtl8367c_asicdrv_mirror.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/rtk_switch.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/port.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/cpu.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/vlan.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/smi.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/l2.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/led.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/stat.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/mirror.o
SFAX8_REALTEK8367C_SRC_OBJS   += $(rtk_dir)/sf_rtk8367c_ops.o
endif

ccflags-y += -DMDC_MDIO_OPERATION
