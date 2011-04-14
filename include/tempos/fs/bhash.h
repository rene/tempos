/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: bhash.h
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

#ifndef BHASH_H

	#define BHASH_H

	#include <linkedl.h>
	#include <tempos/mm.h>
	#include <unistd.h>

	/* Block buffer: possible status */
	#define BUFF_ST_LOCKED  0x01 /* Buffer is locked (busy) */
	#define BUFF_ST_VALID   0x02 /* The buffer contains valid data */
	#define BUFF_ST_FLUSH   0x04 /* Must be flushed to device */
	#define BUFF_ST_BUSY    0x08 /* Kernel is reading or writing to device */
	#define BUFF_ST_WAITING 0x0F /* A process is currently waiting for the buffer to become free */
	
	/* Buffer structure */
	struct _buffer_header_t {
		uint64_t block_num;
		int status;
		char *data;
	};

	struct _hash_queue_t {
		uint64_t size;
		int device;
		c_llist *blocks;
	};

	//typedef struct buffer

#endif /* BHASH_H */

