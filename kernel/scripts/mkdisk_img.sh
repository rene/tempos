#!/bin/bash

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

# Check $? and show the properly message
check_result() {

	if [ $? -eq 0 ]; then
		echo "OK"
	else
		echo "ERROR!"
		echo "Log output:"
		[ -n "$errorlog" ] && cat $errorlog || echo "No error log avaliable."
		rm $errorlog
		exit 1
	fi
}

# Make image using grub 1
use_grub1() {
	
	MKISOFS=$(which mkisofs 2> /dev/null)

	if [ -z "$MKISOFS" ]; then
		echo "ERROR: mkisofs not found. Please, install cdrtools."
		echo "http://cdrecord.berlios.de/private/cdrecord.html"
		exit 1
	fi

	mktemp=$(which mktemp)
	if [ -n $mktemp ]; then
		errorlog=$($mktemp)
	else
		errorlog="/tmp/$$"
	fi

	echo -n " + Creating CD-ROM root directory..."
	TMPDIR=$($mktemp -d)
	if [ $? != 0 ]; then
		TMPDIR=/tmp/$$-dir
		mkdir $TMPDIR 2>>$errorlog
	fi
	check_result

	echo -n " + Copying TempOS kernel image..."
	mkdir -p $TMPDIR/boot/grub
	cp $TEMPOSFILE $TMPDIR/boot 2>>$errorlog
	check_result
	echo -n " + Copying GRUB boot image..."
	cp scripts/stage2_eltorito $TMPDIR/boot/grub 2>>$errorlog
	check_result

	echo -n " + Creating GRUB configuration file..."
	cat > $TMPDIR/boot/grub/menu.lst << EOF
##
# TempOS
#

color white/blue light-green/black

title   TempOS
kernel  /boot/tempos.elf root=3:1 init=/sbin/init

title	TempOS over serial
kernel  /boot/tempos.elf root=3:1 init=/sbin/init console=ttyS0 kgdbwait=1
EOF
	check_result

	echo -n " + Creating bootable GRUB ISO image..."
	$MKISOFS -R -b boot/grub/stage2_eltorito -no-emul-boot \
		-boot-load-size 4 -boot-info-table -o $IMGFILE $TMPDIR > $errorlog 2>&1
	check_result

	rm -rf $TMPDIR 2> /dev/null
	rm -f $errorlog 2> /dev/null
}

# Make image using grub 2
use_grub2() {

	mktemp=$(which mktemp)
	if [ -n $mktemp ]; then
		errorlog=$($mktemp)
	else
		errorlog="/tmp/$$"
	fi

	echo -n " + Creating CD-ROM root directory..."
	TMPDIR=$($mktemp -d)
	if [ $? != 0 ]; then
		TMPDIR=/tmp/$$-dir
		mkdir $TMPDIR 2>>$errorlog
	fi
	check_result

	echo -n " + Copying TempOS kernel image..."
	mkdir -p $TMPDIR/boot/grub
	cp $TEMPOSFILE $TMPDIR/boot 2>>$errorlog
	check_result

	echo -n " + Creating GRUB configuration file..."
	cat > $TMPDIR/boot/grub/grub.cfg << EOF
set menu_color_normal="white/blue"
set menu_color_highlight="light-green/black"

##
# TempOS
#
menuentry "TempOS" {
	multiboot /boot/$(basename $TEMPOSFILE) root=3:1 init=/sbin/init
}
menuentry "TempOS over serial" {
        multiboot /boot/$(basename $TEMPOSFILE) root=3:1 init=/sbin/init kgdbwait=1
}
EOF
	check_result

	echo -n " + Creating bootable GRUB ISO image..."
	$GRUBMKR -o $IMGFILE $TMPDIR > $errorlog 2>&1
	check_result

	rm -rf $TMPDIR 2> /dev/null
	rm -f $errorlog 2> /dev/null
}

# main
if [ $# != 2 ]; then
	echo "Use:"
	echo "$0 <tempos_kernel_image> <iso_cd_image>"
	echo
	exit 1
else
	TEMPOSFILE=$1
	IMGFILE=$2
fi


# Check grub's version
GRUBI=$(which grub-install 2> /dev/null || which grub2-install 2> /dev/null)
GRUBMKR=$(which grub-mkrescue 2> /dev/null || which grub2-mkrescue 2> /dev/null)
if [ -z "$GRUBI" -a -z "$GRUBMKR" ]; then
	echo "ERROR: GRUB not found."
	exit 1
fi

if [ -z "$GRUBMKR" ]; then
	# GRUB1
	use_grub1
else
	# TODO: fix GRUB2
	use_grub1
fi

