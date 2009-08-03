/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: linkedl.h
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

#ifndef LLIST_H

	#define LLIST_H

	#include <tempos/kernel.h>
	#include <tempos/mm.h>
	#include <stdlib.h>
	#include <string.h>


	#define foreach(list, tmp) for(tmp = list; tmp != NULL; tmp = tmp->next)


	/* Singly-linked list */
	struct _llist {
		void *element;
		struct _llist *next;
	} __attribute__ ((packed));


	/* Circular-linked list */
	struct _c_llist {
		void *element;
		struct _c_llist *prev;
		struct _c_llist *next;
	} __attribute__ ((packed));


	typedef struct _llist   llist;
	typedef struct _c_llist c_llist;


	int llist_create(llist **list);

	int llist_destroy(llist **list);

	int llist_add(llist **list, void *element);

	int llist_remove_nth(llist **list, uint32_t pos);

	int llist_remove(llist **list, void *element);

	void *llist_nth(llist *list, uint32_t index);

	int32_t llist_index(llist *list, void *element);

	int32_t llist_length(llist *list);


	int c_llist_create(c_llist **list);

	int c_llist_destroy(c_llist **list);

	int c_llist_add(c_llist **list, void *element);

	int c_llist_remove_nth(c_llist **list, uint32_t pos);

	int c_llist_remove(c_llist **list, void *element);

	void *c_llist_nth(c_llist *list, uint32_t index);

	int32_t c_llist_index(c_llist *list, void *element);

	int32_t c_llist_length(c_llist *list);

#endif /* LLIST_H */

