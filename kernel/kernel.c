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
#include <tempos/wait.h>
#include <drv/i8042.h>
#include <drv/ata_generic.h>
#include <fs/vfs.h>
#include <string.h>
#include <stdlib.h>
#include <linkedl.h>
#include <semaphore.h>

/** information passed from first stage */
karch_t kinfo;

/** kernel stack */
extern uint32_t stack;

/** kernel main thread */
void kernel_main_thread(void *arg);

/** idle thread */
void idle_thread(void *arg);

/** indicates when idle_thread should exit */
static int thread_done = 0;

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

	/* Initialize wait queues */
	init_wait_queues();

	/* Initialize the scheduler */
	init_scheduler(kernel_main_thread);

	/* Should never reaches here! */
	panic("Error on initialize scheduler!");
}

void fth1(void *arg)
{
	int i;
	//while(1) {
	for (i = 0; i < 10; i++) {
		mdelay(10);
		kprintf("Eu sou a thread 1\n");
	}
	kernel_thread_exit(0);
}
void fth2(void *arg)
{
	task_t *kth = (task_t*)arg;
	kprintf("Eu sou a thread 2, aguardando termino da thread 1\n");
	kernel_thread_wait(kth);
	kprintf("Legal, Thread 1 acabou!\n");
	kernel_thread_exit(0);
}

/**
 * This is the main kernel thread. When TempOS initializes his scheduler
 * the kernel process becomes a kernel thread (running this function). So
 * this thread executes all functions after scheduler initialization.
 *
 * \param arg Not used.
 */
void kernel_main_thread(void *arg)
{
	task_t *idle_th;
	
	/* NOTE: keep calling order for the functions below */

	/* Create idle thread */
	idle_th = kernel_thread_create(DEFAULT_PRIORITY, idle_thread, NULL);

	/* Initialize Virtual File System layer */
	register_all_fs_types();

	/* ATA controller */
	init_ata_generic();

	/* Show command line */
	kprintf(KERN_INFO "Kernel command line: %s\n", kinfo.cmdline);

	/* tests */
	buff_header_t *buff;
	/*buff = bread(3, 0, 2416);*/
	buff = bread(3, 1, 368);
	int i;
	for (i = 0; i < 512; i++) {
		kprintf("%c ", buff->data[i]);
	}
	/*getblk(3, 0, 24);
	getblk(3, 0, 4);
	getblk(3, 0, 8);
	getblk(3, 0, 16);
	getblk(3, 0, 24);
	 kprintf(KERN_INFO "Hello, I'm the main kernel process!\n"); */
	/*new_alarm(jiffies + (3 * HZ), test, 2);*/
	/* Call a system call */
	/*char *hello = "Ola Mundo!\n";
	asm volatile("movl $4,  %%eax  \n" // syscall number
				 "movl $11, %%ebx  \n" // count
				 "movl %0,  %%ecx  \n" // hello
				 "movl $0,  %%edx  \n" // fd
				 "int $0x85" : : "g" (hello) );

	kprintf(KERN_INFO "\nI'am back from syscall!!\n");*/

	/*kprintf(KERN_INFO "1 ");
	mdelay(1000);
	kprintf(KERN_INFO "2 ");
	mdelay(1000);
	kprintf(KERN_INFO "3 ");*/

	task_t *th1, *th2;

	th1 = kernel_thread_create(DEFAULT_PRIORITY, fth1, NULL);
	th2 = kernel_thread_create(DEFAULT_PRIORITY, fth2, th1);

	//kernel_thread_wait(th1);
	//kprintf("Thread 1 acabou!\n");

	/* Mount root file system */
	/* ... */
	//mdelay(1000);
	for(;;);
}

/**
 * This function does nothing.
 * \note This function will run as a kernel thread just to keep another
 * process running when the main kernel thread goes to sleep
 * at system initialization, when there is no user process running yet.
 *
 * \param arg Not used.
 */
void idle_thread(void *arg)
{
	while(!thread_done);
	kernel_thread_exit(0);
}

/**
 * Show error message and dump CPU information.
 * This function should be used when something goes really wrong.
 * System will hang out without any chance of recovery.
 *
 * \param str Error message.
 */
void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	/* FIXME: dump_cpu(); halt_cpu(); */
	for(;;);
}

