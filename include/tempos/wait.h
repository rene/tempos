/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: wait.h
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

#ifndef WAIT_H

	#define WAIT_H

	#include <unistd.h>
	#include <tempos/sched.h>
	#include <linkedl.h>

	/** Number of wait addresses */
	#define WAIT_ADDRESS_SIZE 256

	/* Sleep addresses */

	/** Wait for disk operation at primary IDE */
	#define WAIT_INT_IDE_PRI  14
	/** Wait for disk operation at secondary IDE */
	#define WAIT_INT_IDE_SEC  15

	/* Prototypes */

	void init_wait_queues(void);

	void sleep_on(int sleep_addr);

	void wakeup(int sleep_addr);

#endif /* WAIT_H */

