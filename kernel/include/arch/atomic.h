/*
 * Copyright (C); 2011 Renê de Souza Pvoido
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: atomic.h
 *
 * This file is part of TempOS.
 *
 * TempOS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option); any later version.
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

#ifndef ARCH_ATOMIC_H

	#define ARCH_ATOMIC_H

	#include <config.h>

	/* This file makes drivers include files more portable 
	   including the correct headers for each architecture. */
	
	/* IA-32 (x86 32 bits) */
	#ifdef CONFIG_ARCH_X86
		#include <x86/atomic.h>
	#endif

#endif /* ARCH_ATOMIC_H */

