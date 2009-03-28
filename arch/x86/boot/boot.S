/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: boot.S
 * Desc: Boot Stage: Follow multiboot specification. Call karch to start
         the TempOS kernel.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define ASM 1

#include "multiboot.h"
#include "karch.h"

	.text

	.globl start, _start
	.extern karch

start:
_start:

	jmp multiboot_entry

	.align 4

multiboot_header:

	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS

	/* Checksum */
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

multiboot_entry:
	/* Initialize the stack pointer */
	movl $(stack + STACK_SIZE), %esp

	/* Reset EFLAGS */
	pushl $0
	popf

	/* Push multiboot structure and magic value */
	pushl %ebx
	pushl %eax

	/* Go to kernel, should never return from here  */
	call EXT_C(karch)

	hlt
error:
	jmp error

	/* Stack area */
	.comm stack, STACK_SIZE

