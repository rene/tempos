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
kernel  /boot/tempos.elf
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

