/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: task.c
 * Desc: Implements functions to create/handle tasks.
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
#include <x86/x86.h>
#include <x86/io.h>

#define load_esp(x) __asm__ __volatile__("movl %%esp, %0" : "=r"(x))

#define load_eflags(x) __asm__ __volatile__("pushfl ; popl %0" : "=r"(x) :: "eax")

#define push_into_stack(task, arg) { task->kstack -= sizeof(arg); \
										memcpy(task->kstack, &arg, sizeof(arg)); }


extern pagedir_t *kerneldir;

/**
 * This is the low level routine to make context switch
 * \see arch/x86/task.S
 */
void task_switch_to(arch_tss_t *task);
void initial_task(task_t *task);

arch_tss_t *arch_tss_cur_task;

/**
 * Configure and start the first kernel thread.
 * \param start_routine Pointer to the function which will be executed.
 */
void arch_init_scheduler(void (*start_routine)(void*))
{
	uint32_t eflags;
	task_t *newth = NULL;

	/* Alloc memory for task structure */
	newth = (task_t*)kmalloc(sizeof(task_t), GFP_NORMAL_Z);
	if (newth == NULL) {
		return;
	}

	load_eflags(eflags);
	newth->state       = TASK_RUNNING;
	newth->priority    = DEFAULT_PRIORITY;
	newth->pid         = KERNEL_PID;
	newth->return_code = 0;
	newth->wait_queue  = 0;

	newth->arch_tss.regs.eip = (uint32_t)start_routine;
	newth->arch_tss.regs.ds  = KERNEL_DS;
	newth->arch_tss.regs.fs  = KERNEL_DS;
	newth->arch_tss.regs.gs  = KERNEL_DS;
	newth->arch_tss.regs.ss  = KERNEL_DS;
	newth->arch_tss.regs.es  = KERNEL_DS;
	newth->arch_tss.regs.cs  = KERNEL_CS;
	newth->arch_tss.cr3 = (uint32_t)kerneldir->dir_phy_addr; /* physical address */
	newth->arch_tss.regs.eflags = (eflags | EFLAGS_IF); /* enable interrupts */

	cli();
	c_llist_add(&tasks, newth);
	cur_task = tasks;
	arch_tss_cur_task = &newth->arch_tss;
	sti();

	/* Jump to main thread */
	load_esp(newth->arch_tss.regs.esp);
	newth->kstack = (char*)(newth->arch_tss.regs.esp);
	initial_task(newth);
}

void setup_task(task_t *task, void (*start_routine)(void*))
{
	if (task == NULL) {
		return;
	}

	task->arch_tss.regs.eip = (uint32_t)start_routine;
	task->arch_tss.regs.ds  = KERNEL_DS;
	task->arch_tss.regs.fs  = KERNEL_DS;
	task->arch_tss.regs.gs  = KERNEL_DS;
	task->arch_tss.regs.ss  = KERNEL_DS;
	task->arch_tss.regs.es  = KERNEL_DS;
	task->arch_tss.regs.cs  = KERNEL_CS;
	task->arch_tss.cr3 = (uint32_t)kerneldir->dir_phy_addr; /* physical address */

	task->arch_tss.regs.eflags = 0x2020000;
	
	/* Setup thread context into stack */
	task->arch_tss.regs.esp = (uint32_t)task->kstack - (11 * sizeof(uint32_t)) - (6 * sizeof(uint16_t));
	
	/* Configure thread's stack */
	push_into_stack(task, task->arch_tss.regs.eflags);
	push_into_stack(task, task->arch_tss.regs.cs);
	push_into_stack(task, task->arch_tss.regs.eip);
	push_into_stack(task, task->arch_tss.regs.eax);
	push_into_stack(task, task->arch_tss.regs.ecx);
	push_into_stack(task, task->arch_tss.regs.edx);
	push_into_stack(task, task->arch_tss.regs.ebx);
	push_into_stack(task, task->arch_tss.regs.esp);
	push_into_stack(task, task->arch_tss.regs.ebp);
	push_into_stack(task, task->arch_tss.regs.esi);
	push_into_stack(task, task->arch_tss.regs.edi);
	push_into_stack(task, task->arch_tss.regs.ds);
	push_into_stack(task, task->arch_tss.regs.es);
	push_into_stack(task, task->arch_tss.regs.fs);
	push_into_stack(task, task->arch_tss.regs.gs);
	push_into_stack(task, task->arch_tss.regs.ss);
	push_into_stack(task, task->arch_tss.cr3);

	return;
}

/**
 * Switch task
 */
void switch_to(c_llist *tsk)
{
	task_t *task = GET_TASK(tsk);
	task_t *current_task = GET_TASK(cur_task);

	if (current_task == NULL || task == NULL) {
		return;
	}
	
	/* Change context to the new task */
	current_task->state = TASK_READY_TO_RUN;
	arch_tss_cur_task = &current_task->arch_tss;
	cur_task = tsk;
	task->state = TASK_RUNNING;
	task_switch_to(&task->arch_tss);
}

