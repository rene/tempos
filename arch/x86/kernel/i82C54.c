/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: i82C54.c
 * Desc: Driver for Programmable Interval Timer (PIT) 82C54 compatile
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

#include <x86/i82C54.h>
#include <x86/io.h>


/**
 * init_PIT
 *
 * Init the PIT controller
 */
void init_PIT(void)
{
	outb((CH0_MASK | AM_LOW_HIGH | MODE_2 | DATA_BIN), CM_PORT);
	pit_delay();
	outb((uchar8_t)(SYSTEM_TIMER & 0xFF), CH0_PORT);
	pit_delay();
	outb((uchar8_t)((SYSTEM_TIMER >> 0x08) & 0xFF), CH0_PORT);
	pit_delay();
}

