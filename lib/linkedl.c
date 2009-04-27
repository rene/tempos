/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
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


uchar8_t llist_create(llist **list)
{
	llist *tmp = *list;
	tmp   = NULL;
	*list = tmp;
	return(1);
}


uchar8_t llist_add(llist **list, void *element)
{
	llist *plist = *list;
	llist *new_node;
	llist *tmp;

	/* Alloc memory */
	new_node = (llist *)kmalloc(sizeof(llist), GFP_NORMAL_Z);
	if(new_node == 0) {
		return(0);
	}

	/* Add to list */
	new_node->element = element;
	new_node->next    = NULL;

	tmp = plist;
	if(tmp != NULL) {
		/* Add to the end of list */
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
	} else {
		/* Head of list */
		plist = new_node;
	}

	*list = plist;
	return(1);
}


uchar8_t llist_remove(llist **list, uint32_t pos)
{
	llist *_list = *list;
	llist *prev, *next, *tmp;
	uint32_t i, size;

	size = llist_get_size(_list);

	if(pos >= size)
		return(0);

	if(_list != NULL) {
		if(pos == 0) {
			/* Element on the top */
			next  = _list->next;
			kfree(_list);
			_list = next;
		} else if(pos < (size - 1)) {
			/* Element at the middle */
			//
		} else {
			/* Element at the end */
			tmp = _list;

			for(i=1; i<pos; i++) {
				tmp = tmp->next;
			}

			prev       = tmp;
			prev->next = NULL;
			kfree(tmp->next);
		}
	}

	*list = _list;
	return(1);
}


uint32_t llist_get_size(llist *list)
{
	llist *_list = list;
	uint32_t size      = 0;

	while(_list != NULL) {
		size++;
		_list = _list->next;
	}

	return(size);
}


uchar8_t llist_destroy(llist **list)
{
	llist *_list;
	uint32_t size;
	uint32_t pos;

	_list = *list;
	size = llist_get_size(_list);
	for(pos=0; pos<size; pos++)
		llist_remove(&_list, 0);

	return(1);
}

