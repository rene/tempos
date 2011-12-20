/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: vfs.h
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

/**
 * TempOS Virtual File System structures and functions.
 */
#ifndef VFS_H

	#define VFS_H

	#include <unistd.h>
	#include <sys/stat.h>
	#include <fs/bhash.h>
	#include <fs/device.h>
	#include <semaphore.h>

	/** Maximum file name length */
	#define VFS_NAME_LEN 512
	
	/** Maximum open files */
	#define VFS_MAX_OPEN_FILES 32767

	/**
	 * Super Block structure
	 */
	struct _vfs_superblock_st {
		/** Count of inodes in the filesystem */
		uint32_t s_inodes_count;
		/** Count of blocks in the filesystem */
		uint32_t s_blocks_count;
		/** Count of the number of free blocks */
		uint32_t s_free_blocks_count;   
		/** Count of the number of free inodes */
		uint32_t s_free_inodes_count;
		/** Indicator of the block size */
		uint32_t s_log_block_size;  	
		/** The time that the filesystem was last mounted */
		uint32_t s_mtime;
		/** The time that the filesystem was last written to */
		uint32_t s_wtime;
		/** The number of times the file system has been mounted */
		uint16_t s_mnt_count;
		/** File System Magic number */
		uint16_t s_magic;
		/** Flags indicating the current state of the filesystem */
		uint16_t s_state;
		/** Flags indicating the procedures for error reporting */
		uint16_t s_errors;
		/** The time that the filesystem was last checked */
		uint32_t s_lastcheck;
		/** The maximum time permissible between checks */
		uint32_t s_checkinterval;		
		/** 128-bit uuid for volume */
		uchar8_t s_uuid[16];
		/** volume name */
		uchar8_t s_volume_name[16];
	};

	/**
	 * i-node structure
	 */
	struct _vfs_inode_st {
		/** File mode */
		uint16_t i_mode;               
		/** Owner uid */
		uint16_t i_uid;                
		/** File/dir size in bytes */
		uint32_t i_size;               
		/** Last access time */
		uint32_t i_atime;              
		/** Creation time */
		uint32_t i_ctime;              
		/** Last modification time */
		uint32_t i_mtime;              
		/** Group id */
		uint16_t i_gid;                
		/** Number of (hard) links to this inode */
		uint16_t i_links_count;        
		/** Number of blocks used */
		uint32_t i_blocks;             
		/** File flags */
		uint32_t i_flags;              
		/** Blocks table */
		uint32_t i_block[15];
		
		/* attributes present only at memory */
		
		/** Lock for operations */
		sem_t lock;
		/** Device which i-node belongs */
		dev_t device;
		/** Reference count */
		int reference;
		/** i-node number */
		uint32_t number;
	};

	/**
	 * Directory structure
	 */
	struct _vfs_directory_st {
		/** i-node number */
		uint32_t inode;						
		/** Directory entry length */
		uint16_t rec_len;
		/** Name length */
		uchar8_t name_len;			
		/** File Type */
		uchar8_t file_type;
		/** File name */
		char name[VFS_NAME_LEN];
	};

	
	typedef struct _vfs_superblock_st vfs_superblock;
	typedef struct _vfs_inode_st      vfs_inode;
	typedef struct _vfs_directory_st  vfs_directory;


	/* Prototypes */
 
	/** Register all know File System types */
	void register_all_fs_types(void);

#endif /* VFS_H */

