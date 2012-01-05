/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: fork.c
 * Desc: Syscall fork
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
#include <tempos/syscall.h>
#include <tempos/sched.h>
#include <arch/io.h>
#include <x86/x86.h>

/** Stack of PIDs numbers */
static pid_t pid_stack[MAX_NUM_PROCESS];

/** PID's stack top */
static uint32_t pid_st_top;


/**
 * Fork system call.
 */
_pushargs void sys_fork(void)
{
}

/**
 * This is just for test
 */
static uint32_t get_phy_addr(void *ptr)
{
	extern pagedir_t *kerneldir;
	mregion *mem_area = (mregion *)((void*)ptr - sizeof(mregion));
	uint32_t index    = mem_area->initial_addr >> TABLE_SHIFT;
	uint32_t ffpage   = mem_area->initial_addr - (TABLE_SIZE * index);
	uint32_t *table;

	table = kerneldir->tables[index];
	return (table[ffpage] & 0xFFFFF000);
}
void _exec_init(char *init_data)
{
	task_t *newth = NULL;
	char *new_stack = NULL;
	extern pagedir_t *kerneldir;
	pagedir_t *pg_pdir;
	uint32_t *ptable, *dtable, ptable_addr, dtable_addr;
	uint32_t i;

	/* Alloc memory for task structure */
	newth = (task_t*)kmalloc(sizeof(task_t), GFP_NORMAL_Z | GFP_USER);
	if (newth == NULL) {
		return;
	}

	/* Alloc memory for process's stack */
	new_stack = (char*)kmalloc(PROCESS_STACK_SIZE, GFP_NORMAL_Z | GFP_USER);
	if (new_stack == NULL) {
		kfree(newth);
		return;
	}

	/* Alloc memory for page table directory */
	pg_pdir = (pagedir_t*)kmalloc(sizeof(pagedir_t), GFP_NORMAL_Z | GFP_USER);
	if (pg_pdir == NULL) {
		kfree(newth);
		kfree(new_stack);
		return;
	}
	
	/* Set process structure */
	newth->state       = TASK_READY_TO_RUN;
	newth->priority    = DEFAULT_PRIORITY;
	newth->pid         = get_new_pid();
	newth->stack_base  = new_stack;
	newth->return_code = 0;
	newth->wait_queue  = 0;
	newth->kstack = (char*)((void*)new_stack + PROCESS_STACK_SIZE);

	newth->arch_tss.regs.eip = (uint32_t)0xC0000C; /* Start point */
	newth->arch_tss.regs.ds  = KERNEL_DS;
	newth->arch_tss.regs.fs  = KERNEL_DS;
	newth->arch_tss.regs.gs  = KERNEL_DS;
	newth->arch_tss.regs.ss  = KERNEL_DS;
	newth->arch_tss.regs.es  = KERNEL_DS;
	newth->arch_tss.regs.cs  = KERNEL_CS;

	/*
	newth->arch_tss.regs.ds  = USER_DS | USER_DPL;
	newth->arch_tss.regs.fs  = USER_DS | USER_DPL;
	newth->arch_tss.regs.gs  = USER_DS | USER_DPL;
	newth->arch_tss.regs.ss  = USER_DS | USER_DPL;
	newth->arch_tss.regs.es  = USER_DS | USER_DPL;
	newth->arch_tss.regs.cs  = USER_CS | USER_DPL;
*/
	newth->arch_tss.regs.eflags = 0x2020000;
	
	/* Setup thread context into stack */
	newth->arch_tss.regs.esp = (uint32_t)newth->kstack - sizeof(arch_tss_t) - sizeof(newth->arch_tss.regs.ss) - sizeof(newth->arch_tss.regs.esp);
	
	ptable_addr = alloc_page(GFP_NORMAL_Z);
	dtable_addr = alloc_page(GFP_NORMAL_Z);

	/* Map dtable at 0x1000000 just for fill data */
	kerneldir->tables[4][0] = MAKE_ENTRY(dtable_addr, (PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER));
	dtable = (uint32_t*)0x1000000;
	for (i = 0; i < 1024; i++) {
		pg_pdir->tables[i] = kerneldir->tables[i];
		dtable[i] = kerneldir->tables_phy_addr[i];
	}
	dtable[3] = MAKE_ENTRY(ptable_addr, (PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER));

	/* Now, map ptable at 0xC00000 just for fill data */
	kerneldir->tables[3][0] = MAKE_ENTRY(ptable_addr, (PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER));
	ptable = (uint32_t*)0xC00000;
	ptable[0] = MAKE_ENTRY(get_phy_addr(init_data), (PAGE_WRITABLE | PAGE_PRESENT | PAGE_USER));

	pg_pdir->tables_phy_addr = (uint32_t*)dtable_addr;
	pg_pdir->dir_phy_addr = dtable_addr;

	/* 12MB+12bytes : start point */
	//kerneldir->tables[3][0] = MAKE_ENTRY(get_phy_addr(init_data), (PAGE_PRESENT));
	//pg_pdir->dir_phy_addr = kerneldir->dir_phy_addr;

	newth->arch_tss.cr3 = pg_pdir->dir_phy_addr;

	/* Configure thread's stack */
	push_into_stack(newth->kstack, newth->arch_tss.regs.ss);
	push_into_stack(newth->kstack, newth->arch_tss.regs.esp);
	push_into_stack(newth->kstack, newth->arch_tss.regs.eflags);
	push_into_stack(newth->kstack, newth->arch_tss.regs.cs);
	push_into_stack(newth->kstack, newth->arch_tss.regs.eip);
	push_into_stack(newth->kstack, newth->arch_tss.regs.eax);
	push_into_stack(newth->kstack, newth->arch_tss.regs.ecx);
	push_into_stack(newth->kstack, newth->arch_tss.regs.edx);
	push_into_stack(newth->kstack, newth->arch_tss.regs.ebx);
	push_into_stack(newth->kstack, newth->arch_tss.regs.esp);
	push_into_stack(newth->kstack, newth->arch_tss.regs.ebp);
	push_into_stack(newth->kstack, newth->arch_tss.regs.esi);
	push_into_stack(newth->kstack, newth->arch_tss.regs.edi);
	push_into_stack(newth->kstack, newth->arch_tss.regs.ds);
	push_into_stack(newth->kstack, newth->arch_tss.regs.es);
	push_into_stack(newth->kstack, newth->arch_tss.regs.fs);
	push_into_stack(newth->kstack, newth->arch_tss.regs.gs);
	push_into_stack(newth->kstack, newth->arch_tss.regs.ss);
	push_into_stack(newth->kstack, newth->arch_tss.cr3);

	/* Add to task queue */
	cli();
	c_llist_add(&tasks, newth);
	sti();

	return;
}

/**
 * Fork a thread (process).
 *
 * \param thread The thread to fork.
 */
pid_t _fork(task_t *thread)
{
	task_t *newth = NULL;
	char *new_stack = NULL;
	pid_t child;

	/* Alloc memory for task structure */
	newth = (task_t*)kmalloc(sizeof(task_t), GFP_NORMAL_Z);
	if (newth == NULL) {
		return -1;
	}

	/* Alloc memory for process's stack */
	new_stack = (char*)kmalloc(PROCESS_STACK_SIZE, GFP_NORMAL_Z);
	if (new_stack == NULL) {
		kfree(newth);
		return -1;
	}

	/* Copy process structure */
	memcpy(newth, thread, sizeof(task_t));

	/* Alloc a PID */
	child = get_new_pid();
	newth->pid = child;

	/* Copy process's stack */
	memcpy(new_stack, thread->stack_base, PROCESS_STACK_SIZE);
	newth->stack_base = new_stack;
	/* FIXME: */
	if (GET_TASK(cur_task)->pid == child) {
		return 0;
	}

	/* Add to task queue */
	cli();
	c_llist_add(&tasks, newth);
	sti();

	/* This is the father, so return child's PID */
	return child;
}

/**
 * Retrieve a new PID.
 *
 * \return pid_t New PID.
 */
pid_t get_new_pid(void)
{
	return pid_stack[pid_st_top--];
}

/**
 * Release a allocated PID.
 *
 * \param pid PID.
 */
void release_pid(pid_t pid)
{
	pid_stack[++pid_st_top] = pid;
}

/**
 * Initialize stack of PIDs numbers.
 */
void init_pids(void)
{
	pid_t i, p;

	for (i = 0, p = 0; i < MAX_NUM_PROCESS; i++, p++) {
		if (p == KERNEL_PID || p == INIT_PID) {
			p++;
		}
		pid_stack[i] = p;
	}
	pid_st_top = i-1;
}

