/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: unistd.h
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

#ifndef UNISTD_H

	#define UNISTD_H

	#include <config.h>

	#ifdef CONFIG_ARCH_X86
	typedef char           		char8_t;
	typedef unsigned char  		uchar8_t;
	typedef short          		int16_t;
	typedef unsigned short 		uint16_t;
	typedef int            		int32_t;
	typedef unsigned int   		uint32_t;
	typedef long long      		int64_t;
	typedef unsigned long long 	uint64_t;

	typedef unsigned long size_t;
	typedef long ssize_t;
	#endif

#endif /* UNISTD_H */

