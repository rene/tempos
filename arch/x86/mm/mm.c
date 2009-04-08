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
#include <x86/karch.h>
#include <x86/gdt.h>


/**
 * This is the low level memory manager system of TempOS
 *
 *
 *
 *
 *
 *  ----------------4GB
 * |                |
 * | Kernel virtual |
 * |     space      |
 * |                |
 * |   3GB to 4GB   |
 * |                |
 * |----------------3GB
 * |                |
 * |  User virtual  |
 * |     space      |
 * |                |
 * |   0 to 3GB     |
 * |                |
 *  ----------------0
 */

uint32_t read_cr0() {
	uint32_t cr0;
	asm volatile("movl %%cr0, %0" : "=r" (cr0));
	return(cr0);
}

void write_cr0(uint32_t value)
{
	asm volatile("movl %0, %%cr0" : : "r" (value));
}

uint32_t read_cr3() {
	uint32_t cr3;
	asm volatile("movl %%cr3, %0" : "=r" (cr3));
	return(cr3);
}

void write_cr3(uint32_t value)
{
	asm volatile("movl %0, %%cr3" : : "r" (value));
}


/**
 * init_mm
 *
 * This function starts the low level Memory Manager allocator, configuring
 * 4Kb pages and creating two memory zones: DMA and NORMAL. If the system has
 * less then 16MB of memory, NORMAL zone will contain the whole "giant" ;-)
 * memory. Otherwise, DMA will contain the first 16MB of memory and NORMAL
 * zone the rest.
 *
 * 
 *
 *
 */
void init_mm(void)
{
	uint32_t *pagedir     = (uint32_t *)KERNEL_END_ADDR;
	uint32_t *pagetable	  = pagedir + 0x1000;

	uint32_t i, address;

	for(i=0; i<1024; i++) {
		pagedir[i] = 0 | 2;
	}

	/* Map 0MB-4MB*/
	address = 0;
	for(i=0; i<1024; i++) {
		pagetable[i] = address | 0x03;
		address     += 4096;
	}

	/* Page DIR */
	pagedir[0]  = (uint32_t)pagetable | 0x03;//& 0xFFFFF003;
	for(i=1; i<1024; i++) {
		pagedir[i] = 0 | 2;
	}

	write_cr3((uint32_t)pagedir);
	//write_cr0(read_cr0() | 0x80000000);
	{
		uint32_t newcr0 = read_cr0() | 0x80000000;

	asm volatile(
		"movl %0,   %%cr0   \n"
		/* Reload CODE segment */
		"ljmp $0x08, $1f    \n"
		"1:                 \n"
		"movl $stack, %%esp"
			: : "r" (newcr0) );
	}
}

