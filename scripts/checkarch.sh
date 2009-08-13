#!/bin/sh

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

if [ $# -lt 1 ]; then
	echo "Usage: $0 <config_file> [target]"
	exit 1
else
	conffile=$1
	target=$2
fi

ARCH=$(sed -n "s/^ \\{0,\\}CONFIG_ARCH_\\(\\w\\+\\).*/\1/gp" $conffile)
MKFILE=$(sed -n "s#^ \\{0,\\}CONFIG_${ARCH}_MAKEFILE \\{0,\\}= \\{0,\\}\\(.*\\)#\1#gp" $conffile)

if [ -f "$MKFILE" ]; then
	echo "$ARCH"
	make --quiet -f $MKFILE $target
else
	echo "Unknow"
	echo "***********************************"
	echo "* ERROR: Unsupported architecture *"
	echo "***********************************"
fi

