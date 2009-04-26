/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: mm.h
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

#ifndef MEM_MANAGER_H

	#define MEM_MANAGER_H

	#include <tempos/kernel.h>
	#include <x86/page.h>
	#include <x86/mm.h>


	#define BITMAP_SIZE	  0x20000 /* (TABLE_SIZE^2) / (sizeof(uchar8_t) * 8) */
	#define BITMAP_SHIFT	    3
	#define BITMAP_FBIT		 0x80

	#define GET_DINDEX(page)	 (page >> TABLE_SHIFT)
	#define DINDEX_VADDR(index)  (KERNEL_START_ADDR + (index << TABLE_SHIFT))
	#define TABLE_ADDR(index)	 (index << PAGE_SHIFT)

	/* Flags for kmalloc functions */
	#define GFP_DMA_Z		0x01
	#define GFP_NORMAL_Z	0x02
	#define GFP_ZEROP		0x04


	/* Map of a directory */
	struct _mem_map {
		volatile pagedir_t *pagedir;	/* page directory */
		uchar8_t bitmap[BITMAP_SIZE];
	} __attribute__ ((packed));

	/* Region of allocated memory */
	struct _mregion {
		struct _mem_map *memm;
		uint32_t initial_addr;
		uint32_t size;
	} __attribute__ ((packed));

	typedef struct _mem_map mem_map;
	typedef struct _mregion mregion;

	void init_mm(void);

	void bmap_clear(volatile mem_map *map);

	void bmap_on(volatile mem_map *map, uint32_t block);

	void bmap_off(volatile mem_map *map, uint32_t block);

	void *kmalloc(uint32_t size, uint16_t flags);

	void *_vmalloc_(mem_map *memm, uint32_t size, uint16_t flags);

	void kfree(void *ptr);

#endif /* MEM_MANAGER_H */


