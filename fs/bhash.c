/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: bhash.c
 * Desc: Implements the buffer hash queue for block devices
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

#include <fs/bhash.h>


/**
 * Each disk of the system needs to have a buffer queue associated
 * with him. For performance purposes (we need to have at least 
 * a little performance concerns) all buffer queues will be located
 * at a vector of MAX_BUFFER_QUEUES size.
 */
static buff_hashq_t *buffer_queues[MAX_BUFFER_QUEUES];


/**
 * Function to initialize the hash queues.
 */
void init_hash_queues(void)
{
	int i;

	for (i = 0; i < MAX_BUFFER_QUEUES; i++) {
		buffer_queues[i] = NULL;
	}
}


/**
 * Creates a buffer queue (cache of blocks) to a specific disk.
 * \param major Major number of the device.
 * \param size The size (in sectors) of the device.
 * \return int -1 on error. Otherwise the number of the queue. 
 * \note The returned number should be used as argument to cache 
 *  block functions (getblk, etc).
 */
int create_hash_queue(int major, uint64_t size)
{
	uint64_t i, hq_size, hq_entries;
	buff_hashq_t *hash_queue;
	buff_header_t *buf, *prev;

	/* Find a free position in the vector */
	for (i = 0; i < MAX_BUFFER_QUEUES; i++) {
		if (buffer_queues[i] == NULL) {
			break;
		}
	}
	if (i >= MAX_BUFFER_QUEUES) {
		return -1;
	}

	/* Instead allocate each block structure, we allocate the whole
	 * memory necessary to hold hash queue and all block structures. 
	 * The layout is: hash queue structure followed by all block structures. */
	hq_entries = size / 4;
	hq_size    = sizeof(buff_hashq_t) + (2 * hq_entries * sizeof(buff_header_t*)) + (hq_entries * sizeof(buff_header_t)); 
	hash_queue = (buff_hashq_t*)kmalloc(hq_size, GFP_NORMAL_Z);
	
	if (hash_queue == NULL) {
		return -1;
	}

	hash_queue->size = hq_entries;
	hash_queue->device.major = major;

	/* Create each buffer structure */
	buf = (buff_header_t*)((char*)hash_queue + sizeof(buff_hashq_t) + (2 * hq_entries + sizeof(buff_header_t)));
	prev = buf;
	for (i = 0; i < hq_entries; i++) {
		//
	}


	buffer_queues[i] = hash_queue;

	return i;
}

