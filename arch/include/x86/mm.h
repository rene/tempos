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

#ifndef ARCH_X86_MM_H

	#define ARCH_X86_MM_H

	#include <tempos/kernel.h>
	#include <x86/page.h>

	/* Get the physical address of x */
	#define GET_PHYADDR(x)		(((uint32_t)(x)) - KERNEL_ADDR_OFFSET)

	/* Directory position that kernel is mapped */
	#define KERNEL_PDIR_SPACE	  768 /* 3GB */

	/* Memory zones */
	#define DMA_ZONE		     0x01
	#define NORMAL_ZONE		     0x02
	#define DMA_ZONE_SIZE	0x1000000 /* 16MB */

	/* Pages directory */
	struct _page_dir {
		/* Pointer to each page table */
		uint32_t *tables[TABLE_SIZE];

		/* Physical address of each page table */
		uint32_t *tables_phy_addr;

		/* Physical address of the directory */
		uint32_t dir_phy_addr;
	} __attribute__((packed));

	typedef uchar8_t zone_t;
	typedef struct _page_dir pagedir_t;


	void init_pg(karch_t *kinf);

	pagedir_t *make_kerneldir(void);

	uint32_t get_kernel_size(void);

	uint32_t alloc_page(zone_t zone);

	void free_page(uint32_t page_e);

	void *kmalloc_e(uint32_t size);

#endif /* ARCH_X86_MM_H */

