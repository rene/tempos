##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := kernel.o

SUBDIRS = mm

.PHONY: subdirs $(SUBDIRS)

all: $(objs-files) $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<


