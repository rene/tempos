/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: init_mm.c
 * Desc: Functions to start the memory manager
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

extern volatile pagedir_t *kerneldir;

/* Kernel Map memory */
volatile mem_map kmem;


void init_mm(void)
{
	uint32_t kpages;
	uint32_t i;

	kpages = PAGE_ALIGN(get_kernel_size()) >> PAGE_SHIFT;

	/* Init Kernel map */
	kmem.pagedir = kerneldir;
	bmap_clear(&kmem);

	/*
	   Map used space
	   NOTE: kmalloc could be called just after this map !
	*/
	for(i=0; i<kpages; i++) {
		bmap_on(&kmem, i);
		bmap_on(&kmem, (KERNEL_PDIR_SPACE * TABLE_SIZE) + i);
	}

	/* We are ready for kmalloc =:) */
}


/**
 * bmap_clear
 *
 * Clear a bit map
 */
void bmap_clear(volatile mem_map *map)
{
	uint32_t i;

	for(i=0; i<BITMAP_SIZE; i++) {
		map->bitmap[i] = 0;
	}
}


/**
 * bmap_on
 *
 * Mark a bit (block) on a bitmap
 */
void bmap_on(volatile mem_map *map, uint32_t block)
{
	uint32_t byte = block >> BITMAP_SHIFT;
	uint32_t bit  = block - (byte * (sizeof(uchar8_t) * 8));

	map->bitmap[byte] |= (uchar8_t)(BITMAP_FBIT >> bit);
}


/**
 * bmap_off
 *
 * Unmark a bit (block) on a bitmap
 */
void bmap_off(volatile mem_map *map, uint32_t block)
{
	uint32_t byte = block >> BITMAP_SHIFT;
	uint32_t bit  = block - (byte * (sizeof(uchar8_t) * 8));

	map->bitmap[byte] |= (uchar8_t)~(BITMAP_FBIT >> bit);
}

