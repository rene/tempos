/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: exceptions.c
 * Desc: Functions to handle processor exceptions
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

#include <tempos/kernel.h>
#include <x86/exceptions.h>


void ex_div()
{
	kprintf("Divisao por 0 detectada!\n");
	while(1);
}


void ex_debug()
{
}


void ex_nmi()
{
}


void ex_break()
{
}


void ex_overflow()
{
}


void ex_bounds()
{
}


void ex_inv_op()
{
}


void ex_device()
{
}


void ex_dfault()
{
}


void ex_coproc()
{
}


void ex_inv_tss()
{
}


void ex_segnp()
{
}


void ex_stack()
{
}


void ex_gp()
{
}


/* Page fault - This fault is handle by mm */
void ex_pfault()
{
}


void ex_floatp()
{
}


void ex_align()
{
}


void ex_machine()
{
}


void ex_simd()
{
}


