/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ext2.h
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
 * The structures declared here are based on John's spec 
 * of the second extended filesystem. For more information,
 * see http://uranus.chrysocome.net/explore2fs/es2fs.htm
 */
#ifndef VFS_FS_EXT2

	#define VFS_FS_EXT2

	#include <unistd.h>

	#define EXT2_MAGIC		0xEF53

	#define SUPERBLOCK_OFFSET 1024

	#define BLOCK_SIZE_1k	0
	#define BLOCK_SIZE_2k	1
	#define BLOCK_SIZE_4k	2

	#define BLOCK_ST_FREE	   0x00
	#define BLOCK_ST_ALLOCATED 0x01

	#define EXT2_NDIR_BLOCKS 12
	#define EXT2_IND_BLOCKS   2

	#define EXT2_NAME_LEN 255

	/* File mode flags */
	//#define S_IFMT 0xF000 /* format mask */
	//#define S_IFSOCK 0xA000 /* socket */ 
	//#define S_IFLNK 0xC000 /* symbolic link */
	//#define S_IFREG 0x8000 /* regular file */
	//#define S_IFBLK 0x6000 /* block device */
	//#define S_IFDIR 0x4000 /* directory */
	//#define S_IFCHR 0x2000 /* character device */
	//#define S_IFIFO 0x1000 /* fifo */
	//#define S_ISUID 0x0800 /* SUID */
	//#define S_ISGID 0x0400 /* SGID */
	//#define S_ISVTX 0x0200 /* sticky bit */
	//#define S_IRWXU 0x01C0 /* user mask */
	//#define S_IRUSR 0x0100 /* read */
	//#define S_IWUSR 0x0080 /* write */
	//#define S_IXUSR 0x0040 /* execute */
	//#define S_IRWXG 0x0038 /* group mask */
	//#define S_IRGRP 0x0020 /* read */
	//#define S_IWGRP 0x0010 /* write */
	//#define S_IXGRP 0x0008 /* execute */
	//#define S_IRWXO 0x0007 /* other mask */
	//#define S_IROTH 0x0004 /* read */
	//#define S_IWOTH 0x0002 /* write */
	//#define S_IXOTH 0x0001 /* execute */

	/**
	 * Super Block structure
	 */
	struct _superblock_st {
		uint32_t s_inodes_count;  		/* Count of inodes in the filesystem */
		uint32_t s_blocks_count; 		/* Count of blocks in the filesystem */
		uint32_t s_r_blocks_count;  	/* Count of the number of reserved blocks */
		uint32_t s_free_blocks_count;   /* Count of the number of free blocks */
		uint32_t s_free_inodes_count; 	/* Count of the number of free inodes */
		uint32_t s_first_data_block;  	/* The first block which contains data */
		uint32_t s_log_block_size;  	/* Indicator of the block size */
		int16_t  s_log_frag_size;		/* Indicator of the size of the fragments */
		uint32_t s_blocks_per_group;  	/* Count of the number of blocks in each block group */
		uint32_t s_frags_per_group;  	/* Count of the number of fragments in each block group */
		uint32_t s_inodes_per_group;  	/* Count of the number of inodes in each block group */
		uint32_t s_mtime; 				/* The time that the filesystem was last mounted */
		uint32_t s_wtime; 				/* The time that the filesystem was last written to */
		uint16_t s_mnt_count;  			/* The number of times the file system has been mounted */
		int16_t  s_max_mnt_count; 		/* The number of times the file system can be mounted */
		uint16_t s_magic;  				/* Magic number indicating ex2fs */
		uint16_t s_state; 				/* Flags indicating the current state of the filesystem */
		uint16_t s_errors;  			/* Flags indicating the procedures for error reporting */
		uint16_t s_pad;  				/* padding */
		uint32_t s_lastcheck;			/* The time that the filesystem was last checked */
		uint32_t s_checkinterval;		/* The maximum time permissible between checks */
		uint32_t s_creator_os;			/* Indicator of which OS created the filesystem */
		uint32_t s_rev_level;			/* The revision level of the filesystem */
		uint16_t s_def_resuid;			/* The default uid for reserved blocks */
		uint16_t s_def_resgid;			/* The default gid for reserved blocks */
		/* Extended attributes */
		uint32_t s_first_ino;			/* First non-reserved inode */
		uint16_t s_inode_size; 			/* size of inode structure */
		uint16_t s_block_group_nr; 		/* block group # of this superblock */
		uint32_t s_feature_compat; 		/* compatible feature set */
		uint32_t s_feature_incompat; 	/* incompatible feature set */
		uint32_t s_feature_ro_compat; 	/* readonly-compatible feature set */
		uchar8_t s_uuid[16];			/* 128-bit uuid for volume */
		uchar8_t s_volume_name[16]; 	/* volume name */
		uchar8_t s_last_mounted[64]; 	/* directory where last mounted */
		uint32_t s_algorithm_usage_bitmap; /* For compression */
		/* --- */
		uint32_t s_reserved[205];  		/* Padding to 1024 bytes */
	};

	/**
	 * Group Descriptor structure
	 */
	struct _group_descriptor_st {
		uint32_t bg_block_bitmap;       /* Block number of the block bitmap */
		uint32_t bg_inode_bitmap;       /* Block number of the inode bitmap */
		uint32_t bg_inode_table;        /* Block number of the inode table */
		uint16_t bg_free_blocks_count;  /* Free blocks in the group */
		uint16_t bg_free_inodes_count;  /* Free inodes in the group */
		uint16_t bg_used_dirs_count;    /* Number of directories in the group */
		uint16_t bg_pad;				/* Padding */
		uint32_t bg_reserved[3];		/* Padding */
	};

	/**
	 * i-node structure
	 */
	struct _inode_st {
		uint16_t i_mode;                /* File mode */
		uint16_t i_uid;                 /* Owner uid */
		uint32_t i_size;                /* File/dir size in bytes */
		uint32_t i_atime;               /* Last access time */
		uint32_t i_ctime;               /* Creation time */
		uint32_t i_mtime;               /* Last modification time */
		uint32_t i_dtime;               /* Deletion time */
		uint16_t i_gid;                 /* Group id */
		uint16_t i_links_count;         /* Number of (hard) links to this inode */
		uint32_t i_blocks;              /* Number of blocks used */
		uint32_t i_flags;               /* File flags */
		uint32_t i_reserved1;			/* OS dependent */
		uint32_t i_block[15];           /* Blocks table */
		uint32_t i_version;             /* File version (for NFS) */
		uint32_t i_file_acl;            /* File access control list */
		uint32_t i_dir_acl;             /* Directory access control list */
		uint32_t i_faddr;               /* Fragment address */
		uchar8_t i_frag;                /* Fragments count*/
		uchar8_t i_fsize;               /* Fragment size */
		uint16_t i_pad1;
		uint32_t i_reserved2[2];
	};

	/**
	 * Directory structure
	 */
	struct _directory_st {
		uint32_t inode;				/* Inode number */
		uint16_t rec_len;			/* Directory entry length */
		uchar8_t name_len;			/* Name length */
		uchar8_t file_type;			/* File Type */
		char name[EXT2_NAME_LEN];	/* File name */
	};


	typedef struct _superblock_st superblock_st;
	typedef struct _group_descriptor_st group_st;
	typedef struct _inode_st inode_st;
	typedef struct _directory_st directory_st;


	/* Prototypes */
	uint32_t div_rup(uint32_t a, uint32_t b);
	uint32_t get_block_size(superblock_st sb);

#endif /* VFS_FS_EXT2 */

