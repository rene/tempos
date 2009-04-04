/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: x86.h
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

#ifndef ARCH_X86_GEN

	#define ARCH_X86_GEN

	#include <unistd.h>

	#define KERNEL_DPL 0x00
	#define USER_DPL   0x03

	#define KERNEL_CS	0x08	/* Position 1 on GDT */
	#define KERNEL_DS	0x10	/* Position 2 on GDT */


	#define cli() asm volatile("cli")
	#define sti() asm volatile("sti")
	//inline void cli();
	//inline void sti();

#endif /* ARCH_X86_GEN */

