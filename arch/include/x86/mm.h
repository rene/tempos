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

	/* Directory position that kernel is mapped */
	#define KERNEL_PDIR_SPACE	768 /* 3GB */

	/* Memory zones */
	#define DMA_ZONE		     0x01
	#define NORMAL_ZONE		     0x02
	#define DMA_ZONE_SIZE	0x1000000


	typedef uchar8_t zone_t;

	void init_pg(karch_t *kinf);

	uint32_t alloc_page(zone_t zone);

	void free_page(uint32_t page_e);

	uint32_t get_maxpages(void);

	uint32_t get_kspages(void);

	uint32_t alloc_table(void);

	void free_table(uint32_t table);

#endif /* ARCH_X86_MM_H */

