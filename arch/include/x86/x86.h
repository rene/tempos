/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: x86.h
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

#ifndef ARCH_X86_H

	#define ARCH_X86_H

#ifndef ASM
	#include <unistd.h>

	#define _pushargs __attribute__((regparm(0)))

	/**
	 * Registers saved at interrupt trapping.
	 *
	 * \note Do not change the order of elements in the structure!
	 * \see arch/x86/isr.S
	 */
	struct _pt_regs {
		/** Stack Segment */
		uint16_t ss;
		/** Data segment */
		uint16_t gs;
		/** Data segment */
		uint16_t fs;
		/** Data segment */
		uint16_t es;
		/** Data segment */
		uint16_t ds;
		/** General-Purpose register : stack pointer */
		uint32_t esp;
		/** General-Purpose register */
		uint32_t ebx;
		/** General-Purpose register */
		uint32_t edx;
		/** General-Purpose register */
		uint32_t ecx;
		/** General-Purpose register */
		uint32_t eax;
		/** Instruction pointer */
		uint32_t eip;
		/** Code segment register */
		uint16_t cs;
		/** EFLAGS */
		uint32_t eflags;
	} __attribute__((packed));

	typedef struct _pt_regs pt_regs;

#endif

	#define KERNEL_DPL 0x00
	#define USER_DPL   0x03

	/** Position 1 on GDT */
	#define KERNEL_CS	0x08
	/** Position 2 on GDT */
	#define KERNEL_DS	0x10
	/** Position 3 on GDT */
	#define USER_CS		0x18
	/** Position 4 on GDT */
	#define USER_DS		0x20
	/** Position 5 on GDT */
	#define TSS_SEG     0x28

	/** Interrupt flags in EFLAGS */
	#define EFLAGS_IF   0x2020000

#endif /* ARCH_X86_H */

