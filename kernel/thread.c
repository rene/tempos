/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: thread.c
 * Desc: Implements functions to create/handle threads.
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

#include <tempos/sched.h>
#include <tempos/kernel.h>
#include <tempos/mm.h>
#include <arch/io.h>
#include <linkedl.h>
#include <string.h>


task_t *kernel_thread_create(int priority, void *(*start_routine)(void *), void *arg)
{
	task_t *newth = NULL;
	size_t *stack = NULL;

	/* Alloc memory for task structure */
	newth = (task_t*)kmalloc(sizeof(task_t), GFP_NORMAL_Z);
	if (newth == NULL) {
		return NULL;
	}

	/* Alloc memory for process stack */
	stack = (size_t*)kmalloc(PROCESS_STACK_SIZE, GFP_NORMAL_Z);
	if (stack == NULL) {
		kfree(newth);
		return NULL;
	}

	newth->state = TASK_READY_TO_RUN;
	newth->priority = priority;
	newth->pid = KERNEL_PID;
	newth->return_code = 0;
	newth->wait_queue = 0;
	newth->stack = &stack[PROCESS_STACK_SIZE-1];

	/* "push" start_routine argument */
	memcpy(&newth->stack[PROCESS_STACK_SIZE-1], arg, sizeof(size_t));

	/* Architecture specific */
	setup_task(newth, start_routine);

	/* Add to task queue */
	c_llist_add(&tasks, newth);

	/* Return */
	return newth;
}

void kernel_thread_exit(int return_code)
{
	cli();

	sti();
}

