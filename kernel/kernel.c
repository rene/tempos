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

extern pagedir_t *kerneldir;

/*char kstack[4096];
void k_thread_1(void);
extern tss_t task_tss;*/


/**
 * tempos_main : Second stage
 *
 * This is the function called when first stage is done, which means that
 * all dependent machine boot code was executed. See arch/$ARCH/boot/karch.c
 */
void tempos_main(karch_t kinf)
{
	memcpy(&kinfo, &kinf, sizeof(karch_t));

	/* keep init_timer() in first palce because drivers use timer functions */
	init_timer();
	calibrate_delay();

	/* Keyboard */
	init_8042();

	/* ATA controller */
	init_ata_generic();

	/* Init scheduler */
	init_scheduler();

	/* Init Virtual File System */
	register_all_fs_types();

	/* Show command line */
	kprintf(KERN_INFO "Kernel command line: %s\n", kinfo.cmdline);

	/* Test *
	sem_t mt;
	mutex_init(&mt);
	kprintf(KERN_DEBUG "Value of mutex: %d\n", (int)mt);
	mutex_lock(&mt);
	if ( mutex_is_locked(mt) ) {
		kprintf(KERN_DEBUG "MUTEX LOCKED!: %d\n", (int)mt);
	}
	kprintf(KERN_INFO "We are in TempOS kernel!\n");
	mutex_unlock(&mt);
	kprintf(KERN_DEBUG "Value of mutex: %d\n", (int)mt);*/


	/*new_alarm(jiffies + (3 * HZ), test, 2);*/
	/* Call a system call */
	/*asm volatile("movl $4,  %%eax  \n" // syscall number
				 "movl $12, %%ebx  \n" // count
				 "movl %0,  %%ecx  \n" // hello
				 "movl $0,  %%edx  \n" // fd
				 "int $0x85" : : "g" (hello) );

	kprintf(KERN_INFO "\nI'am back from syscall!!\n");*/

	kprintf(KERN_INFO "1 ");
	mdelay(1000);
	kprintf(KERN_INFO "2 ");
	mdelay(1000);
	kprintf(KERN_INFO "3 ");

	/*task_st kth;

	memset(&kth, 0, sizeof(kth));

	kth.arch_tss.cr3 = (uint32_t)kerneldir;
	kth.arch_tss.eip = (uint32_t)k_thread_1;

	kth.arch_tss.cs = 0x08; // KERNEL_CS
	kth.arch_tss.ds = 0x10; // KERNEL_DS
	kth.arch_tss.es = 0x10;
	kth.arch_tss.fs = 0x10;
	kth.arch_tss.gs = 0x10;

	kth.arch_tss.ss0 = 0x10;
	kth.arch_tss.esp0 = (uint32_t)kstack;

	kth.arch_tss.iomap = sizeof(task_tss);

	task_tss = kth.arch_tss;

	asm("cli                 \n"
		"movl  %%esp, %%eax  \n"
		"movl  %0, %%esp     \n"
		"pushl %%eax         \n"
		"sti \n"
		"ljmp  %1, %2        \n"
		"popl  %%eax         \n"
		"movl  %%eax, %%esp  \n" :: "i" (kstack), "i" (KERNEL_CS), "i" (k_thread_1) : "eax"); */

	for(;;);
}


void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	for(;;);
}

void k_thread_1(void)
{
	kprintf(KERN_INFO "Hello, I'm the Kernel Thread 1\n");
	mdelay(1000);
	kprintf(KERN_INFO "Hello, I'm the Kernel Thread 1\n");
	for(;;);
}

