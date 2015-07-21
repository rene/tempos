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
#ifdef TKGDB
#include <drv/serial.h>
#endif


/** information passed from first stage */
karch_t kinfo;

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

void thread1(void *arg)
{
	int *t = (int*)arg;
	kprintf("Ola Mundo! %x\n", t);
	mdelay(1000);
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
	char rdev_str[10], *rstr, *init;
#ifdef TKGDB
	char *tkgdb_str;
#endif
	dev_t rootdev;
	size_t i, rdev_len;
	//task_t *idle_th;
	
	/* NOTE: keep calling order for the functions below */

	/* Create idle thread */
	//idle_th = kernel_thread_create(DEFAULT_PRIORITY, idle_thread, NULL);
	kernel_thread_create(DEFAULT_PRIORITY, idle_thread, NULL);

	/* Initialize Virtual File System layer */
	register_all_fs_types();

	/* Initialize PID numbers */
	init_pids();

	/* ATA controller */
	init_ata_generic();

	/* Show and parse command line */
	kprintf(KERN_INFO "Kernel command line: %s\n", kinfo.cmdline);
	parse_cmdline((char*)kinfo.cmdline);

#ifdef TKGDB
	struct serial_interface serial;

	kprintf(KERN_INFO "Using serial.\n");
	tkgdb_str = cmdline_get_value("kgdbwait");
	if (tkgdb_str != NULL) {
		serial_init(&serial, 115200);
		/* Initialize debugger. */
	}

        while(1);
#endif

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


	/* TEST: Read root directory */
	/*kprintf("DEBUG:\n");
	vfs_inode *root = vfs_namei("/");
	if ( !(root->i_mode & S_IFDIR) ) {
		panic("Not dir.");
	}
	char *block = root->sb->sb_op->get_fs_block(root->sb, root->i_block[0]);
	vfs_directory dir;
	
	int pos = 0;
	int newpos = 0;
	kprintf("/: %ld\n", root->i_size);
	while(1) {
		memcpy(&dir.inode, &block[pos], sizeof(uint32_t));
		if (dir.inode == 0) {
			break;
		} else {
			pos += sizeof(uint32_t);
		}

		memcpy(&dir.rec_len, &block[pos], sizeof(uint16_t));
		pos += sizeof(uint16_t);

		memcpy(&dir.name_len, &block[pos], sizeof(uchar8_t));
		pos += 2 * sizeof(uchar8_t);

		memcpy(&dir.name[0], &block[pos], dir.name_len);
		dir.name[dir.name_len] = '\0';
		newpos += dir.rec_len;
		pos = newpos;

		kprintf("%.8ld|", dir.inode);
		//kprintf("|%ld\n", dir.rec_len);
		//kprintf("|%ld\n", dir.name_len);
		kprintf("%s\n",  dir.name);
	}

	kfree(block);

	//vfs_inode *arq = vfs_namei("/test.txt");
	vfs_inode *arq = vfs_namei("/test2.txt");
	//vfs_inode *arq = vfs_iget(root->sb, 12);
	//vfs_inode *arq = vfs_iget(root->sb, 14);
	//vfs_bmap_t bk = vfs_bmap(arq, 12298);
	vfs_bmap_t bk = vfs_bmap(arq, 274442);
	//vfs_bmap_t bk = vfs_bmap(arq, 67384320);
	*/
	
	/* Load init */
	vfs_inode *arq = vfs_namei(init);
	vfs_bmap_t bk = vfs_bmap(arq, 0);
	//kprintf("bk.blk_number = %ld\n", bk.blk_number);
	//kprintf("bk.blk_offset = %ld\n", bk.blk_offset);
	//kprintf("bk.blk_breada = %ld\n", bk.blk_breada);
	char *block = arq->sb->sb_op->get_fs_block(arq->sb, bk.blk_number);
	
	//kfree(block);

	_exec_init(block);


	/*pid_t pid = _fork(GET_TASK(cur_task));

	if (pid != 0) {
		kprintf("Eu sou o PAI!\n");
		while(1) {
			kprintf("PAI: %d\n", pid);
			mdelay(500);
		}
	} else {
		kprintf("Eu sou o Filho!\n");
		while(1) {
			kprintf("Filho: %d\n", GET_TASK(cur_task)->pid);
			mdelay(500);
		}
	}*/

/*	task_t *th1 = kernel_thread_create(DEFAULT_PRIORITY, thread1, (void*)0x24);

	kernel_thread_wait(th1);

	kprintf("Acordei!\n");*/

	/* fork thread */
	/* call execve_init */

	/**
	 * We are done. User process "init" is running, now
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

