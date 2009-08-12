##
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# Makefile - TempOS Build System
#

srcname=makedeps.c
objname=makedeps.o
execname=$(deptool)

all: $(objname)
	@$(CC) $< -g -o $(execname)

$(objname): $(srcname)
	@echo " + CC $<"
	@$(CC) -g -c $<

clean:
	@[ -f $(execname) ] && (rm -f $(execname) && echo " - Remove $(execname)") || echo " ! $(execname) not found."
	@[ -f $(objname) ] && (rm -f $(objname) && echo " - Remove $(objname)") || echo " ! $(objname) not found."

