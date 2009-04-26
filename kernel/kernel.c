/**
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purposing Operating System
 *
 * File: kernel.c
 * Desc: The TempOS kernel
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
#include <tempos/mm.h>
#include <x86/mm.h>
#include <string.h>


/* information passed from first stage */
karch_t kinfo;

extern uint32_t *kpagedir;


/**
 * tempos_main
 *
 * This is the function called when first stage is done, which means that
 * all dependent machine boot code was executed. See arch/$ARCH/boot/karch.c
 */
void tempos_main(karch_t kinf)
{
	uint32_t *mymem;
	uint32_t i;
	char *ola  = "OLA MUNDO DO TEMPOS\n";
	char *ola2 = "Rene\n";
	char *p1, *p2, *p3, *p4;

	memcpy(&kinfo, &kinf, sizeof(karch_t));

	kprintf("We are in TempOS kernel!\n");
	kprintf("Command line passed: %s\n", kinfo.cmdline);

	p1 = (char *)kmalloc(sizeof(char) * 52000, GFP_NORMAL_Z);
	if(p1 == 0) {
		kprintf("ERRO\n");
	} else {
		for(i=0; i<52000; i++) {
			p1[i] = 'A';
		}
		p1[10] = '\0';
	}

	p2 = (char *)kmalloc(sizeof(char) * strlen(ola2), GFP_NORMAL_Z);
	strcpy(p2, ola2);

	p3 = (char *)kmalloc(sizeof(char) * strlen(ola), GFP_NORMAL_Z);
	strcpy(p3, ola);

	p4 = (char *)kmalloc(sizeof(char) * strlen(ola2), GFP_NORMAL_Z);
	strcpy(p4, ola2);

	kprintf(">%ld -- %s\n", (uint32_t)p1, p1);
	kprintf(">%ld -- %s\n", (uint32_t)p2, p2);
	kprintf(">%ld -- %s\n", (uint32_t)p3, p3);
	kprintf(">%ld -- %s\n", (uint32_t)p4, p4);

	kprintf("Deu tudo certo!\n");

	for(;;);
}


void panic(const char *str)
{
	kprintf(KERN_CRIT "%s\n", str);
	for(;;);
}

