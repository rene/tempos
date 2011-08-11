/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: task.S
 * Desc: Contains the low level code to make context switching
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

.global initial_task, task_switch_to
.extern arch_tss_cur_task

/**
 * This is function should be used only once to
 * Jump to the first kernel thread.
 */
initial_task:
	cli
	movl 4(%esp), %eax
	/**
	 * Since this function should be called just once at kernel
	 * initialization, and so at the kernel mode, we doesn't
	 * need to make level switch, because we are already in kernel mode,
	 * so here all we need it's ajust stack and jmp to main thread function
	 */
	movl 34(%eax), %ebx /* arch_tss.regs.esp    */
	movl 72(%eax), %ecx /* arch_tss.regs.eflags */
	movl 66(%eax), %edx /* arch_tss.regs.eip    */
	
	/* Ajust stack */
	movl  %ebx, %esp
	
	/* Push arguments for iret and call it */
	pushl %ecx
	pushw %cs
	pushl %edx
	sti
	iret


/**
 * void task_switch_to(task_t *task);
 * Make a context switch
 *
 * Here we have the stack of the current task with:
 *  ----------------
 * | return address | <-- ESP
 *  ----------------
 * | task_t *task   |
 *  ----------------
 * |      ...       |
 *  ----------------
 *
 * First, we will rearrange this stack to:
 *
 *  ----------------
 * | return address | <-- ESP
 *  ----------------
 * |       CS       |
 *  ----------------
 * |     EFLAGS     |
 *  ----------------
 * | task_t *task   |
 *  ----------------
 * |      ...       |
 *  ----------------
 * 
 * Are you asking why? A: To make iret happy ;)
 *
 * So then we gonna save all context of the current task in it's stack, and
 * then we will make a stack switch. In the new stack we gonna
 * pop all registers context and call iret to jump to the new thread.
 */
task_switch_to:
	cli
	pushl %eax            /* save EAX           */
	movl  4(%esp), %eax   /* get return address */
	movl %eax, -4(%esp)   /* "push" return address without change esp */
	addl  $8,%esp
	pushf                 /* save eflags                */
	xchgl %eax,-4(%esp) /* back EAX value to register */
	pushl %cs
	subl $4,%esp

	/* push fake error code and interrupt number */
#pushl $0
#	pushl $0

	/* Save registers */
	pushal
	pushw %ds
	pushw %es
	pushw %fs
	pushw %gs
	pushw %ss

	/* Save CR3 */
	pushl %eax
	movl %cr3, %eax
	xchgl %eax,(%esp)   /* exchange CR3 with EAX value in the stack */

	/**
	 * Save stack
	 */
	movl arch_tss_cur_task,%eax
	movl %esp, 10(%eax)

	/**
	 * Make stack switch
	 * keep offset values sync with struct _task_struct
	 * \see include/tempos/sched.h
	 */
	movl 58(%esp), %eax
	movl 10(%eax), %esp

	/* Invalidate TLB cache and load page table directory */
	popl %eax
	movl %eax, %cr3

	popl %ebx
lp:
	jmp lp

	/* Restore register values from new task */
	popw %ss
	popw %gs
	popw %fs
	popw %es
	popw %ds
	popal

	/* Skip interrupt number and error code */
#addl $8, %esp

	/* Call iret and go to new thread */
	sti
	iret

