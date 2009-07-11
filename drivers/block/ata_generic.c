/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ata_generic.c
 * Desc: Generic driver for ATA controllers
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
#include <drv/i8042.h>
#include <x86/irq.h>
#include <x86/io.h>


#define TIMEOUT		(jiffies + (HZ / 40)) /* timeout in 40ms */





/**
 * init_ata_generic
 *
 * Initialize the generic driver for ATA controllers.
 * This function will look for disks connected to the bus
 * and initialize them.
 */
void init_ata_generic(void)
{
	kprintf(KERN_INFO "Initializing generic IDE controller...\n");

	//

}


