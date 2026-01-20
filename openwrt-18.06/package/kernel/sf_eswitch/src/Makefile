rtk_dir=./realtek8367c_src
intel_dir=./intel7084_src
nf2507_dir=./nf2507_src
yt9215rb_dir=./yt9215rb_src
an8855_dir=./an8855_src
jl5106_dir=./jl51xx_src

export rtk_dir
export intel_dir
export nf2507_dir
export yt9215rb_dir
export an8855_dir
export jl5106_dir

include $(SUBDIRS)/$(rtk_dir)/rtk_config.mk
include $(SUBDIRS)/$(intel_dir)/intel_config.mk
include $(SUBDIRS)/$(nf2507_dir)/nf2507_config.mk
include $(SUBDIRS)/$(yt9215rb_dir)/yt9215rb_config.mk
include $(SUBDIRS)/$(an8855_dir)/an8855_config.mk
include $(SUBDIRS)/$(jl5106_dir)/jl5106_config.mk

SFAX8_RTK_API_OBJS :=	$(SFAX8_REALTEK8367C_SRC_OBJS)
SFAX8_INTEL_API_OBJS :=	$(SFAX8_INTEL7084_SRC_OBJS)
SFAX8_NF2507_API_OBJS :=	$(SFAX8_NF2507_SRC_OBJS)
SFAX8_YT9215RB_API_OBJS :=	$(SFAX8_YT9215RB_SRC_OBJS)
SFAX8_AN8855_API_OBJS :=	$(SFAX8_AN8855_SRC_OBJS)
SFAX8_JL5106_API_OBJS :=	$(SFAX8_JL5106_SRC_OBJS)

sf_eswitch-y   += $(SFAX8_RTK_API_OBJS)
sf_eswitch-y   += $(SFAX8_INTEL_API_OBJS)
sf_eswitch-y   += $(SFAX8_NF2507_API_OBJS)
sf_eswitch-y   += $(SFAX8_YT9215RB_API_OBJS)
sf_eswitch-y   += $(SFAX8_AN8855_API_OBJS)
sf_eswitch-y   += $(SFAX8_JL5106_API_OBJS)
sf_eswitch-y   += sf_eswitch_driver.o
sf_eswitch-y   += sf_eswitch_debug.o

# Build as part of Kernel driver
obj-$(CONFIG_SFAX8_ESWITCH_DRIVER)	+= sf_eswitch.o
