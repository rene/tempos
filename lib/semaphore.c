/**
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: semaphore.c
 * Desc: Implements semaphore functions.
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

#include <semaphore.h>
#include <stdlib.h>
#include <arch/io.h>


/**
 * Init a mutex semaphore.
 * @param mutex Pointer to sem_t mutex.
 * @return int 0 if mutex was started, -1 otherwise.
 */
int mutex_init(sem_t *mutex)
{
	if (mutex == NULL) {
		return -1;
	} else {
		*mutex = 1;
		return 0;
	}
}


/**
 * Lock a mutex semaphore.
 * @param mutex Semaphore to lock.
 */
void mutex_lock(sem_t *mutex)
{
	cli();
	(*mutex)--;
	sti();
}


/**
 * Unlock a mutex semaphore.
 * @param mutex Semaphore to unlock.
 */
void mutex_unlock(sem_t *mutex)
{
	cli();
	(*mutex)++;
	sti();
}


/**
 * Get the state of a mutex
 * @param mutex
 * @return int 1 if mutex is locked, 0 otherwise.
 */
int mutex_is_locked(sem_t mutex)
{
	int ret;

	cli();
	if (mutex != 0) {
		ret = 0;
	} else {
		ret = 1;
	}
	sti();

	return ret;
}

