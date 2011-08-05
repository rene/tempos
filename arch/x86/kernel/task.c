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

extern pagedir_t *kerneldir;

/**
 * This is the low level routine to make context switch
 * \see arch/x86/task.S
 */
void jmp_to_task(task_t task);

/**
 *
 *
 */
void arch_init_scheduler(void (*start_routine)(void*))
{
	uint32_t esp, eflags;
	task_t *newth = NULL;

	/* Alloc memory for task structure */
	newth = (task_t*)kmalloc(sizeof(task_t), GFP_NORMAL_Z);
	if (newth == NULL) {
		return;
	}

	asm("movl %%esp, %0 \n"
		"pushf          \n"
		"popl %1" : "=r" (esp), "=r" (eflags)); 
 
	newth->state       = TASK_RUNNING;
	newth->priority    = DEFAULT_PRIORITY;
	newth->pid         = KERNEL_PID;
	newth->return_code = 0;
	newth->wait_queue  = 0;
	newth->stack       = (size_t*)esp;

	newth->arch_tss.regs.eip = (uint32_t)start_routine;
	newth->arch_tss.regs.ds  = KERNEL_DS;
	newth->arch_tss.regs.fs  = KERNEL_DS;
	newth->arch_tss.regs.gs  = KERNEL_DS;
	newth->arch_tss.regs.ss  = KERNEL_DS;
	newth->arch_tss.regs.es  = KERNEL_DS;
	newth->arch_tss.regs.esp = (uint32_t)newth->stack;
	newth->arch_tss.regs.cs  = KERNEL_CS;
	newth->arch_tss.cr3 = (uint32_t)kerneldir->dir_phy_addr; /* physical address */
	newth->arch_tss.regs.eflags = (eflags | EFLAGS_IF); /* enable interrupts */

	c_llist_add(&tasks, newth);
	cur_task = tasks;

	jmp_to_task(*newth);
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
	task->arch_tss.regs.esp = (uint32_t)task->stack;
	task->arch_tss.regs.cs  = KERNEL_CS;
	task->arch_tss.cr3 = (uint32_t)kerneldir->dir_phy_addr; /* physical address */
	task->arch_tss.regs.eflags = 0x2020000;

	return;
}

/**
 * Switch task
 */
void switch_to(pt_regs *regs, c_llist *tsk)
{
	task_t *current_task = GET_TASK(cur_task);
	task_t *task = GET_TASK(tsk);

	if (current_task == NULL || task == NULL) {
		return;
	}
	
	/* Save current task registers */
	current_task->arch_tss.regs.eip = regs->eip;
	current_task->arch_tss.regs.ss  = regs->ss;
	current_task->arch_tss.regs.gs  = regs->gs;
	current_task->arch_tss.regs.fs  = regs->fs;
	current_task->arch_tss.regs.es  = regs->es;
	current_task->arch_tss.regs.ds  = regs->ds;
	current_task->arch_tss.regs.esp = regs->esp + 26;

	/*uint32_t val;
	asm("movl (%1), %%eax \n"
		"movl %%eax, %0" : "=r"(val) : "r"(current_task->arch_tss.regs.esp) : "eax");
	kprintf("Valor de (%%esp): 0x%x\n", val);
	kprintf("Valor de EIP:   0x%x\n", regs->eip);
	while(1);*/

	current_task->arch_tss.regs.eax = regs->eax;
	current_task->arch_tss.regs.ecx = regs->ecx;
	current_task->arch_tss.regs.edx = regs->edx;
	current_task->arch_tss.regs.ebx = regs->ebx;

	current_task->state = TASK_READY_TO_RUN;
	current_task->arch_tss.regs.eflags = regs->eflags;

	/* Change context to the new task */
	cur_task = tsk;
	task->state = TASK_RUNNING;
	jmp_to_task(*task);
}

