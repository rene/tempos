/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: karch.c
 * Desc: First Stage: Implement the architecture dependent functions to 
 *       start the TempOS kernel.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <tempos/kernel.h>
#include "multiboot.h"
#include "video.h"


/**
 * karch
 *
 * First Stage: Called from Boot Stage.
 */
void karch(unsigned long magic, unsigned long addr)
{
	karch_t kinf;
	multiboot_info_t *mboot_info;

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
		kprintf(KERN_CRIT "TempOS does not support other binary formats than ELF.\n");
		return;
	}

	if( !(mboot_info->flags & FLAG_MMAP) ) {
		kprintf(KERN_CRIT "Unable to get memory information. Abort.\n");
		return;
	}

	if( (mboot_info->flags & FLAG_CMDLINE) ) {
		kinf.cmdline = (u8 *)mboot_info->cmdline;
	} else {
		kinf.cmdline = NULL;
	}

	/* TODO: Configure processor */

	/* Call the TempOS kernel */
	tempos_main(kinf);
}

