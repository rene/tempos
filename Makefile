##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

PWD := $(shell pwd)

ARCH   := x86
CC     := gcc
CFLAGS := -I$(PWD)/include -fno-builtin -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs

export ARCH CC CFLAGS

SUBDIRS = arch kernel lib

.PHONY: subdirs $(SUBDIRS) clean tempos test install

all: tempos

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@


clean:
	@find . -type f -name "*.o" -exec rm {} \;
	@rm tempos.elf

tempos: subdirs
	@echo "Generating objects file list..."
	@find . -type f -name "*.o" > objs.list
	@ld -o tempos.elf -T arch/$(ARCH)/boot/setup.ld @objs.list
	@echo "Removing temporary files..."
	@rm objs.list

test:
	qemu -M pc -fda disk.img -boot a

install:
	sudo mount -o loop disk.img /mnt
	sudo cp tempos.elf /mnt/boot
	sudo umount /mnt

