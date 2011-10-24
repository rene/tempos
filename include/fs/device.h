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

	#include <unistd.h>
	#include <fs/bhash.h>


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
		/** Type: Block, Character */
		char type;
	};

	/** Block device operations */
	struct _blk_dev_op {
		/** read(): Read a block */
		int (*read_block) (int, int, buff_header_t *);
		/** write_async(): Write asynchronously */
		int (*write_async_block) (int, int, buff_header_t *);
		/** write_sync(): Write synchronously */
		int (*write_sync_block) (int, int, buff_header_t *);
	};

	/** Character device operations */
	struct _char_dev_op {
	};

	/** Character device driver structure */
	struct _char_device_driver_t {
		/** Major number */
		int major;
	};

	/** Block device driver structure */
	struct _blk_device_driver_t {
		/** Major number */
		int major;
		/** Device size (for block devices ) */
		uint64_t size;
		/** Buffer queue for block devices */
		buff_hashq_t *buffer_queue;
		/** Driver operations */
		struct _blk_dev_op *dev_ops;
	};

	typedef struct _device_t dev_t;
	typedef struct _char_device_driver_t dev_char_driver_t;
	typedef struct _blk_device_driver_t  dev_blk_driver_t;

	/** Table of device drivers for character devices */
	extern dev_char_driver_t *char_dev_drivers[MAX_DEVCHAR_DRIVERS];

	/** Table of device drivers for block devices */
	extern dev_blk_driver_t *block_dev_drivers[MAX_DEVBLOCK_DRIVERS];

	/* Prototypes */
	void init_drivers_interface(void);
	
	int register_block_driver(dev_blk_driver_t *driver);

#endif /* VFS_DEVICE_H */

