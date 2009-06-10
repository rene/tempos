##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := arch/x86
OBJS := exceptions.o  gdt.o  idt.o  io.o

OBJFILES += $(CDIR)/isr.o
OBJA := $(CDIR)/isr.o


#--- DO NOT CHANGE HERE ---#

OBJDIRS += $(CDIR)

OBJS := $(patsubst %.o,$(CDIR)/%.o,$(OBJS))
SRC  := $(OBJS:.o=.c)
DEPS := $(OBJS:.o=.d)

OBJFILES += $(OBJS)

all: $(OBJS) $(OBJA)

$(CDIR)/%.o: $(CDIR)/%.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c 
	@$(CC) -MM $(CFLAGS) $< \
		| sed 's#\(.*\)\.o[ :]*#\1.o $@ : #g' > $@; \
		[ -s $@ ] || rm -f $@

include $(DEPS)

#--------------------------#


$(CDIR)/isr.o: $(CDIR)/isr.S $(CDIR)/../include/x86/x86.h
	@echo + AS $<
	@$(CC) $(CFLAGS) -DASM -c $< -o $@

