/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: timer.c
 * Desc: Time system of TempOS
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

#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <tempos/sched.h>
#include <linkedl.h>
#include <semaphore.h>
#include <unistd.h>
#include <arch/irq.h>
#include <arch/io.h>

/** Queue of alarms */
llist *alarm_queue;


_pushargs void timer_handler(int i, pt_regs *regs);


/**
 * Contains the number of system clock ticks
 *
 * \note System frequency is expressed in hertz by HZ defined at compilation.
 * \see tempos/timer.h
 */
volatile uint32_t jiffies;

/**
 * Initialize time system
 */
void init_timer(void)
{
	jiffies = 0;

	kprintf(KERN_INFO "Initializing timer...\n");

	llist_create(&alarm_queue);

	if( request_irq(TIMER_IRQ, timer_handler, 0, "PIT") < 0 ) {
		kprintf(KERN_ERROR "Error on initialize PIT\n");
	}
}


/**
 * Interrupt handler
 */
_pushargs void timer_handler(int i, pt_regs *regs)
{
	llist *tmp;
	alarm_t *alarm;
	uint32_t pos;

	jiffies++;

	/*
 	 * Check and execute handlers of expired alarms
 	 */
	tmp = alarm_queue;
	pos = 0;
	foreach(alarm_queue, tmp) {
		alarm = (alarm_t*)tmp->element;

		if( time_after(jiffies, alarm->expires) ) {
			/* Execute handler and remove from list */
			llist_remove_nth(&alarm_queue, pos);
			alarm->handler(regs, alarm->arg);
		}

		pos++;
	}

	/*
	 * Call schedule()
	 */
	schedule(regs);
}


/**
 * Create a new alarm
 *
 * \param expires When alarm will expire (expressed in jiffies)
 * \param handler The function to be executed when alarm expires.
 * \param arg Argument to be passed to handler function.
 */
int new_alarm(uint32_t expires, void (*handler)(pt_regs *, void *), void *arg)
{
	alarm_t *nalarm;

	if(expires < jiffies) {
		return(0);
	} else {
		nalarm = (alarm_t*)kmalloc(sizeof(alarm_t), GFP_NORMAL_Z);
		if(nalarm == NULL) {
			return(0);
		} else {
			nalarm->expires = expires;
			nalarm->handler = handler;
			nalarm->arg     = arg;
			/* Interrupt routines can access and change linked lists, so 
			   we disable interrupts to keep the consistency */
			cli();
				llist_add(&alarm_queue, nalarm);
			sti();
		}
	}

	return(1);
}


