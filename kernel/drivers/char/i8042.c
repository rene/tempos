/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: i8042.c
 * Desc: Driver for keyboard controller
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
#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <drv/i8042.h>
#include <arch/irq.h>
#include <arch/io.h>


#define TIMEOUT		(jiffies + (HZ / 40)) /* timeout in 40ms */


//static void keyboard_handler(void);
static void keyboard_handler(int i, pt_regs *regs);

/**
 * Scan codes table
 */
/**
 * This table I got from: http://www.brokenthorn.com/Resources/OSDev19.html
 */
unsigned char scancodes2ascii[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    1,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   1,				/* Right shift */
  '*',
    1,	/* Alt */
  ' ',	/* Space bar */
    1,	/* Caps lock */
    1,	/* 59 - F1 key ... > */
    1,   1,   1,   1,   1,   1,   1,   1,
    1,	/* < ... F11 */
    1,	/* 69 - Num lock*/
    1,	/* Scroll Lock */
    1,	/* Home key */
    1,	/* Up Arrow */
    1,	/* Page Up */
  '-',
    1,	/* Left Arrow */
    1,
    1,	/* Right Arrow */
  '+',
    1,	/* 79 - End key*/
    1,	/* Down Arrow */
    1,	/* Page Down */
    1,	/* Insert Key */
    1,	/* Delete Key */
    1,   1,   '\\',
    1,	/* F11 Key */
    1,	/* F12 Key */
    1,	/* All other keys are undefined */
};		


/**
 * Initialize keyboard controller
 */
void init_8042(void)
{
	kprintf(KERN_INFO "Initializing i8042 keyboard controller...\n");

	/* Routines to initialize i8042 */

	/* Enable keyboard */
	kbc_sendcomm(KBD_ENABLE);

	/* Register IRQ service routine */
	if( request_irq(KBD_IRQ, (void(*)(int, pt_regs *))keyboard_handler, 0, "i8042") < 0) {
		kprintf(KERN_ERROR "Error on initialize i8042\n");
	}

	/* Clear the output buffer */
	inb(KBD_OUT_BUF);
}

/**
 * Interrupt handler
 */
static void keyboard_handler(int i, pt_regs *regs)
{
	uchar8_t scancode = read_scancode();
	
	if ( !(scancode & 0x80) ) {
		kprintf("%c", scancodes2ascii[scancode]);
	}

	//kprintf("IRQ %d REGS: 0x%x 0x%x 0x%x K: %lx \n", i, regs->cs, regs->ds, regs->eflags, key );

	//wakeup(1);
}


/**
 * Send a command to keyboard controller
 */
void kbc_sendcomm(uchar8_t command)
{
	wait_write_8042();
	outb(command, KBD_CMD_BUF);
	wait_read_8042();
}


/**
 * Read output buffer
 */
uchar8_t kbc_read(void)
{
	return(inb(KBD_OUT_BUF));
}


/**
 * Wait for data
 */
void wait_read_8042(void)
{
	int32_t timeout = TIMEOUT;

	while( !time_after(jiffies, timeout) && (!(inb(STATUS_PORT) & OUT_BUF_FULL)) );
	return;
}


/**
 * Wait while 8042 is busy
 */
void wait_write_8042(void)
{
	int32_t timeout = TIMEOUT;

	while( !time_after(jiffies, timeout) && ((inb(STATUS_PORT) & INPT_BUF_FULL)) );
	return;
}


/**
 * Read a key
 */
uchar8_t read_scancode(void)
{
	return kbc_read();
}

