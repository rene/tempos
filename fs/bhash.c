/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
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
#include <arch/io.h>

/* Prototypes */
static buff_header_t *search_blk(buff_hashq_t *queue, int device, uint64_t blocknum);
static void remove_from_freelist(buff_hashq_t *queue, int device, uint64_t blocknum);
static buff_header_t *get_free_blk(buff_hashq_t *queue, int device, uint64_t blocknum);
static void add_to_buff_queue(buff_hashq_t *queue, buff_header_t *buff, int device, uint64_t blocknum);
static buff_header_t *getblk(int major, int device, uint64_t blocknum);


/**
 * Creates a buffer queue (cache of blocks) to a specific disk.
 * \param size The size (in sectors) of the device.
 * \return int -1 on error. Otherwise the number of the queue. 
 * \note The returned number should be used as argument to cache 
 *  block functions (getblk, etc).
 */
buff_hashq_t *create_hash_queue(uint64_t size)
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
	ht_entries = 4; /*(size / 4);*/
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
	head->status = BUFF_ST_HEAD;

	/* Add each block to the "tail" of Free list */
	hash_queue->freelist_head = head;
	prev = hash_queue->freelist_head;
	for (i = 1; i < BUFF_QUEUE_SIZE; i++) {
		nblock = &hash_queue->blocks[i];
		nblock->free_prev = prev;
		nblock->free_next = head;
		head->free_prev = nblock;
		prev->free_next = nblock;
		prev = nblock;
	}

	return hash_queue;
}


/**
 * Search for a block on hash queue.
 *
 * \param queue The hash queue.
 * \param blocknum Block number.
 * \param device Device number.
 * \return buff_header_t The block (if was found), NULL otherwise.
 */
static buff_header_t *search_blk(buff_hashq_t *queue, int device, uint64_t blocknum)
{
	struct _buffer_header_t *head, *tmp;
	uint64_t pos;

	/* Find position based on block number.
	   The hash algorithm is: (BLOCKNUM % 4) */
	pos = blocknum % 4;
	head = queue->hashtable[pos];

	if (head == NULL) {
		return NULL;
	}
	
	/* Search for the block */
	tmp = head;
	while (tmp != NULL) {
		if (tmp->addr == blocknum && tmp->device == device) {
			break;
		}
		tmp = tmp->next;
	}

	return tmp;
}

/**
 * Remove a block from free list
 *
 * \param queue The hash queue.
 * \param blocknum Block number.
 * \param device Device number.
 * \return buff_header_t The block (if was found), NULL otherwise.
 */
static void remove_from_freelist(buff_hashq_t *queue, int device, uint64_t blocknum)
{
	struct _buffer_header_t *head, *tmp, *prev, *next;

	/* Search for the block */
	head = queue->freelist_head; 
	tmp = head->free_next;
	while (tmp != head) {
		if (tmp->addr == blocknum && tmp->device == device) {
			break;
		}
		tmp = tmp->free_next;
	}
	if (tmp == head) {
		/* Block is not on free list */
		return;
	}
	
	/* Remove the block */
	cli();
	prev = tmp->free_prev;
	next = tmp->free_next;

	next->prev = prev;
	prev->next = next;
	sti();
	return;
}


/**
 * Search and get a free block from free list.
 *
 * \param queue The hash queue.
 * \param device Device number.
 * \param blocknum Try to find (and retrieve) a particular block (blocknum) on the list.
 */
static buff_header_t *get_free_blk(buff_hashq_t *queue, int device, uint64_t blocknum)
{
	struct _buffer_header_t *head, *tmp, *prev, *next;

	/* Search for the block */
	head = queue->freelist_head; 
	tmp = head->free_next;

	if (tmp == head) {
		/* there are no free buffers on the list */
		return NULL;
	}

	/* try to find the block on the free list */
	while (tmp != head) {
		if (tmp->addr == blocknum && tmp->device == device) {
			break;
		}
		tmp = tmp->free_next;
	}
	if (tmp == head) {
		/* Block is not on free list, pick up any block */
		tmp = head->free_next;
	}
	
	/* remove block from free list */
	cli();
	prev = tmp->free_prev;
	next = tmp->free_next;
	prev->free_next = next;
	next->free_prev = prev;
	sti();

	return tmp;
}


/**
 * Remove buffer from old hash queue and insert onto new hash queue
 *
 * \param queue The new hash queue.
 * \param buff The Buffer.
 * \param device Device number.
 * \param blocknum New block number.
 */
static void add_to_buff_queue(buff_hashq_t *queue, buff_header_t *buff, int device, uint64_t blocknum)
{
	struct _buffer_header_t *head, *tmp, *prev, *next;
	uint64_t pos;

	pos = buff->addr % 4;
	head = queue->hashtable[pos];
	if (head != NULL) {
		if (search_blk(queue, device, buff->addr) != NULL) {
			/* Remove from old hash queue */
			tmp = head;
			if (tmp->next == NULL) {
				queue->hashtable[pos] = NULL;
			} else {
				while(tmp->next != NULL) {
					if (tmp->addr == buff->addr && tmp->device == device) {
						cli();
						prev = tmp->prev;
						if (tmp->next == NULL) {
							next = NULL;
						} else {
							next = tmp->next;
							next->prev = prev;
						}
						prev->next = next;
						sti();
					}
					tmp = tmp->next;
				}
			}
		}
	}
	
	/* Add to new hash queue */
	pos  = blocknum % 4;
	head = queue->hashtable[pos];
	buff->addr   = blocknum;
	buff->device = device;

	if (head == NULL) {
		buff->next = NULL;
		buff->prev = buff;
		queue->hashtable[pos] = buff; 
		return;
	} else if(head->next == NULL) {
		cli(); 
		head->next = buff;
		head->prev = NULL;
		buff->next = NULL;
		buff->prev = head;
		sti();
		return;
	}

	tmp = head;
	while(tmp->next != NULL) tmp = tmp->next;

	cli();
	buff->prev = tmp;
	buff->next = NULL;
	tmp->next  = buff;
	sti();

	return;
}


/**
 * getblk
 *
 * Each disk has a buffer queue of blocks. This function will search for a
 * specific block in the disk's buffer queue.
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

	driver = block_dev_drivers[major]; 

	while(1) {
	
		if ( (buff = search_blk(driver->buffer_queue, device, blocknum)) != NULL ) {
			/* Block is in hash queue */
			
			if (buff->status == BUFF_ST_BUSY) {
				sleep_on(WAIT_THIS_BLOCK_BUFFER_GET_FREE);
				continue;
			}
			
			/* Remove buffer from free list and set as busy */
			cli();
			buff->status = BUFF_ST_BUSY;
			sti();
			remove_from_freelist(driver->buffer_queue, device, blocknum);
			return buff;
		} else {
			/* Block is not on hash queue */
			
			/* There are no free buffers on free list */
			if ( (buff = get_free_blk(driver->buffer_queue, device, blocknum)) == NULL ) {
				sleep_on(WAIT_BLOCK_BUFFER_GET_FREE);
				continue;
			} else {

				/* Buffer should be flushed to disk */
				if (buff->status == BUFF_ST_FLUSH) {
					/* asynchronous wirte buffer to disk */
					driver->dev_ops->write_async_block(major, device, buff);
					continue;
				}

				/* Remove buffer from old hash queue and put block
				   onto new hash queue */
				add_to_buff_queue(driver->buffer_queue, buff, device, blocknum);

				return buff;
			}
		}
	}
}


/**
 * brelse
 *
 * Release a locked buffer.
 * \param major Major number of the device
 * \param device Minor number (device number)
 * \param buff The buffer to be released.
 */
void brelse(int major, int device, buff_header_t *buff)
{
	dev_blk_driver_t *driver;
	buff_header_t *head, *tmp;

	driver = block_dev_drivers[major]; 

	wakeup(WAIT_BLOCK_BUFFER_GET_FREE);
	wakeup(WAIT_THIS_BLOCK_BUFFER_GET_FREE);
	
	cli();

	head = driver->buffer_queue->freelist_head;
	
	if (buff->status == BUFF_ST_VALID) {
		/* enqueue buffer at end of free list */
		tmp = head->free_prev;
		buff->free_prev = tmp;
		buff->free_next = head;
		head->free_prev = buff;
		tmp->free_next  = buff;
	} else {
		/* enqueue buffer at beginning of free list */
		tmp = head->free_next;
		buff->free_next = tmp;
		buff->free_prev = head;
		head->free_next = buff;
		tmp->free_prev  = buff;
	}

	sti();

	buff->status = BUFF_ST_UNLOCKED;
}


/**
 * bread
 *
 * Read a specific block from device (handling the cache).
 */
buff_header_t *bread(int major, int device, uint64_t blocknum)
{
	buff_header_t *buff;
	dev_blk_driver_t *driver;

	driver = block_dev_drivers[major]; 

	if ((buff = getblk(major, device, blocknum)) == NULL) {
		kprintf(KERN_ERROR "bread(): Error on get cached block.\n");
		return NULL;
	}

	if (buff->status != BUFF_ST_VALID) {
		/* Read from device (synchronous) */
		if (driver->dev_ops->read_block(major, device, buff) < 0) {
			kprintf(KERN_ERROR "Error on reading block from device: MAJOR = %d | MINOR = %d", major, device);
		}
	}

	return buff;
}

