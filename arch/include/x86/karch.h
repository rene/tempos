/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: karch.h
 * Desc: Kernel definitions for x86 architecture
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

#ifndef ARCH_X86_KARCH_H

	#define ARCH_X86_KARCH_H

	/* Stak size = 16Kb */
	#define STACK_SIZE	0x4000

#ifndef ASM

	#include <unistd.h>

	/* Kernel memory address
	   See arch/x86/boot/setup.ld */
	#define KERNEL_START_ADDR	&_KERNEL_START
	#define KERNEL_END_ADDR		&_KERNEL_END

	extern uint32_t _KERNEL_START;
	extern uint32_t _KERNEL_END;

#endif

#endif /* ARCH_X86_KARCH_H */

