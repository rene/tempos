/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: setup.ld
 * Desc: Script to define the layout of executable file
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

/* The first function of executable file
 * start - defined in boot.S */
ENTRY(start)

/**
 * Executable file sections
 */
SECTIONS {

	/**
	 * Bootloader will load the kernel at 1MB of memory, so
	 * this is the physical address of the Kernel.
	 */
	. = 0x00100000;
	_KERNEL_PA_START = . ;

	/**
	 * With paging system, Kernel will be reallocated to (3GB+1MB)
	 * of virtual address space.
	 */
	. = 0xC0100000;
	_KERNEL_START = . ; /* (3GB+1MB) */

	.text _KERNEL_START : AT(ADDR(.text) - 0xC0000000) {
		*(.text)
	}

	.rodata ALIGN(0x1000) : AT(ADDR(.rodata) - 0xC0000000) {
		*(.rodata)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data) - 0xC0000000) {
		*(.data)
	}

	.bss : AT(ADDR(.bss) - 0xC0000000) {
		*(.bss)
		*(COMMON) /* This puts all uninitialized data here */
	}

	/* End of the kernel
	   PS: We need 4096 aligned because of the paging system */
	. = ALIGN(0x1000);
	_KERNEL_END = . ;
}

