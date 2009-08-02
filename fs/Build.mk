##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

CDIR := fs
objs := binfmt_elf32.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

