/*
 * Copyright (C); 2009 Renê de Souza Pvoido
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: exceptions.h
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

#ifndef ARCH_X86_EXCEPTIONS_H

	#define ARCH_X86_EXCEPTIONS_H

	void ex_div(pt_regs regs);
	void ex_debug();
	void ex_nmi();
	void ex_break();
	void ex_overflow();
	void ex_bounds();
	void ex_inv_op();
	void ex_device();
	void ex_dfault(int code);
	void ex_coproc();
	void ex_inv_tss(int code);
	void ex_segnp(int code);
	void ex_stack(int code);
	void ex_gp(int code, pt_regs regs);
	void ex_pfault(int code, pt_regs regs);
	void ex_floatp();
	void ex_align(int code);
	void ex_machine();
	void ex_simd();

	/* These are implemented in assembly (see isr.S) */
	 extern void nullisr(void);
	 extern void _ex_div(void);
	 extern void _ex_debug(void);
	 extern void _ex_nmi(void);
	 extern void _ex_break(void);
	 extern void _ex_overflow(void);
	 extern void _ex_bounds(void);
	 extern void _ex_inv_op(void);
	 extern void _ex_device(void);
	 extern void _ex_dfault(void);
	 extern void _ex_coproc(void);
	 extern void _ex_inv_tss(void);
	 extern void _ex_segnp(void);
	 extern void _ex_stack(void);
	 extern void _ex_gp(void);
	 extern void _ex_pfault(void);
	 extern void _ex_floatp(void);
	 extern void _ex_align(void);
	 extern void _ex_machine(void);
	 extern void _ex_simd(void);

#endif /* ARCH_X86_EXCEPTIONS_H */

