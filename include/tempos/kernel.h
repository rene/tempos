/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: kernel.h
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

#ifndef KERNEL_H

	#define KERNEL_H

	#include <stdarg.h>
	#include <ctype.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <tempos/error.h>

	#include "../arch/x86/boot/video.h" /* temporary */


	#define CHECK_BIT(a, b)		((a >> b) & 0x01)
	#define SET_BIT(a, b)		a |= (0x01 << b)


	/* kprintf messages */
	#define KERN_CRIT			"!0!"
	#define KERN_ERROR			"!1!"
	#define KERN_WARNING		"!2!"
	#define KERN_INFO			"!3!"
	#define KERN_DEBUG			"!4!"

	/** Maximum string size for kprintf */
	#define MAX_KPRINTF_SIZE 	1024

	/** Command line max size */
	#define CMDLINE_MAX			1024

	/** Max memory regions */
	#define MBOOT_MMAP_MAXREG	  40

	/* Memory map structure
	   Types avaliable:
	   		0x01 - Avaliable
			0x02 - Reserved
			0x03 - ACPI
			0x04 - ACPI NVS */
	#define MTYPE_AVALIABLE		0x01
	#define MTYPE_RESERVED		0x02
	#define MTYPE_ACPI			0x03
	#define MTYPE_ACPI_NVS		0x04

	struct _mmap_tentry {
  		uint32_t base_addr_low;
  		uint32_t base_addr_high;
  		uint32_t length_low;
  		uint32_t length_high;
  		uint32_t type;
	};


	/** Information passed from first stage (karch) */
	struct _karch_t {
		uchar8_t            cmdline[CMDLINE_MAX];
		uint32_t            mem_lower;
		uint32_t            mem_upper;
		uchar8_t			mmap_size;  /* Number of elements */
		struct _mmap_tentry mmap_table[MBOOT_MMAP_MAXREG];
	};

	/** Command line argument: key-value pair */
	struct _cmdline_arg {
		char *key;
		char *value;
	};

	typedef struct _karch_t karch_t;
	typedef struct _mmap_tentry mmap_tentry;
	typedef struct _cmdline_arg cmdline_arg_t;

	int vsprintf(char *str, const char *format, va_list ap);

	int sprintf(char *str, const char *format, ...);

	int kprintf(const char *format, ...);

	void panic(const char *format, ...);

	void tempos_main(karch_t kinf);

	/* command line functions */
	int parse_cmdline(char *cmdline);

	char *cmdline_get_value(char *key);

	/** Although is defined here, this function is architecture dependent, so it
	    shall be implemented on each architecture port code. */
	void halt_cpu(void);

#endif /* KERNEL_H */

