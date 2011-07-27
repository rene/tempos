/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: semaphore.h
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

#ifndef SEMAPHORE_H

	#define SEMAPHORE_H

	#include <unistd.h>

	/** Semaphore structure */
	typedef int16_t sem_t;

	/* prototypes */
	int mutex_init(sem_t *mutex);
	void mutex_lock(sem_t *mutex);
	void mutex_unlock(sem_t *mutex);
	int mutex_is_locked(sem_t mutex);

#endif /* SEMAPHORE_H */

