/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: dump_cpu.c
 * Desc: Architecture dependent functions: dump_cpu() and halt_cpu()
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
#include <x86/x86.h>

/**
 * Dump CPU information with current register values.
 */
void dump_cpu(void)
{
	pt_regs regs;

	asm("movl %%eax, %0  \n" \
		"movl %%ebx, %1  \n" \
		"movl %%ecx, %2  \n" \
		"movl %%edx, %3":  \
		"=r" (regs.eax), "=r" (regs.ebx), "=r" (regs.ecx), "=r" (regs.edx) ::);

	asm("movw %%cs, %0  \n" \
		"movw %%ds, %1  \n" \
		"movw %%es, %2  \n" \
		"movw %%fs, %3":
		"=r" (regs.cs), "=r" (regs.ds), "=r" (regs.es), "=r" (regs.fs) ::);

	asm("movw %%gs,  %0  \n" \
		"movw %%ss,  %1  \n" \
		"movl %%esi, %2  \n" \
		"movl %%edi, %3  \n": \
		"=r" (regs.gs), "=r" (regs.ss), "=r" (regs.esi), "=r" (regs.edi) ::);

	dump_cpu_regs(&regs);
}

/**
 * Dump CPU passing registers values.
 * \param regs Value of each register.
 */
void dump_cpu_regs(pt_regs *regs)
{
	kprintf("\nDEBUG INFORMATION:\n");
	kprintf("EAX: %.9x    CS: %.5x\n", regs->eax, regs->cs);
	kprintf("EBX: %.9x    DS: %.5x\n", regs->ebx, regs->ds);
	kprintf("ECX: %.9x    ES: %.5x\n", regs->ecx, regs->es);
	kprintf("EDX: %.9x    FS: %.5x\n", regs->edx, regs->fs);
	kprintf("ESP: %.9x    GS: %.5x\n", regs->esp, regs->gs);
	kprintf("EBP: %.9x    SS: %.5x\n", regs->ebp, regs->ss);
	kprintf("ESI: %.9x\n", regs->esi);
	kprintf("EDI: %.9x\n", regs->edi);
	kprintf("EFLAGS: %09x\n", regs->eflags);
}

/**
 * Halt the CPU
 */
void halt_cpu(void)
{
	asm("hlt");
}

