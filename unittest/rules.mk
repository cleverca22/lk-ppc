LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_SRCS := \
	$(LOCAL_DIR)/ppc_alu_tests.c \
	$(LOCAL_DIR)/ppc_cmp_tests.c \
	$(LOCAL_DIR)/ppc_logical_tests.c \
	$(LOCAL_DIR)/ppc_rotate_tests.c \
	$(LOCAL_DIR)/ppc_shift_tests.c \

MODULES += lib/unittest

include make/module.mk
