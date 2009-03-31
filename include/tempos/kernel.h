/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: kernel.h
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
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

	#include "../arch/x86/boot/video.h" /* temporary */

	#define CHECK_BIT(a, b)		((a >> b) & 0x01)
	#define SET_BIT(a, b)		a |= (0x01 << b)


	/* kprintf messages */
	#define KERN_CRIT			"!0!"
	#define KERN_ERROR			"!1!"
	#define KERN_WARNING		"!2!"
	#define KERN_INFO			"!3!"
	#define KERN_DEBUG			"!4!"


	struct _mmap_tentry {
  		ulong32_t base_addr_low;
  		ulong32_t base_addr_high;
  		ulong32_t length_low;
  		ulong32_t length_high;
  		ulong32_t type;
	};

	struct _karch_t {
		uchar8_t            *cmdline;
		ulong32_t           mmap_size;
		struct _mmap_tentry *mmap_table;
	};

	typedef struct _karch_t karch_t;
	typedef struct _mmap_tentry mmap_tentry;


	int vsprintf(char *str, const char *format, va_list ap);
	int sprintf(char *str, const char *format, ...);
	int kprintf(const char *format, ...);

	void tempos_main(karch_t kinf);

#endif /* KERNEL_H */

