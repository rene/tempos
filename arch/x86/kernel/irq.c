/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: irq.c
 * Desc: Architecture dependent functions to handle IRQs
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

#include <tempos/mm.h>
#include <string.h>
#include <x86/irq.h>
#include <x86/io.h>
#include <x86/i8259A.h>


/** handlers queue for each IRQ */
irq_queue_t irq_list[N_IRQ];


/**
 * Start IRQ handler system
 */
void init_IRQ(void)
{
	uint16_t i;

	for(i=0; i<N_IRQ; i++) {
		irq_list[i].irqnum = i;
		irq_list[i].flags  = SA_SHIRQ;
		llist_create(&irq_list[i].queue);
	}
}


/**
 * Manager for IRQ handlers. This function is called
 * on every IRQ interrupt (see arch/x86/isr.S).
 */
void do_irq(uint32_t irqnum, pt_regs regs)
{
	irq_queue_t *queue;
	irq_handler_t *handler;
	llist *tmp;

	if(irqnum < N_IRQ) {
		queue = &irq_list[irqnum];
	} else {
		return;
	}

	/* Call each handler */
	foreach(queue->queue, tmp) {
		handler = (irq_handler_t *)tmp->element;
		if(handler != NULL)
			handler->handler(handler->id, &regs);
	}


	/* Send EOI to the slave PIC (when necessary) */
	if(irqnum > 7)
		outb(PIC_EOI, PIC_SLAVE_CMD);

	/* Send EOI to the master PIC */
	outb(PIC_EOI, PIC_MASTER_CMD);
}


/**
 * Register an IRQ handler
 *
 * This function it's used by device drivers
 */
int request_irq(uint16_t irq, void (*handler)(int, pt_regs *),
						uint16_t flags, const char *name)
{
	irq_queue_t *queue;
	irq_handler_t *newh, *htmp;
	llist *tmp;

	if(irq < N_IRQ) {
		queue = &irq_list[irq];
	} else {
		return(-1);
	}

	/* Check if IRQ could be shared */
	if( (flags & SA_SHIRQ) ) {
		if( !(queue->flags & SA_SHIRQ) )
			return(-1);
	} else {
		if(queue->queue != NULL) {
			/* IRQ are already used */
			return(-1);
		} else {
			queue->flags &= ~SA_SHIRQ;
		}
	}

	/* Create newh */
	newh = (irq_handler_t *)kmalloc(sizeof(irq_handler_t), GFP_NORMAL_Z);
	if(newh == NULL)
		return(-1);

	if(strlen(name) < MAX_IRQH_NAME) {
		strcpy(newh->name, name);
	} else {
		strncpy(newh->name, name, MAX_IRQH_NAME);
	}

	newh->handler = handler;

	/* Generate a ID */
	tmp = queue->queue;
	if(tmp == NULL) {
		newh->id = 1;
	} else {
		while(tmp->next != NULL)
				tmp = tmp->next;

		htmp = (irq_handler_t *)tmp->element;
		if(htmp != NULL) {
			newh->id = htmp->id + 1;
		} else {
			return(-1);
		}
	}

	/* Install handler */
	llist_add(&queue->queue, newh);

	return(1);
}


