/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: kernel.c
 * Desc: The TempOS kernel
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
#include <tempos/mm.h>
#include <tempos/timer.h>
#include <tempos/jiffies.h>
#include <tempos/delay.h>
#include <drv/i8042.h>
#include <drv/ata_generic.h>
#include <string.h>
#include <stdlib.h>
#include <linkedl.h>


/* information passed from first stage */
karch_t kinfo;

extern uint32_t *kpagedir;


/**
 * tempos_main
 *
 * This is the function called when first stage is done, which means that
 * all dependent machine boot code was executed. See arch/$ARCH/boot/karch.c
 */
void tempos_main(karch_t kinf)
{
	/* Start the second stage */
	memcpy(&kinfo, &kinf, sizeof(karch_t));

	/* keep init_timer() in first palce because drivers use timer functions */
	init_timer();
	calibrate_delay();

	/* Keyboard */
	init_8042();

	/* ATA controller */
	init_ata_generic();

	/* For test */
	kprintf("We are in TempOS kernel!\n");
	kprintf("Command line passed: %s\n", kinfo.cmdline);

	/*new_alarm(jiffies + (3 * HZ), test, 2);*/
	/* Call a system call */
	/*asm volatile("movl $4,  %%eax  \n" // syscall number
				 "movl $12, %%ebx  \n" // count
				 "movl %0,  %%ecx  \n" // hello
				 "movl $0,  %%edx  \n" // fd
				 "int $0x85" : : "g" (hello) );

	kprintf("\nI'am back from syscall!!\n");*/

	kprintf("1 ");
	mdelay(1000);
	kprintf("2 ");
	mdelay(1000);
	kprintf("3 ");

	for(;;);
}


void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	for(;;);
}

