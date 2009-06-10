##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := arch/x86/kernel
OBJS :=  i8259A.o  irq.o

OBJFILES += $(CDIR)/sys_enter.o
OBJA := $(CDIR)/sys_enter.o


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


$(CDIR)/sys_enter.o: $(CDIR)/sys_enter.S $(CDIR)/../../include/x86/x86.h include/tempos/error.h include/tempos/syscall.h
	@echo + AS $<
	@$(CC) $(CFLAGS) -DASM -c $< -o $@

