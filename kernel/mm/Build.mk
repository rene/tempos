##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := kernel/mm
objs := init_mm.o kmalloc.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

