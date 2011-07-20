/**
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
	 * TODO: Document this struct.
	 */
	struct _tss_struct {
		uint16_t prev_task_link;
		uint16_t reserverd0;
		uint32_t esp0;
		uint16_t ss0;
		uint16_t reserverd1;
		uint32_t esp1;
		uint16_t ss1;
		uint16_t reserverd2;
		uint32_t esp2;
		uint16_t ss2;
		uint16_t reserverd3;
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
		uint16_t reserverd4;
		uint16_t cs;
		uint16_t reserverd5;
		uint16_t ss;
		uint16_t reserverd6;
		uint16_t ds;
		uint16_t reserverd7;
		uint16_t fs;
		uint16_t reserverd8;
		uint16_t gs;
		uint16_t reserverd9;
		uint16_t LDT_seg_selector;
		uint16_t reserverd10;
	} __attribute__ ((packed));

	typedef struct _tss_struct tss_t;

	extern tss_t task_tss;

#endif /* ARCH_X86_TSS_H */

