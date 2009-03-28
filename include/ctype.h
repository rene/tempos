/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: ctype.h
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

#ifndef CTYPE_H

	#define CTYPE_H

	int isalnum(int c);
	int isalpha(int c);
	int isascii(int c);
	int isblank(int c);
	int iscntrl(int c);
	int isdigit(int c);
	int isgraph(int c);
	int islower(int c);
	int isprint(int c);
	int ispunct(int c);
	int isspace(int c);
	int isupper(int c);
	int isxdigit(int c);

#endif /* CTYPE_H */

