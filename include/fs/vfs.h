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

	/** Compare to devices */
	#define DEV_CMP(d1, d2)	((d1.major == d2.major && d1.minor == d2.minor) ? 1 : 0)

	/** Maximum file name length */
	#define VFS_NAME_LEN 512
	
	/** Maximum open files */
	#define VFS_MAX_OPEN_FILES 4000

	/** Maximum file system mounted */
	#define VFS_MAX_MOUNTED_FS 512

	/** I-node hash table entries */
	#define INODE_HASH_TABLE_SIZE VFS_MAX_OPEN_FILES

	/** Number of file systems supported by TempOS */
	#define VFS_SUPPORTED_FS 1

	/** i-node is a mount point */
	#define IFLAG_MOUNT_POINT  0x01
	/** Head of linked list */
	#define IFLAG_LIST_HEAD    0x02

	/** 
	 * As EXT2, TempOS VFS i-nodes has 15 addressing blocks.
	 * 12 - Direct blocks
	 * 1  - Single indirect block 
	 * 1  - Double 
	 * 1  - Triple indirect block
	 */
	#define VFS_NDIR_BLOCKS     12
	#define	VFS_IND_BLOCK       VFS_NDIR_BLOCKS
	#define	VFS_DIND_BLOCK      (VFS_IND_BLOCK + 1)
	#define	VFS_TIND_BLOCK      (VFS_DIND_BLOCK + 1)


	/**
	 * Super Block structure
	 */
	struct _vfs_superblock_st {
		/** Count of i-nodes in the file system */
		uint32_t s_inodes_count;
		/** Count of blocks in the file system */
		uint32_t s_blocks_count;
		/** Count of the number of free blocks */
		uint32_t s_free_blocks_count;   
		/** Count of the number of free i-nodes */
		uint32_t s_free_inodes_count;
		/** Indicator of the block size */
		uint32_t s_log_block_size;  	
		/** The time that the file system was last mounted */
		uint32_t s_mtime;
		/** The time that the file system was last written to */
		uint32_t s_wtime;
		/** The number of times the file system has been mounted */
		uint16_t s_mnt_count;
		/** File System type */
		struct _vfs_fs_type_st *type;
		/** Flags indicating the current state of the file system */
		uint16_t s_state;
		/** Flags indicating the procedures for error reporting */
		uint16_t s_errors;
		/** The time that the file system was last checked */
		uint32_t s_lastcheck;
		/** The maximum time permissible between checks */
		uint32_t s_checkinterval;		
		/** 128-bit uuid for volume */
		uchar8_t s_uuid[16];
		/** volume name */
		uchar8_t s_volume_name[16];

		/* attributes present only at memory */

		/** Device */
		dev_t device;
		/** Flags */
		uint16_t flags;
		/** Super block operations for this kind of file system */
		struct _vfs_sb_operations *sb_op;
		/** For the use of file system driver */
		void *fs_driver;
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
		/** Number of (hard) links to this i-node */
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
		/** Flags */
		uint16_t flags;
		/** Reference count */
		int reference;
		/** i-node number */
		uint32_t number;
		/** links to make a double linked list */
		struct _vfs_inode_st *prev;
		struct _vfs_inode_st *next;
		/** links to free list */
		struct _vfs_inode_st *free_next;
		struct _vfs_inode_st *free_prev;
		/** Associated super block */
		struct _vfs_superblock_st *sb;
		/** For the use of file system driver */
		void *fs_driver;
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
		/** Super block of file */
		struct _vfs_superblock_st *d_sb;
	};

	/**
	 * File system type
	 */
	struct _vfs_fs_type_st {
		/** File System name */
		char *name;
		/** Check if such device is this file system valid */
		int (*check_fs_type) (dev_t);
		/** Get super block from file system */
		int (*get_sb) (dev_t, struct _vfs_superblock_st *);
	};

	/**
	 * Mount table entry
	 */
	struct _vfs_mount_table_entry {
		/** Mounted device */
		dev_t device;
		/** Super block of mounted device */
		struct _vfs_superblock_st sb;
		/** Root i-node of mounted device */
		struct _vfs_inode_st *root_inode;
		/** Directory i-node where device is mounted on */
		struct _vfs_inode_st *mnt_on_inode;
		/** File system type */
		struct _vfs_fs_type_st *fs;
		/** Device root name */
		char *root_name;
		/** Directory name */
		char *mnt_on_name;
	};

	/**
	 * File structure. Represents a file only at system runtime.
	 */
	struct _vfs_file_st {
		/** File i-node */
		struct _vfs_inode_st *inode;
	};

	/**
	 * Super block operations
	 */
	struct _vfs_sb_operations {
		/** get an i-node from disk */
		int (*get_inode) (struct _vfs_inode_st *);
		/** Update disk i-node with current information */
		int (*write_inode) (struct _vfs_inode_st *);
		/** write an i-node to disk and free i-node object */
		int (*put_inode) (struct _vfs_inode_st *);
		/** Alloc new i-node */
		int (*alloc_inode) (struct _vfs_superblock_st *, struct _vfs_inode_st *);
		/** Delete a i-node and all block associated with it */
		int (*free_inode) (struct _vfs_superblock_st *, struct _vfs_inode_st *);
		/** Update super block disk with current information */
		int (*write_super) (struct _vfs_superblock_st*);
	};


	typedef struct _vfs_superblock_st 		vfs_superblock;
	typedef struct _vfs_inode_st      		vfs_inode;
	typedef struct _vfs_directory_st  		vfs_directory;
	typedef struct _vfs_mount_table_entry 	vfs_mount_table;
	typedef struct _vfs_fs_type_st			vfs_fs_type;
	typedef struct _vfs_file_st				vfs_file;
	typedef struct _vfs_sb_operations		vfs_sb_ops;


	/** Global free i-nodes queue */
	extern vfs_inode *free_inodes_head;

	/** Global mount table */
	extern vfs_mount_table mount_table[VFS_MAX_MOUNTED_FS];

	/** Global system's file table */
	extern vfs_file *file_table;

	/** Supported file systems */
	extern vfs_fs_type *vfs_filesystems[VFS_SUPPORTED_FS];


	/* Prototypes */
 
	void register_all_fs_types(void);

	void register_fs_type(vfs_fs_type *type);

	int vfs_mount_root(dev_t device);

	vfs_inode *vfs_iget(vfs_superblock *sb, uint32_t number);

#endif /* VFS_H */

