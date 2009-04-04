##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#
#

objs-files := i8259A.o

#SUBDIRS = boot kernel

#.PHONY: subdirs $(SUBDIRS)

#all: $(objs-files) $(SUBDIRS)
all: $(objs-files)

#$(SUBDIRS):
#	$(MAKE) -C $@

.o: .c
	$(CC) $(CFLAGS) -c $<


