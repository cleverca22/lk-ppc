#!/bin/sh

set -e

make PROJECT=lk-ppc
mkdir -pv iso
elf-converter build-lk-ppc/lk.elf iso/output.elf
mkisofs -full-iso9660-filenames -o test.iso iso
echo done
