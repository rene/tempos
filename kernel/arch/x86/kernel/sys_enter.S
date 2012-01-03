/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: sys_enter.S
 * Desc: Contains the low level code to execute an system call
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
#include <tempos/syscall.h>
#include <tempos/error.h>

.globl _sys_enter;
.extern syscall_table;

_sys_enter:
	pushl %esp
	pushl %ebp
	pushw %ds
	pushw %es
	pushw %fs
	pushw %gs
	pushl %edx /* Save EDX argument */

	/* Move to kernel mode */
	movw $KERNEL_DS, %dx
	movw %dx, %ds
	movw %dx, %fs
	movw %dx, %gs
	movw %dx, %ss
	movw %dx, %es
	
	popl %edx

	pushl $exit_syscall

	cmpl $SYSCALL_COUNT, %eax
	jae  exit_syscall

	/* Push syscall arguments */
	pushl %ebx
	pushl %ecx
	pushl %edx

	/* Now the proccess is ready to make the syscall, so
	   we can enable interrupts again. */
	sti
	call *syscall_table(,%eax,4)
	addl $4*3,%esp
	ret

exit_syscall:
	/*pushl %eax*/
	cli
	popw  %gs
	popw  %fs
	popw  %es
	popw  %ds
	popl  %ebp
	popl  %esp
	sti
	iret

