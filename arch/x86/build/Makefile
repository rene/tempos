##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile - TempOS Build System
#

.PHONY: x86 portable

CWD := arch/x86/build

# Architecture compiler and link flags
ARCH     := x86
ARCH_CF  := -m32
ARCH_LF  := -melf_i386
INCDIRS  := -I$(PWD)/include -I$(PWD)/arch/include
CFLAGS_2 := $(INCDIRS) -fno-builtin -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs $(ARCH_CF)

CFLAGS := $(CFLAGS_2)

export obj-y CFLAGS_2 C_FLAGS

flist_1  := $(CWD)/independent.list
flist_2  := $(CWD)/dependent.list
rules_1  := $(CWD)/Makefile_c.rules
rules_2  := $(CWD)/Makefile_asm.rules

mfile_1  := $(CWD)/Makefile1.gen
mfile_2  := $(CWD)/Makefile2.gen

bootfile := arch/x86/boot/boot.o


all: x86 portable
	@echo " * Linking kernel image..."
	@sed -i "#$(bootfile)#d" $(objlist)
	@$(LD) $(ARCH_LF) -o $(kimage) -T arch/x86/boot/setup.ld @$(objlist)
	@rm -f $(objlist)
	@echo "Build done."

###
# Specific rules for architecture
###

x86: $(mfile_2)
	@echo " * Start architecture dependent compilation:"
	@$(MAKE) --quiet -f $<
	@echo " * Architecture dependent build finished."

portable: $(mfile_1)
	@echo " * Start architecture independent compilation:"
	@$(MAKE) --quiet -f $<
	@echo " * Architecture independent build finished."


$(mfile_2): $(rules_2) $(flist_2)
	@echo -n " + Generating ASM dependencies..."
	@$(deptool) $(rules_2) $(flist_2) > $@ && echo "OK" || echo "ERROR!"

$(mfile_1): $(rules_1) $(flist_1) 
	@echo -n " + Generating dependencies..."
	@$(deptool) $(rules_1) $(flist_1) > $@ && echo "OK" || echo "ERROR!"


####
# Mandaroty rules for Build System
####

##
# install
#
install: $(fimage)
	@mount -o loop $(fimage) /mnt
	@cp $(kimage) /mnt/boot
	@umount /mnt
	@echo "Installation done."

##
# test
#
test:
	@qemu -M pc -fda $(fimage) -boot a


##
# Clean
#
clean: $(mfile_1) $(mfile_2)
	@$(MAKE) --quiet -f $(mfile_1) clean
	@$(MAKE) --quiet -f $(mfile_2) clean
	@[ -f $(mfile_1) ] && (rm -f $(mfile_1) && echo " - Remove $(mfile_1)") || echo " ! $(mfile_1) not found."
	@[ -f $(mfile_2) ] && (rm -f $(mfile_2) && echo " - Remove $(mfile_2)") || echo " ! $(mfile_2) not found."
	@[ -f $(kimage) ] && (rm -f $(kimage) && echo " - Remove $(kimage)") || echo " ! $(kimage) not found."
	@[ -f $(fimage) ] && (rm -f $(fimage) && echo " - Remove $(fimage)") || echo " ! $(fimage) not found."

