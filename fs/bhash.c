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
	uint64_t i, ht_entries;
	buff_hashq_t *hash_queue;
	buff_header_t *head, *prev, *nblock;

	/* Find a free position in the vector */
	for (i = 0; i < MAX_BUFFER_QUEUES; i++) {
		if (buffer_queues[i] == NULL) {
			break;
		}
	}
	if (i >= MAX_BUFFER_QUEUES) {
		return -1;
	}

	/* Alloc memory for blocks and structures */
	hash_queue = (buff_hashq_t*)kmalloc(sizeof(buff_hashq_t), GFP_NORMAL_Z);
	if (hash_queue == NULL) {
		return -1;
	}
 
	/* Alloc memory for hashtable */
	ht_entries = (size / 4);
	hash_queue->hashtable = (buff_header_t*)kmalloc(ht_entries * sizeof(buff_header_t), GFP_NORMAL_Z);

	if (hash_queue->hashtable == NULL) {
		kfree(hash_queue);
		return -1;
	}

	/* Initialize all blocks (put them into freelist)*/
	memset(hash_queue, 0, sizeof(buff_hashq_t));

	/* Free list head */
	head = &hash_queue->blocks[0];
	head->free_prev = head;
	head->free_next = head;
	prev = head;

	/* Add each block to the "tail" of Free list */
	for (i = 1; i < BUFF_QUEUE_SIZE; i++) {
		nblock = &hash_queue->blocks[i];
		nblock->free_prev = prev;
		nblock->free_next = head;
		head->free_prev = nblock;
		prev->free_next = nblock;
		prev = nblock;
	}

	hash_queue->freelist_head = head;
	
	buffer_queues[i] = hash_queue;

	return i;
}

