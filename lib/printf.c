/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: printf.c
 * Desc: Implement the printf family C functions
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
#include <stdlib.h>

#define MAXDIG 20

#define FLAG_ALT	0x00
#define FLAG_ZPAD	0x01
#define FLAG_FB		0x02
#define FLAG_SPACE	0x03
#define FLAG_PLUS	0x04

#define SIGNED         1
#define UNSIGNED       0


static void numtostr(char **dest, char flags, long int value, int base, int prec, char sig);


static void numtostr(char **dest, char flags, long int value, int base, int prec, char sig)
{
	char *ndest = *dest;
	long int pos, div, quo;
	unsigned long int udiv, uquo, rem;
	char temp, sch, fch;
	int i, sh, len;

	/* This is painful, there is a better way? */
	pos = 0;
	if(sig) {
		/* Signed conversion */
		div = value < 0 ? (value * -1) : value;  /* Dividend  (absolute) */
		quo = 1;	  /* Quotient  */
		rem = 0;	  /* Remainder */
		if(base != 16) {
			while(quo != 0) {
				quo        = (div / base);
				rem        = (div % base);
				div        = quo;
				ndest[pos] = (rem + '0');
				pos++;
			}
		} else {
			while(quo != 0) {
				quo       = (div / base);
				rem       = (div % base);
				div       = quo;
				if(rem >= 10) {
					ndest[pos] = (rem - 10 + 'A');
				} else {
					ndest[pos] = (rem + '0');
				}
				pos++;
			}
		}
	} else {
		/* Unsigned conversion */
		udiv = (unsigned)value; /* Dividend  (absolute) */
		uquo = 1;	  			/* Quotient  */
		rem  = 0;	  			/* Remainder */
		if(base != 16) {
			while(uquo != 0) {
				uquo        = (udiv / base);
				rem         = (udiv % base);
				udiv        = uquo;
				ndest[pos] = (rem + '0');
				pos++;
			}
		} else {
			while(uquo != 0) {
				uquo       = (udiv / base);
				rem        = (udiv % base);
				udiv       = uquo;
				if(rem >= 10) {
					ndest[pos] = (rem - 10 + 'A');
				} else {
					ndest[pos] = (rem + '0');
				}
				pos++;
			}
		}
	}
	pos--;
	for(i=0; i<=(pos / 2); i++) {
		temp         = ndest[i];
		ndest[i]     = ndest[pos-i];
		ndest[pos-i] = temp;
	}

	/* Signal */
	sh  = 0;
	sch = 0;
	if( CHECK_BIT(flags, FLAG_SPACE) || CHECK_BIT(flags, FLAG_PLUS) ) {
		sh = 1;
		if(value >= 0) {
			if(	CHECK_BIT(flags, FLAG_PLUS) ) {
				sch = '+';
			} else {
				sch = ' ';
			}
		} else {
			if(sig)
				sch = '-';
		}
	}

	/* Shift */
	fch = ' ';
	pos++;
	if(prec > 0) {
		len = (prec - pos);
		if(len > 0)
			sh += len;

		if( CHECK_BIT(flags, FLAG_ZPAD) )
			fch = '0';
	}
	if(sh > 0) {
		for(i=pos; i>=0; i--)
			ndest[i + sh] = ndest[i];

		for(i=0; i<sh; i++)
			ndest[i] = fch;

		if( sch ) {
			if( CHECK_BIT(flags, FLAG_ZPAD) )
				ndest[0] = sch;
			else
				ndest[sh - 1] = sch;
		}
	}

	*dest += (pos + sh);
}


int vsprintf(char *str, const char *format, va_list ap)
{
	char *fmt  = (char *)format;
	char *nstr = str;
	char flags = 0;
	char num[MAXDIG], *src;
	int p;

	while(*fmt) {
		if(*fmt != '%') {
			*nstr++ = *fmt++;
		} else {
			/* conversion specification */
			while(*fmt++) {
				/* Flags */
				rflags:
					switch(*fmt) {
						case '#':
							SET_BIT(flags, FLAG_ALT);
							fmt++;
							goto rflags;

						case '0':
							SET_BIT(flags, FLAG_ZPAD);
							fmt++;
							goto rflags;

						case '-':
							SET_BIT(flags, FLAG_FB);
							fmt++;
							goto rflags;

						case ' ':
							SET_BIT(flags, FLAG_SPACE);
							fmt++;
							goto rflags;

						case '+':
							SET_BIT(flags, FLAG_PLUS);
							fmt++;
							goto rflags;
					}

				/* Field width */
				p = 0;
				while(*fmt && (*fmt >= '0' && *fmt <= '9'))
					num[p++] = *fmt++;
				num[p] = '\0';

				/* Precision */
				if(*fmt == '.') {
					fmt++;
					while(*fmt && (*fmt >= '0' && *fmt <= '9'))
						num[p++] = *fmt++;
					num[p] = '\0';
				}

				/* Length modifier */
				switch(*fmt) {
					case 'l':
						fmt++;
						break;
				}

				/* Modifier */
				switch(*fmt) {
					case 'i':
					case 'd':
						numtostr(&nstr, flags, (int)(va_arg(ap, int)), 10, atoi(num), SIGNED);
						fmt++;
						break;

					case 'u':
						numtostr(&nstr, flags, (unsigned int)(va_arg(ap, unsigned int)), 10, atoi(num), UNSIGNED);
						fmt++;
						break;

					case 'o':
						numtostr(&nstr, flags, (int)(va_arg(ap, int)), 8, atoi(num), SIGNED);
						fmt++;
						break;

					case 'x':
						numtostr(&nstr, flags, (int)(va_arg(ap, int)), 16, atoi(num), SIGNED);
						fmt++;
						break;

					case 'c':
						*nstr++ = (int)(va_arg(ap, int));
						fmt++;
						break;

					case 's':
						src = (char*)(va_arg(ap, char*));
						while(*src)
							*nstr++ = *src++;
						fmt++;
						break;

				}
				break;
			}
		}
	}
	*nstr = '\0';

	return(nstr - str);
}


int sprintf(char *str, const char *format, ...)
{
	va_list args;
	int res;

	va_start(args, format);
	res = vsprintf(str, format, args);
	va_end(args);
	return(res);
}


/**
 * This is a temporary implementation
 * TODO: final implementation
 */
int kprintf(const char *format, ...)
{
	char str[4096];
	va_list args;
	int res, offset;

	if(format[0] == '!')
		offset = 3;
	else
		offset = 0;

	va_start(args, format);
	res = vsprintf(str, &format[offset], args);
	va_end(args);

	kprint(str);

	return(res);
}

