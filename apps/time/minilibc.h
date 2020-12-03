
#ifndef _MINILIBC
#define _MINILIBC

#include "stdarg.h"

#define CHECK_BIT(a, b)		((a >> b) & 0x01)
#define SET_BIT(a, b)		a |= (0x01 << b)

long write(int fd, void *buf, unsigned long count);
long read(int fd, void *buf, unsigned long count);
int vsprintf(char *str, const char *format, va_list ap);
int sprintf(char *str, const char *format, ...);
int _printf(const char *format, ...);
int atoi(const char *nptr);

#endif

