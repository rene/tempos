#include "minilibc.h"

long write(int fd, void *buf, unsigned long count)
{
	long res;

	asm volatile("int $0x85"
		: "=a" (res)
		: "a" (4), "b"(fd), "c"(buf), "d"(count) 
		: "memory"
		);

	return res;
}

long read(int fd, void *buf, unsigned long count)
{
	long res;

	asm volatile("int $0x85"
		: "=a" (res)
		: "a" (3), "b"(fd), "c"(buf), "d"(count) 
		: "memory"
		);

	return res;
}

