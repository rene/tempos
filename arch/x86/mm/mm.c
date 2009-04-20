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
#include <x86/gdt.h>
#include <x86/karch.h>
#include <tempos/kernel.h>


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
 *      |           |  stack_pages2  |
 *      |           |   NORMAL Zone  |
 *      |           |                |
 *      |           |----------------|
 *      |           |     .....      |
 *    Kernel        |  stack_pages1  |
 *    Block2        |    DMA Zone    |
 *      |           |  (0 to 16MB)   |
 *      |           |                |
 *      |           |----------------End of page tables used by Kernel
 *      |           |     .....      |
 *      |           |     .....      |
 *      |           |    kpgtables   |
 *       ---------->|----------------KERNEL_END_ADDR + 4Kb
 *      |           |    kpagedir    |
 *      |           |----------------KERNEL_END_ADDR
 *    Kernel        |                |
 *    Block1        |     Kernel     |
 *      |           |  Code + Data   |
 *      |           |    Sections    |
 *       ---------->|----------------KERNEL_START_ADDR
 *                  |                |
 *                  |      ...       |
 *
 */

/* Kernel tables */
uint32_t *kpagedir   = 0;
uint32_t *kpgtables  = 0;

/* Pages */
static uint32_t maxpages  = 0; /* Total of pages on the system   */
static uint32_t ks_pages  = 0; /* Number of pages used by kernel */

/* Stack pointers */
static uint32_t *stack_pages1      = 0;
static uint32_t *stack_pages2      = 0;
static uint32_t *stack_pages1_top  = 0;
static uint32_t *stack_pages2_top  = 0;
static uint32_t stackp1_top        = 0;
static uint32_t stackp2_top        = 0;
static uint32_t stackp1_maxtop     = 0;
static uint32_t stackp2_maxtop     = 0;


/**
 * init_pg
 *
 * This function starts the low level Memory Manager, configure
 * 4Kb pages, allocate and map correct memory to the kernel, prepare
 * pages stacks and so on.
 */
void init_pg(karch_t *kinf)
{
	uint32_t i, j, ap, np, nt;
	uint32_t address;
	uint32_t nptotal; /* Number of total memory pages to the system */
	uint32_t totalmem;
	uint32_t kpgtables_size, mempages_size, z1_size;
	uint32_t m_end, m_length, index;
	uint32_t kpa_start, kpa_length;

	/* Physical address of tables */
	void *kpagedir_ptr;
	void *kpgtables_ptr;


	/* Ajust pointers */
	kpagedir      = (uint32_t *)KERNEL_END_ADDR;
	kpgtables     = (uint32_t *)((uint32_t)kpagedir + 0x1000);
	kpagedir_ptr  = (void *)((uint32_t)kpagedir - KERNEL_ADDR_OFFSET);
	kpgtables_ptr = (void *)((uint32_t)kpgtables - KERNEL_ADDR_OFFSET);

	/* Calculate how many pages we need to Kernel Block1
	   PS: 256 - For the first 1MB of memory
	         1 - For kpagedir                             */
	np = 256 + 1 + (PAGE_ALIGN((uint32_t)KERNEL_END_ADDR -
					(uint32_t)KERNEL_START_ADDR) >> PAGE_SHIFT);

	/* Calculate how many pages are remaining to the system        */
	totalmem = kinf->mem_upper << 10; /* Total upper system memory */
	nptotal  = (totalmem / 4096) - np;

	/* Calculate and sum pages that we need to Kernel Block2       */
	mempages_size   = PAGE_ALIGN(nptotal * TABLE_ENTRY_SIZE) >> PAGE_SHIFT;
	np             += mempages_size;
	kpgtables_size  = PAGE_ALIGN(np * TABLE_ENTRY_SIZE) >> PAGE_SHIFT;

	/* Calculate total page tables Kernel needs */
	nt = (TABLE_ALIGN(np + kpgtables_size) >> TABLE_SHIFT);

	/* Final number of pages used by kernel     */
	np = nt * TABLE_SIZE;


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
	ap      = j;
	for(i=0; i<nt && ap<np; i++) {
		for(; j<TABLE_SIZE && ap<np; j++, ap++) {
			kpgtables[ap] = address | (PAGE_WRITABLE | PAGE_PRESENT);
			address      += PAGE_SIZE;
		} j = 0;
	}

	/* Page DIR */
	for(i=0; i<nt; i++) {
		kpagedir[0 + i]   = ((uint32_t)kpgtables_ptr +
								(i * TABLE_SIZE * TABLE_ENTRY_SIZE)) |
									(PAGE_WRITABLE | PAGE_PRESENT); /* 0MB  */
		kpagedir[KERNEL_PDIR_SPACE + i] = ((uint32_t)kpgtables_ptr +
										(i * TABLE_SIZE * TABLE_ENTRY_SIZE)) |
									(PAGE_WRITABLE | PAGE_PRESENT); /* 3GB  */
	}

	/* Enable Paging System */
	write_cr3((uint32_t)kpagedir_ptr);
	write_cr0(read_cr0() | CR0_PG_MASK);

	/* Reload GDT */
	setup_GDT();



	/* Configure stack addresses
	   PS: If the system has less then 16MB of memory,
	       just one stack will exist. */
 	stack_pages1     = (uint32_t *)((uint32_t)kpgtables +
						(TABLE_SIZE * TABLE_ENTRY_SIZE * nt));
	stack_pages1_top = &stackp1_top;

	if(kinf->mem_upper <= 0x3C00) {
		z1_size          = mempages_size;
		stack_pages2     = stack_pages1;
		stack_pages2_top = stack_pages1_top;
	} else {
		z1_size          = PAGE_ALIGN(DMA_ZONE_SIZE) >> PAGE_SHIFT;
		stack_pages2     = (uint32_t *)((uint32_t)stack_pages1 +
								(z1_size * TABLE_ENTRY_SIZE));
		stack_pages2_top = &stackp2_top;
	}


	/** 
	   Re-arrange memory map to insert kernel region. In a
	   region of free memory we could have three situations:
	   
		      |-------------[********]--------------|
		1     |             [ kernel ]              |
		      |-------------[********]--------------|

		   [**|*****]-------------------------------|
		2  [ kernel ]                               |
		   [**|*****]-------------------------------|

		      |----------------------------------[**|*****]
		3     |                                  [ kernel ]
		      |----------------------------------[**|*****]

	*/
	kpa_start  = KERNEL_PA_START;
	kpa_length = (KERNEL_END_ADDR + (np * PAGE_SIZE) +
					(nt * TABLE_SIZE * TABLE_ENTRY_SIZE)) - KERNEL_START_ADDR;

	for(i=0; i<kinf->mmap_size; i++) {

		m_end = kinf->mmap_table[i].base_addr_low +
						kinf->mmap_table[i].length_low;

		if(m_end > (kpa_start + kpa_length)) {

			if(kinf->mmap_table[i].base_addr_low < kpa_start) {
				/* Situation 1 */
				m_length = kinf->mmap_table[i].length_low;

				kinf->mmap_table[i].length_low =
					(kpa_start - kinf->mmap_table[i].base_addr_low);

				/* Create another free region */
				m_length -= kinf->mmap_table[i].length_low;
				index     = kinf->mmap_size++;

				if(index < MBOOT_MMAP_MAXREG) {
					kinf->mmap_table[index].base_addr_low  =
											(kpa_start + kpa_length);
					kinf->mmap_table[index].base_addr_high = 0x00000000;
					kinf->mmap_table[index].length_low     = m_length;
					kinf->mmap_table[index].length_high    = 0x00000000;
					kinf->mmap_table[index].type           = MTYPE_RESERVED;
				} else {
					kprintf(KERN_CRIT "Memory region table full! :'(\n");
					for(;;);
				}

			} else if( kinf->mmap_table[i].base_addr_low <
						(kpa_start + kpa_length) ) {

				/* Situation 2 */
				kinf->mmap_table[i].base_addr_low = (kpa_start + kpa_length);
				kinf->mmap_table[i].length_low   -= kpa_length;
			}

		} else {
			/* Situation 3 */
			if( (kinf->mmap_table[i].base_addr_low < kpa_start) &&
					m_end < (kpa_start + kpa_length) )
				kinf->mmap_table[i].length_low =
						(kpa_start - kinf->mmap_table[i].base_addr_low);
		}
	}

	/* Map memory */
	*stack_pages1_top = 0;
	*stack_pages2_top = 0;
	for(i=0; i<kinf->mmap_size; i++) {
		if(kinf->mmap_table[i].type == MTYPE_AVALIABLE) {

			/* Map only over the 1MB of memory, because the first 1MB 
			   is mapped on kernel page tables */
			if(kinf->mmap_table[i].base_addr_low >= 0x100000) {

				/* Map the region */
				m_end   = kinf->mmap_table[i].base_addr_low +
								kinf->mmap_table[i].length_low;
				address = PAGE_ALIGN(kinf->mmap_table[i].base_addr_low);

				while( (address < m_end) ) {
					if(*stack_pages1_top < z1_size) {
						stack_pages1[(*stack_pages1_top)++] =
								address | (PAGE_WRITABLE | PAGE_PRESENT);
					} else {
						stack_pages2[(*stack_pages2_top)++] =
								address | (PAGE_WRITABLE | PAGE_PRESENT);
					}
					address += PAGE_SIZE;
				}
			}
		}
	}

	stackp1_maxtop = *stack_pages1_top;
	stackp2_maxtop = *stack_pages2_top;

	maxpages = np + nptotal;
	ks_pages = np;
}


/**
 * alloc_page
 *
 * Return the address to a free page entry or 0 if memory
 * is full
 *
 * Parameters:
 * 		zone - DMA_ZONE or NORMAL_ZONE
 */
uint32_t alloc_page(zone_t zone)
{
	switch(zone) {
		case DMA_ZONE:
			if(*stack_pages1_top)
				return(stack_pages1[(*stack_pages1_top)--]);
			break;

		case NORMAL_ZONE:
			if(*stack_pages2_top)
				return(stack_pages2[(*stack_pages2_top)--]);
			break;

		default:
			return(0);
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
	if( (page_e >> PAGE_SHIFT) <= 0x1000 ) {
		if(*stack_pages1_top < stackp1_maxtop) {
			stack_pages1[++(*stack_pages1_top)] =
					page_e | (PAGE_WRITABLE | PAGE_PRESENT);
		}
	} else {
		if(*stack_pages2_top < stackp2_maxtop) {
			stack_pages2[++(*stack_pages2_top)] =
					page_e | (PAGE_WRITABLE | PAGE_PRESENT);
		}
	}
	return;
}


/**
 * get_maxpages
 *
 * Return the total of pages on the system
 */
uint32_t get_maxpages(void)
{
	return(maxpages);
}


/**
 * get_kspages
 *
 * Return the total of pages used only by kernel (Block1 + Block2)
 */
uint32_t get_kspages(void)
{
	return(ks_pages);
}

