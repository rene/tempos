##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile
#

obj-y += exceptions.o  gdt.o  idt.o  io.o

obj-x86asm += isr.o

