/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: clinkedl.c
 * Desc: Circular linked list
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
 * Create a circular linked list
 */
int c_llist_create(c_llist **list)
{
	*list = NULL;
	return(1);
}


/**
 * Destroy (free memory) a list.
 */
int c_llist_destroy(c_llist **list)
{
	c_llist *head = *list;
	c_llist *tmp, *aux;

	if(head == NULL)
		return(1);

	aux       = head->prev;
	aux->next = NULL;

	tmp = head;
	while(tmp != NULL) {
		aux = tmp->next;
		kfree(tmp);
		tmp = aux;
	}

	return(1);
}

/**
 * Add a element into the list
 *
 * \param list List.
 * \param element Element to be inserted.
 */
int c_llist_add(c_llist **list, void *element)
{
	c_llist *head = *list;
	c_llist *new_node, *last;

	new_node = (c_llist*)kmalloc(sizeof(c_llist), GFP_NORMAL_Z);
	if(new_node == NULL) {
		return(0);
	} else {
		new_node->element = element;
	}


	/* First element */
	if(head == NULL) {
		new_node->next = new_node;
		new_node->prev = new_node;
		head           = new_node;
	} else  {
		/* Add to the end */
		last           = head->prev;
		last->next     = new_node;
		new_node->next = head;
		new_node->prev = last;
		head->prev     = new_node;
	}

	*list = head;
	return(1);
}

/**
 * Remove the nth element from the list
 *
 * \param list List.
 * \param pos Element position.
 */
int c_llist_remove_nth(c_llist **list, uint32_t pos)
{
	c_llist *head = *list;
	c_llist *aux1, *aux2, *tmp;
	uint32_t p;

	if(head == NULL)
		return(0);

	/* Remove first element */
	if(pos == 0) {

		aux1 = head->next;
		aux2 = head->prev;

		aux1->prev = aux2;
		aux2->next = aux1;

		*list = aux1;
		return(1);
	}

	/* Remove element */
	for(tmp = head->next, p = 0; tmp != head && p < pos;
			p++, tmp = tmp->next);

	tmp = tmp->prev;
	if(tmp != head) {
		aux1 = tmp->next;
		aux2 = tmp->prev;

		aux1->prev = aux2;
		aux2->next = aux1;

		kfree(tmp);
		*list = head;
		return(1);
	} else {
		return(0);
	}
}

/**
 * Remove a element from the list
 *
 * \param list List.
 * \parma element Element to remove.
 */
int c_llist_remove(c_llist **list, void *element)
{
	c_llist *head = *list;
	c_llist *tmp, *aux1, *aux2;

	if(head == NULL)
		return(0);

	/* Remove first element */
	if(head->element == element) {

		aux1 = head->next;
		aux2 = head->prev;

		aux1->prev = aux2;
		aux2->next = aux1;

		kfree(head);
		*list = aux1;
		return(1);
	}

	/* Remove element */
	for(tmp = head->next; tmp != head; tmp = tmp->next) {
		if(tmp->element == element) {
			break;
		}
	}

	if(tmp != head) {
		aux1 = tmp->next;
		aux2 = tmp->prev;

		aux1->prev = aux2;
		aux2->next = aux1;

		kfree(tmp);
		*list = head;
		return(1);
	} else {
		return(0);
	}
}

/**
 * Return the element at nth position in the list
 *
 * \param list List.
 * \param index Element position.
 * \return The element or NULL if not found.
 */
void *c_llist_nth(c_llist *list, uint32_t index)
{
	c_llist *head = list;
	c_llist *tmp;
	uint32_t p;

	if(list == NULL)
		return(NULL);

	if(index == 0) {
		return(head->element);
	}

	for(tmp = head->next, p = 0; tmp != head && p < index;
			p++, tmp = tmp->next);

	if(p == index) {
		return(tmp->element);
	} else {
		return(NULL);
	}
}

/**
 * Return the index of a element in the list
 *
 * \param list List.
 * \param element Element to find the index.
 * \return -1 if element was not found, the index otherwise.
 */
int32_t c_llist_index(c_llist *list, void *element)
{
	c_llist *tmp;
	int32_t p;

	if(list == NULL)
		return(-1);

	/* First element */
	if(list->element == element) {
		return(0);
	}

	/* Search in list */
	for(tmp = list->next, p = 1; tmp != list; p++, tmp = tmp->next) {
		if(tmp->element == element) {
			break;
		}
	}

	if(tmp != list) {
		return(p);
	} else {
		return(-1);
	}
}

/**
 * Return the length of a circular linked list
 *
 * \param list List
 */
int32_t c_llist_length(c_llist *list)
{
	c_llist *head  = list;
	c_llist *tmp;
	int32_t size = 0;

	if(head == NULL)
		return(0);

	for(tmp = head->next, size=1; tmp != head;
			size++, tmp = tmp->next);

	return(size);
}


