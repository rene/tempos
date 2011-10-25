/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: partition.h
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

#ifndef VFS_PARTITION_H

	#define VFS_PARTITION_H

	#include <unistd.h>
	#include <fs/device.h>

	#define PART_TYPE_PRIMARY  0x01
	#define PART_TYPE_EXTENDED 0x02
	#define PART_TYPE_LOGIC    0x03


	/**
	 * Partition MBR entry structure
	 */
	struct _part_st {
		uchar8_t  bootable;
		uchar8_t  starting_head;
		uint16_t  starting_sector:6;
		uint16_t  starting_cylinder:10;
		uchar8_t  sysid;
		uchar8_t  ending_head;
		uint16_t  ending_sector:6;
		uint16_t  ending_cylinder:10;
		uint32_t  LBA_first_sector;
		uint32_t  total_sectors;
	} __attribute__((packed));

	/**
	 * MBR (Master Boot Record) structure
	 */
	struct _mbr_st {
		uchar8_t bootcode[440];
		uint32_t diskid_low;
		uint16_t diskid_high;
		struct _part_st partition[4];
		uchar8_t boot_signature[2];
	} __attribute__((packed));

	/**
	 * EBR (Extended Boot Record) structure
	 */
	struct _ebr_st {
		uchar8_t bootcode[446];
		struct _part_st partition;
		struct _part_st next_ebr;
		uchar8_t reserved[32];
		uchar8_t boot_signature[2];
	} __attribute__((packed));



	/**
	 * This is the generic partition structure used by TempOS.
	 * All disks will be parsed and their partitions will be store in this
	 * generic partition format. For while, TempOS needs to know only where
	 * partition starts, what is it's size (in sectors), and it's type.
	 */
	struct _partition_st {
		/* Partition system id */
		uchar8_t id;
		/* Partition type: Primary | Logic */
		uchar8_t type;
		/* Partition number */
		uint32_t number;
		/* First block of the partition */
		uint64_t init;
		/* Partition size (in sectors) */
		uint64_t length;
	};

	/**
	 * Partition table.
	 */
	struct _ptable_st {
		/* How many partitions are in the disk */
		uint32_t size;
		/* Vector of partitions */
		struct _partition_st *partitions;
		/* Disk ID */
		uint32_t diskid_low;
		/* Disk ID extension (usually nulls; 0x0000) */
		uint32_t diskid_high;
	};


	typedef struct _mbr_st       mbr_st;
	typedef struct _part_st 	 part_st;
	typedef struct _ebr_st		 ebr_st;
	typedef struct _partition_st partition_st;
	typedef struct _ptable_st    part_table_st;

	/* Prototypes */
	part_table_st *parse_mbr(dev_blk_driver_t blk_drv, int device);

	void print_partition_table(part_table_st *ptable, char *devstr);
	
	int translate_part_address(uint64_t *diskaddr, part_table_st *ptable, uint32_t pnumber, uint64_t paddress);

#endif /* VFS_PARTITION_H */

