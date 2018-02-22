/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: serial.h
 * Written by: Julio Faracco
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

#ifndef _SERIAL_H
	#define _SERIAL_H

	#define SERIAL_PORT_COM1 0x3f8
	#define SERIAL_PORT_COM2 0x2f8
	#define SERIAL_PORT_COM3 0x3e8
	#define SERIAL_PORT_COM4 0x2e8

	#define	SERIAL_TXR 0
	#define SERIAL_IER 1

	#define	SERIAL_DLL 0
	#define SERIAL_DLM 1
	#define SERIAL_IIF 2
	#define SERIAL_LCR 3
	#define SERIAL_MCR 4
	#define SERIAL_LSR 5
	#define SERIAL_MSR 6
	#define SERIAL_SR 7

	#define SERIAL_DIS 0x00
	#define SERIAL_LCR_STATE 0x0f
	#define SERIAL_LCR_8BIT	0x03
	#define SERIAL_LCR_1STOP 0x00
	#define SERIAL_LCR_NO_PARITY 0x00
	#define SERIAL_LCR_BREAK 0x40
	#define SERIAL_LCR_DLAB	0x80

	#define SERIAL_LSR_THR_EMPTY 0x10

	#define SERIAL_BAUD_DEFAULT 115200

	struct serial_interface {
		unsigned int irq;
	        uint16_t serial_port;
	        uint16_t io_size;
		unsigned int baud;
		unsigned int stop;
		unsigned int bits;
		int parity;
	};

	struct serial_driver {
	/* Needs improvement. */
	/* 	void* mem_base; */
	/* 	size_t mem_size; */
	/* 	struct list_element node; */
	/* 	struct device_char char_dev; */

	        int (*init)(struct serial_interface *, int);
		void (*write)(const struct serial_interface *, char);
		uchar8_t (*read)(const struct serial_interface*);
	};

	uchar8_t serial_read(struct serial_interface *interface);

	void serial_write(struct serial_interface *interface, char c);

	int serial_init(struct serial_interface *interface, int baud);

#endif
