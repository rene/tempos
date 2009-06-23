##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

PWD      := $(shell pwd)

kimage   := tempos.elf

# Architecture compiler and link flags
ARCH     := x86
ARCH_CF  := -m32
ARCH_LF  := -melf_i386

CC       := gcc
INCDIRS  := -I$(PWD)/include -I$(PWD)/arch/include
CFLAGS   := $(INCDIRS) -fno-builtin -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs $(ARCH_CF)

OBJFILES :=
DEPSDIR  := $(PWD)/.deps
CMRULES  := $(PWD)/Makefile.rules

export OBJFILES CFLAGS CC

.PHONY: clean tempos test install

all: tempos

include arch/$(ARCH)/Build.mk
include arch/$(ARCH)/boot/Build.mk
include arch/$(ARCH)/kernel/Build.mk
include arch/$(ARCH)/mm/Build.mk

include drivers/char/Build.mk
include drivers/block/Build.mk

include lib/Build.mk

include kernel/Build.mk
include kernel/mm/Build.mk


# ---------- Generic rules to build C sources ----------

SRC  := $(OBJFILES:.o=.c)
DEPS := $(patsubst %.o,$(DEPSDIR)/%.d,$(OBJFILES))

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(DEPSDIR)/%.d: %.c
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@$(CC) -MM -MT "$(patsubst $(DEPSDIR)/%.d,%.o,$@)" $(CFLAGS) $< \
			| sed 's#\($*\)\.o[ :]*#\1.o $@ : #g' > $@; \
			[ -s $@ ] || rm -f $@

-include $(DEPS)

# ------------------------------------------------------


tempos: $(OBJFILES)
	@echo Linking...
	@ld -o $(kimage) -T arch/$(ARCH)/boot/setup.ld $(OBJFILES) $(ARCH_LF)
	@echo done.

clean:
	@rm -rf $(DEPSDIR)
	@rm -f $(OBJFILES)
	@[ -f $(kimage) ] && rm -f $(kimage) || echo "No image found."

test:
	@qemu -M pc -fda disk.img -boot a

install:
	@sudo mount -o loop disk.img /mnt
	@sudo cp $(kimage) /mnt/boot
	@sudo umount /mnt

