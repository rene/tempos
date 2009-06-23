##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := arch/x86
objs := exceptions.o  gdt.o  idt.o  io.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))


OBJFILES += $(CDIR)/isr.o
$(CDIR)/isr.o: $(CDIR)/isr.S $(CDIR)/../include/x86/x86.h
	@echo + AS $<
	@$(CC) $(CFLAGS) -DASM -c $< -o $@

