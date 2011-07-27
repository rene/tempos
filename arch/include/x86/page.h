/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: page.h
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

#ifndef ARCH_X86_PAGE_H

	#define ARCH_X86_PAGE_H

	#include <x86/karch.h>


	#define PAGE_SHIFT			12 /* 4Kb */
	#define PAGE_SIZE       	(1UL << PAGE_SHIFT)
	#define PAGE_MASK		 	(~(PAGE_SIZE - 1))
	#define PAGE_ALIGN(addr)    (((addr) + PAGE_SIZE - 1) & PAGE_MASK)
	#define PAGE_PADDR(p)		(p & 0xFFFFF000)

	#define TABLE_SHIFT			10 /* 1024 */
	#define TABLE_SIZE			(1UL << TABLE_SHIFT)
	#define TABLE_MASK          (~(TABLE_SIZE - 1))
	#define TABLE_ALIGN(addr)   (((addr) + TABLE_SIZE -1) & TABLE_MASK)
	#define TABLE_PADDR(t)		(t & 0xFFFFF000)

	#define TABLE_ENTRY_SIZE	sizeof(uint32_t)

	#define MAKE_ENTRY(addr, params)	(((addr) & 0xFFFFF000) | params)

	#define PAGE_PRESENT		0x01
	#define PAGE_WRITABLE		0x02
	#define PAGE_USER			0x04

#endif /* ARCH_X86_PAGE_H */

