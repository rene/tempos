#!/bin/bash

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

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

GRUB=$(whereis -b grub | cut -d" " -f2)

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
fi

echo -n " + Copying files..."
mkdir -p $TMPDIR/boot/grub
cp $TEMPOSFILE $TMPDIR/boot 2>>$errorlog
check_result
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
kernel  /boot/tempos.elf
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

