##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#

objs-files := karch.o video.o

all: boot.o $(objs-files)

boot.o: boot.S
	@echo + AS $<
	@$(CC) $(CFLAGS) -c boot.S

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<

