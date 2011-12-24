/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ext2.c
 * Desc: Implements the Extended File System Version 2 (EXT2) File System
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

#include <fs/vfs.h>
#include <fs/ext2/ext2.h>
#include <fs/bhash.h>
#include <string.h>


/** ext2 fs type */
vfs_fs_type ext2_fs_type;

/** ext2 super block operations */
vfs_sb_ops ext2_sb_ops;


/* Prototypes */
int check_is_ext2(dev_t device);

int ext2_get_sb(dev_t device, vfs_superblock *sb);

int ext2_get_inode(vfs_inode *inode);


/**
 * This function registers EXT2 file system in VFS.
 */
void register_ext2(void)
{
	ext2_fs_type.name = "ext2";
	ext2_fs_type.check_fs_type = check_is_ext2;
	ext2_fs_type.get_sb = ext2_get_sb;

	ext2_sb_ops.get_inode = ext2_get_inode;

	register_fs_type(&ext2_fs_type);
}

/**
 * Check if some device is formated as EXT2
 *
 * \param device Device
 * \return 1 if device is formated as EXT2, 0 otherwise.
 */
int check_is_ext2(dev_t device)
{
	buff_header_t *blk;
	ext2_superblock_t sb;

	blk = bread(device.major, device.minor, EXT2_SUPERBLOCK_SECTOR);

	memcpy(&sb, blk->data, sizeof(ext2_superblock_t));

	if (sb.s_magic == EXT2_MAGIC) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * Read EXT2 super block.
 *
 * \param device Device.
 * \param sb Super block.
 * \return 1 on success. 0 otherwise.
 */
int ext2_get_sb(dev_t device, vfs_superblock *sb)
{
	buff_header_t *sb_blks[2];
	ext2_superblock_t *ext2_sb;
	char tmp[1024];

	ext2_sb = (ext2_superblock_t*)kmalloc(sizeof(ext2_superblock_t), GFP_NORMAL_Z);
	if (ext2_sb == NULL) {
		return 0;
	}

	sb_blks[0] = bread(device.major, device.minor, EXT2_SUPERBLOCK_SECTOR);
	sb_blks[1] = bread(device.major, device.minor, EXT2_SUPERBLOCK_SECTOR+1);

	/* Keep EXT2 super block in memory */
	memcpy(tmp, sb_blks[0]->data, 512);
	memcpy(&tmp[512], sb_blks[1]->data, 512);
	memcpy(ext2_sb, tmp, 1024);
	sb->fs_driver = ext2_sb;
	
	brelse(device.major, device.minor, sb_blks[0]);
	brelse(device.major, device.minor, sb_blks[1]);

	/* Now, fill VFS super block */
	sb->s_inodes_count      = ext2_sb->s_inodes_count;
	sb->s_blocks_count      = ext2_sb->s_r_blocks_count;  
	sb->s_free_blocks_count = ext2_sb->s_free_blocks_count;  
	sb->s_free_inodes_count = ext2_sb->s_free_inodes_count; 
	sb->s_log_block_size    = ext2_sb->s_log_block_size;  
	sb->s_mtime             = ext2_sb->s_mtime; 			
	sb->s_wtime             = ext2_sb->s_wtime; 			
	sb->s_mnt_count         = ext2_sb->s_mnt_count;  		
	sb->s_state             = ext2_sb->s_state; 			
	sb->s_errors            = ext2_sb->s_errors;  		
	sb->s_lastcheck         = ext2_sb->s_lastcheck;		
	sb->s_checkinterval     = ext2_sb->s_checkinterval;	
	
	strcpy((char*)sb->s_uuid, (char*)ext2_sb->s_uuid);
	strcpy((char*)sb->s_volume_name, (char*)ext2_sb->s_volume_name);

	sb->type = &ext2_fs_type;
	sb->device = device;
	sb->sb_op = &ext2_sb_ops;

	return 1;
}

/**
 * Get an i-node from disk
 *
 * \param inode i-node structure, where information will be store.
 * \return 1 on success. 0 otherwise.
 */
int ext2_get_inode(vfs_inode *inode)
{
	kprintf("\next2: get inode: %ld\n", inode->number);

	return 0;
}

/**
 * Division a/b with rounded up.
 * \param a Value of a.
 * \param b Value of b.
 * \return Result of division a/b with rounded up.
 **/
uint32_t div_rup(uint32_t a, uint32_t b)
{
	uint32_t res = (a/b);
	if ((res*b) < a) {
		res++;
	}
	return res;
}


/**
 * Return block size in bytes.
 * \param sb Super block.
 * \return Block size in bytes.
 */
uint32_t get_block_size(ext2_superblock_t sb)
{
	uint32_t size = 1024; /* default */

	switch(sb.s_log_block_size) {
		case EXT2_BLOCK_SIZE_2k:
			size = 2048;
			break;

		case EXT2_BLOCK_SIZE_4k:
			size = 4096;
			break;
	}

	return size;
}

