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

.global jmp_to_task


jmp_to_task:
	cli
	/* Context level switching */
	movw 36(%esp), %ax # regs.ds
	movw %ax, %ds
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movw %ax, %es
	/* Read task_t values */
	xorl %eax, %eax
	movl 38(%esp), %ebp # regs.esp
	movw 62(%esp), %cx  # regs.cs
	movl 64(%esp), %ebx # regs.eflags
	movl 58(%esp), %edx # regs.eip
	movl 68(%esp), %eax # regs.cr3 
	/* Invalidate TLB's caches and set page table directory */
	movl %eax, %cr3
	/* Stack switching */
	movl %ebp, %esp
	xorl %ebp, %ebp
	/* Now, finish context switching calling iret */
	pushl %ebx
	pushw %cx
	pushl %edx
	sti
	iret

