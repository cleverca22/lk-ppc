LOCAL_DIR := $(GET_LOCAL_DIR)

# tested with: qemu-system-ppc64 -serial mon:stdio -M pseries -cpu 970 -kernel ~/apps/ppc/lk-ppc/build-qemu-ppc64/lk.elf

TARGET := qemu-ppc64

MODULES += app/shell
MODULES += app/tests
MODULES += lib/debugcommands
#MODULES += lib/gfx
#MODULES += lib/gfxconsole

DEBUG := 2
WITH_TESTS := false
