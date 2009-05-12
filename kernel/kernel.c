/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: kernel.c
 * Desc: The TempOS kernel
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

#include <tempos/kernel.h>
#include <tempos/mm.h>
#include <drv/i8042.h>
#include <string.h>
#include <stdlib.h>
#include <linkedl.h>


/* information passed from first stage */
karch_t kinfo;

extern uint32_t *kpagedir;


/**
 * tempos_main
 *
 * This is the function called when first stage is done, which means that
 * all dependent machine boot code was executed. See arch/$ARCH/boot/karch.c
 */
void tempos_main(karch_t kinf)
{
	/* Start the second stage */
	init_8042();

	/* For test */
	llist *mylist, *mylist2, *tmp;
	memcpy(&kinfo, &kinf, sizeof(karch_t));

	kprintf("We are in TempOS kernel!\n");
	kprintf("Command line passed: %s\n", kinfo.cmdline);

	llist_create(&mylist);
	llist_create(&mylist2);

	llist_add(&mylist, "Hello");
	llist_add(&mylist2, "It's amazing");
	llist_add(&mylist, "World");
	llist_add(&mylist2, " when you do things ");
	llist_add(&mylist, "from TempOS !!!");
	llist_add(&mylist2, "that it's hard to do!\n");

	tmp = mylist;
	while(tmp != NULL) {
		kprintf("%s ", tmp->element);
		tmp = tmp->next;
	}

	llist_destroy(&mylist);

	kprintf("\n");
	tmp = mylist2;
	while(tmp != NULL) {
		kprintf("%s ", tmp->element);
		tmp = tmp->next;
	}

	llist_destroy(&mylist2);

	for(;;);
}


void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	for(;;);
}

