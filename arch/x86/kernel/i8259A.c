/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
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
	/* Mask all interrupts */
	outb(PIC_MASTER_MASK, PIC_MASTER_DATA);
	outb(PIC_SLAVE_MASK,  PIC_SLAVE_DATA);


	/* ICW1 */
	outb((PIC_ICW1_INIT | PIC_ICW1_IC4), PIC_MASTER_CMD);
	pic_delay();

	outb((PIC_ICW1_INIT | PIC_ICW1_IC4), PIC_SLAVE_CMD);
	pic_delay();


	/* ICW2
	   Remap interrupts:
	   	On master:
	   		IRQ0 to 0x20
			IRQ1 to 0x21
			...
		On slave:
			IRQ8 to 0x28
			IRQ9 to 0x29
			...
	 */
	outb(IRQ0_VECTOR, PIC_MASTER_DATA);
	pic_delay();

	outb(IRQ8_VECTOR, PIC_SLAVE_DATA);
	pic_delay();


	/* ICW3 */
	outb(PIC_ICW3_M_CASCADE, PIC_MASTER_DATA);
	pic_delay();

	outb(PIC_ICW3_S_CASCADE, PIC_SLAVE_DATA);
	pic_delay();


	/* ICW4 */
	outb((PIC_ICW4_8086 | PIC_ICW4_AEOI), PIC_MASTER_DATA);
	pic_delay();

	outb((PIC_ICW4_8086 | PIC_ICW4_AEOI), PIC_MASTER_DATA);
	pic_delay();
}


/**
 * get_picmask
 *
 * Return the interrupt mask
 *
 * Parameters:
 * 		pic - PIC_MASTER or PIC_SLAVE
 *
 * Return:
 * 		uchar8_t - Mask or 0 on error
 */
uchar8_t get_picmask(uchar8_t pic)
{
	uchar8_t mask;
	if(pic != PIC_MASTER_DATA && pic != PIC_SLAVE_DATA) {
		return(0);
	} else {
		mask = inb(pic);
		pic_delay();
		return(mask);
	}
}


/**
 * set_picmask
 *
 * Set the interrupt mask
 *
 * Parameters:
 * 		mask - Mask
 * 		pic  - PIC_MASTER or PIC_SLAVE
 */
void set_picmask(uchar8_t mask, uchar8_t pic)
{
	if(pic == PIC_MASTER_DATA || pic == PIC_SLAVE_DATA) {
		outb(mask, pic);
		pic_delay();
	}
}

