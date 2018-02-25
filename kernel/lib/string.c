/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: string.c
 * Desc: Implements strings C functions
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

#include <string.h>


char *strcat(char *dest, const char *src)
{
	char *pdest = dest;

	while(*pdest != '\0')
		pdest++;

	while(*src != '\0')
		*pdest++ = *src++;
	*pdest = '\0';

	return dest;
}


int strcmp(const char *s1, const char *s2)
{
	while((*s1 == *s2) && *s1) {
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


char *strcpy(char *dest, const char *src)
{
	size_t len = strlen(src);
	memcpy(dest, src, len);
	return dest;
}


size_t strlen(const char *s)
{
	char *tmp = (char*)s;
	while(*tmp != '\0')
		tmp++;
	return (size_t)(tmp - s);
}


char *strncat(char *dest, const char *src, size_t n)
{
	size_t p = n;
	char *pdest = dest;

	while(*pdest != '\0')
		pdest++;

	while(p--)
		*pdest++ = *src++;
	*pdest = '\0';

	return dest;
}


int strncmp(const char *s1, const char *s2, size_t n)
{
	size_t p = n;
	while( (*s1++ != *s2++) && (*s1 && *s2) && p-- );
	return ((int)((unsigned char)*s1 - (unsigned char)*s2));
}


char *strncpy(char *dest, const char *src, size_t n)
{
	memcpy(dest, src, n);
	return dest;
}


char *strstr(const char *haystack, const char *needle)
{
	char *res;
	char *phay = (char *)haystack;
	char *pnee = (char *)needle;

	res = phay;
	while(*phay && *pnee) {
		if(*phay != *pnee)
			res = phay;
		else
			pnee++;

		phay++;
	}

	if(!*pnee)
		return res;
	else
		return NULL;
}


void *memcpy(void *dest, const void *src, size_t n)
{
	size_t p    = n;
	char *pdest = (char*)dest;
	char *psrc  = (char*)src;

	while(p--)
		*pdest++ = *psrc++;

	return dest;
}


void *memset(void *s, int c, size_t n)
{
	size_t p = n;
	char *pdest = (char*)s;

	while(p--)
		*pdest++ = c;

	return s;
}

