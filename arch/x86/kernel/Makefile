##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := i8259A.o irq.o sys_enter.o

OBJS += $(objs-files)

#SUBDIRS = boot kernel

#.PHONY: subdirs $(SUBDIRS)

#all: $(objs-files) $(SUBDIRS)
all: $(objs-files)

#$(SUBDIRS):
#	$(MAKE) -C $@

sys_enter.o: sys_enter.S
	@echo + AS $<
	@$(CC) $(CFLAGS) -c $<

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<


