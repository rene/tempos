/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: mm.c
 * Desc: Functions to handle the memory pages on x86 architecture
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

#include <x86/mm.h>
#include <x86/io.h>
#include <x86/karch.h>


/**
 * This is the low level memory manager system of TempOS
 *
 * TempOS uses a reallocable kernel, which means that kernel
 * are mapped to 3GB+1MB of virtual address space. This is done
 * using GDT trick while pageing system is not enable (in boot
 * stage). When paging system are working, GDT are reloaded with
 * base 0, since translation will be done by paging system.
 *
 *  \/\/\/\/\/\/\ VIRTUAL ADDRESS SPACE MAP \/\/\/\/\/\/\
 *
 *                   ----------------4GB
 *                  |     Kernel     |
 *                  |                |
 *                  |   3GB to 4GB   |
 *                  |----------------3GB
 *                  |                |
 *                  |                |
 *                  |                |
 *                  |                |
 *                  |      User      |
 *                  |                |
 *                  |   0  to  3GB   |
 *                  |                |
 *                   ----------------0
 *
 *  \/\/\/\/\/\/\/\/ KERNEL MEMORY MAP /\/\/\/\/\/\/\/\/\
 *
 *                
 *                  |      ...       |
 *                  |                |
 *                  |----------------X = Number of page tables used by Kernel
 *                  |     .....      |
 *                  |     .....      |
 *                  |   kpgtables    |
 *                  |----------------KERNEL_END_ADDR + (1024 * 4) bytes
 *                  |    kpagedir    |
 *                  |----------------KERNEL_END_ADDR
 *                  |                |
 *                  |     Kernel     |
 *                  |  Code + Data   |
 *                  |    Sections    |
 *                  |----------------KERNEL_START_ADDR
 *                  |                |
 *                  |      ...       |
 *
 */


uint32_t *kpagedir;
uint32_t *kpgtables;

/* Physical address of tables */
void *kpagedir_ptr;
void *kpgtables_ptr;


/**
 * init_mm
 *
 * This function starts the low level Memory Manager allocator, configuring
 * 4Kb pages and allocating correct memory to the kernel
 */
void init_mm(void)
{
	kpagedir  = (uint32_t *)KERNEL_END_ADDR;
	kpgtables = (uint32_t *)kpagedir + 0x1000;

	/* Physical address of tables */
	kpagedir_ptr  = (void *)kpagedir - KERNEL_PAGE_OFFSET;
	kpgtables_ptr = (void *)kpgtables - KERNEL_PAGE_OFFSET;


	uint32_t *pagedir     = (uint32_t *)KERNEL_END_ADDR;
	uint32_t *pagetable	  = (uint32_t *)pagedir + 0x1000;

	void *pagetablePTR = (void*)pagetable - KERNEL_PAGE_OFFSET;
	void *pagedirPTR   = (void*)pagedir - KERNEL_PAGE_OFFSET;

	uint32_t i, address;

	for(i=0; i<1024; i++) {
		pagedir[i]   = 0 | 2;
		pagetable[i] = 0 | 2;
	}

	/* Map 0MB-4MB*/
	address = 0;
	for(i=0; i<1024; i++) {
		pagetable[i] = address | 0x03;
		address     += 4096;
	}

	/* Page DIR */
	pagedir[0]   = (uint32_t)pagetablePTR | 0x03;//& 0xFFFFF003;
	pagedir[768] = (uint32_t)pagetablePTR | 0x03;

	write_cr3((uint32_t)pagedirPTR);
	write_cr0(read_cr0() | 0x80000000);
}


