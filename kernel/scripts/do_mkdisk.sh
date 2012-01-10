#!/bin/bash

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#


if [ $# != 2 ]; then
	echo "Use:"
	echo "$0 <tempos_kernel_image> <cd_iso_image>"
	echo
	exit 1
fi

./scripts/mkdisk_img.sh $1 $2

