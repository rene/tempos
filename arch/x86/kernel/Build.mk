##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := arch/x86/kernel
objs := i8259A.o i82C54.o irq.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))


OBJFILES += $(CDIR)/sys_enter.o
$(CDIR)/sys_enter.o: $(CDIR)/sys_enter.S $(CDIR)/../../include/x86/x86.h include/tempos/error.h include/tempos/syscall.h
	@echo + AS $<
	@$(CC) $(CFLAGS) -DASM -c $< -o $@

