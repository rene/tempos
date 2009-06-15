/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
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
#include <x86/exceptions.h>
#include <x86/irq.h>

/* syscall handler (see sys_enter.S)*/
extern void _sys_enter(void);


/* IDT table */
idt_t idt_table[IDT_TABLE_SIZE];


/**
 * setup_IDT
 *
 * Setup the IDT table, but not enable interrupts yet. Interrupts will be
 * enable after PIC initialization. See arch/x86/kernel/i8259A.c and 
 * arch/x86/boot/karch.c.
 *
 * As you should know, x86 architecture permits three types of gate
 * descriptors in IDT: Task-gate, Interrupt-gate and Trap-gate descriptors.
 * TempOS uses only Interrupt-gate descritors, that has this format:
 *
 *
 * 31                  16 15 14 13 12        8  7     5 4       0
 * |------------------------------------------------------------|
 * |  Offset 31..16      | P | DPL | 0 D 1 1 0 | 0 0 0 | UNUSED | 4
 * |---------------------|--------------------------------------|
 * |  Segment Selector   |           Offset 15..0               | 0
 * |------------------------------------------------------------|
 * 31                                                           0
 *
 * Offset is the address of the ISR (Interrupt Service Routine) and
 * Segment Selector is the point to descritor in GDT table, in our case,
 * KERNEL_CS.
 *
 * For complete understand, see Intel Manual vol.3, chapter 5.
 */
void setup_IDT(void)
{
	idt_tpintdesc_t *idtentry;
	uint16_t pos;

	cli();

	/* Setup IDT with exceptions handlers */
	for(pos=0; pos<=19; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, nullisr);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 1;
	}
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[0]),  _ex_div);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[1]),  _ex_debug);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[2]),  _ex_nmi);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[3]),  _ex_break);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[4]),  _ex_overflow);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[5]),  _ex_bounds);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[6]),  _ex_inv_op);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[7]),  _ex_device);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[8]),  _ex_dfault);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[9]),  _ex_coproc);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[10]), _ex_inv_tss);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[11]), _ex_segnp);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[12]), _ex_stack);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[13]), _ex_gp);

	/* Page fault - This fault is handle by mm */
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[14]), _ex_pfault);

	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[16]), _ex_floatp);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[17]), _ex_align);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[18]), _ex_machine);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[19]), _ex_simd);


	/* 15 - Intel Reserved */
	idtentry = (idt_tpintdesc_t *)&idt_table[15];
	IDT_SET_OFFSET(idtentry, 0x00000000);
	idtentry->high.notused   = 0x0;
	idtentry->high.reserved3 = 0x0;
	idtentry->high.reserved1 = 0;
	idtentry->high.present   = 0;

	/* 20-31 - Intel Reserved */
	for(pos=20; pos<=31; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, 0x00000000);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 0;
	}

	/* IRQs */
	for(pos=32; pos<=46; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, nullisr);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 1;
	}
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[32]),  isr_irq0);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[33]),  isr_irq1);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[34]),  isr_irq2);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[35]),  isr_irq3);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[36]),  isr_irq4);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[37]),  isr_irq5);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[38]),  isr_irq6);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[39]),  isr_irq7);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[40]),  isr_irq8);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[41]),  isr_irq9);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[42]),  isr_irq10);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[43]),  isr_irq11);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[44]),  isr_irq12);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[45]),  isr_irq13);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[46]),  isr_irq14);
	IDT_SET_OFFSET(((idt_tpintdesc_t *)&idt_table[47]),  isr_irq15);


	/* User defined */
	for(pos=48; pos<IDT_TABLE_SIZE; pos++) {
		idtentry = (idt_tpintdesc_t *)&idt_table[pos];
		IDT_SET_OFFSET(idtentry, nullisr);
		idtentry->seg_selector   = KERNEL_CS;
		idtentry->high.notused   = 0x0;
		idtentry->high.reserved3 = 0x0;
		idtentry->high.type      = IDT_INT_GATE;
		idtentry->high.gate_size = IDT_INTGATE_S32;
		idtentry->high.reserved1 = 0;
		idtentry->high.DPL       = KERNEL_DPL;
		idtentry->high.present   = 0;
	}

	/* TempOS syscall interrupt : 0x85 */
	idtentry = (idt_tpintdesc_t *)&idt_table[133];
	IDT_SET_OFFSET(idtentry, _sys_enter);
	idtentry->high.present = 1;


	IDTR.table_limit = (IDT_TABLE_SIZE * sizeof(idt_t)) - 1;
	IDTR.idt_ptr     = (uint32_t)idt_table;
	load_idt();
}


inline void load_idt(void)
{
	asm("lidtl %0" : : "m" (IDTR));
}


