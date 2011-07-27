/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: ctype.c
 * Desc: Implements C functions from ctype.h
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

#include <ctype.h>


int isalnum(int c)
{
	return(isalpha(c) || isdigit(c));
}


int isalpha(int c)
{
	return(isupper(c) || islower(c));
}


int isascii(int c)
{
	return(c <= 127);
}


int isblank(int c)
{
	return(c == ' ' || c == '\t');
}


int iscntrl(int c)
{
	return(c >= 1 && c <= 31);
}


int isdigit(int c)
{
	return(c >= '0' && c <= '9');
}


int isgraph(int c)
{
	return(c >= 33 && c <= 126);
}


int islower(int c)
{
	return(c >= 'a' && c <= 'z');
}


int isprint(int c)
{
	return(c >= 32 && c <= 126);
}


int ispunct(int c)
{
	return(isgraph(c) && !isalnum(c));
}


int isspace(int c)
{
	return(c == '\f' || c == '\n' || c == '\r' ||
				c == '\t' || c == '\v');
}


int isupper(int c)
{
	return(c >= 'A' && c <= 'Z');
}


int isxdigit(int c)
{
	if( (c >= 'A' && c <= 'F') ||
		(c >= 'a' && c <= 'f') ||
		(c >= '0' && c <= '9') ) {

		return(1);
	} else {
		return(0);
	}
}

