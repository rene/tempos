#!/bin/sh

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

if [ $# -lt 1 ]; then
	echo "Error: you need to pass the configuration file."
	exit 1
else
	conffile=$1
	target=$2
	makefile=
fi

echo -n " * Checking architecture..."

if [ $(sed -n "s/CONFIG_ARCH_X86 \{0,\}= \{0,\}[y]/y/gp" $conffile) == "y" ]; then
	echo "IA-32"
	makefile="arch/x86/build/Makefile"

elif [ $(sed -n "s/CONFIG_ARCH_X86_64 \{0,\}= \{0,\}[y]/y/gp" $conffile) == "y" ]; then
	echo "IA-64"
	makefile="arch/x86_64/build/Makefile"

else
	echo "Unknown"
	exit 1
fi

make --quiet -f $makefile $target && exit 0 || exit 1

