##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

PWD     := $(shell pwd)

# Architecture compile and link flags
ARCH := x86
ARCH_CF := -m32
ARCH_LF := -melf_i386

CC      := gcc
INCDIRS := -I$(PWD)/include -I$(PWD)/arch/include
CFLAGS  := $(INCDIRS) -fno-builtin -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs $(ARCH_CF)

OBJFILES :=
OBJDIRS  :=

export OBJDIRS OBJFILES CFLAGS CC

.PHONY: clean tempos test install


include arch/$(ARCH)/Build.mk
include arch/$(ARCH)/boot/Build.mk
include arch/$(ARCH)/kernel/Build.mk
include arch/$(ARCH)/mm/Build.mk

include drivers/char/Build.mk

include lib/Build.mk

include kernel/Build.mk
include kernel/mm/Build.mk

DFILES := $(OBJFILES:.o=.d)


tempos: $(OBJFILES)
	@echo Linking...
	@ld -o tempos.elf -T arch/$(ARCH)/boot/setup.ld $(OBJFILES) $(ARCH_LF)
	@echo done.

clean:
	@rm -f $(DFILES)
	@rm -f $(OBJFILES)
	@[ -f tempos.elf ] && rm -f tempos.elf || echo "No image found."

test:
	@qemu -M pc -fda disk.img -boot a

install:
	@sudo mount -o loop disk.img /mnt
	@sudo cp tempos.elf /mnt/boot
	@sudo umount /mnt

