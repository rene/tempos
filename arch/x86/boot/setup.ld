/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: setup.ld
 * Desc: Script to define the layout of executable file (passed to the linker)
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
	 * text section need to start at 1MB
	 */
	.text 0x00100000 : {
		*(.text)
	}

	.rodata ALIGN(0x1000) : {
		*(.rodata)
	}

	.data ALIGN(0x1000) : {
		*(.data)
	}

	.bss : {
		*(.bss)
		*(COMMON) /* This puts all uninitialized data here */
	}
}

