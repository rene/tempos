##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := arch/x86/boot
objs := karch.o  video.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))


# We need to keep boot.o at first place
OBJFILES := $(CDIR)/boot.o $(OBJFILES)

$(CDIR)/boot.o: $(CDIR)/boot.S $(CDIR)/../../include/x86/multiboot.h $(CDIR)/../../include/x86/karch.h
	@echo + AS $<
	@$(CC) $(CFLAGS) -DASM -c $< -o $@

