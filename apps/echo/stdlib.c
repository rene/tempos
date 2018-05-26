/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: stdlib.c
 * Desc: Implements C functions from stdlib.h
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

#include "stdlib.h"
#include "ctype.h"

int atoi(const char *nptr)
{
	char *nstr = (char *)nptr;
	char isneg = 0;
	int sum    = 0;
	int mult   = 1;

	if(*nstr == '-') {
		isneg = 1;
		nstr++;
	}

	while(*nstr++) {
		if(*nstr && !isdigit(*nstr))
			return(0);
	}

	nstr--;
	while(nstr-- != (nptr + isneg)) {
		sum  += (*nstr - '0') * mult;
		mult *= 10;
	}

	if(isneg)
		return(sum * -1);
	else
		return(sum);
}

