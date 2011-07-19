/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: karch.c
 * Desc: First Stage: Implement the architecture dependent functions to 
 *       start the TempOS kernel.
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
#include <tempos/mm.h>
#include <x86/karch.h>
#include <x86/multiboot.h>
#include <x86/idt.h>
#include <x86/i8259A.h>
#include <x86/i82C54.h>
#include <x86/io.h>
#include <x86/irq.h>
#include <x86/mm.h>
#include <string.h>
#include "video.h" /* TODO: console */



/**
 * karch
 *
 * First Stage: Called from Boot Stage.
 */
void karch(unsigned long magic, unsigned long addr)
{
	karch_t kinf;
	multiboot_info_t *mboot_info;
	memory_map_t *mmap;
	uint32_t i, type;
	char8_t *mtypes[] = { "Avaliable", "Reserved", "ACPI", "ACPI NVS", "Unknown" };


	/**
	 * Here the address translation it's done by GDT trick, we need to
	 * take care about addresses passed by multiboot structures (physical)
	 * and translate them to virtual address
	 */

	/* Multiboot data */
	if( magic != MULTIBOOT_BOOTLOADER_MAGIC ) {
		return;
	}
	mboot_info = (multiboot_info_t *)VIRADDR((void*)addr);

	/* Memory information */
	if( !(mboot_info->flags & FLAG_MEM) ) {
		return;
	} else {
		kinf.mem_lower = mboot_info->mem_lower;
		kinf.mem_upper = mboot_info->mem_upper;
	}

	/* Get memory map */
	if( !(mboot_info->flags & FLAG_MMAP) ) {
		return;
	} else {
 		mmap  = (memory_map_t *)VIRADDR((void*)mboot_info->mmap_addr);

		i = 0;
		while( (uint32_t)mmap <
					(uint32_t)VIRADDR(mboot_info->mmap_addr + mboot_info->mmap_length) ) {

			kinf.mmap_table[i].base_addr_low  = mmap->base_addr_low;
			kinf.mmap_table[i].base_addr_high = mmap->base_addr_high;
			kinf.mmap_table[i].length_low     = mmap->length_low;
			kinf.mmap_table[i].length_high    = mmap->length_high;
			kinf.mmap_table[i].type           = mmap->type;

			mmap = (memory_map_t *)((uint32_t)mmap +
						mmap->size + sizeof(mmap->size));
			i++;
		}
		kinf.mmap_size = --i;
	}

	/* Command line */
	if( (mboot_info->flags & FLAG_CMDLINE) ) {
		strcpy((char *)kinf.cmdline, (const char *)VIRADDR((void*)mboot_info->cmdline));
	} else {
		kinf.cmdline[0] = '\0';
	}

	/**
	 * Still here we use the GDT trick to translate the virtual
	 * into physical address, now the first thing to do it's
	 * enable the paging system and reload de GDT with
	 * base 0, after that, we can continue to load the kernel
	 */
	init_pg(&kinf);

	/* Init the Memory Manager */
	init_mm();

	/* Setup interrupts */
	setup_IDT();
	init_PIC();
	init_IRQ();
	init_PIT();
	set_picmask(0x00, PIC_MASTER);
	set_picmask(0x00, PIC_SLAVE);
	sti();

	/* TODO: start console */
    clrscr();
	setattr(LIGHT_GRAY);

	/* This is the first message from kernel =:) */
	kprintf(KERN_INFO "TempOS\n");

	for(i=0; i<kinf.mmap_size; i++) {
		type = kinf.mmap_table[i].type - 1;

		if(type > 4)
			type = 4;

		kprintf(KERN_INFO "%0.9x:%0.9x - %s\n", kinf.mmap_table[i].base_addr_low,
					kinf.mmap_table[i].base_addr_low +
					kinf.mmap_table[i].length_low,
					mtypes[type]);
	}

	kprintf(KERN_INFO ">> First stage done.\n");

	/* Call the TempOS kernel */
	tempos_main(kinf);
}

