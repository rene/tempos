/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: vfs.c
 * Desc: Implements the Virtual File System Layer
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
#include <fs/vfs.h>
#include <fs/device.h>
#include <fs/ext2/ext2.h>

/** Global list of free i-nodes */
vfs_inode *free_inodes;
/** Head of the free i-nodes list */
vfs_inode *free_inodes_head;

/** Global mount table */
vfs_mount_table mount_table[VFS_MAX_MOUNTED_FS];


/**
 * This function initializes all File System types recognized
 * by TempOS.
 * \note If you are going to implement a new File System type for TempOS,
 *       your init function should be called from here.
 */
void register_all_fs_types(void)
{
	int i;
	vfs_inode *head, *inode, *prev;

	kprintf(KERN_INFO "Initializing VFS...\n");

	free_inodes = (vfs_inode*)kmalloc(sizeof(vfs_inode) * VFS_MAX_OPEN_FILES, GFP_NORMAL_Z);
	if (free_inodes == NULL) {
		panic("Could not allocate memory for i-node system queue.");
	}

	/* Make circular linked list of free i-nodes */
	head = &free_inodes[0];
	head->free_next = head;
	head->free_prev = head;
	prev = head;
	
	for (i = 1; i < VFS_MAX_OPEN_FILES; i++) {
		inode = &free_inodes[i];
		inode->free_prev = prev;
		inode->free_next = head;
		head->free_prev = inode;
		prev->free_next = inode;
		prev = inode;
	}
	
	free_inodes_head = head;

	/* Initialize device drivers interface */
	init_drivers_interface();

	/* Register all supported file systems by the kernel */
	#ifdef CONFIG_FS_EXT2
		register_ext2();
	#endif

	return;
}

