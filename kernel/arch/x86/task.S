/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
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

#include <x86/x86.h>

.global initial_task2, initial_task, task_switch_to
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
	 * initialization, thus at the kernel mode, we doesn't
	 * need to make level switch because we are already in kernel mode,
	 * so here all we need it's adjust stack and jmp to main thread function
	 *
	 * \note keep offset values sync with struct _task_struct
	 * \see include/tempos/sched.h
	 */
	movl 10(%eax), %ebx /* arch_tss.regs.esp    */
	movl 48(%eax), %ecx /* arch_tss.regs.eflags */
	movl 42(%eax), %edx /* arch_tss.regs.eip    */

	/* Ajust stack */
	movl %ebx, %esp

	/* Push arguments for iret and call it */
	pushl %ecx
	pushl %cs
	pushl %edx
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
 * |      ESP       |
 *  ----------------
 * |      SS        |
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

	pushl %eax
	movl arch_tss_cur_task, %eax
	movl 10(%eax), %eax

	pushl %ebx
	movl 12(%esp), %eax

	/* Check if task it's kernel or user process */
	movw (%eax), %bx
	cmpw  $USER_DS_RPL, %bx
	popl %ebx
	popl %eax

	jne kernel_proc
	pushl $USER_DS_RPL /* SS  */
	pushl %esp         /* ESP */
	pushl $USER_CS_RPL /* CS  */
	jmp cs_cont
kernel_proc:
	pushl $KERNEL_DS   /* SS  */
	pushl %esp         /* ESP */
	pushl $KERNEL_CS   /* CS  */
cs_cont:

	pushl %eax 		/* save EAX in the stack */
	
	/* Move return address to top of stack */
	subl $0x04, %esp
	movl 20(%esp), %eax
	movl %eax, (%esp)
	
	/* Move SS down */
	movw 16(%esp), %ax
	movw %ax, 20(%esp)

	/* Move ESP down */
	movl 12(%esp), %eax
	movl %eax, 16(%esp)

	/* put eflags in EAX */
	pushf
	popl  %eax

	/* Move eflags above ESP */
	movl %eax, 12(%esp)

	/* exchange return value with EAX onto the stack */
	popl %eax /* return value */
	xchgl %eax, (%esp)

	/* Save general registers */
	pushal
	/* Get correct data segment RPL */
	movl 52(%esp), %eax
	/* Push data segment "registers" */
	pushw (%eax) /* SS, ES, FS, GS, SS */

	/* Save CR3 */
	pushl %eax
	movl %cr3, %eax
	xchgl %eax,(%esp)   /* exchange CR3 with EAX value in the stack */

	/**
	 * Save stack
	 */
	movl arch_tss_cur_task, %eax
	movl %esp, 10(%eax)
#	movl %esp, 50(%esp)

	/**
	 * Check if iret will pop SS and ESP (intra-privilege)
	 */
	movw  8(%eax), %bx   /* SS of current task */
	movl 58(%esp), %eax
	movw  8(%eax), %cx   /* SS of this task */
	cmpw %bx, %cx
	
	/**
	 * Make stack switch
	 */
	movl 58(%esp), %eax
	movl 10(%eax), %esp
	
	movl %eax, arch_tss_cur_task
	
	/* Invalidate TLB cache and load page table directory */
	popl %eax
	movl %eax, %cr3

	/* Restore register values for new task */
	popw %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	popal

	/* Check for intra-privilege */
	jne intra
	pushl %eax
	movl 12(%esp), %eax  /* EFLAGS */
	xchgl %eax, 20(%esp)
	movl  8(%esp), %eax  /* CS */
	xchgl %eax, 16(%esp)
	movl 4(%esp), %eax   /* Ret addr */
	xchgl %eax, 12(%esp)
	popl %eax
	addl $0x08, %esp
intra:
	/* Call iret and go to new thread */
	iret

