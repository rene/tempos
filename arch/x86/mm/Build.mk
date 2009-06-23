##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := arch/x86/mm
objs := mm.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

