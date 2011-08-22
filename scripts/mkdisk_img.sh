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
	
	GRUB=$(which grub)

	mktemp=$(which mktemp)
	if [ -n $mktemp ]; then
		errorlog=$($mktemp)
	else
		errorlog="/tmp/$$"
	fi

	echo -n " + Creating floppy root directory..."
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
	echo -n " + Copying GRUB files..."
	cp /boot/grub/stage* $TMPDIR/boot/grub 2>>$errorlog
	check_result

	DEVICEMAP=$(mktemp)
	CMDFILE=$(mktemp)

	cat > $TMPDIR/boot/grub/menu.lst << EOF
##
# TempOS
#

color white/blue light-green/black

title   TempOS
root    (fd0)
kernel  /boot/tempos.elf root=3:1:b
EOF

echo "(fd0)		$IMGFILE" > $DEVICEMAP
cat > $CMDFILE << EOF
root (fd0)
install /boot/grub/stage1 d (fd0) (fd0)/boot/grub/stage2 0x8000 p (fd0)/boot/grub/menu.lst
quit
EOF

	echo -n " + Creating ext2 file system image..."
	$GENEXT2FS -d $TMPDIR -b1536 $IMGFILE >>$errorlog 2>&1
	check_result

	echo -n " + Installing GRUB..."

	grep -v ^# $CMDFILE | $GRUB --batch --device-map=$DEVICEMAP > /dev/null 2>>$errorlog
	check_result

	rm -f $DEVICEMAP
	rm -f $CMDFILE
	rm -rf $TMPDIR
}

# Make image using grub 2
use_grub2() {
	# FIXME: This is a workaround to make floppy disk with
	#        GRUB1 on systems with GRUB2 installed.

	mktemp=$(which mktemp)
	if [ -n $mktemp ]; then
		errorlog=$($mktemp)
	else
		errorlog="/tmp/$$"
	fi

	echo -n " + Creating floppy root directory..."
	TMPDIR=$($mktemp -d)
	if [ $? != 0 ]; then
		TMPDIR=/tmp/$$-dir
		mkdir $TMPDIR 2>>$errorlog
	fi
	check_result

	echo -n " + Copying TempOS kernel image..."
	mkdir -p $TMPDIR/boot
	cp $TEMPOSFILE $TMPDIR/boot 2>>$errorlog
	check_result
	
	echo -n " + Uncompressing GRUB1 files..."
	tar -xjf $GRUB1TARBALL -C $TMPDIR/boot
	mv $TMPDIR/boot/grub1.bin /tmp
	check_result

	echo -n " + Creating ext2 file system image..."
	$GENEXT2FS -d $TMPDIR -b1536 $IMGFILE >>$errorlog 2>&1
	check_result

	echo -n " + Installing GRUB..."
	dd if=/tmp/grub1.bin of=$IMGFILE conv=notrunc bs=512 count=1 >>$errorlog 2>&1
	check_result

	rm /tmp/grub1.bin
	rm -rf $TMPDIR
}

# main
if [ $# != 3 ]; then
	echo "Use:"
	echo "$0 <tempos_kernel_image> <floppy_disk_image> <path to genext2fs>"
	echo
	exit 1
else
	TEMPOSFILE=$1
	IMGFILE=$2
	GENEXT2FS=$3
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

