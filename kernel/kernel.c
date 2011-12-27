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
#include <fs/device.h>
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

/**
 * This is the main kernel thread. When TempOS initializes his scheduler
 * the kernel process becomes a kernel thread (running this function). So
 * this thread executes all functions after scheduler initialization.
 *
 * \param arg Not used.
 */
void kernel_main_thread(void *arg)
{
	char rdev_str[10], *rstr, *init;
	dev_t rootdev;
	size_t i, rdev_len;
	task_t *idle_th;
	
	/* NOTE: keep calling order for the functions below */

	/* Create idle thread */
	idle_th = kernel_thread_create(DEFAULT_PRIORITY, idle_thread, NULL);

	/* Initialize Virtual File System layer */
	register_all_fs_types();

	/* ATA controller */
	init_ata_generic();

	/* Show and parse command line */
	kprintf(KERN_INFO "Kernel command line: %s\n", kinfo.cmdline);
	parse_cmdline((char*)kinfo.cmdline);

	/* Mount root file system */
	rstr = cmdline_get_value("root");
	strcpy(rdev_str, rstr);
	rdev_len = strlen(rstr);
	rdev_str[rdev_len] = '\0';
	for (i = 0; i < rdev_len; i++) {
		if (rdev_str[i] == ':') {
			rdev_str[i] = '\0';
			rootdev.major = atoi(rdev_str);
			rootdev.minor = atoi(&rdev_str[i+1]);
			break;
		}
	}
	if (i == rdev_len) {
		panic("Kernel command line root argument bad formated.");
	}
	if ( !vfs_mount_root(rootdev) ) {
		panic("VFS ERROR: Could not mount root file system.");
	}

	/* Load init */
	init = cmdline_get_value("init");
	if (init == NULL) {
		init = DEFAULT_INIT_PROCCESS;
	}
	kprintf("Loading %s...\n", init);

	/* fork thread */
	/* call execve_init */

	/**
	 * We are done. User proccess "init" is running, now
	 * idle_thread can go away...
	 */
	/* thread_done = 1; */
	for(;;) {
		/* measure system load */
	}

	/* tests */
	/*buff_header_t *buff;
	buff = bread(3, 1, 368);
	for (i = 0; i < 512; i++) {
		kprintf("%c ", buff->data[i]);
	}*/
	/*getblk(3, 0, 24);
	getblk(3, 0, 4);
	getblk(3, 0, 8);
	getblk(3, 0, 16);
	getblk(3, 0, 24);
	 kprintf(KERN_INFO "Hello, I'm the main kernel process!\n"); */
	/*new_alarm(jiffies + (3 * HZ), test, 2);*/
	/* Call a system call */
	/*char *hello = "Hello World!\n";
	asm("movl $4,  %%eax  \n" // syscall number
		 "movl $13, %%ebx  \n" // count
		 "movl %0,  %%ecx  \n" // hello
		 "movl $0,  %%edx  \n" // fd
		 "pushl %%eax\n"
		 "int $0x85" : : "g" (hello) : "eax", "ebx", "ecx", "edx");
	kprintf(KERN_INFO "\nI'am back from syscall!!\n");*/
	/*
	kprintf(KERN_INFO "\n1 ");
	mdelay(1000);
	kprintf(KERN_INFO "2 ");
	mdelay(1000);
	kprintf(KERN_INFO "3 ");*/
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
 * Show error message and dump halt CPU.
 * This function should be used when something goes really wrong.
 * System will hang out without any chance of recovery.
 *
 * \param format Error message (formated).
 */
void panic(const char *format, ...)
{
	char str[MAX_KPRINTF_SIZE];
	va_list args;

	va_start(args, format);
	vsprintf(str, format, args);
	va_end(args);

	kprintf(KERN_CRIT "\nPANIC: %s\n", str);

	halt_cpu();
	for(;;);
}

