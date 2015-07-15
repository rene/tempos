/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: atomic.c
 * Desc: Implements basic atomic operations.
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
#include <x86/atomic.h>

inline void atomic_incl(int32_t *i)
{
	asm("incl (%0)" : "=r"(i));
}

inline void atomic_decl(int32_t *i)
{
	asm("decl (%0)" : "=r"(i));
}

void atomic_spin_down(uint32_t *i)
{
	asm("_spin_loop:         \n"
		"   cli              \n"
		"   movl (%1), %%ecx \n"
		"   sti              \n"
		"	jecxz _spin_loop \n"
		"decl (%0)" : "=r"(i) : "0"(i) : "ecx");
}

