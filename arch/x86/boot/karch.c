/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
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
#include <x86/karch.h>
#include <x86/multiboot.h>
#include <x86/gdt.h>
#include <x86/idt.h>
#include <x86/i8259A.h>
#include <x86/io.h>
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
	elf_section_header_table_t *elf_sec;
	memory_map_t *mmap;
	uint32_t i;
	char8_t *mtypes[] = { "Avaliable", "Reserved", "ACPI", "ACPI NVS" };


	/* TODO: start console */
    clrscr();
	setattr(LIGHT_GRAY);

	/* This is the first message from kernel :) */
	kprintf("TempOS\n");

	/* Multiboot data */
	if( magic != MULTIBOOT_BOOTLOADER_MAGIC ) {
		kprintf(KERN_CRIT "Invalid bootloader magic number: 0x%x\n", magic);
		return;
	}
	mboot_info = (multiboot_info_t *)addr;

	if( (mboot_info->flags & FLAG_ELF) ) {
		elf_sec = &(mboot_info->u.elf_sec);
		kprintf("Kernel start at: 0x%x\n", KERNEL_START_ADDR);
	}

	/* Get memory map */
	if( !(mboot_info->flags & FLAG_MMAP) ) {
		kprintf(KERN_CRIT "Unable to get memory information. Abort.\n");
		return;
	} else {
 		mmap  = (memory_map_t *)mboot_info->mmap_addr;

		i = 0;
		while( (ulong32_t)mmap < (mboot_info->mmap_addr + mboot_info->mmap_length) ) {

			if(mmap->size > sizeof(mmap_tentry)) {
				kprintf(KERN_CRIT "Incompatible size of memory structure, the results are unpredictable!\n");
			}

			kinf.mmap_table[i].base_addr_low  = mmap->base_addr_low;
			kinf.mmap_table[i].base_addr_high = mmap->base_addr_high;
			kinf.mmap_table[i].length_low     = mmap->length_low;
			kinf.mmap_table[i].length_high    = mmap->length_high;
			kinf.mmap_table[i].type           = mmap->type;

			kprintf("%0.12ld:%0.12ld - %s\n", kinf.mmap_table[i].base_addr_low,
						kinf.mmap_table[i].base_addr_low +
						kinf.mmap_table[i].length_low,
						mtypes[kinf.mmap_table[i].type - 1]);

			mmap = (memory_map_t *)((ulong32_t)mmap +
						mmap->size + sizeof(mmap->size));
			i++;
		}
		kinf.mmap_size = i;
	}

	/* Command line */
	if( (mboot_info->flags & FLAG_CMDLINE) ) {
		strcpy((char *)kinf.cmdline, (const char *)mboot_info->cmdline);
	} else {
		kinf.cmdline[0] = '\0';
	}

	/* Configure processor */
	setup_GDT();
	setup_IDT();
	init_PIC();
	init_mm();
	sti();

	/* Call the TempOS kernel */
	tempos_main(kinf);
}

