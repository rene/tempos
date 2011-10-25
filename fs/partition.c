/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: partition.c
 * Desc: Implements partition handler.
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

#include <string.h>
#include <tempos/kernel.h>
#include <fs/vfs.h>
#include <fs/partition.h>

/**
 * Parses the MBR table from block device.
 *
 * \param blk_drv Driver structure of block device.
 * \param device Device number.
 * \return The partition table (\see fs/partition.h)
 */
part_table_st *parse_mbr(dev_blk_driver_t blk_drv, int device)
{
	buff_header_t sec;
	mbr_st mbr;
	ebr_st ebr;
	part_table_st *ptable;
	part_st *part, *epart;
	partition_st *partitions;
	int i;
	uint32_t fsector, pos, count, exnum;


	/* Read MBR */
	sec.addr = 0;
	blk_drv.dev_ops->read_block(blk_drv.major, device, &sec);
	memcpy(&mbr, sec.data, sizeof(mbr));

	/* Check for boot signature */
	if (mbr.boot_signature[0] != 0x55 || mbr.boot_signature[1] != 0xaa) {
		return NULL;
	}
	
	/* Alloc partition table structure */
	ptable = (part_table_st*)kmalloc(sizeof(part_table_st), GFP_NORMAL_Z);
	if (ptable == NULL) {
		return NULL;
	}

	/* Disk ID */
	ptable->diskid_low  = mbr.diskid_low;
	ptable->diskid_high = mbr.diskid_high;

	/* First, count how many partition are in the disk (primary and extended) */
	count = 0;
	for (i = 0; i < 4; i++) {
		part = &mbr.partition[i];
		if (part->sysid != 0x00) {
			if (part->sysid == 0x05 || part->sysid == 0x0f) {
				/* Extended, read logic partitions  */
				epart = part;
				fsector = epart->LBA_first_sector;
				while(epart->sysid != 0) {

					/* Read the first EBR from extended partition */
					sec.addr = fsector;
					blk_drv.dev_ops->read_block(blk_drv.major, device, &sec);
					memcpy(&ebr, sec.data, sizeof(ebr));
				
					if (ebr.boot_signature[0] == 0x55 && ebr.boot_signature[1] == 0xaa) {
						epart = &ebr.partition;
						count++;
					}
				
					epart = &ebr.next_ebr;
					fsector += epart->LBA_first_sector;
				}
			}
			count++;
		}
	}

	/* Alloc memory for each partition of table */
	ptable->size = count;
	partitions = (partition_st*)kmalloc(sizeof(partition_st) * count, GFP_NORMAL_Z);
	if (partitions == NULL) {
		kfree(ptable);
		return NULL;
	}
	
	/* Now, parse partitions again to fill table with information */
	pos = 0;
	for (i = 0; i < 4; i++) {
		part = &mbr.partition[i];
		if (part->sysid != 0x00) {
			if (part->sysid == 0x05 || part->sysid == 0x0f) {
				/* Extended, read logic partitions  */
				partitions[pos].init   = part->LBA_first_sector;
				partitions[pos].length = part->total_sectors;
				partitions[pos].id     = part->sysid;
				partitions[pos].type   = PART_TYPE_EXTENDED;
				partitions[pos].number = i+1;
				pos++;

				exnum   = 5;
				epart   = part;
				fsector = epart->LBA_first_sector;
				while(epart->sysid != 0) {

					/* Read the first EBR from extended partition */
					sec.addr = fsector;
					blk_drv.dev_ops->read_block(blk_drv.major, device, &sec);
					memcpy(&ebr, sec.data, sizeof(ebr));
				
					if (ebr.boot_signature[0] == 0x55 && ebr.boot_signature[1] == 0xaa) {
						epart = &ebr.partition;
						partitions[pos].init   = fsector;
						partitions[pos].length = (uint64_t)epart->total_sectors;
						partitions[pos].id     = epart->sysid;
						partitions[pos].type   = PART_TYPE_LOGIC;
						partitions[pos].number = exnum++;
						pos++;
					}
				
					epart = &ebr.next_ebr;
					fsector += epart->LBA_first_sector;
				}
			} else {
				/* Primary */
				partitions[pos].init   = part->LBA_first_sector;
				partitions[pos].length = part->total_sectors;
				partitions[pos].id     = part->sysid;
				partitions[pos].type   = PART_TYPE_PRIMARY;
				partitions[pos].number = i+1;
				pos++;
			}
		}
	}
	ptable->partitions = partitions;

	return ptable;
}


/**
 * Print partition numbers from partition table.
 * \param ptable Partition table.
 * \param devstr String to precede the number (i.e. sdaX, hdaX, ..., where X is
 * the partition number).
 */
void print_partition_table(part_table_st *ptable, char *devstr)
{
	uint32_t i;
	partition_st *part;

	for (i = 0; i < ptable->size; i++) {
		part = &ptable->partitions[i];
		kprintf("%s%d ", devstr, part->number);
	}
}


/**
 * Translate partition block address to disk sector address.
 *
 * \param diskaddr Disk address (sector number converted from partition address).
 * \param ptable Partition table.
 * \param pnumber Partition number.
 * \param paddress Partition address.
 * \return 0 on success (it's a valid partition address), -1 otherwise.
 */
int translate_part_address(uint64_t *diskaddr, part_table_st *ptable, uint32_t pnumber, uint64_t paddress)
{
	uint32_t i;
	uint64_t newaddr;
	partition_st *part;

	for (i = 0; i < ptable->size; i++) {
		part = &ptable->partitions[i];
		if (part->number == pnumber) { 
			/* Check limits */
			if (paddress > part->length) {
				return -1;
			} else {
				newaddr = part->init + paddress;
				*diskaddr = newaddr;
				return 0;
			}
		}
	}
	
	return -1;
}

