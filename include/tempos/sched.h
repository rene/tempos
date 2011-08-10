/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: sched.h
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

#ifndef SCHED_H

	#define SCHED_H

	#include <sys/types.h>
	#include <unistd.h>
	#include <arch/task.h>
	#include <linkedl.h>

	/** Task state: Ready to run */
	#define TASK_READY_TO_RUN 0x00
	/** Task state: Running */
	#define TASK_RUNNING      0x01
	/** Task state: Stopped */
	#define TASK_STOPPED      0x02
	/** Task state: Zombie */
	#define TASK_ZOMBIE       0x03

	/** Default priority */
	#define DEFAULT_PRIORITY  0

	/** PID of kernel threads */
	#define KERNEL_PID 0
	
	/** PID of init */
	#define INIT_PID 1

	/** Process's stack size */
	#define PROCESS_STACK_SIZE STACK_SIZE

	/** Return cur_task circular linked list element (or NULL) */
	#define GET_TASK(a) (a == NULL ? NULL : (task_t*)a->element)

	/**
	 * Process structure. This structure holds all information about a
	 * process, PID, state, and also its context.
	 */
	struct _task_struct {
		/** Process state */
		int state;
		/** Process priority */
		int priority;
		/** Process ID */
		pid_t pid;
		/** Process kernel stack */
		size_t *kstack;
		/** Return code */
		int return_code;
		/** Wait queue */
		int wait_queue;
		/** Architecture dependent */
		arch_tss_t arch_tss;
	};

	typedef struct _task_struct task_t;

	extern c_llist *tasks;
	extern c_llist *cur_task;

	void init_scheduler(void (*start_routine)(void*));

	void schedule(pt_regs *regs);

	task_t *kernel_thread_create(int priority, void (*start_routine)(void *), void *arg);
	void kernel_thread_exit(int return_code);
	int kernel_thread_wait(task_t *th);


	/* These are Architecture specific */
	void arch_init_scheduler(void (*start_routine)(void*));
	void setup_task(task_t *task, void (*start_routine)(void *));
	void switch_to(c_llist *tsk);

#endif /* SCHED_H */

