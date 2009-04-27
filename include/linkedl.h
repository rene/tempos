/**
 * CASH - The C Again SHell
 *
 * Copyright (C)2007 Matheus Borges Teixeira   (arkanos.matheus at gmail.com)
 * Copyright (C)2007 Renê de Souza Pinto       (rene at grad.icmc.usp.br)
 * Copyright (C)2007 Thiago Camargo Fernandes  (thiagocf at grad.icmc.usp.br)
 *
 * linkedl.h
 *
 * Created: 18/05/2007 
 * Updated: 18/05/2007
 *
 *  This file is part of CASH, CASH is The C Again SHell.
 *
 *  CASH is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2.
 *
 *  CASH is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Bash; see the file COPYING.  If not, write to the Free
 *  Software Foundation, 59 Temple Place, Suite 330, Boston, MA 02111 USA.
 */

#ifndef LLIST_H

	#define LLIST_H

	#include <tempos/kernel.h>
	#include <tempos/mm.h>
	#include <stdlib.h>
	#include <string.h>


	struct _llist {
		void *element;  	  /* Element of node list  */
		struct _llist *next;  /* Next element on list  */
	} __attribute__ ((packed));

	typedef struct _llist llist;


	uchar8_t llist_create(llist **list);

	uchar8_t llist_add(llist **list, void *element);

	uchar8_t llist_remove(llist **list, uint32_t pos);

	uchar8_t llist_destroy(llist **list);

	uint32_t llist_get_size(llist *list);
	//uint32_t llist_search(linked_list *list, void *element, size_t size);

#endif /* LLIST_H */

