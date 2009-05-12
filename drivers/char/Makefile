##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := i8042.o

#SUBDIRS = 

#.PHONY: subdirs $(SUBDIRS)

all: $(objs-files)

#$(SUBDIRS):
#	$(MAKE) -C $@

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<


