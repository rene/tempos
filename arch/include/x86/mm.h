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


	/* Page directory entry */
	struct _pdir_entry {
		uint32_t present   : 1;
		uint32_t rw		   : 1;
		uint32_t level     : 1;
		uint32_t write     : 1;
		uint32_t cache     : 1;
		uint32_t accessed  : 1;
		uint32_t reserved  : 1;
		uint32_t size      : 1;
		uint32_t global    : 1;
		uint32_t avaliable : 3;
		uint32_t address   : 20;
	};


	void init_pg(karch_t kinf);

#endif /* ARCH_X86_MM_H */

