/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: dvhash.h
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

#ifndef DVHASH_H

	#define DVHASH_H

	#include <linkedl.h>
	#include <tempos/mm.h>
	#include <unistd.h>

	#define DEF_V1SIZE 4
	#define DEF_V2SIZE 4
	#define MIN_V1SIZE 1
	#define MIN_V2SIZE 1

	struct _dvhash_t {
		c_llist ***vector1;
		uint16_t major;
		uint64_t tblocks;
		uint32_t v1_size;
		uint32_t v2_size;
	};

	typedef struct _dvhash_t dvhash_t;


	int initialize_dvhash(dvhash_t **dvect, uint32_t v1_size, uint32_t v2_size,
						uint16_t major, uint64_t totalblocks);

#endif /* DVHASH_H */

