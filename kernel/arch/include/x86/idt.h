/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: idt.h
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

#ifndef ARCH_X86_IDT_H

	#define ARCH_X86_IDT_H

	#include <x86/x86.h>
	#include <x86/io.h>


	#define IDT_TABLE_SIZE		256

	#define IDT_SET_OFFSET(a, offset)	a->offset_low       = (((uint32_t)offset) & 0x0000FFFF);          \
										a->high.offset_high = ((((uint32_t)offset) >> 16) & 0x0000FFFF);


	#define IDT_INT_GATE		0x6
	#define IDT_TRAP_GATE		0x7
	#define IDT_INTGATE_S16		0
	#define IDT_INTGATE_S32		1

	/**
	 * idt_entry
	\verbatim
	  Each entry of IDT stores a Gate Descriptor. A gate descriptor in IDT
	  should describe any of follow gates below:
	 
	  		TSS_DESC  - Task-gate descriptor
	  		INT_DESC  - Interrupt-gate descriptor
	  		TRAP_DESC - Trap-gate descriptor
	  		  ^              ^
	         |              |
	         |              |
	         |              [---- Intel terminology
	         |
	         [---- TempOS terminology
	\endverbatim 
	 */
	struct _idt_entry {
		uint32_t lower;
		uint32_t high;
	} __attribute__ ((packed));


	/**
	 * idt_tpint_desc
	 *
	 * The IDT table entry format for Interrupt-gate and Trap-gate descriptors. Both are
	 * very similar, so we can use just one structure to keep them.
	 * This is not the better way to express a IDT entry, but it's clear and more easy 
	 * to understand.
	 *
	 * For more information, see Intel Manual vol.3, chapter 5.
	 */
	struct _idt_tpint_desc {
		uint16_t  offset_low;
		uint16_t  seg_selector;
		struct _idt_high {
			uint16_t notused     : 5;
			uint16_t reserved3   : 3;
			uint16_t type        : 3;
			uint16_t gate_size   : 1;
			uint16_t reserved1   : 1;
			uint16_t DPL         : 2;
			uint16_t present     : 1;
			uint16_t offset_high;
		} __attribute__ ((packed)) high;
	} __attribute__ ((packed));

	/**
	 * idtr_t
	 *
	 * IDTR store IDT table size (limit) and the pointer to the table
	 */
	struct _idt_idtr {
		uint16_t table_limit;
		void* idt_ptr;
	} __attribute__ ((packed));

	typedef struct _idt_entry      idt_t;
	typedef struct _idt_tpint_desc idt_tpintdesc_t;
	typedef struct _idt_idtr       idtr_t;

	extern idtr_t IDTR;

	void setup_IDT(void);

	extern void load_idt(void);

#endif /* ARCH_X86_IDT_H */

