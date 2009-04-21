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
	uint32_t npages, apages;
	uint32_t pstart, freep;
	uint32_t i, j, b, index, oldindex, ffpage;
	uint32_t size_region;
	uchar8_t value, start, atable;
	mregion *mem_area;
	uint32_t newpage, newtable;
	uint32_t *mem_block, *table;
	zone_t mzone;


	/* Check flags */
	if( (flags & GFP_DMA_Z) ) {
		mzone = DMA_ZONE;
	} else {
		mzone = NORMAL_ZONE;
	}

	/* Calculate number of pages needed */
	size_region = sizeof(mregion) + size;
	npages      = PAGE_ALIGN(size_region) >> PAGE_SHIFT;

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
					pstart = ((i * 8 * sizeof(uchar8_t)) + b);
					start  = 1;
					freep++;
				}
			} else {
				if(value != 0 && freep < npages) {
					start = 0;
					freep = 0;
				} else {
					freep++;
				}
			}

			if(freep >= npages) {
				start = 2;
				break;
			}
		}
	}
	if(start != 2)
		return(0);

	/* We have the necessary space, now we need to take a
	   look at rigth index in pagedir and if there is a
	   table there, get it and fill with pages, otherwise
	   we start a new table. Other tables will be started
	   as they are needed. */
	index = GET_DINDEX(pstart);
	table = (uint32_t *)(memm->pagedir[index] >> PAGE_SHIFT);
	if(table == 0) {
		/* Alloc table */
		if( !(newtable = alloc_table()) ) {
			return(0);
		} else {
			memm->pagedir[index] = (newtable | PAGE_PRESENT);
			table  = (uint32_t *)DINDEX_VADDR(index);
			atable = 1;
		}
	} else {
		atable = 0;
	}

	oldindex = index;
	ffpage   = 0;
	apages   = 0; /* Allocated pages */
	while(apages <= npages) {
		table = (uint32_t *)(memm->pagedir[index] >> PAGE_SHIFT);

		/* Search for a free entry in page table */
		for(j=0; (j<TABLE_SIZE && apages <= npages); j++) {
			if((table[j] >> PAGE_SHIFT) == 0) {
				/* Alloc a page */
				if( !(newpage = alloc_page(mzone)) ) {
					goto error;
				} else {
					/* Is this the first page of our block? */
					if(apages == 0) {
						oldindex = index;
						ffpage   = j;
					}
					bmap_on(memm, pstart + apages);
					apages++;
					table[j] = newpage;
				}
			}
		}

		if(apages < npages) {
			/* Alloc table */
			if( !(newtable = alloc_table()) ) {
				goto error;
			} else {
				index++;
				memm->pagedir[index] = (newtable | PAGE_PRESENT);
				table = (uint32_t *)DINDEX_VADDR(index);
			}
		}
	}

	/* Start the block allocated information. The vfree 
	   function will receive only an address as an argument,
	   so the trick here is hold an information about the
	   block just before the block itself. */
	mem_block = (uint32_t *)((TABLE_ADDR(oldindex) +
					(ffpage * TABLE_ENTRY_SIZE)) & 0xFFFFF000);
	mem_area               = (mregion *)mem_block;
	mem_area->memm         = memm;
	mem_area->initial_addr = pstart;
	mem_area->size         = npages;

	mem_block = (uint32_t *)((uint32_t)mem_block + sizeof(mregion));

	if( (flags & GFP_ZEROP) ) {
		for(i=0; i<(size - sizeof(mregion)); i++) {
			mem_block[i] = 0;
		}
	}

	/* We have done =:) */
	return(mem_block);

error:
	/* Free pages allocated */
	while(apages > 0) {
		table = (uint32_t *)(memm->pagedir[index] >> PAGE_SHIFT);

		for(i=j; i>=1 && apages>0; i--) {
			if((table[i] >> PAGE_SHIFT) != 0) {
				/* free a page */
				free_page(table[i]);
				bmap_off(memm, pstart + apages);
				apages--;
				table[i] = 0;
			}

		} j = TABLE_SIZE;

		if(apages > 0 && i == 0) {
			/* free a table */
			free_page(memm->pagedir[index]);
			memm->pagedir[index] = 0;
			index--;
		}
	}

	return(0);
}


/**
 * kfree
 *
 * Free memory allocated with _vmalloc
 */
void kfree(void *ptr)
{
	mregion *mem_area = (mregion *)((uint32_t)ptr - sizeof(mregion));
	uint32_t index    = mem_area->initial_addr >> TABLE_SHIFT;
	uint32_t ffpage   = mem_area->initial_addr - (index * TABLE_SIZE) + 1;
	uint32_t lpage    = ffpage + mem_area->size - 1;
	uint32_t *pagedir, *table;
	uint32_t i, j, k;

	/* Free memory pages */
	j = k = lpage;
	while(j >= ffpage) {
		table = (uint32_t *)(mem_area->memm->pagedir[index] >> PAGE_SHIFT);

		for(i=k; i>=1; i--) {
			if((table[i] >> PAGE_SHIFT) != 0) {
				/* free a page */
				free_page(table[i]);
				table[i] = 0;
				bmap_off(mem_area->memm, j);
				j--;
			}
		} k = TABLE_SIZE;

		if(i == 0) {
			/* free a table */
			free_page(mem_area->memm->pagedir[index]);
			mem_area->memm->pagedir[index] = 0;
			index--;
		}
	}
	index++;

	/* Finally, we free the last page of block */
	pagedir = mem_area->memm->pagedir;
	bmap_off(mem_area->memm, mem_area->initial_addr);
	free_page(table[ffpage]);
	if(ffpage == 0) {
		/* free the table */
		free_page(pagedir[index]);
		pagedir[index] = 0;
	}

}


