/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: read.c
 * Desc: Syscall read
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

#include <tempos/syscall.h>
#include <tempos/kernel.h>
#include <drv/i8042.h>
#include <drv/serial.h>

_pushargs ssize_t sys_read(int fd, void *buf, size_t count)
{
	ssize_t bytes = 0;
	uchar8_t *buffer = (uchar8_t*)buf;

	if (fd == 0) {
		/* Read from keyboard or serial */
		if (console_over_serial == 1) {
			buffer[0] = serial_read(&tty_serial);
		} else {
			buffer[0] = kbc_read_from_buffer();
		}
		bytes     = 1;
		kprintf(KERN_DEBUG "%c", buffer[0]);
	}

	//kprintf(KERN_DEBUG "read: %d -- %s -- %d\n", fd, buf, count);

	return(bytes);
}


