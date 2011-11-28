/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: devices.c
 * Desc: Implements the API to register device drivers
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
#include <fs/vfs.h>
#include <fs/device.h>

/** Table of device drivers for character devices */
dev_char_driver_t *char_dev_drivers[MAX_DEVCHAR_DRIVERS];

/** Table of device drivers for block devices */
dev_blk_driver_t *block_dev_drivers[MAX_DEVBLOCK_DRIVERS];


/**
 * Initialize drivers interface.
 */
void init_drivers_interface(void)
{
	int i;

	/* Character drivers table */
	for (i = 0; i < MAX_DEVCHAR_DRIVERS; i++) {
		char_dev_drivers[i] = NULL;
	}

	/* Block drivers table */
	for (i = 0; i < MAX_DEVBLOCK_DRIVERS; i++) {
		block_dev_drivers[i] = NULL;
	}
}


/**
 * Register a block device driver.
 *
 * \param driver Block driver structure.
 * \see fs/device.h
 * \return int 0 on Success, -1 otherwise.
 */
int register_block_driver(dev_blk_driver_t *driver)
{
	if (driver == NULL) {
		return -1;
	}

	/* Check if there is another registered driver with the same major number */
	if (block_dev_drivers[driver->major] != NULL) {
		return -1;
	}

	/* Create a block hash queue for the device */
	driver->buffer_queue = create_hash_queue(driver->size);
	if (driver->buffer_queue == NULL) {
		return -1;
	}
	
	block_dev_drivers[driver->major] = driver;

	return 0;
}

