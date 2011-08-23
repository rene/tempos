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

int parse_mbr(char *user_mbr)
{
	int i;
	partition_st *part, *epart;
	mbr_st mbr;
	ebr_st ebr;

	memcpy(&mbr, user_mbr, sizeof(mbr));

	if (mbr.boot_signature[0] != 0x55 || mbr.boot_signature[1] != 0xaa) {
		return -1;
	}
	
	kprintf("DiskID: 0x%x\n", (mbr.diskid_high << 4) | mbr.diskid_low);

	kprintf("Part. BF   SH    SS     SC  EH   ES   EC  LFS  TS\n");
	for (i = 0; i < 4; i++) {
		part = &mbr.partition[i];
		kprintf("%d    ", i+1);
		kprintf("%x  ", part->bootable);
		kprintf("%d  ", part->starting_head);
		kprintf("%d  ", part->starting_sector);
		kprintf("%d  ", part->starting_cylinder);
		kprintf("%d  ", part->ending_head);
		kprintf("%d  ", part->ending_sector);
		kprintf("%d  ", part->ending_cylinder);
		kprintf("%d  ", part->LBA_first_sector);
		kprintf("%d  \n", part->total_sectors);
	
		/*
		if (part->sysid == 0x05 || part->sysid == 0x0f) {
			printf("Extendida    ");
			
			* Faz a leitura das partições extendidas *
			epart = part;
			j = 5;
			while(epart->sysid != 0) {
				printf("\n");

				* Lê o primeiro EBR da partição extendida *
				lseek(fd, ((unsigned long)epart->LBA_first_sector*512), SEEK_SET);
				read(fd, &ebr, sizeof(ebr));
				
				fsector = epart->LBA_first_sector;

				if (ebr.boot_signature[0] != 0x55 && ebr.boot_signature[1] != 0xaa) {
					printf("    Código de Boot NÃO encontrado!\n");
				} else {
					epart = &ebr.partition;
					printf("%d    ", j);
					printf("%x  ", epart->bootable);
					printf("%d  ", epart->starting_head);
					printf("%d  ", epart->starting_sector);
					printf("%d  ", epart->starting_cylinder);
					printf("%d  ", epart->ending_head);
					printf("%d  ", epart->ending_sector);
					printf("%d  ", epart->ending_cylinder);
					printf("%d  ", epart->LBA_first_sector);
					printf("%d  ", epart->total_sectors);
					printf("0x%x", epart->sysid);
				}
				
				epart = &ebr.next_ebr;
				epart->LBA_first_sector += fsector;
				j++;
			};

		} else if (part->sysid == 0x00) {
			printf("VAZIA        ");
		} else {
			printf("0x%x", part->sysid);
		}
		printf("\n");*/
	}

	return 0;
}

