/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: namei.c
 * Desc: Implements namei() function (VFS).
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
#include <tempos/sched.h>
#include <fs/vfs.h>
#include <string.h>

/* Prototypes */

static vfs_inode *_vfs_find_component(vfs_inode *inode, char *component);


/**
 * Convert path name to i-node.
 *
 * \param pathname Path name.
 * \return NULL if path name is invalid, or the i-node (locked) otherwise.
 */
vfs_inode *vfs_namei(const char *pathname)
{
	char comp[VFS_NAME_LEN], isroot;
	vfs_inode *inode;
	task_t *current_task;
	size_t i, start, clen;
	
	current_task = GET_TASK(cur_task);

	if (pathname[0] == '/') {
		inode  = current_task->i_root;
		isroot = 1; 
	} else {
		inode  = current_task->i_cdir;
		isroot = 0;
	}

	start = 1;
	for (i = 1; i <= strlen(pathname); i++) {
		if (pathname[i] == '/' || pathname[i] == '\0') {
			clen = i - start;
			strncpy(comp, &pathname[start], clen);
			start      = i+1;
			comp[clen] = '\0';

			if (strlen(comp) == 0) {
				continue;
			}

			if (strcmp(comp, "..") == 0 && isroot) {
				continue;
			}

			/* Find component at i-node directory */
			if ((inode = _vfs_find_component(inode, comp)) == NULL) {
				return NULL;
			}
		}
	}

	return inode;
}

/**
 * Find component directory entry in i-node.
 *
 * \param inode Direcroty i-node to search.
 * \param component Component name.
 * \return Component i-node (if was found), or NULL otherwise.
 */
static vfs_inode *_vfs_find_component(vfs_inode *inode, char *component)
{
	uint32_t dirsize, blk_size, pos, bpos, oldpos;
	char *block;
	vfs_directory dir;
	vfs_superblock *sb;
	vfs_bmap_t bmap;
	vfs_inode *newinode;

	/* Check if i-node is a directory */
	if ( !(inode->i_mode & S_IFDIR) ) {
		return NULL;
	} else {
		/* Get information */
		sb       = inode->sb;
		dirsize  = inode->i_size;
		blk_size = inode->sb->s_log_block_size;
	}

	/* Read directory contents */
	pos      = 0;
	bpos     = 0;
	newinode = NULL;
	while (pos < dirsize) {
		bmap = vfs_bmap(inode, pos);
		block = sb->sb_op->get_fs_block(sb, bmap.blk_number);
		if (block == NULL) {
			break;
		} else {
			bpos = bmap.blk_offset;
		}
		
		oldpos = bpos;
		while(bpos < blk_size) {
			/* Read information */
			memcpy(&dir.inode, &block[bpos], sizeof(uint32_t));
			bpos += sizeof(uint32_t);
			
			if (dir.inode == 0) {
				break;
			}

			memcpy(&dir.rec_len, &block[bpos], sizeof(uint16_t));
			bpos += sizeof(uint16_t);
			
			memcpy(&dir.name_len, &block[bpos], sizeof(uchar8_t));
			/* skip file type, not necessary here */
			bpos += 2 * sizeof(uchar8_t);
			
			memcpy(&dir.name[0], &block[bpos], dir.name_len);
			dir.name[dir.name_len] = '\0';

			oldpos += dir.rec_len;
			bpos = oldpos;

			if (strcmp(dir.name, component) == 0) {
				/* Component found! */
				newinode = vfs_iget(inode->sb, dir.inode);
				/* TODO: iput current inode */
				/* vfs_iput(inode->sb, inode); */
				break;
			}
		}

		pos += blk_size;
		kfree(block);
	}

	return newinode;
}

