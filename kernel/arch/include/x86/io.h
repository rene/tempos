/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: io.h
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

#ifndef ARCH_X86_IO_H

	#define ARCH_X86_IO_H

	#include <unistd.h>

	#define CR0_PG_MASK		0x80000000


	extern uchar8_t inb(uint16_t port);

	extern void outb(uchar8_t value, uint16_t port);

	extern uint16_t inw(uint16_t port);

	extern void outw(uint16_t value, uint16_t port);

	extern uint32_t inl(uint16_t port);

	extern void outl(uint32_t value, uint16_t port);

	extern void cli(void);

	extern void sti(void);

	extern uint32_t read_cr0();

	extern void write_cr0(uint32_t value);

	extern void write_cr3(uint32_t value);

#endif /* ARCH_X86_IO_H */

