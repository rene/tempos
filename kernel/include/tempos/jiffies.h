/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: jiffies.h
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

#ifndef JIFFIES_H

	#define JIFFIES_H

	#include <unistd.h>


	/* These inlines come from Linux and deal with timer wrapping correctly.
	   You are strongly encouraged to use them
	 
	   time_after(a,b) returns true if the time a is after time b.
	 
	   Do this with "<0" and ">=0" to only test the sign of the result. A
	   good compiler would generate better code (and a really good compiler
	   wouldn't care). Gcc is currently neither. */
	#define time_after(a,b)     \
				((int32_t)(b) - (int32_t)(a) < 0)
	#define time_before(a,b)    time_after(b,a)
	#define time_after_eq(a,b)  \
				((int32_t)(a) - (int32_t)(b) >= 0)
	#define time_before_eq(a,b) time_after_eq(b,a)


	extern volatile uint32_t jiffies;

#endif /* JIFFIES_H */

