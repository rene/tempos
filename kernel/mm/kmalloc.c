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

extern uint32_t *kpagedir;

/* Kernel Map memory */
extern mem_map kmem;


/**
 * kmalloc
 *
 * Alloc memory =:)
*/
uint32_t *kmalloc(uint32_t size, uint16_t flags)
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
 * region allocated. So once we call _free_ function it will look at
 * inital_address-sizeof(mregion), get the region information and free the
 * memory.
 */
uint32_t *_vmalloc_(mem_map *memm, uint32_t size, uint16_t flags)
{
	uint32_t npages, ntables;
	uint32_t pstart, freep;
	uint32_t i, b, index;
	uint32_t size_region;
	uchar8_t value, start;
	mregion mem_area;

	/* Calculate number of pages and page tables needed */
	size_region = sizeof(mregion) + size;
	npages      = PAGE_ALIGN(size_region) >> PAGE_SHIFT;
	ntables     = TABLE_ALIGN(npages) >> TABLE_SHIFT;

	/* Search for space in bitmap */
	start  = 0;
	pstart = 0;
	freep  = 0;
	for(i=0; i<BITMAP_SIZE; i++) {

		for(b=0; b<(sizeof(uchar8_t) * 8); b++) {
			value = memm->bitmap[i] & (BITMAP_FBIT >> b);

			if(!start) {
				if(value == 0) {
					/* We found a free block */
					pstart = ((i * sizeof(uchar8_t)) + b);
					start  = 1;
					freep++;
				}
			} else {
				if(value == 1 && freep < npages) {
					return(0);
				} else {
					freep++;
				}
			}

			if(freep >= size) {
				/* We have the necessary space, now we need to take a
				   look at rigth index in pagedir and if there is a
				   table there, get it and fill with pages, otherwise
				   we start a new table. Other tables will be started
				   as they are needed. */
				index = GET_DINDEX(pstart);

				if((memm->pagedir[index] >> PAGE_SHIFT)  == 0) {
					/* Alloc table */
				}
			}
		}
	}

	return(0);
}


