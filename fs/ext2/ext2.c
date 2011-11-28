/*
 * Copyright (C) 2009-2011 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ext2.c
 * Desc: Implements the Extended File System Version 2 (EXT2) File System
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

#include <fs/vfs.h>
#include <fs/ext2/ext2.h>


/**
 * Division a/b with rounded up.
 * \param a Value of a.
 * \param b Value of b.
 * \return Result of division a/b with rounded up.
 **/
uint32_t div_rup(uint32_t a, uint32_t b)
{
	uint32_t res = (a/b);
	if ((res*b) < a) {
		res++;
	}
	return res;
}


/**
 * Return block size in bytes.
 * \param sb Super block.
 * \return Block size in bytes.
 */
uint32_t get_block_size(superblock_st sb)
{
	uint32_t size = 1024; /* default */

	switch(sb.s_log_block_size) {
		case BLOCK_SIZE_2k:
			size = 2048;
			break;

		case BLOCK_SIZE_4k:
			size = 4096;
			break;
	}

	return size;
}

