/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: i82C54.h
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

#ifndef  ARCH_X86_PIT

	#define ARCH_X86_PIT

	#include <unistd.h>
	#include <tempos/timer.h>

	/* i82C54 Frequency */
	#define PIT82C54_HZ		1193182
	#define SYSTEM_TIMER	(PIT82C54_HZ / HZ)

	/* I/O ports used by PIT */
	#define CH0_PORT	0x40
	#define CH2_PORT	0x42
	#define CM_PORT		0x43

	/* Channels mask */
	#define CH0_MASK	0x00
	#define CH2_MASK	0x80

	/* Access mode */
	#define AM_LOW		0x10
	#define AM_HIGH		0x20
	#define AM_LOW_HIGH	0x30

	/* Operation modes */
	#define MODE_0		0x00
	#define MODE_1		0x02
	#define MODE_2		0x04
	#define MODE_3		0x06
	#define MODE_4		0x08

	/* Data format */
	#define DATA_BIN	0x00
	#define DATA_BCD	0x01


	/* A simple wait for io operations */
	#define pit_delay()	asm volatile("nop; nop; nop");


	void init_PIT(void);

#endif /* ARCH_X86_PIT */

