#!/bin/sh

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#
# File: deptool.sh
# Desc: Receive a .mk file as argument and generate dependencies for
#       all objetcs described in the file. You can also pass the CC
#       and CFLAGS to be used as and with the compiler.
# Note: If you'll pass CFLAGS, you'll need to pass CC too.
#

if [ $# -lt 1 ]; then
	echo "Usage: $0 <file.mk> [CC CFLAGS]"
	exit 1
else
	mkfile=$1
	cc=$2
	shift 2
	cflags=$*
fi

# Sanity check
[ -z "$cc" ] && cc=cc

# Base dir of the .mk file
bdir=$(dirname $mkfile)

targets=$(sed -n -e "/ \{0,\}#.*/d" -e "s#\(\w\+\.o\)#${bdir}/\1#g" -e "/ \{0,\}\(\w\+\) \{0,\}[+:]=.*/p" $mkfile)

deps=$(echo -e "$targets" | sed "s# \{0,\}\(\w\+\) \{0,\}[+:]=.*#\1#g" | uniq | sed "s#\([^ ?]\+\)#\$(\1) #g" | tr -d "\n")

echo -e "##\n# This Makefile was automatic generated. Please, do not edit.\n#\n"
echo -e "\n${targets}\n"
echo -e "\nall: $deps\n\n"


OLDIFS=$IFS
LF=$(echo -en "\n\b")
IFS=$LF
for line in $targets; do

	target=$(echo $line | sed "s# \{0,\}\(\w\+\) \{0,\}[+:]=.*#\1#")

	objects=$(echo $line | sed "s#\(.*\) \{0,\}[+:]= \{0,\}\(.*\)#\2#g")

	IFS=$OLDIFS
	for object in $objects; do

		filename=$(echo $object | sed "s#\(.*\)\.o#\1#")

		# Check extension
		if [ -f "${filename}.c" ]; then
			# c extension
			ext="c"
		elif [ -f "${filename}.S" ]; then
			# S extension
			ext="S"
		fi

		# Check dependencies
		$cc -MM ${filename}.${ext} -MT $object $* || exit 1

		# Compile rules
		echo -e "\t@echo ' + COMPILE ' \$<"
		echo -e "\t\$(CC) -c \$< -o \$@ \$(${target}-CFLAGS)\n"

	done

done

echo -e "\n# Export ALL variables. Do not remove.\nexport\n"

