/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: kmalloc.c
 * Desc: Functions to alloc memory
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

#include <tempos/mm.h>


/* Kernel Map memory */
extern mem_map kmem;


/**
 * kmalloc
 *
 * Alloc memory =:)
*/
void *kmalloc(uint32_t size, uint16_t flags)
{
	return( _vmalloc_(&kmem, size, flags) );
}


/**
 * _vmalloc_
 *
 * What this functions does it's look at a memory map bitmap and try to
 * find space to alloc size bytes. We can also free the memory allocated
 * with _vmalloc_, to do that, every piece of memory allocated will start
 * with mregion structure, that contains the initial address and size of
 * region allocated. So once we call kfree function it will look at
 * inital_address-sizeof(mregion), get the region information and free the
 * memory.
 */
void *_vmalloc_(mem_map *memm, uint32_t size, uint16_t flags)
{
	uint32_t npages, pstart, ffpage;
	uint32_t apages, index, oldindex;
	uint32_t size_region;
	uint32_t newpage;
	uint32_t *mem_block;
	uint32_t *table;
	mregion *mem_area;
	zone_t mzone;
	uchar8_t bit, istart;
	volatile pagedir_t *pgdir;
	uint32_t i, j;

	/* Check flags */
	if( (flags & GFP_DMA_Z) ) {
		mzone = DMA_ZONE;
	} else {
		mzone = NORMAL_ZONE;
	}

	/* Calculate number of pages needed */
	size_region = sizeof(mregion) + size;
	npages      = PAGE_ALIGN(size_region) >> PAGE_SHIFT;

	/* Search in bitmap */
	istart = 0;
	pstart = 0;
	apages = 0;
	for(i=0; i<BITMAP_SIZE; i++) {

		for(j=0; j<(sizeof(uchar8_t) * 8); j++) {
			bit = (memm->bitmap[i] & (BITMAP_FBIT >> j));

			if(bit == 0) {
				/* Free block */
				if(!istart) {
					istart = 1;
					pstart = (i * sizeof(uchar8_t) * 8) + j;
					apages++;
				} else {
					apages++;
				}
			} else {
				if(apages < npages) {
					istart = 0;
					apages = 0;
				}
			}

			if(apages >= npages)
				break;
		}
	}

	if(apages < npages)
		return(NULL);

	pgdir = &(memm->pagedir); while(1);


	/* We have the necessary space, now we need to take a
	   look at rigth index in pagedir and if there is a
	   table there, get it and fill with pages, otherwise
	   we start a new table. Other tables will be started
	   as they are needed. *
	index = GET_DINDEX(pstart);
	if(pgdir->tables[index] == 0) {
		* Alloc table *
		if( !(newpage = alloc_page(mzone)) ) {
			return(NULL);
		} else {
			pgdir->tables[index]         = MAKE_ENTRY(newpage, (PAGE_WRITABLE | PAGE_PRESENT));
			pgdir->tables_phy_addr[index] = pgdir->tables[index];

			table = (uint32_t *)pgdir->tables[index];
			for(i=0; i<TABLE_SIZE; i++) {
				table[i] = 0;
			}
		}
	 }*/

	/* Start the block allocated information. The vfree 
	   function will receive only an address as an argument,
	   so the trick here is hold an information about the
	   block just before the block itself. */
	mem_block = (uint32_t *)(pstart * PAGE_SIZE);
	mem_area               = (mregion *)mem_block;
	mem_area->memm         = memm;
	mem_area->initial_addr = pstart;
	mem_area->size         = npages;

	mem_block = (uint32_t *)((uint32_t)mem_block + sizeof(mregion));

	if( (flags & GFP_ZEROP) ) {
		for(i=0; i<((size - sizeof(mregion)) / sizeof(uint32_t)); i++) {
			mem_block[i] = 0;
		}
	}

	/* We have done =:) */
	return(mem_block);

//error:
	/* Free pages allocated */
	return(0);
}


/**
 * kfree
 *
 * Free memory allocated with _vmalloc
 */
void kfree(uint32_t *ptr)
{
	mregion *mem_area = (mregion *)((uint32_t)ptr - sizeof(mregion));
	uint32_t index    = mem_area->initial_addr >> TABLE_SHIFT;
	uint32_t ffpage   = mem_area->initial_addr - (index * TABLE_SIZE) + 1;
	uint32_t lpage    = ffpage + mem_area->size - 1;
	uint32_t *table;
	volatile pagedir_t *pgdir;
	uint32_t i, j, k;

	pgdir = &(mem_area->memm->pagedir);

}


