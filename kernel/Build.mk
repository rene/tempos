##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purposing Operating System
#
# Makefile
#

CDIR := kernel
objs := execve.o exit.o fork.o kernel.o read.o syscall.o write.o timer.o

OBJFILES += $(patsubst %.o,$(CDIR)/%.o,$(objs))

