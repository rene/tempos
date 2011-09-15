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
#include <fs/device.h>
#include <tempos/wait.h>

/**
 * Creates a buffer queue (cache of blocks) to a specific disk.
 * \param major Major number of the device.
 * \param size The size (in sectors) of the device.
 * \return int -1 on error. Otherwise the number of the queue. 
 * \note The returned number should be used as argument to cache 
 *  block functions (getblk, etc).
 */
buff_hashq_t *create_hash_queue(int major, uint64_t size)
{
	uint64_t i, ht_entries;
	buff_hashq_t *hash_queue;
	buff_header_t *head, *prev, *nblock;

	/* Alloc memory for blocks and structures */
	hash_queue = (buff_hashq_t*)kmalloc(sizeof(buff_hashq_t), GFP_NORMAL_Z);
	if (hash_queue == NULL) {
		return NULL;
	}
 
	/* Alloc memory for hashtable */
	ht_entries = (size / 4);
	hash_queue->hashtable = (buff_header_t**)kmalloc(ht_entries * sizeof(buff_header_t*), GFP_NORMAL_Z);

	if (hash_queue->hashtable == NULL) {
		kfree(hash_queue);
		return NULL;
	}

	/* Initialize all blocks (put them into freelist)*/
	memset(hash_queue, 0, sizeof(buff_hashq_t));

	for (i = 0; i < ht_entries; i++) {
		hash_queue->hashtable[i] = NULL;
	}

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
	
	return hash_queue;
}


/**
 * Search for a block on hash queue.
 *
 * \param queue The hash queue.
 * \param blocknum Block number.
 * \return buff_header_t The block (if was found), NULL otherwise.
 */
static buff_header_t *search_blk(buff_hashq_t *queue, uint64_t blocknum)
{
	struct _buffer_header_t *head, *tmp;
	uint64_t pos;

	/* Find position based on block number.
	   The hash algorithm is: (BLOCKNUM % 4) */
	pos = blocknum % 4;

	if (queue->hashtable[pos] == NULL) {
		return NULL;
	} else {
		head = queue->hashtable[pos];
	}

	/* Search for the block */
	tmp = head;
	if (head->addr == blocknum) {
		return head;
	} else {
		tmp = head->next;
		while (tmp->addr != head->addr) {
			if (tmp->addr == blocknum) {
				break;
			}
			tmp = tmp->next;
		}
	}

	if (tmp->addr == head->addr) {
		return NULL;
	} else {
		return tmp;
	}
}


/**
 * Search and get a free block from free list.
 *
 * \param queue The hash queue.
 */
static buff_header_t *get_free_blk(buff_hashq_t *queue)
{
	return NULL;
}


/**
 * getblk
 *
 * Each disk has a buffer queue of blocks. This function will search for a
 * specific block in the disk's buffer queue. If the block is not present in the
 * queue, it will be read from the disk. 
 *
 * \param major Major number of the device
 * \param device Minor number (device number)
 * \param blocknum Block number (address)
 * \return buff_header_t* Pointer to the block
 */
buff_header_t *getblk(int major, int device, uint64_t blocknum)
{
	buff_header_t *buff;
	dev_blk_driver_t *driver;
	char buffer_not_found = 1;
	
	driver = block_dev_drivers[major];

	while(buffer_not_found) {
	
		if ( (buff = search_blk(driver->buffer_queue, blocknum)) != NULL ) {
			/* Block is on hash queue */
			kprintf("Achou!\n");
		} else {
			/* Block is not on hash queue */ kprintf("Nao achou\n");
			
			/* There are no free buffers on free list */
			if ( (buff = get_free_blk(driver->buffer_queue)) == NULL ) {
				sleep_on(WAIT_BLOCK_BUFFER_GET_FREE);
				continue;
			}
		}
	
	}

	return NULL;
}

