/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: wait.c
 * Desc: sleep/wakeup functions.
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

#include <tempos/wait.h>
#include <arch/io.h>

/** List of wait addresses. Each position represents a wait address */
llist *wait_queues[WAIT_ADDRESS_SIZE];


/**
 * This functions initializes the wait queues.
 */
void init_wait_queues(void)
{
	int i;
	for (i = 0; i < WAIT_ADDRESS_SIZE; i++) {
		llist_create(&wait_queues[i]);
	}
}


/**
 * This function puts the process that called it to sleep
 * until the event represented by sleep_addr become available.
 *
 * \param sleep_addr The event that process should wait for.
 */
void sleep_on(int sleep_addr)
{
	task_t *current_task = GET_TASK(cur_task);

	/* sanity check */
	if (sleep_addr < 0 || sleep_addr >= WAIT_ADDRESS_SIZE) {
		return;
	}

	cli();
	
	current_task->state = TASK_STOPPED;

	llist_add(&wait_queues[sleep_addr], current_task);

	schedule();

	/* Process resumes execution from here when it wakes up */
	llist_remove(&wait_queues[sleep_addr], current_task);
	
	sti();

	return;
}


/**
 * Wake up all process sleeping waiting for sleep_addr resource(s).
 * \param sleep_addr Slepe address.
 * \note All process waiting for sleep_addr will be put in TASK_READY_TO_RUN state.
 */
void wakeup(int sleep_addr)
{
	task_t *task;
	llist *queue, *tmp;

	/* sanity check */
	if (sleep_addr < 0 || sleep_addr >= WAIT_ADDRESS_SIZE) {
		return;
	}

	cli();

	queue = wait_queues[sleep_addr];
	foreach(queue, tmp) {
		task = GET_TASK(tmp);

		task->state = TASK_READY_TO_RUN;
		llist_remove(&queue, tmp);
	}

	sti();
}

