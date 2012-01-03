/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: stat.h
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

#ifndef STAT_H

	#define STAT_H

	/* File mode flags */

	/** format mask */ 
	#define S_IFMT 0xF000
	/** socket */  
	#define S_IFSOCK 0xA000
	/** symbolic link */ 
	#define S_IFLNK 0xC000
	/** regular file */ 
	#define S_IFREG 0x8000
	/** block device */ 
	#define S_IFBLK 0x6000
	/** directory */  
	#define S_IFDIR 0x4000
	/** character device */ 
	#define S_IFCHR 0x2000
	/** fifo */  
	#define S_IFIFO 0x1000
	/** SUID */  
	#define S_ISUID 0x0800
	/** SGID */  
	#define S_ISGID 0x0400
	/** sticky bit */ 
	#define S_ISVTX 0x0200
	/** user mask */ 
	#define S_IRWXU 0x01C0
	/** read */  
	#define S_IRUSR 0x0100
	/** write */  
	#define S_IWUSR 0x0080
	/** execute */  
	#define S_IXUSR 0x0040
	/** group mask */ 
	#define S_IRWXG 0x0038
	/** read */  
	#define S_IRGRP 0x0020
	/** write */  
	#define S_IWGRP 0x0010
	/** execute */  
	#define S_IXGRP 0x0008
	/** other mask */ 
	#define S_IRWXO 0x0007
	/** read */  
	#define S_IROTH 0x0004
	/** write */  
	#define S_IWOTH 0x0002
	/** execute */  
	#define S_IXOTH 0x0001
		

	/** Default file mode: 0666*/
	#define DEFFILEMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#endif /* STAT_H */

