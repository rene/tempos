/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ata_generic.h
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

#ifndef  BLK_ATA_GENERIC_H

	#define BLK_ATA_GENERIC_H

	#include <unistd.h>
	#include <fs/bhash.h>

	/** Buffer size must have the same size of disk sectors */
	#define SECTOR_SIZE  	BUFF_SIZE

	#define ATA_DEVICE		0x8000
	#define REM_MEDIA		0x00FF

	#define SUPPORT_LBA		0x0300
	#define SUPPORT_DMA		0x0100
	#define SUPPORT_LBA48	0x0400

	#define PRESENT			0x01
	#define LBA48			0x02


	/**
	 * Device info
	 \verbatim
	 Flags format:
	  7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 
	  |   |   |   |   |   |   |   |
	  |   |   |   |   |   |   |   |---> PRESENT (0 = NO, 1 = YES)
	  |   |   |   |   |   |   |-------> LBA48   (0 = NO, 1 = YES)
	  |   |   |   |   |   |
	   --   NOT USED    --
	 \endverbatim
	 */
	struct _ata_dev_info {
		uchar8_t flags;
		uint64_t sectors;
		uint16_t type;
		char     serial[20];
		char     firmware_rev[8];
		char     model[40];
		uint16_t mult_secs;
		uint16_t max_secs[2];
		uint16_t capabilities[2];
		uint16_t mult_sec;
		uint16_t mword_dma;
		uint16_t major_ver;
		uint16_t minor_ver;
		uint16_t cmds_supported[6];
		uint16_t ultra_dma;
		uint16_t max_lba48[4];
	};

	typedef struct _ata_dev_info ata_dev_info;

	/* Prototypes */

	void init_ata_generic(void);

	int read_ata_sector(int major, int device, buff_header_t *buf);

	//int write_async_ata_block(buffer_header_t *buf);

	//int write_sync_ata_block(buffer_header_t *buf);

#endif /* BLK_ATA_GENERIC_H */

