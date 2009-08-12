#!/bin/sh

#
# Copyright (C) 2009 Renê de Souza Pinto
# TempOS - Tempos is an Educational and multi purpose Operating System
#
# TempOS Build System
#

execute() {

	$*

	if [ $? -eq 0 ]; then
		echo "OK"
	else
		echo "ERROR!"
		exit 1
	fi
}


IMGFILE=floppy.img
DIRMNT=/mnt

if [ $# -ge 1 ]; then
	case $1 in
	
		"-h" | "--help")
			echo "Use:"
			echo "      $0 <image_file>"
			echo "      $0               -- For default file name ($IMGFILE)"
			exit 0
		;;

		*)
			IMGFILE=$1
		;;
	esac
fi

echo -n "Creating floppy disk image..."
execute dd if=/dev/zero of=$IMGFILE bs=512 count=2880

echo "Creating ext2 file system..."
execute yes "" | mkfs.ext2 $IMGFILE

echo "Mouting image..."
execute mount -o loop $IMGFILE $DIRMNT

echo "Installing GRUB..."
execute mkdir -p $DIRMNT/boot/grub
execute cp /boot/grub/stage* $DIRMNT/boot/grub

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

grep -v ^# $CMDFILE | grub --batch --device-map=$DEVICEMAP


rm -f $DEVICEMAP
rm -f $CMDFILE
umount $DIRMNT

echo "Done."

