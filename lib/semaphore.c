/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
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
#include <arch/atomic.h>

/**
 * Init a mutex semaphore.
 * \param mutex Pointer to sem_t mutex.
 * \return int 0 if mutex was started, -1 otherwise.
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
 * Get the state of a mutex
 * \param mutex
 * \return int 1 if mutex is locked, 0 otherwise.
 */
int mutex_is_locked(sem_t mutex)
{
	int ret;
	cli();
	if (mutex > 0) {
		ret = 1;
	} else {
		ret = 0;
	}
	sti();
	return ret;
}


/**
 * Make a down operation on the mutex.
 * This function works by first check the value of mutex, if mutex is greater
 * than 0, the value is decremented and the function returns. On the other hand,
 * if mutex is 0, the function will wait in a loop (busy wait) until mutex is
 * locked.
 * \param mutex Mutex
 */
void mutex_spin_down(sem_t *mutex)
{
	atomic_spin_down((uint32_t*)mutex);
}

/**
 * Make an UP operation on mutex.
 * This function just increments the value of mutex.
 * \param mutex Mutex to "unlock".
 */
void mutex_up(sem_t *mutex)
{
	atomic_incl((int32_t*)mutex);
	//atomic_addl((int32_t*)mutex, 10);
}

