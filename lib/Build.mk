##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := lib
objs := ctype.o linkedl.o printf.o stdlib.o string.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

