/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: irq.c
 * Desc: Architecture dependent functions to handle IRQs
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

#include <x86/irq.h>
#include <x86/io.h>
#include <tempos/kernel.h>

void do_irq(uint32_t irqnum, pt_regs regs)
{
	uchar8_t key;

	if(irqnum == 0)
		return;

	if(irqnum == 1) {
		key = inb(0x60);
		//outb(0x20, 0x20);
	}
	kprintf("IRQ %d chamada! -- AX = %ld\n", irqnum, regs.eax);
}


