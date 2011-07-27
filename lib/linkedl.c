/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: linkedl.c
 * Desc: Linked list
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

#include <linkedl.h>

/**
 * Create a new linked list
 * @param list New list
 * @return int Always return true
 */
int llist_create(llist **list)
{
	*list = NULL;
	return(1);
}


int llist_destroy(llist **list)
{
	llist *tmp, *aux;

	tmp = *list;
	if(tmp != NULL) {
		tmp = tmp->next;
		foreach(tmp, aux) {
			kfree(aux);
		}
		kfree(tmp);
	}

	*list = NULL;
	return(1);
}


int llist_add(llist **list, void *element)
{
	llist *rlist = *list;
	llist *new_node, *tmp, *prev;

	new_node = (llist*)kmalloc(sizeof(llist), GFP_NORMAL_Z);
	if(new_node == NULL) {
		return(0);
	} else {
		new_node->element = element;
		new_node->next    = NULL;
	}

	if(rlist == NULL) {
		/* First element */
		rlist = new_node;
	} else {
		/* Add to the end */
		for(tmp = prev = rlist; tmp != NULL;
				prev = tmp, tmp = tmp->next);

		if(prev != NULL)
			prev->next = new_node;
	}

	*list = rlist;
	return(1);
}


int llist_remove_nth(llist **list, uint32_t pos)
{
	llist *rlist = *list;
	llist *tmp, *prev;
	uint32_t p;

	/* Remove first element */
	if(pos == 0) {
		tmp   = rlist;
		rlist = rlist->next;
		kfree(tmp);
		*list = rlist;
		return(1);
	}

	/* Remove element */
	for(tmp = prev = rlist, p = 0; tmp != NULL && p < pos;
			p++, prev = tmp, tmp = tmp->next);

	if(prev != NULL) {
		if(tmp != NULL) {
			prev->next = tmp->next;
			kfree(tmp);
		} else {

		}
		*list = rlist;
		return(1);
	} else {
		return(0);
	}
}


int llist_remove(llist **list, void *element)
{
	llist *rlist = *list;
	llist *tmp, *prev;

	if(rlist == NULL)
		return(0);

	/* Remove first element */
	if(rlist->element == element) {
		tmp   = rlist;
		rlist = rlist->next;
		kfree(tmp);
		*list = rlist;
		return(1);
	}

	/* Remove element */
	for(tmp = prev = rlist; tmp != NULL;
			prev = tmp, tmp = tmp->next) {
		if(tmp->element == element) {
			break;
		}
	}

	if(tmp != NULL && prev != NULL) {
		prev->next = tmp->next;
		kfree(tmp);
		*list = rlist;
		return(1);
	} else {
		return(0);
	}
}


void *llist_nth(llist *list, uint32_t index)
{
	llist *tmp = list;
	uint32_t p;

	for(tmp = list, p = 0; tmp != NULL && p < index;
			p++, tmp = tmp->next);

	if(tmp != NULL) {
		return(tmp->element);
	} else {
		return(NULL);
	}
}


int32_t llist_index(llist *list, void *element)
{
	llist *tmp;
	int32_t p;

	if(list == NULL)
		return(-1);

	/* First element */
	if(list->element == element) {
		return(0);
	}

	/* Search in list */
	for(tmp = list, p = 0; tmp != NULL; p++, tmp = tmp->next) {
		if(tmp->element == element) {
			break;
		}
	}

	if(tmp != NULL) {
		return(p);
	} else {
		return(-1);
	}
}


int32_t llist_length(llist *list)
{
	llist *tmp   = list;
	int32_t size = 0;

	while(tmp != NULL) {
		size++;
		tmp = tmp->next;
	}

	return(size);
}


