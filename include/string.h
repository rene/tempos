/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: string.h
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

#ifndef STRING_H

	#define STRING_H

	#include <stdlib.h>
	#include <unistd.h>

	char *strcat(char *dest, const char *src);

	//char *strchr(const char *s, int c);

	int strcmp(const char *s1, const char *s2);

	//int strcoll(const char *s1, const char *s2);

	char *strcpy(char *dest, const char *src);

	//size_t strcspn(const char *s, const char *reject);

	//char *strdup(const char *s);

	//char *strfry(char *string);

	size_t strlen(const char *s);

	char *strncat(char *dest, const char *src, size_t n);

	int strncmp(const char *s1, const char *s2, size_t n);

	char *strncpy(char *dest, const char *src, size_t n);

	//char *strpbrk(const char *s, const char *accept);

	//char *strrchr(const char *s, int c);

	//char *strsep(char **stringp, const char *delim);

	//size_t strspn(const char *s, const char *accept);

	char *strstr(const char *haystack, const char *needle);

	//char *strtok(char *s, const char *delim);

	//size_t strxfrm(char *dest, const char *src, size_t n);

	void *memcpy(void *dest, const void *src, size_t n);

#endif /* STRING_H */

