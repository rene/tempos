#!/bin/sh

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

if [ $# -lt 2 ]; then
	echo "Usage: $0 <config_file> <output_makefile>"
	exit 1
else
	conffile=$1
	makefile=$2
fi

valid_vars=$(sed -n "s/ \{0,\}\\(\w\+\\) \{0,\}= \{0,\}y/\1=y\\nexport \1/gp" $conffile)

for var in $valid_vars; do
	echo ">>> $var"
done

