/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: idt.c
 * Desc: Functions to setup IDT table 
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

#include <x86/idt.h>

#include <tempos/kernel.h> /* temporary */

void nullint(void)
{
	kprintf("\nUma excessao ocorreu!\n");
	for(;;);
}



/**
 * setup_IDT
 *
 * TODO: comment here...
 *
 */
void setup_IDT(void)
{
	idt_t           idt_table[IDT_TABLE_SIZE];
	idt_tpintdesc_t *idtentry;
	uint16_t pos;

	cli();

	/* Setup IDT with exceptions handlers */
	for(pos=0; pos<=14; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, (uint32_t)&nullint);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 1;
	}

	/* Unused - Reserved */
	for(pos=15; pos<=31; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, 0x000000);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 0;
	}


	/* Setup IDT non used entries with
	   NULL Interrupt-gate descriptor *
	for(pos=FIRST_NONUSED_INT; pos<IDT_TABLE_SIZE; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, 0x000000);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 0;
	}*/

	IDTR.table_limit = (IDT_TABLE_SIZE * sizeof(idt_t)) - 1;
	IDTR.idt_ptr     = (uint32_t)&(idt_table);
	load_idt();
}


inline void load_idt(void)
{
	asm("lidtl %0" : : "m" (IDTR));
}

