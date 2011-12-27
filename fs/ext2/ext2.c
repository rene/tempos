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

#define SECTOR_SIZE 512

/** 
 * This structure is used only by EXT2 driver to keep information about the
 * file system into the VFS structure.
 */
struct _ext2_fs_driver {
	/** ext2 superblock */
	struct _ext2_superblock_st *sb;
	/** group descriptor */
	struct _ext2_group_descriptor_st *gdesc;
	/** number of groups */
	uint32_t n_groups;
	/** size of blocks bitmap */
	uint32_t blks_bmap_size;
	/** size of i-node bitmap */
	uint32_t inodes_bmap_size;
	/** block size in sector units */
	uint32_t block_size;
};
typedef struct _ext2_fs_driver ext2_fsdriver_t;


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
	buff_header_t *blks[2];
	ext2_superblock_t *ext2_sb;
	ext2_group_t *ext2_gd;
	ext2_fsdriver_t *fsdriver;
	char tmp[2*SECTOR_SIZE];
	uint32_t grp_offset;

	fsdriver = (ext2_fsdriver_t*)kmalloc(sizeof(ext2_fs_type), GFP_NORMAL_Z); 
	ext2_sb  = (ext2_superblock_t*)kmalloc(sizeof(ext2_superblock_t), GFP_NORMAL_Z);
	ext2_gd  = (ext2_group_t*)kmalloc(sizeof(ext2_group_t), GFP_NORMAL_Z);
	if (ext2_sb == NULL || fsdriver == NULL || ext2_gd == NULL) {
		return 0;
	}

	blks[0] = bread(device.major, device.minor, EXT2_SUPERBLOCK_SECTOR);
	blks[1] = bread(device.major, device.minor, EXT2_SUPERBLOCK_SECTOR+1);

	/* Keep EXT2 super block in memory */
	memcpy(tmp, blks[0]->data, SECTOR_SIZE);
	memcpy(&tmp[SECTOR_SIZE], blks[1]->data, SECTOR_SIZE);
	memcpy(ext2_sb, tmp, (2*SECTOR_SIZE));
	
	/* Read EXT2 Group Descriptor and calculate FS information */
	fsdriver->block_size = get_block_size(*ext2_sb) / SECTOR_SIZE;

	grp_offset = ext2_sb->s_first_data_block * fsdriver->block_size;
	blks[0] = bread(device.major, device.minor, grp_offset + EXT2_SUPERBLOCK_SECTOR);
	memcpy(ext2_gd, blks[0]->data, sizeof(ext2_group_t));

	fsdriver->n_groups          = div_rup(ext2_sb->s_blocks_count, ext2_sb->s_blocks_per_group);
	fsdriver->blks_bmap_size    = div_rup(div_rup(ext2_sb->s_blocks_per_group, 8), get_block_size(*ext2_sb));
	fsdriver->inodes_bmap_size  = div_rup(div_rup(ext2_sb->s_inodes_per_group, 8), get_block_size(*ext2_sb));

	/* Keep enouth information of ext2 fs in memory */
	fsdriver->sb    = ext2_sb;
	fsdriver->gdesc = ext2_gd;
	sb->fs_driver   = fsdriver;
	
	brelse(device.major, device.minor, blks[0]);
	brelse(device.major, device.minor, blks[1]);

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
	uint32_t itab_addr, iblk, iblk_addr, grp_block, grp_number, number;
	ext2_fsdriver_t *fs;
	ext2_superblock_t *sb;
	buff_header_t *blk;
	ext2_inode_t inode_ext2;
	int i;

	/* get ext2 super block */
	fs = inode->sb->fs_driver;
	sb = fs->sb;

	/* i-node number */
	if (inode->number == 0) {
		/* get root i-node */
		number = EXT2_ROOT_INO;
	} else {
		number = inode->number;
	}

	/* Calculate which group i-node belongs */
	grp_number = div_rup((number - 1), sb->s_inodes_per_group);
	grp_block  = (sb->s_blocks_per_group * (grp_number-1));
	grp_block += fs->gdesc->bg_inode_table;

	if (sb->s_rev_level > 0) {
		/**
		 * Revision 1 and later reduce the number of backups of super block 
		 * by keeping a copy of super block only in groups 0, 1 and powers
		 * of 3, 5 and 7.
		 */
		if (grp_number != 1 && (grp_number % 3) != 0 &&
				(grp_number % 5) != 0 && (grp_number % 7) != 0) {
			grp_block--;
		}
	}

	/* Find i-node table block addr */
	itab_addr  = (grp_block * fs->block_size);
	iblk       = ((number - 1) * sizeof(ext2_inode_t));
	itab_addr += (iblk / SECTOR_SIZE);
	iblk_addr  = iblk - (iblk / SECTOR_SIZE);

	/* Read the i-node */
	blk = bread(inode->device.major, inode->device.minor, itab_addr);
	if (blk == NULL) {
		return 0;
	} else {
		memcpy(&inode_ext2, &blk->data[iblk_addr], sizeof(ext2_inode_t));
	}

	/* Now, fill VFS i-node with information */
	inode->i_mode        = inode_ext2.i_mode;
	inode->i_uid         = inode_ext2.i_uid;
	inode->i_size        = inode_ext2.i_size;
	inode->i_atime       = inode_ext2.i_atime;
	inode->i_ctime       = inode_ext2.i_ctime;
	inode->i_mtime       = inode_ext2.i_mtime;
	inode->i_gid         = inode_ext2.i_gid;
	inode->i_links_count = inode_ext2.i_links_count;
	inode->i_blocks      = inode_ext2.i_blocks;
	inode->i_flags       = inode_ext2.i_flags;
	for (i = 0; i < 15; i++) {
		inode->i_block[i] = inode_ext2.i_block[i];
	}

	return 1;
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

