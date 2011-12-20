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

/*
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

	/**
	 * EXT2 Super Block structure
	 */
	struct _superblock_st {
		/** Count of inodes in the filesystem */
		uint32_t s_inodes_count;  	
		/** Count of blocks in the filesystem */
		uint32_t s_blocks_count; 	
		/** Count of the number of reserved blocks */
		uint32_t s_r_blocks_count;  
		/** Count of the number of free blocks */
		uint32_t s_free_blocks_count;  
		/** Count of the number of free inodes */
		uint32_t s_free_inodes_count; 
		/** The first block which contains data */
		uint32_t s_first_data_block;  
		/** Indicator of the block size */
		uint32_t s_log_block_size;  
		/** Indicator of the size of the fragments */
		int16_t  s_log_frag_size;	
		/** Count of the number of blocks in each block group */
		uint32_t s_blocks_per_group;  
		/** Count of the number of fragments in each block group */
		uint32_t s_frags_per_group;  
		/** Count of the number of inodes in each block group */
		uint32_t s_inodes_per_group;  
		/** The time that the filesystem was last mounted */
		uint32_t s_mtime; 			
		/** The time that the filesystem was last written to */
		uint32_t s_wtime; 			
		/** The number of times the file system has been mounted */
		uint16_t s_mnt_count;  		
		/** The number of times the file system can be mounted */
		int16_t  s_max_mnt_count; 	
		/** Magic number indicating ex2fs */
		uint16_t s_magic;  			
		/** Flags indicating the current state of the filesystem */
		uint16_t s_state; 			
		/** Flags indicating the procedures for error reporting */
		uint16_t s_errors;  		
		/** padding */
		uint16_t s_pad;  			
		/** The time that the filesystem was last checked */
		uint32_t s_lastcheck;		
		/** The maximum time permissible between checks */
		uint32_t s_checkinterval;	
		/** Indicator of which OS created the filesystem */
		uint32_t s_creator_os;		
		/** The revision level of the filesystem */
		uint32_t s_rev_level;		
		/** The default uid for reserved blocks */
		uint16_t s_def_resuid;		
		/** The default gid for reserved blocks */
		uint16_t s_def_resgid;		
		
		/* Extended attributes */
	
		/** First non-reserved inode */
		uint32_t s_first_ino;		
		/** size of inode structure */
		uint16_t s_inode_size; 		
		/** block group # of this superblock */
		uint16_t s_block_group_nr; 	
		/** compatible feature set */
		uint32_t s_feature_compat; 	
		/** incompatible feature set */
		uint32_t s_feature_incompat; 
		/** readonly-compatible feature set */
		uint32_t s_feature_ro_compat; 
		/** 128-bit uuid for volume */
		uchar8_t s_uuid[16];		
		/** volume name */
		uchar8_t s_volume_name[16]; 
		/** directory where last mounted */
		uchar8_t s_last_mounted[64]; 
		/** For compression */
		uint32_t s_algorithm_usage_bitmap;
		/* --- */
		/** Padding to 1024 bytes */
		uint32_t s_reserved[205];  	
	};

	/**
	 * EXT2 Group Descriptor structure
	 */
	struct _group_descriptor_st {
		/** Block number of the block bitmap */
		uint32_t bg_block_bitmap;      
		/** Block number of the inode bitmap */
		uint32_t bg_inode_bitmap;      
		/** Block number of the inode table */
		uint32_t bg_inode_table;       
		/** Free blocks in the group */
		uint16_t bg_free_blocks_count; 
		/** Free inodes in the group */
		uint16_t bg_free_inodes_count; 
		/** Number of directories in the group */
		uint16_t bg_used_dirs_count;   
		/** Padding */
		uint16_t bg_pad;
		/** Padding */
		uint32_t bg_reserved[3];
	};

	/**
	 * EXT2 i-node structure
	 */
	struct _inode_st {
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
		/** Deletion time */
		uint32_t i_dtime;              
		/** Group id */
		uint16_t i_gid;                
		/** Number of (hard) links to this inode */
		uint16_t i_links_count;        
		/** Number of blocks used */
		uint32_t i_blocks;             
		/** File flags */
		uint32_t i_flags;
		/** OS dependent */
		uint32_t i_reserved1;
		/** Blocks table */
		uint32_t i_block[15];          
		/** File version (for NFS) */
		uint32_t i_version;            
		/** File access control list */
		uint32_t i_file_acl;           
		/** Directory access control list */
		uint32_t i_dir_acl;            
		/** Fragment address */
		uint32_t i_faddr;              
		/** Fragments count*/
		uchar8_t i_frag;               
		/** Fragment size */
		uchar8_t i_fsize;
		/** Padding */
		uint16_t i_pad1;
		/** Reserved */
		uint32_t i_reserved2[2];
	};

	/**
	 * EXT2 Directory structure
	 */
	struct _directory_st {
		/** Inode number */
		uint32_t inode;			
		/** Directory entry length */
		uint16_t rec_len;		
		/** Name length */
		uchar8_t name_len;		
		/** File Type */
		uchar8_t file_type;		
		/** File name */
		char name[EXT2_NAME_LEN];
	};


	typedef struct _superblock_st superblock_st;
	typedef struct _group_descriptor_st group_st;
	typedef struct _inode_st inode_st;
	typedef struct _directory_st directory_st;


	/* Prototypes */
	uint32_t div_rup(uint32_t a, uint32_t b);
	uint32_t get_block_size(superblock_st sb);

#endif /* VFS_FS_EXT2 */

