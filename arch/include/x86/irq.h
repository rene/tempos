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

#ifndef ARCH_X86_IRQ_H

	#define ARCH_X86_IRQ_H

	#include <unistd.h>
	#include <linkedl.h>
	#include <x86/x86.h>


	#define N_IRQ			  16
	#define MAX_IRQH_NAME 	 256

	#define SA_SHIRQ		0x01 /* Shared IRQ */

	/** Interrupt handler struct */
	struct _irq_handler {
		uint32_t id;
		char name[MAX_IRQH_NAME];
		void (*handler)(int, pt_regs *);
	};

	/** IRQ queue struct */
	struct _irq_queue {
		uchar8_t irqnum;
		uint16_t flags;
		llist *queue;
	};

	typedef struct _irq_handler irq_handler_t;
	typedef struct _irq_queue   irq_queue_t;
	void init_IRQ(void);

	void do_irq(uint32_t irqnum, pt_regs regs);

	int request_irq(uint16_t irq, void (*handler)(int, pt_regs *), \
						uint16_t flags, const char *name);


	/* These are implemented in assembly (see isr.S) */
	 extern void isr_irq0(void);
	 extern void isr_irq1(void);
	 extern void isr_irq2(void);
	 extern void isr_irq3(void);
	 extern void isr_irq4(void);
	 extern void isr_irq5(void);
	 extern void isr_irq6(void);
	 extern void isr_irq7(void);
	 extern void isr_irq8(void);
	 extern void isr_irq9(void);
	 extern void isr_irq10(void);
	 extern void isr_irq11(void);
	 extern void isr_irq12(void);
	 extern void isr_irq13(void);
	 extern void isr_irq14(void);
	 extern void isr_irq15(void);

#endif /* EXCEPTIONS_H */

