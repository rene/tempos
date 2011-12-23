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
#include <tempos/wait.h>
#include <fs/vfs.h>
#include <fs/device.h>
#include <fs/ext2/ext2.h>

/** I-nodes hash queue */
vfs_inode **inode_hash_table;

/** Global list of free i-nodes */
vfs_inode *free_inodes;
/** Head of the free i-nodes list */
vfs_inode *free_inodes_head;

/** Global system file table */
vfs_file *file_table;

/** Global mount table */
vfs_mount_table mount_table[VFS_MAX_MOUNTED_FS];

/** Registered file system types */
vfs_fs_type *vfs_filesystems[SUPPORTED_FS];

/** Number of file system type already registered */
static int vfs_reg_types = 0;

/* Prototypes */
static vfs_inode *search_inode(dev_t device, uint32_t number);


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
	size_t ht_entries = INODE_HASH_TABLE_SIZE;

	kprintf(KERN_INFO "Initializing VFS...\n");

	free_inodes = (vfs_inode*)kmalloc(sizeof(vfs_inode) * VFS_MAX_OPEN_FILES, GFP_NORMAL_Z);
	inode_hash_table = (vfs_inode**)kmalloc(sizeof(vfs_inode*) * ht_entries, GFP_NORMAL_Z);
	if (free_inodes == NULL || inode_hash_table == NULL) {
		panic("Could not allocate memory for i-node system queue.");
	}

	/* Initialize i-node hash table */
	memset(inode_hash_table, 0, sizeof(vfs_inode*) * ht_entries);

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

	/* Initialize system's file table */
	file_table = (vfs_file*)kmalloc(sizeof(vfs_file) * VFS_MAX_OPEN_FILES, GFP_NORMAL_Z);
	if (file_table == NULL) {
		panic("Could not allocate memory for system's file table.");
	}

	/* Initialize device drivers interface */
	init_drivers_interface();

	for (i = 0; i < SUPPORTED_FS; i++) {
		vfs_filesystems[i] = NULL;
	}

	/* Register all supported file systems by the kernel */
	#ifdef CONFIG_FS_EXT2
		register_ext2();
	#endif

	return;
}

/**
 * Register a file system type to VFS.
 */
void register_fs_type(vfs_fs_type *type)
{
	if (vfs_reg_types >= SUPPORTED_FS) {
		kprintf(KERN_ERROR "File systems table is full!\n");
		return;
	}

	vfs_filesystems[vfs_reg_types] = type;

	vfs_reg_types++;
}

/**
 * Search for an i-node on i-nodes queue
 *
 * \param device Device which i-node belong.
 * \param number i-node number
 * \return vfs_inode The i-node (if was found), NULL otherwise.
 */
static vfs_inode *search_inode(dev_t device, uint32_t number)
{
	vfs_inode *head, *tmp;
	uint32_t pos;

	/* Find position based on i-node number */
	pos = number % INODE_HASH_TABLE_SIZE;
	head = inode_hash_table[pos];

	if (head == NULL) {
		return NULL;
	}

	/* Search for the i-node */
	tmp = head;
	while (tmp != NULL) {
		if (tmp->device.major == device.major &&
				tmp->device.minor == device.minor) {
			break;
		}
		tmp = tmp->next;
	}

	return tmp;
}


/**
 * iget
 */
vfs_inode *vfs_iget(vfs_superblock *sb, uint32_t number)
{
	vfs_inode *inode;
	vfs_superblock *i_sb = sb;

	while(1) {
	
		if ( (inode = search_inode(i_sb->device, number)) != NULL ) {
			
			/* Block is in hash table, check if is locked */
			if (mutex_is_locked(inode->lock)) {
				sleep_on(WAIT_INODE_BECOMES_UNLOCKED);
				continue;
			}

			/* Special processing for mount points */
			if ((inode->flags & IFLAG_MOUNT_POINT) != 0) {
				/* TODO: mount point processing */
			}

			/* */

		}
	
	}
}

