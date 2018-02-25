/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: serial.c
 * Written by: Julio Faracco
 * Desc: Generic driver for Serial communication
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
#include <tempos/delay.h>
#include <tempos/wait.h>
#include <fs/device.h>
#include <fs/dev_numbers.h>
#include <fs/partition.h>
#include <drv/serial.h>
#include <arch/irq.h>
#include <arch/io.h>
#include <linkedl.h>

static inline void
serial_out(struct serial_interface *interface, uint16_t addr, uchar8_t val)
{
	outb(val, interface->serial_port + addr);
}

static inline uchar8_t
serial_in(struct serial_interface *interface, uint16_t addr)
{
	return inb(interface->serial_port + addr);
}

uchar8_t serial_read(struct serial_interface *interface)
{
	uchar8_t c;

	do {
		c = serial_in(interface, SERIAL_LSR);
	} while ((c & 0x1e) || ((c & 1) == 0));

	return serial_in(interface, SERIAL_DLL);
}

void serial_write(struct serial_interface *interface, char c)
{
	while ((serial_in(interface, SERIAL_LSR) & 0x20) == 0);

	serial_out(interface, SERIAL_DLL, c);
}

int serial_init(struct serial_interface *interface, int baud)
{
	interface->serial_port = SERIAL_PORT_COM1;

	serial_out(interface, SERIAL_MCR, SERIAL_LCR_STATE);
	serial_out(interface, SERIAL_IER, SERIAL_DIS);

	interface->baud = SERIAL_BAUD_DEFAULT / baud;
	serial_out(interface, SERIAL_LCR, 0x83);
	serial_out(interface, SERIAL_DLL, interface->baud & 0xff);
	serial_out(interface, SERIAL_DLM, SERIAL_DIS);
	serial_out(interface, SERIAL_LCR,
		SERIAL_LCR_8BIT | SERIAL_LCR_1STOP | SERIAL_LCR_NO_PARITY);

	return 0;
}
