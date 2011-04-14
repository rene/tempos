/**
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: devices.h
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

#ifndef DEVICES_H

	#define DEVICES_H

	/**
	 * This file contains the list of device numbers.
	 * All numbers placed here follows the LINUX ALLOCATED DEVICES
	 * list, revised at 6th April 2009. For more information, please
	 * see http://www.kernel.org
	 **/

	/* TempOS device numbers declaration: DEVNUM_<NAME>
	 * Where <NAME>, usually (it's not a rule), is the same as 
	 * used in device file name (e.g. /dev/<NAME>) */

	/* 1 char - Memory Devices */
	#define DEVNUM_NULL          3
	#define DEVNUM_ZERO          5
	#define DEVNUM_RANDOM        8
	#define DEVNUM_URANDOM       9

	/* 3 block */
	#define DEVNUM_HDA           0
	#define DEVNUM_HDB           64
	
	/* 2 block */
	#define DEVNUM_HDC           0
	#define DEVNUM_HDD           64

	/* 5 char - Alternate TTY devices */
	#define DEVNUM_TTY           0
	#define DEVNUM_CONSOLE       1


	/* Now, the major numbers */
	#define DEVMAJOR_MEMORY      1
	#define DEVMAJOR_ATA_PRI     3
	#define DEVMAJOR_ATA_SEC     2

#endif /* DEVICES_H */

