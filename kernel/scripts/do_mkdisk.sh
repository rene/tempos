#!/bin/bash

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#


if [ $# != 2 ]; then
	echo "Use:"
	echo "$0 <tempos_kernel_image> <floppy_disk_image>"
	echo
	exit 1
fi

##
# Check for genext2fs
#
echo -n " + Checking for genext2fs..." 
GENEXT2FS=$(which genext2fs 2> /dev/null)
if [ $? -eq 0 ]; then
	echo "OK"
	./scripts/mkdisk_img.sh $1 $2 $GENEXT2FS
else
	# genext2fs not present, check for root user
	echo "not found."
	echo -n " ! genext2fs not present. Checking for root user..."
	WHOAMI=$(whoami)

	if [ $WHOAMI == "root" ]; then
		echo "OK"
		./scripts/mkdisk_img_with_root.sh $1 $2
	else
		echo "non root"
		echo " ERROR: genext2fs was not found in your system. You should run 'make install'"
		echo "        as root to create floppy disk image."
		echo " >> You can download genext2fs from http://genext2fs.sourceforge.net/"
		exit 1
	fi
fi

