/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: syscall.h
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

#ifndef SYSCALL_H

	#define SYSCALL_H

	#define SYSCALL_COUNT 5

#ifndef ASM
	#include <unistd.h>

	int      sys_exit(int status);
	void     sys_fork(void);
	int      sys_execve(const char *filename, char *const argv[], char *const envp[]);
	ssize_t  sys_read(int fd, void *buf, size_t count);
	ssize_t  sys_write(int fd, const void *buf, size_t count);
#endif

#endif /* SYSCALL_H */


