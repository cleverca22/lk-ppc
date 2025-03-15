LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)
TOOLCHAIN_PREFIX := powerpc64-unknown-linux-gnuabielfv2-
ARCH_COMPILEFLAGS += -mcpu=powerpc64
#ARCH_COMPILEFLAGS += -finstrument-functions
# ARCH_LDFLAGS += -mcpu=powerpc64

#LD := vc4-elf-ld
#CC := vc4-elf-gcc
#OBJCOPY := vc4-elf-objcopy
#OBJDUMP := vc4-elf-objdump

MODULE_SRCS += $(LOCAL_DIR)/boot.S $(LOCAL_DIR)/arch.c $(LOCAL_DIR)/thread.c $(LOCAL_DIR)/timer.c

GLOBAL_DEFINES += PLATFORM_HAS_DYNAMIC_TIMER=1

include make/module.mk
