/*
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
	#include <unistd.h>
	#include <tempos/mm.h>
	//#include <fs/device.h>
	#include <config.h>

	/* Block buffer: possible status */
	
 	/** Buffer is unlocked (not busy) */
	#define BUFF_ST_UNLOCKED  	0x01
	/** The buffer contains valid data */
	#define BUFF_ST_VALID   	0x02 
	/** Must be flushed to device */
	#define BUFF_ST_FLUSH   	0x04
	/** Kernel is reading or writing to device */
	#define BUFF_ST_BUSY    	0x08 	
	/** A process is currently waiting for the buffer to become free */
	#define BUFF_ST_WAITING 	0x0F
	/** The buffer contains invalid data (circular list head) */
	#define BUFF_ST_HEAD 		0x40

	/** Buffer size */
	#define BUFF_SIZE 		512

	/** How many blocks has each buffer queue */
	#ifdef CONFIG_BUFFER_QUEUE_SIZE
		/** System frequency defined at kernel configuration file */
		#define BUFF_QUEUE_SIZE CONFIG_BUFFER_QUEUE_SIZE
	#else
		#error "CONFIG_BUFFER_QUEUE_SIZE it's not defined. It should be defined at configuration file."
	#endif

	/** Maximum of buffer queues */
	#define MAX_BUFFER_QUEUES 50


	/** Buffer structure */
	struct _buffer_header_t {
		uint64_t addr;
		char status;
		char data[BUFF_SIZE];
		/* links to make a circular linked list into hash queue */
		struct _buffer_header_t *prev;
		struct _buffer_header_t *next;
		/* links to make a circular linked list into free list */
		struct _buffer_header_t *free_prev;
		struct _buffer_header_t *free_next;
	};
	
	typedef struct _buffer_header_t buff_header_t;

	/** Buffer hash queue. Each device should have one of this. */
	struct _buff_hash_queue_t {
		/** How many position are in hash table. */
		uint64_t size;
		/** Each position has a circular linked list of buffer headers. */
		struct _buffer_header_t **hashtable;
		/** Free list head */
		struct _buffer_header_t *freelist_head;
		/** Blocks */
		struct _buffer_header_t blocks[BUFF_QUEUE_SIZE];
	};

	typedef struct _buff_hash_queue_t buff_hashq_t;

	/* Prototypes */
	buff_hashq_t  *create_hash_queue(uint64_t size);
	
	buff_header_t *bread(int major, int device, uint64_t blocknum);

#endif /* BHASH_H */

