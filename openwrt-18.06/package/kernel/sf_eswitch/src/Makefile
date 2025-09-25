rtk_dir=./realtek8367c_src
intel_dir=./intel7084_src
an8855_dir=./an8855_src
yt9215rb_dir=./yt9215rb_src

export rtk_dir
export intel_dir
export an8855_dir
export yt9215rb_dir

include $(SUBDIRS)/$(rtk_dir)/rtk_config.mk
include $(SUBDIRS)/$(intel_dir)/intel_config.mk
include $(SUBDIRS)/$(an8855_dir)/an8855_config.mk
include $(SUBDIRS)/$(yt9215rb_dir)/yt9215rb_config.mk

SFAX8_RTK_API_OBJS :=	$(SFAX8_REALTEK8367C_SRC_OBJS)
SFAX8_INTEL_API_OBJS :=		$(SFAX8_INTEL7084_SRC_OBJS)
SFAX8_AN8855_API_OBJS :=	$(SFAX8_AN8855_SRC_OBJS)
SFAX8_YT9215RB_API_OBJS :=  $(SFAX8_YT9215RB_SRC_OBJS)

sf_eswitch-y   += $(SFAX8_RTK_API_OBJS)
sf_eswitch-y   += $(SFAX8_INTEL_API_OBJS)
sf_eswitch-y   += $(SFAX8_AN8855_API_OBJS)
sf_eswitch-y   += $(SFAX8_YT9215RB_API_OBJS)
sf_eswitch-y   += sf_eswitch_driver.o
sf_eswitch-y   += sf_eswitch_debug.o

# Build as part of Kernel driver
obj-$(CONFIG_SFAX8_ESWITCH_DRIVER)	+= sf_eswitch.o
