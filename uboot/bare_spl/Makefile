
ifdef use_mti
GCC_VENDOR=mti
CC=mips-$(GCC_VENDOR)-elf-gcc
LD=mips-$(GCC_VENDOR)-elf-ld
OD=mips-$(GCC_VENDOR)-elf-objdump
OC=mips-$(GCC_VENDOR)-elf-objcopy
SZ=mips-$(GCC_VENDOR)-elf-size
else
PWD := $(shell pwd)
GCC_VENDOR=$(PWD)/../toolchain/mips/bin/mipsel-openwrt-linux-uclibc
STAGING_DIR:=$(PWD)
export STAGING_DIR
CC=$(GCC_VENDOR)-gcc
LD=$(GCC_VENDOR)-ld
OD=$(GCC_VENDOR)-objdump
OC=$(GCC_VENDOR)-objcopy
SZ=$(GCC_VENDOR)-size
endif

BASE=.
ARCH=./arch/mips
INC=./include
ARCH_INC=$(ARCH)/include

CFLAGS= -Os -g -EL -c -I $(BASE)/common -I . -I $(ARCH_INC) -I $(INC) -mmt
CFLAGS+=-Wall -Werror
CFLAGS+=-march=mips32r2
CFLAGS+= -ffunction-sections -fdata-sections
## SMP support
# CFLAGS+= -DSMP

LDFLAGS_SF1688=-nostdlib -EL -Wl,-Map=sf1688_map -Wl,--gc-sections

ifdef fullmask
LDFLAGS_SF1688+=-T $(BASE)/sf1688_fullmask.ld
else
LDFLAGS_SF1688+=-T $(BASE)/sf1688_mpw0.ld
endif

ASOURCES= \
$(ARCH)/cpu/start.S \
$(ARCH)/cpu/init_cp0.S \
$(ARCH)/cpu/copy_c2_ram.S \
$(ARCH)/cpu/cpu_timer.S \
$(ARCH)/lib/init_L2_CM2.S \
$(ARCH)/lib/release_mp.S \
$(ARCH)/lib/init_caches.S

CSOURCES= \
$(BASE)/main.c \
$(BASE)/common/sys_manager.c \
$(BASE)/common/clk.c \
$(BASE)/common/pad.c

ifdef small
CFLAGS +=-DCONFIG_SFA18_UBOOT_LITE
endif

ifdef rmii
CFLAGS +=-DRMII
else
CFLAGS +=-DRGMII
endif

ifdef sf19a28_mpw0
CFLAGS += -DSF19A28_MPW0 -DSF19A28
else
ifdef mpw0
CFLAGS +=-DSFA18_CRYSTAL_6M -DMPW0
endif
endif

ifdef sf19a28_fullmask
ifdef performance_mode
CFLAGS += -DPERFORMANCE_MODE
endif
CFLAGS += -DSF19A28_FULLMASK -DSF19A28
else
ifdef fullmask
CFLAGS += -DFULLMASK
endif
endif

ifdef fpga
CFLAGS +=-DFPGA
CFLAGS +=-DDDR -I $(BASE)/ddr_fpga
CSOURCES += $(BASE)/ddr_fpga/ddr_init.c
else
include $(BASE)/ddr/Makefile
CSOURCES += $(addprefix $(BASE)/ddr/, $(obj-y))
endif

ifeq ($(skip), 1)
CFLAGS +=-DSKIP_SPI_FLASH
endif

ifeq ($(odt), 0)
CFLAGS +=-DDDR_DISABLE_ODT
endif

ifdef led_no
CFLAGS +=-DLED_ID=$(led_no)
endif

ifdef flash_size
CFLAGS +=-DSFA18_FLASH_SIZE_MB=$(flash_size)
endif

ifeq ($(cmd_boot_test), 1)
CFLAGS += -DCONFIG_CMD_BOOT_TEST
CFLAGS += -DCMD_BOOT_TEST_SPL_SIZE_KB=$(cmd_boot_test_spl_size_kb)
endif

ifeq ($(spi_nand), 1)
CFLAGS += -DSPI_NAND
endif

CFLAGS +=-DSPI -DFLASH -I $(BASE)/spi
CSOURCES += $(wildcard spi/*.c)

CFLAGS +=-DUART -I $(BASE)/uart
CSOURCES += $(BASE)/uart/uart.c

ifdef mmc
CFLAGS +=-DMMC -I $(BASE)/mmc
CSOURCES += $(BASE)/mmc/mmc.c
endif

ifdef security_boot
CSOURCES += $(wildcard security_boot/*.c)
CFLAGS +=-DSECURITY_BOOT
endif

ifdef poe
CSOURCES += $(wildcard ip808/*.c)
CFLAGS += -DPOE -I $(BASE)/ip808
endif

ifdef crystal_40m
CFLAGS += -DCRYSTAL_40M
endif

ASOURCES += \
	$(BASE)/common/memset.S \
	$(BASE)/common/memcpy.S \
	$(BASE)/common/memcmp.S

CSOURCES += \
	$(BASE)/common/heap.c \
	$(BASE)/common/stdio.c

COBJECTS=$(CSOURCES:.c=.o)
AOBJECTS=$(ASOURCES:.S=.o)


AFLAGS = $(CFLAGS) -D__ASSEMBLY__

SF1688 : $(COBJECTS) $(AOBJECTS)
	$(CC)  $(LDFLAGS_SF1688) $(COBJECTS) $(AOBJECTS) $(LIBS) -o sf1688.elf
	$(SZ) sf1688.elf
	$(OD) -d -S -l sf1688.elf > sf1688_dasm
	$(OC) sf1688.elf -O binary sf1688.bin
	./tools/checksum sf1688.bin ../$(irom_patch)
.c.o:
	$(CC) $(CFLAGS) $< -o $@
.S.o:
	$(CC) $(AFLAGS) $< -o $@

clean:
	rm -f sf1688_map
	rm -f sf1688_dasm
	rm -f sf1688.elf
	rm -f `find -name "*.o"`
	rm -f *.bin
	rm -f irom_spl.img
ifneq ($(OS),Windows_NT)
	find  $(BASE)/ -name "*.o" -exec rm -rf {} \;
endif
