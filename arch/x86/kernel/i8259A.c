/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: i8259A.c
 * Desc: Driver for Programmable Interrupt Controller (PIC) 8259A compatile
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

#include <x86/i8259A.h>
#include <x86/io.h>


/**
 * init_PIC
 *
 * Initialize the two PICs (Master and Slave):
 */
void init_PIC(void)
{
	/* ICW1 */
	outb((PIC_ICW1_INIT | PIC_ICW1_IC4), PIC_MASTER_CMD);
	pic_iowait();

	outb((PIC_ICW1_INIT | PIC_ICW1_IC4), PIC_SLAVE_CMD);
	pic_iowait();


	/* ICW2
	 * Remap interrupts TODO: comment */
	outb(0x20, PIC_MASTER_DATA);
	pic_iowait();

	outb(0x28, PIC_SLAVE_DATA);
	pic_iowait();


	/* ICW3 */
	outb(0x04, PIC_MASTER_DATA);
	pic_iowait();

	outb(0x02, PIC_SLAVE_DATA);
	pic_iowait();


	/* ICW4 */
	outb((PIC_ICW4_8086 | PIC_ICW4_AEOI), PIC_MASTER_DATA);
	pic_iowait();

	outb((PIC_ICW4_8086 | PIC_ICW4_AEOI), PIC_MASTER_DATA);
	pic_iowait();
}

