##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TBS - Build configuration file
#

obj-y += exceptions.o gdt.o idt.o io.o dump_cpu.o

obj-x86asm += isr.o task.o

