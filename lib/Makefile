##
# Copyright (C) 2009 Renê de Souza Pinto
# Tempos - Tempos is an Educational and multi purposing Operating System
#

objs-files := ctype.o printf.o stdlib.o string.o linkedl.o

OBJS += $(objs-files)

all: $(objs-files)

%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c $<

