/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: mount.c
 * Desc: Implements mount related functions.
 *
 * This file is part of TempOS.
 *
 * TempOS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * TempOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <tempos/kernel.h>
#include <tempos/wait.h>
#include <fs/vfs.h>
#include <fs/device.h>

/**
 * Mount root file system
 *
 * \param device Root device.
 * \return 1 on success, 0 otherwise.
 */
int vfs_mount_root(dev_t device)
{
	int i;
	char found;
	vfs_fs_type *fs;
	vfs_mount_table *mnt;
	vfs_inode *root;

	/* Try to find which file system device is formated */
	found = 0;
	for (i = 0; i < VFS_SUPPORTED_FS; i++) {
		fs = vfs_filesystems[i];
		if (fs != NULL) {
			if ( fs->check_fs_type(device) ) {
				kprintf("VFS: Found %s file system.\n", fs->name);
				found = 1;
				break;
			}
		}
	}
	if (!found) {
		panic("VFS: Cannot mount root. Unknown file system type.");
	}

	/* Mount table entry: first position */
	mnt = &mount_table[0];

	/* Read file system super block */
	fs->get_sb(device, &mnt->sb);

	/* Get root i-node */
	root = vfs_iget(&mnt->sb, 0);

	/* Check if is a directory */
	if ( !(root->i_mode & S_IFDIR) ) {
		return 0;
	}

	/* Fill mount table entry */
	mnt->device       = device;
	mnt->root_inode   = root;
	mnt->mnt_on_inode = root;
	mnt->fs           = fs;

	kprintf("VFS: device (%d,%d) mounted as root.\n", device.major, device.minor);

	return 1;
}

