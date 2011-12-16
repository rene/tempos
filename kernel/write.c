/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: write.c
 * Desc: Syscall write
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

_pushargs ssize_t sys_write(int fd, const void *buf, size_t count)
{
	char buffer[256];
	char *tmp = (char *)buf;
	size_t i;

	if(count < 256) {
		for(i=0;i<=count;i++)
			buffer[i] = tmp[i];
	}

	if(fd == 0) {
		kprintf(KERN_DEBUG "SYSCALL WRITE: %s\n", buffer);
	}

	return(10);
}


