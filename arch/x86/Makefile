##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := gdt.o idt.o io.o exceptions.o isr.o

SUBDIRS = boot kernel mm

.PHONY: subdirs $(SUBDIRS)

all: $(objs-files) $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

isr.o: isr.S
	@echo + AS $<
	@$(CC) $(CFLAGS) -c $<

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<



