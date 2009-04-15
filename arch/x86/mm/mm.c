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
#include <x86/page.h>
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
 *                  |----------------3GB+1MB
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
 *       ---------> |----------------End of memory used by Kernel
 *      |           |     .....      |
 *      |           |  stack_pages2  |
 *      |           |   NORMAL Zone  |
 *      |           |                |
 *      |           |----------------|
 *      |           |     .....      |
 *    Kernel        |  stack_pages1  |
 *    Block2        |    DMA Zone    |
 *      |           |  (0 to 16MB)   |
 *      |           |                |
 *      |           |----------------End of remaining memory pages
 *      |           |     .....      |
 *      |           |     .....      | 
 *      |           |    mempages    |
 *       ---------> |----------------End of page tables used by Kernel
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
uint32_t *mempages;

/* Physical address of tables */
void *kpagedir_ptr;
void *kpgtables_ptr;
void *mempages_ptr;

/* Stack pointers */
uint32_t *stack_pages1_top;
uint32_t *stack_pages2_top;


/**
 * init_pg
 *
 * This function starts the low level Memory Manager, configuring
 * 4Kb pages and allocating correct memory to the kernel
 */
void init_pg(karch_t kinf)
{
	uint32_t i, j, ap, np, nt;
	uint32_t address;
	uint32_t nptotal; /* Number of total free memory pages to the system */
	uint32_t totalmem;
	uint32_t mempages_size;


	kpagedir      = (uint32_t *)KERNEL_END_ADDR;
	kpgtables     = (uint32_t *)kpagedir + 0x1000;
	kpagedir_ptr  = (void *)kpagedir - KERNEL_ADDR_OFFSET;
	kpgtables_ptr = (void *)kpgtables - KERNEL_ADDR_OFFSET;

	/* Calculate how many pages Kernel needs
	   PS: 256 - First 1MB of memory */
	np = 256 + (PAGE_ALIGN((uint32_t)KERNEL_END_ADDR -
					(uint32_t)KERNEL_START_ADDR) >> PAGE_SHIFT);

	/* Ajust pointers to remaining memory pages */
	mempages     = (uint32_t *)kpgtables + (sizeof(uint32_t) * np);
	mempages_ptr = (void *)mempages - KERNEL_ADDR_OFFSET;

	/* Calculate how many pages are remaining to the system       */
	totalmem = kinf.mem_upper << 10; /* Total upper system memory */
	nptotal  = (PAGE_ALIGN(totalmem) >> PAGE_SHIFT) - np;

	/* Calculate and sum pages that we need to Kernel Block2      */
	mempages_size = PAGE_ALIGN(nptotal * TABLE_ENTRY_SIZE) >> PAGE_SHIFT;
	np           += (2 * mempages_size);

	/* Calculate total page tables Kernel needs */
	nt = TABLE_ALIGN(np) >> TABLE_SHIFT;


	/* Zero kernel page directory */
	for(i=0; i<1024; i++) {
		kpagedir[i]  = (0 | PAGE_WRITABLE);
	}

	/* Zero ALL kernel page tables */
	for(i=0; i<nt; i++) {
		for(j=0; j<TABLE_SIZE; j++) {
			kpgtables[(i * TABLE_SIZE) + j] = (0 | PAGE_WRITABLE);
		}
	}


	/* Map 0-1MB */
	address = 0;
	ap      = 0;
	for(i=0; i<256; i++, ap++) {
		kpgtables[ap] = address | (PAGE_WRITABLE | PAGE_PRESENT);
		address      += PAGE_SIZE;
	}

	/* Map Kernel memory */
	address = KERNEL_PA_START;
	j       = 256;
	for(i=0; i<nt && ap<np; i++) {
		for(; j<TABLE_SIZE && ap<np; j++, ap++) {
			kpgtables[ap] = address | (PAGE_WRITABLE | PAGE_PRESENT);
			address      += PAGE_SIZE;
		} j = 0;
	}

	/* Page DIR */
	for(i=0; i<nt; i++) {
		kpagedir[0 + i]   = ((uint32_t)kpgtables_ptr + (i * PAGE_SIZE)) |
								(PAGE_WRITABLE | PAGE_PRESENT); /* 0MB     */
		kpagedir[768 + i] = ((uint32_t)kpgtables_ptr + (i * PAGE_SIZE)) |
								(PAGE_WRITABLE | PAGE_PRESENT); /* 3GB+1MB */
	}

	/* Enable Paging System */
	write_cr3((uint32_t)kpagedir_ptr);
	write_cr0(read_cr0() | CR0_PG_MASK);

	/* Configure mempages (Page Table entries) */
	address -= PAGE_SIZE;
	for(i=0; i<mempages_size; i++) {
		mempages[i] = address | (PAGE_WRITABLE | PAGE_PRESENT);
		address    += PAGE_SIZE;
	}

	/* Start stacks */

	/*
	asm volatile("movl %0, %%eax \n"
				 "movl %1, %%ebx \n"
	             "loop:         \n"
				 "jmp loop      \n" : : "r" (np), "r" (mempages_size): "eax");*/




}


