/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: tss.h
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

#ifndef ARCH_X86_TSS_H

	#define ARCH_X86_TSS_H

	#include <x86/x86.h>
	#include <unistd.h>

	/**
	 * This is the Task-State Segment (TSS).
	 *
	 * \note This structure is used when hardware context switch is used.
	 * However, TempOS does context switches by software, so we will NOT use
	 * this structure. The only reason to declare it it's to make a TSS entry at
	 * GDT (Global Descriptor Table).
	 *
	 * \see arch/x86/gdt.c
	 */
	struct _tss_struct {
		uint16_t prev_task_link;
		uint16_t reserved0;
		uint32_t esp0;
		uint16_t ss0;
		uint16_t reserved1;
		uint32_t esp1;
		uint16_t ss1;
		uint16_t reserved2;
		uint32_t esp2;
		uint16_t ss2;
		uint16_t reserved3;
		uint32_t cr3;
		uint32_t eip;
		uint32_t eflags;
		uint32_t eax;
		uint32_t ecx;
		uint32_t edx;
		uint32_t ebx;
		uint32_t esp;
		uint32_t ebp;
		uint32_t esi;
		uint32_t edi;
		uint16_t es;
		uint16_t reserved4;
		uint16_t cs;
		uint16_t reserved5;
		uint16_t ss;
		uint16_t reserved6;
		uint16_t ds;
		uint16_t reserved7;
		uint16_t fs;
		uint16_t reserved8;
		uint16_t gs;
		uint16_t reserved9;
		uint16_t LDT_seg;
		uint16_t reserved10;
		uint16_t debug : 1;
		uint16_t reserved11 : 15;
		uint16_t iomap;
	} __attribute__((packed));

	/**
	 * This is the structure that contains all "registers context" of a process.
	 *
	 * TempOS scheduler will uses this structure to store/reload process
	 * architecture specific context.
	 *
	 * \see arch/x86/isr.S
	 */
	struct _tss_st {
		/** Context registers */
		pt_regs regs;
		/** Page table directory */
		uint32_t cr3;
	} __attribute__((packed));

	typedef struct _tss_struct tss_t;

	typedef struct _tss_st arch_tss_t;

#endif /* ARCH_X86_TSS_H */

