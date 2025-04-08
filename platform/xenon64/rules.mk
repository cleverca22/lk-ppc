LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)
ARCH_COMPILEFLAGS += -mcpu=powerpc64 -m64
GLOBAL_MODULE_LDFLAGS += -melf64ppc

MEMBASE := 0x10000000
MEMSIZE := 0x10000000

GLOBAL_DEFINES += SMP_MAX_CPUS=6
GLOBAL_DEFINES += MEMBASE=$(MEMBASE) MEMSIZE=$(MEMSIZE)

LINKER_SCRIPT += $(LOCAL_DIR)/stage1.ld

MODULE_SRCS += $(LOCAL_DIR)/platform.c

include make/module.mk
