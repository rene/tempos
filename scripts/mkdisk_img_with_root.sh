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
		exit 1
	fi
}

# Make image using grub 1
use_grub1() {

	mktemp=$(which mktemp)
	if [ -n $mktemp ]; then
		errorlog=$($mktemp)
	else
		errorlog="/tmp/$$"
	fi

	echo -n " + Creating floppy disk image..."
	dd if=/dev/zero of=$IMGFILE bs=512 count=2880 > /dev/null 2>>$errorlog
	check_result

	echo -n " + Creating ext2 file system..."
	mkfs.ext2 -F $IMGFILE > /dev/null 2>>$errorlog
	check_result

	echo -n " * Mouting disk image..."
	mount -o loop $IMGFILE $DIRMNT > /dev/null 2>>$errorlog
	check_result

	echo -n " + Copying TempOS kernel file..."
	mkdir $DIRMNT/boot
	cp $TEMPOSFILE $DIRMNT/boot 2>>$errorlog
	check_result

	echo -n " + Installing GRUB..."
	mkdir $DIRMNT/boot/grub
	cp /boot/grub/stage* $DIRMNT/boot/grub

	IODEV=$(cat /proc/mounts | grep $DIRMNT | cut -d" " -f1)
	DEVICEMAP=$(mktemp)
	CMDFILE=$(mktemp)

	cat > $DIRMNT/boot/grub/menu.lst << EOF
##
# TempOS
#

color white/blue light-green/black

title   TempOS
root    (fd0)
kernel  /boot/tempos.elf root=3:1:b
EOF

echo "(fd0)		$IODEV" > $DEVICEMAP
cat > $CMDFILE << EOF
root (fd0)
install /boot/grub/stage1 d (fd0) (fd0)/boot/grub/stage2 0x8000 p (fd0)/boot/grub/menu.lst
quit
EOF

	grep -v ^# $CMDFILE | grub --batch --device-map=$DEVICEMAP > /dev/null 2>>$errorlog
	check_result

	rm -f $DEVICEMAP
	rm -f $CMDFILE

	echo -n " * Umount image..."
	umount $DIRMNT > /dev/null 2>>$errorlog
	check_result
}

# Make image using grub 2
use_grub2() {
	#FIXME: Add support for GRUB2
	echo "Sorry. TempOS does not support create floppy disk image with GRUB2 without genext2fs."
	echo "Please, call make install from other user than root."
	false
	check_result
}

# main
DIRMNT=/mnt

if [ $# != 2 ]; then
	echo "Use:"
	echo "$0 <tempos_kernel_image> <floppy_disk_image>"
	echo
	exit 1
else
	TEMPOSFILE=$1
	IMGFILE=$2
fi

# GRUB1 files
GRUB1TARBALL=scripts/grub1.tar.bz2

# Check grub's version
GRUBI=$(which grub-install)
if [ -z "$GRUBI" ]; then
	echo "ERROR: GRUB not found."
	false
	check_result
else
	GVERSION=$($GRUBI -v | sed "s/.*\([0-9]\.[0-9][0-9]\).*/\1/")
fi

if [ ${GVERSION:0:1} -eq 0 ]; then
	# GRUB1
	use_grub1
else
	# GRUB2
	use_grub2
fi

