##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := gdt.o idt.o io.o exceptions.o

SUBDIRS = boot kernel mm

.PHONY: subdirs $(SUBDIRS)

all: isr.o $(objs-files) $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

isr.o: isr.S
	$(CC) $(CFLAGS) -c $<

.o: .c
	$(CC) $(CFLAGS) -c $<


