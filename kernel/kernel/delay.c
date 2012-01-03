/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: delay.c
 * Desc: Delay functions
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

#include <tempos/delay.h>
#include <tempos/kernel.h>
#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <unistd.h>

/** BogoMIPS calculated at system startup */
uint32_t bogomips;


/**
 * Calibrate delay (calculate BogoMIPS)
 */
void calibrate_delay(void)
{
	uint32_t timeout;
	bogomips = 0;

	kprintf(KERN_INFO "Calibrating loop delay...");
	timeout = jiffies + ((HZ / 10)); /* Calibration takes 100ms */
	
	while( !time_after_eq(jiffies, timeout) )
			bogomips++;
	
	/* FIXME: 3 it's a factor correction */
	bogomips = bogomips / 300000; /* microsecond precision (us) */

	kprintf(KERN_INFO "%d BogoMIPS\n", bogomips);
}


/**
 * Delay in microseconds
 *
 * \param usecs Microseconds to wait
 */
void udelay(uint32_t usecs)
{
	uint32_t steps = usecs * bogomips;

	while( steps-- );
}


/**
 * Delay in milliseconds
 *
 * \param msecs Miliseconds to wait
 */
void mdelay(uint32_t msecs)
{
	udelay(msecs * 1000);
}

