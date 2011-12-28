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
#include <arch/io.h>

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
vfs_fs_type *vfs_filesystems[VFS_SUPPORTED_FS];

/** Number of file system type already registered */
static int vfs_reg_types = 0;

/* Prototypes */
static uint32_t _ipow(uint32_t x, uint32_t y);

static vfs_inode *search_inode(dev_t device, uint32_t number);

static void inode_remove_from_freelist(vfs_inode *inode);

static void add_inode_htable(vfs_inode *inode);

static vfs_inode *get_free_inode(vfs_superblock *sb, uint32_t number);

/**
 * Integer power function.
 *
 *\param x Operand.
 *\param y Operand.
 *\return uint32_t Value x raised to the power y.
 *\note Be coherent with values =:)
 */
static uint32_t _ipow(uint32_t x, uint32_t y)
{
	uint32_t i;
	uint32_t res = x;

	for (i = 1; i < y; i++) {
		res *= x;
	}

	return res;
}

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

	free_inodes = (vfs_inode*)kmalloc(sizeof(vfs_inode) * (VFS_MAX_OPEN_FILES+1), GFP_NORMAL_Z);
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
	
	head->flags = IFLAG_LIST_HEAD;
	free_inodes_head = head;

	/* Initialize system's file table */
	file_table = (vfs_file*)kmalloc(sizeof(vfs_file) * VFS_MAX_OPEN_FILES, GFP_NORMAL_Z);
	if (file_table == NULL) {
		panic("Could not allocate memory for system's file table.");
	}

	/* Initialize device drivers interface */
	init_drivers_interface();

	for (i = 0; i < VFS_SUPPORTED_FS; i++) {
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
	if (vfs_reg_types >= VFS_SUPPORTED_FS) {
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
		if ( DEV_CMP(tmp->device, device) ) {
			break;
		}
		tmp = tmp->next;
	}

	return tmp;
}

/**
 * Remove i-node from free list.
 * \param inode i-node.
 */
static void inode_remove_from_freelist(vfs_inode *inode)
{
	vfs_inode *head, *tmp, *prev, *next;

	head = free_inodes_head;
	tmp = head->free_next;
	while (tmp != head) {
		if ( DEV_CMP(tmp->device, inode->device) ) {
			break;
		}
		tmp = tmp->free_next;
	}

	if (tmp == head) {
		/* i-node it's not on free list */
		return;
	}

	/* Remove i-node */
	cli();
	prev = tmp->free_prev;
	next = tmp->free_next;

	next->prev = prev;
	prev->next = next;
	sti();
	return;
}

/**
 * Get new i-node from free list and initialize it.
 *
 * \param sb Super block associated with i-node.
 * \param number i-node number.
 * \return The new i-node.
 */
static vfs_inode *get_free_inode(vfs_superblock *sb, uint32_t number)
{
	vfs_inode *head, *tmp, *prev, *next;

	head = free_inodes_head;
	tmp = head->free_next;
	
	/* Sanity check */
	if (tmp == head) {
		panic("VFS: no i-node object available!");
	}

	/* try to find the i-node on the free list */
	while (tmp != head) {
		if ( DEV_CMP(tmp->device, sb->device) ) {
			break;
		}
		tmp = tmp->free_next;
	}

	if (tmp == head) {
		/* i-node it's not on free list, pick up any i-node */
		tmp = head->free_next;
	}

	/* remove i-node from free list */
	cli();
	prev = tmp->free_prev;
	next = tmp->free_next;
	prev->free_next = next;
	next->free_prev = prev;
	sti();

	/* Initialize i-node */
	tmp->device.major = sb->device.major;
	tmp->device.minor = sb->device.minor;
	tmp->sb = sb;
	tmp->number = number;
	tmp->reference = 1;
	/* Read i-node from device */
	sb->sb_op->get_inode(tmp);

	return tmp;
}

/**
 * Remove i-node from old hash queue and inser onto new hash queue.
 *
 * \param inode i-node.
 */
static void add_inode_htable(vfs_inode *inode)
{
	vfs_inode *head, *tmp, *prev, *next;
	uint32_t pos;

	/* Find position based on i-node number */
	pos = inode->number % INODE_HASH_TABLE_SIZE;
	head = inode_hash_table[pos];

	if (head != NULL) {
		/* Search and remove from hash queue */
		tmp = head;
		if (tmp->next == NULL) {
			inode_hash_table[pos] = NULL;
		} else {
			while (tmp->next != NULL) {
				if ( DEV_CMP(tmp->device, inode->device) ) {
					cli();
					prev = tmp->prev;
					if (tmp->next == NULL) {
						next = NULL;
					} else {
						next = tmp->next;
						next->prev = prev;
					}
					prev->next = next;
					sti();
				}
				tmp = tmp->next;
			}
		}
	}

	/* Add to new hash queue */
	pos = inode->number % INODE_HASH_TABLE_SIZE;
	head = inode_hash_table[pos];

	if (head == NULL) {
		inode->next = NULL;
		inode->prev = NULL;
		inode_hash_table[pos] = inode;
		return;
	} else if (head->next == NULL) {
		cli();
		head->next = inode;
		head->prev = NULL;
		inode->next = NULL;
		inode->prev = head;
		sti();
		return;
	}

	tmp = head;
	while(tmp->next != NULL)
		tmp = tmp->next;

	cli();
	inode->prev = tmp;
	inode->next = NULL;
	tmp->next   = inode;
	sti();
}

/**
 * Get i-node from hash table. Read from disk if i-node it's not at memory.
 *
 * \param sb Super block of associated file system.
 * \param number i-node number.
 */
vfs_inode *vfs_iget(vfs_superblock *sb, uint32_t number)
{
	vfs_inode *inode;
	vfs_superblock *i_sb = sb;

	while(1) {
	
		if ( (inode = search_inode(i_sb->device, number)) != NULL ) {
			
			/* Block is in hash table, check if is locked */
			if ( mutex_is_locked(inode->lock) ) {
				sleep_on(WAIT_INODE_BECOMES_UNLOCKED);
				continue;
			}

			/* Special processing for mount points */
			if ( (inode->flags & IFLAG_MOUNT_POINT) ) {
				/* TODO: mount point processing */
			}

			inode_remove_from_freelist(inode);
			return inode;

		} else {
			/* i-node is not on hash table */
			
			/* get new free i-node and initialize it */
			if ((inode = get_free_inode(i_sb, number)) == NULL) {
				panic("VFS: free i-nodes list empty!");
			}

			add_inode_htable(inode);

			return inode;
		}
	}
}

/**
 *
 * Translate file byte offset into file system block number, block offset, etc.
 *
 * \param inode File i-node.
 * \param offset File byte offset.
 * \return vfs_bmap_t Structure with converted numbers.
 */
vfs_bmap_t vfs_bmap(vfs_inode *inode, uint32_t offset)
{
	uint32_t blk_size;
	uint32_t blk_ind_size;
	uint32_t div, quo, ind;
	vfs_bmap_t bmap;
	char *block;
	uint32_t *ind_block, baddr, b_dind_size;

	/* Block size (in bytes) */
	blk_size = inode->sb->s_log_block_size;

	/* Check if block is in direct blocks */
	div = offset;
	quo = (div / blk_size);
	if (quo < VFS_NDIR_BLOCKS) {
		ind = quo;
		bmap.blk_number = inode->i_block[ind];
		bmap.blk_offset = div - (quo * blk_size);
		bmap.blk_breada = 0;
		return bmap;
	}
	
	/* Calculate how many entries indirect blocks have */
	blk_ind_size = blk_size / sizeof(inode->i_block[0]);

	/* Check if block is single indirect block */
	div -= (quo * blk_size);
	quo  = (div / blk_size);
	if (quo < (blk_ind_size * blk_size)) {
		ind = quo / blk_ind_size;

		/* Read indirect block */
		block = inode->sb->sb_op->get_fs_block(inode->sb, inode->i_block[VFS_IND_BLOCK]);

		/* Read indirect block entry */
		ind_block = (uint32_t*)block;
		baddr = ind_block[ind];
		kfree(block);

		bmap.blk_number = baddr;
		bmap.blk_offset = div - (quo * blk_size);
		bmap.blk_breada = 0;
		return bmap;
	}

	/* Check if block is double indirect block */
	/* TODO: finish implement double indirect search block */
	b_dind_size = _ipow(blk_ind_size, 2);
	div -= (quo * b_dind_size * blk_size);
	quo  = (div / blk_size);
	if (quo < b_dind_size) {
		ind = quo / b_dind_size;

		/* Read double indirect block */
		block = inode->sb->sb_op->get_fs_block(inode->sb, inode->i_block[VFS_DIND_BLOCK]);
		/* Read double first indirect block entry */
		ind_block = (uint32_t*)block;
		baddr = ind_block[ind];
		kfree(block);

		/* Calculate and read second index */
		

		bmap.blk_number = baddr;
		bmap.blk_offset = div - (quo * blk_size);
		bmap.blk_breada = 0;
		return bmap;
	}

	/* Block is in triple indirect block */
	/* TODO: implement triple indirect search block. */

	return bmap;
}

