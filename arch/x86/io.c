/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: io.c
 * Desc: I/O functions for x86
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <x86/io.h>


inline uchar8_t inb(uint16_t port)
{
	uchar8_t value;
	asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
	return(value);
}


inline void outb(uchar8_t value, uint16_t port)
{
	asm volatile("outb %0, %1" : : "a" (value), "dN" (port));
}


inline uint16_t inw(uint16_t port)
{
	uint16_t value;
	asm volatile("inw %1, %0" : "=a" (value) : "dN" (port));
	return(value);
}


inline void outw(uint16_t value, uint16_t port)
{
	asm volatile("outw %0, %1" : : "a" (value), "dN" (port));
}


inline uint32_t inl(uint16_t port)
{
	uint32_t value;
	asm volatile("inl %1, %0" : "=a" (value) : "dN" (port));
	return(value);
}


inline void outl(uint32_t value, uint16_t port)
{
	asm volatile("outl %0, %1" : : "a" (value), "dN" (port));
}


inline void cli(void)
{
	asm volatile("cli");
}


inline void sti(void)
{
	asm volatile("sti");
}


