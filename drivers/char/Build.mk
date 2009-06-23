##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := drivers/char
objs := i8042.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

