/*
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
#include <tempos/sched.h>
#include <drv/i8042.h>
#include <drv/ata_generic.h>
#include <fs/vfs.h>
#include <string.h>
#include <stdlib.h>
#include <linkedl.h>
#include <semaphore.h>

/** information passed from first stage */
karch_t kinfo;

extern uint32_t stack;
void kernel_main_thread(void *arg);
void idle_thread(void *arg);

int thread_done = 0;

/**
 * tempos_main : Second stage
 *
 * This is the function called when first stage is done, which means that
 * all dependent machine boot code was executed. See arch/$ARCH/boot/karch.c
 */
void tempos_main(karch_t kinf)
{
	memcpy(&kinfo, &kinf, sizeof(karch_t));

	/* keep init_timer() in first place because drivers use timer functions */
	init_timer();
	calibrate_delay();

	/* Keyboard */
	init_8042();

	/* ATA controller */
	init_ata_generic();

	/* Init Virtual File System layer */
	register_all_fs_types();

	/* Show command line */
	kprintf(KERN_INFO "Kernel command line: %s\n", kinfo.cmdline);

	/* Init scheduler */
	init_scheduler(kernel_main_thread);

	/* Should never reaches here! */
	panic("Error on initialize scheduler!");
}

void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	for(;;); /* FIXME: dump_cpu(); halt_cpu(); */
}

extern void teste(void *);

void kernel_main_thread(void *arg)
{
	kprintf(KERN_INFO "Hello, I'm the main kernel process!\n");

	kernel_thread_create(DEFAULT_PRIORITY, idle_thread, NULL);

	/*new_alarm(jiffies + (3 * HZ), test, 2);*/
	/* Call a system call */
	/*asm volatile("movl $4,  %%eax  \n" // syscall number
				 "movl $12, %%ebx  \n" // count
				 "movl %0,  %%ecx  \n" // hello
				 "movl $0,  %%edx  \n" // fd
				 "int $0x85" : : "g" (hello) );

	kprintf(KERN_INFO "\nI'am back from syscall!!\n");*/

	/*kprintf(KERN_INFO "1 ");
	mdelay(1000);
	kprintf(KERN_INFO "2 ");
	mdelay(1000);
	kprintf(KERN_INFO "3 ");*/

	/* Mount root file system */
	/* ... */
	//mdelay(1000);
	for(;;) {
		mdelay(100);
		kprintf("Hello foo!\n");
	}
}

void idle_thread(void *arg)
{
	while(!thread_done) {
		mdelay(20);
		kprintf("Hello bar!\n");
	}
	//kernel_thread_exit(0);
}

