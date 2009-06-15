/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: timer.c
 * Desc: Time system of TempOS
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

#include <tempos/timer.h>
#include <unistd.h>
#include <x86/irq.h>


static void timer_handler(int id, pt_regs *regs);

/**
 * jiffies
 *
 * Contains the number of system clock ticks
 */
volatile ulong32_t jiffies;


/**
 * init_timer
 *
 * Initialize time system
 */
void init_timer(void)
{
	kprintf(KERN_INFO "Initializing timer...\n");

	if( request_irq(TIMER_IRQ, timer_handler, 0, "PIT") < 0 ) {
		kprintf(KERN_ERROR "Error on initialize PIT\n");
	}

	jiffies = 0;
}


/**
 * timer_handler
 *
 * Interrupt handler
 */
static void timer_handler(int id, pt_regs *regs)
{
	jiffies++;
}

