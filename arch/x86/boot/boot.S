/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: boot.S
 * Desc: Boot Stage: Follow multiboot specification. Call karch to start
 *       the TempOS kernel.
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

#include <x86/multiboot.h>
#include <x86/karch.h>

#define BOOT_GDT_ENTRIES 3

/**
 * Text Section. Here we do not have the paging system yet
 */

	.text

	.globl start, _start
	.extern karch

	.align 4

multiboot_header:

	.long MULTIBOOT_HEADER_MAGIC
	.long MULTIBOOT_HEADER_FLAGS

	/* Checksum */
	.long -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

start:
_start:

	/**
     * Save contents of EAX into ECX
	 */
	 movl %eax, %ecx

	/**
	 * Bootloader leave us at 1MB of memory, but Kernel was
	 * linked at 3GB+1MB of virtual address, so we need to
	 * ajust the base at GDT table to translate the virtual
	 * address (3GB+1MB) to the physical address (1MB).
	 * After enabling paging system, we can reload GDT with
	 * base 0, because the address translation will be done
	 * by paging system.
	 */

	lgdt PHYADDR(boot_gdt_desc)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss
	movw %ax, %es

	ljmp $0x08, $higherhalf

higherhalf:
	/* Initialize the stack pointer */
	movl $(stack + STACK_SIZE), %esp

	/* Reset EFLAGS */
	pushl $0
	popf

	/* Push multiboot structure and magic value */
	pushl %ebx
	pushl %ecx

	/* Go to kernel, should never return from here  */
	call EXT_C(karch)

	hlt
error:
	jmp error


/**
 * Data section: GDT table only for boot
 */

.data

	.word 0							// 32 bit align boot_gdt_desc
boot_gdt_desc:
	.word BOOT_GDT_ENTRIES*8-1		// Size of GDT table
	.long gdt - KERNEL_PAGE_OFFSET	// Linear address of GDT

gdt:
	.long 0, 0				  		// Null descriptor
	.quad 0x40CF9A000000FFFF  		// 0x08 - Code selector: Base 0x40000000, Limit 0xFFFF
	.quad 0x40CF92000000FFFF  		// 0x10 - Data selector: Base 0x40000000, Limit 0xFFFF

/**
 * BSS Section, our stack goes here
 */
.bss
	/* Stack area */
	.comm stack, STACK_SIZE

