
#include <tempos/kernel.h>
#include "multiboot.h"
#include "video.h"

void karch(unsigned long magic, unsigned long addr)
{

	clrscr();
	kprintf("Estou no kmain!\n");

	tempos_main();
}

