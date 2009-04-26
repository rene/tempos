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

/**
 * This is the low level memory manager system of TempOS
 *
 * TempOS uses a reallocable kernel, which means that kernel
 * are mapped to 3GB of virtual address space. This is done
 * using GDT trick while paging system is not enable (in boot
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
 *       ---------> |----------------End of memory used by Kernel
 *      |           |     .....      |
 *      |           |                |
 *    Kernel        |   stack_pages  |
 *    Block2        |     .....      |
 *      |           |                |
 *      |           |    kerneldir   |
 *       ---------->|----------------KERNEL_END_ADDR
 *      |           |                |
 *    Kernel        |     Kernel     |
 *    Block1        |  Code + Data   |
 *      |           |    Sections    |
 *       ---------->|----------------KERNEL_START_ADDR
 *                  |                |
 *                  |      ...       |
 *
 */

#include <x86/mm.h>
#include <x86/io.h>
#include <x86/gdt.h>
#include <x86/karch.h>
#include <tempos/kernel.h>


/* Address used by kmalloc_e */
static uint32_t free_phy_addr;

/* Stack pages */
static uint32_t *stack_pages;
static uint32_t stack_top;
static uint32_t stack_max_top;

/* Kernel pages directory */
volatile pagedir_t *kerneldir;

/* Kernel size: Block1 + Block2 */
static uint32_t kernel_size;

/**
 * init_pg
 *
 * This function starts the low level Memory Manager, configure
 * 4Kb pages, allocate and map correct memory to the kernel, prepare
 * pages stacks and so on.
 */
void init_pg(karch_t *kinf)
{
	uint32_t stack_sz;                /* stack_pages  size          */
	uint32_t totalmem;                /* Total memory of the system */
	uint32_t kpa_start, kpa_length;
	uint32_t m_end, index;
	uint32_t address, *table1, *table2;
	mmap_tentry *mmap;
	uint32_t i, j;

	/* Initialize free_phy_addr. We use "virtual address" because
	   translation are done by GDT trick */
	free_phy_addr = (uint32_t)KERNEL_END_ADDR;

	/**
	 * Start Kernel pages directory
	 */
	kerneldir = make_kerneldir();

	/**
	 * Alloc space for stack pages
	 */
	totalmem = (kinf->mem_upper << 10);
	stack_sz = (totalmem / PAGE_SIZE) * sizeof(uint32_t);

	stack_pages = (uint32_t *)kmalloc_e(stack_sz);


	/* Map kernel memory */
	address        = 0;
	free_phy_addr -= KERNEL_ADDR_OFFSET;
	table1         = kerneldir->tables[0];
	table2         = kerneldir->tables[KERNEL_PDIR_SPACE];
	i = j = 0;
	while(address <= free_phy_addr) {

		table1[i] = MAKE_ENTRY(address, (PAGE_WRITABLE | PAGE_PRESENT));
		table2[i] = table1[i];
		address  += PAGE_SIZE;

		if(i < TABLE_SIZE) {
			i++;
		} else {
			i = 0;
			j++;
			table1 = kerneldir->tables[0 + j];
			table2 = kerneldir->tables[KERNEL_PDIR_SPACE + j];
		}
	}

	/** 
	  * Re-arrange memory map to insert kernel region.
	  */
	kpa_start   = KERNEL_PA_START;
	kpa_length  = free_phy_addr - kpa_start;
	kernel_size = kpa_length;

	for(i=0; i<kinf->mmap_size; i++) {
		mmap = &(kinf->mmap_table[i]);

		if(mmap->type == MTYPE_AVALIABLE) {

			if(mmap->base_addr_low >= 0x100000) {
				/* Create kernel region */
				index = kinf->mmap_size++;
				if(index < MBOOT_MMAP_MAXREG) {
					kinf->mmap_table[index].base_addr_low  = kpa_start;
					kinf->mmap_table[index].base_addr_high = 0x00000000;
					kinf->mmap_table[index].length_low     = kpa_length;
					kinf->mmap_table[index].length_high    = 0x00000000;
					kinf->mmap_table[index].type           = MTYPE_RESERVED;
				} else {
					panic("Memory region table full! :'(\n");
				}

				/* Ajust memory */
				mmap->base_addr_low += kpa_length;
				mmap->length_low    -= kpa_length;
			}
		}
	}

	/* Start stack page */
	stack_max_top = stack_sz / sizeof(uint32_t);
	stack_top     = 0;

	for(i=0; i<kinf->mmap_size; i++) {
		mmap = &(kinf->mmap_table[i]);

		/* Map only memory avaliable and over 1MB */
		if(mmap->type == MTYPE_AVALIABLE &&
				mmap->base_addr_low >= 0x100000) {

			m_end   = mmap->base_addr_low + mmap->length_low;
			address = PAGE_ALIGN(mmap->base_addr_low);
			while(address < m_end && stack_top < stack_max_top) {
				stack_pages[stack_top++] = address;
				address                 += PAGE_SIZE;
			}
		}
	}
	stack_top = 0;

	/* Enable Paging System */
	write_cr3(kerneldir->dir_phy_addr);
	write_cr0(read_cr0() | CR0_PG_MASK);

	/* Reload GDT */
	setup_GDT();
}


/**
 * make_kerneldir
 *
 * Start new kernel pages directory. This functions will
 * alloc all tables used by directory (yes, I know this waste
 * memory space, on x86 each directory will get 4MB of memory
 * because all tables are allocated, but this approach turns
 * the implementation more easy and clear to understand).
 */
pagedir_t *make_kerneldir(void)
{
	pagedir_t *kdir;
	uint32_t *table;
	uint32_t i, j;

	kdir = (pagedir_t *)kmalloc_e(sizeof(pagedir_t));

	kdir->tables_phy_addr = (uint32_t *)kmalloc_e(PAGE_SIZE);
	kdir->dir_phy_addr    = GET_PHYADDR(kdir->tables_phy_addr);

	/* Alloc tables */
	for(i=0; i<TABLE_SIZE; i++) {
		kdir->tables[i] = (uint32_t *)kmalloc_e(PAGE_SIZE);

		/* Create dir entry */
		kdir->tables_phy_addr[i] = MAKE_ENTRY(GET_PHYADDR(kdir->tables[i]),
											(PAGE_WRITABLE | PAGE_PRESENT));

		/* Start table */
		table = kdir->tables[i];
		for(j=0; j<TABLE_SIZE; j++) {
			 table[j] = 0;
		}
	}

	return(kdir);
}


/**
 * get_kernel_size
 *
 * Return the number of bytes used by Kernel
 */
uint32_t get_kernel_size(void)
{
	return(kernel_size);
}


/**
 * alloc_page
 *
 * Return a free page entry or 0 if memory is full
 *
 * Parameters:
 * 		zone - DMA_ZONE or NORMAL_ZONE
 */
uint32_t alloc_page(zone_t zone)
{
	if(stack_top >= stack_max_top)
		return(0);

	if(zone == DMA_ZONE || zone == NORMAL_ZONE) {
		return( stack_pages[stack_top++] );
	}
	return(0);
}


/**
 * free_page
 *
 * Free the page entry allocated with alloc_page
 */
void free_page(uint32_t page_e)
{
	if(stack_top > 0)
		stack_pages[--stack_top] = page_e;
}



/**
 * kmalloc_e
 *
 * This is our kmalloc_e (early), which aims to be used before
 * enabling paging system. The variable free_phy_addr points to
 * the physical free memory space (after kernel). So as far as
 * we need more memory, the pointer will be incremented.
 */
void *kmalloc_e(uint32_t size)
{
	uint32_t tmp   = free_phy_addr;
	free_phy_addr  = PAGE_ALIGN(free_phy_addr + size);
	return((void *)tmp);
}


