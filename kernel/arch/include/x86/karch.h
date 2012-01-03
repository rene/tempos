/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: karch.h
 *
 * This file is part of TempOS.
 *
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

#ifndef ARCH_X86_KARCH_H

	#define ARCH_X86_KARCH_H

	/** Kernel stack size */
	#define STACK_SIZE			0x4000 /* 16Kb */

	#define KERNEL_ADDR_OFFSET  0xBFF00000 /* 3GB-1MB */
	#define PHYADDR(x)			((x) - KERNEL_ADDR_OFFSET)
	#define VIRADDR(x)			((x) + KERNEL_ADDR_OFFSET)

#ifndef ASM

	#include <unistd.h>

	extern uint32_t _KERNEL_PA_START;
	extern uint32_t _KERNEL_START;
	extern uint32_t _KERNEL_END;

	/** 
	 * Kernel memory addresses.
	 * See arch/x86/boot/setup.ld
	 */
	#define KERNEL_PA_START		 (void*)&_KERNEL_PA_START
	#define KERNEL_START_ADDR	 (void*)&_KERNEL_START
	#define KERNEL_END_ADDR		 (void*)&_KERNEL_END

#endif

#endif /* ARCH_X86_KARCH_H */

