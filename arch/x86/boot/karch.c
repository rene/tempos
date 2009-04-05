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

	/* TODO: start console */
    clrscr();
	setattr(LIGHT_GRAY);

	/* Multiboot data */
	if( magic != MULTIBOOT_BOOTLOADER_MAGIC ) {
		kprintf(KERN_CRIT "Invalid bootloader magic number: 0x%x\n", magic);
		return;
	}
	mboot_info = (multiboot_info_t *)addr;

	if( !(mboot_info->flags & FLAG_ELF) ) {
		kprintf(KERN_CRIT "TempOS does not support other binary format to kernel than ELF.\n");
		return;
	} else {
		elf_sec = &(mboot_info->u.elf_sec);
		kprintf ("elf_sec: num = %u, size = 0x%x,"
		         " addr = 0x%x, shndx = 0x%x\n",
		        (unsigned) elf_sec->num, (unsigned) elf_sec->size,
				(unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);

		kprintf("Kernel start at: 0x%x\n", KERNEL_START_ADDR);
		kprintf("Kernel end at:   0x%x\n", KERNEL_END_ADDR);
	}

	if( !(mboot_info->flags & FLAG_MMAP) ) {
		kprintf(KERN_CRIT "Unable to get memory information. Abort.\n");
		return;
	} else {
		char *mtype[] = { "Avaliable", "Reserved", "ACPI", "ACPI NVS" };
		memory_map_t *mmap;

		kprintf("\nmmap_addr = 0x%0.4x, mmap_length = 0x%0.4x\n",
                   (unsigned) mboot_info->mmap_addr, (unsigned) mboot_info->mmap_length);
           for (mmap = (memory_map_t *) mboot_info->mmap_addr;
                (unsigned long) mmap < mboot_info->mmap_addr + mboot_info->mmap_length;
                mmap = (memory_map_t *) ((unsigned long) mmap
                                         + mmap->size + sizeof (mmap->size)))
             kprintf (" size = 0x%0.4x, base_addr = 0x%0.8x,"
                     " length = 0x%.8x, type = %s\n",
                     (unsigned) mmap->size,
                     (unsigned) mmap->base_addr_low,
                     (unsigned) mmap->length_low,
                     mtype[mmap->type - 1] );

	}

	if( (mboot_info->flags & FLAG_CMDLINE) ) {
		kinf.cmdline = (uchar8_t *)mboot_info->cmdline;
	} else {
		kinf.cmdline = NULL;
	}

	/* Configure processor */
	setup_GDT();
	setup_IDT();
	init_PIC();
	sti();

	/* Call the TempOS kernel */
	tempos_main(kinf);
}

