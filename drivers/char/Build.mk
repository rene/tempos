##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := drivers/char
objs := i8042.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

