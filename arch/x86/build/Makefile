##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile - TempOS Build System
#

.PHONY: x86 portable

CWD := arch/x86/build


# Architecture compiler and link flags
ARCH_CF  := -m32
ARCH_LF  := -melf_i386
INCDIRS  := -I$(PWD)/include -I$(PWD)/arch/include
CFLAGS   := $(INCDIRS) -fno-builtin -Wall -nostdlib -nostartfiles -nodefaultlibs $(ARCH_CF)

MAKEGEN  := $(CWD)/MakefileX86.gen

##
# Specific CFLAGS for each target described in .mk files
#
obj-y-CFLAGS      := $(CFLAGS)
obj-x86asm-CFLAGS := -DASM $(CFLAGS)

export obj-y-CFLAGS obj-x86asm-CFLAGS

.PHONY: kernel install test clean

##
# Architecture specific
#
all: $(kimage)
	@$(ECHO) " * Linking kernel image..."
	@$(LD) $(ARCH_LF) -o $(kimage) -T arch/x86/boot/setup.ld $(obj-x86asm) $(obj-y)
	@$(ECHO) "Build done."


##
# Generate Makefile for kernel
#
-include $(MAKEGEN)

$(kimage): $(MAKEGEN)
	@$(ECHO) " * Start kernel compilation:"

$(MAKEGEN): \
		arch/x86/boot/Build.mk		\
		arch/x86/Build.mk			\
		arch/x86/kernel/Build.mk	\
		arch/x86/mm/Build.mk		\
		lib/Build.mk				\
		drivers/char/Build.mk		\
		drivers/block/Build.mk		\
		fs/Build.mk					\
		kernel/Build.mk				\
		kernel/mm/Build.mk
	@$(ECHO) -n " + Generating dependencies..."
	@for file in $?; do 					 			 \
		$(DEPTOOL) $$file $(CC) $(CFLAGS) >> $(MAKEGEN); \
	done && echo "OK" || echo "ERROR"



####
# Mandatory rules for Build System
####

##
# install
#
install:
	@scripts/do_mkdisk.sh $(PWD)/$(kimage) $(PWD)/$(fimage)
	@$(ECHO) "Installation done."


##
# test
#
test:
	@[ -f hdtest.img.gz ] || @qemu -M pc -fda $(fimage) -boot a &&  ( TEMPOSHD=`mktemp`; gunzip hdtest.img.gz -c > $$TEMPOSHD; qemu -M pc -fda $(fimage) -hda $$TEMPOSHD -boot a; rm $$TEMPOSHD )


##
# Clean
#
clean: $(MAKEGEN)
	@for obj in $(obj-x86asm) $(obj-y); do \
		[ -f $$obj ] && (rm -f $$obj && echo " - REMOVING $$obj") || echo " ! $$obj not found."; \
	done
	@[ -f $(MAKEGEN) ] && (rm -f $(MAKEGEN) && echo " - REMOVING $(MAKEGEN)") || echo " ! $(MAKEGEN) not found."
	@[ -f $(kimage) ] && (rm -f $(kimage) && echo " - REMOVING $(kimage)") || echo " ! $(kimage) not found."
	@[ -f $(fimage) ] && (rm -f $(fimage) && echo " - REMOVING $(fimage)") || echo " ! $(fimage) not found."

