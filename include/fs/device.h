/*
 * Copyright (C) 2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: device.h
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

#ifndef VFS_DEVICE_H

	#define VFS_DEVICE_H

	/** Number of maximum block device drivers */
	#define MAX_DEVBLOCK_DRIVERS 256
	/** Number of maximum character device drivers */
	#define MAX_DEVCHAR_DRIVERS  256

	/** Character device driver type */
	#define DRIVER_TYPE_CHAR  0x01
	/** Block device driver type */
	#define DRIVER_TYPE_BLOCK 0x02
	/** Device driver type (not specified) */
	#define DRIVER_TYPE_OTHER 0x03

	/** Character device type */
	#define DEV_TYPE_CHAR DRIVER_TYPE_CHAR
	/** Block device type */
	#define DEV_TYPE_BLOCK DRIVER_TYPE_BLOCK


	/** Device structure */
	struct _device_t {
		/** Major number */
		int major;
		/** Minor number */
		int minor;
	};

	/** Device driver structure */
	struct _device_driver_t {
		/** Major number */
		int major;
		/** Type of driver: Block, Character, Other */
		char type;
	};

	typedef struct _device_t dev_t;
	typedef struct _device_driver_t dev_driver_t;

#endif /* VFS_DEVICE_H */

