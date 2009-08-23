/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: dvcache.c
 * Desc: Implement the dvcache
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

#include <tempos/fs/dvhash.h>

/**
 * DVHASH - The Double Vector Hash
 *
 * DVHASH it's a simple solution for the Buffer Cache of TempOS. It consists
 * of one vector where each position points to another vector (this why it
 * called double vector). Each position of the second vector points to a
 * circular linked list of data blocks.
 *
 * The hash function works based on the total blocks of the device.
 * First, the cache needs to be initialized passing the total of addresseable
 * blocks of the device. For example:
 *
 * Device: Hard disk with 268435456 sectors (127GB)
 *  First vector size (configurable): 1024 entries
 * 	Second vector size (configurable): 1024 entries
 *
 *  TOTAL_BLOCKS = 268435456
 *  VECTOR1_SIZE = 1024
 *  VECTOR2_SIZE = 1024
 *
 *  So on initialization we'll have:
 *
 * 		A = TOTAL_BLOCKS / VECTOR1_SIZE
 * 		B = A / VECTOR2_SIZE
 *
 * To find an block on the hash, DVHASH will do the follow calculation:
 *
 * 		FINDEX = BLOCK_ADDR / A
 * 		SINDEX = (BLOCK_ADDR - (FINDEX * A)) / B
 *
 *  FINDEX is the position at first vector and SINDEX is the position
 *  at the second. Than, DVHASH will just look at the circular linked
 *  list present at VECTOR2[SINDEX] and search for the block. If the
 *  block was not found, NULL is returned, otherwise the block is
 *  returned.
 */

int initialize_dvhash(dvhash_t ** dvect, uint32_t v1_size, uint32_t v2_size,
		      uint16_t major, uint64_t totalblocks)
{
	dvhash_t *dvector;
	c_llist **vector2;
	uint32_t i, j, k;

	/* Start DVHASH structure */
	dvector = (dvhash_t *) kmalloc(sizeof(dvhash_t), GFP_NORMAL_Z);
	if (dvector == NULL)
		return (-ENOMEM);

	dvector->major = major;

	if (v1_size >= MIN_V1SIZE && v2_size >= MIN_V2SIZE) {
		dvector->v1_size = v1_size;
		dvector->v2_size = v2_size;
	} else {
		dvector->v1_size = DEF_V1SIZE;
		dvector->v2_size = DEF_V2SIZE;
	}

	if (totalblocks > 0) {
		dvector->tblocks = totalblocks;
	} else {
		dvector->tblocks = 1;	/* Avoid division by zero */
	}

	/* Start vector 1 */
	dvector->vector1 =
	    (c_llist ***) kmalloc(sizeof(c_llist **) * dvector->v1_size,
				  GFP_NORMAL_Z);
	if (dvector->vector1 == NULL) {
		kfree(dvector);
		return (-ENOMEM);
	}

	/* Start each vetor 2 */
	for (i = 0; i < dvector->v1_size; i++) {

		vector2 =
		    (c_llist **) kmalloc(sizeof(c_llist *) * dvector->v2_size,
					 GFP_NORMAL_Z);
		if (vector2 == NULL)
			goto error;

		/* Start each linked list */
		for (j = 0; j < dvector->v2_size; j++) {

			if (!c_llist_create(&vector2[j])) {
				for (k = 0; k < j; k++)
					c_llist_destroy(&vector2[k]);
				kfree(vector2);
				goto error;
			}
		}

		dvector->vector1[i] = vector2;
	}

	/* Initialized, return */
	*dvect = dvector;
	return (1);

error:
	for (j = 0; j < i; j++) {
		vector2 = dvector->vector1[j];
		for (k = 0; k < dvector->v2_size; k++)
			c_llist_destroy(&vector2[k]);
		kfree(vector2);
	}
	kfree(dvector->vector1);
	kfree(dvector);
	return (-ENOMEM);
}
