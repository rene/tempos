/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: isr.S
 * Desc: Contains the low level code to call interrupt service routines
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

.extern ex_div, ex_debug, ex_nmi, ex_break, ex_overflow, ex_bounds, \
		ex_inv_op, ex_device, ex_dfault, ex_coproc, ex_inv_tss,     \
		ex_segnp, ex_stack, ex_gp, ex_pfault, ex_floatp, ex_align,  \
		ex_machine, ex_simd
.extern do_irq


.globl  _ex_div, _ex_debug, _ex_nmi, _ex_break, _ex_overflow, _ex_bounds, \
		_ex_inv_op, _ex_device, _ex_dfault, _ex_coproc, _ex_inv_tss,      \
		_ex_segnp, _ex_stack, _ex_gp, _ex_pfault, _ex_floatp, _ex_align,  \
		_ex_machine, _ex_simd
.globl  isr_irq0, isr_irq1, isr_irq2, isr_irq3, isr_irq4, isr_irq5,   \
		isr_irq6, isr_irq7, isr_irq8, isr_irq9, isr_irq10, isr_irq11, \
		isr_irq12, isr_irq13, isr_irq14, isr_irq15, nullisr


no_error_code:

	/**
	 * Save machine state
	 */

	/* Exchange EAX value with the address function pushed onto the stack */
	xchgl %eax,(%esp)
	pushl %ebx
	pushl %ecx
	pushl %edx
	pushl %edi
	pushl %esi
	pushl %ebp
	pushw %ds
	pushw %es
	pushw %fs
	pushl  $0x00 /* Error code */

	/* Move to kernel mode */

	movw $KERNEL_DS, %dx
	movw %dx, %ds
	movw %dx, %fs
	movw %dx, %gs
	movw %dx, %ss
	movw %dx, %es

	/* Execute the ISR */
	call *%eax

	/**
	 * Restore machine state
	 */
	popw %fs
	popw %es
	popw %ds
	popl %ebp
	popl %esi
	popl %edi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret


error_code:
	/**
	 * Save machine state
	 */

	/* Exchange EAX value with the error code pushed onto the stack and also
	   with EBX value with the address function pushed too */
	xchgl %eax,4(%esp)
	xchgl %ebx,(%esp)
	pushl %ecx
	pushl %edx
	pushl %edi
	pushl %esi
	pushl %ebp
	pushw %ds
	pushw %es
	pushw %fs
	pushl %eax /* Error code */

	/* Move to kernel mode */

	movw $KERNEL_DS, %dx
	movw %dx, %ds
	movw %dx, %fs
	movw %dx, %gs
	movw %dx, %ss
	movw %dx, %es

	/* Execute the ISR */
	call *%ebx

	/**
	 * Restore machine state
	 */
	popl %eax /* error code */
	popw %fs
	popw %es
	popw %ds
	popl %ebp
	popl %esi
	popl %edi
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret


/**
 * Null ISR (do nothing)
 */
nullisr:
	iret


/**
 * Exceptions
 */
_ex_div:
	pushl $ex_div
	jmp no_error_code

_ex_debug:
	pushl $ex_debug
	jmp no_error_code

_ex_nmi:
	pushl $ex_nmi
	jmp no_error_code

_ex_break:
	pushl $ex_break
	jmp no_error_code

_ex_overflow:
	pushl $ex_overflow
	jmp no_error_code

_ex_bounds:
	pushl $ex_bounds
	jmp no_error_code

_ex_inv_op:
	pushl $ex_inv_op
	jmp no_error_code

_ex_device:
	pushl $ex_device
	jmp no_error_code

_ex_dfault:
	pushl $ex_dfault
	jmp error_code

_ex_coproc:
	pushl $ex_coproc
	jmp no_error_code

_ex_inv_tss:
	pushl $ex_inv_tss
	jmp error_code

_ex_segnp:
	pushl $ex_segnp
	jmp error_code

_ex_stack:
	pushl $ex_stack
	jmp error_code

_ex_gp:
	pushl $ex_gp
	jmp error_code

_ex_pfault:
	pushl $ex_pfault
	jmp error_code

_ex_floatp:
	pushl $ex_floatp
	jmp no_error_code

_ex_align:
	pushl $ex_align
	jmp error_code

_ex_machine:
	pushl $ex_machine
	jmp no_error_code

_ex_simd:
	pushl $ex_simd
	jmp no_error_code


/**
 * ISR for IRQ handler
 */
_do_irq:
	pushl $do_irq
	jmp error_code

isr_irq0:
	pushl $0x00
	jmp _do_irq

isr_irq1:
	pushl $0x01
	jmp _do_irq

isr_irq2:
	pushl $0x02
	jmp _do_irq

isr_irq3:
	pushl $0x03
	jmp _do_irq

isr_irq4:
	pushl $0x04
	jmp _do_irq

isr_irq5:
	pushl $0x05
	jmp _do_irq

isr_irq6:
	pushl $0x06
	jmp _do_irq

isr_irq7:
	pushl $0x07
	jmp _do_irq

isr_irq8:
	pushl $0x08
	jmp _do_irq

isr_irq9:
	pushl $0x09
	jmp _do_irq

isr_irq10:
	pushl $0x0A
	jmp _do_irq

isr_irq11:
	pushl $0x0B
	jmp _do_irq

isr_irq12:
	pushl $0x0C
	jmp _do_irq

isr_irq13:
	pushl $0x0D
	jmp _do_irq

isr_irq14:
	pushl $0x0E
	jmp _do_irq

isr_irq15:
	pushl $0x0F
	jmp _do_irq

