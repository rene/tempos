#!/bin/sh

RTREE=rtree
MINSIZE=10485760 #10MB
IMGNAME=hdisk.img
FFILE=part1.fdisk
PARTNUM=1

LOGFILE=$(mktemp)
MNTDIR=$(mktemp -d)

exec_c()
{
	$* > $LOGFILE 2>&1
	if [ $? -eq 0 ]; then
		echo -e "[ \033[0;32mOK\033[0m ]"
	else
		echo -e "[ \033[0;31mFailed\033[0m ]"
		echo "Log information:"
		cat $LOGFILE
		clean
		exit 1
	fi
}

clean()
{
	rm $LOGFILE
	if [ -n "$(mount | grep $MNTDIR)" ]; then
		umount $MNTDIR > /dev/null 2>&1
	fi
	if [ -n "$LOOPDEV" ]; then
		kpartx -d $LOOPDEV > /dev/null 2>&1
		losetup -d $LOOPDEV > /dev/null 2>&1
	fi
	rmdir $MNTDIR
}


##
# Main
#
if [ $UID != 0 ]; then
	echo "Please, run this script as root."
	echo
	exit 1
fi

DSIZE=$(du -s $RTREE | cut -f1)
if [ $DSIZE -lt $MINSIZE ]; then
	DSIZE=$MINSIZE
fi

# Initializing
echo "*** TempOS hard disk image generator ***"
echo "Disk output image name: $IMGNAME"
echo "Disk size (in bytes):   $DSIZE"
echo "Root tree source:       $RTREE"
echo "fdisk commands source:  $FFILE"
echo "Root partition number:  $PARTNUM"
echo "---------------------------------"

# Create image
echo -n "Creating disk image...             "
exec_c dd if=/dev/zero of=$IMGNAME bs=512 count=$(($DSIZE/512))

LOOPDEV=$(losetup -f)
res=$?
echo -n "Partitioning image...              "
if [ $res != 0 ]; then
	echo "Error: could not find loop device file."
	echo
	exit 1
else
	exec_c fdisk $IMGNAME < $FFILE
fi

echo -n "Checking partition(s)...           "
losetup $LOOPDEV $IMGNAME
exec_c kpartx -a $LOOPDEV

DEVPART=/dev/mapper/$(basename ${LOOPDEV})p${PARTNUM}

echo -n "Formating EXT2 file system...      "
exec_c mkfs.ext2 $DEVPART 

echo -n "Mounting partition...              "
exec_c mount $DEVPART $MNTDIR 

echo -n "Copying files...                   "
exec_c cp -rv $RTREE/* $MNTDIR

echo -n "Umounting partition...             "
exec_c umount $MNTDIR ; kpartx -d $LOOPDEV ; losetup -d $LOOPDEV

echo -n "Compressing...                     "
exec_c gzip $IMGNAME

clean
echo "Done."
exit 0

